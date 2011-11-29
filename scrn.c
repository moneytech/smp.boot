#include "system.h"
#include "cpu.h"

#ifndef EARLY
#include "sync.h"
#endif

/*
 * This file is used in 32-Bit Boot Code (REAL MODE) called from startXX.asm and boot32.c
 * as well as in the final kernel (main.c etc.) for 32- and 64-bit mode.
 */

/* These define our textpointer, our background and foreground
*  colors (attributes), and x and y cursor coordinates */
static uint16_t *textmemptr;
static uint8_t attrib = 0x0F;
static uint8_t attrib_status = 0x1F;
static uint8_t csr_x = 0, csr_y = 0;
static uint8_t screen_h = 25;              /* screen height (currently constant, but may be set according to multiboot info) */
static uint8_t screen_w = 80;              /* screen width  (currently constant, but may be set according to multiboot info) */

/* Scrolls the screen */
void scroll(void)
{
    uint16_t blank, temp;

    /* A blank is defined as a space... we need to give it
    *  backcolor too */
    blank = 0x20 | (attrib << 8);

    /* Row screen_h ( = 25) is the end, this means we need to scroll up */
    if(csr_y >= screen_h)
    {
        /* Move the current text chunk that makes up the screen
        *  back in the buffer by a line */
        temp = csr_y - screen_h + 1;         /* skip first line (used as status monitor) */
        memcpy ((uint16_t*)textmemptr + screen_w, (uint16_t*)textmemptr + (temp+1) * screen_w, (screen_h - temp ) * screen_w * 2);

        /* Finally, we set the chunk of memory that occupies
        *  the last line of text to our 'blank' character */
        memsetw (textmemptr + (screen_h - temp) * screen_w, blank, screen_w);
        csr_y = screen_h - 1;
    }
}

/* Updates the hardware cursor: the little blinking line
*  on the screen under the last character pressed! */
void move_csr(void)
{
    unsigned temp;

    /* The equation for finding the index in a linear
    *  chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    temp = csr_y * screen_w + csr_x;

    /* This sends a command to indicies 14 and 15 in the
    *  CRT Control Register of the VGA controller. These
    *  are the high and low bytes of the index that show
    *  where the hardware cursor is to be 'blinking'. To
    *  learn more, you should look up some VGA specific
    *  programming documents. A great start to graphics:
    *  http://www.brackeen.com/home/vga */
    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}

/* Clears the screen */
void cls()
{
    uint16_t blank;
    int i;

    /* Again, we need the 'short' that will be used to
    *  represent a space with color */
    blank = 0x20 | (attrib << 8);

    /* set background color of status line */
    for (i=0; i < screen_w; i++)
        *((uint8_t*)(textmemptr + i)+1) = attrib_status;

    /* Sets the entire screen to spaces in our current
    *  color */
    /* skip first line (used as status monitor) */
    for(i = 1; i < screen_h; i++)
        memsetw (textmemptr + i * screen_w, blank, screen_w);

    /* Update out virtual cursor, and then move the
    *  hardware cursor */
    csr_x = 0;
    csr_y = 0;
    move_csr();
}

/* Puts a single character on the screen */
void putch(char c)
{
    unsigned short *where;
    unsigned att = attrib << 8;

    /* Handle a backspace, by moving the cursor back one space */
    if(c == 0x08)
    {
        if(csr_x != 0) csr_x--;
    }
    /* Handles a tab by incrementing the cursor's x, but only
    *  to a point that will make it divisible by 8 */
    else if(c == 0x09)
    {
        csr_x = (csr_x + 8) & ~(8 - 1);
    }
    /* Handles a 'Carriage Return', which simply brings the
    *  cursor back to the margin */
    else if(c == '\r')
    {
        csr_x = 0;
    }
    /* We handle our newlines the way DOS and the BIOS do: we
    *  treat it as if a 'CR' was also there, so we bring the
    *  cursor to the margin and we increment the 'y' value */
    else if(c == '\n')
    {
        csr_x = 0;
        csr_y++;
    }
    /* Any character greater than and including a space, is a
    *  printable character. The equation for finding the index
    *  in a linear chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    else if(c >= ' ')
    {
        where = textmemptr + (csr_y * screen_w + csr_x);
        *where = c | att;	/* Character AND attributes: color */
        csr_x++;
    }

    /* If the cursor has reached the edge of the screen's width, we
    *  insert a new line in there */
    if(csr_x >= screen_w)
    {
        csr_x = 0;
        csr_y++;
    }

    /* Scroll the screen if needed, and finally move the cursor */
    scroll();
    move_csr();
}

/*
 * This functions puts the character c at the position x in the first line (the status monitor)
 * The cursor (for putch, puts, printf, et. al.) is NOT moved.
 * Scrolling does not touch the status monitor line.
 */
void status_putch(int x, int c)
{
    *((uint16_t*)textmemptr + x) = c | (attrib_status << 8);
}

/* Uses the above routine to output a string... */
void puts(char *text)
{
    int i;

    for (i = 0; i < strlen((const char*)text); i++)
    {
        putch(text[i]);
    }
}

/* Sets the forecolor and backcolor that we will use */
void settextcolor(unsigned char forecolor, unsigned char backcolor)
{
    /* Top 4 bytes are the background, bottom 4 bytes
    *  are the foreground color */
    attrib = (backcolor << 4) | (forecolor & 0x0F);
}

/* Sets our text-mode VGA pointer, then clears the screen for us */
void init_video(void)
{
    textmemptr = (unsigned short *)0xB8000;
    cls();
    csr_y = 1;      /* skip first line (used as status monitor) */
}


////////////////////////////////////////////////////////////////////////////////
// the following is from the multiboot specification
// http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#multiboot_002eh
////////////////////////////////////////////////////////////////////////////////

/* Convert the integer D to a string and save the string in BUF. If
   BASE is equal to 'd', interpret that D is decimal, and if BASE is
   equal to 'x', interpret that D is hexadecimal. */
void itoa (char *buf, int base, long d)
{
  char *p = buf;
  char *p1, *p2;
  unsigned long ud = *((unsigned long*)&d);
  int divisor = 10;

  /* If %d is specified and D is minus, put `-' in the head. */
  if (base == 'd' && d < 0)
    {
      *p++ = '-';
      buf++;
      ud = -d;
    }
  else if (base == 'x')
    divisor = 16;

  /* Divide UD by DIVISOR until UD == 0. */
  do
    {
      int remainder = ud % divisor;

      *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    }
  while (ud /= divisor);

  /* Terminate BUF. */
  *p = 0;

  /* Reverse BUF. */
  p1 = buf;
  p2 = p - 1;
  while (p1 < p2)
    {
      char tmp = *p1;
      *p1 = *p2;
      *p2 = tmp;
      p1++;
      p2--;
    }
}


 /* Format a string and print it on the screen, just like the libc
   function printf. */
#ifndef EARLY
mutex_t mutex_printf = MUTEX_INITIALIZER;
extern unsigned cpu_online;
#endif

void printf (const char *format, ...)
{
  int c;
  char buf[20];
  unsigned long value;
  __builtin_va_list ap;
  __builtin_va_start(ap, format);

# ifndef EARLY
  if (cpu_online > 1) mutex_lock(&mutex_printf);
# endif

  while ((c = *format++) != 0)
    {
      if (c != '%')
        putch (c);
      else
        {
          char *p;
          int len = 0;

          c = *format++;
          while (c >= '0' && c <= '9') {
              len *= 10;
              len += (c-'0');
              c = *format++;
          }
          switch (c)
            {
            case 'd':
            case 'i':
              value = __builtin_va_arg(ap, long);
              goto weiter;
            case 'u':
            case 'x':
              value = __builtin_va_arg(ap, unsigned long);
            weiter:
              itoa (buf, c, value);
              p = buf;
              goto string;
              break;

            case 's':
              p = __builtin_va_arg(ap, char*);
              if (! p)
                p = "(null)";

            string:
              len -= strlen(p);
              while (len > 0) {
                  putch(' ');
                  len--;
              }
              while (*p)
                putch (*p++);
              break;

            default:
              value = __builtin_va_arg(ap, long);
              putch (value);
              break;
            }
        }
    }
    __builtin_va_end(ap);
# ifndef EARLY
  if (cpu_online > 1) mutex_unlock(&mutex_printf);
# endif
}


#include <graphics/basic_graphics.h>

#ifndef NSOS_DEFAULT_FONT_INDEX
#define NSOS_DEFAULT_FONT_INDEX KERNEL_FONT_PIXEL_MPLUS
#endif

const font_info_t *const kernel_fonts[KERNEL_FONT_COUNT] = {
    &font_basic,
    &font_pixel_mplus,
};

static const font_info_t *active_font = kernel_fonts[NSOS_DEFAULT_FONT_INDEX];

void set_active_font(const font_info_t *f) {
  if (f) {
    active_font = f;
  }
}

void set_active_font_by_index(unsigned i) {
  if (i < KERNEL_FONT_COUNT) {
    active_font = kernel_fonts[i];
  }
}

int createColor(int red, int green, int blue) {
  return ((red & 0xFF) << 16) | ((green & 0xff) << 8) | (blue & 0xff);
}

void drawRect(int x1, int y1, int x2, int y2, uint32_t color, int fill) {
  int y = y1;

  while (y <= y2) {
    int x = x1;
    while (x <= x2) {
      if ((x == x1 || x == x2) || (y == y1 || y == y2))
        draw_pixel(x, y, color);
      else if (fill)
        draw_pixel(x, y, color);
      x++;
    }
    y++;
  }
}

void drawLine(int x1, int y1, int x2, int y2, uint32_t color) {
  int dx, dy, p, x, y;

  dx = x2 - x1;
  dy = y2 - y1;
  x = x1;
  y = y1;
  p = 2 * dy - dx;

  while (x < x2) {
    if (p >= 0) {
      draw_pixel(x, y, color);
      y++;
      p = p + 2 * dy - 2 * dx;
    } else {
      draw_pixel(x, y, color);
      p = p + 2 * dy;
    }
    x++;
  }
}

void drawCircle(int x1, int y1, int radius, uint32_t color, int fill) {
  int x = radius;
  int y = 0;
  int err = 0;

  while (x >= y) {
    if (fill) {
      drawLine(x1 - y, y1 + x, x1 + y, y1 + x, color);
      drawLine(x1 - x, y1 + y, x1 + x, y1 - y, color);
      drawLine(x1 - x, y1 - y, x1 + x, y1 - y, color);
      drawLine(x1 - y, y1 - x, x1 + y, y1 - x, color);
    }

    draw_pixel(x1 - y, y1 + x, color);
    draw_pixel(x1 + y, y1 + x, color);
    draw_pixel(x1 - x, y1 + y, color);
    draw_pixel(x1 + x, y1 + y, color);
    draw_pixel(x1 - x, y1 - y, color);
    draw_pixel(x1 + x, y1 - y, color);
    draw_pixel(x1 - y, y1 - x, color);
    draw_pixel(x1 + y, y1 - x, color);

    if (err <= 0) {
      y += 1;
      err += 2 * y + 1;
    }

    if (err > 0) {
      x -= 1;
      err -= 2 * x + 1;
    }
  }
}

void drawChar(unsigned char ch, int x, int y, uint32_t color) {
  const font_info_t *f = active_font;
  unsigned idx = ((unsigned)ch < f->num_glyphs) ? (unsigned)ch : 0u;
  const unsigned char *glyph = f->glyphs + idx * f->bytes_per_glyph;

  for (unsigned i = 0; i < f->height; i++) {
    for (unsigned j = 0; j < f->width; j++) {
      unsigned byte_idx = j >> 3;
      unsigned bit = j & 7u;
      unsigned shift = f->msb_left ? (7u - bit) : bit;
      unsigned char mask = (unsigned char)(1u << shift);
      unsigned char pix = glyph[byte_idx];
      uint32_t col = (mask & pix) ? color : 0;

      draw_pixel((int)(x + (int)j), (int)(y + (int)i), col);
    }
    glyph += f->bytes_per_line;
  }
}

void drawString(char *s, int x, int y, uint32_t color) {
  const font_info_t *f = active_font;
  while (*s) {
    if (*s == '\r') {
      x = 0;
    } else if (*s == '\n') {
      x = 0;
      y += (int)f->line_height;
    } else {
      drawChar(*s, x, y, color);
      x += (int)f->advance_x;
    }
    s++;
  }
}

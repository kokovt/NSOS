#include <graphics/basic_graphics.h>


int createColor(int red, int green, int blue) {
    return ((red & 0xFF) << 16) | ((green & 0xff) << 8) | (blue & 0xff);
}

void drawRect(int x1, int y1, int x2, int y2, uint32_t color,int fill) {
    int y = y1;

    while(y <= y2) {
        int x = x1;
        while(x <= x2) {
            if((x == x1 || x == x2) || (y == y1 || y == y2)) draw_pixel(x, y, color);
            else if(fill) draw_pixel(x,y,color);
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
    p = 2*dy - dx;

    while(x < x2) {
        if(p >= 0) {
            draw_pixel(x,y,color);
            y++;
            p = p+2*dy-2*dx;
        } else {
            draw_pixel(x,y,color);
            p = p+2*dy;
        }
        x++;
    }
}

void drawCircle(int x1, int y1, int radius, uint32_t color, int fill) {
    int x = radius;
    int y = 0;
    int err = 0;

    while(x >= y) {
        if(fill) {
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
        draw_pixel(x1-y,y1-x,color);
        draw_pixel(x1 + y, y1 - x, color);

        if(err <= 0) {
            y += 1;
            err += 2*y + 1;
        }

        if(err > 0) {
            x -= 1;
            err -= 2*x + 1;
        }
    }
}

void drawChar(unsigned char ch, int x, int y, uint32_t color) {
    unsigned char *glyph = (unsigned char*)&font + (ch < FONT_NUMGLYPHS ? ch : 0) * FONT_BPG;

    for(int i = 0; i < FONT_HEIGHT; i++) {
        for (int j = 0; j < FONT_WIDTH; j++) {
            unsigned char mask = 1 << j;
            uint32_t col = (mask & *glyph) ? color : 0;

            draw_pixel(x+j, y + i, col);
        }
        glyph += FONT_BPL;
    }
}

void drawString(char *s, int x, int y, uint32_t color) {
    while(*s) {
        if(*s == '\r') {
            x = 0;
        } else if(*s == '\n') {
            x = 0; 
            y += FONT_HEIGHT;
        } else {
            drawChar(*s, x, y, color);
            x+=FONT_WIDTH;
        }
        s++;
    }
}


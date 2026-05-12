#pragma once

typedef struct font_info {
    unsigned width;
    unsigned height;
    unsigned advance_x;
    unsigned line_height;
    unsigned bytes_per_line;
    unsigned bytes_per_glyph;
    unsigned num_glyphs;
    unsigned msb_left;
    const unsigned char *glyphs;
} font_info_t;
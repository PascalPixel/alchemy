/* Indexed PNG I/O and GBA tile layouts. Palette indices stay editable and
 * stable. */
#include "graphics.h"
#include <png.h>
#include <ctype.h>

typedef struct {
  const Buffer *buffer;
  size_t at;
} PngInput;
static void pngError(png_structp png, png_const_charp error) {
  (void)png;
  fail("PNG: %s", error);
}

static void pngWarning(png_structp png, png_const_charp error) {
  (void)png;
  fprintf(stderr, "PNG: %s\n", error);
}

static void pngRead(png_structp png, png_bytep bytes, png_size_t count) {
  PngInput *in = png_get_io_ptr(png);
  if (count > in->buffer->len - in->at)
    fail("truncated PNG");
  memcpy(bytes, in->buffer->data + in->at, count);
  in->at += count;
}

static void pngWrite(png_structp png, png_bytep bytes, png_size_t count) {
  append(png_get_io_ptr(png), bytes, count);
}

static void pngFlush(png_structp png) { (void)png; }
Image readImage(const Buffer *data) {
  PngInput in = {data, 0};
  Image image = {0};
  png_structp png =
      png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, pngError, pngWarning);
  if (!png)
    fail("cannot create PNG reader");
  png_infop info = png_create_info_struct(png);
  if (!info)
    fail("cannot create PNG metadata");
  png_set_read_fn(png, &in, pngRead);
  png_read_info(png, info);
  image.width = png_get_image_width(png, info);
  image.height = png_get_image_height(png, info);
  image.depth = png_get_bit_depth(png, info);
  if (!image.width || !image.height ||
      png_get_color_type(png, info) != PNG_COLOR_TYPE_PALETTE ||
      image.depth > 8)
    fail("PNG must use an indexed 1/2/4/8-bit palette");
  png_colorp palette;
  int colors;
  if (!png_get_PLTE(png, info, &palette, &colors) || colors < 1 ||
      colors > 256 || colors > (1 << image.depth))
    fail("invalid PNG palette");
  for (int i = 0; i < colors; i++) {
    byte(&image.palette, palette[i].red);
    byte(&image.palette, palette[i].green);
    byte(&image.palette, palette[i].blue);
  }
  png_bytep alpha;
  int alphaCount;
  png_color_16p transparent;
  if (png_get_tRNS(png, info, &alpha, &alphaCount, &transparent)) {
    if (alphaCount > colors)
      fail("transparency exceeds palette");
    for (int i = 0; i < alphaCount; i++) {
      if (alpha[i] != 0 && alpha[i] != 255)
        fail("transparency must contain only binary alpha");
      byte(&image.alpha, alpha[i]);
    }
  }
  png_set_packing(png);
  png_set_interlace_handling(png);
  png_read_update_info(png, info);
  if (image.height > SIZE_MAX / image.width)
    fail("PNG dimensions overflow");
  size_t n = image.width * image.height;
  reserve(&image.pixels, n);
  image.pixels.len = n;
  png_bytep *rows = allocate(image.height, sizeof(png_bytep));
  for (size_t y = 0; y < image.height; y++)
    rows[y] = image.pixels.data + y * image.width;
  png_read_image(png, rows);
  png_read_end(png, info);
  free(rows);
  png_destroy_read_struct(&png, &info, NULL);
  for (size_t i = 0; i < n; i++)
    if (image.pixels.data[i] >= colors)
      fail("pixel references a missing palette entry");
  return image;
}

Buffer writeImage(const Image *image) {
  Buffer out = {0};
  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL,
                                            pngError, pngWarning);
  if (!png)
    fail("cannot create PNG writer");
  png_infop info = png_create_info_struct(png);
  if (!info)
    fail("cannot create PNG metadata");
  png_set_write_fn(png, &out, pngWrite, pngFlush);
  png_set_IHDR(png, info, (png_uint_32)image->width, (png_uint_32)image->height,
               image->depth, PNG_COLOR_TYPE_PALETTE, PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
  png_color palette[256];
  size_t colors = image->palette.len / 3;
  for (size_t i = 0; i < colors; i++)
    palette[i] =
        (png_color){image->palette.data[3 * i], image->palette.data[3 * i + 1],
                    image->palette.data[3 * i + 2]};
  png_set_PLTE(png, info, palette, (int)colors);
  if (image->alpha.len)
    png_set_tRNS(png, info, image->alpha.data, (int)image->alpha.len, NULL);
  png_write_info(png, info);
  png_set_packing(png);
  for (size_t y = 0; y < image->height; y++)
    png_write_row(png, image->pixels.data + y * image->width);
  png_write_end(png, info);
  png_destroy_write_struct(&png, &info);
  return out;
}

void freeImage(Image *i) {
  free(i->pixels.data);
  free(i->palette.data);
  free(i->alpha.data);
  *i = (Image){0};
}

Buffer gbaPalette(const Image *i) {
  Buffer out = {0};
  for (size_t n = 0; n < i->alpha.len; n++)
    if (i->alpha.data[n] != 255)
      fail("GBA palettes cannot represent transparent PNG entries");
  for (size_t n = 0; n < i->palette.len; n++)
    if (i->palette.data[n] & 7)
      fail("palette channels must be multiples of eight");
  for (size_t n = 0; n < i->palette.len; n += 3)
    half(&out, (i->palette.data[n] >> 3) | (i->palette.data[n + 1] >> 3) << 5 |
                   (i->palette.data[n + 2] >> 3) << 10);
  return out;
}

bool tileForm(const char *form, unsigned *depth, size_t *width,
              size_t *height) {
  if (strncmp(form, "4bpp", 4) && strncmp(form, "8bpp", 4))
    return false;
  *depth = form[0] - '0';
  *width = *height = 8;
  if (!form[4])
    return true;
  unsigned w, h;
  char extra;
  if (sscanf(form + 4, "%ux%u%c", &w, &h, &extra) != 2)
    return false;
  const unsigned shapes[][2] = {{8, 8},  {16, 16}, {32, 32}, {64, 64},
                                {16, 8}, {32, 8},  {32, 16}, {64, 32},
                                {8, 16}, {8, 32},  {16, 32}, {32, 64}};
  for (size_t i = 0; i < 12; i++)
    if (shapes[i][0] == w && shapes[i][1] == h) {
      *width = w;
      *height = h;
      return true;
    }
  return false;
}

Buffer tiles(const Image *image, unsigned depth, size_t mw, size_t mh) {
  if (!mw || !mh || mw % 8 || mh % 8 || image->width % mw || image->height % mh)
    fail("image is not whole metatiles");
  Buffer out = {0};
  for (size_t my = 0; my < image->height; my += mh)
    for (size_t mx = 0; mx < image->width; mx += mw)
      for (size_t ty = 0; ty < mh; ty += 8)
        for (size_t tx = 0; tx < mw; tx += 8)
          for (size_t y = 0; y < 8; y++)
            for (size_t x = 0; x < 8; x += depth == 4 ? 2 : 1) {
              const uint8_t *p = image->pixels.data +
                                 (my + ty + y) * image->width + mx + tx + x;
              byte(&out, depth == 4 ? (p[0] & 15) | (p[1] & 15) << 4 : p[0]);
            }
  return out;
}

char *sibling(const char *input, const char *name) {
  char *lower = copyString(name);
  for (char *p = lower; *p; p++)
    *p = tolower((unsigned char)*p);
  if (strncmp(lower, "common/", 7)) {
    char *path = beside(input, lower);
    free(lower);
    return path;
  }
  const char *shared = lower + 7;
  Strings parts = split(shared, '/');
  for (size_t i = 0; i < parts.len; i++)
    if (!*parts.items[i] || !strcmp(parts.items[i], ".") ||
        !strcmp(parts.items[i], "..") || strchr(parts.items[i], '\\'))
      fail("invalid common asset path: %s", name);
  freeStrings(&parts);
  char *folder = beside(input, "");
  char *src = strstr(folder, "/src/");
  if (!src)
    fail("%s lies under no game's src", input);
  char *suffix = copyString(src + 1);
  *src = 0;
  char *game = strrchr(folder, '/');
  if (!game)
    fail("invalid game path: %s", input);
  *game = 0;
  char *path = strchr(shared, '/')
                   ? formatString("%s/common/src/%s", folder, shared)
                   : formatString("%s/common/%s%s", folder, suffix, shared);
  free(folder);
  free(suffix);
  free(lower);
  return path;
}

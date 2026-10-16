/* Encode an editable PNG or TSV. The output suffix chooses its game format. */
#include "graphics.h"

static const char *option(int argc, char **argv, const char *name) {
  for (int i = 3; i < argc; i++)
    if (!strcmp(argv[i], name)) {
      if (i + 1 == argc)
        fail("%s needs a value", name);
      return argv[i + 1];
    }
  return NULL;
}

int main(int argc, char **argv) {
  const char *usage =
      "usage: ags-graphics INPUT OUTPUT [-mwidth N -mheight N] [--lz KIND "
      "--machine W,R,D,P]\n       ags-graphics INPUT OUTPUT.png --from FORM "
      "--to FORM [--wide N] [--palette PNG]";
  startTool(argc, argv, usage);
  if (argc < 3)
    fail("%s", usage);
  const char *input = argv[1], *output = argv[2],
             *codecOption = option(argc, argv, "--lz");
  Buffer out = {0};
  const char *allowed[] = {"-mwidth", "-mheight", "--lz",   "--machine",
                           "--from",  "--to",     "--wide", "--palette"};
  for (int i = 3; i < argc; i += 2) {
    bool valid = false;
    for (size_t j = 0; j < 8; j++)
      valid |= !strcmp(argv[i], allowed[j]);
    if (!valid || i + 1 == argc)
      fail("unknown or incomplete option: %s", argv[i]);
  }
  if (!strcmp(extension(output), "png")) {
    const char *wide = option(argc, argv, "--wide");
    size_t width = wide ? number(wide, 10, SIZE_MAX) : SIZE_MAX;
    out =
        redraw(input, option(argc, argv, "--from"), option(argc, argv, "--to"),
               width, option(argc, argv, "--palette"));
  } else if (!strcmp(extension(input), "png") &&
             (!strcmp(extension(output), "4bpp") ||
              !strcmp(extension(output), "8bpp"))) {
    const char *w = option(argc, argv, "-mwidth"),
               *h = option(argc, argv, "-mheight");
    size_t mw = w ? number(w, 10, SIZE_MAX / 8) : 1,
           mh = h ? number(h, 10, SIZE_MAX / 8) : 1;
    Buffer png = readFile(input);
    Image image = readImage(&png);
    out = tiles(&image, extension(output)[0] - '0', mw * 8, mh * 8);
    freeImage(&image);
    free(png.data);
  } else if (codecOption && strcmp(extension(input), "png")) {
    if (strcmp(extension(output), "lz"))
      fail("explicit LZ conversion needs an .lz output");
    LzMachine machine = packer;
    if (strcmp(codecOption, "mtf4")) {
      const char *text = option(argc, argv, "--machine");
      if (!text)
        fail("LZ needs --machine W,R,D,P");
      Strings values = split(text, ',');
      if (values.len != 4)
        fail("LZ machine needs four numbers");
      machine = (LzMachine){number(trim(values.items[0]), 10, SIZE_MAX),
                            number(trim(values.items[1]), 10, SIZE_MAX),
                            number(trim(values.items[2]), 10, SIZE_MAX),
                            number(trim(values.items[3]), 10, SIZE_MAX)};
      freeStrings(&values);
    }
    Buffer data = readFile(input);
    out = compressLz(&data, &data, codecOption, machine);
    free(data.data);
  } else {
    const char *base = strrchr(output, '/');
    base = base ? base + 1 : output;
    Strings parts = split(base, '.');
    if ((parts.len != 2 && parts.len != 3) || !*parts.items[0])
      fail("output must name STEM.FORM[.CODEC]");
    out = buildResource(input, parts.items[1],
                        parts.len == 3 ? parts.items[2] : NULL, 0);
    freeStrings(&parts);
  }
  writeFile(output, &out);
  free(out.data);
  return 0;
}

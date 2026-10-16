/* Pack an overlay after converting Thumb calls to the loader's stored form. */
#include "../common/compress.h"
int main(int argc, char **argv) {
  const char *usage = "usage: ags-overlays OVERLAY.bin OVERLAY.lz";
  startTool(argc, argv, usage);
  if (argc != 3)
    fail("%s", usage);
  Buffer data = readFile(argv[1]);
  /* The loader restores relative BL displacements after decompression. */
  for (size_t at = 2; at + 2 <= data.len; at += 2) {
    uint32_t hi = read16(data.data + at - 2), lo = read16(data.data + at);
    if (lo >> 11 != 31 || hi >> 11 != 30)
      continue;
    uint32_t call = (hi & 0x7ff) << 12 | (lo & 0x7ff) << 1;
    uint32_t stored = (call + (uint32_t)at) & 0x7ffffe;
    write16(data.data + at - 2, 0xf000 | stored >> 12);
    write16(data.data + at, 0xf800 | (stored >> 1 & 0x7ff));
  }
  Buffer out = compressLz(&data, &data, "tagged", packer);
  writeFile(argv[2], &out);
  free(data.data);
  free(out.data);
  return 0;
}

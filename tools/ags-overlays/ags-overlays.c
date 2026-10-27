/*
 * ags-overlays.c
 *
 * Pack overlays using the loader's Thumb-call filter.
 * Convert relative BL displacements to stored positions before LZ compression.
 * The loader reverses this filter after decompressing the overlay.
 */
#include "../shared/compress.h"
/* Store absolute call positions modulo the Thumb displacement width. */
static void
filterCalls(Buffer *data)
{
	for (size_t at = 2; at + 2 <= data->len; at += 2)
	{
		uint32_t hi = read16(data->data + at - 2), lo = read16(data->data + at);
		if (lo >> 11 != 31 || hi >> 11 != 30)
			continue;
		uint32_t call = (hi & 0x7ff) << 12 | (lo & 0x7ff) << 1;
		uint32_t stored = (call + (uint32_t) at) & 0x7ffffe;
		write16(data->data + at - 2, 0xf000 | stored >> 12);
		write16(data->data + at, 0xf800 | (stored >> 1 & 0x7ff));
	}
}

int
main(int argc, char **argv)
{
	const char *usage = "usage: ags-overlays OVERLAY.bin OVERLAY.lz";
	startTool(argc, argv, usage);
	if (argc != 3)
		fail("%s", usage);
	Buffer data = readFile(argv[1]);
	/* The loader restores relative BL displacements after decompression. */
	filterCalls(&data);
	Buffer out = compressLz(&data, &data, "tagged", packer);
	writeFile(argv[2], &out);
	free(data.data);
	free(out.data);
	return 0;
}

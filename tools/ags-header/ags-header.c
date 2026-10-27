/*
 * ags-header.c
 *
 * Fix cartridge header fields and optional ROM padding.
 * The logo and complement are written here, after linking. User-supplied title,
 * code, maker, and version replace only their header fields; padding extends
 * the image to a power of two without changing its existing payload.
 */
#include "../shared/shared.h"
static const uint8_t logo[156] = {
	0x24, 0xff, 0xae, 0x51, 0x69, 0x9a, 0xa2, 0x21, 0x3d, 0x84, 0x82, 0x0a,
	0x84, 0xe4, 0x09, 0xad, 0x11, 0x24, 0x8b, 0x98, 0xc0, 0x81, 0x7f, 0x21,
	0xa3, 0x52, 0xbe, 0x19, 0x93, 0x09, 0xce, 0x20, 0x10, 0x46, 0x4a, 0x4a,
	0xf8, 0x27, 0x31, 0xec, 0x58, 0xc7, 0xe8, 0x33, 0x82, 0xe3, 0xce, 0xbf,
	0x85, 0xf4, 0xdf, 0x94, 0xce, 0x4b, 0x09, 0xc1, 0x94, 0x56, 0x8a, 0xc0,
	0x13, 0x72, 0xa7, 0xfc, 0x9f, 0x84, 0x4d, 0x73, 0xa3, 0xca, 0x9a, 0x61,
	0x58, 0x97, 0xa3, 0x27, 0xfc, 0x03, 0x98, 0x76, 0x23, 0x1d, 0xc7, 0x61,
	0x03, 0x04, 0xae, 0x56, 0xbf, 0x38, 0x84, 0x00, 0x40, 0xa7, 0x0e, 0xfd,
	0xff, 0x52, 0xfe, 0x03, 0x6f, 0x95, 0x30, 0xf1, 0x97, 0xfb, 0xc0, 0x85,
	0x60, 0xd6, 0x80, 0x25, 0xa9, 0x63, 0xbe, 0x03, 0x01, 0x4e, 0x38, 0xe2,
	0xf9, 0xa2, 0x34, 0xff, 0xbb, 0x3e, 0x03, 0x44, 0x78, 0x00, 0x90, 0xcb,
	0x88, 0x11, 0x3a, 0x94, 0x65, 0xc0, 0x7c, 0x63, 0x87, 0xf0, 0x3c, 0xaf,
	0xd6, 0x25, 0xe4, 0x8b, 0x38, 0x0a, 0xac, 0x72, 0x21, 0xd4, 0xf8, 0x07,
};
/*
 * Header strings have fixed widths and no required terminator when full.
 */
static void
field(uint8_t *out, size_t size, const char *value)
{
	memset(out, 0, size);
	size_t n = strlen(value);
	if (n > size)
		n = size;
	memcpy(out, value, n);
}

/* The complement covers title through version; the logo is outside that sum. */
static void
fixHeader(uint8_t header[192])
{
	memcpy(header + 4, logo, sizeof(logo));
	header[0xb2] = 0x96;
	header[0xb4] = 0;
	header[0xbe] = header[0xbf] = 0;
	unsigned sum = 0x19;
	for (size_t i = 0xa0; i < 0xbd; i++)
		sum += header[i];
	header[0xbd] = -sum;
}

int
main(int argc, char **argv)
{
	const char *usage = "usage: ags-header ROM [-p] [-t[TITLE]] [-cCODE] "
						"[-mMAKER] [-rVERSION] [--silent]";
	startTool(argc, argv, usage);
	if (argc < 2 || argv[1][0] == '-')
		fail("%s", usage);
	Buffer data = readFile(argv[1]);
	uint8_t header[192] = {0};
	memcpy(header, data.data,
		   data.len < sizeof(header) ? data.len : sizeof(header));
	bool pad = false, silent = false;
	for (int i = 2; i < argc; i++)
	{
		const char *arg = argv[i];
		if (!strcmp(arg, "-p"))
		{
			pad = true;
			continue;
		}
		if (!strcmp(arg, "--silent"))
		{
			silent = true;
			continue;
		}
		if (!strcmp(arg, "--help") || !strcmp(arg, "-h"))
		{
			puts(usage);
			free(data.data);
			return 0;
		}
		if (arg[0] != '-' || !arg[1] || !strchr("tcmr", arg[1]))
			fail("invalid option: %s", arg);
		const char *value = arg + 2;
		char *title = NULL;
		if (!*value && i + 1 < argc && argv[i + 1][0] != '-')
			value = argv[++i];
		if (!*value && arg[1] == 't')
		{
			const char *name = strrchr(argv[1], '/');
			title = copyString(name ? name + 1 : argv[1]);
			char *dot = strrchr(title, '.');
			if (dot)
				*dot = 0;
			value = title;
		}
		else if (!*value)
			fail("%s needs a value", arg);
		switch (arg[1])
		{
		case 't':
			field(header + 0xa0, 12, value);
			break;
		case 'c':
			field(header + 0xac, 4, value);
			break;
		case 'm':
			field(header + 0xb0, 2, value);
			break;
		case 'r':
			header[0xbc] = number(value, 0, UINT64_MAX);
			break;
		}
		free(title);
	}
	fixHeader(header);
	if (pad && data.len)
	{
		size_t n = 1;
		while (n < data.len)
		{
			if (n > SIZE_MAX / 2)
				fail("ROM too large");
			n *= 2;
		}
		while (data.len < n)
			byte(&data, 0xff);
	}
	while (data.len < sizeof(header))
		byte(&data, 0);
	memcpy(data.data, header, sizeof(header));
	writeFile(argv[1], &data);
	free(data.data);
	if (!silent)
		puts("ROM fixed!");
	return 0;
}

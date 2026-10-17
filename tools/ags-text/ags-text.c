/*
 * ags-text.c
 *
 * Encode an editable PO catalogue as assembly or C message declarations.
 * Catalogue parsing, symbol conversion, and context compression have separate
 * owners. Both output modes validate message text before replacing a file.
 */
#include "text.h"

int
main(int argc, char **argv)
{
	const char *usage =
		"usage: ags-text CATALOG.po OUTPUT.inc|OUTPUT.h [--label NAME] "
		"[--header] [--as-value] [--charmap FILE]";
	startTool(argc, argv, usage);
	if (argc < 3)
		fail("%s", usage);
	bool header = false, asValue = false;
	const char *label = "Text_Message", *mapPath = NULL;
	for (int i = 3; i < argc; i++)
	{
		if (!strcmp(argv[i], "--header"))
			header = true;
		else if (!strcmp(argv[i], "--as-value"))
			asValue = true;
		else if (!strcmp(argv[i], "--label") && i + 1 < argc)
			label = argv[++i];
		else if (!strcmp(argv[i], "--charmap") && i + 1 < argc)
			mapPath = argv[++i];
		else
			fail("unknown or incomplete option: %s", argv[i]);
	}
	Catalog c = readCatalog(argv[1]);
	encodeCatalog(&c, mapPath);
	Buffer out = {0};
	if (header)
	{
		print(&out,
			  "/* Message numbers from the edition's editable PO catalog. "
			  "*/\n#ifndef ALCHEMY_MESSAGE_IDS_H\n#define "
			  "ALCHEMY_MESSAGE_IDS_H\n#include \"types.h\"\n#define "
			  "MSGNO(name) TEXT_MESSAGE_NUMBER_ ## name\n");
		for (size_t i = 0; i < c.count; i++)
			if (strcmp(c.messages[i].name, "message"))
				print(&out, "#define TEXT_MESSAGE_NUMBER_%s %llu\n",
					  c.messages[i].name,
					  (unsigned long long) number(c.messages[i].id, 10, 65535));
		print(&out, "#define TEXT_MESSAGE_NUMBER_MsgEnd %zu\n", c.count);
		if (asValue)
			print(
				&out,
				"/* A message's name is its linker symbol, whose value is the "
				"message's\n * number. C can name a link-time value only as an "
				"address, so each\n * name below stands for that address read "
				"as an "
				"s32. A macro is not\n * expanded again inside its own "
				"replacement "
				"(C89 3.8.3.4): the name in\n * the cast is the symbol, which "
				"the "
				"code still loads from a relocated\n * pool word. */\n");
		for (size_t i = 0; i <= c.count; i++)
		{
			const char *name = i == c.count ? "MsgEnd" : c.messages[i].name;
			if (!strcmp(name, "message"))
				continue;
			print(&out, "extern u8 %s[];\n", name);
			if (asValue)
				print(&out, "#define %s ((s32)%s)\n", name, name);
		}
		print(&out, "#endif\n");
	}
	else
	{
		print(&out,
			  "@ %s's message archive and message numbers, built from %s.\n",
			  c.target, argv[1]);
		Assembly a = messageArchive(&c, label);
		appendAssemblySource(&out, &a);
		freeAssembly(&a);
		for (size_t i = 0; i < c.count; i++)
			if (strcmp(c.messages[i].name, "message"))
				print(&out, "\t.global %s\n\t.set %s, %llu\n",
					  c.messages[i].name, c.messages[i].name,
					  (unsigned long long) number(c.messages[i].id, 10, 65535));
		print(&out, "\t.global MsgEnd\n\t.set MsgEnd, %zu\n", c.count);
	}
	writeChanged(argv[2], &out);
	free(out.data);
	freeCatalog(&c);
	return 0;
}

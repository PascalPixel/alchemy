/*
 * text.h
 *
 * Catalogue shared by PO input, symbol encoding, and archive output. The
 * catalogue owns its messages and their encoded symbols until freeCatalog.
 */
#ifndef AGS_TEXT_H
#define AGS_TEXT_H

#include "../common/assembly.h"

typedef struct
{
	char *name, *id, *text;
	bool null;
	Buffer symbols;
} Message;

typedef struct
{
	Message *messages;
	size_t count, alphabet;
	char *target;
} Catalog;

Catalog readCatalog(const char *path);
void freeCatalog(Catalog *catalog);
void encodeCatalog(Catalog *catalog, const char *mapPath);
Assembly messageArchive(Catalog *catalog, const char *label);

#endif

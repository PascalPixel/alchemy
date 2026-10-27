/*
 * files.c
 *
 * File ownership, atomic output, and paths shared by the host tools.
 * Read buffers belong to the caller. Writes use a temporary sibling and rename
 * so concurrent edition builds cannot observe a partially written shared asset.
 * writeChanged also preserves an existing identical file and its timestamp.
 */
#define _POSIX_C_SOURCE 200809L
#include "shared.h"
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

static Strings sRead;

/*
 * Read to EOF rather than trusting a size from stat, then keep a spare NUL
 * so the same buffer can be parsed as text.
 */
Buffer
readFile(const char *path)
{
	FILE *file = fopen(path, "rb");
	if (!file)
		fail("%s: %s", path, strerror(errno));
	Buffer b = {0};
	uint8_t chunk[16384];
	size_t n;
	while ((n = fread(chunk, 1, sizeof(chunk), file)))
		append(&b, chunk, n);
	if (ferror(file))
		fail("%s: read failed", path);
	fclose(file);
	reserve(&b, 0);
	addUniqueString(&sRead, path);
	return b;
}

/* An encoder lists these as its output's prerequisites (ags-graphics). */
const Strings *
filesRead(void)
{
	return &sRead;
}

bool
exists(const char *path)
{
	struct stat s;
	return !stat(path, &s) && S_ISREG(s.st_mode);
}

void
makeParents(const char *path)
{
	if (!*path)
		fail("output path is empty");
	char *copy = copyString(path);
	for (char *p = copy + 1; *p; p++)
		if (*p == '/')
		{
			*p = 0;
			if (mkdir(copy, 0777) && errno != EEXIST)
				fail("%s: %s", copy, strerror(errno));
			*p = '/';
		}
	free(copy);
}

/* Editions may encode a shared asset at the same time. Rename a complete
 * sibling file so readers never see a partially written asset. */
void
writeFile(const char *path, const Buffer *buffer)
{
	char *temp = formatString("%s.XXXXXX", path);
	int fd = mkstemp(temp);
	if (fd < 0)
		fail("%s: %s", path, strerror(errno));
	FILE *file = fdopen(fd, "wb");
	if (!file)
	{
		close(fd);
		unlink(temp);
		fail("%s: open failed", path);
	}
	bool ok = !buffer->len ||
			  fwrite(buffer->data, 1, buffer->len, file) == buffer->len;
	if (fclose(file))
		ok = false;
	if (!ok || rename(temp, path))
	{
		unlink(temp);
		fail("%s: write failed", path);
	}
	free(temp);
}

/*
 * Do not disturb timestamps when several editions generate the same include.
 */
void
writeChanged(const char *path, const Buffer *buffer)
{
	if (exists(path))
	{
		Buffer old = readFile(path);
		bool same =
			old.len == buffer->len &&
			(!buffer->len || !memcmp(old.data, buffer->data, buffer->len));
		free(old.data);
		if (same)
			return;
	}
	makeParents(path);
	writeFile(path, buffer);
}

char *
beside(const char *path, const char *name)
{
	const char *slash = strrchr(path, '/');
	return slash ? formatString("%.*s/%s", (int) (slash - path), path, name)
				 : copyString(name);
}

const char *
extension(const char *path)
{
	const char *p = strrchr(path, '.');
	return p ? p + 1 : "";
}

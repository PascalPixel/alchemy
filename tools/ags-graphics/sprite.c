/*
 * sprite.c
 *
 * Build sprite records, shared animations, and picture banks from tables.
 * sprites.tsv has a row for each sprite: its sizes, and the sprite whose
 * pictures and whose animations it uses, itself or another, or neither; a
 * sprite that uses only the first of another's animations says how many.
 * An edition's bank, sprites_ja, is built from the same table, and draws
 * NAME_ja.png and reads NAME_ja.tsv wherever a sprite has them. The bank owns
 * all it reads; labels and pointers keep the sharing in either layout.
 */
#include "resource.h"
#include "../shared/assembly.h"

typedef struct
{
	char *name, *pictures, *animations;
	size_t animationLimit;
	uint8_t bytes[12];
	Buffer *scripts;
	size_t scriptCount;
	Strings animationList, pictureList, empty;
	bool hasAnimations, hasPictures;
} Sprite;
typedef struct
{
	Sprite *sprites;
	size_t count;
	const char *input, *edition;
	Strings pictures;
} Bank;

static Sprite *
find(Bank *bank, const char *name)
{
	for (size_t i = 0; i < bank->count; i++)
		if (!strcmp(bank->sprites[i].name, name))
			return bank->sprites + i;
	fail("sprite %s has no record", name);
}

static bool
owns(const Sprite *sprite, const char *list)
{
	return list && !strcmp(sprite->name, list);
}

static unsigned
integer(const char *s, bool sign, unsigned max)
{
	bool negative = *s == '-';
	if (negative)
		s++;
	uint64_t v = number(s, 10, sign ? (negative ? 128 : 127) : max);
	if (negative && !sign)
		fail("negative sprite field");
	return negative ? (unsigned) -v : (unsigned) v;
}

/*
 * An animation script is a list of commands, two bytes each, which the
 * field reads (enum AnimationCommand in games/tbs/src/field/field_internal.h).
 * A row of the sprite's table is one of them:
 *
 *   picture N D   show picture N for D frames (N below 239)
 *   wait D        keep the current picture D frames longer
 *   blank D       show nothing for D frames
 *   kind N        use facing table N
 *   play N        switch to animation N
 *   loop          continue at the first command
 *   jump N        continue at command N
 *   hold          stay on this command for good
 *   end           blank the sprite and stop
 *   command C A   a command with no name here, C, with argument A
 *
 * hold and end take an optional argument, which the field ignores.
 */
static const struct
{
	const char *name;
	unsigned code;
	bool optional;
} animationCommands[] = {
	{"end", 239, true},	  {"kind", 240, false}, {"hold", 241, true},
	{"wait", 245, false}, {"jump", 253, false}, {"play", 254, false},
	{"blank", 255, false},
};
enum
{
	ANIMATION_COMMAND_COUNT =
		sizeof(animationCommands) / sizeof(*animationCommands)
};

static Buffer
animationScript(Strings commands)
{
	Buffer script = {0};
	for (size_t i = 0; i < commands.len; i++)
	{
		Strings words = split(trim(commands.items[i]), ' ');
		const char *name = words.items[0];
		size_t n = words.len;
		unsigned code = 256,
				 argument = n > 1 ? number(words.items[n - 1], 10, 255) : 0;
		if (!strcmp(name, "picture") && n == 3)
			code = number(words.items[1], 10, 238);
		else if (!strcmp(name, "loop") && n == 1)
			code = 253;
		else if (!strcmp(name, "command") && n == 3)
		{
			code = number(words.items[1], 10, 255);
			for (size_t j = 0; j < ANIMATION_COMMAND_COUNT; j++)
				if (code < 239 || animationCommands[j].code == code)
					fail("command %u has a name", code);
		}
		for (size_t j = 0; code == 256 && j < ANIMATION_COMMAND_COUNT; j++)
			if (!strcmp(name, animationCommands[j].name) &&
				(n == 2 || (n == 1 && animationCommands[j].optional)))
				code = animationCommands[j].code;
		if (code == 256)
			fail("unknown animation command: %s", commands.items[i]);
		// The field jumps to a byte offset; the text counts commands.
		if (!strcmp(name, "jump"))
		{
			if (!argument || argument >= commands.len || argument > 127)
				fail("jump %u is the first command or past the end", argument);
			argument *= 2;
		}
		byte(&script, code);
		byte(&script, argument);
		freeStrings(&words);
	}
	freeStrings(&commands);
	return script;
}

/*
 * A sprite's animations as a table, one row a command: the animation it
 * belongs to, counted from 0, the command, its value and its frames, as
 * the commands above take them (picture N D is value N, frames D).
 */
#define ANIMATION_COLUMNS "animation\tcommand\tvalue\tframes"

static void
animationTable(Sprite *sprite, const Strings *lines)
{
	Strings commands = {0};
	char *animation = NULL;
	for (size_t i = 1; i <= lines->len; i++)
	{
		Strings cells = {0};
		if (i < lines->len)
		{
			if (!*trim(lines->items[i]))
				continue;
			cells = split(trim(lines->items[i]), '\t');
			if (cells.len < 2)
				fail("an animation row needs its animation and a command");
		}
		if (commands.len && (i == lines->len || strcmp(cells.items[0], animation)))
		{
			sprite->scripts = resize(sprite->scripts, sprite->scriptCount + 1,
									 sizeof(Buffer));
			sprite->scripts[sprite->scriptCount++] = animationScript(commands);
			commands = (Strings){0};
		}
		if (i == lines->len)
			break;
		char text[256];
		int length = snprintf(text, sizeof text, "%s", cells.items[1]);
		for (size_t cell = 2; cell < cells.len; cell++)
			if (*cells.items[cell])
				length += snprintf(text + length, sizeof text - (size_t) length,
								   " %s", cells.items[cell]);
		addString(&commands, text);
		free(animation);
		animation = copyString(cells.items[0]);
		freeStrings(&cells);
	}
	free(animation);
}

/*
 * A setting of the sprite in a table of its own beside it, NAME.KIND.tsv:
 * one column, one number a row. Nothing when there is no such table.
 */
static bool
setting(const char *table, const char *kind, Strings *values)
{
	char *path = formatString("%.*s.%s.tsv", (int) (strlen(table) - 4), table, kind);
	bool found = exists(path);
	if (found)
	{
		Buffer text = readFile(path);
		Strings lines = split((char *) text.data, '\n');
		freeStrings(values);
		for (size_t i = 1; i < lines.len; i++)
			if (*trim(lines.items[i]))
				addString(values, trim(lines.items[i]));
		freeStrings(&lines);
		free(text.data);
	}
	free(path);
	return found;
}

/*
 * A sprite's file for the bank's edition: name_ja.png where there is one,
 * else name.png.
 */
static char *
spriteFile(const Bank *bank, const char *name, const char *suffix)
{
	char *file = formatString("%s%s%s", name, bank->edition, suffix),
		 *path = sibling(bank->input, file);
	free(file);
	if (exists(path))
		return path;
	free(path);
	file = formatString("%s%s", name, suffix);
	path = sibling(bank->input, file);
	free(file);
	return path;
}

static void
metadata(Sprite *sprite, const char *table)
{
	Buffer data = readFile(table);
	Strings lines = split((char *) data.data, '\n');
	if (!lines.len || strcmp(lines.items[0], ANIMATION_COLUMNS))
		fail("a sprite's columns are animation, command, value and frames");
	animationTable(sprite, &lines);
	freeStrings(&lines);
	free(data.data);
	if (setting(table, "pictures", &sprite->pictureList))
		sprite->hasPictures = true;
	if (setting(table, "animations", &sprite->animationList))
		sprite->hasAnimations = true;
	setting(table, "empty", &sprite->empty);
}

/*
 * Parse sharing relationships before writing records. A sprite's own table
 * is read only by an owner, so a borrower does not emit a second copy.
 */
static Bank
readBank(const char *input, const char *edition)
{
	Bank b = {0};
	b.input = input;
	b.edition = edition;
	Buffer data = readFile(input);
	Strings lines = linesOf(&data);
	if (!lines.len ||
		strcmp(lines.items[0],
			   "sprite\twidth\theight\tscale\tdraw\tadjust_x\tadjust_y\tbox_x"
			   "\tbox_y\tcodec\tpictures\tanimations\tanimation_count"))
		fail("a sprite table has its thirteen columns");
	for (size_t i = 1; i < lines.len; i++)
	{
		Strings f = split(lines.items[i], '\t');
		if (f.len < 10 || f.len > 13)
			fail("a sprite row has ten to thirteen cells");
		while (f.len < 13)
			addString(&f, "");
		Sprite s = {0};
		s.animationLimit = SIZE_MAX;
		s.name = copyString(f.items[0]);
		s.bytes[0] = integer(f.items[1], false, 255);
		s.bytes[1] = integer(f.items[2], false, 255);
		write16(s.bytes + 2, integer(f.items[3], false, 65535));
		s.bytes[4] = integer(f.items[4], false, 255);
		s.bytes[6] = integer(f.items[5], true, 0);
		s.bytes[7] = integer(f.items[6], true, 0);
		for (unsigned j = 7; j <= 9; j++)
			s.bytes[j + 1] = integer(f.items[j], false, 255);
		if (*f.items[10])
			s.pictures = copyString(f.items[10]);
		if (*f.items[11])
			s.animations = copyString(f.items[11]);
		if (*trim(f.items[12]))
			s.animationLimit = number(trim(f.items[12]), 10, SIZE_MAX);
		char *path = spriteFile(&b, s.name, ".tsv");
		if (owns(&s, s.animations) || (owns(&s, s.pictures) && exists(path)))
			metadata(&s, path);
		free(path);
		b.sprites = resize(b.sprites, b.count + 1, sizeof(Sprite));
		b.sprites[b.count++] = s;
		freeStrings(&f);
	}
	char *path = sibling(input, "pictures.tsv");
	if (exists(path))
	{
		Buffer pictures = readFile(path);
		b.pictures = split((char *) pictures.data, '\n');
		free(pictures.data);
		if (!b.pictures.len || strcmp(b.pictures.items[0], "sprite\tpicture"))
			fail("invalid pictures table header");
	}
	free(path);
	freeStrings(&lines);
	free(data.data);
	return b;
}

static char *
spriteLabel(const char *name, const char *item)
{
	return formatString("Sprite_%s_%s", name, item);
}

static void
label(Assembly *a, const char *name, const char *item)
{
	char *s = spriteLabel(name, item);
	labelAt(a, s, a->bytes.len, false);
	free(s);
}

static void
pointer(Assembly *a, const char *name, const char *item)
{
	char *s = spriteLabel(name, item);
	addPointer(a, s, 0);
	free(s);
}

/*
 * Animation counts come from the owner, optionally capped by the borrowing
 * record. The two pointers may independently name different owners.
 */
static void
records(Bank *bank, Assembly *assembly)
{
	for (size_t i = 0; i < bank->count; i++)
	{
		Sprite *sprite = bank->sprites + i;
		append(&assembly->bytes, sprite->bytes, 5);
		size_t count = 0;
		if (sprite->animations)
		{
			Sprite *owner = find(bank, sprite->animations);
			count = owner->hasAnimations ? owner->animationList.len
										 : owner->scriptCount;
			if (sprite->animationLimit != SIZE_MAX)
			{
				if (sprite->animationLimit > count)
					fail("sprite animation count exceeds owner");
				count = sprite->animationLimit;
			}
		}
		if (count > 255)
			fail("too many sprite animations");
		byte(&assembly->bytes, count);
		append(&assembly->bytes, sprite->bytes + 6, 6);
		if (sprite->pictures)
			pointer(assembly, sprite->pictures, "Pictures");
		else
			word(&assembly->bytes, 0);
		if (sprite->animations)
			pointer(assembly, sprite->animations, "Animations");
		else
			word(&assembly->bytes, 0);
	}
}

/*
 * An entry may name another sprite's animation script; validate its index
 * against that script owner before creating a relocation.
 */
static void
animationList(Bank *bank, Sprite *sprite, Assembly *assembly)
{
	label(assembly, sprite->name, "Animations");
	size_t count =
		sprite->hasAnimations ? sprite->animationList.len : sprite->scriptCount;
	for (size_t i = 0; i < count; i++)
	{
		char *value = sprite->hasAnimations
						  ? copyString(sprite->animationList.items[i])
						  : formatString("%zu", i);
		char *colon = strchr(value, ':');
		const char *owner = sprite->name, *index = value;
		if (colon)
		{
			*colon = 0;
			owner = value;
			index = colon + 1;
		}
		size_t n = number(index, 10, SIZE_MAX);
		if (n >= find(bank, owner)->scriptCount)
			fail("sprite has no animation %zu", n);
		char *item = formatString("Animation%zu", n);
		pointer(assembly, owner, item);
		free(item);
		free(value);
	}
}

static void
scripts(Sprite *sprite, Assembly *assembly)
{
	for (size_t i = 0; i < sprite->scriptCount; i++)
	{
		char *item = formatString("Animation%zu", i);
		label(assembly, sprite->name, item);
		free(item);
		append(&assembly->bytes, sprite->scripts[i].data,
			   sprite->scripts[i].len);
	}
}

/*
 * Picture pointers follow the editable list, while payloads follow sheet order.
 * Arena compression sees the encoded bytes of all preceding pictures.
 */
static void
spritePictures(Bank *bank, Sprite *sprite, Assembly *list, Assembly *data)
{
	char *name = spriteFile(bank, sprite->name, ".png");
	// pictures.tsv draws a sprite with another sprite's picture.
	for (size_t i = 1; i < bank->pictures.len; i++)
	{
		Strings f = split(bank->pictures.items[i], '\t');
		if (f.len == 2 && !strcmp(f.items[0], sprite->name))
		{
			free(name);
			name = spriteFile(bank, f.items[1], ".png");
		}
		freeStrings(&f);
	}
	Image image = readImageFile(name);
	free(name);
	size_t width = sprite->bytes[0], height = sprite->bytes[1];
	if (!width || !height || image.width != width || image.height % height)
		fail("sprite pictures have wrong dimensions");
	size_t count = image.height / height, size = width * height;
	label(list, sprite->name, "Pictures");
	for (size_t i = 0; i < (sprite->hasPictures ? sprite->pictureList.len : count);
		 i++)
	{
		const char *value =
			sprite->hasPictures ? sprite->pictureList.items[i] : NULL;
		if (value && !strcmp(value, "-"))
		{
			word(&list->bytes, 0);
			continue;
		}
		size_t n = value ? number(value, 10, SIZE_MAX) : i;
		if (n >= count)
			fail("picture list exceeds sprite sheet");
		char *item = formatString("Picture%zu", n);
		pointer(list, sprite->name, item);
		free(item);
	}
	Buffer arena = {0};
	for (size_t i = 0; i < count; i++)
	{
		char *item = formatString("Picture%zu", i);
		label(data, sprite->name, item);
		free(item);
		Buffer picture = {image.pixels.data + i * size, size, size}, stream = {0};
		bool empty = false;
		for (size_t j = 0; j < sprite->empty.len; j++)
			if (number(sprite->empty.items[j], 10, SIZE_MAX) == i)
				empty = true;
		if (empty)
		{
			if (sprite->bytes[10] != 3)
				fail("only an arena picture can be empty");
			for (size_t p = 0; p < size; p++)
				if (picture.data[p])
					fail("empty arena picture is inked");
		}
		switch (sprite->bytes[10])
		{
		case 0:
			stream = compressZeros(&picture);
			break;
		case 1:
			stream = compressLz(&picture, &picture, "tagged", packer);
			break;
		case 3:
		{
			Buffer decoded = empty ? (Buffer){0} : compressZeros(&picture);
			stream = compressArena(&decoded, &arena);
			free(decoded.data);
			break;
		}
		default:
			fail("sprite codec has no pictures");
		}
		append(&arena, stream.data, stream.len);
		append(&data->bytes, stream.data, stream.len);
		free(stream.data);
	}
	free(arena.data);
	freeImage(&image);
}

static void
freeBank(Bank *bank)
{
	for (size_t i = 0; i < bank->count; i++)
	{
		Sprite *sprite = bank->sprites + i;
		free(sprite->name);
		free(sprite->pictures);
		free(sprite->animations);
		for (size_t j = 0; j < sprite->scriptCount; j++)
			free(sprite->scripts[j].data);
		free(sprite->scripts);
		freeStrings(&sprite->animationList);
		freeStrings(&sprite->pictureList);
		freeStrings(&sprite->empty);
	}
	free(bank->sprites);
	freeStrings(&bank->pictures);
	*bank = (Bank){0};
}

/*
 * The flat layout groups tables and payloads. The sectioned layout follows
 * layout.tsv, each sprite in order with the section it is placed in, and
 * emits each picture list after that sprite's picture payload. The bank's
 * name after its table's is its edition: sprites_ja from sprites.tsv is _ja.
 */
Buffer
spriteSource(const char *input, const char *bankName, bool blocks)
{
	const char *slash = strrchr(input, '/'), *table = slash ? slash + 1 : input;
	size_t stem = strcspn(table, ".");
	if (strncmp(bankName, table, stem))
		fail("a sprite bank is named after its table");
	Bank bank = readBank(input, bankName + stem);
	Assembly data = {0};
	records(&bank, &data);
	Buffer out = {0};
	if (!blocks)
	{
		for (size_t i = 0; i < bank.count; i++)
			if (owns(bank.sprites + i, bank.sprites[i].animations))
				animationList(&bank, bank.sprites + i, &data);
		Assembly images = {0};
		for (size_t i = 0; i < bank.count; i++)
			if (owns(bank.sprites + i, bank.sprites[i].pictures))
			{
				Assembly list = {0}, image = {0};
				spritePictures(&bank, bank.sprites + i, &list, &image);
				appendAssembly(&data, &list);
				appendAssembly(&images, &image);
				freeAssembly(&list);
				freeAssembly(&image);
			}
		for (size_t i = 0; i < bank.count; i++)
			if (owns(bank.sprites + i, bank.sprites[i].animations))
				scripts(bank.sprites + i, &data);
		appendAssembly(&data, &images);
		freeAssembly(&images);
		appendAssemblySource(&out, &data);
	}
	else
	{
		Buffer layout = readSibling(input, "layout.tsv");
		Strings lines = linesOf(&layout), order = {0};
		char *section = copyString("");
		if (!lines.len || strcmp(lines.items[0], "sprite\tsection"))
			fail("a sprite layout's columns are sprite and section");
		for (size_t i = 1; i < lines.len; i++)
		{
			Strings f = split(lines.items[i], '\t');
			const char *name = trim(f.items[0]),
					   *placed = f.len > 1 ? trim(f.items[1]) : "";
			if (strcmp(placed, section))
			{
				appendAssemblySource(&out, &data);
				freeAssembly(&data);
				print(&out, "\t.section .rodata.%s,\"a\"\n", placed);
				free(section);
				section = copyString(placed);
			}
			Sprite *s = find(&bank, name);
			if (owns(s, s->animations))
			{
				scripts(s, &data);
				alignAssembly(&data, 4, 0);
				animationList(&bank, s, &data);
			}
			addString(&order, name);
			freeStrings(&f);
		}
		free(section);
		for (size_t i = 0; i < order.len; i++)
		{
			Sprite *s = find(&bank, order.items[i]);
			if (!owns(s, s->pictures))
				continue;
			Assembly list = {0}, image = {0};
			spritePictures(&bank, s, &list, &image);
			appendAssembly(&data, &image);
			alignAssembly(&data, 4, 0);
			appendAssembly(&data, &list);
			word(&data.bytes, 0);
			freeAssembly(&list);
			freeAssembly(&image);
		}
		appendAssemblySource(&out, &data);
		free(layout.data);
		freeStrings(&lines);
		freeStrings(&order);
	}
	freeAssembly(&data);
	freeBank(&bank);
	return out;
}

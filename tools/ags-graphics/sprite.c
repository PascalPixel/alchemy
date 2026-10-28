/*
 * sprite.c
 *
 * Build sprite records, shared animations, and frame banks from TSV lists.
 * A sprite may borrow another sprite's frame or animation list. The bank owns
 * all parsed metadata; assembly labels and pointers preserve those relationships
 * in either the flat-bank or sectioned layout.
 */
#include "resource.h"
#include "../shared/assembly.h"

typedef struct
{
	char *name, *files, *frames, *animations;
	size_t animationLimit;
	uint8_t bytes[12];
	Buffer *scripts;
	size_t scriptCount;
	Strings animationList, frameList, empty;
	bool hasAnimations, hasFrames;
} Sprite;
typedef struct
{
	Sprite *sprites;
	size_t count;
	const char *input;
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
	uint64_t v = decimalOrHex(s, sign ? (negative ? 128 : 127) : max);
	if (negative && !sign)
		fail("negative sprite field");
	return negative ? (unsigned) -v : (unsigned) v;
}

/*
 * An animation script is a list of commands, two bytes each, which the
 * field reads (enum AnimationCommand in games/tbs/src/field/field_internal.h).
 * An anim line names them, separated by commas:
 *
 *   frame N D   show frame N for D frames (N below 239)
 *   wait D      keep the current frame D frames longer
 *   blank D     show nothing for D frames
 *   kind N      use facing table N
 *   play N      switch to animation N
 *   loop        continue at the first command
 *   jump N      continue at command N
 *   hold        stay on this command for good
 *   end         blank the sprite and stop
 *   command C A a command with no name here, C, with argument A
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
animationScript(const char *text)
{
	Buffer script = {0};
	Strings commands = split(text, ',');
	for (size_t i = 0; i < commands.len; i++)
	{
		Strings words = split(trim(commands.items[i]), ' ');
		const char *name = words.items[0];
		size_t n = words.len;
		unsigned code = 256,
				 argument = n > 1 ? number(words.items[n - 1], 10, 255) : 0;
		if (!strcmp(name, "frame") && n == 3)
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

static void
metadata(Sprite *sprite, const Buffer *data)
{
	Strings lines = split((char *) data->data, '\n');
	for (size_t i = 0; i < lines.len; i++)
	{
		char *line = trim(lines.items[i]);
		if (!*line)
			continue;
		char *tab = strchr(line, '\t');
		if (!tab)
			fail("sprite metadata needs key and values");
		*tab++ = 0;
		Strings values = split(tab, ' ');
		if (!strcmp(line, "anim"))
		{
			sprite->scripts = resize(sprite->scripts, sprite->scriptCount + 1,
									 sizeof(Buffer));
			sprite->scripts[sprite->scriptCount++] = animationScript(tab);
		}
		else if (!strcmp(line, "animations"))
		{
			freeStrings(&sprite->animationList);
			sprite->animationList = values;
			sprite->hasAnimations = true;
			values = (Strings){0};
		}
		else if (!strcmp(line, "list"))
		{
			freeStrings(&sprite->frameList);
			sprite->frameList = values;
			sprite->hasFrames = true;
			values = (Strings){0};
		}
		else if (!strcmp(line, "empty"))
		{
			freeStrings(&sprite->empty);
			sprite->empty = values;
			values = (Strings){0};
		}
		else
			fail("unknown sprite metadata key: %s", line);
		freeStrings(&values);
	}
	freeStrings(&lines);
}

/*
 * Parse sharing relationships before writing records. Metadata is loaded only
 * by an owner, so aliases do not emit a second copy of a list.
 */
static Bank
readBank(const char *input)
{
	Bank b = {0};
	b.input = input;
	Buffer data = readFile(input);
	Strings lines = split((char *) data.data, '\n');
	Strings header = split(lines.items[0], '\t');
	if (header.len != 12)
		fail("sprite table requires twelve columns");
	freeStrings(&header);
	for (size_t i = 1; i < lines.len; i++)
	{
		if (!*trim(lines.items[i]))
			continue;
		Strings f = split(lines.items[i], '\t');
		if (f.len != 12)
			fail("sprite row requires twelve fields");
		Sprite s = {0};
		s.animationLimit = SIZE_MAX;
		char *alias = strchr(f.items[0], '=');
		if (alias)
			*alias++ = 0;
		s.name = copyString(f.items[0]);
		s.files = copyString(alias ? alias : f.items[0]);
		s.bytes[0] = integer(f.items[1], false, 255);
		s.bytes[1] = integer(f.items[2], false, 255);
		write16(s.bytes + 2, integer(f.items[3], false, 65535));
		s.bytes[4] = integer(f.items[4], false, 255);
		s.bytes[6] = integer(f.items[5], true, 0);
		s.bytes[7] = integer(f.items[6], true, 0);
		for (unsigned j = 7; j <= 9; j++)
			s.bytes[j + 1] = integer(f.items[j], false, 255);
		if (strcmp(f.items[10], "-"))
			s.frames = copyString(f.items[10]);
		char *colon = strchr(f.items[11], ':');
		if (colon)
		{
			*colon++ = 0;
			s.animationLimit = number(colon, 10, SIZE_MAX);
		}
		if (strcmp(f.items[11], "-"))
			s.animations = copyString(f.items[11]);
		char *name = formatString("%s.tsv", s.files),
			 *path = sibling(input, name);
		free(name);
		if (owns(&s, s.animations) || (owns(&s, s.frames) && exists(path)))
		{
			Buffer meta = readFile(path);
			metadata(&s, &meta);
			free(meta.data);
		}
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
		if (sprite->frames)
			pointer(assembly, sprite->frames, "Frames");
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
 * Frame pointers follow the editable list, while payloads follow sheet order.
 * Arena compression sees the encoded bytes of all preceding frames.
 */
static void
frames(Bank *bank, Sprite *sprite, Assembly *list, Assembly *data)
{
	char *name = formatString("%s.png", sprite->files);
	// pictures.tsv draws a sprite with another sprite's picture.
	for (size_t i = 1; i < bank->pictures.len; i++)
	{
		Strings f = split(bank->pictures.items[i], '\t');
		if (f.len == 2 && !strcmp(f.items[0], sprite->files))
		{
			free(name);
			name = formatString("%s.png", f.items[1]);
		}
		freeStrings(&f);
	}
	Buffer png = readSibling(bank->input, name);
	free(name);
	Image image = readImage(&png);
	free(png.data);
	size_t width = sprite->bytes[0], height = sprite->bytes[1];
	if (!width || !height || image.width != width || image.height % height)
		fail("sprite frames have wrong dimensions");
	size_t count = image.height / height, size = width * height;
	label(list, sprite->name, "Frames");
	for (size_t i = 0; i < (sprite->hasFrames ? sprite->frameList.len : count);
		 i++)
	{
		const char *value =
			sprite->hasFrames ? sprite->frameList.items[i] : NULL;
		if (value && !strcmp(value, "-"))
		{
			word(&list->bytes, 0);
			continue;
		}
		size_t n = value ? number(value, 10, SIZE_MAX) : i;
		if (n >= count)
			fail("frame list exceeds sprite sheet");
		char *item = formatString("Frame%zu", n);
		pointer(list, sprite->name, item);
		free(item);
	}
	Buffer arena = {0};
	for (size_t i = 0; i < count; i++)
	{
		char *item = formatString("Frame%zu", i);
		label(data, sprite->name, item);
		free(item);
		Buffer frame = {image.pixels.data + i * size, size, size}, stream = {0};
		bool empty = false;
		for (size_t j = 0; j < sprite->empty.len; j++)
			if (number(sprite->empty.items[j], 10, SIZE_MAX) == i)
				empty = true;
		if (empty)
		{
			if (sprite->bytes[10] != 3)
				fail("only an arena frame can be empty");
			for (size_t p = 0; p < size; p++)
				if (frame.data[p])
					fail("empty arena frame is inked");
		}
		switch (sprite->bytes[10])
		{
		case 0:
			stream = compressZeros(&frame);
			break;
		case 1:
			stream = compressLz(&frame, &frame, "tagged", packer);
			break;
		case 3:
		{
			Buffer decoded = empty ? (Buffer){0} : compressZeros(&frame);
			stream = compressArena(&decoded, &arena);
			free(decoded.data);
			break;
		}
		default:
			fail("sprite codec has no frames");
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
		free(sprite->files);
		free(sprite->frames);
		free(sprite->animations);
		for (size_t j = 0; j < sprite->scriptCount; j++)
			free(sprite->scripts[j].data);
		free(sprite->scripts);
		freeStrings(&sprite->animationList);
		freeStrings(&sprite->frameList);
		freeStrings(&sprite->empty);
	}
	free(bank->sprites);
	freeStrings(&bank->pictures);
	*bank = (Bank){0};
}

/*
 * The flat layout groups tables and payloads. The sectioned layout follows
 * layout.tsv and emits each frame list after that sprite's frame payload.
 */
Buffer
spriteSource(const char *input, bool blocks)
{
	Bank bank = readBank(input);
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
			if (owns(bank.sprites + i, bank.sprites[i].frames))
			{
				Assembly list = {0}, image = {0};
				frames(&bank, bank.sprites + i, &list, &image);
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
		Strings lines = split((char *) layout.data, '\n'), order = {0};
		for (size_t i = 0; i < lines.len; i++)
		{
			char *line = trim(lines.items[i]);
			if (!*line)
				continue;
			if (!strncmp(line, "section\t", 8))
			{
				appendAssemblySource(&out, &data);
				freeAssembly(&data);
				print(&out, "\t.section .rodata.%s,\"a\"\n", line + 8);
			}
			else
			{
				if (strchr(line, '\t'))
					fail("invalid sprite layout row");
				Sprite *s = find(&bank, line);
				if (owns(s, s->animations))
				{
					scripts(s, &data);
					alignAssembly(&data, 4, 0);
					animationList(&bank, s, &data);
				}
				addString(&order, line);
			}
		}
		for (size_t i = 0; i < order.len; i++)
		{
			Sprite *s = find(&bank, order.items[i]);
			if (!owns(s, s->frames))
				continue;
			Assembly list = {0}, image = {0};
			frames(&bank, s, &list, &image);
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

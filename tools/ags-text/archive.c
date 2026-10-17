/*
 * archive.c
 *
 * Build context Huffman models, message banks, and their pointer tables.
 * Each preceding symbol selects a model; symbol zero starts and terminates
 * messages. Stable insertion order resolves equal frequencies. Models and
 * message banks use separate directories, grouped in runs of 256.
 */
#include "text.h"

typedef struct
{
	unsigned symbol;
	uint64_t count;
	Buffer path;
} Transition;
typedef struct
{
	Transition *transitions;
	size_t count;
	Buffer leaves;
	Bits tree;
} Context;
typedef struct
{
	uint64_t count;
	size_t order;
	int left, right, transition;
} Node;

static Transition *
transition(Context *context, unsigned symbol)
{
	for (size_t i = 0; i < context->count; i++)
		if (context->transitions[i].symbol == symbol)
			return context->transitions + i;
	context->transitions =
		resize(context->transitions, context->count + 1, sizeof(Transition));
	context->transitions[context->count] = (Transition){symbol, 0, {0}};
	return context->transitions + context->count++;
}

/*
 * Write tree shape in preorder and record each leaf's path for message encoding.
 */
static void
walk(Context *context, Node *nodes, int index, Buffer *path)
{
	Node *node = nodes + index;
	if (node->transition >= 0)
	{
		Transition *next = context->transitions + node->transition;
		putBits(&context->tree, 1, 1);
		half(&context->leaves, next->symbol);
		append(&next->path, path->data, path->len);
	}
	else
	{
		putBits(&context->tree, 0, 1);
		byte(path, 0);
		walk(context, nodes, node->left, path);
		path->data[path->len - 1] = 1;
		walk(context, nodes, node->right, path);
		path->len--;
	}
}

static bool
before(const Node *a, const Node *b)
{
	return a->count < b->count || (a->count == b->count && a->order < b->order);
}

/* Equal frequencies keep insertion order; the tie break defines the archive. */
/* Each preceding symbol has its own Huffman tree. Equal frequencies keep
 * insertion order: changing that tie break changes the whole archive. */
static void
derive(Context *context)
{
	if (!context->count)
		return;
	Node *nodes = allocate(2 * context->count, sizeof(Node));
	int *queue = allocate(context->count, sizeof(int));
	size_t length = context->count, used = context->count;
	for (size_t i = 0; i < context->count; i++)
	{
		nodes[i] = (Node){context->transitions[i].count, i, -1, -1, (int) i};
		queue[i] = (int) i;
	}
	while (length > 1)
	{
		for (size_t i = 1; i < length; i++)
		{
			int value = queue[i];
			size_t j = i;
			while (j && before(nodes + value, nodes + queue[j - 1]))
			{
				queue[j] = queue[j - 1];
				j--;
			}
			queue[j] = value;
		}
		int a = queue[0], b = queue[1];
		nodes[used] = (Node){nodes[a].count + nodes[b].count, used, a, b, -1};
		memmove(queue, queue + 2, (length - 2) * sizeof(int));
		queue[length - 2] = (int) used++;
		length--;
	}
	Buffer path = {0};
	walk(context, nodes, queue[0], &path);
	free(path.data);
	free(nodes);
	free(queue);
}

/*
 * Every non-null message starts in context zero and contributes its terminating
 * zero transition; null messages contribute neither symbols nor transitions.
 */
static void
countTransitions(Context *contexts, Message **messages, size_t count)
{
	for (size_t i = 0; i < count; i++)
		if (!messages[i]->null)
		{
			Buffer *symbols = &messages[i]->symbols;
			unsigned previous = 0;
			for (size_t at = 0; at <= symbols->len; at += 2)
			{
				unsigned symbol =
					at == symbols->len ? 0 : read16(symbols->data + at);
				transition(contexts + previous, symbol)->count++;
				previous = symbol;
			}
		}
}

static Buffer
encodeLeaves(const Context *context)
{
	Buffer leaves = {0};
	size_t count = context->leaves.len / 2;
	/* The final pair is stored first; an odd final leaf occupies two bytes. */
	for (size_t group = (count + 1) / 2; group; group--)
	{
		size_t at = (group - 1) * 2;
		unsigned first = read16(context->leaves.data + at * 2);
		if (at + 1 < count)
		{
			unsigned second = read16(context->leaves.data + (at + 1) * 2);
			byte(&leaves, second);
			byte(&leaves, (first & 15) << 4 | second >> 8);
			byte(&leaves, first >> 4);
		}
		else
		{
			byte(&leaves, (first & 15) << 4);
			byte(&leaves, first >> 4);
		}
	}
	return leaves;
}

/*
 * A null message has no bytes. Other messages follow the paths derived from
 * the complete catalogue, including the terminal zero symbol.
 */
static Bits
encodeMessage(Context *contexts, const Message *message)
{
	Bits bits = {0};
	unsigned previous = 0;
	if (!message->null)
	{
		const Buffer *symbols = &message->symbols;
		for (size_t at = 0; at <= symbols->len; at += 2)
		{
			unsigned symbol =
				at == symbols->len ? 0 : read16(symbols->data + at);
			Transition *t = transition(contexts + previous, symbol);
			for (size_t b = 0; b < t->path.len; b++)
				putBits(&bits, t->path.data[b], 1);
			previous = symbol;
		}
	}
	return bits;
}

static void
freeContexts(Context *contexts, size_t alphabet)
{
	for (size_t i = 0; i < alphabet; i++)
	{
		Context *context = contexts + i;
		for (size_t j = 0; j < context->count; j++)
			free(context->transitions[j].path.data);
		free(context->transitions);
		free(context->tree.bytes.data);
		free(context->leaves.data);
	}
	free(contexts);
}

/* Build symbol contexts first, then the message banks and their pointers. */
/*
 * Model offsets are relative to their 256-symbol group. Message lengths use
 * repeated 255 bytes plus a remainder, separate from their packed payloads.
 */
Assembly
messageArchive(Catalog *catalog, const char *label)
{
	size_t alphabet = catalog->alphabet;
	Context *contexts = allocate(alphabet, sizeof(Context));
	Message **messages = allocate(catalog->count, sizeof(Message *));
	for (size_t i = 0; i < catalog->count; i++)
		messages[number(catalog->messages[i].id, 10, catalog->count - 1)] =
			catalog->messages + i;
	countTransitions(contexts, messages, catalog->count);
	Assembly assembly = {0};
	assembly.alignment = 4;
	Buffer offsets = {0}, bases = {0}, directory = {0};
	size_t groupStart = 0;
	char *models = formatString("%sModels", label);
	labelAt(&assembly, models, 0, true);
	for (size_t i = 0; i < alphabet; i++)
	{
		Context *context = contexts + i;
		derive(context);
		if (!(i % 256))
		{
			groupStart = assembly.bytes.len;
			word(&bases, (uint32_t) groupStart);
		}
		if (!context->count)
		{
			half(&offsets, 0x8000);
			continue;
		}
		Buffer leaves = encodeLeaves(context);
		size_t offset = assembly.bytes.len + leaves.len - groupStart;
		if (offset > 65535)
			fail("context model exceeds its offset table");
		half(&offsets, (unsigned) offset);
		append(&assembly.bytes, leaves.data, leaves.len);
		append(&assembly.bytes, context->tree.bytes.data,
			   context->tree.bytes.len);
		free(leaves.data);
	}
	alignBuffer(&assembly.bytes, 2);
	size_t offsetTable = assembly.bytes.len;
	append(&assembly.bytes, offsets.data, offsets.len);
	alignBuffer(&assembly.bytes, 4);
	char *name = formatString("%sContexts", label);
	labelAt(&assembly, name, assembly.bytes.len, true);
	free(name);
	for (size_t i = 0; i < bases.len / 4; i++)
	{
		addPointer(&assembly, models, read32(bases.data + 4 * i));
		addPointer(&assembly, models, offsetTable + i * 512);
	}
	for (size_t start = 0; start < catalog->count; start += 256)
	{
		Buffer payload = {0}, lengths = {0};
		for (size_t i = start; i < start + 256 && i < catalog->count; i++)
		{
			Bits bits = encodeMessage(contexts, messages[i]);
			size_t n = bits.bytes.len;
			if (n > 65535)
				fail("encoded message exceeds its length field");
			while (n >= 255)
			{
				byte(&lengths, 255);
				n -= 255;
			}
			byte(&lengths, n);
			append(&payload, bits.bytes.data, bits.bytes.len);
			free(bits.bytes.data);
		}
		word(&directory, (uint32_t) assembly.bytes.len);
		append(&assembly.bytes, payload.data, payload.len);
		word(&directory, (uint32_t) assembly.bytes.len);
		append(&assembly.bytes, lengths.data, lengths.len);
		free(payload.data);
		free(lengths.data);
	}
	alignBuffer(&assembly.bytes, 4);
	name = formatString("%sBanks", label);
	labelAt(&assembly, name, assembly.bytes.len, true);
	free(name);
	for (size_t i = 0; i < directory.len; i += 4)
		addPointer(&assembly, models, read32(directory.data + i));
	freeContexts(contexts, alphabet);
	free(messages);
	free(models);
	free(offsets.data);
	free(bases.data);
	free(directory.data);
	return assembly;
}

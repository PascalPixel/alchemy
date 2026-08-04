typedef unsigned char u8;

void Func_080f9a30(void *);

struct Node {
    u8 flags;
    u8 pad[0x33];
    struct Node *next;
};

struct Context {
    u8 active;
    u8 pad[0x1f];
    struct Node *head;
};

void Func_080f9a50(void *unused, struct Context *context)
{
    struct Node *node;

    node = context->head;
    while (node != 0) {
        u8 flags;

        flags = node->flags;
        if (flags & 0xc7) {
            flags |= 0x40;
            node->flags = flags;
        }
        Func_080f9a30(node);
        node = node->next;
    }
    context->active = 0;
}

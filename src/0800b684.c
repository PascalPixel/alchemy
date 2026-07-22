typedef unsigned char u8;
typedef unsigned int u32;

struct Child_0800b684 {
    u8 filler0[5];
    u8 value;
};

struct Parent_0800b684 {
    u8 filler0[37];
    u8 dirty;
    u8 filler38;
    u8 count;
    struct Child_0800b684 *children[1];
};

void Func_0800b684(struct Parent_0800b684 *parent, u32 value)
{
    if (parent != 0) {
        u32 rawCount = parent->count;

        if (rawCount != 0) {
            struct Child_0800b684 **child = parent->children;
            u32 count = rawCount;
            do {
                struct Child_0800b684 *current = *child++;
                if (current->value != 15)
                    current->value = value;
                count--;
            } while (count != 0);
        }
        parent->dirty = 1;
    }
}

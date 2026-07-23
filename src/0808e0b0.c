typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

struct Item0808e0b0 {
    u8 padding0[5];
    u8 value;
    u8 padding6[10];
    void *field10;
};

struct Inner0808e0b0 {
    u8 padding0[37];
    u8 changed;
    u8 padding26;
    u8 count;
    struct Item0808e0b0 *items[1];
};

struct Outer0808e0b0 {
    u8 padding0[80];
    struct Inner0808e0b0 *inner;
    u8 mode;
};

extern u32 Data_03001e40;
extern u8 Data_0809e6b8[];

void Func_0808e0b0(struct Outer0808e0b0 *outer, s32 value)
{
    struct Inner0808e0b0 *inner;
    struct Item0808e0b0 **item;
    struct Item0808e0b0 *current;
    s32 count;
    s32 replacement;
    u8 rawCount;

    if ((outer->mode & 15) == 1) {
        inner = outer->inner;
        replacement = value - 1;
        if (value == 0)
            replacement = Data_0809e6b8[(Data_03001e40 >> 1) & 7];

        rawCount = inner->count;
        if (rawCount != 0) {
            item = inner->items;
            count = rawCount;
            do {
                current = *item++;
                if (current != 0 && current->field10 != 0 &&
                    current->value != 15)
                    current->value = replacement;
                count--;
            } while (count != 0);
        }
        inner->changed = 1;
    }
}

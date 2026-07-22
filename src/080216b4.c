typedef unsigned char u8;
typedef unsigned int u32;

struct Object_080216b4 {
    struct Object_080216b4 *child;
    u8 filler4[4];
    volatile u8 source;
    u8 filler9[11];
    u8 destination;
};

extern volatile u32 Data_03001800;
extern u8 Data_08037226[];

#define ADD_U8(left, right) ((left) - ~(right) - 1)

void Func_080216b4(struct Object_080216b4 *object)
{
    volatile u8 *table = Data_08037226;
    u32 index = (Data_03001800 >> 2) & 7;
    u32 value = ADD_U8(object->source, table[index]);
    u32 nextIndex = (Data_03001800 >> 2) & 7;

    object->destination = value;
    object = object->child;
    value = ADD_U8(object->source, table[nextIndex]);
    object->destination = value;
}

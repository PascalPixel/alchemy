#include "types.h"

struct Object_080216b4 {
    struct Object_080216b4 *next;
    u8 filler4[4];
    volatile u8 source;
    u8 filler9[11];
    u8 destination;
};

extern volatile u32 Data_03001800;
extern u8 Data_08037226[];

#define ADD_U8(left, right) ((left) - ~(right) - 1)

void Ui_ApplyTableOffsetToPair(struct Object_080216b4 *obj)
{
    volatile u8 *tbl = Data_08037226;
    u32 idx = (Data_03001800 >> 2) & 7;
    u32 value = ADD_U8(obj->source, tbl[idx]);
    u32 idx2 = (Data_03001800 >> 2) & 7;

    obj->destination = value;
    obj = obj->next;
    value = ADD_U8(obj->source, tbl[idx2]);
    obj->destination = value;
}

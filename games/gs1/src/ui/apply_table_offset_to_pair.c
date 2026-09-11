#include "types.h"
#include "scene.h"

struct Object_080216b4 {
    struct Object_080216b4 *next;
    u8 filler4[4];
    volatile u8 source;
    u8 filler9[11];
    u8 destination;
};

extern volatile u32 gIw;
extern u8 gRom[];

#define ADD_U8(left, right) ((left) - ~(right) - 1)

void Ui_ApplyTableOffsetToPair(struct Object_080216b4 *obj)
{
    volatile u8 *tbl = gRom;
    u32 idx = (gIw >> 2) & 7;
    u32 value = ADD_U8(obj->source, tbl[idx]);
    u32 idx2 = (gIw >> 2) & 7;

    obj->destination = value;
    obj = obj->next;
    value = ADD_U8(obj->source, tbl[idx2]);
    obj->destination = value;
}

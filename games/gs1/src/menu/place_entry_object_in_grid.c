#include "a9_motion.h"
#include "types.h"

#define Menu_PlaceEntryObjectInGrid Func_080a9bd8

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 FixedPoint_Ratio(s32, s32);
s32 Modulo(s32, s32);
void Func_080a17c4(void *arg0);

void Menu_PlaceEntryObjectInGrid(struct Object080a9bd8 *obj, s32 index,
    s32 origin_x, s32 origin_y, s32 phase) {
    s32 no;

    no = index;
    if (no > 0x1F) {
        no = 0;
    }
    obj->y =
        (s16)((FixedPoint_Ratio(no, phase) * 0x10) + origin_y);
    obj->x =
        (s16)((Modulo(no, phase) * 0x10) + origin_x);
    Func_080a17c4(obj);
}

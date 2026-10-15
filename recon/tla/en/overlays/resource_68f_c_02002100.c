/* Draft, differ 27 of 61: the native keeps the actor in r7 and the object in r6 (here the reverse) and loads the row id before the actor (ldrh, then ldrsh); it also builds the 255 of 0xffff before its entry jump. Tried: the actor read inside the comparison, read twice, permute 1 minute (no source form). */
#include "../../../../games/tla/src/scene/tundaria_tower/tundaria_tower.h"

struct TundariaTowerEffectWork {
    u8 unk00[0x1a];
    s16 actor;
};

void Func_02002184(s32 id, u32 kind, s32 flag, s32 layer);

void Func_02002100(u16 *row, s32 mode, s32 count)
{
    struct TundariaTowerEffectWork *work = Ram_HeapSlots->effectWork;

    goto test;
    do {
        s32 actor = work->actor;

        if (actor == *row) {
            struct ObjectRuntime *object = Object_GetById(actor);
            s32 kind;
            s32 flag;

            row++;
            kind = row[0];
            flag = row[1];
            if (count <= 7)
                object->scaleY -= 0x1f00;
            if (mode == 1) {
                GameFlag_SetBit(flag);
                Func_02002184(actor, kind, flag, object->terrainId);
                object->scaleY = 0x10000;
            }
            return;
        }
        row += 3;
    test:;
    } while (*row != 0xffff);
}

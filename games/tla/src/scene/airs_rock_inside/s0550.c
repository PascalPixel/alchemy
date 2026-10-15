#include "airs_rock_inside.h"

extern u32 gData_0200654c[];
extern u32 gData_02006564[];
extern u32 gData_0200663c[];
extern u32 gData_020066b4[];
extern u32 gData_0200675c[];
extern u32 gData_020068ac[];
extern u32 gData_020068f4[];
extern u32 gData_020069e4[];
extern u32 gData_02006aec[];
extern u32 gData_02006bac[];

s32 Math_Cosine(s32 angle);
void ObjectDispatch_ApplyValueToChildren(struct FieldActor *object, s32 value);
void Map_EnableUpdateCallback(void);
void Map_DisableUpdateCallback(void);
u32 Random16Far(void);

// On most frames, moves the object to a random spot: x 176 to 431 pixels, y and
// z up to 31 and 127 pixels past the leader's. Then it keeps its action as the
// previous one and sets mode 9 after action 212, mode 10 otherwise.
void Func_020004d4(struct FieldActor *object)
{
    struct FieldActor *leader = Object_GetById(gPartyState.currentOwner);
    struct ObjectRuntime *self = (struct ObjectRuntime *)object;

    if (Random16Far() & 0xff)
    {
        object->x = ((Random16Far() & 0xff) << 16) + (176 << 16);
        {
            s32 r = Random16Far();
            /* FAKEMATCH: the native loads the leader's y into r3 before it builds the mask; pinned and held behind the barrier it does (differ 4 without both). */
            register s32 y asm("r3") = leader->y;

            /* FAKEMATCH: the barrier keeps the mask after the load (with the pin above). */
            __asm__ volatile("");
            object->y = y + ((r & 31) << 16);
        }
        {
            s32 r = Random16Far();
            /* FAKEMATCH: as for y, the leader's z loads into r3 before the mask (differ 4 without both). */
            register s32 z asm("r3") = leader->z;

            /* FAKEMATCH: the barrier keeps the mask after the load (with the pin above). */
            __asm__ volatile("");
            object->z = z + ((r & 127) << 16);
        }
    }
    self->unk66 = self->action;
    if (self->action == 212)
        Func_02005d5c(object, 9);
    else
        Func_02005d5c(object, 10);
}

void Func_02000550(struct FieldActor *object)
{
    u8 *work = Ram_HeapSlots->eventWork;

    if (*(s16 *)(work + 358) != 0 || *(s16 *)(work + 346) != 0 || *(s16 *)(work + 350) != 0
        || *(s16 *)(work + 360) != 0) {
        ObjectDispatch_ApplyValueToChildren(object, 0);
        Map_EnableUpdateCallback();
        return;
    }
    ObjectDispatch_ApplyValueToChildren(object, 16);
    Map_DisableUpdateCallback();
    if (BYTE_AT(object, 99) != 0) {
        if ((gFrameCount & 7) == 0)
            BYTE_AT(object, 99)--;
        return;
    }
    if (*(s16 *)((u8 *)object + 102) == 0)
        Func_020004d4(object);
    do { } while (0); /* FAKEMATCH: the loop note keeps the timer load after the reset call (stated plainly it is loaded before the test as well, 13 differ). */
    (*(s16 *)((u8 *)object + 102))--;
    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: the native stores the timer before it loads z (2 differ without it). */
    object->x += Math_Cosine(((object->z >> 14) + (object->y >> 15)) << 8) / 2;
    object->y += WORD_AT(object, 48);
}

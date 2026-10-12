/* Draft, differ 30 of 105 (English; the Japanese listing lacks the block under #if). Left: the
 * native builds each placement call's arguments on its own, angle first (movs r3, #176; movs r1;
 * lsls r3; movs r0; lsls r1), and keeps both calls; through the inline here the two calls share
 * their tail, and as direct calls the x is built first (differ 27). And it loads x and z before
 * it builds the steps 0xe0000 and 0x80000 (two reordered pairs), where this form builds the step
 * first. */
#include "../../../../games/tla/src/field/overlay_670/ov670.h"

void Func_02003288(void);
void Func_02004cf4(void);
void Func_02005b14(s32 actor, s32 x, s32 z, s32 angle);
void Func_02005c74(s32 actor, s32 value);

static inline void Ov670_ObjPlace(s32 actor, s32 x, s32 z, s32 angle)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02005b14(actor, x, z, angle);
}

void Func_02005898(void)
{
    struct Ov670Object *object;

    ((struct Ov670EventWork *)Ram_HeapSlots->event_work)->screen_effect = 521;
    Func_02003288();
    if (gPartyState.entrance == 9) {
        if (GameFlag_Test(0x8ef)) {
            Func_02005b0c(10, 0, 0);
            if (GameFlag_Test(0x8ee))
                Ov670_ObjPlace(8, 0x1b20000, 0x27a0000, 0xb000);
            else
                Ov670_ObjPlace(8, 0x16c0000, 0x2400000, 0xb000);
        } else {
            Func_02004cf4();
        }
    }
#if !defined(TLA_EDITION_JA)
    if (GameFlag_Test(0x109)) {
        object = Object_GetById(16);
        object->x -= 0xe0000;
        object->z -= 0x80000;
    }
#endif
    Func_02005c74(16, 0);
    object = Object_GetById(16);
    object->sprite->rotation = 0;
    object->scale_x = 0x10000;
    object->scale_y = 0x10000;
    object->x += 0xe0000;
    object->z += 0x80000;
    Object_SetModeById(16, 6);
    if (gPartyState.entrance == 14)
        GameFlag_SetBit(0x8ee);
}

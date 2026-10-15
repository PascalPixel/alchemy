/* Draft, differ 54 of 103: the instructions are the native's but four
 * registers trade places. The native keeps the other object in r6, the
 * sprite in r7, this object in r8 and the pass count in r10; this form the
 * sprite in r6, the other object in r7, the pass count in r8 and this object
 * in r10. Measured: all four loops written with goto, no loop notes (the
 * same registers, and the counter's step moves ahead of the store, differ
 * 64). Two pool loads also sit one step later than the native's. */
#include "../../../../games/tla/src/field/overlay_654/shrine_of_the_sea_god.h"

extern s32 Data_02004a18[3];

void Func_02001340(struct KabeObject *object, struct KabeObject *other)
{
    struct KabeSprite *sprite = object->sprite;
    u32 n;
    u32 i;

    Func_02003ab0(gPartyState.current_owner, 258);
    Func_02003b48(210);
    object->motion_flags = 0;
    for (n = 0; n <= 1; n++) {
        other->y -= 0x10000;
        for (i = 0; i <= 3; i++) {
            sprite->rotation += 128;
            WaitFrames(1);
        }
        other->y += 0x10000;
        for (i = 0; i <= 7; i++) {
            sprite->rotation -= 128;
            WaitFrames(1);
        }
        other->y -= 0x10000;
        for (i = 0; i <= 3; i++) {
            sprite->rotation += 128;
            WaitFrames(1);
        }
    }
    other->y += 0x10000;
    WaitFrames(20);
    Data_02004a18[0] = other->x - object->x;
    Data_02004a18[1] = other->y - object->y;
    Data_02004a18[2] = other->z - object->z;
    ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.current_owner), 0);
}

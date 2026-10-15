/* Draft, differ 2 of 37: the native builds 728 before it reloads gFrameCount for the sine; the draft loads first. Tried: 728 * gFrameCount, an s32 cast, a u32 local, + 0xf800, and a 1-minute permute (nothing better). Through the emote inline (scene layer) the call's 258 is built as the native does (direct: differ 4). Also tried: the two byte stores through a u8 pointer (2) or swapped (6); 728 in a one-pass block (10); 728 set first in a statement expression, which gives the native order with the registers swapped (10); 728 pinned to r3, the frame pinned to r2, or both (2 to 6). In sched2 the load and 728 tie on priority and on dependants, and the load wins by its earlier place. */
#include "../../../../games/tla/src/scene/jupiter_room_m/jupiter_room_m.h"
#include "field_sprite.h"
#include "frame.h"

s32 Math_Sine(s32 angle);
void Func_020048d4(s32 actor, s32 emote);

static inline void ObjEmote(s32 actor, s32 emote)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_020048d4(actor, emote);
}

s32 Func_02000a78(void)
{
    struct ObjectRuntime *object = Object_GetById(1);

    if (object != NULL) {
        struct FieldSprite *sprite = object->animation;

        if ((gFrameCount & 127) == 0)
            ObjEmote(29, 258);
        if (sprite != NULL) {
            sprite->unknown_14[3] = 30;
            sprite->unknown_14[2] = 252;
            sprite->rotation = Math_Sine(gFrameCount * 728) / 128 - 0x800;
        }
    }
    return 1;
}

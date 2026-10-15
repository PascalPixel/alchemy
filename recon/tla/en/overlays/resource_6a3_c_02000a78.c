/* Draft, differ 2 of 37: the native builds 728 before it reloads gFrameCount for the sine; the draft loads first. Tried: 728 * gFrameCount, an s32 cast, a u32 local, + 0xf800, and a 1-minute permute (nothing better). Through the emote inline (scene layer) the call's 258 is built as the native does (direct: differ 4). */
#include "../../../../games/tla/src/field/overlay_6a3/ov6a3.h"
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

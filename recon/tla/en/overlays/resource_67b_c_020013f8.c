/* Near miss, differ 4 of 48: sets the sprite's two priorities from the
 * render mode, as layer.c's Func_080d4ab4 sets layers. An empty asm holding
 * gPartyState's address loads it ahead of the 0x212 offset's split constant,
 * as the native does (without it, 6). Left: the native loads the sprite
 * (ldr r4, [r5, #80]) right after movs r1, #2 and its byte 9 right after
 * orrs; here both come two instructions later, sched2 ranking eors and lsrs
 * above them. Tried 2026-10-15: the sprite before or after the priority (6,
 * 14, 16), the priority as an if or 2 - (mode != 2) (37, 40), a second
 * barrier after the mode load (14), the sprite pinned to r4 and held by an
 * empty asm, with and without the 2 as its input (4), the 2 in a local held
 * by an asm (32: a compare and branch). */
#include "../../../../games/tla/src/field/overlay_67b/local.h"

void Func_020013f8(void)
{
    struct ObjectRuntime *object = Object_GetById(Func_02002a68());

    if (object != 0 && (object->animation_kind & 15) == 1) {
        struct PartyState *party = &gPartyState;
        struct FieldSprite *sprite;
        s32 priority;

        asm volatile("" : "+r"(party));
        sprite = object->animation;
        priority = party->render_mode == 2 ? 2 : 1;
        sprite->priority = priority;
        sprite->part_priority = priority;
        object->priority_flags &= 0xfe;
    }
}

/* NONMATCHING: 568 of 526 bytes, 111 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: twin of 020000c4 (actor 22, other positions) with the same allocation residual: the rise_counter address lands in r8 instead of r6. The reviewed span (526) stops before the function's own 24-byte literal pool; the complete extent is 552. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

struct DeckBird {
    u8 unknown_00[0x4c];
    s32 drift;
    u8 unknown_50[0x14];
    s16 turn_x;
    s16 turn_y;
};

#define BIRD(obj) ((struct DeckBird *)(obj))

s32 Local_020002ec(struct FieldActor *obj)
{
    u32 step;

    step = obj->rise_counter;
    if (step != 0) {
        switch (step) {
        case 1:
            obj->speed = 0x40000;
            obj->acceleration = 0x20000;
            Engine_ObjectSetPosition(obj, 0x1000000, 0x140000, 0x2800000);
            obj->rise_counter++;
            break;
        case 3:
            if (obj->target_x == ACTOR_NO_TARGET && obj->target_y == obj->target_x && obj->target_z == obj->target_y) {
                obj->rise_counter++;
                Engine_AudioPlayCue(146);
                if (obj->rise_enabled != 0) {
                    Engine_ActorFaceDirection(22, 0xd000, 0);
                } else {
                    Engine_ActorFaceDirection(22, 0xb000, 0);
                }
                if (((u32)Engine_RandomNext() << 2) >> 16 != 0) {
                    Engine_ActorGet(22)->velocity_y = 0x20000;
                } else {
                    Engine_ActorShowEmote(22, 0x103, 0);
                    Engine_ActorGet(22)->velocity_y = 0x60000;
                }
            }
            break;
        case 5:
            if (obj->rise_enabled != 0) {
                Engine_ObjectSetPosition(obj, 0x1080000, 0, 0x2580000);
            } else {
                Engine_ObjectSetPosition(obj, 0xf20000, 0, 0x25c0000);
            }
        case 2:
        case 4:
        case 6:
            obj->rise_counter++;
            break;
        case 7:
            if (obj->target_x == ACTOR_NO_TARGET && obj->target_y == obj->target_x && obj->target_z == obj->target_y) {
                obj->speed = 0x20000;
                obj->acceleration = 0x10000;
                BIRD(obj)->turn_x = 0;
                BIRD(obj)->turn_y = 0;
                obj->rise_counter++;
                BIRD(obj)->drift = 0;
            }
            break;
        case 8:
            obj->rise_counter = 0;
            break;
        }
    } else {
        if (BIRD(obj)->turn_x != 0) {
            BIRD(obj)->drift -= ((u32)Engine_RandomNext() << 12) >> 16;
            if (BIRD(obj)->drift < -0x4000) {
                BIRD(obj)->turn_x = step;
            }
        } else {
            BIRD(obj)->drift += ((u32)Engine_RandomNext() << 12) >> 16;
            if (BIRD(obj)->drift > 0x4000) {
                BIRD(obj)->turn_x = 1;
            }
        }
        if (obj->x.fixed > 0xe80000 && obj->x.fixed < 0x1100000) {
            obj->x.fixed += BIRD(obj)->drift;
        }
        if (BIRD(obj)->turn_y != 0) {
            obj->y.fixed = obj->y.fixed - (((u32)Engine_RandomNext() << 15) >> 16) - 0x8000;
            if (obj->y.fixed < 0) {
                BIRD(obj)->turn_y = 0;
            }
        } else {
            obj->y.fixed = obj->y.fixed + (((u32)Engine_RandomNext() << 15) >> 16) + 0x8000;
            if (obj->y.fixed > 0x80000) {
                BIRD(obj)->turn_y = 1;
            }
        }
    }
    if ((((u32)Engine_RandomNext() * 100) >> 16) == 0) {
        obj->rise_counter = 1;
    }
    return 1;
}

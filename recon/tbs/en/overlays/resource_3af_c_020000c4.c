/* NONMATCHING: 568 of 552 bytes, 97 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: the rise_counter address (obj+98) lands in r8 and the loaded step in r6; the reference keeps the address in r6 and the step in r8. With the address in a high register the two 'rise_counter++; break' tails use different scratch registers and are not cross-jumped (8 bytes), and the argument scheduling of the ObjectSetPosition calls differs. */
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

s32 Func_020000c4(struct FieldActor *obj)
{
    u32 step;

    step = obj->rise_counter;
    if (step != 0) {
        switch (step) {
        case 1:
            obj->speed = 0x40000;
            obj->acceleration = 0x20000;
            Engine_ObjectSetPosition(obj, 0x10c0000, 0x140000, 0x2b40000);
            obj->rise_counter++;
            break;
        case 3:
            if (obj->target_x == ACTOR_NO_TARGET && obj->target_y == obj->target_x && obj->target_z == obj->target_y) {
                obj->rise_counter++;
                Engine_AudioPlayCue(146);
                if (obj->rise_enabled != 0) {
                    Engine_ActorFaceDirection(21, 0xd000, 0);
                } else {
                    Engine_ActorFaceDirection(21, 0xb000, 0);
                }
                if (((u32)Engine_RandomNext() << 2) >> 16 != 0) {
                    Engine_ActorGet(21)->velocity_y = 0x20000;
                } else {
                    Engine_ActorShowEmote(21, 0x103, 0);
                    Engine_ActorGet(21)->velocity_y = 0x60000;
                }
            }
            break;
        case 5:
            if (obj->rise_enabled != 0) {
                Engine_ObjectSetPosition(obj, 0x11a0000, 0, 0x2920000);
            } else {
                Engine_ObjectSetPosition(obj, 0xfe0000, 0, 0x29c0000);
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
        if (obj->x.fixed > 0xf80000 && obj->x.fixed < 0x1240000) {
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

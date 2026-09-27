/* NONMATCHING: complete extent 552 bytes including the trailing pool;
 * candidate 552, 171 differing halfwords, 69 aligned edits (2026-09-27).
 * H1 transfers Call3/Call4 from exact FUNE_KANPAN/FLY_BY_21.C and
 * FLY_BY_22.C. Both callbacks share six callee bindings, actor layout and
 * state machine; only actor IDs and coordinate constants differ.
 * Baseline aligned edits were 98; facing/emote and case-5 position
 * argument construction now match. Remaining: first position-call y is
 * formed too early; counter address is r8 rather than r6, loaded step r6
 * rather than r8, and case 1 duplicates the increment shared by cases
 * 2/4/6 in the ROM. These differences shift branches, table entries and
 * pool reach; zero scratch and idle turn-y registers also differ. No DONE.
 * H2: explicit shared advance tail, after H1 was saved at 0fd473f02.
 * Case 1 also cross-jumps its final coordinate shift/call into case 5,
 * unlike the ROM. Equal size hides that 6-byte merge against extra counter
 * rematerialization and different alignment; it is not equal topology.
 * Full remaining model: counter address/value r8/r6 instead of r6/r8;
 * consequently scratch copies, cue setup, idle turn-y r6/r7, branch/table
 * positions and pool reach differ. Both owners retain complete pools.
 * STOP: one model plus one structural follow-up; no complete owner exact. */
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

/* FAKEMATCH: the exact fly-by siblings pass constants through these
 * inline call interfaces to preserve argument-register construction. */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

s32 Func_020002ec(struct FieldActor *obj)
{
    u32 step;

    step = obj->rise_counter;
    if (step != 0) {
        switch (step) {
        case 1:
            obj->speed = 0x40000;
            obj->acceleration = 0x20000;
            Call4(Engine_ObjectSetPosition, (s32)obj, 0x1000000, 0x140000, 0x2800000);
            goto advance;
        case 3:
            if (obj->target_x == ACTOR_NO_TARGET && obj->target_y == obj->target_x && obj->target_z == obj->target_y) {
                obj->rise_counter++;
                Engine_AudioPlayCue(146);
                if (obj->rise_enabled != 0) {
                    Call3(Engine_ActorFaceDirection, 22, 0xd000, 0);
                } else {
                    Call3(Engine_ActorFaceDirection, 22, 0xb000, 0);
                }
                if (((u32)Engine_RandomNext() << 2) >> 16 != 0) {
                    Engine_ActorGet(22)->velocity_y = 0x20000;
                } else {
                    Call3(Engine_ActorShowEmote, 22, 0x103, 0);
                    Engine_ActorGet(22)->velocity_y = 0x60000;
                }
            }
            break;
        case 5:
            if (obj->rise_enabled != 0) {
                Call4(Engine_ObjectSetPosition, (s32)obj, 0x1080000, 0, 0x2580000);
            } else {
                Call4(Engine_ObjectSetPosition, (s32)obj, 0xf20000, 0, 0x25c0000);
            }
        case 2:
        case 4:
        case 6:
        advance:
            /* FAKEMATCH: preserve the shared state-transition tail. */
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

/* Astra entry-call boundary (2026-09-27): call ObjectSetPosition directly
 * in case 1, retaining the exact fly-by Call4 interface in case 5.
 * Admission: case 1 must keep its own coordinate shifts and call, then
 * branch to the shared increment rather than entering case 5's call tail.
 * The complete normalized diff proves that local topology correction.
 * Candidate is now 560/552 bytes, 239 differing halfwords / 75 aligned
 * edits. The worse aggregate is understood: the previously missing call
 * tail returns, while counter address/value remain r8/r6 instead of r6/r8.
 * Initial y and z scheduling is still wrong; all other call targets and
 * six trailing pool words remain present. Keep this ordinary direct-call
 * boundary as the canonical structural correction, not an exact match.
 * No DONE or padding credit. Do not restore the shorter cross-jumped model
 * merely because it has fewer edits; the next correction must preserve
 * the separate case-1 call and shared increment.
 *
 * Previous NONMATCHING: complete extent 552 bytes including the trailing pool;
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
 * STOP: one model plus one structural follow-up; no complete owner exact.
 *
 * H3 (west, 2026-09-27): test a byte snapshot for the byte counter, with
 * its promoted switch input separate from the saved idle reset value.
 * Reference 02f8 loads the byte, 02fa saves it in r8, and 0302 copies the
 * dispatch scratch; 0464 retrieves that saved value after the random call.
 * Prediction was counter/value r6/r8 without changing the shared advance
 * tail or pools. Actual full diff is byte-identical to the old draft:
 * 552/552, 171 differing halfwords, 69 aligned edits. No admission.
 * Initial RTL already promotes the u8 user local to SI pseudo 33; combine deletes
 * its redundant copy. The value remains 4 uses/15 instructions/1 call,
 * counter-address pseudo 35 remains 10 uses/169 instructions/15 calls,
 * allocated last. No allocation tie, and no loop-pass transformation.
 * Case 1 still crossjumps its final coordinate shift and position call into
 * case 5; equal total size offsets that missing code against extra counter
 * copies and different alignment. The whole pool is present but not exact.
 * Together with 020000c4 H4 this closes direct-field/byte-snapshot producer
 * spellings. No new supported shared helper or source-phase boundary was
 * found in exact FLY_BY_21/22 and DECK_SEQ. Function/alignment credit +0.
 * H3 is preserved in 340f1f19d; retain the canonical word snapshot below.
 * A future trial needs evidence changing the persistent counter-address
 * lifetime or its real uses, not another spelling of the initial value.
 *
 * 2026-09-27 sol-deck closing audit: fresh full normalized diff reproduces
 * 552/552 bytes, 171 differing halfwords, 69 aligned edits. The case-1
 * branch at candidate +0x60 enters case 5's coordinate-shift/call tail;
 * reference +0x5a shifts its own z, +0x62 calls, then +0x66 enters the
 * shared phase increment. Candidate size equality therefore does not
 * prove the call blocks, dispatch addresses or complete pool exact.
 * Initial state pseudo 33 still has 4 uses across 15 instructions and
 * 1 call, allocated r6; phase address is r8. The first callback's H5
 * control record (042b1d5f3) increases real pointer consumers to 20 but
 * gets r7, loses r8 preservation and merges the separate mode/turn
 * addresses. It fails admission and is not blindly transferred here.
 * Existing Call3/4, word snapshot and shared advance remain canonical.
 * STOP: no supported new phase boundary follows from exact FLY_BY_21/22
 * or DECK_SEQ. Qualified pointers, local declarations, direct dispatch,
 * byte snapshots and the rejected control-record consumer axis stay
 * closed. Both owners remain C not yet written. Function and alignment
 * credit +0; bindings, padding and compiler gaps remain unchanged. */
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
            Engine_ObjectSetPosition(obj, 0x1000000, 0x140000, 0x2800000);
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
                    Object_GetById(22)->velocity_y = 0x20000;
                } else {
                    Call3(Engine_ActorShowEmote, 22, 0x103, 0);
                    Object_GetById(22)->velocity_y = 0x60000;
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

/* NONMATCHING: complete extent 552 bytes including the trailing pool;
 * candidate 560, 240 differing halfwords, 73 aligned edits (2026-09-27).
 * H1 transfers Call3/Call4 from exact FUNE_KANPAN/FLY_BY_21.C and
 * FLY_BY_22.C. Both callbacks share six callee bindings, actor layout and
 * state machine; only actor IDs and coordinate constants differ.
 * Baseline aligned edits were 97; facing/emote and case-5 position
 * argument construction now match. Remaining: first position-call y is
 * formed too early; counter address is r8 rather than r6, loaded step r6
 * rather than r8, and case 1 duplicates the increment shared by cases
 * 2/4/6 in the ROM. These differences shift branches, table entries and
 * pool reach; zero scratch and idle turn-y registers also differ. No DONE.
 * H2: explicit shared advance tail, after H1 was saved at 0fd473f02.
 * Case 1 now branches to the shared increment, but its argument shift order
 * differs; high-register counter rematerialization leaves 8 excess bytes.
 * Full remaining model: counter address/value r8/r6 instead of r6/r8;
 * consequently scratch copies, cue setup, idle turn-y r6/r7, branch/table
 * positions and pool reach differ. Both owners retain complete pools.
 * STOP: one model plus one structural follow-up; no complete owner exact.
 *
 * 2026-09-27 central family audit, after checkpoint e2a4a087:
 * H3 transfers FIELD_EFFECT.H's union FieldObject ownership pattern, joining
 * the actor and bird views without changing calls, control flow or stores.
 * Exact FUNE_KANPAN/FLY_BY_21.C and FLY_BY_22.C remain the call precedents;
 * their Call3/Call4 boundary was already transferred above, not retried.
 * Full own-ROM extent is [020000c4,020002ec), including the dispatch table
 * and all seven trailing pool words. Fresh baseline and H3 both score
 * 560/552 bytes, 240 differing halfwords, 73 aligned edits. The only H3
 * assembly change moves the case-7 state-pointer copy past the second
 * turn halfword store; there is no byte gain or missing-call repair.
 * Allocator evidence: the loaded state (pseudo 34) gets r6; the persistent
 * object+0x62 byte pointer (pseudo 36) is allocated last and gets r8.
 * The reference instead keeps that pointer in r6 and the initial state in
 * r8. H3 leaves this lifetime/order unchanged, hence the excess copies,
 * 8-byte growth and shifted branch/table/pool locations. The first position
 * call's shift order and idle turn-y pointer also remain different.
 * Keep the coherent union model as a draft; the previous model is in Git.
 * No second new helper boundary is supported by the exact sibling audit.
 * STOP this ownership-union axis: no declaration, pointer or register sweep.
 * Neither this callback nor its 020002ec twin is newly exact. DONE +0.
 *
 * H4 (west): exact FLY_BY_21/22 dispatch directly on their record field.
 * Remove the named step snapshot and spell the idle reset as literal zero,
 * retaining the union, Call3/4 and shared increment tail. Predicted admission
 * was counter/value r6/r8 and 552 bytes. Actual: 560/552, 234 halfwords,
 * 74 aligned edits, counter r8 and initial value r7. Rejected.
 * CSE still uses its known-zero state for the idle turn_x store. Counter
 * address pseudo 35 has 11 uses over 170 instructions and 15 calls; value
 * 37 has 3 uses over 14 instructions and 1 call. Allocation orders 37 then
 * 35, with no tie. The loop pass performs no loop transformation here.
 * This disproves the named snapshot alone as cause of pointer priority;
 * direct-field spelling is now closed, not a reason to permute declarations.
 * Full normalized diff read: initial position shift order, idle pointers,
 * extra counter copies and shifted table/pool remain. No exact credit.
 * H4 is preserved in 3f451ae66; restore the canonical H3 source below.
 * The second callback's byte-snapshot follow-up also leaves allocation
 * unchanged. Neither result supplies a tied allocator priority or a new
 * supported phase/helper boundary. Stop this producer axis.
 *
 * H5 (sol-deck): a control record beginning at object+0x62 owns phase,
 * mode and both signed turn fields. All four fields use that record, so
 * this changes real pointer consumers rather than the snapshot spelling.
 * Admission predicted counter pointer r6, saved state r8 and the existing
 * shared advance tail. Actual complete diff: 516/552 bytes, 262 differing
 * halfwords, 101 aligned edits. Pointer pseudo 34 has 20 uses across 163
 * instructions and 15 calls, allocated r7; state pseudo 35 has 4 uses
 * across 14 instructions and 1 call, allocated r6. The r8 save disappears.
 * Mode reads become [ctrl,#1], turn accesses [ctrl,#2]/[ctrl,#4]; case 7
 * hoists its phase load over both turn stores. The shared advance tail
 * survives, but its pointer and all separate idle field addresses fail
 * admission. A nested control member also aligned to object+0x64; this
 * corrected trial takes the control view at the known phase byte +0x62.
 * STOP the control-record consumer axis; no transfer to the twin or
 * pointer/declaration variants. Function credit +0, alignment credit +0.
 * Rejected H5 is preserved in 042b1d5f3; canonical H3 is restored below.
 * Fresh near-sibling recall is complete: no exact, equivalent or near
 * source sibling is registered for this owner. Exact FLY_BY_21/22 remain
 * call/layout evidence, not a second unresolved-pointer solution.
 * The admitted tail and all source bindings, padding and compiler gaps
 * remain unchanged. Resume only with a new supported pointer producer or
 * phase boundary; bounded ownership axes are exhausted. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

struct DeckBird {
    u8 unknown_00[0x4c];
    s32 drift;
    u8 unknown_50[0x14];
    s16 turn_x;
    s16 turn_y;
};

/* As in FIELD_EFFECT.H, the callback owns one record with two views. */
union DeckObject {
    struct FieldActor actor;
    struct DeckBird bird;
};

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

s32 Func_020000c4(union DeckObject *work)
{
    struct FieldActor *obj = &work->actor;
    u32 step;

    step = obj->rise_counter;
    if (step != 0) {
        switch (step) {
        case 1:
            obj->speed = 0x40000;
            obj->acceleration = 0x20000;
            Call4(Engine_ObjectSetPosition, (s32)obj, 0x10c0000, 0x140000, 0x2b40000);
            goto advance;
        case 3:
            if (obj->target_x == ACTOR_NO_TARGET && obj->target_y == obj->target_x && obj->target_z == obj->target_y) {
                obj->rise_counter++;
                Engine_AudioPlayCue(146);
                if (obj->rise_enabled != 0) {
                    Call3(Engine_ActorFaceDirection, 21, 0xd000, 0);
                } else {
                    Call3(Engine_ActorFaceDirection, 21, 0xb000, 0);
                }
                if (((u32)Engine_RandomNext() << 2) >> 16 != 0) {
                    Object_GetById(21)->velocity_y = 0x20000;
                } else {
                    Call3(Engine_ActorShowEmote, 21, 0x103, 0);
                    Object_GetById(21)->velocity_y = 0x60000;
                }
            }
            break;
        case 5:
            if (obj->rise_enabled != 0) {
                Call4(Engine_ObjectSetPosition, (s32)obj, 0x11a0000, 0, 0x2920000);
            } else {
                Call4(Engine_ObjectSetPosition, (s32)obj, 0xfe0000, 0, 0x29c0000);
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
                work->bird.turn_x = 0;
                work->bird.turn_y = 0;
                obj->rise_counter++;
                work->bird.drift = 0;
            }
            break;
        case 8:
            obj->rise_counter = 0;
            break;
        }
    } else {
        if (work->bird.turn_x != 0) {
            work->bird.drift -= ((u32)Engine_RandomNext() << 12) >> 16;
            if (work->bird.drift < -0x4000) {
                work->bird.turn_x = step;
            }
        } else {
            work->bird.drift += ((u32)Engine_RandomNext() << 12) >> 16;
            if (work->bird.drift > 0x4000) {
                work->bird.turn_x = 1;
            }
        }
        if (obj->x.fixed > 0xf80000 && obj->x.fixed < 0x1240000) {
            obj->x.fixed += work->bird.drift;
        }
        if (work->bird.turn_y != 0) {
            obj->y.fixed = obj->y.fixed - (((u32)Engine_RandomNext() << 15) >> 16) - 0x8000;
            if (obj->y.fixed < 0) {
                work->bird.turn_y = 0;
            }
        } else {
            obj->y.fixed = obj->y.fixed + (((u32)Engine_RandomNext() << 15) >> 16) + 0x8000;
            if (obj->y.fixed > 0x80000) {
                work->bird.turn_y = 1;
            }
        }
    }
    if ((((u32)Engine_RandomNext() * 100) >> 16) == 0) {
        obj->rise_counter = 1;
    }
    return 1;
}

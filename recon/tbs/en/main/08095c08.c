/* NONMATCHING: 11 differing halfwords, all scheduling: in state 0 the ROM
 * copies source to r1 one instruction earlier, and in state 2 it loads the
 * object pointer after the first origin store. Statement order, const,
 * temporaries and callee prototypes did not move either.
 * Complete owner [08095c08,08095dd0), 456 bytes including both pool words.
 * Baseline verified: 456/456, 11 differing halfwords, 7 aligned edits.
 * H1 ownership evidence: exact EffectSlot_Initialize records x/z together
 * at +0x14/+0x18; UpdateOrbitAndReturn reads that saved pair, as state 3
 * does here. Model only origin capture as a typed inline operation; leave
 * object-flag consumers and all state changes in their original owner.
 * Prediction: object reload stays after the first origin store, with the
 * 12-byte frame and other states unchanged. First model <=10 minutes, one
 * follow-up only, stop/commit by 01:15 Lisbon. Full byte equality plus
 * compare/coverage/verify required for adoption; results stay in this header.
 * H1 result: 456/456, unchanged 11 differing halfwords / 7 aligned edits.
 * Complete diff read: origin-only inline ownership moves neither residual.
 * H1 preserved in e9b306bb2. H2 isolates only the state-0 stack-position
 * copy and its polar-offset consumer, as exact POLAR_OFFSET.C reads/writes
 * x/z and skips y. Exact radial/orbit siblings establish this same input.
 * Prediction: this geometry boundary changes state-0 source-copy scheduling
 * without absorbing target stores, flags, or the whole phase/loop. One try.
 * H2 result: 456/456, unchanged 11 differing halfwords / 7 aligned edits.
 * Both emitted binaries compare identical to baseline, not merely equal
 * scores; complete diffs confirm the same state-0 and state-2 schedules.
 * Origin-pair and coordinate-copy/consumer ownership alone do not recover
 * either invariant. Stop after the model and one follow-up; no adoption,
 * zero new DONE. No whole-phase wrapper, RA sweep, or toolchain changes.
 * 2026-09-29: callees and globals now carry the build's names (gGameState's
 * current owner, gFrameTick, Vector_AddPolarOffset, Camera_WorldToScreen,
 * BattleFx_HasReachedTarget, BattleFx_ClearOwnedSlot), so alchemy permute
 * scores only the two schedules: 280 (2 operand, 4 reordered). In state 0
 * the sched2 dump shows the source reload (mov r1, r9) ready throughout
 * but outranked by the increment's add, which issues first. Ten minutes
 * (53,077 candidates), effect->state++, moving
 * the linked read, one combined flags expression, an object local between
 * the origin stores, x/z temporaries and an origin pointer: none below.
 */
#include "PARTY_STATE.H"
#include "FIXED_MATH.H"
#include "TYPES.H"

struct EffectVector {
    s32 x;
    s32 y;
    s32 z;
};

struct EffectOrigin {
    s32 x;
    s32 z;
};

struct EffectObjectFlags {
    u8 unknown_00[9];
    s8 flags;
};

struct EffectPositionSource {
    u8 unknown_00[8];
    struct EffectVector position;
    u8 unknown_14[60];
    struct EffectObjectFlags *linked_object;
};

struct PhasedParticleSlot {
    struct EffectObjectFlags *object;
    s32 x;
    s32 z;
    s32 target_x;
    s32 target_z;
    struct EffectOrigin origin;
    s32 speed;
    s32 max_speed;
    s32 acceleration;
    s32 scale_x;
    s32 scale_y;
    u16 heading;
    s16 max_turn_step;
    void *callback;
    u16 age;
    s16 callback_delay;
    u8 unknown_3c[4];
    s8 state;
    s8 flag41;
    s8 flag42;
    s8 update_motion;
    s8 render;
    s8 active;
    u8 random_value;
    u8 flags;
};

extern u32 gFrameTick;

void *Engine_ActorGet(s32 actor);

s32 BattleFx_HasReachedTarget(void *object);
void BattleFx_ClearOwnedSlot(void *object);
u32 Random16(void);
void Vector_AddPolarOffset(
    s32 magnitude,
    s32 angle,
    void *position);
void Camera_WorldToScreen(void *position);
void Audio_PlayCue(s32 cue);

/* FAKEMATCH: isolate only the coordinate input and its polar consumer. */
static __inline__ void EffectPosition_AddLaunchOffset(
    struct PhasedParticleSlot *effect, struct EffectVector *position)
{
    position->x = effect->x;
    position->z = effect->z;
    Vector_AddPolarOffset(
        0x780000,
        ((Random16() * 3 << 11) >> 16)
            - ((Random16() * 3 << 11) >> 16)
            + 0xc000,
        position);
}

void BattleEffect_UpdatePhasedRadialParticle(struct PhasedParticleSlot *effect)
{
    struct EffectPositionSource *source;
    struct EffectVector position;
    s8 *state_pointer;
    s32 state;
    u8 linked_flags;
    u8 object_flags;

    source = (struct EffectPositionSource *)Engine_ActorGet(gGameState.current_owner);
    state_pointer = &effect->state;
    state = *state_pointer;

    if (state == 0) {
        effect->x = effect->origin.x;
        effect->z = effect->origin.z;
        EffectPosition_AddLaunchOffset(effect, &position);
        effect->target_x = position.x;
        effect->target_z = position.z;
        effect->acceleration = 0x50000;
        effect->max_speed = 0x50000;
        effect->flag42 = state;
        (*state_pointer)++;

        linked_flags = source->linked_object->flags & 0xc;
        object_flags = effect->object->flags & -13;
        object_flags |= linked_flags;
        effect->object->flags = object_flags;
        effect->flags = 0;
        effect->age = 0;
        if ((gFrameTick & 1) != 0)
            Audio_PlayCue(134);
    } else if (state == 1) {
        if ((s16)effect->age == 3) {
            object_flags = effect->object->flags & -13;
            effect->object->flags = object_flags;
            effect->flags = 4;
        }
        if (BattleFx_HasReachedTarget(effect) == 0)
            (*state_pointer)--;
    } else if (state == 2) {
        if (BattleFx_HasReachedTarget(effect) == 0) {
            effect->origin.x = effect->x;
            effect->origin.z = effect->z;
            effect->object->flags &= -13;
            effect->flags = 4;
            effect->render = 0;
            (*state_pointer)++;
            effect->callback_delay = 40;
        }
    } else if (state == 3) {
        effect->render = 1;
        effect->x = effect->origin.x;
        effect->z = effect->origin.z;
        position.x = source->position.x;
        position.y = source->position.y + 0x140000;
        position.z = source->position.z;
        Camera_WorldToScreen(&position);
        Vector_AddPolarOffset(0x40000, Random16(), &position);
        effect->target_x = position.x;
        effect->target_z = position.z;
        (*state_pointer)++;
        if ((gFrameTick & 1) != 0)
            Audio_PlayCue(145);
    } else if (state == 4) {
        if (BattleFx_HasReachedTarget(effect) == 0)
            (*state_pointer)--;
    } else if (state == 5) {
        if (BattleFx_HasReachedTarget(effect) == 0)
            BattleFx_ClearOwnedSlot(effect);
    }
}

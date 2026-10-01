/* Draft, not exact (2026-09-26): 444 bytes, 122 differing halfwords.
   Typed positions and the canonical EffectSlot replace unrelated arrays
   and padded records. Value snapshots keep each interpolation origin over
   its divide call; the typed model alone gave 448 bytes / 174 halfwords.
   Remaining: step in r8 rather than r7, destination in r7 rather than r8,
   origin-load order, and strength-reduced scale instead of a multiply.
   2026-09-29 alchemy permute (seed 1, 3 jobs, 10 minutes): 34,546
   candidates; the best scored 1795 against 2645 (44 register-only, 9
   operand, 3 reordered, 6 inserted, 6 deleted) after 32 rewrites (reorder
   independent statements, introduce a temporary, swap commutative operands,
   pointer arithmetic or indexing), none of them kept. The gains come from
   moving the destination and origin loads and the waits around through
   temporaries standing for the loaded fields; they depend on one another
   and are not natural, so they stay out of the draft. */
#include "TYPES.H"
#include "EFFECT_0809B11C.H"

struct MotionPosition {
    s32 x;
    s32 y;
    s32 z;
};

struct BattleEffectMotionObject {
    u8 reserved_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[4];
    s32 scale_x;
    s32 scale_y;
};

struct BattleEffectMotionState {
    s32 reserved_00;
    s32 x;
    s32 y;
    s32 z;
    struct BattleEffectMotionObject *target;
    u8 reserved_14[0x44];
    struct EffectSlot records[16];
};

extern struct BattleEffectMotionState *gEffectWork;

struct BattleEffectMotionObject *Object_Spawn(s32, s32, s32, s32);
void Object_SetMode(struct BattleEffectMotionObject *, s32);
void BattleEffect_InitializeSharedScene(void);
s32 __divsi3(s32, s32);
void WaitFrames(s32);
void Audio_PlayCue(s32);
void Camera_WorldToScreen(s32 *);
u32 Random16(void);
void Vector_AddPolarOffset(s32, u32, s32 *);
void EffectSlot_Initialize(struct EffectSlot *, s32, s32, s32);
void ObjectGroup_SetChildValueUnlessFifteenFar(struct BattleEffectMotionObject *, s32);
void Object_Destroy(struct BattleEffectMotionObject *);
void BattleFx_UpdateRadialCamera(struct EffectSlot *);
void BattleFx_PrepareBufferInterpolation(void);

static __inline__ s32 InterpolateCoordinate(s32 from, s32 to, s32 step)
{
    s32 delta;

    delta = to - from;
    return from + __divsi3(step * delta, 10);
}

void BattleEffect_RunFallbackObjectTransition(void)
{
    struct BattleEffectMotionState *state = gEffectWork;
    struct BattleEffectMotionObject *target = state->target;
    struct BattleEffectMotionObject *object;
    struct EffectSlot *record;
    struct MotionPosition position;
    struct MotionPosition origin;
    struct MotionPosition destination;
    s32 step;
    s32 index;

    state->y = target->y;
    object = Object_Spawn(0xfa, 0, 0, 0);
    step = 0;
    Object_SetMode(object, 0);
    if (object == 0)
        return;

    BattleEffect_InitializeSharedScene();
    origin.x = target->x;
    origin.y = target->y + 0x100000;
    origin.z = target->z;
    destination.x = state->x;
    destination.y = state->y + 0x80000;
    destination.z = state->z;
Interpolate:
    {
        s32 scale;

        object->x = InterpolateCoordinate(origin.x, destination.x, step);
        object->y = InterpolateCoordinate(origin.y, destination.y, step);
        object->z = InterpolateCoordinate(origin.z, destination.z, step);
        scale = __divsi3(step * 0xc000, 10) + 0x4000;
        object->scale_x = scale;
        object->scale_y = scale;
        step++;
        WaitFrames(1);
    }
    if (step < 11)
        goto Interpolate;

    WaitFrames(5);
    Object_SetMode(object, 1);
    Audio_PlayCue(0x6c);
    WaitFrames(10);
    Audio_PlayCue(0x6c);
    WaitFrames(10);
    Audio_PlayCue(0x6c);
    WaitFrames(10);
    Audio_PlayCue(0x6d);

    record = &state->records[0];
    index = 15;
    do {
        position.x = object->x;
        position.y = object->y + 0x80000;
        position.z = object->z;
        Camera_WorldToScreen(&position.x);
        Vector_AddPolarOffset(0x40000, Random16(), &position.x);
        EffectSlot_Initialize(record, 0x11d, position.x, position.z);
        EffectSlot_SetCallback(record, BattleFx_UpdateRadialCamera);
        ObjectGroup_SetChildValueUnlessFifteenFar(record->object, 7);
        record++;
        index--;
    } while (index >= 0);

    position.x = object->x;
    position.y = object->y + 0x80000;
    position.z = object->z;
    WaitFrames(8);
    Object_Destroy(object);
    WaitFrames(4);
    WaitFrames(30);
    BattleFx_PrepareBufferInterpolation();
}

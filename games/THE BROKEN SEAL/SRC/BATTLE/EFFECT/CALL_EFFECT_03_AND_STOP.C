#include "RESOURCE.H"
#include "TYPES.H"
#include "FX_SCENE.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void RunBattleEffect03(void);

void BattleFx_CallEffect03AndStop(void)
{
    RunBattleEffect03();
    EffectRuntime_StopCurrentObject();
}

/* Battle effect 03: spawn eight shrinking orbit objects around the target,
 * then one spark object that flashes for 30 frames, runs its emitter and is
 * sent off and destroyed; release the linked resource and run the finish
 * callback. The spark reuses the target and object variables. */

struct BattleEffect03Object {
    u8 reserved_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[4];
    s32 scale_x;
    s32 scale_y;
    u8 reserved_20[16];
    s32 velocity_x;
    s32 velocity_y;
    u8 reserved_38[24];
    void *visual;
    u8 reserved_54;
    u8 mode;
    u8 reserved_56[4];
    u8 unknown_5a;
    u8 reserved_5b[9];
    s16 angle;
    s16 phase;
    u8 reserved_68[4];
    void *callback;
};

struct BattleEffect03Link {
    u8 reserved_00[28];
    u8 marker;
};

extern struct BattleFxScene *gEffectWork;
void BattleFx_UpdateShrinkingOrbitObject(void);
void BattleFx_RunSparkEmitter(void);

void BattleEffect_InitializeSharedScene(void);
struct BattleEffect03Object *Object_Spawn(s32, s32, s32, s32);
struct BattleEffect03Link *Object_ReplaceResourceEntry(void *, struct BattleEffect03Link *);
void WaitFrames(s32);
void Audio_PlayCue(s32);
void Animation_ApplyChildValuesFar(struct BattleEffect03Object *, s32);
void Motion_SetTargetPositionFromMagnitudeAngle(struct BattleEffect03Object *, s32, s32);
void Object_CommitPosition(struct BattleEffect03Object *);
void Object_Destroy(struct BattleEffect03Object *);
void BattleFx_PrepareBufferInterpolation(void);

void RunBattleEffect03(void)
{
    struct BattleFxScene *state = gEffectWork;
    struct BattleEffect03Object *target = state->main_object;
    struct BattleEffect03Object *object;
    struct BattleEffect03Link *last;
    u8 link_marker;
    s32 i;

    BattleEffect_InitializeSharedScene();
    last = 0;
    for (i = 0; i < 8; i++) {
        object = Object_Spawn(
            0xe9, target->x, target->y + 0x200000, target->z);
        if (object != 0) {
            object->scale_y = 0xb333;
            object->scale_x = 0xb333;
            object->callback = BattleFx_UpdateShrinkingOrbitObject;
            object->angle = 0x78;
            object->phase = i << 13;
            object->mode = 4;
            last = Object_ReplaceResourceEntry(object->visual, last);
        }
        WaitFrames(1);
    }

    link_marker = last->marker;
    Audio_PlayCue(0x82);
    WaitFrames(110);
    target = Object_Spawn(0xe9, 0, 0, 0);
    /* FAKEMATCH: the ROM keeps object as a second copy of the new spark (r5 beside r6); a plain object = target is copy-propagated away by gcse. */
    asm("" : "=r"(object) : "0"(target));
    if (target != 0) {
        target->scale_y = 0xb333;
        target->scale_x = 0xb333;
        target->x = state->x;
        target->y = state->y + 0x100000;
        target->z = state->z;
        target->mode = 4;
        Animation_ApplyChildValuesFar(target, 7);
    }

    Audio_PlayCue(0x83);
    WaitFrames(12);
    if (target != 0) {
        for (i = 0; i < 30; i++) {
            if (i & 3)
                Animation_ApplyChildValuesFar(object, 9);
            else
                Animation_ApplyChildValuesFar(object, 10);
            WaitFrames(2);
        }
    }

    Animation_ApplyChildValuesFar(object, 0);
    Audio_PlayCue(0x54);
    if (object != 0) {
        target->callback = BattleFx_RunSparkEmitter;
        target->angle = 0;
        if (state->enabled != 0)
            WaitFrames(128);
        else
            WaitFrames(192);
    }
    if (target != 0) {
        target->angle = -1;
        target->velocity_x = 0x50000;
        target->velocity_y = 0x6666;
        target->unknown_5a = 0;
        Motion_SetTargetPositionFromMagnitudeAngle(target, 0xc00000, 0xe800);
        Object_CommitPosition(target);
        Object_Destroy(target);
    }
    if (link_marker != 0x60)
        Resource_ResetEntry(link_marker);
    if (state->finish_callback != 0)
        state->finish_callback();
    BattleFx_PrepareBufferInterpolation();
}

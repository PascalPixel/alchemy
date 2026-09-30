#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "TYPES.H"
#include "SCENE.H"
#include "OBJECT_EFX.H"

extern u8 *gEventWork;
extern s16 gGameState[];
void *ObjectTable_Get(u32);
void Audio_PlayCue(s32);
void Object_SetMode(void *, s32);
void MapEvent_RunTileTriggerSequence(void);
void CheckObjectMapTile(void);

typedef struct {
    u8 unknown_00[37];
    u8 flag_a;
    u8 flag_b;
} EffectSprite;

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
s32 ObjectDispatch_InitializeFar(void *, s32);
void Motion_SetTargetPositionFromMagnitudeAngle(
    struct Object_08096bec *object, s32 magnitude, s32 angle);
void *Object_Spawn(s32, s32, s32, s32);

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
s32 RunBattleEffect05();

void BattleFx_RunFlashingCallbackSequence(void)
{
    u8 *state = gEventWork;
    s32 index = 250;
    u8 *object = ObjectTable_Get(*(u32 *)&gGameState[index]);
    EffectSprite *record = *(EffectSprite **)(object + 80);
    u8 *entry = *(u8 **)((u8 *)record + 40);
    u32 cycle;
    void (*callback)(void);

    Audio_PlayCue(0x82);
    Object_SetMode(object, 0);
    *(void **)(object + 108) = 0;
    cycle = 0;
    do {
        entry[5] = 7;
        record->flag_a = 1;
        record->flag_b = 2;
        WaitFrames(2);
        record->flag_a = 1;
        record->flag_b = 0;
        WaitFrames(2);
        cycle++;
    } while (cycle <= 9);
    cycle = 0;
    entry[5] = cycle;
    record->flag_b = 2;
    record->flag_a = 1;
    callback = CheckObjectMapTile;
    Scheduler_AddOrUpdateCallback((s32)callback, 0xc80);
    index = 147;
    *(s16 *)&((s32 *)gGameState)[index] = 1;
    callback();
    if (*(s16 *)(state + 382) == 0x2092) {
        MapEvent_RunTileTriggerSequence();
        *(s16 *)(state + 382) = cycle;
    }
}

/* battle/effects/particles/spawn_random_angle_triplet.c */
void BattleFx_SpawnRandomAngleTriplet(void *object)
{
    s32 i;
    void *p;
    s32 phase = 2;
    s32 phase2;
    s16 *pp;

    if ((s32)FIELD_AT_OFFSET(object, s32 *, 0xC) <= (s32)FIELD_AT_OFFSET(object, s32 *, 0x14)) {
        FIELD_AT_OFFSET(object, s16 *, 0x5E) = phase;
        ObjectDispatch_InitializeFar(object, BattleFx_CommonParticleScript);
        p = NULL;
        FIELD_AT_OFFSET(object, void **, 0x6C) = p;
        for (i = 0; i <= 2; i++) {
            p = Object_Spawn(0xF0, FIELD_AT_OFFSET(object, s32 *, 8), FIELD_AT_OFFSET(object, s32 *, 0xC), FIELD_AT_OFFSET(object, s32 *, 0x10));
            if (p == NULL) {
                break;
            }
            FIELD_AT_OFFSET(p, s32 *, 0x1C) = 0x8000;
            FIELD_AT_OFFSET(p, s32 *, 0x18) = 0x8000;
            FIELD_AT_OFFSET(p, s8 *, 0x55) = 2;
            FIELD_AT_OFFSET(p, s32 *, 0x28) = 0x10000;
            FIELD_AT_OFFSET(p, s32 *, 0x30) = (s32)(Random16() + 0x13333);
            Motion_SetTargetPositionFromMagnitudeAngle(
                p, 0x200000, Random16());
            pp = &FIELD_AT_OFFSET(p, s16 *, 0x5E);
            phase2 = 6;
            *pp = phase2;
            ObjectDispatch_InitializeFar(p, BattleFx_CommonParticleScript);
        }
    }
}

/* battle/effects/obj/update_drifting_fall_object.c */
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
void BattleFx_UpdateDriftingFallObject(void *obj)
{
    s32 r;

    FIELD_AT_OFFSET(obj, s32 *, 0xC) = (s32)(FIELD_AT_OFFSET(obj, s32 *, 0xC) + 0xFFFFB334);
    r = Random16();
    FIELD_AT_OFFSET(obj, s32 *, 8) = (s32)(FIELD_AT_OFFSET(obj, s32 *, 8) + (r - Random16()));
    if ((s32)FIELD_AT_OFFSET(obj, s32 *, 0xC) <= (s32)FIELD_AT_OFFSET(obj, s32 *, 0x14)) {
        ObjectDispatch_InitializeFar(obj, BattleFx_CommonParticleScript);
    }
}

void BattleFx_CallEffect05(void)
{
    RunBattleEffect05();
}

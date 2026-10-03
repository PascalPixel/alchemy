#include "SCRIPT_MOTION.H"
#include "ANIMSPR.H"
#include "EVENT_RUNTIME.H"
#include "OBJECT_RUNTIME.H"
#include "FIELDOBJ.H"
#include "OBJECT_LOOKUP.H"
#include "OBJDISP.H"
#include "TYPES.H"
#include "SCENE.H"
#include "MAP.H"
#include "SYSTEM.H"
#include "GAME_STATE.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIXED_MATH.H"
#include "OBJECT_EFX.H"

extern u8 gMapCellBuffer[];

/* map/shared/events/CheckObjectMapTile.c */



/* map/shared/events/MapEvent_RunTileTriggerSequence.c */



void Audio_PlayCue(s32);
void Object_SetMode(struct ObjectRuntime *, s32);
void CheckObjectMapTile(void);

void *ObjectTable_Get(u32);
void MapEvent_RunTileTriggerSequence(void);

typedef struct {
    u8 unknown_00[37];
    u8 flag_a;
    u8 flag_b;
} EffectSprite;

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
void Motion_SetTargetPositionFromMagnitudeAngle(
    struct ObjectRuntime *object, s32 magnitude, s32 angle);
void *Object_Spawn(s32, s32, s32, s32);
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
s32 RunBattleEffect05();

void CheckObjectMapTile(void);

void CheckObjectMapTile(void)
{
    u32 runtime_slot_address;
    struct EventRuntime *runtime;
    struct ObjectRuntime *object;
    u8 *tile;
    s32 x;
    s32 y;

    runtime_slot_address = (u32)&gWork;
    runtime = gWork;
    object = ObjectTable_Get(gGameState.selected_actor);
    /* The map-state pointer slot is 19 words before the runtime pointer slot. */
    tile = (u8 *)*(struct MapState **)(runtime_slot_address - 76);

    if (runtime->mode_19e == 3) {
        u32 tile_x;
        u32 tile_y;

        x = object->x;
        if (x < 0)
            x += 0x1fffff;
        tile_x = (x >> 21) & 31;

        y = object->z;
        if (y < 0)
            y += 0x1fffff;
        tile_y = (y >> 21) & 31;

        tile = (u8 *)gMapBlocks +
            ((tile_x + (tile_y << 5)) << 2);
    } else {
        if (object->terrain_id <= 2) {
            tile = (u8 *)((struct MapState *)tile)
                ->layers[object->terrain_id].cells;
        } else
            tile = (u8 *)gMapCellBuffer;

        x = object->x;
        if (x < 0)
            x += 0xfffff;
        {
            u32 tile_x = x >> 20;

            y = object->z;
            if (y < 0)
                y += 0xfffff;

            tile = (u8 *)((u32)tile +
                ((tile_x + ((u32)(y >> 20) << 7)) << 2));
        }
    }

    if (tile[2] != 0xfb)
        ((struct EventWork *)runtime)->psynergy_request = 0x2092;
}

void MapEvent_RunTileTriggerSequence(void)
{
    struct ObjectRuntime *object;
    struct AnimationObject *state;
    struct AnimationEntry *controller;
    u32 i;

    object = ObjectTable_Get(gGameState.selected_actor);
    state = object->animation;
    controller = state->entries[0];

    Audio_PlayCue(154);
    Scheduler_RemoveCallback(CheckObjectMapTile);
    Object_SetMode(object, 0);
    ((struct ScriptMotionObject *)object)->hook = 0;

    for (i = 0; i < 5; ++i) {
        controller->param = 7;
        state->dirty = 1;
        state->flags = 2;
        WaitFrames(2);
        state->dirty = 1;
        state->flags = 0;
        WaitFrames(2);
    }

    for (i = 0; i < 5; ++i) {
        controller->param = 7;
        state->dirty = 1;
        state->flags = 0;
        WaitFrames(2);
        controller->param = 0;
        state->dirty = 1;
        WaitFrames(2);
    }

    state->flags = 1;
    gGameState.cloaked = 0;
}

void BattleFx_ScheduleCallbackWhenValue24cSet(void)
{
    if (gGameState.cloaked != 0) {
        Scheduler_AddOrUpdateCallback((s32)&CheckObjectMapTile, 0xc80);
    }
}

void BattleFx_RunFlashingCallbackSequence(void)
{
    u8 *state = (u8 *)gEventWork;
    u8 *object = ObjectTable_Get(gGameState.selected_actor);
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
    gGameState.cloaked = 1;
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
        ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)BattleFx_CommonParticleScript);
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
            ObjectDispatch_InitializeFar((struct DispatchObject *)p, (u32)BattleFx_CommonParticleScript);
        }
    }
}

/* battle/effects/obj/update_drifting_fall_object.c */
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
void BattleFx_UpdateDriftingFallObject(struct ObjectRuntime *object)
{
    s32 r;

    object->y = (s32)((u32)object->y - 0x4ccc);
    r = Random16();
    object->x += r - Random16();
    if (object->y <= object->terrain_height) {
        ObjectDispatch_InitializeFar((struct DispatchObject *)object,
            (u32)BattleFx_CommonParticleScript);
    }
}
void BattleFx_CallEffect05(void)
{
    RunBattleEffect05();
}

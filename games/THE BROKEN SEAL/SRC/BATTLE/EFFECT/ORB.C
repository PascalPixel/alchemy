#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
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

void Audio_PlayCue(s32);
void Object_SetMode(struct ObjectRuntime *, s32);

void *ObjectTable_Get(u32);

typedef struct {
    u8 unknown_00[37];
    u8 flag_a;
    u8 flag_b;
} EffectSprite;

void *Object_Spawn(s32, s32, s32, s32);
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

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

            tile = (u8 *)(tile +
                ((tile_x + ((y >> 20) << 7)) << 2));
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

void BattleFx_ScheduleCloakedMapCheck(void)
{
    if (gGameState.cloaked != 0) {
        Scheduler_AddOrUpdateCallback((SchedulerCallback)CheckObjectMapTile, 0xc80);
    }
}

void BattleFx_RunFlashingCallbackSequence(void)
{
    struct EventWork *state = gEventWork;
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
    Scheduler_AddOrUpdateCallback(callback, 0xc80);
    gGameState.cloaked = 1;
    callback();
    if (state->psynergy_request == 0x2092) {
        MapEvent_RunTileTriggerSequence();
        state->psynergy_request = cycle;
    }
}

void BattleFx_SpawnRandomAngleTriplet(void *object)
{
    s32 i;
    void *p;
    s32 phase = 2;
    s32 phase2;
    s16 *pp;

    if (FIELD_AT_OFFSET(object, s32 *, 0xC) <= FIELD_AT_OFFSET(object, s32 *, 0x14)) {
        FIELD_AT_OFFSET(object, s16 *, 0x5E) = phase;
        ObjectDispatch_InitializeFar((struct DispatchObject *)object, BattleFx_CommonParticleScript);
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
            FIELD_AT_OFFSET(p, s32 *, 0x30) = Random16() + 0x13333;
            Motion_SetTargetPositionFromMagnitudeAngle(
                p, 0x200000, Random16());
            pp = &FIELD_AT_OFFSET(p, s16 *, 0x5E);
            phase2 = 6;
            *pp = phase2;
            ObjectDispatch_InitializeFar((struct DispatchObject *)p, BattleFx_CommonParticleScript);
        }
    }
}

void BattleFx_UpdateDriftingFallObject(struct ObjectRuntime *object)
{
    s32 r;

    object->y = object->y - 0x4ccc;
    r = Random16();
    object->x += r - Random16();
    if (object->y <= object->terrain_height) {
        ObjectDispatch_InitializeFar((struct DispatchObject *)object,
            BattleFx_CommonParticleScript);
    }
}
void BattleFx_CallEffect05(void)
{
    RunBattleEffect05();
}

struct Effect05Point {
    s32 x, y, z;
};

struct Effect05Target {
    u8 reserved_00[8];
    struct Effect05Point position;
};

struct Effect05Sprite {
    u32 unknown_00[2];
    u8 unknown_08;
    u32 unknown_09_0 : 2;
    u32 mode : 2;
    u32 unknown_09_4 : 4;
};

struct Effect05Object {
    u8 reserved_00[8];
    s32 x, y, z;
    s32 altitude;
    s32 scale_x, scale_y;
    u8 reserved_20[0x30];
    struct Effect05Sprite *sprite;
    u8 reserved_54;
    u8 flag;
    u8 reserved_56[0x16];
    void *callback;
};

struct Effect05State {
    s32 angle;
    s32 x, y, z;
    struct Effect05Target *target;
    s32 initialized;
    u8 reserved_18[8];
    u8 high_arc;
    u8 reserved_21[0x13];
    u8 variant;
    u8 reserved_35[0x10];
    u8 alternate;
};

extern struct Effect05State *gEffectWork;
void Vector_AddPolarOffset(s32, s32, struct Effect05Point *);
void ObjectDispatch_ReleaseFar(struct Effect05Object *);
void Animation_ApplyChildValuesFar(struct Effect05Object *, s32);
void AudioCommand_PlayFar(s32);

static __inline__ s32 Effect05_Interpolate(s32 start, s32 end, s32 step)
{
    return start + step * (end - start) / 10;
}

void RunBattleEffect05(void)
{
    struct Effect05Point spawn;
    struct Effect05Point start;
    struct Effect05Point end;
    struct Effect05State *state = gEffectWork;
    struct Effect05Target *target = state->target;
    struct Effect05Object *main;
    struct Effect05Point *from;
    struct Effect05Point *to;
    s32 i;
    s32 steps = 11;
    s32 count;
    s32 y;
    struct Effect05Object *particle;

    main = Object_Spawn(0xef, i = 0, 0, 0);
    if (main == 0)
        return;
    BattleEffect_InitializeSharedScene();
    AudioCommand_PlayFar(0x8a);
    if (state->initialized == 0) {
        state->x = target->position.x;
        state->z = target->position.z;
        Vector_AddPolarOffset(0x100000, state->angle, (struct Effect05Point *)&state->x);
        state->y = Map_GetTerrainHeightFar(0, state->x, state->z);
    }
    from = &start;
    from->x = target->position.x;
    from->y = target->position.y + 0x100000;
    from->z = target->position.z;
    to = &end;
    to->x = state->x;
    y = state->y;
    to->y = y + 0x200000;
    to->z = state->z;
    if ((s8)state->variant != 0)
        to->y = y + 0x500000;

    for (; i < steps; i++) {
        s32 scale;

        {
            struct Effect05Point *a = &start;
            struct Effect05Point *b = &end;
            s32 e, t;

            e = b->x; t = a->x; t += i * (e - t) / 10; main->x = t;
            e = b->y; t = a->y; t += i * (e - t) / 10; main->y = t;
            e = b->z; t = a->z; t += i * (e - t) / 10; main->z = t;
        }
        scale = Effect05_Interpolate(0x4000, 0x10000, i);
        main->scale_x = scale;
        main->scale_y = scale;
        WaitFrames(1);
    }
    WaitFrames(10);

    if ((s8)state->alternate == 0) {
        count = 10;
        if ((s8)state->high_arc == 0)
            count = 24;
        for (i = 0; i < count; i++) {
            spawn.x = main->x;
            spawn.y = main->y;
            spawn.z = main->z;
            Vector_AddPolarOffset(Random16() * 5 + 0x30000, Random16(), &spawn);
            if (i == count - 1) {
                WaitFrames(25);
                spawn.x = main->x;
                spawn.y = main->y;
                spawn.z = main->z;
            }
            particle = Object_Spawn(0xf0, spawn.x, spawn.y, spawn.z);
            if (particle != 0) {
                particle->altitude = spawn.y - 0x200000;
                particle->callback = (void *)BattleFx_SpawnRandomAngleTriplet;
                particle->flag = 2;
            }
            AudioCommand_PlayFar(0x84);
            WaitFrames(6);
        }
        WaitFrames(10);
    } else {
        if ((s8)state->high_arc != 0)
            count = 10;
        else
            count = 30;
        for (i = 0; i < count; i++) {
            spawn.x = main->x;
            spawn.y = main->y;
            spawn.z = main->z;
            Vector_AddPolarOffset(Random16() * 5 + 0x30000, Random16(), &spawn);
            particle = Object_Spawn(0x11c, spawn.x, spawn.y, spawn.z);
            if (particle != 0) {
                particle->callback = (void *)BattleFx_UpdateDriftingFallObject;
                particle->flag = 0;
                particle->sprite->mode = 2;
                Object_SetMode((struct ObjectRuntime *)particle, 8);
                Animation_ApplyChildValuesFar(particle, 7);
            }
            WaitFrames(6);
        }
        WaitFrames(70);
    }

    i = 0;
    do {
        s32 scale;

        {
            struct Effect05Point *a = &end;
            struct Effect05Point *b = &start;
            s32 e, t;

            e = b->x; t = a->x; t += i * (e - t) / 10; main->x = t;
            e = b->y; t = a->y; t += i * (e - t) / 10; main->y = t;
            e = b->z; t = a->z; t += i * (e - t) / 10; main->z = t;
        }
        scale = Effect05_Interpolate(0x10000, 0x4000, i);
        main->scale_x = scale;
        main->scale_y = scale;
        WaitFrames(1);
        i++;
    } while (i < steps);
    ObjectDispatch_ReleaseFar(main);
    BattleFx_PrepareBufferInterpolation();
}

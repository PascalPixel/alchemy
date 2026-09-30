#include "RAMAKAN.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "SCENE_IDS.H"
#include "CALL.H"
#include "IWRAM_CALL.H"

extern u32 gFrameCount;
void Engine_AudioPlayCue();
s32 Engine_RandomNext();
void Effect_Spawn();

struct EffectParams {
    u8 pad00[8];
    s32 scaleX;
    s32 scaleY;
    u8 pad10[18];
    u16 angle;
    u8 pad24[4];
};

extern const struct SceneEntrance gRamakanSabakuEntrances1[];
extern const struct SceneEntrance gRamakanSabakuEntrances2[];
extern const struct SceneEntrance gRamakanSabakuEntrances3[];
extern const struct SceneEntrance gRamakanSabakuEntrances4[];
extern const struct SceneEntrance gRamakanSabakuEntrancesOther[];

extern const u32 RamakanSabaku_Exits[];

extern const struct ScenePlacement gRamakanSabakuPlacements1[];
extern const struct ScenePlacement gRamakanSabakuPlacements2[];
extern const struct ScenePlacement gRamakanSabakuPlacements3[];
extern const struct ScenePlacement gRamakanSabakuPlacementsOther[];

extern const struct SceneEvent RamakanSabaku_Events[];

void Engine_GameFlagClear();
void Engine_MapCopyCells();
void Engine_MapCopyCellAttributes();
void Engine_MapCopyCellsLayered();
void Engine_ActorSetPosition();
s32 Engine_DisplayScrollStartHBlankDma();

s32 Engine_GameFlagIsSet();
s32 BattleFx_EmitRandomParticle();
s32 DisplayScroll_DisarmHBlankDma();

void Party_SetFields1ceAnd1d0(s32 scene, s32 entrance);
void BattleFx_SetWeightedResult(s32 value, s32 weight);

void FieldScene_ApplyActor13Values3And3(void) { BattleFx_RunPageEffectForSlot(13, 3, 3); }

/*
 * The countdown is tested at the top of the loop and decremented inside the
 * body, after the call.  A post-decrement test would move the subtract ahead
 * of the call.
 */
void OverlayObject_WaitUntilField12BelowLimit(u8 *o, s32 limit)
{
    s32 frames = 60;

    while (frames != 0) {
        Task_Wait(1);
        frames--;
        if (*(s32 *)(o + 12) <= limit) break;
    }
}

/* Every eighth frame play the sand cue; every sixteenth spawn a sand effect
 * at the actor with a random heading. */
s32 RamakanSabaku_EmitSandEffect(u8 *actor)
{
    struct EffectParams params;
    s32 phase;

    if ((*(volatile u32 *)&gFrameCount & 7) == 0) {
        Engine_AudioPlayCue(118);
    }
    phase = *(volatile u32 *)&gFrameCount & 15;
    if (phase != 0) {
        return 0;
    }
    params.scaleX = 0xcccc;
    params.scaleY = 0xcccc;
    params.angle = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
    Effect_Spawn(*(s32 *)(actor + 8), *(s32 *)(actor + 12), *(s32 *)(actor + 16), 0, phase, phase, 0x00880001, &params);
    return 0;
}

/* Where the party appears in each of the desert's four areas. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&SceneId_RamakanSabaku1) {
        return gRamakanSabakuEntrances1;
    }
    if (v == (s32)&SceneId_RamakanSabaku2) {
        return gRamakanSabakuEntrances2;
    }
    if (v == (s32)&SceneId_RamakanSabaku3) {
        return gRamakanSabakuEntrances3;
    }
    if (v == (s32)&SceneId_RamakanSabaku4) {
        return gRamakanSabakuEntrances4;
    }
    return gRamakanSabakuEntrancesOther;
}

/* The desert's regions and exits, between its scene-dependent getters. */
const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return RamakanSabaku_Exits;
}

/* The actors placed in each of the desert's areas. Entering the third area
   by its fifth entrance sets flag 0x90a first. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == (s32)&SceneId_RamakanSabaku3) {
        if (gGameState.entrance == 5) {
            GameFlag_Set(0x90a);
        }
    }
    if (gGameState.scene == (s32)&SceneId_RamakanSabaku1) {
        return gRamakanSabakuPlacements1;
    }
    if (gGameState.scene == (s32)&SceneId_RamakanSabaku2) {
        return gRamakanSabakuPlacements2;
    }
    if (gGameState.scene == (s32)&SceneId_RamakanSabaku3) {
        return gRamakanSabakuPlacements3;
    }
    return gRamakanSabakuPlacementsOther;
}

void FieldScene_RunFlags8B2And8B3Steps(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x8b2) == 0) {
        if (GameFlag_IsSet(0x8b3) == 0) {
            GameFlag_Set(0x8b3);
            GameFlag_Set(0x8b2);
        }
    }
    Audio_PlayCue(123);
    Event_RequestExit(3);
    Event_End();
}

const struct SceneEvent *Scene_GetEvents(void)
{
    return RamakanSabaku_Events;
}

/* Lamakan Desert entry: clear flag 0x201, lay out the map cells for the
 * entrance taken, park the scene actors and, outside the fourth area
 * (SceneId_RamakanSabaku4), start the heat-shimmer scroll. */
s32 RamakanSabaku_ApplyEntryState(void)
{
    u32 i;
    s32 record;
    s32 v5;

    Engine_GameFlagClear(0x201);
    if (gGameState.scene == (s32)&SceneId_RamakanSabaku1) {
        Call6(Engine_MapCopyCells, 70, 68, 4, 2, 22, 7);
        Call6(Engine_MapCopyCells, 70, 68, 4, 2, 8, 10);
        Engine_MapCopyCells(70, 68, 4, 2, 23, 21);
        Engine_MapCopyCellAttributes(70, 68, 4, 1, 23, 23);
        Call6(Engine_MapCopyCells, 70, 68, 4, 2, 16, 42);
        Call6(Engine_MapCopyCells, 70, 68, 4, 2, 36, 44);
        Call6(Engine_MapCopyCells, 70, 68, 4, 2, 14, 55);
    } else {
        if (gGameState.scene != (s32)&SceneId_RamakanSabaku2) {
        } else {
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 42, 5);
            Engine_MapCopyCells(70, 68, 4, 2, 20, 11);
            Engine_MapCopyCellAttributes(70, 68, 4, 1, 20, 13);
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 14, 12);
            Engine_MapCopyCells(70, 68, 4, 2, 56, 18);
            Engine_MapCopyCells(70, 68, 4, 2, 7, 22);
            Engine_MapCopyCellAttributes(70, 68, 4, 1, 7, 24);
            Engine_MapCopyCells(70, 68, 4, 2, 44, 23);
            Engine_MapCopyCellAttributes(70, 68, 4, 1, 44, 25);
            Engine_MapCopyCells(70, 68, 4, 2, 38, 24);
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 26, 28);
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 17, 35);
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 50, 36);
            Engine_MapCopyCells(70, 68, 4, 2, 34, 43);
            Engine_MapCopyCellAttributes(70, 68, 4, 1, 34, 45);
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 6, 46);
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 27, 55);
            Engine_MapCopyCells(70, 68, 4, 2, 43, 56);
            goto clear_actors;
        }
        if (gGameState.scene == (s32)&SceneId_RamakanSabaku3) {
            Engine_MapCopyCellsLayered(69, 99, 4, 2, 8, 16);
            Engine_MapCopyCellsLayered(69, 99, 4, 2, 6, 20);
            Engine_MapCopyCellsLayered(69, 99, 4, 2, 10, 23);
            Engine_MapCopyCellAttributes(69, 99, 4, 2, 8, 14);
            Call6(Engine_MapCopyCellAttributes, 69, 99, 4, 2, 6, 18);
            Engine_MapCopyCellAttributes(69, 99, 4, 1, 6, 20);
            Engine_MapCopyCellAttributes(69, 99, 4, 2, 10, 21);
            Call6(Engine_MapCopyCells, 0, 121, 5, 7, 8, 32);
            Engine_MapCopyCells(0, 121, 5, 7, 43, 32);
            Engine_MapCopyCells(6, 120, 3, 1, 9, 5);
            Engine_MapCopyCells(9, 120, 3, 1, 44, 5);
            Engine_MapCopyCellAttributes(9, 0, 3, 3, 9, 6);
        }
    }
    clear_actors:;
    Engine_ActorSetPosition(8, 0, 0);
    Engine_ActorSetPosition(9, 0, 0);
    Engine_ActorSetPosition(10, 0, 0);
    Engine_ActorSetPosition(11, 0, 0);
    Engine_ActorSetPosition(12, 0, 0);
    Engine_ActorSetPosition(13, 0, 0);
    v5 = 100;
    do {
        record = ((s32 (*)())Engine_MapObjectSetPosition)(v5, 0, 0);
        v5 = (v5 + 1);
    } while (v5 <= 107);
    if (gGameState.scene != (s32)&SceneId_RamakanSabaku4) {
        record = Value7(Engine_DisplayScrollStartHBlankDma, 0, 0x40000, 0x10000, 0x2000, 0x10000, 0x8000, 0x4000);
        return record;
    }
    return record;
}

/* Lamakan Desert: open the map cells and place the actors for the area
 * variant stored at +0x1c0 of the game state, then reset the map objects. */
s32 RamakanSabaku_ConfigureAreaLayout(void)
{
    s32 i;
    s32 record;

    Engine_GameFlagClear(0x200);
    Engine_GameFlagSet(0x201);
    if (gGameState.scene == (s32)&SceneId_RamakanSabaku1) {
        Engine_MapCopyCells(64, 126, 4, 2, 22, 7);
        Call6(Engine_MapCopyCells, 68, 126, 4, 2, 8, 10);
        Engine_MapCopyCells(72, 126, 4, 2, 23, 21);
        Engine_MapCopyCellAttributes(72, 126, 4, 2, 23, 22);
        Call6(Engine_MapCopyCells, 76, 126, 4, 2, 16, 42);
        Call6(Engine_MapCopyCells, 80, 126, 4, 2, 36, 44);
        Call6(Engine_MapCopyCells, 84, 126, 4, 2, 14, 55);
        Call3(Engine_ActorSetPosition, 9, 0x1900000, 0x16c0000);
    } else {
        if (gGameState.scene != (s32)&SceneId_RamakanSabaku2) {
            goto third_area;
        }
        Call6(Engine_MapCopyCells, 64, 126, 4, 2, 42, 5);
        Engine_MapCopyCells(68, 126, 4, 2, 20, 11);
        Call6(Engine_MapCopyCellAttributes, 68, 126, 4, 2, 20, 12);
        Engine_MapCopyCells(72, 126, 4, 2, 14, 12);
        Engine_MapCopyCells(76, 126, 4, 2, 56, 18);
        Engine_MapCopyCells(80, 126, 4, 2, 7, 22);
        Call6(Engine_MapCopyCellAttributes, 80, 126, 4, 2, 7, 23);
        Engine_MapCopyCells(84, 126, 4, 2, 44, 23);
        Engine_MapCopyCellAttributes(84, 126, 4, 2, 44, 24);
        Engine_MapCopyCells(88, 126, 4, 2, 38, 24);
        Call6(Engine_MapCopyCells, 92, 126, 4, 2, 26, 28);
        Call6(Engine_MapCopyCells, 96, 126, 4, 2, 17, 35);
        Call6(Engine_MapCopyCells, 100, 126, 4, 2, 50, 36);
        Engine_MapCopyCells(104, 126, 4, 2, 34, 43);
        Engine_MapCopyCellAttributes(104, 126, 4, 2, 34, 44);
        Call6(Engine_MapCopyCells, 108, 126, 4, 2, 6, 46);
        Call6(Engine_MapCopyCells, 112, 126, 4, 2, 27, 55);
        Engine_MapCopyCells(116, 126, 4, 2, 43, 56);
        Call3(Engine_ActorSetPosition, 9, 0x1600000, 0xcc0000);
        Call3(Engine_ActorSetPosition, 10, 0x2e00000, 0x18c0000);
        Call3(Engine_ActorSetPosition, 11, 0x900000, 0x17c0000);
        Call3(Engine_ActorSetPosition, 12, 0x2400000, 0x2cc0000);
        Call3(Engine_ActorSetPosition, 13, 0x2880000, 0x1980000);
    }
    goto reset_objects;
    third_area:;
    if (gGameState.scene == (s32)&SceneId_RamakanSabaku3) {
        Engine_MapCopyCells(64, 124, 4, 4, 8, 14);
        Engine_MapCopyCells(68, 124, 4, 4, 6, 18);
        Engine_MapCopyCellAttributes(68, 124, 4, 1, 6, 20);
        Call6(Engine_MapCopyCells, 72, 124, 4, 4, 10, 21);
        Call6(Engine_MapCopyCells, 10, 121, 5, 7, 8, 32);
        Engine_MapCopyCells(5, 121, 5, 7, 43, 32);
        Call6(Engine_MapCopyCells, 0, 120, 3, 1, 9, 5);
        Engine_MapCopyCells(3, 120, 3, 1, 44, 5);
        Call3(Engine_ActorSetPosition, 8, 0xa80000, 0x5c0000);
        Call3(Engine_ActorSetPosition, 9, 0x800000, 0x13c0000);
        Engine_MapCopyCellAttributes(6, 0, 3, 3, 9, 6);
        if (Engine_GameFlagIsSet(0x90a) == 0) {
            Engine_MapCopyCells(0, 119, 3, 1, 9, 5);
        }
    }
    reset_objects:;
    for (i = 100; i <= 107; i++) {
        Call3(Engine_MapObjectSetPosition, i, -1, -1);
    }
    record = BattleFx_EmitRandomParticle();
    if (gGameState.scene != (s32)&SceneId_RamakanSabaku4) {
        record = DisplayScroll_DisarmHBlankDma();
        return record;
    }
    return record;
}

/* Actor 8 repeats its motion, and the party is set to come back to the
   desert's third area by its fifth entrance. */
void RamakanSabaku_ReturnToArea3(void)
{
    Actor_RunRepeatedMotion(8, 2);
    Party_SetFields1ceAnd1d0((s32)&SceneId_RamakanSabaku3, 5);
    /* FAKEMATCH: the do/while loads the game state's base before the 0x22b
       offset, which fixes their registers and literal-pool order. */
    do {
        gGameState.unknown_200[0x22b - 0x200] = 3;
    } while (0);
    BattleFx_SetWeightedResult(53, 5);
}

void FieldScene_RunScene3a5_02000c6c(s32 a0)
{
    s32 i;
    s32 p8;
    s32 record;
    s32 value;
    s32 v5;
    s32 v6;

    p8 = a0;
    for (i = 0; i <= 2; i++) {
        value = Engine_RandomNext();
        v6 = (u32)((value << 1) + value) >> 16;
        v5 = v6 + 0x303;
        record = GameFlag_IsSet(v5);
        if (record == 0) {
            GameFlag_Set(v5);
            break;
        }
    }
    Event_Begin();
    Event_SetMessage((s32)((s32)(((s32)p8 << 1) + p8) + v6) + 0x1a10);
    Event_ShowMessage((v6 + 1), 0);
    Event_End();
}

/* The desert crossing in quarters: the percentage of the way the party has
   come clears the quarter flags it has fallen back behind and raises the
   trigger of each quarter it has newly passed. */
void RamakanSabaku_RaiseQuarterTriggers(void)
{
    struct EventWork *work = (struct EventWork *)gWork;
    u8 *state = (u8 *)&gGameState;
    s32 percent = *(s16 *)(state + 0x232) * 100 / *(s16 *)(state + 0x22c);

    if (GameFlag_IsSet(0x201) != 0)
        return;
    if (GameFlag_IsSet(0x302) != 0 && percent <= 74) {
        GameFlag_Clear(0x302);
        GameFlag_Clear(0x303);
        GameFlag_Clear(0x304);
        GameFlag_Clear(0x305);
    }
    if (GameFlag_IsSet(0x301) != 0 && percent <= 49) {
        GameFlag_Clear(0x301);
        GameFlag_Clear(0x303);
        GameFlag_Clear(0x304);
        GameFlag_Clear(0x305);
    }
    if (GameFlag_IsSet(0x300) != 0 && percent <= 24) {
        GameFlag_Clear(0x300);
        GameFlag_Clear(0x303);
        GameFlag_Clear(0x304);
        GameFlag_Clear(0x305);
    }
    if (GameFlag_IsSet(0x300) == 0 && percent > 24) {
        GameFlag_Set(0x300);
        work->raised_trigger = 1;
    }
    if (GameFlag_IsSet(0x301) == 0 && percent > 49) {
        GameFlag_Set(0x301);
        work->raised_trigger = 2;
    }
    if (GameFlag_IsSet(0x302) == 0 && percent > 74) {
        GameFlag_Set(0x302);
        work->raised_trigger = 3;
    }
}

/* The whole-pixel distance across the ground between two 16.16 positions of
   three words each, x and z apart; the resident square root takes the sum
   of the squares. */
s32 RamakanSabaku_CalculatePlanarDistance(s32 *position_a, s32 *position_b)
{
    s32 dx = (*position_b++ - *position_a++) >> 16;
    s32 dz = (*position_b - position_a[1]) >> 16;
    s32 dz_squared = dz * dz;
    s32 dx_squared = dx * dx;

    return Iwram_Sqrt(dx_squared + dz_squared);
}

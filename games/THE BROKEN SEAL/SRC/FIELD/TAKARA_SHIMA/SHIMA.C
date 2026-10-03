#include "TYPES.H"
#include "EDITION.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "STAGED_ACTOR.H"
#include "FIELD_EFFECT.H"
#include "CALL.H"
#include "SCENE_IDS.H"

enum {
    /* Message 0x182 + 181. */
    ITEM_NUT = 181
};

/* Integrate position, velocity, rate and sprite angle for one scene effect.
   Signed division preserves decay toward zero for negative Z velocity. */
struct Sprite {
    u8 pad00[9];
    u8 flags9;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state26;
};

struct Effect {
    u8 pad00[8];
    s32 position[3];
    u8 pad14[4];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flatla3;
    u8 pad24[12];
    s32 rate30;
    s32 rate34;
    u8 pad38[12];
    s32 velocity[3];
    struct Sprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

struct T {
    u8 pad00[30];
    u16 f1e;
};

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    u8 pad10[32];
    s32 f30;
    s32 f34;
    s32 f38;
    s32 f3c;
    u8 pad40[16];
    struct T *f50;
};

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[28];
    s32 f30;
    s32 f34;
} Obj;

struct V6 {
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 e;
    s32 f;
};

struct S_02000474 {
    s32 f00;
    u16 f04;
    u16 f06;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[20];
    s32 f28;
    u8 pad2c[4];
    s32 f30;
    s32 f34;
    u8 pad38[29];
    u8 f55;
};

struct V {
    s32 a;
    s32 b;
    s32 c;
};

struct Owner {
    u8 unk0[9];
    u8 unk9_0 : 2;
    u8 mode : 2;
    u8 unk9_4 : 4;
};

struct S_020009dc {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[15];
    u8 f23;
};

struct SceneObject {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

struct EffectParams {
    u8 filler00[0x24];
    s32 callback;
};

typedef union {
    s32 w;
    s16 h[2];
} RecWord;

struct S_02001b14 { s32 pad[2]; s32 f08; s32 f0c; s32 f10; };

struct S_02001bbc {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001c84 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001d2c {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001de0 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001e5c {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001ef4 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001f78 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002004 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002080 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_0200216c {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002200 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_020022c8 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_020023c4 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002450 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

typedef struct OrbitingSceneObjectSprite {
    u8 padding_00[5];
    u8 flags_05_low : 5;
    u8 flags_05_bit_5 : 1;
    u8 flags_05_high : 2;
    u8 padding_06[3];
    u8 flags_09_low : 2;
    u8 flags_09_mode : 2;
    u8 flags_09_high : 4;
    u8 padding_0a[18];
    u8 palette;
    u8 padding_1d[10];
    u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject {
    u8 padding_00[8];
    s32 x;
    s32 y;
    u8 padding_10[19];
    u8 flags_23;
    u8 padding_24[12];
    s32 orbit_angle;
    u8 padding_34[4];
    s32 orbit_center_x;
    s32 orbit_center_y;
    u8 padding_40[16];
    OrbitingSceneObjectSprite *sprite;
    u8 padding_54;
    u8 mode;
    u8 state;
    u8 padding_57[5];
    u8 active;
    u8 padding_5d[4];
    u8 visible;
    u8 padding_62[10];
    u32 callback;
} OrbitingSceneObject;

extern const struct SceneEntrance gTakaraShimaEntrances1[];
extern const struct SceneEntrance gTakaraShimaEntrances2[];
extern const struct SceneEntrance gTakaraShimaEntrances3[];
extern const struct SceneEntrance gTakaraShimaEntrances4[];
extern const struct SceneEntrance gTakaraShimaEntrances5[];
extern const struct SceneEntrance gTakaraShimaEntrancesOther[];

extern const u32 gTakaraShimaExits[];

extern const struct ScenePlacement gTakaraShimaPlacements1[];
extern const struct ScenePlacement gTakaraShimaPlacements3[];
extern const struct ScenePlacement gTakaraShimaPlacements6To14[];
extern const struct ScenePlacement gTakaraShimaPlacementsOther[];

void Party_SetFields1ceAnd1d0(s32 arg0, s32 arg1);
void BattleFx_SetWeightedResult(s32 arg0, s32 arg1);

extern const struct SceneEvent gTakaraShimaEvents1[];
extern const struct SceneEvent gTakaraShimaEvents2[];
extern const struct SceneEvent gTakaraShimaEvents3[];
extern const struct SceneEvent gTakaraShimaEvents4[];
extern const struct SceneEvent gTakaraShimaEvents5[];
extern const struct SceneEvent gTakaraShimaEventsOther[];

void Battle_Reset(s32 arg0);

void InitializeEscapeSceneActors(void);
void FieldScene_RunScene3b2_0200167c(void);
s32 FieldScene_RedrawActorFootprint(s32 id);
void InitializeSwayingSceneObject();
void FieldScene_RunScene3b2SequenceA(void);
void FieldScene_RunScene3b2_02001494(void);
void SetMapCellCollision();

union GameStateRows {
    s16 halves[512][1];
};

void Engine_EventWait();
void SpawnRadialEffectBurst();
void Engine_AudioPlayCue();
void Engine_ActorSetSpriteFlags();
void ObjectMotion_SetActionVariant();
void Engine_MapCopyCellAttributes();

extern u8 TakaraShima_EntranceCells[];

void Engine_EventBegin();
void BattleFx_SetWeightedResult();
void Engine_EventEnd();
void Engine_MapAnimateCells();
void Engine_EventRequestExit();

void Object_SetPosition(Obj *, s32, s32, s32);
void Object_CommitPosition(Obj *);
void Vector_AddPolarOffset(s32 arg0, s32 arg1, struct V *arg2);
s32 Object_CheckMovementCollision(struct S *arg0, struct V *arg1);
void MarkGridLeftOfSceneActor(s32 actor_mode, s32 grid_value, s32 grid_attribute, s32 unused);
void MarkGridAboveSceneActor(s32 actor_mode, s32 grid_value, s32 grid_attribute, s32 unused);
void SetMapCellCollision(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 TryPushBlockingSceneActor(struct S_02000474 *actor, struct V *requested);
s32 StagedActor_FindClearPosition(struct StagedActorProbe *probe);

void PositionSceneActorPair(s32 actor, s32 x, s32 z);

u8 *Object_GetByIdFar();
void PositionSceneActorPair(s32 actor_id, s32 x_offset, s32 z_offset);

/* Deliberate no-op callback. */
void NoOpEffectCallback(void) {}

/* Where the party appears on the island: its first five scenes have their
   own entrances, every other scene shares one table. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_TakaraShima1) {
        return gTakaraShimaEntrances1;
    }
    if (scene == (s32)&SceneId_TakaraShima2) {
        return gTakaraShimaEntrances2;
    }
    if (scene == (s32)&SceneId_TakaraShima3) {
        return gTakaraShimaEntrances3;
    }
    if (scene == (s32)&SceneId_TakaraShima4) {
        return gTakaraShimaEntrances4;
    }
    if (scene == (s32)&SceneId_TakaraShima5) {
        return gTakaraShimaEntrances5;
    }
    return gTakaraShimaEntrancesOther;
}

/* The island has no regions. */
const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

/* Every island scene leaves through one exit table. */
const u32 *Scene_GetExits(void)
{
    return gTakaraShimaExits;
}

/* The actors placed on the island: the first and third scenes have their
   own, the sixth to the fourteenth share one table. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_TakaraShima1) {
        return gTakaraShimaPlacements1;
    }
    if (scene == (s32)&SceneId_TakaraShima3) {
        return gTakaraShimaPlacements3;
    }
    if (scene <= (s32)&SceneId_TakaraShima14 && scene >= (s32)&SceneId_TakaraShima6) {
        return gTakaraShimaPlacements6To14;
    }
    return gTakaraShimaPlacementsOther;
}

void AdvanceEffectMotion(struct Effect *effect)
{

    s32 velocity_z;
    struct Sprite *sprite;
    s32 velocity_x;

    /* Macro-shaped block keeps the following Z load after the Y store. */
    /* FAKEMATCH: removing this one-pass block changes instruction scheduling. */
    do {
        velocity_x = effect->velocity[0];
        effect->position[0] += velocity_x;
        effect->position[1] += effect->velocity[1];
    } while (0);
    velocity_z = effect->velocity[2];
    effect->position[2] += velocity_z;

    effect->velocity[0] = velocity_x - Math_Divide(velocity_x, 18);
    effect->velocity[2] = velocity_z - velocity_z / 16;

    effect->accum18 += effect->rate30;
    effect->accum1c += effect->rate34;

    sprite = effect->sprite;
    sprite->angle += effect->step64;
}

void SpawnRadialEffectBurst(void)
{
    struct SceneObject *object;
    struct Vec vec;
    struct EffectParams params;
    u32 angle_step;
    s32 angle;
    s32 x;
    s32 z;

    object = (struct SceneObject *)Object_GetByIdFar();
    params.callback = (s32)AdvanceEffectMotion;
    for (angle_step = 0; angle_step <= 16; angle_step += 2) {
        angle = angle_step << 12;
        vec.x = Engine_MathCos(angle);
        vec.y = 0;
        z = Engine_MathSin(angle);
        x = vec.x;
        vec.z = z;
        x = x + Math_Divide(x, 3);
        vec.x = x;
        Effect_Spawn(object->x, object->y, object->z, x, vec.y, z, 0x01000001, &params);
    }
}

void InitializePrologueSceneState(void)
{
    extern u8 Data_02000240[];

    u8 *base;

    Engine_EventBegin();
    base = Data_02000240;
    Party_SetFields1ceAnd1d0(*(s16 *)(base + 448), 5);
    base[555] = 3;
    BattleFx_SetWeightedResult(84, 5);
    Engine_EventEnd();
}

/* What the island answers, chosen as its entrances are. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_TakaraShima1) {
        return gTakaraShimaEvents1;
    }
    if (scene == (s32)&SceneId_TakaraShima2) {
        return gTakaraShimaEvents2;
    }
    if (scene == (s32)&SceneId_TakaraShima3) {
        return gTakaraShimaEvents3;
    }
    if (scene == (s32)&SceneId_TakaraShima4) {
        return gTakaraShimaEvents4;
    }
    if (scene == (s32)&SceneId_TakaraShima5) {
        return gTakaraShimaEvents5;
    }
    return gTakaraShimaEventsOther;
}

void StartScriptedSceneMessage(s32 message_id)
{
    Battle_Reset(message_id);
    Engine_ActorSetPosition(8, 0, 0);
    Engine_GameFlagSet(4055);
    Engine_ItemShowFound(ITEM_NUT, 3);
    Engine_PartyGiveItem(ITEM_NUT, 0);
    Engine_EventEnd();
}

/* The island's scene start. The screen opens through a window. The escape
   scene places its actors; the fifth scene clears two rocks until flag
   0xef7 is set and opens its passage from entrance 5 or once flag 0x8d1
   is set; the first scene hides the four statues whose flags 0x240 to
   0x243 are set and sways the tree until flag 0xfd7; the sixth to the
   fourteenth run the shared column sequence. */
s32 Scene_Initialize(void)
{
    struct FieldActor *actor;
    s32 scene;
    s32 current;
    s32 first;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    scene = gGameState.scene;
    if (scene == (s32)&SceneId_TakaraShima3) {
        InitializeEscapeSceneActors();
        return 0;
    }
    if (scene == (s32)&SceneId_TakaraShima5) {
        if (Engine_GameFlagIsSet(0xef7) == 0) {
            Engine_MapCopyCellAttributes(0, 3, 1, 1, 13, 40);
            Engine_MapCopyCellAttributes(0, 2, 1, 1, 15, 40);
            Engine_MapObjectSetPosition(101, 0xd80000, 0x2880000);
        }
        if (gGameState.scene == scene) {
            if (gGameState.entrance != 5) {
                if (Engine_GameFlagIsSet(0x8d1) == 0) {
                    return 0;
                }
            }
            Engine_GameFlagSet(0x8d1);
            Engine_MapCopyCellAttributes(0, 1, 1, 1, 13, 30);
            Engine_MapObjectSetPosition(100, 0xd80000, 0x1e80000);
            return 0;
        }
    }
    current = gGameState.scene;
    if (current == (s32)&SceneId_TakaraShima1) {
        FieldScene_RunScene3b2_0200167c();
        *(s32 *)((u8 *)Object_GetById(8) + 56) = 0x810000;
        FieldScene_RedrawActorFootprint(9);
        FieldScene_RedrawActorFootprint(10);
        if (Engine_GameFlagIsSet(0x240) != 0) {
            actor = Object_GetById(11);
            if (actor != 0) {
                ((u8 *)actor)[89] = 0;
                Object_SetMode(actor, 4);
                Engine_ActorSetSpriteFlags(actor, 0);
            }
            Call4(SetMapCellCollision, 0, 0x1300000, 0x1700000, 253);
        }
        if (Engine_GameFlagIsSet(0x241) != 0) {
            actor = Object_GetById(12);
            if (actor != 0) {
                ((u8 *)actor)[89] = 0;
                Object_SetMode(actor, 4);
                Engine_ActorSetSpriteFlags(actor, 0);
            }
            Call4(SetMapCellCollision, 0, 0x500000, 0x1700000, 253);
        }
        if (Engine_GameFlagIsSet(0x242) != 0) {
            actor = Object_GetById(13);
            if (actor != 0) {
                ((u8 *)actor)[89] = 0;
                Object_SetMode(actor, 4);
                Engine_ActorSetSpriteFlags(actor, 0);
            }
            Call4(SetMapCellCollision, 0, 0x600000, 0x1500000, 253);
        }
        if (Engine_GameFlagIsSet(0x243) != 0) {
            actor = Object_GetById(14);
            if (actor != 0) {
                ((u8 *)actor)[89] = 0;
                Object_SetMode(actor, 4);
                Engine_ActorSetSpriteFlags(actor, 0);
            }
            Call4(SetMapCellCollision, 0, 0x900000, 0x1400000, 253);
            Call4(SetMapCellCollision, 0, 0x2f00000, 0x1400000, 253);
        }
        if (Engine_GameFlagIsSet(0xfd7) != 0) {
            return 0;
        }
        InitializeSwayingSceneObject(8);
        return 0;
    }
    first = (s32)&SceneId_TakaraShima6;
    if (current == first) {
        if (Engine_GameFlagIsSet(0xef4) == 0) {
            Engine_MapCopyCellAttributes(0, 0, 1, 1, 37, 10);
            Engine_MapObjectSetPosition(100, 0x2580000, 0xa80000);
        }
    }
    current = gGameState.scene;
    if (current >= first) {
        if (current <= (s32)&SceneId_TakaraShima14) {
            FieldScene_RunScene3b2SequenceA();
            if (gGameState.entrance == 5) {
                FieldScene_RunScene3b2_02001494();
            }
        }
    }
    return 0;
}

void FieldScene_RunScene3b2SequenceA(void)
{
    u32 i;
    s32 rec7;
    u8 *rec8;
    s32 record;

    rec8 = Object_GetById(8);
    rec7 = Engine_GameFlagIsSet((((union GameStateRows *)&gGameState)->halves[224][0] + (0x8d2 - (s32)&SceneId_TakaraShima6)));
    if (rec7 != 0) {
        Engine_ActorSetPosition(8, 0x28a0000, 0xa80000);
        *(volatile s32 *)((s32)rec8 + 12) = -0x200000;
        record = Object_GetById(8);
        Engine_ActorSetSpriteFlags(record, 0);
        Engine_ActorSetSpritePriority(8, 3);
        rec8[85] = 0;
        {
            u8 value = *(volatile u8 *)&rec8[35];

            rec8[35] = (u8)(value | 2);
        }
        Engine_MapCopyCellAttributes(42, 10, 1, 1, 40, 10);
    } else {
        *(u8 *)((s32)Object_GetByIdFar(8) + 85) = rec7;
    }
}

void FieldScene_HandleEscapeColumn(void)
{
    /* FAKEMATCH: the block-local two preserves the sprite-flag registers;
     * Japanese reuses a zero automatic across the waits and sprite reset.
     * Literal zero clears make its complete function 12 bytes shorter. */
    u8 *entity;
    s16 *slot;
    s32 column;
    s32 zero;

    entity = (u8 *)Object_GetById(8);
    column = *(s32 *)(entity + 8) >> 20;    /* 16.16 -> 16-pixel tile grid */
    if (column != 40) {
        return;
    }

#if EDITION_INTERNATIONAL
    {
        s32 off = 448;
        slot = (s16 *) ((u8 *) ((s16 *)&gGameState) + off);
    }
    if ((u8 *)Engine_GameFlagIsSet(*slot + (0x8d2 - (s32)&SceneId_TakaraShima6)) != 0) {
        return;                             /* handled by 0x02001214 instead */
    }
#endif

    entity[85] = 3;
#if !EDITION_INTERNATIONAL
    zero = 0;
#endif

    Engine_EventWait(8);
    ((s32 (*)())SpawnRadialEffectBurst)(8);
    Engine_AudioPlayCue(136);
    Engine_EventWait(40);

#if EDITION_INTERNATIONAL
    Engine_ActorSetSpriteFlags((u8 *)Object_GetById(8), 0);
#else
    Engine_ActorSetSpriteFlags((u8 *)Object_GetById(8), zero);
#endif
    ObjectMotion_SetActionVariant(8, 3);

#if EDITION_INTERNATIONAL
    entity[85] = 0;
#else
    entity[85] = zero;
#endif
    {
        s32 flags = 2;

        flags |= entity[35];
        entity[35] = flags;
    }

    Engine_MapCopyCellAttributes(42, 10, 1, 1, column, 10);

#if !EDITION_INTERNATIONAL
    {
        s32 off = 448;
        slot = (s16 *) ((u8 *) ((s16 *)&gGameState) + off);
    }
#endif
    Engine_GameFlagSet(*slot + (0x8d2 - (s32)&SceneId_TakaraShima6));
}

void RunPrologueSceneSetup(void)
{
    Engine_EventBegin();
    StagedActor_AdvancePair();
    Engine_EventEnd();
    FieldScene_HandleEscapeColumn();
}

void StartSceneScript37(void)
{
    Engine_MapAnimateCells(TakaraShima_EntranceCells, 37, 7);
    Engine_AudioPlayCue(183);
    Engine_EventRequestExit(4);
}

/* Crossbone Isle: on the first visit to an area (flag 0x8c8 + area) show its
 * title card; later visits animate the entrance cells and leave. */
void TakaraShima_RunAreaEntry(void)
{
    if (Engine_GameFlagIsSet(gGameState.scene + (0x8c8 - (s32)&SceneId_TakaraShima6)) == 0) {
        Engine_EventBegin();
        Party_SetFields1ceAnd1d0(gGameState.scene, 5);
        ((u8 *)((s16 (*)[1])&gGameState))[0x22b] = 3;
        switch (gGameState.scene - (s32)&SceneId_TakaraShima6) {
        case 0:
            BattleFx_SetWeightedResult(63, 0);
            break;
        case 1:
            BattleFx_SetWeightedResult(63, 1);
            break;
        case 2:
            BattleFx_SetWeightedResult(63, 2);
            break;
        case 3:
            BattleFx_SetWeightedResult(63, 3);
            break;
        case 4:
            BattleFx_SetWeightedResult(84, 0);
            break;
        case 5:
            BattleFx_SetWeightedResult(84, 1);
            break;
        case 6:
            BattleFx_SetWeightedResult(84, 2);
            break;
        case 7:
            BattleFx_SetWeightedResult(84, 3);
            break;
        case 8:
            BattleFx_SetWeightedResult(84, 4);
            break;
        }
        Engine_EventEnd();
    } else {
        Engine_MapAnimateCells((s32)TakaraShima_EntranceCells, 44, 7);
        Engine_AudioPlayCue(183);
        Engine_EventRequestExit(3);
    }
}

/*
 * Distance between two three-component 16.16 fixed-point positions.
 *
 * Each argument walks three consecutive 16.16 words in x, y, z order. The
 * per-axis deltas are taken in fixed point, shifted down to integers, squared,
 * and summed; the total is passed to the resident IWRAM integer square root.
 *
 * Expressions are preserved exactly as reconstructed: the walking-pointer form
 * is load-bearing for byte-identity and must not become struct field access.
 */

/* Advance actor 18 and its companions along the Z-axis escape route.
   Shared branches preserve the transition call sites used by multiple rows. */

/* Old-style declarations: overlay imports vary in arity between call sites.
   One import name per call site: bl displacements are per-site. */

/* This overlay's transition starter at 0x02001774, one name per site. */

/* Advance actor 9 and its companions along the Z-axis escape route.
   Shared branches preserve the transition call sites used by multiple rows. */

/* Return leg of the slot-14 transition beat: negated ids. */

/* This overlay's transition starter at 0x02001774. */

/* Outbound leg of the slot-14 transition beat. */

/* Return leg of the slot-16 transition beat: negated ids. */

/* Outbound leg of the slot-16 transition beat. */
void FieldScene_RunScene3b2_02001494(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_GameFlagSet((((union GameStateRows *)&gGameState)->halves[224][0] + (0x8c8 - (s32)&SceneId_TakaraShima6)));
    Engine_EventWait(30);
    Engine_MapAnimateCells(((const u16 *)TakaraShima_EntranceCells), 44, 7);
    Engine_ActorCenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
    Engine_EventRequestExit(3);
    Engine_EventEnd();
}

void ConfigureSceneActor11(s32 actor_id)
{
    s32 a = 0x1300000;
    s32 b = 0x1700000;
    u8 *p = Object_GetById(11);

    if (p != 0) {
        p[89] = 0;
    }
    Engine_ActorSetSpriteFlags(Object_GetById(actor_id), 0);
    SetMapCellCollision(0, a, b, 253);
    Engine_GameFlagSet(576);
}

void ConfigureSceneActor12(s32 actor_id)
{
    s32 a = 0x500000;
    s32 b = 0x1700000;
    u8 *p = Object_GetById(12);

    if (p != 0) {
        p[89] = 0;
    }
    Engine_ActorSetSpriteFlags(Object_GetById(actor_id), 0);
    SetMapCellCollision(0, a, b, 253);
    Engine_GameFlagSet(577);
}

void ConfigureSceneActor13(s32 actor_id)
{
    s32 a = 0x600000;
    s32 b = 0x1500000;
    u8 *p = Object_GetById(13);

    if (p != 0) {
        p[89] = 0;
    }
    Engine_ActorSetSpriteFlags(Object_GetById(actor_id), 0);
    SetMapCellCollision(0, a, b, 253);
    Engine_GameFlagSet(578);
}

void ConfigureSceneActor14(s32 actor_id)
{
    s32 a = 0x900000;
    s32 b = 0x1400000;
    s32 c = 0x2f00000;
    s32 d = 0x1400000;
    u8 *p = Object_GetById(14);

    if (p != 0) {
        p[89] = 0;
    }
    Engine_ActorSetSpriteFlags(Object_GetById(actor_id), 0);
    SetMapCellCollision(0, a, b, 253);
    SetMapCellCollision(0, c, d, 253);
    Engine_GameFlagSet(579);
}

void ShowForgetEverythingMessage(void)
{
    Engine_GameFlagSet(2244);
    {
        s32 k4 = 8, k5 = 21;

        Engine_MapCopyCellAttributes(0, 0, 1, 1, k4, k5);
    }
}

void ShowHelpYouForgetMessage(void)
{
    Engine_GameFlagSet(2245);
}

void ShowDamagedDoorMessage(void)
{
    Engine_GameFlagSet(2246);
}

void ShowSaveMyLifeMessage(void)
{
    Engine_GameFlagSet(2247);
}

void FieldScene_RunScene3b2_0200167c(void)
{
    s32 record;

    if (Engine_GameFlagIsSet(0x8c4) != 0) {
        Engine_MapCopyCellAttributes(0, 0, 1, 1, 8, 21);
        Engine_ActorSetPosition(15, 0x3c80000, 0x3c80000);
    } else {
        record = Object_GetById(15);
        *(s32 *)(record + 28) = 0x19999;
    }
    if (Engine_GameFlagIsSet(0x8c5) != 0) {
        Engine_ActorSetPosition(16, 0x3c80000, 0x3c80000);
    } else {
        record = Object_GetById(16);
        *(s32 *)(record + 28) = 0x19999;
    }
    if (Engine_GameFlagIsSet(0x8c6) != 0) {
        Engine_ActorSetPosition(17, 0x3c80000, 0x3c80000);
    } else {
        record = Object_GetById(17);
        *(s32 *)(record + 28) = 0x19999;
    }
    if (Engine_GameFlagIsSet(0x8c7) != 0) {
        Engine_ActorSetPosition(18, 0x3c80000, 0x3c80000);
    } else {
        record = Object_GetById(18);
        *(s32 *)(record + 28) = 0x19999;
    }
}

void RunSceneVectorTransition(void)
{
    struct V6 transition;

    Engine_EventBegin();
    if (StagedActor_FindClearPosition(&transition) != 0) {
        ((void (*)(struct V6))SceneActor_MoveAndRedraw)(transition);
    }
    Engine_EventEnd();
}

void PositionSceneActorPair(s32 actor_id, s32 x_offset, s32 z_offset)
{
    Obj *p;
    Obj *q;
    s32 x;
    s32 y;

    p = Object_GetById(gGameState.selected_actor);
    q = Object_GetById(actor_id);
    Engine_EventBegin();
    {
        x = ((p->f08 + (x_offset << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (z_offset << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Object_SetPosition(p, x, p->f0c, y);
    }
    Object_SetMode(p, 27);
    {
        x = ((q->f08 + (x_offset << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (z_offset << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        Object_SetPosition(q, x, q->f0c, y);
    }
    if (x_offset < 0 || z_offset < 0) {
        Object_SetMode(q, 4);
    } else {
        Object_SetMode(q, 3);
    }
    Engine_AudioPlayCue(226);
    Object_CommitPosition(p);
    Engine_AudioPlayCue(288);
    Engine_EventEnd();
}

void MarkGridLeftOfSceneActor(s32 actor_mode, s32 grid_value, s32 grid_attribute, s32 unused)
{
    struct S_020009dc *p = (struct S_020009dc *)Object_GetByIdFar(actor_mode);

    if (p != 0) {
        s32 v;

        Engine_ActorSetSpritePriority(actor_mode, 3);
        v = 2;
        v |= p->f23;
        p->f23 = (u8)v;
        {
            s32 k5 = p->f10 >> 20;
            s32 k4 = (p->f08 >> 20) - 1;

            Engine_MapCopyCellAttributes(grid_value, grid_attribute, 3, 1, k4, k5);
        }
    }
}

void MarkGridAboveSceneActor(s32 actor_mode, s32 grid_value, s32 grid_attribute, s32 unused)
{
    struct S_020009dc *p = (struct S_020009dc *)Object_GetByIdFar(actor_mode);

    if (p != 0) {
        s32 v;

        Engine_ActorSetSpritePriority(actor_mode, 3);
        v = 2;
        v |= p->f23;
        p->f23 = (u8)v;
        {
            s32 k4 = p->f08 >> 20;
            s32 k5 = (p->f10 >> 20) - 1;

            Engine_MapCopyCellAttributes(grid_value, grid_attribute, 1, 3, k4, k5);
        }
    }
}

void SetSceneActorModes(int actor_id)
{
    Engine_ActorSetAnimation(actor_id, 1);
    Engine_ActorSetAnimation(actor_id, 2);
}

void InitializeEscapeSceneActors(void)
{
    MarkGridLeftOfSceneActor(8, 0x11, 0x1E, 0x15);
    MarkGridLeftOfSceneActor(0xA, 0x11, 0x1F, 0x16);
    MarkGridAboveSceneActor(0xB, 0x14, 0x1E, 0x17);
    MarkGridAboveSceneActor(0xC, 0x15, 0x1E, 0x18);
    MarkGridAboveSceneActor(0xD, 0x16, 0x1E, 0x19);
    MarkGridAboveSceneActor(0xF, 0x17, 0x1E, 0x1A);
    MarkGridLeftOfSceneActor(0x11, 0, 0x1E, 0x1F);
    MarkGridLeftOfSceneActor(0x12, 0, 0x1F, 0x20);
    MarkGridLeftOfSceneActor(9, 0, 0x20, 0x21);
    MarkGridAboveSceneActor(0x13, 4, 0x1E, 0x22);
    MarkGridAboveSceneActor(0xE, 5, 0x1E, 0x23);
    MarkGridAboveSceneActor(0x10, 6, 0x1E, 0x24);
}

void ActivateSceneActor8(void)
{
    SetSceneActorModes(8);
}

void ActivateSceneActor10(void)
{
    SetSceneActorModes(10);
}

void ActivateSceneActor11(void)
{
    SetSceneActorModes(11);
}

void ActivateSceneActor12(void)
{
    SetSceneActorModes(12);
}

void ActivateSceneActor13(void)
{
    SetSceneActorModes(13);
}

void ActivateSceneActor15(void)
{
    SetSceneActorModes(15);
}

void ActivateSceneActor17(void)
{
    SetSceneActorModes(17);
}

void ActivateSceneActor18(void)
{
    SetSceneActorModes(18);
}

void ActivateSceneActor9(void)
{
    SetSceneActorModes(9);
}

void ActivateSceneActor19(void)
{
    SetSceneActorModes(19);
}

void ActivateSceneActor14(void)
{
    SetSceneActorModes(14);
}

void ActivateSceneActor16(void)
{
    SetSceneActorModes(16);
}

s32 TryPushBlockingSceneActor(struct S_02000474 *actor, struct V *requested)
{
    u8 *state = &actor->f55;
    s32 saved_state = *state;
#if EDITION_INTERNATIONAL
    struct V destination;

    destination.a = (actor->f08 & 0xfff00000) + 0x80000;
    destination.b = actor->f0c;
    destination.c = (actor->f10 & 0xfff00000) + 0x80000;
    {
        s32 direction = (actor->f06 + 0x2000) & 0xc000;

        Vector_AddPolarOffset(0x200000, direction, &destination);
    }
    if (Object_CheckMovementCollision(actor, &destination) == 0) {
#else
    if (Object_CheckMovementCollision(actor, requested) == 0) {
#endif
        s32 t;

        Engine_EventBegin();
        Object_SetMode(actor, 6);
        Engine_TaskWait(6);
        Engine_AudioPlayCue(152);
        Object_SetMode(actor, 7);
        actor->f30 = 0x30000;
        actor->f34 = 0x20000;
        actor->f28 = 0x40000;
        t = 126;
        t &= *state;
        *state = (u8)t;
        Engine_ActorSetSpriteFlags(actor, 0);
        {
#if EDITION_INTERNATIONAL
            s16 *coordinates = (s16 *)&destination;
#else
            s16 *coordinates = (s16 *)requested;
#endif

            Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, coordinates[1], coordinates[5]);
        }
        Object_SetMode(actor, 6);
        Engine_ActorSetSpriteFlags(actor, 1);
        *state = (u8)saved_state;
        Engine_EventEnd();
        return 1;
    }
    return 0;
}

s32 CheckActorPathSouth(void)
{
    struct S_02001b14 *actor = Object_GetById(ACTOR_PARTY_LEADER);
    struct V destination;

    destination.a = actor->f08;
    destination.b = actor->f0c;
    destination.c = actor->f10 + -0x200000;
    return TryPushBlockingSceneActor(actor, &destination);
}

s32 CheckActorPathNorth(void)
{
    struct S_02001b14 *actor = Object_GetById(ACTOR_PARTY_LEADER);
    struct V destination;

    destination.a = actor->f08;
    destination.b = actor->f0c;
    destination.c = actor->f10 + 0x200000;
    return TryPushBlockingSceneActor(actor, &destination);
}

s32 CheckActorPathWest(void)
{
    struct S_02001b14 *actor = Object_GetById(ACTOR_PARTY_LEADER);
    struct V destination;

    destination.a = actor->f08 + -0x200000;
    destination.b = actor->f0c;
    destination.c = actor->f10;
    return TryPushBlockingSceneActor(actor, &destination);
}

s32 CheckActorPathEast(void)
{
    struct S_02001b14 *actor = Object_GetById(ACTOR_PARTY_LEADER);
    struct V destination;

    destination.a = actor->f08 + 0x200000;
    destination.b = actor->f0c;
    destination.c = actor->f10;
    return TryPushBlockingSceneActor(actor, &destination);
}

void UpdateEscapeRouteForActorPositions(void)
{
    s32 actor_x = ((Obj *)Object_GetByIdFar(8))->f08 >> 20;
    s32 actor_z = ((Obj *)Object_GetByIdFar(8))->f10 >> 20;
    s32 actor_12_x = ((Obj *)Object_GetByIdFar(12))->f08 >> 20;
    s32 actor_15_x = ((Obj *)Object_GetByIdFar(15))->f08 >> 20;

    if (actor_z == 19) {
        if (actor_12_x == 24) {
            PositionSceneActorPair(8, 0, -80);
        } else if (actor_15_x == 24) {
            PositionSceneActorPair(8, 0, -112);
            PositionSceneActorPair(8, 0, -32);
        } else {
            PositionSceneActorPair(8, 0, -80);
            PositionSceneActorPair(8, 0, -112);
        }
    } else if (actor_z == 14) {
        if (actor_12_x == 24) {
            return;
        }
        if (actor_15_x == 24) {
            PositionSceneActorPair(8, 0, -64);
        } else {
            PositionSceneActorPair(8, 0, -112);
        }
    } else if (actor_z == 10) {
        if (actor_15_x == 24) {
            return;
        }
        PositionSceneActorPair(8, 0, -48);
    } else {
        CheckActorPathSouth();
        return;
    }
    Engine_TaskWait(2);
    {
        s32 route_end_z = ((Obj *)Object_GetByIdFar(8))->f10 >> 20;
        s32 route_x = actor_x - 1;

        Engine_MapCopyCellAttributes(route_x, actor_z, 3, 1, route_x, route_end_z);
    }
    Engine_MapCopyCellAttributes(0, 0, 3, 1, actor_x - 1, actor_z);
}

void UpdateActor8ReturnRoute(void)
{
    s32 x = ((Obj *)Object_GetByIdFar(8))->f08 >> 20;
    s32 y = ((Obj *)Object_GetByIdFar(8))->f10 >> 20;
    s32 z = ((Obj *)Object_GetByIdFar(12))->f08 >> 20;

    if (y == 7) {
        if (z == 24) {
            PositionSceneActorPair(8, 0, 48);
        } else {
            PositionSceneActorPair(8, 0, 80);
            PositionSceneActorPair(8, 0, 112);
        }
    } else if (y == 10) {
        if (z == 24) {
            return;
        }
        PositionSceneActorPair(8, 0, 144);
    } else if (y == 14) {
        PositionSceneActorPair(8, 0, 80);
    } else {
        return;
    }
    Engine_TaskWait(2);
    {
        s32 k = ((Obj *)Object_GetByIdFar(8))->f10 >> 20;
        s32 m = x - 1;

        Engine_MapCopyCellAttributes(m, y, 3, 1, m, k);
    }
    Engine_MapCopyCellAttributes(0, 0, 3, 1, x - 1, y);
}

void UpdateActor10RetreatRoute(void)
{
    s32 x = ((Obj *)Object_GetByIdFar(10))->f08 >> 20;
    s32 y = ((Obj *)Object_GetByIdFar(10))->f10 >> 20;
    s32 z = ((Obj *)Object_GetByIdFar(13))->f08 >> 20;
    s32 w = ((Obj *)Object_GetByIdFar(15))->f08 >> 20;

    if (y == 18) {
        if (w >= 31 && w <= 33) {
            PositionSceneActorPair(10, 0, -128);
        } else if (z >= 31 && z <= 33) {
            PositionSceneActorPair(10, 0, -128);
        } else {
            PositionSceneActorPair(10, 0, -112);
            PositionSceneActorPair(10, 0, -64);
        }
    } else if (y == 10) {
        if (w >= 31 && w <= 33) {
            return;
        }
        if (z >= 31 && z <= 33) {
            return;
        }
        PositionSceneActorPair(10, 0, -48);
    } else if (y == 7) {
        return;
    }
    Engine_TaskWait(2);
    {
        s32 k = ((Obj *)Object_GetByIdFar(10))->f10 >> 20;
        s32 m = x - 1;

        Engine_MapCopyCellAttributes(m, y, 3, 1, m, k);
    }
    Engine_MapCopyCellAttributes(0, 0, 3, 1, x - 1, y);
}

void UpdateActor10AdvanceRoute(void)
{
    s32 x = ((Obj *)Object_GetByIdFar(10))->f08 >> 20;
    s32 y = ((Obj *)Object_GetByIdFar(10))->f10 >> 20;

    if (y != 18) {
        if (y == 10) {
            PositionSceneActorPair(10, 0, 128);
        } else {
            PositionSceneActorPair(10, 0, 112);
            PositionSceneActorPair(10, 0, 64);
        }
        Engine_TaskWait(2);
        {
            s32 k = ((Obj *)Object_GetByIdFar(10))->f10 >> 20;
            s32 m = x - 1;

            Engine_MapCopyCellAttributes(m, y, 3, 1, m, k);
        }
        Engine_MapCopyCellAttributes(0, 0, 3, 1, x - 1, y);
    }
}

void UpdateActor11WestRoute(void)
{
    s32 x = ((Obj *)Object_GetByIdFar(11))->f08 >> 20;
    s32 y = ((Obj *)Object_GetByIdFar(11))->f10 >> 20;

    if (x != 30) {
        if (x == 34) {
            if ((((Obj *)Object_GetByIdFar(10))->f10 >> 20) == 18) {
                return;
            }
            PositionSceneActorPair(11, -64, 0);
        } else if (x == 36) {
            if ((((Obj *)Object_GetByIdFar(10))->f10 >> 20) == 18) {
                PositionSceneActorPair(11, -32, 0);
            } else {
                PositionSceneActorPair(11, -96, 0);
            }
        }
        Engine_TaskWait(2);
        {
            s32 k = ((Obj *)Object_GetByIdFar(11))->f08 >> 20;
            s32 m = y - 1;

            Engine_MapCopyCellAttributes(x, m, 1, 3, k, m);
        }
        Engine_MapCopyCellAttributes(0, 0, 1, 3, x, y - 1);
    }
}

void UpdateActor11EastRoute(void)
{
    s32 x = ((Obj *)Object_GetByIdFar(11))->f08 >> 20;
    s32 y = ((Obj *)Object_GetByIdFar(11))->f10 >> 20;

    if (x != 36) {
        if (x == 30) {
            if ((((Obj *)Object_GetByIdFar(10))->f10 >> 20) == 18) {
                return;
            }
            PositionSceneActorPair(11, 96, 0);
        } else if (x == 34) {
            PositionSceneActorPair(11, 32, 0);
        }
        Engine_TaskWait(2);
        {
            s32 k = ((Obj *)Object_GetByIdFar(11))->f08 >> 20;
            s32 m = y - 1;

            Engine_MapCopyCellAttributes(x, m, 1, 3, k, m);
        }
        Engine_MapCopyCellAttributes(0, 0, 1, 3, x, y - 1);
    }
}

void UpdateActor12WestRoute(void)
{
    s32 x = ((Obj *)Object_GetByIdFar(12))->f08 >> 20;
    s32 y = ((Obj *)Object_GetByIdFar(12))->f10 >> 20;

    if (x == 36) {
        PositionSceneActorPair(12, -96, 0);
        PositionSceneActorPair(12, -96, 0);
    } else if (x == 34) {
        PositionSceneActorPair(12, -96, 0);
        PositionSceneActorPair(12, -64, 0);
    } else if (x == 24) {
        return;
    }
    Engine_TaskWait(2);
    {
        s32 k = ((Obj *)Object_GetByIdFar(12))->f08 >> 20;
        s32 m = y - 1;

        Engine_MapCopyCellAttributes(x, m, 1, 3, k, m);
    }
    Engine_MapCopyCellAttributes(0, 0, 1, 3, x, y - 1);
}

void UpdateActor12EastRoute(void)
{
    s32 x = ((Obj *)Object_GetByIdFar(12))->f08 >> 20;
    s32 y = ((Obj *)Object_GetByIdFar(12))->f10 >> 20;

    if (x == 24) {
        PositionSceneActorPair(12, 96, 0);
        PositionSceneActorPair(12, 96, 0);
    } else if (x == 34) {
        PositionSceneActorPair(12, 32, 0);
    } else if (x == 36) {
        return;
    }
    Engine_TaskWait(2);
    {
        s32 k = ((Obj *)Object_GetByIdFar(12))->f08 >> 20;
        s32 m = y - 1;

        Engine_MapCopyCellAttributes(x, m, 1, 3, k, m);
    }
    Engine_MapCopyCellAttributes(0, 0, 1, 3, x, y - 1);
}

void UpdateActor13WestRoute(void)
{
    s32 x = ((Obj *)Object_GetByIdFar(13))->f08 >> 20;
    s32 y = ((Obj *)Object_GetByIdFar(13))->f10 >> 20;
    s32 z = ((Obj *)Object_GetByIdFar(10))->f10 >> 20;
    s32 w = ((Obj *)Object_GetByIdFar(15))->f08 >> 20;

    if (x == 36) {
        if (w == 34) {
            PositionSceneActorPair(13, -16, 0);
        } else if (z == 7) {
            PositionSceneActorPair(13, -32, 0);
        } else if (w == 30) {
            PositionSceneActorPair(13, -80, 0);
        } else {
            PositionSceneActorPair(13, -96, 0);
            PositionSceneActorPair(13, -80, 0);
        }
    } else if (x == 35) {
        if (w == 34) {
            return;
        } else if (z == 7) {
            PositionSceneActorPair(13, -16, 0);
        } else if (w == 30) {
            PositionSceneActorPair(13, -64, 0);
        } else {
            PositionSceneActorPair(13, -80, 0);
            PositionSceneActorPair(13, -80, 0);
        }
    } else if (x == 34) {
        if (z == 7) {
            return;
        }
        if (w == 30) {
            PositionSceneActorPair(13, -48, 0);
        } else {
            PositionSceneActorPair(13, -144, 0);
        }
    } else if (x == 31) {
        if (w == 30) {
            return;
        }
        PositionSceneActorPair(13, -96, 0);
    } else if (x == 25) {
        return;
    }
    Engine_TaskWait(2);
    {
        s32 k = ((Obj *)Object_GetByIdFar(13))->f08 >> 20;
        s32 m = y - 1;

        Engine_MapCopyCellAttributes(x, m, 1, 3, k, m);
    }
    Engine_MapCopyCellAttributes(0, 0, 1, 3, x, y - 1);
}

void UpdateActor13EastRoute(void)
{
    s32 x = ((Obj *)Object_GetByIdFar(13))->f08 >> 20;
    s32 y = ((Obj *)Object_GetByIdFar(13))->f10 >> 20;

    Object_GetById(15);
    if (x == 25) {
        PositionSceneActorPair(13, 96, 0);
        PositionSceneActorPair(13, 80, 0);
    } else if (x == 31) {
        PositionSceneActorPair(13, 80, 0);
    } else if (x == 34) {
        PositionSceneActorPair(13, 32, 0);
    } else if (x == 35) {
        PositionSceneActorPair(13, 16, 0);
    } else if (x == 36) {
        return;
    }
    Engine_TaskWait(2);
    {
        s32 k = ((Obj *)Object_GetByIdFar(13))->f08 >> 20;
        s32 m = y - 1;

        Engine_MapCopyCellAttributes(x, m, 1, 3, k, m);
    }
    Engine_MapCopyCellAttributes(0, 0, 1, 3, x, y - 1);
}

void UpdateActor15WestRoute(void)
{
    s32 x = ((Obj *)Object_GetByIdFar(15))->f08 >> 20;
    s32 y = ((Obj *)Object_GetByIdFar(15))->f10 >> 20;
    s32 z = ((Obj *)Object_GetByIdFar(8))->f10 >> 20;
    s32 w = ((Obj *)Object_GetByIdFar(10))->f10 >> 20;

    if (x == 35) {
        if (w == 7) {
            PositionSceneActorPair(15, -16, 0);
        } else if (z == 7) {
            PositionSceneActorPair(15, -112, 0);
        } else {
            PositionSceneActorPair(15, -96, 0);
            PositionSceneActorPair(15, -80, 0);
        }
    } else if (x == 34) {
        if (w == 7) {
            return;
        }
        PositionSceneActorPair(15, -96, 0);
        PositionSceneActorPair(15, -64, 0);
    } else if (x == 33) {
        PositionSceneActorPair(15, -144, 0);
    } else if (x == 31) {
        PositionSceneActorPair(15, -80, 0);
    } else if (x == 30) {
        PositionSceneActorPair(15, -96, 0);
    } else if (x == 24) {
        return;
    }
    Engine_TaskWait(2);
    {
        s32 k = ((Obj *)Object_GetByIdFar(15))->f08 >> 20;
        s32 m = y - 1;

        Engine_MapCopyCellAttributes(x, m, 1, 3, k, m);
    }
    Engine_MapCopyCellAttributes(0, 0, 1, 3, x, y - 1);
}

void UpdateActor15EastRoute(void)
{
    s32 x = ((Obj *)Object_GetByIdFar(15))->f08 >> 20;
    s32 y = ((Obj *)Object_GetByIdFar(15))->f10 >> 20;
    s32 z = ((Obj *)Object_GetByIdFar(10))->f10 >> 20;
    s32 w = ((Obj *)Object_GetByIdFar(13))->f08 >> 20;

    if (x == 24) {
        if (z == 7 || w == 31) {
            PositionSceneActorPair(15, 96, 0);
        } else if (w == 34) {
            PositionSceneActorPair(15, 64, 0);
            PositionSceneActorPair(15, 80, 0);
        } else if (w == 35) {
            PositionSceneActorPair(15, 80, 0);
            PositionSceneActorPair(15, 80, 0);
        } else {
            PositionSceneActorPair(15, 80, 0);
            PositionSceneActorPair(15, 96, 0);
        }
    } else if (x == 30 || w == 31) {
        if (z == 7) {
            return;
        }
        if (w == 34) {
            PositionSceneActorPair(15, 48, 0);
        } else if (w == 35) {
            PositionSceneActorPair(15, 64, 0);
        } else {
            PositionSceneActorPair(15, 80, 0);
        }
    } else if (x == 33) {
        if (w == 34) {
            return;
        }
        if (w == 35) {
            PositionSceneActorPair(15, 16, 0);
        } else {
            PositionSceneActorPair(15, 32, 0);
        }
    } else if (x == 34) {
        PositionSceneActorPair(15, 16, 0);
    } else if (x == 35) {
        return;
    }
    Engine_TaskWait(2);
    {
        s32 k = ((Obj *)Object_GetByIdFar(15))->f08 >> 20;
        s32 m = y - 1;

        Engine_MapCopyCellAttributes(x, m, 1, 3, k, m);
    }
    Engine_MapCopyCellAttributes(0, 0, 1, 3, x, y - 1);
}

void UpdateActor17SouthRoute(void)
{
    s32 x = ((Obj *)Object_GetByIdFar(17))->f08 >> 20;
    s32 y = ((Obj *)Object_GetByIdFar(17))->f10 >> 20;
    s32 z = ((Obj *)Object_GetByIdFar(19))->f08 >> 20;

    if (y == 19) {
        if (z >= 3 && z <= 5) {
            PositionSceneActorPair(17, 0, -16);
        } else {
            PositionSceneActorPair(17, 0, -64);
        }
    } else if (y == 18) {
        if (z >= 3 && z <= 5) {
            return;
        }
        PositionSceneActorPair(17, 0, -48);
    } else if (y == 15) {
        return;
    }
    Engine_TaskWait(2);
    {
        s32 k = ((Obj *)Object_GetByIdFar(17))->f10 >> 20;
        s32 m = x - 1;

        Engine_MapCopyCellAttributes(m, y, 3, 1, m, k);
    }
    Engine_MapCopyCellAttributes(0, 0, 3, 1, x - 1, y);
}

void UpdateActor17NorthRoute(void)
{
    s32 x = ((Obj *)Object_GetByIdFar(17))->f08 >> 20;
    s32 y = ((Obj *)Object_GetByIdFar(17))->f10 >> 20;

    if (y == 15) {
        PositionSceneActorPair(17, 0, 64);
    } else if (y == 18) {
        PositionSceneActorPair(17, 0, 16);
    } else if (y == 19) {
        return;
    }
    Engine_TaskWait(2);
    {
        s32 k = ((Obj *)Object_GetByIdFar(17))->f10 >> 20;

        s32 m = x - 1;

        Engine_MapCopyCellAttributes(m, y, 3, 1, m, k);
    }
    Engine_MapCopyCellAttributes(0, 0, 3, 1, x - 1, y);
}

#define IN_COLUMNS(x) ((x) >= 6 && (x) <= 8)

/* Crossbone Isle push block, northward: by the row actor 18 stopped in and whether actors 19, 14 and 16 stand in columns 6 to 8, slide its pair back to the matching offset, then move the block's cell attributes. */
void TakaraShima_SettlePushedBlockNorth(void)
{
    s32 cell_x;
    s32 cell_z;
    s32 x19;
    s32 x14;
    s32 x16;

    cell_x = Object_GetById(18)->x.fixed >> 20;
    cell_z = Object_GetById(18)->z.fixed >> 20;
    x19 = Object_GetById(19)->x.fixed >> 20;
    x14 = Object_GetById(14)->x.fixed >> 20;
    x16 = Object_GetById(16)->x.fixed >> 20;
    if (cell_z == 19) {
        if (IN_COLUMNS(x19)) {
            PositionSceneActorPair(18, 0, -16);
        } else if (IN_COLUMNS(x14)) {
            PositionSceneActorPair(18, 0, -64);
        } else if (IN_COLUMNS(x16)) {
            PositionSceneActorPair(18, 0, -112);
        } else {
            PositionSceneActorPair(18, 0, -64);
            PositionSceneActorPair(18, 0, -96);
        }
    } else if (cell_z == 18) {
        if (IN_COLUMNS(x19)) {
            return;
        }
        if (IN_COLUMNS(x14)) {
            PositionSceneActorPair(18, 0, -48);
        } else if (IN_COLUMNS(x16)) {
            PositionSceneActorPair(18, 0, -96);
        } else {
            PositionSceneActorPair(18, 0, -144);
        }
    } else if (cell_z == 15) {
        if (IN_COLUMNS(x14)) {
            return;
        }
        if (IN_COLUMNS(x16)) {
            PositionSceneActorPair(18, 0, -48);
        } else {
            PositionSceneActorPair(18, 0, -96);
        }
    } else if (cell_z == 14) {
        if (IN_COLUMNS(x14)) {
            return;
        }
        if (IN_COLUMNS(x16)) {
            PositionSceneActorPair(18, 0, -32);
        } else {
            PositionSceneActorPair(18, 0, -80);
        }
    } else if (cell_z == 12) {
        if (IN_COLUMNS(x16)) {
            return;
        }
        PositionSceneActorPair(18, 0, -48);
    } else if (cell_z == 11) {
        if (IN_COLUMNS(x16)) {
            return;
        }
        PositionSceneActorPair(18, 0, -32);
    } else if (cell_z == 9) {
        return;
    }
    Engine_TaskWait(2);
    Engine_MapCopyCellAttributes(cell_x - 1, cell_z, 3, 1, cell_x - 1, Object_GetById(18)->z.fixed >> 20);
    Engine_MapCopyCellAttributes(0, 0, 3, 1, cell_x - 1, cell_z);
}

#undef IN_COLUMNS

void AdvanceActor18AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion19Column;
    s32 companion14Column;

    s32 permuted_5;
    permuted_5 = *(s32 *)(Object_GetByIdFar(18) + 8) >> 20;
    row = *(s32 *)(Object_GetByIdFar(18) + 16) >> 20;
    column  = permuted_5;
    companion19Column = *(s32 *)(Object_GetByIdFar(19) + 8) >> 20;
    companion14Column = *(s32 *)(Object_GetByIdFar(14) + 8) >> 20;

    if (row == 9) {
        if ((u32)(companion14Column - 6) <= 2) {
            goto transition32;
        }
        if ((u32)(companion19Column - 6) <= 2) {
            goto transition80;
        }
        /* This arm runs two transitions back to back. */
        PositionSceneActorPair(18, 0, 64);
        PositionSceneActorPair(18, 0, 96);
    } else if (row == 11) {
        if ((u32)(companion14Column - 6) <= 2) {
            return;
        }
        if ((u32)(companion19Column - 6) <= 2) {
            PositionSceneActorPair(18, 0, 48);
        } else {
            PositionSceneActorPair(18, 0, 128);
        }
    } else if (row == 12) {
        if ((u32)(companion19Column - 6) <= 2) {
transition32:
            PositionSceneActorPair(18, 0, 32);
        } else {
            PositionSceneActorPair(18, 0, 112);
        }
    } else if (row == 14) {
        if ((u32)(companion19Column - 6) <= 2) {
            return;
        }
transition80:
        PositionSceneActorPair(18, 0, 80);
    } else if (row == 15) {
        PositionSceneActorPair(18, 0, 64);
    } else if (row == 18) {
        PositionSceneActorPair(18, 0, 16);
    } else if (row == 19) {
        return;
    }

    Engine_TaskWait(2);

    column -= 1;
    Engine_MapCopyCellAttributes(column, row, 3, 1,
                  column, *(s32 *)(Object_GetByIdFar(18) + 16) >> 20);
    Engine_MapCopyCellAttributes(0, 0, 3, 1, column, row);
}

#define IN_COLUMNS(x) ((x) >= 9 && (x) <= 11)

/* Crossbone Isle push block (actor 9), northward: by the row it stopped in and whether actors 19, 14 and 16 stand in columns 9 to 11, slide its pair back to the matching offset, then move the block's cell attributes. */
void TakaraShima_SettleSecondBlockNorth(void)
{
    s32 cell_x;
    s32 cell_z;
    s32 x19;
    s32 x14;
    s32 x16;

    cell_x = Object_GetById(9)->x.fixed >> 20;
    cell_z = Object_GetById(9)->z.fixed >> 20;
    x19 = Object_GetById(19)->x.fixed >> 20;
    x14 = Object_GetById(14)->x.fixed >> 20;
    x16 = Object_GetById(16)->x.fixed >> 20;
    if (cell_z == 19) {
        if (IN_COLUMNS(x19)) {
            PositionSceneActorPair(9, 0, -16);
        } else if (IN_COLUMNS(x14)) {
            PositionSceneActorPair(9, 0, -64);
        } else if (IN_COLUMNS(x16)) {
            PositionSceneActorPair(9, 0, -112);
        } else {
            PositionSceneActorPair(9, 0, -80);
            PositionSceneActorPair(9, 0, -96);
        }
    } else if (cell_z == 18) {
        if (IN_COLUMNS(x19)) {
            return;
        }
        if (IN_COLUMNS(x14)) {
            PositionSceneActorPair(9, 0, -48);
        } else if (IN_COLUMNS(x16)) {
            PositionSceneActorPair(9, 0, -96);
        } else {
            PositionSceneActorPair(9, 0, -96);
            PositionSceneActorPair(9, 0, -64);
        }
    } else if (cell_z == 15) {
        if (IN_COLUMNS(x14)) {
            return;
        }
        if (IN_COLUMNS(x16)) {
            PositionSceneActorPair(9, 0, -48);
        } else {
            PositionSceneActorPair(9, 0, -112);
        }
    } else if (cell_z == 14) {
        if (IN_COLUMNS(x14)) {
            return;
        }
        if (IN_COLUMNS(x16)) {
            PositionSceneActorPair(9, 0, -32);
        } else {
            PositionSceneActorPair(9, 0, -96);
        }
    } else if (cell_z == 12) {
        if (IN_COLUMNS(x16)) {
            return;
        }
        PositionSceneActorPair(9, 0, -64);
    } else if (cell_z == 11) {
        if (IN_COLUMNS(x16)) {
            return;
        }
        PositionSceneActorPair(9, 0, -48);
    } else if ((u32)cell_z <= 9) {
        return;
    }
    Engine_TaskWait(2);
    Engine_MapCopyCellAttributes(cell_x - 1, cell_z, 3, 1, cell_x - 1, Object_GetById(9)->z.fixed >> 20);
    Engine_MapCopyCellAttributes(0, 0, 3, 1, cell_x - 1, cell_z);
}

#undef IN_COLUMNS

void AdvanceActor9AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion19Column;
    s32 companion14Column;
    s32 companion16Column;

    s32 permuted_6;
    permuted_6 = *(s32 *)(Object_GetByIdFar(9) + 8) >> 20;
    row = *(s32 *)(Object_GetByIdFar(9) + 16) >> 20;
    column  = permuted_6;
    companion19Column = *(s32 *)(Object_GetByIdFar(19) + 8) >> 20;
    companion14Column = *(s32 *)(Object_GetByIdFar(14) + 8) >> 20;
    companion16Column = *(s32 *)(Object_GetByIdFar(16) + 8) >> 20;

    if (row == 8) {
        if ((u32)(companion16Column - 9) <= 2) {
            return;
        }
        if ((u32)(companion14Column - 9) <= 2) {
            goto transition48;
        }
        if ((u32)(companion19Column - 9) > 2) {
            PositionSceneActorPair(9, 0, 80);
        }
        /* Falls through into the id-96 site from both paths. */
        PositionSceneActorPair(9, 0, 96);
    } else if (row == 11) {
        if ((u32)(companion14Column - 9) <= 2) {
            return;
        }
        if ((u32)(companion19Column - 9) <= 2) {
transition48:
            PositionSceneActorPair(9, 0, 48);
        } else {
            PositionSceneActorPair(9, 0, 128);
        }
    } else if (row == 12) {
        if ((u32)(companion14Column - 9) <= 2) {
            return;
        }
        if ((u32)(companion19Column - 9) <= 2) {
            PositionSceneActorPair(9, 0, 32);
        } else {
            PositionSceneActorPair(9, 0, 112);
        }
    } else if (row == 14) {
        if ((u32)(companion19Column - 9) <= 2) {
            return;
        }
        PositionSceneActorPair(9, 0, 80);
    } else if (row == 15) {
        PositionSceneActorPair(9, 0, 64);
    } else if (row == 18) {
        PositionSceneActorPair(9, 0, 16);
    }

    Engine_TaskWait(2);

    column -= 1;
    Engine_MapCopyCellAttributes(column, row, 3, 1,
                  column, *(s32 *)(Object_GetByIdFar(9) + 16) >> 20);
    Engine_MapCopyCellAttributes(0, 0, 3, 1, column, row);
}

/* Crossbone Isle push block, westward: by the column actor 19 stopped in and where actors 17, 18 and 9 stand, slide its pair back to the matching offset, then move the block's cell attributes. */
void TakaraShima_SettlePushedBlockWest(void)
{
    s32 cell_x;
    s32 cell_z;
    s32 z17;
    s32 z18;
    s32 z9;

    cell_x = Object_GetById(19)->x.fixed >> 20;
    cell_z = Object_GetById(19)->z.fixed >> 20;
    z17 = Object_GetById(17)->z.fixed >> 20;
    z18 = Object_GetById(18)->z.fixed >> 20;
    z9 = Object_GetById(9)->z.fixed >> 20;
    if (cell_x == 3) {
        return;
    }
    if (cell_x == 13) {
        if (z9 == 15) {
            PositionSceneActorPair(19, -16, 0);
        } else if (z18 == 15) {
            PositionSceneActorPair(19, -64, 0);
        } else if (z17 == 15) {
            PositionSceneActorPair(19, -112, 0);
        } else {
            PositionSceneActorPair(19, -112, 0);
            PositionSceneActorPair(19, -48, 0);
        }
    } else if (cell_x == 6) {
        if (z17 == 15) {
            return;
        }
        PositionSceneActorPair(19, -48, 0);
    } else if (cell_x == 5) {
        PositionSceneActorPair(19, -32, 0);
    } else if (cell_x == 8) {
        if (z18 == 15) {
            return;
        }
        if (z17 == 15) {
            PositionSceneActorPair(19, -32, 0);
        } else {
            PositionSceneActorPair(19, -80, 0);
        }
    } else if (cell_x == 9) {
        if (z18 == 15) {
            return;
        }
        if (z17 == 15) {
            PositionSceneActorPair(19, -48, 0);
        } else {
            PositionSceneActorPair(19, -96, 0);
        }
    } else if (cell_x == 12) {
        if (z9 == 15) {
            return;
        }
        if (z18 == 15) {
            PositionSceneActorPair(19, -48, 0);
        } else if (z17 == 15) {
            PositionSceneActorPair(19, -96, 0);
        } else {
            PositionSceneActorPair(19, -144, 0);
        }
    }
    Engine_TaskWait(2);
    Engine_MapCopyCellAttributes(cell_x, cell_z - 1, 1, 3, Object_GetById(19)->x.fixed >> 20, cell_z - 1);
    Engine_MapCopyCellAttributes(0, 0, 1, 3, cell_x, cell_z - 1);
}

/* Crossbone Isle push block: by the column actor 19 stopped in and where actors 18 and 9 stand, slide its pair to the matching offset, then move the block's cell attributes to its new column. */
void TakaraShima_SettlePushedBlock(void)
{
    s32 cell_x;
    s32 cell_z;
    s32 z18;
    s32 z9;

    cell_x = Object_GetById(19)->x.fixed >> 20;
    cell_z = Object_GetById(19)->z.fixed >> 20;
    z18 = Object_GetById(18)->z.fixed >> 20;
    z9 = Object_GetById(9)->z.fixed >> 20;
    if (cell_x == 3) {
        if (z18 == 15) {
            PositionSceneActorPair(19, 32, 0);
        } else if (z9 == 15) {
            PositionSceneActorPair(19, 80, 0);
        } else {
            PositionSceneActorPair(19, 112, 0);
            PositionSceneActorPair(19, 48, 0);
        }
    } else if (cell_x == 5) {
        if (z18 == 15) {
            return;
        }
        if (z9 == 15) {
            PositionSceneActorPair(19, 48, 0);
        } else {
            PositionSceneActorPair(19, 128, 0);
        }
    } else if (cell_x == 6) {
        if (z9 == 15) {
            PositionSceneActorPair(19, 32, 0);
        } else {
            PositionSceneActorPair(19, 112, 0);
        }
    } else if (cell_x == 8) {
        if (z9 == 15) {
            return;
        }
        PositionSceneActorPair(19, 80, 0);
    } else if (cell_x == 9) {
        PositionSceneActorPair(19, 64, 0);
    } else if (cell_x == 12) {
        PositionSceneActorPair(19, 16, 0);
    }
    Engine_TaskWait(2);
    Engine_MapCopyCellAttributes(cell_x, cell_z - 1, 1, 3, Object_GetById(19)->x.fixed >> 20, cell_z - 1);
    Engine_MapCopyCellAttributes(0, 0, 1, 3, cell_x, cell_z - 1);
}

void RetreatActor14AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion18Row;
    s32 companion9Row;
    s32 rowM1;

    s32 permuted_7;
    column = *(s32 *)(Object_GetByIdFar(14) + 8) >> 20;
    permuted_7 = *(s32 *)(Object_GetByIdFar(14) + 16) >> 20;
    companion18Row = *(s32 *)(Object_GetByIdFar(18) + 16) >> 20;
    row  = permuted_7;
    companion9Row = *(s32 *)(Object_GetByIdFar(9) + 16) >> 20;

    if (column == 13) {
        if ((u32)(companion9Row - 12) <= 2) {
            PositionSceneActorPair(14, -16, 0);
        } else if ((u32)(companion18Row - 12) <= 2) {
            PositionSceneActorPair(14, -64, 0);
        } else {
            PositionSceneActorPair(14, -112, 0);
        }
    } else if (column == 12) {
        if ((u32)(companion9Row - 12) <= 2) {
            return;
        }
        if ((u32)(companion18Row - 12) <= 2) {
            PositionSceneActorPair(14, -48, 0);
        } else {
            PositionSceneActorPair(14, -96, 0);
        }
    } else if (column == 9) {
        if ((u32)(companion18Row - 12) <= 2) {
            return;
        }
        PositionSceneActorPair(14, -48, 0);
    } else if (column == 8) {
        if ((u32)(companion18Row - 12) <= 2) {
            return;
        }
        PositionSceneActorPair(14, -32, 0);
    } else if (column == 6) {
        return;
    }

    Engine_TaskWait(2);

    rowM1 = row - 1;
    Engine_MapCopyCellAttributes(column, rowM1, 1, 3,
                  *(s32 *)(Object_GetByIdFar(14) + 8) >> 20, rowM1);
    Engine_MapCopyCellAttributes(0, 0, 1, 3, column, rowM1);
}

void AdvanceActor14AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion18Row;
    s32 companion9Row;
    s32 rowM1;

    s32 permuted_7;
    column = *(s32 *)(Object_GetByIdFar(14) + 8) >> 20;
    permuted_7 = *(s32 *)(Object_GetByIdFar(14) + 16) >> 20;
    companion18Row = *(s32 *)(Object_GetByIdFar(18) + 16) >> 20;
    row  = permuted_7;
    companion9Row = *(s32 *)(Object_GetByIdFar(9) + 16) >> 20;

    if (column == 6) {
        if ((u32)(companion9Row - 12) <= 2) {
            PositionSceneActorPair(14, 32, 0);
        } else if ((u32)(companion18Row - 12) <= 2) {
            PositionSceneActorPair(14, 64, 0);
        } else {
            PositionSceneActorPair(14, 112, 0);
        }
    } else if (column == 8) {
        if ((u32)(companion9Row - 12) <= 2) {
            return;
        }
        PositionSceneActorPair(14, 80, 0);
    } else if (column == 9) {
        if ((u32)(companion9Row - 12) <= 2) {
            return;
        }
        PositionSceneActorPair(14, 64, 0);
    } else if (column == 12) {
        PositionSceneActorPair(14, 16, 0);
    } else if (column == 13) {
        return;
    }

    Engine_TaskWait(2);

    rowM1 = row - 1;
    Engine_MapCopyCellAttributes(column, rowM1, 1, 3,
                  *(s32 *)(Object_GetByIdFar(14) + 8) >> 20, rowM1);
    Engine_MapCopyCellAttributes(0, 0, 1, 3, column, rowM1);
}

void RetreatActor16AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companion18Row;
    s32 companion9Row;
    s32 rowM1;

    s32 permuted_7;
    column = *(s32 *)(Object_GetByIdFar(16) + 8) >> 20;
    permuted_7 = *(s32 *)(Object_GetByIdFar(16) + 16) >> 20;
    companion18Row = *(s32 *)(Object_GetByIdFar(18) + 16) >> 20;
    row  = permuted_7;
    companion9Row = *(s32 *)(Object_GetByIdFar(9) + 16) >> 20;

    if (column == 13) {
        if ((u32)(companion9Row - 9) <= 2) {
            PositionSceneActorPair(16, -16, 0);
        } else if ((u32)(companion18Row - 9) <= 2) {
            PositionSceneActorPair(16, -64, 0);
        } else {
            PositionSceneActorPair(16, -112, 0);
        }
    } else if (column == 12) {
        if ((u32)(companion9Row - 9) <= 2) {
            return;
        }
        if ((u32)(companion18Row - 9) <= 2) {
            PositionSceneActorPair(16, -48, 0);
        } else {
            PositionSceneActorPair(16, -96, 0);
        }
    } else if (column == 9) {
        if ((u32)(companion18Row - 9) <= 2) {
            return;
        }
        PositionSceneActorPair(16, -48, 0);
    } else if (column == 8) {
        PositionSceneActorPair(16, -32, 0);
    } else if (column == 6) {
        return;
    }

    Engine_TaskWait(2);

    rowM1 = row - 1;
    Engine_MapCopyCellAttributes(column, rowM1, 1, 3,
                  *(s32 *)(Object_GetByIdFar(16) + 8) >> 20, rowM1);
    Engine_MapCopyCellAttributes(0, 0, 1, 3, column, rowM1);
}

void AdvanceActor16AlongEscapeRoute(void)
{
    s32 column;
    s32 row;
    s32 companionRow;

    s32 permuted_6;
    column = *(s32 *)(Object_GetByIdFar(16) + 8) >> 20;
    permuted_6 = *(s32 *)(Object_GetByIdFar(16) + 16) >> 20;
    companionRow = *(s32 *)(Object_GetByIdFar(9) + 16) >> 20;
    row  = permuted_6;

    if (column == 6) {
        if ((u32)(companionRow - 9) <= 2) {
            PositionSceneActorPair(16, 32, 0);
        } else {
            PositionSceneActorPair(16, 112, 0);
        }
    } else if (column == 8) {
        if ((u32)(companionRow - 9) <= 2) {
            return;
        }
        PositionSceneActorPair(16, 80, 0);
    } else if (column == 9) {
        PositionSceneActorPair(16, 64, 0);
    } else if (column == 12) {
        PositionSceneActorPair(16, 16, 0);
    } else if (column == 13) {
        return;
    }

    Engine_TaskWait(2);

    row -= 1;
    Engine_MapCopyCellAttributes(column, row, 1, 3,
                  *(s32 *)(Object_GetByIdFar(16) + 8) >> 20, row);
    Engine_MapCopyCellAttributes(0, 0, 1, 3, column, row);
}

s32 UpdateSwayingSceneObject(struct S *object)
{

    struct T *sprite = object->f50;
    s32 vertical_offset = Engine_MathSin(object->f30) * 2;
    s32 random_b;
    s32 random_a;

    if (vertical_offset > 0) {
        vertical_offset = -vertical_offset;
    }
    object->f08 = object->f38 + Engine_MathCos(object->f30) * 2;
    object->f0c = object->f3c + vertical_offset;
    sprite->f1e = (u16)(Engine_MathCos(object->f30 + 0x8000) / 8);
    random_a = Engine_RandomNext();
    random_b = Engine_RandomNext();
    object->f30 += (((u32)(random_a << 9)) >> 16) + (((u32)(random_b << 9)) >> 16) + 0x400;
    return 0;
}

void InitializeSwayingSceneObject(void)
{
    OrbitingSceneObject *actor;
    OrbitingSceneObjectSprite *sprite;
    u8 *transfer;
    s32 zero;

    actor = GetOrbitingSceneObject();
    sprite = actor->sprite;
    sprite->flags_09_mode = 1;
    sprite->flags_05_bit_5 = 0;
    sprite->flags_09_high = 0;

    zero = 0;
    sprite->state = zero;
    Engine_ActorSetSpriteFlags(actor, zero);
    actor->active = zero;
    actor->mode = zero;

    if (Engine_GameFlagIsSet(0x109) == 0)
        actor->y += 0x200000;

    actor->flags_23 &= 0xfe;
    actor->visible = 1;

    transfer = AllocateEffectTransfer(17, 0x608);
    Engine_ItemLoadIcon(ITEM_NUT);
    transfer += 0x400;
    Engine_VramLoad(sprite->palette, 128, transfer);
    Engine_HeapRelease(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)UpdateSwayingSceneObject;
    actor->state = zero;
}

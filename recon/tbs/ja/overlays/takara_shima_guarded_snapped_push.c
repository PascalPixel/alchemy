/* Draft: Crossbone Isle escape column and push destination.
 * 2026-10-01: International checks the saved escape flag before running
 * the sequence and computes a snapped forward push point; Japanese runs
 * the column sequence directly and checks the caller-provided destination.
 * Complete callbacks shrink28+60 bytes. Ordinary approved TBS flags.
 */
#include "TYPES.H"
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
void FieldScene_HandleEscapeColumn(void)
{
    u8 *entity;
    s16 *slot;
    s32 column;

    entity = (u8 *)Object_GetById(8);
    column = *(s32 *)(entity + 8) >> 20;    /* 16.16 -> 16-pixel tile grid */
    if (column != 40) {
        return;
    }

    {
        s32 off = 448;
        slot = (s16 *) ((u8 *) ((s16 *)&gGameState) + off);
    }
    if ((u8 *)Engine_GameFlagIsSet(*slot + (0x8d2 - (s32)&SceneId_TakaraShima6)) != 0) {
        return;                             /* handled by 0x02001214 instead */
    }

    entity[85] = 3;

    Engine_EventWait(8);
    ((s32 (*)())SpawnRadialEffectBurst)(8);
    Engine_AudioPlayCue(136);
    Engine_EventWait(40);

    Engine_ActorSetSpriteFlags((u8 *)Object_GetById(8), 0);
    ObjectMotion_SetActionVariant(8, 3);

    entity[85] = 0;
    /* FAKEMATCH: a temporary holding the 2 picks the reference registers. */
    {
        s32 flags = 2;

        flags |= entity[35];
        entity[35] = flags;
    }

    Engine_MapCopyCellAttributes(42, 10, 1, 1, column, 10);

    Engine_GameFlagSet(*slot + (0x8d2 - (s32)&SceneId_TakaraShima6));
}

s32 TryPushBlockingSceneActor(struct S_02000474 *actor, struct V *requested)
{
    u8 *state = &actor->f55;
    s32 saved_state = *state;
    struct V destination;

    destination.a = (actor->f08 & 0xfff00000) + 0x80000;
    destination.b = actor->f0c;
    destination.c = (actor->f10 & 0xfff00000) + 0x80000;
    {
        s32 direction = (actor->f06 + 0x2000) & 0xc000;

        Vector_AddPolarOffset(0x200000, direction, &destination);
    }
    if (Object_CheckMovementCollision(actor, &destination) == 0) {
        s32 t;

        Engine_EventBegin();
        Object_SetMode(actor, 6);
        Engine_TaskWait(6);
        Audio_PlayCue(152);
        Object_SetMode(actor, 7);
        actor->f30 = 0x30000;
        actor->f34 = 0x20000;
        actor->f28 = 0x40000;
        t = 126;
        t &= *state;
        *state = (u8)t;
        Engine_ActorSetSpriteFlags(actor, 0);
        {
            s16 *coordinates = (s16 *)&destination;

            Actor_MoveToAndWait(ACTOR_PARTY_LEADER, coordinates[1], coordinates[5]);
        }
        Object_SetMode(actor, 6);
        Engine_ActorSetSpriteFlags(actor, 1);
        *state = (u8)saved_state;
        Engine_EventEnd();
        return 1;
    }
    return 0;
}

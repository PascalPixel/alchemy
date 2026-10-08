#ifndef BABI_H
#define BABI_H

#include "TYPES.H"
#include "text/MSG_IDS.H"
#define FIELD_STAGED_ACTOR_IMPORTS
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "FIELD_EFFECT.H"

#include "STAGED_ACTOR.H"
#include "SPAWN_CONFIGURED_EFFECT.H"


struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

struct EffectParams {
    s32 unk00;
    s32 mode;
    u8 filler08[0x1c];
    s32 callback;
};

struct Slot02000f10 {
    u8 head[8];
    s32 x;
    s32 y;
    s32 rank;
};

typedef struct Actor_02000cd0 {
    u8 pad0[6];
    u16 tag;
    s32 x;
    s32 y;
    s32 z;
    u8 pad20[20];
    s32 speedZ;
    s32 pad44;
    s32 speedX;
    s32 speedY;
    u8 pad56[29];
    u8 flags;
} Actor_02000cd0;

extern const s32 BabiChika_FlickerScript[];

typedef struct Target_02000cd0 {
    union FieldCoordinate x;
    s32 pad4;
    union FieldCoordinate z;
} Target_02000cd0;

typedef struct Record {
    s32 pad0;
    s32 pad4;
    s32 w8;
    s32 w12;
    s32 w16;
} Record;

struct Actor_02000ec8 { u8 unknown_00[8]; s32 x; s32 y; s32 z; u8 unknown_14[0xf]; u8 flatla3; };

struct Track02001038 {
    u8 head[12];
    s32 value;
    u8 gap16[4];
    s32 limit;
    u8 gap24[16];
    s32 state;
    u8 gap44[16];
    s32 mark;
};

struct MotionEffect {
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

extern u8 Data_0200b8f4[];
extern u8 Data_0200ba74[];
extern u8 Data_0200bc0c[];
extern u8 Data_0200bef4[];

void Engine_ObjectSetPosition();
s32 Object_CheckMovementCollision();
s32 Vector_AddPolarOffset();
s32 Battle_GetWorkObject1e0();
void BattleFx_RunRisingObjectSequence(s32, s32, s32);
struct Record_02000ec8;

struct Record_02000ec8;

s32 SceneActor_SetFlagBitByRelativeDepth();

void SceneEffect_SpawnNineRadialEffects(s32 actor);
void Effect_AdvanceMotion();

extern u8 Data_0200b85c[];
extern u16 Data_0200b3ec[];
extern u16 Data_0200b40c[];
s32 OverlayObject_SetYAboveLinkedActor();

#endif

#ifndef GUARD_ENTRY_SETUP_H
#define GUARD_ENTRY_SETUP_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "FIELD_EFFECT.H"
#define NULL ((void *)0)

#include "OVERLAY_OBJECT.H"
#include "STAGED_ACTOR.H"

/*
 * Effect spawning for overlay resource_39f.  Every cutscene beat in the
 * overlay creates its effects through this routine.
 */

struct Sprite {
    u8 pad00[9];
    u8 flags9;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state26;
};

struct Effect {
    u8 pad00[24];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flatla3;
    u8 pad24[12];
    s32 rate30;
    s32 rate34;
    u8 pad38[12];
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    struct Sprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
    u8 filler14[15];
    u8 unk23;
    u8 filler24[49];
    u8 unk55;
} Struct_1a14;

struct Args_02003fe4 {
    s32 first;
    s32 unused;
    s32 last;
};

struct Args_02003ffc {
    s32 first;
    s32 unused;
    s32 last;
};

struct Args_02004014 {
    s32 first;
    s32 unused;
    s32 last;
};

struct Args_02004030 {
    s32 first;
    s32 unused;
    s32 last;
};

struct BusyObject {
    u8 filler00[0x28];
    s32 busy;
};

typedef struct {
    u8 filler0[9];
    u8 unk9;
} Struct_Sub;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
    u8 filler14[48];
    s32 unk44;
    u8 filler48[8];
    Struct_Sub *unk50;
} Struct_1a50;

struct Object_020005e4 { u8 unknown_00[8]; s32 x; s32 y; s32 z; };

struct EffectParams_020005e4 { s32 unk00; s32 mode; s32 color1; s32 color2; u8 unknown_10[0x18]; };

struct Object_020006a0 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct EffectParams_020006a0 {
    s32 unk00;
    s32 mode;
    s32 color1;
    s32 color2;
    u8 unknown_10[0x18];
};

struct EffectObject {
    u8 filler00[0x30];
    s32 color;
    u8 filler34[0x38];
    s32 callback;
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
    s32 unk00;
    s32 mode;
    u8 filler08[0x1c];
    s32 callback;
};

struct SceneObject_02000b98 {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[0x24];
    s32 near;
    u8 filler3c[4];
    s32 far;
};

struct EffectParams_02000b98 {
    s32 unk00;
    s32 mode;
    s32 color1;
    s32 color2;
    s32 unk10;
    s32 unk14;
    u16 angle;
    u8 filler1a[0xa];
    s32 callback;
};

struct EffectObject_02000c5c {
    u8 filler00[0xc];
    s32 timer;
    u8 filler10[0x13];
    u8 mode;
    u8 filler24[0xc];
    s32 scale_x;
    s32 scale_y;
    u8 filler38[0x1d];
    u8 state;
    u8 filler56[3];
    u8 phase;
    u8 filler5a[9];
    u8 flag;
    u8 filler64[8];
    s32 callback;
};

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} Struct_A;

typedef struct {
    u8 filler0[10];
    s16 unkA;
    u8 fillerC[6];
    s16 unk12;
} Struct_B;

struct Actor_020008c8 { u8 unknown_00[8]; s32 x; s32 y; s32 z; u8 unknown_14[0xf]; u8 flatla3; };

struct Position {
    u32 filler00[2];
    s32 x;
    s32 y;
    s32 z;
};

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} Struct_0ff0;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} Struct_199c;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
} Struct_19c0;

typedef struct {
    u8 filler0[8];
    s32 unk8;
} Struct_1a9c;

typedef struct {
    s16 filler0[50];
    s32 unk64;
} Struct_22a4;

typedef struct {
    s32 filler0[3];
    s32 unkC;
} Struct_22a4b;

/*
 * Per-frame motion for an effect record in resource_3c8.
 */
struct Sprite_0200096c {
    u8 pad00[9];
    u8 flags9;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state26;
};

struct Effect_0200096c {
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
    struct Sprite_0200096c *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

struct SceneObject_02000cc8 {
    u8 filler00[6];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[0x14];
    s32 accel;
    u8 filler2c[4];
    s32 scale_x;
    s32 scale_y;
    u8 filler38[0x1d];
    u8 state;
};

union VecView {
    struct Vec v;
    s16 h[6];
};

typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    u8 filler10[4];
    s32 unk14;
    u8 filler18[61];
    u8 unk55;
} Struct_1644;

typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
} Struct_18f8;

extern u32 StagedActor_DirectionSteps[];
extern u8 *gMapWork;
extern u8 gMapCellBuffer[];
extern u32 gFrameCount;
extern u8 gVinasuHeyaPrimaryTable[];

void Engine_ObjectCommitPosition();
void VinasuHeya_SettlePushedBlocks(void);
void VinasuHeya_StepActorWithDust();
void VinasuHeya_SpawnRandomParticles();
void Engine_ActorStartAction();
void BattleFx_RunRisingObjectSequence();
void BattleFx_PlayQueuedSound();
void SetMapCellCollision(s32, s32, s32, s32);
void OverlayObject_WaitUntilIdle();
void Vector_AddPolarOffset(s32, s32, struct Vec *);
s32 Object_CheckMovementCollision();
s32 *SceneData_FindSlotAtPosition();
void SceneEvents_UpdateInView(u8 *);
void Scene_RunScene3c8SequenceA(void);
void VinasuHeya_RunCellPushScene(void);
void *OverlayObject_SpawnWithMode14(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void RunStagedActorTransition(void);
void SceneActor_SwapPositionsByDepth(s32 group, s32 index);
s32 SceneActor_TryMoveActorZeroTwoTilesAhead(void);

/* Complete scene setup, including the two global halfword state writes. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
void SceneEffect_SpawnNineRadialEffects(s32 actor);

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void SetActorDirection(s32 actor, s32 direction, s32 duration)
{
    Actor_FaceDirection(actor, direction, duration);
}

static __inline__ void SetFlagBits(u8 *flags, u8 bits)
{
    *flags |= bits;
}

struct EventWorkState {
    u8 unknown_000[0xcb6];
    u16 field_cb6;
    u8 unknown_cb8[2];
    u16 field_cba;
};

#endif

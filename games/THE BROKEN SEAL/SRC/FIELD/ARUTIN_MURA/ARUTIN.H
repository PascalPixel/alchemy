#ifndef ALCHEMY_ARUTIN_H
#define ALCHEMY_ARUTIN_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

struct Slot02000338 {
    u8 head[6];
    u16 angle;
};

struct SceneMotion {
    u8 unk_00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 ground;
    s32 scale_x;
    s32 scale_y;
    u8 unk_20[8];
    s32 velocity;
    u8 unk_2c[47];
    u8 state;
    u8 unk_5c[8];
    s16 timer;
    s16 delay;
    s32 active;
    void (*callback)(struct SceneMotion *);
};

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
};

struct Actor02000c0c {
    u8 head[18];
    u8 gap18[6];
    s32 span;
    s32 reach;
    u8 gap32[48];
    u8 *attached;
    u8 gap84;
    u8 state;
};

/* A cell pair for each of the village's exit triggers. */
extern u8 ArutinMura_TriggerTable[];

s32 Battle_GetWorkObject1e0();
void BattleFx_SetQueuedSoundAndPlay(s32 sound);
u32 Random16(void);
void SceneEffect_UpdateDriftingParticle(struct SceneMotion *work);
void SceneEffect_SpawnDriftingParticle(void);

#endif

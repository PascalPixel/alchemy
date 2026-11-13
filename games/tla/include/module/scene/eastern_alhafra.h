#ifndef ALCHEMY_MODULE_SCENE_EASTERN_ALHAFRA_H
#define ALCHEMY_MODULE_SCENE_EASTERN_ALHAFRA_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"

struct Actor;
struct RockSprite;
struct Vector3;

struct Unk_02004240 {
  s32 value[6];
};

struct EasternAlhafraSparkWork {
  u8 unk0000[0x1604];
  s16 resource;
};

struct PushState {
  s32 x;
  s32 y;
  s32 z;
  s32 kind;
  s32 number;
  s32 unk14;
};

void Func_02000fa4(void);
void Rocks_Spawn(s32, s32, s32, s32);

void Func_02004004(void);
void Func_020040c0(void);

void Func_020017ec(void);
void Func_02001cb0(void);
void Func_02003274(void);

extern s32 gData_020054f4[];
extern s32 gData_0200556c[];
extern u32 gData_0200d5d0[];
extern u32 gData_02005620[];
extern s32 gData_02005670[];

void Func_02000514(void);

void Func_02000730(void);

void Func_02000838(void);

void Func_02000954(void);
void Func_02000968(void);
void Func_02001574(void);
void Func_02002048(void);
void Func_02002a08(void);

extern s16 gData_02005284[];

void Import_DecompressTaggedLz(void *entry, void *destination);
void Func_02004c64(struct RockSprite *sprite, s32 width, s32 height,
                   u32 attributes, s32 tile);

void Particles_UpdateFalling(void);

extern u16 gPushKinds[];

struct Actor *PushTarget_FindAhead(s32 *direction, s32 *number, s32 *kind);

s32 MapGrid_CheckCollision(struct Actor *actor, struct Vector3 *cell);

void Actor_StopAtPushTarget(struct Actor *object);

struct Actor *Actor_FindAtCell(s32 *position, s32 mode);

void Actor_SetCellOpen(s32 id, u32 kind, s32 flag, s32 layer);

s32 Func_02004c34(struct Actor *object);

s32 Func_02004c24();
s32 Tightrope_Release();
s32 Leader_CanPushObjectAhead();
s32 Func_02004c4c();
extern u16 gData_020056e0[2][160];
void Actor_FallOverWithAnimationById();
void Rope_Initialize(void);
void Tightrope_Initialize(void);
void MapGrid_SetAttributeBAt();

#endif

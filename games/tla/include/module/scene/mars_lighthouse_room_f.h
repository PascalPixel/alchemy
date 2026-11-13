#ifndef ALCHEMY_MODULE_SCENE_MARS_LIGHTHOUSE_ROOM_F_H
#define ALCHEMY_MODULE_SCENE_MARS_LIGHTHOUSE_ROOM_F_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "module/scene/scene.h"

struct EffectOptions;
struct LightOam;

struct WarpWork {
  s16 unk00;
  s16 positionFlag;
  s16 unk04;
  s16 sourceX;
  s16 sourceZ;
  s16 count;
  s16 current;
  s16 busy;
  u8 unk10[0x10];
  struct WarpEntry entries[1];
};

extern s16 gData_02005f18[];

void Func_02000cb8(s32 id);

void Func_02004700(s32 value);

void Func_020025b0(void);

void Func_02002604(void);

void Func_020027cc(s32 index, s16 value);

void Func_02003d80(void);

s32 Func_02004938(s32 id);

void LightSprites_InitB(void);

void Func_0200538c(s32 id, s32 value);

void Func_020053bc(s32 value);

void Func_02001468(void);

void Func_02001490(void);

void Func_02001894(struct Actor *self);

void Func_020035b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

s32 Func_02001d5c(struct Actor *a, struct Actor *b);
s32 Func_02001dc0(struct Actor *a, struct Actor *b);
void Func_02001e20(struct Actor *object);

void Func_02001ffc(struct Actor *object);

void Func_02001fb4();
void Func_020021a0();

void Func_020021e8();

void Func_02002230();
void Func_0200242c(void);
void Func_020000bc(void);

void Func_02000168(void);

void Func_02000170(void);

void Func_0200021c(void);

void Func_02000418(void);

void Func_020004f8(void);

void Func_02000554(void);

void Func_02000754(void);

void Func_02000760(void);

void Func_0200076c(void);

void Func_02000790(void);

void Func_020007a8(void);

void Func_020007c0(void);

void Func_020007d8(void);

void Func_020007e4(void);

void Func_020008bc(void);

void Func_02000944(void);

void Func_02000ad8(void);

void Func_02000b40(void);

s32 MarsLighthouseRoomF_SlideOnIce(void);

void Func_02004140(void);

extern u32 gData_02006980[];

void Func_02002800(void);

s32 MapGrid_GetCellAttributeLowNibbleOrBump(s32 terrain, s32 x, s32 z);

extern struct WarpWork gData_020070a8;

void Leader_CenterInCell(void);

s32 Import_FieldCheckSpheresOverlap(s32 *a, s32 radiusA, struct Position *b,
                                    s32 radiusB);

extern volatile s16 gData_020074c8;

s32 Actor_HasArrived(struct Actor *object);

void Func_0200410c(void);
void Func_02004080(struct Actor *object);
void Func_02004a44(s32 layer, s32 x, s32 z, u32 width, u32 height, s32 value);
struct Actor *PushTarget_FindAhead(s32 *direction, s32 *slot, s32 *kind);

s32 MapGrid_CheckCollision(struct Actor *object, s32 *position);

void Actor_StopAtPushTarget(struct Actor *object);

extern s16 gData_020074cc;
extern s16 gData_020074ce;
extern struct Actor *gData_020074d0[];
extern struct LightOam Data_0200f510[];
extern s16 gData_020075d0;
extern s16 gData_020075d2;

s32 BattleEffect_GetObjectPriority(struct Actor *actor);
void Import_OamAddSpriteAtDepthClamped(struct LightOam *oam, s32 depth);

void Func_02000d80(void);

void Actor_Release(void *handle);

#endif

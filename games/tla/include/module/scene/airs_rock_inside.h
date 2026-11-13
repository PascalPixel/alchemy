#ifndef ALCHEMY_MODULE_SCENE_AIRS_ROCK_INSIDE_H
#define ALCHEMY_MODULE_SCENE_AIRS_ROCK_INSIDE_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"
#include "scene_tables.h"
#include "actor.h"

struct Actor;

struct AirsRockInsideVector {
  s32 x;
  s32 y;
  s32 z;
};

struct AirsRockInsideSpawn {
  s32 mode;
  s32 unk04;
  s32 scaleX;
  s32 scaleY;
  s32 speed;
  s32 acceleration;
  u16 kind;
  const u8 *script;
  s32 unk20;
  void *update;
};

struct SparkBytes {
  u8 unk00[0x62];
  u8 unk62;
  u8 unk63;
};

union SparkView {
  struct Actor actor;
  struct SparkBytes bytes;
};

extern struct ActorPlacement gData_0200654c[];
extern struct ActorPlacement gData_02006564[];
extern struct ActorPlacement gData_0200663c[];
extern struct ActorPlacement gData_020066b4[];
extern struct ActorPlacement gData_020068ac[];

void Func_020026c0(void);

s32 Func_02005f64(struct Actor *object);

void Actor_SetCellOpen(s32 id, u32 kind, s32 flag, s32 layer);

void Func_02002aa8(s32 x, s32 z);
void Func_02002b58(s32 x, s32 z);

void WindowColor_Apply(s32 hue, s32 brightness);

s32 Func_02000690(u32 layer, s32 x, s32 y);

extern s16 gData_02006050[];
extern s16 gData_02006054[];
extern s16 gData_0200605a[];
extern s16 gData_02006064[];
extern s16 gData_02006068[];
extern s16 gData_02006072[];
extern s16 gData_0200607e[];

void Actor_LiftTargetBlock(const s16 *list);
void Actor_PlaceMovedBlock(void);

void Func_02002790(void);
void Func_02004534(void);

s32 Func_02005ea4();
s32 Func_02005eac();

extern struct Trigger gData_02006c10[];
extern struct Trigger gData_02006d6c[];
extern struct Trigger gData_02006e2c[];
extern struct Trigger gData_02006eec[];
extern struct Trigger gData_02006fa0[];
extern struct Trigger gData_0200706c[];
extern struct Trigger gData_020070fc[];
extern struct Trigger gData_02007228[];
extern struct Trigger gData_02007318[];
extern struct Trigger gData_020073a8[];

void FieldEffect_SetAttached(u32 id, s32 mode);
void MapGrid_WaitForShakeToSettle(void);

void Func_02002770(void);
void Func_02004f3c(void);
void Func_02004eb0(void);

void Actor_PlaceBlocks(u16 *actors);

void Func_02004cc8(void);

void Scene_QueueMusic(s32 id);
void Tightrope_Initialize(void);
s32 Func_02005f5c(s32 slot, s32 a, s32 b);

void Func_02000908(u16 *list);
void Func_0200061c(void);
void Func_02000988(void);
void Func_02001828(void);

void Func_02002898(void);
void Func_02002980(void);
void Func_02002a14(void);
void Func_02002cf0(void);
void Func_02003170(void);
void Func_02003254(void);
void Func_0200332c(void);
void Func_02003654(void);
void Func_02003ac4(void);
void Func_02003b08(void);
void Func_02003cac(void);
void Func_02003d40(void);
void Func_02003dd4(void);
void Func_02003e28(void);
void Func_02004740(void);
void Func_020047b0(void);
void Func_02004818(void);
void Func_02004884(void);
void Func_020048bc(void);
void Func_020048c4(void);
void Func_02004ab8(void);
void Func_02004bb4(void);
void Func_02004880(void);

extern u32 gData_02005fd8[];
extern u32 gData_020061f8[];

void MapGrid_SetCellHeight(s32 x, s32 z, s32 terrain, s32 height);
struct Actor *FieldCamera_GetActor(void);
void Func_02005f8c(struct Actor *object, s32 cue);
s32 Actor_HasArrived(struct Actor *object);

#endif

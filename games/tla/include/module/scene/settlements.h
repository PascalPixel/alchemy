#ifndef ALCHEMY_MODULE_SCENE_SETTLEMENTS_H
#define ALCHEMY_MODULE_SCENE_SETTLEMENTS_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

extern s16 gData_020022b6[];

void Func_0200125c(void);

void Func_0200173c(void);

void Func_02000290(void);

void Func_02000404(void);
void Func_02000488(void);

void Func_020004a8(void);

void Func_020005f4(void);

void Func_02000624(void);

void Func_02000c1c(void);

void Func_02000c2c(void);
void Func_02000c3c(void);
void Func_02000cfc(void);

void Func_02000ef0(void);

void Func_02000f00(void);

void Func_020011c8(void);

void Func_020012c0(void);

void Func_02001304(void);

void Func_0200155c(void);

void Func_020016a4(void);

extern u32 gData_020021b8[];
extern u32 gData_020021f4[];
extern s32 gData_020022e4[];
extern u32 gData_0200231c[];

struct OrbitSprite {
  u8 unk00[5];
  u8 flags05Low : 5;
  u8 flags05Bit5 : 1;
  u8 flags05High : 2;
  u8 unk06[3];
  u8 flags09Low : 2;
  u8 flags09Mode : 2;
  u8 flags09High : 4;
  u8 unk0a[6];
  u8 palette;
  u8 unk11[10];
  u8 state;
};

struct OrbitObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[15];
  u8 flags23;
  u8 unk24[12];
  s32 orbitAngle;
  u8 unk34[4];
  s32 orbitCenterX;
  s32 orbitCenterY;
  s32 orbitCenterZ;
  u8 unk44[12];
  struct OrbitSprite *sprite;
  u8 unk54;
  u8 mode;
  u8 state;
  u8 unk57[5];
  u8 active;
  u8 unk5d[4];
  u8 visible;
  u8 unk62[10];
  u32 callback;
};

struct SettlementsVector {
  s32 x;
  s32 y;
  s32 z;
};

struct SettlementsSpawn {
  s32 unk00;
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

extern char SceneId_Ov690Room1;
extern char SceneId_Ov690Room2;
extern char SceneId_Ov690Room4;
extern char SceneId_Ov690Room7;
extern char SceneId_Ov690Room8;
extern char SceneId_Ov690Room9;
extern char SceneId_Ov690Room10;

void Func_02002170(void);

extern char SceneId_Ov690Room6;

void Func_0200125c(void);

void Func_02002140(s32 a);

void Func_02002188(const void *data);

extern u16 gData_02002c60[];
extern u16 gData_02002c80[];

void Func_02001620(s32 objectId);

void Func_020011bc(void);

#endif

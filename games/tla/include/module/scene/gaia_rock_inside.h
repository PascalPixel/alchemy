#ifndef ALCHEMY_MODULE_SCENE_GAIA_ROCK_INSIDE_H
#define ALCHEMY_MODULE_SCENE_GAIA_ROCK_INSIDE_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "scene_tables.h"

struct Actor;

// One grid cell: the step that stands on it (0 for none, 99 for the border)
// and the column and row of the next step.
typedef struct {
  s8 unk_0;
  s8 unk_1;
  s8 unk_2;
} Unk_Cell;

// One step of the path: its number, then the step found below, to the left,
// to the right and above it.
typedef struct {
  u8 unk_0;
  u8 unk_1;
  u8 unk_2;
  u8 unk_3;
  u8 unk_4;
  u8 unk_5;
  u8 unk_6;
  u8 unk_7;
} Unk_Step;

// At gData_020023c4 + 0x288.
typedef struct {
  s32 unk_0;
  Unk_Step steps[15];
} Unk_Path;

struct Counted {
  s32 *script;
  u8 unk04[0x57];
  u8 movementState;
  u8 unk5c[4];
  u8 count;
};

struct EventWork900 {
  u8 unk000[362];
  u16 timer;
};

struct CellPoint {
  s16 position[2];
};

struct BoulderSprite {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 layer : 2;
};

struct GaiaRockInsideOrbit {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x30];
  s32 centreX;
  s32 centreZ;
  s32 radius;
  u8 unk50[0x12];
  u8 phase;
  u8 growth;
  s16 angle;
  s16 speed;
};

struct GaiaRockInsideEventWork {
  u8 unk000[0x15a];
  u16 unk15a;
};

struct SwayState {
  u16 frame;
  u16 pending;
};

struct GaiaRockInsideLayer {
  u8 unk00[8];
  s32 x;
  s32 z;
  u8 unk10[0x28];
};

struct GaiaRockInsideMapWork {
  u8 unk000[0x108];
  struct GaiaRockInsideLayer layers[2];
};

struct GaiaRockInsideNode {
  u8 unk0[2];
  u8 next[4];
  u8 unk6[2];
};

struct GaiaRockInsidePath {
  s32 current;
  struct GaiaRockInsideNode nodes[1];
};

extern u32 gData_020050f4[];
extern s32 gData_020056c4[];
extern s32 gData_02005728[];

extern char SceneId_Ov681Room2;
extern char SceneId_Ov681Room3;
extern char SceneId_Ov681Room4;

extern struct ActorPlacement gData_02005970[];
extern struct ActorPlacement gData_020059a0[];
extern struct ActorPlacement gData_02005afc[];
extern struct ActorPlacement gData_02005b8c[];
extern struct ActorPlacement gData_02005c7c[];
extern s32 gData_02005d6c[];

void Func_02004ff4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void Func_0200500c(s32 a, s32 b, s32 c);

extern u32 gData_020051b8[];
extern s32 gData_02005118[];
extern s32 gData_02005168[];

extern s32 gData_020064ec;
extern s32 gData_020064f0;
extern s32 Data_020064f4;

extern s32 gData_020055c0[];
extern s32 gData_02005610[];

void Func_02000f00(void);
void Func_02000fac(void);

void FieldCamera_WaitForArrival(void);
void Import_ActorDestroy(struct Actor *object);

extern char SceneId_Ov681Room1;

s32 Func_02004f8c();

void Func_02002b58(s32 id, s32 x, s32 z);

void Func_02002b04(u32 id, s8 mode);
void Func_02002b8c(void);

void Func_0200495c(s32 layer, s32 keep);

extern u32 gData_020053e4[];

void Func_020044f8(s32 level);
void Func_020048a0(void);
s32 Func_02004904(void);

void Func_02004e0c(s32 x, s32 z, s32 width, s32 height, s32 cellX, s32 cellZ);

void Func_0200036c(void);

void Func_02000c38(void);

void Func_02000c74(void);

void Func_02000cf8(void);

void Func_02000d18(void);

void Func_02000edc(void);

void Func_02001048(void);
void Func_020011c8(void);
void Func_020012ac(void);

void Func_02001d14(void);
void Func_02001ebc(void);

void Func_02001f68(void);

void Func_02001f84(void);
void Func_02001fa0(void);
void Func_02001fd8(void);
void Func_02002020(void);
void Func_0200225c(void);
void Func_020024b8(void);
void Func_02002718(void);

void Func_02002a14(void);

void Func_02002a24(void);
void Func_02002a34(void);

extern u32 gData_0200501c[];
extern u32 gData_02005058[];
extern u32 gData_02005094[];

#endif

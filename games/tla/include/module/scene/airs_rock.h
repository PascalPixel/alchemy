#ifndef ALCHEMY_MODULE_SCENE_AIRS_ROCK_H
#define ALCHEMY_MODULE_SCENE_AIRS_ROCK_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "scene_tables.h"

struct Unk_02000468 {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x63 - 0x14];
  u8 unk63;
};

struct AirsRockMapLayer {
  u8 unk00[0x30];
  u8 *cells;
  u8 unk34[4];
};

struct AirsRockMapWork {
  u8 unk000[0x108];
  struct AirsRockMapLayer layers[3];
};

s32 Func_02000348(u32 layer, s32 x, s32 y);

extern s16 gData_02003e84[];
extern s16 gData_02003ec4[];
extern struct ActorPlacement gAirsRock_Room1Placements[];
extern struct ActorPlacement gAirsRock_Room2Placements[];
extern struct ActorPlacement gAirsRock_Placements[];
extern struct Trigger gAirsRock_Room1Triggers[];
extern struct Trigger gAirsRock_Room2Triggers[];
extern struct Trigger gAirsRock_Triggers[];

void Func_020024b0(void);
void Func_0200257c(void);

void Func_02000ef4(void);

extern s16 gData_02003d94[];
extern s16 gData_02003e4c[];

extern s8 gData_02004694;
extern s8 gData_02004695;
extern s8 gData_02004696;
extern s8 gData_02004697;

void Func_02000548(void);

void Func_02000568(void);
void Func_020005ec(void);

void Func_020006d4(void);

void Func_02000700(void);
void Func_02000824(void);
void Func_02000830(void);

void Func_0200095c(void);

void Func_02001010(void);
void Func_02001560(void);
void Func_02001ca4(void);
void Func_02001cc8(void);
void Func_02001da8(void);
void Func_02001f7c(void);
void Func_020026b8(void);

extern u32 gData_02003ce0[];
extern u32 gData_02003d1c[];
extern u32 gData_02003d58[];
extern s32 gData_02003dac[];

void Func_02002b88(void);

struct Actor;

extern char SceneId_Ov663Room1;
extern char SceneId_Ov663Room2;
extern char SceneId_Ov663Room3;

#endif

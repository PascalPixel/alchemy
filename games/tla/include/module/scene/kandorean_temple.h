#ifndef ALCHEMY_MODULE_SCENE_KANDOREAN_TEMPLE_H
#define ALCHEMY_MODULE_SCENE_KANDOREAN_TEMPLE_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"
#include "scene_tables.h"

struct MapWork17 {
  s32 *origin;
  s32 shakeX, shakeY, shakeDecay;
  u8 *script;
  u16 flags;
  u8 wideTiles;
  u8 bits0 : 2;
  u8 mode : 2;
};

extern u32 gData_020056c0[];
extern struct ActorPlacement gData_02005b20[];
extern struct ActorPlacement gData_02005be0[];
extern s16 gData_02005de4[];
void Func_020037e0(void);
void Func_02005028(void);

void Func_02000d64(void);

void Func_02003e8c(void);
void Func_020044d4(void);
void Func_020046d8(void);
void Func_0200482c(void);
void Func_02004fb4(void);
extern u32 gData_02005564[];
extern u32 gData_020055a0[];
extern u32 gData_020055e4[];

void Func_02000bcc(void);
void Func_020004a8(void);

void Func_02000710(void);

void Func_02001d00(void);

void Func_02001d20(void);

void Func_02001e0c(void);

void Func_02001e30(void);

void Func_02001e44(void);
void Func_02001e58(void);

void Func_02001f7c(void);

void Func_02002014(void);
void Func_020020c8(void);
void Func_020020d8(void);
void Func_02002154(void);
void Func_020021b8(void);
void Func_020021dc(void);

void Func_02002248(void);

void Func_020022c0(void);
void Func_02002314(void);
void Func_02002674(void);
void Func_0200269c(void);
void Func_020026cc(void);
void Func_020027b4(void);
void Func_020027d4(void);
void Func_020027f4(void);
void Func_02002814(void);

void Func_02002834(void);

extern u32 gData_020053e0[];

extern u32 gData_0200541c[];

extern s32 gData_02005634[];
extern s32 gData_0200564c[];
extern s32 gData_02005664[];

void Func_02003684(void);

extern s32 gData_020062a0;
extern u16 gData_020062a8;
extern u16 gData_020062aa;
void Func_02005398(void);
void Func_020053b8(const void *data);

#endif

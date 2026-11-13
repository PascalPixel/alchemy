#ifndef ALCHEMY_MODULE_SCENE_MAGMA_ROCK_H
#define ALCHEMY_MODULE_SCENE_MAGMA_ROCK_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct Actor;
struct ResourceEntry;

struct MagmaRockEffectOptions {
  s32 kind;
  s32 palette;
  s32 scaleX;
  s32 scaleY;
  u8 unk10[8];
  u16 id;
  u8 unk1a[2];
  const void *data;
  s32 unk20;
  void (*update)();
};

struct Func_02002fc4_Entry {
  u16 id;
  u16 flag;
};

struct Entry {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 unk18;
};

struct EntryWork {
  u8 unk0[0x500];
  struct Entry entries[64];
  u8 unkc00[0xa00];
  s16 unk1600;
};

struct MagmaRockSparkWork {
  u8 unk0000[0x1604];
  s16 resource;
};

s32 MapCell_GetAttributes(u32 layer, s32 x, s32 z);

extern s16 gData_02003d98[];

extern s16 gData_02003dac[];

extern s16 gData_02003dd2[];
extern s16 gData_02003de4[];
extern s16 gData_02003de8[];

void Func_02000dc0(struct Actor *object);

extern u32 gData_02003d50[];

void Func_02001738(struct Actor *object);

extern s16 gData_02003d9e[];
extern s16 gData_02003db2[];
extern s16 gData_02003dc4[];
extern s16 gData_02003dd6[];
extern s16 gData_02003dee[];
extern s16 gData_02003e02[];

void Func_02000fcc(struct Actor *object);

void Func_02003a54(struct Actor *object, s32 a);

void Func_02003820(s32 x, s32 y, s32 z, s32 count);
void Func_020038dc(void);
void Func_02003998(void);

void Func_020014bc(s32 mode, s32 cue);

void Func_02003b3c(s32 id);

void Func_020036f8(s32 heightOffset, s32 layer, s32 id);

struct Actor *Actor_FindInCell(s32 *position, struct Actor *self);

extern s16 gData_02003dba[];

void Func_02003c0c();

void Func_02002d08(const u16 *table);

void Func_02003bec();
void Func_02003bf4();
void Func_02003be4();
s32 Func_02003c04(struct Actor *object);
s32 Func_02003b8c(void);
struct Actor *Func_02003c2c(s32 *position, s32 flags);

void Func_020034ac(s32 spread);
void Func_02003784(s32 a, s32 b);
extern u32 gData_02003e0c[];

struct ResourceEntry *Func_02003bd4(void *sprite, struct ResourceEntry *last);

extern u32 gData_02003e3c[];

void Func_02001444(void);

void Func_0200148c(void);
void Func_02001b24(void);

void Func_02001b44(void);

void Func_02001ba4(void);
void Func_02001bb0(void);

void Func_02001bc0(void);

void Func_02001be0(void);
void Func_02001c00(void);
void Func_02001e08(void);
void Func_02001edc(void);
void Func_02001fb0(void);
void Func_02001fd4(void);
void Func_02001ffc(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

s32 SceneActor_CalculateFixedPointDistance(s32 *a, s32 *b);

#endif

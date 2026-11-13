#ifndef ALCHEMY_MODULE_SCENE_MAGMA_ROCK_INSIDE_H
#define ALCHEMY_MODULE_SCENE_MAGMA_ROCK_INSIDE_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct Actor;
struct FieldSprite;
struct Particle;
struct ParticleSprite;
struct RockSprite__s4b18;

struct MagmaRockInsideSparkWork {
  u8 unk0000[0x1604];
  s16 resource;
};

void Func_020007ac();

extern s16 gData_020054fc[];

extern s16 gData_02005508[];

extern s16 gData_02005520[];

extern s16 gData_0200552e[];

void Func_020006dc(u16 *ids);

void Func_0200063c(struct Actor *object);

s32 Func_02000658(u32 layer, s32 x, s32 y);

s32 Func_02000dcc(u16 *table);

s32 Func_020005b0(s32 *from, s32 *to);

void Func_02000d1c(struct Actor *object);
void Func_02001350(void);

void Func_02004a5c(s32 x, s32 y, s32 z, s32 count);

void Func_02004b18(void);

void Func_020034bc(void);
void Func_020034f0(void);
void Func_02003548(void);

extern s16 gData_0200525c[];
extern s16 gData_020052bc[];
extern s16 gData_0200531c[];
extern s16 gData_0200537c[];

void Func_02004e60(s32 value);

void Func_02000880(u16 *list);

void Func_02004f08(s32 id, s32 value);
s32 Func_02004ed0(struct Actor *object);
s32 Func_02004e38(void);
struct Actor *Func_02004ef8(s32 *position, s32 mode);

void Func_020046e8(s32 spread);
void Func_020049c0(s32 a, s32 b);
void Func_0200481c(void);
extern u32 gData_02005638[];

u32 Func_02004e98(struct FieldSprite *sprite, u32 frame);

extern u32 gData_02005668[];

void Func_02004f18(struct ParticleSprite *sprite, struct Particle *particle);
void Func_02004c58(void *entry, void *destination);
void Func_02004f10(struct RockSprite__s4b18 *sprite, s32 width, s32 height,
                   u32 attributes, s32 tile);

void Func_020049d8(void);

void Func_02000594(void);

void Func_0200099c(void);
void Func_02000a30(void);
void Func_020011fc(void);

void Func_02001828(void);

void Func_02001d64(void);
void Func_02002128(void);

void Func_020021c0(void);

void Func_02002318(void);

void MagmaRockInside_ExamineEmberStone(void);

void Func_0200257c(void);

void Func_02002610(void);

void Func_020027d4(void);

void Func_02002868(void);
void Func_020028ac(void);
void Func_020029dc(void);
void Func_020030cc(void);
void Func_020032d8(void);
void Func_02003348(void);
void Func_020033b0(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];

void Func_02001190(s32 mode, s32 cue);

#endif

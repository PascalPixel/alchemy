#ifndef ALCHEMY_MODULE_SCENE_TUNDARIA_TOWER_H
#define ALCHEMY_MODULE_SCENE_TUNDARIA_TOWER_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct Actor;
struct Particle;
struct ParticleSprite;
union FieldObject;

struct TundariaTowerEventWork {
  u8 unk000[428];
  s32 screenEffect;
};

void Func_02001868(void);

s32 Func_02003028(s32 *a, s32 aRadius, s32 *b, s32 bRadius);

void Func_02002184(s32 id, u32 kind, s32 flag, s32 layer);

s32 Func_02003160(struct Actor *object);
s32 Func_02003010(s32 layer, s32 x, s32 z);
s32 Func_020030f8(void);
struct Actor *Func_02003188(s32 *position, s32 mode);

void Func_020029d0(s32 spread);
void Func_02002ca8(s32 a, s32 b);

void Func_02002b04(void);

void Func_02003090(s32 id, s32 target);
void Func_02002fa8(void);

extern union FieldObject *Data_02003ccc;

void Func_020010d4(void);
void Func_02000548(void);
void Func_0200058c(void);
void Func_02000404(const u8 *data);

extern s16 gData_02003272[];
extern s16 gData_020032b2[];

extern u32 gData_020032d4[];

extern u32 gData_02003304[];

void Func_02003198(struct ParticleSprite *sprite, struct Particle *particle);
void Func_020031a0(struct Particle *particle, s32 unk1, s32 step);
void Func_02002f40(void *source, void *destination);

void Func_020007dc(void);
void Func_02000920(void);
void Func_0200095c(void);
void Func_02000964(void);
void Func_02000b34(s32 arg0, s32 arg1);
void Func_02000d2c(void);
void Func_02000de4(void);
void Func_02000e78(void);
void Func_02000f00(void);
void Func_02000f0c(void);
void Func_02000fac(void);
void Func_02000fb8(void);

extern u32 gData_020031b0[];
extern u32 gData_020031ec[];
extern u32 gData_02003228[];

#endif

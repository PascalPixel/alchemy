#ifndef ALCHEMY_MODULE_SCENE_DEHKAN_PLATEAU_H
#define ALCHEMY_MODULE_SCENE_DEHKAN_PLATEAU_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

struct Actor;

struct EffectWork1a {
  u8 unk00[0x1a];
  s16 active;
};

/// A shape grid byte seen as a bit-field, so its store takes the struct's
/// alias set (see the FAKEMATCH tag in Func_02000d78).
struct ShapeBits {
  u32 shape : 8;
};

void Func_02002390(void);

void Pillars_Pound(const void *data, s32 mode, s32 timer);

void Func_02001474(s32 show);

void Actor_DropAndWait(struct Actor *object);
void Func_0200172c(void);

void Import_ActorRelease(struct Actor *object);

void Func_02002b68(void);
void Func_02002f08(void);

extern u16 gData_02003fea[];
extern u16 gData_020041b6[];
extern u16 gData_020041b8[];

void CrumblingPillar_Prepare(s32 id);

void Rubble_Spray(s32 spread);

void Rubble_Burst(void);

void Leader_SetCellShapeAt(s32 a, s32 b);

extern u16 gData_02003ff2[];
extern u16 gData_02004012[];
extern u16 gData_02004032[];
extern u16 gData_02004052[];
extern u16 gData_02004068[];
extern u16 gData_02004110[];
extern u16 gData_02004170[];

void Func_020021b4(void);
void Func_02002604(void);

void Func_0200118c(void);

void Func_020012c8(void);
void Func_02001368(void);
void Func_020013fc(void);
void Func_02001444(void);
void Func_020014ec(void);

void Func_02001500(void);

void Func_020015e0(void);

void Func_02001630(void);

void Func_02001640(void);
void Func_020016a0(void);

void Func_020017a0(void);

void Func_020017f4(void);

void Func_02001a30(void);
void Scene_RunIfHeld(void);
void Func_02002114(void);
void Func_02002124(void);
void Func_020021e8(void);
void Func_02002458(void);

void Func_020027e0(void);
void Func_02002a5c(void);
void Func_02002f74(void);
void Func_02002f9c(void);
void Func_02002ff4(void);

extern u32 gData_02003e60[];
extern u32 gData_02003e9c[];

void Actor_SetCellOpen(s32 id, u32 mode, s32 flag, s32 layer);

void Actor_SetCellShape(s32 level, s32 layer, s32 owner);

void Func_02001f94(void);

struct Actor;
struct EffectOptions;

struct AnimationFacing {
  u8 unk00[9];
  u8 low : 2;
  u8 facing : 2;
  u8 high : 4;
};

void Func_02002140(void);

void Func_02002bcc(void);

void Func_02002698();
void Func_020030e4(void);

void Func_02003164(void);

void Func_02003284(void);
void Func_0200369c(void);
void Func_0200390c(void);
void Func_02003aa0(void);

void CrumblingPillars_Place(const void *data);

void Func_020012d8(void);
void Func_02001fac(void);

void Func_020013b0(void);
void Func_02001930(void);
void Func_02001fc4(void);

void Func_02003d90(void);

s32 Func_02003da0(s32 a, s32 b, s32 c);

void Func_02003da8(s32 a, s32 b, s32 c, s32 d);

void Func_02001fdc(void);

void Func_02003764(void);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

void Func_020019b8(void);

void Func_02002c14(void);
void Func_020035ec(void);
void Func_02003a30(void);

void Func_02001ff4(void);

void Func_02003de8(void);
void Func_02003ba0(struct Actor *object, s32 mode);
s32 Func_02003db0(struct Actor *object);
extern s32 gData_0200567c;

#endif

#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_8_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_8_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "field/cell_target.h"

struct Actor;
struct CellTarget;
struct CellTargetEntry;
struct FieldEffect;

struct IsletsVector {
  s32 x;
  s32 y;
  s32 z;
};

s32 Func_02001f60(void);

void Func_020022c0(s32 x, s32 z, s32 a, s32 b);

void Func_02002248(s32 id);

void Func_02002010(s32 x, s32 z, s32 effect);

void Func_02000068(void);

extern u32 gData_020027a8[];
extern u32 gData_020027e4[];
extern u32 gData_02002820[];

struct Actor *Func_02000dac(s32 *direction, s32 *slot, s32 *kind);

extern u16 gData_020022d0[];

extern s16 gData_02002af2;
extern s16 gData_02002bf4;
extern s16 gData_02002bf6;

void Func_02002160(struct FieldEffect *effect, s32 mode);

void Func_02001b38(void);

extern struct CellTarget gData_02002bf8;

void Func_02001bc0(struct CellTarget *table, struct CellTargetEntry *entry);

extern u16 gData_020022d0[];

#endif

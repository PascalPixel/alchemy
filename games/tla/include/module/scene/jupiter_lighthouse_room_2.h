#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_2_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_2_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "field/cell_target.h"
#include "module/scene/scene.h"

struct Actor;

void Func_020021d4(struct Actor *object);

void Func_02000b28(s32 id);

void Func_02001498(void);

void Func_020015a0(s32 id, s32 value);

void Func_02002314(s32 a);

void Func_02000aa4(s32 mode);

extern s32 Data_02002b94;

s32 Func_020022fc(s32 x, s32 z, s32 layer);

s32 Func_02000d68(s32 *a, s32 *b);

extern s16 gData_02002bd8;
extern s16 gData_02002bda;
extern struct Actor *gData_02002bdc[];
extern struct LightOam Data_02002c1c[];
extern s16 gData_02002cde;

s32 Func_020022f4(struct Actor *actor);

void Func_02001b94(void);

extern struct CellTarget gData_02002ce0;

void Func_020003f4(void);
void Func_02000410(void);

void Func_02000440(void);

void Func_02000454(void);

void Func_020006dc(void);

void Func_02000704(void);

extern u32 gData_020027a8[];
extern u32 gData_020027e4[];
extern u32 gData_02002820[];

#endif

#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_7_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_7_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"
#include "module/scene/scene.h"

struct CellTarget;
struct CellTargetEntry;
struct LightOam;

void Func_020012fc(void);

void Func_02000060(s32 value);

void Func_020000b4(void);

void Func_02000100(void);

void Func_02000160(void);

void Func_02000184(void);

void Func_020009d8(void);
void Func_02000a74(void);

void Func_02000c7c(void);

void Func_02000cec(void);

extern u32 gData_02003110[];
extern u32 gData_0200314c[];
extern u32 gData_02003188[];

void Func_02001acc(s32 id, u32 mode, s32 flag, s32 layer);

s32 Func_02002c1c(struct Actor *object);

extern s16 gData_020035fc;
extern struct LightOam Data_02003640[];
extern s16 gData_02003700;

void Func_02002aa4(struct LightOam *oam, s32 depth);

void Func_02002534(struct CellTarget *table, struct CellTargetEntry *entry);

void Func_020025a4(struct CellTarget *table, struct CellTargetEntry *entry);

#endif

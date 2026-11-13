#ifndef ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_M_H
#define ALCHEMY_MODULE_SCENE_JUPITER_LIGHTHOUSE_ROOM_M_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"
#include "field/cell_target.h"
#include "module/scene/scene.h"

struct CellTarget;
struct CellTargetEntry;

void Func_02002efc(s32 value);

extern u32 gData_02005524[];
extern u32 gData_02005548[];
extern u32 gData_020056ec[];
extern u32 gData_020057d0[];
extern u32 gData_02005800[];
extern u32 gData_0200d830[];
extern u32 gData_020058d8[];
extern u32 gData_02005920[];

extern u32 gData_020049cc[];
extern u32 gData_020049ec[];
extern u32 gData_020049fc[];
extern u32 gData_02004a18[];
extern u32 gData_02004a34[];
extern u32 gData_02004a50[];

extern u32 gData_0200599c[];

extern u32 gData_02004a6c[];
extern u32 gData_02004a90[];
extern u32 gData_02004ab4[];
extern u32 gData_02004ae8[];
extern u32 gData_02004b1c[];

extern u32 gData_020059a4[];
extern u32 gData_02005a00[];

void JupiterLighthouse_LoadPalettes(s32 mode);

void Func_02000060(void);

void Func_02000080(void);

void Func_0200060c(void);

void Func_020008c8(void);

void Whirlwind_CarryLeader(void);

extern u32 gData_02004fc4[];
extern u32 gData_02005000[];

extern s16 gData_02005a80;
extern s16 gData_02005a82;
extern struct Actor *gData_02005a84[];
extern struct LightOam Data_02005ac4[];
extern s16 gData_02005b86;

s32 Func_0200499c(struct Actor *actor);
void Func_020046f4(struct FieldEffect *effect, s32 mode);

extern struct CellTarget Data_02005b88;

void Teleporter_Uncover(struct CellTarget *table,
                        struct CellTargetEntry *entry);

void Teleporter_Sink(struct CellTarget *table, struct CellTargetEntry *entry);

#endif

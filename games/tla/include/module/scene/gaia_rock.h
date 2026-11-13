#ifndef ALCHEMY_MODULE_SCENE_GAIA_ROCK_H
#define ALCHEMY_MODULE_SCENE_GAIA_ROCK_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"
#include "scene_tables.h"

struct Actor;

extern s16 gData_02000f5c[];
extern u32 gData_02000f8c[];

void Func_02000bc0(s32 id, s32 flag);

extern struct ActorPlacement gData_0200102c[];
extern struct ActorPlacement gData_02001044[];

extern struct ActorPlacement gData_02001194[];
extern s16 gData_020012cc[];

void Func_02000edc(s32 a);

extern struct Trigger gData_020013c8[];
extern struct Trigger gData_020014b8[];
extern struct Trigger gData_02001554[];
extern u32 gData_020015c0[];

void Func_02000e24(struct Actor *object, s32 a);

extern s32 gData_020015c4[];

void Func_02000dd0(void);
void Func_02000b2c(s32 id, s32 x, s32 y, s32 z);

extern s32 gData_020016cc[];

extern u32 gData_02001688[];
extern u32 gData_0200101c[];

#endif

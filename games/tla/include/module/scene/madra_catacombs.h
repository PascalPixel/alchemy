#ifndef ALCHEMY_MODULE_SCENE_MADRA_CATACOMBS_H
#define ALCHEMY_MODULE_SCENE_MADRA_CATACOMBS_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "scene_tables.h"

struct Actor;

s32 Func_020011f4(s32 layer, s32 shape, s32 height);

void Func_0200056c(s32 id);

void Func_02000974(s32 mode);

void Func_02000aa8(void);

void Func_02000c4c(s32 level, s32 layer);

struct Actor *Func_02003bb0(s32 type, s32 x, s32 y, s32 z);
void Func_02003c28(void);
void Func_02003c68(s32 id, s32 x, s32 z);
void Func_02003c78(s32 id, s32 target);
void Func_02003df8(s32 cue);
extern struct SceneEntrance gData_020013d0[];
extern struct SceneEntrance gData_02001400[];

extern struct SceneEntrance gData_02001430[];

extern s16 gData_020014a8[];

extern struct ActorPlacement gData_0200159c[];

extern struct ActorPlacement gData_020015b4[];

extern struct ActorPlacement gData_0200165c[];

extern struct ActorPlacement gData_020016bc[];
extern struct ActorPlacement gData_020016ec[];
extern struct Trigger gData_02001920[];
extern struct Trigger gData_020019f8[];
extern struct Trigger gData_02001a1c[];
extern struct Trigger gData_02001aac[];
extern struct Trigger gData_02001b54[];
extern struct Trigger gData_02001bcc[];

void Func_02000dcc(void);

void Func_02000f2c(void);

void Func_02000fd8(void);

void Func_0200102c(void);

extern s16 gData_020013c0[];
extern s16 gData_020013c4[];

extern u32 gData_02001264[];

extern s32 gData_020012f4[];
extern s16 gData_020013ca[];
extern s32 gData_0200121c[];

void Func_0200115c(s32 id);

s32 Field_FindNearbyTalkTarget(s32 owner);

s32 Func_020011fc(void);

void Func_02000a28(void);

void Func_02000c94(void);

void Func_020011bc(s32 a, s32 b, s32 c, s32 d);

#endif

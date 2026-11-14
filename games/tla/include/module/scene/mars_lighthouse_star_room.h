#ifndef ALCHEMY_MODULE_SCENE_MARS_LIGHTHOUSE_STAR_ROOM_H
#define ALCHEMY_MODULE_SCENE_MARS_LIGHTHOUSE_STAR_ROOM_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "field/cell_target.h"

struct Actor;
struct CellTarget;
struct CellTargetEntry;

void Leader_KnockBack(void);
void Actor_LaunchAhead(struct Actor *object);

void Func_020014f4(void);

void Beam_DrawTargets(void);

void Party_TakeFallDamage(void);

void Actor_RollAndCrumble(struct Actor *actor);

void Leader_StopIfPending(void);
void Actor_SteerByPad(struct Actor *object);

extern struct CellTarget gData_02004cf4;

void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);

void Func_02000060(void);

void Func_02000074(void);

void Func_02000188(void);

void Func_020001e8(void);

void Func_020005b0(void);

void MarsLighthouseStarRoom_HoldWarmHand(void);

void MarsLighthouseStarRoom_TakeMarsStar(void);

void MarsLighthouseStarRoom_UseMarsStar(void);

void Func_0200007c(void);

extern u32 gData_0200485c[];
extern u32 gData_02004898[];
extern u32 gData_020048d4[];

#endif

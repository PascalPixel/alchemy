#ifndef ALCHEMY_CANVAS_H
#define ALCHEMY_CANVAS_H

#include "types.h"
#include "battle_efx.h"

void BattleFx_BeginCanvasLayer(s32 bg_control);
void BattleFx_OpenCanvasLayer(s32 bg_control);
void BattleFx_BeginTiledCanvas(s32 bg_control);
void BattleFx_EndCanvasLayer(void);
void BattleFx_FetchRectangleBlitters(s32 alternate, BattleEffectDrawRectangle *output);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_SpawnObjects(s32 count, s32 resource, u32 variant);
void BattleFx_SelectLivingTargets(struct BattleEffectArgument *effect);

#endif

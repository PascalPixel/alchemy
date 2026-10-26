#ifndef ALCHEMY_UNIT_MENU_BATTLE_ARENA_H
#define ALCHEMY_UNIT_MENU_BATTLE_ARENA_H

#include "types.h"
#include "scene_tables.h"

extern const s32 gBattleArena_SlotValues[];
u8 *BattleArena_GetEntrances(void);
s32 Scene_GetExitRegions(void);
u8 *BattleArena_GetExits(void);
s32 BattleArena_SelectActorTemplates(void);
extern u32 gBattleArenaCallFrames;
u8 *BattleArena_GetTriggers(void);
s32 BattleArena_RunRoundResult(void);
extern u8 gBattleArena_SlotColumns[];
extern struct SceneEntrance gBattleArenaEntrances[];
extern u32 gBattleArenaExits[];
extern struct ActorTemplate gBattleArenaActorTemplates[];
extern struct ActorTemplate gBattleArenaBattleActorTemplates[];
extern struct Trigger gBattleArenaTriggers[];

#endif

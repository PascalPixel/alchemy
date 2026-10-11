#ifndef DEBUG_ITEM_LEVEL_LEVEL_H
#define DEBUG_ITEM_LEVEL_LEVEL_H

#include "types.h"
#include "owner_state.h"

#include "window.h"

extern struct EventWork *gEventWork;
extern u8				 gEffectWork[];

extern struct SceneEntrance	 gItemLevelEntrances[];
extern u32					 gItemLevelExits[];
extern struct ScenePlacement gItemLevelPlacements[];
extern struct SceneEvent	 gItemLevelEvents[];
extern char					 gItemLevelGlyphsUpper[28];
extern char					 gItemLevelGlyphsLower[28];
extern char					 gItemLevelGlyphsMarks[12];
extern char					 gItemLevelLvLabel[4];

void Engine_ActorSetAnimation();
void Engine_ActorShowEmote();
void Engine_EventBegin(void);
void Engine_EventEnd(void);
void Engine_ActorSetPosition(u32 actor, s32 x, s32 z);
void Engine_EventSetMessage();
void Engine_EventShowMessage();
void Engine_EventRequestExit();
void Engine_AudioPlayCue();
void Engine_TaskWait();

#endif

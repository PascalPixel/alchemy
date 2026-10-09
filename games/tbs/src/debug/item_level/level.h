#ifndef DEBUG_ITEM_LEVEL_LEVEL_H
#define DEBUG_ITEM_LEVEL_LEVEL_H

#include "types.h"
#include "owner_state.h"

#include "window.h"
#define UiWindow_Create ((s32 (*)())UiWindow_Create)

extern struct EventWork *gEventWork;
extern u8				 gEffectWork[];

extern u8 gItemLevelEntrances[];
extern u8 gItemLevelExits[];
extern u8 gItemLevelPlacements[];
extern u8 gItemLevelEvents[];
extern u8 gItemLevelGlyphsUpper[];
extern u8 gItemLevelGlyphsLower[];
extern u8 gItemLevelGlyphsMarks[];
extern u8 gItemLevelLvLabel[];

void UnitCalc();
s32	 Party_ListActiveOwners();
void Party_AdvanceOwnerCountToTarget();
u8	*Object_GetById();
void Engine_ActorSetAnimation();
void Engine_ActorShowEmote();
void Engine_EventBegin(void);
void Engine_EventEnd(void);
void Engine_ActorSetPosition(u32 actor, s32 x, s32 z);
void ObjectMotion_WaitForAnimationChange(u32 actor);
void Engine_EventSetMessage();
void Engine_EventShowMessage();
void Engine_EventRequestExit();
void UiWork_Finalize();
void UiText_DrawResource();
void UiText_DrawStringAtOffset();
void UiText_DrawStringInWindow();
void UiText_DrawNumberInWindow();
void RenderOutput_RedrawSavedRect();
void Engine_AudioPlayCue();
void Engine_TaskWait();
s32	 DebugMenu_BrowseIcons(void);
s32	 DebugMenu_BrowseEntryGlyphs(void);

u8	*ItemLevel_GetEntrances(void);
s32	 SceneData_ReturnZero(void);
u8	*ItemLevel_GetExits(void);
u8	*ItemLevel_GetPlacements(void);
void FieldScene_RunActor13Mode102Step(void);
void FieldScene_RunActor13Mode105Step(void);
u8	*ItemLevel_GetEvents(void);
void SceneState_AddToRecordCount(s32 id, s32 amount);
void Scene_AddToListedRecordCounts(s32 amount);
void FieldScene_RunCountAdjustPanel(void);
void SceneState_SetWorkWords1c0And1c8(void);
s32	 FieldScene_RunEntrySetup(void);
void FieldScene_DrawThreeCaptionWindow(void);
void ItemLevel_RunMotionTest(void);
void SceneState_SetRecordFlag53(void);
int	 SceneState_GetFarResult100c(void);
int	 SceneState_GetFarResult1020(void);

#endif

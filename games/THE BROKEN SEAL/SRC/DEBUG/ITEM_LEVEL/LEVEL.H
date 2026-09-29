#ifndef DEBUG_ITEM_LEVEL_LEVEL_H
#define DEBUG_ITEM_LEVEL_LEVEL_H

/* The item and level debug room (resource_3cd): the declarations its
 * sources share. */

#include "TYPES.H"

extern struct EventWork *gEventWork;
extern u8 gEffectWork[];

/* The room's scene tables and the glyph rows its caption window draws. */
extern u8 gItemLevelEntrances[];
extern u8 gItemLevelExits[];
extern u8 gItemLevelPlacements[];
extern u8 gItemLevelEvents[];
extern u8 gItemLevelGlyphsUpper[];
extern u8 gItemLevelGlyphsLower[];
extern u8 gItemLevelGlyphsMarks[];

/* Old-style declarations: the import veneers vary in arity between call
 * sites. */
u8 *Owner_GetState();
void Owner_RecalculateStats();
s32 Party_ListActiveOwners();
void Party_AdvanceOwnerCountToTarget();
u8 *Engine_ActorGet();
void Engine_ActorSetAnimation();
void Engine_ActorShowEmote();
void Engine_EventSetMessage();
void Engine_EventShowMessage();
void Engine_EventRequestExit();
s32 UiWindow_Create();
void UiText_DrawStringInWindow();
s32 DebugMenu_BrowseIcons(void);
s32 DebugMenu_BrowseEntryGlyphs(void);

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo shared with later uses in the block. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

u8 *ItemLevel_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *ItemLevel_GetExits(void);
u8 *ItemLevel_GetPlacements(void);
void FieldScene_RunActor13Mode102Step(void);
void FieldScene_RunActor13Mode105Step(void);
u8 *ItemLevel_GetEvents(void);
void SceneState_AddToRecordCount(s32 id, s32 amount);
void Scene_AddToListedRecordCounts(s32 amount);
void FieldScene_RunCountAdjustPanel(void);
void SceneState_SetWorkWords1c0And1c8(void);
s32 FieldScene_RunEntrySetup(void);
void FieldScene_DrawThreeCaptionWindow(void);
void SceneState_SetRecordFlag53(void);
int SceneState_GetFarResult100c(void);
int SceneState_GetFarResult1020(void);

#endif

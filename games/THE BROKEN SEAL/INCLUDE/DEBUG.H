#ifndef DEBUG_H
#define DEBUG_H

/* DEBUG */

#include "TYPES.H"
#include "EDITION.H"

struct ItemDefinition;
struct RenderInput;

extern const u8 Debug_PaletteSwatchTiles[];
extern u8 gDebugItemCapacityLabel[];
extern u8 gDebugItemFullLabel[];
extern u8 gDebugItemPrompt[];
extern u8 gItemLevelItemFull[];
extern u8 gItemLevelItemHelp[];
extern u8 gItemLevelItemPrompt[];
extern u8 gItemLevelPsyHelp[];
extern u8 gItemLevelPsyPrompt[];
#ifndef OWN_SideObject_ActorKindIdMap
extern s16 Portrait_SpriteMap[][2];
#endif
#ifndef OWN_SideObject_CharacterIdMap
extern s16 Portrait_CharacterMap[][2];
#endif
extern u8 TestRooms_Entrances[];
extern u8 TestRooms_Events[];
extern u8 TestRooms_Exits[];
extern u8 TestRooms_Placements[];
void BattleMap_ApplyEntranceView(void);
void Debug_RunPaletteEditor(void);
void DebugBattle_ViewMessages(void);
s32 Djinn_AddToOwnerFar(s32 owner, s32 element, s32 djinn);
void Engine_DebugClearWindow(struct RenderInput *window);
void Engine_DebugDrawItemDetails(struct RenderInput *window, s32 item);
u8 *Engine_DebugGetAbility(s32 ability);
struct ItemDefinition *Engine_DebugGetItem(s32 item);
void Engine_DebugRedrawWindow(struct RenderInput *window);
void GameState_InitDefaultsFar(void);
s32 Object_GetTriggerTileAheadOfCurrent(void);
void Party_AdvanceOwnerCountToTargetFar(s32 owner, s32 level);
void Ui_AdjustValueWithoutLimitFar(s32, u16 *);
void UiWork_DrainPendingFar(s32);

#endif

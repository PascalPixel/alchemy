#ifndef DEBUG_H
#define DEBUG_H

/* DEBUG */

#include "types.h"
#include "edition.h"
#include "farcall.h"

struct ItemDefinition;
struct RenderInput;

extern const u8		   Debug_PaletteSwatchTiles[];
extern char			   gDebugItemCapacityLabel[24];
extern char			   gDebugItemFullLabel[20];
extern char			   gDebugItemPrompt[12];
extern char gItemLevelItemFull[20];
extern char gItemLevelItemHelp[24];
extern char gItemLevelItemPrompt[12];
extern char gItemLevelPsyHelp[12];
extern char gItemLevelPsyPrompt[8];
extern const s16	   Portrait_SpriteMap[];	// 2 entries a row: key, id
extern const s16	   Portrait_CharacterMap[]; // 2 entries a row: key, id
void				   BattleMap_ApplyEntranceView(void);
void				   Debug_RunPaletteEditor(void);
void				   DebugBattle_ViewMessages(void);
void				   Engine_DebugClearWindow(struct RenderInput *window);
void				   Engine_DebugDrawItemDetails(struct RenderInput *window, s32 item);
u8					  *Engine_DebugGetAbility(s32 ability);
struct ItemDefinition *Engine_DebugGetItem(s32 item);
void				   Engine_DebugRedrawWindow(struct RenderInput *window);
s32					   Object_GetTriggerTileAheadOfCurrent(void);

#endif

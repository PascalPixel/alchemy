#ifndef ALCHEMY_UNIT_MENU_CLEAR_H
#define ALCHEMY_UNIT_MENU_CLEAR_H

#include "types.h"
#include "field_scene.h"

u8 *Clear_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *Clear_GetExits(void);
u8 *Clear_GetPlacements(void);
u8 *Clear_GetEvents(void);
extern const u16 gClear_CodeSequence[];
extern const u16 gClear_ExtraCodeSequence[];
s32 SaveMenu_Run(void);
extern struct SceneEntrance gClear_Entrances[];
extern u32 gClear_Exits[];
extern struct ScenePlacement gClear_Placements[];
extern struct SceneEvent gClear_Events[];
extern s16 gClear_BlendFrame;
extern s16 gClear_CodeUnlocked;
extern s16 gClear_ExtraCodeUnlocked;
extern s16 gClear_CodeProgress;
extern s16 gClear_ExtraCodeProgress;
extern s16 gClear_CodeHeld;
extern s32 gClear_ExtraCodeHeld;
extern u32 gClear_PasswordMembers[];
extern u32 gClear_PasswordFlags[];
extern u32 gClear_PasswordKeyItems[];
extern u16 gClear_PasswordStackItems[];

#endif

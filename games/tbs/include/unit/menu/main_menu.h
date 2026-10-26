#ifndef ALCHEMY_UNIT_MENU_MAIN_MENU_H
#define ALCHEMY_UNIT_MENU_MAIN_MENU_H

#include "types.h"
#include "map_tables.h"

u8 *MainMenu_GetEntrances(void);
s32 Scene_GetExitRegions(void);
u8 *MainMenu_GetExits(void);
u8 *MainMenu_GetActorTemplates(void);
u8 *MainMenu_GetTriggers(void);
extern const u16 gMainMenu_CodeSequence[];
extern const u16 gMainMenu_ExtraCodeSequence[];
s32 MainMenu_Run(void);
extern struct SceneEntrance gMainMenu_Entrances[];
extern u32 gMainMenu_Exits[];
extern struct ActorTemplate gMainMenu_ActorTemplates[];
extern struct Trigger gMainMenu_Triggers[];
extern s16 gMainMenu_BlendFrame;
extern s16 gMainMenu_CodeUnlocked;
extern s16 gMainMenu_ExtraCodeUnlocked;
extern s16 gMainMenu_CodeProgress;
extern s16 gMainMenu_ExtraCodeProgress;
extern s16 gMainMenu_CodeHeld;
extern s32 gMainMenu_ExtraCodeHeld;
extern u32 gMainMenu_PasswordMembers[];
extern u32 gMainMenu_PasswordFlags[];
extern u32 gMainMenu_PasswordKeyItems[];
extern u16 gMainMenu_PasswordStackItems[];

#endif

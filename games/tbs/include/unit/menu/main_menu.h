#ifndef ALCHEMY_UNIT_MENU_MAIN_MENU_H
#define ALCHEMY_UNIT_MENU_MAIN_MENU_H

#include "types.h"
#include "map_tables.h"

u8 *MainMenu_GetWarpDests(void);
s32 MapData_ReturnZero(void);
u8 *MainMenu_GetWarps(void);
u8 *MainMenu_GetObjectEventTemplates(void);
u8 *MainMenu_GetEvents(void);
extern const u16 gMainMenu_CodeSequence[];
extern const u16 gMainMenu_ExtraCodeSequence[];
s32 MainMenu_Run(void);
extern struct MapWarpDest gMainMenu_WarpDests[];
extern u32 gMainMenu_Warps[];
extern struct ObjectEventTemplate gMainMenu_ObjectEventTemplates[];
extern struct MapEvent gMainMenu_Events[];
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

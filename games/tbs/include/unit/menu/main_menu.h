#ifndef ALCHEMY_UNIT_MENU_MAIN_MENU_H
#define ALCHEMY_UNIT_MENU_MAIN_MENU_H

#include "types.h"
#include "scene_tables.h"

u8 *FileScreen_GetEntrances(void);
s32 Scene_GetExitRegions(void);
u8 *FileScreen_GetExits(void);
u8 *FileScreen_GetActorTemplates(void);
u8 *FileScreen_GetTriggers(void);
extern const u16 gFileScreen_CodeSequence[];
extern const u16 gFileScreen_ExtraCodeSequence[];
s32 FileScreen_Run(void);
extern struct SceneEntrance gFileScreen_Entrances[];
extern u32 gFileScreen_Exits[];
extern struct ActorTemplate gFileScreen_ActorTemplates[];
extern struct Trigger gFileScreen_Triggers[];
extern s16 gFileScreen_BlendFrame;
extern s16 gFileScreen_CodeUnlocked;
extern s16 gFileScreen_ExtraCodeUnlocked;
extern s16 gFileScreen_CodeProgress;
extern s16 gFileScreen_ExtraCodeProgress;
extern s16 gFileScreen_CodeHeld;
extern s32 gFileScreen_ExtraCodeHeld;
extern u32 gFileScreen_PasswordMembers[];
extern u32 gFileScreen_PasswordFlags[];
extern u32 gFileScreen_PasswordKeyItems[];
extern u16 gFileScreen_PasswordStackItems[];

#endif

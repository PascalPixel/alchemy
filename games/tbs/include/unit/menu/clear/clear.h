#ifndef ALCHEMY_UNIT_MENU_CLEAR_CLEAR_H
#define ALCHEMY_UNIT_MENU_CLEAR_CLEAR_H

#include "types.h"
#include "field_scene.h"

u8 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
u8 *SceneData_GetActorTable(void);
u8 *SceneData_GetEffectTable(void);
extern const u16 Clear_CodeSequence[];
extern const u16 Clear_ExtraCodeSequence[];
s32 SaveMenu_Run(void);
extern struct SceneEntrance Clear_ScriptTable[];
extern u32 Clear_MessageTable[];
extern struct ScenePlacement Clear_ActorTable[];
extern struct SceneEvent Clear_EffectTable[];
extern s16 Clear_BlendFrame;
extern s16 Clear_CodeUnlocked;
extern s16 Clear_ExtraCodeUnlocked;
extern s16 Clear_CodeProgress;
extern s16 Clear_ExtraCodeProgress;
extern s16 Clear_CodeHeld;
extern s32 Clear_ExtraCodeHeld;
extern u32 Clear_PasswordMembers[];
extern u32 Clear_PasswordFlags[];
extern u32 Clear_PasswordKeyItems[];
extern u16 Clear_PasswordStackItems[];

#endif

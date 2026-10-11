#ifndef ALCHEMY_UNIT_MENU_CLEAR_CLEAR_H
#define ALCHEMY_UNIT_MENU_CLEAR_CLEAR_H

#include "types.h"

struct RenderInput;

u8 *SceneData_GetScriptTable(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
u8 *SceneData_GetActorTable(void);
u8 *SceneData_GetEffectTable(void);
void Clear_LoadBackground(void);
void Clear_UpdateBlend(void);
void Clear_CheckButtonCodes(void);
void Clear_NameEntryCharacter(s32 cell, u8 *text);
s32 Scene_GetModeMask(void);
s32 SaveMenu_Run(void);
void SaveMenu_FillTileGrid(struct RenderInput *window, s32 value);
void SaveMenu_FillTileBlock(const struct RenderInput *window);
s32 Clear_PackPasswordData(s32 unused, s32 mode, u8 *out);
s32 Clear_EncodePassword(u8 *data, s32 size, u8 *out);
u16 SceneData_GetBufferCrc16(u32 count, u8 *p);

#endif

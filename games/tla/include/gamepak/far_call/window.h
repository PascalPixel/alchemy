#ifndef ALCHEMY_FAR_CALL_WINDOW_H
#define ALCHEMY_FAR_CALL_WINDOW_H

/// @file
/// The far calls into the window bank, as the other banks' code calls them.

#include "types.h"

struct BattleActionRecord;
struct RenderInput;
struct WindowSprite;
void WindowSystem_InitBattleFar(s32 mode);
void PartyStatus_CreateWindowFar(s32 layout);
s32 Message_AllTypedFar(void);
s32 WindowSprite_GetPointerTilesFar(s32 index);
void WindowSystem_InitFieldFar(void);
struct RenderInput *Window_CreateFar(s32 x, s32 y, s32 width, s32 height,
                                     s32 mode);
void Window_CloseFar(struct RenderInput *window, s32 mode);
void Text_DrawMessageAsTilesFar(s32 message, struct RenderInput *window, s32 x,
                                s32 y);
void Message_CloseFinishedWindowsFar(void);
s32 YesNoMenu_RunFar(s32 useBottom, s32 name, s32 width, s32 third);
s32 Resource_LoadFixedBlockBIntoFreeSlotFar(void);
void WindowSprite_PrepareForRedrawFar(void *window);
void FieldMessage_ShowFar(s32 message, s32 flags);
void DebugFlagEditor_RunFar(void);
void FieldMenu_RunFar(void);
void DebugSceneSelect_RunFar(void);
void PartyStatus_CloseFar(void);
void Message_SetDecodeFlagsFar(s32 flags);
void ScanlineWindow_FillSpansFar(u16 *pairs);
void Text_GetMessageSizeAltFar(s32 message, s32 *left, s32 *top, u32 *width,
                               u32 *height);
void Text_GetMessageSizeFar(s32 message, s32 *left, s32 *top, u32 *width,
                            u32 *height);
s32 Portrait_GetForSpeakerFar(s32 sprite);
void Message_DecodeRawFar(s32 message, u16 *buffer, s32 length);
struct WindowSprite *WindowSprite_CreateLoadedFar(s32 item, s32 mode,
                                                  struct RenderInput *window,
                                                  s32 x, s32 y);
s32 Menu_SelectShopCommandFar(s32 action);
s32 Menu_SelectSanctumServiceFar(s32 prev);
void Text_MeasureMessageFar(s32 message, s32 *width, s32 *height);
void WindowColor_ApplyFar(s32 first, s32 second);
s32 Battle_CollectPartyCommandsFar(struct BattleActionRecord *entries,
                                   void *buffer, s32 mode);
void AreaNameWindow_ShowFar(s32 scene);
void PauseMenu_RunFar(s32 mode);
void Func_08038340(s32 value);

#endif

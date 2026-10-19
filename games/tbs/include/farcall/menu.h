/// @file
/// Party menus and their interactive selection screens.
/// Far-call interfaces; their ordered jump stubs live in
/// games/tbs/src/system/far_call/MENU.S.

/// Far call to ItemMenu_Run.
s32 ItemMenu_RunFar(void);
/// Far call to PsynergyMenu_Run.
s32 PsynergyMenu_RunFar(void);
/// Far call to StatusMenu_Run.
s32 StatusMenu_RunFar(void);
/// Far call to Menu_OpenCharacterSelector.
s32 Menu_OpenCharacterSelectorFar(void);
/// Far call to PsynergyMenu_InitializeEntryObjects.
void PsynergyMenu_InitializeEntryObjectsFar(void *source, s32 originX,
                                            s32 originY, s32 spacing,
                                            s32 unused);
/// Far call to Menu_ReleaseEntryObjects.
void Menu_ReleaseEntryObjectsFar(void);
/// Far call to UiIcon_PrepareObject.
void UiIcon_PrepareObjectFar(struct RenderOutput *object);
/// Far call to DjinnMenu_Run.
s32 DjinnMenu_RunFar(void);
/// Far call to ItemMenu_DrawItemDetails.
void ItemMenu_DrawItemDetailsFar(s32 window, s32 item);

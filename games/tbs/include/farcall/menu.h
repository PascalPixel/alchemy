// Party menus and their interactive selection screens.
// Far-call interfaces; their ordered jump stubs live in
// games/tbs/src/system/far_call/MENU.S.
s32 ItemMenu_RunFar(void);
s32 PsynergyMenu_RunFar(void);
s32 StatusMenu_RunFar(void);
s32 Menu_OpenCharacterSelectorFar(void);
void PsynergyMenu_InitializeEntryObjectsFar(void *source, s32 originX,
                                            s32 originY, s32 spacing,
                                            s32 unused);
void Menu_ReleaseEntryObjectsFar(void);
void UiIcon_PrepareObjectFar(struct RenderOutput *object);
s32 DjinnMenu_RunFar(void);
void ItemMenu_DrawItemDetailsFar(s32 window, s32 item);

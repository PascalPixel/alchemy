/// @file
/// Party menus and their interactive selection screens.

s32 ItemMenu_RunFar(void);
s32 PsynergyMenu_RunFar(void);
s32 StatusScreen_RunFar(void);
s32 PartyMemberChooser_OpenFar(void);
void Menu_CreatePartyFiguresFar(void *source, s32 originX, s32 originY,
                                s32 spacing, s32 unused);
void Menu_ReleasePartyFiguresFar(void);
void Icon_PrepareActorFar(struct WindowSprite *object);
s32 DjinnMenu_RunFar(void);
void ItemMenu_DrawItemDetailsFar(s32 window, s32 item);

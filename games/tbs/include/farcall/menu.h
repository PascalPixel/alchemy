/// @file
/// Party menus and their interactive selection screens.

/// Runs the field menu's Item screen. Returns 1 when an item is to be used on
/// the field, -1 when the player backs out.
s32 ItemMenu_RunFar(void);
/// Runs the field menu's Psynergy screen. Returns 1 when the player chose
/// Psynergy to use on the field, -1 when they back out.
s32 PsynergyMenu_RunFar(void);
/// Runs the field menu's Status screen, which puts back the palettes and window
/// tiles it draws over. -1 reopens the field menu.
s32 StatusMenu_RunFar(void);
/// Lets the player pick one party member over the field, as Colosso does.
/// Returns the member's id, or -1 when the player backs out.
s32 Menu_OpenCharacterSelectorFar(void);
/// Sets out a walking figure for each active party member under a window's
/// corner; the shops reach it here.
void PsynergyMenu_InitializeEntryObjectsFar(void *source, s32 originX,
                                            s32 originY, s32 spacing,
                                            s32 unused);
/// Frees the party figures set out for a menu and stops their placement
/// callback.
void Menu_ReleaseEntryObjectsFar(void);
/// Shows a menu icon at its stored position, unrotated and unscaled.
void UiIcon_PrepareObjectFar(struct RenderOutput *object);
/// Runs the Djinn screen and the Djinn tutorial. Returns 1, which makes the
/// field menu open again.
s32 DjinnMenu_RunFar(void);
/// Writes an item's description into a window, a line a row: its bonuses,
/// effects, curse and whether it breaks.
void ItemMenu_DrawItemDetailsFar(s32 window, s32 item);

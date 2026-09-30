#include "TYPES.H"
#include "ITEM_MENU.H"
#include "BATTLE_TYPES.H"

extern u8 gMenuWork[];

/* The panel loads the "Exp" label from the literal pool and reaches the four
   stat labels, which sit 23 messages before it, by subtracting; the
   class-name base is loaded from the pool too. */
extern u8 MsgExpLabel[];
extern u8 MsgClassName;

/* Fixed labels drawn with the panel. */
extern const u8 Menu_LvString[];
extern const u8 Data_080af230[];
extern const u8 Data_080af234[];
extern const u8 Data_080af238[];

void UiWindow_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width, s32 height);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiText_DrawStringAtOffsetFar(const void *text, s32 window, s32 x, s32 y);
void UiText_DrawStringInWindowFar(const void *text, s32 window, s32 x, s32 y);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x, s32 y);
void UiWork_SetParamNibbleFar(s32 color);
struct BattleUnit *Owner_GetStateFar(s32 owner);
void WaitFrames(s32 frames);

/* The Japanese panel gives its four battle stats a fourth digit, a column
   further left. */
#if defined(TBS_EDITION_JA)
#define STAT_DIGITS 4
#define STAT_X      192
#else
#define STAT_DIGITS 3
#define STAT_X      200
#endif


/* The owner's status panel: name, class, level, HP and PP against their
   maxima and experience on the left, the four battle stats on the right.
   Bit 8 of flags skips clearing the panel first. */
void ItemMenu_DrawOwnerStatus(s32 window, s32 owner, s32 flags)
{
    struct ItemMenuState *menu;
    struct BattleUnit *unit;

    menu = *(struct ItemMenuState **)gMenuWork;
    unit = Owner_GetStateFar(owner);
    menu->cursor->state = 1;
    flags &= 0x100;
    if (flags == 0) {
        UiWindow_ClearInteriorTilesFar(window, 0, 0, 128, 40);
    }
    UiText_DrawStringAtOffsetFar(unit->name, window, 40, 0);
    UiText_DrawCharacterAtOffsetFar(unit->class_index + (s32)&MsgClassName, window, 0, 32);
#if defined(TBS_EDITION_JA)
    UiText_DrawStringInWindowFar(Menu_LvString, window, 104, 0);
#else
    UiText_DrawStringAtOffsetFar(Menu_LvString, window, 104, 0);
#endif
    UiWork_SetParamNibbleFar(15);
    UiText_DrawNumberInWindowFar(unit->level, 2, window, 128, 0);
#if defined(TBS_EDITION_JA)
    UiText_DrawStringInWindowFar(Data_080af234, window, 40, 16);
#else
    UiText_DrawStringAtOffsetFar(Data_080af234, window, 40, 16);
#endif
    UiText_DrawNumberInWindowFar(unit->hp, 4, window, 72, 16);
    UiText_DrawNumberInWindowFar(unit->max_hp, 4, window, 112, 16);
    UiText_DrawStringInWindowFar(Data_080af230, window, 104, 16);
#if defined(TBS_EDITION_JA)
    UiText_DrawStringInWindowFar(Data_080af238, window, 40, 24);
#else
    UiText_DrawStringAtOffsetFar(Data_080af238, window, 40, 24);
#endif
    UiText_DrawNumberInWindowFar(unit->pp, 4, window, 72, 24);
    UiText_DrawNumberInWindowFar(unit->max_pp, 4, window, 112, 24);
    UiText_DrawStringInWindowFar(Data_080af230, window, 104, 24);
    UiText_DrawCharacterAtOffsetFar((s32)MsgExpLabel, window, 40, 8);
    UiText_DrawNumberInWindowFar(unit->experience, 7, window, 88, 8);
    if (flags == 0) {
        WaitFrames(1);
        UiWindow_ClearInteriorTilesFar(window, 144, 0, 224, 40);
    }
    UiText_DrawCharacterAtOffsetFar((s32)MsgExpLabel - 23, window, 152, 0);
    UiText_DrawCharacterAtOffsetFar((s32)MsgExpLabel - 22, window, 152, 8);
    UiText_DrawCharacterAtOffsetFar((s32)MsgExpLabel - 21, window, 152, 16);
    UiText_DrawCharacterAtOffsetFar((s32)MsgExpLabel - 20, window, 152, 24);
    UiText_DrawNumberInWindowFar(unit->attack, STAT_DIGITS, window, STAT_X, 0);
    UiText_DrawNumberInWindowFar(unit->defense, STAT_DIGITS, window, STAT_X, 8);
    UiText_DrawNumberInWindowFar(unit->agility, STAT_DIGITS, window, STAT_X, 16);
    UiText_DrawNumberInWindowFar(unit->luck, STAT_DIGITS, window, STAT_X, 24);
}

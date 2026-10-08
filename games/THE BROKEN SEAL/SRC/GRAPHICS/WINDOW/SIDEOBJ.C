#include "TYPES.H"
/* FAKEMATCH: needed to match: this file reads Portrait_CharacterMap through its own declaration; the shop's changes its code. */
#define OWN_SideObject_CharacterIdMap
extern const s16 Portrait_CharacterMap[];
/* FAKEMATCH: needed to match: this file reads Portrait_SpriteMap through its own declaration; the shop's changes its code. */
#define OWN_SideObject_ActorKindIdMap
extern const s16 Portrait_SpriteMap[];
#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "EDITION.H"
#include "TYPES.H"
#include "WINDOW.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"



extern s32 Localization_LookupEntryId();
extern s32 CreateSideObject();

void UiWork_SetTwoEntriesTo999(void)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    s32 index = 0;

    do {
        work->resource_ids[index] = 999;
        index++;
    } while (index != 2);
}

s32 Localization_LookupEntryId(u32 value)
{
    s32 result = -1;
    s32 i = 0;

    if (value < 20) {
        for (;;) {
            s32 key = Portrait_CharacterMap[i];

            if (key == -1)
                break;
            if (key == value) {
                i++;
                result = Portrait_CharacterMap[i];
                break;
            }
            i += 2;
        }
    } else {
        for (;;) {
            s32 key = Portrait_SpriteMap[i];

            if (key == -1)
                break;
            if (key == value) {
                i++;
                result = Portrait_SpriteMap[i];
                result += 128;
                break;
            }
            i += 2;
        }
    }
    return result;
}

s32 UiWindow_CreateWithSideObject(s32 arg0, s32 arg1, s32 x, s32 y)
{
    s32 win;
    s32 minus_four;
    s32 ofs;
    void *work;

    work = gWindowWork[0];
    if (Localization_LookupEntryId(arg0) == -1) {
        return 0;
    }
    minus_four = -4;
    ofs = minus_four;
    if (((struct UiRenderWork *)work)->mode != 0) {
        win = (s32)UiWindow_Create(x, y, 6, 5, 2);
        ofs = 0;
    } else {
        win = (s32)UiWindow_Create(x, y, 5, 5, 2);
    }
    if (win != 0) {
        CreateSideObject(arg0, arg1, -1, win, ofs, minus_four);
    }
    return win;
}

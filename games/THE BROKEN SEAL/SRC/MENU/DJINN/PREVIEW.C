#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "BATTLE_UNIT.H"
#include "RENDER_INPUT.H"
#include "IWRAM_CALL.H"
#include "EDITION.H"
#include "DJINN_PREVIEW.H"
#include "BATTLE_RUNTIME.H"
#include "UIFLOW.H"
#include "text/MSG_IDS.H"

#define UiWindow_Create ((struct RenderInput * (*)(s32, s32, s32, s32, s32))UiWindow_Create)
#define UiText_DrawCharacterAtOffset ((void (*)(s32, struct RenderInput *, s32, s32))UiText_DrawCharacterAtOffset)

#define UiWindow_SetTilemapEntry ((void (*)(struct RenderInput*,s32,s32,s32,s32))UiWindow_SetTilemapEntry)
#define UiWindow_DrawThreeTileColumn ((s32 (*)(struct RenderInput*,s32,s32,s32,s32))UiWindow_DrawThreeTileColumn)

#define Djinn_DeactivateFar ((void (*)(s32,s32,s32))Djinn_DeactivateFar)
#define Ability_GetData ((struct AbilityData * (*)(s32))Ability_GetData)

struct AbilityData {
    u8 field_00[2];
    u8 kind;
    u8 field_03[5];
    u8 element;
    u8 cost;
};

struct BattleUnitRecord {
    u8 field_00[0x38];
    s16 hp;
    s16 pp;
    u16 atk;
    u16 def;
    u16 agi;
    u8 luk;
    u8 field_43[0x58 - 0x43];
    u8 grid[0x129 - 0x58];
    u8 element;
};

s32 UiText_RenderWideStringInWindow(s16 *text, struct RenderInput *win, s32 x, s32 y);
void UiWindow_DrawDividerLine(struct RenderInput *win, s32 x, s32 y, s32 w, s32 h);
void UiWork_SetParamNibble(s32 nibble);
s32 UiText_FormatNumberToHalfwords(s16 *out, s32 value);
s32 DjinnMenu_ListChangedDjinn(const struct OwnerActionSlot *oldGrid, const struct OwnerActionSlot *newGrid, u16 *out, s32 *gained, s32 *lost);
void Djinn_ActivateFar(s32 owner, s32 col, s32 row);

s32 __divsi3(s32 numerator, s32 denominator);
void UiText_DrawNumberInWindow(s32 value, s32 digits, struct RenderInput *win, s32 x, s32 y);

struct RenderInput *DjinnMenu_ShowChangePreview(
    struct RenderInput *win, s32 owner, s32 code, s32 page, s32 *pageCount)
{
    struct BattleUnitRecord *unit;
    struct BattleUnitRecord *snap;
    struct AbilityData *info;
    s16 *buf;
    s32 oldHp;
    s32 oldPp;
    s32 oldAtk;
    s32 oldDef;
    s32 oldAgi;
    u16 *list;
    s32 total;
    s32 n;
    s32 marks;
    s32 rows;
    s32 col;
    s32 row;
    s32 pages;
    s32 gained;
    s32 lost;
    s32 tile;
    s32 val;
    s32 i;

    void DjinnMenu_DrawStatArrow(s32 x, s32 y, s32 rising)
    {
        struct RenderOutput *output = RenderOutput_AcquireFree();
        struct PreviewSprite *entry;
        u32 zero;
        if (output) {
            output->active = 1;
            output->kind = 1;
            /* FAKEMATCH: a char store (alias set 0) keeps the index store ahead of the attribute word in sched2 */
            *(s8 *)&output->index = Resource_LoadIntoFreeSlot(128);
            entry = (struct PreviewSprite *)((u8 *)output + 16);
            output->sentinel = 240;
            output->x = 120;
            output->y = 120;
            /* FAKEMATCH: typed like the window pointer, the word store waits for the window load but not for the x and y stores, so sched2 places the entry copy before them */
            *(struct RenderInput **)&entry->attributes = (struct RenderInput *)0x40000400;
            zero = 0;
            /* FAKEMATCH: a fresh zero keeps the window chain in r9 */
            __asm__("" : "=r"(zero) : "0"(zero));
            /* FAKEMATCH: a pointer-typed clear does not wait for the x and y stores (sched2 order) */
            *(void **)&entry->tile = (void *)zero;
            entry->attributes.bits.x = win->x * 8 + x;
            entry->attributes.bits.y = win->y * 8 + y;
            entry->tile.bits.index = Resource_GetBuffer((u8)output->index, (s32)(rising ? DjinnMenu_ArrowUpTiles : DjinnMenu_ArrowDownTiles));
            RenderOutput_AppendToList((struct RenderOutputList *)win, output);
        }
    }

    unit = Owner_GetStateFar(owner);
    if (win != 0)
        UiWork_Finalize(win, 1);

    if (page == 0)
        win = UiWindow_Create(0, 8, 21, 11, 6);
    else
        win = UiWindow_Create(0, 5, 21, 14, 6);
    if (win == 0)
        return 0;

    buf = (s16 *)Runtime_BumpAllocate(128);
    snap = (struct BattleUnitRecord *)Runtime_BumpAllocate(332);
    list = (u16 *)Runtime_BumpAllocateAlternatePool(96);
    Iwram_CopyWords(snap, unit, 332);

    col = (code >> 8) & 15;
    row = code & 255;
    if (Djinn_IsActiveFar(owner, col, row) != 0)
        Djinn_DeactivateFar(owner, col, row);
    else
        Djinn_ActivateFar(owner, col, row);
    Owner_RecalculateStatsFar(owner);

    total = DjinnMenu_ListChangedDjinn((const struct OwnerActionSlot *)snap->grid, (const struct OwnerActionSlot *)unit->grid, list, &gained, &lost);
    pages = __divsi3(total - 1, 5) + 1;
    *pageCount = pages;
    if (page * 5 - 5 >= total)
        page = pages;

    if (page == 0) {
        s32 label = (s32)MsgPreviewStatLabel;

        UiText_DrawCharacterAtOffset(label, win, 0, 8);
        UiText_DrawCharacterAtOffset(label + 1, win, 0, 16);
        UiText_DrawCharacterAtOffset(label + 2, win, 0, 24);
        UiText_DrawCharacterAtOffset(label + 3, win, 0, 32);
        UiText_DrawCharacterAtOffset(label + 4, win, 0, 40);
        UiText_DrawCharacterAtOffset(label + 5, win, 0, 48);

        oldHp = snap->hp;
        UiText_FormatNumberToHalfwords(buf, oldHp);
#if !EDITION_INTERNATIONAL
        UiText_RenderWideStringInWindow(buf + 7, win, 4, 1);
#else
        UiText_RenderWideStringInWindow(buf + 7, win, 5, 1);
#endif
        oldPp = snap->pp;
        UiText_FormatNumberToHalfwords(buf, oldPp);
#if !EDITION_INTERNATIONAL
        UiText_RenderWideStringInWindow(buf + 7, win, 4, 2);
#else
        UiText_RenderWideStringInWindow(buf + 7, win, 5, 2);
#endif
        oldAtk = snap->atk;
        UiText_FormatNumberToHalfwords(buf, oldAtk);
#if !EDITION_INTERNATIONAL
        UiText_RenderWideStringInWindow(buf + 7, win, 4, 3);
#else
        UiText_RenderWideStringInWindow(buf + 8, win, 6, 3);
#endif
        oldDef = snap->def;
        UiText_FormatNumberToHalfwords(buf, oldDef);
#if !EDITION_INTERNATIONAL
        UiText_RenderWideStringInWindow(buf + 7, win, 4, 4);
#else
        UiText_RenderWideStringInWindow(buf + 8, win, 6, 4);
#endif
        oldAgi = snap->agi;
        UiText_FormatNumberToHalfwords(buf, oldAgi);
#if !EDITION_INTERNATIONAL
        UiText_RenderWideStringInWindow(buf + 7, win, 4, 5);
#else
        UiText_RenderWideStringInWindow(buf + 8, win, 6, 5);
#endif
        UiText_FormatNumberToHalfwords(buf, snap->luk);
        UiText_RenderWideStringInWindow(buf + 8, win, 5, 6);

        UiWindow_DrawDividerLine(win, 0, 8, 19, 8);
        if (gained != 0 || lost != 0)
            UiWork_SetParamNibble(2);
#if !EDITION_INTERNATIONAL
        UiText_DrawCharacterAtOffset((s32)MsgBattleAdditionalPsynergyHelp, win, 40, 64);
#else
        UiText_DrawCharacterAtOffset((s32)MsgBattleAdditionalPsynergyHelp, win, 24, 64);
#endif
        UiWork_SetParamNibble(15);
    }

    if (page > 0) {
        marks = 0;
        rows = total;
        if (total > 4)
            rows = 5;
        i = page * 5 - 5;
        for (n = 0; n < rows && i < total; n++, i++) {
            info = Ability_GetData(list[i]);
            if (info->kind != 4)
                UiWindow_SetTilemapEntry(win, 0x5001 + info->kind, 15, n * 2, 0);

            tile = info->element;
            if (tile == 255)
                tile = 11;
            else
                tile--;
            UiWindow_DrawThreeTileColumn(win, 16, n * 2, tile, 0);

            Ui_CreateOutputFromResourceSlot(win, 0, n * 16 - 4, list[i] & 0x3fff);
            if ((list[i] & 0x8000) != 0)
                UiWork_SetParamNibble(4);
            else if ((list[i] & 0x4000) != 0)
                UiWork_SetParamNibble(2);
            else
                UiWork_SetParamNibble(15);

            UiText_DrawCharacterAtOffset((list[i] & 0x3fff) + (s32)MsgAbilityName, win, 16, n * 16);
            UiWindow_SetTilemapEntry(win, 0xf01f, 11, n * 2, 0);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
            UiWindow_SetTilemapEntry(win, 0xf005, 12, n * 2, 0);
#else
            UiWindow_SetTilemapEntry(win, 0xf01e, 12, n * 2, 0);
#endif
            UiText_DrawNumberInWindow(
                Ability_GetData(list[i])->cost, 2, win, 104, n * 16);
        }

        if (gained != 0) {
            UiWork_SetParamNibble(4);
            UiText_DrawCharacterAtOffset((s32)MsgPsynergyGained, win, 32, 80);
            marks = 1;
        }
        if (lost != 0) {
            UiWork_SetParamNibble(2);
            UiText_DrawCharacterAtOffset((s32)MsgPsynergyLost, win, 32, marks * 8 + 80);
            marks++;
        }
        if (marks == 0)
            UiText_DrawCharacterAtOffset((s32)MsgPsynergyUnchanged, win, 32, 80);
        UiWork_SetParamNibble(15);
        UiWork_SetParamNibble(15);
        UiWindow_DrawDividerLine(win, 0, 10, 19, 10);
    }

    if (page == 0) {
        UiText_DrawCharacterAtOffset((s32)MsgClassName + snap->element, win, 0, 0);
        UiText_DrawCharacterAtOffset((s32)MsgClassName + unit->element, win, 80, 0);
        if (snap->element != unit->element)
            UiWindow_SetTilemapEntry(win, 0xf728, 9, 0, page);
        else
            UiWindow_SetTilemapEntry(win, 0xf729, 9, 0, page);

        UiText_FormatNumberToHalfwords(buf, unit->hp);
        UiText_RenderWideStringInWindow(buf + 7, win, 11, 1);
        if (unit->hp != oldHp) {
            val = 0;
            if (unit->hp > oldHp)
                val = 1;
            DjinnMenu_DrawStatArrow(80, 14, val);
        }

        UiText_FormatNumberToHalfwords(buf, unit->pp);
        UiText_RenderWideStringInWindow(buf + 7, win, 11, 2);
        if (unit->pp != oldPp) {
            val = 0;
            if (unit->pp > oldPp)
                val = 1;
            DjinnMenu_DrawStatArrow(80, 22, val);
        }

        UiText_FormatNumberToHalfwords(buf, unit->atk);
        UiText_RenderWideStringInWindow(buf + 7, win, 11, 3);
        if (unit->atk != oldAtk) {
            val = 0;
            if (unit->atk > oldAtk)
                val = 1;
            DjinnMenu_DrawStatArrow(80, 30, val);
        }

        UiText_FormatNumberToHalfwords(buf, unit->def);
        UiText_RenderWideStringInWindow(buf + 7, win, 11, 4);
        if (unit->def != oldDef) {
            val = 0;
            if (unit->def > oldDef)
                val = 1;
            DjinnMenu_DrawStatArrow(80, 38, val);
        }

        UiText_FormatNumberToHalfwords(buf, unit->agi);
        UiText_RenderWideStringInWindow(buf + 7, win, 11, 5);
        if (unit->agi != oldAgi) {
            val = 0;
            if (unit->agi > oldAgi)
                val = 1;
            DjinnMenu_DrawStatArrow(80, 46, val);
        }

        UiText_FormatNumberToHalfwords(buf, unit->luk);
        UiText_RenderWideStringInWindow(buf + 8, win, 12, 6);
        if (unit->luk != snap->luk) {
            val = 0;
            if (unit->luk > snap->luk)
                val = 1;
            DjinnMenu_DrawStatArrow(80, 54, val);
        }
    }

    Iwram_CopyWords(unit, snap, 332);
    Runtime_BumpFree(list);
    Runtime_BumpFree(snap);
    Runtime_BumpFree(buf);
    return win;
}

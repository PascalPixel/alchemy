#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "BATTLE_UNIT.H"
/* 2026-10-04 sun finisher 4 (second wave): 2 off as written now (the
 * fresh-zero clear through the entry pointer is in the source). The one
 * remaining difference is sched2 placing the reload copy "adds r7, r6, #0"
 * (insn 329) after the x/y stores instead of between "movs r3, #120" and
 * the x store. -fsched-verbose=9 gives the dependence table: x store prio
 * 20 with deps {tile clear, word store, word constant load (anti on r3)},
 * the copy prio 20 with one dep, so the x store wins on dependants. The ROM
 * order needs prio(copy) = prio(x store) + 1 = prio(movs r3, #120); the x
 * store's prio is the word constant load's (2 + word store) through the r3
 * anti-dependence, the copy's is 2 + max(word store, clear), so the clear
 * would need one more priority than the word store (both 18 now, each from
 * its dependence on the static-chain load of win). Searched without effect:
 * all orders of entry/sentinel/x/y/word/clear with the word and the clear
 * as union member, *(u32 *) or *(unsigned long *) and x/y as field or
 * *(s16 *) (u32/long word casts drop the word store's dependence on the win
 * load and pull the copy up to the call); the index store split from its
 * call at every position; "+r" lifetime asms on entry or output and empty
 * barriers at every line of the block; x and y as a chained assignment or
 * through an s16 pointer. */
/* 2026-10-04 sun finisher 8: DjinnMenu_ShowChangePreview (main:08022b44)
 * with its nested arrow renderer DjinnMenu_DrawStatArrow (main:08022a7c),
 * one object, 1788 bytes (Japanese 1784). As written: 6 instructions off in
 * the nested function only, the parent exact in all six TBS editions
 * (Japanese stat columns at x 4/5 and the separator at 40; Spanish, French
 * and Italian label tile 0xf005).
 * Found: the stat labels as label = (s32)MsgPreviewStatLabel; label + k
 * (the DJINN.C pattern; CSE keeps the adds from one base), the real message
 * names (MsgAbilityName, MsgClassName, MsgPsynergyGained/Lost/Unchanged,
 * MsgBattleAdditionalPsynergyHelp) and Iwram_CopyWords replace the old
 * stand-in symbols; sentinel, x, y and then the attribute word in that order.
 * Remaining: the tile clear. The ROM clears it through the entry pointer
 * after the attribute word (str r5, [r7, #8], zero materialised late in r5).
 * Written that way the clear reuses the zero of the s8 field stores' bit-field
 * expansion (all s8 struct-field stores expand as read-modify-write here),
 * global gives it r8 and x/chain move to r9/r5 (20). A tagged empty asm on a
 * fresh zero fixes that (2 off): then only sched2 differs, placing the
 * "adds r7, r6, #0" half of entry = output + 16 after the x/y stores: both
 * have priority 20 and the x store wins the tie on dependants (it has four:
 * the union word store and tile clear are alias set 0). Tried: every order
 * of entry/clear/sentinel/x/y/word (both clear forms, with active and/or kind
 * through u8 casts), field/u16/s16/u32 casts of x, y, word and clear, a
 * separate bit-field struct, entry split across a barrier, a slot local. */
#include "RENDER_INPUT.H"
#include "IWRAM_CALL.H"
#include "EDITION.H"

/* Preview setting or resting one Djinn: toggle it on the live owner record,
 * recalculate, show the old and new class and stats with an arrow beside
 * each change (page 0) or the Psynergy gained and lost (later pages), then
 * restore the record from the snapshot. The arrow renderer is a GNU C nested
 * function (main:08022a7c) that reaches the window through the static chain.
 *
 * UiText_RenderWideStringInWindow and UiWindow_DrawThreeTileColumn are
 * declared returning a value so r0 is set last at each call. The locals are
 * declared in the reference's spill-slot order. */


/* Ability table entry returned by Ability_GetData. */
struct AbilityData {
    u8 field_00[2];
    u8 kind;    /* 0x02, 4 means "no category icon" */
    u8 field_03[5];
    u8 element; /* 0x08, 0xff means "none" */
    u8 cost;    /* 0x09 */
};

/* Party unit record returned by Runtime_GetObject.  Only the cells this
 * screen reads are named; the copied extent is 332 bytes. */
struct BattleUnitRecord {
    u8 field_00[0x38];
    s16 hp;                    /* 0x38 */
    s16 pp;                    /* 0x3a */
    u16 atk;                   /* 0x3c */
    u16 def;                   /* 0x3e */
    u16 agi;                   /* 0x40 */
    u8 luk;                    /* 0x42 */
    u8 field_43[0x58 - 0x43];
    u8 grid[0x129 - 0x58];     /* 0x58, compared entry by entry */
    u8 element;                /* 0x129 */
};

/* Callees the owner register already names, spelled the way types.h spells
 * its own aliases: the address symbol stays the ABI. */

s32 UiText_RenderWideStringInWindow(s16 *text, struct RenderInput *win, s32 x, s32 y);
void UiWindow_DrawDividerLine(struct RenderInput *win, s32 x, s32 y, s32 w, s32 h);
void UiWork_SetParamNibble(s32 nibble);
void UiWindow_SetTilemapEntry(
    struct RenderInput *win, s32 tile, s32 x, s32 y, s32 flags);
s32 UiWindow_DrawThreeTileColumn(
    struct RenderInput *win, s32 x, s32 y, s32 tile, s32 flags);
struct PreviewSprite {
    u32 unknown;
    union {
        u32 word;
        struct { unsigned y:8; unsigned flags:8; unsigned x:9; unsigned high:7; } bits;
    } attributes;
    union RenderTableValue tile;
};
struct RenderOutput *RenderOutput_AcquireFree(void);
void RenderOutput_AppendToList(struct RenderInput *, struct RenderOutput *);
extern u8 MsgAbilityName[];
extern u8 MsgPreviewStatLabel[];
extern u8 MsgBattleAdditionalPsynergyHelp[];
extern u8 MsgPsynergyGained[];
extern u8 MsgPsynergyLost[];
extern u8 MsgPsynergyUnchanged[];
extern u8 MsgClassName[];
extern u8 Data_080313a4[];
extern u8 Data_08031424[];
s32 UiText_FormatNumberToHalfwords(s16 *out, s32 value);
s32 DjinnMenu_ListChangedDjinn(const struct OwnerActionSlot *oldGrid, const struct OwnerActionSlot *newGrid, u16 *out, s32 *gained, s32 *lost);
s32 Djinn_IsActiveFar(s32 owner, s32 col, s32 row);
void Djinn_ActivateFar(s32 owner, s32 col, s32 row);
void Djinn_DeactivateFar(s32 owner, s32 col, s32 row);

s32 __divsi3(s32 numerator, s32 denominator);
struct BattleUnitRecord *Owner_GetStateFar(s32 owner);
struct AbilityData *Ability_GetData(s32 code);
void Owner_RecalculateStatsFar(s32 owner);
struct RenderInput *UiWindow_Create(s32 x, s32 y, s32 w, s32 h, s32 style);
void UiWork_Finalize(struct RenderInput *win, s32 mode);
void UiText_DrawCharacterAtOffset(s32 message, struct RenderInput *win, s32 x, s32 y);
void UiText_DrawNumberInWindow(
    s32 value, s32 digits, struct RenderInput *win, s32 x, s32 y);

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
        if (output) {
            output->active = 1;
            output->kind = 1;
            output->index = Resource_LoadIntoFreeSlot(128);
            entry = (struct PreviewSprite *)((u8 *)output + 16);
            output->sentinel = 240;
            output->x = 120;
            output->y = 120;
            entry->attributes.word = 0x40000400;
            {
                s32 zero = 0;
                /* FAKEMATCH: a fresh zero keeps the clear off the s8 stores' bit-field zero, which global gives r8 (6 without, 2 with). */
                __asm__("" : "+r"(zero));
                entry->tile.value = zero;
            }
            entry->attributes.bits.x = win->x * 8 + x;
            entry->attributes.bits.y = win->y * 8 + y;
            entry->tile.bits.index = Resource_GetBuffer((u8)output->index, rising ? Data_080313a4 : Data_08031424);
            RenderOutput_AppendToList(win, output);
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

    /* Apply the pending change to the live record, then recompute. */
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
        /* Stat labels down the left edge, then the pre-change column. */
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
        /* Five list rows per page, clipped to whatever is left. */
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
        /* Post-change column, with an arrow beside every cell that moved. */
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

    /* Put the record back the way it was and release the scratch blocks. */
    Iwram_CopyWords(unit, snap, 332);
    Runtime_BumpFree(list);
    Runtime_BumpFree(snap);
    Runtime_BumpFree(buf);
    return win;
}

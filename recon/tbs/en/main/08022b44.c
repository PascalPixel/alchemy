#include "BATTLE_UNIT.H"
/*
 * Draft: the parent matches but its nested arrow renderer does not yet, so the
 * unit links as assembly (recon/tbs/raw/08022a7c.s and 08022b44.s).
 *
 * 2026-09-28, why the zero moves: the byte stores to one5/one4 expand as
 * read-modify-write with a zero mask (every byte member store does with this
 * compiler), CSE reuses that zero pseudo for the tile clear, and its REG_EQUIV
 * doubles its live length. With the tile clear last (the reference store
 * order), r5's local ratio 2/34 falls below the static chain pseudo's 2/22,
 * so global.c's find_reg evicts the zero from r5 ("utilized poorly by
 * local-alloc") and hands r5 to the chain; with the clear right after the
 * entry pointer (this draft) the ratio is 2/20 and nothing moves. u8-cast
 * byte stores remove the mask, give the reference's late zero and its store
 * order, but then the zero takes r2 and the chain r7. All 360 orders of the
 * index, entry, zero, sentinel, x, y and word statements were scored.
 */
/* DjinnMenu_ShowChangePreview (main:08022b44, 1588 bytes) is exact. Its
 * nested arrow renderer DjinnMenu_DrawStatArrow (main:08022a7c) is emitted
 * first in the same object and is not exact yet, so the unit keeps it as
 * retained assembly (NONMATCHING) and links the parent alone; the parent's
 * calls to DjinnMenu_DrawStatArrow.0 are aliased to the retained owner. The nested
 * function is 198+2 bytes with 7 halfwords off: in the reference the
 * word/zero stores of the sprite attributes follow the sentinel and scale
 * stores through the entry pointer, and its zero is materialised late in r5;
 * written in that order here, the zero constant is hoisted above the slot
 * load into r8 and every callee-saved assignment shifts (89 halfwords).
 * Storing the zero through output->table right after entry is formed keeps
 * the allocation (r8 x, sl y, fp rising, r9 chain, r6 output, r7 entry) and
 * leaves only the store order and the zero's base register.
 * Cause, from the RTL dumps (alchemy build allocator): the byte stores
 * output->active/one4 = 1 expand as load/and 0/or 1/store, and CSE reuses that
 * and-mask zero (set before the slot-load call) for the later tile zero, so
 * it crosses the call and takes r8. Storing the bytes through a u8 cast
 * removes the mask and the hoist, but the static chain is then copied to r7
 * and x moves to r9.
 * Second pass (2026-09-24): the ROM needs a call-crossing zero in r5 so the
 * chain loses the low registers and stays in r9. The mask zero gets r5 from
 * local-alloc only while its use sits right after entry is formed (r7 is
 * the frame pointer to local-alloc, r4 is call-used); stored after the
 * attribute word it takes r8 or r9. Written in ROM order with
 * a zero pinned to r5 (zero = 0; entry->tile.value = zero), the
 * allocation and store order match and 3 halfwords remain: sched2 issues the
 * reload copy `adds r7, r6, #0` after the x/y halfword stores. In sched2 the
 * copy and both stores have priority 20; the stores win the tie on
 * dependant count (4 against 2) because they may alias the attribute word
 * and tile stores (RenderOutput has char members, so its alias set
 * conflicts with every set, and the union stores are set 0). Local bitfield
 * views of RenderOutput, flat sprite structs, u32 casts, the store order of
 * x/y/sentinel/word/zero (all 120 orders), a two-step entry and u8 casts on
 * one4/one5 moved nothing further. */
#include "RENDER_INPUT.H"

/* Preview setting or resting one Djinn: toggle it on the live owner record,
 * recalculate, show the old and new class and stats with an arrow beside
 * each change (page 0) or the Psynergy gained and lost (later pages), then
 * restore the record from the snapshot. The arrow renderer is a GNU C nested
 * function (main:08022a7c) that reaches the window through the static chain.
 *
 * The six stat labels start at message 0x8ae and the
 * ability names another (Value_00000333); _call_via_r3,
 * UiText_RenderWideStringInWindow and UiWindow_DrawThreeTileColumn are
 * declared returning a value so r0 is set last at each call. The locals are
 * declared in the reference's spill-slot order. */

/* The IWRAM block-copy routine _call_via_r3 is asked to drive. */
#define VRAM_COPY_PROC 0x03001388

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

void Sys_Free(void *block);
s32 _call_via_r3(void *dst, const void *src, s32 size, s32 proc);
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
s32 Resource_LoadIntoFreeSlot(s32);
s32 Resource_GetBuffer(s32, const void *);
void RenderOutput_AppendToList(struct RenderInput *, struct RenderOutput *);
extern u8 Value_00000333[];
extern u8 Data_080313a4[];
extern u8 Data_08031424[];
s32 UiText_FormatNumberToHalfwords(s16 *out, s32 value);
s32 DjinnMenu_ListChangedDjinn(const struct OwnerActionSlot *oldGrid, const struct OwnerActionSlot *newGrid, u16 *out, s32 *gained, s32 *lost);
s32 Djinn_IsActiveFar(s32 owner, s32 col, s32 row);
void Djinn_ActivateFar(s32 owner, s32 col, s32 row);
void Djinn_DeactivateFar(s32 owner, s32 col, s32 row);

s32 __divsi3(s32 numerator, s32 denominator);
void *Runtime_BumpAllocate(s32 size);
void *Runtime_BumpAllocateAlternatePool(s32 size);
struct BattleUnitRecord *Runtime_GetObject(s32 owner);
struct AbilityData *Ability_GetData(s32 code);
void BattleUnit_Recalculate(s32 owner);
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
            output->table.value = 0;
            output->sentinel = 240;
            output->y = 120;
            output->x = 120;
            entry->attributes.word = 0x40000400;
            entry->attributes.bits.x = win->x * 8 + x;
            entry->attributes.bits.y = win->y * 8 + y;
            entry->tile.bits.index = Resource_GetBuffer((u8)output->index, rising ? Data_080313a4 : Data_08031424);
            RenderOutput_AppendToList(win, output);
        }
    }

    unit = Runtime_GetObject(owner);
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
    _call_via_r3(snap, unit, 332, VRAM_COPY_PROC);

    /* Apply the pending change to the live record, then recompute. */
    col = (code >> 8) & 15;
    row = code & 255;
    if (Djinn_IsActiveFar(owner, col, row) != 0)
        Djinn_DeactivateFar(owner, col, row);
    else
        Djinn_ActivateFar(owner, col, row);
    BattleUnit_Recalculate(owner);

    total = DjinnMenu_ListChangedDjinn((const struct OwnerActionSlot *)snap->grid, (const struct OwnerActionSlot *)unit->grid, list, &gained, &lost);
    pages = __divsi3(total - 1, 5) + 1;
    *pageCount = pages;
    if (page * 5 - 5 >= total)
        page = pages;

    if (page == 0) {
        /* Stat labels down the left edge, then the pre-change column. */
        UiText_DrawCharacterAtOffset(0x8ae, win, 0, 8);
        UiText_DrawCharacterAtOffset(0x8ae + 1, win, 0, 16);
        UiText_DrawCharacterAtOffset(0x8ae + 2, win, 0, 24);
        UiText_DrawCharacterAtOffset(0x8ae + 3, win, 0, 32);
        UiText_DrawCharacterAtOffset(0x8ae + 4, win, 0, 40);
        UiText_DrawCharacterAtOffset(0x8ae + 5, win, 0, 48);

        oldHp = snap->hp;
        UiText_FormatNumberToHalfwords(buf, oldHp);
        UiText_RenderWideStringInWindow(buf + 7, win, 5, 1);
        oldPp = snap->pp;
        UiText_FormatNumberToHalfwords(buf, oldPp);
        UiText_RenderWideStringInWindow(buf + 7, win, 5, 2);
        oldAtk = snap->atk;
        UiText_FormatNumberToHalfwords(buf, oldAtk);
        UiText_RenderWideStringInWindow(buf + 8, win, 6, 3);
        oldDef = snap->def;
        UiText_FormatNumberToHalfwords(buf, oldDef);
        UiText_RenderWideStringInWindow(buf + 8, win, 6, 4);
        oldAgi = snap->agi;
        UiText_FormatNumberToHalfwords(buf, oldAgi);
        UiText_RenderWideStringInWindow(buf + 8, win, 6, 5);
        UiText_FormatNumberToHalfwords(buf, snap->luk);
        UiText_RenderWideStringInWindow(buf + 8, win, 5, 6);

        UiWindow_DrawDividerLine(win, 0, 8, 19, 8);
        if (gained != 0 || lost != 0)
            UiWork_SetParamNibble(2);
        UiText_DrawCharacterAtOffset(0x8ad, win, 24, 64);
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

            UiText_DrawCharacterAtOffset((list[i] & 0x3fff) + (s32)Value_00000333, win, 16, n * 16);
            UiWindow_SetTilemapEntry(win, 0xf01f, 11, n * 2, 0);
            UiWindow_SetTilemapEntry(win, 0xf01e, 12, n * 2, 0);
            UiText_DrawNumberInWindow(
                Ability_GetData(list[i])->cost, 2, win, 104, n * 16);
        }

        if (gained != 0) {
            UiWork_SetParamNibble(4);
            UiText_DrawCharacterAtOffset(0xba2, win, 32, 80);
            marks = 1;
        }
        if (lost != 0) {
            UiWork_SetParamNibble(2);
            UiText_DrawCharacterAtOffset(0xba3, win, 32, marks * 8 + 80);
            marks++;
        }
        if (marks == 0)
            UiText_DrawCharacterAtOffset(0xba8, win, 32, 80);
        UiWork_SetParamNibble(15);
        UiWork_SetParamNibble(15);
        UiWindow_DrawDividerLine(win, 0, 10, 19, 10);
    }

    if (page == 0) {
        /* Post-change column, with an arrow beside every cell that moved. */
        UiText_DrawCharacterAtOffset(0x741 + snap->element, win, 0, 0);
        UiText_DrawCharacterAtOffset(0x741 + unit->element, win, 80, 0);
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
    _call_via_r3(unit, snap, 332, VRAM_COPY_PROC);
    Sys_Free(list);
    Sys_Free(snap);
    Sys_Free(buf);
    return win;
}

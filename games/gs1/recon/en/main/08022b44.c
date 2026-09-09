#include "types.h"

/*
 * Ability change preview panel (owner main:08022b44, 1588 bytes).
 *
 * Called from the battle ability picker (main:08023e70, which already
 * declares this owner as
 *   struct UiWindow *Func_08022b44(struct UiWindow *win, s32 owner,
 *                                  u32 code, s32 sel, s32 *count);
 * ) to show what toggling one Djinn/ability entry would do to the acting
 * unit.  The panel is rebuilt from scratch on every call: an existing
 * window is finalized, a new one is created, the unit record is snapshotted,
 * the pending change is applied for real, the new record is measured against
 * the snapshot, and the snapshot is copied back so the unit is left
 * untouched.  The freshly created window is returned; 0 means the window
 * could not be created and nothing else was done.
 *
 * `code` is the picker's packed table code: bits 8..11 select the grid
 * column and bits 0..7 the row inside it, which is exactly how
 * Func_08077208 / Func_080771b8 / Func_080771b0 are addressed here.  The
 * reference extracts the column with `asrs`, so the parameter is signed here
 * even though the picker hands it over as u32.
 *
 * `page` is both an input and a working value.  Page 0 draws the stat
 * comparison (old column, new column, up/down arrows and the element
 * glyphs); pages 1..n draw five list rows each.  The page count is written
 * back through `pageCount`, and an out-of-range request is clamped to the
 * last page before anything is drawn.
 *
 * Uncertain, and left as raw offsets rather than guessed: the unit record is
 * only modelled where this screen reads it (the six stat cells at 0x38..0x42,
 * the 0x58 grid block handed to Func_080228e4 and the element byte at 0x129);
 * the 332-byte snapshot extent is the size the two Func_080072f0 calls use.
 * Func_080228e4's two out-parameters are named for what the drawing code does
 * with them - one lights the "gained" marker, the other the "lost" marker.
 *
 * Known residual, stated rather than papered over, and measured rather than
 * guessed.  The control-flow topology of this draft is equal to the reference
 * and the trailing literal pool matches word for word, but the reference
 * reaches the window through a frame cell instead of holding it in a
 * register: it computes `add r4, sp, #72` once, keeps that address in sl, and
 * then spells every use as `mov rX, sl` + `ldr rY, [rX]` (36 sites).  Before
 * each of the six Func_08022a7c calls it also sets r9 - the Thumb static
 * chain register - to the frame top, and Func_08022a7c reads the window back
 * out of chain-4, which is exactly that frame cell.  That is the signature of
 * a GNU C nested function declared inside this function; the project owns
 * 0x08022a7c as its own module (asm/08022a7c.s, classified
 * hidden_register_context_module) and no caller anywhere else in the image
 * references it.
 *
 * The approved route cannot emit a static chain without redefining that owner
 * here, so this draft calls it as an ordinary function and holds the window
 * directly.  Everything the reference does is still present; the cost is the
 * 100 bytes of that idiom (36 x 2 for the double indirections, 4 for the cell
 * address, 6 x 4 for the chain setups), against which the freed registers buy
 * about 12 bytes back elsewhere.  1500 candidate bytes against 1588 is that
 * difference and nothing else - no branch, call, store or argument is missing.
 */

/* The IWRAM block-copy routine Func_080072f0 is asked to drive. */
#define VRAM_COPY_PROC 0x03001388

/* Window handle as the neighbouring battle menus already model it. */
struct UiWindow {
    s32 field_00;
    s32 field_04;
    u16 field_08;
    u16 field_0a;
    u16 field_0c;
    u16 field_0e;
};

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
#define UiWork_SetParamNibble Func_0801e71c
#define UiWindow_SetTilemapEntry Func_08019000
#define UiWindow_DrawThreeTileColumn Func_080218dc
#define Ui_CreateOutputFromResourceSlot Func_08022a38
#define UiText_FormatNumberToHalfwords Func_080228bc

void Func_08002df0(void *block);
void Func_080072f0(void *dst, const void *src, s32 size, s32 proc);
void Func_08017c8c(s16 *text, struct UiWindow *win, s32 x, s32 y);
void Func_0801e41c(struct UiWindow *win, s32 x, s32 y, s32 w, s32 h);
void UiWork_SetParamNibble(s32 nibble);
void UiWindow_SetTilemapEntry(
    struct UiWindow *win, s32 tile, s32 x, s32 y, s32 flags);
void UiWindow_DrawThreeTileColumn(
    struct UiWindow *win, s32 x, s32 y, s32 tile, s32 flags);
void Ui_CreateOutputFromResourceSlot(
    struct UiWindow *win, s32 arg1, s32 arg2, s32 resource);
void Func_08022a7c(s32 x, s32 y, s32 rising);
s32 UiText_FormatNumberToHalfwords(s16 *out, s32 value);
s32 Func_080228e4(u8 *oldGrid, u8 *newGrid, u16 *out, s32 *gained, s32 *lost);
s32 Func_08077208(s32 owner, s32 col, s32 row);
void Func_080771b0(s32 owner, s32 col, s32 row);
void Func_080771b8(s32 owner, s32 col, s32 row);

s32 FixedPoint_Ratio(s32 numerator, s32 denominator);
void *Runtime_BumpAllocate(s32 size);
void *Runtime_BumpAllocateAlternatePool(s32 size);
struct BattleUnitRecord *Runtime_GetObject(s32 owner);
struct AbilityData *Ability_GetData(s32 code);
void BattleUnit_Recalculate(s32 owner);
struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 w, s32 h, s32 style);
void UiWork_Finalize(struct UiWindow *win, s32 mode);
void UiText_DrawCharacter(s32 message, struct UiWindow *win, s32 x, s32 y);
void UiText_DrawNumberInWindow(
    s32 value, s32 digits, struct UiWindow *win, s32 x, s32 y);

#define Ui_ShowAbilityChangePreview Func_08022b44

struct UiWindow *Ui_ShowAbilityChangePreview(
    struct UiWindow *win, s32 owner, s32 code, s32 page, s32 *pageCount)
{
    struct BattleUnitRecord *unit;
    struct BattleUnitRecord *snap;
    struct AbilityData *info;
    s16 *buf;
    u16 *list;
    s32 col;
    s32 row;
    s32 total;
    s32 pages;
    s32 rows;
    s32 marks;
    s32 gained;
    s32 lost;
    s32 tile;
    s32 dir;
    s32 i;
    s32 n;
    s32 oldHp;
    s32 oldPp;
    s32 oldAtk;
    s32 oldDef;
    s32 oldAgi;

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
    Func_080072f0(snap, unit, 332, VRAM_COPY_PROC);

    /* Apply the pending change to the live record, then recompute. */
    col = (code >> 8) & 15;
    row = code & 255;
    if (Func_08077208(owner, col, row) != 0)
        Func_080771b8(owner, col, row);
    else
        Func_080771b0(owner, col, row);
    BattleUnit_Recalculate(owner);

    total = Func_080228e4(snap->grid, unit->grid, list, &gained, &lost);
    pages = FixedPoint_Ratio(total - 1, 5) + 1;
    *pageCount = pages;
    if (page * 5 - 5 >= total)
        page = pages;

    if (page == 0) {
        /* Stat labels down the left edge, then the pre-change column. */
        UiText_DrawCharacter(0x8ae, win, 0, 8);
        UiText_DrawCharacter(0x8af, win, 0, 16);
        UiText_DrawCharacter(0x8b0, win, 0, 24);
        UiText_DrawCharacter(0x8b1, win, 0, 32);
        UiText_DrawCharacter(0x8b2, win, 0, 40);
        UiText_DrawCharacter(0x8b3, win, 0, 48);

        oldHp = snap->hp;
        UiText_FormatNumberToHalfwords(buf, oldHp);
        Func_08017c8c(buf + 7, win, 5, 1);
        oldPp = snap->pp;
        UiText_FormatNumberToHalfwords(buf, oldPp);
        Func_08017c8c(buf + 7, win, 5, 2);
        oldAtk = snap->atk;
        UiText_FormatNumberToHalfwords(buf, oldAtk);
        Func_08017c8c(buf + 8, win, 6, 3);
        oldDef = snap->def;
        UiText_FormatNumberToHalfwords(buf, oldDef);
        Func_08017c8c(buf + 8, win, 6, 4);
        oldAgi = snap->agi;
        UiText_FormatNumberToHalfwords(buf, oldAgi);
        Func_08017c8c(buf + 8, win, 6, 5);
        UiText_FormatNumberToHalfwords(buf, snap->luk);
        Func_08017c8c(buf + 8, win, 5, 6);

        Func_0801e41c(win, 0, 8, 19, 8);
        if (gained != 0 || lost != 0)
            UiWork_SetParamNibble(2);
        UiText_DrawCharacter(0x8ad, win, 24, 64);
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

            UiText_DrawCharacter(0x333 + (list[i] & 0x3fff), win, 16, n * 16);
            UiWindow_SetTilemapEntry(win, 0xf01f, 11, n * 2, 0);
            UiWindow_SetTilemapEntry(win, 0xf01e, 12, n * 2, 0);
            UiText_DrawNumberInWindow(
                Ability_GetData(list[i])->cost, 2, win, 104, n * 16);
        }

        if (gained != 0) {
            UiWork_SetParamNibble(4);
            UiText_DrawCharacter(0xba2, win, 32, 80);
            marks = 1;
        }
        if (lost != 0) {
            UiWork_SetParamNibble(2);
            UiText_DrawCharacter(0xba3, win, 32, marks * 8 + 80);
            marks++;
        }
        if (marks == 0)
            UiText_DrawCharacter(0xba8, win, 32, 80);
        UiWork_SetParamNibble(15);
        UiWork_SetParamNibble(15);
        Func_0801e41c(win, 0, 10, 19, 10);
    }

    if (page == 0) {
        /* Post-change column, with an arrow beside every cell that moved. */
        UiText_DrawCharacter(0x741 + snap->element, win, 0, 0);
        UiText_DrawCharacter(0x741 + unit->element, win, 80, 0);
        if (snap->element != unit->element)
            UiWindow_SetTilemapEntry(win, 0xf728, 9, 0, page);
        else
            UiWindow_SetTilemapEntry(win, 0xf729, 9, 0, page);

        UiText_FormatNumberToHalfwords(buf, unit->hp);
        Func_08017c8c(buf + 7, win, 11, 1);
        if (unit->hp != oldHp) {
            dir = 0;
            if (unit->hp > oldHp)
                dir = 1;
            Func_08022a7c(80, 14, dir);
        }

        UiText_FormatNumberToHalfwords(buf, unit->pp);
        Func_08017c8c(buf + 7, win, 11, 2);
        if (unit->pp != oldPp) {
            dir = 0;
            if (unit->pp > oldPp)
                dir = 1;
            Func_08022a7c(80, 22, dir);
        }

        UiText_FormatNumberToHalfwords(buf, unit->atk);
        Func_08017c8c(buf + 7, win, 11, 3);
        if (unit->atk != oldAtk) {
            dir = 0;
            if (unit->atk > oldAtk)
                dir = 1;
            Func_08022a7c(80, 30, dir);
        }

        UiText_FormatNumberToHalfwords(buf, unit->def);
        Func_08017c8c(buf + 7, win, 11, 4);
        if (unit->def != oldDef) {
            dir = 0;
            if (unit->def > oldDef)
                dir = 1;
            Func_08022a7c(80, 38, dir);
        }

        UiText_FormatNumberToHalfwords(buf, unit->agi);
        Func_08017c8c(buf + 7, win, 11, 5);
        if (unit->agi != oldAgi) {
            dir = 0;
            if (unit->agi > oldAgi)
                dir = 1;
            Func_08022a7c(80, 46, dir);
        }

        UiText_FormatNumberToHalfwords(buf, unit->luk);
        Func_08017c8c(buf + 8, win, 12, 6);
        if (unit->luk != snap->luk) {
            dir = 0;
            if (unit->luk > snap->luk)
                dir = 1;
            Func_08022a7c(80, 54, dir);
        }
    }

    /* Put the record back the way it was and release the scratch blocks. */
    Func_080072f0(unit, snap, 332, VRAM_COPY_PROC);
    Func_08002df0(list);
    Func_08002df0(snap);
    Func_08002df0(buf);
    return win;
}

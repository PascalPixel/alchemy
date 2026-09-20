#include "RENDER_INPUT.H"

/* Preview a pending ability change, then restore the original unit record.
 * The nested arrow renderer captures this panel's window.
 */

/* The IWRAM block-copy routine Func_080072f0 is asked to drive. */
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
#define UiWork_SetParamNibble Func_0801e71c
#define UiWindow_SetTilemapEntry Func_08019000
#define UiWindow_DrawThreeTileColumn Func_080218dc
#define UiText_FormatNumberToHalfwords Func_080228bc

void Func_08002df0(void *block);
void Func_080072f0(void *dst, const void *src, s32 size, s32 proc);
void Func_08017c8c(s16 *text, struct RenderInput *win, s32 x, s32 y);
void Func_0801e41c(struct RenderInput *win, s32 x, s32 y, s32 w, s32 h);
void UiWork_SetParamNibble(s32 nibble);
void UiWindow_SetTilemapEntry(
    struct RenderInput *win, s32 tile, s32 x, s32 y, s32 flags);
void UiWindow_DrawThreeTileColumn(
    struct RenderInput *win, s32 x, s32 y, s32 tile, s32 flags);
struct PreviewSprite {
    u32 unknown;
    union {
        u32 word;
        struct { unsigned y:8; unsigned flags:8; unsigned x:9; unsigned high:7; } bits;
    } attributes;
    union RenderTableValue tile;
};
struct RenderOutput *Func_08015e8c(void);
s32 Func_080040b4(s32);
s32 Func_080040d0(s32, const void *);
void Func_08016584(struct RenderInput *, struct RenderOutput *);
extern u8 Data_080313a4[];
extern u8 Data_08031424[];
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
struct RenderInput *UiWindow_Create(s32 x, s32 y, s32 w, s32 h, s32 style);
void UiWork_Finalize(struct RenderInput *win, s32 mode);
void UiText_DrawCharacterAtOffset(s32 message, struct RenderInput *win, s32 x, s32 y);
void UiText_DrawNumberInWindow(
    s32 value, s32 digits, struct RenderInput *win, s32 x, s32 y);

#define Ui_ShowAbilityChangePreview Func_08022b44

struct RenderInput *Ui_ShowAbilityChangePreview(
    struct RenderInput *win, s32 owner, s32 code, s32 page, s32 *pageCount)
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


    void Func_08022a7c(s32 x, s32 y, s32 rising)
    {
        struct RenderOutput *output = Func_08015e8c();
        struct PreviewSprite *entry;
        if (output) {
            output->one5 = 1;
            output->one4 = 1;
            output->index = Func_080040b4(128);
            output->sentinel = 240;
            output->x = 120;
            output->y = 120;
            entry = (struct PreviewSprite *)((u8 *)output + 16);
            entry->attributes.word = 0x40000400;
            entry->tile.value = 0;
            entry->attributes.bits.x = win->x * 8 + x;
            entry->attributes.bits.y = win->y * 8 + y;
            entry->tile.bits.index = Func_080040d0((u8)output->index, rising ? Data_080313a4 : Data_08031424);
            Func_08016584(win, output);
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
        UiText_DrawCharacterAtOffset(0x8ae, win, 0, 8);
        UiText_DrawCharacterAtOffset(0x8af, win, 0, 16);
        UiText_DrawCharacterAtOffset(0x8b0, win, 0, 24);
        UiText_DrawCharacterAtOffset(0x8b1, win, 0, 32);
        UiText_DrawCharacterAtOffset(0x8b2, win, 0, 40);
        UiText_DrawCharacterAtOffset(0x8b3, win, 0, 48);

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

            UiText_DrawCharacterAtOffset(0x333 + (list[i] & 0x3fff), win, 16, n * 16);
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
        Func_0801e41c(win, 0, 10, 19, 10);
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

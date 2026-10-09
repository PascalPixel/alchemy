#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "mem.h"
#include "battle_unit.h"
#include "render_input.h"
#include "iwram_call.h"
#include "edition.h"
#include "djinn_preview.h"
#include "btlsys.h"
#include "uiflow.h"
#include "text/msg_ids.h"
#include "io_reg.h"
#include "callback_scheduler.h"
#include "btlwork.h"
#include "menu_list.h"
#include "ui.h"
#include "vram_block.h"
#include "motobj.h"
#include "heap_state.h"
#include "types.h"
#include "btlpty.h"
struct BtlAct * BtlActGet(s32 action_id);
void UnitCalcFar(s32 unit_id);

#define UiWindow_Create ((struct RenderInput * (*)(s32, s32, s32, s32, s32))UiWindow_Create)
#define UiText_DrawCharacterAtOffset ((void (*)(s32, struct RenderInput *, s32, s32))UiText_DrawCharacterAtOffset)

#define UiWindow_SetTilemapEntry ((void (*)(struct RenderInput*,s32,s32,s32,s32))UiWindow_SetTilemapEntry)
#define UiWindow_DrawThreeTileColumn ((s32 (*)(struct RenderInput*,s32,s32,s32,s32))UiWindow_DrawThreeTileColumn)

#define Djinn_DeactivateFar ((void (*)(s32,s32,s32))Djinn_DeactivateFar)
#define BtlActGet ((struct AbilityData * (*)(s32))BtlActGet)

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
    s16 ep;
    u16 atk;
    u16 def;
    u16 agi;
    u8 luk;
    u8 field_43[0x58 - 0x43];
    u8 grid[0x129 - 0x58];
    u8 element;
};

void UiWindow_DrawDividerLine(struct RenderInput *win, s32 x, s32 y, s32 w, s32 h);
void UiWork_SetParamNibble(s32 nibble);
s32 UiText_FormatNumberToHalfwords(s16 *out, s32 value);
s32 DjinnMenu_ListChangedDjinn(const struct OwnerActionSlot *oldGrid, const struct OwnerActionSlot *newGrid, u16 *out, s32 *gained, s32 *lost);

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
            /* FAKEMATCH: a fresh zero keeps the window chain in r9. Without this barrier, the nested DjinnMenu_DrawStatArrow.0 has reference 80, built 82, common 71, differ 20: r8/r9 allocation changes and two explicit zero instructions remain; extent 0xc6 -> 0xca. */
            __asm__("" : "=r"(zero) : "0"(zero));
            /* FAKEMATCH: a pointer-typed clear does not wait for the x and y stores (sched2 order) */
            *(void **)&entry->tile = (void *)zero;
            entry->attributes.bits.x = win->x * 8 + x;
            entry->attributes.bits.y = win->y * 8 + y;
            entry->tile.bits.index = Resource_GetBuffer((u8)output->index, (s32)(rising ? DjinnMenu_ArrowUpTiles : DjinnMenu_ArrowDownTiles));
            RenderOutput_AppendToList((struct RenderOutputList *)win, output);
        }
    }

    unit = UnitGetFar(owner);
    if (win != 0)
        UiWork_Finalize(win, 1);

    if (page == 0)
        win = UiWindow_Create(0, 8, 21, 11, 6);
    else
        win = UiWindow_Create(0, 5, 21, 14, 6);
    if (win == 0)
        return 0;

    buf = (s16 *)MemAlloc(128);
    snap = (struct BattleUnitRecord *)MemAlloc(332);
    list = (u16 *)Runtime_BumpAllocateAlternatePool(96);
    Iwram_CopyWords(snap, unit, 332);

    col = (code >> 8) & 15;
    row = code & 255;
    if (Djinn_IsActiveFar(owner, col, row) != 0)
        Djinn_DeactivateFar(owner, col, row);
    else
        Djinn_ActivateFar(owner, col, row);
    UnitCalcFar(owner);

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
        oldPp = snap->ep;
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
            info = BtlActGet(list[i]);
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
                BtlActGet(list[i])->cost, 2, win, 104, n * 16);
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

        UiText_FormatNumberToHalfwords(buf, unit->ep);
        UiText_RenderWideStringInWindow(buf + 7, win, 11, 2);
        if (unit->ep != oldPp) {
            val = 0;
            if (unit->ep > oldPp)
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
    MemFree(list);
    MemFree(snap);
    MemFree(buf);
    return win;
}

#if EDITION_INTERNATIONAL
#define OWNER_STATUS_TEXT_MAX 0x80
#define OWNER_STATUS_VALUE_X 184
#else
#define OWNER_STATUS_TEXT_MAX 0x40
#define OWNER_STATUS_VALUE_X 176
#endif

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
#define OWNER_STATUS_LABEL(row) ((s32)MsgOwnerStatusAttack + (row))
#else
#define OWNER_STATUS_LABEL(row) ((s32)MsgOwnerStatusExp - 10 + (row))
#endif

struct StatusPixel {
    s32 x;
    s32 y;
};

struct StatusCursor {
    s8 pos[5];
    s32 entry;
    s32 lastEntry;
    s32 mode;
    s32 limit;
    s32 col;
    s32 row;
};

void UiWork_PushValueSlot(s32 value, s32 slot);
void UiWindow_MarkVisibleTileAttributes(void);
void Resource_LoadTableEntryToBuffer(s32 icon, s32 handle);
s32 Ui_LoadEntryForKind(s32 owner, s32 handle);
s32 Party_SumDjinnCountsFar(s32 request);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
s32 System_GetLanguage(void);
#endif

s32 Ui_RunOwnerStatusScreen(u16 *list, s32 listCount, s32 owner)
{
    struct MenuSprite objFace;
    struct MenuSprite objCursor;
    s32 tbl[11];
    s8 sel[11];
    struct MenuSprite objIcon[11];
    struct MenuSprite objName;
    struct StatusCursor cur;
    struct UiRenderWork *base;
    struct UiWindow *winMain;
    struct UiWindow *winDesc;
    s32 idx;
    s32 handleCursor;
    struct StatusPixel pix;
    s32 handleFrame;
    s32 cnt;
    s32 extended;
    s32 keys;
    u16 *text;
    s32 iconIdx;
    s32 *q;
    s32 defDelta;
    s32 i;
    s32 j;
    s32 atkDelta;
    s32 redraw;
    struct BtlUnit *object;
    struct BtlUnit *saved;
    const struct OwnerStatusStatCursor *stat;
    struct MenuSprite *p;
    s32 agiDelta;
    s32 code;
    s32 n;
    struct MenuCell *screen;
    s32 x;

    base = (struct UiRenderWork *)gWindowWork[0];
    redraw = 1;
    idx = -1;
    handleFrame = Resource_LoadIntoFreeSlot(0x200);
    cnt = 0;
    extended = Party_SumDjinnCountsFar(-1);

    screen = ((struct MenuCell *)gWindowWork[42]);
    screen->busy = 1;
    if (screen->work != NULL) {
        UiWork_Finalize(screen->work, 1);
        screen->work = NULL;
    }
    AudioCommand_PlayFar(0x70);

    for (i = 4; i >= 0; i--) {
        cur.pos[i] = 0;
    }
    cur.entry = 0;
    cur.lastEntry = 0;
    cur.mode = 0;

    handleCursor = Resource_LoadIntoFreeSlot(0x80);
    for (i = 0; i <= 10; i++) {
        tbl[i] = Resource_LoadIntoFreeSlot(0x80);
        sel[i] = -1;
    }

    if (listCount != 0) {
        for (i = 0; i <= 5; i++) {
            s32 kind = list[i];

            if (kind == 0xFF) {
                break;
            }
            if (kind != 0xFE) {
                if (kind == owner) {
                    idx = i;
                    break;
                }
            }
        }
    }

    pix.x = 0;
    pix.y = 0;
    winMain = (struct UiWindow *)UiWindow_Create(0, 0, 30, 20, 6);
    winDesc = (struct UiWindow *)UiWindow_Create(0, 14, 30, 6, 10);
    UiWindow_MarkVisibleTileAttributes();

    for (;;) {
        keys = gKeysRepeat;

        objName.oam.word.attr01 = 0x80000400;
        objName.oam.word.attr23 = 0;
        objName.oam.f.tile = Ui_LoadEntryForKind(owner, handleFrame);
        objName.oam.f.x = 8;
        objName.oam.f.y = 24;
        objName.oam.f.palette = 14;
        Runtime_PushSlotEntry((s32 *)&objName, 240);

        switch (cur.entry) {
        case 0:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        default:
            cur.mode = 0;
            if (extended != 0) {
                cur.limit = 9;
            } else {
                cur.limit = 7;
            }
            break;
        case 10:
        case 11:
        case 12:
        case 13:
            cur.mode = 1;
            cur.limit = 4;
            break;
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
            cur.mode = 2;
            cur.limit = cnt;
            break;
        }

        Ui_SetRectHighlight(
            winMain->x, winMain->y, winMain->width, winMain->height, 15);

        if (cnt != 0) {
            if ((u32)cur.mode <= 1) {
                n = cur.pos[cur.mode];
                if ((keys & 0x80) != 0) {
                    keys = 0;
                    AudioCommand_PlayFar(0x6F);
                    n++;
                    if (n >= cur.limit) {
                        n = 0;
                        if (cur.mode == 1) {
                            cur.mode = 2;
                            n = cur.pos[cur.mode];
                        }
                    }
                } else if ((keys & 0x40) != 0) {
                    keys = 0;
                    AudioCommand_PlayFar(0x6F);
                    n--;
                    if (n < 0) {
                        n = cur.limit - 1;
                        if (cur.mode == 1) {
                            cur.mode = 2;
                            n = cur.pos[cur.mode];
                        }
                    }
                } else if ((keys & 0x31) != 0) {
                    keys = 0;
                    AudioCommand_PlayFar(0x6F);
                    cur.mode ^= 2;
                    if ((gKeysRepeat & KEY_A) != 0) {
                        n = cur.pos[cur.mode];
                    } else if ((gKeysRepeat & KEY_LEFT) != 0) {
                        n = cnt - 1;
                    } else {
                        n = 0;
                    }
                }
                cur.pos[cur.mode] = (s8)n;
            } else if (cur.mode == 2) {
                n = cur.pos[cur.mode];
                if (n >= cur.limit) {
                    n = cur.limit - 1;
                }
                if (n < 0) {
                    cur.mode = 0;
                    n = cur.pos[0];
                } else if ((keys & 0x10) != 0) {
                    keys = 0;
                    n++;
                    if (n >= cur.limit) {
                        cur.mode = 0;
                        n = cur.pos[0];
                    }
                    AudioCommand_PlayFar(0x6F);
                } else if ((keys & 0x20) != 0) {
                    keys = 0;
                    n--;
                    if (n < 0) {
                        cur.mode = 0;
                        n = cur.pos[0];
                    }
                    AudioCommand_PlayFar(0x6F);
                } else if ((keys & 0xC1) != 0) {
                    keys = 0;
                    cur.mode = 0;
                    n = cur.pos[0];
                    AudioCommand_PlayFar(0x6F);
                }
                cur.pos[cur.mode] = (s8)n;
            }
        }

        if (cur.mode == 0) {
            s32 k = cur.pos[cur.mode];

            if (extended == 0) {
                k += 9;
            }
            stat = &OwnerStatus_StatCursors[k];
            cur.entry = stat->entry;
            cur.col = stat->column;
            cur.row = stat->row;
            Ui_SetRectHighlight(
                winMain->x + stat->highlight_column + 1, winMain->y + stat->highlight_row + 1, stat->highlight_width, 1,
                14);
        } else if (cur.mode == 1) {
            cur.entry = OwnerStatus_EquipmentCursors[cur.pos[cur.mode]].entry;
            cur.col = OwnerStatus_EquipmentCursors[cur.pos[cur.mode]].column;
            cur.row = OwnerStatus_EquipmentCursors[cur.pos[cur.mode]].row;
        } else if (cur.mode == 2) {
            cur.entry = OwnerStatus_ConditionCursors[cur.pos[cur.mode]].entry;
            cur.col = OwnerStatus_ConditionCursors[cur.pos[cur.mode]].column;
            cur.row = OwnerStatus_ConditionCursors[cur.pos[cur.mode]].row;
        }

        if (cur.lastEntry != cur.entry) {
            cur.lastEntry = cur.entry;
            redraw = 2;
        }
        pix.x = cur.col * 8;
        pix.y = cur.row * 8;

        {
            struct MenuSprite *face = GetBattleObjectSlotFar(owner)->object->records;

            objFace.oam.word.attr01 = 0xC0002400;
            objFace.oam.word.attr23 = 0;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
            /* FAKEMATCH: the zero lives in sl here; sched2 puts the attr23 store before the move into sl and the 0xFC00 load before the face tile load, which may alias that store; native moves into sl between the two stores and loads the tile first (4 instructions) */
            asm("");
#endif
            objFace.oam.f.tile = face->oam.f.tile;
            objFace.oam.f.x = 0xAC;
            objFace.oam.f.y = 56;
            Runtime_PushSlotEntry((s32 *)&objFace, 240);
        }

        objCursor.oam.word.attr01 = 0x40000400;
        objCursor.oam.word.attr23 = 0;
        /* FAKEMATCH: arm_reorg counts this empty asm as 4 bytes, so the gFrameCount literal falls past the face x literal's 64-byte ldrh range and opens the next pool, as native; the pool and its branch move two instructions earlier, 85 -> 63. */
        /* FAKEMATCH: removing both barriers gives EN 1392/1392, common 1390, differ 4: gFrameCount joins the earlier pool and the branch follows two extra loads. ES 1458/1458, common 1455, differ 6: attr23/mov sl order and the 0x3ff load also move. */
        asm("");
        objCursor.oam.f.tile = Resource_GetBuffer(handleCursor, (s32)Resource_FixedBlockBTiles);
        objCursor.oam.f.x = (pix.x + winMain->x * 8
                          - ((gFrameCount & 4) >> 2))
            + 16;
        objCursor.oam.f.y = (u8)((pix.y + winMain->y * 8
                               - ((gFrameCount & 4) >> 2))
            + 16);
        objCursor.oam.f.affine_index = 8;
        Runtime_PushSlotEntry((s32 *)&objCursor, 241);

        if (redraw != 0) {
            object = UnitGetFar(owner);
#if EDITION_INTERNATIONAL
            UiWindow_MarkVisibleTileAttributes();
#endif
            Ui_FillVramBlockPattern();

            if ((redraw & 1) != 0) {
                RenderOutput_RedrawSavedRect(winMain);
                UiWindow_MapTextCanvasTiles(winDesc->x, winDesc->y, winDesc->width,
                    winDesc->height, 0);
                UiWindow_DrawDividerLine((struct RenderInput *)winMain, 0, 14, 29, 14);

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
                switch (System_GetLanguage()) {
                case 2:
                    UiText_DrawStringAtOffset((u8 *)object->name, (struct RenderInput *)winMain, 0, 0);
                    UiText_DrawStringAtOffset((u8 *)OwnerStatus_NivString, (struct RenderInput *)winMain, 51, 0);
                    UiText_DrawNumberInWindow(object->level, 2, (struct RenderInput *)winMain, 72, 0);
                    break;
                case 3:
                    UiText_DrawStringAtOffset((u8 *)object->name, (struct RenderInput *)winMain, 0, 0);
                    UiText_DrawStringAtOffset((u8 *)OwnerStatus_NvString, (struct RenderInput *)winMain, 56, 0);
                    UiText_DrawNumberInWindow(object->level, 2, (struct RenderInput *)winMain, 72, 0);
                    break;
                default:
                    UiText_DrawStringAtOffset((u8 *)object->name, (struct RenderInput *)winMain, 0, 0);
                    UiText_DrawStringAtOffset((u8 *)OwnerStatus_LevelString, (struct RenderInput *)winMain, 56, 0);
                    UiText_DrawNumberInWindow(object->level, 2, (struct RenderInput *)winMain, 72, 0);
                    break;
                }
#else
                UiText_DrawStringAtOffset((u8 *)object->name, (struct RenderInput *)winMain, 0, 0);
#if defined(TBS_EDITION_FR)
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_LevelString, (struct RenderInput *)winMain, 51, 0);
#else
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_LevelString, (struct RenderInput *)winMain, 56, 0);
#endif
                UiText_DrawNumberInWindow(object->level, 2, (struct RenderInput *)winMain, 72, 0);
#endif
                UiText_DrawCharacterAtOffset((s32)MsgOwnerStatusExp, (struct RenderInput *)winMain, 0, 8);
                UiText_DrawNumberInWindow(
                    object->experience, 8, (struct RenderInput *)winMain, 40, 8);

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
                switch (System_GetLanguage()) {
                case 1:
                    UiText_DrawStringAtOffset((u8 *)OwnerStatus_KpString, (struct RenderInput *)winMain, 40, 24);
                    break;
                case 2:
                    UiText_DrawStringAtOffset((u8 *)OwnerStatus_PvString, (struct RenderInput *)winMain, 40, 24);
                    break;
                case 3:
                    UiText_DrawStringAtOffset((u8 *)OwnerStatus_PvString, (struct RenderInput *)winMain, 40, 24);
                    break;
                case 4:
                    UiText_DrawStringAtOffset((u8 *)OwnerStatus_HpString, (struct RenderInput *)winMain, 40, 24);
                    break;
                }
#else
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_HpString, (struct RenderInput *)winMain, 40, 24);
#endif
                UiText_DrawNumberInWindow(
                    object->hp, 4, (struct RenderInput *)winMain, 56, 24);
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_SlashString, (struct RenderInput *)winMain, 88, 24);
                UiText_DrawNumberInWindow(
                    object->max_hp, 4, (struct RenderInput *)winMain, 96, 24);
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_PpString, (struct RenderInput *)winMain, 40, 32);
                UiText_DrawNumberInWindow(
                    object->ep, 4, (struct RenderInput *)winMain, 56, 32);
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_SlashString, (struct RenderInput *)winMain, 88, 32);
                UiText_DrawNumberInWindow(
                    object->max_ep, 4, (struct RenderInput *)winMain, 96, 32);

                UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(0), (struct RenderInput *)winMain, 136, 16);
                UiText_DrawNumberInWindow(
                    object->atk, 3, (struct RenderInput *)winMain, OWNER_STATUS_VALUE_X, 16);
                UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(1), (struct RenderInput *)winMain, 136, 24);
                UiText_DrawNumberInWindow(
                    object->def, 3, (struct RenderInput *)winMain, OWNER_STATUS_VALUE_X, 24);
                UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(2), (struct RenderInput *)winMain, 136, 32);
                UiText_DrawNumberInWindow(
                    object->agi, 3, (struct RenderInput *)winMain, OWNER_STATUS_VALUE_X, 32);
                UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(3), (struct RenderInput *)winMain, 136, 40);
                UiText_DrawNumberInWindow(object->luck, 3, (struct RenderInput *)winMain, OWNER_STATUS_VALUE_X, 40);

                UiText_DrawCharacterAtOffset(
                    (s32)MsgClassName + object->class_index, (struct RenderInput *)winMain, 0, 48);
                if (extended != 0) {
                    UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(9), (struct RenderInput *)winMain, 0, 72);
                }
                UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(5), (struct RenderInput *)winMain, 0, 80);
                UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(6), (struct RenderInput *)winMain, 0, 88);
                UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(7), (struct RenderInput *)winMain, 0, 96);

                for (j = 0; j <= 3; j++) {
                    s32 pal;

                    pal = 1;
                    if (extended != 0) {
                        pal = 0;
                    }
                    UiWindow_SetTilemapEntry(
                        (struct RenderInput *)winMain, 0x5001 + j, 7 + j * 4, pal + 8, 0);
                    if (extended != 0) {
                        UiText_DrawNumberInWindow(object->djinn_active_counts[j], 1,
                            (struct RenderInput *)winMain, 40 + j * 32, 72);
                        UiText_DrawStringAtOffset(
                            (u8 *)OwnerStatus_SlashString, (struct RenderInput *)winMain, 48 + j * 32, 72);
                        UiText_DrawNumberInWindow(object->djinn_owned_counts[j], 1,
                            (struct RenderInput *)winMain, 48 + j * 32 + 8, 72);
                    }
                    UiText_DrawNumberInWindow(Owner_GetResistanceValueFar(owner, j), 2,
                        (struct RenderInput *)winMain, 48 + j * 32, 80);
                    UiText_DrawNumberInWindow(
                        object->elm[j].pow, 3, (struct RenderInput *)winMain,
                        40 + j * 32, 88);
                    UiText_DrawNumberInWindow(
                        object->elm[j].res, 3, (struct RenderInput *)winMain,
                        40 + j * 32, 96);
                }

                i = 0;
                if (object->hp == 0) {
                    sel[0] = 16;
                    i = 1;
                }
                do {
                    if (object->restraint != 0) {
                        sel[i++] = 15;
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->shiryo != 0) {
                        sel[i++] = 8;
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->doku == 1) {
                        sel[i++] = 1;
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->doku == 2) {
                        sel[i++] = 2;
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->fuuji != 0) {
                        sel[i++] = 4;
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->shibire != 0) {
                        sel[i++] = 3;
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->nemuri != 0) {
                        sel[i++] = 5;
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->akuryo != 0) {
                        sel[i++] = 7;
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->maboroshi != 0) {
                        sel[i++] = 6;
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->atk_cnt != 0) {
                        if (object->atk_lv > 0) {
                            sel[i++] = 9;
                        }
                        if (object->atk_lv < 0) {
                            sel[i++] = 10;
                        }
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->def_cnt != 0) {
                        if (object->def_lv > 0) {
                            sel[i++] = 11;
                        }
                        if (object->def_lv < 0) {
                            sel[i++] = 12;
                        }
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->res_cnt != 0) {
                        if (object->res_lv > 0) {
                            sel[i++] = 13;
                        }
                        if (object->res_lv < 0) {
                            sel[i++] = 14;
                        }
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->agi_lv > 0) {
                        sel[i++] = 17;
                    }
                    if (object->agi_lv < 0) {
                        sel[i++] = 18;
                    }
                } while (0);

                for (j = 0; j < i; j++) {
                    Resource_LoadTableEntryToBuffer(sel[j], tbl[j]);
                }
                if (i == 0) {
                    sel[0] = 0;
                    i = 1;
                }
                if (i <= 10) {
                    for (j = i; j < 11; j++) {
                        sel[j] = -1;
                    }
                }
                cnt = i;
                if (sel[0] == 0) {
                    UiText_DrawCharacterAtOffset(
                        object->hp != 0 ? (s32)MsgOwnerStatusNormal : (s32)MsgOwnerStatusDowned,
                        (struct RenderInput *)winMain, 112, 0);
                }
            }

            text = (u16 *)MemAlloc(OWNER_STATUS_TEXT_MAX * sizeof(*text));
            if ((u32)cur.entry > 13) {
                iconIdx = sel[cur.entry - 14];
                if (iconIdx == 0 && object->hp == 0) {
                    iconIdx = 16;
                }

                saved = (struct BtlUnit *)MemAlloc(sizeof(*object));
                code = 0;
                Iwram_CopyWords(saved, object, sizeof(*object));

                atkDelta = object->atk;
                defDelta = object->def;
                agiDelta = object->agi;
                object->atk_lv = 0;
                object->def_lv = 0;
                object->agi_lv = 0;
                UnitCalcFar(owner);
                atkDelta -= object->atk;
                defDelta -= object->def;
                agiDelta -= object->agi;
                Iwram_CopyWords(object, saved, sizeof(*object));
                MemFree(saved);

                switch (iconIdx) {
                case 8:
                    code = object->shiryo;
                    break;
                case 9:
                    code = atkDelta;
                    break;
                case 10:
                    code = -atkDelta;
                    break;
                case 11:
                    code = defDelta;
                    break;
                case 12:
                    code = -defDelta;
                    break;
                case 17:
                    code = agiDelta;
                    break;
                case 18:
                    code = -agiDelta;
                    break;
                case 13:
                case 14:
                    code = object->res_lv * 20;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
                    if (code < 0) {
                        code = -code;
                    }
#endif
                    break;
                default:
                    break;
                }
                UiWork_PushValueSlot(code, 5);
                UiText_CopyMessageString(iconIdx + (s32)MsgOwnerStatusConditionHelp, text, OWNER_STATUS_TEXT_MAX);
            } else if (cur.entry == 2 && object->level <= 98) {
                UiWork_PushValueSlot(
                    Owner_GetLevelThresholdFar(owner, object->level + 1)
                        - object->experience,
                    5);
                UiText_CopyMessageString((s32)MsgOwnerStatusNextLevel, text, OWNER_STATUS_TEXT_MAX);
            } else {
                UiText_CopyMessageString(cur.entry + (s32)MsgOwnerStatusEntryHelp, text, OWNER_STATUS_TEXT_MAX);
            }

            UiText_RenderWideStringAtOffset(text, (struct UiWindow *)winDesc, 0, 4);
            MemFree(text);
            redraw = 0;
            base->dirty = 1;
        }

        p = objIcon;
        q = tbl;
        {
            struct VramBlockCacheEntry *vc;

            for (i = 0, vc = gVramBlockCache, x = 112; i <= 10; i++) {
                p->oam.word.attr01 = 0x40000400;
                p->oam.word.attr23 = 0;
                p->oam.f.tile = vc[*q++].offset >> 5;
                p->oam.f.x = x;
                p->oam.f.y = (u8)(winMain->y * 8 + 8);
                if (sel[i] > 0) {
                    Runtime_PushSlotEntry((s32 *)p, 240);
                }
                x += 15;
                p++;
            }
        }

        if (gLinkCountdownWork->active == 0) {
            break;
        }
        if ((gKeyState & KEY_B) != 0) {
            break;
        }

        if (listCount != 0) {
            if ((keys & 0x100) != 0) {
                idx++;
                if (idx >= listCount) {
                    idx = 0;
                }
                owner = list[idx];
                redraw = 1;
                AudioCommand_PlayFar(0x6F);
            } else if ((keys & 0x200) != 0) {
                idx--;
                if (idx < 0) {
                    idx = listCount - 1;
                }
                owner = list[idx];
                redraw = 1;
                AudioCommand_PlayFar(0x6F);
            }
        }
        WaitFrames(1);
    }

    {
        s32 *h = tbl;

        for (i = 10; i >= 0; i--) {
            Resource_ResetEntry(*h++);
        }
    }
    Resource_ResetEntry(handleFrame);
    Resource_ResetEntry(handleCursor);
    WaitFrames(1);
#if EDITION_INTERNATIONAL
    UiWindow_MarkVisibleTileAttributes();
#endif
    UiWork_Finalize(winMain, 1);
    UiWork_Finalize(winDesc, 1);
    PtyStDraw(BtlWk->pty_st);
    ((struct MenuCell **)&BtlWk)[HEAP_SLOT_WINDOW + 42 - HEAP_SLOT_BATTLE]->busy = 0;
    WaitFrames(1);
    return 0;
}

#if EDITION_INTERNATIONAL
#define TEXT_COUNT 52
#define TEXT_LOAD UiText_CopyMessageString
#define TURNS_DIGITS 1
#define TURNS_X 48
#else
#define TEXT_COUNT 32
#define TEXT_LOAD UiText_DecodeMessage
#define TURNS_DIGITS 2
#define TURNS_X 40
#endif

struct BattleMenuSelection {
    struct MenuCell menu;
    u32 selected[8];
};

LAYOUT_SIZE_GUARD(BattleMenuSelection_Size, struct BattleMenuSelection, 0x104);
LAYOUT_OFFSET_GUARD(BattleMenuSelection_Selected, struct BattleMenuSelection, selected, 0xe4);

#define MENU_STATE ((struct BattleMenuSelection *)gLinkCountdownWork)
#define CURSOR_OWNER (*(struct PartyStatusWindow **)gPartyStatusWork)

extern volatile u32 gKeysHeld;
extern const u8 DjinnMenu_BlinkTile[32];
extern const u8 DjinnMenu_ElementXOffsets[4];
extern const u8 DjinnMenu_ElementYOffsets[4];

void RenderOutput_ClearList(void *work);
void UiWork_SetAltFlagAndClearTable(s32 flag);
struct DjinnRecoveryTable *Trade_GetOfferStateFar(s32 side);

s32 Battle_SelectAbility(s32 owner, s32 unused)
{
    struct CursorSprite slot;
    u16 buf[64];
    struct UiRenderWork *ctx;
    struct UiWindow *detail_win;
    s32 prev_row;
    s32 res;
    u32 *tbl;
    s32 cnt;
    s32 prev_page;
    struct UiWindow *msg_win;
    s32 mask;
    s32 saved_row;
    s32 sub_sel;
    s32 redraw;
    s32 is_used;
    s32 sub_cnt;
    struct BattleMenuSelection *menu;
    struct BtlUnit *unit;
    struct DjinnRecoveryList *list;
    struct UiWindow *list_win;
    struct UiWindow *icon_win;
    struct PartyStatusWindow *cursor;
    u32 *pos;
    u32 cur;
    u32 blink;
    s32 page;
    s32 row;
    u32 code;
    s32 pages;
    s32 pal;
    struct MenuPoint cursor_pos;
    s32 trig;
    s32 rep;
    s32 opts;
    s32 x;
    s32 y;
    s32 i;
    s32 j;
    struct CursorSprite *cur_spr;

    ctx = (struct UiRenderWork *)gWindowWork[0];
    detail_win = 0;
    prev_row = -1;
    res = Resource_LoadIntoFreeSlot(0x80);
    tbl = (u32 *)Runtime_BumpAllocateAlternatePool(0xa8 * 2);
    cnt = 0;
    prev_page = -1;
    code = 0;
    mask = 0;
    sub_sel = 0;
    sub_cnt = 0;
    redraw = 0;
    is_used = 0;
    msg_win = (struct UiWindow *)UiWindow_Create(0, 4, 30, 4, 42);
    UiWork_SetAltFlagAndClearTable(1);
    list_win = (struct UiWindow *)UiWindow_Create(21, 9, 9, 11, 6);
    /* FAKEMATCH: needed to match: a one-pass block restores the saved row before the owner lookup */
    do {
        page = ((struct MenuCell *)gWindowWork[42])->page;
        row = ((struct MenuCell *)gWindowWork[42])->row;
        saved_row = ((struct MenuCell *)gWindowWork[42])->preferred_row;
    } while (0);
    unit = UnitGetFar(owner);

    for (x = 0; x <= 3; x++) {
        for (y = 0, pos = (u32 *)((cnt << 2) + (u32)tbl); y <= 19; y++) {
            if (unit->djinn_active[x] & (u32)(1 << y)) {
                *pos = (u32)((x << 8) | y);
                pos++;
                cnt++;
            } else if (unit->djinn_available[x] & (u32)(1 << y)) {
                u32 v;
                s32 k;
                list = &Trade_GetOfferStateFar((u32)owner > 7 ? 1 : 0)->list;
                for (k = 0; k < list->count; k++) {
                    if (list->entries[k].unit_id == owner && list->entries[k].element == x &&
                        list->entries[k].index == y) {
                        break;
                    }
                }
                v = (u32)((x << 8) | y | 0x10000);
                *pos = v;
                if (list->entries[k].turns > 0) {
                    v |= (u32)(list->entries[k].turns << 17);
                    *pos = v;
                }
                cnt++;
                pos++;
            }
        }
    }
    tbl[cnt] = 0x80000000;
    ctx->dirty = 1;

    cur_spr = &slot;
    for (;;) {
        if (page != prev_page || row != prev_row || redraw != 0) {
            cur = tbl[page + row];
            mask = 0;
            ctx->menu_busy = 1;
            Ui_SetRectHighlight(list_win->x + 1, list_win->y + prev_row * 2 + 1,
                          list_win->width - 2, 1, 15);
            if (sub_sel != 0) {
                UiWork_Finalize(msg_win, 1);
                msg_win = (struct UiWindow *)UiWindow_Create(0, 4, 30, 4, 42);
                Ui_FillVramBlockPattern();
            }
            redraw = 0;
            if (cnt != 0) {
                s32 k;
                UiWork_ClearValueNameTables();
                /* FAKEMATCH: restores a CSE1 block end the bytes prove (k = redraw survives as the native ldr r1, [sp, #28]): an empty one-pass block, without which CSE1 deletes the second redraw = 0 below as a no-op and folds k to 0 (29 rows) */
                do { } while (0);
                is_used = 0;
                /* FAKEMATCH: restores redraw set in the search block, which the surviving copy proves gcse saw: cprop leaves k = redraw alone only when its source is set earlier in its block, and cse2 deletes this store as a no-op (29 rows without) */
                redraw = 0;
                /* FAKEMATCH: restores a CSE block that ended where no code is left: this volatile asm flushes cse2, so k = redraw stays the native ldr r1, [sp, #28] instead of folding to 0 (29 rows without) */
                asm("" :);
                k = redraw;
                if ((u32)MENU_STATE->selected[k] == cur) {
                    is_used = 1;
                } else {
                    for (;;) {
                        /* FAKEMATCH: needed to match: is_used is zero on every search-loop continuation. Its step dependency prevents loop.c from strength-reducing k into a pointer; CSE later removes the flag term. Plain k differs 35 instructions in EN; this form restores the native reload and indexing (differ 0). */
                        k += 1 - is_used;
                        if (k > 7) break;
                        if ((u32)MENU_STATE->selected[k] == cur) {
                            is_used = 1;
                            break;
                        }
                    }
                }
                if (is_used != 0) {
                    TEXT_LOAD((s32)MsgBattleDjinnTwice, buf, TEXT_COUNT);
                    if (detail_win != 0) {
                        UiWork_Finalize(detail_win, 1);
                        detail_win = 0;
                        sub_sel = 0;
                    }
                } else if (cur & 0x10000) {
                    if (cur & 0x3e0000) {
                        UiWork_PushValueSlot((s32)((cur & 0x3e0000) >> 17), 5);
                        TEXT_LOAD((s32)(((cur & 0xf00) >> 8) * 20 + (cur & 0xff)) + (s32)MsgDjinnEffect, buf, TEXT_COUNT);
                        if (detail_win != 0) {
                            UiWork_Finalize(detail_win, 1);
                            detail_win = 0;
                            sub_sel = 0;
                        }
                    } else {
                        detail_win = (struct UiWindow *)DjinnMenu_ShowChangePreview((struct RenderInput *)detail_win, owner, (s32)cur, sub_sel, &sub_cnt);
                        UiText_CopyMessageString((s32)MsgBattleSummonStandby, buf, TEXT_COUNT);
                        mask = 1 << ((cur & 0xf00) >> 8);
                    }
                } else {
                    detail_win = (struct UiWindow *)DjinnMenu_ShowChangePreview((struct RenderInput *)detail_win, owner, (s32)cur, sub_sel, &sub_cnt);
                    TEXT_LOAD((s32)(((cur & 0xf00) >> 8) * 20 + (cur & 0xff)) + (s32)MsgDjinnEffect, buf, TEXT_COUNT);
                }
            } else {
                TEXT_LOAD((s32)MsgBattleNoDjinn, buf, TEXT_COUNT);
            }
            ctx->menu_busy = 0;
            if (sub_sel == 0) {
                ctx->menu_busy = 1;
                UiWork_Finalize(msg_win, 1);
                msg_win = (struct UiWindow *)UiWindow_Create(0, 4, 30, 4, 42);
                ctx->menu_busy = (u8)sub_sel;
            }
            UiText_RenderWideStringAtOffset(buf, msg_win, 0, 4);
            prev_row = row;
            if (page != prev_page) {
                RenderOutput_RedrawSavedRect(list_win);
                j = 0;
                while ((code = tbl[page + j]) != 0x80000000) {
                    UiWindow_SetTilemapEntry((struct RenderInput *)list_win, (s32)((code & 0xf00) >> 8) + 0x5001, 0, j * 2, 0);
                    if (code & 0x3e0000) {
                        UiWork_SetParamNibble(4);
                    } else if (code & 0x10000) {
                        UiWork_SetParamNibble(2);
                    }
                    UiText_DrawCharacterAtOffset((s32)(((code & 0xf00) >> 8) * 20 + (code & 0xff)) + (s32)MsgDjinnName, (struct RenderInput *)list_win, 8, j * 16);
                    if (code & 0x3e0000) {
                        UiText_DrawNumberAtOffset((s32)((code & 0x3e0000) >> 17), TURNS_DIGITS, (struct RenderInput *)list_win, TURNS_X, j * 16);
                    }
                    UiWork_SetParamNibble(15);
                    j++;
                    if (j > 4) {
                        break;
                    }
                }
                prev_page = page;
            }
            if (cnt > 5) {
                for (j = 0; j < (pages = ((cnt + 4) / 5)); j++) {
                    s32 tile = j + 0xf301;
                    if (j == (page / 5)) {
                        tile = j + 0xf30b;
                    }
                    UiWindow_SetTilemapEntry((struct RenderInput *)list_win, tile, list_win->width - pages + j - 2, -1, 0);
                }
            }
            Ui_SetRectHighlight(list_win->x + 1, list_win->y + row * 2 + 1,
                          list_win->width - 2, 1, 14);
            ctx->dirty = 1;
            ctx->menu_busy = 0;
        }

        if (cnt > 5) {
            for (j = 0; j < (cnt + 4) / 5; j++) {
                s32 tile = j + 0xf301;
                if ((gKeysHeld & 0x100) || (gFrameCount & 15) <= 11) {
                    if (j == (page / 5)) {
                        tile = j + 0xf30b;
                    }
                }
                UiWindow_SetTilemapEntry((struct RenderInput *)list_win, tile,
                              list_win->width - ((cnt + 4) / 5) + j - 2, -1, 0);
            }
            if ((gKeysHeld & 0x100) == 0) {
                UiWindow_SetTilemapEntry((struct RenderInput *)list_win, 0xf334, list_win->width - (cnt + 4) / 5 - 3, -1, 0);
                UiWindow_SetTilemapEntry((struct RenderInput *)list_win, 0xf335, list_win->width - 2, -1, 0);
            } else {
                UiWindow_SetTilemapEntry((struct RenderInput *)list_win, 0xf011, list_win->width - (cnt + 4) / 5 - 3, -1, 0);
                UiWindow_SetTilemapEntry((struct RenderInput *)list_win, 0xf012, list_win->width - 2, -1, 0);
            }
            ctx->dirty |= (u8)(2 << ((u32)(list_win->y - 1) >> 2));
        }

        trig = gKeyState;
        rep = gKeysRepeat;
        opts = (s32)gKeysHeld;
        menu = MENU_STATE;
        if (menu->menu.auto_enabled != 0) {
            rep = 0;
            trig = 0;
            opts = 0;
            if (menu->menu.auto_delay == 0) {
                menu->menu.auto_delay = 60;
                rep = 1;
                trig = 1;
            } else {
                menu->menu.auto_delay = menu->menu.auto_delay - 1;
            }
        }
        menu = MENU_STATE;
        if (menu->menu.active == 0 || (trig & 2)) {
            AudioCommand_PlayFar(0x71);
            code = (u32)-1;
            break;
        }
        if (trig & 1) {
            u32 sel;
            u32 turns;
            if (cnt != 0) {
                sel = tbl[page + row];
                turns = sel & 0x3e0000;
                if (turns == 0) {
                    if (is_used == 0) {
                        menu->menu.page = page;
                        menu->menu.row = row;
                        menu->menu.preferred_row = saved_row;
                        code = sel;
                        break;
                    }
                } else if (is_used == 0) {
                    Ui_FillVramBlockPattern();
                    UiWork_ClearValueNameTables();
                    UiWork_PushValueSlot((s32)((turns & 0x3e0000) >> 17), 5);
                    UiText_CopyMessageString((s32)MsgBattleDjinnRecovering, buf, TEXT_COUNT);
                    UiText_RenderWideStringAtOffset(buf, msg_win, 0, 4);
                }
                AudioCommand_PlayFar(0x72);
            } else {
                code = (u32)-1;
                break;
            }
        }

        if (cnt != 0) {
            if (rep & 0x80) {
                AudioCommand_PlayFar(0x6f);
                row++;
                if (row == 5 || page + row == cnt) {
                    row = 0;
                }
                saved_row = row;
            } else if (rep & 0x40) {
                AudioCommand_PlayFar(0x6f);
                row--;
                if (row < 0) {
                    if (page == ((cnt - 1) / 5) * 5) {
                        row = cnt - page - 1;
                    } else {
                        row = 4;
                    }
                }
                saved_row = row;
            } else if (opts & 0x100) {
                if (detail_win != 0) {
                    for (i = 0; i < sub_cnt; i++) {
                        s32 tile = i + 0xf301;
                        if ((gFrameCount & 15) <= 11 && i == sub_sel - 1) {
                            tile = sub_sel - 1 + 0xf30b;
                        }
                        UiWindow_SetTilemapEntry((struct RenderInput *)detail_win, tile,
                                      detail_win->width - sub_cnt + i - 2, -1, 0);
                    }
                    UiWindow_SetTilemapEntry((struct RenderInput *)detail_win, 0xf334, detail_win->width - sub_cnt - 3, -1,
                                  0);
                    UiWindow_SetTilemapEntry((struct RenderInput *)detail_win, 0xf335, detail_win->width - 2, -1, 0);
                    /* FAKEMATCH: restores a block end the bytes prove (the 0xf335 arguments are scheduled as if the dirty update were not in their block): an empty one-pass block whose loop notes make the dirty update a sched2 barrier, so the post-call writes of r1, r2 and r3 stop counting as dependants and subs r2, #2 follows the r0 copy as native (4 rows without) */
                    do { } while (0);
                    ctx->dirty |= (u8)(2 << (detail_win->y >> 2));
                }
                if (sub_sel == 0 && sub_cnt != 0) {
                    if (detail_win != 0) {
                        RenderOutput_ClearList(detail_win);
                    }
                    sub_sel = 1;
                    redraw = 1;
                } else {
                    if (sub_sel > sub_cnt) {
                        sub_sel = sub_cnt;
                    }
                    if (sub_sel != 0) {
                        if (rep & 16) {
                            AudioCommand_PlayFar(0x6f);
                            sub_sel++;
                            if (sub_sel > sub_cnt) {
                                sub_sel = 1;
                            }
                            redraw = 1;
                        } else if (rep & 32) {
                            AudioCommand_PlayFar(0x6f);
                            sub_sel--;
                            if (sub_sel <= 0) {
                                sub_sel = sub_cnt;
                            }
                            redraw = 1;
                        }
                    }
                }
            } else if (sub_sel != 0) {
                if (detail_win != 0) {
                    RenderOutput_ClearList(detail_win);
                }
                sub_sel = 0;
                redraw = 1;
            } else if (rep & 16) {
                AudioCommand_PlayFar(0x6f);
                Runtime_SetMainState19();
                if (page + 5 >= cnt) {
                    if (page != 0) {
                        page = 0;
                        row = saved_row;
                    }
                } else {
                    page = page + 5;
                    row = saved_row;
                    if (page == ((cnt - 1) / 5) * 5) {
                        row = cnt - page - 1;
                        if (row > saved_row) {
                            row = saved_row;
                        }
                    }
                }
            } else if (rep & 32) {
                AudioCommand_PlayFar(0x6f);
                Runtime_SetMainState19();
                if (page != 0) {
                    page -= 5;
                    row = saved_row;
                } else {
                    page = ((cnt - 1) / 5) * 5;
                    row = saved_row;
                    if (page != 0) {
                        row = cnt - page - 1;
                        if (row > saved_row) {
                            row = saved_row;
                        }
                    }
                }
            }
        }

        {
            s32 cx = list_win->x * 8;

            /* FAKEMATCH: instruction order: this barrier keeps the reload of row for row * 2 (mov r2, r10) after lsls; without it the reload is scheduled in the ldrh delay slot (2 rows) */
            asm("");
            /* FAKEMATCH: instruction order: row read in a low register here is reloaded before the subtract, and row * 2 inherits that reload, so mov r2, r10 precedes subs r3, #2 as native (2 rows without; equal priority, and the reload otherwise has the later luid) */
            asm("" : : "l"(row));
            cursor_pos.x = cx - 2;
        }
        cursor_pos.y = (row * 2 + list_win->y) * 8 + 20;
        cur_spr = &slot;
        ((u32 *)cur_spr)[1] = MENU_SPRITE_SIZE_16;
        ((u32 *)cur_spr)[2] = 0;
        cur_spr->tile = Resource_GetBuffer(res, (s32)Resource_FixedBlockBTiles);
        cur_spr->x = cursor_pos.x + ((gFrameCount & 4) >> 1) - 6;
        cur_spr->y = cursor_pos.y - ((gFrameCount & 4) >> 2) - 8;
        if (cnt != 0) {
            Runtime_PushSlotEntry(cur_spr, 242);
        }
        cursor = CURSOR_OWNER;
        blink = gFrameCount & 4;
        icon_win = cursor->window;
        if (cursor->flags & 2) {
            for (i = 0; i <= 3; i++) {
                pal = 15 - (blink != 0);
                if (((1 << i) & mask) == 0) {
                    pal = 15;
                }
                Ui_SetRectHighlight(icon_win->x + DjinnMenu_ElementXOffsets[i] + 1,
                              icon_win->y + DjinnMenu_ElementYOffsets[i] + 1, 2, 2, pal);
            }
        }
        if (gFrameCount & 4) {
            Iwram_CopyWords((void *)0x06006500, DjinnMenu_BlinkTile, 32);
            Iwram_CopyWords((void *)0x06006520, DjinnMenu_BlinkTile, 32);
        } else {
            Iwram_FillWords((void *)0x06006500, 32, 0x44444444);
            Iwram_CopyWords((void *)0x06006520, DjinnMenu_BlinkTile, 32);
        }
        WaitFrames(1);
    }

    if (CURSOR_OWNER->flags & 2) {
        Ui_SetRectHighlight(CURSOR_OWNER->window->x + 1, CURSOR_OWNER->window->y + 1, 4, 4, 15);
    }
    Resource_ResetEntry(res);
    UiWork_Finalize(msg_win, 1);
    UiWork_Finalize(detail_win, 1);
    UiWork_Finalize(list_win, 1);
    UiWindow_MarkVisibleTileAttributes();
    UiWork_SetAltFlagAndClearTable(0);
    MemFree(tbl);
    WaitFrames(1);
    return (s32)code;
}

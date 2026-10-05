/* 2026-10-05 sun deep finisher 21: alchemy diff 378 -> 335.
 * The party-panel pointer is its own IWRAM cell, Data_03001e90 (the
 * native pool names it, read at offset 0), not gWindowWork[1]: 376. The
 * exit reads it again directly instead of reusing the loop's cursor and
 * icon_win, so both become block-local (r3, as natively): 364. The
 * confirm handler is if (cnt != 0) { ... PlayFar(0x72); } else { ret = -1;
 * break; }, which places the -1 block after the handler as natively: 335.
 * Tried without change: two separate timer/is_used ifs, an explicit
 * second 0x3e0000 mask (both fold away; the native's redundant
 * `ands r5, r6` in the timer path is still unexplained).
 */
/* 2026-10-05 sun deep finisher 19: alchemy diff 869 -> 378 (frame 224).
 * The spill slots follow declaration order, so the declarations now run
 * slot, buf, then ctx, detail_win, prev_row, res, tbl, cnt, prev_page,
 * msg_win, mask, saved_row, sub_sel, redraw, is_used as the native frame
 * places them (76 down to 24; sub_cnt's address takes 80). The cursor is a
 * CursorSprite reached through cur_spr, pointed at it before the loop and
 * again each frame, as ItemList_SelectEntry does: the native keeps that
 * address in the lowest slot ([sp+4]) and builds its fields with word
 * masks. The recovery-list search and the used-code search have their own
 * block-local index k (r1 in the native, not the long-lived i), the page
 * redraw walks its own pointer entry (r8) instead of the table's pos (r5),
 * the confirm handler reads its own code (r6; cur stays r5 in the redraw),
 * and the blinking page loop recomputes (cnt + 4) / 5 instead of storing
 * pages, so the arrows reuse the loop's last quotient in r0.
 * Measured on the way: declarations reordered 739, CursorSprite 712, block
 * index 679, redraw pointer without the re-pointed cursor 744 (frame 220:
 * the cursor address lost its slot), with it 434, page quotient 415,
 * confirm code 378. mask = 0 after menu_busy 444 (kept before); pos as
 * tbl + cnt unchanged; 800 random declaration moves found nothing.
 * Remaining: the five zero stores at entry take a fresh r6 = 0 natively
 * (here the cnt zero and detail_win's slot); k starts from redraw's slot
 * natively; the used and recovering message paths then share one
 * CopyMessageString/Finalize tail here (the reload round robin differs),
 * and the row-status SetParamNibble calls likewise; later register choices
 * in the detail-window and input blocks follow from these.
 */
/* 2026-10-04 H2 data-owner/flow pass: current EN score 11030
 * (197 register, 60 stack, 25 operand, 60 reordered, 32 inserted, 25 deleted).
 * All six TBS plans compile complete 2760-byte objects with a 224-byte frame;
 * native extent 2756 includes all pools and also has a 224-byte frame.
 *
 * gWindowWork[42] and gLinkCountdownWork name the same EN pointer cell.
 * Native setup uses the former's base plus 168; later selected/auto/commit
 * paths load the named cell directly. Following those real accesses gives
 * 11085 from 13161, removing 16 inserted instructions, without new state.
 * Separate consumed build/row code locals give 11030, retained.
 * Runtime_BumpFree is the existing producer name paired with allocation.
 *
 * DjinnMenu_BlinkTile is the 32-byte BG tile copied to both adjacent VRAM
 * tiles, the first alternating with palette index 4. ElementXOffsets and
 * ElementYOffsets are four-byte tables consumed by the four highlight calls.
 * Names remain at their existing scaffold owner; this adds no data credit.
 * Independent current-tree/owned-ROM review found only this function's
 * three literals in main and no pointers in 96 active EN overlays.
 *
 * Ordinary trials from 11085: selection scope 11748; destructive message-code
 * consumption 11175; reversed row flags 11405; scoped consumption 11838.
 * From 11030: bounded row for-loop 11190; consumed timer local 11105;
 * flat available/timer branches 11030, no improvement.
 * Native 84 nonlocal calls versus candidate 81: the used/recovering paths
 * merge one CopyMessageString and Finalize pair; the row status paths merge
 * one SetParamNibble call. These are duplicated native sites, not omitted
 * source behavior. No barrier or other instruction-presence device added.
 * Broad register/stack/control/pool differences keep this draft uncredited.
 */

/* Earlier H1 owner/API pass: EN score13161 (214 register,
 * 61 stack,29 operand,61 reordered,48 inserted,28 deleted).
 * All six TBS plans compile complete2780-byte objects with a224-byte frame;
 * native extent2756 includes every pool, with the same224-byte frame.
 * The former ordinary draft scored13657 and had a 216-byte frame.
 *
 * Shared UiWindow/MenuSprite/UiRenderWork, MenuCell, BattleUnit and
 * DjinnRecoveryList replace duplicate views. Current WINDOW15.C owns the
 * 16-byte party-panel shape retained locally here. The command collector
 * allocates260 bytes and writes actor/result fields ate0/e4; that tail
 * remains a guarded local view, not an extension invented for the frame.
 * Its native call also supplies unused argumentr1=1, represented explicitly.
 * Input cells and IWRAM helpers use real declarations. No status semantics
 * are inferred for the ignored fill/copy results.
 *
 * Owner normalization alone:13925/frame216. Ordinary trials:
 * - C division instead of explicit __divsi3 calls:13785.
 * - Both real cursor coordinates as the existing MenuPoint:13306.
 * - Both together:13161/frame224, retained. Each coordinate is consumed;
 *   the aggregate recovers the eight frame bytes without unread storage.
 * No new asm, binding, compiler option or instruction device was added.
 * Remaining broad allocation/control/pool differences keep this uncredited.
 * H2 above supplies the three formerly anonymous data names.
 */
#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "MENU_LIST.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_PARTY.H"
#include "IWRAM_CALL.H"

/*
 * Battle Djinn picker (owner main:08023e70, 2756 bytes).
 *
 * Builds a flat table of the acting unit's selectable Djinn out of two
 * 4 x 20 bit grids in the unit record, then runs a modal five-per-page list
 * until the player confirms an entry or cancels.  The return value is the
 * packed table code of the confirmed entry, or -1 on cancel.
 *
 * Packed table code, as used by every draw and commit site below:
 *   bits  0.. 7  row within the grid column (0..19)
 *   bits  8..11  grid column (0..3)
 *   bit  16      entry came from the second grid (djinn_available), so it carries
 *                a placement record and a detail sub-window
 *   bits 17..21  placement timer, when positive
 *   bit  31      table terminator (0x80000000)
 *
 * The single `bl` back into the middle of the owner at 0x0802489c is the
 * out-of-range back edge of the outer `for (;;)`, not a call.
 *
 * The shared 12-byte MenuSprite passed to Runtime_PushSlotEntry exposes
 * its two OAM words and the real narrow fields. The +0xfffa and +248
 * cursor biases preserve the native wrapped hardware coordinates.
 *
 * Earlier scalar-draft allocation trials had a216-byte frame and missing
 * duplicated call sites. H1 recovered the real consumed MenuPoint; H2
 * above records the current frame and the precise remaining call merges.
 */

/* The command collector owns this 260-byte state. The common menu prefix
 * ends at 0xe0; the actor and eight selected Djinn codes are written by
 * Battle_CollectPartyCommands and read here to reject duplicate choices. */
struct BattleMenuSelection {
    struct MenuCell menu;
    s32 actor_id;
    u32 selected[8];
};

LAYOUT_SIZE_GUARD(BattleMenuSelection_Size, struct BattleMenuSelection, 0x104);
LAYOUT_OFFSET_GUARD(BattleMenuSelection_Selected, struct BattleMenuSelection, selected, 0xe4);

/* Same producer-owned record as WINDOW15.C. It describes the party panel,
 * not a cursor sprite; leave this local until that owner is shared. */
struct PartyStatusWindow {
    struct UiWindow *window;
    u16 x;
    u16 y;
    u16 width;
    u16 height;
    u16 flags;
};

LAYOUT_SIZE_GUARD(PartyStatusWindow_Size, struct PartyStatusWindow, 16);
LAYOUT_OFFSET_GUARD(PartyStatusWindow_Flags, struct PartyStatusWindow, flags, 12);

/* Direct name for the same work cell accessed through slot 42 at setup. */
#define MENU_STATE ((struct BattleMenuSelection *)gLinkCountdownWork)
extern u8 Data_03001e90[];
#define CURSOR_OWNER (*(struct PartyStatusWindow **)Data_03001e90)

extern volatile u32 gKeysHeld;
/* One 8x8 BG tile; the first copy blinks against palette-index 4. */
extern const u8 DjinnMenu_BlinkTile[32];
extern const u8 DjinnMenu_ElementXOffsets[4];
extern const u8 DjinnMenu_ElementYOffsets[4];

void RenderOutput_ClearList(void *work);
void UiWork_ClearValueNameTables(void);
void UiWork_PushValueSlot(s32 value, s32 width);
void UiWindow_MarkVisibleTileAttributes(void);
void UiWork_SetAltFlagAndClearTable(s32 flag);
struct UiWindow *DjinnMenu_ShowChangePreview(struct UiWindow *window, s32 owner,
    u32 code, s32 selection, s32 *count);
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
    struct BattleUnit *unit;
    struct DjinnRecoveryList *list;
    struct UiWindow *list_win;
    struct UiWindow *icon_win;
    struct PartyStatusWindow *cursor;
    u32 *pos;
    u32 *entry;
    u32 cur;
    u32 blink;
    s32 page;
    s32 row;
    s32 ret;
    s32 pages;
    s32 tile;
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
    mask = 0;
    sub_sel = 0;
    sub_cnt = 0;
    redraw = 0;
    is_used = 0;
    msg_win = UiWindow_Create(0, 4, 30, 4, 42);
    UiWork_SetAltFlagAndClearTable(1);
    list_win = UiWindow_Create(21, 9, 9, 11, 6);
    page = ((struct MenuCell *)gWindowWork[42])->page;
    row = ((struct MenuCell *)gWindowWork[42])->row;
    saved_row = ((struct MenuCell *)gWindowWork[42])->preferred_row;
    unit = Owner_GetStateFar(owner);

    /* Flatten the active and available Djinn grids into one page-ordered table. */
    for (x = 0; x <= 3; x++) {
        pos = &tbl[cnt];
        for (y = 0; y <= 19; y++) {
            if (unit->djinn_active[x] & (u32)(1 << y)) {
                *pos = (u32)((x << 8) | y);
                pos++;
                cnt++;
            } else if (unit->djinn_available[x] & (u32)(1 << y)) {
                u32 code;
                s32 k;
                list = &Trade_GetOfferStateFar((u32)owner > 7 ? 1 : 0)->list;
                for (k = 0; k < list->count; k++) {
                    if (list->entries[k].unit_id == owner && list->entries[k].element == x &&
                        list->entries[k].index == y) {
                        break;
                    }
                }
                code = (u32)((x << 8) | y | 0x10000);
                *pos = code;
                if (list->entries[k].turns > 0) {
                    code |= (u32)(list->entries[k].turns << 17);
                    *pos = code;
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
                msg_win = UiWindow_Create(0, 4, 30, 4, 42);
                Ui_FillVramBlockPattern();
            }
            redraw = 0;
            if (cnt != 0) {
                s32 k;
                UiWork_ClearValueNameTables();
                is_used = 0;
                for (k = 0; k <= 7; k++) {
                    if ((u32)MENU_STATE->selected[k] == cur) {
                        is_used = 1;
                        break;
                    }
                }
                if (is_used != 0) {
                    UiText_CopyMessageString(0x8ef, buf, 52);
                    if (detail_win != 0) {
                        UiWork_Finalize(detail_win, 1);
                        detail_win = 0;
                        sub_sel = 0;
                    }
                } else if (cur & 0x10000) {
                    if (cur & 0x3e0000) {
                        UiWork_PushValueSlot((s32)((cur & 0x3e0000) >> 17), 5);
                        UiText_CopyMessageString((s32)(((cur & 0xf00) >> 8) * 20 + (cur & 0xff)) + 0x666,
                                      buf, 52);
                        if (detail_win != 0) {
                            UiWork_Finalize(detail_win, 1);
                            detail_win = 0;
                            sub_sel = 0;
                        }
                    } else {
                        detail_win = DjinnMenu_ShowChangePreview(detail_win, owner, cur, sub_sel, &sub_cnt);
                        UiText_CopyMessageString(0x899, buf, 52);
                        mask = 1 << ((cur & 0xf00) >> 8);
                    }
                } else {
                    detail_win = DjinnMenu_ShowChangePreview(detail_win, owner, cur, sub_sel, &sub_cnt);
                    UiText_CopyMessageString((s32)(((cur & 0xf00) >> 8) * 20 + (cur & 0xff)) + 0x666,
                                  buf, 52);
                }
            } else {
                UiText_CopyMessageString(0x8ed, buf, 52);
            }
            ctx->menu_busy = 0;
            if (sub_sel == 0) {
                ctx->menu_busy = 1;
                UiWork_Finalize(msg_win, 1);
                msg_win = UiWindow_Create(0, 4, 30, 4, 42);
                ctx->menu_busy = (u8)sub_sel;
            }
            UiText_RenderWideStringAtOffset(buf, msg_win, 0, 4);
            prev_row = row;
            if (page != prev_page) {
                u32 code;
                RenderOutput_RedrawSavedRect(list_win);
                entry = &tbl[page];
                code = *entry;
                j = 0;
                while (code != 0x80000000) {
                    UiWindow_SetTilemapEntry(list_win, (s32)((code & 0xf00) >> 8) + 0x5001, 0, j * 2, 0);
                    if (code & 0x3e0000) {
                        UiWork_SetParamNibble(4);
                    } else if (code & 0x10000) {
                        UiWork_SetParamNibble(2);
                    }
                    UiText_DrawCharacterAtOffset((s32)(((code & 0xf00) >> 8) * 20 + (code & 0xff)) + 0x45f,
                                  (struct RenderInput *)list_win, 8, j * 16);
                    if (code & 0x3e0000) {
                        UiText_DrawNumberAtOffset((s32)((code & 0x3e0000) >> 17), 1, (struct RenderInput *)list_win, 48, j * 16);
                    }
                    UiWork_SetParamNibble(15);
                    j++;
                    if (j > 4) {
                        break;
                    }
                    entry++;
                    code = *entry;
                }
                prev_page = page;
            }
            if (cnt > 5) {
                for (j = 0; j < (pages = ((cnt + 4) / 5)); j++) {
                    tile = j + 0xf301;
                    if (j == (page / 5)) {
                        tile = j + 0xf30b;
                    }
                    UiWindow_SetTilemapEntry(list_win, tile, list_win->width - pages + j - 2, -1, 0);
                }
            }
            Ui_SetRectHighlight(list_win->x + 1, list_win->y + row * 2 + 1,
                          list_win->width - 2, 1, 14);
            ctx->dirty = 1;
            ctx->menu_busy = 0;
        }

        if (cnt > 5) {
            for (j = 0; j < (cnt + 4) / 5; j++) {
                tile = j + 0xf301;
                if ((gKeysHeld & 0x100) || (gFrameCount & 15) <= 11) {
                    if (j == (page / 5)) {
                        tile = j + 0xf30b;
                    }
                }
                UiWindow_SetTilemapEntry(list_win, tile,
                              list_win->width - ((cnt + 4) / 5) + j - 2, -1, 0);
            }
            if ((gKeysHeld & 0x100) == 0) {
                UiWindow_SetTilemapEntry(list_win, 0xf334, list_win->width - (cnt + 4) / 5 - 3, -1, 0);
                UiWindow_SetTilemapEntry(list_win, 0xf335, list_win->width - 2, -1, 0);
            } else {
                UiWindow_SetTilemapEntry(list_win, 0xf011, list_win->width - (cnt + 4) / 5 - 3, -1, 0);
                UiWindow_SetTilemapEntry(list_win, 0xf012, list_win->width - 2, -1, 0);
            }
            ctx->dirty |= (u8)(2 << ((u32)(list_win->y - 1) >> 2));
        }

        trig = gKeyState;
        rep = gKeysRepeat;
        opts = (s32)gKeysHeld;
        menu = MENU_STATE;
        if (menu->menu.auto_enabled != 0) {
            trig = 0;
            rep = 0;
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
            ret = -1;
            break;
        }
        if (trig & 1) {
            u32 code;
            if (cnt != 0) {
                code = tbl[page + row];
                if ((code & 0x3e0000) == 0) {
                    if (is_used == 0) {
                        menu->menu.page = page;
                        menu->menu.row = row;
                        menu->menu.preferred_row = saved_row;
                        ret = (s32)code;
                        break;
                    }
                } else if (is_used == 0) {
                    Ui_FillVramBlockPattern();
                    UiWork_ClearValueNameTables();
                    UiWork_PushValueSlot((s32)((code & 0x3e0000) >> 17), 5);
                    UiText_CopyMessageString(0x898, buf, 52);
                    UiText_RenderWideStringAtOffset(buf, msg_win, 0, 4);
                }
                AudioCommand_PlayFar(0x72);
            } else {
                ret = -1;
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
                        tile = i + 0xf301;
                        if ((gFrameCount & 15) <= 11 && i == sub_sel - 1) {
                            tile = sub_sel + 0xf30b;
                        }
                        UiWindow_SetTilemapEntry(detail_win, tile,
                                      detail_win->width - sub_cnt + i - 2, -1, 0);
                    }
                    UiWindow_SetTilemapEntry(detail_win, 0xf334, detail_win->width - sub_cnt - 3, -1,
                                  0);
                    UiWindow_SetTilemapEntry(detail_win, 0xf335, detail_win->width - 2, -1, 0);
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
                        row = saved_row;
                        page = 0;
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

        cursor_pos.x = list_win->x * 8 - 2;
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
    Runtime_BumpFree(tbl);
    WaitFrames(1);
    return ret;
}

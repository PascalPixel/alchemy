/* 08023e70: differ 145 of 1201. Trial: initializing the search from redraw instead of 0 is unchanged (145). Further trials: a consumed is_used constraint differs 183, empty barriers stay 145, and reversing the detail reset statements differs 150. Remaining: the native loads the is_used search index from the redraw slot (one extra reload) and keeps the 0x8ef branch separate; the reload registers after it rotate differently. */
/* MenuCell now includes its automatic-mode word at 0xe0; remove the unused duplicate tail word so selected remains at 0xe4 and total size at 0x104. */
/* Shared PartyStatusWindow from the verified STATUS.C corpus leaves differ 145. Further search trials: combined initialization, is_used as index, while-loop and memory clobber remain 145; do-while 166; index-first initialization 155; initialization before the clear call 148. Redraw memory/output constraints range 447-722. Clobbering r1 reaches 120 but emits mov-zero rather than the native reload, so it was removed. */
/* Stubborn (2026-10-06): CSE seeds the native search index from redraw's spilled zero (ldr r1, [sp, #28]); the draft keeps a fresh zero, and the reload rotation after the 0x8ef branch follows from it. */

#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "MENU_LIST.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_PARTY.H"
#include "IWRAM_CALL.H"

struct BattleMenuSelection {
    struct MenuCell menu;
    u32 selected[8];
};

LAYOUT_SIZE_GUARD(BattleMenuSelection_Size, struct BattleMenuSelection, 0x104);
LAYOUT_OFFSET_GUARD(BattleMenuSelection_Selected, struct BattleMenuSelection, selected, 0xe4);

#define MENU_STATE ((struct BattleMenuSelection *)gLinkCountdownWork)
extern u8 Data_03001e90[];
#define CURSOR_OWNER (*(struct PartyStatusWindow **)Data_03001e90)

extern volatile u32 gKeysHeld;
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
    msg_win = UiWindow_Create(0, 4, 30, 4, 42);
    UiWork_SetAltFlagAndClearTable(1);
    list_win = UiWindow_Create(21, 9, 9, 11, 6);
    page = ((struct MenuCell *)gWindowWork[42])->page;
    row = ((struct MenuCell *)gWindowWork[42])->row;
    saved_row = ((struct MenuCell *)gWindowWork[42])->preferred_row;
    unit = Owner_GetStateFar(owner);

    for (x = 0; x <= 3; x++) {
        pos = &tbl[cnt];
        for (y = 0; y <= 19; y++) {
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
                            sub_sel = 0;
                            detail_win = 0;
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
                    s32 tile = j + 0xf301;
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
                s32 tile = j + 0xf301;
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
                    UiText_CopyMessageString(0x898, buf, 52);
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
    return (s32)code;
}

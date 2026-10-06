/* Fifth pass (2026-10-06), still 55; causes narrowed from the sched2 priority tables (haifa rank_for_schedule: priority, then class against the last scheduled insn, then dependant count, then luid). Start schedule: movs r6, #0 (pseudo 59, code) and str r0, [sp, #56] (tbl) tie at priority 134 with five dependants each, so luid decides; the native order needs code's zero set before the pool allocation call (sched2 floats it past the call, since r6 is call-saved). Trial: code = 0 before the pool allocation or before res 54, but reload then inherits r6 for cnt = 0 (str r6, [sp, #52]; the native keeps movs r1, #0) and the gWindowWork loads swap; code = 0 before detail_win = 0 or first 71. So the native zero in r6 is set early yet hidden from find_equiv_reg at cnt's reload. Late subs #2 at 0xf335: all five argument insns tie at priority 43, and the native order (str, adds r0, subs r2, ldr r1, subs r3) is exactly what luid order gives when width - 2 sits at the r2 argument move (insn 2181), as if combine had merged it there; the draft keeps it at its precompute position (2170), before the stack store. Combine cannot merge it into the move as the call is written: combinable_i3pat refuses an argument register when the insn before it sets another argument register (sets_function_arg_p; here r1 = 0xf335), so the native's late subtract needs the value to reach the r2 move another way. cursor_pos.x - 2 looks the same: the native subtract follows mov r2, sl. Search index: reload_cse_simplify_set only offers constants and registers, never memory, so the native ldr r1, [sp, #28] must be k = (reg redraw) at reload; slot 28 is redraw's alone (its only stores are 0 at function start, 0 before the cnt test and the 1s in input handling), and CSE (const 0 costs 0, a pseudo 1) and GCSE cprop (which propagates into any copy whose source has a constant available set) both fold it, so no plain source form found yet. */
/* 08023e70: differ 55 of 1201 (2026-10-06, fourth pass).The page list loop reads its code in the while condition, code = tbl[page + j], with no entry pointer: loop strength reduction then makes the pointer a giv set after the copied exit test (native mov r8, r3 after beq), and the first 0x80000000 test takes r1 as the native does (65 to 55). Trials this pass: a function-scope base asm("r3") 202, 138 without the barriers; block-scoped asm("r3") holds at all three sites 202, and with the sum computed first 104; at the first 0x666 site alone 67 (r3 is right, but its literal load is scheduled after adds r0, r0, r5 where the native loads it before lsls r0, r0, #2), the same with asm("" : "=r"(hold) : "0"(0x666)) 67, and without the barrier combine folds it back (207); on the 55 draft the first-site r3 hold is 57. The permuter with PERM_LINESWAP over the twelve initializations and the three cell loads compiled 1,976 candidates and none beat the draft, so the start-of-function schedule (movs r6, #0 late; mov r9, r0 after the gWindowWork loads) does not come from statement order. Remaining at 55: the ldr r1, [sp, #28] search-index load (its 2 bytes shift every later pool load and branch: about 25 of the 55), the 0x666/0x45f literal loads in r2 where the native has r3 (12), the start schedule (8), and two late subs #imm scheduled one slot early (detail_win->width - 2 for 0xf335; cursor_pos.x = list_win->x * 8 - 2). Trial: y = row * 2 computed before cursor_pos.x 71; the r3 hold at the 0x45f site alone 110 (the loop around it reallocates); the permuter with PERM_RANDOMIZE on the 0xf335 call and the two cursor_pos lines, 473 candidates, none beat 55. The b.n -740 against +44 is only the pool alignment that the missing 2-byte load changes. Earlier, third pass at 65: Cause found: in the native, the message-id bases 0x666 (both arms) and 0x45f sit in local-alloc pseudos and take r3, first in the allocation order. The draft adds them inline, so they become rotation reloads, and every detail_win and zero reload after them shifts. Tagged asm-held temporaries (base) do this: 137 to 71. The 0x666 arm then resets detail_win before sub_sel as the native stores do (69), and entry is an integer sum page * 4 + tbl (65). Before that, the page wrap setting page = 0 before row = saved_row took 145 to 137 (from the permuter's best-1; its other rewrites are noise). Remaining: the held constant takes r2 where the native has r3, because its life still overlaps the 255 mask; a statement expression, =r/0 or +l constraints all stay at 69. Also left: the start-of-function schedule (movs r6, #0; mov r9, r0) and the k = redraw search-index load. Trial: initializing the search from redraw instead of 0 is unchanged (145). Further trials: a consumed is_used constraint differs 183, empty barriers stay 145, and reversing the detail reset statements differs 150. Remaining: the native loads the is_used search index from the redraw slot (one extra reload) and keeps the 0x8ef branch separate; the reload registers after it rotate differently. */
/* MenuCell now includes its automatic-mode word at 0xe0; remove the unused duplicate tail word so selected remains at 0xe4 and total size at 0x104. */
/* Shared PartyStatusWindow from the verified STATUS.C corpus leaves differ 145. Further search trials: combined initialization, is_used as index, while-loop and memory clobber remain 145; do-while 166; index-first initialization 155; initialization before the clear call 148. Redraw memory/output constraints range 447-722. Clobbering r1 reaches 120 but emits mov-zero rather than the native reload, so it was removed. */
/* Stubborn (2026-10-06): CSE seeds the native search index from redraw's spilled zero (ldr r1, [sp, #28]); the draft keeps a fresh zero, and the reload rotation after the 0x8ef branch follows from it. */
/* Third pass (2026-10-06), measured against the 137 draft. Correction: the missing search-index load does not shift the reload rotation. Reload regs from is_used = 0 through the 0x8ef branch (r1, r2, r3, r1, r2, r3) match the native, and an asm tie that does reload redraw into r1 leaves the later swaps unchanged (141). The first divergence is the 0x666 constant reload in the cur & 0x3e0000 branch: native r3, draft r1. After it, detail_win and its zero come in rotation order (native r1/r2, draft r2/r3). Cause still unknown: some reload or local-alloc choice between the 0x8ef zero and insn 676. cse.c sets COST(pseudo) 1 against const 0 at 0, so CSE always folds k = redraw while redraw = 0 is known. The native copy survives only if CSE does not know redraw there, yet the native's first unrolled probe still folds k to 0 (movs r2, #228). No source form found that does both. Trials: asm("" : "=r"(k) : "0"(redraw)) 141 (CSE canonicalises redraw to mask, so it loads [sp, #40] and the first probe keeps cmp/lsls); k = redraw then +r k 149; asm use of redraw before the loop 160, before is_used 175; explicit first probe plus while (++k <= 7) 169 (strength-reduced); do-while from k 166/294; redraw = 0 in both arms of the sub_sel block 207 (cprop still makes k = 0, now movs r1, #0); detail_win = 0 before sub_sel = 0 in the 0x666 arm 142 (cross-jumps with the 0x8ef arm, 1189 insns); 0x666 written first or summed in u32 137; index-first sum 159; code = 0 before cnt = 0 or before the pool allocation 136 but 1199 insns (cnt's zero folds into r6, the native keeps movs r1, #0); prev_page before cnt 145; code = 0 last 147; the permuter's declaration swap, j = 0 hoist and PushValueSlot temporary each leave the count unchanged. */
/* Trials (2026-10-06, second pass): pos as cnt + tbl or as an integer sum 145 (same code); k < 8 145; function-scope j as the index 145 (it gets its own movs r7, #0); redraw = 0 moved before the sub_sel block with k = redraw 155 (CSE still folds it through the skipped block). The -da dumps keep k = (const_int 0) through cse, gcse, loop and combine, and reload_cse then drops it because r1 already holds is_used's zero; the native needs k = (reg redraw) alive at reload. The r1/r2/r3 swaps on the later 0x666 and 0x45f constant reloads follow from the reload register rotation that one missing reload shifts. */

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
    s32 base;
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
                        base = 0x666;
                        /* FAKEMATCH: the native holds 0x666 in a local-alloc pseudo (r3); inline it is a reload in rotation (r1) and shifts every later reload */
                        asm("" : "+r"(base));
                        UiText_CopyMessageString((s32)(((cur & 0xf00) >> 8) * 20 + (cur & 0xff)) + base,
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
                    base = 0x666;
                    /* FAKEMATCH: 0x666 in a pseudo (native r3), not a rotation reload */
                    asm("" : "+r"(base));
                    UiText_CopyMessageString((s32)(((cur & 0xf00) >> 8) * 20 + (cur & 0xff)) + base,
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
                j = 0;
                while ((code = tbl[page + j]) != 0x80000000) {
                    UiWindow_SetTilemapEntry(list_win, (s32)((code & 0xf00) >> 8) + 0x5001, 0, j * 2, 0);
                    if (code & 0x3e0000) {
                        UiWork_SetParamNibble(4);
                    } else if (code & 0x10000) {
                        UiWork_SetParamNibble(2);
                    }
                    base = 0x45f;
                    /* FAKEMATCH: 0x45f in a pseudo (native r3), not a rotation reload */
                    asm("" : "+r"(base));
                    UiText_DrawCharacterAtOffset((s32)(((code & 0xf00) >> 8) * 20 + (code & 0xff)) + base,
                                  (struct RenderInput *)list_win, 8, j * 16);
                    if (code & 0x3e0000) {
                        UiText_DrawNumberAtOffset((s32)((code & 0x3e0000) >> 17), 1, (struct RenderInput *)list_win, 48, j * 16);
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

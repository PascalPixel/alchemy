/* 08023178: Ui_RunOwnerStatusScreen draft.
 * Native 1392 instructions, draft 1406, common 992, differ 814.
 * Frame now 352 bytes, as native. Remaining: redraw in r11 where native
 * has r10, the setup loop counter in r4 (caller-saved, sp+4) where native
 * has r6 and the -1 store value in r8, the search value in r7 where native
 * has r3, then register choices through the loop body.
 *
 * Diagnosis 2026-10-06: the native frame is 96 bytes of spill
 * slots under exactly 256 bytes of locals (cur 96, objName 128, objIcon 140,
 * sel 272, tbl 284, objCursor 328, objFace 340: declaration order, frame
 * growing down). Spill slots follow pseudo (declaration) order downward:
 * list, listCount, owner, base, winMain, winDesc, idx, handleCursor, the
 * cursor pixel pair, handleFrame, cnt, extended, keys, text, iconIdx, then
 * the array addresses. The cursor pixel offsets' zero stores survive flow
 * only if their sets are partial: one 8-byte {x, y} struct held in a DImode
 * pseudo, spilled to sp+56/sp+60. That gave the 4 missing frame bytes.
 *
 * Trials 2026-10-06: pixel pair as struct StatusPixel 1087 ->
 * 1007; declarations in native spill order 859; searched single moves 857;
 * owner search indexed by i 817; tbl/sel/objIcon used directly instead of
 * handles/codes/icons aliases 814. Rejected: owner search on list[n] without
 * code 868; n as the setup loop counter 1298; pixel ints in spill order
 * without the struct 1219.
 *
 * Earlier trials 2026-10-06: searched declaration order alone 1171 -> 1127. The
 * closing busy flag addressed from the battle work slot, as RESOLVE.C does,
 * reuses the native literal (gBattleWork, then +192): 1162 alone, 1118 with
 * the searched order. Cursor tables indexed by cur.pos[cur.mode] (native
 * loads by the mode register): 1113. Equipment and condition cursors read
 * straight from their tables (native loads entry by [table, index]): 1087.
 * A separate icon-loop x: 1354 (1330 after search). A separate index for
 * the condition code: 1179. Declarations in native spill-slot order: 1250.
 *
 * Earlier compiler ablation: three old register bindings gave differ 1280.
 * Removing only the redraw binding gave 1331; removing both copier bindings
 * as well gave 1196 and reduced the frame difference from 8 to 4 bytes.
 * Ordinary direct MenuSprite accesses give 1184. Keeping the redraw binding
 * with direct copies gave 1197; direct handle/code arrays gave 1267.
 * Indexed cursor initialization gives 1178, indexed handle initialization
 * 1177, together 1171. Unsigned redraw and block-local saved-unit variables
 * leave 1184 unchanged. The indexed loops preserve the native store bounds.
 * No register bindings or other steering devices remain in this draft.
 */
#include "IO_REG.H"
#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_WORK.H"
#include "MENU_LIST.H"
#include "UI.H"
#include "VRAM_BLOCK.H"
#include "MOTION_OBJECT.H"
#include "RUNTIME_MEM.H"
#include "IWRAM_CALL.H"
#include "HEAP_STATE.H"

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

extern u8 MsgClassName;
extern char MsgOwnerStatusExp;
extern char MsgOwnerStatusNormal;
extern char MsgOwnerStatusDowned;
extern char MsgOwnerStatusNextLevel;
extern char MsgOwnerStatusEntryHelp;
extern char MsgOwnerStatusConditionHelp;

void UiWindow_MapTextCanvasTiles(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiWork_PushValueSlot(s32 value, s32 slot);
void UiWindow_MarkVisibleTileAttributes(void);
void UiWindow_DrawDividerLine(struct UiWindow *window, s32 x, s32 y, s32 width, s32 height);
void UiText_DrawNumberInWindow(s32 value, s32 digits, s32 window, s32 x, s32 y);
void UiWindow_DrawPartyStatusContents(s32 id);
void Resource_LoadTableEntryToBuffer(s32 icon, s32 handle);
s32 Ui_LoadEntryForKind(s32 owner, s32 handle);
s32 Owner_GetResistanceValueFar(s32 owner, s32 index);
s32 Owner_GetLevelThresholdFar(s32 owner, s32 level);
s32 Party_SumDjinnCountsFar(s32 request);

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
    s8 *out;
    s32 code;
    s32 atkDelta;
    s32 redraw;
    struct BattleUnit *object;
    struct BattleUnit *saved;
    const struct OwnerStatusStatCursor *stat;
    struct MenuSprite *p;
    s32 agiDelta;
    s32 n;
    struct MenuCell *screen;
    s32 x;

    base = (struct UiRenderWork *)gWindowWork[0];
    idx = -1;
    redraw = 1;
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
            code = list[i];
            if (code == 0xFF) {
                break;
            }
            if (code != 0xFE) {
                if (code == owner) {
                    idx = i;
                    break;
                }
            }
        }
    }

    pix.x = 0;
    pix.y = 0;
    winMain = UiWindow_Create(0, 0, 30, 20, 6);
    winDesc = UiWindow_Create(0, 14, 30, 6, 10);
    UiWindow_MarkVisibleTileAttributes();

    for (;;) {
        keys = gKeysRepeat;

        objName.oam.word.attr01 = 0x80000400;
        objName.oam.word.attr23 = 0;
        objName.oam.f.tile = Ui_LoadEntryForKind(owner, handleFrame) & 0x3FF;
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
                            n = cur.pos[2];
                        }
                    }
                } else if ((keys & 0x40) != 0) {
                    keys = 0;
                    n--;
                    AudioCommand_PlayFar(0x6F);
                    if (n < 0) {
                        n = cur.limit - 1;
                        if (cur.mode == 1) {
                            cur.mode = 2;
                            n = cur.pos[2];
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
                n = cur.pos[2];
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
            i = cur.pos[cur.mode];
            if (extended == 0) {
                i += 9;
            }
            stat = &OwnerStatus_StatCursors[i];
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

        objFace.oam.word.attr01 = 0xC0002400;
        objFace.oam.word.attr23 = 0;
        objFace.oam.f.tile = ((struct MenuSprite *)GetBattleObjectSlotFar(owner)->object->records)->oam.f.tile;
        objFace.oam.f.x = 0xAC;
        objFace.oam.f.y = 56;
        Runtime_PushSlotEntry((s32 *)&objFace, 240);

        objCursor.oam.word.attr01 = 0x40000400;
        objCursor.oam.word.attr23 = 0;
        objCursor.oam.f.tile = Resource_GetBuffer(handleCursor, (s32)Resource_FixedBlockBTiles)
            & 0x3FF;
        objCursor.oam.f.x = (pix.x + winMain->x * 8
                          - ((gFrameCount & 4) >> 2))
            + 16;
        objCursor.oam.f.y = (u8)((pix.y + winMain->y * 8
                               - ((gFrameCount & 4) >> 2))
            + 16);
        objCursor.oam.f.affine_index = 8;
        Runtime_PushSlotEntry((s32 *)&objCursor, 241);

        if (redraw != 0) {
            object = Owner_GetStateFar(owner);
            UiWindow_MarkVisibleTileAttributes();
            Ui_FillVramBlockPattern();

            if ((redraw & 1) != 0) {
                RenderOutput_RedrawSavedRect(winMain);
                UiWindow_MapTextCanvasTiles(winDesc->x, winDesc->y, winDesc->width,
                    winDesc->height, 0);
                UiWindow_DrawDividerLine(winMain, 0, 14, 29, 14);

                UiText_DrawStringAtOffset((u8 *)object->name, (struct RenderInput *)winMain, 0, 0);
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_LevelString, (struct RenderInput *)winMain, 56, 0);
                UiText_DrawNumberInWindow(object->level, 2, (s32)winMain, 72, 0);
                UiText_DrawCharacterAtOffset((s32)&MsgOwnerStatusExp, (struct RenderInput *)winMain, 0, 8);
                UiText_DrawNumberInWindow(
                    object->experience, 8, (s32)winMain, 40, 8);

                UiText_DrawStringAtOffset((u8 *)OwnerStatus_HpString, (struct RenderInput *)winMain, 40, 24);
                UiText_DrawNumberInWindow(
                    object->hp, 4, (s32)winMain, 56, 24);
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_SlashString, (struct RenderInput *)winMain, 88, 24);
                UiText_DrawNumberInWindow(
                    object->max_hp, 4, (s32)winMain, 96, 24);
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_PpString, (struct RenderInput *)winMain, 40, 32);
                UiText_DrawNumberInWindow(
                    object->pp, 4, (s32)winMain, 56, 32);
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_SlashString, (struct RenderInput *)winMain, 88, 32);
                UiText_DrawNumberInWindow(
                    object->max_pp, 4, (s32)winMain, 96, 32);

                UiText_DrawCharacterAtOffset((s32)&MsgOwnerStatusExp - 10, (struct RenderInput *)winMain, 136, 16);
                UiText_DrawNumberInWindow(
                    object->attack, 3, (s32)winMain, 184, 16);
                UiText_DrawCharacterAtOffset((s32)&MsgOwnerStatusExp - 9, (struct RenderInput *)winMain, 136, 24);
                UiText_DrawNumberInWindow(
                    object->defense, 3, (s32)winMain, 184, 24);
                UiText_DrawCharacterAtOffset((s32)&MsgOwnerStatusExp - 8, (struct RenderInput *)winMain, 136, 32);
                UiText_DrawNumberInWindow(
                    object->agility, 3, (s32)winMain, 184, 32);
                UiText_DrawCharacterAtOffset((s32)&MsgOwnerStatusExp - 7, (struct RenderInput *)winMain, 136, 40);
                UiText_DrawNumberInWindow(object->luck, 3, (s32)winMain, 184, 40);

                UiText_DrawCharacterAtOffset(
                    (s32)&MsgClassName + object->class_index, (struct RenderInput *)winMain, 0, 48);
                if (extended != 0) {
                    UiText_DrawCharacterAtOffset((s32)&MsgOwnerStatusExp - 1, (struct RenderInput *)winMain, 0, 72);
                }
                UiText_DrawCharacterAtOffset((s32)&MsgOwnerStatusExp - 5, (struct RenderInput *)winMain, 0, 80);
                UiText_DrawCharacterAtOffset((s32)&MsgOwnerStatusExp - 4, (struct RenderInput *)winMain, 0, 88);
                UiText_DrawCharacterAtOffset((s32)&MsgOwnerStatusExp - 3, (struct RenderInput *)winMain, 0, 96);

                for (i = 0; i <= 3; i++) {
                    if (extended != 0) {
                        code = 8;
                    } else {
                        code = 9;
                    }
                    UiWindow_SetTilemapEntry(
                        winMain, 0x5001 + i, 7 + i * 4, code, 0);
                    if (extended != 0) {
                        UiText_DrawNumberInWindow(object->djinn_active_counts[i], 1,
                            (s32)winMain, 40 + i * 32, 72);
                        UiText_DrawStringAtOffset(
                            (u8 *)OwnerStatus_SlashString, (struct RenderInput *)winMain, 48 + i * 32, 72);
                        UiText_DrawNumberInWindow(object->djinn_owned_counts[i], 1,
                            (s32)winMain, 48 + i * 32 + 8, 72);
                    }
                    UiText_DrawNumberInWindow(Owner_GetResistanceValueFar(owner, i), 2,
                        (s32)winMain, 48 + i * 32, 80);
                    UiText_DrawNumberInWindow(
                        object->elements[i].power, 3, (s32)winMain,
                        40 + i * 32, 88);
                    UiText_DrawNumberInWindow(
                        object->elements[i].resist, 3, (s32)winMain,
                        40 + i * 32, 96);
                }

                n = 0;
                if (object->hp == 0) {
                    sel[0] = 16;
                    n = 1;
                }
                out = &sel[n];
                do {
                    if (object->restraint != 0) {
                        *out++ = 15;
                        n++;
                    }
                    if (n > 7) {
                        break;
                    }
                    if (object->death_count != 0) {
                        *out++ = 8;
                        n++;
                    }
                    if (n > 7) {
                        break;
                    }
                    if (object->poison == 1) {
                        *out++ = 1;
                        n++;
                    }
                    if (n > 7) {
                        break;
                    }
                    if (object->poison == 2) {
                        *out++ = 2;
                        n++;
                    }
                    if (n > 7) {
                        break;
                    }
                    if (object->psy_seal != 0) {
                        *out++ = 4;
                        n++;
                    }
                    if (n > 7) {
                        break;
                    }
                    if (object->stun != 0) {
                        *out++ = 3;
                        n++;
                    }
                    if (n > 7) {
                        break;
                    }
                    if (object->sleep != 0) {
                        *out++ = 5;
                        n++;
                    }
                    if (n > 7) {
                        break;
                    }
                    if (object->evil_spirit != 0) {
                        *out++ = 7;
                        n++;
                    }
                    if (n > 7) {
                        break;
                    }
                    if (object->delusion != 0) {
                        *out++ = 6;
                        n++;
                    }
                    if (n > 7) {
                        break;
                    }
                    if (object->attack_modifier_turns != 0) {
                        if ((s8)object->attack_modifier > 0) {
                            *out++ = 9;
                            n++;
                        }
                        if ((s8)object->attack_modifier < 0) {
                            *out++ = 10;
                            n++;
                        }
                    }
                    if (n > 7) {
                        break;
                    }
                    if (object->defense_modifier_turns != 0) {
                        if ((s8)object->defense_modifier > 0) {
                            *out++ = 11;
                            n++;
                        }
                        if ((s8)object->defense_modifier < 0) {
                            *out++ = 12;
                            n++;
                        }
                    }
                    if (n > 7) {
                        break;
                    }
                    if (object->res_modifier_turns != 0) {
                        if ((s8)object->res_modifier > 0) {
                            *out++ = 13;
                            n++;
                        }
                        if ((s8)object->res_modifier < 0) {
                            *out++ = 14;
                            n++;
                        }
                    }
                    if (n > 7) {
                        break;
                    }
                    if ((s8)object->agility_modifier > 0) {
                        *out++ = 17;
                        n++;
                    }
                    if ((s8)object->agility_modifier < 0) {
                        *out = 18;
                        n++;
                    }
                } while (0);

                if (n > 0) {
                    i = n;
                    out = sel;
                    q = tbl;
                    do {
                        Resource_LoadTableEntryToBuffer(*out, *q++);
                        i--;
                        out++;
                    } while (i != 0);
                }
                if (n == 0) {
                    sel[0] = 0;
                    n = 1;
                }
                if (n <= 10) {
                    for (i = n; i < 11; i++) {
                        sel[i] = -1;
                    }
                }
                cnt = n;
                if (sel[0] == 0) {
                    if (object->hp != 0) {
                        code = (s32)&MsgOwnerStatusNormal;
                    } else {
                        code = (s32)&MsgOwnerStatusDowned;
                    }
                    UiText_DrawCharacterAtOffset(code, (struct RenderInput *)winMain, 112, 0);
                }
            }

            text = (u16 *)Runtime_BumpAllocate(0x100);
            if ((u32)cur.entry > 13) {
                iconIdx = sel[cur.entry - 14];
                if (iconIdx == 0 && object->hp == 0) {
                    iconIdx = 16;
                }

                saved = (struct BattleUnit *)Runtime_BumpAllocate(sizeof(*object));
                code = 0;
                Iwram_CopyWords(saved, object, sizeof(*object));

                atkDelta = object->attack;
                defDelta = object->defense;
                agiDelta = object->agility;
                object->attack_modifier = 0;
                object->defense_modifier = 0;
                object->agility_modifier = 0;
                BattleUnit_Recalculate(owner);
                atkDelta -= object->attack;
                defDelta -= object->defense;
                agiDelta -= object->agility;
                Iwram_CopyWords(object, saved, sizeof(*object));
                Sys_Free(saved);

                switch (iconIdx) {
                case 8:
                    code = object->death_count;
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
                    code = object->res_modifier * 20;
                    break;
                default:
                    break;
                }
                UiWork_PushValueSlot(code, 5);
                UiText_CopyMessageString(iconIdx + (s32)&MsgOwnerStatusConditionHelp, text, 0x80);
            } else if (cur.entry == 2 && object->level <= 98) {
                UiWork_PushValueSlot(
                    Owner_GetLevelThresholdFar(owner, object->level + 1)
                        - object->experience,
                    5);
                UiText_CopyMessageString((s32)&MsgOwnerStatusNextLevel, text, 0x80);
            } else {
                UiText_CopyMessageString(cur.entry + (s32)&MsgOwnerStatusEntryHelp, text, 0x80);
            }

            UiText_RenderWideStringAtOffset(text, (struct UiWindow *)winDesc, 0, 4);
            Sys_Free(text);
            base->dirty = 1;
            redraw = 0;
        }

        q = tbl;
        p = objIcon;
        x = 112;
        for (i = 0; i <= 10; i++) {
            p->oam.word.attr01 = 0x40000400;
            p->oam.word.attr23 = 0;
            p->oam.f.tile = gVramBlockCache[*q++].offset >> 5;
            p->oam.f.x = x;
            p->oam.f.y = (u8)(winMain->y * 8 + 8);
            if (sel[i] > 0) {
                Runtime_PushSlotEntry((s32 *)p, 240);
            }
            x += 15;
            p++;
        }

        if (((struct MenuCell *)gWindowWork[42])->active == 0) {
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

    q = tbl;
    for (i = 10; i >= 0; i--) {
        Resource_ResetEntry(*q++);
    }
    Resource_ResetEntry(handleFrame);
    Resource_ResetEntry(handleCursor);
    WaitFrames(1);
    UiWindow_MarkVisibleTileAttributes();
    UiWork_Finalize(winMain, 1);
    UiWork_Finalize(winDesc, 1);
    UiWindow_DrawPartyStatusContents(gBattleWork->party_status_mode);
    ((struct MenuCell **)&gBattleWork)[HEAP_SLOT_WINDOW + 42 - HEAP_SLOT_BATTLE]->busy = 0;
    WaitFrames(1);
    return 0;
}

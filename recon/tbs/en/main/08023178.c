/* Draft, not exact: current EN score14696, all external symbols resolve.
 * Native3320/candidate3352 bytes including pools; frames352/344.
 * Every retained aggregate and bound pointer has real uses.
 */
/* Not-yet-C: complete 3320-byte owner.
 * 2026-10-02 unchanged draft: score23698 (347 register, 93 stack,
 * 70 operand, 107 reordered, 76 inserted, 61 deleted), frame372 versus352;
 * six fake Value_000008* message symbols unresolved in that measurement.
 * Trial1 ownership cleanup: score18714 (307 register, 94 stack,
 * 59 operand, 90 reordered, 43 inserted, 59 deleted), frame344.
 * New data/message names still unresolved in the copied target context.
 * Trial2 nine consumed stat cases recover dense dispatch: score15950
 * (313 register, 95 stack, 69 operand, 87 reordered, 40 inserted,
 * 34 deleted), frame344. Six catalog symbols unresolved at measurement.
 * Trial3 shared initialized redraw for setup busy/finalize: identical15950
 * and frame344; ineffective body change reverted.
 * Naming-complete trial2 recheck: score15840 (315 register, 95 stack,
 * 63 operand, 87 reordered, 40 inserted, 34 deleted), frame344.
 * Trial4 live redraw in r10 retained: score15792 (329 register, 102 stack,
 * 61 operand, 92 reordered, 42 inserted, 28 deleted), frame344.
 * Every external symbol resolves against the refreshed byte-identical EN ELF.
 * Existing MenuSprite/UiWindow/MenuCell, BattleUnit/BattleSession,
 * VramBlockCacheEntry and typed Iwram_CopyWords replace local fake views,
 * fixed global addresses and a four-argument copier-veneer declaration.
 * Real native stack: cursor32 at96, name sprite12 at128, icon sprites132
 * at140, signed icon codes11 at272, natural alignment1, handles44 at284,
 * cursor sprite12 at328 and face sprite12 at340. Sp+0..95 is arguments and
 * compiler spills, never source storage. Every array element is consumed.
 * The three delta locals hold attack/defense/agility changes.
 * Formation trial5 consumed pointer bases: score14745 (322 register,
 * 70 stack, 65 operand, 85 reordered, 37 inserted, 27 deleted),
 * frame348 and complete compiled extent3348 versus native3320.
 * Trial6 natural redraw with pointer bases: score15510 (314 register,
 * 70 stack, 64 operand, 87 reordered, 41 inserted, 30 deleted), frame348;
 * measured redraw binding remains better.
 * Trial7 face lookup before stores: score14780 (322 register, 70 stack,
 * 66 operand, 82 reordered, 38 inserted, 28 deleted), frame348; reverted.
 * Trial8 owned IWRAM bank expression: unchanged14745/frame348; reverted.
 * Trial9 used-pointer inline wrapper: unchanged14745/frame348; reverted.
 * Trial10 scoped real copier pointer in r3: score14696 (326 register,
 * 106 stack, 61 operand, 83 reordered, 39 inserted, 26 deleted),
 * frame344 and extent3352; attack spill removed by the real register choice.
 * Trial11 plain shared owner-copy size: unchanged14696/frame344; reverted.
 * Trial12 consumed size in r8: score15041 (332 register, 71 stack,
 * 61 operand, 97 reordered, 36 inserted, 24 deleted), frame348; reverted.
 * Trial13 scoped condition value: score16016 (346 register, 101 stack,
 * 66 operand, 93 reordered, 39 inserted, 31 deleted), frame344; reverted.
 * Trial14 final redraw ablation with r3 copies: score14890 (348 register,
 * 110 stack, 60 operand, 83 reordered, 39 inserted, 27 deleted), frame344;
 * retained redraw binding improves the current body by194.
 * Final retained trial10: score14696, all external symbols resolved,
 * native3320/candidate3352 including pools; native frame352/candidate344.
 * Pointer/spill residual: native pixel coordinates occupy56/60; generated
 * x lives in r9 and y in outgoing sp+4, accounting for eight fewer spill bytes.
 * Remaining: compiler spills/lifetimes, register order, branch/layout and
 * literal-pool extent. Native frame352 versus retained344. */
#include "BATTLE_RUNTIME.H"
#include "BATTLE_WORK.H"
#include "MENU_LIST.H"
#include "UI.H"
#include "VRAM_BLOCK.H"
#include "MOTION_OBJECT.H"
#include "RUNTIME_MEM.H"
#include "IWRAM_CALL.H"

/* One cursor position per selectable row; fields follow the native record. */
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
struct BattleObjectSlot *GetBattleObjectSlotFar(s32 owner);

/* Modal owner status screen. Cursor rows choose stats, equipment or conditions;
 * the active menu cell or cancellation closes the screen after releasing slots. */
s32 Ui_RunOwnerStatusScreen(u16 *list, s32 listCount, s32 owner)
{
    struct MenuSprite objFace;
    struct MenuSprite objCursor;
    s32 tbl[11];
    s8 sel[11];
    struct MenuSprite objIcon[11];
    struct MenuSprite objName;
    struct StatusCursor cur;
    struct UiWindow *winMain;
    struct UiWindow *winDesc;
    struct BattleUnit *object;
    struct UiRenderWork *base;
    struct MenuCell *screen;
    const struct OwnerStatusStatCursor *stat;
    const struct OwnerStatusCursor *cell;
    s32 *q;
    u16 *text;
    s32 handleFrame;
    s32 handleCursor;
    s32 extended;
    /* FAKEMATCH: native redraw uses r10 across calls; final ablation scores14890 versus14696 with this consumed flag bound. */
    register s32 redraw asm("r10");
    s32 keys;
    s32 cnt;
    s32 idx;
    s32 iconIdx;
    s32 colPixels;
    s32 rowPixels;
    s32 i;
    s32 n;
    s32 code;
    s32 atkDelta;
    s32 defDelta;
    s32 agiDelta;
    s8 *out;
    struct BattleUnit *saved;
    struct MenuSprite *p;
    struct MenuSprite *face;
    struct MenuSprite *cursor;
    struct MenuSprite *name;
    struct MenuSprite *icons;
    s32 *handles;
    s8 *codes;

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

    out = &cur.pos[4];
    for (i = 4; i >= 0; i--) {
        *out-- = 0;
    }
    cur.entry = 0;
    cur.lastEntry = 0;
    cur.mode = 0;

    handleCursor = Resource_LoadIntoFreeSlot(0x80);
    handles = tbl;
    codes = sel;
    q = handles;
    out = codes;
    for (i = 10; i >= 0; i--) {
        *q++ = Resource_LoadIntoFreeSlot(0x80);
        *out++ = -1;
    }

    if (listCount != 0) {
        for (n = 0; n <= 5; n++) {
            code = list[n];
            if (code == 0xFF) {
                break;
            }
            if (code != 0xFE) {
                if (code == owner) {
                    idx = n;
                    break;
                }
            }
        }
    }

    colPixels = 0;
    rowPixels = 0;
    winMain = UiWindow_Create(0, 0, 30, 20, 6);
    winDesc = UiWindow_Create(0, 14, 30, 6, 10);
    UiWindow_MarkVisibleTileAttributes();

    name = &objName;
    face = &objFace;
    cursor = &objCursor;
    icons = objIcon;
    for (;;) {
        keys = gKeysRepeat;

        name->oam.word.attr01 = 0x80000400;
        name->oam.word.attr23 = 0;
        name->oam.f.tile = Ui_LoadEntryForKind(owner, handleFrame) & 0x3FF;
        name->oam.f.x = 8;
        name->oam.f.y = 24;
        name->oam.f.palette = 14;
        Runtime_PushSlotEntry((s32 *)name, 240);

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
                    if ((gKeysRepeat & 1) != 0) {
                        n = cur.pos[cur.mode];
                    } else if ((gKeysRepeat & 0x20) != 0) {
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
            i = cur.pos[0];
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
            cell = &OwnerStatus_EquipmentCursors[cur.pos[1]];
            cur.entry = cell->entry;
            cur.col = cell->column;
            cur.row = cell->row;
        } else if (cur.mode == 2) {
            cell = &OwnerStatus_ConditionCursors[cur.pos[2]];
            cur.entry = cell->entry;
            cur.col = cell->column;
            cur.row = cell->row;
        }

        if (cur.lastEntry != cur.entry) {
            cur.lastEntry = cur.entry;
            redraw = 2;
        }
        colPixels = cur.col * 8;
        rowPixels = cur.row * 8;

        face->oam.word.attr01 = 0xC0002400;
        face->oam.word.attr23 = 0;
        face->oam.f.tile = ((struct MenuSprite *)GetBattleObjectSlotFar(owner)->object->records)->oam.f.tile;
        face->oam.f.x = 0xAC;
        face->oam.f.y = 56;
        Runtime_PushSlotEntry((s32 *)face, 240);

        cursor->oam.word.attr01 = 0x40000400;
        cursor->oam.word.attr23 = 0;
        cursor->oam.f.tile = Resource_GetBuffer(handleCursor, (s32)Resource_FixedBlockBTiles)
            & 0x3FF;
        cursor->oam.f.x = (colPixels + winMain->x * 8
                          - ((gFrameCount & 4) >> 2))
            + 16;
        cursor->oam.f.y = (u8)((rowPixels + winMain->y * 8
                               - ((gFrameCount & 4) >> 2))
            + 16);
        cursor->oam.f.affine_index = 8;
        Runtime_PushSlotEntry((s32 *)cursor, 241);

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
                    codes[0] = 16;
                    n = 1;
                }
                out = &codes[n];
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
                    out = codes;
                    q = handles;
                    do {
                        Resource_LoadTableEntryToBuffer(*out, *q++);
                        i--;
                        out++;
                    } while (i != 0);
                }
                if (n == 0) {
                    codes[0] = 0;
                    n = 1;
                }
                if (n <= 10) {
                    for (i = n; i < 11; i++) {
                        codes[i] = -1;
                    }
                }
                cnt = n;
                if (codes[0] == 0) {
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
                iconIdx = codes[cur.entry - 14];
                if (iconIdx == 0 && object->hp == 0) {
                    iconIdx = 16;
                }

                saved = (struct BattleUnit *)Runtime_BumpAllocate(sizeof(*object));
                code = 0;
                {
                    /* FAKEMATCH: direct and wrapper trials hoist the used copier to r10; native reloads its consumed pointer in r3 for this call. */
                    register s32 (*copy)(void *, const void *, s32) asm("r3") = Iwram_CopyWords;

                    copy(saved, object, sizeof(*object));
                }

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
                {
                    /* FAKEMATCH: the second native copy also uses the real routine pointer in r3; intervening calls clobber that register. */
                    register s32 (*copy)(void *, const void *, s32) asm("r3") = Iwram_CopyWords;

                    copy(object, saved, sizeof(*object));
                }
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

        q = handles;
        p = icons;
        colPixels = 112;
        for (i = 0; i <= 10; i++) {
            p->oam.word.attr01 = 0x40000400;
            p->oam.word.attr23 = 0;
            p->oam.f.tile = gVramBlockCache[*q++].offset >> 5;
            p->oam.f.x = colPixels;
            p->oam.f.y = (u8)(winMain->y * 8 + 8);
            if (codes[i] > 0) {
                Runtime_PushSlotEntry((s32 *)p, 240);
            }
            colPixels += 15;
            p++;
        }

        if (((struct MenuCell *)gWindowWork[42])->active == 0) {
            break;
        }
        if ((gKeyState & 2) != 0) {
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

    q = handles;
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
    ((struct MenuCell *)gWindowWork[42])->busy = 0;
    WaitFrames(1);
    return 0;
}

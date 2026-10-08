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
#include "resmenu.h"
#include "dma.h"
#include "system.h"
#include "types.h"
#include "io_reg.h"
#include "callback_scheduler.h"
#include "global_cells.h"
#include "affine.h"
#include "vram_tab.h"
#include "text/msg_ids.h"
#include "edition.h"
#include "btlwork.h"
#include "battle_unit.h"
#include "item.h"
#include "btlsmn.h"
#include "menu_list.h"
#include "serial_runtime.h"
s32 GameFlag_TestFar(s32 flag);
struct BtlAct * BtlActGet(s32 action_id);
struct ItemDefinition *Item_GetDirectFar(s32 item);
struct BtlUnit *UnitGetFar(s32 object_id);

#define gKeyState (*(volatile u32 *)&gKeyState)
#define gKeysRepeat (*(volatile u32 *)&gKeysRepeat)
#define UiText_DrawCharacterAtOffset ((void (*)(s32, void *, s32, s32))UiText_DrawCharacterAtOffset)

struct WindowLine {
    u16 left;
    u16 span;
};

struct WindowBuffer {
    u8 unknown_000[4];
    struct WindowLine lines[160];
};

extern volatile u32 gFrameCount;
extern u8 *gDisplayTransition;

#define COUNTDOWN ((struct LinkRoundState *)gLinkCountdownWork)

extern u16 gLinkPeerSignatures[][12];

struct LinkRoundState {
    struct MenuSprite entries[3];
    u8 entryActive[3];
    u8 unk_27;
    s32 targetOffset;
    s32 currentOffset;
    s32 unk_30;
    s32 unk_34;
    s32 unk_38;
    s32 unk_3c;
    s32 unk_40;
    s32 displayHandle;
    s32 secondaryHandle;
    s32 timer;
    s32 enabled;
    u8 cost[8][4];
    u8 pick[34];
    u16 list[33];
    s32 hintCount;
    s32 hintTimer;
    s32 actorId;
    u32 result[8];
};

extern volatile u16 gLinkStatus;
extern u8 gDebugMode;
extern volatile u32 gKeysHeld;
void AudioCommand_PlayFar(s32 cue);
void Graphics_ExpandVramTilesByColorTable(s32 destination);
void Menu_BuildLocalizedPatternTiles(void);
void UpdateLinkSessionCountdown(void);
s32 Battle_ClassifyEntryKind(const struct BtlAct *entry);
s32 UiText_ShowLocalizedMessageAndWait(void);
struct UiWindow *UiText_ShowMessageAndWaitComplete(s32 id, s32 x, s32 y);
void UiWindow_MarkVisibleTileAttributes(void);
s32 UiWork_IsComplete(void);
s32 Ui_LoadEntryForKind(s32 kind, s32 handle);
s32 BattleTarget_RunSelection(s32 actor, s32 a, s32 b, s32 kind);
void UiWindow_PutGlyph(struct UiWindow *win, s32 id, s32 pos, s32 arg3, u32 arg4);
s32 Ui_RunSelectionScreen(s32 mode);
void Ui_RunOwnerStatusScreen(void *block, s32 handle, s32 actor);
s32 Battle_SelectAbility(s32 actor, s32 mode);
s32 SummonMenu_SelectSummon(s32 a, s32 b, const u8 *standby);
s32 BattleMenu_RunActionSelection(s32 actor, u16 *list, s32 count);
s32 ItemList_SelectEntry(s32 actor, u16 *list, s32 count);

s32 Battle_CollectPartyCommands(struct BattleActionRecord *out, u16 *in, s32 count)
{
    struct LinkRoundState *state;
    struct BtlWork *link;
    struct BtlWork *session;
    u16 *peer;
    u16 *sync;
    struct BattleActionRecord *ent;
    struct BtlUnit *actor;
    struct UiWindow *win;
    struct MenuSprite *slot;
    u16 hdr[2];
    s32 aux;
    s32 actorId;
    s32 mode;
    s32 kind;
    s32 param;
    s32 res1024;
    s32 res512;
    s32 miss;
    s32 i;
    s32 k;
    s32 order;

    void DrawPartyPanels(void)
    {
        u16 data[2];
        data[0] = 0xff;
        BattleLayout_HighlightPartyPanels(data);
        BattlePres_SetActorModesFar(data, 1);
    }

    aux = 256;
    kind = 0;
    param = 0;
    res1024 = Resource_LoadIntoFreeSlot(1024);
    res512 = Resource_LoadIntoFreeSlot(512);
    state = (struct LinkRoundState *)Runtime_AllocateBlock(57, 260);
    Resource_LoadIndexedIntoBuffer(res1024, 0);
    Graphics_ExpandVramTilesByColorTable(0x06006000);
    Link_DrawShiftedTilePair(0x06006680);
    Menu_BuildLocalizedPatternTiles();
    link = BtlWk;
    for (k = 0; k < 8; k++) {
        state->result[k] = 0x80000000;
    }
    for (k = 0; k < 3; k++) {
        state->entryActive[k] = 0;
    }
    state->targetOffset = 0;
    state->currentOffset = 0;
    state->timer = -1;
    state->unk_3c = 0;
    state->unk_40 = 0;
    state->enabled = 0;
    state->secondaryHandle = 0;
    state->displayHandle = 0;
    if (link->two_sided != 0) {
        state->enabled = 1;
        sync = (u16 *)gSerialTransfer.payload;
        sync[4] = 'V';
        sync[5] = 'S';
        sync[6] = 'S';
        sync[7] = 'T';
        miss = 0;
        /* FAKEMATCH: instruction order only: the one-pass block's loop
           notes before the jump end the schedule there, so miss = 0 and the
           'T' store go out before the copies of the side and pause
           pointers that PRE puts before the jump, as native; its LOOP_CONT
           note keeps jump from copying the check in front (differ 4). */
        do {
            goto handshake_check;
handshake_loop:
            if ((gLinkStatus & 3) != 3) {
                if (++miss > 24) {
                    count = -1;
                    state->enabled = 0;
                    goto handshake_done;
                }
            } else {
                miss = 0;
                if (peer[4] == 'V' && peer[5] == 'S' && peer[6] == 'S' && peer[7] == 'T') {
                    goto handshake_done;
                }
                if (peer[4] == 'E' && peer[5] == 'D' && peer[6] == 'V' && peer[7] == 'S') {
                    goto handshake_done;
                }
                if (!(peer[0] == 'E' && peer[1] == 'X' && peer[2] == 'E' && peer[3] == 'C')) {
                    count = -1;
                    state->enabled = 0;
                    goto handshake_done;
                }
            }
            WaitFrames(1);
handshake_check:
            peer = gLinkPeerSignatures[link->link_side ^ 1];
            if (link->link_paused == 0) {
                goto handshake_loop;
            }
        } while (0);
        count = -1;
        state->enabled = 0;
    }
handshake_done:
    Scheduler_AddOrUpdateCallback(UpdateLinkSessionCountdown, 3200);

    for (;;) {
        DrawPartyPanels();
        state->entryActive[2] = 0;
        state->actorId = -1;
        state->hintCount = 0;
        if (GameFlag_TestFar(366) != 0) {
            state->hintCount = 1;
            state->hintTimer = 60;
        }
        if (count > 0) {
            mode = Ui_RunSelectionScreen(0);
        } else {
            mode = 14;
        }
        if (mode == 7) {
            void *block;
            s32 sel;
            s32 handle;

            block = MemAlloc(12);
            if (gDebugMode != 0 && (gKeysHeld & KEY_START) != 0) {
                sel = 2;
            } else {
                sel = 1;
            }
            handle = BattleParty_ListActorIdsFar(sel, block);
            WaitFrames(1);
            Ui_RunOwnerStatusScreen(block, handle, *(u16 *)block);
            /* FAKEMATCH: instruction order only: the one-pass block's loop
               notes keep MemFree's argument out of the call's schedule, so
               *block is loaded before block is copied to r0, as native
               (differ 2). */
            do {
            } while (0);
            MemFree(block);
            continue;
        }
        if (mode == 4) {
            if (UiText_ShowLocalizedMessageAndWait() != 0) {
                continue;
            }
            count = 1;
            ent = out;
            ent->unit_id = in[0];
            ent->priority = 0x7ffe;
            ent->command = 99;
            ent->parameter = 0;
            ent->target = 256;
            goto finish;
        }
        if (mode != 14) {
            goto finish;
        }
        AudioCommand_PlayFar(154);
        i = 0;
        if (i >= count) {
            goto finish;
        }

        for (;;) {
            if (i == 0) {
                BattlePlacement_CountValidEntriesFar(0, state->cost[0]);
            } else {
                s32 k;

                for (k = 0; k < 4; k++) {
                    state->cost[i][k] = state->cost[i - 1][k];
                }
            }

            ent = &out[i];
            actorId = in[i];
            /* FAKEMATCH: instruction order only: called as returning a
               doubleword, the call also sets r1, so the r1 reload for
               class_id no longer depends on the in reload and sched2 loads
               i * 2 before in, as native (differ 2). */
            actor = (struct BtlUnit *)(s32)((s64 (*)(s32))UnitGetFar)(actorId);
            COUNTDOWN->actorId = actorId;
            COUNTDOWN->unk_40 = 0;
            slot = &COUNTDOWN->entries[2];
            slot->oam.word.attr01 = 0x80000400;
            slot->oam.word.attr23 = 0;
            slot->oam.f.tile = Ui_LoadEntryForKind(actor->class_id, res512);
#if EDITION_INTERNATIONAL
            slot->oam.f.x = 0;
#else
            slot->oam.f.x = 4;
#endif
            slot->oam.f.y = 0x80;
            slot->oam.f.palette = 14;
            state->entryActive[2] = 1;

mark_visible:
            UiWindow_MarkVisibleTileAttributes();
            COUNTDOWN->entryActive[0] = 0;
            COUNTDOWN->result[i] = 0x80000000;
            /* FAKEMATCH: register choice (spill slots) only: twelve dead
               stores, one insn each until flow deletes them. GCSE sizes its
               expression table from the insn count (n / 2 | 1 buckets) and
               PRE numbers its reaching registers in bucket order, which is
               the order their spill slots are assigned. &hdr (sfp - 4)
               hashes to 13922 and i << 1, i << 2, i << 4 to 13966, 13967,
               13969: with these stores the table has 775 buckets (747
               against 16, 17, 19), so the givs take sp+32, 28 and 24
               before &hdr (sp+20) as in the native frame; without them 769
               (80 against 124, 125, 127) puts &hdr above the givs. 775
               needs 11 or 12 stores in every edition (Japanese is two
               insns longer). Without them differ 50 in every edition (en
               96 with the nested helpers' 46); every row they remove is a
               spill slot's offset. */
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = 0;
            hdr[0] = (u16)actorId;
            hdr[1] = 255;
            BattlePres_SetActorModesFar(hdr, 1);
            BattleLayout_HighlightPartyPanels(hdr);
            WaitFrames(1);
            mode = Ui_RunSelectionScreen(1);
            WaitFrames(1);
            if (mode == -2) {
                void *block;
                s32 handle;

                block = MemAlloc(12);
                handle = BattleParty_ListActorIdsFar(1, block);
                state->entryActive[2] = 0;
                WaitFrames(1);
                Ui_RunOwnerStatusScreen(block, handle, actorId);
                state->entryActive[2] = 1;
                MemFree(block);
                continue;
            }
            BattlePres_SetActorModesFar(hdr, 0);
            if (mode == -1) {
                if (i == 0) {
                    goto next_round;
                }
                i--;
                continue;
            }

            if (COUNTDOWN->timer == 0) {
                mode = 3;
            }
            slot = &COUNTDOWN->entries[0];
            slot->oam.word.attr23 = 0;
            slot->oam.word.attr01 = 0x80002400;
            slot->oam.f.tile = Resource_LoadIndexedIntoBuffer(res1024, mode);
            if (mode == 15) {
                slot->oam.f.x = 128;
            } else {
                slot->oam.f.x = 96;
            }
            slot->oam.f.y = 136;
            state->entryActive[0] = 1;
            switch (mode) {
            case 0:
                win = UiWindow_Create(11, 17, 11, 3, 6);
#if defined(TBS_EDITION_FR)
                UiText_DrawCharacterAtOffset((s32)MsgCommandName, (struct RenderInput *)win, 14, 0);
#else
                UiText_DrawCharacterAtOffset((s32)MsgCommandName, (struct RenderInput *)win, 16, 0);
#endif
                state->entries[0].oam.f.x = 64;
                AudioCommand_PlayFar(112);
                mode = BattleTarget_RunSelection(actorId, 1, 1, 0);
                UiWork_Finalize(win, 1);
                if (mode == -1) {
                    goto mark_visible;
                }
                kind = 0;
                aux = mode;
                ent->rng = 1;
                break;

            case 1: {
                struct MenuSprite *slot;
                struct BtlAct *ability;
                struct UiRenderWork *ui;
                u16 *list;
                u8 *pick;
                s32 cost;
                s32 handle;
                s32 cnt;
                s32 k;
                s32 cur;

                AudioCommand_PlayFar(112);
                mode = 0;
                /* FAKEMATCH: instruction order only: the one-pass block's loop
                   notes end the schedule here, so mode = 0 goes out before the
                   countdown base is loaded, as native (differ 4). */
                do {
                } while (0);
                COUNTDOWN->unk_34 = mode;
                COUNTDOWN->unk_30 = mode;
                COUNTDOWN->unk_38 = mode;
psynergy_menu:
                list = state->list;
                pick = state->pick;
                state->entries[0].oam.f.x = 48;
                cnt = 0;
                k = 0;
                cur = actor->action_slots[k].encoded_action & 0x3fff;
                if (cur != 0) {
                    do {
                        if ((BtlActGet(cur)->tgt_flg & 0x80) != 0) {
                            pick[cnt] = k;
                            list[cnt] = cur;
                            cnt++;
                        }
                        k++;
                        if (k == 32) {
                            break;
                        }
                        cur = actor->action_slots[k].encoded_action & 0x3fff;
                    } while (cur != 0);
                }
                pick[cnt] = 0;
                list[cnt] = 0;
                mode = BattleMenu_RunActionSelection(actorId, list, cnt);
                if (mode == -1) {
                    goto mark_visible;
                }
                param = actor->action_slots[pick[mode]].encoded_action & 0x3fff;
                ability = BtlActGet(param);
                cost = ability->rng;
                slot = &state->entries[1];
                handle = Resource_LoadIntoFreeSlot(128);
                ui = (struct UiRenderWork *)gWindowWork[0];
                win = UiWindow_Create(8, 17, 18, 3, 6);
                state->entries[0].oam.f.x = 40;
                slot->oam.word.attr01 = 0x40000400;
                slot->oam.word.attr23 = 0;
                slot->oam.f.x = win->x * 8 + 8;
                slot->oam.f.y = win->y * 8 + 4;
                /* FAKEMATCH: instruction order only (pool placement): arm_reorg counts this empty asm as a 4-byte insn, so the pool goes in after mov r1, r10 as native, whose constant 40 acts as a range-60 (zero_extendhisi2) pool load (differ 14). */
                __asm__ volatile ("");
                slot->oam.f.tile = Resource_LoadIndexedEntryToBuffer(param, handle);
                state->entryActive[1] = 1;
                ui->level = 5;
                if ((s32)ability->pp_cost > (s32)actor->ep) {
                    UiWork_SetParamNibble(2);
                } else if (actor->fuuji != 0) {
                    UiWork_SetParamNibble(9);
                }
                UiText_DrawCharacterAtOffset(param + (s32)MsgAbilityName, (struct RenderInput *)win, 16, 0);
                UiText_DrawNumberAtOffset(ability->pp_cost, 2, (struct RenderInput *)win, 104, 0);
                ui->level = 15;
                UiWork_SetParamNibble(15);
                UiWindow_SetTilemapEntry(win, 61471, 11, 0, 0);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
                UiWindow_SetTilemapEntry(win, 61445, 12, 0, 0);
#else
                UiWindow_SetTilemapEntry(win, 61470, 12, 0, 0);
#endif
                if (ability->damage_class != 4) {
                    UiWindow_SetTilemapEntry(win, ability->damage_class + 20481, 15, 0, 0);
                }
                ent->rng = cost;
                AudioCommand_PlayFar(112);
                mode = BattleTarget_RunSelection(actorId, ability->target_mode, cost,
                                                 Battle_ClassifyEntryKind((const struct BtlAct *)ability));
                state->entryActive[1] = 0;
                Resource_ResetEntry(handle);
                UiWork_Finalize(win, 1);
                if (mode == -1) {
                    goto psynergy_menu;
                }
                kind = 1;
                aux = mode;
                break;
            }

            case 16: {
                const struct SummonDefinition *djinn;
                struct BtlAct *ability;
                struct MenuSprite *slot;
                s32 handle;
                s32 ok;
                s32 j;
                s32 n;

                AudioCommand_PlayFar(112);
                COUNTDOWN->unk_34 = 0;
                COUNTDOWN->unk_30 = 0;
                COUNTDOWN->unk_38 = 0;
djinn_menu:
#if EDITION_INTERNATIONAL
                state->entries[0].oam.f.x = 80;
#else
                state->entries[0].oam.f.x = 104;
#endif
                BattleLayout_HighlightPartyPanels(hdr);
                mode = SummonMenu_SelectSummon(0, 0, state->cost[i]);
                BattleLayout_HighlightPartyPanels(hdr);
                if (mode == -1) {
                    goto mark_visible;
                }
                kind = 6;
                param = mode;
                WaitFrames(1);
                slot = &state->entries[1];
                djinn = SummonDefinition_Get(mode);
                ability = BtlActGet(djinn->name_message_id);
                handle = Resource_LoadIntoFreeSlot(128);
#if EDITION_INTERNATIONAL
                win = UiWindow_Create(10, 17, 17, 3, 6);
#else
                win = UiWindow_Create(10, 17, 14, 3, 6);
#endif
                n = 0;
                for (j = 0; j <= 3; j++) {
                    if (djinn->djinn_required[j] > state->cost[i][j]) {
                        break;
                    }
                }
                ok = (j == 4);
                state->entries[0].oam.f.x = 56;
                slot->oam.word.attr01 = 0x40000400;
                slot->oam.word.attr23 = 0;
                /* FAKEMATCH: instruction order only (pool placement, schedule): counted as 4 bytes, the empty asm puts the pool after mov r2, r9 as native (constant 56) and keeps ok = 1 - (j != 4) among the slot stores (differ 12). */
                __asm__ volatile ("");
                slot->oam.f.x = win->x * 8 + 8;
                slot->oam.f.y = win->y * 8 + 4;
                slot->oam.f.tile = Resource_LoadIndexedEntryToBuffer(djinn->name_message_id & 0x3fff, handle);
                state->entryActive[1] = 1;
                if (ok == 0) {
                    UiWork_SetParamNibble(2);
                }
                UiText_DrawCharacterAtOffset(SummonDefinition_Get(mode)->name_message_id + (s32)MsgAbilityName,
                                             win, 16, 0);
                for (j = 0; j <= 3; j++) {
                    if (djinn->djinn_required[j] != 0) {
#if EDITION_INTERNATIONAL
                        UiWindow_SetTilemapEntry(win, j + 20481, n * 2 + 13, 0, 0);
                        UiWindow_PutGlyph(win, djinn->djinn_required[j] + 48, n * 2 + 14, 0, 0);
#else
                        UiWindow_SetTilemapEntry(win, j + 20481, n * 2 + 10, 0, 0);
                        UiWindow_PutGlyph(win, djinn->djinn_required[j] + 48, n * 2 + 11, 0, 0);
#endif
                        n++;
                    }
                }
                if (ok != 0) {
                    AudioCommand_PlayFar(112);
                } else {
                    AudioCommand_PlayFar(114);
                }
                mode = BattleTarget_RunSelection(actorId, ability->target_mode, ability->rng,
                                                 Battle_ClassifyEntryKind((const struct BtlAct *)ability));
                ent->rng = ability->rng;
                state->entryActive[1] = 0;
                Resource_ResetEntry(handle);
                UiWork_Finalize(win, 1);
                if (mode == -1) {
                    goto djinn_menu;
                }
                for (j = 0; j <= 3; j++) {
                    if (djinn->djinn_required[j] > state->cost[i][j]) {
                        state->cost[i][j] = 0;
                        break;
                    }
                    state->cost[i][j] -= djinn->djinn_required[j];
                }
                aux = mode;
                break;
            }

            case 15: {
                struct BtlAct *ability;
                struct UiWindow *hint;
                s32 cost;
                s32 element;
                s32 index;

                AudioCommand_PlayFar(112);
                COUNTDOWN->unk_34 = 0;
                COUNTDOWN->unk_30 = 0;
                COUNTDOWN->unk_38 = 0;
summon_menu:
                state->entries[0].oam.f.x = 144;
                COUNTDOWN->result[i] = 0x80000000;
                mode = Battle_SelectAbility(actorId, 1);
                ent->rng = 0;
                if (mode == -1) {
                    goto mark_visible;
                }
                kind = 5;
                param = mode;
                COUNTDOWN->result[i] = mode;
                element = (param >> 8) & 15;
                index = param & 255;
                if (Djinn_IsActiveFar(actorId, element, index) != 0) {
                    ability = BtlActGet(Djinn_GetDefinitionHeaderFar(element, index));
                    cost = ability->rng;
                    win = UiWindow_Create(11, 17, 10, 3, 6);
                    state->entries[0].oam.f.x = 64;
                    UiWindow_SetTilemapEntry(win, element + 20481, 0, 0, 0);
                    UiText_DrawCharacterAtOffset(element * 20 + index + (s32)MsgDjinnName, win, 16, 0);
                    ent->rng = cost;
                    WaitFrames(1);
                    AudioCommand_PlayFar(112);
                    mode = BattleTarget_RunSelection(actorId, ability->target_mode, cost,
                                                     Battle_ClassifyEntryKind((const struct BtlAct *)ability));
                    if (state->hintCount != 0) {
                        hint = UiText_ShowMessageAndWaitComplete((s32)MsgDjinnUnleashHint, 15, 8);
                        while (UiWork_IsComplete() == 0) {
                            WaitFrames(1);
                        }
                        UiWork_Finalize(hint, 1);
                        state->hintCount++;
                        state->hintTimer = 45;
                    }
                    UiWork_Finalize(win, 1);
                    if (mode == -1) {
                        goto summon_menu;
                    }
                    aux = mode;
                    break;
                }
                win = UiWindow_Create(11, 17, 10, 3, 6);
                state->entries[0].oam.f.x = 64;
                UiWork_SetParamNibble(2);
                UiWindow_SetTilemapEntry(win, element + 20481, 0, 0, 0);
                UiText_DrawCharacterAtOffset(element * 20 + index + (s32)MsgDjinnName, win, 16, 0);
                UiWork_SetParamNibble(15);
                ent->rng = 1;
                WaitFrames(1);
                AudioCommand_PlayFar(112);
                mode = BattleTarget_RunSelection(actorId, 4, 0, 7);
                UiWork_Finalize(win, 1);
                if (mode == -1) {
                    goto summon_menu;
                }
                aux = mode;
                break;
            }

            case 2: {
                struct MenuSprite *slot;
                struct BtlAct *ability;
                u16 *list;
                u8 *pick;
                s32 cost;
                s32 handle;
                s32 cnt;
                s32 k;
                s32 cur;
                s32 sel;

                AudioCommand_PlayFar(112);
                mode = 0;
                /* FAKEMATCH: instruction order only: as in the psynergy case,
                   mode = 0 before the countdown base (differ 2). */
                do {
                } while (0);
                COUNTDOWN->unk_34 = mode;
                COUNTDOWN->unk_30 = mode;
                COUNTDOWN->unk_38 = mode;
item_menu:
                state->entries[0].oam.f.x = 96;
                pick = state->pick;
                list = state->list;
                cnt = 0;
                k = 0;
                cur = actor->inventory[k];
                if (cur != 0) {
                    do {
                        Item_GetDirectFar(cur);
                        if (Item_ClassifyUseAbility(actorId, cur) == 0) {
                            list[cnt] = cur;
                            pick[cnt++] = k;
                        }
                        k++;
                        if (k == 15) {
                            break;
                        }
                        cur = actor->inventory[k];
                    } while (cur != 0);
                }
                k = 0;
                cur = actor->inventory[k];
                if (cur != 0) {
                    do {
                        Item_GetDirectFar(cur);
                        if (Item_ClassifyUseAbility(actorId, cur) != 0) {
                            list[cnt] = cur;
                            pick[cnt++] = k;
                        }
                        k++;
                        if (k == 15) {
                            break;
                        }
                        cur = actor->inventory[k];
                    } while (cur != 0);
                }
                list[cnt] = 0;
                mode = ItemList_SelectEntry(actorId, list, cnt);
                if (mode == -1) {
                    goto mark_visible;
                }
                sel = pick[mode];
                param = sel;
                ability = BtlActGet(Item_GetDirectFar(actor->inventory[sel])->action_id);
                cost = ability->rng;
                slot = &state->entries[1];
                handle = Resource_LoadIntoFreeSlot(128);
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || \
    defined(TBS_EDITION_IT)
                win = UiWindow_Create(9, 17, 16, 3, 6);
#else
                win = UiWindow_Create(9, 17, 15, 3, 6);
#endif
                state->entries[0].oam.f.x = 48;
                slot->oam.word.attr01 = 0x40000400;
                slot->oam.word.attr23 = 0;
                /* FAKEMATCH: instruction order only (pool placement): counted as 4 bytes, the empty asm puts the pool after ldr r1, [sp, #72] as native (constant 48) (differ 10). */
                __asm__ volatile ("");
                slot->oam.f.x = win->x * 8 + 8;
                slot->oam.f.y = win->y * 8 + 4;
                slot->oam.f.tile = Resource_LoadKind26EntryToBuffer(actor->inventory[sel], handle);
                state->entryActive[1] = 1;
                UiText_DrawCharacterAtOffset((actor->inventory[sel] & 0x1ff) + (s32)MsgItemName,
                                             (struct RenderInput *)win, 24, 0);
                ent->rng = cost;
                AudioCommand_PlayFar(112);
                mode = BattleTarget_RunSelection(actorId, ability->target_mode, cost,
                                                 Battle_ClassifyEntryKind((const struct BtlAct *)ability));
                state->entryActive[1] = 0;
                Resource_ResetEntry(handle);
                UiWork_Finalize(win, 1);
                if (mode == -1) {
                    goto item_menu;
                }
                kind = 2;
                aux = mode;
                break;
            }

            case 3:
                kind = 3;
                break;
            }

            AudioCommand_PlayFar(110);
            ent->unit_id = (u16)actorId;
            order = actor->agi;
            if (order != 0) {
                order += (u32)(actor->agi * Random16()) >> 20;
            }
            ent->priority = order;
            if (i != 0 && in[i] == in[i - 1]) {
                ent->priority = (s16)order / 2;
            }
            if ((s16)ent->priority < 0) {
                ent->priority = 2000;
            }
            ent->command = kind;
            ent->parameter = param;
            ent->target = aux;
            i++;
            if (i < count) {
                continue;
            }
            break;
        }

next_round:
        if (i < count) {
            continue;
        }
        break;
    }

finish:
    if (state->enabled != 0) {
        sync = (u16 *)gSerialTransfer.payload;
        sync[4] = 'E';
        sync[5] = 'D';
        sync[6] = 'V';
        sync[7] = 'S';
    }
    if (state->displayHandle != 0) {
        UiWork_Finalize((struct UiWindow *)state->displayHandle, 1);
    }
    Resource_ResetEntry(res512);
    Resource_ResetEntry(res1024);
    Scheduler_RemoveCallback(UpdateLinkSessionCountdown);
    if (state->enabled != 0) {
        s32 DrawLocalizedResource(void)
        {
            u16 data[64];
#if EDITION_INTERNATIONAL
            UiText_CopyMessageString((s32)MsgWaitingForOpponent, data, 52);
#else
            UiText_DecodeMessage((s32)MsgWaitingForOpponent, data, 32);
#endif
            UiText_RenderWideStringAtOffset(data, (struct UiWindow *)state->displayHandle, 0, 4);
        }

        session = BtlWk;
        mode = 0;
        if (state->displayHandle == 0 && session->link_paused == 0) {
            state->displayHandle = (s32)UiWindow_Create(0, 16, 30, 4, 42);
            Ui_FillVramBlockPattern();
            DrawLocalizedResource();
        } else {
            state->displayHandle = 0;
        }
        for (;;) {
            peer = gLinkPeerSignatures[session->link_side ^ 1];
            if (session->link_paused != 0) {
                count = -1;
                break;
            }
            if ((gLinkStatus & 3) != 3) {
                mode++;
                if (mode > 24) {
                    count = -1;
                    break;
                }
            } else {
                mode = 0;
                if (peer[4] == 'E' && peer[5] == 'D' && peer[6] == 'V' && peer[7] == 'S') {
                    break;
                }
                if (!(peer[4] == 'V' && peer[5] == 'S' && peer[6] == 'S' && peer[7] == 'T')) {
                    mode = 1;
                }
            }
            WaitFrames(1);
        }
        if (state->displayHandle != 0) {
            UiWork_Finalize((struct UiWindow *)state->displayHandle, 1);
        }
    }
    Camera_ConfigureSceneFar(0);
    Runtime_ReleaseHeapBlock(57);
    return count;
}

void AffineEffect_UpdateFrame(void)
{
    struct ResourceMenuWork *work;
    struct ResourceMenuEntry *sprite;
    struct AffineTransform effect;
    s32 scale;
    s32 matrix;
    s32 edge;

    s32 y;
    u32 end;
    u32 i;
    u16 *line;
    u8 *buffers;

    work = gMenuSelectWork;
    sprite = work->entries;
    scale = Ui_BounceScales[(gFrameCount * 2) & 31];
    scale = (scale - 256) / 4 + 304;
    effect.scale_x = scale;
    effect.scale_y = scale;
    effect.angle = 0;
    matrix = AffineMatrix_BuildForEffect(&effect);
    for (i = 0; i < work->count; i++, sprite++) {
        u32 *attr;
        s32 slot;
        s32 px;

        px = sprite->x;
        if (px == 0)
            continue;
        if (i == work->selection) {
            s32 left = px + scale * 7 / 512 - 20;

            if (sprite->y != 0)
                y = sprite->y + scale * 3 / 256 - 20;
            else
                y = (scale * 15 / 256 - 30) & 255;
            attr = &sprite->link;
            *attr++ = 0;
            *attr++ = (matrix << 25) | y | (left << 16) | 0x80002300;
            *attr = gVramBlockCache[sprite->slot].offset >> 5;
            slot = 246;
        } else {
            u32 base;
            u32 value;

            attr = &sprite->link;

            /* FAKEMATCH: instruction order, measured 2 instructions without. */
            __asm__("" : : "r"(attr));
            y = sprite->y;
            *attr++ = 0;
            value = y | (px << 16) | 0x80002000;
            base = gVramBlockCache[sprite->slot].offset;
            *attr++ = value;
            *attr = base >> 5;
            slot = 245;
        }
        Runtime_PushSlotEntry(sprite, slot);
    }
    if (work->row == 0) {
        struct WindowBuffer *window;

        s32 span;

        buffers = gDisplayTransition;
        if (buffers == 0 || work->count == 0)
            return;
        window = (struct WindowBuffer *)(buffers + buffers[0x539] * 644);
        edge = work->entries[work->selection].x;
        span = (scale * 12 - 2817) / 256;
        edge = ((edge - span) << 8) + (edge + span) + 23;
        end = work->entries[work->selection].y + 24;
        end += (scale * 32 - 7937) / 512 + 1;
        line = &window->lines[24].span;
        i = 24;
        if (i < end) {
            do {
                *line = (*line & 0xff) | edge;
                i++;
                line += 2;
            } while (i < end);
        }
        edge = work->entries[0].x;
        if (work->selection == 0)
            edge -= (scale * 12 - 2817) / 256;
        edge <<= 8;
        line = &window->lines[0].span;
        for (i = 0; i <= 23; i++) {
            *line = (*line & 0xff) | edge;
            line += 2;
        }
    } else {
        struct WindowBuffer *window;
        s32 start;

        s32 span;

        buffers = gDisplayTransition;
        if (buffers == 0 || work->count == 0)
            return;
        window = (struct WindowBuffer *)(buffers + buffers[0x539] * 644);
        edge = work->entries[work->selection].x;
        span = (scale * 12 - 2817) / 256;
        edge = ((edge - span) << 8) + (edge + span) + 23;
        start = work->entries[work->selection].y;
        start -= (scale * 32 - 7937) / 512 + 1;
        line = &window->lines[start].span;
        i = start;
        if (i <= 135) {
            do {
                *line = (*line & 0xff) | edge;
                i++;
                line += 2;
            } while (i <= 135);
        }
        edge = work->entries[0].x;
        if (work->selection == 0)
            edge -= (scale * 12 - 2817) / 256;
        edge <<= 8;
        line = &window->lines[136].span;
        for (i = 136; i <= 159; i++) {
            *line = (*line & 0xff) | edge;
            line += 2;
        }
    }
}


void *AffineEffect_InitializeWork(void)
{
    void *work;
    volatile u32 zero;
    work = Runtime_AllocateBlock(58, 152);
    zero = 0;
    Dma_Set(&zero, work, 0x85000026, REG_DMA3);
    Scheduler_AddOrUpdateCallback(AffineEffect_UpdateFrame, 0xc76);
    return work;
}

void Menu_EndResourceSelection(void)
{
    struct UiWindow *child;
    s32 i;
    struct ResourceMenuWork *work;

    work = gMenuSelectWork;
    Scheduler_RemoveCallback(AffineEffect_UpdateFrame);
    child = work->window;
    if (child != 0) {
        UiWork_Finalize(child, 2);
    }
    i = 0;
    while (i < work->count) {
        Resource_ResetEntry(work->entries[i].slot);
        i += 1;
    }
    Runtime_ReleaseHeapBlock(0x3A);
    WaitFrames(1U);
}



void RenderOutput_PrepareForRedraw(void *);


void AudioCommand_PlayFar(s32);

s32 Menu_RunResourceSelectionLoop(s32 initial)
{
    struct ResourceMenuWork *work = gMenuSelectWork;
    s32 resource;

    work->selection = initial;
redraw:
    RenderOutput_PrepareForRedraw(work->window);
    if (work->resource_base != 0)
        resource = work->resource_base + work->selection;
    else
        resource = work->resource_ids[work->selection] + (s32)MsgCommandName;
    UiText_DrawCharacterAtOffset(resource, work->window, 0, 0);
    for (;;) {
        WaitFrames(1);
        if (gKeyState & KEY_A) {
            AudioCommand_PlayFar(112);
            return work->selection;
        }
        if (gKeyState & KEY_B) {
            AudioCommand_PlayFar(113);
            return -1;
        }
        if (gKeyState & KEY_START) {
            AudioCommand_PlayFar(113);
            return -1;
        }
        if ((gKeysRepeat & KEY_LEFT) || (gKeysRepeat & KEY_UP)) {
            AudioCommand_PlayFar(111);
            work->selection--;
            if (work->selection < 0)
                work->selection = work->count - 1;
            goto redraw;
        }
        if ((gKeysRepeat & KEY_RIGHT) || (gKeysRepeat & KEY_DOWN)) {
            AudioCommand_PlayFar(111);
            work->selection++;
            if (work->selection >= work->count)
                work->selection = 0;
            goto redraw;
        }
    }
}

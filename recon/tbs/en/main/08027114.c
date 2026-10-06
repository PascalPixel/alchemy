/* 08027114: differ 1802 against the 1727-line parent listing (draft 1801,
 * common 863; was 2131).
 * The draft also emits the two preceding nested helpers, 080270ac/080270d8,
 * now adopted on their own as Battle_DrawPartyPanelsWithEmptyList and
 * UiText_DrawWaitingForOpponent; the score counts them as extra lines.
 * Shared battle, action, summon, item, window and OAM layouts replace private
 * copies; serial payloads use their existing linker names. The parent frame is
 * 104 bytes against the native 100. Spill slots go out in pseudo order, top
 * down; native: out 88, in 84, count 80, ent 76, actor 72, aux 68, actorId 64,
 * kind 60, param 56, res1024 52, res512 48, i 44, djinn-case handle 40,
 * &state 36, then the i*2/i*4/i*16 givs 32/28/24, &hdr 20 and caller-save
 * slots 16..4 (ok is a caller-saved r4 at sp+4). The draft's one extra slot
 * is djinn: greg gives it no register (priority 5*38/158, below ok, cost and
 * handle; r5 holds a local, r6 the k*2+13 giv, r7 res, r8 win, r9 k, r10
 * ability, r11 ok, r4 slot). Native: r4 ok, r5/r6 the second loop's givs,
 * r7 k, r8 &entries[1], r9 djinn, r10 ability, r11 win. Declaring djinn
 * first leaves only out/in/count 4 bytes high. Below &state the draft makes
 * &hdr before the givs (draft 32, native 20).
 * Trials: scalar command fields unchanged; a structured handshake loop gives
 * 2245 before scoped initialization. Scoped counters give 2207; local ordering
 * 2187; ordinary record indexing 2172; command switch 2131.
 * 2026-10-06 T15: signatures as u16 rows (gLinkPeerSignatures[][12] and the
 * payload as u16, as WAIT_SYNC.C reads them) load the native pool constants
 * from the native base and keep each halfword compare separate: 2044. The
 * opening handshake as while, for(;;) with break, or for with WaitFrames as
 * its step all give 2044: GCC copies the exit test in front of the loop. A
 * goto loop entered at its peer test keeps the single test and the native
 * known-zero store: 1969. In the djinn case, taking &entries[1] before
 * SummonDefinition_Get and writing entries[0].x directly: 1892.
 * 2026-10-06 T15-08027114 s1: aux declared before actorId (native slot
 * order): 1864. handle declared after i: 1848. djinn declared first: 1824.
 * The closing handshake's session pointer as its own variable, so the
 * opening one crosses no call and stays in r0 as native: 1802.
 * Worse, reverted: block-local handles in the psynergy and item cases (native
 * keeps them in r10 and r9, the djinn one on the stack) 1867 on 1864;
 * SummonDefinition_Get(res) with the tile stored straight from the loader
 * 1943; every slot = &entries[0]; slot->x pair written directly 1880 (frame
 * 96: djinn still spilled, two caller-save slots fewer); a block-local
 * counter in the djinn case 1966; a block-local &entries[1] pointer there
 * 1948. Native state->timer = -1 loads its own -1 where the draft's CSE
 * derives it from the zero (subs r3, #1). */
#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "BATTLE_WORK.H"
#include "BATTLE_UNIT.H"
#include "ITEM.H"
#include "BATTLE_SUMMON.H"
#include "MENU_LIST.H"
#include "SERIAL_RUNTIME.H"
#include "text/MSG_IDS.H"

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
extern volatile u8 gDebugMode;
extern volatile u32 gKeysHeld;

void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
s32 Resource_LoadIndexedIntoBuffer(s32 handle, s32 index);
s32 Resource_LoadIndexedEntryToBuffer(s32 id, s32 handle);
s32 Resource_LoadKind26EntryToBuffer(s32 id, s32 handle);
void Graphics_ExpandVramTilesByColorTable(s32 destination);
void Link_DrawShiftedTilePair(s32 destination);
void Menu_BuildLocalizedPatternTiles(void);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 period);
void UpdateLinkSessionCountdown(void);
void Battle_DrawPartyPanelsWithEmptyList(void);
s32 Battle_ClassifyEntryKind(const struct BattleAction *entry);
s32 UiText_ShowLocalizedMessageAndWait(void);
s32 UiText_DrawWaitingForOpponent(void);
struct UiWindow *UiText_ShowMessageAndWaitComplete(s32 id, s32 x, s32 y);
struct UiWindow *UiWindow_Create(s32 a, s32 b, s32 c, s32 d, s32 e);
void UiWindow_SetTilemapEntry(struct UiWindow *win, s32 id, s32 pos, s32 arg3, u32 arg4);
void UiWindow_MarkVisibleTileAttributes(void);
void UiWork_SetParamNibble(s32 value);
void UiWork_Finalize(struct UiWindow *win, s32 release);
s32 UiWork_IsComplete(void);
s32 Ui_LoadEntryForKind(s32 kind, s32 handle);

struct BattleUnit *Runtime_GetObject(s32 actor);
struct BattleAction *Ability_GetData(s32 id);
struct ItemDefinition *Item_GetData(s32 id);
s32 GameFlag_IsSet(s32 flag);
s32 Item_ClassifyUseAbility(s32 actor, s32 item);
s32 BattleTarget_RunSelection(s32 actor, s32 a, s32 b, s32 kind);
void UiWindow_PutGlyph(struct UiWindow *win, s32 id, s32 pos, s32 arg3, u32 arg4);
s32 Ui_RunSelectionScreen(s32 mode);
void BattleLayout_HighlightPartyPanels(u16 *header);
void Ui_RunOwnerStatusScreen(void *block, s32 handle, s32 actor);
s32 Battle_SelectAbility(s32 actor, s32 mode);
s32 SummonMenu_SelectSummon(s32 a, s32 b, const u8 *standby);
s32 BattleMenu_RunActionSelection(s32 actor, u16 *list, s32 count);
s32 ItemList_SelectEntry(s32 actor, u16 *list, s32 count);
void Sys_Free(void *block);

s32 Djinn_GetDefinitionHeaderFar(s32 element, s32 index);
s32 Djinn_IsActiveFar(s32 actor, s32 element, s32 index);
s32 BattleParty_ListActorIdsFar(s32 mode, void *block);
void Camera_ConfigureSceneFar(s32 offset);
void BattlePres_SetActorModesFar(u16 *header, s32 mode);
void BattlePlacement_CountValidEntriesFar(s32 mode, u8 *cost);

s32 Battle_CollectPartyCommands(struct BattleActionRecord *out, u16 *in, s32 count)
{
    const struct SummonDefinition *djinn;
    struct LinkRoundState *state;
    struct BattleSession *link;
    struct BattleSession *session;
    u16 *peer;
    u16 *sync;
    struct BattleActionRecord *ent;
    struct BattleUnit *actor;
    struct UiWindow *win;
    struct UiWindow *hint;
    struct MenuSprite *slot;
    struct BattleAction *ability;
    struct UiRenderWork *ui;
    u16 *scan;
    u16 hdr[2];
    void *block;
    s32 aux;
    s32 actorId;
    s32 mode;
    s32 kind;
    s32 param;
    s32 res1024;
    s32 res512;
    s32 miss;
    s32 cnt;
    s32 sel;
    s32 cost;
    s32 element;
    s32 index;
    s32 ok;
    s32 i;
    s32 handle;
    s32 k;
    s32 cur;
    s32 order;
    s32 res;

    void DrawPartyPanels(void)
    {
        u16 data[2];
        data[0] = 0xff;
        BattleLayout_HighlightPartyPanels(data);
        BattlePres_SetActorModesFar(data, 1);
    }

    s32 DrawLocalizedResource(void)
    {
        u16 data[64];
        UiText_CopyMessageString((s32)MsgWaitingForOpponent, data, 52);
        UiText_RenderWideStringAtOffset(data, (struct UiWindow *)state->displayHandle, 0, 4);
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
    link = gBattleWork;
    {
        u32 *word = state->result;
        s32 n = 7;

        do {
            n--;
            *word++ = 0x80000000;
        } while (n >= 0);
    }
    {
        u8 *byte = state->entryActive;
        s32 n = 2;

        do {
            n--;
            *byte++ = 0;
        } while (n >= 0);
    }
    state->targetOffset = 0;
    state->currentOffset = 0;
    state->unk_3c = 0;
    state->unk_40 = 0;
    state->enabled = 0;
    state->secondaryHandle = 0;
    state->displayHandle = 0;
    state->timer = -1;
    if (link->two_sided != 0) {
        state->enabled = 1;
        sync = (u16 *)gSerialTransfer.payload;
        sync[4] = 'V';
        sync[5] = 'S';
        sync[6] = 'S';
        sync[7] = 'T';
        miss = 0;
        goto handshake_check;
handshake_loop:
        {
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
                    goto handshake_failed;
                }
            }
            WaitFrames(1);
        }
handshake_check:
        peer = gLinkPeerSignatures[link->link_side ^ 1];
        if (link->link_paused == 0) {
            goto handshake_loop;
        }
        count = -1;
handshake_failed:
        state->enabled = 0;
    }
handshake_done:
    Scheduler_AddOrUpdateCallback(UpdateLinkSessionCountdown, 3200);

    for (;;) {
        DrawPartyPanels();
        state->entryActive[2] = 0;
        state->actorId = -1;
        state->hintCount = 0;
        if (GameFlag_IsSet(366) != 0) {
            state->hintCount = 1;
            state->hintTimer = 60;
        }
        if (count > 0) {
            mode = Ui_RunSelectionScreen(0);
        } else {
            mode = 14;
        }
        if (mode == 7) {
            block = Runtime_BumpAllocate(12);
            if (gDebugMode != 0 && (gKeysHeld & KEY_START) != 0) {
                sel = 2;
            } else {
                sel = 1;
            }
            handle = BattleParty_ListActorIdsFar(sel, block);
            WaitFrames(1);
            Ui_RunOwnerStatusScreen(block, handle, *(u16 *)block);
            Sys_Free(block);
            continue;
        }
        if (mode == 4) {
            if (UiText_ShowLocalizedMessageAndWait() != 0) {
                continue;
            }
            ent = out;
            count = 1;
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
        Audio_PlayCue(154);
        i = 0;
        if (i >= count) {
            goto finish;
        }

        for (;;) {
            if (i == 0) {
                BattlePlacement_CountValidEntriesFar(0, state->cost[0]);
            } else {
                for (k = 0; k < 4; k++) {
                    state->cost[i][k] = state->cost[i - 1][k];
                }
            }

            ent = &out[i];
            actorId = in[i];
            actor = Runtime_GetObject(actorId);
            COUNTDOWN->actorId = actorId;
            COUNTDOWN->unk_40 = 0;
            slot = &COUNTDOWN->entries[2];
            slot->oam.word.attr01 = 0x80000400;
            slot->oam.word.attr23 = 0;
            res = Ui_LoadEntryForKind(actor->class_id, res512);
            slot->oam.f.tile = res;
            slot->oam.f.x = 0;
            slot->oam.f.y = 0x80;
            slot->oam.f.palette = 14;
            state->entryActive[2] = 1;

mark_visible:
            UiWindow_MarkVisibleTileAttributes();
            COUNTDOWN->entryActive[0] = 0;
            COUNTDOWN->result[i] = 0x80000000;
            hdr[0] = (u16)actorId;
            hdr[1] = 255;
            BattlePres_SetActorModesFar(hdr, 1);
            BattleLayout_HighlightPartyPanels(hdr);
            WaitFrames(1);
            mode = Ui_RunSelectionScreen(1);
            WaitFrames(1);
            if (mode == -2) {
                block = Runtime_BumpAllocate(12);
                handle = BattleParty_ListActorIdsFar(1, block);
                state->entryActive[2] = 0;
                WaitFrames(1);
                Ui_RunOwnerStatusScreen(block, handle, actorId);
                state->entryActive[2] = 1;
                Sys_Free(block);
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
            res = Resource_LoadIndexedIntoBuffer(res1024, mode);
            slot->oam.f.tile = res;
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
                UiText_DrawCharacterAtOffset(31, (struct RenderInput *)win, 16, 0);
                slot = &state->entries[0];
                slot->oam.f.x = 64;
                Audio_PlayCue(112);
                res = BattleTarget_RunSelection(actorId, 1, 1, 0);
                UiWork_Finalize(win, 1);
                if (res == -1) {
                    goto mark_visible;
                }
                kind = 0;
                aux = res;
                ent->range = 1;
                goto commit;

            case 1:
                Audio_PlayCue(112);
psynergy_menu:
                COUNTDOWN->unk_34 = 0;
                COUNTDOWN->unk_30 = 0;
                COUNTDOWN->unk_38 = 0;
                slot = &state->entries[0];
                slot->oam.f.x = 48;
                cnt = 0;
                k = 0;
                cur = actor->action_slots[0].encoded_action & 0x3fff;
                if (cur != 0) {
                    scan = &actor->action_slots[0].encoded_action;
                    do {
                        ability = Ability_GetData(cur);
                        if ((ability->target_flags & 0x80) != 0) {
                            state->pick[cnt] = (u8)k;
                            state->list[cnt] = (u16)cur;
                            cnt++;
                        }
                        k++;
                        if (k == 32) {
                            break;
                        }
                        scan += 2;
                        cur = *scan & 0x3fff;
                    } while (cur != 0);
                }
                state->pick[cnt] = 0;
                state->list[cnt] = 0;
                res = BattleMenu_RunActionSelection(actorId, state->list, cnt);
                if (res == -1) {
                    goto mark_visible;
                }
                sel = state->pick[res];
                param = actor->action_slots[sel].encoded_action & 0x3fff;
                ability = Ability_GetData(param);
                cost = ability->range;
                handle = Resource_LoadIntoFreeSlot(128);
                ui = (struct UiRenderWork *)gWindowWork[0];
                win = UiWindow_Create(8, 17, 18, 3, 6);
                slot = &state->entries[0];
                slot->oam.f.x = 40;
                slot = &state->entries[1];
                slot->oam.word.attr01 = 0x40000400;
                slot->oam.word.attr23 = 0;
                slot->oam.f.x = (win->x << 3) + 8;
                slot->oam.f.y = (u8)((win->y << 3) + 4);
                res = Resource_LoadIndexedEntryToBuffer(param, handle);
                slot->oam.f.tile = res;
                state->entryActive[1] = 1;
                ui->level = 5;
                if ((s32)ability->pp_cost > (s32)actor->pp) {
                    UiWork_SetParamNibble(2);
                } else if (actor->psy_seal != 0) {
                    UiWork_SetParamNibble(9);
                }
                UiText_DrawCharacterAtOffset(param + 819, (struct RenderInput *)win, 16, 0);
                UiText_DrawNumberAtOffset(ability->pp_cost, 2, (struct RenderInput *)win, 104, 0);
                ui->level = 15;
                UiWork_SetParamNibble(15);
                UiWindow_SetTilemapEntry(win, 61471, 11, 0, 0);
                UiWindow_SetTilemapEntry(win, 61470, 12, 0, 0);
                if (ability->damage_class != 4) {
                    UiWindow_SetTilemapEntry(win, ability->damage_class + 20481, 15, 0, 0);
                }
                ent->range = (u16)cost;
                Audio_PlayCue(112);
                res = Battle_ClassifyEntryKind((const struct BattleAction *)ability);
                res = BattleTarget_RunSelection(actorId, ability->target_mode, cost, res);
                state->entryActive[1] = 0;
                Resource_ResetEntry(handle);
                UiWork_Finalize(win, 1);
                if (res == -1) {
                    goto psynergy_menu;
                }
                kind = 1;
                goto store_target;

            case 2:
                Audio_PlayCue(112);
item_menu:
                COUNTDOWN->unk_34 = 0;
                COUNTDOWN->unk_30 = 0;
                COUNTDOWN->unk_38 = 0;
                slot = &state->entries[0];
                slot->oam.f.x = 96;
                cnt = 0;
                cur = actor->inventory[0];
                k = 0;
                if (cur != 0) {
                    do {
                        Item_GetData(cur);
                        if (Item_ClassifyUseAbility(actorId, cur) == 0) {
                            state->list[cnt] = (u16)cur;
                            state->pick[cnt] = (u8)k;
                            cnt++;
                        }
                        k++;
                        if (k == 15) {
                            break;
                        }
                        cur = actor->inventory[k];
                    } while (cur != 0);
                }
                cur = actor->inventory[0];
                k = 0;
                if (cur != 0) {
                    do {
                        Item_GetData(cur);
                        if (Item_ClassifyUseAbility(actorId, cur) != 0) {
                            state->list[cnt] = (u16)cur;
                            state->pick[cnt] = (u8)k;
                            cnt++;
                        }
                        k++;
                        if (k == 15) {
                            break;
                        }
                        cur = actor->inventory[k];
                    } while (cur != 0);
                }
                state->list[cnt] = 0;
                res = ItemList_SelectEntry(actorId, state->list, cnt);
                if (res == -1) {
                    goto mark_visible;
                }
                sel = state->pick[res];
                param = sel;
                ability = Ability_GetData(Item_GetData(actor->inventory[sel])->action_id);
                cost = ability->range;
                handle = Resource_LoadIntoFreeSlot(128);
                win = UiWindow_Create(9, 17, 15, 3, 6);
                slot = &state->entries[0];
                slot->oam.f.x = 48;
                slot = &state->entries[1];
                slot->oam.word.attr01 = 0x40000400;
                slot->oam.word.attr23 = 0;
                slot->oam.f.x = (win->x << 3) + 8;
                slot->oam.f.y = (u8)((win->y << 3) + 4);
                res = Resource_LoadKind26EntryToBuffer(actor->inventory[sel], handle);
                slot->oam.f.tile = res;
                state->entryActive[1] = 1;
                UiText_DrawCharacterAtOffset((actor->inventory[sel] & 0x1ff) + 386,
                                             win, 24, 0);
                ent->range = (u16)cost;
                Audio_PlayCue(112);
                res = Battle_ClassifyEntryKind((const struct BattleAction *)ability);
                res = BattleTarget_RunSelection(actorId, ability->target_mode, cost, res);
                state->entryActive[1] = 0;
                Resource_ResetEntry(handle);
                UiWork_Finalize(win, 1);
                if (res == -1) {
                    goto item_menu;
                }
                kind = 2;
                goto store_target;

            case 3:
                kind = 3;
                goto commit;

            case 15:
                Audio_PlayCue(112);
summon_menu:
                COUNTDOWN->unk_34 = 0;
                COUNTDOWN->unk_30 = 0;
                COUNTDOWN->unk_38 = 0;
                slot = &state->entries[0];
                slot->oam.f.x = 144;
                COUNTDOWN->result[i] = 0x80000000;
                res = Battle_SelectAbility(actorId, 1);
                ent->range = 0;
                if (res == -1) {
                    goto mark_visible;
                }
                kind = 5;
                param = res;
                COUNTDOWN->result[i] = res;
                element = (res >> 8) & 15;
                index = res & 255;
                if (Djinn_IsActiveFar(actorId, element, index) != 0) {
                    ability = Ability_GetData(Djinn_GetDefinitionHeaderFar(element, index));
                    cost = ability->range;
                    win = UiWindow_Create(11, 17, 10, 3, 6);
                    slot = &state->entries[0];
                    slot->oam.f.x = 64;
                    UiWindow_SetTilemapEntry(win, element + 20481, 0, 0, 0);
                    UiText_DrawCharacterAtOffset(element * 20 + index + 1119,
                                                 win, 16, 0);
                    ent->range = (u16)cost;
                    WaitFrames(1);
                    Audio_PlayCue(112);
                    res = Battle_ClassifyEntryKind((const struct BattleAction *)ability);
                    res = BattleTarget_RunSelection(actorId, ability->target_mode, cost, res);
                    if (state->hintCount != 0) {
                        hint = UiText_ShowMessageAndWaitComplete(3150, 15, 8);
                        while (UiWork_IsComplete() == 0) {
                            WaitFrames(1);
                        }
                        UiWork_Finalize(hint, 1);
                        state->hintCount = state->hintCount + 1;
                        state->hintTimer = 45;
                    }
                    UiWork_Finalize(win, 1);
                    if (res == -1) {
                        goto summon_menu;
                    }
                    goto store_target;
                } else {
                    win = UiWindow_Create(11, 17, 10, 3, 6);
                    slot = &state->entries[0];
                    slot->oam.f.x = 64;
                    UiWork_SetParamNibble(2);
                    UiWindow_SetTilemapEntry(win, element + 20481, 0, 0, 0);
                    UiText_DrawCharacterAtOffset(element * 20 + index + 1119,
                                                 win, 16, 0);
                    UiWork_SetParamNibble(15);
                    ent->range = 1;
                    WaitFrames(1);
                    Audio_PlayCue(112);
                    res = BattleTarget_RunSelection(actorId, 4, 0, 7);
                    UiWork_Finalize(win, 1);
                    if (res == -1) {
                        goto summon_menu;
                    }
                    goto store_target;
                }

            case 16:
                Audio_PlayCue(112);
djinn_menu:
                COUNTDOWN->unk_34 = 0;
                COUNTDOWN->unk_30 = 0;
                COUNTDOWN->unk_38 = 0;
                slot = &state->entries[0];
                slot->oam.f.x = 80;
                BattleLayout_HighlightPartyPanels(hdr);
                res = SummonMenu_SelectSummon(0, 0, state->cost[i]);
                BattleLayout_HighlightPartyPanels(hdr);
                if (res == -1) {
                    goto mark_visible;
                }
                kind = 6;
                param = res;
                WaitFrames(1);
                slot = &state->entries[1];
                djinn = SummonDefinition_Get(res);
                ability = Ability_GetData(djinn->name_message_id);
                handle = Resource_LoadIntoFreeSlot(128);
                win = UiWindow_Create(10, 17, 17, 3, 6);
                for (k = 0; k <= 3; k++) {
                    if (djinn->djinn_required[k] > state->cost[i][k]) {
                        break;
                    }
                }
                ok = (k == 4);
                state->entries[0].oam.f.x = 56;
                slot->oam.word.attr01 = 0x40000400;
                slot->oam.word.attr23 = 0;
                slot->oam.f.x = (win->x << 3) + 8;
                slot->oam.f.y = (u8)((win->y << 3) + 4);
                res = Resource_LoadIndexedEntryToBuffer(djinn->name_message_id & 0x3fff, handle);
                slot->oam.f.tile = res;
                state->entryActive[1] = 1;
                if (ok == 0) {
                    UiWork_SetParamNibble(2);
                }
                UiText_DrawCharacterAtOffset(SummonDefinition_Get(param)->name_message_id + 819,
                                             win, 16, 0);
                for (k = 0; k <= 3; k++) {
                    if (djinn->djinn_required[k] != 0) {
                        UiWindow_SetTilemapEntry(win, k + 20481, k * 2 + 13, 0, 0);
                        UiWindow_PutGlyph(win, djinn->djinn_required[k] + 48, k * 2 + 14, 0, 0);
                    }
                }
                if (ok != 0) {
                    Audio_PlayCue(112);
                } else {
                    Audio_PlayCue(114);
                }
                res = Battle_ClassifyEntryKind((const struct BattleAction *)ability);
                res = BattleTarget_RunSelection(actorId, ability->target_mode, ability->range, res);
                ent->range = ability->range;
                state->entryActive[1] = 0;
                Resource_ResetEntry(handle);
                UiWork_Finalize(win, 1);
                if (res == -1) {
                    goto djinn_menu;
                }
                for (k = 0; k <= 3; k++) {
                    if (djinn->djinn_required[k] > state->cost[i][k]) {
                        state->cost[i][k] = 0;
                        break;
                    }
                    state->cost[i][k] = (u8)(state->cost[i][k] - djinn->djinn_required[k]);
                }
                goto store_target;

            default:
                goto commit;
            }

store_target:
            aux = res;

commit:
            Audio_PlayCue(110);
            ent->unit_id = (u16)actorId;
            order = actor->agility;
            if (order != 0) {
                res = Random16();
                order += (s32)((u32)(actor->agility * res) >> 20);
            }
            ent->priority = (u16)order;
            if (i != 0) {
                scan = &in[i];
                if (scan[0] == *(scan - 1)) {
                    ent->priority = (u16)((s16)order / 2);
                }
            }
            if ((s16)ent->priority < 0) {
                ent->priority = 2000;
            }
            ent->command = (u16)kind;
            ent->parameter = (u16)param;
            ent->target = (u16)aux;
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
        session = gBattleWork;
        miss = 0;
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
                miss++;
                if (miss > 24) {
                    count = -1;
                    break;
                }
            } else {
                miss = 0;
                if (peer[4] == 'E' && peer[5] == 'D' && peer[6] == 'V' && peer[7] == 'S') {
                    break;
                }
                if (!(peer[4] == 'V' && peer[5] == 'S' && peer[6] == 'S' && peer[7] == 'T')) {
                    miss = 1;
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

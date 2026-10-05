#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "IO_REG.H"

typedef s32 M2C_UNK;
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

#define LINK_LOCAL  ((struct LinkPacket *)0x02002224)
#define LINK_PEERS  ((struct LinkPacket *)0x02002024)

struct LinkPacket {
    u16 tag[4];
    u16 code[4];
    u8 unk_10[8];
};

struct LinkWork {
    u8 unk_00[0x44];
    u8 enabled;
    u8 unk_45[0x0b];
    u8 side;
    u8 unk_51;
    u8 paused;
};

struct LinkRoundState {
    u8 entries[3][12];
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

struct BattleCommandEntry {
    u16 actor;
    u16 unk_02;
    u16 order;
    u16 command;
    u16 param;
    u16 target;
    u16 sub;
    u16 unk_0e;
};

struct BattleActorWork {
    u8 unk_000[0x3a];
    s16 rank;
    u8 unk_03c[4];
    u16 agility;
    u8 unk_042[0x16];
    u16 ability[32][2];
    u16 item[15];
    u8 unk_0f6[0x32];
    u8 kind;
    u8 unk_129[0x14];
    u8 flag_13d;
};

struct UiWindowWork {
    u8 unk_00[12];
    u16 field_0c;
    u16 field_0e;
};

struct DjinnDefinition {
    u16 id;
    u16 unk_02;
    u8 cost[4];
};

extern struct LinkWork *gBattleWork;
extern struct LinkRoundState *gLinkCountdownWork;
extern volatile u16 gLinkStatus;
extern volatile u8 gDebugMode;
extern volatile u32 gKeysHeld;
extern u8 *gWindowWork;

void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
s32 Resource_LoadIndexedIntoBuffer(s32 handle, s32 index);
s32 Resource_LoadIndexedEntryToBuffer(s32 id, s32 handle);
s32 Resource_LoadKind26EntryToBuffer(s32 id, s32 handle);
void Graphics_ExpandVramTilesByColorTable(s32 destination);
void Link_DrawShiftedTilePair(s32 destination);
void Menu_BuildLocalizedPatternTiles(void);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 period);
void Scheduler_RemoveCallback(void (*callback)(void));
void UpdateLinkSessionCountdown(void);
void Battle_DrawPartyPanelsWithEmptyList(void);
s32 Battle_ClassifyEntryKind(const struct BattleAction *entry);
s32 UiText_ShowLocalizedMessageAndWait(void);
s32 UiText_DrawWaitingForOpponent(void);
struct UiWindowWork *UiText_ShowMessageAndWaitComplete(s32 id, s32 x, s32 y);
struct UiWindowWork *UiWindow_Create(s32 a, s32 b, s32 c, s32 d, s32 e);
void UiWindow_SetTilemapEntry(struct UiWindowWork *win, s32 id, s32 pos, s32 arg3, s32 arg4);
void UiWindow_MarkVisibleTileAttributes(void);
void UiText_DrawCharacterAtOffset(s32 id, struct UiWindowWork *win, s32 pos, s32 arg3);
void UiText_DrawNumberAtOffset(s32 value, s32 width, struct UiWindowWork *win, s32 pos, s32 arg4);
void UiWork_SetParamNibble(s32 value);
void UiWork_Finalize(struct UiWindowWork *win, s32 release);
s32 UiWork_IsComplete(void);
s32 Ui_LoadEntryForKind(s32 kind, s32 handle);
void Ui_FillVramBlockPattern(void);
struct BattleActorWork *Runtime_GetObject(s32 actor);
u8 *Ability_GetData(s32 id);
u8 *Item_GetData(s32 id);
s32 GameFlag_IsSet(s32 flag);
s32 Item_ClassifyUseAbility(s32 actor, s32 item);
s32 BattleTarget_RunSelection(s32 actor, s32 a, s32 b, s32 kind);
s32 Random16(void);
void UiWindow_PutGlyph(struct UiWindowWork *win, s32 id, s32 pos, s32 arg3, s32 arg4);
s32 Ui_Place(s32, s16 *, s32);
s32 Ui_SetMode(s16 *, s32, s32, s32);
s32 Ui_RunSelectionScreen(s32 mode);
void BattleLayout_HighlightPartyPanels(u16 *header);
void Ui_RunOwnerStatusScreen(void *block, s32 handle, s32 actor);
s32 Battle_SelectAbility(s32 actor, s32 mode);
s32 SummonMenu_SelectSummon(s32 a, s32 b, const u8 *standby);
s32 BattleMenu_RunActionSelection(s32 actor, u16 *list, s32 count);
s32 ItemList_SelectEntry(s32 actor, u16 *list, s32 count);
void Sys_Free(void *block);
struct DjinnDefinition *SummonDefinition_Get(s32 id);
s32 Djinn_GetDefinitionHeaderFar(s32 element, s32 index);
s32 Djinn_IsActiveFar(s32 actor, s32 element, s32 index);
s32 BattleParty_ListActorIdsFar(s32 mode, void *block);
void Camera_ConfigureSceneFar(s32 offset);
void BattlePres_SetActorModesFar(u16 *header, s32 mode);
void BattlePlacement_CountValidEntriesFar(s32 mode, u8 *cost);

s32 Battle_CollectPartyCommands(struct BattleCommandEntry *out, u16 *in, s32 count)
{
    struct LinkRoundState *state;
    struct LinkWork *link;
    struct LinkPacket *peer;
    struct BattleCommandEntry *ent;
    struct BattleActorWork *actor;
    struct UiWindowWork *win;
    struct UiWindowWork *hint;
    struct DjinnDefinition *djinn;
    u8 *slot;
    u8 *ability;
    u8 *ui;
    u16 *scan;
    u16 hdr[2];
    void *block;
    s32 res1024;
    s32 res512;
    s32 actorId;
    s32 kind;
    s32 param;
    s32 aux;
    s32 mode;
    s32 miss;
    s32 cnt;
    s32 sel;
    s32 cost;
    s32 handle;
    s32 element;
    s32 index;
    s32 ok;
    s32 i;
    s32 k;
    s32 off2;
    s32 off4;
    s32 off16;
    s32 cur;
    s32 order;
    u32 *word;
    u8 *byte;
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
        s16 data[64];
        Ui_Place(0x80d, data, 0x34);
        return Ui_SetMode(data, state->displayHandle, 0, 4);
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
    word = state->result;
    k = 7;
    do {
        k--;
        *word++ = 0x80000000;
    } while (k >= 0);
    byte = state->entryActive;
    k = 2;
    do {
        k--;
        *byte++ = 0;
    } while (k >= 0);
    state->targetOffset = 0;
    state->currentOffset = 0;
    state->unk_3c = 0;
    state->unk_40 = 0;
    state->enabled = 0;
    state->secondaryHandle = 0;
    state->displayHandle = 0;
    state->timer = -1;
    if (link->enabled != 0) {
        state->enabled = 1;
        LINK_LOCAL->code[0] = 'V';
        LINK_LOCAL->code[1] = 'S';
        LINK_LOCAL->code[2] = 'S';
        LINK_LOCAL->code[3] = 'T';
        miss = 0;
        for (;;) {
            peer = &LINK_PEERS[link->side ^ 1];
            if (link->paused != 0) {
                goto handshake_failed;
            }
            if ((gLinkStatus & 3) != 3) {
                miss++;
                if (miss > 24) {
                    goto handshake_failed;
                }
            } else {
                miss = 0;
                if (peer->code[0] == 'V' && peer->code[1] == 'S' &&
                    peer->code[2] == 'S' && peer->code[3] == 'T') {
                    goto handshake_done;
                }
                if (peer->code[0] == 'E' && peer->code[1] == 'D' &&
                    peer->code[2] == 'V' && peer->code[3] == 'S') {
                    goto handshake_done;
                }
                if (!(peer->tag[0] == 'E' && peer->tag[1] == 'X' &&
                      peer->tag[2] == 'E' && peer->tag[3] == 'C')) {
                    goto handshake_failed;
                }
            }
            WaitFrames(1);
        }
handshake_failed:
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
            ent->actor = in[0];
            ent->order = 0x7ffe;
            ent->command = 99;
            ent->param = 0;
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
        off4 = 0;
        off16 = 0;
        off2 = 0;

        for (;;) {
            if (i == 0) {
                BattlePlacement_CountValidEntriesFar(0, state->cost[0]);
            } else {
                for (k = 0; k < 4; k++) {
                    state->cost[i][k] = state->cost[i - 1][k];
                }
            }

            ent = (struct BattleCommandEntry *)((u8 *)out + off16);
            actorId = *(u16 *)((u8 *)in + off2);
            actor = Runtime_GetObject(actorId);
            gLinkCountdownWork->actorId = actorId;
            gLinkCountdownWork->unk_40 = 0;
            slot = gLinkCountdownWork->entries[2];
            M2C_FIELD(slot, u32 *, 4) = 0x80000400;
            M2C_FIELD(slot, u32 *, 8) = 0;
            res = Ui_LoadEntryForKind(actor->kind, res512);
            M2C_FIELD(slot, u16 *, 8) =
                (u16)((M2C_FIELD(slot, u16 *, 8) & ~0x3ff) | (res & 0x3ff));
            M2C_FIELD(slot, u16 *, 6) = (u16)(M2C_FIELD(slot, u16 *, 6) & ~0x1ff);
            M2C_FIELD(slot, u8 *, 4) = 0x80;
            M2C_FIELD(slot, u8 *, 9) = (u8)((M2C_FIELD(slot, u8 *, 9) & 15) | 0xe0);
            state->entryActive[2] = 1;

mark_visible:
            UiWindow_MarkVisibleTileAttributes();
            gLinkCountdownWork->entryActive[0] = 0;
            gLinkCountdownWork->result[i] = 0x80000000;
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
                off4 = i * 4;
                off16 = i * 16;
                off2 = i * 2;
                continue;
            }

            if (gLinkCountdownWork->timer == 0) {
                mode = 3;
            }
            slot = gLinkCountdownWork->entries[0];
            M2C_FIELD(slot, u32 *, 8) = 0;
            M2C_FIELD(slot, u32 *, 4) = 0x80002400;
            res = Resource_LoadIndexedIntoBuffer(res1024, mode);
            M2C_FIELD(slot, u16 *, 8) =
                (u16)((M2C_FIELD(slot, u16 *, 8) & ~0x3ff) | (res & 0x3ff));
            if (mode == 15) {
                M2C_FIELD(slot, u16 *, 6) =
                    (u16)((M2C_FIELD(slot, u16 *, 6) & ~0x1ff) | 128);
            } else {
                M2C_FIELD(slot, u16 *, 6) =
                    (u16)((M2C_FIELD(slot, u16 *, 6) & ~0x1ff) | 96);
            }
            M2C_FIELD(slot, u8 *, 4) = 136;
            state->entryActive[0] = 1;
            switch (mode) {
            case 0:
                win = UiWindow_Create(11, 17, 11, 3, 6);
                UiText_DrawCharacterAtOffset(31, win, 16, 0);
                slot = state->entries[0];
                M2C_FIELD(slot, u16 *, 6) =
                    (u16)((M2C_FIELD(slot, u16 *, 6) & ~0x1ff) | 64);
                Audio_PlayCue(112);
                res = BattleTarget_RunSelection(actorId, 1, 1, 0);
                UiWork_Finalize(win, 1);
                if (res == -1) {
                    goto mark_visible;
                }
                kind = 0;
                aux = res;
                ent->sub = 1;
                goto commit;

            case 1:
                Audio_PlayCue(112);
psynergy_menu:
                gLinkCountdownWork->unk_34 = 0;
                gLinkCountdownWork->unk_30 = 0;
                gLinkCountdownWork->unk_38 = 0;
                slot = state->entries[0];
                M2C_FIELD(slot, u16 *, 6) =
                    (u16)((M2C_FIELD(slot, u16 *, 6) & ~0x1ff) | 48);
                cnt = 0;
                k = 0;
                cur = actor->ability[0][0] & 0x3fff;
                if (cur != 0) {
                    scan = &actor->ability[0][0];
                    do {
                        ability = Ability_GetData(cur);
                        if ((ability[1] & 0x80) != 0) {
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
                param = actor->ability[sel][0] & 0x3fff;
                ability = Ability_GetData(param);
                cost = ability[8];
                handle = Resource_LoadIntoFreeSlot(128);
                ui = gWindowWork;
                win = UiWindow_Create(8, 17, 18, 3, 6);
                slot = state->entries[0];
                M2C_FIELD(slot, u16 *, 6) =
                    (u16)((M2C_FIELD(slot, u16 *, 6) & ~0x1ff) | 40);
                slot = state->entries[1];
                M2C_FIELD(slot, u32 *, 4) = 0x40000400;
                M2C_FIELD(slot, u32 *, 8) = 0;
                M2C_FIELD(slot, u16 *, 6) =
                    (u16)((M2C_FIELD(slot, u16 *, 6) & ~0x1ff) |
                          (((win->field_0c << 3) + 8) & 0x1ff));
                M2C_FIELD(slot, u8 *, 4) = (u8)((win->field_0e << 3) + 4);
                res = Resource_LoadIndexedEntryToBuffer(param, handle);
                M2C_FIELD(slot, u16 *, 8) =
                    (u16)((M2C_FIELD(slot, u16 *, 8) & ~0x3ff) | (res & 0x3ff));
                state->entryActive[1] = 1;
                ui[3751] = 5;
                if ((s32)ability[9] > (s32)actor->rank) {
                    UiWork_SetParamNibble(2);
                } else if (actor->flag_13d != 0) {
                    UiWork_SetParamNibble(9);
                }
                UiText_DrawCharacterAtOffset(param + 819, win, 16, 0);
                UiText_DrawNumberAtOffset(ability[9], 2, win, 104, 0);
                ui[3751] = 15;
                UiWork_SetParamNibble(15);
                UiWindow_SetTilemapEntry(win, 61471, 11, 0, 0);
                UiWindow_SetTilemapEntry(win, 61470, 12, 0, 0);
                if (ability[2] != 4) {
                    UiWindow_SetTilemapEntry(win, ability[2] + 20481, 15, 0, 0);
                }
                ent->sub = (u16)cost;
                Audio_PlayCue(112);
                res = Battle_ClassifyEntryKind((const struct BattleAction *)ability);
                res = BattleTarget_RunSelection(actorId, ability[0], cost, res);
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
                gLinkCountdownWork->unk_34 = 0;
                gLinkCountdownWork->unk_30 = 0;
                gLinkCountdownWork->unk_38 = 0;
                slot = state->entries[0];
                M2C_FIELD(slot, u16 *, 6) =
                    (u16)((M2C_FIELD(slot, u16 *, 6) & ~0x1ff) | 96);
                cnt = 0;
                cur = actor->item[0];
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
                        cur = actor->item[k];
                    } while (cur != 0);
                }
                cur = actor->item[0];
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
                        cur = actor->item[k];
                    } while (cur != 0);
                }
                state->list[cnt] = 0;
                res = ItemList_SelectEntry(actorId, state->list, cnt);
                if (res == -1) {
                    goto mark_visible;
                }
                sel = state->pick[res];
                param = sel;
                ability = Ability_GetData(M2C_FIELD(Item_GetData(actor->item[sel]),
                                                    u16 *, 40));
                cost = ability[8];
                handle = Resource_LoadIntoFreeSlot(128);
                win = UiWindow_Create(9, 17, 15, 3, 6);
                slot = state->entries[0];
                M2C_FIELD(slot, u16 *, 6) =
                    (u16)((M2C_FIELD(slot, u16 *, 6) & ~0x1ff) | 48);
                slot = state->entries[1];
                M2C_FIELD(slot, u32 *, 4) = 0x40000400;
                M2C_FIELD(slot, u32 *, 8) = 0;
                M2C_FIELD(slot, u16 *, 6) =
                    (u16)((M2C_FIELD(slot, u16 *, 6) & ~0x1ff) |
                          (((win->field_0c << 3) + 8) & 0x1ff));
                M2C_FIELD(slot, u8 *, 4) = (u8)((win->field_0e << 3) + 4);
                res = Resource_LoadKind26EntryToBuffer(actor->item[sel], handle);
                M2C_FIELD(slot, u16 *, 8) =
                    (u16)((M2C_FIELD(slot, u16 *, 8) & ~0x3ff) | (res & 0x3ff));
                state->entryActive[1] = 1;
                UiText_DrawCharacterAtOffset((actor->item[sel] & 0x1ff) + 386,
                                             win, 24, 0);
                ent->sub = (u16)cost;
                Audio_PlayCue(112);
                res = Battle_ClassifyEntryKind((const struct BattleAction *)ability);
                res = BattleTarget_RunSelection(actorId, ability[0], cost, res);
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
                gLinkCountdownWork->unk_34 = 0;
                gLinkCountdownWork->unk_30 = 0;
                gLinkCountdownWork->unk_38 = 0;
                slot = state->entries[0];
                M2C_FIELD(slot, u16 *, 6) =
                    (u16)((M2C_FIELD(slot, u16 *, 6) & ~0x1ff) | 144);
                gLinkCountdownWork->result[i] = 0x80000000;
                res = Battle_SelectAbility(actorId, 1);
                ent->sub = 0;
                if (res == -1) {
                    goto mark_visible;
                }
                kind = 5;
                param = res;
                gLinkCountdownWork->result[i] = res;
                element = (res >> 8) & 15;
                index = res & 255;
                if (Djinn_IsActiveFar(actorId, element, index) != 0) {
                    ability = Ability_GetData(Djinn_GetDefinitionHeaderFar(element, index));
                    cost = ability[8];
                    win = UiWindow_Create(11, 17, 10, 3, 6);
                    slot = state->entries[0];
                    M2C_FIELD(slot, u16 *, 6) =
                        (u16)((M2C_FIELD(slot, u16 *, 6) & ~0x1ff) | 64);
                    UiWindow_SetTilemapEntry(win, element + 20481, 0, 0, 0);
                    UiText_DrawCharacterAtOffset(element * 20 + index + 1119,
                                                 win, 16, 0);
                    ent->sub = (u16)cost;
                    WaitFrames(1);
                    Audio_PlayCue(112);
                    res = Battle_ClassifyEntryKind((const struct BattleAction *)ability);
                    res = BattleTarget_RunSelection(actorId, ability[0], cost, res);
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
                    slot = state->entries[0];
                    M2C_FIELD(slot, u16 *, 6) =
                        (u16)((M2C_FIELD(slot, u16 *, 6) & ~0x1ff) | 64);
                    UiWork_SetParamNibble(2);
                    UiWindow_SetTilemapEntry(win, element + 20481, 0, 0, 0);
                    UiText_DrawCharacterAtOffset(element * 20 + index + 1119,
                                                 win, 16, 0);
                    UiWork_SetParamNibble(15);
                    ent->sub = 1;
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
                gLinkCountdownWork->unk_34 = 0;
                gLinkCountdownWork->unk_30 = 0;
                gLinkCountdownWork->unk_38 = 0;
                slot = state->entries[0];
                M2C_FIELD(slot, u16 *, 6) =
                    (u16)((M2C_FIELD(slot, u16 *, 6) & ~0x1ff) | 80);
                BattleLayout_HighlightPartyPanels(hdr);
                res = SummonMenu_SelectSummon(0, 0, state->cost[i]);
                BattleLayout_HighlightPartyPanels(hdr);
                if (res == -1) {
                    goto mark_visible;
                }
                kind = 6;
                param = res;
                WaitFrames(1);
                djinn = SummonDefinition_Get(res);
                ability = Ability_GetData(djinn->id);
                handle = Resource_LoadIntoFreeSlot(128);
                win = UiWindow_Create(10, 17, 17, 3, 6);
                for (k = 0; k <= 3; k++) {
                    if (djinn->cost[k] > state->cost[i][k]) {
                        break;
                    }
                }
                ok = (k == 4);
                slot = state->entries[0];
                M2C_FIELD(slot, u16 *, 6) =
                    (u16)((M2C_FIELD(slot, u16 *, 6) & ~0x1ff) | 56);
                slot = state->entries[1];
                M2C_FIELD(slot, u32 *, 4) = 0x40000400;
                M2C_FIELD(slot, u32 *, 8) = 0;
                M2C_FIELD(slot, u16 *, 6) =
                    (u16)((M2C_FIELD(slot, u16 *, 6) & ~0x1ff) |
                          (((win->field_0c << 3) + 8) & 0x1ff));
                M2C_FIELD(slot, u8 *, 4) = (u8)((win->field_0e << 3) + 4);
                res = Resource_LoadIndexedEntryToBuffer(djinn->id & 0x3fff, handle);
                M2C_FIELD(slot, u16 *, 8) =
                    (u16)((M2C_FIELD(slot, u16 *, 8) & ~0x3ff) | (res & 0x3ff));
                state->entryActive[1] = 1;
                if (ok == 0) {
                    UiWork_SetParamNibble(2);
                }
                UiText_DrawCharacterAtOffset(SummonDefinition_Get(param)->id + 819,
                                             win, 16, 0);
                for (k = 0; k <= 3; k++) {
                    if (djinn->cost[k] != 0) {
                        UiWindow_SetTilemapEntry(win, k + 20481, k * 2 + 13, 0, 0);
                        UiWindow_PutGlyph(win, djinn->cost[k] + 48, k * 2 + 14, 0, 0);
                    }
                }
                if (ok != 0) {
                    Audio_PlayCue(112);
                } else {
                    Audio_PlayCue(114);
                }
                res = Battle_ClassifyEntryKind((const struct BattleAction *)ability);
                res = BattleTarget_RunSelection(actorId, ability[0], ability[8], res);
                ent->sub = ability[8];
                state->entryActive[1] = 0;
                Resource_ResetEntry(handle);
                UiWork_Finalize(win, 1);
                if (res == -1) {
                    goto djinn_menu;
                }
                for (k = 0; k <= 3; k++) {
                    if (djinn->cost[k] > state->cost[i][k]) {
                        state->cost[i][k] = 0;
                        break;
                    }
                    state->cost[i][k] = (u8)(state->cost[i][k] - djinn->cost[k]);
                }
                goto store_target;

            default:
                goto commit;
            }

store_target:
            aux = res;

commit:
            Audio_PlayCue(110);
            ent->actor = (u16)actorId;
            order = actor->agility;
            if (order != 0) {
                res = Random16();
                order += (s32)((u32)(actor->agility * res) >> 20);
            }
            ent->order = (u16)order;
            if (i != 0) {
                scan = (u16 *)((u8 *)in + off2);
                if (scan[0] == *(scan - 1)) {
                    ent->order = (u16)((s16)order / 2);
                }
            }
            if ((s16)ent->order < 0) {
                ent->order = 2000;
            }
            ent->command = (u16)kind;
            ent->param = (u16)param;
            ent->target = (u16)aux;
            i++;
            off4 = i * 4;
            off16 = i * 16;
            off2 = i * 2;
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
        LINK_LOCAL->code[0] = 'E';
        LINK_LOCAL->code[1] = 'D';
        LINK_LOCAL->code[2] = 'V';
        LINK_LOCAL->code[3] = 'S';
    }
    if (state->displayHandle != 0) {
        UiWork_Finalize((struct UiWindowWork *)state->displayHandle, 1);
    }
    Resource_ResetEntry(res512);
    Resource_ResetEntry(res1024);
    Scheduler_RemoveCallback(UpdateLinkSessionCountdown);
    if (state->enabled != 0) {
        link = gBattleWork;
        miss = 0;
        if (state->displayHandle == 0 && link->paused == 0) {
            state->displayHandle = (s32)UiWindow_Create(0, 16, 30, 4, 42);
            Ui_FillVramBlockPattern();
            DrawLocalizedResource();
        } else {
            state->displayHandle = 0;
        }
        for (;;) {
            peer = &LINK_PEERS[link->side ^ 1];
            if (link->paused != 0) {
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
                if (peer->code[0] == 'E' && peer->code[1] == 'D' &&
                    peer->code[2] == 'V' && peer->code[3] == 'S') {
                    break;
                }
                if (!(peer->code[0] == 'V' && peer->code[1] == 'S' &&
                      peer->code[2] == 'S' && peer->code[3] == 'T')) {
                    miss = 1;
                }
            }
            WaitFrames(1);
        }
        if (state->displayHandle != 0) {
            UiWork_Finalize((struct UiWindowWork *)state->displayHandle, 1);
        }
    }
    Camera_ConfigureSceneFar(0);
    Runtime_ReleaseHeapBlock(57);
    return count;
}

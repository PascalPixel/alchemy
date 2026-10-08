/* 08027114: differ 46 against the 1727-line parent listing (draft 1773,
 * common 1727): only the two nested helpers' lines (see below); the parent
 * matches, and by symbol differ 0 in every other edition. Frame 100 as
 * native.
 * 2026-10-08: the GCSE bucket-order device (dead hdr stores, see its tag)
 * gives the native spill slots: 150 -> 102. An empty asm in each of the
 * psynergy, djinn and item cases counts four bytes in arm_reorg and puts
 * the literal pool where native has it (and the djinn ok computation
 * among the slot stores): 102 -> 66. One-pass blocks after mode = 0 in the
 * psynergy and item cases and after the owner status call, and the actor
 * lookup called as returning a doubleword: 66 -> 56. French draws the
 * command name at 14, and the European item window is 16 wide.
 * A one-pass block around the opening handshake loop: 56 -> 52.
 * The mismatch and closing-check exits each clear state->enabled (jump2
 * cross-jumps the two tails into one; the TLA twin 0804bc08 shows them
 * half-merged), so reload allocates the mismatch path's state reload and the exit's -1 lands
 * in r1: 52 -> 46 (twelve dead stores keep 775 buckets).
 * The draft emits its two nested helpers first, 080270ac/080270d8, adopted on
 * their own as Battle_DrawPartyPanelsWithEmptyList and
 * UiText_DrawWaitingForOpponent with FAKEMATCH static-chain devices; compiled
 * here as real nested functions both match exactly, but the score counts
 * their 46 lines as extra. They adopt with this parent.
 * 2026-10-07 sun-08027114 (restructure, 1366 -> 424):
 * - The switch is on mode, not on the loader's result: the native jumps on r6
 *   after Resource_LoadIndexedIntoBuffer; every loaded tile is stored straight
 *   from the call, never through mode.
 * - Each case keeps its own locals (native registers per case: psynergy
 *   ability r6, cost r8, handle r10, ui r7, list r8, pick r10, cnt r4, k r1,
 *   cur r5; djinn djinn r9, ability r10, slot r8, handle on the stack, ok r4,
 *   j r7; summon ability r5, cost r6, element r7, index r4; item ability r8,
 *   cost r10, handle r9, list r10, pick r9, cnt r8, k r4, cur r5), and the
 *   psynergy and item cases their own slot. win (r11) and slot (r5) stay
 *   function-wide.
 * - Psynergy and item set mode = 0 before the three countdown zero stores:
 *   the native stores r6.
 * - The scan loops are if + do-while, each first load indexed by k (k = 0):
 *   actor->inventory[0] makes actor+216 a GCSE expression hoisted above the
 *   menu label and spilled; [k] keeps the native movs #216 / ldrh reg+reg.
 * - The djinn icons sit at n*2+13 with their own counter n, which only
 *   advances for a required element.
 * - Classify results go straight into BattleTarget_RunSelection.
 * - Message bases are their linker symbols ((s32)MsgCommandName,
 *   MsgAbilityName, MsgDjinnName, MsgItemName), loaded from the pool as
 *   native (31 included).
 * - The closing handshake counts misses in mode (native r6); with miss there,
 *   CSE made param, which then outlived mode, the djinn case's first
 *   SummonDefinition_Get argument.
 * - Sprite positions are win->x * 8 + 8 and win->y * 8 + 4: the shifted
 *   (u8) form narrows the y load to ldrb, native ldrh.
 * - The djinn icon counter n is set before the first requirement loop, so
 *   loop.c does not fold its start into the giv (native lsls/adds #13).
 * - The summon case's active path ends with its own aux = mode; break;
 *   (an if/else join inverts the native retry jump).
 * - Psynergy: list = state->list; pick = state->pick; then x = 48 (list
 *   r8 before pick r10, as native).
 * - Item scans store list[cnt] = cur; pick[cnt++] = k;: with cnt++ on its own
 *   line the loop has 18 insns and loop.c reduces the pick giv only on its
 *   second pass, after the list giv (native inits pick r7 first, list r6).
 * - Mode 4 sets count = 1 before ent = out: reload then takes r1 for the 1
 *   and r2 for out, and ent stays inherited in r2 (one load fewer).
 * - The two init loops are plain for loops (loop.c reverses them as native);
 *   state->timer = -1 among the zero stores (sched2 puts the store last).
 * - Japanese branches (EDITION_INTERNATIONAL): the first actor sprite starts
 *   at x 4 (else 0), the djinn window is 14 wide (else 17) and its icons
 *   start at column 10 (else 13). Scaffold differ per edition: ja 110,
 *   en 104 (150 with the helpers), de 106, es 106, fr 108, it 106.
 * Remaining (2026-10-07, all instruction order, register or pool placement;
 * the instruction stream is otherwise the native's):
 * - Frame-slot order: native &hdr 20 below the i*16/i*4/i*2 givs 24/28/32,
 *   draft &hdr 32 above them. GCSE's PRE makes its reaching registers in
 *   hash-bucket order; buckets are (rtx hash mod table size), table size =
 *   (real insns at GCSE)/2 | 1. Here 765: &state (sfp-8) 148, &hdr (sfp-4)
 *   152, i<<1/i<<2/i<<4 196/197/199 (i is pseudo 52), so &hdr comes first.
 *   Measured: twenty dead stores at the top (table 775) give the native
 *   frame exactly and differ 102. So the native function had about 20 more
 *   (or about 60 fewer) real insns at GCSE than this draft, removed before
 *   allocation. Not found: Audio_PlayCue as an inline wrapper, declaration
 *   initializers and moving DrawLocalizedResource (top, loop bodies, case
 *   blocks) all leave the table at 765.
 * - Literal-pool barriers in the psynergy, djinn and item cases sit one or
 *   two insns earlier in native. arm_reorg's arithmetic fits all three
 *   exactly if the cursor x constant stored after UiWindow_Create (40, 56,
 *   48) is a zero_extendhisi2 pool load (pool_range 60) in native, a movhi
 *   one (pool_range 64) here; the other cursor constants match as movhi.
 *   No source form tried (casts, u16/s32/u8 temporaries, inline setters)
 *   makes that zero_extend.
 * - sched2 order: the opening handshake's miss = 0 (native before the 'T'
 *   store), psynergy/item mode = 0 (native before the countdown load), the
 *   djinn case's ok = 1 - (j != 4) (native before the slot attr23 store),
 *   the mode-7 block's arguments (native loads *block before the r0 copy),
 *   in[i]'s two loads.
 * - Reload: the opening handshake's third count = -1 takes r3 (native r1).
 * Worse or neutral this session: while (cur) scan loops (actor+88 hoisted);
 * actor->inventory[0] (actor+216 hoisted; [k]: 1124 -> 809); the do-while
 * countdown init loops; timer = -1 last; slot or win function-wide in
 * psynergy/item (slot block-local: 766 -> 424); pick before list in
 * psynergy; item pick store first; mode as the classify result; param for
 * the summon element; y as
 * (u8)((win->y << 3) + 4) or (win->y << 3) + 4 (ldrb); x as
 * (win->x * 8 + 8) & 0x1ff (extra and); n = 0 after ok or at the menu label;
 * DrawLocalizedResource at the function top (442), in a loop body (316) or
 * the summon/item case (162); permute cannot parse nested functions (with
 * PERM_IGNORE around them it found no rewrite in 9 minutes).
 *
 * Earlier sessions (figures against the pre-restructure draft): differ 1366
 * against the 1727-line parent listing (draft 1823, common 1092; was 2131).
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
 * 1948; state->timer = -1 before the zero fields 1809; one variable for
 * mode and the selection results 1887; block-local counters in the
 * psynergy, item and djinn cases 1942. Native state->timer = -1 loads its
 * own -1 where the draft's reload move2add derives it from the zero in r3.
 * The native jump table sends 0, 1, 2, 3, 15 and 16 to bodies laid out as
 * 0, 1, 16, 15, 2, 3, then commit: the case bodies in that source order
 * give 1616 (from 1802). In that order the psynergy and item cases' own
 * block-local handles give 1601. Still worse on 1616/1601: every
 * entries[0].x pair written directly 1678; the mode-7 party counts kept in
 * sel 1610. In the djinn case the summon id stays in res (native r6) for
 * the second SummonDefinition_Get, the icon tile stored straight from the
 * loader: 1574 (it was 1943 in the old case order). mode and the
 * selection results as one variable, as native keeps both in r6: 1563
 * (1887 in the old order). Every entries[0].x store written directly
 * instead of through slot: 1508 (1678 on 1616). The frame figures above
 * date from 1802; recheck them. The djinn case's own counter j: 1504
 * (1966 in the old order). Next: djinn's register, the round end at
 * next_round, and the djinn and summon loops.
 * 2026-10-07 T15-08027114 s1: DrawLocalizedResource defined in the closing
 * handshake's block, where it is called, instead of at the top: state
 * stays a pseudo until the nested function captures it, so put_var_into_stack
 * fixes up the allocator's store after the call and the &state address is
 * formed after Runtime_AllocateBlock as native: 1483. A window of its own in
 * the djinn case (native fp): 1477; in the psynergy case too: 1476. The
 * djinn case's own sprite slot (native r8, its counter then r7 as native):
 * 1417. Block-local handle and block in the mode-7 branch: 1405; in the
 * mode -2 branch: 1403; the djinn case's handle block-local: 1403. sel
 * block-local in the mode-7 branch: 1401; the summon case as a block with
 * its own element, index and hint: 1401; its own cost: 1393; the psynergy
 * case's own scan: 1373; the summon case's own ability: 1366.
 * Worse, reverted: block-local djinn 1666; djinn as a fixed r9 (frame then
 * 100 as native, but registers reshuffle) 1579 on 1504; uncast allocator
 * result 1504 (unchanged); own window in the item case 1514 and in the
 * summon case 1425; own ability in the djinn case 1638, in the item case
 * 1413, in the psynergy case 1471 (on 1366); own slot in the psynergy case
 * 1517, in the item case 1445; own cost in the item case 1409 (on 1393);
 * the last cost loop as while (required <= cost) with the counter test
 * inside 1687; permuting the declaration order (500 candidates) found
 * nothing below 1504. Neutral: own sel in the item case, own cost in the
 * psynergy case, own ok in the djinn case, a block-local scan at commit.
 * Remaining causes: djinn still spills (frame 104 against 100: out, in and
 * count 4 bytes high): ok (prio 5*35/89) outranks djinn (5*38/158), so ok
 * takes r9 where native has djinn in r9 and ok caller-saved in r4. mode
 * (pseudo 52, 437 refs) is native r6 throughout but conflicts here with
 * local-alloc hard regs r5, r6 and r8 (the mode -2 branch's block and
 * handle, the item case's handle, a 0x1ff mask kept across a call) and
 * lands in caller-saved r4. The last djinn cost loop enters at its bottom
 * test; native copies the required > cost test to the entry. */
#include "RESOURCE.H"
#include "MEM.H"
#include "TYPES.H"
#include "EDITION.H"
#include "IO_REG.H"
#include "BTLWORK.H"
#include "BATTLE_UNIT.H"
#include "ITEM.H"
#include "BTLSMN.H"
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
s32 Battle_ClassifyEntryKind(const struct BtlAct *entry);
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

struct BtlUnit *Runtime_GetObject(s32 actor);
struct BtlAct *Ability_GetData(s32 id);
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
void MemFree(void *block);

s32 Djinn_GetDefinitionHeaderFar(s32 element, s32 index);
s32 Djinn_IsActiveFar(s32 actor, s32 element, s32 index);
s32 BattleParty_ListActorIdsFar(s32 mode, void *block);
void Camera_ConfigureSceneFar(s32 offset);
void BattlePres_SetActorModesFar(u16 *header, s32 mode);
void BattlePlacement_CountValidEntriesFar(s32 mode, u8 *cost);

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
        Audio_PlayCue(154);
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
            actor = (struct BtlUnit *)(s32)((s64 (*)(s32))Runtime_GetObject)(actorId);
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
                Audio_PlayCue(112);
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

                Audio_PlayCue(112);
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
                        if ((Ability_GetData(cur)->tgt_flg & 0x80) != 0) {
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
                ability = Ability_GetData(param);
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
                UiWindow_SetTilemapEntry(win, 61470, 12, 0, 0);
                if (ability->damage_class != 4) {
                    UiWindow_SetTilemapEntry(win, ability->damage_class + 20481, 15, 0, 0);
                }
                ent->rng = cost;
                Audio_PlayCue(112);
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

                Audio_PlayCue(112);
                COUNTDOWN->unk_34 = 0;
                COUNTDOWN->unk_30 = 0;
                COUNTDOWN->unk_38 = 0;
djinn_menu:
                state->entries[0].oam.f.x = 80;
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
                ability = Ability_GetData(djinn->name_message_id);
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
                    Audio_PlayCue(112);
                } else {
                    Audio_PlayCue(114);
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

                Audio_PlayCue(112);
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
                    ability = Ability_GetData(Djinn_GetDefinitionHeaderFar(element, index));
                    cost = ability->rng;
                    win = UiWindow_Create(11, 17, 10, 3, 6);
                    state->entries[0].oam.f.x = 64;
                    UiWindow_SetTilemapEntry(win, element + 20481, 0, 0, 0);
                    UiText_DrawCharacterAtOffset(element * 20 + index + (s32)MsgDjinnName, win, 16, 0);
                    ent->rng = cost;
                    WaitFrames(1);
                    Audio_PlayCue(112);
                    mode = BattleTarget_RunSelection(actorId, ability->target_mode, cost,
                                                     Battle_ClassifyEntryKind((const struct BtlAct *)ability));
                    if (state->hintCount != 0) {
                        hint = UiText_ShowMessageAndWaitComplete(3150, 15, 8);
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
                Audio_PlayCue(112);
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

                Audio_PlayCue(112);
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
                        Item_GetData(cur);
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
                        Item_GetData(cur);
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
                ability = Ability_GetData(Item_GetData(actor->inventory[sel])->action_id);
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
                Audio_PlayCue(112);
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

            Audio_PlayCue(110);
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
            UiText_CopyMessageString((s32)MsgWaitingForOpponent, data, 52);
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

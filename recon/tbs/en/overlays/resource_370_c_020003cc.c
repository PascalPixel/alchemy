/* SaveMenu_Run (overlay resource_370 020083cc, listed as Func_020003cc): differ 0 of 920 (English) with --symbol Func_020003cc; all 2,352 bytes, all four literal pools, every call and data relocation agree with the listing's (messages and scene ids resolve to the listing's numbers: 7, 0xa, 4, 6, 2, 8, 0xbe; gGameState is gCell's address).
 * Remaining before adoption into SRC/MENU/CLEAR/CLEAR.C (its linker neighbour; ENTRY.S's veneer and the linker line name Func_020003cc):
 * - Headers: the draft declares its callees unprototyped; CLEAR.C's headers declare them with real prototypes and types (UiWindow_Create returns struct UiWindow *, UiWork_Finalize and UiWindow_SetTilemapEntry take one, Scheduler_AddOrUpdateCallback a SchedulerCallback, SerialRuntime_BeginTransferA a void *, Battle_WaitMode0 returns void); give the windows their type, drop the draft's declarations and rescore. Measured: the body under CLEAR.C's include block, with the draft's UiWork_Finalize, UiWindow_Create and UiWindow_SetTilemapEntry declarations dropped (the only conflicts), still scores differ 0; with all its callee declarations dropped, the callees passed to the Value and Call wrappers that CLEAR.C's headers lack (SaveState_ScanRecordFlags, Menu_SelectSaveSlotAction, UiText_DrawResource, Menu_SelectResourceLayout, Clear_PackPasswordData, Clear_EncodePassword, Graphics_ExpandVramTilesByColorTable ...) are undeclared.
 * - Editions: the listing differs per edition. Japanese (not EDITION_INTERNATIONAL) has a 488-byte frame (sub sp) and other pools; the link greeting compares the edition's letter (EN 85, DE 69, FR 70, ES 83, IT 73, JA 67); German and French move pools. Spanish and Italian end the link section with UiWork_Finalize(v6, 2) and WaitFrames(1) and no RenderOutput_PrepareForRedraw(v6), which cross-jumps into the case-0 tail (b Clear_FinalizeAndWait), and their case-4 test branches the other way (beq Clear_LoadFourthSlot); Japanese ends it as English does. Each needs its #if branch or stays on its scaffold (S4).
 * Trials (2026-10-07, session 4):
 * - 26 -> 0: the A-key wait after a link error tests gKeyState & v5 (KEY_A held in v5 = 1): the native's ldr r7 loads gKeyState from the shared pool entry; the raw 0x3001c94 address added a pool word, which moved every later branch and pool load by 4.
 * - 0 -> 0: every Func_ placeholder callee named as the listing's relocations pair them (137 calls, one name each), raw addresses as their symbols (Clear_CheckButtonCodes, Clear_UpdateBlend, Clear_CodeUnlocked, Clear_ExtraCodeUnlocked, gSleepDisabled, gDebugMode, gOptionMirror, gWindowWork, gEventWork), the save record union as struct GameState fields (build_stamp, play_time, palette_glow, unknown_20f, saved_options), scene row 8 named SceneId_HaidiaBabi in SCENES.S after its overlay folder, and the dead 0x2000240 stores and unused locals dropped.
 * Trials (2026-10-07, session 3):
 * - 78 -> 205: the draw call in each glyph arm: cross-jumping (after sched2) merges the tails as native, and caller-save no longer restores n at each arm's end; but CSE carries the f0c call's glyph address into both draws, loop.c hoists it (life 47) and global gives it r8, which pushes page off its register.
 * - 205 -> 141: the glyph address for f0c through a pointer behind asm volatile ("+r"): CSE no longer reuses it, but gcse cprop folds the draws' -524 and PRE hoists their sp + 24 to the loop entry (fp); the same in a goto loop (141).
 * - 205 -> 69: an empty asm volatile ("" : :) after the f0c call (FAKEMATCH): an ASM_OPERANDS barrier flushes CSE's table (asm ("") is an ASM_INPUT and does not), so each draw rebuilds sp + 24 as native.
 * - 69 -> 57: x passed as rec7 * 12 + 18 (or + 8) directly in the call: the product is built in r2 as native; as a separate x it goes through r3 (65-78).
 * - 57 -> 53: the row in value (a user variable that is r1 in the page dots): its copy from r0 stays, as native, but in r1 where the native has r3; record, v3, rec, rec5, x or base7_3001c94 coalesce with r0 (57), v4 (73), base3_2000240 (100), link_message (267); v3 = row; v3 <<= 4 (+= 2) (60-62); x as its own variable with the shift before or after (65-241).
 * - 53 -> 45: the row in a fixed-register variable row asm("r3") (FAKEMATCH): the copy from r0 to r3 and the shift in place, as native; the glyph loop now matches instruction for instruction.
 * - 45 -> 45: the f0c call in do { } while (0) in place of the asm barrier (FAKEMATCH): its LOOP_END note ends the CSE path the same way and emits no insn, so the pool difference is not the asm's length estimate; kept, as it needs no inline asm. The glyph call with no barrier (183).
 * - 45 -> 26: the 0x3e7 store through v3 (an s32 user variable): as a plain constant it is a HImode pool load (*thumb_zero_extendhisi2, pool_range 60), which forced the last pool before the epilogue; as SImode (*thumb_movsi_insn, 1020) it joins the native's one pool after the function. value puts it in r1 (30), v1, v4 or v6 (30), rec5 (36), link_message (73); v3, x, rec or count give r2 as native (26).
 * Trials (2026-10-07, session 2):
 * - 231 -> 221: the greetings stored as plain u16 through gSerialTransfer.payload and a 12-halfword peer row: the native's 0x30 is a pool load (thumb_movhi_insn takes a HImode constant through its "mn" alternative), and the struct stores expanded to an and/ior word update; the case-4/5 pool now sits after the greeting loop as native.
 * - 221 -> 221: gKeyState (volatile s32) and gLinkStatus (volatile u16) by name instead of raw addresses, as the native's relocations show; no change in code, kept.
 * - 221 -> 213: the page dots' x as v7 + 18 - page_count instead of a second counter: loop.c's giv makes the native's negs/adds start value.
 * - 213 -> 211: v7 as the page-dot counter: CSE keeps v7 (last use later than page's) in value = v7 + 0xf30b, as native.
 * - 211 -> 209: the glyph loop as for (v7 = page * 50, n = 0; v7 != v6; v7++, n++), comparing v7 with v6 at entry as native.
 * - 209 -> 181: the password section as if (v6 == 0) { ... } instead of an empty then-arm with an else: jump.c no longer pulls the help exit up before the page loop, so it sits after it and the B exit before the test, as native.
 * - 181 -> 177: the save data pointer kept in a local (r5 from before the first call, as native) for the three calls, the two checksum bytes still stored through the array (through the pointer they fold to [r3, #1]: 186).
 * - 177 -> 174: the transfer loop tests the mask variable v1 (r1, caller-saved at [sp, #8], as native).
 * - 174 -> 171: RenderOutput_PrepareForRedraw (the redraw prepare) called with the window alone: the native leaves the mask in r1, the draft rematerialized 3 for a second argument it does not take.
 * - 171 -> 171: v7 as the transfer counter (r7, as native); no change alone, but it frees r5: count no longer conflicts with v5.
 * - 171 -> 122: v5 as the greeting-loop mask: with count gone v5 keeps r5, a callee-saved register, so no caller save splits the loop.
 * - 122 -> 120: v7 = 0 before v5 = 3, as the native orders them.
 * - 120 -> 116: the peer greetings as for (v1 = 0; v1 != 4; v1++) over gLinkPeerSignatures[v1]: the row pointer is a giv set after the hoisted 0x30, as native.
 * - 116 -> 108: the page end as v6 = page * 50 + 50: page * 50 is its own temporary (r0), copied to v6 and later to v7, as native.
 * - 108 -> 92: the LEFT and RIGHT arms compute the new page number and one __modsi3 call follows them, where the native has its one shared mov r1, r8 / bl (195 when the score was 181: the earlier pool and help placement hid it).
 * - 92 -> 80: the page number as rec5 = page + page_count, then rec5-- or rec5++ in place: r0 throughout, as native.
 * - 80 -> 78: the redraw prepare before v7 = 0 and v5 = 2 for the dividers, as the native calls it first.
 * - no gain: the row call into rec (108) or into v3 shifted after the arms (243); the column test on the call directly (108); at 78, v3 = record << 4 before x in each arm (234) and the column in rec apart from the row's record (78).
 * - no gain: v5 as the greeting-loop mask while count shared the transfer loop (399; with v7 as the transfer counter 395); v7 as the transfer counter alone (495); the row call into v3 shifted in place, in each arm or after (356); one __modsi3 call after the LEFT and RIGHT arms, v7 = 1 before or after it (195).
 * Trials (2026-10-07):
 * - 463 -> 350: the glyph counter n apart from the link result v4, and password[v7] & 63 passed directly: v3 no longer conflicts with r3, the link result takes a caller-saved register, and page gets r11 with the encoded size in r9 and the page window in r10, as native.
 * - 350 -> 332: the missed-frame count kept in v5 (r5, as native).
 * - 332 -> 315: the link wait loop as a real for loop testing the constant 3: loop.c moves its ten-missed-frames exit to the barrier after case 4, where the native has it, and leaves the status and source addresses in the loop.
 * - 315 -> 304: the new-game scene loaded as a scene table id (row 8) instead of an immediate.
 * - 304 -> 251: the new-game entrance stored as a plain 20.
 * - 251 -> 231: help_window declared before heading_window: spill slots [sp, #16] and [sp, #12] as native; the dead v3 = 50 and the two-step row shift dropped (231).
 * - no gain: x after v3 in the glyph arms (460); the draw call in each glyph arm (628, no cross-jump); v3 & 63 (463); the status read before the mask (no change); the status through v3 (no change); the first wait loop testing v5 (447); the for loop testing the mask variable (409: loop.c hoists the status address at 15 real insns, threshold 15), plus an empty asm in it (329: the rerun still hoists it at 15); an explicit goto to the help exit (231, unchanged).
 * Trials (2026-10-06):
 * - 873 -> 757: the record prompt as a goto loop; the native reloads every invariant in it, as goto loops do.
 * - 757 -> 711: SceneId_WorldMap from the scene table; one callee name per function, so the case-1 tails cross-jump through one message call; a state pointer kept in r5.
 * - 711 -> 703: the menu head and the owner-name loop as goto loops; return 0 from the adventure branch.
 * - 703 -> 607: the password page loop as a real loop with its A and B exits inline: loop.c moves each exit back to the barrier before its target, where the native has them.
 * - 607 -> 598: the link exits (matched greeting, ten missed frames) inline in their loops; the record-select prompt as a goto loop.
 * - 598 -> 573: SceneId_LinkLobby (+4 alone, from the pool shift); the scene state's word at 0x100 as a struct member, which keeps the base and offset apart as the native does and moves the literal pool to after case 3.
 * - 573 -> 522: the 0x109 flag set through the Call1 wrapper (FAKEMATCH): direct, CSE keeps 0x109 in r7 for the later clears, which the native derives from 0x1c2.
 * - 522 -> 507: the link wait loop written test-first with gotos; the native reloads its status and source addresses each pass.
 * - 507 -> 503: the checksum as an s16 stored with two post-increments.
 * - 503 -> 475: the checksum's high byte through (u16), stored at n and n + 1 before n += 2; the two help messages as the password label's id minus 2 and minus 1, which the native derives from the label's register.
 * - 475 -> 463: page = 0 right after the page window is made, where the native zeroes r11.
 * - no gain: the 0x100 word through a (u32) cast (602); the state pointer set before the flag call (575); the adventure block in an if (0) arm (711); the first link loop testing its mask variable instead of 3 (604); dropping the glyph loop's slot4 copies (507, kept as simpler); one UiWindow_Create and UiText_DrawCharacterAtOffset for both help arms (463). */
#include "TYPES.H"
#include "IO_REG.H"
#include "SERIAL_RUNTIME.H"
#include "FIELD_EVENT.H"
#include "SCENE_IDS.H"
extern u32 gKeysHeld;
struct SaveMenuSceneState {
    u8 unknown_000[0x100];
    s32 word_100;
};
void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void UiWork_FinalizePendingCore(void);
s32 Scene_GetModeMask(void);
s32 Menu_RunConfirmSelection(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern u8 MsgNoBackupMemory[];
extern u8 MsgSaveAdventureQuestion[];
extern u8 MsgSaveDataCorrupted[];
extern u8 MsgClearCommunicationError[];
extern u8 MsgClearSendingData[];
extern u8 MsgClearPasswordLabel[];
extern u8 MsgClearSaveLabel[];

extern volatile s32 gKeyState;
extern volatile u16 gLinkStatus;
void UiWork_Finalize(s32 window, s32 mode);
void RenderOutput_PrepareForRedraw(s32 window);

extern u8 gSaveBuffer[];
extern volatile u8 gSleepDisabled;
extern u8 gDebugMode;
extern volatile u8 gOptionMirror;
extern s16 Clear_CodeUnlocked;
extern s16 Clear_ExtraCodeUnlocked;
extern u8 *gWindowWork[];
void Clear_UpdateBlend(void);
void Clear_CheckButtonCodes(void);
struct SaveLinkGreeting {
    u16 code[4];
    u8 reserved[16];
};

extern struct SaveLinkGreeting gLinkPeerSignatures[4];
s32 Battle_WaitMode0(s32);
s32 Clear_EncodePassword();
void Clear_LoadBackground();
void Clear_NameEntryCharacter();
s32 Clear_PackPasswordData();
void Engine_AudioPlayCue(s32);
s32 Engine_GameFlagIsSet();
void Event_FinishScreenTransition();
void Event_SetDestination();
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames();
void GameState_InitDefaults();
void Graphics_ExpandVramTilesByColorTable();
void InventorySnapshot_Restore(void);
void Link_DrawShiftedTilePair();
s32 Menu_SelectResourceLayout(s32);
s32 Menu_SelectSaveSlotAction();
s32 NameEntry_EditOwnerName();
s32 Object_CallSpawnRoutineAtOrigin(s32);
void PaletteGlow_Update();
void Party_AddActiveOwner();
void Party_RemoveActiveOwner(s32);
void Party_SetFlag32AndRefreshMembers();
s32 Runtime_GetBuildStampTime();
void SaveState_CopySlotPair();
void SaveState_DeleteSelectedSlot();
s32 SaveState_LoadRecordIntoWork();
s32 SaveState_ScanRecordFlags();
s32 Save_WriteSelectedSlot();
s32 SceneData_GetBufferCrc16();
s32 Scheduler_AddOrUpdateCallback();
s32 SerialRuntime_BeginTransferA();
void SerialRuntime_Initialize();
void SerialRuntime_RemoveIrqHandlers();
void UiText_DrawCharacterAtOffset();
void UiText_DrawResource();
void UiText_DrawString();
void UiText_ShowCenteredMessage();
s32 UiWindow_Create();
void UiWindow_DrawDividerLine();
void UiWindow_SetTilemapEntry(s32, s32, s32, s32, s32);
void Ui_LoadWindowGraphics();
void WaitFrames();
s32 __divsi3(s32, s32);
s32 __modsi3();

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

s32 SaveMenu_Run(void)
{
    s32 page_count;
    s32 rec;
    s32 encoded_size;
    s32 page_window;
    s32 rec5;
    s32 rec7;
    s32 record;
    s32 value;
    s32 v5;
    s32 v7;
    s32 count;
    s32 v6;
    s32 link_message;
    s32 v1;
    s32 v3;
    s32 v4;
    s32 n;
    s32 page_message;
    s32 page;
    s32 x;
    s32 started;
    s32 help_window;
    s32 heading_window;
    u8 save_data[200];
    u8 password[320];
    u8 glyph[4];

    started = 0;
    Clear_LoadBackground();
    Value2(Scheduler_AddOrUpdateCallback, (s32)Clear_CheckButtonCodes, 0xc80);
    *(s32 *)((*(u8 **)&gEventWork + 0x1c0)) = started;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    if (gGameState.entrance == 2) {
    L_ask:
        UiText_ShowPositionedMessageAndWait((s32)MsgSaveAdventureQuestion, 5);
        v7 = Value1(Object_CallSpawnRoutineAtOrigin, 1);
        UiWork_FinalizePendingCore();
        if (v7 == 0) {
            UiText_ShowPositionedMessageAndWait(((s32)MsgSaveAdventureQuestion + 1), 1);
            gGameState.unknown_20f = 1;
            v7 = Save_WriteSelectedSlot();
            if (v7 == -1) {
                goto L_ask;
            }
        }
        Battle_WaitMode0(60);
        Event_FinishScreenTransition();
        Engine_AudioPlayCue(17);
        Call1(Battle_WaitMode0, 0x12c);
        Event_SetDestination((s32)&SceneId_WorldMap, 72);
        return 0;
    } else {
        gSleepDisabled = started;
    }
    L_020004a8:;
    v6 = Value0(SaveState_ScanRecordFlags);
    if (v6 < 0) {
        if (gDebugMode != 0) {
            gGameState.saved_options = 1;
            gOptionMirror = 1;
            UiText_ShowCenteredMessage((s32)MsgNoBackupMemory, 1, 8);
        }
    }
    if (v6 == 0) {
        if (started == 0) {
            WaitFrames(30);
            Value2(Scheduler_AddOrUpdateCallback, (s32)Clear_UpdateBlend, 0xc80);
            WaitFrames(1);
            started = 1;
        }
    }
    if (v6 > 0) {
        v6 = Value0(Menu_SelectSaveSlotAction);
    } else {
        v6 = 0;
    }
    if (v6 != 0) {
        goto L_0200057a;
    }
    GameState_InitDefaults();
    PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
    v5 = 1;
    v7 = 0;
    L_02000522:;
    WaitFrames(6);
    v6 = NameEntry_EditOwnerName(v7);
    if (v6 == -1) {
        if (v7 == 0) {
            goto L_020004a8;
        }
        v7--;
        goto L_02000522;
    }
    if (Clear_CodeUnlocked != 0) {
        v5 = 4;
    }
    if (Clear_ExtraCodeUnlocked != 0) {
        v5 = 7;
    }
    v7 = (v7 + 1);
    if (v7 < v5) {
        goto L_02000522;
    }
    Party_SetFlag32AndRefreshMembers();
    gGameState.scene = (s32)&SceneId_HaidiaBabi;
    gGameState.entrance = 20;
    goto L_02000c96;
    L_0200057a:;
    if (v6 != 1) {
    } else {
        struct GameState *state;

        v6 = SaveState_LoadRecordIntoWork(1);
        if (v6 == -1) {
            goto L_020004a8;
        }
        /* FAKEMATCH: called directly, CSE keeps 0x109 in r7 for the later clears; the native derives them from 0x1c2 (522 vs 573). */
        Call1((void (*)())GameFlag_SetBit, 0x109);
        state = &gGameState;
        PaletteGlow_Update(state->palette_glow[0], state->palette_glow[1]);
        if (state->build_stamp != Runtime_GetBuildStampTime()) {
            state->scene = state->saved_scene;
            state->entrance = state->saved_entrance;
            GameFlag_ClearBit(0x109);
        } else if ((gKeysHeld & 0x208) == 0x208) {
            if (Scene_GetModeMask() != 0) {
                UiText_ShowPositionedMessageAndWait((s32)MsgSaveDataCorrupted + 2, 9);
                goto L_020004a8;
            }
            state->scene = state->saved_scene;
            state->entrance = state->saved_entrance;
            GameFlag_ClearBit(0x109);
            GameFlag_SetBit(0x13e);
        } else if (state->play_time != ((struct SaveMenuSceneState *)gSceneState)->word_100) {
            UiText_ShowPositionedMessageAndWait((s32)MsgSaveDataCorrupted, 9);
            UiText_ShowPositionedMessageAndWait((s32)MsgSaveDataCorrupted + 1, 13);
            if (Menu_RunConfirmSelection(1, 0, 0, 0) != 0) {
                UiWork_FinalizePendingCore();
                goto L_020004a8;
            }
            UiWork_FinalizePendingCore();
            if (Scene_GetModeMask() != 0) {
                UiText_ShowPositionedMessageAndWait((s32)MsgSaveDataCorrupted + 2, 9);
                goto L_020004a8;
            }
            state->scene = state->saved_scene;
            state->entrance = state->saved_entrance;
            GameFlag_ClearBit(0x109);
            GameFlag_SetBit(0x13f);
        }
        GameFlag_ClearBit(0x106);
        goto L_02000c96;
    }
    if (v6 == 2) {
        SaveState_CopySlotPair();
        goto L_020004a8;
    }
    if (v6 == 3) {
        SaveState_DeleteSelectedSlot();
        goto L_020004a8;
    }
    if (v6 == 4) {
        v6 = SaveState_LoadRecordIntoWork(4);
        if (v6 == -1) {
            goto L_020004a8;
        }
        gGameState.selected_actor = 0;
        if (Value1(Engine_GameFlagIsSet, 0x952) != 0) {
            InventorySnapshot_Restore();
            Party_RemoveActiveOwner(0);
            Party_RemoveActiveOwner(1);
            Party_RemoveActiveOwner(2);
            Party_RemoveActiveOwner(3);
            Party_AddActiveOwner(0);
            Party_AddActiveOwner(1);
            Party_AddActiveOwner(2);
            Party_AddActiveOwner(3);
        }
        PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
        Call1(GameFlag_ClearBit, 0x109);
        GameFlag_ClearBit(0x106);
        GameFlag_SetBit(0x17e);
        gSleepDisabled = 1;
        Event_SetDestination((s32)&SceneId_LinkLobby, 1);
        goto L_02000c96;
    } else {
        if (v6 != 5) {
            goto L_020004a8;
        }
        L_020007b8:;
        v6 = SaveState_LoadRecordIntoWork(5);
        if (v6 == -1) {
            goto L_020004a8;
        }
        PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
        L_020007dc:;
        v6 = Menu_SelectResourceLayout(0);
        if (v6 == -1) {
            goto L_020007b8;
        }
        if (v6 != 1) {
            goto L_020009cc;
        }
        v6 = UiWindow_Create(6, 5, 18, 8, 2);
        link_message = (s32)MsgClearSaveLabel;

        UiText_DrawResource(link_message, v6, 0, 4);
        UiText_DrawResource((link_message + 1), v6, 0, 16);
        UiText_DrawResource((link_message + 3), v6, 0, 36);
        SerialRuntime_Initialize();
        WaitFrames(10);
        {
            u16 *sync = (u16 *)gSerialTransfer.payload;

            sync[0] = 0x30;
            sync[1] = 0x30;
            sync[2] = 0x30;
            sync[3] = 0x30;
        }
        v7 = 0;
        v5 = 3;
        for (v1 = 0; v1 != 4; v1++) {
            u16 *greeting = gLinkPeerSignatures[v1].code;

            greeting[0] = 0x30;
            greeting[1] = 0x30;
            greeting[2] = 0x30;
            greeting[3] = 0x30;
        }
        while ((gKeyState & KEY_B) == 0) {
            if ((v5 & gLinkStatus) == v5) {
                u32 control = REG_SIOCNT;
                u32 peer = ((control << 26) >> 30) ^ 1;
                u16 *received = gLinkPeerSignatures[peer].code;

                if (received[0] == 85) {
                    if (received[1] == 86) {
                        if (received[2] == 84) {
                            if (received[3] == 83) {
                                v7 = 1;
                                goto L_020008e0;
                            }
                        }
                    }
                }
            }
            WaitFrames(1);
        }
        Engine_AudioPlayCue(113);
        L_020008e0:;
        UiWork_Finalize(v6, 2);
        if (v7 == 0) {
            goto L_020007dc;
        }
        v6 = UiWindow_Create(5, 10, 20, 4, 2);

        Call4(UiText_DrawResource, (s32)MsgClearSendingData, v6, 0, 4);
        WaitFrames(10);
        Value2(SerialRuntime_BeginTransferA, (s32)gSaveBuffer, 0x1004);
        WaitFrames(10);
        v5 = 0;
        v1 = 3;
        v4 = 1;
        for (v7 = 0; v7 <= 0x927bf; v7++) {
            v5 = (v5 + 1);
            if ((v1 & gLinkStatus) == v1) {
                v5 = 0;
            }
            if (v5 == 10) {
                v4 = 0;
                break;
            }
            if (gSerialSendSource == 0) {
                break;
            }
            WaitFrames(1);
        }
    }
    L_0200095e:;
    if (v4 == 0) {
        RenderOutput_PrepareForRedraw(v6);
        Call4(UiText_DrawResource, (s32)MsgClearCommunicationError, v6, 0, 4);
        v5 = 1;
        do {
            WaitFrames(1);
        } while ((gKeyState & v5) == 0);
    }
    WaitFrames(10);
        SerialRuntime_RemoveIrqHandlers();
    WaitFrames(10);
    RenderOutput_PrepareForRedraw(v6);
    UiWork_Finalize(v6, 2);
    goto L_020004a8;
    L_020009cc:;
    if (v6 == 0) {
        L_020009d2:;
        v6 = Value1(Menu_SelectResourceLayout, 1);

        if (v6 == -1) {
            goto L_020007dc;
        }
        {
            u8 *data = save_data;
            s16 sum;

            encoded_size = Value3(Clear_PackPasswordData, 0, v6, data);
            sum = SceneData_GetBufferCrc16(encoded_size, data);
            save_data[encoded_size] = (u16)sum >> 8;
            save_data[encoded_size + 1] = sum;
            encoded_size += 2;
            encoded_size = Value3(Clear_EncodePassword, data, encoded_size, password);
        }
        v6 = 2;
        Ui_LoadWindowGraphics();
        page_window = UiWindow_Create(5, 4, 20, 12, v6);
        page = 0;
        page_count = (__divsi3(encoded_size, 50) + 1);
        record = UiWindow_Create(10, 0, 10, 4, v6);
        page_message = (s32)MsgClearPasswordLabel;
        heading_window = record;
        UiText_DrawResource(page_message, heading_window, 6, 4);
        v7 = 1;
        if (page_count == 1) {
            record = UiWindow_Create(5, 16, 20, 3, v6);
            help_window = record;
            Call4(UiText_DrawCharacterAtOffset, page_message - 2, help_window, 80, 0);
        } else {
            record = UiWindow_Create(1, 16, 28, 3, v6);
            help_window = record;
            Call4(UiText_DrawCharacterAtOffset, page_message - 1, help_window, 0, 0);
        }
        Call1(Graphics_ExpandVramTilesByColorTable, 0x6006000);
        RenderOutput_PrepareForRedraw(page_window);
        for (;;) {
        Call1(Link_DrawShiftedTilePair, 0x6002500);
        if ((gKeyState & KEY_B) != 0) {
            Engine_AudioPlayCue(113);
            RenderOutput_PrepareForRedraw(page_window);
            UiWork_Finalize(page_window, 2);
            UiWork_Finalize(help_window, 2);
            UiWork_Finalize(heading_window, 2);
            goto L_020009d2;
        }
        if ((gKeyState & KEY_A) != 0) {
            page = (page + 1);
            v7 = 1;
            if (page == page_count) {
                Engine_AudioPlayCue(112);
                RenderOutput_PrepareForRedraw(page_window);
                UiWork_Finalize(page_window, 2);
                UiWork_Finalize(help_window, 2);
                UiWork_Finalize(heading_window, 2);
                WaitFrames(1);
                goto L_020004a8;
            }
            Engine_AudioPlayCue(111);
        } else {
            if ((gKeyState & KEY_LEFT) != 0) {
                if (page_count <= 1) {
                    goto L_02000b30;
                }
                Engine_AudioPlayCue(111);
                rec5 = page + page_count;
                rec5--;
            } else {
                L_02000b30:;
                if ((gKeyState & KEY_RIGHT) == 0) {
                    goto L_02000b58;
                }
                if (page_count <= 1) {
                    goto L_02000b58;
                }
                Engine_AudioPlayCue(111);
                rec5 = page + page_count;
                rec5++;
            }
            rec5 = __modsi3(rec5, page_count);
            v7 = 1;
            page = rec5;
        }
        L_02000b58:;
        if (v7 != 1) {
            v5 = page_message;
        } else {
            RenderOutput_PrepareForRedraw(page_window);
            v7 = 0;
            v5 = 2;
            do {
                UiWindow_DrawDividerLine(page_window, 0, v5, 18, v5);
                v5 = (v5 + 2);
                v7 = (v7 + 1);
            } while (v7 != 4);
            if (page_count > 1) {
                    v7 = 0;
                if (page_count != 0) {
                    v6 = 0;
                    do {
                        value = v7 + 0xf301;
                        if (v7 == page) {
                            value = v7 + 0xf30b;
                        }
                        UiWindow_SetTilemapEntry(page_window, value, v7 + 18 - page_count, -1, v6);
                        v7 = (v7 + 1);
                    } while (v7 != page_count);
                }
                v5 = 0;
                UiWindow_SetTilemapEntry(page_window, 0xf128, (17 - (s32)page_count), -1, 0);
                UiWindow_SetTilemapEntry(page_window, 0xf129, 18, -1, 0);
                gWindowWork[0][0xea3] |= (2 << ((u32)*(u16 *)(page_window + 14) >> 2));
            }
            v6 = page * 50 + 50;
            if (v6 > encoded_size) {
                v6 = encoded_size;
            }
            for (v7 = page * 50, n = 0; v7 != v6; v7++, n++) {
                /* FAKEMATCH: the row's copy from r0 goes to r1 (value) or folds into r0; the native copies it to r3 and shifts it there */
                register s32 row asm("r3");

                /* FAKEMATCH: the loop note ends the CSE path, which otherwise carries the glyph address into both draw calls for gcse to hoist out of the loop; the native rebuilds sp + 24 at each call */
                do {
                    Clear_NameEntryCharacter(password[v7] & 63, glyph);
                } while (0);
                record = __modsi3(v7, 10);
                if (record > 4) {
                    rec7 = __modsi3(n, 10);
                    row = __divsi3(n, 10);
                    UiText_DrawString(glyph, page_window, rec7 * 12 + 18, (row << 4) + 2);
                } else {
                    rec7 = __modsi3(n, 10);
                    row = __divsi3(n, 10);
                    UiText_DrawString(glyph, page_window, rec7 * 12 + 8, (row << 4) + 2);
                }
            }
            v7 = 0;
        }
        WaitFrames(1);
        }
    }
    Call1(WaitFrames, 0x12c);
    goto L_020004a8;
    L_02000c96:;
    v3 = 0x3e7;
    *(u16 *)((*(u8 **)&gEventWork + 0x170)) = v3;
    Battle_WaitMode0(30);
    Engine_AudioPlayCue(17);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Battle_WaitMode0(60);
    L_Return:;
    return 0;
}

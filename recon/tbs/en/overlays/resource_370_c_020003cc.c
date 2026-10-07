/* SaveMenu_Run (overlay resource_370 020083cc, listed as Func_020003cc): differ 45 of 920 (draft 921, common 898) with --symbol Func_020003cc.
 * Remaining: every instruction now matches; only literal pool placement differs. The native keeps one pool after the epilogue (gKeyState, 0xf301, 0xf30b, 0xf128, 0xf129, gWindowWork, 0xea3, gEventWork, 0x3e7); the draft's arm_reorg finds the function-end barrier past the first fix's max_address, so it dumps the first seven after the far jump back to the page loop (after Func_02001fb4) and the last two before the epilogue with a b.n around them (the 921st instruction), which moves the far branches to the tail and the pool loads. The draft's address estimate up to the end must be larger than the native's: look for an RTL jump or insn whose default length differs (branches are measured before shorten_branches). Before adoption, row 8 of the scene table (code 0x377, area 3, map 7) needs its name in SRC/FIELD/SCENES.S: the draft calls it SceneId_Opening, which nothing defines yet.
 * Trials (2026-10-07, session 3):
 * - 78 -> 205: the draw call in each glyph arm: cross-jumping (after sched2) merges the tails as native, and caller-save no longer restores n at each arm's end; but CSE carries the f0c call's glyph address into both draws, loop.c hoists it (life 47) and global gives it r8, which pushes page off its register.
 * - 205 -> 141: the glyph address for f0c through a pointer behind asm volatile ("+r"): CSE no longer reuses it, but gcse cprop folds the draws' -524 and PRE hoists their sp + 24 to the loop entry (fp); the same in a goto loop (141).
 * - 205 -> 69: an empty asm volatile ("" : :) after the f0c call (FAKEMATCH): an ASM_OPERANDS barrier flushes CSE's table (asm ("") is an ASM_INPUT and does not), so each draw rebuilds sp + 24 as native.
 * - 69 -> 57: x passed as rec7 * 12 + 18 (or + 8) directly in the call: the product is built in r2 as native; as a separate x it goes through r3 (65-78).
 * - 57 -> 53: the row in value (a user variable that is r1 in the page dots): its copy from r0 stays, as native, but in r1 where the native has r3; record, v3, rec, rec5, x or base7_3001c94 coalesce with r0 (57), v4 (73), base3_2000240 (100), link_message (267); v3 = row; v3 <<= 4 (+= 2) (60-62); x as its own variable with the shift before or after (65-241).
 * - 53 -> 45: the row in a fixed-register variable row asm("r3") (FAKEMATCH): the copy from r0 to r3 and the shift in place, as native; the glyph loop now matches instruction for instruction.
 * - 45 -> 45: the f0c call in do { } while (0) in place of the asm barrier (FAKEMATCH): its LOOP_END note ends the CSE path the same way and emits no insn, so the pool difference is not the asm's length estimate; kept, as it needs no inline asm. The glyph call with no barrier (183).
 * Trials (2026-10-07, session 2):
 * - 231 -> 221: the greetings stored as plain u16 through gSerialTransfer.payload and a 12-halfword peer row: the native's 0x30 is a pool load (thumb_movhi_insn takes a HImode constant through its "mn" alternative), and the struct stores expanded to an and/ior word update; the case-4/5 pool now sits after the greeting loop as native.
 * - 221 -> 221: gKeyState (volatile s32) and gLinkStatus (volatile u16) by name instead of raw addresses, as the native's relocations show; no change in code, kept.
 * - 221 -> 213: the page dots' x as v7 + 18 - page_count instead of a second counter: loop.c's giv makes the native's negs/adds start value.
 * - 213 -> 211: v7 as the page-dot counter: CSE keeps v7 (last use later than page's) in value = v7 + 0xf30b, as native.
 * - 211 -> 209: the glyph loop as for (v7 = page * 50, n = 0; v7 != v6; v7++, n++), comparing v7 with v6 at entry as native.
 * - 209 -> 181: the password section as if (v6 == 0) { ... } instead of an empty then-arm with an else: jump.c no longer pulls the help exit up before the page loop, so it sits after it and the B exit before the test, as native.
 * - 181 -> 177: the save data pointer kept in a local (r5 from before the first call, as native) for the three calls, the two checksum bytes still stored through the array (through the pointer they fold to [r3, #1]: 186).
 * - 177 -> 174: the transfer loop tests the mask variable v1 (r1, caller-saved at [sp, #8], as native).
 * - 174 -> 171: Func_0200939c (the redraw prepare) called with the window alone: the native leaves the mask in r1, the draft rematerialized 3 for a second argument it does not take.
 * - 171 -> 171: v7 as the transfer counter (r7, as native); no change alone, but it frees r5: count no longer conflicts with v5.
 * - 171 -> 122: v5 as the greeting-loop mask: with count gone v5 keeps r5, a callee-saved register, so no caller save splits the loop.
 * - 122 -> 120: v7 = 0 before v5 = 3, as the native orders them.
 * - 120 -> 116: the peer greetings as for (v1 = 0; v1 != 4; v1++) over gLinkPeerSignatures[v1]: the row pointer is a giv set after the hoisted 0x30, as native.
 * - 116 -> 108: the page end as v6 = page * 50 + 50: page * 50 is its own temporary (r0), copied to v6 and later to v7, as native.
 * - 108 -> 92: the LEFT and RIGHT arms compute the new page number and one Func_02001e6e call follows them, where the native has its one shared mov r1, r8 / bl (195 when the score was 181: the earlier pool and help placement hid it).
 * - 92 -> 80: the page number as rec5 = page + page_count, then rec5-- or rec5++ in place: r0 throughout, as native.
 * - 80 -> 78: the redraw prepare before v7 = 0 and v5 = 2 for the dividers, as the native calls it first.
 * - no gain: the row call into rec (108) or into v3 shifted after the arms (243); the column test on the call directly (108); at 78, v3 = record << 4 before x in each arm (234) and the column in rec apart from the row's record (78).
 * - no gain: v5 as the greeting-loop mask while count shared the transfer loop (399; with v7 as the transfer counter 395); v7 as the transfer counter alone (495); the row call into v3 shifted in place, in each arm or after (356); one Func_02001e6e call after the LEFT and RIGHT arms, v7 = 1 before or after it (195).
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
extern char SceneId_Opening;
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
void Engine_UiWorkFinalize(s32 window, s32 mode);
void Engine_RenderOutputPrepareForRedraw(s32 window);

extern u8 gSaveBuffer[];
struct SaveMenuRecord {
    s32 word_00;
    s32 word_04;
    u8 unknown_08[0x1fd];
    u8 value_205;
    u8 value_206;
    u8 unknown_207[8];
    u8 value_20f;
    u8 unknown_210[0x1a];
    u8 value_22a;
};

union SaveMenuState {
    struct GameState location;
    struct SaveMenuRecord record;
};

LAYOUT_OFFSET_GUARD(SaveMenuRecord_Value205, struct SaveMenuRecord, value_205, 0x205);
LAYOUT_OFFSET_GUARD(SaveMenuRecord_Value20f, struct SaveMenuRecord, value_20f, 0x20f);
LAYOUT_OFFSET_GUARD(SaveMenuRecord_Value22a, struct SaveMenuRecord, value_22a, 0x22a);

extern union SaveMenuState Data_02000240;
extern u8 Data_02001100[];
struct SaveLinkGreeting {
    u16 code[4];
    u8 reserved[16];
};

extern struct SaveLinkGreeting Data_02002224;
extern struct SaveLinkGreeting gLinkPeerSignatures[4];
extern u8 Data_03001ebc[];
void Func_02000438();
s32 Func_0200096c();
s32 Func_020009e0();
void Func_02000f0c();
s32 Func_0200171a();
void Func_020017b2();
void Func_020017c0();
void Func_020017ca();
s32 Func_020017d2();
void Func_020017ee();
void Func_02001804();
void Func_02001814();
s32 Func_02001816();
void Func_0200939c();
s32 Func_0200938c(s32, s32);
s32 Func_02001830();
void Func_0200184a();
void Func_020094ac(void);
void Func_020093dc();
void Func_020093ac();
s32 Func_020018b6();
void Func_020018c0();
s32 Func_020018c8();
s32 Func_020094c4(s32);
s32 Func_02009494(s32);
void Func_020018fa();
void Func_0200190c();
void Func_02001912();
void Func_0200192a();
void Func_02001938();
s32 Func_02001940();
void Func_02001944();
void Func_02001956();
s32 Func_02001978();
void Func_020019c2();
s32 Func_020019ca();
void Func_020019d2();
void Func_020019dc();
void Func_020019f0();
void Func_020019fc();
void Func_02001a16();
void Func_02001a1c();
s32 Func_02001a2e();
void Func_02001a38();
s32 Func_02001a6e();
void Func_0200945c(s32);
void Func_02009464(s32);
void Func_02001aa2();
void Func_02001ab4();
void Func_02001af0();
void Func_02001af2();
void Func_02001b00();
s32 Func_02001b10();
void Func_02001b58();
s32 Func_02001b84();
s32 Func_02001b8e();
void Func_02001b9a();
void Func_02001b9e();
s32 Func_02001bb0();
s32 Func_02001bb4();
void Func_02009474(s32);
void Func_02001bc2();
void Func_02001bc8();
void Func_02001bca();
void Func_02001bcc();
void Func_0200948c(void);
void Func_02001bd2();
void Func_02001bd6();
void Func_02001bd8();
void Func_02001bde();
void Func_02001bec();
void Func_02001bf0();
void Func_02001bf6();
s32 Func_02001bf8();
void Func_02001c34();
void Func_02001c42();
void Func_02001c4a();
void Func_02001c56();
void Func_02001c72();
s32 Func_02001c80();
s32 Func_02001c94();
void Func_02001ca0();
void Func_02001cae();
void Func_02001cb6();
void Func_02001cb8();
s32 Func_02001cbe();
void Func_02009374();
void Func_02001d1e();
void Func_02001d2e();
void Func_02001d36();
s32 Func_02009314(s32, s32);
void Func_02001d6e();
void Func_020094cc(s32);
s32 Func_020093d4(s32);
s32 Func_02001dae();
s32 Func_02001dce();
s32 Func_02001df4();
void Func_02001e06();
s32 Func_02001e12();
void Func_02001e32();
void Func_02001e50();
s32 Func_02001e6e();
void Func_02001e76();
void Func_02001ece();
void Func_02001ee2();
void Func_02001f1a();
s32 Func_02001f3c();
s32 Func_02001f4a();
s32 Func_02001f4e();
s32 Func_02001f6e();
s32 Func_02001f72();
void Func_02001f92();
void Func_02001fa2();
void Func_02001faa();
void Func_02001fac();
void Func_02001fb4();
void Func_020093e4(s32, s32, s32, s32, s32);
void Func_02002012();
void Func_02002030();
void Func_0200213c();
void Func_02002150();
void Func_02002166();
void Func_02002172();
void Func_0200217a();

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
    s32 base3_2000240;
    s32 v5;
    s32 v7;
    s32 base2_2000240;
    s32 count;
    s32 base5_2000240;
    s32 v6;
    s32 link_message;
    s32 v1;
    s32 v3;
    s32 v4;
    s32 n;
    s32 base7_3001c94;
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
    Func_02000438();
    Value2(Func_0200171a, 0x20081fd, 0xc80);
    *(s32 *)((*(u8 **)Data_03001ebc + 0x1c0)) = started;
    Func_020094ac();
    Func_020018c0();
    if (Data_02000240.location.entrance == 2) {
    L_ask:
        Func_020017b2((s32)MsgSaveAdventureQuestion, 5);
        v7 = Value1(Func_020094c4, 1);
        Func_020017ee();
        if (v7 == 0) {
            Func_020017ca(((s32)MsgSaveAdventureQuestion + 1), 1);
            Data_02000240.record.value_20f = 1;
            v7 = Func_02001830();
            if (v7 == -1) {
                goto L_ask;
            }
        }
        Func_02009494(60);
        Func_0200190c();
        Func_0200192a(17);
        Call1(Func_020018fa, 0x12c);
        Func_02001912((s32)&SceneId_WorldMap, 72);
        return 0;
    } else {
        *(u8 *)0x03001ca0 = started;
    }
    L_020004a8:;
    v6 = Value0(Func_020018b6);
    if (v6 < 0) {
        if (*(u8 *)0x03001f54 != 0) {
            Data_02000240.record.value_22a = 1;
            *(u8 *)0x03001d08 = 1;
            Func_020093dc((s32)MsgNoBackupMemory, 1, 8);
        }
    }
    if (v6 == 0) {
        if (started == 0) {
            Func_02001804(30);
            Value2(Func_02001816, 0x2008155, 0xc80);
            Func_02001814(1);
            started = 1;
        }
    }
    if (v6 > 0) {
        v6 = Value0(Func_020018c8);
    } else {
        v6 = 0;
    }
    if (v6 != 0) {
        goto L_0200057a;
    }
    Func_02001956();
    Func_02001938(Data_02000240.record.value_205, Data_02000240.record.value_206);
    v5 = 1;
    v7 = 0;
    L_02000522:;
    Func_0200184a(6);
    v6 = Func_02001940(v7);
    if (v6 == -1) {
        if (v7 == 0) {
            goto L_020004a8;
        }
        v7--;
        goto L_02000522;
    }
    if (*(s16 *)(0x20096b2) != 0) {
        v5 = 4;
    }
    if (*(s16 *)(0x20096b4) != 0) {
        v5 = 7;
    }
    v7 = (v7 + 1);
    if (v7 < v5) {
        goto L_02000522;
    }
    Func_020019dc();
    Data_02000240.location.scene = (s32)&SceneId_Opening;
    Data_02000240.location.entrance = 20;
    goto L_02000c96;
    L_0200057a:;
    if (v6 != 1) {
    } else {
        union SaveMenuState *state;

        v6 = Func_02001978(1);
        if (v6 == -1) {
            goto L_020004a8;
        }
        /* FAKEMATCH: called directly, CSE keeps 0x109 in r7 for the later clears; the native derives them from 0x1c2 (522 vs 573). */
        Call1((void (*)())GameFlag_SetBit, 0x109);
        state = &Data_02000240;
        Func_020019c2(state->record.value_205, state->record.value_206);
        if (state->record.word_00 != Func_02001a2e()) {
            state->location.scene = state->location.saved_scene;
            state->location.entrance = state->location.saved_entrance;
            GameFlag_ClearBit(0x109);
        } else if ((gKeysHeld & 0x208) == 0x208) {
            if (Scene_GetModeMask() != 0) {
                UiText_ShowPositionedMessageAndWait((s32)MsgSaveDataCorrupted + 2, 9);
                goto L_020004a8;
            }
            state->location.scene = state->location.saved_scene;
            state->location.entrance = state->location.saved_entrance;
            GameFlag_ClearBit(0x109);
            GameFlag_SetBit(0x13e);
        } else if (state->record.word_04 != ((struct SaveMenuSceneState *)gSceneState)->word_100) {
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
            state->location.scene = state->location.saved_scene;
            state->location.entrance = state->location.saved_entrance;
            GameFlag_ClearBit(0x109);
            GameFlag_SetBit(0x13f);
        }
        GameFlag_ClearBit(0x106);
        goto L_02000c96;
    }
    if (v6 == 2) {
        Func_02001aa2();
        goto L_020004a8;
    }
    if (v6 == 3) {
        Func_02001ab4();
        goto L_020004a8;
    }
    if (v6 == 4) {
        v6 = Func_02001b10(4);
        if (v6 == -1) {
            goto L_020004a8;
        }
        v5 = 0x2000240;
        v5 = 0x2000240;
        v5 = 0x2000240;
        Data_02000240.location.selected_actor = 0;
        if (Value1(Func_02001b8e, 0x952) != 0) {
            Func_0200948c();
            Func_02009474(0);
            Func_02001bc2(1);
            Func_02001bc8(2);
            Func_02009474(3);
            Func_02001bcc(0);
            Func_02001bd2(1);
            Func_02001bd8(2);
            Func_02001bde(3);
        }
        base3_2000240 = (s32)&Data_02000240;
        base3_2000240 = (s32)&Data_02000240;
        Func_02001b9e(Data_02000240.record.value_205, Data_02000240.record.value_206);
        Call1(Func_02001bec, 0x109);
        Func_02009464(0x106);
        Func_0200945c(0x17e);
        *(u8 *)0x03001ca0 = 1;
        Func_02001c4a((s32)&SceneId_LinkLobby, 1);
        goto L_02000c96;
    } else {
        if (v6 != 5) {
            goto L_020004a8;
        }
        L_020007b8:;
        v6 = Func_02001bb0(5);
        if (v6 == -1) {
            goto L_020004a8;
        }
        Func_02001bf6(Data_02000240.record.value_205, Data_02000240.record.value_206);
        L_020007dc:;
        v6 = Func_02001bb4(0);
        if (v6 == -1) {
            goto L_020007b8;
        }
        if (v6 != 1) {
            goto L_020009cc;
        }
        v6 = Func_02001b84(6, 5, 18, 8, 2);
        link_message = (s32)MsgClearSaveLabel;

        Func_020093ac(link_message, v6, 0, 4);
        Func_02001bca((link_message + 1), v6, 0, 16);
        Func_02001bd6((link_message + 3), v6, 0, 36);
        Func_02001b9a();
        Func_02001b58(10);
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
            Func_02001bf0(1);
        }
        Func_020094cc(113);
        L_020008e0:;
        Func_02001c72(v6, 2);
        if (v7 == 0) {
            goto L_020007dc;
        }
        v6 = Func_02001c80(5, 10, 20, 4, 2);

        Call4(Func_02001cb6, (s32)MsgClearSendingData, v6, 0, 4);
        Func_02001c34(10);
        Value2(Func_02001c94, (s32)gSaveBuffer, 0x1004);
        Func_02001c42(10);
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
            Func_02001c56(1);
        }
    }
    L_0200095e:;
    if (v4 == 0) {
        Func_0200939c(v6);
        Call4(Func_02001d1e, (s32)MsgClearCommunicationError, v6, 0, 4);
            base7_3001c94 = 0x3001c94;
        v5 = 1;
        do {
            Func_02001ca0(1);
            v7 = base7_3001c94;
        } while ((*(s32 *)base7_3001c94 & 1) == 0);
    }
    Func_02001cae(10);
        Func_02009374();
    Func_02001cb8(10);
    Func_02001d36(v6);
    Func_02001d2e(v6, 2);
    goto L_020004a8;
    L_020009cc:;
    if (v6 == 0) {
        L_020009d2:;
        v6 = Value1(Func_020093d4, 1);

        if (v6 == -1) {
            goto L_020007dc;
        }
        {
            u8 *data = save_data;
            s16 sum;

            encoded_size = Value3(Func_020017d2, 0, v6, data);
            sum = Func_02001cbe(encoded_size, data);
            save_data[encoded_size] = (u16)sum >> 8;
            save_data[encoded_size + 1] = sum;
            encoded_size += 2;
            encoded_size = Value3(Func_02001bf8, data, encoded_size, password);
        }
        v6 = 2;
        Func_02001d6e();
        page_window = Func_02001dae(5, 4, 20, 12, v6);
        page = 0;
        page_count = (Func_02009314(encoded_size, 50) + 1);
        record = Func_02001dce(10, 0, 10, 4, v6);
        page_message = (s32)MsgClearPasswordLabel;
        heading_window = record;
        Func_02001e06(page_message, heading_window, 6, 4);
        v7 = 1;
        if (page_count == 1) {
            record = Func_02001df4(5, 16, 20, 3, v6);
            help_window = record;
            Call4(Func_02001e32, page_message - 2, help_window, 80, 0);
        } else {
            record = Func_02001e12(1, 16, 28, 3, v6);
            help_window = record;
            Call4(Func_02001e50, page_message - 1, help_window, 0, 0);
        }
        Call1(Func_02001ece, 0x6006000);
        Engine_RenderOutputPrepareForRedraw(page_window);
        for (;;) {
        Call1(Func_02001ee2, 0x6002500);
        if ((gKeyState & KEY_B) != 0) {
            Func_02001e76(113);
            Engine_RenderOutputPrepareForRedraw(page_window);
            Engine_UiWorkFinalize(page_window, 2);
            Engine_UiWorkFinalize(help_window, 2);
            Engine_UiWorkFinalize(heading_window, 2);
            goto L_020009d2;
        }
        if ((gKeyState & KEY_A) != 0) {
            page = (page + 1);
            v7 = 1;
            if (page == page_count) {
                Func_02001944(112);
                Engine_RenderOutputPrepareForRedraw(page_window);
                Engine_UiWorkFinalize(page_window, 2);
                Engine_UiWorkFinalize(help_window, 2);
                Engine_UiWorkFinalize(heading_window, 2);
                Func_020017c0(1);
                goto L_020004a8;
            }
            Func_02001fa2(111);
        } else {
            if ((gKeyState & KEY_LEFT) != 0) {
                if (page_count <= 1) {
                    goto L_02000b30;
                }
                Func_020094cc(111);
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
                Func_02002012(111);
                rec5 = page + page_count;
                rec5++;
            }
            rec5 = Func_02001e6e(rec5, page_count);
            v7 = 1;
            page = rec5;
        }
        L_02000b58:;
        if (v7 != 1) {
            v5 = page_message;
        } else {
            Engine_RenderOutputPrepareForRedraw(page_window);
            v7 = 0;
            v5 = 2;
            do {
                Func_02001f1a(page_window, 0, v5, 18, v5);
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
                        Func_02001f92(page_window, value, v7 + 18 - page_count, -1, v6);
                        v7 = (v7 + 1);
                    } while (v7 != page_count);
                }
                v5 = 0;
                Func_02001fac(page_window, 0xf128, (17 - (s32)page_count), -1, 0);
                Func_020093e4(page_window, 0xf129, 18, -1, 0);
                *(u8 *)((*(s32 *)0x03001e8c + 0xea3)) |= (2 << ((u32)*(u16 *)(page_window + 14) >> 2));
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
                    Func_02000f0c(password[v7] & 63, glyph);
                } while (0);
                record = Func_02001f3c(v7, 10);
                if (record > 4) {
                    rec7 = Func_02001f4a(n, 10);
                    row = Func_02001f4e(n, 10);
                    Func_02002030(glyph, page_window, rec7 * 12 + 18, (row << 4) + 2);
                } else {
                    rec7 = Func_02001f6e(n, 10);
                    row = Func_02001f72(n, 10);
                    Func_02002030(glyph, page_window, rec7 * 12 + 8, (row << 4) + 2);
                }
            }
            v7 = 0;
        }
        Func_02001faa(1);
        }
    }
    Call1(Func_02001fb4, 0x12c);
    goto L_020004a8;
    L_02000c96:;
    *(u16 *)((*(u8 **)Data_03001ebc + 0x170)) = 0x3e7;
    Func_0200213c(30);
    Func_0200217a(17);
    Func_02002166();
    Func_02002172();
    Func_02002150(60);
    L_Return:;
    return 0;
}

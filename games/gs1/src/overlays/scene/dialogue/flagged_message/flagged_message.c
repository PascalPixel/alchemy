#include "types.h"
#include "select_overlay_data_by_runtime_selector.h"
#include "select_overlay_data_by_runtime_selector_body.inc"

/* overlays/scene/dialogue/flagged_message/actor_presentation.c */
#define Actor_SetMode3AndFlagBit1 Func_02000b68
u8 *Func_0200210e(s32);
u8 *Func_02002116(s32);
void Func_0200208c(u8 *, s32);
void Func_020021cc(s32, s32);

void Actor_SetMode3AndFlagBit1(s32 no)
{
    u8 *p = Func_0200210e(no);
    u8 *flag;

    Func_0200208c(Func_02002116(no), 0);
    Func_020021cc(no, 3);
    flag = p + 85;
    *flag = 0;
    p += 35;
    {
        u8 bit = 2;

        *p = bit | *p;
    }
}
#undef Actor_SetMode3AndFlagBit1

/* overlays/scene/dialogue/flagged_message/scene_data.c */
#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_0000006b
#define SecondaryRuntimeSelector Value_00000070
#define TertiaryRuntimeSelector Value_0000006c
#define PrimaryOverlayData Data_02009738
#define SecondaryOverlayData Data_020097c8
#define TertiaryOverlayData Data_02009840
#define DefaultOverlayData Data_02009708
#define SceneData_SelectByRuntimeSelector Func_02000040
#define SceneData_GetTable98a0 Func_02000098
#define SceneData_SelectDataBySelectorAndFlags Func_020000a0
#define SceneData_SelectSecondaryDataBySelectorAndFlags Func_0200019c


extern s16 Data_02000240[];
extern u8 Value_0000006b;
extern u8 Value_00000070;
extern u8 Value_0000006c;
extern u8 Data_020098d4[];
extern u8 Data_020098ec[];
extern u8 Data_020099c4[];
extern u8 Data_02009acc[];
extern u8 Data_02009ba4[];
extern u8 Data_02009c7c[];
extern u8 Data_02009d24[];
extern u8 Data_02009dcc[];
extern u8 Data_02009e74[];
extern u8 Data_0200a018[];
extern u8 Data_0200a120[];
extern u8 Data_02009e80[];
extern u8 Data_02009fa0[];
extern u8 Data_0200a24c[];
extern u8 Data_0200a30c[];
extern u8 Data_0200a390[];

s32 Func_02001608();
s32 Func_02001622();
s32 Func_0200163c();
s32 Func_0200164a();
s32 Func_02001704();
s32 Func_0200171e();
s32 Func_02001738();
s32 Func_02001746();

/*
 * Data selector for resource_3ae.  The 164-byte owner includes its 13-word
 * literal pool.
 */

/* Per-call-site import labels.  All four sites reach the same event-flag test,
 * but each keeps its own name because the relocated call word differs. */

/*
 * All four symbols name the loader-relocated call word each site holds; they
 * reach one event-flag test.
 */

s32 SceneData_SelectByRuntimeSelector(void)
{
}

/*
 * Table getter for resource_3ae. The owner at 0x02000098 is eight bytes and
 * includes its one pool word at 0x0200009c: the pc-relative load reads that
 * word, so the word is part of the owner and not a following gap.
 *
 * The pool word is an address that is returned without being dereferenced.
 * Many getters share this body but each returns a different address, so the
 * constant here is not interchangeable with theirs.
 */
u8 *SceneData_GetTable98a0(void)
{
    return (u8 *)0x020098a0;
}

/*
 * The selector is the signed halfword at byte offset 448 of the resident
 * workspace, Data_02000240[224].  It is compared as a signed 32-bit value
 * against full words held in the pool, which is why the three constants are
 * spelled as the addresses of Value_ symbols rather than as immediates.
 * 0x93e and 0x950 are read as event-flag ids only from their use as the sole
 * argument of the flag test.
 */
s32 SceneData_SelectDataBySelectorAndFlags(void)
{
    s16 room = Data_02000240[224];

    if (room == (s32)&Value_0000006b) {
        if (Func_02001608(0x93e) != 0) {
            return (s32)Data_02009ba4;
        }
        return (s32)Data_02009acc;
    }

    if (room == (s32)&Value_00000070) {
        /* 0x950 is built from an immediate and a shift. */
        if (Func_02001622(0x950) != 0) {
            return (s32)Data_020099c4;
        }
        return (s32)Data_020098ec;
    }

    if (room == (s32)&Value_0000006c) {
        if (Func_0200163c(0x950) != 0) {
            return (s32)Data_02009dcc;
        }
        if (Func_0200164a(0x93e) != 0) {
            return (s32)Data_02009d24;
        }
        return (s32)Data_02009c7c;
    }

    return (s32)Data_020098d4;
}

/*
 * Pick one of eight in-image data tables from the scene selector and two
 * event flags. The selector is the signed halfword at offset 448 of the
 * shared workspace, and each arm compares it as a signed 32-bit value
 * against a full word, which is why the selector constants are spelled as
 * addresses. 0x93e and 0x950 are taken to be event-flag ids only from their
 * use here. The 164-byte owner includes its thirteen-word literal pool.
 */
s32 SceneData_SelectSecondaryDataBySelectorAndFlags(void)
{
    s16 scene = Data_02000240[224];

    if (scene == (s32)&Value_0000006b) {
        if (Func_02001704(0x93e) != 0) {
            return (s32)Data_02009fa0;
        }
        return (s32)Data_02009e80;
    }

    if (scene == (s32)&Value_00000070) {
        /* 0x950 is built by shifting a small constant, not loaded. */
        if (Func_0200171e(0x950) != 0) {
            return (s32)Data_0200a120;
        }
        return (s32)Data_0200a018;
    }

    if (scene == (s32)&Value_0000006c) {
        if (Func_02001738(0x950) != 0) {
            return (s32)Data_0200a390;
        }
        if (Func_02001746(0x93e) != 0) {
            return (s32)Data_0200a30c;
        }
        return (s32)Data_0200a24c;
    }

    return (s32)Data_02009e74;
}
#undef RuntimeSelectorTable
#undef PrimaryRuntimeSelector
#undef SecondaryRuntimeSelector
#undef TertiaryRuntimeSelector
#undef PrimaryOverlayData
#undef SecondaryOverlayData
#undef TertiaryOverlayData
#undef DefaultOverlayData
#undef SceneData_SelectByRuntimeSelector
#undef SceneData_GetTable98a0
#undef SceneData_SelectDataBySelectorAndFlags
#undef SceneData_SelectSecondaryDataBySelectorAndFlags

/* overlays/scene/dialogue/flagged_message/scene_dialogue.c */
#define Dialogue_RunActor12Event Func_02000328
#define Dialogue_ShowLine1CF8 Func_02000240
#define Dialogue_RunActor8Message1f09 Func_02000688
#define Dialogue_RunActor10Message1f15 Func_020006a8

extern u8 Data_00001d20[];
extern u8 Data_03001ebc[];

void Func_020017c4(void);
void Func_02001862(s32);
s32 Func_0200188a(s32, s32);
void Func_020017de(void);
s32 Func_02001898();
s32 Func_020018a2();
void Func_020018c2();
u8 *Func_020018ce();
void Func_02001910();
void Func_02001922();
s32 Func_02001926();
void Func_0200193c();
s32 Func_0200194a();
s32 Func_0200194c();
s32 Func_0200194c_next();
void Func_02001974();
void Func_0200197a();
void Func_02001984();
void Func_02001996();
s32 Func_020019a6();
void Func_020019ac();
void Func_020019b6();
void Func_020019be();
void Func_020019ce();
void Func_020019ea();
void Func_020019ea_position();
u8 *Func_020019fa();
void Func_020019fe();
void Func_020019fe_mode();
void Func_02001a02();
void Func_02001a04();
void Func_02001a1c();
void Func_02001a20();
void Func_02001a30();
void Func_02001a3c();
void Func_02001a48();
void Func_02001a6e();
void Func_02001a6e_close();
void Func_02001abc();
void Func_02001aca();
void Func_02001c0c(void);
void Func_02001caa(s32);
s32 Func_02001cd2(s32, s32);
void Func_02001c26(void);
void Func_02001c2c(void);
void Func_02001cca(s32);
s32 Func_02001cf2(s32, s32);
void Func_02001c46(void);

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Contiguous unnamed leaf-owner run for resource_3ae. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void Dialogue_ShowLine1CF8(void)
{
    Func_020017c4();
    Func_02001862(0x1CF8);
    Func_0200188a(8, 0);
    Func_020017de();
}

void Dialogue_RunActor12Event(void)
{
    s32 rec7;
    s32 rec8;
    u8 *record;
    s16 angle;

    record = Func_020018ce(0);
    angle = (u16)((*(u16 *)(record + 6) + 0x2000) & ~0x3fff);
    Func_020018c2();
    if (Value1(Func_02001898, 0x8a7) != 0) {
        if (Value1(Func_020018a2, 0x8a9) != 0) {
            Call1(Func_02001974, 0x1d23);
            Func_02001984(12, 0);
            goto L_02000496;
        }
        Func_02001996((s32)Data_00001d20);
        Value2(Func_020019a6, 12, 0);
        if (Value2(Func_02001926, 0, 0) == 0) {
            Func_02001910(10);
            Func_020019b6(((s32)Data_00001d20 + 1));
            Func_020019ce(12, 0);
            Call3(Func_0200197a, 12, 88, 0x508);
            Call3(Func_020019fe, 12, 0x4000, 0);
            Func_0200193c(20);
            Call1(Func_02001922, 0x8a9);
            goto L_02000496;
        }
        Func_020019ea(((s32)Data_00001d20 + 2));
        Func_02001a02(12, 0);
    } else {
        if ((u16)angle != 0x8000) {
            goto L_0200049a;
        }
        Call1(Func_02001a04, 0x1d16);
        Func_02001a1c(12, 0);
        if (Value1(Func_0200194a, 0x8a5) != 0) {
            rec7 = Func_0200194c(235);
            rec8 = Func_0200194c_next(rec7, 235);
            Func_020019fe_mode(12, 3);
            Call3(Func_020019ea_position, 12, 88, 0x508);
            Call3(Func_02001a6e, 12, 0x4000, 0);
            bump_step(1);
            Func_02001a6e_close(12, 0);
            Func_020019be(rec7, rec8);
            Call1(Func_020019ac, 0x8a7);
            record = Func_020019fa(0);
            Call3(Func_02001a30, 0, *(s16 *)(record + 10), 0x518);
            Call3(Func_02001a3c, 0, 72, 0x518);
            Call3(Func_02001a48, 12, 88, 0x518);
            Func_02001aca(12, 0, 0);
        } else {
            Func_02001abc(12, 0);
        }
    }
    L_02000496:;
    Func_02001a20();
    L_0200049a:;
}

void Dialogue_RunActor8Message1f09(void)
{
    Func_02001c0c();
    Func_02001caa(0x1F09);
    Func_02001cd2(8, 0);
    Func_02001c26();
}

void Dialogue_RunActor10Message1f15(void)
{
    Func_02001c2c();
    Func_02001cca(0x1F15);
    Func_02001cf2(10, 0);
    Func_02001c46();
}
#undef Dialogue_RunActor12Event
#undef Dialogue_ShowLine1CF8
#undef Dialogue_RunActor8Message1f09
#undef Dialogue_RunActor10Message1f15

/* overlays/scene/dialogue/flagged_message/scene_primary_script.c */
#define Battle_Reset_1(args...) Func_02002124(args)
#define Motion_SetHPosTerrain_1(a0, a1, a2) Call3(Func_0200218e, a0, a1, a2)
#define Scene_GetRecord_1(args...) Func_0200215c(args)
#define Battle_WaitMode0_1(args...) Func_0200223e(args)
#define Motion_SetSpeed_1(args...) Func_0200224a(args)
#define Battle_WaitMode0_2(args...) Func_02002148(args)
#define Motion_RealignToTrackedObjectAndArmCallback_1(a0, a1, a2, a3) Call4(Func_02002278, a0, a1, a2, a3)
#define Motion_CommitPos_1(args...) Func_020021b6(args)
#define Motion_ArmCb_1(a0, a1, a2) Call3(Func_02002232, a0, a1, a2)
#define Battle_WaitMode0_3(args...) Func_02002170(args)
#define SceneWork_SetStepValue_1(a0) Call1(Func_02002216, a0)
#define Motion_ArmCb_2(a0, a1, a2) Call3(Func_0200224a_a, a0, a1, a2)
#define Motion_SetSpeed_2(a0, a1, a2) Call3(Func_020021bc, a0, a1, a2)
#define Motion_SetPosReset_1(a0, a1, a2) Call3(Func_020021e8, a0, a1, a2)
#define Motion_ArmCb_3(a0, a1, a2) Call3(Func_0200226c, a0, a1, a2)
#define Motion_CamBounds_1(a0, a1, a2, a3) Call4(Func_0200229e, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_1(args...) Func_020022aa(args)
#define Battle_WaitMode0_4(args...) Func_020021c0(args)
#define Motion_Launch_1(args...) Func_02002242(args)
#define Motion_Launch_2(args...) Func_0200224c(args)
#define BattleEv_RunWait_1(args...) Func_0200228c(args)
#define Battle_WaitMode0_5(args...) Func_020021e2(args)
#define Motion_CamBounds_2(a0, a1, a2, a3) Call4(Func_020022dc, a0, a1, a2, a3)
#define Object_CommitPositionThenWaitIfModeZero_2(args...) Func_020022e8(args)
#define Battle_WaitMode0_6(args...) Func_020021fe(args)
#define BattleFx_SpawnLinked_1(a0, a1, a2) Call3(Func_020022e2, a0, a1, a2)
#define Motion_SetSpeed_3(a0, a1, a2) Call3(Func_02002244, a0, a1, a2)
#define Motion_SetPosReset_2(a0, a1, a2) Call3(Func_02002272, a0, a1, a2)
#define Motion_ArmCb_4(a0, a1, a2) Call3(Func_020022f6, a0, a1, a2)
#define Battle_WaitMode0_7(args...) Func_02002234(args)
#define Motion_ArmCb_5(args...) Func_02002306(args)
#define Battle_WaitMode0_8(args...) Func_02002244_a(args)
#define Battle_WaitMode0_9(args...) Func_0200224a_b(args)
#define Motion_CallWaitAnim_1(args...) Func_020022c2(args)
#define Battle_WaitMode0_10(args...) Func_02002258(args)
#define BattleEv_RunWait_2(args...) Func_02002310(args)
#define Battle_WaitMode0_11(args...) Func_02002266(args)
#define BattleFx_SpawnLinked_2(a0, a1, a2) Call3(Func_0200234a, a0, a1, a2)
#define Battle_WaitMode0_12(args...) Func_02002278_a(args)
#define Motion_ArmCb_6(a0, a1, a2) Call3(Func_0200234c, a0, a1, a2)
#define Battle_WaitMode0_13(args...) Func_0200228a(args)
#define BattleEv_RunWait_3(args...) Func_02002342(args)
#define Battle_WaitMode0_14(args...) Func_02002298(args)
#define Motion_ArmCb_7(a0, a1, a2) Call3(Func_0200236c, a0, a1, a2)
#define Battle_WaitMode0_15(args...) Func_020022aa_a(args)
#define Motion_SetSpeed_4(a0, a1, a2) Call3(Func_020022e8_a, a0, a1, a2)
#define Motion_SetPosReset_3(a0, a1, a2) Call3(Func_02002316, a0, a1, a2)
#define Motion_ArmCb_8(args...) Func_02002398(args)
#define Motion_ArmCb_9(a0, a1, a2) Call3(Func_020023a4, a0, a1, a2)
#define Motion_ArmCb_10(a0, a1, a2) Call3(Func_020023b0, a0, a1, a2)
#define Battle_WaitMode0_16(args...) Func_020022ee(args)
#define BattleEv_RunWait_4(args...) Func_020023a6(args)
#define Battle_WaitMode0_17(args...) Func_020022fc(args)
#define Motion_CallWaitAnim_2(args...) Func_02002374(args)
#define Battle_WaitMode0_18(args...) Func_0200230a(args)
#define Motion_CallWaitAnim_3(args...) Func_02002382(args)
#define Battle_WaitMode0_19(args...) Func_02002318(args)
#define Motion_SetSpeed_5(a0, a1, a2) Call3(Func_02002352, a0, a1, a2)
#define Object_SetModeById_1(args...) Func_02002392(args)
#define Scene_GetRecord_2(a0) Value1(Func_02002358, a0)
#define Motion_ResetAndSetPosition_1(args...) Func_02002382_a(args)
#define Motion_CommitPos_2(args...) Func_020023a0(args)
#define Motion_SetHPosTerrain_2(args...) Func_020023b2(args)
#define Battle_WaitMode0_20(args...) ((void (*)())Func_02002358_a)(args)
#define Scene_GetRecord_3(args...) Func_02002386(args)
#define Motion_EnableActCb_1(a0, a1) Value2(Func_020023a4_a, a0, a1)
#define Scene_GetRecord_4(a0) Value1(Func_0200239a, a0)
#define Battle_SchedShoulder_1(args...) Func_020023aa(args)
#define FieldScene_RunScene3ae_02000144 Func_02000144
#define FieldScene_RunScene3ae_02000260 Func_02000260
#define FieldScene_RunScene3ae_020002dc Func_020002dc
#define Scene_RunOpeningAuxiliarySequence Func_0200051c
#define FieldScene_RunScene3ae_020006c8 Func_020006c8
#define FieldScene_RunScene3ae_020007dc Func_020007dc
#define FieldScene_RunScene3ae_0200086c Func_0200086c
#define FieldScene_RunScene3ae_020008cc Func_020008cc
#define Scene_RunScene3aeSequenceA Func_02000ba0
#define Scene_RunScene3aeSequenceB Func_02000e40

extern u8 Data_03001ebc[];
extern u8 Data_02000240[];
extern u8 Data_0000006b[];
extern u8 Data_00000070[];
extern u8 Data_0000006c[];
extern s16 Data_02000240_t[][1];

void Func_020016a6();
void Func_020016c8();
void Func_020016fe();
void Func_0200170e();
void Func_02001712();
void Func_02001724();
void Func_0200173c();
void Func_020017c0();
s32 Func_020017ba();
void Func_020017e4();
void Func_020017ee();
s32 Func_0200181c();
void Func_0200184c();
void Func_0200188c();
s32 Func_0200189c();
void Func_020018b8();
void Func_020018d0();
void Func_020018d8();
void Func_020018e8();
s32 Func_0200183c();
s32 Func_02001846();
void Func_02001866();
void Func_02001882();
void Func_02001918();
s32 Func_02001928();
void Func_02001954();
s32 Func_02001a82();
void Func_02001aac();
s32 Func_02001ab4();
void Func_02001abc_a();
void Func_02001ad2();
void Func_02001aec();
void Func_02001af2();
s32 Func_02001af4();
void Func_02001b10();
void Func_02001b44();
void Func_02001b54();
void Func_02001b58();
s32 Func_02001b5e();
void Func_02001b64();
void Func_02001b6c();
s32 Func_02001b74();
void Func_02001b76();
void Func_02001b9a();
void Func_02001baa();
s32 Func_02001bac();
void Func_02001bae();
s32 Func_02001bc2();
void Func_02001bc6();
s32 Func_02001be0();
s32 Func_02001be0_a();
void Func_02001bea();
void Func_02001c14();
void Func_02001c42();
void Func_02001c70();
void Func_02001cc2();
s32 Func_02001c22();
void Func_02001c4c();
void Func_02001c5e();
void Func_02001c78();
s32 Func_02001c80();
s32 Func_02001c80_a();
void Func_02001c94();
void Func_02001cb2();
void Func_02001cc6();
void Func_02001cd8();
void Func_02001ce8();
s32 Func_02001cee();
s32 Func_02001cee_a();
void Func_02001d04();
void Func_02001d0c();
void Func_02001d1c();
void Func_02001d1e();
void Func_02001d24();
void Func_02001d34();
void Func_02001d3a();
void Func_02001d48();
void Func_02001d52();
void Func_02001d5c();
s32 Func_02001d6e();
void Func_02001d90();
void Func_02001d96();
void Func_02001db6();
void Func_02001dde();
void Func_02001df6();
void Func_02001d26();
void Func_02001d42();
void Func_02001d60();
void Func_02001d9c();
void Func_02001dc0();
void Func_02001dda();
void Func_02001ddc();
void Func_02001df8();
void Func_02001e44();
void Func_02001e96();
void Func_02001ebe();
void Func_02001164();
void Func_020011fc();
void Func_02001384();
void Func_02001ddc_a();
void Func_02001490();
s32 Func_02001e32();
void Func_02001e44_a();
s32 Func_02001e54();
s32 Func_02001e64();
s32 Func_02001e6e();
s32 Func_02001e6e_a();
void Func_02001f08();
void Func_02001f7a();
void Func_02002124();
void Func_02002148();
s32 Func_0200215c();
void Func_02002170();
void Func_0200218e();
void Func_020021b6();
void Func_020021bc();
void Func_020021c0();
void Func_020021e2();
void Func_020021e8();
void Func_020021fe();
void Func_02002216();
void Func_02002230();
void Func_02002232();
void Func_02002234();
void Func_0200223e();
void Func_02002242();
void Func_02002244();
void Func_02002244_a();
void Func_0200224a();
void Func_0200224a_a();
void Func_0200224a_b();
void Func_0200224c();
void Func_02002258();
void Func_02002266();
void Func_0200226c();
void Func_02002272();
void Func_02002278();
void Func_02002278_a();
void Func_0200228a();
void Func_0200228c();
void Func_02002298();
void Func_0200229e();
void Func_020022aa();
void Func_020022aa_a();
void Func_020022c2();
void Func_020022dc();
void Func_020022e2();
void Func_020022e8();
void Func_020022e8_a();
void Func_020022ee();
void Func_020022f6();
void Func_020022fc();
void Func_02002306();
void Func_0200230a();
void Func_02002310();
void Func_02002316();
void Func_02002318();
void Func_02002342();
void Func_0200234a();
void Func_0200234c();
void Func_02002352();
s32 Func_02002358();
s32 Func_02002358_a();
void Func_0200236c();
void Func_02002374();
void Func_02002382();
void Func_02002382_a();
s32 Func_02002386();
void Func_02002392();
void Func_02002398();
s32 Func_0200239a();
void Func_020023a0();
s32 Func_020023a4();
s32 Func_020023a4_a();
void Func_020023a6();
void Func_020023aa();
void Func_020023b0();
void Func_020023b2();
void Func_020023a4_b();
void Func_020023d0();
u8 *Func_02002400();
void Func_0200243c();
void Func_0200244a();
void Func_02002456();
void Func_02002472();
void Func_02002476();
void Func_0200248c();
void Func_0200249a();
void Func_020024a8();
void Func_020024b6();
void Func_020024c4();
void Func_020024dc();
void Func_020024ec();
void Func_020024f4();
void Func_020024f6();
void Func_020024fa();
void Func_02002500();
void Func_0200250c();
void Func_0200251c();
void Func_02002520();
void Func_02002526();
s32 Func_02002528();
void Func_02002532();
void Func_02002536();
void Func_02002538();
void Func_02002552();
void Func_02002558();
void Func_02002568();
s32 Func_0200256e();
s32 Func_0200256e_a();
s32 Func_0200256e_b();
void Func_0200257a();
void Func_02002586();
void Func_0200258c();
void Func_020025a6();
void Func_020025a8();
void Func_020025b8();
void Func_020025c6();
void Func_020025e2();
void Func_020025fe();
void Func_0200260c();
void Func_02002610();
void Func_02002616();
void Func_02002618();
void Func_0200261a();
void Func_0200262a();
void Func_0200262c();
void Func_0200263a();
void Func_02002640();
void Func_02002648();
void Func_02002656();
void Func_0200265a();
void Func_02002662();
void Func_0200266c();
void Func_02002670();
void Func_02002676();
void Func_0200267a();
void Func_0200268c();
void Func_020026a0();
void Func_020026a6();
void Func_020026aa();
void Func_020026ac();
void Func_020026b4();
void Func_020026b8();
void Func_020026bc();
void Func_020026c0();
void Func_020026c2();
void Func_020026c4();
void Func_020026d4();
void Func_020026de();
void Func_020026e2();
void Func_020026e6();
void Func_020026ee();
void Func_020026f2();
void Func_020026f4();
void Func_0200270e();
void Func_0200271c();
void Func_0200271c_a();
void Func_02002724();
void Func_0200272e();
void Func_02002736();
void Func_0200274e();
void Func_02002750();
void Func_0200275e();
void Func_0200276c();
void Func_02002770();
void Func_02002776();
void Func_02002796();
void Func_0200279e();
s32 Func_020027a6();
void Func_020027a8();
void Func_020027b2();
void Func_020027c2();
void Func_020027c6();
s32 Func_020027cc();
void Func_020027d0();
void Func_020027d4();
void Func_020027ec();
void Func_020027fa();
void Func_02002808();
void Func_02002816();
void Func_02002826();
s32 Func_02002844();
void Func_02002850();
void Func_0200285a();
void Func_02002860();
void Func_02002868();
s32 Func_0200286a();
void Func_0200286e();
void Func_02002872();
void Func_02002882();
void Func_020028a2();
void Func_020028a4();
void Func_020028c0();
void Func_020028ce();
void Func_020028d2();
s32 Func_020028d6();
void Func_020028e0();
s32 Func_020028fa();
void Func_020028fe();
void Func_02002918();
void Func_0200291c();
void Func_02002930();
void Func_02002938();
u8 *Func_02002944();
void Func_02002962();
void Func_0200296c();
void Func_02002970();
void Func_02002978();
void Func_0200297e();
void Func_0200298c();
void Func_020029a8();
void Func_020029ac();
void Func_020029b6();
void Func_020029c0();
void Func_020029c4();
void Func_020029c6();
void Func_020029ca();
void Func_020029ce();
void Func_020029da();
void Func_020029de();
void Func_020029e8();
void Func_020029e8_a();
void Func_020029f6();
void Func_020029fc();
void Func_020029fc_a();
void Func_02002a14();
void Func_02002a1a();
void Func_02002a24();
void Func_02002a32();
void Func_02002a36();
void Func_02002a3a();
void Func_02002a3a_a();
void Func_02002a46();
void Func_02002a52();
void Func_02002a52_a();
void Func_02002a60();
void Func_02002a60_a();
void Func_02002a62();
void Func_02002a66();
void Func_02002a70();
void Func_02002a82();
void Func_02002a9c();
void Func_02002ad2();
void Func_02002af4();
void Func_02002b46();
void Func_02002b50();
void Func_02002b7c();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_020007dc(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

void FieldScene_RunScene3ae_02000144(void)
{
    u32 i;
    s32 record;

    Func_020016c8();
    Call1(Func_020016a6, 0x8aa);
    Call3(Func_02001724, 0, 0x188, 0x128);
    Call3(Func_0200170e, 8, 0x13333, 0x9999);
    Call3(Func_0200173c, 8, 0x198, 0x128);
    Call3(Func_020017c0, 8, 0x8000, 0);
    Func_020016fe(20);
    Func_02001712();
}

void FieldScene_RunScene3ae_02000260(void)
{
    u32 i;
    s32 record;

    Func_020017e4();
    if (Value1(Func_020017ba, 0x8a6) == 0) {
        Call1(Func_0200188c, 0x1cfd);
        Value2(Func_0200189c, 11, 0);
        if (Value2(Func_0200181c, 0, 0) == 0) {
            Func_020018b8(11, 0);
            Call1(Func_020017ee, 0x8a6);
            goto L_020002c2;
        }
        bump_step(1);
        Func_020018d8(11, 0);
    } else {
        Call1(Func_020018d0, 0x1cfe);
        Func_020018e8(11, 0);
    }
    L_020002c2:;
    Func_0200184c();
}

void FieldScene_RunScene3ae_020002dc(void)
{
    u32 i;
    s32 record;

    Func_02001882(0);
    Func_02001866();
    if (Value1(Func_0200183c, 0x8a7) != 0) {
        if (Value1(Func_02001846, 0x8a9) != 0) {
            Call1(Func_02001918, 0x1d23);
            Value2(Func_02001928, 12, 0);
            Call3(Func_02001954, 12, 0x4000, 0);
        }
    }
}

void Scene_RunOpeningAuxiliarySequence(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 base6_2000240;
    s32 six00;

    six00 = 0x258;
    Func_02001aac();
    if (Value1(Func_02001a82, 0x8a5) != 0) {
        Call1(Func_02001b54, 0x1d0b);
        Func_02001b6c(8, 0);
    } else {
        Call1(Func_02001b64, 0x1d04);
        Value2(Func_02001b74, 8, 0);
        if (Value2(Func_02001af4, 0, 0) == 1) {
            Func_02001b9a(8, 0, 10);
        } else {
            bump_step(1);
            Func_02001abc_a(six00, 5);
            Value2(Func_02001bac, 8, 0);
            rec7 = Func_02001ab4(19, 8, 11, 4, 2);
            Call4(Func_02001ad2, 0xc8a, rec7, 0, 0);
            base6_2000240 = (s32)Data_02000240;
            Func_02001aec(*(volatile s32 *)(base6_2000240 + 16), 6, rec7, 24, 8);
            if (Value2(Func_02001b5e, -1, 0) == 1) {
                Func_02001af2(rec7, 2);
                Func_02001bc2(0, 4);
                Func_02001b58(10);
                Func_02001c42(8, 0);
                goto L_02000660;
            } else {
                if ((u32)six00 <= (u32)*(volatile s32 *)(base6_2000240 + 16)) {
                    goto L_0200061e;
                }
                Func_02001b10(rec7, 2);
                Func_02001be0(0, 3);
                Func_02001b76(10);
                bump_step(1);
                Func_02001cc2(113);
                Func_02001c42(8, 0);
                goto L_02000660;
            }
            L_0200061e:;
            Func_02001b44(rec7, 2);
            Func_02001c14(0, 3);
            Func_02001baa(10);
            bump_step(3);
            Func_02001c70(8, 0);
            Func_02001be0_a(235, 0);
            Call1(Func_02001bae, 0x8a5);
            Call1(Func_02001bc6, -six00);
        }
        L_02000660:;
        Func_02001bea();
    }
}

void FieldScene_RunScene3ae_020006c8(void)
{
    u32 i;
    s32 record;

    Func_02001c4c();
    if (Value1(Func_02001c22, 0x8a8) != 0) {
        Func_02001ce8(11, 0, 0);
        Func_02001c5e(20);
        Call1(Func_02001d04, 0x1f1c);
        Func_02001d1c(11, 0);
        ((void (*)())Func_02001c80)();
    } else {
        Func_02001c78(20);
        Call3(Func_02001d5c, 11, 0x100, 50);
        Func_02001d1e(11, 0, 0);
        Func_02001c94(20);
        Call1(Func_02001d3a, 0x1f18);
        Func_02001d52(11, 0);
        if (Value1(Func_02001c80_a, 0x8a6) != 0) {
            Func_02001cb2(20);
            Call3(Func_02001d96, 11, 0x102, 40);
            Value2(Func_02001d6e, 11, 0);
            if (Value2(Func_02001cee, 0, 0) == 0) {
                Func_02001cd8(20);
                Func_02001d90(11, 0);
                Call1(Func_02001cc6, 0x8a8);
                goto L_020007be;
            }
            ((void (*)())Func_02001cee_a)(10);
            bump_step(1);
            Func_02001db6(11, 0);
            Func_02001d0c(10);
            Func_02001dde(11, 0, 0);
            Func_02001d1c_a(30);
        } else {
            Func_02001d24(10);
            Func_02001df6(11, 0, 0);
            Func_02001d34(30);
        }
        L_020007be:;
        Func_02001d48();
    }
}

void FieldScene_RunScene3ae_020007dc(void)
{
    u8 *work;

    Func_02001d60();
    Func_02001e96(158);
    Call3_020007dc(Func_02001d9c, 0, 0x8000, 0x4000);
    Func_02001e44(0, 3);
    work = Data_02000240;
    if (*(s16 *)(work + 0x1c0) == (s32)Data_0000006b) {
        Call3_020007dc(Func_02001ddc, 0, 0x130, 0x570);
        Call3_020007dc(Func_02001d26, 0x20096b8, 78, 86);
    } else {
        if (*(s16 *)(work + 0x1c0) == (s32)Data_00000070) {
            Func_02001df8(0, 248, 192);
            Call3_020007dc(Func_02001d42, 0x20096ce, 74, 9);
        }
    }
    Func_02001dc0(16);
    Func_02001ebe(3);
    Func_02001dda();
}

s32 FieldScene_RunScene3ae_0200086c(void)
{
    u32 i;
    s32 record;

    if (Data_02000240_t[225][0] == 90) {
        Call1(Func_02001ddc_a, 0x950);
    }
    if (Data_02000240_t[224][0] == (s32)Data_0000006b) {
        Func_02001164();
    } else {
        if (Data_02000240_t[224][0] == (s32)Data_00000070) {
            Func_020011fc();
        } else {
            if (Data_02000240_t[224][0] == (s32)Data_0000006c) {
                Func_02001384();
            }
        }
    }
    return 0;
}

void FieldScene_RunScene3ae_020008cc(void)
{
    u32 i;
    s32 record;

    if (Data_02000240_t[225][0] == 1) {
        if (Value1(Func_02001e32, 0x8ac) == 0) {
            Call1(Func_02001e44_a, 0x8ac);
            Func_02001490();
        }
    }
    if (Data_02000240_t[225][0] == 2) {
        if (Value1(Func_02001e54, 0x109) == 0) {
            Call1(Func_02001e6e, 0x8a9);
        }
    }
    if (Value1(Func_02001e64, 0x911) != 0) {
        if (Value1(Func_02001e6e_a, 0x8a9) == 0) {
            Call3(Func_02001f08, 12, 0x580000, 0x5180000);
            Func_02001f7a(12, 0, 0);
        }
    }
}

/* Runs a fixed sequence of scene calls (positioning, timing, and animation
 * triggers) for scene 3ae, sequence A. */
void Scene_RunScene3aeSequenceA(void)
{
    u32 i;
    s32 record;

    Battle_Reset_1();
    Func_02002230();
    Motion_SetHPosTerrain_1(8, 0x1480000, 0x5900000);
    /* Set the flag byte at +91 of record 8. */
    *(u8 *)(Scene_GetRecord_1(8) + 91) = 1;
    Battle_WaitMode0_1();
    Motion_SetSpeed_1();
    Battle_WaitMode0_2(20);
    Motion_RealignToTrackedObjectAndArmCallback_1(1, -16, 0, 0x8000);
    Motion_CommitPos_1(1);
    Motion_ArmCb_1(0, 0xa000, 0);
    Battle_WaitMode0_3(20);
    SceneWork_SetStepValue_1(0x1f89);
    Motion_ArmCb_2(0, 0xa000, 0);
    Motion_SetSpeed_2(1, 0x19999, 0xcccc);
    Motion_SetPosReset_1(1, 232, 0x590);
    Motion_ArmCb_3(1, 0x8000, 0);
    Motion_CamBounds_1(0xb80000, -1, 0x5a00000, 1);
    Object_CommitPositionThenWaitIfModeZero_1();
    Battle_WaitMode0_4(10);
    Motion_Launch_1(1, 6, 15);
    Motion_Launch_2(1, 6, 40);
    BattleEv_RunWait_1(1, 0);
    Battle_WaitMode0_5(20);
    Motion_CamBounds_2(0x1080000, -1, 0x5a80000, 1);
    Object_CommitPositionThenWaitIfModeZero_2();
    Battle_WaitMode0_6(20);
    BattleFx_SpawnLinked_1(8, 0x100, 50);
    Motion_SetSpeed_3(8, 0x13333, 0x9999);
    Motion_SetPosReset_2(8, 0x108, 0x590);
    Motion_ArmCb_4(8, 0x8000, 0);
    Battle_WaitMode0_7(10);
    Motion_ArmCb_5(1, 0, 0);
    Battle_WaitMode0_8(20);
    Battle_WaitMode0_9(10);
    Motion_CallWaitAnim_1(8, 4);
    Battle_WaitMode0_10(10);
    BattleEv_RunWait_2(8, 0);
    Battle_WaitMode0_11(20);
    BattleFx_SpawnLinked_2(1, 0x102, 40);
    Battle_WaitMode0_12(30);
    Motion_ArmCb_6(1, 0x8000, 0);
    Battle_WaitMode0_13(50);
    BattleEv_RunWait_3(1, 0);
    Battle_WaitMode0_14(20);
    Motion_ArmCb_7(1, 0x2000, 0);
    Battle_WaitMode0_15(30);
    Motion_SetSpeed_4(1, 0x10000, 0x8000);
    Motion_SetPosReset_3(1, 0x108, 0x5b8);
    Motion_ArmCb_8(1, 0, 0);
    Motion_ArmCb_9(0, 0x8000, 0);
    Motion_ArmCb_10(8, 0x4000, 0);
    Battle_WaitMode0_16(30);
    BattleEv_RunWait_4(1, 0);
    Battle_WaitMode0_17(10);
    Motion_CallWaitAnim_2(0, 3);
    Battle_WaitMode0_18(30);
    Motion_CallWaitAnim_3(1, 3);
    Battle_WaitMode0_19(30);
    Motion_SetSpeed_5(1, 0x13333, 0x9999);
    Object_SetModeById_1(1, 2);
    /* If a record is returned, pass its s16 fields at +10 and +18 back in as
     * arguments. */
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        Motion_ResetAndSetPosition_1(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Motion_CommitPos_2(1);
    Motion_SetHPosTerrain_2(1, 0, 0);
    /* Same routine as above, called directly instead of through the Value
     * wrapper. */
    Battle_WaitMode0_20(20);
    /* Clear the flag byte at +91 of record 8. */
    *(u8 *)(Scene_GetRecord_3(8) + 91) = 0;
    Motion_EnableActCb_1(8, 2);
    record = Scene_GetRecord_4(8);
    /* Store the integer part of the 16.16 fixed-point fields at +8 and +16
     * into the halfwords at +100 and +102. */
    {
        s32 shown = *(s32 *)(record + 8) / 0x10000;

        *(u16 *)(record + 100) = shown;
    }
    {
        s32 shown = *(s32 *)(record + 16) / 0x10000;

        *(u16 *)(record + 102) = shown;
    }
    Battle_SchedShoulder_1();
}

void Scene_RunScene3aeSequenceB(void)
{
    u32 i;
    u8 *record;
    s32 none;
    s32 v5;
    s32 v6;

    Call1(Func_020023a4_b, 0x8ab);
    Func_020023d0();
    Func_020024dc();
    Call1(Func_02002472, 0x23eb);
    record = Func_02002400(11);
    none = 0;
    record[35] = none;
    *(u8 *)(*(volatile s32 *)((s32)record + 80) + 9) |= 12;
    *(u8 *)(*(volatile s32 *)((s32)record + 80) + 21) |= 12;
    Call4(Func_020024f4, 0xe80000, -1, 0x1300000, 1);
    Call3(Func_0200244a, 0, 0x10000, 0x8000);
    Call3(Func_02002476, 0, 216, 0x110);
    Call3(Func_020024fa, 0, 0x2000, 0);
    Func_02002526();
    Func_0200243c(20);
    Call3(Func_02002520, 13, 0x102, 50);
    Func_02002500(13, 0);
    Func_02002456(10);
    Call3(Func_02002538, 10, 0x107, 50);
    Func_02002532(10, 0, 0);
    Func_020024ec(10, 4, 13);
    Func_020024f6(10, 4, 30);
    Func_02002536(10, 0);
    Func_0200248c(10);
    Func_0200251c(11, 2);
    Func_0200249a(20);
    Func_02002552(11, 0);
    Func_020024a8(10);
    Func_02002520_a(13, 4);
    Func_020024b6(20);
    Call2((void (*)())Func_0200256e, 13, 0);
    Func_020024c4(10);
    Call3(Func_020025a6, 10, 0x103, 55);
    Call3(Func_0200250c, 10, 0x20000, 0x10000);
    Func_02002616(10, 16, 0);
    Func_02002568(10, 7, 0);
    v5 = 254;
    Func_0200262a(10, 24, 0);
    *(u8 *)(Func_02002528(10) + 90) &= v5;
    Call3(Func_02002640, 10, -16, 0);
    Func_02002656(153);
    Call3(Func_02002558, 13, 0x26666, 0x13333);
    Func_02002662(13, 16, 0);
    Func_02002538_a(10);
    v6 = 1;
    Func_020025a8(10, 1);
    *(u8 *)(Func_0200256e_a(10) + 90) |= v6;
    Call2(Func_0200263a, 13, 0x102);
    Func_020025e2(13, 2);
    Func_020026a0(155);
    Call1((void (*)())Func_0200256e_b, 10);
    Func_020026ac(155);
    Func_0200257a(10);
    Func_020026b8(155);
    Func_02002586(10);
    Func_0200258c(20);
    Call3(Func_020025c6, 13, 0x6666, 0x3333);
    Func_02002618(13, 6, 0);
    Func_020026de(159);
    Call3(Func_020026e2, 13, -8, 0);
    Func_020025b8(20);
    Func_02002670(10, 0);
    Func_020025c6_a(10);
    Call3(Func_020026aa, 13, 0x102, 70);
    Call3(Func_02002610, 16, 0x10000, 0x8000);
    Call3(Func_0200271c, 16, -8, 0);
    Call3(Func_020026c0, 16, 0x5000, 0);
    Func_020025fe(30);
    Func_02002676(16, 4);
    Func_0200260c(20);
    Func_020026c4(16, 0);
    Func_0200261a(10);
    Call3(Func_020026ee, 10, 0xe000, 0);
    Func_0200262c(35);
    Func_020026bc(10, 2);
    Func_0200263a_a(20);
    Func_020026f2(10, 0);
    Func_02002648(10);
    Call3(Func_0200271c_a, 16, 0x2000, 0);
    Func_0200265a(55);
    Call3(Func_0200272e, 16, 0x5000, 0);
    Func_0200266c(30);
    Func_02002724(16, 0);
    Func_0200267a(10);
    Call3(Func_0200274e, 11, 0xe000, 0);
    Func_0200268c(20);
    Call3(Func_02002770, 11, 0x102, 50);
    Func_02002750(11, 0);
    Func_020026a6(10);
    Func_02002736(13, 2);
    Func_020026b4(20);
    Func_0200276c(13, 0);
    Func_020026c2(10);
    Call3(Func_02002796, 13, 0xa000, 0);
    Func_020026d4(60);
    Call3(Func_020027a8, 13, 0x8000, 0);
    Func_020026e6(30);
    Func_0200279e(13, 0);
    Func_020026f4(10);
    Func_020027c6(10, 0, 0);
    Func_020027d0(11, 0, 0);
    Func_0200270e(20);
    Func_0200279e_a(10, 2);
    Func_0200271c_b(20);
    Func_020027d4(10, 0);
    Call3(Func_0200275e, 10, 0x13333, 0x9999);
    Func_02002868(10, 8, 0);
    Call3(Func_02002776, 16, 0x20000, 0x10000);
    Call3(Func_02002882, 16, -8, 16);
    Call3(Func_02002826, 16, 0x8000, 0);
    Func_02002816(16, 0);
    Func_0200276c_a(10);
    Call3(Func_02002850, 10, 0x102, 50);
    *(u8 *)(Func_020027a6(10) + 90) &= v5;
    Call3(Func_020027c2, 10, 0xcccc, 0x6666);
    Call3(Func_020028ce, 10, -8, 0);
    *(u8 *)(Func_020027cc(10) + 90) |= v6;
    Func_020027b2(20);
    Func_0200286e(10, 2);
    Func_020027ec(20);
    Func_020028a4(10, 0);
    Func_020027fa(10);
    Func_02002872(16, 4);
    Func_02002808(20);
    Func_020028c0(16, 0);
    Func_02002816_a(10);
    *(u8 *)(Func_02002844(10) + 90) &= v5;
    Call3(Func_02002860, 10, 0xcccc, 0x6666);
    Call3(Func_0200296c, 10, -16, 0);
    *(u8 *)(Func_0200286a(10) + 90) |= v6;
    Func_0200291c(10, 0, 0);
    Func_0200285a(20);
    Func_020028d2(10, 4);
    Func_02002868_a(20);
    Call3(Func_020028a2, 10, 0x1cccc, 0xe666);
    Func_020029ac(10, 8, 0);
    Func_020028fe(10, 6, 0);
    Func_020029c0(10, 24, 0);
    Func_020029ce(133);
    Func_02002918(16, 6, 0);
    Call2(Func_020028e0, 16, 0x20096e4);
    *(u8 *)(Func_020028d6(10) + 90) &= v5;
    Func_02002938(10, 6, 0);
    Call3(Func_020029fc, 10, -12, 4);
    record = Value1(Func_020028fa, 10);
    record[89] = none;
    record[35] = 2;
    *(u8 *)(*(volatile s32 *)((s32)record + 80) + 9) |= 12;
    *(u8 *)(*(volatile s32 *)((s32)record + 80) + 38) = none;
    {
        s32 target = *(volatile s32 *)((s32)record + 80);
        s32 shown = 0xc000;

        *(volatile u16 *)(target + 30) = shown;
    }
    Call3(Func_02002a3a, 10, -12, 4);
    Call3(Func_020029de, 10, 0x4000, 0);
    {
        u8 *record = Func_02002944(10);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | v6);
    }
    Func_02002a62(159);
    Func_02002930(20);
    Call3(Func_02002a14, 11, 0x102, 50);
    Call3(Func_02002978, 11, 0x18000, 0xc000);
    Func_02002a82(11, 24, 0);
    Func_02002a24(11, 0xc000, 0);
    Func_02002962(10);
    Func_02002a1a(11, 0);
    Func_02002970(10);
    Func_020029e8(16, 4);
    Func_0200297e(20);
    Func_02002a36(16, 0);
    Func_0200298c(20);
    Call3(Func_020029c6, 16, 0xcccc, 0x6666);
    Call3(Func_02002ad2, 16, -8, 0);
    Func_020029a8(20);
    Func_02002a60(16, 0);
    Func_020029b6(10);
    Func_02002a46(10, 2);
    Func_020029c4(20);
    Func_020029ca(10);
    Func_02002a9c(16, 0, 0);
    Func_020029da(40);
    Func_02002a52(13, 3);
    Func_020029e8_a(10);
    Func_02002a60_a(13, 3);
    Func_020029f6(20);
    Func_020029fc_a(10);
    Call3(Func_02002a3a_a, 16, 0x10000, 0x8000);
    Call3(Func_02002b46, 16, 24, -24);
    Func_02002b50(16, 8, 0);
    Call3(Func_02002af4, 16, 0xe000, 0);
    Func_02002a32(20);
    Call3(Func_02002a70, 13, 0x10000, 0x8000);
    Call3(Func_02002b7c, 13, 0, -8);
    Func_02002a52_a(10);
    Func_02002a66();
}
#undef Battle_Reset_1
#undef Motion_SetHPosTerrain_1
#undef Scene_GetRecord_1
#undef Battle_WaitMode0_1
#undef Motion_SetSpeed_1
#undef Battle_WaitMode0_2
#undef Motion_RealignToTrackedObjectAndArmCallback_1
#undef Motion_CommitPos_1
#undef Motion_ArmCb_1
#undef Battle_WaitMode0_3
#undef SceneWork_SetStepValue_1
#undef Motion_ArmCb_2
#undef Motion_SetSpeed_2
#undef Motion_SetPosReset_1
#undef Motion_ArmCb_3
#undef Motion_CamBounds_1
#undef Object_CommitPositionThenWaitIfModeZero_1
#undef Battle_WaitMode0_4
#undef Motion_Launch_1
#undef Motion_Launch_2
#undef BattleEv_RunWait_1
#undef Battle_WaitMode0_5
#undef Motion_CamBounds_2
#undef Object_CommitPositionThenWaitIfModeZero_2
#undef Battle_WaitMode0_6
#undef BattleFx_SpawnLinked_1
#undef Motion_SetSpeed_3
#undef Motion_SetPosReset_2
#undef Motion_ArmCb_4
#undef Battle_WaitMode0_7
#undef Motion_ArmCb_5
#undef Battle_WaitMode0_8
#undef Battle_WaitMode0_9
#undef Motion_CallWaitAnim_1
#undef Battle_WaitMode0_10
#undef BattleEv_RunWait_2
#undef Battle_WaitMode0_11
#undef BattleFx_SpawnLinked_2
#undef Battle_WaitMode0_12
#undef Motion_ArmCb_6
#undef Battle_WaitMode0_13
#undef BattleEv_RunWait_3
#undef Battle_WaitMode0_14
#undef Motion_ArmCb_7
#undef Battle_WaitMode0_15
#undef Motion_SetSpeed_4
#undef Motion_SetPosReset_3
#undef Motion_ArmCb_8
#undef Motion_ArmCb_9
#undef Motion_ArmCb_10
#undef Battle_WaitMode0_16
#undef BattleEv_RunWait_4
#undef Battle_WaitMode0_17
#undef Motion_CallWaitAnim_2
#undef Battle_WaitMode0_18
#undef Motion_CallWaitAnim_3
#undef Battle_WaitMode0_19
#undef Motion_SetSpeed_5
#undef Object_SetModeById_1
#undef Scene_GetRecord_2
#undef Motion_ResetAndSetPosition_1
#undef Motion_CommitPos_2
#undef Motion_SetHPosTerrain_2
#undef Battle_WaitMode0_20
#undef Scene_GetRecord_3
#undef Motion_EnableActCb_1
#undef Scene_GetRecord_4
#undef Battle_SchedShoulder_1
#undef FieldScene_RunScene3ae_02000144
#undef FieldScene_RunScene3ae_02000260
#undef FieldScene_RunScene3ae_020002dc
#undef Scene_RunOpeningAuxiliarySequence
#undef FieldScene_RunScene3ae_020006c8
#undef FieldScene_RunScene3ae_020007dc
#undef FieldScene_RunScene3ae_0200086c
#undef FieldScene_RunScene3ae_020008cc
#undef Scene_RunScene3aeSequenceA
#undef Scene_RunScene3aeSequenceB

/* overlays/scene/dialogue/flagged_message/scene_setup.c */
#define Scene_PlaceSlots14And15 Func_02000af8
s32 *Func_020020a4(s32);
s32 *Func_020020b4(s32);
void Func_02002032(s32, s32, s32, s32, s32, s32);
void Func_02002044(s32, s32, s32, s32, s32, s32);
void Func_02002056(s32, s32, s32, s32, s32, s32);
void Func_020016bc(s32);
void Func_020016c2(s32);

void Scene_PlaceSlots14And15(void)
{
    s32 pos14;
    s32 pos15;

    pos14 = Func_020020a4(14)[2] >> 20;   /* [r0,#8], asrs #20 */
    pos15 = Func_020020b4(15)[2] >> 20;

    Func_02002032(5, 12, 5, 1, 5, 11);
    Func_02002044(1, 0, 1, 1, pos15, 11);
    Func_02002056(1, 0, 1, 1, pos14, 11);

    Func_020016bc(14);
    Func_020016c2(15);
}
#undef Scene_PlaceSlots14And15

/* overlays/scene/dialogue/flagged_message/scene_state_interaction.c */
#define Scene_RunActorThirteenFlagDialogue Func_020004b8
void Func_02001a3c_a(void);
s32 Func_02001a12(s32);
void Func_02001ae4(s32);
void Func_02001af4_a(s32, s32);
s32 Func_02001a2c(s32);
void Func_02001afe(s32);
void Func_02001b16(s32, s32);
void Func_02001b0e(s32);
void Func_02001b26(s32, s32);
void Func_02001a8a(void);

void Scene_RunActorThirteenFlagDialogue(void)
{
    Func_02001a3c_a();

    if (Func_02001a12(0x8A7) != 0) {
        Func_02001ae4(0x1D1F);
        Func_02001af4_a(13, 0);
    } else if (Func_02001a2c(0x8A5) != 0) {
        Func_02001afe(0x1D1B);
        Func_02001b16(13, 0);
    } else {
        Func_02001b0e(0x1D19);
        Func_02001b26(13, 0);
    }

    Func_02001a8a();
}
#undef Scene_RunActorThirteenFlagDialogue

/* overlays/scene/dialogue/flagged_message/shared.c */
s32 Func_02000094(void)
{
    return 0;
}

/* overlays/scene/dialogue/flagged_message/state_update.c */
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define State_ApplyValues14And0And5 Func_02000030
#define State_SetRuntimeWord448To521AndSend303 Func_02000ad4

extern u8 *Data_03001ebc;

void Func_020016ca(s32, s32, s32);
void Func_02002048(s32);

void State_ApplyValues14And0And5(void)
{
    Func_020016ca(0xE, 0, 5);
}

void State_SetRuntimeWord448To521AndSend303(void)
{
    u8 *state = Data_03001ebc;
    s32 *slot = (s32 *)(state + 0x1C0);

    *slot = 0x209;
    Func_02002048(0x12F);
}
#undef NULL
#undef FIELD_AT_OFFSET
#undef State_ApplyValues14And0And5
#undef State_SetRuntimeWord448To521AndSend303

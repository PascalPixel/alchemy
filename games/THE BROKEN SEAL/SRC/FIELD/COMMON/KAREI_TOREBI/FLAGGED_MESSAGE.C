#include "TYPES.H"
#include "FIELD_EVENT.H"

#define NULL ((void *)0)

extern u8 *Data_03001ebc;

void Func_020016ca(s32, s32, s32);

#include "TYPES.H"

#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_0000006b
#define SecondaryRuntimeSelector Value_00000070
#define TertiaryRuntimeSelector Value_0000006c
#define PrimaryOverlayData Data_02009738
#define SecondaryOverlayData Data_020097c8
#define TertiaryOverlayData Data_02009840
#define DefaultOverlayData Data_02009708

#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"

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

/*
 * Table getter for resource_3ae. The owner at 0x02000098 is eight bytes and
 * includes its one pool word at 0x0200009c: the pc-relative load reads that
 * word, so the word is part of the owner and not a following gap.
 *
 * The pool word is an address that is returned without being dereferenced.
 * Many getters share this body but each returns a different address, so the
 * constant here is not interchangeable with theirs.
 */

/*
 * The selector is the signed halfword at byte offset 448 of the resident
 * workspace, Data_02000240[224].  It is compared as a signed 32-bit value
 * against full words held in the pool, which is why the three constants are
 * spelled as the addresses of Value_ symbols rather than as immediates.
 * 0x93e and 0x950 are read as event-flag ids only from their use as the sole
 * argument of the flag test.
 */

/*
 * Pick one of eight in-image data tables from the scene selector and two
 * event flags. The selector is the signed halfword at offset 448 of the
 * shared workspace, and each arm compares it as a signed 32-bit value
 * against a full word, which is why the selector constants are spelled as
 * addresses. 0x93e and 0x950 are taken to be event-flag ids only from their
 * use here. The 164-byte owner includes its thirteen-word literal pool.
 */

#include "TYPES.H"

#include "TYPES.H"

#define Scene_GetRecord_1(args...) Func_0200215c(args)
#define ObjectMotion_RealignToTrackedObjectAndArmCallback_1(a0, a1, a2, a3) Call4(Func_02002278, a0, a1, a2, a3)
#define Scene_GetRecord_2(a0) Value1(Func_02002358, a0)
#define BattleRuntime_WaitIfModeZero_20(args...) ((void (*)())Engine_EventWait)(args)
#define Scene_GetRecord_3(args...) Func_02002386(args)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define Scene_GetRecord_4(a0) Value1(Func_0200239a, a0)

extern u8 Data_0000006b[];
extern u8 Data_00000070[];
extern u8 Data_0000006c[];
extern s16 Data_02000240_t[][1];

void Func_02001882();
s32 Func_02001ab4();
void Func_02001abc();
void Func_02001ad2();
void Func_02001aec();
void Func_02001af2();
void Func_02001b10();
void Func_02001b44();
void Func_02001bc6();
void Func_020011fc();
s32 Func_0200215c();
void Func_02002230();
void Func_02002278();
s32 Func_02002358();
s32 Func_02002386();
s32 Func_0200239a();
u8 *Func_02002400();
void Func_020024dc();
s32 Func_02002528();
s32 Func_0200256e_a();
s32 Func_020027a6();
s32 Func_020027cc();
s32 Func_02002844();
s32 Func_0200286a();
s32 Func_020028d6();
s32 Func_020028fa();
u8 *Func_02002944();

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
    u8 *work = Data_03001ebc;

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

/* Runs a fixed sequence of scene calls (positioning, timing, and animation
 * triggers) for scene 3ae, sequence A. */

#include "TYPES.H"

extern u8 Data_00001d20[];

u8 *Func_020018ce();
s32 Func_0200194c();
s32 Func_0200194c_next();
void Func_020019be();
u8 *Func_020019fa();
void Func_02001abc_scene_dialogue();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Contiguous unnamed leaf-owner run for resource_3ae. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* The scene step counter at 0x1d8 of the shared scene work record. */

#include "TYPES.H"

void Func_02001a3c_scene_state_interaction(void);
void Func_02001af4_scene_state_interaction(s32, s32);

#include "TYPES.H"

s32 *Func_020020a4(s32);
s32 *Func_020020b4(s32);

#include "TYPES.H"

u8 *Func_0200210e(s32);
u8 *Func_02002116(s32);

void SceneState_ApplyValues14And0And5(void)
{
    Func_020016ca(0xE, 0, 5);
}

s32 SceneData_SelectByRuntimeSelector(void)
{
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR_BODY.INC"
}

s32 Func_02000094(void)
{
    return 0;
}

u8 *SceneData_GetTable98a0(void)
{
    return (u8 *)0x020098a0;
}

s32 SceneData_SelectDataBySelectorAndFlags(void)
{
    s16 room = gGameState.scene;

    if (room == (s32)&Value_0000006b) {
        if (GameFlag_IsSet(0x93e) != 0) {
            return (s32)Data_02009ba4;
        }
        return (s32)Data_02009acc;
    }

    if (room == (s32)&Value_00000070) {
        /* 0x950 is built from an immediate and a shift. */
        if (GameFlag_IsSet(0x950) != 0) {
            return (s32)Data_020099c4;
        }
        return (s32)Data_020098ec;
    }

    if (room == (s32)&Value_0000006c) {
        if (GameFlag_IsSet(0x950) != 0) {
            return (s32)Data_02009dcc;
        }
        if (GameFlag_IsSet(0x93e) != 0) {
            return (s32)Data_02009d24;
        }
        return (s32)Data_02009c7c;
    }

    return (s32)Data_020098d4;
}

void FieldScene_RunScene3ae_02000144(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    GameFlag_Set(0x8aa);
    Actor_WalkToAndWait(0, 0x188, 0x128);
    Actor_SetSpeed(8, 0x13333, 0x9999);
    Actor_WalkToAndWait(8, 0x198, 0x128);
    Actor_FaceDirection(8, 0x8000, 0);
    Event_Wait(20);
    Event_End();
}

s32 SceneData_SelectSecondaryDataBySelectorAndFlags(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&Value_0000006b) {
        if (GameFlag_IsSet(0x93e) != 0) {
            return (s32)Data_02009fa0;
        }
        return (s32)Data_02009e80;
    }

    if (scene == (s32)&Value_00000070) {
        /* 0x950 is built by shifting a small constant, not loaded. */
        if (GameFlag_IsSet(0x950) != 0) {
            return (s32)Data_0200a120;
        }
        return (s32)Data_0200a018;
    }

    if (scene == (s32)&Value_0000006c) {
        if (GameFlag_IsSet(0x950) != 0) {
            return (s32)Data_0200a390;
        }
        if (GameFlag_IsSet(0x93e) != 0) {
            return (s32)Data_0200a30c;
        }
        return (s32)Data_0200a24c;
    }

    return (s32)Data_02009e74;
}

void SceneDialogue_ShowLine1CF8(void)
{
    Event_Begin();
    Event_SetMessage(0x1CF8);
    Event_AskYesNo(8, 0);
    Event_End();
}

void FieldScene_RunScene3ae_02000260(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x8a6) == 0) {
        Event_SetMessage(0x1cfd);
        Event_OpenMessage(11, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_ShowMessage(11, 0);
            GameFlag_Set(0x8a6);
            goto L_020002c2;
        }
        bump_step(1);
        Event_ShowMessage(11, 0);
    } else {
        Event_SetMessage(0x1cfe);
        Event_ShowMessage(11, 0);
    }
    L_020002c2:;
    Event_End();
}

void FieldScene_RunScene3ae_020002dc(void)
{
    u32 i;
    s32 record;

    Func_02001882(0);
    Event_Begin();
    if (GameFlag_IsSet(0x8a7) != 0) {
        if (GameFlag_IsSet(0x8a9) != 0) {
            Event_SetMessage(0x1d23);
            Event_OpenMessage(12, 0);
            Actor_FaceDirection(12, 0x4000, 0);
        }
    }
}

void SceneDialogue_RunActor12Event(void)
{
    s32 rec7;
    s32 rec8;
    u8 *record;
    s16 angle;

    record = Func_020018ce(0);
    angle = (u16)((*(u16 *)(record + 6) + 0x2000) & ~0x3fff);
    Event_Begin();
    if (GameFlag_IsSet(0x8a7) != 0) {
        if (GameFlag_IsSet(0x8a9) != 0) {
            Event_SetMessage(0x1d23);
            Event_OpenMessage(12, 0);
            goto L_02000496;
        }
        Event_SetMessage((s32)Data_00001d20);
        Event_OpenMessage(12, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_Wait(10);
            Event_SetMessage(((s32)Data_00001d20 + 1));
            Event_ShowMessage(12, 0);
            Actor_WalkToAndWait(12, 88, 0x508);
            Actor_FaceDirection(12, 0x4000, 0);
            Event_Wait(20);
            GameFlag_Set(0x8a9);
            goto L_02000496;
        }
        Event_SetMessage(((s32)Data_00001d20 + 2));
        Event_ShowMessage(12, 0);
    } else {
        if ((u16)angle != 0x8000) {
            goto L_0200049a;
        }
        Event_SetMessage(0x1d16);
        Event_ShowMessage(12, 0);
        if (GameFlag_IsSet(0x8a5) != 0) {
            rec7 = Func_0200194c(235);
            rec8 = Func_0200194c_next(rec7, 235);
            Actor_SetAnimationAndWait(12, 3);
            Actor_WalkToAndWait(12, 88, 0x508);
            Actor_FaceDirection(12, 0x4000, 0);
            bump_step(1);
            Event_ShowMessage(12, 0);
            Func_020019be(rec7, rec8);
            GameFlag_Set(0x8a7);
            record = Func_020019fa(0);
            Actor_WalkToAndWait(0, *(s16 *)(record + 10), 0x518);
            Actor_WalkToAndWait(0, 72, 0x518);
            Actor_WalkToAndWait(12, 88, 0x518);
            Actor_FaceDirection(12, 0, 0);
        } else {
            Func_02001abc_scene_dialogue(12, 0);
        }
    }
    L_02000496:;
    Event_End();
    L_0200049a:;
}

void FieldScene_RunActorThirteenFlagDialogue(void)
{
    Func_02001a3c_scene_state_interaction();

    if (GameFlag_IsSet(0x8A7) != 0) {
        Event_SetMessage(0x1D1F);
        Func_02001af4_scene_state_interaction(13, 0);
    } else if (GameFlag_IsSet(0x8A5) != 0) {
        Event_SetMessage(0x1D1B);
        Event_ShowMessage(13, 0);
    } else {
        Event_SetMessage(0x1D19);
        Event_ShowMessage(13, 0);
    }

    Event_End();
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 base6_2000240;
    s32 six00;

    six00 = 0x258;
    Event_Begin();
    if (GameFlag_IsSet(0x8a5) != 0) {
        Event_SetMessage(0x1d0b);
        Event_ShowMessage(8, 0);
    } else {
        Event_SetMessage(0x1d04);
        Event_OpenMessage(8, 0);
        if (Event_ChooseYesNo(0, 0) == 1) {
            Event_ShowMessageAndWait(8, 0, 10);
        } else {
            bump_step(1);
            Func_02001abc(six00, 5);
            Event_OpenMessage(8, 0);
            rec7 = Func_02001ab4(19, 8, 11, 4, 2);
            Call4(Func_02001ad2, 0xc8a, rec7, 0, 0);
            base6_2000240 = (s32)Data_02000240;
            Func_02001aec(*(s32 *)(base6_2000240 + 16), 6, rec7, 24, 8);
            if (Event_ChooseYesNo(-1, 0) == 1) {
                Func_02001af2(rec7, 2);
                Actor_SetAnimationAndWait(0, 4);
                Event_Wait(10);
                Event_ShowMessage(8, 0);
                goto L_02000660;
            } else {
                if ((u32)six00 <= (u32)*(s32 *)(base6_2000240 + 16)) {
                    goto L_0200061e;
                }
                Func_02001b10(rec7, 2);
                Actor_SetAnimationAndWait(0, 3);
                Event_Wait(10);
                bump_step(1);
                Audio_PlayCue(113);
                Event_ShowMessage(8, 0);
                goto L_02000660;
            }
            L_0200061e:;
            Func_02001b44(rec7, 2);
            Actor_SetAnimationAndWait(0, 3);
            Event_Wait(10);
            bump_step(3);
            Event_ShowMessage(8, 0);
            Party_GiveItem(235, 0);
            GameFlag_Set(0x8a5);
            Call1(Func_02001bc6, -six00);
        }
        L_02000660:;
        Event_End();
    }
}

void SceneDialogue_RunActor8Message1f09(void)
{
    Event_Begin();
    Event_SetMessage(0x1F09);
    Event_AskYesNo(8, 0);
    Event_End();
}

void SceneDialogue_RunActor10Message1f15(void)
{
    Event_Begin();
    Event_SetMessage(0x1F15);
    Event_AskYesNo(10, 0);
    Event_End();
}

void FieldScene_RunScene3ae_020006c8(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x8a8) != 0) {
        Actor_FaceActor(11, 0, 0);
        Event_Wait(20);
        Event_SetMessage(0x1f1c);
        Event_ShowMessage(11, 0);
        ((void (*)())Engine_EventEnd)();
    } else {
        Event_Wait(20);
        Actor_ShowEmote(11, 0x100, 50);
        Actor_FaceActor(11, 0, 0);
        Event_Wait(20);
        Event_SetMessage(0x1f18);
        Event_ShowMessage(11, 0);
        if (GameFlag_IsSet(0x8a6) != 0) {
            Event_Wait(20);
            Actor_ShowEmote(11, 0x102, 40);
            Event_OpenMessage(11, 0);
            if (Event_ChooseYesNo(0, 0) == 0) {
                Event_Wait(20);
                Event_ShowMessage(11, 0);
                GameFlag_Set(0x8a8);
                goto L_020007be;
            }
            ((void (*)())Engine_EventWait)(10);
            bump_step(1);
            Event_ShowMessage(11, 0);
            Event_Wait(10);
            Actor_FaceDirection(11, 0, 0);
            Event_Wait(30);
        } else {
            Event_Wait(10);
            Actor_FaceDirection(11, 0, 0);
            Event_Wait(30);
        }
        L_020007be:;
        Event_End();
    }
}

void FieldScene_RunScene3ae_020007dc(void)
{
    u8 *work;

    Event_Begin();
    Audio_PlayCue(158);
    Actor_SetSpeed(0, 0x8000, 0x4000);
    Actor_SetSpritePriority(0, 3);
    work = (u8 *)Data_02000240;
    if (*(s16 *)(work + 0x1c0) == (s32)Data_0000006b) {
        Actor_WalkTo(0, 0x130, 0x570);
        Map_AnimateCells(0x20096b8, 78, 86);
    } else {
        if (*(s16 *)(work + 0x1c0) == (s32)Data_00000070) {
            Actor_WalkTo(0, 248, 192);
            Map_AnimateCells(0x20096ce, 74, 9);
        }
    }
    Event_Wait(16);
    Event_RequestExit(3);
    Event_End();
}

s32 FieldScene_RunScene3ae_0200086c(void)
{
    u32 i;
    s32 record;

    if (Data_02000240_t[225][0] == 90) {
        GameFlag_Set(0x950);
    }
    if (Data_02000240_t[224][0] == (s32)Data_0000006b) {
        FieldScene_RunScene3ae_020008cc();
    } else {
        if (Data_02000240_t[224][0] == (s32)Data_00000070) {
            Func_020011fc();
        } else {
            if (Data_02000240_t[224][0] == (s32)Data_0000006c) {
                SceneState_SetRuntimeWord448To521AndSend303();
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
        if (GameFlag_IsSet(0x8ac) == 0) {
            GameFlag_Set(0x8ac);
            FieldScene_RunScene3aeSequenceA();
        }
    }
    if (Data_02000240_t[225][0] == 2) {
        if (GameFlag_IsSet(0x109) == 0) {
            GameFlag_Clear(0x8a9);
        }
    }
    if (GameFlag_IsSet(0x911) != 0) {
        if (GameFlag_IsSet(0x8a9) == 0) {
            Actor_SetPosition(12, 0x580000, 0x5180000);
            Actor_FaceDirection(12, 0, 0);
        }
    }
}

void SceneState_SetRuntimeWord448To521AndSend303(void)
{
    u8 *state = Data_03001ebc;
    s32 *slot = (s32 *)(state + 0x1C0);

    *slot = 0x209;
    GameFlag_Clear(0x12F);
}

void FieldScene_PlaceSlots14And15(void)
{
    s32 pos14;
    s32 pos15;

    pos14 = Func_020020a4(14)[2] >> 20;   /* [r0,#8], asrs #20 */
    pos15 = Func_020020b4(15)[2] >> 20;

    Map_CopyCellAttributes(5, 12, 5, 1, 5, 11);
    Map_CopyCellAttributes(1, 0, 1, 1, pos15, 11);
    Map_CopyCellAttributes(1, 0, 1, 1, pos14, 11);

    SceneActor_SetMode3AndFlagBit1(14);
    SceneActor_SetMode3AndFlagBit1(15);
}

void SceneActor_SetMode3AndFlagBit1(s32 no)
{
    u8 *p = Func_0200210e(no);
    u8 *flag;

    Actor_SetSpriteFlags(Func_02002116(no), 0);
    Actor_SetSpritePriority(no, 3);
    flag = p + 85;
    *flag = 0;
    p += 35;
    {
        u8 bit = 2;

        *p = bit | *p;
    }
}

void FieldScene_RunScene3aeSequenceA(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Func_02002230();
    Actor_SetPosition(8, 0x1480000, 0x5900000);
    /* Set the flag byte at +91 of record 8. */
    *(u8 *)(Scene_GetRecord_1(8) + 91) = 1;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    ObjectMotion_RealignToTrackedObjectAndArmCallback_1(1, -16, 0, 0x8000);
    Actor_WaitForMove(1);
    Actor_FaceDirection(0, 0xa000, 0);
    Event_Wait(20);
    Event_SetMessage(0x1f89);
    Actor_FaceDirection(0, 0xa000, 0);
    Actor_SetSpeed(1, 0x19999, 0xcccc);
    Actor_WalkToAndWait(1, 232, 0x590);
    Actor_FaceDirection(1, 0x8000, 0);
    Camera_MoveTo(0xb80000, -1, 0x5a00000, 1);
    Camera_WaitForMove();
    Event_Wait(10);
    Actor_Jump(1, 6, 15);
    Actor_Jump(1, 6, 40);
    Event_ShowMessage(1, 0);
    Event_Wait(20);
    Camera_MoveTo(0x1080000, -1, 0x5a80000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_ShowEmote(8, 0x100, 50);
    Actor_SetSpeed(8, 0x13333, 0x9999);
    Actor_WalkToAndWait(8, 0x108, 0x590);
    Actor_FaceDirection(8, 0x8000, 0);
    Event_Wait(10);
    Actor_FaceDirection(1, 0, 0);
    Event_Wait(20);
    Event_Wait(10);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(10);
    Event_ShowMessage(8, 0);
    Event_Wait(20);
    Actor_ShowEmote(1, 0x102, 40);
    Event_Wait(30);
    Actor_FaceDirection(1, 0x8000, 0);
    Event_Wait(50);
    Event_ShowMessage(1, 0);
    Event_Wait(20);
    Actor_FaceDirection(1, 0x2000, 0);
    Event_Wait(30);
    Actor_SetSpeed(1, 0x10000, 0x8000);
    Actor_WalkToAndWait(1, 0x108, 0x5b8);
    Actor_FaceDirection(1, 0, 0);
    Actor_FaceDirection(0, 0x8000, 0);
    Actor_FaceDirection(8, 0x4000, 0);
    Event_Wait(30);
    Event_ShowMessage(1, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(0, 3);
    Event_Wait(30);
    Actor_SetAnimationAndWait(1, 3);
    Event_Wait(30);
    Actor_SetSpeed(1, 0x13333, 0x9999);
    Actor_SetAnimation(1, 2);
    /* If a record is returned, pass its s16 fields at +10 and +18 back in as
     * arguments. */
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        Actor_SetDestination(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(1);
    Actor_SetPosition(1, 0, 0);
    /* Same routine as above, called directly instead of through the Value
     * wrapper. */
    BattleRuntime_WaitIfModeZero_20(20);
    /* Clear the flag byte at +91 of record 8. */
    *(u8 *)(Scene_GetRecord_3(8) + 91) = 0;
    ObjectMotion_EnableActionAndSetCallback_1(8, 2);
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
    Event_End();
}

void FieldScene_RunScene3aeSequenceB(void)
{
    u32 i;
    u8 *record;
    s32 none;
    s32 v5;
    s32 v6;

    GameFlag_Set(0x8ab);
    Event_Begin();
    Func_020024dc();
    Event_SetMessage(0x23eb);
    record = Func_02002400(11);
    none = 0;
    record[35] = none;
    *(u8 *)(*(s32 *)((s32)record + 80) + 9) |= 12;
    *(u8 *)(*(s32 *)((s32)record + 80) + 21) |= 12;
    Camera_MoveTo(0xe80000, -1, 0x1300000, 1);
    Actor_SetSpeed(0, 0x10000, 0x8000);
    Actor_WalkToAndWait(0, 216, 0x110);
    Actor_FaceDirection(0, 0x2000, 0);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_ShowEmote(13, 0x102, 50);
    Event_ShowMessage(13, 0);
    Event_Wait(10);
    Actor_ShowEmote(10, 0x107, 50);
    Actor_FaceDirection(10, 0, 0);
    Actor_Jump(10, 4, 13);
    Actor_Jump(10, 4, 30);
    Event_ShowMessage(10, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(11, 2);
    Event_Wait(20);
    Event_ShowMessage(11, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(13, 4);
    Event_Wait(20);
    Call2((void (*)())Engine_EventShowMessage, 13, 0);
    Event_Wait(10);
    Actor_ShowEmote(10, 0x103, 55);
    Actor_SetSpeed(10, 0x20000, 0x10000);
    Actor_WalkByAndWait(10, 16, 0);
    Actor_Jump(10, 7, 0);
    v5 = 254;
    Actor_WalkByAndWait(10, 24, 0);
    *(u8 *)(Func_02002528(10) + 90) &= v5;
    Actor_WalkBy(10, -16, 0);
    Audio_PlayCue(153);
    Actor_SetSpeed(13, 0x26666, 0x13333);
    Actor_WalkByAndWait(13, 16, 0);
    Event_Wait(10);
    v6 = 1;
    Actor_SetAnimation(10, 1);
    *(u8 *)(Func_0200256e_a(10) + 90) |= v6;
    Actor_SetAttachedEffect(13, 0x102);
    Actor_StartRepeatedMotion(13, 2);
    Audio_PlayCue(155);
    Call1((void (*)())Engine_EventWait, 10);
    Audio_PlayCue(155);
    Event_Wait(10);
    Audio_PlayCue(155);
    Event_Wait(10);
    Event_Wait(20);
    Actor_SetSpeed(13, 0x6666, 0x3333);
    Actor_Jump(13, 6, 0);
    Audio_PlayCue(159);
    Actor_WalkByAndWait(13, -8, 0);
    Event_Wait(20);
    Event_ShowMessage(10, 0);
    Event_Wait(10);
    Actor_ShowEmote(13, 0x102, 70);
    Actor_SetSpeed(16, 0x10000, 0x8000);
    Actor_WalkByAndWait(16, -8, 0);
    Actor_FaceDirection(16, 0x5000, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(16, 4);
    Event_Wait(20);
    Event_ShowMessage(16, 0);
    Event_Wait(10);
    Actor_FaceDirection(10, 0xe000, 0);
    Event_Wait(35);
    Actor_RunRepeatedMotion(10, 2);
    Event_Wait(20);
    Event_ShowMessage(10, 0);
    Event_Wait(10);
    Actor_FaceDirection(16, 0x2000, 0);
    Event_Wait(55);
    Actor_FaceDirection(16, 0x5000, 0);
    Event_Wait(30);
    Event_ShowMessage(16, 0);
    Event_Wait(10);
    Actor_FaceDirection(11, 0xe000, 0);
    Event_Wait(20);
    Actor_ShowEmote(11, 0x102, 50);
    Event_ShowMessage(11, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(13, 2);
    Event_Wait(20);
    Event_ShowMessage(13, 0);
    Event_Wait(10);
    Actor_FaceDirection(13, 0xa000, 0);
    Event_Wait(60);
    Actor_FaceDirection(13, 0x8000, 0);
    Event_Wait(30);
    Event_ShowMessage(13, 0);
    Event_Wait(10);
    Actor_FaceDirection(10, 0, 0);
    Actor_FaceDirection(11, 0, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(10, 2);
    Event_Wait(20);
    Event_ShowMessage(10, 0);
    Actor_SetSpeed(10, 0x13333, 0x9999);
    Actor_WalkByAndWait(10, 8, 0);
    Actor_SetSpeed(16, 0x20000, 0x10000);
    Actor_WalkByAndWait(16, -8, 16);
    Actor_FaceDirection(16, 0x8000, 0);
    Event_ShowMessage(16, 0);
    Event_Wait(10);
    Actor_ShowEmote(10, 0x102, 50);
    *(u8 *)(Func_020027a6(10) + 90) &= v5;
    Actor_SetSpeed(10, 0xcccc, 0x6666);
    Actor_WalkByAndWait(10, -8, 0);
    *(u8 *)(Func_020027cc(10) + 90) |= v6;
    Event_Wait(20);
    Actor_RunRepeatedMotion(10, 2);
    Event_Wait(20);
    Event_ShowMessage(10, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(16, 4);
    Event_Wait(20);
    Event_ShowMessage(16, 0);
    Event_Wait(10);
    *(u8 *)(Func_02002844(10) + 90) &= v5;
    Actor_SetSpeed(10, 0xcccc, 0x6666);
    Actor_WalkByAndWait(10, -16, 0);
    *(u8 *)(Func_0200286a(10) + 90) |= v6;
    Actor_FaceDirection(10, 0, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(10, 4);
    Event_Wait(20);
    Actor_SetSpeed(10, 0x1cccc, 0xe666);
    Actor_WalkByAndWait(10, 8, 0);
    Actor_Jump(10, 6, 0);
    Actor_WalkByAndWait(10, 24, 0);
    Audio_PlayCue(133);
    Actor_Jump(16, 6, 0);
    Actor_EnableActionCallback(16, 0x20096e4);
    *(u8 *)(Func_020028d6(10) + 90) &= v5;
    Actor_Jump(10, 6, 0);
    Actor_WalkByAndWait(10, -12, 4);
    record = Value1(Func_020028fa, 10);
    record[89] = none;
    record[35] = 2;
    *(u8 *)(*(s32 *)((s32)record + 80) + 9) |= 12;
    *(u8 *)(*(s32 *)((s32)record + 80) + 38) = none;
    {
        s32 target = *(s32 *)((s32)record + 80);
        s32 shown = 0xc000;

        *(u16 *)(target + 30) = shown;
    }
    Actor_WalkByAndWait(10, -12, 4);
    Actor_FaceDirection(10, 0x4000, 0);
    {
        u8 *flags = Func_02002944(10) + 90;
        u8 value = *flags | v6;

        *flags = value;
    }
    Audio_PlayCue(159);
    Event_Wait(20);
    Actor_ShowEmote(11, 0x102, 50);
    Actor_SetSpeed(11, 0x18000, 0xc000);
    Actor_WalkByAndWait(11, 24, 0);
    Actor_FaceDirection(11, 0xc000, 0);
    Event_Wait(10);
    Event_ShowMessage(11, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(16, 4);
    Event_Wait(20);
    Event_ShowMessage(16, 0);
    Event_Wait(20);
    Actor_SetSpeed(16, 0xcccc, 0x6666);
    Actor_WalkByAndWait(16, -8, 0);
    Event_Wait(20);
    Event_ShowMessage(16, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(10, 2);
    Event_Wait(20);
    Event_Wait(10);
    Actor_FaceDirection(16, 0, 0);
    Event_Wait(40);
    Actor_SetAnimationAndWait(13, 3);
    Event_Wait(10);
    Actor_SetAnimationAndWait(13, 3);
    Event_Wait(20);
    Event_Wait(10);
    Actor_SetSpeed(16, 0x10000, 0x8000);
    Actor_WalkByAndWait(16, 24, -24);
    Actor_WalkByAndWait(16, 8, 0);
    Actor_FaceDirection(16, 0xe000, 0);
    Event_Wait(20);
    Actor_SetSpeed(13, 0x10000, 0x8000);
    Actor_WalkByAndWait(13, 0, -8);
    Event_Wait(10);
    Event_End();
}

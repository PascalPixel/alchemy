#include "TYPES.H"
#include "FIELD_EVENT.H"

#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_00000075
#define SecondaryRuntimeSelector Value_00000076
#define TertiaryRuntimeSelector Value_00000078
#define PrimaryOverlayData Data_0200a898
#define SecondaryOverlayData Data_0200a8e0
#define TertiaryOverlayData Data_0200a928
#define DefaultOverlayData Data_0200a868
#define PrimaryOverlayData_02000a50 Data_0200a9b0
#define SecondaryOverlayData_02000a50 Data_0200aa40
#define TertiaryOverlayData_02000a50 Data_0200aad0
#define DefaultOverlayData_02000a50 Data_0200a998
#define PrimaryOverlayData_02000ee0 Data_0200abb4
#define SecondaryOverlayData_02000ee0 Data_0200acb0
#define TertiaryOverlayData_02000ee0 Data_0200adac
#define DefaultOverlayData_02000ee0 Data_0200aba8

#include "STAGED_ACTOR.H"
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct Frame {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct Slot {
    u16 f00;
    u16 f02;
    u16 f04;
    u16 f06;
};

extern s32 Data_0200af74[];

void Func_02002fd6(s32 actor, s32 mode, s32 frames);
u8 *Func_02003fa2(s32);
s32 Func_02002f42();
void Func_02003156_a();
void Func_020031b0();
s32 Func_02003236();
s32 Func_02003588();
void Func_020035ac();
void Func_02004044();
void Func_02003168_a();
void Func_02003178();
void Func_02003188();
void Func_02003198();
void Func_0200425a();
void Func_02004262();
void Func_0200426a();
void Func_02004272();
void Func_0200427a();
void Func_0200475e_a();
s32 Func_02004768();
s32 Func_02004772();
s32 Func_020047d6();
s32 Func_020047e2();
s32 Func_020047ec();
s32 Func_020047f6();
void Func_02003e3c();
void Func_02004074();
void Func_020047ba();
s32 Func_020047c4();
s32 Func_0200487a();
void Func_020048c4();
s32 Func_020048d2();
s32 Func_020048dc();
s32 Func_020033bc();
s32 Func_020033c4();
s32 Func_020033ce();
s32 Func_020033d6();
u8 *Func_02003ff8(s32);
u8 *Func_020041fc(s32);
void Func_02003a68(void);
s32 Func_020035d8(s32);
void Func_02002038(s32);
u8 *Func_02003eec(s32);
u8 *Func_02003f78(s32);
u8 *Func_02004228(s32);
void Func_02003ef2();
void Func_02003ef8_a();
u8 *Func_02003fcc(s32);
void Func_02004174();
void Func_0200417a();
void Func_02003680();
void Func_02004322(s32, s32, s32);

/*
 * Distance between two three-component 16.16 fixed-point positions.
 *
 * Each argument walks three consecutive 16.16 words in x, y, z order. The
 * per-axis deltas are taken in fixed point, shifted down to integers, squared,
 * and summed; the total is passed to the resident IWRAM integer square root.
 *
 * Expressions are preserved exactly as reconstructed: the walking-pointer form
 * is load-bearing for byte-identity and must not become struct field access.
 */

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Actor_SetPosition(actor, x, y);
}

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02000fdc(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_020015f0(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02002188(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02002290(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02002334(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value1_02000e50(s32 (*f)(), s32 a0)
{

    return f(a0);
}

/* resource_3b4 actor presentation: cell repaints for slots 11 and 12. */

/*
 * Func_ names below are loader-relocated call words in this overlay's import
 * veneer table, not runtime addresses.  The declarations are old-style
 * because the same imports are reached with differing argument counts from
 * different call sites.
 */

/*
 * Actor presentation for resource_3b4.
 *
 * A Func_ name in the import veneer band 0x02002468-0x0200261f names the
 * main-image address held in the veneer's trailing word, not a runtime
 * address the call reaches directly.  Declarations are old-style because
 * those imports are reached with differing argument counts from different
 * call sites.
 */

/* Slot record lookup, then the mode imports. */

/* Complete 16-byte actor-15 mode wrapper before the no-op leaf at 0x9ec. */
void SceneActor_SetActor15ModeZero(void)
{
    Func_02002fd6(15, 0, 6);
}

/* Complete four-byte no-op leaf plus its alignment halfword. */
void Resource3b4_EmptyHookA(void)
{
}

/* Contiguous unnamed leaf-owner run for resource_3b4. */
s32 SceneData_SelectTableA898ByRuntimeSelector(void)
{
    s16 selector = RuntimeSelectorTable[224];

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)PrimaryOverlayData;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)SecondaryOverlayData;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)TertiaryOverlayData;
    }
    return (s32)DefaultOverlayData;
}

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* Complete eight-byte literal-address getter, including its sole pool word. */
u8 *SceneData_GetTablea970(void)
{
    return (u8 *)0x0200a970;
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    extern u8 PrimaryOverlayData_02000a50[];
    extern u8 SecondaryOverlayData_02000a50[];
    extern u8 TertiaryOverlayData_02000a50[];
    extern u8 DefaultOverlayData_02000a50[];

    s16 selector = RuntimeSelectorTable[224];

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)PrimaryOverlayData_02000a50;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)SecondaryOverlayData_02000a50;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)TertiaryOverlayData_02000a50;
    }
    return (s32)DefaultOverlayData_02000a50;
}

/* Complete two-byte empty hook plus its alignment halfword. */
void Resource3b4_EmptyHookB(void)
{
}

/*
 * Complete 40-byte heading update: face the supplied entity towards entity 0,
 * store the resulting angle in its +6 halfword and report zero.
 */
s32 SceneActor_FaceTowardActorZero(u8 *obj)
{
    u8 *p = Engine_GetTriggerActor(0);
    *(u16 *)(obj + 6) = (u16)Func_02002f42(
        *(s32 *)(p + 16) - *(s32 *)(obj + 16),
        *(s32 *)(p + 8) - *(s32 *)(obj + 8));
    return 0;
}

void FieldScene_RunScene3b4_02000ad0(void)
{
    s32 record;

    if (GameFlag_IsSet(0x9c8) == 0) {
        GameFlag_Set(0x9c8);
        Event_Begin();
        Camera_SetSpeed(0x20000, 0x4000);
        Camera_MoveToActor(15, 1);
        Camera_WaitForMove();
        Actor_FaceDirection(15, 0, 20);
        Actor_SetAttachedEffect(15, 0x102);
        Actor_RunRepeatedMotion(15, 2);
        Event_Wait(20);
        Actor_SetSpeed(15, 0x10000, 0x8000);
        Audio_PlayCue(152);
        record = Engine_GetTriggerActor(15);
        *(s32 *)(record + 40) = 0x80000;
        Actor_WalkToAndWait(15, 0x248, 0x2a8);
        Actor_FaceDirection(15, 0x4000, 20);
        Event_End();
    }
}

void FieldScene_RunScene3b4_02000b68(void)
{
    s32 rec7;
    s32 rec8;
    s32 record;

    if (GameFlag_IsSet(0x9c8) != 0) {
        rec8 = GameFlag_IsSet(0x9c9);
        if (rec8 == 0) {
            GameFlag_Set(0x9c9);
            Event_Begin();
            Camera_SetSpeed(0x20000, 0x4000);
            Camera_MoveToActor(15, 1);
            Camera_WaitForMove();
            Actor_FaceDirection(15, 0x4000, 20);
            Actor_SetAttachedEffect(15, 0x102);
            Actor_RunRepeatedMotion(15, 2);
            Event_Wait(20);
            Actor_SetSpeed(15, 0x10000, 0x8000);
            Audio_PlayCue(152);
            record = Engine_GetTriggerActor(15);
            *(s32 *)(record + 40) = 0xa0000;
            Actor_WalkToAndWait(15, 0x248, 0x298);
            Actor_FaceDirection(15, 0x4000, 20);
            Actor_SetAttachedEffect(15, 0x102);
            Event_Wait(30);
            Actor_SetSpeed(15, 0x80000, 0x4000);
            Actor_WalkToAndWait(15, 0x298, 0x298);
            Actor_WalkToAndWait(15, 0x2e8, 0x298);
            Actor_WalkToAndWait(15, 0x338, 0x298);
            Event_Wait(10);
            Audio_PlayCue(208);
            Work_SetValuesIfNonNegative(0x40000, 0x20000, 0x10000);
            Event_Wait(20);
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
            Event_Wait(30);
            Actor_SetPosition(15, 0x3780000, 0x2980000);
            rec7 = Value1(Engine_GetTriggerActor, 15);
            {
                s32 target = *(s32 *)(rec7 + 80);
                s32 shown = 0xf800;

                *(u16 *)(target + 30) = shown;
            }
            *(u16 *)(rec7 + 6) = 0;
            Func_02003156_a(rec7, 0);
            Value2(Engine_ObjectSetScript, rec7, 0x200a6fc);
            Event_End();
        }
    }
}

void FieldScene_RunScene3b4_02000ccc(void)
{
    s32 record;

    if (GameFlag_IsSet(0x9c9) != 0 && GameFlag_IsSet(0x9ca) == 0) {
        GameFlag_Set(0x9ca);
        Event_Begin();
        record = Value1(Func_02003236, 15);
        *(u16 *)(*(s32 *)(record + 80) + 30) = 0;
        Func_020031b0(record, 16);
        Audio_PlayCue(152);
        record = Engine_GetTriggerActor(15);
        *(s32 *)(record + 40) = 0x80000;
        Actor_FaceDirection(15, 0x8000, 30);
        Actor_SetAttachedEffect(15, 0x102);
        Actor_RunRepeatedMotion(15, 2);
        Event_Wait(20);
        Actor_SetSpeed(15, 0x10000, 0x8000);
        Audio_PlayCue(152);
        record = Engine_GetTriggerActor(15);
        *(s32 *)(record + 40) = 0x40000;
        Actor_WalkToAndWait(15, 0x370, 0x2a8);
        Event_Wait(10);
        Actor_SetAttachedEffect(15, 0x101);
        Actor_SetSpeed(15, 0x20000, 0x10000);
        Actor_WalkToAndWait(15, 0x370, 0x2b8);
        Actor_WalkToAndWait(15, 0x372, 0x2c0);
        Actor_WalkToAndWait(15, 0x370, 0x2c8);
        Actor_WalkToAndWait(15, 0x36e, 0x2d0);
        Actor_WalkToAndWait(15, 0x370, 0x2d8);
        Actor_WalkToAndWait(15, 0x372, 0x2e0);
        Actor_WalkToAndWait(15, 0x370, 0x2e8);
        Actor_WalkToAndWait(15, 0x36e, 0x2f0);
        Actor_WalkToAndWait(15, 0x370, 0x2f8);
        Actor_SetPosition(15, 0x3580000, 0x3380000);
        Event_Wait(10);
        Actor_FaceDirection(15, 0xc000, 20);
        Actor_SetAttachedEffect(15, 0x100);
        record = Engine_GetTriggerActor(15);
        *(s32 *)(record + 108) = 0x2008aa9;
        Event_End();
    }
}

void Func_02000e50(void)
{
    extern u8 Data_03001ebc[];

    s32 dst;
    s32 src;
    s32 idx;
    s32 tbl;
    s32 idx4;
    u8 *work;

    work = *(u8 **)Data_03001ebc;
    if (GameFlag_IsSet(0x9ca) != 0) {
        if (Data_02000240[293] != 15) {
            idx = *(s16 *)(work + 0x16c);
            dst = Value1_02000e50(Func_020033bc, 15);
            src = Value1_02000e50(Func_020033c4, 0);
            *(s32 *)(dst + 48) = *(s32 *)(src + 48);
            dst = Value1_02000e50(Func_020033ce, 15);
            src = Value1_02000e50(Func_020033d6, 0);
            *(s32 *)(dst + 52) = *(s32 *)(src + 48);
            idx -= 30;
            tbl = 0x0200a808;
            idx <<= 3;
            idx4 = idx + 4;
            Actor_WalkTo(15, *(s32 *)(tbl + idx), *(s32 *)(tbl + idx4));
        }
    }
}

/* Contiguous unnamed leaf-owner run for resource_3b4. */
void FieldScene_RunActor15ZeroStep(void)
{
    Event_Begin();
    Actor_SetAnimation(15, 0);
    Event_End();
}

s32 SceneData_SelectSecondaryByRuntimeSelector(void)
{
    extern u8 PrimaryOverlayData_02000ee0[];
    extern u8 SecondaryOverlayData_02000ee0[];
    extern u8 TertiaryOverlayData_02000ee0[];
    extern u8 DefaultOverlayData_02000ee0[];

    s16 selector = RuntimeSelectorTable[224];

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)PrimaryOverlayData_02000ee0;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)SecondaryOverlayData_02000ee0;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)TertiaryOverlayData_02000ee0;
    }
    return (s32)DefaultOverlayData_02000ee0;
}

void FieldScene_CallHelper3500(void)
{
    Leader_CheckAhead();
}

void FieldScene_RunScene3b4_02000fdc(s32 a0)
{
    u32 i;
    s32 record;

    if ((a0 & 0x100) != 0) {
        Audio_PlayCue(157);
        Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        Map_CopyCells(84, 29, 1, 3, 70, 49);
        Task_Wait(60);
    }
    Map_CopyCells(85, 29, 1, 3, 70, 49);
    Map_CopyCellAttributes(6, 49, 1, 1, 6, 50);
    Map_CopyCellAttributes(6, 49, 1, 1, 6, 51);
}

void FieldScene_RunScene3b4SequenceC(void)
{
    u32 i;
    s32 record;

    record = Engine_GetTriggerActor(0);
    if (*(u16 *)(record + 6) == 0xc000) {
        if (GameFlag_IsSet(0x9c4) == 0) {
            if (Func_02003588(243) != -1) {
                GameFlag_Set(0x9c4);
                Call1(FieldScene_RunScene3b4_02000fdc, 0x100);
                Func_020035ac(243);
            }
        }
    }
}

/* Four sites of one import, so four names. */
void SceneState_SetSelectorFlagWhenFacingC000(s32 selector)
{
    u8 *slot = Engine_GetTriggerActor(0);
    s32 flag;

    if (*(u16 *)(slot + 6) != 0xC000) {
        return;
    }
    flag = selector + 2496;
    if (GameFlag_IsSet(flag)!= 0) {
        return;
    }
    if (Func_020035d8(244) == -1) {
        return;
    }
    GameFlag_Set(flag);
    Func_02002038(0x100 | selector);
    Func_020035fe_a(244);
}

void FieldScene_RunIndexedStep0(void)
{
    SceneState_SetSelectorFlagWhenFacingC000(0);
}

void FieldScene_RunIndexedStep1(void)
{
    SceneState_SetSelectorFlagWhenFacingC000(1);
}

void SceneState_ResetCounter412OnHeading4000B(void)
{
    extern u8 *Data_03001ebc;

    u8 *slot;
    s16 *cnt;
    s32 reset;

    SceneState_SetSelectorFlagWhenFacingC000(2);
    slot = Engine_GetTriggerActor(0);
    if (*(u16 *)(slot + 6) == 0x4000) {
        cnt = (s16 *)(Data_03001ebc + 412);
        if (*cnt > 12) {
            Leader_CheckAhead();
            reset = 0;
            *cnt = reset;
        }
    }
}

void SceneState_ResetCounter412OnHeading4000(void)
{
    extern u8 *Data_03001ebc;

    u8 *p;
    s16 *cnt;
    s32 zero;

    SceneState_SetSelectorFlagWhenFacingC000(3);
    p = Engine_GetTriggerActor(0);
    if (*(u16 *)(p + 6) == 0x4000) {
        cnt = (s16 *)(Data_03001ebc + 412);
        if (*cnt > 12) {
            Leader_CheckAhead();
            zero = 0;
            *cnt = zero;
        }
    }
}

void SceneState_ApplyRectAndPlaceSlot12(void)
{
    s32 a = 25;
    s32 b = 48;
    s32 slot = 12;
    s32 x = 0x1980000;
    s32 z = 0x3080000;

    Map_CopyCells(25, 45, 1, 2, a, b);
    if (GameFlag_IsSet(0xeeb) == 0)
        Actor_SetPosition(slot, x, z);
    Event_Wait(1);
}

void ConfigureAndPlaceActorTwelve(void)
{
    s32 a = 25, b = 48;
    Map_CopyCells(24, 48, 1, 2, a, b);
    PlaceActor(12, 0x00080000, 0x00080000);
}

void FieldScene_RunStep8ValueEe7(void)
{
    Item_ShowFound(0xF4, 3);
    Actor_SetAnimation(0, 1);
    Party_GiveItem(0xF4, 0);
    Actor_SetPosition(8, 0, 0);
    GameFlag_Set(0xEE7);
}

void FieldScene_RunStep9ValueEe8(void)
{
    Item_ShowFound(0xF4, 3);
    Actor_SetAnimation(0, 1);
    Party_GiveItem(0xF4, 0);
    Actor_SetPosition(9, 0, 0);
    GameFlag_Set(0xEE8);
}

void FieldScene_RunStep10ValueEe9(void)
{
    Item_ShowFound(0xF4, 3);
    Actor_SetAnimation(0, 1);
    Party_GiveItem(0xF4, 0);
    Actor_SetPosition(0xA, 0, 0);
    GameFlag_Set(0xEE9);
}

void FieldScene_RunStep11ValueEea(void)
{
    Item_ShowFound(0xF4, 3);
    Actor_SetAnimation(0, 1);
    Party_GiveItem(0xF4, 0);
    Actor_SetPosition(0xB, 0, 0);
    GameFlag_Set(0xEEA);
}

void FieldScene_RunStep12ValueEeb(void)
{
    Item_ShowFound(0xF3, 3);
    Actor_SetAnimation(0, 1);
    Party_GiveItem(0xF3, 0);
    Actor_SetPosition(0xC, 0, 0);
    GameFlag_Set(0xEEB);
}

s32 *Engine_GetTriggerActor(s32 slot);
s32 Engine_TestTriggerFlag(s32 flag);
void Engine_SetTriggerFlag(s32 flag);

static __inline__ void SceneState_StoreStep(s16 *field, s32 step)
{
    *field = step;
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetSpeed(0, 0x1e666, 0xf333);
    Actor_SetSpeed(8, 0x1e666, 0xf333);
    Audio_PlayCue(188);
    record = Value1(Engine_GetTriggerActor, 0);
    if (record != 0) {
        Actor_SetDestination(8, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(8);
    Actor_SetDestinationOffset(0, 0, 24);
    Event_Wait(4);
    Audio_PlayCue(188);
    Actor_SetDestinationOffset(8, 0, 16);
    Actor_WaitForMove(0);
    Actor_SetDestination(8, 0x168, 152);
    Actor_WaitForMove(8);
    Event_End();
    GameFlag_Clear(0x220);
}

void SceneState_TriggerColumnTen(void)
{
    extern u8 *Data_03001ebc;
    extern s16 Data_02000240[];
    s32 *pos = Engine_GetTriggerActor(0);
    s32 x = pos[2] / 0x100000;
    s32 z = pos[4] / 0x100000;
    u8 *work = Data_03001ebc;

    if (Value1(Engine_TestTriggerFlag, 0x220) == 0 &&
        Data_02000240[0x24c / 2] == 0 && Data_02000240[0x24a / 2] != 9 &&
        x == 10 && (u32)(z - 16) <= 2) {
        Call1(Engine_SetTriggerFlag, 0x220);
        SceneState_StoreStep((s16 *)(work + 386), 92);
    }
}

void FieldScene_RunScene3b4SequenceA(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetSpeed(0, 0x1b333, 0xd999);
    Actor_SetSpeed(9, 0x1b333, 0xd999);
    Audio_PlayCue(188);
    record = Value1(Engine_GetTriggerActor, 0);
    if (record != 0) {
        Actor_SetDestination(9, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(9);
    Actor_SetDestinationOffset(0, 0, 24);
    Audio_PlayCue(188);
    Event_Wait(4);
    Actor_SetDestinationOffset(9, 0, 16);
    Actor_WaitForMove(0);
    Actor_SetDestination(9, 168, 0x108);
    Actor_WaitForMove(9);
    Event_End();
    GameFlag_Clear(0x220);
}

void SceneActor_TrackOriginColumnForSlot(s32 no)
{
    extern s16 Data_02000240[];

    s32 *pos = Engine_GetTriggerActor(0);
    s32 col = pos[2] / 0x100000;   /* +8  */
    s32 row = pos[4] / 0x100000;      /* +16 */
    s32 slot = no + 10;

    if (Data_02000240[293] == slot) return;
    if (col == Data_0200af74[no]) return;

    Actor_SetSpeed(slot, 0x48000, 0x24000);
    Audio_PlayCue(188);
    Actor_SetDestination(slot, (col << 4) + 8, 360);

    Data_0200af74[no] = col;

    if (row <= 22) {
        Actor_SetDestinationOffset(0, 0, 8);
    }
    Actor_WaitForMove(0);
}

void FieldScene_RunLateIndexedStep0(void)
{
    SceneActor_TrackOriginColumnForSlot(0);
}

void FieldScene_RunLateIndexedStep1(void)
{
    SceneActor_TrackOriginColumnForSlot(1);
}

void FieldScene_RunLateIndexedStep2(void)
{
    SceneActor_TrackOriginColumnForSlot(2);
}

void FieldScene_RunPrimarySequence(void)
{
    u32 i;
    s32 record;
    s32 base3_2000240;

    base3_2000240 = (s32)Data_02000240;
    if (*(s16 *)((base3_2000240 + 0x24a)) != 10) {
        Event_Begin();
        Actor_SetSpeed(0, 0x1b333, 0xd999);
        Actor_SetSpeed(10, 0x1b333, 0xd999);
        Audio_PlayCue(188);
        record = Value1_020015f0(Engine_GetTriggerActor, 0);
        if (record != 0) {
            Actor_SetDestination(10, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(10);
        Actor_SetDestinationOffset(0, 0, 24);
        Event_Wait(4);
        Audio_PlayCue(188);
        Actor_SetDestinationOffset(10, 0, 16);
        Actor_WaitForMove(0);
        Actor_SetDestination(10, 0x108, 0x168);
        Actor_WaitForMove(10);
        Event_Wait(10);
        Event_End();
    }
}

void SceneState_TriggerColumnNineteen(void)
{
    extern u8 *Data_03001ebc;
    extern s16 Data_02000240[];
    s32 *pos = Engine_GetTriggerActor(0);
    s32 x = pos[2] / 0x100000;
    s32 z = pos[4] / 0x100000;
    u8 *work = Data_03001ebc;
    s16 *state = Data_02000240;

    if (state[0x24a / 2] != 12 && Value1(Engine_TestTriggerFlag, 0x220) == 0 &&
        state[0x24c / 2] == 0 && x == 19 && (u32)(z - 15) <= 1) {
        Call1(Engine_SetTriggerFlag, 0x220);
        SceneState_StoreStep((s16 *)(work + 386), 96);
    }
}

void FieldScene_RunScene3b4SequenceB(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetSpeed(0, 0x1b333, 0xd999);
    Actor_SetSpeed(12, 0x1b333, 0xd999);
    Audio_PlayCue(188);
    record = Value1(Engine_GetTriggerActor, 0);
    if (record != 0) {
        Actor_SetDestination(12, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(12);
    Actor_SetDestinationOffset(0, 0, 24);
    Audio_PlayCue(188);
    Actor_SetDestinationOffset(12, 0, 16);
    Actor_WaitForMove(0);
    Actor_SetDestination(12, 0x138, 232);
    Actor_WaitForMove(12);
    Event_End();
    GameFlag_Clear(0x220);
}

void SetBlendAlphaCoefficients(void)
{
    u32 coefficient = 208;

    coefficient <<= 4;
    *(u16 *)0x04000052 = coefficient;
}

/* Complete blend-alpha setter through return and its two pool words. */
void SceneEffect_SetBlendAlpha0607(void)
{
    u16 value = 0x0607;

    *(volatile u16 *)0x04000052 = value;
}

/*
 * Scene state interaction for resource_3b4.
 *
 * A Func_ name in the import veneer band 0x02002468-0x0200261f names the
 * main-image address held in the veneer's trailing word, not a runtime
 * address the call reaches directly.  Declarations are old-style because
 * those imports are reached with differing argument counts from different
 * call sites.
 */

/* Slot record lookup, then the scene flag test, clear and set imports. */

/*
 * Dispatch on slot 0's halfword at +6, the facing field, which takes the
 * values 0, 0x4000, 0x8000 and 0xc000.  The 160-byte owner includes its two
 * pool words, 0x206 and 0x207.  Two arms repaint one collision cell, but
 * only while scene flag 0x206 is set, and then move flag 0x207.  The 0x8000
 * arm does no flag work and branches on slot 0's height word at +12, read
 * from the record pointer already in hand rather than a fresh lookup.
 */
void FieldScene_DispatchBySlotZeroFacing(void)
{
    s32 *slot = Engine_GetTriggerActor(0);
    u16 facing = *(u16 *)((u8 *)slot + 6);

    if (facing == 0xc000) {
        if (GameFlag_IsSet(0x206) != 0) {
            { s32 fifth = 45; s32 last = 43; Map_CopyCellAttributes(46, 43, 1, 1, fifth, last); }
        }
        GameFlag_Clear(0x207);
        Func_02003ef2();
    } else if (facing == 0x4000) {
        Func_02003ef8_a();
    } else if (facing == 0) {
        if (GameFlag_IsSet(0x206) != 0) {
            { s32 fifth = 45; s32 last = 43; Map_CopyCellAttributes(58, 36, 1, 1, fifth, last); }
        }
        GameFlag_Set(0x207);
        Leader_CheckAhead();
    } else if (facing == 0x8000) {
        if (slot[3] == 0) {          /* +12 */
            SceneActor_BranchOnSlotZeroAtTile38();
        } else {
            Leader_CheckAhead();
        }
    }
}

/* Deliberate no-op callback. */
void FieldScene_NoOpCallback(void) {}

void SceneActor_MarkSlot13AndSetFlag200(void)
{
    u8 *slot = Engine_GetTriggerActor(13);
    s32 fifth = 40;
    s32 sixth = 55;

    Map_CopyCellAttributes(40, 54, 1, 1, fifth, sixth);
    if (slot != 0) {
        u8 *other = Func_02003eec(13) + 85;
        u8 *flags = slot + 35;

        *other = 0;
        *flags = 2;
    }
    GameFlag_Set(512);
}

/*
 * Facing target scene for resource_3b4.
 *
 * A Func_ name in the import veneer band 0x02002468-0x0200261f names the
 * main-image address held in the veneer's trailing word, not a runtime
 * address the call reaches directly.  Declarations are old-style because
 * those imports are reached with differing argument counts from different
 * call sites.
 */

/* Slot record lookup, then the notification and step imports. */
void SceneState_BranchOnActorZeroFacing(void)
{
    struct Slot *slot = Engine_GetTriggerActor(0);

    if (slot->f06 == 0) {
        Leader_CheckAhead();
    } else {
        SceneActor_BranchOnSlotZeroAtTile38();
    }
}

void SceneState_ApplyFourRectsAndSetActor8Byte85(void)
{
    Map_CopyCellAttributes(57, 42, 1, 1, 40, 42);
    Map_CopyCellAttributes(57, 42, 1, 1, 41, 42);
    Map_CopyCellAttributes(58, 42, 1, 1, 42, 42);
    Map_CopyCellAttributes(62, 37, 3, 1, 37, 42);

    Func_02003f78(8)[85] = 1;
}

void ActorPresentation_SetSceneCell58AndMarkActorEight(void)
{
    s32 extent = 42;
    u8 *entry;

    Map_CopyCellAttributes(58, 41, 1, 1, extent, extent);
    entry = Func_02003fa2(8) + 35;
    *entry = 2;
}

void SceneState_ApplyRectAndSetActor8Byte35(void)
{
    s32 w = 41;
    s32 h = 42;
    u8 *p;

    Map_CopyCellAttributes(44, 42, 1, 1, w, h);
    p = Func_02003fcc(8) + 35;
    *p = 2;
}

void SceneState_ApplyRectAndSetSlotEightByte35(void)
{
    s32 width = 40;
    s32 height = 42;
    u8 *entry;

    Map_CopyCellAttributes(39, 42, 1, 1, width, height);
    entry = Func_02003ff8(8) + 35;
    *entry = 2;
}

/*
 * Compare the X tiles of slots 0 and 8, each the word at +8 divided by
 * 0x100000.  The 100-byte owner includes its one pool word, 0x000fffff,
 * which that truncating signed division reads.  The special cases apply only
 * when slot 0 sits on tile 38 and slot 8 does not; then slot 0's halfword at
 * +6 selects one of two notifications.  Every other case, an unrecognised
 * halfword included, runs the three-step ordinary path.
 */
void SceneActor_BranchOnSlotZeroAtTile38(void)
{
    s32 *slot0 = Engine_GetTriggerActor(0);
    s32 *slot8 = Engine_GetTriggerActor(8);
    s32 x0 = slot0[2] / 0x100000;
    s32 x8 = slot8[2] / 0x100000;

    if (x0 == 38 && x8 != 38) {
        u16 facing = ((u16 *)slot0)[3];

        /*
         * The facing value is still in r0 at both branches, but whether
         * either callee reads it is unverified, so no argument is passed.
         */
        if (facing == 0xc000) {
            Func_02004174();
            return;
        }
        if (facing == 0x4000) {
            Func_0200417a();
            return;
        }
    }

    SceneState_ApplyFourRectsAndSetActor8Byte85();
    StagedActor_AdvancePair();
    Func_02003680();
}

void FieldScene_RunScene3b4_02001bc4(void)
{
    u32 i;
    s32 record;

    Call1(Func_02004044, 0x2009e95);
    Actor_SetPosition(14, 0, 0);
    if (GameFlag_IsSet(0x207) != 0) {
        Map_CopyCellAttributes(58, 36, 1, 1, 45, 43);
    } else {
        Map_CopyCellAttributes(46, 43, 1, 1, 45, 43);
    }
    SceneActor_PassActorNinePositionWithId107();
    GameFlag_Set(0x206);
}

void SceneActor_RunWhenActor9AtTile45x43(void)
{
    s32 *slot = Engine_GetTriggerActor(9);
    s32 x = slot[2] / 0x100000;
    s32 z = slot[4] / 0x100000;

    if (x == 45 && z == 43) {
        FieldScene_RunScene3b4_02001bc4();
    }
}

void FieldScene_RunTwoStepSequence(void)
{
    StagedActor_AdvancePair();
    SceneActor_RunWhenActor9AtTile45x43();
}

void SceneState_ApplyTwoRectsAtRow56(void)
{
    s32 base = 55;

    Map_CopyCellAttributes(38, 56, 1, 1, 38, base);
    Map_CopyCellAttributes(42, 56, 1, 1, 42, base);
}

void SceneState_ApplyRectAndClearSlotTenByte85(void)
{
    s32 width = 38;
    s32 height = 55;
    u8 *entry;

    Map_CopyCellAttributes(40, 54, 1, 1, width, height);
    entry = Func_020041fc(10) + 85;
    *entry = 0;
}

void SceneState_ApplyRectAndClearActor10Byte85(void)
{
    s32 w = 42;
    s32 h = 55;
    u8 *p;

    Map_CopyCellAttributes(40, 54, 1, 1, w, h);
    p = Func_02004228(10) + 85;
    *p = 0;
}

void FieldScene_RunThreeCallSequenceB(void)
{
    SceneState_ApplyTwoRectsAtRow56();
    StagedActor_AdvancePair();
    Func_02003a68();
}

void FieldScene_DispatchByActorZeroFacing(void)
{
    struct Slot *slot = Engine_GetTriggerActor(0);

    if (slot->f06 == 0x4000) {
        Func_02004322(0, 6, 0);
    } else {
        FieldScene_RunThreeCallSequenceB();
    }
}

void SceneState_ApplyFourRectsAt48_55(void)
{
    s32 base = 55;

    Map_CopyCellAttributes(48, 55, 1, 1, 49, base);
    Map_CopyCellAttributes(48, 55, 1, 1, 50, base);
    Map_CopyCellAttributes(48, 55, 1, 1, 51, base);
    Map_CopyCellAttributes(48, 55, 1, 1, 52, base);
}

/*
 * Repaint the four cells, then one cell for each of slots 11 and 12 at that
 * slot's own X tile.  The 92-byte owner at 0x02001df8 includes two bytes of
 * alignment and the pool word 0x000fffff.  The tile divisions must stay
 * spelled `/ 0x100000': the reference biases a negative value before the
 * arithmetic shift, which is exactly this truncating signed division.
 */
void ActorPresentation_RepaintCellsAtActorsElevenAndTwelve(void)
{
    s32 *slot;
    s32 tile;

    SceneState_ApplyFourRectsAt48_55();

    slot = Engine_GetTriggerActor(11);
    tile = slot[2] / 0x100000;
    Map_CopyCellAttributes(53, 55, 1, 1, tile, 55);
    slot = Engine_GetTriggerActor(12);
    tile = slot[2] / 0x100000;
    Map_CopyCellAttributes(53, 55, 1, 1, tile, 55);
}

void FieldScene_RunSingleStep(void)
{
    ActorPresentation_RepaintCellsAtActorsElevenAndTwelve();
}

void FieldScene_RunThreeCallSequence(void)
{
    SceneState_ApplyFourRectsAt48_55();
    StagedActor_AdvancePair();
    FieldScene_RunSingleStep();
}

void FieldScene_CallHelper3c70(void)
{
    ActorPresentation_RepaintCellsAtActorsElevenAndTwelve();
}

void FieldScene_RunThreeStepSequence(void)
{
    SceneState_ApplyFourRectsAt48_55();
    StagedActor_AdvancePair();
    FieldScene_CallHelper3c70();
}

void ActorPresentation_PlaceActorFourteenOnActorNine(void)
{
    struct Actor *target = Engine_GetTriggerActor(14);
    struct Actor *source = Engine_GetTriggerActor(9);

    target->f0c = 0x200000;
    target->f08 = source->f08;
    target->f10 = source->f10 + 0x10000;
}

void SceneActor_PassActorNinePositionWithId107(void)
{
    struct Frame *frame = Engine_GetTriggerActor(9);

    MapObject_SetPosition(107, frame->f08, frame->f10 + 0x10000);
}

/*
 * Publish one marker byte at +35 to slots 8, 10, 11 and 12 according to slot
 * 0's height word at +12.  The 156-byte owner includes its one pool word,
 * 0x000fffff, read by the tile division.  The marker local is what carries
 * the value 2 across the high path, which branches over the clear to 0.
 * Slot 11's record is fetched once on each path rather than once before
 * them, and that duplication is what reproduces the reference.
 */
void SceneActor_PublishMarkerBySlotZeroHeight(void)
{
    s32 *slot0 = Engine_GetTriggerActor(0);
    u8 marker;

    if (slot0[3] > 0x100000) {                 /* +12 */
        marker = 2;
        ((u8 *)Engine_GetTriggerActor(8))[35] = marker;
        if (Engine_GetTriggerActor(10)[3] == 0) {
            ((u8 *)Engine_GetTriggerActor(10))[35] = marker;
        }
        ((u8 *)Engine_GetTriggerActor(11))[35] = marker;
    } else {
        if (Engine_GetTriggerActor(10)[3] == 0 &&
            Engine_GetTriggerActor(0)[4] / 0x100000 > 56) {   /* +16 */
            Actor_SetSpritePriority(10, 3);
        } else {
            Actor_SetSpritePriority(10, 1);
            ((u8 *)Engine_GetTriggerActor(10))[35] = 1;
        }
        marker = 0;
        ((u8 *)Engine_GetTriggerActor(11))[35] = marker;
    }

    ((u8 *)Engine_GetTriggerActor(12))[35] = marker;
}

void FieldScene_RunScene3b4_02002188(void);
void FieldScene_RunScene3b4_02002290(void);
void FieldScene_RunScene3b4_02002334(void);

s32 FieldScene_DispatchByRuntimeSelector(void)
{
    u8 *base;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    base = RuntimeSelectorTable;
    if (*(s16 *)(base + 0x1c0) == (s32)&PrimaryRuntimeSelector) {
        FieldScene_RunScene3b4_02002188();
    }
    if (*(s16 *)(base + 0x1c0) == (s32)&SecondaryRuntimeSelector) {
        FieldScene_RunScene3b4_02002290();
    }
    if (*(s16 *)(base + 0x1c0) == (s32)&TertiaryRuntimeSelector) {
        FieldScene_RunScene3b4_02002334();
    }
    return 0;
}

void FieldScene_RunScene3b4_02002188(void)
{
    u32 i;
    s32 record;

    Task_Wait(1);
    Func_0200425a(12, 243);
    Func_02004262(11, 244);
    Func_0200426a(10, 244);
    Func_02004272(9, 244);
    Func_0200427a(8, 244);
    if (GameFlag_IsSet(0xee7) == 0) {
        Actor_SetPosition(8, 0xe80000, 0x3680000);
    }
    if (GameFlag_IsSet(0xee8) == 0) {
        Actor_SetPosition(9, 0x1280000, 0x3380000);
    }
    if (GameFlag_IsSet(0xee9) == 0) {
        Actor_SetPosition(10, 0x1480000, 0x2f80000);
    }
    if (GameFlag_IsSet(0xeea) == 0) {
        Actor_SetPosition(11, 0x1680000, 0x3680000);
    }
    if (GameFlag_IsSet(0x9c0) != 0) {
        Func_02003168_a(0);
    }
    if (GameFlag_IsSet(0x9c1) != 0) {
        Func_02003178(1);
    }
    if (GameFlag_IsSet(0x9c2) != 0) {
        Func_02003188(2);
    }
    if (GameFlag_IsSet(0x9c3) != 0) {
        Func_02003198(3);
    }
    if (GameFlag_IsSet(0x9c4) != 0) {
        FieldScene_RunScene3b4_02000fdc(0);
    }
}

void FieldScene_RunScene3b4_02002290(void)
{
    s32 record;

    *(u8 *)(Func_020047d6(8) + 89) = 1;
    *(u8 *)(Func_020047e2(9) + 89) = 1;
    *(u8 *)(Func_020047ec(10) + 89) = 1;
    *(u8 *)(Func_020047f6(11) + 89) = 1;
    record = Engine_GetTriggerActor(8);
    *(s32 *)(record + 24) = 0xb333;
    record = Value1_02002290(Engine_GetTriggerActor, 9);
    *(s32 *)(record + 24) = 0xb333;
    record = Value1_02002290(Engine_GetTriggerActor, 10);
    *(s32 *)(record + 24) = 0xb333;
    record = Value1_02002290(Engine_GetTriggerActor, 11);
    *(s32 *)(record + 24) = 0xb333;
    record = Engine_GetTriggerActor(12);
    *(s32 *)(record + 24) = 0xb333;
    Call2(Func_0200475e_a, 0x20097ad, 0xc80);
    Value2(Func_02004768, 0x200941d, 0xc80);
    Value2(Func_02004772, 0x2009309, 0xc80);
    {
        u16 t;
        t = 0x3f42;
        *(volatile u16 *)0x04000050 = t;
        t = 0x607;
        *(volatile u16 *)0x04000052 = t;
    }
}

void FieldScene_RunScene3b4_02002334(void)
{
    s32 record;

    *(u8 *)(Func_0200487a(14) + 85) = 0;
    Call2(Func_020047ba, 0x2009e95, 0xc80);
    Value2(Func_020047c4, 0x2009edd, 0xc80);
    MapObject_SetPosition(107, 0, 0);
    if (GameFlag_IsSet(0xed9) != 0) {
        Actor_SetAnimation(14, 2);
    }
    Func_02003e3c();
    SceneActor_RunWhenActor9AtTile45x43();
    Func_02004074();
    FieldScene_RunSingleStep();
    FieldScene_CallHelper3c70();
    Actor_SetSpritePriority(8, 3);
    *(u8 *)(Func_020048d2(11) + 85) = 0;
    *(u8 *)(Func_020048dc(12) + 85) = 0;
    ActorPresentation_RepaintCellsAtActorsElevenAndTwelve();
    if (GameFlag_IsSet(0x200) != 0) {
        SceneActor_MarkSlot13AndSetFlag200();
        Actor_SetAnimation(13, 5);
    }
    if (GameFlag_IsSet(0x109) == 0) {
        if (GameFlag_IsSet(0x9ca) != 0) {
            Actor_SetPosition(15, 0x3580000, 0x3380000);
            record = Engine_GetTriggerActor(15);
            *(s32 *)(record + 108) = 0x2008aa9;
        } else if (GameFlag_IsSet(0x9c9) != 0) {
            Actor_SetPosition(15, 0x3780000, 0x2980000);
            record = Value1_02002334(Engine_GetTriggerActor, 15);
            *(u16 *)(*(s32 *)(record + 80) + 30) = 0;
            Func_020048c4(record, 16);
        } else if (GameFlag_IsSet(0x9c8) != 0) {
            Actor_SetPosition(15, 0x2480000, 0x2a80000);
        } else {
            Actor_SetPosition(15, 0x2480000, 0x2980000);
        }
    }
}

#include "TYPES.H"

#define NULL ((void *)0)
#define CalculateFacingAngle Func_0200107a
#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_0000001e
#define SecondaryRuntimeSelector Value_00000023
#define TertiaryRuntimeSelector Value_00000020
#define PrimaryOverlayData Data_020091ec
#define SecondaryOverlayData Data_0200930c
#define TertiaryOverlayData Data_0200936c
#define DefaultOverlayData Data_020091d4
#define PrimaryOverlayData_0200011c Data_02009498
#define SecondaryOverlayData_0200011c Data_02009600
#define TertiaryOverlayData_0200011c Data_020096f0
#define DefaultOverlayData_0200011c Data_02009480
#define PrimaryOverlayData_02000198 Data_02009744
#define SecondaryOverlayData_02000198 Data_02009a2c
#define TertiaryOverlayData_02000198 Data_02009bc4
#define DefaultOverlayData_02000198 Data_02009738

#include "FACING_OBJECT.H"
#include "FIELD_EVENT.H"
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"

extern s16 Data_02000240[];
extern u8 Value_00000020;
extern u8 Data_020093fc[];
extern u8 Data_020091c0[];
extern u8 Value_0000001e;
extern u8 Value_00000023;
extern u8 Data_02009d28[];
extern u8 Data_02009d7c[];

s32 Func_0200107a(s32, s32);
void Func_020012c6(s32, s32, s32);
s32 Func_020013e4();
s32 Func_0200140a();
s32 Func_02001460();
s32 Func_020014da();
s32 Func_020014e2();
s32 Func_02001522();
void Func_02001552();
void Func_02001568();
void Func_0200157c();
void Func_02001598();
void Func_020015ac();
struct FieldActor *Func_020015c0();
void Func_020015c8();
void Func_020015ee();
void Func_020015f6();
void Func_0200166a();
void Func_020016ca();
struct FieldActor *Func_02001522_a();
s32 Func_0200162a();
s32 Func_02001636();
s32 Func_0200165a();
u8 *Func_0200169a();
void Func_0200172e();
void Func_02001760();
void Func_02001764();
void Func_020017a4();
void Func_020017ac();
void Func_020017b0();
void Func_020017bc();
void Func_020017d0();
void Func_020017d4();
void Func_020017f0();
void Func_020017fc();
s32 Func_02001804();
void Func_02001808();
void Func_0200181c();
void Func_02001826();
s32 Func_02001828();
void Func_0200182c();
void Func_02001834();
void Func_02001838();
void Func_02001844();
void Func_02001880();
s32 Func_020018a2();
void Func_020018ba_a();
u8 *Func_020018c2();
void Func_020018ce();
void Func_020018f6();
void Func_0200190e();
void Func_02001928();
void Func_0200193a();
void Func_02001960();
void Func_0200196c();
void Func_02001978();
void Func_0200135e();
s32 Func_0200198e();
void Func_020019b2();
s32 Func_020019f4();
s32 Func_02001a04();
s32 Func_02001a04_a();
s32 Func_02001a0a();
s32 Func_02001a30();
s32 Func_02001a3a();
void Func_02001a4e();
void Func_02001a60();
void Func_02001a6c();
void Func_02001948();
void Func_02001b86();
s32 Func_02001b9c();
s32 Func_02001bb8();
s32 Func_02001bc0();
s32 Func_02001bc8();
s32 Func_02001bc8_a();
s32 Func_02001bee();
s32 Func_02001bf8();
void Func_02001c3c();
void Func_02001c48();
void Func_02001c54();
s32 Func_02001c70();
s32 Func_02001c70_a();
s32 Func_02001c7e();
s32 Func_02001c8a();
s32 Func_02001c96();
void Func_02001caa();
s32 Func_02001cb2();
s32 Func_02001cc2();
void Func_02001ccc();
s32 Func_02001cd0();
void Func_02001cd8();
s32 Func_02001cde();
s32 Func_02001cec();
void Func_02001cc2_a();
s32 Func_02001dd6();
s32 Func_02001e18();
s32 Func_02009098();
void Func_020090a0();
void Func_0200170c(void);
void Func_0200119c();
void Func_02001398();
void Func_020015de();
void Func_020018e2();
s32 Func_02001aac();
s32 Func_02001ab4();
void Func_02001b82();
void Func_02001b9c_a();
void Func_02001baa();
void Func_02001bfe();
void Func_02001c18();
u8 *Func_02001d90();
u8 *Func_02001f90();
void Func_02001f96();
void Func_02001fa8();
u8 *Func_02002078();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Per-site raw names preserve the stock relocated branch operands; they
 * resolve to Func_080090c8, Object_SetCallback and Object_InitializeMode respectively. */

/* Per-site raw names: the first resolves to Scene_GetRecord and the four
 * renderer sites resolve to Func_080091c0. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3);
}

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Shared cross-overlay scene-record block; +450 is the scene sub-state. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_020008f0(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];


    f(a0);
}

static __inline__ s32 Value1_020008f0(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];


    return f(a0);
}

static __inline__ void Call3_020008f0(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];


    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02000ae0(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02000d10(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];


    return f(a0);
}

static __inline__ u16 ReadU16Elem(u16 *base, s32 idx)
{
    extern u8 Data_03001ebc[];


    return *(u16 *)(base + idx);
}

static __inline__ void Call4_020008f0(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_03001ebc[];


    f(a0, a1, a2, a3);
}

static __inline__ void Call1_020001ec(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ void bump_step_020001ec(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call2_02000890(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02000890(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

s32 SceneActor_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 delta;
    u16 old;
    s32 tgt;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8)(0xFE & object->facing_flags);
        tgt = (u16)CalculateFacingAngle(target->position_z - object->position_z, target->position_x - object->position_x);
        old = object->facing;
        delta = (s16)(tgt - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            /* The loader relocates the stored pool word to -0x1000. */
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            object->facing = (u16)(old + delta);
        }
    }
    return 1;
}

s32 SceneData_SelectOverlayDataByRuntimeSelector(void)
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

s32 SceneData_GetTable93FCWhenState20(void)
{
    if (Data_02000240[224] == (s32)&Value_00000020) {
        return (s32)Data_020093fc;
    }
    return 0;
}

/*
 * Table getter for resource_38b. The owner at 0x02000104 is eight bytes and
 * includes its one pool word at 0x02000108: the pc-relative load reads that
 * word, so the word belongs to this owner. The word is an address returned
 * without being dereferenced. Many getters share this body, but each returns
 * a different address.
 */
u8 *SceneData_GetTable942c(void)
{
    return (u8 *)0x0200942c;
}

void SceneState_SetValues9_3_0(void)
{
    Func_020012c6(9, 3, 0);
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    extern u8 PrimaryOverlayData_0200011c[];
    extern u8 SecondaryOverlayData_0200011c[];
    extern u8 TertiaryOverlayData_0200011c[];
    extern u8 DefaultOverlayData_0200011c[];
    s16 selector = RuntimeSelectorTable[224];

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)PrimaryOverlayData_0200011c;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)SecondaryOverlayData_0200011c;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)TertiaryOverlayData_0200011c;
    }
    return (s32)DefaultOverlayData_0200011c;
}

void FieldScene_RunScriptedSteps947And29DD(void)
{
    extern u8 Data_03001ebc[];

    Engine_EventBegin();
    Engine_MessageShowCentered(0x947, 1);
    Engine_MessageShowCentered(0x29DD, 1);
    Engine_EventEnd();
}

s32 SceneData_SelectDataByRuntimeSelectorB(void)
{
    extern u8 PrimaryOverlayData_02000198[];
    extern u8 SecondaryOverlayData_02000198[];
    extern u8 TertiaryOverlayData_02000198[];
    extern u8 DefaultOverlayData_02000198[];
    s16 selector = RuntimeSelectorTable[224];

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)PrimaryOverlayData_02000198;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)SecondaryOverlayData_02000198;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)TertiaryOverlayData_02000198;
    }
    return (s32)DefaultOverlayData_02000198;
}

void FieldScene_RunScriptedStep1472(void)
{
    extern u8 Data_03001ebc[];

    Engine_EventBegin();
    Engine_MessageShowCentered(0x1472, 1);
    Engine_EventEnd();
}

void FieldScene_RunScriptedStep146E(void)
{
    extern u8 Data_03001ebc[];

    Engine_EventBegin();
    Engine_MessageShowCentered(0x146E, 1);
    Engine_EventEnd();
}

void SceneDialogue_RunLine1470(void)
{
    extern u8 Data_03001ebc[];

    Engine_EventBegin();
    Engine_MessageShowCentered(0x1470, 1);
    Engine_EventEnd();
}

void FieldScene_RunScene38b_02000240(void)
{
    extern u8 Data_03001ebc[];

    Engine_EventBegin();
    Call1_020001ec(Engine_EventSetMessage, 0x13ae);
    if (Value1(Func_02009098, 0x301) != 0) {
        bump_step_020001ec(1);
    }
    Engine_EventShowMessage(9, 0);
    Call1_020001ec(Func_020090a0, 0x301);
    Engine_EventEnd();
}

void SceneDialogue_RunActorTwelveDialogue(void)
{
    extern u8 Data_03001ebc[];

    Engine_EventBegin();
    Engine_EventSetMessage(0x13B3);
    Engine_EventAskYesNo(12, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActorFourteenDialogue(void)
{
    extern u8 Data_03001ebc[];

    Engine_EventBegin();
    Engine_EventSetMessage(0x13B7);
    Engine_EventAskYesNo(14, 0);
    Engine_EventEnd();
}

void SceneDialogue_ShowLine16BF(void)
{
    extern u8 Data_03001ebc[];

    Engine_EventBegin();
    Engine_EventSetMessage(0x16BF);
    Engine_EventAskYesNo(21, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActorSixteenDialogue(void)
{
    extern u8 Data_03001ebc[];

    Engine_EventBegin();
    Engine_EventSetMessage(0x16C8);
    Engine_EventAskYesNo(16, 0);
    Engine_EventEnd();
}

void SceneDialogue_ShowLine16CC(void)
{
    extern u8 Data_03001ebc[];

    Engine_EventBegin();
    Engine_EventSetMessage(0x16CC);
    Engine_EventAskYesNo(18, 0);
    Engine_EventEnd();
}

void FieldScene_RunEarlySequence(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    u8 *record;
    s32 v5;
    u8 *tbl;
    u8 *tbl2;
    s32 off;
    s32 off2;
    s32 a1;
    s32 a2;
    u8 *p7;

    p7 = *(u8 **)Data_03001ebc;
    Engine_EventBegin();
    for (i = 8; i < 66; i++) {
        record = (u8 *)Value1(Func_0200140a, i);
        if (record != 0) {
            record[85] = 0;
        }
    }
    v5 = (s32)((s32)(*(u16 *)(p7 + 0x16c) - 3) << 16) >> 16;
    if (v5 == 6) {
        Engine_AudioPlayCue(188);
    } else {
        Call1((void (*)())Func_02001522, 158);
    }
    off = v5 << 2;
    tbl = (u8 *)0x02009d0c;
    a1 = *(s16 *)(tbl + off);
    off2 = off + 2;
    a2 = *(s16 *)(tbl + off2);
    tbl2 = (u8 *)0x02009cf0;
    Value3(Func_020013e4, *(s32 *)(tbl2 + off), a1, a2);
    Call3(Engine_ActorSetSpeed, 0, 0x8000, 0x4000);
    *(u8 *)(Func_02001460(0) + 85) = 0;
    *(s32 *)((*(u8 **)Data_03001ebc + 0x1c0)) = 0x100;
    if (v5 == 6) {
        Engine_ActorSetAnimation(0, 2);
        Call3(Engine_ActorSetDestinationOffset, 0, 0, -4);
    } else {
        Call3(Engine_ActorCenterAndWalk, 0, 3, -16);
    }
    if (v5 == 4) {
        Engine_ActorSetSpritePriority(0, 3);
    } else {
        Engine_ActorSetSpritePriority(0, 2);
    }
    Engine_EventWait(16);
    Engine_EventRequestExit(v5 + 3);
    Engine_EventEnd();
}

void FieldScene_RunScene38bSequenceC(void)
{
    struct FieldActor *rec;
    struct FieldActor *rec7;

    rec = (struct FieldActor *)Value1(Func_020014da, 0);
    rec7 = (struct FieldActor *)Value1(Func_020014e2, 11);
    if ((rec7->x.fixed >> 20) == 6) {
        Engine_EventBegin();
        Engine_ActorSetSpritePriority(11, 1);
        Func_02001552(0, 2);
        Engine_EventWait(20);
        Call3(Engine_ActorSetSpeed, 0, 0x3333, 0x1999);
        Call3(Engine_ActorSetSpeed, 11, 0x3333, 0x1999);
        Func_02001522_a(0)->unknown_5a &= ~1;
        rec7->motion_flags = 0;
        rec->scale_x = -0x10000;
        Call2(Engine_ActorSetAttachedEffect, 0, 0x102);
        Engine_ActorSetAnimation(0, 16);
        Func_02001568(11, 111, 196);
        rec->scale_x = 0x10000;
        Func_0200157c(0, 128, 185);
        Engine_EventWait(20);
        rec->scale_x = -0x10000;
        Call2(Engine_ActorSetAttachedEffect, 0, 0x102);
        Engine_ActorSetAnimation(0, 16);
        Func_02001598(11, 121, 190);
        rec->scale_x = 0x10000;
        Func_020015ac(0, 141, 189);
        Engine_EventWait(20);
        rec->scale_x = -0x10000;
        Call2(Engine_ActorSetAttachedEffect, 0, 0x102);
        Engine_ActorSetAnimation(0, 16);
        Func_020015c8(11, 132, 186);
        rec->scale_x = 0x10000;
        Func_020015c0(0)->unknown_5a |= 1;
        Call3(Engine_ActorSetSpeed, 0, 0x9999, 0x4ccc);
        Func_020015f6(0, 166, 185);
        Call3(Func_0200166a, 0, 0x8000, 20);
        Engine_ActorSetSpritePriority(11, 2);
        Func_020016ca(0, 11);
        Engine_TaskWait(10);
        Call1(Engine_EventSetMessage, 0x1774);
        Engine_EventShowMessage(11, 0);
        Engine_PsynergyCancel();
        Engine_TaskWait(10);
        Call1(Func_020015ee, 0x848);
        Engine_EventEnd();
    }
}

void FieldScene_CallHelper170c(void)
{
    Func_0200170c();
}

void FieldScene_RunScene38b_02000584(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    struct FieldActor *rec7;
    struct FieldActor *record;
    s32 base5_20091c0;

    rec7 = Value1(Func_0200165a, 0);
    if (Value1(Func_0200162a, 0x845) == 0) {
    } else {
        if (Value1(Func_02001636, 0x848) == 0) {
        } else {
            Engine_EventBegin();
            Call2(Engine_CameraSetSpeed, 0x26666, 0x4ccc);
            Call4(Engine_CameraMoveTo, 0x1070000, -1, 0xad0000, 1);
            Engine_CameraWaitForMove();
            record = Func_0200169a(12);
            if (record->x.fixed > rec7->x.fixed) {
                Call3(Func_0200172e, 13, 0x5000, 20);
                Call3(Engine_ActorShowEmote, 13, 0x100, 20);
                Call1(Engine_EventSetMessage, 0x1775);
                Engine_EventShowMessageAndWait(13, 0, 10);
                Call3(Engine_ActorShowEmote, 12, 0x100, 0);
            } else {
                Call3(Func_02001764, 12, 0x3000, 20);
                Call3(Engine_ActorShowEmote, 12, 0x100, 20);
                Call1(Engine_EventSetMessage, 0x1775);
                Engine_EventShowMessageAndWait(12, 0, 10);
                Call3(Engine_ActorShowEmote, 13, 0x100, 0);
            }
            Call3(Engine_ActorShowEmote, 14, 0x100, 0);
            Call3(Func_020017a4, 14, 0x3000, 0);
            Call3(Func_020017b0, 12, 0x5000, 0);
            Call3(Func_020017bc, 13, 0x3000, 0);
            Call3(Func_02001760, 0, 0x10c, 184);
            Call3(Func_020017d4, 0, 0xc000, 40);
            Func_020017ac(13, 2);
            Engine_EventShowMessageAndWait(13, 0, 10);
            Func_020017f0(13, 0, 0);
            Call3(Func_020017fc, 14, 0x3000, 20);
            Call3(Func_02001808, 12, 0x8000, 20);
            Func_020017d0(12, 3);
            Call2(Engine_ActorSetAttachedEffect, 14, 0x102);
            Engine_EventWait(40);
            Call3(Func_0200182c, 14, 0x3000, 10);
            Call3(Func_02001838, 12, 0x5000, 0);
            Call3(Func_02001844, 13, 0x3000, 10);
            Func_0200181c(14, 1);
            Engine_EventShowMessageAndWait(14, 0, 10);
            Engine_ActorSetAnimation(12, 3);
            Func_02001826(13, 3);
            Engine_EventWait(20);
            Engine_EventShowMessage(14, 0);
            Call3(Engine_ActorSetSpeed, 14, 0x9999, 0x4ccc);
            *(u8 *)(Func_02001804(14) + 90) &= 254;
            Call3(Func_02001834, 14, 0x10a, 172);
            Engine_EventWait(1);
            *(u8 *)(Func_02001828(14) + 90) |= 1;
            Engine_EventWait(10);
            Func_02001880(14, 3);
            Engine_EventShowMessageAndWait(14, 0, 10);
            Call2(Engine_MessageShowCentered, 0x177a, 1);
            bump_step(1);
            Engine_ItemShowFound(194, 3);
            Value2(Engine_PartyGiveItem, 194, 0);
            Func_020018ba_a(14, 3);
            Engine_ActorSetAnimation(0, 1);
            Call3(Func_0200190e, 0, 0xc000, 0);
            Call3(Engine_ActorSetSpeed, 14, 0x10000, 0x8000);
            *(u8 *)(Func_020018a2(14) + 90) &= 254;
            Call3(Func_020018ce, 14, 0x106, 156);
            Engine_EventWait(1);
            {
                u8 *record = Func_020018c2(14);
                u8 value = record[90] | 1;

                record[90] = value;
            }
            Engine_EventWait(20);
            Func_02001928(12, 2);
            Engine_EventShowMessageAndWait(12, 0, 10);
            Engine_ActorSetAnimation(12, 3);
            Engine_ActorSetAnimation(13, 3);
            Func_0200193a(14, 3);
            base5_20091c0 = (s32)Data_020091c0;
            Call3(Func_02001960, 12, 0x10000, base5_20091c0);
            Call3(Func_0200196c, 13, 0x10000, base5_20091c0);
            Call3(Func_02001978, 14, 0x10000, base5_20091c0);
            Call1(Func_020018f6, 0x849);
            Engine_EventEnd();
        }
    }
}

s32 Scene_DispatchPuzzleEvent(void)
{
    extern u8 Data_03001ebc[];

    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x100;
    if (Data_02000240[224] == (s32)&Value_0000001e) {
        Func_0200119c();
    } else {
        if (Data_02000240[224] == (s32)&Value_00000023) {
            Func_02001398();
            Call2_02000890(Func_020018e2, 0x2008ed9, 0xc80);
        } else {
            if (Data_02000240[224] == (s32)&Value_00000020) {
                Func_020015de();
            }
        }
    }
    return 0;
}

void FieldScene_RunScene38b_020008f0(void)
{
    extern u8 Data_03001ebc[];


    struct FieldActor *record;
    s16 sub_state;

    if (Value1_020008f0(Func_0200198e, 0x845) != 0) {
        ((void (*)())Func_02001a04)(9, 0, 0);
        Call3_020008f0(Func_02001a60, 14, 0x3000, 0);
        Call3_020008f0(Func_02001a6c, 15, 0x5000, 0);
    } else {
        record = Func_020019f4(9);
        Func_020019b2(record, 0);
        Engine_ActorSetPosition(21, 0, 0);
    }
    record = Func_02001a0a(8);
    record->scale_y = 0x18000;
    {
        s32 off = 450;
        sub_state = *(s16 *)((u8 *)Data_02000240 + off);
    }
    if (sub_state == 10) {
        Engine_ActorSetPosition(8, 0, 0);
    } else {
        if (sub_state == 9) {
            Call1_020008f0(Engine_GameFlagClear, 0x12f);
        }
    }
    if (Value1_020008f0(Func_02001a04_a, 0x109) == 0) {
        {
            s32 off = 450;
            sub_state = *(s16 *)((u8 *)Data_02000240 + off);
        }
        if (sub_state == 11) {
            Call3_020008f0(Engine_ActorSetPosition, 20, 0xf80000, 0xd80000);
        }
    }
    Func_0200135e();
    if (Value1_020008f0(Func_02001a30, 0x84a) != 0) {
        if (Value1_020008f0(Func_02001a3a, 0x84b) == 0) {
            Call1_020008f0(Func_02001a4e, 0x304);
        }
    }
}

void Scene_UpdatePuzzleActors(void)
{
    extern u8 Data_03001ebc[];

    s32 p10;
    s32 p9;
    s32 rec7;
    s32 record;
    s32 p6;
    s32 row;

    rec7 = Value1_02000890(Func_02001aac, 0);
    record = Value1_02000890(Func_02001ab4, 20);
    row = *(s32 *)(record + 16) >> 20;
    p9 = (*(s32 *)(rec7 + 8) >> 20);
    p10 = (*(s32 *)(rec7 + 16) >> 20);
    p6 = *(s32 *)(record + 8);
    Call6(Engine_MapCopyCellAttributes, 15, 11, 3, 1, 15, 12);
    Call6(Engine_MapCopyCellAttributes, 15, 11, 3, 1, 15, 13);
    Call6(Engine_MapCopyCellAttributes, 15, 11, 3, 1, 15, 14);
    Call6(Engine_MapCopyCellAttributes, 1, 0, 1, 1, (p6 >> 20), row);
    if (((s32)p6 >> 20) == 16) {
        if (row == 13) {
            goto L_02000a60;
        }
    }
    Call6(Engine_MapCopyCellAttributes, 0, 0, 1, 1, 16, 13);
    L_02000a60:;
    if (p9 == 16) {
        if (p10 == 13) {
            Engine_EventBegin();
            Call3(Engine_ActorShowEmote, 0, 0x100, 20);
            Call3(Engine_ActorSetSpeed, 0, 0x20000, 0x10000);
            Func_02001baa(0, 6, 0);
            if (row == 13) {
                Call3(Func_02001b82, 0, 0x106, 196);
                Call3(Func_02001bfe, 0, 0x4000, 20);
            } else {
                Call3(Func_02001b9c_a, 0, 0x11e, 218);
                Call3(Func_02001c18, 0, 0x8000, 20);
            }
            Engine_EventEnd();
        }
    }
}

void FieldScene_RunScene38bSequenceA(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    struct FieldActor *rec;
    s32 rec7;
    struct FieldActor *rec8;
    s32 record;

    rec8 = Value1_02000ae0(Func_02001bb8, 10);
    rec = Value1_02000ae0(Func_02001bc0, 11);
    record = Func_02001bc8(8);
    Func_02001b86(record, 0);
    rec7 = Value1_02000ae0(Func_02001b9c, 0x845);
    if (rec7 != 0) {
        Engine_ActorSetPosition(9, 0, 0);
        Engine_ActorSetPosition(10, 0, 0);
        Engine_ActorSetPosition(11, 0, 0);
        Call6(Engine_MapCopyCellsTo, 56, 15, 40, 15, 1, 2);
        Call6(Func_02001bc8_a, 26, 15, 1, 3, 10, 15);
        if (Value1_02000ae0(Func_02001bee, 0x849) == 0) {
            if (Value1_02000ae0(Func_02001bf8, 0x848) != 0) {
                goto L_02000c92;
            }
            ((void (*)())Func_02001c70)(14, 0, 0);
        }
        Call3_020008f0(Func_02001ccc, 12, 0xd000, 0);
        Call3_020008f0(Func_02001cd8, 13, 0xb000, 0);
    } else {
        Engine_ActorSetPosition(12, 0, 0);
        Engine_ActorSetPosition(13, 0, 0);
        Engine_ActorSetPosition(14, 0, 0);
        record = Func_02001c7e(9);
        Func_02001c3c(record, 0);
        record = Func_02001c8a(10);
        Func_02001c48(record, 0);
        record = Func_02001c96(11);
        Func_02001c54(record, 0);
        rec8->motion_flags = rec7;
        record = Value1_02000ae0(Func_02001c70_a, 0x881);
        if (record != 0) {
            *(u8 *)(Func_02001cb2(9) + 89) |= 16;
            *(u8 *)(Func_02001cc2(16) + 89) |= 16;
            *(u8 *)(Func_02001cd0(11) + 89) |= 16;
            Call3_020008f0(Engine_ActorSetPosition, 16, 0x8e0000, 0x9c0000);
            record = Func_02001cec(16);
            Func_02001caa(record, 0);
            Call3_020008f0(Engine_ActorSetPosition, 10, 0x8e0000, 0x9c0000);
            rec8->sprite->rotation = 0x4000;
            rec8->y.fixed += -0x80000;
            if (Value1_02000ae0(Func_02001cde, 0x848) != 0) {
                Call3_020008f0(Engine_ActorSetPosition, 11, 0x840000, 0xba0000);
                goto L_02000c92;
            }
            Call3_020008f0(Engine_ActorSetPosition, 11, 0x580000, 0xc40000);
            Engine_ActorSetSpritePriority(11, 3);
            rec->collision_flags |= 4;
        } else {
            rec8->y.fixed = 0x200000;
            rec->motion_flags = record;
            rec->y.fixed = 0x300000;
        }
    }
    L_02000c92:;
    Func_02001948();
}

/*
 * Ten (x, z) tile pairs, held in the overlay's own writable image.  Overlay
 * data lives in EWRAM and is deliberately not const.
 */

/*
 * Slot accessor: Func_02001d90(slot) returns the actor record, or NULL.
 * Typed as a byte pointer so the +0x08 and +0x10 field reads are explicit.
 */

/*
 * The six-argument renderer ABI: four register arguments plus two stack
 * words, here the tile x and tile z of the cell being repainted.  The two
 * names are separate per-site call words that reach the same renderer.
 */

/*
 * Repaint ten fixed collision cells and then actor 8's own cell.  The
 * 92-byte owner includes the alignment halfword and the single pool word
 * that follows the code; that word holds the address of Data_02009d28, which
 * is in-image data rather than a RAM global.  The two renderer calls must
 * keep their separate call words -- naming one renderer for both changes the
 * displacement emitted at each site.
 */
void ActorPresentation_RepaintTenCellsAndActorEightCell(void)
{
    u8 *actor;
    s32 tx;
    s32 tz;
    u32 i;

    /*
     * Slot 8 is the scene's own actor; the accessor result is not
     * null-checked here.  The 20-bit shift is one signed arithmetic shift:
     * 16 takes the fixed-point coordinate to pixels, the further 4 take it
     * to the 16-pixel tile grid.
     */
    actor = Func_02001d90(8);
    tx = *(s32 *)(actor + 0x08) >> 20;
    tz = *(s32 *)(actor + 0x10) >> 20;

    /*
     * Ten fixed cells from the table, then the actor's own cell.  The table
     * is walked by the byte index itself rather than by a 0..9 counter
     * scaled by two, so the loop steps the byte offset directly.
     */
    for (i = 0; i < 20; i += 2) {
        s32 x = (s32)Data_02009d28[i];
        s32 z = (s32)Data_02009d28[i + 1];
        Engine_MapCopyCellAttributes(1, 0, 1, 1, x, z);
    }

    /*
     * The same repaint with 0 rather than 1 in the first argument.  What
     * that selector chooses is not established.
     */
    Engine_MapCopyCellAttributes(0, 0, 1, 1, tx, tz);
}

void FieldScene_RunScene38b_02000d10(void)
{
    extern u8 Data_03001ebc[];


    s32 arg0;
    s32 rec7;
    s32 record;

    *(s32 *)((*(u8 **)Data_03001ebc + 0x1c0)) = 0x204;
    Func_02001cc2_a();
    record = ReadU16Elem((u16 *)Data_02000240, 225);
    if ((u32)((record - 3) << 16) <= 0x10000) {
        if (Value1_02000d10(Func_02001dd6, 0x109) == 0) {
            rec7 = Value1_02000d10(Func_02001e18, 0);
            Engine_EventBegin();
            arg0 = *(s32 *)(rec7 + 8);
            *(s32 *)(rec7 + 12) = 0x100000;
            Call4_020008f0(Engine_CameraMoveTo, arg0, 0x100000, *(s32 *)(rec7 + 16), 0);
            Engine_MapRedraw();
            Engine_EventEnd();
            Engine_TaskWait(1);
        }
    }
}

s32 *SceneActor_FindAtTileXZ(s32 x, s32 z)
{
    extern u8 *Data_03001ebc;

    s32 **tbl = (s32 **)(Data_03001ebc + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = tbl[i];

        if (x == (p[2] >> 20) && z == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

void OverlayObject_SpawnKind24AtActor(u8 *actor)
{
    u8 *obj;
    u8 *child;
    s32 mask;

    obj = Func_02001f90(24,
                            *(s32 *)(actor + 0x08),
                            *(s32 *)(actor + 0x0c),
                            *(s32 *)(actor + 0x10));
    if (obj == 0)
        return;

    child = *(u8 **)(obj + 0x50);
    Func_02001f96(obj, Data_02009d7c);
    *(u8 *)(obj + 0x55) = 0;
    *(u8 *)(obj + 0x22) = 1;
    *(u8 *)(obj + 0x23) = 2;
    if (child == 0)
        return;

    Func_02001fa8(child, 2);
    *(u8 *)(child + 0x26) = 0;
    mask = 13;
    mask = -mask;
    *(u8 *)(child + 5) = (u8)((*(u8 *)(child + 5) & mask) | 4);
    *(u8 *)(child + 9) |= 12;
}

void FieldScene_DrawTilesByActor8Row(void)
{
    u8 *actor;
    actor = Func_02002078(8);
    if (actor == 0)
        return;

    {
        s32 tile_z = *(s32 *)(actor + 0x10) >> 20;
        if (tile_z == 6)
            Engine_MapCopyCellAttributes(2, 0, 1, 1, 14, 6);
        else {
            s32 x = 14;
            s32 z = 6;
            Engine_MapCopyCellAttributes(0, 0, 1, 1, x, z);
        }
    }

    {
        s32 tile_z = *(s32 *)(actor + 0x10) >> 20;
        if (tile_z == 9)
            Engine_MapCopyCellAttributes(2, 0, 1, 1, 14, 9);
        else {
            s32 x = 14;
            s32 z = 9;
            Engine_MapCopyCellAttributes(1, 0, 1, 1, x, z);
        }
    }
}

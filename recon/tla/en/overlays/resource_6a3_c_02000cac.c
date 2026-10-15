/* Draft, differ 2 of 1568: the scene in which actor 4 meets 5, 6, 7 and 28 and the group is
 * placed for the arrival (arg selects the two layouts). Written as calls through I_ forwarding
 * inlines (they keep each call's own constants; the 11-argument metatile calls must go through
 * one too, or the shared constants take other registers: differ 154), with the angle 0x2000 as
 * one variable (the native holds it in r6) and one-pass blocks where the native starts a new
 * scheduling region (found by single-statement search: differ 82 without them). Left: the arm
 * call after Func_0200489c(2, 0, 5) sets r0 before r1 and r2 (the native sets r0 last).
 * Tried: that call direct or with 0x2000, its neighbours with and without one-pass blocks,
 * an empty block before Func_0200489c(2, 0, 5) (2 to 6), 4 minutes of alchemy permute. */
#include "../../../../games/tla/src/field/overlay_6a3/s0b48.c"

extern const u8 MsgAhhhh2[];
extern u32 Data_020056ec[];
extern u32 Data_02005748[];
extern u32 Data_0200578c[];
extern u32 Data_020057d0[];
extern u32 Data_02005800[];
extern u32 Data_0200d830[];
extern u32 Data_02005860[];
extern u32 Data_02005890[];
extern u32 Data_020058d8[];
extern u32 Data_02005920[];
extern u32 Data_02005950[];
extern u32 Data_020049cc[];
extern u32 Data_020049dc[];
extern u32 Data_020049ec[];
extern u32 Data_020049fc[];
extern u32 Data_02004a18[];
extern u32 Data_02004a34[];
extern u32 Data_02004a50[];
extern u32 Data_0200599c[];

void Func_020047d4(void);
void Func_020047dc(void);
void Func_0200488c(const u8 *message);
void Func_0200489c(s32 actor, s32 a, s32 b);
void Func_020048b4(s32 actor, s32 a);
void Func_020048bc(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k);
struct ObjectRuntime *Func_020048cc(s32 actor, s32 emote, s32 frames);
void Func_020048d4(s32 actor, s32 a);
void Func_020048e4(s32 a, s32 b);
void Func_020048f4(void);
void Func_0200494c(s32 a);
void Func_02004964(s32 actor, s32 x, s32 z, s32 angle);
void Func_020049c4(s32 cue);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void ObjectMotion_EnableActionAndResetMotion(s32 id);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void ObjectMotion_CommitPositionAndActivate(s32 id, s32 x, s32 z);
void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void ObjectMotion_OffsetPositionAndReset(s32 id, s32 dx, s32 dz);
void ObjectMotion_SetVariantCallback(s32 id, s32 variant);
void ObjectMotion_SetAngleToward(s32 id, s32 target, s32 flags);
void ObjectMotion_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
void ObjectMotion_ResetAndSetPosition(s32 id, s32 x, s32 z);
void Object_AttachWorkTargetToObject(s32 id, s32 mode);
void Object_LinkPair(s32 a, s32 b, s32 c);
void Motion_SetVarCbAndRefresh(s32 id, s32 variant);
void Motion_CamBounds(s32 x, s32 y, s32 z, s32 d);
s32 UiText_OpenMessageAtObject(s32 speaker, s32 flags);
s32 Inventory_PromptAndSetObjectMode(s32 id, s32 flags);

static __inline__ void I_Func_0200494c(s32 a)
{
    Func_0200494c(a);
}

static __inline__ void I_Func_0200488c(const u8 *message)
{
    Func_0200488c(message);
}

static __inline__ void I_Func_020049c4(s32 cue)
{
    Func_020049c4(cue);
}

static __inline__ void I_Battle_WaitMode0(s32 frames)
{
    Battle_WaitMode0(frames);
}

static __inline__ void I_ObjectMotion_EnableActionAndSetCallback(s32 id, const void *script)
{
    ObjectMotion_EnableActionAndSetCallback(id, script);
}

static __inline__ void I_Func_020048bc(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k)
{
    Func_020048bc(a, b, c, d, e, f, g, h, i, j, k);
}

static __inline__ void I_ObjectMotion_EnableActionAndResetMotion(s32 id)
{
    ObjectMotion_EnableActionAndResetMotion(id);
}

static __inline__ void I_ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags)
{
    ObjectMotion_ArmCallback(id, angle, flags);
}

static __inline__ void I_Func_02004964(s32 actor, s32 x, s32 z, s32 angle)
{
    Func_02004964(actor, x, z, angle);
}

static __inline__ void I_Object_AttachWorkTargetToObject(s32 id, s32 mode)
{
    Object_AttachWorkTargetToObject(id, mode);
}

static __inline__ void I_ObjectMotion_CommitCurrentPositionAndActivate(s32 id)
{
    ObjectMotion_CommitCurrentPositionAndActivate(id);
}

static __inline__ struct ObjectRuntime * I_Func_020048cc(s32 actor, s32 emote, s32 frames)
{
    return Func_020048cc(actor, emote, frames);
}

static __inline__ void I_Func_0200489c(s32 actor, s32 a, s32 b)
{
    Func_0200489c(actor, a, b);
}

static __inline__ s32 I_UiText_OpenMessageAtObject(s32 speaker, s32 flags)
{
    return UiText_OpenMessageAtObject(speaker, flags);
}

static __inline__ s32 I_Inventory_PromptAndSetObjectMode(s32 id, s32 flags)
{
    return Inventory_PromptAndSetObjectMode(id, flags);
}

static __inline__ void I_Object_SetModeById(u32 a0, s32 a1)
{
    Object_SetModeById(a0, a1);
}

static __inline__ void I_Motion_SetVarCbAndRefresh(s32 id, s32 variant)
{
    Motion_SetVarCbAndRefresh(id, variant);
}

static __inline__ void I_ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration)
{
    ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static __inline__ void I_Func_020048e4(s32 a, s32 b)
{
    Func_020048e4(a, b);
}

static __inline__ void I_Motion_CamBounds(s32 x, s32 y, s32 z, s32 d)
{
    Motion_CamBounds(x, y, z, d);
}

static __inline__ void I_Object_RefreshSelectorById(u32 a0)
{
    Object_RefreshSelectorById(a0);
}

static __inline__ void I_Func_0200484c(s32 id, s32 x, s32 z, s32 angle)
{
    Func_0200484c(id, x, z, angle);
}

static __inline__ void I_Func_020048b4(s32 actor, s32 a)
{
    Func_020048b4(actor, a);
}

static __inline__ void I_ObjectMotion_SetVariantCallback(s32 id, s32 variant)
{
    ObjectMotion_SetVariantCallback(id, variant);
}

static __inline__ void I_Func_020048d4(s32 actor, s32 a)
{
    Func_020048d4(actor, a);
}

static __inline__ struct ObjectRuntime * I_Object_GetById(s32 id)
{
    return Object_GetById(id);
}

static __inline__ void I_ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz)
{
    ObjectMotion_OffsetPositionAndResetMotion(id, dx, dz);
}

static __inline__ void I_Object_LinkPair(s32 a, s32 b, s32 c)
{
    Object_LinkPair(a, b, c);
}

static __inline__ void I_ObjectMotion_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z)
{
    ObjectMotion_ResetAndSetPositionInMode2(id, x, z);
}

static __inline__ void I_ObjectMotion_CommitPositionAndActivate(s32 id, s32 x, s32 z)
{
    ObjectMotion_CommitPositionAndActivate(id, x, z);
}

static __inline__ void I_ObjectMotion_OffsetPositionAndReset(s32 id, s32 dx, s32 dz)
{
    ObjectMotion_OffsetPositionAndReset(id, dx, dz);
}

static __inline__ void I_ObjectMotion_SetAngleToward(s32 id, s32 target, s32 flags)
{
    ObjectMotion_SetAngleToward(id, target, flags);
}

static __inline__ void I_ObjectMotion_ResetAndSetPosition(s32 id, s32 x, s32 z)
{
    ObjectMotion_ResetAndSetPosition(id, x, z);
}

void Func_02000cac(s32 arg)
{
    struct ObjectRuntime *object;
    s32 angle;

    Func_020047d4();
    I_Func_0200494c(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    I_Func_0200488c(MsgAhhhh2);
    I_Func_020049c4(124);
    I_Battle_WaitMode0(26);
    I_Func_020049c4(217);
    I_ObjectMotion_EnableActionAndSetCallback(4, Data_020056ec);
    if (arg != 0) {
        I_Func_020048bc(3, 20, 9, 15, 8, 1, 20, 2, 15, 1, 25);
        I_Func_020049c4(310);
        I_Battle_WaitMode0(30);
        I_Func_020049c4(144);
        I_Func_020048bc(3, 21, 9, 16, 8, 1, 21, 2, 16, 1, 0);
        I_Func_020048bc(0, 20, 9, 15, 8, 2, 20, 2, 15, 1, 0);
    } else {
        I_Func_020048bc(3, 5, 9, 0, 8, 1, 5, 2, 0, 1, 25);
        I_Func_020049c4(310);
        I_Battle_WaitMode0(30);
        I_Func_020049c4(144);
        I_Func_020048bc(3, 5, 9, 0, 8, 1, 5, 2, 0, 1, 0);
        I_Func_020048bc(0, 5, 9, 0, 8, 2, 5, 2, 0, 1, 0);
    }
    I_ObjectMotion_EnableActionAndResetMotion(4);
    I_ObjectMotion_ArmCallback(4, 0x4000, 0);
    I_Func_02004964(7, 16, 0, 0x8000);
    I_Func_02004964(28, -16, 0, 0);
    I_Func_02004964(6, 16, 24, 0x8000);
    I_Func_02004964(5, -16, 24, 0);
    I_Object_AttachWorkTargetToObject(5, 1);
    I_ObjectMotion_CommitCurrentPositionAndActivate(5);
    I_Func_020048cc(28, 0x102, 30);
    I_Func_0200489c(28, 0, 5);
    I_ObjectMotion_ArmCallback(5, 0xc000, 0);
    I_ObjectMotion_ArmCallback(6, 0xb000, 0);
    I_Func_020048cc(7, 0x101, 30);
    I_ObjectMotion_ArmCallback(5, 0xd000, 0);
    I_UiText_OpenMessageAtObject(7, 0);
    I_ObjectMotion_ArmCallback(4, 0, 0);
    if (Inventory_PromptAndSetObjectMode(4, 0) == 0) {
        I_Object_SetModeById(28, 3);
        I_Battle_WaitMode0(20);
        I_Battle_WaitMode0(10);
        I_Func_0200489c(28, 0, 5);
        (*(u16 *)((u8 *)Ram_HeapSlots->event_work + 0x1c4))++;
    } else {
        I_Object_SetModeById(28, 4);
        I_Battle_WaitMode0(33);
        (*(u16 *)((u8 *)Ram_HeapSlots->event_work + 0x1c4))++;
        I_Func_0200489c(28, 0, 5);
    }
    I_Motion_SetVarCbAndRefresh(5, 2);
    I_ObjectMotion_ArmCallback(4, 0x4000, 0);
    I_Battle_WaitMode0(5);
    I_Func_0200489c(5, 0, 5);
    I_Func_020048cc(6, 0x101, 30);
    I_Func_0200489c(6, 0, 5);
    I_Func_020048cc(4, 0x102, 30);
    I_ObjectMotion_SetSpeedParameters(4, 0x00013333, 0x9999);
    ObjectMotion_EnableActionAndSetCallback(4, arg != 0 ? Data_02005748 : Data_0200578c);
    I_Battle_WaitMode0(30);
    I_ObjectMotion_ArmCallback(6, 0x8000, 0);
    I_ObjectMotion_ArmCallback(5, 0, 0);
    I_ObjectMotion_ArmCallback(28, 0, 0);
    I_ObjectMotion_ArmCallback(7, 0x8000, 0);
    I_Battle_WaitMode0(10);
    I_Object_SetModeById(6, 3);
    I_Object_SetModeById(7, 3);
    I_Object_SetModeById(5, 3);
    I_Object_SetModeById(28, 3);
    I_Battle_WaitMode0(30);
    I_ObjectMotion_SetSpeedParameters(5, 0x10000, 0x8000);
    I_ObjectMotion_SetSpeedParameters(7, 0x10000, 0x8000);
    I_ObjectMotion_SetSpeedParameters(6, 0x10000, 0x8000);
    I_ObjectMotion_SetSpeedParameters(28, 0xcccc, 0x6666);
    ObjectMotion_EnableActionAndSetCallback(28, arg != 0 ? Data_020057d0 : Data_02005800);
    ObjectMotion_EnableActionAndSetCallback(7, arg != 0 ? Data_0200d830 : Data_02005860);
    ObjectMotion_EnableActionAndSetCallback(5, arg != 0 ? Data_02005890 : Data_020058d8);
    ObjectMotion_EnableActionAndSetCallback(6, arg != 0 ? Data_02005920 : Data_02005950);
    I_Func_020048e4(0x8000, 0x1000);
    if (arg != 0) {
        I_Motion_CamBounds(0x02a50000, 0xffc00000, 0x011f0000, 1);
        Func_020048f4();
    } else {
        I_Motion_CamBounds(0x03b60000, 0xffc00000, 0x011f0000, 1);
        Func_020048f4();
    }
    I_Object_RefreshSelectorById(4);
    I_Object_RefreshSelectorById(7);
    I_Object_RefreshSelectorById(5);
    I_Object_RefreshSelectorById(6);
    I_Object_RefreshSelectorById(28);
    I_Battle_WaitMode0(20);
    angle = 0x2000;
    I_Func_0200484c(2, 0x02e50000, 0xe80000, angle);
    I_Func_020048e4(0x40000, 0x8000);
    I_Motion_CamBounds(0x3200000, 0, 0x10e0000, 1);
    Func_020048f4();
    I_Func_020049c4(39);
    I_Battle_WaitMode0(50);
    I_Motion_SetVarCbAndRefresh(0, 2);
    I_Func_0200489c(0, 0, 5);
    I_Object_SetModeById(3, 0);
    I_Func_020048b4(3, 0xa000);
    I_Func_0200489c(3, 0, 5);
    I_Func_020048b4(3, angle);
    I_Object_SetModeById(3, 9);
    I_Func_0200489c(3, 0, 5);
    I_Motion_SetVarCbAndRefresh(1, 2);
    I_Battle_WaitMode0(5);
    I_Func_0200489c(1, 0, 5);
    I_ObjectMotion_SetVariantCallback(3, 3);
    I_Func_020048d4(3, 258);
    I_Battle_WaitMode0(40);
    I_Func_0200489c(3, 0, 5);
    I_Motion_SetVarCbAndRefresh(1, 2);
    I_Battle_WaitMode0(5);
    I_Func_0200489c(1, 0, 5);
    Object_GetById(3)->action_flags &= 0xfe;
    I_ObjectMotion_SetSpeedParameters(3, 0x1999, 0xccc);
    I_ObjectMotion_OffsetPositionAndResetMotion(3, 4, 2);
    I_ObjectMotion_CommitCurrentPositionAndActivate(3);
    I_Object_SetModeById(3, 9);
    I_Motion_SetVarCbAndRefresh(3, 2);
    I_ObjectMotion_OffsetPositionAndResetMotion(3, -2, 2);
    I_ObjectMotion_CommitCurrentPositionAndActivate(3);
    I_Object_SetModeById(3, 9);
    I_Motion_SetVarCbAndRefresh(3, 2);
    I_ObjectMotion_OffsetPositionAndResetMotion(3, 2, 1);
    I_ObjectMotion_CommitCurrentPositionAndActivate(3);
    I_Object_SetModeById(3, 9);
    I_Motion_SetVarCbAndRefresh(3, 2);
    I_ObjectMotion_OffsetPositionAndResetMotion(3, -2, -1);
    I_ObjectMotion_CommitCurrentPositionAndActivate(3);
    I_Object_SetModeById(3, 9);
    I_Motion_SetVarCbAndRefresh(3, 2);
    I_Battle_WaitMode0(40);
    I_ObjectMotion_OffsetPositionAndResetMotion(3, 0, -2);
    I_ObjectMotion_CommitCurrentPositionAndActivate(3);
    I_ObjectMotion_SetSpeedParameters(3, 0x20000, 0x10000);
    do { I_Object_SetModeById(3, 4); } while (0);
    do { I_Battle_WaitMode0(33); } while (0);
    I_Func_0200489c(3, 0, 5);
    Object_GetById(3)->action_flags |= 1;
    I_ObjectMotion_OffsetPositionAndResetMotion(3, -2, -8);
    I_ObjectMotion_CommitCurrentPositionAndActivate(3);
    I_Object_SetModeById(3, 1);
    I_Func_020048b4(3, 0xa000);
    I_Battle_WaitMode0(10);
    I_Func_0200489c(3, 0, 5);
    I_Object_SetModeById(0, 1);
    I_Object_SetModeById(2, 1);
    I_Object_LinkPair(0, 2, 0);
    I_Battle_WaitMode0(10);
    I_Func_020048cc(0, 0x102, 30);
    do { I_Func_0200489c(0, 0, 5); } while (0);
    I_Object_SetModeById(2, 3);
    I_Battle_WaitMode0(20);
    I_Battle_WaitMode0(10);
    I_ObjectMotion_EnableActionAndSetCallback(3, Data_02005548);
    I_Func_0200484c(26, 0x02990000, 0x800000, angle);
    I_Func_0200484c(27, 0x02810000, 0x800000, angle);
    I_Func_020049c4(78);
    I_Func_0200489c(26, 0, 5);
    I_Func_020048cc(0, 0x100, 0);
    I_Func_020048cc(2, 0x100, 30);
    I_ObjectMotion_ArmCallback(0, 0xa000, 0);
    I_Func_020048b4(2, 0xa000);
    I_Battle_WaitMode0(10);
    I_Motion_CamBounds(0x2d00000, 0, 0xd20000, 1);
    I_ObjectMotion_SetSpeedParameters(26, 0x4000, angle);
    I_ObjectMotion_SetSpeedParameters(27, 0x4000, angle);
    I_ObjectMotion_ResetAndSetPositionInMode2(26, 714, 193);
    I_ObjectMotion_ResetAndSetPositionInMode2(27, 698, 209);
    I_ObjectMotion_EnableActionAndSetCallback(26, Data_020049cc);
    I_ObjectMotion_EnableActionAndSetCallback(27, Data_020049dc);
    I_Battle_WaitMode0(20);
    I_Func_020048cc(2, 0x102, 30);
    I_Func_0200489c(2, 0, 5);
    I_Func_020049c4(34);
    I_Func_020048cc(27, 0x108, 30);
    I_Func_0200489c(27, 0, 5);
    I_Func_020048cc(26, 0x107, 30);
    I_Func_0200489c(26, 0, 5);
    I_Func_020048cc(0, 0x107, 30);
    I_Func_0200489c(0, 0, 5);
    I_Object_RefreshSelectorById(26);
    do { I_Object_RefreshSelectorById(27); } while (0);
    I_Object_SetModeById(27, 4);
    I_Battle_WaitMode0(33);
    I_Func_0200489c(27, 0, 5);
    I_Func_020048cc(2, 0x100, 30);
    I_Func_0200489c(2, 0, 5);
    I_Func_020048cc(26, 0x107, 30);
    I_Func_0200489c(26, 0, 5);
    I_Func_020048cc(2, 0x101, 0);
    I_Func_020048cc(0, 0x101, 30);
    I_Object_LinkPair(0, 2, 0);
    I_Battle_WaitMode0(10);
    I_Func_020048cc(0, 0x106, 30);
    I_ObjectMotion_ArmCallback(2, 0xa000, 0);
    I_Func_020048b4(0, 0xa000);
    do { I_Func_0200489c(0, 0, 5); } while (0);
    I_Object_SetModeById(27, 3);
    I_Battle_WaitMode0(20);
    I_Battle_WaitMode0(10);
    I_Func_0200489c(27, 0, 5);
    I_Func_020048cc(2, 0x102, 30);
    I_Func_0200489c(2, 0, 5);
    I_Func_020048b4(26, 0xa000);
    I_Battle_WaitMode0(10);
    I_Func_0200489c(26, 0, 5);
    I_Motion_SetVarCbAndRefresh(26, 2);
    I_Battle_WaitMode0(5);
    I_Func_0200489c(26, 0, 5);
    I_Func_020048cc(0, 0x109, 30);
    I_Func_0200489c(0, 0, 5);
    I_Motion_SetVarCbAndRefresh(27, 2);
    I_Battle_WaitMode0(5);
    I_Func_0200489c(27, 0, 5);
    I_Func_020048b4(26, angle);
    I_Battle_WaitMode0(10);
    I_Func_0200489c(26, 0, 5);
    I_Func_020048cc(2, 0x107, 30);
    I_Func_0200489c(2, 0, 5);
    I_Func_020048cc(27, 0x108, 30);
    I_Func_0200489c(27, 0, 5);
    I_Func_020048cc(0, 0x103, 30);
    I_Func_0200489c(0, 0, 5);
    I_Func_020048cc(26, 0x108, 30);
    do { I_Func_0200489c(26, 0, 5); } while (0);
    I_Object_SetModeById(27, 3);
    I_Battle_WaitMode0(20);
    I_Battle_WaitMode0(10);
    I_Func_0200489c(27, 0, 5);
    I_Motion_SetVarCbAndRefresh(2, 2);
    I_Battle_WaitMode0(5);
    I_Func_0200489c(2, 0, 5);
    I_Object_SetModeById(27, 4);
    do { I_Object_SetModeById(26, 4); } while (0);
    do { I_Battle_WaitMode0(33); } while (0);
    I_Func_020048cc(2, 0x107, 30);
    I_Func_0200489c(2, 0, 5);
    I_Func_020048cc(26, 0x100, 30);
    I_Func_0200489c(26, 0, 5);
    I_Func_020048cc(27, 0x109, 30);
    I_Func_0200489c(27, 0, 5);
    I_Func_020048cc(0, 0x100, 30);
    I_Func_020048b4(0, angle);
    I_Object_SetModeById(0, 38);
    I_Motion_CamBounds(0x3100000, 0, 0x1120000, 1);
    Func_020048f4();
    I_Battle_WaitMode0(60);
    I_Motion_CamBounds(0x2d00000, 0, 0xd20000, 1);
    Func_020048f4();
    I_Object_SetModeById(0, 1);
    I_Func_020048b4(0, 0xa000);
    I_Battle_WaitMode0(10);
    I_Func_0200489c(0, 0, 5);
    I_Object_LinkPair(26, 27, 0);
    I_Battle_WaitMode0(10);
    I_Func_020048cc(26, 0x108, 0);
    I_Func_020048cc(26, 0x108, 30);
    I_Func_020048cc(2, 0x101, 30);
    do { I_Func_0200489c(2, 0, 5); } while (0);
    I_ObjectMotion_ArmCallback(26, angle, 0);
    do { I_Func_020048b4(27, angle); } while (0);
    I_Object_SetModeById(26, 3);
    I_Battle_WaitMode0(20);
    I_Battle_WaitMode0(10);
    do { I_Func_0200489c(26, 0, 5); } while (0);
    I_Object_SetModeById(27, 4);
    I_Battle_WaitMode0(33);
    I_Func_0200489c(27, 0, 5);
    I_Func_020048cc(2, 0x103, 30);
    I_Func_0200489c(2, 0, 5);
    I_Func_020048cc(26, 0x108, 30);
    I_Func_0200489c(26, 0, 5);
    I_Motion_SetVarCbAndRefresh(0, 2);
    I_Battle_WaitMode0(5);
    I_Func_0200489c(0, 0, 5);
    I_Func_020048cc(27, 0x108, 30);
    I_Func_0200489c(27, 0, 5);
    I_Func_020048b4(2, 0xe000);
    I_Battle_WaitMode0(10);
    I_Func_0200489c(2, 0, 5);
    I_Func_020048cc(26, 0x109, 30);
    do { I_Func_0200489c(26, 0, 5); } while (0);
    I_Object_SetModeById(27, 4);
    do { I_Battle_WaitMode0(33); } while (0);
    I_Func_0200489c(27, 0, 5);
    I_Func_020048b4(27, 0xa000);
    I_Battle_WaitMode0(20);
    I_Func_020048cc(27, 0x101, 30);
    I_ObjectMotion_SetSpeedParameters(27, 0x20000, 0x10000);
    I_ObjectMotion_CommitPositionAndActivate(27, -24, -24);
    I_Battle_WaitMode0(5);
    I_Func_020048b4(27, 0x8000);
    I_Battle_WaitMode0(10);
    I_Func_020048b4(27, 0xc000);
    I_Battle_WaitMode0(10);
    I_Func_020048b4(27, 0);
    I_Battle_WaitMode0(10);
    I_Func_020048b4(27, 0xc000);
    I_Battle_WaitMode0(10);
    I_Func_020048cc(27, 0x102, 30);
    I_Func_0200489c(27, 0, 5);
    I_Func_0200489c(26, 0, 5);
    I_Func_020048b4(27, angle);
    I_Battle_WaitMode0(10);
    I_Func_0200489c(27, 0, 5);
    I_Func_020048b4(26, 0xa000);
    I_Battle_WaitMode0(10);
    I_Func_0200489c(26, 0, 5);
    I_Object_LinkPair(0, 2, 0);
    I_Battle_WaitMode0(10);
    I_Func_020048bc(0, 20, 7, 15, 6, 2, 17, 13, 12, 12, 0);
    I_ObjectMotion_ArmCallback(0, 0xa000, 0);
    I_ObjectMotion_ArmCallback(2, 0xa000, 0);
    I_Func_020048cc(27, 0x103, 30);
    I_Func_0200489c(27, 0, 5);
    I_Func_020048cc(26, 0x107, 30);
    I_Func_0200489c(26, 0, 5);
    I_Func_020048b4(26, angle);
    I_Battle_WaitMode0(10);
    I_Func_020048cc(26, 0x108, 30);
    I_Func_0200489c(26, 0, 5);
    I_Func_020048cc(27, 0x108, 30);
    I_ObjectMotion_OffsetPositionAndReset(27, 24, 24);
    I_ObjectMotion_EnableActionAndSetCallback(27, Data_020049ec);
    I_Func_0200489c(27, 0, 5);
    I_Object_RefreshSelectorById(27);
    I_Object_LinkPair(26, 27, 30);
    I_Object_SetModeById(26, 3);
    do { I_Object_SetModeById(27, 3); } while (0);
    I_Battle_WaitMode0(20);
    I_Battle_WaitMode0(30);
    I_ObjectMotion_SetSpeedParameters(26, 0x30000, 0x18000);
    I_ObjectMotion_SetSpeedParameters(27, 0x30000, 0x18000);
    I_ObjectMotion_SetSpeedParameters(0, 0x30000, 0x18000);
    I_ObjectMotion_SetSpeedParameters(2, 0x30000, 0x18000);
    I_ObjectMotion_ResetAndSetPositionInMode2(26, 737, 202);
    I_ObjectMotion_ResetAndSetPositionInMode2(27, 731, 213);
    I_ObjectMotion_EnableActionAndSetCallback(27, Data_020049fc);
    I_ObjectMotion_EnableActionAndSetCallback(26, Data_02004a18);
    I_Battle_WaitMode0(5);
    I_ObjectMotion_ResetAndSetPositionInMode2(0, 776, 202);
    I_ObjectMotion_ResetAndSetPositionInMode2(2, 731, 232);
    I_ObjectMotion_EnableActionAndSetCallback(0, Data_02004a34);
    I_ObjectMotion_EnableActionAndSetCallback(2, Data_02004a50);
    I_Object_RefreshSelectorById(0);
    I_Object_RefreshSelectorById(2);
    I_Object_RefreshSelectorById(26);
    I_Object_RefreshSelectorById(27);
    I_Battle_WaitMode0(40);
    Func_02000b48();
    I_Battle_WaitMode0(200);
    I_Object_AttachWorkTargetToObject(4, 1);
    I_Battle_WaitMode0(10);
    I_Motion_SetVarCbAndRefresh(28, 2);
    I_Battle_WaitMode0(5);
    I_Func_0200489c(28, 0, 5);
    I_Func_020048cc(6, 0x109, 30);
    I_Func_0200489c(6, 0, 5);
    I_Func_020048cc(7, 0x107, 30);
    I_Func_0200489c(7, 0, 5);
    I_Func_020048cc(5, 0x109, 30);
    I_Func_0200489c(5, 0, 5);
    I_Func_020048b4(28, 0x4000);
    I_Battle_WaitMode0(10);
    do { I_Func_0200489c(28, 0, 5); } while (0);
    I_Object_SetModeById(4, 3);
    I_Battle_WaitMode0(20);
    I_Battle_WaitMode0(10);
    I_ObjectMotion_SetAngleToward(5, 4, 0);
    I_ObjectMotion_SetAngleToward(7, 4, 0);
    I_ObjectMotion_SetAngleToward(6, 4, 0);
    I_Battle_WaitMode0(20);
    I_Object_SetModeById(7, 3);
    do { I_Object_SetModeById(6, 3); } while (0);
    I_Object_SetModeById(5, 3);
    I_Battle_WaitMode0(20);
    I_Battle_WaitMode0(10);
    I_Object_SetModeById(7, 2);
    object = Object_GetById(4);
    if (object != NULL)
        I_ObjectMotion_ResetAndSetPosition(7, ((s16 *)&object->x)[1], ((s16 *)&object->z)[1]);
    I_Object_SetModeById(6, 2);
    object = Object_GetById(4);
    if (object != NULL)
        I_ObjectMotion_ResetAndSetPosition(6, ((s16 *)&object->x)[1], ((s16 *)&object->z)[1]);
    I_Object_SetModeById(5, 2);
    object = Object_GetById(4);
    if (object != NULL)
        I_ObjectMotion_ResetAndSetPosition(5, ((s16 *)&object->x)[1], ((s16 *)&object->z)[1]);
    I_Object_SetModeById(28, 2);
    object = Object_GetById(4);
    if (object != NULL)
        I_ObjectMotion_ResetAndSetPosition(28, ((s16 *)&object->x)[1], ((s16 *)&object->z)[1]);
    ObjectMotion_EnableActionAndSetCallback(7, Data_0200599c);
    ObjectMotion_EnableActionAndSetCallback(6, Data_0200599c);
    ObjectMotion_EnableActionAndSetCallback(5, Data_0200599c);
    ObjectMotion_EnableActionAndSetCallback(28, Data_0200599c);
    I_Object_RefreshSelectorById(7);
    I_Object_RefreshSelectorById(6);
    I_Object_RefreshSelectorById(5);
    I_Object_RefreshSelectorById(28);
    Func_020047dc();
}

/* Draft of SceneData_SelectTableByThreeFlags, resource_3af at 0x020089fc (split from FIELD/FUNE_KANPAN/MULTI_ENCOUNTER.C).
 * Remaining difference: its messages have catalogue names now and its bytes
 * match the ROM, but it names symbols no link defines (Value_0000093e,
 * Data_0200d9d0, Data_0200da54, Value_00000928, Data_0200d958,
 * Data_0200d778). */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define ObjectMotion_EnableActionAndSetCallback_2(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define Scene_GetRecord_1(args...) Func_020052d2(args)
#define Scene_GetRecord_2(args...) Func_02005336(args)
#define Scene_GetRecord_3(a0) Value1(Func_02005342, a0)
#define Object_NotifyLastActiveOfEvent_1(a0) Call1(Func_02006cda, a0)
#define Scene_GetRecord_1_020029d4(args...) Func_02006d1a(args)
#define Scene_GetRecord_2_020029d4(args...) Func_02006d2c(args)
#define SCENE_PHASE (*(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0))
#define Scene_GetRecord_1_02002b7c(args...) Func_02006e9a(args)
#define Object_NotifyLastActiveOfEvent_1_02002b7c(a0) Call1(Func_02006e96, a0)
#define Scene_GetRecord_2_02002b7c(args...) Func_02006ec0(args)
#define Scene_GetRecord_3_02002b7c(args...) Func_02006eda_a(args)
#define ObjectMotion_SetActionVariant_1(a0, a1) Value2(Engine_ActorSetSpritePriority, a0, a1)
#define Scene_GetRecord_4(args...) Func_02006efc(args)
#define ObjectMotion_ArmCallback_1_02002b7c(a0, a1, a2) Value3(Engine_ActorFaceDirection, a0, a1, a2)
#define Object_NotifyLastActiveOfEvent_1_02003a0c(a0) Call1(Func_02007d20, a0)
#define Scene_GetRecord_1_02003a0c(args...) Func_02007d48(args)
#define Scene_GetRecord_2_02003a0c(args...) Func_02007d62(args)
#define Scene_GetRecord_3_02003a0c(args...) Func_02007d7a(args)
#define Scene_GetRecord_4_02003a0c(args...) Func_02007d94(args)
#define Scene_GetRecord_1_02003c88(a0) Value1(Func_02007fc2, a0)
#define Scene_GetRecord_2_02003c88(a0) Value1(Func_02007fd6, a0)
#define Scene_GetRecord_3_02003c88(a0) Value1(Func_02007fea, a0)
#define Scene_GetRecord_4_02003c88(args...) Func_02008164(args)
#define Scene_GetRecord_5(args...) Func_02008186(args)
#define Scene_GetRecord_6(a0) Value1(Func_02008196, a0)
#define Scene_GetRecord_7(args...) Func_020081b6(args)
#define ACTOR_FLAGS_OFFSET 90
#define Object_NotifyLastActiveOfEvent_1_02003f30(a0) Call1(Func_0200823a, a0)
#define ObjectMotion_ArmCallback_8(a0, a1, a2) Value3(Engine_ActorFaceDirection, a0, a1, a2)
#define SCENE_WORK (*(u8 **)0x03001ebc)
#define SCENE_PHASE_02003f30 (*(s32 *)(SCENE_WORK + 0x1c0))


union Slot {
    s32 w;
    s16 h[2];
};

extern u8 Data_0200c994[];
extern u8 Data_0200cb44[];
extern u8 Data_0200cb64[];
extern u8 Value_0000093e;
extern u8 Value_00000927;
extern u8 Value_00000928;
extern u8 Value_00000911;
extern u8 Value_00000925;
extern u8 Value_00000922;
extern u8 Data_0200d508[];
extern u8 Data_0200cef0[];
extern u8 Data_0200d028[];
extern u8 Data_0200ccf8[];
extern u8 Data_0200cba8[];
extern u8 Data_0200d9d0[];
extern u8 Data_0200da54[];
extern u8 Data_0200d958[];
extern u8 Data_0200d778[];
extern s32 Data_0200db08[];
extern unsigned char Value_00001f00;
extern u8 *Data_03001e70;
extern u32 Data_0200db58;
extern u32 Data_0200db38;
extern u16 Data_0200db30[];
extern u16 Data_0200db40[];
extern s32 Data_0200db90[];
extern u8 Data_0200d160[];
extern u8 Data_00002014[];
extern u8 Data_0200c918[];

s32 BuildMotionCountdown(s32, s16);
u8 *Func_02007692(s32);
s32 Func_02004d98();
s32 Func_02004de0();
s32 Func_02004e40();
s32 Func_02004d38();
void Func_02004764();
void Func_02004796();
void Func_020047da();
void Func_0200489c();
void Func_0200497e();
void Func_02004a04();
void Func_02005114();
u8 *Func_020052d2();
u8 *Func_02005336();
s32 Func_02005342();
s32 Func_020053dc();
s32 Func_02005400();
s32 Func_0200542e();
u8 *Func_0200544e();
void Func_02005510();
s32 Func_02005504();
s32 Func_02005528();
s32 Func_02005556();
u8 *Func_02005576();
void Func_02005638();
void Func_02005b62();
u8 *Func_02005ba0();
s32 Func_02005bac();
u8 *Func_02005bce();
void Func_02005c26();
u8 *Func_02005c64();
u8 *Func_02005c76();
void Func_0200286a(void);
void Func_02006a1c(s32, s32);
void Func_02006a2a(s32, s32);
void Func_02006a38(s32, s32);
void Func_02006a4a(s32, s32);
void Func_02006af2(s32, s32, s32);
void Func_02006afc(s32, s32, s32);
void Func_02006b06(s32, s32, s32);
void Func_02006b10(s32, s32, s32);
void Func_02006b1a(s32, s32, s32);
void Func_02006b24(s32, s32, s32);
void Func_02006b2e(s32, s32, s32);
void Func_02006b38(s32, s32, s32);
s32 *Func_02007aac_b();
s32 *Func_02007a90(s32);
s32 *Func_02007aa0(s32);
s32 *Func_02007ab8(s32);
s32 *Func_02007adc(s32);
s32 *Func_02007ae6(s32);
s32 *Func_02007aee(s32);
s32 *Func_02007af6(s32);
s32 *Func_02007b08(s32);
s32 *Func_02007b14(s32);
s32 *Func_02007b20(s32);
s32 *Func_02007b2c_a();
s32 *Func_02007b38(s32);
void Func_02007c44(s32, s32);
void Func_02007c4c(s32, s32);
void Func_02007c54(s32, s32);
void Func_02007c5c(s32, s32);
void Func_02007c64(s32, s32);
void Func_02007c6c(s32, s32);
void Func_02007c74(s32, s32);
u8 *Func_02007b8a(s32);
u8 *Func_02007b96(s32);
u8 *Func_02007ba0(s32);
u8 *Func_02007baa(s32);
u8 *Func_02007bb4(s32);
u8 *Func_02007bbe(s32);
u8 *Func_02007bc8(s32);
u8 *Func_02007bd2(s32);
u8 *Func_02005abe(s32);
u8 *Func_02005ad8(s32);
u8 *Func_02005af6(s32);
u8 *Func_02005b16(s32);
u8 *Func_02005b2e(s32);
void Func_02003760();
s32 Func_02005e16();
s32 Func_02005e2e();
void Func_02003818();
void Func_02005e5e();
s32 Func_02005e90();
s32 Func_02005ee4();
s32 Func_02005efc();
void Func_020056ac();
void Func_02006012(s32);
s32 Func_02006036(s32);
void Func_02006476();
s32 Func_02006480_a();
s32 Func_02006480_b();
void Func_02006cda();
u8 *Func_02006d1a();
u8 *Func_02006d2c();
void Func_02006e96();
s32 Func_02006e9a();
s32 Func_02006ec0();
s32 Func_02006eda_a();
s32 Func_02006efc();
u8 *Func_02007596(s32);
u8 *Func_020075a2(s32);
u8 *Func_020075ac(s32);
u8 *Func_020075b6(s32);
s32 *Func_020075f0(s32);
s32 *Func_020075fc(s32);
s32 *Func_02007606(s32);
s32 *Func_02007610(s32);
s32 *Func_02007632(s32);
s32 *Func_0200763c(s32);
s32 *Func_02007644(s32);
s32 *Func_0200764c(s32);
s32 *Func_02007654(s32);
s32 *Func_0200765e(s32);
s32 *Func_02007668(s32);
s32 *Func_02007672(s32);
s32 Func_0200790e_a(s32);
void Func_02007a40(s32, s32);
void Func_02007a54(s32, s32);
void Func_02007a68(s32, s32);
void Func_02007a7c(s32, s32);
void Func_02007d20();
s32 Func_02007d7a();
s32 Func_02007d94();
s32 Func_02007fc2();
s32 Func_02007fd6();
s32 Func_02007fea();
s32 Func_02008164();
u8 *Func_02008186();
s32 Func_02008196();
u8 *Func_020081b6();
void Func_0200823a();
s32 Func_0200854e();

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* The step value differs in the localized scene data. */

/* Offset of a flag byte on an actor record, cleared and set below. */

/* Pointer, held at fixed address 0x03001ebc, to the shared scene work
 * record. The phase/status word lives at offset 0x1c0 of that record. */

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call1_020029d4(void (*f)(), s32 a0)
{
    u8 *Func_02007d48();
    u8 *Func_02007d62();

    f(a0);
}

static __inline__ void Call1_02003a0c(void (*f)(), s32 a0)
{
    extern u8 Data_0200db50[];
    extern u8 Data_0200db60[];
    s32 Func_02007d48();
    s32 Func_02007d62();

    f(a0);
}

/* EN draft. In the German, Spanish, French and Italian overlays this work
 * block sits 0x40 bytes later, and the German event-work pointer 16 bytes
 * later; those editions need their own names for these places. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00006014[];

u8 *SceneData_SelectTableByThreeFlags(void)
{
    if (GameFlag_IsSet((s32)&Value_0000093e))
        return Data_0200d9d0;
    if (GameFlag_IsSet(0x8A0))
        return Data_0200da54;
    if (GameFlag_IsSet((s32)&Value_00000928))
        return Data_0200d958;
    return Data_0200d778;
}

#include "TYPES.H"
#include "FIELD_EVENT.H"

#define BuildMotionCountdown Func_020042e6
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

#include "RESOURCE_3AF_MOTION.H"

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
extern u8 Data_00001d40[];
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

s32 Func_020042e6(s32, s16);
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

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* The step value differs in the localized scene data. */

/* Offset of a flag byte on an actor record, cleared and set below. */

/* Pointer, held at fixed address 0x03001ebc, to the shared scene work
 * record. The phase/status word lives at offset 0x1c0 of that record. */

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

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_020010a0(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_020011c8(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02001a98(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02001b58(void (*f)(), s32 a0)
{
    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02001c14(void (*f)(), s32 a0)
{
    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_020029d4(void (*f)(), s32 a0)
{
    u8 *Func_02007d48();
    u8 *Func_02007d62();

    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02002b7c(void (*f)(), s32 a0)
{
    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02003a0c(void (*f)(), s32 a0)
{
    extern u8 Data_0200db50[];
    extern u8 Data_0200db60[];
    s32 Func_02007d48();
    s32 Func_02007d62();

    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02003c88(void (*f)(), s32 a0)
{
    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02003f30(void (*f)(), s32 a0)
{
    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02004218(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define Data_0200db40 Data_0200db80
#define Data_0200db70 Data_0200dbb0
#define Data_0200db90 Data_0200dbd0
#endif


#if defined(TBS_EDITION_JA)
#define SCENE_STEP_VALUE 0x20ac
#elif defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define SCENE_STEP_VALUE 0x1f05
#else
#define SCENE_STEP_VALUE 0x1f23
#endif

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define Data_0200db50 Data_0200db90
#define Data_0200db60 Data_0200dba0
#define Data_0200d160 Data_0200d1a4
#endif
#if defined(TBS_EDITION_DE)
#define Data_03001ebc Data_03001ecc
#endif

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00006014[];

s32 SceneEffect_UpdateCountdownMotion(struct Resource3afMotion *motion)
{
    switch (motion->countdown) {
    case 6:
        motion->component_a += (s32) 0xFFFFC000;
        motion->component_b += 0x2000;
        break;
    case 4:
        motion->component_a += 0x2000;
        /* The loader relocates the stored pool word to -0x1000. */
        motion->component_b -= 0x1000;
        break;
    case 2:
        motion->component_a += 0x1000;
        motion->component_b += (s32) 0xFFFFF800;
        break;
    case 0:
        motion->component_a = 0x10000;
        motion->component_b = 0x10000;
        motion->countdown =
            (s16)(BuildMotionCountdown(Random_Next(), 90) + 60);
        break;
    }
    motion->countdown--;
    return 1;
}

s32 SceneState_ApplyArgMode1AndReturnZero(s32 a)
{
    Actor_SetSpriteFlags(a, 1);
    return 0;
}

s32 SceneActor_OscillateHeightBetweenLimits(u8 *obj)
{
    s16 *flag = (s16 *)(obj + 0x66);
    s32 val;
    s32 tmp;

    if (*flag != 0) {
        val = *(s32 *)(obj + 0xc) - (((u32)(Random_Next() << 15)) >> 16) - 0x8000;
        *(s32 *)(obj + 0xc) = val;
        if (val >= 0x40000)
            goto done;
        tmp = 0;
    } else {
        val = *(s32 *)(obj + 0xc) + (((u32)(Random_Next() << 15)) >> 16) + 0x8000;
        *(s32 *)(obj + 0xc) = val;
        if (val <= 0xC0000)
            goto done;
        tmp = 1;
    }
    *flag = tmp;
done:
    return 1;
}

s32 SceneActor_SetFacingFromSample(u8 *a)
{
    u32 v = ((u32)(Random_Next() << 5)) >> 16;

    if (v == 6) {
        s32 t = 0xD0;
        *(u16 *)(a + 6) = t << 8;
    } else if (v == 9) {
        s32 t = 0xB0;
        *(u16 *)(a + 6) = t << 8;
    }
    return 1;
}

s32 SceneActor_SetWord28RandomlyOneIn40(u8 *obj)
{
    if ((((u32)(Random_Next() * 40)) >> 16) == 0)
        *(s32 *)(obj + 0x28) = 0x40000;
    return 1;
}

void OverlayObject_DecayFields24And28(u8 *o)
{
    if (*(s32 *)(o + 24) > 0x10000) {
        *(s32 *)(o + 24) += 0xFFFFF800;
        *(s32 *)(o + 28) += 0xFFFFF800;
    }
}

/* Contiguous unnamed leaf-owner run for resource_3af. */
u8 *SceneData_GetTablec994(void)
{
    return Data_0200c994;
}

u8 *SceneData_GetTablecb44(void)
{
    return Data_0200cb44;
}

u8 *SceneData_GetTablecb64(void)
{
    return Data_0200cb64;
}

u8 *SceneData_SelectTableByFlags(void)
{
    s32 v;

    if (GameFlag_IsSet((s32)&Value_0000093e))
        return Data_0200d508;
    if (GameFlag_IsSet((s32)&Value_00000927))
        return Data_0200cef0;
    v = GameFlag_IsSet((s32)&Value_00000928);
    if (v != 0)
        return Data_0200d028;
    if (GameFlag_IsSet((s32)&Value_00000911)) {
        if (GameFlag_IsSet((s32)&Value_00000925)) {
            Data_0200ccf8[0x14E] = v;
            Data_0200ccf8[0x1AE] = 2;
            Data_0200ccf8[0x1C6] = 2;
        } else if (GameFlag_IsSet((s32)&Value_00000922)) {
            Data_0200ccf8[0x1AE] = 1;
            Data_0200ccf8[0x1C6] = 1;
        }
        return Data_0200ccf8;
    }
    return Data_0200cba8;
}

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

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    s32 rec7;
    s32 record;
    s32 shown;

    ((void (*)())Engine_EventBegin)();
    if (GameFlag_IsSet(0x925) != 0) {
        Event_SetMessage(0x1e08);
        Event_ShowMessage(21, 0);
    } else {
        if (GameFlag_IsSet(0x922) != 0) {
            Actor_RunRepeatedMotion(21, 2);
            Event_SetMessage(0x1d6f);
            Event_ShowMessage(21, 0);
            rec7 = Value1(Func_02004d98, 21);
            record = Random_Next();
            shown = ((u32)(90 * record) >> 16) + 60;
            *(u16 *)(rec7 + 100) = shown;
            Engine_ActorEnableActionCallback(21, 0x200c4d8);
        } else {
            Actor_ShowEmote(21, 0x103, 0);
            Actor_StartRepeatedMotion(21, 3);
            Event_SetMessage(0x1d36);
            Event_ShowMessage(21, 0);
        }
    }
    Event_End();
}

void Func_02000af0(void)
{
    s32 rec7;
    s32 record;
    s32 shown;

    ((void (*)())Engine_EventBegin)();
    if (GameFlag_IsSet(0x925) != 0) {
        Event_SetMessage(0x1e09);
        Event_ShowMessage(24, 0);
    } else {
        if (GameFlag_IsSet(0x922) != 0) {
            Actor_RunRepeatedMotion(24, 2);
            Event_SetMessage(0x1d70);
            Event_ShowMessage(24, 0);
            rec7 = Value1(Func_02004e40, 24);
            record = Random_Next();
            shown = ((u32)(90 * record) >> 16) + 60;
            *(u16 *)(rec7 + 100) = shown;
            Engine_ActorEnableActionCallback(24, 0x200c4d8);
        } else {
            Actor_ShowEmote(24, 0x103, 0);
            Actor_StartRepeatedMotion(24, 3);
            Event_SetMessage(0x1d37);
            Event_ShowMessage(24, 0);
        }
    }
    Event_End();
}

/*
 * One-line dialogue callback in overlay resource_3af.  It is installed into
 * the script-record table rather than called.
 */

/*
 * Open the scripted-scene bracket, speak the line, hand actor 21 back, close
 * the bracket.  The 32-byte owner ends with its one pool word, 0x1f00, the
 * dialogue line id: it is passed as the address of Value_00001f00 so the id
 * stays a pool word and is not folded into an immediate.  The actor and line
 * ids are transcribed only, and what the installing record represents is not
 * established.
 */
void SceneDialogue_RunActor21Line(void)
{
    Event_Begin();
    Event_SetMessage((s32)&Value_00001f00);
    Event_AskYesNo(21, 0);
    Event_End();
}

void FieldScene_RunScene3af_02000bb8(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x271) == 0) {
        Audio_PlayCue(158);
        Map_CopyCellsTo(30, 94, 13, 94, 1, 3);
        GameFlag_Set(0x271);
    }
}

void FieldScene_RunScene3af_02000bf0(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x272) == 0) {
        Audio_PlayCue(158);
        Map_CopyCellsTo(30, 108, 13, 108, 1, 2);
        GameFlag_Set(0x272);
    }
}

/* Gated on scene condition 0x911; when set, configures actors 20, 22 and
 * 23 (position, pose, movement and sprite flags) and their attached
 * effects, then advances the shared scene phase. */
void FieldScene_RunActorAndEffectPresentationSetup(void)
{
    u8 *record;

    if (GameFlag_IsSet(0x911) != 0) {
        Event_Begin();
        Func_02005114();
        Actor_FaceActor(0, 20, 10);
        Camera_SetSpeed(0x19999, 0x3333);
        Camera_MoveTo(0xbe0000, -1, 0x2c40000, 1);
        Camera_WaitForMove();
        Event_Wait(40);
        Actor_RunRepeatedMotion(22, 1);
        Event_SetMessage(0x1d26);
        FieldScene_RunStepThen10(0x4016);
        Actor_ShowEmote(20, 0x102, 60);
        Actor_StartRepeatedMotion(20, 2);
        FieldScene_RunStepThen10(20);
        Actor_RunRepeatedMotion(22, 1);
        Actor_FaceDirection(22, 0x5000, 0);
        FieldScene_RunStepThen10(0x4016);
        Actor_RunRepeatedMotion(20, 1);
        FieldScene_CallPairWith10(20, 0xb000);
        FieldScene_RunStepThen10(20);
        Func_02004764(23, 0x3000);
        Actor_SetAnimation(23, 3);
        FieldScene_RunStepThen10(0x4017);
        Actor_ShowEmote(22, 0x101, 40);
        Actor_FaceDirection(22, 0x8000, 20);
        FieldScene_RunStepThen10(0x4016);
        Func_02004796(23, 0);
        Actor_SetAnimationAndWait(23, 4);
        FieldScene_RunStepThen10(0x4017);
        Actor_ShowEmote(20, 0x100, 40);
        Actor_StartRepeatedMotion(20, 2);
        FieldScene_RunStepThen10(20);
        Actor_SetAnimationAndWait(22, 3);
        FieldScene_RunStepThen10(0x4016);
        Func_020047da(20, 0xd000);
        Actor_SetAnimation(23, 3);
        Actor_SetAnimationAndWait(20, 3);
        Event_Wait(60);
        Actor_ShowEmote(22, 0x106, 40);
        FieldScene_CallPairWith10(22, 0x5000);
        Event_SetMessage((s32)Data_00001d40);
        Actor_StartRepeatedMotion(22, 1);
        FieldScene_RunStepThen10(0x4016);
        Actor_ShowEmote(20, 0x101, 40);
        Actor_StartRepeatedMotion(20, 2);
        FieldScene_RunStepThen10(20);
        Actor_ShowEmote(22, 0x108, 20);
        Event_ShowMessageAndWait(0x4016, 0, 20);
        Actor_ShowEmote(23, 0x102, 60);
        FieldScene_RunStepThen10(0x4017);
        Value2(FieldScene_CallPairWith10, 22, 0x8000);
        Actor_SetAnimationAndWait(22, 3);
        Event_ShowMessageAndWait(0x4016, 0, 20);
        Actor_ShowEmote(20, 0x102, 40);
        Actor_StartRepeatedMotion(20, 2);
        FieldScene_RunStepThen10(20);
        Func_0200489c(22, 0x5000);
        Actor_SetAnimation(22, 4);
        FieldScene_RunStepThen10(22);
        Actor_FaceDirection(20, 0xb000, 0);
        Actor_FaceDirection(23, 0x3000, 40);
        Actor_FaceDirection(23, 0, 0);
        Actor_FaceDirection(20, 0xd000, 20);
        Actor_RunRepeatedMotion(22, 2);
        Event_Wait(20);
        FieldScene_RunStepThen10(0x4016);
        Actor_SetAttachedEffect(23, 0x102);
        Actor_SetAttachedEffect(20, 0x102);
        Event_Wait(40);
        Actor_SetAnimationAndWait(22, 3);
        FieldScene_RunStepThen10(0x4016);
        Camera_SetSpeed(0xcccc, 0x1999);
        Camera_MoveTo(0xb60000, -1, 0x2f80000, 1);
        Actor_SetSpeed(23, 0xcccc, 0x6666);
        ObjectMotion_EnableActionAndSetCallback_1(23, 0x200c464);
        Actor_SetSpeed(22, 0xcccc, 0x6666);
        ObjectMotion_EnableActionAndSetCallback_2(22, 0x200c49c);
        Actor_SetSpeed(20, 0xcccc, 0x6666);
        Actor_WalkToAndWait(20, 182, 0x2f8);
        Actor_StartRepeatedMotion(20, 2);
        Actor_ShowEmote(20, 0x100, 60);
        Func_0200497e(20, 0xd000);
        Event_ShowMessageAndWait(20, 0, 20);
        Actor_SetAnimationAndWait(20, 3);
        Actor_Jump(20, 4, 0);
        Actor_FaceDirection(20, 0x3000, 40);
        Camera_SetSpeed(0x10000, 0x2000);
        Camera_MoveTo(0xd80000, -1, 0x3160000, 1);
        {
            /* Set the low bit of the flag byte at +35 of actor 20's record. */
            u8 *record = Scene_GetRecord_1(20);
            u8 bits = 1;

            bits |= record[35];
            record[35] = bits;
        }
        Actor_SetSpeed(20, 0x13333, 0x9999);
        Actor_WalkToAndWait(20, 182, 0x30e);
        Actor_WalkToAndWait(20, 192, 0x328);
        Actor_WalkToAndWait(20, 216, 0x328);
        Func_02004a04(20, 0xd000);
        Actor_RunRepeatedMotion(20, 2);
        FieldScene_RunScene3af_02000bf0();
        Actor_WalkToAndWait(20, 216, 0x31e);
        Actor_SetPosition(20, 0, 0);
        /* Set the fixed-point word at +24 of actor 20's record to 1.0. */
        record = Scene_GetRecord_2(20);
        *(s32 *)(record + 24) = 0x10000;
        /* Set the fixed-point word at +28 of actor 20's record to 1.0. */
        record = Scene_GetRecord_3(20);
        *(s32 *)(record + 28) = 0x10000;
        GameFlag_Set(0x920);
        Event_End();
    }
}

void FieldScene_RunScene3af_020010a0(void)
{
    u8 bits;

    if (GameFlag_IsSet(0x911) != 0) {
        if (GameFlag_IsSet(0x922) == 0) {
            Event_Begin();
            Func_02005510();
            FieldScene_RunScene3af_020012f0();
            Actor_SetSpeed(20, 0x6666, 0x3333);
            *(u8 *)(Func_020053dc(20) + 90) &= 254;
            Actor_WalkToAndWait(20, 232, 0x330);
            Event_Wait(1);
            bits = 1;
            {
                u8 *record = Func_02005400(20);
                u8 value = record[90];

                record[90] = value | bits;
            }
            Event_Wait(20);
            Actor_StartRepeatedMotion(20, 2);
            FieldScene_RunStepThen10(20);
            Actor_SetSpeed(20, 0x13333, 0x9999);
            *(u8 *)(Func_0200542e(20) + 90) &= 254;
            Actor_WalkToAndWait(20, 244, 0x324);
            Event_Wait(1);
            {
                u8 *record = Func_0200544e(20);

                bits |= record[90];
                record[90] = bits;
            }
            Event_Wait(20);
            Actor_SetSpeed(20, 0x33333, 0x19999);
            Actor_WalkToAndWait(20, 248, 0x30a);
            Actor_WalkToAndWait(20, 248, 0x2bc);
            Actor_SetPosition(20, 0xf60000, 0x2000000);
            Actor_FaceDirection(20, 0, 0);
            Actor_ShowEmote(0, 0x101, 60);
            Event_End();
        }
    }
}

void FieldScene_RunScene3af_020011c8(void)
{
    u8 bits;

    if (GameFlag_IsSet(0x911) != 0) {
        if (GameFlag_IsSet(0x922) == 0) {
            Event_Begin();
            Func_02005638();
            FieldScene_RunScene3af_020012f0();
            Actor_SetSpeed(20, 0x6666, 0x3333);
            *(u8 *)(Func_02005504(20) + 90) &= 254;
            Actor_WalkToAndWait(20, 202, 0x330);
            Event_Wait(1);
            bits = 1;
            {
                u8 *record = Func_02005528(20);
                u8 value = record[90];

                record[90] = value | bits;
            }
            Event_Wait(20);
            Actor_StartRepeatedMotion(20, 2);
            FieldScene_RunStepThen10(20);
            Actor_SetSpeed(20, 0x13333, 0x9999);
            *(u8 *)(Func_02005556(20) + 90) &= 254;
            Actor_WalkToAndWait(20, 192, 0x324);
            Event_Wait(1);
            {
                u8 *record = Func_02005576(20);

                bits |= record[90];
                record[90] = bits;
            }
            Event_Wait(20);
            Actor_SetSpeed(20, 0x33333, 0x19999);
            Actor_WalkToAndWait(20, 180, 0x30a);
            Actor_WalkToAndWait(20, 180, 0x2bc);
            Actor_SetPosition(20, 0xf60000, 0x2000000);
            Actor_FaceDirection(20, 0, 0);
            Actor_ShowEmote(0, 0x101, 60);
            Event_End();
        }
    }
}

void FieldScene_RunScene3af_020012f0(void)
{
    u32 i;
    s32 record;

    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0xd80000, -1, 0x3380000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    FieldScene_RunScene3af_02000bf0();
    Map_CopyCellsTo(30, 108, 13, 108, 1, 2);
    Event_Wait(10);
    Actor_SetPosition(20, 0xd80000, 0x3200000);
    Actor_SetSpeed(20, 0x13333, 0x9999);
    Actor_WalkToAndWait(20, 216, 0x32e);
    Actor_FaceActor(0, 20, 10);
    Actor_SetAnimationAndWait(20, 4);
    Actor_StartRepeatedMotion(20, 2);
    Actor_ShowEmote(20, 0x100, 20);
    Actor_FaceActor(20, 0, 20);
    Actor_StartRepeatedMotion(20, 2);
    Event_SetMessage(0x1d8d);
    Event_ShowMessageAndWait(20, 0, 20);
    Value3(Engine_ActorShowEmote, 20, 0x102, 0);
    GameFlag_Set(0x923);
}

s32 SceneState_ResetWorkValuesAndStart(void)
{
    extern u32 Data_0200db50[];
    extern u32 Data_0200db60;
    extern s32 Data_0200db70[];

    u8 **base = &Data_03001e70;
    u8 *p = base[0] + 0x104;

    GameFlag_Clear(0x11C);
    *(s32 *)(base[19] + 0x1C0) = 0x209;
    *(s32 *)(p + 0x1C) = 0;
    Data_0200db58 = (u16)Random_Next();
    Data_0200db38 = (u16)Random_Next();
    Data_0200db50[0] = 0;
    Data_0200db50[1] = 0;
    Data_0200db60 = 0;
    Map_Redraw();
    Task_Wait(1);
    Func_0200286a();
    return 0;
}

void SceneActor_PlaceActors20To27(void)
{
    s32 m = 0xA0;

    m <<= 7;
    Actor_SetPosition(21, 0x1060000, 0x2C20000);
    *(u16 *)(Func_02005abe(21) + 6) = m;
    Actor_SetPosition(24, 0xA40000, 0x2880000);
    {
        s32 z = 0;
        *(u16 *)(Func_02005ad8(24) + 6) = z;
    }
    Actor_SetSpritePriority(24, 1);
    Actor_SetPosition(25, 0xC60000, 0x2990000);
    {
        s32 x = 0x80;
        *(u16 *)(Func_02005af6(25) + 6) = x << 8;
    }
    Actor_SetSpritePriority(25, 1);
    Actor_SetPosition(26, 0xBC0000, 0x2A60000);
    {
        s32 x = 0xB0;
        *(u16 *)(Func_02005b16(26) + 6) = x << 8;
    }
    Actor_SetPosition(27, 0xBA0000, 0x27B0000);
    *(u16 *)(Func_02005b2e(27) + 6) = m;
    Actor_SetPosition(22, 0, 0);
    Actor_SetPosition(23, 0, 0);
    Actor_SetPosition(20, 0, 0);
}

void FieldScene_RunScene3af_0200185c(void)
{
    u8 *record;
    u8 bits;

    Event_Begin();
    Call1(Func_02005b62, 0x200d160);
    Task_Wait(1);
    Actor_SetPosition(20, 0, 0);
    Actor_SetPosition(23, 0xee0000, 0x2720000);
    Actor_SetPosition(22, 0xcc0000, 0x2090000);
    record = Func_02005ba0(22);
    *(s32 *)(record + 12) = 0x100000;
    bits = 128;
    {
        u8 *record = Func_02005bac(22);
        u8 value = record[89];

        record[89] = value | bits;
    }
    Actor_SetSpeed(22, 0x9999, 0x4ccc);
    Actor_EnableActionCallback(22, 0x200c58c);
    {
        u8 *record = Func_02005bce(21);

        bits |= record[89];
        record[89] = bits;
    }
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Actor_EnableActionCallback(21, 0x200c628);
    if (GameFlag_IsSet(0x109) != 0) {
        FieldScene_RunScene3af_02004218();
    }
    Event_End();
}

void FieldScene_RunScene3af_02001920(void)
{
    u8 *record;

    Event_Begin();
    Call1(Func_02005c26, 0x200d160);
    Task_Wait(1);
    Actor_SetPosition(20, 0, 0);
    Actor_SetPosition(23, 0xee0000, 0x2720000);
    Actor_SetPosition(22, 0x10c0000, 0x2a60000);
    record = Func_02005c64(22);
    {
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    Actor_EnableActionCallback(22, 0x200c980);
    {
        u8 *record = Func_02005c76(21);
        u8 bits = 128;

        bits |= record[89];
        record[89] = bits;
    }
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Actor_EnableActionCallback(21, 0x200c628);
    if (GameFlag_IsSet(0x109) != 0) {
        FieldScene_RunScene3af_02004218();
    }
    Event_End();
}

void FieldScene_RunScene3af_02001a98(void)
{
    s32 record;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(20, 0, 0);
    Actor_SetPosition(22, 0, 0);
    Actor_SetPosition(24, 0, 0);
    Actor_SetPosition(25, 0, 0);
    Actor_SetPosition(26, 0, 0);
    Actor_SetPosition(27, 0, 0);
    Actor_SetPosition(0, 0, 0);
    Actor_SetPosition(23, 0, 0);
    record = Func_02005e16(23);
    {
        s32 shown = 0x3000;

        *(u16 *)(record + 6) = shown;
    }
    Actor_SetPosition(21, 0xe80000, 0x28a0000);
    record = Func_02005e2e(21);
    {
        s32 shown = 0xb000;

        *(u16 *)(record + 6) = shown;
    }
    Camera_MoveTo(0xe80000, -1, 0x27c0000, 0);
    Map_Redraw();
    Task_Wait(1);
    Func_02003760(23, 21);
}

void FieldScene_RunScene3af_02001b58(void)
{
    s32 record;

    Event_Begin();
    Call1_02001b58(Func_02005e5e, 0x200d160);
    Task_Wait(1);
    Actor_SetPosition(0, 0xe80000, 0x27c0000);
    Actor_SetChildValue(0, 15);
    record = Func_02005e90(0);
    Actor_SetSpriteFlags(record, 0);
    Task_Wait(1);
    Camera_FollowActor(0, 0);
    Map_Redraw();
    Task_Wait(1);
    Actor_Stop(22);
    Actor_Stop(21);
    Task_Wait(1);
    Actor_SetPosition(22, 0, 0);
    Actor_SetPosition(21, 0, 0);
    Actor_SetPosition(20, 0, 0);
    record = Func_02005ee4(20);
    {
        s32 shown = 0x3000;

        *(u16 *)(record + 6) = shown;
    }
    Actor_SetPosition(23, 0xe80000, 0x28a0000);
    record = Func_02005efc(23);
    {
        s32 shown = 0xb000;

        *(u16 *)(record + 6) = shown;
    }
    Task_Wait(1);
    Func_02003818(20, 23);
}

void FieldScene_RunScene3af_02001c14(s32 a0, s32 a1)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    FieldScene_RunScene3af_02000bb8();
    Actor_SetPosition(a0, 0xd80000, 0x24c0000);
    Actor_SetSpeed(a0, 0xcccc, 0x6666);
    Actor_WalkToAndWait(a0, 216, 0x258);
    Actor_WalkToAndWait(a0, 218, 0x25c);
    Actor_WalkToAndWait(a0, 234, 0x25c);
    Actor_WalkToAndWait(a0, 236, 0x26a);
    Actor_FaceDirection(a0, 0x5000, 20);
    Actor_SetAnimationAndWait(a0, 3);
    Event_Wait(20);
    Call2(Func_020056ac, a1, 0x5000);
    Actor_Jump(a1, 4, 40);
    Actor_StartRepeatedMotion(a1, 2);
    Event_SetMessage(0x1e39);
    Event_ShowMessageAndWait(a1, 0, 20);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(10);
}

void FieldScene_RunActorTwentyDialogueSequence(void)
{
    extern s32 *Data_03001ebc;

    Event_Begin();
    Func_02006012((s32)Data_0200d160);
    Task_Wait(1);
    Actor_SetChildValue(0, 15);
    Actor_SetSpriteFlags(Func_02006036(0), 0);
    Data_03001ebc[0x70] = 0x202;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_RunRepeatedMotion(20, 1);
    Event_SetMessage(0x1e41);
    Event_ShowMessageAndWait(20, 0, 10);
    FieldScene_CallPairWith10(22, 0x5000);
    Actor_Jump(22, 4, 20);
    Actor_StartRepeatedMotion(22, 2);
    Event_ShowMessageAndWait(0x6016, 0, 20);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(11);
}

/* Configures actors 20, 21, 22 and 23 (position, pose, and movement/sprite
 * flags) and advances the shared scene phase before the scene runs. */
void FieldScene_ConfigureLeadActors(void)
{
    u8 *record;

    Event_Begin();
    Object_NotifyLastActiveOfEvent_1(0x200d160);
    Task_Wait(1);
    Actor_SetPosition(20, 0xb60000, 0x26a0000);
    Actor_SetPosition(23, 0xee0000, 0x2720000);
    Actor_SetPosition(22, 0x10c0000, 0x2a60000);
    record = Scene_GetRecord_1_020029d4(22);
    {
        /* Clear the visibility/active flag at +6. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    Actor_EnableActionCallback(22, 0x200c980);
    {
        /* Set the high bit of the flag byte at +89. */
        u8 *record = Scene_GetRecord_2_020029d4(21);
        u8 bits = 128;

        bits |= record[89];
        record[89] = bits;
    }
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Actor_EnableActionCallback(21, 0x200c628);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_SetSpeed(20, 0x19999, 0xcccc);
    Actor_WalkToAndWait(20, 182, 0x224);
    Func_02006476(20, 0);
    Value2(Func_02006480_a, 0, 0x8000);
    Actor_RunRepeatedMotion(20, 1);
    Event_SetMessage(0x1ee1);
    ((void (*)())Func_02006480_b)(20);
    Actor_SetAnimationAndWait(0, 3);
    Event_Wait(40);
    Actor_FaceDirection(20, 0x5000, 20);
    Actor_ShowEmote(20, 0x105, 60);
    Event_ShowMessageAndWait(20, 0, 40);
    FieldScene_CallPairWith10(20, 0);
    FieldScene_RunStepThen10(20);
    Actor_SetAnimationAndWait(0, 3);
    Actor_SetAnimationAndWait(20, 3);
    Actor_WalkToAndWait(20, 182, 0x258);
    Actor_WalkToAndWait(20, 216, 0x258);
    Call2(FieldScene_CallPairWith10, 20, 0xc000);
    FieldScene_RunScene3af_02000bb8();
    Event_Wait(10);
    Actor_WalkToAndWait(20, 216, 0x244);
    Actor_SetPosition(20, 0, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    GameFlag_Set(0x92b);
    GameFlag_Clear(0x302);
    Event_End();
}

/* Configures actors 20, 21 and 22 (position and movement/sprite flags) and
 * advances the shared scene phase before the scene runs. */
void FieldScene_ConfigureThreeActors(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetChildValue(0, 15);
    record = Scene_GetRecord_1_02002b7c(0);
    Actor_SetSpriteFlags(record, 0);
    Object_NotifyLastActiveOfEvent_1_02002b7c(0x200d160);
    Task_Wait(1);
    Actor_SetPosition(20, 0xc40000, 0x1f60000);
    record = Scene_GetRecord_2_02002b7c(20);
    {
        /* Set the visibility/active flag at +6. */
        s32 shown = 0xa000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Actor_SetPosition(22, 0xb80000, 0x20c0000);
    record = Scene_GetRecord_3_02002b7c(22);
    {
        /* Set the visibility/active flag at +6. */
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    ObjectMotion_SetActionVariant_1(21, 1);
    Actor_SetPosition(21, 0xb80000, 0x2780000);
    record = Scene_GetRecord_4(21);
    {
        /* Set the visibility/active flag at +6. */
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    SCENE_PHASE = 0x202;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Actor_Jump(22, 4, 10);
    Actor_Jump(22, 6, 20);
    Event_SetMessage(0x1ee5);
    FieldScene_RunStepThen10(22);
    Actor_SetAnimationAndWait(20, 3);
    Actor_SetSpeed(21, 0x30000, 0x18000);
    Actor_WalkToAndWait(21, 180, 0x222);
    ObjectMotion_ArmCallback_1_02002b7c(21, 0xb000, 40);
    Actor_RunRepeatedMotion(21, 1);
    FieldScene_RunStepThen10(21);
    Event_RequestExit(15);
}

s32 SceneState_FindFirstSetFlagOfGroup(u32 sel)
{
    s32 v = 0;
    s32 id = 23;
    u32 i;

    switch (sel) {
    case 0:
        v = 0x92C;
        break;
    case 1:
        v = 0x935;
        break;
    case 2:
        v = 0x917;
        break;
    case 3:
        v = 0x990;
        break;
    }
    for (i = 0; i < 9; i++) {
        if (GameFlag_IsSet(v)!= 0) return id;
        v++;
        id++;
    }
    return 0;
}

void SceneEffect_InitSlotsEightToNineteen(void)
{
    extern u32 Data_0200db70[];

    {
        s32 z = 0;
        *(Func_02007596(8) + 89) = z;
        *(Func_020075a2(9) + 89) = z;
        *(Func_020075ac(10) + 89) = z;
        *(Func_020075b6(11) + 89) = z;
    }
    OverlayObject_ActivateSlotWithMode3(8);
    OverlayObject_ActivateSlotWithMode3(9);
    OverlayObject_ActivateSlotWithMode3(10);
    OverlayObject_ActivateSlotWithMode3(11);
    OverlayObject_ActivateSlotWithMode3(12);
    OverlayObject_ActivateSlotWithMode3(13);
    OverlayObject_ActivateSlotWithMode3(14);
    OverlayObject_ActivateSlotWithMode3(15);
    {
        Data_0200db90[0] = Func_020075f0(12)[4];
        Data_0200db90[1] = Func_020075fc(13)[4];
        Data_0200db90[2] = Func_02007606(14)[4];
        Data_0200db90[3] = Func_02007610(15)[4];
        OverlayObject_ActivateSlotWithMode3(16);
        OverlayObject_ActivateSlotWithMode3(17);
        OverlayObject_ActivateSlotWithMode3(18);
        OverlayObject_ActivateSlotWithMode3(19);
        {
            s32 v = 0xFFFF0000;

            Func_02007632(16)[6] = v;
            Func_0200763c(17)[6] = v;
            Func_02007644(18)[6] = v;
            Func_0200764c(19)[6] = v;
        }
        Data_0200db90[4] = Func_02007654(16)[4];
        Data_0200db90[5] = Func_0200765e(17)[4];
        Data_0200db90[6] = Func_02007668(18)[4];
        Data_0200db90[7] = Func_02007672(19)[4];
    }
    SceneState_FillTableDb70ByChecks();
}

void OverlayObject_ActivateSlotWithMode3(s32 a)
{
    u8 *p = Func_02007692(a);

    if (p != 0) {
        Actor_SetSpritePriority(a, 3);
        Actor_SetSpriteFlags(p, 0);
        p[89] = 0;
        {
            s32 c;
            c = 2 | p[35];
            p[35] = c;
        }
    }
}

void SceneState_FillTableDb70ByChecks(void)
{
    extern s32 Data_0200db70[];

    {
        s32 *p = Data_0200db70;
        u32 i = 0;
        s32 t = 3;

        for (i = 0; i < 4; i++) {
            if (SceneState_FindFirstSetFlagOfGroup(i)!= 0) p[i] = SceneData_SelectTableEntryByFlagGroup(i);
            else p[i] = t;
        }
    }
    if (SceneState_FindFirstSetFlagOfGroup(0) != 0) Data_0200db70[0] = SceneData_SelectTableEntryByFlagGroup(0);
    else Data_0200db70[0] = 3;
    if (SceneState_FindFirstSetFlagOfGroup(2) != 0) Data_0200db70[1] = SceneData_SelectTableEntryByFlagGroup(2);
    else Data_0200db70[1] = 3;
    {
        s32 *q = Data_0200db70;
        s32 t = 3;

        q[2] = t;
        q[3] = t;
        if (SceneState_FindFirstSetFlagOfGroup(1) != 0) q[4] = SceneData_SelectTableEntryByFlagGroup(1);
        else q[4] = t;
    }
    if (SceneState_FindFirstSetFlagOfGroup(3) != 0) Data_0200db70[5] = SceneData_SelectTableEntryByFlagGroup(3);
    else Data_0200db70[5] = 3;
    Data_0200db70[6] = 3;
    Data_0200db70[7] = 3;
}

s32 SceneData_SelectTableEntryByFlagGroup(u32 sel)
{
    s32 base = 0;
    u32 i;

    switch (sel) {
    case 0:
        base = 0x92C;
        break;
    case 1:
        base = 0x935;
        break;
    case 2:
        base = 0x917;
        break;
    case 3:
        base = 0x990;
        break;
    }
    for (i = 0; i < 9; i++) {
        if (GameFlag_IsSet(base + i) != 0) return Data_0200db08[i];
    }
    return 0;
}

void SceneState_ConfigureEntries8Through19(void)
{
    extern u32 Data_0200db50[];
    extern u32 Data_0200db60;
    extern s32 Data_0200db70[];

    u16 *p;

    Func_02006a1c(8, 0);
    p = Data_0200db30;
    {
        s32 z = 0;

        p[0] = z;
    }
    Func_02006a2a(9, 1);
    p[1] += 0x80;
    Func_02006a38(10, 2);
    p[2] += 0x100;
    Func_02006a4a(11, 3);
    p[3] += 0x200;
    Func_02006af2(12, 0, 0);
    Func_02006afc(13, 1, 0);
    Func_02006b06(14, 2, 0);
    Func_02006b10(15, 3, 0);
    Func_02006b1a(16, 4, 1);
    Func_02006b24(17, 5, 1);
    Func_02006b2e(18, 6, 1);
    Func_02006b38(19, 7, 1);
}

void SceneEffect_AdvanceSlotByValueBand(s32 a, s32 i)
{
    extern u32 Data_0200db70[];

    u16 v = Data_0200db40[i];

    if (v >= 0x6801 && v <= 0x6FFF) {
        Data_0200db30[i] += 0x70;
        Actor_SetAnimation(a, 3);
    } else if (v >= 0xE801 && v <= 0xEFFF) {
        Data_0200db30[i] += 0xE0;
        Actor_SetAnimation(a, 3);
    } else if (v >= 0x7001 && v <= 0xEFFF) {
        Data_0200db30[i] += 0x1C0;
        Actor_SetAnimation(a, 2);
    } else {
        Data_0200db30[i] += 0x300;
        Actor_SetAnimation(a, 1);
    }
}

void SceneEffect_SelectSlotValueAndPosition(s32 a, s32 b, s32 c)
{
    extern u32 Data_0200db70[];

    s32 *p = (s32 *)Func_0200790e_a(a);
    union Slot *q = (union Slot *)p[20];
    s32 t;

    if ((c & 2) == 0) {
        switch (Data_0200db70[b]) {
        case 1:
            Data_0200db40[b] = Data_0200db30[0];
            Func_02007a40(a, 8);
            break;
        case 2:
            Data_0200db40[b] = Data_0200db30[1];
            Func_02007a54(a, 9);
            break;
        case 3:
            Data_0200db40[b] = Data_0200db30[2];
            Func_02007a68(a, 10);
            break;
        case 4:
            Data_0200db40[b] = Data_0200db30[3];
            Func_02007a7c(a, 11);
            break;
        }
    }
    if ((c & 1) != 0) {
        t = Math_Sin(Data_0200db40[b]);
        q[7].h[1] = Math_Sin(Data_0200db40[b] + 0x8000) >> 5;
        p[4] = Data_0200db90[b] - (t << 2) - (t << 1);
    } else {
        t = Math_Sin(Data_0200db40[b] + 0x8000);
        q[7].h[1] = Math_Sin(Data_0200db40[b]) >> 5;
        p[4] = Data_0200db90[b] + (t << 2) + (t << 1);
    }
}

void SceneState_InitActorSlots8To19(void)
{
    extern u32 Data_0200db50[];
    extern u32 Data_0200db60;
    extern s32 Data_0200db70[];

    {
        u16 *p = Data_0200db40;
        u32 i;

        for (i = 0; i < 8; i++) p[i] = 0xC000;
    }
    OverlayObject_ActivateSlotWithMode3(8);
    ((void (*)())Engine_ActorSetPosition)(9, 0, 0);
    Actor_SetPosition(10, 0, 0);
    Actor_SetPosition(11, 0, 0);
    Actor_SetPosition(12, 0, 0);
    OverlayObject_ActivateSlotWithMode3(13);
    OverlayObject_ActivateSlotWithMode3(14);
    OverlayObject_ActivateSlotWithMode3(15);
    {
        s32 *t = Data_0200db70;
        s32 z = 0;
        t[0] = z;
        t[1] = z;
        t[2] = z;
        t[3] = z;
        Data_0200db90[0] = Func_02007a90(8)[4];
        Data_0200db90[1] = Func_02007aa0(13)[4];
        Data_0200db90[2] = Func_02007aac_b(14)[4];
        Data_0200db90[3] = Func_02007ab8(15)[4];
        OverlayObject_ActivateSlotWithMode3(16);
        OverlayObject_ActivateSlotWithMode3(17);
        OverlayObject_ActivateSlotWithMode3(18);
        OverlayObject_ActivateSlotWithMode3(19);
        {
            s32 v = 0xFFFF0000;

            Func_02007adc(16)[6] = v;
            Func_02007ae6(17)[6] = v;
            Func_02007aee(18)[6] = v;
            Func_02007af6(19)[6] = v;
        }
        t[4] = z;
        t[5] = z;
        t[6] = z;
        t[7] = z;
        Data_0200db90[4] = Func_02007b08(16)[4];
        Data_0200db90[5] = Func_02007b14(17)[4];
        Data_0200db90[6] = Func_02007b20(18)[4];
        Data_0200db90[7] = Func_02007b2c_a(19)[4];
    }
    {
        s32 *w = Func_02007b38(0);

        if (w != 0) Actor_SetPosition(8, w[2], w[4]);
    }
    Task_Wait(1);
    Func_02007c44(13, 8);
    Func_02007c4c(14, 8);
    Func_02007c54(15, 8);
    Func_02007c5c(16, 8);
    Func_02007c64(17, 8);
    Func_02007c6c(18, 8);
    Func_02007c74(19, 8);
    {
        s32 o = 1;

        Func_02007b8a(8)[92] = o;
        Func_02007b96(13)[92] = o;
        Func_02007ba0(14)[92] = o;
        Func_02007baa(15)[92] = o;
        Func_02007bb4(16)[92] = o;
        Func_02007bbe(17)[92] = o;
        Func_02007bc8(18)[92] = o;
        Func_02007bd2(19)[92] = o;
    }
    Task_Wait(1);
    Actor_SetPosition(8, 0x840000, 0x2780000);
    Task_Wait(1);
    SceneEffect_SelectSlotValueAndPosition(8, 0, 2);
    SceneEffect_SelectSlotValueAndPosition(13, 1, 2);
    SceneEffect_SelectSlotValueAndPosition(14, 2, 2);
    SceneEffect_SelectSlotValueAndPosition(15, 3, 2);
    SceneEffect_SelectSlotValueAndPosition(16, 4, 3);
    SceneEffect_SelectSlotValueAndPosition(17, 5, 3);
    SceneEffect_SelectSlotValueAndPosition(18, 6, 3);
    SceneEffect_SelectSlotValueAndPosition(19, 7, 3);
}

void SceneState_ApplyFiveRectsAtColumn78(void)
{
    Map_CopyCellsTo(78, 39, 78, 40, 5, 1);
    Map_CopyCellsTo(78, 39, 78, 41, 5, 1);
    Map_CopyCellsTo(78, 39, 79, 42, 4, 1);
    Map_CopyCellsTo(78, 39, 82, 43, 1, 1);
    {
        s32 x = 17;
        s32 y = 40;

        Map_CopyCellAttributes(17, 38, 5, 2, x, y);
    }
}

void DialogueLayout_ConfigureTwoRegions(void)
{
    Map_CopyCellsTo(66, 61, 64, 40, 5, 4);
    Map_CopyCellAttributes(0, 0, 5, 4, 5, 39);
}

void FieldScene_RunStepThen10(s32 a)
{
    Event_ShowMessage(a, 0);
    Event_Wait(10);
}

void FieldScene_CallPairWith10(s32 a, s32 b)
{
    Actor_FaceDirection(a, b, 10);
}

void FieldScene_ConfigureFourActorPresentation(void)
{
    extern s32 Data_0200db50[];
    extern s32 Data_0200db60;
    s32 Func_02007d48();
    s32 Func_02007d62();

    s32 record;
    s32 base6_6014;

    Event_Begin();
    Data_0200db50[0] = 0x40000;
    Data_0200db60 = -0x8000;
    Object_NotifyLastActiveOfEvent_1_02003a0c((s32)Data_0200d160);
    Task_Wait(1);
    Actor_SetPosition(21, 0xb60000, 0x26a0000);
    record = Scene_GetRecord_1_02003a0c(21);
    {
        s32 shown = 0xc000;

        *(u16 *)(record + 6) = shown;
    }
    Actor_SetPosition(20, 0xda0000, 0x2040000);
    record = Scene_GetRecord_2_02003a0c(20);
    {
        s32 shown = 0xb000;

        *(u16 *)(record + 6) = shown;
    }
    Actor_SetPosition(22, 0xcc0000, 0x20e0000);
    record = Scene_GetRecord_3_02003a0c(22);
    {
        s32 shown = 0xb000;

        *(u16 *)(record + 6) = shown;
    }
    Actor_SetPosition(23, 0, 0);
    Actor_SetChildValue(0, 15);
    record = Scene_GetRecord_4_02003a0c(0);
    Actor_SetSpriteFlags(record, 0);
    Task_Wait(1);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Event_OpenScreen();
    Event_WaitForScreen();
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Actor_WalkToAndWait(21, 182, 0x214);
    Actor_FaceDirection(21, 0xb000, 40);
    Event_SetMessage(SCENE_STEP_VALUE);
    FieldScene_RunStepThen10(21);
    base6_6014 = (s32)Data_00006014;
    Actor_RunRepeatedMotion(20, 2);
    Actor_SetAnimation(20, 4);
    FieldScene_RunStepThen10(base6_6014);
    Actor_FaceDirection(21, 0xd000, 0);
    Actor_FaceDirection(22, 0xd000, 0);
    Actor_ShowEmote(21, 0x101, 0);
    Actor_ShowEmote(22, 0x101, 60);
    Actor_SetAnimation(20, 3);
    FieldScene_RunStepThen10(base6_6014);
    Actor_SetAttachedEffect(21, 0x102);
    Actor_SetAttachedEffect(22, 0x102);
    Event_Wait(80);
    Actor_ShowEmote(21, 0x100, 20);
    Actor_SetSpeed(21, 0x19999, 0xcccc);
    Actor_WalkToAndWait(21, 194, 0x1f4);
    Actor_FaceDirection(21, 0xb000, 20);
    Actor_SetSpeed(22, 0xcccc, 0x6666);
    Actor_WalkToAndWait(22, 192, 0x206);
    Actor_FaceDirection(22, 0xb000, 0);
    Actor_SetSpeed(20, 0x10000, 0x8000);
    Actor_WalkToAndWait(20, 210, 0x1fc);
    Value2(FieldScene_CallPairWith10, 20, 0xb000);
    Actor_RunRepeatedMotion(21, 1);
    Call1_02003a0c(FieldScene_RunStepThen10, 0x5015);
    Actor_SetAnimationAndWait(20, 3);
    Call2(FieldScene_CallPairWith10, 22, 0xd000);
    Call1_02003a0c(FieldScene_RunStepThen10, 0x9016);
    Actor_FaceDirection(20, 0x8000, 20);
    Actor_SetAnimation(20, 4);
    Call1_02003a0c(FieldScene_RunStepThen10, 0xa014);
    Actor_WalkToAndWait(20, 204, 0x218);
    Actor_FaceDirection(22, 0xb000, 0);
    Actor_WalkToAndWait(20, 182, 0x224);
    Actor_WalkToAndWait(20, 182, 0x250);
    Actor_WalkTo(20, 182, 0x298);
    Event_Wait(40);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(16);
}

/* Sets up actors 1, 2, and 3 from three source records, runs their
 * animations and a wait loop gated on actor 0, then clears a flag byte
 * at +90 on actors 21 and 22 before finishing the scene. */
void FieldScene_RunThreeActorEncounter(void)
{
    u8 *record;
    u8 bits;

    Event_Begin();
    Actor_SetSpeed(0, 0x10000, 0x8000);
    Actor_WalkToAndWait(0, 180, 0x28e);
    Actor_FaceDirection(0, 0x8000, 0);
    /* For each of actors 1, 2, and 3: fetch a source record, and if one
     * exists, copy its fields at +8 and +16 into the actor. */
    record = Scene_GetRecord_1_02003c88(0);
    if (record != 0) {
        Actor_SetPosition(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Scene_GetRecord_2_02003c88(0);
    if (record != 0) {
        Actor_SetPosition(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Scene_GetRecord_3_02003c88(0);
    if (record != 0) {
        Actor_SetPosition(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(1, 0x13333, 0x9999);
    Actor_SetSpeed(2, 0x10000, 0x8000);
    Actor_SetSpeed(3, 0x13333, 0x9999);
    Actor_WalkTo(1, 194, 0x280);
    Actor_WalkTo(2, 198, 0x28e);
    Actor_WalkToAndWait(3, 194, 0x2a0);
    Actor_SetAnimation(1, 1);
    Actor_SetAnimation(2, 1);
    Event_Wait(10);
    Actor_FaceDirection(1, 0x8000, 0);
    Actor_FaceDirection(2, 0x8000, 0);
    Value2(FieldScene_CallPairWith10, 3, 0x8000);
    FieldScene_CallPairWith10(22, 0);
    Event_SetMessage(0x1f55);
    Call1_02003c88(FieldScene_RunStepThen10, 22);
    Value2(FieldScene_CallPairWith10, 21, 0xd000);
    Event_ShowMessageAndWait(21, 0, 40);
    Actor_ShowEmote(22, 0x100, 20);
    Actor_RunRepeatedMotion(22, 1);
    Event_OpenMessage(22, 0);
    /* Gated on a condition read from actor 0: configure actors 2, 1, and 3,
     * then spin, re-checking actor 0, while a condition on actor 2 holds. */
    if (Event_ChooseYesNo(0, 0) == 1) {
        Actor_SetAnimationAndWait(2, 4);
        FieldScene_RunStepThen10(2);
        Value2(FieldScene_CallPairWith10, 3, 0xa000);
        Actor_SetAnimation(3, 3);
        FieldScene_RunStepThen10(3);
        Value2(FieldScene_CallPairWith10, 1, 0x6000);
        Actor_RunRepeatedMotion(1, 1);
        Event_OpenMessage(1, 0);
        L_02003dfa:;
        if (Event_ChooseYesNo(0, 0) == 1) {
            Actor_RunRepeatedMotion(2, 1);
            Event_SetMessage(0x1f53);
            Event_OpenMessage(2, 0);
            goto L_02003dfa;
        }
    }
    Event_Wait(20);
    Actor_SetAnimationAndWait(22, 3);
    Event_SetMessage(0x1f5b);
    FieldScene_RunStepThen10(22);
    Actor_SetSpeed(22, 0x10000, 0x8000);
    Actor_SetSpeed(21, 0x10000, 0x8000);
    /* Clear the low bit of the flag byte on actor 22. */
    *(u8 *)(Scene_GetRecord_4_02003c88(22) + ACTOR_FLAGS_OFFSET) &= 254;
    Actor_WalkToAndWait(22, 162, 0x27a);
    Event_Wait(1);
    bits = 1;
    {
        /* Set the low bit of the flag byte on actor 22. */
        u8 *record = Scene_GetRecord_5(22);
        u8 value = record[ACTOR_FLAGS_OFFSET];

        record[ACTOR_FLAGS_OFFSET] = value | bits;
    }
    /* Clear the low bit of the flag byte on actor 21. */
    *(u8 *)(Scene_GetRecord_6(21) + ACTOR_FLAGS_OFFSET) &= 254;
    Actor_WalkToAndWait(21, 162, 0x2a4);
    Event_Wait(1);
    {
        /* Set the low bit of the flag byte on actor 21. */
        u8 *record = Scene_GetRecord_7(21);

        bits |= record[ACTOR_FLAGS_OFFSET];
        record[ACTOR_FLAGS_OFFSET] = bits;
    }
    Actor_FaceDirection(22, 0x3000, 0);
    Call2(FieldScene_CallPairWith10, 21, 0xd000);
    FieldScene_RunStepThen10(22);
    /* Finish actors 1, 2, and 3 with the same target values used earlier. */
    Actor_WalkTo(1, 180, 0x28e);
    Actor_WalkTo(2, 180, 0x28e);
    Actor_WalkToAndWait(3, 180, 0x28e);
    Actor_Destroy(1);
    Actor_Destroy(2);
    Actor_Destroy(3);
    GameFlag_Set(0x903);
    Event_End();
}

/* Sets up background/camera parameters and either takes the short branch
 * (branch value 1 from Engine_EventChooseYesNo) or runs a longer sequence that moves
 * and reconfigures actors 20, 21, 22 and 26 before advancing the scene
 * phase to 0x201 and finishing the scene. */
void FieldScene_RunEncounterClosingSequence(void)
{
    u32 i;
    s32 record;
    s32 data_00002014_addr;
    s32 data_0200c918_addr;

    Event_Begin();
    Object_NotifyLastActiveOfEvent_1_02003f30(0x200d1d8);
    Task_Wait(1);
    Event_OpenScreen();
    Actor_SetSpeed(0, 0x10000, 0x8000);
    Actor_WalkToAndWait(0, 148, 0x290);
    Actor_ShowEmote(22, 0x100, 0);
    Actor_RunRepeatedMotion(22, 1);
    FieldScene_CallPairWith10(22, 0x5000);
    Event_SetMessage(0x1f69);
    Event_OpenMessage(0x2016, 0);
    Actor_FaceDirection(0, 0xe000, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        Call1_02003f30(FieldScene_RunStepThen10, 0x2016);
        Event_End();
    } else {
        bump_step(1);
        Event_AskYesNo(0x2016, 0);
        FieldScene_RunScene3af_02000bb8();
        Actor_SetPosition(26, 0xd80000, 0x24c0000);
        Actor_SetSpeed(26, 0x13333, 0x9999);
        Actor_WalkToAndWait(26, 216, 0x254);
        Actor_WalkToAndWait(26, 188, 0x268);
        Actor_FaceDirection(0, 0xe000, 0);
        Actor_FaceDirection(21, 0xd000, 0);
        Actor_FaceDirection(22, 0xd000, 0);
        Value2(FieldScene_CallPairWith10, 26, 0x5000);
        Actor_Jump(26, 2, 0);
        Actor_SetAnimation(26, 4);
        Event_ShowMessage(26, 0);
        Actor_SetPosition(20, 0xb40000, 0x3090000);
        Actor_SetSpeed(20, 0x10000, 0x8000);
        Actor_WalkToAndWait(20, 180, 0x298);
        data_00002014_addr = (s32)Data_00002014;
        Actor_FaceDirection(20, 0xd000, 0);
        FieldScene_RunStepThen10(data_00002014_addr);
        Actor_FaceDirection(0, 0x2000, 0);
        Actor_FaceDirection(22, 0x3000, 0);
        Actor_ShowEmote(26, 0x101, 60);
        Actor_RunRepeatedMotion(20, 1);
        FieldScene_RunStepThen10(data_00002014_addr);
        Actor_StartRepeatedMotion(21, 2);
        FieldScene_RunStepThen10(21);
        ObjectMotion_ArmCallback_8(20, 0x5000, 20);
        Actor_SetAnimation(20, 3);
        Call1_02003f30(FieldScene_RunStepThen10, 0x6014);
        Actor_Jump(26, 2, 20);
        Actor_SetAnimation(26, 4);
        FieldScene_RunStepThen10(26);
        Actor_WalkToAndWait(20, 182, 0x280);
        Actor_FaceDirection(20, 0xd000, 0);
        Call1_02003f30(FieldScene_RunStepThen10, 0x8014);
        Actor_ShowEmote(26, 0x100, 20);
        Actor_StartRepeatedMotion(26, 2);
        FieldScene_RunStepThen10(26);
        Actor_SetAnimationAndWait(20, 3);
        FieldScene_CallPairWith10(22, 0);
        Actor_RunRepeatedMotion(22, 1);
        FieldScene_RunStepThen10(22);
        Actor_SetSpeed(22, 0x19999, 0xcccc);
        data_0200c918_addr = (s32)Data_0200c918;
        Actor_EnableActionCallback(22, data_0200c918_addr);
        Actor_SetSpeed(21, 0x19999, 0xcccc);
        Actor_WalkToAndWait(21, 168, 0x278);
        Actor_EnableActionCallback(21, data_0200c918_addr);
        Event_Wait(80);
        Actor_EnableActionCallback(26, data_0200c918_addr);
        Event_Wait(40);
        Call2(FieldScene_CallPairWith10, 20, 0x8000);
        FieldScene_RunStepThen10(data_00002014_addr);
        Value2(FieldScene_CallPairWith10, 0, 0xe000);
        Actor_SetAnimationAndWait(0, 3);
        Actor_SetAnimationAndWait(20, 3);
        SCENE_PHASE_02003f30 = 0x201;
        Event_CloseScreen();
        Event_WaitForScreen();
        Event_RequestExit(17);
    }
}

void FieldScene_RunScene3af_02004218(void)
{
    s32 record;

    Camera_MoveTo(0xe80000, -1, 0x2a40000, 0);
    Map_Redraw();
    Actor_SetPosition(0, 0xe80000, 0x2a40000);
    record = Func_0200854e(0);
    {
        s32 shown = 0x4000;

        *(u16 *)(record + 6) = shown;
    }
    Task_Wait(1);
}

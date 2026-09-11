#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_DIALOGUE_PLACEMENT
#define ALCHEMY_ABI_OVERLAYS_SCENE_DIALOGUE_PLACEMENT

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_020006d4(u8 *);
void Func_020006da(s32);
void Func_0200071a(s32, s32);
void Func_02000752(s32, s32);
void Func_020006f6(void);
void Func_02000756(s32);
void Func_02000758(s32, s32, s32);
void Func_0200016a(s32);
void Func_02000776(s32);
void Func_02000778(s32, s32, s32);
void Func_0200018a(s32);
void Func_02000796(s32);
void Func_02000798(s32, s32, s32);
void Func_020001aa(s32);
void Func_0200075c();
void Func_02000784();
void Func_020007ac();
void Func_020007ba();
void Func_020007bc();
void Func_020007c2();
void Func_020007c6();
void Func_020007c8();
s32 Func_020007dc();
void Func_020007ee();
void Func_020007f4();
void Func_02000806();
s32 Func_02000814();
void Func_0200081c();
void Func_02000822();
void Func_0200084a();
void Func_0200085e(s32);
void Func_02000860(s32, s32, s32);
void Func_02000272(s32);
void Func_0200087e(s32);
void Func_02000292(s32);
s32 Func_0200082c();
void Func_02000844();
void Func_0200088e();
s32 Func_02000898();
void Func_0200089c();
s32 Func_0200089e();
void Func_020008b2();
void Func_020008b6();
void Func_020008c0();
void Func_020008c6();
void Func_020008c6_a();
void Func_020008d6();
void Func_020008e6();
void Func_020008f0();
void Func_020008f2();
void Func_020008f4();
void Func_020008f8();
void Func_020008fe();
void Func_02000918();
void Func_02000926();
void Func_02000930();
void Func_02000940();
void Func_0200095a();
void Func_02000992();
void Func_0200093c(void);
void Func_0200098c(s32, s32);
void Func_02000950(void);
void Func_02000438(void);
void Func_0200099c(s32, s32);
void Func_02000978(void);
s32 Func_0200096c(void);
void Func_020009ec(s32);
void Func_02000a04(s32, s32);
void Func_02000a2e(s32, s32);
void Func_020009c6(s32, s32);
void Func_020009ba(void);
void Func_02000a6a(s32, s32);
void Func_02000a3a(s32);
void Func_02000a52(s32, s32);
void Func_020009f6(void);
void Func_02000a10(void);
void Func_02000ab2(s32, s32);
void Func_02000a82(s32);
void Func_02000a9a(s32, s32);
void Func_02000a3e(void);
void Func_02000a58(void);
void Func_02000afa(s32, s32);
void Func_02000aca(s32);
void Func_02000ae2(s32, s32);
void Func_02000a86(void);
void Func_02000aa0(void);
void Func_02000b4a(s32, s32);
void Func_02000b12(s32);
void Func_02000b2a(s32, s32);
void Func_02000ace(void);
void Func_02000ae0(void);
void Func_02000b48(s32);
void Func_02000b50(s32);
void Func_02000b68(s32, s32);
void Func_02000b0c(void);
void Func_02000b1a(s32, s32, s32, s32, s32, s32);
void Func_02000b34();
void Func_02000bc8(s32);
void Func_02000bc0(s32, s32, u8 *);
void Func_02000bbe(s32);

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ s32 Value0(s32 (*f)())
{
    void Func_02000880();
    void Func_02000976();

    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    void Func_02000880();
    void Func_02000976();

    return f(a0);
}

#define Actor_RunActor16StepWithFlag91 Func_020002fc
#define Actor_RunActorStep Func_02000098
#define Dialogue_RunActor11Line Func_020000dc
#define Dialogue_RunActor12Line Func_020000fc
#define Dialogue_RunActor16Line Func_020001c4
#define Dialogue_RunActor18FlaggedLine Func_020004a0
#define Dialogue_RunActor23Line Func_020001e4
#define Dialogue_RunActor9Line Func_020000bc
#define OvObj_InitObject22 Func_02000570
#define SceneData_GetEffectTable Func_020004dc
#define SceneData_GetMessageTable Func_02000078
#define SceneData_GetScriptTable Func_0200006c
#define SceneData_InitAndGetTable87f4 Func_02000080
#define SceneData_ReturnZero Func_02000074
#define Scene_InitSceneStateByStep Func_020004e4
#define Scene_RunActor16Sequence Func_0200011c
#define Scene_RunActor18ConditionalCue Func_02000338
#define Scene_RunActor18FlaggedSequence Func_02000204
#define Scene_RunActor19StepByPlace Func_02000380
#define Scene_RunActor20StepByPlace Func_020003c8
#define Scene_RunActor21StepByPlace Func_02000410
#define Scene_RunActor22StepByPlace Func_02000458
#define State_CheckPositionWindow Func_02000030
#define Talk_Apply Func_0200071a
#define Talk_Apply10 Func_02000ab2
#define Talk_Apply11 Func_02000a9a
#define Talk_Apply12 Func_02000afa
#define Talk_Apply13 Func_02000ae2
#define Talk_Apply14 Func_02000b4a
#define Talk_Apply15 Func_02000b2a
#define Talk_Apply16 Func_02000b68
#define Talk_Apply2 Func_02000752
#define Talk_Apply3 Func_0200098c
#define Talk_Apply4 Func_0200099c
#define Talk_Apply5 Func_02000a04
#define Talk_Apply6 Func_02000a2e
#define Talk_Apply7 Func_020009c6
#define Talk_Apply8 Func_02000a6a
#define Talk_Apply9 Func_02000a52
#define Talk_Check5 Func_0200096c
#define Talk_Do10 Func_02000756
#define Talk_Do11 Func_0200016a
#define Talk_Do12 Func_02000776
#define Talk_Do13 Func_0200018a
#define Talk_Do14 Func_02000796
#define Talk_Do15 Func_020001aa
#define Talk_Do16 Func_0200085e
#define Talk_Do17 Func_02000272
#define Talk_Do18 Func_0200087e
#define Talk_Do19 Func_02000292
#define Talk_Do20 Func_020009ec
#define Talk_Do21 Func_02000a3a
#define Talk_Do22 Func_02000a82
#define Talk_Do23 Func_02000aca
#define Talk_Do24 Func_02000b12
#define Talk_Do25 Func_02000b48
#define Talk_Do26 Func_02000b50
#define Talk_Do27 Func_02000bc8
#define Talk_Do28 Func_02000bbe
#define Talk_Do8 Func_020006d4
#define Talk_Do9 Func_020006da
#define Talk_Place3 Func_02000758
#define Talk_Place4 Func_02000778
#define Talk_Place5 Func_02000798
#define Talk_Place6 Func_02000860
#define Talk_Place7 Func_02000bc0
#define Talk_Run10 Func_02000b98
#define Talk_Run11 Func_020006f6
#define Talk_Run12 Func_0200075c
#define Talk_Run13 Func_020007bc
#define Talk_Run14 Func_020007ac
#define Talk_Run15 Func_020007ee
#define Talk_Run16 Func_020007c6
#define Talk_Run17 Func_02000784
#define Talk_Run18 Func_02000806
#define Talk_Run19 Func_0200081c
#define Talk_Run2 Func_020009ea
#define Talk_Run20 Func_0200084a
#define Talk_Run21 Func_020007f4
#define Talk_Run22 Func_02000880
#define Talk_Run23 Func_02000880_a
#define Talk_Run24 Func_0200089e_a
#define Talk_Run25 Func_02000976
#define Talk_Run26 Func_02000844
#define Talk_Run27 Func_020008c6
#define Talk_Run28 Func_020008d6
#define Talk_Run29 Func_020008f8
#define Talk_Run3 Func_02000a32
#define Talk_Run30 Func_0200088e
#define Talk_Run31 Func_020008e6
#define Talk_Run32 Func_0200089c
#define Talk_Run33 Func_020008f4
#define Talk_Run34 Func_020008b2
#define Talk_Run35 Func_02000930
#define Talk_Run36 Func_0200095a
#define Talk_Run37 Func_020008f2
#define Talk_Run38 Func_020008f0
#define Talk_Run39 Func_02000940
#define Talk_Run4 Func_02000a7a
#define Talk_Run40 Func_020008fe
#define Talk_Run41 Func_02000926
#define Talk_Run42 Func_02000976_a
#define Talk_Run43 Func_02000986
#define Talk_Run44 Func_0200093c
#define Talk_Run45 Func_02000950
#define Talk_Run46 Func_02000438
#define Talk_Run47 Func_02000986_a
#define Talk_Run48 Func_020009c8
#define Talk_Run49 Func_02000978
#define Talk_Run5 Func_02000ac2
#define Talk_Run50 Func_020009ba
#define Talk_Run51 Func_020009c8_a
#define Talk_Run52 Func_020009f6
#define Talk_Run53 Func_02000a10
#define Talk_Run54 Func_02000a3e
#define Talk_Run55 Func_02000a58
#define Talk_Run56 Func_02000a86
#define Talk_Run57 Func_02000aa0
#define Talk_Run58 Func_02000ace
#define Talk_Run59 Func_02000abe
#define Talk_Run6 Func_02000b80
#define Talk_Run60 Func_02000ae0
#define Talk_Run61 Func_02000b0c
#define Talk_Run62 Func_02000abe_a
#define Talk_Run63 Func_02000b34
#define Talk_Run7 Func_02000b8c
#define Talk_Run8 Func_02000b94
#define Talk_Run9 Func_02000b76
#define Talk_SetRect Func_02000b1a
#define gCell Data_02000240
#define gWork Data_03001ebc
#define Talk_Do(a0) Call1(Func_020007ba, a0)
#define Talk_Place(a0, a1, a2) Call3(Func_02000822, a0, a1, a2)
#define Talk_Check(a0, a1) Value2(Func_02000814, a0, a1)
#define Talk_Check2(a0, a1) Value2(Func_020007dc, a0, a1)
#define Talk_Do2(a0) Call1(Func_020007c2, a0)
#define Talk_Do3(a0) Call1(Func_020007c8, a0)
#define Talk_Check3(a0) Value1(Func_0200082c, a0)
#define Talk_Do4(a0) Call1(Func_020008b6, a0)
#define Talk_Do5(a0) Call1(Func_020008c6_a, a0)
#define Talk_Check4(a0, a1) Value2(Func_0200089e, a0, a1)
#define Talk_Run() Value0(Func_02000898)
#define Talk_Do6(a0) Call1(Func_02000918, a0)
#define Talk_Do7(a0) Call1(Func_020008c0, a0)
#define Talk_Place2(a0, a1, a2) Call3(Func_02000992, a0, a1, a2)

#endif

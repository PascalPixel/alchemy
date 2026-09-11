#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_MESSAGE_BRANCH
#define ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_MESSAGE_BRANCH

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_02000574(s32);
void Func_02000580(void);
void Func_02000602(s32, s32);
s32 Func_02000582(s32);
void Func_020005dc(s32);
void Func_020005e4(s32);
void Func_020005fc(s32, s32);
void Func_020005c0(void);
void Func_020005e0(void);
s32 Func_020005e2(s32);
void Func_0200063c(s32);
void Func_02000644(s32);
void Func_0200065c(s32, s32);
void Func_02000620(void);
void Func_02000648();
s32 Func_0200064a();
void Func_02000674();
void Func_020006a4();
void Func_020006a6();
void Func_020006ac();
void Func_020006ca();
void Func_020006d0();
s32 Func_020006dc();
void Func_020006e8();
void Func_020006f0();
void Func_020006c4(void);
void Func_02000702(s32);
void Func_02000704(s32, s32, s32);
void Func_02000724(s32, s32);
void Func_02000748(s32, s32, s32);
void Func_020006f4(void);
void Func_020006fc();
void Func_02000714();
void Func_0200073a();
s32 Func_02000744();
void Func_02000746();
void Func_0200075c();
void Func_02000760();
s32 Func_02000764();
void Func_02000790();
void Func_020007b4();
void Func_0200076c();
void Func_0200077a();
s32 Func_020007aa();
void Func_020007aa_a();
void Func_020007ac();
void Func_020007c6();
s32 Func_020007ca();
void Func_020007f6();
void Func_020007d0(void);
void Func_0200080e(s32);
s32 Func_02000836(s32, s32);
void Func_020007ea(void);
void Func_020007f0(void);
void Func_0200082e(s32);
s32 Func_02000856(s32, s32);
void Func_0200080a(void);
void Func_02000818(void);
void Func_020008a2(s32, s32);
void Func_02000874(s32);
void Func_02000884(s32);
void Func_0200089c(s32, s32);
void Func_0200089c_a(s32, s32);
void Func_02000860(void);
s32 Func_02000868();
void Func_02000878();
void Func_020008b6();
void Func_020008c2();
void Func_020008c4();
void Func_020008c4_a();
void Func_020008d6();
void Func_020008da();
void Func_020008ee();
void Func_020008fe();
void Func_02000904();
void Func_0200090e();
void Func_0200090e_a();
void Func_0200091c();
void Func_02000920();
void Func_02000934();
void Func_02000910(void);
void Func_0200094e(s32);
void Func_02000966(s32, s32);
void Func_02000978(s32, s32);
void Func_02000946(void);
void Func_0200094c(void);
void Func_0200098a(s32);
void Func_020009a2(s32, s32);
void Func_02000994(s32, s32, s32);
void Func_020009b4(s32, s32);
void Func_02000984(void);
void Func_0200098c(void);
void Func_020009ca(s32);
s32 Func_020009f2(s32, s32);
void Func_020009a6(void);
void Func_020009ac(void);
void Func_020009f4(s32);
void Func_020009fc(s32);

static __inline__ void Call1(void (*f)(), s32 a0)
{
    s32 Func_02000662();

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    s32 Func_02000662();

    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    s32 Func_02000662();

    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    s32 Func_02000662();

    f(a0, a1, a2);
}

static __inline__ void bump_step(s32 amount)
{
    s32 Func_02000662();

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    void Func_02000958();

    f(a0, a1);
}

#define Actor_Apply Func_02000602
#define Actor_Apply10 Func_02000966
#define Actor_Apply11 Func_02000978
#define Actor_Apply12 Func_020009a2
#define Actor_Apply13 Func_020009b4
#define Actor_Apply14 Func_020009f2
#define Actor_Apply2 Func_020005fc
#define Actor_Apply3 Func_0200065c
#define Actor_Apply4 Func_02000724
#define Actor_Apply5 Func_02000836
#define Actor_Apply6 Func_02000856
#define Actor_Apply7 Func_020008a2
#define Actor_Apply8 Func_0200089c
#define Actor_Apply9 Func_0200089c_a
#define Actor_Check10 Func_020005e2
#define Actor_Check9 Func_02000582
#define Actor_Do10 Func_020005e4
#define Actor_Do11 Func_0200063c
#define Actor_Do12 Func_02000644
#define Actor_Do13 Func_02000702
#define Actor_Do14 Func_0200080e
#define Actor_Do15 Func_0200082e
#define Actor_Do16 Func_02000874
#define Actor_Do17 Func_02000884
#define Actor_Do18 Func_0200094e
#define Actor_Do19 Func_0200098a
#define Actor_Do20 Func_020009ca
#define Actor_Do21 Func_020009f4
#define Actor_Do22 Func_020009fc
#define Actor_Do8 Func_02000574
#define Actor_Do9 Func_020005dc
#define Actor_Place6 Func_02000748
#define Actor_Place7 Func_02000704
#define Actor_Place8 Func_02000994
#define Actor_Run10 Func_02000620
#define Actor_Run11 Func_02000662_a
#define Actor_Run12 Func_02000648
#define Actor_Run13 Func_020006ca
#define Actor_Run14 Func_020006a6
#define Actor_Run15 Func_02000674
#define Actor_Run16 Func_020006e8
#define Actor_Run17 Func_020006ac
#define Actor_Run18 Func_020006c4
#define Actor_Run19 Func_020006f4
#define Actor_Run2 Func_0200059a
#define Actor_Run20 Func_020006fc
#define Actor_Run21 Func_0200075c
#define Actor_Run22 Func_02000746
#define Actor_Run23 Func_02000714
#define Actor_Run24 Func_02000790
#define Actor_Run25 Func_02000760
#define Actor_Run26 Func_0200081a
#define Actor_Run27 Func_0200076c
#define Actor_Run28 Func_020007ac
#define Actor_Run29 Func_0200077a
#define Actor_Run3 Func_020005fa
#define Actor_Run30 Func_020007f6
#define Actor_Run31 Func_020007c6
#define Actor_Run32 Func_020007d0
#define Actor_Run33 Func_020007ea
#define Actor_Run34 Func_020007f0
#define Actor_Run35 Func_0200080a
#define Actor_Run36 Func_0200081a_a
#define Actor_Run37 Func_02000818
#define Actor_Run38 Func_02000860
#define Actor_Run39 Func_02000958
#define Actor_Run4 Func_02000832
#define Actor_Run40 Func_02000878
#define Actor_Run41 Func_020008da
#define Actor_Run42 Func_020008ee
#define Actor_Run43 Func_020008d6
#define Actor_Run44 Func_020008b6
#define Actor_Run45 Func_0200090e
#define Actor_Run46 Func_020008c4
#define Actor_Run47 Func_0200090e_a
#define Actor_Run48 Func_02000934
#define Actor_Run49 Func_02000904
#define Actor_Run5 Func_020009d8
#define Actor_Run50 Func_02000958_a
#define Actor_Run51 Func_0200099a
#define Actor_Run52 Func_02000910
#define Actor_Run53 Func_02000946
#define Actor_Run54 Func_0200094c
#define Actor_Run55 Func_02000984
#define Actor_Run56 Func_02000a14
#define Actor_Run57 Func_0200098c
#define Actor_Run58 Func_020009a6
#define Actor_Run59 Func_0200099a_a
#define Actor_Run6 Func_02000662
#define Actor_Run60 Func_020009d8_a
#define Actor_Run61 Func_020009ac
#define Actor_Run62 Func_02000a14_a
#define Actor_Run7 Func_02000580
#define Actor_Run8 Func_020005c0
#define Actor_Run9 Func_020005e0
#define SceneData_GetPrimaryTable Func_02000030
#define SceneData_GetSecondaryTable Func_0200003c
#define SceneData_GetTertiaryTable Func_0200011c
#define SceneData_PrepareTable86b0 Func_02000044
#define SceneData_ReturnZero Func_02000038
#define Scene_ConfigureActor21Scene Func_020001a8
#define Scene_ConfigureActor22Scene Func_020003f4
#define Scene_ConfigureActor23Scene Func_02000430
#define Scene_RunActor10MessageBranch Func_02000490
#define Scene_RunActor13Message Func_020002d4
#define Scene_RunActor16MessageBranch Func_0200005c
#define Scene_RunActor17MessageBranch Func_02000124
#define Scene_RunActor18MessageBranch Func_020000bc
#define Scene_RunActor19MessageBranch Func_020002f4
#define Scene_RunActor21SequenceOnFlag300 Func_0200035c
#define Scene_RunActor24Sequence Func_020001e0
#define Scene_RunActor27Message Func_02000470
#define Scene_RunActor27Sequence Func_02000250
#define Scene_RunActor8Message Func_020002b4
#define Scene_SetupActor27OnEntry Func_020004c8
#define SetScale2 Func_020009d8
#define gWork Data_03001ebc
#define Actor_Check(a0) Value1(Func_02000662_a, a0)
#define Actor_Check2(a0) Value1(Func_0200064a, a0)
#define Actor_Do(a0) Call1(Func_020006a4, a0)
#define Actor_Check3(a0, a1) Value2(Func_020006dc, a0, a1)
#define Actor_Place(a0, a1, a2) Call3(Func_020006f0, a0, a1, a2)
#define Actor_Do2(a0) Call1(Func_020006d0, a0)
#define Actor_Do3(a0) Call1(Func_0200073a, a0)
#define Actor_Check4(a0, a1) Value2(Func_02000764, a0, a1)
#define Actor_Check5(a0, a1) Value2(Func_02000744, a0, a1)
#define Actor_Place2(a0, a1, a2) Call3(Func_020007b4, a0, a1, a2)
#define Actor_Do4(a0) Call1(Func_020007aa_a, a0)
#define Actor_Check6(a0, a1) Value2(Func_020007ca, a0, a1)
#define Actor_Check7(a0, a1) Value2(Func_020007aa, a0, a1)
#define Actor_Place3(a0, a1, a2) Call3(Func_0200081a, a0, a1, a2)
#define Actor_Check8(a0) Value1(Func_02000868, a0)
#define Actor_Do5(a0) Call1(Func_020008c2, a0)
#define Actor_Place4(a0, a1, a2) Call3(Func_020008fe, a0, a1, a2)
#define Actor_Run(a0, a1) Call2(Func_02000920, a0, a1)
#define Actor_Do6(a0) Call1(Func_020008c4_a, a0)
#define Actor_Do7(a0) Call1(Func_0200091c, a0)
#define Actor_Place5(a0, a1, a2) Call3(Func_02000958, a0, a1, a2)

#endif

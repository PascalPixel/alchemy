#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_STAGED_PLACEMENT_STAGED_PLACEMENT
#define ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_STAGED_PLACEMENT_STAGED_PLACEMENT

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_02001f8a();
void Func_02001fa4();
void Func_02001fc4_a();
void Func_02001fd0();
void Func_02001fe2();
void Func_02002006();
void Func_02001ec8(s32, s32, s32);
void Func_02001e92(s32, s32, s32);
void Func_02001ee2(s32, s32);
void Func_02001f1e(s32 actor, s32 mode);
void Func_020016ba();
void Func_02001730();
void Func_020018b0();
void Func_020019c8();
void Func_020019fa();
void Func_02001a2e();
void Func_02001a5c();
void Func_02001a8c();
void Func_02001abc();
s32 Func_02001fb4();
s32 Func_02001fde();
s32 Func_02001ff8();
s32 Func_0200202a();
s32 Func_0200203e();
s32 Func_02002054();
s32 Func_0200205a();
s32 Func_0200206e();
s32 Func_020020b4();
s32 Func_020020d0();
void Func_02002108();
void Func_02002124();
void Func_02002140();
s32 Func_02002142();
void Func_02002184();
void Func_0200218a();
s32 Func_020021aa();
void Func_020021ac();
void Func_020021cc();
void Func_020021ce();
s32 Func_020021d4();
void Func_020021d6();
void Func_020021e0();
s32 Func_020021ee();
void Func_020021f6();
void Func_02002204();
void Func_02002210();
void Func_02002220();
s32 Func_02002234();
s32 Func_02002250();
void Func_0200226e();
void Func_02002274();
void Func_02002282();
void Func_02002286();
void Func_0200228e();
void Func_02002296();
void Func_020022ba();
void Func_020022c4();
void Func_020022ee();
void Func_020022f2();
void Func_02002308();
void Func_02002312();
s32 Func_02002330();
s32 Func_02002342();
void Func_02002364();
s32 Func_0200236a();
s32 Func_02002372();
s32 Func_02002382();
s32 Func_0200238a();
s32 Func_020023b6();
s32 Func_020023be();
s32 Func_020023c6();
s32 Func_020023e8();
s32 Func_020023f0();
s32 Func_020023f8();
s32 Func_0200241c();
s32 Func_02002424();
void Func_0200243c();
s32 Func_0200244a();
s32 Func_02002452();
s32 Func_0200245a();
s32 Func_0200247a();
s32 Func_02002482();
s32 Func_0200248a();
void Func_02008bd1();
s32 Func_02002142_a();
s32 Func_020021ee_a();
void Func_02002204_a();
s32 Func_02002482_a();
void Func_0200251a();
void Func_020025a6();
void Func_020025da();
s32 Func_020025f8();
void Func_02002618();
void Func_0200262a();
void Func_0200262e();
void Func_02002634();
void Func_0200263c();
void Func_02002648();
void Func_02002652();
void Func_02002652_a();
void Func_02002654();
void Func_02002656();
void Func_0200265e();
void Func_02002664();
void Func_02002678();
void Func_02002046();
void Func_0200265e_a();
s32 Func_0200266a();
void Func_02002672();
void Func_0200269c();
s32 Func_020026aa();
s32 Func_020026d2();
void Func_020026e4();
void Func_02002706();
void Func_02002720();
s32 Func_02002730();
void Func_02002734();
s32 Func_0200273c();
void Func_0200274c();
void Func_0200277c();
s32 Func_0200279a();
s32 Func_0200279a_a();
void Func_020027c4();
void Func_020027c6();
void Func_02002824();
void Func_02002830();
void Func_020024cc(void);
void Func_02001f5c(void);
void Func_020024dc(void);
void Func_020024e0(void);
void Func_02001168(void);
void Func_02001f74(void);
void Func_020024f4(void);
void Func_020025a8(void);
s32 Func_02001598(struct StagedActor *arg0, s32 *arg1);
void Func_02001580(struct StagedActor *arg0, s32 arg1);
void Func_02001568(s32 arg0);
void Func_020016d6(s32 arg0);
void Func_020015b6(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_020015c6(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_020015d4(struct StagedActor *arg0);
void Func_020016f8(void);
void Func_020015ec(struct StagedActor *arg0, s32 arg1);

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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call7(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{
    f(a0, a1, a2, a3, a4, a5, a6);
}

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call1_0200115c(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call1_0200121c(void (*f)(), s32 a0)
{
    f(a0);
}

#define ActorProbeOffsets Data_020095ac
#define ActorSearchStep Data_02009554
#define Actor_Apply Func_02001ee2
#define Actor_Apply2 Func_02001f1e
#define Actor_Check25 Func_0200205a
#define Actor_Check26 Func_02001ff8
#define Actor_Check27 Func_020020d0
#define Actor_Check28 Func_0200206e
#define Actor_Check29 Func_02002142_a
#define Actor_Check30 Func_02002250
#define Actor_Check31 Func_020021ee_a
#define Actor_Check32 Func_020023c6
#define Actor_Check33 Func_020023f8
#define Actor_Check34 Func_0200245a
#define Actor_Check35 Func_0200248a
#define Actor_Check36 Func_02002482_a
#define Actor_Check37 Func_020025f8
#define Actor_Check38 Func_02002730
#define Actor_Check39 Func_0200279a
#define Actor_Place17 Func_02001ec8
#define Actor_Place18 Func_02001e92
#define Actor_Run10 Func_02001fc4_a
#define Actor_Run11 Func_02001f8a
#define Actor_Run12 Func_02001fd0
#define Actor_Run13 Func_02001fa4
#define Actor_Run14 Func_02001fe2
#define Actor_Run15 Func_02002006
#define Actor_Run16 Func_02002062
#define Actor_Run17 Func_020020d8
#define Actor_Run18 Func_0200215e
#define Actor_Run19 Func_0200216e
#define Actor_Run20 Func_02002182
#define Actor_Run21 Func_020021da
#define Actor_Run22 Func_020021e4
#define Actor_Run23 Func_02002216
#define Actor_Run24 Func_02002258
#define Actor_Run25 Func_02002360
#define Actor_Run26 Func_02002392
#define Actor_Run27 Func_0200242c
#define Actor_Run28 Func_020021e4_a
#define Actor_Run29 Func_02002646
#define Actor_Run30 Func_0200268a
#define Actor_Run31 Func_02002748
#define Actor_Run32 Func_02000ce0
#define Actor_Run33 Func_02002124
#define Actor_Run34 Func_02002140
#define Actor_Run35 Func_02002184
#define Actor_Run36 Func_02008bd1
#define Actor_Run37 Func_02002108
#define Actor_Run38 Func_0200226e
#define Actor_Run39 Func_020021e0
#define Actor_Run40 Func_020021ac
#define Actor_Run41 Func_020021f6
#define Actor_Run42 Func_02002204
#define Actor_Run43 Func_020021d6
#define Actor_Run44 Func_020021cc
#define Actor_Run45 Func_0200218a
#define Actor_Run46 Func_02002210
#define Actor_Run47 Func_020022ee
#define Actor_Run48 Func_02002286
#define Actor_Run49 Func_02002364
#define Actor_Run50 Func_0200228e
#define Actor_Run51 Func_02002308
#define Actor_Run52 Func_020022f2
#define Actor_Run53 Func_02000ecc
#define Actor_Run54 Func_0200243c
#define Actor_Run55 Func_020019c8
#define Actor_Run56 Func_020019fa
#define Actor_Run57 Func_02001a2e
#define Actor_Run58 Func_02001a5c
#define Actor_Run59 Func_02001a8c
#define Actor_Run60 Func_02001abc
#define Actor_Run61 Func_020025a6
#define Actor_Run62 Func_0200262a
#define Actor_Run63 Func_02002664
#define Actor_Run64 Func_02002634
#define Actor_Run65 Func_020025da
#define Actor_Run66 Func_02002678
#define Actor_Run67 Func_02002652_a
#define Actor_Run68 Func_02002720
#define Actor_Run69 Func_02002824
#define Actor_Run70 Func_02002830
#define Actor_Run71 Func_0200277c
#define Actor_Run72 Func_02002046
#define Actor_Run73 Func_020024cc
#define Actor_Run74 Func_02001f5c
#define Actor_Run75 Func_020024dc
#define Actor_Run76 Func_020024e0
#define Actor_Run77 Func_02001168
#define Actor_Run78 Func_02001f74
#define Actor_Run79 Func_020024f4
#define Actor_Run80 Func_020025a8
#define Actor_Run9 Func_02001fc4
#define CanStartStagedActorMove Func_02001598
#define ConfigureActorThirteenSceneParameters Func_020009dc
#define ConfigureSceneForActorEightColumn Func_02000b6c
#define Effect_AdvancePositionByAxisMode Func_02000a00
#define FindBlockingStagedActor Func_020001a2
#define FindElevatedBlockingStagedActor Func_020001ce
#define FindNextStagedActor Func_02000176
#define FinishStagedActorEffect Func_020016f8
#define FinishStagedActorMove Func_020015d4
#define GetStagedActor Func_02001524
#define OvObj_ApplyValue15 Func_02000a58
#define SceneData_GetTable9690 Func_02000b50
#define SceneData_GetTable9750 Func_02000b5c
#define SceneData_GetTable976c Func_02000b64
#define SceneData_GetTable9814 Func_020010c0
#define SceneData_ReturnZero Func_02000b58
#define Scene_Forward Func_020010b4
#define Scene_RunFourStepSequence Func_0200109c
#define Scene_RunPrimarySequence Func_02000bd0
#define Scene_RunScene389 Func_0200121c
#define Scene_RunThreeCallSequence Func_02001088
#define SelectStagedActorSlot Func_02001568
#define SetStagedActorMode Func_02001580
#define SetStagedActorTransition Func_020015ec
#define StagedActorStepTable Data_02009554
#define StagedActor_AdvanceActorPair Func_020000c4
#define StagedActor_FindClearPosition Func_02000474
#define StartLeadStagedActorMove Func_020015c6
#define StartNextStagedActorMove Func_020015b6
#define StartStagedActorEffect Func_020016d6
#define TestActorPosition Func_02001954
#define gCell Data_02000240
#define gOv Data_020098ec
#define gWork Data_03001ebc
#define Actor_Check(a0) Value1(Func_0200203e, a0)
#define Actor_Run() Value0(Func_02001fb4)
#define Actor_Run2() Value0(Func_02001fde)
#define Actor_SetRect(a0, a1, a2, a3, a4, a5, a6) Call7(Func_020016ba, a0, a1, a2, a3, a4, a5, a6)
#define Actor_Check2(a0) Value1(Func_020020b4, a0)
#define Actor_Run3() Value0(Func_0200202a)
#define Actor_Run4() Value0(Func_02002054)
#define Actor_SetRect2(a0, a1, a2, a3, a4, a5, a6) Call7(Func_02001730, a0, a1, a2, a3, a4, a5, a6)
#define Actor_Check3(a0) Value1(Func_02002142, a0)
#define Actor_Run5(a0, a1) Call2((void (*)())Func_020021e4, a0, a1)
#define Actor_Place(a0, a1, a2) Call3(Func_020021ce, a0, a1, a2)
#define Actor_Do(a0) Call1((void (*)())Func_020021ee, a0)
#define Actor_Place2(a0, a1, a2) Call3(Func_02002204_a, a0, a1, a2)
#define Actor_Do2(a0) Call1(Func_020022c4, a0)
#define Actor_Place3(a0, a1, a2) Call3(Func_02002220, a0, a1, a2)
#define Actor_Check4(a0) Value1(Func_02002234, a0)
#define Actor_Run6() Value0(Func_020021aa)
#define Actor_Run7() Value0(Func_020021d4)
#define Actor_SetRect3(a0, a1, a2, a3, a4, a5, a6) Call7(Func_020018b0, a0, a1, a2, a3, a4, a5, a6)
#define Actor_Place4(a0, a1, a2) Call3(Func_02002274, a0, a1, a2)
#define Actor_Place5(a0, a1, a2) Call3(Func_02002282, a0, a1, a2)
#define Actor_Place6(a0, a1, a2) Call3(Func_02002312, a0, a1, a2)
#define Actor_Do3(a0) Call1(Func_020022ba, a0)
#define Actor_SetRect4(a0, a1, a2, a3, a4, a5) Call6(Func_02002296, a0, a1, a2, a3, a4, a5)
#define Actor_Check5(a0) Value1(Func_02002330, a0)
#define Actor_Check6(a0) Value1(Func_02002342, a0)
#define Actor_Check7(a0) Value1(Func_0200236a, a0)
#define Actor_Check8(a0) Value1(Func_02002372, a0)
#define Actor_Check9(a0) Value1(Func_02002382, a0)
#define Actor_Check10(a0) Value1(Func_0200238a, a0)
#define Actor_Check11(a0) Value1(Func_020023b6, a0)
#define Actor_Check12(a0) Value1(Func_020023be, a0)
#define Actor_Check13(a0) Value1(Func_020023e8, a0)
#define Actor_Check14(a0) Value1(Func_020023f0, a0)
#define Actor_Check15(a0) Value1(Func_0200241c, a0)
#define Actor_Check16(a0) Value1(Func_02002424, a0)
#define Actor_Check17(a0) Value1(Func_0200244a, a0)
#define Actor_Check18(a0) Value1(Func_02002452, a0)
#define Actor_Check19(a0) Value1(Func_0200247a, a0)
#define Actor_Check20(a0) Value1(Func_02002482, a0)
#define Actor_Do4(a0) Call1_0200115c(Func_0200251a, a0)
#define Actor_Place7(a0, a1, a2) Call3(Func_02002652, a0, a1, a2)
#define Actor_Place8(a0, a1, a2) Call3(Func_0200265e, a0, a1, a2)
#define Actor_Do5(a0) Call1_0200115c(Func_02002654, a0)
#define Actor_Place9(a0, a1, a2) Call3(Func_02002618, a0, a1, a2)
#define Actor_Place10(a0, a1, a2) Call3(Func_0200263c, a0, a1, a2)
#define Actor_Place11(a0, a1, a2) Call3(Func_02002648, a0, a1, a2)
#define Actor_Place12(a0, a1, a2) Call3(Func_02002656, a0, a1, a2)
#define Actor_Do6(a0) Call1_0200115c(Func_0200262e, a0)
#define Actor_Check21(a0) Value1(Func_0200266a, a0)
#define Actor_Place13(a0, a1, a2) Call3(Func_020026e4, a0, a1, a2)
#define Actor_SetRect5(a0, a1, a2, a3, a4, a5) Call6(Func_0200265e_a, a0, a1, a2, a3, a4, a5)
#define Actor_SetRect6(a0, a1, a2, a3, a4, a5) Call6(Func_02002672, a0, a1, a2, a3, a4, a5)
#define Actor_Check22(a0) Value1(Func_020026aa, a0)
#define Actor_SetRect7(a0, a1, a2, a3, a4, a5) Call6(Func_0200269c, a0, a1, a2, a3, a4, a5)
#define Actor_Check23(a0) Value1(Func_020026d2, a0)
#define Actor_Place14(a0, a1, a2) Call3(Func_0200274c, a0, a1, a2)
#define Actor_Check24(a0) Value1(Func_0200273c, a0)
#define Actor_SetRect8(a0, a1, a2, a3, a4, a5) Call6(Func_02002706, a0, a1, a2, a3, a4, a5)
#define Actor_Place15(a0, a1, a2) Call3(Func_020027c6, a0, a1, a2)
#define Actor_Place16(a0, a1, a2) Call3(Func_020027c4, a0, a1, a2)
#define Actor_Do7(a0) Call1_0200121c((void (*)())Func_0200279a_a, a0)
#define Actor_Run8(a0, a1) Call2(Func_02002734, a0, a1)

#endif

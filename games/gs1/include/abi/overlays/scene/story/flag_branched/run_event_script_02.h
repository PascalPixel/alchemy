#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_STORY_FLAG_BRANCHED_RUN_EVENT_SCRIPT_02
#define ALCHEMY_ABI_OVERLAYS_SCENE_STORY_FLAG_BRANCHED_RUN_EVENT_SCRIPT_02

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_020035de();
void Func_020036ec();
void Func_02003748();
void Func_0200376a();
void Func_020037d4();
void Func_02003846();
void Func_020038ac();
void Func_020038fc();
void Func_02003dd4();
void Func_02003df4();
void Func_02003e08();
void Func_02003e1a();
void Func_02003e24();
void Func_02003e8c();
void Func_02003eee();
void Func_02003ef0();
void Func_02003efc();
void Func_02003f06();
s32 Func_02003f10();
void Func_02003f12();
void Func_02003f28();
void Func_02003f34();
void Func_02003f44();
void Func_02003f46();
void Func_02003f52();
void Func_02003f66();
void Func_02003f7e();
void Func_02003f8c();
void Func_02003f96();
void Func_02003fa4();
void Func_02003fae();
void Func_02003fb2();
void Func_02003fbc();
void Func_02003fc6();
void Func_02003fea();
void Func_02003ff0();
void Func_02003ff8();
void Func_02004002();
void Func_02004010();
void Func_0200401e();
void Func_02004024();
void Func_02004032();
void Func_02004034();
void Func_02004046();
s32 Func_0200405a();
void Func_0200406a();
void Func_02004078();
void Func_0200407c();
void Func_0200407e();
void Func_02004080();
void Func_02004086();
void Func_02004088();
void Func_02004090();
s32 Func_0200409a();
void Func_0200409e();
void Func_020040ac();
void Func_020040b6();
void Func_020040be();
void Func_020040c6();
void Func_020040da();
s32 Func_020040e2();
void Func_020040f4();
void Func_02004138();
void Func_02004144();
void Func_02004146();
void Func_02004156();
void Func_02004158();
void Func_02004180();
void Func_02004186();
void Func_020041b6();
void Func_020041c0();
s32 Func_020041c4();
void Func_0200420a();
void Func_02004214();
void Func_02004222();
s32 Func_0200422e();
void Func_02004232();
void Func_0200423e();
void Func_02004250();
void Func_02004256();
void Func_0200425c();
void Func_0200426a();
void Func_02004272();
void Func_0200428e();
void Func_020042a0();
void Func_020042a8();
s32 Func_020042aa();
void Func_020042ca();
s32 Func_020042d0();
void Func_020042d6();
void Func_02004320();
void Func_02004324();
void Func_02004330();
s32 Func_0200434e();
void Func_0200436a();
void Func_02004382();
void Func_02004384();
void Func_020043a0();
void Func_020043aa();
void Func_020043c2();
void Func_020043c8();
void Func_020043de();
void Func_020043e4();
void Func_020043ea();
void Func_020043ec();
s32 Func_020043ee();
void Func_02004400();
void Func_02004402();
void Func_02004404();
void Func_02004412();
void Func_0200441e();
void Func_0200443a();
void Func_02004450();
void Func_02004464();
void Func_0200447a();
void Func_02004488();
void Func_02004496();
void Func_020044a0();
void Func_020044c6();
void Func_020044ca();
void Func_020044d2();
void Func_020044d4();
void Func_02004502();
void Func_02004518();
void Func_0200451a();
void Func_02004522();
void Func_02004526();
void Func_0200452a();
void Func_02004530();
void Func_02004538();
void Func_0200453a();
void Func_02004540();
void Func_02004542();
void Func_0200454e();
void Func_02004550();
void Func_02004558();
void Func_02004560();
void Func_02004562();
void Func_02004580();
void Func_02004586();
void Func_0200458e();
void Func_020045a0();
void Func_020045a8();
void Func_020045aa();
s32 Func_020045b0();
void Func_020045b2();
void Func_020045ba();
void Func_020045c8();
void Func_020045ce();
void Func_020045dc();
void Func_020045f2();
void Func_0200460c();
void Func_0200460e();
void Func_02004610();
void Func_02004624();
void Func_02004634();
void Func_02004638();
void Func_02004670();
void Func_02004678();
void Func_020046f0();
void Func_020046fc();
void Func_02004706();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

#define RunEventScript02 Func_020019b0
#define Story_Check10 Func_020042aa
#define Story_Check11 Func_020043ee
#define Story_Check9 Func_0200422e
#define Story_Run10 Func_02003fd8
#define Story_Run100 Func_020045aa
#define Story_Run101 Func_020045a8
#define Story_Run102 Func_020045ba
#define Story_Run103 Func_02004530
#define Story_Run104 Func_020045c2
#define Story_Run105 Func_02004638
#define Story_Run106 Func_02004586
#define Story_Run107 Func_02004678
#define Story_Run108 Func_020046f0
#define Story_Run109 Func_020046fc
#define Story_Run11 Func_020044de
#define Story_Run110 Func_02004706
#define Story_Run111 Func_020045f2
#define Story_Run12 Func_020044fe
#define Story_Run13 Func_0200451e
#define Story_Run14 Func_02003e24
#define Story_Run15 Func_02003dd4
#define Story_Run16 Func_02003df4
#define Story_Run17 Func_02003eee
#define Story_Run18 Func_02003e1a
#define Story_Run19 Func_02003e08
#define Story_Run20 Func_02003fb2
#define Story_Run21 Func_02003fc6
#define Story_Run22 Func_02003e8c
#define Story_Run23 Func_02003fd2
#define Story_Run24 Func_02003ef0
#define Story_Run25 Func_02003f7e
#define Story_Run26 Func_02003f96
#define Story_Run27 Func_02003f8c
#define Story_Run28 Func_02003fa4
#define Story_Run29 Func_02003f12
#define Story_Run30 Func_02004146
#define Story_Run31 Func_02004024
#define Story_Run32 Func_02003fae
#define Story_Run33 Func_0200401e
#define Story_Run34 Func_02004032
#define Story_Run35 Func_02004046
#define Story_Run36 Func_02004078
#define Story_Run37 Func_02004080
#define Story_Run38 Func_02004088
#define Story_Run39 Func_02004086
#define Story_Run40 Func_02004090
#define Story_Run41 Func_0200409e
#define Story_Run42 Func_020040b6
#define Story_Run43 Func_020040be
#define Story_Run44 Func_020040c6
#define Story_Run45 Func_02004034
#define Story_Run46 Func_020040da
#define Story_Run47 Func_020040ac
#define Story_Run48 Func_0200406a
#define Story_Run49 Func_02004186
#define Story_Run50 Func_020035de
#define Story_Run51 Func_020040f4
#define Story_Run52 Func_02004158
#define Story_Run53 Func_02004222
#define Story_Run54 Func_0200423e
#define Story_Run55 Func_02004290
#define Story_Run56 Func_02004250
#define Story_Run57 Func_020041b6
#define Story_Run58 Func_020042a0
#define Story_Run59 Func_020036ec
#define Story_Run6 Func_02004510
#define Story_Run60 Func_02003748
#define Story_Run61 Func_02004232
#define Story_Run62 Func_0200435c
#define Story_Run63 Func_0200376a
#define Story_Run64 Func_020037d4
#define Story_Run65 Func_02004382
#define Story_Run66 Func_020043c2
#define Story_Run67 Func_020043f4
#define Story_Run68 Func_020043ea
#define Story_Run69 Func_020043c8
#define Story_Run7 Func_02003f58
#define Story_Run70 Func_02003846
#define Story_Run71 Func_02004330
#define Story_Run72 Func_0200442c
#define Story_Run73 Func_020043e4
#define Story_Run74 Func_020043ec
#define Story_Run75 Func_02004404
#define Story_Run76 Func_0200436a
#define Story_Run77 Func_02004412
#define Story_Run78 Func_02004490
#define Story_Run79 Func_02004450
#define Story_Run8 Func_02003fb0
#define Story_Run80 Func_020038ac
#define Story_Run81 Func_020038fc
#define Story_Run82 Func_020044ca
#define Story_Run83 Func_02004516
#define Story_Run84 Func_020044c6
#define Story_Run85 Func_020044a0
#define Story_Run86 Func_020044d4
#define Story_Run87 Func_02004526
#define Story_Run88 Func_0200451a
#define Story_Run89 Func_02004522
#define Story_Run9 Func_02003fc4
#define Story_Run90 Func_0200452a
#define Story_Run91 Func_0200453a
#define Story_Run92 Func_02004540
#define Story_Run93 Func_02004518
#define Story_Run94 Func_02004560
#define Story_Run95 Func_02004538
#define Story_Run96 Func_02004580
#define Story_Run97 Func_02004558
#define Story_Run98 Func_0200458e
#define Story_Run99 Func_020045a0
#define gOv Data_0200a5c0
#define gOv2 Data_0200a5d4
#define gWork Data_03001ebc
#define Story_Run(a0, a1, a2, a3) Call4(Func_02003f46, a0, a1, a2, a3)
#define Story_Run2(a0, a1, a2, a3) Call4(Func_02003f66, a0, a1, a2, a3)
#define Story_Place(a0, a1, a2) Call3(Func_02003f28, a0, a1, a2)
#define Story_Place2(a0, a1, a2) Call3(Func_02003f34, a0, a1, a2)
#define Story_Run3(a0, a1) Call2(Func_02003fbc, a0, a1)
#define Story_Place3(a0, a1, a2) Call3(Func_02003efc, a0, a1, a2)
#define Story_Place4(a0, a1, a2) Call3(Func_02003f06, a0, a1, a2)
#define Story_Place5(a0, a1, a2) Call3(Func_02003f44, a0, a1, a2)
#define Story_Place6(a0, a1, a2) Call3(Func_02003f52, a0, a1, a2)
#define Story_Do(a0) Call1(Func_02003ff0, a0)
#define Story_Check(a0) Value1(Func_02003f10, a0)
#define Story_Run4(a0, a1, a2, a3) Call4(Func_0200407e, a0, a1, a2, a3)
#define Story_Place7(a0, a1, a2) Call3(Func_02003fea, a0, a1, a2)
#define Story_Place8(a0, a1, a2) Call3(Func_02003ff8, a0, a1, a2)
#define Story_Place9(a0, a1, a2) Call3(Func_02004002, a0, a1, a2)
#define Story_Place10(a0, a1, a2) Call3(Func_02004010, a0, a1, a2)
#define Story_Place11(a0, a1, a2) Call3(Func_0200407c, a0, a1, a2)
#define Story_Place12(a0, a1, a2) Call3(Func_02004138, a0, a1, a2)
#define Story_Place13(a0, a1, a2) Call3(Func_02004144, a0, a1, a2)
#define Story_Place14(a0, a1, a2) Call3(Func_02004156, a0, a1, a2)
#define Story_Check2(a0) Value1(Func_0200405a, a0)
#define Story_Place15(a0, a1, a2) Call3(Func_02004180, a0, a1, a2)
#define Story_Check3(a0) Value1(Func_0200409a, a0)
#define Story_Place16(a0, a1, a2) Call3(Func_020041c0, a0, a1, a2)
#define Story_Check4(a0) Value1(Func_020040e2, a0)
#define Story_Place17(a0, a1, a2) Call3(Func_0200420a, a0, a1, a2)
#define Story_Place18(a0, a1, a2) Call3(Func_02004214, a0, a1, a2)
#define Story_Place19(a0, a1, a2) Call3(Func_0200425c, a0, a1, a2)
#define Story_Place20(a0, a1, a2) Call3(Func_02004256, a0, a1, a2)
#define Story_Place21(a0, a1, a2) Call3(Func_02004272, a0, a1, a2)
#define Story_Run5(a0, a1) Call2(Func_0200426a, a0, a1)
#define Story_Place22(a0, a1, a2) Call3(Func_0200428e, a0, a1, a2)
#define Story_Place23(a0, a1, a2) Call3(Func_020042a8, a0, a1, a2)
#define Story_Place24(a0, a1, a2) Call3(Func_020042ca, a0, a1, a2)
#define Story_Place25(a0, a1, a2) Call3(Func_020042d6, a0, a1, a2)
#define Story_Check5(a0) Value1(Func_020041c4, a0)
#define Story_Check6(a0, a1) Value2(Func_020042d0, a0, a1)
#define Story_Place26(a0, a1, a2) Call3(Func_02004320, a0, a1, a2)
#define Story_Place27(a0, a1, a2) Call3(Func_02004324, a0, a1, a2)
#define Story_Check7(a0, a1) Value2(Func_0200434e, a0, a1)
#define Story_Do2(a0) Call1(Func_02004384, a0)
#define Story_Place28(a0, a1, a2) Call3(Func_020043aa, a0, a1, a2)
#define Story_Do3(a0) Call1(Func_020043a0, a0)
#define Story_Place29(a0, a1, a2) Call3(Func_020043de, a0, a1, a2)
#define Story_Place30(a0, a1, a2) Call3(Func_020043ea, a0, a1, a2)
#define Story_Place31(a0, a1, a2) Call3(Func_02004400, a0, a1, a2)
#define Story_Place32(a0, a1, a2) Call3(Func_02004402, a0, a1, a2)
#define Story_Place33(a0, a1, a2) Call3(Func_0200441e, a0, a1, a2)
#define Story_Place34(a0, a1, a2) Call3(Func_0200443a, a0, a1, a2)
#define Story_Place35(a0, a1, a2) Call3(Func_02004464, a0, a1, a2)
#define Story_Place36(a0, a1, a2) Call3(Func_02004488, a0, a1, a2)
#define Story_Place37(a0, a1, a2) Call3(Func_0200447a, a0, a1, a2)
#define Story_Place38(a0, a1, a2) Call3(Func_02004496, a0, a1, a2)
#define Story_Place39(a0, a1, a2) Call3(Func_020044d2, a0, a1, a2)
#define Story_Place40(a0, a1, a2) Call3(Func_02004502, a0, a1, a2)
#define Story_Place41(a0, a1, a2) Call3(Func_02004542, a0, a1, a2)
#define Story_Place42(a0, a1, a2) Call3(Func_0200454e, a0, a1, a2)
#define Story_Place43(a0, a1, a2) Call3(Func_02004550, a0, a1, a2)
#define Story_Place44(a0, a1, a2) Call3(Func_02004562, a0, a1, a2)
#define Story_Place45(a0, a1, a2) Call3(Func_02004634, a0, a1, a2)
#define Story_Place46(a0, a1, a2) Call3(Func_0200460e, a0, a1, a2)
#define Story_Place47(a0, a1, a2) Call3(Func_020045ce, a0, a1, a2)
#define Story_Place48(a0, a1, a2) Call3(Func_020045dc, a0, a1, a2)
#define Story_Place49(a0, a1, a2) Call3(Func_02004670, a0, a1, a2)
#define Story_Place50(a0, a1, a2) Call3(Func_0200460c, a0, a1, a2)
#define Story_Place51(a0, a1, a2) Call3(Func_02004610, a0, a1, a2)
#define Story_Place52(a0, a1, a2) Call3(Func_02004624, a0, a1, a2)
#define Story_Do4(a0) Call1(Func_020045b2, a0)
#define Story_Check8(a0) Value1(Func_020045b0, a0)
#define Story_Do5(a0) Call1(Func_020045c8, a0)

#endif

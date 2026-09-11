#include "types.h"

#define SceneData_GetTableC194 Func_02000030
#define SceneData_GetTablec1dc Func_0200003c
#define SceneData_GetTablec1f4 Func_02000044
#define SceneData_SelectBlockAndResetCounters Func_020026a8
#define Object_SetModeById_1(args...) Func_02003dda(args)
#define Object_SetModeById_2(args...) Func_02003daa(args)
#define Object_SetModeById_3(args...) Func_02003dda(args)
#define Object_SetModeById_4(args...) Func_02003e0a(args)
#define AUX_COUNTDOWN (*(volatile s32 *)Data_0200c41c)
#define Scene_RunOpeningAuxiliarySequence Func_0200004c
#define FieldScene_RunScene3ba_02000270 Func_02000270
#define FieldScene_RunScene3ba_02000974 Func_02000974
#define Scene_RunCommandSequence Func_02000db8
#define FieldScene_RunScene3ba_020015e0 Func_020015e0
#define Scene_RunNearestActor165Scene Func_02001a28
#define State_ResetCounterAndStartTask Func_02000134
#define State_SetMode66AndPassOpeningSequence Func_02000158
#define State_ApplyRectAndSend303 Func_02000248
#define State_ApplyValue768 Func_02000830
#define State_SetStateHalfword386To99WhenMatched Func_020019e0
#define State_SendIdBySceneId Func_02001e20
#define State_InitHalfwordC6a6Once Func_02002bcc
#define State_StoreParamsAndInitTable Func_02002d28
#define State_InitTableWordsAndLoad3200 Func_02002d8c
#define State_ReleaseTableAndResetC6a6 Func_02002de8
#define State_WaitUntilWord1000IsNine Func_02002fa0
#define State_SetStateHalfword220 Func_020038a0
#define State_WaitUntilWordC41cIs22 Func_02000178
#define Dialogue_RunFlagGatedPromptInteraction Func_02001d64
#define State_SetHalfword1000To9 Func_02002f94
#define State_ApplyRectsForActorsNineAndTen Func_020001a8
#define Scene_RunTwoCallSequence Func_02000238
#define Scene_RunBranchedStep Func_02000954
#define SceneData_GetTablec420 Func_0200096c
#define Scene_BuildSceneDescriptorAndInstallTask Func_02003764
#define State_InitControlWhenFlag109Clear Func_0200384c
#define StagedActor_PlacePairAtOffsetAndRun Func_0200033c
#define Actor_ShiftActorSeventeenByLeaderRow Func_0200042c
#define Actor_ShiftActorEighteenByInputAndLeaderColumn Func_02000490
#define Actor_MarkObjectAtTiles94To95 Func_02000a10
#define Actor_PlaceSlots1To3FromWork Func_02001898
#define Actor_PlaceLinkedActorAbove Func_02003058
#define Scene_RunSceneFourCoordinator Func_020016ec
#define SceneTransition_Phase Data_02000240[225]
#define GetPartyInteractionRecord Func_0200593a
#define FinishPartyInteractionRecordRead Func_0200592e
#define GetPartyMemberCount Func_0200590a
#define SetInteractionCue Func_020059cc
#define CheckActorInteraction Func_020059ec
#define PlaceSelectedActor Func_020059b0
#define SetInteractionStep Func_0200596e
#define PlaceActorZero Func_020059c8
#define PlaceSupportActor Func_020059d4
#define StartInteractionPhase Func_02005aa8
#define AdvanceInteractionPhase Func_02005ab4
#define SelectInteractionStep Func_02005a82
#define SetLargePartyInteractionCue Func_02005a32
#define RunLargePartyInteraction Func_02005a4a_a
#define FinishPartyCountInteraction Func_020059b6
#define RunPartyCountInteraction Func_02001c7c
#define Actor_ApplyValueAndMatchingSlots Func_020020e8
#define OvObj_ResetMotionFields Func_02002bac
#define OvObj_PlaceWithScale14000 Func_02002e50
#define Actor_StartMode5MoveToTile Func_02002e10
#define HexDigits Data_0200bfd0
#define Text_WriteU32AsHex Func_02002f64
#define Effect_SpawnKind285AtRandomChance Func_02002fc4
#define Actor_FindSlotAtTilePosition Func_020038b0
#define StagedActor_PushActorAhead Func_020038f8
#define Actor_FindOccupantAheadOfSubject Func_02003a60
#define Scene_RunSixSteps380To3A8 Func_02001d20
#define Scene_RunMiddleSequence Func_02001e7c
#define Scene_RunTwoArmSequence Func_02002738
#define Scene_RunLateSequence Func_02002844
#define Resource3ba_NoOpCallback Func_02002f90
#define Scene_RunFlag211ApproachScene Func_020030ac

typedef struct Sub {
    u8 pad00[0x28];
    s16 *f28;
} Sub;

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[0x3c];
    Sub *f50;
    u8 f54;
} Obj;

typedef struct Obj_020001a8 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[15];
    u8 f23;
    u8 f24[49];
    u8 f55;
} Obj_020001a8;

typedef struct Ctl {
    s16 f0;
    s16 f2;
    s16 f4;
    s16 f6;
    s16 f8;
} Ctl;

typedef struct Obj_0200033c {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[28];
    s32 f30;
    s32 f34;
} Obj_0200033c;

typedef struct Obj_0200042c {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
} Obj_0200042c;

typedef struct Obj_02000a10 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[14];
    u8 f22;
} Obj_02000a10;

typedef struct Obj_02003058 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[65];
    u8 f55;
    u8 f56[14];
    s16 f64;
} Obj_02003058;

typedef struct PartyInteractionRecord {
    u8 padding_00[10];
    s16 x;
    u8 padding_0c[6];
    s16 y;
} PartyInteractionRecord;

typedef struct Rec {
    u8 pad00[216];
    u16 fd8[15];
} Rec;

/* The two mode records the entry point seeds; the halfword at +26 holds the
 * per-mode span in sixtieths. */
struct ModeRecord {
    u8 pad[26];
    u16 span;
};

typedef struct Obj_02002bac {
    u32 f00[9];
    s32 f24;
    s32 f28;
    s32 f2c;
    s32 f30;
    s32 f34;
    u32 f38;
    s32 f3c;
    u32 f40;
} Obj_02002bac;

typedef struct Obj_02002e50 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    u8 f10[32];
    s32 f30;
    s32 f34;
    u8 f38[35];
    u8 f5b;
} Obj_02002e50;

typedef struct Obj_02002e10 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    u8 f10[32];
    s32 f30;
    s32 f34;
    u8 f38[35];
    u8 f5b;
} Obj_02002e10;

typedef struct Obj_02002fc4 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[20];
    s32 f28;
    u8 f2c[41];
    u8 f55;
} Obj_02002fc4;

typedef struct SceneRecord {
    u8 pad_00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    u8 pad_14[14];
    u8 state;
    u8 pad_23;
    s32 motion_24;
    u8 pad_28[4];
    s32 motion_2c;
    s32 rate_x;
    s32 rate_z;
    u8 pad_38[33];
    u8 flags;
} SceneRecord;

typedef struct Position3 {
    s32 x;
    s32 y;
    s32 z;
} Position3;

/* The active subject's handle sits 500 bytes into the shared table. */
typedef struct ActiveSubjectSlot {
    u8 pad[500];
    void *handle;
} ActiveSubjectSlot;

extern u8 Data_0200c194[];
extern u8 Data_0200c1dc[];
extern u8 Data_0200c1f4[];
extern u8 Data_0200a1b9[];
extern u8 Data_0200be4e[];
extern u8 Data_0200c5aa[];
extern u16 Data_0200c790;
extern u16 Data_0200c764;
extern u16 Data_0200c79c;
extern u8 *Data_0200c7a0;
extern u16 Data_0200c7f8;
extern u16 Data_0200c76c;
extern u32 Data_0200c770;
extern u8 Value_0000008f;
extern u8 Value_00000090;
extern u8 Value_00002076;
extern u8 Value_00002078;
extern u8 Value_0000207a;
extern s16 Data_0200c6a6;
extern u8 Data_0200abed[];
extern u16 Data_0200c7f4;
extern u16 Data_0200c780;
extern u16 Data_0200c758;
extern u16 Data_0200c774;
extern u16 Data_0200c78c;
extern u16 Data_0200c760;
extern u16 Data_0200c800;
extern u16 Data_0200c7a4;
extern u16 Data_0200c7bc;
extern u16 Data_0200c750;
extern u8 *Data_03001f3c;
extern u8 Value_0000207c;
extern u8 Data_0200c420[];
extern u8 Data_0200bf14[];
extern u8 Data_0200b1c1[];
extern volatile u32 Data_03001ae8;
extern u8 Data_0200c008[];
extern u8 Data_00000000[];
extern u8 Data_02002090[];
extern u8 Data_0200cbfc[];
extern u8 Data_0200cc28[];
extern u8 Data_0200cca4[];
extern u8 Data_0200bfd0[];
extern u8 Data_0200bfe4[];
extern u32 Data_0200c154[];

void Func_020061d0(u8 *, s32);
void Func_02003c7e();
void Func_02003c90();
void Func_02003c9a();
void Func_02003cc0();
void Func_02003cd2();
void Func_02003cf0();
void Func_02003d02();
void Func_02003d14();
void Func_02003daa();
void Func_02003dda();
void Func_02003e0a();
void Func_02003e58();
void Func_02003eb0();
void Func_02003ed2();
void Func_02003ede();
void Func_02003ee0();
void Func_02003f12();
void Func_02003f2a();
s32 Func_02003f3c();
s32 Func_02003f96();
void Func_02003f9e();
void Func_02003fc6();
void Func_02004006();
void Func_02004020();
void Func_02004034();
void Func_02003246();
void Func_020044f4();
void Func_02004618();
void Func_0200462c();
void Func_0200463c();
void Func_02004642();
void Func_02004692();
void Func_020046a0();
void Func_020046ae();
void Func_020046cc();
void Func_020046d4();
void Func_020046d4_a();
void Func_020046de();
void Func_020046e0();
void Func_020046e6();
void Func_02004774();
void Func_02004a76();
s32 Func_02004a7e();
void Func_02004aac();
void Func_02004aba();
void Func_02004ac8();
void Func_02004ad6();
void Func_02004ae4();
s32 Func_02004b28();
s32 Func_02004b28_a();
void Func_02004b3a();
void Func_02004b48();
void Func_02004b54();
void Func_02004b62();
void Func_02004b70();
void Func_02004bbc();
void Func_02004bbe();
void Func_02004bd2();
void Func_02004bd4();
void Func_02004be0();
void Func_02004be6();
void Func_02004be6_a();
void Func_02004be8();
void Func_02004bee();
void Func_02004bf2();
void Func_02004bf6();
void Func_02004bfe();
void Func_02004c08();
void Func_02004c18();
void Func_02004c20();
void Func_02004c28();
void Func_02004c32();
void Func_02004c34();
void Func_02004c3a();
void Func_02004c42();
void Func_02004c46();
void Func_02004c4c();
void Func_02004c50();
void Func_02004c52();
void Func_02004c5c();
void Func_02004c64();
void Func_02004c66();
s32 Func_02004c68();
void Func_02004c6e();
void Func_02004c78();
void Func_02004cae();
void Func_02004cc8();
void Func_02004cca();
void Func_02004cf2();
void Func_02004d04();
void Func_02004d18();
void Func_02004d1e();
void Func_02004d24();
void Func_02004d2a();
void Func_02004d2e();
void Func_02004d32();
void Func_02004d48();
void Func_02004d5a();
void Func_02004d5a_a();
void Func_02004d5c();
void Func_02004d60();
void Func_02004d7a();
void Func_02004d7a_a();
void Func_02004d82();
void Func_02004d86();
void Func_02004d8a();
void Func_02004d90();
void Func_02004d92();
void Func_02004d94();
void Func_02004da2();
void Func_02004da4();
void Func_02004dac();
void Func_02004dae();
void Func_02004db0();
void Func_02004db0_a();
s32 Func_02004db2();
void Func_02004dbc();
void Func_02004dbe();
void Func_02004dc2();
void Func_02004dc6();
void Func_02004ddc();
void Func_02004dde();
s32 Func_02004dec();
void Func_02004dee();
void Func_02004dee_a();
void Func_02004df6();
void Func_02004df8();
void Func_02004e02();
void Func_02004e04();
s32 Func_02004e0c();
void Func_02004e0e();
void Func_02004e0e_a();
void Func_02004e12();
void Func_02004e1e();
void Func_02004e1e_a();
void Func_02004e28();
s32 Func_02004e2c();
void Func_02004e2e();
void Func_02004e2e_a();
void Func_02004e3e();
void Func_02004e4e();
void Func_02004e5e();
void Func_02004e6e();
void Func_02004e7e();
void Func_02004e82();
void Func_02004e98();
void Func_02004ea2();
void Func_02004ea8();
void Func_02004eac();
void Func_02004eb2();
void Func_02004f26();
void Func_02001748();
void Func_0200179a();
void Func_020017e2();
void Func_02003152();
s32 Func_02003368();
void Func_020034d2();
s32 Func_0200354c();
s32 Func_02003f6c();
void Func_0200418e();
void Func_0200529c();
void Func_020052e0();
void Func_02005322();
void Func_02005340();
void Func_02005340_a();
void Func_0200536c();
void Func_0200537c();
void Func_0200537e();
void Func_02005390();
void Func_02005390_a();
void Func_020053aa();
s32 Func_020053b6();
s32 Func_020053b6_a();
void Func_020053dc();
void Func_020053ee();
void Func_020053f4();
void Func_02005406();
void Func_02005412();
Obj *Func_02005708(s32);
void Func_020056f6(void);
Obj *Func_02005716(s32);
void Func_020057f0(s32);
void Func_02005808(s32, s32);
void Func_02005780();
void Func_02005780_a();
void Func_020058a4(void);
void Func_020058b0(void);
void Func_02005770(s32, s32);
void Func_020058a2(s32);
void Func_02005782(s32);
void Func_0200343a(s32);
void Func_020058de(void);
void Func_020058f2(void);
void Func_020057e4(void);
typedef void(*Task02000134)(void);
void Func_02003c5a(Task02000134);
s32 Func_02003c78(s32, s32);
void Func_02003e56(s32, s32, s32, s32, s32, s32);
void Func_02003ec4(s32);
void Func_02004498(s32);
s32 Func_0200565e(s32);
void Func_02005a4a(s32, s32);
void Func_02005b82(s32);
void Func_02005b9a(s32, s32);
s16 Func_02006816(void);
void Func_02005902(void);
void Func_0200686c(u8 *, s32);
void Func_020068c8(u8 *, s32);
void Func_02006906(u8 *);
void Func_02006960(s16);
void Func_02006ab8(s32);
void Func_02003c86(s32);
void Func_02003c9a_a(s32);
void Func_02005b3c(void);
void Func_02005994(s32, s32);
void Func_02005aca(s32);
void Func_02005ae2(s32, s32);
s32 Func_02005a04(s32);
s32 Func_02005a14(s32);
s32 Func_020059f6(s32);
void Func_02005a44(s32);
void Func_02005b1a(s32);
s32 Func_02005b2a(s32, s32);
s32 Func_02005aaa(s32, s32);
void Func_02003db6(s32, s32, s32, s32, s32, s32);
Obj *Func_02003e7c(s32);
s32 Func_02003db8(s32, s32, s32);
void Func_02003df8(s32, s32, s32, s32, s32, s32);
Obj *Func_02003ebe(s32);
void Func_02003e84(s32, s32);
void Func_02003e24(s32, s32, s32, s32, s32, s32);
void Func_02003b34(void);
void Func_020003e8(void);
s32 Func_020043b8(void);
void Func_02004708(void);
void Func_02000b9e(void);
void Func_0200b3a0(void);
s32 Func_020073ee(void);
void Func_020073be(s32, s32);
s32 Func_020074bc(s32);
void Func_02007392(s32, s32);
Obj *Func_02004016(s32);
Obj *Func_0200401e(s32);
void Func_0200400c(void);
void Func_02003f7e(Obj *, s32, s32, s32);
void Func_02003f56(Obj *, s32);
void Func_02003faa(Obj *, s32, s32, s32);
void Func_02003f8e(Obj *, s32);
void Func_02003f98(Obj *, s32);
void Func_020041ee(s32);
void Func_02003fdc(Obj *);
void Func_02003fac(Obj *, s32);
void Func_02004204(s32);
void Func_020040b8(void);
Obj *Func_020040f4(s32);
void Func_02004054(s32, s32, s32, s32, s32, s32);
void Func_020007a2(s32, s32, s32);
Obj *Func_02004124(s32);
void Func_02004078(s32, s32, s32, s32, s32, s32);
Obj *Func_02004158(s32);
Obj *Func_0200417e(s32);
void Func_02004100(s32, s32, s32, s32, s32, s32);
void Func_0200084e(s32, s32, s32);
Obj *Func_020041d0(s32);
void Func_02004124_a(s32, s32, s32, s32, s32, s32);
Obj *Func_020046cc_a(void);
s32 Func_02005508(s32);
s32 Func_02005512(s32);
void Func_020055b8(s32, s32, s32);
s32 Func_02005530(s32);
s32 Func_0200553a(s32);
void Func_020055dc(s32, s32, s32);
s32 Func_02005554(s32);
s32 Func_0200555e(s32);
void Func_02005600(s32, s32, s32);
Obj *Func_02006d24(s16);
void Func_02006c56(Obj *, s32, s32, s32);
void Func_02006c3e(Obj *, s32);
void Func_02006e8c(s32);
void Func_02001b18();
void Func_0200325e();
s32 Func_02003474();
void Func_02003680();
void Func_020036fa();
void Func_02004080();
void Func_0200432e();
void Func_0200447a();
void Func_02004546();
s32 Func_02004606();
s32 Func_02004610();
void Func_0200461c();
void Func_020053a8();
void Func_020053fc();
void Func_0200544e();
void Func_0200546a();
void Func_02005474();
void Func_02005488();
void Func_02005494();
void Func_0200bd40();
void Func_0200bd70();
void Func_0200bd78();
void Func_0200bc98();
void Func_020054c0();
void Func_020054c2();
void Func_020054ca();
void Func_020054d2();
void Func_020054de();
void Func_020054f6();
void Func_020054fa();
void Func_020054fe();
void Func_0200552a();
void Func_02005550();
void Func_02005566();
void Func_02005578();
void Func_0200557c();
void Func_02005594();
void Func_0200559c();
void Func_020055b2();
void Func_020055b4();
PartyInteractionRecord *Func_0200593a(void);
void Func_0200592e(void);
s32 Func_0200590a(void);
void Func_020059cc(s32 cue);
s32 Func_020059ec(s32 actorId, s32 mode);
void Func_0200597e(s32 actorId, s32 horizontalRate, s32 verticalRate);
void Func_0200598c(s32 actorId, s32 horizontalRate, s32 verticalRate);
void Func_020059b0(s32 actorId, s32 x, s32 y);
void Func_0200596e(s32 step);
void Func_020059c8(s32 actorId, s32 x, s32 y);
void Func_020059d4(s32 actorId, s32 x, s32 y);
void Func_02005aa8(void);
void Func_02005ab4(void);
void Func_02005a82(s32 step);
void Func_02005a32(s32 cue);
void Func_02005a4a_a(s32 actorId, s32 mode);
void Func_020059b6(void);
void Func_0200599a(s32, s32);
void Func_020059a4(s32, s32);
void Func_020059ae(s32, s32);
void Func_020059b8(s32, s32);
void Func_020059c2(s32, s32);
void Func_020059cc_a(s32, s32);
void Func_020059fc();
s32 Func_02005b24();
s32 Func_02005b38();
s32 Func_02005b4e();
void Func_02005bb0();
void Func_02005bc4();
void Func_02005bc4_a();
void Func_02005bee();
void Func_02005c00();
void Func_02005c2c();
s32 Func_02005c3c();
void Func_02005c88();
void Func_02005c96();
s32 Func_02005c9a();
void Func_02005ca4();
void Func_02005cc2();
void Func_02005cda();
void Func_02005cda_a();
void Func_02005ce8();
void Func_02005cea();
void Func_02005cf2();
void Func_02005d10();
void Func_02005d1a();
s32 Func_02005d1c();
void Func_02005d1e();
void Func_02005d1e_a();
void Func_02005d2c();
void Func_02005d30();
void Func_02005d36();
s32 Func_02005d52();
void Func_02005d56();
void Func_02005d56_a();
void Func_02005d60();
void Func_02005d6a();
void Func_02005db2();
void Func_02005e2c();
s32 Func_02005bbc();
s32 Func_02005cf4();
s32 Func_02005cf4_a();
s32 Func_02005d3e();
s32 Func_02005d3e_a();
Rec *Func_02005d34(s32);
void Func_02005d46(s32, s32);
void Func_02005d62(s32, s32);
void Func_020063e2(void);
void Func_020064f6(void);
void Func_0200650a(void);
void Func_020063e8(s32);
void Func_0200654e(s32);
void Func_02004880(s32);
void Func_02004e0c_a(s32, s32);
void Func_02006402(s32);
void Func_02006416(void);
void Func_0200656e(s32);
void Func_0200641a(void);
void Func_0200652e(void);
void Func_02006542(void);
void Func_0200642a(s32);
void Func_02006592(s32);
void Func_020048c4(s32);
void Func_02004e50(s32, s32);
void Func_02006446(s32);
s32 Func_020065ba(void);
void Func_020062be(s32);
void Func_020065bc(s32);
void Func_020048ee(s32);
void Func_02004e7a(s32, s32);
void Func_020065d0(s32);
void Func_02006476(s32);
void Func_02004e8e(s32, s32);
void Func_020065e4(s32);
void Func_0200648a(s32);
void Func_0200491c(s32);
void Func_02004ea8_a(s32, s32);
void Func_020065fe(s32);
void Func_020064a4(s32);
void Func_02004936(s32);
void Func_02004ec2(s32, s32);
void Func_02006618(s32);
void Func_020065fc(void);
void Func_020064d0(void);
void Func_0200648e(s32);
void Func_020049aa();
void Func_020049d6();
s32 Func_02004f36();
s32 Func_02004f62();
void Func_020063dc();
void Func_02006512();
void Func_02006a56();
void Func_02006a5e();
void Func_02006532();
void Func_0200653c();
void Func_02006a76();
void Func_0200655a();
void Func_02006aa6();
void Func_02006580();
u8 *Func_02006b14();
void Func_02006600();
void Func_02006614();
void Func_0200662c();
void Func_02006644();
void Func_02006bba();
void Func_02006678();
void Func_02006be4();
void Func_02006bec();
void Func_020066aa();
void Func_020066a4();
void Func_020066b6();
s32 Func_020066d8();
void Func_020066da();
void Func_020066e6();
void Func_02006c5c();
void Func_02006c62();
void Func_02006c6a();
void Func_02006c80();
void Func_02006cb2();
u8 *Func_02006cde();
void Func_02006cea();
void Func_02006d00();
void Func_02006d36();
void Func_02006d42();
void Func_02006db2();
Obj *Func_02006868(void);
void Func_0200677e(Obj *);
Obj *Func_02006c20(s32);
void Func_02006a3c(Obj *);
void Func_02006a24(Obj *, s32);
void Func_02006a60(Obj *, s32, s32, s32);
void Func_02006a6e(Obj *);
void Func_02006a3e(Obj *, s32);
Obj *Func_02006be0(s32);
void Func_020069fc(Obj *);
void Func_020069e4(Obj *, s32);
void Func_02006a20(Obj *, s32, s32, s32);
u32 Func_02006b00(void);
s32 Func_02006b1c(void);
s32 Func_02006b22(void);
void Func_02006b3e(s32, s32, s32 *);
Obj *Func_02006bd2(s32, s32, s32, s32);
void Func_02006c3e_a(Obj *, s32);
void Func_02006be6(Obj *, s32);
void Func_02006be6_a(Obj *, s32);
void Func_02006bee(Obj *, s32);
void Func_02006da4_a();
SceneRecord *Func_020071f6(Position3 *, SceneRecord *);
SceneRecord *Func_02007220(Position3 *, SceneRecord *);
SceneRecord *Func_0200724c(Position3 *, SceneRecord *);
u8 *Func_0200772c();
void Func_020075de();
s32 *Func_02007366();
void Func_02007610();
s32 *Func_02007398();

/* Contiguous unnamed leaf-owner run for resource_3ba. */

/*
 * Scene setup for resource_3ba: allocates a scene descriptor, stamps its
 * parameter block, uploads image and palette, and installs the per-frame task.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */

/* In-image data at file offset 0x3f14 (0x0200bf14 - 0x8000). */

/* The per-frame task this owner installs: in-image code, published below as
 * its entry address plus the Thumb bit. */

/* AUDITED GENERATED CALL SCRIPT for Scene_RunSceneFourCoordinator:
 * A phase-two fast path, full and revisit branches, and all 42 calls across
 * the complete scene-four coordinator. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* This overlay's own occupancy lookup for a cell. The record pointer the call
 * sites also load is spelled here, although the lookup itself uses only the
 * position. */

/* In-image direction table: sixteen packed steps, high half x, low half z. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* A countdown word this overlay owns at Data_0200c41c: each call decrements
 * it by one, and specific values select which sub-sequence runs this call.
 * Reaching 0 restarts the countdown at 120 after running its own branch. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern s16 Data_02000240_t[][1];
    extern u8 Data_0200c41c[];

    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern s16 Data_02000240_t[][1];
    extern u8 Data_0200c41c[];

    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern s16 Data_02000240_t[][1];
    extern u8 Data_0200c41c[];

    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern s16 Data_02000240_t[][1];
    extern u8 Data_0200c41c[];

    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern s16 Data_02000240_t[][1];
    extern u8 Data_0200c41c[];

    extern u8 Data_03001ebc[];

    f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern s16 Data_02000240_t[][1];
    extern u8 Data_0200c41c[];

    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3);
}

static __inline__ void Call6_02000270(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern s16 Data_02000240_t[][1];
    extern u8 Data_0200c41c[];

    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern s16 Data_02000240_t[][1];
    extern u8 Data_0200c41c[];

    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern s16 Data_02000240_t[][1];
    extern u8 Data_0200c41c[];

    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

static __inline__ void Call3_02000db8(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern s16 Data_02000240_t[][1];
    extern u8 Data_0200c41c[];

    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ void Call3_020015e0(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern s16 Data_02000240_t[][1];
    extern u8 Data_0200c41c[];

    extern u8 Data_02000240[];
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern s16 Data_02000240_t[][1];
    extern u8 Data_0200c41c[];

    extern u8 Data_02000240[];
    extern u8 Data_03001ebc[];

    return f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_020016ec(void (*f)(), s32 a0)
{
    extern s16 Data_02000240[];

    f(a0);
}

static __inline__ void Call2_020016ec(void (*f)(), s32 a0, s32 a1)
{
    extern s16 Data_02000240[];

    f(a0, a1);
}

static __inline__ void Call3_020016ec(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern s16 Data_02000240[];

    f(a0, a1, a2);
}

static __inline__ s32 Value3_020016ec(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern s16 Data_02000240[];

    return f(a0, a1, a2);
}

static __inline__ void Call4_020016ec(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern s16 Data_02000240[];

    f(a0, a1, a2, a3);
}

static inline void InitializeActorZero(void)
{
    Func_0200597e(0, 0x10000, 0x8000);
}

static inline void InitializeSelectedActor(s32 actorId)
{
    Func_0200598c(actorId, 0x10000, 0x8000);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value0(s32 (*f)())
{
    extern u8 Data_02000240[];

    return f();
}

static __inline__ void Call1_02001e7c(void (*f)(), s32 a0)
{
    extern u8 Data_02000240[];

    f(a0);
}

static __inline__ s32 Value1_02001e7c(s32 (*f)(), s32 a0)
{
    extern u8 Data_02000240[];

    return f(a0);
}

static __inline__ s32 Value2_02001e7c(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_02000240[];

    return f(a0, a1);
}

static __inline__ void Call3_02001e7c(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_02000240[];

    f(a0, a1, a2);
}

static __inline__ s32 Value3_02001e7c(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_02000240[];

    return f(a0, a1, a2);
}

static __inline__ void Call1_02002844(void (*f)(), s32 a0)
{
    extern u8 Data_02000240[];
    extern u8 Data_02000240_t[][2];
    extern struct ModeRecord Data_0200be76;
    extern struct ModeRecord Data_0200c628;
    extern u8 Data_03001ebc[];
    void Func_02002ba8();
    void Func_02002d84();
    void Func_02006c4e();
    void Func_02006d62();

    f(a0);
}

static __inline__ s32 Value2_02002844(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_02000240[];
    extern u8 Data_02000240_t[][2];
    extern struct ModeRecord Data_0200be76;
    extern struct ModeRecord Data_0200c628;
    extern u8 Data_03001ebc[];
    void Func_02002ba8();
    void Func_02002d84();
    void Func_02006c4e();
    void Func_02006d62();

    return f(a0, a1);
}

static __inline__ void Call3_02002844(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_02000240[];
    extern u8 Data_02000240_t[][2];
    extern struct ModeRecord Data_0200be76;
    extern struct ModeRecord Data_0200c628;
    extern u8 Data_03001ebc[];
    void Func_02002ba8();
    void Func_02002d84();
    void Func_02006c4e();
    void Func_02006d62();

    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_02002844(s32 amount)
{
    extern u8 Data_02000240[];
    extern u8 Data_02000240_t[][2];
    extern struct ModeRecord Data_0200be76;
    extern struct ModeRecord Data_0200c628;
    extern u8 Data_03001ebc[];
    void Func_02002ba8();
    void Func_02002d84();
    void Func_02006c4e();
    void Func_02006d62();

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

u8 *Func_020072c6();            /* allocate a record by (id, size) */

s32 Func_020072e0();            /* reserve a graphics handle */

u8 *Func_02007478();            /* scene record for an actor selector */

u8 *Func_02007480();            /* scene record for an actor selector */

s32 Func_02007428();            /* test a story flag */

void Func_02007356();           /* upload image data to a handle */

s32 Func_0200737a();            /* next palette slot index */

s32 Func_02007386();            /* upload a palette ramp */

void Func_02007326();           /* install a per-frame task (callback, rate) */

void Func_02007374();           /* release a graphics handle */

s32 Func_02006d18();           /* veneer to GameFlag_IsSet */

u8 *Func_02006d88();           /* veneer to Scene_GetRecord */

void Func_02006d02();          /* veneer to Object_SetPosition */

void Func_02006d90();          /* veneer to GameFlag_Set */

void Func_02006ce8();          /* veneer to Object_SetCallback */

void Func_02006c4e();          /* veneer to Func_080000c0 */

s32 Func_02005242();           /* local thunk to Func_020020e8, site A */

s32 Func_02005254();           /* local thunk to Func_020020e8, site B */

void Func_02006d82();          /* veneer to UiText_DrawQuantity, site A */

void Func_02006d94();          /* veneer to UiText_DrawQuantity, site B */

void Func_02006da4();          /* shared veneer, selector refresh + 0x96a */

void Func_02006d62();          /* veneer to Func_08009148 */

SceneRecord *Func_020075cc();   /* scene record for a subject handle */

s32 Func_020075e6(SceneRecord *, Position3 *);  /* terrain probe */

void Func_02007594(SceneRecord *, s32);         /* select presentation mode */

void Func_020074fa(s32);                        /* wait n frames */

void Func_020075da(SceneRecord *, s32, s32, s32);   /* place at (x, y, z) */

void Func_020075ea(SceneRecord *, s32, s32, s32);   /* place at (x, y, z) */

void Func_02007810(s32);                        /* play a cue */

void Func_020075fe(SceneRecord *);              /* re-attach the camera */

void Func_0200781e(s32);                        /* play a cue */

void Func_020075e4(SceneRecord *, s32);         /* select presentation mode */

/* Contiguous unnamed leaf-owner run for resource_3ba. */
u8 *SceneData_GetTableC194(void)
{
    extern u8 Data_0200be76[];
    extern u8 Data_0200c57e[];
    extern u8 Data_0200c628[];

    return Data_0200c194;
}

s32 Func_02000038(void)
{
    return 0;
}

u8 *SceneData_GetTablec1dc(void)
{
    extern u8 Data_0200be76[];
    extern u8 Data_0200c57e[];
    extern u8 Data_0200c628[];

    return Data_0200c1dc;
}

u8 *SceneData_GetTablec1f4(void)
{
    extern u8 Data_0200be76[];
    extern u8 Data_0200c57e[];
    extern u8 Data_0200c628[];

    return Data_0200c1f4;
}

/* Runs one branch of a scripted auxiliary sequence selected by the current
 * countdown value, then advances (or, from 0, restarts) the countdown. */
void Scene_RunOpeningAuxiliarySequence(void)
{
    extern s16 Data_02000240_t[][1];
    extern u8 Data_0200c41c[];

    extern u8 Data_03001ebc[];

    switch ((u32)AUX_COUNTDOWN) {
    case 66:
        Call6(Func_02003cc0, 92, 31, 2, 2, 50, 38); /* main:080091c8 */
        Call6(Func_02003cd2, 92, 31, 2, 2, 54, 38); /* main:080091c8 */
        Object_SetModeById_1(16, 10); /* object 16, action 10 */
        break;
    case 60:
        Call6(Func_02003c7e, 92, 33, 2, 2, 50, 38); /* main:080091c8 */
        Call6(Func_02003c90, 92, 33, 2, 2, 54, 38); /* main:080091c8 */
        Call6(Func_02003c9a, 50, 25, 6, 1, 50, 12); /* main:080091c0 */
        Object_SetModeById_2(16, 11); /* object 16, action 11 */
        break;
    case 6:
        Call6(Func_02003cc0, 92, 31, 2, 2, 50, 38); /* main:080091c8 */
        Call6(Func_02003cd2, 92, 31, 2, 2, 54, 38); /* main:080091c8 */
        Object_SetModeById_3(16, 10); /* object 16, action 10 */
        break;
    case 0:
        Call6(Func_02003cf0, 92, 29, 2, 2, 50, 38); /* main:080091c8 */
        Call6(Func_02003d02, 92, 29, 2, 2, 54, 38); /* main:080091c8 */
        Object_SetModeById_4(16, 12); /* object 16, action 12 */
        Call6(Func_02003d14, 50, 24, 6, 1, 50, 12); /* main:080091c0 */
        AUX_COUNTDOWN = 120;
        break;
    }
    AUX_COUNTDOWN = AUX_COUNTDOWN - 1;
}

void State_ResetCounterAndStartTask(void)
{
    extern s16 Data_02001000;
    extern s32 Data_0200c41c;
    extern u8 *Data_03001ebc;

    Task02000134 task;

    Data_0200c41c = 0;
    task = (Task02000134) 0x0200804D;
    Func_02003c5a(task);
    task();
}

void State_SetMode66AndPassOpeningSequence(void)
{
    extern s16 Data_02001000;
    extern s32 Data_0200c41c;
    extern u8 *Data_03001ebc;

    s32 value = 66;
    s32 *mode = (s32 *)0x0200C41C;

    *mode = value;
    Func_02003c78(0x0200804D, 0xC80);
}

void State_WaitUntilWordC41cIs22(void)
{
    extern s16 Data_02000240[];
    extern u16 Data_02001000;
    extern u32 Data_0200c41c;

    s32 i;

    Func_02003c86(10);
    i = 0;
    if (Data_0200c41c != 22) {
        do {
            Func_02003c9a_a(1);
            i++;
            if (i > 119) {
                break;
            }
        } while (Data_0200c41c != 22);
    }
}

void State_ApplyRectsForActorsNineAndTen(void)
{
    extern Ctl Data_02001000;

    Obj_020001a8 *o;
    s32 r;

    {
        s32 x = 23;
        s32 y = 12;

        Func_02003db6(27, 13, 3, 1, x, y);
    }
    o = Func_02003e7c(9);
    r = Func_02003db8(0, o->f08, o->f10);
    if (o->f0c == 0 && r == 0) {
        o->f23 = 2;
        o->f55 = 0;
        {
            s32 x = o->f08 >> 20;
            s32 y = o->f10 >> 20;

            Func_02003df8(14, 13, 1, 1, x, y);
        }
    }
    o = Func_02003ebe(10);
    {
        s32 x = o->f08 >> 20;

        Func_02003e84(784, x);
    }
    {
            s32 x = o->f08 >> 20;
            s32 y = o->f10 >> 20;

        Func_02003e24(14, 13, 1, 1, x, y);
    }
}

void Scene_RunTwoCallSequence(void)
{
    extern Ctl Data_02001000;

    Func_02003b34();
    Func_020003e8();
}

void State_ApplyRectAndSend303(void)
{
    extern s16 Data_02001000;
    extern s32 Data_0200c41c;
    extern u8 *Data_03001ebc;

    {
        s32 x = 47;
        s32 y = 12;

        Func_02003e56(47, 24, 1, 1, x, y);
    }
    Func_02003ec4(0x303);
}

void FieldScene_RunScene3ba_02000270(void)
{
    extern s16 Data_02000240_t[][1];
    extern u8 Data_0200c41c[];

    extern u8 Data_03001ebc[];

    u32 i;
    u8 *rec7;
    s32 record;
    s32 none;

    Call1(Func_02003ede, 0x301);
    rec7 = Value1(Func_02003f3c, 13);
    Func_02003f2a();
    Call2(Func_02004006, 0x20000, 0x4000);
    Call4(Func_02004020, 0x2580000, -1, 0xc80000, 1);
    Func_02003e58((s32)rec7, 3);
    Func_02004034();
    none = 0;
    rec7[85] = none;
    *(s32 *)((s32)rec7 + 52) = 0x6666;
    *(s32 *)((s32)rec7 + 48) = 0xcccc;
    Call4(Func_02003eb0, (s32)rec7, *(s32 *)((s32)rec7 + 8), 0x80000, *(s32 *)((s32)rec7 + 16));
    rec7 = Value1(Func_02003f96, 14);
    rec7[85] = none;
    *(s32 *)((s32)rec7 + 52) = 0x6666;
    *(s32 *)((s32)rec7 + 48) = 0xcccc;
    Call4(Func_02003ed2, (s32)rec7, *(s32 *)((s32)rec7 + 8), 0x200000, *(s32 *)((s32)rec7 + 16));
    Func_02003ee0((s32)rec7);
    Func_02003f9e(45);
    Call6_02000270(Func_02003f12, 43, 12, 1, 1, 41, 12);
    Func_02003fc6();
}

void StagedActor_PlacePairAtOffsetAndRun(s32 a0, s32 a1, s32 a2)
{
    extern s32 Data_02000240[];

    Obj_0200033c *p;
    Obj_0200033c *q;
    s32 x;
    s32 y;

    p = Func_02004016(Data_02000240[125]);
    q = Func_0200401e(a0);
    Func_0200400c();
    {
        x = ((p->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Func_02003f7e(p, x, p->f0c, y);
    }
    Func_02003f56(p, 27);
    {
        x = ((q->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        Func_02003faa(q, x, q->f0c, y);
    }
    if (a1 < 0 || a2 < 0) {
        Func_02003f8e(q, 4);
    } else {
        Func_02003f98(q, 3);
    }
    Func_020041ee(226);
    Func_02003fdc(p);
    Func_02003fac(q, 2);
    Func_02004204(288);
    Func_020040b8();
}

void Actor_ShiftActorSeventeenByLeaderRow(void)
{
    extern s32 Data_02000240[];

    Obj_0200042c *o;
    s32 v;
    s32 t;

    o = Func_020040f4(Data_02000240[125]);
    v = o->f10 >> 20;
    t = -48;
    if (v <= 8) {
        t = 48;
    }
    Func_02004054(67, 8, 3, 1, 64, v);
    Func_020007a2(17, 0, t);
    o = Func_02004124(17);
    v = o->f10 >> 20;
    Func_02004078(64, 24, 3, 1, 64, v);
}

void Actor_ShiftActorEighteenByInputAndLeaderColumn(void)
{
    extern s32 Data_02000240[];

    Obj_0200042c *o;
    s32 a;
    s32 b;
    s32 s;

    o = Func_02004158(Data_02000240[125]);
    a = o->f08 >> 20;
    if ((Data_03001ae8 & 32) != 0) {
        s = -1;
    }
    if ((Data_03001ae8 & 16) != 0) {
        s = 1;
    }
    o = Func_0200417e(17);
    b = o->f10 >> 20;
    if (a == 63) {
        if (b == 11) {
            return;
        }
        b = 160;
    } else if (a == 67) {
        if (b == 11 && s == -1) {
            return;
        }
        b = 96;
    } else {
        if (b == 11) {
            b = 96;
        } else {
            b = 160;
        }
        b = -b;
    }
    Func_02004100(72, 9, 1, 3, a, 9);
    Func_0200084e(18, b, 0);
    o = Func_020041d0(18);
    a = o->f08 >> 20;
    Func_02004124_a(63, 25, 1, 3, a, 9);
}

void State_ApplyValue768(void)
{
    extern s16 Data_02001000;
    extern s32 Data_0200c41c;
    extern u8 *Data_03001ebc;

    Func_02004498(768);
}

void Scene_RunBranchedStep(void)
{
    extern Ctl Data_02001000;

    if (Func_020043b8() == 0) {
        Func_02004708();
    } else {
        Func_02000b9e();
    }
}

u8 *SceneData_GetTablec420(void)
{
    extern Ctl Data_02001000;

    return Data_0200c420;
}

void FieldScene_RunScene3ba_02000974(s32 a0)
{
    extern s16 Data_02000240_t[][1];
    extern u8 Data_0200c41c[];

    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_0200463c(24);
    Func_02004642(25);
    Func_02004618(1);
    Func_0200462c();
    Call3(Func_02004692, 8, 0x5280000, 0xc00000);
    Call3(Func_020046a0, 0, 0x5080000, 0xc00000);
    Call3(Func_020046d4, 8, 0x4000, 0);
    Call3(Func_020046e0, 0, 0x4000, 0);
    if (a0 < 0) {
        Func_020046cc(8, 10);
        Func_020046d4_a(0, 35);
    } else {
        Func_020046de(8, 8);
        Func_020046e6(0, 28);
    }
    Func_020044f4(1);
    Call4(Func_02004774, 0x5180000, 0, 0x800000, 0);
    Func_02003246(a0);
    Func_020046ae();
}

void Actor_MarkObjectAtTiles94To95(void)
{
    extern s32 Data_02000240[];

    Obj_02000a10 *o;
    s32 x;
    s32 y;

    o = Func_020046cc_a();
    if (o != 0) {
        x = o->f08 >> 19;
        y = o->f10 >> 19;
        if (x >= 94 && x <= 95 && y > 23 && y <= 26) {
            o->f22 = 1;
        }
    }
}

void Scene_RunCommandSequence(s32 a0)
{
    extern s16 Data_02000240_t[][1];
    extern u8 Data_0200c41c[];

    extern u8 Data_03001ebc[];

    u32 i;
    s32 p10;
    s32 p10b;
    s32 p9;
    s32 p9b;
    s32 record;

    record = Func_02004a7e();
    p9 = *(s16 *)(record + 10);
    p10 = *(s16 *)(record + 18);
    Func_02004a76();
    Call3_02000db8(Func_02004aac, a0, 0x10000, 0x8000);
    Call3_02000db8(Func_02004aba, 0, 0x10000, 0x8000);
    Call3_02000db8(Func_02004ac8, 1, 0x10000, 0x8000);
    Call3_02000db8(Func_02004ad6, 2, 0x10000, 0x8000);
    Call3_02000db8(Func_02004ae4, 3, 0x10000, 0x8000);
    Call3_02000db8((void (*)())Func_02004b28, 0, ((s32)p9 << 16), ((s32)((s32)p10 << 16) + -0x300000));
    Func_02004b3a(1, ((s32)((s32)p9 << 16) + -0x100000), (-0x280000 + (s32)((s32)p10 << 16)));
    Func_02004b48(2, ((s32)((s32)p9 << 16) + 0x100000), (-0x280000 + (s32)((s32)p10 << 16)));
    Func_02004b54(3, ((s32)p9 << 16), ((s32)((s32)p10 << 16) + -0x200000));
    Func_02004b62(a0, ((s32)p9 << 16), ((s32)((s32)p10 << 16) + -0x500000));
    record = Func_02004b28_a(0);
    {
        s32 shown = 0xc000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Func_02004be6(0, 0);
    Func_02004c32();
    Func_02004c46();
    Call1(Func_02004bbc, 0x20cb);
    Func_02004bd4(a0, 0);
    Call3_02000db8(Func_02004bfe, 3, 0x101, 60);
    Func_02004be6_a(3, 0);
    Func_02004bbe(a0, 3);
    Func_02004bf6(a0, 0);
    Call3_02000db8(Func_02004c20, 2, 0x101, 60);
    Func_02004c08(2, 0);
    Func_02004bf2(a0, 2, 0);
    Func_02004b70(20);
    Func_02004be8(a0, 3);
    Func_02004c28(a0, 0);
    Call3_02000db8(Func_02004c52, 1, 0x101, 60);
    Func_02004c3a(1, 0);
    Call3_02000db8(Func_02004c64, 3, 0x101, 60);
    Func_02004c4c(3, 0);
    Call3_02000db8(Func_02004c78, a0, 0x102, 60);
    if (Value2(Func_02004c68, a0, 0) != 0) {
    } else {
        L_02000f28:;
        Call1(Func_02004c5c, 0x20d5);
        Func_02004c34(2, 3);
        Func_02004bd2(2);
        Func_02004c42(1, 3);
        Func_02004be0(2);
        Func_02004c50(3, 3);
        Func_02004bee(1);
        Func_02004c66(0, 3);
        Func_02004c6e(a0, 3);
        Func_02004cae(a0, 0);
        Call3_02000db8(Func_02004cca, a0, 0xa000, 0);
        Func_02004c18(20);
        Func_02004cc8(a0, 0);
        Call2(Func_02004d04, 0x30000, 0x6000);
        Call4(Func_02004d1e, 0x1380000, -1, 0x680000, 1);
        Func_02004d2a();
        Func_02004cf2(a0, 0);
        Call2(Func_02004d2e, 0x18000, 0x3000);
        Call4(Func_02004d48, 0x3080000, -1, 0x680000, 1);
        Func_02004d18(a0, 0);
        Func_02004d5c();
        Func_02004d24(a0, 0);
        Call2(Func_02004d60, 0x30000, 0x6000);
        Call4(Func_02004d7a, 0x4d80000, -1, 0xa80000, 1);
        Func_02004d86();
        Call3_02000db8(Func_02004d32, a0, 0x6000, 0);
        Func_02004d5a(a0, 0);
        Call4(Func_02004da4, 0x5180000, -1, 0xa80000, 1);
        Func_02004db0();
        Func_02004d5a_a(a0, 0, 0);
        Func_02004d82(a0, 0);
        Func_02004d8a(a0, 0);
        Func_02004d92(a0, 0);
        Func_02004dc2(0, 0);
        Func_02004d7a_a(a0, 2);
        if (Value2(Func_02004db2, a0, 0) != 0) {
            goto L_02000f28;
        }
        Func_02004d90(a0, 2);
        Call1(Func_02004dae, 0x20d4);
        Func_02004dc6(a0, 0);
    }
    Call1(Func_02004dbc, 0x20e1);
    Func_02004dac(a0, 2);
    Func_02004ddc(a0, 0);
    Call3_02000db8(Func_02004df8, 0, 0x4000, 0);
    Func_02004e02(1, 0, 0);
    Call3_02000db8(Func_02004e0e, 2, 0x8000, 0);
    Func_02004dde(1, 2);
    Func_02004e0e_a(1, 0);
    Func_02004dee(2, 2);
    Func_02004e1e(2, 0);
    Func_02004dee_a(3, 3);
    Func_02004e2e(3, 0);
    Func_02004df6(3, 3);
    Func_02004d94(1);
    Func_02004e04(1, 3);
    Func_02004da2(2);
    Func_02004e12(2, 3);
    Func_02004db0_a(1);
    Func_02004e28(0, 3);
    Func_02004dbe(6);
    Func_02004e2e_a(1, 2);
    record = Value1(Func_02004dec, 0);
    if (record != 0) {
        Func_02004e1e_a(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02004e4e(2, 2);
    record = Value1(Func_02004e0c, 0);
    if (record != 0) {
        Func_02004e3e(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02004e6e(3, 2);
    record = Value1(Func_02004e2c, 0);
    if (record != 0) {
        Func_02004e5e(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02004e7e(a0, (p9 - 16), (p10 - 64));
    Func_02004e98(1, 0, 0);
    Func_02004ea2(2, 0, 0);
    Func_02004eac(3, 0, 0);
    Func_02004ea8(a0, (p9 - 16), (p10 - 16));
    Func_02004eb2(a0, p9, p10);
    Call3_02000db8(Func_02004f26, a0, 0xc000, 10);
    Func_02004e82();
    p9b = (p9 - 16);
    p10b = ((s32)p9 << 16);
}

void FieldScene_RunScene3ba_020015e0(s32 a0)
{
    extern s16 Data_02000240_t[][1];
    extern u8 Data_0200c41c[];

    extern u8 Data_02000240[];
    extern u8 Data_03001ebc[];

    u32 i;
    s32 rec8;
    s32 record;

    if (Data_02000240_t[225][0] == 2) {
        Func_02003152();
    } else {
        Func_0200529c();
        rec8 = Value2(Func_02003368, a0, 3);
        if (rec8 == 0) {
            Call1(Func_02005340, 0x2095);
            Func_02001748();
            Call2(Func_02005390, 0x30000, 0x6000);
            Call4(Func_020053aa, 0x3480000, -1, 0xd80000, 1);
            ((void (*)())Func_020053b6)();
            Func_0200537e(a0, 0);
            Func_0200179a();
            Func_020052e0(60);
            Func_02005390_a(a0, 0);
            Value3(Func_02003f6c, 0, 0x2e0, 200);
            Value3(Func_020053b6_a, 0, 0, 0);
            Func_020017e2();
            Call3_020015e0(Func_02005340_a, 0, 0x10000, 0x8000);
            Call3_020015e0(Func_0200536c, 0, 0x330, 200);
            Func_02005322(30);
            Call3_020015e0(Func_020053f4, 0, 0x105, 60);
            Func_020053dc(a0, 0);
            Func_0200418e(0);
            Func_02005412(0, 0);
            Func_020034d2(a0, 3);
        } else {
            if (rec8 == 1) {
                Call1(Func_020053ee, 0x2094);
                Func_02005406(a0, 0);
            }
        }
        Value3(Func_0200354c, rec8, a0, 3);
        Func_0200537c();
    }
}

void Func_020016ec(s32 scene)
{
    extern s16 Data_02000240[];

    s32 path;

    if (SceneTransition_Phase == 2) {
        Func_0200325e();
        return;
    }
    Func_020053a8();
    path = Func_02003474(scene, 4);
    if (path == 0) {
        Call1_020016ec(Func_0200544e, 8345);
        Call2_020016ec(Func_0200bd70, 196608, 24576);
        Call4_020016ec(Func_0200bd78, 71303168, -1, 11010048, 1);
        Func_020054c0();
        Func_02005488(scene, 0);
        Value3_020016ec(Func_0200447a, 120, 72, 0);
        Func_0200bd40(scene, 0);
        Func_02004546();
        Func_020053fc(15);
        Value3_020016ec(Func_02004080, 0, 984, 200);
        Func_020054ca(0, 0, 10);
        Func_020054c2(scene, 0);
        Call3_020016ec(Func_020054de, 0, 16384, 30);
        Call3_020016ec(Func_020054fa, 0, 262, 60);
        Call3_020016ec(Func_02005474, 0, 98304, 49152);
        Func_02004606(0, 1000, 192);
        Func_02004610(0, 1000, 176);
        Call3_020016ec(Func_0200461c, 0, 1016, 168);
        Func_0200546a(15);
        Value3_020016ec(Func_02001b18, 18, 160, 0);
        Call4_020016ec(Func_02005566, 71303168, -1, 11010048, 1);
        Func_020054f6(0, 1);
        Func_02005494(10);
        Call3_020016ec(Func_020054d2, 0, 65536, 32768);
        Call3_020016ec(Func_020054fe, 0, 1192, 168);
        Func_0200bc98(10);
        Call3_020016ec(Func_02005578, 0, 32768, 30);
        Call3_020016ec(Func_02005594, 0, 258, 60);
        Func_0200557c(scene, 0);
        Func_0200432e(0);
        Func_020055b2(0, 0);
        Call3_020016ec(Func_02005550, 18, 66584576, 11010048);
        Func_02003680(scene, 4);
    } else if (path == 1) {
        Call1_020016ec(Func_0200559c, 8344);
        Func_020055b4(scene, 0);
    }
    Value3_020016ec(Func_020036fa, path, scene, 4);
    Func_0200552a();
}

void Actor_PlaceSlots1To3FromWork(void)
{
    extern s32 Data_02000240[];

    {
        s32 x = Func_02005508(896);
        s32 y = Func_02005512(904);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Func_020055b8(1, x, y);
    }
    {
        s32 x = Func_02005530(912);
        s32 y = Func_0200553a(920);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Func_020055dc(2, x, y);
    }
    {
        s32 x = Func_02005554(928);
        s32 y = Func_0200555e(936);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Func_02005600(3, x, y);
    }
}

void State_SetStateHalfword386To99WhenMatched(void)
{
    extern s16 Data_02001000;
    extern s32 Data_0200c41c;
    extern u8 *Data_03001ebc;

    extern s32 Data_02000240[];

    u8 *state = Data_03001ebc;
    s32 sel = Data_02000240[125];

    if (sel != 0 && ((s32)(s16)*(u16 *)(state + 382) >> 10) == sel
        && Func_0200565e(321) != 0) {
        u16 *p = (u16 *)(state + 386);
        s32 val = 99;

        *p = val;
    }
}

void Scene_RunNearestActor165Scene(void)
{
    extern s16 Data_02000240_t[][1];
    extern u8 Data_0200c41c[];

    extern s32 Data_02000240[];
    extern u8 *Data_03001ebc;

    u8 *state = Data_03001ebc;
    s32 best = 8;
    s32 bestd = 0x100000;
    s32 n = Data_02000240[125];
    Obj *p = Func_02005708(n);
    s32 i;
    s32 *q;
    s32 base;

    Func_020056f6();
    for (i = 8; i <= 66; i++) {
        Obj *o = Func_02005716(i);

        if (o != 0 && o->f54 == 1 && *o->f50->f28 == 165) {
            s32 dx = (p->f08 - o->f08) / 65536;
            s32 dy = (p->f10 - o->f10) / 65536;

            if (dy <= 0) {
                s32 a = dx;
                s32 d;

                if (a < 0) a = -a;
                if (dy < 0) dy = -dy;
                d = a + dy;
                if (d < bestd) {
                    best = i;
                    bestd = d;
                }
            }
        }
    }
    Func_020057f0(0x2085);
    Func_02005808(best, 0);
    q = (s32 *)(state + 448);
    *q = 512;
    *(s32 *)(state + 456) = 15;
    Func_02005780(20);
    Func_020058a4();
    Func_020058b0();
    base = n << 4;
    Func_02005770(base + 880, p->f08 >> 20);
    {
        s32 v = p->f10 >> 20;

        Func_02005780_a(base + 888, v);
    }
    n++;
    if (n > 3) {
        Func_020058a2(10);
        Func_02005782(282);
    } else {
        Func_0200343a(n);
        Func_020058de();
        Func_020058f2();
        *q = 0;
    }
    Func_020057e4();
}

void RunPartyCountInteraction(s32 actorId)
{
    PartyInteractionRecord *record;
    s32 x;
    s32 y;

    record = GetPartyInteractionRecord();
    x = record->x;
    y = record->y;
    FinishPartyInteractionRecordRead();

    if (GetPartyMemberCount() <= 1) {
        SetInteractionCue(0x20e5);
        if (CheckActorInteraction(actorId, 0) == 0) {
            InitializeActorZero();
            InitializeSelectedActor(actorId);
            PlaceSelectedActor(actorId, x, y + 0x40);
            SetInteractionStep(15);
            PlaceActorZero(0, x, y);
            PlaceSupportActor(0, x, y + 0x20);
            StartInteractionPhase();
            AdvanceInteractionPhase();
            SelectInteractionStep(11);
        }
    } else {
        SetLargePartyInteractionCue(0x20e8);
        RunLargePartyInteraction(actorId, 0);
    }

    FinishPartyCountInteraction();
}

void Scene_RunSixSteps380To3A8(void)
{
    Func_0200599a(896, 0);
    Func_020059a4(904, 0);
    Func_020059ae(912, 0);
    Func_020059b8(920, 0);
    Func_020059c2(928, 0);
    Func_020059cc_a(936, 0);
}

s32 Dialogue_RunFlagGatedPromptInteraction(s32 a, s32 b)
{
    extern s16 Data_02000240[];
    extern u16 Data_02001000;
    extern u32 Data_0200c41c;

    s32 v;
    s32 id;
    s32 r;

    Func_02005b3c();
    Func_02005994(b, 5);
    v = Data_02000240[224];
    if (v == (s32)&Value_0000008f) {
        id = (s32)&Value_00002076;
    } else if (v == (s32)&Value_00000090) {
        id = (s32)&Value_00002078;
    } else {
        id = (s32)&Value_0000207a;
    }
    Func_02005aca(id);
    Func_02005ae2(a, 0);
    if (Func_02005a04(b + 512) != 0) {
        return 2;
    }
    if (Func_02005a14(b + 520) != 0) {
        r = Func_020059f6(0);
        if (r == 1) {
            return 2;
        }
        if (r == 2 || r == -1) {
            return 3;
        }
        return r;
    }
    Func_02005a44(b + 520);
    Func_02005b1a((s32)&Value_0000207c);
    Func_02005b2a(a, 0);
    return Func_02005aaa(0, 0);
}

void State_SendIdBySceneId(s32 a, s32 b)
{
    extern s16 Data_02001000;
    extern s32 Data_0200c41c;
    extern u8 *Data_03001ebc;

    extern s16 Data_02000240[];

    s32 v;
    s32 id;

    Func_02005a4a(b, 5);
    v = Data_02000240[224];
    if (v == (s32)&Value_0000008f) {
        id = (s32)&Value_00002076;
    } else if (v == (s32)&Value_00000090) {
        id = (s32)&Value_00002078;
    } else {
        id = (s32)&Value_0000207a;
    }
    Func_02005b82(id + 1);
    Func_02005b9a(a, 0);
}

void Scene_RunMiddleSequence(s32 mode, s32 owner, s32 base)
{
    extern u8 Data_02000240[];

    s32 rec;
    s32 record;
    s32 p9;
    s32 p11;
    s32 count;
    s32 state;
    s32 obj;
    s32 hi;
    s32 lo;
    s32 tail;
    s32 sx;
    s32 sy;
    s32 i;
    u8 buf[8];

    rec = Value1_02001e7c(Func_02005b4e, owner);
    p9 = *(s16 *)(rec + 10);
    p11 = *(s16 *)(rec + 18);
    if (mode != 3) {
        count = Value0(Func_02005b24);
        for (i = 0; i < count; i++) {
            buf[i] = Data_02000240[504 + i];
        }
        if (count <= 1) {
            Func_02005cc2(0x2083);
            Func_02005cda(owner, 0);
            return;
        }
        if (Value1_02001e7c(Func_02005b38, base + 512) != 0) {
            Func_02005cc2(0x2084);
            Func_02005cda(owner, 0);
            return;
        }
        if (mode == 2) {
            state = 0;
            Func_020059fc(6);
        } else {
            Call1_02001e7c(Func_02005c2c, 0x207d);
            Value2_02001e7c(Func_02005c3c, owner, 0);
            state = Value2_02001e7c(Func_02005bbc, 0, 0);
        }
        if (state == 0) {
            if (state < count) {
                for (i = 0; i < count; i++) {
                    Func_02005bb0((s32)(s8)buf[i]);
                }
            }
            for (i = 0; i < count; i++) {
                if ((s32)(s8)buf[i] != 0) {
                    Func_02005bc4((s32)(s8)buf[i]);
                }
            }
            obj = Value0(Func_02005d3e);
            for (i = 0; i < count; i++) {
                Func_02005bee((s32)(s8)buf[i]);
            }
            for (i = 0; i < count; i++) {
                Func_02005c00((s32)(s8)buf[i]);
            }
            if (obj != -1) {
                goto L_main;
            }
        }
    }
    Func_02005cc2(0x207e);
    Func_02005cda(owner, 0);
    return;
L_main:
    ((void (*)())Func_02005bc4_a)(obj, 1);
    Call1_02001e7c(Func_02005cda_a, 0x207f);
    Func_02005cf2(owner, 0);
    Call3_02001e7c(Func_02005c88, 0, 0x10000, 0x8000);
    Call3_02001e7c(Func_02005c96, obj, 0x10000, 0x8000);
    Call3_02001e7c(Func_02005ca4, owner, 0x10000, 0x8000);
    record = Value1_02001e7c(Func_02005c9a, 0);
    if (record != 0) {
        Func_02005ce8(obj, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    hi = p11 + 16;
    Func_02005cea(obj, p9, hi);
    lo = p9 + 16;
    Value3_02001e7c(Func_02005cf4, 0, lo, hi);
    ((void (*)())Func_02005d3e_a)(obj, 0, 30);
    Func_02005d1e(obj, 3);
    tail = hi - 32;
    Func_02005d30(0, 3);
    Func_02005d1a(owner, p9, tail);
    Value3_02001e7c(Func_02005d1c, owner, lo, tail);
    Func_02005e2c(0, obj);
    Func_02005d36(obj, p9, tail);
    Func_02005d56(owner, 1);
    Call3_02001e7c(Func_02005db2, owner, 0x8000, 0);
    Func_02005d56_a(obj, p9, p11 - 48);
    Func_02005d60(owner, p9, tail);
    Func_02005d6a(owner, p9, p11);
    Func_02005d10(obj);
    ((void (*)())Func_02005cf4_a)(base + 512);
    rec = Value1_02001e7c(Func_02005d52, obj);
    sx = *(volatile s32 *)(rec + 8) >> 20;
    Func_02005d1e_a((obj << 4) + 880, sx);
    sy = *(volatile s32 *)(rec + 16) >> 20;
    Func_02005d2c((obj << 4) + 888, sy);
}

void Actor_ApplyValueAndMatchingSlots(s32 a, s32 b)
{
    Rec *t = Func_02005d34(a);
    s32 i;

    Func_02005d46(a, b);
    for (i = 0; i <= 14; i++) {
        if (t->fd8[i] == b) {
            Func_02005d62(a, i);
        }
    }
}

void SceneData_SelectBlockAndResetCounters(s32 a, s32 b)
{
    extern u8 Data_0200be76[];
    extern u8 Data_0200c57e[];
    extern u8 Data_0200c628[];

    u8 *p;

    Data_0200c790 = a;
    Data_0200c764 = b << 4;
    Func_020061d0(Data_0200a1b9, 3200);
    p = Data_0200c57e;
    if (a == 2) {
        p = Data_0200be4e;
    }
    if (a == 4) {
        p = Data_0200c5aa;
    }
    if (a == 3) {
        if (b != 0) {
            p = Data_0200be76;
        } else {
            p = Data_0200c628;
        }
    }
    Data_0200c79c = 0;
    Data_0200c7a0 = p;
    Data_0200c7f8 = 0;
    Data_0200c76c = 0;
    Data_0200c770 = 0;
}

void Scene_RunTwoArmSequence(s32 a)
{
    extern u16 Data_0200c57e[];

    if (a == 0) {
        Func_020063e2();
        Func_020064f6();
        Func_0200650a();
        Func_020063e8(30);
        Func_0200654e(89);
        Func_02004880(0);
        Func_02004e0c_a(1, 0);
        Func_02006402(120);
        Func_02006416();
    } else {
        Func_0200656e(247);
        Func_0200641a();
        Func_0200652e();
        Func_02006542();
        Data_0200c57e[15] = a * 60;
        Func_0200642a(30);
        Func_02006592(a + 90);
        Func_020048c4(a);
        Func_02004e50(1, 0);
        Func_02006446(120);
        while (Func_020065ba() != 0) {
            Func_020062be(1);
        }
        Func_020065bc(0x121);
        Func_020048ee(5);
        Func_02004e7a(2, 0);
        Func_020065d0(236);
        Func_02006476(60);
        Func_02004e8e(2, 1);
        Func_020065e4(236);
        Func_0200648a(60);
        Func_0200491c(6);
        Func_02004ea8_a(2, 0);
        Func_020065fe(236);
        Func_020064a4(60);
        Func_02004936(7);
        Func_02004ec2(4, 0);
        Func_02006618(237);
        Func_020065fc();
        Func_020064d0();
        Func_0200648e(0x123);
    }
}

void Scene_RunLateSequence(s32 a0)
{
    extern u8 Data_02000240[];
    extern u8 Data_02000240_t[][2];
    extern struct ModeRecord Data_0200be76;
    extern struct ModeRecord Data_0200c628;
    extern u8 Data_03001ebc[];
    void Func_02002ba8();
    void Func_02002d84();
    void Func_02006c4e();
    void Func_02006d62();

    s32 kind;

    Func_02006644(247);
    Func_02006600();
    Func_02006614();
    Data_0200c628.span = a0 * 60;
    Data_0200be76.span = (a0 < 0 ? -a0 : a0) * 60;
    if (a0 < 0) {
        Func_02006512(30);
        Func_02006678(86);
        Func_020049aa(8);
        Value2_02002844(Func_02004f36, 3, 1);
        Func_02006532(-a0 * 60 + 60);
        kind = 0;
    } else {
        Func_0200653c(30);
        Func_020066a4(a0 + 90);
        Func_020049d6(4);
        Value2_02002844(Func_02004f62, 3, 0);
        Func_0200655a(a0 * 60 + 60);
        kind = 8;
    }
    Call3_02002844(Func_0200662c, kind, 0x105, 0);
    while (Value0(Func_020066d8)!= 0) {
        Func_020063dc(1);
    }
    Func_020066da(19);
    Func_02006580(30);
    Call1_02002844(Func_020066e6, 0x121);
    Func_020066aa();
    Func_020066b6();
}

void OvObj_ResetMotionFields(void)
{
    Obj_02002bac *o = Func_02006868();

    Func_0200677e(o);
    o->f24 = 0;
    o->f2c = 0;
    o->f38 = 0x80000000;
    o->f40 = 0x80000000;
}

void State_InitHalfwordC6a6Once(void)
{
    extern s16 Data_02001000;
    extern s32 Data_0200c41c;
    extern u8 *Data_03001ebc;

    if (Data_0200c6a6 == -1) {
        Data_0200c6a6 = Func_02006816();
    }
}

void State_StoreParamsAndInitTable(s32 a, s32 b, s32 c)
{
    extern s16 Data_02001000;
    extern s32 Data_0200c41c;
    extern u8 *Data_03001ebc;

    Func_02005902();
    Data_0200c7f4 = a;
    Data_0200c780 = b;
    Data_0200c758 = c & 3;
    Data_0200c774 = 0;
    Data_0200c78c = 0;
    {
        s32 n = 3200;

        Func_0200686c(Data_0200abed, n);
    }
}

void State_InitTableWordsAndLoad3200(s32 a, s32 b, s32 c)
{
    extern s16 Data_02001000;
    extern s32 Data_0200c41c;
    extern u8 *Data_03001ebc;

    Data_0200c760 = a;
    Data_0200c800 = b;
    Data_0200c7a4 = Data_0200c7f4;
    Data_0200c7bc = Data_0200c780;
    Data_0200c78c = c;
    Data_0200c750 = 0;
    {
        s32 n = 3200;

        Func_020068c8(Data_0200abed, n);
    }
}

void State_ReleaseTableAndResetC6a6(void)
{
    extern s16 Data_02001000;
    extern s32 Data_0200c41c;
    extern u8 *Data_03001ebc;

    Func_02006906(Data_0200abed);
    Func_02006960(Data_0200c6a6);
    Data_0200c6a6 = -1;
}

void Actor_StartMode5MoveToTile(s32 a, s32 b, s32 c)
{
    Obj_02002e10 *o = Func_02006be0(a);

    if (o != 0) {
        s32 v = 0x20000;
        s32 z = 0;

        o->f30 = v;
        o->f34 = v >> 1;
        o->f5b = z;
        Func_020069fc(o);
        Func_020069e4(o, 5);
        Func_02006a20(o, b << 16, o->f0c, c << 16);
    }
}

void OvObj_PlaceWithScale14000(s32 a, s32 b, s32 c)
{
    Obj_02002e50 *o = Func_02006c20(a);

    if (o != 0) {
        s32 v = 0x14000;
        s32 z = 0;

        o->f30 = v;
        o->f34 = v >> 1;
        o->f5b = z;
        Func_02006a3c(o);
        Func_02006a24(o, 5);
        Func_02006a60(o, b << 16, o->f0c, c << 16);
        Func_02006a6e(o);
        Func_02006a3e(o, 1);
    }
}

void Text_WriteU32AsHex(u8 *buf, u32 value)
{
    s32 i;

    buf += 8;
    *buf = 0;
    buf--;
    for (i = 7; i >= 0; i--) {
        *buf = HexDigits[value & 15];
        value >>= 4;
        buf--;
    }
}

void Resource3ba_NoOpCallback(void)
{
}

void State_SetHalfword1000To9(void)
{
    extern s16 Data_02000240[];
    extern u16 Data_02001000;
    extern u32 Data_0200c41c;

    u16 *p = &Data_02001000;
    s32 v = 9;

    *p = v;
}

void State_WaitUntilWord1000IsNine(void)
{
    extern s16 Data_02001000;
    extern s32 Data_0200c41c;
    extern u8 *Data_03001ebc;

    s16 *p = &Data_02001000;

    while (*p != 9) {
        Func_02006ab8(1);
    }
}

void Effect_SpawnKind285AtRandomChance(Obj_02002fc4 *a)
{
    s32 t[3];
    u32 n;

    if (a->f28 >= -255 && a->f28 <= 255) {
        a->f55 = 0;
    }
    n = Func_02006b00();
    if (n * 100 >> 16 <= 9) {
        Obj_02002fc4 *o;
        s32 u;
        s32 w;

        t[0] = a->f08;
        t[1] = a->f0c;
        t[2] = a->f10;
        u = Func_02006b1c();
        w = Func_02006b22();
        Func_02006b3e(u << 4, w, t);
        {
            s32 x = t[0];
            s32 y = t[1];
            s32 z = t[2];

            o = Func_02006bd2(285, x, y, z);
        }
        if (o != 0) {
            o->f55 = 0;
            Func_02006c3e_a(o, 0);
            Func_02006be6(o, (s32)Data_0200bfe4);
            Func_02006be6_a(o, 1);
            Func_02006bee(o, 0);
        }
    }
}

s32 Actor_PlaceLinkedActorAbove(Obj_02003058 *a)
{
    extern s32 Data_02000240[];

    Obj_02003058 *o = Func_02006d24(a->f64);

    Func_02006c56(o, a->f08, a->f0c + 0x240000, a->f10);
    o->f55 = 0;
    Func_02006c3e(o, (s32)Data_0200c008);
    Func_02006e8c(83);
    a->f64 = 0;
    return 0;
}

s32 Scene_RunFlag211ApproachScene(s32 handle_a, s32 handle_b)
{
    extern u8 Data_02000240[];

    u8 *work = *(u8 **)0x03001f3c;
    u8 *shared;
    u8 *rec;
    s32 flag;
    s32 x;
    s32 z;
    u16 *cuep;
    s16 *waitp;

    flag = Func_02006d18(0x211);

    shared = Data_02000240;
    rec = Func_02006d88(*(s32 *)(shared + 500));

    if (*(s32 *)(work + 232) < *(s32 *)(rec + 8)) {
        x = *(s32 *)(work + 232) + 0xc0000;
    } else {
        x = *(s32 *)(work + 232) - 0xc0000;
    }

    if (flag != 0) {
        z = *(s32 *)(work + 236) + 0x100000;
        cuep = (u16 *)(work + 228);
    } else {
        z = *(s32 *)(work + 236) - 0x100000;
        cuep = (u16 *)(work + 226);
    }

    waitp = (s16 *)(rec + 100);
    *waitp = *cuep;
    *(s32 *)(rec + 52) = 0x4000;
    *(s32 *)(rec + 48) = 0x10000;

    Func_02006d02(rec, x, 0, z);
    Func_02006d90(0x211);
    Func_02006ce8(rec, (void *)0x0200c6fc);

    while (*waitp != 0) {
        Func_02006c4e(1);
    }

    if (flag == 0) {
        Func_02005242(0, handle_a);
        Func_02006d82(handle_a, 2);
    } else {
        Func_02005254(0, handle_b);
        Func_02006d94(handle_b, 2);
    }

    shared = Data_02000240;
    Func_02006da4(*(s32 *)(shared + 500), 1);
    Func_02006da4_a(0x96a, 3);
    Func_02006d62(rec);

    return flag;
}

/*
 * Seven arguments: four in registers, three from the caller's stack. The
 * 232-byte owner includes the six-word literal pool the body branches over.
 * The palette index stored at +216 is passed on sign-extended from sixteen
 * bits, so the narrowing is deliberate. Descriptor layout is asserted only
 * for the fields written here, and the actor records are touched at +8 and
 * +16 only on the flag-clear path.
 */
void Scene_BuildSceneDescriptorAndInstallTask(s32 first, s32 second, s32 mode, s32 centre,
                   s32 extra, s32 third, s32 fourth)
{
    extern Ctl Data_02001000;

    u8 *desc;
    u8 *rec0;
    u8 *rec1;
    s32 handle;
    s32 pal;

    desc = Func_020072c6(59, 0x7170);
    handle = Func_020072e0(512);

    *(u16 *)(desc + 222) = (u16)first;
    *(u16 *)(desc + 224) = (u16)second;
    *(u16 *)(desc + 226) = (u16)third;
    *(u16 *)(desc + 228) = (u16)fourth;
    *(u16 *)(desc + 230) = (u16)mode;
    *(s32 *)(desc + 232) = centre;
    *(s32 *)(desc + 236) = extra;

    rec0 = Func_02007478(first);
    rec1 = Func_02007480(second);

    if (Func_02007428(0x109) == 0) {
        *(s32 *)(rec1 + 8) =
            (centre << 1) - *(s32 *)(rec0 + 8);
        *(s32 *)(rec1 + 16) = *(s32 *)(rec0 + 16);
    }

    *(u16 *)(desc + 218) = 0;
    *(u16 *)(desc + 220) = 0;

    Func_02007356(Data_0200bf14, handle);

    pal = Func_0200737a();
    *(u16 *)(desc + 216) = (u16)pal;
    Func_02007386((s16)pal, 512, handle);

    Func_02007326((s32)Func_0200b3a0 + 1, 0xc76);

    Func_02007374(handle);
}

void State_InitControlWhenFlag109Clear(void)
{
    extern Ctl Data_02001000;

    u8 *state = Data_03001f3c;
    Ctl *m = &Data_02001000;

    Func_020073be(Func_020073ee(), (s32)(state + 240));
    if (Func_020074bc(0x109) == 0) {
        m->f0 = 1;
        m->f2 = 1;
        m->f4 = *(u16 *)(state + 224);
        m->f8 = 0;
        m->f6 = 0;
    }
    {
        s32 e = 0xc85;

        Func_02007392((s32)Data_0200b1c1, e);
    }
}

void State_SetStateHalfword220(s32 a)
{
    extern s16 Data_02001000;
    extern s32 Data_0200c41c;
    extern u8 *Data_03001ebc;

    u8 *p = Data_03001f3c;

    *(s16 *)(p + 220) = a;
}

s32 *Actor_FindSlotAtTilePosition(s32 *arg0)
{
    extern u8 *Data_03001ebc;

    s32 **slots = (s32 **)(Data_03001ebc + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if ((arg0[0] >> 20) == (p[2] >> 20)
            && (arg0[1] >> 20) == (p[3] >> 20)
            && (arg0[2] >> 20) == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

/*
 * The push interaction: probe the cell one step ahead of the subject and, if
 * something occupies it, slide it and the subject one step on. The 360-byte
 * owner includes its four-word literal pool, which ends where the next owner
 * begins. Record fields are asserted only where written -- facing at +6,
 * position at +8/+12/+16, state at +0x22, occupancy flag bit 0 at +0x59 -- and
 * the terrain probe is tested signed, so only a positive code refuses.
 */
void StagedActor_PushActorAhead(void)
{
    extern s16 Data_02000240[];

    SceneRecord *subject;
    SceneRecord *target;
    SceneRecord *blocker;
    u32 step;
    u32 dir;
    Position3 pos;
    u32 idx = 250;
    s32 zero;
    s32 handle;

    handle = *(s32 *)((u8 *)Data_02000240 + (idx << 1));
    subject = Func_020075cc(handle);

    dir = subject->facing >> 12;

    step = Data_0200c154[dir];
    pos.x = subject->x + (s32)(step & 0xffff0000);
    pos.y = subject->y;
    step <<= 16;
    pos.z = subject->z + (s32)step;

    target = Func_020071f6(&pos, subject);
    if (target == 0) {
        return;
    }

    /* Is the cell one step beyond the target already taken? */
    step = Data_0200c154[dir];
    pos.x = target->x + (s32)(step & 0xffff0000);
    pos.y = target->y;
    step <<= 16;
    pos.z = target->z + (s32)step;

    blocker = Func_02007220(&pos, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    /* ...and the cell directly above the target? */
    pos.x = target->x;
    pos.y = target->y + 0x100000;      /* 128 << 13 */
    pos.z = target->z;

    blocker = Func_0200724c(&pos, target);
    if (blocker != 0 && (blocker->flags & 1) != 0) {
        return;
    }

    target->state = 2;
    zero = 0;

    step = Data_0200c154[dir];
    pos.x = target->x + (s32)(step & 0xffff0000);
    pos.y = target->y;
    step <<= 16;
    pos.z = target->z + (s32)step;

    if (Func_020075e6(target, &pos) > 0) {
        return;
    }

    Func_02007594(subject, 8);
    Func_020074fa(15);

    target->rate_x = 0x3333;
    target->rate_z = 0x3333;
    Func_020075da(target, pos.x, pos.y, pos.z);

    /* The same destination block, moved onto the subject this time. */
    subject->rate_x = 0x3333;
    subject->rate_z = 0x3333;
    Func_020075ea(subject, pos.x, pos.y, pos.z);

    Func_02007810(0xee);
    Func_020075fe(target);
    Func_0200781e(0x120);                                /* 144 << 1 */

    target->x = pos.x;
    target->z = pos.z;
    target->motion_24 = zero;
    target->motion_2c = zero;

    Func_020075e4(subject, 1);
}

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay.
 * The occupancy lookups take the position block; the record pointer the call
 * sites also pass is not asserted as an argument. */

/*
 * Probe the two cells ahead of the active subject and return what occupies the
 * nearer one, else the further one, else zero. The 160-byte owner includes its
 * alignment bytes and two-word literal pool. Facing is the biased quadrant of
 * the halfword at +6, with no sign extension; each probe rounds x and z down to
 * whole units and re-centres them by half a unit, carrying y unrounded. Only
 * the record fields at +6, +8, +12 and +16 are asserted.
 */
s32 *Actor_FindOccupantAheadOfSubject(void)
{
    extern s16 Data_02000240[];

    u8 *rec;
    s32 facing;
    s32 pos[3];
    s32 *hit;

    rec = Func_0200772c(((ActiveSubjectSlot *)Data_02000240)->handle);

    /* 128 << 6 = 0x2000 bias, then masked to bits 14-15 (192 << 8). */
    facing = (*(u16 *)(rec + 6) + 0x2000) & 0xc000;

    pos[0] = (*(s32 *)(rec + 8) & 0xfff00000) + 0x80000;
    pos[1] = *(s32 *)(rec + 12);
    pos[2] = (*(s32 *)(rec + 16) & 0xfff00000) + 0x80000;
    Func_020075de(0x100000, facing, pos);          /* 128 << 13 */

    hit = Func_02007366(pos, rec);
    if (hit == 0) {
        pos[0] = (*(s32 *)(rec + 8) & 0xfff00000) + 0x80000;
        pos[1] = *(s32 *)(rec + 12);
        pos[2] = (*(s32 *)(rec + 16) & 0xfff00000) + 0x80000;
        Func_02007610(0x200000, facing, pos);      /* 128 << 14 */

        hit = Func_02007398(pos, rec);
    }

    return hit;
}



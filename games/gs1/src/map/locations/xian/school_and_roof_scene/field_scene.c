#include "types.h"

#define OpenSceneExit Func_02004edc
#define WaitSceneExitStep Func_02004e58
#define ClearSceneExitGateAtEntry Func_02004f0a
#define GetSceneExitPendingWork Func_02004f1e
#define ResetSceneExitPendingWork Func_02004f2a
#define SetSceneExitCompletionMode Func_02004f34
#define ClearSceneExitField40 Func_02004f40
#define SetSceneExitGate Func_02004f48
#define TransitionSceneExitSlot Func_02004fd0
#define IsFlag0895Set Func_02004f16
#define IsFlag089bSet Func_02004f28
#define ShowSceneExitDialogue1a5b Func_02005000
#define ShowSceneExitDialogue189e Func_02005012
#define ShowSceneExitDialogue182a Func_0200502c
#define FinalizeSceneExitSlot Func_02005044
#define SetSceneExitHeading Func_02004fa2
#define ClearSceneExitGateBeforeDescriptor Func_02004fae
#define InstallSceneExitDescriptor Func_02004fd4
#define CloseSceneExit Func_02004fa0
#define AdvanceSceneStep       Func_02005722
#define OpenSceneSection        Func_0200571c
#define CloseSceneSection       Func_02005784
#define GetSceneRecord          Func_0200573e
#define SetSceneActorMode       Func_0200579c
#define SetSceneActorModeAfterBranch Func_020057ea
#define RunFacingVariantA       Func_020021f6
#define RunFacingVariantB       Func_02001f68
#define RunFacingVariantC       Func_0200213c
#define RunFacingVariantD       Func_02002062
#define RunSceneVariant         Func_020023c8
#define FieldScene_RunFlag88FBranch Func_02000484
#define FieldScene_ShowDialogue17B1 Func_02000abc
#define FieldScene_ShowDialogue1825 Func_02000adc
#define FieldScene_RunRoofSceneExit Func_02000afc
#define FieldScene_ShowDialogue182D Func_02000bd4
#define FieldScene_RunForwardArcBurst Func_02000bf4
#define FieldScene_RunDescentBurst Func_02000db4
#define FieldScene_DispatchApproachByFacing Func_020012e0
#define FieldScene_DispatchByFacing Func_02001334
#define FieldScene_DispatchByFacingAndFlags Func_020013b8
#define FieldScene_RunSkippableStoryBeat Func_02001d50
#define FieldScene_ShowDialogue1A58 Func_02002464
#define FieldScene_DispatchByRange Func_02002508
#define FieldScene_ShowDialogue17DF Func_0200254c
#define FieldScene_SelectData Func_02002574
#define FieldScene_SpawnRandomizedParticle Func_020025b8
#define FieldScene_ApplyOffset0Neg32 Func_0200265c
#define FieldScene_ApplyOffset0Pos32 Func_0200266c
#define FieldScene_ApplyOffsetNeg32_0 Func_0200267c
#define FieldScene_PlaySound123AndEnable Func_02002764
#define FieldScene_SetFlag140AndFinishSequence Func_020040dc
#define FieldScene_FinishSequence Func_02004128
#define FieldScene_SpawnEightShots Func_02004140
#define FieldScene_SelectActorModeFromInputBit Func_020041c4
#define FieldScene_RunParticleRain Func_020041ec

struct SceneRecordHeading {
    u8 pad[6];
    u16 heading;
};

struct SceneActor_02001334 {
    u8 unknown_00[6];
    u16 facing;
};

struct Params {
    s32 unused0;
    s32 field1;
    s32 field2;
    s32 field3;
    u8 pad[24];
};

struct Descriptor {
    u32 field0;
    u8 unused[12];
    u32 field16;
    u32 field20;
    u16 field24;
    u16 unused26;
    u32 field28;
    u8 unused32[8];
};

struct Descriptor_020041ec {
    u32 field0;
    u32 field4;
    u8 unused8[16];
    u16 field24;
    u8 unused26[6];
    u8 unused32[8];
};

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];
extern u8 Value_0000003c;
extern u8 Data_0200cb90[];
extern u8 Data_0200d184[];
extern u8 Data_0200cd40[];
extern s32 Data_03001e40;

void Func_02004864(void);
s32 Func_0200484a(s32);
void Func_02004934(s32);
s32 Func_0200495c(s32, s32);
void Func_02004888(void);
void Func_02004948(s32);
s32 Func_02004958(s32, s32);
s32 Func_020048b0(s32, s32);
s32 Func_0200497c(s32, s32);
s32 Func_020048d4(s32, s32);
void Func_020049a6(s32, s32);
void Func_020048e2(void);
void Func_02004e9c(void);
void Func_02004f62(s32);
s32 Func_02004f8a(s32, s32);
void Func_02004eb6(void);
void Func_02004ebc(void);
void Func_02004f82(s32);
s32 Func_02004faa(s32, s32);
void Func_02004ed6(void);
void Func_02004e58(s32 arg0);
s32 Func_02004f16(s32 flagId);
s32 Func_02004f28(s32 flagId);
void Func_02004edc(void);
void Func_02004fa0(void);
u8 *Func_02004f0a(s32 slot);
u8 *Func_02004f1e(s32 slot);
u8 *Func_02004f2a(s32 slot);
u8 *Func_02004f34(s32 slot);
u8 *Func_02004f40(s32 slot);
u8 *Func_02004f48(s32 slot);
u8 *Func_02004fa2(s32 slot);
u8 *Func_02004fae(s32 slot);
void Func_02004fd4(s32 slot, u8 *descriptor);
void Func_02004fd0(s32 slot, s32 a1, s32 a2);
void Func_02005000(s32 dialogueId);
void Func_02005012(s32 dialogueId);
void Func_0200502c(s32 dialogueId);
void Func_02005044(s32 slot, s32 a1);
void Func_02004fb4(void);
void Func_02004fce(void);
void Func_0200507a(s32 dialogueId);
s32 Func_020050a2(s32 slot, s32 arg1);
u8 *Func_02005004();
void Func_02004f56();
s32 Func_02004f8c();
s32 Func_02004f96();
void Func_020051cc();
void Func_02000dae();
void Func_02000dc6();
void Func_02000de4();
u8 *Func_020051c6(s32 id);
void Func_02005118(s32 arg0);
void Func_02005370(s32 sound_id);
void Func_02000f4c(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
void Func_02000f62(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
void Func_02000f86(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
void Func_02000fa2(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
s32 Func_020056d0(s32 flagId);
void Func_020056c0(void);
void Func_0200570c(void);
u8 *Func_020056ee(s32 slot);
u8 *Func_020056fe(s32 slot);
void Func_02001efc(void);
void Func_02001fe2(void);
void Func_0200247c(void);
void Func_02002350(s32 arg0);
void Func_02005722(s32 frames);
void Func_0200571c(void);
void Func_02005784(void);
struct SceneActor_02001334 *Func_0200573e(s32 slot);
void Func_0200579c(s32 slot, s32 mode);
void Func_020057ea(s32 slot, s32 mode);
void Func_02001f68(void);
void Func_0200213c(void);
void Func_02002062(void);
void Func_020021f6(void);
void Func_020023c8(s32 variant);
u8 *Func_020057c2();
void Func_020057a0();
void Func_0200226c(void);
void Func_02001fde(void);
void Func_020021b2(void);
void Func_020020d8(void);
void Func_020058f4();
void Func_02005914();
void Func_02005910(void);
s32 Func_020057e6();
s32 Func_020057f0();
void Func_0200246a();
void Func_020025a4(void);
void Func_0200582c();
s32 Func_0200580c();
void Func_02002486();
s32 Func_0200581e();
void Func_020028fe(void);
void Func_0200322c(void);
void Func_02005858();
void Func_02006130(void);
void Func_02006148();
void Func_02006148_a();
s32 Func_0200615e(s32 arg0, s32 arg1);
void Func_0200616c(void);
void Func_0200618c(void);
void Func_020061f6(s32 dialogueId);
s32 Func_02006206(s32 slot, s32 arg1);
void Func_02006232(s32 slot, s32 arg1, s32 arg2);
void Func_02006258(s32 slot, s32 arg1, s32 arg2);
void Func_02006844(void);
void Func_0200685e(void);
void Func_0200690a(s32 dialogue_id);
s32 Func_02006932(s32 id, s32 arg1);
void Func_020068f0();
void Func_0200691c();
u8 *Func_02006912();
void Func_020069c8();
void Func_020069e0();
void Func_02006a98();
void Func_0200692c(void);
void Func_0200694e(void);
void Func_020069cc(s32 id, s32 arg1);
void Func_020069fa(s32 dialogue_id);
void Func_02006a12(s32 id, s32 arg1);
u8 *Func_020069c4(s32 arg0);
void Func_02006ac8(s32 id, s32 arg1);
void Func_02006ad2(s32 id, s32 arg1);
s32 Func_02006928(void);
s32 Func_02006944(void);
void Func_02002766(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6,
                          u32 flags, u8 *extra);
void Func_02004cf2(s32 arg0, s32 arg1);
void Func_02004d00(s32 arg0, s32 arg1);
void Func_02004d12(s32 arg0, s32 arg1);
void Func_02006ce6(s32 sound_id);
void Func_02006c74(s32 arg0);
void Func_020084b0();
void Func_02008638();
void Func_0200864c();
void Func_02008666();
void Func_0200845a();
void Func_02008584(s32 id, s32 arg1);
void Func_02008672(s32 arg0);
void Func_0200868e(void);
u8 *Func_0200854c(s32 id);
void Func_02008540(s32 frames);
void Func_020086f6(s32 sound_id);
void Func_020042d6(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
void Func_02008578(s32 frames);
void Func_0200866c(s32 arg0, s32 arg1);
void Func_02008674(s32 arg0, s32 arg1);
u8 *Func_02008606(s32 id);
u8 *Func_0200863a(s32 id);
u8 *Func_02008648(s32 id);
u8 *Func_020086fa(s32 id);
u8 *Func_02008704(s32 id);
void Func_020085e0(s32 frames);
void Func_020085fe(s32 frames);
void Func_02008612(s32 frames);
void Func_020086ae(s32 frames);
void Func_020086c4(s32 frames);
void Func_02008798(s32 id, s32 arg1);
void Func_020087a0(s32 id, s32 arg1);
void Func_0200872a(s32 arg0, s32 arg1);
void Func_020087f0(s32 arg0, s32 arg1);
void Func_02008740(s32 arg0);
void Func_02008806(s32 arg0);
void Func_0200860e(s32 arg0);
s32 Func_020085c6(void);
s32 Func_020085de(void);
s32 Func_020085f2(void);
void Func_02008778(s32 sound_id);
void Func_020087ac(s32 sound_id);
void Func_020087e2(s32 sound_id);
void Func_02008850(s32 sound_id);
void Func_020043fe(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);

void FieldScene_RunFlag88FBranch(void)
{
    Func_02004864();
    if (Func_0200484a(0x88F) != 0) {
        Func_02004934(0x17D6);
        Func_0200495c(12, 0);
        Func_02004888();
    } else {
        Func_02004948(0x1794);
        Func_02004958(12, 0);
        if (Func_020048b0(0, 0) == 1) {
            u16 *q = (u16 *)(Data_03001ebc + 0x1D8);
            q[0] = q[0] + 1;
            Func_0200497c(12, 0);
            if (Func_020048d4(0, 0) == 1) {
                u16 *r = (u16 *)(Data_03001ebc + 0x1D8);
                r[0] = r[0] + 1;
            }
        }
        Func_020049a6(12, 0);
        Func_020048e2();
    }
}

void FieldScene_ShowDialogue17B1(void)
{
    Func_02004e9c();
    Func_02004f62(0x17B1);
    Func_02004f8a(8, 0);
    Func_02004eb6();
}

void FieldScene_ShowDialogue1825(void)
{
    Func_02004ebc();
    Func_02004f82(0x1825);
    Func_02004faa(9, 0);
    Func_02004ed6();
}

void FieldScene_RunRoofSceneExit(void)
{
    OpenSceneExit();

    ClearSceneExitGateAtEntry(12)[91] = 0;

    goto testPendingWork;
waitPendingWork:
        WaitSceneExitStep(1);
testPendingWork:
    if (*(s32 *)(GetSceneExitPendingWork(12) + 12) > 0) {
        goto waitPendingWork;
    }

    *(s32 *)(ResetSceneExitPendingWork(12) + 12) = 0;

    *(s32 *)(SetSceneExitCompletionMode(12) + 60) = 128 << 24;

    *(s32 *)(ClearSceneExitField40(12) + 40) = 0;

    SetSceneExitGate(12)[91] = 1;

    TransitionSceneExitSlot(12, 0, 0);

    if (IsFlag0895Set(0x895) != 0) {
        ShowSceneExitDialogue1a5b(0x1a5b);
    } else if (IsFlag089bSet(0x89b) != 0) {
        ShowSceneExitDialogue189e(0x189e);
    } else {
        ShowSceneExitDialogue182a(0x182a);
    }

    FinalizeSceneExitSlot(12, 0);

    ((struct SceneRecordHeading *)SetSceneExitHeading(12))->heading = 128 << 7;

    ClearSceneExitGateBeforeDescriptor(12)[91] = 0;

    InstallSceneExitDescriptor(12, (u8 *)0x0200c638);
    CloseSceneExit();
}

void FieldScene_ShowDialogue182D(void)
{
    Func_02004fb4();
    Func_0200507a(0x182d);
    Func_020050a2(15, 0);
    Func_02004fce();
}

void FieldScene_RunForwardArcBurst(void)
{
    u8 *record = Func_02005004(19);
    u32 index;
    s32 angle;

    for (index = 8; index > 3; index--) {
        angle = index << 12;
        *(u16 *)(*(u8 **)(record + 80) + 30) = (u16)angle;
        Func_02004f56((index - 4) * 2);
        *(s32 *)(record + 8) += Func_02004f8c(angle)* 6;
        *(s32 *)(record + 16) += Func_02004f96(angle)* 6;
    }

    *(s32 *)(record + 12) = 0x120000;
    *(s32 *)(record + 60) = 0x120000;

    Func_020051cc(227);

    Func_02000dae(*(s32 *)(record + 8) - 0xc0000,
                  *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0x80000,
                  0xffffcccd, 0x6666, 0, 0, 0);
    Func_02000dc6(*(s32 *)(record + 8),
                  *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0x80000,
                  0xffff3334, 0x4ccc, 0, 0, 0);
    Func_02000de4(*(s32 *)(record + 8) + 0xa0000,
                  *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0x80000,
                  0xffff0000, 0x3333, 0, 0, 0);
}

void FieldScene_RunDescentBurst(void)
{
    u8 *record = Func_020051c6(19);
    u32 i = 0;
    s32 step = 8;
    s32 zero;
    do {
        Func_02005118(step);
        *(s32 *)(record + 16) += 0xffff0000;
        *(u32 *)(record + 64) = 0x80000000;
        i++;
        step -= 2;
    } while (i <= 3);
    zero = 0;
    *(u16 *)(*(u8 **)(record + 80) + 30) = (u16)zero;
    Func_02005370(227);
    Func_02000f4c(*(s32 *)(record + 8), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0xfff80000, 0xffff3334,
                  0, 0xffffcccd, 0, 0);
    Func_02000f62(*(s32 *)(record + 8), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0xfff80000, 0x0000cccc,
                  0, 0xffffcccd, 0, 0);
    Func_02000f86(*(s32 *)(record + 8) + 0xfffa0000, *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + (160 << 12), 0x00003333,
                  0, 0xffff0000, 0, 0);
    Func_02000fa2(*(s32 *)(record + 8) + (192 << 11), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + (160 << 12), 0x00003333,
                  0, 0xffff0000, 0, 0);
}

void FieldScene_DispatchApproachByFacing(void)
{
    Func_020056c0();

    if (*(u16 *)(Func_020056ee(0) + 6) > (128 << 7)
        && *(u16 *)(Func_020056fe(0) + 6) < (192 << 8)) {
        Func_02001efc();
    } else {
        Func_02001fe2();
    }

    if (Func_020056d0(0x898) != 0) {
        Func_0200247c();
    } else {
        Func_02002350(0);
    }

    Func_0200570c();
}

void FieldScene_DispatchByFacing(void)
{
    struct SceneActor_02001334 *record = GetSceneRecord(0);
    u16 angle;

    OpenSceneSection();
    SetSceneActorMode(0, 8);
    AdvanceSceneStep(20);

    angle = *(u16 *)((u8 *)record + 6);

    if ((u16)(angle - 0x2000) <= 0x3fffu) {
        RunFacingVariantA();
    } else if ((u16)(angle - 0x6000) <= 0x3fffu) {
        RunFacingVariantB();
    } else if ((u16)(angle + (192 << 7)) <= 0x3fffu) {
        RunFacingVariantC();
    } else {
        RunFacingVariantD();
    }

    SetSceneActorModeAfterBranch(0, 1);
    RunSceneVariant(1);
    CloseSceneSection();
}

void FieldScene_DispatchByFacingAndFlags(void)
{
    u8 *record = Func_020057c2(0);
    u16 facing;

    Func_020057a0();

    facing = *(u16 *)(record + 6);
    if ((u16)(facing - 0x2000) <= 0x3fff) {
        Func_0200226c();
    } else if ((u16)(facing - 0x6000) <= 0x3fff) {
        Func_02001fde();
    } else if ((u16)(facing + 0x6000) <= 0x3fff) {
        Func_020021b2();
    } else {
        Func_020020d8();
    }

    Func_020058f4(0x10000, 0x2000);
    Func_02005914(20, 1);
    Func_02005910();

    if (*(s16 *)(record + 18) <= 209) {
        if (Func_020057e6(0x89a) == 0) goto scene0;
        if (Func_020057f0(0x89b) != 0) goto scene0;
        goto scene1;
scene0:
        Func_0200246a(0);
        goto firstSceneComplete;
scene1:
        Func_020025a4();
firstSceneComplete:
        Func_0200582c();
        return;
    }

    if (Func_0200580c(0x89b) != 0) {
        Func_02002486(2);
    } else if (Func_0200581e(0x89a) == 0) {
        Func_020028fe();
    } else {
        Func_0200322c();
    }
    Func_02005858();
}

void FieldScene_RunSkippableStoryBeat(void)
{
    u8 *workspace;

    Func_02006130();
    Func_020061f6(0x186e);
    Func_02006206(18, 0);

    if (Func_0200615e(0, 0) == 0) {
        Func_02006148(20);
        Func_02006232(18, 0, 20);
        Func_02006148_a(0x898);
        Func_0200616c();
    } else {
        workspace = Data_03001ebc;
        *(u16 *)(workspace + 472) += 1;
        Func_02006258(18, 0, 20);
        Func_0200618c();
    }
}

void FieldScene_ShowDialogue1A58(void)
{
    Func_02006844();
    Func_0200690a(0x1a58);
    Func_02006932(11, 0);
    Func_0200685e();
}

void FieldScene_DispatchByRange(void)
{
    u8 *record;
    u32 biased;

    record = Func_02006912(0);
    biased = *(u16 *)(record + 6);
    Func_020068f0();

    biased = biased + 0xffff5fff;
    if (biased <= 0x3ffe) {
        Func_02006a98(13);
    } else {
        Func_020069c8(0x1a1c);
        Func_020069e0(13, 0);
    }

    Func_0200691c();
}

void FieldScene_ShowDialogue17DF(void)
{
    Func_0200692c();
    Func_020069cc(8, 2);
    Func_020069fa(0x17df);
    Func_02006a12(8, 0);
    Func_0200694e();
}

s32 FieldScene_SelectData(void)
{
    if (Data_02000240[224] == (s32)&Value_0000003c) {
        return (s32)Data_0200cb90;
    }
    if (Data_02000240[225] == 3) {
        return (s32)Data_0200d184;
    }
    return (s32)Data_0200cd40;
}

void FieldScene_SpawnRandomizedParticle(void)
{
    struct Params params;
    u8 *record;
    s32 draw;
    s32 offset;

    record = Func_020069c4(0);

    params.field1 = 7;
    draw = (u32)(Func_02006928() * 7) >> 16;
    if ((draw & 7) == 0)
        params.field1 = 5;

    params.field2 = 0xb333;
    params.field3 = 0xcccc;

    offset = ((u32)(Func_02006944() * 8) >> 16) * 13107;

    Func_02002766(*(s32 *)(record + 8) + ((8 - (Data_03001e40 & 15)) << 16),
                  *(s32 *)(record + 12) + (192 << 13),
                  *(s32 *)(record + 16),
                  0,
                  -offset,
                  0,
                  144 << 12,
                  (u8 *)&params);

    if ((Data_03001e40 & 1) != 0)
        Func_02006ac8(0, 15);
    else
        Func_02006ad2(0, 1);
}

void FieldScene_ApplyOffset0Neg32(void)
{
    Func_02004cf2(0, -32);
}

void FieldScene_ApplyOffset0Pos32(void)
{
    Func_02004d00(0, 32);
}

void FieldScene_ApplyOffsetNeg32_0(void)
{
    Func_02004d12(-32, 0);
}

void FieldScene_PlaySound123AndEnable(void)
{
    Func_02006ce6(123);
    Func_02006c74(1);
}

void FieldScene_SetFlag140AndFinishSequence(s32 arg0, s32 arg1)
{
    u8 *globalCtx;

    Func_020084b0(160 << 1);
    Func_02008638(141, 1);
    globalCtx = *(u8 **)0x03001f30;
    Func_0200864c(arg0, arg1);
    globalCtx[0x23] = 0;
    Func_02008666();
    Func_0200864c_a(1);
    Func_0200845a(1);
}

void FieldScene_FinishSequence(void)
{
    Func_02008584(0, 1);
    Func_02008672(2);
    Func_0200868e();
}

void FieldScene_SpawnEightShots(void)
{
    struct Descriptor descriptor;
    u8 *record;
    u32 i;

    record = Func_0200854c(8);
    descriptor.field0 = 1;
    descriptor.field24 = 0x0119;
    descriptor.field28 = 0x0200d1d8;
    descriptor.field16 = 224 << 10;
    descriptor.field20 = 192 << 9;
    for (i = 0; i <= 7; i++) {
        Func_02008540(10);
        if (i & 1) {
            Func_020086f6(0x82);
        }
        Func_020042d6(*(s32 *)(record + 8), *(s32 *)(record + 12),
                      *(s32 *)(record + 16) + 0xffe80000, 0,
                      0x9999, 0, 0x00360001, (u8 *)&descriptor);
    }
    Func_02008578(60);
}

void FieldScene_SelectActorModeFromInputBit(s32 arg0)
{
    if ((*(u32 *)0x03001e40 >> 1) & 1) {
        Func_0200866c(arg0, 10);
    } else {
        Func_02008674(arg0, 9);
    }
}

void FieldScene_RunParticleRain(void)
{
    struct Descriptor_020041ec descriptor;
    u8 *record;
    u32 i;
    s32 x;
    s32 y;
    s32 scale;

    Func_02008778(0x83);
    *(u32 *)(Func_02008606(8) + 108) = 0x0200c1c5;
    Func_020085e0(40);
    Func_0200872a(128 << 9, 0);
    Func_0200872a_a(0x205c54, 1);
    Func_02008740(60);
    Func_020085fe(40);
    Func_020087ac(0x83);
    *(u32 *)(Func_0200863a(2) + 108) = 0x0200c1c5;
    Func_02008612(120);
    record = Func_02008648(8);
    descriptor.field0 = 1;
    descriptor.field4 = 2;
    descriptor.field24 = 0x011d;
    for (i = 0; i <= 63; i++) {
        if ((i & 3) == 0) {
            Func_020087e2(246);
        }
        x = *(s32 *)(record + 8)
            + ((((u32)(Func_020085c6() * 3) << 4) >> 16) << 16)
            + 0xfff40000;
        y = *(s32 *)(record + 12)
            + ((((u32)Func_020085de() << 5) >> 16) << 16)
            + 0xfff00000;
        scale = (((u32)((u32)Func_020085f2() << 2) >> 16) << 15) + (128 << 8);
        Func_020043fe(x, y, *(s32 *)(record + 16), 0,
                      scale, 0, 152 << 13, (u8 *)&descriptor);
        Func_0200860e(2);
    }
    Func_02008850(220);
    Func_020086ae(30);
    Func_020087f0(128 << 9, 1);
    Func_02008806(60);
    Func_020086c4(40);
    *(u32 *)(Func_020086fa(8) + 108) = 0;
    *(u32 *)(Func_02008704(2) + 108) = 0;
    Func_02008798(8, 0);
    Func_020087a0(2, 0);
}

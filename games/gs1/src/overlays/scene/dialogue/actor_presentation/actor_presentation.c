#include "types.h"
#include "scene.h"
#include "configured_effect_spawn.h"
#include "configured_effect_spawn_body.inc"

/* overlays/scene/dialogue/actor_presentation/scene_effect.c */
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct EffectRecord {
    u8 pad[9];
    u8 flags_lo : 2;
    u8 mode : 2;
    u8 flags_hi : 4;
};

struct EffectWork {
    u8 pad[80];
    struct EffectRecord *record;
};

struct SceneActor {
    u8 reserved_00[6];
    s16 temporary_state;
    u8 reserved_08[92];
    u16 presentation_flags;
};

struct SceneActor {
    u8 reserved_00[100];
    u16 presentation_flags;
};

struct SceneActor2 { u8 reserved_00[100]; u16 presentation_flags; };

struct Presentation {
    u8 reserved_00[9];
    u8 flags;
};

struct SceneActor3 {
    u8 reserved_00[35];
    u8 state_23;
    u8 reserved_24[44];
    struct Presentation *presentation;
};

struct SceneActor4 {
    u8 reserved_00[35];
    u8 state_23;
    u8 reserved_24[44];
    struct Presentation *presentation;
};

struct SceneActor5 {
    u8 reserved_00[35];
    u8 state_23;
    u8 reserved_24[44];
    struct Presentation *presentation;
};

struct OverlayEffectMotion {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[28];
    s32 horizontal_rate;
    s32 vertical_rate;
    s32 shadow_x;
    s32 shadow_y;
    s32 shadow_z;
    u8 pad44[32];
    s16 mode;
};

extern u8 *gIw[];
extern u8 gVal[];

void *AcquireOverlayObject(s32, s32, s32, s32);

void *CreateOverlayObject(s32, s32, s32, s32);

u8 *Talk_unk6_4();

u8 *Talk_unk7_4();

u8 *Talk_unk8_4(s32);

u8 *Talk_unk9_4(int);

u8 *Talk_unk10_4(int);

struct SceneActor *Talk_unk11_4(s32);

struct SceneActor *Talk_unk12_4(s32);

struct SceneActor *Talk_unk13_4(s32);

struct SceneActor *Talk_unk14_4(s32);

struct SceneActor *Talk_unk15_4(s32);

struct SceneActor *Talk_unk16_4(s32);

struct SceneActor *Talk_unk17_4(s32);

struct SceneActor *Talk_unk18_4(s32);

u8 *Talk_unk19_4(int);

struct SceneActor *Talk_unk20_4(s32);

struct SceneActor *Talk_unk21_4(s32);

u8 *Talk_unk22_4(int);

typedef s32(*IwramSqrt02001638)(s32);

/*
 * Every Func_ symbol above names a loader-relocated call word, not a runtime
 * address. Declarations are old-style because an overlay import's arity is
 * not fixed per name, and each call site names the veneer it reaches rather
 * than the import behind it -- one import reached from several sites carries
 * several names, and naming the import instead makes fresh veneers.
 * Talk_unk23_4 is void because its result is discarded. Talk_unk8
 * returns s32 so that testing it emits no narrowing shift.
 */

/*
 * The wrapper helpers below pass their constants straight into the argument
 * registers. A direct call precomputes an expensive constant into a value the
 * compiler then shares with later uses in the same block.
 */

/*
 * A value-returning call sets r0 last of its arguments, so the callee must be
 * spelled as returning a value even where the result is unused.
 */

static __inline__ void SetScale(s32 actor, s32 horizontal, s32 vertical)
{
    extern u8 *gWork;

    Talk_unk13_3(actor, horizontal, vertical);
}

void Effect_SetEffectRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}

void *OvObj_PrepareSceneObject(s32 first, s32 second, s32 third, s32 fourth)
{
    void *obj;
    void *rec;
    s32 mask;

    obj = AcquireOverlayObject(fourth, first, second, third);
    if (obj != NULL) {
        rec = FIELD_AT_OFFSET(obj, void *, 0x50);
        mask = -0xD;
        FIELD_AT_OFFSET(rec, u8, 9) = (u8)(mask & FIELD_AT_OFFSET(rec, u8, 9));
        FIELD_AT_OFFSET(obj, u8, 0x55) = 0;
        FIELD_AT_OFFSET(obj, u8, 0x59) = 8;
        RunOverlayObjectCommand0(obj, 0);
        RunOverlayObjectCommand14(obj, 0xE);
        RunOverlayObjectCommand1(obj, 1);
        return obj;
    }
    return NULL;
}

void *OvObj_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *result = CreateOverlayObject(arg3, arg0, arg1, arg2);

    if (result != NULL) {
        u8 *object = *(u8 **)(result + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = object[9];
        mask = -mask;
        mask &= flags;
        mask |= 4;
        object[9] = mask;
        result[0x55] = 0;
        result[0x59] = 8;
        SetOverlayObjectMode(result, 0);
        SetOverlayObjectSlot(result, 15);
        result[0x23] = (result[0x23] & 0xfe) | 2;
        return result;
    }
    return NULL;
}

void Effect_SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
}

/*
 * Per-frame callback for one actor record, installed into field +0x6c of
 * actors 14 and 15 by the overlay initialiser. Always returns 0. The pointer
 * table at 0x03001e8c holds the scene record at entry 0 and the overlay
 * workspace at entry 12; entry 12 is the same pointer the rest of this
 * overlay loads as gWork, and is modelled here as one global rather
 * than two.
 */
s32 Actor_UpdateProximityToLeader(u8 *self)
{
    u8 **globals = (u8 **)0x03001e8c;
    u8 *scene = globals[0];
    u8 *workspace = globals[12];        /* == *(u8 **)0x03001ebc */
    u16 *flags = (u16 *)(self + 100);
    s32 force = 0;
    s32 range = 18;
    u8 *partner;
    u8 *player;

    /*
     * Bit 0 of the actor's own flag halfword selects which partner to test.
     * This must stay two calls rather than one call on a conditional
     * expression: the conditional form folds to arithmetic on the flag.
     */
    if ((*flags & 1) != 0) {
        partner = Talk_unk6_4(15);
    } else {
        partner = Talk_unk6_4(14);
    }
    if (Talk_unk8(self, partner, 32, 0) != 0) {
        return 0;
    }

    player = Talk_unk7_4(0);

    /*
     * Widen the range when the scene counter at workspace + 376 is already
     * running, or when the scene byte at scene + 0x0ea4 is set.
     */
    if (*(s16 *)(workspace + 376) != 0 || scene[0x0ea4] != 0) {
        range = 26;
        if ((*flags & 2) != 0) {
            force = 1;
        }
    }

    Talk_unk23_4(self, player, range, force);
    return 0;
}

s32 ActorDraw_UpdateEntityFromLeader(u8 *entity)
{
    u8 *base = gIw[0];
    u8 *workspace = gIw[12];
    s32 flag = 0;
    s32 selector = 18;
    u8 *leader;

    if (*(s32 *)(entity + 56) == (s32)0x80000000)
        return 0;

    leader = Talk_unk8_4(0);
    if (*(s16 *)(workspace + 376) != 0 || base[0x0ea4] != 0) {
        selector = 26;
        flag = 1;
    }
    Talk_SetMode(entity, leader, selector, flag);
    return 0;
}

void *SceneData_GetScriptTable(void)
{
    return (void *)0x02009f5c;
}

int SceneData_ReturnZero(void)
{
    return 0;
}

void *SceneData_GetMessageTable(void)
{
    return (void *)0x0200a094;
}

void *SceneData_SelectActorTableByFlag855(void)
{
    if (Talk_unk9(0x855) != 0)
        return (void *)0x0200a27c;
    return (void *)0x0200a0cc;
}

int OvObj_GetObject2Byte280(void)
{
    return Talk_unk9_4(2)[280];
}

/*
 * The flagged path passes the result of its final call back to the caller, so
 * this is spelled as a tail call and the return type is s32. The
 * fall-through path returns nothing.
 */
s32 OvObj_RunObject2WhenFlagged(void)
{
    Talk_unk24_4();
    if ((*(u32 *)(Talk_unk10_4(2) + 248) & 1) != 0) {
        Talk_SetMode2(2, 0, 0, 0);
        Talk_unk6_2(126);
        Talk_unk7_2(0);
        return Talk_unk10(2);
    }
}

void State_SetFlags947And29dc(void)
{
    Talk_unk25_4();
    Talk_Apply(0x947, 1);
    Talk_Apply2(0x29dc, 1);
    Talk_unk26_4();
}

void *SceneData_SelectTableA414ByFlag855(void)
{
    if (Talk_unk8_2(0x855) != 0)
        return (void *)0x0200a630;
    return (void *)0x0200a414;
}

void Dialogue_RunActor9LineAndAdvance(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Talk_unk27_4();
    Talk_Do(0x1223);
    Talk_unk28_4(9, 0, 2);
    Talk_Check(9, 0);
    if (Talk_unk2(0, 0) != 0) {
        bump_step(1);
    }
    Talk_unk29_4(9, 0);
    Talk_Place(9, 0x5000, 0);
    Talk_unk30_4();
}

void ActorDraw_RunActorThirteenSceneSetup(void)
{
    u8 *workspace;

    Talk_unk31_4();
    Talk_unk9_2(0x1229);
    Talk_Apply3(13, 1);
    Talk_unk14_3(13, 0, 2);
    Talk_Apply4(13, 0);
    if (Talk_Apply5(0, 0) != 0) {
        workspace = *(u8 **)0x03001ebc;
        ++*(u16 *)(workspace + 472);
    }
    Talk_Apply6(13, 0);
    Talk_unk32_4();
}

void ActorDraw_RunActorSeventeenSceneSetup(void)
{
    int Talk_unk33_4(int, int);

    u8 *workspace;

    Talk_unk34_4();
    Talk_unk10_2(0x122f);
    Talk_unk15_3(17, 0, 2);
    Talk_Apply7(17, 0);
    if (Talk_unk33_4(0, 0) != 0) {
        workspace = *(u8 **)0x03001ebc;
        ++*(u16 *)(workspace + 472);
    }
    Talk_Apply8(17, 0);
    Talk_unk35_4();
}

void ActorDraw_RunActorEighteenSceneSetup(void)
{
    u8 *workspace;

    Talk_unk36_4();
    Talk_unk11_2(0x1232);
    Talk_unk16_3(18, 0, 2);
    Talk_Apply9(18, 0);
    if (Talk_Apply10(0, 0) != 0) {
        workspace = *(u8 **)0x03001ebc;
        ++*(u16 *)(workspace + 472);
    }
    Talk_Apply11(18, 0);
    Talk_unk37_4();
}

void Dialogue_RunActor11Line(void) { Talk_unk38_4(); Talk_unk12_2(0x1227); Talk_unk13_2(11); Talk_unk39_4(); }

void Dialogue_RunActor16Line(void) { Talk_unk40_4(); Talk_unk14_2(0x122e); Talk_unk15_2(16); Talk_unk41_4(); }

void Dialogue_RunActor19Line(void)
{
    void Talk_unk42_4(int, int);

    Talk_unk43_4(); Talk_unk16_2(0x1235); Talk_Apply12(19, 0);
    Talk_unk17_3(19, 0, 2); Talk_unk42_4(19, 0); Talk_unk44_4();
}

void ActorDraw_RunActorFourteenDialogue(void)
{
    void Talk_unk45_4(s32, s32);
    void Talk_unk46_4(s32, s32);

    struct SceneActor *actor = Talk_unk11_4(14);
    s16 saved = actor->temporary_state;

    actor->presentation_flags |= 2;
    Talk_unk47_4();
    Talk_unk17_2(0x122c);
    Talk_unk46_4(14, 0);
    Talk_unk18_3(14, 0, 2);
    Talk_unk45_4(14, 10);
    actor->temporary_state = saved;
    Talk_unk18_2(1);
    Talk_unk48_4();
    actor->presentation_flags &= 1;
}

void ActorDraw_RunActorFifteenDialogue(void)
{
    struct SceneActor *actor = Talk_unk12_4(15);
    s16 saved = actor->temporary_state;

    actor->presentation_flags |= 2;
    Talk_unk49_4();
    Talk_unk19_2(0x122d);
    Talk_Apply13(15, 0);
    Talk_unk19_3(15, 0, 2);
    Talk_Apply14(15, 10);
    actor->temporary_state = saved;
    Talk_unk20_2(1);
    Talk_unk50_4();
    actor->presentation_flags &= 1;
}

void Scene_RunActor21Sequence(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Talk_unk51_4();
    Talk_unk52_4((s32)gVal);
    Talk_unk53_4(21, 0, 2);
    Talk_unk2_3(21, 0x103, 0);
    Talk_unk54_4(30);
    Talk_unk3(21, 0);
    Talk_unk55_4();
}

void Actor_RunActorStep(int actor)
{
    void Talk_unk56_4(int, int);

    Talk_unk57_4(); Talk_Apply15(actor, 1); Talk_unk20_3(actor, 0, 2);
    Talk_unk56_4(actor, 0); Talk_unk58_4();
}

void Actor_RunActorCommandWithFlag91(s32 x)
{
    void Talk_unk59_4(s32, s32);

    u8 *flag = (u8 *)Talk_unk11() + 91;
    s32 zero = 0;

    *flag = 1;
    Talk_unk60_4();
    Talk_Apply16(x, 1);
    Talk_unk21_2(2);
    Talk_unk59_4(x, 0);
    Talk_unk61_4();
    *flag = zero;
}

void ActorDraw_RunActorEightSceneSetup(void)
{
    void Talk_unk62_4(int, int, int);

    u8 *workspace;
    Talk_unk63_4(); Talk_unk22_2(0x1330); Talk_unk62_4(8, 0, 2); Talk_Apply17(8, 0);
    if (Talk_Apply18(0, 0) != 0) { workspace = *(u8 **)0x03001ebc; ++*(u16 *)(workspace + 472); }
    Talk_Apply19(8, 0); Talk_unk64_4();
}

void Dialogue_RunActor11SecondLine(void) { Talk_unk65_4(); Talk_unk23_2(0x1335); Talk_unk24_2(11); Talk_unk66_4(); }

void Dialogue_RunActor12LineAndAdvance(void)
{
    u8 *workspace;
    Talk_unk67_4(); Talk_unk25_2(0x1336);
    if (Talk_unk26_2(2) != 0) { workspace = *(u8 **)0x03001ebc; ++*(u16 *)(workspace + 472); }
    Talk_unk27_2(12); Talk_unk68_4();
}

void Dialogue_RunActor13Line(void) { Talk_unk69_4(); Talk_unk28_2(0x1338); Talk_unk29_2(13); Talk_unk70_4(); }

void ActorDraw_RunActorFourteenDialogueAndAdvanceStory(void)
{
    extern u8 *gWork;
    void Talk_unk71_4(s32);

    struct SceneActor *actor = Talk_unk13_4(14);
    u16 *flags = &actor->presentation_flags;
    s16 saved = actor->temporary_state;
    /* tmp keeps the flag result live in a register; do not fold it away. */
    s32 tmp;

    *flags = (tmp = *flags | 2);
    Talk_unk72_4();
    Talk_unk73_4(0x1339);
    if (Talk_unk12(2) != 0)
        ++*(u16 *)(gWork + 472);
    Talk_unk74_4(14, 0);
    Talk_unk21_3(14, 0, 2);
    Talk_Apply20(14, 10);
    actor->temporary_state = saved;
    Talk_unk71_4(1);
    Talk_unk75_4();
    *flags &= 1;
}

void ActorDraw_RunActorFifteenFollowupDialogue(void)
{
    struct SceneActor *actor = Talk_unk14_4(15);
    s16 saved = actor->temporary_state;

    actor->presentation_flags |= 2;
    Talk_unk76_4();
    Talk_unk30_2(0x133b);
    Talk_Apply21(15, 0);
    Talk_unk22_3(15, 0, 2);
    Talk_Apply22(15, 10);
    actor->temporary_state = saved;
    Talk_unk31_2(1);
    Talk_unk77_3();
    actor->presentation_flags &= 1;
}

void ActorDraw_RunActorSixteenSceneSetup(void)
{
    u8 *workspace;
    Talk_unk78_3(); Talk_unk32_2(0x133c); Talk_Apply23(16, 1); Talk_unk23_3(16, 0, 2); Talk_Apply24(16, 0);
    if (Talk_Apply25(0, 0) != 0) { workspace = *(u8 **)0x03001ebc; ++*(u16 *)(workspace + 472); }
    Talk_Apply26(16, 0); Talk_unk79_3();
}

void ActorDraw_RunActorEighteenFollowupSceneSetup(void)
{
    void Talk_unk80_3(int, int);

    u8 *workspace;
    Talk_unk81_3(); Talk_unk33_2(0x133f); Talk_unk24_3(18, 0, 2); Talk_Apply27(18, 0);
    if (Talk_Apply28(0, 0) != 0) { workspace = *(u8 **)0x03001ebc; ++*(u16 *)(workspace + 472); }
    Talk_unk80_3(18, 0); Talk_unk82_3();
}

void ActorDraw_RunActorNineteenDialogueAndSetSceneState(void)
{
    extern u8 *gWork;
    void Talk_unk83_3(s32, s32);
    void Talk_unk84_3(s32, s32);

    Talk_unk85_3();
    Talk_unk34_2(0x1342);
    Talk_unk83_3(19, 0);
    Talk_unk25_3(19, 0, 2);
    Talk_Apply29(19, 0);
    Talk_unk84_3(19, 1);
    if (Talk_unk13(231) != -1 && Talk_unk14(0x858) == 0) {
        u16 *p = (u16 *)(gWork + 370);
        u16 value = 1;

        *p = value;
    }
    Talk_unk86_3();
}

void Dialogue_RunActor20Line(void)
{
    void Talk_unk87_3(int, int, int);

    Talk_unk88_3(); Talk_unk35_2(0x137f); Talk_unk87_3(20, 0, 2); Talk_Apply30(20, 3);
    Talk_unk36_2(20); Talk_Apply31(20, 0); Talk_unk89_3();
}

void Dialogue_RunActor11FlaggedLine(void)
{
    int Talk_unk90_3(int);

    Talk_unk91_3();
    if (Talk_unk90_3(0x855) == 0) Talk_unk37_2(0x1239); else Talk_unk38_2(0x1346);
    Talk_unk39_2(11); Talk_unk92_3();
}

void Dialogue_RunActor13FlaggedLine(void)
{
    BeginSceneStep();
    if (SceneFlagIsClear(0x855) == 0) {
        PlaySceneCueA(0x123b);
    } else {
        PlaySceneCueB(0x1348);
    }
    SetSceneActor(13);
    EndSceneStep();
}

void ActorDraw_RunActorFourteenFlaggedDialogue(void)
{
    extern u8 *gWork;

    Talk_unk15_4(14)->presentation_flags |= 2;
    Talk_unk93_3();
    if (Talk_unk15(0x855) == 0) {
        Talk_unk40_2(0x123c);
    } else {
        Talk_unk41_2(0x1349);
        if (Talk_unk16(2) != 0)
            ++*(u16 *)(gWork + 472);
    }
    Talk_unk42_2(14);
    Talk_unk94_3();
    Talk_unk16_4(14)->presentation_flags &= 1;
}

void ActorDraw_RunActorFifteenScriptBranch(void)
{
    void Talk_unk95_3(s32);

    Talk_unk17_4(15)->presentation_flags |= 2;
    Talk_unk96_3();
    if (Talk_unk17(0x855) == 0)
        Talk_unk95_3(0x123d);
    else
        Talk_unk43_2(0x134b);
    Talk_unk44_2(15);
    Talk_unk97_3();
    Talk_unk18_4(15)->presentation_flags &= 1;
}

void ActorDraw_RunActorSixteenScriptBranch(void)
{
    void Talk_unk98_3(int);

    Talk_unk99_3();
    if (Talk_unk45_2(0x855) == 0) Talk_unk46_2(0x123e); else Talk_unk98_3(0x134c);
    Talk_unk47_2(16); Talk_unk100_3();
}

void ActorDraw_RunActorNineteenScriptBranch(void)
{
    void Talk_unk101_3(int);

    u8 *actor = Talk_unk19_4(19); actor[91] = 1; Talk_unk102_3();
    if (Talk_unk103_3(0x855) == 0) {
        Talk_unk104_3(0x1241); Talk_Apply32(19, 0); Talk_unk101_3(2);
    } else if (Talk_unk48_2(0x858) != 0) {
        Talk_unk105_3(0x13ab);
    } else {
        Talk_unk49_2(0x134e);
    }
    Talk_Apply33(19, 0); Talk_unk106_3(); actor[91] = 0;
}

void Dialogue_RunActor21Line(void) { Talk_unk107_3(); Talk_unk50_2(0x12c1); Talk_unk51_2(21); Talk_unk108_3(); }

void State_Apply200ThenPlace55_26(void)
{
    Talk_unk52_2(0x200);
    {
        int v1 = 23;
        int v2 = 26;
        Talk_SetRect(55, 26, 4, 2, v1, v2);
    }
}

void State_Apply200ThenPlace23_23(void)
{
    Talk_unk53_2(0x200);
    {
        int v1 = 23;
        int v2 = 26;
        Talk_unk2_5(23, 23, 4, 2, v1, v2);
    }
}

void Actor_PlaceActor0AndSetSceneDelay(s32 x, s32 y, s32 continuation)
{
    extern u8 *gWork;

    SetScale(0, 0x8000, 0x4000);
    Talk_unk26_3(0, x, y);
    *(s32 *)(gWork + 456) = 16;
    Talk_unk54_2(continuation);
}

void Scene_SetupScene5At408_320(void)
{
    Talk_unk55_2(158); Talk_unk27_3((void *)0x0200a828, 56, 19); Talk_unk28_3(408, 320, 5);
}

void Scene_SetupScene6At312_304(void)
{ Talk_unk56_2(158); Talk_unk29_3((void *)0x0200a83e, 50, 18); Talk_unk30_3(312, 304, 6); }

void Scene_SetupScene7At216_288(void)
{ Talk_unk57_2(158); Talk_unk31_3((void *)0x0200a854, 44, 17); Talk_unk32_3(216, 288, 7); }

void ActorDraw_SetupActorZeroForSceneEightAt376_224(void)
{
    struct SceneActor3 *actor = Talk_unk20_4(0);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Talk_unk58_2(158);
    Talk_unk33_3((const void *)0x0200a86a, 54, 13);
    {
        s32 cell = 23;
        s32 row = 12;

        Talk_unk3_5(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    Talk_unk34_3(376, 224, 8);
}

void ActorDraw_SetupActorZeroForSceneNineAt296_176(void)
{
    void Talk_unk109_3(s32, s32, s32);

    struct SceneActor4 *actor = Talk_unk21_4(0);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Talk_unk59_2(158);
    Talk_unk35_3((const void *)0x0200a880, 49, 10);
    {
        s32 cell = 18;
        s32 row = 10;

        Talk_unk4_5(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    Talk_unk109_3(296, 176, 9);
}

void Scene_SetupScene10At120_144(void)
{ Talk_unk60_2(158); Talk_unk36_3((void *)0x0200a896, 38, 6); Talk_unk37_3(120, 144, 10); }

void ActorDraw_SetupActorZeroForSceneTwelveAt72_160(void)
{
    struct SceneActor *Talk_unk110_3(s32);

    struct SceneActor5 *actor = Talk_unk110_3(0);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Talk_unk61_2(158);
    Talk_unk38_3((const void *)0x0200a8ac, 35, 9);
    {
        s32 cell = 4;
        s32 row = 10;

        Talk_unk5_5(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    Talk_unk39_3(72, 160, 12);
}

void Scene_SetupScene13At152_264(void) { Talk_unk62_2(123); Talk_unk40_3(152, 264, 13); }

void ActorDraw_MoveActorToPositionAndWait(int actor, int x, int z, int field40)
{
    void Talk_unk111_3(int);
    void Talk_unk112_3(int, int, int);

    u8 *record = Talk_unk22_4(actor); int frames;
    Talk_unk41_3(actor, 0x30000, 0x18000); *(s32 *)(record + 72) = 0x8000;
    *(s32 *)(record + 68) = 0; *(s32 *)(record + 40) = field40; Talk_Apply34(record, 0);
    Talk_unk42_3(actor, x, z); Talk_unk112_3(actor, x << 16, z << 16);
    for (frames = 60; frames != 0; --frames) { Talk_unk111_3(1); if (*(s16 *)(record + 42) == 0) break; }
    Talk_Apply35(record, 1); *(s32 *)(record + 72) = 0x10000;
}

void Scene_RunActor23SequenceOnceByFlag867(void)
{
    extern u8 gWork[];
    void Talk_unk113_3();

    u32 i;
    s32 record;

    Talk_unk114_3();
    Talk_unk115_3(100);
    Talk_unk116_3(40);
    if (Talk_unk4(0x867) == 0) {
        Talk_Run(23, 0x102);
        Talk_unk117_3(23, 4, 0);
        Talk_unk113_3(12);
        Talk_unk118_3(23, 4, 0);
        Talk_unk119_3(20);
        Talk_unk2_4(23, 0x188, 104, 0x70000);
        Talk_unk120_3(20);
        Talk_unk3_3(23, 0x198, 104);
        Talk_unk4_3(23, 0x198, 120);
        Talk_unk2_2(0x867);
    }
    Talk_unk121_3();
}

void Scene_RunActor19MotionSequence(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Talk_unk122_3(231);
    Talk_unk123_3();
    Talk_unk124_3(10);
    Talk_unk125_3(19, 2);
    Talk_unk5_3(19, 0xcccc, 0x6666);
    Talk_unk6_3(19, 216, 0x198);
    Talk_unk126_3(10);
    Talk_unk7_3(19, 0x4000, 20);
    Talk_unk127_3(19, 6, 0);
    Talk_unk128_3(30);
    Talk_unk129_3(19, 6, 0);
    Talk_unk130_3(30);
    Talk_unk131_3(19, 6, 0);
    Talk_unk132_3(30);
    Talk_unk8_3(19, 216, 0x188);
    Talk_unk133_3(10);
    Talk_unk9_3(19, 0x4000, 20);
    Talk_unk3_2(0x858);
    Talk_unk134_3();
}

s32 Actor_GetPositionDistance(s32 *a, s32 *b)
{
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 dz = (*a - *b) >> 16;
    s32 dxsq = dx *dx;
    s32 dysq = dy *dy;
    s32 dzsq = dz *dz;

    return ((IwramSqrt02001638) 0x030001D8)(dxsq + dysq + dzsq);
}

void Actor_ApplyActorZeroThenWait(s32 actor, s32 delay)
{
    Talk_Apply36(actor, 0);
    Talk_unk63_2(delay);
}

void Actor_ApplyActorCueThenWait(s32 actor, s32 cue, s32 delay)
{
    Talk_unk43_3(actor, cue, 0);
    Talk_unk64_2(delay);
}

/*
 * Per-frame step for a projectile. Advance x by its rate and mirror it into
 * the shadow copy, then either follow the vertical rate or fall at a fixed
 * rate depending on the mode word, and finally decay both rates.
 */
void Effect_UpdateMotionWithDamping(struct OverlayEffectMotion *effect)
{
    s32 horizontal_rate;
    s32 vertical_rate;

    effect->x += effect->horizontal_rate;
    effect->shadow_x = effect->x;

    if (effect->mode != 0) {
        effect->y += effect->vertical_rate;
        effect->shadow_y = effect->y;
    } else {
        effect->z += effect->vertical_rate;
        effect->shadow_z = effect->z;
        effect->y += 1024;
        effect->shadow_y = effect->y;
    }

    horizontal_rate = effect->horizontal_rate;
    effect->horizontal_rate = horizontal_rate - EffectMotion_GetHorizontalDecay(horizontal_rate, 28);
    vertical_rate = effect->vertical_rate;
    effect->vertical_rate = vertical_rate - EffectMotion_GetVerticalDecay(vertical_rate, 28);
}

/* overlays/scene/dialogue/actor_presentation/scene_primary_script.c */
extern u8 gWork[];

void Scene_RunScene382(void)
{
    u32 i;
    s32 record;
    u8 *p5;

    p5 = *(volatile s32 *)gWork;
    if (Talk_unk5(0x855) == 0) {
        record = Talk_unk6(0x856);
        if (record != 0) {
            goto L_020004cc;
        }
    }
    Talk_unk135_3((*(s16 *)(((s32)p5 + 0x16c)) - 19));
    goto L_0200059e;
    L_020004cc:;
    Talk_unk136_3();
    record = Talk_unk7(0);
    if (record != 0) {
        Talk_unk137_3(2, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Talk_unk10_3(2, 0xcccc, 0x6666);
    if (*(s16 *)(((s32)p5 + 0x16c)) == 20) {
        Talk_unk11_3(2, 0x190, 0x1c0);
    } else {
        Talk_unk3_4(0xcccc, 0x1999);
        Talk_unk4_4(0xe00000, -1, 0xa20000, 1);
        Talk_unk138_3(2, 224, 162);
        Talk_unk139_3();
    }
    Talk_unk140_3(0, 2, 0);
    Talk_unk141_3(20);
    Talk_unk4_2(0x1327);
    Talk_unk12_3(0x9002, 0, 20);
    Talk_unk142_3(0, 3);
    if (Talk_unk5_4()!= 0) {
        Talk_unk5_2(0x132a);
        Talk_unk143_3(2, 0);
        Talk_unk144_3();
        Talk_unk145_3(20);
    }
    Talk_unk146_3(2);
    Talk_unk147_3((*(s16 *)(((s32)p5 + 0x16c)) - 19));
    Talk_unk148_3();
    Talk_unk149_3();
    Talk_unk150_3();
    L_0200059e:;
}

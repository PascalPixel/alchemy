#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/dialogue/actor_presentation/actor_presentation.h"
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

u8 *Talk_Run6();

u8 *Talk_Run7();

u8 *Talk_Run8(s32);

u8 *Talk_Run9(int);

u8 *Talk_Run10(int);

struct SceneActor *Talk_Run11(s32);

struct SceneActor *Talk_Run12(s32);

struct SceneActor *Talk_Run13(s32);

struct SceneActor *Talk_Run14(s32);

struct SceneActor *Talk_Run15(s32);

struct SceneActor *Talk_Run16(s32);

struct SceneActor *Talk_Run17(s32);

struct SceneActor *Talk_Run18(s32);

u8 *Talk_Run19(int);

struct SceneActor *Talk_Run20(s32);

struct SceneActor *Talk_Run21(s32);

u8 *Talk_Run22(int);

typedef s32(*IwramSqrt02001638)(s32);

/*
 * Every Func_ symbol above names a loader-relocated call word, not a runtime
 * address. Declarations are old-style because an overlay import's arity is
 * not fixed per name, and each call site names the veneer it reaches rather
 * than the import behind it -- one import reached from several sites carries
 * several names, and naming the import instead makes fresh veneers.
 * Talk_Run23 is void because its result is discarded. Talk_Check8
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

    Talk_Place13(actor, horizontal, vertical);
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
        partner = Talk_Run6(15);
    } else {
        partner = Talk_Run6(14);
    }
    if (Talk_Check8(self, partner, 32, 0) != 0) {
        return 0;
    }

    player = Talk_Run7(0);

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

    Talk_Run23(self, player, range, force);
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

    leader = Talk_Run8(0);
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
    if (Talk_Check9(0x855) != 0)
        return (void *)0x0200a27c;
    return (void *)0x0200a0cc;
}

int OvObj_GetObject2Byte280(void)
{
    return Talk_Run9(2)[280];
}

/*
 * The flagged path passes the result of its final call back to the caller, so
 * this is spelled as a tail call and the return type is s32. The
 * fall-through path returns nothing.
 */
s32 OvObj_RunObject2WhenFlagged(void)
{
    Talk_Run24();
    if ((*(u32 *)(Talk_Run10(2) + 248) & 1) != 0) {
        Talk_SetMode2(2, 0, 0, 0);
        Talk_Do6(126);
        Talk_Do7(0);
        return Talk_Check10(2);
    }
}

void State_SetFlags947And29dc(void)
{
    Talk_Run25();
    Talk_Apply(0x947, 1);
    Talk_Apply2(0x29dc, 1);
    Talk_Run26();
}

void *SceneData_SelectTableA414ByFlag855(void)
{
    if (Talk_Do8(0x855) != 0)
        return (void *)0x0200a630;
    return (void *)0x0200a414;
}

void Dialogue_RunActor9LineAndAdvance(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Talk_Run27();
    Talk_Do(0x1223);
    Talk_Run28(9, 0, 2);
    Talk_Check(9, 0);
    if (Talk_Check2(0, 0) != 0) {
        bump_step(1);
    }
    Talk_Run29(9, 0);
    Talk_Place(9, 0x5000, 0);
    Talk_Run30();
}

void ActorDraw_RunActorThirteenSceneSetup(void)
{
    u8 *workspace;

    Talk_Run31();
    Talk_Do9(0x1229);
    Talk_Apply3(13, 1);
    Talk_Place14(13, 0, 2);
    Talk_Apply4(13, 0);
    if (Talk_Apply5(0, 0) != 0) {
        workspace = *(u8 **)0x03001ebc;
        ++*(u16 *)(workspace + 472);
    }
    Talk_Apply6(13, 0);
    Talk_Run32();
}

void ActorDraw_RunActorSeventeenSceneSetup(void)
{
    int Talk_Run33(int, int);

    u8 *workspace;

    Talk_Run34();
    Talk_Do10(0x122f);
    Talk_Place15(17, 0, 2);
    Talk_Apply7(17, 0);
    if (Talk_Run33(0, 0) != 0) {
        workspace = *(u8 **)0x03001ebc;
        ++*(u16 *)(workspace + 472);
    }
    Talk_Apply8(17, 0);
    Talk_Run35();
}

void ActorDraw_RunActorEighteenSceneSetup(void)
{
    u8 *workspace;

    Talk_Run36();
    Talk_Do11(0x1232);
    Talk_Place16(18, 0, 2);
    Talk_Apply9(18, 0);
    if (Talk_Apply10(0, 0) != 0) {
        workspace = *(u8 **)0x03001ebc;
        ++*(u16 *)(workspace + 472);
    }
    Talk_Apply11(18, 0);
    Talk_Run37();
}

void Dialogue_RunActor11Line(void) { Talk_Run38(); Talk_Do12(0x1227); Talk_Do13(11); Talk_Run39(); }

void Dialogue_RunActor16Line(void) { Talk_Run40(); Talk_Do14(0x122e); Talk_Do15(16); Talk_Run41(); }

void Dialogue_RunActor19Line(void)
{
    void Talk_Run42(int, int);

    Talk_Run43(); Talk_Do16(0x1235); Talk_Apply12(19, 0);
    Talk_Place17(19, 0, 2); Talk_Run42(19, 0); Talk_Run44();
}

void ActorDraw_RunActorFourteenDialogue(void)
{
    void Talk_Run45(s32, s32);
    void Talk_Run46(s32, s32);

    struct SceneActor *actor = Talk_Run11(14);
    s16 saved = actor->temporary_state;

    actor->presentation_flags |= 2;
    Talk_Run47();
    Talk_Do17(0x122c);
    Talk_Run46(14, 0);
    Talk_Place18(14, 0, 2);
    Talk_Run45(14, 10);
    actor->temporary_state = saved;
    Talk_Do18(1);
    Talk_Run48();
    actor->presentation_flags &= 1;
}

void ActorDraw_RunActorFifteenDialogue(void)
{
    struct SceneActor *actor = Talk_Run12(15);
    s16 saved = actor->temporary_state;

    actor->presentation_flags |= 2;
    Talk_Run49();
    Talk_Do19(0x122d);
    Talk_Apply13(15, 0);
    Talk_Place19(15, 0, 2);
    Talk_Apply14(15, 10);
    actor->temporary_state = saved;
    Talk_Do20(1);
    Talk_Run50();
    actor->presentation_flags &= 1;
}

void Scene_RunActor21Sequence(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Talk_Run51();
    Talk_Run52((s32)gVal);
    Talk_Run53(21, 0, 2);
    Talk_Place2(21, 0x103, 0);
    Talk_Run54(30);
    Talk_Check3(21, 0);
    Talk_Run55();
}

void Actor_RunActorStep(int actor)
{
    void Talk_Run56(int, int);

    Talk_Run57(); Talk_Apply15(actor, 1); Talk_Place20(actor, 0, 2);
    Talk_Run56(actor, 0); Talk_Run58();
}

void Actor_RunActorCommandWithFlag91(s32 x)
{
    void Talk_Run59(s32, s32);

    u8 *flag = (u8 *)Talk_Check11() + 91;
    s32 zero = 0;

    *flag = 1;
    Talk_Run60();
    Talk_Apply16(x, 1);
    Talk_Do21(2);
    Talk_Run59(x, 0);
    Talk_Run61();
    *flag = zero;
}

void ActorDraw_RunActorEightSceneSetup(void)
{
    void Talk_Run62(int, int, int);

    u8 *workspace;
    Talk_Run63(); Talk_Do22(0x1330); Talk_Run62(8, 0, 2); Talk_Apply17(8, 0);
    if (Talk_Apply18(0, 0) != 0) { workspace = *(u8 **)0x03001ebc; ++*(u16 *)(workspace + 472); }
    Talk_Apply19(8, 0); Talk_Run64();
}

void Dialogue_RunActor11SecondLine(void) { Talk_Run65(); Talk_Do23(0x1335); Talk_Do24(11); Talk_Run66(); }

void Dialogue_RunActor12LineAndAdvance(void)
{
    u8 *workspace;
    Talk_Run67(); Talk_Do25(0x1336);
    if (Talk_Do26(2) != 0) { workspace = *(u8 **)0x03001ebc; ++*(u16 *)(workspace + 472); }
    Talk_Do27(12); Talk_Run68();
}

void Dialogue_RunActor13Line(void) { Talk_Run69(); Talk_Do28(0x1338); Talk_Do29(13); Talk_Run70(); }

void ActorDraw_RunActorFourteenDialogueAndAdvanceStory(void)
{
    extern u8 *gWork;
    void Talk_Run71(s32);

    struct SceneActor *actor = Talk_Run13(14);
    u16 *flags = &actor->presentation_flags;
    s16 saved = actor->temporary_state;
    /* tmp keeps the flag result live in a register; do not fold it away. */
    s32 tmp;

    *flags = (tmp = *flags | 2);
    Talk_Run72();
    Talk_Run73(0x1339);
    if (Talk_Check12(2) != 0)
        ++*(u16 *)(gWork + 472);
    Talk_Run74(14, 0);
    Talk_Place21(14, 0, 2);
    Talk_Apply20(14, 10);
    actor->temporary_state = saved;
    Talk_Run71(1);
    Talk_Run75();
    *flags &= 1;
}

void ActorDraw_RunActorFifteenFollowupDialogue(void)
{
    struct SceneActor *actor = Talk_Run14(15);
    s16 saved = actor->temporary_state;

    actor->presentation_flags |= 2;
    Talk_Run76();
    Talk_Do30(0x133b);
    Talk_Apply21(15, 0);
    Talk_Place22(15, 0, 2);
    Talk_Apply22(15, 10);
    actor->temporary_state = saved;
    Talk_Do31(1);
    Talk_Run77();
    actor->presentation_flags &= 1;
}

void ActorDraw_RunActorSixteenSceneSetup(void)
{
    u8 *workspace;
    Talk_Run78(); Talk_Do32(0x133c); Talk_Apply23(16, 1); Talk_Place23(16, 0, 2); Talk_Apply24(16, 0);
    if (Talk_Apply25(0, 0) != 0) { workspace = *(u8 **)0x03001ebc; ++*(u16 *)(workspace + 472); }
    Talk_Apply26(16, 0); Talk_Run79();
}

void ActorDraw_RunActorEighteenFollowupSceneSetup(void)
{
    void Talk_Run80(int, int);

    u8 *workspace;
    Talk_Run81(); Talk_Do33(0x133f); Talk_Place24(18, 0, 2); Talk_Apply27(18, 0);
    if (Talk_Apply28(0, 0) != 0) { workspace = *(u8 **)0x03001ebc; ++*(u16 *)(workspace + 472); }
    Talk_Run80(18, 0); Talk_Run82();
}

void ActorDraw_RunActorNineteenDialogueAndSetSceneState(void)
{
    extern u8 *gWork;
    void Talk_Run83(s32, s32);
    void Talk_Run84(s32, s32);

    Talk_Run85();
    Talk_Do34(0x1342);
    Talk_Run83(19, 0);
    Talk_Place25(19, 0, 2);
    Talk_Apply29(19, 0);
    Talk_Run84(19, 1);
    if (Talk_Check13(231) != -1 && Talk_Check14(0x858) == 0) {
        u16 *p = (u16 *)(gWork + 370);
        u16 value = 1;

        *p = value;
    }
    Talk_Run86();
}

void Dialogue_RunActor20Line(void)
{
    void Talk_Run87(int, int, int);

    Talk_Run88(); Talk_Do35(0x137f); Talk_Run87(20, 0, 2); Talk_Apply30(20, 3);
    Talk_Do36(20); Talk_Apply31(20, 0); Talk_Run89();
}

void Dialogue_RunActor11FlaggedLine(void)
{
    int Talk_Run90(int);

    Talk_Run91();
    if (Talk_Run90(0x855) == 0) Talk_Do37(0x1239); else Talk_Do38(0x1346);
    Talk_Do39(11); Talk_Run92();
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

    Talk_Run15(14)->presentation_flags |= 2;
    Talk_Run93();
    if (Talk_Check15(0x855) == 0) {
        Talk_Do40(0x123c);
    } else {
        Talk_Do41(0x1349);
        if (Talk_Check16(2) != 0)
            ++*(u16 *)(gWork + 472);
    }
    Talk_Do42(14);
    Talk_Run94();
    Talk_Run16(14)->presentation_flags &= 1;
}

void ActorDraw_RunActorFifteenScriptBranch(void)
{
    void Talk_Run95(s32);

    Talk_Run17(15)->presentation_flags |= 2;
    Talk_Run96();
    if (Talk_Check17(0x855) == 0)
        Talk_Run95(0x123d);
    else
        Talk_Do43(0x134b);
    Talk_Do44(15);
    Talk_Run97();
    Talk_Run18(15)->presentation_flags &= 1;
}

void ActorDraw_RunActorSixteenScriptBranch(void)
{
    void Talk_Run98(int);

    Talk_Run99();
    if (Talk_Do45(0x855) == 0) Talk_Do46(0x123e); else Talk_Run98(0x134c);
    Talk_Do47(16); Talk_Run100();
}

void ActorDraw_RunActorNineteenScriptBranch(void)
{
    void Talk_Run101(int);

    u8 *actor = Talk_Run19(19); actor[91] = 1; Talk_Run102();
    if (Talk_Run103(0x855) == 0) {
        Talk_Run104(0x1241); Talk_Apply32(19, 0); Talk_Run101(2);
    } else if (Talk_Do48(0x858) != 0) {
        Talk_Run105(0x13ab);
    } else {
        Talk_Do49(0x134e);
    }
    Talk_Apply33(19, 0); Talk_Run106(); actor[91] = 0;
}

void Dialogue_RunActor21Line(void) { Talk_Run107(); Talk_Do50(0x12c1); Talk_Do51(21); Talk_Run108(); }

void State_Apply200ThenPlace55_26(void)
{
    Talk_Do52(0x200);
    {
        int v1 = 23;
        int v2 = 26;
        Talk_SetRect(55, 26, 4, 2, v1, v2);
    }
}

void State_Apply200ThenPlace23_23(void)
{
    Talk_Do53(0x200);
    {
        int v1 = 23;
        int v2 = 26;
        Talk_SetRect2(23, 23, 4, 2, v1, v2);
    }
}

void Actor_PlaceActor0AndSetSceneDelay(s32 x, s32 y, s32 continuation)
{
    extern u8 *gWork;

    SetScale(0, 0x8000, 0x4000);
    Talk_Place26(0, x, y);
    *(s32 *)(gWork + 456) = 16;
    Talk_Do54(continuation);
}

void Scene_SetupScene5At408_320(void)
{
    Talk_Do55(158); Talk_Place27((void *)0x0200a828, 56, 19); Talk_Place28(408, 320, 5);
}

void Scene_SetupScene6At312_304(void)
{ Talk_Do56(158); Talk_Place29((void *)0x0200a83e, 50, 18); Talk_Place30(312, 304, 6); }

void Scene_SetupScene7At216_288(void)
{ Talk_Do57(158); Talk_Place31((void *)0x0200a854, 44, 17); Talk_Place32(216, 288, 7); }

void ActorDraw_SetupActorZeroForSceneEightAt376_224(void)
{
    struct SceneActor3 *actor = Talk_Run20(0);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Talk_Do58(158);
    Talk_Place33((const void *)0x0200a86a, 54, 13);
    {
        s32 cell = 23;
        s32 row = 12;

        Talk_SetRect3(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    Talk_Place34(376, 224, 8);
}

void ActorDraw_SetupActorZeroForSceneNineAt296_176(void)
{
    void Talk_Run109(s32, s32, s32);

    struct SceneActor4 *actor = Talk_Run21(0);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Talk_Do59(158);
    Talk_Place35((const void *)0x0200a880, 49, 10);
    {
        s32 cell = 18;
        s32 row = 10;

        Talk_SetRect4(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    Talk_Run109(296, 176, 9);
}

void Scene_SetupScene10At120_144(void)
{ Talk_Do60(158); Talk_Place36((void *)0x0200a896, 38, 6); Talk_Place37(120, 144, 10); }

void ActorDraw_SetupActorZeroForSceneTwelveAt72_160(void)
{
    struct SceneActor *Talk_Run110(s32);

    struct SceneActor5 *actor = Talk_Run110(0);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Talk_Do61(158);
    Talk_Place38((const void *)0x0200a8ac, 35, 9);
    {
        s32 cell = 4;
        s32 row = 10;

        Talk_SetRect5(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    Talk_Place39(72, 160, 12);
}

void Scene_SetupScene13At152_264(void) { Talk_Do62(123); Talk_Place40(152, 264, 13); }

void ActorDraw_MoveActorToPositionAndWait(int actor, int x, int z, int field40)
{
    void Talk_Run111(int);
    void Talk_Run112(int, int, int);

    u8 *record = Talk_Run22(actor); int frames;
    Talk_Place41(actor, 0x30000, 0x18000); *(s32 *)(record + 72) = 0x8000;
    *(s32 *)(record + 68) = 0; *(s32 *)(record + 40) = field40; Talk_Apply34(record, 0);
    Talk_Place42(actor, x, z); Talk_Run112(actor, x << 16, z << 16);
    for (frames = 60; frames != 0; --frames) { Talk_Run111(1); if (*(s16 *)(record + 42) == 0) break; }
    Talk_Apply35(record, 1); *(s32 *)(record + 72) = 0x10000;
}

void Scene_RunActor23SequenceOnceByFlag867(void)
{
    extern u8 gWork[];
    void Talk_Run113();

    u32 i;
    s32 record;

    Talk_Run114();
    Talk_Run115(100);
    Talk_Run116(40);
    if (Talk_Check4(0x867) == 0) {
        Talk_Run(23, 0x102);
        Talk_Run117(23, 4, 0);
        Talk_Run113(12);
        Talk_Run118(23, 4, 0);
        Talk_Run119(20);
        Talk_Run2(23, 0x188, 104, 0x70000);
        Talk_Run120(20);
        Talk_Place3(23, 0x198, 104);
        Talk_Place4(23, 0x198, 120);
        Talk_Do2(0x867);
    }
    Talk_Run121();
}

void Scene_RunActor19MotionSequence(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Talk_Run122(231);
    Talk_Run123();
    Talk_Run124(10);
    Talk_Run125(19, 2);
    Talk_Place5(19, 0xcccc, 0x6666);
    Talk_Place6(19, 216, 0x198);
    Talk_Run126(10);
    Talk_Place7(19, 0x4000, 20);
    Talk_Run127(19, 6, 0);
    Talk_Run128(30);
    Talk_Run129(19, 6, 0);
    Talk_Run130(30);
    Talk_Run131(19, 6, 0);
    Talk_Run132(30);
    Talk_Place8(19, 216, 0x188);
    Talk_Run133(10);
    Talk_Place9(19, 0x4000, 20);
    Talk_Do3(0x858);
    Talk_Run134();
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
    Talk_Do63(delay);
}

void Actor_ApplyActorCueThenWait(s32 actor, s32 cue, s32 delay)
{
    Talk_Place43(actor, cue, 0);
    Talk_Do64(delay);
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
    if (Talk_Check5(0x855) == 0) {
        record = Talk_Check6(0x856);
        if (record != 0) {
            goto L_020004cc;
        }
    }
    Talk_Run135((*(s16 *)(((s32)p5 + 0x16c)) - 19));
    goto L_0200059e;
    L_020004cc:;
    Talk_Run136();
    record = Talk_Check7(0);
    if (record != 0) {
        Talk_Run137(2, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Talk_Place10(2, 0xcccc, 0x6666);
    if (*(s16 *)(((s32)p5 + 0x16c)) == 20) {
        Talk_Place11(2, 0x190, 0x1c0);
    } else {
        Talk_Run3(0xcccc, 0x1999);
        Talk_Run4(0xe00000, -1, 0xa20000, 1);
        Talk_Run138(2, 224, 162);
        Talk_Run139();
    }
    Talk_Run140(0, 2, 0);
    Talk_Run141(20);
    Talk_Do4(0x1327);
    Talk_Place12(0x9002, 0, 20);
    Talk_Run142(0, 3);
    if (Talk_Run5()!= 0) {
        Talk_Do5(0x132a);
        Talk_Run143(2, 0);
        Talk_Run144();
        Talk_Run145(20);
    }
    Talk_Run146(2);
    Talk_Run147((*(s16 *)(((s32)p5 + 0x16c)) - 19));
    Talk_Run148();
    Talk_Run149();
    Talk_Run150();
    L_0200059e:;
}

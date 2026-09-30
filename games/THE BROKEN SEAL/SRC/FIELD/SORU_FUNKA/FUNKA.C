/* Scene tables, the hostage scene and small callbacks. */
#include "FUNKA.H"
#include "CALL.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IWRAM_CALL.H"
#include "FIELD_EFFECT.H"

extern u8 MsgSoruIKnowItsARock[];
extern u8 MsgSoruJasmineWhatHappened[];
extern u8 MsgSoruSomeoneIsLiftingIt[];
extern u8 MsgSoruSukuretaCouldThatBeThe[];

s32 IwramUnsignedDivide(s32 num, s32 den);
void Engine_WorkSetValuesIfNonNegative(s32 first, s32 second, s32 third);
s32 Engine_RandomNext(void);
s32 IwramUnsignedRemainder(s32 value, s32 modulus);
s32 Engine_GameFlagIsSet(s32 flag);
void Engine_AudioPlayCue(s32 cue);

struct EmberSprite {
    u8 unknown_00[9];
    u8 lo : 2;
    u8 layer : 2;
    u8 hi : 4;
};

struct Ember {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[16];
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 speed;
    s32 acceleration;
    s32 target_x;
    s32 target_y;
    s32 target_z;
    u8 unknown_44[12];
    struct EmberSprite *sprite;
    u8 unknown_54;
    u8 motion_flags;
};

/* The embers' animation script, laid out after the code. */
extern const s32 Funka_EmberScript[];
extern s32 gEmberTimer;
extern s32 gEmberState[16];
extern s32 gEmberMask;
extern u8 MsgSoruCannotResist[];
extern u8 MsgSoruFriendsGone[];
extern u8 MsgSoruOverHere[];
extern u8 MsgSoruQuitActingTough[];
extern u8 MsgSoruReturnStarToBag[];
extern u8 MsgSoruThanksALot[];
extern u8 MsgSoruTheyllBeSafe[];
extern u8 MsgSoruThisIsTerrible[];
extern u8 MsgSoruWellTurnedBadly[];

enum {
    ACTOR_WISE_ONE = 15,
    /* The actors SoruFunka_StepEmbers moves. */
    ACTOR_FIRST_ROCK = 16,
    ROCK_COUNT = 16
};

enum {
    OBJECT_ELEMENTAL_STAR = 22,
    /* Its name is MsgItemName + 222. */
    ITEM_MARS_STAR = 222
};

enum {
    /* Set as the party leaves the collapsing chamber. */
    FLAG_SOL_SANCTUM_ERUPTED = 0x814,
    FLAG_STAR_ROOM_COLLAPSED = 0x83f
};

struct QuakeWork {
    u8 unknown_000[0x40c];
    s32 unknown_40c;
};

/* The ten arcing effects' origins, laid out after the code. */
extern s32 Funka_ArcOrigins[][2];
void SoruFunka_SpawnEffectPair();
void SceneEffect_UpdateArcOverAnchor();
void SceneEffect_UpdateAnchoredRiseArc();

struct PairDetail {
    u8 unknown_00[22];
    u8 field_16;
};

struct PairSprite {
    struct FieldSprite sprite;
    struct PairDetail *detail;
};

union PairObject {
    union FieldObject object;
    s32 words[28];
    struct {
        u8 unknown_00[0x68];
        union PairObject *parent;
    } link;
};

struct PairWork {
    u8 unknown_00[70];
    u16 vram_block;
};

LAYOUT_OFFSET_GUARD(PairSprite_Detail, struct PairSprite, detail, 0x28);
LAYOUT_OFFSET_GUARD(PairObject_Parent, union PairObject, link.parent, 0x68);
extern struct PairWork *gEffectWork;

struct WorldMapVramBlock {
    u16 base;
    u16 offset;
};

extern struct WorldMapVramBlock gVramBlockCache[];
void Resource_ResetEntry(s32 block);
s32 AnimationObjects_SelectAnimation(struct FieldSprite *sprite, s32 animation);

/* The OAM view with attribute 1 ending in the two-bit size field. */
struct WorldMapOam {
    u8 unknown_00[4];
    u16 attr0;
    u16 x : 9;
    u16 affine_index : 5;
    u16 size : 2;
};

u8 *SceneData_GetScriptTable(void)
{
    return Placement_Scripts;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetMessageTable(void)
{
    return Placement_Messages;
}

u8 *SceneData_GetActorTable(void)
{
    return Placement_Actors;
}

u8 *SceneData_GetEffectTable(void)
{
    return Placement_Effects;
}

void Scene_SaturosTakesHostages(void)
{
    void Actor_ShowEmote();
    void ColorBuffer_ApplyTarget();
    void ColorBuffer_Interpolate();
    void Audio_PlayCue();

    u32 i;
    u8 *record;
    s32 base5_3001ec4;
    s32 base5_3001ebc;
    u8 *p7;

    base5_3001ec4 = (s32)gParticleWork;
    p7 = *(u8 **)base5_3001ec4;
    Event_Begin();
    *(s32 *)(((s32)p7 + 0x40c)) = 0;
    Audio_PlayCue(141);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0xe80000, 0x9c0000);
    Actor_SetPosition(ACTOR_GERALD, 0xda0000, 0xac0000);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xb000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 0);
    Actor_SetPosition(ACTOR_JASMINE, 0x1db0000, 0x14c0000);
    Actor_SetPosition(ACTOR_SUKURETA, 0x1eb0000, 0x14c0000);
    Actor_SetPosition(ACTOR_MENARDI, 0x1cb0000, 0x15c0000);
    Actor_SetPosition(ACTOR_SATUROS, 0x1fb0000, 0x15c0000);
    Actor_SetPosition(ACTOR_GARCIA, 0x1d70000, 0x1320000);
    Actor_SetPosition(ACTOR_ALEX, 0x1df0000, 0x16a0000);
    Actor_SetSpritePriority(15, 1);
    base5_3001ebc = base5_3001ec4 - 8;
    Camera_MoveTo(0xe80000, -1, 0x9c0000, 0);
    Map_Redraw();
    *(s32 *)((*(s32 *)base5_3001ebc + 0x1c8)) = 8;
    Event_OpenScreen();
    Event_WaitForScreen();
    ColorBuffer_ApplyTarget(0x7fff, 0);
    ColorBuffer_Interpolate(4);
    Event_Wait(4);
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(4);
    Event_Wait(4);
    SoruFunka_ThrowEruptionRing();
    ColorBuffer_ApplyTarget(0x7fff, 0);
    ColorBuffer_Interpolate(4);
    Event_Wait(16);
    Audio_PlayCue(144);
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(4);
    Event_Wait(4);
    ColorBuffer_ApplyTarget(0x7fff, 0);
    ColorBuffer_Interpolate(4);
    Event_Wait(4);
    Audio_PlayCue(144);
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(48);
    Event_Wait(48);
    Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
    Actor_Jump(ACTOR_GERALD, 6, 20);
    FieldScene_RunVariantStep(1, 20, 20);
    FieldScene_RunVariantStep(0, 20, 40);
    Event_SetMessage((s32)MsgSoruJasmineWhatHappened);
    Engine_EventShowMessageAndWait(11, 0, 20);
    Engine_EventShowMessage(10, 0);
    FieldScene_RunVariantStep(1, 20, 0);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 20);
    FieldScene_RunVariantStep(0, 20, 0);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 20);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Event_Wait(40);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xb000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 30);
    FieldScene_RunVariantStep(1, 20, 0);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    FieldScene_RunVariantStep(0, 20, 20);
    record = Object_GetById(15);
    Actor_SetSpriteFlags(record, 0);
    Actor_SetPosition(15, 0x1450000, 0x12e0000);
    record = Object_GetById(15);
    record[85] = 5;
    *(s32 *)(((s32)p7 + 0x40c)) = 1;
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    MapRender_WaitForValues();
    Event_Wait(150);
    Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 20);
    Event_ShowMessage(ACTOR_JASMINE, 0);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x3000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x3000, 10);
    Actor_SetPosition(ACTOR_JASMINE, 0x1db0000, 0x14c0000);
    Actor_SetPosition(ACTOR_SUKURETA, 0x1eb0000, 0x14c0000);
    Actor_SetPosition(ACTOR_MENARDI, 0x1cb0000, 0x15c0000);
    Actor_SetPosition(ACTOR_SATUROS, 0x1fb0000, 0x15c0000);
    Actor_SetPosition(ACTOR_GARCIA, 0x1d70000, 0x1320000);
    Actor_SetPosition(ACTOR_ALEX, 0x1df0000, 0x16a0000);
    Camera_SetSpeed(0x66666, 0xcccc);
    Camera_MoveTo(0x1480000, -1, 0x12b0000, 1);
    Camera_WaitForMove();
    Audio_PlayCue(167);
    ColorBuffer_ApplyTarget(0x205294, 2);
    ColorBuffer_Interpolate(20);
    Task_Wait(20);
    ColorBuffer_ApplyTarget(0x10000, 2);
    ColorBuffer_Interpolate(20);
    Event_Wait(200);
    Event_OpenMessage(0x1001, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_SetMessage((s32)MsgSoruIKnowItsARock);
    } else {
        Event_SetMessage((s32)MsgSoruSomeoneIsLiftingIt);
    }
    Event_ShowMessageAndWait(0x1001, 0, 80);
    Event_SetMessage((s32)MsgSoruSukuretaCouldThatBeThe);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 20);
    FieldScene_RunVariantStep(1, 20, 0);
    *(s32 *)(((s32)p7 + 0x40c)) = 0;
    Audio_PlayCue(141);
    Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
    Event_Wait(80);
    *(s32 *)(((s32)p7 + 0x40c)) = 1;
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    MapRender_WaitForValues();
    FieldScene_RunVariantStep(0, 20, 60);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 30);
    Actor_FaceDirection(15, 0xa000, 40);
    FieldScene_RunVariantStep(1, 20, 20);
    FieldScene_RunVariantStep(0, 20, 20);
    Event_ShowMessageAndWait(0x1001, 0, 30);
    Actor_FaceDirection(15, 0x1000, 40);
    FieldScene_RunVariantStep(1, 20, 20);
    FieldScene_RunVariantStep(0, 20, 20);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 30);
    SceneState_InitStateWordsAndSlots();
    Value2(Engine_TaskAddCallback, (s32)SoruFunka_StepEmbers, 0xc80);
    Value2(Engine_TaskAddCallback, (s32)SceneState_UpdateRandomTimerLevel, 0xc80);
    Event_Wait(240);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 30);
    Actor_SetPosition(ACTOR_JASMINE, 0x1db0000, 0x14c0000);
    Actor_SetPosition(ACTOR_SUKURETA, 0x1eb0000, 0x14c0000);
    Actor_SetPosition(ACTOR_MENARDI, 0x1cb0000, 0x15c0000);
    Actor_SetPosition(ACTOR_SATUROS, 0x1fb0000, 0x15c0000);
    Actor_SetPosition(ACTOR_GARCIA, 0x1d70000, 0x1320000);
    Actor_SetPosition(ACTOR_ALEX, 0x1df0000, 0x16a0000);
    Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x8000, 0);
    Actor_FaceDirection(ACTOR_MENARDI, 0x8000, 0);
    Actor_FaceDirection(ACTOR_SATUROS, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GARCIA, 0x8000, 0);
    Actor_FaceDirection(ACTOR_ALEX, 0x8000, 0);
    record = Object_GetById(5);
    Camera_MoveTo((*(s16 *)((s32)record + 10) << 16), -1, (*(s16 *)((s32)record + 18) << 16), 1);
    Camera_WaitForMove();
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_GARCIA, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GARCIA, 0);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
    Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 10);
    Actor_SetAnimation(ACTOR_ALEX, 4);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
    Actor_FaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 40);
    Actor_SetAnimation(ACTOR_SATUROS, 4);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
    Actor_ShowEmote(ACTOR_JASMINE, 0x100, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x3000, 40);
    Event_ShowMessageAndWait(0x4005, 0, 10);
    Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 0);
    Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 40);
    Actor_FaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 40);
    Actor_ShowEmote(ACTOR_SATUROS, 0x105, 40);
    Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 10);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
    Actor_FaceDirection(ACTOR_SATUROS, 0x8000, 10);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
    Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 10);
    Actor_SetAnimation(ACTOR_SATUROS, 3);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 40);
    Actor_FaceDirection(ACTOR_ALEX, 0xb000, 60);
    Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 10);
    Actor_FaceDirection(ACTOR_GARCIA, 0x3000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x3000, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 0);
    Actor_FaceDirection(ACTOR_MENARDI, 0x3000, 0);
    Actor_FaceDirection(ACTOR_SATUROS, 0x5000, 20);
    Actor_FaceDirection(ACTOR_ALEX, 0xd000, 20);
    Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 10);
    Actor_ShowEmote(ACTOR_GARCIA, 0x103, 0);
    Actor_SetSpeed(ACTOR_GARCIA, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_GARCIA, 0x1d7, 0x13a);
    Actor_StartRepeatedMotion(ACTOR_GARCIA, 3);
    Event_ShowMessageAndWait(ACTOR_GARCIA, 0, 10);
    Actor_FaceDirection(ACTOR_ALEX, 0xb000, 10);
    Actor_SetAnimation(ACTOR_ALEX, 4);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 10);
    Actor_StartRepeatedMotion(ACTOR_GARCIA, 3);
    Event_ShowMessageAndWait(ACTOR_GARCIA, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_ShowMessageAndWait(0x4009, 0, 10);
    Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 20);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 30);
    Actor_FaceDirection(ACTOR_MENARDI, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 30);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 4);
    Event_Wait(10);
    Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 10);
    Actor_FaceDirection(ACTOR_GARCIA, 0x5000, 20);
    Actor_RunRepeatedMotion(ACTOR_GARCIA, 2);
    Event_Wait(20);
    Actor_FaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 0);
    Event_Wait(30);
    Actor_SetAnimation(ACTOR_SATUROS, 3);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
    Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 0);
    Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 0);
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_GARCIA, 1);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_GARCIA, 0x8000, 0);
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_GARCIA, 0x102, 0);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_ALEX, 0xb000, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
    Event_ShowMessageAndWait(0x200e, 0, 30);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xb000, 0);
    Actor_FaceDirection(ACTOR_GARCIA, 0x3000, 20);
    Actor_ShowEmote(ACTOR_JASMINE, 0x102, 40);
    Event_ShowMessageAndWait(0x2005, 0, 40);
    Actor_ShowEmote(ACTOR_GARCIA, 0x105, 60);
    Actor_SetAnimationAndWait(ACTOR_GARCIA, 3);
    Actor_ShowEmote(ACTOR_SATUROS, 0x101, 0);
    Actor_ShowEmote(ACTOR_MENARDI, 0x101, 60);
    Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 60);
    Actor_SetAnimationAndWait(ACTOR_GARCIA, 3);
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_JASMINE, 0x102, 0);
    Actor_ShowEmote(ACTOR_SUKURETA, 0x102, 40);
    Actor_FaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 20);
    Actor_SetAnimation(ACTOR_SATUROS, 3);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_SATUROS, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_MENARDI, 0x9999, 0x4ccc);
    Actor_WalkTo(ACTOR_MENARDI, 0x1db, 0x15c);
    Actor_WalkTo(ACTOR_SATUROS, 0x1eb, 0x15c);
    Actor_WaitForMove(ACTOR_MENARDI);
    Actor_WaitForMove(ACTOR_SATUROS);
    Actor_SetAnimation(ACTOR_MENARDI, 1);
    Actor_SetAnimation(ACTOR_SATUROS, 1);
    Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 0);
    Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 0);
    Actor_FaceDirection(ACTOR_GARCIA, 0x3000, 10);
    Actor_ShowEmote(ACTOR_MENARDI, 0x103, 0);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
    Event_ShowMessage(0x200b, 0);
    Actor_SetSpeed(ACTOR_MENARDI, 0x19999, 0xcccc);
    Actor_SetSpeed(ACTOR_JASMINE, 0x13333, 0x9999);
    Actor_WalkToAndWait(ACTOR_MENARDI, 0x1db, 0x152);
    record = Object_GetById(11);
    ((struct FacingObject *)record)->facing_flags = (u8)(254 & ((struct FacingObject *)record)->facing_flags);
    Actor_WalkToAndWait(ACTOR_MENARDI, 0x1db, 0x15c);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 1);
    Actor_Jump(ACTOR_JASMINE, 4, 0);
    Actor_WalkToAndWait(ACTOR_JASMINE, 0x1cb, 0x13c);
    Actor_SetSpeed(ACTOR_JASMINE, 0x8000, 0x4000);
    Actor_ShowEmote(ACTOR_GARCIA, 0x103, 0);
    Actor_StartRepeatedMotion(ACTOR_GARCIA, 3);
    Event_ShowMessageAndWait(ACTOR_GARCIA, 0, 30);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
    Event_Wait(20);
    ((struct FacingObject *)record)->facing_flags |= 1;
    Actor_SetSpeed(ACTOR_MENARDI, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_MENARDI, 0x1db, 0x14c);
    Actor_FaceDirection(ACTOR_MENARDI, 0xb000, 20);
    Event_ShowMessage(0x200b, 0);
    Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 0);
    Actor_StartRepeatedMotion(ACTOR_SATUROS, 2);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
    Actor_ShowEmote(ACTOR_MENARDI, 0x102, 20);
    Actor_FaceDirection(ACTOR_JASMINE, 0x3000, 0);
    Actor_FaceDirection(ACTOR_MENARDI, 0x3000, 20);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, 4);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
    Actor_FaceDirection(ACTOR_MENARDI, 0xb000, 20);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
    Actor_SetAnimationAndWait(ACTOR_GARCIA, 3);
    Event_Wait(20);
    Actor_WalkTo(ACTOR_JASMINE, 0x1b0, 0x13c);
    Actor_WalkToAndWait(ACTOR_GARCIA, 0x1a6, 0x137);
    Actor_SetAnimation(ACTOR_JASMINE, 1);
    Actor_FaceDirection(ACTOR_JASMINE, 0x3000, 0);
    Actor_FaceDirection(ACTOR_GARCIA, 0x3000, 20);
    Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
    Actor_FaceDirection(ACTOR_SATUROS, 0xd000, 20);
    Event_ShowMessageAndWait(0x100a, 0, 40);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 30);
    Actor_SetSpeed(ACTOR_SUKURETA, 0x8000, 0x4000);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1eb, 0x128);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 40);
    Actor_SetSpeed(ACTOR_SATUROS, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_MENARDI, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_GARCIA, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_ALEX, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_SATUROS, 0x1d7, 0x134);
    Actor_FaceDirection(ACTOR_SATUROS, 0x5000, 0);
    Actor_WalkToAndWait(ACTOR_MENARDI, 0x1c7, 0x134);
    Actor_FaceDirection(ACTOR_MENARDI, 0x3000, 0);
    Actor_WalkToAndWait(ACTOR_ALEX, 0x1e7, 0x134);
    Actor_FaceDirection(ACTOR_SATUROS, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 0);
    Actor_FaceDirection(ACTOR_ALEX, 0xb000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xd000, 0);
    Actor_FaceDirection(ACTOR_GARCIA, 0xd000, 0);
    Camera_SetSpeed(0x8000, 0x1000);
    Camera_MoveTo(0x1d80000, -1, 0x12c0000, 1);
    Camera_WaitForMove();
    Event_Wait(40);
    GameFlag_Set(0x246);
    Actor_SetSpeed(ACTOR_SATUROS, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_MENARDI, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_ALEX, 0x8000, 0x4000);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, 3);
    FieldScene_RunScene381_02000e30(10);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    FieldScene_RunScene381_02000e30(9);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
    FieldScene_RunScene381_02000e30(11);
    Actor_FaceDirection(ACTOR_JASMINE, 0x9000, 40);
    Actor_StartRepeatedMotion(ACTOR_JASMINE, 2);
    Event_ShowMessageAndWait(0x2005, 0, 40);
    Actor_FaceDirection(ACTOR_GARCIA, 0x3000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xb000, 30);
    Actor_FaceDirection(ACTOR_GARCIA, 0xe000, 0);
    FieldScene_RunScene381_02000e30(5);
    FieldScene_RunScene381_02000e30(13);
    Actor_FaceDirection(ACTOR_ALEX, 0x7000, 40);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 30);
    Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 30);
    FieldScene_RunScene381_02000e30(14);
    Scene_RunExtendedEffectPresentation();
    Party_RemoveOwnerRestored(5);
}

void FieldScene_RunScene381_02000e30(s32 a0)
{
    u8 *rec7;
    s32 recA;
    s32 rec2;
    u8 i;

    recA = Object_GetById(8);
    *(s32 *)(recA + 24) = 0x10000;
    *(s32 *)(recA + 28) = 0x10000;
    Engine_ActorWalkToAndWait(a0, 0x1d7, 0x122);
    Actor_FaceDirection(a0, 0xc000, 0);
    Event_Wait(10);
    Actor_SetPosition(8, 0x1d70000, 0x1220000);
    rec7 = Object_GetById(a0);
    rec2 = Object_GetById(a0);
    Actor_SetSpriteFlags(rec2, 0);
    Actor_SetChildValue(a0, 0x100);
    rec7[85] = 0;
    Audio_PlayCue(201);
    i = 0;
    do {
        *(s32 *)(rec7 + 12) += 0x8000;
        Event_Wait(1);
        i++;
    } while (i != 60);
    Audio_PlayCue(190);
    i = 0;
    do {
        *(s32 *)(rec7 + 12) += 0x1999;
        *(s32 *)(rec7 + 24) -= 0x28f;
        *(s32 *)(rec7 + 28) -= 0x28f;
        *(s32 *)(recA + 24) -= 0x28f;
        *(s32 *)(recA + 28) -= 0x28f;
        Event_Wait(1);
        i++;
    } while (i != 90);
    Actor_SetPosition(a0, 0, 0);
    Actor_SetPosition(8, 0, 0);
}

void Resource381_NoOpCallbackA(void)
{
}

void Resource381_NoOpCallbackB(void)
{
}

s32 FieldScene_RunWhenWord225Is10(void)
{
    if (gGameState.entrance == 10) {
        FieldEffect_InitSparkles();
        Scene_SaturosTakesHostages();
    }
    return 0;
}

s32 OverlayObject_SetRecordAngleFromHeading(Ent *p)
{
    *(u16 *)(p->unk50 + 30) = p->unk6 + 0x4000;
    return 1;
}

/* Gather actors 16 to 31 at the crater, fling them out in a ring, then
 * reset them once they land. */
void SoruFunka_ThrowEruptionRing(void)
{
    struct FieldActor *actor;
    struct FieldActor *spun;
    u32 i;
    s32 angle;
    struct FieldSprite *sprite;
    s32 dx;
    s32 dz;

    for (i = 16; i <= 31; i++) {
        actor = Object_GetById(i);
        Engine_ActorStop(i);
        Engine_ActorSetSpriteFlags(actor, 0);
        Object_SetMode(actor, 2);
        actor->sprite->priority = 0;
        actor->motion_flags = 0;
        actor->speed = 0x80000;
        actor->acceleration = 0xc000;
        actor->scale_x = 0x1cccc;
        actor->scale_y = 0x1cccc;
        actor->x.fixed = 0xe80000;
        actor->y.fixed = 0x140000;
        actor->z.fixed = 0x840000;
    }
    Engine_AudioPlayCue(145);
    for (i = 0; i <= 15; i++) {
        spun = Object_GetById(i + 16);
        sprite = spun->sprite;
        angle = i << 12;
        sprite->rotation = angle - 0x4000;
        dx = Iwram_MulQ16(Engine_MathCos(angle), 0x1000000);
        dz = Iwram_MulQ16(Engine_MathSin(angle), 0x1000000);
        Engine_ObjectSetPosition(spun, spun->x.fixed + dx, spun->y.fixed, spun->z.fixed + dz);
    }
    Engine_EventWait(20);
    Engine_ActorWaitForMove(16);
    for (i = 16; i <= 31; i++) {
        actor = Object_GetById(i);
        Engine_ActorStop(i);
        actor->scale_x = 0x10000;
        actor->scale_y = 0x10000;
        actor->x.fixed = 0;
        actor->y.fixed = 0;
        actor->z.fixed = 0;
        actor->velocity_x = 0;
        actor->velocity_y = 0;
        actor->velocity_z = 0;
        actor->target_x = ACTOR_NO_TARGET;
        actor->target_y = ACTOR_NO_TARGET;
        actor->target_z = ACTOR_NO_TARGET;
    }
}

/* The scene's state words and slots. */
void SceneState_InitStateWordsAndSlots(void)
{
    s32 *p;
    u32 i;

    gEmberMask = 63;
    gEmberTimer = 0;
    gEmberLevel = 0;
    gEmberLevelTimer = 120;
    p = gEmberState;
    for (i = 0; i < 16; i++) {
        *p++ = 0;
    }
}

/* Sixteen-actor effect step (actors 16..31): sets the work values from the
 * effect timer, ages the actors that have come to rest, and when the frame
 * mask allows, launches the first idle one on a random bearing. */
void SoruFunka_StepEmbers(void)
{
    u8 *work;
    s32 level;
    s32 t;
    u8 i;
    struct Ember *spark;
    struct Ember *ember;
    s16 angle;
    s32 x;
    s32 z;

    work = (*(u8 * *)gParticleWork);
    level = IwramUnsignedDivide(gEmberTimer, 10);
    if (level != 0) {
        *(s32 *)(work + 0x40c) = 0;
        Call3((void (*)())Engine_WorkSetValuesIfNonNegative, level << 16, level << 16, 0x10000);
    } else {
        *(s32 *)(work + 0x40c) = 1;
        Call3((void (*)())Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    }
    t = gEmberTimer;
    if (t != 0)
        gEmberTimer = t - 3;

    for (i = 0; i < 16; i++) {
        if (gEmberState[i] != 0) {
            spark = (void *)Object_GetById(i + 16);
            if (spark->target_x == (s32)0x80000000 && spark->target_z == (s32)0x80000000) {
                gEmberState[i]++;
                if (gEmberState[i] == 2)
                    Object_SetMode(spark, 3);
                if (gEmberState[i] == 19) {
                    spark->x = 0;
                    spark->y = 0;
                    spark->z = 0;
                    spark->velocity_x = 0;
                    spark->velocity_y = 0;
                    spark->velocity_z = 0;
                    spark->target_x = 0x80000000;
                    spark->target_y = 0x80000000;
                    spark->target_z = 0x80000000;
                    ObjectGroup_SetChildValue(spark, 15);
                } else if (gEmberState[i] == 20) {
                    gEmberState[i] = 0;
                }
            }
        }
    }

    if (gEmberMask == 999)
        return;
    if (gEmberMask & gFrameCount)
        return;
    for (i = 0; i < 16; i++) {
        angle = IwramUnsignedRemainder(Engine_RandomNext(), 0xffff);
        ember = (void *)Object_GetById(i + 16);
        if (gEmberState[i] == 0) {
            if (Engine_GameFlagIsSet(0x246) == 0)
                Engine_AudioPlayCue(246);
            gEmberState[i] = 1;
            ember->motion_flags = 0;
            ember->speed = 0x80000;
            ember->acceleration = 0x10000;
            Engine_ActorSetSpriteFlags(ember, 0);
            ember->sprite->layer = 1;
            Object_SetMode(ember, 2);
            Engine_ObjectSetScript(ember, Funka_EmberScript);
            ember->x = Iwram_MulQ16(((s32 (*)(u16))Engine_MathCos)(angle), (((u32)(Engine_RandomNext() << 8) >> 16) << 16) + 0x1000000) + 0x1450000;
            ember->y = 0;
            ember->z = Iwram_MulQ16(((s32 (*)(u16))Engine_MathSin)(angle), (((u32)(Engine_RandomNext() << 8) >> 16) << 16) + 0x1000000) + 0x12e0000;
            x = Iwram_MulQ16(((s32 (*)(u16))Engine_MathCos)(angle), ((Engine_RandomNext() & 63) << 16) + 0x80000);
            z = Iwram_MulQ16(((s32 (*)(u16))Engine_MathSin)(angle), ((Engine_RandomNext() & 63) << 16) + 0x80000);
            Engine_ObjectSetPosition(ember, x + 0x1450000, 0, z + 0x11e0000);
            ObjectGroup_SetChildValue(ember, 0);
            gEmberTimer = 30;
            return;
        }
    }
}

/* The random timer level. */
void SceneState_UpdateRandomTimerLevel(void)
{
    u32 v;

    if (gEmberLevelTimer != 0) {
        gEmberLevelTimer--;
        return;
    }
    if (gEmberLevel != 0) {
        gEmberLevel--;
    } else {
        gEmberLevel = (u32)(Random_Next() << 2) >> 16;
    }
    v = gEmberLevel;
    switch (v) {
    case 3:
        gEmberMask = v;
        gEmberLevelTimer = ((u32)(Random_Next() * 20) >> 16) + 40;
        break;
    case 2:
        gEmberMask = 15;
        gEmberLevelTimer = ((u32)(Random_Next() * 40) >> 16) + 80;
        break;
    case 1:
        gEmberMask = 63;
        gEmberLevelTimer = ((u32)(Random_Next() * 80) >> 16) + 160;
        break;
    default:
        gEmberMask = 127;
        gEmberLevelTimer = ((u32)(Random_Next() * 160) >> 16) + 320;
        break;
    }
}

/* The star chamber's collapse and escape (Scene_RunExtendedEffectPresentation). */

/*
 * The Elemental Star chamber collapses around the party leader and Gerald.
 * The Wise One appears, has the Mars Star put back in its bag, and
 * helps them out as the volcano erupts.
 */
void Scene_RunExtendedEffectPresentation(void)
{
    struct QuakeWork *quake;
    struct FieldActor *wise_one;
    struct FieldActor *center;
    struct FieldActor *leader;
    struct FieldActor *actor;
    struct FieldActor *gerald;
    struct FieldSprite *leader_sprite;
    struct FieldSprite *gerald_sprite;
    struct FieldActor *star;
    struct FieldSprite *sprite;
    u8 *buf;
    u8 i;
    u32 cnt;
    s32 wait;

    quake = (struct QuakeWork *)gParticleWork[0];
    wise_one = Actor_Get(ACTOR_WISE_ONE);
    Task_RemoveCallback(SceneState_UpdateRandomTimerLevel);
    gEmberMask = 3;
    Event_Wait(80);
    Audio_PlayCue(17);
    ColorBuffer_ApplyTarget(0x7fff, 0);
    ColorBuffer_Interpolate(40);
    Event_Wait(40);
    Task_RemoveCallback(SoruFunka_StepEmbers);
    for (i = 0; i < ROCK_COUNT; i++) {
        Actor_SetPosition(ACTOR_FIRST_ROCK + i, 0, 0);
    }
    Task_Wait(1);
    Actor_SetPosition(ACTOR_WISE_ONE, 0, 0);
    SceneActor_MoveTo232_125AndFace4000(ACTOR_PARTY_LEADER);
    SceneActor_MoveTo232_125AndFace4000(ACTOR_GERALD);
    quake->unknown_40c = 0;
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Event_Wait(80);
    SceneState_ConfigureEightCornerRegions();
    center = Event_GetViewCenter();
    center->motion_flags = 0;
    center->x.fixed = PIXELS(231);
    center->z.fixed = PIXELS(144);
    center->target_x = ACTOR_NO_TARGET;
    center->target_y = ACTOR_NO_TARGET;
    center->target_z = ACTOR_NO_TARGET;
    center->y.fixed = 0;
    center->velocity_x = 0;
    center->velocity_z = 0;
    Task_Wait(4);
    Map_Redraw();
    Task_Wait(4);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
    Actor_SetSpritePriority(ACTOR_GERALD, 3);
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(40);
    Event_Wait(40);

    /* The two spin apart. */
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    gerald = Actor_Get(ACTOR_GERALD);
    leader_sprite = leader->sprite;
    gerald_sprite = gerald->sprite;
    for (cnt = 0; cnt < 20; cnt++) {
        leader_sprite->rotation += 0x100;
        gerald_sprite->rotation -= 0x100;
        leader->x.fixed += 0x6000;
        gerald->x.fixed -= 0x6000;
        Task_Wait(1);
    }
    Event_Wait(40);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Actor_SetSpeed(ACTOR_GERALD, 0x20000, 0x10000);
    Actor_Get(ACTOR_PARTY_LEADER)->sprite->rotation = 0;
    Actor_Get(ACTOR_GERALD)->sprite->rotation = 0;
    Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
    Engine_ActorJump(ACTOR_GERALD, 6, 0);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 246, 150);
    Engine_ObjectMotionSetPositionAndCommit(ACTOR_GERALD, 220, 150);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
    Actor_SetSpritePriority(ACTOR_GERALD, 2);
    Actor_Get(ACTOR_PARTY_LEADER)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Actor_Get(ACTOR_GERALD)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Actor_Stop(ACTOR_PARTY_LEADER);
    Actor_Stop(ACTOR_GERALD);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTHEAST, 0);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST + FACING_STEP, 0);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH + FACING_STEP, 0);
    Event_Wait(80);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_GERALD, FACING_EAST + FACING_STEP, 0);
    Event_Wait(60);

    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_SetMessage((s32)MsgSoruWellTurnedBadly);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimationAndWait(ACTOR_GERALD, ANIM_NOD);
        Event_SetMessage((s32)MsgSoruWellTurnedBadly + 1);
    } else {
        Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
        Event_SetMessage((s32)MsgSoruWellTurnedBadly + 2);
    }
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 60);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHEAST + FACING_STEP, 0);
    Event_Wait(40);
    Engine_ActorJump(ACTOR_GERALD, 2, 0);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 2, 0);
    Event_Wait(40);
    Event_SetMessage((s32)MsgSoruFriendsGone);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
    Event_Wait(60);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
    Event_Wait(80);
    Actor_FaceDirection(ACTOR_GERALD, FACING_EAST + FACING_STEP, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        Event_Wait(10);
        Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHEAST + FACING_STEP, 0);
        Event_Wait(60);
        Actor_FaceDirection(ACTOR_GERALD, FACING_EAST + FACING_STEP, 0);
        Event_Wait(10);
        Actor_SetAnimation(ACTOR_GERALD, ANIM_SHAKE_HEAD);
        Event_SetMessage((s32)MsgSoruThanksALot);
    } else {
        Event_Wait(10);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        Event_Wait(10);
        Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHEAST + FACING_STEP, 0);
        Event_Wait(60);
        Actor_FaceDirection(ACTOR_GERALD, FACING_EAST + FACING_STEP, 0);
        Event_Wait(10);
        Actor_SetAnimation(ACTOR_GERALD, ANIM_SHAKE_HEAD);
        Event_SetMessage((s32)MsgSoruTheyllBeSafe);
    }
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 40);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_GERALD, ANIM_NOD);
    Event_Wait(40);

    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHWEST + FACING_STEP, 0);
    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(PIXELS(284), -1, PIXELS(92), 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH + FACING_STEP, 0);
    Camera_SetSpeed(0x18000, 0x3000);
    Camera_MoveTo(PIXELS(127), -1, PIXELS(162), 1);
    Camera_WaitForMove();
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHEAST + FACING_STEP, 0);
    Camera_SetSpeed(0x40000, 0x8000);
    Camera_MoveTo(PIXELS(304), -1, PIXELS(294), 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_EAST + FACING_STEP, 0);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(PIXELS(400), -1, PIXELS(215), 1);
    Camera_WaitForMove();
    Event_Wait(60);
    Camera_SetSpeed(0x40000, 0x8000);
    Camera_MoveTo(PIXELS(273), -1, PIXELS(145), 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_SetMessage((s32)MsgSoruThisIsTerrible);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHEAST + FACING_STEP, 0);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH + FACING_STEP, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTHEAST + FACING_STEP, 0);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHEAST + FACING_STEP, 0);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTHEAST + FACING_STEP, 0);
    Event_Wait(10);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(10);

    Audio_PlayCue(23);
    FieldScene_RunVariantStep(1, 4, 0);
    quake->unknown_40c = 0;
    Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
    Event_Wait(10);
    FieldScene_RunVariantStep(0, 40, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Actor_SetSpeed(ACTOR_GERALD, 0x20000, 0x10000);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
    Engine_ActorJump(ACTOR_GERALD, 6, 0);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 243, 144);
    Actor_SetDestination(ACTOR_GERALD, 202, 144);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Event_Wait(20);
    quake->unknown_40c = 1;
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Event_Wait(60);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 2, 0);
    Event_Wait(80);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
    Actor_SetSpeed(ACTOR_GERALD, 0x6666, 0x3333);
    Actor_WalkTo(ACTOR_GERALD, 220, 150);
    Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 246, 150);
    Actor_SetAnimation(ACTOR_GERALD, ANIM_STAND);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHWEST + FACING_STEP, 0);
    Event_Wait(60);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHEAST + FACING_STEP, 0);
    Event_Wait(40);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_FaceDirection(ACTOR_GERALD, FACING_EAST + FACING_STEP, 0);
        Event_Wait(10);
        Actor_SetAnimationAndWait(ACTOR_GERALD, ANIM_NOD);
    } else {
        Actor_FaceDirection(ACTOR_GERALD, FACING_EAST + FACING_STEP, 0);
        Event_Wait(10);
        Actor_SetAnimation(ACTOR_GERALD, ANIM_SHAKE_HEAD);
        Event_SetMessage((s32)MsgSoruQuitActingTough);
    }
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);

    /* The Wise One arrives. */
    Actor_SetPosition(ACTOR_WISE_ONE, PIXELS(110), PIXELS(152));
    Task_Wait(1);
    Actor_SetSpeed(ACTOR_WISE_ONE, 0x13333, 0x9999);
    Actor_SetDestination(ACTOR_WISE_ONE, 171, 152);
    Actor_FaceDirection(ACTOR_WISE_ONE, FACING_EAST, 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_WISE_ONE), 1);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_WalkTo(ACTOR_GERALD, 217, 182);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST + FACING_STEP, 0);
    Event_Wait(10);
    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(PIXELS(217), -1, PIXELS(176), 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
    Event_Wait(20);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
    Event_Wait(10);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
    Event_Wait(30);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT, 0);
    Actor_SetAnimation(ACTOR_GERALD, ANIM_STAND);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHEAST, 0);
    Event_Wait(4);
    Engine_ActorWalkToAndWait(ACTOR_GERALD, 231, 175);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTHEAST, 0);
    Event_SetMessage((s32)MsgSoruOverHere);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 1, 0);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTHWEST, 0);
    Event_Wait(60);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_SetAttachedEffect(ACTOR_GERALD, EMOTE_IN_FRONT | 2);
    Event_Wait(40);

    FieldScene_RunVariantStep(1, 20, 0);
    quake->unknown_40c = 0;
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Event_Wait(40);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Audio_PlayCue(0x121);
    ColorBuffer_ApplyTarget(0x10000, 1);
    ColorBuffer_Interpolate(40);
    quake->unknown_40c = 1;
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Event_Wait(120);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 2, 0);
    Event_Wait(100);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    Event_Wait(20);
    FieldScene_RunVariantStep(1, 10, 0);
    quake->unknown_40c = 0;
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Actor_SetSpeed(ACTOR_GERALD, 0x20000, 0x10000);
    Actor_Get(ACTOR_PARTY_LEADER)->unknown_5a &= ~1;
    Actor_Get(ACTOR_GERALD)->unknown_5a &= ~1;
    Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
    Engine_ActorJump(ACTOR_GERALD, 4, 0);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 256, 150);
    Actor_SetDestination(ACTOR_GERALD, 231, 180);
    Actor_WaitForMove(ACTOR_GERALD);
    FieldScene_RunVariantStep(0, 40, 0);
    Event_Wait(20);
    Actor_Get(ACTOR_PARTY_LEADER)->unknown_5a |= 1;
    Actor_Get(ACTOR_GERALD)->unknown_5a |= 1;
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 3, 0);
    Event_Wait(40);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_WISE_ONE, EMOTE_IN_FRONT | 1, 0);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH + FACING_STEP, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTHWEST + FACING_STEP, 0);
    Event_Wait(10);

    Audio_PlayCue(107);
    Task_AddCallback(FieldScene_RunRandomHalfBranch, TASK_PRIORITY_SCENE);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_WISE_ONE, EMOTE_IN_FRONT, 0);
    Event_Wait(40);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(PIXELS(186), -1, PIXELS(166), 1);
    Actor_SetDestination(ACTOR_WISE_ONE, 130, 113);
    Actor_WaitForMove(ACTOR_WISE_ONE);
    Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTHEAST + FACING_STEP, 0);
    Event_Wait(20);
    quake->unknown_40c = 0;
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    ColorBuffer_ApplyTarget(0x20119e, 1);
    ColorBuffer_Interpolate(20);
    Event_Wait(20);
    SceneState_SetValue140Mode0();
    Actor_SetAnimationAndWait(ACTOR_WISE_ONE, 2);
    for (cnt = 0; cnt < 40; cnt++) {
        FieldScene_RunStepByRuntimeBits((s32)wise_one);
        Task_Wait(1);
    }
    Task_AddCallback(FieldScene_RunActor15TwoStep, TASK_PRIORITY_SCENE);
    ColorBuffer_ApplyTarget(0x10000, 1);
    ColorBuffer_Interpolate(60);
    Event_Wait(30);
    Audio_PlayCue(0x121);
    Task_RemoveCallback(FieldScene_RunRandomHalfBranch);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    wait = 16;
    while (wait--) {
        SceneState_ApplyRectsByCondition(0);
        Task_Wait(wait);
        SceneState_ApplyRectsByCondition(1);
        Task_Wait(wait);
    }
    SceneState_ApplyRectsByCondition(0);
    quake->unknown_40c = 1;
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_MapRenderWaitForValues();
    Actor_SetAnimationAndWait(ACTOR_WISE_ONE, 3);
    Task_RemoveCallback(FieldScene_RunActor15TwoStep);
    Task_Wait(1);
    Actor_SetChildValue(ACTOR_WISE_ONE, 0);
    Event_Wait(60);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 2, 0);
    Event_Wait(60);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(PIXELS(218), -1, PIXELS(181), 1);
    Actor_SetSpeed(ACTOR_WISE_ONE, 0x10000, 0x8000);
    Actor_SetDestination(ACTOR_WISE_ONE, 169, 151);
    Actor_WaitForMove(ACTOR_WISE_ONE);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_WISE_ONE, FACING_EAST + FACING_STEP, 0);
    Event_Wait(40);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH + FACING_STEP, 0);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_WISE_ONE, FACING_NORTHEAST + FACING_STEP, 0);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTHWEST + FACING_STEP, 0);
    Camera_MoveTo(PIXELS(224), -1, PIXELS(158), 1);
    Camera_WaitForMove();
    FieldScene_RunFourWayEffectSequence(0);
    Actor_RunRepeatedMotion(ACTOR_WISE_ONE, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    Event_Wait(40);
    Event_OpenMessage(ACTOR_WISE_ONE, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(40);
    } else {
        Message_ShowCentered((s32)MsgSoruCannotResist, 1);
        Event_Wait(40);
    }

    /* The Elemental Star rises from the party leader. */
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    star = Object_Create(OBJECT_ELEMENTAL_STAR, actor->x.fixed, actor->y.fixed + PIXELS(36),
                         actor->z.fixed);
    if (star != NULL) {
        buf = Heap_Allocate(17, 0x608);
        sprite = star->sprite;
        sprite->flags = 0;
        sprite->part_count = 0;
        sprite->full_color = 0;
        sprite->palette = 0;
        sprite->priority = 1;
        Item_LoadIcon(ITEM_MARS_STAR);
        Vram_Load(sprite->vram_block, 128, buf + 0x400);
        Heap_Release(17);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 28);
        Engine_RunRisingObjectSequence(star, 3);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 28);
    }
    FieldScene_RunVariantStep(1, 20, 0);
    for (cnt = 0; cnt < 24; cnt++) {
        FieldScene_RunStepByRuntimeBits((s32)wise_one);
        Task_Wait(1);
        FieldScene_RunStepByRuntimeBits((s32)wise_one);
        Task_Wait(1);
        star->scale_x = 0x6666;
        star->scale_y = 0x6666;
        FieldScene_RunStepByRuntimeBits((s32)wise_one);
        Task_Wait(1);
        FieldScene_RunStepByRuntimeBits((s32)wise_one);
        Task_Wait(1);
        star->scale_x = 0x10000;
        star->scale_y = 0x10000;
    }
    Actor_SetChildValue(ACTOR_WISE_ONE, 0);
    FieldScene_RunVariantStep(0, 20, 0);
    Event_SetMessage((s32)MsgSoruReturnStarToBag);
    Event_ShowMessageAndWait(ACTOR_WISE_ONE, 0, 20);
    if (star != NULL) {
        Engine_ObjectDispatchRelease(star);
    }
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTHWEST, 60);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    Event_Wait(40);

    Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTH, 0);
    Camera_SetSpeed(0x40000, 0x8000);
    SceneActor_PlaceAtTileAndRunSteps(232, 464);
    FieldScene_RunFourWayEffectSequence(1);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    SceneActor_PlaceAtTileAndRunSteps(711, 144);
    FieldScene_RunFourWayEffectSequence(2);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    SceneActor_PlaceAtTileAndRunSteps(711, 464);
    FieldScene_RunFourWayEffectSequence(3);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    Actor_FaceDirection(ACTOR_WISE_ONE, FACING_EAST + FACING_STEP, 0);
    Actor_SetPosition(ACTOR_GERALD, PIXELS(582), PIXELS(345));
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_SetPosition(ACTOR_GERALD, PIXELS(231), PIXELS(180));
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTHWEST + FACING_STEP, 0);
    Task_Wait(20);
    SceneActor_PlaceAtTileAndRunSteps(219, 171);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    Event_Wait(10);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    Event_Wait(40);

    quake->unknown_40c = 0;
    Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
    ColorBuffer_ApplyTarget(0x20119e, 1);
    ColorBuffer_Interpolate(20);
    Event_Wait(20);
    Audio_PlayCue(107);
    Task_AddCallback(FieldScene_RunLateRandomHalfBranch, TASK_PRIORITY_SCENE);
    Event_Wait(20);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(PIXELS(184), -1, PIXELS(132), 1);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
    Engine_ActorJump(ACTOR_GERALD, 6, 0);
    Actor_Get(ACTOR_PARTY_LEADER)->unknown_5a &= ~1;
    Actor_Get(ACTOR_GERALD)->unknown_5a &= ~1;
    Actor_SetDestination(ACTOR_PARTY_LEADER, 245, 145);
    Actor_SetDestination(ACTOR_GERALD, 215, 168);
    Actor_WaitForMove(ACTOR_GERALD);
    Event_Wait(80);
    Actor_Get(ACTOR_PARTY_LEADER)->unknown_5a |= 1;
    Actor_Get(ACTOR_GERALD)->unknown_5a |= 1;
    Actor_SetDestination(ACTOR_WISE_ONE, 184, 87);
    Actor_WaitForMove(ACTOR_WISE_ONE);
    Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTH, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_WISE_ONE, 2);
    for (cnt = 0; cnt < 40; cnt++) {
        FieldScene_RunStepByRuntimeBits((s32)wise_one);
        Task_Wait(1);
    }
    Task_AddCallback(FieldScene_RunActor15TwoStep, TASK_PRIORITY_SCENE);
    ColorBuffer_ApplyTarget(0x10000, 1);
    ColorBuffer_Interpolate(60);
    Event_Wait(30);
    Audio_PlayCue(0x121);
    Task_RemoveCallback(FieldScene_RunLateRandomHalfBranch);
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    wait = 8;
    while (wait--) {
        SceneState_ApplyRectPairByFlag(0);
        Task_Wait(wait);
        SceneState_ApplyRectPairByFlag(1);
        Task_Wait(wait);
    }
    SceneState_ApplyRectPairByFlag(0);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Actor_SetAnimationAndWait(ACTOR_WISE_ONE, 3);
    Task_RemoveCallback(FieldScene_RunActor15TwoStep);
    Task_Wait(1);
    Actor_SetChildValue(ACTOR_WISE_ONE, 0);
    Event_Wait(60);

    Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
    ColorBuffer_ApplyTarget(0x20119e, 1);
    ColorBuffer_Interpolate(20);
    Event_Wait(20);
    Audio_PlayCue(107);
    Task_AddCallback(FieldScene_RunRandomHalfBranch, TASK_PRIORITY_SCENE);
    Event_Wait(40);
    Engine_ObjectMotionSetPositionAndCommit(ACTOR_WISE_ONE, 127, 110);
    Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTH, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_WISE_ONE, 2);
    for (cnt = 0; cnt < 40; cnt++) {
        FieldScene_RunStepByRuntimeBits((s32)wise_one);
        Task_Wait(1);
    }
    Task_AddCallback(FieldScene_RunActor15TwoStep, TASK_PRIORITY_SCENE);
    ColorBuffer_ApplyTarget(0x10000, 1);
    ColorBuffer_Interpolate(60);
    Audio_PlayCue(0x121);
    Event_Wait(30);
    Task_RemoveCallback(FieldScene_RunRandomHalfBranch);
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    wait = 8;
    while (wait--) {
        SceneState_ApplyRectsByCondition(0);
        Task_Wait(wait);
        SceneState_ApplyRectsByCondition(1);
        Task_Wait(wait);
    }
    SceneState_ApplyRectsByCondition(0);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Audio_PlayCue(107);
    Audio_PlayCue(63);
    Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
    ColorBuffer_ApplyTarget(0x20119e, 1);
    ColorBuffer_Interpolate(20);
    Event_Wait(20);
    Audio_PlayCue(107);
    Task_AddCallback(FieldScene_RunLateRandomHalfBranch, TASK_PRIORITY_SCENE);
    Actor_SetAnimationAndWait(ACTOR_WISE_ONE, 3);
    Task_RemoveCallback(FieldScene_RunActor15TwoStep);
    Task_Wait(1);
    Actor_SetChildValue(ACTOR_WISE_ONE, 0);
    Event_Wait(60);
    Engine_ObjectMotionSetPositionAndCommit(ACTOR_WISE_ONE, 184, 87);
    Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTH, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_WISE_ONE, 3);
    Task_RemoveCallback(FieldScene_RunActor15TwoStep);
    Task_Wait(1);
    Actor_SetChildValue(ACTOR_WISE_ONE, 0);
    Audio_PlayCue(141);
    Event_Wait(100);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 2, 0);
    Event_Wait(60);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 3);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(20);
    Audio_PlayCue(0x121);
    Actor_FaceDirection(ACTOR_WISE_ONE, FACING_SOUTHEAST + FACING_STEP, 0);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_WISE_ONE, 0);
    Event_Wait(20);
    for (cnt = 0; cnt < 40; cnt++) {
        FieldScene_RunStepByRuntimeBits((s32)wise_one);
        Task_Wait(1);
    }
    Task_AddCallback(FieldScene_RunActor15TwoStep, TASK_PRIORITY_SCENE);
    Event_Wait(20);
    ColorBuffer_ApplyTarget(0x7fff, 2);
    ColorBuffer_Interpolate(60);
    Task_Wait(100);
    ColorBuffer_ApplyTarget(0x7fff, 1);
    ColorBuffer_Interpolate(60);
    Task_Wait(60);
    Task_RemoveCallback(FieldScene_RunActor15TwoStep);
    quake->unknown_40c = 1;
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_MapRenderWaitForValues();
    FieldScene_CallHelper6620();
    GameFlag_Set(FLAG_SOL_SANCTUM_ERUPTED);
    GameFlag_Set(FLAG_STAR_ROOM_COLLAPSED);
    Event_RequestExit(5);
    GameFlag_Set(0x100);
}

/* Scene steps, regions and arcing effects. */
/* The IWRAM remainder, reached through an import veneer. */
void SceneActor_MoveTo232_125AndFace4000(s32 no)
{
    Ent_02002820 *rec;

    rec = (Ent_02002820 *)Object_GetById(no);
    Actor_SetPosition(no, 0xe80000, 0x7d0000);
    rec->unk6 = 0x4000;
    Actor_SetSpritePriority(no, 3);
}

void SceneState_ApplyRectsByCondition(s32 a)
{
    if (a != 0) {
        s32 x;
        s32 y;
        x = 1;
        Map_CopyCellsTo(8, 47, 64, 7, x, x);
        y = 2;
        Map_CopyCellsTo(7, 48, 63, 8, y, x);
        Map_CopyCellsTo(7, 49, 63, 9, y, x);
    } else {
        s32 x;
        x = 1;
        Map_CopyCellsTo(56, 0, 64, 7, x, x);
        Map_CopyCellsTo(56, 0, 63, 8, x, x);
        Map_CopyCellsTo(56, 0, 63, 9, 2, x);
        Map_CopyCellsTo(58, 25, 64, 8, x, x);
    }
    Map_Redraw();
}

void SceneState_ApplyRectPairByFlag(s32 a)
{
    if (a != 0) {
        s32 n;
        n = 2;
        Map_CopyCellsTo(9, 45, 65, 5, n, n);
        Map_CopyCellsTo(11, 46, 67, 6, 1, n);
    } else {
        s32 n;
        n = 2;
        Map_CopyCellsTo(89, 2, 65, 5, n, n);
        Map_CopyCellsTo(102, 32, 67, 6, 1, n);
    }
    Map_Redraw();
}

void FieldScene_RunRandomHalfBranch(void)
{
    if ((gFrameCount & 1) == 0) {
        if ((u32)IwramUnsignedRemainder(Random_Next(), 100) > 50) {
            SceneState_ApplyRectsByCondition(1);
        } else {
            SceneState_ApplyRectsByCondition(0);
        }
    }
}

void FieldScene_RunLateRandomHalfBranch(void)
{
    if ((gFrameCount & 1) == 0) {
        if ((u32)IwramUnsignedRemainder(Random_Next(), 100) > 50) {
            SceneState_ApplyRectPairByFlag(1);
        } else {
            SceneState_ApplyRectPairByFlag(0);
        }
    }
}

void FieldScene_RunFourWayEffectSequence(u32 mode)
{
    extern u8 *gArcEffects[];
    void Actor_ShowEmote(s32, s32, s32);
    void ColorBuffer_ApplyTarget(s32, s32);
    void ColorBuffer_Interpolate(s32);
    void Audio_PlayCue(s32);

    u32 i, zero;
    s32 x, y, z;
    s32 *pos;
    u8 *obj, *sprite;

    for (i = 0; i <= 15; i++)
        Actor_Destroy(i + 16);
    switch (mode) {
    case 0: ColorBuffer_ApplyTarget(0x4039d2, 1); break;
    case 1: ColorBuffer_ApplyTarget(0x4049d2, 1); break;
    case 2: ColorBuffer_ApplyTarget(0x404a4e, 1); break;
    case 3: ColorBuffer_ApplyTarget(0x403a52, 1); break;
    }
    ColorBuffer_Interpolate(60);
    Audio_PlayCue(214);
    i = 0;
    zero = i;
    for (pos = &Funka_ArcOrigins[0][0]; i <= 9; i++, pos += 2) {
        x = pos[0];
        y = pos[1];
        z = 0;
        switch (mode) {
        case 0: x += 0xe80000; z = 0x900000; break;
        case 1: x += 0xe80000; z = 0x1d00000; break;
        case 2: x += 0x2c70000; z = 0x900000; break;
        case 3: x += 0x2c70000; z = 0x1d00000; break;
        }
        gArcEffectTimers[i] = zero;
        obj = (u8 *)Engine_ObjectCreate(284, x, y, z);
        gArcEffects[i] = obj;
        obj[85] = zero;
        sprite = *(u8 **)(obj + 80);
        sprite[38] = zero;
        ((SpriteMode *)sprite)->mode = 1;
        Object_SetAnimation(obj, 6);
        Task_Wait(6);
    }
    if (mode == 0) {
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 256, 0);
        Actor_ShowEmote(ACTOR_GERALD, 256, 0);
    }
    Task_Wait(20);
    Engine_TaskAddCallback(SceneEffect_AdvanceTenEntryTimers, 3200);
    Audio_PlayCue(246);
    gArcEffectTimers[0] = 1; Task_Wait(6);
    gArcEffectTimers[1] = 1; Task_Wait(6);
    gArcEffectTimers[2] = 1; Task_Wait(6);
    gArcEffectTimers[3] = 1; Task_Wait(6);
    gArcEffectTimers[4] = 1; Task_Wait(6);
    gArcEffectTimers[5] = 1; Task_Wait(6);
    gArcEffectTimers[6] = 1; Task_Wait(6);
    gArcEffectTimers[7] = 1; Task_Wait(6);
    gArcEffectTimers[8] = 1; Task_Wait(6);
    gArcEffectTimers[9] = 1; Task_Wait(6);
    for (;;) {
        for (i = 0; i <= 9; i++) {
            if (gArcEffectTimers[i] != 0) {
                i = 888;
                break;
            }
        }
        if (i != 888)
            break;
        Task_Wait(1);
    }
    Task_Wait(40);
    Engine_TaskRemoveCallback(SceneEffect_AdvanceTenEntryTimers);
    ColorBuffer_ApplyTarget(65536, 1);
    ColorBuffer_Interpolate(40);
}

void SceneEffect_AdvanceTenEntryTimers(void)
{
    extern Ent *gArcEffects[];

    u32 i;
    s32 v;
    Ent_02002ba0 *p;

    for (i = 0; i <= 9; i++) {
        v = gArcEffectTimers[i];
        if (v != 0) {
            p = gArcEffects[i];
            if ((u32)v <= 8) {
                p->unk18 += -0x1ccc;
                p->unk1C += 0x8000;
                p->unkC += 0x4ccc;
                p->unk3C += 0x4ccc;
            } else {
                p->unkC += 0x140000;
                p->unk3C += 0x140000;
            }
            v = gArcEffectTimers[i] + 1;
            gArcEffectTimers[i] = v;
            if ((u32)v > 14) {
                gArcEffectTimers[i] = 0;
            }
        }
    }
}

void SceneActor_PlaceAtTileAndRunSteps(s32 a, s32 b)
{
    Ent_02002c1c *p;

    /* FAKEMATCH: the view-centre import is passed the tile x it ignores, as
     * the game passes it. */
    p = (Ent_02002c1c *)((s32 (*)())Engine_EventGetViewCenter)(a);
    b = b << 16;
    a = a << 16;
    Camera_MoveTo(a, -1, b, 1);
    ColorBuffer_ApplyTarget(0, 0);
    ColorBuffer_Interpolate(20);
    Task_Wait(40);
    p->unk10 = b;
    p->unk8 = a;
    p->unk38 = 0x80000000;
    p->unk40 = 0x80000000;
    p->unk24 = 0;
    p->unk2C = 0;
    Task_Wait(5);
    Map_Redraw();
    Task_Wait(5);
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(20);
    Task_Wait(30);
}

void SceneState_ConfigureEightCornerRegions(void)
{
    s32 x;
    s32 y;
    s32 z;
    s32 w;
    s32 v;

    x = 0;
    Map_CopyCellAttributes(14, 8, 1, 1, 10, x);
    Map_CopyCellAttributes(14, 28, 1, 1, 11, x);
    Map_CopyCellAttributes(44, 8, 1, 1, 12, x);
    Map_CopyCellAttributes(44, 28, 1, 1, 13, x);
    z = 14;
    y = 8;
    Map_CopyCellAttributes(13, 8, 1, 1, z, y);
    w = 28;
    Map_CopyCellAttributes(13, 28, 1, 1, z, w);
    v = 44;
    Map_CopyCellAttributes(43, 8, 1, 1, v, y);
    Map_CopyCellAttributes(43, 28, 1, 1, v, w);
}

void FieldScene_RunVariantStep(s32 a, s32 b, s32 c)
{
    if (a == 1) {
        Audio_PlayCue(0x134);
        ColorBuffer_ApplyTarget(0x203a52, 1);
    } else {
        Audio_PlayCue(0x121);
        ColorBuffer_ApplyTarget(0x10000, 1);
    }
    ColorBuffer_Interpolate(b);
    if (c != 0) {
        Event_Wait(c);
    }
}

void FieldScene_RunStepByRuntimeBits(s32 a)
{
    if ((gFrameCount & 2) != 0) {
        Object_SetPartPalettes(a, 7);
    } else {
        Object_SetPartPalettes(a, 0);
    }
    if ((gFrameCount & 15) == 0) {
        SoruFunka_SpawnEffectPair(a);
    }
}

void SceneState_ForwardByRuntimeWordBits(s32 a)
{
    volatile u32 *p = &gFrameCount;

    if (*p & 1) {
        Object_SetPartPalettes(a, (u32)IwramUnsignedRemainder(*p >> 1, 6));
    }
    if ((*p & 15) == 0) {
        SoruFunka_SpawnEffectPair(a);
    }
}

void SceneEffect_UpdateArcOverAnchor(struct Actor_02002e0c *self)
{
    struct Actor_02002e0c *anchor;
    s32 frame;
    s32 amplitude;

    anchor = self->anchor;
    self->frame = (u16)(self->frame + 1);
    frame = (s16)self->frame;

    if (frame > 31) {
        Engine_ObjectDispatchRelease(self);
        return;
    }

    amplitude = Math_Sin(frame << 10);
    self->amplitude_x = amplitude;
    self->amplitude_y = amplitude;
    self->x = anchor->x;
    self->y += 0x10000;
    self->z = anchor->z + (0x10000 - amplitude) * 5 + 0x80000;
}

void SceneEffect_UpdateAnchoredRiseArc(struct Actor_02002e5c *obj)
{
    struct Actor_02002e5c *anchor;
    s32 frame;
    s32 amp;

    anchor = obj->anchor;
    obj->frame = (u16)(obj->frame + 1);
    frame = (s16)obj->frame;

    if (frame > 31) {
        Engine_ObjectDispatchRelease(obj);
        return;
    }

    amp = Math_Sin(frame << 10);
    obj->amplitude_x = amp;
    obj->amplitude_y = -amp;
    obj->x = anchor->x;
    obj->y += 0x10000;
    obj->z = anchor->z - (0x10000 - amp) * 5 + 0x100000;
}

/* Soru volcano: spawns the linked pair of effect objects above the parent actor, with a cue, and gives them actor 15's sprite priority. */
void SoruFunka_SpawnEffectPair(union PairObject *parent)
{
    union PairObject *pair[2];
    union PairObject *child;
    struct PairSprite *part;
    struct FieldSprite *sprite;
    struct PairWork *work = gEffectWork;
    s32 i;

    Engine_AudioPlayCue(292);
    for (i = 0; i < 2; ++i) {
        child = (union PairObject *)Engine_ObjectCreate(26,
            parent->object.actor.x.fixed, parent->object.actor.y.fixed,
            parent->object.actor.z.fixed);
        pair[i] = child;
        if (child != NULL) {
            child->words[5] = parent->words[5];
            part = (struct PairSprite *)child->object.actor.sprite;
            child->object.actor.motion_flags = 0;
            child->object.effect.spin = 0;
            child->link.parent = parent;
            if (part != NULL) {
                sprite = &part->sprite;
                AnimationObjects_SelectAnimation(sprite, 0);
                sprite->flags = 0;
                Resource_ResetEntry(sprite->vram_block);
                sprite->vram_block = work->vram_block;
                /* FAKEMATCH: a plain byte access; the struct field store
                 * leaves a dead QImode zero that takes r3 from the +85
                 * address. */
                *(u8 *)&sprite->unknown_1d |= 1;
                sprite->tile = (gVramBlockCache[sprite->vram_block].offset >> 5) & 0x3ff;
                sprite->full_color = 0;
                sprite->shape = 1;
                ((struct WorldMapOam *)sprite)->size = 2;
                part->detail->field_16 = 0;
            }
        }
    }
    pair[0]->object.actor.update = (void (*)(union FieldObject *))SceneEffect_UpdateAnchoredRiseArc;
    pair[0]->object.actor.sprite->priority = Object_GetById(15)->sprite->priority;
    {
        struct FieldActor *q = Object_GetById(15);
        struct FieldActor *p = &pair[1]->object.actor;

        p->sprite->priority = q->sprite->priority;
        p->update = (void (*)(union FieldObject *))SceneEffect_UpdateArcOverAnchor;
        p->priority_flags = 2;
    }
}

/* Small scene steps. */
void SceneState_SetValue140Mode0(void)
{
    Psynergy_Begin(140, 0);
}

void FieldScene_CallHelper6620(void)
{
    BattleEffect_CleanupSceneObjects();
}

void FieldScene_RunActor15TwoStep(void)
{
    SceneState_ForwardByRuntimeWordBits((s32)Object_GetById(15));
}

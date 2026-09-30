/* Scripted scenes and the paired effects they spawn. */
#include "HAIDIA.H"
#include "CALL.H"

void HaidiaIe_RunScene015B4(void)
{

    u32 i;
    s32 record;
    s32 v5;

    Engine_AudioPlayCue(17);
    Engine_EventBegin();
    Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
    Engine_TaskWait(1);
    Engine_CameraMoveTo(0x400000, 0x900000, 0x15e0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Call3(Engine_ActorSetPosition, 0, 0x300000, 0x15a0000);
    Call3(Engine_ActorSetPosition, 25, 0x4e0000, 0x1660000);
    Call3(Engine_ActorSetPosition, 23, 0x670000, 0x1560000);
    Call3(Engine_ActorSetPosition, 24, 0x700000, 0x1680000);
    Call3(Engine_ActorFaceDirection, 23, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 24, 0x8000, 0);
    Engine_ActorSetAnimation(0, 16);
    record = Engine_ActorGet(0);
    *(s32 *)(record + 24) = -0x10000;
    record = Engine_ActorGet(0);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_ActorSetAnimation(25, 7);
    record = Engine_ActorGet(25);
    {
        u8 *motion = *(u8 **)(record + 80);
        s32 shown = 0x1555;

        *(u16 *)(motion + 30) = shown;
    }
    record = Engine_ActorGet(25);
    Engine_ActorSetSpriteFlags(record, 0);
    {
        u8 *work = (u8 *)gEventWork;

        *(s32 *)(work + 0x1c0) = 0x100;
    }
    Engine_EventOpenScreen();
    BattleFx_SetBlock30Values128One();
    Engine_EventWaitForScreen();
    Engine_EventWait(80);
    Call3(Engine_ActorFaceDirection, 23, 0x3000, 0);
    Call3(Engine_ActorFaceDirection, 24, 0xc000, 40);
    Engine_ActorSetAnimationAndWait(23, 3);
    Engine_EventWait(20);
    ((void (*)())Engine_ActorSetAnimationAndWait)(24, 3);
    Call3(Engine_ActorFaceDirection, 23, 0x8000, 10);
    Call3(Engine_ActorFaceDirection, 24, 0x8000, 10);
    Engine_ActorSetSpritePriority(0, 3);
    Engine_ActorSetSpritePriority(25, 3);
    Call3(Engine_ActorSetSpeed, 23, 0x26666, 0x13333);
    v5 = 128;
    record = Engine_ActorGet(23);
    *(s32 *)(record + 68) = 0x28f;
    *(s32 *)(record + 72) = (v5 << 8);
    Engine_ActorEnableActionCallback(23, (s32)Data_0200aa48);
    Engine_EventWait(24);
    Engine_ActorSetSpeed(24, 0x26666, 0x13333);
    record = Engine_ActorGet(24);
    *(s32 *)(record + 68) = 0x28f;
    *(s32 *)(record + 72) = (v5 << 8);
    Object_SetActionCallbackAndRefreshById(24, (s32)Data_0200ab2c);
    Engine_EventWait(40);
    BattleFx_SetBlock30ValuesMaxZero();
    BattleFx_SetBlock30Values12Zero();
    Engine_TaskWait(20);
    BattleFx_SetBlock30Values12Zero();
    Engine_TaskWait(60);
    BattleFx_SetBlock30Values12Zero();
    Engine_TaskWait(20);
    BattleFx_SetBlock30ValuesMaxZero();
    Engine_EventWait(40);
    *(s32 *)(((s32)gEventWork + 0x1c8)) = 120;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_GameFlagClear(0x834);
    Engine_EventRequestExit(9);
    Engine_EventEnd();
}

void Scene_RunExtendedActorSequence(void)
{
    extern const u8 Data_0200ac00[];

    u8 *record;
    struct EventWork *work;
    const u8 *base5_200ac00;
    s32 v5;
    const u8 *base5_200ac90;
    s32 v6;
    const u8 *base5_200adf0;

    Owner_RefreshActiveRatios(1);
    Engine_EventBegin();
    Call6(Engine_MapCopyCellsTo, 42, 53, 42, 54, 3, 1);
    Call4(Engine_CameraMoveTo, 0xb40000, 0x100000, 0x26a0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    record = Engine_ActorGet(22);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Engine_ActorGet(23);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Engine_ActorGet(24);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Engine_ActorGet(25);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Engine_ActorGet(26);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    record = Engine_ActorGet(29);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    Engine_ActorSetSpritePriority(0, 1);
    Engine_ActorSetSpritePriority(1, 1);
    Engine_ActorSetSpritePriority(17, 1);
    Engine_ActorSetSpritePriority(16, 1);
    Engine_ActorSetSpritePriority(15, 1);
    Call3(Engine_ActorSetPosition, 0, 0xd00000, 0x32e0000);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(80);
    Call3(Engine_ActorShowEmote, 12, 0x101, 40);
    SceneActor_SetPairZeroAndValue(12, 0x7000, 20);
    Call1(Engine_EventSetMessage, 0x11fa);
    Event_SayThenWait(12, 10);
    Call3(Engine_ActorShowEmote, 11, 0x102, 20);
    SceneActor_SetPairZeroAndValue(11, 0x1000, 10);
    Event_SayThenWait(11, 10);
    Engine_ActorSetAnimationAndWait(12, 3);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(11, 2);
    Event_SayThenWait(11, 10);
    Call3(Engine_ActorShowEmote, 12, 0x100, 40);
    Call3(Engine_ActorSetSpeed, 12, 0xcccc, 0x6666);
    Call3(Engine_ActorWalkToAndWait, 12, 184, 0x26a);
    SceneActor_SetPairZeroAndValue(12, 0x3000, 60);
    Event_SayThenWait(12, 20);
    Call3(Engine_ActorSetSpeed, 11, 0x10000, 0x8000);
    Call3(Engine_ActorWalkToAndWait, 11, 168, 0x26a);
    Call3(SceneActor_SetPairZeroAndValue, 11, 0xf000, 10);
    Engine_ActorSetAnimation(11, 4);
    Event_SayThenWait(11, 20);
    SceneActor_SetPairZeroAndValue(12, 0x7000, 10);
    Engine_ActorRunRepeatedMotion(12, 1);
    Engine_EventShowMessage(12, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorSetSpeed, 30, 0x26666, 0x13333);
    Call3(Engine_ActorSetPosition, 30, 0x6e0000, 0x2e80000);
    Engine_TaskWait(2);
    Engine_ActorSetAnimation(30, 3);
    Engine_ActorEnableActionCallback(30, (s32)Data_0200ac14);
    Engine_EventWait(40);
    base5_200ac00 = Data_0200ac00;
    Call3(Object_SetTargetAndCallback, 11, 0x1001e, (s32)(base5_200ac00));
    Call3(Object_SetTargetAndCallback, 12, 0x1001e, (s32)(base5_200ac00));
    Object_RefreshSelectorById(30);
    Engine_ActorStop(11);
    Engine_ActorStop(12);
    Engine_EventWait(60);
    Call3(Engine_ActorShowEmote, 11, 0x105, 0);
    Call3(Engine_ActorShowEmote, 12, 0x105, 120);
    Engine_ActorFaceDirection(11, 0x1000, 0);
    SceneActor_SetPairZeroAndValue(12, 0x7000, 80);
    SceneActor_SetPairZeroAndValue(11, 0x5000, 40);
    SceneActor_SetPairZeroAndValue(11, 0x1000, 20);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_EventWait(20);
    SceneActor_SetPairZeroAndValue(12, 0x5000, 60);
    SceneActor_SetPairZeroAndValue(12, 0x3000, 40);
    SceneActor_SetPairZeroAndValue(12, 0x5000, 60);
    Call3(Engine_ActorShowEmote, 12, 0x101, 80);
    Engine_ActorFaceDirection(11, 0x3000, 0);
    Call3(Engine_ActorWalkToAndWait, 12, 184, 0x276);
    Engine_EventWait(20);
    SceneActor_SetPairZeroAndValue(12, 0x3000, 20);
    SceneActor_SetPairZeroAndValue(12, 0x5000, 20);
    SceneActor_SetPairZeroAndValue(12, 0x3000, 20);
    Call3(Engine_ActorShowEmote, 12, 0x101, 40);
    Call3(Engine_ActorShowEmote, 11, 0x101, 40);
    Call3(Engine_ActorWalkToAndWait, 11, 168, 0x276);
    Engine_EventWait(20);
    SceneActor_SetPairZeroAndValue(11, 0x3000, 40);
    SceneActor_SetPairZeroAndValue(11, 0x5000, 40);
    SceneActor_SetPairZeroAndValue(11, 0x3000, 40);
    Call3(Engine_ActorShowEmote, 11, 0x101, 40);
    SceneActor_SetPairZeroAndValue(11, 0x1000, 10);
    Event_SayThenWait(11, 20);
    Engine_ActorSetAnimationAndWait(12, 3);
    Event_SayThenWait(12, 10);
    Call3(Engine_ActorShowEmote, 11, 0x100, 20);
    SceneActor_SetPairZeroAndValue(11, 0x5000, 20);
    SceneActor_SetPairZeroAndValue(11, 0x3000, 20);
    SceneActor_SetPairZeroAndValue(11, 0x5000, 20);
    SceneActor_SetPairZeroAndValue(11, 0x5000, 60);
    Engine_ActorSetAnimationAndWait(11, 3);
    Event_SayThenWait(11, 10);
    {
        struct FieldActor *actor = Engine_ActorGet(30);

        if (actor != NULL) {
            Engine_ActorSetPosition(31, actor->x.fixed, actor->z.fixed);
        }
    }
    v5 = 254;
    Engine_TaskWait(2);
    Engine_ActorGet(30)->priority_flags &= v5;
    Engine_ActorGet(31)->priority_flags &= v5;
    Engine_ActorSetSpritePriority(30, 2);
    Engine_ActorSetSpritePriority(31, 2);
    Call3(Engine_ActorSetSpeed, 31, 0x39999, 0x1cccc);
    Engine_ActorSetAnimation(31, 2);
    base5_200ac90 = Data_0200ac90;
    Engine_ActorEnableActionCallback(31, base5_200ac90);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(30, 3);
    Call3(Engine_ActorSetSpeed, 30, 0x4cccc, 0x26666);
    Object_SetActionCallbackAndRefreshById(30, base5_200ac90);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(12, 2);
    SceneActor_SetPairZeroAndValue(12, 0x7000, 10);
    Event_SayThenWait(12, 10);
    Engine_ActorRunRepeatedMotion(11, 1);
    Engine_EventWait(20);
    SceneActor_SetPairZeroAndValue(11, 0x1000, 10);
    Engine_ActorSetAnimationAndWait(11, 3);
    Event_SayThenWait(11, 20);
    Engine_ActorSetAnimation(12, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_EventWait(20);
    Call3(Engine_ActorSetSpeed, 11, 0x26666, 0x13333);
    Call3(Engine_ActorSetSpeed, 12, 0x26666, 0x13333);
    Engine_ActorEnableActionCallback(11, (s32)Data_0200acf8);
    Engine_EventWait(10);
    Call2(Engine_CameraSetSpeed, 0x26666, 0x4ccc);
    v6 = 0;
    Battle_GetWorkObject1e0()->motion_flags = v6;
    Call4(Engine_CameraMoveTo, 0xd70000, 0x100000, 0x3210000, 1);
    Engine_EventWait(10);
    Engine_ActorEnableActionCallback(12, (s32)Data_0200ad74);
    Object_RefreshSelectorById(12);
    SceneActor_SetPairZeroAndValue(12, 0x3000, 120);
    Engine_ActorRunRepeatedMotion(13, 2);
    Engine_EventWait(20);
    Event_SayThenWait(13, 20);
    Engine_ActorFaceDirection(0, 0, 0);
    SceneActor_SetPairZeroAndValue(1, 0x9000, 20);
    SceneActor_SetPairZeroAndValue(0, 0xc000, 10);
    SceneActor_SetPairZeroAndValue(1, 0xb000, 10);
    Engine_ActorSetAnimation(0, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(1, 3);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(16, 2);
    Engine_EventWait(20);
    Event_SayThenWait(16, 10);
    Call3(Engine_ActorSetSpeed, 16, 0x10000, 0x8000);
    Call3(Engine_ActorWalkToAndWait, 16, 216, 0x320);
    Call3(Engine_ActorFaceDirection, 16, 0x4000, 0);
    Party_GiveItem(180, 0);
    Call3(Engine_ActorWalkToAndWait, 16, 0x108, 0x320);
    Call3(Engine_ActorFaceDirection, 16, 0x6000, 0);
    Call3(Engine_ActorShowEmote, 1, 0x102, 40);
    Call3(SceneActor_SetPairZeroAndValue, 1, 0xf000, 10);
    Event_SayThenWait(1, 10);
    Engine_ActorSetAnimationAndWait(17, 4);
    Event_SayThenWait(17, 10);
    Engine_ActorFaceDirection(1, 0x1000, 0);
    Call3(Engine_ActorShowEmote, 1, 0x103, 20);
    ObjectMotion_Launch(1, 4, 60);
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventWait(20);
    SceneActor_SetPairZeroAndValue(14, 0xd000, 10);
    Event_SayThenWait(14, 60);
    Call3(Engine_ActorShowEmote, 1, 0x102, 0);
    Call3(Engine_ActorShowEmote, 16, 0x102, 0);
    Call3(Engine_ActorShowEmote, 17, 0x102, 0);
    Call3(Engine_ActorShowEmote, 18, 0x102, 0);
    Call3(Engine_ActorShowEmote, 19, 0x102, 80);
    Call3(Engine_ActorShowEmote, 17, 0x100, 0);
    Event_SayThenWait(17, 60);
    Object_LinkObjectAndSetCallback(0, 17);
    Object_LinkObjectAndSetCallback(1, 17);
    Call3(Engine_ActorWalkToAndWait, 17, 216, 0x320);
    Call3(Engine_ActorFaceDirection, 17, 0x4000, 0);
    Event_SayThenWait(17, 60);
    Party_GiveItem(207, 0);
    Engine_ActorStop(0);
    Engine_ActorStop(1);
    Call3(Engine_ActorWalkToAndWait, 17, 0x110, 0x330);
    Call3(Engine_ActorFaceDirection, 17, 0x8000, 0);
    Engine_ActorRunRepeatedMotion(1, 2);
    SceneActor_SetPairZeroAndValue(1, 0x9000, 10);
    Event_SayThenWait(1, 10);
    Engine_ActorFaceDirection(14, 0x3000, 0);
    SceneActor_SetPairZeroAndValue(0, 0, 10);
    Call3(Engine_ActorShowEmote, 0, 0x101, 60);
    Engine_ActorRunRepeatedMotion(16, 1);
    Event_SayThenWait(16, 10);
    Call3(SceneActor_SetPairZeroAndValue, 1, 0xf000, 10);
    Call3(Engine_ActorShowEmote, 1, 0x101, 20);
    Engine_ActorSetAnimationAndWait(16, 4);
    Event_SayThenWait(16, 10);
    Engine_ActorSetAnimationAndWait(18, 3);
    Engine_EventShowMessage(18, 0);
    Engine_ActorFaceDirection(1, 0xd000, 0);
    Engine_ActorSetAnimation(18, 4);
    Engine_EventShowMessage(18, 0);
    Engine_ActorStartRepeatedMotion(18, 3);
    Engine_EventShowMessage(18, 0);
    Engine_ActorSetAnimationAndWait(16, 3);
    Engine_ActorSetAnimation(19, 3);
    Engine_ActorSetAnimation(17, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(24, 3);
    Engine_ActorSetAnimation(18, 3);
    Engine_ActorSetAnimation(27, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(28, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(25, 3);
    Engine_ActorSetAnimation(20, 3);
    Engine_ActorSetAnimationAndWait(21, 3);
    ObjectMotion_Launch(15, 2, 10);
    ObjectMotion_Launch(15, 4, 40);
    Event_SayThenWait(15, 10);
    Engine_ActorFaceDirection(1, 0xb000, 0);
    SceneActor_SetPairZeroAndValue(0, 0xc000, 20);
    SceneActor_SetPairZeroAndValue(15, 0xd000, 10);
    Event_SayThenWait(15, 10);
    SceneActor_SetPairZeroAndValue(15, 0x9000, 20);
    SceneActor_SetPairZeroAndValue(15, 0x5000, 10);
    Engine_ActorSetAnimation(11, 3);
    Engine_ActorSetAnimation(14, 3);
    Engine_ActorSetAnimation(17, 3);
    Engine_ActorSetAnimation(20, 3);
    Engine_ActorSetAnimation(23, 3);
    Engine_ActorSetAnimation(26, 3);
    Engine_ActorSetAnimation(29, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(12, 3);
    Engine_ActorSetAnimation(15, 3);
    Engine_ActorSetAnimation(18, 3);
    Engine_ActorSetAnimation(21, 3);
    Engine_ActorSetAnimation(24, 3);
    Engine_ActorSetAnimation(27, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(13, 3);
    Engine_ActorSetAnimation(16, 3);
    Engine_ActorSetAnimation(19, 3);
    Engine_ActorSetAnimation(22, 3);
    Engine_ActorSetAnimation(25, 3);
    Engine_ActorSetAnimationAndWait(28, 3);
    Engine_EventWait(80);
    ObjectMotion_Launch(11, 4, 0);
    ObjectMotion_Launch(14, 4, 0);
    ObjectMotion_Launch(17, 4, 0);
    ObjectMotion_Launch(20, 4, 0);
    ObjectMotion_Launch(23, 4, 0);
    ObjectMotion_Launch(26, 4, 0);
    ObjectMotion_Launch(29, 4, 0);
    ObjectMotion_Launch(12, 4, 0);
    ObjectMotion_Launch(15, 4, 0);
    ObjectMotion_Launch(18, 4, 0);
    ObjectMotion_Launch(21, 4, 0);
    ObjectMotion_Launch(24, 4, 0);
    ObjectMotion_Launch(27, 4, 0);
    ObjectMotion_Launch(13, 4, 0);
    ObjectMotion_Launch(16, 4, 0);
    ObjectMotion_Launch(19, 4, 0);
    ObjectMotion_Launch(22, 4, 0);
    ObjectMotion_Launch(25, 4, 0);
    ObjectMotion_Launch(28, 4, 0);
    Engine_MessageShowCentered(0x1214, 1);
    v5 = 1;
    Engine_EventWait(80);
    Engine_ActorGet(0)->priority_flags |= v5;
    {
        struct FieldActor *actor = Engine_ActorGet(1);
        u8 value = (u8)(v5 | actor->priority_flags);

        actor->priority_flags = value;
    }
    Call3(Engine_ActorShowEmote, 0, 0x102, 0);
    Call3(Engine_ActorShowEmote, 1, 0x102, 80);
    SceneActor_SetPairZeroAndValue(0, 0x4000, 10);
    SceneActor_SetPairZeroAndValue(1, 0x5000, 20);
    Call3(Engine_ActorSetSpeed, 0, 0xcccc, 0x6666);
    base5_200adf0 = Data_0200adf0;
    Engine_ActorEnableActionCallback(0, base5_200adf0);
    Engine_EventWait(20);
    Call2(Engine_CameraSetSpeed, 0x6666, 0xccc);
    Engine_CameraMoveTo(0xd80000, 0x100000, 0x3890000, 1);
    Engine_EventWait(20);
    Call3(Engine_ActorSetSpeed, 1, 0xcccc, 0x6666);
    Engine_ActorEnableActionCallback(1, base5_200adf0);
    Engine_EventWait(60);
    work = *(struct EventWork **)Data_03001ebc;
    work->start_transition = 0x100;
    work->transition_frames = 60;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_ActorStop(0);
    Engine_ActorStop(1);
    Engine_EventRequestExit(10);
    Engine_EventEnd();
}

/* Shows the next line of dialogue, then holds the scene for a moment. */
void Event_SayThenWait(s32 speaker, s32 frames)
{
    Event_ShowMessage(speaker, 0);
    Event_Wait(frames);
}

void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c)
{
    Actor_FaceDirection(a, b, 0);
    Event_Wait(c);
}

void OverlayObject_UpdateOnFrameBit1(s32 obj)
{
    if ((*(volatile s32 *)&gFrameCount & 2) != 0) {
        Object_SetPartPalettes(obj, 7);
    } else {
        Object_SetPartPalettes(obj, 0);
    }
    if ((*(volatile s32 *)&gFrameCount & 15) == 0) {
        HaidiaIe_SpawnEffectPair(obj);
    }
}

void SceneEffect_UpdateByFrameBits(s32 no)
{
    volatile s32 *p = (volatile s32 *)&gFrameCount;
    if ((*p & 1) != 0) {
        s32 t = IwramUnsignedRemainder((u32)*p >> 1, 6);
        Object_SetPartPalettes(no, t);
    }
    if ((*p & 15) == 0) {
        HaidiaIe_SpawnEffectPair(no);
    }
}

void SceneEffect_UpdateByFrameBit(s32 no)
{
    volatile s32 *p = (volatile s32 *)&gFrameCount;
    if ((*p & 1) != 0) {
        s32 t = IwramUnsignedRemainder((u32)*p >> 1, 6);
        Object_SetPartPalettes(no, t);
    }
}

void SceneEffect_AnimateVerticalPositive(struct SceneVerticalEffect *effect)
{
    struct VerticalEffectAnchor *anchor;
    s32 frame;
    s32 amplitude;
    s32 offset;

    anchor = effect->anchor;
    effect->frame = effect->frame + 1;
    frame = (s16)effect->frame;
    if (frame > 31) {
        Engine_ObjectDispatchRelease(effect);
        return;
    }
    amplitude = Math_Sin(frame << 10);
    effect->amplitude_x = amplitude;
    effect->amplitude_y = amplitude;
    effect->x = anchor->x;
    offset = 0x10000;
    effect->y = effect->y + offset;
    offset = offset - amplitude;
    effect->z = anchor->z + (offset * 4 + offset) + 0x80000;
}

void SceneEffect_AnimateVerticalNegative(struct SceneVerticalEffect *effect)
{
    struct VerticalEffectAnchor *anchor;
    s32 frame;
    s32 amplitude;
    s32 offset;

    anchor = effect->anchor;
    effect->frame = effect->frame + 1;
    frame = (s16)effect->frame;
    if (frame > 31) {
        Engine_ObjectDispatchRelease(effect);
        return;
    }
    amplitude = Math_Sin(frame << 10);
    effect->amplitude_x = amplitude;
    effect->amplitude_y = -amplitude;
    effect->x = anchor->x;
    offset = 0x10000;
    effect->y = effect->y + offset;
    offset = offset - amplitude;
    effect->z = anchor->z - (offset * 4 + offset) + 0x100000;
}

/* Haidia house: spawns the linked pair of effect objects above the parent actor, with a cue, and gives the two their update routines and priorities. */
void HaidiaIe_SpawnEffectPair(union PairObject *parent)
{
    union PairObject *pair[2];
    union PairObject *child;
    struct PairSprite *part;
    struct FieldSprite *sprite;
    struct PairWork *work = Data_03001f30;
    s32 i;

    Engine_AudioPlayCue(131);
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
                sprite->tile = (ResourceTableEntries[sprite->vram_block].offset >> 5) & 0x3ff;
                sprite->full_color = 0;
                sprite->shape = 1;
                ((struct WorldMapOam *)sprite)->size = 2;
                part->detail->field_16 = 0;
            }
        }
    }
    {
        union PairObject *p = pair[0];
        struct FieldSprite *sp = p->object.actor.sprite;

        p->object.actor.update = (void (*)(union FieldObject *))SceneEffect_AnimateVerticalNegative;
        sp->priority = 2;
    }
    {
        struct FieldActor *p = &pair[1]->object.actor;
        struct FieldSprite *sp = p->sprite;

        sp->priority = 2;
        p->update = (void (*)(union FieldObject *))SceneEffect_AnimateVerticalPositive;
        p->priority_flags = 2;
    }
}

void SceneState_ApplyPair140And0(void)
{
    Psynergy_Begin(140, 0);
}

void FieldScene_Forward4dac(void)
{
    BattleEffect_CleanupSceneObjects();
}

void FieldScene_RunStep15(void)
{
    SceneEffect_UpdateByFrameBits((s32)Engine_ActorGet(15));
}

void FieldScene_RunStep17(void)
{
    SceneEffect_UpdateByFrameBits((s32)Engine_ActorGet(17));
}

void FieldScene_RunStep20(void)
{
    SceneEffect_UpdateByFrameBit((s32)Engine_ActorGet(20));
}

void SceneState_SetValues352_365_2116_2117_40(void)
{
    GameFlag_Set(352);
    GameFlag_Set(0x16d);
    GameFlag_Set(0x844);
    GameFlag_Set(0x845);
    Event_RequestExit(40);
}

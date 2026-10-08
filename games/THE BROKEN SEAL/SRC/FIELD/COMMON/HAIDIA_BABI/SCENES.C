#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "RESOURCE.H"
#include "GLYPH.H"
#include "ANIMSPR.H"
#include "VRAM_BLOCK.H"
#include "B5_CONTEXT.H"
#include "FIELD_EVENT.H"
#include "IO_REG.H"
#include "CALL.H"
#include "HAIDIA_BABI.H"
#include "TYPES.H"
#include "FIELD_EFFECT.H"
#include "FXBLEND.H"
#include "text/MSG_IDS.H"

void DisplayBlend_EnableRunScript();
void Object_RefreshSelectorById(s32 actor);
void Object_SetActionCallbackAndRefreshById(s32 actor, const void *script);

struct EventSlots {
    struct EventWork *event;
    void *unknown_04[2];
    struct FieldBlendWork *blend;
};

static __inline__ void SetBlendTarget(u32 value)
{
    REG_BLDCNT = value;
}

static __inline__ void SetBlendAlpha(u32 value)
{
    REG_BLDALPHA = value;
}



extern struct EventWork *gEventWork;
void Map_SetLayerEntryFlag();
void Map_ClearLayerEntryFlag();

void Object_SetTargetAndCallback();


void ObjectDispatch_WaitForValue16();

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

extern struct PairWork *gEffectWork;


void BattleEffect_CleanupSceneObjects(void);
void OverlayObject_UpdateOnFrameParity(union FieldObject *object);


void FieldScene_RunPaletteRampSequence(void)
{
    struct FieldActor *actor;
    struct FieldSprite *sprite;
    u32 i;
    volatile u16 *alpha;

    actor = Object_GetById(10);
    sprite = actor->sprite;
    Engine_EventBegin();
    Engine_ActorSetPosition(11, 0, 0);
    Engine_ActorSetPosition(12, 0, 0);
    Engine_ActorSetPosition(13, 0, 0);
    Engine_ActorSetPosition(14, 0, 0);
    Engine_ActorSetPosition(15, 0, 0);
    Engine_ActorSetPosition(16, 0, 0);
    Actor_SetPosition(8, PIXELS(431), PIXELS(391));
    Engine_ActorSetPosition(10, PIXELS(463), PIXELS(404));
    Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
    actor->priority_flags &= 0xfe;
    actor->motion_flags = 0;
    sprite->priority = 1;
    Engine_ActorEnableActionCallback(10, gHaidiaBabiRampActor10Action);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    Engine_MapCopyCellsTo(83, 15, 83, 19, 5, 4);
    Engine_MapCopyCellsTo(90, 16, 90, 20, 5, 4);
    Engine_MapCopyCellsTo(77, 23, 82, 23, 5, 7);
    Engine_MapCopyCellsTo(83, 33, 85, 33, 2, 2);
    Engine_MapCopyCellsTo(91, 28, 90, 28, 1, 1);
    Engine_MapCopyCellsTo(91, 28, 88, 30, 1, 1);
    Engine_MapCopyCellsTo(94, 27, 94, 23, 6, 4);
    Engine_MapCopyCellsTo(92, 28, 87, 23, 4, 4);
    Engine_MapCopyCellsTo(65, 53, 88, 24, 2, 2);
    DisplayBlend_EnableRunScript();
    SetBlendTarget(0x3f42);
    /* FAKEMATCH: needed to match: keep the initial value/port publication boundary. */
    do {
        s32 value = 0x100c;

        SetBlendAlpha(value);
    } while (0);
    BattleFx_StartTwelveFrameBlend();
    ((struct EventSlots *)&gEventWork)->blend->loud = 1;
    BattleFx_SetBlock30Values12Zero();
    Engine_TaskWait(30);
    Engine_CameraFollowActor(8, 1);
    Actor_SetSpeed(8, 0x18000, 0xc000);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
    Actor_SetSpeed(9, 0x18000, 0xc000);
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, gHaidiaBabiRampLeaderAction);
    Engine_ActorEnableActionCallback(8, gHaidiaBabiRampActor8Action);
    Engine_EventOpenScreen();
    Object_RefreshSelectorById(8);
    Engine_AudioPlayCue(158);
    Actor_ShowEmote(8, 0x100, 0);
    Engine_ActorRunRepeatedMotion(8, 2);
    Actor_FaceDirection(8, FACING_SOUTH, 10);
    Camera_SetSpeed(0x40000, 0x8000);
    Camera_MoveTo(PIXELS(414), -1, PIXELS(530), 1);
    Actor_SetPosition(9, PIXELS(414), PIXELS(530));
    Engine_ActorWalkToAndWait(9, 427, 483);
    Engine_CameraWaitForMove();
    Engine_EventSetMessage((s32)MsgHaidiaDoraHurryBoulder);
    Engine_EventShowMessageAndWait(32777, 0, 10);
    Engine_CameraSetSpeed(0x18000, 0x3000);
    Engine_CameraMoveTo(PIXELS(480), -1, PIXELS(444), 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Actor_FaceDirection(8, FACING_WEST, 20);
    Engine_ActorSetAnimation(8, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(9, 3);
    Engine_ActorWalkTo(9, 415, 589);
    Engine_EventWait(10);
    Engine_ActorEnableActionCallback(8, gHaidiaBabiRampActor8ActionB);
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, gHaidiaBabiRampLeaderActionB);
    Engine_AudioPlayCue(234);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(10, gHaidiaBabiRampActor10ActionB);
    alpha = &REG_BLDALPHA;

    i = 0;
ramp:
    *alpha = 0x100e + i;
    Engine_TaskWait(1);
    if (++i <= 3)
        goto ramp;
    Engine_AudioPlayCue(202);
    Engine_TaskWait(10);
    for (i = 0; i <= 15; i++) {
        SetBlendAlpha(0x100f - i);
        Engine_TaskWait(1);
    }
    Object_RefreshSelectorById(0);
    Engine_ActorSetAnimation(8, 1);
    Engine_ActorStartRepeatedMotion(8, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(10);
    Actor_FaceDirection(8, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
    Actor_SetAttachedEffect(8, 0x102);
    Engine_ActorSetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Engine_EventWait(80);
    Engine_ActorSetPosition(9, 0, 0);
    Engine_ActorFaceEachOther(8, 0, 20);
    Engine_ActorSetAnimation(8, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(40);
    Engine_CameraSetSpeed(0xcccc, 0x1999);
    Engine_CameraFollowActor(8, 1);
    Engine_ActorEnableActionCallback(8, gHaidiaBabiRampFinalAction);
    Object_SetActionCallbackAndRefreshById(ACTOR_PARTY_LEADER, gHaidiaBabiRampFinalAction);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 32;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(21);
}

void HaidiaBabi_RunInnkeeperTalk(void)
{
    struct FieldActor *actor;

    actor = Object_GetById(0);
    if ((u32)(actor->facing + -0x2000) > 0x9000) {
        Engine_InnOpen(0, 13);
    } else {
        Engine_EventBegin();
        if (GameFlag_IsSet(0x87a) != 0) {
            Engine_ActorRunRepeatedMotion(13, 2);
            Actor_FaceActor(13, ACTOR_PARTY_LEADER, 10);
            if (GameFlag_IsSet(0x300) == 0) {
                Engine_EventSetMessage((s32)MsgHaidiaCameBack2);
                Event_ShowMessage(13, 0);
                GameFlag_Set(0x300);
            }
            Engine_EventSetMessage((s32)MsgHaidiaHomeJustToStay);
            Event_AskYesNo(13, 0);
            Actor_FaceDirection(13, 0x9000, 10);
        } else {
            if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
                Engine_EventSetMessage((s32)MsgHaidiaTheVisitorsCausedTheEruption);
            } else {
                Engine_EventSetMessage((s32)MsgHaidiaTheThreeTravelersSeemOdd);
            }
            Event_ShowMessage(13, 0);
        }
        Engine_EventEnd();
    }
}

void SceneDialogue_ShowLine1C13WithActor16Steps(void)
{
    Engine_EventBegin();
    Engine_ActorFaceActor(0x10, ACTOR_PARTY_LEADER, 0xA);
    Engine_EventSetMessage((s32)MsgHaidiaYouveGrownSoMuch);
    Engine_EventShowMessage(0x10, 0);
    Engine_ActorFaceDirection(0x10, 0xB000, 0xA);
    Engine_GameFlagSet(0x301);
    Engine_EventEnd();
}

void SceneDialogue_RunActorThirteenDialogue(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgHaidiaDoraWouldntLetHimStay);
    Engine_EventShowMessage(0xD, 0);
    Engine_GameFlagSet(0x81C);
    Engine_EventEnd();
}

void SceneDialogue_RunActor16LineAndFlag81c(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgHaidiaDoraWasStruckWithIllness);
    Engine_EventShowMessage(0x10, 0);
    Engine_GameFlagSet(0x81C);
    Engine_EventEnd();
}

void HaidiaBabi_RunSickbedVisit(void)
{
    struct FieldActor *record;
    s32 msg;

    Engine_EventBegin();
    Actor_SetSpeed(0, 0x9999, 0x4ccc);
    Actor_WalkToAndWait(0, 0x239, 0x189);
    Actor_FaceDirection(0, 0x4000, 40);
    Engine_ActorStartRepeatedMotion(8, 2);
    Engine_EventSetMessage((s32)MsgHaidiaCoughCoughWho);
    Engine_EventShowMessageAndWait(8, 0, 80);
    Actor_ShowEmote(8, 0x101, 60);
    Engine_ActorStartRepeatedMotion(8, 1);
    Engine_EventShowMessageAndWait(8, 0, 60);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(80);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Engine_ActorSetDestination(8, 0x248, 0x196);
    Map_SetLayerEntryFlag(11);
    Map_ClearLayerEntryFlag(12);
    Engine_ActorSetAnimation(8, 12);
    Engine_EventWait(80);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(40);
    Engine_EventShowMessageAndWait(8, 0, 40);
    Actor_ShowEmote(8, 0x108, 40);
    Engine_EventShowMessageAndWait(8, 0, 40);
    Actor_ShowEmote(0, 0x105, 60);
    Engine_ActorSetAnimationAndWait(8, 13);
    Actor_ShowEmote(8, 0x103, 0);
    Engine_ActorSetAnimation(8, 11);
    Engine_EventWait(40);
    Engine_EventShowMessageAndWait(8, 0, 40);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(20);
    Engine_EventShowMessageAndWait(8, 0, 20);
    Engine_ActorSetAnimationAndWait(8, 12);
    Engine_EventWait(20);
    Actor_ShowEmote(0, 0x102, 60);
    Engine_ActorSetAnimation(8, 13);
    Engine_EventOpenMessage(8, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        gEventWork->message += 1;
    }
    if (GameFlag_IsSet(0x81c) != 0) {
        Actor_ShowEmote(8, 0x102, 60);
    }
    Engine_EventWait(20);
    Engine_EventShowMessage(8, 0);
    Actor_ShowEmote(8, 0x107, 60);
    msg = (s32)MsgHaidiaWhoToldIll;
    Engine_EventSetMessage(msg);
    Engine_EventOpenMessage(8, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        gEventWork->message += 1;
    }
    if (Engine_GameFlagIsSet(0x81c) != 0) {
        Actor_ShowEmote(8, 0x102, 60);
    }
    Engine_EventWait(20);
    Engine_EventShowMessage(8, 0);
    Engine_ActorShowEmote(8, 0x107, 60);
    Engine_EventSetMessage(msg + 3);
    Engine_EventShowMessage(8, 0);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 13);
    Engine_ActorStartRepeatedMotion(8, 2);
    Engine_EventShowMessageAndWait(8, 0, 40);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(20);
    Engine_EventShowMessageAndWait(8, 0, 40);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(40);
    record = Object_GetById(0);
    record->facing = 0;
    Engine_TaskWait(1);
    Object_GetById(0)->steering_flags &= ~ACTOR_FACES_MOTION;
    Actor_SetDestination(0, 0x22e, 0x184);
    Actor_SetSpeed(8, 0x13333, 0x9999);
    Engine_ActorSetAnimation(8, 14);
    Engine_ActorMoveToAndWait(8, 0x24a, 0x190);
    Engine_EventWait(40);
    Actor_WalkToAndWait(8, 0x244, 0x17e);
    Actor_FaceDirection(8, 0x8000, 40);
    Object_GetById(0)->steering_flags |= ACTOR_FACES_MOTION;
    Engine_ActorFaceDirection(8, 0xc000, 8);
    Engine_ActorFaceDirection(8, 0, 8);
    Actor_FaceDirection(8, 0x4000, 8);
    Actor_FaceDirection(8, 0x8000, 10);
    Engine_ActorJump(8, 4, 20);
    Engine_ActorJump(8, 6, 40);
    Engine_ActorJump(8, 4, 20);
    Engine_EventShowMessageAndWait(8, 0, 40);
    Actor_SetSpeed(8, 0x6666, 0x3333);
    Actor_WalkToAndWait(8, 0x23c, 0x180);
    Engine_EventShowMessageAndWait(8, 0, 20);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_GameFlagSet(0x81e);
    Engine_GameFlagSet(0x203);
    Engine_EventEnd();
}

void HaidiaBabi_RunActorEightMessageScene(void)
{
    s32 message;

    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x203) != 0) {
        Object_SetTargetAndCallback(8, 0x10000, (s32)gHaidiaBabiActor8Departure);
        Engine_EventWait(20);
        Engine_EventSetMessage((s32)MsgHaidiaDontWorryBest);
        Engine_EventShowMessage(8, 0);
    } else {
        Engine_ActorRunRepeatedMotion(8, 2);
        Engine_EventWait(40);
        message = (s32)MsgHaidiaUnnOhhKyle;
        Engine_EventSetMessage(message);
        Engine_EventShowMessageAndWait(8, 0, 40);
        Engine_MessageShowCentered((message + 1), 1);
    }
    Engine_EventEnd();
}

void ActorPresentation_SetTwoSceneCells(void)
{
    {
        s32 size = 2;

        Map_CopyCellsTo(22, 85, 25, 85, size, size);
    }
    {
        s32 destination = 25;

        Engine_MapCopyCellAttributes(25, 15, 2, 2, destination, destination);
    }
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    struct FieldActor *actor;
    struct FieldSprite *sprite;
    GlyphTransfer *glyph;
    u8 clear;

    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(18, 0x1e00000, 0xca0000);
    Engine_TaskWait(1);
    Engine_CameraFollowActor(18, 1);
    clear = 0;
    actor = (struct FieldActor *)Engine_ObjectCreate(22, 0x1480000, 0x20000, 0xc30000);
    actor->motion_flags = clear;
    sprite = actor->sprite;
    actor->y.fixed = 0x50000;
    sprite->part_count = clear;
    sprite->full_color = 0;
    sprite->palette = 0;
    glyph = Engine_HeapAllocate(17, sizeof(*glyph));
    Engine_ItemLoadIcon(ITEM_MYTHRIL_BAG);
    Engine_VramLoad(sprite->vram_block, 128, glyph->tiles);
    Engine_HeapRelease(17);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Engine_EventOpenScreen();
    Actor_SetSpeed(18, 0x10000, 0x8000);
    Actor_WalkToAndWait(18, 0x1e0, 176);
    Actor_WalkToAndWait(18, 0x1a4, 164);
    Actor_WalkToAndWait(18, 0x146, 185);
    Actor_FaceDirection(18, 0x4000, 10);
    Engine_ObjectSetScript(actor, gHaidiaBabiBagLiftScript);
    ObjectDispatch_WaitForValue16(actor);
    Object_SetScript(actor, gHaidiaBabiBagShowScript);
    ObjectDispatch_WaitForValue16(actor);
    Engine_EventWait(20);
    Engine_ObjectDispatchRelease(actor);
    Engine_ActorJump(18, 2, 20);
    Actor_FaceDirection(18, 0, 40);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(22);
}

void SceneEffect_UpdateObjectByFrameParity(union FieldObject *object)
{
    if ((gFrameCount & 2) != 0) {
        Engine_ObjectSetPartPalettes(&object->actor, 7);
    } else {
        Engine_ObjectSetPartPalettes(&object->actor, 0);
    }
    if ((gFrameCount & 15) == 0) {
        HaidiaBabi_SpawnEffectPair(object);
    }
}

void OverlayObject_UpdateOnFrameParity(union FieldObject *object)
{
    if ((gFrameCount & 1) != 0) {
        Engine_ObjectSetPartPalettes(&object->actor, (gFrameCount >> 1) % 6);
    }
    if ((gFrameCount & 15) == 0) {
        HaidiaBabi_SpawnEffectPair(object);
    }
}

void OverlayObject_ApplyRandomSlotOnOddFrames(union FieldObject *object)
{
    if ((gFrameCount & 1) != 0) {
        s32 slot = (gFrameCount >> 1) % 6;

        Engine_ObjectSetPartPalettes(&object->actor, slot);
    }
}

void SceneEffect_UpdateAnchoredRiseFrame(union FieldObject *object)
{
    struct FieldActor *self = &object->actor;
    struct FieldActor *anchor;
    s32 frame;
    s32 amplitude;

    anchor = &((union PairObject *)object)->link.parent->object.actor;
    self->timer = (u16)(self->timer + 1);
    frame = (s16)self->timer;

    if (frame > 31) {
        Engine_ObjectDispatchRelease(self);
        return;
    }

    amplitude = Engine_MathSin(frame << 10);
    self->scale_x = amplitude;
    self->scale_y = amplitude;
    self->x.fixed = anchor->x.fixed;
    self->y.fixed += 0x10000;
    self->z.fixed = anchor->z.fixed + (0x10000 - amplitude) * 5 + 0x80000;
}

void OverlayObject_UpdateArcFromAnchor(union FieldObject *object)
{
    struct FieldActor *obj = &object->actor;
    struct FieldActor *anchor;
    s32 frame;
    s32 amp;

    anchor = &((union PairObject *)object)->link.parent->object.actor;
    obj->timer = (u16)(obj->timer + 1);
    frame = (s16)obj->timer;

    if (frame > 31) {
        Engine_ObjectDispatchRelease(obj);
        return;
    }

    amp = Engine_MathSin(frame << 10);
    obj->scale_x = amp;
    obj->scale_y = -amp;
    obj->x.fixed = anchor->x.fixed;
    obj->y.fixed += 0x10000;
    obj->z.fixed = anchor->z.fixed - (0x10000 - amp) * 5 + 0x100000;
}

void HaidiaBabi_SpawnEffectPair(union FieldObject *object)
{
    union PairObject *parent = (union PairObject *)object;
    union PairObject *pair[2];
    union PairObject *child;
    struct AnimationObject *part;
    struct FieldSprite *sprite;
    struct PairWork *work = gEffectWork;
    s32 i;

    for (i = 0; i < 2; ++i) {
        child = (union PairObject *)Engine_ObjectCreate(26,
            parent->object.actor.x.fixed, parent->object.actor.y.fixed,
            parent->object.actor.z.fixed);
        pair[i] = child;
        if (child != NULL) {
            child->words[5] = parent->words[5];
            part = (struct AnimationObject *)child->object.actor.sprite;
            child->object.actor.motion_flags = 0;
            child->object.effect.spin = 0;
            child->link.parent = parent;
            if (part != NULL) {
                sprite = (struct FieldSprite *)part;
                Object_InitializeMode(part, 0);
                sprite->flags = 0;
                Resource_ResetEntry(sprite->vram_block);
                sprite->vram_block = work->vram_block;
                /* FAKEMATCH: needed to match: byte access avoids a dead byte zero and preserves the motion/spin zero and address registers. */
                *(u8 *)&part->display_flags |= 1;
                sprite->tile = (gVramBlockCache[sprite->vram_block].offset >> 5) & 0x3ff;
                sprite->full_color = 0;
                sprite->shape = 1;
                part->part[0].size = 2;
                part->entries[0]->frame = 0;
            }
        }
    }
    {
        union PairObject *p = pair[0];
        struct FieldSprite *sp = p->object.actor.sprite;

        p->object.actor.update = OverlayObject_UpdateArcFromAnchor;
        sp->priority = 2;
    }
    {
        struct FieldActor *p = &pair[1]->object.actor;
        struct FieldSprite *sp = p->sprite;

        sp->priority = 2;
        p->update = SceneEffect_UpdateAnchoredRiseFrame;
        p->priority_flags = 2;
    }
}

void SceneState_SetValue140Mode0(void)
{
    Engine_PsynergyBegin(0x8C, 0);
}

void FieldScene_RunSingleStep(void)
{
    BattleEffect_CleanupSceneObjects();
}

void FieldScene_RunActor8TwoStep(void)
{
    OverlayObject_UpdateOnFrameParity((union FieldObject *)Object_GetById(8));
}

void FieldScene_RunStep17(void)
{
    OverlayObject_ApplyRandomSlotOnOddFrames((union FieldObject *)Object_GetById(17));
}

void FieldScene_RunSixStepSequence17e4(void)
{
    Engine_PsynergyBegin(0x94, 1);
    Engine_PsynergySetTarget(8, 0x11);
    Engine_PsynergyRaiseHands();
    Engine_PsynergyPlayEffect(1);
    Engine_PsynergyLowerHands();
    BattleEffect_CleanupSceneObjects();
}

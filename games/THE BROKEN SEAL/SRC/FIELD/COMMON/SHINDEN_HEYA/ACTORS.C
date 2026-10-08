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
#include "MEM.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "ANIMSPR.H"
#include "FIELD_EFFECT.H"
#include "FIELD_SCENE.H"
#include "FACING_OBJECT.H"
#include "CALL.H"
#include "OBJECT_DISPATCH.H"
#include "text/MSG_IDS.H"


void ShindenHeya_SpawnOwnerEffect();


void Object_RefreshSelectorById();

enum FacingGatedMessage {
    MSG_WIELDERS_PSYNERGY_CALLED_ADEPTS_ADEPTS = 0x1035,
    MSG_WE_HAD_IDEA_TRUE_SANCTUM = 0x1138,
    MSG_ROBIN_WILL_ACCEPT_RESPONSIBILITY_FOR = 0x1162,
    MSG_ARE_YOU_SURE = 0x1164,
    MSG_ONCE_STEP_OUTSIDE_VILLAGE_CANNOT = 0x116c,
    MSG_ACCEPT_ROBIN_CANT_MEAN = 0x1171,
    MSG_MY_CONTROL_OVER_PSYNERGY_HAS = 0x119d,
    MSG_DO_FEEL_ANY_CHANGE_IN = 0x119f,
    MSG_WE_WILL_HELP_ANYTIME_AS = 0x1288,
    MSG_HEALER_MUST_WORRIED_ABOUT_NEVER = 0x1289,
    MSG_WONDER_IF_EVER_SEE_OUR = 0x128b,
    MSG_WAS_HAND_FATE_RETURNED_GOLD = 0x1376,
    MSG_WHEN_STRAY_FROM_YOUR_WORLDLY = 0x1377,
    MSG_CHILD_HAS_AWAKENED_OUR_TEACHINGS = 0x1379,
    MSG_AM_STARTING_FEEL_ONLY_BEGINNING = 0x1408,
    MSG_CURSE_MAY_OVER_BUT_WE = 0x171c,
    MSG_CAME_XIAN_FROM_VERY_DISTANT = 0x1823,
    MSG_WAS_AFTER_EERIE_NIGHT_WHEN = 0x190a,
    MSG_SAVED_ALTIN_FROM_MONSTERS_CLEARLY = 0x1951,
    MSG_PATH_SOL_SANCTUM_STILL_CLOSED = 0x1bfc,
    MSG_ROBIN_YOUR_NEW_FRIENDS_ADEPTS = 0x1bfd,
    MSG_MAY_WRONG_BUT_LATELY_THERE = 0x1ce8,
    MSG_POLISHED_GOLD_STATUE_RETURNED_US = 0x1ce9,
    MSG_DIRTY_GOLDEN_STATUE_CLEANED_UP = 0x1ceb
};


void WaitFrames();
void Ui_PrepareTransferForItem();



s32 Math_RemainderUnsigned();

extern s16 Data_02000240[];
void FieldScene_RunSupplementalSequenceOne(void);

void FieldScene_RunPairedActorChoreography(void)
{

    Actor_SetSpeed(0, 0x18000, 0xc000);
    Actor_SetSpeed(1, 0x18000, 0xc000);
    Engine_ActorRunRepeatedMotion(12, 2);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(12, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(15);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, GERALD, 0);
    Engine_ActorStartRepeatedMotion(0, 1);
    ((struct FacingObject *)Object_GetById(0))->facing_flags &= ~1;
    Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 184, 168);
    ((struct FacingObject *)Object_GetById(1))->facing_flags &= ~1;
    Engine_ActorWalkToAndWait(1, 200, 168);
    Engine_EventWait(1);
    ((struct FacingObject *)Object_GetById(1))->facing_flags |= 1;
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    ((struct FacingObject *)Object_GetById(0))->facing_flags |= 1;
    ((struct FacingObject *)Object_GetById(1))->facing_flags |= 1;
    Engine_ActorJump(GERALD, 2, 0);
    Engine_EventWait(15);
    Engine_ActorFaceActor(GERALD, 8, 0);
    Engine_EventWait(5);
    Engine_ActorJump(GERALD, 2, 0);
    Engine_EventWait(25);
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    Engine_ActorFaceActor(GERALD, 12, 0);
    Engine_EventWait(5);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(5);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(10);
    Engine_ActorFaceActor(GERALD, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Engine_EventWait(15);
    Engine_ActorSetAnimation(11, 3);
    Engine_ActorSetAnimation(12, 3);
    Engine_ActorSetAnimation(8, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimationAndWait(10, 3);
    Engine_EventWait(20);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 12, 0);
    Engine_ActorFaceActor(GERALD, 12, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Engine_EventWait(20);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 11, 0);
    Engine_ActorFaceActor(GERALD, 11, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Engine_EventWait(15);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(10);
    ((void (*)())ShindenHeya_RaiseItemIcon)(222, 0xb80000, 0x1b0000, 0xa80000);
    Engine_ActorFaceActor(GERALD, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(GERALD, 1);
    Engine_EventWait(10);
    Engine_ActorJump(GERALD, 4, 0);
    Actor_FaceDirection(1, 0xd000, 0);
    Engine_EventWait(15);
    Actor_FaceDirection(1, 0xb000, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(1, 0xd000, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(1, 0xb000, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(1, 0xd000, 0);
    Engine_EventWait(30);
    Engine_ActorJump(GERALD, 4, 0);
    Actor_FaceDirection(1, 0x3000, 0);
    Engine_EventWait(15);
    Actor_FaceDirection(1, 0x5000, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(1, 0x3000, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(1, 0x5000, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(1, 0x3000, 0);
    Engine_EventWait(30);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(0, 0x102, 0);
    Actor_ShowEmote(1, 0x102, 0);
    Engine_EventWait(60);
    Engine_ActorFaceActor(GERALD, 12, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Engine_EventWait(10);
    Object_GetById(8)->timer = 1;
    Object_GetById(8)->update =
        (void (*)(union FieldObject *))UpdateFacingFromResolvedObject;
    Object_GetById(12)->timer = 1;
    Object_GetById(12)->update =
        (void (*)(union FieldObject *))UpdateFacingFromResolvedObject;
    Engine_ActorWalkToAndWait(1, 196, 180);
    Engine_ActorWalkToAndWait(GERALD, 184, 184);
    Engine_ActorWalkToAndWait(GERALD, 180, 180);
    Engine_ActorWalkToAndWait(GERALD, 168, 168);
    Engine_ActorWalkToAndWait(GERALD, 180, 156);
    Engine_ActorWalkTo(GERALD, 200, 104);
    Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 192, 168);
    Actor_FaceDirection(0, 0xc000, 0);
    Engine_ActorWaitForMove(GERALD);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(GERALD, 1);
    Engine_EventWait(10);
    Actor_FaceDirection(1, 0x5000, 0);
    Engine_EventWait(15);
    Object_GetById(12)->update = NULL;
    Object_GetById(8)->update = NULL;
    Engine_ActorStartRepeatedMotion(8, 2);
    Engine_ActorShowEmote(8, 0x100, 0);
    Engine_EventWait(60);
    Engine_ActorSetAnimation(8, 0);
    Engine_ActorShowEmote(0, 0x102, 0);
    Engine_EventWait(60);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 11, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_EventWait(10);
    Actor_FaceDirection(0, 0xc000, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(10);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
    Engine_EventWait(20);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
    Engine_EventWait(20);
    Engine_EventWait(15);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 12, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(12, 3);
    Engine_EventWait(60);
    Engine_ActorWalkToAndWait(GERALD, 208, 168);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 11, 0);
    Engine_ActorFaceActor(GERALD, 12, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 4);
    Engine_ActorSetAnimationAndWait(GERALD, 4);
    Engine_EventWait(10);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorRunRepeatedMotion(GERALD, 1);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(GERALD, 0, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Engine_EventWait(10);
    Engine_ActorFaceActor(GERALD, 12, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(12, 3);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Engine_ActorFaceDirection(GERALD, 0, 0);
    Engine_EventWait(10);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    Engine_EventWait(10);
    Engine_ActorEnableActionCallback(0, ShindenHeya_LeaderCircleScript);
    Engine_ActorEnableActionCallback(1, ShindenHeya_GeraldCircleScript);
    Object_RefreshSelectorById(0);
    Object_RefreshSelectorById(1);
    Actor_SetSpeed(0, 0x18000, 0xc000);
    Actor_SetSpeed(1, 0x18000, 0xc000);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
    Engine_ActorJump(GERALD, 6, 0);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 9, 0);
    Engine_ActorFaceActor(GERALD, 8, 0);
    Engine_EventWait(1);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 12, 0);
    Engine_ActorFaceActor(GERALD, 11, 0);
    Engine_EventWait(1);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Engine_ActorFaceActor(GERALD, 9, 0);
    Engine_EventWait(1);
    Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 192, 168);
    Engine_ActorWalkToAndWait(GERALD, 208, 168);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Engine_EventWait(10);
    Actor_FaceDirection(0, 0x3000, 0);
    Actor_FaceDirection(1, 0xd000, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(0, 0x5000, 0);
    Actor_FaceDirection(1, 0xb000, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(0, 0x3000, 0);
    Engine_ActorFaceDirection(1, 0xd000, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(0, 0x5000, 0);
    Engine_ActorFaceDirection(1, 0xb000, 0);
    Engine_EventWait(20);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 11, 0);
    Engine_ActorFaceActor(GERALD, 12, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(11, 3);
    Engine_ActorSetAnimationAndWait(12, 3);
    Engine_EventWait(30);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GERALD, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(GERALD, 0, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
    Engine_ActorSetAnimation(GERALD, 2);
    Engine_EventWait(60);
}

void ShindenHeya_RaiseItemIcon(s32 item)
{
    struct FieldActor *obj;
    struct FieldSprite *spr;
    u8 *buf;
    u32 i;
    u8 zero;
    s32 z;
    u8 *flag;

    obj = ((struct FieldActor *(*)())Engine_ObjectCreate)(22);
    zero = 0;
    if (obj != 0) {
        ObjectDispatch_Initialize((s32)obj, (s32)ShindenHeya_ItemIconGrowScript);
        spr = obj->sprite;
        spr->flags = zero;
        spr->part_count = zero;
        spr->full_color = 0;
        spr->palette = 0;
        obj->velocity_y = 0x20000;
        ((struct FieldEffect *)obj)->velocity_y = 0x4000;
        buf = Runtime_AllocateHeapBlock(17, 0x608);
        Ui_PrepareTransferForItem(item);
        VramBlock_LoadCached(spr->vram_block, 128, buf + 0x400);
        Runtime_ReleaseHeapBlock(17);
        /* FAKEMATCH: this initializer order keeps the sprite zero in r5 instead of r3. */
        for (i = 0, flag = &obj->motion_flags, z = 0; i < 60; i++) {
            if ((u32)(obj->velocity_y + 255) <= 0x1fe)
                *flag = z;
            WaitFrames(1);
        }
        ObjectDispatch_Initialize((s32)obj, (s32)ShindenHeya_ItemIconEndScript);
    }
}

void ShindenHeya_UpdateRisingSpark(union FieldObject *object)
{
    struct FieldActor *spark = &object->actor;
    s32 scale;
    s32 timer;

    spark->y.fixed += (s16)spark->timer << 12;
    spark->target_y = spark->y.fixed;
    scale = 0;
    switch ((u16)((s16)spark->work >> 2) & 3) {
    case 0:
        scale = 0x10000;
        break;
    case 1:
    case 3:
        scale = 0xcccc;
        break;
    case 2:
        scale = 0x9999;
        break;
    }
    spark->scale_x = scale;
    spark->scale_y = scale;
    timer = spark->work - 1;
    spark->work = timer;
    if ((s16)timer <= 0)
        Engine_ObjectSetScript(spark, ShindenHeya_SparkEndScript);
}

void ShindenHeya_SpawnActorSpark(s32 id)
{
    struct FieldActor *actor;
    struct FieldActor *obj;
    struct FieldSprite *spr;
    s32 x;
    s32 r;

    actor = Object_GetById(id);
    if (actor == 0)
        return;
    r = Math_RemainderUnsigned(Engine_RandomNext(), 20);
    x = actor->x.fixed;
    x += r << 16;
    x += -0xa0000;
    obj = Engine_ObjectCreate(0x11e, x, actor->y.fixed + ((Engine_RandomNext() & 15) << 16) + -0x80000, actor->z.fixed);
    if (obj == 0)
        return;
    spr = obj->sprite;
    obj->motion_flags = 0;
    obj->timer = Math_RemainderUnsigned(Engine_RandomNext(), 10) + 5;
    obj->work = Math_RemainderUnsigned(Engine_RandomNext(), 60) + 30;
    obj->update = ShindenHeya_UpdateRisingSpark;
    spr->flags = 0;
    spr->priority = actor->sprite->priority;
}

void SceneState_ApplyTwoRects(void)
{
    {
        s32 width = 3, height = 2;
        Map_CopyCellsTo(0, 64, 11, 68, width, height);
    }
    {
        s32 x = 11, y = 8;
        Engine_MapCopyCellAttributes(11, 10, 3, 2, x, y);
    }
    Engine_TaskWait(1);
}

s32 IsActorFacingInward(void)
{
    struct FieldActor *actor = GetActorState(0);

    if ((u32)((actor->facing + 0x5fff) << 16) <= 0x3ffe0000) {
        return 1;
    }
    return 0;
}

void FieldScene_RunActorEightFacingDialogue(void)
{
    if (IsActorFacingInward() != 0) {
        Engine_SanctumOpen(8);
        return;
    }

    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x87a) != 0)
        Engine_EventSetMessage((s32)MsgShindenPathSolSanctumStillClosed);
    else if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0)
        Engine_EventSetMessage((s32)MsgShindenMyControlOverPsynergyHas);
    else
        Engine_EventSetMessage((s32)MsgShindenWieldersPsynergyCalledAdeptsAdepts);
    Engine_EventShowMessage(8, 0);
    Engine_EventEnd();
}

void FieldScene_DispatchBySceneId(void)
{
    s32 no;

    if (IsActorFacingInward() != 0) {
        Engine_SanctumOpen(8);
        return;
    }

    Engine_EventBegin();

    no = gGameState.entrance;

    switch (no) {
    case 10:
    case 12:
        if (Engine_GameFlagIsSet(0x855) != 0) {
            Engine_EventSetMessage((s32)MsgShindenWasHandFateReturnedGold);
        } else {
            Engine_EventSetMessage((s32)MsgShindenWeWillHelpAnytimeAs);
        }
        break;
    case 11:
        Engine_EventSetMessage((s32)MsgShindenMayWrongButLatelyThere);
        break;
    case 20:
    case 21:
    case 50:
        Engine_EventEnd();
        FieldScene_RunSupplementalSequenceOne();
        return;
    default:
        break;
    }

    Engine_EventShowMessage(8, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActorEightFlaggedDialogue(void)
{
    if (IsActorFacingInward() != 0) {
        Engine_SanctumOpen(8);
        return;
    }

    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x845) != 0)
        Engine_EventSetMessage((s32)MsgShindenCurseMayOverButWe);
    else
        Engine_EventSetMessage((s32)MsgShindenAmStartingFeelOnlyBeginning);
    Engine_EventShowMessage(8, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActorEightFollowupDialogue(void)
{
    if (IsActorFacingInward() != 0) {
        Engine_SanctumOpen(8);
        return;
    }

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgShindenWasAfterEerieNightWhen);
    if (Engine_GameFlagIsSet(0x909) != 0)
        Engine_EventSetMessage((s32)MsgShindenSavedAltinFromMonstersClearly);
    Engine_EventShowMessage(8, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActorEightDialogue(void)
{
    if (IsActorFacingInward() != 0) {
        Engine_SanctumOpen(8);
        return;
    }

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgShindenCameXianFromVeryDistant);
    Engine_EventShowMessage(8, 0);
    Engine_EventEnd();
}

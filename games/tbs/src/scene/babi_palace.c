#include "game_state.h"
#include "keystate.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/babi_palace.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/shop/shop.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "io_reg.h"
#include "field_service.h"
#include "field_scene.h"
#include "babi_palace.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "field_event.h"
#include "item_ids.h"
#include "objcmd.h"

struct OwnerState
{
    u8  unk00[0x34];
    u16 maxHp;
    u16 maxPp;
    u16 hp;
    u16 pp;
};

static __inline__ void Party_RestoreAll(void)
{
    s16                owners[8];
    struct OwnerState *state;
    s32                cnt;
    s32                i;

    cnt = FieldParty_ListActiveOwners(owners);
    for (i = 0; i < cnt; i++)
    {
        state = UnitGet(owners[i]);
        state->hp = state->maxHp;
        state->pp = state->maxPp;
        Engine_OwnerRecalculateRatios(owners[i]);
    }
    Party_AddActiveOwner(1);
    Party_AddActiveOwner(2);
    Party_AddActiveOwner(3);
    Engine_InventorySnapshotRestore();
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
    if (gGameState.scene == (s32)&SceneId_BabiPalace2)
    {
        return gBabiPalaceEntrances2;
    }
    return gBabiPalaceEntrancesOther;
}

s32 BabiPalace_GetRegions(void)
{
    return (s32)gBabiPalace_EmptyTable;
}

s32 BabiPalace_GetExits(void)
{
    return (s32)gBabiPalace_MessageTable;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == (s32)&SceneId_BabiPalace2)
    {
        return gBabiPalacePlacements2;
    }
    if (Engine_GameFlagIsSet(0x950) != 0)
    {
        return gBabiPalacePlacementsAfterColosso;
    }
    if (Engine_GameFlagIsSet(0x962) != 0)
    {
        return gBabiPalacePlacementsColosso;
    }
    return gBabiPalacePlacementsOther;
}

static void FieldScene_RunBranchedSteps1FF1(s32 a)
{
    s32 k = MsgTolbiRunBabisSoldiers;

    Engine_EventSetMessage(k);
    Engine_EventOpenMessage(a, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0)
        Engine_EventSetMessage(k + 1);
    else
        Engine_EventSetMessage(k + 2);
    Engine_EventShowMessage(a, 0);
}

static void RunOpeningAuxiliarySequence(s32 a)
{

    struct FieldActor *ret;
    s16                v;
    s32                c;
    s32                t;

    ret = Actor_Get(ACTOR_PARTY_LEADER);
    v = (ret->facing + 0x2000) & 0xc000;
    Event_Begin();
    Battle_ResetEffectCounter();
    if (GameFlag_IsSet(512) == 0)
    {
        GameFlag_Set(512);
        GameFlag_Clear(0x969);
        Event_SetMessage(MsgTolbiWeHaveJustEnoughExtra);
        Event_ShowMessage(a, 0);
        Event_Wait(10);
        t = v << 16;
        c = 0x4000;
        if (t == (0x4000 << 16))
        {
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 40, 104);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
        }
        Actor_SetSpeed(a, 0x10000, 0x8000);
        Actor_WalkByAndWait(a, 0, -48);
        Actor_WalkByAndWait(a, 64, 0);
        Actor_FaceDirection(a, c, 0);
    }
    else
    {
        GameFlag_Clear(512);
        GameFlag_Set(0x969);
        Actor_FaceDirection(a, FACING_SOUTH, 0);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 96);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
        Event_Wait(20);
        c = MsgTolbiUseFourBeds;
        Event_SetMessage(c);
        Event_OpenMessage(a, 0);
        if (Event_ChooseYesNo(0, 0) == 0)
        {
            Event_SetMessage(c + 1);
            Event_ShowMessage(a, 0);
        }
        else
        {
            Event_SetMessage(c + 2);
            Event_ShowMessage(a, 0);
        }
        Event_Wait(10);
        Actor_SetAnimationAndWait(a, 3);
        Event_Wait(20);
        Actor_WalkByAndWait(a, -64, 0);
        Actor_WalkByAndWait(a, 0, 48);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
        Actor_SetPosition(a, 56 << 16, 120 << 16);
#endif
    }
    Event_End();
}

static void BabiPalace_OfferRest(s32 a0)
{
    s32 base6_2241;

    Event_Begin();
    Battle_ResetEffectCounter();
    if (GameFlag_IsSet(0x966) == 0)
    {
        GameFlag_Set(0x966);
        GameFlag_Set(0x967);
        Actor_FaceDirection(a0, FACING_SOUTH, 0);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 96);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
        Event_Wait(20);
        base6_2241 = MsgTolbiCameRestBefore;
        Event_SetMessage(base6_2241);
        Event_OpenMessage(a0, 0);
        if (Event_ChooseYesNo(0, 0) == 0)
        {
            Event_Wait(10);
            Event_SetMessage((base6_2241 + 1));
        }
        else
        {
            Event_SetMessage((base6_2241 + 2));
        }
        Event_ShowMessage(a0, 0);
        Event_Wait(10);
        Actor_SetAnimationAndWait(a0, 3);
        Event_Wait(20);
        Actor_SetSpeed(a0, 0x10000, 0x8000);
        Actor_WalkByAndWait(a0, -64, 0);
        Actor_WalkByAndWait(a0, 0, 48);
    }
    else
    {
        Event_SetMessage(MsgTolbiTiredFeelFreeRest);
        Event_OpenMessage(a0, 0);
    }
    Event_End();
}

static void FieldScene_RunBranchedSteps2006(s32 a)
{
    s32 k = MsgTolbiPlanningEnterColosso;

    Engine_EventSetMessage(k);
    Engine_EventOpenMessage(a, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0)
    {
        Engine_EventWait(10);
        Engine_ActorShowEmote(a, 0x102, 0x28);
        Engine_EventSetMessage(k + 1);
    }
    else
    {
        Engine_EventWait(10);
        Engine_ActorShowEmote(a, 0x105, 0x28);
        Engine_EventSetMessage(k + 2);
    }
    Engine_EventShowMessage(a, 0);
}

static void RunMiddleAuxiliarySequence(s32 a)
{
    struct FieldActor *obj;
    u8                *q;

    obj = Actor_Get(a);
    Event_Begin();
    q = (u8 *)gBabiPalace_MiddleActionScript;
    Actor_EnableActionCallback(a, q);
    Event_SetMessage(MsgTolbiCantFightBecauseLittleIndigestion);
    Event_ShowMessage(a, 0);
    Actor_Stop(a);
    obj->scaleY = 0x10000;
    obj->scaleX = 0x10000;
    Event_Wait(30);
    Actor_RunRepeatedMotion(a, 2);
    Event_Wait(30);
    Actor_RunRepeatedMotion(a, 2);
    Event_Wait(60);
    Event_ShowMessage(a, 0);
    Event_Wait(20);
    Actor_ShowEmote(a, 258, 60);
    Actor_RunRepeatedMotion(a, 2);
    Event_Wait(30);
    Actor_RunRepeatedMotion(a, 2);
    Event_Wait(30);
    Actor_RunRepeatedMotion(a, 2);
    Event_Wait(30);
    Actor_EnableActionCallback(a, q);
    Event_ShowMessage(a, 0);
    Actor_FaceDirection(a, FACING_NORTHEAST, 0);
    Event_Wait(10);
    obj->scaleY = 0x10000;
    obj->scaleX = 0x10000;
    Actor_EnableActionCallback(a, q);
    Event_End();
}

static void EV06_PSYNERGY(s32 unused0, s32 a1)
{
    Event_Begin();
    Event_SetMessage(MsgTolbiEvenIfEscapedBabiPalace);
    Event_ShowMessage(a1, 0);
    if (GameFlag_IsSet(0x968) == 0)
    {
        GameFlag_Set(0x968);
        Psynergy_Cancel();
        Event_Wait(50);
        Actor_ShowEmote(a1, 0x100, 70);
        Actor_FaceActor(a1, ACTOR_PARTY_LEADER, 40);
        Event_ShowMessage(a1, 0);
        Event_Wait(30);
        Actor_SetAnimationAndWait(a1, 4);
        Event_Wait(20);
        Event_ShowMessage(a1, 0);
        Actor_FaceDirection(a1, FACING_WEST, 0);
    }
    Event_End();
}

static void SceneDialogue_ShowMessage22a8Branch(s32 a)
{
    s32 k = MsgTolbiMeetBabi;

    Engine_EventSetMessage(k);
    Engine_EventOpenMessage(a, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0)
        Engine_EventSetMessage(k + 1);
    else
        Engine_EventSetMessage(k + 2);
    Engine_EventShowMessage(a, 0);
}

static void SceneDialogue_RunChoiceSequence22ab(s32 no)
{
    s32 msg = MsgTolbiEasternShoresKaragol;

    Engine_EventSetMessage(msg);
    Engine_EventOpenMessage(no, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0)
        Engine_EventSetMessage(msg + 1);
    else
        Engine_EventSetMessage(msg + 2);
    Engine_EventShowMessage(no, 0);
}

static void EV16_TALK(void)
{
    s32 msg;

    Engine_EventBegin();
    Battle_ResetEffectCounter();
    msg = MsgTolbiFoundCloakBall;
    Engine_EventSetMessage(msg);
    Engine_EventShowMessage(-1, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventWait(30);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 14, 30);
    Engine_EventOpenMessage(14, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0)
    {
        Engine_EventSetMessage(msg + 2);
        Engine_EventShowMessage(14, 0);
    }
    else
    {
        Engine_EventWait(20);
        Engine_EventSetMessage(msg + 3);
        Engine_EventShowMessage(14, 0);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
        Engine_EventWait(30);
        Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
        Engine_EventWait(30);
        Engine_ActorSetPosition(16, 0, 0);
        Engine_ItemShowFound(ITEM_CLOAK_BALL, 3);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
        Engine_PartyGiveItem(ITEM_CLOAK_BALL, 0);
        Engine_GameFlagSet(0xf31);
    }
}

static void FieldScene_RunScene3b8SequenceB(void)
{
    struct FieldActor *record;

    Event_Begin();
    Event_SetMessage(MsgTolbiGetUp);
    Actor_Get(0)->active = 0;
    Actor_Get(10)->active = 0;
    Task_Wait(1);
    REG_DISPCNT = 0x1140;
    Event_ShowMessage(-1, 0);
    REG_DISPCNT = 0x140;
    Actor_Get(0)->active = 1;
    Actor_Get(10)->active = 1;
    Actor_SetAnimation(0, 31);
    record = Actor_Get(0);
    Actor_SetSpriteFlags(record, 0);
    Actor_SetPosition(1, 0x780000, 0x680000);
    Actor_SetPosition(3, 0x680000, 0x500000);
    Actor_SetPosition(2, 0x780000, 0x780000);
    Actor_FaceDirection(1, FACING_EAST, 0);
    Actor_FaceDirection(3, FACING_EAST, 0);
    Actor_FaceDirection(2, FACING_NORTHEAST, 0);
    gEventWork->transitionFrames = 60;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Event_Wait(20);
    gEventWork->transitionFrames = 24;
    Actor_SetSpeed(3, 0x10000, 0x8000);
    Actor_WalkByAndWait(MIA, 16, 0);
    Actor_FaceDirection(3, FACING_SOUTHEAST, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(3, 2);
    Event_Wait(30);
    Event_ShowMessage(3, 0);
    Event_Wait(10);
    record = Actor_Get(0);
    record->z.fixed += -0x30000;
    record = Actor_Get(0);
    record->targetZ += -0x30000;
    Actor_SetAnimation(0, 32);
    Event_Wait(40);
    Actor_SetAnimationAndWait(0, 34);
    Event_Wait(30);
    Actor_SetAnimation(0, 33);
    Event_Wait(50);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(30);
    Event_ShowMessage(GARET, 0);
    Event_Wait(10);
    Actor_ShowEmote(0, 0x105, 60);
    Event_Wait(20);
    Actor_ShowEmote(1, 0x102, 60);
    Event_ShowMessage(1, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(1, 4);
    Event_Wait(20);
    Event_ShowMessage(1, 0);
    Event_Wait(10);
    Actor_ShowEmote(0, 0x102, 80);
    Actor_ShowEmote(2, 0x106, 60);
    Actor_FaceDirection(2, FACING_NORTH, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(2, 4);
    Event_Wait(20);
    Event_ShowMessage(2, 0);
    Event_Wait(10);
    Actor_FaceDirection(1, FACING_SOUTH, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(45);
    Actor_FaceDirection(1, FACING_EAST, 0);
    Actor_FaceDirection(2, FACING_NORTHEAST, 0);
    Event_Wait(30);
    Event_OpenMessage(1, 0);
    if (Event_ChooseYesNo(-1, 0) != 0)
    {
        Event_Wait(10);
        Actor_SetAnimationAndWait(0, 34);
        Event_Wait(20);
        Actor_SetAnimationAndWait(1, 3);
        Event_Wait(20);
        Event_ShowMessage(1, 0);
        Event_Wait(10);
        Actor_SetAnimationAndWait(0, 33);
        Event_Wait(30);
        Actor_SetAnimationAndWait(1, 3);
        Event_Wait(20);
        Event_ShowMessage(GARET, 0);
        gEventWork->message += 1;
    }
    else
    {
        Event_Wait(10);
        Actor_SetAnimationAndWait(0, 33);
        gEventWork->message += 2;
        Event_Wait(30);
        Actor_SetAnimationAndWait(1, 3);
        Event_Wait(20);
        Event_ShowMessage(1, 0);
    }
    Event_Wait(10);
    Actor_SetSpeed(1, 0x10000, 0x8000);
    Actor_WalkByAndWait(1, -16, 0);
    Actor_FaceDirection(1, FACING_EAST, 0);
    Event_Wait(35);
    Actor_Jump(0, 6, 0);
    Actor_SetSpeed(0, 0x1e666, 0xf333);
    Actor_WalkByAndWait(0, -32, 0);
    record = Actor_Get(0);
    Actor_SetSpriteFlags(record, 1);
    Actor_FaceDirection(3, FACING_SOUTH, 0);
    Actor_FaceDirection(2, FACING_NORTH, 0);
    Event_Wait(40);
    Actor_SetAnimation(0, 3);
    Event_Wait(30);
    Actor_SetAnimation(2, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimationAndWait(3, 3);
    Event_Wait(30);
    Actor_SetSpeed(1, 0x13333, 0x9999);
    Actor_SetSpeed(3, 0x13333, 0x9999);
    Actor_SetSpeed(2, 0x13333, 0x9999);
    Actor_SetAnimation(1, 2);
    record = Actor_Get(0);
    if (record != 0)
    {
        Engine_ActorSetDestination(1, record->x.part.pixel, record->z.part.pixel);
    }
    Actor_WaitForMove(1);
    Actor_SetPosition(1, 0, 0);
    Actor_SetAnimation(3, 2);
    record = Actor_Get(0);
    if (record != 0)
    {
        Engine_ActorSetDestination(3, record->x.part.pixel, record->z.part.pixel);
    }
    Actor_WaitForMove(3);
    Actor_SetPosition(3, 0, 0);
    Actor_SetAnimation(2, 2);
    record = Actor_Get(ACTOR_PARTY_LEADER);
    if (record != 0)
    {
        Engine_ActorSetDestination(2, record->x.part.pixel, record->z.part.pixel);
    }
    Actor_WaitForMove(2);
    Actor_SetPosition(2, 0, 0);
    Event_Wait(10);
    Event_End();
}

static void EV70_SEARCH(void)
{
    struct FieldActor *record;
    s16                dir;
    u16                facing;

    record = Actor_Get(0);
    dir = (record->facing + 0x2000) & -0x4000;
    Event_Begin();
    Battle_ResetEffectCounter();
    Event_SetMessage(MsgTolbiLikeSleep);
    Event_OpenMessage(-1, 0);
    if (Event_ChooseYesNo(0, 0) != 0)
    {
    }
    else
    {
        if (GameFlag_IsSet(0x96a) != 0)
        {
            Event_Wait(20);
            Inn_PlaySleep(0);
            return;
        }
        Event_Wait(20);
        facing = dir;
        if (facing == 0)
        {
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 128, 120);
        }
        if (facing == 0x8000)
        {
            Actor_WalkToAndWait(0, 240, 120);
        }
        Actor_WalkToAndWait(0, 184, 120);
        Actor_FaceDirection(0, FACING_EAST, 0);
        Event_Wait(10);
        FieldMotion_LaunchFromFocusedObject(1, 16, 0, 0x8000);
        Object_RefreshSelectorById(1);
        Event_Wait(10);
        Event_ShowMessage(1, 0);
        Event_Wait(10);
        Actor_SetAnimationAndWait(0, 3);
        Event_Wait(20);
        Actor_SetSpeed(1, 0x10000, 0x8000);
        Actor_WalkByAndWait(GARET, 40, 0);
        Actor_WalkByAndWait(1, 0, -32);
        Actor_FaceDirection(1, FACING_SOUTHWEST, 0);
        Event_Wait(20);
        Actor_FaceDirection(0, FACING_WEST, 0);
        Event_Wait(20);
        FieldMotion_LaunchFromFocusedObject(2, -16, 0, 0);
        Object_RefreshSelectorById(2);
        Event_Wait(10);
        Event_ShowMessage(2, 0);
        Event_Wait(10);
        Actor_SetAnimationAndWait(0, 3);
        Event_Wait(20);
        Actor_SetSpeed(2, 0x10000, 0x8000);
        Actor_WalkByAndWait(2, -40, 0);
        Actor_WalkByAndWait(2, 0, 40);
        Actor_FaceDirection(2, FACING_NORTHEAST, 0);
        Event_Wait(20);
        Actor_FaceDirection(0, FACING_EAST, 0);
        Event_Wait(20);
        FieldMotion_LaunchFromFocusedObject(3, 16, 0, 0x8000);
        Object_RefreshSelectorById(3);
        Event_Wait(10);
        Event_ShowMessage(3, 0);
        Event_Wait(10);
        Actor_SetAnimationAndWait(0, 3);
        Event_Wait(20);
        Actor_SetSpeed(3, 0x10000, 0x8000);
        Actor_WalkByAndWait(3, 40, 0);
        Actor_WalkByAndWait(3, 0, 40);
        Actor_FaceDirection(3, FACING_NORTHWEST, 0);
        Event_Wait(20);
        Camera_MoveTo(-1, -1, -1, 0);
        Actor_WalkByAndWait(0, -56, 0);
        Actor_WalkByAndWait(0, 0, -32);
        Actor_FaceDirection(0, FACING_SOUTHEAST, 0);
        Event_Wait(30);
        Actor_SetAnimationAndWait(3, 3);
        Event_Wait(10);
        Event_ShowMessage(3, 0);
        Actor_SetAnimationAndWait(1, 3);
        Event_Wait(10);
        if (GameFlag_IsSet(0x96a) == 0)
        {
            Event_ShowMessage(1, 0);
        }
        else
        {
            gEventWork->message++;
        }
        Actor_SetAnimationAndWait(2, 3);
        Event_Wait(10);
        if (GameFlag_IsSet(0x96a) == 0)
        {
            Event_ShowMessage(2, 0);
        }
        else
        {
            gEventWork->message++;
        }
        Event_Wait(20);
        Actor_FaceDirection(0, FACING_EAST, 0);
        Actor_FaceDirection(1, FACING_WEST, 0);
        Actor_FaceDirection(3, FACING_WEST, 0);
        Actor_FaceDirection(2, FACING_EAST, 0);
        Inn_PlaySleep(0);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
        Actor_FaceDirection(1, FACING_SOUTHWEST, 0);
        Actor_FaceDirection(3, FACING_NORTHWEST, 0);
        Actor_FaceDirection(2, FACING_NORTHEAST, 0);
        Event_Wait(20);
        Actor_SetAnimationAndWait(1, 3);
        Event_Wait(10);
        Event_ShowMessage(1, 0);
        Actor_SetAnimationAndWait(IVAN, 3);
        Event_Wait(10);
        if (GameFlag_IsSet(0x96a) == 0)
        {
            Event_ShowMessage(2, 0);
        }
        else
        {
            gEventWork->message++;
        }
        Actor_SetAnimationAndWait(3, 3);
        Event_Wait(10);
        if (GameFlag_IsSet(0x96a) == 0)
        {
            Event_ShowMessage(MIA, 0);
        }
        else
        {
            gEventWork->message++;
        }
        Actor_WalkByAndWait(1, 0, 32);
        Actor_WalkBy(1, -112, 0);
        Actor_WalkByAndWait(3, 0, -40);
        Actor_WalkBy(MIA, -112, 0);
        Event_Wait(50);
        Actor_WalkByAndWait(2, 0, -24);
        Actor_WaitForMove(GARET);
        Actor_FaceDirection(0, FACING_SOUTH, 0);
        Actor_WalkByAndWait(1, 0, -16);
        Actor_WaitForMove(3);
        Actor_FaceDirection(3, FACING_NORTH, 0);
        Event_Wait(20);
        Event_ShowMessage(GARET, 0);
        Event_Wait(10);
        Actor_SetAnimationAndWait(0, 3);
        Event_Wait(20);
        Actor_SetSpeed(1, 0x13333, 0x9999);
        Actor_SetSpeed(2, 0x13333, 0x9999);
        Actor_SetSpeed(3, 0x13333, 0x9999);
        Actor_SetAnimation(1, 2);
        record = Actor_Get(0);
        if (record != 0)
        {
            Engine_ActorSetDestination(1, record->x.part.pixel, record->z.part.pixel);
        }
        Actor_WaitForMove(GARET);
        Actor_SetPosition(1, 0, 0);
        Actor_SetAnimation(3, 2);
        record = Actor_Get(ACTOR_PARTY_LEADER);
        if (record != 0)
        {
            Engine_ActorSetDestination(3, record->x.part.pixel, record->z.part.pixel);
        }
        Actor_WaitForMove(MIA);
        Actor_SetPosition(3, 0, 0);
        Actor_SetAnimation(2, 2);
        record = Actor_Get(0);
        if (record != 0)
        {
            Engine_ActorSetDestination(2, record->x.part.pixel, record->z.part.pixel);
        }
        Actor_WaitForMove(2);
        Actor_SetPosition(2, 0, 0);
        GameFlag_Set(0x96a);
    }
    Event_End();
}

static void EV60_TOUCH(void)
{
    GameFlag_Set(2411);
    Event_Begin();
    Battle_ResetEffectCounter();
    Event_SetMessage(MsgTolbiIodemIodem);
    Actor_WalkToAndWait(0, 520, 424);
    Actor_FaceDirection(0, 57344, 0);
    Camera_MoveTo(36700160, -1, 24117248, 1);
    Camera_WaitForMove();
    Actor_SetSpeed(20, 65536, 32768);
    Actor_WalkByAndWait(20, 40, 0);
    Actor_WalkToAndWait(20, 584, 360);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 257, 40);
    Actor_FaceDirection(20, 32768, 0);
    Actor_SetSpeed(21, 131072, 65536);
    Actor_SetSpeed(22, 131072, 65536);
    Actor_WalkTo(21, 528, 352);
    Actor_WalkToAndWait(22, 528, 368);
    Task_Wait(3);
    Actor_SetAnimation(21, 1);
    Event_Wait(30);
    Event_Wait(10);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(30);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Actor_WalkByAndWait(20, -16, 0);
    Event_Wait(10);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimation(21, 4);
    Actor_SetAnimationAndWait(22, 4);
    Event_Wait(20);
    Event_Wait(10);
    Actor_ShowEmote(20, 261, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_ShowEmote(21, 258, 40);
    Actor_SetSpeed(21, 65536, 32768);
    Actor_WalkByAndWait(21, 8, 0);
    Event_Wait(20);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 257, 80);
    Event_Wait(10);
    Actor_ShowEmote(22, 258, 40);
    Actor_SetSpeed(22, 65536, 32768);
    Actor_WalkByAndWait(22, 8, 0);
    Event_Wait(20);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 4);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimation(21, 3);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(20);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(22, 4);
    Event_Wait(20);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 261, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_ShowEmote(21, 257, 40);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 258, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(22, 2);
    Event_Wait(20);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(20);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 258, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(20);
    Actor_FaceEachOther(21, 22, 60);
    Actor_FaceActor(21, 20, 0);
    Actor_FaceActor(22, 20, 0);
    Event_Wait(20);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_ShowEmote(21, 256, 40);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(30);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_SetAttachedEffect(20, 258);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(30);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_ShowEmote(21, 257, 40);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_ShowEmote(22, 257, 40);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 4);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(20);
    Actor_FaceEachOther(21, 22, 60);
    Actor_FaceActor(21, 20, 0);
    Actor_FaceActor(22, 20, 0);
    Event_Wait(20);
    Event_Wait(10);
    Actor_ShowEmote(20, 261, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(20);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(30);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_RunRepeatedMotion(22, 2);
    Event_Wait(20);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 258, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(20);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 4);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_ShowEmote(22, 256, 40);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 258, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_ShowEmote(21, 257, 40);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 4);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(20);
    Actor_FaceEachOther(21, 22, 60);
    Actor_FaceActor(21, 20, 0);
    Actor_FaceActor(22, 20, 0);
    Event_Wait(20);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAttachedEffect(21, 258);
    Actor_SetAttachedEffect(22, 258);
    Actor_StartRepeatedMotion(21, 2);
    Actor_RunRepeatedMotion(22, 2);
    Event_Wait(30);
    Event_Wait(10);
    Actor_FaceDirection(20, FACING_EAST, 0);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(21, 4);
    Event_Wait(20);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(22, 4);
    Event_Wait(20);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Actor_FaceDirection(20, 32768, 0);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(21, 3);
    Event_Wait(30);
    Event_ShowMessage(21, 0);
    Event_Wait(20);
    Actor_ShowEmote(20, 261, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(30);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(40);
    Actor_FaceEachOther(21, 22, 60);
    Actor_SetAnimationAndWait(21, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(20);
    Actor_FaceDirection(22, 16384, 0);
    Event_Wait(40);
    Actor_SetSpeed(21, 85196, 42598);
    Actor_SetSpeed(22, 85196, 42598);
    Actor_WalkBy(21, 0, 120);
    Actor_WalkByAndWait(22, 0, 120);
    Actor_SetPosition(21, 0, 0);
    Actor_SetPosition(22, 0, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(40);
    Actor_SetSpeed(20, 52428, 26214);
    Actor_WalkByAndWait(20, -16, 0);
    Event_Wait(30);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 4);
    Event_Wait(30);
    Actor_FaceDirection(20, FACING_EAST, 0);
    Event_Wait(30);
    Actor_SetSpeed(20, 65536, 32768);
    Actor_WalkByAndWait(20, 120, 0);
    Actor_WalkByAndWait(20, 60, 0);
    Actor_SetPosition(20, 0, 0);
    Event_End();
}

static void FieldScene_RunBranchingActorSequence(void)
{
    struct FieldActor *actor;
    s32                step;
    s32                pick;
    struct EventWork  *event;
    u8                *work;

    Event_Begin();
    Event_SetMessage(MsgTolbiCallsName);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 31);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetPosition(GARET, 0x680000, 0x680000);
    Actor_SetPosition(MIA, 0x580000, 0x780000);
    Actor_SetPosition(IVAN, 0x780000, 0x780000);
    Actor_FaceDirection(GARET, FACING_SOUTH, 0);
    Actor_FaceDirection(MIA, FACING_EAST, 0);
    Actor_FaceDirection(IVAN, FACING_WEST, 0);
    event = (struct EventWork *)((u8 **)&gEventWork)[0];
    event->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    event->transitionFrames = 12;
    Task_Wait(1);
    DisplayTransition_InitializeBattleEffectState(9);
    work = ((u8 **)&gEventWork)[4];
    SetHalf((u16 *)(work + 0x52a), 0);
    SetHalf((u16 *)(work + 0x534), 0x1f1f);
    SetHalf((u16 *)(work + 0x536), 1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    for (step = 1; step <= 5; step++)
    {
        Task_Wait(3);
        *(u16 *)(work + 0x52a) = step;
    }
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(30);
    for (step = 5; step <= 31; step++)
    {
        Task_Wait(3);
        *(u16 *)(work + 0x52a) = step;
    }
    SetHalf((u16 *)(work + 0x536), 31);
    event = (struct EventWork *)((u8 **)&gEventWork)[0];
    event->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    event->transitionFrames = 24;
    Event_Wait(20);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 50);
    Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 40);
    Actor_RunRepeatedMotion(GARET, 2);
    Event_Wait(30);
    Event_Wait(10);
    Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 40);
    Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
    Event_Wait(30);
    Event_ShowMessage(IVAN, 0);
    Event_Wait(10);
    Event_ShowMessage(MIA, 0);
    Actor_FaceDirection(MIA, FACING_NORTHEAST, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(MIA, 2);
    Event_Wait(30);
    Actor_SetSpeed(MIA, 0x10000, 0x8000);
    Actor_WalkByAndWait(MIA, 0, -40);
    Actor_WalkByAndWait(MIA, 32, 0);
    Actor_FaceDirection(MIA, FACING_SOUTHEAST, 0);
    Event_Wait(10);
    Event_ShowMessage(MIA, 0);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    actor->z.fixed -= 0x30000;
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    actor->targetZ -= 0x30000;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 32);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 34);
    Event_Wait(30);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 33);
    Event_Wait(40);
    Actor_SetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Event_ShowMessage(GARET, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 80);
    Actor_ShowEmote(IVAN, EMOTE_IN_FRONT, 50);
    Event_OpenMessage(IVAN, 0);
    if (Event_ChooseYesNo(-1, 0) == 0)
    {
        Event_Wait(10);
        Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 33);
        Event_Wait(20);
        Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 3, 40);
        Actor_Jump(GARET, 4, 13);
        Actor_Jump(GARET, 4, 30);
        Event_ShowMessage(GARET, 0);
        ((struct EventWork *)((u8 **)&gEventWork)[0])->message++;
    }
    else
    {
        Event_Wait(10);
        Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 34);
        Event_Wait(20);
        Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 3, 40);
        Actor_Jump(GARET, 4, 13);
        Actor_Jump(GARET, 4, 30);
        ((struct EventWork *)((u8 **)&gEventWork)[0])->message++;
        Event_ShowMessage(GARET, 0);
    }
    Event_Wait(10);
    Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(GARET, 2);
    Event_Wait(40);
    Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
    Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(GARET, ANIM_NOD);
    Event_Wait(30);
    Event_ShowMessage(GARET, 0);
    Event_Wait(10);
    Actor_FaceDirection(MIA, FACING_SOUTHWEST, 0);
    Event_Wait(50);
    Actor_FaceDirection(MIA, FACING_SOUTHEAST, 0);
    Event_Wait(35);
    Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 8, 50);
    Event_Wait(10);
    Actor_SetAnimationAndWait(MIA, ANIM_NOD);
    Event_Wait(30);
    Event_ShowMessage(MIA, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(IVAN, 2);
    Event_Wait(20);
    Event_OpenMessage(IVAN, 0);
    if (Event_ChooseYesNo(-1, 0) == 0)
    {
        Event_Wait(10);
        Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 33);
        Event_Wait(20);
        Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 7, 40);
        Event_ShowMessage(GARET, 0);
        pick = 0;
        ((struct EventWork *)((u8 **)&gEventWork)[0])->message++;
    }
    else
    {
        Event_Wait(10);
        Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 34);
        Event_Wait(20);
        Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 7, 40);
        ((struct EventWork *)((u8 **)&gEventWork)[0])->message++;
        pick = 1;
        Event_ShowMessage(GARET, 0);
    }
    Event_Wait(10);
    Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(GARET, 2);
    Event_Wait(40);
    Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
    Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(GARET, ANIM_NOD);
    Event_Wait(30);
    Event_ShowMessage(GARET, 0);
    Event_Wait(10);
    Actor_FaceDirection(MIA, FACING_SOUTHWEST, 0);
    Event_Wait(50);
    Actor_FaceDirection(MIA, FACING_SOUTHEAST, 0);
    Event_Wait(35);
    Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 8, 50);
    Event_Wait(10);
    Actor_SetAnimationAndWait(MIA, ANIM_NOD);
    Event_Wait(30);
    Event_ShowMessage(MIA, 0);
    if (pick == 0)
    {
        Event_Wait(10);
        Actor_SetAnimationAndWait(IVAN, ANIM_NOD);
        Event_Wait(30);
        Event_ShowMessage(IVAN, 0);
        Event_Wait(10);
        Actor_RunRepeatedMotion(GARET, 2);
        Event_Wait(30);
        Event_ShowMessage(GARET, 0);
        ((struct EventWork *)((u8 **)&gEventWork)[0])->message += 2;
    }
    else
    {
        ((struct EventWork *)((u8 **)&gEventWork)[0])->message += 2;
        Event_Wait(10);
        Actor_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
        Event_Wait(20);
        Event_ShowMessage(IVAN, 0);
        Event_Wait(10);
        Actor_RunRepeatedMotion(GARET, 2);
        Event_Wait(20);
        Event_ShowMessage(GARET, 0);
    }
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 60);
    Event_Wait(10);
    Actor_SetAnimationAndWait(GARET, ANIM_NOD);
    Event_Wait(30);
    Event_ShowMessage(GARET, 0);
    Event_Wait(20);
    Actor_FaceEachOther(IVAN, MIA, 40);
    Actor_FaceDirection(MIA, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
    Event_Wait(30);
    while ((gKeysHeld & KEYS_DPAD) == 0)
    {
        Task_Wait(1);
    }
    Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1e666, 0xf333);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, -32, -8);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    Actor_SetSpriteFlags(actor, 1);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(MIA, FACING_SOUTH, 0);
    Actor_FaceDirection(IVAN, FACING_NORTH, 0);
    Event_Wait(20);
    Event_Wait(10);
    Actor_RunRepeatedMotion(MIA, 2);
    Event_Wait(20);
    Event_ShowMessage(MIA, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(GARET, 2);
    Event_Wait(20);
    Event_ShowMessage(GARET, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(IVAN, ANIM_NOD);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
    Event_Wait(30);
    Event_ShowMessage(IVAN, 0);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimation(GARET, ANIM_NOD);
    Actor_SetAnimationAndWait(MIA, ANIM_NOD);
    Event_Wait(30);
    Actor_SetSpeed(GARET, 0x13333, 0x9999);
    Actor_SetSpeed(MIA, 0x13333, 0x9999);
    Actor_SetSpeed(IVAN, 0x13333, 0x9999);
    Actor_SetAnimation(GARET, ANIM_WALK);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL)
    {
        Engine_ActorSetDestination(GARET, actor->x.part.pixel, actor->z.part.pixel);
    }
    Actor_WaitForMove(1);
    Actor_SetPosition(GARET, 0, 0);
    Actor_SetAnimation(MIA, ANIM_WALK);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL)
    {
        Engine_ActorSetDestination(MIA, actor->x.part.pixel, actor->z.part.pixel);
    }
    Actor_WaitForMove(3);
    Actor_SetPosition(MIA, 0, 0);
    Actor_SetAnimation(IVAN, ANIM_WALK);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL)
    {
        Engine_ActorSetDestination(IVAN, actor->x.part.pixel, actor->z.part.pixel);
    }
    Actor_WaitForMove(2);
    Actor_SetPosition(IVAN, 0, 0);
    Event_Wait(10);
    Event_End();
}

static void FieldScene_RunMainCutsceneSequence(void)
{
    s16 *position;

    Event_PlayCue(30);
    Event_Begin();
    Event_SetMessage(MsgTolbiWaitingCompanions);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Camera_MoveTo(0xd80000, -1, 0x2e00000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Event_Wait(10);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetSpeed(14, 0xcccc, 0x6666);
    Actor_WalkByAndWait(14, 0, 16);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Engine_CameraFollowActor(0, 1);
    Camera_WaitForMove();
    Event_Wait(40);
    Actor_SetSpeed(0, 0x10000, 0x8000);
    Actor_WalkToAndWait(0, 208, 0x2f8);
    Event_Wait(10);
    Camera_MoveTo(0xd80000, -1, 0x2e00000, 1);
    FieldMotion_LaunchFromFocusedObject(1, -16, 16, 0xc000);
    FieldMotion_LaunchFromFocusedObject(3, 0, 24, 0xc000);
    FieldMotion_LaunchFromFocusedObject(2, 16, 16, 0xc000);
    Actor_WaitForMove(1);
    Event_Wait(30);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);

    if (GameFlag_IsSet(0x951) != 0)
    {
        Event_Wait(10);
        Actor_SetAnimationAndWait(20, 3);
        Event_Wait(20);
        Event_ShowMessage(20, 0);
        Event_Wait(10);
        Actor_FaceDirection(14, FACING_NORTHWEST, 0);
        Event_Wait(30);
        Actor_SetAnimationAndWait(14, 3);
        Event_Wait(20);
        Event_ShowMessage(14, 0);
        Event_Wait(10);
        Actor_SetAnimationAndWait(20, 3);
        Event_Wait(20);
        Event_ShowMessage(20, 0);
        Actor_FaceDirection(20, FACING_NORTH, 0);
        Event_Wait(20);
        Actor_SetSpeed(20, 0x10000, 0x8000);
        Actor_WalkByAndWait(20, 0, -16);
        Event_Wait(40);
        Actor_SetAnimationAndWait(20, 3);
        Event_Wait(40);
        Actor_FaceDirection(20, FACING_SOUTH, 0);
        Event_Wait(20);
        Actor_WalkByAndWait(20, 0, 32);
        Actor_FaceDirection(14, FACING_WEST, 0);
        Event_Wait(10);
        Actor_FaceDirection(20, FACING_EAST, 0);
        Event_Wait(30);
        Actor_SetAnimationAndWait(20, 3);
        Event_Wait(30);
        Actor_FaceDirection(20, FACING_SOUTH, 0);
        Event_Wait(20);
        Actor_SetSpeed(20, 0xcccc, 0x6666);
        Actor_Get(20)->steeringFlags &= ~ACTOR_FACES_MOTION;
        Actor_WalkByAndWait(20, 0, -16);
        Actor_Get(20)->steeringFlags |= ACTOR_FACES_MOTION;
        Actor_FaceDirection(14, FACING_SOUTH, 0);
        Event_Wait(40);
        Actor_SetSpeed(14, 0xcccc, 0x6666);
        Actor_WalkByAndWait(14, 0, 16);
        Event_Wait(40);
        Engine_UiTextDrawQuantity(164, 2);
        Event_ShowMessage(-1, 0);
        Item_ShowFound(164, 3);
        Party_GiveItem(164, 0);
        Actor_FaceDirection(0, FACING_NORTH, 0);
        Event_Wait(30);
        Actor_WalkByAndWait(14, 0, -16);
        Actor_FaceDirection(14, FACING_SOUTH, 0);
        Event_Wait(30);
        Actor_RunRepeatedMotion(14, 2);
        Event_Wait(20);
        Event_OpenMessage(14, 0);
        gEventWork->message += 2;
    }
    else
    {
        gEventWork->message += 5;
        Event_Wait(10);
        Actor_SetAnimationAndWait(20, 3);
        Event_Wait(20);
        Event_ShowMessage(20, 0);
        Actor_FaceDirection(14, FACING_NORTHWEST, 0);
        Event_Wait(40);
        Actor_SetAnimationAndWait(14, 3);
        Event_Wait(20);
        Actor_SetAnimationAndWait(20, 3);
        Event_Wait(30);
        Actor_FaceDirection(14, FACING_SOUTH, 0);
        Event_Wait(30);
        Actor_RunRepeatedMotion(14, 2);
        Event_Wait(20);
        Event_OpenMessage(14, 0);
    }

    if (Event_ChooseYesNo(0, 0) == 0)
    {
        Event_Wait(20);
        Actor_SetAnimationAndWait(20, 3);
        Event_Wait(20);
        Event_ShowMessage(20, 0);
        gEventWork->message++;
    }
    else
    {
        Event_Wait(10);
        Actor_SetAnimationAndWait(20, 3);
        Event_Wait(20);
        gEventWork->message++;
        Event_ShowMessage(20, 0);
    }

    Event_Wait(10);
    Actor_FaceDirection(14, FACING_NORTHWEST, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_Jump(GARET, 4, 13);
    Actor_Jump(1, 4, 30);
    Event_ShowMessage(1, 0);
    Event_Wait(10);
    Actor_FaceDirection(14, FACING_SOUTH, 0);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Actor_SetSpeed(14, 0x19999, 0xcccc);
    Actor_WalkByAndWait(20, 0, 16);
    Task_Wait(2);
    Actor_FaceDirection(20, FACING_SOUTHEAST, 0);
    Event_Wait(10);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(30);
    Actor_FaceActor(14, 20, 30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Actor_FaceEachOther(14, GARET, 0);
    Event_Wait(40);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(30);
    Actor_FaceActor(1, 2, 30);
    Event_Wait(10);
    Actor_RunRepeatedMotion(2, 2);
    Event_Wait(20);
    Actor_FaceActor(2, 1, 30);
    Event_ShowMessage(2, 0);
    Event_Wait(10);
    Actor_ShowEmote(1, 0x106, 50);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_FaceDirection(1, FACING_NORTH, 0);
    Actor_FaceDirection(2, FACING_NORTH, 0);
    Event_Wait(30);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_FaceEachOther(1, 0, 0);
    Actor_FaceEachOther(3, 2, 50);
    Actor_FaceDirection(0, FACING_NORTH, 0);
    Actor_FaceDirection(GARET, FACING_NORTH, 0);
    Actor_FaceDirection(3, FACING_NORTH, 0);
    Actor_FaceDirection(2, FACING_NORTH, 0);
    Event_Wait(30);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 4);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(2, 0x101, 40);
    Event_ShowMessage(2, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(20);
    Event_OpenMessage(14, 0);
    if (Event_ChooseYesNo(0, 0) == 0)
    {
        Event_Wait(20);
        Actor_SetAnimationAndWait(14, 3);
        Event_Wait(30);
        Event_ShowMessage(14, 0);
        gEventWork->message++;
    }
    else
    {
        Event_Wait(10);
        Actor_SetAnimationAndWait(14, 4);
        Event_Wait(20);
        gEventWork->message++;
        Event_ShowMessage(14, 0);
    }

    Event_Wait(10);
    Actor_ShowEmote(20, 0x102, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(GARET, 3);
    Actor_SetAnimation(3, 3);
    Actor_SetAnimationAndWait(IVAN, 3);
    Event_Wait(50);
    Actor_FaceActor(14, 20, 60);
    Actor_FaceDirection(14, FACING_SOUTH, 0);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 0x102, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 4);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_ShowEmote(0, 0x102, 40);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_FaceEachOther(1, 0, 50);
    Actor_FaceDirection(0, FACING_NORTH, 0);
    Actor_FaceDirection(1, FACING_NORTH, 0);
    Event_Wait(30);
    Event_Wait(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(1, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(3, 0x100, 40);
    Event_ShowMessage(3, 0);
    Event_Wait(10);
    Actor_ShowEmote(14, 0x102, 50);
    Actor_FaceActor(20, 14, 50);
    Event_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(20);
    Actor_FaceEachOther(1, 0, 0);
    Actor_FaceEachOther(3, 2, 50);
    Actor_FaceDirection(0, FACING_NORTH, 0);
    Actor_FaceDirection(1, FACING_NORTH, 0);
    Actor_FaceDirection(3, FACING_NORTH, 0);
    Actor_FaceDirection(2, FACING_NORTH, 0);
    Event_Wait(50);
    Actor_FaceDirection(20, FACING_SOUTHEAST, 0);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(20);
    Event_ShowMessage(1, 0);
    Event_Wait(20);
    Actor_FaceDirection(14, FACING_WEST, 0);
    Event_Wait(40);
    Actor_ShowEmote(14, 0x105, 60);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(2, 0x100, 40);
    Event_ShowMessage(2, 0);
    Event_Wait(10);
    Actor_FaceEachOther(14, 2, 40);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_FaceEachOther(1, 0, 0);
    Actor_FaceEachOther(3, 2, 60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(1, FACING_NORTH, 0);
    Actor_FaceDirection(3, FACING_NORTH, 0);
    Actor_FaceDirection(2, FACING_NORTH, 0);
    Event_Wait(50);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_FaceEachOther(1, 0, 40);
    Actor_RunRepeatedMotion(GARET, 2);
    Event_Wait(20);
    Event_OpenMessage(1, 0);
    if (Event_ChooseYesNo(0, 0) == 0)
    {
        Event_Wait(20);
        Event_ShowMessage(1, 0);
        gEventWork->message += 3;
    }
    else
    {
        Event_Wait(10);
        gEventWork->message++;
        Event_OpenMessage(1, 0);
        if (Event_ChooseYesNo(0, 0) == 0)
        {
            Event_Wait(20);
            Event_ShowMessage(1, 0);
            gEventWork->message++;
        }
        else
        {
            gEventWork->message++;
            Event_ShowMessage(1, 0);
        }
    }

    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_FaceDirection(0, FACING_NORTH, 0);
    Actor_FaceDirection(1, FACING_NORTH, 0);
    Event_Wait(40);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(20);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(3, 0x101, 40);
    Event_ShowMessage(3, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(2, 2);
    Event_Wait(20);
    Event_ShowMessage(2, 0);
    Event_Wait(10);
    Actor_FaceActor(20, 14, 40);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(20);
    Actor_ShowEmote(14, 0x105, 70);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(1, 0x101, 0);
    Actor_ShowEmote(MIA, 0x101, 0);
    Actor_ShowEmote(2, 0x101, 40);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(GARET, 3);
    Event_Wait(30);
    Event_ShowMessage(1, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_ShowEmote(IVAN, 0x101, 40);
    Event_ShowMessage(2, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(40);
    Actor_FaceActor(0, 2, 0);
    Actor_FaceActor(GARET, IVAN, 0);
    Actor_FaceActor(3, 2, 0);
    Actor_FaceActor(20, 2, 0);
    Event_Wait(50);
    Event_Wait(10);
    Actor_RunRepeatedMotion(2, 2);
    Event_Wait(20);
    Event_ShowMessage(2, 0);
    Event_Wait(10);
    Actor_ShowEmote(14, 0x101, 40);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimationAndWait(3, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_SetAnimationAndWait(2, 4);
    Event_Wait(20);
    Event_ShowMessage(2, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(30);
    Actor_FaceActor(20, 14, 30);
    Actor_FaceDirection(0, FACING_NORTH, 0);
    Actor_FaceDirection(1, FACING_NORTH, 0);
    Actor_FaceDirection(3, FACING_NORTH, 0);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(1, 2);
    Actor_StartRepeatedMotion(3, 2);
    Actor_RunRepeatedMotion(2, 2);
    Event_Wait(30);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_FaceDirection(14, FACING_WEST, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_FaceEachOther(1, 0, 0);
    Actor_FaceEachOther(3, 2, 50);
    Actor_FaceDirection(0, FACING_NORTH, 0);
    Actor_FaceDirection(1, FACING_NORTH, 0);
    Actor_FaceDirection(MIA, FACING_NORTH, 0);
    Actor_FaceDirection(2, FACING_NORTH, 0);
    Event_Wait(40);
    Actor_FaceDirection(14, FACING_SOUTH, 0);
    Event_Wait(30);
    Actor_ShowEmote(14, 0x102, 40);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 0x102, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(20);
    Actor_FaceEachOther(14, 20, 40);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_SetAttachedEffect(20, 0x102);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Actor_SetSpeed(20, 0x19999, 0xcccc);
    Actor_WalkByAndWait(20, 0, 24);
    Actor_FaceActor(0, 20, 0);
    Actor_FaceActor(1, 20, 0);
    Actor_FaceActor(3, 20, 0);
    Actor_FaceActor(2, 20, 0);
    Actor_FaceDirection(14, FACING_SOUTH, 0);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 0x100, 60);
    Actor_FaceActor(20, 14, 0);
    Actor_FaceDirection(0, FACING_NORTH, 0);
    Actor_FaceDirection(1, FACING_NORTH, 0);
    Actor_FaceDirection(3, FACING_NORTH, 0);
    Actor_FaceDirection(2, FACING_NORTH, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(30);
    Actor_ShowEmote(14, 0x105, 40);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 0x101, 60);
    Actor_SetSpeed(20, 0x13333, 0x9999);
    Actor_WalkByAndWait(20, 0, -24);
    Actor_FaceDirection(20, FACING_EAST, 0);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_FaceDirection(14, FACING_WEST, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 0x102, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(20);
    Event_ShowMessage(1, 0);
    Event_Wait(10);
    Actor_FaceDirection(14, FACING_SOUTH, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(3, 0x102, 40);
    Event_ShowMessage(3, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(2, 3);
    Event_Wait(30);
    Event_ShowMessage(2, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_FaceEachOther(1, 0, 0);
    Actor_FaceEachOther(3, 2, 50);
    Actor_FaceDirection(0, FACING_NORTH, 0);
    Actor_FaceDirection(GARET, FACING_NORTH, 0);
    Actor_FaceDirection(3, FACING_NORTH, 0);
    Actor_FaceDirection(2, FACING_NORTH, 0);
    Event_Wait(30);
    Event_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(20);
    Actor_ShowEmote(0, 0x101, 0);
    Actor_ShowEmote(1, 0x101, 0);
    Actor_ShowEmote(3, 0x101, 0);
    Actor_ShowEmote(IVAN, 0x101, 50);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 0x102, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_FaceEachOther(14, 20, 30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(20);
    Event_ShowMessage(1, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(20);
    Actor_FaceDirection(14, FACING_SOUTH, 0);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 0x101, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Actor_FaceDirection(14, FACING_WEST, 0);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(2, 0x101, 40);
    Event_ShowMessage(2, 0);
    Event_Wait(10);
    Actor_FaceDirection(14, FACING_SOUTH, 0);
    Actor_FaceDirection(20, FACING_SOUTHEAST, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_RunRepeatedMotion(3, 2);
    Event_Wait(20);
    Event_ShowMessage(MIA, 0);
    Event_Wait(10);
    Actor_ShowEmote(14, 0x102, 40);
    Event_ShowMessage(14, 0);
    Event_Wait(20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(3, 3);
    Actor_SetAnimationAndWait(IVAN, 3);
    Event_Wait(30);
    Event_Wait(20);
    Actor_ShowEmote(14, 0x105, 60);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(1, 0x101, 40);
    Event_ShowMessage(1, 0);
    Event_Wait(10);
    Actor_ShowEmote(14, 0x100, 40);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_FaceEachOther(1, 0, 50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(1, FACING_NORTH, 0);
    Event_Wait(30);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(3, 0x101, 40);
    Event_ShowMessage(MIA, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(0, 0x101, 0);
    Actor_ShowEmote(GARET, 0x101, 0);
    Actor_ShowEmote(2, 0x101, 0);
    Actor_ShowEmote(3, 0x101, 40);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(2, 0x100, 40);
    Event_OpenMessage(2, 0);
    if (Event_ChooseYesNo(14, 0) == 0)
    {
        Event_Wait(10);
        Event_ShowMessage(2, 0);
        gEventWork->message++;
    }
    else
    {
        Event_Wait(10);
        gEventWork->message++;
        Event_ShowMessage(2, 0);
    }

    Event_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Actor_FaceDirection(20, FACING_EAST, 0);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_FaceActor(14, 20, 40);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_FaceEachOther(1, 0, 40);
    Event_ShowMessage(1, 0);
    Actor_FaceDirection(14, FACING_SOUTH, 0);
    Actor_FaceDirection(20, FACING_SOUTHEAST, 0);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_FaceDirection(0, FACING_NORTH, 0);
    Actor_FaceDirection(1, FACING_NORTH, 0);
    Event_Wait(30);
    Actor_FaceEachOther(3, 2, 60);
    Actor_FaceDirection(3, FACING_NORTH, 0);
    Actor_FaceDirection(2, FACING_NORTH, 0);
    Event_Wait(30);
    Event_ShowMessage(3, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(2, 4);
    Event_Wait(20);
    Event_ShowMessage(2, 0);
    Event_Wait(10);
    Actor_FaceEachOther(20, 14, 60);
    Actor_FaceDirection(20, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(14, FACING_SOUTH, 0);
    Event_Wait(30);
    Actor_FaceDirection(14, FACING_SOUTH, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_OpenMessage(20, 0);
    if (Event_ChooseYesNo(0, 0) == 0)
    {
        Event_Wait(20);
        Actor_ShowEmote(1, 0x102, 40);
        Event_ShowMessage(1, 0);
        gEventWork->message++;
    }
    else
    {
        Event_Wait(10);
        gEventWork->message++;
        Event_ShowMessage(1, 0);
    }

    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 0x100, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(20);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(3, 3);
    Actor_SetAnimationAndWait(IVAN, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_ShowEmote(14, 0x100, 40);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_FaceActor(20, 14, 40);
    Event_Wait(10);
    Actor_ShowEmote(20, 0x102, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_FaceActor(14, 20, 40);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Actor_FaceDirection(14, FACING_SOUTH, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 80);
    Actor_ShowEmote(20, 0x102, 70);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_FaceEachOther(1, 0, 0);
    Actor_FaceEachOther(3, 2, 50);
    Actor_FaceDirection(0, FACING_NORTH, 0);
    Actor_FaceDirection(1, FACING_NORTH, 0);
    Actor_FaceDirection(3, FACING_NORTH, 0);
    Actor_FaceDirection(2, FACING_NORTH, 0);
    Event_Wait(30);
    Event_Wait(10);
    Actor_ShowEmote(20, 0x106, 50);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Actor_FaceDirection(20, FACING_SOUTHEAST, 0);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_ShowEmote(14, 0x108, 40);
    Event_OpenMessage(14, 0);
    if (Event_ChooseYesNo(0, 0) == 0)
    {
        Event_Wait(20);
        Actor_ShowEmote(14, 0x100, 40);
        Event_ShowMessage(14, 0);
        gEventWork->message++;
    }
    else
    {
        Event_Wait(10);
        Actor_ShowEmote(14, 0x100, 40);
        gEventWork->message++;
        Event_ShowMessage(14, 0);
    }

    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_FaceActor(14, 20, 40);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_FaceActor(20, 14, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_ShowEmote(14, 0x100, 40);
    Actor_FaceDirection(14, FACING_WEST, 0);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_ShowEmote(20, 0x100, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(30);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Actor_FaceDirection(20, FACING_SOUTHEAST, 0);
    Event_Wait(20);
    Object_LinkObjectAndSetCallback(0, 20);
    Object_LinkObjectAndSetCallback(1, 20);
    Object_LinkObjectAndSetCallback(3, 20);
    Object_LinkObjectAndSetCallback(2, 20);
    Actor_SetSpeed(20, 0x10000, 0x8000);
    Actor_WalkByAndWait(20, 0, 32);
    Actor_FaceDirection(20, FACING_EAST, 0);
    Actor_FaceDirection(14, FACING_SOUTH, 0);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(30);
    Actor_WalkByAndWait(1, 16, 0);
    Actor_WalkBy(20, 0, 80);
    Event_Wait(40);
    Actor_WalkByAndWait(GARET, -16, 0);
    Actor_FaceDirection(1, FACING_SOUTH, 0);
    Actor_FaceDirection(3, FACING_SOUTH, 0);
    Actor_WaitForMove(20);
    Event_Wait(80);
    Actor_SetPosition(20, 0, 0);
    Actor_FaceDirection(0, FACING_NORTH, 0);
    Actor_FaceDirection(1, FACING_NORTH, 0);
    Actor_FaceDirection(3, FACING_NORTH, 0);
    Actor_FaceDirection(2, FACING_NORTH, 0);
    Event_Wait(30);
    Object_LinkObjectAndSetCallback(0, 14);
    Object_LinkObjectAndSetCallback(1, 14);
    Object_LinkObjectAndSetCallback(3, 14);
    Object_LinkObjectAndSetCallback(2, 14);
    Event_Wait(30);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Actor_SetSpeed(14, 0xcccc, 0x6666);
    Actor_WalkByAndWait(14, 0, 24);
    Actor_WalkByAndWait(14, -80, 0);
    Event_Wait(10);
    Actor_FaceDirection(14, FACING_EAST, 0);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(20);
    Actor_FaceDirection(14, FACING_SOUTH, 0);
    Event_Wait(20);
    Actor_WalkByAndWait(14, 0, 48);
    Actor_WalkByAndWait(14, -64, 0);
    Actor_Stop(0);
    Actor_Stop(GARET);
    Actor_Stop(3);
    Actor_Stop(2);
    Actor_SetPosition(14, 0, 0);
    Event_Wait(20);
    Actor_FaceEachOther(0, 3, 0);
    Actor_FaceEachOther(1, 2, 0);
    Event_Wait(30);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(3, 3);
    Actor_SetAnimationAndWait(IVAN, 3);
    Event_Wait(30);
    Event_PlayCue(17);
    Actor_SetSpeed(1, 0x13333, 0x9999);
    Actor_SetSpeed(2, 0x13333, 0x9999);
    Actor_SetSpeed(MIA, 0x13333, 0x9999);
    Actor_SetAnimation(1, 2);
    position = Actor_Get(0);
    if (position != 0)
        Engine_ActorSetDestination(1, position[5], position[9]);
    Actor_WaitForMove(1);
    Actor_SetPosition(1, 0, 0);
    Actor_SetAnimation(IVAN, 2);
    position = Actor_Get(ACTOR_PARTY_LEADER);
    if (position != 0)
        Engine_ActorSetDestination(2, position[5], position[9]);
    Actor_WaitForMove(2);
    Actor_SetPosition(2, 0, 0);
    Actor_SetAnimation(3, 2);
    position = Actor_Get(0);
    if (position != 0)
        Engine_ActorSetDestination(3, position[5], position[9]);
    Actor_WaitForMove(3);
    Actor_SetPosition(3, 0, 0);
    Event_Wait(10);
    Audio_PlayCueFromEventWork();
    Event_End();
}

static void BabiPalace_LeaveThroughDoor(void)
{
    struct EventWork *work;

    work = (struct EventWork *)((u8 **)&gEventWork)[0];
    Event_Begin();
    Event_PlayCue(158);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    if (work->touchedTrigger == 32)
    {
        Map_ClearLayerEntryFlag(1);
        Event_Wait(10);
        Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
    }
    else
    {
        if (work->touchedTrigger == 30)
        {
            Map_ClearLayerEntryFlag(4);
            Event_Wait(10);
            Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
        }
        else
        {
            Map_ClearLayerEntryFlag(2);
            Event_Wait(10);
            Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
        }
    }
    Event_Wait(16);
    Event_RequestExit(work->touchedTrigger);
    Map_SetLayerEntryFlag(1);
    Map_SetLayerEntryFlag(2);
    Map_SetLayerEntryFlag(4);
    Event_End();
}

static void EV02_TOUCH(void)
{
    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 32768, 16384);
    Event_PlayCue(158);
    Map_AnimateCells(gBabiPalace_CellSteps, 36, 10);
    Actor_CenterAndWalk(0, 2, -16);
    Event_Wait(16);
    Event_RequestExit(2);
    Event_End();
}

static void RunSupplementalSequenceOne(void)
{
    s32 p;
    GameFlag_Set(2412);
    Event_Begin();
    Battle_ResetEffectCounter();
    Actor_FaceDirection(8, 20480, 0);
    Actor_FaceDirection(9, 12288, 0);
    Actor_WalkToAndWait(0, 200, 272);
    Actor_FaceDirection(0, 49152, 0);
    Event_Wait(20);
    p = MsgTolbiArent;
    Event_SetMessage(p);
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 0)
    {
        Event_Wait(20);
        Event_SetMessage(p + 1);
        Event_ShowMessage(8, 0);
    }
    else
    {
        Event_Wait(20);
        Event_SetMessage(p + 2);
        Event_ShowMessage(8, 0);
        Event_Wait(20);
        Actor_FaceEachOther(8, 9, 60);
        Actor_FaceDirection(9, 12288, 0);
        Event_Wait(40);
        Actor_RunRepeatedMotion(9, 2);
        Event_Wait(30);
        Actor_FaceEachOther(8, 9, 30);
        Actor_SetAnimationAndWait(9, 3);
        Event_Wait(30);
        Actor_ShowEmote(8, 258, 50);
        Actor_FaceDirection(8, 20480, 0);
        Actor_FaceDirection(9, 12288, 0);
        Event_Wait(20);
        Actor_SetAnimationAndWait(8, 4);
        Event_Wait(20);
        Event_ShowMessage(8, 0);
        Event_Wait(10);
        Actor_RunRepeatedMotion(8, 2);
        Event_Wait(20);
        Event_ShowMessage(8, 0);
    }
    Event_End();
}

static void SceneDialogue_ThankForSavingBabi(void)
{
    if (GameFlag_IsSet(0x96d) == 0)
    {
        GameFlag_Set(0x96d);
        Event_SetMessage(MsgTolbiIsaacIdReallyLikeThank);
        Event_ShowMessage(9, 0);
    }
    else
    {
        Event_SetMessage(MsgTolbiBabiWaitingForAtColosseum);
        Event_ShowMessage(9, 0);
    }
}

static void SceneDialogue_AskIfLeavingPalace(s32 a)
{
    s32 k = MsgTolbiWarriorsWhoStayed;

    Engine_EventSetMessage(k);
    Engine_EventOpenMessage(a, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0)
    {
        Engine_EventSetMessage(k + 1);
        Engine_EventShowMessage(a, 0);
    }
    else
    {
        Engine_EventSetMessage(k + 2);
        Engine_EventShowMessage(a, 0);
    }
}

static void EV90_SEARCH(void)
{
    Engine_EventBegin();
    Engine_MessageShowCentered(MsgFieldPeeredWell, 1);
    Engine_MessageShowCentered(MsgTolbiItsFilledWithFreshClean, 1);
    Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_BabiPalace2)
    {
        if (Engine_GameFlagIsSet(0x950) != 0)
        {
            return gBabiPalaceEvents2AfterColosso;
        }
        if (Engine_GameFlagIsSet(0x962) != 0)
        {
            return gBabiPalaceEvents2Colosso;
        }
        return gBabiPalaceEvents2;
    }
    if (Engine_GameFlagIsSet(0x950) != 0)
    {
        return gBabiPalaceEventsAfterColosso;
    }
    if (Engine_GameFlagIsSet(0x962) != 0)
    {
        return gBabiPalaceEventsColosso;
    }
    return gBabiPalaceEventsOther;
}

s32 BabiPalace_ApplyEntryState(void)
{
    struct FieldActor  *actor;
    struct FieldSprite *sprite;
    u8                 *buf;
    s32                 set;

    Map_SetLayerEntryFlag(1);
    Map_SetLayerEntryFlag(2);
    Map_SetLayerEntryFlag(4);
    if (gGameState.entrance == 90)
    {
        GameFlag_Set(0x962);
    }
    if (gGameState.entrance == 91)
    {
        GameFlag_Set(0x962);
        GameFlag_Set(0x950);
    }
    if (gGameState.scene != (s32)&SceneId_BabiPalace2)
    {
        if (gGameState.entrance == 11)
        {
            GameFlag_Clear(0x12f);
        }
        if (GameFlag_IsSet(0x950))
        {
            set = GameFlag_IsSet(0xf31);
            if (set)
            {
                Actor_SetPosition(16, 0, 0);
            }
            else
            {
                actor = Actor_Get(16);
                actor->unk5c = 1;
                actor->motionFlags = set;
                sprite = actor->sprite;
                actor->y.fixed = 0x40000;
                sprite->partCount = set;
                sprite->fullColor = 0;
                sprite->palette = 0;
                buf = Heap_Allocate(17, 0x608);
                Item_LoadIcon(205);
                Vram_Load(sprite->vramBlock, 128, buf + 0x400);
                Heap_Release(17);
            }
            if (gGameState.entrance == 33 && !GameFlag_IsSet(0x96f))
            {
                GameFlag_Set(0x96f);
                Actor_SetPosition(14, 0xd00000, 0x2c00000);
                FieldScene_RunMainCutsceneSequence();
            }
            Actor_SetAnimation(14, 5);
            Actor_SetSpriteFlags(Actor_Get(14), 0);
        }
        else if (GameFlag_IsSet(0x962) && !GameFlag_IsSet(0x966))
        {
            Actor_SetPosition(10, 0x780000, 0x480000);
        }
        gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
        Actor_Get(9)->collisionFlags |= 4;
        if (gGameState.entrance == 99)
        {
            Party_RestoreAll();
            FieldScene_RunBranchingActorSequence();
            gGameState.entrance = 8;
        }
        if (gGameState.entrance == 98)
        {
            Party_RestoreAll();
            GameFlag_Set(0x966);
            GameFlag_Set(0x967);
            Actor_SetPosition(10, 0x380000, 0x780000);
            Actor_FaceDirection(10, 0xf000, 0);
            FieldScene_RunScene3b8SequenceB();
            gGameState.entrance = 8;
        }
    }
    return 0;
}

// clang-format off
u16 gBabiPalace_CellSteps[] = {
    MAP_CELL_STEP(25, 28, 1, 3, 5), MAP_CELL_STEP(26, 28, 1, 3, 5), MAP_CELL_STEP(27, 28, 1, 3, 5), MAP_CELL_STEPS_END,
};

s32 gBabiPalace_MiddleActionScript[] = {
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -0x1000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(4),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gBabiPalaceEntrancesOther[] = {
    { 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 768, 0, 200, FACING_NORTH, 0, 512, 0, 1024, 224, 0 },
    { 2, CONDITION_ALWAYS, 728, 0, 88, FACING_EAST, 0, 512, 0, 1024, 224, 0 },
    { 3, CONDITION_ALWAYS, 808, 0, 88, FACING_WEST, 0, 512, 0, 1024, 224, 0 },
    { 4, CONDITION_ALWAYS, 600, 0, 152, FACING_EAST, 0, 512, 0, 1024, 224, 0 },
    { 5, CONDITION_ALWAYS, 936, 0, 152, FACING_WEST, 0, 512, 0, 1024, 224, 0 },
    { 6, CONDITION_ALWAYS, 584, 0, 56, FACING_SOUTH, 0, 512, 0, 1024, 224, 0 },
    { 7, CONDITION_ALWAYS, 952, 0, 56, FACING_SOUTH, 0, 512, 0, 1024, 224, 0 },
    { 8, CONDITION_ALWAYS, 72, 0, 216, FACING_NORTH, 0, 0, 0, 512, 240, 0 },
    { 9, CONDITION_ALWAYS, 440, 0, 216, FACING_NORTH, 0, 0, 0, 512, 240, 0 },
    { 10, CONDITION_ALWAYS, 72, 0, 968, FACING_EAST, 0, 0, 864, 240, 1024, 0 },
    { 11, CONDITION_ALWAYS, 168, 0, 936, FACING_SOUTH, 0, 0, 864, 240, 1024, 0 },
    { 12, CONDITION_ALWAYS, 424, 0, 680, FACING_WEST, 0, 328, 600, 568, 760, 0 },
    { 13, CONDITION_ALWAYS, 472, 0, 696, FACING_SOUTH, 0, 328, 600, 568, 760, 0 },
    { 16, CONDITION_ALWAYS, 408, 0, 824, FACING_EAST, 0, 320, 728, 560, 888, 0 },
    { 17, CONDITION_ALWAYS, 456, 0, 808, FACING_SOUTH, 0, 320, 728, 560, 888, 0 },
    { 18, CONDITION_ALWAYS, 824, 0, 648, FACING_NORTH, 0, 768, 480, 1024, 672, 0 },
    { 20, CONDITION_ALWAYS, 872, 0, 936, FACING_WEST, 0, 784, 864, 1024, 1024, 0 },
    { 21, CONDITION_ALWAYS, 888, 0, 968, FACING_WEST, 0, 784, 864, 1024, 1024, 0 },
    { 22, CONDITION_ALWAYS, 760, 0, 936, FACING_EAST, 0, 608, 864, 848, 1024, 0 },
    { 23, CONDITION_ALWAYS, 744, 0, 968, FACING_EAST, 0, 608, 864, 848, 1024, 0 },
    { 24, CONDITION_ALWAYS, 440, 0, 584, FACING_WEST, 0, 272, 240, 784, 608, 0 },
    { 25, CONDITION_ALWAYS, 616, 0, 584, FACING_EAST, 0, 272, 240, 784, 608, 0 },
    { 26, CONDITION_ALWAYS, 328, 0, 312, FACING_WEST, 0, 272, 240, 784, 608, 0 },
    { 27, CONDITION_ALWAYS, 728, 0, 312, FACING_EAST, 0, 272, 240, 784, 608, 0 },
    { 28, CONDITION_ALWAYS, 312, 0, 296, FACING_WEST, 0, 272, 240, 784, 608, 0 },
    { 29, CONDITION_ALWAYS, 744, 0, 296, FACING_EAST, 0, 272, 240, 784, 608, 0 },
    { 30, CONDITION_ALWAYS, 472, 0, 504, FACING_SOUTH, 0, 272, 240, 784, 608, 0 },
    { 31, CONDITION_ALWAYS, 584, 0, 504, FACING_SOUTH, 0, 272, 240, 784, 608, 0 },
    { 32, CONDITION_ALWAYS, 528, 0, 296, FACING_SOUTH, 0, 272, 240, 784, 608, 0 },
    { 33, CONDITION_ALWAYS, 208, 0, 840, FACING_NORTH, 0, 0, 624, 336, 864, 0 },
    { 34, CONDITION_ALWAYS, 40, 0, 760, FACING_WEST, 0, 0, 624, 336, 864, 0 },
    { 35, CONDITION_ALWAYS, 200, 0, 360, FACING_NORTH, 0, 0, 240, 256, 480, 0 },
    { 36, CONDITION_ALWAYS, 824, 0, 360, FACING_NORTH, 0, 768, 240, 1024, 480, 0 },
    { 40, CONDITION_ALWAYS, 376, 0, 936, FACING_EAST, 0, 232, 864, 472, 1024, 0 },
    { 41, CONDITION_ALWAYS, 520, 0, 936, FACING_WEST, 0, 424, 864, 664, 1024, 0 },
    { 42, CONDITION_ALWAYS, 904, 0, 776, FACING_EAST, 0, 776, 704, 1016, 864, 0 },
    { 43, CONDITION_ALWAYS, 216, 0, 552, FACING_WEST, 0, 0, 472, 304, 632, 0 },
    { 44, CONDITION_ALWAYS, 88, 0, 552, FACING_SOUTH, 0, 0, 472, 304, 632, 0 },
    { 45, CONDITION_ALWAYS, 728, 0, 824, FACING_NORTH, 0, 552, 656, 792, 848, 0 },
    { 98, CONDITION_ALWAYS, 152, 0, 104, FACING_SOUTH, 0, 0, 0, 512, 240, 0 },
    { 99, CONDITION_ALWAYS, 152, 0, 104, FACING_SOUTH, 0, 0, 0, 512, 240, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gBabiPalaceEntrances2[] = {
    { 0, CONDITION_ALWAYS, 264, 0, 440, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 264, 0, 424, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { 2, CONDITION_ALWAYS, 200, 0, 248, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

s16 gBabiPalace_EmptyTable[] = {
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gBabiPalace_MessageTable[] = {
    SCENE_EXITS(138),
    SCENE_EXIT(1, 139, 2),
    SCENE_EXIT(2, 138, 24),
    SCENE_EXIT(3, 138, 25),
    SCENE_EXIT(4, 138, 20),
    SCENE_EXIT(5, 138, 22),
    SCENE_EXIT(6, 138, 8),
    SCENE_EXIT(7, 138, 9),
    SCENE_EXIT(8, 138, 6),
    SCENE_EXIT(9, 138, 7),
    SCENE_EXIT(10, 138, 26),
    SCENE_EXIT(11, 8, 19),
    SCENE_EXIT(12, 138, 27),
    SCENE_EXIT(13, 141, 1),
    SCENE_EXIT(16, 138, 34),
    SCENE_EXIT(17, 138, 18),
    SCENE_EXIT(18, 138, 17),
    SCENE_EXIT(20, 138, 4),
    SCENE_EXIT(21, 138, 40),
    SCENE_EXIT(22, 138, 5),
    SCENE_EXIT(23, 138, 41),
    SCENE_EXIT(24, 138, 2),
    SCENE_EXIT(25, 138, 3),
    SCENE_EXIT(26, 138, 10),
    SCENE_EXIT(27, 138, 12),
    SCENE_EXIT(28, 138, 42),
    SCENE_EXIT(29, 138, 43),
    SCENE_EXIT(30, 138, 35),
    SCENE_EXIT(31, 138, 36),
    SCENE_EXIT(32, 138, 33),
    SCENE_EXIT(33, 138, 32),
    SCENE_EXIT(34, 138, 16),
    SCENE_EXIT(35, 138, 30),
    SCENE_EXIT(36, 138, 31),
    SCENE_EXIT(40, 138, 21),
    SCENE_EXIT(41, 138, 23),
    SCENE_EXIT(42, 138, 28),
    SCENE_EXIT(43, 138, 29),
    SCENE_EXIT(44, 138, 45),
    SCENE_EXIT(45, 138, 44),
    SCENE_EXIT(0, 139, 0),
    SCENE_EXIT(1, 135, 11),
    SCENE_EXIT(2, 138, 1),
    SCENE_EXITS_END,
};

struct ScenePlacement gBabiPalacePlacements2[] = {
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(248), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(248), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gBabiPalacePlacementsOther[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(752), 0, PIXELS(136), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(56), 0, PIXELS(120), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(96), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x3e, CONDITION_ALWAYS, (s32)gBabiPalace_MiddleActionScript,
        PIXELS(472), 0, PIXELS(136), FACING_NORTHEAST, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(296), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(888), 0, PIXELS(344), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(984), 0, PIXELS(440), FACING_NORTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(984), 0, PIXELS(312), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(944), 0, PIXELS(384), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(920), 0, PIXELS(560), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(112), 0, PIXELS(552), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(728), 0, PIXELS(728), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(600), 0, PIXELS(744), FACING_SOUTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x39, 0x96b, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(360), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x98, 0x96b, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(352), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x98, 0x96b, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(368), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gBabiPalacePlacementsColosso[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(712), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(472), 0, PIXELS(696), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(56), 0, PIXELS(120), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(920), 0, PIXELS(560), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(112), 0, PIXELS(552), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(728), 0, PIXELS(744), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(600), 0, PIXELS(744), FACING_SOUTHWEST, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gBabiPalacePlacementsAfterColosso[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(712), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(392), 0, PIXELS(112), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(472), 0, PIXELS(696), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(296), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(984), 0, PIXELS(440), FACING_NORTHEAST, TALK_FACE_PARTY, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(944), 0, PIXELS(384), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x34, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(888), 0, PIXELS(553), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(920), 0, PIXELS(560), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(856), 0, PIXELS(532), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(112), 0, PIXELS(552), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(712), 0, PIXELS(744), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x39, 0x96f, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(704), FACING_SOUTHEAST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gBabiPalaceEventsOther[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { 0x31, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { 0x31, 13, CONDITION_ALWAYS, 13 },
    { EVENT_EXIT, 16, CONDITION_ALWAYS, 16 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 18, CONDITION_ALWAYS, 18 },
    { EVENT_EXIT, 20, CONDITION_ALWAYS, 20 },
    { EVENT_EXIT, 21, CONDITION_ALWAYS, 21 },
    { EVENT_EXIT, 22, CONDITION_ALWAYS, 22 },
    { EVENT_EXIT, 23, CONDITION_ALWAYS, 23 },
    { EVENT_EXIT, 24, CONDITION_ALWAYS, 24 },
    { EVENT_EXIT, 25, CONDITION_ALWAYS, 25 },
    { EVENT_EXIT, 26, CONDITION_ALWAYS, 26 },
    { EVENT_EXIT, 27, CONDITION_ALWAYS, 27 },
    { EVENT_EXIT, 28, CONDITION_ALWAYS, 28 },
    { EVENT_EXIT, 29, CONDITION_ALWAYS, 29 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 30, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 32, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 33, CONDITION_ALWAYS, 33 },
    { EVENT_EXIT, 34, CONDITION_ALWAYS, 34 },
    { EVENT_EXIT, 35, CONDITION_ALWAYS, 35 },
    { EVENT_EXIT, 36, CONDITION_ALWAYS, 36 },
    { EVENT_EXIT, 40, CONDITION_ALWAYS, 40 },
    { EVENT_EXIT, 41, CONDITION_ALWAYS, 41 },
    { EVENT_EXIT, 42, CONDITION_ALWAYS, 42 },
    { EVENT_EXIT, 43, CONDITION_ALWAYS, 43 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 44, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 45, CONDITION_ALWAYS, 45 },
    { EVENT_TOUCH, 60, 0x96b, EVENT_SCRIPT(EV60_TOUCH) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunBranchedSteps1FF1) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIfYoureEnteringColossoHurryAlong) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWonderHowDiligentlyBabisSoldiers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWarriorsAreNotAllowedToStay) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(RunOpeningAuxiliarySequence) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunBranchedSteps2006) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(RunMiddleAuxiliarySequence) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWantToWatchTheColosso) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIGetSoAngrySeeingAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceMaybeIShouldntHaveEatenQuite) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceOnlyWarriorsWhoFightWellIn) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceLordBabiIsBuildingTheLighthouse) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabisNameIsRidingOnThe) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceEvenIfLordBabiDoesntReturn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceMyFirstColossoWasSoExciting) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceOnceTheLighthouseIsCompleteBabis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWedNeverGetNearTheLighthouse) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIHearThatGirlIsGoing) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceItLooksLikeLordBabiDidnt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIGuessLordBabiDidNot) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorHasBeenStayingAsLord) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorWasReveredAsAChild) },
    { EVENT_TALK, 6, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiIsHoldingMeHostageIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorWasTakenHostageToForce) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorIsNotPermittedToLeave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 6, CONDITION_ALWAYS, EVENT_SCRIPT(EV06_PSYNERGY) },
    { SEARCH(SEARCH_UNNAMED), 70, IF_FLAG_SET(0x969), EVENT_SCRIPT(EV70_SEARCH) },
    { SEARCH(SEARCH_SHELF), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheArmorLooksHeavyItLooks)) },
    { SEARCH(SEARCH_SHELF), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresNothingInThereItsCompletely)) },
    { SEARCH(SEARCH_SHELF), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresAHeadbandItSaysNever)) },
    { SEARCH(SEARCH_BOOKCASE), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledTheHeart)) },
    { SEARCH(SEARCH_BOOKCASE), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresANotebookThatShowsThe)) },
    { SEARCH(SEARCH_BOOKCASE), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledBecomeA)) },
    { SEARCH(SEARCH_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceHeyItsBabisDiaryItReads)) },
    { SEARCH(SEARCH_BARREL), 100, 0xf9f, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gBabiPalaceEventsColosso[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { 0x31, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { 0x31, 13, CONDITION_ALWAYS, 13 },
    { EVENT_EXIT, 16, CONDITION_ALWAYS, 16 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 18, CONDITION_ALWAYS, 18 },
    { EVENT_EXIT, 20, CONDITION_ALWAYS, 20 },
    { EVENT_EXIT, 21, CONDITION_ALWAYS, 21 },
    { EVENT_EXIT, 22, CONDITION_ALWAYS, 22 },
    { EVENT_EXIT, 23, CONDITION_ALWAYS, 23 },
    { EVENT_EXIT, 24, CONDITION_ALWAYS, 24 },
    { EVENT_EXIT, 25, CONDITION_ALWAYS, 25 },
    { EVENT_EXIT, 26, CONDITION_ALWAYS, 26 },
    { EVENT_EXIT, 27, CONDITION_ALWAYS, 27 },
    { EVENT_EXIT, 28, CONDITION_ALWAYS, 28 },
    { EVENT_EXIT, 29, CONDITION_ALWAYS, 29 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 30, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 32, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 33, CONDITION_ALWAYS, 33 },
    { EVENT_EXIT, 34, CONDITION_ALWAYS, 34 },
    { EVENT_EXIT, 35, CONDITION_ALWAYS, 35 },
    { EVENT_EXIT, 36, CONDITION_ALWAYS, 36 },
    { EVENT_EXIT, 40, CONDITION_ALWAYS, 40 },
    { EVENT_EXIT, 41, CONDITION_ALWAYS, 41 },
    { EVENT_EXIT, 42, CONDITION_ALWAYS, 42 },
    { EVENT_EXIT, 43, CONDITION_ALWAYS, 43 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 44, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 45, CONDITION_ALWAYS, 45 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabisSoldiersHaveGoneToSee) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWhatAreYouDoingHurryOver) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceThoseGuysLookReallyConfidentAbout) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheFinalsAreAllThatsLeft) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_OfferRest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIHopeYoullAllDoWell) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiHasReturnedAndTheFinals) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWhenBabiReturnedHeHeadedOff) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWhatsGoingOnActorIsLooking) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheSoldiersCameToSeeActor) },
    { EVENT_TALK, 6, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceHmphIWishColossoWouldHurry) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceASmileOnAPrettyGirl) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorIsAGoodGirlBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 6, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIDontHaveTheLeastInterest) },
    { SEARCH(SEARCH_UNNAMED), 70, IF_FLAG_SET(0x967), EVENT_SCRIPT(EV70_SEARCH) },
    { SEARCH(SEARCH_SHELF), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheArmorLooksHeavyItLooks)) },
    { SEARCH(SEARCH_SHELF), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresNothingInThereItsCompletely)) },
    { SEARCH(SEARCH_SHELF), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresAHeadbandItSaysNever)) },
    { SEARCH(SEARCH_BOOKCASE), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledTheHeart)) },
    { SEARCH(SEARCH_BOOKCASE), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresANotebookThatShowsThe)) },
    { SEARCH(SEARCH_BOOKCASE), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledBecomeA)) },
    { SEARCH(SEARCH_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceHeyItsBabisDiaryItReads)) },
    { SEARCH(SEARCH_BARREL), 100, 0xf9f, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gBabiPalaceEventsAfterColosso[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { 0x31, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { 0x31, 13, CONDITION_ALWAYS, 13 },
    { EVENT_EXIT, 16, CONDITION_ALWAYS, 16 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 18, CONDITION_ALWAYS, 18 },
    { EVENT_EXIT, 20, CONDITION_ALWAYS, 20 },
    { EVENT_EXIT, 21, CONDITION_ALWAYS, 21 },
    { EVENT_EXIT, 22, CONDITION_ALWAYS, 22 },
    { EVENT_EXIT, 23, CONDITION_ALWAYS, 23 },
    { EVENT_EXIT, 24, CONDITION_ALWAYS, 24 },
    { EVENT_EXIT, 25, CONDITION_ALWAYS, 25 },
    { EVENT_EXIT, 26, CONDITION_ALWAYS, 26 },
    { EVENT_EXIT, 27, CONDITION_ALWAYS, 27 },
    { EVENT_EXIT, 28, CONDITION_ALWAYS, 28 },
    { EVENT_EXIT, 29, CONDITION_ALWAYS, 29 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 30, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 32, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 33, CONDITION_ALWAYS, 33 },
    { EVENT_EXIT, 34, CONDITION_ALWAYS, 34 },
    { EVENT_EXIT, 35, CONDITION_ALWAYS, 35 },
    { EVENT_EXIT, 36, CONDITION_ALWAYS, 36 },
    { EVENT_EXIT, 40, CONDITION_ALWAYS, 40 },
    { EVENT_EXIT, 41, CONDITION_ALWAYS, 41 },
    { EVENT_EXIT, 42, CONDITION_ALWAYS, 42 },
    { EVENT_EXIT, 43, CONDITION_ALWAYS, 43 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 44, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 45, CONDITION_ALWAYS, 45 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_ShowMessage22a8Branch) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunChoiceSequence22ab) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIHaventSeenMuchOfBabi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceItWouldBeAWasteTo) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceNoOnesGoingToSleepIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWouldThisRoomGetVeryDusty) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiWasFoundSafeAndSound) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceNowThatColossoIsOverIm) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheSouthernContinentIsADangerous) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWasHopingIdBeThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceColossoHasEndedSoWeHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWantThatLighthouseFinishedQuickly) },
    { EVENT_TALK, 14, IF_FLAG_SET(0xf31), MSGNO(MsgBabiPalaceIfYouUseTheCloakBall) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, IF_FLAG_SET(0xf31), MSGNO(MsgBabiPalaceWhyWouldAnAdeptLikeYou) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceSleepingALotHelpsToDelay) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabisFaceIsVeryPaleLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWishIHadMetYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIfSomethingHappensToBabiWhat) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(EV16_TALK) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheGirlFromLaliveroIsNo) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIAmSurprisedActorWasAllowed) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceItSeemedOnlyNaturalThatActor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIAmSoRelievedActorHas) },
    { SEARCH(SEARCH_UNNAMED), 70, CONDITION_ALWAYS, EVENT_SCRIPT(EV70_SEARCH) },
    { SEARCH(SEARCH_SHELF), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheArmorLooksHeavyItLooks)) },
    { SEARCH(SEARCH_SHELF), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresNothingInThereItsCompletely)) },
    { SEARCH(SEARCH_SHELF), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresAHeadbandItSaysNever)) },
    { SEARCH(SEARCH_BOOKCASE), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledTheHeart)) },
    { SEARCH(SEARCH_BOOKCASE), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresANotebookThatShowsThe)) },
    { SEARCH(SEARCH_BOOKCASE), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledBecomeA)) },
    { SEARCH(SEARCH_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceHeyItsBabisDiaryItReads)) },
    { SEARCH(SEARCH_BARREL), 100, 0xf9f, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gBabiPalaceEvents2[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(EV02_TOUCH) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceNormallyNoOnesAllowedIntoBabis) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceSomeOfTheRoomsInThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceShouldThePalaceReallyBeOpen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWonderHowManyPeopleWill) },
    { SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(EV90_SEARCH) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gBabiPalaceEvents2Colosso[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(EV02_TOUCH) },
    { EVENT_TALK, 8, 0x96c, EVENT_SCRIPT(RunSupplementalSequenceOne) },
    { EVENT_TALK, 9, 0x96c, EVENT_SCRIPT(RunSupplementalSequenceOne) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x408, 0x96c, EVENT_SCRIPT(RunSupplementalSequenceOne) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x409, 0x96c, EVENT_SCRIPT(RunSupplementalSequenceOne) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiWantsYouToGoStraight) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_ThankForSavingBabi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceSureTheySavedBabiButIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiIsADeliberateManHe) },
    { SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(EV90_SEARCH) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gBabiPalaceEvents2AfterColosso[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(EV02_TOUCH) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIveLostAllMyEnthusiasmSince) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_AskIfLeavingPalace) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheEndOfAFestivalIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceAllTheHeroicWarriorsWhoTook) },
    { SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(EV90_SEARCH) },
    { SCENE_EVENTS_END },
};
// clang-format on

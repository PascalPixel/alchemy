/// @file
/// Event commands: waits, received items, questions and destinations.
// Part of field_sequence.c.

#include "field_sequence_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "field/flag_call.h"
#include "system/frame.h"
#include "gamepak/far_call/game.h"
#include "gamepak/far_call/window.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/object.h"
#include "window/window.h"

/// Debug only: while debug mode is on, R makes every event wait (Event_Wait)
/// end at once and L restores normal waits.
void Event_UpdateDebugSkip(void) {
  struct FieldWork *runtime = gFieldWork;

  if (gDebugMode != 0) {
    if (gInput.pressed & KEY_L) {
      runtime->fastForward = 0;
    }
    if (gInput.pressed & KEY_R) {
      runtime->fastForward = -1;
    }
  }
}

/// Waits the requested frames through wait(), unless debug fast-forward
/// (Event_UpdateDebugSkip) is on: the wait events use.
void Event_Wait(s32 frames) {
  if (gFieldWork->fastForward == 0 && frames != 0) {
    wait(frames);
  }
}

/// Stops the leader where it stands: no horizontal velocity, the normal walking
/// speed (1.0) and acceleration (0.5), no destination, and the standing pose,
/// or the climbing pose (12) while the leader is on a climbable wall.
void Leader_StopMoving(void) {
  struct Actor *object = Actor_Lookup(Party_GetLeaderActor());
  object->speedLimit = Q16_ONE;
  object->acceleration = px(1) / 2;
  object->targetX = TARGET_UNSET;
  object->targetZ = TARGET_UNSET;
  object->velocityX = 0;
  object->velocityZ = 0;
  if (gMainRoutineWork.movementMode == 2)
    Actor_SelectAnimationFar(object, 12);
  else
    Actor_SelectAnimationFar(object, 1);
}

/// Begins an event: closes any area name window, stops the leader, ends the
/// lasting Psynergy if a trigger is running, resets the message and timing
/// state, and starts the debug fast-forward task with the leader as the
/// message speaker.
void Event_Start(void) {
  struct FieldWork *state = Ram_HeapSlots->fieldWork;
  AreaNameWindow_HideFar();
  Leader_StopMoving();
  if (state->busy != 0)
    Event_ResolveAllPendingActions();
  state->choiceMessage = 0;
  state->choiceWidth = 0;
  state->choiceSelection = 0;
  state->transitionFrames = 16;
  state->fastForward = 0;
  state->unusedAfterMessage = 0xffff;
  state->unusedBeforeCamera[0] = -1;
  state->unusedBeforeCamera[1] = -1;
  Task_Add((s32)Event_UpdateDebugSkip, TASK_PASS_VBLANK);
  GameFlag_Clear(0x132);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;
    state->speaker = party->leaderActorId;
    state->messageWindow = 0;
  }
}

/// Ends an event: stops the debug fast-forward task, gives the camera back to
/// the leader (or to actor 8 while gMainRoutineWork.ownerOverride is set) and
/// refreshes the Lure Cap's flag (GameFlag_RefreshLureCap).
void Event_End(void) {
  Task_Remove((u32)Event_UpdateDebugSkip);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;
    if (party->ownerOverride == 0)
      FieldCamera_FollowActor(party->leaderActorId, 1);
    else
      FieldCamera_FollowActor(8, 1);
    GameFlag_RefreshLureCapFar();
  }
}

/// An empty event callback.
void Event_EmptyCallback(void) {}

/// An empty hook; it does nothing in the released game. Only
/// Event_RunObjectHookAndWait calls it.
void Event_EmptyObjectHook(void) {}

/// Rebuilds the leader's actor from its placement row (Actor_ResetForLeader),
/// waits a frame and returns the leader's actor.
struct Actor *Event_RunObjectHookAndWait(s32 object) {
  Event_EmptyObjectHook();
  Actor_ResetForLeader(object);
  wait(1);
  return Actor_Lookup(Party_GetLeaderActor());
}

/// Spawns the actors of an actor placement table after the last actor in use.
void Event_CallWithLastActiveObjectId(s32 event_id) {
  Event_SpawnObjectTable((struct ActorPlacement *)event_id,
                         Actor_FindLastActiveId());
}

/// Replaces the scene's trigger table with events.
void Event_SetSceneEvents(s32 events) { gFieldWork->triggers = events; }

/// Adds unit to the party. With announce nonzero it also shows `<name> joined
/// your party.` (Party_ShowJoinedMessage, which receives announce too).
void Event_AddPartyMember(s32 event_id, s32 value) {
  Unit_Get(event_id);
  Party_AddMemberFar(event_id);
  if (value != 0) {
    Party_ShowJoinedMessageFar(event_id, value);
  }
}

/// Adds two units to the party and announces them together.
void Event_PrepareTwoObjectsAndApply(s32 first, s32 second) {
  Party_AddMemberFar(first);
  Party_AddMemberFar(second);
  Party_ShowPairJoinedMessageFar(first, second);
}

/// Checks an L or R button shortcut, packed as the party member in bits 10 to
/// 13 and the Psynergy in bits 0 to 9.
s32 Party_CheckShortcut(u32 packed_id) {
  s32 group = (packed_id >> 10) & 0xf;
  s32 entry = packed_id & 0x3ff;

  if (group > 7)
    return -1;
  if (GameFlag_Get(group) == 0)
    return -2;
  if (Unit_KnowsPsynergyFar(group, entry) == 0)
    return -3;
  return 0;
}

/// Clears each button shortcut whose Psynergy can no longer be cast: zeroes
/// gMainRoutineWork.firstShortcut or secondShortcut.
void Party_ClearUnusableShortcuts(void) {
  if (Party_CheckShortcut(gMainRoutineWork.firstShortcut) != 0)
    gMainRoutineWork.firstShortcut = 0;
  if (Party_CheckShortcut(gMainRoutineWork.secondShortcut) != 0)
    gMainRoutineWork.secondShortcut = 0;
}

/// Takes unit out of the party, fully restoring its HP and PP and curing its
/// poison.
void Party_RemoveMemberRestored(s32 unit) {
  struct Unit *vitals;
  s32 partyIndex;
  s32 partyCount;
  s32 survivingMembers;

  Party_RemoveMemberFar(unit);
  Party_ClearUnusableShortcuts();
  vitals = Unit_Get(unit);
  vitals->hp = vitals->hpMax;
  vitals->pp = vitals->ppMax;
  Unit_UpdateGauges(vitals);
  vitals->poison = 0;
  // Count the party members who remain and still have HP.
  survivingMembers = 0;
  partyCount = Party_CountMembersFar();
  for (partyIndex = 0; partyIndex < partyCount; partyIndex++) {
    vitals = Unit_Get(gMainRoutineWork.party[partyIndex]);
    if (vitals->hp != 0)
      survivingMembers++;
  }
  if (survivingMembers == 0) {
    vitals = Unit_Get(gMainRoutineWork.leaderActorId);
    vitals->hp = 1;
    Unit_UpdateGauges(vitals);
  }
}

/// Checks whether the party can carry one more of item; when it cannot, says
/// so and returns -1, else 0.
s32 Party_CheckItemLimit(s32 item) {
  Party_CountMembersFar();
  if (PartyInventory_FindRoomFar(item) < 0) {
    MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
    FieldMessage_ShowFar((s32)MsgPartyCannotCarry, 1);
    MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
    FieldMessage_ShowFar((s32)MsgPartyCannotCarry + 1, 1);
    return -1;
  }
  return 0;
}

// The mode of the message that an item went to another member: patch 2 (German
// alone) changes it, and patch 3 drops the message.
#if TLA_PATCH_2 && !TLA_PATCH_3
#define ITEM_GIVEN_MODE 1
#else
#define ITEM_GIVEN_MODE 3
#endif

/// The event command "give item": gives item to the first party member with
/// room for it and says so; with no room, the player chooses what to drop.
s32 PartyInventory_GiveItem(s32 item, s32 unused) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s16 saved = work->message;
  s32 owner;
  s32 result;
  s32 count;
  s32 i;
  s32 member;
  s32 slot;

  owner = PartyInventory_AddFar(item);
  if (owner == -1) {
    MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
    FieldMessage_ShowFar((s32)&MsgItemReceived, 1);
    FieldMessage_ShowFar((s32)&MsgItemCannotCarry, 1);
  retry:
    {
      FieldMessage_ShowFar((s32)&MsgItemDropPrompt, 1);
      result = ItemPicker_RunToDropFar(&member, &slot);
      if (result == -1) {
        if (((struct ItemHead *)Item_GetDirectFar(item))->flags & 8) {
          MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
          FieldMessage_ShowFar((s32)&MsgItemDropPrompt + 4, 1);
          goto retry;
        }
        MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
        FieldMessage_ShowFar((s32)&MsgItemDropPrompt + 1, 5);
        result = FieldYesNo_Ask(1);
        Message_CloseFinishedWindowsFar();
        if (result != 0)
          goto retry;
        Item_AdjustCounterFar(item, 1);
        MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
        FieldMessage_ShowFar((s32)&MsgItemDropPrompt + 2, 1);
        goto done;
      }
      Unit_Get(member);
      count = Inventory_GetQuantityFar(member, slot);
      if (Inventory_CountItemFar(member, item) > 29) {
        MessageArgument_PushFar(member, MESSAGE_ARGUMENT_ACTOR);
        MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
        FieldMessage_ShowFar((s32)&MsgItemDropPrompt + 7, 1);
        goto retry;
      }
      for (i = 0; i < count; i++)
        Inventory_DiscardFar(member, slot);
      Unit_RefreshPsynergyFar(member);
      Unit_CalculateFar(member);
      owner = PartyInventory_AddFar(item);
      Sound_PlayFar(MUS_TREASURE_FOUND);
      if (owner == gMainRoutineWork.leaderActorId) {
        MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
        FieldMessage_ShowFar((s32)&MsgItemReceived, 3);
      } else {
        MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
        MessageArgument_PushFar(owner, MESSAGE_ARGUMENT_ACTOR);
#if !TLA_PATCH_3
        FieldMessage_ShowFar((s32)&MsgItemGiven, ITEM_GIVEN_MODE);
#endif
      }
    }
  done:
    work->message = saved;
    return owner;
  }
  Sound_PlayFar(MUS_TREASURE_FOUND);
  MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
  FieldMessage_ShowFar((s32)&MsgItemReceived, 3);
  if (owner != gMainRoutineWork.leaderActorId) {
    MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
    MessageArgument_PushFar(owner, MESSAGE_ARGUMENT_ACTOR);
    FieldMessage_ShowFar((s32)&MsgItemReceived + 1, 1);
  }
  work->message = saved;
  return owner;
}

// Inventory: give an item and return its owner.

/// Gives the item. Returns the character ID on success, or -1 if the
/// inventory is full.
/// item: item ID.
/// owner: recipient character ID.
s32 Inventory_TryAddAndReturnMember(s32 item, s32 unused, s32 unit) {
  if (Inventory_AddItemFar(unit, item) >= 0)
    return unit;
  return -1;
}

/// An empty callback.
void Inventory_NoOpCallback(void) {}

/// An empty callback.
void Inventory_ReservedNoOp(void) {}

/// Waits, at most 90 frames, for an actor's animation to stop being animation:
/// the actor has finished a one-off animation such as a nod.
void Actor_WaitForAnimationEnd(s32 objectId, s32 value) {
  struct Actor *obj = Actor_Lookup(objectId);

  if (obj != 0 && obj->spriteStorageKind == 1) {
    s32 cnt = 0;
    u8 *anim = (u8 *)obj->sprite;

    while (cnt <= 89) {
      wait(1);
      if (value != anim[0x18]) {
        break;
      }
      cnt++;
    }
  }
}

/// The event command "choose yes or no": waits for the message, asks yes or no
/// below it or above it when the message windows reach past row 15, and the
/// speaker nods (animation 3) at yes and shakes its head (4) at no. It waits
/// (2 - event mode) * 3 frames, passes the name and its width and a third value
/// to the menu, and skips the animation wait in event mode 2.
/// The event command "choose yes or no": once the message on screen has been
/// read, asks yes or no below it, or above it when the message windows reach
/// past row 15 or forceBottom is set.
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 forceBottom) {
  u8 *work = Ram_HeapSlots->fieldWork;
  s32 sprite = *(s16 *)Scene_FindActorPlacement(*(s32 *)(work + 0x1e0));
  struct Window *window = *(struct Window **)(work + 0x1e4);
  struct Window *side = *(struct Window **)(work + 0x1e8);
  s32 useBottom = 1;
  s32 answer;

  while (Message_AllTypedFar() == 0)
    wait(1);

  wait((2 - gMainRoutineWork.messageSpeed) * 3);

  if (forceBottom == 0) {
    s32 bottom = (s16)window->y + window->height;

    if (side != NULL) {
      s32 sideBottom = (s16)side->y + side->height;
      if (bottom < sideBottom)
        bottom = sideBottom;
    }

    if (bottom > 15)
      useBottom = 0;
  }

  answer = YesNoMenu_RunFar(useBottom, *(s16 *)(work + 0xcb0),
                            *(s16 *)(work + 0xcb2), *(s16 *)(work + 0xcb4));
  if (answer != 0) {
    Actor_SetAnimationById(actor, 4);
    Portrait_CloseWindowFar(sprite);
    Message_CloseFinishedWindowsFar();
    if (gMainRoutineWork.messageSpeed != 2)
      Actor_WaitForAnimationEnd(actor, 4);
  } else {
    Actor_SetAnimationById(actor, 3);
    Portrait_CloseWindowFar(sprite);
    Message_CloseFinishedWindowsFar();
    if (gMainRoutineWork.messageSpeed != 2)
      Actor_WaitForAnimationEnd(actor, 3);
  }

  return answer;
}

/// Asks yes or no at the top of the screen, or at the bottom with atBottom set.
/// Returns 0 for yes, 1 for no.
s32 FieldYesNo_Ask(s32 atBottom) { return YesNoMenu_RunFar(atBottom, 0, 0, 0); }

/// Gives the party item from actor: with no room it shows the item, says it
/// was found and left behind and returns -1; otherwise it adds the item, sets
/// flag (unless -1) and returns 0.
s32 Event_PickUpItem(s32 actor, s32 item, s32 flag) {
  if (PartyInventory_FindRoomFar(item) < 0) {
    Actor_GetById(actor)->scaleX = 0;
    ItemDisplay_ShowItem(item, 3);
    MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
    FieldMessage_ShowFar((s32)&MsgFoundItem, 1);
    // Four messages on: "But <protagonist>'s party can't carry any more".
    FieldMessage_ShowFar((s32)&MsgFoundItem + 4, 1);
    Actor_GetById(actor)->scaleX = Q16_ONE;
    Actor_FaceActorById(gMainRoutineWork.leaderActorId, actor, 0);
    Event_Wait(20);
    return -1;
  }
  Actor_SetHorizontalPositionWithTerrain(actor, 0, 0);
  ItemDisplay_ShowItem(item, 3);
  PartyInventory_GiveItem(item, 0);
  if (flag != -1)
    GameFlag_Set(flag);
  return 0;
}

// Messages: teach an ability and display the learning message.

/// Teaches the ability, plays a sound, shows a message and waits for it to
/// advance.
/// unit: character ID (message name 1).
/// ability: ability ID (message ability 4).
void Party_LearnAbility(s32 unit, s32 ability) {
  Unit_LearnPsynergyFar(unit, ability);
  Sound_PlayFar(MUS_TREASURE_FOUND);
  MessageArgument_PushFar(unit, MESSAGE_ARGUMENT_ACTOR);
  MessageArgument_PushFar(ability, MESSAGE_ARGUMENT_ABILITY);
  FieldMessage_ShowFar(MsgAbilityLearned,
                       FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_WAITING_FOR_SOUND);
}

/// The event command "open screen": starts the event's screen transition and
/// sets transitionActive.
void Event_StartScreenTransition(void) {
  struct FieldWork *runtime = gFieldWork;

  ScreenTransition_Start(runtime->transition, runtime->transitionFrames);
  runtime->transitionActive = 1;
}

/// The event command "close screen": finishes the event's screen transition
/// and clears transitionActive.
void Event_FinishScreenTransition(void) {
  struct FieldWork *runtime = gFieldWork;

  ScreenTransition_Finish(runtime->transition, runtime->transitionFrames);
  runtime->transitionActive = 0;
}

/// The event command "wait for screen": waits as long as the transition runs
/// (transitionFrames).
void Event_WaitTransitionFrames(void) { wait(gFieldWork->transitionFrames); }

/// Makes the event leave for the given entrance of the given scene: raises the
/// exit request 999 and writes gMainRoutineWork.next and entrance.
void Event_SetDestination(s32 sceneId, s32 entrance) {
  gFieldWork->exit = EXIT_KEEP_DESTINATION;
  gMainRoutineWork.next = sceneId;
  gMainRoutineWork.entrance = entrance;
}

/// Makes the event leave and records scene and entrance as the restart point.
void Event_SetRestartPoint(s32 scene, s32 entrance) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct MainRoutineWork *party;

  event->exit = EXIT_KEEP_DESTINATION;
  party = &gMainRoutineWork;
  party->restartScene = scene;
  party->restartEntrance = entrance;
}

/// The event command "request exit": sets the field's exit request (999 leaves
/// for gMainRoutineWork's next and entrance).
void Event_SetExitRequest(u16 request) {
  struct FieldWork *state = gFieldWork;
  state->exit = request;
}

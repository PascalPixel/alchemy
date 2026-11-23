/// @file
/// Event commands: waits, received items, questions and destinations.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// When a gift goes to another party member, `<leader> gave <item> to
/// <member>.` follows `<leader> got <item>.`; it is MsgGaveItemToMember, the
/// next message, in every edition.
#if LANGUAGE == LANGUAGE_GERMAN
#define GIFT_RECEIVED_MESSAGE MsgGaveItemToMember
#else
#define GIFT_RECEIVED_MESSAGE (MsgColossoIsaacGotItem + 1)
#endif

/// Japanese and English show it zooming open and waiting for the jingle; from
/// patch 2 it only zooms open.
#if TBS_PATCH_2
#define GIFT_RECEIVED_MESSAGE_MODE FIELD_MESSAGE_ZOOM
#else
#define GIFT_RECEIVED_MESSAGE_MODE                                             \
  (FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_WAITING_FOR_SOUND)
#endif

/// Shows the message for an item the party has just received.
static inline void ItemGift_ShowGotItem(s32 item, s32 flags) {
#if LANGUAGE == LANGUAGE_GERMAN
  if (item == ITEM_BLACK_ORB) {
    FieldMessage_ShowFar(MsgGotBlackOrb, flags);
    return;
  }
#endif
  MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
  FieldMessage_ShowFar(MsgColossoIsaacGotItem, flags);
}

/// Debug only: while debug mode is on, R makes every event wait (Event_Wait)
/// end at once and L restores normal waits.
void Event_UpdateDebugSkip(void) {
  struct FieldWork *runtime = gFieldWork;

  if (gDebugMode != 0) {
    if (gKeysPressed & KEY_L) {
      runtime->fastForward = 0;
    }
    if (gKeysPressed & KEY_R) {
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
/// or the clinging pose while the leader is on a climbable wall.
void Leader_StopMoving(void) {
  struct Actor *actor;

  actor = Actor_Lookup(gMainRoutineWork.leaderActorId);
  actor->speedLimit = FIXED_ONE;
  actor->acceleration = toFIXED(1) / 2;
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;
  actor->velocityX = 0;
  actor->velocityZ = 0;
  if (gMainRoutineWork.movementMode == LEADER_MOVEMENT_CLIMB) {
    Actor_SelectAnimationFar(actor, ANIM_LEADER_CLING);
    return;
  }
  Actor_SelectAnimationFar(actor, ANIM_STAND);
}

/// Begins an event: closes any area name window, stops the leader, resets the
/// message and timing state, and starts the debug fast-forward task with the
/// leader as the message speaker.
void Event_Start(void) {
  struct FieldWork *runtime = gFieldWork;

  AreaNameWindow_HideFar();
  Leader_StopMoving();
  // Keep the event reset's signed halfword read.
  if ((s16)runtime->busy != 0) {
    Field_EndBusyAndReveal();
  }
  // Every event starts with no name window and normal waits.
  runtime->choiceMessage = 0;
  runtime->choiceWidth = 0;
  runtime->transitionFrames = MAP_TRANSITION_FRAMES;
  runtime->fastForward = 0;
  runtime->unusedAfterMessage = -1;
  runtime->unusedBeforeCamera[0] = -1;
  runtime->unusedBeforeCamera[1] = -1;
  Task_Add(Event_UpdateDebugSkip, TASK_PASS_FRAME);
  // XXX (no evidence): what flag 0x132 means; every event start in both games
  // clears it, nothing in either game's source names it or reads it by its
  // number, and no other code of tbs-en's build loads that number for a flag.
  GameFlag_ClearFar(0x132);
  runtime->speaker = gMainRoutineWork.leaderActorId;
  runtime->messageWindow = 0;
}

/// Ends an event: stops the debug fast-forward task, gives the camera back to
/// the leader and refreshes FLAG_LURE_CAP (GameFlag_RefreshLureCap).
void Event_End(void) {
  Task_Remove(Event_UpdateDebugSkip);
  FieldCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  GameFlag_RefreshLureCapFar();
}

UNUSED void Event_NoOpUpdateA(void) {}

/// An empty hook; it does nothing in the released game. Only
/// Event_RunActorUpdateAndWait calls it.
void Event_NoOpUpdate(void) {}

/// Respawns the scene's actors from an actor placement table and
/// waits one frame for them to appear.
void Event_RunActorUpdateAndWait(struct ActorPlacement *table) {
  Event_NoOpUpdate();
  Actor_ResetForLeader(table);
  wait(1);
  Actor_Lookup(gMainRoutineWork.leaderActorId);
}

/// Spawns the actors of an actor placement table after the last actor in use.
void Event_AppendActorPlacements(struct ActorPlacement *table) {
  Event_SpawnActorPlacements(table, Actor_FindLastActiveId());
}

/// Sets the scene's trigger table, which the field checks as the leader moves:
/// writes work->events. Altin Peak's event calls it.
void Event_SetTriggers(const struct Trigger *triggers) {
  struct FieldWork *work = gFieldWork;

  work->triggers = triggers;
}

/// Adds unit to the party. With announce nonzero it also shows `<name> joined
/// your party.` (Party_ShowJoinedMessage, which receives announce too).
void Event_AddPartyMember(s32 unit, s32 announce) {
  Unit_GetFar(unit);
  Party_AddMemberFar(unit);
  if (announce != 0) {
    Party_ShowJoinedMessageFar(unit, announce);
  }
}

/// Adds two units to the party and announces them together. Vale's
/// event calls it twice.
void Event_AddPartyMemberPair(s32 first, s32 second) {
  Party_AddMemberFar(first);
  Party_AddMemberFar(second);
  Party_ShowPairJoinedMessageFar(first, second);
}

/// Checks an L or R button shortcut, packed as a field Psynergy use is: the
/// party member (ACT_UNIT) and the Psynergy (ABILITY_ID).
s32 Party_CheckShortcut(u32 shortcut) {
  s32 member = ACT_UNIT(shortcut);
  s32 ability = ABILITY_ID(shortcut);

  if (member > ALLY_MAX)
    return -1;
  if (GameFlag_GetFar(FLAG_IN_PARTY(member)) == 0)
    return -2;
  if (Unit_KnowsPsynergyFar(member, ability) == 0)
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
  vitals = Unit_GetFar(unit);
  vitals->hp = vitals->hpMax;
  vitals->pp = vitals->ppMax;
  Unit_UpdateGauges(vitals);
  vitals->poison = 0;
  // Count the party members who remain and still have HP.
  survivingMembers = 0;
  partyCount = Party_CountMembersFar();
  for (partyIndex = 0; partyIndex < partyCount; partyIndex++) {
    vitals = Unit_GetFar(gMainRoutineWork.party[partyIndex]);
    if (vitals->hp != 0)
      survivingMembers++;
  }
  if (survivingMembers == 0) {
    vitals = Unit_GetFar(gMainRoutineWork.leaderActorId);
    vitals->hp = 1;
    Unit_UpdateGauges(vitals);
  }
}

/// Checks whether the party can carry one more of item: each party member
/// carries at most INVENTORY_STACK_MAX of one item.
s32 Party_CheckItemLimit(s32 item) {
  s32 partyCount;
  s32 heldQuantity = 0;
  s32 partyIndex;

  partyCount = Party_CountMembersFar();
  for (partyIndex = 0; partyIndex < partyCount; partyIndex++)
    heldQuantity +=
        Inventory_CountItemFar(gMainRoutineWork.party[partyIndex], item);

  if (heldQuantity >= partyCount * INVENTORY_STACK_MAX) {
    MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
    // "Isaac's party cannot carry any more <item>s..."
    FieldMessage_ShowFar(MsgCannotCarryMore, FIELD_MESSAGE_ZOOM);
    MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
    // "Isaac threw the <item> as far away as possible."
    FieldMessage_ShowFar(MsgCannotCarryMore + 1, FIELD_MESSAGE_ZOOM);
    return -1;
  }
  return 0;
}

/// Gives item to the first party member with
/// room for it and says so.
s32 PartyInventory_GiveItem(s32 item, s32 unused) {
  struct FieldWork *work;
  s32 savedMessage;
  s32 owner;
  s32 result;
  s32 discardQuantity;
  s32 discardIndex;
  s32 member;
  s32 slot;

  work = gFieldWork;
  // The messages below move the event's message on; every return puts it
  // back, so the event continues where it was.
  savedMessage = work->message;
  owner = PartyInventory_AddFar(item);
  if (owner == -1) {
    // Every bag is full: "But you can't carry any more items."
    ItemGift_ShowGotItem(item, FIELD_MESSAGE_ZOOM);
    FieldMessage_ShowFar(MsgCannotCarryAnyMore, FIELD_MESSAGE_ZOOM);
  retry:
    FieldMessage_ShowFar(MsgWhatWillYouDrop, FIELD_MESSAGE_ZOOM);
    result = ItemPicker_RunFar(&member, &slot);
    if (result == -1) {
      // Cancelled: drop the new item instead, unless it is indispensable
      // ("That <item>'s valuable! You must get it!").
      if (Item_GetDirectFar(item)->flags & ITEM_INDISPENSABLE) {
        MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
        FieldMessage_ShowFar(MsgWhatWillYouDrop + 4, FIELD_MESSAGE_ZOOM);
        goto retry;
      }
      // "Drop the <item>?"
      MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
      FieldMessage_ShowFar(MsgWhatWillYouDrop + 1,
                           FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_KEEP_OPEN);
      result = FieldYesNo_Ask(1);
      Message_CloseFinishedWindowsFar();
      if (result != 0)
        goto retry;
      // "Isaac drops the <item>."
      Item_AdjustCounterFar(item, 1);
      MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
      FieldMessage_ShowFar(MsgWhatWillYouDrop + 2, FIELD_MESSAGE_ZOOM);
      work->message = savedMessage;
      return owner;
    } else {
      // A carried item was picked: drop it (the whole stack) and take the new
      // one.
      {
        struct Unit *unit = Unit_GetFar(member);
        /* FAKEMATCH(dead): unit and the picked item read through it. */
        UNUSED s32 pickedItem = unit->inventory[slot];

        discardQuantity = Shop_GetSelectionState(member, slot);
      }
      // A party member who already carries INVENTORY_STACK_MAX of the new item
      // cannot take it: say so and ask again.
#if !(TBS_PATCH_1 && !TBS_PATCH_2)
      if (Inventory_CountItemFar(member, item) >= INVENTORY_STACK_MAX) {
        MessageArgument_PushFar(member, MESSAGE_ARGUMENT_ACTOR);
        MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
        FieldMessage_ShowFar(MsgWhatWillYouDrop + 7, FIELD_MESSAGE_ZOOM);
        goto retry;
      }
#endif
      for (discardIndex = 0; discardIndex < discardQuantity; discardIndex++)
        Inventory_DiscardFar(member, slot);
      owner = PartyInventory_AddFar(item);
      Sound_PlayFar(MUS_TREASURE_FOUND);
      if (owner == gMainRoutineWork.leaderActorId) {
        ItemGift_ShowGotItem(item, FIELD_MESSAGE_ZOOM |
                                       FIELD_MESSAGE_WAITING_FOR_SOUND);
      } else {
        MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
        MessageArgument_PushFar(owner, MESSAGE_ARGUMENT_ACTOR);
        FieldMessage_ShowFar(MsgGaveItemToMember,
                             FIELD_MESSAGE_ZOOM |
                                 FIELD_MESSAGE_WAITING_FOR_SOUND);
      }
      work->message = savedMessage;
      return owner;
    }
  } else {
    // There was room: "Isaac got <item>.", and "Isaac gave <item> to
    // <member>." when someone other than the leader carries it.
    Sound_PlayFar(MUS_TREASURE_FOUND);
    ItemGift_ShowGotItem(item,
                         FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_WAITING_FOR_SOUND);
    if (owner != gMainRoutineWork.leaderActorId) {
      MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
      MessageArgument_PushFar(owner, MESSAGE_ARGUMENT_ACTOR);
      FieldMessage_ShowFar(GIFT_RECEIVED_MESSAGE, GIFT_RECEIVED_MESSAGE_MODE);
    }
    work->message = savedMessage;
  }
  return owner;
}

/// Gives item to one party member. Returns unit, or -1 if its bag is full.
s32 Inventory_TryAddAndReturnMember(s32 item, s32 unused, s32 unit) {
  if (Inventory_AddItemFar(unit, item) >= 0)
    return unit;
  return -1;
}

/// An empty function; nothing in The Broken Seal calls it.
void Inventory_NoOpCallback(void) {}

/// An empty function; nothing in The Broken Seal calls it.
void Inventory_ReservedNoOp(void) {}

/// Waits, at most 90 frames, for an actor's animation to stop being animation:
/// the actor has finished a one-off animation such as a nod.
void Actor_WaitForAnimationEnd(s32 actorId, s32 animation) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != 0 && actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
    s32 frames = 0;
    u8 *animationNumber = &actor->sprite->lastNo;

    for (; frames <= 89; frames++) {
      wait(1);
      if (animation != *animationNumber) {
        break;
      }
    }
  }
}

/// The last tile row the message windows may reach with a yes-or-no question
/// still asked below them.
#define YES_NO_BELOW_LAST_ROW 15

/// Once the message on screen has been
/// read, asks yes or no below it, or above it when the message windows reach
/// past YES_NO_BELOW_LAST_ROW, unless forceBottom is set.
s32 Event_AskYesNoWithActorAnimation(s32 actorId, s32 forceBottom) {
  struct FieldWork *work = gFieldWork;
  s32 sprite = Scene_FindActorPlacement(work->speaker)->sprite;
  struct Window *window = (struct Window *)work->messageWindow;
  struct Window *side = (struct Window *)work->sideWindow;
  s32 useBottom = 1;
  s32 answer;

  // Wait for every button to be released and the message to finish.
  while (gKeysPressed != 0)
    wait(1);

  while (Message_AllTypedFar() == 0)
    wait(1);

  wait(3);

  // The question goes above the message windows when they reach too low, so it
  // does not cover them.
  if (forceBottom == 0) {
    s32 bottom = window->y + window->height;

    if (side != 0) {
      s32 sideBottom = side->y + side->height;
      if (bottom < sideBottom)
        bottom = sideBottom;
    }

    if (bottom > YES_NO_BELOW_LAST_ROW)
      useBottom = 0;
  }

  answer =
      YesNoMenu_RunFar(useBottom, work->choiceMessage, work->choiceWidth, 0);
  if (answer != 0) {
    Actor_SetAnimationById(actorId, ANIM_SHAKE_HEAD);
    Portrait_CloseWindowFar(sprite);
    Message_CloseFinishedWindowsFar();
    Actor_WaitForAnimationEnd(actorId, ANIM_SHAKE_HEAD);
  } else {
    Actor_SetAnimationById(actorId, ANIM_NOD);
    Portrait_CloseWindowFar(sprite);
    Message_CloseFinishedWindowsFar();
    Actor_WaitForAnimationEnd(actorId, ANIM_NOD);
  }

  return answer;
}

/// Asks yes or no at the top of the screen, or at the bottom with atBottom set.
/// Returns 0 for yes, 1 for no.
s32 FieldYesNo_Ask(s32 atBottom) { return YesNoMenu_RunFar(atBottom, 0, 0, 0); }

/// Teaches unit the Psynergy ability and says so with its jingle, waiting for
/// the player to read it.
void Party_LearnAbility(s32 unit, s32 ability) {
  // Declared without parameters: unit and ability pass through in r0 and r1.
  Unit_LearnPsynergyFar();
  Sound_PlayFar(MUS_TREASURE_FOUND);
  MessageArgument_PushFar(unit, MESSAGE_ARGUMENT_ACTOR);
  MessageArgument_PushFar(ability, MESSAGE_ARGUMENT_ABILITY);
  FieldMessage_ShowFar(MsgAbilityLearned,
                       FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_WAITING_FOR_SOUND);
}

/// Starts the event's screen transition and
/// sets work->transitionActive.
void Event_StartScreenTransition(void) {
  struct FieldWork *runtime = gFieldWork;

  ScreenTransition_Start(runtime->transition, runtime->transitionFrames);
  runtime->transitionActive = 1;
}

/// Finishes the event's screen transition and
/// clears work->transitionActive.
void Event_FinishScreenTransition(void) {
  struct FieldWork *runtime = gFieldWork;

  ScreenTransition_Finish(runtime->transition, runtime->transitionFrames);
  runtime->transitionActive = 0;
}

/// Waits as long as the transition runs
/// (work->transitionFrames). Events reach it.
void Event_WaitTransitionFrames(void) { wait(gFieldWork->transitionFrames); }

/// Makes the event leave for the given entrance of the given scene: raises the
/// exit request EXIT_KEEP_DESTINATION and writes gMainRoutineWork.next and
/// entrance.
void Event_SetDestination(s32 sceneId, s32 entrance) {
  gFieldWork->exit = EXIT_KEEP_DESTINATION;
  gMainRoutineWork.next = sceneId;
  gMainRoutineWork.entrance = entrance;
}

/// Makes the event leave and records scene and entrance as the restart
/// point.
void Event_SetRestartPoint(u16 sceneId, u16 entrance) {
  struct FieldWork *state = gFieldWork;
  state->exit = EXIT_KEEP_DESTINATION;
  gMainRoutineWork.restartScene = sceneId;
  gMainRoutineWork.restartEntrance = entrance;
}

/// Sets the field's exit request
/// (EXIT_KEEP_DESTINATION leaves for gMainRoutineWork's next and entrance).
void Event_SetExitRequest(u16 request) {
  struct FieldWork *state = gFieldWork;
  state->exit = request;
}

#undef GIFT_RECEIVED_MESSAGE
#undef GIFT_RECEIVED_MESSAGE_MODE
#undef YES_NO_BELOW_LAST_ROW

/// @file
/// Event commands: waits, received items, questions and destinations.
// Part of field_sequence.c.

#include "field_sequence_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "system/frame.h"
#include "gamepak/far_call/game.h"
#include "gamepak/far_call/window.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/object.h"
#include "window/window.h"
#include "window/message_speed.h"
#include "game/item.h"
#include "field/field_sequence.h"

/// After a drop, "Felix gave <item> to <member>." shows zooming open alone in
/// German (patch 2), waiting for the jingle as well before it, and patch 3
/// drops the message.
#if TLA_PATCH_2 && !TLA_PATCH_3
#define GIFT_RECEIVED_MESSAGE_MODE FIELD_MESSAGE_ZOOM
#else
#define GIFT_RECEIVED_MESSAGE_MODE                                             \
  (FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_WAITING_FOR_SOUND)
#endif

/// Debug only: while debug mode is on, R makes every event wait (Event_Wait)
/// end at once and L restores normal waits.
void Event_UpdateDebugSkip(void) {
  struct FieldState *field = gFieldState;

  if (gDebugMode != 0) {
    if (gInput.pressed & KEY_L) {
      field->fastForward = 0;
    }
    if (gInput.pressed & KEY_R) {
      field->fastForward = -1;
    }
  }
}

/// Waits the requested frames through wait(), unless debug fast-forward
/// (Event_UpdateDebugSkip) is on: the wait events use.
void Event_Wait(s32 frames) {
  if (gFieldState->fastForward == 0 && frames != 0) {
    wait(frames);
  }
}

/// Stops the leader where it stands: no horizontal velocity, the normal walking
/// speed (1.0) and acceleration (0.5), no destination, and the standing pose,
/// or ANIM_LEADER_CLING while the leader is on a climbable wall.
void Leader_StopMoving(void) {
  struct Actor *actor = Actor_Lookup(Party_GetLeaderActor());
  actor->speedLimit = FIXED_ONE;
  actor->acceleration = toFIXED(1) / 2;
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;
  actor->velocityX = 0;
  actor->velocityZ = 0;
  if (gMainRoutineState.movementMode == LEADER_MOVEMENT_KEYS)
    Actor_SelectAnimationFar(actor, ANIM_LEADER_CLING);
  else
    Actor_SelectAnimationFar(actor, ANIM_STAND);
}

/// Begins an event: closes any area name window, stops the leader, ends the
/// lasting Psynergy if a trigger is running, resets the message and timing
/// state, and starts the debug fast-forward task with the leader as the
/// message speaker.
void Event_Start(void) {
  struct FieldState *field = Ram_HeapSlots->fieldState;
  AreaNameWindow_HideFar();
  Leader_StopMoving();
  if (field->busy != 0)
    Event_EndAllLastingPsynergy();
  field->choiceMessage = 0;
  field->choiceWidth = 0;
  field->choiceSelection = 0;
  field->transitionFrames = MAP_TRANSITION_FRAMES;
  field->fastForward = 0;
  field->unusedAfterMessage = -1;
  field->unusedBeforeCamera[0] = -1;
  field->unusedBeforeCamera[1] = -1;
  Task_Add((s32)Event_UpdateDebugSkip, TASK_PASS_FRAME);
  // XXX (no evidence): what flag 0x132 means; every event start in both games
  // clears it, and nothing in either game's code, tables or cleared ranges of
  // flags sets or reads it.
  GameFlag_Clear(0x132);
  {
    struct MainRoutineState *party = &gMainRoutineState;
    field->speaker = party->leaderActorId;
    field->messageWindow = 0;
  }
}

/// Ends an event: stops the debug fast-forward task, gives the camera back to
/// the leader (or to the first placed actor, the ship, while
/// gMainRoutineState.ownerOverride is set) and refreshes the Lure Cap's flag
/// (GameFlag_RefreshLureCap).
void Event_End(void) {
  Task_Remove((u32)Event_UpdateDebugSkip);
  {
    struct MainRoutineState *party = &gMainRoutineState;
    if (party->ownerOverride == 0)
      FieldCamera_FollowActor(party->leaderActorId, 1);
    else
      FieldCamera_FollowActor(ACTOR_FIRST_PLACED, 1);
    GameFlag_RefreshLureCapFar();
  }
}

/// An empty function nothing calls.
UNUSED void Event_EmptyCallback(void) {}

/// An empty hook; it does nothing in the released game. Only
/// Event_ResetLeaderAndWait calls it.
void Event_EmptyHook(void) {}

/// Rebuilds the leader's actor from its placement row (Actor_ResetForLeader),
/// waits a frame and returns the leader's actor.
struct Actor *Event_ResetLeaderAndWait(s32 table) {
  Event_EmptyHook();
  Actor_ResetForLeader(table);
  wait(1);
  return Actor_Lookup(Party_GetLeaderActor());
}

/// Spawns the actors of an actor placement table after the last actor in use.
void Event_SpawnActorsAfterLast(s32 table) {
  Event_SpawnActorTable((struct ActorPlacement *)table,
                        Actor_FindLastActiveId());
}

/// Replaces the scene's trigger table with events.
void Event_SetSceneEvents(s32 events) { gFieldState->triggers = events; }

/// Adds unit to the party. With announce nonzero it also shows `<name> joined
/// your party.` (Party_ShowJoinedMessage, which receives announce too).
void Event_AddPartyMember(s32 unit, s32 announce) {
  // DEAD: the unit looked up and never used; the ROM keeps the call.
  Unit_Get(unit);
  Party_AddMemberFar(unit);
  if (announce != 0) {
    Party_ShowJoinedMessageFar(unit, announce);
  }
}

/// Adds two units to the party and announces them together ("<a> and <b>
/// joined your party.", Party_ShowPairJoinedMessage).
void Event_AddTwoPartyMembers(s32 first, s32 second) {
  Party_AddMemberFar(first);
  Party_AddMemberFar(second);
  Party_ShowPairJoinedMessageFar(first, second);
}

/// What Party_CheckShortcut finds wrong with a shortcut.
enum ShortcutProblem {
  SHORTCUT_USABLE = 0,
  SHORTCUT_NO_SUCH_MEMBER = -1,
  SHORTCUT_MEMBER_AWAY = -2, ///< not in the party
  SHORTCUT_NOT_LEARNED = -3  ///< the member does not know the Psynergy
};

/// Checks an L or R button shortcut, packed as the party member in bits 10 to
/// 13 and the Psynergy in bits 0 to 9.
s32 Party_CheckShortcut(u32 shortcut) {
  s32 member = ACT_UNIT(shortcut);
  s32 ability = ABILITY_ID(shortcut);

  if (member > ALLY_MAX)
    return SHORTCUT_NO_SUCH_MEMBER;
  if (GameFlag_Get(FLAG_IN_PARTY(member)) == 0)
    return SHORTCUT_MEMBER_AWAY;
  if (Unit_KnowsPsynergyFar(member, ability) == 0)
    return SHORTCUT_NOT_LEARNED;
  return SHORTCUT_USABLE;
}

/// Clears each button shortcut whose Psynergy can no longer be cast: zeroes
/// gMainRoutineState.firstShortcut or secondShortcut.
void Party_ClearUnusableShortcuts(void) {
  if (Party_CheckShortcut(gMainRoutineState.firstShortcut) != SHORTCUT_USABLE)
    gMainRoutineState.firstShortcut = 0;
  if (Party_CheckShortcut(gMainRoutineState.secondShortcut) != SHORTCUT_USABLE)
    gMainRoutineState.secondShortcut = 0;
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
    vitals = Unit_Get(gMainRoutineState.party[partyIndex]);
    if (vitals->hp != 0)
      survivingMembers++;
  }
  if (survivingMembers == 0) {
    vitals = Unit_Get(gMainRoutineState.leaderActorId);
    vitals->hp = 1;
    Unit_UpdateGauges(vitals);
  }
}

/// Checks whether the party can carry one more of item; when it cannot, says
/// so and returns -1, else 0.
s32 Party_CheckItemLimit(s32 item) {
  // DEAD: the count never used; the ROM keeps the call.
  Party_CountMembersFar();
  if (PartyInventory_FindRoomFar(item) < 0) {
    MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
    // "Felix's party cannot carry any more <item>s..."
    FieldMessage_ShowFar((s32)MsgPartyCannotCarry, FIELD_MESSAGE_ZOOM);
    MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
    // "Felix threw the <item> as far away as possible."
    FieldMessage_ShowFar((s32)MsgPartyCannotCarry + 1, FIELD_MESSAGE_ZOOM);
    return -1;
  }
  return 0;
}

/// The event command "give item": gives item to the first party member with
/// room for it and says so; with no room, the player chooses what to drop.
s32 PartyInventory_GiveItem(s32 item, s32 unused) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  s16 saved = state->message;
  s32 owner;
  s32 result;
  s32 count;
  s32 i;
  s32 member;
  s32 slot;

  owner = PartyInventory_AddFar(item);
  if (owner == -1) {
    MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
    // "Felix got <item>."
    FieldMessage_ShowFar((s32)&MsgItemReceived, FIELD_MESSAGE_ZOOM);
    // "But you can't carry any more items."
    FieldMessage_ShowFar((s32)&MsgItemCannotCarry, FIELD_MESSAGE_ZOOM);
  retry: {
    // "What will you drop?"
    FieldMessage_ShowFar((s32)&MsgItemDropPrompt, FIELD_MESSAGE_ZOOM);
    result = ItemPicker_RunToDropFar(&member, &slot);
    if (result == -1) {
      if (Item_GetDirectFar(item)->flags & ITEM_INDISPENSABLE) {
        MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
        // "That <item>'s valuable! You must get it!"
        FieldMessage_ShowFar((s32)&MsgItemDropPrompt + 4, FIELD_MESSAGE_ZOOM);
        goto retry;
      }
      MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
      // "Drop the <item>?"
      FieldMessage_ShowFar((s32)&MsgItemDropPrompt + 1,
                           FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_KEEP_OPEN);
      result = FieldYesNo_Ask(1);
      Message_CloseFinishedWindowsFar();
      if (result != 0)
        goto retry;
      Item_AdjustRareItemStockFar(item, 1);
      MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
      // "Felix drops the <item>."
      FieldMessage_ShowFar((s32)&MsgItemDropPrompt + 2, FIELD_MESSAGE_ZOOM);
      goto done;
    }
    // A carried item was picked: drop it (the whole stack) and take the new
    // one. A party member who already carries INVENTORY_STACK_MAX of the new
    // item cannot take it: say so and ask again.
    Unit_Get(member);
    count = Inventory_GetQuantityFar(member, slot);
    if (Inventory_CountItemFar(member, item) >= INVENTORY_STACK_MAX) {
      MessageArgument_PushFar(member, MESSAGE_ARGUMENT_ACTOR);
      MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
      // "Empty": the line every edition's text has here, the word for an
      // empty inventory slot, though the member and item pushed suit a
      // sentence such as MsgPartyCannotCarry's.
      FieldMessage_ShowFar((s32)&MsgItemDropPrompt + 7, FIELD_MESSAGE_ZOOM);
      goto retry;
    }
    for (i = 0; i < count; i++)
      Inventory_DiscardFar(member, slot);
    Unit_RefreshPsynergyFar(member);
    Unit_CalculateFar(member);
    owner = PartyInventory_AddFar(item);
    Sound_PlayFar(MUS_TREASURE_FOUND);
    if (owner == gMainRoutineState.leaderActorId) {
      MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
      FieldMessage_ShowFar((s32)&MsgItemReceived,
                           FIELD_MESSAGE_ZOOM |
                               FIELD_MESSAGE_WAITING_FOR_SOUND);
    } else {
      MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
      MessageArgument_PushFar(owner, MESSAGE_ARGUMENT_ACTOR);
#if !TLA_PATCH_3
      FieldMessage_ShowFar((s32)&MsgItemGiven, GIFT_RECEIVED_MESSAGE_MODE);
#endif
    }
  }
  done:
    state->message = saved;
    return owner;
  }
  // There was room: "Felix got <item>.", and "Felix gave <item> to
  // <member>." when someone other than the leader carries it.
  Sound_PlayFar(MUS_TREASURE_FOUND);
  MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
  FieldMessage_ShowFar((s32)&MsgItemReceived,
                       FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_WAITING_FOR_SOUND);
  if (owner != gMainRoutineState.leaderActorId) {
    MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
    MessageArgument_PushFar(owner, MESSAGE_ARGUMENT_ACTOR);
    FieldMessage_ShowFar((s32)&MsgItemReceived + 1, FIELD_MESSAGE_ZOOM);
  }
  state->message = saved;
  return owner;
}

/// Gives item to one party member. Returns unit, or -1 if its bag is full.
s32 Inventory_TryAddAndReturnMember(s32 item, s32 unused, s32 unit) {
  if (Inventory_AddItemFar(unit, item) >= 0)
    return unit;
  return -1;
}

/// An empty function nothing calls.
UNUSED void Inventory_NoOpCallback(void) {}

/// An empty function nothing calls.
UNUSED void Inventory_ReservedNoOp(void) {}

/// Waits, at most 90 frames, for an actor's animation to stop being animation:
/// the actor has finished a one-off animation such as a nod.
void Actor_WaitForAnimationEnd(s32 actorId, s32 animation) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != 0 && actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
    s32 frames;
    struct Sprite *sprite = actor->sprite;

    for (frames = 0; frames <= 89; frames++) {
      wait(1);
      if (animation != sprite->lastNo) {
        break;
      }
    }
  }
}

/// The last tile row the message windows may reach with a yes-or-no question
/// still asked below them.
#define YES_NO_BELOW_LAST_ROW 15

/// The event command "choose yes or no": once the message on screen has been
/// read, asks yes or no below it, or above it when the message windows reach
/// past YES_NO_BELOW_LAST_ROW (unless forceBottom is set). The speaker nods
/// (ANIM_NOD) at yes and shakes its head (ANIM_SHAKE_HEAD) at no; at the
/// fastest message speed neither is waited for.
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 forceBottom) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  s32 sprite = *(s16 *)Scene_FindActorPlacement(state->speaker);
  struct Window *window = state->messageWindow;
  struct Window *side = state->sideWindow;
  s32 useBottom = 1;
  s32 answer;

  while (Message_AllTypedFar() == 0)
    wait(1);

  wait((MESSAGE_SPEED_FAST - gMainRoutineState.messageSpeed) * 3);

  if (forceBottom == 0) {
    s32 bottom = window->y + window->height;

    if (side != NULL) {
      s32 sideBottom = side->y + side->height;
      if (bottom < sideBottom)
        bottom = sideBottom;
    }

    if (bottom > YES_NO_BELOW_LAST_ROW)
      useBottom = 0;
  }

  answer = YesNoMenu_RunFar(useBottom, state->choiceMessage, state->choiceWidth,
                            state->choiceSelection);
  if (answer != 0) {
    Actor_SetAnimationById(actor, ANIM_SHAKE_HEAD);
    Portrait_CloseWindowFar(sprite);
    Message_CloseFinishedWindowsFar();
    if (gMainRoutineState.messageSpeed != MESSAGE_SPEED_FAST)
      Actor_WaitForAnimationEnd(actor, ANIM_SHAKE_HEAD);
  } else {
    Actor_SetAnimationById(actor, ANIM_NOD);
    Portrait_CloseWindowFar(sprite);
    Message_CloseFinishedWindowsFar();
    if (gMainRoutineState.messageSpeed != MESSAGE_SPEED_FAST)
      Actor_WaitForAnimationEnd(actor, ANIM_NOD);
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
    ItemDisplay_ShowItem(item, ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
    MessageArgument_PushFar(item, MESSAGE_ARGUMENT_ITEM);
    // "Felix found <item>."
    FieldMessage_ShowFar((s32)&MsgFoundItem, FIELD_MESSAGE_ZOOM);
    // "But Felix's party can't carry any more, so they left it behind."
    FieldMessage_ShowFar((s32)&MsgFoundItem + 4, FIELD_MESSAGE_ZOOM);
    Actor_GetById(actor)->scaleX = FIXED_ONE;
    Actor_FaceActorById(gMainRoutineState.leaderActorId, actor, 0);
    Event_Wait(20);
    return -1;
  }
  Actor_SetHorizontalPositionWithTerrain(actor, 0, 0);
  ItemDisplay_ShowItem(item, ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
  PartyInventory_GiveItem(item, 0);
  if (flag != -1)
    GameFlag_Set(flag);
  return 0;
}

/// Teaches unit the Psynergy ability and says so with its jingle, waiting for
/// the player to read it.
void Party_LearnAbility(s32 unit, s32 ability) {
  Unit_LearnPsynergyFar(unit, ability);
  Sound_PlayFar(MUS_TREASURE_FOUND);
  MessageArgument_PushFar(unit, MESSAGE_ARGUMENT_ACTOR);
  MessageArgument_PushFar(ability, MESSAGE_ARGUMENT_ABILITY);
  // "<member> learned <ability>."
  FieldMessage_ShowFar(MsgAbilityLearned,
                       FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_WAITING_FOR_SOUND);
}

/// The event command "open screen": starts the event's screen transition and
/// sets transitionActive.
void Event_StartScreenTransition(void) {
  struct FieldState *field = gFieldState;

  ScreenTransition_Start(field->transition, field->transitionFrames);
  field->transitionActive = 1;
}

/// The event command "close screen": finishes the event's screen transition
/// and clears transitionActive.
void Event_FinishScreenTransition(void) {
  struct FieldState *field = gFieldState;

  ScreenTransition_Finish(field->transition, field->transitionFrames);
  field->transitionActive = 0;
}

/// The event command "wait for screen": waits as long as the transition runs
/// (transitionFrames).
void Event_WaitTransitionFrames(void) { wait(gFieldState->transitionFrames); }

/// Makes the event leave for the given entrance of the given scene: raises the
/// exit request EXIT_KEEP_DESTINATION and writes gMainRoutineState.next and
/// entrance.
void Event_SetDestination(s32 sceneId, s32 entrance) {
  gFieldState->exit = EXIT_KEEP_DESTINATION;
  gMainRoutineState.next = sceneId;
  gMainRoutineState.entrance = entrance;
}

/// Makes the event leave and records sceneId and entrance as the restart
/// point.
void Event_SetRestartPoint(s32 sceneId, s32 entrance) {
  struct FieldState *field = Ram_HeapSlots->fieldState;
  struct MainRoutineState *party;

  field->exit = EXIT_KEEP_DESTINATION;
  party = &gMainRoutineState;
  party->restartScene = sceneId;
  party->restartEntrance = entrance;
}

/// The event command "request exit": sets the field's exit request
/// (EXIT_KEEP_DESTINATION leaves for gMainRoutineState's next and entrance).
void Event_SetExitRequest(u16 exit) {
  struct FieldState *field = gFieldState;
  field->exit = exit;
}

#undef GIFT_RECEIVED_MESSAGE_MODE
#undef YES_NO_BELOW_LAST_ROW

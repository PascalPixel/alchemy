/// @file
/// Event commands: waits, received items, questions and destinations.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

// When a gift goes to another member, `<leader> gave <item> to <member>.`
// follows `<leader> got <item>.`; it is MsgGaveItemToMember, the next
// message, in every edition. Japanese and English show it with window flags
// 3 (zoom, busy while it shows), the European releases with 1 (zoom only).
#if (LANGUAGE == LANGUAGE_GERMAN)
#define GIFT_RECEIVED_MESSAGE MsgGaveItemToMember
#define GIFT_RECEIVED_MESSAGE_MODE 1
#elif (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||       \
    (LANGUAGE == LANGUAGE_ITALIAN)
#define GIFT_RECEIVED_MESSAGE (MsgColossoIsaacGotItem + 1)
#define GIFT_RECEIVED_MESSAGE_MODE 1
#else
#define GIFT_RECEIVED_MESSAGE (MsgColossoIsaacGotItem + 1)
#define GIFT_RECEIVED_MESSAGE_MODE 3
#endif

/// Shows the message for an item the party has just received. The message is
/// `Isaac got <item>.`; the German release has its own sentence for the Black
/// Orb. flags are the message window flags of
/// UiText_ShowPositionedMessageAndWait.
static inline void ItemGift_ShowGotItem(s32 item, s32 flags) {
#if (LANGUAGE == LANGUAGE_GERMAN)
  if (item == ITEM_BLACK_ORB) {
    UiText_ShowPositionedMessageAndWaitFar(MsgGotBlackOrb, flags);
    return;
  }
#endif
  UiWork_PushValueSlotFar(item, 2);
  UiText_ShowPositionedMessageAndWaitFar(MsgColossoIsaacGotItem, flags);
}

/// Debug only: while debug mode is on, R makes every event wait
/// (FieldEvent_WaitFrames) end at once and L restores normal waits. Runs as a
/// scheduler callback for the length of an event (FieldEvent_Begin). R also
/// skips the actors' messages, which open nothing while fastForward is set
/// (UiText_OpenMessageAtObject).
void Event_UpdateDebugSkip(void) {
  struct FieldWork *runtime = (struct FieldWork *)gEventWork;

  if (gDebugMode != 0) {
    if (gKeysPressed & KEY_L) {
      runtime->fastForward = 0;
    }
    if (gKeysPressed & KEY_R) {
      runtime->fastForward = -1;
    }
  }
}

/// The event command "wait": waits the requested frames, unless debug fast
/// forward (Event_UpdateDebugSkip) is on. Scene scripts reach it as
/// Engine_EventWait (the WAIT command); this object's actor, message, camera
/// and climbing commands wait through it too.
void FieldEvent_WaitFrames(s32 frames) {
  if (((struct FieldWork *)gEventWork)->fastForward == 0 && frames != 0) {
    WaitFrames(frames);
  }
}

/// Stops the leader where it stands: no horizontal velocity, the normal walking
/// speed (1.0) and acceleration (0.5), no destination, and the standing pose,
/// or the climbing pose (12) while the leader is on a climbable wall. Called by
/// FieldEvent_Begin, by Field_RunScene before it handles several requests, by
/// Field_UsePsynergy, Scene_TalkToActor, Field_CheckExitRegions and the world
/// map's zoom (BattleFx_RunVisibilityTransition).
void Leader_StopMoving(void) {
  struct ObjectRuntime *object;

  object = ObjectTable_Get(gGameState.selectedActor);
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  object->targetX = ACTOR_NO_TARGET;
  object->targetZ = ACTOR_NO_TARGET;
  object->velocityX = 0;
  object->velocityZ = 0;
  if (gGameState.movementMode == 1) {
    ObjectDispatch_SelectAnimationFar(object, 0xC);
    return;
  }
  ObjectDispatch_SelectAnimationFar(object, 1);
}

/// The event command "begin event": closes any timed notice, stops the leader,
/// resets the message and timing state, and starts the debug fast-forward
/// callback with the leader as the message speaker. Scene scripts reach it as
/// Engine_EventBegin (EV_BEGIN); the field's item, Psynergy, Djinni, Retreat
/// and climbing events open with it too. Clears the field work's name window,
/// fastForward and messageWindow, sets transitionFrames to 16 and the speaker
/// to the leader.
void FieldEvent_Begin(void) {
  struct FieldWork *runtime = (struct FieldWork *)gEventWork;

  UiTimedNotice_CloseIfActiveFar();
  Leader_StopMoving();
  // Keep the event reset's signed halfword read.
  if ((s16)runtime->busy != 0) {
    Battle_ResetEffectCounter();
  }
  // Every event starts with no name window and normal waits.
  runtime->nameMessage = 0;
  runtime->nameWidth = 0;
  runtime->transitionFrames = 0x10;
  runtime->fastForward = 0;
  runtime->unk1da = 0xFFFF;
  runtime->unk1dc = -1;
  runtime->unk1de = -1;
  Scheduler_AddOrUpdateCallback(Event_UpdateDebugSkip, 0xC80);
  // XXX: flag 0x132 is cleared at every event and nothing in ☀️ names it; the
  // scene scripts or overlays that set or test 0x132 would settle its meaning.
  GameFlag_ClearBitFar(0x132);
  runtime->speaker = gGameState.selectedActor;
  runtime->messageWindow = 0;
}

/// Ends an event: stops the debug fast-forward callback, gives the camera back
/// to the leader and refreshes FLAG_LURE_CAP (GameFlag_RefreshLureCap). Scene
/// scripts reach it as Engine_EventEnd (EV_END), and every event that opens
/// with FieldEvent_Begin closes with it.
void FieldEvent_End(void) {
  Scheduler_RemoveCallback(Event_UpdateDebugSkip);
  Camera_FollowActor(gGameState.selectedActor, 1);
  GameFlag_RefreshLureCapFar();
}

/// Unused: an empty function nothing calls.
UNUSED void Event_NoOpHookA(void) {}

/// An empty hook; it does nothing in the released game. Only
/// Event_RunObjectHookAndWait calls it.
void Event_NoOpHook(void) {}

/// Respawns the scene's actors from a placement table and waits one frame for
/// them to appear. Reached only through an unnamed veneer in the object bank's
/// far-call table; nothing in ☀️ names it.
void Event_RunObjectHookAndWait(struct ScenePlacement *table) {
  Event_NoOpHook();
  ObjectTable_ResetForObject(table);
  WaitFrames(1);
  // XXX: the leader's object is fetched and not used; a caller in an overlay
  // that reads the result would show whether this returned it once.
  ObjectTable_Get(gGameState.selectedActor);
}

/// Spawns the actors of a placement table after the last actor in use. Called
/// by the Kolima, Kalay and Tolbi ship scene scripts to bring actors in during
/// a scene.
void Event_CallWithLastActiveObjectId(struct ScenePlacement *table) {
  Event_SpawnObjectTable(table, ObjectTable_FindLastActiveId());
}

/// Sets the scene's event table, which the field checks as the leader moves:
/// writes work->events. Altin Peak's scene script calls it as
/// Event_LoadAreaScript.
void Event_SetSceneEvents(const struct SceneEvent *events) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;

  work->events = events;
}

/// Adds unit to the party. With announce nonzero it also shows
/// `<name> joined your party.` (Party_ShowJoinedMessage, which receives
/// announce too). Called by six scene scripts as a character joins (Vale,
/// the Vault, Goma Cave and Mercury Lighthouse among them).
void Event_PrepareObjectAndApplyValue(s32 unit, s32 announce) {
  UnitGetFar(unit);
  Party_AddActiveOwnerFar(unit);
  if (announce != 0) {
    Party_ShowJoinedMessageFar(unit, announce);
  }
}

/// Adds two units to the party and announces them together. Vale's scene
/// script calls it twice, as Engine_PartyAddMembers.
void Event_PrepareTwoObjectsAndApply(s32 first, s32 second) {
  Party_AddActiveOwnerFar(first);
  Party_AddActiveOwnerFar(second);
  Party_ShowPairJoinedMessageFar(first, second);
}

/// Checks an L or R button shortcut, packed as the member in bits 10 to 13 and
/// the Psynergy in bits 0 to 9. Returns 0 if it can still be cast, -1 for no
/// such party member, -2 when that member is not in the party and -3 when the
/// member no longer knows the Psynergy. Only Event_ClearInvalidPackedValues
/// calls it.
s32 Event_ValidatePackedId(u32 shortcut) {
  s32 member = (shortcut >> 10) & 0xf;
  s32 ability = shortcut & 0x3ff;

  if (member > 7)
    return -1;
  // Flags 0 to 7 say who is in the party (FLAG_IN_PARTY).
  if (GameFlag_TestFar(member) == 0)
    return -2;
  if (Unit_KnowsActionFar(member, ability) == 0)
    return -3;
  return 0;
}

/// Clears each button shortcut whose Psynergy can no longer be cast: zeroes
/// gGameState.firstShortcut or secondShortcut. Called whenever the party or
/// what it knows may have changed: by Party_RemoveOwnerRestored,
/// Inventory_Discard, and the field menu's item, Psynergy and Djinn screens.
void Event_ClearInvalidPackedValues(void) {
  if (Event_ValidatePackedId(gGameState.firstShortcut) != 0)
    gGameState.firstShortcut = 0;
  if (Event_ValidatePackedId(gGameState.secondShortcut) != 0)
    gGameState.secondShortcut = 0;
}

/// Takes unit out of the party, fully restoring its HP and PP and curing its
/// poison. If no member left standing has HP, the leader is given 1 HP so the
/// party is never left all down. Called by the scene scripts where a member
/// leaves: Vale in the storm, the Vault, and Sol Sanctum.
void Party_RemoveOwnerRestored(s32 unit) {
  struct BattleUnit *vitals;
  s32 partyIndex;
  s32 partyCount;
  s32 survivingMembers;

  Party_RemoveActiveOwnerFar(unit);
  Event_ClearInvalidPackedValues();
  vitals = UnitGetFar(unit);
  vitals->hp = vitals->maxHp;
  vitals->pp = vitals->maxPp;
  Unit_UpdateGauges(vitals);
  vitals->poison = 0;
  // Count the members who remain and still have HP.
  survivingMembers = 0;
  partyCount = Party_CountActiveOwnersFar();
  for (partyIndex = 0; partyIndex < partyCount; partyIndex++) {
    vitals = UnitGetFar(gGameState.activeOwners[partyIndex]);
    if (vitals->hp != 0)
      survivingMembers++;
  }
  if (survivingMembers == 0) {
    vitals = UnitGetFar(gGameState.selectedActor);
    vitals->hp = 1;
    Unit_UpdateGauges(vitals);
  }
}

/// Checks whether the party can carry one more of item: each member carries at
/// most 30 of one item. When they are all full, says
/// `Isaac's party cannot carry any more <item>s...` and
/// `Isaac threw the <item> as far away as possible.` and returns -1; otherwise
/// returns 0. Field_RunScene calls it before it gives gGameState.pendingItem
/// with PartyInventory_GiveItem.
s32 Party_CheckItemLimit(s32 item) {
  s32 partyCount;
  s32 heldQuantity = 0;
  s32 partyIndex;

  partyCount = Party_CountActiveOwnersFar();
  for (partyIndex = 0; partyIndex < partyCount; partyIndex++)
    heldQuantity +=
        Inventory_CountItemFar(gGameState.activeOwners[partyIndex], item);

  if (heldQuantity >= partyCount * 30) {
    UiWork_PushValueSlotFar(item, 2);
    UiText_ShowPositionedMessageAndWaitFar(MsgCannotCarryMore, 1);
    UiWork_PushValueSlotFar(item, 2);
    UiText_ShowPositionedMessageAndWaitFar(MsgCannotCarryMore + 1, 1);
    return -1;
  }
  return 0;
}

/// The event command "give item": gives item to the first party member with
/// room for it and says so. When every bag is full, the player picks an item
/// to drop to make room, or drops the new item instead (not allowed for an
/// indispensable one). Returns the member who received the item, or -1 when
/// it was left behind. Scene scripts reach it as Engine_PartyGiveItem (about
/// 24 of them); Field_RunScene gives the pending item and the debug test menu
/// gives items through it. The field work's message is restored afterwards.
s32 PartyInventory_GiveItem(s32 item, s32 unused) {
  // A for/continue retry loop shrinks native English 0x1c4 -> 0x1a8;
  // retain the original backedges until a plain form matches.
  struct FieldWork *work;
  s32 savedMessage;
  s32 owner;
  s32 result;
  s32 discardQuantity;
  s32 discardIndex;
  s32 member;
  s32 slot;

  work = (struct FieldWork *)gEventWork;
  // The messages below move the event's message on; every return puts it
  // back, so the scene script continues where it was.
  savedMessage = work->message;
  owner = PartyInventory_AddFar(item);
  if (owner == -1) {
    // Every bag is full: "But you can't carry any more items."
    ItemGift_ShowGotItem(item, 1);
    UiText_ShowPositionedMessageAndWaitFar(MsgCannotCarryAnyMore, 1);
  retry:
    UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop, 1);
    result = Shop_PickUnitItemFar(&member, &slot);
    if (result == -1) {
      // Cancelled: drop the new item instead, unless it is indispensable
      // ("That <item>'s valuable! You must get it!").
      if (Item_GetDirectFar(item)->flags & ITEM_INDISPENSABLE) {
        UiWork_PushValueSlotFar(item, 2);
        UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 4, 1);
        goto retry;
      }
      // "Drop the <item>?"
      UiWork_PushValueSlotFar(item, 2);
      UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 1, 5);
      result = Menu_AskYesNo(1);
      UiWork_FinalizePendingCoreFar();
      if (result != 0)
        goto retry;
      // "Isaac drops the <item>."
      Item_AdjustCounterFar(item, 1);
      UiWork_PushValueSlotFar(item, 2);
      UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 2, 1);
      work->message = savedMessage;
      return owner;
    } else {
      // A carried item was picked: drop it (the whole stack) and take the new
      // one.
      UnitGetFar(member);
      {
        /* FAKEMATCH(temporary): the slot copied to a block local loads before
         * the member; as Shop_GetSelectionState(member, slot), 2 of 181
         * instructions differ in tbs-en. */
        s32 selectedSlot = slot;

        discardQuantity = Shop_GetSelectionState(member, selectedSlot);
      }
      // A member who already carries 30 of the new item (the cap
      // Party_CheckItemLimit counts) cannot take it: say so and ask again.
      // Editions at TBS_PATCH_1 alone skip the check.
#if !(TBS_PATCH_1 && !TBS_PATCH_2)
      if (Inventory_CountItemFar(member, item) > 29) {
        UiWork_PushValueSlotFar(member, 1);
        UiWork_PushValueSlotFar(item, 2);
        UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 7, 1);
        goto retry;
      }
#endif
      for (discardIndex = 0; discardIndex < discardQuantity; discardIndex++)
        Inventory_DiscardFar(member, slot);
      owner = PartyInventory_AddFar(item);
      AudioCommand_PlayFar(MUS_TREASURE_FOUND);
      if (owner == gGameState.selectedActor) {
        ItemGift_ShowGotItem(item, 3);
      } else {
        UiWork_PushValueSlotFar(item, 2);
        UiWork_PushValueSlotFar(owner, 1);
        UiText_ShowPositionedMessageAndWaitFar(MsgGaveItemToMember, 3);
      }
      work->message = savedMessage;
      return owner;
    }
  } else {
    // There was room: "Isaac got <item>.", and "Isaac gave <item> to
    // <member>." when someone other than the leader carries it.
    AudioCommand_PlayFar(MUS_TREASURE_FOUND);
    ItemGift_ShowGotItem(item, 3);
    if (owner != gGameState.selectedActor) {
      UiWork_PushValueSlotFar(item, 2);
      UiWork_PushValueSlotFar(owner, 1);
      UiText_ShowPositionedMessageAndWaitFar(GIFT_RECEIVED_MESSAGE,
                                             GIFT_RECEIVED_MESSAGE_MODE);
    }
    work->message = savedMessage;
  }
  return owner;
}

/// Gives item to one party member. Returns unit, or -1 if its bag is full.
/// Reached only through an unnamed veneer in the object bank's far-call
/// table; nothing in ☀️ names it.
s32 Inventory_TryAddAndReturnOwner(s32 item, s32 unused, s32 unit) {
  if (Inventory_AddItemFar(unit, item) >= 0)
    return unit;
  return -1;
}

/// An empty function; nothing in ☀️ calls it.
void Inventory_NoOpCallback(void) {}

/// An empty function; nothing in ☀️ calls it.
void Inventory_ReservedNoOp(void) {}

/// Waits, at most 90 frames, for an actor's animation to stop being animation:
/// the actor has finished a one-off animation such as a nod. Only actors drawn
/// as an animation object (OBJECT_SPRITE_ONE) are waited for. Called by
/// Event_AskYesNoWithActorAnimation.
void Actor_WaitForAnimationEnd(s32 actor, s32 animation) {
  struct ObjectRuntime *actorObject = ObjectTable_Get(actor);

  if (actorObject != 0 && actorObject->animationKind == OBJECT_SPRITE_ONE) {
    s32 frames = 0;
    u8 *animationNumber =
        &((struct AnimationObject *)actorObject->animation)->lastNo;

    while (frames <= 89) {
      WaitFrames(1);
      if (animation != *animationNumber) {
        break;
      }
      frames++;
    }
  }
}

/// The event command "choose yes or no": once the message on screen has been
/// read, asks yes or no below it, or above it when the message windows reach
/// past row 15 or forceBottom is set. The speaker nods (animation 3) at yes
/// and shakes its head (animation 4) at no. Returns 0 for yes, 1 for no.
/// Scene scripts reach it as Engine_EventChooseYesNo (about 54 of them); the
/// link lobby and FieldEvent_AskYesNoAndShowResponse call it too.
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 forceBottom) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  s32 sprite = Scene_FindActorPlacement(work->speaker)->sprite;
  struct UiWindow *window = (struct UiWindow *)work->messageWindow;
  struct UiWindow *side = (struct UiWindow *)work->sideWindow;
  s32 useBottom = 1;
  s32 answer;

  // Wait for every button to be released and the message to finish.
  while (gKeysPressed != 0)
    WaitFrames(1);

  while (UiWork_IsCompleteFar() == 0)
    WaitFrames(1);

  WaitFrames(3);

  // The question goes above the message windows when they reach past row 15,
  // so it does not cover them.
  if (forceBottom == 0) {
    s32 bottom = window->y + window->height;

    if (side != 0) {
      s32 sideBottom = side->y + side->height;
      if (bottom < sideBottom)
        bottom = sideBottom;
    }

    if (bottom > 15)
      useBottom = 0;
  }

  answer = Menu_RunConfirmSelectionFar(useBottom, work->nameMessage,
                                       work->nameWidth, 0);
  if (answer != 0) {
    Object_SetModeById(actor, 4);
    UiWork_FinalizeEntityMatchingLocalizedIdFar(sprite);
    UiWork_FinalizePendingCoreFar();
    Actor_WaitForAnimationEnd(actor, 4);
  } else {
    Object_SetModeById(actor, 3);
    UiWork_FinalizeEntityMatchingLocalizedIdFar(sprite);
    UiWork_FinalizePendingCoreFar();
    Actor_WaitForAnimationEnd(actor, 3);
  }

  return answer;
}

/// Asks yes or no at the top of the screen, or at the bottom with atBottom
/// set. Returns 0 for yes, 1 for no. Called by PartyInventory_GiveItem,
/// Field_UseItem, Field_UsePsynergy and the cleared game's menu.
s32 Menu_AskYesNo(s32 atBottom) {
  return Menu_RunConfirmSelectionFar(atBottom, 0, 0, 0);
}

/// Teaches unit the Psynergy ability and says so with its jingle, waiting for
/// the player to read it. The message is `<unit> learned <ability>.` Fuchin
/// Temple's scene script calls it.
void Party_LearnAbility(s32 unit, s32 ability) {
  // Declared without parameters: unit and ability pass through in r0 and r1.
  Unit_LearnPsynergyFar();
  AudioCommand_PlayFar(MUS_TREASURE_FOUND);
  UiWork_PushValueSlotFar(unit, 1);
  UiWork_PushValueSlotFar(ability, 4);
  UiText_ShowPositionedMessageAndWaitFar(MsgAbilityLearned, 3);
}

/// The event command "open screen": starts the event's screen transition and
/// sets work->transitionActive. Scene scripts reach it as
/// Engine_EventOpenScreen; the inn, the title, cleared-game and link lobby
/// menus and Map_ShowWorldMap call it too.
void Event_StartScreenTransition(void) {
  struct FieldWork *runtime = (struct FieldWork *)gEventWork;

  DisplayTransition_Start(runtime->transition, runtime->transitionFrames);
  runtime->transitionActive = 1;
}

/// The event command "close screen": finishes the event's screen transition
/// and clears work->transitionActive. Scene scripts reach it as
/// Engine_EventCloseScreen; the inn, the cleared-game menu and
/// Map_ShowWorldMap call it too.
void Event_FinishScreenTransition(void) {
  struct FieldWork *runtime = (struct FieldWork *)gEventWork;

  DisplayTransition_Finish(runtime->transition, runtime->transitionFrames);
  runtime->transitionActive = 0;
}

/// The event command "wait for screen": waits as long as the transition runs
/// (work->transitionFrames). Scene scripts reach it as
/// Engine_EventWaitForScreen.
void Event_WaitTransitionFrames(void) {
  WaitFrames(((struct FieldWork *)gEventWork)->transitionFrames);
}

/// Makes the event leave for the given entrance of the given scene: raises the
/// exit request 999 and writes gGameState.scene and entrance. Called by the
/// title, cleared-game and link lobby menus, the debug battle command and five
/// scene scripts (the Lemurian ship and the world map among them).
void Event_SetDestination(s32 scene, s32 entrance) {
  ((struct FieldWork *)gEventWork)->exit = 999;
  gGameState.scene = scene;
  gGameState.entrance = entrance;
}

/// Makes the event leave and records scene and entrance as the saved
/// destination. Reached only through an unnamed veneer in the object bank's
/// far-call table; nothing in ☀️ names it.
void Event_SetSavedDestination(u16 scene, u16 entrance) {
  struct FieldWork *state = (struct FieldWork *)gEventWork;
  state->exit = 999;
  gGameState.savedScene = scene;
  gGameState.savedEntrance = entrance;
}

/// The event command "request exit": sets the field's exit request (999 leaves
/// for gGameState's scene and entrance). Scene scripts reach it as
/// Engine_EventRequestExit (about 61 of them).
void Event_SetExitRequest(u16 request) {
  struct FieldWork *state = (struct FieldWork *)gEventWork;
  state->exit = request;
}

#undef GIFT_RECEIVED_MESSAGE
#undef GIFT_RECEIVED_MESSAGE_MODE

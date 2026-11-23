/// @file
/// The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3):
/// before Isaac runs, Garet, Ivan and Mia each look around the stages on their
/// own and choose where to cheer from, and the guide at each stage describes it
/// and offers a friend's cheering there.
///
/// Each match scene defines its own two exits before including this file:
/// COLOSSO_SOLO_TO_RACE, its way from lending a hand to the race, and
/// COLOSSO_SOLO_TO_COLOSSEUM, its way back out to the Colosseum.

/// Byte flags (eight flags each) holding where a friend cheers from: the cell
/// column at COLOSSO_CHEER_COLUMN + 16 * character and the cell row at
/// COLOSSO_CHEER_ROW + 16 * character, for Garet, Ivan and Mia.
#define COLOSSO_CHEER_COLUMN FLAG_AREA_TEMP_70
#define COLOSSO_CHEER_ROW FLAG_AREA_TEMP_78
#define COLOSSO_CHEER_SLOT(character) ((character) << 4)

/// Map temp flags, by stage number: a friend cheers at the stage, and its
/// description was heard. When a friend cheers there, the guide says
/// "Unfortunately, we have a full house."
#define COLOSSO_STAGE_CHEERED FLAG_SCENE_TEMP_00
#define COLOSSO_STAGE_DESCRIBED FLAG_SCENE_TEMP_08

/// Puts Garet, Ivan and Mia back at the cells where they chose to cheer, at the
/// centre of each cell.
void Colosso_PlaceFriendsAtCheeringSpots(void) {
  {
    s32 x =
        FieldGameFlag_GetByte(COLOSSO_CHEER_COLUMN + COLOSSO_CHEER_SLOT(GARET));
    s32 y =
        FieldGameFlag_GetByte(COLOSSO_CHEER_ROW + COLOSSO_CHEER_SLOT(GARET));

    Actor_SetPositionFX(ACTOR_GARET, FIELD_CELL_CENTER(x),
                        FIELD_CELL_CENTER(y));
  }
  {
    s32 x =
        FieldGameFlag_GetByte(COLOSSO_CHEER_COLUMN + COLOSSO_CHEER_SLOT(IVAN));
    s32 y = FieldGameFlag_GetByte(COLOSSO_CHEER_ROW + COLOSSO_CHEER_SLOT(IVAN));

    Actor_SetPositionFX(ACTOR_IVAN, FIELD_CELL_CENTER(x), FIELD_CELL_CENTER(y));
  }
  {
    s32 x =
        FieldGameFlag_GetByte(COLOSSO_CHEER_COLUMN + COLOSSO_CHEER_SLOT(MIA));
    s32 y = FieldGameFlag_GetByte(COLOSSO_CHEER_ROW + COLOSSO_CHEER_SLOT(MIA));

    Actor_SetPositionFX(ACTOR_MIA, FIELD_CELL_CENTER(x), FIELD_CELL_CENTER(y));
  }
}

/// Makes the character the only party member, the one the player moves and the
/// camera follows, with full HP and PP and no poison.
void Colosso_ControlCharacterAlone(s32 id) {
  struct Unit *unit;

  Party_RemoveMember(ISAAC);
  Party_RemoveMember(GARET);
  Party_RemoveMember(IVAN);
  Party_RemoveMember(MIA);
  Party_RemoveMember(JENNA);
  Party_AddMember(id);
  gMainRoutineWork.leaderActorId = id;
  FieldCamera_FollowActor(id, 0);
  unit = Unit_Get(id);
  unit->hp = unit->hpMax;
  unit->pp = unit->ppMax;
  unit->poison = 0;
  Unit_UpdateGauges(unit);
  Party_ClearUnusableShortcuts();
}

/// The trigger each match scene raises for Colosso_TakePlaceForMatch.
#define COLOSSO_TAKE_PLACE_TRIGGER 99

/// A task while a friend looks around the stages alone (the visit to
/// lend a hand): when the Psynergy request names that friend and
/// FLAG_PSYNERGY_TARGET is set, raise COLOSSO_TAKE_PLACE_TRIGGER.
void Colosso_RaiseTakePlaceOnPsynergy(void) {
  struct FieldWork *work = gFieldWork;
  s32 selection = gMainRoutineWork.leaderActorId;

  if (selection != 0 && (work->psynergy >> ACT_UNIT_SHIFT) == selection &&
      GameFlag_Get(FLAG_PSYNERGY_TARGET) != 0) {
    u16 *raised = (u16 *)&work->raisedTrigger;
    s32 trigger = COLOSSO_TAKE_PLACE_TRIGGER;

    *raised = trigger;
  }
}

/// COLOSSO_TAKE_PLACE_TRIGGER: the nearest fair-haired young man level with or
/// south of the friend calls the warriors to their places, and the
/// friend's cell becomes their place to cheer from. "The match is about to
/// begin. Please take your places." The next friend then looks around, or,
/// after Mia, the party leaves for the race.
static void Colosso_TakePlaceForMatch(void) {
  struct FieldWork *work = gFieldWork;
  s32 speakerId = ACTOR_FIRST_PLACED;
  FIXED nearestDistance = toFIXED(16);
  s32 supporterId = gMainRoutineWork.leaderActorId;
  struct Actor *supporter = Actor_GetById(supporterId);
  s32 actorId;
  s32 *transition;
  s32 flagBase;

  Event_Start();
  // BUG: one past the actor table: Actor_Lookup(ACTOR_ID_COUNT) is the first
  // treasure spot's actor, which is looked at too; it is never the speaker.
#ifdef BUGFIX
  for (actorId = ACTOR_FIRST_PLACED; actorId < ACTOR_ID_COUNT; actorId++) {
#else
  for (actorId = ACTOR_FIRST_PLACED; actorId <= ACTOR_ID_COUNT; actorId++) {
#endif
    struct Actor *actor = Actor_GetById(actorId);

    if (actor != NULL && actor->spriteStorageKind == ACTOR_SPRITE_ONE &&
        actor->sprite->entries[0]->animationId ==
            SPRITE_YOUNG_MAN_WITH_FAIR_HAIR) {
      s32 distanceX = (supporter->x.fixed - actor->x.fixed) / toFIXED(1);
      s32 distanceZ = (supporter->z.fixed - actor->z.fixed) / toFIXED(1);

      if (distanceZ <= 0) {
        s32 horizontalDistance = distanceX;
        s32 distance;

        if (horizontalDistance < 0)
          horizontalDistance = -horizontalDistance;
        if (distanceZ < 0)
          distanceZ = -distanceZ;
        distance = horizontalDistance + distanceZ;
        if (distance < nearestDistance) {
          speakerId = actorId;
          nearestDistance = distance;
        }
      }
    }
  }
  // "The match is about to begin. Please take your places."
  Event_SetMessageId(MsgColossoMatchAboutBeginPleaseTake);
  Event_ShowMessageAndWait(speakerId, 0);
  transition = &work->transition;
  *transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  work->transitionFrames = 15;
  Event_Wait(20);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  flagBase = COLOSSO_CHEER_SLOT(supporterId);
  FieldGameFlag_SetByte(flagBase + COLOSSO_CHEER_COLUMN,
                        FIELD_CELL(supporter->x.fixed));
  {
    s32 cellRow = FIELD_CELL(supporter->z.fixed);

    FieldGameFlag_SetByte(flagBase + COLOSSO_CHEER_ROW, cellRow);
  }
  supporterId++;
  if (supporterId > MIA) {
    Import_EventSetExitRequest(COLOSSO_SOLO_TO_RACE);
    GameFlag_Set(FLAG_KEEP_MUSIC_ONCE);
  } else {
    Colosso_ControlCharacterAlone(supporterId);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    *transition = 0;
  }
  Event_End();
}

/// A stage's guide, talking to a friend who looks around alone, asks whether
/// they are done cheering.
void Colosso_AskDoneCheering(s32 id) {
  struct FieldWork *work;
  s32 friendId;
  struct Actor *actor;
  s32 message;
  s32 answer;

  work = gFieldWork;
  Actor_GetById(id);
  Actor_GetById(id);
  friendId = gMainRoutineWork.leaderActorId;
  actor = Actor_GetById(friendId);
  Event_Start();
  // "Huh? Are you done cheering?" Yeah: "Then please wait here until the
  // match begins." Nope: "Then cheer as best you can!"
  message = MsgColossoCheering;
  Event_SetMessageId(message);
  FieldMessage_ShowAtActor(id, 0);
  gFieldWork->choiceMessage = MsgColossoCheeringChoices;
  gFieldWork->choiceWidth = 4;
  answer = Event_AskYesNoWithActorAnimation(friendId, 0);
  if (answer == 0) {
    Event_SetMessageId(message + 1);
    Event_ShowMessageAndWait(id, 0);
    work->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    work->transitionFrames = 15;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    FieldGameFlag_SetByte(COLOSSO_CHEER_SLOT(friendId) + COLOSSO_CHEER_COLUMN,
                          FIELD_CELL(actor->x.fixed));
    FieldGameFlag_SetByte(COLOSSO_CHEER_SLOT(friendId) + COLOSSO_CHEER_ROW,
                          FIELD_CELL(actor->z.fixed));
    friendId++;
    if (friendId > MIA) {
      Import_EventSetExitRequest(COLOSSO_SOLO_TO_RACE);
      GameFlag_Set(FLAG_KEEP_MUSIC_ONCE);
    } else {
      Colosso_ControlCharacterAlone(friendId);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      work->transition = SCENE_TRANSITION(TRANSITION_FADE, 0);
    }
  } else {
    Event_SetMessageId(message + 2);
    Event_ShowMessageAndWait(id, 0);
  }
  Event_End();
}

/// The guide where the stages start. While Isaac looks around alone he asks
/// whether Isaac has seen enough and, on yes, walks him back out to the
/// Colosseum; while friends are still with Isaac, he tells him to choose where
/// they cheer from first.
static void Colosso_AskReadyToRace(s32 actorId) {
  struct Actor *actor;
  s32 x;
  s32 z;

  actor = Actor_GetById(actorId);
  x = actor->x.part[0].integer;
  z = actor->z.part[0].integer;
  Event_Start();

  if (Party_CountMembers() <= 1) {
    // "Isaac, did you get a good look around?" Yes: "Let's head back, then."
    // No: "Let me know when you're done looking around."
    Event_SetMessageId(MsgColossoIsaacDidGetGoodLook);
    if (Event_AskYesNoAndShowResponse(actorId, 0) == 0) {
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
      Actor_SetSpeed(actorId, 1);
      Actor_WalkToById(actorId, x, z + 64);
      Event_Wait(15);
      Actor_WalkToAndWaitById(ACTOR_PARTY_LEADER, x, z);
      Actor_WalkToAndWaitById(ACTOR_PARTY_LEADER, x, z + 32);
      Event_FinishScreenTransition();
      Event_WaitTransitionFrames();
      Import_EventSetExitRequest(COLOSSO_SOLO_TO_COLOSSEUM);
    }
  } else {
    // "Wait, shouldn't you decide where the best place to cheer for Isaac
    // is?..."
    Event_SetMessageId(MsgColossoWaitShouldntDecideWhereBest);
    Event_ShowMessageAndWait(actorId, 0);
  }

  Event_End();
}

/// Forgets where Garet, Ivan and Mia cheer from.
void Colosso_ClearCheeringSpots(void) {
  FieldGameFlag_SetByte(COLOSSO_CHEER_COLUMN + COLOSSO_CHEER_SLOT(GARET), 0);
  FieldGameFlag_SetByte(COLOSSO_CHEER_ROW + COLOSSO_CHEER_SLOT(GARET), 0);
  FieldGameFlag_SetByte(COLOSSO_CHEER_COLUMN + COLOSSO_CHEER_SLOT(IVAN), 0);
  FieldGameFlag_SetByte(COLOSSO_CHEER_ROW + COLOSSO_CHEER_SLOT(IVAN), 0);
  FieldGameFlag_SetByte(COLOSSO_CHEER_COLUMN + COLOSSO_CHEER_SLOT(MIA), 0);
  FieldGameFlag_SetByte(COLOSSO_CHEER_ROW + COLOSSO_CHEER_SLOT(MIA), 0);
}

/// A stage's guide names the stage and decides what to do next. "This is stage
/// N of the first finals match." (in Colosseum Final 1; the second in Final 2
/// and the third in Final 3). Returns what the guide does next, an enum
/// ColossoGuideReply; the yes or no answer to the description's question is
/// COLOSSO_GUIDE_DESCRIBE or COLOSSO_GUIDE_OFFER_CHEERING.
s32 Colosso_GreetAtStage(s32 speaker, s32 stage) {
  s32 sceneId;
  s32 message;
  s32 choice;

  Field_EndBusyAndReveal();
  MessageArgument_Push(stage, MESSAGE_ARGUMENT_NUMBER);
  sceneId = gMainRoutineWork.next;
  if (sceneId == SCENE_COLOSSEUM_AZART_MATCH) {
    message = MsgColossoStageFirstFinalsMatch;
  } else if (sceneId == SCENE_COLOSSEUM_SATRAGE_MATCH) {
    message = MsgColossoStageSecondFinalsMatch;
  } else {
    message = MsgColossoStageThirdFinalsMatch;
  }
  Event_SetMessageId(message);
  Event_ShowMessageAndWait(speaker, 0);
  if (GameFlag_Get(stage + COLOSSO_STAGE_CHEERED) != 0) {
    return COLOSSO_GUIDE_CHEER;
  }
  if (GameFlag_Get(stage + COLOSSO_STAGE_DESCRIBED) != 0) {
    // Descriptions, Cheer or Nothing.
    choice = ColossoMenu_Choose(0);
    if (choice == 1) {
      return COLOSSO_GUIDE_CHEER;
    }
    if (choice == 2 || choice == -1) {
      return COLOSSO_GUIDE_NOTHING;
    }
    return choice;
  }
  GameFlag_Set(stage + COLOSSO_STAGE_DESCRIBED);
  // "Would you like to hear a description of this stage?"
  Event_SetMessageId(MsgColossoWouldYouLikeHearDescription);
  FieldMessage_ShowAtActor(speaker, 0);
  return Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0);
}

/// A stage's guide, after describing the stage, names it again: the copy of
/// that line after it in the text.
void Colosso_RepeatStageNumber(s32 speaker, s32 stage) {
  s32 sceneId;
  s32 message;

  MessageArgument_Push(stage, MESSAGE_ARGUMENT_NUMBER);
  sceneId = gMainRoutineWork.next;
  if (sceneId == SCENE_COLOSSEUM_AZART_MATCH) {
    message = MsgColossoStageFirstFinalsMatch;
  } else if (sceneId == SCENE_COLOSSEUM_SATRAGE_MATCH) {
    message = MsgColossoStageSecondFinalsMatch;
  } else {
    message = MsgColossoStageThirdFinalsMatch;
  }
  Event_SetMessageId(message + 1);
  Event_ShowMessageAndWait(speaker, 0);
}

/// The guide at a stage, with the reply Colosso_GreetAtStage returned: offer a
/// friend's cheering at the stage.
void Colosso_OfferCheeringSpot(s32 mode, s32 guide, s32 stage) {
  struct Actor *actor;
  struct Actor *leader;
  s32 guideX;
  s32 guideZ;
  s32 memberCount;
  s32 answer;
  s32 friendId;
  s32 frontZ;
  s32 besideX;
  s32 behindZ;
  s32 column;
  s32 row;
  s32 i;
  u8 members[8];

  actor = Actor_GetById(guide);
  guideX = actor->x.part[0].integer;
  guideZ = actor->z.part[0].integer;
  if (mode == COLOSSO_GUIDE_NOTHING) {
    Event_SetMessageId(MsgColossoIfKnowWhoWantCheer);
    Event_ShowMessage(guide, 0);
    return;
  }
  memberCount = Party_CountMembers();
  for (i = 0; i < memberCount; i++) {
    members[i] = gMainRoutineWork.party[i];
  }
  if (memberCount <= 1) {
    Event_SetMessageId(MsgColossoDoYourBest);
    Event_ShowMessage(guide, 0);
    return;
  }
  if (GameFlag_Get(stage + COLOSSO_STAGE_CHEERED) != 0) {
    Event_SetMessageId(MsgColossoUnfortunatelyWeHaveFullHouse);
    Event_ShowMessage(guide, 0);
    return;
  }
  if (mode == COLOSSO_GUIDE_CHEER) {
    answer = 0;
    wait(6);
  } else {
    Event_SetMessageId(MsgColossoWouldLikeFriendCheerFor);
    Event_OpenMessage(guide, 0);
    answer = Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0);
  }
  if (answer != 0) {
    Event_SetMessageId(MsgColossoIfKnowWhoWantCheer);
    Event_ShowMessage(guide, 0);
    return;
  }
  if (answer < memberCount) {
    for (i = 0; i < memberCount; i++) {
      Party_RemoveMember((s8)members[i]);
    }
  }
  for (i = 0; i < memberCount; i++) {
    if ((s8)members[i] != 0) {
      Party_AddMember((s8)members[i]);
    }
  }
  friendId = PartyMemberChooser_Open();
  for (i = 0; i < memberCount; i++) {
    Party_RemoveMember((s8)members[i]);
  }
  for (i = 0; i < memberCount; i++) {
    Party_AddMember((s8)members[i]);
  }
  if (friendId == -1) {
    Event_SetMessageId(MsgColossoIfKnowWhoWantCheer);
    Event_ShowMessage(guide, 0);
    return;
  }
  MessageArgument_Push(friendId, MESSAGE_ARGUMENT_ACTOR);
  Event_SetMessageId(MsgColossoIsaacWillCheerForWay);
  Event_ShowMessage(guide, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_SetSpeed(friendId, 1);
  Actor_SetSpeed(guide, 1);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPositionFX(friendId, leader->x.fixed, leader->z.fixed);
  }
  frontZ = guideZ + 16;
  Actor_WalkToAndWait(friendId, guideX, frontZ);
  besideX = guideX + 16;
  Actor_WalkToAndWaitById(ACTOR_PARTY_LEADER, besideX, frontZ);
  Actor_TurnToFaceEachOther(friendId, ACTOR_PARTY_LEADER, 30);
  Actor_SetAnimation(friendId, ANIM_NOD);
  behindZ = frontZ - 32;
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_WalkToAndWait(guide, guideX, behindZ);
  Actor_WalkToById(guide, besideX, behindZ);
  Actor_WatchAndSetScript(ACTOR_PARTY_LEADER, friendId);
  Actor_WalkToAndWait(friendId, guideX, behindZ);
  Actor_SetAnimation(guide, ANIM_STAND);
  Actor_FaceDirection(guide, FACING_WEST, 0);
  Actor_WalkToAndWait(friendId, guideX, guideZ - 48);
  Actor_WalkToAndWait(guide, guideX, behindZ);
  Actor_WalkToAndWait(guide, guideX, guideZ);
  Party_RemoveMember(friendId);
  GameFlag_Set(stage + COLOSSO_STAGE_CHEERED);
  actor = Actor_GetById(friendId);
  column = FIELD_CELL(actor->x.fixed);
  FieldGameFlag_SetByte(COLOSSO_CHEER_SLOT(friendId) + COLOSSO_CHEER_COLUMN,
                        column);
  row = FIELD_CELL(actor->z.fixed);
  FieldGameFlag_SetByte(COLOSSO_CHEER_SLOT(friendId) + COLOSSO_CHEER_ROW, row);
}

#undef COLOSSO_CHEER_COLUMN
#undef COLOSSO_CHEER_ROW
#undef COLOSSO_CHEER_SLOT
#undef COLOSSO_STAGE_CHEERED
#undef COLOSSO_STAGE_DESCRIBED

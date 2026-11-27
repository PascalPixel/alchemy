/* @file */
/* The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3):
 */
/* before Isaac runs, Garet, Ivan and Mia each look around the stages on their
 */
/* own and choose where to cheer from, and the guide at each stage describes it
 */
/* and offers a friend's cheering there. */
/* */
/* Each match scene defines its own two exits before including this file: */
/* COLOSSO_SOLO_TO_RACE, its way from lending a hand to the race, and */
/* COLOSSO_SOLO_TO_COLOSSEUM, its way back out to the Colosseum. */

/* Byte flags (eight flags each) holding where a friend cheers from: the cell */
/* column at COLOSSO_CHEER_COLUMN + 16 * character and the cell row at */
/* COLOSSO_CHEER_ROW + 16 * character, for Garet, Ivan and Mia. */
#define COLOSSO_CHEER_COLUMN		  FLAG_AREA_TEMP_70
#define COLOSSO_CHEER_ROW			  FLAG_AREA_TEMP_78
#define COLOSSO_CHEER_SLOT(character) ((character) << 4)

/* Map temp flags, by stage number: a friend cheers at the stage, and its */
/* description was heard. When a friend cheers there, the guide says */
/* "Unfortunately, we have a full house." */
#define COLOSSO_STAGE_CHEERED	FLAG_SCENE_TEMP_00
#define COLOSSO_STAGE_DESCRIBED FLAG_SCENE_TEMP_08

/* Puts Garet, Ivan and Mia back at the cells where they chose to cheer, at the
 */
/* centre of each cell. */
void scColossoPlaceFriendsAtCheeringSpots(void) {
	{
		s32 x = syFieldGameFlagGetByte(COLOSSO_CHEER_COLUMN +
									   COLOSSO_CHEER_SLOT(GARET));
		s32 y = syFieldGameFlagGetByte(COLOSSO_CHEER_ROW +
									   COLOSSO_CHEER_SLOT(GARET));

		fdActorSetPosFX(ACTOR_GARET, FIELD_CELL_CENTER(x),
						FIELD_CELL_CENTER(y));
	}
	{
		s32 x = syFieldGameFlagGetByte(COLOSSO_CHEER_COLUMN +
									   COLOSSO_CHEER_SLOT(IVAN));
		s32 y = syFieldGameFlagGetByte(COLOSSO_CHEER_ROW +
									   COLOSSO_CHEER_SLOT(IVAN));

		fdActorSetPosFX(ACTOR_IVAN, FIELD_CELL_CENTER(x), FIELD_CELL_CENTER(y));
	}
	{
		s32 x = syFieldGameFlagGetByte(COLOSSO_CHEER_COLUMN +
									   COLOSSO_CHEER_SLOT(MIA));
		s32 y =
			syFieldGameFlagGetByte(COLOSSO_CHEER_ROW + COLOSSO_CHEER_SLOT(MIA));

		fdActorSetPosFX(ACTOR_MIA, FIELD_CELL_CENTER(x), FIELD_CELL_CENTER(y));
	}
}

/* Makes the character the only party member, the one the player moves and the
 */
/* camera follows, with full HP and PP and no poison. */
void scColossoCtrlCharacterAlone(s32 id) {
	UNIT *unit;

	gmPartyRemoveMember(ISAAC);
	gmPartyRemoveMember(GARET);
	gmPartyRemoveMember(IVAN);
	gmPartyRemoveMember(MIA);
	gmPartyRemoveMember(JENNA);
	gmPartyAddMember(id);
	MainRoutineStateWork.leaderActorId = id;
	fdCameraFollowActor(id, 0);
	unit = gmUnitGet(id);
	unit->hp = unit->hpMax;
	unit->pp = unit->ppMax;
	unit->poison = 0;
	gmUnitUpdateGauges(unit);
	fdPartyClearUnusableShortcuts();
}

/* The trigger each match scene raises for scColossoTakePlaceForMatch. */
#define COLOSSO_TAKE_PLACE_TRIGGER 99

/* A task while a friend looks around the stages alone (the visit to */
/* lend a hand): when the Psynergy request names that friend and */
/* FLAG_PSYNERGY_TARGET is set, raise COLOSSO_TAKE_PLACE_TRIGGER. */
void scColossoRaiseTakePlaceOnPsynergy(void) {
	FIELD_STATE *state = FieldStateWork;
	s32 selection = MainRoutineStateWork.leaderActorId;

	if(selection != 0 && (state->psynergy >> ACT_UNIT_SHIFT) == selection &&
	   gmFlagGet(FLAG_PSYNERGY_TARGET) != 0) {
		u16 *raised = (u16 *)&state->raisedTrigger;
		s32 trigger = COLOSSO_TAKE_PLACE_TRIGGER;

		*raised = trigger;
	}
}

/* COLOSSO_TAKE_PLACE_TRIGGER: the nearest fair-haired young man level with or
 */
/* south of the friend calls the warriors to their places, and the */
/* friend's cell becomes their place to cheer from. "The match is about to */
/* begin. Please take your places." The next friend then looks around, or, */
/* after Mia, the party leaves for the race. */
static void scColossoTakePlaceForMatch(void) {
	FIELD_STATE *state = FieldStateWork;
	s32 speakerId = ACTOR_FIRST_PLACED;
	FIXED nearestDistance = toFIXED(16);
	s32 supporterId = MainRoutineStateWork.leaderActorId;
	ACTOR *supporter = fdActorGetById(supporterId);
	s32 actorId;
	s32 *transition;
	s32 flagBase;

	fdEventStart();
	/* BUG: one past the actor table: fdActorLookup(ACTOR_ID_COUNT) is the first
	 */
	/* treasure spot's actor, which is looked at too; it is never the speaker.
	 */
#ifdef BUGFIX
	for(actorId = ACTOR_FIRST_PLACED; actorId < ACTOR_ID_COUNT; actorId++) {
#else
	for(actorId = ACTOR_FIRST_PLACED; actorId <= ACTOR_ID_COUNT; actorId++) {
#endif
		ACTOR *actor = fdActorGetById(actorId);

		if(actor != NULL && actor->sprStorageKind == ACTOR_SPRITE_ONE &&
		   actor->spr->entries[0]->anmId == SPRITE_YOUNG_MAN_WITH_FAIR_HAIR) {
			s32 distanceX = (supporter->x.fixed - actor->x.fixed) / toFIXED(1);
			s32 distanceZ = (supporter->z.fixed - actor->z.fixed) / toFIXED(1);

			if(distanceZ <= 0) {
				s32 horizontalDistance = distanceX;
				s32 distance;

				if(horizontalDistance < 0)
					horizontalDistance = -horizontalDistance;
				if(distanceZ < 0)
					distanceZ = -distanceZ;
				distance = horizontalDistance + distanceZ;
				if(distance < nearestDistance) {
					speakerId = actorId;
					nearestDistance = distance;
				}
			}
		}
	}
	/* "The match is about to begin. Please take your places." */
	fdEventSetMessageId(MsgColossoMatchAboutBeginPleaseTake);
	fdEventShowMessageAndWait(speakerId, 0);
	transition = &state->transition;
	*transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
	state->transitionFrm = 15;
	fdEventWait(20);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	flagBase = COLOSSO_CHEER_SLOT(supporterId);
	syFieldGameFlagSetByte(flagBase + COLOSSO_CHEER_COLUMN,
						   FIELD_CELL(supporter->x.fixed));
	{
		s32 cellRow = FIELD_CELL(supporter->z.fixed);

		syFieldGameFlagSetByte(flagBase + COLOSSO_CHEER_ROW, cellRow);
	}
	supporterId++;
	if(supporterId > MIA) {
		syImportEventSetExitRequest(COLOSSO_SOLO_TO_RACE);
		gmFlagSet(FLAG_KEEP_MUSIC_ONCE);
	}
	else {
		scColossoCtrlCharacterAlone(supporterId);
		fdEventStartScrTransition();
		fdEventWaitTransitionFrames();
		*transition = 0;
	}
	fdEventEnd();
}

/* A stage's guide, talking to a friend who looks around alone, asks whether */
/* they are done cheering. */
void scColossoAskDoneCheering(s32 id) {
	FIELD_STATE *state;
	s32 friendId;
	ACTOR *actor;
	s32 message;
	s32 answer;

	state = FieldStateWork;
	fdActorGetById(id);
	fdActorGetById(id);
	friendId = MainRoutineStateWork.leaderActorId;
	actor = fdActorGetById(friendId);
	fdEventStart();
	/* "Huh? Are you done cheering?" Yeah: "Then please wait here until the */
	/* match begins." Nope: "Then cheer as best you can!" */
	message = MsgColossoCheering;
	fdEventSetMessageId(message);
	fdMessageShowAtActor(id, 0);
	FieldStateWork->choiceMsg = MsgColossoCheeringChoices;
	FieldStateWork->choiceWidth = 4;
	answer = fdEventAskYesNoWithActorAnimation(friendId, 0);
	if(answer == 0) {
		fdEventSetMessageId(message + 1);
		fdEventShowMessageAndWait(id, 0);
		state->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
		state->transitionFrm = 15;
		fdEventFinishScrTransition();
		fdEventWaitTransitionFrames();
		syFieldGameFlagSetByte(COLOSSO_CHEER_SLOT(friendId) +
								   COLOSSO_CHEER_COLUMN,
							   FIELD_CELL(actor->x.fixed));
		syFieldGameFlagSetByte(COLOSSO_CHEER_SLOT(friendId) + COLOSSO_CHEER_ROW,
							   FIELD_CELL(actor->z.fixed));
		friendId++;
		if(friendId > MIA) {
			syImportEventSetExitRequest(COLOSSO_SOLO_TO_RACE);
			gmFlagSet(FLAG_KEEP_MUSIC_ONCE);
		}
		else {
			scColossoCtrlCharacterAlone(friendId);
			fdEventStartScrTransition();
			fdEventWaitTransitionFrames();
			state->transition = SCENE_TRANSITION(TRANSITION_FADE, 0);
		}
	}
	else {
		fdEventSetMessageId(message + 2);
		fdEventShowMessageAndWait(id, 0);
	}
	fdEventEnd();
}

/* The guide where the stages start. While Isaac looks around alone he asks */
/* whether Isaac has seen enough and, on yes, walks him back out to the */
/* Colosseum; while friends are still with Isaac, he tells him to choose where
 */
/* they cheer from first. */
static void scColossoAskDoneLookingAround(s32 actorId) {
	ACTOR *actor;
	s32 x;
	s32 z;

	actor = fdActorGetById(actorId);
	x = actor->x.part[0].integer;
	z = actor->z.part[0].integer;
	fdEventStart();

	if(gmPartyCountMembers() <= 1) {
		/* "Isaac, did you get a good look around?" Yes: "Let's head back,
		 * then." */
		/* No: "Let me know when you're done looking around." */
		fdEventSetMessageId(MsgColossoIsaacDidGetGoodLook);
		if(fdEventAskYesNoAndShowResponse(actorId, 0) == 0) {
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
			fdActorSetSpeed(actorId, 1);
			fdActorWalkToById(actorId, x, z + 64);
			fdEventWait(15);
			fdActorWalkToAndWaitById(ACTOR_PARTY_LEADER, x, z);
			fdActorWalkToAndWaitById(ACTOR_PARTY_LEADER, x, z + 32);
			fdEventFinishScrTransition();
			fdEventWaitTransitionFrames();
			syImportEventSetExitRequest(COLOSSO_SOLO_TO_COLOSSEUM);
		}
	}
	else {
		/* "Wait, shouldn't you decide where the best place to cheer for Isaac
		 */
		/* is?..." */
		fdEventSetMessageId(MsgColossoWaitShouldntDecideWhereBest);
		fdEventShowMessageAndWait(actorId, 0);
	}

	fdEventEnd();
}

/* Forgets where Garet, Ivan and Mia cheer from. */
void scColossoClearCheeringSpots(void) {
	syFieldGameFlagSetByte(COLOSSO_CHEER_COLUMN + COLOSSO_CHEER_SLOT(GARET), 0);
	syFieldGameFlagSetByte(COLOSSO_CHEER_ROW + COLOSSO_CHEER_SLOT(GARET), 0);
	syFieldGameFlagSetByte(COLOSSO_CHEER_COLUMN + COLOSSO_CHEER_SLOT(IVAN), 0);
	syFieldGameFlagSetByte(COLOSSO_CHEER_ROW + COLOSSO_CHEER_SLOT(IVAN), 0);
	syFieldGameFlagSetByte(COLOSSO_CHEER_COLUMN + COLOSSO_CHEER_SLOT(MIA), 0);
	syFieldGameFlagSetByte(COLOSSO_CHEER_ROW + COLOSSO_CHEER_SLOT(MIA), 0);
}

/* A stage's guide names the stage and decides what to do next. "This is stage
 */
/* N of the first finals match." (in Colosseum Final 1; the second in Final 2 */
/* and the third in Final 3). Returns what the guide does next, an enum */
/* COLOSSO_GUIDE_REPLY; the yes or no answer to the description's question is */
/* COLOSSO_GUIDE_DESCRIBE or COLOSSO_GUIDE_OFFER_CHEERING. */
s32 scColossoGreetAtStage(s32 speaker, s32 stage) {
	s32 sceneId;
	s32 message;
	s32 choice;

	fdEndBusyAndReveal();
	mnMessageArgumentPush(stage, MESSAGE_ARGUMENT_NUMBER);
	sceneId = MainRoutineStateWork.next;
	if(sceneId == SCENE_COLOSSEUM_AZART_MATCH) {
		message = MsgColossoStageFirstFinalsMatch;
	}
	else if(sceneId == SCENE_COLOSSEUM_SATRAGE_MATCH) {
		message = MsgColossoStageSecondFinalsMatch;
	}
	else {
		message = MsgColossoStageThirdFinalsMatch;
	}
	fdEventSetMessageId(message);
	fdEventShowMessageAndWait(speaker, 0);
	if(gmFlagGet(stage + COLOSSO_STAGE_CHEERED) != 0) {
		return COLOSSO_GUIDE_CHEER;
	}
	if(gmFlagGet(stage + COLOSSO_STAGE_DESCRIBED) != 0) {
		/* Descriptions, Cheer or Nothing. */
		choice = mnColossoMenuChoose(0);
		if(choice == 1) {
			return COLOSSO_GUIDE_CHEER;
		}
		if(choice == 2 || choice == -1) {
			return COLOSSO_GUIDE_NOTHING;
		}
		return choice;
	}
	gmFlagSet(stage + COLOSSO_STAGE_DESCRIBED);
	/* "Would you like to hear a description of this stage?" */
	fdEventSetMessageId(MsgColossoWouldYouLikeHearDescription);
	fdMessageShowAtActor(speaker, 0);
	return fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0);
}

/* A stage's guide, after describing the stage, names it again: the copy of */
/* that line after it in the text. */
void scColossoRepeatStageNum(s32 speaker, s32 stage) {
	s32 sceneId;
	s32 message;

	mnMessageArgumentPush(stage, MESSAGE_ARGUMENT_NUMBER);
	sceneId = MainRoutineStateWork.next;
	if(sceneId == SCENE_COLOSSEUM_AZART_MATCH) {
		message = MsgColossoStageFirstFinalsMatch;
	}
	else if(sceneId == SCENE_COLOSSEUM_SATRAGE_MATCH) {
		message = MsgColossoStageSecondFinalsMatch;
	}
	else {
		message = MsgColossoStageThirdFinalsMatch;
	}
	fdEventSetMessageId(message + 1);
	fdEventShowMessageAndWait(speaker, 0);
}

/* The guide at a stage, with the reply scColossoGreetAtStage returned: offer a
 */
/* friend's cheering at the stage. */
void scColossoOfferCheeringSpot(s32 mode, s32 guide, s32 stage) {
	ACTOR *actor;
	ACTOR *leader;
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

	actor = fdActorGetById(guide);
	guideX = actor->x.part[0].integer;
	guideZ = actor->z.part[0].integer;
	if(mode == COLOSSO_GUIDE_NOTHING) {
		fdEventSetMessageId(MsgColossoIfKnowWhoWantCheer);
		fdEventShowMessage(guide, 0);
		return;
	}
	memberCount = gmPartyCountMembers();
	for(i = 0; i < memberCount; i++) {
		members[i] = MainRoutineStateWork.party[i];
	}
	if(memberCount <= 1) {
		fdEventSetMessageId(MsgColossoDoYourBest);
		fdEventShowMessage(guide, 0);
		return;
	}
	if(gmFlagGet(stage + COLOSSO_STAGE_CHEERED) != 0) {
		fdEventSetMessageId(MsgColossoUnfortunatelyWeHaveFullHouse);
		fdEventShowMessage(guide, 0);
		return;
	}
	if(mode == COLOSSO_GUIDE_CHEER) {
		answer = 0;
		sySynch(6);
	}
	else {
		fdEventSetMessageId(MsgColossoWouldLikeFriendCheerFor);
		fdEventOpenMessage(guide, 0);
		answer = fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0);
	}
	if(answer != 0) {
		fdEventSetMessageId(MsgColossoIfKnowWhoWantCheer);
		fdEventShowMessage(guide, 0);
		return;
	}
	if(answer < memberCount) {
		for(i = 0; i < memberCount; i++) {
			gmPartyRemoveMember((s8)members[i]);
		}
	}
	for(i = 0; i < memberCount; i++) {
		if((s8)members[i] != 0) {
			gmPartyAddMember((s8)members[i]);
		}
	}
	friendId = mnPartyMemberChooserOpen();
	for(i = 0; i < memberCount; i++) {
		gmPartyRemoveMember((s8)members[i]);
	}
	for(i = 0; i < memberCount; i++) {
		gmPartyAddMember((s8)members[i]);
	}
	if(friendId == -1) {
		fdEventSetMessageId(MsgColossoIfKnowWhoWantCheer);
		fdEventShowMessage(guide, 0);
		return;
	}
	mnMessageArgumentPush(friendId, MESSAGE_ARGUMENT_ACTOR);
	fdEventSetMessageId(MsgColossoIsaacWillCheerForWay);
	fdEventShowMessage(guide, 0);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorSetSpeed(friendId, 1);
	fdActorSetSpeed(guide, 1);
	leader = fdActorGetById(ACTOR_PARTY_LEADER);
	if(leader != NULL) {
		fdActorSetPosFX(friendId, leader->x.fixed, leader->z.fixed);
	}
	frontZ = guideZ + 16;
	fdActorWalkToAndWait(friendId, guideX, frontZ);
	besideX = guideX + 16;
	fdActorWalkToAndWaitById(ACTOR_PARTY_LEADER, besideX, frontZ);
	fdActorTurnToFaceEachOther(friendId, ACTOR_PARTY_LEADER, 30);
	fdActorSetAnimation(friendId, ANIM_NOD);
	behindZ = frontZ - 32;
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorWalkToAndWait(guide, guideX, behindZ);
	fdActorWalkToById(guide, besideX, behindZ);
	fdActorWatchAndSetScript(ACTOR_PARTY_LEADER, friendId);
	fdActorWalkToAndWait(friendId, guideX, behindZ);
	fdActorSetAnimation(guide, ANIM_STAND);
	fdActorFaceDirection(guide, FACING_WEST, 0);
	fdActorWalkToAndWait(friendId, guideX, guideZ - 48);
	fdActorWalkToAndWait(guide, guideX, behindZ);
	fdActorWalkToAndWait(guide, guideX, guideZ);
	gmPartyRemoveMember(friendId);
	gmFlagSet(stage + COLOSSO_STAGE_CHEERED);
	actor = fdActorGetById(friendId);
	column = FIELD_CELL(actor->x.fixed);
	syFieldGameFlagSetByte(COLOSSO_CHEER_SLOT(friendId) + COLOSSO_CHEER_COLUMN,
						   column);
	row = FIELD_CELL(actor->z.fixed);
	syFieldGameFlagSetByte(COLOSSO_CHEER_SLOT(friendId) + COLOSSO_CHEER_ROW,
						   row);
}

#undef COLOSSO_CHEER_COLUMN
#undef COLOSSO_CHEER_ROW
#undef COLOSSO_CHEER_SLOT
#undef COLOSSO_STAGE_CHEERED
#undef COLOSSO_STAGE_DESCRIBED

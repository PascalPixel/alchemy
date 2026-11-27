/* The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3):
   before Isaac runs, Garet, Ivan and Mia each look around the stages on their
   own and choose where to cheer from, and the guide at each stage describes it
   and offers a friend's cheering there.

   Each match scene defines its own two exits before including this file:
   COLOSSO_SOLO_TO_RACE, its way from lending a hand to the race, and
   COLOSSO_SOLO_TO_COLOSSEUM, its way back out to the Colosseum. */

/* Byte flags (eight flags each) holding where a friend cheers from: the
   metatile column at COLOSSO_CHEER_COLUMN + 16 * character and the metatile row
   at COLOSSO_CHEER_ROW + 16 * character, for Garet, Ivan and Mia. */
#define COLOSSO_CHEER_COLUMN		  FLAG_AREA_TEMP_70
#define COLOSSO_CHEER_ROW			  FLAG_AREA_TEMP_78
#define COLOSSO_CHEER_SLOT(character) ((character) << 4)

/* Map temp flags, by stage number: a friend cheers at the stage, and its
   description was heard. When a friend cheers there, the guide says
   "Unfortunately, we have a full house." */
#define COLOSSO_STAGE_CHEERED	FLAG_SCENE_TEMP_00
#define COLOSSO_STAGE_DESCRIBED FLAG_SCENE_TEMP_08

/* Puts Garet, Ivan and Mia back at the metatiles where they chose to cheer, at
   the centre of each metatile. */
void scColossoPlaceFriendsAtCheeringSpots(void)
{
	{
		Sint32 x = syFieldGameFlagGetByte(
			COLOSSO_CHEER_COLUMN + COLOSSO_CHEER_SLOT(GARET));
		Sint32 y = syFieldGameFlagGetByte(
			COLOSSO_CHEER_ROW + COLOSSO_CHEER_SLOT(GARET));

		fdActorSetPosFX(
			ACTOR_GARET, FIELD_METATILE_CENTER(x), FIELD_METATILE_CENTER(y));
	}
	{
		Sint32 x = syFieldGameFlagGetByte(
			COLOSSO_CHEER_COLUMN + COLOSSO_CHEER_SLOT(IVAN));
		Sint32 y = syFieldGameFlagGetByte(
			COLOSSO_CHEER_ROW + COLOSSO_CHEER_SLOT(IVAN));

		fdActorSetPosFX(
			ACTOR_IVAN, FIELD_METATILE_CENTER(x), FIELD_METATILE_CENTER(y));
	}
	{
		Sint32 x = syFieldGameFlagGetByte(
			COLOSSO_CHEER_COLUMN + COLOSSO_CHEER_SLOT(MIA));
		Sint32 y =
			syFieldGameFlagGetByte(COLOSSO_CHEER_ROW + COLOSSO_CHEER_SLOT(MIA));

		fdActorSetPosFX(
			ACTOR_MIA, FIELD_METATILE_CENTER(x), FIELD_METATILE_CENTER(y));
	}
}

/* Makes the character the only party member, the one the player moves and the
   camera follows, with full HP and PP and no poison. */
void scColossoCtrlCharacterAlone(Sint32 id)
{
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
	UNIT_UPDATE_GAUGES(unit);
	fdPartyClearUnusableShortcuts();
}

/* The cue each match scene raises for scColossoTakePlaceForMatch. */
#define COLOSSO_TAKE_PLACE_CUE 99

/* An event while a friend looks around the stages alone (the visit to lend a
   hand): when the Psynergy request names that friend and FLAG_PSYNERGY_TARGET
   is set, raise COLOSSO_TAKE_PLACE_CUE. */
void scColossoRaiseTakePlaceOnPsynergy(void)
{
	FIELD_STATE *state = FieldStateWork;
	Sint32		 selection = MainRoutineStateWork.leaderActorId;

	if(selection != 0 && (state->psynergy >> ACT_UNIT_SHIFT) == selection &&
		gmFlagGet(FLAG_PSYNERGY_TARGET) != 0) {
		Uint16 *raised = (Uint16 *)&state->raisedCue;
		Sint32	cue = COLOSSO_TAKE_PLACE_CUE;

		*raised = cue;
	}
}

/* COLOSSO_TAKE_PLACE_CUE: the nearest fair-haired young man level with or south
   of the friend calls the warriors to their places, and the friend's metatile
   becomes their place to cheer from. "The match is about to begin. Please take
   your places." The next friend then looks around, or, after Mia, the party
   leaves for the race. */
static void scColossoTakePlaceForMatch(void)
{
	FIELD_STATE *state = FieldStateWork;
	Sint32		 speakerId = ACTOR_FIRST_PLACED;
	FIXED		 nearestDistance = toFIXED(16);
	Sint32		 supporterId = MainRoutineStateWork.leaderActorId;
	ACTOR		*supporter = fdActorGetById(supporterId);
	Sint32		 actorId;
	Sint32		*transition;
	Sint32		 flgBase;

	fdScriptStart();
	/* BUG: one past the actor table: fdActorLookup(ACTOR_ID_COUNT) is the first
	   treasure spot's actor, which is looked at too; it is never the
	   speaker. */
#if FIX_FIRST_TREASURE_SPOT_IN_ACTOR_LOOPS
	for(actorId = ACTOR_FIRST_PLACED; actorId < ACTOR_ID_COUNT; actorId++) {
#else
	for(actorId = ACTOR_FIRST_PLACED; actorId <= ACTOR_ID_COUNT; actorId++) {
#endif
		ACTOR *actor = fdActorGetById(actorId);

		if(actor != NULL && actor->sprStorageKind == ACTOR_SPRITE_ONE &&
			actor->spr->entries[0]->anmId == SPRITE_YOUNG_MAN_WITH_FAIR_HAIR) {
			Sint32 distanceX =
				(supporter->x.fixed - actor->x.fixed) / toFIXED(1);
			Sint32 distanceZ =
				(supporter->z.fixed - actor->z.fixed) / toFIXED(1);

			if(distanceZ <= 0) {
				Sint32 horizontalDistance = distanceX;
				Sint32 distance;

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
	fdScriptSetMessageId(MsgColossoMatchAboutBeginPleaseTake);
	fdScriptShowMessageAndWait(speakerId, 0);
	transition = &state->transition;
	*transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
	state->transitionFrm = 15;
	fdScriptWait(20);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	flgBase = COLOSSO_CHEER_SLOT(supporterId);
	syFieldGameFlagSetByte(
		flgBase + COLOSSO_CHEER_COLUMN, FIELD_METATILE(supporter->x.fixed));
	{
		Sint32 metatileRow = FIELD_METATILE(supporter->z.fixed);

		syFieldGameFlagSetByte(flgBase + COLOSSO_CHEER_ROW, metatileRow);
	}
	supporterId++;
	if(supporterId > MIA) {
		syImportScriptSetExitRequest(COLOSSO_SOLO_TO_RACE);
		gmFlagSet(FLAG_KEEP_MUSIC_ONCE);
	} else {
		scColossoCtrlCharacterAlone(supporterId);
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionTicks();
		*transition = 0;
	}
	fdScriptEnd();
}

/* A stage's guide, talking to a friend who looks around alone, asks whether
   they are done cheering. */
void scColossoAskDoneCheering(Sint32 id)
{
	FIELD_STATE *state;
	Sint32		 friendId;
	ACTOR		*actor;
	Sint32		 msg;
	Sint32		 answer;

	state = FieldStateWork;
	fdActorGetById(id);
	fdActorGetById(id);
	friendId = MainRoutineStateWork.leaderActorId;
	actor = fdActorGetById(friendId);
	fdScriptStart();
	/* "Huh? Are you done cheering?" Yeah: "Then please wait here until the
	   match begins." Nope: "Then cheer as best you can!" */
	msg = MsgColossoCheering;
	fdScriptSetMessageId(msg);
	fdMessageShowAtActor(id, 0);
	FieldStateWork->choiceMsg = MsgColossoCheeringChoices;
	FieldStateWork->choiceWidth = 4;
	answer = fdScriptAskYesNoWithActorAnimation(friendId, 0);
	if(answer == 0) {
		fdScriptSetMessageId(msg + 1);
		fdScriptShowMessageAndWait(id, 0);
		state->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
		state->transitionFrm = 15;
		fdScriptFinishScreenTransition();
		fdScriptWaitTransitionTicks();
		syFieldGameFlagSetByte(
			COLOSSO_CHEER_SLOT(friendId) + COLOSSO_CHEER_COLUMN,
			FIELD_METATILE(actor->x.fixed));
		syFieldGameFlagSetByte(COLOSSO_CHEER_SLOT(friendId) + COLOSSO_CHEER_ROW,
			FIELD_METATILE(actor->z.fixed));
		friendId++;
		if(friendId > MIA) {
			syImportScriptSetExitRequest(COLOSSO_SOLO_TO_RACE);
			gmFlagSet(FLAG_KEEP_MUSIC_ONCE);
		} else {
			scColossoCtrlCharacterAlone(friendId);
			fdScriptStartScreenTransition();
			fdScriptWaitTransitionTicks();
			state->transition = SCENE_TRANSITION(TRANSITION_FADE, 0);
		}
	} else {
		fdScriptSetMessageId(msg + 2);
		fdScriptShowMessageAndWait(id, 0);
	}
	fdScriptEnd();
}

/* The guide where the stages start. While Isaac looks around alone he asks
   whether Isaac has seen enough and, on yes, walks him back out to the
   Colosseum; while friends are still with Isaac, he tells him to choose where
   they cheer from first. */
static void scColossoAskDoneLookingAround(Sint32 actorId)
{
	ACTOR *actor;
	Sint32 x;
	Sint32 z;

	actor = fdActorGetById(actorId);
	x = actor->x.part[0].integer;
	z = actor->z.part[0].integer;
	fdScriptStart();

	if(gmPartyCountMembers() <= 1) {
		/* "Isaac, did you get a good look around?" Yes: "Let's head back,
		   then." No: "Let me know when you're done looking around." */
		fdScriptSetMessageId(MsgColossoIsaacDidGetGoodLook);
		if(fdScriptAskYesNoAndShowResponse(actorId, 0) == 0) {
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
			fdActorSetSpeed(actorId, 1);
			fdActorWalkToById(actorId, x, z + 64);
			fdScriptWait(15);
			fdActorWalkToAndWaitById(ACTOR_PARTY_LEADER, x, z);
			fdActorWalkToAndWaitById(ACTOR_PARTY_LEADER, x, z + 32);
			fdScriptFinishScreenTransition();
			fdScriptWaitTransitionTicks();
			syImportScriptSetExitRequest(COLOSSO_SOLO_TO_COLOSSEUM);
		}
	} else {
		/* "Wait, shouldn't you decide where the best place to cheer for Isaac
		   is?..." */
		fdScriptSetMessageId(MsgColossoWaitShouldntDecideWhereBest);
		fdScriptShowMessageAndWait(actorId, 0);
	}

	fdScriptEnd();
}

/* Forgets where Garet, Ivan and Mia cheer from. */
void scColossoClearCheeringSpots(void)
{
	syFieldGameFlagSetByte(COLOSSO_CHEER_COLUMN + COLOSSO_CHEER_SLOT(GARET), 0);
	syFieldGameFlagSetByte(COLOSSO_CHEER_ROW + COLOSSO_CHEER_SLOT(GARET), 0);
	syFieldGameFlagSetByte(COLOSSO_CHEER_COLUMN + COLOSSO_CHEER_SLOT(IVAN), 0);
	syFieldGameFlagSetByte(COLOSSO_CHEER_ROW + COLOSSO_CHEER_SLOT(IVAN), 0);
	syFieldGameFlagSetByte(COLOSSO_CHEER_COLUMN + COLOSSO_CHEER_SLOT(MIA), 0);
	syFieldGameFlagSetByte(COLOSSO_CHEER_ROW + COLOSSO_CHEER_SLOT(MIA), 0);
}

/* A stage's guide names the stage and decides what to do next. "This is stage N
   of the first finals match." (in Colosseum Final 1; the second in Final 2 and
   the third in Final 3). Returns what the guide does next, an enum
   COLOSSO_GUIDE_REPLY; the yes or no answer to the description's question is
   COLOSSO_GUIDE_DESCRIBE or COLOSSO_GUIDE_OFFER_CHEERING. */
Sint32 scColossoGreetAtStage(Sint32 speaker, Sint32 stage)
{
	Sint32 sceneId;
	Sint32 msg;
	Sint32 choice;

	fdEndBusyAndReveal();
	mnMessageArgumentPush(stage, MESSAGE_ARGUMENT_NUMBER);
	sceneId = MainRoutineStateWork.next;
	if(sceneId == SCENE_COLOSSEUM_AZART_MATCH) {
		msg = MsgColossoStageFirstFinalsMatch;
	} else if(sceneId == SCENE_COLOSSEUM_SATRAGE_MATCH) {
		msg = MsgColossoStageSecondFinalsMatch;
	} else {
		msg = MsgColossoStageThirdFinalsMatch;
	}
	fdScriptSetMessageId(msg);
	fdScriptShowMessageAndWait(speaker, 0);
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
	fdScriptSetMessageId(MsgColossoWouldYouLikeHearDescription);
	fdMessageShowAtActor(speaker, 0);
	return fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0);
}

/* A stage's guide, after describing the stage, names it again: the copy of that
   line after it in the text. */
void scColossoRepeatStageNum(Sint32 speaker, Sint32 stage)
{
	Sint32 sceneId;
	Sint32 msg;

	mnMessageArgumentPush(stage, MESSAGE_ARGUMENT_NUMBER);
	sceneId = MainRoutineStateWork.next;
	if(sceneId == SCENE_COLOSSEUM_AZART_MATCH) {
		msg = MsgColossoStageFirstFinalsMatch;
	} else if(sceneId == SCENE_COLOSSEUM_SATRAGE_MATCH) {
		msg = MsgColossoStageSecondFinalsMatch;
	} else {
		msg = MsgColossoStageThirdFinalsMatch;
	}
	fdScriptSetMessageId(msg + 1);
	fdScriptShowMessageAndWait(speaker, 0);
}

/* The guide at a stage, with the reply scColossoGreetAtStage returned: offer a
   friend's cheering at the stage. */
void scColossoOfferCheeringSpot(Sint32 mode, Sint32 guide, Sint32 stage)
{
	ACTOR *actor;
	ACTOR *leader;
	Sint32 guideX;
	Sint32 guideZ;
	Sint32 memberCnt;
	Sint32 answer;
	Sint32 friendId;
	Sint32 frontZ;
	Sint32 besideX;
	Sint32 behindZ;
	Sint32 column;
	Sint32 row;
	Sint32 i;
	Uint8  members[8];

	actor = fdActorGetById(guide);
	guideX = actor->x.part[0].integer;
	guideZ = actor->z.part[0].integer;
	if(mode == COLOSSO_GUIDE_NOTHING) {
		fdScriptSetMessageId(MsgColossoIfKnowWhoWantCheer);
		fdScriptShowMessage(guide, 0);
		return;
	}
	memberCnt = gmPartyCountMembers();
	for(i = 0; i < memberCnt; i++) {
		members[i] = MainRoutineStateWork.party[i];
	}
	if(memberCnt <= 1) {
		fdScriptSetMessageId(MsgColossoDoYourBest);
		fdScriptShowMessage(guide, 0);
		return;
	}
	if(gmFlagGet(stage + COLOSSO_STAGE_CHEERED) != 0) {
		fdScriptSetMessageId(MsgColossoUnfortunatelyWeHaveFullHouse);
		fdScriptShowMessage(guide, 0);
		return;
	}
	if(mode == COLOSSO_GUIDE_CHEER) {
		answer = 0;
		sySynch(6);
	} else {
		fdScriptSetMessageId(MsgColossoWouldLikeFriendCheerFor);
		fdScriptOpenMessage(guide, 0);
		answer = fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0);
	}
	if(answer != 0) {
		fdScriptSetMessageId(MsgColossoIfKnowWhoWantCheer);
		fdScriptShowMessage(guide, 0);
		return;
	}
	if(answer < memberCnt) {
		for(i = 0; i < memberCnt; i++) {
			gmPartyRemoveMember((Sint8)members[i]);
		}
	}
	for(i = 0; i < memberCnt; i++) {
		if((Sint8)members[i] != 0) {
			gmPartyAddMember((Sint8)members[i]);
		}
	}
	friendId = mnPartyMemberChooserOpen();
	for(i = 0; i < memberCnt; i++) {
		gmPartyRemoveMember((Sint8)members[i]);
	}
	for(i = 0; i < memberCnt; i++) {
		gmPartyAddMember((Sint8)members[i]);
	}
	if(friendId == -1) {
		fdScriptSetMessageId(MsgColossoIfKnowWhoWantCheer);
		fdScriptShowMessage(guide, 0);
		return;
	}
	mnMessageArgumentPush(friendId, MESSAGE_ARGUMENT_ACTOR);
	fdScriptSetMessageId(MsgColossoIsaacWillCheerForWay);
	fdScriptShowMessage(guide, 0);
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
	column = FIELD_METATILE(actor->x.fixed);
	syFieldGameFlagSetByte(
		COLOSSO_CHEER_SLOT(friendId) + COLOSSO_CHEER_COLUMN, column);
	row = FIELD_METATILE(actor->z.fixed);
	syFieldGameFlagSetByte(
		COLOSSO_CHEER_SLOT(friendId) + COLOSSO_CHEER_ROW, row);
}

#undef COLOSSO_CHEER_COLUMN
#undef COLOSSO_CHEER_ROW
#undef COLOSSO_CHEER_SLOT
#undef COLOSSO_STAGE_CHEERED
#undef COLOSSO_STAGE_DESCRIBED

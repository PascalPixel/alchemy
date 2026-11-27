/* An actor's screen position, the encounter transition, and the context effects
   of leaving through an exit and arriving. Part of script_commands.c. */

/* Writes the actor's position on screen, in pixels, to position[0] and
   position[1]; an actor with a sprite is raised by its frame box. Returns -1
   when the actor does not exist. */
Sint32 fdActorGetScreenPos(Sint32 actorId, Sint32 *position)
{
	ACTOR *actor = fdActorLookup(actorId);
	Sint32 cameraX;
	Sint32 cameraZ;
	FIXED  x;
	FIXED  z;

	if(actor == 0)
		return -1;
	cameraX = syMapStateView(Ram_HeapSlots->mapGrid)[0] & FIELD_PIXEL_MASK;
	cameraZ = syMapStateView(Ram_HeapSlots->mapGrid)[1] & FIELD_PIXEL_MASK;
	x = actor->x.fixed - cameraX;
	z = actor->z.fixed - cameraZ - actor->y.fixed;
	*position++ = x / toFIXED(1);
	*position = z / toFIXED(1);
	if((actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE)
		*position -= (Sint8)fdResourceGetMetadataRecordStubStub(
			((SPR *)actor->spr)->entries[0]->anmId)
						 ->sprTop;
	return 0;
}

/* The encounter's screen closes over ENCOUNTER_CLOSE_FRAMES, and its white
   flash then fades to black over ENCOUNTER_FADE_FRAMES, ENCOUNTER_FADE_STEP
   levels a frame from just below RGB_LEVEL_MAX. */
#define ENCOUNTER_CLOSE_FRAMES 16
#define ENCOUNTER_FADE_FRAMES  16
#define ENCOUNTER_FADE_STEP	   2

/* Starts the transition into a random battle: plays the scene's battle music
   and the encounter sounds, closes the screen and flashes the backdrop white,
   fading it to black. */
void fdStartEncounterTransition(Sint32 unused)
{
	FIELD_STATE *state;
	Sint32		 i;
	Sint32		 level;

	state = Ram_HeapSlots->fieldState;
	sySndPlayStub(MainRoutineStateWork.battleMusic);
	sySndPlayStub(SE_STOP_EXIT_SE);
	sySndPlayStub(SE_BEAST_CRY);
	if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
		/* On the World Map the flash colours the window palette's black
		   instead, and the screen closes to the centre line in flickers; a
		   field scene closes with WINDOW_TRANSITION_LEADER_INVERSE. */
		syIOWrite16(RGB_WHITE,
			(Uint16 *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_COLOR_BLACK));
		fdScreenTransitionFinish(
			SCENE_TRANSITION(TRANSITION_CENTRE_LINE, CENTRE_LINE_FLICKERING),
			ENCOUNTER_CLOSE_FRAMES);
		state->transitionActive = 0;
		sySynch(ENCOUNTER_CLOSE_FRAMES);
		for(i = 0; i < ENCOUNTER_FADE_FRAMES; i++) {
			level = RGB_LEVEL_MAX - 1 - i * ENCOUNTER_FADE_STEP;
			syIOWrite16(RGB(level, level, level),
				(Uint16 *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_COLOR_BLACK));
			sySynch(1);
		}
	} else {
		syIOWrite16(RGB_WHITE, &PLTT_BACKDROP);
		fdScreenTransitionFinish(SCENE_TRANSITION(TRANSITION_WINDOW,
									 WINDOW_TRANSITION_LEADER_INVERSE),
			ENCOUNTER_CLOSE_FRAMES);
		state->transitionActive = 0;
		sySynch(ENCOUNTER_CLOSE_FRAMES);
		for(i = 0; i < ENCOUNTER_FADE_FRAMES; i++) {
			level = RGB_LEVEL_MAX - 1 - i * ENCOUNTER_FADE_STEP;
			syIOWrite16(RGB(level, level, level), &PLTT_BACKDROP);
			sySynch(1);
		}
	}
}

void *fdSprAddAnimationStub(void *, Sint32);

/* fdExitPrepareAnimation for any actor: centres the actor on its metatile at
   the metatile's south edge, stops it, hides its shadow and plays
   ANIM_LEADER_LEAVE (side 1) or ANIM_LEADER_LEAVE_ALTERNATE over 18 frames, its
   sprite gaining the doorway (SPRITE_DOORWAY) as a second part. */
void fdExitPrepareActorAnimation(Sint32 actorId, Sint32 side)
{
	ACTOR		  *actor;
	SPR			  *sprite;
	SPR_ANIMATION *doorway;
	Sint32		   animation;

	actor = fdActorLookup(actorId);
	sprite = actor->spr;
	animation = (side != 1);
	animation = ANIM_LEADER_LEAVE - animation;
	doorway = (SPR_ANIMATION *)fdSprAddAnimationStub(sprite, SPRITE_DOORWAY);
	{
		Uint32 cleared = 0;
		Uint8  hidden = SPRITE_COLOR_HIDDEN;

		sprite->flg = cleared;
		doorway->colMode = hidden;
		actor->x.fixed = (actor->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
		actor->z.fixed = (actor->z.fixed & FIELD_METATILE_MASK) + toFIXED(16);
		actor->velX = cleared;
		actor->velZ = cleared;
		actor->tgtX = ACTOR_NO_TARGET;
		actor->tgtZ = ACTOR_NO_TARGET;
		fdActorSelectAnimationStub(actor, animation);
		sySynch(18);
	}
}

/* Starts the leader's animation for leaving through an exit: centres it on its
   metatile, at the metatile's south edge, stops it and plays animation over 18
   frames, its sprite gaining the doorway (SPRITE_DOORWAY) as a second part. */
void fdExitPrepareAnimation(Sint32 animation)
{
	ACTOR		  *actor;
	SPR			  *sprite;
	SPR_ANIMATION *doorway;

	actor = fdActorLookup(MainRoutineStateWork.leaderActorId);
	sprite = actor->spr;
	doorway = (SPR_ANIMATION *)fdSprAddAnimationStub(sprite, SPRITE_DOORWAY);
	{
		Uint32 cleared = 0;
		Uint8  hidden = SPRITE_COLOR_HIDDEN;

		/* No shadow as the leader leaves. */
		sprite->flg = cleared;
		doorway->colMode = hidden;
		/* Keep the metatile and put the leader at its middle in x and its south
		   edge in z. */
		actor->x.fixed = (actor->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
		actor->z.fixed = (actor->z.fixed & FIELD_METATILE_MASK) + toFIXED(16);
		actor->velX = cleared;
		actor->velZ = cleared;
		actor->tgtX = ACTOR_NO_TARGET;
		actor->tgtZ = ACTOR_NO_TARGET;
		fdActorSelectAnimationStub(actor, animation);
		sySynch(18);
	}
}

/* Leaves through a stairs-up exit (ANIM_LEADER_LEAVE); FLAG_ARRIVAL_STAIRS_UP
   makes the next scene play the matching arrival (fdEntrancePlayArrival). */
void fdExitStartStairsUp(void)
{
	fdExitPrepareAnimation(ANIM_LEADER_LEAVE);
	syGameFlagSet(FLAG_ARRIVAL_STAIRS_UP);
}

/* Leaves through a stairs-down exit (ANIM_LEADER_LEAVE_ALTERNATE), recorded in
   FLAG_ARRIVAL_STAIRS_DOWN. */
void fdExitStartStairsDown(void)
{
	fdExitPrepareAnimation(ANIM_LEADER_LEAVE_ALTERNATE);
	syGameFlagSet(FLAG_ARRIVAL_STAIRS_DOWN);
}

/* Plays the leader's arrival animation through an exit: centred on its metatile
   at the metatile's north edge, the leader plays animation for 30 frames, loses
   the doorway (SPRITE_DOORWAY) and walks 8 pixels south. */
void fdEntranceEndArrivalAnimation(Sint32 animation)
{
	ACTOR		  *leader = fdActorLookup(MainRoutineStateWork.leaderActorId);
	SPR			  *sprite = leader->spr;
	SPR_ANIMATION *doorway =
		(SPR_ANIMATION *)fdSprAddAnimationStub(sprite, SPRITE_DOORWAY);
	sprite->flg = 0;
	doorway->colMode = SPRITE_COLOR_HIDDEN;
	leader->x.fixed = (leader->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	leader->z.fixed &= FIELD_METATILE_MASK;
	fdActorSelectAnimationStub(leader, animation);
	sySynch(30);
	/* Back to one sprite part, with its shadow, walking 8 pixels south at 1
	   pixel a frame. */
	sprite->cnt = 1;
	fdSprAnimationClearStub(sprite->entries[1]);
	sprite->entries[1] = NULL;
	sprite->flg = SPRITE_FLAG_SHADOW;
	leader->acceleration = toFIXED(1);
	leader->speedLimit = toFIXED(1);
	fdActorSetMoveTargetStub(
		leader, leader->x.fixed, leader->y.fixed, leader->z.fixed + toFIXED(8));
	fdActorWaitForMoveStub(leader);
}

/* Plays the leader's arrival for the way the last scene was left: through a
   stairs-up exit (FLAG_ARRIVAL_STAIRS_UP) or a stairs-down one
   (FLAG_ARRIVAL_STAIRS_DOWN), or falling in from 160 pixels above
   (FLAG_ARRIVE_FALLING). Returns the ENTRANCE_ARRIVAL it played. */
Sint32 fdEntrancePlayArrival(void)
{
	Sint32 result = ARRIVAL_NONE;
	Sint32 flag = FLAG_ARRIVAL_STAIRS_UP;

	if(syGameFlagGet(flag) != 0) {
		fdEntranceEndArrivalAnimation(ANIM_LEADER_ARRIVE);
		syGameFlagClear(flag);
		result = ARRIVAL_STAIRS_UP;
	} else {
		flag = FLAG_ARRIVAL_STAIRS_DOWN;
		if(syGameFlagGet(flag) != 0) {
			fdEntranceEndArrivalAnimation(ANIM_LEADER_ARRIVE_ALTERNATE);
			syGameFlagClear(flag);
			result = ARRIVAL_STAIRS_DOWN;
		} else {
			flag = FLAG_ARRIVE_FALLING;
			if(syGameFlagGet(flag) != 0) {
				Sint32 id;
				ACTOR *actor;

				syGameFlagClear(flag);
				id = MainRoutineStateWork.leaderActorId;
				actor = fdActorLookup(id);
				actor->y.fixed += toFIXED(160);
				fdCameraMoveWithinMapBounds(-1, -1, -1, 0);
				while(actor->y.fixed + actor->velY > actor->groundHeight) {
					sySynch(1);
				}
				sySndPlayStub(SE_DOOR_CLOSE);
				actor->y.fixed = actor->groundHeight;
				fdActorSelectAnimationStub(actor, ANIM_LEADER_LEAN_BACK);
				fdScriptWait(15);
				fdCameraFollowActor(id, 1);
				result = ARRIVAL_FALLING;
			}
		}
	}
	return result;
}

/* An EXIT_STYLE_HIDDEN_LAYER exit: gives the leader's sprite a second part,
   SPRITE_HIDDEN_LAYER, a band over the top 8 rows of its 32 by 32 picture,
   drawn in SPRITE_COLOR_HIDDEN. */
void fdExitAddHiddenLeaderLayer(void)
{
	ACTOR		  *actor = fdActorLookup(MainRoutineStateWork.leaderActorId);
	SPR_ANIMATION *layer =
		fdSprAddAnimationStub(actor->spr, SPRITE_HIDDEN_LAYER);

	layer->colMode = SPRITE_COLOR_HIDDEN;
}

#undef ENCOUNTER_CLOSE_FRAMES
#undef ENCOUNTER_FADE_FRAMES
#undef ENCOUNTER_FADE_STEP

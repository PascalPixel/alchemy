/* @file */
/* Lemuria's palace. From the city's avenue the party comes into the entrance */
/* hall, whose west door opens on a room of dressers and whose east door opens
 */
/* on the barracks. Two stairs at the back of the hall lead up into the throne
 */
/* room, where a guard stops Felix before King Hydros; then the king, Lunpa and
 */
/* Lord Conservato receive the party and show it two glowing maps, the ancient
 */
/* world and the one Lunpa had. Stairs either side of the throne climb to a */
/* room with a painting, whose passage south leads to a bedroom also entered */
/* from the top of the city. */

/* This unit declares its own form of fdEventEnd, below. */
#define OWN_Event_End

#include "scene/event_layer.h"
#include "field/field_state.h"
#include "field/field.h"
#include "game/game.h"
#include "field/event_inline.h"
#include "scene/scene.h"
#include "field/call.h"
#include "macros.h"
#include "text/msg_ids.h"

/* The ROM's callers here are compiled with no declaration in scope, as against
 */
/* this form: no parameter list and an s32 result, where the definition takes */
/* nothing and returns nothing. scLemuriaPalaceMeetKingHydros returns what it */
/* leaves behind. */
s32 fdEventEnd();

/* The palace, by the rows of ActorPlacements; the rows for Jenna, Sheba */
/* and Piers go to their party slots. */
typedef enum LEMURIA_PALACE_ACTOR_ID {
	/* "He is still in chambers with Lord Conservato." */
	ACTOR_GIRL_WAITING_ON_THE_KING = ACTOR_FIRST_PLACED,
	/* "If you seek the counsel of His Highness, the king, follow this */
	/* passage..." */
	ACTOR_WOMAN_SHOWING_THE_WAY,
	/* "His Highness never leaves a meeting with Conservato in a good mood." */
	ACTOR_MAN_ON_THE_KINGS_MOOD,
	/* "Conservato and the king were yelling so loudly that I could hear */
	/* them..." */
	ACTOR_LEMURIAN_WHO_HEARD_THE_YELLING,
	/* "King Hydros is the wisest of all the Lemurian kings..." */
	ACTOR_LEMURIAN_PRAISING_HYDROS,
	ACTOR_HYDROS,
	/* "So it would, Your Highness." "There is no need, Lunpa." */
	ACTOR_LUNPA,
	ACTOR_CONSERVATO,
	/* "You are Lord Felix, correct?" */
	ACTOR_PALACE_GUARD,
	ACTOR_KRADEN
} LEMURIA_PALACE_ACTOR_ID;

/* The ways between the palace's rooms, each an exit and the entrance beside */
/* it, in pairs: each way leads to its partner's entrance and back. The hall's
 */
/* doors and the bedroom's open as Felix walks into them. */
typedef enum LEMURIA_PALACE_WAY {
	WAY_HALL_WEST_DOOR = 2,	   /* to the dresser room's door */
	WAY_HALL_EAST_DOOR = 3,	   /* to the barracks' door */
	WAY_HALL_WEST_STAIRS = 4,  /* up to the throne room's south-west corner */
	WAY_HALL_EAST_STAIRS = 5,  /* up to the throne room's south-east corner */
	WAY_DRESSER_ROOM_DOOR = 6, /* to the hall's west door */
	WAY_BARRACKS_DOOR = 7,	   /* to the hall's east door */
	WAY_THRONE_ROOM_SOUTH_WEST = 8,	  /* down the hall's west stairs */
	WAY_THRONE_ROOM_SOUTH_EAST = 9,	  /* down the hall's east stairs */
	WAY_THRONE_ROOM_WEST_STAIRS = 11, /* up to the painting room's west side */
	WAY_THRONE_ROOM_EAST_STAIRS = 12, /* up to the painting room's east side */
	WAY_PAINTING_ROOM_WEST_STAIRS = 13, /* down to the throne room's west */
	WAY_PAINTING_ROOM_EAST_STAIRS = 14, /* down to the throne room's east */
	WAY_PAINTING_ROOM_PASSAGE = 15,		/* south to the bedroom's door */
	WAY_BEDROOM_DOOR = 16,				/* north to the painting room */
} LEMURIA_PALACE_WAY;

/* The cells before the throne where the guard stops Felix */
/* (scLemuriaPalaceMeetKingHydros). */
enum { CELL_BEHAVIOR_BEFORE_THRONE = 20 };

/* The glow palette: every sixth frame its rotating colours move along by one,
 */
/* and every four frames its glow colour takes the next of GLOW_STEPS steps. */
#define GLOW_PALETTE		 12
#define ROTATING_FIRST_COLOR 1
#define ROTATING_LAST_COLOR	 14
#define GLOW_COLOR			 15
#define GLOW_STEPS			 12

/* What scLemuriaPalaceDrawMaps does with the maps' blend each frame. */
typedef enum LEMURIA_PALACE_MAP_MODE {
	MAP_MODE_HELD,		/* leaves it as the fades set it */
	MAP_MODE_SHIMMER,	/* the shown map dims a little and back, now and then */
	MAP_MODE_ALTERNATE, /* the two maps fade into each other, back and forth */
} LEMURIA_PALACE_MAP_MODE;

/* The maps' pictures, 64 by 64 pixels of 256 colours each. */
#define MAP_PICTURE_BYTES (64 * 64)
/* The steps of LemuriaPalace_MapFadeLevels the shown map shimmers through, */
/* eight frames apiece. */
#define MAP_FADE_STEPS 16
/* The steps of LemuriaPalace_MapBlendSteps, four frames apiece: the first map
 */
/* shows from the first and the second from the middle. */
#define MAP_BLEND_STEPS 28

extern MAP_GRID_CELL_ANIMATION LemuriaPalace_Doors[];
extern MAP_GRID_CELL_ANIMATION LemuriaPalace_BedroomDoor;

void scLemuriaPalaceDrawMaps(void);

extern u16 LemuriaPalace_DoorOpening[];

s32 LemuriaPalace_FirstMapVram BSS_DATA;
s32 LemuriaPalace_SecondMapShown BSS_DATA;
/* The sprite each of the two maps is drawn as. */
EFFECT_SPR LemuriaPalace_SecondMapSprite BSS_DATA ALIGNED(16);
s32 LemuriaPalace_SecondMapTile BSS_DATA;
EFFECT_SPR LemuriaPalace_FirstMapSprite BSS_DATA ALIGNED(16);
s32 LemuriaPalace_FirstMapTile BSS_DATA;
s8 LemuriaPalace_MapMode BSS_DATA;
s32 LemuriaPalace_FirstMapShown BSS_DATA;
s32 LemuriaPalace_SecondMapVram BSS_DATA;
s32 LemuriaPalace_MapTime BSS_DATA;
extern u8 LemuriaPalace_MapFadeLevels[];
extern const u8 LemuriaPalace_FirstMapTiles[];
extern const u8 LemuriaPalace_SecondMapTiles[];

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) { return ActorPlacements; }

#include "shared/leader_open_door_and_enter.h"

/* Felix walks north into one of the palace's doors: it opens and he goes */
/* through. The entrance hall's west door, cell (5, 23), leads to a room of */
/* dressers where an old woman stands; its east door, cell (30, 23), to the */
/* barracks with their beds and barrel; and the door in the north wall of the */
/* bedroom, cell (56, 4), to the passage up to the room with a painting. */
void scLemuriaPalaceEnterDoor(s32 exit) {
	const MAP_GRID_CELL_ANIMATION *door = LemuriaPalace_Doors;

	if(exit == WAY_HALL_EAST_DOOR)
		door++;
	if(exit == WAY_BEDROOM_DOOR)
		door = &LemuriaPalace_BedroomDoor;
	fdLeaderOpenDoorAndEnter(door, exit, 0);
}

const TRIGGER *scGetTriggers(void) { return SceneEvents; }

extern s16 LemuriaPalace_GlowColors[];

/* A task: every sixth frame the glow palette's rotating colours move along by
 */
/* one; every frame its glow colour takes the glow's step for the frame, each */
/* step four frames long. */
void scLemuriaPalaceCycleGlowCols(void) {
	s32 color;

	if(FrameCount % 6 == 0) {
		s32 index;

		color = (s16)BG_PLTT_COLOR(GLOW_PALETTE, ROTATING_LAST_COLOR);
		index = ROTATING_LAST_COLOR;
		do {
			BG_PLTT_COLOR(GLOW_PALETTE, index) =
				BG_PLTT_COLOR(GLOW_PALETTE, index - 1);
		} while(--index != ROTATING_FIRST_COLOR);
		BG_PLTT_COLOR(GLOW_PALETTE, ROTATING_FIRST_COLOR) = color;
	}
	color = LemuriaPalace_GlowColors[(FrameCount >> 2) % GLOW_STEPS];
	BG_PLTT_COLOR(GLOW_PALETTE, GLOW_COLOR) = color;
}

/* The room's setup: it schedules the palette cycle */
/* scLemuriaPalaceCycleGlowCols. */
s32 scInit(void) {
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
	syTaskAddFrame(scLemuriaPalaceCycleGlowCols, TASK_PASS_VBLANK);
	scFieldActorSetSprPriority(ACTOR_GIRL_WAITING_ON_THE_KING, 3);
	if(MainRoutineStateWork.entrance == WAY_DRESSER_ROOM_DOOR)
		fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
			ACTOR_PRIORITY_IGNORES_RISES;
	if(fdEventGetFlag(FLAG_MET_KING_HYDROS)) {
		fdActorSetPos(ACTOR_PIERS, 0, 0);
		fdActorSetPos(ACTOR_CONSERVATO, 0, 0);
	}
	return 0;
}

s32 scPrepareMap(void) { return 0; }

/* Hydros raises a hand and two glints fly up, as each map is called up. The */
/* caller passes an actor, but the glints always come from Hydros. */
void scLemuriaPalaceCastMapSparks(s32 actorId) {
	ACTOR *actor = fdActorGet(ACTOR_HYDROS);
	ACTOR *first;
	ACTOR *second;

	fdActorSetAnimation(ACTOR_HYDROS, 5);
	syFieldEventWait(28);
	fdEventPlaySnd(SE_MENU_CONFIRM);
	first = fdActorSpawn(SPRITE_GLINT, actor->x.fixed + toFIXED(10),
						 actor->y.fixed + toFIXED(16), actor->z.fixed);
	first->speedLimit = toFIXED(2);
	first->acceleration = toFIXED(2);
	first->motionFlg = 0;
	scFieldActorSetMoveTargetFX(first, first->x.fixed + toFIXED(6),
								first->y.fixed + toFIXED(8), first->z.fixed);
	second = fdActorSpawn(SPRITE_GLINT, actor->x.fixed + toFIXED(10),
						  actor->y.fixed + toFIXED(16), actor->z.fixed);
	second->speedLimit = toFIXED(2);
	second->acceleration = toFIXED(2);
	second->motionFlg = 0;
	scFieldActorSetMoveTargetFX(second, first->x.fixed + toFIXED(8),
								first->y.fixed - toFIXED(4), first->z.fixed);
	scFieldActorWaitForMove(first);
	fdActorRelease(first);
	scFieldActorWaitForMove(second);
	fdActorRelease(second);
	syFieldEventWait(30);
}

extern u16 LemuriaPalace_MapBlendSteps[];

/* A task: draws the two maps floating above the throne room as they are */
/* shown, blending them by LemuriaPalace_MapMode. Each time the alternation */
/* turns to a map, a chime sounds. */
void scLemuriaPalaceDrawMaps(void) {
	VECTOR3 position;
	s8 mode = LemuriaPalace_MapMode;

	if(mode == MAP_MODE_SHIMMER) {
		s32 level = LemuriaPalace_MapFadeLevels[(LemuriaPalace_MapTime >> 3) &
												(MAP_FADE_STEPS - 1)];

		syIoWrite16(level << 8 | (BLEND_WEIGHT_MAX - level), &REG_BLDALPHA);
		LemuriaPalace_MapTime++;
	}
	else if(mode == MAP_MODE_ALTERNATE) {
		s32 *time = &LemuriaPalace_MapTime;
		s32 step = (*time >> 2) % MAP_BLEND_STEPS;

		{
			s32 alpha = LemuriaPalace_MapBlendSteps[step];

			REG_BLDALPHA = alpha;
		}
		if(step == 0) {
			LemuriaPalace_FirstMapShown = 1;
			LemuriaPalace_SecondMapShown = step;
			fdEventPlaySnd(SE_HIGH_JUMP);
		}
		if(step == MAP_BLEND_STEPS / 2) {
			LemuriaPalace_FirstMapShown = 0;
			LemuriaPalace_SecondMapShown = 1;
			fdEventPlaySnd(SE_HIGH_JUMP);
		}
		(*time)++;
	}
	position.x = toFIXED(832);
	position.y = 0;
	position.z = toFIXED(448);
	fdCameraWorldToScr(&position);
	if(LemuriaPalace_FirstMapShown != 0) {
		LemuriaPalace_FirstMapSprite.x = position.x;
		LemuriaPalace_FirstMapSprite.y = position.z;
		fdEffectSprDraw(&LemuriaPalace_FirstMapSprite);
	}
	if(LemuriaPalace_SecondMapShown != 0) {
		LemuriaPalace_SecondMapSprite.x = position.x;
		LemuriaPalace_SecondMapSprite.y = position.z;
		fdEffectSprDraw(&LemuriaPalace_SecondMapSprite);
	}
}

/* Loads the two maps the council shows the party, the ancient world and */
/* Lunpa's, and starts drawing them. */
void scLemuriaPalaceLoadMaps(void) {
	void *buffer = syHeapCallAlternatePool(syHeapAlloc, MAP_PICTURE_BYTES);
	s32 *firstSlot;
	s32 *firstTile;
	s32 *secondSlot;
	s32 *secondTile;

	syIoWrite16(BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL, &REG_BLDCNT);
	syIoWrite16(BLEND_ALPHA(0, BLEND_WEIGHT_MAX), &REG_BLDALPHA);
	syDecompressTaggedLz(LemuriaPalace_FirstMapTiles, buffer);
	firstSlot = &LemuriaPalace_FirstMapVram;
	*firstSlot = syVramCacheFindFreeEntry();
	firstTile = &LemuriaPalace_FirstMapTile;
	*firstTile =
		mnVramLoad(LemuriaPalace_FirstMapVram, MAP_PICTURE_BYTES, buffer);
	fdEffectSprInit(&LemuriaPalace_FirstMapSprite, 0, 0, OBJ_SIZE_64X64,
					LemuriaPalace_FirstMapTile);
	LemuriaPalace_FirstMapSprite.depth = 1;
	LemuriaPalace_FirstMapSprite.oam.pri = 2;
	LemuriaPalace_FirstMapSprite.oam.objMode = 1;
	LemuriaPalace_FirstMapSprite.oam.fullCol = 1;
	LemuriaPalace_FirstMapSprite.oam.pal = 0;
	syDecompressTaggedLz(LemuriaPalace_SecondMapTiles, buffer);
	secondSlot = &LemuriaPalace_SecondMapVram;
	*secondSlot = syVramCacheFindFreeEntry();
	secondTile = &LemuriaPalace_SecondMapTile;
	*secondTile =
		mnVramLoad(LemuriaPalace_SecondMapVram, MAP_PICTURE_BYTES, buffer);
	fdEffectSprInit(&LemuriaPalace_SecondMapSprite, 0, 0, OBJ_SIZE_64X64,
					LemuriaPalace_SecondMapTile);
	LemuriaPalace_SecondMapSprite.depth = 2;
	LemuriaPalace_SecondMapSprite.oam.pri = 2;
	LemuriaPalace_SecondMapSprite.oam.objMode = 1;
	LemuriaPalace_SecondMapSprite.oam.fullCol = 1;
	LemuriaPalace_SecondMapSprite.oam.pal = 0;
	LemuriaPalace_FirstMapShown = 0;
	LemuriaPalace_SecondMapShown = 0;
	LemuriaPalace_MapTime = 0;
	LemuriaPalace_MapMode = MAP_MODE_HELD;
	syTaskAddFrame(scLemuriaPalaceDrawMaps, TASK_PASS_FRAME);
	syHeapRelease(buffer);
}

/* Stops drawing the maps and frees their tiles. */
void scLemuriaPalaceFreeMaps(void) {
	syFieldTaskRemove(scLemuriaPalaceDrawMaps);
	syVramCacheFreeEntry(LemuriaPalace_FirstMapVram);
	syVramCacheFreeEntry(LemuriaPalace_SecondMapVram);
}

/* Fades the shown map in over 16 steps of four frames, then lets it shimmer. */
void scLemuriaPalaceFadeMapsIn(void) {
	s32 i;

	syIoWrite16(BLEND_ALPHA(0, BLEND_WEIGHT_MAX), &REG_BLDALPHA);
	LemuriaPalace_MapMode = MAP_MODE_HELD;
	for(i = 0; i < BLEND_WEIGHT_MAX; i++) {
		syIoWrite16(BLEND_ALPHA(i, BLEND_WEIGHT_MAX - i), &REG_BLDALPHA);
		syWait(4);
	}
	syIoWrite16(BLEND_ALPHA(BLEND_WEIGHT_MAX, 0), &REG_BLDALPHA);
	LemuriaPalace_MapMode = MAP_MODE_SHIMMER;
}

/* Fades the shown map out over 16 steps of three frames. */
void scLemuriaPalaceFadeMapsOut(void) {
	s32 i;

	syIoWrite16(BLEND_ALPHA(BLEND_WEIGHT_MAX, 0), &REG_BLDALPHA);
	LemuriaPalace_MapMode = MAP_MODE_HELD;
	for(i = 0; i < BLEND_WEIGHT_MAX; i++) {
		syIoWrite16((i << 8) | (BLEND_WEIGHT_MAX - i), &REG_BLDALPHA);
		syWait(3);
	}
	syIoWrite16(BLEND_ALPHA(0, BLEND_WEIGHT_MAX), &REG_BLDALPHA);
}

/* Felix walks up the throne room towards King Hydros, cells (53, 35) and */
/* (54, 35), the first time: the guard steps in front of him, "You are Lord */
/* Felix, correct?" Answered no, the guard sends the party back. Answered yes,
 */
/* he steps aside, and King Hydros and his council receive the warriors Piers */
/* spoke of. */
s32 scLemuriaPalaceMeetKingHydros(void) {
	s32 answer, frame, growFrame;
	ACTOR *target;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId((s32)MsgLemuriaPalaceYouAreLordFelixCorrect);
	fdActorSetSpeed(ACTOR_PALACE_GUARD, 1.2);
	fdActorWalkByAndWait(ACTOR_PALACE_GUARD, -24, 0);
	fdActorFaceDirectionAndWait(ACTOR_PALACE_GUARD, FACING_SOUTH_SOUTHWEST);
	fdActorFaceActor(ACTOR_PALACE_GUARD, MainRoutineStateWork.leaderActorId, 0);
	/* "You are Lord Felix, correct?" */
	fdEventOpenMessage(ACTOR_PALACE_GUARD, 0);
	answer = fdEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0);
	if(answer != 0) {
		fdEventWait(30);
		fdActorSetAnimationAndWait(ACTOR_PALACE_GUARD, 4);
		/* No: "Then you have no business here! His Majesty is involved in a..."
		 */
		fdEventShowMessageThenWait(ACTOR_PALACE_GUARD, 0, 5);
		fdActorWalkByAndWait(MainRoutineStateWork.leaderActorId, 0, 16);
		fdActorWalkByAndWait(ACTOR_PALACE_GUARD, 24, 0);
		fdActorFaceDirectionAndWait(ACTOR_PALACE_GUARD, FACING_SOUTH_SOUTHWEST);
		return fdEventEnd();
	}
	fdActorSetAnimationAndWait(ACTOR_PALACE_GUARD, 3);
	fdEventWait(10);
	fdActorWalkByAndWait(ACTOR_PALACE_GUARD, 24, 0);
	fdActorFaceDirection(ACTOR_PALACE_GUARD, FACING_SOUTH_SOUTHWEST, 0);
	fdActorWatchAndSetScript(ACTOR_PALACE_GUARD,
							 MainRoutineStateWork.leaderActorId);
	fdEventSetFlag(FLAG_MET_KING_HYDROS);
	fdEventSetFlag(FLAG_SHEBA_TO_ASK_PIERS_AGE);
	gmPartyAddMember(PIERS);
	/* Flattened out of sight until Lunpa brings it out. */
	fdActorGetById(FIELD_TREASURE_ACTOR)->scaleY = 0;
	fdActorFaceDirection(ACTOR_LUNPA, FACING_EAST, 0);
	fdActorSetSpeed(ACTOR_HYDROS, 0.7);
	fdActorSetSpeed(ACTOR_LUNPA, 0.7);
	fdActorSetSpeed(ACTOR_PIERS, 0.7);
	fdActorWalkToAndWait(ACTOR_FELIX, 872, 528);
	fdCameraMoveTo(872, CAMERA_KEEP, 408, 1);
	fdCameraWaitForArrival();
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_PALACE_GUARD, FACING_SOUTH_SOUTHWEST, 0);
	fdActorShowEmote(ACTOR_HYDROS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 45);
	fdActorFaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHWEST);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_HYDROS, 3);
	fdEventWait(10);
	/* Yes: past the guard's "Then you have no business here!" */
	FieldStateWork->msg = (FieldStateWork->msg) + 1;
	/* "It would seem the warriors of which you spoke have arrived." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0,
									5);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_CONSERVATO, FACING_SOUTH_SOUTHWEST);
	fdEventWait(30);
	fdActorFaceDirectionAndWait(ACTOR_LUNPA, FACING_EAST);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_LUNPA, 3);
	fdEventWait(10);
	/* "So it would, Your Highness. I shall call them to us immediately." */
	fdEventShowMessageThenWait(ACTOR_LUNPA, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_HYDROS, 4);
	/* "There is no need, Lunpa. Let us go to them." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0,
									5);
	fdActorRunRepeatedMotion(ACTOR_CONSERVATO, 2);
	fdEventWait(5);
	fdActorFaceDirectionAndWait(ACTOR_CONSERVATO, FACING_WEST);
	fdActorShowEmote(ACTOR_CONSERVATO, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	/* "Hydros! Would you see me dishonored thus? They ought to present..." */
	fdEventShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_HYDROS, FACING_EAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_LUNPA, FACING_EAST);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_HYDROS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "I do not go to present myself to them, Conservato. I am merely going..."
	 */
	fdEventShowMessageThenWait(ACTOR_HYDROS, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_CONSERVATO, 2);
	/* "Regardless of your reasons, sire, I will not go to them!" */
	fdEventShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_HYDROS, 3);
	fdEventWait(10);
	/* "Do as you will, Conservato." */
	fdEventShowMessageThenWait(ACTOR_HYDROS, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHWEST);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_HYDROS, 3);
	fdEventWait(10);
	/* "However, we shall go." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0,
									5);
	fdCameraFollowActor(ACTOR_HYDROS, 1);
	fdActorWalkTo(ACTOR_HYDROS, 864, 444);
	fdEventWait(20);
	fdActorWalkTo(ACTOR_LUNPA, 848, 444);
	fdEventWait(20);
	fdActorWalkTo(ACTOR_PIERS, 832, 444);
	fdActorWaitForMoveById(ACTOR_HYDROS);
	fdActorSetAnimation(ACTOR_HYDROS, 1);
	fdActorFaceDirection(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST, 0);
	fdActorWaitForMoveById(ACTOR_LUNPA);
	fdActorSetAnimation(ACTOR_LUNPA, 1);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST, 0);
	fdActorWaitForMoveById(ACTOR_PIERS);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_STAND);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH);
	fdCameraMoveTo(872, CAMERA_KEEP, 496, 1);
	fdCameraWaitForArrival();
	fdEventWait(5);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, 16, 0, FACING_NORTH_NORTHWEST);
	scFieldActorLaunchFromLeader(ACTOR_JENNA, -16, 0, FACING_NORTH);
	scFieldActorLaunchFromLeader(ACTOR_SHEBA, -32, 0, FACING_NORTH);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorWaitForMoveById(ACTOR_SHEBA);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_HYDROS, 3);
	fdEventWait(10);
	/* "I heard of your arrival. It is quite remarkable that you were able
	 * to..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0,
									5);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	fdEventWait(10);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH);
	fdEventWait(10);
	/* "Allow me to introduce my companions. This is Sheba..." */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 5);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHWEST);
	fdActorWalkByAndWait(ACTOR_SHEBA, 0, -8);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	fdEventWait(10);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH);
	/* "Jenna..." */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 5);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHWEST);
	fdActorWalkByAndWait(ACTOR_JENNA, 0, -8);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdEventWait(10);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHEAST);
	/* "Felix..." */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 5);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST);
	fdActorWalkByAndWait(ACTOR_FELIX, 0, -8);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	fdEventWait(10);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHEAST);
	/* "...And Kraden." */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 5);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST);
	fdActorGetById(ACTOR_KRADEN)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_KRADEN, 0, -8);
	fdEventWait(1);
	fdActorGetById(ACTOR_KRADEN)->steeringFlg |= ACTOR_FACES_MOTION;
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdEventWait(10);
	fdActorFaceDirectionAndWait(ACTOR_LUNPA, FACING_EAST);
	fdEventWait(10);
	/* "Kraden is the one who told me of the outside world's civilizations,..."
	 */
	fdEventShowMessageThenWait(ACTOR_LUNPA, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_HYDROS, FACING_WEST);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_HYDROS, 3);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_HYDROS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Ah, Kraden! Lunpa describes you as a remarkable scholar. Your..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0,
									5);
	fdActorStartRepeatedMotion(ACTOR_KRADEN, 3);
	fdActorSetAttachedEffect(ACTOR_KRADEN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(37);
	/* "Thank you, Your Highness." */
	fdEventShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_HYDROS, 3);
	fdEventWait(10);
	/* "Your theories are correct. Our world is now on a path to destruction."
	 */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0,
									0);
	/* "And who has decided this? You, Hydros?" */
	fdEventShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_HYDROS, FACING_NORTH_NORTHEAST);
	fdEventWait(15);
	/* "Some civilizations have vanished, surely, but that is the way of the..."
	 */
	fdEventShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST);
	fdEventWait(10);
	/* "Don't delude yourself. Lemuria has been in slow decline for ages
	 * now,..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0,
									5);
	fdActorShowEmote(ACTOR_HYDROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
	/* "My most trusted scholars have reached this conclusion after
	 * extensive..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0,
									5);
	fdActorRunRepeatedMotion(ACTOR_HYDROS, 2);
	fdEventWait(5);
	/* "I sent Piers to confirm this for me..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0,
									0);
	/* "So you admit it, Hydros! You violated the directive of the senate!..."
	 */
	fdEventShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_PIERS, 2);
	fdEventWait(10);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHEAST);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "You are incorrect. The tidal wave carried me away by chance alone." */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_HYDROS, FACING_NORTH_NORTHEAST);
	fdEventWait(5);
	/* "Conservato, you must know that the tidal wave was caused by..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0,
									5);
	fdActorSetAnimationAndWait(ACTOR_HYDROS, 4);
	/* "Only a fool would have chosen to send a man to sea if he knew that..."
	 */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0,
									5);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	/* "What was it that you intended to have Piers confirm?" */
	fdEventShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_HYDROS, 3);
	fdEventWait(10);
	scLemuriaPalaceCastMapSparks(ACTOR_HYDROS);
	fdEventStartColTransition(32, RGB(8, 8, 8));
	fdEventWait(32);
	fdEventWait(20);
	scLemuriaPalaceLoadMaps();
	LemuriaPalace_FirstMapShown = 1;
	LemuriaPalace_SecondMapShown = 0;
	fdEventPlaySnd(SE_HIGH_JUMP);
	scLemuriaPalaceFadeMapsIn();
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 55);
	/* "This map shows the ancient world, when Lemuria still traded with..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0,
									5);
	/* "It was a great age... A time when the world shined brightly." */
	fdEventShowMessageThenWait(ACTOR_CONSERVATO, 0, 10);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
	scLemuriaPalaceCastMapSparks(ACTOR_HYDROS);
	scLemuriaPalaceFadeMapsOut();
	fdEventWait(30);
	LemuriaPalace_FirstMapShown = 0;
	LemuriaPalace_SecondMapShown = 1;
	fdEventPlaySnd(SE_HIGH_JUMP);
	scLemuriaPalaceFadeMapsIn();
	/* "And this...is the map Lunpa had with him when he arrived in Lemuria..."
	 */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0,
									10);
	/* "I used this map to sail around the world. It is quite accurate." */
	fdEventShowMessageThenWait(ACTOR_LUNPA, 0, 5);
	fdActorGetById(ACTOR_KRADEN)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_KRADEN, 0, -16);
	fdEventWait(1);
	fdActorGetById(ACTOR_KRADEN)->steeringFlg |= ACTOR_FACES_MOTION;
	fdEventWait(25);
	fdActorStartRepeatedMotion(ACTOR_KRADEN, 3);
	fdActorSetAttachedEffect(ACTOR_KRADEN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(40);
	/* "In-Incredible!" */
	fdEventShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorShowEmote(ACTOR_HYDROS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "You understand, Kraden..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0,
									5);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_EAST);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "What do you understand?" */
	fdEventShowMessageThenWait(ACTOR_SHEBA, 0, 5);
	fdActorWalkByAndWait(ACTOR_KRADEN, 0, 16);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "Well, unless my eyes deceive me, the continents have shrunk since
	 * the..." */
	fdEventShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_EAST);
	fdEventWait(5);
	/* "That's insane!" */
	fdEventShowMessageThenWait(ACTOR_JENNA, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_LUNPA, 2);
	/* "It is not insane! It is true!" */
	fdEventShowMessageThenWait(ACTOR_LUNPA, 0, 5);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_NORTH);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_HYDROS, 3);
	fdEventWait(10);
	/* "Perhaps this will make the point more clearly. Look at these two
	 * maps..." */
	fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
	scLemuriaPalaceCastMapSparks(ACTOR_HYDROS);
	scLemuriaPalaceFadeMapsOut();
	fdEventWait(10);
	LemuriaPalace_MapTime = 0;
	LemuriaPalace_MapMode = MAP_MODE_ALTERNATE;
	fdEventWait(150);
	fdActorStartRepeatedMotion(ACTOR_KRADEN, 3);
	fdActorSetAttachedEffect(ACTOR_KRADEN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(40);
	/* "There's no mistaking it. The continents on Lunpa's map are clearly..."
	 */
	fdEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	/* "Lies! Nothing but lies!" */
	fdEventShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
	fdActorSetSpeed(ACTOR_CONSERVATO, 1);
	fdActorWalkToAndWait(ACTOR_CONSERVATO, 888, 444);
	fdActorFaceDirectionAndWait(ACTOR_CONSERVATO, FACING_SOUTH_SOUTHWEST);
	fdEventWait(5);
	fdCameraMoveTo(872, CAMERA_KEEP, 472, 1);
	fdCameraWaitForArrival();
	fdEventWait(10);
	fdActorFaceDirectionAndWait(ACTOR_CONSERVATO, FACING_WEST);
	fdEventWait(10);
	/* "What do you hope to gain from foisting these lies upon our people?" */
	fdEventShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_HYDROS, FACING_EAST);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_HYDROS, 4);
	/* "You know well that I hope to gain nothing more than the truth,..." */
	fdEventShowMessageThenWait(ACTOR_HYDROS, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_CONSERVATO, 2);
	fdEventWait(5);
	/* "And who do you expect will believe such nonsense?" */
	fdEventShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
	scFieldActorShowEmote(ACTOR_HYDROS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	fdActorFaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST);
	fdEventWait(10);
	scLemuriaPalaceCastMapSparks(ACTOR_HYDROS);
	fdEventPlaySnd(SE_HIGH_JUMP);
	/* Waits, two seconds at most, for the alternation to come round to a map.
	 */
	for(frame = 0; frame < 120; frame++) {
		sySynch(1);
		if((LemuriaPalace_MapTime >> 2) % (MAP_BLEND_STEPS / 2) == 0)
			break;
	}
	scLemuriaPalaceFreeMaps();
	fdStartPaletteTransition(32);
	fdEventWait(40);
	fdActorFaceDirectionAndWait(ACTOR_HYDROS, FACING_EAST);
	fdEventWait(10);
	/* "If you do not believe the evidence before your eyes, what do you
	 * believe?" */
	fdEventShowMessageThenWait(ACTOR_HYDROS, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_HYDROS, 2);
	fdEventWait(5);
	/* "Do you believe the Lemurian map to be accurate to this day?" */
	fdEventShowMessageThenWait(ACTOR_HYDROS, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_CONSERVATO, 3);
	fdEventWait(15);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_WEST, 0);
	/* A sliver short of west: from the east he turns round by the south, toward
	 */
	/* the viewer, where west itself would take him round by the north. */
	fdActorFaceDirectionAndWait(ACTOR_HYDROS, FACING_WEST - 1);
	fdEventWait(10);
	/* "Tell me, Piers... You were able to investigate this matter,..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0,
									5);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	fdEventWait(10);
	/* "Thanks to Felix and my companions, I was able to travel much..." */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 5);
	fdActorShowEmote(ACTOR_HYDROS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "And what did you find?" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0,
									5);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "I found, in truth, that Lunpa's map is the more accurate of the two." */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 5);
	fdActorShowEmote(ACTOR_HYDROS, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "However, the world seems even smaller now than it appears on Lunpa's..."
	 */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 5);
	fdActorShowEmote(ACTOR_LUNPA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_HYDROS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_EAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_HYDROS, FACING_EAST);
	fdEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_CONSERVATO, 2);
	/* "What are you saying?" */
	fdEventShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdEventWait(5);
	/* "Time itself has stopped..." */
	fdEventShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_CONSERVATO, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_EAST);
	fdEventWait(25);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
	fdEventWait(20);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Think of Weyard as a living, breathing being, possessing its own
	 * life..." */
	fdEventShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
	fdEventWait(15);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdEventWait(10);
	/* "The four elements are the nourishment needed to sustain this being." */
	fdEventShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_LUNPA, 2);
	fdEventWait(5);
	/* "Kraden... This is exactly what King Hydros himself has said to me!" */
	fdEventShowMessageThenWait(ACTOR_LUNPA, 0, 5);
	fdActorShowEmote(ACTOR_HYDROS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Ever since Alchemy was sealed away, the world has been cut off from..."
	 */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0,
									5);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdEventWait(10);
	/* "By using less energy, Weyard prolongs its life..." */
	fdEventShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_HYDROS, 4);
	/* "Yet when hibernating, the being has only stopped its own clock...
	 * not..." */
	fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "When a bear sleeps through the winter, it needs only wait for spring..."
	 */
	fdEventShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_PIERS, 2);
	fdEventWait(5);
	/* "But if spring never comes, the bear will eventually die..." */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 5);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_LUNPA, 4);
	/* "Weyard is wasting away, its continents shrinking, because its spring..."
	 */
	fdEventShowMessageThenWait(ACTOR_LUNPA, 0, 5);
	scFieldActorShowEmote(ACTOR_CONSERVATO, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	fdActorFaceDirectionAndWait(ACTOR_CONSERVATO, FACING_WEST);
	fdEventWait(10);
	/* "Hydros, how can you make such outrageous claims? The world is not..." */
	fdEventShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_HYDROS, 4);
	fdActorFaceDirectionAndWait(ACTOR_HYDROS, FACING_EAST);
	fdEventWait(10);
	/* "Tell me, Conservato, why do you think Poseidon has returned?" */
	fdEventShowMessageThenWait(ACTOR_HYDROS, 0, 5);
	scFieldActorShowEmote(ACTOR_CONSERVATO, EMOTE_IN_FRONT | EMOTE_QUESTION,
						  50);
	fdActorSetAnimationAndWait(ACTOR_CONSERVATO, 4);
	/* "Do you have the answer, Hydros?" */
	fdEventShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_HYDROS, 3);
	fdEventWait(10);
	/* "The elemental lighthouses..." */
	fdEventShowMessageThenWait(ACTOR_HYDROS, 0, 5);
	scFieldActorShowEmote(ACTOR_CONSERVATO, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
						  45);
	fdActorFaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST);
	fdEventWait(5);
	/* "Felix and his companions have lit the beacons on both..." */
	fdEventShowMessageThenWait(ACTOR_HYDROS, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_CONSERVATO, FACING_SOUTH_SOUTHWEST);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_CONSERVATO, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	/* "What!? Have you any idea what will happen if you set that evil loose */
	/* upon the world again?" */
	fdEventOpenMessage(ACTOR_CONSERVATO, 0);
	if(!fdEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
		fdEventWait(15);
		/* Yes: "You know that it could bring about the destruction of the..."
		 */
		fdEventShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
		FieldStateWork->msg = (FieldStateWork->msg) + 1;
	}
	else {
		fdEventWait(35);
		FieldStateWork->msg = (FieldStateWork->msg) + 1;
		/* No: "When the four beacons are fired, Alchemy will be released, and
		 */
		/* our..." */
		fdEventShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
	}
	fdActorFaceDirectionAndWait(ACTOR_CONSERVATO, FACING_WEST);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_CONSERVATO, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "My lord, you cannot possibly intend for these people to light the..." */
	fdEventShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_HYDROS, FACING_EAST);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_HYDROS, 3);
	fdEventWait(10);
	/* "I do indeed, Conservato, and I also fear the possibility that
	 * Alchemy..." */
	fdEventShowMessageThenWait(ACTOR_HYDROS, 0, 5);
	fdActorShowEmote(ACTOR_HYDROS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "But I cannot stand by, knowing as I do that the world will wither
	 * and..." */
	fdEventShowMessageThenWait(ACTOR_HYDROS, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_CONSERVATO, 4);
	fdActorSetSpeed(ACTOR_CONSERVATO, 0.5);
	fdActorGetById(ACTOR_CONSERVATO)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_CONSERVATO, 32, 0);
	fdEventWait(1);
	fdActorGetById(ACTOR_CONSERVATO)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_HYDROS, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_NORTHEAST);
	fdEventWait(5);
	fdActorRunRepeatedMotion(ACTOR_CONSERVATO, 2);
	fdEventWait(10);
	/* "This is madness, Hydros! Does the threat of Alchemy mean so little
	 * to..." */
	fdEventShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_CONSERVATO, 4);
	/* "With only two of the beacons lit, I assume you intend to send..." */
	fdEventShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_PIERS, 2);
	fdEventWait(10);
	/* "And I will go, Lord Conservato. Felix has need of my power!" */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_CONSERVATO, 3);
	fdEventWait(10);
	/* "Then go, Piers... But if you do, say farewell...for you will..." */
	fdEventShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorShowEmote(ACTOR_CONSERVATO, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "You know our laws. Are you prepared for the outcome?" */
	fdEventShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_CONSERVATO, 2);
	fdEventWait(5);
	/* "I myself am disgusted by the whole matter. I take my leave of you,..."
	 */
	fdEventShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
	fdActorSetSpeed(ACTOR_CONSERVATO, 1.2);
	fdActorWalkByAndWait(ACTOR_CONSERVATO, -50, 24);
	fdActorWatchAndSetScript(ACTOR_HYDROS, ACTOR_CONSERVATO);
	fdActorWatchAndSetScript(ACTOR_LUNPA, ACTOR_CONSERVATO);
	fdActorWatchAndSetScript(ACTOR_PIERS, ACTOR_CONSERVATO);
	fdActorWatchAndSetScript(ACTOR_FELIX, ACTOR_CONSERVATO);
	fdActorWatchAndSetScript(ACTOR_JENNA, ACTOR_CONSERVATO);
	fdActorWatchAndSetScript(ACTOR_SHEBA, ACTOR_CONSERVATO);
	fdActorWatchAndSetScript(ACTOR_KRADEN, ACTOR_CONSERVATO);
	fdCameraFollowActor(ACTOR_CONSERVATO, 1);
	fdActorWalkByAndWait(ACTOR_CONSERVATO, -8, 32);
	fdActorFaceDirection(ACTOR_PALACE_GUARD, FACING_SOUTH_SOUTHWEST, 0);
	fdActorStartRepeatedMotion(ACTOR_FELIX, 3);
	fdActorSetAttachedEffect(ACTOR_FELIX,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorStartRepeatedMotion(ACTOR_JENNA, 3);
	fdActorSetAttachedEffect(ACTOR_JENNA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(37);
	fdActorSetSpeed(ACTOR_FELIX, 2.2);
	fdActorSetSpeed(ACTOR_JENNA, 2.2);
	fdActorWalkBy(ACTOR_JENNA, -8, 16);
	fdActorWalkByAndWait(ACTOR_FELIX, 8, 16);
	fdActorWaitForMoveById(ACTOR_JENNA);
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_EAST);
	fdEventWait(15);
	fdActorWatchAndSetScript(ACTOR_FELIX, ACTOR_CONSERVATO);
	fdActorWatchAndSetScript(ACTOR_JENNA, ACTOR_CONSERVATO);
	fdActorWalkByAndWait(ACTOR_CONSERVATO, 0, 64);
	fdEventWait(15);
	fdActorFaceDirectionAndWait(ACTOR_CONSERVATO, FACING_NORTH_NORTHEAST);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_CONSERVATO, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
	/* "But I warn you... I will not permit..." */
	fdEventShowMessageThenWait(ACTOR_CONSERVATO, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_CONSERVATO, FACING_SOUTH_SOUTHWEST);
	fdEventWait(10);
	fdActorFaceDirectionAndWait(ACTOR_PALACE_GUARD, FACING_WEST);
	fdEventWait(15);
	fdActorFaceDirectionAndWait(ACTOR_PALACE_GUARD, FACING_SOUTH_SOUTHEAST);
	fdEventWait(15);
	fdActorSetAnimation(ACTOR_PALACE_GUARD, 5);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdActorWalkByAndWait(ACTOR_CONSERVATO, 0, 96);
	fdEventWait(20);
	fdActorSetPos(ACTOR_CONSERVATO, 0, 0);
	fdActorSetSpeed(ACTOR_FELIX, 1);
	fdActorSetSpeed(ACTOR_JENNA, 1);
	fdActorWalkBy(ACTOR_JENNA, 8, -16);
	fdActorWalkByAndWait(ACTOR_FELIX, -8, -16);
	fdActorWaitForMoveById(ACTOR_JENNA);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdCameraMoveTo(872, CAMERA_KEEP, 480, 1);
	fdCameraWaitForArrival();
	fdEventWait(10);
	fdActorStop(ACTOR_PALACE_GUARD);
	fdActorSetAnimation(ACTOR_PALACE_GUARD, 1);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
	fdEventWait(10);
	/* "What will you do, Your Highness?" */
	fdEventShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_HYDROS, 4);
	/* "I knew this outcome was inevitable." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0,
									5);
	fdActorSetAnimationAndWait(ACTOR_HYDROS, 4);
	/* "But I could not sit silent while our world drifted down the path of..."
	 */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0,
									5);
	fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_EAST);
	fdEventWait(40);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_HYDROS, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH);
	fdEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_HYDROS, 2);
	/* "I want you to light the remaining beacons before you return." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0,
									5);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	/* "That's what we intend to do, but we cannot leave the Eastern Sea..." */
	fdEventShowMessageThenWait(ACTOR_SHEBA, 0, 5);
	fdActorShowEmote(ACTOR_HYDROS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "I have already heard of this problem from Piers..." */
	fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_HYDROS, FACING_WEST);
	fdEventWait(20);
	fdActorFaceDirectionAndWait(ACTOR_LUNPA, FACING_EAST);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_LUNPA, 3);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST);
	fdEventWait(5);
	fdActorWalkByAndWait(ACTOR_LUNPA, 24, 26);
	fdActorWalkByAndWait(ACTOR_LUNPA, 0, 8);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_LUNPA, 2);
	fdEventWait(10);
	fdActorJump(ACTOR_LUNPA, 4, 20);
	/* What Lunpa brings out grows to its height over 16 frames. */
	for(growFrame = 0; growFrame < 16; growFrame++) {
		fdActorGetById(FIELD_TREASURE_ACTOR)->scaleY += FIXED_ONE / 16;
		sySynch(1);
	}
	fdActorGetById(FIELD_TREASURE_ACTOR)->scaleY = FIXED_ONE;
	fdEventWait(5);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorWalkByAndWait(ACTOR_LUNPA, -24, -26);
	fdActorWalkByAndWait(ACTOR_LUNPA, 0, -8);
	fdActorFaceDirectionAndWait(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_LUNPA, 2);
	fdEventWait(10);
	/* "This is something I found by accident in some ruins back in my days..."
	 */
	fdEventShowMessageThenWait(ACTOR_LUNPA, 0, 5);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_EAST);
	fdEventWait(30);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH);
	fdEventWait(15);
	fdActorRunRepeatedMotion(ACTOR_HYDROS, 2);
	fdEventWait(10);
	/* "I'm not sure what it is, but it seems to contain some powerful
	 * Psynergy." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0,
									5);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "Seems to? You are not certain?" */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_HYDROS, FACING_WEST);
	fdActorSetAnimationAndWait(ACTOR_HYDROS, 4);
	/* "It is a Psynergy that we Lemurians are unable to use." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0,
									5);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST);
	fdEventWait(5);
	/* "I understand you travel with many different kinds of Adepts, do you..."
	 */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0,
									5);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdEventWait(10);
	/* "If we include Piers in our numbers, we have one Adept for..." */
	fdEventShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_EAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_HYDROS, FACING_WEST);
	fdEventWait(30);
	fdActorSetAnimation(ACTOR_LUNPA, 3);
	fdActorSetAnimationAndWait(ACTOR_HYDROS, 3);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST);
	fdEventWait(10);
	/* "Then at least one of you should be able to use the Psynergy that
	 * this..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0,
									5);
	fdActorSetAnimationAndWait(ACTOR_LUNPA, 3);
	fdEventWait(10);
	/* "King Hydros believes the item can only be used by one who wields..." */
	fdEventShowMessageThenWait(ACTOR_LUNPA, 0, 5);
	fdActorShowEmote(ACTOR_HYDROS, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "If I am not mistaken, it is a powerful Psynergy we call Grind." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0,
									5);
	fdActorSetAnimationAndWait(ACTOR_HYDROS, 3);
	fdEventWait(10);
	/* "If you master this Psynergy, the reefs that block your way shall
	 * pose..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0,
									5);
	fdActorRunRepeatedMotion(ACTOR_LUNPA, 2);
	fdEventWait(10);
	/* "With Grind, you should be able to create a path by which you can
	 * sail..." */
	fdEventShowMessageThenWait(ACTOR_LUNPA, 0, 5);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_EAST);
	fdEventWait(30);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH);
	fdEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_HYDROS, 2);
	fdEventWait(10);
	/* "Go, brave Adepts! By your hands, may the remaining lighthouses burn..."
	 */
	fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HYDROS, 0, 10);
	fdActorFaceDirectionAndWait(ACTOR_HYDROS, FACING_NORTH_NORTHEAST);
	fdEventWait(5);
	fdActorWalkToAndWait(ACTOR_HYDROS, 864, 392);
	fdActorFaceDirectionAndWait(ACTOR_HYDROS, FACING_SOUTH_SOUTHEAST);
	fdActorRunRepeatedMotion(ACTOR_LUNPA, 2);
	fdEventWait(5);
	/* "That Psynergy is quite powerful. It can only be trusted to a very
	 * few..." */
	fdEventShowMessageThenWait(ACTOR_LUNPA, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_LUNPA, FACING_WEST);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_LUNPA, 3);
	fdEventWait(10);
	/* "That His Highness has given it to you should illustrate how great
	 * his..." */
	fdEventShowMessageThenWait(ACTOR_LUNPA, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_LUNPA, 2);
	fdEventWait(10);
	/* "I have no doubt that your journey will be fraught with ever-..." */
	fdEventShowMessageThenWait(ACTOR_LUNPA, 0, 5);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_EAST);
	fdEventWait(20);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	target = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(target != 0) {
		fdActorSetDestination(ACTOR_KRADEN, target->x.part[0].integer,
							  target->z.part[0].integer);
	}
	fdActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdActorSetSpeed(ACTOR_JENNA, 1.2);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_WALK);
	target = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(target != 0) {
		fdActorSetDestination(ACTOR_JENNA, target->x.part[0].integer,
							  target->z.part[0].integer);
	}
	fdActorWaitForMoveById(ACTOR_JENNA);
	fdActorSetPos(ACTOR_JENNA, 0, 0);
	fdActorSetSpeed(ACTOR_SHEBA, 1.2);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_WALK);
	target = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(target != 0) {
		fdActorSetDestination(ACTOR_SHEBA, target->x.part[0].integer,
							  target->z.part[0].integer);
	}
	fdActorWaitForMoveById(ACTOR_SHEBA);
	fdActorSetPos(ACTOR_SHEBA, 0, 0);
	fdEventWait(10);
	fdActorWalkByAndWait(ACTOR_FELIX, -32, -48);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_LUNPA, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	fdEventWait(10);
	fdActorSetSpeed(ACTOR_PIERS, 1.2);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_WALK);
	target = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(target != 0) {
		fdActorSetDestination(ACTOR_PIERS, target->x.part[0].integer,
							  target->z.part[0].integer);
	}
	fdActorWaitForMoveById(ACTOR_PIERS);
	fdActorSetPos(ACTOR_PIERS, 0, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_LUNPA, 3);
	fdEventWait(15);
	fdActorWalkToAndWait(ACTOR_LUNPA, 840, 408);
	fdActorFaceDirectionAndWait(ACTOR_LUNPA, FACING_SOUTH_SOUTHEAST);
	fdActorFaceDirection(ACTOR_PALACE_GUARD, FACING_SOUTH_SOUTHWEST, 0);
	fdEventWait(10);
	return fdEventEnd();
}

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 288, 0, 454, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 SceneExits[] = {
	SCENE_EXITS(LemuriaPalace),
	SCENE_EXIT_UNLESS(LEMURIA_PALACE_TO_LEMURIA_SOUTH, Lemuria,
					  LEMURIA_FROM_LEMURIA_PALACE_SOUTH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_HALL_WEST_DOOR, LemuriaPalace, WAY_DRESSER_ROOM_DOOR,
					  EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_HALL_EAST_DOOR, LemuriaPalace, WAY_BARRACKS_DOOR,
					  EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_HALL_WEST_STAIRS, LemuriaPalace,
					  WAY_THRONE_ROOM_SOUTH_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_HALL_EAST_STAIRS, LemuriaPalace,
					  WAY_THRONE_ROOM_SOUTH_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_DRESSER_ROOM_DOOR, LemuriaPalace, WAY_HALL_WEST_DOOR,
					  EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_BARRACKS_DOOR, LemuriaPalace, WAY_HALL_EAST_DOOR,
					  EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_THRONE_ROOM_SOUTH_WEST, LemuriaPalace,
					  WAY_HALL_WEST_STAIRS, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_THRONE_ROOM_SOUTH_EAST, LemuriaPalace,
					  WAY_HALL_EAST_STAIRS, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LEMURIA_PALACE_TO_LEMURIA_NORTHEAST, Lemuria,
					  LEMURIA_FROM_LEMURIA_PALACE_NORTHEAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_THRONE_ROOM_WEST_STAIRS, LemuriaPalace,
					  WAY_PAINTING_ROOM_WEST_STAIRS, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_THRONE_ROOM_EAST_STAIRS, LemuriaPalace,
					  WAY_PAINTING_ROOM_EAST_STAIRS, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_PAINTING_ROOM_WEST_STAIRS, LemuriaPalace,
					  WAY_THRONE_ROOM_WEST_STAIRS, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_PAINTING_ROOM_EAST_STAIRS, LemuriaPalace,
					  WAY_THRONE_ROOM_EAST_STAIRS, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_PAINTING_ROOM_PASSAGE, LemuriaPalace,
					  WAY_BEDROOM_DOOR, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_BEDROOM_DOOR, LemuriaPalace,
					  WAY_PAINTING_ROOM_PASSAGE, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LEMURIA_PALACE_TO_LEMURIA_NORTH, Lemuria,
					  LEMURIA_FROM_LEMURIA_PALACE_NORTH, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

/* clang-format off */
ACTOR_PLACE ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_GIRL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 912, 0, 624, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 520, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 456, FACING_EAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_IN_TEAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 360, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LEMURIAN_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 80, 0, 136, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_HYDROS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 864, 0, 392, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LUNPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 408, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CONSERVATO, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 888, 0, 392, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 888, 0, 560, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 424, FACING_NORTHEAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
/* clang-format on */

/* clang-format off */
/* How a palace door opens, as two frames of cells copied over the closed door. */
u16 LemuriaPalace_DoorOpening[] = {
    MAPGRID_ANIMATION_STEP(1, 52, 1, 2, 6),
    MAPGRID_ANIMATION_STEP(2, 52, 1, 2, 6),
    MAPGRID_ANIMATION_STEPS_END,
    /* Unused: the same for a door two cells wide. */
    MAPGRID_ANIMATION_STEP(2, 54, 2, 2, 6),
    MAPGRID_ANIMATION_STEP(4, 54, 2, 2, 6),
    MAPGRID_ANIMATION_STEPS_END,
};

/* The openings of the entrance hall's west and east doors, at their places on */
/* the map grid. */
MAP_GRID_CELL_ANIMATION LemuriaPalace_Doors[] = {
    {LemuriaPalace_DoorOpening, 5, 23},
    {LemuriaPalace_DoorOpening, 30, 23},
};

/* The opening of the bedroom's north door, at its place on the map grid. */
MAP_GRID_CELL_ANIMATION LemuriaPalace_BedroomDoor = {
    LemuriaPalace_DoorOpening, 56, 4};

TRIGGER SceneEvents[] = {
    {TRIGGER_EXIT, LEMURIA_PALACE_TO_LEMURIA_SOUTH, CONDITION_ALWAYS, LEMURIA_PALACE_TO_LEMURIA_SOUTH},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), WAY_HALL_WEST_DOOR, CONDITION_ALWAYS, EVENT(scLemuriaPalaceEnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), WAY_HALL_EAST_DOOR, CONDITION_ALWAYS, EVENT(scLemuriaPalaceEnterDoor)},
    {TRIGGER_EXIT, WAY_HALL_WEST_STAIRS, CONDITION_ALWAYS, WAY_HALL_WEST_STAIRS},
    {TRIGGER_EXIT, WAY_HALL_EAST_STAIRS, CONDITION_ALWAYS, WAY_HALL_EAST_STAIRS},
    {TRIGGER_EXIT, WAY_DRESSER_ROOM_DOOR, CONDITION_ALWAYS, WAY_DRESSER_ROOM_DOOR},
    {TRIGGER_EXIT, WAY_BARRACKS_DOOR, CONDITION_ALWAYS, WAY_BARRACKS_DOOR},
    {TRIGGER_EXIT, WAY_THRONE_ROOM_SOUTH_WEST, CONDITION_ALWAYS, WAY_THRONE_ROOM_SOUTH_WEST},
    {TRIGGER_EXIT, WAY_THRONE_ROOM_SOUTH_EAST, CONDITION_ALWAYS, WAY_THRONE_ROOM_SOUTH_EAST},
    {TRIGGER_EXIT, LEMURIA_PALACE_TO_LEMURIA_NORTHEAST, CONDITION_ALWAYS, LEMURIA_PALACE_TO_LEMURIA_NORTHEAST},
    {TRIGGER_EXIT, WAY_THRONE_ROOM_WEST_STAIRS, CONDITION_ALWAYS, WAY_THRONE_ROOM_WEST_STAIRS},
    {TRIGGER_EXIT, WAY_THRONE_ROOM_EAST_STAIRS, CONDITION_ALWAYS, WAY_THRONE_ROOM_EAST_STAIRS},
    {TRIGGER_EXIT, WAY_PAINTING_ROOM_WEST_STAIRS, CONDITION_ALWAYS, WAY_PAINTING_ROOM_WEST_STAIRS},
    {TRIGGER_EXIT, WAY_PAINTING_ROOM_EAST_STAIRS, CONDITION_ALWAYS, WAY_PAINTING_ROOM_EAST_STAIRS},
    {TRIGGER_EXIT, WAY_PAINTING_ROOM_PASSAGE, CONDITION_ALWAYS, WAY_PAINTING_ROOM_PASSAGE},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), WAY_BEDROOM_DOOR, CONDITION_ALWAYS, EVENT(scLemuriaPalaceEnterDoor)},
    {TRIGGER_EXIT, LEMURIA_PALACE_TO_LEMURIA_NORTH, CONDITION_ALWAYS, LEMURIA_PALACE_TO_LEMURIA_NORTH},
    {TRIGGER_STEP, CELL_BEHAVIOR_BEFORE_THRONE, FLAG_MET_KING_HYDROS, EVENT(scLemuriaPalaceMeetKingHydros)},
    {TRIGGER_TALK, ACTOR_GIRL_WAITING_ON_THE_KING, IF_FLAG_SET(FLAG_MET_KING_HYDROS), MSGNO(MsgLemuriaPalaceLordConservatoSeemedVeryAngry)},
    {TRIGGER_TALK, ACTOR_WOMAN_SHOWING_THE_WAY, IF_FLAG_SET(FLAG_MET_KING_HYDROS), MSGNO(MsgLemuriaPalaceBothLordConservatoAndKing)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_KINGS_MOOD, IF_FLAG_SET(FLAG_MET_KING_HYDROS), MSGNO(MsgLemuriaPalaceMaybeOnceYouHaveLeft)},
    {TRIGGER_TALK, ACTOR_LEMURIAN_WHO_HEARD_THE_YELLING, IF_FLAG_SET(FLAG_MET_KING_HYDROS), MSGNO(MsgLemuriaPalaceTodayHisHighnessAndLord)},
    {TRIGGER_TALK, ACTOR_LEMURIAN_PRAISING_HYDROS, IF_FLAG_SET(FLAG_MET_KING_HYDROS), MSGNO(MsgLemuriaPalaceIfHydrosPlansToUnleash)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_WAITING_ON_THE_KING, IF_FLAG_SET(FLAG_MET_KING_HYDROS), MSGNO(MsgLemuriaPalaceSenatesReportsUsuallyTend)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_SHOWING_THE_WAY, IF_FLAG_SET(FLAG_MET_KING_HYDROS), MSGNO(MsgLemuriaPalaceKingsLeadershipIsTooProgressive)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_KINGS_MOOD, IF_FLAG_SET(FLAG_MET_KING_HYDROS), MSGNO(MsgLemuriaPalaceWheneverOutsidersArriveInLemuria)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LEMURIAN_WHO_HEARD_THE_YELLING, IF_FLAG_SET(FLAG_MET_KING_HYDROS), MSGNO(MsgLemuriaPalaceWhatWillHappenToUs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LEMURIAN_PRAISING_HYDROS, IF_FLAG_SET(FLAG_MET_KING_HYDROS), MSGNO(MsgLemuriaPalaceAsLongAsICan)},
    {TRIGGER_TALK, ACTOR_GIRL_WAITING_ON_THE_KING, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceHeIsStillInChambers)},
    {TRIGGER_TALK, ACTOR_WOMAN_SHOWING_THE_WAY, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceIfYouSeekCounsel)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_KINGS_MOOD, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceHisHighnessNeverLeavesMeeting)},
    {TRIGGER_TALK, ACTOR_LEMURIAN_WHO_HEARD_THE_YELLING, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceConservatoAndKingWereYelling)},
    {TRIGGER_TALK, ACTOR_LEMURIAN_PRAISING_HYDROS, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceKingHydrosIsWisest)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_WAITING_ON_THE_KING, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceLordConservatoHoldsMostPower)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_SHOWING_THE_WAY, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceOhDearUsingPsynergy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_KINGS_MOOD, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceHisHighnessKingIsMost)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LEMURIAN_WHO_HEARD_THE_YELLING, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceLordConservatoOnlyRaisesHis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LEMURIAN_PRAISING_HYDROS, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceHisHighnessIsWorried)},
    {TRIGGER_TALK, ACTOR_HYDROS, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceDoNotListenToConservato)},
    {TRIGGER_TALK, ACTOR_LUNPA, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceYouMustSailToWestern)},
    {TRIGGER_TALK, ACTOR_PALACE_GUARD, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceIThinkIOverheardSomething)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HYDROS, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceMyPeopleMayNotDesire)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUNPA, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceHisMajestyHasCome)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PALACE_GUARD, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalaceActionsOfKingHydros)},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaRoomsTheresBookCalledGreatAdventures))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaRoomsIFoundLetterFromBabi))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaRoomsHeresBookCalledHistory))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaRoomsHeresBookCalledElementalLighthouses))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaRoomsHeresBookCalledGreatLemurian))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaRoomsHeresBookCalledAlchemy))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaRoomsHeresBookCalledAlchemyAndAncient))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaRoomsHeresBookCalledDiary))},
    {EXAMINE_TRIGGER(EXAMINE_WARDROBE), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaRoomsWhatInWhatsWithAll))},
    {TRIGGER_TABLE_END},
};

/* The glow's colours, in turn. */
s16 LemuriaPalace_GlowColors[] = {
    RGB(0, 24, 30), RGB(0, 22, 29), RGB(0, 20, 28), RGB(0, 18, 27),
    RGB(0, 16, 26), RGB(0, 14, 25), RGB(0, 12, 24), RGB(0, 14, 25),
    RGB(0, 16, 26), RGB(0, 18, 27), RGB(0, 20, 28), RGB(0, 22, 29),
};
/* clang-format on */

/* The blend weights the hall's two maps flicker between as one fades into */
/* the other. */
LATE_DATA ALIGNED(4) u16 LemuriaPalace_MapBlendSteps[] = {
	BLEND_ALPHA(2, 14), BLEND_ALPHA(4, 12), BLEND_ALPHA(6, 10),
	BLEND_ALPHA(8, 8),	BLEND_ALPHA(10, 6), BLEND_ALPHA(12, 4),
	BLEND_ALPHA(16, 0), BLEND_ALPHA(16, 0), BLEND_ALPHA(12, 4),
	BLEND_ALPHA(10, 6), BLEND_ALPHA(8, 8),	BLEND_ALPHA(6, 10),
	BLEND_ALPHA(4, 12), BLEND_ALPHA(2, 14), BLEND_ALPHA(2, 14),
	BLEND_ALPHA(4, 12), BLEND_ALPHA(6, 10), BLEND_ALPHA(8, 8),
	BLEND_ALPHA(10, 6), BLEND_ALPHA(12, 4), BLEND_ALPHA(16, 0),
	BLEND_ALPHA(16, 0), BLEND_ALPHA(12, 4), BLEND_ALPHA(10, 6),
	BLEND_ALPHA(8, 8),	BLEND_ALPHA(6, 10), BLEND_ALPHA(4, 12),
	BLEND_ALPHA(2, 14),
};

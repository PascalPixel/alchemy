/* @file */
/* The innermost rooms of the Gabomba Statue at Kibombo, three in this */
/* overlay: the small idol's chamber in the statue's head, the corridor with */
/* the lift, and the final chamber where the Great Gabomba speaks. */
/* */
/* In the small idol's chamber a groove runs from the idol at the north, down */
/* the statue's tongue between two pink drums. A ladder in the west alcove */
/* leads down to the room of pinions, and one in the east up to the room of */
/* the arrow panels behind the eyes. Once the ceremony's charges reach the */
/* statue it swallows the black orb, and the party comes in to find it gone */
/* (scGabombaStatueChamberWhereIsTheOrb): they remember it rolling up the */
/* groove into the hole under the idol, and Akafubu comes up the tongue behind
 */
/* them. When the party has told him where the orb went, he sets the Dragon's */
/* Eye in the idol, which slides east to open a passage north */
/* (scGabombaStatueChamberAkafubuOffersToHelp). The passage leads to a long */
/* corridor whose lift carries Felix down a shaft */
/* (scGabombaStatueChamberRideLiftDown) to the final chamber. There Akafubu */
/* waits before a seated idol with the orb in its hands */
/* (scGabombaStatueChamberAkafubuReachesChamber). Examining the idol, Felix */
/* hears the Great Gabomba name Akafubu witch doctor, give a message for him, */
/* open a hole east of the idol and give back the orb */
/* (scGabombaStatueChamberGabombaNamesWitchDoctor). On a later visit that */
/* hole's ladder leads down to the Gabomba Catacombs. */
#include "item_ids.h"

/* Set once Akafubu, in the final chamber, has said "I do not know..." */
#define FLAG_AKAFUBU_REACHED_CHAMBER 0x8fe

#include "field/field_sequence.h"
#include "game/game.h"
#include "graphics/palette_buffers.h"
#include "graphics/glyph.h"
#include "scene/scene.h"
#include "text/msg_ids.h"
#include "field/field_sprite.h"
#include "field/event_inline.h"

#include "field/field_sequence_event.h"

void scGabombaStatueChamberDropIntoHole(s32 x, s32 z);

extern ACTOR_PLACE GabombaStatueChamber_IdolActorPlacements[];
extern ACTOR_PLACE GabombaStatueChamber_WitchDoctorActorPlacements[];
extern ACTOR_PLACE GabombaStatueChamber_Placements[];
extern TRIGGER GabombaStatueChamber_IdolTriggers[];
extern TRIGGER GabombaStatueChamber_LiftTriggers[];
extern TRIGGER GabombaStatueChamber_WitchDoctorTriggers[];
extern TRIGGER GabombaStatueChamber_Triggers[];
extern SCENE_REGION GabombaStatueChamber_IdolExitRegions[];

#include "version.h"
#include "field/actor_script.h"
#include "window/window.h"

/* How far, in pixels, the lift goes down the shaft, a pixel a frame. The lift
 */
/* stays on screen and the map's layers move up past it. */
#define LIFT_TRAVEL 352
/* The frames over which the corridor around the lift fades into the shaft's */
/* brick walls, or back, a step every four frames. */
#define LIFT_BLEND_FRAMES 63
/* The lift's blend weight for its second target at the given step, rising */
/* one every four steps; the first target keeps the rest of BLEND_WEIGHT_MAX */
/* (BLEND_ALPHA's bottom and top). */
#define LIFT_BLEND_BOTTOM(step) (((step) + 1) / 4)
/* Going down, the corridor starts fading this many pixels into the ride. */
#define LIFT_DESCENT_BLEND_START 17
/* Coming back up, it has faded back in this many pixels before the top. */
#define LIFT_ASCENT_BLEND_END 33
/* The frames the Black Crystal rises over the idol, half a pixel a frame, as */
/* the Great Gabomba begins, and the frame Akafubu startles at it. */
#define CRYSTAL_RISE_FRAMES	   80
#define AKAFUBU_STARTLED_FRAME 20
/* The stone door halves in the final chamber's six alcoves, three to each, */
/* along the path up to the idol. */
#define STONE_DOOR_HALVES 18
/* Set once the lift has carried Felix, while he stays on it, so that it does */
/* not set off again until he steps off its south edge at the top. */
#define FLAG_LEADER_ON_LIFT FLAG_SCENE_TEMP_06
/* Set once Felix has ridden the lift down to the foot of the shaft. */
#define FLAG_LIFT_DOWN FLAG_SCENE_TEMP_07

/* The small idol's chamber (SceneId_GabombaStatueChamberIdol): the seated */
/* idol, and Kraden ("It vanished into that hole!") and Akafubu ("Who... Who */
/* are you!?"); Piers's, Sheba's and Jenna's rows take their own numbers. */
enum {
	ACTOR_SEATED_IDOL = ACTOR_FIRST_PLACED,
	/* The two pink drums either side of the groove where the tongue begins, */
	/* which turn as the black orb rolls between them. */
	ACTOR_LEFT_DRUM,
	ACTOR_RIGHT_DRUM,
	ACTOR_KRADEN,
	ACTOR_AKAFUBU,
	/* The black orb rolling up the groove into the hole, as the party */
	/* remembers it, drawn as the Black Crystal's item icon. */
	ACTOR_BLACK_CRYSTAL
};

/* The final chamber, where the Great Gabomba names the witch doctor: Kraden, */
/* Akafubu ("I have followed Oeia's instructions and entered the final */
/* chamber!"), the chest, and Akafubu again in the witch doctor's dress. */
enum {
	ACTOR_CHAMBER_KRADEN = ACTOR_FIRST_PLACED + 1,
	ACTOR_CHAMBER_AKAFUBU,
	ACTOR_CHAMBER_CHEST = ACTOR_FIRST_PLACED + 4,
	/* Unseen until the Great Gabomba's flash, when it turns white over the */
	/* floor where the second hole opens, hiding the change. Its "?" earlier in
	 */
	/* the scene is shown while it is still off the map, where nobody sees it.
	 */
	ACTOR_HOLE_COVER,
	ACTOR_WITCH_DOCTOR,
	ACTOR_FIRST_DOOR_HALF /* the first of the STONE_DOOR_HALVES */
};

/* The cell behaviors of the trigger tables. A hole's cells take the number */
/* of the exit its ladder leaves by, and a doorway's that of its exit. */
typedef enum GABOMBA_STATUE_CHAMBER_CELL_BEHAVIOR {
	/* The lift, at the north end of the corridor: cells (18, 11) to (22, 11).
	 */
	CELL_BEHAVIOR_LIFT = 3,
	/* The corridor just south of the lift: cells (19, 12) to (21, 12). */
	CELL_BEHAVIOR_SOUTH_OF_LIFT,
	/* The final chamber's path a few steps south of where Akafubu stands: */
	/* cells (23, 11) to (25, 11). */
	CELL_BEHAVIOR_BELOW_AKAFUBU = 10,
	/* The final chamber's seated idol, holding the black orb: cell (24, 5). */
	CELL_BEHAVIOR_IDOL
} GABOMBA_STATUE_CHAMBER_CELL_BEHAVIOR;

void scGabombaStatueChamberOpenSecondHole(void);

void scGabombaStatueChamberKeepItemWithIdol();
void scGabombaStatueChamberInitIdol(void);
void scGabombaStatueChamberInitLift(void);

void scGabombaStatueChamberInitWitchDoctor(void);

extern s32 GabombaStatueChamber_MirroredScript[];

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_GABOMBA_STATUE_CHAMBER_IDOL)
		return GabombaStatueChamber_IdolExitRegions;
	return 0;
}

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_GABOMBA_STATUE_CHAMBER_IDOL)
		return GabombaStatueChamber_IdolActorPlacements;
	if(scene == SCENE_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR)
		return GabombaStatueChamber_WitchDoctorActorPlacements;
	return GabombaStatueChamber_Placements;
}

const TRIGGER *scGetTriggers(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_GABOMBA_STATUE_CHAMBER_IDOL)
		return GabombaStatueChamber_IdolTriggers;
	if(scene == SCENE_GABOMBA_STATUE_CHAMBER_LIFT)
		return GabombaStatueChamber_LiftTriggers;
	if(scene == SCENE_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR)
		return GabombaStatueChamber_WitchDoctorTriggers;
	return GabombaStatueChamber_Triggers;
}

/* The Great Gabomba speaks a line through the seated idol: the idol glows */
/* gold, and its words appear in white across the middle of the screen, with */
/* no window. */
void scGabombaStatueChamberStatueSpeaks(void) {
	volatile u16 *color = (volatile u16 *)&CENTERED_TEXT_COLOR;
	FIELD_STATE *state;

	fdActorSetAnimationById(ACTOR_SEATED_IDOL, 2);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, 2);
	fdPaletteFadeSetTarget(PALETTE_TINT_WARM, 2);
	fdPaletteFadeStartInterpolation(20);
	sySynch(20);
	*color = RGB_WHITE;
	sySynch(1);
	state = Ram_HeapSlots->fieldState;
	mnFieldMessageShowCentered((s16)state->msg++, 0, 2);
	fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 2);
	fdPaletteFadeStartInterpolation(20);
	sySynch(20);
	fdActorSetAnimationById(ACTOR_SEATED_IDOL, 1);
}

/* The item shown on the idol, while it is there: the Dragon's Eye in the */
/* small idol's chamber, the black orb in the final chamber. */
extern ACTOR *GabombaStatueChamber_ShownItem;
extern u32 GabombaStatueChamber_SpinScript[];
extern s32 GabombaStatueChamber_ChestDescendsScript[];
extern u32 GabombaStatueChamber_ChestRisesScript[];
extern u32 GabombaStatueChamber_PartyLeavesScript[];

/* Felix examines the seated idol in the final chamber (cell (24, 5)), the */
/* black orb in its hands, and the Great Gabomba speaks: the room darkens, */
/* the orb rises over the idol, and the voice names Akafubu witch doctor. He */
/* changes into feathers and gown and hurries off south, too soon for the */
/* gift, a chest, that sinks before the idol. The Gabomba gives Felix its */
/* message for Akafubu, opens a hole with a ladder east of the idol in a flash
 */
/* of white, and gives him the orb. The screen goes dark, and Felix tells */
/* Akafubu the message in Kibombo by dawn. */
/* */
/* The Black Crystal is the item the party calls the black orb. The chest is */
/* the Gabomba's magic, kept back until Akafubu earns it; it rises away again */
/* as the voice says it will pass the magic on to no one yet. The second hole */
/* is scGabombaStatueChamberOpenSecondHole, drawn while `ACTOR_HOLE_COVER` */
/* whitens its cell. Kraden's two yes-or-no questions only choose his replies.
 */
void scGabombaStatueChamberGabombaNamesWitchDoctor(void) {
	ACTOR *actor, *leader;
	u32 i;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	sySndPlay(MUS_JUPITER_LIGHTHOUSE);
	sySndPlay(SE_POWER_UP);
	fdActorSetAnimationById(ACTOR_SEATED_IDOL, 3);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, 2);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, 2);
	fdPaletteFadeSetSource(PALETTE_TINT_NONE, 1);
	fdPaletteFadeSetTarget(PALETTE_TINT_BLEND | RGB(8, 8, 12), 1);
	fdPaletteFadeStartInterpolation(40);
	sySynch(40);
	sySndPlay(SE_GABOMBA_VOICE);
	sySynch(1);
	if(GabombaStatueChamber_ShownItem != 0) {
		for(i = 0; i < CRYSTAL_RISE_FRAMES; i++) {
			if(i == AKAFUBU_STARTLED_FRAME)
				fdActorShowEmote(ACTOR_CHAMBER_AKAFUBU,
								 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
			GabombaStatueChamber_ShownItem->y.fixed += toFIXED(1) / 2;
			sySynch(1);
		}
		GabombaStatueChamber_ShownItem->motionFlg = ACTOR_FLOATS;
	}
	fdActorSetAnimationById(ACTOR_SEATED_IDOL, 1);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
	fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 358, 108);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTHEAST,
						 0);
	fdActorPlaceOnActor(ACTOR_CHAMBER_KRADEN,
						MainRoutineStateWork.leaderActorId);
	sySynch(1);
	fdActorSetSpeed(ACTOR_CHAMBER_KRADEN, 0.8);
	fdActorWalkToAndWait(ACTOR_CHAMBER_KRADEN, 364, 124);
	fdActorFaceDirection(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHEAST, 0);
	fdEventSetMessageId(MsgOhYoungWitch);
	/* "Oh, young witch doctor... You have succeeded in your task. You have..."
	 */
	scGabombaStatueChamberStatueSpeaks();
	fdActorFaceDirectionById(ACTOR_CHAMBER_AKAFUBU, FACING_EAST, 20);
	fdActorFaceDirection(ACTOR_CHAMBER_AKAFUBU, FACING_WEST, 40);
	fdActorFaceDirectionById(ACTOR_CHAMBER_AKAFUBU, FACING_EAST, 20);
	/* "Wh-Who speaks?" */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAMBER_AKAFUBU, 0);
	fdActorSetAttachedEffect(ACTOR_CHAMBER_KRADEN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	/* "It sounded like it was the statue of the Great Gabomba." */
	fdEventShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
	fdActorFaceDirectionAndWait(ACTOR_CHAMBER_AKAFUBU, FACING_NORTH_NORTHWEST);
	fdActorJump(ACTOR_CHAMBER_AKAFUBU, 4, 20);
	/* "That's ridiculous..." */
	fdEventShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAMBER_AKAFUBU,
							  0);
	/* "Akafubu! Your magic has grown strong! At last, you are worthy of the..."
	 */
	scGabombaStatueChamberStatueSpeaks();
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHWEST, 40);
	/* "I give you the feathers and gown that are the badges of your office!" */
	scGabombaStatueChamberStatueSpeaks();
	fdActorShowEmote(MainRoutineStateWork.leaderActorId,
					 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_CHAMBER_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
					 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTHEAST,
						 0);
	fdActorFaceDirection(ACTOR_CHAMBER_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorSetAnimationById(ACTOR_SEATED_IDOL, 3);
	fdActorFaceDirectionById(ACTOR_CHAMBER_AKAFUBU, FACING_SOUTH_SOUTHEAST, 20);
	actor = fdActorGet(ACTOR_WITCH_DOCTOR);
	actor->facing = fdActorGet(ACTOR_CHAMBER_AKAFUBU)->facing;
	sySynch(1);
	fdActorPlaceOnActor(ACTOR_WITCH_DOCTOR, ACTOR_CHAMBER_AKAFUBU);
	fdActorSetPos(ACTOR_CHAMBER_AKAFUBU, 0, 0);
	sySndPlay(SE_CHIME);
	fdActorSetAnimationById(ACTOR_WITCH_DOCTOR, 5);
	fdEventWait(60);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_WITCH_DOCTOR, FACING_NORTH_NORTHWEST, 0);
	fdActorSetAnimationById(ACTOR_SEATED_IDOL, 1);
	fdEventWait(40);
	fdActorSetAnimationById(ACTOR_SEATED_IDOL, 3);
	sySndPlay(SE_CHIME);
	fdActorSetAnimationById(ACTOR_WITCH_DOCTOR, 6);
	fdEventWait(60);
	fdActorSetAnimationById(ACTOR_WITCH_DOCTOR, ANIM_STAND);
	fdEventWait(20);
	fdActorSetAnimationById(ACTOR_SEATED_IDOL, 1);
	fdEventWait(40);
	/* "Akafubu, from this day forward, you shall be Kibombo's witch doctor." */
	scGabombaStatueChamberStatueSpeaks();
	fdActorJump(ACTOR_WITCH_DOCTOR, 2, 10);
	fdActorJump(ACTOR_WITCH_DOCTOR, 4, 10);
	/* "I've done it! I've finally earned the right to be witch doctor!" */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_WITCH_DOCTOR, 0);
	fdActorSetMovementTypeAndWaitById(ACTOR_WITCH_DOCTOR,
									  GabombaStatueChamber_SpinScript);
	/* "At last!" */
	fdEventShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_WITCH_DOCTOR, 0);
	fdActorSetSpeed(ACTOR_WITCH_DOCTOR, 1.6);
	fdActorWalkToAndWait(ACTOR_WITCH_DOCTOR, 392, 164);
	fdActorShowEmoteById(ACTOR_WITCH_DOCTOR, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
	fdActorJump(ACTOR_WITCH_DOCTOR, 6, 20);
	/* "Yes!" */
	fdEventShowMessageAndWait(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_WITCH_DOCTOR, 0);
	fdActorWalkToAndWait(ACTOR_WITCH_DOCTOR, 392, 220);
	fdActorSetPos(ACTOR_WITCH_DOCTOR, 0, 0);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_EAST,
							 0);
	fdActorFaceDirectionById(ACTOR_CHAMBER_KRADEN, FACING_EAST, 0);
	fdActorGet(ACTOR_CHAMBER_CHEST)->motionFlg = 0;
	fdActorSetMovementTypeAndWaitById(ACTOR_CHAMBER_CHEST,
									  GabombaStatueChamber_ChestDescendsScript);
	fdEventWait(20);
	/* "And there is one thing that remains for me to give you..." */
	scGabombaStatueChamberStatueSpeaks();
	fdActorShowEmote(MainRoutineStateWork.leaderActorId,
					 EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_CHAMBER_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTHEAST,
						 0);
	fdActorFaceDirection(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHEAST, 80);
	/* "Akafubu... You hasty fool... I had something else to give you, and..."
	 */
	scGabombaStatueChamberStatueSpeaks();
	fdEventWait(20);
	/* "Say, you there..." */
	scGabombaStatueChamberStatueSpeaks();
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTHEAST,
						 20);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_WEST,
							 40);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTHEAST,
						 20);
	fdActorSetAnimationById(ACTOR_SEATED_IDOL, 3);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, 2);
	fdPaletteFadeSetTarget(PALETTE_TINT_WARM, 2);
	{
		volatile u16 *color = (volatile u16 *)&CENTERED_TEXT_COLOR;

		fdPaletteFadeStartInterpolation(10);
		sySynch(10);
		*color = RGB_WHITE;
		sySynch(1);
	}
	/* "Yes! You! You, looking around there!" */
	mnFieldMessageShowCentered((s16)(FieldStateWork->msg)++, 0, 2);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, 2);
	fdPaletteFadeStartInterpolation(20);
	sySynch(20);
	fdActorSetAnimationById(ACTOR_SEATED_IDOL, 1);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTHEAST,
						 0);
	fdActorShowEmoteById(MainRoutineStateWork.leaderActorId,
						 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHWEST, 20);
	/* "I have magic for Akafubu, magic that he will soon need, ...but he is..."
	 */
	scGabombaStatueChamberStatueSpeaks();
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTHEAST,
						 0);
	fdActorFaceDirection(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHEAST, 0);
	/* "As a result, I have decided that I shall not pass this magic on to..."
	 */
	scGabombaStatueChamberStatueSpeaks();
	fdActorShowEmote(MainRoutineStateWork.leaderActorId,
					 EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_CHAMBER_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "Tell this to Akafubu: If he desires this new magic, then he must
	 * earn..." */
	scGabombaStatueChamberStatueSpeaks();
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_EAST,
							 0);
	fdActorFaceDirectionById(ACTOR_CHAMBER_KRADEN, FACING_EAST, 0);
	fdActorSetMovementTypeAndWaitById(ACTOR_CHAMBER_CHEST,
									  GabombaStatueChamber_ChestRisesScript);
	/* "If Akafubu does not refine his magical arts, then he will never earn..."
	 */
	scGabombaStatueChamberStatueSpeaks();
	fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_CHAMBER_KRADEN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTHEAST,
						 0);
	fdActorFaceDirection(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHEAST, 20);
	/* "This power shall not be realized until he ceases living only for..." */
	scGabombaStatueChamberStatueSpeaks();
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHWEST, 20);
	/* "Tell him so!" */
	scGabombaStatueChamberStatueSpeaks();
	fdActorFaceDirectionAndWait(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHEAST);
	/* "I don't suppose you could just...give that magic to us?" */
	fdEventShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTHEAST,
						 0);
	fdActorShowEmoteById(ACTOR_HOLE_COVER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "What do you mean?" */
	scGabombaStatueChamberStatueSpeaks();
	/* "I'm just asking if maybe we could earn that magic before Akafubu." */
	fdEventShowMessageThenWait(ACTOR_CHAMBER_KRADEN, 0, 40);
	/* "The power can be claimed by another, but only if Akafubu lacks the..."
	 */
	scGabombaStatueChamberStatueSpeaks();
	fdActorJump(ACTOR_CHAMBER_KRADEN, 4, 10);
	/* "Aha! So we CAN get it!" */
	fdEventShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
	/* "Perhaps if he must compete for his power, he will focus and take his..."
	 */
	scGabombaStatueChamberStatueSpeaks();
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_SOUTH,
							 0);
	fdActorFaceDirectionAndWait(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHWEST);
	fdActorSetAnimationAndWait(ACTOR_CHAMBER_KRADEN, ANIM_NOD);
	/* "But before you do this thing, you must convey to Akafubu all that I..."
	 */
	scGabombaStatueChamberStatueSpeaks();
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId,
							 FACING_NORTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHEAST);
	fdActorSetAnimationById(ACTOR_CHAMBER_KRADEN, ANIM_SHAKE_HEAD);
	/* "Oh, of course! We wouldn't dream of cheating him... We will tell him..."
	 */
	fdEventShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
	/* "Follow that path, and you will find a cavern..." */
	scGabombaStatueChamberStatueSpeaks();
	sySndPlay(MUS_SILENCE);
	sySndPlay(SE_SHIMMER);
	fdActorSetAnimationById(ACTOR_SEATED_IDOL, 3);
	fdActorSetSprCol(fdActorGetById(ACTOR_HOLE_COVER), SPRITE_COLOR_WHITE);
	fdActorSetPos(ACTOR_HOLE_COVER, 424, 104);
	fdPaletteFadeSetTarget(RGB_WHITE, 0);
	fdPaletteFadeStartInterpolation(40);
	sySynch(40);
	scGabombaStatueChamberOpenSecondHole();
	fdActorSetPos(ACTOR_HOLE_COVER, 0, 0);
	fdEventWait(1);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, 2);
	fdPaletteFadeSetTarget(PALETTE_TINT_BLEND | RGB(8, 8, 12), 1);
	fdPaletteFadeStartInterpolation(60);
	sySynch(60);
	/* "But now, I shall sleep...until the time comes when I must anoint a..."
	 */
	scGabombaStatueChamberStatueSpeaks();
	fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 1);
	fdPaletteFadeStartInterpolation(20);
	sySynch(20);
	GabombaStatueChamber_ShownItem->motionFlg =
		ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdEventWait(10);
	sySndPlay(SE_STAR_BOUNCE);
	fdEventWait(40);
	sySndPlay(MUS_INSIDE_THE_GABOMBA_STATUE);
	fdEventWait(40);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId,
							 FACING_SOUTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHWEST);
	/* "Felix... Go get Piers's precious orb." */
	fdEventShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorFaceDirection(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHEAST, 0);
	fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 392, 104);
	fdActorFaceDirectionAndRefreshById(MainRoutineStateWork.leaderActorId,
									   FACING_NORTH);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId,
							ANIM_LEADER_ARMS_RAISED);
	if(GabombaStatueChamber_ShownItem != 0) {
		leader = fdActorGet(MainRoutineStateWork.leaderActorId);
		actor = GabombaStatueChamber_ShownItem;
		actor->motionFlg = 0;
		fdActorSetMoveTarget((s32)actor, leader->x.fixed,
							 leader->y.fixed + toFIXED(36), leader->z.fixed);
		fdActorWaitForMove((s32)GabombaStatueChamber_ShownItem);
	}
	fdPartyInventoryGiveItem(ITEM_BLACK_CRYSTAL, 0);
	if(GabombaStatueChamber_ShownItem != 0)
		fdActorRelease((s32)GabombaStatueChamber_ShownItem);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_STAND);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId,
							 FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHEAST, 40);
	/* "Felix... Do you have any idea exactly what this \"Great Gabomba\" really
	 */
	/* is?" Yes: "If you have unraveled that which I cannot, then perhaps..."
	 * No: */
	/* "You do not understand either? Then I do not feel quite so foolish..." */
	fdMessageShowAtActor(ACTOR_CHAMBER_KRADEN, 0);
	if(fdEventAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId,
										 0) == 0) {
		fdActorSetAnimationById(ACTOR_CHAMBER_KRADEN, ANIM_SHAKE_HEAD);
		/* "If you have unraveled that which I cannot, then perhaps you are
		 * the..." */
		fdEventShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
		FieldStateWork->msg += 1;
	}
	else {
		FieldStateWork->msg += 1;
		fdActorSetAnimationById(ACTOR_CHAMBER_KRADEN, ANIM_NOD);
		/* "You do not understand either? Then I do not feel quite so
		 * foolish..." */
		fdEventShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
	}
	fdActorWalkToAndWaitById(ACTOR_CHAMBER_KRADEN, 376, 124);
	fdEventWait(20);
	/* "But doesn't this witch doctor magic seem not unlike some kind of..." */
	fdEventShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
	fdActorFaceDirectionAndWait(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHEAST);
	/* "Wouldn't it be quite fortunate if we were to get our hands on that */
	/* power ourselves?" Yes: "Oh! You didn't need much convincing at all, did
	 */
	/* you, Felix?" No: "Hrmm... You would feel guilty about taking it, eh,..."
	 */
	fdMessageShowAtActor(ACTOR_CHAMBER_KRADEN, 0);
	if(fdEventAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId,
										 0) == 0) {
		/* "Oh! You didn't need much convincing at all, did you, Felix?" */
		fdEventShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
		FieldStateWork->msg += 1;
	}
	else {
		FieldStateWork->msg += 1;
		/* "Hrmm... You would feel guilty about taking it, eh, Felix?" */
		fdEventShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
	}
	fdActorSetAnimationById(ACTOR_CHAMBER_KRADEN, ANIM_SHAKE_HEAD);
	/* "I needn't remind you, but... We're going to need every kind of..." */
	fdEventShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	/* "Either way, Felix, we are, at the very least, obligated to deliver..."
	 */
	fdEventShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
	fdActorSetAnimationAndWait(ACTOR_CHAMBER_KRADEN, ANIM_NOD);
	/* "We must fulfill our promise to the Gabomba." */
	fdEventShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
	/* "Then, maybe we can come back inside this Gabomba statue some other..."
	 */
	fdEventShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetMovementTypeById(ACTOR_CHAMBER_KRADEN,
							   GabombaStatueChamber_PartyLeavesScript);
	fdEventWait(20);
	fdActorSetMovementTypeById(MainRoutineStateWork.leaderActorId,
							   GabombaStatueChamber_PartyLeavesScript);
	fdEventWait(40);
	fdPaletteFadeSetTarget(0, 0);
	fdPaletteFadeStartInterpolation(60);
	sySynch(64);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	FieldStateWork->transitionFrm = 10;
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
	FieldStateWork->transitionFrm = 1;
	fdPaletteFadeStartInterpolation(1);
	fdActorSystemSuspendProjectionAndShowSprs();
	mnFieldMessageShowCentered(MsgCarriedTheMessage, 0, 3);
	fdActorSystemResumeProjectionAndHideLayers();
	syGameFlagSet(FLAG_GABOMBA_MESSAGE_CARRIED);
	syGameFlagClear(FLAG_KIBOMBO_AT_CEREMONY);
	syGameFlagClear(FLAG_GABOMBA_CEREMONY_UNDER_WAY);
	MainRoutineStateWork.retreatScene = SCENE_KIBOMBO;
	MainRoutineStateWork.retreatEntrance = KIBOMBO_FROM_WORLD_MAP;
	MainRoutineStateWork.restartScene = SCENE_KIBOMBO_HOUSES;
	MainRoutineStateWork.restartEntrance =
		KIBOMBO_HOUSES_FROM_KIBOMBO_NORTHEAST;
	syImportEventSetExitRequest(
		GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR_TO_KIBOMBO_HOUSES);
}

/* Felix walks to the hole at (x, z), in pixels, turns north and climbs down */
/* its ladder out of sight, and the screen goes dark. He leaves by the exit */
/* whose number the hole's cells hold. */
void scGabombaStatueChamberDropIntoHole(s32 x, s32 z) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR *actor = fdActorGetById(MainRoutineStateWork.leaderActorId);

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	actor->motionFlg = 0;
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1);
	fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, x, z);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_NORTH,
							 0);
	fdActorWaitForScriptEndById(MainRoutineStateWork.leaderActorId);
	fdActorSetSprFlags(fdActorGetById(MainRoutineStateWork.leaderActorId), 0);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId,
							ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
	scFieldActorSetMoveTarget(actor, x, -12, z);
	fdActorWaitForMove(actor);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId,
							ANIM_LEADER_CLIMB);
	sySndPlay(SE_MAP_EXIT);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(state->stepTrigger);
}

/* Felix walks north into the hole in the small idol's chamber, in the narrow */
/* alcove west of the idol (cell (19, 10)), and climbs down its ladder to the */
/* room of pinions below. */
void scGabombaStatueChamberStepIntoFirstHole(void) {
	scGabombaStatueChamberDropIntoHole(312, 144);
}

/* Felix walks into the hole the Great Gabomba opened east of the final */
/* chamber's idol (cell (26, 6)) and climbs down its ladder to the Gabomba */
/* Catacombs. The four cells beside the hole answer only while he presses */
/* toward it, so that he can walk round it without falling in. */
void scGabombaStatueChamberStepIntoSecondHole(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	s32 x = FIELD_CELL(leader->x.fixed);
	s32 z = FIELD_CELL(leader->z.fixed);

	if(x == 26) {
		if(z == 5 && (InputWork.held & KEY_DOWN) == 0)
			return;
		if(z == 7 && (InputWork.held & KEY_UP) == 0)
			return;
	}
	else if(z == 6) {
		if(x == 25 && (InputWork.held & KEY_RIGHT) == 0)
			return;
		if(x == 27 && (InputWork.held & KEY_LEFT) == 0)
			return;
	}
	scGabombaStatueChamberDropIntoHole(424, 100);
}

/* Coming back from the final chamber, Felix arrives at the foot of the lift */
/* shaft, steps south onto the lift, and it carries him up to the top, the */
/* brick walls sliding down past it until the corridor fades back in around */
/* him. It stops at the north end of the corridor (cell (20, 11)) that leads */
/* south to the small idol's chamber. */
/* */
/* The ride is the descent played backwards: the map's layers start */
/* `LIFT_TRAVEL` pixels on and come back a pixel a frame. */
void scGabombaStatueChamberRideLiftBackUp(void) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	MAP_LAYER *lower = &map->layers[0];
	MAP_LAYER *upper = &map->layers[2];
	s32 i, step;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdBlendScriptSuspend();
	step = 1;
	for(i = 0; i < LIFT_TRAVEL; i++) {
		if(i >= LIFT_DESCENT_BLEND_START &&
		   i < LIFT_DESCENT_BLEND_START + LIFT_BLEND_FRAMES) {
			s32 blend = LIFT_BLEND_BOTTOM(step) << 8;

			blend |= BLEND_WEIGHT_MAX - LIFT_BLEND_BOTTOM(step);
			REG_BLDALPHA = blend;
			step++;
		}
		lower->ofsY += toFIXED(1);
		upper->ofsY += toFIXED(1);
	}
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
	fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 328, 184);
	fdEventWait(20);
	sySndPlay(SE_MATERIALIZE);
	fdActorSetSprPriority(MainRoutineStateWork.leaderActorId, 2);
	for(; i != 0; i--) {
		if(i >= LIFT_ASCENT_BLEND_END &&
		   i < LIFT_ASCENT_BLEND_END + LIFT_BLEND_FRAMES) {
			s32 blend = LIFT_BLEND_BOTTOM(step) << 8;

			blend |= BLEND_WEIGHT_MAX - LIFT_BLEND_BOTTOM(step);
			REG_BLDALPHA = blend;
			step--;
		}
		lower->ofsY += -toFIXED(1);
		upper->ofsY += -toFIXED(1);
		sySynch(1);
	}
	fdActorSetSprPriority(MainRoutineStateWork.leaderActorId, 1);
	sySndPlay(SE_STOP_EXIT_SE);
	sySynch(1);
	sySndPlay(SE_CHEST_CLOSE);
	fdEventWait(20);
	syGameFlagSet(FLAG_LEADER_ON_LIFT);
	fdEventEnd();
}

/* Felix steps onto the lift at the north end of the corridor from the small */
/* idol's chamber (cells (18, 11) to (22, 11)), turns to face south, and it */
/* carries him down the shaft: the corridor fades into brick walls that slide */
/* up past him until the lift stops at the foot, where the way on leads north */
/* to the final chamber. */
/* */
/* The lift never moves on screen; both map layers scroll `LIFT_TRAVEL` */
/* pixels, and Felix is put on the lower floor's layer at the end. */
void scGabombaStatueChamberRideLiftDown(void) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	MAP_LAYER *lower = &map->layers[0];
	MAP_LAYER *upper = &map->layers[2];
	s32 i, step;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdBlendScriptSuspend();
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
	fdActorSetDestinationOffsetById(MainRoutineStateWork.leaderActorId, 0, -8);
	fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
	fdActorFaceDirectionAndRefreshById(MainRoutineStateWork.leaderActorId,
									   FACING_SOUTH);
	sySndPlay(SE_MATERIALIZE);
	step = 1;
	fdActorSetSprPriority(MainRoutineStateWork.leaderActorId, 2);
	for(i = 0; i <= LIFT_TRAVEL - 1; i++) {
		if((u32)(i - LIFT_DESCENT_BLEND_START) <= LIFT_BLEND_FRAMES - 1) {
			s32 blend = LIFT_BLEND_BOTTOM(step) << 8;

			blend |= BLEND_WEIGHT_MAX - LIFT_BLEND_BOTTOM(step);
			REG_BLDALPHA = blend;
			step++;
		}
		lower->ofsY += toFIXED(1);
		upper->ofsY += toFIXED(1);
		sySynch(1);
	}
	fdActorSetSprPriority(MainRoutineStateWork.leaderActorId, 1);
	sySndPlay(SE_STOP_EXIT_SE);
	sySynch(1);
	sySndPlay(SE_CHEST_CLOSE);
	fdEventWait(20);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->layer = 1;
	syGameFlagSet(FLAG_LEADER_ON_LIFT);
	syGameFlagSet(FLAG_LIFT_DOWN);
	fdEventEnd();
}

/* Nothing visible happens when Felix steps off the lift at the top onto the */
/* corridor south of it (cells (19, 12) to (21, 12)). The game notes that he */
/* has left it, so that stepping back on carries him down again. */
void scGabombaStatueChamberStepOffLift(void) {
	syGameFlagClear(FLAG_LEADER_ON_LIFT);
}

/* As Felix walks up the path of the final chamber towards the idol (cells */
/* (23, 11) to (25, 11)), the view moves up to Akafubu standing before it, */
/* who says he has followed Oeia's instructions to this chamber but does not */
/* know what he must do here to become witch doctor. It plays once. */
void scGabombaStatueChamberAkafubuReachesChamber(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdCameraMoveTo(392, CAMERA_KEEP, 120, 1);
	fdCameraWaitForArrival();
	fdEventWait(20);
	fdEventSetMessageId((s32)MsgIHaveFollowed);
	fdActorSetAttachedEffect(ACTOR_CHAMBER_AKAFUBU,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	/* "I have followed Oeia's instructions and entered the final chamber!" */
	fdEventShowMessageAndWait(ACTOR_CHAMBER_AKAFUBU, 0);
	fdActorFaceDirectionAndWait(ACTOR_CHAMBER_AKAFUBU, FACING_WEST);
	/* "He told me that if I reached this chamber, I would become Kibombo's..."
	 */
	fdEventShowMessageAndWait(ACTOR_CHAMBER_AKAFUBU, 0);
	fdActorFaceDirectionAndRefreshById(ACTOR_CHAMBER_AKAFUBU, FACING_EAST);
	/* "But what must I do here to become a witch doctor?" */
	fdEventShowMessageAndWait(ACTOR_CHAMBER_AKAFUBU, 0);
	fdActorFaceDirectionAndWait(ACTOR_CHAMBER_AKAFUBU, FACING_NORTH_NORTHEAST);
	fdActorSetAnimationAndWait(ACTOR_CHAMBER_AKAFUBU, ANIM_SHAKE_HEAD);
	/* "I do not know..." */
	fdEventShowMessageAndWait(ACTOR_CHAMBER_AKAFUBU, 0);
	syGameFlagSet(FLAG_AKAFUBU_REACHED_CHAMBER);
	fdEventEnd();
}

/* Each frame, keeps the item shown on the idol with the idol, so that the */
/* Dragon's Eye slides aside with it. */
void scGabombaStatueChamberKeepItemWithIdol(ACTOR *actor) {
	actor->x.fixed = fdActorGetById(ACTOR_SEATED_IDOL)->x.fixed;
}

/* Shows the Dragon's Eye, its red item icon, set in the front of the small */
/* idol, where Akafubu places it. */
void scGabombaStatueChamberShowDragonsEye(void) {
	ACTOR *idol = fdActorGetById(ACTOR_SEATED_IDOL);
	s32 x = idol->x.fixed;
	s32 z = idol->z.fixed;
	ACTOR *item;
	SPR *sprite;
	u8 *icon;
	u8 clear = 0;

	GabombaStatueChamber_ShownItem = item =
		syImportActorCreate(SPRITE_UNSEEN_MARKER, x, toFIXED(18), z);
	if(item != NULL) {
		sprite = item->spr;
		sprite->image.fullCol = 0;
		sprite->image.pal = 0;
		sprite->image.pri = 2;
		item->motionFlg = clear;
		item->retainSprCache = ACTOR_KEEP_SPRITE_VRAM;
		item->update = scGabombaStatueChamberKeepItemWithIdol;
		sprite->flg = clear;
		sprite->cnt = clear;
		icon = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
		mnIconDrawItem(ITEM_DRAGONS_EYE);
		syVramCacheLoad(sprite->slot, ICON_TILE_BYTES,
						((ICON_TRANSFER *)icon)->tiles);
		syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	}
}

/* Shows the black orb, the Black Crystal's item icon, resting in the hands of
 */
/* the final chamber's idol. */
void scGabombaStatueChamberShowBlackCrystal(void) {
	ACTOR *idol = fdActorGetById(ACTOR_SEATED_IDOL);
	s32 x = idol->x.fixed;
	s32 z = idol->z.fixed;
	ACTOR *item;
	SPR *sprite;
	u8 *icon;
	u8 clear = 0;

	GabombaStatueChamber_ShownItem = item =
		syImportActorCreate(SPRITE_UNSEEN_MARKER, x, toFIXED(18), z);
	if(item != NULL) {
		sprite = item->spr;
		sprite->image.fullCol = 0;
		sprite->image.pal = 0;
		sprite->image.pri = 2;
		item->motionFlg = clear;
		item->retainSprCache = ACTOR_KEEP_SPRITE_VRAM;
		item->update = scGabombaStatueChamberKeepItemWithIdol;
		sprite->flg = clear;
		sprite->cnt = clear;
		icon = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
		mnIconDrawItem(ITEM_BLACK_CRYSTAL);
		syVramCacheLoad(sprite->slot, ICON_TILE_BYTES,
						((ICON_TRANSFER *)icon)->tiles);
		syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	}
}

/* Unrolls the statue's tongue in the map: the orange roll between the two */
/* pink drums becomes a tongue running south out of the small idol's chamber, */
/* its end (cells (23, 21) to (25, 21)) the way out to the statue grounds. */
void scGabombaStatueChamberUnrollTongue(void) {
	fdMapGridCopyCellTilesIndicesRect(85, 78, 22, 79, 5, 11);
	fdMapGridCopyCellAttributes(85, 78, 5, 11, 22, 15);
	fdMapGridCopyCellsTo(86, 30, 86, 20, 5, 2);
	fdMapGridCopyCellTilesIndicesRect(86, 30, 84, 22, 9, 2);
}

/* Moves the small idol's place in the map with it as it slides east: its old */
/* cell (24, 9), in front of the passage north, is freed and cell (26, 9) */
/* blocked. */
void scGabombaStatueChamberUncoverPassage(void) {
	fdMapCopyCellAttributeRect(23, 1, 3, 1, 23, 9);
	fdMapCopyCellAttributeRect(23, 0, 3, 1, 25, 9);
}

/* Opens the hole east of the final chamber's idol in the map, its ladder the */
/* way down to the Gabomba Catacombs: the hole at cell (26, 6) and the four */
/* cells around it that lead in. */
void scGabombaStatueChamberOpenSecondHole(void) {
	fdMapGridCopyCellTilesIndicesRect(0, 1, 26, 6, 1, 1);
	fdMapGridCopyCellTilesIndicesRect(0, 65, 26, 70, 1, 1);
	fdMapGridCopyCellAttributes(0, 2, 3, 3, 25, 5);
}

extern u32 GabombaStatueChamber_WalkOffScript[];

/* The party comes into the small idol's chamber after the statue has */
/* swallowed the black orb, and Piers asks where it is. In sepia, the view */
/* follows the orb as they remember it rolling up the groove between the pink */
/* drums and vanishing into the hole under the idol. Sheba sees light beyond */
/* the hole, Akafubu comes up the tongue demanding who they are, and Jenna */
/* leads him off east with the party to the room of the arrow panels. */
/* */
/* It plays on arriving from the ceremony by */
/* `GABOMBA_STATUE_CHAMBER_IDOL_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH`. */
/* The question "Are you Kibombo!?" only chooses Akafubu's reply. */
void scGabombaStatueChamberWhereIsTheOrb(void) {
	ACTOR *crystal = fdActorGetById(ACTOR_BLACK_CRYSTAL);
	FIELD_SPR *sprite;
	u8 *block;
	s32 westSideFacing, eastSideFacing;
	s32 leader;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	scFieldActorSetPosAndResetMotion(fdActorGetById(ACTOR_BLACK_CRYSTAL),
									 toFIXED(392), 0, toFIXED(268));
	sySynch(1);
	leader = MainRoutineStateWork.leaderActorId;
	fdActorSetPosAndFacing(leader, toFIXED(360), toFIXED(184),
						   westSideFacing = FACING_NORTHEAST);
	fdActorSetPosAndFacing(ACTOR_PIERS, toFIXED(376), toFIXED(184),
						   westSideFacing);
	fdActorSetPosAndFacing(ACTOR_KRADEN, toFIXED(360), toFIXED(168),
						   westSideFacing);
	fdActorSetPosAndFacing(ACTOR_SHEBA, toFIXED(423), toFIXED(168),
						   eastSideFacing = FACING_NORTHWEST);
	fdActorSetPosAndFacing(ACTOR_JENNA, toFIXED(408), toFIXED(184),
						   eastSideFacing);
	if(crystal != 0) {
		sprite = crystal->spr;
		sprite->flg = 0;
		sprite->cnt = 0;
		sprite->fullCol = 0;
		sprite->pal = 0;
		sprite->pri = 1;
		crystal->retainSprCache = ACTOR_KEEP_SPRITE_VRAM;
		block = (u8 *)syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER,
									  sizeof(ICON_TRANSFER));
		mnIconDrawItem(ITEM_BLACK_CRYSTAL);
		syVramCacheLoad(sprite->slot, ICON_TILE_BYTES,
						(s32)((ICON_TRANSFER *)block)->tiles);
		syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	}
	scGabombaStatueChamberUnrollTongue();
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(40);
	fdActorJump(ACTOR_PIERS, 4, 10);
	fdEventSetMessageId(MsgTheBlackOrb);
	/* "The black orb... Where is it?" */
	fdEventShowMessageAndWait(ACTOR_PIERS, 0);
	fdActorSetAttachedEffect(ACTOR_SHEBA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
	/* "I saw it roll in there..." */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
	fdActorShowEmoteById(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "It came in through there and rolled along this groove in the floor..."
	 */
	fdEventShowMessageAndWait(ACTOR_JENNA, 0);
	scFieldActorSetPosAndResetMotion(fdActorGetById(ACTOR_BLACK_CRYSTAL),
									 toFIXED(392), 0, toFIXED(340));
	sySndPlay(SOUND_FADE_OUT_BGM);
	fdPaletteFadeSetTarget(PALETTE_TINT_SEPIA, 0);
	fdPaletteFadeStartInterpolation(40);
	sySynch(40);
	fdActorSetAnimationById(ACTOR_LEFT_DRUM, 2);
	fdActorSetAnimationById(ACTOR_RIGHT_DRUM, 2);
	fdActorWatchAndSetScript(MainRoutineStateWork.leaderActorId,
							 ACTOR_BLACK_CRYSTAL);
	fdActorWatchAndSetScript(ACTOR_JENNA, ACTOR_BLACK_CRYSTAL);
	fdActorWatchAndSetScript(ACTOR_SHEBA, ACTOR_BLACK_CRYSTAL);
	fdActorWatchAndSetScript(ACTOR_PIERS, ACTOR_BLACK_CRYSTAL);
	fdActorWatchAndSetScript(ACTOR_KRADEN, ACTOR_BLACK_CRYSTAL);
	fdEventWait(10);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(392, CAMERA_KEEP, 246, 1);
	fdActorSetSpeed(ACTOR_BLACK_CRYSTAL, 0.8);
	fdActorMoveToAndWait(ACTOR_BLACK_CRYSTAL, 392, 246);
	fdActorSetAnimationById(ACTOR_LEFT_DRUM, 1);
	fdActorSetAnimationById(ACTOR_RIGHT_DRUM, 1);
	crystal->velY = toFIXED(3);
	crystal->gravity = toFIXED(1) / 2;
	fdActorSetSpeed(ACTOR_BLACK_CRYSTAL, 1.6);
	fdActorMoveToAndWait(ACTOR_BLACK_CRYSTAL, 392, 234);
	fdEventWait(10);
	fdCameraMoveTo(392, CAMERA_KEEP, 168, 1);
	fdActorSetSpeed(ACTOR_BLACK_CRYSTAL, 0.8);
	fdActorMoveToAndWaitById(ACTOR_BLACK_CRYSTAL, 392, 160);
	fdActorStop(MainRoutineStateWork.leaderActorId);
	fdActorStop(ACTOR_JENNA);
	fdActorStop(ACTOR_SHEBA);
	fdActorStop(ACTOR_PIERS);
	fdActorStop(ACTOR_KRADEN);
	sySynch(1);
	fdActorSetPos(ACTOR_BLACK_CRYSTAL, 0, 0);
	fdEventWait(20);
	fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
	fdPaletteFadeStartInterpolation(40);
	sySynch(40);
	scPlayMusic();
	fdActorShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "It vanished into that hole!" */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
	fdActorShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
	/* "It looks pretty deep..." */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
	fdActorFaceDirectionAndRefreshById(ACTOR_JENNA, westSideFacing);
	/* "Can you see it? Do you think you can reach it?" */
	fdEventShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
	fdActorFaceDirectionById(ACTOR_JENNA, FACING_NORTHWEST, 0);
	fdActorSetSpeed(ACTOR_SHEBA, 0.8);
	fdActorWalkToAndWait(ACTOR_SHEBA, 400, 172);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_LEADER_LOOK_AROUND);
	fdEventWait(20);
	/* "Hm... No, I can't even see it." */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
	fdActorSetAnimationById(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "It's no use... Now, I'll never see Lemuria again... I am lost..." */
	fdEventShowMessageAndWait(ACTOR_PIERS, 0);
	/* "Wait, hold on... I can see light on the other side of this hole..." */
	fdEventShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
	fdActorShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	/* "A light?" */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST);
	/* "There must be a room on the other side of this statue..." */
	fdEventShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	sySndPlay(MUS_SILENCE);
	{
		s32 turn = FACING_NORTH_NORTHEAST;
		fdActorSetHorizontalPosAndAngWithTerrain(ACTOR_AKAFUBU, toFIXED(400),
												 toFIXED(320), turn);
	}
	sySynch(1);
	/* "Who... Who are you!?" */
	fdEventShowMessageAndWait(ACTOR_AKAFUBU, 0);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_STAND);
	sySynch(1);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdCameraSetSpeed(1.6);
	fdCameraMoveTo(392, CAMERA_KEEP, 200, 1);
	fdActorSetSpeed(ACTOR_AKAFUBU, 1.2);
	fdActorWalkToAndWait(ACTOR_AKAFUBU, 404, 220);
	fdActorJump(ACTOR_AKAFUBU, 4, 10);
	/* "What are you doing here? Are you Kibombo!?" Yes: "What? Do you take me
	 * for */
	/* a naive fool!?" No: "Then what are you doing inside the Great..." */
	fdMessageShowAtActor(ACTOR_AKAFUBU, 0);
	if(fdEventAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId,
										 0) == 0) {
		fdEffectSetAttached(ACTOR_AKAFUBU,
							ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdEventWait(20);
		/* "What? Do you take me for a naive fool!?" */
		fdEventShowMessageAndWait(ACTOR_AKAFUBU, 0);
		FieldStateWork->msg += 1;
	}
	else {
		FieldStateWork->msg += 1;
		fdActorShowEmote(ACTOR_AKAFUBU, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
		/* "Then what are you doing inside the Great Gabomba!?" */
		fdEventShowMessageAndWait(ACTOR_AKAFUBU, 0);
	}
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
	/* "We've come to reclaim the black orb you stole!" */
	fdEventShowMessageAndWait(ACTOR_PIERS, 0);
	fdActorShowEmoteById(ACTOR_AKAFUBU, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "Do you really believe I would permit you to take it!?" */
	fdEventShowMessageAndWait(ACTOR_AKAFUBU, 0);
	fdActorShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
	/* "Hey, watch it! You wouldn't even be in here if it weren't for us!" */
	fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
	fdEffectSetAttached(ACTOR_AKAFUBU,
						ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(20);
	/* "What is that supposed to mean?" */
	fdEventShowMessageAndWait(ACTOR_AKAFUBU, 0);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_PIERS, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_AKAFUBU, ANIM_SHAKE_HEAD);
	/* "Why should I believe you? I am Akafubu, the chosen witch doctor of..."
	 */
	fdEventShowMessageAndWait(ACTOR_AKAFUBU, 0);
	fdActorShowEmoteById(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
	fdActorSetSpeed(ACTOR_JENNA, 0.8);
	fdActorWalkToAndWait(ACTOR_JENNA, 420, 228);
	fdActorFaceDirectionAndRefreshById(ACTOR_JENNA, FACING_NORTHWEST);
	fdEventWait(10);
	fdActorSetSpeed(ACTOR_JENNA, 1.6);
	fdActorWalkToAndWait(ACTOR_JENNA, 412, 222);
	fdActorSetSpeed(ACTOR_JENNA, 1);
	fdActorSetSpeed(ACTOR_AKAFUBU, 1);
	fdActorSetMovementTypeById(ACTOR_JENNA, GabombaStatueChamber_WalkOffScript);
	fdEventWait(10);
	fdActorSetMovementTypeById(ACTOR_AKAFUBU,
							   GabombaStatueChamber_WalkOffScript);
	fdEventWait(60);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syGameFlagSet(FLAG_KEEP_PARTY_POSITION);
	syImportEventSetExitRequest(
		GABOMBA_STATUE_CHAMBER_IDOL_TO_GABOMBA_STATUE_ARROWS_EAST);
}

extern u32 GabombaStatueChamber_RejoinLeaderScript[];

/* Back in the small idol's chamber with the party, Akafubu walks up to the */
/* idol and sets the Dragon's Eye in it, and the idol slides east, opening a */
/* passage north behind it. He says he might be able to retrieve the orb and */
/* goes in, and the party talks over whether to trust him before following. */
/* */
/* It plays on arriving from the room of the arrow panels by */
/* `GABOMBA_STATUE_CHAMBER_IDOL_FROM_GABOMBA_STATUE_ARROWS_WEST`. Answering No
 */
/* to Jenna's question is counted by scGabombaStatueChamberCountAnswer. */
void scGabombaStatueChamberAkafubuOffersToHelp(void) {
	HEAP_SLOTS *heap;
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	syWait(1);
	scGabombaStatueChamberUnrollTongue();
	fdMapGridCenterOnFollowedPoint();
	syWait(1);
	fdActorSetPosAndFacing(MainRoutineStateWork.leaderActorId, toFIXED(360),
						   toFIXED(216), FACING_NORTHEAST);
	fdActorSetPosAndFacing(ACTOR_PIERS, toFIXED(376), toFIXED(216),
						   FACING_NORTHEAST);
	fdActorSetPosAndFacing(ACTOR_KRADEN, toFIXED(360), toFIXED(200), 0);
	fdActorSetPosAndFacing(ACTOR_JENNA, toFIXED(408), toFIXED(216),
						   FACING_NORTHWEST);
	fdActorSetPosAndFacing(ACTOR_SHEBA, toFIXED(424), toFIXED(200),
						   FACING_WEST);
	fdActorSetPosAndFacing(ACTOR_AKAFUBU, toFIXED(392), toFIXED(200),
						   FACING_NORTH_NORTHWEST);
	heap = Ram_HeapSlots;
	syWait(1);
	((FIELD_STATE *)heap->fieldState)->transition =
		SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	syFieldEventWait(40);
	fdActorFaceDirection(ACTOR_AKAFUBU, FACING_NORTH_NORTHEAST, 20);
	fdActorFaceDirectionAndWait(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHWEST);
	fdActorShowEmoteById(ACTOR_AKAFUBU, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	fdActorSetSpeed(ACTOR_AKAFUBU, 0.8);
	fdActorWalkToAndWait(ACTOR_AKAFUBU, 392, 168);
	fdEventPlaySnd(SE_HIDDEN_PASSAGE_OPEN);
	scGabombaStatueChamberShowDragonsEye();
	syFieldEventWait(40);
	fdEventPlaySnd(SE_PUSH);
	fdActorGetById(ACTOR_SEATED_IDOL)->motionFlg = 0;
	scGabombaStatueChamberUncoverPassage();
	fdActorSetSpeed(ACTOR_SEATED_IDOL, 0.4);
	fdActorMoveToAndWait(ACTOR_SEATED_IDOL, 424, 152);
	fdActorFaceDirectionAndWait(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHWEST);
	syFieldEventSetMessageId((s32)MsgIMightBe);
	/* "I might be able to retrieve the orb." */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
	fdActorSetAnimationAndWait(ACTOR_AKAFUBU, ANIM_NOD);
	fdActorWalkToAndWait(ACTOR_AKAFUBU, 392, 140);
	syWait(1);
	fdActorSetPos(ACTOR_AKAFUBU, 0, 0);
	syWait(1);
	fdActorShowEmoteById(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	/* "Do you think Akafubu will really return the orb to us?" Yes: "Maybe we
	 * had */
	/* him all wrong..." No: "I think Jenna is right. It sounds like he's..." */
	fdMessageShowAtActor(SPEAKER_WINDOW_ABOVE | ACTOR_JENNA, 0);
	if(fdEventAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId,
										 0) == 0) {
		fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
		fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
		/* "Maybe we had him all wrong..." */
		fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
		((FIELD_STATE *)heap->fieldState)->msg++;
	}
	else {
		((FIELD_STATE *)heap->fieldState)->msg++;
		fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_WEST);
		/* "I think Jenna is right. It sounds like he's going to return what
		 * he..." */
		fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
		scGabombaStatueChamberCountAnswer();
	}
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	fdActorSetAttachedEffect(ACTOR_SHEBA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(20);
	/* "I thought Akafubu was an evil man, but I guess we were wrong..." */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "You never can tell, Sheba..." */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
	/* "We have little choice but to trust that he will return it. Come on!" */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	/* "Let's go, Felix!" */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 20);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetSpeed(ACTOR_KRADEN, 0.8);
	fdActorSetSpeed(ACTOR_PIERS, 0.8);
	fdActorSetSpeed(ACTOR_JENNA, 1);
	fdActorSetSpeed(ACTOR_SHEBA, 1.2);
	{
		s32 path = (s32)GabombaStatueChamber_RejoinLeaderScript;

		fdActorSetScript(ACTOR_KRADEN, path);
		fdActorSetScript(ACTOR_PIERS, path);
		fdActorSetScript(ACTOR_JENNA, path);
		fdActorRunScriptAndWait(ACTOR_SHEBA, path);
	}
	fdEventSetFlag(FLAG_AKAFUBU_OFFERED_HELP);
	fdEventEnd();
}

s32 scInit(void) {
	FIELD_STATE *state;
	s16 scene;

	state = Ram_HeapSlots->fieldState;
	state->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	((ACTOR *)fdActorGetById(fdPartyGetLeaderActor()))->priFlg |=
		ACTOR_PRIORITY_IGNORES_RISES;
	scene = MainRoutineStateWork.next;
	if(scene == SCENE_GABOMBA_STATUE_CHAMBER_IDOL)
		scGabombaStatueChamberInitIdol();
	else if(scene == SCENE_GABOMBA_STATUE_CHAMBER_LIFT)
		scGabombaStatueChamberInitLift();
	else if(scene == SCENE_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR)
		scGabombaStatueChamberInitWitchDoctor();
	return 0;
}

s32 scPrepareMap(void) { return 0; }

/* Sets up the small idol's chamber. Once Akafubu has offered his help, the */
/* tongue is out and the idol stands aside from the passage, the Dragon's Eye */
/* in it; before that, arriving from the ceremony or from the arrow panels */
/* plays his two scenes. Coming in by the tongue makes Retreat in the final */
/* chamber lead back to it. */
void scGabombaStatueChamberInitIdol(void) {
	fdActorDisableById(ACTOR_SEATED_IDOL);
	fdActorDisableById(ACTOR_LEFT_DRUM);
	fdActorDisableById(ACTOR_RIGHT_DRUM);
	sySynch(1);
	if(syGameFlagGet(FLAG_AKAFUBU_OFFERED_HELP)) {
		scGabombaStatueChamberUnrollTongue();
		scGabombaStatueChamberUncoverPassage();
		fdActorSetPos(ACTOR_SEATED_IDOL, 424, 152);
		sySynch(1);
		scGabombaStatueChamberShowDragonsEye();
	}
	else {
		s32 entrance;

		entrance = MainRoutineStateWork.entrance;
		if(entrance ==
		   GABOMBA_STATUE_CHAMBER_IDOL_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH) {
			scGabombaStatueChamberWhereIsTheOrb();
			return;
		}
		if(entrance ==
		   GABOMBA_STATUE_CHAMBER_IDOL_FROM_GABOMBA_STATUE_ARROWS_WEST) {
			scGabombaStatueChamberAkafubuOffersToHelp();
			return;
		}
	}
	{
		MAIN_ROUTINE_STATE *routine = &MainRoutineStateWork;

		if(routine->entrance ==
		   GABOMBA_STATUE_CHAMBER_IDOL_FROM_KIBOMBO_STATUE_GROUNDS) {
			syGameFlagClear(FLAG_GABOMBA_RETREAT_TO_PINION);
			syGameFlagSet(FLAG_GABOMBA_RETREAT_TO_IDOL);
		}
	}
}

/* Sets up the corridor with the lift: with the lift at the foot of the shaft */
/* if the party left it there, or riding back up if the party arrives from the
 */
/* final chamber. */
void scGabombaStatueChamberInitLift(void) {
	if(syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
		if(syGameFlagGet(FLAG_LIFT_DOWN)) {
			MAP_STATE *map = Ram_HeapSlots->mapGrid;
			MAP_LAYER *front = &map->layers[0];
			MAP_LAYER *back = &map->layers[2];
			s32 step = 1;
			s32 i;

			fdBlendScriptSuspend();
			for(i = 0; i < LIFT_TRAVEL; i++) {
				if(i >= LIFT_DESCENT_BLEND_START &&
				   i <= LIFT_DESCENT_BLEND_START + LIFT_BLEND_FRAMES - 1) {
					u32 blend = LIFT_BLEND_BOTTOM(step) << 8;

					blend |= BLEND_WEIGHT_MAX - LIFT_BLEND_BOTTOM(step);
					REG_BLDALPHA = blend;
					step++;
				}
				front->ofsY += toFIXED(1);
				back->ofsY += toFIXED(1);
			}
			fdMapGridCenterOnFollowedPoint();
			sySynch(1);
		}
	}
	else if(
		MainRoutineStateWork.entrance ==
		GABOMBA_STATUE_CHAMBER_LIFT_FROM_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR) {
		scGabombaStatueChamberRideLiftBackUp();
	}
}

/* Sets up the final chamber: once the Gabomba's message is carried, its hole */
/* is open and Akafubu gone; before that, the black orb rests in the idol's */
/* hands. Climbing up from the Gabomba Catacombs, Retreat leads back to where */
/* the party came into the statue: the hall of pinions, or the tongue. */
void scGabombaStatueChamberInitWitchDoctor(void) {
	u32 i;

	for(i = 0; i <= STONE_DOOR_HALVES - 1; i++)
		fdActorSetSprPriority(i + ACTOR_FIRST_DOOR_HALF, 2);
	fdActorDisableById(ACTOR_SEATED_IDOL);
	fdActorGetById(ACTOR_SEATED_IDOL)->y.fixed = 0;
	if(syGameFlagGet(FLAG_GABOMBA_MESSAGE_CARRIED)) {
		scGabombaStatueChamberOpenSecondHole();
		fdActorSetPos(ACTOR_CHAMBER_AKAFUBU, 0, 0);
	}
	else {
		scGabombaStatueChamberShowBlackCrystal();
	}
#if TLA_PATCH_1 /* The Japanese edition does not set the retreat. */
	if(MainRoutineStateWork.entrance ==
	   GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR_FROM_GABOMBA_CATACOMBS_TELEPORTERS) {
		if(syGameFlagGet(FLAG_GABOMBA_RETREAT_TO_PINION)) {
			MainRoutineStateWork.retreatScene = SCENE_GABOMBA_STATUE_PINION;
			MainRoutineStateWork.retreatEntrance =
				GABOMBA_STATUE_PINION_FROM_KIBOMBO_STATUE_GROUNDS;
		}
		else if(syGameFlagGet(FLAG_GABOMBA_RETREAT_TO_IDOL)) {
			MainRoutineStateWork.retreatScene =
				SCENE_GABOMBA_STATUE_CHAMBER_IDOL;
			MainRoutineStateWork.retreatEntrance =
				GABOMBA_STATUE_CHAMBER_IDOL_FROM_KIBOMBO_STATUE_GROUNDS;
		}
	}
#endif
}

/* clang-format off */
/* Jenna, then Akafubu, walks east out of the small idol's chamber. */
u32 GabombaStatueChamber_WalkOffScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(428, 0, 234),
    SC_SYNC,
    SC_MOVE(488, 0, 234),
    SC_SYNC,
    SC_END,
};

/* A companion walks to Felix's place and is put away, as the party follows */
/* Akafubu. */
u32 GabombaStatueChamber_RejoinLeaderScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(360, 0, 216),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_POSITION_ORIGIN,
    SC_END,
};

/* The new witch doctor spins round once, a 32nd of a turn a frame. */
u32 GabombaStatueChamber_SpinScript[] = {
    SC_ADD(SW_FACING, TRIG_TURN / 32),
    SC_WAIT(1),
    SC_LOOP(32, SC_TOP),
    SC_END,
};

/* The chest appears 64 pixels above the floor before the idol and sinks to */
/* it, a pixel a frame. */
s32 GabombaStatueChamber_ChestDescendsScript[] = {
    SC_POSITION(392, 64, 124),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_Y, toFIXED(-1)),
    SC_WAIT(1),
    SC_LOOP(64, SC_TO(1)),
    SC_WAIT(1),
    SC_END,
};

/* The chest rises away, two pixels a frame for 80 frames, and is put away. */
u32 GabombaStatueChamber_ChestRisesScript[] = {
    SC_ADD(SW_Y, toFIXED(2)),
    SC_WAIT(1),
    SC_LOOP(80, SC_TOP),
    SC_POSITION_ORIGIN,
    SC_END,
};

/* Kraden, then Felix, walks to the path below the idol and down it, south, */
/* out of the final chamber. */
u32 GabombaStatueChamber_PartyLeavesScript[] = {
    SC_ANIMATION(2), SC_MOVE(392, 0, 148),
    SC_SYNC,   SC_MOVE(392, 0, 236),
    SC_SYNC,   SC_ANIMATION(1),
    SC_END,
};

/* Turns a stone door half the other way: the halves east of the path. */
s32 GabombaStatueChamber_MirroredScript[] = {
    SC_SET(SW_SCALE_X, -FIXED_ONE),
    SC_END,
};

ACTOR *GabombaStatueChamber_ShownItem = NULL;

SCENE_ENTRANCE SceneEntrances[] = {
    {0, CONDITION_ALWAYS, 392, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

SCENE_REGION GabombaStatueChamber_IdolExitRegions[] = {
    {464, 16, 144, 480, 32, 160, CONDITION_ALWAYS,
     GABOMBA_STATUE_CHAMBER_IDOL_TO_GABOMBA_STATUE_ARROWS_EAST},
    {SCENE_TABLE_END},
};

u32 SceneExits[] = {
    SCENE_EXITS(GabombaStatueChamberIdol),
    SCENE_EXIT_UNLESS(GABOMBA_STATUE_CHAMBER_IDOL_TO_GABOMBA_STATUE_COGS, GabombaStatueCogs, GABOMBA_STATUE_COGS_FROM_GABOMBA_STATUE_CHAMBER_IDOL, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GABOMBA_STATUE_CHAMBER_IDOL_TO_GABOMBA_STATUE_ARROWS_EAST, GabombaStatueArrows, GABOMBA_STATUE_ARROWS_FROM_GABOMBA_STATUE_CHAMBER_IDOL_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GABOMBA_STATUE_CHAMBER_IDOL_TO_KIBOMBO_STATUE_GROUNDS, KibomboStatueGrounds, KIBOMBO_STATUE_GROUNDS_FROM_GABOMBA_STATUE_CHAMBER_IDOL, FLAG_KIBOMBO_AT_CEREMONY),
    SCENE_EXIT_UNLESS(GABOMBA_STATUE_CHAMBER_IDOL_TO_KIBOMBO_STATUE_GROUNDS, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_CHAMBER_IDOL_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GABOMBA_STATUE_CHAMBER_IDOL_TO_GABOMBA_STATUE_CHAMBER_LIFT, GabombaStatueChamberLift, GABOMBA_STATUE_CHAMBER_LIFT_FROM_GABOMBA_STATUE_CHAMBER_IDOL, EXIT_FLAG_NONE),
    SCENE_EXITS(GabombaStatueChamberLift),
    SCENE_EXIT(GABOMBA_STATUE_CHAMBER_LIFT_TO_GABOMBA_STATUE_CHAMBER_IDOL, GabombaStatueChamberIdol, GABOMBA_STATUE_CHAMBER_IDOL_FROM_GABOMBA_STATUE_CHAMBER_LIFT),
    SCENE_EXIT(GABOMBA_STATUE_CHAMBER_LIFT_TO_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR, GabombaStatueChamberWitchDoctor, GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR_FROM_GABOMBA_STATUE_CHAMBER_LIFT),
    SCENE_EXITS(GabombaStatueChamberWitchDoctor),
    SCENE_EXIT(GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR_TO_GABOMBA_STATUE_CHAMBER_LIFT, GabombaStatueChamberLift, GABOMBA_STATUE_CHAMBER_LIFT_FROM_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR),
    SCENE_EXIT(GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR_TO_GABOMBA_CATACOMBS_TELEPORTERS, GabombaCatacombsTeleporters, GABOMBA_CATACOMBS_TELEPORTERS_FROM_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR),
    SCENE_EXIT(GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR_TO_KIBOMBO_HOUSES, KibomboHouses, KIBOMBO_HOUSES_FROM_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR),
    SCENE_EXITS_END,
};

ACTOR_PLACE GabombaStatueChamber_IdolActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SEATED_IDOL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PINK_DRUM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 256, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PINK_DRUM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 256, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_AKAFUBU, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE GabombaStatueChamber_WitchDoctorActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SEATED_IDOL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_AKAFUBU, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 136, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 392, 0, 84, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_AKAFUBU_WITCH_DOCTOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 336, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 368, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, (s32)GabombaStatueChamber_MirroredScript, 416, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, (s32)GabombaStatueChamber_MirroredScript, 432, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, (s32)GabombaStatueChamber_MirroredScript, 448, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 336, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 368, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, (s32)GabombaStatueChamber_MirroredScript, 416, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, (s32)GabombaStatueChamber_MirroredScript, 432, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, (s32)GabombaStatueChamber_MirroredScript, 448, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 336, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 368, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, (s32)GabombaStatueChamber_MirroredScript, 416, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, (s32)GabombaStatueChamber_MirroredScript, 432, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, (s32)GabombaStatueChamber_MirroredScript, 448, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

ACTOR_PLACE GabombaStatueChamber_Placements[] = {
    {SCENE_TABLE_END},
};
/* clang-format on */

TRIGGER GabombaStatueChamber_IdolTriggers[] = {
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 GABOMBA_STATUE_CHAMBER_IDOL_TO_GABOMBA_STATUE_COGS, CONDITION_ALWAYS,
	 EVENT(scGabombaStatueChamberStepIntoFirstHole)},
	{TRIGGER_EXIT, GABOMBA_STATUE_CHAMBER_IDOL_TO_KIBOMBO_STATUE_GROUNDS,
	 CONDITION_ALWAYS, GABOMBA_STATUE_CHAMBER_IDOL_TO_KIBOMBO_STATUE_GROUNDS},
	{TRIGGER_EXIT, GABOMBA_STATUE_CHAMBER_IDOL_TO_GABOMBA_STATUE_CHAMBER_LIFT,
	 CONDITION_ALWAYS,
	 GABOMBA_STATUE_CHAMBER_IDOL_TO_GABOMBA_STATUE_CHAMBER_LIFT},
	{TRIGGER_TABLE_END},
};

TRIGGER GabombaStatueChamber_LiftTriggers[] = {
	{TRIGGER_EXIT, GABOMBA_STATUE_CHAMBER_LIFT_TO_GABOMBA_STATUE_CHAMBER_IDOL,
	 CONDITION_ALWAYS,
	 GABOMBA_STATUE_CHAMBER_LIFT_TO_GABOMBA_STATUE_CHAMBER_IDOL},
	{TRIGGER_EXIT,
	 GABOMBA_STATUE_CHAMBER_LIFT_TO_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR,
	 CONDITION_ALWAYS,
	 GABOMBA_STATUE_CHAMBER_LIFT_TO_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR},
	{TRIGGER_STEP, CELL_BEHAVIOR_LIFT, FLAG_LEADER_ON_LIFT,
	 EVENT(scGabombaStatueChamberRideLiftDown)},
	{TRIGGER_STEP, CELL_BEHAVIOR_SOUTH_OF_LIFT, CONDITION_ALWAYS,
	 EVENT(scGabombaStatueChamberStepOffLift)},
	{TRIGGER_TABLE_END},
};

TRIGGER GabombaStatueChamber_WitchDoctorTriggers[] = {
	{TRIGGER_EXIT,
	 GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR_TO_GABOMBA_STATUE_CHAMBER_LIFT,
	 CONDITION_ALWAYS,
	 GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR_TO_GABOMBA_STATUE_CHAMBER_LIFT},
	{STEP_TRIGGER_WALKING,
	 GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR_TO_GABOMBA_CATACOMBS_TELEPORTERS,
	 CONDITION_ALWAYS, EVENT(scGabombaStatueChamberStepIntoSecondHole)},
	{TRIGGER_STEP, CELL_BEHAVIOR_BELOW_AKAFUBU, FLAG_AKAFUBU_REACHED_CHAMBER,
	 EVENT(scGabombaStatueChamberAkafubuReachesChamber)},
	{TRIGGER_TALK, ACTOR_CHAMBER_AKAFUBU, CONDITION_ALWAYS,
	 MSGNO(MsgYourBlackOrb)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHAMBER_AKAFUBU,
	 CONDITION_ALWAYS, MSGNO(MsgOeiaWhatAm)},
	{EXAMINE_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_IDOL,
	 FLAG_GABOMBA_MESSAGE_CARRIED,
	 EVENT(scGabombaStatueChamberGabombaNamesWitchDoctor)},
	{TRIGGER_TABLE_END},
};

TRIGGER GabombaStatueChamber_Triggers[] = {
	{TRIGGER_TABLE_END},
};

/* @file */
/* Garoh, the village of werewolves that guards Air's Rock, in five scenes that
 */
/* share this overlay: the way up from the world map (SceneId_GarohWayUp), */
/* the path up to the village (SceneId_GarohPath, whose chest */
/* holds the Hypnos Sword), the village itself (SceneId_Garoh), the tunnels */
/* under its houses (SceneId_GarohTunnels) and the cave behind the rock by the
 */
/* sanctum (SceneId_GarohWerewolfCave). */
/* */
/* The party first comes on a night of the full moon. Climbing the way up, */
/* the light turns to evening and then to night (scGarohCrossLowerTwilightLine,
 */
/* scGarohCrossUpperTwilightLine); on the path a werewolf howls and runs past */
/* into the village (scGarohWerewolfRunsPast). The hooded villagers say there */
/* are no werewolves, and by the sanctum an old werewolf peers out of the big */
/* grey rock and hides again (scGarohWerewolfPeersFromCave). With Reveal from */
/* Air's Rock the party opens the rock, follows the little werewolf into the */
/* cave (scGarohFollowWerewolfIntoCave) and is surrounded by the werewolves, */
/* whose elder Maha tells them to come back later */
/* (scGarohWerewolvesRevealThemselves). By day the villagers show their faces,
 */
/* and Maha tells the werewolves' legend and gives the party Ether */
/* (scGarohMahaTellsTheLegend). */

/* This unit declares its own form of fdRopeLash, below. */
#define OWN_Rope_Lash

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

/* The ROM's callers here are compiled with no declaration in scope, as against
 */
/* this form: no parameter list and an s32 result, where the definition takes */
/* the rope and two posts and returns nothing. */
s32 fdRopeLash();
#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"

#include "macros.h"

void scGarohSinkPillar(s32 id, ACTOR *actor);
void scGarohSinkFirstPillarOnCell(void);
void scGarohSinkSecondPillarOnCell(void);
void scGarohCheckThirdPillar(void);
void scGarohSinkFourthPillarOnCell(void);
void scGarohScatterDustRing(s32 id);
#include "field/event_inline.h"
#include "scene/scene.h"
#include "field/field_sequence.h"
#include "graphics/palette_buffers.h"
#include "game/game.h"
#include "field/call.h"
#include "field/actor_script.h"
#include "text/msg_ids.h"

extern ACTOR_PLACE Garoh_VillageDayActorPlacements[];
extern TRIGGER Garoh_WayUpTriggers[];
void scGarohWerewolvesRevealThemselves(void);
void scGarohMahaTellsTheLegend(void);
void scGarohInitVillage(void);
void scGarohInitWayUp(void);
extern u32 Garoh_WerewolfLookAroundScript[];
s32 scGarohMetWerewolves();
void scGarohOpenDoorAndEnter(void);
void scGarohWerewolfPeersFromCave(void);
void scGarohEnterOpening(void);
void scGarohMeetInTheCave(void);
void scGarohPlaceFirstPillar(void);
void scGarohPlaceSecondPillar(void);
void scGarohPlaceThirdPillar(void);
void scGarohPlaceFourthPillar(void);
void scGarohPlacePushedPillar(void);
void scGarohRevealTunnelPassage(void);
void scGarohHideCaveEntrance(void);
void scGarohRestoreWayUpLight(void);
void scGarohHideTunnelPassage(void);
void scGarohHideCavePassage(void);
void scGarohCrossLowerTwilightLine(void);
void scGarohCrossUpperTwilightLine(void);
extern u32 FieldEffect_ShortScript[];
extern u32 FieldEffect_LongScript[];
extern s32 Garoh_ShrunkSpriteScript[];
extern s16 Garoh_DoorFrames[];
extern s16 Garoh_WideDoorFrames[];
extern s16 Garoh_TallDoorFrames[];
void scGarohLashRope(void);
/* BG2's horizontal offset for each screen line while the path's layer ripples
 */
/* by night (scGarohWavePathLayer). */
u16 Garoh_WaveLines[DISPLAY_HEIGHT] BSS_DATA ALIGNED(16);

/* The village's people by night and by day (SceneId_Garoh), numbering the */
/* rows of Garoh_VillageActorPlacements and Garoh_VillageDayActorPlacements: */
/* the five who go hooded by night show their faces by day. */
typedef enum GAROH_VILLAGE_ACTOR_ID {
	/* By night he asks whether the party has seen a werewolf; by day he has */
	/* heard from Maha that the party is special. */
	ACTOR_VILLAGER_ASKING_ABOUT_THE_WEREWOLF = ACTOR_FIRST_PLACED,
	/* "Forgive me for not taking my hood off..." */
	ACTOR_HOODED_VILLAGER,
	/* "Gaaaroooooo!" ... "I was just calling my dog." */
	ACTOR_HOWLING_VILLAGER,
	/* "Strangers should not go about at night..." */
	ACTOR_VILLAGER_WARNING_STRANGERS,
	/* "Don't you love the sight of the full moon?" */
	ACTOR_VILLAGER_ON_THE_FULL_MOON,
	/* By day only: "I was in the cave with Maha! I was that little werewolf!"
	 */
	ACTOR_LITTLE_WEREWOLF,
	/* The old werewolf who peers out of the cave's entrance by the Sanctum the
	 */
	/* first night: Maha, on his sprite in the cave. */
	ACTOR_MAHA_AT_THE_CAVE_ENTRANCE = ACTOR_FIRST_PLACED + 13,
	ACTOR_CAVE_ENTRANCE
} GAROH_VILLAGE_ACTOR_ID;

/* The path up to the village and the werewolves' cave */
/* (SceneId_GarohPath and SceneId_GarohWerewolfCave), numbering the */
/* rows of Garoh_WerewolfCaveActorPlacements; GarohPath places the */
/* werewolf and Kraden on the same rows. */
typedef enum GAROH_WEREWOLF_ACTOR_ID {
	ACTOR_WEREWOLF = ACTOR_FIRST_PLACED,
	ACTOR_KRADEN,
	ACTOR_MAHA,
	ACTOR_CAVE_PASSAGE,
	ACTOR_ETHER,
	/* The cave's fires, shrunk to wall lamps; the first two burn behind the */
	/* party. */
	ACTOR_FIRE_A = ACTOR_FIRST_PLACED + 5,
	ACTOR_FIRE_B
} GAROH_WEREWOLF_ACTOR_ID;

/* The tightrope posts on the path up to the village: Jenna, Sheba and Piers */
/* take their own numbers, so the posts follow the werewolf and Kraden. */
enum {
	ACTOR_PATH_TIGHTROPE_POST_A = ACTOR_FIRST_PLACED + 2,
	ACTOR_PATH_TIGHTROPE_POST_B
};

/* The stone pillars on the way up to the village (SceneId_GarohWayUp), */
/* numbering the rows of Garoh_WayUpActorPlacements: Move or a push takes */
/* each onto the slot where it sinks. */
typedef enum GAROH_WAY_UP_ACTOR_ID {
	ACTOR_FIRST_PILLAR = ACTOR_FIRST_PLACED,
	ACTOR_SECOND_PILLAR,
	ACTOR_THIRD_PILLAR,
	ACTOR_FOURTH_PILLAR
} GAROH_WAY_UP_ACTOR_ID;

/* The cells the step and examine triggers answer, by their behaviour in each */
/* scene's map. A door's or an opening's behaviour is the exit it leaves by. */
typedef enum GAROH_CELL_BEHAVIOR {
	/* The village: east of the sanctum's door, cell (20, 16). */
	CELL_BEHAVIOR_BY_SANCTUM_DOOR = 20,
	/* The village: the well, cells (19..20, 18..19). */
	CELL_BEHAVIOR_GAROH_WELL = 200,
	/* The path up to the village: across it at cells (9..11, 19). */
	CELL_BEHAVIOR_WEREWOLF_RUNS_PAST = 30,
	/* The way up: where the light turns to evening, cell (13, 26), and to */
	/* night, cell (16, 22). */
	CELL_BEHAVIOR_LOWER_TWILIGHT_LINE = 20,
	CELL_BEHAVIOR_UPPER_TWILIGHT_LINE = 21,
	/* The werewolves' cave: below the cleft, cells (17..18, 49), and below */
	/* where Maha stands, cells (19..21, 15). */
	CELL_BEHAVIOR_CAVE_MOUTH = 30,
	CELL_BEHAVIOR_BELOW_MAHA = 31
} GAROH_CELL_BEHAVIOR;

/* Story flags of the werewolves' secret, in the order the party meets them. */
#define FLAG_GAROH_REACHED_VILLAGE	 0x919
#define FLAG_GAROH_SAW_WEREWOLF		 0x91c
#define FLAG_GAROH_FOLLOWED_WEREWOLF 0x91d
#define FLAG_GAROH_MET_WEREWOLVES	 0x91e
#define FLAG_GAROH_HEARD_MAHA		 0x91f
/* For this visit to the area: the old werewolf has peered from the cave, */
/* the little werewolf has thanked the party, and the way up is in the */
/* evening's light, between its two lines. */
#define FLAG_WEREWOLF_PEERED_OUT	 FLAG_AREA_TEMP_20
#define FLAG_LITTLE_WEREWOLF_THANKED FLAG_AREA_TEMP_01
#define FLAG_EVENING				 FLAG_AREA_TEMP_18
/* By night the village's actors take the warm tint as well as its map. */
/* Nothing in the code sets it, so they keep their own colours. */
#define FLAG_NIGHT_TINTS_ACTORS FLAG_AREA_TEMP_11

/* The passage within the werewolves' cave behind the round grey rock, which */
/* Reveal opens (scGarohEnterOpening): from before the rock to the little room
 */
/* beyond it and back (scene_exits.h names only the exits between scenes). An */
/* exit and the entrance on its side share a number. */
typedef enum GAROH_WEREWOLF_CAVE_PASSAGE {
	GAROH_WEREWOLF_CAVE_TO_ROOM_BEHIND_ROCK = 6,
	GAROH_WEREWOLF_CAVE_ROOM_BEHIND_ROCK_TO_CAVE,
	GAROH_WEREWOLF_CAVE_FROM_ROOM_BEHIND_ROCK = 6,
	GAROH_WEREWOLF_CAVE_ROOM_BEHIND_ROCK_FROM_CAVE
} GAROH_WEREWOLF_CAVE_PASSAGE;

/* The werewolf sheet's last picture, held: sitting on the ground. */
enum { ANIM_WEREWOLF_SIT = 5 };
/* The way up: the pillars sunk into their slots, and the rope Lashed. */
#define FLAG_FIRST_PILLAR_SUNK	FLAG_SCENE_TEMP_01
#define FLAG_SECOND_PILLAR_SUNK FLAG_SCENE_TEMP_02
#define FLAG_GAROH_ROPE_LASHED	FLAG_SCENE_TEMP_03
#define FLAG_FOURTH_PILLAR_SUNK FLAG_SCENE_TEMP_04
/* The two lines on the way up where the light changes, crossed since the */
/* party came in. */
#define FLAG_CROSSED_LOWER_TWILIGHT_LINE FLAG_SCENE_TEMP_10
#define FLAG_CROSSED_UPPER_TWILIGHT_LINE FLAG_SCENE_TEMP_11

/* The houses' lit windows, colour 14 of BG palette 1, kept white when the */
/* village darkens for the night. */
#define WINDOW_LIGHT 14

/* The way into the path up to the village that no exit uses: by night it tints
 */
/* the path purple rather than warm and dark. */
#define ENTRANCE_PATH_PURPLE_NIGHT 12

/* The steps in which the Ether grows from two fifths of its size to full. */
#define ETHER_GROW_STEPS 20

/* By day the path up to the village's second layer takes the first's scale and
 */
/* wrap. */
void scGarohMatchSecondLayer(void) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	MAP_LAYER *to = &map->layers[1];
	MAP_LAYER *from = &map->layers[0];

	to->scaleX = from->scaleX;
	to->scaleY = from->scaleY;
	to->maskX = from->maskX;
	to->maskY = from->maskY;
}

/* By night the second map layer of the path up to the village ripples: each */
/* line is offset by two sines of the frame count, handed to an HBlank DMA. */
void scGarohWavePathLayer(void) {
	s32 x;
	/* The sine function itself, called at the address sySin's stub call */
	/* jumps to. */
	s32 (*sine)(s32) = *(s32 (**)(s32))(((u32)sySin & ~3) + 4);
	MAP_LAYER *layer = fdMapGridGetLayer(1);
	u16 *line = Garoh_WaveLines;

	if(FieldStateWork->revealActive == 0) {
		u32 frame;
		s32 z;
		s32 i;
		volatile u16 *dma;

		layer->speedY = 0;
		frame = (u8)FrameCount;
		i = 0;
		/* FAKEMATCH(barrier): i and line held through an empty asm; as no asm.
		 */
		asm("" : "+r"(i), "+r"(line) : "r"(line));
		z = FIXEDtoINT(layer->y);
		x = FIXEDtoINT(layer->x);
		/* FAKEMATCH(dead): a once-only block that places the two waves' steps
		 * and angles around the loop; as them declared with z. */
		{
			/* The two waves: their angles at the top line, and the steps each
			 * line */
			/* turns them on by. */
			s32 stepA = TRIG_TURN * 51 / 256;
			s32 stepB = TRIG_TURN * 153 / 2048;
			s32 a = z * stepA + frame * (TRIG_TURN / 64);
			s32 b = z * stepB - frame * (TRIG_TURN / 64);

			for(; i != DISPLAY_HEIGHT;) {
				s32 s1 = sine(a);
				s32 s2 = sine(b);

				i++;
				/* FAKEMATCH(dead): a store to z, which nothing reads again. */
				if(i == 300 || x == 300)
					z = 0;
				*line++ = FIXEDtoINT((s1 * 3 + s2 * 7) >> 3) + x;
				a += stepA;
				b += stepB;
			}
		}
		line = Garoh_WaveLines;
		REG_BG2HOFS = line[0];
		dma = REG_DMA0;
		dma[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
		dma[5] &= ~DMA_ENABLE;
		(void)dma[5];
		syDMAXCopy(line + 1, (void *)&REG_BG2HOFS, DMA_HBLANK_HALFWORD_PER_LINE,
				   (volatile u32 *)dma);
	}
}

extern u16 Garoh_PathDayColors[];
extern u16 Garoh_PathNightColors[];

/* Writes count colours from first on, from a table of red, green and blue */
/* halfwords, to the BG palette and the palette fade's source. */
#define DEFINE_GAROH_LOAD_PALETTE(name, components, first, count)              \
	void name(void) {                                                          \
		PALETTE_FADE *buffers = Ram_HeapSlots->palFade;                        \
		u32 i;                                                                 \
                                                                               \
		for(i = 0; i < count; i++) {                                           \
			BG_PLTT[first + i] = components[i * 3 + 2] << 10 |                 \
								 components[i * 3 + 1] << 5 |                  \
								 components[i * 3];                            \
			buffers->pal[first + i] = BG_PLTT[first + i];                      \
		}                                                                      \
	}

/* By day the path's colours: BG palette 1's colours 8 to 15. */
DEFINE_GAROH_LOAD_PALETTE(scGarohLoadPathDayCols, Garoh_PathDayColors, 24, 8)

/* By night the path's colours: BG palette 6's colours 1 to 9. */
DEFINE_GAROH_LOAD_PALETTE(scGarohLoadPathNightCols, Garoh_PathNightColors, 97,
						  9)

/* A script's call: clears the actor's sprite flags. */
s32 scGarohClearScriptedActorSprFlags(ACTOR *actor) {
	fdActorSetSprFlags(actor, 0);
	return 0;
}

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

extern ACTOR_PLACE Garoh_ActorPlacements[];
extern ACTOR_PLACE Garoh_VillageActorPlacements[];
extern ACTOR_PLACE Garoh_WayUpActorPlacements[];
extern ACTOR_PLACE Garoh_TunnelsActorPlacements[];
extern ACTOR_PLACE Garoh_PathActorPlacements[];
extern ACTOR_PLACE Garoh_WerewolfCaveActorPlacements[];

extern MAP_GRID_CELL_ANIMATION Garoh_Doors[];

/* The people of the scene being entered: in the village, the hooded */
/* villagers by night or their faces once Maha has shared the secret. */
const void *scGetActorPlacements(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_GAROH) {
		if(fdEventGetFlag(FLAG_GAROH_SECRET_SHARED))
			return Garoh_VillageDayActorPlacements;
		return Garoh_VillageActorPlacements;
	}
	if(scene == SCENE_GAROH_PATH)
		return Garoh_PathActorPlacements;
	if(scene == SCENE_GAROH_WAY_UP)
		return Garoh_WayUpActorPlacements;
	if(scene == SCENE_GAROH_TUNNELS)
		return Garoh_TunnelsActorPlacements;
	if(scene == SCENE_GAROH_WEREWOLF_CAVE)
		return Garoh_WerewolfCaveActorPlacements;
	return Garoh_ActorPlacements;
}

/* Walking north into the door of one of Garoh's round houses, or of the */
/* sanctum, opens it and Felix walks inside, while the villagers stand still: */
/* the young werewolf's family's home at cell (14, 23), the old couple's at */
/* (8, 14), the weapons and armor shop's wide door at (10, 24), the inn at */
/* (21, 25) and the sanctum's tall door at (14, 15). */
/* */
/* The door's cell behaviour is the exit it leaves by, which also picks its */
/* opening frames from `Garoh_Doors`. */
void scGarohOpenDoorAndEnter(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	s32 index;
	s32 i;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	for(i = ACTOR_FIRST_PLACED; (u32)i <= ACTOR_COUNT - 1; i++) {
		{
			ACTOR *actor = fdActorGet(i);

			if(actor != NULL)
				actor->motionFlg = 0;
		}
	}
	index = state->stepTrigger;
	fdEventPlaySnd(SE_DOOR_OPEN);
	index--;
	fdMapGridAnimateCells(Garoh_Doors[index].steps, Garoh_Doors[index].x,
						  Garoh_Doors[index].y);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	{
		s32 owner = MainRoutineStateWork.leaderActorId;

		/* The sanctum's tall door is entered straight on, the others from the
		 */
		/* centre of their cell. */
		if(index == GAROH_TO_GAROH_ROOMS_SANCTUM - 1)
			fdActorSetDestinationOffset(owner, 0, -4);
		else
			fdActorCenterAndWalk(owner, 2, -4);
	}
	syFieldEventWait(6);
	syFieldEventSetExitRequest(state->stepTrigger);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdEventEnd();
}

/* Talking at night to the hooded villager outside the young werewolf's */
/* family's home: "So, uh...you say you saw a werewolf?" Yes gets "Don't be */
/* ridiculous... There's no such thing as werewolves!"; No gets its own reply.
 */
void scGarohAskIfSawWerewolf(s32 actor) {
	s32 msg = (s32)MsgGarohSoUhYouSayYou;

	syFieldEventSetMessageId(msg);
	fdEventOpenMessage(actor, 0);
	if(syFieldEventAskYesNoWithActorAnimation(syFieldPartyGetLeaderActor(),
											  0) == 0) {
		syFieldEventWait(10);
		syFieldEventSetMessageId(msg + 1);
	}
	else {
		syFieldEventWait(20);
		syFieldEventSetMessageId(msg + 2);
	}
	fdEventShowMessage(actor, 0);
}

/* Talking at night to the hooded villager outside the Item Shopkeeper's */
/* house: "Don't you love the sight of the full moon?" Yes gets "Yes, I */
/* agree... But it brings me grief."; No gets its own reply. */
void scGarohAskAboutFullMoon(s32 actor) {
	s32 msg = (s32)MsgGarohDontYouLoveTheSight;

	syFieldEventSetMessageId(msg);
	fdEventOpenMessage(actor, 0);
	if(syFieldEventAskYesNoWithActorAnimation(syFieldPartyGetLeaderActor(),
											  0) == 0) {
		syFieldEventWait(10);
		syFieldEventSetMessageId(msg + 1);
	}
	else {
		syFieldEventWait(20);
		syFieldEventSetMessageId(msg + 2);
	}
	fdEventShowMessage(actor, 0);
}

/* Talking by day to the woman in green south-west of the well, the villager */
/* who warned strangers off at night: "Maha said that you were the only */
/* outsiders we could share our secret with...", and she asks whether it */
/* bothers them that everyone here is a werewolf. */
void scGarohAskAboutSecret(s32 actor) {
	s32 msg = (s32)MsgGarohMahaSaidThatYouWere;

	syFieldEventSetMessageId(msg);
	fdEventOpenMessage(actor, 0);
	if(syFieldEventAskYesNoWithActorAnimation(syFieldPartyGetLeaderActor(),
											  0) == 0) {
		syFieldEventWait(10);
		syFieldEventSetMessageId(msg + 1);
	}
	else {
		syFieldEventWait(20);
		syFieldEventSetMessageId(msg + 2);
	}
	fdEventShowMessage(actor, 0);
}

/* Talking by day to the grey-haired man west of the well, who claimed at */
/* night to be calling his dog: he howls "Awoooooh!" and is glad he no longer */
/* has to hide it. */
void scGarohHowlOutLoud(s32 actor) {
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventSetMessageId((s32)MsgGarohAwoooooh);
	/* "Awoooooh!" */
	syFieldEventShowMessageThenWait(actor, 0, 10);
	/* "Heh heh... Sorry about that! I'm so happy I don't have to hide that..."
	 */
	syFieldEventShowMessageThenWait(actor, 0, 10);
	fdEventEnd();
}

/* Talking at night to the hooded villager west of the well: he howls */
/* "Gaaaroooooo!", catches himself ("Gah! Whoops...") and says he was just */
/* calling his dog. His code would cut him to the howl alone once */
/* FLAG_AREA_TEMP_00 is set, but nothing in Garoh, its paths, tunnels, cave or
 */
/* rooms sets that flag, so the player always hears all three lines. */
void scGarohHowlForDog(s32 actor) {
	s32 flag;
	s32 msg;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	flag = fdEventGetFlag(FLAG_AREA_TEMP_00);
	msg = (s32)MsgGarohGaaarooooooForDog;
	if(flag) {
		syFieldEventSetMessageId(msg);
	}
	else {
		syFieldEventSetMessageId(msg);
		/* "Gaaaroooooo!" */
		syFieldEventShowMessageThenWait(actor, 0, 10);
		/* "Gah! Whoops..." */
		syFieldEventShowMessageThenWait(actor, 0, 10);
	}
	syFieldEventShowMessageThenWait(actor, 0, 10);
	fdEventEnd();
}

/* Talking by day to the fair-haired boy running about by the sanctum, or */
/* reading his mind the first time: "Hey, it's you guys! Thanks for last */
/* night!", and he explains he was the little werewolf in the cave. Later he */
/* says he is the reason the village no longer needs its hoods. */
void scGarohLittleWerewolfThanks(void) {
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	scFieldActorStop(ACTOR_LITTLE_WEREWOLF);
	fdActorSetAnimation(ACTOR_LITTLE_WEREWOLF, ANIM_STAND);
	syFieldEventWait(10);
	fdActorFaceActor(ACTOR_LITTLE_WEREWOLF, ACTOR_FELIX, 0);
	if(fdEventGetFlag(FLAG_LITTLE_WEREWOLF_THANKED)) {
		syFieldEventSetMessageId((s32)MsgGarohImTheReasonYouCan);
	}
	else {
		syFieldEventSetMessageId((s32)MsgGarohHeyItsYouGuysThanks);
		/* "Hey, it's you guys! Thanks for last night!" */
		syFieldEventShowMessageThenWait(ACTOR_LITTLE_WEREWOLF, 0, 10);
		scFieldActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 15);
		scFieldActorShowEmote(ACTOR_LITTLE_WEREWOLF,
							  EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 10);
		/* "Oh, yeah... That's right... You probably don't recognize me in
		 * my..." */
		syFieldEventShowMessageThenWait(ACTOR_LITTLE_WEREWOLF, 0, 5);
		scFieldActorShowEmote(ACTOR_LITTLE_WEREWOLF,
							  EMOTE_IN_FRONT | EMOTE_SMILE, 15);
		/* "I was in the cave with Maha! I was that little werewolf!" */
		syFieldEventShowMessageThenWait(ACTOR_LITTLE_WEREWOLF, 0, 5);
	}
	syFieldEventShowMessageThenWait(ACTOR_LITTLE_WEREWOLF, 0, 10);
	fdEventSetFlag(FLAG_LITTLE_WEREWOLF_THANKED);
	/* Back to running about the village. */
	fdActorSetScript(ACTOR_LITTLE_WEREWOLF, MOVEMENT_TYPE_WANDER);
	fdEventEnd();
}

/* Walking past the sanctum's door at night before meeting the werewolves, */
/* cell (20, 16): the camera looks up at the big grey rock on the ledge above,
 */
/* an old grey-maned werewolf opens it with Reveal and steps out, cries */
/* "What!? Outsiders, here?" and slips back inside, the rock closing behind */
/* him. Once a visit. */
void scGarohWerewolfPeersFromCave(void) {
	s32 flag;
	s32 facing;

	void scGarohOpenCaveEntrance(s32 id) {
		fdPsynergyLoadResources(ABILITY_REVEAL, 1);
		fdPsynergySetupActorPair(id, 0);
		fdPsynergyRunHandler(FIELD_PSYNERGY_HANDLER_ON_TARGET);
		fdBattleEffectStopMainActor();
		fdPsynergyEnd();
	}

	void scGarohCloseCaveEntrance(s32 id) {
		fdPsynergyLoadResources(ABILITY_REVEAL, 1);
		fdPsynergySetupActorPair(id, 0);
		fdPsynergyRunHandler(FIELD_PSYNERGY_HANDLER_EXTRA);
		fdBattleEffectStopMainActor();
		fdPsynergyEnd();
	}

	flag = fdEventGetFlag(FLAG_WEREWOLF_PEERED_OUT);
	if(flag == 0) {
		fdEventSetFlag(FLAG_WEREWOLF_PEERED_OUT);
		fdEventStart();
		syFieldEventEndLastingPsynergy(0);
		fdActorSetSprFlags(fdActorGet(ACTOR_CAVE_ENTRANCE), 0);
		facing = FACING_SOUTH;
		fdActorSetPosAndFacing(ACTOR_CAVE_ENTRANCE, toFIXED(360), toFIXED(199),
							   facing);
		fdCameraMoveTo(359, CAMERA_KEEP, 220, 1);
		fdCameraWaitForArrival();
		scGarohOpenCaveEntrance(ACTOR_CAVE_ENTRANCE);
		syFieldEventWait(30);
		fdActorSetPosAndFacing(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE, toFIXED(360),
							   toFIXED(199), facing);
		fdActorWalkByAndWait(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE, 0, 16);
		scGarohCloseCaveEntrance(ACTOR_CAVE_ENTRANCE);
		syFieldEventSetMessageId((s32)MsgGarohWhatOutsidersHere);
		fdActorFaceDirectionAndWait(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE,
									FACING_SOUTH_SOUTHWEST);
		fdActorRunRepeatedMotion(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE, 2);
		syFieldEventWait(20);
		fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
		/* "What!? Outsiders, here?" */
		syFieldEventShowMessageThenWait(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE, 0, 10);
		scFieldActorShowEmote(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE,
							  EMOTE_IN_FRONT | EMOTE_WINCE, 50);
		fdActorGet(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE)->steeringFlg &=
			~ACTOR_FACES_MOTION;
		fdActorFaceDirection(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE, facing, 0);
		syFieldEventWait(30);
		scGarohOpenCaveEntrance(ACTOR_CAVE_ENTRANCE);
		syFieldEventWait(20);
		fdActorWalkByAndWait(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE, 0, -16);
		fdActorGet(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE)->motionFlg = flag;
		fdActorWalkByAndWait(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE, 0, -16);
		fdActorSetPos(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE, 0, 0);
		syFieldEventWait(10);
		scGarohCloseCaveEntrance(ACTOR_CAVE_ENTRANCE);
		syFieldEventWait(60);
		fdActorSetPos(ACTOR_CAVE_ENTRANCE, 0, 0);
		fdEventEnd();
	}
}

extern TRIGGER Garoh_Triggers[];
extern TRIGGER Garoh_VillageTriggers[];
extern TRIGGER Garoh_VillageDayTriggers[];
extern TRIGGER Garoh_PathTriggers[];
extern TRIGGER Garoh_TunnelsTriggers[];
extern TRIGGER Garoh_WerewolfCaveTriggers[];

extern s32 Garoh_HowlFrames;

/* Walking north into an opening Reveal has shown takes the party through it: */
/* the cleft at the foot of the big grey rock north-east of the sanctum, cell */
/* (22, 11), leads down into the werewolves' cave, and the passage behind the */
/* round grey rock in the cave leads on. */
void scGarohEnterOpening(void) {
	FIELD_STATE *event = Ram_HeapSlots->fieldState;

	fdEventPlaySnd(SE_MAP_EXIT);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdEventSetFlag(FLAG_ARRIVAL_STAIRS_DOWN);
	syFieldEventSetExitRequest(event->stepTrigger);
}

/* Walking north to cells (19..21, 15) of the werewolves' cave, below where */
/* Maha stands: the first night the werewolves surround the party, and by day */
/* Maha is waiting there to tell the legend. */
void scGarohMeetInTheCave(void) {
	if(!fdEventGetFlag(FLAG_GAROH_SECRET_SHARED)) {
		if(!fdEventGetFlag(FLAG_GAROH_MET_WEREWOLVES))
			scGarohWerewolvesRevealThemselves();
	}
	else {
		if(!fdEventGetFlag(FLAG_GAROH_HEARD_MAHA))
			scGarohMahaTellsTheLegend();
	}
}

/* The triggers of the scene being entered: in the village, the night's until */
/* Maha has shared the secret and the day's after. */
const void *scGetTriggers(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_GAROH) {
		if(fdEventGetFlag(FLAG_GAROH_SECRET_SHARED))
			return Garoh_VillageDayTriggers;
		return Garoh_VillageTriggers;
	}
	if(scene == SCENE_GAROH_PATH)
		return Garoh_PathTriggers;
	if(scene == SCENE_GAROH_WAY_UP)
		return Garoh_WayUpTriggers;
	if(scene == SCENE_GAROH_TUNNELS)
		return Garoh_TunnelsTriggers;
	if(scene == SCENE_GAROH_WEREWOLF_CAVE)
		return Garoh_WerewolfCaveTriggers;
	return Garoh_Triggers;
}

/* A howl comes each time the night's frames under this mask are 0. */
#define HOWL_PERIOD_MASK 0x3ff

/* A wolf howls in the distance every 1,024 frames while it is night. */
void scGarohHowlAtNight(void) {
	if(!fdEventGetFlag(FLAG_GAROH_SECRET_SHARED) &&
	   fdEventGetFlag(FLAG_GAROH_NIGHT)) {
		Garoh_HowlFrames++;
		if((Garoh_HowlFrames & HOWL_PERIOD_MASK) == 0)
			fdEventPlaySnd(SE_HOWL);
	}
}

void scGarohInitPath(void);

/* Sets up the scene being entered: the cave's lamps, and Maha once he has */
/* told the legend; the village by night or day; the path up to the village; */
/* or the way up, which also makes the sanctum the place a defeat returns the */
/* party to once they have reached the village. */
s32 scInit(void) {
	FIELD_STATE *state;

	if(MainRoutineStateWork.next == SCENE_GAROH_TUNNELS) {
		FieldStateWork->transition = SCENE_TRANSITION(
			TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	}
	else if(MainRoutineStateWork.next == SCENE_GAROH_WEREWOLF_CAVE) {
		scFieldActorSetSprPriority(ACTOR_FIRE_A, 1);
		scFieldActorSetSprPriority(ACTOR_FIRE_B, 1);
		if(fdEventGetFlag(FLAG_GAROH_HEARD_MAHA)) {
			fdActorSetPosAndFacing(ACTOR_MAHA, toFIXED(328), toFIXED(162),
								   FACING_SOUTH);
		}
		FieldStateWork->transition = SCENE_TRANSITION(
			TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
		fdEffectStartBlankOverlay(0);
		if(MainRoutineStateWork.entrance ==
		   GAROH_WEREWOLF_CAVE_FROM_ROOM_BEHIND_ROCK) {
			state = Ram_HeapSlots->fieldState;
			state->transition =
				SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
			fdEventClearFlag(FLAG_ARRIVAL_STAIRS_UP);
			fdEventClearFlag(FLAG_ARRIVAL_STAIRS_DOWN);
			FieldStateWork->transition = SCENE_TRANSITION(
				TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
			fdEventStartScrTransition();
			fdEventWaitTransitionFrames();
		}
	}
	else {
		FieldStateWork->transition = 0;
		if(MainRoutineStateWork.next == SCENE_GAROH) {
			fdEventSetFlag(FLAG_GAROH_REACHED_VILLAGE);
			scGarohInitVillage();
		}
		else if(MainRoutineStateWork.next == SCENE_GAROH_PATH) {
			scGarohInitPath();
		}
		else if(MainRoutineStateWork.next == SCENE_GAROH_WAY_UP) {
			scGarohInitWayUp();
			if(fdEventGetFlag(FLAG_GAROH_REACHED_VILLAGE)) {
				MainRoutineStateWork.restartScene = SCENE_GAROH_ROOMS_SANCTUM;
				MainRoutineStateWork.restartEntrance =
					GAROH_ROOMS_SANCTUM_FROM_GAROH;
			}
		}
	}
	return 0;
}

s32 scPrepareMap(void) { return 0; }

/* The village: by night darkened, its windows lit and the wolves howling; */
/* by day the cleft in the big grey rock and the cave's other ways stand */
/* open. */
void scGarohInitVillage(void) {
	syFieldTaskAddFrame(scGarohHowlAtNight, TASK_PASS_FRAME);
	FieldStateWork->transition = 0;
	if(fdEventGetFlag(FLAG_GAROH_SECRET_SHARED) == 0 ||
	   fdEventGetFlag(FLAG_GAROH_NIGHT) != 0) {
		fdEventSetFlag(FLAG_GAROH_NIGHT);
		fdPaletteFadeApplyTarget(PALETTE_TINT_WARM_DARK, PALETTES_BG);
		if(fdEventGetFlag(FLAG_NIGHT_TINTS_ACTORS))
			fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, PALETTES_OBJECTS);
		fdPaletteFadeInterpolate(1);
		fdEventWaitTransitionFrames();
		fdPaletteFadePackTarget();
		{
			PALETTE_FADE *fade = Ram_HeapSlots->palFade;

			*(volatile u16 *)&BG_PLTT_COLOR(1, WINDOW_LIGHT) = RGB_WHITE;
			fade->pal[PALETTE_COLORS + WINDOW_LIGHT] =
				*(volatile u16 *)&BG_PLTT_COLOR(1, WINDOW_LIGHT);
		}
		fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, 1);
	}
	else {
		fdMapGridCopyCellsTo(10, 32, 10, 10, 1, 1);
		fdMapGridCopyCellsTo(12, 32, 25, 22, 1, 1);
		fdMapGridCopyCellsTo(11, 33, 8, 22, 1, 1);
		fdMapGridCopyCellsTo(74, 32, 74, 10, 1, 1);
		fdMapGridCopyCellsTo(76, 32, 89, 22, 1, 1);
		fdMapGridCopyCellsTo(75, 33, 72, 22, 1, 1);
	}
	if(MainRoutineStateWork.entrance == GAROH_FROM_GAROH_WEREWOLF_CAVE) {
		FIELD_STATE *state = Ram_HeapSlots->fieldState;

		state->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
		fdEventClearFlag(FLAG_ARRIVAL_STAIRS_UP);
		fdEventClearFlag(FLAG_ARRIVAL_STAIRS_DOWN);
		fdEventStartScrTransition();
		fdEventWaitTransitionFrames();
		FieldStateWork->transition = 0;
	}
}

/* The path up to the village: the tightrope strung, and by night its dark */
/* colours and rippling water, by day its hidden ways open. */
void scGarohInitPath(void) {
	syTaskAddFrame(scGarohHowlAtNight, TASK_PASS_FRAME);
	fdTightropeInit();
	fdTightropePlace(0, ACTOR_PATH_TIGHTROPE_POST_A,
					 ACTOR_PATH_TIGHTROPE_POST_B);
	if(fdEventGetFlag(FLAG_GAROH_NIGHT)) {
		if(MainRoutineStateWork.entrance == ENTRANCE_PATH_PURPLE_NIGHT) {
			fdPaletteFadeApplyTarget(PALETTE_TINT_CUSTOM | RGB(9, 6, 18), 1);
		}
		else {
			fdPaletteFadeApplyTarget(PALETTE_TINT_WARM_DARK, 1);
		}
		fdPaletteFadeInterpolate(1);
		fdEventWaitTransitionFrames();
		fdPaletteFadePackTarget();
		scGarohLoadPathNightCols();
		fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, 1);
		syTaskAddFrame((s32)scGarohWavePathLayer, TASK_PASS_VBLANK);
	}
	else {
		fdMapGridCopyCellsTo(20, 7, 9, 7, 5, 5);
		fdMapGridCopyCellsTo(20, 71, 9, 71, 5, 5);
		fdMapGridCopyCellsTo(84, 7, 73, 7, 5, 5);
		scGarohLoadPathDayCols();
		fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, 1);
		scGarohMatchSecondLayer();
	}
}

extern s16 Garoh_Pillars[];

/* The way up from the world map: the four stone pillars where they were */
/* left, in the evening's or the night's light once the party has climbed */
/* into it. */
void scGarohInitWayUp(void) {
	syFieldTaskAddFrame(scGarohHowlAtNight, TASK_PASS_FRAME);
	if(fdEventGetFlag(FLAG_GAROH_NIGHT)) {
		if(fdEventGetFlag(FLAG_EVENING))
			fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, 1);
		else
			fdPaletteFadeApplyTarget(PALETTE_TINT_WARM_DARK, 1);
	}
	fdActorPlaceBlocks(Garoh_Pillars);
	fdPaletteFadeInterpolate(1);
	fdEventWaitTransitionFrames();
}

/* Nothing visible happens as Felix starts casting Move on one of the stone */
/* pillars on the way up; the game lifts the pillar off the map's cells so */
/* that Move can slide it. */
void scGarohLiftPillar(void) { fdActorLiftTargetBlock(Garoh_Pillars); }

/* Nothing visible happens as Felix starts pushing one of the stone pillars on
 */
/* the way up; as with Move, the game first lifts the pillar off the map's */
/* cells so that it can slide. */
void scGarohLiftPushedPillar(void) { fdActorLiftTargetBlock(Garoh_Pillars); }

/* A pillar set down on its slot sinks 32 pixels into the ground with a whoosh
 */
/* and a ring of dust. */
void scGarohSinkPillar(s32 id, ACTOR *actor) {
	s32 i;
	s32 saved;

	actor->motionFlg = 0;
	saved = id;
	for(i = 16; i > 0; i--) {
		actor->y.fixed -= toFIXED(2);
		syFieldEventWait(1);
	}
	fdEventPlaySnd(SE_WHOOSH);
	scGarohScatterDustRing(saved);
}

/* Defines a check that sinks pillar actorId (scGarohSinkPillar) once it stands
 */
/* on cell (cellX, cellZ), and sets flag so it sinks once. */
#define DEFINE_GAROH_TRIGGER_AT_CELL(name, actorId, flag, cellX, cellZ)        \
	void name(void) {                                                          \
		ACTOR *actor = fdActorGet(actorId);                                    \
		s32 x = FIELD_CELL(actor->x.fixed);                                    \
		s32 raw;                                                               \
		s32 z;                                                                 \
                                                                               \
		raw = actor->z.fixed;                                                  \
		z = FIELD_CELL(raw);                                                   \
		if(!fdEventGetFlag(flag) && x == cellX && z == cellZ) {                \
			scGarohSinkPillar(actorId, actor);                                 \
			fdEventSetFlag(flag);                                              \
		}                                                                      \
	}

/* The first pillar sinks once it stands on cell (10, 29), one west of where it
 */
/* starts. */
DEFINE_GAROH_TRIGGER_AT_CELL(scGarohSinkFirstPillarOnCell, ACTOR_FIRST_PILLAR,
							 FLAG_FIRST_PILLAR_SUNK, 10, 29)

/* The second pillar sinks once it stands on cell (15, 28), one east of where */
/* it starts. */
DEFINE_GAROH_TRIGGER_AT_CELL(scGarohSinkSecondPillarOnCell, ACTOR_SECOND_PILLAR,
							 FLAG_SECOND_PILLAR_SUNK, 15, 28)

/* The third pillar has no slot to sink into: its check reads the pillar and */
/* the rope's flag and does nothing. */
void scGarohCheckThirdPillar(void) {
	fdActorGet(ACTOR_THIRD_PILLAR);
	fdEventGetFlag(FLAG_GAROH_ROPE_LASHED);
}

/* The fourth pillar sinks once it stands on cell (15, 25), one south of where
 */
/* it starts. */
void scGarohSinkFourthPillarOnCell(void) {
	ACTOR *actor = fdActorGet(ACTOR_FOURTH_PILLAR);
	s32 x = FIELD_CELL(actor->x.fixed);
	s32 z = FIELD_CELL(actor->z.fixed);

	if(!fdEventGetFlag(FLAG_FOURTH_PILLAR_SUNK) && x == 15 && z == 25) {
		scGarohSinkPillar(ACTOR_FOURTH_PILLAR, actor);
		fdEventSetFlag(FLAG_FOURTH_PILLAR_SUNK);
	}
}

/* Move on the first stone pillar, on the lowest ledge of the way up at cell */
/* (11, 29): slid one cell west, onto cell (10, 29), it drops off the ledge and
 */
/* sinks into the ground with a whoosh and a ring of dust until its top is */
/* level with the ledge, so that Felix can walk west off the ledge onto it. */
/* From the ledge its top is a dead end: holding west or south there takes him
 */
/* no further, and he can only step back east onto the ledge. The second */
/* pillar's top is the same (scGarohPlaceSecondPillar). */
void scGarohPlaceFirstPillar(void) {
	fdActorPlaceMovedBlock();
	scGarohSinkFirstPillarOnCell();
}

/* Move on the second stone pillar, east of the first at cell (14, 28): slid */
/* one cell east, onto cell (15, 28), it drops off its ledge onto the path */
/* below and sinks into the ground with a whoosh and a ring of dust until its */
/* top is level with the ledge. Felix can walk east onto its top, but holding */
/* east or south there takes him no further. */
void scGarohPlaceSecondPillar(void) {
	fdActorPlaceMovedBlock();
	scGarohSinkSecondPillarOnCell();
}

/* Move on the third stone pillar, on the ledge above at cell (10, 24): it */
/* slides where Move sends it, and has no slot to sink into. */
void scGarohPlaceThirdPillar(void) {
	fdActorPlaceMovedBlock();
	scGarohCheckThirdPillar();
}

/* Move on the fourth stone pillar, east of the third at cell (15, 24): cast */
/* from the ledge north of it and slid one cell south, onto cell (15, 25), it */
/* drops off the ledge onto the path below and sinks into the ground with a */
/* whoosh and a ring of dust until its top is level with the ledge. Felix can */
/* walk south onto its top, but like the first two it is a dead end: holding */
/* south, west or east there takes him no further. */
void scGarohPlaceFourthPillar(void) {
	fdActorPlaceMovedBlock();
	scGarohSinkFourthPillarOnCell();
}

/* Pushing a stone pillar on the way up sinks it just as Move does when it */
/* comes to rest on its slot: the first on cell (10, 29), the second on */
/* (15, 28), the fourth on (15, 25). */
/* */
/* The push rows answer only while the first pillar has not sunk. */
void scGarohPlacePushedPillar(void) {
	fdActorPlaceMovedBlock();
	scGarohSinkFirstPillarOnCell();
	scGarohSinkSecondPillarOnCell();
	scGarohCheckThirdPillar();
	scGarohSinkFourthPillarOnCell();
}

/* Reveal in the village shows the cleft at the foot of the big grey rock */
/* north-east of the sanctum, cell (22, 11), as a way in. */
void scGarohRevealCaveEntrance(void) {
	fdMapGridCopyCellsTo(98, 8, 86, 8, 1, 2);
	fdMapGridCopyCellAttributes(34, 9, 1, 1, 22, 11);
}

/* Reveal in the tunnels shows a hidden passage at cells (11, 33..35). */
void scGarohRevealTunnelPassage(void) {
	fdMapGridCopyCellsTo(25, 47, 11, 33, 1, 3);
	fdMapGridCopyCellAttributes(25, 47, 1, 1, 11, 33);
}

/* Reveal in the werewolves' cave shows a passage through the round grey rock */
/* in its middle, cells (19..21, 5..7). */
void scGarohRevealCavePassage(void) {
	fdMapGridCopyCellsTo(44, 5, 19, 5, 3, 3);
	fdMapGridCopyCellsTo(108, 5, 83, 5, 3, 2);
	fdMapGridCopyCellAttributes(44, 5, 3, 3, 19, 5);
}

/* Reveal wearing off in the village hides the cleft in the big grey rock */
/* again. */
void scGarohHideCaveEntrance(void) {
	fdMapGridCopyCellsTo(102, 8, 86, 8, 1, 2);
	fdMapGridCopyCellAttributes(22, 10, 1, 1, 22, 11);
}

/* Reveal wearing off on the path up to the village: at night the path's dark */
/* colours, there before the cast, come back exactly as they were. This row */
/* plays no part in it: it only checks whether it is night in Garoh, and does */
/* nothing with the answer. */
s32 scGarohIsNight(void) { return fdEventGetFlag(FLAG_GAROH_NIGHT); }

/* Reveal wearing off on the way up brings back the evening's or the night's */
/* light. */
void scGarohRestoreWayUpLight(void) {
	if(fdEventGetFlag(FLAG_GAROH_NIGHT)) {
		if(fdEventGetFlag(FLAG_EVENING))
			fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, 1);
		else
			fdPaletteFadeApplyTarget(PALETTE_TINT_WARM_DARK, 1);
		fdPaletteFadeInterpolate(16);
	}
}

/* Reveal wearing off in the tunnels hides their passage again. */
void scGarohHideTunnelPassage(void) {
	fdMapGridCopyCellsTo(8, 30, 11, 33, 1, 3);
	fdMapGridCopyCellAttributes(8, 30, 1, 1, 11, 33);
}

/* Reveal wearing off in the werewolves' cave closes the round grey rock */
/* again. */
void scGarohHideCavePassage(void) {
	fdMapGridCopyCellsTo(39, 5, 19, 5, 3, 3);
	fdMapGridCopyCellsTo(103, 5, 83, 5, 3, 2);
	fdMapGridCopyCellAttributes(39, 5, 3, 3, 19, 5);
}

/* Climbing the way up from the world map before the werewolves' secret is */
/* shared, the step onto cell (13, 26), the lower of its two lines, turns the */
/* daylight to a golden evening; coming down from the path, the same step */
/* brings the daylight back. Once a visit. */
void scGarohCrossLowerTwilightLine(void) {
	FIELD_STATE *state;

	if(!fdEventGetFlag(FLAG_GAROH_SECRET_SHARED)) {
		fdEventSetFlag(FLAG_CROSSED_LOWER_TWILIGHT_LINE);
		if(MainRoutineStateWork.entrance == GAROH_WAY_UP_FROM_WORLD_MAP) {
			fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, 1);
			fdEventSetFlag(FLAG_GAROH_NIGHT);
			fdEventSetFlag(FLAG_EVENING);
		}
		else {
			fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, 1);
			fdEventClearFlag(FLAG_GAROH_NIGHT);
			fdEventClearFlag(FLAG_EVENING);
		}
		state = Ram_HeapSlots->fieldState;
		if(state->revealActive == 0)
			fdPaletteFadeInterpolate(120);
	}
}

/* Climbing on up past cell (16, 22), the upper of the way up's two lines, */
/* the evening darkens into night; coming down from the path, the same step */
/* turns the night to evening. Once a visit, before the werewolves' secret is */
/* shared. */
void scGarohCrossUpperTwilightLine(void) {
	FIELD_STATE *state;

	if(!fdEventGetFlag(FLAG_GAROH_SECRET_SHARED)) {
		fdEventSetFlag(FLAG_CROSSED_UPPER_TWILIGHT_LINE);
		if(MainRoutineStateWork.entrance == GAROH_WAY_UP_FROM_GAROH_PATH) {
			fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, 1);
			fdEventSetFlag(FLAG_GAROH_NIGHT);
			fdEventSetFlag(FLAG_EVENING);
		}
		else {
			fdPaletteFadeApplyTarget(PALETTE_TINT_WARM_DARK, 1);
			fdEventSetFlag(FLAG_GAROH_NIGHT);
			fdEventClearFlag(FLAG_EVENING);
		}
		state = Ram_HeapSlots->fieldState;
		if(state->revealActive == 0)
			fdPaletteFadeInterpolate(120);
	}
}

#include "../field/field_effect_advance_motion.c"

#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef DEFINE_GAROH_LOAD_PALETTE
#undef DEFINE_GAROH_TRIGGER_AT_CELL
void scFieldEffectSpawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
						s32 velocityZ, u32 flags,
						const EFFECT_OPTIONS *options);

/* A ring of puffs of dust spreads out from the actor, one every eighth of a */
/* turn: nine, the first and last both to the east. */
void scGarohScatterDustRing(s32 objectId) {
	ACTOR *actor;
	VECTOR3 vec;
	EFFECT_OPTIONS params;
	u32 angleStep;
	s32 angle;
	s32 x;
	FIXED z;

	actor = fdActorGet(objectId);
	params.update = fdEffectAdvanceMotion;
	angleStep = 0;
	do {
		angle = angleStep * (TRIG_TURN / DUST_RING_PUFFS);
		vec.x = syCos(angle);
		vec.y = 0;
		z = syMathSin(angle);
		x = vec.x;
		vec.z = z;
		x += x / 3;
		vec.x = x;
		scFieldEffectSpawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, x,
						   vec.y, z, EFFECT_USE_UPDATE | 1, &params);
		angleStep += 2;
	} while(angleStep <= DUST_RING_PUFFS);
}

extern u32 Garoh_QuestionMarkScript[];

/* Unused: Lash ties a rope between the third and fourth stone pillars on the */
/* way up; no trigger runs it. */
void scGarohLashRope(void) {
	fdRopeLash(1, ACTOR_THIRD_PILLAR, ACTOR_FOURTH_PILLAR);
	fdEventSetFlag(FLAG_GAROH_ROPE_LASHED);
}

/* Until the secret is shared, whether the party has met the werewolves. */
s32 scGarohMetWerewolves(void) {
	if(!fdEventGetFlag(FLAG_GAROH_SECRET_SHARED))
		return fdEventGetFlag(FLAG_GAROH_MET_WEREWOLVES);
}

/* Pops a "?" bubble up at (x, y, z), tilted by rotation, for the given */
/* frames. */
void scGarohPopQuestionMark(x, y, z, rotation, frames) s32 x, y, z;
s16 rotation;
s32 frames;
{
	ACTOR *object;
	SPR *sprite;

	object = scFieldActorCreate(SPRITE_EMOTE, x, y, z);
	if(object != NULL) {
		sprite = object->spr;
		fdActorSelectAnimation(object, EMOTE_QUESTION);
		fdActorStartScript(object, Garoh_QuestionMarkScript);
		object->motionFlg = 0;
		object->wait = frames;
		sprite->flg = 0;
		sprite->rotation = rotation;
	}
}

/* Two question marks pop up over the actor's head in turn, right and left, */
/* each tilted, a third of a second apart. */
void scGarohPopQuestionMarks(s32 id) {
	ACTOR *actor = fdActorGet(id);

	scGarohPopQuestionMark(actor->x.fixed + toFIXED(8),
						   actor->y.fixed + toFIXED(28), actor->z.fixed,
						   TRIG_TURN / 16, 20);
	fdEventPlaySnd(SE_LIGHT_APPEAR);
	syFieldEventWait(20);
	scGarohPopQuestionMark(actor->x.fixed - toFIXED(8),
						   actor->y.fixed + toFIXED(28), actor->z.fixed,
						   TRIG_TURN * 15 / 16, 20);
	fdEventPlaySnd(SE_LIGHT_APPEAR);
	syFieldEventWait(20);
}

/* Defines Reveal's effect played on actor at the cave's passage: the effect's
 */
/* phase, and the passage's 3 by 3 cells redrawn from column fromX. */
#define DEFINE_GAROH_PLAY_EFFECT(name, phase, fromX)                           \
	void name(s32 actor) {                                                     \
		fdPsynergyLoadResources(ABILITY_REVEAL, 1);                            \
		fdPsynergySetupActorPair(actor, 0);                                    \
		fdPsynergyRunHandler(phase);                                           \
		fdMapGridCopyCellsLayered(fromX, 5, 3, 3, 19, 5);                      \
		fdBattleEffectStopMainActor();                                         \
		fdPsynergyEnd();                                                       \
	}

/* Reveal's effect played on the cave's passage as it opens. */
DEFINE_GAROH_PLAY_EFFECT(scGarohOpenCavePassage, 1, 44)

/* Reveal's effect played on the cave's passage as it closes. */
DEFINE_GAROH_PLAY_EFFECT(scGarohCloseCavePassage, 3, 39)

/* Sends an actor walking to where the leader stands, if there is a leader. */
static inline void scWalkTowardLeader(s32 actorId) {
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);

	if(leader != NULL) {
		fdActorSetDestination(actorId, leader->x.part[0].integer,
							  leader->z.part[0].integer);
	}
}

/* An actor walks into the leader and is taken off the map, as the party */
/* closes up into one walking sprite at the end of an event. */
static inline void scWalkIntoLeader(s32 actorId) {
	ACTOR *leader;

	fdActorSetAnimation(actorId, ANIM_WALK);
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	if(leader != NULL) {
		fdActorSetDestination(actorId, leader->x.part[0].integer,
							  leader->z.part[0].integer);
	}
	scFieldActorWaitForMoveById(actorId);
	fdActorSetPos(actorId, 0, 0);
}

/* The point i eighths of the way from from to to. */
static inline s32 scGarohPathStep(s32 from, s32 to, s32 i) {
	return from + ((to - from) * i) / 8;
}

/* How far, in pixels, the werewolf weaves either side of the path as it runs */
/* past: across the screen, and up and down it. */
enum { WEAVE_ACROSS = 12, WEAVE_DOWN = 7 };

/* Walking up the path to the village the first night, cells (9..11, 19): a */
/* werewolf howls "Gaaaroooooo!" and bounds down the path past the pond and */
/* off into the village, and Kraden explains to the startled party what a */
/* werewolf is. */
void scGarohWerewolfRunsPast(void) {
	ACTOR *werewolf;
	s32 i;
	s32 position[3];
	s32 *pos;
	FIXED x, z;
	FIXED toX, toZ;

	fdEventSetFlag(FLAG_GAROH_SAW_WEREWOLF);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdActorSetPos(ACTOR_WEREWOLF, 96, 8);
	syFieldEventSetMessageId((s32)MsgGarohGaaaroooooo);
	/* "Gaaaroooooo!" */
	syFieldEventShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorSetSpeed(ACTOR_FELIX, 0.8);
	fdActorWalkToAndWait(ACTOR_FELIX, 152, 304);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, 32, -16, FACING_NORTH_NORTHWEST);
	scFieldActorLaunchFromLeader(ACTOR_JENNA, 0, 0, FACING_NORTH);
	scFieldActorLaunchFromLeader(ACTOR_SHEBA, 16, 0, FACING_NORTH_NORTHWEST);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		scFieldActorLaunchFromLeader(ACTOR_PIERS, 8, 16, FACING_NORTH);
	fdActorWalkByAndWait(ACTOR_FELIX, -16, -16);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	scFieldActorWaitForMoveById(ACTOR_KRADEN);
	syFieldEventWait(20);
	/* "Aow-aow-oooo!!!" */
	syFieldEventShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	syFieldEventWait(25);
	/* "What was that!?" */
	syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	/* "Awoooooo!!!" */
	syFieldEventShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		scFieldActorStartRepeatedMotion(ACTOR_PIERS, 2);
	scFieldActorStartRepeatedMotion(ACTOR_SHEBA, 2);
	scFieldActorStartRepeatedMotion(ACTOR_KRADEN, 2);
	scFieldActorStartRepeatedMotion(ACTOR_FELIX, 2);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	syFieldEventWait(20);
	fdCameraMoveTo(144, CAMERA_KEEP, 128, 1);
	fdCameraWaitForArrival();
	werewolf = fdActorGet(ACTOR_WEREWOLF);
	fdActorSetAnimation(ACTOR_WEREWOLF, ANIM_WALK);
	/* The werewolf lopes down the path in eight bounds at random speeds, */
	/* weaving from side to side. */
	for(i = 0; i <= 7; i++) {
		s32 tenths;
		s32 wave;

		wave = syMathSin(i * (DEGtoANG(45)));
		pos = position;
		pos[0] =
			scGarohPathStep(toFIXED(104), toFIXED(88), i) - wave * WEAVE_ACROSS;
		wave = syMathSin(i * (DEGtoANG(45)));
		position[2] =
			scGarohPathStep(toFIXED(32), toFIXED(120), i) - wave * WEAVE_DOWN;
		position[1] = 0;
		tenths = syRandomBelow(10) + 4;
		fdActorSetSpeedFX(ACTOR_WEREWOLF, tenths * (toFIXED(1) / 10),
						  tenths * (toFIXED(1) / 20));
		fdActorWalkToAndWait(ACTOR_WEREWOLF, position[0] / toFIXED(1),
							 position[2] / toFIXED(1));
	}
	fdActorSetAnimation(ACTOR_WEREWOLF, ANIM_STAND);
	syFieldEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_WEREWOLF, 2);
	fdActorSetScript(ACTOR_WEREWOLF, Garoh_WerewolfLookAroundScript);
	FieldStateWork->msg--;
	/* "Awoooooo!!!" */
	syFieldEventShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	scFieldActorStop(ACTOR_WEREWOLF);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_SOUTHEAST, 0);
	syFieldEventWait(20);
	syEventCameraFollowActor(ACTOR_WEREWOLF, 1);
	fdActorSetAnimation(ACTOR_WEREWOLF, ANIM_WALK);
	x = werewolf->x.fixed;
	z = werewolf->z.fixed;
	toX = toFIXED(128);
	toZ = toFIXED(184);
	for(i = 0; i <= 7; i++) {
		s32 tenths;
		FIXED wave;

		wave = syMathSin(i * (DEGtoANG(45)));
		position[0] = scGarohPathStep(x, toX, i) - wave * 4;
		position[2] = scGarohPathStep(z, toZ, i);
		position[1] = 0;
		tenths = syRandomBelow(10) + 4;
		fdActorSetSpeedFX(ACTOR_WEREWOLF, tenths * (toFIXED(1) / 10),
						  tenths * (toFIXED(1) / 20));
		fdActorWalkToAndWait(ACTOR_WEREWOLF, position[0] / toFIXED(1),
							 position[2] / toFIXED(1));
	}
	fdActorSetAnimation(ACTOR_WEREWOLF, ANIM_STAND);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_SOUTH_SOUTHEAST, 0);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_EAST, 0);
	syFieldEventWait(30);
	/* "Ah-ah-awoooo!!!" */
	syFieldEventShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	/* "Eeek!" */
	syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_WEREWOLF, 2);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_SOUTH_SOUTHEAST, 0);
	syFieldEventWait(30);
	fdActorShowEmote(ACTOR_WEREWOLF, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
	fdActorSetSpeed(ACTOR_WEREWOLF, 0.6);
	fdActorGet(ACTOR_WEREWOLF)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_WEREWOLF, 128, 148);
	syFieldEventWait(1);
	fdActorGet(ACTOR_WEREWOLF)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorSetSpeed(ACTOR_WEREWOLF, 2.5);
	fdActorWalkByAndWait(ACTOR_WEREWOLF, 0, 4);
	syFieldEventWait(20);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_NORTH, 0);
	syFieldEventWait(20);
	scFieldActorStartRepeatedMotion(ACTOR_WEREWOLF, 3);
	fdActorSetAttachedEffect(ACTOR_WEREWOLF,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(40);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_WEST, 0);
	syFieldEventWait(10);
	fdActorSetSpeed(ACTOR_WEREWOLF, 2.4);
	fdActorWalkByAndWait(ACTOR_WEREWOLF, -24, -8);
	fdActorWalkByAndWait(ACTOR_WEREWOLF, -8, -100);
	syFieldEventWait(30);
	fdActorSetPos(ACTOR_WEREWOLF, 0, 0);
	fdCameraMoveTo(168, CAMERA_KEEP, 280, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(10);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	scFieldActorJump(ACTOR_KRADEN, 6, 15);
	scFieldActorJump(ACTOR_KRADEN, 6, 23);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldEventWait(10);
	/* "I don't believe it! Was that a°" */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH_NORTHEAST, 0);
	syFieldEventWait(25);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	syFieldEventWait(10);
	/* "A what? Kraden, do you know what that thing was?" */
	syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(25);
	/* "Well... Perhaps..." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	syFieldEventWait(10);
	/* "So, what are you waiting for!? Tell us!" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "I suspect it may have been...a lycanthrope." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	syFieldEventWait(10);
	/* "A lycanthwhat?" */
	syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldEventWait(10);
	/* "Just think of them as a race of people born with special powers..." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	syFieldEventWait(10);
	/* "Kind of like Adepts, you mean?" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "Yes, that sounds right. Sheba, you've put it quite well." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldEventWait(10);
	/* "Adepts borrow their abilities from the power of the elements..." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldEventWait(10);
	/* "While lycanthropes borrow theirs from the power of beasts..." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
	syFieldEventWait(30);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "This is why they take the forms of animals..." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD, 33);
	/* "They may look frightening... But we must try to communicate with them."
	 */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorWalkByAndWait(ACTOR_KRADEN, -8, -8);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorWalkByAndWait(ACTOR_KRADEN, -10, -6);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorWalkByAndWait(ACTOR_KRADEN, -14, -4);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	syFieldEventWait(30);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	syFieldEventWait(20);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldEventWait(10);
	/* "Lycanthropes... Werewolves... A whole village in hiding..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
									0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH_NORTHWEST, 0);
	syFieldEventWait(20);
	/* "Is it just me... Or does Kraden seem a little TOO happy to have found */
	/* werewolves?" Yes: "What? Me? Happy to find werewolves? You must be
	 * joking!" */
	/* No: "I would love to study them, but still...they do frighten me..." */
	fdEventOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
	if(syFieldEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		syFieldEventWait(30);
		fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
		syFieldEventWait(20);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
		syFieldEventWait(20);
		if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
			fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
		/* "What? Me? Happy to find werewolves? You must be joking!" */
		syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN,
										0, 0);
		FieldStateWork->msg++;
	}
	else {
		syFieldEventWait(40);
		fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
		syFieldEventWait(20);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
		syFieldEventWait(20);
		if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
			fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
		FieldStateWork->msg++;
		/* "I would love to study them, but still...they do frighten me..." */
		syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN,
										0, 0);
	}
	scFieldActorJump(ACTOR_KRADEN, 6, 15);
	scFieldActorJump(ACTOR_KRADEN, 6, 23);
	/* "I guess I'm just curious about all of the secrets of these new lands..."
	 */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
									0);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	syFieldEventWait(15);
	/* "Any real scholar would be thrilled to explore a new land full of
	 * mystery, */
	/* huh?" */
	syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
	syFieldEventWait(25);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldEventWait(8);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldEventWait(8);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldEventWait(20);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "So, are there werewolves only on Osenia?" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0, 0);
	scFieldActorStartRepeatedMotion(ACTOR_KRADEN, 3);
	fdActorSetAttachedEffect(ACTOR_KRADEN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(40);
	/* "Ummm..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
									0);
	scGarohPopQuestionMarks(ACTOR_SHEBA);
	syFieldEventWait(20);
	/* "If so, how could you even have known about them, Kraden?" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldEventWait(10);
	fdActorSetSpeed(ACTOR_KRADEN, 0.4);
	fdActorGet(ACTOR_KRADEN)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_KRADEN, 0, -16);
	syFieldEventWait(1);
	fdActorGet(ACTOR_KRADEN)->steeringFlg |= ACTOR_FACES_MOTION;
	syFieldEventWait(20);
	fdActorSetAttachedEffect(ACTOR_KRADEN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(40);
	/* "Er... I, uh..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
									0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	syFieldEventWait(20);
	/* "Yeah... That is weird." */
	syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH_NORTHWEST, 0);
	syFieldEventWait(20);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
	syFieldEventWait(35);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
	syFieldEventWait(25);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	syFieldEventWait(20);
	fdActorSetSpeed(ACTOR_KRADEN, 1);
	fdActorWalkByAndWait(ACTOR_KRADEN, 0, 16);
	syFieldEventWait(20);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	/* "All right, so I lied!" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
									0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldEventWait(10);
	/* "I'm glad we found werewolves! There! Are you happy!?" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
									0);
	fdActorSetSpeed(ACTOR_KRADEN, 2);
	fdActorWalkByAndWait(ACTOR_KRADEN, -16, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
	syFieldEventWait(20);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD, 33);
	/* "And you know what!? I even want to get a closer look if I can!" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
									0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	/* "So let's go! Let's find us some werewolves!" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
									0);
	scFieldActorJump(ACTOR_KRADEN, 6, 15);
	scFieldActorJump(ACTOR_KRADEN, 6, 23);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
	syFieldEventWait(40);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTHEAST);
	syFieldEventWait(20);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
	syFieldEventWait(30);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	scWalkIntoLeader(ACTOR_KRADEN);
	fdActorSetSpeed(ACTOR_JENNA, 1.2);
	scWalkIntoLeader(ACTOR_JENNA);
	fdActorSetSpeed(ACTOR_SHEBA, 1.2);
	scWalkIntoLeader(ACTOR_SHEBA);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		fdActorSetSpeed(ACTOR_PIERS, 1.2);
		scWalkIntoLeader(ACTOR_PIERS);
	}
	syEventCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdEventEnd();
}

/* Coming down into the werewolves' cave through the cleft by the sanctum, */
/* cells (17..18, 49): a howl echoes, the little werewolf from the path comes */
/* into sight, Kraden cries "Look! It's that werewolf!", and it looks around */
/* and runs off deeper into the cave; Kraden wants to follow it. */
void scGarohFollowWerewolfIntoCave(void) {
	fdEventSetFlag(FLAG_GAROH_FOLLOWED_WEREWOLF);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventSetMessageId((s32)MsgGarohHaowooo);
	fdActorSetPos(ACTOR_WEREWOLF, 144, 752);
	fdActorWalkToAndWait(ACTOR_FELIX, 280, 776);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	syFieldEventWait(10);
	/* "Haowooo!!!" */
	syFieldEventShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, 16, 0, FACING_NORTH_NORTHWEST);
	scFieldActorWaitForMoveById(ACTOR_KRADEN);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	syFieldEventWait(20);
	scFieldActorStartRepeatedMotion(ACTOR_KRADEN, 3);
	fdActorSetAttachedEffect(ACTOR_KRADEN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scFieldActorStartRepeatedMotion(ACTOR_FELIX, 3);
	fdActorSetAttachedEffect(ACTOR_FELIX,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(45);
	/* "Aow-aow-oooo!!!" */
	syFieldEventShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	syFieldEventWait(20);
	fdCameraMoveTo(280, CAMERA_KEEP, 752, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(20);
	fdActorSetPos(ACTOR_WEREWOLF, 144, 720);
	fdActorSetSpeed(ACTOR_WEREWOLF, 0.8);
	fdActorWalkToAndWait(ACTOR_WEREWOLF, 168, 720);
	syFieldEventWait(10);
	fdActorSetSpeed(ACTOR_WEREWOLF, 2);
	scFieldActorJump(ACTOR_WEREWOLF, 6, 0);
	fdActorWalkToAndWait(ACTOR_WEREWOLF, 200, 720);
	syFieldEventWait(10);
	fdActorSetSpeed(ACTOR_WEREWOLF, 0.8);
	fdActorWalkToAndWait(ACTOR_WEREWOLF, 216, 712);
	fdActorWalkToAndWait(ACTOR_WEREWOLF, 320, 712);
	/* "Look! It's that werewolf!" */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_WEREWOLF, 2);
	syFieldEventWait(10);
	fdActorSetSpeed(ACTOR_WEREWOLF, 0.6);
	fdActorGet(ACTOR_WEREWOLF)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_WEREWOLF, 296, 712);
	syFieldEventWait(1);
	fdActorGet(ACTOR_WEREWOLF)->steeringFlg |= ACTOR_FACES_MOTION;
	syFieldEventWait(15);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_WEST_NORTHWEST, 0);
	syFieldEventWait(25);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_EAST_NORTHEAST, 0);
	syFieldEventWait(25);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_WEST_NORTHWEST, 0);
	syFieldEventWait(25);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_SOUTH, 0);
	syFieldEventWait(25);
	fdActorShowEmote(ACTOR_WEREWOLF, EMOTE_IN_FRONT | EMOTE_WINCE, 45);
	scFieldActorJump(ACTOR_WEREWOLF, 6, 15);
	scFieldActorJump(ACTOR_WEREWOLF, 6, 23);
	fdActorSetSpeed(ACTOR_WEREWOLF, 2.4);
	fdActorWalkToAndWait(ACTOR_WEREWOLF, 200, 712);
	syFieldEventWait(3);
	fdActorSetSpeed(ACTOR_WEREWOLF, 2);
	scFieldActorJump(ACTOR_WEREWOLF, 6, 0);
	fdActorWalkToAndWait(ACTOR_WEREWOLF, 168, 712);
	syFieldEventWait(3);
	fdActorSetSpeed(ACTOR_WEREWOLF, 2.4);
	fdActorWalkToAndWait(ACTOR_WEREWOLF, 144, 712);
	fdActorSetPos(ACTOR_WEREWOLF, 0, 0);
	syFieldEventWait(30);
	syEventCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(20);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "He was right there!" */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	syFieldEventWait(20);
	/* "Let's go find him, Felix!" */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	syFieldEventWait(20);
	scFieldActorJump(ACTOR_KRADEN, 6, 15);
	scFieldActorJump(ACTOR_KRADEN, 6, 23);
	fdActorAnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
	syFieldEventWait(10);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	scWalkTowardLeader(ACTOR_KRADEN);
	scFieldActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdEventEnd();
}

/* The first night in the werewolves' cave, at cells (19..21, 15): Kraden */
/* wonders where the werewolf went, the werewolves surround the party, and */
/* their elder Maha speaks with them and tells them to come back later. */
void scGarohWerewolvesRevealThemselves(void) {
	fdEventSetFlag(FLAG_GAROH_MET_WEREWOLVES);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventSetMessageId((s32)MsgGarohWait);
	fdActorSetPos(ACTOR_MAHA, 328, 40);
	fdActorSetPos(ACTOR_WEREWOLF, 328, 40);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_SOUTH, 0);
	fdActorWalkToAndWait(ACTOR_FELIX, 320, 224);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	syFieldEventWait(10);
	/* "Felix, wait..." */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0, 0);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, -16, -8, FACING_NORTH);
	scFieldActorWaitForMoveById(ACTOR_KRADEN);
	syFieldEventWait(30);
	fdCameraSetSpeed(1);
	fdCameraMoveTo(328, CAMERA_KEEP, 152, 1);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 0);
	syFieldEventWait(25);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	syFieldEventWait(25);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 0);
	syFieldEventWait(25);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	syFieldEventWait(25);
	/* "Where could that werewolf have gone?" */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
	fdCameraMoveToActorById(ACTOR_FELIX, 1);
	fdCameraWaitForArrival();
	fdCameraSetSpeed(1.5);
	/* "Do you think we lost him somewhere?" Yes: "That would be a shame..." No:
	 */
	/* "I hope we didn't... Or rather, I'd be quite unhappy if we did..." */
	fdEventOpenMessage(ACTOR_KRADEN, 0);
	if(!syFieldEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
		syFieldEventWait(20);
		fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
		/* "That would be a shame..." */
		syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
		FieldStateWork->msg = (FieldStateWork->msg + 1);
	}
	else {
		syFieldEventWait(30);
		fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
		FieldStateWork->msg = (FieldStateWork->msg + 1);
		/* "I hope we didn't... Or rather, I'd be quite unhappy if we did..." */
		syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	}
	/* "Grrrr..." */
	syFieldEventShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_FELIX, 2);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	syFieldEventWait(20);
	scFieldActorLaunchFromLeader(ACTOR_JENNA, 16, 0, FACING_NORTH);
	scFieldActorLaunchFromLeader(ACTOR_SHEBA, 32, 0, FACING_NORTH);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		scFieldActorLaunchFromLeader(ACTOR_PIERS, 48, -8, FACING_NORTHWEST);
	}
	scFieldActorWaitForMoveById(ACTOR_SHEBA);
	syFieldEventWait(20);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "What was that?" */
	syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	syFieldEventWait(20);
	/* "It sounded like a wolf growling, didn't it?" */
	syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 0);
	syFieldEventWait(25);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	syFieldEventWait(25);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 0);
	syFieldEventWait(25);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	syFieldEventWait(25);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
	syFieldEventWait(25);
	/* "My hearing is not so very good, but... there was something." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	syFieldEventWait(40);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	syFieldEventWait(25);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
	syFieldEventWait(10);
	/* "Perhaps we should search this area more thoroughly..." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	/* "There is no need..." */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	fdCameraMoveTo(328, CAMERA_KEEP, 144, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(20);
	fdActorSetPos(ACTOR_CAVE_PASSAGE, 328, 136);
	fdActorSetSprFlags(fdActorGet(ACTOR_CAVE_PASSAGE), 0);
	scGarohOpenCavePassage(ACTOR_CAVE_PASSAGE);
	syFieldEventWait(20);
	/* "Look... He's using Psynergy." */
	syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorSetSpeed(ACTOR_WEREWOLF, 0.7);
	fdActorSetSpeed(ACTOR_MAHA, 0.7);
	fdActorSetPos(ACTOR_MAHA, 328, 128);
	syFieldEventWait(10);
	fdActorWalkByAndWait(ACTOR_MAHA, 0, 16);
	fdActorSetPos(ACTOR_WEREWOLF, 328, 128);
	syFieldEventWait(10);
	fdActorWalkBy(ACTOR_WEREWOLF, 0, 16);
	fdActorWalkByAndWait(ACTOR_MAHA, 0, 16);
	syFieldEventWait(10);
	scGarohCloseCavePassage(ACTOR_CAVE_PASSAGE);
	fdActorSetPos(ACTOR_CAVE_PASSAGE, 0, 0);
	syEventCameraFollowActor(ACTOR_MAHA, 1);
	syFieldEventWait(20);
	fdActorWalkBy(ACTOR_WEREWOLF, 0, 24);
	fdActorWalkByAndWait(ACTOR_MAHA, 0, 24);
	fdActorWalkByAndWait(ACTOR_WEREWOLF, 16, 8);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_SOUTH, 0);
	syFieldEventWait(20);
	fdCameraMoveTo(328, CAMERA_KEEP, 208, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_WEREWOLF, 2);
	syFieldEventWait(10);
	/* "Garrrrooow..." */
	syFieldEventShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	scFieldActorStartRepeatedMotion(ACTOR_KRADEN, 2);
	scFieldActorStartRepeatedMotion(ACTOR_SHEBA, 2);
	scFieldActorStartRepeatedMotion(ACTOR_JENNA, 2);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		scFieldActorStartRepeatedMotion(ACTOR_PIERS, 2);
	}
	fdActorRunRepeatedMotion(ACTOR_FELIX, 2);
	syFieldEventWait(20);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
	/* "Fear not... This child is harmless." */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "He does look kind of...small..." */
	syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	scFieldActorStartRepeatedMotion(ACTOR_WEREWOLF, 3);
	fdActorSetAttachedEffect(ACTOR_WEREWOLF,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(40);
	/* "Grrrrrrr..." */
	syFieldEventShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldEventWait(10);
	/* "Tell me, who are you? How did you come to be here?" */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	scFieldActorJump(ACTOR_WEREWOLF, 6, 15);
	scFieldActorJump(ACTOR_WEREWOLF, 6, 23);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_SOUTHWEST, 0);
	syFieldEventWait(20);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
	/* "I should like to ask you the same question." */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	}
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldEventWait(10);
	/* "Could it be? Do you outsiders possess the power to reveal the unseen?"
	 */
	/* Yes: "Of course... I was certain that you must." No: "Impossible...
	 * Without */
	/* Reveal, you could not have entered this cave." */
	fdEventOpenMessage(ACTOR_MAHA, 0);
	if(syFieldEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
		syFieldEventWait(40);
		fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
		/* "Impossible... Without Reveal, you could not have entered this cave."
		 */
		syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
		FieldStateWork->msg = (FieldStateWork->msg + 1);
	}
	else {
		syFieldEventWait(30);
		fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
		syFieldEventWait(10);
		FieldStateWork->msg = (FieldStateWork->msg + 1);
		/* "Of course... I was certain that you must." */
		syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	}
	fdActorRunRepeatedMotion(ACTOR_WEREWOLF, 2);
	syFieldEventWait(10);
	/* "Grawooo..." */
	syFieldEventShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_EAST, 0);
	syFieldEventWait(30);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	/* "Yes, you are correct. They must have passed the challenge of Air's
	 * Rock." */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "Er... How come that kid doesn't talk, like you?" */
	syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH, 0);
	syFieldEventWait(20);
	/* "When we become werewolves, speaking the human tongue becomes more */
	/* difficult." */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	syFieldEventWait(20);
	/* "But you can... You must be someone special, then..." */
	syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
	/* "Hurh hurr hrrr... 'Special'? Perhaps. Why don't you */
	/* just call me Maha..." */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "So... I assume you also can use Whirlwind..." */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	}
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	/* "If you learned Reveal on Air's Rock, you must also know Whirlwind." Yes:
	 */
	/* "That's right... I think we can tell Maha that we're Adepts." No: "I
	 * don't */
	/* think we need to hide the fact that we're Adepts from Maha..." */
	fdMessageShowAtActor(ACTOR_MAHA, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	}
	syFieldEventWait(20);
	if(syFieldEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
		syFieldEventWait(40);
		fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
		syFieldEventWait(20);
		/* "I don't think we need to hide the fact that we're Adepts from
		 * Maha..." */
		syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 0);
		FieldStateWork->msg = (FieldStateWork->msg + 1);
	}
	else {
		syFieldEventWait(30);
		fdActorAnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
		syFieldEventWait(20);
		FieldStateWork->msg = (FieldStateWork->msg + 1);
		/* "That's right... I think we can tell Maha that we're Adepts." */
		syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 0);
	}
	fdActorFaceDirection(ACTOR_MAHA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_SOUTHWEST, 0);
	syFieldEventWait(30);
	/* "Awooo..." */
	syFieldEventShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
	}
	syFieldEventWait(30);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	syFieldEventWait(10);
	/* "What did he say? I don't understand..." */
	syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH, 0);
	syFieldEventWait(30);
	/* "He wanted to know what an Adept is... A question I would like answered
	 * as */
	/* well." */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	/* "Adepts tap the power of elemental energies to generate forces like */
	/* Whirlwind..." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "Elementals... You mean the four elements, the powers of earth and fire,
	 */
	/* water and wind?" */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	scFieldActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		scFieldActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	}
	scFieldActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	syFieldEventWait(20);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
	syFieldEventWait(20);
	/* "That is correct. These elements together comprise everything in our */
	/* world..." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	syFieldEventWait(30);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	/* "Interesting. Very interesting..." */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_WEREWOLF, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Awooo..." */
	syFieldEventShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_NORTHEAST, 0);
	syFieldEventWait(30);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	syFieldEventWait(10);
	/* "Um... What did he say that time?" */
	syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHEAST, 0);
	syFieldEventWait(20);
	/* "He asked if there were no longer any need to hide from you..." */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "...Why did he have to hide in here in the first place?" */
	syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_SOUTH, 0);
	syFieldEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_WEREWOLF, 2);
	syFieldEventWait(10);
	/* "Aaah-Ah-Awoooh! Hawawoooh!" */
	syFieldEventShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	syFieldEventWait(20);
	/* "No doubt, most would find the sight of a werewolf rather unnerving..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	syFieldEventWait(30);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	/* "You are, sadly, correct. Many consider ours an accursed race." */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "And if they found you in your werewolf form?" */
	syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldEventWait(10);
	/* "...Once, our people were burned at the stake as abominations... They */
	/* called this 'purification.'" */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD, 33);
	/* "Horrible..." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_SOUTH, 0);
	syFieldEventWait(20);
	fdActorWalkByAndWait(ACTOR_WEREWOLF, 0, 16);
	syFieldEventWait(10);
	scFieldActorJump(ACTOR_WEREWOLF, 6, 15);
	scFieldActorJump(ACTOR_WEREWOLF, 6, 23);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "But...this little one... He won't get excited and try to bite, will he?"
	 */
	syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorSetSpeed(ACTOR_MAHA, 2.5);
	fdActorSetSpeed(ACTOR_WEREWOLF, 2.5);
	fdActorFaceDirection(ACTOR_MAHA, FACING_EAST, 0);
	syFieldEventWait(20);
	fdActorWalkByAndWait(ACTOR_MAHA, 8, 0);
	scFieldActorJump(ACTOR_WEREWOLF, 4, 0);
	fdActorGet(ACTOR_MAHA)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkBy(ACTOR_MAHA, -8, 0);
	fdActorGet(ACTOR_WEREWOLF)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_WEREWOLF, 16, 0);
	syFieldEventWait(1);
	fdActorGet(ACTOR_WEREWOLF)->steeringFlg |= ACTOR_FACES_MOTION;
	scFieldActorSetAnimation(ACTOR_WEREWOLF, ANIM_WEREWOLF_SIT);
	syFieldEventWait(20);
	fdActorSetSpeed(ACTOR_WEREWOLF, 1.3);
	fdActorSetSpeed(ACTOR_MAHA, 0.7);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
	syFieldEventWait(10);
	fdActorShowEmote(ACTOR_WEREWOLF, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
	fdActorSetAnimation(ACTOR_WEREWOLF, ANIM_STAND);
	scFieldActorJump(ACTOR_WEREWOLF, 6, 0);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_WEST, 0);
	syFieldEventWait(30);
	fdActorSetAttachedEffect(ACTOR_WEREWOLF,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorAnimateThenWait(ACTOR_WEREWOLF, ANIM_NOD, 20);
	syFieldEventWait(15);
	fdActorSetAttachedEffect(ACTOR_WEREWOLF,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorAnimateThenWait(ACTOR_WEREWOLF, ANIM_NOD, 20);
	syFieldEventWait(20);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldEventWait(10);
	/* "Child, return to your home." */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_WEREWOLF, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "Arrooo?" */
	syFieldEventShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
	/* "Don't worry. We can trust them... Go back home now." */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(45);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_WEST, 0);
	syFieldEventWait(20);
	/* "Aarrrooo?" */
	syFieldEventShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
	/* "They are Adepts, not unlike us werewolves... They guard an ancient */
	/* secret." */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(45);
	fdActorFaceDirection(ACTOR_MAHA, FACING_EAST, 0);
	syFieldEventWait(20);
	/* "We can trust them. They will not betray us to the world." */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_WEREWOLF, ANIM_NOD, 20);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_SOUTH, 0);
	syFieldEventWait(35);
	fdActorAnimateThenWait(ACTOR_WEREWOLF, ANIM_NOD, 20);
	syFieldEventWait(20);
	/* "Woof." */
	syFieldEventShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldEventWait(20);
	scFieldActorWatchAndSetScript(ACTOR_FELIX, ACTOR_WEREWOLF);
	scFieldActorWatchAndSetScript(ACTOR_KRADEN, ACTOR_WEREWOLF);
	scFieldActorWatchAndSetScript(ACTOR_JENNA, ACTOR_WEREWOLF);
	scFieldActorWatchAndSetScript(ACTOR_SHEBA, ACTOR_WEREWOLF);
	scFieldActorWatchAndSetScript(ACTOR_MAHA, ACTOR_WEREWOLF);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		scFieldActorWatchAndSetScript(ACTOR_PIERS, ACTOR_WEREWOLF);
	}
	fdActorWalkToAndWait(ACTOR_WEREWOLF, 376, 208);
	fdActorWalkByAndWait(ACTOR_WEREWOLF, 0, 16);
	fdActorWalkByAndWait(ACTOR_WEREWOLF, -16, 24);
	fdActorWalkByAndWait(ACTOR_WEREWOLF, -32, 16);
	fdActorWalkByAndWait(ACTOR_WEREWOLF, 0, 48);
	syFieldEventWait(20);
	scFieldActorStop(ACTOR_FELIX);
	scFieldActorStop(ACTOR_KRADEN);
	scFieldActorStop(ACTOR_JENNA);
	scFieldActorStop(ACTOR_SHEBA);
	scFieldActorStop(ACTOR_MAHA);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		scFieldActorStop(ACTOR_PIERS);
	}
	fdActorSetPos(ACTOR_WEREWOLF, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldEventWait(10);
	/* "It has grown quite late." */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
	}
	syFieldEventWait(30);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	/* "Why don't you go to the inn and rest..." */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "But then..." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "I will not leave this place." */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "You aren't going home?" */
	syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "When I learned Reveal, I lost my human form. The wild is my home now."
	 */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "So, Master Maha, you always look this way?" */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	syFieldEventWait(30);
	/* "And the little one?" */
	syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
	/* "The child changed because he looked directly at the full moon." */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 0);
	syFieldEventWait(30);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
	syFieldEventWait(20);
	/* "So, by tomorrow morning, he will have returned to his human form again?"
	 */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldEventWait(10);
	/* "Correct. Now, let us retire." */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	syFieldEventWait(20);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	}
	syFieldEventWait(20);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	scFieldActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		scFieldActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	}
	scFieldActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	syFieldEventWait(20);
	syFieldEventWait(20);
	fdActorAnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
	syFieldEventWait(10);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	scFieldActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	scWalkTowardLeader(ACTOR_KRADEN);
	scFieldActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdActorSetSpeed(ACTOR_JENNA, 1.2);
	scFieldActorSetAnimation(ACTOR_JENNA, ANIM_WALK);
	scWalkTowardLeader(ACTOR_JENNA);
	scFieldActorWaitForMoveById(ACTOR_JENNA);
	fdActorSetPos(ACTOR_JENNA, 0, 0);
	fdActorSetSpeed(ACTOR_SHEBA, 1.2);
	scFieldActorSetAnimation(ACTOR_SHEBA, ANIM_WALK);
	scWalkTowardLeader(ACTOR_SHEBA);
	scFieldActorWaitForMoveById(ACTOR_SHEBA);
	fdActorSetPos(ACTOR_SHEBA, 0, 0);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		fdActorSetSpeed(ACTOR_PIERS, 1.2);
		scFieldActorSetAnimation(ACTOR_PIERS, ANIM_WALK);
		scWalkTowardLeader(ACTOR_PIERS);
		scFieldActorWaitForMoveById(ACTOR_PIERS);
		fdActorSetPos(ACTOR_PIERS, 0, 0);
	}
	syEventCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	scGarohMetWerewolves();
	syFieldEventSetExitRequest(GAROH_WEREWOLF_CAVE_TO_GAROH_ROOMS_HOUSES);
	fdEventEnd();
}

/* By day in the werewolves' cave, at cells (19..21, 15): "Ah, you came... */
/* I've been waiting for you." Maha tells the werewolves' legend and of */
/* Mt. Aleph, and gives the party the Jupiter Djinni Ether. */
void scGarohMahaTellsTheLegend(void) {
	s32 frames;
	FIXED total;
	ACTOR *ether;
	SPR *sprite;

	fdEventSetFlag(FLAG_GAROH_HEARD_MAHA);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventSetMessageId((s32)MsgGarohAhYouCame);
	fdActorSetPos(ACTOR_MAHA, 328, 40);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH, 0);
	/* "Ah, you came..." */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorSetSpeed(ACTOR_FELIX, 0.8);
	fdActorWalkToAndWait(ACTOR_FELIX, 320, 224);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, -16, 0, FACING_NORTH);
	scFieldActorLaunchFromLeader(ACTOR_JENNA, 16, 0, FACING_NORTH);
	scFieldActorLaunchFromLeader(ACTOR_SHEBA, 32, 0, FACING_NORTH);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		scFieldActorLaunchFromLeader(ACTOR_PIERS, 48, 0, FACING_NORTH);
	}
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	scFieldActorWaitForMoveById(ACTOR_SHEBA);
	syFieldEventWait(20);
	fdCameraMoveTo(328, CAMERA_KEEP, 144, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(20);
	fdActorSetPos(ACTOR_CAVE_PASSAGE, 328, 136);
	fdActorSetSprFlags(fdActorGet(ACTOR_CAVE_PASSAGE), 0);
	scGarohOpenCavePassage(ACTOR_CAVE_PASSAGE);
	syFieldEventWait(30);
	fdActorSetSpeed(ACTOR_MAHA, 0.7);
	fdActorSetPos(ACTOR_MAHA, 328, 128);
	syFieldEventWait(10);
	fdActorWalkByAndWait(ACTOR_MAHA, 0, 16);
	syFieldEventWait(20);
	scGarohCloseCavePassage(ACTOR_CAVE_PASSAGE);
	fdActorSetPos(ACTOR_CAVE_PASSAGE, 0, 0);
	syEventCameraFollowActor(ACTOR_MAHA, 1);
	syFieldEventWait(20);
	fdActorWalkByAndWait(ACTOR_MAHA, 0, 48);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
	}
	fdCameraMoveTo(328, CAMERA_KEEP, 184, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(10);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldEventWait(10);
	/* "I've been waiting for you." */
	syFieldEventShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	}
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	syFieldEventWait(20);
	syFieldEventWait(20);
	fdActorWalkByAndWait(ACTOR_KRADEN, 0, -16);
	syFieldEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldEventWait(10);
	/* "Would you mind telling me a little more about werewolves?" */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorWalkByAndWait(ACTOR_MAHA, -8, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(20);
	/* "You want to know the reason we possess the power to transform into */
	/* beasts?" */
	syFieldEventShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Ah... Well, I, er... Actually, yes." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "Do you find it strange that we can change our shapes?" */
	syFieldEventShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "Well, ummm... Yes." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldEventWait(10);
	/* "But surely changing one's shape is not really all that unusual." */
	syFieldEventShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "What are you getting at?" */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorWalkByAndWait(ACTOR_MAHA, 16, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(20);
	/* "Think about the caterpillar for a moment. When the time is right, does
	 * it */
	/* not transform into a beautiful butterfly?" */
	syFieldEventShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	/* "Well, yes. It's only natural..." */
	syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTHEAST, 0);
	syFieldEventWait(20);
	/* "And the pill bug and hedgehog? Do they not change their shapes to
	 * protect */
	/* themselves?" */
	syFieldEventShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	syFieldEventWait(10);
	/* "I have heard of these creatures, yes..." */
	syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorWalkByAndWait(ACTOR_MAHA, -8, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(20);
	/* "And many amphibians possess the power to change color at will, correct?"
	 */
	syFieldEventShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
	syFieldEventWait(35);
	fdActorWalkByAndWait(ACTOR_MAHA, -8, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(25);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldEventWait(10);
	/* "In light of all this, do you mean to tell me you find werewolves so */
	/* abnormal?" */
	syFieldEventShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Hrmm, well... To be honest, I hadn't thought of it that way before..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
	/* "Perhaps you simply did not give it enough thought." */
	syFieldEventShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 45);
	scFieldActorJump(ACTOR_KRADEN, 6, 15);
	scFieldActorJump(ACTOR_KRADEN, 6, 23);
	/* "How insulting! I, sir... am a SCIENTIST!" */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Indeed! A scientist? That is impressive." */
	syFieldEventShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_MAHA, FACING_NORTH, 0);
	syFieldEventWait(30);
	/* "I have spent a great deal of time thinking about werewolves." */
	syFieldEventShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
	fdActorWalkByAndWait(ACTOR_MAHA, 4, -12);
	syFieldEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldEventWait(10);
	/* "For a time, I even felt cursed to have been born a werewolf..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorWalkByAndWait(ACTOR_MAHA, 4, -12);
	syFieldEventWait(10);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
	/* "But after taking a more objective look at nature..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH, 0);
	syFieldEventWait(30);
	/* "I realized that weaker species have survived by evolving...by */
	/* transforming. Don't you agree?" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldEventWait(20);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldEventWait(10);
	/* "Think about it for a moment. Without changing their bodies with weapons
	 */
	/* and armor... Humans would be very weak creatures indeed." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldEventWait(10);
	/* "I believe that evolving into werewolves is just one way for humanity to
	 */
	/* survive..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldEventWait(10);
	/* "But that's..." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
	/* "Of course, there's no way I can prove this theory." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldEventWait(10);
	/* "But I felt that our ability to use Whirlwind was evidence of this." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "Whirlwind? Why?" */
	syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHEAST, 0);
	syFieldEventWait(20);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldEventWait(10);
	/* "I had thought only the people of Garoh could use Whirlwind." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "It was a gift known only to werewolves, to Garoh's pack..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldEventWait(10);
	/* "And that only a chosen few could use Reveal..." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(20);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
	/* "But now I find that you, too, can use these... What does it all mean?"
	 */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	}
	syFieldEventWait(40);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldEventWait(10);
	/* "Why are you able to use Whirlwind, and what is an Adept?" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
	}
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	syFieldEventWait(25);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldEventWait(10);
	/* "Master Maha, do you know of a continent called Angara? It lies far north
	 */
	/* of the Eastern Sea." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "Angara?" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldEventWait(10);
	/* "In the middle of Angara, there is a peak called Mt. Aleph." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldEventWait(10);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldEventWait(10);
	/* "Mt. Aleph? ...Then it's true! Our legends say this peak has stood since
	 */
	/* this world began!" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "If Mt. Aleph does exist, then perhaps our legends are true!" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldEventWait(10);
	/* "It does exist..." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
	syFieldEventWait(30);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldEventWait(10);
	/* "These two hail from the village at the base of Mt. Aleph, a village
	 * called */
	/* Vale." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldEventWait(15);
	fdActorFaceDirection(ACTOR_MAHA, FACING_NORTH, 0);
	syFieldEventWait(30);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldEventWait(10);
	/* "They come from Mt. Aleph!? Incredible!" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
	syFieldEventWait(30);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	syFieldEventWait(25);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldEventWait(10);
	/* "And they are both Adepts. And while their powers may vary... Everyone
	 * from */
	/* Vale is an Adept to a certain degree." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	scFieldActorStartRepeatedMotion(ACTOR_MAHA, 3);
	fdActorSetAttachedEffect(ACTOR_MAHA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(40);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(30);
	/* "Then, does that not make them like us in Garoh?" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldEventWait(10);
	/* "In Vale, the myths say that Mt. Aleph itself gave birth to the
	 * Adepts..." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "This is not what the legends of Garoh tell us..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	syFieldEventWait(20);
	/* "As a people, they are quite gifted...but it goes far beyond that..." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorWalkByAndWait(ACTOR_KRADEN, 16, 0);
	syFieldEventWait(20);
	fdActorWalkByAndWait(ACTOR_KRADEN, 16, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
	syFieldEventWait(20);
	fdActorWalkByAndWait(ACTOR_KRADEN, 16, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	}
	syFieldEventWait(20);
	fdActorWalkByAndWait(ACTOR_KRADEN, 16, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
	syFieldEventWait(20);
	fdCameraMoveTo(344, CAMERA_KEEP, 208, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(20);
	/* "The mystical stones on Mt. Aleph... The Psynergy Stones... They are */
	/* closely linked to the powers of the townsfolk." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorSetSpeed(ACTOR_MAHA, 1.2);
	fdActorWalkByAndWait(ACTOR_MAHA, 16, 16);
	fdActorSetSpeed(ACTOR_MAHA, 0.7);
	/* "'Psynergy Stones,' you say?" */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldEventWait(10);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
	}
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 0);
	syFieldEventWait(30);
	/* "If there were similar stones in Garoh, it might explain your powers as
	 */
	/* well." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldEventWait(10);
	/* "And what manner of stones are these Psynergy Stones?" */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	syFieldEventWait(10);
	/* "You've seen them, Maha! The shining stone at the heart of Air's Rock."
	 */
	syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(20);
	/* "I know the stone you speak of, but..." */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD, 33);
	/* "Air's Rock is quite far from Garoh." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTHEAST, 0);
	syFieldEventWait(20);
	/* "I don't see how it could affect the people here." */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldEventWait(10);
	syFieldEventWait(20);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	syFieldEventWait(20);
	/* "I wonder... Could it be the wind?" Yes: "You probably figured it out for
	 */
	/* yourself already, Felix." No: "I guess I couldn't have expected you to */
	/* understand, Felix..." */
	fdEventOpenMessage(ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	}
	if(!syFieldEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
		syFieldEventWait(25);
		/* "You probably figured it out for yourself already, Felix." */
		syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 0);
		FieldStateWork->msg = (FieldStateWork->msg + 1);
	}
	else {
		syFieldEventWait(40);
		FieldStateWork->msg = (FieldStateWork->msg + 1);
		/* "I guess I couldn't have expected you to understand, Felix..." */
		syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	}
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	}
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
	syFieldEventWait(30);
	/* "There were strong winds blowing within Air's Rock... Those winds would
	 */
	/* carry far..." */
	syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "Carry? Carry what?" */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "Ugh! You're so dense! The power of the Psynergy Stone!" */
	syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
	/* "How is that possible? How could the stone's power be carried on the
	 * wind?" */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	syFieldEventWait(20);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	/* "Do not belittle the power of the wind!" */
	syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorAnimateThenWait(ACTOR_SHEBA, ANIM_SHAKE_HEAD, 33);
	/* "Given time, the wind can grind great mountains down to sand!" */
	syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	syFieldEventWait(10);
	/* "I mean, the wind at Air's Rock was strong enough to blow us off the */
	/* ground!" */
	syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MAHA, ANIM_NOD);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	}
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	syFieldEventWait(20);
	syFieldEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	syFieldEventWait(10);
	/* "So why couldn't the wind carry particles of the stone all the way to */
	/* Garoh?" */
	syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	scFieldActorStartRepeatedMotion(ACTOR_MAHA, 3);
	fdActorSetAttachedEffect(ACTOR_MAHA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scFieldActorStartRepeatedMotion(ACTOR_KRADEN, 3);
	fdActorSetAttachedEffect(ACTOR_KRADEN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(40);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTHEAST, 0);
	syFieldEventWait(25);
	scFieldActorJump(ACTOR_KRADEN, 6, 15);
	scFieldActorJump(ACTOR_KRADEN, 6, 23);
	syFieldEventWait(10);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldEventWait(10);
	/* "She's right!", said by both at once. */
	syFieldEventShowTwoMessagesAndWait(ACTOR_MAHA, 9, 2, 9, 5, ACTOR_KRADEN, 17,
									   14, 20, 9, 0);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
	}
	scFieldActorStartRepeatedMotion(ACTOR_MAHA, 3);
	fdActorSetAttachedEffect(ACTOR_MAHA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(40);
	/* "Those particles from the Psynergy Stone must be charged with its power!"
	 */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	scFieldActorStartRepeatedMotion(ACTOR_KRADEN, 3);
	fdActorSetAttachedEffect(ACTOR_KRADEN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(40);
	/* "This explains why your Psynergy is weaker than that of Vale's people...
	 */
	/* You've been exposed to smaller particles." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "The wind patterns might also affect who among us in Garoh can use */
	/* Whirlwind." */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldEventWait(10);
	/* "Of course, this is all mere conjecture, but it does offer us some */
	/* clues..." */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldEventWait(10);
	/* "Thank you, umm..." */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_SHEBA, ANIM_NOD, 20);
	syFieldEventWait(10);
	/* "It's Sheba. I'm pleased to meet you." */
	syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHWEST, 0);
	syFieldEventWait(20);
	/* "Oh! Haven't we given you our names, Master Maha?" */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 0);
	syFieldEventWait(20);
	/* "I am Kraden." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldEventWait(10);
	fdActorWalkByAndWait(ACTOR_KRADEN, -18, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(20);
	/* "This is Jenna..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
									0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(20);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldEventWait(20);
	fdActorAnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
	syFieldEventWait(10);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
		/* "Piers..." */
		syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN,
										0, 0);
		fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
		syFieldEventWait(20);
		fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
		syFieldEventWait(20);
		fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
		syFieldEventWait(20);
		syFieldEventWait(10);
	}
	else {
		FieldStateWork->msg = (FieldStateWork->msg + 1);
	}
	fdActorWalkByAndWait(ACTOR_KRADEN, -42, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	syFieldEventWait(20);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldEventWait(10);
	/* "And Felix." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	syFieldEventWait(20);
	fdActorWalkByAndWait(ACTOR_MAHA, -16, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(20);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldEventWait(30);
	fdActorAnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
	syFieldEventWait(25);
	/* "You must permit me to thank you for all you've done." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD, 33);
	/* "No, no, please... We don't need any thanks..." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_NORTH, 0);
	syFieldEventWait(40);
	scGarohPopQuestionMarks(ACTOR_MAHA);
	scGarohPopQuestionMarks(ACTOR_MAHA);
	syFieldEventWait(20);
	fdActorWalkByAndWait(ACTOR_MAHA, 16, 0);
	syFieldEventWait(20);
	scFieldActorStartRepeatedMotion(ACTOR_MAHA, 3);
	fdActorSetAttachedEffect(ACTOR_MAHA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(40);
	fdActorFaceDirection(ACTOR_MAHA, FACING_WEST, 0);
	syFieldEventWait(20);
	fdActorWalkByAndWait(ACTOR_MAHA, -16, 0);
	syFieldEventWait(10);
	fdActorWalkByAndWait(ACTOR_MAHA, -16, 0);
	syFieldEventWait(20);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
	fdActorFaceDirection(ACTOR_MAHA, FACING_EAST, 0);
	syFieldEventWait(20);
	fdActorWalkByAndWait(ACTOR_MAHA, 16, 0);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_MAHA, FACING_NORTH, 0);
	syFieldEventWait(20);
	scFieldActorStartRepeatedMotion(ACTOR_MAHA, 3);
	fdActorSetAttachedEffect(ACTOR_MAHA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(40);
	fdActorWalkByAndWait(ACTOR_MAHA, 0, -12);
	syFieldEventWait(20);
	fdActorWalkByAndWait(ACTOR_MAHA, 0, -12);
	syFieldEventWait(20);
	syFieldEventWait(20);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH, 0);
	syFieldEventWait(20);
	/* "That's too bad... I was going to give you this." */
	syFieldEventShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldEventWait(10);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldEventWait(5);
	ether = fdActorGet(ACTOR_ETHER);
	sprite = ether->spr;
	sprite->scale = FIXED_ONE * 2 / 5;
	scFieldActorSetSprPriority(ACTOR_ETHER, 1);
	fdActorSetSprFlags(fdActorGet(ACTOR_ETHER), 0);
	fdActorSetPos(ACTOR_ETHER, 328, 160);
	scFieldActorJump(ACTOR_ETHER, 9, 0);
	total = 0;
	for(frames = 0; frames <= ETHER_GROW_STEPS; frames++) {
		sprite->scale =
			syMathDivide(total, ETHER_GROW_STEPS) + FIXED_ONE * 2 / 5;
		ether->facing += FACING_STEP;
		syWait(1);
		total += FIXED_ONE * 3 / 5;
	}
	ether->facing = FACING_SOUTH;
	fdActorWalkByAndWait(ACTOR_ETHER, 0, 8);
	fdActorSetSprFlags(fdActorGet(ACTOR_ETHER), 1);
	syFieldEventWait(5);
	fdActorWalkByAndWait(ACTOR_ETHER, 0, 8);
	syFieldEventWait(5);
	fdActorWalkByAndWait(ACTOR_ETHER, 0, 8);
	syFieldEventWait(5);
	fdActorWalkByAndWait(ACTOR_ETHER, 0, 8);
	syFieldEventWait(5);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "It's a Djinni!" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHEAST, 0);
	syFieldEventWait(20);
	/* "A 'Djinni'? Is that what you call them?" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorWalkByAndWait(ACTOR_MAHA, 0, 24);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHEAST, 0);
	syFieldEventWait(10);
	/* "When he is with you, he adds to your power. He is very helpful." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(20);
	/* "I suppose you'll be continuing your journey now?" Yes: "So you intend to
	 */
	/* travel by sea? That is a long journey..." No: "So, you still can't tell
	 */
	/* just what sort of journeys await you..." */
	fdEventOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	}
	syFieldEventWait(20);
	if(!syFieldEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
		syFieldEventWait(30);
		/* "So you intend to travel by sea? That is a long journey..." */
		syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0,
										0);
		FieldStateWork->msg = (FieldStateWork->msg + 1);
	}
	else {
		syFieldEventWait(40);
		FieldStateWork->msg = (FieldStateWork->msg + 1);
		/* "So, you still can't tell just what sort of journeys await you..." */
		syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0,
										0);
	}
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
	}
	syFieldEventWait(30);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldEventWait(10);
	/* "In that case, I think you will get more use out of this 'Djinni' than I
	 */
	/* will." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorSetSpeed(ACTOR_MAHA, 2.4);
	fdActorGet(ACTOR_MAHA)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	fdActorWalkByAndWait(ACTOR_MAHA, 0, 8);
	fdActorWalkByAndWait(ACTOR_MAHA, 0, -8);
	syFieldEventWait(1);
	fdActorGet(ACTOR_MAHA)->steeringFlg |= ACTOR_FACES_MOTION;
	syFieldEventWait(1);
	fdActorSetSpeed(ACTOR_MAHA, 0.7);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	}
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	fdDjinnJoinParty(ACTOR_ETHER, DJINNI_ETHER / DJINN_PER_ELEMENT,
					 DJINNI_INDEX(DJINNI_ETHER));
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_STAND);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH, 0);
	syFieldEventWait(20);
	FieldStateWork->msg = (FieldStateWork->msg + 1);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Another Djinni! Talk about lucky!" */
	syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	syFieldEventWait(30);
	fdActorAnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	syFieldEventWait(20);
	/* "This is a great gift. We are quite thankful." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	syFieldEventWait(20);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
	/* "You don't need to thank me. Just take care on your travels." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldEventWait(20);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldEventWait(10);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Good luck with the... werewolf thing, Maha..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHEAST, 0);
	syFieldEventWait(20);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldEventWait(20);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	}
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	syFieldEventWait(20);
	syFieldEventWait(20);
	fdActorWalkByAndWait(ACTOR_KRADEN, 12, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
	syFieldEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldEventWait(10);
	/* "Well, Felix, we must be going now." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorAnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
	syFieldEventWait(20);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	}
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
	syFieldEventWait(20);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	scWalkIntoLeader(ACTOR_KRADEN);
	fdActorSetSpeed(ACTOR_JENNA, 1.2);
	scWalkIntoLeader(ACTOR_JENNA);
	fdActorSetSpeed(ACTOR_SHEBA, 1.2);
	scWalkIntoLeader(ACTOR_SHEBA);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
		fdActorSetSpeed(ACTOR_PIERS, 1.2);
		scWalkIntoLeader(ACTOR_PIERS);
	}
	syEventCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdEventEnd();
}

void scGarohLittleWerewolfThanks(void);
void scGarohLiftPillar(void);
void scGarohLiftPushedPillar(void);
void scGarohRevealCaveEntrance(void);
void scGarohRevealCavePassage(void);

extern u32 FieldEffect_MediumScript[];

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

/* The four pillars Move pushes, ended by -1. */
s16 Garoh_Pillars[] = {
    ACTOR_FIRST_PILLAR, ACTOR_SECOND_PILLAR, ACTOR_THIRD_PILLAR,
    ACTOR_FOURTH_PILLAR, -1, 0,
};

u32 Garoh_WerewolfLookAroundScript[] = {
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
    SC_WAIT(12),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(7),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
    SC_WAIT(34),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/* The path's colours by night, BG palette 6's colours 1 to 9. */
u16 Garoh_PathNightColors[] = {
    31, 31, 31, 29, 29, 30, 27, 28,
    29, 25, 27, 28, 23, 25, 27, 21,
    24, 26, 15, 18, 19, 10, 12, 13,
    5, 6, 6,
};

/* The path's colours by day, BG palette 1's colours 8 to 15. */
u16 Garoh_PathDayColors[] = {
    24, 28, 30, 19, 24, 27, 15, 20,
    25, 11, 16, 23, 7, 12, 21, 3,
    8, 19, 27, 27, 27, 16, 24, 29,
    0,
};

const s32 *FieldEffect_DurationScripts[] = {
    FieldEffect_ShortScript,
    FieldEffect_MediumScript,
    FieldEffect_LongScript,
};

s32 Garoh_ShrunkSpriteScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, toFIXED(-4)),
    SC_ADD(SW_X, toFIXED(6)),
    SC_SET(SW_SCALE_X, toFIXED(0.8)),
    SC_SET(SW_SCALE_Y, toFIXED(0.8)),
    SC_CALL(scGarohClearScriptedActorSprFlags),
    SC_END,
};

#include "shared/scene_entrances_default.c"

u32 SceneExits[] = {
    SCENE_EXITS(Garoh),
    SCENE_EXIT_UNLESS(GAROH_TO_GAROH_ROOMS_HOUSES_SOUTH, GarohRoomsHouses, GAROH_ROOMS_HOUSES_FROM_GAROH_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TO_GAROH_ROOMS_SHOP, GarohRoomsShop, GAROH_ROOMS_SHOP_FROM_GAROH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TO_GAROH_ROOMS_HOUSES_NORTHWEST, GarohRoomsHouses, GAROH_ROOMS_HOUSES_FROM_GAROH_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TO_GAROH_ROOMS_HOUSES_SOUTHWEST, GarohRoomsHouses, GAROH_ROOMS_HOUSES_FROM_GAROH_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TO_GAROH_ROOMS_HOUSES_SOUTHEAST, GarohRoomsHouses, GAROH_ROOMS_HOUSES_FROM_GAROH_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TO_GAROH_ROOMS_SANCTUM, GarohRoomsSanctum, GAROH_ROOMS_SANCTUM_FROM_GAROH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TO_GAROH_TUNNELS_NORTHEAST, GarohTunnels, GAROH_TUNNELS_FROM_GAROH_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TO_GAROH_TUNNELS_WEST, GarohTunnels, GAROH_TUNNELS_FROM_GAROH_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TO_GAROH_TUNNELS_SOUTH, GarohTunnels, GAROH_TUNNELS_FROM_GAROH_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TO_GAROH_PATH_EAST, GarohPath, GAROH_PATH_FROM_GAROH_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TO_GAROH_PATH_WEST, GarohPath, GAROH_PATH_FROM_GAROH_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TO_GAROH_WEREWOLF_CAVE, GarohWerewolfCave, GAROH_WEREWOLF_CAVE_FROM_GAROH, EXIT_FLAG_NONE),
    SCENE_EXITS(GarohPath),
    SCENE_EXIT_UNLESS(GAROH_PATH_TO_GAROH_WAY_UP, GarohWayUp, GAROH_WAY_UP_FROM_GAROH_PATH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_PATH_TO_GAROH_EAST, Garoh, GAROH_FROM_GAROH_PATH_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_PATH_TO_GAROH_WEST, Garoh, GAROH_FROM_GAROH_PATH_WEST, EXIT_FLAG_NONE),
    SCENE_EXITS(GarohWayUp),
    SCENE_EXIT_UNLESS(GAROH_WAY_UP_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_GAROH_WAY_UP, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_WAY_UP_TO_GAROH_PATH, GarohPath, GAROH_PATH_FROM_GAROH_WAY_UP, EXIT_FLAG_NONE),
    SCENE_EXITS(GarohTunnels),
    SCENE_EXIT_UNLESS(GAROH_TUNNELS_TO_GAROH_WEST, Garoh, GAROH_FROM_GAROH_TUNNELS_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, GarohTunnels, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, GarohTunnels, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TUNNELS_TO_GAROH_NORTHEAST, Garoh, GAROH_FROM_GAROH_TUNNELS_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TUNNELS_TO_GAROH_ROOMS_SHOP_WEST, GarohRoomsShop, GAROH_ROOMS_SHOP_FROM_GAROH_TUNNELS_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TUNNELS_TO_GAROH_ROOMS_SHOP_EAST, GarohRoomsShop, GAROH_ROOMS_SHOP_FROM_GAROH_TUNNELS_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TUNNELS_TO_GAROH_ROOMS_HOUSES_EAST, GarohRoomsHouses, GAROH_ROOMS_HOUSES_FROM_GAROH_TUNNELS_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TUNNELS_TO_GAROH_ROOMS_HOUSES_WEST, GarohRoomsHouses, GAROH_ROOMS_HOUSES_FROM_GAROH_TUNNELS_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, GarohTunnels, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, GarohTunnels, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TUNNELS_TO_GAROH_SOUTH, Garoh, GAROH_FROM_GAROH_TUNNELS_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXITS(GarohWerewolfCave),
    SCENE_EXIT_UNLESS(GAROH_WEREWOLF_CAVE_TO_GAROH, Garoh, GAROH_FROM_GAROH_WEREWOLF_CAVE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, GarohWerewolfCave, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, GarohWerewolfCave, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, GarohWerewolfCave, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, GarohWerewolfCave, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_WEREWOLF_CAVE_TO_ROOM_BEHIND_ROCK, GarohWerewolfCave, GAROH_WEREWOLF_CAVE_ROOM_BEHIND_ROCK_FROM_CAVE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_WEREWOLF_CAVE_ROOM_BEHIND_ROCK_TO_CAVE, GarohWerewolfCave, GAROH_WEREWOLF_CAVE_FROM_ROOM_BEHIND_ROCK, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_WEREWOLF_CAVE_TO_GAROH_ROOMS_HOUSES, GarohRoomsHouses, GAROH_ROOMS_HOUSES_FROM_GAROH_WEREWOLF_CAVE, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

ACTOR_PLACE Garoh_ActorPlacements[] = {
    {SCENE_TABLE_END},
};

ACTOR_PLACE Garoh_VillageActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_HOODED_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 400, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_HOODED_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 416, 0, 424, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_HOODED_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 192, 0, 304, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_HOODED_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 344, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_HOODED_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 368, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 232, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 232, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 232, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 312, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 280, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 296, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 408, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_MAHA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE Garoh_VillageDayActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 400, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 416, 0, 424, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 192, 0, 304, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 344, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 368, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 384, 0, 232, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 232, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 232, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 232, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 312, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 280, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 296, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 408, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

ACTOR_PLACE Garoh_WayUpActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 472, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 456, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 392, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 392, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE Garoh_TunnelsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)Garoh_ShrunkSpriteScript, 128, 0, 168, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)Garoh_ShrunkSpriteScript, 208, 0, 56, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)Garoh_ShrunkSpriteScript, 152, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)Garoh_ShrunkSpriteScript, 248, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)Garoh_ShrunkSpriteScript, 352, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)Garoh_ShrunkSpriteScript, 216, 0, 424, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)Garoh_ShrunkSpriteScript, 96, 0, 488, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)Garoh_ShrunkSpriteScript, 328, 0, 440, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)Garoh_ShrunkSpriteScript, 456, 0, 352, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)Garoh_ShrunkSpriteScript, 136, 0, 664, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE Garoh_PathActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WEREWOLF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 120, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 120, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

ACTOR_PLACE Garoh_WerewolfCaveActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WEREWOLF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAHA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_ETHER), MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)Garoh_ShrunkSpriteScript, 64, 0, 562, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)Garoh_ShrunkSpriteScript, 370, 0, 56, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)Garoh_ShrunkSpriteScript, 256, 0, 80, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)Garoh_ShrunkSpriteScript, 184, 0, 352, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)Garoh_ShrunkSpriteScript, 392, 0, 368, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)Garoh_ShrunkSpriteScript, 280, 0, 626, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)Garoh_ShrunkSpriteScript, 336, 0, 784, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

/* The village's doors opening, as MAPGRID_ANIMATION_STEP rows: two steps of six */
/* frames each: a door, the shop's wide door, the sanctum's tall door. */
s16 Garoh_DoorFrames[] = {
    82, 32, 1, 2, 6, 80, 32, 1,
    2, 6, -1,
};
s16 Garoh_WideDoorFrames[] = {
    82, 35, 1, 2, 6, 80, 35, 1,
    2, 6, -1,
};
s16 Garoh_TallDoorFrames[] = {
    78, 32, 2, 2, 6, 78, 35, 2,
    2, 6, -1, 0,
};

/* The doors Felix walks into, a row for each exit from the first: the opening */
/* and the cell it plays at. */
MAP_GRID_CELL_ANIMATION Garoh_Doors[] = {
    {Garoh_DoorFrames, 78, 21},
    {Garoh_DoorFrames, 0, 0},
    {Garoh_DoorFrames, 72, 10},
    {Garoh_WideDoorFrames, 74, 22},
    {Garoh_DoorFrames, 85, 23},
    {Garoh_TallDoorFrames, 78, 13},
};

TRIGGER Garoh_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TABLE_END},
};
/* clang-format on */

TRIGGER Garoh_VillageTriggers[] = {
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), GAROH_TO_GAROH_ROOMS_HOUSES_SOUTH,
	 CONDITION_ALWAYS, EVENT(scGarohOpenDoorAndEnter)},
	{TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 2, CONDITION_ALWAYS, 2},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 GAROH_TO_GAROH_ROOMS_HOUSES_NORTHWEST, CONDITION_ALWAYS,
	 EVENT(scGarohOpenDoorAndEnter)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 GAROH_TO_GAROH_ROOMS_HOUSES_SOUTHWEST, CONDITION_ALWAYS,
	 EVENT(scGarohOpenDoorAndEnter)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 GAROH_TO_GAROH_ROOMS_HOUSES_SOUTHEAST, CONDITION_ALWAYS,
	 EVENT(scGarohOpenDoorAndEnter)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), GAROH_TO_GAROH_ROOMS_SANCTUM,
	 CONDITION_ALWAYS, EVENT(scGarohOpenDoorAndEnter)},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
	{TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 8, CONDITION_ALWAYS, 8},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, 9},
	{TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
	{TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
	{STEP_TRIGGER_FACING(FACING_NORTH), GAROH_TO_GAROH_WEREWOLF_CAVE,
	 CONDITION_ALWAYS, EVENT(scGarohEnterOpening)},
	{TRIGGER_STEP, CELL_BEHAVIOR_BY_SANCTUM_DOOR, FLAG_GAROH_MET_WEREWOLVES,
	 EVENT(scGarohWerewolfPeersFromCave)},
	{TRIGGER_TALK, ACTOR_VILLAGER_ASKING_ABOUT_THE_WEREWOLF, CONDITION_ALWAYS,
	 EVENT(scGarohAskIfSawWerewolf)},
	{TRIGGER_TALK, ACTOR_HOODED_VILLAGER, CONDITION_ALWAYS,
	 MSGNO(MsgGarohGoodEveningForgiveMeFor)},
	{TRIGGER_TALK, ACTOR_HOWLING_VILLAGER, CONDITION_ALWAYS,
	 EVENT(scGarohHowlForDog)},
	{TRIGGER_TALK, ACTOR_VILLAGER_WARNING_STRANGERS, CONDITION_ALWAYS,
	 MSGNO(MsgGarohYouThereStrangersShouldNot)},
	{TRIGGER_TALK, ACTOR_VILLAGER_ON_THE_FULL_MOON, CONDITION_ALWAYS,
	 EVENT(scGarohAskAboutFullMoon)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
	 ACTOR_VILLAGER_ASKING_ABOUT_THE_WEREWOLF, CONDITION_ALWAYS,
	 MSGNO(MsgGarohItsGrimLuckThatBrings)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOODED_VILLAGER,
	 CONDITION_ALWAYS, MSGNO(MsgGarohSuchGorgeousMoonlightIfOnly)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOWLING_VILLAGER,
	 CONDITION_ALWAYS, MSGNO(MsgGarohWhichOneOfUsWas)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WARNING_STRANGERS,
	 CONDITION_ALWAYS, MSGNO(MsgGarohForeignersOnlyComeHereEn)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_FULL_MOON,
	 CONDITION_ALWAYS, MSGNO(MsgGarohTheNightOfTheFull)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
	 CONDITION_ALWAYS, EVENT(scGarohRevealCaveEntrance)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
	 CONDITION_ALWAYS, EVENT(scGarohHideCaveEntrance)},
	{EXAMINE_TRIGGER(EXAMINE_WELL), CELL_BEHAVIOR_GAROH_WELL, CONDITION_ALWAYS,
	 FIND_MESSAGE(MSGNO(MsgGarohMehISawTheSilhouette))},
	{TRIGGER_TABLE_END},
};

TRIGGER Garoh_VillageDayTriggers[] = {
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), GAROH_TO_GAROH_ROOMS_HOUSES_SOUTH,
	 CONDITION_ALWAYS, EVENT(scGarohOpenDoorAndEnter)},
	{TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 2, CONDITION_ALWAYS, 2},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 GAROH_TO_GAROH_ROOMS_HOUSES_NORTHWEST, CONDITION_ALWAYS,
	 EVENT(scGarohOpenDoorAndEnter)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 GAROH_TO_GAROH_ROOMS_HOUSES_SOUTHWEST, CONDITION_ALWAYS,
	 EVENT(scGarohOpenDoorAndEnter)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
	 GAROH_TO_GAROH_ROOMS_HOUSES_SOUTHEAST, CONDITION_ALWAYS,
	 EVENT(scGarohOpenDoorAndEnter)},
	{STEP_TRIGGER_WALKING_INTO(FACING_NORTH), GAROH_TO_GAROH_ROOMS_SANCTUM,
	 CONDITION_ALWAYS, EVENT(scGarohOpenDoorAndEnter)},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
	{TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 8, CONDITION_ALWAYS, 8},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, 9},
	{TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
	{TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
	{STEP_TRIGGER_FACING(FACING_NORTH), GAROH_TO_GAROH_WEREWOLF_CAVE,
	 CONDITION_ALWAYS, EVENT(scGarohEnterOpening)},
	{TRIGGER_TALK, ACTOR_VILLAGER_ASKING_ABOUT_THE_WEREWOLF, CONDITION_ALWAYS,
	 MSGNO(MsgGarohIHearTellFromMaster)},
	{TRIGGER_TALK, ACTOR_HOODED_VILLAGER, CONDITION_ALWAYS, MSGNO(MsgGaroh16)},
	{TRIGGER_TALK, ACTOR_HOWLING_VILLAGER, CONDITION_ALWAYS,
	 EVENT(scGarohHowlOutLoud)},
	{TRIGGER_TALK, ACTOR_VILLAGER_WARNING_STRANGERS, CONDITION_ALWAYS,
	 EVENT(scGarohAskAboutSecret)},
	{TRIGGER_TALK, ACTOR_VILLAGER_ON_THE_FULL_MOON, CONDITION_ALWAYS,
	 MSGNO(MsgGarohIHeardThereWereOther)},
	{TRIGGER_TALK, ACTOR_LITTLE_WEREWOLF, CONDITION_ALWAYS,
	 EVENT(scGarohLittleWerewolfThanks)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
	 ACTOR_VILLAGER_ASKING_ABOUT_THE_WEREWOLF, CONDITION_ALWAYS,
	 MSGNO(MsgGarohHowCouldTheseOutsidersHave)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOODED_VILLAGER,
	 CONDITION_ALWAYS, MSGNO(MsgGarohIThoughtWedNeverBe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOWLING_VILLAGER,
	 CONDITION_ALWAYS, MSGNO(MsgGarohINeverThoughtIdBe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WARNING_STRANGERS,
	 CONDITION_ALWAYS, MSGNO(MsgGarohMahaSaidTheseTravelersWere)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_FULL_MOON,
	 CONDITION_ALWAYS, MSGNO(MsgGarohIWouldLoveToMeet)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
	 ACTOR_LITTLE_WEREWOLF | TRIGGER_SUBJECT_OWN_EFFECT,
	 FLAG_LITTLE_WEREWOLF_THANKED, EVENT(scGarohLittleWerewolfThanks)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LITTLE_WEREWOLF,
	 CONDITION_ALWAYS, MSGNO(MsgGarohMyDadToldMeI)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
	 CONDITION_ALWAYS, EVENT(scGarohRevealCaveEntrance)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
	 CONDITION_ALWAYS, EVENT(scGarohHideCaveEntrance)},
	{EXAMINE_TRIGGER(EXAMINE_WELL), CELL_BEHAVIOR_GAROH_WELL, CONDITION_ALWAYS,
	 FIND_MESSAGE(MSGNO(MsgGarohMehISawTheSilhouette))},
	{TRIGGER_TABLE_END},
};

TRIGGER Garoh_PathTriggers[] = {
	{TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
	{TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
	{TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
	{TRIGGER_STEP, CELL_BEHAVIOR_WEREWOLF_RUNS_PAST, FLAG_GAROH_SAW_WEREWOLF,
	 EVENT(scGarohWerewolfRunsPast)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
	 CONDITION_ALWAYS, EVENT(scGarohIsNight)},
	{TRIGGER_TABLE_END},
};

/* No end row: the room reads on into the tunnels' table, whose exits and */
/* Reveal answer here too. */
TRIGGER Garoh_WayUpTriggers[] = {
	{TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
	{TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
	{TRIGGER_STEP, CELL_BEHAVIOR_LOWER_TWILIGHT_LINE,
	 FLAG_CROSSED_LOWER_TWILIGHT_LINE, EVENT(scGarohCrossLowerTwilightLine)},
	{TRIGGER_STEP, CELL_BEHAVIOR_UPPER_TWILIGHT_LINE,
	 FLAG_CROSSED_UPPER_TWILIGHT_LINE, EVENT(scGarohCrossUpperTwilightLine)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_FIRST_PILLAR, FLAG_FIRST_PILLAR_SUNK, EVENT(scGarohLiftPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR,
	 FLAG_FIRST_PILLAR_SUNK, EVENT(scGarohPlaceFirstPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_SECOND_PILLAR, FLAG_SECOND_PILLAR_SUNK, EVENT(scGarohLiftPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_PILLAR,
	 FLAG_SECOND_PILLAR_SUNK, EVENT(scGarohPlaceSecondPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_THIRD_PILLAR, FLAG_GAROH_ROPE_LASHED, EVENT(scGarohLiftPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THIRD_PILLAR,
	 FLAG_GAROH_ROPE_LASHED, EVENT(scGarohPlaceThirdPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
	 ACTOR_FOURTH_PILLAR, FLAG_FOURTH_PILLAR_SUNK, EVENT(scGarohLiftPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FOURTH_PILLAR,
	 FLAG_FOURTH_PILLAR_SUNK, EVENT(scGarohPlaceFourthPillar)},
	{TRIGGER_PUSH, 0, FLAG_FIRST_PILLAR_SUNK, EVENT(scGarohLiftPushedPillar)},
	{TRIGGER_PUSHED, 0, FLAG_FIRST_PILLAR_SUNK,
	 EVENT(scGarohPlacePushedPillar)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
	 CONDITION_ALWAYS, EVENT(scGarohRestoreWayUpLight)},
};

TRIGGER Garoh_TunnelsTriggers[] = {
	{TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, 4},
	{TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, 6},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 8, CONDITION_ALWAYS, 8},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 9, CONDITION_ALWAYS, 9},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 10, CONDITION_ALWAYS, 10},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 11, CONDITION_ALWAYS, 11},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
	 CONDITION_ALWAYS, EVENT(scGarohRevealTunnelPassage)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
	 CONDITION_ALWAYS, EVENT(scGarohHideTunnelPassage)},
	{TRIGGER_TABLE_END},
};

TRIGGER Garoh_WerewolfCaveTriggers[] = {
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
	{TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
	{TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
	{TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
	{TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
	{STEP_TRIGGER_FACING(FACING_NORTH), 6, CONDITION_ALWAYS,
	 EVENT(scGarohEnterOpening)},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 10, CONDITION_ALWAYS, 10},
	{TRIGGER_STEP, CELL_BEHAVIOR_CAVE_MOUTH, FLAG_GAROH_FOLLOWED_WEREWOLF,
	 EVENT(scGarohFollowWerewolfIntoCave)},
	{TRIGGER_STEP, CELL_BEHAVIOR_BELOW_MAHA, CONDITION_ALWAYS,
	 EVENT(scGarohMeetInTheCave)},
	{TRIGGER_TALK, ACTOR_MAHA, CONDITION_ALWAYS,
	 MSGNO(MsgGarohTakeCareInYourTravels)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAHA, CONDITION_ALWAYS,
	 MSGNO(MsgGarohIWishThatIWere)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
	 CONDITION_ALWAYS, EVENT(scGarohRevealCavePassage)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
	 CONDITION_ALWAYS, EVENT(scGarohHideCavePassage)},
	{TRIGGER_TABLE_END},
};

/* The frames counted at night; a wolf howls every 1,024. */
s32 Garoh_HowlFrames = 0;

u32 Garoh_QuestionMarkScript[] = {
	SC_RELEASE,
};
#undef fdActorAnimateThenWait

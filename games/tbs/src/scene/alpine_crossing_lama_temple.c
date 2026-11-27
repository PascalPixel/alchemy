/* @file */
/* The Alpine Crossing (SceneId_AlpineCrossing) and Lama Temple */
/* (SceneId_LamaTemple), two places on Silk Road west of Xian that share this */
/* overlay. The getters pick each scene's tables by MainRoutineStateWork.next,
 */
/* and the two share one exit table. */
/* */
/* The Alpine Crossing is a valley of cliffs and ledges between Xian and Altin.
 */
/* Silk Road comes in from the World Map on the east, a road climbs north to */
/* Altin, and the way west to Lama Temple is buried under a rockslide (the */
/* rocks at cells (17, 21) to (17, 23)). A signpost beside the road west of */
/* the east entrance reads `North: Altin Mine West: Lama Temple`. */
/* */
/* If the party has seen Feizhi leave Xian (FLAG_FEIZHI_WENT_TO_HSU, set in */
/* fuchin_temple.c), she runs in behind them a few steps inside the east */
/* entrance (scAlpineCrossingFeizhiArrives), cries out at the rockslide and */
/* runs down past the puddle to it, where she waits until Hama's lesson. On */
/* this first visit the party can freeze the puddle at the foot of the gully */
/* into an ice pillar with Frost, grow the sprout below the signpost into a */
/* vine with Growth, and climb round to the chest of Power Bread in the */
/* south-east corner. These two puzzles are kept only while the party stays */
/* (scene temp flags). Then the road north leads on to Altin. */
/* */
/* Lama Temple lies west, at the head of a wooded valley: a path from the World
 */
/* Map on the south crosses a bridge, where a traveling healer stands, and */
/* climbs to the temple building with five monks about it. They have one set */
/* of lines and thoughts until Hsu is freed (FLAG_HSU_RESCUED) and another */
/* after. A defeat anywhere here brings the party back in front of the healer */
/* (LAMA_TEMPLE_AFTER_DEFEAT). */
/* */
/* The temple's sliding door (scLamaTempleEnterHamasRoom) leads into Hama's */
/* room, in LacquerRoomsA (LACQUER_ROOMS_A_HAMAS_ROOM_FROM_LAMA_TEMPLE, in */
/* fuchin_temple.c), where she teaches Ivan Reveal and Feizhi brings word that
 */
/* Hsu is trapped (FLAG_XIAN_HEARD_HSU_TRAPPED). Coming back out, the area name
 */
/* is not shown again. East of the waterfall, writing on the cliff hints at */
/* Reveal, and Reveal takes away the mossy rock beneath it to show a chest of */
/* Water of Life (scLamaTempleRevealChest). The temple grounds also hold a */
/* Psynergy Stone. */
/* */
/* Back at the Alpine Crossing after the lesson, Hama and Feizhi stand by Hsu,
 */
/* who is pinned under a boulder west of the rockslide. Reveal on the cliff */
/* west of him shows the mouth of a small cave (scAlpineCrossingRevealPath). It
 */
/* is part of Altin Peak's AltinPeakSouthEntrance, whose other mouth opens */
/* just north-east of Hsu. The international editions call that cave */
/* Alpine Crossing too, so coming back out of it the area name is not shown */
/* again. The first time the party comes out beside Hsu, the camera finds him */
/* and Hama asks if he is hurt (scAlpineCrossingHamaReachesHsu). Lift on the */
/* boulder (scAlpineCrossingLiftBoulder) raises it while Hama and Feizhi pull */
/* him free, and the party is taken back to Lama Temple, where Hama says */
/* farewell outside her door (scLamaTempleRunHamasFarewell). */
/* */
/* If the party comes back by the road from the north instead */
/* (ALPINE_CROSSING_FROM_WORLD_MAP_NORTH), Hsu has been freed without them */
/* (FLAG_HSU_RESCUED_WITHOUT_PARTY, which lamakan_desert.c can set too), and */
/* Hama and Feizhi are gone. Either way, once he is free the rockslide has been
 */
/* cleared and two workers from Xian stand by the open road. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/field_effect.h"
#include "graphics/graphics.h"
#include "text/msg_ids.h"

extern s32 AlpineCrossing_FeizhiPath[];
extern s32 AlpineCrossing_LeaderPath[];
extern s32 AlpineCrossing_HsuStruggle[];
extern u16 LamaTemple_DoorOpenCells[];
extern u16 LamaTemple_DoorCloseCells[];
extern SCENE_ENTRANCE LamaTemple_Entrances[];
extern SCENE_ENTRANCE AlpineCrossing_Entrances[];
extern u32 AlpineCrossingLamaTemple_Exits[];
extern ACTOR_PLACE LamaTemple_ActorPlacements[];
extern ACTOR_PLACE AlpineCrossing_ActorPlacements[];
extern TRIGGER LamaTemple_Triggers[];
extern TRIGGER AlpineCrossing_Triggers[];

/* The temple's Psynergy Stone. It is an area temp flag, so it refills once the
 */
/* party has been to another area. */
#define FLAG_LAMA_TEMPLE_PSYNERGY_STONE FLAG_AREA_TEMP_50

/* The overlay's two scenes. */
#define MAP_ALPINE_CROSSING (SCENE_ALPINE_CROSSING)
#define MAP_LAMA_TEMPLE		(SCENE_LAMA_TEMPLE)

/* Lama Temple's actors. Garet, Ivan and Mia are placed in their own party */
/* slots for Hama's farewell. The others count from ACTOR_FIRST_PLACED. */
typedef enum LAMA_TEMPLE_ACTOR_ID {
	/* Hama, back in her temple for her farewell. */
	ACTOR_TEMPLE_HAMA = ACTOR_FIRST_PLACED,
	ACTOR_GREETING_MONK,
	ACTOR_MONK_ASKING_ABOUT_THE_EAST,
	ACTOR_MEDITATING_MONK,
	ACTOR_MONK_SENSING_ANXIETY,
	ACTOR_CHI_MONK,
	/* The mossy rock east of the waterfall that Reveal takes away to show the
	 */
	/* chest under it (scLamaTempleRevealChest). */
	ACTOR_MOSSY_ROCK,
	ACTOR_TRAVELING_HEALER
} LAMA_TEMPLE_ACTOR_ID;

/* Alpine Crossing's actors. */
typedef enum ALPINE_CROSSING_ACTOR_ID {
	ACTOR_HAMA = ACTOR_FIRST_PLACED,
	ACTOR_HSU,
	ACTOR_FEIZHI,
	ACTOR_BOULDER,
	/* A young man in a white headband, one of the people of Xian who moved the
	 */
	/* rocks. */
	ACTOR_FIRST_WORKER,
	/* A young man with his hair tied back. */
	ACTOR_SECOND_WORKER,
	/* The puddle at the foot of the gully, which Frost freezes into a pillar */
	/* (scAlpineCrossingFreezePuddle). */
	ACTOR_PUDDLE,
	/* The sprout below the signpost, which Growth grows into a vine */
	/* (scAlpineCrossingGrowSprout). */
	ACTOR_SPROUT
} ALPINE_CROSSING_ACTOR_ID;

/* Alpine Crossing's Psynergy puzzles, kept while the party stays. */
enum {
	/* Frost has frozen the puddle into an ice pillar. */
	FLAG_ALPINE_CROSSING_PUDDLE_FROZEN = FLAG_SCENE_TEMP_00,
	/* Growth has grown the sprout into a vine the party can climb. */
	FLAG_ALPINE_CROSSING_SPROUT_GROWN = FLAG_SCENE_TEMP_01
};

/* Animations of these scenes' own sprites beyond ACTOR_ANIMATION, */
/* named by when the events play them. */
enum {
	ANIM_HAMA_PULL = 5,
	ANIM_FEIZHI_PULL = 6,
	/* Drops into a crouch, then holds a half crouch with her head bowed */
	/* (feizhi.png's frames 33 and 36). */
	ANIM_FEIZHI_CROUCH = 5,
	/* SPRITE_TURNING_ROCK's mossy rock (turning_rock.png's frame 1). Its first
	 */
	/* animations show picture 0, a purple glow. */
	ANIM_TURNING_ROCK_MOSSY = 2,
	ANIM_HSU_STRUGGLE = 5,
	ANIM_MONK_MEDITATE = 5
};

/* The cell behaviors Lama Temple's triggers answer. */
typedef enum LAMA_TEMPLE_CELL_BEHAVIOR {
	/* The path out to the World Map at the south edge. */
	CELL_BEHAVIOR_LAMA_TEMPLE_SOUTH_ROAD = 1,
	CELL_BEHAVIOR_LAMA_TEMPLE_DOOR = 2,
	CELL_BEHAVIOR_LAMA_TEMPLE_WALL_WRITING = 5,
	CELL_BEHAVIOR_LAMA_TEMPLE_CHEST = 100,
	CELL_BEHAVIOR_LAMA_TEMPLE_PSYNERGY_STONE = 110
} LAMA_TEMPLE_CELL_BEHAVIOR;

/* The cell behaviors Alpine Crossing's triggers answer. */
typedef enum ALPINE_CROSSING_CELL_BEHAVIOR {
	/* Silk Road at the east edge, back toward Xian. */
	CELL_BEHAVIOR_ALPINE_CROSSING_EAST_ROAD = 1,
	/* The road at the top of the valley, toward Altin. */
	CELL_BEHAVIOR_ALPINE_CROSSING_NORTH_ROAD = 2,
	/* The road at the west edge, toward Lama Temple. */
	CELL_BEHAVIOR_ALPINE_CROSSING_WEST_ROAD = 3,
	/* The cave mouth north-east of Hsu. */
	CELL_BEHAVIOR_ALPINE_CROSSING_EAST_CAVE_MOUTH = 4,
	/* The cave mouth Reveal shows in the cliff west of Hsu */
	/* (scAlpineCrossingRevealPath). */
	CELL_BEHAVIOR_ALPINE_CROSSING_WEST_CAVE_MOUTH = 5,
	CELL_BEHAVIOR_ALPINE_CROSSING_SIGNPOST = 7,
	CELL_BEHAVIOR_ALPINE_CROSSING_FEIZHI_ARRIVAL = 10,
	CELL_BEHAVIOR_ALPINE_CROSSING_CHEST = 100
} ALPINE_CROSSING_CELL_BEHAVIOR;

/* The door of Hama's room, three cells wide and two high. Each step copies the
 */
/* next door picture onto it for five frames. */
enum { DOOR_X = 67, DOOR_Y = 6 };

/* The chest of Water of Life under the mossy rock: its treasure spot is the */
/* chest cell's own. */
enum { TREASURE_SPOT_WATER_OF_LIFE = CELL_BEHAVIOR_LAMA_TEMPLE_CHEST };

/* Draws the actor behind the background, on sprite priority 3 with automatic */
/* priority off. */
static s32 scActorDrawBehindBackground(ACTOR *actor) {
	actor->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	actor->spr->image.pri = 3;
	actor->spr->shadow.pri = 3;
	return 0;
}

/* Update function that turns the party leader toward Feizhi while she runs */
/* past. */
static UNREAD_RESULT scAlpineCrossingLeaderFaceFeizhi(ACTOR *leader) {
	ACTOR *feizhi;

	feizhi = fdActorGet(ACTOR_FEIZHI);
	leader->facing = syAtan(feizhi->z.fixed - leader->z.fixed,
							feizhi->x.fixed - leader->x.fixed);
#ifndef UBFIX
	return 0;
#endif
}

const SCENE_ENTRANCE *scGetEntrances(void) {
	if(MainRoutineStateWork.next == MAP_ALPINE_CROSSING) {
		return AlpineCrossing_Entrances;
	}
	return LamaTemple_Entrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void) { return 0; }

/* Both scenes share one exit table. */
const u32 *scGetExits(void) { return AlpineCrossingLamaTemple_Exits; }

const ACTOR_PLACE *scGetActorPlacements(void) {
	if(MainRoutineStateWork.next == MAP_ALPINE_CROSSING) {
		return AlpineCrossing_ActorPlacements;
	}
	return LamaTemple_ActorPlacements;
}

/* A monk in Lama Temple, until Hsu is freed, asks whether the party comes from
 */
/* the east. */
static void scLamaTempleTalkToMonkAskingAboutTheEast(void) {
	fdEventStart();
	fdEventSetMessageId(MsgMountainYoungWarriorsDoComeFrom);
	/* "Young warriors, do you come from the east?" */
	fdEventOpenMessage(ACTOR_MONK_ASKING_ABOUT_THE_EAST, 0);

	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
		fdEventWait(20);
		/* "Then you must have come across Lamakan Desert..." */
		fdEventShowMessage(ACTOR_MONK_ASKING_ABOUT_THE_EAST, 0);
	}
	else {
		fdEventSkipMessages(1);
		/* "Did you stop in Xian along the way?" */
		/* Yes: "Master Feh, in Xian, is very close to our Master Hama...." */
		/* No: "Then it is good that you have come here. Feh's kung fu is..." */
		fdEventAskYesNo(ACTOR_MONK_ASKING_ABOUT_THE_EAST, 0);
	}

	fdEventEnd();
}

/* The meditating monk explains meditation. He's in Lama Temple until Hsu is */
/* freed, and first asks whether the party knows what meditation is. */
static void scLamaTempleTalkToMeditatingMonk(void) {
	fdEventStart();
	fdEventSetMessageId(MsgMountainDoKnowMeditation);
	/* "Do you know what meditation is?" */
	/* Yes: "It allows you to find peace by closing your eyes... and..." */
	/* No: "It is training in which you clear your mind of all..." */
	fdEventAskYesNo(ACTOR_MEDITATING_MONK, 0);
	fdEventEnd();
}

/* A monk in Lama Temple praises Master Hama as the greatest master of Chi, */
/* until Hsu is freed. */
static void scLamaTempleTalkToChiMonk(void) {
	fdEventStart();
	fdEventSetMessageId(MsgMountainDidKnowMasterHamaGreatest);
	/* "Did you know that Master Hama is the greatest master of Chi?" */
	/* Yes: "You do? Then you must have already met Master Feh in Xian,..." */
	/* No: "Then today, you shall see that Master Hama is truly the..." */
	fdEventAskYesNo(ACTOR_CHI_MONK, 0);
	fdEventEnd();
}

/* Walking north into the temple's sliding door (cell (8, 10)) slides it open */
/* with a thump, and Isaac steps through into Hama's room. */
static void scLamaTempleEnterHamasRoom(void) {
	sySndPlay(SE_THUMP);
	fdMapGridAnimateCells(LamaTemple_DoorOpenCells, DOOR_X, DOOR_Y);
	fdActorGet(ACTOR_PARTY_LEADER)->motionFlg = 0;
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
	fdEventWait(16);
	syImportEventSetExitRequest(LAMA_TEMPLE_TO_LACQUER_ROOMS_A_HAMAS_ROOM);
}

/* Hama thanks the party in front of her temple door once they have freed Hsu,
 */
/* Mia teases Garet over whose idea it was to help, and Hama goes back inside;
 */
/* Ivan then decides not to ask her what she would not say to him. The party is
 */
/* brought here straight from the rescue at the Alpine Crossing. */
static void scLamaTempleRunHamasFarewell(void) {
	fdEventStart();
	fdActorSetPos(ACTOR_TEMPLE_HAMA, 136, 168);
	fdActorFaceDirection(ACTOR_TEMPLE_HAMA, FACING_SOUTH_SOUTHWEST, 0);
	fdActorSetPos(ACTOR_PARTY_LEADER, 144, 200);
	fdActorSetPos(ACTOR_GARET, 160, 192);
	fdActorSetPos(ACTOR_IVAN, 128, 200);
	fdActorSetPos(ACTOR_MIA, 112, 192);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(60);

	/* "Adepts, let me thank you again." */
	fdActorSetAnimationAndWait(ACTOR_TEMPLE_HAMA, ANIM_NOD);
	fdEventWait(20);
	fdEventSetMessageId(MsgMountainAdeptsLetMeThankAgain);
	/* "Adepts, let me thank you again. I am truly grateful." */
	fdEventShowMessageThenWait(ACTOR_TEMPLE_HAMA, 0, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_TEMPLE_HAMA, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "I did not expect you to come and aid us." */
	fdEventShowMessageThenWait(ACTOR_TEMPLE_HAMA, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "No, Master Hama... We are the ones who should be grateful." */
	fdEventShowMessageThenWait(ACTOR_MIA, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdEventWait(20);
	/* "Thank you for passing the power of Reveal on to me... But..." */
	fdEventShowMessageThenWait(ACTOR_IVAN, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_TEMPLE_HAMA, 2);
	fdActorFaceDirection(ACTOR_TEMPLE_HAMA, FACING_SOUTH_SOUTHWEST, 20);
	/* "What is it, Ivan?" */
	fdEventShowMessageThenWait(ACTOR_TEMPLE_HAMA, 0, 20);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdEventWait(120);

	/* Garet claims that helping was his idea and asks the leader to agree. */
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 20);
	/* "Well, really, I was the one who suggested we help you, right?" */
	fdEventOpenMessage(ACTOR_GARET, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdEventWait(20);
		fdActorFaceDirection(ACTOR_MIA, FACING_EAST, 20);
		fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
		/* "What? Didn't you want us to hurry on to the desert, Garet?" */
		fdEventShowMessageThenWait(ACTOR_MIA, 0, 20);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
		fdActorStartRepeatedMotion(ACTOR_GARET, 2);
		fdActorSetAttachedEffect(ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT |
												  ATTACHED_EFFECT_ONCE);
		fdEventWait(60);
		fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 20);
		/* "Did I, Mia? Did I really? I think you must be mistaken...." */
		fdEventShowMessageThenWait(ACTOR_GARET, 0, 20);
		fdEventSkipMessages(2);
	}
	else {
		fdEventSkipMessages(2);
		fdEventWait(20);
		fdActorFaceDirection(ACTOR_MIA, FACING_EAST, 20);
		fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
		fdEventWait(20);
		/* "No, Garet... You wanted us to get moving!" */
		fdEventShowMessageThenWait(ACTOR_MIA, 0, 20);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
		/* "Isaac, Mia... Don't make me look like a fool..." */
		fdEventShowMessageThenWait(ACTOR_GARET, 0, 20);
	}

	/* Hama wishes them well and goes back into her room. */
	fdActorRunRepeatedMotion(ACTOR_TEMPLE_HAMA, 2);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_TEMPLE_HAMA, FACING_SOUTH_SOUTHEAST, 20);
	/* "Do not worry, Garet. I do not mind." */
	fdEventShowMessageThenWait(ACTOR_TEMPLE_HAMA, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_TEMPLE_HAMA, ANIM_NOD);
	fdEventWait(20);
	/* "Travel well, my friends." */
	fdEventShowMessageThenWait(ACTOR_TEMPLE_HAMA, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_TEMPLE_HAMA, 2);
	fdEventWait(20);
	/* "I hope to see you soon." */
	fdEventShowMessageThenWait(ACTOR_TEMPLE_HAMA, 0, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_TEMPLE_HAMA, ANIM_NOD);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_TEMPLE_HAMA, FACING_NORTH, 30);
	/* The door slides open with SE_THUMP, Hama walks through, and it slides
	 * shut. */
	sySndPlay(SE_THUMP);
	fdMapGridAnimateCells(LamaTemple_DoorOpenCells, DOOR_X, DOOR_Y);
	fdActorSetSpeed(ACTOR_TEMPLE_HAMA, 0.8);
	fdActorWalkToAndWait(ACTOR_TEMPLE_HAMA, 136, 136);
	fdActorSetPos(ACTOR_TEMPLE_HAMA, 0, 0);
	sySndPlay(SE_THUMP);
	fdMapGridAnimateCells(LamaTemple_DoorCloseCells, DOOR_X, DOOR_Y);
	fdEventWait(60);
	scPlayQueuedMusic();

	/* Garet asks whether the leader means to leave Hama's words unasked. */
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 20);
	/* "Weren't you going to ask Master Hama what she meant earlier?" */
	fdEventShowMessageThenWait(ACTOR_GARET, 0, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 20);
	/* "We do not know when we will be able to see Master Hama again." */
	fdEventShowMessageThenWait(ACTOR_MIA, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
	/* "Isaac, don't you think we should ask Master Hama what she meant?" */
	/* Yes: "See? If it's bothering you, you should just go ahead and ask..." */
	/* No: "Huh? You think we should just leave it as is?" */
	fdEventAskYesNo(ACTOR_GARET, 0);
	fdEventWait(20);

	/* Ivan decides that Hama will tell him when their mission is done. */
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
	/* "Thank you, everyone..." */
	fdEventShowMessageThenWait(ACTOR_IVAN, 0, 20);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	fdActorStartRepeatedMotion(ACTOR_GARET, 1);
	fdActorRunRepeatedMotion(ACTOR_MIA, 1);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "but it is better that we leave now." */
	fdEventShowMessageThenWait(ACTOR_IVAN, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdEventWait(20);
	/* "What do you mean?" */
	fdEventShowMessageThenWait(ACTOR_MIA, 0, 20);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH_NORTHWEST, 20);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(20);
	/* "Master Hama hasn't looked me in the eye since she said those..." */
	fdEventShowMessageThenWait(ACTOR_IVAN, 0, 20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	/* "Well, that just makes me even more curious!" */
	fdEventShowMessageThenWait(ACTOR_GARET, 0, 20);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 20);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "Don't worry about it. I doubt she would even tell me if I asked." */
	fdEventShowMessageThenWait(ACTOR_IVAN, 0, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	/* "Well, that's just..." */
	fdEventShowMessageThenWait(ACTOR_MIA, 0, 20);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(20);
	/* "Master Hama will tell me when we have completed our mission." */
	fdEventShowMessageThenWait(ACTOR_IVAN, 0, 20);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	fdActorStartRepeatedMotion(ACTOR_GARET, 1);
	fdActorRunRepeatedMotion(ACTOR_MIA, 1);
	fdEventWait(20);
	fdActorSetSpeed(ACTOR_IVAN, 0.5);
	fdActorWalkToAndWait(ACTOR_IVAN, 128, 184);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTH, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 20);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(20);
	/* "I have to believe that..." */
	fdEventShowMessageThenWait(ACTOR_IVAN, 0, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(20);

	/* Garet, Ivan and Mia walk to the leader and rejoin the party. */
	fdActorSetSpeed(ACTOR_GARET, 0.5);
	fdActorSetSpeed(ACTOR_MIA, 0.5);
	fdActorWalkTo(ACTOR_GARET, 144, 200);
	fdActorWalkTo(ACTOR_IVAN, 144, 200);
	fdActorWalkTo(ACTOR_MIA, 144, 200);
	fdActorWaitForMoveById(ACTOR_GARET);
	fdActorSetPos(ACTOR_GARET, 0, 0);
	fdActorWaitForMoveById(ACTOR_IVAN);
	fdActorSetPos(ACTOR_IVAN, 0, 0);
	fdActorWaitForMoveById(ACTOR_MIA);
	fdActorSetPos(ACTOR_MIA, 0, 0);
	fdEventEnd();
}

/* Searching the writing on the cliff east of the waterfall (cells (20, 6) and
 */
/* (21, 6)) reads "He who has the power to see that which cannot be seen shall
 */
/* see the truth...", a hint to use Reveal on the mossy rock below it. */
static void scLamaTempleReadWallWriting(void) {
	fdEventStart();
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	mnFieldMessageShow(MsgMountainHeWhoHasPowerSee, 1);
	fdEventEnd();
}

/* When Reveal wears off in Lama Temple, the mossy rock east of the waterfall */
/* stands over the chest again (cell (21, 9)), and the chest can no longer be */
/* opened. The same happens on every entry. The chest's spot is hidden and its
 */
/* cell takes the plain ground's attributes from the cell kept at (85, 9). */
static void scLamaTempleHideChest(void) {
	fdMapGridCopyCellAttributes(85, 9, 1, 1, 21, 9);
	fdTreasureSpotSetActorPos(TREASURE_SPOT_WATER_OF_LIFE, 0, 0);
	fdActorSetPos(ACTOR_MOSSY_ROCK, 344, 152);
}

/* Reveal in Lama Temple makes the mossy rock east of the waterfall vanish, */
/* showing the chest of Water of Life under it (cell (21, 9)). The chest's */
/* spot goes back to its cell, and the cell gets back the chest's attributes, */
/* which scInit kept at (21, 73). */
static void scLamaTempleRevealChest(void) {
	fdMapGridCopyCellAttributes(21, 73, 1, 1, 21, 9);
	fdTreasureSpotSetPos(TREASURE_SPOT_WATER_OF_LIFE, -1, -1);
	fdActorSetPos(ACTOR_MOSSY_ROCK, 0, 0);
}

/* The traveling healer at the east end of the bridge in Lama Temple offers the
 */
/* Sanctum's services when spoken to from the south; from any other side he */
/* only says he travels the world to spread good will. */
static void scLamaTempleTalkToHealer(void) {
	u32 heading = fdActorGet(ACTOR_PARTY_LEADER)->facing;

	fdEventStart();
	if(FACING_IS_NORTH(heading)) {
		mnSanctumRun(ACTOR_TRAVELING_HEALER);
	}
	else {
		fdEventSetMessageId(MsgMountainAmTravelingAroundWorldSpread);
		/* "I am traveling around the world to spread good will." */
		fdEventShowMessage(ACTOR_TRAVELING_HEALER, 0);
	}
	fdEventEnd();
}

/* The row Feizhi runs along, from (536, 216) west to (408, 216). */
#define FEIZHI_ROW 13

/* Feizhi runs in behind Isaac as he walks onto Silk Road a few steps inside */
/* the east entrance (cells (27, 12) to (27, 14)), cries out at the boulders */
/* blocking the road, and runs down past the puddle to the rockslide calling */
/* for Hsu, with Isaac following. Nothing happens here unless the party has */
/* seen her leave Xian, and it plays once. */
static void scAlpineCrossingFeizhiArrives(void) {
	ACTOR *leader;

	if(gmFlagGet(FLAG_FEIZHI_WENT_TO_HSU)) {
		fdEventStart();
		fdActorSetPos(ACTOR_FEIZHI, 536, 216);
		fdEventSetMessageId(MsgMountainYahhSilkRoadBouldersBlock);
		/* "Yahh! Silk Road! Boulders block the road!" */
		fdEventShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
		fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
		fdEventWait(20);

		/* The leader turns to follow Feizhi as she runs past; one standing in
		 */
		/* her row steps aside first. */
		leader = fdActorGet(ACTOR_PARTY_LEADER);
#ifdef UBFIX
		leader->update = scAlpineCrossingLeaderFaceFeizhi;
#else
		leader->update = (void (*)(ACTOR *))scAlpineCrossingLeaderFaceFeizhi;
#endif
		leader = fdActorGet(ACTOR_PARTY_LEADER);
		if(FIELD_CELL(leader->z.fixed) == FEIZHI_ROW) {
			fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 440, 200);
		}
		fdActorSetSpeed(ACTOR_FEIZHI, 2);
		fdActorSetSprPriority(ACTOR_FEIZHI, 2);
		fdActorWalkToAndWait(ACTOR_FEIZHI, 408, 216);
		fdActorGet(ACTOR_FEIZHI)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
		fdEventWait(10);
		fdActorFaceDirection(ACTOR_FEIZHI, FACING_WEST, 20);
		/* "My vision... It came true." */
		fdEventShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
		fdActorStartRepeatedMotion(ACTOR_FEIZHI, 2);
		fdActorSetAttachedEffect(ACTOR_FEIZHI, ATTACHED_EFFECT_IN_FRONT |
												   ATTACHED_EFFECT_ONCE);
		fdEventWait(60);
		/* "Hsu, Hsu!" */
		fdEventShowMessageThenWait(ACTOR_FEIZHI, 0, 20);

		/* Feizhi runs down the path with the camera; the leader follows her */
		/* until the camera arrives. */
		fdActorSetScript(ACTOR_FEIZHI, AlpineCrossing_FeizhiPath);
		fdCameraMoveTo(296, CAMERA_KEEP, 344, 1);
		gmFlagSet(FLAG_FEIZHI_SAW_ROCKSLIDE);
		fdActorWaitForScriptEndById(ACTOR_FEIZHI);
		fdCameraWaitForArrival();
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
		fdActorSetScript(ACTOR_PARTY_LEADER, AlpineCrossing_LeaderPath);
		fdActorWaitForScriptEndById(ACTOR_PARTY_LEADER);
		fdEventWait(10);
		fdActorGet(ACTOR_PARTY_LEADER)->update = NULL;
		fdEventWait(30);

		/* She cannot hear Hsu's voice. */
		fdActorRunRepeatedMotion(ACTOR_FEIZHI, 2);
		fdEventWait(20);
		fdActorFaceDirection(ACTOR_FEIZHI, FACING_SOUTH_SOUTHWEST, 120);
		fdActorShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
		fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
						 60);
		fdActorSetAnimationAndWait(ACTOR_FEIZHI, ANIM_SHAKE_HEAD);
		fdEventWait(20);
		/* "It is no good. I cannot hear Hsu's voice!" */
		fdEventShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
		fdEventEnd();
	}
}

/* Feizhi, waiting by the rockslide, asks whether she should return to Xian, */
/* and whatever Isaac answers decides to keep looking for Hsu. */
static void scAlpineCrossingTalkToFeizhi(void) {
	fdEventStart();
	fdEventSetMessageId(MsgMountainDoDoWarriorShouldReturn);
	fdActorShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	/* "What do I do? Warrior, should I return to Xian?" */
	fdEventOpenMessage(ACTOR_FEIZHI, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
		fdEventSkipMessages(1);
	}
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_FEIZHI, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	fdEventShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
	fdEventEnd();
}

/* The first time the party comes out of the cave mouth north-east of Hsu, */
/* the camera finds him pinned under the boulder; Hama asks if he is okay, and
 */
/* none of them knows how to free him. */
static void scAlpineCrossingHamaReachesHsu(void) {
	fdEventStart();
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(30);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	fdCameraMoveToActorById(ACTOR_HSU, 1);
	fdCameraWaitForArrival();
	fdActorRunRepeatedMotion(ACTOR_HAMA, 2);
	fdEventWait(20);
	fdEventSetMessageId(MsgMountainHsuOkay);
	/* "Hsu, are you okay?" */
	fdEventShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_HSU, 1);
	fdEventWait(20);
	/* "Master Hama, I am okay. But the boulder... I cannot move." */
	fdEventShowMessageThenWait(ACTOR_HSU, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
	fdEventWait(20);
	/* "Hsu seems unhurt. You must be glad, Feizhi." */
	fdEventShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorFaceDirection(ACTOR_FEIZHI, FACING_NORTH_NORTHEAST, 20);
	/* "Yes. Thank you, Master. But how can we free him?" */
	fdEventShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
	fdEventWait(60);
	fdActorShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "I don't know what we can do." */
	fdEventShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorStartRepeatedMotion(ACTOR_FEIZHI, 2);
	fdActorSetAttachedEffect(ACTOR_FEIZHI,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(60);
	fdActorFaceDirection(ACTOR_FEIZHI, FACING_NORTH_NORTHWEST, 20);
	fdActorSetAnimation(ACTOR_HSU, ANIM_HSU_STRUGGLE);
	fdEventEnd();
	gmFlagSet(FLAG_HAMA_REACHED_HSU);
}

/* Lift on the boulder pinning Hsu at the Alpine Crossing raises it, glowing, */
/* while Hama and Feizhi pull him out in two heaves. Hama then sends for Master
 */
/* Feh and the party is taken to Lama Temple for her farewell. */
static void scAlpineCrossingLiftBoulder(void) {
	fdEventStart();
	fdActorSetAttachedEffect(ACTOR_HAMA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorStartRepeatedMotion(ACTOR_HAMA, 2);
	fdEventWait(60);
	fdEventSetMessageId(MsgMountainIsaacDidLiftBoulder);
	/* "Isaac, did you lift the boulder?" */
	fdEventShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorSetAttachedEffect(ACTOR_FEIZHI,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorJump(ACTOR_FEIZHI, 4, 0);
	fdEventWait(60);
	/* "Oh my! The boulder! It floats!" */
	fdEventShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_HAMA, 1);
	fdEventWait(20);
	/* "Quickly, Feizhi. We must pull Hsu free!" */
	fdEventShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorFaceDirection(ACTOR_FEIZHI, FACING_NORTH_NORTHEAST, 20);
	fdActorSetAnimationAndWait(ACTOR_FEIZHI, ANIM_NOD);
	fdEventWait(20);

	/* Hama and Feizhi take hold of Hsu. */
	fdActorWalkTo(ACTOR_HAMA, 178, 276);
	fdActorWalkToAndWait(ACTOR_FEIZHI, 172, 284);
	fdActorWaitForMoveById(ACTOR_HAMA);
	fdActorFaceDirection(ACTOR_HAMA, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_FEIZHI, FACING_NORTH_NORTHWEST, 0);
	fdEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_HAMA, 2);
	fdEventWait(20);
	/* "Ready yourself, Hsu. I am going to pull. One, two, three!" */
	fdEventShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorGet(ACTOR_HAMA)->steeringFlg &= (u8)~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_FEIZHI)->steeringFlg &= (u8)~ACTOR_FACES_MOTION;
	fdActorSetSpeed(ACTOR_HAMA, 0.2);
	fdActorSetSpeed(ACTOR_FEIZHI, 0.2);

	/* The first heave: two tugs draw Hsu halfway out. */
	fdActorSetAnimation(ACTOR_HAMA, ANIM_HAMA_PULL);
	fdActorSetAnimation(ACTOR_FEIZHI, ANIM_FEIZHI_PULL);
	fdEventWait(20);
	sySndPlay(SE_CHEST_CLOSE);
	fdActorSetDestinationOffset(ACTOR_HAMA, 2, 0);
	fdActorSetDestinationOffset(ACTOR_HSU, 2, 0);
	fdActorSetDestinationOffset(ACTOR_FEIZHI, 2, 0);
	fdActorWaitForMoveById(ACTOR_FEIZHI);
	fdEventWait(30);
	fdActorSetAnimation(ACTOR_HAMA, ANIM_HAMA_PULL);
	fdActorSetAnimation(ACTOR_FEIZHI, ANIM_FEIZHI_PULL);
	fdEventWait(20);
	sySndPlay(SE_CHEST_CLOSE);
	fdActorSetDestinationOffset(ACTOR_HAMA, 4, 0);
	fdActorSetDestinationOffset(ACTOR_HSU, 4, 0);
	fdActorSetDestinationOffset(ACTOR_FEIZHI, 4, 0);
	fdActorWaitForMoveById(ACTOR_FEIZHI);
	fdActorStop(ACTOR_HSU);
	fdActorSetAnimation(ACTOR_HAMA, ANIM_STAND);
	fdActorSetAnimation(ACTOR_FEIZHI, ANIM_STAND);
	fdEventWait(50);
	fdActorJump(ACTOR_FEIZHI, 2, 0);
	fdEventWait(20);
	/* "He is halfway out. Just a little further." */
	fdEventShowMessageThenWait(ACTOR_FEIZHI, 0, 20);

	/* Hama asks the leader to hold the boulder a moment longer. */
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
	fdEventWait(20);
	/* "Can you hang on just a moment longer, Isaac?" */
	/* Yes: "Thank you, Isaac!" */
	/* No: "You cannot back out now! You must be responsible!" */
	fdEventAskYesNo(ACTOR_HAMA, 0);
	/* "Ready? I am going to pull! One, two, three!" */
	fdEventShowMessageThenWait(ACTOR_HAMA, 0, 20);

	/* The second heave pulls Hsu free. */
	fdActorSetAnimation(ACTOR_HAMA, ANIM_HAMA_PULL);
	fdActorSetAnimation(ACTOR_FEIZHI, ANIM_FEIZHI_PULL);
	fdEventWait(20);
	sySndPlay(SE_CHEST_CLOSE);
	fdActorSetDestinationOffset(ACTOR_HAMA, 2, 0);
	fdActorSetDestinationOffset(ACTOR_HSU, 2, 0);
	fdActorSetDestinationOffset(ACTOR_FEIZHI, 2, 0);
	fdActorWaitForMoveById(ACTOR_FEIZHI);
	fdEventWait(30);
	fdActorSetAnimation(ACTOR_HAMA, ANIM_HAMA_PULL);
	fdActorSetAnimation(ACTOR_FEIZHI, ANIM_FEIZHI_PULL);
	fdEventWait(20);
	sySndPlay(SE_CHEST_CLOSE);
	fdActorSetDestinationOffset(ACTOR_HAMA, 4, 0);
	fdActorSetDestinationOffset(ACTOR_HSU, 4, 0);
	fdActorSetDestinationOffset(ACTOR_FEIZHI, 4, 0);
	fdActorWaitForMoveById(ACTOR_FEIZHI);
	fdEventWait(40);
	fdActorSetAnimation(ACTOR_HAMA, ANIM_STAND);
	fdActorSetAnimation(ACTOR_FEIZHI, ANIM_STAND);
	fdActorJump(ACTOR_FEIZHI, 2, 0);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_FEIZHI, FACING_NORTH_NORTHEAST, 20);
	/* "We did it! Master Hama, Hsu is free!" */
	fdEventShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
	fdEventWait(20);
	/* "We did it, Feizhi!" */
	fdEventShowMessageThenWait(ACTOR_HAMA, 0, 30);
	/* "Thank you, Isaac!" */
	fdEventShowMessageThenWait(ACTOR_HAMA, 0, 20);

	/* Feizhi steps back; they agree to send for Master Feh. */
	fdActorGet(ACTOR_FEIZHI)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorSetSpeed(ACTOR_FEIZHI, 0.8);
	fdActorWalkToAndWait(ACTOR_FEIZHI, 168, 296);
	fdActorFaceDirection(ACTOR_FEIZHI, FACING_NORTH_NORTHEAST, 20);
	fdActorSetAnimation(ACTOR_FEIZHI, ANIM_FEIZHI_CROUCH);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
	fdEventWait(20);
	/* "We must contact Master Feh and ask him to tend to Hsu's..." */
	fdEventShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_FEIZHI, ANIM_NOD);
	fdEventWait(20);
	/* "I will ask him to clean up the rocks, too." */
	fdEventShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
	fdEventWait(20);
	/* "Excellent. If Silk Road remains blocked, everyone will suffer." */
	fdEventShowMessageThenWait(ACTOR_HAMA, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdEventWait(20);
	fdEventEnd();
	gmFlagSet(FLAG_HSU_RESCUED);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	syImportEventSetExitRequest(ALPINE_CROSSING_TO_LAMA_TEMPLE);
}

/* Searching the signpost beside the road west of the east entrance (cell */
/* (25, 11)) reads "North: Altin Mine West: Lama Temple". */
static void scAlpineCrossingReadSignpost(void) {
	fdEventStart();
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	mnFieldMessageShow(MsgMountainNorthAltinMineWestLama, 1);
	fdEventEnd();
}

/* Reveal at the Alpine Crossing opens the mouth of a cave in the cliff west of
 */
/* Hsu (cells (5, 13) and (5, 14)). The cave inside comes out again just */
/* north-east of him. The cave mouth's pictures and cells are kept off the map
 */
/* at (5, 28). */
static void scAlpineCrossingRevealPath(void) {
	fdMapGridCopyCellsTo(5, 28, 5, 13, 1, 2);
	fdMapGridCopyCellAttributes(5, 28, 1, 2, 5, 13);
	fdEventWait(1);
}

/* When Reveal wears off at the Alpine Crossing, the cave mouth in the cliff */
/* west of Hsu is solid rock again (cells (5, 13) and (5, 14)), from the cells
 */
/* kept at (6, 28). */
static void scAlpineCrossingHidePath(void) {
	fdMapGridCopyCellsTo(6, 28, 5, 13, 1, 2);
	fdMapGridCopyCellAttributes(6, 28, 1, 2, 5, 13);
	fdEventWait(1);
}

/* Update function of the ice pillar: Isaac is drawn in front of it while he */
/* stands higher than its top, so that he shows standing on it, and behind it */
/* otherwise. */
static UNREAD_RESULT scAlpineCrossingSortPillarAgainstLeader(ACTOR *pillar) {
	if(fdActorGet(ACTOR_PARTY_LEADER)->y.fixed > pillar->y.fixed) {
		pillar->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	}
	else {
		pillar->priFlg &= (u8)~ACTOR_PRIORITY_UNDERFOOT;
	}
}

/* Nothing visible happens as Frost reaches the puddle at the foot of the */
/* gully. When Isaac casts it from north of the puddle, the game draws the */
/* puddle in front of the scenery, so that the pillar about to rise from it is
 */
/* not hidden behind the ground he stands on. */
static void scAlpineCrossingPrepareFrost(void) {
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
	ACTOR *puddle = fdActorGet(ACTOR_PUDDLE);

	if(leader->z.fixed <= puddle->z.fixed) {
		fdActorSetSprPriority(ACTOR_PUDDLE, 1);
	}
}

/* Frost on the puddle at the foot of the gully (cell (20, 18)) freezes it into
 */
/* a tall ice pillar, which the party can jump onto from either side. The */
/* pillar's cell, and its twin in the map's second layer at (56, 18), become */
/* jump cells like the one kept at (55, 16). */
static void scAlpineCrossingFreezePuddle(void) {
	fdEventStart();

	fdActorGet(ACTOR_PUDDLE)->priFlg &= (u8)~ACTOR_PRIORITY_UNDERFOOT;
	fdActorGet(ACTOR_PUDDLE)->collisionFlg &= (u8)~ACTOR_PUSHABLE;
	fdActorGet(ACTOR_PUDDLE)->motionFlg = 0;
	fdActorGet(ACTOR_PUDDLE)->update =
		(void (*)(ACTOR *))scAlpineCrossingSortPillarAgainstLeader;

	fdMapGridCopyCellAttributes(55, 16, 1, 1, 56, 18);
	fdMapGridCopyCellAttributes(55, 16, 1, 1, 20, 18);

	sySynch(1);
	gmFlagSet(FLAG_ALPINE_CROSSING_PUDDLE_FROZEN);
	fdActorSetSprPriority(ACTOR_PUDDLE, 2);
	fdEventEnd();
}

/* Growth on the sprout at the foot of the cliff below the signpost (cell */
/* (21, 12)) grows it into a vine up the cliff, which the party can climb. Its
 */
/* two cells take the climbable cells kept at (14, 6). */
static void scAlpineCrossingGrowSprout(void) {
	fdEventStart();
	fdMapGridCopyCellAttributes(14, 6, 1, 2, 21, 11);
	fdActorGet(ACTOR_SPROUT)->collisionFlg = (u8)~ACTOR_SOLID;
	gmFlagSet(FLAG_ALPINE_CROSSING_SPROUT_GROWN);
	fdEventEnd();
}

const TRIGGER *scGetTriggers(void) {
	if(MainRoutineStateWork.next == MAP_ALPINE_CROSSING) {
		return AlpineCrossing_Triggers;
	}
	return LamaTemple_Triggers;
}

/* Sets up whichever scene is being entered and runs its arrival events. */
s32 scInit(void) {
	s32 entrance;

	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	if(MainRoutineStateWork.next == MAP_LAMA_TEMPLE) {
		/* Lama Temple: the wind blows (SE_AMBIENT_WIND), two monks meditate,
		 * and */
		/* the mossy rock stands over the chest. The chest cell's attributes are
		 */
		/* kept at (21, 73) for Reveal to restore. */
		scQueueMusic(SE_AMBIENT_WIND);
		fdActorSetAnimation(ACTOR_MEDITATING_MONK, ANIM_MONK_MEDITATE);
		fdActorSetAnimation(ACTOR_MONK_SENSING_ANXIETY, ANIM_MONK_MEDITATE);
		fdActorSetAnimation(ACTOR_MOSSY_ROCK, ANIM_TURNING_ROCK_MOSSY);
		fdMapGridCopyCellAttributes(21, 9, 1, 1, 21, 73);
		scLamaTempleHideChest();
		if(gmFlagGet(FLAG_HSU_RESCUED)) {
			fdActorSetPos(ACTOR_CHI_MONK, 136, 256);
			fdActorFaceDirection(ACTOR_CHI_MONK, FACING_EAST, 0);
		}
		entrance = MainRoutineStateWork.entrance;
		if(entrance == LAMA_TEMPLE_FROM_LACQUER_ROOMS_A_HAMAS_ROOM) {
			gmFlagClear(FLAG_SHOW_AREA_NAME);
		}
		else if(entrance == LAMA_TEMPLE_FROM_ALPINE_CROSSING &&
				!gmFlagGet(FLAG_KEEP_PARTY_POSITION)) {
			scLamaTempleRunHamasFarewell();
		}
	}
	else if(MainRoutineStateWork.next == MAP_ALPINE_CROSSING) {
		/* Alpine Crossing: restore the Psynergy puzzles already solved. */
		fdActorSetSprFlags(fdActorGet(ACTOR_PUDDLE), 0);
		fdActorGet(ACTOR_PUDDLE)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		if(gmFlagGet(FLAG_ALPINE_CROSSING_PUDDLE_FROZEN)) {
			fdActorSetAnimation(ACTOR_PUDDLE, PUDDLE_ANIMATION_PILLAR);
			scAlpineCrossingFreezePuddle();
		}
		if(gmFlagGet(FLAG_ALPINE_CROSSING_SPROUT_GROWN)) {
			fdActorSetAnimation(ACTOR_SPROUT, SPROUT_ANIMATION_GROWN);
			scAlpineCrossingGrowSprout();
		}
		if(MainRoutineStateWork.entrance ==
			   ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_WEST ||
		   MainRoutineStateWork.entrance ==
			   ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_EAST) {
			gmFlagClear(FLAG_SHOW_AREA_NAME);
		}

		/* Feizhi is here only once she has set out after Hsu. */
		if(!gmFlagGet(FLAG_FEIZHI_WENT_TO_HSU) &&
		   !gmFlagGet(FLAG_XIAN_HEARD_HSU_TRAPPED) &&
		   !gmFlagGet(FLAG_HSU_RESCUED)) {
			fdActorSetPos(ACTOR_FEIZHI, 0, 0);
		}

		/* Arriving from the north after Hama's lesson, before the party has
		 * freed */
		/* Hsu: he has been freed without it and the boulder is gone. */
		if(!fdEventGetFlag(FLAG_HSU_RESCUED) &&
		   gmFlagGet(FLAG_XIAN_HEARD_HSU_TRAPPED) &&
		   MainRoutineStateWork.entrance ==
			   ALPINE_CROSSING_FROM_WORLD_MAP_NORTH) {
			fdActorSetPos(ACTOR_BOULDER, 0, 0);
			gmFlagSet(FLAG_HSU_RESCUED);
			gmFlagSet(FLAG_HSU_RESCUED_WITHOUT_PARTY);
			fdActorSetPos(ACTOR_FEIZHI, 0, 0);
		}

		/* Once Hsu is free, the rockslide and a rock west of him are cleared */
		/* away. */
		if(fdEventGetFlag(FLAG_HSU_RESCUED)) {
			fdMapGridCopyCellsTo(54, 21, 53, 21, 1, 2);
			fdMapGridCopyCellAttributes(18, 20, 1, 3, 17, 21);
			fdMapGridCopyCellsTo(44, 18, 43, 17, 1, 1);
			fdMapGridCopyCellAttributes(8, 17, 1, 1, 7, 17);
		}

		/* After Hama's lesson, while Hsu is still trapped, Hama and Feizhi
		 * stand */
		/* by him, and the workers from Xian have not come yet. */
		if(gmFlagGet(FLAG_XIAN_HEARD_HSU_TRAPPED) &&
		   !gmFlagGet(FLAG_HSU_RESCUED)) {
			fdActorSetPos(ACTOR_FIRST_WORKER, 0, 0);
			fdActorSetPos(ACTOR_SECOND_WORKER, 0, 0);
			fdActorSetPos(ACTOR_HAMA, 192, 264);
			fdActorSetPos(ACTOR_HSU, 164, 280);
			fdActorSetPos(ACTOR_FEIZHI, 184, 304);
			fdActorFaceDirection(ACTOR_HAMA, FACING_SOUTH_SOUTHWEST, 0);
			fdActorFaceDirection(ACTOR_FEIZHI, FACING_NORTH_NORTHWEST, 0);
			fdActorSetScript(ACTOR_HSU, AlpineCrossing_HsuStruggle);
			fdActorGet(ACTOR_HSU)->scaleX = -FIXED_ONE;
		}
		if(!fdEventGetFlag(FLAG_HSU_RESCUED)) {
			fdActorSetPos(ACTOR_HSU, 164, 280);
			fdActorSetScript(ACTOR_HSU, AlpineCrossing_HsuStruggle);
			fdActorGet(ACTOR_HSU)->scaleX = -FIXED_ONE;
		}
		if(MainRoutineStateWork.entrance ==
			   ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_EAST &&
		   !gmFlagGet(FLAG_HAMA_REACHED_HSU) &&
		   !gmFlagGet(FLAG_KEEP_PARTY_POSITION) &&
		   !gmFlagGet(FLAG_HSU_RESCUED)) {
			scAlpineCrossingHamaReachesHsu();
		}
	}
	return 0;
}

/* The first worker's actor placement script: he is drawn behind the scenery. */
s32 AlpineCrossing_WorkerActorPlacementScript[] = {
	SC_CALL(scActorDrawBehindBackground),
	SC_END,
};

/* Feizhi's run down the path toward Hsu, walking, then standing. */
s32 AlpineCrossing_FeizhiPath[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(392, 0, 232),
	SC_SYNC,
	SC_MOVE(376, 0, 232),
	SC_SYNC,
	SC_MOVE(344, 0, 264),
	SC_SYNC,
	SC_MOVE(344, 0, 312),
	SC_SYNC,
	SC_MOVE(312, 0, 344),
	SC_SYNC,
	SC_MOVE(296, 0, 344),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

/* The party leader follows Feizhi most of the way. */
s32 AlpineCrossing_LeaderPath[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(392, 0, 232),
	SC_SYNC,
	SC_MOVE(376, 0, 232),
	SC_SYNC,
	SC_MOVE(344, 0, 264),
	SC_SYNC,
	SC_MOVE(344, 0, 312),
	SC_SYNC,
	SC_MOVE(328, 0, 328),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

/* Hsu, pinned under the boulder, struggles now and then. */
s32 AlpineCrossing_HsuStruggle[] = {
	SC_ANIMATION(ANIM_STAND),
	SC_ANIMATION(ANIM_HSU_STRUGGLE),
	SC_WAIT(240),
	SC_ANIMATION(ANIM_STAND),
	SC_ANIMATION(ANIM_HSU_STRUGGLE),
	SC_WAIT(60),
	SC_ANIMATION(ANIM_STAND),
	SC_ANIMATION(ANIM_HSU_STRUGGLE),
	SC_WAIT(300),
	SC_LOOP(SC_FOREVER, SC_TOP),
	SC_END,
};

/* The door of Hama's room sliding open, and shut again. */
u16 LamaTemple_DoorOpenCells[] = {
	MAPGRID_ANIMATION_STEP(66, 46, 3, 2, 5),
	MAPGRID_ANIMATION_STEP(63, 46, 3, 2, 5),
	MAPGRID_ANIMATION_STEPS_END,
};
u16 LamaTemple_DoorCloseCells[] = {
	MAPGRID_ANIMATION_STEP(66, 46, 3, 2, 5),
	MAPGRID_ANIMATION_STEP(69, 46, 3, 2, 5),
	MAPGRID_ANIMATION_STEPS_END,
};

SCENE_ENTRANCE LamaTemple_Entrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 496, 0, 216, FACING_WEST, 0, -1, -1,
	 -1, -1, 0},
	{LAMA_TEMPLE_FROM_WORLD_MAP, CONDITION_ALWAYS, 216, 0, 520, FACING_NORTH, 0,
	 -1, -1, -1, -1, 0},
	{LAMA_TEMPLE_FROM_LACQUER_ROOMS_A_HAMAS_ROOM, CONDITION_ALWAYS, 136, 0, 168,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{LAMA_TEMPLE_FROM_ALPINE_CROSSING, CONDITION_ALWAYS, 144, 0, 200,
	 FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{LAMA_TEMPLE_AFTER_DEFEAT, CONDITION_ALWAYS, 232, 0, 352, FACING_SOUTH, 0,
	 -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE AlpineCrossing_Entrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 496, 0, 216, FACING_WEST, 0, -1, -1,
	 -1, -1, 0},
	{ALPINE_CROSSING_FROM_WORLD_MAP_EAST, CONDITION_ALWAYS, 496, 0, 216,
	 FACING_WEST, 0, 16, 32, 520, 416, 0},
	{ALPINE_CROSSING_FROM_WORLD_MAP_NORTH, CONDITION_ALWAYS, 384, 0, 72,
	 FACING_SOUTH, 0, 16, 32, 520, 416, 0},
	{ALPINE_CROSSING_FROM_WORLD_MAP_WEST, CONDITION_ALWAYS, 40, 0, 280,
	 FACING_EAST, 0, 16, 32, 520, 416, 0},
	{ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_WEST, CONDITION_ALWAYS, 88,
	 0, 248, FACING_SOUTH, 0, 16, 32, 520, 416, 0},
	{ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_EAST, CONDITION_ALWAYS, 216,
	 0, 248, FACING_SOUTH, 0, 16, 32, 520, 416, 0},
	{SCENE_TABLE_END},
};

u32 AlpineCrossingLamaTemple_Exits[] = {
	SCENE_EXITS(LamaTemple),
	SCENE_EXIT(LAMA_TEMPLE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_LAMA_TEMPLE),
	SCENE_EXIT(LAMA_TEMPLE_TO_LACQUER_ROOMS_A_HAMAS_ROOM, LacquerRoomsA,
			   LACQUER_ROOMS_A_HAMAS_ROOM_FROM_LAMA_TEMPLE),
	SCENE_EXITS(AlpineCrossing),
	SCENE_EXIT(ALPINE_CROSSING_TO_WORLD_MAP_EAST, WorldMap,
			   WORLD_MAP_FROM_ALPINE_CROSSING_EAST),
	SCENE_EXIT(ALPINE_CROSSING_TO_WORLD_MAP_NORTH, WorldMap,
			   WORLD_MAP_FROM_ALPINE_CROSSING_NORTH),
	SCENE_EXIT(ALPINE_CROSSING_TO_WORLD_MAP_WEST, WorldMap,
			   WORLD_MAP_FROM_ALPINE_CROSSING_WEST),
	SCENE_EXIT(ALPINE_CROSSING_TO_ALTIN_PEAK_SOUTH_ENTRANCE_WEST,
			   AltinPeakSouthEntrance,
			   ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_WEST),
	SCENE_EXIT(ALPINE_CROSSING_TO_ALTIN_PEAK_SOUTH_ENTRANCE_EAST,
			   AltinPeakSouthEntrance,
			   ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_EAST),
	SCENE_EXIT(ALPINE_CROSSING_TO_LAMA_TEMPLE, LamaTemple,
			   LAMA_TEMPLE_FROM_ALPINE_CROSSING),
	SCENE_EXITS_END,
};

/* clang-format off */
ACTOR_PLACE LamaTemple_ActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_HAMA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MONK_IN_ORANGE_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        168, 0, 336, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MONK_IN_ORANGE_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        216, 0, 240, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MONK_IN_ORANGE_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        312, 0, 320, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MONK_IN_ORANGE_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        344, 0, 320, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MONK_IN_ORANGE_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        120, 0, 176, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_TURNING_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        344, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        232, 0, 328, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE AlpineCrossing_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_HAMA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_HSU, CONDITION_ALWAYS, (s32)AlpineCrossing_HsuStruggle,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FEIZHI, FLAG_HSU_RESCUED, MOVEMENT_TYPE_STAND,
        296, 0, 344, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, FLAG_HSU_RESCUED, MOVEMENT_TYPE_STAND,
        152, 0, 276, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUTH_WITH_HEADBAND, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), (s32)AlpineCrossing_WorkerActorPlacementScript,
        456, 0, 200, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUTH_WITH_TIED_HAIR, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MOVEMENT_TYPE_STAND,
        184, 0, 264, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        328, 0, 296, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        344, 0, 194, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

TRIGGER LamaTemple_Triggers[] = {
    { TRIGGER_EXIT, CELL_BEHAVIOR_LAMA_TEMPLE_SOUTH_ROAD, CONDITION_ALWAYS, LAMA_TEMPLE_TO_WORLD_MAP },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_LAMA_TEMPLE_DOOR, CONDITION_ALWAYS, EVENT(scLamaTempleEnterHamasRoom) },
    { TRIGGER_TALK, ACTOR_GREETING_MONK, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleThisIsLamaTempleWelcomeTravelers) },
    { TRIGGER_TALK, ACTOR_MONK_ASKING_ABOUT_THE_EAST, FLAG_HSU_RESCUED, EVENT(scLamaTempleTalkToMonkAskingAboutTheEast) },
    { TRIGGER_TALK, ACTOR_MEDITATING_MONK, FLAG_HSU_RESCUED, EVENT(scLamaTempleTalkToMeditatingMonk) },
    { TRIGGER_TALK, ACTOR_MONK_SENSING_ANXIETY, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleYouWhoSpeakToMeI) },
    { TRIGGER_TALK, ACTOR_CHI_MONK, FLAG_HSU_RESCUED, EVENT(scLamaTempleTalkToChiMonk) },
    { TRIGGER_TALK, ACTOR_GREETING_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleItIsVeryDangerousToCross) },
    { TRIGGER_TALK, ACTOR_MONK_ASKING_ABOUT_THE_EAST, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleTheEvilDesertIsHotterThan) },
    { TRIGGER_TALK, ACTOR_MEDITATING_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleIBelieveItIsPossibleTo) },
    { TRIGGER_TALK, ACTOR_MONK_SENSING_ANXIETY, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleIWillBeMostThankfulOnce) },
    { TRIGGER_TALK, ACTOR_CHI_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleWhenYourTemperatureReachesItsMaximum) },
    { TRIGGER_TALK, ACTOR_TRAVELING_HEALER, CONDITION_ALWAYS, EVENT(scLamaTempleTalkToHealer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GREETING_MONK, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleTheySeemToBeAnxiousThey) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MONK_ASKING_ABOUT_THE_EAST, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleTheySayAPersonCanOvercome) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MEDITATING_MONK, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleIAmHungry) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MONK_SENSING_ANXIETY, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleISenseImpureThoughtsFromThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHI_MONK, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleChiIsButAFragmentOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GREETING_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleICannotCrossTheDesertIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MONK_ASKING_ABOUT_THE_EAST, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleTheEvilInTheDesertDistorts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MEDITATING_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleHowNiceItWouldBeIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MONK_SENSING_ANXIETY, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleMasterHamaWillBeVerySad) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHI_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleWithRevealYouWillFindOases) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELING_HEALER, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleForSomeReasonThesePeopleGive) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(scLamaTempleRevealChest) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(scLamaTempleHideChest) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_LAMA_TEMPLE_WALL_WRITING, CONDITION_ALWAYS, EVENT(scLamaTempleReadWallWriting) },
    { EXAMINE_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_LAMA_TEMPLE_PSYNERGY_STONE, FLAG_LAMA_TEMPLE_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { EXAMINE_TRIGGER_FACING(FACING_NORTHEAST), CELL_BEHAVIOR_LAMA_TEMPLE_PSYNERGY_STONE, FLAG_LAMA_TEMPLE_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { EXAMINE_TRIGGER_FACING(FACING_NORTHWEST), CELL_BEHAVIOR_LAMA_TEMPLE_PSYNERGY_STONE, FLAG_LAMA_TEMPLE_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { EXAMINE_TRIGGER_FACING(FACING_WEST), CELL_BEHAVIOR_LAMA_TEMPLE_PSYNERGY_STONE, FLAG_LAMA_TEMPLE_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_LAMA_TEMPLE_CHEST, FLAG_LAMA_TEMPLE_WATER_OF_LIFE, FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { TRIGGER_TABLE_END },
};

TRIGGER AlpineCrossing_Triggers[] = {
    { TRIGGER_EXIT, CELL_BEHAVIOR_ALPINE_CROSSING_EAST_ROAD, CONDITION_ALWAYS, ALPINE_CROSSING_TO_WORLD_MAP_EAST },
    { TRIGGER_EXIT, CELL_BEHAVIOR_ALPINE_CROSSING_NORTH_ROAD, CONDITION_ALWAYS, ALPINE_CROSSING_TO_WORLD_MAP_NORTH },
    { TRIGGER_EXIT, CELL_BEHAVIOR_ALPINE_CROSSING_WEST_ROAD, CONDITION_ALWAYS, ALPINE_CROSSING_TO_WORLD_MAP_WEST },
    { TRIGGER_EXIT, CELL_BEHAVIOR_ALPINE_CROSSING_WEST_CAVE_MOUTH, CONDITION_ALWAYS, ALPINE_CROSSING_TO_ALTIN_PEAK_SOUTH_ENTRANCE_WEST },
    { TRIGGER_EXIT, CELL_BEHAVIOR_ALPINE_CROSSING_EAST_CAVE_MOUTH, CONDITION_ALWAYS, ALPINE_CROSSING_TO_ALTIN_PEAK_SOUTH_ENTRANCE_EAST },
    { TRIGGER_STEP, CELL_BEHAVIOR_ALPINE_CROSSING_FEIZHI_ARRIVAL, FLAG_FEIZHI_SAW_ROCKSLIDE, EVENT(scAlpineCrossingFeizhiArrives) },
    { TRIGGER_TALK, ACTOR_FEIZHI, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT(scAlpineCrossingTalkToFeizhi) },
    { TRIGGER_TALK, ACTOR_HAMA, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleWellIsaacAdeptsYouCameTo) },
    { TRIGGER_TALK, ACTOR_HSU, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleIAmTrappedBetweenTheseTwo) },
    { TRIGGER_TALK, ACTOR_FEIZHI, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleBeStrongHsuWeWillHelp) },
    { TRIGGER_TALK, ACTOR_FIRST_WORKER, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleWeMovedTheRocksNowPeople) },
    { TRIGGER_TALK, ACTOR_SECOND_WORKER, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleManyPeopleFromXianHelpedMove) },
    { PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_BOULDER, FLAG_HSU_RESCUED, EVENT(scAlpineCrossingLiftBoulder) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEIZHI, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgAlpineCrossingLamaTempleHsuIsInDangerICan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HAMA, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleHowShouldWeMoveTheBoulders) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HSU, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleIWantToScreamButFeizhi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEIZHI, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleHsuTiresHeLooksExhaustedI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_WORKER, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleWeClearedTheWayButWhat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_WORKER, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleIAlmostHurtMyBackIt) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(scAlpineCrossingRevealPath) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(scAlpineCrossingHidePath) },
    /* Frost's first phase, as it reaches the puddle */
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PUDDLE, FLAG_ALPINE_CROSSING_PUDDLE_FROZEN, EVENT(scAlpineCrossingPrepareFrost) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, FLAG_ALPINE_CROSSING_PUDDLE_FROZEN, EVENT(scAlpineCrossingFreezePuddle) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_SPROUT, FLAG_ALPINE_CROSSING_SPROUT_GROWN, EVENT(scAlpineCrossingGrowSprout) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_ALPINE_CROSSING_SIGNPOST, CONDITION_ALWAYS, EVENT(scAlpineCrossingReadSignpost) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_ALPINE_CROSSING_CHEST, FLAG_ALPINE_CROSSING_POWER_BREAD, FIND_ITEM(ITEM_POWER_BREAD) },
    { TRIGGER_TABLE_END },
};
/* clang-format on */

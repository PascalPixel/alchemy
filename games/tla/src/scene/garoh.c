// Garoh, the village of werewolves that guards Air's Rock, in five scenes that share this overlay:
// the way up from the world map (SceneIdGarohWayUp), the path up to the village (SceneIdGarohPath,
// whose chest holds the Hypnos Sword), the village itself (SceneIdGaroh), the tunnels under its
// houses (SceneIdGarohTunnels) and the cave behind the rock by the sanctum
// (SceneIdGarohWerewolfCave).
//
// The party first comes on a night of the full moon. Climbing the way up, the light turns to
// evening and then to night (scGarohCrossLowerTwilightLine, scGarohCrossUpperTwilightLine); on the
// path a werewolf howls and runs past into the village (scGarohWerewolfRunsPast). The hooded
// villagers say there are no werewolves, and by the sanctum an old werewolf peers out of the big
// grey rock and hides again (scGarohWerewolfPeersFromCave). With Reveal from Air's Rock the party
// opens the rock, follows the little werewolf into the cave (scGarohFollowWerewolfIntoCave) and is
// surrounded by the werewolves, whose elder Maha tells them to come back later
// (scGarohWerewolvesRevealThemselves). By day the villagers show their faces, and Maha tells the
// werewolves' legend and gives the party Ether (scGarohMahaTellsTheLegend).

// This unit declares its own form of fdRopeLash, below.
#define OWN_fdRopeLash

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

// The ROM's callers here are compiled with no declaration in scope, as against this form: no
// parameter list and an Sint32 result, where the definition takes the rope and two posts and
// returns nothing.
Sint32 fdRopeLash();
#define EFFECT_SET_MODE   fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"

#include "macros.h"

void scGarohSinkPillar(Sint32 id, ACTOR *actor);
void scGarohSinkFirstPillarOnMetatile(void);
void scGarohSinkSecondPillarOnMetatile(void);
void scGarohCheckThirdPillar(void);
void scGarohSinkFourthPillarOnMetatile(void);
void scGarohScatterDustRing(Sint32 id);
#include "field/script_inline.h"
#include "scene/scene.h"
#include "field/field_mode.h"
#include "graphics/palette_buffers.h"
#include "game/game.h"
#include "field/call.h"
#include "field/actor_script.h"
#include "text/msg_ids.h"

extern ACTOR_PLACE GarohVillageDayActorPlacements[];
extern CUE         GarohWayUpCues[];
void               scGarohWerewolvesRevealThemselves(void);
void               scGarohMahaTellsTheLegend(void);
void               scGarohInitVillage(void);
void               scGarohInitWayUp(void);
extern Uint32      GarohWerewolfLookAroundScript[];
Sint32             scGarohMetWerewolves();
void               scGarohOpenDoorAndEnter(void);
void               scGarohWerewolfPeersFromCave(void);
void               scGarohEnterOpening(void);
void               scGarohMeetInTheCave(void);
void               scGarohPlaceFirstPillar(void);
void               scGarohPlaceSecondPillar(void);
void               scGarohPlaceThirdPillar(void);
void               scGarohPlaceFourthPillar(void);
void               scGarohPlacePushedPillar(void);
void               scGarohRevealTunnelPassage(void);
void               scGarohHideCaveEntrance(void);
void               scGarohRestoreWayUpLight(void);
void               scGarohHideTunnelPassage(void);
void               scGarohHideCavePassage(void);
void               scGarohCrossLowerTwilightLine(void);
void               scGarohCrossUpperTwilightLine(void);
extern Uint32      FieldEffectShortScript[];
extern Uint32      FieldEffectLongScript[];
extern Sint32      GarohShrunkSpriteScript[];
extern Sint16      GarohDoorFrames[];
extern Sint16      GarohWideDoorFrames[];
extern Sint16      GarohTallDoorFrames[];
void               scGarohLashRope(void);
// BG2's horizontal offset for each screen line while the path's layer ripples by night
// (scGarohWavePathLayer).
Uint16 GarohWaveLines[DISPLAY_HEIGHT] BSS_DATA ALIGNED(16);

// The village's people by night and by day (SceneIdGaroh), numbering the rows of
// GarohVillageActorPlacements and GarohVillageDayActorPlacements: the five who go hooded by night
// show their faces by day.
typedef enum GAROH_VILLAGE_ACTOR_ID
{
	// By night he asks whether the party has seen a werewolf; by day he has heard from Maha that
	// the party is special.
	ACTOR_VILLAGER_ASKING_ABOUT_THE_WEREWOLF = ACTOR_FIRST_PLACED,
	// "Forgive me for not taking my hood off..."
	ACTOR_HOODED_VILLAGER,
	// "Gaaaroooooo!" ... "I was just calling my dog."
	ACTOR_HOWLING_VILLAGER,
	// "Strangers should not go about at night..."
	ACTOR_VILLAGER_WARNING_STRANGERS,
	// "Don't you love the sight of the full moon?"
	ACTOR_VILLAGER_ON_THE_FULL_MOON,
	// By day only: "I was in the cave with Maha! I was that little werewolf!"
	ACTOR_LITTLE_WEREWOLF,
	// The old werewolf who peers out of the cave's entrance by the Sanctum the first night: Maha,
	// on his sprite in the cave.
	ACTOR_MAHA_AT_THE_CAVE_ENTRANCE = ACTOR_FIRST_PLACED + 13,
	ACTOR_CAVE_ENTRANCE
} GAROH_VILLAGE_ACTOR_ID;

// The path up to the village and the werewolves' cave (SceneIdGarohPath and
// SceneIdGarohWerewolfCave), numbering the rows of GarohWerewolfCaveActorPlacements; GarohPath
// places the werewolf and Kraden on the same rows.
typedef enum GAROH_WEREWOLF_ACTOR_ID
{
	ACTOR_WEREWOLF = ACTOR_FIRST_PLACED,
	ACTOR_KRADEN,
	ACTOR_MAHA,
	ACTOR_CAVE_PASSAGE,
	ACTOR_ETHER,
	// The cave's fires, shrunk to wall lamps; the first two burn behind the party.
	ACTOR_FIRE_A = ACTOR_FIRST_PLACED + 5,
	ACTOR_FIRE_B
} GAROH_WEREWOLF_ACTOR_ID;

// The tightrope posts on the path up to the village: Jenna, Sheba and Piers take their own numbers,
// so the posts follow the werewolf and Kraden.
enum
{
	ACTOR_PATH_TIGHTROPE_POST_A = ACTOR_FIRST_PLACED + 2,
	ACTOR_PATH_TIGHTROPE_POST_B
};

// The stone pillars on the way up to the village (SceneIdGarohWayUp), numbering the rows of
// GarohWayUpActorPlacements: Move or a push takes each onto the slot where it sinks.
typedef enum GAROH_WAY_UP_ACTOR_ID
{
	ACTOR_FIRST_PILLAR = ACTOR_FIRST_PLACED,
	ACTOR_SECOND_PILLAR,
	ACTOR_THIRD_PILLAR,
	ACTOR_FOURTH_PILLAR
} GAROH_WAY_UP_ACTOR_ID;

// The metatiles the step and examine cues answer, by their behaviour in each scene's map. A door's
// or an opening's behaviour is the exit it leaves by.
typedef enum GAROH_METATILE_BEHAVIOR
{
	// The village: east of the sanctum's door, metatile (20, 16).
	METATILE_BEHAVIOR_BY_SANCTUM_DOOR     = 20,
	// The village: the well, metatiles (19..20, 18..19).
	METATILE_BEHAVIOR_GAROH_WELL          = 200,
	// The path up to the village: across it at metatiles (9..11, 19).
	METATILE_BEHAVIOR_WEREWOLF_RUNS_PAST  = 30,
	// The way up: where the light turns to evening, metatile (13, 26), and to night, metatile (16,
	// 22).
	METATILE_BEHAVIOR_LOWER_TWILIGHT_LINE = 20,
	METATILE_BEHAVIOR_UPPER_TWILIGHT_LINE = 21,
	// The werewolves' cave: below the cleft, metatiles (17..18, 49), and below where Maha stands,
	// metatiles (19..21, 15).
	METATILE_BEHAVIOR_CAVE_MOUTH          = 30,
	METATILE_BEHAVIOR_BELOW_MAHA          = 31
} GAROH_METATILE_BEHAVIOR;

// Story flags of the werewolves' secret, in the order the party meets them.
#define FLAG_GAROH_REACHED_VILLAGE   0x919
#define FLAG_GAROH_SAW_WEREWOLF      0x91c
#define FLAG_GAROH_FOLLOWED_WEREWOLF 0x91d
#define FLAG_GAROH_MET_WEREWOLVES    0x91e
#define FLAG_GAROH_HEARD_MAHA        0x91f
// For this visit to the area: the old werewolf has peered from the cave, the little werewolf has
// thanked the party, and the way up is in the evening's light, between its two lines.
#define FLAG_WEREWOLF_PEERED_OUT     FLAG_AREA_TEMP_20
#define FLAG_LITTLE_WEREWOLF_THANKED FLAG_AREA_TEMP_01
#define FLAG_EVENING                 FLAG_AREA_TEMP_18
// By night the village's actors take the warm tint as well as its map. Nothing in the code sets it,
// so they keep their own colours.
#define FLAG_NIGHT_TINTS_ACTORS      FLAG_AREA_TEMP_11

// The passage within the werewolves' cave behind the round grey rock, which Reveal opens
// (scGarohEnterOpening): from before the rock to the little room beyond it and back (scene_exits.h
// names only the exits between scenes). An exit and the entrance on its side share a number.
typedef enum GAROH_WEREWOLF_CAVE_PASSAGE
{
	GAROH_WEREWOLF_CAVE_TO_ROOM_BEHIND_ROCK = 6,
	GAROH_WEREWOLF_CAVE_ROOM_BEHIND_ROCK_TO_CAVE,
	GAROH_WEREWOLF_CAVE_FROM_ROOM_BEHIND_ROCK = 6,
	GAROH_WEREWOLF_CAVE_ROOM_BEHIND_ROCK_FROM_CAVE
} GAROH_WEREWOLF_CAVE_PASSAGE;

// The werewolf sheet's last picture, held: sitting on the ground.
enum
{
	ANIM_WEREWOLF_SIT = 5
};

// The way up: the pillars sunk into their slots, and the rope Lashed.
#define FLAG_FIRST_PILLAR_SUNK           FLAG_SCENE_TEMP_01
#define FLAG_SECOND_PILLAR_SUNK          FLAG_SCENE_TEMP_02
#define FLAG_GAROH_ROPE_LASHED           FLAG_SCENE_TEMP_03
#define FLAG_FOURTH_PILLAR_SUNK          FLAG_SCENE_TEMP_04
// The two lines on the way up where the light changes, crossed since the party came in.
#define FLAG_CROSSED_LOWER_TWILIGHT_LINE FLAG_SCENE_TEMP_10
#define FLAG_CROSSED_UPPER_TWILIGHT_LINE FLAG_SCENE_TEMP_11

// The houses' lit windows, colour 14 of BG palette 1, kept white when the village darkens for the
// night.
#define WINDOW_LIGHT                     14

// The way into the path up to the village that no exit uses: by night it tints the path purple
// rather than warm and dark.
#define ENTRANCE_PATH_PURPLE_NIGHT       12

// The steps in which the Ether grows from two fifths of its size to full.
#define ETHER_GROW_STEPS                 20

// By day the path up to the village's second layer takes the first's scale and wrap.
void scGarohMatchSecondLayer(void)
{
	MAP_STATE *map  = RamHeapSlots->mapGrid;
	MAP_LAYER *to   = &map->layers[1];
	MAP_LAYER *from = &map->layers[0];

	to->scaleX      = from->scaleX;
	to->scaleY      = from->scaleY;
	to->maskX       = from->maskX;
	to->maskY       = from->maskY;
}

// By night the second map layer of the path up to the village ripples: each line is offset by two
// sines of the tick count, handed to an HBlank DMA.
void scGarohWavePathLayer(void)
{
	Sint32     x;
	// The sine function itself, called at the address sySin's stub call jumps to.
	Sint32     (*sine)(Sint32) = *(Sint32 (**)(Sint32))(((Uint32)sySin & ~3) + 4);
	MAP_LAYER *layer           = fdMapGridGetLayer(1);
	Uint16    *line            = GarohWaveLines;

	if (FieldStateWork->revealActive == 0)
	{
		Uint32           frame;
		Sint32           z;
		Sint32           i;
		volatile Uint16 *dma;

		layer->speedY = 0;
		frame         = (Uint8)TickCount;
		i             = 0;
		// FAKEMATCH(barrier): i and line held through an empty asm; as no
		// * asm.
		asm("" : "+r"(i), "+r"(line) : "r"(line));
		z = FIXEDtoINT(layer->y);
		x = FIXEDtoINT(layer->x);
		// FAKEMATCH(dead): a once-only block that places the two waves' steps
		// * and angles around the loop; as them declared with z.
		{
			// The two waves: their angles at the top line, and the steps each line turns them on
			// by.
			Sint32 stepA = TRIG_TURN * 51 / 256;
			Sint32 stepB = TRIG_TURN * 153 / 2048;
			Sint32 a     = z * stepA + frame * (TRIG_TURN / 64);
			Sint32 b     = z * stepB - frame * (TRIG_TURN / 64);

			for (; i != DISPLAY_HEIGHT;)
			{
				Sint32 s1 = sine(a);
				Sint32 s2 = sine(b);

				i++;
				// FAKEMATCH(dead): a store to z, which nothing reads again.
				if (i == 300 || x == 300)
					z = 0;
				*line++  = FIXEDtoINT((s1 * 3 + s2 * 7) >> 3) + x;
				a       += stepA;
				b       += stepB;
			}
		}
		line         = GarohWaveLines;
		REG_BG2HOFS  = line[0];
		dma          = REG_DMA0;
		dma[5]      &= ~DMA_START_TIMING_DRQ_REPEAT;
		dma[5]      &= ~DMA_ENABLE;
		(void)dma[5];
		syDMAXCopy(
			line + 1, (void *)&REG_BG2HOFS, DMA_HBLANK_HALFWORD_PER_LINE, (volatile Uint32 *)dma);
	}
}

extern Uint16 GarohPathDayColors[];
extern Uint16 GarohPathNightColors[];

// Writes count colours from first on, from a table of red, green and blue halfwords, to the BG
// palette and the palette fade's source.
#define DEFINE_GAROH_LOAD_PALETTE(name, components, first, count)                                  \
	void name(void)                                                                                \
	{                                                                                              \
		PALETTE_FADE *buffers = RamHeapSlots->paletteFade;                                         \
		Uint32        i;                                                                           \
                                                                                                   \
		for (i = 0; i < count; i++)                                                                \
		{                                                                                          \
			BG_PLTT[first + i] =                                                                   \
				components[i * 3 + 2] << 10 | components[i * 3 + 1] << 5 | components[i * 3];      \
			buffers->palette[first + i] = BG_PLTT[first + i];                                      \
		}                                                                                          \
	}

// By day the path's colours: BG palette 1's colours 8 to 15.
DEFINE_GAROH_LOAD_PALETTE(scGarohLoadPathDayColors, GarohPathDayColors, 24, 8)

// By night the path's colours: BG palette 6's colours 1 to 9.
DEFINE_GAROH_LOAD_PALETTE(scGarohLoadPathNightColors, GarohPathNightColors, 97, 9)

// A script's call: clears the actor's sprite flags.
Sint32 scGarohClearScriptedActorSpriteFlags(ACTOR *actor)
{
	fdActorSetSpriteFlags(actor, 0);
	return 0;
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	return 0;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

extern ACTOR_PLACE                 GarohActorPlacements[];
extern ACTOR_PLACE                 GarohVillageActorPlacements[];
extern ACTOR_PLACE                 GarohWayUpActorPlacements[];
extern ACTOR_PLACE                 GarohTunnelsActorPlacements[];
extern ACTOR_PLACE                 GarohPathActorPlacements[];
extern ACTOR_PLACE                 GarohWerewolfCaveActorPlacements[];

extern MAP_GRID_METATILE_ANIMATION GarohDoors[];

// The people of the scene being entered: in the village, the hooded villagers by night or their
// faces once Maha has shared the secret.
const void                        *scGetActorPlacements(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if (scene == SCENE_GAROH)
	{
		if (fdScriptGetFlag(FLAG_GAROH_SECRET_SHARED))
			return GarohVillageDayActorPlacements;
		return GarohVillageActorPlacements;
	}
	if (scene == SCENE_GAROH_PATH)
		return GarohPathActorPlacements;
	if (scene == SCENE_GAROH_WAY_UP)
		return GarohWayUpActorPlacements;
	if (scene == SCENE_GAROH_TUNNELS)
		return GarohTunnelsActorPlacements;
	if (scene == SCENE_GAROH_WEREWOLF_CAVE)
		return GarohWerewolfCaveActorPlacements;
	return GarohActorPlacements;
}

// Walking north into the door of one of Garoh's round houses, or of the sanctum, opens it and Felix
// walks inside, while the villagers stand still: the young werewolf's family's home at metatile
// (14, 23), the old couple's at (8, 14), the weapons and armor shop's wide door at (10, 24), the
// inn at (21, 25) and the sanctum's tall door at (14, 15).
//
// The door's metatile behaviour is the exit it leaves by, which also picks its opening ticks from
// `GarohDoors`.
void scGarohOpenDoorAndEnter(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	Sint32       index;
	Sint32       i;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	for (i = ACTOR_FIRST_PLACED; (Uint32)i <= ACTOR_COUNT - 1; i++)
	{
		{
			ACTOR *actor = fdActorGet(i);

			if (actor != NULL)
				actor->motionFlags = 0;
		}
	}
	index = state->stepCue;
	fdScriptPlaySound(SE_DOOR_OPEN);
	index--;
	fdMapGridAnimateMetatiles(GarohDoors[index].steps, GarohDoors[index].x, GarohDoors[index].y);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	{
		Sint32 owner = MainRoutineStateWork.leaderActorId;

		// The sanctum's tall door is entered straight on, the others from the centre of their
		// metatile.
		if (index == GAROH_TO_GAROH_ROOMS_SANCTUM - 1)
			fdActorSetDestinationOffset(owner, 0, -4);
		else
			fdActorCenterAndWalk(owner, 2, -4);
	}
	syFieldScriptWait(6);
	syFieldScriptSetExitRequest(state->stepCue);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptEnd();
}

// Talking at night to the hooded villager outside the young werewolf's family's home: "So, uh...you
// say you saw a werewolf?" Yes gets "Don't be ridiculous... There's no such thing as werewolves!";
// No gets its own reply.
void scGarohAskIfSawWerewolf(Sint32 actor)
{
	Sint32 msg = (Sint32)MsgGarohSoUhYouSayYou;

	syFieldScriptSetMessageId(msg);
	fdScriptOpenMessage(actor, 0);
	if (syFieldScriptAskYesNoWithActorAnimation(syFieldPartyGetLeaderActor(), 0) == 0)
	{
		syFieldScriptWait(10);
		syFieldScriptSetMessageId(msg + 1);
	}
	else
	{
		syFieldScriptWait(20);
		syFieldScriptSetMessageId(msg + 2);
	}
	fdScriptShowMessage(actor, 0);
}

// Talking at night to the hooded villager outside the Item Shopkeeper's house: "Don't you love the
// sight of the full moon?" Yes gets "Yes, I agree... But it brings me grief."; No gets its own
// reply.
void scGarohAskAboutFullMoon(Sint32 actor)
{
	Sint32 msg = (Sint32)MsgGarohDontYouLoveTheSight;

	syFieldScriptSetMessageId(msg);
	fdScriptOpenMessage(actor, 0);
	if (syFieldScriptAskYesNoWithActorAnimation(syFieldPartyGetLeaderActor(), 0) == 0)
	{
		syFieldScriptWait(10);
		syFieldScriptSetMessageId(msg + 1);
	}
	else
	{
		syFieldScriptWait(20);
		syFieldScriptSetMessageId(msg + 2);
	}
	fdScriptShowMessage(actor, 0);
}

// Talking by day to the woman in green south-west of the well, the villager who warned strangers
// off at night: "Maha said that you were the only outsiders we could share our secret with...", and
// she asks whether it bothers them that everyone here is a werewolf.
void scGarohAskAboutSecret(Sint32 actor)
{
	Sint32 msg = (Sint32)MsgGarohMahaSaidThatYouWere;

	syFieldScriptSetMessageId(msg);
	fdScriptOpenMessage(actor, 0);
	if (syFieldScriptAskYesNoWithActorAnimation(syFieldPartyGetLeaderActor(), 0) == 0)
	{
		syFieldScriptWait(10);
		syFieldScriptSetMessageId(msg + 1);
	}
	else
	{
		syFieldScriptWait(20);
		syFieldScriptSetMessageId(msg + 2);
	}
	fdScriptShowMessage(actor, 0);
}

// Talking by day to the grey-haired man west of the well, who claimed at night to be calling his
// dog: he howls "Awoooooh!" and is glad he no longer has to hide it.
void scGarohHowlOutLoud(Sint32 actor)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syFieldScriptSetMessageId((Sint32)MsgGarohAwoooooh);
	// "Awoooooh!"
	syFieldScriptShowMessageThenWait(actor, 0, 10);
	// "Heh heh... Sorry about that! I'm so happy I don't have to hide that..."
	syFieldScriptShowMessageThenWait(actor, 0, 10);
	fdScriptEnd();
}

// Talking at night to the hooded villager west of the well: he howls "Gaaaroooooo!", catches
// himself ("Gah! Whoops...") and says he was just calling his dog. His code would cut him to the
// howl alone once FLAG_AREA_TEMP_00 is set, but nothing in Garoh, its paths, tunnels, cave or rooms
// sets that flag, so the player always hears all three lines.
void scGarohHowlForDog(Sint32 actor)
{
	Sint32 flag;
	Sint32 msg;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	flag = fdScriptGetFlag(FLAG_AREA_TEMP_00);
	msg  = (Sint32)MsgGarohGaaarooooooForDog;
	if (flag)
	{
		syFieldScriptSetMessageId(msg);
	}
	else
	{
		syFieldScriptSetMessageId(msg);
		// "Gaaaroooooo!"
		syFieldScriptShowMessageThenWait(actor, 0, 10);
		// "Gah! Whoops..."
		syFieldScriptShowMessageThenWait(actor, 0, 10);
	}
	syFieldScriptShowMessageThenWait(actor, 0, 10);
	fdScriptEnd();
}

// Talking by day to the fair-haired boy running about by the sanctum, or reading his mind the first
// time: "Hey, it's you guys! Thanks for last night!", and he explains he was the little werewolf in
// the cave. Later he says he is the reason the village no longer needs its hoods.
void scGarohLittleWerewolfThanks(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	scFieldActorStop(ACTOR_LITTLE_WEREWOLF);
	fdActorSetAnimation(ACTOR_LITTLE_WEREWOLF, ANIM_STAND);
	syFieldScriptWait(10);
	fdActorFaceActor(ACTOR_LITTLE_WEREWOLF, ACTOR_FELIX, 0);
	if (fdScriptGetFlag(FLAG_LITTLE_WEREWOLF_THANKED))
	{
		syFieldScriptSetMessageId((Sint32)MsgGarohImTheReasonYouCan);
	}
	else
	{
		syFieldScriptSetMessageId((Sint32)MsgGarohHeyItsYouGuysThanks);
		// "Hey, it's you guys! Thanks for last night!"
		syFieldScriptShowMessageThenWait(ACTOR_LITTLE_WEREWOLF, 0, 10);
		scFieldActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 15);
		scFieldActorShowEmote(ACTOR_LITTLE_WEREWOLF, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 10);
		// "Oh, yeah... That's right... You probably don't recognize me in my..."
		syFieldScriptShowMessageThenWait(ACTOR_LITTLE_WEREWOLF, 0, 5);
		scFieldActorShowEmote(ACTOR_LITTLE_WEREWOLF, EMOTE_IN_FRONT | EMOTE_SMILE, 15);
		// "I was in the cave with Maha! I was that little werewolf!"
		syFieldScriptShowMessageThenWait(ACTOR_LITTLE_WEREWOLF, 0, 5);
	}
	syFieldScriptShowMessageThenWait(ACTOR_LITTLE_WEREWOLF, 0, 10);
	fdScriptSetFlag(FLAG_LITTLE_WEREWOLF_THANKED);
	// Back to running about the village.
	fdActorSetScript(ACTOR_LITTLE_WEREWOLF, MOVEMENT_TYPE_WANDER);
	fdScriptEnd();
}

// Walking past the sanctum's door at night before meeting the werewolves, metatile (20, 16): the
// camera looks up at the big grey rock on the ledge above, an old grey-maned werewolf opens it with
// Reveal and steps out, cries "What!? Outsiders, here?" and slips back inside, the rock closing
// behind him. Once a visit.
void scGarohWerewolfPeersFromCave(void)
{
	Sint32 flag;
	Sint32 facing;

	void   scGarohOpenCaveEntrance(Sint32 id)
	{
		fdPsynergyLoadResources(ABILITY_REVEAL, 1);
		fdPsynergySetupActorPair(id, 0);
		fdPsynergyRunHandler(FIELD_PSYNERGY_HANDLER_ON_TARGET);
		fdBattleEffectStopMainActor();
		fdPsynergyEnd();
	}

	void scGarohCloseCaveEntrance(Sint32 id)
	{
		fdPsynergyLoadResources(ABILITY_REVEAL, 1);
		fdPsynergySetupActorPair(id, 0);
		fdPsynergyRunHandler(FIELD_PSYNERGY_HANDLER_EXTRA);
		fdBattleEffectStopMainActor();
		fdPsynergyEnd();
	}

	flag = fdScriptGetFlag(FLAG_WEREWOLF_PEERED_OUT);
	if (flag == 0)
	{
		fdScriptSetFlag(FLAG_WEREWOLF_PEERED_OUT);
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		fdActorSetSpriteFlags(fdActorGet(ACTOR_CAVE_ENTRANCE), 0);
		facing = FACING_SOUTH;
		fdActorSetPosAndFacing(ACTOR_CAVE_ENTRANCE, toFIXED(360), toFIXED(199), facing);
		fdCameraMoveTo(359, CAMERA_KEEP, 220, 1);
		fdCameraWaitForArrival();
		scGarohOpenCaveEntrance(ACTOR_CAVE_ENTRANCE);
		syFieldScriptWait(30);
		fdActorSetPosAndFacing(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE, toFIXED(360), toFIXED(199), facing);
		fdActorWalkByAndWait(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE, 0, 16);
		scGarohCloseCaveEntrance(ACTOR_CAVE_ENTRANCE);
		syFieldScriptSetMessageId((Sint32)MsgGarohWhatOutsidersHere);
		fdActorFaceDirectionAndWait(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE, FACING_SOUTH_SOUTHWEST);
		fdActorRunRepeatedMotion(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE, 2);
		syFieldScriptWait(20);
		fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
		// "What!? Outsiders, here?"
		syFieldScriptShowMessageThenWait(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE, 0, 10);
		scFieldActorShowEmote(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
		fdActorGet(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE)->steeringFlags &= ~ACTOR_FACES_MOTION;
		fdActorFaceDirection(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE, facing, 0);
		syFieldScriptWait(30);
		scGarohOpenCaveEntrance(ACTOR_CAVE_ENTRANCE);
		syFieldScriptWait(20);
		fdActorWalkByAndWait(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE, 0, -16);
		fdActorGet(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE)->motionFlags = flag;
		fdActorWalkByAndWait(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE, 0, -16);
		fdActorSetPos(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE, 0, 0);
		syFieldScriptWait(10);
		scGarohCloseCaveEntrance(ACTOR_CAVE_ENTRANCE);
		syFieldScriptWait(60);
		fdActorSetPos(ACTOR_CAVE_ENTRANCE, 0, 0);
		fdScriptEnd();
	}
}

extern CUE    GarohCues[];
extern CUE    GarohVillageCues[];
extern CUE    GarohVillageDayCues[];
extern CUE    GarohPathCues[];
extern CUE    GarohTunnelsCues[];
extern CUE    GarohWerewolfCaveCues[];

extern Sint32 GarohHowlTicks;

// Walking north into an opening Reveal has shown takes the party through it: the cleft at the foot
// of the big grey rock north-east of the sanctum, metatile (22, 11), leads down into the
// werewolves' cave, and the passage behind the round grey rock in the cave leads on.
void          scGarohEnterOpening(void)
{
	FIELD_STATE *event = RamHeapSlots->fieldState;

	fdScriptPlaySound(SE_MAP_EXIT);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdScriptSetFlag(FLAG_ARRIVAL_STAIRS_DOWN);
	syFieldScriptSetExitRequest(event->stepCue);
}

// Walking north to metatiles (19..21, 15) of the werewolves' cave, below where Maha stands: the
// first night the werewolves surround the party, and by day Maha is waiting there to tell the
// legend.
void scGarohMeetInTheCave(void)
{
	if (!fdScriptGetFlag(FLAG_GAROH_SECRET_SHARED))
	{
		if (!fdScriptGetFlag(FLAG_GAROH_MET_WEREWOLVES))
			scGarohWerewolvesRevealThemselves();
	}
	else
	{
		if (!fdScriptGetFlag(FLAG_GAROH_HEARD_MAHA))
			scGarohMahaTellsTheLegend();
	}
}

// The cues of the scene being entered: in the village, the night's until Maha has shared the secret
// and the day's after.
const void *scGetCues(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if (scene == SCENE_GAROH)
	{
		if (fdScriptGetFlag(FLAG_GAROH_SECRET_SHARED))
			return GarohVillageDayCues;
		return GarohVillageCues;
	}
	if (scene == SCENE_GAROH_PATH)
		return GarohPathCues;
	if (scene == SCENE_GAROH_WAY_UP)
		return GarohWayUpCues;
	if (scene == SCENE_GAROH_TUNNELS)
		return GarohTunnelsCues;
	if (scene == SCENE_GAROH_WEREWOLF_CAVE)
		return GarohWerewolfCaveCues;
	return GarohCues;
}

// A howl comes each time the night's ticks under this mask are 0.
#define HOWL_PERIOD_MASK 0x3ff

// A wolf howls in the distance every 1,024 ticks while it is night.
void scGarohHowlAtNight(void)
{
	if (!fdScriptGetFlag(FLAG_GAROH_SECRET_SHARED) && fdScriptGetFlag(FLAG_GAROH_NIGHT))
	{
		GarohHowlTicks++;
		if ((GarohHowlTicks & HOWL_PERIOD_MASK) == 0)
			fdScriptPlaySound(SE_HOWL);
	}
}

void   scGarohInitPath(void);

// Sets up the scene being entered: the cave's lamps, and Maha once he has told the legend; the
// village by night or day; the path up to the village; or the way up, which also makes the sanctum
// the place a defeat returns the party to once they have reached the village.
Sint32 scInit(void)
{
	FIELD_STATE *state;

	if (MainRoutineStateWork.next == SCENE_GAROH_TUNNELS)
	{
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	}
	else if (MainRoutineStateWork.next == SCENE_GAROH_WEREWOLF_CAVE)
	{
		scFieldActorSetSpritePriority(ACTOR_FIRE_A, 1);
		scFieldActorSetSpritePriority(ACTOR_FIRE_B, 1);
		if (fdScriptGetFlag(FLAG_GAROH_HEARD_MAHA))
		{
			fdActorSetPosAndFacing(ACTOR_MAHA, toFIXED(328), toFIXED(162), FACING_SOUTH);
		}
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
		fdEffectStartBlankOverlay(0);
		if (MainRoutineStateWork.entrance == GAROH_WEREWOLF_CAVE_FROM_ROOM_BEHIND_ROCK)
		{
			state             = RamHeapSlots->fieldState;
			state->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
			fdScriptClearFlag(FLAG_ARRIVAL_STAIRS_UP);
			fdScriptClearFlag(FLAG_ARRIVAL_STAIRS_DOWN);
			FieldStateWork->transition =
				SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
			fdScriptStartScreenTransition();
			fdScriptWaitTransitionTicks();
		}
	}
	else
	{
		FieldStateWork->transition = 0;
		if (MainRoutineStateWork.next == SCENE_GAROH)
		{
			fdScriptSetFlag(FLAG_GAROH_REACHED_VILLAGE);
			scGarohInitVillage();
		}
		else if (MainRoutineStateWork.next == SCENE_GAROH_PATH)
		{
			scGarohInitPath();
		}
		else if (MainRoutineStateWork.next == SCENE_GAROH_WAY_UP)
		{
			scGarohInitWayUp();
			if (fdScriptGetFlag(FLAG_GAROH_REACHED_VILLAGE))
			{
				MainRoutineStateWork.restartScene    = SCENE_GAROH_ROOMS_SANCTUM;
				MainRoutineStateWork.restartEntrance = GAROH_ROOMS_SANCTUM_FROM_GAROH;
			}
		}
	}
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

// The village: by night darkened, its windows lit and the wolves howling; by day the cleft in the
// big grey rock and the cave's other ways stand open.
void scGarohInitVillage(void)
{
	syFieldSetTickEvent(scGarohHowlAtNight, EVENT_PASS_TICK);
	FieldStateWork->transition = 0;
	if (fdScriptGetFlag(FLAG_GAROH_SECRET_SHARED) == 0 || fdScriptGetFlag(FLAG_GAROH_NIGHT) != 0)
	{
		fdScriptSetFlag(FLAG_GAROH_NIGHT);
		fdPaletteFadeApplyTarget(PALETTE_TINT_WARM_DARK, PALETTES_BG);
		if (fdScriptGetFlag(FLAG_NIGHT_TINTS_ACTORS))
			fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, PALETTES_OBJECTS);
		fdPaletteFadeInterpolate(1);
		fdScriptWaitTransitionTicks();
		fdPaletteFadePackTarget();
		{
			PALETTE_FADE *fade                                  = RamHeapSlots->paletteFade;

			*(volatile Uint16 *)&BG_PLTT_COLOR(1, WINDOW_LIGHT) = RGB_WHITE;
			fade->palette[PALETTE_COLORS + WINDOW_LIGHT] =
				*(volatile Uint16 *)&BG_PLTT_COLOR(1, WINDOW_LIGHT);
		}
		fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, 1);
	}
	else
	{
		fdMapGridCopyMetatilesTo(10, 32, 10, 10, 1, 1);
		fdMapGridCopyMetatilesTo(12, 32, 25, 22, 1, 1);
		fdMapGridCopyMetatilesTo(11, 33, 8, 22, 1, 1);
		fdMapGridCopyMetatilesTo(74, 32, 74, 10, 1, 1);
		fdMapGridCopyMetatilesTo(76, 32, 89, 22, 1, 1);
		fdMapGridCopyMetatilesTo(75, 33, 72, 22, 1, 1);
	}
	if (MainRoutineStateWork.entrance == GAROH_FROM_GAROH_WEREWOLF_CAVE)
	{
		FIELD_STATE *state = RamHeapSlots->fieldState;

		state->transition  = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
		fdScriptClearFlag(FLAG_ARRIVAL_STAIRS_UP);
		fdScriptClearFlag(FLAG_ARRIVAL_STAIRS_DOWN);
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionTicks();
		FieldStateWork->transition = 0;
	}
}

// The path up to the village: the tightrope strung, and by night its dark colours and rippling
// water, by day its hidden ways open.
void scGarohInitPath(void)
{
	sySetTickEvent(scGarohHowlAtNight, EVENT_PASS_TICK);
	fdTightropeInit();
	fdTightropePlace(0, ACTOR_PATH_TIGHTROPE_POST_A, ACTOR_PATH_TIGHTROPE_POST_B);
	if (fdScriptGetFlag(FLAG_GAROH_NIGHT))
	{
		if (MainRoutineStateWork.entrance == ENTRANCE_PATH_PURPLE_NIGHT)
		{
			fdPaletteFadeApplyTarget(PALETTE_TINT_CUSTOM | RGB(9, 6, 18), 1);
		}
		else
		{
			fdPaletteFadeApplyTarget(PALETTE_TINT_WARM_DARK, 1);
		}
		fdPaletteFadeInterpolate(1);
		fdScriptWaitTransitionTicks();
		fdPaletteFadePackTarget();
		scGarohLoadPathNightColors();
		fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, 1);
		sySetTickEvent((Sint32)scGarohWavePathLayer, EVENT_PASS_VBLANK);
	}
	else
	{
		fdMapGridCopyMetatilesTo(20, 7, 9, 7, 5, 5);
		fdMapGridCopyMetatilesTo(20, 71, 9, 71, 5, 5);
		fdMapGridCopyMetatilesTo(84, 7, 73, 7, 5, 5);
		scGarohLoadPathDayColors();
		fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, 1);
		scGarohMatchSecondLayer();
	}
}

extern Sint16 GarohPillars[];

// The way up from the world map: the four stone pillars where they were left, in the evening's or
// the night's light once the party has climbed into it.
void          scGarohInitWayUp(void)
{
	syFieldSetTickEvent(scGarohHowlAtNight, EVENT_PASS_TICK);
	if (fdScriptGetFlag(FLAG_GAROH_NIGHT))
	{
		if (fdScriptGetFlag(FLAG_EVENING))
			fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, 1);
		else
			fdPaletteFadeApplyTarget(PALETTE_TINT_WARM_DARK, 1);
	}
	fdActorPlaceBlocks(GarohPillars);
	fdPaletteFadeInterpolate(1);
	fdScriptWaitTransitionTicks();
}

// Nothing visible happens as Felix starts casting Move on one of the stone pillars on the way up;
// the game lifts the pillar off the map's metatiles so that Move can slide it.
void scGarohLiftPillar(void)
{
	fdActorLiftTargetBlock(GarohPillars);
}

// Nothing visible happens as Felix starts pushing one of the stone pillars on the way up; as with
// Move, the game first lifts the pillar off the map's metatiles so that it can slide.
void scGarohLiftPushedPillar(void)
{
	fdActorLiftTargetBlock(GarohPillars);
}

// A pillar set down on its slot sinks 32 pixels into the ground with a whoosh and a ring of dust.
void scGarohSinkPillar(Sint32 id, ACTOR *actor)
{
	Sint32 i;
	Sint32 saved;

	actor->motionFlags = 0;
	saved              = id;
	for (i = 16; i > 0; i--)
	{
		actor->y.fixed -= toFIXED(2);
		syFieldScriptWait(1);
	}
	fdScriptPlaySound(SE_WHOOSH);
	scGarohScatterDustRing(saved);
}

// Defines a check that sinks pillar actorId (scGarohSinkPillar) once it stands on metatile
// (metatileX, metatileZ), and sets flag so it sinks once.
#define DEFINE_GAROH_CUE_AT_METATILE(name, actorId, flag, metatileX, metatileZ)                    \
	void name(void)                                                                                \
	{                                                                                              \
		ACTOR *actor = fdActorGet(actorId);                                                        \
		Sint32 x     = FIELD_METATILE(actor->x.fixed);                                             \
		Sint32 raw;                                                                                \
		Sint32 z;                                                                                  \
                                                                                                   \
		raw = actor->z.fixed;                                                                      \
		z   = FIELD_METATILE(raw);                                                                 \
		if (!fdScriptGetFlag(flag) && x == metatileX && z == metatileZ)                            \
		{                                                                                          \
			scGarohSinkPillar(actorId, actor);                                                     \
			fdScriptSetFlag(flag);                                                                 \
		}                                                                                          \
	}

// The first pillar sinks once it stands on metatile (10, 29), one west of where it starts.
DEFINE_GAROH_CUE_AT_METATILE(
	scGarohSinkFirstPillarOnMetatile, ACTOR_FIRST_PILLAR, FLAG_FIRST_PILLAR_SUNK, 10, 29)

// The second pillar sinks once it stands on metatile (15, 28), one east of where it starts.
DEFINE_GAROH_CUE_AT_METATILE(
	scGarohSinkSecondPillarOnMetatile, ACTOR_SECOND_PILLAR, FLAG_SECOND_PILLAR_SUNK, 15, 28)

// The third pillar has no slot to sink into: its check reads the pillar and the rope's flag and
// does nothing.
void scGarohCheckThirdPillar(void)
{
	fdActorGet(ACTOR_THIRD_PILLAR);
	fdScriptGetFlag(FLAG_GAROH_ROPE_LASHED);
}

// The fourth pillar sinks once it stands on metatile (15, 25), one south of where it starts.
void scGarohSinkFourthPillarOnMetatile(void)
{
	ACTOR *actor = fdActorGet(ACTOR_FOURTH_PILLAR);
	Sint32 x     = FIELD_METATILE(actor->x.fixed);
	Sint32 z     = FIELD_METATILE(actor->z.fixed);

	if (!fdScriptGetFlag(FLAG_FOURTH_PILLAR_SUNK) && x == 15 && z == 25)
	{
		scGarohSinkPillar(ACTOR_FOURTH_PILLAR, actor);
		fdScriptSetFlag(FLAG_FOURTH_PILLAR_SUNK);
	}
}

// Move on the first stone pillar, on the lowest ledge of the way up at metatile (11, 29): slid one
// metatile west, onto metatile (10, 29), it drops off the ledge and sinks into the ground with a
// whoosh and a ring of dust until its top is level with the ledge, so that Felix can walk west off
// the ledge onto it. From the ledge its top is a dead end: holding west or south there takes him no
// further, and he can only step back east onto the ledge. The second pillar's top is the same
// (scGarohPlaceSecondPillar).
void scGarohPlaceFirstPillar(void)
{
	fdActorPlaceMovedBlock();
	scGarohSinkFirstPillarOnMetatile();
}

// Move on the second stone pillar, east of the first at metatile (14, 28): slid one metatile east,
// onto metatile (15, 28), it drops off its ledge onto the path below and sinks into the ground with
// a whoosh and a ring of dust until its top is level with the ledge. Felix can walk east onto its
// top, but holding east or south there takes him no further.
void scGarohPlaceSecondPillar(void)
{
	fdActorPlaceMovedBlock();
	scGarohSinkSecondPillarOnMetatile();
}

// Move on the third stone pillar, on the ledge above at metatile (10, 24): it slides where Move
// sends it, and has no slot to sink into.
void scGarohPlaceThirdPillar(void)
{
	fdActorPlaceMovedBlock();
	scGarohCheckThirdPillar();
}

// Move on the fourth stone pillar, east of the third at metatile (15, 24): cast from the ledge
// north of it and slid one metatile south, onto metatile (15, 25), it drops off the ledge onto the
// path below and sinks into the ground with a whoosh and a ring of dust until its top is level with
// the ledge. Felix can walk south onto its top, but like the first two it is a dead end: holding
// south, west or east there takes him no further.
void scGarohPlaceFourthPillar(void)
{
	fdActorPlaceMovedBlock();
	scGarohSinkFourthPillarOnMetatile();
}

// Pushing a stone pillar on the way up sinks it just as Move does when it comes to rest on its
// slot: the first on metatile (10, 29), the second on (15, 28), the fourth on (15, 25).
//
// The push rows answer only while the first pillar has not sunk.
void scGarohPlacePushedPillar(void)
{
	fdActorPlaceMovedBlock();
	scGarohSinkFirstPillarOnMetatile();
	scGarohSinkSecondPillarOnMetatile();
	scGarohCheckThirdPillar();
	scGarohSinkFourthPillarOnMetatile();
}

// Reveal in the village shows the cleft at the foot of the big grey rock north-east of the sanctum,
// metatile (22, 11), as a way in.
void scGarohRevealCaveEntrance(void)
{
	fdMapGridCopyMetatilesTo(98, 8, 86, 8, 1, 2);
	fdMapGridCopyMetatileAttributes(34, 9, 1, 1, 22, 11);
}

// Reveal in the tunnels shows a hidden passage at metatiles (11, 33..35).
void scGarohRevealTunnelPassage(void)
{
	fdMapGridCopyMetatilesTo(25, 47, 11, 33, 1, 3);
	fdMapGridCopyMetatileAttributes(25, 47, 1, 1, 11, 33);
}

// Reveal in the werewolves' cave shows a passage through the round grey rock in its middle,
// metatiles (19..21, 5..7).
void scGarohRevealCavePassage(void)
{
	fdMapGridCopyMetatilesTo(44, 5, 19, 5, 3, 3);
	fdMapGridCopyMetatilesTo(108, 5, 83, 5, 3, 2);
	fdMapGridCopyMetatileAttributes(44, 5, 3, 3, 19, 5);
}

// Reveal wearing off in the village hides the cleft in the big grey rock again.
void scGarohHideCaveEntrance(void)
{
	fdMapGridCopyMetatilesTo(102, 8, 86, 8, 1, 2);
	fdMapGridCopyMetatileAttributes(22, 10, 1, 1, 22, 11);
}

// Reveal wearing off on the path up to the village: at night the path's dark colours, there before
// the cast, come back exactly as they were. This row plays no part in it: it only checks whether it
// is night in Garoh, and does nothing with the answer.
Sint32 scGarohIsNight(void)
{
	return fdScriptGetFlag(FLAG_GAROH_NIGHT);
}

// Reveal wearing off on the way up brings back the evening's or the night's light.
void scGarohRestoreWayUpLight(void)
{
	if (fdScriptGetFlag(FLAG_GAROH_NIGHT))
	{
		if (fdScriptGetFlag(FLAG_EVENING))
			fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, 1);
		else
			fdPaletteFadeApplyTarget(PALETTE_TINT_WARM_DARK, 1);
		fdPaletteFadeInterpolate(16);
	}
}

// Reveal wearing off in the tunnels hides their passage again.
void scGarohHideTunnelPassage(void)
{
	fdMapGridCopyMetatilesTo(8, 30, 11, 33, 1, 3);
	fdMapGridCopyMetatileAttributes(8, 30, 1, 1, 11, 33);
}

// Reveal wearing off in the werewolves' cave closes the round grey rock again.
void scGarohHideCavePassage(void)
{
	fdMapGridCopyMetatilesTo(39, 5, 19, 5, 3, 3);
	fdMapGridCopyMetatilesTo(103, 5, 83, 5, 3, 2);
	fdMapGridCopyMetatileAttributes(39, 5, 3, 3, 19, 5);
}

// Climbing the way up from the world map before the werewolves' secret is shared, the step onto
// metatile (13, 26), the lower of its two lines, turns the daylight to a golden evening; coming
// down from the path, the same step brings the daylight back. Once a visit.
void scGarohCrossLowerTwilightLine(void)
{
	FIELD_STATE *state;

	if (!fdScriptGetFlag(FLAG_GAROH_SECRET_SHARED))
	{
		fdScriptSetFlag(FLAG_CROSSED_LOWER_TWILIGHT_LINE);
		if (MainRoutineStateWork.entrance == GAROH_WAY_UP_FROM_WORLD_MAP)
		{
			fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, 1);
			fdScriptSetFlag(FLAG_GAROH_NIGHT);
			fdScriptSetFlag(FLAG_EVENING);
		}
		else
		{
			fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, 1);
			fdScriptClearFlag(FLAG_GAROH_NIGHT);
			fdScriptClearFlag(FLAG_EVENING);
		}
		state = RamHeapSlots->fieldState;
		if (state->revealActive == 0)
			fdPaletteFadeInterpolate(120);
	}
}

// Climbing on up past metatile (16, 22), the upper of the way up's two lines, the evening darkens
// into night; coming down from the path, the same step turns the night to evening. Once a visit,
// before the werewolves' secret is shared.
void scGarohCrossUpperTwilightLine(void)
{
	FIELD_STATE *state;

	if (!fdScriptGetFlag(FLAG_GAROH_SECRET_SHARED))
	{
		fdScriptSetFlag(FLAG_CROSSED_UPPER_TWILIGHT_LINE);
		if (MainRoutineStateWork.entrance == GAROH_WAY_UP_FROM_GAROH_PATH)
		{
			fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, 1);
			fdScriptSetFlag(FLAG_GAROH_NIGHT);
			fdScriptSetFlag(FLAG_EVENING);
		}
		else
		{
			fdPaletteFadeApplyTarget(PALETTE_TINT_WARM_DARK, 1);
			fdScriptSetFlag(FLAG_GAROH_NIGHT);
			fdScriptClearFlag(FLAG_EVENING);
		}
		state = RamHeapSlots->fieldState;
		if (state->revealActive == 0)
			fdPaletteFadeInterpolate(120);
	}
}

#include "../field/field_effect_advance_motion.c"

#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef DEFINE_GAROH_LOAD_PALETTE
#undef DEFINE_GAROH_CUE_AT_METATILE
void scFieldEffectSpawn(Sint32                x,
                        Sint32                y,
                        Sint32                z,
                        Sint32                velocityX,
                        Sint32                velocityY,
                        Sint32                velocityZ,
                        Uint32                flags,
                        const EFFECT_OPTIONS *options);

// A ring of puffs of dust spreads out from the actor, one every eighth of a turn: nine, the first
// and last both to the east.
void scGarohScatterDustRing(Sint32 objectId)
{
	ACTOR         *actor;
	VECTOR3        vector;
	EFFECT_OPTIONS params;
	Uint32         angleStep;
	Sint32         angle;
	Sint32         x;
	FIXED          z;

	actor         = fdActorGet(objectId);
	params.update = fdEffectAdvanceMotion;
	angleStep     = 0;
	do
	{
		angle     = angleStep * (TRIG_TURN / DUST_RING_PUFFS);
		vector.x  = syCos(angle);
		vector.y  = 0;
		z         = syMathSin(angle);
		x         = vector.x;
		vector.z  = z;
		x        += x / 3;
		vector.x  = x;
		scFieldEffectSpawn(actor->x.fixed,
		                   actor->y.fixed,
		                   actor->z.fixed,
		                   x,
		                   vector.y,
		                   z,
		                   EFFECT_USE_UPDATE | 1,
		                   &params);
		angleStep += 2;
	} while (angleStep <= DUST_RING_PUFFS);
}

extern Uint32 GarohQuestionMarkScript[];

// Unused: Lash ties a rope between the third and fourth stone pillars on the way up; no cue runs
// it.
void          scGarohLashRope(void)
{
	fdRopeLash(1, ACTOR_THIRD_PILLAR, ACTOR_FOURTH_PILLAR);
	fdScriptSetFlag(FLAG_GAROH_ROPE_LASHED);
}

// Until the secret is shared, whether the party has met the werewolves.
Sint32 scGarohMetWerewolves(void)
{
	if (!fdScriptGetFlag(FLAG_GAROH_SECRET_SHARED))
		return fdScriptGetFlag(FLAG_GAROH_MET_WEREWOLVES);
}

// Pops a "?" bubble up at (x, y, z), tilted by rotation, for the given ticks.
void   scGarohPopQuestionMark(x, y, z, rotation, ticks) Sint32 x, y, z;
Sint16 rotation;
Sint32 ticks;
{
	ACTOR *object;
	SPR   *sprite;

	object = scFieldActorCreate(SPRITE_EMOTE, x, y, z);
	if (object != NULL)
	{
		sprite = object->sprite;
		fdActorSelectAnimation(object, EMOTE_QUESTION);
		fdActorStartScript(object, GarohQuestionMarkScript);
		object->motionFlags = 0;
		object->wait        = ticks;
		sprite->flags       = 0;
		sprite->rotation    = rotation;
	}
}

// Two question marks pop up over the actor's head in turn, right and left, each tilted, a third of
// a second apart.
void scGarohPopQuestionMarks(Sint32 id)
{
	ACTOR *actor = fdActorGet(id);

	scGarohPopQuestionMark(actor->x.fixed + toFIXED(8),
	                       actor->y.fixed + toFIXED(28),
	                       actor->z.fixed,
	                       TRIG_TURN / 16,
	                       20);
	fdScriptPlaySound(SE_LIGHT_APPEAR);
	syFieldScriptWait(20);
	scGarohPopQuestionMark(actor->x.fixed - toFIXED(8),
	                       actor->y.fixed + toFIXED(28),
	                       actor->z.fixed,
	                       TRIG_TURN * 15 / 16,
	                       20);
	fdScriptPlaySound(SE_LIGHT_APPEAR);
	syFieldScriptWait(20);
}

// Defines Reveal's effect played on actor at the cave's passage: the effect's phase, and the
// passage's 3 by 3 metatiles redrawn from column fromX.
#define DEFINE_GAROH_PLAY_EFFECT(name, phase, fromX)                                               \
	void name(Sint32 actor)                                                                        \
	{                                                                                              \
		fdPsynergyLoadResources(ABILITY_REVEAL, 1);                                                \
		fdPsynergySetupActorPair(actor, 0);                                                        \
		fdPsynergyRunHandler(phase);                                                               \
		fdMapGridCopyMetatilesLayered(fromX, 5, 3, 3, 19, 5);                                      \
		fdBattleEffectStopMainActor();                                                             \
		fdPsynergyEnd();                                                                           \
	}

// Reveal's effect played on the cave's passage as it opens.
DEFINE_GAROH_PLAY_EFFECT(scGarohOpenCavePassage, 1, 44)

// Reveal's effect played on the cave's passage as it closes.
DEFINE_GAROH_PLAY_EFFECT(scGarohCloseCavePassage, 3, 39)

// Sends an actor walking to where the leader stands, if there is a leader.
static inline void scWalkTowardLeader(Sint32 actorId)
{
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);

	if (leader != NULL)
	{
		fdActorSetDestination(actorId, leader->x.part[0].integer, leader->z.part[0].integer);
	}
}

// An actor walks into the leader and is taken off the map, as the party closes up into one walking
// sprite at the end of a script.
static inline void scWalkIntoLeader(Sint32 actorId)
{
	ACTOR *leader;

	fdActorSetAnimation(actorId, ANIM_WALK);
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	if (leader != NULL)
	{
		fdActorSetDestination(actorId, leader->x.part[0].integer, leader->z.part[0].integer);
	}
	scFieldActorWaitForMoveById(actorId);
	fdActorSetPos(actorId, 0, 0);
}

// The point i eighths of the way from from to to.
static inline Sint32 scGarohPathStep(Sint32 from, Sint32 to, Sint32 i)
{
	return from + ((to - from) * i) / 8;
}

// How far, in pixels, the werewolf weaves either side of the path as it runs past: across the
// screen, and up and down it.
enum
{
	WEAVE_ACROSS = 12,
	WEAVE_DOWN   = 7
};

// Walking up the path to the village the first night, metatiles (9..11, 19): a werewolf howls
// "Gaaaroooooo!" and bounds down the path past the pond and off into the village, and Kraden
// explains to the startled party what a werewolf is.
void scGarohWerewolfRunsPast(void)
{
	ACTOR  *werewolf;
	Sint32  i;
	Sint32  pos[3];
	Sint32 *posPtr;
	FIXED   x, z;
	FIXED   toX, toZ;

	fdScriptSetFlag(FLAG_GAROH_SAW_WEREWOLF);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorSetPos(ACTOR_WEREWOLF, 96, 8);
	syFieldScriptSetMessageId((Sint32)MsgGarohGaaaroooooo);
	// "Gaaaroooooo!"
	syFieldScriptShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorSetSpeed(ACTOR_FELIX, 0.8);
	fdActorWalkToAndWait(ACTOR_FELIX, 152, 304);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, 32, -16, FACING_NORTH_NORTHWEST);
	scFieldActorLaunchFromLeader(ACTOR_JENNA, 0, 0, FACING_NORTH);
	scFieldActorLaunchFromLeader(ACTOR_SHEBA, 16, 0, FACING_NORTH_NORTHWEST);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		scFieldActorLaunchFromLeader(ACTOR_PIERS, 8, 16, FACING_NORTH);
	fdActorWalkByAndWait(ACTOR_FELIX, -16, -16);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	scFieldActorWaitForMoveById(ACTOR_KRADEN);
	syFieldScriptWait(20);
	// "Aow-aow-oooo!!!"
	syFieldScriptShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	syFieldScriptWait(25);
	// "What was that!?"
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	// "Awoooooo!!!"
	syFieldScriptShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		scFieldActorStartRepeatedMotion(ACTOR_PIERS, 2);
	scFieldActorStartRepeatedMotion(ACTOR_SHEBA, 2);
	scFieldActorStartRepeatedMotion(ACTOR_KRADEN, 2);
	scFieldActorStartRepeatedMotion(ACTOR_FELIX, 2);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	syFieldScriptWait(20);
	fdCameraMoveTo(144, CAMERA_KEEP, 128, 1);
	fdCameraWaitForArrival();
	werewolf = fdActorGet(ACTOR_WEREWOLF);
	fdActorSetAnimation(ACTOR_WEREWOLF, ANIM_WALK);
	// The werewolf lopes down the path in eight bounds at random speeds, weaving from side to side.
	for (i = 0; i <= 7; i++)
	{
		Sint32 tenths;
		Sint32 wave;

		wave      = syMathSin(i * (DEGtoANG(45)));
		posPtr    = pos;
		posPtr[0] = scGarohPathStep(toFIXED(104), toFIXED(88), i) - wave * WEAVE_ACROSS;
		wave      = syMathSin(i * (DEGtoANG(45)));
		pos[2]    = scGarohPathStep(toFIXED(32), toFIXED(120), i) - wave * WEAVE_DOWN;
		pos[1]    = 0;
		tenths    = syRandomBelow(10) + 4;
		fdActorSetSpeedFX(ACTOR_WEREWOLF, tenths * (toFIXED(1) / 10), tenths * (toFIXED(1) / 20));
		fdActorWalkToAndWait(ACTOR_WEREWOLF, pos[0] / toFIXED(1), pos[2] / toFIXED(1));
	}
	fdActorSetAnimation(ACTOR_WEREWOLF, ANIM_STAND);
	syFieldScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_WEREWOLF, 2);
	fdActorSetScript(ACTOR_WEREWOLF, GarohWerewolfLookAroundScript);
	FieldStateWork->msg--;
	// "Awoooooo!!!"
	syFieldScriptShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	scFieldActorStop(ACTOR_WEREWOLF);
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_SOUTHEAST, 0);
	syFieldScriptWait(20);
	syScriptCameraFollowActor(ACTOR_WEREWOLF, 1);
	fdActorSetAnimation(ACTOR_WEREWOLF, ANIM_WALK);
	x   = werewolf->x.fixed;
	z   = werewolf->z.fixed;
	toX = toFIXED(128);
	toZ = toFIXED(184);
	for (i = 0; i <= 7; i++)
	{
		Sint32 tenths;
		FIXED  wave;

		wave   = syMathSin(i * (DEGtoANG(45)));
		pos[0] = scGarohPathStep(x, toX, i) - wave * 4;
		pos[2] = scGarohPathStep(z, toZ, i);
		pos[1] = 0;
		tenths = syRandomBelow(10) + 4;
		fdActorSetSpeedFX(ACTOR_WEREWOLF, tenths * (toFIXED(1) / 10), tenths * (toFIXED(1) / 20));
		fdActorWalkToAndWait(ACTOR_WEREWOLF, pos[0] / toFIXED(1), pos[2] / toFIXED(1));
	}
	fdActorSetAnimation(ACTOR_WEREWOLF, ANIM_STAND);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_SOUTH_SOUTHEAST, 0);
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_EAST, 0);
	syFieldScriptWait(30);
	// "Ah-ah-awoooo!!!"
	syFieldScriptShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	// "Eeek!"
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_WEREWOLF, 2);
	syFieldScriptWait(10);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_SOUTH_SOUTHEAST, 0);
	syFieldScriptWait(30);
	fdActorShowEmote(ACTOR_WEREWOLF, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
	fdActorSetSpeed(ACTOR_WEREWOLF, 0.6);
	fdActorGet(ACTOR_WEREWOLF)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_WEREWOLF, 128, 148);
	syFieldScriptWait(1);
	fdActorGet(ACTOR_WEREWOLF)->steeringFlags |= ACTOR_FACES_MOTION;
	fdActorSetSpeed(ACTOR_WEREWOLF, 2.5);
	fdActorWalkByAndWait(ACTOR_WEREWOLF, 0, 4);
	syFieldScriptWait(20);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_NORTH, 0);
	syFieldScriptWait(20);
	scFieldActorStartRepeatedMotion(ACTOR_WEREWOLF, 3);
	fdActorSetAttachedEffect(ACTOR_WEREWOLF, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(40);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_WEST, 0);
	syFieldScriptWait(10);
	fdActorSetSpeed(ACTOR_WEREWOLF, 2.4);
	fdActorWalkByAndWait(ACTOR_WEREWOLF, -24, -8);
	fdActorWalkByAndWait(ACTOR_WEREWOLF, -8, -100);
	syFieldScriptWait(30);
	fdActorSetPos(ACTOR_WEREWOLF, 0, 0);
	fdCameraMoveTo(168, CAMERA_KEEP, 280, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	scFieldActorJump(ACTOR_KRADEN, 6, 15);
	scFieldActorJump(ACTOR_KRADEN, 6, 23);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldScriptWait(10);
	// "I don't believe it! Was that a°"
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH_NORTHEAST, 0);
	syFieldScriptWait(25);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	syFieldScriptWait(10);
	// "A what? Kraden, do you know what that thing was?"
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	syFieldScriptWait(25);
	// "Well... Perhaps..."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	syFieldScriptWait(10);
	// "So, what are you waiting for!? Tell us!"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	// "I suspect it may have been...a lycanthrope."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	syFieldScriptWait(10);
	// "A lycanthwhat?"
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldScriptWait(10);
	// "Just think of them as a race of people born with special powers..."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	syFieldScriptWait(10);
	// "Kind of like Adepts, you mean?"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	// "Yes, that sounds right. Sheba, you've put it quite well."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldScriptWait(10);
	// "Adepts borrow their abilities from the power of the elements..."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldScriptWait(10);
	// "While lycanthropes borrow theirs from the power of beasts..."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
	syFieldScriptWait(30);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	// "This is why they take the forms of animals..."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD, 33);
	// "They may look frightening... But we must try to communicate with them."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorWalkByAndWait(ACTOR_KRADEN, -8, -8);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorWalkByAndWait(ACTOR_KRADEN, -10, -6);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorWalkByAndWait(ACTOR_KRADEN, -14, -4);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	syFieldScriptWait(30);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	syFieldScriptWait(20);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldScriptWait(10);
	// "Lycanthropes... Werewolves... A whole village in hiding..."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH_NORTHWEST, 0);
	syFieldScriptWait(20);
	// "Is it just me... Or does Kraden seem a little TOO happy to have found werewolves?" Yes:
	// "What? Me? Happy to find werewolves? You must be joking!" No: "I would love to study them,
	// but still...they do frighten me..."
	fdScriptOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
	if (syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0)
	{
		syFieldScriptWait(30);
		fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
		syFieldScriptWait(20);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
		syFieldScriptWait(20);
		if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
			fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
		// "What? Me? Happy to find werewolves? You must be joking!"
		syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
		FieldStateWork->msg++;
	}
	else
	{
		syFieldScriptWait(40);
		fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
		syFieldScriptWait(20);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
		syFieldScriptWait(20);
		if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
			fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
		FieldStateWork->msg++;
		// "I would love to study them, but still...they do frighten me..."
		syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
	}
	scFieldActorJump(ACTOR_KRADEN, 6, 15);
	scFieldActorJump(ACTOR_KRADEN, 6, 23);
	// "I guess I'm just curious about all of the secrets of these new lands..."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	syFieldScriptWait(10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	syFieldScriptWait(15);
	// "Any real scholar would be thrilled to explore a new land full of mystery, huh?"
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
	syFieldScriptWait(25);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldScriptWait(8);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldScriptWait(8);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldScriptWait(20);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	// "So, are there werewolves only on Osenia?"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0, 0);
	scFieldActorStartRepeatedMotion(ACTOR_KRADEN, 3);
	fdActorSetAttachedEffect(ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(40);
	// "Ummm..."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
	scGarohPopQuestionMarks(ACTOR_SHEBA);
	syFieldScriptWait(20);
	// "If so, how could you even have known about them, Kraden?"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldScriptWait(10);
	fdActorSetSpeed(ACTOR_KRADEN, 0.4);
	fdActorGet(ACTOR_KRADEN)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_KRADEN, 0, -16);
	syFieldScriptWait(1);
	fdActorGet(ACTOR_KRADEN)->steeringFlags |= ACTOR_FACES_MOTION;
	syFieldScriptWait(20);
	fdActorSetAttachedEffect(ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(40);
	// "Er... I, uh..."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	syFieldScriptWait(20);
	// "Yeah... That is weird."
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH_NORTHWEST, 0);
	syFieldScriptWait(20);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
	syFieldScriptWait(35);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
	syFieldScriptWait(25);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	syFieldScriptWait(20);
	fdActorSetSpeed(ACTOR_KRADEN, 1);
	fdActorWalkByAndWait(ACTOR_KRADEN, 0, 16);
	syFieldScriptWait(20);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	// "All right, so I lied!"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldScriptWait(10);
	// "I'm glad we found werewolves! There! Are you happy!?"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
	fdActorSetSpeed(ACTOR_KRADEN, 2);
	fdActorWalkByAndWait(ACTOR_KRADEN, -16, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
	syFieldScriptWait(20);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD, 33);
	// "And you know what!? I even want to get a closer look if I can!"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	// "So let's go! Let's find us some werewolves!"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
	scFieldActorJump(ACTOR_KRADEN, 6, 15);
	scFieldActorJump(ACTOR_KRADEN, 6, 23);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
	syFieldScriptWait(40);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTHEAST);
	syFieldScriptWait(20);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
		fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
	syFieldScriptWait(30);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	scWalkIntoLeader(ACTOR_KRADEN);
	fdActorSetSpeed(ACTOR_JENNA, 1.2);
	scWalkIntoLeader(ACTOR_JENNA);
	fdActorSetSpeed(ACTOR_SHEBA, 1.2);
	scWalkIntoLeader(ACTOR_SHEBA);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		fdActorSetSpeed(ACTOR_PIERS, 1.2);
		scWalkIntoLeader(ACTOR_PIERS);
	}
	syScriptCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdScriptEnd();
}

// Coming down into the werewolves' cave through the cleft by the sanctum, metatiles (17..18, 49): a
// howl echoes, the little werewolf from the path comes into sight, Kraden cries "Look! It's that
// werewolf!", and it looks around and runs off deeper into the cave; Kraden wants to follow it.
void scGarohFollowWerewolfIntoCave(void)
{
	fdScriptSetFlag(FLAG_GAROH_FOLLOWED_WEREWOLF);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syFieldScriptSetMessageId((Sint32)MsgGarohHaowooo);
	fdActorSetPos(ACTOR_WEREWOLF, 144, 752);
	fdActorWalkToAndWait(ACTOR_FELIX, 280, 776);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	syFieldScriptWait(10);
	// "Haowooo!!!"
	syFieldScriptShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, 16, 0, FACING_NORTH_NORTHWEST);
	scFieldActorWaitForMoveById(ACTOR_KRADEN);
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	syFieldScriptWait(20);
	scFieldActorStartRepeatedMotion(ACTOR_KRADEN, 3);
	fdActorSetAttachedEffect(ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scFieldActorStartRepeatedMotion(ACTOR_FELIX, 3);
	fdActorSetAttachedEffect(ACTOR_FELIX, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(45);
	// "Aow-aow-oooo!!!"
	syFieldScriptShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	syFieldScriptWait(20);
	fdCameraMoveTo(280, CAMERA_KEEP, 752, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(20);
	fdActorSetPos(ACTOR_WEREWOLF, 144, 720);
	fdActorSetSpeed(ACTOR_WEREWOLF, 0.8);
	fdActorWalkToAndWait(ACTOR_WEREWOLF, 168, 720);
	syFieldScriptWait(10);
	fdActorSetSpeed(ACTOR_WEREWOLF, 2);
	scFieldActorJump(ACTOR_WEREWOLF, 6, 0);
	fdActorWalkToAndWait(ACTOR_WEREWOLF, 200, 720);
	syFieldScriptWait(10);
	fdActorSetSpeed(ACTOR_WEREWOLF, 0.8);
	fdActorWalkToAndWait(ACTOR_WEREWOLF, 216, 712);
	fdActorWalkToAndWait(ACTOR_WEREWOLF, 320, 712);
	// "Look! It's that werewolf!"
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_WEREWOLF, 2);
	syFieldScriptWait(10);
	fdActorSetSpeed(ACTOR_WEREWOLF, 0.6);
	fdActorGet(ACTOR_WEREWOLF)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_WEREWOLF, 296, 712);
	syFieldScriptWait(1);
	fdActorGet(ACTOR_WEREWOLF)->steeringFlags |= ACTOR_FACES_MOTION;
	syFieldScriptWait(15);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_WEST_NORTHWEST, 0);
	syFieldScriptWait(25);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_EAST_NORTHEAST, 0);
	syFieldScriptWait(25);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_WEST_NORTHWEST, 0);
	syFieldScriptWait(25);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_SOUTH, 0);
	syFieldScriptWait(25);
	fdActorShowEmote(ACTOR_WEREWOLF, EMOTE_IN_FRONT | EMOTE_WINCE, 45);
	scFieldActorJump(ACTOR_WEREWOLF, 6, 15);
	scFieldActorJump(ACTOR_WEREWOLF, 6, 23);
	fdActorSetSpeed(ACTOR_WEREWOLF, 2.4);
	fdActorWalkToAndWait(ACTOR_WEREWOLF, 200, 712);
	syFieldScriptWait(3);
	fdActorSetSpeed(ACTOR_WEREWOLF, 2);
	scFieldActorJump(ACTOR_WEREWOLF, 6, 0);
	fdActorWalkToAndWait(ACTOR_WEREWOLF, 168, 712);
	syFieldScriptWait(3);
	fdActorSetSpeed(ACTOR_WEREWOLF, 2.4);
	fdActorWalkToAndWait(ACTOR_WEREWOLF, 144, 712);
	fdActorSetPos(ACTOR_WEREWOLF, 0, 0);
	syFieldScriptWait(30);
	syScriptCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(20);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	// "He was right there!"
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldScriptWait(10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	syFieldScriptWait(20);
	// "Let's go find him, Felix!"
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	syFieldScriptWait(20);
	scFieldActorJump(ACTOR_KRADEN, 6, 15);
	scFieldActorJump(ACTOR_KRADEN, 6, 23);
	fdActorAnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
	syFieldScriptWait(10);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	scWalkTowardLeader(ACTOR_KRADEN);
	scFieldActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdScriptEnd();
}

// The first night in the werewolves' cave, at metatiles (19..21, 15): Kraden wonders where the
// werewolf went, the werewolves surround the party, and their elder Maha speaks with them and tells
// them to come back later.
void scGarohWerewolvesRevealThemselves(void)
{
	fdScriptSetFlag(FLAG_GAROH_MET_WEREWOLVES);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syFieldScriptSetMessageId((Sint32)MsgGarohWait);
	fdActorSetPos(ACTOR_MAHA, 328, 40);
	fdActorSetPos(ACTOR_WEREWOLF, 328, 40);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_SOUTH, 0);
	fdActorWalkToAndWait(ACTOR_FELIX, 320, 224);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	syFieldScriptWait(10);
	// "Felix, wait..."
	syFieldScriptShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0, 0);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, -16, -8, FACING_NORTH);
	scFieldActorWaitForMoveById(ACTOR_KRADEN);
	syFieldScriptWait(30);
	fdCameraSetSpeed(1);
	fdCameraMoveTo(328, CAMERA_KEEP, 152, 1);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 0);
	syFieldScriptWait(25);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	syFieldScriptWait(25);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 0);
	syFieldScriptWait(25);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	syFieldScriptWait(25);
	// "Where could that werewolf have gone?"
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
	fdCameraMoveToActorById(ACTOR_FELIX, 1);
	fdCameraWaitForArrival();
	fdCameraSetSpeed(1.5);
	// "Do you think we lost him somewhere?" Yes: "That would be a shame..." No: "I hope we
	// didn't... Or rather, I'd be quite unhappy if we did..."
	fdScriptOpenMessage(ACTOR_KRADEN, 0);
	if (!syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0))
	{
		syFieldScriptWait(20);
		fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
		// "That would be a shame..."
		syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
		FieldStateWork->msg = (FieldStateWork->msg + 1);
	}
	else
	{
		syFieldScriptWait(30);
		fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
		FieldStateWork->msg = (FieldStateWork->msg + 1);
		// "I hope we didn't... Or rather, I'd be quite unhappy if we did..."
		syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	}
	// "Grrrr..."
	syFieldScriptShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_FELIX, 2);
	syFieldScriptWait(10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	syFieldScriptWait(20);
	scFieldActorLaunchFromLeader(ACTOR_JENNA, 16, 0, FACING_NORTH);
	scFieldActorLaunchFromLeader(ACTOR_SHEBA, 32, 0, FACING_NORTH);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		scFieldActorLaunchFromLeader(ACTOR_PIERS, 48, -8, FACING_NORTHWEST);
	}
	scFieldActorWaitForMoveById(ACTOR_SHEBA);
	syFieldScriptWait(20);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	// "What was that?"
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	syFieldScriptWait(20);
	// "It sounded like a wolf growling, didn't it?"
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 0);
	syFieldScriptWait(25);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	syFieldScriptWait(25);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 0);
	syFieldScriptWait(25);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	syFieldScriptWait(25);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
	syFieldScriptWait(25);
	// "My hearing is not so very good, but... there was something."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	syFieldScriptWait(40);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	syFieldScriptWait(25);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
	syFieldScriptWait(10);
	// "Perhaps we should search this area more thoroughly..."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	// "There is no need..."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	fdCameraMoveTo(328, CAMERA_KEEP, 144, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(20);
	fdActorSetPos(ACTOR_CAVE_PASSAGE, 328, 136);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_CAVE_PASSAGE), 0);
	scGarohOpenCavePassage(ACTOR_CAVE_PASSAGE);
	syFieldScriptWait(20);
	// "Look... He's using Psynergy."
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorSetSpeed(ACTOR_WEREWOLF, 0.7);
	fdActorSetSpeed(ACTOR_MAHA, 0.7);
	fdActorSetPos(ACTOR_MAHA, 328, 128);
	syFieldScriptWait(10);
	fdActorWalkByAndWait(ACTOR_MAHA, 0, 16);
	fdActorSetPos(ACTOR_WEREWOLF, 328, 128);
	syFieldScriptWait(10);
	fdActorWalkBy(ACTOR_WEREWOLF, 0, 16);
	fdActorWalkByAndWait(ACTOR_MAHA, 0, 16);
	syFieldScriptWait(10);
	scGarohCloseCavePassage(ACTOR_CAVE_PASSAGE);
	fdActorSetPos(ACTOR_CAVE_PASSAGE, 0, 0);
	syScriptCameraFollowActor(ACTOR_MAHA, 1);
	syFieldScriptWait(20);
	fdActorWalkBy(ACTOR_WEREWOLF, 0, 24);
	fdActorWalkByAndWait(ACTOR_MAHA, 0, 24);
	fdActorWalkByAndWait(ACTOR_WEREWOLF, 16, 8);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_SOUTH, 0);
	syFieldScriptWait(20);
	fdCameraMoveTo(328, CAMERA_KEEP, 208, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_WEREWOLF, 2);
	syFieldScriptWait(10);
	// "Garrrrooow..."
	syFieldScriptShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	scFieldActorStartRepeatedMotion(ACTOR_KRADEN, 2);
	scFieldActorStartRepeatedMotion(ACTOR_SHEBA, 2);
	scFieldActorStartRepeatedMotion(ACTOR_JENNA, 2);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		scFieldActorStartRepeatedMotion(ACTOR_PIERS, 2);
	}
	fdActorRunRepeatedMotion(ACTOR_FELIX, 2);
	syFieldScriptWait(20);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
	// "Fear not... This child is harmless."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	// "He does look kind of...small..."
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	scFieldActorStartRepeatedMotion(ACTOR_WEREWOLF, 3);
	fdActorSetAttachedEffect(ACTOR_WEREWOLF, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(40);
	// "Grrrrrrr..."
	syFieldScriptShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldScriptWait(10);
	// "Tell me, who are you? How did you come to be here?"
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	scFieldActorJump(ACTOR_WEREWOLF, 6, 15);
	scFieldActorJump(ACTOR_WEREWOLF, 6, 23);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_SOUTHWEST, 0);
	syFieldScriptWait(20);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
	// "I should like to ask you the same question."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	}
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldScriptWait(10);
	// "Could it be? Do you outsiders possess the power to reveal the unseen?" Yes: "Of course... I
	// was certain that you must." No: "Impossible... Without Reveal, you could not have entered
	// this cave."
	fdScriptOpenMessage(ACTOR_MAHA, 0);
	if (syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0))
	{
		syFieldScriptWait(40);
		fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
		// "Impossible... Without Reveal, you could not have entered this cave."
		syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
		FieldStateWork->msg = (FieldStateWork->msg + 1);
	}
	else
	{
		syFieldScriptWait(30);
		fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
		syFieldScriptWait(10);
		FieldStateWork->msg = (FieldStateWork->msg + 1);
		// "Of course... I was certain that you must."
		syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	}
	fdActorRunRepeatedMotion(ACTOR_WEREWOLF, 2);
	syFieldScriptWait(10);
	// "Grawooo..."
	syFieldScriptShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_EAST, 0);
	syFieldScriptWait(30);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	// "Yes, you are correct. They must have passed the challenge of Air's Rock."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	// "Er... How come that kid doesn't talk, like you?"
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldScriptWait(10);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH, 0);
	syFieldScriptWait(20);
	// "When we become werewolves, speaking the human tongue becomes more difficult."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	syFieldScriptWait(20);
	// "But you can... You must be someone special, then..."
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
	// "Hurh hurr hrrr... 'Special'? Perhaps. Why don't you just call me Maha..."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	// "So... I assume you also can use Whirlwind..."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	}
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	// "If you learned Reveal on Air's Rock, you must also know Whirlwind." Yes: "That's right... I
	// think we can tell Maha that we're Adepts." No: "I don't think we need to hide the fact that
	// we're Adepts from Maha..."
	fdMessageShowAtActor(ACTOR_MAHA, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	}
	syFieldScriptWait(20);
	if (syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0))
	{
		syFieldScriptWait(40);
		fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
		syFieldScriptWait(20);
		// "I don't think we need to hide the fact that we're Adepts from Maha..."
		syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 0);
		FieldStateWork->msg = (FieldStateWork->msg + 1);
	}
	else
	{
		syFieldScriptWait(30);
		fdActorAnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
		syFieldScriptWait(20);
		FieldStateWork->msg = (FieldStateWork->msg + 1);
		// "That's right... I think we can tell Maha that we're Adepts."
		syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 0);
	}
	fdActorFaceDirection(ACTOR_MAHA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_SOUTHWEST, 0);
	syFieldScriptWait(30);
	// "Awooo..."
	syFieldScriptShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
	}
	syFieldScriptWait(30);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	syFieldScriptWait(10);
	// "What did he say? I don't understand..."
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH, 0);
	syFieldScriptWait(30);
	// "He wanted to know what an Adept is... A question I would like answered as well."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	// "Adepts tap the power of elemental energies to generate forces like Whirlwind..."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	// "Elementals... You mean the four elements, the powers of earth and fire, water and wind?"
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	scFieldActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		scFieldActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	}
	scFieldActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	syFieldScriptWait(20);
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
	syFieldScriptWait(20);
	// "That is correct. These elements together comprise everything in our world..."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	syFieldScriptWait(30);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	// "Interesting. Very interesting..."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_WEREWOLF, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	// "Awooo..."
	syFieldScriptShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_NORTHEAST, 0);
	syFieldScriptWait(30);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	syFieldScriptWait(10);
	// "Um... What did he say that time?"
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHEAST, 0);
	syFieldScriptWait(20);
	// "He asked if there were no longer any need to hide from you..."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	// "...Why did he have to hide in here in the first place?"
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_SOUTH, 0);
	syFieldScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_WEREWOLF, 2);
	syFieldScriptWait(10);
	// "Aaah-Ah-Awoooh! Hawawoooh!"
	syFieldScriptShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	syFieldScriptWait(20);
	// "No doubt, most would find the sight of a werewolf rather unnerving..."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	syFieldScriptWait(30);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	// "You are, sadly, correct. Many consider ours an accursed race."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	// "And if they found you in your werewolf form?"
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldScriptWait(10);
	// "...Once, our people were burned at the stake as abominations... They called this
	// 'purification.'"
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD, 33);
	// "Horrible..."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_SOUTH, 0);
	syFieldScriptWait(20);
	fdActorWalkByAndWait(ACTOR_WEREWOLF, 0, 16);
	syFieldScriptWait(10);
	scFieldActorJump(ACTOR_WEREWOLF, 6, 15);
	scFieldActorJump(ACTOR_WEREWOLF, 6, 23);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	// "But...this little one... He won't get excited and try to bite, will he?"
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorSetSpeed(ACTOR_MAHA, 2.5);
	fdActorSetSpeed(ACTOR_WEREWOLF, 2.5);
	fdActorFaceDirection(ACTOR_MAHA, FACING_EAST, 0);
	syFieldScriptWait(20);
	fdActorWalkByAndWait(ACTOR_MAHA, 8, 0);
	scFieldActorJump(ACTOR_WEREWOLF, 4, 0);
	fdActorGet(ACTOR_MAHA)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorWalkBy(ACTOR_MAHA, -8, 0);
	fdActorGet(ACTOR_WEREWOLF)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_WEREWOLF, 16, 0);
	syFieldScriptWait(1);
	fdActorGet(ACTOR_WEREWOLF)->steeringFlags |= ACTOR_FACES_MOTION;
	scFieldActorSetAnimation(ACTOR_WEREWOLF, ANIM_WEREWOLF_SIT);
	syFieldScriptWait(20);
	fdActorSetSpeed(ACTOR_WEREWOLF, 1.3);
	fdActorSetSpeed(ACTOR_MAHA, 0.7);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_WEREWOLF, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
	fdActorSetAnimation(ACTOR_WEREWOLF, ANIM_STAND);
	scFieldActorJump(ACTOR_WEREWOLF, 6, 0);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_WEST, 0);
	syFieldScriptWait(30);
	fdActorSetAttachedEffect(ACTOR_WEREWOLF, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorAnimateThenWait(ACTOR_WEREWOLF, ANIM_NOD, 20);
	syFieldScriptWait(15);
	fdActorSetAttachedEffect(ACTOR_WEREWOLF, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorAnimateThenWait(ACTOR_WEREWOLF, ANIM_NOD, 20);
	syFieldScriptWait(20);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldScriptWait(10);
	// "Child, return to your home."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_WEREWOLF, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	// "Arrooo?"
	syFieldScriptShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
	// "Don't worry. We can trust them... Go back home now."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_SOUTH_SOUTHWEST, 0);
	syFieldScriptWait(45);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_WEST, 0);
	syFieldScriptWait(20);
	// "Aarrrooo?"
	syFieldScriptShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
	// "They are Adepts, not unlike us werewolves... They guard an ancient secret."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	syFieldScriptWait(45);
	fdActorFaceDirection(ACTOR_MAHA, FACING_EAST, 0);
	syFieldScriptWait(20);
	// "We can trust them. They will not betray us to the world."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_WEREWOLF, ANIM_NOD, 20);
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_WEREWOLF, FACING_SOUTH, 0);
	syFieldScriptWait(35);
	fdActorAnimateThenWait(ACTOR_WEREWOLF, ANIM_NOD, 20);
	syFieldScriptWait(20);
	// "Woof."
	syFieldScriptShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldScriptWait(20);
	scFieldActorWatchAndSetScript(ACTOR_FELIX, ACTOR_WEREWOLF);
	scFieldActorWatchAndSetScript(ACTOR_KRADEN, ACTOR_WEREWOLF);
	scFieldActorWatchAndSetScript(ACTOR_JENNA, ACTOR_WEREWOLF);
	scFieldActorWatchAndSetScript(ACTOR_SHEBA, ACTOR_WEREWOLF);
	scFieldActorWatchAndSetScript(ACTOR_MAHA, ACTOR_WEREWOLF);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		scFieldActorWatchAndSetScript(ACTOR_PIERS, ACTOR_WEREWOLF);
	}
	fdActorWalkToAndWait(ACTOR_WEREWOLF, 376, 208);
	fdActorWalkByAndWait(ACTOR_WEREWOLF, 0, 16);
	fdActorWalkByAndWait(ACTOR_WEREWOLF, -16, 24);
	fdActorWalkByAndWait(ACTOR_WEREWOLF, -32, 16);
	fdActorWalkByAndWait(ACTOR_WEREWOLF, 0, 48);
	syFieldScriptWait(20);
	scFieldActorStop(ACTOR_FELIX);
	scFieldActorStop(ACTOR_KRADEN);
	scFieldActorStop(ACTOR_JENNA);
	scFieldActorStop(ACTOR_SHEBA);
	scFieldActorStop(ACTOR_MAHA);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		scFieldActorStop(ACTOR_PIERS);
	}
	fdActorSetPos(ACTOR_WEREWOLF, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldScriptWait(10);
	// "It has grown quite late."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
	}
	syFieldScriptWait(30);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	// "Why don't you go to the inn and rest..."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	// "But then..."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	// "I will not leave this place."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	// "You aren't going home?"
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	// "When I learned Reveal, I lost my human form. The wild is my home now."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	// "So, Master Maha, you always look this way?"
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	syFieldScriptWait(10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	syFieldScriptWait(30);
	// "And the little one?"
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
	// "The child changed because he looked directly at the full moon."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 0);
	syFieldScriptWait(30);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
	syFieldScriptWait(20);
	// "So, by tomorrow morning, he will have returned to his human form again?"
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldScriptWait(10);
	// "Correct. Now, let us retire."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	syFieldScriptWait(20);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	}
	syFieldScriptWait(20);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	scFieldActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		scFieldActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	}
	scFieldActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	syFieldScriptWait(20);
	syFieldScriptWait(20);
	fdActorAnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
	syFieldScriptWait(10);
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
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		fdActorSetSpeed(ACTOR_PIERS, 1.2);
		scFieldActorSetAnimation(ACTOR_PIERS, ANIM_WALK);
		scWalkTowardLeader(ACTOR_PIERS);
		scFieldActorWaitForMoveById(ACTOR_PIERS);
		fdActorSetPos(ACTOR_PIERS, 0, 0);
	}
	syScriptCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	scGarohMetWerewolves();
	syFieldScriptSetExitRequest(GAROH_WEREWOLF_CAVE_TO_GAROH_ROOMS_HOUSES);
	fdScriptEnd();
}

// By day in the werewolves' cave, at metatiles (19..21, 15): "Ah, you came... I've been waiting for
// you." Maha tells the werewolves' legend and of Mt. Aleph, and gives the party the Jupiter Djinni
// Ether.
void scGarohMahaTellsTheLegend(void)
{
	Sint32 ticks;
	FIXED  total;
	ACTOR *ether;
	SPR   *sprite;

	fdScriptSetFlag(FLAG_GAROH_HEARD_MAHA);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syFieldScriptSetMessageId((Sint32)MsgGarohAhYouCame);
	fdActorSetPos(ACTOR_MAHA, 328, 40);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH, 0);
	// "Ah, you came..."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorSetSpeed(ACTOR_FELIX, 0.8);
	fdActorWalkToAndWait(ACTOR_FELIX, 320, 224);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, -16, 0, FACING_NORTH);
	scFieldActorLaunchFromLeader(ACTOR_JENNA, 16, 0, FACING_NORTH);
	scFieldActorLaunchFromLeader(ACTOR_SHEBA, 32, 0, FACING_NORTH);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		scFieldActorLaunchFromLeader(ACTOR_PIERS, 48, 0, FACING_NORTH);
	}
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	scFieldActorWaitForMoveById(ACTOR_SHEBA);
	syFieldScriptWait(20);
	fdCameraMoveTo(328, CAMERA_KEEP, 144, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(20);
	fdActorSetPos(ACTOR_CAVE_PASSAGE, 328, 136);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_CAVE_PASSAGE), 0);
	scGarohOpenCavePassage(ACTOR_CAVE_PASSAGE);
	syFieldScriptWait(30);
	fdActorSetSpeed(ACTOR_MAHA, 0.7);
	fdActorSetPos(ACTOR_MAHA, 328, 128);
	syFieldScriptWait(10);
	fdActorWalkByAndWait(ACTOR_MAHA, 0, 16);
	syFieldScriptWait(20);
	scGarohCloseCavePassage(ACTOR_CAVE_PASSAGE);
	fdActorSetPos(ACTOR_CAVE_PASSAGE, 0, 0);
	syScriptCameraFollowActor(ACTOR_MAHA, 1);
	syFieldScriptWait(20);
	fdActorWalkByAndWait(ACTOR_MAHA, 0, 48);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
	}
	fdCameraMoveTo(328, CAMERA_KEEP, 184, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(10);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldScriptWait(10);
	// "I've been waiting for you."
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	}
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	syFieldScriptWait(20);
	syFieldScriptWait(20);
	fdActorWalkByAndWait(ACTOR_KRADEN, 0, -16);
	syFieldScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldScriptWait(10);
	// "Would you mind telling me a little more about werewolves?"
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorWalkByAndWait(ACTOR_MAHA, -8, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	syFieldScriptWait(20);
	// "You want to know the reason we possess the power to transform into beasts?"
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	// "Ah... Well, I, er... Actually, yes."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	// "Do you find it strange that we can change our shapes?"
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	// "Well, ummm... Yes."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldScriptWait(10);
	// "But surely changing one's shape is not really all that unusual."
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	// "What are you getting at?"
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorWalkByAndWait(ACTOR_MAHA, 16, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	syFieldScriptWait(20);
	// "Think about the caterpillar for a moment. When the time is right, does it not transform into
	// a beautiful butterfly?"
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	// "Well, yes. It's only natural..."
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTHEAST, 0);
	syFieldScriptWait(20);
	// "And the pill bug and hedgehog? Do they not change their shapes to protect themselves?"
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	syFieldScriptWait(10);
	// "I have heard of these creatures, yes..."
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorWalkByAndWait(ACTOR_MAHA, -8, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	syFieldScriptWait(20);
	// "And many amphibians possess the power to change color at will, correct?"
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
	syFieldScriptWait(35);
	fdActorWalkByAndWait(ACTOR_MAHA, -8, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	syFieldScriptWait(25);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldScriptWait(10);
	// "In light of all this, do you mean to tell me you find werewolves so abnormal?"
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	// "Hrmm, well... To be honest, I hadn't thought of it that way before..."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
	// "Perhaps you simply did not give it enough thought."
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 45);
	scFieldActorJump(ACTOR_KRADEN, 6, 15);
	scFieldActorJump(ACTOR_KRADEN, 6, 23);
	// "How insulting! I, sir... am a SCIENTIST!"
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	// "Indeed! A scientist? That is impressive."
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldScriptWait(10);
	fdActorFaceDirection(ACTOR_MAHA, FACING_NORTH, 0);
	syFieldScriptWait(30);
	// "I have spent a great deal of time thinking about werewolves."
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
	fdActorWalkByAndWait(ACTOR_MAHA, 4, -12);
	syFieldScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldScriptWait(10);
	// "For a time, I even felt cursed to have been born a werewolf..."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorWalkByAndWait(ACTOR_MAHA, 4, -12);
	syFieldScriptWait(10);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
	// "But after taking a more objective look at nature..."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH, 0);
	syFieldScriptWait(30);
	// "I realized that weaker species have survived by evolving...by transforming. Don't you
	// agree?"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldScriptWait(20);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldScriptWait(10);
	// "Think about it for a moment. Without changing their bodies with weapons and armor... Humans
	// would be very weak creatures indeed."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldScriptWait(10);
	// "I believe that evolving into werewolves is just one way for humanity to survive..."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldScriptWait(10);
	// "But that's..."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
	// "Of course, there's no way I can prove this theory."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldScriptWait(10);
	// "But I felt that our ability to use Whirlwind was evidence of this."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	// "Whirlwind? Why?"
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHEAST, 0);
	syFieldScriptWait(20);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldScriptWait(10);
	// "I had thought only the people of Garoh could use Whirlwind."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	// "It was a gift known only to werewolves, to Garoh's pack..."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldScriptWait(10);
	// "And that only a chosen few could use Reveal..."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	syFieldScriptWait(20);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
	// "But now I find that you, too, can use these... What does it all mean?"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	}
	syFieldScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldScriptWait(10);
	// "Why are you able to use Whirlwind, and what is an Adept?"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
	}
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	syFieldScriptWait(25);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldScriptWait(10);
	// "Master Maha, do you know of a continent called Angara? It lies far north of the Eastern
	// Sea."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	// "Angara?"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldScriptWait(10);
	// "In the middle of Angara, there is a peak called Mt. Aleph."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldScriptWait(10);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldScriptWait(10);
	// "Mt. Aleph? ...Then it's true! Our legends say this peak has stood since this world began!"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	// "If Mt. Aleph does exist, then perhaps our legends are true!"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldScriptWait(10);
	// "It does exist..."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
	syFieldScriptWait(30);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldScriptWait(10);
	// "These two hail from the village at the base of Mt. Aleph, a village called Vale."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldScriptWait(15);
	fdActorFaceDirection(ACTOR_MAHA, FACING_NORTH, 0);
	syFieldScriptWait(30);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldScriptWait(10);
	// "They come from Mt. Aleph!? Incredible!"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
	syFieldScriptWait(30);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	syFieldScriptWait(25);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldScriptWait(10);
	// "And they are both Adepts. And while their powers may vary... Everyone from Vale is an Adept
	// to a certain degree."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	scFieldActorStartRepeatedMotion(ACTOR_MAHA, 3);
	fdActorSetAttachedEffect(ACTOR_MAHA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(40);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	syFieldScriptWait(30);
	// "Then, does that not make them like us in Garoh?"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldScriptWait(10);
	// "In Vale, the myths say that Mt. Aleph itself gave birth to the Adepts..."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	// "This is not what the legends of Garoh tell us..."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	syFieldScriptWait(20);
	// "As a people, they are quite gifted...but it goes far beyond that..."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorWalkByAndWait(ACTOR_KRADEN, 16, 0);
	syFieldScriptWait(20);
	fdActorWalkByAndWait(ACTOR_KRADEN, 16, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
	syFieldScriptWait(20);
	fdActorWalkByAndWait(ACTOR_KRADEN, 16, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	}
	syFieldScriptWait(20);
	fdActorWalkByAndWait(ACTOR_KRADEN, 16, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
	syFieldScriptWait(20);
	fdCameraMoveTo(344, CAMERA_KEEP, 208, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(20);
	// "The mystical stones on Mt. Aleph... The Psynergy Stones... They are closely linked to the
	// powers of the townsfolk."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorSetSpeed(ACTOR_MAHA, 1.2);
	fdActorWalkByAndWait(ACTOR_MAHA, 16, 16);
	fdActorSetSpeed(ACTOR_MAHA, 0.7);
	// "'Psynergy Stones,' you say?"
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldScriptWait(10);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
	}
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 0);
	syFieldScriptWait(30);
	// "If there were similar stones in Garoh, it might explain your powers as well."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldScriptWait(10);
	// "And what manner of stones are these Psynergy Stones?"
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	syFieldScriptWait(10);
	// "You've seen them, Maha! The shining stone at the heart of Air's Rock."
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldScriptWait(10);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	syFieldScriptWait(20);
	// "I know the stone you speak of, but..."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD, 33);
	// "Air's Rock is quite far from Garoh."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTHEAST, 0);
	syFieldScriptWait(20);
	// "I don't see how it could affect the people here."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldScriptWait(10);
	syFieldScriptWait(20);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	syFieldScriptWait(20);
	// "I wonder... Could it be the wind?" Yes: "You probably figured it out for yourself already,
	// Felix." No: "I guess I couldn't have expected you to understand, Felix..."
	fdScriptOpenMessage(ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	}
	if (!syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0))
	{
		syFieldScriptWait(25);
		// "You probably figured it out for yourself already, Felix."
		syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 0);
		FieldStateWork->msg = (FieldStateWork->msg + 1);
	}
	else
	{
		syFieldScriptWait(40);
		FieldStateWork->msg = (FieldStateWork->msg + 1);
		// "I guess I couldn't have expected you to understand, Felix..."
		syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	}
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	}
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
	syFieldScriptWait(30);
	// "There were strong winds blowing within Air's Rock... Those winds would carry far..."
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	// "Carry? Carry what?"
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	// "Ugh! You're so dense! The power of the Psynergy Stone!"
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
	// "How is that possible? How could the stone's power be carried on the wind?"
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	syFieldScriptWait(20);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	// "Do not belittle the power of the wind!"
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorAnimateThenWait(ACTOR_SHEBA, ANIM_SHAKE_HEAD, 33);
	// "Given time, the wind can grind great mountains down to sand!"
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	syFieldScriptWait(10);
	// "I mean, the wind at Air's Rock was strong enough to blow us off the ground!"
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MAHA, ANIM_NOD);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	}
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	syFieldScriptWait(20);
	syFieldScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	syFieldScriptWait(10);
	// "So why couldn't the wind carry particles of the stone all the way to Garoh?"
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	scFieldActorStartRepeatedMotion(ACTOR_MAHA, 3);
	fdActorSetAttachedEffect(ACTOR_MAHA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scFieldActorStartRepeatedMotion(ACTOR_KRADEN, 3);
	fdActorSetAttachedEffect(ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(40);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTHEAST, 0);
	syFieldScriptWait(25);
	scFieldActorJump(ACTOR_KRADEN, 6, 15);
	scFieldActorJump(ACTOR_KRADEN, 6, 23);
	syFieldScriptWait(10);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldScriptWait(10);
	// "She's right!", said by both at once.
	syFieldScriptShowTwoMessagesAndWait(ACTOR_MAHA, 9, 2, 9, 5, ACTOR_KRADEN, 17, 14, 20, 9, 0);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
	}
	scFieldActorStartRepeatedMotion(ACTOR_MAHA, 3);
	fdActorSetAttachedEffect(ACTOR_MAHA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(40);
	// "Those particles from the Psynergy Stone must be charged with its power!"
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	scFieldActorStartRepeatedMotion(ACTOR_KRADEN, 3);
	fdActorSetAttachedEffect(ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(40);
	// "This explains why your Psynergy is weaker than that of Vale's people... You've been exposed
	// to smaller particles."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	// "The wind patterns might also affect who among us in Garoh can use Whirlwind."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldScriptWait(10);
	// "Of course, this is all mere conjecture, but it does offer us some clues..."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldScriptWait(10);
	// "Thank you, umm..."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_SHEBA, ANIM_NOD, 20);
	syFieldScriptWait(10);
	// "It's Sheba. I'm pleased to meet you."
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldScriptWait(10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHWEST, 0);
	syFieldScriptWait(20);
	// "Oh! Haven't we given you our names, Master Maha?"
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 0);
	syFieldScriptWait(20);
	// "I am Kraden."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldScriptWait(10);
	fdActorWalkByAndWait(ACTOR_KRADEN, -18, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	syFieldScriptWait(20);
	// "This is Jenna..."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	syFieldScriptWait(20);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldScriptWait(20);
	fdActorAnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
	syFieldScriptWait(10);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
		// "Piers..."
		syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
		fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
		syFieldScriptWait(20);
		fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
		syFieldScriptWait(20);
		fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
		syFieldScriptWait(20);
		syFieldScriptWait(10);
	}
	else
	{
		FieldStateWork->msg = (FieldStateWork->msg + 1);
	}
	fdActorWalkByAndWait(ACTOR_KRADEN, -42, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	syFieldScriptWait(20);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldScriptWait(10);
	// "And Felix."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	syFieldScriptWait(20);
	fdActorWalkByAndWait(ACTOR_MAHA, -16, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	syFieldScriptWait(20);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldScriptWait(30);
	fdActorAnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
	syFieldScriptWait(25);
	// "You must permit me to thank you for all you've done."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD, 33);
	// "No, no, please... We don't need any thanks..."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_NORTH, 0);
	syFieldScriptWait(40);
	scGarohPopQuestionMarks(ACTOR_MAHA);
	scGarohPopQuestionMarks(ACTOR_MAHA);
	syFieldScriptWait(20);
	fdActorWalkByAndWait(ACTOR_MAHA, 16, 0);
	syFieldScriptWait(20);
	scFieldActorStartRepeatedMotion(ACTOR_MAHA, 3);
	fdActorSetAttachedEffect(ACTOR_MAHA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(40);
	fdActorFaceDirection(ACTOR_MAHA, FACING_WEST, 0);
	syFieldScriptWait(20);
	fdActorWalkByAndWait(ACTOR_MAHA, -16, 0);
	syFieldScriptWait(10);
	fdActorWalkByAndWait(ACTOR_MAHA, -16, 0);
	syFieldScriptWait(20);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
	fdActorFaceDirection(ACTOR_MAHA, FACING_EAST, 0);
	syFieldScriptWait(20);
	fdActorWalkByAndWait(ACTOR_MAHA, 16, 0);
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_MAHA, FACING_NORTH, 0);
	syFieldScriptWait(20);
	scFieldActorStartRepeatedMotion(ACTOR_MAHA, 3);
	fdActorSetAttachedEffect(ACTOR_MAHA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(40);
	fdActorWalkByAndWait(ACTOR_MAHA, 0, -12);
	syFieldScriptWait(20);
	fdActorWalkByAndWait(ACTOR_MAHA, 0, -12);
	syFieldScriptWait(20);
	syFieldScriptWait(20);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH, 0);
	syFieldScriptWait(20);
	// "That's too bad... I was going to give you this."
	syFieldScriptShowMessageThenWait(ACTOR_MAHA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldScriptWait(10);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldScriptWait(5);
	ether         = fdActorGet(ACTOR_ETHER);
	sprite        = ether->sprite;
	sprite->scale = toFIXED(2) / 5;
	scFieldActorSetSpritePriority(ACTOR_ETHER, 1);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_ETHER), 0);
	fdActorSetPos(ACTOR_ETHER, 328, 160);
	scFieldActorJump(ACTOR_ETHER, 9, 0);
	total = 0;
	for (ticks = 0; ticks <= ETHER_GROW_STEPS; ticks++)
	{
		sprite->scale  = syMathDivide(total, ETHER_GROW_STEPS) + toFIXED(2) / 5;
		ether->facing += FACING_STEP;
		syWait(1);
		total += toFIXED(3) / 5;
	}
	ether->facing = FACING_SOUTH;
	fdActorWalkByAndWait(ACTOR_ETHER, 0, 8);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_ETHER), 1);
	syFieldScriptWait(5);
	fdActorWalkByAndWait(ACTOR_ETHER, 0, 8);
	syFieldScriptWait(5);
	fdActorWalkByAndWait(ACTOR_ETHER, 0, 8);
	syFieldScriptWait(5);
	fdActorWalkByAndWait(ACTOR_ETHER, 0, 8);
	syFieldScriptWait(5);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	// "It's a Djinni!"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 0);
	fdActorShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHEAST, 0);
	syFieldScriptWait(20);
	// "A 'Djinni'? Is that what you call them?"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorWalkByAndWait(ACTOR_MAHA, 0, 24);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHEAST, 0);
	syFieldScriptWait(10);
	// "When he is with you, he adds to your power. He is very helpful."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	syFieldScriptWait(20);
	// "I suppose you'll be continuing your journey now?" Yes: "So you intend to travel by sea? That
	// is a long journey..." No: "So, you still can't tell just what sort of journeys await you..."
	fdScriptOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	}
	syFieldScriptWait(20);
	if (!syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0))
	{
		syFieldScriptWait(30);
		// "So you intend to travel by sea? That is a long journey..."
		syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
		FieldStateWork->msg = (FieldStateWork->msg + 1);
	}
	else
	{
		syFieldScriptWait(40);
		FieldStateWork->msg = (FieldStateWork->msg + 1);
		// "So, you still can't tell just what sort of journeys await you..."
		syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	}
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
	}
	syFieldScriptWait(30);
	fdActorRunRepeatedMotion(ACTOR_MAHA, 2);
	syFieldScriptWait(10);
	// "In that case, I think you will get more use out of this 'Djinni' than I will."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorSetSpeed(ACTOR_MAHA, 2.4);
	fdActorGet(ACTOR_MAHA)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	fdActorWalkByAndWait(ACTOR_MAHA, 0, 8);
	fdActorWalkByAndWait(ACTOR_MAHA, 0, -8);
	syFieldScriptWait(1);
	fdActorGet(ACTOR_MAHA)->steeringFlags |= ACTOR_FACES_MOTION;
	syFieldScriptWait(1);
	fdActorSetSpeed(ACTOR_MAHA, 0.7);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	}
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	fdDjinnJoinParty(ACTOR_ETHER, DJINNI_ETHER / DJINN_PER_ELEMENT, DJINNI_INDEX(DJINNI_ETHER));
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_STAND);
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH, 0);
	syFieldScriptWait(20);
	FieldStateWork->msg = (FieldStateWork->msg + 1);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	// "Another Djinni! Talk about lucky!"
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	syFieldScriptWait(30);
	fdActorAnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	syFieldScriptWait(20);
	// "This is a great gift. We are quite thankful."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	syFieldScriptWait(20);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
	// "You don't need to thank me. Just take care on your travels."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldScriptWait(20);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	// "Good luck with the... werewolf thing, Maha..."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHEAST, 0);
	syFieldScriptWait(20);
	fdActorAnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
	syFieldScriptWait(20);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	}
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	syFieldScriptWait(20);
	syFieldScriptWait(20);
	fdActorWalkByAndWait(ACTOR_KRADEN, 12, 0);
	fdActorFaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
	syFieldScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldScriptWait(10);
	// "Well, Felix, we must be going now."
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorAnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
	syFieldScriptWait(20);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	}
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
	syFieldScriptWait(20);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	scWalkIntoLeader(ACTOR_KRADEN);
	fdActorSetSpeed(ACTOR_JENNA, 1.2);
	scWalkIntoLeader(ACTOR_JENNA);
	fdActorSetSpeed(ACTOR_SHEBA, 1.2);
	scWalkIntoLeader(ACTOR_SHEBA);
	if (fdScriptGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
	{
		fdActorSetSpeed(ACTOR_PIERS, 1.2);
		scWalkIntoLeader(ACTOR_PIERS);
	}
	syScriptCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdScriptEnd();
}

void          scGarohLittleWerewolfThanks(void);
void          scGarohLiftPillar(void);
void          scGarohLiftPushedPillar(void);
void          scGarohRevealCaveEntrance(void);
void          scGarohRevealCavePassage(void);

extern Uint32 FieldEffectMediumScript[];

// clang-format off
#include "shared/field_effect_duration_scripts.c"

// The four pillars Move pushes, ended by -1.
Sint16 GarohPillars[] = {
	ACTOR_FIRST_PILLAR, ACTOR_SECOND_PILLAR, ACTOR_THIRD_PILLAR,
	ACTOR_FOURTH_PILLAR, -1, 0,
};

Uint32 GarohWerewolfLookAroundScript[] = {
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

// The path's colours by night, BG palette 6's colours 1 to 9.
Uint16 GarohPathNightColors[] = {
	31, 31, 31, 29, 29, 30, 27, 28,
	29, 25, 27, 28, 23, 25, 27, 21,
	24, 26, 15, 18, 19, 10, 12, 13,
	5, 6, 6,
};

// The path's colours by day, BG palette 1's colours 8 to 15.
Uint16 GarohPathDayColors[] = {
	24, 28, 30, 19, 24, 27, 15, 20,
	25, 11, 16, 23, 7, 12, 21, 3,
	8, 19, 27, 27, 27, 16, 24, 29,
	0,
};

const Sint32 *FieldEffectDurationScripts[] = {
	FieldEffectShortScript,
	FieldEffectMediumScript,
	FieldEffectLongScript,
};

Sint32 GarohShrunkSpriteScript[] = {
	SC_SET(SW_MOTION, 0),
	SC_ADD(SW_Y, toFIXED(-4)),
	SC_ADD(SW_X, toFIXED(6)),
	SC_SET(SW_SCALE_X, toFIXED(0.8)),
	SC_SET(SW_SCALE_Y, toFIXED(0.8)),
	SC_CALL(scGarohClearScriptedActorSpriteFlags),
	SC_END,
};

#include "shared/scene_entrances_default.c"

Uint32 SceneExits[] = {
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

ACTOR_PLACE GarohActorPlacements[] = {
	{SCENE_TABLE_END},
};

ACTOR_PLACE GarohVillageActorPlacements[] = {
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

ACTOR_PLACE GarohVillageDayActorPlacements[] = {
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

ACTOR_PLACE GarohWayUpActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 472, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 456, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 392, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 392, FACING_NORTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE GarohTunnelsActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (Sint32)GarohShrunkSpriteScript, 128, 0, 168, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (Sint32)GarohShrunkSpriteScript, 208, 0, 56, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (Sint32)GarohShrunkSpriteScript, 152, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (Sint32)GarohShrunkSpriteScript, 248, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (Sint32)GarohShrunkSpriteScript, 352, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (Sint32)GarohShrunkSpriteScript, 216, 0, 424, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (Sint32)GarohShrunkSpriteScript, 96, 0, 488, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (Sint32)GarohShrunkSpriteScript, 328, 0, 440, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (Sint32)GarohShrunkSpriteScript, 456, 0, 352, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (Sint32)GarohShrunkSpriteScript, 136, 0, 664, FACING_NORTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE GarohPathActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_WEREWOLF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 120, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 120, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{SCENE_TABLE_END},
};

ACTOR_PLACE GarohWerewolfCaveActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_WEREWOLF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MAHA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_ETHER), MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (Sint32)GarohShrunkSpriteScript, 64, 0, 562, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (Sint32)GarohShrunkSpriteScript, 370, 0, 56, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (Sint32)GarohShrunkSpriteScript, 256, 0, 80, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (Sint32)GarohShrunkSpriteScript, 184, 0, 352, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (Sint32)GarohShrunkSpriteScript, 392, 0, 368, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (Sint32)GarohShrunkSpriteScript, 280, 0, 626, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (Sint32)GarohShrunkSpriteScript, 336, 0, 784, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{SCENE_TABLE_END},
};

// The village's doors opening, as MAPGRID_ANIMATION_STEP rows: two steps of six ticks each: a door, the shop's wide door, the sanctum's tall door.
Sint16 GarohDoorFrames[] = {
	82, 32, 1, 2, 6, 80, 32, 1,
	2, 6, -1,
};
Sint16 GarohWideDoorFrames[] = {
	82, 35, 1, 2, 6, 80, 35, 1,
	2, 6, -1,
};
Sint16 GarohTallDoorFrames[] = {
	78, 32, 2, 2, 6, 78, 35, 2,
	2, 6, -1, 0,
};

// The doors Felix walks into, a row for each exit from the first: the opening and the metatile it plays at.
MAP_GRID_METATILE_ANIMATION GarohDoors[] = {
	{GarohDoorFrames, 78, 21},
	{GarohDoorFrames, 0, 0},
	{GarohDoorFrames, 72, 10},
	{GarohWideDoorFrames, 74, 22},
	{GarohDoorFrames, 85, 23},
	{GarohTallDoorFrames, 78, 13},
};

CUE GarohCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_TABLE_END},
};
// clang-format on

CUE           GarohVillageCues[] = {
    {STEP_CUE_WALKING_INTO(FACING_NORTH),
     GAROH_TO_GAROH_ROOMS_HOUSES_SOUTH, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohOpenDoorAndEnter)},
    {CUE_EXIT | CUE_FLAG_FACING | FACING_NORTH, 2, CONDITION_ALWAYS, 2},
    {STEP_CUE_WALKING_INTO(FACING_NORTH),
     GAROH_TO_GAROH_ROOMS_HOUSES_NORTHWEST, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohOpenDoorAndEnter)},
    {STEP_CUE_WALKING_INTO(FACING_NORTH),
     GAROH_TO_GAROH_ROOMS_HOUSES_SOUTHWEST, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohOpenDoorAndEnter)},
    {STEP_CUE_WALKING_INTO(FACING_NORTH),
     GAROH_TO_GAROH_ROOMS_HOUSES_SOUTHEAST, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohOpenDoorAndEnter)},
    {STEP_CUE_WALKING_INTO(FACING_NORTH),
     GAROH_TO_GAROH_ROOMS_SANCTUM, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohOpenDoorAndEnter)},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {CUE_EXIT | CUE_FLAG_FACING | FACING_NORTH, 8, CONDITION_ALWAYS, 8},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, 9},
    {CUE_EXIT, 10, CONDITION_ALWAYS, 10},
    {CUE_EXIT, 11, CONDITION_ALWAYS, 11},
    {STEP_CUE_FACING(FACING_NORTH),
     GAROH_TO_GAROH_WEREWOLF_CAVE, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohEnterOpening)},
    {CUE_STEP,
     METATILE_BEHAVIOR_BY_SANCTUM_DOOR, FLAG_GAROH_MET_WEREWOLVES,
     SCRIPT_REF(scGarohWerewolfPeersFromCave)},
    {CUE_TALK,
     ACTOR_VILLAGER_ASKING_ABOUT_THE_WEREWOLF, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohAskIfSawWerewolf)},
    {CUE_TALK, ACTOR_HOODED_VILLAGER, CONDITION_ALWAYS, MSGNO(MsgGarohGoodEveningForgiveMeFor)},
    {CUE_TALK, ACTOR_HOWLING_VILLAGER, CONDITION_ALWAYS, SCRIPT_REF(scGarohHowlForDog)},
    {CUE_TALK,
     ACTOR_VILLAGER_WARNING_STRANGERS, CONDITION_ALWAYS,
     MSGNO(MsgGarohYouThereStrangersShouldNot)},
    {CUE_TALK,
     ACTOR_VILLAGER_ON_THE_FULL_MOON, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohAskAboutFullMoon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_VILLAGER_ASKING_ABOUT_THE_WEREWOLF, CONDITION_ALWAYS,
     MSGNO(MsgGarohItsGrimLuckThatBrings)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_HOODED_VILLAGER, CONDITION_ALWAYS,
     MSGNO(MsgGarohSuchGorgeousMoonlightIfOnly)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_HOWLING_VILLAGER, CONDITION_ALWAYS,
     MSGNO(MsgGarohWhichOneOfUsWas)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_VILLAGER_WARNING_STRANGERS, CONDITION_ALWAYS,
     MSGNO(MsgGarohForeignersOnlyComeHereEn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_VILLAGER_ON_THE_FULL_MOON, CONDITION_ALWAYS,
     MSGNO(MsgGarohTheNightOfTheFull)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS),
     0, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohRevealCaveEntrance)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES),
     0, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohHideCaveEntrance)},
    {EXAMINE_CUE(EXAMINE_WELL),
     METATILE_BEHAVIOR_GAROH_WELL, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohMehISawTheSilhouette))},
    {CUE_TABLE_END},
};

CUE GarohVillageDayCues[] = {
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
     GAROH_TO_GAROH_ROOMS_HOUSES_SOUTH, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohOpenDoorAndEnter)},
	{CUE_EXIT | CUE_FLAG_FACING | FACING_NORTH, 2, CONDITION_ALWAYS, 2},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
     GAROH_TO_GAROH_ROOMS_HOUSES_NORTHWEST, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohOpenDoorAndEnter)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
     GAROH_TO_GAROH_ROOMS_HOUSES_SOUTHWEST, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohOpenDoorAndEnter)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
     GAROH_TO_GAROH_ROOMS_HOUSES_SOUTHEAST, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohOpenDoorAndEnter)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
     GAROH_TO_GAROH_ROOMS_SANCTUM, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohOpenDoorAndEnter)},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
	{CUE_EXIT | CUE_FLAG_FACING | FACING_NORTH, 8, CONDITION_ALWAYS, 8},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, 9},
	{CUE_EXIT, 10, CONDITION_ALWAYS, 10},
	{CUE_EXIT, 11, CONDITION_ALWAYS, 11},
	{STEP_CUE_FACING(FACING_NORTH),
     GAROH_TO_GAROH_WEREWOLF_CAVE, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohEnterOpening)},
	{CUE_TALK,
     ACTOR_VILLAGER_ASKING_ABOUT_THE_WEREWOLF, CONDITION_ALWAYS,
     MSGNO(MsgGarohIHearTellFromMaster)},
	{CUE_TALK, ACTOR_HOODED_VILLAGER, CONDITION_ALWAYS, MSGNO(MsgGaroh16)},
	{CUE_TALK, ACTOR_HOWLING_VILLAGER, CONDITION_ALWAYS, SCRIPT_REF(scGarohHowlOutLoud)},
	{CUE_TALK,
     ACTOR_VILLAGER_WARNING_STRANGERS, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohAskAboutSecret)},
	{CUE_TALK,
     ACTOR_VILLAGER_ON_THE_FULL_MOON, CONDITION_ALWAYS,
     MSGNO(MsgGarohIHeardThereWereOther)},
	{CUE_TALK, ACTOR_LITTLE_WEREWOLF, CONDITION_ALWAYS, SCRIPT_REF(scGarohLittleWerewolfThanks)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_VILLAGER_ASKING_ABOUT_THE_WEREWOLF, CONDITION_ALWAYS,
     MSGNO(MsgGarohHowCouldTheseOutsidersHave)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_HOODED_VILLAGER, CONDITION_ALWAYS,
     MSGNO(MsgGarohIThoughtWedNeverBe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_HOWLING_VILLAGER, CONDITION_ALWAYS,
     MSGNO(MsgGarohINeverThoughtIdBe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_VILLAGER_WARNING_STRANGERS, CONDITION_ALWAYS,
     MSGNO(MsgGarohMahaSaidTheseTravelersWere)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_VILLAGER_ON_THE_FULL_MOON, CONDITION_ALWAYS,
     MSGNO(MsgGarohIWouldLoveToMeet)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_LITTLE_WEREWOLF | CUE_SUBJECT_OWN_EFFECT,
     FLAG_LITTLE_WEREWOLF_THANKED, SCRIPT_REF(scGarohLittleWerewolfThanks)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_LITTLE_WEREWOLF, CONDITION_ALWAYS,
     MSGNO(MsgGarohMyDadToldMeI)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS),
     0, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohRevealCaveEntrance)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES),
     0, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohHideCaveEntrance)},
	{EXAMINE_CUE(EXAMINE_WELL),
     METATILE_BEHAVIOR_GAROH_WELL, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohMehISawTheSilhouette))},
	{CUE_TABLE_END},
};

CUE GarohPathCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{CUE_STEP,
     METATILE_BEHAVIOR_WEREWOLF_RUNS_PAST, FLAG_GAROH_SAW_WEREWOLF,
     SCRIPT_REF(scGarohWerewolfRunsPast)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES),
     0, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohIsNight)},
	{CUE_TABLE_END},
};

// No end row: the room reads on into the tunnels' table, whose exits and Reveal answer here too.
CUE GarohWayUpCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_STEP,
     METATILE_BEHAVIOR_LOWER_TWILIGHT_LINE, FLAG_CROSSED_LOWER_TWILIGHT_LINE,
     SCRIPT_REF(scGarohCrossLowerTwilightLine)},
	{CUE_STEP,
     METATILE_BEHAVIOR_UPPER_TWILIGHT_LINE, FLAG_CROSSED_UPPER_TWILIGHT_LINE,
     SCRIPT_REF(scGarohCrossUpperTwilightLine)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_FIRST_PILLAR, FLAG_FIRST_PILLAR_SUNK,
     SCRIPT_REF(scGarohLiftPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_FIRST_PILLAR, FLAG_FIRST_PILLAR_SUNK,
     SCRIPT_REF(scGarohPlaceFirstPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_SECOND_PILLAR, FLAG_SECOND_PILLAR_SUNK,
     SCRIPT_REF(scGarohLiftPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_SECOND_PILLAR, FLAG_SECOND_PILLAR_SUNK,
     SCRIPT_REF(scGarohPlaceSecondPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_THIRD_PILLAR, FLAG_GAROH_ROPE_LASHED,
     SCRIPT_REF(scGarohLiftPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_THIRD_PILLAR, FLAG_GAROH_ROPE_LASHED,
     SCRIPT_REF(scGarohPlaceThirdPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_FOURTH_PILLAR, FLAG_FOURTH_PILLAR_SUNK,
     SCRIPT_REF(scGarohLiftPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_FOURTH_PILLAR, FLAG_FOURTH_PILLAR_SUNK,
     SCRIPT_REF(scGarohPlaceFourthPillar)},
	{CUE_PUSH, 0, FLAG_FIRST_PILLAR_SUNK, SCRIPT_REF(scGarohLiftPushedPillar)},
	{CUE_PUSHED, 0, FLAG_FIRST_PILLAR_SUNK, SCRIPT_REF(scGarohPlacePushedPillar)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES),
     0, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohRestoreWayUpLight)},
};

CUE GarohTunnelsCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, 6},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 8, CONDITION_ALWAYS, 8},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 9, CONDITION_ALWAYS, 9},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 10, CONDITION_ALWAYS, 10},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 11, CONDITION_ALWAYS, 11},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS),
     0, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohRevealTunnelPassage)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES),
     0, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohHideTunnelPassage)},
	{CUE_TABLE_END},
};

CUE GarohWerewolfCaveCues[] = {
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT, 5, CONDITION_ALWAYS, 5},
	{STEP_CUE_FACING(FACING_NORTH), 6, CONDITION_ALWAYS, SCRIPT_REF(scGarohEnterOpening)},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 10, CONDITION_ALWAYS, 10},
	{CUE_STEP,
     METATILE_BEHAVIOR_CAVE_MOUTH, FLAG_GAROH_FOLLOWED_WEREWOLF,
     SCRIPT_REF(scGarohFollowWerewolfIntoCave)},
	{CUE_STEP, METATILE_BEHAVIOR_BELOW_MAHA, CONDITION_ALWAYS, SCRIPT_REF(scGarohMeetInTheCave)},
	{CUE_TALK, ACTOR_MAHA, CONDITION_ALWAYS, MSGNO(MsgGarohTakeCareInYourTravels)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_MAHA, CONDITION_ALWAYS,
     MSGNO(MsgGarohIWishThatIWere)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS),
     0, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohRevealCavePassage)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES),
     0, CONDITION_ALWAYS,
     SCRIPT_REF(scGarohHideCavePassage)},
	{CUE_TABLE_END},
};

// The ticks counted at night; a wolf howls every 1,024.
Sint32 GarohHowlTicks            = 0;

Uint32 GarohQuestionMarkScript[] = {
	SC_RELEASE,
};
#undef fdActorAnimateThenWait

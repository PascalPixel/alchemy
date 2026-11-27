/* @file */
/* Contigo, the village of the Jupiter clan north of Atteka Inlet, beside the */
/* crater below Jupiter Lighthouse, and the rooms of its overlay: the houses, */
/* shops, inn and Sanctum, the three game tents, and the Anemos geoglyph east */
/* of the village, where the villagers build the Wings of Anemos in front of */
/* the Anemos Sanctum. */
/* */
/* On Felix's first visit the village is busy with the wings. Scoop in the */
/* open gap of the big weed patch east of the village gate finds the Djinni */
/* Salt, and Cyclone on the clump of weeds beside the house at the top of the */
/* hill uncovers a Bramble Seed. The tent left of the inn holds Lucky Wheels, */
/* and the two tents north of the Teleport circle Super Lucky Dice (west) and */
/* Lucky Dice (east). Once Jupiter Lighthouse's beacon is lit, the villagers */
/* are away transporting the wings and four doors are locked; Isaac's party */
/* waits in the house at the top of the hill, and walking in starts the */
/* reunion (scContigoTheReunion). That house becomes Hamma's new home, and on */
/* the party's first visit after the ship has its wings she talks with Ivan */
/* (scContigoReturnToNewHome). In a linked game, Force on the tree stump */
/* behind the inn frees the Djinni Shine. Late in the game, Reveal in front of
 */
/* the Sanctum opens its middle door, and Teleport on the circle in the middle
 */
/* of the village leads into the Anemos Inner Sanctum. */

#include "scene/event_layer.h"
#include "macros.h"
#include "window/message_argument.h"
#include "item_ids.h"
#include "field/field_sequence.h"
#include "scene/sparkle.h"
#include "graphics/palette_buffers.h"
#include "version.h"
#include "shop/shop.h"

/* Force has freed Shine from the tree stump it was stuck in. */
#define FLAG_SHINE_FREED 0x9e7
/* Set as Shine, freed, joins the party; until then it waits where it landed. */
#define FLAG_SHINE_CAUGHT 0x9e6
/* Set once Hamma has welcomed the party to her new home, after it set out */
/* with the wings, and told Ivan why she kept from him that she is his sister */
/* (scContigoReturnToNewHome). */
#define FLAG_HAMMA_WELCOMED_PARTY 0x9d1

/* The village's temp flag: the camera follows the leader east past the */
/* village gate as far as the weed patch, rather than stopping at the gate */
/* (scContigoWidenCameraBounds). */
#define FLAG_WHOLE_VILLAGE_IN_VIEW FLAG_SCENE_TEMP_00

/* The map cells the village's, the geoglyph's and the houses' trigger rows */
/* answer. A door's cell takes the number of the exit it leaves by, so those */
/* rows use the exit's own name. */
typedef enum CONTIGO_CELL_BEHAVIOR {
	/* The geoglyph's west edge, where the path from the village's east edge */
	/* comes in. */
	CELL_BEHAVIOR_GEOGLYPH_WEST_EDGE = 12,
	/* The doors at the head of the Sanctum's west, middle and east steps. */
	CELL_BEHAVIOR_SANCTUM_WEST_DOOR = 13,
	CELL_BEHAVIOR_SANCTUM_MIDDLE_DOOR = 14,
	CELL_BEHAVIOR_SANCTUM_EAST_DOOR = 15,
	/* A door shut while the villagers are away with the wings */
	/* (scContigoLockDoors). */
	CELL_BEHAVIOR_LOCKED_DOOR = 19,
	/* Across the three ways from the middle of the village down to its south */
	/* part: the north line keeps the view west of the gate, the south one lets
	 */
	/* it follow the leader east. */
	CELL_BEHAVIOR_VIEW_WEST_OF_GATE = 20,
	CELL_BEHAVIOR_VIEW_EAST_OF_GATE = 21,
	/* Just inside the door of Hamma's new home. */
	CELL_BEHAVIOR_INSIDE_HAMMAS_DOOR = 20,
	/* The Sanctum's carved wall beside the translation sign. */
	CELL_BEHAVIOR_CARVED_WALL = 22,
	/* The signs outside the three game tents. */
	CELL_BEHAVIOR_LUCKY_WHEELS_SIGN = 24,
	CELL_BEHAVIOR_SUPER_LUCKY_DICE_SIGN = 25,
	CELL_BEHAVIOR_LUCKY_DICE_SIGN = 26,
	CELL_BEHAVIOR_TELEPORT_CIRCLE = 30,
	/* The open gap in the middle of the weed patch, where Salt hides. */
	CELL_BEHAVIOR_WEED_PATCH_GAP = 32,
	/* The clumps of weeds about the village, and the five rows of the big */
	/* patch east of the gate, from north to south. */
	CELL_BEHAVIOR_WEEDS_WEST_OF_ITEM_SHOP = 39,
	CELL_BEHAVIOR_WEEDS_BEHIND_INN,
	CELL_BEHAVIOR_WEEDS_OVER_BRAMBLE_SEED,
	CELL_BEHAVIOR_WEEDS_WEST_OF_WEAPON_SHOP,
	CELL_BEHAVIOR_WEEDS_BY_GATE,
	CELL_BEHAVIOR_WEEDS_BELOW_LUCKY_WHEELS,
	CELL_BEHAVIOR_WEEDS_ON_CLIFF_WEST,
	CELL_BEHAVIOR_WEEDS_ON_CLIFF_EAST,
	CELL_BEHAVIOR_WEED_PATCH_TOP_ROW,
	CELL_BEHAVIOR_WEED_PATCH_SECOND_ROW,
	CELL_BEHAVIOR_WEED_PATCH_MIDDLE_ROW,
	CELL_BEHAVIOR_WEED_PATCH_FOURTH_ROW,
	CELL_BEHAVIOR_WEED_PATCH_BOTTOM_ROW,
	/* The pots and ovens in the houses: the bean soup, the corn bread and the
	 */
	/* cookies. */
	CELL_BEHAVIOR_BEAN_SOUP = 200,
	CELL_BEHAVIOR_CORN_BREAD,
	CELL_BEHAVIOR_COOKIES
} CONTIGO_CELL_BEHAVIOR;

/* The tile row of Hamma's portrait as she and Isaac speak at once in the */
/* reunion (scContigoTheReunion): two rows higher from the first patch on. */
#if !TLA_PATCH_1
#define REUNION_HAMMA_PORTRAIT_Y 12
#else
#define REUNION_HAMMA_PORTRAIT_Y 10
#endif

/* What the dice and wheels attendants say as the party comes back from a */
/* game (scContigoInitLuckyDice); the Spanish text numbers these lines */
/* differently. */
#if LANGUAGE == LANGUAGE_SPANISH
#define COIN_WON_MESSAGE		((s32)MsgContigoHeyYaDontHaveEnough + 1)
#define COIN_LOST_MESSAGE		((s32)MsgContigoHeyYaDontHaveEnough + 2)
#define READ_SIGN_MESSAGE		((s32)MsgContigoYaWonCoinTalkAbout)
#define CONGRATULATIONS_MESSAGE ((s32)MsgContigoYaLostCoinBetterLuck)
#define FIRST_PRIZE_MESSAGE		((s32)MsgContigoTheRulesAreListedOn)
#define NEXT_PRIZE_MESSAGE		((s32)MsgContigoCongratulations)
#define COME_AGAIN_MESSAGE		((s32)MsgContigoComeAgain)
#else
#define COIN_WON_MESSAGE		((s32)MsgContigoYaWonCoinTalkAbout)
#define COIN_LOST_MESSAGE		((s32)MsgContigoYaLostCoinBetterLuck)
#define READ_SIGN_MESSAGE		((s32)MsgContigoTheRulesAreListedOn)
#define CONGRATULATIONS_MESSAGE ((s32)MsgContigoCongratulations)
#define FIRST_PRIZE_MESSAGE		((s32)MsgContigoYouWonHereYouGo)
#define NEXT_PRIZE_MESSAGE		((s32)MsgContigoAndYouWonHereYou)
#define COME_AGAIN_MESSAGE		((s32)MsgContigoComeAgain)
#endif

/* Set as the party leaves the dice hall for one of its games, so that the */
/* hall greets it on its way back by the game's entrance. */
#define FLAG_LEFT_FOR_A_GAME 0x9e5

/* The dice hall's ways back from its games. */
typedef enum CONTIGO_DICE_HALL_ENTRANCE {
	ENTRANCE_FROM_SUPER_LUCKY_DICE = 18,
	ENTRANCE_FROM_LUCKY_DICE,
	ENTRANCE_FROM_LUCKY_WHEELS
} CONTIGO_DICE_HALL_ENTRANCE;

/* The village's people and Djinn (Contigo_VillagePlacements). */
typedef enum CONTIGO_VILLAGE_ACTOR_ID {
	/* "The Anemos blood in our veins often brings about strange..." */
	ACTOR_YOUNG_MAN_ON_THE_WIND_LEGEND = ACTOR_FIRST_PLACED,
	/* "The beacon within Jupiter Lighthouse will soon be lit. I know this */
	/* because of a strange premonition I had three years ago." */
	ACTOR_WOMAN_FORESEEING_THE_BEACON,
	/* "It must be so exciting to travel around the world in a flying ship." */
	ACTOR_MAN_ON_THE_FLYING_SHIP,
	/* "The people of Anemos possessed power over the wind." */
	ACTOR_WOMAN_ON_THE_ANEMOS,
	/* "If a ship can rise up from the sea, why can't a city rise up from the */
	/* ground?" */
	ACTOR_OLD_MAN_ON_THE_RISEN_CITY,
	/* "The Anemos geoglyphs show a boat with wings..." */
	ACTOR_OLD_WOMAN_ON_THE_GEOGLYPHS,
	/* The two children walking round the middle of the village: the boy, */
	/* "Cities flying up into the sky? I don't buy that for a second!" */
	/* (scContigoTalkToDoubtingBoy), and the girl, "It wasn't what I expected,
	 */
	/* but seeing the ship take off was still amazing." */
	/* (scContigoTalkAboutTheShip) */
	/* @{ */
	ACTOR_BOY_DOUBTING_THE_LEGENDS,
	ACTOR_GIRL_ON_THE_SHIP,
	/* @} */
	/* The Djinn Salt (Venus) and Shine (Mars). */
	ACTOR_SALT,
	ACTOR_SHINE
} CONTIGO_VILLAGE_ACTOR_ID;

/* The shops and inn before the reunion */
/* (Contigo_ShopsPlacementsBeforeReunion). */
typedef enum CONTIGO_SHOPS_ACTOR_ID {
	/* "I guess the lighthouse was lit because Ivan and Hamma finally returned
	 */
	/* to Contigo." */
	ACTOR_MAN_ON_IVANS_RETURN = ACTOR_FIRST_PLACED,
	/* "Ivan doesn't remember his mother..." */
	ACTOR_WOMAN_ON_IVANS_MOTHER,
	/* "From the look on Ivan's..."; his thought: "Someday I'm gonna fight */
	/* Ivan..." */
	ACTOR_BOY_WHO_WOULD_FIGHT_IVAN,
	/* "Long ago, the people of Hesperia attacked Atteka." */
	ACTOR_OLD_MAN_ON_HESPERIA,
	/* "Winter just ended, but it keeps getting colder. What's going on?" */
	ACTOR_OLD_WOMAN_FEELING_THE_COLD,
	/* The woman behind the item shop's counter (scContigoTalkToItemMerchant).
	 */
	ACTOR_ITEM_MERCHANT,
	/* "Soon, the siblings with the power of the Anemos will return." */
	ACTOR_YOUNG_MAN_ON_THE_SIBLINGS,
	/* "The power of the Anemos is all but gone in Contigo." */
	ACTOR_WOMAN_ON_THE_ANEMOS_POWER,
	/* The baby's brother: "I don't have any powers like the Anemos did, but my
	 */
	/* sister, Ahri, is different." */
	ACTOR_BABYS_BROTHER,
	/* The baby whose furrowed brow brings the wind (scContigoBabyThinks). */
	ACTOR_BABY,
	/* The two men behind the counter of swords and shields: the weapon */
	/* merchant, and the armor merchant, "Mining materials in the ruins is */
	/* dangerous work, so my armor is selling well." */
	/* @{ */
	ACTOR_WEAPON_MERCHANT,
	ACTOR_ARMOR_MERCHANT,
	/* @} */
	ACTOR_INNKEEPER,
	/* "All the work on the wings means that our inn is always full!" */
	ACTOR_INN_WORKER,
	/* "Gotta make all the beds before everyone returns from Anemos */
	/* Sanctum..." */
	ACTOR_WOMAN_MAKING_THE_BEDS,
	/* "I must be the greatest chef in the world!" */
	ACTOR_COOK,
	/* The workers on the wings, resting at the inn: "I worked myself half to */
	/* death yesterday." */
	ACTOR_WORN_OUT_WORKER,
	/* "The prophecies of the Anemos tell that someday soon..." */
	ACTOR_WORKER_ON_THE_PROPHECIES,
	/* "I'm afraid I'm too old to help in the construction..." */
	ACTOR_OLD_MAN_TOO_OLD_TO_HELP,
	/* "When is that miracle of the Anemos gonna hurry up..." */
	ACTOR_OLD_WOMAN_AWAITING_THE_MIRACLE,
	/* The robed man behind the Sanctum's counter (scContigoTalkToHealer). */
	ACTOR_HEALER
} CONTIGO_SHOPS_ACTOR_ID;

/* Hamma and Kraden at the reunion and after it (Contigo_ShopsPlacements, */
/* Contigo_ShopsPlacementsReunion). */
typedef enum CONTIGO_REUNION_ACTOR_ID {
	ACTOR_HAMMA = ACTOR_FIRST_PLACED,
	ACTOR_KRADEN
} CONTIGO_REUNION_ACTOR_ID;

/* The dice and wheels hall (Contigo_LuckyDicePlacements). */
typedef enum CONTIGO_LUCKY_DICE_ACTOR_ID {
	/* A young man with fair hair (SPRITE_YOUNG_MAN_WITH_FAIR_HAIR), who runs */
	/* Super Lucky Dice (scContigoOfferSuperLuckyDice). */
	ACTOR_SUPER_LUCKY_DICE_ATTENDANT = ACTOR_FIRST_PLACED,
	/* Beside him, in his sprite, pointing the party to the rules: "If you want
	 */
	/* to learn about the rules to Super Lucky Dice, just check out the sign."
	 */
	ACTOR_SUPER_LUCKY_DICE_RULES_ATTENDANT,
	/* The players: "This kid's got unlucky written all over his face" (his */
	/* thought); "If you lose the hi-lo game, you don't win anything..."; */
	/* "I almost always get at least one pair..." */
	/* @{ */
	ACTOR_MAN_WARY_OF_UNLUCKY_PLAYERS,
	ACTOR_OLD_MAN_ON_THE_HI_LO_GAME,
	ACTOR_WOMAN_WHO_LOSES_AT_HI_LO,
	/* @} */
	/* The sign of Super Lucky Dice's rules (scContigoReadSuperLuckyDiceRules).
	 */
	ACTOR_SUPER_LUCKY_DICE_RULES_SIGN,
	/* A young man with fair hair, in other colours, who runs Lucky Dice */
	/* (scContigoOfferLuckyDice). */
	ACTOR_LUCKY_DICE_ATTENDANT,
	/* Beside him, in his sprite, pointing the party to the rules: "Read the */
	/* sign if ya don't know the rules." */
	ACTOR_LUCKY_DICE_RULES_ATTENDANT,
	/* Lucky Dice's two signs: how to play (scContigoReadLuckyDiceRules), and */
	/* what pairs, triples and four of a kind win (scContigoReadLuckyDicePairs).
	 */
	/* @{ */
	ACTOR_LUCKY_DICE_RULES_SIGN,
	ACTOR_LUCKY_DICE_PAIRS_SIGN,
	/* @} */
	/* A young woman in green (SPRITE_YOUNG_WOMAN_IN_GREEN), who runs Lucky */
	/* Wheels (scContigoTalkToWheelsAttendant). */
	ACTOR_LUCKY_WHEELS_ATTENDANT,
	/* Lucky Wheels' signs: its rules (scContigoReadLuckyWheelsRules), and its
	 */
	/* prizes (scContigoReadLuckyWheelsPrizes). */
	/* @{ */
	ACTOR_LUCKY_WHEELS_RULES_SIGN,
	ACTOR_LUCKY_WHEELS_PRIZE_SIGN
	/* @} */
} CONTIGO_LUCKY_DICE_ACTOR_ID;

/* The geoglyph, where the Wings of Anemos are built */
/* (Contigo_GeoglyphPlacements). */
typedef enum CONTIGO_GEOGLYPH_ACTOR_ID {
	/* "I've come a long way to see the flying boat." */
	ACTOR_VISITOR_FROM_AFAR = ACTOR_FIRST_PLACED,
	/* "The wings are almost done." */
	ACTOR_WOMAN_ON_THE_WINGS,
	/* The four workers building the wings: "Which part goes where? Does this */
	/* metal feather connect to that wooden post?" */
	/* @{ */
	ACTOR_WING_WORKER_A,
	ACTOR_WING_WORKER_B,
	ACTOR_WING_WORKER_C,
	ACTOR_WING_WORKER_D,
	/* @} */
	/* The four men in long robes by the Sanctum's wall: "Have you taken a good
	 */
	/* look at the Anemos geoglyph?"; "I know that devoted pilgrims will */
	/* appreciate having this translation."; "I want to get a good look at the
	 */
	/* inside of the sanctum, but I can't get this door open."; "The merchant */
	/* Hammet took the gold in this room with him when he left Contigo." */
	/* @{ */
	ACTOR_MAN_ON_THE_GEOGLYPH,
	ACTOR_MAN_WHO_TRANSLATED_THE_WALL,
	ACTOR_MAN_LOCKED_OUT_OF_THE_SANCTUM,
	ACTOR_MAN_ON_HAMMET,
	/* @} */
	/* The sign with a translation of the characters carved into the wall. */
	ACTOR_SIGN,
	/* The wings' parts lying about: three frames, five metal feathers and */
	/* seven loose pieces. */
	/* @{ */
	ACTOR_WING_FRAME_A,
	ACTOR_WING_FRAME_B,
	ACTOR_WING_FRAME_C,
	ACTOR_METAL_FEATHER_A,
	ACTOR_METAL_FEATHER_B,
	ACTOR_METAL_FEATHER_C,
	ACTOR_METAL_FEATHER_D,
	ACTOR_METAL_FEATHER_E,
	ACTOR_LOOSE_PART_A,
	ACTOR_LOOSE_PART_B,
	ACTOR_LOOSE_PART_C,
	ACTOR_LOOSE_PART_D,
	ACTOR_LOOSE_PART_E,
	ACTOR_LOOSE_PART_F,
	ACTOR_LOOSE_PART_G
	/* @} */
} CONTIGO_GEOGLYPH_ACTOR_ID;
#include "field/event_inline.h"
#include "field/actor_script.h"

struct ACTOR;

void scContigoOfferLuckyWheels(s32 mode);
void scContigoOpenSanctumDoor(s32 sourceX, s32 sourceY, s32 doorX, s32 doorY);

#include "scene/scene.h"
#include "field/field_sequence_event.h"
#include "game/game.h"

/* The village's treasure actors after the Power Bread's (SceneItems): the */
/* Corn, and the Bramble Seed under the weeds north of the Teleport circle. */
enum {
	ACTOR_CORN = FIELD_TREASURE_ACTOR + 1,
	ACTOR_BRAMBLE_SEED = FIELD_TREASURE_ACTOR + 2
};

extern const s32 *FieldEffect_DurationScripts[];
extern ACTOR_PLACE Contigo_VillagePlacements[];
extern ACTOR_PLACE Contigo_GeoglyphPlacements[];
extern ACTOR_PLACE Contigo_ShopsPlacementsBeforeReunion[];
extern ACTOR_PLACE Contigo_ShopsPlacementsReunion[];
extern ACTOR_PLACE Contigo_ShopsPlacements[];
extern ACTOR_PLACE Contigo_LuckyDicePlacements[];
extern TRIGGER Contigo_VillageTriggers[];
extern TRIGGER Contigo_ShopsTriggers[];
extern TRIGGER Contigo_ShopsReunionTriggers[];

void scTeleportStartShimmer(void);

void scTeleportStopShimmer(void);

extern u32 Contigo_WheelsPrizes[];
/* The Lucky Wheels' icons that win prizes, shirts, boots, rings, then the */
/* stars' and hearts' items, three prizes to each (Contigo_WheelsPrizes); the */
/* Moon after them is wild, any of the five at random. */
enum {
	WHEELS_PRIZE_ICONS = 5,
	WHEELS_ICON_MOON = WHEELS_PRIZE_ICONS,
	WHEELS_PRIZES_PER_ICON = 3
};
extern s32 Contigo_EnterSanctumDoorScript[];
extern u32 Contigo_RejoinLeaderScript[];
void scContigoInitVillage(void);
void scContigoInitGeoglyph(void);
void scContigoInitShops(void);
void scContigoInitLuckyDice(void);
extern u16 TeleportCircle_PaletteSlots[];
extern u16 TeleportCircle_Colors[];
extern u16 TeleportCircle_ShimmerColors[];
extern u16 TeleportCircle_ShimmerSlots[];
extern s32 Contigo_ShiverScript[];
extern u32 LightRing_Tiles[];
extern u32 FieldEffect_ShortScript[];
extern u32 FieldEffect_MediumScript[];
extern u32 FieldEffect_LongScript[];
extern u32 Contigo_DoubtingBoyScript[];
extern u32 Contigo_GirlOnTheShipScript[];
extern s32 Contigo_PuzzledWorkerScript[];

u16 PaletteCycleBright BSS_DATA;
u16 *PaletteCycleSlots BSS_DATA;
u16 PaletteCycleTimer BSS_DATA;
u16 PaletteCycleDelay BSS_DATA ALIGNED(4);
u16 *PaletteSlots BSS_DATA;
u16 *PaletteColors BSS_DATA;
u16 *PaletteCycleColors BSS_DATA;
u16 PaletteCycleLength BSS_DATA;
u16 PaletteCycleIndex BSS_DATA ALIGNED(4);

void scTeleportArriveWhirling(void);

s32 syListCountEntries(u16 *list);

void fdPaletteCycleFade(void);

#include "shared/teleport_shimmer.c"

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#include "text/msg_ids.h"

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT

/* Salt, popped up out of the gap in the weed patch east of the village gate, */
/* joins the party: "Felix found the Venus Djinni Salt!" */
void scContigoSaltJoinsParty(void) {
	fdDjinnJoinParty(ACTOR_SALT, DJINNI_SALT / DJINN_PER_ELEMENT,
					 DJINNI_INDEX(DJINNI_SALT));
}

/* Talking to Shine where it landed, beside the tree stump behind the inn, */
/* once Force has freed it: the Mars Djinni joins the party. */
void scContigoFreedDjinniJoinsParty(void) {
	fdDjinnJoinParty(ACTOR_SHINE, DJINNI_SHINE / DJINN_PER_ELEMENT,
					 DJINNI_INDEX(DJINNI_SHINE));
	syGameFlagSet(FLAG_SHINE_CAUGHT);
}

#include "shared/position_distance.h"

#include "shared/actor_is_facing_near.h"

/* The update of the two children walking round the middle of the village: */
/* each stands while it is talked to, while Mind Read shows its thoughts, or */
/* while it faces the other child or the leader close by, and walks on */
/* otherwise or once the leader is right by it. */
s32 scContigoPauseWhenFaced(ACTOR *self) {
	FIELD_STATE *event = Ram_HeapSlots->fieldState;
	u8 *state;
	u32 mode;
	FIELD_EFFECT_STATE *effect;
	ACTOR *other;

	fdActorSetSprPriorityDirect(self, fdActorGetSprPriority(fdActorGetById(
										  MainRoutineStateWork.leaderActorId)));
	mode = ABILITY_ID(event->psynergy);
	state = &self->paused;
	if(mode == ABILITY_MIND_READ) {
		effect = Ram_HeapSlots->fieldEffState;
		if((s8)effect->msgShown != 0)
			goto stand;
	}
	if(self->sceneByte == 1)
		goto stand;
	other = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(scPosDistance(&self->x.fixed, &other->x.fixed) <= CHILD_WALKS_ON_RANGE)
		goto walk;
	if(self->sceneCnt == 1)
		other = fdActorGetById(ACTOR_GIRL_ON_THE_SHIP);
	else
		other = fdActorGetById(ACTOR_BOY_DOUBTING_THE_LEGENDS);
	if(fdActorIsFacingNear(self, other, CHILD_SEES_CHILD_RANGE) != 0)
		goto stand;
	if(fdActorIsFacingNear(self,
						   fdActorGetById(MainRoutineStateWork.leaderActorId),
						   CHILD_SEES_LEADER_RANGE) != 0)
		goto stand;
walk:
	*state = 0;
	fdActorSelectAnimation(self, ANIM_WALK);
	return 0;
stand:
	*state = 1;
	fdActorSelectAnimation(self, ANIM_STAND);
	return 0;
}

/* A question mark over the worker wondering which part goes where, as he */
/* looks from one part of the wings to another (Contigo_PuzzledWorkerScript). */
s32 scContigoShowWorkerPuzzled(void) {
	scFieldActorShowEmote(ACTOR_WING_WORKER_B, EMOTE_IN_FRONT | EMOTE_QUESTION,
						  0);
	return 0;
}

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

extern ACTOR_PLACE Contigo_Placements[];

const ACTOR_PLACE *scGetActorPlacements(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_CONTIGO)
		return Contigo_VillagePlacements;
	if(scene == SCENE_CONTIGO_GEOGLYPH)
		return Contigo_GeoglyphPlacements;
	if(scene == SCENE_CONTIGO_SHOPS) {
		if(MainRoutineStateWork.entrance == CONTIGO_SHOPS_FROM_CONTIGO_NORTH) {
			if(syGameFlagGet(FLAG_SET_OUT_WITH_WINGS) != 0)
				return Contigo_ShopsPlacements;
			if(syGameFlagGet(FLAG_JUPITER_BEACON_LIT) != 0 &&
			   syGameFlagGet(FLAG_REUNION_AT_CONTIGO) == 0)
				return Contigo_ShopsPlacementsReunion;
		}
		return Contigo_ShopsPlacementsBeforeReunion;
	}
	if(scene == SCENE_CONTIGO_LUCKY_DICE)
		return Contigo_LuckyDicePlacements;
	return Contigo_Placements;
}

extern TRIGGER Contigo_Triggers[];
extern TRIGGER Contigo_GeoglyphTriggers[];
extern TRIGGER Contigo_LuckyDiceTriggers[];

const TRIGGER *scGetTriggers(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_CONTIGO)
		return Contigo_VillageTriggers;
	if(scene == SCENE_CONTIGO_GEOGLYPH)
		return Contigo_GeoglyphTriggers;
	if(scene == SCENE_CONTIGO_SHOPS) {
		if(MainRoutineStateWork.entrance == CONTIGO_SHOPS_FROM_CONTIGO_NORTH)
			return Contigo_ShopsReunionTriggers;
		return Contigo_ShopsTriggers;
	}
	if(scene == SCENE_CONTIGO_LUCKY_DICE)
		return Contigo_LuckyDiceTriggers;
	return Contigo_Triggers;
}

/* Felix examines the Sanctum's front wall between its west and middle steps, */
/* behind the translation sign (cells (45, 18) and (46, 18)): "There are */
/* characters carved into it", which he cannot make sense of. */
void scContigoExamineWall(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	/* "Felix examined the wall. There are characters carved into it...." */
	mnFieldMessageShow((s32)MsgContigoFelixExaminedTheWallThere, 1);
	fdEventEnd();
}

/* Felix reads the small sign in front of the Sanctum's carved wall: "The */
/* characters from the wall are translated" on it. */
void scContigoReadSign(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	/* "Felix examined the sign. The characters from the wall are translated..."
	 */
	mnFieldMessageShow((s32)MsgContigoFelixExaminedTheSignThe, 1);
	fdEventEnd();
}

/* Felix reads the sign outside the Lucky Wheels tent, left of the inn (cell */
/* (4, 55)): "Turn the slot and get a prize! The lottery slots are over here."
 */
void scContigoReadLuckyWheelsSign(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	/* "Turn the slot and get a prize! The lottery slots are over here." */
	mnFieldMessageShow((s32)MsgContigoTurnTheSlotAndGet, 1);
	fdEventEnd();
}

/* Felix reads the sign west of the two tents north of the Teleport circle */
/* (cell (11, 47)), which points to Super Lucky Dice in the west tent: "Drop */
/* some cash and double up! The hi-lo dice are over here!" */
void scContigoReadSuperLuckyDiceSign(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	/* "Drop some cash and double up! The hi-lo dice are over here!" */
	mnFieldMessageShow((s32)MsgContigoDropSomeCashAndDouble, 1);
	fdEventEnd();
}

/* Felix reads the sign east of the two tents north of the Teleport circle */
/* (cell (19, 47)), which points to Lucky Dice in the east tent: "You never */
/* know what the dice will do next! Come and give our dice a toss!" */
void scContigoReadLuckyDiceSign(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	/* "You never know what the dice will do next! Come and give our dice a..."
	 */
	mnFieldMessageShow((s32)MsgContigoYouNeverKnowWhatThe, 1);
	fdEventEnd();
}

#include "shared/scene_show_entrance_locked.h"

/* Until Jupiter Lighthouse is lit, the young man a few steps inside the */
/* village gate asks Felix whether he knows the legend of the people who could
 */
/* control the wind and air, and answers his yes or no. */
void scContigoAskAboutWindLegend(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId((s32)MsgContigoDoYouKnowAnythingAbout);
	/* "Do you know anything about a legend about these people who could..." */
	fdEventAskYesNoAndShowResponse(ACTOR_YOUNG_MAN_ON_THE_WIND_LEGEND, 0);
	fdEventEnd();
}

/* The update of a leaf in the baby's whirlwind: it circles on a random radius,
 */
/* now and then pausing to spin faster, and is released after 101 frames. */
void scContigoWhirlLeaf(ACTOR *effect) {
	FIXED radius;
	s32 angle;
	FIXED dx;
	FIXED dz;

	/* syRandom is 0 to 1 in 16.16, so 2 to 3 pixels. */
	radius = syRandom() + toFIXED(2);
	angle = effect->facing;
	dx = syMulFX(radius, syCos(angle));
	dz = syMulFX(radius, sySin(angle));
	effect->x.fixed += dx;
	effect->z.fixed += dz;
	/* A small backwards drift, a 4096th of a turn a frame. */
	effect->facing += -(TRIG_TURN / 4096);
	if(effect->turnFrm != 0) {
		effect->turnFrm--;
		effect->facing += TRIG_TURN / 32;
	}
	else if(syRandomBelow(32) == 0) {			 /* one chance in 32 */
		effect->turnFrm = syRandomBelow(16) + 8; /* 8 to 23 frames */
	}
	if(++effect->swirlFrm == 101)
		fdActorRelease(effect);
}

/* A task that spawns a leaf in the baby's whirlwind every eighth frame, flung
 */
/* up to fall back as it circles. */
void scContigoRaiseLeaves(void) {
	if((FrameCount & 7) == 0) {
		ACTOR *actor =
			fdActorSpawn(SPRITE_LEAF, toFIXED(706), toFIXED(0.8), toFIXED(116));

		if(actor != NULL) {
			fdActorSetSprFlags(actor, 0);
			actor->update = scContigoWhirlLeaf;
			actor->motionFlg = 0;
			actor->swirlFrm = -(s32)(syRandomBelow(16)) * 2 + 20;
			actor->turnFrm = 0;
			actor->gravity = toFIXED(1) / 10;
			actor->velY = toFIXED(3);
			actor->facing = syRandom();
		}
	}
}

/* Talking to the baby in its cradle, in the house north of the weapon shop: */
/* "The baby's forehead is furrowed, like it's thinking." The room turns dark */
/* blue, a whirlwind grows over the cradle and leaves whirl up in it, then the
 */
/* light comes back, the baby babbles "Babu bawaba bibaw.", and its brother */
/* grumbles that the shabby old house is drafty. */
void scContigoBabyThinks(void) {
	ACTOR *effect;
	FIXED x = toFIXED(706);
	FIXED z = toFIXED(116);
	u32 i;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	if(syGameFlagGet(FLAG_SET_OUT_WITH_WINGS))
		/* "The baby's forehead is furrowed, like it's thinking." */
		mnFieldMessageShow((s32)MsgContigoTheBabysForeheadIsFurrowedLike, 1);
	else
		/* "The baby's forehead is furrowed, like it's thinking." */
		mnFieldMessageShow((s32)MsgContigoTheBabysForeheadIsFurrowed, 1);
	fdActorSetAnimationById(ACTOR_BABY, 3);
	fdPaletteFadeSetSource(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeSetTarget(PALETTE_TINT_CUSTOM | RGB(4, 4, 16), PALETTES_BG);
	fdPaletteFadeStartInterpolation(16);
	fdBattleEffectStartArcEffect();
	fdBattleEffectSetObjectUpdateMode(fdActorGetById(ACTOR_BABY),
									  CASTER_LOOK_CYCLING_COLORS);
	effect = syImportActorCreate(SPRITE_WHIRLWIND, x, 0, z);
	if(effect != 0) {
		fdActorSetSprFlags(effect, 0);
		fdActorSelectAnimation(effect, 2);
		effect->motionFlg = 0;
		effect->scaleX = FIXED_ONE / 10;
		effect->scaleY = FIXED_ONE / 10;
		for(i = 0; i < 28; i++) {
			effect->scaleX += FIXED_ONE / 40;
			effect->scaleY += FIXED_ONE / 40;
			sySynch(1);
		}
	}
	sySetEvent(scContigoRaiseLeaves, TASK_PASS_FRAME);
	fdEventWait(120);
	syCloseEvent(scContigoRaiseLeaves);
	if(effect != 0) {
		for(i = 0; i < 28; i++) {
			effect->scaleX -= FIXED_ONE / 40;
			effect->scaleY -= FIXED_ONE / 40;
			effect->y.fixed += toFIXED(1) / 2;
			sySynch(1);
		}
		fdActorRelease(effect);
	}
	fdBattleEffectSetObjectUpdateMode(fdActorGetById(ACTOR_BABY),
									  CASTER_LOOK_NORMAL);
	fdBattleEffectWaitForArcGroups();
	fdBattleEffectEndArcEffect();
	fdPaletteFadeSetTarget(PALETTE_TINT_NONE, PALETTES_BG);
	fdPaletteFadeStartInterpolation(16);
	if(syGameFlagGet(FLAG_SET_OUT_WITH_WINGS))
		/* "Goo goo. Gaa gaa... Waaa!" */
		fdEventSetMessageId((s32)MsgContigoGooGooGaaGaaWaaa);
	else
		/* "Babu bawaba bibaw." */
		fdEventSetMessageId((s32)MsgContigoBabuBawabaBibaw);
	fdActorRunRepeatedMotion(ACTOR_BABY, 2);
	fdEventShowMessageAndWait(ACTOR_BABY, 0);
	fdActorSetAnimationById(ACTOR_BABY, 1);
	scFieldActorShowEmote(ACTOR_YOUNG_MAN_ON_THE_SIBLINGS,
						  EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdActorFaceDirection(ACTOR_YOUNG_MAN_ON_THE_SIBLINGS, FACING_WEST, 20);
	/* "It sure is drafty in this shabby old house." */
	fdEventShowMessageAndWait(ACTOR_YOUNG_MAN_ON_THE_SIBLINGS, 0);
	fdActorFaceDirectionAndRefreshById(ACTOR_WOMAN_ON_THE_ANEMOS_POWER,
									   FACING_EAST);
	/* "When the Wings of Anemos are complete, let's fix the house!" */
	fdEventShowMessageAndWait(ACTOR_WOMAN_ON_THE_ANEMOS_POWER, 0);
	fdActorSetAnimationById(ACTOR_YOUNG_MAN_ON_THE_SIBLINGS, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_WOMAN_ON_THE_ANEMOS_POWER, ANIM_NOD);
	fdActorFaceDirection(ACTOR_WOMAN_ON_THE_ANEMOS_POWER,
						 FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_YOUNG_MAN_ON_THE_SIBLINGS,
								FACING_SOUTH_SOUTHEAST);
	fdEventEnd();
}

#include "shared/leader_is_facing_north.h"

/* Contigo's shops, as mnShopRun numbers them. */
enum {
	SHOP_CONTIGO_WEAPONS = 26,
	SHOP_CONTIGO_ARMOR = 27,
	SHOP_CONTIGO_ITEMS = 28
};

/* Contigo's inn, as mnInnCheckIn numbers the inns. */
enum { CONTIGO_INN = 12 };

/* Talking to the item merchant across her counter, in the west of the two */
/* houses south of the Teleport circle, opens the item shop; from any other */
/* side she talks of the wings. */
void scContigoTalkToItemMerchant(void) {
	if(fdLeaderIsFacingNorth()) {
		mnShopRun(SHOP_CONTIGO_ITEMS, ACTOR_ITEM_MERCHANT);
	}
	else {
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		if(syGameFlagGet(FLAG_SET_OUT_WITH_WINGS))
			/* "It's always such a letdown when you finish off a major task
			 * like..." */
			fdEventSetMessageId((s32)MsgContigoItsAlwaysSuchALetdown);
		else
			/* "The villagers are working their hardest to complete the wings
			 * and..." */
			fdEventSetMessageId((s32)MsgContigoTheVillagersAreWorkingTheir);
		fdEventShowMessageAndWait(ACTOR_ITEM_MERCHANT, 0);
		fdEventEnd();
	}
}

/* Talking to the weapon merchant, the west of the two men behind the counter */
/* in the shop east of the Teleport circle, across it opens his shop; from any
 */
/* other side he talks of Atteka's traditions. */
void scContigoTalkToWeaponMerchant(void) {
	if(fdLeaderIsFacingNorth()) {
		mnShopRun(SHOP_CONTIGO_WEAPONS, ACTOR_WEAPON_MERCHANT);
	}
	else {
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		if(syGameFlagGet(FLAG_SET_OUT_WITH_WINGS))
			/* "A cannon? Yeah, I think I heard something about a town up in..."
			 */
			fdEventSetMessageId((s32)MsgContigoACannonYeahIThink);
		else
			/* "It was a longstanding tradition that people in Atteka would take
			 * a..." */
			fdEventSetMessageId((s32)MsgContigoItWasALongstandingTradition);
		fdEventShowMessageAndWait(ACTOR_WEAPON_MERCHANT, 0);
		fdEventEnd();
	}
}

/* Talking to the armor merchant, the east of the two men behind the counter */
/* in the shop east of the Teleport circle, across it opens his shop; from any
 */
/* other side he talks of the mining in the ruins. */
void scContigoTalkToArmorMerchant(void) {
	if(fdLeaderIsFacingNorth()) {
		mnShopRun(SHOP_CONTIGO_ARMOR, ACTOR_ARMOR_MERCHANT);
	}
	else {
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		if(syGameFlagGet(FLAG_SET_OUT_WITH_WINGS))
			/* "Now that the wings are finished, I doubt anyone will be buying
			 * any..." */
			fdEventSetMessageId((s32)MsgContigoNowThatTheWingsAre);
		else
			/* "Mining materials in the ruins is dangerous work, so my armor
			 * is..." */
			fdEventSetMessageId((s32)MsgContigoMiningMaterialsInTheRuins);
		fdEventShowMessageAndWait(ACTOR_ARMOR_MERCHANT, 0);
		fdEventEnd();
	}
}

/* Talking to the innkeeper across the counter of the inn, right of the Lucky */
/* Wheels tent, offers a night's rest; from any other side the innkeeper talks
 */
/* of the visitors and the wings. */
void scContigoTalkToInnkeeper(void) {
	if(fdLeaderIsFacingNorth()) {
		mnInnCheckIn(CONTIGO_INN, ACTOR_INNKEEPER);
	}
	else {
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		if(syGameFlagGet(FLAG_SET_OUT_WITH_WINGS))
			/* "All of our work and all of our celebration are finally at an
			 * end...." */
			fdEventSetMessageId((s32)MsgContigoAllOfOurWorkAnd);
		else if(syGameFlagGet(FLAG_JUPITER_BEACON_LIT))
			/* "You have to hurry! I think the ship from the geoglyphs is going
			 * to..." */
			fdEventSetMessageId((s32)MsgContigoYouHaveToHurryI);
		else
			/* "We've had so many people coming to Contigo lately that we're
			 * full..." */
			fdEventSetMessageId((s32)MsgContigoWeveHadSoManyPeople);
		fdEventShowMessageAndWait(ACTOR_INNKEEPER, 0);
		fdEventEnd();
	}
}

/* Talking to the healer across his counter, at the head of the red carpet in */
/* the Sanctum north-east of the Teleport circle, offers the Sanctum's */
/* healing; from any other side he talks of the miracle of the wings. */
void scContigoTalkToHealer(void) {
	if(fdLeaderIsFacingNorth()) {
		mnSanctumRun(ACTOR_HEALER);
	}
	else {
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		if(syGameFlagGet(FLAG_SET_OUT_WITH_WINGS))
			/* "It was a miracle that made all of the legends of the Anemos
			 * come..." */
			fdEventSetMessageId((s32)MsgContigoItWasAMiracleThat);
		else if(syGameFlagGet(FLAG_JUPITER_BEACON_LIT))
			/* "A flying ship... Surely, it must be a miracle. I'll have to
			 * study..." */
			fdEventSetMessageId((s32)MsgContigoAFlyingShipSurelyIt);
		else
			/* "The real miracle will be if these wings actually make a boat
			 * fly...." */
			fdEventSetMessageId((s32)MsgContigoTheRealMiracleWillBe);
		fdEventShowMessageAndWait(ACTOR_HEALER, 0);
		fdEventEnd();
	}
}

/* In the Super Lucky Dice tent, one of the players asks Felix whether he has */
/* ever played Superdice before, and answers his yes or no. */
void scContigoAskAboutSuperDice(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId((s32)MsgContigoSoTellMePalHave);
	/* "So, tell me, pal... Have you ever played Superdice before?" */
	fdEventAskYesNoAndShowResponse(ACTOR_MAN_WARY_OF_UNLUCKY_PLAYERS, 0);
	fdEventEnd();
}

/* Felix reads the rules sign in the Super Lucky Dice tent: four dice thrown at
 */
/* once, what matching pairs and fours win, and the hi-lo game after a win. */
void scContigoReadSuperLuckyDiceRules(void) {
	s32 msg;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	msg = (s32)MsgContigoSuperLuckyDiceTheRules;
	/* "Super Lucky Dice The Rules of the Game." */
	mnFieldMessageShow(msg, 1);
	/* "Throw all four dice onto the table in a single toss." */
	mnFieldMessageShow(msg + 1, 1);
	/* "If two of the dice match, it's a push, and you win your money back." */
	mnFieldMessageShow(msg + 2, 1);
	/* "If three of the dice match, you win back twice the bet you placed." */
	mnFieldMessageShow(msg + 3, 1);
	/* "If you get two pairs of matching dice, you win three times the bet
	 * you..." */
	mnFieldMessageShow(msg + 4, 1);
	/* "If all four dice match, it's a perfect toss! You win eight times
	 * your..." */
	mnFieldMessageShow(msg + 5, 1);
	/* "If you get one pair, two pair, three of a kind, or a perfect four,
	 * you..." */
	mnFieldMessageShow(msg + 6, 1);
	/* "You have to guess whether the next roll of the dice will be more or..."
	 */
	mnFieldMessageShow(msg + 7, 1);
	/* "If you win the hi-lo game, you double your winnings! You can do this..."
	 */
	mnFieldMessageShow(msg + 8, 1);
	fdEventEnd();
}

/* Felix reads the first of the two signs in the Lucky Dice tent: throw the */
/* dice on the table and try to get the same numbers. */
void scContigoReadLuckyDiceRules(void) {
	s32 msg;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	msg = (s32)MsgContigoLuckyDice;
	/* "Lucky Dice" */
	mnFieldMessageShow(msg, 1);
	/* "Throw the dice on the table and try to get the same numbers." */
	mnFieldMessageShow(msg + 1, 1);
	/* "Prizes are based on the numbers rolled and where each die lands." */
	mnFieldMessageShow(msg + 2, 1);
	/* "The more numbers that match, the more you win." */
	mnFieldMessageShow(msg + 3, 1);
	fdEventEnd();
}

/* Felix reads the second sign in the Lucky Dice tent: what pairs, triples and
 */
/* four of a kind win. */
void scContigoReadLuckyDicePairs(void) {
	s32 msg;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	msg = (s32)MsgContigoLuckyDiceB;
	/* "Lucky Dice" */
	mnFieldMessageShow(msg, 1);
	/* "If two numbers match on either the dice or the table..." */
	mnFieldMessageShow(msg + 1, 1);
	/* "it's called a pair, and you don't win or lose anything." */
	mnFieldMessageShow(msg + 2, 1);
	/* "Two pair doubles your money. Triple matches triple your money." */
	mnFieldMessageShow(msg + 3, 1);
	/* "Four of a kind is perfect, and you win five times your bet." */
	mnFieldMessageShow(msg + 4, 1);
	/* "Plus, if your dice come up perfect on the Perfect Bonus spot on the..."
	 */
	mnFieldMessageShow(msg + 5, 1);
	fdEventEnd();
}

/* Talking to the young woman by the slot machine in the Lucky Wheels tent */
/* offers a game for game tickets (scContigoOfferLuckyWheels). */
void scContigoTalkToWheelsAttendant(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	scContigoOfferLuckyWheels(0);
	fdEventEnd();
}

/* Felix reads the rules sign in the Lucky Wheels tent: five pulls of the */
/* lever, stars to hold a wheel, and more lines for more tickets. */
void scContigoReadLuckyWheelsRules(void) {
	s32 msg;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	msg = (s32)MsgContigoLuckyWheelsRules;
	/* "Lucky Wheels Rules" */
	mnFieldMessageShow(msg, 1);
	/* "Pull the lever and try to line up matching icons to win big." */
	mnFieldMessageShow(msg + 1, 1);
	/* "You have only 5 chances to pull the lever." */
	mnFieldMessageShow(msg + 2, 1);
	/* "Press the star to keep a wheel from spinning and match other wheels." */
	mnFieldMessageShow(msg + 3, 1);
	/* "You get 1 line for 1 game ticket, 3 lines for 2 tickets," */
	mnFieldMessageShow(msg + 4, 1);
	/* "5 lines for 3 tickets, and 7 lines for 4 tickets. Use all your
	 * tickets..." */
	mnFieldMessageShow(msg + 5, 1);
	fdEventEnd();
}

/* Felix reads the prizes sign in the Lucky Wheels tent: shirts, rings and */
/* boots win their kind, stars and hearts win items, and the Moon is wild. */
void scContigoReadLuckyWheelsPrizes(void) {
	s32 msg;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	msg = (s32)MsgContigoLuckyWheelsPrizes;
	/* "Lucky Wheels Prizes" */
	mnFieldMessageShow(msg, 1);
	/* "Prizes are determined by which icons you match." */
	mnFieldMessageShow(msg + 1, 1);
	/* "Shirts win undershirts, Rings win rings, Boots win boots," */
	mnFieldMessageShow(msg + 2, 1);
	/* "Stars and Hearts win you items..." */
	mnFieldMessageShow(msg + 3, 1);
	/* The Spanish sign ends after four messages. */
#if LANGUAGE != LANGUAGE_SPANISH
	/* "And Moons are wild°they can win you anything." */
	mnFieldMessageShow(msg + 4, 1);
#endif
	fdEventEnd();
}

/* On Felix's first visit, the man in a long robe between the Sanctum's middle
 */
/* and east steps asks whether he has taken a good look at the Anemos */
/* geoglyph, and answers his yes or no. */
void scContigoAskAboutGeoglyph(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId((s32)MsgContigoHaveYouTakenAGood);
	/* "Have you taken a good look at the Anemos geoglyph?" */
	fdEventAskYesNoAndShowResponse(ACTOR_MAN_ON_THE_GEOGLYPH, 0);
	fdEventEnd();
}

/* Talking to the boy walking about the middle of the village with a girl: he */
/* talks of the geoglyphs and of the crater where the city of the Anemos is */
/* said to have stood, and Mind Read hears that he will not believe it flew. */
/* He turns to Felix to talk, but not for Mind Read, and turns back after. */
void scContigoTalkToDoubtingBoy(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR *actor = fdActorGetById(ACTOR_BOY_DOUBTING_THE_LEGENDS);
	s16 angle = actor->facing;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	actor->sceneByte = 1;
	if(state->psynergy == 0) {
		fdActorFaceActorById(ACTOR_BOY_DOUBTING_THE_LEGENDS,
							 MainRoutineStateWork.leaderActorId, 0);
		if(syGameFlagGet(FLAG_SET_OUT_WITH_WINGS) != 0)
			/* "I've seen a lot of weird things lately... Maybe I ought to
			 * listen..." */
			fdEventSetMessageId((s32)MsgContigoIveSeenALotOf);
		else if(syGameFlagGet(FLAG_JUPITER_BEACON_LIT) != 0)
			/* "That crater behind the village is supposed to be where the city
			 * of..." */
			fdEventSetMessageId((s32)MsgContigoThatCraterBehindTheVillage);
		else
			/* "If you want to get a look at the Anemos geoglyphs, you ought to
			 * go..." */
			fdEventSetMessageId((s32)MsgContigoIfYouWantToGet);
	}
	else {
		if(syGameFlagGet(FLAG_SET_OUT_WITH_WINGS) != 0)
			/* "Wait... The city of Anemos flew up into the sky? All right,
			 * that's..." */
			fdEventSetMessageId((s32)MsgContigoWaitTheCityOfAnemos);
		else if(syGameFlagGet(FLAG_JUPITER_BEACON_LIT) != 0)
			/* "Cities flying up into the sky? I don't buy that for a second!"
			 */
			fdEventSetMessageId((s32)MsgContigoCitiesFlyingUpIntoThe);
		else
			/* "The Anemos geoglyphs are the most amazing carvings I've ever
			 * seen..." */
			fdEventSetMessageId((s32)MsgContigoTheAnemosGeoglyphsAreThe);
	}
	fdEventShowMessageAndWait(ACTOR_BOY_DOUBTING_THE_LEGENDS, 0);
	if(state->psynergy == 0)
		actor->facing = angle;
	actor->sceneByte = 0;
	fdEventEnd();
}

/* Talking to the girl walking about the middle of the village with the boy: */
/* she talks of the wings the village built and, once they are gone, of seeing
 */
/* the ship take off; Mind Read hears her thoughts on the same. She turns to */
/* Felix to talk, but not for Mind Read, and turns back after. */
void scContigoTalkAboutTheShip(void) {
	FIELD_STATE *event = Ram_HeapSlots->fieldState;
	ACTOR *actor = fdActorGetById(ACTOR_GIRL_ON_THE_SHIP);
	s16 angle = actor->facing;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	actor->sceneByte = 1;
	if(event->psynergy == 0) {
		fdActorFaceActorById(ACTOR_GIRL_ON_THE_SHIP,
							 MainRoutineStateWork.leaderActorId, 0);
		if(syGameFlagGet(FLAG_SET_OUT_WITH_WINGS) != 0)
			/* "It wasn't what I expected, but seeing the ship take off was
			 * still..." */
			fdEventSetMessageId((s32)MsgContigoItWasntWhatIExpected);
		else
			/* "Our people have labored for some time to build the Wings of
			 * the..." */
			fdEventSetMessageId((s32)MsgContigoOurPeopleHaveLaboredFor);
	}
	else {
		if(syGameFlagGet(FLAG_SET_OUT_WITH_WINGS) != 0)
			/* "All of those legends did nothing but build up people's..." */
			fdEventSetMessageId((s32)MsgContigoAllOfThoseLegendsDid);
		else
			/* "We built the Wings of Anemos out of materials we found in
			 * the..." */
			fdEventSetMessageId((s32)MsgContigoWeBuiltTheWingsOf);
	}
	fdEventShowMessageAndWait(ACTOR_GIRL_ON_THE_SHIP, 0);
	if(event->psynergy == 0)
		actor->facing = angle;
	actor->sceneByte = 0;
	fdEventEnd();
}

void fdTeleportPlaySparks(void);

/* Teleport cast on the carved stone circle in the middle of the village */
/* (cells (16, 50) and (17, 50)): the village turns grey, the circle shimmers */
/* and sparks of coloured light rise, and the party arrives on a circle like */
/* it in the Anemos Inner Sanctum. */
void scContigoTeleportOut(void) {
	scTeleportStartShimmer();
	fdTeleportPlaySparks();

	MainRoutineStateWork.teleportState = TELEPORT_ARRIVING;
	fdEventSetDestination(SCENE_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST,
						  ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST_FROM_CONTIGO);
	fdEventWait(40);
	scTeleportStopShimmer();
}

/* Cyclone scatters the clump of weeds just west of the item shop, the west of
 */
/* the two houses south of the Teleport circle (cell (8, 56)), leaving bare */
/* ground. Each clump's event copies bare ground from a cell nearby over the */
/* clump's cells that Cyclone's whirl touched. Where the ground is raised, as */
/* here, a clump is drawn some rows north of the cell it answers on. */
void scContigoScatterWeedsWestOfItemShop(void) {
	syFieldTileCopiesCopyTouchedCells(3, 50, 1, 1, 8, 54, 0);
}

/* Cyclone scatters the clump of weeds behind the inn, beside the tree stump */
/* where Shine is stuck (cells (2, 45) and (3, 45)), leaving bare ground. */
void scContigoScatterWeedsBehindInn(void) {
	syFieldTileCopiesCopyTouchedCells(4, 43, 2, 1, 2, 43, 0);
}

/* The scene's own name for fdTileCopiesCopyTouchedCells, which the Bramble */
/* Seed's clump calls. */
static __inline__ void scContigoCopyTouchedCells(s32 sourceX, s32 sourceZ,
												 s32 width, s32 height,
												 s32 windowX, s32 windowZ,
												 s32 layer) {
	fdTileCopiesCopyTouchedCells(sourceX, sourceZ, width, height, windowX,
								 windowZ, layer);
}

/* Cyclone scatters the clump of weeds beside the east wall of the house at */
/* the top of the hill (cell (17, 41)), and unless it was taken the Bramble */
/* Seed lies uncovered there. If Felix stands on that very spot, he hops a */
/* step north off it. */
void scContigoScatterWeedsOverBrambleSeed(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

	scContigoCopyTouchedCells(16, 37, 1, 1, 17, 37, 0);
	if(syGameFlagGet(FLAG_CONTIGO_BRAMBLE_SEED_TAKEN) == 0) {
		fdActorSetPos(ACTOR_BRAMBLE_SEED, 280, 664);
		if(FIELD_CELL(leader->x.fixed) == 17 &&
		   FIELD_CELL(leader->z.fixed) == 41) {
			fdEventStart();
			fdEventEndLastingPsynergy(0);
			fdEffectSetAttached(MainRoutineStateWork.leaderActorId,
								ATTACHED_EFFECT_IN_FRONT |
									ATTACHED_EFFECT_ONCE);
			fdActorSetDestinationOffset(MainRoutineStateWork.leaderActorId, 0,
										-16);
			leader->velY = toFIXED(6);
			fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
			fdEventEnd();
		}
	}
}

/* Cyclone scatters the clump of weeds against the west wall of the weapon and
 */
/* armor shop, east of the Teleport circle (cells (23, 49) and (23, 50)), */
/* leaving bare ground. */
void scContigoScatterWeedsWestOfWeaponShop(void) {
	syFieldTileCopiesCopyTouchedCells(29, 35, 1, 2, 23, 47, 0);
}

/* Cyclone scatters the weeds just east of the village gate's east post (cells
 */
/* (29, 59) and (30, 58) to (30, 60)), leaving bare ground. */
void scContigoScatterWeedsByGate(void) {
	syFieldTileCopiesCopyTouchedCells(32, 55, 2, 3, 29, 58, 0);
}

/* Cyclone scatters the clump of weeds below the Lucky Wheels tent, at the */
/* edge of the slope (cell (3, 58)), leaving bare ground. */
void scContigoScatterWeedsBelowLuckyWheels(void) {
	syFieldTileCopiesCopyTouchedCells(3, 60, 1, 1, 3, 56, 0);
}

/* Cyclone scatters the western of the two clumps of weeds on top of the cliff
 */
/* south of the item shop (cells (7, 60) and (8, 60)), leaving bare ground. */
void scContigoScatterWeedsOnCliffWest(void) {
	syFieldTileCopiesCopyTouchedCells(7, 62, 2, 1, 7, 58, 0);
}

/* Cyclone scatters the eastern of the two clumps of weeds on top of the cliff
 */
/* south of the item shop (cells (10, 61) and (11, 61)), leaving bare ground. */
void scContigoScatterWeedsOnCliffEast(void) {
	syFieldTileCopiesCopyTouchedCells(10, 63, 2, 1, 10, 59, 0);
}

/* Cyclone scatters the top row of the big weed patch east of the village gate
 */
/* (cells (40, 57) to (42, 57)), leaving bare ground. The patch's five rows */
/* each have an event, and they and the weeds by the gate take their bare */
/* ground from a cell between the gate and the patch. */
void scContigoScatterWeedPatchTopRow(void) {
	syFieldTileCopiesCopyTouchedCells(32, 55, 3, 1, 40, 57, 0);
}

/* Cyclone scatters the second row from the top of the big weed patch east of */
/* the village gate (cells (39, 58) to (43, 58)), leaving bare ground. */
void scContigoScatterWeedPatchSecondRow(void) {
	syFieldTileCopiesCopyTouchedCells(32, 55, 5, 1, 39, 58, 0);
}

/* Cyclone scatters the middle row of the big weed patch east of the village */
/* gate, either side of the open gap where Salt hides (cells (39, 59), */
/* (40, 59), (42, 59) and (43, 59)), leaving bare ground. */
void scContigoScatterWeedPatchMiddleRow(void) {
	syFieldTileCopiesCopyTouchedCells(32, 55, 5, 1, 39, 59, 0);
}

/* Cyclone scatters the second row from the bottom of the big weed patch east */
/* of the village gate (cells (39, 60) to (43, 60)), leaving bare ground. */
void scContigoScatterWeedPatchFourthRow(void) {
	syFieldTileCopiesCopyTouchedCells(32, 55, 5, 1, 39, 60, 0);
}

/* Cyclone scatters the bottom row of the big weed patch east of the village */
/* gate (cells (40, 61) to (42, 61)), leaving bare ground. */
void scContigoScatterWeedPatchBottomRow(void) {
	syFieldTileCopiesCopyTouchedCells(32, 55, 3, 1, 40, 61, 0);
}

/* Nothing visible happens when Felix walks south from the middle of the */
/* village into its south part, on one of the three ways in: west of the item */
/* shop, between it and the house beside it, or east of that house (cells */
/* (5, 55) to (8, 55), (14, 55) and (18, 57) to (20, 57)). From then on the */
/* view follows him east past the village gate to the big weed patch, where */
/* it stopped just past the gate before. FLAG_WHOLE_VILLAGE_IN_VIEW marks the */
/* wider view. */
void scContigoWidenCameraBounds(void) {
	fdSetCameraBounds(0, toFIXED(512), toFIXED(880), toFIXED(1024));
	syGameFlagSet(FLAG_WHOLE_VILLAGE_IN_VIEW);
}

/* Nothing visible happens when Felix walks back north from the village's */
/* south part into its middle, on one of the three ways (cells (4, 54) to */
/* (8, 54), (14, 54) and (18, 56) to (20, 56)); the view stops at the village */
/* gate again, and FLAG_WHOLE_VILLAGE_IN_VIEW is cleared. */
void scContigoNarrowCameraBounds(void) {
	fdSetCameraBounds(0, toFIXED(512), toFIXED(512), toFIXED(1024));
	syGameFlagClear(FLAG_WHOLE_VILLAGE_IN_VIEW);
}

/* The trigger scContigoScoopForSalt raises once Salt has popped up, which runs
 */
/* scContigoSaltJoinsParty. */
typedef enum CONTIGO_RAISED_TRIGGER {
	RAISED_SALT_FOUND = 200
} CONTIGO_RAISED_TRIGGER;

/* Scoop cast in the open gap in the middle of the big weed patch east of the */
/* village gate (cell (41, 59)): until the party has Salt, the Venus Djinni */
/* pops up there, turns to Felix and joins the party. */
void scContigoScoopForSalt(void) {
	if(!syGameFlagGet(FLAG_DJINNI(DJINNI_SALT))) {
		FIELD_STATE *state = Ram_HeapSlots->fieldState;

		fdEventStart();
		fdEventEndLastingPsynergy(1);
		fdActorSetPos(ACTOR_SALT, 664, 952);
		sySynch(1);
		fdActorFaceActorById(ACTOR_SALT, MainRoutineStateWork.leaderActorId, 0);
		state->raisedTrigger = RAISED_SALT_FOUND;
		fdEventEnd();
	}
}

/* Felix casts Reveal anywhere at the geoglyph or the Sanctum's front: the */
/* carved wall at the head of the Sanctum's main steps shows as a dark open */
/* doorway, its middle door, and while Reveal lasts walking north into it */
/* (cells (48, 19) and (49, 19)) leads inside, on the way to the Dragon Skin's
 */
/* chest. The open door is the picture the unused */
/* scContigoEnterSanctumMiddleDoor would open it with; the doorway's exit cells
 */
/* are kept out of reach at the top of the map, (48, 1), and copied into place.
 */
void scContigoRevealMiddleDoor(void) {
	fdMapGridCopyCellsTo(27, 4, 48, 12, 1, 2);
	fdMapGridCopyCellTilesIndicesRect(27, 6, 49, 12, 1, 2);
	fdMapGridCopyCellAttributes(48, 1, 2, 1, 48, 19);
}

/* As Reveal fades in front of the Sanctum, its middle door shows shut again */
/* and walking into it leads nowhere. */
void scContigoHideMiddleDoor(void) {
	fdMapGridCopyCellsTo(27, 8, 48, 12, 1, 2);
	fdMapGridCopyCellTilesIndicesRect(27, 10, 49, 12, 1, 2);
	fdMapGridCopyCellAttributes(48, 0, 2, 1, 48, 19);
}

/* Force cast on the tree stump behind the inn, where Shine is stuck: the */
/* ground shakes with a thump, a ring of dust bursts out of the stump, and */
/* Shine leaps out and lands just north-west of it, Felix hopping aside if it */
/* lands on him. Force comes only with the Orb of Force, carried over from The
 */
/* Broken Seal in a linked game. */
void scContigoForceFreesDjinni(void) {
	ACTOR *actor = fdActorGetById(ACTOR_SHINE);
	ACTOR *leader;
	EFFECT_OPTIONS options;
	FIXED velocity[3];
	s32 angle;
	u32 i;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorSetPos(ACTOR_SHINE, 56, 698);
	sySynch(1);
	options.pri = 2;
	sySndPlay(SE_THUMP);
	fdMapGridSetShake(1, 2, 1);
	for(i = 0; i <= DUST_RING_PUFFS; i++) {
		angle = i * (TRIG_TURN / DUST_RING_PUFFS);
		velocity[0] = syCos(angle);
		velocity[1] = 0;
		velocity[2] = sySin(angle);
		velocity[0] += velocity[0] / 3;
		scFieldEffectSpawn(actor->x.fixed, actor->y.fixed, actor->z.fixed,
						   velocity[0], velocity[1] + (toFIXED(1) / 10),
						   velocity[2], EFFECT_USE_PRIORITY, &options);
	}
	fdActorSetSprCol(fdActorGetById(ACTOR_SHINE), SPRITE_COLOR_NORMAL);
	fdActorSetSprFlags(fdActorGetById(ACTOR_SHINE), 1);
	fdActorSetSpeed(ACTOR_SHINE, 3);
	actor->velY = toFIXED(12);
	fdActorMoveToAndWait(ACTOR_SHINE, 40, 680);
	fdEventWait(10);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(FIELD_CELL(actor->x.fixed) == FIELD_CELL(leader->x.fixed) &&
	   FIELD_CELL(actor->z.fixed) == FIELD_CELL(leader->z.fixed)) {
		fdActorJump(MainRoutineStateWork.leaderActorId, 6, 0);
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 2);
		fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 68, 680);
	}
	fdMapGridSetShake(0, 0, 0);
	fdActorTurnToFaceEachOther(MainRoutineStateWork.leaderActorId, ACTOR_SHINE,
							   0);
	fdEventWait(10);
	syGameFlagSet(FLAG_SHINE_FREED);
	fdEventEnd();
}

/* Once Jupiter Lighthouse's beacon is lit, while the villagers are away */
/* transporting the wings: the doors of the item shop, the house beside it, */
/* the weapon and armor shop and the baby's house north of it show shut, and */
/* trying one says it is locked (scShowEntranceLocked). */
void scContigoLockDoors(void) {
	fdMapGridCopyCellsTo(0, 1, 26, 42, 1, 2);
	fdMapGridCopyCellTilesIndicesRect(0, 1, 26, 48, 1, 2);
	fdMapGridCopyCellTilesIndicesRect(0, 1, 11, 54, 1, 2);
	fdMapGridCopyCellTilesIndicesRect(0, 1, 16, 54, 1, 2);
	fdMapCopyCellAttributeRect(0, 32, 1, 1, 26, 45);
	fdMapCopyCellAttributeRect(0, 32, 1, 1, 26, 51);
	fdMapCopyCellAttributeRect(0, 32, 1, 1, 11, 57);
	fdMapCopyCellAttributeRect(0, 32, 1, 1, 16, 57);
}

/* Lays a frame or feather of the wings flat on the ground, drawn behind. */
void scContigoPlaceWingPart(s32 id) {
	ACTOR *actor = fdActorGetById(id);

	fdActorSetSprPriority(id, 3);
	actor->collisionFlg = ACTOR_NOT_TARGETED;
	actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	actor->motionFlg = 0;
	actor->y.fixed = toFIXED(64);
}

/* Lays out the wings' parts by the geoglyph: each frame and feather shows */
/* its own piece and lies flat, and the loose parts block the way. */
void scContigoSetUpWingParts(void) {
	fdActorSetAnimationById(ACTOR_WING_FRAME_A, 0);
	fdActorSetAnimationById(ACTOR_WING_FRAME_B, 1);
	fdActorSetAnimationById(ACTOR_WING_FRAME_C, 2);
	fdActorSetAnimationById(ACTOR_METAL_FEATHER_A, 0);
	fdActorSetAnimationById(ACTOR_METAL_FEATHER_B, 1);
	fdActorSetAnimationById(ACTOR_METAL_FEATHER_C, 2);
	fdActorSetAnimationById(ACTOR_METAL_FEATHER_D, 3);
	fdActorSetAnimationById(ACTOR_METAL_FEATHER_E, 4);
	scContigoPlaceWingPart(ACTOR_WING_FRAME_A);
	scContigoPlaceWingPart(ACTOR_WING_FRAME_B);
	scContigoPlaceWingPart(ACTOR_WING_FRAME_C);
	scContigoPlaceWingPart(ACTOR_METAL_FEATHER_A);
	scContigoPlaceWingPart(ACTOR_METAL_FEATHER_B);
	scContigoPlaceWingPart(ACTOR_METAL_FEATHER_C);
	scContigoPlaceWingPart(ACTOR_METAL_FEATHER_D);
	scContigoPlaceWingPart(ACTOR_METAL_FEATHER_E);
	fdActorSetSprFlags(fdActorGetById(ACTOR_LOOSE_PART_A), 0);
	fdActorSetSprFlags(fdActorGetById(ACTOR_LOOSE_PART_B), 0);
	fdActorSetSprFlags(fdActorGetById(ACTOR_LOOSE_PART_C), 0);
	fdActorSetSprFlags(fdActorGetById(ACTOR_LOOSE_PART_D), 0);
	fdActorSetSprFlags(fdActorGetById(ACTOR_LOOSE_PART_E), 0);
	fdActorSetSprFlags(fdActorGetById(ACTOR_LOOSE_PART_F), 0);
	fdActorSetSprFlags(fdActorGetById(ACTOR_LOOSE_PART_G), 0);
	fdActorGetById(ACTOR_LOOSE_PART_A)->collisionFlg |= ACTOR_NOT_TARGETED;
	fdActorGetById(ACTOR_LOOSE_PART_B)->collisionFlg |= ACTOR_NOT_TARGETED;
	fdActorGetById(ACTOR_LOOSE_PART_C)->collisionFlg |= ACTOR_NOT_TARGETED;
	fdActorGetById(ACTOR_LOOSE_PART_D)->collisionFlg |= ACTOR_NOT_TARGETED;
	fdActorGetById(ACTOR_LOOSE_PART_E)->collisionFlg |= ACTOR_NOT_TARGETED;
	fdActorGetById(ACTOR_LOOSE_PART_F)->collisionFlg |= ACTOR_NOT_TARGETED;
	fdActorGetById(ACTOR_LOOSE_PART_G)->collisionFlg |= ACTOR_NOT_TARGETED;
	fdActorSetAnimationById(ACTOR_LOOSE_PART_A, 0);
	fdActorSetAnimationById(ACTOR_LOOSE_PART_F, 0);
	fdActorSetAnimationById(ACTOR_LOOSE_PART_G, 0);
}

/* The Lucky Wheels attendant asks whether Felix will use his game tickets, */
/* warns when the party has little room for prizes, and sends it to the slot */
/* machine; `spoken` is nonzero as the party comes back from a game, when she */
/* asks only whether to keep playing. */
void scContigoOfferLuckyWheels(s32 spoken) {
	s32 tickets = gmPartyInventoryCountItem(ITEM_GAME_TICKET);
	s32 room;

	fdActorTableSnapshot();
	if(spoken == 0) {
		s32 message = (s32)MsgContigoStepRightUpAndTry;

		fdEventSetMessageId(message);
		/* "Step right up and try your hand at lucky wheels!" */
		fdEventShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
		if(tickets == 0) {
			/* "You gotta have game tickets to play." */
			fdEventShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
			return;
		}
		fdEventSetMessageId(message + 2);
		mnMessageArgumentPush(tickets, MESSAGE_ARGUMENT_NUMBER);
		/* "You have ... game ticket, Felix. Would you like to use these
		 * tickets?" */
		fdMessageShowAtActor(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
		if(fdEventAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId,
											 0) == 1) {
			/* "The rules are listed on the sign. Take a look, take a look." */
			fdEventShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
			return;
		}
		room = gmPartyInventoryCountFreeSlots();
		if(room == 0) {
			fdEventSetMessageId(message + 4);
			/* "You can't carry any more items. Are you sure you want to play?"
			 */
			fdMessageShowAtActor(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
		}
		else if(room <= 6) {
			fdEventSetMessageId(message + 5);
			/* "If you win anything, you might not be able to take it with
			 * you..." */
			fdMessageShowAtActor(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
		}
		if(room <= 6 && fdEventAskYesNoWithActorAnimation(
							MainRoutineStateWork.leaderActorId, 0) == 1) {
			fdEventSetMessageId((s32)MsgContigoThenGetRidOfSome);
			/* "Then get rid of some items and come again." */
			fdEventShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
			return;
		}
	}
	else {
		if(tickets == 0) {
			fdEventSetMessageId((s32)MsgContigoTooBadComeBackAfter);
			/* "Too bad... Come back after you get some tickets." */
			fdEventShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
			return;
		}
		fdEventSetMessageId((s32)MsgContigoTooBadWouldYouLike);
		/* "Too bad... Would you like to use your tickets to keep playing?" */
		fdMessageShowAtActor(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
		if(fdEventAskYesNoWithActorAnimation(0, 0) == 1) {
			fdEventSetMessageId((s32)MsgContigoComeAgain);
			/* "Come again!" */
			fdEventShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
			return;
		}
	}
	fdEventSetMessageId((s32)MsgContigoStepRightUpAndEnjoy);
	/* "Step right up and enjoy yourself." */
	fdEventShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
	syGameFlagSet(FLAG_KEEP_PARTY_POSITION);
	syGameFlagSet(FLAG_LEFT_FOR_A_GAME);
	sySynch(1);
	fdEventLeaveFor(SEQUENCE_SLOTS, 0);
	fdEventSetAfterBattleDestination(SCENE_CONTIGO_LUCKY_DICE,
									 ENTRANCE_FROM_LUCKY_WHEELS);
}

/* The prize for an icon matched on the Lucky Wheels: one of the icon's three,
 */
/* never the one it gave last; the Moon picks an icon at random, and a negative
 */
/* icon gives nothing. */
s32 scContigoSelectLuckyWheelsPrize(s32 icon) {
	MAIN_ROUTINE_STATE *state;
	s32 cursor;
	s32 variant;
	s32 index;

	if(icon < 0)
		return 0;
	if(icon == WHEELS_ICON_MOON)
		icon = (s32)(syRandomBelow(WHEELS_PRIZE_ICONS));
	state = &MainRoutineStateWork;
	cursor = state->wheelsPrizeCursors[icon];
	variant = (cursor + (s32)(syRandomBelow(2)) + 4) % WHEELS_PRIZES_PER_ICON;
	state->wheelsPrizeCursors[icon] = variant;
	index = icon * 2;

	return Contigo_WheelsPrizes[index + icon + variant];
}

/* The dice games cost 10 coins for each level of the party's average level; */
/* a win of 5,000 coins or more, or of 20,000, is greeted with a bigger */
/* jingle. */
enum {
	LUCKY_DICE_COINS_PER_LEVEL = 10,
	LUCKY_DICE_MEDIUM_WIN = 5000,
	LUCKY_DICE_LARGE_WIN = 20000
};

/* Talking to the young man running the Lucky Dice tent offers a game, */
/* at 10 coins for each level of the party's average level: "Hey, buddy, */
/* wanna try yer luck at the dice?" */
void scContigoOfferLuckyDice(void) {
	u32 price = gmPartyGetAverageLevel() * LUCKY_DICE_COINS_PER_LEVEL;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	if(MainRoutineStateWork.coins < price) {
		fdEventSetMessageId((s32)MsgContigoHeyYaDontHaveEnough);
		/* "Hey! Ya don't have enough coins! Ya gotta pay to play." */
		fdMessageShowAtActor(ACTOR_LUCKY_DICE_ATTENDANT, 0);
		return;
	}
	fdEventSetMessageId((s32)MsgContigoHeyBuddyWannaTryYer);
	mnMessageArgumentPush(price, MESSAGE_ARGUMENT_NUMBER);
	/* "Hey, buddy, wanna try yer luck at the dice? It's coin to play." Yes:
	 * "OK! */
	/* Then take the dice." No: "No? Oh, well. Who's next?" */
	fdMessageShowAtActor(ACTOR_LUCKY_DICE_ATTENDANT, 0);
	if(fdEventAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId,
										 0) == 0) {
		*(s32 *)SceneStateWork = MainRoutineStateWork.coins;
		/* FAKEMATCH(dead): tests of the coins, never above COIN_MAX, around a
		 * store to price, which nothing reads again. */
		if(MainRoutineStateWork.coins > COIN_MAX) {
			if(MainRoutineStateWork.coins != 0)
				price = 0;
		}
		fdActorTableSnapshot();
		/* "OK! Then take the dice." */
		fdEventShowMessageAndWait(ACTOR_LUCKY_DICE_ATTENDANT, 0);
		syGameFlagSet(FLAG_KEEP_PARTY_POSITION);
		syGameFlagSet(FLAG_LEFT_FOR_A_GAME);
		sySynch(1);
		fdEventLeaveFor(SEQUENCE_DICE, 0);
		fdEventSetAfterBattleDestination(SCENE_CONTIGO_LUCKY_DICE,
										 ENTRANCE_FROM_LUCKY_DICE);
	}
	else {
		FieldStateWork->msg++;
		/* "No? Oh, well. Who's next?" */
		fdEventShowMessageAndWait(ACTOR_LUCKY_DICE_ATTENDANT, 0);
	}
	fdEventEnd();
}

/* Talking to the young man running the Super Lucky Dice tent offers a game at
 */
/* the same price as Lucky Dice, which his offer does not say: */
/* "Welcome, welcome, step right up! Care for a round of Super Lucky Dice?" */
void scContigoOfferSuperLuckyDice(void) {
	u32 price = gmPartyGetAverageLevel() * LUCKY_DICE_COINS_PER_LEVEL;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	if(MainRoutineStateWork.coins < price) {
		/* Shown at the Lucky Dice attendant, who stands in the next tent: the
		 * box */
		/* opens at the top of the screen as his welcome does, a little further
		 */
		/* right. */
		fdEventSetMessageId((s32)MsgContigoSorryPalButYouveGotta);
		/* "Sorry, pal, but you've gotta have more coins to play." */
		fdMessageShowAtActor(ACTOR_LUCKY_DICE_ATTENDANT, 0);
		return;
	}
	fdEventSetMessageId((s32)MsgContigoWelcomeWelcomeStepRightUp);
	/* "Welcome, welcome, step right up! Care for a round of Super Lucky Dice?"
	 */
	/* Yes: "OK, then let 'em roll!" No: "Oh really? That's a shame!" */
	fdMessageShowAtActor(ACTOR_SUPER_LUCKY_DICE_ATTENDANT, 0);
	if(fdEventAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId,
										 0) == 0) {
		*(s32 *)SceneStateWork = MainRoutineStateWork.coins;
		/* FAKEMATCH(dead): tests of the coins, never above COIN_MAX, around a
		 * store to price, which nothing reads again. */
		if(MainRoutineStateWork.coins > COIN_MAX) {
			if(MainRoutineStateWork.coins != 0)
				price = 0;
		}
		fdActorTableSnapshot();
		/* "OK, then let 'em roll!" */
		fdEventShowMessageAndWait(ACTOR_SUPER_LUCKY_DICE_ATTENDANT, 0);
		syGameFlagSet(FLAG_KEEP_PARTY_POSITION);
		syGameFlagSet(FLAG_LEFT_FOR_A_GAME);
		sySynch(1);
		fdEventLeaveFor(SEQUENCE_SUPER_DICE, 0);
		fdEventSetAfterBattleDestination(SCENE_CONTIGO_LUCKY_DICE,
										 ENTRANCE_FROM_SUPER_LUCKY_DICE);
	}
	else {
		FieldStateWork->msg++;
		/* "Oh really? That's a shame!" */
		fdEventShowMessageAndWait(ACTOR_SUPER_LUCKY_DICE_ATTENDANT, 0);
	}
	fdEventEnd();
}

/* Draws an actor at seven tenths of its size, as the geoglyph's and the dice */
/* hall's signs are drawn. */
void scContigoShrinkActor(s32 id) {
	ACTOR *actor = fdActorGetById(id);

	actor->scaleX = FIXED_ONE * 7 / 10;
	actor->scaleY = FIXED_ONE * 7 / 10;
	actor->radius = 8;
}

/* A thump, and the door at the head of the Sanctum's steps shows open; Felix */
/* walks slowly in through it and the screen fades out. */
void scContigoOpenSanctumDoor(s32 sourceX, s32 sourceY, s32 doorX, s32 doorY) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	sySndPlay(SE_THUMP);
	{
		s32 width = 1;
		s32 height = 2;

		fdMapGridCopyCellTilesIndicesRect(sourceX, sourceY, doorX, doorY, width,
										  height);
		fdMapGridCopyCellTilesIndicesRect(sourceX, sourceY + 2, doorX + 1,
										  doorY, width, height);
	}
	fdEventWait(20);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	fdActorSetMovementTypeById(MainRoutineStateWork.leaderActorId,
							   Contigo_EnterSanctumDoorScript);
	sySynch(12);
	sySndPlay(SE_MAP_EXIT);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
}

/* Felix walks up the Sanctum's west steps into its door (cells (42, 20) and */
/* (43, 20)): it opens with a thump, he walks in, and he arrives in a small */
/* stone room of the Sanctum with dragon statues. */
void scContigoEnterSanctumWestDoor(void) {
	scContigoOpenSanctumDoor(27, 0, 42, 13);
	syImportEventSetExitRequest(
		CONTIGO_GEOGLYPH_TO_ANEMOS_INNER_SANCTUM_ENTRANCE_NORTH);
}

/* Unused: the same for the Sanctum's middle door, which Reveal opens as a */
/* plain exit instead (scContigoRevealMiddleDoor). */
UNUSED void scContigoEnterSanctumMiddleDoor(void) {
	scContigoOpenSanctumDoor(27, 4, 48, 12);
	syImportEventSetExitRequest(
		CONTIGO_GEOGLYPH_TO_ANEMOS_INNER_SANCTUM_ENTRANCE_NORTHEAST);
}

/* Felix walks up the Sanctum's east steps into its door (cells (54, 20) and */
/* (55, 20)): it opens with a thump, he walks in, and he arrives in a small */
/* stone room of the Sanctum with dragon statues. */
void scContigoEnterSanctumEastDoor(void) {
	scContigoOpenSanctumDoor(27, 0, 54, 13);
	syImportEventSetExitRequest(
		CONTIGO_GEOGLYPH_TO_ANEMOS_INNER_SANCTUM_ENTRANCE_SOUTH);
}

extern u32 Contigo_JennaReunionScript[];
extern u32 Contigo_PiersReunionScript[];
extern u32 Contigo_ShebaReunionScript[];
extern u32 Contigo_KradenReunionScript[];
extern u32 Contigo_JoinLeaderScript[];
SPARKLE_STATE Contigo_SparkleState BSS_DATA ALIGNED(16);
void scContigoStartSparkle(ACTOR *actor, SPARKLE_STATE *state);
void scContigoStopSparkle(ACTOR *actor);

/* Walking into the house at the top of the hill once Jupiter Lighthouse's */
/* beacon is lit: Isaac, Garet, Mia and Ivan are waiting, and the reunion */
/* begins (scContigoInitShops). The two parties make peace, Kraden tells */
/* why Felix has been lighting the beacons, and Hamma comes to send them all */
/* to Mars Lighthouse as one party. */
void scContigoTheReunion(void) {
	ACTOR *actor;
	s32 answer;
	s32 angle;

	actor = fdActorGetById(ACTOR_HAMMA);
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	sySndPlay(SOUND_FADE_OUT_BGM);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdActorWatchAndSetScript(ACTOR_ISAAC, MainRoutineStateWork.leaderActorId);
	fdActorWatchAndSetScript(ACTOR_GARET, MainRoutineStateWork.leaderActorId);
	fdActorWatchAndSetScript(ACTOR_MIA, MainRoutineStateWork.leaderActorId);
	fdActorWatchAndSetScript(ACTOR_IVAN, MainRoutineStateWork.leaderActorId);
	fdActorWatchAndSetScript(ACTOR_HAMMA, MainRoutineStateWork.leaderActorId);
	fdEventWait(10);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 160, 382);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_WEST, 0);
	fdActorPlaceOnActor(ACTOR_JENNA, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_PIERS, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_SHEBA, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId);
	sySynch(1);
	fdActorSetSpeed(ACTOR_JENNA, 0.8);
	fdActorSetSpeed(ACTOR_PIERS, 0.8);
	fdActorSetSpeed(ACTOR_KRADEN, 0.8);
	fdActorSetSpeed(ACTOR_SHEBA, 0.8);
	fdActorSetMovementTypeById(ACTOR_JENNA, Contigo_JennaReunionScript);
	fdActorSetMovementTypeById(ACTOR_KRADEN, Contigo_KradenReunionScript);
	fdActorSetMovementTypeById(ACTOR_SHEBA, Contigo_ShebaReunionScript);
	fdActorSetMovementTypeAndWaitById(ACTOR_PIERS, Contigo_PiersReunionScript);
	fdActorStop(MainRoutineStateWork.leaderActorId);
	fdActorStop(ACTOR_GARET);
	fdActorStop(ACTOR_MIA);
	fdActorStop(ACTOR_IVAN);
	fdActorStop(ACTOR_HAMMA);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySndPlay(MUS_JOURNEY_TO_SAVE_THE_WORLD);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
	fdEventSetMessageId((s32)MsgGladYouKeptYourPromise);
	/* "I'm glad you kept your promise, Felix." */
	fdEventShowMessageAndWait(ACTOR_ISAAC, 0);
	fdActorSetAnimationById(ACTOR_GARET, ANIM_NOD);
	/* "All right, let's hear what he has to say for himself!" */
	fdEventShowMessageAndWait(ACTOR_GARET, 0);
	fdActorSetAttachedEffect(ACTOR_JENNA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(20);
	/* "What can we say, Isaac? Has Felix ever harmed you? Have we ever..." */
	fdEventShowMessageAndWait(ACTOR_JENNA, 0);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	/* "Felix's been avoiding you, but he never had any intention of..." */
	fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	/* "And he definitely helped us out back at Jupiter Lighthouse." */
	fdEventShowMessageAndWait(ACTOR_MIA, 0);
	fdActorSetAnimationById(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	/* "Isaac's been worrying about Jenna nonstop since this nightmare..." */
	fdEventShowMessageAndWait(ACTOR_IVAN, 0);
	fdActorSetAttachedEffect(ACTOR_PIERS,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	/* "She was afraid that if we met, we would be forced to fight... She..." */
	fdEventShowMessageAndWait(ACTOR_PIERS, 0);
	scFieldActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	/* "I know that, now... But it doesn't explain why you're doing this..." */
	fdEventShowMessageAndWait(ACTOR_ISAAC, 0);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	/* "Felix betrayed his hometown, Vale... That's why he hasn't been able..."
	 */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_GARET, FACING_SOUTH);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(148, CAMERA_KEEP, 368, 1);
	fdActorWalkToAndWait(ACTOR_KRADEN, 172, 342);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	fdActorWalkToAndWaitById(ACTOR_KRADEN, 146, 342);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "Felix had hoped to play the villain alone, without getting Jenna or..."
	 */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorJump(ACTOR_GARET, 2, 10);
	/* "He betrayed Vale? Kraden, what are you talking about?" */
	fdEventShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_GARET, 0);
	/* "He conspired to steal the Elemental Stars and fire the beacons of..." */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorSetAnimationById(ACTOR_MIA, ANIM_SHAKE_HEAD);
	/* "That's what Saturos and Menardi were trying to do..." */
	fdEventShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_MIA, 0);
	/* "Why were you helping them?" */
	fdEventShowMessageAndWait(ACTOR_IVAN, 0);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTHWEST);
	/* "Our parents' lives were at stake! We had to help!" */
	fdEventShowMessageAndWait(ACTOR_JENNA, 0);
	fdActorFaceDirectionById(ACTOR_GARET, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_ISAAC, FACING_EAST, 0);
	scFieldActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "Parents? But...your parents died three years ago, in that storm..." */
	fdEventShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "That night, Saturos and his men had raided Sol Sanctum... The storm..."
	 */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_GARET, FACING_SOUTH);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_WEST);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	/* "Garet, you saw two strangers that night°Saturos and Menardi. They..." */
	fdEventShowMessageAndWait(ACTOR_JENNA, 0);
	fdActorFaceDirectionById(ACTOR_GARET, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_ISAAC, FACING_EAST, 0);
	scFieldActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "They had failed to solve the mystery of Sol Sanctum. In doing so..." */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	/* "Everyone thought that you were killed by that boulder, Felix. I..." */
	fdEventShowMessageAndWait(ACTOR_JENNA, 0);
	fdActorSetAnimationById(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "In fact, nobody was killed by the boulder that day!" */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	scFieldActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 0);
	/* "So that means..." */
	fdEventShowMessageThenWait(ACTOR_ISAAC, 0, 40);
	scFieldActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	/* "Wait a minute, Kraden... Did you just say that nobody was killed by..."
	 */
	fdEventShowMessageAndWait(ACTOR_IVAN, 0);
	scFieldActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorFaceDirectionAndWait(ACTOR_MIA, FACING_SOUTH);
	/* "But what about Isaac's dad? And Jenna and Felix's parents? I thought..."
	 */
	fdEventShowMessageAndWait(ACTOR_MIA, 0);
	fdActorFaceDirectionAndWait(ACTOR_IVAN, FACING_NORTH);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdActorFaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHEAST);
	fdActorFaceDirectionById(ACTOR_ISAAC, FACING_EAST, 0);
	scFieldActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	/* "I'm not so sure now..." */
	fdEventShowMessageAndWait(ACTOR_ISAAC, 0);
	fdActorFaceDirectionById(ACTOR_MIA, FACING_EAST, 0);
	scFieldActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorFaceDirectionAndWait(ACTOR_GARET, FACING_NORTHEAST);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	/* "Yes! If Felix survived, Kyle and the others may have as well..." */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	scFieldActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	fdActorFaceDirectionAndWait(ACTOR_MIA, FACING_SOUTHEAST);
	/* "Why didn't you tell us? We could have worked together to save your..."
	 */
	fdMessageShowAtActor(ACTOR_MIA, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	answer = fdEventAskYesNoWithActorAnimation(
		MainRoutineStateWork.leaderActorId, 0);
	if(answer == 0) {
		fdActorWaitForAnimationChange(MainRoutineStateWork.leaderActorId);
		fdActorSetAnimationById(ACTOR_IVAN, ANIM_NOD);
		/* "Except that we had only just begun our journey, and we simply..." */
		fdEventShowMessageAndWait(ACTOR_IVAN, 0);
		FieldStateWork->msg++;
	}
	else {
		fdActorWaitForAnimationChange(MainRoutineStateWork.leaderActorId);
		FieldStateWork->msg++;
		fdActorSetAnimationById(ACTOR_IVAN, ANIM_SHAKE_HEAD);
		/* "He's right. They were too powerful then... It would have been..." */
		fdEventShowMessageAndWait(ACTOR_IVAN, 0);
	}
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "There's more to it than that. He had another reason to light the..." */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	fdActorFaceDirectionById(ACTOR_MIA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_GARET, FACING_SOUTH);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "Unless the lighthouse beacons are all lit, Weyard will eventually be..."
	 */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	scFieldActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 20);
	/* "Destroyed? But...how? Why?" */
	fdEventShowMessageAndWait(ACTOR_ISAAC, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
	fdActorSetAnimationById(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "King Hydros, ruler of Lemuria, says that our world is steadily
	 * shrinking." */
	fdEventShowMessageAndWait(ACTOR_PIERS, 0);
	fdActorFaceDirectionById(ACTOR_ISAAC, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_GARET, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_IVAN, FACING_EAST, 0);
	scFieldActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	/* "Elemental energy drives the growth of civilization. Without it, we
	 * and..." */
	fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTH, 20);
	scFieldActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	/* "All that because the lighthouses aren't illuminated?" */
	fdEventShowMessageAndWait(ACTOR_MIA, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 0);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
	/* "According to my research, that seems to be the case." */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorSetAttachedEffect(ACTOR_GARET,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(20);
	/* "But...once the lighthouses are all lit, you said the world would end..."
	 */
	fdEventShowMessageAndWait(ACTOR_GARET, 0);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	/* "That...might be true, too... But if we do nothing, the world will..." */
	fdEventShowMessageThenWait(ACTOR_KRADEN, 0, 80);
	/* "Wait for the end of the world to come or wind up accidentally..." */
	fdEventShowMessageAndWait(ACTOR_ISAAC, 0);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "Nothing is certain. There is no way to prevent the world from
	 * reaching..." */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	/* "However, we can fight to save the world from withering away due to
	 * the..." */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorFaceDirectionAndWait(ACTOR_GARET, FACING_SOUTHEAST);
	/* "And, Felix, you knew this? You were helping them because you knew
	 * what..." */
	fdEventOpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId,
							 FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	answer = fdEventAskYesNoWithActorAnimation(
		MainRoutineStateWork.leaderActorId, 0);
	if(answer == 0) {
		fdActorWaitForAnimationChange(MainRoutineStateWork.leaderActorId);
		fdActorSetAttachedEffect(ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT |
												  ATTACHED_EFFECT_ONCE);
		/* "You're a lot smarter than I remember you being..." */
		fdEventShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
		FieldStateWork->msg++;
	}
	else {
		fdActorWaitForAnimationChange(MainRoutineStateWork.leaderActorId);
		scFieldActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
		FieldStateWork->msg++;
		/* "You just figured it out along the way, huh? That's still pretty..."
		 */
		fdEventShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
	}
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirectionById(ACTOR_ISAAC, FACING_EAST, 0);
	scFieldActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "Why did you wait so long to tell me? I would have helped you..." */
	fdEventShowMessageAndWait(ACTOR_ISAAC, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
	fdActorSetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "You wouldn't have done it before, Isaac... It would have meant..." */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	scFieldActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	scFieldActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
	sySndPlay(MUS_SILENCE);
	fdActorSetSprCol(fdActorGetById(ACTOR_HAMMA), SPRITE_COLOR_HIDDEN);
	angle = FACING_NORTH_NORTHWEST;
	fdActorSetHorizontalPosAndAngWithTerrain(ACTOR_HAMMA, toFIXED(160),
											 toFIXED(466), angle);
	sySynch(1);
	/* "Now that we know all of this, shouldn't we be going?" */
	fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
	fdActorWatchAndSetScript(MainRoutineStateWork.leaderActorId, ACTOR_HAMMA);
	fdActorWatchAndSetScript(ACTOR_JENNA, ACTOR_HAMMA);
	fdActorWatchAndSetScript(ACTOR_PIERS, ACTOR_HAMMA);
	fdActorWatchAndSetScript(ACTOR_SHEBA, ACTOR_HAMMA);
	fdActorWatchAndSetScript(ACTOR_ISAAC, ACTOR_HAMMA);
	fdActorWatchAndSetScript(ACTOR_GARET, ACTOR_HAMMA);
	fdActorWatchAndSetScript(ACTOR_MIA, ACTOR_HAMMA);
	fdActorWatchAndSetScript(ACTOR_IVAN, ACTOR_HAMMA);
	fdActorSetSprCol(fdActorGetById(ACTOR_HAMMA), SPRITE_COLOR_NORMAL);
	fdCameraSetSpeed(2.4);
	fdCameraMoveTo(180, CAMERA_KEEP, 410, 1);
	fdCameraWaitForArrival();
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(148, CAMERA_KEEP, 368, 1);
	fdActorSetSpeed(ACTOR_HAMMA, 0.8);
	fdActorWalkToAndWait(ACTOR_HAMMA, 146, 406);
	scFieldActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
	/* "Hamma!" */
	fdEventShowMessageAndWait(ACTOR_MIA, 0);
	fdActorStop(MainRoutineStateWork.leaderActorId);
	fdActorStop(ACTOR_JENNA);
	fdActorStop(ACTOR_PIERS);
	fdActorStop(ACTOR_SHEBA);
	fdActorStop(ACTOR_ISAAC);
	fdActorStop(ACTOR_GARET);
	fdActorStop(ACTOR_MIA);
	fdActorStop(ACTOR_IVAN);
	sySndPlay(MUS_REUNITING_WITH_HAMA);
	fdActorSetAnimationAndWait(ACTOR_HAMMA, ANIM_NOD);
	/* "Hello again..." */
	fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_PIERS, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_ISAAC, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(20);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	/* "Hamma! You...know Isaac? And his companions, too?" */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHEAST);
	fdActorSetAnimationById(ACTOR_ISAAC, ANIM_NOD);
	fdEventWait(10);
	/* Isaac: "We met at the temple on the edge of the Lamakan Desert...", and
	 */
	/* Hamma at once: "I'm Hamma, descendant of the Anemos." */
	syFieldEventShowTwoMessagesAndWait(ACTOR_ISAAC, 5, 5, 8, 10, ACTOR_HAMMA, 9,
									   20, 18, REUNION_HAMMA_PORTRAIT_Y, 10);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_SOUTHEAST, 0);
	scFieldActorShowEmote(MainRoutineStateWork.leaderActorId,
						  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorSetAnimationById(ACTOR_HAMMA, ANIM_NOD);
	/* "I was born in Contigo, and I inherited the power of the Anemos." */
	fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
	fdActorSetAttachedEffect(ACTOR_GARET,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(20);
	/* "Hey... We just got into Contigo, and we found out this is where Ivan..."
	 */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
	/* "Yeah... We also learned that he's got a sister!" */
	fdEventShowMessageAndWait(ACTOR_MIA, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
	/* "Sister... You don't mean..." */
	fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
	scFieldActorShowEmote(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
	fdActorFaceDirectionAndRefreshById(ACTOR_HAMMA, FACING_EAST);
	/* "That would be me, yes." */
	fdEventShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_HAMMA, 0);
	fdActorSetSpeed(ACTOR_IVAN, 0.8);
	fdActorWalkToAndWait(ACTOR_IVAN, 130, 400);
	fdActorFaceDirectionById(ACTOR_IVAN, FACING_EAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_HAMMA, FACING_WEST);
	/* "Not now, Ivan... This isn't the time." */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMA, 0);
	scFieldActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorSetAnimationById(ACTOR_HAMMA, ANIM_SHAKE_HEAD);
	/* "Three lighthouses have been lit... The elements have been thrown out..."
	 */
	fdEventShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMA, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
	fdActorFaceDirectionById(ACTOR_MIA, FACING_EAST, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 40);
	fdActorFaceDirectionAndWait(ACTOR_HAMMA, FACING_NORTH_NORTHEAST);
	/* "Jupiter is growing stronger, and the north grows colder with every..."
	 */
	fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTHWEST,
						 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
	fdActorSetAnimationById(ACTOR_HAMMA, ANIM_SHAKE_HEAD);
	/* "You must hurry to Mars Lighthouse. Ignite its beacon, before all of..."
	 */
	fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
	/* "Mars Lighthouse? Agatio has the Mars Star. He's on his way to light
	 * it..." */
	fdEventShowMessageThenWait(ACTOR_JENNA, 0, 10);
	scContigoStartSparkle(actor, &Contigo_SparkleState);
	fdEventWait(80);
	scContigoStopSparkle(actor);
	fdActorSetAnimationAndWait(ACTOR_HAMMA, ANIM_SHAKE_HEAD);
	/* "I doubt they will be able to light the Mars beacon..." */
	fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
	fdActorSetAttachedEffect(ACTOR_PIERS,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	/* "Why's that?" */
	fdEventShowMessageThenWait(ACTOR_PIERS, 0, 10);
	scContigoStartSparkle(actor, &Contigo_SparkleState);
	fdEventWait(80);
	scContigoStopSparkle(actor);
	fdActorFaceDirectionAndWait(ACTOR_HAMMA, FACING_NORTH_NORTHEAST);
	fdActorSetAnimationById(ACTOR_HAMMA, ANIM_SHAKE_HEAD);
	/* "There is a powerful force that does not want to see Mars rekindled." */
	fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
	fdActorSetAttachedEffect(ACTOR_ISAAC,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_GARET,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_MIA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_JENNA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_PIERS,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_SHEBA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_IVAN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(20);
	fdActorFaceDirectionAndRefreshById(ACTOR_HAMMA, angle);
	fdActorSetAnimationById(ACTOR_HAMMA, ANIM_SHAKE_HEAD);
	/* "You will fail as well...unless you pool your strengths together and..."
	 */
	fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
	fdActorFaceDirectionById(ACTOR_ISAAC, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_GARET, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_MIA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	fdActorJump(ACTOR_KRADEN, 4, 10);
	/* "This is bad. We have to go...now!" */
	fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
	fdActorSetAnimationById(ACTOR_ISAAC, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_PIERS, ANIM_NOD);
	scFieldActorShowEmote(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
	/* "I had hoped that you would feel that way..." */
	fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
	/* "I've prepared a gift for you, to aid you on your way to Mars
	 * Lighthouse." */
	fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
	fdActorSetAnimationById(ACTOR_HAMMA, ANIM_NOD);
	/* "I must leave you now, but we will meet at the inlet." */
	fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
	sySndPlay(SOUND_FADE_OUT_BGM);
	fdCameraMoveTo(180, CAMERA_KEEP, 410, 1);
	fdActorWalkToAndWait(ACTOR_HAMMA, 160, 466);
	fdActorSetPos(ACTOR_HAMMA, 0, 0);
	fdCameraSetSpeed(2.4);
	fdCameraMoveTo(148, CAMERA_KEEP, 368, 1);
	fdCameraWaitForArrival();
	scPlayMusic();
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "I'm sorry we left Venus Lighthouse without seeing you... I'm sorry
	 * for..." */
	fdEventShowMessageAndWait(ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTH, 20);
	fdActorFaceDirectionById(ACTOR_GARET, FACING_EAST, 0);
	fdActorFaceDirectionAndRefreshById(ACTOR_ISAAC, FACING_EAST);
	fdActorSetAnimationById(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
	fdActorJump(ACTOR_JENNA, 2, 0);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTH);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
	/* "Maybe when this is all over, we can all go on a trip together." */
	fdEventShowMessageAndWait(ACTOR_JENNA, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_NORTHWEST);
	fdActorSetAttachedEffect(ACTOR_SHEBA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	/* "Hey, Jenna, you sure seem calm, considering that we're racing to
	 * meet..." */
	fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	scFieldActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
	/* "I'm just relieved that we've sorted our differences..." */
	fdEventShowMessageAndWait(ACTOR_MIA, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorSetAnimationById(ACTOR_PIERS, ANIM_NOD);
	/* "Me too, Mia... We could not have stood divided against a common foe." */
	fdEventShowMessageAndWait(ACTOR_PIERS, 0);
	scFieldActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "Yeah, I guess I'm a little happy that we're not going to have to
	 * beat..." */
	fdEventShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTH);
	fdActorSetAnimationById(ACTOR_ISAAC, ANIM_NOD);
	/* "Listen, this is Felix's quest now... We're just doing what we can to..."
	 */
	fdEventShowMessageAndWait(ACTOR_ISAAC, 0);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 20);
	/* "Aren't you happy?" */
	fdMessageShowAtActor(ACTOR_JENNA, 0);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_NORTH,
							 0);
	if(fdEventAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId,
										 0) == 0) {
		fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST);
		fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
		/* "Everything's finally come together... This is how it was meant
		 * to..." */
		fdEventShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
		FieldStateWork->msg++;
	}
	else {
		fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST);
		FieldStateWork->msg++;
		fdActorSetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
		/* "Don't worry about the future, Felix... Enjoy this brief moment's..."
		 */
		fdEventShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
	}
	fdActorWalkToAndWait(ACTOR_KRADEN, 146, 368);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 40);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_EAST, 40);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
	/* "But Hamma's waiting for us. Let's go to our ship!" */
	fdEventShowMessageAndWait(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
	fdActorSetAnimationById(ACTOR_ISAAC, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	fdActorSetMovementTypeById(ACTOR_ISAAC, Contigo_JoinLeaderScript);
	fdActorSetMovementTypeById(ACTOR_GARET, Contigo_JoinLeaderScript);
	fdActorSetMovementTypeById(ACTOR_MIA, Contigo_JoinLeaderScript);
	fdActorSetMovementTypeById(ACTOR_IVAN, Contigo_JoinLeaderScript);
	fdActorSetMovementTypeById(ACTOR_KRADEN, Contigo_JoinLeaderScript);
	fdActorSetMovementTypeById(ACTOR_JENNA, Contigo_JoinLeaderScript);
	fdActorSetMovementTypeById(ACTOR_SHEBA, Contigo_JoinLeaderScript);
	fdActorSetMovementTypeAndWaitById(ACTOR_PIERS, Contigo_JoinLeaderScript);
	gmPartyAddIsaacsParty();
	syGameFlagSet(FLAG_REUNION_AT_CONTIGO);
	fdEventEnd();
}

extern u32 Contigo_LeaderVisitsScript[];

/* The party's first steps into the house at the top of the hill after the */
/* ship has its wings (cells (7, 25) to (12, 25), just inside the door): Hamma
 */
/* welcomes it to her new home, tells Ivan why she kept from him that she is */
/* his sister, and warns that the world is falling into ruin unless Mars */
/* Lighthouse is lit. */
void scContigoReturnToNewHome(void) {
	if(syGameFlagGet(FLAG_SET_OUT_WITH_WINGS)) {
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		fdCameraSetSpeed(0.8);
		fdCameraMoveTo(160, CAMERA_KEEP, 368, 1);
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 158, 394);
		fdActorFaceDirection(ACTOR_HAMMA, FACING_SOUTH_SOUTHWEST, 0);
		fdActorPlaceOnActor(ACTOR_IVAN, MainRoutineStateWork.leaderActorId);
		sySynch(1);
		fdActorSetSpeed(ACTOR_IVAN, 0.8);
		fdActorWalkToAndWaitById(ACTOR_IVAN, 144, 374);
		fdActorFaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHEAST);
		scFieldActorShowEmote(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
							  20);
		fdEventSetMessageId((s32)MsgReturnedToMyNewHome);
		/* "Ah, you've returned! What brings you to my new home?" */
		fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
		/* "Hamma..." */
		fdEventShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0);
		fdActorSetAnimationById(ACTOR_IVAN, ANIM_SHAKE_HEAD);
		/* "You're my sister... Why didn't you tell me sooner? Why didn't
		 * you..." */
		fdEventShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0);
		fdActorSetAnimationById(ACTOR_HAMMA, ANIM_SHAKE_HEAD);
		/* "You play an important role in all this. If I'd told you, you
		 * would..." */
		fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 40);
		fdActorWalkToAndWait(ACTOR_IVAN, 180, 364);
		fdEventWait(20);
		fdActorWalkToAndWait(ACTOR_IVAN, 186, 388);
		fdEventWait(40);
		fdActorWalkToAndWait(ACTOR_IVAN, 146, 380);
		scFieldActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
							  20);
		/* "This place is so familiar... I feel like I've lived here my whole
		 * life." */
		fdEventShowMessageAndWait(ACTOR_IVAN, 0);
		scFieldActorShowEmote(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
							  20);
		/* "Look at me, Ivan." */
		fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
		scFieldActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 40);
		/* "You're too sentimental. You would have given up your quest to
		 * find..." */
		fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
		scFieldActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
							  0);
		/* "What are you saying, sister?" */
		fdEventShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0);
		fdActorSetAnimationById(ACTOR_HAMMA, ANIM_SHAKE_HEAD);
		/* "No, not sister... Do not think of me as your sister until you
		 * have..." */
		fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
		fdActorSetAttachedEffect(ACTOR_IVAN, ATTACHED_EFFECT_IN_FRONT |
												 ATTACHED_EFFECT_ONCE);
		/* "W-Why?" */
		fdEventShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0);
		scFieldActorShowEmote(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
							  20);
		/* "Our world is falling into ruin..." */
		fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
		fdActorPlaceOnActor(ACTOR_ISAAC, MainRoutineStateWork.leaderActorId);
		sySynch(1);
		scFieldActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
							  0);
		fdActorSetSpeed(ACTOR_ISAAC, 1.2);
		fdActorWalkToAndWaitById(ACTOR_ISAAC, 168, 380);
		fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHWEST);
		/* "What? Hamma, what do you mean? What's happening?" */
		fdEventShowMessageAndWait(ACTOR_ISAAC, 0);
		scFieldActorShowEmote(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
							  0);
		fdActorFaceDirection(ACTOR_HAMMA, FACING_SOUTH_SOUTHEAST, 0);
		fdActorPlaceOnActor(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId);
		sySynch(1);
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
		fdActorSetMovementTypeById(MainRoutineStateWork.leaderActorId,
								   Contigo_LeaderVisitsScript);
		fdActorSetSpeed(ACTOR_KRADEN, 0.8);
		fdActorWalkToAndWait(ACTOR_KRADEN, 182, 378);
		fdActorWalkToAndWaitById(ACTOR_KRADEN, 182, 360);
		fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
		scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
							  20);
		/* "Look, I don't want to interrupt your reunion or anything, but..." */
		fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
		fdActorFaceDirectionById(ACTOR_HAMMA, FACING_EAST, 0);
		fdActorSetAttachedEffect(ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT |
												   ATTACHED_EFFECT_ONCE);
		/* "I couldn't help overhearing you say something about the world..." */
		fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
		fdActorSetAnimationAndWait(ACTOR_HAMMA, ANIM_NOD);
		/* "Our world requires a delicate balance of elemental forces. That..."
		 */
		fdEventShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_HAMMA, 0);
		scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
		scFieldActorShowEmote(MainRoutineStateWork.leaderActorId,
							  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
		scFieldActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
		scFieldActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
		fdActorFaceDirectionAndWait(ACTOR_HAMMA, FACING_SOUTH_SOUTHWEST);
		/* "Without that balance, one element might be lost beneath the
		 * others..." */
		fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
		/* "You're talking about Mars Lighthouse, right?" */
		fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
		fdActorFaceDirectionAndRefreshById(ACTOR_HAMMA, FACING_EAST);
		fdActorSetAnimationAndWait(ACTOR_HAMMA, ANIM_NOD);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
		fdActorFaceDirection(MainRoutineStateWork.leaderActorId,
							 FACING_SOUTHEAST, 40);
		fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId,
								 FACING_EAST, 0);
		fdActorFaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHEAST);
		/* "Isn't Agatio taking care of Mars Lighthouse? Everything should
		 * be..." */
		fdEventShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0);
		scFieldActorShowEmote(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
							  0);
		fdActorFaceDirection(ACTOR_HAMMA, FACING_SOUTH_SOUTHWEST, 20);
		/* "Agatio will not light Mars Lighthouse." */
		fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
		fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 0);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 40);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
		fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHWEST);
		/* "You don't think he can do it?" */
		fdEventShowMessageAndWait(ACTOR_ISAAC, 0);
		fdActorSetAttachedEffect(ACTOR_HAMMA, ATTACHED_EFFECT_IN_FRONT |
												  ATTACHED_EFFECT_ONCE);
		fdActorFaceDirectionAndWait(ACTOR_HAMMA, FACING_SOUTH_SOUTHEAST);
		/* "No. A great power stands ready to oppose them." */
		fdEventShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_HAMMA, 0);
		scFieldActorShowEmote(MainRoutineStateWork.leaderActorId,
							  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
		scFieldActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
							  0);
		scFieldActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
							  0);
		scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
							  40);
		fdActorFaceDirectionAndWait(ACTOR_HAMMA, FACING_SOUTH_SOUTHWEST);
		fdActorSetAnimationById(ACTOR_HAMMA, ANIM_SHAKE_HEAD);
		/* "They lack the power to resist that force, and they will fall before
		 * it." */
		fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
		fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 0);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
		fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId,
								 FACING_SOUTHEAST, 40);
		fdEffectSetAttached(0, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdEventWait(20);
		fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHWEST);
		/* "But what will happen if Mars Lighthouse isn't lit quickly enough?"
		 */
		fdEventShowMessageAndWait(ACTOR_ISAAC, 0);
		fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId,
								 FACING_EAST, 0);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
		scFieldActorShowEmote(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
							  0);
		fdActorFaceDirection(ACTOR_HAMMA, FACING_SOUTH_SOUTHEAST, 0);
		/* "Our world will be plunged into ice." */
		fdEventShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_HAMMA, 0);
		fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
		/* "But what would make that happen?" */
		fdEventShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0);
		fdActorFaceDirectionAndWait(ACTOR_HAMMA, FACING_SOUTH_SOUTHWEST);
		/* "Even I do not know the answer to that question." */
		fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
		scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
							  20);
		fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 0);
		fdActorFaceDirectionById(ACTOR_HAMMA, FACING_EAST, 80);
		scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
							  40);
		scFieldActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
		scFieldActorShowEmote(MainRoutineStateWork.leaderActorId,
							  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
		scFieldActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
		scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB,
							  0);
		/* "Ah... Yes, given the way that the elements interact, I see..." */
		fdEventShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
		/* "Kraden, you understand?" */
		fdEventShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ISAAC, 0);
		scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
		/* "Well, I'm only guessing... But I think it's a good guess." */
		fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
		fdActorJump(ACTOR_IVAN, 2, 10);
		/* "Quit fooling around and tell me!" */
		fdEventShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0);
		fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
		/* "The earth is nurturing and warm. Many creatures are reared in
		 * this..." */
		fdMessageShowAtActor(ACTOR_KRADEN, 0);
		scFieldActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
		if(fdEventAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId,
											 0) == 0) {
			fdActorFaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHEAST);
			fdActorSetAnimationById(ACTOR_IVAN, ANIM_NOD);
			/* "I follow you..." */
			fdEventShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0);
			FieldStateWork->msg++;
		}
		else {
			fdActorFaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHEAST);
			FieldStateWork->msg++;
			fdActorSetAnimationById(ACTOR_IVAN, ANIM_SHAKE_HEAD);
			/* "It's OK, Felix... I don't really get it either, but let's
			 * see..." */
			fdEventShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0);
		}
		fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
		fdActorSetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
		/* "Now, think about the ocean. It is a cold place, is it not?" */
		fdMessageShowAtActor(ACTOR_KRADEN, 0);
		scFieldActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
		fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTHWEST, 0);
		if(fdEventAskYesNoWithActorAnimation(MainRoutineStateWork.leaderActorId,
											 0) == 0) {
			fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHEAST);
			fdActorSetAnimationById(ACTOR_ISAAC, ANIM_NOD);
			/* "Yeah, it is awfully cold..." */
			fdEventShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ISAAC, 0);
			FieldStateWork->msg++;
		}
		else {
			fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHEAST);
			FieldStateWork->msg++;
			fdActorSetAnimationById(ACTOR_ISAAC, ANIM_SHAKE_HEAD);
			/* "I don't know what point he's trying to make, but I have to
			 * agree..." */
			fdEventShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ISAAC, 0);
		}
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
		/* "Both lighthouses were lit, and a balance was reached." */
		fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
		scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
							  0);
		fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
		/* "But with only the lighthouse of wind lit, the balance has been..."
		 */
		fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
		fdActorSetAttachedEffect(ACTOR_IVAN, ATTACHED_EFFECT_IN_FRONT |
												 ATTACHED_EFFECT_ONCE);
		/* "I get it... Wind cools things down, but it can't heat them up..." */
		fdEventShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0);
		fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
		fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
		/* "When you burn your hand on something hot, you blow on it to cool
		 * it..." */
		fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
		fdActorSetAnimationById(ACTOR_ISAAC, ANIM_NOD);
		/* "Right, yeah... I've always done that without thinking about it,
		 * but..." */
		fdEventShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ISAAC, 0);
		scFieldActorShowEmote(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
		/* "When Mercury Lighthouse was lit, this didn't happen, though. I..."
		 */
		fdEventShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_HAMMA, 0);
		fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
		fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
		/* "Water cannot cool things as swiftly as wind, perhaps." */
		fdEventShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
		fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
		scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
							  0);
		/* "Or maybe the combined power of water and wind is cooling things
		 * down..." */
		fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
		fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHWEST);
		/* "If what Hamma's saying is true, we'd better get moving again,
		 * Felix." */
		fdEventShowMessageAndWait(ACTOR_ISAAC, 0);
		fdActorSetAttachedEffect(ACTOR_IVAN, ATTACHED_EFFECT_IN_FRONT |
												 ATTACHED_EFFECT_ONCE);
		fdActorFaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHWEST);
		/* "I got too worked up about seeing my sister... Sorry." */
		fdEventShowMessageAndWait(ACTOR_IVAN, 0);
		fdActorFaceDirectionAndWait(ACTOR_HAMMA, FACING_SOUTH_SOUTHWEST);
		fdActorSetAnimationAndWait(ACTOR_HAMMA, ANIM_NOD);
		/* "It's all right. I'm happy that you cared so much..." */
		fdEventShowMessageAndWait(ACTOR_HAMMA, 0);
		fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
		fdActorSetAnimationById(ACTOR_KRADEN, ANIM_NOD);
		/* "All right, Felix! Let's head to Mars Lighthouse!" */
		fdEventShowMessageAndWait(ACTOR_KRADEN, 0);
		fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId,
								   ANIM_NOD);
		fdActorSetSpeed(ACTOR_IVAN, 1.2);
		fdActorSetSpeed(ACTOR_ISAAC, 1.2);
		fdActorSetSpeed(ACTOR_KRADEN, 1.2);
		fdActorSetMovementTypeById(ACTOR_IVAN, Contigo_RejoinLeaderScript);
		fdActorSetMovementTypeById(ACTOR_ISAAC, Contigo_RejoinLeaderScript);
		fdActorSetMovementTypeAndWaitById(ACTOR_KRADEN,
										  Contigo_RejoinLeaderScript);
		syGameFlagSet(FLAG_HAMMA_WELCOMED_PARTY);
		fdEventEnd();
	}
}

s32 scInit(void) {
	s32 scene = MainRoutineStateWork.next;

	if(scene == SCENE_CONTIGO)
		scContigoInitVillage();
	else if(scene == SCENE_CONTIGO_GEOGLYPH)
		scContigoInitGeoglyph();
	else if(scene == SCENE_CONTIGO_SHOPS)
		scContigoInitShops();
	else if(scene == SCENE_CONTIGO_LUCKY_DICE)
		scContigoInitLuckyDice();
	return 0;
}

s32 scPrepareMap(void) { return 0; }

/* A task that draws the Corn at the leader's sprite priority. */
void scContigoKeepPriorityWithLeader(void) {
	if(fdActorGetById(ACTOR_CORN) != 0) {
		ACTOR *owner = fdActorGetById(MainRoutineStateWork.leaderActorId);

		fdActorSetSprPriority(ACTOR_CORN, ((FIELD_SPR *)owner->spr)->pri);
	}
}

/* Sets up the village: the two children walking about its middle stop when */
/* faced (scContigoPauseWhenFaced); once Jupiter Lighthouse's beacon is lit, */
/* until the ship has its wings, the girl and the old woman are gone and four */
/* doors are locked; Shine waits where it landed until caught. Arriving by the
 */
/* gate, or out of the two houses south of the Teleport circle or the Lucky */
/* Wheels tent, the view follows Felix east past the gate to the weed patch; */
/* then the Teleport circle's colours are set. */
void scContigoInitVillage(void) {
	ACTOR *actor = fdActorGetById(ACTOR_BOY_DOUBTING_THE_LEGENDS);

	sySetEvent(scContigoKeepPriorityWithLeader, TASK_PASS_FRAME);
	actor->sceneCnt = 1;
	actor->sceneByte = 0;
	actor->update = scContigoPauseWhenFaced;
	if(syGameFlagGet(FLAG_JUPITER_BEACON_LIT) != 0 &&
	   syGameFlagGet(FLAG_SET_OUT_WITH_WINGS) == 0) {
		fdActorStop(ACTOR_GIRL_ON_THE_SHIP);
		fdActorSetPos(ACTOR_GIRL_ON_THE_SHIP, 0, 0);
		fdActorSetPos(ACTOR_OLD_WOMAN_ON_THE_GEOGLYPHS, 0, 0);
		scContigoLockDoors();
	}
	else {
		actor = fdActorGetById(ACTOR_GIRL_ON_THE_SHIP);
		actor->sceneCnt = 0;
		actor->sceneByte = 0;
		actor->update = scContigoPauseWhenFaced;
	}
	if(syGameFlagGet(FLAG_SHINE_FREED) != 0 &&
	   syGameFlagGet(FLAG_SHINE_CAUGHT) == 0) {
		fdActorSetPos(ACTOR_SHINE, 32, 708);
	}
	else {
		fdActorSetSprCol(fdActorGetById(ACTOR_SHINE), SPRITE_COLOR_HIDDEN);
		fdActorSetSprFlags(fdActorGetById(ACTOR_SHINE), 0);
	}
	if(syGameFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
		switch(MainRoutineStateWork.entrance) {
		case CONTIGO_FROM_WORLD_MAP:
		case CONTIGO_FROM_CONTIGO_SHOPS_SOUTH:
		case CONTIGO_FROM_CONTIGO_SHOPS_SOUTHWEST:
		case CONTIGO_FROM_CONTIGO_LUCKY_DICE_SOUTHWEST:
			syGameFlagSet(FLAG_WHOLE_VILLAGE_IN_VIEW);
			break;
		default:
			fdSetCameraBounds(0, toFIXED(512), toFIXED(512), toFIXED(1024));
			break;
		}
	}
	if(syGameFlagGet(FLAG_WHOLE_VILLAGE_IN_VIEW) != 0) {
		fdSetCameraBounds(0, toFIXED(512), toFIXED(880), toFIXED(1024));
		fdCameraFollowActor(MainRoutineStateWork.leaderActorId, 0);
		fdMapGridCenterOnFollowedPoint();
		sySynch(1);
	}
	scPaletteSetColsFade(TeleportCircle_PaletteSlots, TeleportCircle_Colors,
						 TeleportCircle_ShimmerColors,
						 TeleportCircle_ShimmerSlots);
}

/* Sets up the geoglyph: until Jupiter Lighthouse's beacon is lit the wings' */
/* parts lie on it among the workers; after, they and the workers are gone, */
/* and the men in long robes too until the ship has its wings. */
void scContigoInitGeoglyph(void) {
	scContigoShrinkActor(ACTOR_SIGN);
	if(syGameFlagGet(FLAG_JUPITER_BEACON_LIT) != 0) {
		fdMapGridCopyCellAttributes(44, 40, 8, 3, 44, 36);
		fdActorSetPos(ACTOR_WING_FRAME_A, 0, 0);
		fdActorSetPos(ACTOR_WING_FRAME_B, 0, 0);
		fdActorSetPos(ACTOR_WING_FRAME_C, 0, 0);
		fdActorSetPos(ACTOR_METAL_FEATHER_A, 0, 0);
		fdActorSetPos(ACTOR_METAL_FEATHER_B, 0, 0);
		fdActorSetPos(ACTOR_METAL_FEATHER_C, 0, 0);
		fdActorSetPos(ACTOR_METAL_FEATHER_D, 0, 0);
		fdActorSetPos(ACTOR_METAL_FEATHER_E, 0, 0);
		fdActorSetPos(ACTOR_LOOSE_PART_A, 0, 0);
		fdActorSetPos(ACTOR_LOOSE_PART_B, 0, 0);
		fdActorSetPos(ACTOR_LOOSE_PART_C, 0, 0);
		fdActorSetPos(ACTOR_LOOSE_PART_D, 0, 0);
		fdActorSetPos(ACTOR_LOOSE_PART_E, 0, 0);
		fdActorSetPos(ACTOR_LOOSE_PART_F, 0, 0);
		fdActorSetPos(ACTOR_LOOSE_PART_G, 0, 0);
		fdActorSetPos(ACTOR_VISITOR_FROM_AFAR, 0, 0);
		fdActorSetPos(ACTOR_WOMAN_ON_THE_WINGS, 0, 0);
		fdActorSetPos(ACTOR_WING_WORKER_B, 0, 0);
		fdActorSetPos(ACTOR_WING_WORKER_C, 0, 0);
		fdActorSetPos(ACTOR_WING_WORKER_D, 0, 0);
		if(syGameFlagGet(FLAG_SET_OUT_WITH_WINGS) == 0) {
			fdActorSetPos(ACTOR_WING_WORKER_A, 0, 0);
			fdActorSetPos(ACTOR_MAN_ON_THE_GEOGLYPH, 0, 0);
			fdActorSetPos(ACTOR_MAN_WHO_TRANSLATED_THE_WALL, 0, 0);
			fdActorSetPos(ACTOR_MAN_LOCKED_OUT_OF_THE_SANCTUM, 0, 0);
			fdActorSetPos(ACTOR_MAN_ON_HAMMET, 0, 0);
		}
		else {
			fdActorGetById(ACTOR_WING_WORKER_A)->facing =
				FACING_NORTH_NORTHEAST;
			sySynch(1);
		}
	}
	else {
		scContigoSetUpWingParts();
	}
}

/* Sets up the houses: walking into the house at the top of the hill once the */
/* beacon is lit starts the reunion; in the others the baby lies in its */
/* cradle, the old man on Hesperia shivers once the ship has its wings, and */
/* the workers resting at the inn are gone after the reunion. */
void scContigoInitShops(void) {
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	if(MainRoutineStateWork.entrance == CONTIGO_SHOPS_FROM_CONTIGO_NORTH) {
		if(syGameFlagGet(FLAG_JUPITER_BEACON_LIT) != 0 &&
		   syGameFlagGet(FLAG_REUNION_AT_CONTIGO) == 0)
			scContigoTheReunion();
	}
	else if(MainRoutineStateWork.entrance ==
			CONTIGO_SHOPS_FROM_ATTEKA_INLET_GEODE) {
		syGameFlagClear(FLAG_SHOW_AREA_NAME);
	}
	else {
		fdActorSetAnimationById(ACTOR_BABY, 2);
		fdActorSetSprFlags(fdActorGetById(ACTOR_BABY), 0);
		if(syGameFlagGet(FLAG_SET_OUT_WITH_WINGS) != 0)
			fdActorSetMovementTypeById(ACTOR_OLD_MAN_ON_HESPERIA,
									   Contigo_ShiverScript);
		if(syGameFlagGet(FLAG_REUNION_AT_CONTIGO) != 0) {
			fdActorSetPos(ACTOR_WORN_OUT_WORKER, 0, 0);
			fdActorSetPos(ACTOR_WORKER_ON_THE_PROPHECIES, 0, 0);
			fdActorSetPos(ACTOR_OLD_MAN_TOO_OLD_TO_HELP, 0, 0);
			fdActorSetPos(ACTOR_OLD_WOMAN_AWAITING_THE_MIRACLE, 0, 0);
		}
	}
}

/* Back from a dice game, the attendant reports the coins the party won, with */
/* a jingle for the size of the win, or lost. */
static inline void scContigoReportCoinChange(void) {
	s32 coinChange;

	coinChange = MainRoutineStateWork.coins - *(s32 *)SceneStateWork;
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	if(coinChange > 0) {
		if(coinChange >= LUCKY_DICE_LARGE_WIN)
			sySndPlay(MUS_SUPER_LUCKY);
		else if(coinChange >= LUCKY_DICE_MEDIUM_WIN)
			sySndPlay(MUS_VERY_LUCKY);
		else
			sySndPlay(MUS_LUCKY);
		fdEventWait(20);
		fdEventSetMessageId(COIN_WON_MESSAGE);
		mnMessageArgumentPush(coinChange, MESSAGE_ARGUMENT_NUMBER);
		/* "Ya won ... coin! Talk about lucky!" */
		fdEventShowMessageAndWait(ACTOR_LUCKY_DICE_ATTENDANT, 0);
		mnSndWaitForJingle();
	}
	else if(coinChange < 0) {
		fdEventSetMessageId(COIN_LOST_MESSAGE);
		mnMessageArgumentPush(-coinChange, MESSAGE_ARGUMENT_NUMBER);
		/* "Ya lost ... coin. Better luck next time." */
		fdEventShowMessageAndWait(ACTOR_LUCKY_DICE_ATTENDANT, 0);
	}
	fdEventEnd();
}

/* Sets up the dice hall: its signs drawn small and its attendants' colours, */
/* and on the party's return from a game the coins won or lost, or the */
/* prizes. */
void scContigoInitLuckyDice(void) {
	s8 *wonPrizes;
	s8 *prize;
	s32 item;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	scContigoShrinkActor(ACTOR_SUPER_LUCKY_DICE_RULES_SIGN);
	scContigoShrinkActor(ACTOR_LUCKY_DICE_RULES_SIGN);
	scContigoShrinkActor(ACTOR_LUCKY_DICE_PAIRS_SIGN);
	scContigoShrinkActor(ACTOR_LUCKY_WHEELS_RULES_SIGN);
	scContigoShrinkActor(ACTOR_LUCKY_WHEELS_PRIZE_SIGN);
	fdActorSetSprCol(fdActorGetById(ACTOR_LUCKY_DICE_ATTENDANT),
					 SPRITE_COLOR_GRB);
	fdActorSetSprCol(fdActorGetById(ACTOR_LUCKY_DICE_RULES_ATTENDANT),
					 SPRITE_COLOR_RBG);
	fdActorSetSprCol(fdActorGetById(ACTOR_SUPER_LUCKY_DICE_ATTENDANT),
					 SPRITE_COLOR_NORMAL);
	fdActorSetSprCol(fdActorGetById(ACTOR_SUPER_LUCKY_DICE_RULES_ATTENDANT),
					 SPRITE_COLOR_BGR);
	if(!fdEventGetFlag(FLAG_LEFT_FOR_A_GAME))
		return;
	syGameFlagClear(FLAG_LEFT_FOR_A_GAME);
	/* The dice games come back here to report the change in the party's coins.
	 */
	if(MainRoutineStateWork.entrance == ENTRANCE_FROM_SUPER_LUCKY_DICE) {
		scContigoReportCoinChange();
	}
	else if(MainRoutineStateWork.entrance == ENTRANCE_FROM_LUCKY_DICE) {
		scContigoReportCoinChange();
	}
	else if(MainRoutineStateWork.entrance == ENTRANCE_FROM_LUCKY_WHEELS) {
		/* Lucky Wheels comes back with the icons matched, for the attendant to
		 */
		/* award a prize for each. */
		wonPrizes = MainRoutineStateWork.wheelsPrizes;
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		fdEventStartScrTransition();
		fdEventWaitTransitionFrames();
		fdEventWait(20);
		if(wonPrizes[0] == WHEELS_PRIZES_END) {
			scContigoOfferLuckyWheels(1);
		}
		else if(wonPrizes[0] == WHEELS_PRIZES_NONE_TO_HAND_OUT) {
			fdEventSetMessageId(READ_SIGN_MESSAGE);
			/* "The rules are listed on the sign. Take a look, take a look." */
			fdEventShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
		}
		else {
			fdEventSetMessageId(CONGRATULATIONS_MESSAGE);
			/* "Congratulations!" */
			fdEventShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
			if(wonPrizes[0] != WHEELS_PRIZES_END) {
				for(prize = wonPrizes; *prize != WHEELS_PRIZES_END; prize++) {
					if(prize == wonPrizes)
						fdEventSetMessageId(FIRST_PRIZE_MESSAGE);
					else
						fdEventSetMessageId(NEXT_PRIZE_MESSAGE);
					item = scContigoSelectLuckyWheelsPrize(*prize);
					mnMessageArgumentPush(item, MESSAGE_ARGUMENT_ITEM);
					/* "You won ...! Here you go.", then "And you won ...! Here
					 * you go." */
					fdEventShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
					fdItemDispShowItem(item, ITEM_DISPLAY_SPARKLES |
												 ITEM_DISPLAY_RAISED);
					fdPartyInventoryGiveItem(item, 0);
					fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTH);
				}
			}
			MainRoutineStateWork.wheelsPrizes[0] =
				WHEELS_PRIZES_NONE_TO_HAND_OUT;
			fdEventSetMessageId(COME_AGAIN_MESSAGE);
			/* "Come again!" */
			fdEventShowMessageAndWait(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
		}
		fdEventEnd();
	}
}

/* The sparkle's update, played over Hamma as she speaks of Mars Lighthouse in
 */
/* the reunion: two pairs of white half-rings rise around her, a pair starting
 */
/* every 16 frames, until both have climbed RING_CLIMB pixels. */
void scContigoRaiseSparkle(ACTOR *actor) {
	VECTOR3 position;
	s32 x, z;
	FIXED sine;
	s32 i, j;
	SPARKLE_STATE *state = (SPARKLE_STATE *)actor->watchedActor;
	u8 *count = &actor->sceneByte;
	EFFECT_SPR *sprite;

	if(*count & 1)
		fdActorSetSprColMode(actor, (u8)((u32)((COUNTER_BITS *)count)->rest %
										 (SPRITE_COLOR_BRG + 1)));
	if(actor->sceneCnt == 0) {
		if((*count & (RING_INTERVAL - 1)) == 0) {
			sySndPlay(SE_RISING_RINGS);
			state->heights[state->nextPair] = 0;
			state->nextPair ^= 1;
		}
	}
	else if(actor->sceneCnt == 1) {
		if(state->heights[0] >= RING_CLIMB && state->heights[1] >= RING_CLIMB) {
			fdActorSetSprColMode(actor, SPRITE_COLOR_NORMAL);
			syVramCacheFreeEntry(state->vramEntry);
			actor->update = 0;
			return;
		}
	}
	(*count)++;
	for(i = 0; i < 2; i++) {
		/* Its size: half a turn of sine over the climb. */
		sine = sySin(state->heights[i] * (DEGtoANG(180) / RING_CLIMB));
		if(state->heights[i] >= 0) {
			if(state->heights[i] < RING_CLIMB) {
				position.x = actor->x.fixed;
				position.y = actor->y.fixed + toFIXED(state->heights[i]);
				position.z = actor->z.fixed;
				fdCameraWorldToScr(&position);
				x = position.x;
				z = position.z;
				sprite = &state->sprs[i * 2];
				for(j = 0; j < 2; j++) {
					sprite->x = x;
					sprite->y = z;
					sprite->scaleX = sine;
					sprite->scaleY = sine;
					if(j == 0) {
						sprite->depth = fdActorGetSprDepth(actor) - 1;
					}
					else {
						sprite->depth = fdActorGetSprDepth(actor) + 1;
						sprite->y -= toFIXED(1);
						sprite->scaleY = -sprite->scaleY;
					}
					fdEffectSprDraw(sprite);
					sprite++;
				}
				state->heights[i]++;
			}
		}
	}
}

/* Sets up the sparkle: loads its picture and its four 16 by 16 sprites, and */
/* hands the object to its update, scContigoRaiseSparkle. */
void scContigoStartSparkle(ACTOR *actor, SPARKLE_STATE *state) {
	EFFECT_SPR *sprite;
	s32 tile;
	s32 row;
	s32 col;

	state->vramEntry = syVramCacheFindFreeEntry();
	syVramCacheLoad(state->vramEntry, 256, LightRing_Tiles);
	state->nextPair = 0;
	state->heights[0] = RING_NOT_STARTED;
	state->heights[1] = RING_NOT_STARTED;
	state->actor = actor;
	actor->sceneCnt = 0;
	actor->sceneByte = 0;
	actor->update = scContigoRaiseSparkle;
	tile = VramCacheEntries[state->vramEntry].ofs >> 5;
	actor->watchedActor = (FIELD_OBJECT *)state;
	for(row = 0; row <= 1; row++) {
		for(col = 1, sprite = state->sprs + row + row; col >= 0; col--) {
			fdEffectSprInit(sprite, 16, 16, OBJ_SIZE_32X16, tile);
			sprite->oam.fullCol = 0;
			sprite->oam.pal = 0;
			sprite->oam.pri = fdActorGetSprPriority(actor);
			sprite++;
		}
	}
}

/* Ends the sparkle: its update finishes once both pairs have risen. */
void scContigoStopSparkle(ACTOR *actor) { actor->sceneCnt = 1; }

/* clang-format off */
#include "shared/light_ring_tiles.c"

#include "shared/field_effect_duration_scripts.c"

/* The Lucky Wheels' prizes, three to each icon that wins them */
/* (scContigoSelectLuckyWheelsPrize). */
u32 Contigo_WheelsPrizes[] = {
    ITEM_MYTHRIL_SHIRT, ITEM_SILK_SHIRT,  ITEM_RUNNING_SHIRT,
    ITEM_HYPER_BOOTS,   ITEM_QUICK_BOOTS, ITEM_FUR_BOOTS,
    ITEM_ADEPT_RING,    ITEM_WAR_RING,    ITEM_SLEEP_RING,
    ITEM_POTION,        ITEM_VIAL,        ITEM_NUT,
    ITEM_WATER_OF_LIFE, ITEM_PSY_CRYSTAL, ITEM_ELIXIR,
};

s32 Contigo_EnterSanctumDoorScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Z, -(toFIXED(1) / 2)),
    SC_WAIT(1),
    SC_LOOP(24, SC_TOP),
    SC_END,
};

#define TELEPORT_CIRCLE_FIRST_ENTRY 89
#include "shared/teleport_circle_palette.c"

const s32 *FieldEffect_DurationScripts[] = {
    (s32)FieldEffect_ShortScript,
    (s32)FieldEffect_MediumScript,
    (s32)FieldEffect_LongScript,
};

u32 Contigo_DoubtingBoyScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_SPEED(1),
    SC_ACCELERATION(0.4),
    SC_MOVE(338, 0, 844),
    SC_SYNC,
    SC_MOVE(318, 0, 844),
    SC_SYNC,
    SC_MOVE(304, 0, 872),
    SC_SYNC,
    SC_MOVE(320, 0, 884),
    SC_SYNC,
    SC_MOVE(336, 0, 884),
    SC_SYNC,
    SC_MOVE(352, 0, 872),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u32 Contigo_GirlOnTheShipScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_SPEED(1),
    SC_ACCELERATION(0.4),
    SC_MOVE(318, 0, 884),
    SC_SYNC,
    SC_MOVE(338, 0, 884),
    SC_SYNC,
    SC_MOVE(352, 0, 872),
    SC_SYNC,
    SC_MOVE(338, 0, 844),
    SC_SYNC,
    SC_MOVE(318, 0, 844),
    SC_SYNC,
    SC_MOVE(304, 0, 872),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u32 Contigo_JennaReunionScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(164, 0, 360),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

u32 Contigo_PiersReunionScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(190, 0, 352),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

u32 Contigo_ShebaReunionScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(190, 0, 390),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

u32 Contigo_KradenReunionScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(182, 0, 368),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

/* The end of the reunion: each walks to the leader and goes into him, as the */
/* two parties become one. */
u32 Contigo_JoinLeaderScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(160, 0, 382),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
    /* Unused: past the end. */
    SC_ANIMATION(ANIM_WALK),
    SC_WAIT(10),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/* The old man on Hesperia shivering once the party has set out with the */
/* wings: "Brrr...Brrr... It's been really cold since the lighthouse beacon was */
/* lit!" */
s32 Contigo_ShiverScript[] = {
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_ADD(SW_SCALE_X, -FIXED_ONE * 5 / 32),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE * 5 / 64),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 64),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE * 5 / 128),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 128),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE * 5 / 128),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 128),
    SC_WAIT(2),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/* In Hamma's new home the leader steps up to face her from the west. */
u32 Contigo_LeaderVisitsScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(128, 0, 378),
    SC_SYNC,
    SC_MOVE(132, 0, 360),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(10),
    SC_END,
};

/* The end of the visit to Hamma's new home: Ivan, Isaac and Kraden walk to */
/* the leader and go into him. */
u32 Contigo_RejoinLeaderScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(132, 0, 359),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

/* The worker wondering which part goes where looks from one part of the */
/* wings to another, and wonders (scContigoShowWorkerPuzzled). */
s32 Contigo_PuzzledWorkerScript[] = {
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(120),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(40),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(40),
    SC_CALL((s32)scContigoShowWorkerPuzzled),
    SC_WAIT(120),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

SCENE_ENTRANCE SceneEntrances[] = {
    {0, CONDITION_ALWAYS, 271, 0, 808, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 SceneExits[] = {
    SCENE_EXITS(Contigo),
    SCENE_EXIT(CONTIGO_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_CONTIGO),
    SCENE_EXIT(CONTIGO_TO_CONTIGO_SHOPS_SOUTH, ContigoShops, CONTIGO_SHOPS_FROM_CONTIGO_SOUTH),
    SCENE_EXIT(CONTIGO_TO_CONTIGO_SHOPS_SOUTHWEST, ContigoShops, CONTIGO_SHOPS_FROM_CONTIGO_SOUTHWEST),
    SCENE_EXIT(CONTIGO_TO_CONTIGO_LUCKY_DICE_SOUTHWEST, ContigoLuckyDice, CONTIGO_LUCKY_DICE_FROM_CONTIGO_SOUTHWEST),
    SCENE_EXIT(CONTIGO_TO_CONTIGO_SHOPS_WEST, ContigoShops, CONTIGO_SHOPS_FROM_CONTIGO_WEST),
    SCENE_EXIT(CONTIGO_TO_CONTIGO_LUCKY_DICE_NORTHEAST, ContigoLuckyDice, CONTIGO_LUCKY_DICE_FROM_CONTIGO_NORTHEAST),
    SCENE_EXIT(CONTIGO_TO_CONTIGO_LUCKY_DICE_EAST, ContigoLuckyDice, CONTIGO_LUCKY_DICE_FROM_CONTIGO_EAST),
    SCENE_EXIT(CONTIGO_TO_CONTIGO_SHOPS_EAST, ContigoShops, CONTIGO_SHOPS_FROM_CONTIGO_EAST),
    SCENE_EXIT(CONTIGO_TO_CONTIGO_SHOPS_NORTHEAST_A, ContigoShops, CONTIGO_SHOPS_FROM_CONTIGO_NORTHEAST_A),
    SCENE_EXIT(CONTIGO_TO_CONTIGO_SHOPS_NORTHEAST_B, ContigoShops, CONTIGO_SHOPS_FROM_CONTIGO_NORTHEAST_B),
    SCENE_EXIT(CONTIGO_TO_CONTIGO_SHOPS_NORTH, ContigoShops, CONTIGO_SHOPS_FROM_CONTIGO_NORTH),
    SCENE_EXIT(CONTIGO_TO_CONTIGO_GEOGLYPH, ContigoGeoglyph, CONTIGO_GEOGLYPH_FROM_CONTIGO),
    SCENE_EXITS(ContigoGeoglyph),
    SCENE_EXIT(CONTIGO_GEOGLYPH_TO_CONTIGO, Contigo, CONTIGO_FROM_CONTIGO_GEOGLYPH),
    SCENE_EXIT(CONTIGO_GEOGLYPH_TO_ANEMOS_INNER_SANCTUM_ENTRANCE_NORTH, AnemosInnerSanctumEntrance, ANEMOS_INNER_SANCTUM_ENTRANCE_FROM_CONTIGO_GEOGLYPH_NORTH),
    SCENE_EXIT(CONTIGO_GEOGLYPH_TO_ANEMOS_INNER_SANCTUM_ENTRANCE_NORTHEAST, AnemosInnerSanctumEntrance, ANEMOS_INNER_SANCTUM_ENTRANCE_FROM_CONTIGO_GEOGLYPH_NORTHEAST),
    SCENE_EXIT(CONTIGO_GEOGLYPH_TO_ANEMOS_INNER_SANCTUM_ENTRANCE_SOUTH, AnemosInnerSanctumEntrance, ANEMOS_INNER_SANCTUM_ENTRANCE_FROM_CONTIGO_GEOGLYPH_SOUTH),
    SCENE_EXITS(ContigoShops),
    SCENE_EXIT(CONTIGO_SHOPS_TO_CONTIGO_SOUTH, Contigo, CONTIGO_FROM_CONTIGO_SHOPS_SOUTH),
    SCENE_EXIT(CONTIGO_SHOPS_TO_CONTIGO_SOUTHWEST, Contigo, CONTIGO_FROM_CONTIGO_SHOPS_SOUTHWEST),
    SCENE_EXIT(CONTIGO_SHOPS_TO_CONTIGO_NORTHEAST_B, Contigo, CONTIGO_FROM_CONTIGO_SHOPS_NORTHEAST_B),
    SCENE_EXIT(CONTIGO_SHOPS_TO_CONTIGO_NORTH, Contigo, CONTIGO_FROM_CONTIGO_SHOPS_NORTH),
    SCENE_EXIT(CONTIGO_SHOPS_TO_CONTIGO_EAST, Contigo, CONTIGO_FROM_CONTIGO_SHOPS_EAST),
    SCENE_EXIT(CONTIGO_SHOPS_TO_CONTIGO_NORTHEAST_A, Contigo, CONTIGO_FROM_CONTIGO_SHOPS_NORTHEAST_A),
    SCENE_EXIT(CONTIGO_SHOPS_TO_CONTIGO_WEST, Contigo, CONTIGO_FROM_CONTIGO_SHOPS_WEST),
    SCENE_EXIT(CONTIGO_SHOPS_TO_ATTEKA_INLET_GEODE, AttekaInletGeode, ATTEKA_INLET_GEODE_FROM_CONTIGO_SHOPS),
    SCENE_EXITS(ContigoLuckyDice),
    SCENE_EXIT(CONTIGO_LUCKY_DICE_TO_CONTIGO_NORTHEAST, Contigo, CONTIGO_FROM_CONTIGO_LUCKY_DICE_NORTHEAST),
    SCENE_EXIT(CONTIGO_LUCKY_DICE_TO_CONTIGO_EAST, Contigo, CONTIGO_FROM_CONTIGO_LUCKY_DICE_EAST),
    SCENE_EXIT(CONTIGO_LUCKY_DICE_TO_CONTIGO_SOUTHWEST, Contigo, CONTIGO_FROM_CONTIGO_LUCKY_DICE_SOUTHWEST),
    SCENE_EXITS_END,
};

ACTOR_PLACE Contigo_Placements[] = {
    {SCENE_TABLE_END},
};

ACTOR_PLACE Contigo_VillagePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 340, 0, 936, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 88, 0, 942, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 808, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_WIFE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 116, 0, 680, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 336, 0, 792, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 348, 0, 804, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, (s32)Contigo_DoubtingBoyScript, 352, 0, 872, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS, (s32)Contigo_GirlOnTheShipScript, 304, 0, 872, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_SALT), MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_SHINE), MOVEMENT_TYPE_STAND, 56, 0, 706, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE Contigo_GeoglyphPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 846, 0, 660, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 616, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MUSCULAR_MAN_WITH_BLOND_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 686, 0, 756, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MUSCULAR_MAN_WITH_BLOND_HAIR, CONDITION_ALWAYS, (s32)Contigo_PuzzledWorkerScript, 774, 0, 664, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MUSCULAR_MAN_WITH_BLOND_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 588, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MUSCULAR_MAN_WITH_BLOND_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 806, 0, 596, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 832, 0, 310, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 312, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 346, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 343, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 720, 0, 360, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WING_FRAME, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 720, 0, 606, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WING_FRAME, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 752, 0, 606, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WING_FRAME, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 784, 0, 622, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_METAL_FEATHER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 776, 0, 590, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_METAL_FEATHER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 808, 0, 606, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_METAL_FEATHER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 744, 0, 614, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_METAL_FEATHER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 808, 0, 622, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_METAL_FEATHER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 826, 0, 622, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WING_PART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 700, 0, 604, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WING_PART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WING_PART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 668, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WING_PART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 758, 0, 664, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WING_PART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 858, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WING_PART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 782, 0, 678, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WING_PART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 872, 0, 669, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

ACTOR_PLACE Contigo_ShopsPlacementsBeforeReunion[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 84, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_WIFE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 88, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 112, 0, 130, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 94, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 184, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_TEAL_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 448, 0, 112, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 733, 0, 91, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 686, 0, 108, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 708, 0, 154, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_RED_BOW, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 756, 0, 108, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEAPON_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 352, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_ARMOR_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 352, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_INNKEEPER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 584, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_INN_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 186, 0, 636, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 616, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COOK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 592, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MUSCULAR_MAN_WITH_BLOND_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 94, 0, 574, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MUSCULAR_MAN_WITH_BLOND_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 80, 0, 586, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 664, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 310, 0, 614, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 688, 0, 344, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE Contigo_ShopsPlacementsReunion[] = {
    ACTOR_PLACEMENT(SPRITE_HAMMA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 132, 0, 382, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 366, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 112, 0, 354, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 398, FACING_EAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE Contigo_ShopsPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_HAMMA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 158, 0, 352, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE Contigo_LuckyDicePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 840, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 840, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 92, 0, 902, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 872, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_WIFE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 90, 0, 872, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 132, 0, 906, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 840, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 840, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 300, 0, 906, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 340, 0, 906, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 512, 0, 840, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 508, 0, 906, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 548, 0, 906, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

TRIGGER Contigo_Triggers[] = {
    {TRIGGER_TABLE_END},
};

TRIGGER Contigo_VillageTriggers[] = {
    {TRIGGER_EXIT, CONTIGO_TO_WORLD_MAP, CONDITION_ALWAYS, CONTIGO_TO_WORLD_MAP},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, CONTIGO_TO_CONTIGO_SHOPS_SOUTH, CONDITION_ALWAYS, CONTIGO_TO_CONTIGO_SHOPS_SOUTH},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, CONTIGO_TO_CONTIGO_SHOPS_SOUTHWEST, CONDITION_ALWAYS, CONTIGO_TO_CONTIGO_SHOPS_SOUTHWEST},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, CONTIGO_TO_CONTIGO_LUCKY_DICE_SOUTHWEST, CONDITION_ALWAYS, CONTIGO_TO_CONTIGO_LUCKY_DICE_SOUTHWEST},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, CONTIGO_TO_CONTIGO_SHOPS_WEST, CONDITION_ALWAYS, CONTIGO_TO_CONTIGO_SHOPS_WEST},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, CONTIGO_TO_CONTIGO_LUCKY_DICE_NORTHEAST, CONDITION_ALWAYS, CONTIGO_TO_CONTIGO_LUCKY_DICE_NORTHEAST},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, CONTIGO_TO_CONTIGO_LUCKY_DICE_EAST, CONDITION_ALWAYS, CONTIGO_TO_CONTIGO_LUCKY_DICE_EAST},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, CONTIGO_TO_CONTIGO_SHOPS_EAST, CONDITION_ALWAYS, CONTIGO_TO_CONTIGO_SHOPS_EAST},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, CONTIGO_TO_CONTIGO_SHOPS_NORTHEAST_A, CONDITION_ALWAYS, CONTIGO_TO_CONTIGO_SHOPS_NORTHEAST_A},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, CONTIGO_TO_CONTIGO_SHOPS_NORTHEAST_B, CONDITION_ALWAYS, CONTIGO_TO_CONTIGO_SHOPS_NORTHEAST_B},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, CONTIGO_TO_CONTIGO_SHOPS_NORTH, CONDITION_ALWAYS, CONTIGO_TO_CONTIGO_SHOPS_NORTH},
    {TRIGGER_EXIT, CONTIGO_TO_CONTIGO_GEOGLYPH, CONDITION_ALWAYS, CONTIGO_TO_CONTIGO_GEOGLYPH},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), CELL_BEHAVIOR_TELEPORT_CIRCLE, CONDITION_ALWAYS, EVENT(scContigoTeleportOut)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEEDS_WEST_OF_ITEM_SHOP, CONDITION_ALWAYS, EVENT(scContigoScatterWeedsWestOfItemShop)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEEDS_BEHIND_INN, CONDITION_ALWAYS, EVENT(scContigoScatterWeedsBehindInn)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEEDS_OVER_BRAMBLE_SEED, CONDITION_ALWAYS, EVENT(scContigoScatterWeedsOverBrambleSeed)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEEDS_WEST_OF_WEAPON_SHOP, CONDITION_ALWAYS, EVENT(scContigoScatterWeedsWestOfWeaponShop)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEEDS_BY_GATE, CONDITION_ALWAYS, EVENT(scContigoScatterWeedsByGate)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEEDS_BELOW_LUCKY_WHEELS, CONDITION_ALWAYS, EVENT(scContigoScatterWeedsBelowLuckyWheels)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEEDS_ON_CLIFF_WEST, CONDITION_ALWAYS, EVENT(scContigoScatterWeedsOnCliffWest)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEEDS_ON_CLIFF_EAST, CONDITION_ALWAYS, EVENT(scContigoScatterWeedsOnCliffEast)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEED_PATCH_TOP_ROW, CONDITION_ALWAYS, EVENT(scContigoScatterWeedPatchTopRow)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEED_PATCH_SECOND_ROW, CONDITION_ALWAYS, EVENT(scContigoScatterWeedPatchSecondRow)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEED_PATCH_MIDDLE_ROW, CONDITION_ALWAYS, EVENT(scContigoScatterWeedPatchMiddleRow)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEED_PATCH_FOURTH_ROW, CONDITION_ALWAYS, EVENT(scContigoScatterWeedPatchFourthRow)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEED_PATCH_BOTTOM_ROW, CONDITION_ALWAYS, EVENT(scContigoScatterWeedPatchBottomRow)},
    {TRIGGER_STEP, CELL_BEHAVIOR_VIEW_WEST_OF_GATE, CONDITION_ALWAYS, EVENT(scContigoNarrowCameraBounds)},
    {TRIGGER_STEP, CELL_BEHAVIOR_VIEW_EAST_OF_GATE, CONDITION_ALWAYS, EVENT(scContigoWidenCameraBounds)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_SHINE, FLAG_SHINE_FREED, EVENT(scContigoForceFreesDjinni)},
    {TRIGGER_TALK, ACTOR_SHINE, IF_FLAG_SET(FLAG_SHINE_FREED), EVENT(scContigoFreedDjinniJoinsParty)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_ON_THE_WIND_LEGEND, FLAG_JUPITER_BEACON_LIT, EVENT(scContigoAskAboutWindLegend)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_ON_THE_WIND_LEGEND, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoAreYouLookingForIsaac)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_ON_THE_WIND_LEGEND, CONDITION_ALWAYS, MSGNO(MsgContigoIGotStuckDoingChores)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ON_THE_WIND_LEGEND, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoTheAnemosBloodInOur)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ON_THE_WIND_LEGEND, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoTheyLookLikeTheyreIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ON_THE_WIND_LEGEND, CONDITION_ALWAYS, MSGNO(MsgContigoIDidntGetToSee)},
    {TRIGGER_TALK, ACTOR_WOMAN_FORESEEING_THE_BEACON, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoTheBeaconWithinJupiterLighthouse)},
    {TRIGGER_TALK, ACTOR_WOMAN_FORESEEING_THE_BEACON, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoEveryonesLeftTownForThe)},
    {TRIGGER_TALK, ACTOR_WOMAN_FORESEEING_THE_BEACON, CONDITION_ALWAYS, MSGNO(MsgContigoEveryonesBeenGoingOnAbout)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_FORESEEING_THE_BEACON, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoSurelySomeoneWillComeSoon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_FORESEEING_THE_BEACON, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoThePropheciesAreAllComing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_FORESEEING_THE_BEACON, CONDITION_ALWAYS, MSGNO(MsgContigoSomePeopleWereTalkingAbout)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_FLYING_SHIP, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoIGuessThePeopleOf)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_FLYING_SHIP, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoIsThatYourBoatIt)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_FLYING_SHIP, CONDITION_ALWAYS, MSGNO(MsgContigoItMustBeSoExciting)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_FLYING_SHIP, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoThePowerOfTheWind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_FLYING_SHIP, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoItsJustLikeTheLegends)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_FLYING_SHIP, CONDITION_ALWAYS, MSGNO(MsgContigoICouldntFlyInThe)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_ANEMOS, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoThePeopleOfAnemosPossessed)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_ANEMOS, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoJupiterLighthouseHasBeenLit)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_ANEMOS, CONDITION_ALWAYS, MSGNO(MsgContigoHammaWasTellingUsThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_ANEMOS, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoThereWereTwoSiblingsBorn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_ANEMOS, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoWhatDoesItMeanWhen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_ANEMOS, CONDITION_ALWAYS, MSGNO(MsgContigoIHeardThatTheWorld)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_RISEN_CITY, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoWhenThePowerOfAnemos)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_RISEN_CITY, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoTheLegendsTellUsThat)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_RISEN_CITY, CONDITION_ALWAYS, MSGNO(MsgContigoIfAShipCanRise)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_RISEN_CITY, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoIThinkJupiterLighthouseWill)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_RISEN_CITY, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoTheAncientCityOfThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_RISEN_CITY, CONDITION_ALWAYS, MSGNO(MsgContigoTheCityOfTheAnemos)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_GEOGLYPHS, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoTheAnemosGeoglyphsShowA)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_GEOGLYPHS, CONDITION_ALWAYS, MSGNO(MsgContigoISawTheShipFly)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_GEOGLYPHS, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoItsHardToBelieveBut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_GEOGLYPHS, CONDITION_ALWAYS, MSGNO(MsgContigoTheShipLookedLikeIt)},
    {TRIGGER_TALK, ACTOR_BOY_DOUBTING_THE_LEGENDS, CONDITION_ALWAYS, EVENT(scContigoTalkToDoubtingBoy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_DOUBTING_THE_LEGENDS, CONDITION_ALWAYS, EVENT(scContigoTalkToDoubtingBoy)},
    {TRIGGER_TALK, ACTOR_GIRL_ON_THE_SHIP, CONDITION_ALWAYS, EVENT(scContigoTalkAboutTheShip)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ON_THE_SHIP, CONDITION_ALWAYS, EVENT(scContigoTalkAboutTheShip)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_WEED_PATCH_GAP, CONDITION_ALWAYS, EVENT(scContigoScoopForSalt)},
    {TRIGGER_RAISED, RAISED_SALT_FOUND, CONDITION_ALWAYS, EVENT(scContigoSaltJoinsParty)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_LUCKY_WHEELS_SIGN, CONDITION_ALWAYS, EVENT(scContigoReadLuckyWheelsSign)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_SUPER_LUCKY_DICE_SIGN, CONDITION_ALWAYS, EVENT(scContigoReadSuperLuckyDiceSign)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_LUCKY_DICE_SIGN, CONDITION_ALWAYS, EVENT(scContigoReadLuckyDiceSign)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_LOCKED_DOOR, CONDITION_ALWAYS, EVENT(scShowEntranceLocked)},
    {TRIGGER_TABLE_END},
};

TRIGGER Contigo_GeoglyphTriggers[] = {
    {TRIGGER_EXIT, CELL_BEHAVIOR_GEOGLYPH_WEST_EDGE, CONDITION_ALWAYS, CONTIGO_GEOGLYPH_TO_CONTIGO},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_SANCTUM_WEST_DOOR, CONDITION_ALWAYS, EVENT(scContigoEnterSanctumWestDoor)},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, CELL_BEHAVIOR_SANCTUM_MIDDLE_DOOR, CONDITION_ALWAYS, CONTIGO_GEOGLYPH_TO_ANEMOS_INNER_SANCTUM_ENTRANCE_NORTHEAST},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_SANCTUM_EAST_DOOR, CONDITION_ALWAYS, EVENT(scContigoEnterSanctumEastDoor)},
    {TRIGGER_TALK, ACTOR_VISITOR_FROM_AFAR, CONDITION_ALWAYS, MSGNO(MsgContigoIveComeALongWay)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VISITOR_FROM_AFAR, CONDITION_ALWAYS, MSGNO(MsgContigoTheGuysCarryingTheOther)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_WINGS, CONDITION_ALWAYS, MSGNO(MsgContigoTheWingsAreAlmostDone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_WINGS, CONDITION_ALWAYS, MSGNO(MsgContigoIveBeenReallyBusyLately)},
    {TRIGGER_TALK, ACTOR_WING_WORKER_A, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoTheWingsWillBeFinished)},
    {TRIGGER_TALK, ACTOR_WING_WORKER_A, CONDITION_ALWAYS, MSGNO(MsgContigoICantBelieveThatA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WING_WORKER_A, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoWhatDoWeDoIf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WING_WORKER_A, CONDITION_ALWAYS, MSGNO(MsgContigoThatFlyingShipLookedSo)},
    {TRIGGER_TALK, ACTOR_WING_WORKER_B, CONDITION_ALWAYS, MSGNO(MsgContigoWhichPartGoesWhereDoes)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WING_WORKER_B, CONDITION_ALWAYS, MSGNO(MsgContigoThereAreTooManyParts)},
    {TRIGGER_TALK, ACTOR_WING_WORKER_C, CONDITION_ALWAYS, MSGNO(MsgContigoYouveGottaWatchOutCause)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WING_WORKER_C, CONDITION_ALWAYS, MSGNO(MsgContigoWhenIStartGettingIrritated)},
    {TRIGGER_TALK, ACTOR_WING_WORKER_D, CONDITION_ALWAYS, MSGNO(MsgContigoJustAFewMoreTouches)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WING_WORKER_D, CONDITION_ALWAYS, MSGNO(MsgContigoHowLongHaveWeBeen)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_GEOGLYPH, FLAG_SET_OUT_WITH_WINGS, EVENT(scContigoAskAboutGeoglyph)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_GEOGLYPH, CONDITION_ALWAYS, MSGNO(MsgContigoLookCloselyAtTheGeoglyph)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_GEOGLYPH, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoICantEvenImagineHow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_GEOGLYPH, CONDITION_ALWAYS, MSGNO(MsgContigoWeAllCameTogetherAnd)},
    {TRIGGER_TALK, ACTOR_MAN_WHO_TRANSLATED_THE_WALL, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoThatSignHasATranslation)},
    {TRIGGER_TALK, ACTOR_MAN_WHO_TRANSLATED_THE_WALL, CONDITION_ALWAYS, MSGNO(MsgContigoImConvincedTheresSomethingHidden)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WHO_TRANSLATED_THE_WALL, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoIDontKnowIfAnyone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WHO_TRANSLATED_THE_WALL, CONDITION_ALWAYS, MSGNO(MsgContigoAllRightSoWeveFollowed)},
    {TRIGGER_TALK, ACTOR_MAN_LOCKED_OUT_OF_THE_SANCTUM, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoIWantToGetA)},
    {TRIGGER_TALK, ACTOR_MAN_LOCKED_OUT_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgContigoAllTheAmazingThingsWeve)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_LOCKED_OUT_OF_THE_SANCTUM, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoWouldntThatBeTerribleIf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_LOCKED_OUT_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgContigoIfYouTapOnThe)},
    {TRIGGER_TALK, ACTOR_MAN_ON_HAMMET, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoTheMerchantHammetTookThe)},
    {TRIGGER_TALK, ACTOR_MAN_ON_HAMMET, CONDITION_ALWAYS, MSGNO(MsgContigoHesAnOddBoyBut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_HAMMET, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoSomeoneToldMeThatHammet)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_HAMMET, CONDITION_ALWAYS, MSGNO(MsgContigoIvanWasSupposedToBe)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_CARVED_WALL, CONDITION_ALWAYS, EVENT(scContigoExamineWall)},
    {TRIGGER_TALK, ACTOR_SIGN, CONDITION_ALWAYS, EVENT(scContigoReadSign)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(scContigoRevealMiddleDoor)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(scContigoHideMiddleDoor)},
    {TRIGGER_TABLE_END},
};

TRIGGER Contigo_ShopsTriggers[] = {
    {TRIGGER_EXIT, CONTIGO_SHOPS_TO_CONTIGO_SOUTH, CONDITION_ALWAYS, CONTIGO_SHOPS_TO_CONTIGO_SOUTH},
    {TRIGGER_EXIT, CONTIGO_SHOPS_TO_CONTIGO_SOUTHWEST, CONDITION_ALWAYS, CONTIGO_SHOPS_TO_CONTIGO_SOUTHWEST},
    {TRIGGER_EXIT, CONTIGO_SHOPS_TO_CONTIGO_NORTHEAST_B, CONDITION_ALWAYS, CONTIGO_SHOPS_TO_CONTIGO_NORTHEAST_B},
    {TRIGGER_EXIT, CONTIGO_SHOPS_TO_CONTIGO_EAST, CONDITION_ALWAYS, CONTIGO_SHOPS_TO_CONTIGO_EAST},
    {TRIGGER_EXIT, CONTIGO_SHOPS_TO_CONTIGO_NORTHEAST_A, CONDITION_ALWAYS, CONTIGO_SHOPS_TO_CONTIGO_NORTHEAST_A},
    {TRIGGER_EXIT, CONTIGO_SHOPS_TO_CONTIGO_WEST, CONDITION_ALWAYS, CONTIGO_SHOPS_TO_CONTIGO_WEST},
    {TRIGGER_EXIT, CONTIGO_SHOPS_TO_ATTEKA_INLET_GEODE, CONDITION_ALWAYS, CONTIGO_SHOPS_TO_ATTEKA_INLET_GEODE},
    {TRIGGER_TALK, ACTOR_MAN_ON_IVANS_RETURN, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoTheSiblingsWhoInheritedThe)},
    {TRIGGER_TALK, ACTOR_MAN_ON_IVANS_RETURN, CONDITION_ALWAYS, MSGNO(MsgContigoIGuessTheLighthouseWas)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_IVANS_RETURN, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoTheFamiliesOfChildrenWho)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_IVANS_RETURN, CONDITION_ALWAYS, MSGNO(MsgContigoHammaWillStayWithUs)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_IVANS_MOTHER, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoImSureTheYoungerBrother)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_IVANS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgContigoIvanDoesntRememberHisMother)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_IVANS_MOTHER, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoIvansMotherLeftHimIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_IVANS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgContigoIWonderHowIvanFeels)},
    {TRIGGER_TALK, ACTOR_BOY_WHO_WOULD_FIGHT_IVAN, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoMyFatherDoesntApproveOf)},
    {TRIGGER_TALK, ACTOR_BOY_WHO_WOULD_FIGHT_IVAN, CONDITION_ALWAYS, MSGNO(MsgContigoFromTheLookOnIvans)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_WHO_WOULD_FIGHT_IVAN, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoIWishICouldHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_WHO_WOULD_FIGHT_IVAN, CONDITION_ALWAYS, MSGNO(MsgContigoSomedayImGonnaFightIvan)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_HESPERIA, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoLongAgoThePeopleOf)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_HESPERIA, CONDITION_ALWAYS, MSGNO(MsgContigoBrrrBrrrItsBeenReally)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_HESPERIA, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoForALongTimeNow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_HESPERIA, CONDITION_ALWAYS, MSGNO(MsgContigoJupiterLighthouseHasBeenCalled)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_FEELING_THE_COLD, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoALongTimeAgoThe)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_FEELING_THE_COLD, CONDITION_ALWAYS, MSGNO(MsgContigoWinterJustEndedButIt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_FEELING_THE_COLD, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoEverybodyThinksThePeopleOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_FEELING_THE_COLD, CONDITION_ALWAYS, MSGNO(MsgContigoHammaSaysThatIfIvan)},
    {TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT(scContigoTalkToItemMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoOhHowIWantTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgContigoNowThatTheWingsAreDone)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_ON_THE_SIBLINGS, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoSoonTheSiblingsWithThe)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_ON_THE_SIBLINGS, CONDITION_ALWAYS, MSGNO(MsgContigoTheLighthouseHasBeenLit)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ON_THE_SIBLINGS, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoIJustKnowIvanBecame)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ON_THE_SIBLINGS, CONDITION_ALWAYS, MSGNO(MsgContigoIvanHasThePowerOf)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_ANEMOS_POWER, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoThePowerOfTheAnemos)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_ANEMOS_POWER, CONDITION_ALWAYS, MSGNO(MsgContigoItsUpToThePeople)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_ANEMOS_POWER, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoATerribleMisfortuneFellOn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_ANEMOS_POWER, CONDITION_ALWAYS, MSGNO(MsgContigoNowThatIThinkAbout)},
    {TRIGGER_TALK, ACTOR_BABYS_BROTHER, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoIDontHaveAnyPowers)},
    {TRIGGER_TALK, ACTOR_BABYS_BROTHER, CONDITION_ALWAYS, MSGNO(MsgContigoMomAndDadHaveBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BABYS_BROTHER, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoWheneverMySisterFurrowsHer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BABYS_BROTHER, CONDITION_ALWAYS, MSGNO(MsgContigoItsTimeToFixThose)},
    {TRIGGER_TALK, ACTOR_BABY, CONDITION_ALWAYS, EVENT(scContigoBabyThinks)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BABY, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoBawabaItsHotIWant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BABY, CONDITION_ALWAYS, MSGNO(MsgContigoGooItsGettingColderGagoo)},
    {TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(scContigoTalkToWeaponMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoOfCourseTheyreHereTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgContigoIdSureLikeToSee)},
    {TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(scContigoTalkToArmorMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoNothingElseAroundHereIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgContigoWellIfThereArentGoing)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(scContigoTalkToInnkeeper)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoItsGoodToHaveThis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoOnceAllOurCustomersLeave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgContigoItsGoingToBeA)},
    {TRIGGER_TALK, ACTOR_INN_WORKER, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoAllTheWorkOnThe)},
    {TRIGGER_TALK, ACTOR_INN_WORKER, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoFirstTheLighthouseIsLit)},
    {TRIGGER_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgContigoTheInnHasBecomeSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoOnceTheWingsAreFinished)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoICantBelieveItAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgContigoTheInnIsBoundTo)},
    {TRIGGER_TALK, ACTOR_WOMAN_MAKING_THE_BEDS, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoGottaMakeAllTheBeds)},
    {TRIGGER_TALK, ACTOR_WOMAN_MAKING_THE_BEDS, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoTheBossSaidItWas)},
    {TRIGGER_TALK, ACTOR_WOMAN_MAKING_THE_BEDS, CONDITION_ALWAYS, MSGNO(MsgContigoGoshWeWereSoBusy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_MAKING_THE_BEDS, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoIveGotSoMuchTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_MAKING_THE_BEDS, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoAllTheseYearsIThought)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_MAKING_THE_BEDS, CONDITION_ALWAYS, MSGNO(MsgContigoWhatAreWeGoingTo)},
    {TRIGGER_TALK, ACTOR_COOK, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoIfIDontMakeDinner)},
    {TRIGGER_TALK, ACTOR_COOK, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoTheyveFinishedPuttingTheWings)},
    {TRIGGER_TALK, ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgContigoNowsMyChanceToExperiment)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COOK, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoIMustBeTheGreatest)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COOK, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoItWasKindOfImpressive)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgContigoSeeingThatShipTakeFlight)},
    {TRIGGER_TALK, ACTOR_WORN_OUT_WORKER, CONDITION_ALWAYS, MSGNO(MsgContigoDidYouJustArriveIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORN_OUT_WORKER, CONDITION_ALWAYS, MSGNO(MsgContigoIWorkedMyselfHalfTo)},
    {TRIGGER_TALK, ACTOR_WORKER_ON_THE_PROPHECIES, CONDITION_ALWAYS, MSGNO(MsgContigoThePropheciesOfTheAnemos)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORKER_ON_THE_PROPHECIES, CONDITION_ALWAYS, MSGNO(MsgContigoTheWingsOfAnemosAre)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_TOO_OLD_TO_HELP, CONDITION_ALWAYS, MSGNO(MsgContigoImAfraidImTooOld)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_TOO_OLD_TO_HELP, CONDITION_ALWAYS, MSGNO(MsgContigoIThrewMyBackOut)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_AWAITING_THE_MIRACLE, CONDITION_ALWAYS, MSGNO(MsgContigoWhenIsThatMiracleOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_AWAITING_THE_MIRACLE, CONDITION_ALWAYS, MSGNO(MsgContigoIWantToSeeThe)},
    {TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(scContigoTalkToHealer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, FLAG_JUPITER_BEACON_LIT, MSGNO(MsgContigoIfISeeTheWings)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgContigoPerhapsIfISeeA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgContigoItWasAShockTo)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_BEAN_SOUP, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgContigoTheresABrightRedSoup))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_CORN_BREAD, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgContigoAhTheSmellOfFresh))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_COOKIES, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgContigoContigosFamousForTheseCookies))},
    {TRIGGER_TABLE_END},
};

TRIGGER Contigo_ShopsReunionTriggers[] = {
    {TRIGGER_EXIT, CONTIGO_SHOPS_TO_CONTIGO_NORTH, CONDITION_ALWAYS, CONTIGO_SHOPS_TO_CONTIGO_NORTH},
    {TRIGGER_TALK, ACTOR_HAMMA, CONDITION_ALWAYS, MSGNO(MsgContigoNoMatterWhatHappensYou)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HAMMA, CONDITION_ALWAYS, MSGNO(MsgContigoMyHeartIsWithYou)},
    {TRIGGER_STEP, CELL_BEHAVIOR_INSIDE_HAMMAS_DOOR, FLAG_HAMMA_WELCOMED_PARTY, EVENT(scContigoReturnToNewHome)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_BEAN_SOUP, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgContigoTheresABrightRedSoup))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_CORN_BREAD, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgContigoAhTheSmellOfFresh))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_COOKIES, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgContigoContigosFamousForTheseCookies))},
    {TRIGGER_TABLE_END},
};

TRIGGER Contigo_LuckyDiceTriggers[] = {
    {TRIGGER_EXIT, CONTIGO_LUCKY_DICE_TO_CONTIGO_NORTHEAST, CONDITION_ALWAYS, CONTIGO_LUCKY_DICE_TO_CONTIGO_NORTHEAST},
    {TRIGGER_EXIT, CONTIGO_LUCKY_DICE_TO_CONTIGO_EAST, CONDITION_ALWAYS, CONTIGO_LUCKY_DICE_TO_CONTIGO_EAST},
    {TRIGGER_EXIT, CONTIGO_LUCKY_DICE_TO_CONTIGO_SOUTHWEST, CONDITION_ALWAYS, CONTIGO_LUCKY_DICE_TO_CONTIGO_SOUTHWEST},
    {TRIGGER_TALK, ACTOR_SUPER_LUCKY_DICE_ATTENDANT, CONDITION_ALWAYS, EVENT(scContigoOfferSuperLuckyDice)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUPER_LUCKY_DICE_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgContigoWhenTheyLoseTheyCall)},
    {TRIGGER_TALK, ACTOR_SUPER_LUCKY_DICE_RULES_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgContigoIfYouWantToLearn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUPER_LUCKY_DICE_RULES_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgContigoItsNotAToughGame)},
    {TRIGGER_TALK, ACTOR_SUPER_LUCKY_DICE_RULES_SIGN, CONDITION_ALWAYS, EVENT(scContigoReadSuperLuckyDiceRules)},
    {TRIGGER_TALK, ACTOR_MAN_WARY_OF_UNLUCKY_PLAYERS, CONDITION_ALWAYS, EVENT(scContigoAskAboutSuperDice)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WARY_OF_UNLUCKY_PLAYERS, CONDITION_ALWAYS, MSGNO(MsgContigoThisKidsGotUnluckyWritten)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_HI_LO_GAME, CONDITION_ALWAYS, MSGNO(MsgContigoIfYouLoseTheHi)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_HI_LO_GAME, CONDITION_ALWAYS, MSGNO(MsgContigoItsToughToKnowWhen)},
    {TRIGGER_TALK, ACTOR_WOMAN_WHO_LOSES_AT_HI_LO, CONDITION_ALWAYS, MSGNO(MsgContigoIAlmostAlwaysGetAt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WHO_LOSES_AT_HI_LO, CONDITION_ALWAYS, MSGNO(MsgContigoICanWinICan)},
    {TRIGGER_TALK, ACTOR_LUCKY_DICE_ATTENDANT, CONDITION_ALWAYS, EVENT(scContigoOfferLuckyDice)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_DICE_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgContigoWinningOrLosingIsPure)},
    {TRIGGER_TALK, ACTOR_LUCKY_DICE_RULES_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgContigoReadTheSignIfYa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_DICE_RULES_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgContigoTheRulesForLuckyDice)},
    {TRIGGER_TALK, ACTOR_LUCKY_DICE_RULES_SIGN, CONDITION_ALWAYS, EVENT(scContigoReadLuckyDiceRules)},
    {TRIGGER_TALK, ACTOR_LUCKY_DICE_PAIRS_SIGN, CONDITION_ALWAYS, EVENT(scContigoReadLuckyDicePairs)},
    {TRIGGER_TALK, ACTOR_LUCKY_WHEELS_ATTENDANT, CONDITION_ALWAYS, EVENT(scContigoTalkToWheelsAttendant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_WHEELS_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgContigoHmmIWonderIfThese)},
    {TRIGGER_TALK, ACTOR_LUCKY_WHEELS_RULES_SIGN, CONDITION_ALWAYS, EVENT(scContigoReadLuckyWheelsRules)},
    {TRIGGER_TALK, ACTOR_LUCKY_WHEELS_PRIZE_SIGN, CONDITION_ALWAYS, EVENT(scContigoReadLuckyWheelsPrizes)},
    {TRIGGER_TABLE_END},
};

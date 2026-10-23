/// @file
/// Imil, the snowbound village north of the Goma Range, near the lighthouse
/// that long ago held the Water of Hermes, and its houses and Sanctum.
/// Mia, the healer the party meets here, is of the Mercury Clan, who once
/// lived in Imil and guarded the lighthouse; she is the last of them.
///
/// An epidemic has struck the village. Until the party has beaten Saturos at
/// Mercury Lighthouse (FLAG_SATUROS_BEATEN_AT_MERCURY), by which time the
/// lighthouse is lit and the fountain at its foot flows again, the villagers
/// are sick or waiting for Mia; afterwards both scenes take their cured
/// placements and lines, and the villagers talk of Mia leaving with the
/// party. Some lines also change while she travels with them
/// (FLAG_IN_PARTY(MIA)).
///
/// - Imil (SceneId_Imil), the village. A young man shivers and sneezes in the
///   snow, waiting for Mia, and a strongman counts out his exercises against
///   the cold. Six doors (triggers 51 to 56) open into the houses, the shops,
///   the inn and the Sanctum (SceneId_ImilInteriors), IMIL_TO_WORLD_MAP_SOUTH
///   and IMIL_TO_WORLD_MAP_NORTH lead out to the world map, and the way north
///   (trigger 4) leads into Imil Falls Cave; coming back out of it
///   (IMIL_FROM_IMIL_FALLS_CAVE) the leader walks down into the village. Two
///   callbacks run while the party is here: one makes smoke rise from the
///   five houses' chimneys, staggered over each second, and one turns the
///   leader's footprints in the snow off on the frozen river that runs down
///   from the falls and on the raised ground under the big tree, and on
///   elsewhere (both drawn on places/imil/footprints.png). Stepping onto ice
///   (collision code 90) slides the leader in the held direction, turning
///   toward it as he goes, until a blocked cell or one too high stops him.
///   Move pushes the snowman only when cast from the snow, not from the river's
///   ice or the raised ground, and once it lands at cell (15, 7) it stays
///   there for the rest of the visit (a temporary flag). Searching finds 9
///   coins, a Vial and a Lucky Medal.
/// - Imil (SceneId_ImilInteriors), inside the houses, the shops and the inn; by
///   IMIL_INTERIORS_FROM_IMIL_EAST_NORTHEAST the same scene is the Imil
///   Sanctum. While the epidemic lasts the weapon and armor sellers are sick:
///   searching their counters pans the camera to them, and they say the shop is
///   closed. The item seller's daughter minds that shop for her mother. In the
///   Sanctum Mia's two apprentices keep her place: one offers healing across
///   the counter, the first time saying he can act as the party's healer
///   (FLAG_IMIL_APPRENTICE_OFFERED_HEALING), and the other asks whether the
///   party has visited the old couple who live near the entrance of town, where
///   Mia went (FLAG_IMIL_HEARD_MIA_WENT_TO_COUPLE). In the old couple's house
///   (IMIL_INTERIORS_HOUSE_SOUTHWEST) the old man moans for help, and talking
///   to him sends his wife hurrying to fetch Mia. Either sets
///   FLAG_IMIL_MIA_SENT_FOR, which changes the couple's lines and where they
///   stand.
///
///   Mia's healing is this scene's first scripted sequence. Entering the old
///   couple's house once the apprentice has told the party where Mia went,
///   before they have met her, finds her at the old man's bedside: she heals
///   him with Ply, asks who the party are, and the Mercury Lighthouse flashes.
///   `Was that... the lighthouse?` She runs out, and the party follows her
///   (FLAG_IMIL_MET_MIA); at the lighthouse she goes on with them. Afterwards
///   the old man lies in bed feeling much better, and talking or Mind Read
///   across the bed (ACTOR_OLD_MANS_BED) reaches him.
///
///   Once Saturos is beaten the sellers are well and open their shops across
///   the counters. The second sequence is Mia's farewell, the first time the
///   party then enters the Sanctum (FLAG_IMIL_MIA_SAID_FAREWELL): her
///   apprentices tell her the lighthouse has been lit, and Mia tells them she
///   will join Isaac's quest to protect the world. They promise to protect
///   Imil themselves, as members of the Mercury Clan, and she leaves the
///   village in their hands, Megan and Justin. Searching finds an Empty
///   Bottle, a Sleep Bomb and Lucky Pepper, the last from the oven with the
///   stew, whose find always answers first, so the stew's line is unused.
///
/// Open questions:
/// - XXX (no evidence): what Imil_UpdateCountdownArc and Imil_SwayAndSpark
///   were for; nothing in the game gives them to an actor, so neither the
///   ROM nor a render shows them in use.
/// - XXX (no evidence): which apprentice is Megan and which Justin; the boy
///   (ACTOR_APPRENTICE) and the girl (ACTOR_SECOND_APPRENTICE) speak in turn,
///   but Mia names them together in every edition ("Megan, Justin...", in
///   Japanese ムギ and ヒエイ), facing neither, so only the English names'
///   genders suggest the girl is Megan.

#include "action_script.h"
#include "animspr.h"
#include "version.h"
#include "game_state.h"
#include "keystate.h"
#include "map_ids.h"
#include "unit/map/imil.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "global_cells.h"
#include "types.h"
#include "text/msg_ids.h"
#include "script_layer.h"
#include "map_tables.h"
#include "field_effect.h"
#include "fx_work.h"
#include "facing_object.h"
#include "battle_effect_runtime.h"
#include "object_lookup.h"
#include "field_service.h"
#include "iwram_call.h"
#include "frame.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "fixed_math.h"
#include "palbuf.h"
#include "io_reg.h"
#include "map_grid.h"
#include "map_grid_scroll.h"
#include "macros.h"
#include "flags.h"
#include "character_ids.h"
#include "songs.h"

/// The village's actors (Imil), numbering gImil_TownPlacements' rows, and
/// gImil_TownPlacementsCured's, from ACTOR_FIRST_PLACED.
enum ImilVillageActor {
  /// A young man, shivering and sneezing.
  /// ★ Named from the game: "Brrrrr... Ah-choo!!! I'm freezing!"
  /// (MsgImilBrrrrrChooImFreezingMia)
  ACTOR_FREEZING_YOUNG_MAN = ACTOR_FIRST_PLACED,
  /// A small boy.
  /// ★ Named from the game: "Hi! Are you new in Imil?" (MsgImilHiNewInImil)
  ACTOR_GREETING_BOY,
  /// A strongman counting his exercises in the snow. "One, two, three, four..."
  /// ★ Named from the game: "You have to keep in shape if you don't want to
  /// get sick!" (MsgImilPeopleGetSickBecauseTheyStay)
  ACTOR_STRONGMAN_KEEPING_IN_SHAPE,
  /// Move pushes it.
  /// ★ Named from what the player sees: a snowman with a red cap and a green
  /// scarf by the frozen pond (its render)
  ACTOR_SNOWMAN
};

/// The actors of the houses and the Sanctum (ImilInteriors), numbering
/// gImil_InteriorPlacements' rows from ACTOR_FIRST_PLACED; the rows of Garet,
/// Ivan and Mia take those characters' own slots.
enum ImilInteriorsActor {
  /// The old couple's house.
  /// ★ Named from the game: "Have you visited the old couple who live near the
  /// entrance of town?" (MsgImilHaveVisitedOldCoupleWho)
  ACTOR_OLD_WOMAN = ACTOR_FIRST_PLACED,
  /// ★ Named from the game: "Have you visited the old couple who live near the
  /// entrance of town?" (MsgImilHaveVisitedOldCoupleWho)
  ACTOR_OLD_MAN,
  /// An unseen actor on the old man's bed: once Mia has healed him, talking or
  /// Mind Read here reaches him.
  /// ★ Named from what the player sees: the bed in the old couple's house
  /// (its render)
  ACTOR_OLD_MANS_BED,
  /// An old man who shivers while the epidemic lasts.
  /// ★ Named from the game: "The epidemic has hit almost everyone in Imil.
  /// I'm sick, too." (MsgImilCoughTheEpidemicHasHitAlmost)
  ACTOR_SICK_OLD_MAN,
  /// An old man.
  /// ★ Named from the game: "Do you want weapons?" (MsgImilDoWantWeapons)
  ACTOR_WEAPON_MERCHANT,
  /// An old man.
  /// ★ Named from the game: "Ask me anything about armor."
  /// (MsgArmorShopWelcome)
  ACTOR_ARMOR_MERCHANT,
  /// A little girl who minds the shop while her mother is sick.
  /// ★ Named from the game: "please talk to my daughter."
  /// (MsgImilIfYouWantToBuyAnything)
  /// ★ Named from the game: "Mom's sick, but I can get you whatever you
  /// need..." (MsgMercuryMomsSickGetWhatever)
  ACTOR_ITEM_MERCHANTS_DAUGHTER,
  /// A woman.
  /// ★ Named from the game: "If you want to buy anything, please talk to my
  /// daughter." (MsgImilIfYouWantToBuyAnything)
  ACTOR_ITEM_MERCHANT,
  /// ★ Named from what the player sees: the man behind the counter of the room
  /// whose attendant says it is the inn (its render)
  ACTOR_INNKEEPER,
  /// A woman who waits on the inn's guests.
  /// ★ Named from the game: "We'll wait on you hand and foot, so please stay a
  /// while!" (MsgImilWereTheOnlyInnThatStays)
  ACTOR_INN_ATTENDANT,
  /// A young man who keeps the restaurant and offers its menu.
  /// ★ Named from the game: "Do you want to see the restaurant menu?"
  /// (MsgImilDoWantSeeRestaurantMenu)
  ACTOR_RESTAURANT_OWNER,
  /// Mia's apprentices, a small boy who keeps the Sanctum and a little girl.
  /// Her farewell leaves the village in their hands, "Megan, Justin..."
  /// (MsgImilMia), without saying which is which; XXX: likely the girl is
  /// Megan and the boy Justin, as English gives those names.
  /// ★ Named from the game: "I am just her apprentice." (MsgImilLookingForMia)
  ACTOR_APPRENTICE,
  /// ★ Named from the game: "I will be a kind girl, like"
  /// (MsgImilIWillBeAKindGirl)
  ACTOR_SECOND_APPRENTICE,
  /// A crate in the old couple's house, kept under the party as altin.c keeps
  /// its ground sprites; Vault's crate (vault_interiors.c) uses the same
  /// sprite.
  /// ★ Named from what the player sees: a wooden crate by the old couple's
  /// fireplace (its render)
  ACTOR_CRATE
};

/// The village's own temporary flags, which last until the party leaves the
/// scene.
enum ImilTownFlag {
  /// Whether the leader leaves footprints in the snow, from where he stands
  /// (Imil_UpdateFootprints); the one set says which he last did.
  /// ★ Named from its area name: Imil
  FLAG_IMIL_NO_FOOTPRINTS = FLAG_SCENE_TEMP_00,
  /// ★ Named from its area name: Imil
  FLAG_IMIL_FOOTPRINTS = FLAG_SCENE_TEMP_01,
  /// Move has pushed the snowman to its place, for the rest of the visit.
  /// ★ Named from what the player sees: a snowman in a red hat and green scarf
  /// (its render)
  FLAG_IMIL_SNOWMAN_MOVED = FLAG_SCENE_TEMP_03
};

/// Imil's shops, by their numbers in the shop table.
enum { SHOP_IMIL_WEAPONS = 10, SHOP_IMIL_ARMOR = 11, SHOP_IMIL_ITEMS = 12 };

/// Imil's inn, as Engine_InnOpen numbers the inns.
enum { IMIL_INN = 4 };

/// The weapon and armor counters' trigger cells in the houses: searching one
/// facing north reaches its seller.
enum { TRIGGER_WEAPON_COUNTER = 10, TRIGGER_ARMOR_COUNTER = 11 };

// The leader faces a counter: FACING_IS_NORTH, in the two spellings the sites
// match with, the shops' from the heading read as s16 and the inn's and the
// Sanctum's from the halfword. UB: the shops' form shifts a negative heading
// left.
#ifdef UBFIX
#define IMIL_FACES_COUNTER(facing) FACING_IS_NORTH((u16)(facing))
#else
#define IMIL_FACES_COUNTER(facing)                                             \
  ((u32)(((facing) << 16) + 0x5fff0000) <= 0x3ffe0000)
#endif
#define IMIL_FACES_COUNTER_HALFWORD(facing)                                    \
  ((u16)((facing) - (FACING_NORTHWEST + 1)) <=                                 \
   FACING_NORTHEAST - FACING_NORTHWEST - 2)

// Keep native call forms while UBFIX uses the declared void contracts.
#ifdef UBFIX
#define IMIL_OPEN_SHOP Engine_ShopOpen
#else
#define IMIL_OPEN_SHOP ((s32 (*)())Engine_ShopOpen)
#endif

/// The townsfolk's own animations the scenes play, as their sheets draw them.
enum ImilAnimation {
  /// Lying in bed, only the head above the covers: the old man
  /// (SPRITE_OLD_MAN_WITH_CANE, spr_103.png) and the sick sellers.
  IMIL_ANIMATION_LIE_IN_BED = 5,
  IMIL_ANIMATION_SIT_UP = 7, ///< the old man sits up in bed
  /// The old man lies back down, then IMIL_ANIMATION_LIE_IN_BED.
  IMIL_ANIMATION_LIE_BACK = 8,
  /// The strongman's knee bends, over and over (SPRITE_STRONGMAN,
  /// spr_116.png).
  IMIL_ANIMATION_SQUATS = 9
};

static void Imil_MeetMiaAtOldCouplesHouse(void);
static void Imil_MiaCastsPlyOnOldMan(void);
static void Imil_SpawnAmbientEffects(void);
static void Imil_UpdateFootprints(void);
static void Imil_PlaceMovedSnowman(void);
static void Imil_ArriveFromFallsCave(void);
static void Imil_MiaSaysFarewellToApprentices(void);
static void Imil_RunIceSlide(void);

/// The position (x, y, z) the unused arc and sway effects move around.
extern s32 gImil_ArcOrigin[];

/// A sick person shivers: a quick squash, then a pause, longer for
/// ACTOR_SICK_OLD_MAN, whose work Imil_Initialize sets.
static s32 Imil_UpdateShiver(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  switch (actor->timer) {
  case 6:
    actor->scaleX += -Q16_ONE / 4;
    actor->scaleY += Q16_ONE / 8;
    break;
  case 4:
    actor->scaleX += Q16_ONE / 8;
    actor->scaleY -= Q16_ONE / 16;
    break;
  case 2:
    actor->scaleX += Q16_ONE / 16;
    actor->scaleY += -Q16_ONE / 32;
    break;
  case 0:
    actor->scaleX += Q16_ONE / 16;
    actor->scaleY += -Q16_ONE / 32;
    if ((s16)actor->work != 0) {
      actor->timer = (u32)Engine_RandomNext() % 40 + 40;
    } else {
      actor->timer = (u32)Engine_RandomNext() % 20 + 20;
    }
    break;
  }
  actor->timer--;
  return 1;
}

#include "common/object_facing.h"

/// Imil is two scenes: the village, and its houses and Sanctum.
const struct SceneEntrance *Imil_GetEntrances(void) {
  if (gGameState.scene == (s32)&SceneId_ImilInteriors) {
    return gImil_InteriorEntrances;
  }
  return gImil_TownEntrances;
}

const struct SceneRegion *Imil_GetRegions(void) { return NULL; }

const u32 *Imil_GetExits(void) { return gImil_Exits; }

/// Once Saturos is beaten at Mercury Lighthouse, the villagers are well again:
/// inside, the sick old man is up and wandering, and the restaurant owner
/// keeps facing the party after talking.
const struct ScenePlacement *Imil_GetPlacements(void) {
  struct ScenePlacement *script;

  if (gGameState.scene == ((s32)&SceneId_ImilInteriors)) {
    script = gImil_InteriorPlacements;
    FieldScene_PrepareActors(script);
    if (Engine_GameFlagIsSet(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
      // A byte view: written as script[row].talkFacing, the row's address is
      // formed first.
      ((u8 *)script)[(ACTOR_RESTAURANT_OWNER - ACTOR_FIRST_PLACED) *
                         sizeof(struct ScenePlacement) +
                     offsetof(struct ScenePlacement, talkFacing)] =
          TALK_FACE_PARTY;
      script[ACTOR_SICK_OLD_MAN - ACTOR_FIRST_PLACED].x = PIXELS(182);
      script[ACTOR_SICK_OLD_MAN - ACTOR_FIRST_PLACED].z = PIXELS(564);
      script[ACTOR_SICK_OLD_MAN - ACTOR_FIRST_PLACED].behavior = ACTOR_WANDER;
    }
    return script;
  }

  if (Engine_GameFlagIsSet(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
    return gImil_TownPlacementsCured;
  }
  return gImil_TownPlacements;
}

/// Stepping onto the ice in Imil (collision code 90) starts a slide.
static void Imil_StartIceSlide(void) { Imil_RunIceSlide(); }

/// The village's, or the houses' and the Sanctum's, conversations and
/// Psynergy targets.
const struct SceneEvent *Imil_GetEvents(void) {
  if (gGameState.scene == (s32)&SceneId_ImilInteriors) {
    return gImil_InteriorEvents;
  }
  return gImil_TownEvents;
}

/// In Imil, until Saturos is beaten at Mercury Lighthouse: the freezing young
/// man sneezes and shivers, waiting for Mia; once she travels with the party,
/// he says that even her healing can't help them all.
static void Imil_TalkToFreezingYoungMan(void) {
  Engine_EventStart();
  if (Engine_GameFlagIsSet(FLAG_IN_PARTY(MIA)) != 0) {
    Engine_EventSetMessage(MSGNO(MsgImilEvenIfMiaHealsUs));
  } else {
    Engine_EventSetMessage(MSGNO(MsgImilBrrrrrChooImFreezingMia));
  }
  {
    s32 val = 0;
    s32 speaker = ACTOR_FREEZING_YOUNG_MAN;
    Engine_EventShowMessage(speaker, val);
  }
  Engine_EventEnd();
}

/// Mind Read on the freezing young man in Imil, until Saturos is beaten: Mia
/// should be here by now, or, once she has joined the party, he can't
/// understand why anyone would exercise in the cold.
static void Imil_ReadFreezingYoungMansMind(void) {
  Engine_EventStart();
  if (Engine_GameFlagIsSet(FLAG_IN_PARTY(MIA)) != 0) {
    Engine_EventSetMessage(MSGNO(MsgImilCantUnderstandWhyAnyOne));
  } else {
    Engine_EventSetMessage(MSGNO(MsgImilMiaShouldHereByNow));
  }
  Engine_EventShowMessage(ACTOR_FREEZING_YOUNG_MAN, 0);
  Engine_EventEnd();
}

/// A small boy in Imil, until Saturos is beaten, turns to the leader and asks
/// whether he is new in Imil.
static void Imil_GreetVisitor(void) {
  Engine_EventStart();
  Engine_ActorFaceActor(ACTOR_GREETING_BOY, ACTOR_PARTY_LEADER, 10);
  Engine_EventSetMessage(MSGNO(MsgImilHiNewInImil));
  Engine_EventAskYesNo(ACTOR_GREETING_BOY, 0);
  Engine_EventEnd();
}

/// The strongman who keeps in shape pauses his exercises to talk. He stops,
/// turns to the leader and goes back to counting; once Saturos is beaten, he
/// wonders what he trained so hard for.
static void Imil_TalkToStrongmanKeepingInShape(void) {
  Engine_EventStart();

  if (GameFlag_IsSet(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
    Engine_EventSetMessage(MSGNO(MsgImilCountingAfterDeparture));
    Event_ShowMessage(ACTOR_STRONGMAN_KEEPING_IN_SHAPE, 0);
    Actor_SetAttachedEffect(ACTOR_STRONGMAN_KEEPING_IN_SHAPE,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Engine_EventWait(40);
    Engine_ActorSetAnimation(ACTOR_STRONGMAN_KEEPING_IN_SHAPE, ANIMATION_STAND);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_STRONGMAN_KEEPING_IN_SHAPE, ACTOR_PARTY_LEADER, 20);
    Event_AskYesNo(ACTOR_STRONGMAN_KEEPING_IN_SHAPE, 0);
    Actor_FaceDirection(ACTOR_STRONGMAN_KEEPING_IN_SHAPE,
                        FACING_SOUTHEAST + FACING_STEP, 10);
    Engine_ActorSetAnimation(ACTOR_STRONGMAN_KEEPING_IN_SHAPE,
                             IMIL_ANIMATION_SQUATS);
  } else {
    Engine_EventSetMessage(MSGNO(MsgImilOneTwoThreeFour));
    Event_ShowMessage(ACTOR_STRONGMAN_KEEPING_IN_SHAPE, 0);
    Actor_SetAttachedEffect(ACTOR_STRONGMAN_KEEPING_IN_SHAPE,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Engine_EventWait(40);
    Engine_ActorSetAnimation(ACTOR_STRONGMAN_KEEPING_IN_SHAPE, ANIMATION_STAND);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_STRONGMAN_KEEPING_IN_SHAPE, ACTOR_PARTY_LEADER, 20);
    Event_ShowMessage(ACTOR_STRONGMAN_KEEPING_IN_SHAPE, 0);
    Actor_FaceDirection(ACTOR_STRONGMAN_KEEPING_IN_SHAPE,
                        FACING_SOUTHEAST + FACING_STEP, 10);
    Engine_ActorSetAnimation(ACTOR_STRONGMAN_KEEPING_IN_SHAPE,
                             IMIL_ANIMATION_SQUATS);
  }

  Engine_EventEnd();
}

/// The old man in the old couple's house moans for help, and his wife hurries
/// to fetch Mia (FLAG_IMIL_MIA_SENT_FOR); after Mia's healing he feels much
/// better, and once Saturos is beaten he asks whether the party really thought
/// he would die.
static void Imil_TalkToOldMan(void) {

  struct FieldActor *oldMan;
  struct FieldActor *oldWoman;

  if (GameFlag_IsSet(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
    Engine_EventStart();
    Actor_FaceActor(ACTOR_OLD_MAN, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(10);
    Engine_EventSetMessage(MsgMercuryReallySayDie);
    Event_AskYesNo(ACTOR_OLD_MAN, 0);
    Engine_EventEnd();
  } else {
    if (GameFlag_IsSet(FLAG_IMIL_MET_MIA) != 0) {
      Engine_EventStart();
      Engine_ActorSetAnimation(ACTOR_OLD_MAN, IMIL_ANIMATION_SIT_UP);
      Engine_MapAnimateCells(gImil_CellStepsA, 10, 69);
      Engine_EventSetMessage(MsgMercuryFeelMuchBetter);
      Event_ShowMessage(ACTOR_OLD_MAN, 0);
      Engine_ActorSetAnimation(ACTOR_OLD_MAN, IMIL_ANIMATION_LIE_BACK);
      Map_AnimateCells(gImil_CellStepsB, 10, 69);
      Engine_EventEnd();
    } else {
      Engine_EventStart();
      oldMan = Object_GetById(ACTOR_OLD_MAN);
      oldMan->timer = 10;
      Actor_SetActionScript(ACTOR_OLD_MAN, gImil_ShiverScript);
      Engine_EventSetMessage(MsgMercuryOoohHelpMe);
      Engine_EventShowMessage(ACTOR_OLD_MAN, 0);
      Engine_ActorStop(ACTOR_OLD_WOMAN);
      Actor_ShowEmote(ACTOR_OLD_WOMAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
      Actor_FaceDirection(ACTOR_OLD_WOMAN, FACING_NORTH + FACING_STEP, 10);
      Engine_ActorStartRepeatedMotion(ACTOR_OLD_WOMAN, 2);
      Event_ShowMessageAndWait(ACTOR_OLD_WOMAN, 0, 20);
      Actor_SetActionScript(ACTOR_PARTY_LEADER, gImil_LeaderStepAsideScript);
      Actor_SetSpeed(ACTOR_OLD_WOMAN, PIXELS(8) / 5, PIXELS(4) / 5);
      Actor_RunActionScriptAndWait(ACTOR_OLD_WOMAN, gImil_OldWomanHurryScript);
      Engine_EventWait(40);
      Engine_ActorJump(ACTOR_OLD_WOMAN, 2, 0);
      Engine_ActorStartRepeatedMotion(ACTOR_OLD_WOMAN, 2);
      Engine_ActorSetAttachedEffect(ACTOR_OLD_WOMAN, ATTACHED_EFFECT_IN_FRONT |
                                                         ATTACHED_EFFECT_ONCE);
      Engine_EventWait(60);
      Event_ShowMessageAndWait(ACTOR_OLD_WOMAN, 0, 10);
      Actor_FaceDirection(ACTOR_OLD_WOMAN, FACING_SOUTHEAST + FACING_STEP, 20);
      Engine_ActorStartRepeatedMotion(ACTOR_OLD_WOMAN, 2);
      Engine_EventShowMessage(ACTOR_OLD_WOMAN, 0);
      oldWoman = Object_GetById(ACTOR_OLD_WOMAN);
      oldWoman->collisionFlags ^= 0x2;
      GameFlag_Set(FLAG_IMIL_MIA_SENT_FOR);
      Engine_EventEnd();
    }
  }
}

/// Talking to the weapon seller of Imil, until Saturos is beaten: he asks
/// whether the party wants weapons, cannot sell them until he is cured, and
/// blames the travellers from the south for the disease.
static void Imil_AskAboutWeapons(void) {
  Engine_EventStart();
  Engine_EventSetMessage(MSGNO(MsgImilDoWantWeapons));
  Engine_EventAskYesNo(ACTOR_WEAPON_MERCHANT, 0);
  Engine_EventEnd();
}

/// The restaurant owner of Imil, a young man, until Saturos is beaten, offers
/// the menu.
static void Imil_AskAboutRestaurantMenu(void) {
  Engine_EventStart();
  Engine_EventSetMessage(MSGNO(MsgImilDoWantSeeRestaurantMenu));
  Engine_EventAskYesNo(ACTOR_RESTAURANT_OWNER, 0);
  Engine_EventEnd();
}

/// Mia's second apprentice asks whether the party has visited the old couple.
/// The apprentice is in the Imil Sanctum until Saturos is beaten, and the old
/// couple's house is where Mia went (FLAG_IMIL_HEARD_MIA_WENT_TO_COUPLE, and
/// FLAG_IMIL_MIA_SENT_FOR); once Mia has joined the party, the apprentice says
/// everyone counts on her.
static void Imil_TalkToSecondApprentice(void) {
  Engine_EventStart();
  if (Engine_GameFlagIsSet(FLAG_IN_PARTY(MIA)) != 0) {
    Engine_EventSetMessage(MSGNO(MsgImilEveryoneCountsOnMiaThats));
    Engine_EventShowMessage(ACTOR_SECOND_APPRENTICE, 0);
  } else {
    Engine_EventSetMessage(MsgImilHaveVisitedOldCoupleWho);
    Engine_EventAskYesNo(ACTOR_SECOND_APPRENTICE, 0);
    Engine_GameFlagSet(FLAG_IMIL_HEARD_MIA_WENT_TO_COUPLE);
    Engine_GameFlagSet(FLAG_IMIL_MIA_SENT_FOR);
  }
  Engine_EventEnd();
}

/// Mind Read on the second apprentice in the Imil Sanctum, until Saturos is
/// beaten: Mia had a bad feeling, or, once she has joined, they must help her.
static void Imil_ReadSecondApprenticeMind(void) {
  Engine_EventStart();
  if (Engine_GameFlagIsSet(FLAG_IN_PARTY(MIA)) != 0) {
    Engine_EventSetMessage(MSGNO(MsgImilWeHaveDoWhateverWe));
  } else {
    Engine_EventSetMessage(MSGNO(MsgImilMiaWasSayingSheHas));
  }
  Engine_EventShowMessage(ACTOR_SECOND_APPRENTICE, 0);
  Engine_EventEnd();
}

/// The old woman of the old couple asks what happened at the lighthouse. She's
/// in their house after Mia's healing and until Saturos is beaten.
static void Imil_TalkToOldWoman(void) {
  Engine_EventStart();
  Engine_EventSetMessage(MSGNO(MsgImilHappenedInLighthouseNortheast));
  Event_ShowMessage(ACTOR_OLD_WOMAN, 0);
  Actor_FaceDirection(ACTOR_OLD_WOMAN, FACING_SOUTHEAST + FACING_STEP, 10);
  Engine_EventEnd();
}

/// Mind Read on the old woman of the old couple's house, until Saturos is
/// beaten: about Mia's clan, her husband's exaggerating, or Mia running all
/// over town.
static void Imil_ReadOldWomanMind(void) {
  Engine_EventStart();
  if (Engine_GameFlagIsSet(FLAG_IMIL_MET_MIA) != 0) {
    Engine_EventSetMessage(MSGNO(MsgImilMiaClanOnceLivedHere));
  } else if (Engine_GameFlagIsSet(FLAG_IMIL_MIA_SENT_FOR) != 0) {
    Engine_EventSetMessage(MSGNO(MsgImilHesAlwaysExaggeratingThingsBut));
  } else {
    Engine_EventSetMessage(MSGNO(MsgImilMiaRunningAroundTownCaring));
  }
  Engine_EventShowMessage(ACTOR_OLD_WOMAN, 0);
  Engine_EventEnd();
}

/// After Mia's visit, speaking across the bed in the old couple's house
/// reaches the old man.
static void Imil_TalkToOldManInBed(void) { Imil_TalkToOldMan(); }

/// After Mia's visit, Mind Read across the bed in the old couple's house
/// reaches the old man: he admires Mia, and wishes he had a daughter like her.
static void Imil_ReadOldManInBedMind(void) {
  Engine_EventStart();
  Engine_EventSetMessage(MSGNO(MsgImilMiaIsAGoodGirlI));
  Engine_EventShowMessage(ACTOR_OLD_MANS_BED, 0);
  Engine_EventEnd();
}

/// Mind Read on Mia's apprentice in the Imil Sanctum, until Saturos is beaten:
/// he thinks the party are not bad people, or, once Mia has joined, he feels
/// grown up serving as a healer.
static void Imil_ReadApprenticeMind(void) {
  Engine_EventStart();
  if (Engine_GameFlagIsSet(FLAG_IN_PARTY(MIA)) != 0) {
    Engine_EventSetMessage(MSGNO(MsgImilFeelLikeGrownUpWhen));
  } else {
    Engine_EventSetMessage(MSGNO(MsgImilTheseFolkOkayTheyDont));
  }
  Engine_EventShowMessage(ACTOR_APPRENTICE, 0);
  Engine_EventEnd();
}

/// Walking north into one of Imil's six doors (triggers 51 to 56): everyone
/// stops, the door opens and the leader walks in; the Sanctum's door
/// (IMIL_TO_IMIL_INTERIORS_NORTHEAST) has its own sound and a slower walk.
static void Imil_OpenDoorAndExit(void) {
  struct EventWork *work;
  struct FieldActor *actor;
  u32 i;
  s32 exit;

  work = gEventWork;
  Engine_EventStart();
  for (i = ACTOR_FIRST_PLACED; i <= 65; i++) {
    actor = Object_GetById(i);
    if (actor != NULL) {
      actor->motionFlags = 0;
    }
  }
  exit = (s16)(work->touchedTrigger - 50);
  if (exit == IMIL_TO_IMIL_INTERIORS_NORTHEAST) {
    Engine_AudioPlayCue(SE_188);
  } else {
    Engine_AudioPlayCue(SE_DOOR_OPEN);
  }
  {
    s32 x = gImil_DoorCellPoints[exit - 1][0];
    s32 y = gImil_DoorCellPoints[exit - 1][1];

    Engine_MapAnimateCells(gImil_DoorCellSteps[exit - 1], x, y);
  }
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (exit == IMIL_TO_IMIL_INTERIORS_NORTHEAST) {
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 5, PIXELS(1) / 10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_WALK);
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
  } else {
    Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
  }
  Engine_EventWait(16);
  Engine_EventRequestExit(exit);
  Engine_EventEnd();
}

/// Imil's weapon shop. Once Saturos is beaten, talking to the seller across
/// the counter, the leader facing north, opens the shop, and from elsewhere he
/// is eager for business. Before, searching the counter from the same side
/// pans the camera to him, sick, and he says the shop is closed.
static void Imil_TalkToWeaponMerchant(void) {
  s32 facing;

  facing = (s16)Object_GetById(ACTOR_PARTY_LEADER)->facing;
  if (Engine_GameFlagIsSet(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
    if (IMIL_FACES_COUNTER(facing)) {
      Engine_ShopOpen(SHOP_IMIL_WEAPONS, ACTOR_WEAPON_MERCHANT);
      return;
    }
    Engine_EventStart();
    Engine_ActorFaceActor(ACTOR_WEAPON_MERCHANT, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(10);
    Engine_EventSetMessage(MsgMercuryCatchUpLostOpportunity);
    Engine_EventShowMessage(ACTOR_WEAPON_MERCHANT, 0);
    Actor_FaceDirection(ACTOR_WEAPON_MERCHANT, FACING_SOUTH, 10);
    Engine_EventEnd();
  } else {
    if (IMIL_FACES_COUNTER(facing)) {
      Engine_EventStart();
      Engine_CameraSetSpeed(PIXELS(6), PIXELS(3) / 4);
      Camera_MoveTo(PIXELS(426), -1, PIXELS(492), 1);
      Engine_CameraWaitForMove();
      Engine_EventWait(20);
      Actor_RunActionScriptAndWait(ACTOR_WEAPON_MERCHANT, gImil_BobScript);
      Engine_EventSetMessage(MsgMercuryStoreClosedUntilWell);
      Engine_EventShowMessage(ACTOR_WEAPON_MERCHANT, 0);
      Engine_CameraMoveTo(PIXELS(426), -1, PIXELS(616), 1);
      Engine_CameraWaitForMove();
      Engine_EventEnd();
    }
  }
}

/// Imil's armor shop, as the weapon shop: once Saturos is beaten the seller
/// opens across the counter; before, searching the counter pans to him, too
/// sick for business.
static void Imil_TalkToArmorMerchant(void) {
  s32 facing;

  facing = (s16)Object_GetById(ACTOR_PARTY_LEADER)->facing;
  if (Engine_GameFlagIsSet(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
    if (IMIL_FACES_COUNTER(facing)) {
      Engine_ShopOpen(SHOP_IMIL_ARMOR, ACTOR_ARMOR_MERCHANT);
      return;
    }
    Engine_EventStart();
    Engine_ActorFaceActor(ACTOR_ARMOR_MERCHANT, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(10);
    Engine_EventSetMessage(MsgMercuryShameStoreClosed);
    Engine_EventShowMessage(ACTOR_ARMOR_MERCHANT, 0);
    Actor_FaceDirection(ACTOR_ARMOR_MERCHANT, FACING_SOUTH, 10);
    Engine_EventEnd();
  } else {
    if (IMIL_FACES_COUNTER(facing)) {
      Engine_EventStart();
      Engine_CameraSetSpeed(PIXELS(6), PIXELS(3) / 4);
      Camera_MoveTo(PIXELS(426), -1, PIXELS(492), 1);
      Engine_CameraWaitForMove();
      Engine_EventWait(20);
      Actor_RunActionScriptAndWait(ACTOR_ARMOR_MERCHANT, gImil_BobScript);
      Engine_EventSetMessage(MsgMercuryTooSickBusiness);
      Engine_EventShowMessage(ACTOR_ARMOR_MERCHANT, 0);
      Engine_CameraMoveTo(PIXELS(426), -1, PIXELS(616), 1);
      Engine_CameraWaitForMove();
      Engine_EventEnd();
    }
  }
}

/// Imil's item shop: once Saturos is beaten the mother sells again; before,
/// her daughter minds the counter while she is sick, or, away from it, talks
/// of her father, a lumberjack working in Kolima.
static void Imil_TalkToItemMerchant(void) {
  s32 facing;

  facing = (s16)Object_GetById(ACTOR_PARTY_LEADER)->facing;
  if (Engine_GameFlagIsSet(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
    if (IMIL_FACES_COUNTER(facing)) {
      Engine_ShopOpen(SHOP_IMIL_ITEMS, ACTOR_ITEM_MERCHANT);
      return;
    }
    Engine_EventStart();
    Engine_ActorFaceActor(ACTOR_ITEM_MERCHANT, ACTOR_PARTY_LEADER, 0);
    Engine_EventSetMessage(MsgMercuryRecoveredThanksFountain);
    Engine_EventShowMessage(ACTOR_ITEM_MERCHANT, 0);
    Actor_FaceDirection(ACTOR_ITEM_MERCHANT, FACING_SOUTH, 0);
    Engine_EventEnd();
  } else {
    if (IMIL_FACES_COUNTER(facing)) {
      Engine_EventStart();
      Engine_EventSetMessage(MsgMercuryMomsSickGetWhatever);
      Engine_EventShowMessage(ACTOR_ITEM_MERCHANTS_DAUGHTER, 0);
      /* FAKEMATCH(wrapper): called through a cast to the s32 call the bytes
       * show, which loads r1 first; as a call by its void prototype, 1 of 70
       * instructions differ in tbs-en. */
      IMIL_OPEN_SHOP(SHOP_IMIL_ITEMS, ACTOR_ITEM_MERCHANTS_DAUGHTER);
      Engine_EventEnd();
    } else {
      Engine_ActorFaceActor(ACTOR_ITEM_MERCHANTS_DAUGHTER, ACTOR_PARTY_LEADER,
                            10);
      Engine_EventSetMessage(MsgMercuryDadLumberjack);
      Engine_EventShowMessage(ACTOR_ITEM_MERCHANTS_DAUGHTER, 0);
      Actor_FaceDirection(ACTOR_ITEM_MERCHANTS_DAUGHTER,
                          FACING_SOUTH + FACING_STEP, 10);
    }
  }
}

/// Across the counter of Imil's inn, the leader facing north, the inn opens;
/// otherwise the innkeeper wonders why two groups of travellers came through
/// the snow, or, once Saturos is beaten, talks of the party's leaving.
static void Imil_TalkToInnkeeper(void) {
  struct FieldActor *leader;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (IMIL_FACES_COUNTER_HALFWORD(leader->facing)) {
    Engine_InnOpen(IMIL_INN, ACTOR_INNKEEPER);
  } else {
    Engine_EventStart();
    Actor_FaceActor(ACTOR_INNKEEPER, ACTOR_PARTY_LEADER, 10);
    if (GameFlag_IsSet(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
      Engine_EventSetMessage(MSGNO(MsgImilItsAlmostTimeForLeave));
      Event_AskYesNo(ACTOR_INNKEEPER, 0);
    } else {
      Engine_EventSetMessage(MSGNO(MsgImilWhyHaveTwoGroupsTravelers));
      Event_ShowMessage(ACTOR_INNKEEPER, 0);
    }
    Actor_FaceDirection(ACTOR_INNKEEPER, FACING_SOUTHEAST + FACING_STEP, 10);
    Engine_EventEnd();
  }
}

/// Mia's apprentice offers the Sanctum's services across its counter. He does
/// so when the leader faces north across the counter of the Imil Sanctum, the
/// first time saying he can act as healer; otherwise he talks of Mia.
static void Imil_TalkToApprentice(void) {
  struct FieldActor *leader;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (IMIL_FACES_COUNTER_HALFWORD(leader->facing)) {
    Engine_EventStart();
    if (GameFlag_IsSet(FLAG_IMIL_APPRENTICE_OFFERED_HEALING) == 0) {
      Engine_EventSetMessage(MSGNO(MsgImilMayOnlyStudentButCan));
      Event_ShowMessage(ACTOR_APPRENTICE, 0);
      GameFlag_Set(FLAG_IMIL_APPRENTICE_OFFERED_HEALING);
    }
    Engine_EventEnd();
    Engine_SanctumOpen(ACTOR_APPRENTICE);
  } else {
    Engine_EventStart();
    if (GameFlag_IsSet(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
      Engine_EventSetMessage(MSGNO(MsgImilMiaGoingOnJourneyWith));
      Event_ShowMessage(ACTOR_APPRENTICE, 0);
    } else if (GameFlag_IsSet(FLAG_IN_PARTY(MIA)) != 0) {
      Engine_EventSetMessage(MSGNO(MsgImilAmHealerWhileMiaOut));
      Event_ShowMessage(ACTOR_APPRENTICE, 0);
    } else {
      Engine_EventSetMessage(MSGNO(MsgImilLookingForMia));
      (void)Event_AskYesNo(ACTOR_APPRENTICE, 0);
      Actor_FaceDirection(ACTOR_APPRENTICE, FACING_SOUTHEAST + FACING_STEP, 10);
    }
    Engine_EventEnd();
  }
}

/// Sets up the scene as the party enters. The village gets its rising puffs,
/// map layers and snowman, and coming from Imil Falls Cave the leader walks
/// in; inside, each room restores its people for the village's state, and the
/// old couple's house and the Sanctum start Mia's healing and her farewell.
s32 Imil_Initialize(void) {
  struct FieldActor *leader;
  s32 entrance;

  if (gGameState.scene == (s32)&SceneId_Imil) {
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Engine_ActorSetAnimation(ACTOR_STRONGMAN_KEEPING_IN_SHAPE,
                             IMIL_ANIMATION_SQUATS);
    if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION)) {
      Engine_GameFlagClear(FLAG_IMIL_NO_FOOTPRINTS);
      Engine_GameFlagClear(FLAG_IMIL_FOOTPRINTS);
    }
    leader->timer = 0;
    leader->work = 0;
    Task_AddCallback(Imil_SpawnAmbientEffects, TASK_PRIORITY_SCENE);
    FieldScheduler_AddCallback(Imil_UpdateFootprints, TASK_PRIORITY_SCENE);
    Engine_ActorSetSpritePriority(ACTOR_SNOWMAN, 1);
    if (Engine_GameFlagIsSet(FLAG_IMIL_SNOWMAN_MOVED)) {
      Imil_PlaceMovedSnowman();
    }
    if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) &&
        gGameState.entrance == IMIL_FROM_IMIL_FALLS_CAVE) {
      Imil_ArriveFromFallsCave();
    }
  } else if (gGameState.scene == (s32)&SceneId_ImilInteriors) {
    // Restore each interior's occupants before running its arrival event.
    gEventWork->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    entrance = gGameState.entrance;
    if (entrance == IMIL_INTERIORS_HOUSE_SOUTHWEST) {
      Engine_ActorSetChildValue(ACTOR_CRATE, 15);
      Object_GetById(ACTOR_CRATE)->collisionFlags |= 8;
      Engine_ActorSetSpritePriority(ACTOR_CRATE, 1);
      if (Engine_GameFlagIsSet(FLAG_SATUROS_BEATEN_AT_MERCURY)) {
        Map_CopyCellAttributes(10, 7, 1, 1, 10, 8);
        Map_CopyCellsTo(3, 125, 9, 69, 3, 3);
        Engine_MapRedraw();
        Engine_TaskWait(1);
        Actor_SetBuiltinAction(ACTOR_OLD_WOMAN, 2);
        Engine_ActorSetPosition(ACTOR_OLD_MANS_BED, 0, 0);
      } else if (Engine_GameFlagIsSet(FLAG_IMIL_MIA_SENT_FOR) &&
                 Engine_GameFlagIsSet(FLAG_IMIL_HEARD_MIA_WENT_TO_COUPLE)) {
        Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_OLD_MANS_BED), 0);
        Engine_ActorSetPosition(ACTOR_OLD_MAN, PIXELS(174), PIXELS(164));
        Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_OLD_MAN), 0);
        Engine_ActorSetAnimation(ACTOR_OLD_MAN, IMIL_ANIMATION_LIE_IN_BED);
        Actor_SetPosition(ACTOR_OLD_WOMAN, PIXELS(168), PIXELS(152));
        Object_GetById(ACTOR_OLD_WOMAN)->facing =
            FACING_SOUTHEAST + FACING_STEP;
        if (!Engine_GameFlagIsSet(FLAG_IMIL_MET_MIA)) {
          Imil_MeetMiaAtOldCouplesHouse();
        }
      } else {
        Map_CopyCellAttributes(10, 7, 1, 1, 10, 8);
        Map_CopyCellsTo(3, 125, 9, 69, 3, 3);
        Engine_MapRedraw();
        Engine_TaskWait(1);
        if (Engine_GameFlagIsSet(FLAG_IMIL_MIA_SENT_FOR)) {
          Engine_ActorSetPosition(ACTOR_OLD_WOMAN, PIXELS(149), PIXELS(116));
          Object_GetById(ACTOR_OLD_WOMAN)->facing = 0;
          Object_GetById(ACTOR_OLD_MAN)->work = 0;
          Actor_SetActionScript(ACTOR_OLD_MAN, gImil_ShiverScript);
        } else {
          Actor_SetBuiltinAction(ACTOR_OLD_WOMAN, 2);
        }
      }
    } else if (entrance == IMIL_INTERIORS_FROM_IMIL_WEST_SOUTHWEST) {
      if (!Engine_GameFlagIsSet(FLAG_SATUROS_BEATEN_AT_MERCURY)) {
        Object_GetById(ACTOR_SICK_OLD_MAN)->work = 1;
        Actor_SetActionScript(ACTOR_SICK_OLD_MAN, gImil_ShiverScript);
      }
    } else if (entrance == IMIL_INTERIORS_HOUSE_NORTHEAST) {
      if (Engine_GameFlagIsSet(FLAG_SATUROS_BEATEN_AT_MERCURY)) {
        Actor_SetPosition(ACTOR_WEAPON_MERCHANT, PIXELS(364), PIXELS(578));
        Engine_ActorSetSpritePriority(ACTOR_WEAPON_MERCHANT, 2);
        Object_GetById(ACTOR_WEAPON_MERCHANT)->collisionFlags |= 4;
        Map_CopyCellsTo(6, 125, 22, 88, 3, 3);
        Actor_SetPosition(ACTOR_ARMOR_MERCHANT, PIXELS(492), PIXELS(578));
        Engine_ActorSetSpritePriority(ACTOR_ARMOR_MERCHANT, 2);
        Object_GetById(ACTOR_ARMOR_MERCHANT)->collisionFlags |= 4;
        Map_CopyCellsTo(9, 125, 28, 88, 3, 3);
      } else {
        Object_GetById(ACTOR_WEAPON_MERCHANT)->scaleX = -Q16_ONE;
        Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_WEAPON_MERCHANT), 0);
        Engine_ActorSetAnimation(ACTOR_WEAPON_MERCHANT,
                                 IMIL_ANIMATION_LIE_IN_BED);
        Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_ARMOR_MERCHANT), 0);
        Engine_ActorSetAnimation(ACTOR_ARMOR_MERCHANT,
                                 IMIL_ANIMATION_LIE_IN_BED);
      }
    } else if (entrance == IMIL_INTERIORS_HOUSE_WEST) {
      if (Engine_GameFlagIsSet(FLAG_SATUROS_BEATEN_AT_MERCURY)) {
        Actor_SetPosition(ACTOR_ITEM_MERCHANT, PIXELS(460), PIXELS(258));
        Engine_ActorSetSpritePriority(ACTOR_ITEM_MERCHANT, 2);
        Object_GetById(ACTOR_ITEM_MERCHANT)->collisionFlags |= 4;
        Engine_ActorSetPosition(ACTOR_ITEM_MERCHANTS_DAUGHTER, PIXELS(408),
                                PIXELS(264));
        Object_GetById(ACTOR_ITEM_MERCHANTS_DAUGHTER)->facing =
            FACING_EAST + FACING_STEP;
        Map_CopyCellsTo(12, 125, 26, 70, 3, 3);
      } else {
        Actor_SetPosition(ACTOR_ITEM_MERCHANTS_DAUGHTER, PIXELS(460),
                          PIXELS(258));
        Engine_ActorSetSpritePriority(ACTOR_ITEM_MERCHANTS_DAUGHTER, 2);
        Object_GetById(ACTOR_ITEM_MERCHANTS_DAUGHTER)->collisionFlags |= 4;
        Object_GetById(ACTOR_ITEM_MERCHANT)->scaleX = -Q16_ONE;
        Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_ITEM_MERCHANT), 0);
        Engine_ActorSetAnimation(ACTOR_ITEM_MERCHANT,
                                 IMIL_ANIMATION_LIE_IN_BED);
      }
    } else if (entrance == IMIL_INTERIORS_FROM_IMIL_EAST_NORTHEAST) {
      if (Engine_GameFlagIsSet(FLAG_SATUROS_BEATEN_AT_MERCURY)) {
        Object_GetById(ACTOR_SECOND_APPRENTICE)->facing =
            FACING_SOUTHEAST + FACING_STEP;
        if (!Engine_GameFlagIsSet(FLAG_IMIL_MIA_SAID_FAREWELL)) {
          Actor_SetPosition(ACTOR_SECOND_APPRENTICE, PIXELS(650), PIXELS(161));
          Imil_MiaSaysFarewellToApprentices();
        } else {
          Actor_SetPosition(ACTOR_SECOND_APPRENTICE, PIXELS(644), PIXELS(166));
        }
      }
    }
  }
  return 0;
}

/// Mia's healing, in the old couple's house in Imil: after hearing where Mia
/// went, the party finds her at the old man's bedside. She heals him with Ply,
/// the Mercury Lighthouse flashes, and Mia runs out; the party follows her.
static void Imil_MeetMiaAtOldCouplesHouse(void) {
  struct FieldActor *leader;
  const s32 *table;
  struct EventWork *work;

  Engine_EventStart();
  Actor_SetPosition(MIA, PIXELS(182), PIXELS(150));
  Camera_MoveTo(PIXELS(141), -1, PIXELS(221), 0);
  Engine_TaskWait(1);
  Camera_SetSpeed(PIXELS(3) / 10, PIXELS(3) / 80);
  Camera_MoveTo(PIXELS(140), -1, PIXELS(164), 1);
  work = gEventWork;
  work->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 40;
  Engine_EventOpenScreen();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_SetSpeed(GARET, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_SetSpeed(IVAN, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 142, 221);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPosition(IVAN, leader->x.fixed, leader->z.fixed);
  }
  Actor_WalkTo(GARET, 150, 234);
  Actor_WalkToAndWait(IVAN, 134, 234);
  Engine_ActorSetAnimation(GARET, ANIMATION_STAND);
  FieldObject_SetTargetAndCallback(
      ACTOR_PARTY_LEADER, ACTOR_LINK_KEEP_MOTION | MIA, gImil_FaceTargetScript);
  FieldObject_SetTargetAndCallback(GARET, ACTOR_LINK_KEEP_MOTION | MIA,
                                   gImil_FaceTargetScript);
  FieldObject_SetTargetAndCallback(IVAN, ACTOR_LINK_KEEP_MOTION | MIA,
                                   gImil_FaceTargetScript);
  Engine_CameraWaitForMove();
  table = gImil_BobScript;
  Actor_SetActionScript(ACTOR_OLD_MAN, table);
  Engine_EventWait(40);
  Actor_SetAttachedEffect(MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  // Mia checks on the old man while the party enters the house.
  Engine_EventSetMessage(MSGNO(MsgImilHowFeeling));
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_SetActionScript(ACTOR_OLD_MAN, table);
  Event_ShowMessageAndWait(ACTOR_OLD_MAN, 0, 20);
  Actor_FaceDirection(MIA, FACING_WEST, 20);
  Actor_FaceDirection(ACTOR_OLD_WOMAN, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_OLD_WOMAN, ANIMATION_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_OLD_WOMAN, 0, 40);
  Engine_ActorSetAnimationAndWait(MIA, ANIMATION_NOD);
  Engine_EventWait(10);
  Actor_FaceDirection(MIA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_OLD_WOMAN, FACING_SOUTHEAST + FACING_STEP, 20);
  Event_ShowMessageAndWait(MIA, 0, 10);
  Actor_SetActionScript(ACTOR_OLD_MAN, table);

  // Play the healing effect, then let the old man respond.
  Imil_MiaCastsPlyOnOldMan();
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_ShowMessageAndWait(MIA, 0, 40);
  Event_ShowMessageAndWait(ACTOR_OLD_MAN, 0, 20);
  Actor_ShowEmote(ACTOR_OLD_WOMAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Engine_ActorSetAnimation(ACTOR_OLD_MAN, IMIL_ANIMATION_SIT_UP);
  Map_AnimateCells(gImil_CellStepsA, 10, 69);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_ActorSetAnimationAndWait(MIA, ANIMATION_SHAKE_HEAD);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_OLD_MAN, 1);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(ACTOR_OLD_MAN, IMIL_ANIMATION_LIE_BACK);
  Map_AnimateCells(gImil_CellStepsB, 10, 69);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(MIA, ANIMATION_NOD);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_OLD_WOMAN, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_OLD_WOMAN, ANIMATION_NOD);
  Event_ShowMessageAndWait(ACTOR_OLD_WOMAN, 0, 10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceDirection(MIA, FACING_WEST, 10);
  Engine_ActorSetAnimation(MIA, ANIMATION_SHAKE_HEAD);
  Event_ShowMessageAndWait(MIA, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_OLD_WOMAN, ANIMATION_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(MIA, ANIMATION_NOD);
  Engine_EventWait(40);
  Actor_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  Object_GetById(MIA)->timer = 0;
  Actor_SetActionScript(MIA, gImil_MiaCheckRoomScript);
  while (Object_GetById(MIA)->timer == 0) {
    Engine_TaskWait(1);
  }
  Camera_MoveTo(PIXELS(140), -1, PIXELS(198), 1);
  Object_RefreshSelectorById(MIA);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Event_ShowMessageAndWait(MIA, 0, 40);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  Engine_EventWait(10);
  Event_ShowMessage(MIA, 0);
  Event_PlayCue(SE_131);
  // The lighthouse interrupts the visit; Mia leaves and the party follows.
  ColorBuffer_ApplySource(PALETTE_TRANSFORM_PLAIN, 0);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_TINT | RGB(31, 20, 31), 0);
  Engine_ColorBufferInterpolate(10);
  Engine_TaskWait(1);
  Event_PlayCue(SE_220);
  Engine_TaskWait(40);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 0);
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(60);
  Actor_SetAttachedEffect(MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(20);
  Actor_FaceDirection(MIA, 0, 10);
  Actor_SetSpeed(MIA, PIXELS(2), PIXELS(1));
  Actor_WalkToAndWait(MIA, 202, 198);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Event_ShowMessage(MIA, 0);
  Engine_ActorSetAnimationAndWait(MIA, ANIMATION_SHAKE_HEAD);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_SetAttachedEffect(MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(40);
  Event_ShowMessageAndWait(MIA, 0, 40);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessage(MIA, 0);
  Engine_ActorStop(ACTOR_PARTY_LEADER);
  Engine_ActorStop(GARET);
  Engine_ActorStop(IVAN);
  Actor_SetSpeed(MIA, PIXELS(3), PIXELS(3) / 2);
  Object_GetById(MIA)->timer = 0;
  Actor_SetActionScript(MIA, gImil_MiaRunOutScript);
  while (Object_GetById(MIA)->timer == 0) {
    Engine_TaskWait(1);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTH, 10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4), PIXELS(2));
  Actor_SetSpeed(GARET, PIXELS(4), PIXELS(2));
  Actor_SetSpeed(IVAN, PIXELS(4), PIXELS(2));
  Event_PlayCue(SE_JUMP);
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Object_GetById(GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Object_GetById(IVAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(ACTOR_PARTY_LEADER, 132, 206);
  Actor_SetDestination(GARET, 136, 221);
  Actor_SetDestination(IVAN, 122, 238);
  Object_RefreshSelectorById(MIA);
  Engine_EventWait(80);
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Object_GetById(GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Object_GetById(IVAN)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  table = gImil_FollowMiaOutScript;
  Actor_SetActionScript(GARET, table);
  Actor_RunActionScriptAndWait(IVAN, table);
  Engine_EventWait(20);
  work = gEventWork;
  work->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  work->transitionFrames = 24;
  Engine_GameFlagSet(FLAG_IMIL_MET_MIA);
  Engine_EventEnd();
}

/// Unused: an update hook that moves an effect around its stored origin until
/// its timer runs out; no actor is given it.
UNUSED static void Imil_UpdateCountdownArc(struct FieldActor *actor) {
  s32 buf[3];
  s32 *b;
  s32 n;
  s32 t;

  if (actor != NULL) {
    n = (u16)actor->timer - 1;
    actor->timer = n;
    t = (s16)n;
    if (t != 0) {
      b = buf;
      b[0] = gImil_ArcOrigin[0];
      b[1] = gImil_ArcOrigin[1] + PIXELS(8);
      b[2] = gImil_ArcOrigin[2];
      Vector_AddPolarOffset(t << 16, (t << 11) + (s16)actor->work, b);
      actor->x.fixed = b[0];
      actor->y.fixed = b[1];
      actor->z.fixed = b[2];
    } else {
      Engine_ObjectDispatchRelease(actor);
    }
  }
}

/// Unused: an update hook that sways an actor and now and then gives off a
/// rising spark; no actor is given it.
UNUSED static void Imil_SwayAndSpark(struct FieldActor *actor) {
  s32 pos[3];
  struct FieldActor *spark;
  s32 radius;

  actor->x.fixed = gImil_ArcOrigin[0] +
                   Iwram_MulQ16(0x60000, Engine_MathSin(actor->timer << 10));
  actor->timer++;
  actor->timer = (actor->timer + 64) % 64;
  if (gFrameCount % 3 == 0) {
    SET_VECTOR(pos, actor->x.fixed, actor->y.fixed + PIXELS(2), actor->z.fixed);
    radius = Engine_RandomNext();
    Vector_AddPolarOffset(radius * 6, Engine_RandomNext(), pos);
    spark = Engine_ObjectCreate(SPRITE_SPARKLE, pos[0], pos[1], pos[2]);
    if (spark != NULL) {
      spark->sprite->image.priority = 0;
      Engine_ActorSetSpriteFlags(spark, 0);
      Object_SetMode(spark, 1);
      spark->scaleX = 3 * Q16_ONE / 5;
      spark->scaleY = 3 * Q16_ONE / 5;
      spark->priorityFlags = 2;
      spark->motionFlags = 0;
      ObjectGroup_SetChildValue(spark, 9);
      Engine_ObjectSetScript(spark, gImil_SparkScript);
    }
  }
}

/// The old man bobs and speaks as Ply's effect ends (its finishCallback).
static void Imil_OldManBobsAndSpeaks(void) {
  Actor_SetActionScript(ACTOR_OLD_MAN, gImil_BobScript);
  Engine_EventShowMessage(ACTOR_OLD_MAN, 0);
}

static void Imil_MiaCastsPlyOnOldMan(void) {
  struct FieldEffectScene *work;

  Engine_PsynergyStart(ABILITY_PLY, 1);
  work = gEffectWork;
  Engine_PsynergySetTarget(MIA, ACTOR_OLD_MAN);
  work->finishCallback = Imil_OldManBobsAndSpeaks;
  Engine_PsynergyRaiseHands();
  Engine_PsynergyPlayEffect(1);
  Engine_PsynergyLowerHands();
  FieldEffect_End();
}

/// Resets a rising effect's motion, sprite priority, and initial scale.
static void Imil_InitializeRisingEffect(struct FieldActor *work) {
  struct AnimationObject *sprite;

  work->motionFlags = 0;
  work->timer = 0;
  work->priorityFlags &= ~1;

  sprite = work->sprite;
  sprite->image.priority = 1;

  ObjectGroup_SetChildValue(work, 9);
  Engine_ActorSetSpriteFlags(work, 0);

  work->scaleX = Q16_ONE / 2;
  work->scaleY = Q16_ONE / 2;
}

/// Moves and enlarges an ambient effect, then releases it when its timer
/// expires.
static void Imil_UpdateRisingEffect(union FieldObject *fieldObject) {
  fieldObject->effect.x += (s16)fieldObject->effect.spin << 8;
  fieldObject->effect.y += PIXELS(1) / 2;
  fieldObject->effect.scaleX += 3 * Q16_ONE / 100;
  fieldObject->effect.scaleY += 3 * Q16_ONE / 100;
  fieldObject->effect.spin += 2;
  if (--fieldObject->effect.countdown == 0) {
    Engine_ObjectDispatchRelease(&fieldObject->actor);
  }
}

/// Initializes a newly created ambient effect for its one-second rise.
static inline void Imil_StartRisingEffect(union FieldObject *fieldObject) {
  if (fieldObject != NULL) {
    Imil_InitializeRisingEffect(&fieldObject->actor);
    fieldObject->effect.countdown = 60;
    fieldObject->effect.update = Imil_UpdateRisingEffect;
    Object_SetMode(&fieldObject->actor, 5);
  }
}

/// Smoke rises from the chimneys of Imil's five houses, staggered over each
/// second (places/imil/footprints.png rings the chimneys).
static void Imil_SpawnAmbientEffects(void) {
  // volatile: the type of &gFrameCount (frame.h); the six editions build the
  // same bytes without it
  volatile u32 *frame;
  union FieldObject *fieldObject;

  frame = &gFrameCount;
  if (((u32)*frame % (u32)60) == 0) {
    fieldObject = (union FieldObject *)Object_Create(
        SPRITE_DUST_PUFF, PIXELS(463), 0, PIXELS(292));
    Imil_StartRisingEffect(fieldObject);
  }
  if (((u32)(*frame + 30) % (u32)60) == 0) {
    fieldObject = (union FieldObject *)Object_Create(
        SPRITE_DUST_PUFF, PIXELS(320), PIXELS(32), PIXELS(356));
    Imil_StartRisingEffect(fieldObject);
  }
  if (((u32)(*frame + 10) % (u32)60) == 0) {
    fieldObject = (union FieldObject *)Object_Create(
        SPRITE_DUST_PUFF, PIXELS(118), 0, PIXELS(70));
    Imil_StartRisingEffect(fieldObject);
  }
  if (((u32)(*frame + 50) % (u32)60) == 0) {
    fieldObject = (union FieldObject *)Object_Create(
        SPRITE_DUST_PUFF, PIXELS(342), 0, PIXELS(124));
    Imil_StartRisingEffect(fieldObject);
  }
  if (((u32)(*frame + 80) % (u32)60) == 0) {
    fieldObject = (union FieldObject *)Object_Create(
        SPRITE_DUST_PUFF, PIXELS(431), 0, PIXELS(171));
    Imil_StartRisingEffect(fieldObject);
  }
}

/// Turns the leader's footprints off in two stretches of Imil and back on
/// elsewhere, each frame while the party is in the village: the frozen river,
/// a north-south strip from x 192 to 255 widening to x 176 between z 288 and
/// 367, and the raised ground under the big tree, above a height of 30 pixels
/// east of x 232 and south of z 212 (places/imil/footprints.png).
static void Imil_UpdateFootprints(void) {
  struct FieldActor *actor;
  s32 x;
  s32 column;
  s32 isSet;
  s32 zoneFlag;
  s32 otherZoneFlag;
  struct MapScrollWork *map;

  actor = Object_GetById(ACTOR_PARTY_LEADER);
  x = actor->x.fixed;
  // In units of 8 pixels.
  column = x >> 19;
  zoneFlag = FLAG_IMIL_NO_FOOTPRINTS;
  otherZoneFlag = FLAG_IMIL_FOOTPRINTS;
  if ((u32)(column - 24) <= 7 ||
      ((u32)((actor->z.fixed >> 19) - 36) <= 9 && (u32)(column - 22) <= 9)) {
    isSet = Engine_GameFlagIsSet(zoneFlag);
    if (isSet != 0)
      return;
    ((struct MapScrollWork *)gMapWork[0])->footprints = isSet;
    Engine_GameFlagSet(zoneFlag);
    Engine_GameFlagClear(otherZoneFlag);
    return;
  }
  if (x > PIXELS(232) && actor->y.fixed > PIXELS(30) &&
      actor->z.fixed > PIXELS(212)) {
    map = gMapWork[0];
    map->footprints = 0;
    Engine_GameFlagSet(zoneFlag);
    Engine_GameFlagClear(otherZoneFlag);
    return;
  }
  isSet = Engine_GameFlagIsSet(otherZoneFlag);
  if (isSet != 0)
    return;
  map = gMapWork[0];
  map->footprints = 1;
  Engine_GameFlagSet(otherZoneFlag);
  Engine_GameFlagClear(zoneFlag);
  return;
}

/// Move on Imil's snowman, while the leader leaves footprints: once it is
/// at cell (15, 7) it stays there for the rest of the visit, and that cell
/// takes the attributes of the cell north of it. Before patch 3 any Move on
/// it does so, wherever it stops.
static void Imil_PlaceMovedSnowman(void) {
  s32 a;
  s32 b;

#if TBS_PATCH_3
  struct FieldActor *actor = Object_GetById(ACTOR_SNOWMAN);

  a = actor->x.fixed >> 20;
  if (a != 15)
    return;
  b = actor->z.fixed >> 20;
  if (b != 7)
    return;
  Engine_GameFlagSet(FLAG_IMIL_SNOWMAN_MOVED);
  Engine_ActorSetSpritePriority(ACTOR_SNOWMAN, 3);
#else
  Engine_GameFlagSet(FLAG_IMIL_SNOWMAN_MOVED);
  Engine_ActorSetSpritePriority(ACTOR_SNOWMAN, 3);
  a = 15;
  b = 7;
#endif
  Map_CopyCellAttributes(15, 6, 1, 1, a, b);
}

/// Walking into the way north out of Imil (trigger 4): the leader faces north
/// and goes into Imil Falls Cave (IMIL_TO_IMIL_FALLS_CAVE).
static void Imil_LeaveNorth(void) {
  struct FieldActor *work;

  Engine_EventStart();
  work = Object_GetById(ACTOR_PARTY_LEADER);
  work->facing = FACING_NORTH;
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(IMIL_TO_IMIL_FALLS_CAVE);
}

/// Coming back out of Imil Falls Cave (IMIL_FROM_IMIL_FALLS_CAVE), the leader
/// walks down into the village.
static void Imil_ArriveFromFallsCave(void) {
  Engine_EventStart();
  Engine_EventOpenScreen();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(1) / 10);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 232, 204);
  Engine_EventEnd();
}

/// Mia's farewell, in the Imil Sanctum the first time the party comes once
/// Saturos is beaten: she says farewell to her two apprentices, Megan and
/// Justin, before leaving with the party.
static void Imil_MiaSaysFarewellToApprentices(void) {

  struct FieldActor *leader;
  struct EventWork *work;

  Engine_EventStart();
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 690, 200);

  Camera_GetEventObject()->motionFlags = 0;
  Engine_CameraSetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  Camera_MoveTo(PIXELS(690), 0, PIXELS(164), 1);
  work = gEventWork;
  work->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 48;
  Engine_EventOpenScreen();
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_STAND);
  Actor_SetSpeed(MIA, PIXELS(3) / 5, PIXELS(3) / 10);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPosition(MIA, leader->x.fixed, leader->z.fixed);
  }
  Actor_WalkToAndWait(MIA, 673, 183);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Engine_ActorStartRepeatedMotion(ACTOR_APPRENTICE, 2);
  Engine_ActorRunRepeatedMotion(ACTOR_SECOND_APPRENTICE, 2);
  Engine_EventWait(40);
  Engine_EventSetMessage(MSGNO(MsgImilMia));
  Event_ShowMessageAndWait(ACTOR_APPRENTICE, 0, 10);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 40);
  Engine_ActorSetAnimationAndWait(MIA, ANIMATION_NOD);
  Actor_SetAttachedEffect(ACTOR_SECOND_APPRENTICE,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_SECOND_APPRENTICE, 0,
                           10);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 40);
  Engine_ActorSetAnimationAndWait(MIA, ANIMATION_SHAKE_HEAD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | MIA, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_APPRENTICE, ANIMATION_NOD);
  Engine_EventWait(20);
  Actor_SetAttachedEffect(ACTOR_APPRENTICE,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_APPRENTICE, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST + FACING_STEP, 10);
  Actor_FaceDirection(MIA, FACING_SOUTHEAST, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_SECOND_APPRENTICE, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_SECOND_APPRENTICE, 0,
                           20);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 20);
  Engine_ActorSetAnimationAndWait(MIA, ANIMATION_NOD);
  Engine_EventWait(60);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_ShowEmote(ACTOR_APPRENTICE, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_SECOND_APPRENTICE, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_APPRENTICE, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_APPRENTICE, 0, 10);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 40);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 40);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 20);
  Actor_FaceDirection(MIA, FACING_SOUTHWEST, 80);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | MIA, 0, 20);
  Actor_FaceDirection(ACTOR_SECOND_APPRENTICE, FACING_NORTHEAST + FACING_STEP,
                      0);
  Actor_FaceDirection(ACTOR_APPRENTICE, FACING_SOUTHWEST + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_APPRENTICE, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SECOND_APPRENTICE, FACING_SOUTHEAST + FACING_STEP,
                      20);
  Actor_SetSpeed(ACTOR_SECOND_APPRENTICE, PIXELS(1), PIXELS(1) / 2);
  // Let the student approach Mia before she rejoins the party.
  Object_GetById(ACTOR_SECOND_APPRENTICE)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_SECOND_APPRENTICE, 656, 166);
  Engine_EventWait(1);
  Object_GetById(ACTOR_SECOND_APPRENTICE)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_EventWait(20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_SECOND_APPRENTICE, 0,
                           10);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(40);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 10);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | MIA, 0, 40);
  Actor_FaceDirection(MIA, FACING_SOUTHEAST, 20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | MIA, 0, 10);
  Actor_SetAttachedEffect(ACTOR_APPRENTICE,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_SECOND_APPRENTICE,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(40);
  Actor_FaceDirection(MIA, FACING_NORTH, 20);
  Engine_ActorSetAnimation(MIA, ANIMATION_SHAKE_HEAD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | MIA, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_APPRENTICE, 1);
  Event_ShowMessageAndWait(ACTOR_APPRENTICE, 0, 10);
  Actor_FaceDirection(MIA, FACING_SOUTHEAST, 40);
  Actor_FaceDirection(MIA, FACING_NORTH, 20);
  Engine_ActorSetAnimationAndWait(MIA, ANIMATION_NOD);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_SECOND_APPRENTICE, 1);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_SECOND_APPRENTICE, 0,
                           20);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  Engine_EventWait(20);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 20);
  Engine_ActorSetAnimationAndWait(MIA, ANIMATION_NOD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | MIA, 0, 80);
  Actor_ShowEmote(ACTOR_APPRENTICE, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_SECOND_APPRENTICE, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_APPRENTICE, ANIMATION_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_APPRENTICE, 0, 10);
  Engine_ActorSetAnimation(ACTOR_SECOND_APPRENTICE, ANIMATION_SHAKE_HEAD);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_SECOND_APPRENTICE, 0,
                           20);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | MIA, 0, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_APPRENTICE, ANIMATION_NOD);
  Event_ShowMessageAndWait(ACTOR_APPRENTICE, 0, 10);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_APPRENTICE, ANIMATION_NOD);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_SECOND_APPRENTICE, 0,
                           10);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 60);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 20);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 20);
  Actor_FaceDirection(MIA, FACING_NORTH, 40);
  Engine_ActorSetAnimationAndWait(MIA, ANIMATION_NOD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | MIA, 0, 10);
  Engine_ActorSetAnimation(ACTOR_APPRENTICE, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_APPRENTICE, ANIMATION_NOD);
  Engine_EventWait(40);
  Actor_FaceDirection(MIA, FACING_SOUTHEAST, 20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | MIA, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_EventWait(20);
  Actor_WalkToAndWait(MIA, 688, 200);
  Actor_SetPosition(MIA, 0, 0);
  Object_GetById(ACTOR_SECOND_APPRENTICE)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_SECOND_APPRENTICE, 644, 166);
  Engine_EventWait(1);
  // Restore the student's steering after the farewell walk. A byte view: as
  // the field, the next gEventWork load moves after the constant.
  *((u8 *)Object_GetById(ACTOR_SECOND_APPRENTICE) +
    offsetof(struct FieldActor, steeringFlags)) |= ACTOR_FACES_MOTION;
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  GameFlag_Set(FLAG_IMIL_MIA_SAID_FAREWELL);
  GameFlag_Clear(FLAG_IMIL_APPRENTICE_OFFERED_HEALING);
  Engine_EventEnd();
}

/// While sliding on Imil's ice, the leader turns toward the held direction.
static void Imil_TurnSlidingActor(union FieldObject *fieldObject) {
  struct FieldActor *actor;
  s32 z;
  s32 t;
  s32 d;
  u16 prev;
  s32 n;

  actor = &fieldObject->actor;
  n = (u16)actor->timer;
  z = 0;
  t = ((s16 *)&actor->timer)[z];
  if (t != 0) {
    actor->timer = n - 1;
    return;
  }
  actor->steeringFlags = t;
  z = 1;
  d = gImil_KeyHeadings[(gKeysHeld >> 4) & 0xF];
  z = -z;
  if (d == z) {
    Object_SetMode(actor, 9);
    return;
  }
  prev = actor->facing;
  d = (s16)(d - prev);
  if (d > 0x1000)
    d = 0x1000;
  if (d < -0x1000)
    d = -0x1000;
  actor->facing = prev + d;
  Object_SetMode(actor, 2);
  ObjectDispatch_SetAnimationSpeed((struct DispatchObject *)actor, 0x30);
}

/// The leader slides along Imil's ice until something stops him.
static void Imil_RunIceSlide(void) {

  struct FieldActor *subject;
  struct FieldPosition probe;
  s32 heading;
  s32 goal;
  s32 marker;
  s32 z;
  s32 x;
  u8 *terrainId;

  subject = Engine_ActorLookup(gGameState.selectedActor);

  for (;;) {
    heading = gImil_SwayHeadings[(gKeysHeld >> 4) & 15];
    if ((heading << 16) == -0x10000) {
      return;
    }

    Engine_EventStart();

    probe.x = (subject->x.fixed & 0xfff00000) + PIXELS(8);
    probe.y = subject->y.fixed;
    probe.z = (subject->z.fixed & 0xfff00000) + PIXELS(8);
    z = probe.z;
    x = probe.x;
    terrainId = &subject->terrainId;
    goal = GetMapCellCollision(*terrainId, x, z);
    Vector_AddPolarOffset(PIXELS(16), heading, &probe);

    marker = GetMapCellCollision(*terrainId, probe.x, probe.z);
    if (marker == 255 ||
        Map_GetTerrainHeight(*terrainId, probe.x, probe.z) - subject->y.fixed >
            PIXELS(8)) {
      subject->facing = heading;
      goto tail;
    }

    probe.x = x;
    probe.z = z;
    subject->speed = PIXELS(2);
    subject->acceleration = PIXELS(1) / 10;
    subject->timer = 0;
    Engine_ObjectSetPosition(subject, x, subject->y.fixed, z);
    Object_SetMode(subject, 2);
    ObjectDispatch_SetAnimationSpeed((struct DispatchObject *)subject, 48);
    FieldObject_WaitForMove(subject);
    subject->update = Imil_TurnSlidingActor;

    // Advance along the slide until its next cell is blocked or too high.
    for (;;) {
      FieldVector_AddPolarOffset(PIXELS(16), heading, &probe);
      if ((marker = GetMapCellCollision(*terrainId, probe.x, probe.z)) == 255) {
        break;
      }
      if (Map_GetTerrainHeight(*terrainId, probe.x, probe.z) -
              subject->y.fixed >
          0x80000) {
        break;
      }
      x = probe.x;
      z = probe.z;
      subject->speed = PIXELS(2);
      subject->acceleration = PIXELS(1) / 10;
      Engine_ObjectSetPosition(subject, probe.x, probe.y, probe.z);
      FieldObject_WaitForMove(subject);
      if (marker != goal) {
        goto blocked;
      }
    }

    subject->speed = PIXELS(2);
    subject->acceleration = PIXELS(1);
    Engine_ObjectSetPosition(subject, x, subject->y.fixed, z);
    FieldObject_WaitForMove(subject);
    Engine_TaskWait(2);
  }

blocked:
  subject->update = NULL;
  subject->steeringFlags |= 1;

  subject->acceleration = PIXELS(1) / 4;

tail:
  Engine_TaskWait(10);
  Engine_EventEnd();
}

// clang-format off
s16 gImil_KeyHeadings[] = {
    -1, FACING_EAST, FACING_WEST, -1, FACING_NORTH, FACING_NORTHEAST, FACING_NORTHWEST, FACING_NORTH,
    FACING_SOUTH, FACING_SOUTHEAST, FACING_SOUTHWEST, FACING_SOUTH, -1, FACING_EAST, FACING_WEST, -1,
    -1, FACING_EAST, FACING_WEST, -1, FACING_NORTH, -1, -1, FACING_NORTH,
    FACING_SOUTH, -1, -1, FACING_SOUTH, -1, FACING_EAST, FACING_WEST, -1,
};

s16 gImil_SwayHeadings[] = {
    -1, FACING_EAST, FACING_WEST, -1, FACING_NORTH, FACING_EAST, FACING_WEST, FACING_NORTH,
    FACING_SOUTH, FACING_EAST, FACING_WEST, FACING_SOUTH, -1, FACING_EAST, FACING_WEST, -1,
};

s32 gImil_FreezingYoungManScript[] = {
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_ADD(SW_SCALE_X, -5 * Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, Q16_ONE / 16),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 5 * Q16_ONE / 32),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 5 * Q16_ONE / 64),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 5 * Q16_ONE / 64),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gImil_ShiverScript[] = {
    SC_CALL(Imil_UpdateShiver),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gImil_OldWomanHurryScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(136), 0, PIXELS(192)),
    SC_SYNC,
    SC_MOVE(PIXELS(136), 0, PIXELS(136)),
    SC_SYNC,
    SC_MOVE(PIXELS(149), 0, PIXELS(116)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_EAST),
    SC_ANIMATION(1),
    SC_END,
};

s32 gImil_LeaderStepAsideScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(155), 0, PIXELS(135)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, 0xd000),
    SC_END,
    SC_ADD(SW_SCALE_Y, 3 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -3 * Q16_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -3 * Q16_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -3 * Q16_ONE / 32),
    SC_WAIT(4),
    SC_END,
};

s32 gImil_BobScript[] = {
    SC_ADD(SW_SCALE_Y, 3 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -3 * Q16_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -3 * Q16_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -3 * Q16_ONE / 32),
    SC_WAIT(4),
    SC_LOOP(2, SC_TOP),
    SC_END,
};

s32 gImil_SparkScript[] = {
    SC_ADD(SW_Y, -PIXELS(2) / 5),
    SC_WAIT(1),
    SC_LOOP(72, SC_TOP),
    SC_FREE,
};

s32 gImil_MiaCheckRoomScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(177), 0, PIXELS(136)),
    SC_SYNC,
    SC_MOVE(PIXELS(161), 0, PIXELS(124)),
    SC_SYNC,
    SC_SET(0x23, 1),
    SC_MOVE(PIXELS(134), 0, PIXELS(139)),
    SC_SYNC,
    SC_MOVE(PIXELS(135), 0, PIXELS(189)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gImil_MiaRunOutScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(165), 0, PIXELS(207)),
    SC_SYNC,
    SC_SET(0x23, 1),
    SC_MOVE(PIXELS(147), 0, PIXELS(252)),
    SC_SYNC,
    SC_MOVE(PIXELS(150), 0, PIXELS(268)),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gImil_FaceTargetScript[] = {
    SC_CALL(Scene_TurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gImil_FollowMiaOutScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(132), 0, PIXELS(206)),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

struct SceneEntrance gImil_TownEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 380, 0, 433, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_FROM_HOUSE_SOUTHWEST, CONDITION_ALWAYS, 288, 0, 488, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_EAST_UNUSED, CONDITION_ALWAYS, 480, 0, 280, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { IMIL_FROM_HOUSE_WEST, CONDITION_ALWAYS, 312, 0, 424, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_FROM_HOUSE_NORTHEAST, CONDITION_ALWAYS, 424, 0, 392, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_FROM_IMIL_INTERIORS_EAST, CONDITION_ALWAYS, 456, 0, 248, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_FROM_IMIL_INTERIORS_NORTHEAST, CONDITION_ALWAYS, 376, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_FROM_IMIL_INTERIORS_WEST, CONDITION_ALWAYS, 120, 0, 208, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_FROM_IMIL_INTERIORS_SOUTHWEST, CONDITION_ALWAYS, 64, 0, 360, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_FROM_IMIL_FALLS_CAVE, CONDITION_ALWAYS, 229, 0, 88, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImil_InteriorEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 136, 0, 248, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_INTERIORS_HOUSE_SOUTHWEST, CONDITION_ALWAYS, 144, 0, 242, FACING_NORTH, 0, 32, 52, 256, 280, 0 },
    { IMIL_INTERIORS_FROM_IMIL_WEST_SOUTHWEST, CONDITION_ALWAYS, 143, 0, 596, FACING_NORTH, 0, 42, 424, 256, 631, 0 },
    { IMIL_INTERIORS_HOUSE_WEST, CONDITION_ALWAYS, 399, 0, 322, FACING_NORTH, 0, 320, 58, 536, 360, 0 },
    { IMIL_INTERIORS_HOUSE_NORTHEAST, CONDITION_ALWAYS, 400, 0, 659, FACING_NORTH, 0, 307, 382, 545, 697, 0 },
    { IMIL_INTERIORS_INN, CONDITION_ALWAYS, 129, 0, 915, FACING_NORTH, 0, 50, 706, 350, 949, 0 },
    { IMIL_INTERIORS_INN_UPSTAIRS_FROM_INN, CONDITION_ALWAYS, 446, 0, 825, FACING_SOUTH, 0, 402, 740, 670, 921, 0 },
    { IMIL_INTERIORS_FROM_IMIL_EAST_NORTHEAST, CONDITION_ALWAYS, 687, 0, 264, FACING_NORTH, 0, 594, 49, 806, 295, 0 },
    { IMIL_INTERIORS_INN_FROM_INN_UPSTAIRS, CONDITION_ALWAYS, 182, 0, 772, FACING_SOUTH, 0, 50, 706, 350, 949, 0 },
    { SCENE_TABLE_END },
};

u32 gImil_Exits[] = {
    SCENE_EXITS(Imil),
    SCENE_EXIT(IMIL_TO_HOUSE_SOUTHWEST, ImilInteriors, IMIL_INTERIORS_HOUSE_SOUTHWEST),
    SCENE_EXIT(IMIL_TO_INN, ImilInteriors, IMIL_INTERIORS_INN),
    SCENE_EXIT(IMIL_TO_HOUSE_WEST, ImilInteriors, IMIL_INTERIORS_HOUSE_WEST),
    SCENE_EXIT(IMIL_TO_HOUSE_NORTHEAST, ImilInteriors, IMIL_INTERIORS_HOUSE_NORTHEAST),
    SCENE_EXIT(IMIL_TO_IMIL_INTERIORS_EAST, ImilInteriors, IMIL_INTERIORS_FROM_IMIL_WEST_SOUTHWEST),
    SCENE_EXIT(IMIL_TO_IMIL_INTERIORS_NORTHEAST, ImilInteriors, IMIL_INTERIORS_FROM_IMIL_EAST_NORTHEAST),
    SCENE_EXIT(IMIL_TO_IMIL_FALLS_CAVE, ImilFallsCave, IMIL_FALLS_CAVE_FROM_IMIL),
    SCENE_EXIT(IMIL_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_IMIL_SOUTH),
    SCENE_EXIT(IMIL_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_IMIL_NORTH),
    SCENE_EXITS(ImilInteriors),
    SCENE_EXIT(IMIL_INTERIORS_HOUSE_WEST_TO_IMIL, Imil, IMIL_FROM_HOUSE_WEST),
    SCENE_EXIT(IMIL_INTERIORS_TO_IMIL_WEST_SOUTHWEST, Imil, IMIL_FROM_IMIL_INTERIORS_WEST),
    SCENE_EXIT(IMIL_INTERIORS_TO_IMIL_EAST, Imil, IMIL_FROM_IMIL_INTERIORS_EAST),
    SCENE_EXIT(IMIL_INTERIORS_TO_IMIL, Imil, IMIL_FROM_IMIL_INTERIORS_NORTHEAST),
    SCENE_EXIT(IMIL_INTERIORS_TO_IMIL_SOUTH_SOUTHWEST, Imil, IMIL_FROM_HOUSE_NORTHEAST),
    SCENE_EXIT(IMIL_INTERIORS_INN_UPSTAIRS_TO_INN, ImilInteriors, IMIL_INTERIORS_INN_FROM_INN_UPSTAIRS),
    SCENE_EXIT(IMIL_INTERIORS_TO_IMIL_EAST_NORTHEAST, Imil, IMIL_FROM_IMIL_INTERIORS_SOUTHWEST),
    SCENE_EXIT(IMIL_INTERIORS_INN_TO_INN_UPSTAIRS, ImilInteriors, IMIL_INTERIORS_INN_UPSTAIRS_FROM_INN),
    SCENE_EXITS_END,
};

struct ScenePlacement gImil_TownPlacements[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, (s32)gImil_FreezingYoungManScript,
        PIXELS(99), 0, PIXELS(234), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(376), 0, PIXELS(408), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(248), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_SNOWMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(262), 0, PIXELS(124), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gImil_TownPlacementsCured[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(99), 0, PIXELS(234), FACING_NORTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(376), 0, PIXELS(408), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(248), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_SNOWMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(262), 0, PIXELS(124), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gImil_InteriorPlacements[] = {
    { SPRITE_OLD_WOMAN, 0x0, ACTOR_STAND,
        PIXELS(111), 0, PIXELS(215), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, 0x0, ACTOR_STAND,
        PIXELS(166), 0, PIXELS(116), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLANK, 0x0, ACTOR_RESET_ACTION,
        PIXELS(174), 0, PIXELS(173), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, 0x0, ACTOR_STAND,
        PIXELS(118), 0, PIXELS(488), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, 0x0, ACTOR_STAND,
        PIXELS(370), 0, PIXELS(468), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, 0x0, ACTOR_STAND,
        PIXELS(478), 0, PIXELS(468), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, 0x0, ACTOR_STAND,
        PIXELS(460), 0, PIXELS(266), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, 0x0, ACTOR_STAND,
        PIXELS(436), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_MAN_WITH_DARK_HAIR, 0x0, ACTOR_STAND,
        PIXELS(100), 0, PIXELS(830), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_PINK_APRON, 0x0, ACTOR_WANDER,
        PIXELS(160), 0, PIXELS(880), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, ACTOR_STAND,
        PIXELS(249), 0, PIXELS(832), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, 0x0, ACTOR_STAND,
        PIXELS(686), 0, PIXELS(148), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, 0x0, ACTOR_STAND,
        PIXELS(717), 0, PIXELS(229), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_CRATE, 0x0, ACTOR_RESET_ACTION,
        PIXELS(192), 0, PIXELS(124), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gImil_TownEvents[] = {
    { EVENT_EXIT, 60, CONDITION_ALWAYS, IMIL_TO_WORLD_MAP_SOUTH },
    { EVENT_EXIT, 61, CONDITION_ALWAYS, IMIL_TO_WORLD_MAP_NORTH },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SNOWMAN, FLAG_IMIL_NO_FOOTPRINTS, EVENT_SCRIPT(Imil_PlaceMovedSnowman) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 51, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_OpenDoorAndExit) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 52, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_OpenDoorAndExit) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 53, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_OpenDoorAndExit) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 54, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_OpenDoorAndExit) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 55, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_OpenDoorAndExit) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 56, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_OpenDoorAndExit) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_LeaveNorth) },
    { EVENT_TOUCH, 90, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_StartIceSlide) },
    { EVENT_TALK, ACTOR_FREEZING_YOUNG_MAN, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT_SCRIPT(Imil_TalkToFreezingYoungMan) },
    { EVENT_TALK, ACTOR_FREEZING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgImilTheMercuryLighthouseIsShiningMia) },
    { EVENT_TALK, ACTOR_GREETING_BOY, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT_SCRIPT(Imil_GreetVisitor) },
    { EVENT_TALK, ACTOR_GREETING_BOY, CONDITION_ALWAYS, MSGNO(MsgImilImilWillBeFineDiseaseIs) },
    { EVENT_TALK, ACTOR_STRONGMAN_KEEPING_IN_SHAPE, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_TalkToStrongmanKeepingInShape) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FREEZING_YOUNG_MAN, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT_SCRIPT(Imil_ReadFreezingYoungMansMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FREEZING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgImilMiaWasOnlyInImilTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GREETING_BOY, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilWeveHadALotOfStrangers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GREETING_BOY, CONDITION_ALWAYS, MSGNO(MsgImilDontWorryAboutUsMiaWell) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_KEEPING_IN_SHAPE, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilPeopleGetSickBecauseTheyStay) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_KEEPING_IN_SHAPE, CONDITION_ALWAYS, MSGNO(MsgImilNowThatIveLetMyGuard) },
    { SEARCH(SEARCH_JAR), 100, 0xf5f, FIND_COINS(9) },
    { SEARCH(SEARCH_CHEST), 101, 0xf60, FIND_ITEM(ITEM_VIAL) },
    { SEARCH(SEARCH_GRAVE), 102, 0xf61, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gImil_InteriorEvents[] = {
    { EVENT_TALK, ACTOR_OLD_WOMAN, FLAG_IMIL_MIA_SENT_FOR, MSGNO(MsgImilTheresBeenAnEpidemicGoingAround) },
    { EVENT_TALK, ACTOR_OLD_WOMAN, FLAG_IMIL_MET_MIA, MSGNO(MsgImilGetMiaHurry) },
    { EVENT_TALK, ACTOR_OLD_WOMAN, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT_SCRIPT(Imil_TalkToOldWoman) },
    { EVENT_TALK, ACTOR_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgImilThatFountainWaterReallyMadeGrandpa) },
    { EVENT_TALK, ACTOR_OLD_MAN, FLAG_IMIL_MIA_SENT_FOR, EVENT_SCRIPT(Imil_TalkToOldMan) },
    { EVENT_TALK, ACTOR_OLD_MAN, FLAG_IMIL_MET_MIA, MSGNO(MsgImilItHurts) },
    { EVENT_TALK, ACTOR_OLD_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_TalkToOldMan) },
    { EVENT_TALK, ACTOR_OLD_MANS_BED, IF_FLAG_SET(FLAG_IMIL_MET_MIA), EVENT_SCRIPT(Imil_TalkToOldManInBed) },
    { EVENT_TALK, ACTOR_SICK_OLD_MAN, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilCoughTheEpidemicHasHitAlmost) },
    { EVENT_TALK, ACTOR_SICK_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgImilItsNiceThatTheFountainCan) },
    { EVENT_TALK, ACTOR_WEAPON_MERCHANT, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT_SCRIPT(Imil_AskAboutWeapons) },
    { EVENT_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_TalkToWeaponMerchant) },
    { EVENT_TALK, ACTOR_ARMOR_MERCHANT, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilWereClosedSorryIGetA) },
    { EVENT_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_TalkToArmorMerchant) },
    { EVENT_TALK, ACTOR_ITEM_MERCHANT, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilIfYouWantToBuyAnything) },
    { EVENT_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_TalkToItemMerchant) },
    { EVENT_TALK, ACTOR_ITEM_MERCHANTS_DAUGHTER, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT_SCRIPT(Imil_TalkToItemMerchant) },
    { EVENT_TALK, ACTOR_ITEM_MERCHANTS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgImilImSoHappyThatMommyIs) },
    { EVENT_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_TalkToInnkeeper) },
    { EVENT_TALK, ACTOR_INN_ATTENDANT, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilWereTheOnlyInnThatStays) },
    { EVENT_TALK, ACTOR_INN_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgImilThankGoodnessTheTownIsBack) },
    { EVENT_TALK, ACTOR_RESTAURANT_OWNER, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT_SCRIPT(Imil_AskAboutRestaurantMenu) },
    { EVENT_TALK, ACTOR_RESTAURANT_OWNER, CONDITION_ALWAYS, MSGNO(MsgImilImGladTheBeaconHasBeen) },
    { EVENT_TALK, ACTOR_APPRENTICE, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_TalkToApprentice) },
    { EVENT_TALK, ACTOR_SECOND_APPRENTICE, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT_SCRIPT(Imil_TalkToSecondApprentice) },
    { EVENT_TALK, ACTOR_SECOND_APPRENTICE, CONDITION_ALWAYS, MSGNO(MsgImilIWillBeAKindGirl) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT_SCRIPT(Imil_ReadOldWomanMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgImilGrandpasAllBetterNowThanksTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_OLD_MAN, FLAG_IMIL_MIA_SENT_FOR, EVENT_SCRIPT(Imil_TalkToOldMan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN, FLAG_IMIL_MET_MIA, MSGNO(MsgImilOoohIDontFeelSoGood) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilMiaIsAGoodGirlI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgImilNowMiaCanLeaveTheVillage) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MANS_BED, IF_FLAG_SET(FLAG_IMIL_MET_MIA), EVENT_SCRIPT(Imil_ReadOldManInBedMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SICK_OLD_MAN, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilHerPalmsShineWithABlue) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SICK_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgImilThatBlueLightFromHerHands) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilWhoAreThesePeopleTheyMust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgImilCustomersAreHardToComeBy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilWhyDoTheyAlwaysComeWhen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgImilTheCustomersAreBackIThought) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilIfOnlyWeHadSomeOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgImilMiaSaidItWouldntBeGood) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANTS_DAUGHTER, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilIWonderIfTheyAreHeaded) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANTS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgImilMyDaddysStillInKolimaIll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilSoManyCustomersItJustGoes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgImilMiaWillBeLeavingWithYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_ATTENDANT, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilWintersAreToughHereSoIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgImilTheyCameToOurVillageAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RESTAURANT_OWNER, FLAG_SATUROS_BEATEN_AT_MERCURY, MSGNO(MsgImilWeDontGetToEatMany) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RESTAURANT_OWNER, CONDITION_ALWAYS, MSGNO(MsgImilINeverToldMiaThatI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_APPRENTICE, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT_SCRIPT(Imil_ReadApprenticeMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_APPRENTICE, CONDITION_ALWAYS, MSGNO(MsgImilMiasGoingToDefendTheHonor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_APPRENTICE, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT_SCRIPT(Imil_ReadSecondApprenticeMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_APPRENTICE, CONDITION_ALWAYS, MSGNO(MsgImilImSorryMiaButICant) },
    { SEARCH_FACING(FACING_NORTH), TRIGGER_WEAPON_COUNTER, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT_SCRIPT(Imil_TalkToWeaponMerchant) },
    { SEARCH_FACING(FACING_NORTH), TRIGGER_ARMOR_COUNTER, FLAG_SATUROS_BEATEN_AT_MERCURY, EVENT_SCRIPT(Imil_TalkToArmorMerchant) },
    { SEARCH(SEARCH_CHEST), 100, 0xf26, FIND_ITEM(ITEM_EMPTY_BOTTLE) },
    { SEARCH(SEARCH_BARREL), 101, 0xf62, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { SEARCH(SEARCH_OVEN), 102, 0xf63, FIND_ITEM(ITEM_LUCKY_PEPPER) },
    { SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgImilItLooksLikePorridgeItIs)) },
    // Unused: this oven's item row answers first (see the scenes README).
    { SEARCH(SEARCH_OVEN), 102, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgImilItsSomeHotStewItWarms)) },
    { EVENT_EXIT, 1, CONDITION_ALWAYS, IMIL_INTERIORS_HOUSE_WEST_TO_IMIL },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, IMIL_INTERIORS_TO_IMIL_WEST_SOUTHWEST },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, IMIL_INTERIORS_TO_IMIL_EAST },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, IMIL_INTERIORS_TO_IMIL },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, IMIL_INTERIORS_TO_IMIL_SOUTH_SOUTHWEST },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, IMIL_INTERIORS_INN_UPSTAIRS_TO_INN },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, IMIL_INTERIORS_TO_IMIL_EAST_NORTHEAST },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, IMIL_INTERIORS_INN_TO_INN_UPSTAIRS },
    { SCENE_EVENTS_END },
};

u16 gImil_DoorCellSteps5[] = {
    MAP_CELL_STEP(40, 37, 4, 2, 5), MAP_CELL_STEP(40, 39, 4, 2, 5), MAP_CELL_STEPS_END,
    MAP_CELL_STEP(35, 35, 2, 2, 5), MAP_CELL_STEP(33, 35, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 gImil_DoorCellSteps4[] = {
    MAP_CELL_STEP(35, 37, 2, 2, 5), MAP_CELL_STEP(33, 37, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 gImil_DoorCellSteps1[] = {
    MAP_CELL_STEP(35, 43, 2, 2, 5), MAP_CELL_STEP(33, 43, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 gImil_DoorCellSteps0[] = {
    MAP_CELL_STEP(35, 39, 2, 2, 5), MAP_CELL_STEP(33, 39, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 gImil_DoorCellSteps2[] = {
    MAP_CELL_STEP(42, 48, 2, 2, 5), MAP_CELL_STEP(40, 48, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 gImil_DoorCellSteps3[] = {
    MAP_CELL_STEP(42, 46, 2, 2, 5), MAP_CELL_STEP(40, 46, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 *gImil_DoorCellSteps[] = {
    gImil_DoorCellSteps0,
    gImil_DoorCellSteps1,
    gImil_DoorCellSteps2,
    gImil_DoorCellSteps3,
    gImil_DoorCellSteps4,
    gImil_DoorCellSteps5,
};

/// Cell position (x, y) for each exit.
s16 gImil_DoorCellPoints[][2] = {
    { 50, 24 }, { 57, 22 }, { 59, 13 }, { 54, 10 }, { 38, 8 }, { 34, 19 },
};
// clang-format on

u16 gImil_CellStepsA[] = {
    MAP_CELL_STEP(1, 125, 1, 2, 10),
    MAP_CELL_STEP(0, 125, 1, 2, 10),
    MAP_CELL_STEPS_END,
};

u16 gImil_CellStepsB[] = {
    MAP_CELL_STEP(1, 125, 1, 2, 10),
    MAP_CELL_STEP(2, 125, 1, 2, 10),
    MAP_CELL_STEPS_END,
};

s32 gImil_ArcOrigin[3];

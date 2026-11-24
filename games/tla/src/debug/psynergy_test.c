/// @file
/// The debug room Psynergy test: the shops, an inn and a Sanctum to try, the
/// party's levels, items and Djinn to set, and spots for the field Psynergy
/// The Lost Age adds, each answered by a small event: Scoop digs up a hole, a
/// puddle, a Wicked Mace and a statue before a battle; Cyclone clears leaves
/// and sends the leader away and back in a ring of light; Pound lights a torch
/// and Parch dries a pool; Blaze startles Kraden; Sand drops Felix to the sand
/// layer.

#include "battle/battle.h"
#include "flags.h"
#include "sounds.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "version.h"
#include "sprite_ids.h"
#include "field/scene_tables.h"
#include "system/input.h"
#include "system/io_reg.h"
#include "system/main_routine_state.h"
#include "text/msg_ids.h"
#include "field/event_inline.h"
#include "scene_ids.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "field/actor_view.h"
#include "field/object.h"
#include "field/call.h"
#include "field/field_sequence_event.h"
#include "fixed_math.h"
#include "system/vector3.h"
#include "game/inventory.h"
#include "graphics/window_close.h"
#include "macros.h"
#include "edition.h"
#include "window/item_icon.h"
#include "item_ids.h"
#include "game/character_ids.h"
#include "window/screens.h"
#include "window/window.h"
#include "scene/sparkle.h"
#include "scene/event_layer.h"
#include "shop/shop.h"

/// A Mad Plant leaps at the leader (MadPlant_LeapAtLeader, through the room's
/// own import).
void Import_DebugMadPlantLeap(s32 sprite, s32 x, s32 z);

/// The room's flags: the four Scoop spots dug, the Wicked Mace picked up, and
/// whether Blaze has sent Kraden to his second spot.
#define FLAG_HOLE_DUG FLAG_AREA_TEMP_00
#define FLAG_PUDDLE_DUG FLAG_AREA_TEMP_01
#define FLAG_WICKED_MACE_DUG FLAG_AREA_TEMP_02
#define FLAG_STATUE_DUG FLAG_AREA_TEMP_03
#define FLAG_WICKED_MACE_TAKEN FLAG_AREA_TEMP_10
#define FLAG_KRADEN_MOVED FLAG_SCENE_TEMP_00

/// The room's own cells, as map.txt has them.
enum PsynergyTestCellBehavior {
  /// The tile marked "Ouch!" in Japanese, cell (11, 2).
  CELL_BEHAVIOR_OUCH_TILE = 10,
  CELL_BEHAVIOR_LV_TILE,        ///< the tile marked LV, cell (11, 4)
  CELL_BEHAVIOR_TREASURE_TILE,  ///< marked with the kanji for treasure, (11, 6)
  CELL_BEHAVIOR_RED_CROSS_TILE, ///< the tile marked with a red cross, (11, 8)
  /// The floor of a dug hole, copied in from cell (36, 28): its number is the
  /// exit it leaves by (psynergy_test_exits.c), back to entrance 2.
  CELL_BEHAVIOR_HOLE_EXIT = 20,
  /// The four Scoop spots, corner to corner from (10, 10) to (13, 13).
  CELL_BEHAVIOR_HOLE_SPOT = 50,
  CELL_BEHAVIOR_PUDDLE_SPOT,
  CELL_BEHAVIOR_WICKED_MACE_SPOT,
  CELL_BEHAVIOR_STATUE_SPOT,
  CELL_BEHAVIOR_LEAVES = 55, ///< the patch of leaves at (2, 10), 5 by 4 cells
  CELL_BEHAVIOR_RING_LEAF,   ///< one of its leaves, cell (5, 11)
  CELL_BEHAVIOR_MAD_PLANT_LEAF, ///< its top left leaf, cell (2, 10)
  /// The pool of water, 2 by 4 cells at (16, 10).
  CELL_BEHAVIOR_POOL = 60,
  CELL_BEHAVIOR_CHEST = 100 ///< the chest, cell (14, 9)
};

/// The entrances Cyclone's spots bring the party back in by: there the leader
/// leaves in a ring of light, or arrives in one (Scene_Initialize).
enum { ENTRANCE_DEPART_IN_RING = 6, ENTRANCE_ARRIVE_IN_RING = 8 };

/// The room's actors that run its tests, numbered in the order of
/// gActorPlacements after the party's four rows.
enum PsynergyTestActorId {
  /// The four shops, one for each side he is talked to from.
  ACTOR_SHOPKEEPER = ACTOR_FIRST_PLACED,
  ACTOR_FALLEN_MAN, ///< lies on the floor; talking to him does nothing
  /// Lies on the floor too: the inn from the south, the Sanctum otherwise.
  ACTOR_INNKEEPER,
  ACTOR_MESSAGE_BROWSER, ///< the shops' and the Sanctum's lines
  ACTOR_MARS_DJINNI,     ///< lies on its side; Mind Read hears it
  ACTOR_WEEDS,           ///< the icon browser
  ACTOR_SPROUT,          ///< ignores Force
  /// A puddle frozen as the room starts: Isaac's name entry from the north,
  /// the item picker from elsewhere; it ignores Force, Whirlwind, Catch,
  /// Tremor and Grind.
  ACTOR_ICE_PILLAR,
  ACTOR_FLAME, ///< burns; Blaze at it does nothing
  /// In Kraden's sprite, he answers Blaze as Daila's practising man.
  ACTOR_KRADEN,
  ACTOR_PUDDLE,        ///< grows out of the second Scoop spot
  ACTOR_WICKED_MACE,   ///< the item's icon, which drops onto the third
  ACTOR_WINGED_STATUE, ///< placed off the map; nothing moves it
  ACTOR_TORCH          ///< Pound or A?? lights it
};

s32 ItemPicker_RunToDrop(s32 *selectedUnit, s32 *selectedSlot);
s32 Inn_CheckIn(s32 innId, s32 keeper);
void PsynergyTest_DigCells(s32 stage, s32 sourceX, s32 sourceY, s32 width,
                           s32 height, s32 destinationX, s32 destinationY);
void PsynergyTest_GrowActor(s32 stage, s32 id, s32 x, s32 z);
void PsynergyTest_DropActor(s32 stage, s32 id, s32 x, s32 z);
void PsynergyTest_RaiseStatue(s32 stage, s32 x, s32 z, s32 row, s32 column);
void PsynergyTest_StartRings(struct Actor *actor, struct SparkleState *state);
void PsynergyTest_StopRings(struct Actor *actor);
void Actor_FallOverWithAnimationById(s32 id, u32 angle, s32 animation);
void Actor_FallOverById(u32 id, u32 angle);
void Actor_DepartInRing(s32 actorId);
s32 Sanctum_Run(s32 keeper);
void Actor_ArriveInRing(s32 actorId);
s32 Event_PickUpItem(s32 actor, s32 item, s32 flag);
s32 NameEntry_EditMemberName(s32 member);
s32 Shop_Run(s32 stockRow, s32 keeper);
struct Window *Message_OpenWindow(s32 message, s32 x, s32 y, u32 options);
struct Window *Portrait_OpenWindow(s32 speaker, s32 portraitOption, s32 x,
                                   s32 y);

extern u32 gLightRing_Tiles[];
struct SparkleState gPsynergyTest_LeaderRings BSS_DATA ALIGNED(16);
struct SparkleState gPsynergyTest_TargetRings BSS_DATA ALIGNED(16);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// The event for actors and Psynergy the room leaves alone.
void PsynergyTest_NoOp(void) {}

/// Opens the picker a full party drops an item with, and discards the choice.
void PsynergyTest_OpenItemPicker(void) {
  s32 selectedSlot;
  s32 selectedUnit;

  ItemPicker_RunToDrop(&selectedUnit, &selectedSlot);
}

/// The name entry screen for Isaac.
void PsynergyTest_EditIsaacName(void) { NameEntry_EditMemberName(ISAAC); }

/// The event for Force, Whirlwind and Catch cast at the ice pillar.
void PsynergyTest_IgnorePsynergy(void) {}

/// Closes the finished message windows, opens the message in the window a
/// shopkeeper's lines open (Shop_ShowKeeperMessage) and waits until it is typed
/// out, then a frame more.
void PsynergyTest_ShowMessageAndWait(s32 message) {
  Message_CloseFinishedWindows();
  Message_OpenWindow(message, 5, 0,
                     MESSAGE_WINDOW_FIXED_X | MESSAGE_WINDOW_UNREAD);
  while (Message_AllTyped() == 0)
    wait(1);
  wait(1);
}

/// Browses `count` messages from `message`: A or Down for the next, Up for the
/// one before, B to close.
void PsynergyTest_BrowseMessageRange(s32 message, s32 count) {
  s32 i;
  struct Window *window;

  window = Portrait_OpenWindow(SPRITE_YOUNG_MAN_IN_GREEN, 0, 0, 0);
  for (i = 0; i < count; i++) {
    // Sample name, item and number substitutions before each message: Garet,
    // the Shamshir and 123456.
    MessageArgument_Push(GARET, MESSAGE_ARGUMENT_ACTOR);
    MessageArgument_Push(ITEM_SHAMSHIR, MESSAGE_ARGUMENT_ITEM);
    MessageArgument_Push(123456, MESSAGE_ARGUMENT_NUMBER);
    PsynergyTest_ShowMessageAndWait(message);
    for (;;) {
      if (gInput.held & KEY_B)
        goto done;
      if ((gInput.held & KEY_A) || (gInput.held & KEY_DOWN)) {
        message++;
        break;
      }
      if (gInput.held & KEY_UP) {
        message--;
        break;
      }
      if (gInput.held == 0)
        wait(1);
      else
        break;
    }
  }
done:
  Message_CloseFinishedWindows();
  Window_Close(window, 2);
}

/// The shop and sanctum message browsers: talking to ACTOR_MESSAGE_BROWSER
/// from each side.
void PsynergyTest_BrowseWeaponShopMessages(void) {
  PsynergyTest_BrowseMessageRange((s32)MsgWeaponShopWelcome, SHOP_MESSAGES);
}

/// Browses the armor shop's messages.
void PsynergyTest_BrowseArmorShopMessages(void) {
  PsynergyTest_BrowseMessageRange((s32)MsgArmorShopWelcome, SHOP_MESSAGES);
}

/// Browses the item shop's messages.
void PsynergyTest_BrowseItemShopMessages(void) {
  PsynergyTest_BrowseMessageRange((s32)MsgItemShopWelcome, SHOP_MESSAGES);
}

/// Browses the Sanctum's messages.
void PsynergyTest_BrowseSanctumMessages(void) {
  PsynergyTest_BrowseMessageRange((s32)MsgWelcomeWearyWanderers,
                                  SANCTUM_MESSAGES);
}

/// The shops: talking to ACTOR_SHOPKEEPER from each side opens Daila's weapon,
/// armor or item shop or Izumo's general store. Each passes on the keeper's
/// actor id.
void PsynergyTest_OpenWeaponShop(s32 keeper) {
  Shop_Run(SHOP_DAILA_WEAPONS, keeper);
}

/// Opens Daila's armor shop.
void PsynergyTest_OpenArmorShop(s32 keeper) {
  Shop_Run(SHOP_DAILA_ARMOR, keeper);
}

/// Opens Daila's item shop.
void PsynergyTest_OpenItemShop(s32 keeper) {
  Shop_Run(SHOP_DAILA_ITEMS, keeper);
}

/// Opens Izumo's general store.
void PsynergyTest_OpenGeneralStore(s32 keeper) { Shop_Run(SHOP_IZUMO, keeper); }

/// Opens Daila's inn.
void PsynergyTest_OpenInn(s32 keeper) { Inn_CheckIn(DAILA_INN, keeper); }

/// The event for talking to ACTOR_FALLEN_MAN, which does nothing.
void PsynergyTest_IgnoreTalk(void) {}

/// Raises Felix, Jenna and Sheba ten levels: examining CELL_BEHAVIOR_LV_TILE.
void PsynergyTest_RaisePartyLevels(void) {
  // "You went up a level!"
  Message_ShowCentered((s32)MsgYouWentUp, FIELD_MESSAGE_ZOOM);
  Unit_LevelUpTo(FELIX, Unit_Get(FELIX)->level + 10);
  Unit_LevelUpTo(JENNA, Unit_Get(JENNA)->level + 10);
  Unit_LevelUpTo(SHEBA, Unit_Get(SHEBA)->level + 10);
}

/// Gives 777,777 coins and ten each of Game Tickets and Lucky Medals:
/// examining CELL_BEHAVIOR_TREASURE_TILE.
void PsynergyTest_GiveTreasureTestStock(void) {
  s32 i;

  // "You got treasure!"
  Message_ShowCentered((s32)MsgYouGotTreasure, FIELD_MESSAGE_ZOOM);
  gMainRoutineState.coins = 777777;
  for (i = 0; i < 10; i++) {
    Inventory_AddItem(FELIX, ITEM_GAME_TICKET);
    Inventory_AddItem(FELIX, ITEM_LUCKY_MEDAL);
  }
  Unit_Calculate(FELIX);
  Unit_Calculate(JENNA);
  Unit_Calculate(SHEBA);
}

/// Takes HP and PP from Felix, Jenna and Sheba, then poisons and haunts Felix
/// and binds Jenna and gives her deadly poison: examining
/// CELL_BEHAVIOR_OUCH_TILE.
void PsynergyTest_DamageAndAfflictParty(void) {
  struct Unit *felix;
  struct Unit *jenna;

  // "The body is being torn apart."
  Message_ShowCentered((s32)MsgTheBodyIs, FIELD_MESSAGE_ZOOM);
  FieldUnit_AddHp(FELIX, -100);
  FieldUnit_AddHp(JENNA, -100);
  FieldUnit_AddHp(SHEBA, -33);
  Unit_AddPp(FELIX, -50);
  Unit_AddPp(JENNA, -40);
  Unit_AddPp(SHEBA, -35);
  felix = Unit_Get(FELIX);
  felix->poison = POISON_NORMAL;
  felix->haunt = 1;
  jenna = Unit_Get(JENNA);
  jenna->curse = CURSE_BOUND;
  jenna->poison = POISON_VENOM;
  Unit_Calculate(FELIX);
  Unit_Calculate(JENNA);
  Unit_Calculate(SHEBA);
}

/// Gives `owner` the Djinni `djinn` of `element` (its DJINNI_INDEX) and sets
/// its flag; with `activate` 1 the Djinni is also set.
void PsynergyTest_GiveDjinni(s32 owner, s32 element, s32 djinn, s32 activate) {
  Event_SetFlag(FLAG_DJINNI(element * DJINN_PER_ELEMENT + djinn));
  Djinn_Give(owner, element, djinn);
  if (activate == 1)
    Djinn_Set(owner, element, djinn);
}

/// Gives and sets Djinn: Felix seven of Venus, Jenna seven of Mars and Sheba
/// six of Mercury: examining CELL_BEHAVIOR_RED_CROSS_TILE.
void PsynergyTest_GivePartyDjinn(void) {
  // "You got a Djinni!"
  Message_ShowCentered((s32)MsgYouGotA, FIELD_MESSAGE_ZOOM);
  PsynergyTest_GiveDjinni(FELIX, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_FLINT), 1);
  PsynergyTest_GiveDjinni(FELIX, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_GRANITE),
                          1);
  PsynergyTest_GiveDjinni(FELIX, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_QUARTZ), 1);
  PsynergyTest_GiveDjinni(FELIX, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_VINE), 1);
  PsynergyTest_GiveDjinni(FELIX, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_SAP), 1);
  PsynergyTest_GiveDjinni(FELIX, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_GROUND), 1);
  PsynergyTest_GiveDjinni(FELIX, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_BANE), 1);
  PsynergyTest_GiveDjinni(JENNA, ELEMENT_MARS, DJINNI_INDEX(DJINNI_FORGE), 1);
  PsynergyTest_GiveDjinni(JENNA, ELEMENT_MARS, DJINNI_INDEX(DJINNI_FEVER), 1);
  PsynergyTest_GiveDjinni(JENNA, ELEMENT_MARS, DJINNI_INDEX(DJINNI_CORONA), 1);
  PsynergyTest_GiveDjinni(JENNA, ELEMENT_MARS, DJINNI_INDEX(DJINNI_SCORCH), 1);
  PsynergyTest_GiveDjinni(JENNA, ELEMENT_MARS, DJINNI_INDEX(DJINNI_EMBER), 1);
  PsynergyTest_GiveDjinni(JENNA, ELEMENT_MARS, DJINNI_INDEX(DJINNI_FLASH), 1);
  PsynergyTest_GiveDjinni(JENNA, ELEMENT_MARS, DJINNI_INDEX(DJINNI_TORCH), 1);
  PsynergyTest_GiveDjinni(SHEBA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_FIZZ), 1);
  PsynergyTest_GiveDjinni(SHEBA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_SLEET),
                          1);
  PsynergyTest_GiveDjinni(SHEBA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_MIST), 1);
  PsynergyTest_GiveDjinni(SHEBA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_SPRITZ),
                          1);
  PsynergyTest_GiveDjinni(SHEBA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_HAIL), 1);
  PsynergyTest_GiveDjinni(SHEBA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_TONIC),
                          1);
  Unit_Calculate(FELIX);
  Unit_Calculate(JENNA);
  Unit_Calculate(SHEBA);
}

/// While Felix is Sand he reads his ground height from layer 2, and from
/// layer 0 again once Sand fades.
void PsynergyTest_SetSandLayer(void) { Actor_GetById(FELIX)->layer = 2; }

/// Puts Felix back on layer 0 as Sand fades.
void PsynergyTest_ClearSandLayer(void) { Actor_GetById(FELIX)->layer = 0; }

/// As Blaze reaches him, Kraden shouts and jumps to the other of his two
/// spots, as Daila's practising man would (DailaRooms_PracticeShout,
/// which Daila never runs).
void PsynergyTest_PracticeShout(s32 unused, s32 actor) {
  FieldActor_ShowEmote(actor, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  FieldActor_Jump(actor, 6, 15);
  FieldActor_Jump(actor, 6, 15);
  FieldEvent_Wait(10);
  if (Event_GetFlag(FLAG_KRADEN_MOVED) == 0) {
    FieldEvent_SetMessageId((s32)MsgDailaRoomsGah);
    // "GAH!"
    Event_ShowMessage(actor, 0);
    Actor_WalkToAndWait(actor, 360, 72);
    Actor_FaceDirection(actor, FACING_SOUTH, 0);
    Event_SetFlag(FLAG_KRADEN_MOVED);
  } else {
    FieldEvent_SetMessageId((s32)MsgDailaRoomsYarg);
    // "YARG!"
    Event_ShowMessage(actor, 0);
    Actor_WalkToAndWait(actor, 392, 104);
    Actor_FaceDirection(actor, FACING_WEST, 0);
    Event_ClearFlag(FLAG_KRADEN_MOVED);
  }
  FieldEvent_Wait(20);
}

/// Once Blaze has ended, Kraden turns to Felix, startled, as Daila's
/// practising man would (DailaRooms_StartleAtTheLeader).
void PsynergyTest_StartleAfterBlaze(s32 unused, s32 actor) {
  Actor_FaceActor(actor, FELIX, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(actor, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 50);
  FieldActor_Jump(actor, 6, 15);
  FieldActor_Jump(actor, 6, 15);
  FieldEvent_Wait(10);
  FieldEvent_SetMessageId((s32)MsgDailaRoomsWhatAreYouDoing);
  // "What are you doing???"
  Event_ShowMessage(actor, 0);
  FieldEvent_Wait(10);
}

/// The same, once A?? (ABILITY_ARROW, "A??" in the game's text) has ended.
void PsynergyTest_StartleAgain(s32 unused, s32 actor) {
  Actor_FaceActor(actor, FELIX, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(actor, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 50);
  FieldActor_Jump(actor, 6, 15);
  FieldActor_Jump(actor, 6, 15);
  FieldEvent_Wait(10);
  FieldEvent_SetMessageId((s32)MsgDailaRoomsWhatAreYouDoing);
  // "What are you doing???"
  Event_ShowMessage(actor, 0);
  FieldEvent_Wait(10);
}

/// Once A?? has ended at the torch, it lights, and the 4 by 6 block of cells
/// at (57, 16) is copied over the one at (25, 0).
void PsynergyTest_LightTorch(void) {
  s32 destinationX;
  s32 destinationY;

  Actor_SetAnimation(ACTOR_TORCH, 2);
  FieldEvent_Wait(10);
  destinationX = 25;
  destinationY = 0;
  MapGrid_CopyCells(57, 16, 4, 6, destinationX, destinationY);
}

/// Pound on the torch does the same as its fist lands, without the wait.
void PsynergyTest_PoundTorch(s32 stage) {
  s32 destinationX;
  s32 destinationY;

  if (stage == POUND_STAGE_LANDED) {
    Actor_SetAnimation(ACTOR_TORCH, 2);
    destinationX = 25;
    destinationY = 0;
    MapGrid_CopyCells(57, 16, 4, 6, destinationX, destinationY);
  }
}

/// Cyclone at the patch of leaves blows them away: each cell of the patch it
/// touched takes its picture from the same place of the bare block at
/// (34, 26).
void PsynergyTest_BlowAwayLeaves(void) {
  s32 windowX;
  s32 windowZ;
  s32 layer;

  windowX = 2;
  windowZ = 10;
  layer = 0;
  TileCopies_CopyTouchedCells(34, 26, 5, 4, windowX, windowZ, layer);
}

/// Cyclone at CELL_BEHAVIOR_RING_LEAF blows the leaves away too, then brings
/// the party back in by ENTRANCE_DEPART_IN_RING.
void PsynergyTest_DepartInRing(void) {
  FieldTileCopies_CopyTouchedCells(34, 26, 5, 4, 2, 10, 0);
  Event_SetDestination(SCENE_PSYNERGY_TEST, ENTRANCE_DEPART_IN_RING);
}

/// Cyclone at CELL_BEHAVIOR_RED_CROSS_TILE brings the party back in by
/// ENTRANCE_ARRIVE_IN_RING.
void PsynergyTest_ArriveInRing(void) {
  Event_SetDestination(SCENE_PSYNERGY_TEST, ENTRANCE_ARRIVE_IN_RING);
}

/// Cyclone at CELL_BEHAVIOR_MAD_PLANT_LEAF: a Mad Plant leaps at the leader,
/// and a battle with no escape starts,
/// ENCOUNTER_ZONE_WORLD_MAP_BEACH_AND_GRASSLAND's first formation, one or two
/// Giant Bats.
void PsynergyTest_FightMadPlant(void) {
  Import_DebugMadPlantLeap(SPRITE_MAD_PLANT, 40, 168);
  gMainRoutineState.battleType = BATTLE_TYPE_NO_ESCAPE;
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_WORLD_MAP_BEACH_AND_GRASSLAND, 0);
}

/// The four Scoop spots, the grass patches set corner to corner east of the
/// leaves, each run while its flag is clear. This one opens a hole, whose
/// floor is CELL_BEHAVIOR_HOLE_EXIT.
void PsynergyTest_DigUpHole(s32 stage) {
  Event_SetFlag(FLAG_HOLE_DUG);
  PsynergyTest_DigCells(stage, 36, 28, 1, 1, 10, 10);
}

/// The Scoop spot that digs up a puddle, which grows out of the ground.
void PsynergyTest_DigUpPuddle(s32 stage) {
  Event_SetFlag(FLAG_PUDDLE_DUG);
  PsynergyTest_GrowActor(stage, ACTOR_PUDDLE, 184, 184);
}

/// The Scoop spot that digs up a Wicked Mace, which drops onto it.
void PsynergyTest_DigUpWickedMace(s32 stage) {
  Event_SetFlag(FLAG_WICKED_MACE_DUG);
  PsynergyTest_DropActor(stage, ACTOR_WICKED_MACE, 200, 200);
}

/// The Scoop spot that raises a statue, then a battle.
void PsynergyTest_DigUpStatue(s32 stage) {
  Event_SetFlag(FLAG_STATUE_DUG);
  PsynergyTest_RaiseStatue(stage, 216, 216,
                           ENCOUNTER_ZONE_WORLD_MAP_BEACH_AND_GRASSLAND, 0);
}

/// Talking to the Wicked Mace picks it up.
void PsynergyTest_PickUpItem(void) {
  Event_PickUpItem(ACTOR_WICKED_MACE, Actor_Get(ACTOR_WICKED_MACE)->iconItem,
                   FLAG_WICKED_MACE_TAKEN);
}

/// Parch at the pool dries it, copying in the dry cells at (48, 26).
void PsynergyTest_ParchCells(void) {
  s32 destinationX;
  s32 destinationY;

  destinationX = 16;
  destinationY = 10;
  MapGrid_CopyCells(48, 26, 2, 4, destinationX, destinationY);
}

/// Tremor's event, which does nothing.
void PsynergyTest_IgnoreTremor(void) {}

/// Grind's event, which does nothing.
void PsynergyTest_IgnoreGrind(void) {}

/// Talking to Mia: rings of light rise around her and the leader for 100
/// frames.
void PsynergyTest_ShowRings(s32 actor) {
  PsynergyTest_StartRings(Actor_Get(actor), &gPsynergyTest_TargetRings);
  PsynergyTest_StartRings(Actor_Get(gMainRoutineState.leaderActorId),
                          &gPsynergyTest_LeaderRings);
  wait(100);
  PsynergyTest_StopRings(Actor_Get(actor));
  PsynergyTest_StopRings(Actor_Get(gMainRoutineState.leaderActorId));
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Sets the shops' rare stock, freezes the puddle, lays the fallen actors down,
/// puts back what the Scoop spots and the Wicked Mace have already done, and
/// sends the leader away or brings it in by ring at the entrances for them.
s32 Scene_Initialize(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;

  state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  state->sceneAttributeHigh = 0;
  state->sceneAttributeLow = 0;
  // How many of each rare item the shops may sell, as in The Broken Seal's
  // Menu test, with a Fire Brand where it had a Masamune.
  Item_AdjustRareItemStock(ITEM_SHAMSHIR, 1);
  Item_AdjustRareItemStock(ITEM_SHAMSHIR, 1);
  Item_AdjustRareItemStock(ITEM_SHAMSHIR, 1);
  Item_AdjustRareItemStock(ITEM_SILVER_BLADE, 1);
  Item_AdjustRareItemStock(ITEM_SILVER_BLADE, 1);
  Item_AdjustRareItemStock(ITEM_FIRE_BRAND, 1);
  Item_AdjustRareItemStock(ITEM_CHINA_DRESS, 1);
  Item_AdjustRareItemStock(ITEM_COCKTAIL_DRESS, 1);
  Item_AdjustRareItemStock(ITEM_BLESSED_ROBE, 1);
  Item_AdjustRareItemStock(ITEM_ORACLES_ROBE, 1);
  Item_AdjustRareItemStock(ITEM_DRAGON_SHIELD, 1);
  Item_AdjustRareItemStock(ITEM_VAMBRACE, 1);
  Item_AdjustRareItemStock(ITEM_SPIRIT_ARMLET, 1);
  Item_AdjustRareItemStock(ITEM_WARRIORS_HELM, 1);
  gMainRoutineState.messageSpeed = MESSAGE_SPEED_FAST;
  Actor_SetAnimation(ACTOR_ICE_PILLAR, 2);
  Actor_SetAnimation(ACTOR_FLAME, 2);
  Call3((void (*)())Actor_FallOverWithAnimationById, ACTOR_FALLEN_MAN,
        FACING_WEST, ANIM_STILL);
  Call2((void (*)())Actor_FallOverById, ACTOR_INNKEEPER, FACING_WEST);
  Actor_FallOverById(ACTOR_MARS_DJINNI, FACING_EAST);
  if (Event_GetFlag(FLAG_HOLE_DUG))
    MapGrid_CopyCells(36, 28, 1, 1, 10, 10);
  if (Event_GetFlag(FLAG_PUDDLE_DUG))
    Actor_SetPosition(ACTOR_PUDDLE, 184, 184);
  if (Event_GetFlag(FLAG_WICKED_MACE_DUG) &&
      !Event_GetFlag(FLAG_WICKED_MACE_TAKEN))
    Actor_SetPosition(ACTOR_WICKED_MACE, 200, 200);
  Actor_SetItemIcon(Actor_Get(ACTOR_WICKED_MACE), ITEM_WICKED_MACE);
  if (gMainRoutineState.entrance == ENTRANCE_DEPART_IN_RING)
    Actor_DepartInRing(gMainRoutineState.leaderActorId);
  if (gMainRoutineState.entrance == ENTRANCE_ARRIVE_IN_RING)
    Actor_ArriveInRing(gMainRoutineState.leaderActorId);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Opens the icon browser.
void PsynergyTest_OpenIconBrowser(void) { DebugBrowser_Icons(); }

/// Opens the Sanctum.
void PsynergyTest_OpenSanctum(s32 keeper) { Sanctum_Run(keeper); }

/// As the ground opens, copies the cells of the hole in.
void PsynergyTest_DigCells(s32 stage, s32 sourceX, s32 sourceY, s32 width,
                           s32 height, s32 destinationX, s32 destinationY) {
  if (stage == SCOOP_STAGE_DUG)
    MapGrid_CopyCells(sourceX, sourceY, width, height, destinationX,
                      destinationY);
}

/// As the ground opens, places the actor at (x, z) in pixels with no size;
/// twenty frames later, grows it to full size.
void PsynergyTest_GrowActor(s32 stage, s32 id, s32 x, s32 z) {
  struct Actor *actor;
  s32 scale;

  actor = Actor_Get(id);
  if (stage == SCOOP_STAGE_DUG) {
    Actor_SetPosition(id, x, z);
    actor->scaleX = 0;
    actor->scaleY = 0;
  }
  if (stage == SCOOP_STAGE_DUG_UP) {
    while (actor->scaleX < FIXED_ONE) {
      wait(1);
      scale = actor->scaleX;
      scale += toFIXED(0.02);
      actor->scaleX = scale;
      actor->scaleY = scale;
    }
    actor->scaleX = FIXED_ONE;
    actor->scaleY = FIXED_ONE;
  }
}

/// Twenty frames after the ground opens, drops the actor onto (x, z) in pixels
/// from 32 pixels up.
void PsynergyTest_DropActor(s32 stage, s32 id, s32 x, s32 z) {
  struct Actor *actor;

  actor = Actor_Get(id);
  if (stage == SCOOP_STAGE_DUG_UP) {
    Actor_SetPosition(id, x, z);
    actor->y.fixed = toFIXED(32);
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    actor->gravity = toFIXED(0.09);
    wait(50);
  }
}

/// As the ground opens, places a brown boulder at (x, z) in pixels; twenty
/// frames later, grows a winged statue there from two fifths to full size
/// while it turns, turns it opposite the leader's facing and starts a battle
/// with the formation at `column` of encounter zone `row`.
void PsynergyTest_RaiseStatue(s32 stage, s32 x, s32 z, s32 row, s32 column) {
  struct Actor *actor;

  if (stage == SCOOP_STAGE_DUG) {
    actor = FieldActor_Create(SPRITE_BROWN_BOULDER, toFIXED(x), 0, toFIXED(z));
    if (actor != NULL) {
      Actor_SetSpriteFlags(actor, 0);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    }
  }
  if (stage == SCOOP_STAGE_DUG_UP) {
    actor =
        (FieldActor_Create(SPRITE_WINGED_STATUE, toFIXED(x), 0, toFIXED(z)));
    if (actor != NULL) {
      s32 scale;

      Actor_SetSpriteFlags(actor, 0);
      actor->velocityY = toFIXED(6);
      actor->scaleX = FIXED_ONE * 2 / 5;
      actor->scaleY = FIXED_ONE * 2 / 5;
      while (actor->scaleX < FIXED_ONE) {
        wait(1);
        scale = actor->scaleX + toFIXED(0.02);
        actor->facing += FACING_STEP;
        actor->scaleX = scale;
        actor->scaleY = scale;
      }
      actor->scaleX = FIXED_ONE;
      actor->scaleY = FIXED_ONE;
      actor->facing =
          Actor_Get(gMainRoutineState.leaderActorId)->facing + DEGtoANG(180);
      Field_RequestBattleFromEncounterTable(row, column);
    }
  }
}

/// Each frame of an actor's rings: a ring climbs RING_CLIMB pixels up the
/// actor, swelling and shrinking as it goes, and the next starts with a chime
/// RING_INTERVAL frames later, two at a time, while the actor's colours cycle.
/// Once stopped, the rings climb out and the actor's colours come back.
void PsynergyTest_UpdateRings(struct Actor *actor) {
  struct Vector3 position;
  s32 x, z;
  FIXED sine;
  s32 i, j;
  struct SparkleState *state = (struct SparkleState *)actor->watchedActor;
  u8 *count = &actor->sceneByte;
  struct EffectSprite *sprite;

  // Every other frame, the next of the normal colours and the five swaps.
  if (*count & 1)
    Actor_SetSpriteColorMode(actor,
                             (u8)((u32)((struct CounterBits *)count)->rest %
                                  (SPRITE_COLOR_BRG + 1)));
  if (actor->sceneCounter == 0) {
    if ((*count & (RING_INTERVAL - 1)) == 0) {
      Event_PlaySound(SE_RISING_RINGS);
      state->heights[state->nextPair] = 0;
      state->nextPair ^= 1;
    }
  } else if (actor->sceneCounter == 1) {
    if (state->heights[0] >= RING_CLIMB && state->heights[1] >= RING_CLIMB) {
      Actor_SetSpriteColorMode(actor, SPRITE_COLOR_NORMAL);
      VramCache_FreeEntry(state->vramEntry);
      actor->update = NULL;
      return;
    }
  }
  (*count)++;
  for (i = 0; i < 2; i++) {
    // Its size: half a turn of sine over the climb.
    sine = Math_Sin(state->heights[i] * (DEGtoANG(180) / RING_CLIMB));
    if (state->heights[i] >= 0) {
      if (state->heights[i] < RING_CLIMB) {

        SET_POSITION(&position, actor->x.fixed,
                     actor->y.fixed + toFIXED(state->heights[i]),
                     actor->z.fixed);
        Camera_WorldToScreen(&position);
        x = position.x;
        j = 0;
        z = position.z;
        sprite = &state->sprites[i * 2];
        for (; j <= 1; j++) {
          sprite->x = x;
          sprite->y = z;
          sprite->scaleX = sine;
          sprite->scaleY = sine;
          if (j == 0) {
            sprite->depth = Actor_GetSpriteDepth(actor) - 1;
          } else {
            sprite->depth = Actor_GetSpriteDepth(actor) + 1;
            sprite->y -= toFIXED(1);
            sprite->scaleY = -sprite->scaleY;
          }
          EffectSprite_Draw(sprite);
          sprite++;
        }
        state->heights[i]++;
      }
    }
  }
}

// clang-format off
#include "../scene/shared/light_ring_tiles.c"

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 64, 0, 56, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 112, 0, 56, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 64, 0, 120, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 112, 0, 120, FACING_NORTH, 0, 16, 16, 304, 224, 0},
    {4, CONDITION_ALWAYS, 344, 0, 104, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 88, 0, 184, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {ENTRANCE_DEPART_IN_RING, CONDITION_ALWAYS, 184, 0, 200, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {7, CONDITION_ALWAYS, 424, 0, 136, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {ENTRANCE_ARRIVE_IN_RING, CONDITION_ALWAYS, 184, 0, 200, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

#include "../scene/shared/psynergy_test_exits.c"

// clang-format on

/// The room's people: the party's four rows, then enum PsynergyTestActorId's.
struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 40, 0, 40,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 40, 0, 72,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 40, 0, 104,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 40, 0, 136,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 88, 0, 40, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 82, 0, 72, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 88, 0, 104, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 88, 0, 136, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    136, 0, 40, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0,
                    72, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136,
                    0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136,
                    0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_LARGE_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    360, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392,
                    0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -64,
                    0, -64, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    -64, 0, -64, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    -64, 0, -64, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0,
                    104, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    392, 0, 168, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/// The room's events: talking to its people, Mind Read on them, the cells
/// examined, and the field Psynergy each spot answers.
struct Trigger gSceneEvents[] = {
    {TRIGGER_TALK, JENNA, CONDITION_ALWAYS, EVENT(PsynergyTest_NoOp)},
    {TRIGGER_TALK, IVAN, CONDITION_ALWAYS, EVENT(PsynergyTest_NoOp)},
    {TRIGGER_TALK, MIA, CONDITION_ALWAYS, EVENT(PsynergyTest_ShowRings)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_SHOPKEEPER, CONDITION_ALWAYS,
     EVENT(PsynergyTest_OpenWeaponShop)},
    {TALK_TRIGGER_FACING(FACING_WEST), ACTOR_SHOPKEEPER, CONDITION_ALWAYS,
     EVENT(PsynergyTest_OpenArmorShop)},
    {TALK_TRIGGER_FACING(FACING_EAST), ACTOR_SHOPKEEPER, CONDITION_ALWAYS,
     EVENT(PsynergyTest_OpenItemShop)},
    {TALK_TRIGGER_FACING(FACING_SOUTH), ACTOR_SHOPKEEPER, CONDITION_ALWAYS,
     EVENT(PsynergyTest_OpenGeneralStore)},
    {TRIGGER_TALK, ACTOR_FALLEN_MAN, CONDITION_ALWAYS,
     EVENT(PsynergyTest_IgnoreTalk)},
    {TALK_TRIGGER_FACING(FACING_SOUTH), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     EVENT(PsynergyTest_OpenInn)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS,
     EVENT(PsynergyTest_OpenSanctum)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_MESSAGE_BROWSER, CONDITION_ALWAYS,
     EVENT(PsynergyTest_BrowseWeaponShopMessages)},
    {TALK_TRIGGER_FACING(FACING_WEST), ACTOR_MESSAGE_BROWSER, CONDITION_ALWAYS,
     EVENT(PsynergyTest_BrowseArmorShopMessages)},
    {TALK_TRIGGER_FACING(FACING_EAST), ACTOR_MESSAGE_BROWSER, CONDITION_ALWAYS,
     EVENT(PsynergyTest_BrowseItemShopMessages)},
    {TALK_TRIGGER_FACING(FACING_SOUTH), ACTOR_MESSAGE_BROWSER, CONDITION_ALWAYS,
     EVENT(PsynergyTest_BrowseSanctumMessages)},
    {TRIGGER_TALK, ACTOR_MARS_DJINNI, CONDITION_ALWAYS,
     EVENT(PsynergyTest_NoOp)},
    {TRIGGER_TALK, ACTOR_WEEDS, CONDITION_ALWAYS,
     EVENT(PsynergyTest_OpenIconBrowser)},
    {TRIGGER_TALK, ACTOR_SPROUT, CONDITION_ALWAYS, EVENT(PsynergyTest_NoOp)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_ICE_PILLAR, CONDITION_ALWAYS,
     EVENT(PsynergyTest_EditIsaacName)},
    {TRIGGER_TALK, ACTOR_ICE_PILLAR, CONDITION_ALWAYS,
     EVENT(PsynergyTest_OpenItemPicker)},
    {TRIGGER_TALK, ACTOR_WICKED_MACE, CONDITION_ALWAYS,
     EVENT(PsynergyTest_PickUpItem)},
// Mind Read hears what Eastern Alhafra's men say after Briggs's attack. The
// Japanese table has one more row, for the innkeeper, which the later editions
// dropped, whose line is "Any time you like; I've been kept waiting till I'm
// worn out."
#if !TLA_PATCH_1
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     5618},
#endif
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHOPKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgOooogOw)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLEN_MAN, CONDITION_ALWAYS,
     MSGNO(MsgWhyDoThese)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgStupidBriggs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MESSAGE_BROWSER,
     CONDITION_ALWAYS, MSGNO(MsgBriggsSPirates)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MARS_DJINNI, CONDITION_ALWAYS,
     MSGNO(MsgIJustCame)},
    {TRIGGER_EXIT, CELL_BEHAVIOR_HOLE_EXIT, CONDITION_ALWAYS,
     CELL_BEHAVIOR_HOLE_EXIT},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_OUCH_TILE, CONDITION_ALWAYS,
     EVENT(PsynergyTest_DamageAndAfflictParty)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_LV_TILE, CONDITION_ALWAYS,
     EVENT(PsynergyTest_RaisePartyLevels)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_TREASURE_TILE,
     CONDITION_ALWAYS, EVENT(PsynergyTest_GiveTreasureTestStock)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_RED_CROSS_TILE,
     CONDITION_ALWAYS, EVENT(PsynergyTest_GivePartyDjinn)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_CHEST, CONDITION_ALWAYS,
     FIND_COINS(333)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_SPROUT, CONDITION_ALWAYS,
     EVENT(PsynergyTest_IgnorePsynergy)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_ICE_PILLAR, CONDITION_ALWAYS,
     EVENT(PsynergyTest_IgnorePsynergy)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_ICE_PILLAR, CONDITION_ALWAYS,
     EVENT(PsynergyTest_IgnorePsynergy)},
    {PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_ICE_PILLAR, CONDITION_ALWAYS,
     EVENT(PsynergyTest_IgnorePsynergy)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, CONDITION_ALWAYS,
     EVENT(PsynergyTest_NoOp)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_TORCH,
     CONDITION_ALWAYS, EVENT(PsynergyTest_PoundTorch)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(PsynergyTest_SetSandLayer)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(PsynergyTest_ClearSandLayer)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_FLAME, CONDITION_ALWAYS, EVENT(PsynergyTest_NoOp)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS), ACTOR_KRADEN,
     CONDITION_ALWAYS, EVENT(PsynergyTest_PracticeShout)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END),
     ACTOR_KRADEN, CONDITION_ALWAYS, EVENT(PsynergyTest_StartleAfterBlaze)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_KRADEN, CONDITION_ALWAYS, EVENT(PsynergyTest_NoOp)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_ARROW, PSYNERGY_PHASE_AFTER_END),
     ACTOR_KRADEN, CONDITION_ALWAYS, EVENT(PsynergyTest_StartleAgain)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_ARROW, PSYNERGY_PHASE_AFTER_END),
     ACTOR_TORCH, CONDITION_ALWAYS, EVENT(PsynergyTest_LightTorch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_HOLE_SPOT, FLAG_HOLE_DUG, EVENT(PsynergyTest_DigUpHole)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_PUDDLE_SPOT, FLAG_PUDDLE_DUG,
     EVENT(PsynergyTest_DigUpPuddle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_WICKED_MACE_SPOT, FLAG_WICKED_MACE_DUG,
     EVENT(PsynergyTest_DigUpWickedMace)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_STATUE_SPOT, FLAG_STATUE_DUG,
     EVENT(PsynergyTest_DigUpStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_POOL, CONDITION_ALWAYS, EVENT(PsynergyTest_ParchCells)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_LEAVES, CONDITION_ALWAYS,
     EVENT(PsynergyTest_BlowAwayLeaves)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_RING_LEAF, CONDITION_ALWAYS,
     EVENT(PsynergyTest_DepartInRing)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_RED_CROSS_TILE, CONDITION_ALWAYS,
     EVENT(PsynergyTest_ArriveInRing)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MAD_PLANT_LEAF, CONDITION_ALWAYS,
     EVENT(PsynergyTest_FightMadPlant)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), ACTOR_WEEDS, CONDITION_ALWAYS,
     EVENT(PsynergyTest_IgnoreTremor)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), ACTOR_ICE_PILLAR, CONDITION_ALWAYS,
     EVENT(PsynergyTest_IgnoreTremor)},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_MESSAGE_BROWSER, CONDITION_ALWAYS,
     EVENT(PsynergyTest_IgnoreGrind)},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_ICE_PILLAR, CONDITION_ALWAYS,
     EVENT(PsynergyTest_IgnoreGrind)},
    {TRIGGER_TABLE_END},
};

// The rings' start and stop.

/// Starts an actor's rings, kept in `state`: loads their tiles and hooks
/// PsynergyTest_UpdateRings to the actor.
void PsynergyTest_StartRings(struct Actor *actor, struct SparkleState *state) {
  struct EffectSprite *sprite;
  s32 tile;
  s32 row;
  s32 col;
  s32 offset;

  state->vramEntry = VramCache_FindFreeEntry();
  Vram_Load(state->vramEntry, 8 * TILE_SIZE_4BPP, gLightRing_Tiles);
  state->nextPair = 0;
  state->heights[0] = RING_NOT_STARTED;
  state->heights[1] = RING_NOT_STARTED;
  state->actor = actor;
  actor->sceneCounter = 0;
  actor->sceneByte = 0;
  actor->update = PsynergyTest_UpdateRings;
  tile = gVramCacheEntries[state->vramEntry].offset / TILE_SIZE_4BPP;
  actor->watchedActor = (struct Actor *)state;
  for (row = 0; row <= 1; row++) {
    col = 1;
    offset = (row * 2) * sizeof(struct EffectSprite);
    sprite = (struct EffectSprite *)(offset + (s32)state);
    for (; col >= 0; col--) {
      EffectSprite_Init(sprite, 16, 16, OBJ_SIZE_32X16, tile);
      sprite->oam.fullColor = 0;
      sprite->oam.palette = 0;
      sprite->oam.priority = Actor_GetSpritePriority(actor);
      sprite++;
    }
  }
}

/// Lets an actor's rings climb out.
void PsynergyTest_StopRings(struct Actor *actor) { actor->sceneCounter = 1; }

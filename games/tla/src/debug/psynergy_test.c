/// @file
/// The debug room Psynergy test: the shops, an inn and a sanctum to try, the
/// party's levels, items and Djinn to set, and spots for the field Psynergy
/// The Lost Age adds, each answered by a small event: Scoop digs up a hole, a
/// growing actor, a falling one and a monster; Cyclone clears leaves and sends
/// the leader away and back in a ring of light; Pound and Parch change the
/// floor; Blaze startles a practising man; Sand drops Felix to the sand layer.

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
#include "sprite_ids.h"
#include "field/scene_tables.h"
#include "system/input.h"
#include "system/io_reg.h"
#include "system/main_routine_work.h"
#include "text/msg_ids.h"
#include "field/event_inline.h"
#include "scene_ids.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "field/call.h"
#include "field/field_sequence_event.h"
#include "fixed_math.h"
#include "system/vector3.h"
#include "game/inventory.h"
#include "graphics/uiflow.h"
#include "macros.h"
#include "edition.h"
#include "window/item_icon.h"
#include "item_ids.h"
#include "game/character_ids.h"
#include "window/window.h"
#include "scene/sparkle.h"

void Import_DebugMadPlantLeap(s32, s32, s32);

// The room's flags: the four Scoop spots dug, the falling actor's item taken,
// and which of his two spots the
// practising man stands at.
#define FLAG_HOLE_DUG FLAG_AREA_TEMP_00
#define FLAG_GROWING_ACTOR_DUG FLAG_AREA_TEMP_01
#define FLAG_FALLING_ACTOR_DUG FLAG_AREA_TEMP_02
#define FLAG_MONSTER_DUG FLAG_AREA_TEMP_03
#define FLAG_ITEM_TAKEN FLAG_AREA_TEMP_10
#define FLAG_PRACTICE_AT_DOOR FLAG_SCENE_TEMP_00

/// The room's actors that run its tests, numbered in the order of
/// gActorPlacements after the party's four rows.
enum PsynergyTestActorId {
  ACTOR_SHOPKEEPER = 8,       ///< the four shops, one for each side talked from
  ACTOR_INNKEEPER = 10,       ///< the inn from the south, the sanctum otherwise
  ACTOR_MESSAGE_BROWSER = 11, ///< the shop and sanctum messages
  ACTOR_ICON_BROWSER = 13,
  /// Isaac's name entry from the north, the item picker from elsewhere; it
  /// ignores Force, Whirlwind, Catch, Tremor and Grind.
  ACTOR_MENU_TESTER = 15,
  ACTOR_PRACTISING_MAN = 17, ///< startled by Blaze
  ACTOR_GROWING = 18,        ///< grows out of the second Scoop spot
  ACTOR_FALLING = 19,        ///< drops onto the third, holding an item
  ACTOR_PILLAR = 21          ///< Pound lowers the floor beside it
};

void ItemPicker_RunToDrop(s32 *, s32 *);
void Inn_CheckIn(s32, s32);
void PsynergyTest_DigCells(s32, s32, s32, s32, s32, s32, s32);
void PsynergyTest_GrowActor(s32, s32, s32, s32);
void PsynergyTest_DropActor(s32, s32, s32, s32);
void PsynergyTest_RaiseMonster(s32, s32, s32, s32, s32);
void PsynergyTest_StartRings(struct Actor *object, struct SparkleWork *work);
void PsynergyTest_StopRings(struct Actor *);
void Actor_FallOverWithAnimationById(s32 actor, s32 angle, s32 frames);
void Actor_FallOverById(s32 actor, s32 angle);
void Actor_DepartInRing(s32 actor);
void DebugBrowser_Icons();
void Sanctum_Run();
void Actor_SetSpriteColorMode(struct Actor *, u32);
void Actor_ArriveInRing(s32 actor);
s32 BattleEffect_GetObjectPriority(struct Actor *object);
void Field_RequestBattleFromEncounterTable(s32, s32);
void Event_PickUpItem(s32, s32, s32);
s32 BattleEffect_GetObjectDepth(struct Actor *object);
void NameEntry_EditMemberName(s32);
void Shop_Run(s32, s32);
void TileCopies_CopyTouchedCells(s32, s32, s32, s32, s32, s32, s32);
void Message_OpenWindow(s32, s32, s32, s32);

extern s32 gPsynergyTest_RingTiles[];
extern struct SparkleWork gPsynergyTest_TargetRings;
extern struct SparkleWork gPsynergyTest_LeaderRings;

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// The event for actors and Psynergy the room leaves alone.
void PsynergyTest_NoOp(void) {}

/// Opens the picker a full party drops an item with, and discards the choice.
void PsynergyTest_TestItemSelection(void) {
  s32 pos[2];

  ItemPicker_RunToDrop(&pos[1], &pos[0]);
}

/// The name entry screen for Isaac.
void PsynergyTest_EditIsaacName(void) { NameEntry_EditMemberName(ISAAC); }

/// The event for Force, Whirlwind and Catch cast at the menu tester.
void PsynergyTest_IgnorePsynergy(void) {}

/// Finishes pending windows, shows a message and waits until it is typed out.
void PsynergyTest_ShowMessageAndWait(s32 message) {
  Message_CloseFinishedWindows();
  Message_OpenWindow(message, 5, 0, 0x22);
  while (Message_AllTyped() == 0)
    System_Wait(1);
  System_Wait(1);
}

/// Browses `count` messages from `message`: A or Down for the next, Up for the
/// one before, B to close.
void PsynergyTest_BrowseMessageRange(s32 message, s32 count) {
  s32 i;
  void *window;

  window = Portrait_OpenWindow(SPRITE_YOUNG_MAN_IN_GREEN, 0, 0, 0);
  for (i = 0; i < count; i++) {
    // Sample name, item and number substitutions before each message: Garet,
    // the Shamshir and 123456.
    MessageArgument_Push(GARET, MESSAGE_ARGUMENT_ACTOR);
    MessageArgument_Push(ITEM_SHAMSHIR, MESSAGE_ARGUMENT_ITEM);
    MessageArgument_Push(123456, MESSAGE_ARGUMENT_NUMBER);
    PsynergyTest_ShowMessageAndWait(message);
    goto check;
  wait:
    if (gInput.held != 0)
      goto next;
    System_Wait(1);
  check:
    if (gInput.held & KEY_B)
      goto done;
    if ((gInput.held & KEY_A) || (gInput.held & KEY_DOWN)) {
      message++;
      goto next;
    }
    if (!(gInput.held & KEY_UP))
      goto wait;
    message--;
  next:;
  }
done:
  Message_CloseFinishedWindows();
  Window_Close(window, 2);
}

/// The shop and sanctum message browsers: talking to ACTOR_MESSAGE_BROWSER
/// from each side.
void PsynergyTest_BrowseWeaponShopMessages(void) {
  PsynergyTest_BrowseMessageRange((s32)MsgWeaponShopWelcome,
                                  MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

/// Browses the armor shop's messages.
void PsynergyTest_BrowseArmorShopMessages(void) {
  PsynergyTest_BrowseMessageRange((s32)MsgArmorShopWelcome,
                                  MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

/// Browses the item shop's messages.
void PsynergyTest_BrowseItemShopMessages(void) {
  PsynergyTest_BrowseMessageRange((s32)MsgItemShopWelcome,
                                  MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

/// Browses the Sanctum's messages.
void PsynergyTest_BrowseSanctumMessages(void) {
  PsynergyTest_BrowseMessageRange((s32)MsgWelcomeWearyWanderers,
                                  MsgRightWhenI - MsgWelcomeWearyWanderers);
}

/// The shops: talking to ACTOR_SHOPKEEPER from each side. Each passes on the
/// keeper's actor id.
void PsynergyTest_OpenWeaponShop(s32 keeper) { Shop_Run(1, keeper); }

/// Opens the armor shop.
void PsynergyTest_OpenArmorShop(s32 keeper) { Shop_Run(2, keeper); }

/// Opens the item shop.
void PsynergyTest_OpenItemShop(s32 keeper) { Shop_Run(3, keeper); }

/// Opens the general store.
void PsynergyTest_OpenGeneralStore(s32 keeper) { Shop_Run(24, keeper); }

/// Opens the inn.
void PsynergyTest_OpenInn(s32 keeper) { Inn_CheckIn(0, keeper); }

/// The event for talking to actor 9, which does nothing.
void PsynergyTest_IgnoreTalk(void) {}

/// Raises Felix, Jenna and Sheba ten levels. Examining cell 11.
void PsynergyTest_RaisePartyLevels(void) {
  Message_ShowCentered((s32)MsgYouWentUp, 1);
  Unit_LevelUpTo(FELIX, Unit_Get(FELIX)->level + 10);
  Unit_LevelUpTo(JENNA, Unit_Get(JENNA)->level + 10);
  Unit_LevelUpTo(SHEBA, Unit_Get(SHEBA)->level + 10);
}

/// Gives 777,777 coins and ten each of Game Tickets and Lucky Medals.
/// Examining cell 12.
void PsynergyTest_GiveTreasureTestStock(void) {
  s32 i;

  Message_ShowCentered((s32)MsgYouGotTreasure, 1);
  gMainRoutineWork.coins = 777777;
  for (i = 9; i >= 0; i--) {
    Inventory_AddItem(FELIX, ITEM_GAME_TICKET);
    Inventory_AddItem(FELIX, ITEM_LUCKY_MEDAL);
  }
  Unit_Calculate(FELIX);
  Unit_Calculate(JENNA);
  Unit_Calculate(SHEBA);
}

/// Takes HP and PP from Felix, Jenna and Sheba, then poisons and haunts Felix
/// and curses and envenoms Jenna. Examining cell 10.
void PsynergyTest_DamageAndAfflictParty(void) {
  u8 *state;

  Message_ShowCentered((s32)MsgTheBodyIs, 1);
  FieldUnit_AddHp(FELIX, -100);
  FieldUnit_AddHp(JENNA, -100);
  FieldUnit_AddHp(SHEBA, -33);
  Unit_AddPp(FELIX, -50);
  Unit_AddPp(JENNA, -40);
  Unit_AddPp(SHEBA, -35);
  state = Unit_Get(FELIX);
  state[offsetof(struct Unit, poison)] = 1;
  state += offsetof(struct Unit, haunt);
  *state = 1;
  state = Unit_Get(JENNA);
  state[offsetof(struct Unit, curse)] = 1;
  state[offsetof(struct Unit, poison)] = 2;
  Unit_Calculate(FELIX);
  Unit_Calculate(JENNA);
  Unit_Calculate(SHEBA);
}

/// Gives owner a Djinni and sets its flag; with activate 1 the Djinni is also
/// set.
void PsynergyTest_GiveDjinni(s32 owner, s32 element, s32 djinn, s32 activate) {
  Event_SetFlag(FLAG_DJINNI(element * DJINN_PER_ELEMENT + djinn));
  Djinn_Give(owner, element, djinn);
  if (activate == 1)
    Djinn_Set(owner, element, djinn);
}

/// Gives and sets Djinn: Felix seven of Venus, Jenna seven of Mars and Sheba
/// six of Mercury. Examining cell 13.
void PsynergyTest_GivePartyDjinn(void) {
  Message_ShowCentered((s32)MsgYouGotA, 1);
  PsynergyTest_GiveDjinni(FELIX, ELEMENT_VENUS, 0, 1);
  PsynergyTest_GiveDjinni(FELIX, ELEMENT_VENUS, 1, 1);
  PsynergyTest_GiveDjinni(FELIX, ELEMENT_VENUS, 2, 1);
  PsynergyTest_GiveDjinni(FELIX, ELEMENT_VENUS, 3, 1);
  PsynergyTest_GiveDjinni(FELIX, ELEMENT_VENUS, 4, 1);
  PsynergyTest_GiveDjinni(FELIX, ELEMENT_VENUS, 5, 1);
  PsynergyTest_GiveDjinni(FELIX, ELEMENT_VENUS, 6, 1);
  PsynergyTest_GiveDjinni(JENNA, ELEMENT_MARS, 0, 1);
  PsynergyTest_GiveDjinni(JENNA, ELEMENT_MARS, 1, 1);
  PsynergyTest_GiveDjinni(JENNA, ELEMENT_MARS, 2, 1);
  PsynergyTest_GiveDjinni(JENNA, ELEMENT_MARS, 3, 1);
  PsynergyTest_GiveDjinni(JENNA, ELEMENT_MARS, 4, 1);
  PsynergyTest_GiveDjinni(JENNA, ELEMENT_MARS, 5, 1);
  PsynergyTest_GiveDjinni(JENNA, ELEMENT_MARS, 6, 1);
  PsynergyTest_GiveDjinni(SHEBA, ELEMENT_MERCURY, 0, 1);
  PsynergyTest_GiveDjinni(SHEBA, ELEMENT_MERCURY, 1, 1);
  PsynergyTest_GiveDjinni(SHEBA, ELEMENT_MERCURY, 2, 1);
  PsynergyTest_GiveDjinni(SHEBA, ELEMENT_MERCURY, 3, 1);
  PsynergyTest_GiveDjinni(SHEBA, ELEMENT_MERCURY, 4, 1);
  PsynergyTest_GiveDjinni(SHEBA, ELEMENT_MERCURY, 5, 1);
  Unit_Calculate(FELIX);
  Unit_Calculate(JENNA);
  Unit_Calculate(SHEBA);
}

/// While Felix is Sand he reads his ground height from layer 2, and from
/// layer 0 again once Sand fades.
void PsynergyTest_SetSandLayer(void) { Actor_GetById(FELIX)->layer = 2; }

/// Puts Felix back on layer 0 as Sand fades.
void PsynergyTest_ClearSandLayer(void) { Actor_GetById(FELIX)->layer = 0; }

/// As Blaze reaches him, the practising man shouts and jumps to the other of
/// his two spots, as he does in Daila (DailaInteriors_PracticeShout).
void PsynergyTest_PracticeShout(s32 unused, s32 actor) {
  Actor_ShowEmote(actor, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  FieldActor_Jump(actor, 6, 15);
  FieldActor_Jump(actor, 6, 15);
  FieldEvent_Wait(10);
  if (Event_GetFlag(FLAG_PRACTICE_AT_DOOR) == 0) {
    FieldEvent_SetMessageId((s32)MsgDailaInteriorsGah);
    Event_ShowMessage(actor, 0);
    Actor_WalkToAndWait(actor, 360, 72);
    Actor_FaceDirection(actor, FACING_SOUTH, 0);
    Event_SetFlag(FLAG_PRACTICE_AT_DOOR);
  } else {
    FieldEvent_SetMessageId((s32)MsgDailaInteriorsYarg);
    Event_ShowMessage(actor, 0);
    Actor_WalkToAndWait(actor, 392, 104);
    Actor_FaceDirection(actor, FACING_WEST, 0);
    Event_ClearFlag(FLAG_PRACTICE_AT_DOOR);
  }
  FieldEvent_Wait(20);
}

/// Once Blaze has ended, the practising man turns to Felix, startled, as in
/// Daila (DailaInteriors_StartleAtTheLeader).
void PsynergyTest_StartleAfterBlaze(s32 unused, s32 actor) {
  Actor_FaceActor(actor, FELIX, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(actor, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 50);
  FieldActor_Jump(actor, 6, 15);
  FieldActor_Jump(actor, 6, 15);
  FieldEvent_Wait(10);
  FieldEvent_SetMessageId((s32)MsgDailaInteriorsWhatAreYouDoing);
  Event_ShowMessage(actor, 0);
  FieldEvent_Wait(10);
}

/// The same, once ability 157 ("A??" in the game's text) has ended.
void PsynergyTest_StartleAgain(s32 unused, s32 actor) {
  Actor_FaceActor(actor, FELIX, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(actor, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 50);
  FieldActor_Jump(actor, 6, 15);
  FieldActor_Jump(actor, 6, 15);
  FieldEvent_Wait(10);
  FieldEvent_SetMessageId((s32)MsgDailaInteriorsWhatAreYouDoing);
  Event_ShowMessage(actor, 0);
  FieldEvent_Wait(10);
}

/// Once ability 157 has ended at the pillar, it animates and the floor beside
/// it is copied in from (57, 16).
void PsynergyTest_LowerPillar(void) {
  s32 frames;
  s32 flags;

  Actor_SetAnimation(ACTOR_PILLAR, 2);
  FieldEvent_Wait(10);
  frames = 25;
  flags = 0;
  MapGrid_CopyCells(57, 16, 4, 6, frames, flags);
}

/// Pound on the pillar does the same, without the wait.
void PsynergyTest_PoundPillar(s32 mode) {
  s32 frames;
  s32 flags;

  if (mode == 1) {
    Actor_SetAnimation(ACTOR_PILLAR, 2);
    frames = 25;
    flags = 0;
    MapGrid_CopyCells(57, 16, 4, 6, frames, flags);
  }
}

/// Cyclone on cell 55 blows the leaves away: the cells it touched in the
/// 5 by 4 block at (34, 26) are copied in from (2, 10).
void PsynergyTest_BlowAwayLeaves(void) {
  s32 a;
  s32 b;
  s32 c;

  a = 2;
  b = 10;
  c = 0;
  TileCopies_CopyTouchedCells(34, 26, 5, 4, a, b, c);
}

static __inline__ void Map_CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                     s32 g) {
  TileCopies_CopyTouchedCells(a, b, c, d, e, f, g);
}

/// Cyclone on cell 56 blows the leaves away too, then comes back in by
/// entrance 6, where the leader leaves in a ring of light (Scene_Initialize).
void PsynergyTest_DepartInRing(void) {
  Map_CopyCells(34, 26, 5, 4, 2, 10, 0);
  Event_SetDestination(SCENE_PSYNERGY_TEST, 6);
}

/// Cyclone on cell 13 comes back in by entrance 8, where the leader arrives
/// in a ring of light.
void PsynergyTest_ArriveInRing(void) {
  Event_SetDestination(SCENE_PSYNERGY_TEST, 8);
}

/// Cyclone on cell 57: the Mad Plant leaps, and its battle starts with no
/// escape.
void PsynergyTest_FightMadPlant(void) {
  Import_DebugMadPlantLeap(249, 40, 168);
  gMainRoutineWork.battleType = BATTLE_TYPE_NO_ESCAPE;
  Field_RequestBattleFromEncounterTable(14, 0);
}

/// The four Scoop spots, cells 50 to 53, each run while their flag is clear.
/// Each passes on the stage of the dig.
void PsynergyTest_DigUpHole(s32 mode) {
  Event_SetFlag(FLAG_HOLE_DUG);
  PsynergyTest_DigCells(mode, 36, 28, 1, 1, 10, 10);
}

/// The Scoop spot that digs up an actor that grows out of the ground.
void PsynergyTest_DigUpGrowingActor(s32 mode) {
  Event_SetFlag(FLAG_GROWING_ACTOR_DUG);
  PsynergyTest_GrowActor(mode, ACTOR_GROWING, 184, 184);
}

/// The Scoop spot that digs up an actor that falls holding an item.
void PsynergyTest_DigUpFallingActor(s32 mode) {
  Event_SetFlag(FLAG_FALLING_ACTOR_DUG);
  PsynergyTest_DropActor(mode, ACTOR_FALLING, 200, 200);
}

/// The Scoop spot that digs up a monster to fight.
void PsynergyTest_DigUpMonster(s32 mode) {
  Event_SetFlag(FLAG_MONSTER_DUG);
  PsynergyTest_RaiseMonster(mode, 216, 216, 14, 0);
}

/// Talking to the actor that fell takes the item it holds (its icon).
void PsynergyTest_PickUpItem(void) {
  Event_PickUpItem(ACTOR_FALLING, Actor_Get(ACTOR_FALLING)->iconItem,
                   FLAG_ITEM_TAKEN);
}

/// Parch on cell 60 dries the 2 by 4 block at (48, 26), copied in from
/// (16, 10).
void PsynergyTest_ParchCells(void) {
  s32 frames;
  s32 flags;

  frames = 16;
  flags = 10;
  MapGrid_CopyCells(48, 26, 2, 4, frames, flags);
}

/// Tremor's event, which does nothing.
void PsynergyTest_IgnoreTremor(void) {}

/// Grind's event, which does nothing.
void PsynergyTest_IgnoreGrind(void) {}

/// Talking to Mia: rings of light rise around her and the leader for 100
/// frames.
void PsynergyTest_ShowRings(s32 actor) {
  PsynergyTest_StartRings(Actor_Get(actor), &gPsynergyTest_TargetRings);
  PsynergyTest_StartRings(Actor_Get(gMainRoutineWork.leaderActorId),
                          &gPsynergyTest_LeaderRings);
  System_Wait(100);
  PsynergyTest_StopRings(Actor_Get(actor));
  PsynergyTest_StopRings(Actor_Get(gMainRoutineWork.leaderActorId));
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Sets the shops' rare stock, lays the fallen actors down, puts back what
/// the Scoop spots and the item have already done, and sends the leader away
/// or brings it in by ring at entrances 6 and 8.
s32 Scene_Initialize(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  work->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->sceneAttributeHigh = 0;
  work->sceneAttributeLow = 0;
  // How many of each rare item the shops may sell, as in The Broken Seal's
  // Menu test, with a Fire Brand where it had a Masamune.
  Item_AdjustCounter(ITEM_SHAMSHIR, 1);
  Item_AdjustCounter(ITEM_SHAMSHIR, 1);
  Item_AdjustCounter(ITEM_SHAMSHIR, 1);
  Item_AdjustCounter(ITEM_SILVER_BLADE, 1);
  Item_AdjustCounter(ITEM_SILVER_BLADE, 1);
  Item_AdjustCounter(ITEM_FIRE_BRAND, 1);
  Item_AdjustCounter(ITEM_CHINA_DRESS, 1);
  Item_AdjustCounter(ITEM_COCKTAIL_DRESS, 1);
  Item_AdjustCounter(ITEM_BLESSED_ROBE, 1);
  Item_AdjustCounter(ITEM_ORACLES_ROBE, 1);
  Item_AdjustCounter(ITEM_DRAGON_SHIELD, 1);
  Item_AdjustCounter(ITEM_VAMBRACE, 1);
  Item_AdjustCounter(ITEM_SPIRIT_ARMLET, 1);
  Item_AdjustCounter(ITEM_WARRIORS_HELM, 1);
  gMainRoutineWork.messageSpeed = 2;
  Actor_SetAnimation(ACTOR_MENU_TESTER, 2);
  Actor_SetAnimation(16, 2);
  // Call3 and Call2 build each call's constants afresh (call.h).
  Call3((void (*)())Actor_FallOverWithAnimationById, 9, FACING_WEST, 0);
  Call2((void (*)())Actor_FallOverById, ACTOR_INNKEEPER, FACING_WEST);
  Actor_FallOverById(12, 0);
  if (Event_GetFlag(FLAG_HOLE_DUG))
    MapGrid_CopyCells(36, 28, 1, 1, 10, 10);
  if (Event_GetFlag(FLAG_GROWING_ACTOR_DUG))
    Actor_SetPosition(ACTOR_GROWING, 184, 184);
  if (Event_GetFlag(FLAG_FALLING_ACTOR_DUG) && !Event_GetFlag(FLAG_ITEM_TAKEN))
    Actor_SetPosition(ACTOR_FALLING, 200, 200);
  Actor_SetItemIcon(Actor_Get(ACTOR_FALLING), ITEM_WICKED_MACE);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    if (party->entrance == 6)
      Actor_DepartInRing(party->leaderActorId);
    if (party->entrance == 8)
      Actor_ArriveInRing(party->leaderActorId);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Opens the icon browser.
void PsynergyTest_OpenIconBrowser(void) { DebugBrowser_Icons(); }

/// Opens the Sanctum.
void PsynergyTest_OpenSanctum(void) { Sanctum_Run(); }

/// At the first stage of a dig, copies the cells of the hole in.
void PsynergyTest_DigCells(s32 mode, s32 sourceX, s32 sourceY, s32 width,
                           s32 height, s32 destinationX, s32 destinationY) {
  if (mode == 1)
    MapGrid_CopyCells(sourceX, sourceY, width, height, destinationX,
                      destinationY);
}

/// At the first stage of a dig, places the actor at (x, z) in pixels with no
/// size; at the second, grows it to full size.
void PsynergyTest_GrowActor(s32 mode, s32 id, s32 x, s32 z) {
  struct Actor *object;
  s32 scale;

  object = Actor_Get(id);
  if (mode == 1) {
    Actor_SetPosition(id, x, z);
    object->scaleX = 0;
    object->scaleY = 0;
  }
  if (mode == 2) {
    while (object->scaleX < Q16_ONE) {
      System_Wait(1);
      scale = object->scaleX;
      scale += 0x51e;
      object->scaleX = scale;
      object->scaleY = scale;
    }
    object->scaleX = Q16_ONE;
    object->scaleY = Q16_ONE;
  }
}

/// At the second stage of a dig, drops the actor onto (x, z) in pixels from 32
/// pixels up.
void PsynergyTest_DropActor(s32 mode, s32 id, s32 x, s32 z) {
  struct Actor *object;

  object = Actor_Get(id);
  if (mode == 2) {
    Actor_SetPosition(id, x, z);
    object->y.fixed = px(32);
    object->motionFlags = 3;
    object->gravity = 0x170a;
    System_Wait(50);
  }
}

/// At the first stage of a dig, places actor kind 354 at (x, z) in pixels; at
/// the second, grows actor kind 252 there from two fifths to full size while it
/// turns, faces it away from the leader and starts the battle of row and
/// column of the encounter table.
void PsynergyTest_RaiseMonster(s32 mode, s32 x, s32 z, s32 row, s32 column) {
  struct Actor *object;

  if (mode == 1) {
    object = FieldActor_Create(SPRITE_BROWN_BOULDER, px(x), 0, px(z));
    if (object != NULL) {
      Actor_SetSpriteFlags(object, 0);
      object->priorityFlags = 2;
    }
  }
  if (mode == 2) {
    object = (FieldActor_Create(SPRITE_WINGED_STATUE, px(x), 0, px(z)));
    if (object != NULL) {
      s32 scale;

      Actor_SetSpriteFlags(object, 0);
      object->velocityY = px(6);
      object->scaleX = Q16_ONE * 2 / 5;
      object->scaleY = Q16_ONE * 2 / 5;
      while (object->scaleX < Q16_ONE) {
        System_Wait(1);
        scale = object->scaleX + 0x51e;
        object->facing += FACING_STEP;
        object->scaleX = scale;
        object->scaleY = scale;
      }
      object->scaleX = Q16_ONE;
      object->scaleY = Q16_ONE;
      object->facing =
          Actor_Get(gMainRoutineWork.leaderActorId)->facing + TRIG_TURN / 2;
      Field_RequestBattleFromEncounterTable(row, column);
    }
  }
}

/// Each frame of an actor's rings: two rings of four sprites climb 32 pixels
/// up the actor, a new one every 16 frames with a chime, its colour cycling;
/// once stopped, the rings climb out and the actor's colours come back.
void PsynergyTest_UpdateRings(struct Actor *object) {
  struct Vector3 position;
  s32 x, z, sine;
  s32 i, j;
  struct SparkleWork *work = (struct SparkleWork *)object->watchedActor;
  u8 *count = (u8 *)object + 99;
  struct EffectSprite *sprite;

  if (*count & 1)
    Actor_SetSpriteColorMode(
        object, (u8)((u32)((struct CounterBits *)count)->rest % 6));
  if (((u8 *)object)[98] == 0) {
    if ((*count & 15) == 0) {
      Event_PlaySound(SE_RISING_RINGS);
      work->heights[work->nextPair] = 0;
      work->nextPair ^= 1;
    }
  } else if (((u8 *)object)[98] == 1) {
    if (work->heights[0] > 31 && work->heights[1] > 31) {
      Actor_SetSpriteColorMode(object, SPRITE_COLOR_NORMAL);
      VramCache_FreeEntry(work->vramEntry);
      object->update = 0;
      return;
    }
  }
  (*count)++;
  for (i = 0; i < 2; i++) {
    sine = Math_Sin(work->heights[i] << 10);
    if (work->heights[i] >= 0) {
      if (work->heights[i] <= 31) {
        struct Vector3 *p = &position;

        SET_POSITION(p, object->x.fixed, object->y.fixed + px(work->heights[i]),
                     object->z.fixed);
        Camera_WorldToScreen(p);
        x = p->x;
        j = 0;
        z = p->z;
        sprite = &work->sprites[i * 2];
        for (; j <= 1; j++) {
          sprite->x = x;
          sprite->y = z;
          sprite->scaleX = sine;
          sprite->scaleY = sine;
          if (j == 0) {
            sprite->depth = BattleEffect_GetObjectDepth(object) - 1;
          } else {
            sprite->depth = BattleEffect_GetObjectDepth(object) + 1;
            sprite->y -= 0x10000;
            sprite->scaleY = -sprite->scaleY;
          }
          EffectSprite_Draw(sprite);
          sprite++;
        }
        work->heights[i]++;
      }
    }
  }
}

// clang-format off
s32 gPsynergyTest_RingTiles[] = {
    0, 0, 0, 0, 0, 0,
    0x70000000, (s32)0xf7700000, 0, 0, 0, 0,
    0x77770000, (s32)0xffff7770, -9, 0x7777ffff, 0, 0,
    0, 0, 0x7777, 0x777ffff, 0x7fffffff, (s32)0xffff7777,
    0, 0, 0, 0, 0, 0,
    7, 0x77f, -0x90000, -0x9000, 0x7ffff700, 0x7ffff70,
    0x7fff70, 0x7ffff7, 0x7fff7, 0x7fff7, 0x77ff, 119,
    0, 0, 0, 0, 0, 0,
    -0x890000, 0x77000000, 0, 0, 0, 0,
    0, 0, 0x7fff, 0x7ffff, 0x7ffff7, 0x7ffff70,
    0x7fff700, 0x7ffff700, 0x7fff7000, 0x7fff7000, 0, 0,
    0, 0, 0, 0, 0x40000000, (s32)0xf4400000,
    0, 0, 0, 0, 0x44440000, (s32)0xffff4440,
    -12, 0x4444ffff, 0, 0, 0, 0,
    0x4444, 0x444ffff, 0x4fffffff, (s32)0xffff4444, 0, 0,
    0, 0, 0, 0, 4, 0x44f,
    -0xc0000, -0xc000, 0x4ffff400, 0x4ffff40, 0x4fff40, 0x4ffff4,
    0x4fff4, 0x4fff4, 0x44ff, 68, 0, 0,
    0, 0, 0, 0, -0xbc0000, 0x44000000,
    0, 0, 0, 0, 0, 0,
    0x4fff, 0x4ffff, 0x4ffff4, 0x4ffff40, 0x4fff400, 0x4ffff400,
    0x4fff4000, 0x4fff4000, (s32)0xe9000000, 233, (s32)0xeb000000, 235,
    (s32)0xedeb0000, 0xebed, (s32)0xeeedebe9, (s32)0xe9ebedee, (s32)0xeeedebe9, (s32)0xe9ebedee,
    (s32)0xedeb0000, 0xebed, (s32)0xeb000000, 235, (s32)0xe9000000, 233,
    0xe900, 0xe90000, 0xebe9e9, (s32)0xe9e9eb00, (s32)0xedebeb00, 0xebebed,
    (s32)0xefed0000, 0xedef, (s32)0xefed0000, 0xedef, (s32)0xedebeb00, 0xebebed,
    0xebe9e9, (s32)0xe9e9eb00, 0xe900, 0xe90000, (s32)0xe9000000, 233,
    (s32)0xebe90000, 0xe9eb, (s32)0xedebe900, 0xe9ebed, (s32)0xeeedebe9, (s32)0xe9ebedee,
    (s32)0xeeedebe9, (s32)0xe9ebedee, (s32)0xedebe900, 0xe9ebed, (s32)0xebe90000, 0xe9eb,
    (s32)0xe9000000, 233,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 64, 0, 56, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 112, 0, 56, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 64, 0, 120, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 112, 0, 120, FACING_NORTH, 0, 16, 16, 304, 224, 0},
    {4, CONDITION_ALWAYS, 344, 0, 104, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 88, 0, 184, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {6, CONDITION_ALWAYS, 184, 0, 200, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {7, CONDITION_ALWAYS, 424, 0, 136, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {8, CONDITION_ALWAYS, 184, 0, 200, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(PsynergyTest),
    SCENE_EXIT(20, PsynergyTest, 2),
    SCENE_EXITS_END,
};

// clang-format on

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 40, 0, 40,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 40, 0, 72,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 40, 0, 104,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 40, 0, 136,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(205, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 40,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(71, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 82, 0, 72,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(202, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 104,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(206, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 136,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(245, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 40,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 72,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(299, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 104,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 136,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(364, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 104,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 104,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -64, 0, -64,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -64, 0, -64,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(252, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -64, 0, -64,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(305, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 104,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(365, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 168,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

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
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(PsynergyTest_IgnoreTalk)},
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
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(PsynergyTest_NoOp)},
    {TRIGGER_TALK, ACTOR_ICON_BROWSER, CONDITION_ALWAYS,
     EVENT(PsynergyTest_OpenIconBrowser)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, EVENT(PsynergyTest_NoOp)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_MENU_TESTER, CONDITION_ALWAYS,
     EVENT(PsynergyTest_EditIsaacName)},
    {TRIGGER_TALK, ACTOR_MENU_TESTER, CONDITION_ALWAYS,
     EVENT(PsynergyTest_TestItemSelection)},
    {TRIGGER_TALK, ACTOR_FALLING, CONDITION_ALWAYS,
     EVENT(PsynergyTest_PickUpItem)},
#if (LANGUAGE ==                                                               \
     LANGUAGE_JAPANESE) /* Rows exclusive to the Japanese edition. */
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     5618},
#endif
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHOPKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgOooogOw2)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgWhyDoThese)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgStupidBriggs2)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MESSAGE_BROWSER,
     CONDITION_ALWAYS, MSGNO(MsgBriggsSPirates)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgIJustCame)},
    {TRIGGER_EXIT, 20, CONDITION_ALWAYS, 20},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), 10, CONDITION_ALWAYS,
     EVENT(PsynergyTest_DamageAndAfflictParty)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), 11, CONDITION_ALWAYS,
     EVENT(PsynergyTest_RaisePartyLevels)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), 12, CONDITION_ALWAYS,
     EVENT(PsynergyTest_GiveTreasureTestStock)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), 13, CONDITION_ALWAYS,
     EVENT(PsynergyTest_GivePartyDjinn)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100, CONDITION_ALWAYS, 0x20014d},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), 14, CONDITION_ALWAYS,
     EVENT(PsynergyTest_IgnorePsynergy)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_MENU_TESTER, CONDITION_ALWAYS,
     EVENT(PsynergyTest_IgnorePsynergy)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_MENU_TESTER, CONDITION_ALWAYS,
     EVENT(PsynergyTest_IgnorePsynergy)},
    {PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_MENU_TESTER, CONDITION_ALWAYS,
     EVENT(PsynergyTest_IgnorePsynergy)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_GROWING, CONDITION_ALWAYS,
     EVENT(PsynergyTest_NoOp)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_PILLAR,
     CONDITION_ALWAYS, EVENT(PsynergyTest_PoundPillar)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(PsynergyTest_SetSandLayer)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(PsynergyTest_ClearSandLayer)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 16,
     CONDITION_ALWAYS, EVENT(PsynergyTest_NoOp)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS),
     ACTOR_PRACTISING_MAN, CONDITION_ALWAYS, EVENT(PsynergyTest_PracticeShout)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END),
     ACTOR_PRACTISING_MAN, CONDITION_ALWAYS,
     EVENT(PsynergyTest_StartleAfterBlaze)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_PRACTISING_MAN, CONDITION_ALWAYS, EVENT(PsynergyTest_NoOp)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_157, PSYNERGY_PHASE_AFTER_END),
     ACTOR_PRACTISING_MAN, CONDITION_ALWAYS, EVENT(PsynergyTest_StartleAgain)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_157, PSYNERGY_PHASE_AFTER_END),
     ACTOR_PILLAR, CONDITION_ALWAYS, EVENT(PsynergyTest_LowerPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 50,
     FLAG_HOLE_DUG, EVENT(PsynergyTest_DigUpHole)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 51,
     FLAG_GROWING_ACTOR_DUG, EVENT(PsynergyTest_DigUpGrowingActor)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 52,
     FLAG_FALLING_ACTOR_DUG, EVENT(PsynergyTest_DigUpFallingActor)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 53,
     FLAG_MONSTER_DUG, EVENT(PsynergyTest_DigUpMonster)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 60,
     CONDITION_ALWAYS, EVENT(PsynergyTest_ParchCells)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 55,
     CONDITION_ALWAYS, EVENT(PsynergyTest_BlowAwayLeaves)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 56,
     CONDITION_ALWAYS, EVENT(PsynergyTest_DepartInRing)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 13,
     CONDITION_ALWAYS, EVENT(PsynergyTest_ArriveInRing)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 57,
     CONDITION_ALWAYS, EVENT(PsynergyTest_FightMadPlant)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), ACTOR_ICON_BROWSER, CONDITION_ALWAYS,
     EVENT(PsynergyTest_IgnoreTremor)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), ACTOR_MENU_TESTER, CONDITION_ALWAYS,
     EVENT(PsynergyTest_IgnoreTremor)},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_MESSAGE_BROWSER, CONDITION_ALWAYS,
     EVENT(PsynergyTest_IgnoreGrind)},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_MENU_TESTER, CONDITION_ALWAYS,
     EVENT(PsynergyTest_IgnoreGrind)},
    {TRIGGER_TABLE_END},
};

// The rings' start and stop, which the ROM places after the tables.

/// Starts an actor's rings, kept in `work`: loads their tiles and hooks
/// PsynergyTest_UpdateRings to the actor.
void PsynergyTest_StartRings(struct Actor *object, struct SparkleWork *work) {
  struct EffectSprite *sprite;
  s32 tile;
  s32 row;
  s32 col;
  s32 offset;

  work->vramEntry = VramCache_FindFreeEntry();
  Vram_Load(work->vramEntry, 256, gPsynergyTest_RingTiles);
  work->nextPair = 0;
  work->heights[0] = 999;
  work->heights[1] = 999;
  work->object = object;
  ((u8 *)object)[0x62] = 0;
  ((u8 *)object)[0x63] = 0;
  object->update = PsynergyTest_UpdateRings;
  tile = gVramCacheEntries[work->vramEntry].offset >> 5;
  object->watchedActor = (struct Actor *)work;
  for (row = 0; row <= 1; row++) {
    col = 1;
    offset = (row * 2) * sizeof(struct EffectSprite);
    sprite = (struct EffectSprite *)(offset + (s32)work);
    for (; col >= 0; col--) {
      EffectSprite_Init(sprite, 16, 16, OBJ_SIZE_32X16, tile);
      sprite->oam.fullColor = 0;
      sprite->oam.palette = 0;
      sprite->oam.priority = BattleEffect_GetObjectPriority(object);
      sprite++;
    }
  }
}

/// Lets an actor's rings climb out.
void PsynergyTest_StopRings(struct Actor *object) { ((u8 *)object)[98] = 1; }

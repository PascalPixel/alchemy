// Tolbi's interiors: the houses, inns, shops and sanctum of the festival
// town. Like the town outside, the rooms change with the Colosso story
// flags: the festival, finals day and the quiet after Colosso.

#include "game_state.h"
#include "unit/scene/tolbi_interiors.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "tolbi_interiors.h"
#include "field_event.h"
#include "text/msg_ids.h"
#include "types.h"
#include "ability_ids.h"
#include "edition.h"
#include "field_scene.h"
#include "item_ids.h"
#include "objcmd.h"
#include "songs.h"

// Place flags: Scene_ResetFlagsOnEnter clears 0x300 to 0x3ff when the party
// enters another place group, and other places use the numbers for their own.
#define FLAG_TOLBI_INTERIORS_SAILORS_SEEN 0x300 // their argument has played

#if !TBS_PATCH_1
#define TOLBI_INTERIORS_INN_CAMERA_MAX_Z 736
#define TOLBI_INTERIORS_EAST_ROOM_CAMERA_MAX_X 944
#define TOLBI_INTERIORS_SOUTH_ROOM_CAMERA_MAX_Z 928
#else
#define TOLBI_INTERIORS_INN_CAMERA_MAX_Z 744
#define TOLBI_INTERIORS_EAST_ROOM_CAMERA_MAX_X 952
#define TOLBI_INTERIORS_SOUTH_ROOM_CAMERA_MAX_Z 936
#endif

// The shop rows and door triggers used by the room event tables below.
enum {
  TOLBI_INTERIORS_WEAPON_SHOP = 26,
  TOLBI_INTERIORS_ARMOR_SHOP = 27,
  TOLBI_INTERIORS_ITEM_SHOP = 28,
  TOLBI_INTERIORS_FIRST_DOOR_TRIGGER = 14,
  TOLBI_INTERIORS_ACTOR_SLOT_END = 66
};

// Asks a yes/no question, then answers with the message after the question
// for yes or the one after that for no.
static inline void TolbiInteriors_AskYesNo(s32 question, s32 speaker) {
  Engine_EventSetMessage(question);
  Engine_EventOpenMessage(speaker, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(10);
    Engine_EventSetMessage(question + 1);
  } else {
    Engine_EventSetMessage(question + 2);
  }
  Engine_EventShowMessage(speaker, 0);
}

// The scene's entrances.
u8 *TolbiInteriors_GetEntrances(void) { return gTolbiInteriorsEntrances; }

// The interiors have no regions.
s32 TolbiInteriors_GetRegions(void) { return 0; }

// The scene's exits.
u8 *TolbiInteriors_GetExits(void) { return gTolbiInteriorsExits; }

// After Colosso the rooms hold a different crowd.
s32 TolbiInteriors_SelectPlacements(void) {
  if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
    return (s32)gTolbiInteriorsPlacementsAfterColosso;
  }
  return (s32)gTolbiInteriorsPlacements;
}

#if TBS_PATCH_4
// The Spanish and Italian editions add a question on finals day: was Babi
// missing at all, since he showed himself at the finals?
static void TolbiInteriors_AskWhetherBabiWasMissing(s32 speaker) {
  TolbiInteriors_AskYesNo(MsgTolbiInteriorsBabiShowedHimselfAtFinals, speaker);
}
#endif

// Picks the event table for the current point in the Colosso story.
u8 *TolbiInteriors_SelectEvents(void) {
  if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
    return gTolbiInteriorsEventsAfterColosso;
  }
  if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
    return gTolbiInteriorsEventsFinalsDay;
  }
  return gTolbiInteriorsEventsFestival;
}

// Leaves through a window transition of 24 frames. No event uses it.
UNUSED static void TolbiInteriors_ExitThroughWindow(void) {
  struct EventWork *work = gEventWork;

  work->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  work->transitionFrames = 24;
  Engine_EventRequestExit();
}

// Walking into one of the four doors at triggers 14 to 17: every placed
// actor stops, the door opens and the leader walks through it.
static void TolbiInteriors_EnterDoor(void) {
  u32 actorId;
  struct FieldActor *actor;
  struct EventWork *work;
  s16 *trigger;
  s32 doorIndex;

  work = gEventWork;
  // Stop the room's actors before animating the selected doorway.
  for (actorId = ACTOR_FIRST_PLACED; actorId < TOLBI_INTERIORS_ACTOR_SLOT_END;
       actorId++) {
    actor = Object_GetById(actorId);
    if (actor != 0) {
      actor->motionFlags = 0;
    }
  }
  trigger = &work->touchedTrigger;
  doorIndex = *trigger - TOLBI_INTERIORS_FIRST_DOOR_TRIGGER;
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gTolbiInteriorsDoors[doorIndex].steps,
                   gTolbiInteriorsDoors[doorIndex].x,
                   gTolbiInteriorsDoors[doorIndex].y);
  Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Engine_EventRequestExit(*trigger);
}

// After Colosso, entering by entrance 19: a woman finds something on the
// floor ("Hey, what's this?"), is startled to see the party and hides it in
// the barrel behind the trash pile. The thing is a Lucky Medal, found later in
// that barrel.
static void TolbiInteriors_WatchWomanHideMedal(void) {
  struct FieldActor *sparkle;

  Engine_EventBegin();
  Engine_EventSetMessage(MsgTolbiHeyWhatsThis);

  // A sparkle on the floor where she finds it.
  Engine_EventWait(40);
  sparkle = Engine_ObjectCreate(0x11c, PIXELS(600), 0, PIXELS(824));
  Engine_ActorSetSpriteFlags(sparkle, 0);
  Object_SetMode(sparkle, 6);
  Engine_EventWait(10);
  Object_SetMode(sparkle, 1);
  Engine_EventWait(40);
  Engine_ObjectDispatchRelease(sparkle);
  Engine_EventWait(2);

  // She runs to it and picks it up.
  Actor_ShowEmote(TOLBI_INTERIORS_MEDAL_WOMAN, EMOTE_IN_FRONT, 50);
  Actor_SetSpeed(TOLBI_INTERIORS_MEDAL_WOMAN, 0x10000, 0x8000);
  Actor_WalkToAndWait(TOLBI_INTERIORS_MEDAL_WOMAN, 600, 848);
  Actor_FaceDirection(TOLBI_INTERIORS_MEDAL_WOMAN, FACING_NORTH, 0);
  Engine_EventWait(40);
  Event_ShowMessage(TOLBI_INTERIORS_MEDAL_WOMAN, 0);
  Engine_ActorRunRepeatedMotion(TOLBI_INTERIORS_MEDAL_WOMAN, 2);
  Engine_EventWait(30);
  Actor_WalkToAndWait(TOLBI_INTERIORS_MEDAL_WOMAN, 568, 848);
  Actor_FaceDirection(TOLBI_INTERIORS_MEDAL_WOMAN, FACING_NORTH, 0);
  Engine_EventWait(30);

  // She notices the party.
  Actor_ShowEmote(TOLBI_INTERIORS_MEDAL_WOMAN, EMOTE_IN_FRONT | 8, 50);
  Engine_EventWait(20);
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
  Engine_EventWait(20);
  Actor_FaceDirection(TOLBI_INTERIORS_MEDAL_WOMAN, 0x3000, 0);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(TOLBI_INTERIORS_MEDAL_WOMAN, 2);
  Engine_EventWait(20);
  Event_ShowMessage(TOLBI_INTERIORS_MEDAL_WOMAN, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 50);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(TOLBI_INTERIORS_MEDAL_WOMAN, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessage(TOLBI_INTERIORS_MEDAL_WOMAN, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(TOLBI_INTERIORS_MEDAL_WOMAN, EMOTE_IN_FRONT | 2, 50);
  Event_ShowMessage(TOLBI_INTERIORS_MEDAL_WOMAN, 0);

  // She hides it and comes back, then leaves.
  Actor_SetSpeed(TOLBI_INTERIORS_MEDAL_WOMAN, 0x16666, 0xb333);
  Actor_WalkByAndWait(TOLBI_INTERIORS_MEDAL_WOMAN, 16, 0);
  Actor_WalkByAndWait(TOLBI_INTERIORS_MEDAL_WOMAN, 0, 32);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(TOLBI_INTERIORS_MEDAL_WOMAN, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessage(TOLBI_INTERIORS_MEDAL_WOMAN, 0);
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 16, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Engine_EventWait(20);
  Actor_SetSpeed(TOLBI_INTERIORS_MEDAL_WOMAN, 0x1cccc, 0xe666);
  Actor_WalkByAndWait(TOLBI_INTERIORS_MEDAL_WOMAN, 0, 48);
  Actor_SetPosition(TOLBI_INTERIORS_MEDAL_WOMAN, 0, 0);
  Engine_EventEnd();
}

// After Colosso, entering by entrance 16: three sailors argue about their
// captain, who won't sail the ship.
static void TolbiInteriors_WatchSailorsArgue(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgTolbiHeWontSailShipEven);
  Engine_EventWait(30);
  Engine_ActorJump(TOLBI_INTERIORS_ANGRY_SAILOR, 4, 13);
  Engine_ActorJump(TOLBI_INTERIORS_ANGRY_SAILOR, 4, 30);
  Engine_EventShowMessage(TOLBI_INTERIORS_ANGRY_SAILOR, 0);
  Engine_EventWait(10);

  Engine_ActorShowEmote(TOLBI_INTERIORS_SAILOR_B, EMOTE_IN_FRONT | 2, 50);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(TOLBI_INTERIORS_SAILOR_B, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventShowMessage(TOLBI_INTERIORS_SAILOR_B, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(TOLBI_INTERIORS_SAILOR_C, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventShowMessage(TOLBI_INTERIORS_SAILOR_C, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(TOLBI_INTERIORS_ANGRY_SAILOR, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(TOLBI_INTERIORS_ANGRY_SAILOR, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(TOLBI_INTERIORS_SAILOR_B, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventEnd();
}

// Sets up the rooms on entry. After Colosso: a row of three map cells takes
// new attributes, two of the sailors are drawn at a fixed priority, the medal
// woman stands where she hid the medal, the two short scenes play once each
// on their entrances, and the would-be warrior and actor 36 have left once
// the docks refunded the tickets.
s32 TolbiInteriors_ApplyEntryState(void) {
  s32 clearedPriorityFlags;
  struct FieldActor *actor;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
    // Restore the changed room tiles and the sailors' fixed draw priority.
    Map_CopyCellAttributes(51, 47, 3, 1, 51, 45);
    actor = Object_GetById(TOLBI_INTERIORS_ANGRY_SAILOR);
    // Stored as a plain byte: the field store allocates the zero differently.
    *(u8 *)&actor->priorityFlags = clearedPriorityFlags = 0;
    ((struct TolbiInteriorsSpriteAttr *)actor->sprite)->priority = 2;
    actor = Object_GetById(TOLBI_INTERIORS_SAILOR_B);
    actor->priorityFlags = clearedPriorityFlags;
    ((struct TolbiInteriorsSpriteAttr *)actor->sprite)->priority = 2;
    if (GameFlag_IsSet(FLAG_TOLBI_INTERIORS_MEDAL_HIDDEN) != 0) {
      Actor_SetPosition(TOLBI_INTERIORS_MEDAL_WOMAN, PIXELS(560), PIXELS(680));
      Actor_FaceDirection(TOLBI_INTERIORS_MEDAL_WOMAN, FACING_WEST, 0);
    }
    // Each short scene plays once, only when entering its room.
    if (gGameState.entrance == 19) {
      if (GameFlag_IsSet(FLAG_TOLBI_INTERIORS_MEDAL_HIDDEN) == 0) {
        Engine_GameFlagSet(FLAG_TOLBI_INTERIORS_MEDAL_HIDDEN);
        Engine_EventOpenScreen();
        TolbiInteriors_WatchWomanHideMedal();
      }
    }
    if (gGameState.entrance == 16) {
      if (GameFlag_IsSet(FLAG_TOLBI_INTERIORS_SAILORS_SEEN) == 0) {
        Engine_GameFlagSet(FLAG_TOLBI_INTERIORS_SAILORS_SEEN);
        Engine_EventOpenScreen();
        TolbiInteriors_WatchSailorsArgue();
      }
    }
    // The docks' refund flag also removes these two actors from the rooms.
    if (Engine_GameFlagIsSet(FLAG_TOLBI_DOCKS_TICKETS_REFUNDED) != 0) {
      Engine_ActorSetPosition(TOLBI_INTERIORS_WOULD_BE_WARRIOR, 0, 0);
      Engine_ActorSetPosition(36, 0, 0);
    }
  }
  return 0;
}

// The item shopkeeper: from across the counter (the leader facing west) opens
// shop 28; from elsewhere talks about the finals.
static void TolbiInteriors_TalkToItemShopkeeper(s32 speaker) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (FACING_NEAREST_CARDINAL(leader->facing) == FACING_WEST) {
    Engine_ShopOpen(TOLBI_INTERIORS_ITEM_SHOP, speaker);
  } else if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
    Engine_EventSetMessage(MsgTolbiRightOneWell);
    Engine_EventShowMessage(speaker, 0);
  } else if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
    Engine_EventSetMessage(MsgTolbiRequireLotHealing);
    Engine_EventShowMessage(speaker, 0);
  } else {
    TolbiInteriors_AskYesNo(MsgTolbiFirstTimeTolbi, speaker);
  }
}

// The weapon shopkeeper: across the counter (the leader facing north)
// opens the shop; otherwise talks about Colosso.
static void TolbiInteriors_TalkToWeaponShopkeeper(s32 speaker) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (FACING_NEAREST_CARDINAL(leader->facing) == FACING_NORTH) {
    Engine_ShopOpen(TOLBI_INTERIORS_WEAPON_SHOP, speaker);
  } else if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
    TolbiInteriors_AskYesNo(MsgTolbiComeWayKalay, speaker);
  } else if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
    Engine_EventSetMessage(MsgTolbiColossoFinalsFinally);
    Engine_EventShowMessage(speaker, 0);
  } else {
    Engine_EventSetMessage(MsgTolbiLookStrongGo);
    Engine_EventShowMessage(speaker, 0);
    Engine_ActorShowEmote(speaker, EMOTE_IN_FRONT | 6, 0);
    Engine_EventWait(40);
    Engine_EventShowMessage(speaker, 0);
  }
}

// The armor shopkeeper: across the counter (the leader facing north)
// opens the shop; otherwise talks about the finals.
static void TolbiInteriors_TalkToArmorShopkeeper(s32 speaker) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (FACING_NEAREST_CARDINAL(leader->facing) == FACING_NORTH) {
    Engine_ShopOpen(TOLBI_INTERIORS_ARMOR_SHOP, speaker);
  } else {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
      Engine_EventSetMessage(MsgTolbiWasntAbleWatch);
      Engine_EventShowMessage(speaker, 0);
    } else if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
      Engine_EventSetMessage(MsgTolbiMaybeCloseShop);
      Engine_EventShowMessage(speaker, 0);
    } else {
      Engine_EventSetMessage(MsgTolbiIfCanMakeNameFor);
      Engine_EventShowMessage(speaker, 0);
    }
  }
}

// The chef asks whether the party watched Colosso, which he missed.
static void TolbiInteriors_TalkToChef(s32 speaker) {
  Engine_EventBegin();
  TolbiInteriors_AskYesNo(MsgTolbiLookLikeWarrior, speaker);
  Engine_EventEnd();
}

// After Colosso, a waiter grumbles about the chef's bad mood.
static void TolbiInteriors_TalkToWaiter(s32 speaker) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgTolbiGrrrChefInBadMood);
  Engine_EventShowMessage(speaker, 0);
  Engine_EventEnd();
}

// During the festival, a resident offers the party a bed.
static void TolbiInteriors_TalkToHost(s32 speaker) {
  Engine_EventBegin();
  TolbiInteriors_AskYesNo(MsgTolbiWantStay, speaker);
  Engine_EventEnd();
}

// The woman who hid the Lucky Medal. Until the party takes it from the
// barrel she teases; once it is gone she greets them, startled, the first
// time, and then claims she is fine.
static void TolbiInteriors_TalkToMedalWoman(s32 speaker) {
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_TOLBI_INTERIORS_MEDAL_TAKEN) == 0) {
    TolbiInteriors_AskYesNo(MsgTolbiHeeHeeLook, speaker);
  } else {
    if (GameFlag_IsSet(FLAG_TOLBI_INTERIORS_MEDAL_MISSED) == 0) {
      GameFlag_Set(FLAG_TOLBI_INTERIORS_MEDAL_MISSED);
      Engine_EventSetMessage(MsgTolbiHello);
      Event_ShowMessage(speaker, 0);
      Engine_EventWait(10);
      Engine_ActorRunRepeatedMotion(speaker, 2);
      Engine_EventWait(20);
    }
    Engine_EventSetMessage(MsgTolbiHoHumFine);
    Event_ShowMessage(speaker, 0);
  }
  Engine_EventEnd();
}

// Mind Read on the medal woman gives the hiding place away.
static void TolbiInteriors_ReadMedalWomanMind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_TOLBI_INTERIORS_MEDAL_MISSED) == 0) {
    Engine_EventSetMessage(MsgTolbiHehHehSheJustHid);
  } else {
    Engine_EventSetMessage(MsgTolbiThingFoundDefinitelySameAs);
  }
  Engine_EventShowMessage(TOLBI_INTERIORS_MEDAL_WOMAN, 0);
  Engine_EventEnd();
}

// The angry sailor complains the ships won't sail after Colosso.
static void TolbiInteriors_TalkToAngrySailor(s32 speaker) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgTolbiGrrrScamWhyWontThey);
  Actor_ShowEmote(TOLBI_INTERIORS_ANGRY_SAILOR, EMOTE_IN_FRONT | 3, 40);
  Event_ShowMessage(speaker, 0);
  Engine_EventEnd();
}

// After Colosso, the would-be warrior asks whether the party remembers him.
static void TolbiInteriors_TalkToWouldBeWarrior(s32 speaker) {
  Engine_EventBegin();
  TolbiInteriors_AskYesNo(MsgTolbiWarriorRemember, speaker);
  Engine_EventEnd();
}

// The priest: across the counter (the leader facing north) opens the
// sanctum; otherwise talks about the festival.
static void TolbiInteriors_TalkToPriest(s32 speaker) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (FACING_NEAREST_CARDINAL(leader->facing) == FACING_NORTH) {
    Engine_SanctumOpen(speaker);
  } else {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
      Engine_EventSetMessage(MsgTolbiShipsArentGoing);
      Engine_EventShowMessage(speaker, 0);
    } else if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
      Engine_EventSetMessage(MsgTolbiMissFinalsTolbis);
      Engine_EventShowMessage(speaker, 0);
    } else {
      Engine_EventSetMessage(MsgTolbiWasteStuckHereWhenSuch);
      Engine_EventShowMessage(speaker, 0);
    }
  }
}

// An actor that squashes and stretches twice, then rests 40 frames.
s32 gTolbiInteriorsSquashStretch[] = {
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_ADD(SW_SCALE_X, -0x5000),
    SC_ADD(SW_SCALE_Y, 0x1000),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 0x2800),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 0x1400),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 0x1400),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, -0x5000),
    SC_ADD(SW_SCALE_Y, 0x1000),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 0x2800),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 0x1400),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 0x1400),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(40),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gTolbiInteriorsEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 160, 0, 216, FACING_NORTH, 0, 0, 0, 256, 256, 0},
    {2, CONDITION_ALWAYS, 336, 0, 200, FACING_NORTH, 0, 248, 64, 488, 216, 0},
    {3, CONDITION_ALWAYS, 544, 0, 200, FACING_NORTH, 0, 520, 48, 696, 216, 0},
    {4, CONDITION_ALWAYS, 792, 0, 216, FACING_NORTH, 0, 688, 0, 960, 240, 0},
    {5, CONDITION_ALWAYS, 544, 0, 424, FACING_NORTH, 0, 488, 264, 696, 448, 0},
    {6, CONDITION_ALWAYS, 312, 0, 472, FACING_NORTH, 0, 208, 272, 456, 488, 0},
    {8, CONDITION_ALWAYS, 824, 0, 528, FACING_NORTH, 0, 688, 248, 944, 568, 0},
    {9, CONDITION_ALWAYS, 128, 0, 768, FACING_NORTH, 0, 0, 536, 304, 800, 0},
    {10, CONDITION_ALWAYS, 56, 0, 608, FACING_NORTH, 0, 0, 536, 304, 800, 0},
    {11, CONDITION_ALWAYS, 464, 0, 624, FACING_SOUTH, 0, 320, 568, 616,
     TOLBI_INTERIORS_INN_CAMERA_MAX_Z, 0},
    {12, CONDITION_ALWAYS, 392, 0, 728, FACING_NORTH, 0, 320, 568, 616,
     TOLBI_INTERIORS_INN_CAMERA_MAX_Z, 0},
    {13, CONDITION_ALWAYS, 536, 0, 728, FACING_NORTH, 0, 320, 568, 616,
     TOLBI_INTERIORS_INN_CAMERA_MAX_Z, 0},
    {14, CONDITION_ALWAYS, 392, 0, 680, FACING_SOUTH, 0, 320, 568, 616,
     TOLBI_INTERIORS_INN_CAMERA_MAX_Z, 0},
    {15, CONDITION_ALWAYS, 536, 0, 680, FACING_SOUTH, 0, 320, 568, 616,
     TOLBI_INTERIORS_INN_CAMERA_MAX_Z, 0},
    {16, CONDITION_ALWAYS, 808, 0, 664, FACING_SOUTH, 0, 704, 608,
     TOLBI_INTERIORS_EAST_ROOM_CAMERA_MAX_X, 768, 0},
    {17, CONDITION_ALWAYS, 840, 0, 824, FACING_SOUTH, 0, 704, 768,
     TOLBI_INTERIORS_EAST_ROOM_CAMERA_MAX_X, 928, 0},
    {18, CONDITION_ALWAYS, 440, 0, 920, FACING_NORTH, 0, 288, 768, 528,
     TOLBI_INTERIORS_SOUTH_ROOM_CAMERA_MAX_Z, 0},
    {19, CONDITION_ALWAYS, 584, 0, 920, FACING_NORTH, 0, 496, 768, 736,
     TOLBI_INTERIORS_SOUTH_ROOM_CAMERA_MAX_Z, 0},

    {30, CONDITION_ALWAYS, 824, 0, 352, FACING_NORTH, 0, 688, 248, 944, 568, 0},
    {SCENE_TABLE_END},
};

u32 gTolbiInteriorsExits[] = {
    SCENE_EXITS(136),        SCENE_EXIT(1, 135, 2),   SCENE_EXIT(2, 135, 3),
    SCENE_EXIT(3, 135, 4),   SCENE_EXIT(4, 135, 5),   SCENE_EXIT(5, 135, 6),
    SCENE_EXIT(6, 135, 7),   SCENE_EXIT(7, 135, 8),   SCENE_EXIT(8, 135, 9),
    SCENE_EXIT(9, 135, 10),  SCENE_EXIT(10, 136, 11), SCENE_EXIT(11, 136, 10),
    SCENE_EXIT(12, 136, 17), SCENE_EXIT(13, 136, 16), SCENE_EXIT(14, 136, 18),
    SCENE_EXIT(15, 136, 19), SCENE_EXIT(16, 136, 13), SCENE_EXIT(17, 136, 12),
    SCENE_EXIT(18, 136, 14), SCENE_EXIT(19, 136, 15), SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gTolbiInteriorsPlacements[] = {
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(168), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(152), 0, PIXELS(184), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(104), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6d, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(144), 0, PIXELS(104), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(352), 0, PIXELS(120), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(568), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(600), 0, PIXELS(198), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(824), 0, PIXELS(144), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(840), 0, PIXELS(106), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(608), 0, PIXELS(348), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(536), 0, PIXELS(392), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x7c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(360), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x7d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(360), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x76, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(440), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x77, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(106), 0, PIXELS(696), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { 0x80, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(832), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(488), 0, PIXELS(688), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(584), 0, PIXELS(848), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x84, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(592), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x94, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(230), 0, PIXELS(648), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x42, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(864), 0, PIXELS(848), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x43, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(848), 0, PIXELS(864), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x79, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(824), 0, PIXELS(328), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gTolbiInteriorsPlacementsAfterColosso[] = {
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(168), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(152), 0, PIXELS(184), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(392), 0, PIXELS(168), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(120), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(520), 0, PIXELS(520), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(568), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(600), 0, PIXELS(198), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(824), 0, PIXELS(144), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(840), 0, PIXELS(106), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(608), 0, PIXELS(348), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(536), 0, PIXELS(392), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x7c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(360), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x7d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(360), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x76, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(440), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x77, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(106), 0, PIXELS(696), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { 0x80, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(112), 0, PIXELS(600), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(488), 0, PIXELS(688), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(568), 0, PIXELS(848), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x84, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(592), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x94, CONDITION_ALWAYS, (s32)gTolbiInteriorsSquashStretch,
        PIXELS(230), 0, PIXELS(648), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x42, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(864), 0, PIXELS(848), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x43, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(848), 0, PIXELS(864), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x79, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(824), 0, PIXELS(328), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(848), 0, PIXELS(712), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(816), 0, PIXELS(712), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(848), 0, PIXELS(688), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(856), 0, PIXELS(672), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(440), 0, PIXELS(864), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(416), 0, PIXELS(840), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gTolbiInteriorsEventsFestival[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_EnterDoor) },
    { EVENT_EXIT, 18, CONDITION_ALWAYS, 18 },
    { EVENT_EXIT, 19, CONDITION_ALWAYS, 19 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTolbiIsPackedWithTouristsDuring) },
    { EVENT_TALK, TOLBI_INTERIORS_HOST, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToHost) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsICouldntSleepAtAllLast) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheCrowdsAtTheColosseumMade) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWePaidGoodMoneyToRent) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIReallyWantToSeeColosso) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWahhhIWantToSeeColosso) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWonderIfTheyReallyCant) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBabiWasntAtColossoAccordingTo) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWereGoingToMakeSomeMoney) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIHeardBabiIsUsingThe) },
    { EVENT_TALK, TOLBI_INTERIORS_WEAPON_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToWeaponShopkeeper) },
    { EVENT_TALK, TOLBI_INTERIORS_ITEM_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToItemShopkeeper) },
    { EVENT_TALK, TOLBI_INTERIORS_ARMOR_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToArmorShopkeeper) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWereFullyBookedWithReservationsDuring) },
    { EVENT_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsColossoIsTheBusiestTimeOf) },
    { EVENT_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWereSoBusyRunningTheInn) },
    { EVENT_TALK, TOLBI_INTERIORS_MEDAL_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsATourGroupCameFromKalay) },
    { EVENT_TALK, TOLBI_INTERIORS_CHEF, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWeArentGoingToGetTo) },
    { EVENT_TALK, TOLBI_INTERIORS_WAITER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImNotVeryInterestedInColosso) },
    { EVENT_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOhNoWeBarelyMadeIt) },
    { EVENT_TALK, 29, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfWedGottenHereJustA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOnceColossoStartsWereGoingTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsItsTooLateToStartLooking) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfICouldHaveSleptLast) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHuhUhCrowdsAreADefinite) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWeGetToStayHereUntil) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWasStubbornISaidIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIKeepOnCryingButDaddy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsItsStrangeThatBabisSoldiersAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsItsStrangeBabiLovesColossoBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHeJustTakesACutOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsNamingTheLighthouseHeBuiltAfter) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIGuessINeverWouldHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheyHaveSomeNiceThingsFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBabiChoosesColossosVictorsToBe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheTourGroupIsFeelingCramped) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWeCantHandleTooManyMore) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImGoingToGoToThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThereArentEnoughBedsForThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheInnkeeperToldMeToCook) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIHaveAHeadacheJustThinking) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsEvenThoughICantEnterColosso) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfWeCouldHaveEnteredColosso) },
    { EVENT_TALK, TOLBI_INTERIORS_PRIEST, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToPriest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWonderHowTheTryoutsAre) },
    { SEARCH(SEARCH_BARREL), TOLBI_INTERIORS_VIAL_BARREL, 0xf9a, FIND_ITEM(ITEM_VIAL) },
    { SEARCH(SEARCH_BARREL), TOLBI_INTERIORS_MEDAL_BARREL, 0xf9b, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_OVEN), TOLBI_INTERIORS_PIZZA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsShesMakingTheWorldRenownedTolbi)) },
    { SEARCH(SEARCH_OVEN), TOLBI_INTERIORS_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsHeIsBoilingThePastaIt)) },
    { SEARCH(SEARCH_OVEN), TOLBI_INTERIORS_SEAFOOD_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsItsTolbiSeafoodPastaWithLots)) },
    { SEARCH(SEARCH_UNNAMED), TOLBI_INTERIORS_INN_SIGN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsIsaacCheckedTheSignInnFull)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gTolbiInteriorsEventsFinalsDay[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_EnterDoor) },
    { EVENT_EXIT, 18, CONDITION_ALWAYS, 18 },
    { EVENT_EXIT, 19, CONDITION_ALWAYS, 19 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSoColossoHasAlreadyReachedThe) },
    { EVENT_TALK, TOLBI_INTERIORS_HOST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheFestivalScheduleWasDelayedFor) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThatsStrangeIWonderWhatHappened) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIJustSawBabiTheTowns) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHereIAmInTolbiFor) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImGoingToWatchTheFinals) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHoorayWereGoingToColossoNow) },
#if TBS_PATCH_4
    { EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_AskWhetherBabiWasMissing) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBabiDecidedShowUpForFinals) },
#else
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWellIGuessSoSinceHes) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHuhThenTheyFoundHimIn) },
#endif
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBabiApparentlyHasSomeReasonFor) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIHeardBabiIsSearchingFor) },
    { EVENT_TALK, TOLBI_INTERIORS_WEAPON_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToWeaponShopkeeper) },
    { EVENT_TALK, TOLBI_INTERIORS_ITEM_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToItemShopkeeper) },
    { EVENT_TALK, TOLBI_INTERIORS_ARMOR_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToArmorShopkeeper) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOnceTheFinalsBeginEveryoneTries) },
    { EVENT_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThisYearImNotLettingAnyone) },
    { EVENT_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheWifeHasACloseEye) },
    { EVENT_TALK, TOLBI_INTERIORS_MEDAL_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHmmIThoughtAllMenLoved) },
    { EVENT_TALK, TOLBI_INTERIORS_CHEF, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheMomentIHearTheFinals) },
    { EVENT_TALK, TOLBI_INTERIORS_WAITER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSoColossoIsEndingSoonIts) },
    { EVENT_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheColossoFinalsAreStartingSoon) },
    { EVENT_TALK, 29, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsMaybeIllDoSomeResearchOn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsAreWeMakingMoreMoneyNow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSoTheyDidExtendTheFestival) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsICantAffordToStayHere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheAirInTheColosseumIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfSomebodyElseWouldWatchThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsColossoIsAlreadyAtTheFinals) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSinceWeDidntGetToSee) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSoWhatWereBabisSoldiersDoing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsMaybeWatchingTheTrialsIsToo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBeforeHeStartedTheLighthouseBabi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBabiHasBeenMakingStrangeReferences) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheTouristsAllBuyWeaponsAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsYouCouldSayTheWarriorsIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfMomFindsOutIWent) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImTheOneWhoReallyWants) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOnceThingsSettleDownImSure) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfTheMasterSlipsOutThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWeWomenAreTheOnlyOnes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHasntTheMasterGoneToColosso) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIDontHaveAnyParticularInterest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWantToSeeTheFinals) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheTrialBattlesWerentQuiteLike) },
    { EVENT_TALK, TOLBI_INTERIORS_PRIEST, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToPriest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfYoureNotInterestedInThe) },
    { SEARCH(SEARCH_BARREL), TOLBI_INTERIORS_VIAL_BARREL, 0xf9a, FIND_ITEM(ITEM_VIAL) },
    { SEARCH(SEARCH_BARREL), TOLBI_INTERIORS_MEDAL_BARREL, 0xf9b, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_OVEN), TOLBI_INTERIORS_PIZZA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsShesMakingTheWorldRenownedTolbi)) },
    { SEARCH(SEARCH_OVEN), TOLBI_INTERIORS_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsHeIsBoilingThePastaIt)) },
    { SEARCH(SEARCH_OVEN), TOLBI_INTERIORS_SEAFOOD_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsItsTolbiSeafoodPastaWithLots)) },
    { SEARCH(SEARCH_UNNAMED), TOLBI_INTERIORS_INN_SIGN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsIsaacCheckedTheSignInnFull)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gTolbiInteriorsEventsAfterColosso[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_EnterDoor) },
    { EVENT_EXIT, 18, CONDITION_ALWAYS, 18 },
    { EVENT_EXIT, 19, CONDITION_ALWAYS, 19 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsNowThatColossoIsOverWeve) },
    { EVENT_TALK, TOLBI_INTERIORS_HOST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWeHadALotOfGuests) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIRentedMyHouseToA) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheTouristsAreAllInA) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWasSoMovedByThis) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThatLittleGuyJumpedStraightInto) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsDidYouKnowMtAlephErupted) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsMaybeAlephsEruptionCausedThoseSea) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheWarriorsSaidTheyWouldRush) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThereWontBeManyCustomersUntil) },
    { EVENT_TALK, TOLBI_INTERIORS_WEAPON_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToWeaponShopkeeper) },
    { EVENT_TALK, TOLBI_INTERIORS_ITEM_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToItemShopkeeper) },
    { EVENT_TALK, TOLBI_INTERIORS_ARMOR_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToArmorShopkeeper) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIThoughtWedHaveSomeFree) },
    { EVENT_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheSpectatorsFromTheEastCant) },
    { EVENT_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsItsBusyAllDayEveryDay) },
    { EVENT_TALK, TOLBI_INTERIORS_MEDAL_WOMAN, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToMedalWoman) },
    { EVENT_TALK, TOLBI_INTERIORS_CHEF, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToChef) },
    { EVENT_TALK, TOLBI_INTERIORS_WAITER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToWaiter) },
    { EVENT_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWentToThePortBut) },
    { EVENT_TALK, 29, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsColossoHasEndedButWeStill) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSighMakingTheBedsHasWorn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIdLoveAVacationButWhere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOnceTheShipsStartSailingAgain) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIHeardOfACaveThat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheWarriorsWhoFoughtInThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsDaringAndBraveryAreNiceAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheSparklingStonesThatRainedFrom) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIllBetTheMonstersWereFrightened) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsItBothersMeABitThat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThereArentManyMerchantsAtThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsEvenWithoutTheShipsICould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsDidAllTheWarriorsWhoFought) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImGonnaWatchTheFinalsNext) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSinceTheShipsArentSailingThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHeeHeeItsGreatTheInns) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsEveryoneIsWorkingOnBabiLighthouse) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TOLBI_INTERIORS_MEDAL_WOMAN, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_ReadMedalWomanMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsGrrrIDidntGetToSee) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHeyChefSoWhatIfYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsITriedGoingToTheEastern) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsYoudThinkYouCouldGetThrough) },
    { EVENT_TALK, TOLBI_INTERIORS_PRIEST, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToPriest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsYouAllGotToSeeColosso) },
    { EVENT_TALK, TOLBI_INTERIORS_ANGRY_SAILOR, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToAngrySailor) },
    { EVENT_TALK, 32, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOnceThisGuyGetsMadTheres) },
    { EVENT_TALK, 33, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWonderIfThisReallyIs) },
    { EVENT_TALK, 34, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTolbiGetsSoBoringOnceThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 31, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsYoureSayingIfWeRowWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 32, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheCrewIsSayingThatIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 33, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImSickOfStayingAtThis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 34, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfMomHadJustBoughtMe) },
    { EVENT_TALK, TOLBI_INTERIORS_WOULD_BE_WARRIOR, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiInteriors_TalkToWouldBeWarrior) },
    { EVENT_TALK, 36, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIGotToSeeColossoSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 35, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThatVoyageAcrossTheSeaWas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 36, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsNoMatterWhatHappensImComing) },
    { SEARCH(SEARCH_BARREL), TOLBI_INTERIORS_VIAL_BARREL, 0xf9a, FIND_ITEM(ITEM_VIAL) },
    { SEARCH(SEARCH_BARREL), TOLBI_INTERIORS_MEDAL_BARREL, 0xf9b, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_OVEN), TOLBI_INTERIORS_PIZZA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsShesMakingTheWorldRenownedTolbi)) },
    { SEARCH(SEARCH_OVEN), TOLBI_INTERIORS_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsHeIsBoilingThePastaIt)) },
    { SEARCH(SEARCH_OVEN), TOLBI_INTERIORS_SEAFOOD_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsItsTolbiSeafoodPastaWithLots)) },
    { SEARCH(SEARCH_UNNAMED), TOLBI_INTERIORS_INN_SIGN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsIsaacCheckedTheSignInnFull)) },
    { SEARCH(SEARCH_BARREL), TOLBI_INTERIORS_HIDDEN_MEDAL_BARREL, FLAG_TOLBI_INTERIORS_MEDAL_TAKEN, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SCENE_EVENTS_END },
};
// clang-format on

u16 gTolbiInteriorsDoorOpening[] = {
    MAP_CELL_STEP(2, 123, 1, 2, 6),
    MAP_CELL_STEP(4, 123, 1, 2, 6),
    MAP_CELL_STEPS_END,
};

struct MapCellAnim gTolbiInteriorsDoors[] = {
    {gTolbiInteriorsDoorOpening, 24, 100},
    {gTolbiInteriorsDoorOpening, 33, 100},
    {gTolbiInteriorsDoorOpening, 50, 99},
    {gTolbiInteriorsDoorOpening, 52, 109},
};

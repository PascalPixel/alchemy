/// @file
/// Madra's houses: the inn, the shops, the elder's home with the Black Crystal
/// on its mantel, the jail where Piers is questioned (entrance 20), and, as a
/// second scene, the left cavern home. The old couple trade Char the Mars
/// Djinni for the Healing Fungus, and on the return with Piers the elder's
/// household welcomes him home.

#include "shared/tables.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "scene/scene.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "scene_ids.h"
#include "field/event_inline.h"
#include "version.h"
#include "field/field_sprite.h"
#include "field/field_sequence_event.h"
#include "field/actor_script.h"
#include "djinn_ids.h"

/// Set once the old man of the mushroom house has been given the Healing
/// Fungus and has traded Char for it.
#define FLAG_MADRA_HEALING_FUNGUS_GIVEN 0x98a
/// Set as the Healing Fungus is given, for this visit: the couple look forward
/// to dinner until the party leaves the area.
#define FLAG_MADRA_FUNGUS_FOR_DINNER FLAG_AREA_TEMP_10
/// Set once the old man has been offered the Laughing Fungus on this visit.
#define FLAG_MADRA_LAUGHING_FUNGUS_OFFERED FLAG_SCENE_TEMP_10
/// Set once the ill man has groaned on this visit.
#define FLAG_MADRA_ILL_MAN_GROANED FLAG_SCENE_TEMP_11

/// Madra's houses, numbering the rows gMadraInteriors_Placements and
/// gMadraInteriors_PlacementsOnReturn share.
enum MadraInteriorsActorId {
  /// A couple who fear what lies south in Gondowan, and then the Kibombo.
  ACTOR_OLD_MAN_WARY_OF_GONDOWAN = ACTOR_FIRST_PLACED,
  ACTOR_WOMAN_WARY_OF_GONDOWAN,
  /// The old man who loves the rare mushrooms of the Gondowan Cliffs.
  ACTOR_MUSHROOM_LOVER,
  /// His wife, who cooks them.
  ACTOR_MUSHROOM_COOK,
  /// Two men curious about the strange ship grounded in the cove to the east.
  ACTOR_MAN_ON_THE_STRANGE_SHIP,
  ACTOR_MAN_ON_THE_GROUNDED_SHIP,
  /// The elder's household: who keeps the jewel on his mantel with Milady,
  /// his daughter, the old woman of the house, and his two grandchildren.
  ACTOR_JEWEL_KEEPER,
  ACTOR_ELDERS_DAUGHTER,
  ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE,
  ACTOR_ELDERS_GRANDSON,
  ACTOR_ELDERS_GRANDDAUGHTER,
  /// The sanctum's healer and the two women who pray there.
  ACTOR_HEALER,
  ACTOR_WOMAN_OF_THE_SANCTUM,
  ACTOR_OLD_WOMAN_OF_THE_SANCTUM,
  ACTOR_INNKEEPER,
  /// Who helps at the inn, sure it is safe from the Champa.
  ACTOR_INN_HELPER,
  /// A girl staying at the inn, who hid under her bed in the raid.
  ACTOR_GIRL_STAYING_AT_THE_INN,
  /// Char, the Mars Djinni the mushroom couple keep.
  ACTOR_CHAR,
  /// A blank actor on the elder's mantel that shows the Black Crystal
  /// (MadraInteriors_PlaceBlackCrystal).
  ACTOR_BLACK_CRYSTAL
};

/// The rest of gMadraInteriors_Placements.
enum MadraInteriorsTravelersActorId {
  /// Two travelers at the inn, the first slept through the pirate raid; his
  /// partner wears the same sprite in other colours.
  ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID = ACTOR_BLACK_CRYSTAL + 1,
  ACTOR_TRAVELERS_PARTNER,
  /// The two warriors on their way to Colosso in Tolbi, there once flag 0x821
  /// is set: "I'm Morgan, and this here's Dekka."
  ACTOR_MORGAN,
  ACTOR_DEKKA
};

/// The rest of gMadraInteriors_PlacementsOnReturn, when the party brings Piers
/// home. The rows for Piers, Jenna and Sheba go to their party slots.
enum MadraInteriorsReturnActorId {
  /// The elder, who held Piers' jewel, and Madra's mayor.
  ACTOR_ELDER = ACTOR_BLACK_CRYSTAL + 1,
  ACTOR_MAYOR,
  /// Unused: two more rows with Kraden's sprite, out of sight.
  ACTOR_UNUSED_FIRST_KRADEN,
  ACTOR_UNUSED_SECOND_KRADEN,
  ACTOR_KRADEN,
  ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID_ON_RETURN,
  ACTOR_TRAVELERS_PARTNER_ON_RETURN
};

/// The jail's actors, by the rows of gMadraInteriors_JailPlacements; Piers' row
/// goes to his party slot.
enum MadraInteriorsJailActorId {
  /// The puddle Piers freezes into a shaft of ice.
  ACTOR_JAIL_PUDDLE = ACTOR_FIRST_PLACED,
  /// The two who question Piers.
  ACTOR_STERN_INTERROGATOR,
  ACTOR_ACCUSING_INTERROGATOR,
  /// Kraden, who jumps out from behind Felix.
  ACTOR_JAIL_KRADEN,
  ACTOR_SECOND_JAIL_PUDDLE,
  /// Once FLAG_GABOMBA_MESSAGE_CARRIED is set: who saw Isaac's party hear of
  /// Felix, and the ill man who has feared the cold since Piers' ice.
  ACTOR_WITNESS_ON_ISAACS_PARTY,
  ACTOR_ILL_MAN
};

/// The left cavern home (MadraInteriors2), by the rows of
/// gMadraInteriors_SecondRoomsPlacements.
enum MadraInteriorsCavernHomeActorId {
  /// A couple who talk of Alhafra, and later of a plan to make money.
  ACTOR_MAN_ON_ALHAFRA = ACTOR_FIRST_PLACED,
  ACTOR_WOMAN_ON_ALHAFRA,
  /// A woman lying ill in bed until Briggs is caught.
  ACTOR_SISTER_IN_BED,
  /// Her sister, glad she is better.
  ACTOR_HER_SISTER
};

/// Madra's inn, as Inn_CheckIn numbers the inns.
enum { MADRA_INN = 1 };

extern s32 gMadraInteriors_InterrogatorLifted[];
extern s32 gMadraInteriors_InterrogatorTopples[];
void MadraInteriors_SayGoodbyeAgain(void);

void MadraInteriors_AskToStayLonger(void);

extern u32 gMadraInteriors_OldManWalksToBackRoom[];
extern u32 gMadraInteriors_WifeComesBack[];
extern u32 gMadraInteriors_OldManComesBack[];

extern u32 gMadraInteriors_CharFollows[];

void MadraInteriors_CountAnswer();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "text/msg_ids.h"

extern struct ActorPlacement gMadraInteriors_Placements[];
extern struct ActorPlacement gMadraInteriors_PlacementsOnReturn[];
extern struct ActorPlacement gMadraInteriors_JailPlacements[];
extern struct ActorPlacement gMadraInteriors_SecondRoomsPlacements[];
extern struct Trigger gMadraInteriors_TriggersBeforeRaid[];
extern struct Trigger gMadraInteriors_JailTriggers[];
extern struct Trigger gMadraInteriors_SecondRoomsTriggers[];
extern struct Trigger gMadraInteriors_TriggersAfterRaid[];
extern struct Trigger gMadraInteriors_TriggersOnReturn[];

void Event_End(void);
void Sanctum_Run(s32 actor);
void Inn_CheckIn(s32 slot, s32 actor);
void MadraInteriors_InterrogatePiers(void);

#include "text/msg_ids.h"

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_MADRA_INTERIORS_2)
    return gMadraInteriors_SecondRoomsPlacements;
  if (gMainRoutineWork.entrance == 20)
    return gMadraInteriors_JailPlacements;
  if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
    return gMadraInteriors_PlacementsOnReturn;
  return gMadraInteriors_Placements;
}

/// "Indra is in direct contact with southern Gondowan... Will we be safe?"
void MadraInteriors_AskWillWeBeSafe(void) {
  s32 msg;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  msg = (s32)MsgMadraInteriors06;
  FieldEvent_SetMessageId(msg);
  // "Indra is in direct contact with southern Gondowan... Will we be safe?"
  Event_OpenMessage(ACTOR_WOMAN_WARY_OF_GONDOWAN, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(ACTOR_WOMAN_WARY_OF_GONDOWAN, 0);
  Event_End();
}

/// Madra's innkeeper takes the party in across the counter (inn 1), and
/// otherwise talks of the week's troubles, the Kibombo, and later the dead fish
/// on the shore, which from patch 1 is a question.
void MadraInteriors_TalkToInnkeeper(void) {
  u16 facing = Actor_Get(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Inn_CheckIn(MADRA_INN, ACTOR_INNKEEPER);
  } else {
#if !TLA_PATCH_1
    // Before patch 1 there is no choice prompt.
    if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
      FieldEvent_SetMessageId((s32)MsgMadraInteriors155);
    else if (Event_GetFlag(FLAG_BRIGGS_CAUGHT))
      FieldEvent_SetMessageId((s32)MsgMadraInteriors87);
    else
      FieldEvent_SetMessageId((s32)MsgMadraInteriors39);
    // "Earthquakes, pirate attacks... What a week!"
    Event_ShowMessage(ACTOR_INNKEEPER, 0);
#else
    if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED)) {
      s32 msg = (s32)MsgMadraInteriors155;

      FieldEvent_SetMessageId(msg);
      Event_OpenMessage(ACTOR_INNKEEPER, 0);
      if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(),
                                                0) == 0) {
        FieldEvent_Wait(10);
        FieldEvent_SetMessageId(msg + 1);
      } else {
        FieldEvent_Wait(20);
        FieldEvent_SetMessageId(msg + 2);
      }
      Event_ShowMessage(ACTOR_INNKEEPER, 0);
    } else {
      if (Event_GetFlag(FLAG_BRIGGS_CAUGHT))
        FieldEvent_SetMessageId((s32)MsgMadraInteriors87);
      else
        FieldEvent_SetMessageId((s32)MsgMadraInteriors39);
      // "Earthquakes, pirate attacks... What a week!"
      Event_ShowMessage(ACTOR_INNKEEPER, 0);
    }
#endif
  }
}

/// Madra's sanctum healer heals across the counter, and otherwise talks of the
/// town's trials, the Kibombo, and later the warming sea.
void MadraInteriors_TalkToHealer(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Sanctum_Run(actor);
  } else {
    if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
      FieldEvent_SetMessageId((s32)MsgMadraInteriors149);
    else if (Event_GetFlag(FLAG_BRIGGS_CAUGHT))
      FieldEvent_SetMessageId((s32)MsgMadraInteriors81);
    else
      FieldEvent_SetMessageId((s32)MsgMadraInteriors33);
    // "Surely this town is enduring some sort of test... We must not fail..."
    Event_ShowMessage(actor, 0);
  }
}

/// "Don't touch that gem! The elder gave us strict orders to protect it!"
void MadraInteriors_GuardElderGem(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceActor(ACTOR_JEWEL_KEEPER, ACTOR_FELIX, 0);
  FieldActor_Jump(ACTOR_JEWEL_KEEPER, 6, 20);
  Actor_FaceActor(ACTOR_FELIX, ACTOR_JEWEL_KEEPER, 0);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors18);
  // "Don't touch that gem! The elder gave us strict orders to protect it!"
  Event_ShowMessage(ACTOR_JEWEL_KEEPER, 0);
  Actor_FaceDirection(ACTOR_JEWEL_KEEPER, FACING_SOUTHEAST, 0);
  Event_End();
}

/// The old man who loves the rare mushrooms of the Gondowan Cliffs, before and
/// after the party brings him one.
void MadraInteriors_TalkToMushroomLover(void) {
  s32 msg;

  if (Event_GetFlag(FLAG_MADRA_HEALING_FUNGUS_GIVEN)) {
    if (Event_GetFlag(FLAG_MADRA_FUNGUS_FOR_DINNER))
      msg = (s32)MsgMadraInteriors111;
    else
      msg = (s32)MsgMadraInteriors118;
    FieldEvent_SetMessageId(msg);
    // "That mushroom was great, but I ate way too much. I don't ever want..."
    Event_ShowMessage(ACTOR_MUSHROOM_LOVER, 0);
  } else {
    if (Event_GetFlag(FLAG_MADRA_LAUGHING_FUNGUS_OFFERED)) {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors116);
      // "Thanks for bringing this by, but it's just not the type of..."
      Event_ShowMessage(ACTOR_MUSHROOM_LOVER, 0);
    } else {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors106);
      // "How are we to climb the cliffs between here and Gondowan? If..."
      Event_ShowMessage(ACTOR_MUSHROOM_LOVER, 0);
    }
    gFieldWork->fieldMenu = 1;
  }
}

/// His wife, who cooks them.
void MadraInteriors_TalkToMushroomCook(void) {
  if (Event_GetFlag(FLAG_MADRA_HEALING_FUNGUS_GIVEN)) {
    if (Event_GetFlag(FLAG_MADRA_FUNGUS_FOR_DINNER)) {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors112);
      // "This here's enough of a mushroom that we'll be eating it for days!"
      Event_ShowMessage(ACTOR_MUSHROOM_COOK, 0);
    } else {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors119);
      // "Actually, I didn't cook the mushrooms properly that night. But..."
      Event_ShowMessage(ACTOR_MUSHROOM_COOK, 0);
    }
  } else if (Event_GetFlag(FLAG_MADRA_LAUGHING_FUNGUS_OFFERED)) {
    FieldEvent_SetMessageId((s32)MsgMadraInteriors117);
    // "Please... Won't you please try to find the mushrooms my husband..."
    Event_ShowMessage(ACTOR_MUSHROOM_COOK, 0);
  } else {
    FieldEvent_SetMessageId((s32)MsgMadraInteriors107);
    // "My husband relishes those rare mushrooms growing in the Gondowan..."
    Event_ShowMessage(ACTOR_MUSHROOM_COOK, 0);
  }
}

/// Mind Read on the mushroom lover: he thinks of the Healing Fungus he longs
/// for, and once he has it, of dinner.
void MadraInteriors_MindReadMushroomLover(void) {
  if (Event_GetFlag(FLAG_MADRA_HEALING_FUNGUS_GIVEN)) {
    if (Event_GetFlag(FLAG_MADRA_FUNGUS_FOR_DINNER)) {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors113);
      // "The more I think about dinner tonight, the more I drool..."
      Event_ShowMessage(ACTOR_MUSHROOM_LOVER, 0);
    } else {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors120);
      // "I don't remember mushrooms like that ever tasting bad..."
      Event_ShowMessage(ACTOR_MUSHROOM_LOVER, 0);
    }
  } else {
    s32 flag = Event_GetFlag(FLAG_MADRA_LAUGHING_FUNGUS_OFFERED);
    s32 msg = (s32)MsgMadraInteriors108;

    if (flag) {
      FieldEvent_SetMessageId(msg);
      Event_ShowMessage(ACTOR_MUSHROOM_LOVER, 0);
    } else {
      FieldEvent_SetMessageId(msg);
      Event_ShowMessage(ACTOR_MUSHROOM_LOVER, 0);
    }
  }
}

/// Mind Read on the mushroom cook: she thinks of her husband's mushrooms, and
/// once he has the Healing Fungus, of cooking it.
void MadraInteriors_MindReadMushroomCook(void) {
  if (Event_GetFlag(FLAG_MADRA_HEALING_FUNGUS_GIVEN)) {
    if (Event_GetFlag(FLAG_MADRA_FUNGUS_FOR_DINNER)) {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors114);
      // "These mushrooms will make any meal taste amazing!"
      Event_ShowMessage(ACTOR_MUSHROOM_COOK, 0);
    } else {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors121);
      // "If my husband finds out I messed up the recipe, he'll be so sad..."
      Event_ShowMessage(ACTOR_MUSHROOM_COOK, 0);
    }
  } else {
    s32 flag = Event_GetFlag(FLAG_MADRA_LAUGHING_FUNGUS_OFFERED);
    s32 msg = (s32)MsgMadraInteriors109;

    if (flag) {
      FieldEvent_SetMessageId(msg);
      Event_ShowMessage(ACTOR_MUSHROOM_COOK, 0);
    } else {
      FieldEvent_SetMessageId(msg);
      Event_ShowMessage(ACTOR_MUSHROOM_COOK, 0);
    }
  }
}

/// The ill man in the cavern home groans, the first time, and thinks of the ice
/// he saw made by thinking.
/// "Uhhhnnn... UHHHNNN!"
void MadraInteriors_TalkToIllMan(void) {
  if (!Event_GetFlag(FLAG_MADRA_ILL_MAN_GROANED)) {
    Event_SetFlag(FLAG_MADRA_ILL_MAN_GROANED);
    FieldEvent_SetMessageId((s32)MsgUhhhnnnUhhhnnn2);
    Actor_WalkByAndWait(ACTOR_ILL_MAN, -12, 0);
    FieldEvent_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_ILL_MAN, 2);
    FieldEvent_Wait(20);
    // "Uhhhnnn... UHHHNNN!"
    Event_ShowMessage(ACTOR_ILL_MAN, 0);
    FieldActor_ShowEmote(ACTOR_ILL_MAN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
    FieldEvent_Wait(20);
    Actor_WalkByAndWait(ACTOR_ILL_MAN, 12, 0);
    Actor_FaceActor(ACTOR_ILL_MAN, ACTOR_FELIX, 0);
    // "They told me that you make Psynergy by focusing your mind... I've..."
    Event_ShowMessage(ACTOR_ILL_MAN, 0);
  } else {
    FieldEvent_SetMessageId((s32)MsgMadraInteriors97);
    Actor_FaceActor(ACTOR_ILL_MAN, ACTOR_FELIX, 0);
    // "How can anyone just create ice by thinking about it? I mean, imagine..."
    Event_ShowMessage(ACTOR_ILL_MAN, 0);
  }
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_MADRA_INTERIORS_2)
    return gMadraInteriors_SecondRoomsTriggers;
  if (gMainRoutineWork.entrance == 20)
    return gMadraInteriors_JailTriggers;
  if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
    return gMadraInteriors_TriggersOnReturn;
  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT))
    return gMadraInteriors_TriggersAfterRaid;
  return gMadraInteriors_TriggersBeforeRaid;
}

/// The Black Crystal sits on the elder's mantel until the Kibombo steal it
/// (FLAG_BRIGGS_CAUGHT), after which its actor is put out of sight.
void MadraInteriors_PlaceBlackCrystal(void) {
  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT)) {
    Actor_SetPosition(ACTOR_BLACK_CRYSTAL, 888, 8);
  } else {
    Actor_SetItemIcon(Actor_Get(ACTOR_BLACK_CRYSTAL), ITEM_BLACK_CRYSTAL);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_BLACK_CRYSTAL), 0);
  }
}

/// Places an actor facing an angle, the angle taken as the u16 an actor's
/// facing is; the jail's four placements, named by us.
static inline void MadraInteriors_PlaceFacing(s32 actorId, s32 x, s32 z,
                                              u16 facing) {
  Actor_SetPositionAndFacing(actorId, x, z, facing);
}

/// The jail on entrance 20: Piers' interrogation the first time, then his
/// interrogators by the story flags.
void MadraInteriors_SetUpJail(void) {
  if (!Event_GetFlag(FLAG_BRIGGS_CAUGHT)) {
    gFieldWork->unkd4c = 3;
    if (!Event_GetFlag(0x911)) {
      MadraInteriors_InterrogatePiers();
    } else {
      Actor_SetPosition(ACTOR_STERN_INTERROGATOR, 0, 0);
      Actor_SetPosition(ACTOR_ACCUSING_INTERROGATOR, 0, 0);
    }
  } else if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED)) {
    MadraInteriors_PlaceFacing(ACTOR_STERN_INTERROGATOR, px(515), px(841),
                               FACING_NORTHEAST);
    MadraInteriors_PlaceFacing(ACTOR_ACCUSING_INTERROGATOR, px(472), px(877),
                               FACING_NORTH);
    Actor_SetPosition(ACTOR_JAIL_PUDDLE, 0, 0);
  } else {
    MadraInteriors_PlaceFacing(ACTOR_STERN_INTERROGATOR, px(515), px(841),
                               FACING_NORTHWEST);
    MadraInteriors_PlaceFacing(ACTOR_ACCUSING_INTERROGATOR, px(472), px(877),
                               FACING_NORTH);
    Actor_SetPosition(ACTOR_JAIL_PUDDLE, 0, 0);
  }
}

extern u32 gMadraInteriors_InterrogatorsLeave[];
void FieldCamera_WaitForArrival();
void FieldPsynergy_LoadResources();
void FieldPsynergy_SetupActorPair();
void BattleEffect_StopMainActor();

/// The rooms' setup: the leader's sprite priority, the two rooms' actors, and
/// the sister lying ill in the cavern home, by the story flags.
s32 Scene_Initialize(void) {
  struct FieldSprite *sprite;
  struct FieldWork *event;

  gFieldWork->transition = 521;
  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |= 32;
  FieldEffect_StartBlankOverlay(0);
  if (gMainRoutineWork.next == SCENE_MADRA_INTERIORS_1) {
    event = Ram_HeapSlots->fieldWork;
    event->unkd4c = 3;
    if (!Event_GetFlag(0x821)) {
      Actor_SetPosition(ACTOR_MORGAN, 0, 0);
      Actor_SetPosition(ACTOR_DEKKA, 0, 0);
    } else {
      FieldActor_SetSpriteColor(Actor_Get(ACTOR_MORGAN), 3);
      FieldActor_SetSpriteColor(Actor_Get(ACTOR_DEKKA), 5);
    }
    if (Event_GetFlag(FLAG_BRIGGS_CAUGHT)) {
      MapGrid_CopyCellsLayered(2, 110, 16, 10, 16, 64);
      MapGrid_CopyCellAttributes(2, 46, 16, 10, 16, 0);
    }
  }
  if (gMainRoutineWork.entrance == 20)
    MadraInteriors_SetUpJail();
  if (gMainRoutineWork.next == SCENE_MADRA_INTERIORS_2) {
    if (Event_GetFlag(FLAG_BRIGGS_CAUGHT)) {
      Actor_SetPosition(ACTOR_SISTER_IN_BED, 0, 0);
    } else {
      Actor_SetSpriteRotationById(ACTOR_SISTER_IN_BED, 0xc000);
      Actor_SetSpriteFlags(Actor_Get(ACTOR_SISTER_IN_BED), 0);
      ((struct FieldSprite *)Actor_Get(ACTOR_SISTER_IN_BED)->sprite)
          ->nextFrame |= 8;
      sprite = Actor_Get(ACTOR_SISTER_IN_BED)->sprite;
      sprite->unk11 = (sprite->unk11 & 3) | 32;
      ((struct FieldSprite *)Actor_Get(ACTOR_SISTER_IN_BED)->sprite)->unk19 = 1;
    }
  }
  MadraInteriors_PlaceBlackCrystal();
  if (Event_GetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_TRAVELERS_PARTNER_ON_RETURN), 2);
  else
    FieldActor_SetSpriteColor(Actor_Get(ACTOR_TRAVELERS_PARTNER), 2);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Tips an actor's sprite a thirty-second of a turn, a frame of the
/// interrogator toppling over (gMadraInteriors_InterrogatorTopples).
s32 MadraInteriors_TiltSprite(struct Actor *object) {
  struct FieldSprite *sprite;

  sprite = object->sprite;
  if (object != NULL && sprite != NULL)
    sprite->rotation -= 0x800;
  return 0;
}

/// Unused: stands an actor's sprite upright again; no script calls it.
UNUSED s32 MadraInteriors_StraightenSprite(struct Actor *object) {
  struct FieldSprite *sprite;

  sprite = object->sprite;
  if (object != NULL && sprite != NULL)
    sprite->rotation = 0;
  return 0;
}

/// The party enters the jail as Piers is questioned, and sees him drive off an
/// interrogator with Psynergy.
/// "Admit it! You're one of the Champa, aren't you, Piers?"
void MadraInteriors_InterrogatePiers(void) {
  s32 i;
  struct Actor *object0;
  struct Actor *object1;
  struct Actor *object2;
  struct Actor *object3;
  struct Actor *object4;
  struct Actor *object5;
  struct Actor *object6;
  struct Actor *object7;
  struct Actor *object8;
  struct Actor *object9;
  struct Actor *object10;
  struct Actor *object11;
  struct Actor *object12;
  s32 result13;
  struct Actor *object14;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(ACTOR_JAIL_PUDDLE, 1);
  Actor_SetPosition(ACTOR_FELIX, 400, 888);
  FieldCamera_MoveTo(408, CAMERA_KEEP, 848, 1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(ACTOR_PIERS, 0.8);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors28);
  // "Admit it! You're one of the Champa, aren't you, Piers?"
  FieldEvent_ShowMessageThenWait(ACTOR_ACCUSING_INTERROGATOR, 0, 10);
  Actor_WalkToAndWait(ACTOR_FELIX, 396, 869);
  FieldActor_LaunchFromLeader(ACTOR_JAIL_KRADEN, 25, 19, FACING_NORTH);
  FieldActor_WaitForScriptEndById(ACTOR_JAIL_KRADEN);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JAIL_KRADEN, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JAIL_KRADEN, FACING_NORTHEAST, 0);
  FieldEvent_Wait(10);
  FieldCamera_MoveTo(488, CAMERA_KEEP, 820, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_STERN_INTERROGATOR, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Speak! Or let your silence condemn you!"
  FieldEvent_ShowMessageThenWait(ACTOR_STERN_INTERROGATOR, 0, 10);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  // "I told you already... I'm not a Champa."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_STERN_INTERROGATOR, 2);
  // "Then where did you come from?"
  FieldEvent_ShowMessageThenWait(ACTOR_STERN_INTERROGATOR, 0, 10);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_NORTH);
  Actor_WalkByAndWait(ACTOR_PIERS, 0, -16);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  // "The heart of the Eastern Sea... If I told you where, you'd never..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  FieldActor_ShowEmote(ACTOR_STERN_INTERROGATOR,
                       EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  object0 = Actor_Get(ACTOR_STERN_INTERROGATOR);
  object0->steeringFlags &= 254;
  for (i = 2; i >= 0; i--) {
    Actor_WalkByAndWait(ACTOR_STERN_INTERROGATOR, 0, -8);
    Event_PlaySound(SE_HEAVY_IMPACT);
    Actor_WalkByAndWait(ACTOR_STERN_INTERROGATOR, 0, 8);
  }
  object1 = Actor_Get(ACTOR_STERN_INTERROGATOR);
  object1->steeringFlags |= 1;
  FieldEvent_Wait(30);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Look at me when I'm talking to you! Are you trying to mock me!?"
  FieldEvent_ShowMessageThenWait(ACTOR_STERN_INTERROGATOR, 0, 10);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  // "I implore you, do not anger me."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_STERN_INTERROGATOR, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Oh, do you? So what's going to happen if I make you angry?"
  FieldEvent_ShowMessageThenWait(ACTOR_STERN_INTERROGATOR, 0, 10);
  Actor_FaceActor(ACTOR_ACCUSING_INTERROGATOR, ACTOR_STERN_INTERROGATOR, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_ACCUSING_INTERROGATOR, 2);
  // "Hey, Shin... Maybe you should go easy on him..."
  FieldEvent_ShowMessageThenWait(ACTOR_ACCUSING_INTERROGATOR, 0, 10);
  Actor_FaceActor(ACTOR_STERN_INTERROGATOR, ACTOR_ACCUSING_INTERROGATOR, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_STERN_INTERROGATOR, 2);
  // "Hey, it wasn't your girlfriend who got hurt!"
  FieldEvent_ShowMessageThenWait(ACTOR_STERN_INTERROGATOR, 0, 10);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  // "I'm sorry she was injured, but I cannot°"
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  FieldActor_ShowEmote(ACTOR_STERN_INTERROGATOR, EMOTE_IN_FRONT | EMOTE_SCOWL,
                       20);
  Actor_FaceDirectionAndWait(ACTOR_STERN_INTERROGATOR, FACING_NORTH);
  // "I don't want your sympathy, freak! I want you to get angry for me!"
  FieldEvent_ShowMessageThenWait(ACTOR_STERN_INTERROGATOR, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  // "Stop, please... Even my patience has its limits."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_FaceDirection(ACTOR_STERN_INTERROGATOR, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  // "Hey, don't you want to see what he'll do?"
  FieldEvent_ShowMessageThenWait(ACTOR_STERN_INTERROGATOR, 0, 10);
  Actor_ShowEmote(ACTOR_ACCUSING_INTERROGATOR, EMOTE_IN_FRONT | EMOTE_WINCE,
                  30);
  // "I think we should just stop..."
  FieldEvent_ShowMessageThenWait(ACTOR_ACCUSING_INTERROGATOR, 0, 10);
  // "If my words will not cool your temper, then..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_PIERS, 0, 16);
  FieldPsynergy_LoadResources(ABILITY_FROST, 1);
  FieldPsynergy_SetupActorPair(ACTOR_PIERS, ACTOR_JAIL_PUDDLE);
  FieldPsynergy_StartCast();
  Actor_ShowEmote(ACTOR_STERN_INTERROGATOR, EMOTE_IN_FRONT | EMOTE_QUESTION,
                  30);
  Actor_FaceActor(ACTOR_STERN_INTERROGATOR, ACTOR_PIERS, 0);
  Actor_FaceActor(ACTOR_ACCUSING_INTERROGATOR, ACTOR_PIERS, 0);
  object2 = Actor_Get(ACTOR_STERN_INTERROGATOR);
  Actor_SetSpriteFlags(object2, 0);
  Actor_SetScript(ACTOR_STERN_INTERROGATOR, gMadraInteriors_InterrogatorLifted);
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  FieldPsynergy_End();
  // "Oh! That was Psynergy!"
  FieldEvent_ShowMessageThenWait(ACTOR_JAIL_KRADEN, 0, 10);
  FieldActor_Stop(ACTOR_STERN_INTERROGATOR);
  object3 = Actor_Get(ACTOR_STERN_INTERROGATOR);
  object3->scaleX = Q16_ONE;
  object4 = Actor_Get(ACTOR_STERN_INTERROGATOR);
  object4->scaleY = Q16_ONE;
  FieldEvent_Wait(4);
  object5 = Actor_Get(ACTOR_STERN_INTERROGATOR);
  object5->motionFlags = 2;
  object6 = Actor_Get(ACTOR_STERN_INTERROGATOR);
  object6->steeringFlags &= 254;
  Actor_SetDestinationOffset(ACTOR_STERN_INTERROGATOR, -8, 0);
  Actor_SetScript(ACTOR_STERN_INTERROGATOR,
                  gMadraInteriors_InterrogatorTopples);
  FieldActor_WaitForScriptEndById(ACTOR_STERN_INTERROGATOR);
  Event_PlaySound(SE_LANDING_THUD);
  FieldEvent_Wait(60);
  object7 = Actor_Get(ACTOR_STERN_INTERROGATOR);
  object7->steeringFlags = (object7->steeringFlags | 1);
  FieldActor_Jump(ACTOR_STERN_INTERROGATOR, 6, 0);
  FieldEvent_Wait(3);
  Actor_SetSpriteRotationById(ACTOR_STERN_INTERROGATOR, 0);
  object8 = Actor_Get(ACTOR_STERN_INTERROGATOR);
  Actor_SetSpriteFlags(object8, 1);
  Actor_FaceDirection(ACTOR_STERN_INTERROGATOR, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_ACCUSING_INTERROGATOR, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_ACCUSING_INTERROGATOR, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(ACTOR_ACCUSING_INTERROGATOR,
                       EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_RunRepeatedMotion(ACTOR_STERN_INTERROGATOR, 2);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  FieldActor_Jump(ACTOR_STERN_INTERROGATOR, 6, 20);
  FieldEvent_Wait(20);
  Actor_FaceActor(ACTOR_STERN_INTERROGATOR, ACTOR_PIERS, 0);
  FieldEvent_Wait(20);
  object9 = Actor_Get(ACTOR_ACCUSING_INTERROGATOR);
  object9->steeringFlags &= 254;
  object10 = Actor_Get(ACTOR_STERN_INTERROGATOR);
  object10->steeringFlags &= 254;
  Actor_WalkByAndWait(ACTOR_STERN_INTERROGATOR, -16, 16);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_STERN_INTERROGATOR, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  FieldActor_Jump(ACTOR_STERN_INTERROGATOR, 6, 20);
  FieldEvent_Wait(20);
  object11 = Actor_Get(ACTOR_STERN_INTERROGATOR);
  object11->steeringFlags = (object11->steeringFlags | 1);
  Actor_SetScript(ACTOR_STERN_INTERROGATOR, gMadraInteriors_InterrogatorsLeave);
  FieldActor_WatchAndSetScript(ACTOR_FELIX, ACTOR_STERN_INTERROGATOR);
  FieldActor_WatchAndSetScript(ACTOR_JAIL_KRADEN, ACTOR_STERN_INTERROGATOR);
  // "Waaaaaaaaaahh!"
  FieldEvent_ShowMessageThenWait(ACTOR_STERN_INTERROGATOR, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_ACCUSING_INTERROGATOR, 2);
  FieldEvent_Wait(20);
  Actor_FaceActor(ACTOR_ACCUSING_INTERROGATOR, ACTOR_PIERS, 0);
  FieldActor_WatchAndSetScript(ACTOR_FELIX, ACTOR_ACCUSING_INTERROGATOR);
  FieldActor_WatchAndSetScript(ACTOR_JAIL_KRADEN, ACTOR_ACCUSING_INTERROGATOR);
  // "Did you do that?"
  FieldEvent_ShowMessageThenWait(ACTOR_ACCUSING_INTERROGATOR, 0, 10);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  // "What did your friend expect?"
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_ACCUSING_INTERROGATOR, EMOTE_IN_FRONT | EMOTE_WINCE,
                  30);
  Actor_WalkByAndWait(ACTOR_ACCUSING_INTERROGATOR, -16, 16);
  // "You monster!"
  FieldEvent_ShowMessageThenWait(ACTOR_ACCUSING_INTERROGATOR, 0, 1);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  // "I... I am no monster."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_ACCUSING_INTERROGATOR, -16, 8);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_ACCUSING_INTERROGATOR, 2);
  FieldEvent_Wait(20);
  // "Help!!!"
  FieldEvent_ShowMessageThenWait(ACTOR_ACCUSING_INTERROGATOR, 0, 10);
  object12 = Actor_Get(ACTOR_ACCUSING_INTERROGATOR);
  object12->steeringFlags |= 1;
  Actor_SetScript(ACTOR_ACCUSING_INTERROGATOR,
                  gMadraInteriors_InterrogatorsLeave);
  FieldEvent_Wait(40);
  FieldActor_Stop(ACTOR_FELIX);
  FieldActor_Stop(ACTOR_JAIL_KRADEN);
  FieldActor_WaitForScriptEndById(ACTOR_ACCUSING_INTERROGATOR);
  Actor_SetPosition(ACTOR_STERN_INTERROGATOR, 0, 0);
  Actor_SetPosition(ACTOR_ACCUSING_INTERROGATOR, 0, 0);
  Actor_SetAnimation(ACTOR_JAIL_PUDDLE, 6);
  Actor_SetSpriteColorById(ACTOR_JAIL_PUDDLE, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_WaitForAnimationChange(8);
  EventCamera_FollowActor(ACTOR_FELIX, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceEachOther(ACTOR_JAIL_KRADEN, ACTOR_FELIX, 0);
  Actor_RunRepeatedMotion(ACTOR_JAIL_KRADEN, 2);
  FieldEvent_Wait(20);
  // "This man... He must be an Adept!"
  Event_OpenMessage(ACTOR_JAIL_KRADEN, 0);
  result13 = FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0);
  if (result13 != 0)
    goto answeredNo;
  Actor_RunRepeatedMotion(ACTOR_JAIL_KRADEN, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_JAIL_KRADEN, 0, 10);
  gFieldWork->message += 1;
  goto answered;
answeredNo:
  Actor_SetAnimationAndWait(ACTOR_JAIL_KRADEN, ANIM_SHAKE_HEAD);
  gFieldWork->message += 1;
  FieldEvent_ShowMessageThenWait(ACTOR_JAIL_KRADEN, 0, 10);
answered:
  Actor_RunRepeatedMotion(ACTOR_JAIL_KRADEN, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_JAIL_KRADEN, 0, 10);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_JAIL_KRADEN, ANIM_NOD);
  FieldEvent_Wait(10);
  Actor_SetAnimation(ACTOR_JAIL_KRADEN, ANIM_WALK);
  object14 = Actor_Get(ACTOR_FELIX);
  if (object14 == 0)
    goto piersLeft;
  Actor_SetDestination(ACTOR_JAIL_KRADEN, *(s16 *)((u8 *)object14 + 10),
                       *(s16 *)((u8 *)object14 + 18));
piersLeft:
  FieldActor_WaitForMoveById(ACTOR_JAIL_KRADEN);
  Actor_SetPosition(ACTOR_JAIL_KRADEN, 0, 0);
  Event_SetFlag(0x911);
  Event_End();
  return;
}

/// "Ah... When will that kindly elder return and end my imprisonment?"
void MadraInteriors_TalkToPiers(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors29);
  Actor_FaceDirection(ACTOR_JAIL_KRADEN, FACING_SOUTH, 0);
  // "Ah... When will that kindly elder return and end my imprisonment? I..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Event_End();
  return;
}

/// Piers feels the Mind Read.
/// "Why did I use my Psynergy in anger? I must maintain control..."
void MadraInteriors_MindReadPiers(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors30);
  // "Why did I use my Psynergy in anger? I must maintain control... My..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  // "I...sense someone... using Psynergy..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  // "That person... Could he be°no, he is not of my people..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  // "When will I be freed?"
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Event_End();
  return;
}

/// Unused: Piers' Psynergy at actor 8 alone, as the interrogation plays it;
/// nothing calls it.
UNUSED void MadraInteriors_PiersUsesPsynergy(void) {
  FieldPsynergy_LoadResources(ABILITY_FROST, 1);
  FieldPsynergy_SetupActorPair(7, 8);
  FieldPsynergy_StartCast();
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  FieldPsynergy_End();
  return;
}

extern u32 gMadraInteriors_WifeWalksToBackRoom[];
extern u32 gMadraInteriors_LeaderStepsAside[];

/// The couple are given the Healing Fungus from the Gondowan Cliffs, his
/// favourite, and trade the party Char the Mars Djinni for it.
/// "Oh, my... Is... Is that..."
void MadraInteriors_GiveHealingFungus(void) {
  struct Actor *object;

  Event_SetFlag(FLAG_MADRA_HEALING_FUNGUS_GIVEN);
  Event_SetFlag(FLAG_MADRA_FUNGUS_FOR_DINNER);
  PartyInventory_Remove(ITEM_HEALING_FUNGUS);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpeed(ACTOR_MUSHROOM_COOK, 0.4);
  Actor_WalkTo(ACTOR_MUSHROOM_COOK, 410, 109);
  object = Actor_Get(ACTOR_FELIX);
  if (object->z.fixed > px(95))
    Actor_WalkToAndWait(ACTOR_FELIX, object->x.fixed / 0x10000, 95);
  Actor_WalkToAndWait(ACTOR_FELIX, 362, 95);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
  FieldActor_WaitForMoveById(ACTOR_MUSHROOM_COOK);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors110);
  Actor_FaceActor(ACTOR_MUSHROOM_LOVER, ACTOR_FELIX, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_MUSHROOM_LOVER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Oh, my... Is... Is that..."
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 10);
  Actor_ShowEmote(ACTOR_MUSHROOM_COOK, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 3);
  Actor_WalkToAndWait(ACTOR_MUSHROOM_COOK, 378, 109);
  Actor_FaceDirection(ACTOR_MUSHROOM_COOK, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  // "Is that one of his most favorite mushrooms!?"
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 10);
  Actor_FaceEachOther(ACTOR_MUSHROOM_LOVER, ACTOR_MUSHROOM_COOK, 0);
  FieldEvent_Wait(20);
  Actor_FaceActor(ACTOR_MUSHROOM_LOVER, ACTOR_FELIX, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_MUSHROOM_LOVER, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Did you climb all the way up those cliffs just to get that mushroom..."
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_MUSHROOM_COOK, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "That's the nicest thing anyone has ever done for me. I have to thank..."
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 10);
  Actor_FaceActor(ACTOR_MUSHROOM_LOVER, ACTOR_MUSHROOM_COOK, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_MUSHROOM_LOVER, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_ShowEmote(ACTOR_MUSHROOM_LOVER, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "But what could I have that a warrior like you would want?"
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_MUSHROOM_COOK, 2);
  FieldEvent_Wait(20);
  // "That's a tricky one..."
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 10);
  Actor_FaceDirectionAndWait(ACTOR_MUSHROOM_COOK, FACING_SOUTH);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_MUSHROOM_COOK, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_MUSHROOM_COOK, FACING_WEST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_MUSHROOM_COOK, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  FieldActor_Stop(ACTOR_CHAR);
  Actor_SetSpeed(ACTOR_MUSHROOM_LOVER, 0.5);
  Actor_SetSpeed(ACTOR_MUSHROOM_COOK, 0.5);
  Actor_SetSpeed(ACTOR_CHAR, 0.5);
  Actor_SetScript(ACTOR_MUSHROOM_COOK,
                  (s32)gMadraInteriors_WifeWalksToBackRoom);
  Actor_SetScript(ACTOR_FELIX, (s32)gMadraInteriors_LeaderStepsAside);
  FieldEvent_Wait(60);
  Actor_SetScript(ACTOR_MUSHROOM_LOVER,
                  (s32)gMadraInteriors_OldManWalksToBackRoom);
  FieldActor_WaitForScriptEndById(ACTOR_MUSHROOM_LOVER);
  FieldEvent_Wait(120);
  Actor_SetScript(ACTOR_MUSHROOM_COOK, (s32)gMadraInteriors_WifeComesBack);
  FieldActor_WatchAndSetScript(ACTOR_FELIX, ACTOR_MUSHROOM_COOK);
  FieldEvent_Wait(60);
  Actor_SetScript(ACTOR_MUSHROOM_LOVER, (s32)gMadraInteriors_OldManComesBack);
  Actor_SetScript(ACTOR_CHAR, (s32)gMadraInteriors_CharFollows);
  FieldActor_WaitForScriptEndById(ACTOR_MUSHROOM_LOVER);
  FieldActor_WaitForScriptEndById(ACTOR_MUSHROOM_COOK);
  FieldActor_WaitForScriptEndById(ACTOR_CHAR);
  Actor_RunRepeatedMotion(ACTOR_MUSHROOM_COOK, 2);
  FieldEvent_Wait(20);
  // "I don't know if you want a pet, but this is all we can offer. Take..."
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 10);
  Djinn_JoinParty(ACTOR_CHAR, DJINNI_CHAR / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_CHAR));
  gFieldWork->message += 1;
  Actor_SetAnimation(ACTOR_FELIX, ANIM_STAND);
  Actor_FaceDirection(ACTOR_MUSHROOM_LOVER, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MUSHROOM_LOVER, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  // "I can't wait for dinner tonight. Batter-fried mushrooms! Mmm!"
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MUSHROOM_COOK, ANIM_NOD);
  FieldEvent_Wait(20);
  // "This here's enough of a mushroom that we'll be eating it for days!"
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 10);
  Event_End();
}

/// Given the Laughing Fungus, the old man tells his wife it only looks like the
/// right one.
/// "No, dear... They look similar, but these aren't quite right."
void MadraInteriors_GiveLaughingFungus(void) {
  struct Actor *object0;

  Event_SetFlag(FLAG_MADRA_LAUGHING_FUNGUS_OFFERED);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  object0 = Actor_Get(ACTOR_FELIX);
  if (object0->z.fixed > px(95))
    Actor_WalkToAndWait(ACTOR_FELIX, object0->x.fixed / 0x10000, 95);
  Actor_WalkToAndWait(ACTOR_FELIX, 362, 95);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors110);
  Actor_FaceActor(ACTOR_MUSHROOM_LOVER, ACTOR_FELIX, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(ACTOR_MUSHROOM_LOVER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Oh, my... Is... Is that..."
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 5);
  Actor_ShowEmote(ACTOR_MUSHROOM_COOK, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 3);
  Actor_FaceActor(ACTOR_MUSHROOM_COOK, ACTOR_FELIX, 0);
  FieldEvent_Wait(20);
  // "Is that one of his most favorite mushrooms!?"
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 5);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors115);
  Actor_ShowEmote(ACTOR_MUSHROOM_LOVER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirectionAndWait(ACTOR_MUSHROOM_LOVER, FACING_EAST);
  FieldEvent_Wait(10);
  // "No, dear... They look similar, but these aren't quite right."
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 5);
  Actor_FaceEachOther(ACTOR_MUSHROOM_LOVER, ACTOR_MUSHROOM_COOK, 0);
  FieldEvent_Wait(20);
  Actor_FaceActor(ACTOR_MUSHROOM_LOVER, ACTOR_FELIX, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_MUSHROOM_LOVER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  FieldEvent_Wait(20);
  // "Thanks for bringing this by, but it's just not the type of mushroom..."
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 5);
  Actor_FaceActor(ACTOR_MUSHROOM_COOK, ACTOR_FELIX, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_MUSHROOM_COOK, 2);
  FieldEvent_Wait(5);
  // "Please... Won't you please try to find the mushrooms my husband..."
  FieldEvent_ShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 5);
  Event_End();
  return;
}

/// A party member runs into the leader and vanishes, as the party gathers up
/// to leave; the four sites of the welcome-home event, named by us.
static inline void MadraInteriors_WalkIntoLeader(s32 actorId) {
  struct Actor *leader;

  Actor_SetSpeed(actorId, 1.2);
  Actor_SetAnimation(actorId, 2);
  leader = Actor_Get(ACTOR_FELIX);
  if (leader != NULL)
    Actor_SetDestination(actorId, leader->x.part[0].pixel,
                         leader->z.part[0].pixel);
  FieldActor_WaitForMoveById(actorId);
  Actor_SetPosition(actorId, 0, 0);
}

/// On the return with Piers, the elder's household welcomes him home.
/// "Felix! You're back!"
void MadraInteriors_WelcomePiersHome(void) {
  s32 result0;
  s32 result1;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors126);
  Actor_ShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceActor(ACTOR_MAYOR, ACTOR_FELIX, 0);
  // "Felix! You're back!"
  FieldEvent_ShowMessageThenWait(ACTOR_MAYOR, 0, 10);
  Actor_FaceActor(ACTOR_ELDERS_DAUGHTER, ACTOR_FELIX, 0);
  Actor_FaceDirection(ACTOR_ELDERS_GRANDSON, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceActor(ACTOR_ELDERS_GRANDDAUGHTER, ACTOR_FELIX, 0);
  Actor_FaceActor(ACTOR_JEWEL_KEEPER, ACTOR_FELIX, 0);
  Actor_FaceActor(ACTOR_ELDERS_DAUGHTER, ACTOR_FELIX, 0);
  Actor_FaceActor(ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, ACTOR_FELIX, 0);
  FieldEvent_Wait(20);
  Actor_WalkToAndWait(ACTOR_FELIX, 701, 93);
  Actor_WalkBy(ACTOR_FELIX, -32, 0);
  FieldActor_LaunchFromLeader(ACTOR_JENNA, -8, 32, FACING_WEST);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, -16, 0, FACING_SOUTH_SOUTHWEST);
  FieldActor_LaunchFromLeader(ACTOR_SHEBA, -8, 16, FACING_SOUTHWEST);
  FieldActor_LaunchFromLeader(ACTOR_PIERS, -24, 16, FACING_SOUTHWEST);
  FieldEvent_Wait(30);
  Actor_ShowEmote(ACTOR_ELDERS_DAUGHTER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  30);
  // "And you've brought Piers with you!"
  FieldEvent_ShowMessageThenWait(ACTOR_ELDERS_DAUGHTER, 0, 10);
  Actor_ShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_ELDER, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_ELDER, 2);
  FieldEvent_Wait(20);
  // "Oh! Piers!"
  FieldEvent_ShowMessageThenWait(ACTOR_ELDER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "Piers, I must apologize for the trouble we've caused you. I'm sorry..."
  FieldEvent_ShowMessageThenWait(ACTOR_ELDER, 0, 10);
  Actor_FaceEachOther(ACTOR_PIERS, ACTOR_FELIX, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  // "Don't be. Everything worked out for the best."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  FieldActor_Jump(ACTOR_ELDERS_GRANDDAUGHTER, 6, 20);
  FieldEvent_Wait(20);
  // "Piers, you said you were going to go to Gondowan. Did you make it?"
  FieldEvent_ShowMessageThenWait(ACTOR_ELDERS_GRANDDAUGHTER, 0, 10);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  FieldEvent_Wait(20);
  // "Yes. In fact, we have just returned."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "And your jewel..."
  Event_OpenMessage(ACTOR_MAYOR, 0);
  result0 = FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0);
  if (result0 != 0)
    goto answeredNo;
  Actor_ShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_ELDER, 0, 10);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_ELDERS_DAUGHTER, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_ELDERS_DAUGHTER, 0, 10);
  gFieldWork->message += 3;
  goto answered;
answeredNo:
  Actor_ShowEmote(ACTOR_ELDERS_DAUGHTER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  MadraInteriors_CountAnswer();
  gFieldWork->message += 3;
  FieldEvent_ShowMessageThenWait(ACTOR_ELDER, 0, 10);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  FieldActor_StartRepeatedMotion(ACTOR_ELDERS_DAUGHTER, 3);
  Actor_SetAttachedEffect(ACTOR_ELDERS_DAUGHTER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(50);
  FieldEvent_ShowMessageThenWait(ACTOR_ELDERS_DAUGHTER, 0, 10);
answered:
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_KRADEN, 3);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  result1 = Event_GetFlag(0x981);
  if (result1 == 0)
    goto firstGoodbye;
  MadraInteriors_SayGoodbyeAgain();
  goto saidGoodbye;
firstGoodbye:
  MadraInteriors_AskToStayLonger();
saidGoodbye:
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  FieldEvent_Wait(40);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_KRADEN, 3);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(20);
  MadraInteriors_WalkIntoLeader(ACTOR_KRADEN);
  MadraInteriors_WalkIntoLeader(ACTOR_SHEBA);
  MadraInteriors_WalkIntoLeader(ACTOR_PIERS);
  MadraInteriors_WalkIntoLeader(ACTOR_JENNA);
  Event_SetFlag(0x989);
  Event_End();
  return;
}

/// Leaving a second time, the household asks whether the party must go so soon.
/// "Ah, so you'll be leaving again, will you?"
void MadraInteriors_SayGoodbyeAgain(void) {
  FieldEvent_SetMessageId((s32)MsgMadraInteriors127);
  // "Ah, so you'll be leaving again, will you?"
  FieldEvent_ShowMessageThenWait(ACTOR_MAYOR, 0, 10);
  Actor_FaceEachOther(ACTOR_FELIX, ACTOR_KRADEN, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_ELDERS_DAUGHTER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  30);
  // "Are you sure you must leave so soon, Piers?"
  FieldEvent_ShowMessageThenWait(ACTOR_ELDERS_DAUGHTER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  FieldEvent_Wait(20);
  // "I may yet return here again, but I'm afraid I must be going soon."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "You seem to be quite busy."
  FieldEvent_ShowMessageThenWait(ACTOR_ELDER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  FieldEvent_Wait(20);
  // "It is their youth that allows them to be so busy."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "But we'll be back, I promise. Maybe we'll be able to visit at a more..."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_ELDERS_GRANDSON, 3);
  FieldEvent_Wait(20);
  // "I'd like that..."
  FieldEvent_ShowMessageThenWait(ACTOR_ELDERS_GRANDSON, 0, 10);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTH);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ELDERS_GRANDDAUGHTER, 3);
  FieldEvent_Wait(20);
  // "So would I!"
  FieldEvent_ShowMessageThenWait(ACTOR_ELDERS_GRANDDAUGHTER, 0, 10);
  return;
}

/// The first farewell: the household asks the party to stay, and Piers explains
/// why he cannot.
/// "If you've already recovered your jewel, why don't you stay here for a bit
/// longer?"
void MadraInteriors_AskToStayLonger(void) {
  FieldEvent_SetMessageId((s32)MsgMadraInteriors128);
  Actor_SetAnimationAndWait(ACTOR_ELDERS_GRANDSON, 3);
  FieldEvent_Wait(20);
  // "If you've already recovered your jewel, why don't you stay here for..."
  FieldEvent_ShowMessageThenWait(ACTOR_ELDERS_GRANDSON, 0, 10);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTH);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  FieldEvent_Wait(20);
  // "Unfortunately, we can't..."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_FaceActor(ACTOR_SHEBA, ACTOR_JENNA, 0);
  FieldEvent_Wait(20);
  // "Hey, have you seen the girl out front?"
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_NORTH);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_ELDERS_GRANDSON, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceActor(ACTOR_PIERS, ACTOR_ELDERS_GRANDSON, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "We must go...to the island that is my home."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_FaceActor(ACTOR_KRADEN, ACTOR_PIERS, 0);
  // "Piers, are you... Are you sure?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceActor(ACTOR_PIERS, ACTOR_KRADEN, 0);
  FieldEvent_Wait(60);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  FieldEvent_Wait(20);
  // "After what we've been through, I feel I've learned a few things."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_FaceActor(ACTOR_FELIX, ACTOR_PIERS, 0);
  Actor_FaceActor(ACTOR_SHEBA, ACTOR_PIERS, 0);
  Actor_FaceActor(ACTOR_JENNA, ACTOR_PIERS, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Like what!?"
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_FaceActor(ACTOR_PIERS, ACTOR_SHEBA, 0);
  FieldEvent_Wait(20);
  // "Like...the fact that you truly are good people."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceActor(ACTOR_PIERS, ACTOR_FELIX, 0);
  FieldEvent_Wait(20);
  // "And that you have gone to great lengths for no other reason than to..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  FieldEvent_Wait(20);
  // "I have come to realize that I, too, must do something to help where..."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "Piers..."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHEAST);
  // "I have made my decision."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_FaceActor(ACTOR_PIERS, ACTOR_KRADEN, 0);
  FieldEvent_Wait(20);
  // "And the sooner we depart, the better, right, Kraden?"
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  FieldEvent_Wait(20);
  Actor_FaceActor(ACTOR_PIERS, ACTOR_FELIX, 0);
  FieldEvent_Wait(20);
  // "We shall set sail, Felix! To my ship! To the Eastern Sea!"
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Event_SetFlag(0x981);
  Actor_ShowEmote(ACTOR_ELDERS_DAUGHTER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  30);
  // "Do you truly mean to leave so soon, Piers?"
  FieldEvent_ShowMessageThenWait(ACTOR_ELDERS_DAUGHTER, 0, 10);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  FieldEvent_Wait(20);
  // "I may yet again have reason to visit your town, but... yes, I must go."
  FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHWEST, 0);
  Actor_ShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "You seem quite busy."
  FieldEvent_ShowMessageThenWait(ACTOR_ELDER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  FieldEvent_Wait(20);
  // "It is their youth that allows them to be so busy."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  // "But we'll be back, I promise. Maybe we'll be able to visit at a more..."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_ELDERS_GRANDSON, 3);
  FieldEvent_Wait(20);
  // "I'd like that..."
  FieldEvent_ShowMessageThenWait(ACTOR_ELDERS_GRANDSON, 0, 10);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTH);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ELDERS_GRANDDAUGHTER, 3);
  FieldEvent_Wait(20);
  // "So would I!"
  FieldEvent_ShowMessageThenWait(ACTOR_ELDERS_GRANDDAUGHTER, 0, 10);
  return;
}

// clang-format off
/// The old couple's walks into their back room and out again, Char hopping
/// after them, as they fetch the Djinni for the Healing Fungus.
u32 gMadraInteriors_WifeWalksToBackRoom[] = {
    SC_ANIMATION(2),
    SC_MOVE_XZ(px(356), px(72)),
    SC_SYNC,
    SC_MOVE(px(324), 0, px(73)),
    SC_SYNC,
    SC_CMD_03,
    SC_END,
};

/// The leader's script as the old man passes: one step aside.
u32 gMadraInteriors_LeaderStepsAside[] = {
    SC_ANIMATION(2),
    SC_MOVE_XZ(px(339), px(111)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_END,
};

/// The mushroom lover's script off to the back room to fetch Char.
u32 gMadraInteriors_OldManWalksToBackRoom[] = {
    SC_ANIMATION(2),
    SC_MOVE_XZ(px(356), px(72)),
    SC_SYNC,
    SC_MOVE(px(324), 0, px(73)),
    SC_SYNC,
    SC_CMD_03,
    SC_END,
};

/// The mushroom cook's script back to her place.
u32 gMadraInteriors_WifeComesBack[] = {                                                               
      SC_ANIMATION(2),                                                               
      SC_POSITION(px(327), 0, px(71)),                                 
      SC_SET(SW_FACING, FACING_EAST),                                          
      SC_MOVE(px(352), 0, px(72)),                                     
      SC_SYNC,                                                                 
      SC_MOVE_XZ(px(373), px(109)),                                     
      SC_SYNC,                                                                 
      SC_ANIMATION(1),                                                               
      SC_SET(SW_FACING, FACING_NORTH),                                         
      SC_WAIT(10),                                                             
      SC_SET(SW_FACING, FACING_SOUTHWEST),                                     
      SC_WAIT(10),                                                             
      SC_SET(SW_FACING, FACING_WEST),                                          
      SC_WAIT(10),                                                             
      SC_END,                                                                  
  };

/// The mushroom lover's script back from the back room with Char.
u32 gMadraInteriors_OldManComesBack[] = {                                                               
      SC_ANIMATION(2),                                                               
      SC_POSITION(px(327), 0, px(71)),                                 
      SC_SET(SW_FACING, FACING_EAST),                                          
      SC_MOVE(px(352), 0, px(72)),                                     
      SC_SYNC,                                                                 
      SC_MOVE_XZ(px(373), px(97)),                                     
      SC_SYNC,                                                                 
      SC_ANIMATION(1),                                                               
      SC_SET(SW_FACING, FACING_NORTH),                                         
      SC_WAIT(10),                                                             
      SC_SET(SW_FACING, FACING_SOUTHWEST),                                     
      SC_WAIT(10),                                                             
      SC_SET(SW_FACING, FACING_WEST),                                          
      SC_WAIT(10),                                                             
      SC_END,                                                                  
  };

/// Char's script once the old man gives him to the party: he follows the
/// leader.
u32 gMadraInteriors_CharFollows[] = {
    SC_ANIMATION(2),
    SC_POSITION(px(327), px(24), px(71)),
    SC_SET(SW_FACING, FACING_EAST),
    SC_MOVE(px(352), 0, px(72)),
    SC_SYNC,
    SC_MOVE_XZ(px(373), px(97)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(MadraInteriors1),
    SCENE_EXIT_UNLESS(1, Madra, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Madra, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, MadraInteriors1, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, MadraInteriors1, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, Madra, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, MadraInteriors1, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, MadraInteriors1, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, Madra, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, Madra, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, Madra, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, MadraInteriors1, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, MadraInteriors1, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, MadraInteriors1, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(14, MadraInteriors1, 13, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(15, Madra, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(20, Madra, 9, EXIT_FLAG_NONE),
    SCENE_EXITS(MadraInteriors2),
    SCENE_EXIT_UNLESS(1, Madra, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Madra, 10, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/// Madra's houses, shops, inn and sanctum until the party returns with Piers
/// (enum MadraInteriorsActorId, then MadraInteriorsTravelersActorId).
struct ActorPlacement gMadraInteriors_Placements[] = {
    ACTOR_PLACEMENT(73, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 184, 0, 120, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(74, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 112, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(75, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 109, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(76, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 96, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(224, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 296, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(225, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 384, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(74, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 112, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(200, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 640, 0, 152, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(76, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 615, 0, 311, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(191, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 364, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(186, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 348, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(202, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0, 556, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(74, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 624, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(76, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 648, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(199, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 604, 0, 548, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(209, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 432, 0, 616, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(72, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 518, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(245, 98, MOVEMENT_TYPE_WANDER, 368, 0, 320, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(238, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 676, 0, 762, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(238, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 730, 0, 712, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(239, FLAG_BRIGGS_CAUGHT, MOVEMENT_TYPE_STAND, 847, 0, 711, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(239, FLAG_BRIGGS_CAUGHT, MOVEMENT_TYPE_STAND, 906, 0, 760, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(-1, 0, 0, 0, 0, 0, 0, TALK_FACING_PARTY, 0),
};

/// Madra's houses once the party returns with Piers (enum
/// MadraInteriorsActorId, then MadraInteriorsReturnActorId).
struct ActorPlacement gMadraInteriors_PlacementsOnReturn[] = {
    ACTOR_PLACEMENT(73, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 184, 0, 120, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(74, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 112, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(75, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 109, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(76, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 96, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(224, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 296, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(225, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 384, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(74, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 88, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(200, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 640, 0, 152, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(76, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 620, 0, 152, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(191, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 152, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(186, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(202, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0, 552, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(74, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 624, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(76, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 648, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(199, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 604, 0, 548, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(209, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 432, 0, 616, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(72, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 518, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(245, 0x62, MOVEMENT_TYPE_WANDER, 368, 0, 320, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(19, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 614, 0, 111, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(20, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 636, 0, 111, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1288, 0, 8, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1288, 0, 8, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1288, 0, 8, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(7, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1288, 0, 8, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(5, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(6, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(238, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 676, 0, 762, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(238, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 730, 0, 712, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

/// The jail (enum MadraInteriorsJailActorId).
struct ActorPlacement gMadraInteriors_JailPlacements[] = {
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 528, 0, 840, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(192, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 522, 0, 840, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(187, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 840, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(7, FLAG_BRIGGS_CAUGHT, MOVEMENT_TYPE_STAND, 504, 0, 808, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 362, 0, 847, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(187, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MOVEMENT_TYPE_STAND, 394, 0, 863, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(187, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MOVEMENT_TYPE_STAND, 396, 0, 848, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The left cavern home (enum MadraInteriorsCavernHomeActorId).
struct ActorPlacement gMadraInteriors_SecondRoomsPlacements[] = {
    ACTOR_PLACEMENT(71, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 192, 0, 128, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(72, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 192, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(198, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 356, 0, 109, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(186, IF_FLAG_SET(FLAG_BRIGGS_CAUGHT), MOVEMENT_TYPE_STAND, 357, 0, 131, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The houses' talk and Mind Read before the Kibombo raid.
struct Trigger gMadraInteriors_TriggersBeforeRaid[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_TALK, ACTOR_OLD_MAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors05)},
    {TRIGGER_TALK, ACTOR_WOMAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, EVENT(MadraInteriors_AskWillWeBeSafe)},
    {TRIGGER_TALK, ACTOR_MUSHROOM_LOVER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors09)},
    {TALK_TRIGGER_FACING(FACING_NORTH), 11, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors10)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_STRANGE_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors13)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_GROUNDED_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors14)},
    {TRIGGER_TALK, ACTOR_JEWEL_KEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors17)},
    {TRIGGER_TALK, ACTOR_ELDERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors19)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors20)},
    {TRIGGER_TALK, ACTOR_ELDERS_GRANDSON, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors21)},
    {TRIGGER_TALK, ACTOR_ELDERS_GRANDDAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors22)},
    {TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(MadraInteriors_TalkToHealer)},
    {TRIGGER_TALK, ACTOR_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors34)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors35)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(MadraInteriors_TalkToInnkeeper)},
    {TRIGGER_TALK, ACTOR_INN_HELPER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors40)},
    {TRIGGER_TALK, ACTOR_GIRL_STAYING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors41)},
    {TRIGGER_TALK, ACTOR_BLACK_CRYSTAL, CONDITION_ALWAYS, EVENT(MadraInteriors_GuardElderGem)},
    {TRIGGER_TALK, ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors42)},
    {TRIGGER_TALK, ACTOR_TRAVELERS_PARTNER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors43)},
    {TRIGGER_TALK, ACTOR_MORGAN, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors165)},
    {TRIGGER_TALK, ACTOR_DEKKA, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors166)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors07)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors08)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSHROOM_LOVER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors11)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSHROOM_COOK, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors12)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_STRANGE_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors15)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_GROUNDED_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors16)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JEWEL_KEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors23)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors24)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors25)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_GRANDSON, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors26)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_GRANDDAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors27)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors36)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors37)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors38)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors44)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_HELPER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors45)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_STAYING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors46)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors47)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELERS_PARTNER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors48)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MORGAN, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors167)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DEKKA, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors168)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors169))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors170))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors171))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors172))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors173))},
    {TRIGGER_TABLE_END},
};

/// The jail's talk and Mind Read, by the story flags.
struct Trigger gMadraInteriors_JailTriggers[] = {
    {TRIGGER_EXIT, 20, CONDITION_ALWAYS, 20},
    {TRIGGER_TALK, ACTOR_STERN_INTERROGATOR, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgMadraInteriors143)},
    {TRIGGER_TALK, ACTOR_ACCUSING_INTERROGATOR, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgMadraInteriors144)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_WITNESS_ON_ISAACS_PARTY, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgWhenAndHis)},
    {TALK_TRIGGER_FACING(FACING_NORTHWEST), ACTOR_WITNESS_ON_ISAACS_PARTY, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgWhenAndHis)},
    {TALK_TRIGGER_FACING(FACING_WEST), ACTOR_WITNESS_ON_ISAACS_PARTY, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgWhenAndHis)},
    {TRIGGER_TALK, ACTOR_ILL_MAN, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), EVENT(MadraInteriors_TalkToIllMan)},
    {TRIGGER_TALK, ACTOR_STERN_INTERROGATOR, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors75)},
    {TRIGGER_TALK, ACTOR_ACCUSING_INTERROGATOR, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors76)},
    {TRIGGER_TALK, ACTOR_PIERS, CONDITION_ALWAYS, EVENT(MadraInteriors_TalkToPiers)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STERN_INTERROGATOR, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgMadraInteriors145)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ACCUSING_INTERROGATOR, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgMadraInteriors146)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WITNESS_ON_ISAACS_PARTY, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgSoThisFellow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ILL_MAN, IF_FLAG_SET(FLAG_MADRA_ILL_MAN_GROANED), MSGNO(MsgIKnewI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14 | TRIGGER_SUBJECT_OWN_EFFECT, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), EVENT(MadraInteriors_TalkToIllMan)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STERN_INTERROGATOR, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors77)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ACCUSING_INTERROGATOR, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors78)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PIERS, CONDITION_ALWAYS, EVENT(MadraInteriors_MindReadPiers)},
    {TRIGGER_TABLE_END},
};

/// The left cavern home's talk and Mind Read, by the story flags.
struct Trigger gMadraInteriors_SecondRoomsTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TALK, ACTOR_MAN_ON_ALHAFRA, 0x8ab, MSGNO(MsgMadraInteriors01)},
    {TRIGGER_TALK, ACTOR_MAN_ON_ALHAFRA, FLAG_GABOMBA_MESSAGE_CARRIED, MSGNO(MsgMadraInteriors49)},
    {TRIGGER_TALK, ACTOR_MAN_ON_ALHAFRA, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors98)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_ALHAFRA, 0x8ab, MSGNO(MsgMadraInteriors02)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_ALHAFRA, FLAG_GABOMBA_MESSAGE_CARRIED, MSGNO(MsgMadraInteriors50)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_ALHAFRA, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors99)},
    {TRIGGER_TALK, ACTOR_SISTER_IN_BED, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors31)},
    {TRIGGER_TALK, ACTOR_HER_SISTER, FLAG_GABOMBA_MESSAGE_CARRIED, MSGNO(MsgMadraInteriors79)},
    {TRIGGER_TALK, ACTOR_HER_SISTER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors147)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_ALHAFRA, 0x8ab, MSGNO(MsgMadraInteriors03)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_ALHAFRA, FLAG_GABOMBA_MESSAGE_CARRIED, MSGNO(MsgMadraInteriors51)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_ALHAFRA, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors100)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_ALHAFRA, 0x8ab, MSGNO(MsgMadraInteriors04)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_ALHAFRA, FLAG_GABOMBA_MESSAGE_CARRIED, MSGNO(MsgMadraInteriors52)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_ALHAFRA, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors101)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SISTER_IN_BED, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors32)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HER_SISTER, FLAG_GABOMBA_MESSAGE_CARRIED, MSGNO(MsgMadraInteriors80)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HER_SISTER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors148)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors174))},
    {TRIGGER_TABLE_END},
};

/// The houses' talk and Mind Read once the Kibombo have raided Madra.
struct Trigger gMadraInteriors_TriggersAfterRaid[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_TALK, ACTOR_OLD_MAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors53)},
    {TRIGGER_TALK, ACTOR_WOMAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors54)},
    {TRIGGER_TALK, ACTOR_MUSHROOM_LOVER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors57)},
    {TRIGGER_TALK, ACTOR_MUSHROOM_COOK, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors58)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_STRANGE_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors61)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_GROUNDED_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors62)},
    {TRIGGER_TALK, ACTOR_JEWEL_KEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors65)},
    {TRIGGER_TALK, ACTOR_ELDERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors66)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors67)},
    {TRIGGER_TALK, ACTOR_ELDERS_GRANDSON, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors68)},
    {TRIGGER_TALK, ACTOR_ELDERS_GRANDDAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors69)},
    {TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(MadraInteriors_TalkToHealer)},
    {TRIGGER_TALK, ACTOR_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors82)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors83)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(MadraInteriors_TalkToInnkeeper)},
    {TRIGGER_TALK, ACTOR_INN_HELPER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors88)},
    {TRIGGER_TALK, ACTOR_GIRL_STAYING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors89)},
    {TRIGGER_TALK, ACTOR_BLACK_CRYSTAL, CONDITION_ALWAYS, EVENT(MadraInteriors_GuardElderGem)},
    {TRIGGER_TALK, ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors90)},
    {TRIGGER_TALK, ACTOR_TRAVELERS_PARTNER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors91)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors55)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors56)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSHROOM_LOVER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors59)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSHROOM_COOK, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors60)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_STRANGE_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors63)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_GROUNDED_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors64)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JEWEL_KEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors70)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors71)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors72)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_GRANDSON, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors73)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_GRANDDAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors74)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors84)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors85)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors86)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors92)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_HELPER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors93)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_STAYING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors94)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors95)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELERS_PARTNER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors96)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors169))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors170))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors171))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors172))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors173))},
    {TRIGGER_TABLE_END},
};

/// The houses' talk and Mind Read once the party returns with Piers, the
/// welcome, and the mushroom trade.
struct Trigger gMadraInteriors_TriggersOnReturn[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_STEP, 30, 0x989, EVENT(MadraInteriors_WelcomePiersHome)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors102)},
    {TRIGGER_TALK, ACTOR_WOMAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors103)},
    {TRIGGER_TALK, ACTOR_MUSHROOM_LOVER, CONDITION_ALWAYS, EVENT(MadraInteriors_TalkToMushroomLover)},
    {TRIGGER_TALK, ACTOR_MUSHROOM_COOK, CONDITION_ALWAYS, EVENT(MadraInteriors_TalkToMushroomCook)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_STRANGE_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors122)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_GROUNDED_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors123)},
    {TRIGGER_TALK, ACTOR_ELDER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors129)},
    {TRIGGER_TALK, ACTOR_MAYOR, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors130)},
    {TRIGGER_TALK, ACTOR_JEWEL_KEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors131)},
    {TRIGGER_TALK, ACTOR_ELDERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors132)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors133)},
    {TRIGGER_TALK, ACTOR_ELDERS_GRANDSON, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors134)},
    {TRIGGER_TALK, ACTOR_ELDERS_GRANDDAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors135)},
    {TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(MadraInteriors_TalkToHealer)},
    {TRIGGER_TALK, ACTOR_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors150)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors151)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(MadraInteriors_TalkToInnkeeper)},
    {TRIGGER_TALK, ACTOR_INN_HELPER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors156)},
    {TRIGGER_TALK, ACTOR_GIRL_STAYING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors157)},
    {TRIGGER_TALK, ACTOR_BLACK_CRYSTAL, CONDITION_ALWAYS, EVENT(MadraInteriors_GuardElderGem)},
    {TRIGGER_TALK, ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID_ON_RETURN, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors158)},
    {TRIGGER_TALK, ACTOR_TRAVELERS_PARTNER_ON_RETURN, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors159)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors104)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors105)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSHROOM_LOVER, CONDITION_ALWAYS, EVENT(MadraInteriors_MindReadMushroomLover)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSHROOM_COOK, CONDITION_ALWAYS, EVENT(MadraInteriors_MindReadMushroomCook)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_STRANGE_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors124)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_GROUNDED_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors125)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors136)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYOR, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors137)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JEWEL_KEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors138)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors139)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors140)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_GRANDSON, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors141)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_GRANDDAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors142)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors152)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors153)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors154)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors160)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_HELPER, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors161)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_STAYING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors162)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID_ON_RETURN, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors163)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELERS_PARTNER_ON_RETURN, CONDITION_ALWAYS, MSGNO(MsgMadraInteriors164)},
    {USE_ITEM_ON_ACTOR(ITEM_HEALING_FUNGUS), 10, 0x98a, EVENT(MadraInteriors_GiveHealingFungus)},
    {USE_ITEM_ON_ACTOR(ITEM_LAUGHING_FUNGUS), 10, 0x98a, EVENT(MadraInteriors_GiveLaughingFungus)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors169))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors170))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors171))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors172))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraInteriors173))},
    {TRIGGER_TABLE_END},
};

/// The interrogators walk out of the jail once Piers has been questioned.
u32 gMadraInteriors_InterrogatorsLeave[] = {                                                               
      SC_ANIMATION(2), SC_MOVE(px(408), 0, px(872)),                           
      SC_SYNC,   SC_MOVE(px(408), 0, px(904)),                           
      SC_SYNC,   SC_MOVE(px(408), 0, px(920)),                           
      SC_SYNC,   SC_ANIMATION(1),                                                    
      SC_END,                                                                  
  };

/// The interrogator topples over, a quarter turn in eight frames.
s32 gMadraInteriors_InterrogatorTopples[] = {
    SC_CALL(MadraInteriors_TiltSprite),
    SC_WAIT(1),
    SC_CALL(MadraInteriors_TiltSprite),
    SC_WAIT(1),
    SC_CALL(MadraInteriors_TiltSprite),
    SC_WAIT(1),
    SC_CALL(MadraInteriors_TiltSprite),
    SC_WAIT(1),
    SC_CALL(MadraInteriors_TiltSprite),
    SC_WAIT(1),
    SC_CALL(MadraInteriors_TiltSprite),
    SC_WAIT(1),
    SC_CALL(MadraInteriors_TiltSprite),
    SC_WAIT(1),
    SC_CALL(MadraInteriors_TiltSprite),
    SC_WAIT(1),
    SC_END,
};

/// Piers' Psynergy lifts the interrogator and holds him bobbing in the air.
s32 gMadraInteriors_InterrogatorLifted[] = {
    SC_SET(SW_MOTION, 0),
    SC_WAIT(100),
    SC_WAIT(36),
    SC_ADD(SW_Y, px(3)),
    SC_WAIT(9),
    SC_ADD(SW_Y, px(5)),
    SC_ADD(SW_X, px(1)),
    SC_WAIT(9),
    SC_ADD(SW_Y, px(4)),
    SC_WAIT(9),
    SC_SET(SW_FACING, FACING_WEST),
    SC_ADD(SW_X, px(1)),
    SC_ADD(SW_Y, px(5)),
    SC_WAIT(9),
    SC_ADD(SW_Y, px(4)),
    SC_WAIT(9),
    SC_ADD(SW_Y, px(4)),
    SC_ADD(SW_X, px(1)),
    SC_WAIT(9),
    SC_ADD(SW_Y, px(4)),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ADD(SW_X, px(1)),
    SC_LABEL(1),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_ADD(SW_Y, px(-1)),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_Y, px(1)),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_ADD(SW_Y, px(-1)),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_Y, px(1)),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(4),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};
// clang-format on

/// @file
/// Izumo's houses, inn and Sanctum, with Susa's house when entered by entrance
/// 7, and other tables once the Great Serpent is slain (FLAG_SERPENT_SLAIN).

#include "shared/tables.h"
#include "types.h"
#include "item_ids.h"
#include "flags.h"
#include "djinn_ids.h"
#include "field/actor.h"
#include "field/event_inline.h"
#include "battle/battle.h"
#include "field/field.h"
#include "field/field_sequence.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/scene_tables.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "field/field_sequence_event.h"
#include "field/event_commands.h"
#include "scene/scene.h"
#include "field/actor_script.h"
#include "text/msg_ids.h"

/// Story flags: Uzume has told the party where Susa went; her prayer for him
/// has been read with Mind Read; Kushinada has asked Susa not to come for her;
/// and Uzume has caught the party reading her mind ("Trying again, Felix?").
#define FLAG_UZUME_SPOKE_OF_SUSA 0x922
#define FLAG_UZUMES_PRAYER_READ 0x923
#define FLAG_KUSHINADA_ASKED_SUSA_NOT_TO_COME 0x925
#define FLAG_UZUME_MIND_READ 0x92b

extern struct ActorPlacement gIzumoRooms_ActorPlacements[];
extern struct ActorPlacement gIzumoRooms_SusasHouseActorPlacements[];
extern struct ActorPlacement gIzumoRooms_ActorPlacementsOnceTheSerpentIsSlain[];
extern struct Trigger gIzumoRooms_Triggers[];
extern struct Trigger gIzumoRooms_SusasHouseTriggers[];
extern struct Trigger gIzumoRooms_TriggersOnceTheSerpentIsSlain[];

void Inn_CheckIn(s32 slot, s32 actor);

void IzumoRooms_RecognizeTheHeroes(s32 mode);

void IzumoRooms_ShowMove(void);
extern s32 gIzumoRooms_FirstDancerScript[];
extern s32 gIzumoRooms_SecondDancerScript[];
extern s32 gIzumoRooms_HeavingScript[];

/// Whether the other of the two women dancing in Susa's house is clear of the
/// spot by the doorway this one steps to; their scripts wait on it.
#define DEFINE_IZUMO_ROOMS_IS_CLEAR(name, n1, n2)                              \
  s32 name(void) {                                                             \
    struct Actor *actor = Actor_GetById(n1);                                   \
                                                                               \
    if ((u32)(actor->x.fixed - n2) <= toFIXED(8)) {                            \
      s32 z = actor->z.fixed;                                                  \
                                                                               \
      if (z >= toFIXED(620)) {                                                 \
        if (z <= toFIXED(628))                                                 \
          return 0;                                                            \
      }                                                                        \
    }                                                                          \
    return 1;                                                                  \
  }

DEFINE_IZUMO_ROOMS_IS_CLEAR(IzumoRooms_IsFirstDancerClear, 10, toFIXED(148))

DEFINE_IZUMO_ROOMS_IS_CLEAR(IzumoRooms_IsSecondDancerClear, 9, toFIXED(132))

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

void Shop_Run(s32 slot, s32 actor);
void Sanctum_Run(s32 actor);

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (GameFlag_Get(FLAG_SERPENT_SLAIN))
    return gIzumoRooms_ActorPlacementsOnceTheSerpentIsSlain;
  if (gMainRoutineWork.entrance == IZUMO_ROOMS_FROM_IZUMO_NORTH)
    return gIzumoRooms_SusasHouseActorPlacements;
  return gIzumoRooms_ActorPlacements;
}

const struct Trigger *Scene_GetTriggers(void) {
  if (GameFlag_Get(FLAG_SERPENT_SLAIN))
    return gIzumoRooms_TriggersOnceTheSerpentIsSlain;
  if (gMainRoutineWork.entrance == IZUMO_ROOMS_FROM_IZUMO_NORTH)
    return gIzumoRooms_SusasHouseTriggers;
  return gIzumoRooms_Triggers;
}

/// Before the Serpent is slain: Kushinada, whose turn it is to be offered to
/// the Serpent, and the Sanctum's priest ("Why would our guardian spirit demand
/// a sacrifice?"), who keeps his trade once it is slain.
enum { ACTOR_KUSHINADA = 17, ACTOR_PRIEST = 22, ACTOR_PRIEST_ONCE_SLAIN = 12 };

/// Susa's house before the Serpent is slain: Lady Uzume, Susa's
/// sister ("If the serpent defeats Susa, I'll...be alone."), with two women
/// dancing ("There's no point dancing if Okuni's not here...") and two old men
/// by the drums.
enum {
  ACTOR_SUSAS_HOUSE_UZUME = 8,
  ACTOR_SUSAS_HOUSE_FIRST_DANCER,
  ACTOR_SUSAS_HOUSE_SECOND_DANCER,
  ACTOR_SUSAS_HOUSE_WEST_VILLAGER,
  ACTOR_SUSAS_HOUSE_EAST_VILLAGER
};

/// Once the Serpent is slain, Susa's house holds Lady Uzume ("This is my home,
/// and I am having a private conversation."), the same four, Kushinada,
/// Susa, the man who saw the heroes come down Mt. Mikage ("Yes... After the
/// serpent's roar, these warriors appeared.") and Kraden.
enum {
  ACTOR_UZUME = 15,
  ACTOR_FIRST_DANCER,
  ACTOR_SECOND_DANCER,
  ACTOR_WEST_VILLAGER,
  ACTOR_EAST_VILLAGER,
  ACTOR_SAVED_KUSHINADA,
  ACTOR_SUSA,
  ACTOR_WITNESS,
  ACTOR_KRADEN,
  ACTOR_COAL ///< the Mars Djinni Uzume gives the party
};

/// The priest: across the counter, the Sanctum's healing; from elsewhere, why
/// the guardian spirit would want a sacrifice, or once the Serpent is slain how
/// the island flourishes.
void IzumoRooms_TalkToPriest(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Sanctum_Run(actor);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Get(FLAG_SERPENT_SLAIN)) {
      Event_SetMessageId((s32)MsgNowThatPeace);
      // "Now that peace has been restored, our island is flourishing once..."
      Event_ShowMessageAndWait(ACTOR_PRIEST_ONCE_SLAIN, 0);
    } else {
      Event_SetMessageId((s32)MsgWhyWouldOur);
      // "Why would our guardian spirit demand a sacrifice? It's..."
      Event_ShowMessageAndWait(ACTOR_PRIEST, 0);
    }
    Event_End();
  }
}

/// Izumo's shops, as Shop_Run numbers them.
enum { SHOP_IZUMO = 24 };

/// Izumo's inn, as Inn_CheckIn numbers the inns.
enum { IZUMO_INN = 9 };

/// The innkeeper: across the counter, Izumo's inn; from elsewhere, the legend
/// of the Serpent, or once it is slain news of Briggs.
void IzumoRooms_TalkToInnkeeper(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Inn_CheckIn(IZUMO_INN, actor);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Get(FLAG_SERPENT_SLAIN))
      Event_SetMessageId((s32)MsgBriggsFromChampa);
    else
      Event_SetMessageId((s32)MsgOfCourseI);
    // "Of course, I knew the legend of the great serpent... Everyone does!..."
    Event_ShowMessageAndWait(actor, 0);
    Event_End();
  }
}

/// The merchant: across the counter, Izumo's shop; from elsewhere, the powers
/// of Izumo's leaders.
void IzumoRooms_TalkToMerchant(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Shop_Run(SHOP_IZUMO, actor);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Get(FLAG_SERPENT_SLAIN))
      Event_SetMessageId((s32)MsgYouMustSee);
    else
      Event_SetMessageId((s32)MsgIzumoSLeaders);
    // "Izumo's leaders have always possessed certain special powers...."
    Event_ShowMessageAndWait(actor, 0);
    Event_End();
  }
}

/// Lady Uzume, the first time, tells where Susa has gone and why; then she
/// fears being left alone.
void IzumoRooms_AskAfterSusa(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (!Event_GetFlag(FLAG_UZUME_SPOKE_OF_SUSA)) {
    Event_SetMessageId((s32)MsgSusaSNot);
    // "Susa's not here. I believe he went to Mt. Mikage."
    Event_ShowMessageAndWait(ACTOR_SUSAS_HOUSE_UZUME, 0);
    Event_Wait(30);
    Actor_ShowEmoteById(ACTOR_SUSAS_HOUSE_UZUME,
                        EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
    Event_Wait(60);
    // "He believes he can save Kushinada, and so he has gone to fight the..."
    Event_ShowMessageAndWait(ACTOR_SUSAS_HOUSE_UZUME, 0);
    Event_Wait(30);
    Actor_ShowEmoteById(ACTOR_SUSAS_HOUSE_UZUME,
                        EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    Event_Wait(60);
    // "He truly believes he can defeat the Great Serpent of Mikage."
    Event_ShowMessageAndWait(ACTOR_SUSAS_HOUSE_UZUME, 0);
    Event_Wait(30);
    Actor_StartRepeatedMotion(ACTOR_SUSAS_HOUSE_UZUME, 1);
    Event_Wait(60);
    // "It's all for naught, though, as a hero will certainly appear and..."
    Event_ShowMessageAndWait(ACTOR_SUSAS_HOUSE_UZUME, 0);
    Event_Wait(30);
    GameFlag_Set(FLAG_UZUME_SPOKE_OF_SUSA);
  }
  Actor_SetAnimationById(ACTOR_SUSAS_HOUSE_UZUME, ANIM_SHAKE_HEAD);
  Event_Wait(60);
  Event_SetMessageId((s32)MsgIfTheSerpent);
  // "If the serpent defeats Susa, I'll...be alone."
  Event_ShowMessageAndWait(ACTOR_SUSAS_HOUSE_UZUME, 0);
  Event_End();
}

/// Mind Read on Lady Uzume before the Serpent is slain: her prayer for Susa.
void IzumoRooms_ReadPrayerForSusa(void) {
  GameFlag_Set(FLAG_UZUMES_PRAYER_READ);
  Event_SetMessageId((s32)MsgIPrayThat);
  // "I pray that our guardian's anger is not aroused and that Susa will..."
  Event_ShowMessageAndWait(ACTOR_SUSAS_HOUSE_UZUME, 0);
}

/// Lady Uzume after the festival begins: she invites the party to enjoy it, and
/// with the Dancing Idol in hand the field menu opens to give it; once she has
/// it, she means to climb Mt. Mikage herself.
void IzumoRooms_TalkToUzume(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActorById(ACTOR_UZUME, gMainRoutineWork.leaderActorId, 0);
  if (GameFlag_Get(FLAG_UZUME_HAS_THE_DOLL)) {
    Event_SetMessageId((s32)MsgWithThisDancing);
    // "With this dancing doll, I can explore Mt. Mikage on my own. Take..."
    Event_ShowMessageAndWait(ACTOR_UZUME, 0);
  } else {
    Event_SetMessageId((s32)MsgIfYouHave);
    // "If you have time, won't you enjoy a little of our celebratory festival?"
    Event_ShowMessageAndWait(ACTOR_UZUME, 0);
    if (PartyInventory_FindMember(ITEM_DANCING_IDOL) != -1)
      work->fieldMenu = 1;
  }
  Actor_FaceDirection(ACTOR_UZUME, FACING_SOUTH, 0);
  Event_End();
}

/// One of the dancers in Susa's house asks whether the party went into Mt.
/// Mikage too, and answers by the reply.
void IzumoRooms_AskAboutMtMikage(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgTheyVeBeen;
  Event_SetMessageId(msg);
  // "They've been saying that you went into Mt. Mikage, too... Is that true?"
  FieldMessage_ShowAtActor(ACTOR_FIRST_DANCER, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(ACTOR_FIRST_DANCER, 0);
  Event_End();
}

/// Mind Read on Lady Uzume once the Serpent is slain: she catches the party,
/// scolding it if her prayer was read before; the second time she sends it
/// away.
void IzumoRooms_ReadUzumesMind(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActorById(ACTOR_UZUME, gMainRoutineWork.leaderActorId, 0);
  if (Event_GetFlag(FLAG_UZUME_MIND_READ)) {
    Event_SetMessageId((s32)MsgTryingAgainWhat);
    // "Trying again, Felix? What a pest. You can see my mind, but you are..."
    Event_ShowMessageAndWait(ACTOR_UZUME, 0);
  } else {
    Event_SetMessageId((s32)MsgAndWhatWere);
    // "And what were you planning to do, once you'd read my mind, hm?"
    Event_ShowMessageAndWait(ACTOR_UZUME, 0);
    if (GameFlag_Get(FLAG_UZUMES_PRAYER_READ)) {
      Actor_ShowEmoteById(gMainRoutineWork.leaderActorId,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
      Actor_ShowEmoteById(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
      Event_ShowMessageAndWait(ACTOR_UZUME, 0);
      Actor_ShowEmoteById(gMainRoutineWork.leaderActorId,
                          EMOTE_IN_FRONT | EMOTE_WINCE, 30);
      Actor_SetAnimationById(ACTOR_UZUME, ANIM_SHAKE_HEAD);
      Event_Wait(30);
      Event_ShowMessageAndWait(ACTOR_UZUME, 0);
    }
    GameFlag_Set(FLAG_UZUME_MIND_READ);
  }
  Actor_FaceDirection(ACTOR_UZUME, FACING_SOUTH, 0);
  Event_End();
}

/// Kushinada, the first time, asks Susa not to come for her and explains why
/// she will be offered to the Serpent; then she asks him not to save her, and
/// the leader steps back.
s32 IzumoRooms_KushinadaAsksSusaNotToCome(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (!Event_GetFlag(FLAG_KUSHINADA_ASKED_SUSA_NOT_TO_COME)) {
    Event_SetMessageId((s32)MsgSusaDonT);
    // "Susa, don't come for me..."
    Event_ShowMessageAndWait(ACTOR_KUSHINADA, 0);
    Event_Wait(60);
    // "If I see you, I'll surely lose the courage to sacrifice myself for..."
    Event_ShowMessageAndWait(ACTOR_KUSHINADA, 0);
    Event_Wait(60);
    Event_Wait(15);
    // "If I run, Lady Uzume and all of Izumo will suffer greatly......"
    Event_ShowMessageAndWait(ACTOR_KUSHINADA, 0);
    Event_Wait(60);
    Event_Wait(15);
    // "Lady Uzume has foretold the coming of a hero, and I believe her..."
    Event_ShowMessageAndWait(ACTOR_KUSHINADA, 0);
    Event_Wait(60);
    Actor_StartRepeatedMotion(ACTOR_KUSHINADA, 1);
    Event_Wait(30);
    // "And so I shall remain here, until the coming of the next full..."
    Event_ShowMessageAndWait(ACTOR_KUSHINADA, 0);
    Event_Wait(60);
    Event_SetFlag(FLAG_KUSHINADA_ASKED_SUSA_NOT_TO_COME);
  } else {
    Event_SetMessageId((s32)MsgSusaPleaseDon);
    // "Susa, please... Don't try to save me."
    Event_ShowMessageAndWait(ACTOR_KUSHINADA, 0);
  }
  Actor_GetById(gMainRoutineWork.leaderActorId)->steeringFlags &=
      ~ACTOR_FACES_MOTION;
  Actor_WalkByById(gMainRoutineWork.leaderActorId, 0, 16);
  Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_GetById(gMainRoutineWork.leaderActorId)->steeringFlags |=
      ACTOR_FACES_MOTION;
  Event_End();
  return 0;
}

/// Entering Susa's house from the east, Izumo recognizes the heroes.
void IzumoRooms_HeroesComeFromTheEast(void) {
  IzumoRooms_RecognizeTheHeroes(0);
}

/// Entering Susa's house from the west, Izumo recognizes the heroes.
void IzumoRooms_HeroesComeFromTheWest(void) {
  IzumoRooms_RecognizeTheHeroes(1);
}

/// Susa's answers to Sheba's "Don't forget, Susa dealt the finishing blow,
/// didn't he?", from MsgYouReNotImplying on.
enum FinishingBlowMessage {
  /// "Finishing blow? All I did was stab it with the Cloud Brand."
  SUSA_ALL_I_DID_WAS_STAB_IT = 10,
  /// "That's right. Piercing it with the Cloud Brand was hardly a finishing
  /// blow."
  SUSA_HARDLY_A_FINISHING_BLOW = 11
};

/// Izumo hears that warriors appeared from within Mt. Mikage and recognizes
/// the party as the heroes, through three answers; the party gathers again
/// and FLAG_IZUMO_RECOGNIZED_THE_HEROES is set. Mode 0 and mode 1
/// (IzumoRooms_HeroesComeFromTheEast and
/// IzumoRooms_HeroesComeFromTheWest) start the party at different spots.
void IzumoRooms_RecognizeTheHeroes(s32 mode) {
  struct Actor *person, *leader;
  s32 party, owner, answer, message;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(143, CAMERA_KEEP, 560, 1);
  party = (s32)&gMainRoutineWork;
  if (mode == 0) {
    Actor_WalkToAndWait(
        *(s32 *)(party + offsetof(struct MainRoutineWork, leaderActorId)), 184,
        568);
  } else {
    Actor_WalkToAndWait(
        *(s32 *)(party + offsetof(struct MainRoutineWork, leaderActorId)), 104,
        568);
  }
  FieldEvent_Wait(15);
  FieldEvent_SetMessageId((s32)MsgWarriorsAppearedFrom);
  // "Warriors appeared from within Mt. Mikage?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
  Actor_SetAnimation(ACTOR_WITNESS, ANIM_NOD);
  // "Yes... After the serpent's roar, these warriors appeared."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_WITNESS, 0);
  Actor_ShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "So, it wasn't Susa who defeated the serpent, but those heroes..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimation(ACTOR_WITNESS, ANIM_NOD);
  FieldEvent_Wait(15);
  // "It might have been..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_WITNESS, 0);
  Actor_ShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  FieldActor_StartRepeatedMotion(ACTOR_WITNESS, 3);
  Actor_SetAttachedEffect(ACTOR_WITNESS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(30);
  // "Lady Kushinada, please forgive me..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_WITNESS, 0);
  Actor_FaceDirection(ACTOR_SAVED_KUSHINADA, FACING_EAST, 30);
  Actor_SetAnimation(ACTOR_SAVED_KUSHINADA, ANIM_SHAKE_HEAD);
  // "I don't care who defeated the serpent... I'm more worried about what..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_UZUME, FACING_WEST, 30);
  Actor_SetAnimation(ACTOR_UZUME, ANIM_NOD);
  FieldEvent_Wait(30);
  // "So, Susa never came down from Mt. Mikage?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
  Actor_SetAnimation(ACTOR_WITNESS, ANIM_NOD);
  FieldEvent_Wait(15);
  // "He returned to the village after the heroes appeared, and he knew..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_WITNESS, 0);
  FieldActor_StartRepeatedMotion(ACTOR_SAVED_KUSHINADA, 2);
  FieldEvent_Wait(15);
  // "Oh, Susa..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
  Actor_FaceActor(ACTOR_UZUME, gMainRoutineWork.leaderActorId, 0);
  FieldEvent_Wait(30);
  Actor_SetDestinationOffset(ACTOR_UZUME, 0, 16);
  FieldActor_WaitForMoveById(ACTOR_UZUME);
  FieldEvent_Wait(15);
  Actor_ShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  if (mode == 0) {
    Actor_WalkToAndWait(ACTOR_UZUME, 168, 536);
  } else {
    Actor_WalkToAndWait(ACTOR_UZUME, 120, 536);
  }
  Actor_FaceDirection(ACTOR_UZUME, FACING_SOUTH, 0);
  owner = (s32)&gMainRoutineWork;
  owner = owner + offsetof(struct MainRoutineWork, leaderActorId);
  Actor_FaceActor(ACTOR_SAVED_KUSHINADA, *(s32 *)owner, 0);
  // "Who are you?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
  Actor_ShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  FieldEvent_Wait(30);
  // "This is my home, and I am having a private conversation. Leave at once!"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
  FieldActor_Jump(ACTOR_WITNESS, 4, 0);
  Actor_SetAttachedEffect(ACTOR_WITNESS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_SAVED_KUSHINADA, FACING_SOUTH, 30);
  Actor_ShowEmote(ACTOR_SAVED_KUSHINADA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "What is it?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
  Actor_FaceActor(ACTOR_WITNESS, ACTOR_SAVED_KUSHINADA, 0);
  Actor_SetAttachedEffect(ACTOR_WITNESS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  // "There can be no doubt... These are the heroes who appeared on Mt...."
  Event_ShowMessage(ACTOR_WITNESS, 0);
  FieldEvent_Wait(30);
  FieldEvent_Wait(30);
  // "Are you the ones who defeated the serpent?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
  FieldActor_StartRepeatedMotion(ACTOR_SAVED_KUSHINADA, 2);
  FieldEvent_Wait(30);
  Actor_FaceActor(ACTOR_SAVED_KUSHINADA, *(s32 *)owner, 0);
  // "So it was you..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
  Actor_ShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "My apologies... You are welcome here."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
  Actor_WalkTo(ACTOR_UZUME, 144, 520);
  FieldActor_WaitForMoveById(ACTOR_UZUME);
  Actor_FaceDirection(ACTOR_UZUME, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SAVED_KUSHINADA, FACING_SOUTH, 0);
  Actor_SetAnimation(*(s32 *)owner, ANIM_NOD);
  FieldEvent_Wait(30);
  if (mode == 0) {
    Actor_WalkTo(ACTOR_WITNESS, 104, 568);
  } else {
    Actor_WalkTo(ACTOR_WITNESS, 184, 568);
  }
  FieldActor_WaitForMoveById(ACTOR_WITNESS);
  if (mode == 0) {
    Actor_FaceDirection(ACTOR_WITNESS, FACING_EAST, 0);
  } else {
    Actor_FaceDirection(ACTOR_WITNESS, FACING_WEST, 0);
  }
  party = (s32)&gMainRoutineWork;
  owner = party + offsetof(struct MainRoutineWork, leaderActorId);
  Actor_WalkTo(*(s32 *)owner, 144, 560);
  FieldActor_WaitForMoveById(*(s32 *)owner);
  Actor_FaceDirection(*(s32 *)owner, FACING_NORTH, 0);
  FieldActor_LaunchFromLeader(ACTOR_JENNA, 8, 24, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_SHEBA, 24, 24, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_PIERS, -8, 24, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, -24, 24, FACING_NORTH);
  FieldEvent_Wait(60);
  // "Who are you?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(30);
  // "I'm Kraden, and this is Felix."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAnimation(*(s32 *)owner, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_SetAnimation(ACTOR_UZUME, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_FaceActor(ACTOR_KRADEN, ACTOR_PIERS, 0);
  FieldEvent_Wait(30);
  // "Behind him is Piers."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  FieldEvent_Wait(30);
  // "Pleased to meet you."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_SetAnimation(ACTOR_UZUME, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(30);
  // "I'm Jenna, and this is Sheba."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_SetAnimation(ACTOR_UZUME, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_FaceActor(ACTOR_JENNA, ACTOR_KRADEN, 0);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(30);
  // "We're on a quest. Our travels across the Great Eastern Sea brought..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAnimation(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
  FieldEvent_Wait(30);
  // "You were seen coming down from Mt. Mikage. What brought you to such..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  FieldEvent_Wait(30);
  // "To complete our quest, we need many types of Psynergy."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_ShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Psynergy?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
  Actor_FaceEachOther(ACTOR_UZUME, ACTOR_SAVED_KUSHINADA, 0);
  FieldEvent_Wait(60);
  Actor_FaceActor(ACTOR_UZUME, *(s32 *)owner, 0);
  Actor_FaceActor(ACTOR_SAVED_KUSHINADA, *(s32 *)owner, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimation(ACTOR_SAVED_KUSHINADA, ANIM_SHAKE_HEAD);
  FieldEvent_Wait(30);
  // "What is Psynergy?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
  Actor_FaceActor(*(s32 *)owner, ACTOR_KRADEN, 0);
  Actor_FaceActor(ACTOR_JENNA, ACTOR_KRADEN, 0);
  Actor_FaceActor(ACTOR_SHEBA, ACTOR_KRADEN, 0);
  Actor_FaceActor(ACTOR_PIERS, ACTOR_KRADEN, 0);
  FieldEvent_Wait(30);
  Actor_FaceActor(ACTOR_KRADEN, *(s32 *)owner, 0);
  FieldEvent_Wait(30);
  Actor_FaceActor(ACTOR_KRADEN, ACTOR_UZUME, 0);
  FieldEvent_Wait(30);
  // "Psynergy is many things... The power to move things with one's mind..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_SAVED_KUSHINADA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldEvent_Wait(30);
  // "All with one's mind?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
  Actor_FaceActor(*(s32 *)owner, ACTOR_UZUME, 0);
  Actor_FaceActor(ACTOR_JENNA, ACTOR_UZUME, 0);
  Actor_FaceActor(ACTOR_SHEBA, ACTOR_UZUME, 0);
  Actor_FaceActor(ACTOR_PIERS, ACTOR_UZUME, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimation(*(s32 *)owner, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_ShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Tell me, is it something like...this?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
  FieldEvent_Wait(60);
  Actor_FaceDirection(ACTOR_UZUME, FACING_EAST, 0);
  FieldEvent_Wait(30);
  IzumoRooms_ShowMove();
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_UZUME, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  // "Is that the power you're talking about?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
  Actor_ShowEmote(*(s32 *)owner, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldEvent_Wait(30);
  FieldActor_StartRepeatedMotion(ACTOR_JENNA, 1);
  FieldEvent_Wait(15);
  // "That power? Lady Uzume?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceActor(ACTOR_UZUME, ACTOR_SAVED_KUSHINADA, 0);
  FieldEvent_Wait(30);
  Actor_FaceActor(ACTOR_UZUME, *(s32 *)owner, 0);
  FieldEvent_Wait(30);
  // "It's not mine alone. Kushinada and Susa possess it as well."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
  Actor_ShowEmote(ACTOR_SAVED_KUSHINADA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  FieldEvent_Wait(30);
  // "This is the power we possess. I assume you have something like it..."
  Event_OpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
  answer = Event_AskYesNoWithActorAnimation(*(s32 *)owner, 0);
  if (answer == 1) {
    FieldEvent_SetMessageId((s32)MsgIThoughtSo);
    Actor_ShowEmote(ACTOR_SAVED_KUSHINADA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    30);
    // "I thought so. Only those who can wield the power can see it. And you..."
    Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
    Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
    Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
    FieldEvent_Wait(30);
    Actor_FaceActor(ACTOR_PIERS, *(s32 *)owner, 0);
    FieldEvent_Wait(15);
    Actor_SetAnimation(ACTOR_PIERS, ANIM_SHAKE_HEAD);
    FieldEvent_Wait(30);
    // "Felix, there's no need to hide our abilities from them, is there?"
    Event_ShowMessage(ACTOR_PIERS, 0);
  }
  FieldEvent_SetMessageId((s32)MsgSoYouCall);
  Actor_SetAnimation(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
  FieldEvent_Wait(30);
  // "So, you call what we do "Psynergy," and you can do it, too?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
  Actor_SetAnimation(*(s32 *)owner, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_SetAnimation(ACTOR_UZUME, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
  FieldEvent_Wait(60);
  Actor_ShowEmote(ACTOR_SAVED_KUSHINADA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceActor(ACTOR_SAVED_KUSHINADA, *(s32 *)owner, 0);
  FieldEvent_Wait(30);
  // "If you already have Psynergy, why do you need more of this power?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SAVED_KUSHINADA, 0);
  FieldEvent_Wait(90);
  // "Their powers are different from our own. There seem to be many forms..."
  Event_ShowMessage(ACTOR_SUSA, 0);
  Actor_SetPosition(ACTOR_SUSA, 144, 680);
  Actor_FaceActor(*(s32 *)owner, ACTOR_SUSA, 0);
  Actor_FaceActor(ACTOR_JENNA, ACTOR_SUSA, 0);
  Actor_FaceActor(ACTOR_SHEBA, ACTOR_SUSA, 0);
  Actor_FaceActor(ACTOR_PIERS, ACTOR_SUSA, 0);
  Actor_FaceActor(ACTOR_KRADEN, ACTOR_SUSA, 0);
  Actor_FaceActor(ACTOR_WITNESS, ACTOR_SUSA, 0);
  FieldEvent_Wait(120);
  // "Susa!"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_UZUME, 0);
  FieldEvent_Wait(30);
  EventCamera_FollowActor(ACTOR_SUSA, 1);
  FieldEvent_Wait(120);
  Actor_SetSpeed(ACTOR_SUSA, 0.6);
  Actor_SetSpeed(ACTOR_SUSA, 0.6);
  Actor_GetById(ACTOR_SUSA)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_WalkTo(ACTOR_SUSA, 144, 648);
  if (mode == 0) {
    Actor_WalkToAndWait(ACTOR_SUSA, 88, 648);
    Actor_FaceActorById(*(s32 *)owner, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_JENNA, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_SHEBA, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_PIERS, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_UZUME, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_SAVED_KUSHINADA, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_WITNESS, ACTOR_SUSA, 0);
    Actor_WalkToAndWait(ACTOR_SUSA, 56, 616);
    Actor_FaceActorById(*(s32 *)owner, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_JENNA, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_SHEBA, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_PIERS, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_UZUME, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_SAVED_KUSHINADA, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_WITNESS, ACTOR_SUSA, 0);
    Actor_WalkToAndWait(ACTOR_SUSA, 56, 584);
    Actor_GetById(ACTOR_WITNESS)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(ACTOR_WITNESS, 88, 568);
    Event_Wait(1);
    Actor_GetById(ACTOR_WITNESS)->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_FaceDirectionById(ACTOR_WITNESS, FACING_EAST, 0);
    Actor_SetSpritePriority(ACTOR_SUSA, 1);
    Actor_WalkToAndWait(ACTOR_SUSA, 104, 568);
    Actor_FaceActorById(*(s32 *)owner, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_JENNA, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_SHEBA, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_PIERS, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_UZUME, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_SAVED_KUSHINADA, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_WITNESS, ACTOR_SUSA, 0);
    Actor_SetSpritePriority(ACTOR_KRADEN, 1);
    Actor_SetSpritePriority(ACTOR_SUSA, 2);
    Actor_WalkToAndWaitById(ACTOR_SUSA, 120, 568);
    Actor_SetSpritePriority(ACTOR_SUSA, 3);
    Actor_SetSpritePriority(ACTOR_KRADEN, 3);
    Actor_WalkToAndWait(ACTOR_SUSA, 120, 536);
    Actor_FaceActorById(*(s32 *)owner, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_JENNA, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_SHEBA, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_PIERS, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_UZUME, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_SAVED_KUSHINADA, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_WITNESS, ACTOR_SUSA, 0);
    Actor_SetAnimationById(ACTOR_WITNESS, ANIM_NOD);
    Actor_WalkToAndWait(ACTOR_WITNESS, 56, 584);
    Actor_WalkToAndWait(ACTOR_WITNESS, 88, 648);
    Actor_SetPosition(ACTOR_WITNESS, 0, 0);
  } else {
    Actor_WalkToAndWait(ACTOR_SUSA, 200, 648);
    Actor_FaceActorById(*(s32 *)owner, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_JENNA, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_SHEBA, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_PIERS, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_UZUME, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_SAVED_KUSHINADA, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_WITNESS, ACTOR_SUSA, 0);
    Actor_WalkToAndWait(ACTOR_SUSA, 232, 616);
    Actor_FaceActorById(*(s32 *)owner, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_JENNA, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_SHEBA, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_PIERS, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_UZUME, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_SAVED_KUSHINADA, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_WITNESS, ACTOR_SUSA, 0);
    Actor_WalkToAndWait(ACTOR_SUSA, 232, 584);
    Actor_GetById(ACTOR_WITNESS)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(ACTOR_WITNESS, 200, 568);
    Event_Wait(1);
    Actor_GetById(ACTOR_WITNESS)->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_FaceDirection(ACTOR_WITNESS, FACING_WEST, 0);
    Actor_SetSpritePriority(ACTOR_SUSA, 1);
    Actor_WalkToAndWait(ACTOR_SUSA, 184, 568);
    Actor_FaceActorById(*(s32 *)owner, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_JENNA, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_SHEBA, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_PIERS, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_UZUME, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_SAVED_KUSHINADA, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_WITNESS, ACTOR_SUSA, 0);
    Actor_SetSpritePriority(ACTOR_SUSA, 2);
    Actor_SetSpritePriority(ACTOR_SHEBA, 1);
    Actor_WalkToAndWaitById(ACTOR_SUSA, 168, 568);
    Actor_SetSpritePriority(ACTOR_SUSA, 3);
    Actor_SetSpritePriority(ACTOR_SHEBA, 3);
    Actor_WalkToAndWait(ACTOR_SUSA, 168, 536);
    Actor_FaceActorById(*(s32 *)owner, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_JENNA, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_SHEBA, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_PIERS, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_UZUME, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_SAVED_KUSHINADA, ACTOR_SUSA, 0);
    Actor_FaceActorById(ACTOR_WITNESS, ACTOR_SUSA, 0);
    Actor_SetAnimationById(ACTOR_WITNESS, ANIM_NOD);
    Actor_WalkToAndWait(ACTOR_WITNESS, 232, 584);
    Actor_WalkToAndWait(ACTOR_WITNESS, 200, 648);
    Actor_SetPosition(ACTOR_WITNESS, 0, 0);
  }
  person = Actor_GetById(ACTOR_SUSA);
  {
    u8 *priority = &person->priorityFlags;
    s32 flags = *priority | 1;

    owner = (s32)&gMainRoutineWork;
    *priority = flags;
  }
  owner = owner + offsetof(struct MainRoutineWork, leaderActorId);
  Actor_FaceDirection(*(s32 *)owner, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_NORTH, 0);
  Event_Wait(15);
  // "Susa..."
  Event_ShowMessageAndWait(ACTOR_SAVED_KUSHINADA, 0);
  Event_Wait(15);
  Actor_ShowEmote(ACTOR_SAVED_KUSHINADA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "So, you lived after all, Susa..."
  Event_ShowMessageAndWait(ACTOR_SAVED_KUSHINADA, 0);
  Actor_FaceActorById(ACTOR_SUSA, ACTOR_SAVED_KUSHINADA, 0);
  Event_Wait(30);
  // "Yes, I've survived, and I've made it home to you both."
  Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  Actor_FaceActorById(ACTOR_SUSA, *(s32 *)owner, 0);
  Event_Wait(60);
  Actor_FaceActorById(ACTOR_SUSA, ACTOR_SAVED_KUSHINADA, 0);
  Event_Wait(30);
  // "Thanks to them, actually..."
  Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  Actor_TurnToFaceEachOther(ACTOR_UZUME, ACTOR_SAVED_KUSHINADA, 0);
  Event_Wait(30);
  Actor_FaceActorById(ACTOR_UZUME, *(s32 *)owner, 0);
  Actor_FaceActorById(ACTOR_SAVED_KUSHINADA, *(s32 *)owner, 0);
  // "Felix, our gratitude to you and your friends grows deeper each..."
  Event_ShowMessageAndWait(ACTOR_UZUME, 0);
  Actor_SetAnimationById(ACTOR_UZUME, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
  Event_Wait(30);
  Actor_SetAnimationById(*(s32 *)owner, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Susa, you spoke of different types of power. Tell me, what did you mean?"
  Event_ShowMessageAndWait(ACTOR_UZUME, 0);
  Actor_FaceActorById(ACTOR_SUSA, *(s32 *)owner, 0);
  // "I can only describe what I saw, my sister. Ask Felix here about it."
  Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  Actor_FaceActorById(ACTOR_UZUME, *(s32 *)owner, 0);
  Actor_FaceActorById(ACTOR_SAVED_KUSHINADA, *(s32 *)owner, 0);
  Event_Wait(30);
  Actor_ShowEmoteById(*(s32 *)owner, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(30);
  // "Ah, yes. Psynergy... I have dedicated most of my life to its study..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_SetAnimationById(ACTOR_UZUME, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
  Event_Wait(30);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  // "The power of Psynergy comes from the four basic forces of earth..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_SAVED_KUSHINADA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_WITNESS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "You don't know Alchemy? The four elements? Why, they're the very..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "All existing matter comes from Alchemy and this Psynergy?"
  Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  Actor_FaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
  Event_Wait(30);
  // "Yes, that's right. Everything you see around you is some combination..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_ShowEmoteById(ACTOR_SAVED_KUSHINADA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB,
                      30);
  // "Does this mean that there are as many types of Psynergy as there are..."
  Event_ShowMessageAndWait(ACTOR_SAVED_KUSHINADA, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "That's right, Kushinada. You're a smart lass."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(30);
  // "Mt. Mikage was a place of great earth powers. I'd imagine your..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "That's right, but how did you know this?"
  Event_ShowMessageAndWait(ACTOR_UZUME, 0);
  Actor_FaceActorById(ACTOR_PIERS, ACTOR_KRADEN, 0);
  Event_Wait(30);
  // "Kraden... Mt. Mikage is Gaia Rock, isn't it?"
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_FaceActorById(ACTOR_KRADEN, ACTOR_PIERS, 0);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(30);
  Actor_ShowEmoteById(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceActorById(ACTOR_KRADEN, ACTOR_UZUME, 0);
  Actor_FaceActorById(ACTOR_PIERS, ACTOR_UZUME, 0);
  Event_Wait(30);
  // "...Gaia Rock? Mt. Mikage is the sacred mount of our people!"
  Event_ShowMessageAndWait(ACTOR_UZUME, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Yes, but there are many mountains like your Mt. Mikage all across..."
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_ShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "You have seen other mountains like our Mt. Mikage?"
  Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(30);
  // "Yes! In fact, it seems that each of these rocks contains a different..."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_SetAnimationById(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
  Event_Wait(30);
  // "And now, you are questing to gather the power of these elements, yes?"
  Event_ShowMessageAndWait(ACTOR_SAVED_KUSHINADA, 0);
  Actor_SetAnimationById(*(s32 *)owner, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(90);
  Actor_SetAnimationById(ACTOR_UZUME, ANIM_NOD);
  // "And you only defeated the Great Serpent of Mikage to accomplish this..."
  Event_OpenMessage(ACTOR_UZUME, 0);
  answer = Event_AskYesNoWithActorAnimation(*(s32 *)owner, 0);
  if (answer == 0) {
    Event_SetMessageId((s32)MsgRegardlessOfYour);
    Actor_ShowEmoteById(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    // "Regardless of your motives, you have saved us all."
    Event_ShowMessageAndWait(ACTOR_UZUME, 0);
  } else {
    Event_SetMessageId((s32)MsgYouTookPity);
    // "You took pity on Kushinada? You are very kind to help Susa, Felix."
    Event_ShowMessageAndWait(ACTOR_UZUME, 0);
  }
  message = (s32)MsgYouReNotImplying;
  Event_SetMessageId(message);
  Actor_StartRepeatedMotion(ACTOR_KRADEN, 1);
  Event_Wait(30);
  // "You're...not implying that we defeated the serpent, are you?"
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceActorById(ACTOR_UZUME, ACTOR_KRADEN, 0);
  Actor_ShowEmoteById(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(30);
  // "It's true that we did fight the creature..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
  Event_Wait(30);
  // "But without Susa's power, the battle might not have ended so happily."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_TurnToFaceEachOther(ACTOR_UZUME, ACTOR_SAVED_KUSHINADA, 0);
  Event_Wait(30);
  // "What... What are you°"
  Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  Event_Wait(30);
  // "Susa had given the beast a lot of Dragonsbane in order to weaken it."
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_FaceDirection(ACTOR_UZUME, FACING_SOUTH, 0);
  Actor_FaceDirectionById(ACTOR_SAVED_KUSHINADA, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(30);
  // "We could never have beaten the beast if it had not been in..."
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "We were victorious only because Susa risked everything to weaken the..."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_ShowEmoteById(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Event_Wait(15);
  Actor_FaceActorById(ACTOR_UZUME, ACTOR_SUSA, 0);
  // "Susa, perhaps, after all, you are..."
  Event_ShowMessageAndWait(ACTOR_UZUME, 0);
  Actor_ShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "No, I didn't..."
  Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  Actor_ShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  owner = (s32)&gMainRoutineWork;
  owner = owner + offsetof(struct MainRoutineWork, leaderActorId);
  Actor_FaceActorById(ACTOR_SHEBA, *(s32 *)owner, 0);
  // "Don't forget, Susa dealt the finishing blow, didn't he?"
  Event_OpenMessage(ACTOR_SHEBA, 0);
  Actor_FaceActorById(ACTOR_JENNA, *(s32 *)owner, 0);
  Actor_FaceActorById(ACTOR_SHEBA, *(s32 *)owner, 0);
  Actor_FaceActorById(ACTOR_PIERS, *(s32 *)owner, 0);
  Actor_FaceActorById(ACTOR_KRADEN, *(s32 *)owner, 0);
  Actor_FaceActorById(ACTOR_UZUME, *(s32 *)owner, 0);
  Actor_FaceActorById(ACTOR_SAVED_KUSHINADA, *(s32 *)owner, 0);
  Actor_FaceActorById(ACTOR_SUSA, *(s32 *)owner, 0);
  Event_Wait(30);
  answer = Event_AskYesNoWithActorAnimation(*(s32 *)owner, 0);
  if (answer == 0) {
    Event_SetMessageId(message + SUSA_ALL_I_DID_WAS_STAB_IT);
    Actor_SetAnimationById(ACTOR_SUSA, ANIM_SHAKE_HEAD);
    Event_Wait(30);
    // "Finishing blow? All I did was stab it with the Cloud Brand."
    Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  } else {
    Event_SetMessageId(message + SUSA_HARDLY_A_FINISHING_BLOW);
    Actor_SetAnimationById(ACTOR_SUSA, ANIM_NOD);
    Event_Wait(30);
    // "That's right. Piercing it with the Cloud Brand was hardly a..."
    Event_ShowMessageAndWait(ACTOR_SUSA, 0);
  }
  message = (s32)MsgIDisagree;
  Event_SetMessageId(message);
  Actor_FaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "I disagree..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(30);
  // "You weakened the serpent. You dealt the final blow. I'd say you were..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  owner = (s32)&gMainRoutineWork;
  owner = owner + offsetof(struct MainRoutineWork, leaderActorId);
  Actor_SetAnimationById(*(s32 *)owner, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceActorById(ACTOR_KRADEN, ACTOR_UZUME, 0);
  Event_Wait(30);
  // "So, Lady Uzume, wouldn't you say that Susa deserves the reward for..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceActorById(ACTOR_UZUME, *(s32 *)owner, 0);
  Actor_FaceActorById(ACTOR_KRADEN, *(s32 *)owner, 0);
  Event_Wait(30);
  // "Even Felix agrees, don't you, Felix?"
  Event_OpenMessage(ACTOR_KRADEN, 0);
  answer = Event_AskYesNoWithActorAnimation(*(s32 *)owner, 0);
  if (answer == 0) {
    Event_SetMessageId(message + 4);
    Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
    Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
    Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
    Event_Wait(30);
    // "I do, too."
    Event_ShowMessageAndWait(ACTOR_JENNA, 0);
    // "We weren't fighting in hopes of getting a reward, were we?"
    Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
    // "We'll be satisfied just learning your Psynergy."
    Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  } else {
    Event_SetMessageId(message + 7);
    Actor_SetAnimationById(ACTOR_JENNA, ANIM_SHAKE_HEAD);
    Actor_SetAnimationById(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
    Actor_SetAnimationById(ACTOR_PIERS, ANIM_SHAKE_HEAD);
    Event_Wait(30);
    // "Are you asking for a reward? That's just plain greedy!"
    Event_ShowMessageAndWait(ACTOR_JENNA, 0);
    // "We couldn't possibly take any sort of reward."
    Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
    // "Learning new Psynergy will be reward enough."
    Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  }
  Event_SetMessageId((s32)MsgAsYouCanSee);
  Actor_FaceActorById(ACTOR_KRADEN, ACTOR_UZUME, 0);
  Event_Wait(30);
  // "As you can see."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_TurnToFaceEachOther(ACTOR_UZUME, ACTOR_SAVED_KUSHINADA, 0);
  Event_Wait(30);
  Actor_SetAnimationById(ACTOR_UZUME, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
  Event_Wait(30);
  party = (s32)&gMainRoutineWork;
  owner = party + offsetof(struct MainRoutineWork, leaderActorId);
  Actor_FaceActorById(ACTOR_UZUME, *(s32 *)owner, 0);
  Actor_FaceActorById(ACTOR_SAVED_KUSHINADA, *(s32 *)owner, 0);
  Actor_StartRepeatedMotion(ACTOR_SUSA, 1);
  Event_Wait(15);
  Actor_ShowEmote(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  Actor_FaceActorById(ACTOR_KRADEN, *(s32 *)owner, 0);
  Event_Wait(30);
  // "Now, it's about time for us to leave Izumo Village."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceActorById(*(s32 *)owner, ACTOR_KRADEN, 0);
  Event_Wait(15);
  Actor_SetAnimationById(*(s32 *)owner, ANIM_NOD);
  Event_Wait(30);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(30);
  // "Yes, indeed! We're on our way to Lemuria."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceActorById(ACTOR_JENNA, ACTOR_KRADEN, 0);
  Actor_FaceActorById(ACTOR_SHEBA, ACTOR_KRADEN, 0);
  Actor_FaceActorById(ACTOR_PIERS, ACTOR_KRADEN, 0);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimationById(*(s32 *)owner, ANIM_NOD);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_SAVED_KUSHINADA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_Wait(30);
  // "You're leaving so soon?"
  Event_ShowMessageAndWait(ACTOR_SAVED_KUSHINADA, 0);
  Actor_FaceActorById(*(s32 *)owner, ACTOR_UZUME, 0);
  Actor_FaceActorById(ACTOR_JENNA, ACTOR_UZUME, 0);
  Actor_FaceActorById(ACTOR_SHEBA, ACTOR_UZUME, 0);
  Actor_FaceActorById(ACTOR_PIERS, ACTOR_UZUME, 0);
  Actor_FaceActorById(ACTOR_KRADEN, ACTOR_UZUME, 0);
  Event_Wait(30);
  // "We are in a great hurry."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_ShowEmoteById(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "Then you must return to Izumo someday."
  Event_ShowMessageAndWait(ACTOR_UZUME, 0);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(30);
  // "I'm sure we'll be back once our mission is completed."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_SetAnimationById(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
  Event_Wait(30);
  // "Excellent! I shall accept that as your oath!"
  Event_ShowMessageAndWait(ACTOR_SAVED_KUSHINADA, 0);
  Actor_ShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Event_Wait(30);
  // "...Oath?"
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_FaceActorById(ACTOR_SHEBA, ACTOR_JENNA, 0);
  Event_Wait(30);
  // "Oh, by promising to return, we'll be ensuring that our quest will..."
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_StartRepeatedMotion(ACTOR_PIERS, 1);
  Event_Wait(30);
  // "I understand... We must vow to return, everyone."
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_TurnToFaceEachOther(*(s32 *)owner, 7, 0);
  Actor_SetAnimationById(*(s32 *)owner, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  Actor_TurnToFaceEachOther(ACTOR_JENNA, ACTOR_SHEBA, 0);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(15);
  Actor_TurnToFaceEachOther(*(s32 *)owner, 5, 0);
  Actor_TurnToFaceEachOther(ACTOR_PIERS, ACTOR_SHEBA, 0);
  Actor_SetAnimationById(*(s32 *)owner, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceActorById(*(s32 *)owner, ACTOR_UZUME, 0);
  Actor_FaceActorById(ACTOR_JENNA, ACTOR_UZUME, 0);
  Actor_FaceActorById(ACTOR_SHEBA, ACTOR_UZUME, 0);
  Actor_FaceActorById(ACTOR_PIERS, ACTOR_UZUME, 0);
  Actor_FaceActorById(ACTOR_KRADEN, ACTOR_UZUME, 0);
  Actor_SetAnimationById(ACTOR_UZUME, ANIM_NOD);
  Event_Wait(30);
  // "We'll be waiting."
  Event_ShowMessageAndWait(ACTOR_UZUME, 0);
  Actor_SetAnimationById(*(s32 *)owner, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(15);
  Actor_SetAnimationById(ACTOR_UZUME, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SAVED_KUSHINADA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SUSA, ANIM_NOD);
  Event_Wait(60);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_WALK);
  leader = Actor_GetById(*(s32 *)owner);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_JENNA, leader->x.part[0].integer,
                             leader->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_WALK);
  leader = Actor_GetById(*(s32 *)owner);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_SHEBA, leader->x.part[0].integer,
                             leader->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_SHEBA);
  Actor_SetPosition(ACTOR_SHEBA, 0, 0);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_WALK);
  leader = Actor_GetById(*(s32 *)owner);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_PIERS, leader->x.part[0].integer,
                             leader->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_PIERS);
  Actor_SetPosition(ACTOR_PIERS, 0, 0);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_WALK);
  leader = Actor_GetById(*(s32 *)owner);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_KRADEN, leader->x.part[0].integer,
                             leader->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  GameFlag_Set(FLAG_IZUMO_RECOGNIZED_THE_HEROES);
  GameFlag_Set(FLAG_AREA_TEMP_50);
  Event_End();
}

/// The Move cast below has nothing to move.
s32 IzumoRooms_FindNoMoveTarget(void) { return -1; }

/// Uzume casts Move, with no target.
void IzumoRooms_ShowMove(void) {
  FieldPsynergy_LoadResources(ABILITY_MOVE, 1);
  FieldPsynergy_SetupActorPair(ACTOR_UZUME, -1);
  ((struct FieldEffectWork *)Ram_HeapSlots->fieldEffectWork)->finishCallback =
      (void (*)(void))IzumoRooms_FindNoMoveTarget;
  ((struct FieldEffectWork *)Ram_HeapSlots->fieldEffectWork)->enabled = 1;
  FieldPsynergy_StartCast();
  Event_Wait(10);
  FieldPsynergy_RunHandler(0);
  BattleEffect_StopMainActor();
  FieldPsynergy_End();
}

/// Lady Uzume takes the Dancing Idol, asks where it was found, and in thanks
/// brings out Coal, the Mars Djinni.
void IzumoRooms_GiveUzumeTheDoll(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgAhCouldIt);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 144, 536);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Event_Wait(15);
  Actor_ShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  // "Ah... Could it be? Is this the dancing doll of Mt. Mikage?"
  Event_OpenMessage(ACTOR_UZUME, 0);
  if (!Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0)) {
    Event_SetMessageId((s32)&MsgAhCouldIt + 1);
    // "You found this at the peak of Mt. Mikage?"
    Event_ShowMessageAndWait(ACTOR_UZUME, 0);
  } else {
    Event_SetMessageId((s32)&MsgAhCouldIt + 2);
    // "Don't lie to me. The style, the craftsmanship... You must have found..."
    Event_ShowMessageAndWait(ACTOR_UZUME, 0);
  }
  Actor_ShowEmoteById(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 60);
  Event_SetMessageId((s32)&MsgAgainYouHave);
  // "Again, you have done more for me than I can properly reward you for..."
  Event_ShowMessageAndWait(ACTOR_UZUME, 0);
  Actor_ShowEmote(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 120);
  Actor_ShowEmoteById(ACTOR_UZUME, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
  // "I think I may have something that might please you."
  Event_ShowMessageAndWait(ACTOR_UZUME, 0);
  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode(Actor_GetById(ACTOR_UZUME), 2);
  Sound_Play(SE_ASCEND);
  Event_Wait(60);
  Actor_SetPosition(ACTOR_COAL, 144, 520);
  Actor_Jump(ACTOR_COAL, 6, 0);
  Actor_SetDestination(ACTOR_COAL, 160, 520);
  Actor_WaitForMoveById(ACTOR_COAL);
  Actor_FaceDirectionById(ACTOR_COAL, FACING_SOUTH, 0);
  Event_Wait(60);
  BattleEffect_SetObjectUpdateMode(Actor_GetById(ACTOR_UZUME), 0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  Event_Wait(30);
  Event_Wait(30);
  Actor_FaceActorById(ACTOR_UZUME, ACTOR_COAL, 0);
  Event_Wait(30);
  // "This is my pet. It's very dear to me."
  Event_ShowMessageAndWait(ACTOR_UZUME, 0);
  Actor_FaceActorById(ACTOR_UZUME, gMainRoutineWork.leaderActorId, 0);
  Event_Wait(60);
  // "I am unsure how it works, but this fellow seems to magnify our power..."
  Event_ShowMessageAndWait(ACTOR_UZUME, 0);
  Event_Wait(30);
  Actor_SetAnimationById(ACTOR_UZUME, ANIM_SHAKE_HEAD);
  Event_Wait(60);
  // "Though it saddens me to part with it, I would have you take him, as..."
  Event_ShowMessageAndWait(ACTOR_UZUME, 0);
  Actor_TurnToFaceEachOther(ACTOR_UZUME, ACTOR_COAL, 0);
  // "Felix and his friends have agreed to care for you from now on."
  Event_ShowMessageAndWait(ACTOR_UZUME, 0);
  Event_Wait(60);
  Actor_Jump(ACTOR_COAL, 4, 20);
  Event_Wait(60);
  Actor_FaceActorById(ACTOR_UZUME, gMainRoutineWork.leaderActorId, 0);
  Actor_FaceActorById(ACTOR_COAL, gMainRoutineWork.leaderActorId, 0);
  Event_Wait(60);
  // "Please, accept this gift."
  Event_ShowMessageAndWait(ACTOR_UZUME, 0);
  Actor_Jump(ACTOR_COAL, 2, 20);
  Djinn_JoinParty(ACTOR_COAL, DJINNI_COAL / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_COAL));
  Event_End();
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActorById(gMainRoutineWork.leaderActorId, ACTOR_UZUME, 0);
  Event_Wait(60);
  Actor_StartRepeatedMotion(ACTOR_UZUME, 2);
  Event_Wait(30);
  // "With this dancing doll, I can explore Mt. Mikage on my own. Take..."
  Event_ShowMessageAndWait(ACTOR_UZUME, 0);
  Event_Wait(60);
  GameFlag_Set(FLAG_UZUME_HAS_THE_DOLL);
  PartyInventory_Remove(ITEM_DANCING_IDOL);
  Event_End();
}

/// Sets up Izumo's houses as the party enters: the window in, Susa's house for
/// the heroes' welcome or the festival, and Kushinada and the villagers before
/// the Serpent is slain. Returns 0.
s32 Scene_Initialize(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  if (gMainRoutineWork.entrance != 7)
    Event_ClearFlag(FLAG_AREA_TEMP_50);
  if (GameFlag_Get(FLAG_SERPENT_SLAIN) &&
      !GameFlag_Get(FLAG_IZUMO_RECOGNIZED_THE_HEROES) &&
      gMainRoutineWork.entrance == IZUMO_ROOMS_FROM_IZUMO_NORTH) {
    Event_PlaySound(MUS_IZUMO);
    Actor_SetPosition(ACTOR_WITNESS, 144, 568);
    Actor_SetPosition(ACTOR_SUSA, 0, 0);
    Actor_SetPosition(ACTOR_FIRST_DANCER, 0, 0);
    Actor_SetPosition(ACTOR_SECOND_DANCER, 0, 0);
    Actor_SetPosition(ACTOR_WEST_VILLAGER, 0, 0);
    Actor_SetPosition(ACTOR_EAST_VILLAGER, 0, 0);
  }
  if (GameFlag_Get(FLAG_SERPENT_SLAIN) &&
      GameFlag_Get(FLAG_IZUMO_RECOGNIZED_THE_HEROES) &&
      gMainRoutineWork.entrance == IZUMO_ROOMS_FROM_IZUMO_NORTH &&
      GameFlag_Get(FLAG_AREA_TEMP_50)) {
    Event_PlaySound(MUS_IZUMO);
    Actor_SetPosition(ACTOR_FIRST_DANCER, 0, 0);
    Actor_SetPosition(ACTOR_SECOND_DANCER, 0, 0);
    Actor_SetPosition(ACTOR_WEST_VILLAGER, 0, 0);
    Actor_SetPosition(ACTOR_EAST_VILLAGER, 0, 0);
  }
  if (gMainRoutineWork.entrance == 5 && !GameFlag_Get(FLAG_SERPENT_SLAIN)) {
    Actor_SetAnimation(ACTOR_KUSHINADA, 5);
    Actor_SetAttachedEffect(ACTOR_KUSHINADA,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  }
  if (gMainRoutineWork.entrance == IZUMO_ROOMS_FROM_IZUMO_NORTH &&
      !GameFlag_Get(FLAG_SERPENT_SLAIN)) {
    struct Actor *actor;

    Actor_SetAnimation(ACTOR_SUSAS_HOUSE_FIRST_DANCER, 5);
    Actor_SetAnimation(ACTOR_SUSAS_HOUSE_SECOND_DANCER, 5);
    Actor_SetAnimation(ACTOR_SUSAS_HOUSE_WEST_VILLAGER, 5);
    Actor_SetAnimation(ACTOR_SUSAS_HOUSE_EAST_VILLAGER, 5);
    actor = Actor_GetById(ACTOR_SUSAS_HOUSE_WEST_VILLAGER);
    actor->scaleX = -Actor_GetById(ACTOR_SUSAS_HOUSE_WEST_VILLAGER)->scaleX;
  }
  return 0;
}

/// Nothing to prepare before the map is built. Returns 0.
s32 Scene_PrepareMap(void) { return 0; }

extern s32 gIzumoRooms_KushinadaHeavingScript[];

/// The dancers in Susa's house step to the doorway and back with a hop, each
/// waiting until the other is clear of the spot.
s32 gIzumoRooms_FirstDancerScript[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ACCELERATION(0.2),
    SC_SET(SW_GRAVITY, toFIXED(0.4)),
    SC_SPEED(0.6),
    SC_SET(SW_STEERING, 2),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_LABEL(1),
    SC_CALL(IzumoRooms_IsFirstDancerClear),
    SC_MOVE(112, 0, 624),
    SC_SYNC,
    SC_MOVE(112, 0, 600),
    SC_SYNC,
    SC_MOVE(136, 0, 600),
    SC_SYNC,
    SC_MOVE(136, 0, 624),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(5),
    SC_ADD(SW_Y, toFIXED(4)),
    SC_WAIT(30),
    SC_MOVE(112, 0, 624),
    SC_SYNC,
    SC_MOVE(136, 0, 600),
    SC_SYNC,
    SC_MOVE(112, 0, 600),
    SC_SYNC,
    SC_MOVE(136, 0, 624),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(5),
    SC_ADD(SW_Y, toFIXED(4)),
    SC_WAIT(30),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    -1,
};

s32 gIzumoRooms_SecondDancerScript[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ACCELERATION(0.2),
    SC_SET(SW_GRAVITY, toFIXED(0.4)),
    SC_SPEED(0.6),
    SC_SET(SW_STEERING, 2),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_LABEL(1),
    SC_CALL(IzumoRooms_IsSecondDancerClear),
    SC_MOVE(176, 0, 624),
    SC_SYNC,
    SC_MOVE(176, 0, 600),
    SC_SYNC,
    SC_MOVE(152, 0, 600),
    SC_SYNC,
    SC_MOVE(152, 0, 624),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(5),
    SC_ADD(SW_Y, toFIXED(4)),
    SC_WAIT(30),
    SC_MOVE(176, 0, 624),
    SC_SYNC,
    SC_MOVE(152, 0, 600),
    SC_SYNC,
    SC_MOVE(176, 0, 600),
    SC_SYNC,
    SC_MOVE(152, 0, 624),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(5),
    SC_ADD(SW_Y, toFIXED(4)),
    SC_WAIT(30),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    -1,
};

// clang-format off
/// A girl heaves with sobs ("*Sniff* How could we sacrifice our beloved
/// Kushinada?"): her sprite is squashed and stretched three times, then rests.
s32 gIzumoRooms_HeavingScript[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ANIMATION(0),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 4),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(40),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 4),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(5),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 4),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(30),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_PUSHABLE | ACTOR_SOLID),
    SC_ACCELERATION(0.1),
    SC_SPEED(0.3),
    SC_ANIMATION(2),
    SC_LABEL(1),
    SC_MOVE(264, 0, 120),
    SC_SYNC,
    SC_WAIT(60),
    SC_MOVE(264, 0, 96),
    SC_SYNC,
    SC_WAIT(60),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    -1,
};

/// Kushinada on her seat of stones heaves as gIzumoRooms_HeavingScript's girl
/// does, with longer rests.
s32 gIzumoRooms_KushinadaHeavingScript[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_PUSHABLE | ACTOR_SOLID),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 4),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(60),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 4),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(10),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 4),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(90),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ACCELERATION(0.2),
    SC_SET(SW_GRAVITY, toFIXED(0.1)),
    SC_SPEED(0.4),
    SC_ANIMATION(0),
    SC_LABEL(1),
    SC_SET(SW_Y, toFIXED(4)),
    SC_WAIT(30),
    SC_END,
};
// clang-format on

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(IzumoRooms),
    SCENE_EXIT_UNLESS(1, Izumo, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Izumo, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, Izumo, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, Izumo, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, Izumo, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, Izumo, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, Izumo, 8, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gIzumoRooms_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 120, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 152, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 88, 0, 88, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 96, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 80, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 384, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 84, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 136, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 120, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KUSHINADA, CONDITION_ALWAYS, (s32)gIzumoRooms_KushinadaHeavingScript, 352, 0, 328, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_CREAM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 362, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_MAN_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 108, 0, 320, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_ORANGE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 632, 0, 88, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 560, 0, 328, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_GIRL, CONDITION_ALWAYS, (s32)gIzumoRooms_HeavingScript, 504, 0, 360, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_GIRL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 392, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gIzumoRooms_SusasHouseActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LADY_UZUME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 144, 0, 520, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, (s32)gIzumoRooms_FirstDancerScript, 136, 0, 624, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, (s32)gIzumoRooms_SecondDancerScript, 152, 0, 624, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 90, 0, 622, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 198, 0, 622, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gIzumoRooms_ActorPlacementsOnceTheSerpentIsSlain[] = {
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_CREAM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 362, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_MAN_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 108, 0, 320, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_ORANGE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 632, 0, 88, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 560, 0, 328, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_GIRL, CONDITION_ALWAYS, (s32)gIzumoRooms_HeavingScript, 600, 0, 360, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_GIRL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 376, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_LADY_UZUME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 144, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 624, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 624, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 616, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_OLD_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 616, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KUSHINADA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 520, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUSA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 552, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_COAL), MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gIzumoRooms_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_STEP, 21, CONDITION_ALWAYS,
     EVENT(IzumoRooms_KushinadaAsksSusaNotToCome)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgOkuniKushinadaAnd)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgIfLadyUzume)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgIMSoRelieved)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgWhatIWant)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgAWhileBack)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgThatDragonsbaneSeems)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgICanTFind)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgIAmOld)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTheSerpentCan)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(IzumoRooms_TalkToMerchant)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgOneDayKushinada)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, EVENT(IzumoRooms_TalkToInnkeeper)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgIfIWere)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, EVENT(IzumoRooms_TalkToPriest)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgSniffHowCould)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgWhenIGet)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgSusaSBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgLadyUzumeS)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgIOnlySuggested)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgTheSerpentIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgIWonderIfAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgTheSerpentWill)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgAccordingToLegend)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgOurGuardianSerpent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgThereSNo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgThePowerThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgItWasThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgThingsLikeThis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgWhatDoesThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgIfOnlyI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS,
     MSGNO(MsgIDonTWantThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS,
     MSGNO(MsgNobodyWantsTo)},
    {TRIGGER_TABLE_END},
};

struct Trigger gIzumoRooms_SusasHouseTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, ACTOR_SUSAS_HOUSE_UZUME, CONDITION_ALWAYS,
     EVENT(IzumoRooms_AskAfterSusa)},
    {TRIGGER_TALK, ACTOR_SUSAS_HOUSE_FIRST_DANCER, CONDITION_ALWAYS,
     MSGNO(MsgThereSNoPoint)},
    {TRIGGER_TALK, ACTOR_SUSAS_HOUSE_SECOND_DANCER, CONDITION_ALWAYS,
     MSGNO(MsgTheSerpentAttacked)},
    {TRIGGER_TALK, ACTOR_SUSAS_HOUSE_WEST_VILLAGER, CONDITION_ALWAYS,
     MSGNO(MsgTheSerpentAte)},
    {TRIGGER_TALK, ACTOR_SUSAS_HOUSE_EAST_VILLAGER, CONDITION_ALWAYS,
     MSGNO(MsgTheSerpentLives)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSAS_HOUSE_UZUME,
     CONDITION_ALWAYS, EVENT(IzumoRooms_ReadPrayerForSusa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSAS_HOUSE_FIRST_DANCER,
     CONDITION_ALWAYS, MSGNO(MsgOkuniSSuch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSAS_HOUSE_SECOND_DANCER,
     CONDITION_ALWAYS, MSGNO(MsgTheSerpentWould)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSAS_HOUSE_WEST_VILLAGER,
     CONDITION_ALWAYS, MSGNO(MsgBeforeTheSerpent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSAS_HOUSE_EAST_VILLAGER,
     CONDITION_ALWAYS, MSGNO(MsgLegendHasIt)},
    {TRIGGER_TABLE_END},
};

struct Trigger gIzumoRooms_TriggersOnceTheSerpentIsSlain[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_STEP, 22, FLAG_IZUMO_RECOGNIZED_THE_HEROES,
     EVENT(IzumoRooms_HeroesComeFromTheEast)},
    {TRIGGER_STEP, 23, FLAG_IZUMO_RECOGNIZED_THE_HEROES,
     EVENT(IzumoRooms_HeroesComeFromTheWest)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(IzumoRooms_TalkToMerchant)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgMyGoodsAre)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(IzumoRooms_TalkToInnkeeper)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgAsLadyUzume)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(IzumoRooms_TalkToPriest)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgNowThatTheSerpent)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgIfYouReA)},
    {TRIGGER_TALK, ACTOR_UZUME, CONDITION_ALWAYS,
     EVENT(IzumoRooms_TalkToUzume)},
    {TRIGGER_TALK, ACTOR_FIRST_DANCER, CONDITION_ALWAYS,
     EVENT(IzumoRooms_AskAboutMtMikage)},
    {TRIGGER_TALK, ACTOR_SECOND_DANCER, CONDITION_ALWAYS,
     MSGNO(MsgSupposedlyTheDoll)},
    {TRIGGER_TALK, ACTOR_WEST_VILLAGER, CONDITION_ALWAYS,
     MSGNO(MsgLadyUzumeHas)},
    {TRIGGER_TALK, ACTOR_EAST_VILLAGER, CONDITION_ALWAYS,
     MSGNO(MsgLadyUzumeSNew)},
    {TRIGGER_TALK, ACTOR_SAVED_KUSHINADA, CONDITION_ALWAYS,
     MSGNO(MsgSomedaySusaAnd)},
    {TRIGGER_TALK, ACTOR_SUSA, CONDITION_ALWAYS, MSGNO(MsgIMSureYou)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgIDLoveTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgThisMayBe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgIfBriggsS)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgIUnderstandNow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgIThankThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgMaybeNowKushinada)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgThatDragonWanted)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_UZUME | TRIGGER_SUBJECT_OWN_EFFECT, CONDITION_ALWAYS,
     EVENT(IzumoRooms_ReadUzumesMind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_DANCER, CONDITION_ALWAYS,
     MSGNO(MsgWhyDoesLadyUzume)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_DANCER,
     CONDITION_ALWAYS, MSGNO(MsgIWonderWhatSort)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_VILLAGER,
     CONDITION_ALWAYS, MSGNO(MsgIHeardThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_VILLAGER,
     CONDITION_ALWAYS, MSGNO(MsgIDonTKnowWhatIt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAVED_KUSHINADA,
     CONDITION_ALWAYS, MSGNO(MsgIfSDescendants)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSA, CONDITION_ALWAYS,
     MSGNO(MsgIMNo)},
    {USE_ITEM_ON_ACTOR(ITEM_DANCING_IDOL), ACTOR_UZUME, FLAG_UZUME_HAS_THE_DOLL,
     EVENT(IzumoRooms_GiveUzumeTheDoll)},
    {TRIGGER_TABLE_END},
};

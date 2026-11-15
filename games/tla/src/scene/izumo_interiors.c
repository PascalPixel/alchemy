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

extern struct ActorPlacement gIzumoInteriors_ActorPlacements[];
extern struct ActorPlacement gIzumoInteriors_SusasHouseActorPlacements[];
extern struct ActorPlacement
    gIzumoInteriors_ActorPlacementsOnceTheSerpentIsSlain[];
extern struct Trigger gIzumoInteriors_Triggers[];
extern struct Trigger gIzumoInteriors_SusasHouseTriggers[];
extern struct Trigger gIzumoInteriors_TriggersOnceTheSerpentIsSlain[];

void Inn_CheckIn(s32 slot, s32 actor);

void IzumoInteriors_RecognizeTheHeroes(s32 mode);

void IzumoInteriors_ShowMove(void);
extern s32 gIzumoInteriors_FirstPacerScript[];
extern s32 gIzumoInteriors_SecondPacerScript[];
extern s32 gIzumoInteriors_RockingScript[];

/// Whether the other of the two men pacing in Susa's house is clear of the
/// spot by the doorway this one walks to; their scripts wait on it.
#define DEFINE_IZUMO_INTERIORS_IS_CLEAR(name, n1, n2)                          \
  s32 name(void) {                                                             \
    struct Actor *object = Actor_GetById(n1);                                  \
                                                                               \
    if ((u32)(object->x.fixed - n2) <= 0x80000) {                              \
      s32 z = object->z.fixed;                                                 \
                                                                               \
      if (z >= 0x26c0000) {                                                    \
        if (z <= 0x2740000)                                                    \
          return 0;                                                            \
      }                                                                        \
    }                                                                          \
    return 1;                                                                  \
  }

DEFINE_IZUMO_INTERIORS_IS_CLEAR(IzumoInteriors_IsFirstPacerClear, 10, 0x940000)

DEFINE_IZUMO_INTERIORS_IS_CLEAR(IzumoInteriors_IsSecondPacerClear, 9, 0x840000)

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Shop_Run(s32 slot, s32 actor);

static inline void LeaderAnimate(s32 animation, s32 frames) {
  Actor_ShowEmoteById(gMainRoutineWork.leaderActorId, animation, frames);
}

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (GameFlag_Get(FLAG_SERPENT_SLAIN))
    return gIzumoInteriors_ActorPlacementsOnceTheSerpentIsSlain;
  if (gMainRoutineWork.entrance == 7)
    return gIzumoInteriors_SusasHouseActorPlacements;
  return gIzumoInteriors_ActorPlacements;
}

const struct Trigger *Scene_GetTriggers(void) {
  if (GameFlag_Get(FLAG_SERPENT_SLAIN))
    return gIzumoInteriors_TriggersOnceTheSerpentIsSlain;
  if (gMainRoutineWork.entrance == 7)
    return gIzumoInteriors_SusasHouseTriggers;
  return gIzumoInteriors_Triggers;
}

void IzumoInteriors_TalkToHealer(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Sanctum_Run(actor);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (GameFlag_Get(FLAG_SERPENT_SLAIN)) {
      Event_SetMessageId((s32)MsgNowThatPeace);
      // "Now that peace has been restored, our island is flourishing once..."
      Event_ShowMessageAndWait(12, 0);
    } else {
      Event_SetMessageId((s32)MsgWhyWouldOur);
      // "Why would our guardian spirit demand a sacrifice? It's..."
      Event_ShowMessageAndWait(22, 0);
    }
    Event_End();
  }
}

/// Izumo's shops, as Shop_Run numbers them.
enum { SHOP_IZUMO = 24 };

/// Izumo's inn, as Inn_CheckIn numbers the inns.
enum { IZUMO_INN = 9 };

void IzumoInteriors_TalkToInnkeeper(s32 actor) {
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

void IzumoInteriors_TalkToMerchant(s32 actor) {
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

void IzumoInteriors_AskAfterSusa(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (!Event_GetFlag(0x922)) {
    Event_SetMessageId((s32)MsgSusaSNot);
    // "Susa's not here. I believe he went to Mt. Mikage."
    Event_ShowMessageAndWait(8, 0);
    Event_Wait(30);
    Actor_ShowEmoteById(8, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
    Event_Wait(60);
    // "He believes he can save Kushinada, and so he has gone to fight the..."
    Event_ShowMessageAndWait(8, 0);
    Event_Wait(30);
    Actor_ShowEmoteById(8, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    Event_Wait(60);
    // "He truly believes he can defeat the Great Serpent of Mikage."
    Event_ShowMessageAndWait(8, 0);
    Event_Wait(30);
    Actor_StartRepeatedMotion(8, 1);
    Event_Wait(60);
    // "It's all for naught, though, as a hero will certainly appear and..."
    Event_ShowMessageAndWait(8, 0);
    Event_Wait(30);
    GameFlag_Set(0x922);
  }
  Actor_SetAnimationById(8, 4);
  Event_Wait(60);
  Event_SetMessageId((s32)MsgIfTheSerpent);
  // "If the serpent defeats Susa, I'll...be alone."
  Event_ShowMessageAndWait(8, 0);
  Event_End();
}

void IzumoInteriors_ReadPrayerForSusa(void) {
  GameFlag_Set(0x923);
  Event_SetMessageId((s32)MsgIPrayThat);
  // "I pray that our guardian's anger is not aroused and that Susa will..."
  Event_ShowMessageAndWait(8, 0);
}

void IzumoInteriors_TalkToUzume(void) {
  void *work = Ram_HeapSlots->fieldWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActorById(15, gMainRoutineWork.leaderActorId, 0);
  if (GameFlag_Get(FLAG_UZUME_HAS_THE_DOLL)) {
    Event_SetMessageId((s32)MsgWithThisDancing);
    // "With this dancing doll, I can explore Mt. Mikage on my own. Take..."
    Event_ShowMessageAndWait(15, 0);
  } else {
    Event_SetMessageId((s32)MsgIfYouHave);
    // "If you have time, won't you enjoy a little of our celebratory festival?"
    Event_ShowMessageAndWait(15, 0);
    if (PartyInventory_FindMember(ITEM_DANCING_IDOL) != -1)
      FIELD_AT(work, u16, 346) = 1;
  }
  Actor_FaceDirection(15, FACING_SOUTH, 0);
  Event_End();
}

void IzumoInteriors_AskAboutMtMikage(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgTheyVeBeen;
  Event_SetMessageId(msg);
  // "They've been saying that you went into Mt. Mikage, too... Is that true?"
  FieldMessage_ShowAtActor(16, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(16, 0);
  Event_End();
}

void IzumoInteriors_ReadUzumesMind(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActorById(15, gMainRoutineWork.leaderActorId, 0);
  if (Event_GetFlag(0x92b)) {
    Event_SetMessageId((s32)MsgTryingAgainWhat);
    // "Trying again, Felix? What a pest. You can see my mind, but you are..."
    Event_ShowMessageAndWait(15, 0);
  } else {
    Event_SetMessageId((s32)MsgAndWhatWere);
    // "And what were you planning to do, once you'd read my mind, hm?"
    Event_ShowMessageAndWait(15, 0);
    if (GameFlag_Get(0x923)) {
      LeaderAnimate(256, 30);
      Actor_ShowEmoteById(15, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
      Event_ShowMessageAndWait(15, 0);
      LeaderAnimate(258, 30);
      Actor_SetAnimationById(15, 4);
      Event_Wait(30);
      Event_ShowMessageAndWait(15, 0);
    }
    GameFlag_Set(0x92b);
  }
  Actor_FaceDirection(15, FACING_SOUTH, 0);
  Event_End();
}

s32 IzumoInteriors_KushinadaPleads(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (!Event_GetFlag(0x925)) {
    Event_SetMessageId((s32)MsgSusaDonT);
    // "Susa, don't come for me..."
    Event_ShowMessageAndWait(17, 0);
    Event_Wait(60);
    // "If I see you, I'll surely lose the courage to sacrifice myself for..."
    Event_ShowMessageAndWait(17, 0);
    Event_Wait(60);
    Event_Wait(15);
    // "If I run, Lady Uzume and all of Izumo will suffer greatly......"
    Event_ShowMessageAndWait(17, 0);
    Event_Wait(60);
    Event_Wait(15);
    // "Lady Uzume has foretold the coming of a hero, and I believe her..."
    Event_ShowMessageAndWait(17, 0);
    Event_Wait(60);
    Actor_StartRepeatedMotion(17, 1);
    Event_Wait(30);
    // "And so I shall remain here, until the coming of the next full..."
    Event_ShowMessageAndWait(17, 0);
    Event_Wait(60);
    Event_SetFlag(0x925);
  } else {
    Event_SetMessageId((s32)MsgSusaPleaseDon);
    // "Susa, please... Don't try to save me."
    Event_ShowMessageAndWait(17, 0);
  }
  Actor_GetById(gMainRoutineWork.leaderActorId)->steeringFlags &= ~1;
  Actor_WalkByById(gMainRoutineWork.leaderActorId, 0, 16);
  Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_GetById(gMainRoutineWork.leaderActorId)->steeringFlags |= 1;
  Event_End();
  return 0;
}

void IzumoInteriors_HeroesComeFromTheEast(void) {
  IzumoInteriors_RecognizeTheHeroes(0);
}

void IzumoInteriors_HeroesComeFromTheWest(void) {
  IzumoInteriors_RecognizeTheHeroes(1);
}

/// Izumo hears that warriors appeared from within Mt. Mikage and recognizes
/// the party as the heroes, through three answers; the party gathers again
/// and FLAG_IZUMO_RECOGNIZED_THE_HEROES is set. Mode 0 and mode 1
/// (IzumoInteriors_HeroesComeFromTheEast and
/// IzumoInteriors_HeroesComeFromTheWest) start the party at different spots.
void IzumoInteriors_RecognizeTheHeroes(s32 mode) {
  struct Actor *person, *leader;
  s32 party, owner, answer, message;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(143, CAMERA_KEEP, 560, 1);
  party = (s32)&gMainRoutineWork;
  if (mode == 0) {
    Actor_WalkToAndWait(*(s32 *)(party + 532), 184, 568);
  } else {
    Actor_WalkToAndWait(*(s32 *)(party + 532), 104, 568);
  }
  FieldEvent_Wait(15);
  FieldEvent_SetMessageId((s32)MsgWarriorsAppearedFrom);
  // "Warriors appeared from within Mt. Mikage?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 15, 0);
  Actor_SetAnimation(22, 3);
  // "Yes... After the serpent's roar, these warriors appeared."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 22, 0);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "So, it wasn't Susa who defeated the serpent, but those heroes..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 15, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimation(22, 3);
  FieldEvent_Wait(15);
  // "It might have been..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 22, 0);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  FieldActor_StartRepeatedMotion(22, 3);
  Actor_SetAttachedEffect(22, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(30);
  // "Lady Kushinada, please forgive me..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 22, 0);
  Actor_FaceDirection(20, FACING_EAST, 30);
  Actor_SetAnimation(20, 4);
  // "I don't care who defeated the serpent... I'm more worried about what..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 20, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(15, FACING_WEST, 30);
  Actor_SetAnimation(15, 3);
  FieldEvent_Wait(30);
  // "So, Susa never came down from Mt. Mikage?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 15, 0);
  Actor_SetAnimation(22, 3);
  FieldEvent_Wait(15);
  // "He returned to the village after the heroes appeared, and he knew..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 22, 0);
  FieldActor_StartRepeatedMotion(20, 2);
  FieldEvent_Wait(15);
  // "Oh, Susa..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 20, 0);
  Actor_FaceActor(15, gMainRoutineWork.leaderActorId, 0);
  FieldEvent_Wait(30);
  Actor_SetDestinationOffset(15, 0, 16);
  FieldActor_WaitForMoveById(15);
  FieldEvent_Wait(15);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  if (mode == 0) {
    Actor_WalkToAndWait(15, 168, 536);
  } else {
    Actor_WalkToAndWait(15, 120, 536);
  }
  Actor_FaceDirection(15, FACING_SOUTH, 0);
  owner = (s32)&gMainRoutineWork;
  owner = owner + 532;
  Actor_FaceActor(20, *(s32 *)owner, 0);
  // "Who are you?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 15, 0);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  FieldEvent_Wait(30);
  // "This is my home, and I am having a private conversation. Leave at once!"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 15, 0);
  FieldActor_Jump(22, 4, 0);
  Actor_SetAttachedEffect(22,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  FieldEvent_Wait(30);
  Actor_FaceDirection(20, FACING_SOUTH, 30);
  Actor_ShowEmote(20, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "What is it?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 20, 0);
  Actor_FaceActor(22, 20, 0);
  Actor_SetAttachedEffect(22,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  // "There can be no doubt... These are the heroes who appeared on Mt...."
  Event_ShowMessage(22, 0);
  FieldEvent_Wait(30);
  FieldEvent_Wait(30);
  // "Are you the ones who defeated the serpent?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 15, 0);
  FieldActor_StartRepeatedMotion(20, 2);
  FieldEvent_Wait(30);
  Actor_FaceActor(20, *(s32 *)owner, 0);
  // "So it was you..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 20, 0);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "My apologies... You are welcome here."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 15, 0);
  Actor_WalkTo(15, 144, 520);
  FieldActor_WaitForMoveById(15);
  Actor_FaceDirection(15, FACING_SOUTH, 0);
  Actor_FaceDirection(20, FACING_SOUTH, 0);
  Actor_SetAnimation(*(s32 *)owner, 3);
  FieldEvent_Wait(30);
  if (mode == 0) {
    Actor_WalkTo(22, 104, 568);
  } else {
    Actor_WalkTo(22, 184, 568);
  }
  FieldActor_WaitForMoveById(22);
  if (mode == 0) {
    Actor_FaceDirection(22, FACING_EAST, 0);
  } else {
    Actor_FaceDirection(22, FACING_WEST, 0);
  }
  party = (s32)&gMainRoutineWork;
  owner = party + 532;
  Actor_WalkTo(*(s32 *)owner, 144, 560);
  FieldActor_WaitForMoveById(*(s32 *)owner);
  Actor_FaceDirection(*(s32 *)owner, FACING_NORTH, 0);
  FieldActor_LaunchFromLeader(ACTOR_JENNA, 8, 24, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_SHEBA, 24, 24, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_PIERS, -8, 24, FACING_NORTH);
  FieldActor_LaunchFromLeader(23, -24, 24, FACING_NORTH);
  FieldEvent_Wait(60);
  // "Who are you?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 20, 0);
  Actor_SetAnimation(23, 3);
  FieldEvent_Wait(30);
  // "I'm Kraden, and this is Felix."
  Event_ShowMessage(23, 0);
  Actor_SetAnimation(*(s32 *)owner, 3);
  FieldEvent_Wait(30);
  Actor_SetAnimation(15, 3);
  Actor_SetAnimation(20, 3);
  FieldEvent_Wait(30);
  Actor_FaceActor(23, ACTOR_PIERS, 0);
  FieldEvent_Wait(30);
  // "Behind him is Piers."
  Event_ShowMessage(23, 0);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  FieldEvent_Wait(30);
  // "Pleased to meet you."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_SetAnimation(15, 3);
  Actor_SetAnimation(20, 3);
  FieldEvent_Wait(30);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  FieldEvent_Wait(30);
  // "I'm Jenna, and this is Sheba."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_SetAnimation(15, 3);
  Actor_SetAnimation(20, 3);
  FieldEvent_Wait(30);
  Actor_FaceActor(ACTOR_JENNA, 23, 0);
  Actor_SetAnimation(23, 3);
  FieldEvent_Wait(30);
  // "We're on a quest. Our travels across the Great Eastern Sea brought..."
  Event_ShowMessage(23, 0);
  Actor_SetAnimation(20, 3);
  FieldEvent_Wait(30);
  // "You were seen coming down from Mt. Mikage. What brought you to such..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 20, 0);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  FieldEvent_Wait(30);
  // "To complete our quest, we need many types of Psynergy."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Psynergy?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 15, 0);
  Actor_FaceEachOther(15, 20, 0);
  FieldEvent_Wait(60);
  Actor_FaceActor(15, *(s32 *)owner, 0);
  Actor_FaceActor(20, *(s32 *)owner, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimation(20, 4);
  FieldEvent_Wait(30);
  // "What is Psynergy?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 20, 0);
  Actor_FaceActor(*(s32 *)owner, 23, 0);
  Actor_FaceActor(ACTOR_JENNA, 23, 0);
  Actor_FaceActor(ACTOR_SHEBA, 23, 0);
  Actor_FaceActor(ACTOR_PIERS, 23, 0);
  FieldEvent_Wait(30);
  Actor_FaceActor(23, *(s32 *)owner, 0);
  FieldEvent_Wait(30);
  Actor_FaceActor(23, 15, 0);
  FieldEvent_Wait(30);
  // "Psynergy is many things... The power to move things with one's mind..."
  Event_ShowMessage(23, 0);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(20, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldEvent_Wait(30);
  // "All with one's mind?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 15, 0);
  Actor_FaceActor(*(s32 *)owner, 15, 0);
  Actor_FaceActor(ACTOR_JENNA, 15, 0);
  Actor_FaceActor(ACTOR_SHEBA, 15, 0);
  Actor_FaceActor(ACTOR_PIERS, 15, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimation(*(s32 *)owner, 3);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimation(23, 3);
  FieldEvent_Wait(30);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Tell me, is it something like...this?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 15, 0);
  FieldEvent_Wait(60);
  Actor_FaceDirection(15, FACING_EAST, 0);
  FieldEvent_Wait(30);
  IzumoInteriors_ShowMove();
  FieldEvent_Wait(30);
  Actor_FaceDirection(15, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  // "Is that the power you're talking about?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 15, 0);
  Actor_ShowEmote(*(s32 *)owner, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(23, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldEvent_Wait(30);
  FieldActor_StartRepeatedMotion(ACTOR_JENNA, 1);
  FieldEvent_Wait(15);
  // "That power? Lady Uzume?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceActor(15, 20, 0);
  FieldEvent_Wait(30);
  Actor_FaceActor(15, *(s32 *)owner, 0);
  FieldEvent_Wait(30);
  // "It's not mine alone. Kushinada and Susa possess it as well."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 15, 0);
  Actor_ShowEmote(20, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  FieldEvent_Wait(30);
  // "This is the power we possess. I assume you have something like it..."
  Event_OpenMessage(SPEAKER_WINDOW_BELOW | 20, 0);
  answer = Event_AskYesNoWithActorAnimation(*(s32 *)owner, 0);
  if (answer == 1) {
    FieldEvent_SetMessageId((s32)MsgIThoughtSo);
    Actor_ShowEmote(20, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    // "I thought so. Only those who can wield the power can see it. And you..."
    Event_ShowMessage(SPEAKER_WINDOW_BELOW | 20, 0);
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
  Actor_SetAnimation(20, 3);
  FieldEvent_Wait(30);
  // "So, you call what we do "Psynergy," and you can do it, too?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 20, 0);
  Actor_SetAnimation(*(s32 *)owner, 3);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimation(23, 3);
  FieldEvent_Wait(30);
  Actor_SetAnimation(15, 3);
  Actor_SetAnimation(20, 3);
  FieldEvent_Wait(60);
  Actor_ShowEmote(20, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceActor(20, *(s32 *)owner, 0);
  FieldEvent_Wait(30);
  // "If you already have Psynergy, why do you need more of this power?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 20, 0);
  FieldEvent_Wait(90);
  // "Their powers are different from our own. There seem to be many forms..."
  Event_ShowMessage(21, 0);
  Actor_SetPosition(21, 144, 680);
  Actor_FaceActor(*(s32 *)owner, 21, 0);
  Actor_FaceActor(ACTOR_JENNA, 21, 0);
  Actor_FaceActor(ACTOR_SHEBA, 21, 0);
  Actor_FaceActor(ACTOR_PIERS, 21, 0);
  Actor_FaceActor(23, 21, 0);
  Actor_FaceActor(22, 21, 0);
  FieldEvent_Wait(120);
  // "Susa!"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 15, 0);
  FieldEvent_Wait(30);
  EventCamera_FollowActor(21, 1);
  FieldEvent_Wait(120);
  Actor_SetSpeed(21, 0.6);
  Actor_SetSpeed(21, 0.6);
  Actor_GetById(21)->priorityFlags &= ~1;
  Actor_WalkTo(21, 144, 648);
  if (mode == 0) {
    Actor_WalkToAndWait(21, 88, 648);
    Actor_FaceActorById(*(s32 *)owner, 21, 0);
    Actor_FaceActorById(ACTOR_JENNA, 21, 0);
    Actor_FaceActorById(ACTOR_SHEBA, 21, 0);
    Actor_FaceActorById(ACTOR_PIERS, 21, 0);
    Actor_FaceActorById(23, 21, 0);
    Actor_FaceActorById(15, 21, 0);
    Actor_FaceActorById(20, 21, 0);
    Actor_FaceActorById(22, 21, 0);
    Actor_WalkToAndWait(21, 56, 616);
    Actor_FaceActorById(*(s32 *)owner, 21, 0);
    Actor_FaceActorById(ACTOR_JENNA, 21, 0);
    Actor_FaceActorById(ACTOR_SHEBA, 21, 0);
    Actor_FaceActorById(ACTOR_PIERS, 21, 0);
    Actor_FaceActorById(23, 21, 0);
    Actor_FaceActorById(15, 21, 0);
    Actor_FaceActorById(20, 21, 0);
    Actor_FaceActorById(22, 21, 0);
    Actor_WalkToAndWait(21, 56, 584);
    Actor_GetById(22)->steeringFlags &= ~1;
    Actor_WalkToAndWait(22, 88, 568);
    Event_Wait(1);
    Actor_GetById(22)->steeringFlags |= 1;
    Actor_FaceDirectionById(22, FACING_EAST, 0);
    Actor_SetSpritePriority(21, 1);
    Actor_WalkToAndWait(21, 104, 568);
    Actor_FaceActorById(*(s32 *)owner, 21, 0);
    Actor_FaceActorById(ACTOR_JENNA, 21, 0);
    Actor_FaceActorById(ACTOR_SHEBA, 21, 0);
    Actor_FaceActorById(ACTOR_PIERS, 21, 0);
    Actor_FaceActorById(23, 21, 0);
    Actor_FaceActorById(15, 21, 0);
    Actor_FaceActorById(20, 21, 0);
    Actor_FaceActorById(22, 21, 0);
    Actor_SetSpritePriority(23, 1);
    Actor_SetSpritePriority(21, 2);
    Actor_WalkToAndWaitById(21, 120, 568);
    Actor_SetSpritePriority(21, 3);
    Actor_SetSpritePriority(23, 3);
    Actor_WalkToAndWait(21, 120, 536);
    Actor_FaceActorById(*(s32 *)owner, 21, 0);
    Actor_FaceActorById(ACTOR_JENNA, 21, 0);
    Actor_FaceActorById(ACTOR_SHEBA, 21, 0);
    Actor_FaceActorById(ACTOR_PIERS, 21, 0);
    Actor_FaceActorById(23, 21, 0);
    Actor_FaceActorById(15, 21, 0);
    Actor_FaceActorById(20, 21, 0);
    Actor_FaceActorById(22, 21, 0);
    Actor_SetAnimationById(22, 3);
    Actor_WalkToAndWait(22, 56, 584);
    Actor_WalkToAndWait(22, 88, 648);
    Actor_SetPosition(22, 0, 0);
  } else {
    Actor_WalkToAndWait(21, 200, 648);
    Actor_FaceActorById(*(s32 *)owner, 21, 0);
    Actor_FaceActorById(ACTOR_JENNA, 21, 0);
    Actor_FaceActorById(ACTOR_SHEBA, 21, 0);
    Actor_FaceActorById(ACTOR_PIERS, 21, 0);
    Actor_FaceActorById(23, 21, 0);
    Actor_FaceActorById(15, 21, 0);
    Actor_FaceActorById(20, 21, 0);
    Actor_FaceActorById(22, 21, 0);
    Actor_WalkToAndWait(21, 232, 616);
    Actor_FaceActorById(*(s32 *)owner, 21, 0);
    Actor_FaceActorById(ACTOR_JENNA, 21, 0);
    Actor_FaceActorById(ACTOR_SHEBA, 21, 0);
    Actor_FaceActorById(ACTOR_PIERS, 21, 0);
    Actor_FaceActorById(23, 21, 0);
    Actor_FaceActorById(15, 21, 0);
    Actor_FaceActorById(20, 21, 0);
    Actor_FaceActorById(22, 21, 0);
    Actor_WalkToAndWait(21, 232, 584);
    Actor_GetById(22)->steeringFlags &= ~1;
    Actor_WalkToAndWait(22, 200, 568);
    Event_Wait(1);
    Actor_GetById(22)->steeringFlags |= 1;
    Actor_FaceDirection(22, FACING_WEST, 0);
    Actor_SetSpritePriority(21, 1);
    Actor_WalkToAndWait(21, 184, 568);
    Actor_FaceActorById(*(s32 *)owner, 21, 0);
    Actor_FaceActorById(ACTOR_JENNA, 21, 0);
    Actor_FaceActorById(ACTOR_SHEBA, 21, 0);
    Actor_FaceActorById(ACTOR_PIERS, 21, 0);
    Actor_FaceActorById(23, 21, 0);
    Actor_FaceActorById(15, 21, 0);
    Actor_FaceActorById(20, 21, 0);
    Actor_FaceActorById(22, 21, 0);
    Actor_SetSpritePriority(21, 2);
    Actor_SetSpritePriority(ACTOR_SHEBA, 1);
    Actor_WalkToAndWaitById(21, 168, 568);
    Actor_SetSpritePriority(21, 3);
    Actor_SetSpritePriority(ACTOR_SHEBA, 3);
    Actor_WalkToAndWait(21, 168, 536);
    Actor_FaceActorById(*(s32 *)owner, 21, 0);
    Actor_FaceActorById(ACTOR_JENNA, 21, 0);
    Actor_FaceActorById(ACTOR_SHEBA, 21, 0);
    Actor_FaceActorById(ACTOR_PIERS, 21, 0);
    Actor_FaceActorById(23, 21, 0);
    Actor_FaceActorById(15, 21, 0);
    Actor_FaceActorById(20, 21, 0);
    Actor_FaceActorById(22, 21, 0);
    Actor_SetAnimationById(22, 3);
    Actor_WalkToAndWait(22, 232, 584);
    Actor_WalkToAndWait(22, 200, 648);
    Actor_SetPosition(22, 0, 0);
  }
  person = Actor_GetById(21);
  {
    u8 *priority = &person->priorityFlags;
    s32 flags = *priority | 1;

    owner = (s32)&gMainRoutineWork;
    *priority = flags;
  }
  owner = owner + 532;
  Actor_FaceDirection(*(s32 *)owner, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirectionById(23, FACING_NORTH, 0);
  Event_Wait(15);
  // "Susa..."
  Event_ShowMessageAndWait(20, 0);
  Event_Wait(15);
  Actor_ShowEmote(20, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "So, you lived after all, Susa..."
  Event_ShowMessageAndWait(20, 0);
  Actor_FaceActorById(21, 20, 0);
  Event_Wait(30);
  // "Yes, I've survived, and I've made it home to you both."
  Event_ShowMessageAndWait(21, 0);
  Actor_FaceActorById(21, *(s32 *)owner, 0);
  Event_Wait(60);
  Actor_FaceActorById(21, 20, 0);
  Event_Wait(30);
  // "Thanks to them, actually..."
  Event_ShowMessageAndWait(21, 0);
  Actor_TurnToFaceEachOther(15, 20, 0);
  Event_Wait(30);
  Actor_FaceActorById(15, *(s32 *)owner, 0);
  Actor_FaceActorById(20, *(s32 *)owner, 0);
  // "Felix, our gratitude to you and your friends grows deeper each..."
  Event_ShowMessageAndWait(15, 0);
  Actor_SetAnimationById(15, 3);
  Actor_SetAnimationById(20, 3);
  Event_Wait(30);
  Actor_SetAnimationById(*(s32 *)owner, 3);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimationById(23, 3);
  Event_Wait(30);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Susa, you spoke of different types of power. Tell me, what did you mean?"
  Event_ShowMessageAndWait(15, 0);
  Actor_FaceActorById(21, *(s32 *)owner, 0);
  // "I can only describe what I saw, my sister. Ask Felix here about it."
  Event_ShowMessageAndWait(21, 0);
  Actor_FaceActorById(15, *(s32 *)owner, 0);
  Actor_FaceActorById(20, *(s32 *)owner, 0);
  Event_Wait(30);
  Actor_ShowEmoteById(*(s32 *)owner, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_SetAnimationById(23, 3);
  Event_Wait(30);
  // "Ah, yes. Psynergy... I have dedicated most of my life to its study..."
  Event_ShowMessageAndWait(23, 0);
  Actor_SetAnimationById(15, 3);
  Actor_SetAnimationById(20, 3);
  Event_Wait(30);
  Actor_SetAnimationById(23, 3);
  // "The power of Psynergy comes from the four basic forces of earth..."
  Event_ShowMessageAndWait(23, 0);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(20, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(21, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(22, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(30);
  Actor_ShowEmote(23, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "You don't know Alchemy? The four elements? Why, they're the very..."
  Event_ShowMessageAndWait(23, 0);
  Actor_ShowEmote(21, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "All existing matter comes from Alchemy and this Psynergy?"
  Event_ShowMessageAndWait(21, 0);
  Actor_FaceActorById(23, 21, 0);
  Event_Wait(30);
  // "Yes, that's right. Everything you see around you is some combination..."
  Event_ShowMessageAndWait(23, 0);
  Actor_ShowEmoteById(20, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  // "Does this mean that there are as many types of Psynergy as there are..."
  Event_ShowMessageAndWait(20, 0);
  Actor_ShowEmote(23, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "That's right, Kushinada. You're a smart lass."
  Event_ShowMessageAndWait(23, 0);
  Actor_SetAnimationById(23, 3);
  Event_Wait(30);
  // "Mt. Mikage was a place of great earth powers. I'd imagine your..."
  Event_ShowMessageAndWait(23, 0);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "That's right, but how did you know this?"
  Event_ShowMessageAndWait(15, 0);
  Actor_FaceActorById(ACTOR_PIERS, 23, 0);
  Event_Wait(30);
  // "Kraden... Mt. Mikage is Gaia Rock, isn't it?"
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_FaceActorById(23, ACTOR_PIERS, 0);
  Actor_SetAnimationById(23, 3);
  Event_Wait(30);
  Actor_ShowEmoteById(15, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceActorById(23, 15, 0);
  Actor_FaceActorById(ACTOR_PIERS, 15, 0);
  Event_Wait(30);
  // "...Gaia Rock? Mt. Mikage is the sacred mount of our people!"
  Event_ShowMessageAndWait(15, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Yes, but there are many mountains like your Mt. Mikage all across..."
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_ShowEmoteById(21, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "You have seen other mountains like our Mt. Mikage?"
  Event_ShowMessageAndWait(21, 0);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(30);
  // "Yes! In fact, it seems that each of these rocks contains a different..."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_SetAnimationById(20, 3);
  Event_Wait(30);
  // "And now, you are questing to gather the power of these elements, yes?"
  Event_ShowMessageAndWait(20, 0);
  Actor_SetAnimationById(*(s32 *)owner, 3);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimationById(23, 3);
  Event_Wait(90);
  Actor_SetAnimationById(15, 3);
  // "And you only defeated the Great Serpent of Mikage to accomplish this..."
  Event_OpenMessage(15, 0);
  answer = Event_AskYesNoWithActorAnimation(*(s32 *)owner, 0);
  if (answer == 0) {
    Event_SetMessageId((s32)MsgRegardlessOfYour);
    Actor_ShowEmoteById(15, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    // "Regardless of your motives, you have saved us all."
    Event_ShowMessageAndWait(15, 0);
  } else {
    Event_SetMessageId((s32)MsgYouTookPity);
    // "You took pity on Kushinada? You are very kind to help Susa, Felix."
    Event_ShowMessageAndWait(15, 0);
  }
  message = (s32)MsgYouReNotImplying;
  Event_SetMessageId(message);
  Actor_StartRepeatedMotion(23, 1);
  Event_Wait(30);
  // "You're...not implying that we defeated the serpent, are you?"
  Event_ShowMessageAndWait(23, 0);
  Actor_FaceActorById(15, 23, 0);
  Actor_ShowEmoteById(15, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_SetAnimationById(23, 3);
  Event_Wait(30);
  // "It's true that we did fight the creature..."
  Event_ShowMessageAndWait(23, 0);
  Actor_FaceActorById(23, 21, 0);
  Event_Wait(30);
  // "But without Susa's power, the battle might not have ended so happily."
  Event_ShowMessageAndWait(23, 0);
  Actor_TurnToFaceEachOther(15, 20, 0);
  Event_Wait(30);
  // "What... What are you°"
  Event_ShowMessageAndWait(21, 0);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  Event_Wait(30);
  // "Susa had given the beast a lot of Dragonsbane in order to weaken it."
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_FaceDirection(15, FACING_SOUTH, 0);
  Actor_FaceDirectionById(20, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(30);
  // "We could never have beaten the beast if it had not been in..."
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "We were victorious only because Susa risked everything to weaken the..."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_ShowEmoteById(15, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Event_Wait(15);
  Actor_FaceActorById(15, 21, 0);
  // "Susa, perhaps, after all, you are..."
  Event_ShowMessageAndWait(15, 0);
  Actor_ShowEmoteById(21, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "No, I didn't..."
  Event_ShowMessageAndWait(21, 0);
  Actor_ShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  owner = (s32)&gMainRoutineWork;
  owner = owner + 532;
  Actor_FaceActorById(ACTOR_SHEBA, *(s32 *)owner, 0);
  // "Don't forget, Susa dealt the finishing blow, didn't he?"
  Event_OpenMessage(6, 0);
  Actor_FaceActorById(ACTOR_JENNA, *(s32 *)owner, 0);
  Actor_FaceActorById(ACTOR_SHEBA, *(s32 *)owner, 0);
  Actor_FaceActorById(ACTOR_PIERS, *(s32 *)owner, 0);
  Actor_FaceActorById(23, *(s32 *)owner, 0);
  Actor_FaceActorById(15, *(s32 *)owner, 0);
  Actor_FaceActorById(20, *(s32 *)owner, 0);
  Actor_FaceActorById(21, *(s32 *)owner, 0);
  Event_Wait(30);
  answer = Event_AskYesNoWithActorAnimation(*(s32 *)owner, 0);
  if (answer == 0) {
    Event_SetMessageId(message + 10);
    Actor_SetAnimationById(21, 4);
    Event_Wait(30);
    // "Finishing blow? All I did was stab it with the Cloud Brand."
    Event_ShowMessageAndWait(21, 0);
  } else {
    Event_SetMessageId(message + 11);
    Actor_SetAnimationById(21, 3);
    Event_Wait(30);
    // "That's right. Piercing it with the Cloud Brand was hardly a..."
    Event_ShowMessageAndWait(21, 0);
  }
  message = (s32)MsgIDisagree2;
  Event_SetMessageId(message);
  Actor_FaceActorById(23, 21, 0);
  Actor_ShowEmoteById(23, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "I disagree..."
  Event_ShowMessageAndWait(23, 0);
  Actor_SetAnimationById(23, 3);
  Event_Wait(30);
  // "You weakened the serpent. You dealt the final blow. I'd say you were..."
  Event_ShowMessageAndWait(23, 0);
  owner = (s32)&gMainRoutineWork;
  owner = owner + 532;
  Actor_SetAnimationById(*(s32 *)owner, 3);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceActorById(23, 15, 0);
  Event_Wait(30);
  // "So, Lady Uzume, wouldn't you say that Susa deserves the reward for..."
  Event_ShowMessageAndWait(23, 0);
  Actor_FaceActorById(15, *(s32 *)owner, 0);
  Actor_FaceActorById(23, *(s32 *)owner, 0);
  Event_Wait(30);
  // "Even Felix agrees, don't you, Felix?"
  Event_OpenMessage(23, 0);
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
  Actor_FaceActorById(23, 15, 0);
  Event_Wait(30);
  // "As you can see."
  Event_ShowMessageAndWait(23, 0);
  Actor_TurnToFaceEachOther(15, 20, 0);
  Event_Wait(30);
  Actor_SetAnimationById(15, 3);
  Actor_SetAnimationById(20, 3);
  Event_Wait(30);
  party = (s32)&gMainRoutineWork;
  owner = party + 532;
  Actor_FaceActorById(15, *(s32 *)owner, 0);
  Actor_FaceActorById(20, *(s32 *)owner, 0);
  Actor_StartRepeatedMotion(21, 1);
  Event_Wait(15);
  Actor_ShowEmote(21, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  Actor_FaceActorById(23, *(s32 *)owner, 0);
  Event_Wait(30);
  // "Now, it's about time for us to leave Izumo Village."
  Event_ShowMessageAndWait(23, 0);
  Actor_FaceActorById(*(s32 *)owner, 23, 0);
  Event_Wait(15);
  Actor_SetAnimationById(*(s32 *)owner, 3);
  Event_Wait(30);
  Actor_SetAnimationById(23, 3);
  Event_Wait(30);
  // "Yes, indeed! We're on our way to Lemuria."
  Event_ShowMessageAndWait(23, 0);
  Actor_FaceActorById(ACTOR_JENNA, 23, 0);
  Actor_FaceActorById(ACTOR_SHEBA, 23, 0);
  Actor_FaceActorById(ACTOR_PIERS, 23, 0);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimationById(*(s32 *)owner, 3);
  Event_Wait(30);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(20, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmoteById(21, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_Wait(30);
  // "You're leaving so soon?"
  Event_ShowMessageAndWait(20, 0);
  Actor_FaceActorById(*(s32 *)owner, 15, 0);
  Actor_FaceActorById(ACTOR_JENNA, 15, 0);
  Actor_FaceActorById(ACTOR_SHEBA, 15, 0);
  Actor_FaceActorById(ACTOR_PIERS, 15, 0);
  Actor_FaceActorById(23, 15, 0);
  Event_Wait(30);
  // "We are in a great hurry."
  Event_ShowMessageAndWait(23, 0);
  Actor_ShowEmoteById(15, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "Then you must return to Izumo someday."
  Event_ShowMessageAndWait(15, 0);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(30);
  // "I'm sure we'll be back once our mission is completed."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_SetAnimationById(20, 3);
  Event_Wait(30);
  // "Excellent! I shall accept that as your oath!"
  Event_ShowMessageAndWait(20, 0);
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
  Actor_SetAnimationById(*(s32 *)owner, 3);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  Actor_TurnToFaceEachOther(ACTOR_JENNA, ACTOR_SHEBA, 0);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(15);
  Actor_TurnToFaceEachOther(*(s32 *)owner, 5, 0);
  Actor_TurnToFaceEachOther(ACTOR_PIERS, ACTOR_SHEBA, 0);
  Actor_SetAnimationById(*(s32 *)owner, 3);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceActorById(*(s32 *)owner, 15, 0);
  Actor_FaceActorById(ACTOR_JENNA, 15, 0);
  Actor_FaceActorById(ACTOR_SHEBA, 15, 0);
  Actor_FaceActorById(ACTOR_PIERS, 15, 0);
  Actor_FaceActorById(23, 15, 0);
  Actor_SetAnimationById(15, 3);
  Event_Wait(30);
  // "We'll be waiting."
  Event_ShowMessageAndWait(15, 0);
  Actor_SetAnimationById(*(s32 *)owner, 3);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationById(23, 3);
  Event_Wait(15);
  Actor_SetAnimationById(15, 3);
  Actor_SetAnimationById(20, 3);
  Actor_SetAnimationById(21, 3);
  Event_Wait(60);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_WALK);
  leader = Actor_GetById(*(s32 *)owner);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_JENNA, leader->x.part[0].pixel,
                             leader->z.part[0].pixel);
  Actor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_WALK);
  leader = Actor_GetById(*(s32 *)owner);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_SHEBA, leader->x.part[0].pixel,
                             leader->z.part[0].pixel);
  Actor_WaitForMoveById(ACTOR_SHEBA);
  Actor_SetPosition(ACTOR_SHEBA, 0, 0);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_WALK);
  leader = Actor_GetById(*(s32 *)owner);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_PIERS, leader->x.part[0].pixel,
                             leader->z.part[0].pixel);
  Actor_WaitForMoveById(ACTOR_PIERS);
  Actor_SetPosition(ACTOR_PIERS, 0, 0);
  Actor_SetAnimationById(23, 2);
  leader = Actor_GetById(*(s32 *)owner);
  if (leader != 0)
    Actor_SetDestinationById(23, leader->x.part[0].pixel,
                             leader->z.part[0].pixel);
  Actor_WaitForMoveById(23);
  Actor_SetPosition(23, 0, 0);
  GameFlag_Set(FLAG_IZUMO_RECOGNIZED_THE_HEROES);
  GameFlag_Set(FLAG_AREA_TEMP_50);
  Event_End();
}

/// The Move cast below has nothing to move.
s32 IzumoInteriors_FindNoMoveTarget(void) { return -1; }

/// Actor 15 casts Move, with no target.
void IzumoInteriors_ShowMove(void) {
  FieldPsynergy_LoadResources(ABILITY_MOVE, 1);
  FieldPsynergy_SetupActorPair(15, -1);
  FIELD_AT(Ram_HeapSlots->fieldEffectWork, HandlerFn, 36) =
      IzumoInteriors_FindNoMoveTarget;
  FIELD_AT(Ram_HeapSlots->fieldEffectWork, u8, 32) = 1;
  FieldPsynergy_StartCast();
  Event_Wait(10);
  FieldPsynergy_RunHandler(0);
  BattleEffect_StopMainActor();
  FieldPsynergy_End();
}

void IzumoInteriors_GiveUzumeTheDoll(void) {
  s32 t21, t31;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgAhCouldIt);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 144, 536);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Event_Wait(15);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  // "Ah... Could it be? Is this the dancing doll of Mt. Mikage?"
  Event_OpenMessage(15, 0);
  if (!Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0)) {
    Event_SetMessageId((s32)&MsgAhCouldIt + 1);
    // "You found this at the peak of Mt. Mikage?"
    Event_ShowMessageAndWait(15, 0);
  } else {
    Event_SetMessageId((s32)&MsgAhCouldIt + 2);
    // "Don't lie to me. The style, the craftsmanship... You must have found..."
    Event_ShowMessageAndWait(15, 0);
  }
  Actor_ShowEmoteById(15, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 60);
  Event_SetMessageId((s32)&MsgAgainYouHave);
  // "Again, you have done more for me than I can properly reward you for..."
  Event_ShowMessageAndWait(15, 0);
  Actor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 120);
  Actor_ShowEmoteById(15, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
  // "I think I may have something that might please you."
  Event_ShowMessageAndWait(15, 0);
  BattleEffect_StartArcEffect();
  t21 = ((s32)Actor_GetById(15));
  BattleEffect_SetObjectUpdateMode(t21, 2);
  Sound_Play(SE_ASCEND);
  Event_Wait(60);
  Actor_SetPosition(24, 144, 520);
  Actor_Jump(24, 6, 0);
  Actor_SetDestination(24, 160, 520);
  Actor_WaitForMoveById(24);
  Actor_FaceDirectionById(24, FACING_SOUTH, 0);
  Event_Wait(60);
  t31 = ((s32)Actor_GetById(15));
  BattleEffect_SetObjectUpdateMode(t31, 0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  Event_Wait(30);
  Event_Wait(30);
  Actor_FaceActorById(15, 24, 0);
  Event_Wait(30);
  // "This is my pet. It's very dear to me."
  Event_ShowMessageAndWait(15, 0);
  Actor_FaceActorById(15, gMainRoutineWork.leaderActorId, 0);
  Event_Wait(60);
  // "I am unsure how it works, but this fellow seems to magnify our power..."
  Event_ShowMessageAndWait(15, 0);
  Event_Wait(30);
  Actor_SetAnimationById(15, 4);
  Event_Wait(60);
  // "Though it saddens me to part with it, I would have you take him, as..."
  Event_ShowMessageAndWait(15, 0);
  Actor_TurnToFaceEachOther(15, 24, 0);
  // "Felix and his friends have agreed to care for you from now on."
  Event_ShowMessageAndWait(15, 0);
  Event_Wait(60);
  Actor_Jump(24, 4, 20);
  Event_Wait(60);
  Actor_FaceActorById(15, gMainRoutineWork.leaderActorId, 0);
  Actor_FaceActorById(24, gMainRoutineWork.leaderActorId, 0);
  Event_Wait(60);
  // "Please, accept this gift."
  Event_ShowMessageAndWait(15, 0);
  Actor_Jump(24, 2, 20);
  Djinn_JoinParty(24, DJINNI_COAL / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_COAL));
  Event_End();
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActorById(gMainRoutineWork.leaderActorId, 15, 0);
  Event_Wait(60);
  Actor_StartRepeatedMotion(15, 2);
  Event_Wait(30);
  // "With this dancing doll, I can explore Mt. Mikage on my own. Take..."
  Event_ShowMessageAndWait(15, 0);
  Event_Wait(60);
  GameFlag_Set(FLAG_UZUME_HAS_THE_DOLL);
  PartyInventory_Remove(ITEM_DANCING_IDOL);
  Event_End();
}

s32 Scene_Initialize(void) {
  gFieldWork->transition = 0x201;
  if (gMainRoutineWork.entrance != 7)
    Event_ClearFlag(FLAG_AREA_TEMP_50);
  if (GameFlag_Get(FLAG_SERPENT_SLAIN) &&
      !GameFlag_Get(FLAG_IZUMO_RECOGNIZED_THE_HEROES) &&
      gMainRoutineWork.entrance == 7) {
    Event_PlaySound(MUS_IZUMO);
    Actor_SetPosition(22, 144, 568);
    Actor_SetPosition(21, 0, 0);
    Actor_SetPosition(16, 0, 0);
    Actor_SetPosition(17, 0, 0);
    Actor_SetPosition(18, 0, 0);
    Actor_SetPosition(19, 0, 0);
  }
  if (GameFlag_Get(FLAG_SERPENT_SLAIN) &&
      GameFlag_Get(FLAG_IZUMO_RECOGNIZED_THE_HEROES) &&
      gMainRoutineWork.entrance == 7 && GameFlag_Get(FLAG_AREA_TEMP_50)) {
    Event_PlaySound(MUS_IZUMO);
    Actor_SetPosition(16, 0, 0);
    Actor_SetPosition(17, 0, 0);
    Actor_SetPosition(18, 0, 0);
    Actor_SetPosition(19, 0, 0);
  }
  if (gMainRoutineWork.entrance == 5 && !GameFlag_Get(FLAG_SERPENT_SLAIN)) {
    Actor_SetAnimation(17, 5);
    Actor_SetAttachedEffect(17,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  }
  if (gMainRoutineWork.entrance == 7 && !GameFlag_Get(FLAG_SERPENT_SLAIN)) {
    struct Actor *object;

    Actor_SetAnimation(9, 5);
    Actor_SetAnimation(10, 5);
    Actor_SetAnimation(11, 5);
    Actor_SetAnimation(12, 5);
    object = Actor_GetById(11);
    object->scaleX = -Actor_GetById(11)->scaleX;
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

extern s32 gIzumoInteriors_BounceScript[];

s32 gIzumoInteriors_FirstPacerScript[] = {
    22,
    30,
    129,
    22,
    16,
    0x3333,
    22,
    21,
    0x6666,
    22,
    15,
    0x9999,
    22,
    31,
    2,
    22,
    3,
    0x4000,
    (s32)0x80010000,
    46,
    (s32)IzumoInteriors_IsFirstPacerClear,
    4,
    0x700000,
    0,
    0x2700000,
    1,
    4,
    0x700000,
    0,
    0x2580000,
    1,
    4,
    0x880000,
    0,
    0x2580000,
    1,
    4,
    0x880000,
    0,
    0x2700000,
    1,
    22,
    3,
    0x8000,
    0,
    5,
    22,
    3,
    0xc000,
    0,
    5,
    22,
    3,
    0,
    0,
    5,
    22,
    3,
    0x4000,
    0,
    5,
    23,
    6,
    0x40000,
    0,
    30,
    4,
    0x700000,
    0,
    0x2700000,
    1,
    4,
    0x880000,
    0,
    0x2580000,
    1,
    4,
    0x700000,
    0,
    0x2580000,
    1,
    4,
    0x880000,
    0,
    0x2700000,
    1,
    22,
    3,
    0x8000,
    0,
    5,
    22,
    3,
    0xc000,
    0,
    5,
    22,
    3,
    0,
    0,
    5,
    22,
    3,
    0x4000,
    0,
    5,
    23,
    6,
    0x40000,
    0,
    30,
    13,
    0xffff,
    (s32)0xc0010000,
    -1,
};

s32 gIzumoInteriors_SecondPacerScript[] = {
    22,
    30,
    129,
    22,
    16,
    0x3333,
    22,
    21,
    0x6666,
    22,
    15,
    0x9999,
    22,
    31,
    2,
    22,
    3,
    0x4000,
    (s32)0x80010000,
    46,
    (s32)IzumoInteriors_IsSecondPacerClear,
    4,
    0xb00000,
    0,
    0x2700000,
    1,
    4,
    0xb00000,
    0,
    0x2580000,
    1,
    4,
    0x980000,
    0,
    0x2580000,
    1,
    4,
    0x980000,
    0,
    0x2700000,
    1,
    22,
    3,
    0,
    0,
    5,
    22,
    3,
    0xc000,
    0,
    5,
    22,
    3,
    0x8000,
    0,
    5,
    22,
    3,
    0x4000,
    0,
    5,
    23,
    6,
    0x40000,
    0,
    30,
    4,
    0xb00000,
    0,
    0x2700000,
    1,
    4,
    0x980000,
    0,
    0x2580000,
    1,
    4,
    0xb00000,
    0,
    0x2580000,
    1,
    4,
    0x980000,
    0,
    0x2700000,
    1,
    22,
    3,
    0,
    0,
    5,
    22,
    3,
    0xc000,
    0,
    5,
    22,
    3,
    0x8000,
    0,
    5,
    22,
    3,
    0x4000,
    0,
    5,
    23,
    6,
    0x40000,
    0,
    30,
    13,
    0xffff,
    (s32)0xc0010000,
    -1,
};

// clang-format off
s32 gIzumoInteriors_RockingScript[] = {
    22, 30, 129, 39, 0, (s32)0x80010000,
    23, 9, -0x4000, 23, 10, 0x2000,
    0, 2, 23, 9, 0x2000, 23,
    10, -4096, 0, 2, 23, 9,
    0x1000, 23, 10, -2048, 0, 2,
    23, 9, 0x1000, 23, 10, -2048,
    0, 40, 23, 9, -0x4000, 23,
    10, 0x2000, 0, 2, 23, 9,
    0x2000, 23, 10, -4096, 0, 2,
    23, 9, 0x1000, 23, 10, -2048,
    0, 2, 23, 9, 0x1000, 23,
    10, -2048, 0, 5, 23, 9,
    -0x4000, 23, 10, 0x2000, 0, 2,
    23, 9, 0x2000, 23, 10, -4096,
    0, 2, 23, 9, 0x1000, 23,
    10, -2048, 0, 2, 23, 9,
    0x1000, 23, 10, -2048, 0, 30,
    13, 0xffff, (s32)0xc0010000, 17, 22, 30,
    131, 22, 16, 0x1999, 22, 15,
    0x4ccc, 39, 2, (s32)0x80010000, 4, 0x1080000,
    0, 0x780000, 1, 0, 60, 4,
    0x1080000, 0, 0x600000, 1, 0, 60,
    13, 0xffff, (s32)0xc0010000, -1,
};

s32 gIzumoInteriors_BounceScript[] = {
    SC_SET(SW_COLLISION, 131),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(60),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(10),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(90),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
    SC_SET(SW_COLLISION, 129),
    SC_ACCELERATION(0.2),
    SC_SET(SW_GRAVITY, 0x1999),
    SC_SPEED(0.4),
    SC_ANIMATION(0),
    SC_LABEL(1),
    SC_SET(SW_Y, px(4)),
    SC_WAIT(30),
    SC_END,
};
// clang-format on

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(IzumoInteriors),
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
struct ActorPlacement gIzumoInteriors_ActorPlacements[] = {
    ACTOR_PLACEMENT(120, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 120, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(120, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 152, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(120, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 88, 0, 88, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(124, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 96, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(124, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 80, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(124, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 384, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(124, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 84, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(125, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 136, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(125, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 120, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(32, CONDITION_ALWAYS, (s32)gIzumoInteriors_BounceScript, 352, 0, 328, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(121, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 362, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(118, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 108, 0, 320, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(119, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(123, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 632, 0, 88, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(201, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 560, 0, 328, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(127, CONDITION_ALWAYS, (s32)gIzumoInteriors_RockingScript, 504, 0, 360, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(127, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 392, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gIzumoInteriors_SusasHouseActorPlacements[] = {
    ACTOR_PLACEMENT(30, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 144, 0, 520, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(120, CONDITION_ALWAYS, (s32)gIzumoInteriors_FirstPacerScript, 136, 0, 624, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(120, CONDITION_ALWAYS, (s32)gIzumoInteriors_SecondPacerScript, 152, 0, 624, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(124, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 90, 0, 622, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(124, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 198, 0, 622, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gIzumoInteriors_ActorPlacementsOnceTheSerpentIsSlain[] = {
    ACTOR_PLACEMENT(121, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 362, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(118, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 108, 0, 320, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(119, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(123, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 632, 0, 88, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(201, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 560, 0, 328, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(127, CONDITION_ALWAYS, (s32)gIzumoInteriors_RockingScript, 600, 0, 360, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(127, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 376, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(30, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 144, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(120, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 624, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(120, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 624, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(124, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 616, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(124, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 616, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(32, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 520, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(29, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 552, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(117, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(7, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(5, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(6, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_COAL), MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gIzumoInteriors_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(IzumoInteriors_KushinadaPleads)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgOkuniKushinadaAnd)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgIfLadyUzume)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgIMSoRelieved)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgWhatIWant)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgAWhileBack)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgThatDragonsbaneSeems)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgICanTFind)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgIAmOld)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTheSerpentCan)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(IzumoInteriors_TalkToMerchant)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgOneDayKushinada)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, EVENT(IzumoInteriors_TalkToInnkeeper)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgIfIWere)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, EVENT(IzumoInteriors_TalkToHealer)},
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

struct Trigger gIzumoInteriors_SusasHouseTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(IzumoInteriors_AskAfterSusa)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgThereSNoPoint)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgTheSerpentAttacked)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgTheSerpentAte)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgTheSerpentLives)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     EVENT(IzumoInteriors_ReadPrayerForSusa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgOkuniSSuch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgTheSerpentWould)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgBeforeTheSerpent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgLegendHasIt)},
    {TRIGGER_TABLE_END},
};

struct Trigger gIzumoInteriors_TriggersOnceTheSerpentIsSlain[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_STEP, 22, FLAG_IZUMO_RECOGNIZED_THE_HEROES,
     EVENT(IzumoInteriors_HeroesComeFromTheEast)},
    {TRIGGER_STEP, 23, FLAG_IZUMO_RECOGNIZED_THE_HEROES,
     EVENT(IzumoInteriors_HeroesComeFromTheWest)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(IzumoInteriors_TalkToMerchant)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgMyGoodsAre)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(IzumoInteriors_TalkToInnkeeper)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgAsLadyUzume)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(IzumoInteriors_TalkToHealer)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgNowThatTheSerpent)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgIfYouReA)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, EVENT(IzumoInteriors_TalkToUzume)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS,
     EVENT(IzumoInteriors_AskAboutMtMikage)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgSupposedlyTheDoll)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgLadyUzumeHas)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgLadyUzumeSNew)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgSomedaySusaAnd)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgIMSureYou)},
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
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15 | TRIGGER_SUBJECT_OWN_EFFECT,
     CONDITION_ALWAYS, EVENT(IzumoInteriors_ReadUzumesMind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgWhyDoesLadyUzume)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgIWonderWhatSort)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgIHeardThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgIDonTKnowWhatIt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgIfSDescendants)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgIMNo)},
    {USE_ITEM_ON_ACTOR(ITEM_DANCING_IDOL), 15, FLAG_UZUME_HAS_THE_DOLL,
     EVENT(IzumoInteriors_GiveUzumeTheDoll)},
    {TRIGGER_TABLE_END},
};

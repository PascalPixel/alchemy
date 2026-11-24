/// @file
/// Inside Alhafra's houses, inn, shop and Sanctum, whose people are taking
/// in neighbors who lost their homes to the tidal wave.
///
/// The first time Felix walks into the inn's upstairs room he finds Alex
/// there, also waiting for a boat; in the crowded house an old couple of
/// houseguests send the lady of the house for tea. After Briggs is caught
/// Alex asks about it, and once the boat's mast is cleared and Alex has gone,
/// a game carried over from Golden Sun in which Isaac beat Deadbeard has two
/// sailors in his room telling how Isaac picked Crossbone Isle clean. The
/// people's tables change at those two points (FLAG_BRIGGS_CAUGHT and
/// FLAG_ALHAFRA_MAST_CLEARED).

#include "system/main_routine_state.h"
#include "field/field_sequence.h"
#include "field/field_sequence_event.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "macros.h"
#include "flags.h"
#include "field/scene_tables.h"
#include "scene/scene.h"
#include "system/ram_buffer.h"
#include "scene_ids.h"
#include "field/field_sprite.h"
#include "system/frame.h"
#include "field/event_inline.h"
#include "field/actor.h"
#include "field/call.h"
#include "field/effect_sprite.h"
#include "version.h"
#include "field/actor_script.h"
#include "fixed_math.h"

/// Talks each person gives once, the first time spoken to.
#define FLAG_GUESTS_ASKED_FOR_TEA 0x8b1
#define FLAG_HOST_TOLD_OF_REPAIRS 0x8b8

/// Set, once the mast is cleared, when Felix comes into the rooms east of
/// x 512 on the map; only then are the sailors of the Crossbone Isle tale
/// placed.
#define FLAG_ENTERED_IN_THE_EAST FLAG_SCENE_TEMP_00

struct EffectSprite gAlhafraRooms_SecondGuestSprite BSS_DATA ALIGNED(16);
struct EffectSprite gAlhafraRooms_FirstGuestSprite BSS_DATA ALIGNED(16);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "text/msg_ids.h"

void Shop_Run(s32 shop, s32 keeper);
void Sanctum_Run(s32 keeper);
void Inn_CheckIn(s32 shop, s32 keeper);

/// Alhafra's houses' people, numbering the rows of all three placement tables:
/// the same row is the same person in each, put out of sight (-56, -56) while
/// away. The tables part only in their last rows.
enum AlhafraRoomsActorId {
  ACTOR_ITEM_SHOPKEEPER = ACTOR_FIRST_PLACED,
  /// The Sanctum's priest, who tells of Garoh's curse.
  ACTOR_PRIEST,
  /// "I don't care if it WAS broken! The mayor shouldn't have up and sold..."
  ACTOR_WOMAN_ANGRY_AT_THE_SALE,
  /// "Those guys who bought the boat looked kinda shabby..."
  ACTOR_WOMAN_ON_THE_SHABBY_BUYERS,
  /// The couple taking in neighbors whose house the tidal wave destroyed, and
  /// their two guests, the old man who keeps asking for tea and his wife.
  ACTOR_HOST,
  ACTOR_HOSTESS,
  ACTOR_DEMANDING_GUEST,
  ACTOR_DEMANDING_GUESTS_WIFE,
  ACTOR_INNKEEPER,
  /// "You see how full our inn is? You'd think we'd be making money..."
  ACTOR_INN_WORKER,
  /// The townsfolk lodged at the inn.
  /// "The town ought to give money to all the people who suffered..."
  ACTOR_MAN_ASKING_FOR_MONEY,
  /// "I can't believe the innkeeper is renting out more rooms..."
  ACTOR_WOMAN_ON_THE_CRAMMED_ROOMS,
  /// "The innkeeper told me to use only the cheapest ingredients..."
  ACTOR_INN_COOK,
  /// "The innkeeper serves his paying guests better meals than we Alhafrans"
  ACTOR_MAN_ON_THE_MEALS,
  /// "We're all guests in the same inn, so why is my meal so awful?"
  ACTOR_WOMAN_ON_THE_MEALS,
  /// "Ohhh... I'm hungry... My tummy hurts..."
  ACTOR_HUNGRY_CHILD,
  /// "I don't think I've had a bite of meat or fish..."
  ACTOR_CHILD_MISSING_MEAT,
  /// The family crowded into one room.
  /// "I'll never forgive the mayor for failing to come to our aid..."
  ACTOR_MAN_ANGRY_AT_THE_MAYOR,
  /// "Our guests are very upset with the mayor."
  ACTOR_WOMAN_WORRIED_ABOUT_HER_GUESTS,
  /// "My pop's been lookin' around, an' he says the mayor keeps all his
  /// treasure..."
  ACTOR_BOY_ON_THE_MAYORS_TREASURE,
  /// "Why do we all have to be cooped up in such a small room?"
  ACTOR_CHILD_COOPED_UP,
  /// "We all have to bear the discomfort of this place..."
  ACTOR_MAN_BEARING_THE_DISCOMFORT,
  /// Who eyes the warrior next door: "...so suave and handsome."
  ACTOR_WOMAN_EYEING_THE_WARRIOR,
  /// Alex, in the room next door, until the mast is cleared.
  ACTOR_ALEX
};

/// The last rows of gAlhafraRooms_ActorPlacementsOnceTheMastIsCleared:
/// two sailors in Alex's room, one telling the other of Crossbone Isle.
enum AlhafraRoomsSailorsActorId {
  ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE = ACTOR_ALEX,
  /// "If he's looking for a pirate crew, he can count me in!"
  ACTOR_SAILOR_EAGER_TO_JOIN
};

extern struct ActorPlacement gActorPlacements[];
extern struct ActorPlacement gAlhafraRooms_ActorPlacementsOnceBriggsIsCaught[];
extern struct ActorPlacement
    gAlhafraRooms_ActorPlacementsOnceTheMastIsCleared[];
extern struct Trigger gAlhafraRooms_Triggers[];
extern struct Trigger gAlhafraRooms_TriggersOnceBriggsIsCaught[];
extern struct Trigger gAlhafraRooms_TriggersOnceTheMastIsCleared[];

/// The houses' people: before Briggs is caught, after, and once the mast is
/// cleared.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED))
    return gAlhafraRooms_ActorPlacementsOnceTheMastIsCleared;
  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT))
    return gAlhafraRooms_ActorPlacementsOnceBriggsIsCaught;
  return gActorPlacements;
}

/// The first time Felix walks into the inn's upstairs room (cells (28, 19)
/// to (28, 21)), he walks up to Alex, who says the town's only boat has been
/// sold and that he too is resting here while he looks for one.
void AlhafraRooms_MeetAlex(void) {
  Event_SetFlag(FLAG_ALHAFRA_MET_ALEX);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgAlhafraInteriorsSoYouveFinallyCome);
  FieldCamera_MoveToActorById(ACTOR_ALEX, 1);
  Actor_WalkToAndWait(ACTOR_FELIX, 452, 335);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  // "So, you've finally come."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_ALEX, 0, 10);
  FieldActor_StartRepeatedMotion(ACTOR_FELIX, 3);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(50);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(10);
  // "I had suspected we might run into each other again, since we both..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_ALEX, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  // "Unfortunately, the only boat this town possesses has already been sold."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_ALEX, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  FieldEvent_Wait(10);
  // "I will be resting here in Alhafra a bit longer. I have traveled long..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_ALEX, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Perhaps we can get along at least until we both depart."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_ALEX, 0, 10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  EventCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  Event_End();
}

/// Alhafra's shops, as Shop_Run numbers them.
enum { SHOP_ALHAFRA_ITEMS = 9 };

/// Alhafra's inn, as Inn_CheckIn numbers the inns.
enum { ALHAFRA_INN = 2 };

/// Felix talks to the item merchant: across his counter, the shop opens;
/// from anywhere else he asks Felix not to mention the tidal wave.
void AlhafraRooms_TalkToItemMerchant(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineState.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Shop_Run(SHOP_ALHAFRA_ITEMS, actor);
  } else {
    Event_SetMessageId((s32)MsgAlhafraInteriorsDontMentionTheTidalWave);
    // "Don't mention the tidal wave to the other merchants... It did a lot..."
    Event_ShowMessageAndWait(actor, 0);
  }
}

/// Felix talks to the innkeeper: across his counter, the inn; from anywhere
/// else he tells of the townsfolk who lost their homes.
void AlhafraRooms_TalkToInnkeeper(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineState.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Inn_CheckIn(ALHAFRA_INN, actor);
  } else {
    Event_SetMessageId((s32)MsgAlhafraInteriorsMaybeYouveNoticedButA);
    // "Maybe you've noticed, but a lot of folk here lost their homes in the..."
    Event_ShowMessageAndWait(actor, 0);
  }
}

/// Felix talks to the priest: across his counter, the Sanctum; from anywhere
/// else he tells of a terrible village in the middle of Osenia.
void AlhafraRooms_TalkToPriest(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineState.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Sanctum_Run(actor);
  } else {
    Event_SetMessageId((s32)MsgAlhafraInteriorsInTheMiddleOfOsenia);
    // "In the middle of Osenia, there's a terrible village where they..."
    Event_ShowMessageAndWait(actor, 0);
  }
}

/// Felix talks to the lady of the house, who scowls, worn out by her
/// houseguests.
void AlhafraRooms_TalkToHostess(s32 actor) {
  Actor_ShowEmote(actor, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  FieldEvent_SetMessageId((s32)MsgAlhafraInteriorsAughWithAllTheseDemanding);
  // "Augh! With all these demanding houseguests, I feel like I've lost..."
  Event_ShowMessage(actor, 0);
}

/// The first time Felix talks to the old houseguest, or reads his mind, he
/// steps up to the old couple, who ask the lady of the house for tea; she
/// tells them to fix it themselves.
void AlhafraRooms_GuestsAskForTea(void) {
  Event_SetFlag(FLAG_GUESTS_ASKED_FOR_TEA);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 152, 328);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
  FieldEvent_SetMessageId((s32)MsgAlhafraInteriorsArentYouThirstyMyDear);
  Actor_FaceDirection(ACTOR_DEMANDING_GUEST, FACING_EAST, 0);
  FieldEvent_Wait(20);
  // "Aren't you thirsty, my dear?"
  FieldEvent_ShowMessageThenWait(ACTOR_DEMANDING_GUEST, 0, 10);
  Actor_FaceDirection(ACTOR_DEMANDING_GUESTS_WIFE, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_DEMANDING_GUESTS_WIFE, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Oh, you're right, dear. I am!"
  FieldEvent_ShowMessageThenWait(ACTOR_DEMANDING_GUESTS_WIFE, 0, 10);
  Actor_SetSpeed(ACTOR_DEMANDING_GUEST, 0.5);
  Actor_WalkByAndWait(ACTOR_DEMANDING_GUEST, -32, 0);
  FieldEvent_Wait(10);
  // "Sorry to bother you, but we'd like some tea."
  FieldEvent_ShowMessageThenWait(ACTOR_DEMANDING_GUEST, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_HOSTESS, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_HOSTESS, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_HOSTESS, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Are you talking to me?!"
  FieldEvent_ShowMessageThenWait(ACTOR_HOSTESS, 0, 10);
  FieldActor_StartRepeatedMotion(ACTOR_DEMANDING_GUEST, 3);
  Actor_SetAttachedEffect(ACTOR_DEMANDING_GUEST,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_DEMANDING_GUESTS_WIFE, 3);
  Actor_SetAttachedEffect(ACTOR_DEMANDING_GUESTS_WIFE,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(50);
  Actor_RunRepeatedMotion(ACTOR_HOSTESS, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_HOSTESS, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(10);
  // "If you want tea, you can go right ahead and fix it yourself!"
  FieldEvent_ShowMessageThenWait(ACTOR_HOSTESS, 0, 10);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(ACTOR_DEMANDING_GUEST, 152, 312);
  FieldEvent_Wait(10);
  Actor_SetAnimation(ACTOR_DEMANDING_GUESTS_WIFE, ANIM_SHAKE_HEAD);
  Actor_SetAnimationAndWait(ACTOR_DEMANDING_GUEST, ANIM_SHAKE_HEAD);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_DEMANDING_GUEST, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_DEMANDING_GUESTS_WIFE, FACING_SOUTH, 0);
  Event_End();
}

/// Felix talks to the old houseguest, who grumbles that the lady of the
/// house is no lady at all.
void AlhafraRooms_TalkToDemandingGuest(s32 actor) {
  Actor_FaceEachOther(actor, gMainRoutineState.leaderActorId, 0);
  FieldEvent_SetMessageId((s32)MsgAlhafraInteriorsWellISayItSeems);
  // "Well! I say! It seems the lady of the house is no lady at all!"
  Event_ShowMessage(actor, 0);
  Actor_FaceDirection(ACTOR_DEMANDING_GUEST, FACING_SOUTH_SOUTHEAST, 0);
}

/// Felix casts Mind Read on Alex, who turns to him and asks him to stop.
void AlhafraRooms_ReadAlexsMind(s32 unused, s32 actor) {
  Actor_FaceEachOther(actor, gMainRoutineState.leaderActorId, 0);
  FieldEvent_SetMessageId((s32)MsgAlhafraInteriorsReallyIDoWishYou);
  // "Really, I do wish you would quit using your Mind Read on me... Trust..."
  Event_ShowMessage(actor, 0);
  Actor_FaceDirection(actor, FACING_NORTHEAST, 0);
}

/// The first time Felix talks to the master of the house, he tells that the
/// town has started repairing the houses, and smiles that his guests will
/// soon be gone.
void AlhafraRooms_HostTellsOfRepairs(s32 actor) {
  Event_SetFlag(FLAG_HOST_TOLD_OF_REPAIRS);
  FieldEvent_SetMessageId(
      (s32)MsgAlhafraInteriorsWeveAllStartedWorkingTogether);
  // "We've all started working together to repair the houses!"
  Event_ShowMessage(actor, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(actor, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Oooh... Not much longer, and then I can kick these folks out of my..."
  Event_ShowMessage(actor, 0);
}

/// Felix talks to Alex after Briggs is caught, and Alex asks whether it was
/// he who caught him.
void AlhafraRooms_AlexHearsOfBriggs(s32 actor) {
  s32 msg = (s32)MsgAlhafraInteriorsSoIHearYouCaught;

  FieldEvent_SetMessageId(msg);
  // "So, I hear you caught Briggs..."
  // Yes: "Mia, a young girl from my clan, was very kind, much like you are..."
  // No: "You have no need to hide such fine exploits from me. Working hard..."
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}

/// The houses' triggers.
const struct Trigger *Scene_GetTriggers(void) {
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED))
    return gAlhafraRooms_TriggersOnceTheMastIsCleared;
  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT))
    return gAlhafraRooms_TriggersOnceBriggsIsCaught;
  return gAlhafraRooms_Triggers;
}

s32 EffectSprite_DrawAtWorldXYZ(struct EffectSprite *sprite, s32 x, s32 y,
                                s32 z);
void EffectSprite_InitWithFadeOverlay(struct EffectSprite *sprite, s32 pivotX,
                                      s32 pivotY, u32 attributes);

/// The old houseguest's update: draws his head and upper body again, 12 pixels
/// above him, over the stack of crates he stands in front of; without it the
/// crates cover him from the waist up and only his legs show.
s32 AlhafraRooms_DrawFirstGuestSprite(struct Actor *guest) {
  EffectSprite_DrawAtWorldXYZ(&gAlhafraRooms_FirstGuestSprite, guest->x.fixed,
                              guest->y.fixed + toFIXED(12), guest->z.fixed);
  return 0;
}

/// His wife's: the same, 8 pixels above her, so she too shows whole in front of
/// the crates.
s32 AlhafraRooms_DrawSecondGuestSprite(struct Actor *guest) {
  EffectSprite_DrawAtWorldXYZ(&gAlhafraRooms_SecondGuestSprite, guest->x.fixed,
                              guest->y.fixed + toFIXED(8), guest->z.fixed);
  return 0;
}

// The Japanese edition sets no FLAG_ENTERED_IN_THE_EAST by the leader's
// position and places neither sailor of the Crossbone Isle tale.
#if TLA_PATCH_1
void FadeOverlay_FillTiles(s32 mode);

/// Sets up the houses: the old couple's second sprites, and once the mast is
/// cleared the two sailors in Alex's room.
s32 Scene_Initialize(void) {
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Actor_Get(ACTOR_INNKEEPER)->collisionFlags |= ACTOR_NEAR_TALK;
  Actor_Get(ACTOR_INNKEEPER)->collisionFlags |= ACTOR_NEAR_PSYNERGY;
  FieldActor_SetSpritePriority(ACTOR_PRIEST, 3);
  FieldActor_SetSpritePriority(ACTOR_ALEX, 3);
  FieldActor_SetSpritePriority(ACTOR_DEMANDING_GUEST, 3);
  FieldActor_SetSpritePriority(ACTOR_DEMANDING_GUESTS_WIFE, 3);
  FadeOverlay_FillTiles(0);
  Call4((void (*)())EffectSprite_InitWithFadeOverlay,
        (s32)&gAlhafraRooms_FirstGuestSprite, 8, 15, OBJ_SIZE_16X16);
  Call4((void (*)())EffectSprite_InitWithFadeOverlay,
        (s32)&gAlhafraRooms_SecondGuestSprite, 8, 15, OBJ_SIZE_16X16);
  gAlhafraRooms_SecondGuestSprite.scaleY = FIXED_ONE * 3 / 2;
  Actor_Get(ACTOR_DEMANDING_GUEST)->update = AlhafraRooms_DrawFirstGuestSprite;
  Actor_Get(ACTOR_DEMANDING_GUESTS_WIFE)->update =
      AlhafraRooms_DrawSecondGuestSprite;
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED)) {
    if (Actor_Get(gMainRoutineState.leaderActorId)->x.fixed >= toFIXED(512))
      Event_SetFlag(FLAG_ENTERED_IN_THE_EAST);
    else
      Event_ClearFlag(FLAG_ENTERED_IN_THE_EAST);
    if (Event_GetFlag(FLAG_ENTERED_IN_THE_EAST) &&
        Event_GetFlag(FLAG_ISAAC_CLEARED_CROSSBONE_ISLE) &&
        !Event_GetFlag(FLAG_ALHAFRA_HEARD_OF_CROSSBONE_ISLE)) {
      Actor_SetPosition(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 376, 316);
      Actor_SetPosition(ACTOR_SAILOR_EAGER_TO_JOIN, 392, 344);
    } else {
      Actor_SetPosition(ACTOR_ALEX, 0, 0);
      Actor_SetPosition(ACTOR_SAILOR_EAGER_TO_JOIN, 0, 0);
    }
  } else if (Event_GetFlag(FLAG_BRIGGS_CAUGHT)) {
    if (Event_GetFlag(FLAG_CHAMPA_MET_ALEX))
      Actor_SetPosition(ACTOR_ALEX, 0, 0);
  } else {
    FieldActor_SetSpritePriority(ACTOR_CHILD_MISSING_MEAT, 2);
    FieldActor_SetSpritePriority(ACTOR_WOMAN_ON_THE_CRAMMED_ROOMS, 3);
    FieldActor_SetSpritePriority(ACTOR_CHILD_COOPED_UP, 2);
    FieldActor_SetSpritePriority(ACTOR_MAN_ANGRY_AT_THE_MAYOR, 3);
  }
  return 0;
}
#else
void FadeOverlay_FillTiles(s32 mode);

/// Sets up the houses and the old couple's second sprites.
s32 Scene_Initialize(void) {
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Actor_Get(ACTOR_INNKEEPER)->collisionFlags |= ACTOR_NEAR_TALK;
  Actor_Get(ACTOR_INNKEEPER)->collisionFlags |= ACTOR_NEAR_PSYNERGY;
  FieldActor_SetSpritePriority(ACTOR_PRIEST, 3);
  FieldActor_SetSpritePriority(ACTOR_ALEX, 3);
  FieldActor_SetSpritePriority(ACTOR_DEMANDING_GUEST, 3);
  FieldActor_SetSpritePriority(ACTOR_DEMANDING_GUESTS_WIFE, 3);
  FadeOverlay_FillTiles(0);
  Call4((void (*)())EffectSprite_InitWithFadeOverlay,
        (s32)&gAlhafraRooms_FirstGuestSprite, 8, 15, OBJ_SIZE_16X16);
  Call4((void (*)())EffectSprite_InitWithFadeOverlay,
        (s32)&gAlhafraRooms_SecondGuestSprite, 8, 15, OBJ_SIZE_16X16);
  gAlhafraRooms_SecondGuestSprite.scaleY = FIXED_ONE * 3 / 2;
  Actor_Get(ACTOR_DEMANDING_GUEST)->update = AlhafraRooms_DrawFirstGuestSprite;
  Actor_Get(ACTOR_DEMANDING_GUESTS_WIFE)->update =
      AlhafraRooms_DrawSecondGuestSprite;
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED)) {
    if (!Event_GetFlag(FLAG_ISAAC_CLEARED_CROSSBONE_ISLE) ||
        Event_GetFlag(FLAG_ALHAFRA_HEARD_OF_CROSSBONE_ISLE)) {
      Actor_SetPosition(ACTOR_ALEX, 0, 0);
      Actor_SetPosition(ACTOR_SAILOR_EAGER_TO_JOIN, 0, 0);
    }
  } else if (Event_GetFlag(FLAG_BRIGGS_CAUGHT)) {
    if (Event_GetFlag(FLAG_CHAMPA_MET_ALEX))
      Actor_SetPosition(ACTOR_ALEX, 0, 0);
  } else {
    FieldActor_SetSpritePriority(ACTOR_CHILD_MISSING_MEAT, 2);
    FieldActor_SetSpritePriority(ACTOR_WOMAN_ON_THE_CRAMMED_ROOMS, 3);
    FieldActor_SetSpritePriority(ACTOR_CHILD_COOPED_UP, 2);
    FieldActor_SetSpritePriority(ACTOR_MAN_ANGRY_AT_THE_MAYOR, 3);
  }
  return 0;
}
#endif

s32 Scene_PrepareMap(void) { return 0; }

extern u32 gAlhafraRooms_TravelersWalkOffScript[];

/// In a game carried over from Golden Sun in which Isaac beat Deadbeard, Felix
/// walks into Alex's old room upstairs in the inn after the mast is cleared
/// (cells (31, 19) to (31, 22)) and overhears two sailors: Isaac picked
/// Crossbone Isle clean and beat its ghost pirate, so he is their leader now.
/// They walk out past Felix, and it happens once.
///
/// It runs while FLAG_ISAAC_CLEARED_CROSSBONE_ISLE is set and sets
/// FLAG_ALHAFRA_HEARD_OF_CROSSBONE_ISLE; outside the Japanese edition, only
/// when Felix came in from the east of the map (FLAG_ENTERED_IN_THE_EAST).
void AlhafraRooms_OverhearCrossboneIsleTale(void) {

  if (Event_GetFlag(FLAG_ALHAFRA_HEARD_OF_CROSSBONE_ISLE)) {
  } else {
#if TLA_PATCH_1 /* The Japanese edition does not check                         \
                   FLAG_ENTERED_IN_THE_EAST. */
    if (!Event_GetFlag(FLAG_ENTERED_IN_THE_EAST)) {
    } else
#endif
    {
      Event_SetFlag(FLAG_ALHAFRA_HEARD_OF_CROSSBONE_ISLE);
      Event_Start();
      FieldEvent_EndLastingPsynergy(0);
      FieldEvent_SetMessageId(
          (s32)MsgAlhafraInteriorsLegendsSaidThatCrossboneIsle);
      Actor_WalkToAndWait(ACTOR_FELIX, 504, 328);
      Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_WEST);
      FieldCamera_MoveTo(408, CAMERA_KEEP, 328, 1);
      FieldCamera_WaitForArrival();
      FieldEvent_Wait(10);
      Actor_RunRepeatedMotion(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 2);
      FieldEvent_Wait(5);
      // "Legends said that Crossbone Isle could be found in the middle of..."
      FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 0,
                                     5);
      Actor_ShowEmote(ACTOR_SAILOR_EAGER_TO_JOIN,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
      // "Ah... Crossbone Isle... Wait, did you say "could"? Is it not there..."
      FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_EAGER_TO_JOIN, 0, 5);
      Actor_ShowEmote(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE,
                      EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
      // "Well, it's still there, but there's no point in going. Someone..."
      FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 0,
                                     5);
      Actor_ShowEmote(ACTOR_SAILOR_EAGER_TO_JOIN, EMOTE_IN_FRONT | EMOTE_WINCE,
                      30);
      // "What, he took all the treasure?"
      FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_EAGER_TO_JOIN, 0, 5);
      Actor_SetAnimationAndWait(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE,
                                ANIM_NOD);
      FieldEvent_Wait(10);
      // "Yeah. And apparently, he even defeated the ancient ghost pirate..."
      FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 0,
                                     5);
      FieldActor_Jump(ACTOR_SAILOR_EAGER_TO_JOIN, 6, 15);
      FieldActor_Jump(ACTOR_SAILOR_EAGER_TO_JOIN, 6, 23);
      // "What about the stories that if you beat the ghost pirate, you..."
      FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_EAGER_TO_JOIN, 0, 5);
      Actor_SetAnimationAndWait(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE,
                                ANIM_NOD);
      FieldEvent_Wait(10);
      // "Well, you see, that means that Isaac is our leader now."
      FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 0,
                                     5);
      Actor_ShowEmote(ACTOR_SAILOR_EAGER_TO_JOIN,
                      EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
      // "Wow... just like that? I'll bet he's pretty tough."
      FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_EAGER_TO_JOIN, 0, 5);
      Actor_RunRepeatedMotion(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 2);
      FieldEvent_Wait(5);
      // "No doubt. But you know, he must be a reliable guy, too..."
      FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 0,
                                     5);
      Actor_SetAnimationAndWait(ACTOR_SAILOR_EAGER_TO_JOIN, ANIM_NOD);
      FieldEvent_Wait(10);
      // "You think?"
      FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_EAGER_TO_JOIN, 0, 5);
      Actor_FaceDirectionAndWait(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE,
                                 FACING_NORTH_NORTHEAST);
      FieldEvent_Wait(15);
      // "You'd have to be to beat that ghost. That's why we're looking for..."
      FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 0,
                                     5);
      Actor_SetAnimationAndWait(ACTOR_SAILOR_EAGER_TO_JOIN, ANIM_NOD);
      FieldEvent_Wait(10);
      // "If he's looking for a pirate crew, he can count me in!"
      FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_EAGER_TO_JOIN, 0, 5);
      Actor_FaceDirectionAndWait(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE,
                                 FACING_SOUTH_SOUTHEAST);
      FieldEvent_Wait(15);
      Actor_SetAnimationAndWait(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE,
                                ANIM_NOD);
      FieldEvent_Wait(20);
      Actor_SetAnimationAndWait(ACTOR_SAILOR_EAGER_TO_JOIN, ANIM_NOD);
      FieldEvent_Wait(20);
      Actor_SetSpeed(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 1);
      Actor_SetSpeed(ACTOR_SAILOR_EAGER_TO_JOIN, 1);
      FieldActor_SetSpritePriority(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 2);
      FieldActor_SetSpritePriority(ACTOR_SAILOR_EAGER_TO_JOIN, 2);
      EventCamera_FollowActor(ACTOR_SAILOR_EAGER_TO_JOIN, 1);
      Actor_WalkTo(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 394, 342);
      FieldEvent_Wait(10);
      Actor_WalkTo(ACTOR_SAILOR_EAGER_TO_JOIN, 450, 346);
      FieldActor_WaitForMoveById(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE);
      Actor_WalkTo(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 450, 346);
      FieldActor_WaitForMoveById(ACTOR_SAILOR_EAGER_TO_JOIN);
      Actor_WalkTo(ACTOR_SAILOR_EAGER_TO_JOIN, 470, 328);
      FieldActor_WaitForMoveById(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE);
      Actor_WalkTo(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 470, 328);
      FieldActor_WaitForMoveById(ACTOR_SAILOR_EAGER_TO_JOIN);
      Actor_WalkTo(ACTOR_SAILOR_EAGER_TO_JOIN, 486, 328);
      FieldActor_WaitForMoveById(ACTOR_SAILOR_EAGER_TO_JOIN);
      Actor_SetAnimation(ACTOR_SAILOR_EAGER_TO_JOIN, ANIM_STAND);
      Actor_FaceDirectionAndWait(ACTOR_SAILOR_EAGER_TO_JOIN, FACING_EAST);
      FieldActor_WaitForMoveById(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE);
      Actor_SetAnimation(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, ANIM_STAND);
      Actor_FaceDirectionAndWait(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE,
                                 FACING_EAST);
      Actor_WalkByAndWait(ACTOR_FELIX, 0, -16);
      Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
      FieldEvent_Wait(10);
      Actor_SetScript(ACTOR_SAILOR_EAGER_TO_JOIN,
                      gAlhafraRooms_TravelersWalkOffScript);
      FieldEvent_Wait(10);
      Actor_SetScript(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE,
                      gAlhafraRooms_TravelersWalkOffScript);
      FieldEvent_Wait(10);
      FieldActor_WatchAndSetScript(ACTOR_FELIX, ACTOR_SAILOR_EAGER_TO_JOIN);
      FieldEvent_Wait(60);
      FieldActor_SetSpritePriority(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 3);
      FieldActor_SetSpritePriority(ACTOR_SAILOR_EAGER_TO_JOIN, 3);
      FieldActor_WaitForScriptEndById(ACTOR_SAILOR_EAGER_TO_JOIN);
      FieldActor_Stop(ACTOR_FELIX);
      FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
      Actor_SetPosition(ACTOR_SAILOR_EAGER_TO_JOIN, 0, 0);
      Event_PlaySound(SE_MAP_EXIT);
      FieldActor_WaitForScriptEndById(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE);
      Actor_SetPosition(ACTOR_SAILOR_TELLING_OF_CROSSBONE_ISLE, 0, 0);
      Event_PlaySound(SE_MAP_EXIT);
      FieldEvent_Wait(10);
      Event_End();
    }
  }
}

// clang-format off
u32 gAlhafraRooms_TravelersWalkOffScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(520, 0, 328),
    SC_SYNC,
    SC_MOVE(520, 0, 312),
    SC_SYNC,
    SC_MOVE(552, 0, 312),
    SC_SYNC,
    SC_END,
};

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(AlhafraRooms),
    SCENE_EXIT_UNLESS(1, Alhafra, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Alhafra, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, Alhafra, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, Alhafra, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, Alhafra, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(20, AlhafraRooms, 20, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(21, AlhafraRooms, 21, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(22, AlhafraRooms, 22, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(23, AlhafraRooms, 23, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(24, AlhafraRooms, 24, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(25, AlhafraRooms, 25, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_TEAL_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 88, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0, 544, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 184, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 104, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 344, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 312, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 312, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 84, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 152, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 120, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 280, FACING_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_COOK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 762, 0, 84, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 864, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 136, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 152, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 112, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 520, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 536, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 552, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 544, 0, 544, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 344, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 360, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 312, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAlhafraRooms_ActorPlacementsOnceBriggsIsCaught[] = {
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_TEAL_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 88, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0, 544, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 184, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 104, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 344, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 312, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 312, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 84, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 152, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 120, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 864, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_COOK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 762, 0, 84, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 136, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 512, 0, 536, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 552, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 312, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAlhafraRooms_ActorPlacementsOnceTheMastIsCleared[] = {
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_TEAL_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 88, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0, 544, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 184, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 104, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 344, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 312, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 312, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 312, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 84, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 152, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 120, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 864, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_COOK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 762, 0, 84, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 136, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 512, 0, 536, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 552, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 316, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 344, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gAlhafraRooms_Triggers[] = {
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 20, CONDITION_ALWAYS, 20},
    {TRIGGER_EXIT, 21, CONDITION_ALWAYS, 21},
    {TRIGGER_EXIT, 22, CONDITION_ALWAYS, 22},
    {TRIGGER_EXIT, 23, CONDITION_ALWAYS, 23},
    {TRIGGER_EXIT, 24, CONDITION_ALWAYS, 24},
    {TRIGGER_EXIT, 25, CONDITION_ALWAYS, 25},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
     EVENT(AlhafraRooms_TalkToItemMerchant)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     EVENT(AlhafraRooms_TalkToInnkeeper)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_PRIEST, CONDITION_ALWAYS,
     EVENT(AlhafraRooms_TalkToPriest)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 214, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriorsSeasonalVeggiesBoilingWithSome))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 215, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriorsThereIsASeafoodPie))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 216, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriorsItLooksLikeSomeKind))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 217, CONDITION_ALWAYS,
     FIND_MESSAGE(
         MSGNO(MsgAlhafraInteriorsItsAlhafrasFamousBroiledLobsterits))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriorsThatSeafoodStewHasAn))},
    {TRIGGER_STEP, 30, FLAG_ALHAFRA_MET_ALEX, EVENT(AlhafraRooms_MeetAlex)},
    {TRIGGER_TALK, ACTOR_WOMAN_ANGRY_AT_THE_SALE, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsIDontCareIfIt)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_SHABBY_BUYERS, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsThoseGuysWhoBoughtThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ANGRY_AT_THE_SALE,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsIWonderIfTheMayor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_SHABBY_BUYERS,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsWhyWouldAnyonePaySo)},
    {TRIGGER_TALK, ACTOR_DEMANDING_GUEST, FLAG_GUESTS_ASKED_FOR_TEA,
     EVENT(AlhafraRooms_GuestsAskForTea)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_DEMANDING_GUEST | TRIGGER_SUBJECT_OWN_EFFECT,
     FLAG_GUESTS_ASKED_FOR_TEA, EVENT(AlhafraRooms_GuestsAskForTea)},
    {TRIGGER_TALK, ACTOR_HOST, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsWereTakingCareOfSome)},
    {TRIGGER_TALK, ACTOR_HOSTESS, CONDITION_ALWAYS,
     EVENT(AlhafraRooms_TalkToHostess)},
    {TRIGGER_TALK, ACTOR_DEMANDING_GUEST, CONDITION_ALWAYS,
     EVENT(AlhafraRooms_TalkToDemandingGuest)},
    {TRIGGER_TALK, ACTOR_DEMANDING_GUESTS_WIFE, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsWhenWeLostOurHouse)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOST, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsIJustWishThatOld)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOSTESS, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsIDontMindHelpingPeople)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DEMANDING_GUEST,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsAllIAskIsFor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DEMANDING_GUESTS_WIFE,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsIsThisHowAYoung)},
    {TRIGGER_TALK, ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsDontMentionTheTidalWave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_SHOPKEEPER,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsIfAllMyGoodsWere)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsMaybeYouveNoticedButA)},
    {TRIGGER_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsYouSeeHowFullOur)},
    {TRIGGER_TALK, ACTOR_MAN_ASKING_FOR_MONEY, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsTheTownOughtToGive)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_CRAMMED_ROOMS, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsICantBelieveTheInnkeeper)},
    {TRIGGER_TALK, ACTOR_INN_COOK, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsTheInnkeeperToldMeTo)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_MEALS, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsTheInnkeeperServesHisPaying)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_MEALS, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsWereAllGuestsInThe)},
    {TRIGGER_TALK, ACTOR_HUNGRY_CHILD, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsOhhhImHungryMyTummy)},
    {TRIGGER_TALK, ACTOR_CHILD_MISSING_MEAT, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsIDontThinkIveHad)},
    {TRIGGER_TALK, ACTOR_MAN_ANGRY_AT_THE_MAYOR, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsIllNeverForgiveTheMayor)},
    {TRIGGER_TALK, ACTOR_WOMAN_WORRIED_ABOUT_HER_GUESTS, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsOurGuestsAreVeryUpset)},
    {TRIGGER_TALK, ACTOR_BOY_ON_THE_MAYORS_TREASURE, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsMyPopsBeenLookinAround)},
    {TRIGGER_TALK, ACTOR_CHILD_COOPED_UP, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsWhyDoWeAllHave)},
    {TRIGGER_TALK, ACTOR_MAN_BEARING_THE_DISCOMFORT, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsWeAllHaveToBear)},
    {TRIGGER_TALK, ACTOR_WOMAN_EYEING_THE_WARRIOR, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsIHeardTheresOnlyOne)},
    {TRIGGER_TALK, ACTOR_ALEX, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsTheBrokenMastOnThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsINeverImaginedAnyoneFrom)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsPerhapsTheMayorWillCover)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ASKING_FOR_MONEY,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsIBetTheMasterOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_CRAMMED_ROOMS,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsICantBelieveTheInnkeeperIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_COOK, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsICantBelieveImOnly)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_MEALS,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsMaybeItsCauseAllWeve)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_MEALS,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsIThinkIveLostWeight)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HUNGRY_CHILD, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsMaybeImJustHungryBut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_MISSING_MEAT,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsSniffSniffThatSmellThats)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ANGRY_AT_THE_MAYOR,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsIHopeTheMayorsTreasure)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WORRIED_ABOUT_HER_GUESTS,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsIKnowOurMayorsBad)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_THE_MAYORS_TREASURE,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsEveryoneSaysTheHillBeneath)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_COOPED_UP,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsIThinkTheyPutMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_BEARING_THE_DISCOMFORT,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsWhenYoureLivingPackedIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_EYEING_THE_WARRIOR,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsThatWarriorNextDoorIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_ALEX | TRIGGER_SUBJECT_OWN_EFFECT, CONDITION_ALWAYS,
     EVENT(AlhafraRooms_ReadAlexsMind)},
    {TRIGGER_TALK, ACTOR_PRIEST, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsInTheMiddleOfOsenia)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRIEST, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsThePeopleOfGarohBrought)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAlhafraRooms_TriggersOnceBriggsIsCaught[] = {
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 20, CONDITION_ALWAYS, 20},
    {TRIGGER_EXIT, 21, CONDITION_ALWAYS, 21},
    {TRIGGER_EXIT, 22, CONDITION_ALWAYS, 22},
    {TRIGGER_EXIT, 23, CONDITION_ALWAYS, 23},
    {TRIGGER_EXIT, 24, CONDITION_ALWAYS, 24},
    {TRIGGER_EXIT, 25, CONDITION_ALWAYS, 25},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
     EVENT(AlhafraRooms_TalkToItemMerchant)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     EVENT(AlhafraRooms_TalkToInnkeeper)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_PRIEST, CONDITION_ALWAYS,
     EVENT(AlhafraRooms_TalkToPriest)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 214, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriorsSeasonalVeggiesBoilingWithSome))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 215, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriorsThereIsASeafoodPie))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 216, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriorsItLooksLikeSomeKind))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 217, CONDITION_ALWAYS,
     FIND_MESSAGE(
         MSGNO(MsgAlhafraInteriorsItsAlhafrasFamousBroiledLobsterits))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriorsThatSeafoodStewHasAn))},
    {TRIGGER_TALK, ACTOR_WOMAN_EYEING_THE_WARRIOR,
     IF_FLAG_SET(FLAG_CHAMPA_MET_ALEX),
     MSGNO(MsgAlhafraInteriorsThatCuteBoyNextDoor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_EYEING_THE_WARRIOR,
     IF_FLAG_SET(FLAG_CHAMPA_MET_ALEX),
     MSGNO(MsgAlhafraInteriorsTheFellowNextDoorWas)},
    {TRIGGER_TALK, ACTOR_WOMAN_ANGRY_AT_THE_SALE, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsImAmazedThoseGuysWho)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_SHABBY_BUYERS, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsIThoughtItWasWeird)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ANGRY_AT_THE_SALE,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsPiratesHereInAlhafraThats)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_SHABBY_BUYERS,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsThoseGuysWereJustTrying)},
    {TRIGGER_TALK, ACTOR_HOST, FLAG_HOST_TOLD_OF_REPAIRS,
     EVENT(AlhafraRooms_HostTellsOfRepairs)},
    {TRIGGER_TALK, ACTOR_HOST, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsOoohNotMuchLongerAnd)},
    {TRIGGER_TALK, ACTOR_HOSTESS, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsOnceMyOldNeighborsHouse)},
    {TRIGGER_TALK, ACTOR_DEMANDING_GUEST, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsOnceEveryoneStartedWorkingTo)},
    {TRIGGER_TALK, ACTOR_DEMANDING_GUESTS_WIFE, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsIWonderWhatHappenedThats)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOST, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsAhhItsSoRelievingTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOSTESS, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsItWontBeLongUntil)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DEMANDING_GUEST,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsTheLadyOfThisHouse)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DEMANDING_GUESTS_WIFE,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsTheLadyOfTheHouse)},
    {TRIGGER_TALK, ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsTheMadransDroppedInAnd)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_SHOPKEEPER,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsTheMadransMustHaveBeen)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsAsSoonAsImDone)},
    {TRIGGER_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsWereAllTakingTurnsHelping)},
    {TRIGGER_TALK, ACTOR_MAN_ASKING_FOR_MONEY, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsEveryonesWorkingSoHardTo)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_CRAMMED_ROOMS, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsTheresStillPlentyOfWork)},
    {TRIGGER_TALK, ACTOR_INN_COOK, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsTonightsMealIsChockFull)},
    {TRIGGER_TALK, ACTOR_CHILD_MISSING_MEAT, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsMomSaidWereGonnaHave)},
    {TRIGGER_TALK, ACTOR_MAN_BEARING_THE_DISCOMFORT, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsMySonHasTerribleLuck)},
    {TRIGGER_TALK, ACTOR_WOMAN_EYEING_THE_WARRIOR, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsTheGuyNextToUs)},
    {TRIGGER_TALK, ACTOR_ALEX, CONDITION_ALWAYS,
     EVENT(AlhafraRooms_AlexHearsOfBriggs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsWhyDoIHaveTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsIMayNotLookVery)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ASKING_FOR_MONEY,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsWeHeardALoudPopping)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_CRAMMED_ROOMS,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsIWonderWhatOurNew)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_COOK, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsIWasToldToUse)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_MISSING_MEAT,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsOhIfWeGetA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_BEARING_THE_DISCOMFORT,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsIGuessIJustDont)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_EYEING_THE_WARRIOR,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsOurNeighborIsSoMellow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ALEX, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsFeelFreeToHelpWhomever)},
    {TRIGGER_TALK, ACTOR_PRIEST, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsCapturingTheDreadedBriggsWas)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRIEST, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsIThinkThisManMight)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAlhafraRooms_TriggersOnceTheMastIsCleared[] = {
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 20, CONDITION_ALWAYS, 20},
    {TRIGGER_EXIT, 21, CONDITION_ALWAYS, 21},
    {TRIGGER_EXIT, 22, CONDITION_ALWAYS, 22},
    {TRIGGER_EXIT, 23, CONDITION_ALWAYS, 23},
    {TRIGGER_EXIT, 24, CONDITION_ALWAYS, 24},
    {TRIGGER_EXIT, 25, CONDITION_ALWAYS, 25},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
     EVENT(AlhafraRooms_TalkToItemMerchant)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     EVENT(AlhafraRooms_TalkToInnkeeper)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_PRIEST, CONDITION_ALWAYS,
     EVENT(AlhafraRooms_TalkToPriest)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 214, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriorsSeasonalVeggiesBoilingWithSome))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 215, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriorsThereIsASeafoodPie))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 216, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriorsItLooksLikeSomeKind))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 217, CONDITION_ALWAYS,
     FIND_MESSAGE(
         MSGNO(MsgAlhafraInteriorsItsAlhafrasFamousBroiledLobsterits))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriorsThatSeafoodStewHasAn))},
    {TRIGGER_STEP, 31, IF_FLAG_SET(FLAG_ISAAC_CLEARED_CROSSBONE_ISLE),
     EVENT(AlhafraRooms_OverhearCrossboneIsleTale)},
    {TRIGGER_TALK, ACTOR_WOMAN_ANGRY_AT_THE_SALE, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsIDontWantToSay)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_SHABBY_BUYERS, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsBriggsDidntCommitAnyCrimes)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ANGRY_AT_THE_SALE,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsBriggsPaidForTheBoat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_SHABBY_BUYERS,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsIDontBelieveForOne)},
    {TRIGGER_TALK, ACTOR_HOST, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsFixingThatOldGuysHouse)},
    {TRIGGER_TALK, ACTOR_HOSTESS, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsTheRepairsOnOurNeighbors)},
    {TRIGGER_TALK, ACTOR_DEMANDING_GUEST, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsMyHouseIsDamagedMore)},
    {TRIGGER_TALK, ACTOR_DEMANDING_GUESTS_WIFE, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsMyHusbandAndIDont)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOST, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsWillTheyEverGetOut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOSTESS, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsIDontHaveAnythingTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DEMANDING_GUEST,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsIDontMindNotBeing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DEMANDING_GUESTS_WIFE,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsIHatedThisPlaceAt)},
    {TRIGGER_TALK, ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsTheBridgeToTheWest)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_SHOPKEEPER,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsPeopleWillStartTravelingAgain)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsWereStillRecoveringFromThe)},
    {TRIGGER_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsTheMayorUsedTheMoney)},
    {TRIGGER_TALK, ACTOR_MAN_ASKING_FOR_MONEY, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsItSeemsThatBriggsBroke)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_CRAMMED_ROOMS, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsBriggsMightHaveHadA)},
    {TRIGGER_TALK, ACTOR_INN_COOK, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsABiggerBoatCouldHaul)},
    {TRIGGER_TALK, ACTOR_CHILD_MISSING_MEAT, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsIJustWantAHouse)},
    {TRIGGER_TALK, ACTOR_MAN_BEARING_THE_DISCOMFORT, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsSomeoneSaidMySonAnd)},
    {TRIGGER_TALK, ACTOR_WOMAN_EYEING_THE_WARRIOR, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsSoThatAlexFellowLeft)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsAfterTheTidalWaveIt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsTheMayorMayHavePulled)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ASKING_FOR_MONEY,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsBriggsAndHisWifeWere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_CRAMMED_ROOMS,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsIfIHadAKey)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_COOK, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsSoWhatIfTradeBrings)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_MISSING_MEAT,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsIdReallyLikeToHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_BEARING_THE_DISCOMFORT,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsIsntAnyoneHelpingMyFamily)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_EYEING_THE_WARRIOR,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriorsHeWasJustMyType)},
    {TRIGGER_TALK, ACTOR_PRIEST, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsIHeardYouWereThere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRIEST, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriorsTheyWereSoConfidentWhen)},
    {TRIGGER_TABLE_END},
};

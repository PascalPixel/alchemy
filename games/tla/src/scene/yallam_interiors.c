/// @file
/// The scene code of overlay 0x677, in ROM order.

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "types.h"
#include "system.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "field_sprite.h"
#include "event_inline.h"
#include "frame.h"

struct YallamInteriorsObject {
  u8 unk00[6];
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  s32 height;
  s32 scaleX;
  s32 scaleY;
  u8 unk20[3];
  u8 priorityFlags;
  s32 velocityX;
  s32 velocityY;
  u8 unk2c[0x18];
  s32 field44;
  s32 field48;
  u8 unk4c[4];
  struct FieldSprite *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[3];
  u8 collisionFlags;
  u8 unk5a[2];
  u8 field5c;
  u8 unk5d[5];
  u8 timer;
  u8 unk63;
  s16 field64;
  s16 field66;
  s32 field68;
  void *update;
};

struct YallamInteriorsEventWork {
  u8 unk000[0x154];
  s16 exit;
  u8 unk156[4];
  s16 field15a;
  u8 unk15c[2];
  s16 field15e;
  u8 unk160[6];
  s16 field166;
  u8 unk168[0x44];
  s32 screenEffect;
  u8 unk1b0[4];
  s32 field1b4;
  u8 unk1b8[0x0c];
  u16 field1c4;
};

void Event_End(void);
void Shop_Run(s32 shop, s32 keeper);
void Sanctum_Run(s32 keeper);
void Inn_CheckIn(s32 shop, s32 keeper);
void FieldPsynergy_RunHandler(s32 type);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void Func_02000a48(s32 a);
void Event_StartScreenTransition(void);

// ---- this overlay

extern struct ActorPlacement gYallamInteriors_Placements[];
extern struct ActorPlacement gYallamInteriors_Room1Placements[];
extern struct ActorPlacement gYallamInteriors_Room2Placements[];
extern struct Trigger gYallamInteriors_Triggers[];
extern struct Trigger gYallamInteriors_Room1Triggers[];
extern struct Trigger gYallamInteriors_Room2Triggers[];
#include "inventory.h"
#include "version.h"
#include "event_inline.h"
#include "main_routine_work.h"
#include "module/game/game.h"
#include "module/scene/scene.h"
#include "module/scene/yallam_interiors.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov677Room1)
    return gYallamInteriors_Room1Placements;
  if (scene == (s32)&SceneId_Ov677Room2)
    return gYallamInteriors_Room2Placements;
  return gYallamInteriors_Placements;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

/// Sunshine the blacksmith thinks of the swamp he cannot reach without rain,
/// and once the party has had him forge, of his joy at working (flag 0x301).
s32 YallamInteriors_MindReadSunshine(void) {
  s32 t2;

  Event_Start();
  Event_ResolvePendingActions(0);
  t2 = Event_GetFlag(0x301);
  if (t2 != 0) {
    Event_SetMessageId((s32)MsgYallamInteriors37);
  } else {
    Event_SetMessageId((s32)MsgYallamInteriors33);
  }
  Event_ShowMessage(14, 0);
  Event_End();
  return 0;
}

s32 YallamInteriors_MindReadSunshinesWife(void) {
  s32 t2;

  Event_Start();
  Event_ResolvePendingActions(0);
  t2 = Event_GetFlag(0x301);
  if (t2 != 0) {
    Event_SetMessageId((s32)MsgYallamInteriors38);
  } else {
    Event_SetMessageId((s32)MsgYallamInteriors34);
  }
  Event_ShowMessage(15, 0);
  Event_End();
  return 0;
}

s32 YallamInteriors_MindReadSunshinesChild(void) {
  s32 t2;

  Event_Start();
  Event_ResolvePendingActions(0);
  t2 = Event_GetFlag(0x301);
  if (t2 != 0) {
    Event_SetMessageId((s32)MsgYallamInteriors39);
  } else {
    Event_SetMessageId((s32)MsgYallamInteriors35);
  }
  Event_ShowMessage(16, 0);
  Event_End();
  return 0;
}

/// "Dad's not doing any work right now, but I'm making some basic things for
/// the villagers."
s32 YallamInteriors_TalkToSunshinesChild(void) {
  s32 t2;

  Event_Start();
  Event_ResolvePendingActions(0);
  t2 = Event_GetFlag(0x301);
  if (t2 != 0) {
    Event_SetMessageId((s32)MsgYallamInteriors36);
  } else {
    Event_SetMessageId((s32)MsgYallamInteriors32);
  }
  Event_ShowMessage(16, 0);
  Event_End();
  return 0;
}

s32 YallamInteriors_MindReadWeaponMerchant(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgYallamInteriors22);
  Event_ShowMessage(10, 0);
  Event_End();
  return 0;
}

s32 YallamInteriors_MindReadArmorMerchant(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgYallamInteriors24);
  Event_ShowMessage(11, 0);
  Event_End();
  return 0;
}

s32 YallamInteriors_MindReadItemMerchant(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgYallamInteriors26);
  Event_ShowMessage(12, 0);
  Event_End();
  return 0;
}

const struct Trigger *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov677Room1)
    return gYallamInteriors_Room1Triggers;
  if (scene == (s32)&SceneId_Ov677Room2)
    return gYallamInteriors_Room2Triggers;
  return gYallamInteriors_Triggers;
}

/// "Do you like children's songs?"
void YallamInteriors_AskLikeChildrensSongs(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgYallamInteriors10;
  Event_SetMessageId(msg);
  Event_OpenMessage(9, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessage(9, 0);
  Event_End();
}

/// Yallam's weapon merchant sells across the counter, and otherwise wishes
/// Sunshine would work.
void YallamInteriors_TalkToWeaponMerchant(s32 actor) {
  u16 facing = ((struct YallamInteriorsObject *)Actor_GetById(gMainRoutineWork.currentOwner))->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Shop_Run(20, actor);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgYallamInteriors21);
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

/// Yallam's armor merchant sells across the counter, and otherwise sends the
/// party to Sunshine for something special.
void YallamInteriors_TalkToArmorMerchant(s32 actor) {
  u16 facing = ((struct YallamInteriorsObject *)Actor_GetById(gMainRoutineWork.currentOwner))->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Shop_Run(21, actor);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgYallamInteriors23);
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

/// Yallam's item merchant sells across the counter, and otherwise points the
/// way to Taopo Swamp.
void YallamInteriors_TalkToItemMerchant(s32 actor) {
  u16 facing = ((struct YallamInteriorsObject *)Actor_GetById(gMainRoutineWork.currentOwner))->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Shop_Run(22, actor);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgYallamInteriors25);
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

/// Yallam's innkeeper takes the party in across the counter, and otherwise
/// talks of the town that never gets tourists.
void YallamInteriors_TalkToInnkeeper(s32 actor) {
  u16 facing = ((struct YallamInteriorsObject *)Actor_GetById(gMainRoutineWork.currentOwner))->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Inn_CheckIn(7, actor);
    Event_ClearFlag(0x301);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgYallamInteriors27);
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

void YallamInteriors_TalkToHealer(s32 actor) {
  u16 facing = ((struct YallamInteriorsObject *)Actor_GetById(gMainRoutineWork.currentOwner))->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Sanctum_Run(actor);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgMikasallaInteriors27);
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

/// "Ah, so you want me to make you a little something special?"
void YallamInteriors_OfferToForge(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_StartRepeatedMotion(14, 2);
  msg = (s32)MsgYallamInteriors31;
  Event_SetMessageId(msg);
  Event_OpenMessage(14, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessage(14, 0);
  Event_End();
}

s32 Forge_GetItem(void);
void Func_02000a60(s32 actor, s32 angle);

/// Whether anyone in the party carries a material Sunshine can forge: items 417
/// to 427 or 429 to 437.
s32 YallamInteriors_PartyHasForgeMaterial(void) {
  s32 item;

  for (item = 429; item <= 437; item++) {
    if (PartyInventory_FindMember(item) != -1)
      return 1;
  }
  for (item = 417; item <= 427; item++) {
    if (PartyInventory_FindMember(item) != -1)
      return 1;
  }
  return 0;
}

/// Actor 14's talk. Flag 0x10f follows YallamInteriors_PartyHasForgeMaterial's item check (it stays
/// set while flag 0x301 or Forge_GetItem holds). Once 0x10f is set and
/// neither holds, a yes moves actor 14 and the leader, and if Forge_GetItem
/// then holds, places actor 16 and sets flag 0x301.
void YallamInteriors_TalkToSunshine(void) {
  s32 message;

  if (YallamInteriors_PartyHasForgeMaterial())
    Event_SetFlag(0x10f);
  else if (!Event_GetFlag(0x301) && !Forge_GetItem())
    Event_ClearFlag(0x10f);

  if (!Event_GetFlag(0x10f)) {
    message = (s32)MsgAhSoYou;
    Event_SetMessageId(message);
    Event_OpenMessage(14, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      FieldEvent_Wait(10);
      Event_SetMessageId(message + 1);
    } else {
      FieldEvent_Wait(20);
      Event_SetMessageId(message + 2);
    }
    Event_ShowMessage(14, 0);
  } else if (Event_GetFlag(0x301)) {
    Event_SetMessageId((s32)MsgICanTWait);
    Event_ShowMessage(14, 0);
  } else if (Forge_GetItem()) {
    Event_SetMessageId((s32)MsgMyWifeHas);
    Event_ShowMessage(14, 0);
  } else {
    message = (s32)MsgYouWantMe;
    Event_SetMessageId(message);
    Event_OpenMessage(14, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_SetMessageId(message + 1);
      Event_ShowMessage(14, 0);
      Func_02000a60(14, 0xc000);
      Actor_SetSingleChildFlags(
          ((struct YallamInteriorsObject *)Actor_GetById(14)), 1);
      Actor_SetDestination(gMainRoutineWork.currentOwner, 488, 408);
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Actor_FaceDirection(gMainRoutineWork.currentOwner, 0, 0);
      Actor_SetDestination(14, 504, 408);
      Actor_WaitForMoveById(14);
      Actor_FaceActor(14, gMainRoutineWork.currentOwner, 0);
      Forge_Run(14);
      if (Forge_GetItem()) {
        Actor_SetSpeed(14, 0x6666, 0x3333);
        Actor_WalkTo(14, 568, 408);
        Actor_WaitForMoveById(14);
        Actor_WalkTo(14, 568, 360);
        Actor_WaitForMoveById(14);
        Actor_WalkTo(16, 632, 346);
        Actor_FaceDirection(16, FACING_SOUTH, 0);
        Actor_SetAnimation(16, 0);
        Actor_WalkTo(14, 600, 360);
        Actor_WaitForMoveById(14);
        Actor_SetAnimation(14, 0);
        Actor_FaceDirection(14, FACING_SOUTH, 0);
        ((struct YallamInteriorsObject *)Actor_GetById(14))->collisionFlags = 129;
        ((struct YallamInteriorsObject *)Actor_GetById(14))->priorityFlags = 1;
        Actor_SetSpritePriority(14, 1);
        ((struct YallamInteriorsObject *)Actor_GetById(14))->priorityFlags |= 1;
#if TLA_PATCH_1
        Actor_SetPosition(17, 0, 0);
#endif
        Event_SetFlag(0x301);
      } else {
        Actor_SetDestination(14, 491, 398);
        Actor_WaitForMoveById(14);
        Actor_FaceDirection(14, FACING_SOUTH, 0);
        Actor_SetSingleChildFlags(((struct YallamInteriorsObject *)Actor_GetById(14)), 0);
        Func_02000a60(14, 0);
      }
    } else {
      Event_SetMessageId(message + 2);
      Event_ShowMessage(14, 0);
    }
  }
}

u16 *Func_02000988(s32 item);

/// Sunshine's wife keeps the smithy: she explains the forge, hands over what
/// Sunshine made and takes the payment.
/// "This? This is the blacksmith Sunshine's smithy, the best forge in Weyard!"
void YallamInteriors_TalkToSunshinesWife(s32 actor) {
  if (!Event_GetFlag(0x10f)) {
    Event_SetMessageId((s32)MsgThisThisIs);
    Event_ShowMessage(actor, 0);
  } else if (Event_GetFlag(0x301)) {
    Event_SetMessageId((s32)MsgItTakesOur);
    Event_ShowMessage(actor, 0);
  } else if (Forge_GetItem()) {
    s32 item = Forge_GetItem();
    u32 price = *Func_02000988(item);
    s32 message;

    MessageArgument_Push(item, 2);
    MessageArgument_Push(price, 5);
    message = (s32)MsgHereSThe;
    Event_SetMessageId(message);
    Event_OpenMessage(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) != 0) {
      Event_SetMessageId(message + 1);
      Event_ShowMessage(actor, 0);
    } else if (PartyInventory_FindRoom(item) < 0) {
      Event_SetMessageId(message + 2);
      Event_ShowMessage(actor, 0);
    } else if ((u32)gMainRoutineWork.coins < price) {
      Event_SetMessageId(message + 3);
      Event_ShowMessage(actor, 0);
    } else {
      Event_SetMessageId(message + 4);
      Event_ShowMessage(actor, 0);
      ItemDisplay_ShowItem(item, 3);
      Party_GiveItem(item, 0);
      Forge_SetItem(0);
      Party_AdjustCoins(-price);
    }
  } else {
    Event_SetMessageId((s32)MsgManSunshineWould);
    Event_ShowMessage(actor, 0);
  }
}

/// The scene's start entry: the screen effect 0x209, then the actors of
/// rooms 1 and 2.
s32 Scene_Initialize(void) {
  s32 scene;

  ((struct YallamInteriorsEventWork *)Ram_HeapSlots->eventWork)->screenEffect =
      0x209;
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    scene = party->scene;
  }
  if (scene == (s32)&SceneId_Ov677Room1) {
    if (Event_GetFlag(0x301)) {
      Actor_SetPosition(14, PIXELS(600), PIXELS(360));
      Actor_SetPosition(16, PIXELS(632), PIXELS(346));
      Actor_FaceDirection(16, FACING_SOUTH, 0);
#if TLA_PATCH_1
      Actor_SetPosition(17, 0, 0);
#endif
    } else {
      Func_02000a60(14, 0);
    }
    Actor_SetSingleChildFlags(
        ((struct YallamInteriorsObject *)Actor_GetById(17)), 0);
    Actor_SetSpritePriority(11, 2);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov677Room2) {
    Func_02000a48(0);
    Actor_SetSpritePriority(12, 2);
    ((struct YallamInteriorsObject *)Actor_GetById(12))->collisionFlags |= 4;
    Actor_SetSpritePriority(10, 2);
    ((struct YallamInteriorsObject *)Actor_GetById(10))->collisionFlags |= 4;
    Actor_SetSpritePriority(11, 2);
    ((struct YallamInteriorsObject *)Actor_GetById(11))->collisionFlags |= 4;
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

// clang-format off
u32 gData_02000a98[] = {
    22, 30, 131, 22, 16, 0x1999,
    22, 15, 0x4ccc, 0x80010000, 39, 2,
    4, 0x780000, 0, 0x680000, 1, 4,
    0x800000, 0, 0x5c0000, 1, 0, 20,
    39, 1, 0, 20, 39, 2,
    4, 0x780000, 0, 0x680000, 1, 4,
    0x580000, 0, 0x680000, 1, 0, 20,
    39, 1, 0, 20, 13, 0xffff,
    0xc0010000, 17, 22, 30, 129, 22,
    16, 0x1999, 22, 15, 0x4ccc, 0x80010000,
    39, 2, 4, 0x1b50000, 0, 0x7f0000,
    1, 0, 20, 39, 1, 22,
    3, 0x2000, 0, 60, 39, 2,
    4, 0x1c50000, 0, 0x720000, 1, 0,
    20, 39, 1, 22, 3, 0x2000,
    0, 60, 13, 0xffff, 0xc0010000, 17,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(141),
    SCENE_EXIT_IF(1, 139, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 139, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 139, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 139, 4, EXIT_FLAG_NONE),
    SCENE_EXITS(142),
    SCENE_EXIT_IF(1, 139, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 139, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 139, 5, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gYallamInteriors_Placements[] = {
    {SCENE_TABLE_END},
};
// clang-format on

struct ActorPlacement gYallamInteriors_Room1Placements[] = {
#if !TLA_PATCH_2
    {115, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(136), FACING_SOUTHWEST, 1,
     0},
#else
    {115, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(140), FACING_SOUTHWEST, 1,
     0},
#endif
    {116, CONDITION_ALWAYS, (s32)gData_02000a98, PIXELS(88), 0, PIXELS(104),
     FACING_EAST, 0, 0},
    {114, CONDITION_ALWAYS, 1, PIXELS(424), 0, PIXELS(88), FACING_SOUTHEAST, 1,
     0},
    {128, CONDITION_ALWAYS, 1, PIXELS(454), 0, PIXELS(120), FACING_SOUTHEAST, 1,
     0},
    {134, CONDITION_ALWAYS, 1, PIXELS(294), 0, PIXELS(408), FACING_SOUTH, 1, 0},
    {135, CONDITION_ALWAYS, 2, PIXELS(184), 0, PIXELS(392), FACING_SOUTH, 0, 0},
    {104, CONDITION_ALWAYS, 1, PIXELS(491), 0, PIXELS(398), FACING_SOUTH, 2, 0},
    {107, CONDITION_ALWAYS, 1, PIXELS(560), 0, PIXELS(440), FACING_WEST, 1, 0},
    {115, CONDITION_ALWAYS, 1, PIXELS(600), 0, PIXELS(346), FACING_NORTH, 1, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(506), 0, PIXELS(398), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

#include "text/msg_ids.h"

// clang-format off
struct ActorPlacement gYallamInteriors_Room2Placements[] = {
    {112, CONDITION_ALWAYS, 1, PIXELS(144), 0, PIXELS(188), FACING_EAST, 1, 0},
    {113, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(152), FACING_SOUTH, 1, 0},
    {108, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(405), FACING_SOUTH, 1, 0},
    {109, CONDITION_ALWAYS, 1, PIXELS(112), 0, PIXELS(405), FACING_SOUTH, 1, 0},
    {110, CONDITION_ALWAYS, 1, PIXELS(348), 0, PIXELS(405), FACING_SOUTH, 1, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(408), FACING_SOUTH, 1, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(112), 0, PIXELS(408), FACING_SOUTH, 1, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(348), 0, PIXELS(408), FACING_SOUTH, 1, 0},
    {SCENE_TABLE_END},
};

struct Trigger gYallamInteriors_Triggers[] = {
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGERS_END},
};
// clang-format on

struct Trigger gYallamInteriors_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors09)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(YallamInteriors_AskLikeChildrensSongs)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors13)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors14)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(YallamInteriors_TalkToInnkeeper)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors28)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, EVENT(YallamInteriors_TalkToSunshine)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, EVENT(YallamInteriors_TalkToSunshinesWife)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, EVENT(YallamInteriors_TalkToSunshinesChild)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, EVENT(YallamInteriors_TalkToSunshine)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors11)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors12)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors15)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors16)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors29)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors30)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, EVENT(YallamInteriors_MindReadSunshine)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, EVENT(YallamInteriors_MindReadSunshinesWife)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, EVENT(YallamInteriors_MindReadSunshinesChild)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, EVENT(YallamInteriors_MindReadSunshine)},
    {0x173, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgYallamInteriors40))},
    {TRIGGERS_END},
};

struct Trigger gYallamInteriors_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors17)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors18)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(YallamInteriors_TalkToWeaponMerchant)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(YallamInteriors_TalkToArmorMerchant)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(YallamInteriors_TalkToItemMerchant)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, EVENT(YallamInteriors_TalkToWeaponMerchant)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, EVENT(YallamInteriors_TalkToArmorMerchant)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, EVENT(YallamInteriors_TalkToItemMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors19)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors20)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors22)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors24)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgYallamInteriors26)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, EVENT(YallamInteriors_MindReadWeaponMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, EVENT(YallamInteriors_MindReadArmorMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, EVENT(YallamInteriors_MindReadItemMerchant)},
    {TRIGGERS_END},
};

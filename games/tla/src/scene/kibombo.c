/// @file
/// The scene code of overlay 0x670, in ROM order.

// ---- games/tla/src/scene/kibombo/s0038.c ----

#include "shared/actors.h"
#include "kibombo.h"
#include "event_work.h"
#include "gamepak/far_call/system.h"
#include "module/field/event_commands.h"
#include "module/scene/kibombo.h"
#include "resource.h"

s32 Func_02000038(struct KibomboObject *object) {
  object->sprite->rotation += 0x600;
  return 0;
}

s32 Func_02000048(struct KibomboObject *object) {
  object->field44 = object->x;
  object->field48 = object->y;
  return 0;
}

extern u8 gData_0200635c[];

struct KibomboWork90 {
  u8 unk00[0x18];
  struct KibomboObject *leader;
};

s32 Func_02000054(struct KibomboObject *object) {
  if (gFrameCount & 1) {
    s32 dx;
    s32 dy;

    dx = -0x10000;
    dy = dx;
    dx += ((Random16Far() * 3) >> 16) << 16;
    dy += ((Random16Far() * 3) >> 16) << 16;

    Actor_SetPositionAndResetMotion(object, object->field44 + dx,
                                    object->field48 + dy, object->z);
  }
  return 1;
}

void Func_020000a0(struct KibomboObject *object) {
  if (gFrameCount & 2)
    Actor_SetPartAttribute(object, 10);
  else
    Actor_SetPartAttribute(object, 0);
}

s32 Func_020000c4(struct KibomboObject *object) {
  if (object->timer != 0) {
    object->timer--;
  } else {
    object->timer = ((Random16Far() * 10) >> 16) + 10;
    object->facing = Random16Far();
  }
  return 1;
}

// ---- games/tla/src/scene/kibombo.c ----
// The scene code of overlay 0x670 that its scene tables belong to, in ROM
// order; the rest of the overlay is in kibombo/.

#include "battle.h"
/* VIEW(kibombo): routine Actor_SetPositionAndResetMotion as void
 * Actor_SetPositionAndResetMotion(struct Actor *object, s32 x, s32 y, s32 z);
 * kibombo.h has void Actor_SetPositionAndResetMotion(struct KibomboObject
 * *object, s32 x, s32 y, s32 z); tried as the definition's void
 * Actor_SetPositionAndResetMotion(struct Actor *obj, s32 x, s32 y, s32 z), it
 * does not compile in tla-ja: games/tla/include/actor.h:95: conflicting types
 * for `Actor_SetPositionAndResetMotion'. */
struct Actor;
extern void Actor_SetPositionAndResetMotion__kibombo(
    struct Actor *object, s32 x, s32 y,
    s32 z) __asm__("Actor_SetPositionAndResetMotion");
#define Actor_SetPositionAndResetMotion Actor_SetPositionAndResetMotion__kibombo
/* VIEW(kibombo): routine Actor_SetSpritePriority as void
 * Actor_SetSpritePriority(u32, s32); kibombo.h has void
 * Actor_SetSpritePriority(s32 actor, s32 variant); tried as the definition's
 * void Actor_SetSpritePriority(u32 objectId, s32 priority), it does not compile
 * in tla-ja: games/tla/include/actor.h:106: conflicting types for
 * `Actor_SetSpritePriority'. */
extern void
Actor_SetSpritePriority__kibombo(u32, s32) __asm__("Actor_SetSpritePriority");
#define Actor_SetSpritePriority Actor_SetSpritePriority__kibombo
/* VIEW(kibombo): routine Actor_SetSingleChildFlags as void
 * Actor_SetSingleChildFlags(struct Actor *object, u32 flags); kibombo.h has
 * void Actor_SetSingleChildFlags(struct KibomboObject *object, u32 value);
 * tried as the 35 declarations' void Actor_SetSingleChildFlags(struct
 * FieldEffect *effect, s32 flags), it does not compile in tla-ja:
 * games/tla/include/field/field.h:25: conflicting types for
 * `Actor_SetSingleChildFlags'. */
struct Actor;
extern void Actor_SetSingleChildFlags__kibombo(
    struct Actor *object, u32 flags) __asm__("Actor_SetSingleChildFlags");
#define Actor_SetSingleChildFlags Actor_SetSingleChildFlags__kibombo
/* VIEW(kibombo): routine Animation_SetStateFlags as void
 * Animation_SetStateFlags(struct Actor *object, u32 value); kibombo.h has void
 * Animation_SetStateFlags(struct KibomboObject *object, s32 flags); tried as
 * the definition's void Animation_SetStateFlags(struct Actor *object, u32
 * value), which the object already declares, it does not compile in tla-ja:
 * games/tla/include/field/field.h:58: conflicting types for
 * `Animation_SetStateFlags'. */
struct Actor;
extern void
Animation_SetStateFlags__kibombo(struct Actor *object,
                                 u32 value) __asm__("Animation_SetStateFlags");
#define Animation_SetStateFlags Animation_SetStateFlags__kibombo
/* VIEW(kibombo): routine Actor_SetPartAttribute as void
 * Actor_SetPartAttribute(struct Actor *object, s32 attribute); kibombo.h has
 * void Actor_SetPartAttribute(struct KibomboObject *object, s32 value); tried
 * as the definition's void Actor_SetPartAttribute(struct Actor *object, s32
 * attribute), which the object already declares, it does not compile in tla-ja:
 * games/tla/include/field/field.h:93: conflicting types for
 * `Actor_SetPartAttribute'. */
struct Actor;
extern void Actor_SetPartAttribute__kibombo(
    struct Actor *object, s32 attribute) __asm__("Actor_SetPartAttribute");
#define Actor_SetPartAttribute Actor_SetPartAttribute__kibombo
#include "field.h"
#undef Actor_SetPartAttribute
#undef Animation_SetStateFlags
#undef Actor_SetSingleChildFlags
#undef Actor_SetSpritePriority
#undef Actor_SetPositionAndResetMotion
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "scene_ids.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

u8 *Scene_GetRegions(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Kibombo3)
    return gData_0200635c;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

// ---- games/tla/src/scene/kibombo/events_a.c ----
// Scene code of overlay 0x670, in ROM order, from events_a's offset up to the
// next object the overlay links.

extern u8 gData_02006510[];
extern u8 gData_020066d8[];
extern u8 gData_020069a8[];
extern u8 gData_02006a98[];
extern u8 gData_02006cd8[];
extern u8 gData_020064f8[];
extern u8 gData_02006ef4[];
extern u8 gData_020070d4[];
extern u8 gData_02007434[];
extern u8 gData_020074dc[];
extern u8 gData_02007698[];
extern u8 gData_02006ee8[];

u8 *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Kibombo1)
    return gData_02006510;
  if (scene == (s32)&SceneId_Kibombo4)
    return gData_020066d8;
  if (scene == (s32)&SceneId_Kibombo2)
    return gData_020069a8;
  if (scene == (s32)&SceneId_Kibombo5)
    return gData_02006a98;
  if (scene == (s32)&SceneId_Kibombo3) {
    if (GameFlag_Test(0x8ee))
      gData_02006cd8[22] = 0;
    return gData_02006cd8;
  }
  return gData_020064f8;
}

u8 *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Kibombo1)
    return gData_02006ef4;
  if (scene == (s32)&SceneId_Kibombo4)
    return gData_020070d4;
  if (scene == (s32)&SceneId_Kibombo2)
    return gData_02007434;
  if (scene == (s32)&SceneId_Kibombo5)
    return gData_020074dc;
  if (scene == (s32)&SceneId_Kibombo3)
    return gData_02007698;
  return gData_02006ee8;
}

void Func_02000228(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgItLooksLikeThere);
  Event_AskYesNoAndShowResponse(15, 0);
  Event_End();
}

void Func_0200024c(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgYouDidnT);
  Event_AskYesNoAndShowResponse(23, 0);
  Event_End();
}

void Func_02000270(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgNowThatAkafubu);
  Event_ShowMessageAndWait(24, 0);
  Actor_SetAngleToward(24, gMainRoutineWork.currentOwner, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(24, 0);
  Kibombo_ObjTurn(24, FACING_SOUTHEAST + FACING_STEP);
  Event_End();
}

void Func_020002c4(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgAreYouGoing);
  Event_AskYesNoAndShowResponse(12, 0);
  Event_End();
}

DEFINE_LEADER_IS_FACING_NORTH(Leader_IsFacingNorth)

void Func_02000318(void) {
  if (Leader_IsFacingNorth()) {
    Func_02005c84(17, 20);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgTheWeaponsI);
    Event_ShowMessageAndWait(20, 0);
    Event_End();
  }
}

void Func_02000350(void) {
  if (Leader_IsFacingNorth()) {
    Func_02005c84(18, 21);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgTheOnlyDefenses);
    Event_ShowMessageAndWait(21, 0);
    Event_End();
  }
}

void Func_02000388(void) {
  if (Leader_IsFacingNorth()) {
    Func_02005c84(19, 22);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgIMSorryBut);
    Event_ShowMessageAndWait(22, 0);
    Event_End();
  }
}

void Func_020003c0(void) {
  if (Leader_IsFacingNorth()) {
    Func_02005c94(6, 23);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgDonTWorryAbout);
    Event_ShowMessageAndWait(23, 0);
    Event_End();
  }
}

void Func_020003f8(void) {
  if (Leader_IsFacingNorth()) {
    Func_02005c8c(25);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgKibomboSPeace);
    Event_ShowMessageAndWait(25, 0);
    Event_End();
  }
}

void Func_0200042c(void) {
  if (Kibombo_FlagCheck(0x9df)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_SetAngleToward(36, gMainRoutineWork.currentOwner, 0);
    Event_Wait(10);
    Func_02005c8c(36);
    Kibombo_ObjArm(36, FACING_NORTHWEST + FACING_STEP, 0);
    Event_End();
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgOhOhMy);
    Event_ShowMessageThenWait(36, 0, 40);
    Event_ShowMessageAndWait(36, 0);
    Actor_SetAngleToward(36, gMainRoutineWork.currentOwner, 0);
    Event_Wait(10);
    Func_02005c8c(36);
    Kibombo_ObjArm(36, FACING_NORTHWEST + FACING_STEP, 0);
    GameFlag_SetBit(0x9df);
    Event_End();
  }
}

void Func_020004dc(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgOhMyBack);
  Event_ShowMessageAndWait(16, 0);
  Event_End();
}

void Func_02000500(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgISwearThat);
  Event_ShowMessageAndWait(16, 0);
  Event_End();
}

void Func_02000524(s32 show) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Kibombo2 || scene == (s32)&SceneId_Kibombo5)
    Func_02000590(show, 15, 2);
  else if (scene == (s32)&SceneId_Kibombo3)
    Func_02000590(show, 26, 2);
}

void Func_02000568(void) {
  Func_02000524(1);
  GameFlag_SetBit(0x200);
}

void Func_0200057c(void) {
  Func_02000524(0);
  GameFlag_ClearBit(0x200);
}

void Func_02000590(s32 show, s32 first, u32 count) {
  u32 i;

  for (i = 0; i < count; i++) {
    struct KibomboObject *actor = Actor_GetById(first + i);

    if (show)
      actor->priorityFlags &= ~0x10;
    else
      actor->priorityFlags |= 0x10;
  }
}

void Func_020005d0(void) { Func_02005c6c(gData_02006324); }

void Func_020005e0(void) {
  Map_CopyCellPictureIndicesRect(34, 33, 23, 18, 1, 1);
  Kibombo_BlendStart(34, 33, 1, 1, 23, 18);
  GameFlag_SetBit(0x86a);
}

void Func_02000618(s32 mode) {
  if (mode == 1)
    Func_020005e0();
}

void Func_02000624(void) {
  Map_CopyCellPictureIndicesRect(36, 34, 16, 7, 1, 1);
  Kibombo_BlendStart(36, 34, 1, 1, 16, 7);
  GameFlag_SetBit(0x204);
}

void Func_0200065c(s32 mode) {
  if (mode == 1)
    Func_02000624();
}

void Func_02000668(void) {
  Map_CopyCellPictureIndicesRect(38, 34, 16, 11, 1, 1);
  Kibombo_BlendStart(38, 34, 1, 1, 16, 11);
  GameFlag_SetBit(0x205);
}

void Func_020006a0(s32 mode) {
  if (mode == 1)
    Func_02000668();
}

void Func_020006ac(void) {
  Map_CopyCellPictureIndicesRect(36, 36, 14, 9, 1, 1);
  Kibombo_BlendStart(36, 36, 1, 1, 14, 9);
  GameFlag_SetBit(0x206);
}

void Func_020006e4(s32 mode) {
  if (mode == 1)
    Func_020006ac();
}

void Func_020006f0(void) {
  Map_CopyCellPictureIndicesRect(38, 36, 18, 9, 1, 1);
  Kibombo_BlendStart(38, 36, 1, 1, 18, 9);
  GameFlag_SetBit(0x207);
}

void Func_02000728(s32 mode) {
  if (mode == 1)
    Func_020006f0();
}

void Func_02000734(void) {
  struct KibomboObject *object = Actor_GetById(11);

  if (object->x >> 20 == 6 && object->z >> 20 == 18) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_GetById(11)->motionFlags = 3;
    object->motionFlags = 3;
    do
      wait(1);
    while (object->velocityY != 0);
    Sound_Play(188);
    wait(10);
    object->motionFlags = 0;
    Actor_GetById(11)->motionFlags = 0;
    Kibombo_BlendStart(5, 18, 1, 1, 6, 18);
    GameFlag_SetBit(0x9ea);
    if (!GameFlag_Test(0x8f4))
      Func_02001318();
    Event_End();
  }
}

void Func_020007c8(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Kibombo_CamMove(-1, -1, -1, 0);
  Func_080d4568()->motionFlags = 0;
  Camera_SetSpeedParameters(0x9999, 0x1333);
  Camera_MoveWithinMapBounds(PIXELS(88), -1, PIXELS(392), 1);
  Kibombo_ObjSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Kibombo_ObjWalk(gMainRoutineWork.currentOwner, 72, 392);
  Kibombo_ObjWalk(gMainRoutineWork.currentOwner, 88, 392);
  Actor_ArmCallback(7, FACING_SOUTHWEST, 10);
  Event_SetMessageId((s32)MsgWhWhoAre);
  Event_ShowMessageAndWait(7, 0);
  Actor_MoveToObject(26, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(5, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(6, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_SetSpritePriority(26, 2);
  Actor_SetSpritePriority(5, 2);
  Actor_SetSpritePriority(6, 2);
  Kibombo_ObjSpeed(26, 0xcccc, 0x6666);
  Kibombo_ObjSpeed(5, 0xcccc, 0x6666);
  Kibombo_ObjSpeed(6, 0xcccc, 0x6666);
  Actor_SetMovementTypeById(26, gData_02005d18);
  Actor_SetMovementTypeById(5, gData_02005d5c);
  Actor_SetMovementTypeAndWaitById(6, gData_02005da0);
  Event_ShowMessageAndWait(7, 0);
  if (GameFlag_Test(0x911))
    Func_02000978();
  else
    Func_02000b2c();
  Func_02000d74();
  Kibombo_ObjSpeed(26, 0x19999, 0xcccc);
  Kibombo_ObjSpeed(5, 0x19999, 0xcccc);
  Kibombo_ObjSpeed(6, 0x19999, 0xcccc);
  Actor_SetMovementTypeById(5, gData_02005e54);
  Actor_SetMovementTypeById(6, gData_02005e54);
  Actor_SetMovementTypeAndWaitById(26, gData_02005e54);
  GameFlag_SetBit(0x8f3);
  Event_End();
}

void Func_02000978(void) {
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_WEST, 20);
  Event_SetMessageId((s32)MsgItLooksLikeHas);
  FieldMessage_ShowAtActor(6, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Kibombo_ObjArm(5, FACING_SOUTH, 10);
    Actor_SetModeById(5, 3);
    Event_ShowMessageAndWait(5, 0);
    ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
  } else {
    Kibombo_ObjArm(5, FACING_SOUTH, 10);
    Actor_SetModeById(5, 4);
    ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
    Event_ShowMessageAndWait(5, 0);
  }
  Func_080d47b4(7, 0x101, 20);
  Event_ShowMessageAndWait(7, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 0);
  Kibombo_ObjArm(26, FACING_NORTHWEST + FACING_STEP, 0);
  Kibombo_ObjArm(5, FACING_SOUTHWEST, 0);
  Kibombo_ObjArm(6, FACING_NORTHEAST, 40);
  Kibombo_ObjArm(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  Kibombo_ObjArm(26, FACING_NORTH + FACING_STEP, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Kibombo_ObjArm(6, FACING_NORTHEAST, 20);
  Event_ShowMessageAndWait(7, 0);
  Func_080d47b4(26, 0x100, 0);
  Event_ShowMessageAndWait(26, 0);
  Event_ShowMessageAndWait(7, 0);
  Event_ShowMessageAndWait(6, 0);
  Kibombo_ObjArm(5, FACING_SOUTH, 40);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Event_ShowMessageAndWait(5, 0);
  Event_ShowMessageAndWait(7, 0);
  Kibombo_ObjEmote(6, 0x103, 0);
  Actor_Jump(6, 2, 10);
  Actor_Jump(6, 4, 10);
  Event_ShowMessageAndWait(6, 0);
  Kibombo_ObjEmote(5, 0x103, 10);
  Event_ShowMessageAndWait(5, 0);
  Func_080d47b4(7, 0x103, 10);
  Event_ShowMessageAndWait(7, 0);
}

void Func_02000b2c(void) {
  Event_SetMessageId((s32)MsgHeSeemsTo);
  Event_ShowMessageAndWait(6, 0);
  Kibombo_ObjArm(5, FACING_SOUTH, 10);
  Actor_SetModeById(5, 4);
  Event_ShowMessageAndWait(5, 0);
  Func_080d47b4(7, 0x101, 20);
  Event_ShowMessageAndWait(7, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 0);
  Kibombo_ObjArm(26, FACING_NORTHWEST + FACING_STEP, 0);
  Kibombo_ObjArm(5, FACING_SOUTHWEST, 0);
  Kibombo_ObjArm(6, FACING_NORTHEAST, 40);
  Kibombo_ObjArm(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  Kibombo_ObjArm(26, FACING_NORTH + FACING_STEP, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Kibombo_ObjArm(6, FACING_NORTHEAST, 0);
  FieldEffect_SetAttached(7, 0x102);
  Event_Wait(20);
  Event_ShowMessageAndWait(7, 0);
  Event_ShowMessageThenWait(26, 0, 20);
  Event_ShowMessageAndWait(7, 0);
  Func_080d47b4(6, 0x109, 0);
  Event_ShowMessageAndWait(6, 0);
  Kibombo_ObjArm(5, FACING_SOUTHWEST, 40);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Event_ShowMessageAndWait(5, 0);
  Event_ShowMessageAndWait(7, 0);
  Kibombo_ObjEmote(6, 0x103, 0);
  Actor_Jump(6, 2, 10);
  Actor_Jump(6, 4, 10);
  Event_ShowMessageAndWait(6, 0);
  Kibombo_ObjEmote(5, 0x103, 10);
  Event_ShowMessageAndWait(5, 0);
  Func_080d47b4(7, 0x103, 10);
  Event_ShowMessageAndWait(7, 0);
}

void Func_02000c90(void) {
  struct KibomboObject *leader;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Camera_SetSpeedParameters(0x26666, 0x4ccc);
  Camera_MoveWithinMapBounds(PIXELS(280), -1, PIXELS(88), 1);
  Func_02005bb4();
  Event_Wait(20);
  Event_SetMessageId((s32)MsgYouVeCome);
  Event_ShowMessageAndWait(0x8007, 0);
  Kibombo_ObjTurn(7, FACING_WEST);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Motion_SetModeAndWaitAnimation(7, 3);
  Kibombo_ObjSpeed(7, 0x19999, 0xcccc);
  Actor_SetModeById(7, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_ResetAndSetPosition(7, *(s16 *)((u8 *)leader + 10),
                              *(s16 *)((u8 *)leader + 18));
  Actor_WaitForMoveById(7);
  Actor_SetHorizontalPositionWithTerrain(7, 0, 0);
  GameFlag_ClearBit(0x92f);
  GameFlag_SetBit(0x92e);
  Event_PrepareObjectAndApplyValue(7, 0);
  Event_End();
}

void Func_02005c7c(void);
void Func_02002eb0(void);
void Func_020032b0(void);
void Func_020032c8(void);
void Func_02003610(void);

void Func_02000d74(void) {
  Func_020032c8();
  Sound_Play(0);
  Event_ShowMessageAndWait(17, 0);
  Actor_ArmCallback(7, FACING_EAST, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_ArmCallback(26, FACING_EAST, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Actor_ArmCallback(6, FACING_EAST, 10);
  Camera_SetSpeedParameters(0x4cccc, 0x9999);
  Kibombo_CamMove(PIXELS(264), -1, PIXELS(388), 1);
  Func_02005bb4();
  Event_Wait(40);
  Kibombo_ObjArm(17, FACING_SOUTHEAST + FACING_STEP, 20);
  Motion_SetModeAndWaitAnimation(17, 3);
  Event_ShowMessageAndWait(0x4011, 0);
  Sound_Play(26);
  Actor_SetSingleChildFlags(Actor_GetById(27), 0);
  Actor_GetById(27)->motionFlags = 0;
  Actor_SetPositionAndResetMotion(Actor_GetById(27), PIXELS(265), PIXELS(24),
                                  PIXELS(344));
  Camera_SetSpeedParameters(0x6666, 0xccc);
  Camera_MoveWithinMapBounds(PIXELS(264), -1, PIXELS(340), 1);
  Kibombo_ObjSpeed(17, 0x3333, 0x1999);
  Kibombo_ObjWalk(17, 264, 374);
  Kibombo_TaskAdd(Func_02002eb0, 0x480);
  Event_Wait(20);
  Task_RemoveFar(Func_02002ccc);
  Func_020032e0();
  KIBOMBO_TASK_ADD(Func_02002ccc, 0x480);
  Event_ShowMessageAndWait(7, 0);
  Event_ShowMessageAndWait(17, 0);
  Sound_Play(11);
  Func_02003610();
  Event_Wait(160);
  Event_ShowMessageThenWait(17, 0, 80);
  Sound_Play(0x240);
  Func_02003640(1, 1);
  Event_Wait(20);
  Event_ShowMessageThenWait(17, 0, 80);
  Sound_Play(161);
  Func_02003640(0, 1);
  Event_Wait(20);
  FieldEffect_SetAttached(17, 0x102);
  Event_Wait(20);
  Event_ShowMessageThenWait(17, 0, 20);
  Task_RemoveFar(Func_02002eb0);
  wait(1);
  Actor_SetHorizontalPositionWithTerrain(27, 0, 0);
  Kibombo_ObjArm(17, FACING_NORTH + FACING_STEP, 0);
  if (gData_02007950 != 0) {
    if (gData_02007950->y > 0x240000) {
    wait:
      Kibombo_ObjArm(17, FACING_NORTH + FACING_STEP, 0);
      gData_02007950->y += 0xffffc000;
      wait(1);
      if (gData_02007950->y > 0x240000)
        goto wait;
    }
  }
  Kibombo_ObjTurn(17, FACING_SOUTHEAST + FACING_STEP);
  Event_ShowMessageAndWait(17, 0);
  Camera_SetSpeedParameters(0x4cccc, 0x9999);
  Camera_MoveWithinMapBounds(PIXELS(264), PIXELS(16), PIXELS(420), 1);
  Kibombo_ObjSpeed(17, 0x19999, 0xcccc);
  Actor_SetPositionAndReset(17, 264, 392);
  Event_Wait(20);
  Event_ShowMessageAndWait(0x8011, 0);
  Sound_Play(148);
  Sound_Play(11);
  Func_020032b0();
  Event_Wait(80);
  Camera_MoveWithinMapBounds(PIXELS(88), PIXELS(32), PIXELS(392), 1);
  Func_02005bb4();
  Event_Wait(40);
  Event_ShowMessageAndWait(7, 0);
  Kibombo_ObjEmoteWait(7, 0x102);
  Event_ShowMessageAndWait(7, 0);
  Motion_SetModeAndWaitAnimation(7, 4);
  Event_ShowMessageAndWait(7, 0);
  Actor_SetSpritePriority(7, 2);
  Kibombo_ObjSpeed(7, 0x19999, 0xcccc);
  Kibombo_ObjWalk(7, 152, 386);
  Kibombo_ObjArm(7, FACING_NORTHEAST, 40);
  Kibombo_ObjArm(7, FACING_SOUTHEAST, 20);
  Kibombo_ObjArm(7, FACING_NORTHEAST, 20);
  Kibombo_ObjArm(7, FACING_SOUTHEAST, 40);
  Event_ShowMessageAndWait(7, 0);
  Kibombo_ObjWalk(7, 124, 356);
  Kibombo_ObjArm(7, FACING_NORTHEAST, 20);
  Kibombo_ObjArm(7, FACING_NORTHWEST, 20);
  Kibombo_ObjArm(7, FACING_NORTHEAST, 20);
  Event_ShowMessageAndWait(0x2007, 0);
  Kibombo_ObjArm(5, FACING_SOUTHEAST, 0);
  Event_ShowMessageAndWait(5, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 40);
  Kibombo_ObjSpeed(26, 0xcccc, 0x6666);
  Actor_ResetAndSetPositionInMode2(26, 104, 368);
  Event_Wait(20);
  Camera_SetSpeedParameters(0xcccc, 0x1999);
  Camera_MoveWithinMapBounds(PIXELS(111), -1, PIXELS(356), 1);
  Actor_WaitForMoveById(26);
  Actor_SetModeById(26, 1);
  Kibombo_ObjArm(26, FACING_NORTH + FACING_STEP, 40);
  Func_080d47b4(26, 0x100, 20);
  Event_ShowMessageAndWait(26, 0);
  Kibombo_ObjArm(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Kibombo_ObjArm(5, FACING_NORTH, 0);
  Kibombo_ObjArm(6, FACING_NORTH, 0);
  Kibombo_ObjArm(26, FACING_SOUTH + FACING_STEP, 20);
  Kibombo_MessageOpen(0x601a, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Event_ShowMessageAndWait(0x601a, 0);
    ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
  } else {
    ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
    Event_ShowMessageAndWait(0x601a, 0);
    Func_02005c7c();
  }
}

void Func_02001218(void) {
  if (!Kibombo_FlagCheck(0x92e)) {
    Import_EventSetExitRequest(5);
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_MoveToObject(7, gMainRoutineWork.currentOwner);
  Actor_SetSpritePriority(
      7, Actor_GetById(gMainRoutineWork.currentOwner)->sprite->priority);
  wait(1);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 0);
  Kibombo_ObjSpeed(7, 0x19999, 0xcccc);
  Actor_SetPositionAndReset(7, 62, 444);
  Actor_ArmCallbackAndRefresh(7, FACING_SOUTHEAST);
  Event_SetMessageId((s32)MsgNowThatIKnow);
  Event_ShowMessageAndWait(7, 0);
  Actor_GetById(7)->priorityFlags |= 1;
  Kibombo_ObjWalk(7, 56, 420);
  Kibombo_ObjWalk(7, 40, 344);
  Actor_SetHorizontalPositionWithTerrain(7, 0, 0);
  Kibombo_ObjPlace(gMainRoutineWork.currentOwner, 80, 510);
  Event_Wait(10);
  Import_EventSetExitRequest(5);
  GameFlag_ClearBit(0x92e);
  GameFlag_SetBit(0x92f);
  Party_RemoveMemberRestored(7);
  Event_End();
}

// ---- games/tla/src/scene/kibombo/s1318.c ----
// Scene code of overlay 0x670, in ROM order, from s1318's offset up to the next
// object the overlay links.

#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
/* VIEW(s1318): routine Task_Add as s32 Task_Add(TaskFunc func, s32 order);
 * kibombo.h has void Task_Add(void(*func) (void), s32 priority); tried as the
 * definition's s32 Task_Add(TaskFunc func, s32 order), which the object already
 * declares, it does not compile in tla-ja: games/tla/include/system/task.h:55:
 * conflicting types for `Task_Add'. */
#define Task_Add Task_Add__s1318
/* VIEW(s1318): routine Actor_GetById as struct Actor *Actor_GetById(u32
 * objectId); kibombo.h has struct KibomboObject *Actor_GetById(s32 id); tried
 * as the definition's struct Actor *Actor_GetById(u32 objectId), which the
 * object already declares, it does not compile in tla-ja:
 * games/tla/include/field/event.h:37: conflicting types for `Actor_GetById'. */
struct Actor;
extern struct Actor *
Actor_GetById__s1318(u32 objectId) __asm__("Actor_GetById");
#define Actor_GetById Actor_GetById__s1318
/* VIEW(s1318): routine Actor_SetMoveTarget as void Actor_SetMoveTarget(struct
 * Actor *actor, s32 x, s32 y, s32 z); kibombo.h has void
 * Actor_SetMoveTarget(struct KibomboObject *object, s32 x, s32 y, s32 z); tried
 * as the definition's void Actor_SetMoveTarget(struct Actor *object, s32 x, s32
 * y, s32 z), it does not compile in tla-ja: games/tla/include/field/event.h:59:
 * conflicting types for `Actor_SetMoveTarget'. */
struct Actor;
extern void Actor_SetMoveTarget__s1318(struct Actor *actor, s32 x, s32 y,
                                       s32 z) __asm__("Actor_SetMoveTarget");
#define Actor_SetMoveTarget Actor_SetMoveTarget__s1318
/* VIEW(s1318): routine Import_ActorCreate as struct Actor
 * *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z); kibombo.h has struct
 * KibomboObject *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z); tried as
 * the rest of the object declares it, it does not compile in tla-ja:
 * games/tla/include/field/event.h:60: conflicting types for
 * `Import_ActorCreate'. */
struct Actor;
extern struct Actor *
Import_ActorCreate__s1318(s32 type, s32 x, s32 y,
                          s32 z) __asm__("Import_ActorCreate");
#define Import_ActorCreate Import_ActorCreate__s1318
/* VIEW(s1318): routine Actor_SetSpritePriority as void
 * Actor_SetSpritePriority(u32 actorId, s32 priority); kibombo.h has void
 * Actor_SetSpritePriority(s32 actor, s32 variant); tried as the definition's
 * void Actor_SetSpritePriority(u32 objectId, s32 priority), it does not compile
 * in tla-ja: games/tla/include/field/event.h:105: conflicting types for
 * `Actor_SetSpritePriority'. */
extern void
Actor_SetSpritePriority__s1318(u32 actorId,
                               s32 priority) __asm__("Actor_SetSpritePriority");
#define Actor_SetSpritePriority Actor_SetSpritePriority__s1318
/* VIEW(s1318): routine Actor_SetPositionAndResetMotion as void
 * Actor_SetPositionAndResetMotion(struct Actor *object, s32 x, s32 y, s32 z);
 * kibombo.h has void Actor_SetPositionAndResetMotion(struct KibomboObject
 * *object, s32 x, s32 y, s32 z); tried as the definition's void
 * Actor_SetPositionAndResetMotion(struct Actor *obj, s32 x, s32 y, s32 z), it
 * does not compile in tla-ja: games/tla/include/field/event.h:110: conflicting
 * types for `Actor_SetPositionAndResetMotion'. */
struct Actor;
extern void Actor_SetPositionAndResetMotion__s1318(
    struct Actor *object, s32 x, s32 y,
    s32 z) __asm__("Actor_SetPositionAndResetMotion");
#define Actor_SetPositionAndResetMotion Actor_SetPositionAndResetMotion__s1318
/* VIEW(s1318): routine Actor_SetPartAttribute as void
 * Actor_SetPartAttribute(struct Actor *object, s32 attribute); kibombo.h has
 * void Actor_SetPartAttribute(struct KibomboObject *object, s32 value); tried
 * as the definition's void Actor_SetPartAttribute(struct Actor *object, s32
 * attribute), which the object already declares, it does not compile in tla-ja:
 * games/tla/include/field/event.h:112: conflicting types for
 * `Actor_SetPartAttribute'. */
struct Actor;
extern void
Actor_SetPartAttribute__s1318(struct Actor *object,
                              s32 attribute) __asm__("Actor_SetPartAttribute");
#define Actor_SetPartAttribute Actor_SetPartAttribute__s1318
/* VIEW(s1318): routine Script_WaitForEventTimeout as void
 * Script_WaitForEventTimeout(struct Actor *actor); kibombo.h has void
 * Script_WaitForEventTimeout(struct KibomboObject *object); tried as the
 * definition's void Script_WaitForEventTimeout(void *fieldObject), it does not
 * compile in tla-ja: games/tla/include/field/event.h:115: conflicting types for
 * `Script_WaitForEventTimeout'. */
struct Actor;
extern void Script_WaitForEventTimeout__s1318(struct Actor *actor) __asm__(
    "Script_WaitForEventTimeout");
#define Script_WaitForEventTimeout Script_WaitForEventTimeout__s1318
/* VIEW(s1318): routine Func_080d4568 as struct Actor *Func_080d4568(void);
 * kibombo.h has struct KibomboObject *Func_080d4568(void); tried as the
 * definition's struct Actor *Func_080d4568(void), which the object already
 * declares, it does not compile in tla-ja: games/tla/include/field/event.h:124:
 * conflicting types for `Func_080d4568'. */
extern struct Actor *Func_080d4568__s1318(void) __asm__("Func_080d4568");
#define Func_080d4568 Func_080d4568__s1318
#include "event_inline.h"
#undef Func_080d4568
#undef Script_WaitForEventTimeout
#undef Actor_SetPartAttribute
#undef Actor_SetPositionAndResetMotion
#undef Actor_SetSpritePriority
#undef Import_ActorCreate
#undef Actor_SetMoveTarget
#undef Actor_GetById
#undef Task_Add
extern s32 Task_Add__s1318(TaskFunc func, s32 order) __asm__("Task_Add");

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)

void Func_02005a8c(void);
extern const u8 gData_02005e88[];
extern const u8 gData_02005eec[];
extern const u8 gData_02005f14[];
extern const u8 gData_02005f58[];
extern const u8 gData_02005f9c[];
extern const u8 gData_02005fe0[];

/// A scene in Kibombo: actor 7 speaks, the camera pans, actors 26, 5, 6 and 7
/// move, turn and speak around the controlled character, two yes-or-no
/// questions count up the event work's halfword 0x1c4, and the scene ends with
/// Func_02005a8c. Each call goes through the call layer (call.h).
void Func_02001318(void) {
  Actor_ArmCallback(7, FACING_NORTH, 0);
  Event_SetMessageId(MsgBlank7);
  Event_ShowMessage(0x2007, 0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  ((struct Actor *)((s32)Func_080d4568__s1318()))->motionFlags = 0;
  FieldCamera_SetSpeed(0x33333, 0x6666);
  FieldCamera_MoveTo(PIXELS(96), -1, PIXELS(340), 1);
  Func_02005bb4();
  Actor_FaceDirection(7, FACING_NORTHWEST, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 0);
  FieldEffect_SetAttached(7, 258);
  Event_Wait(20);
  Event_AskYesNo(0x2007, 0);
  FieldCamera_SetSpeed(0x6666, 0xccc);
  FieldCamera_MoveTo(PIXELS(96), -1, PIXELS(372), 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x13333, 0x9999);
  Actor_SetSpeed(7, 0x13333, 0x9999);
  Actor_SetMovementTypeById(7, gData_02005eec);
  Actor_SetMovementTypeAndWaitById(gMainRoutineWork.currentOwner,
                                   gData_02005e88);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_SetSpeed(26, 0xcccc, 0x6666);
  Actor_SetSpeed(5, 0xcccc, 0x6666);
  Actor_SetSpeed(6, 0xcccc, 0x6666);
  Actor_SetPosition(26, PIXELS(56), PIXELS(372));
  Actor_SetPosition(5, PIXELS(56), PIXELS(372));
  Actor_SetHorizontalPositionWithTerrain(6, PIXELS(56), PIXELS(372));
  wait(1);
  Actor_SetMovementTypeById(26, gData_02005f14);
  Actor_SetMovementTypeById(5, gData_02005f58);
  Actor_SetMovementTypeAndWaitById(6, gData_02005f9c);
  Event_Wait(20);
  Event_ShowMessage(0x2007, 0);
  Func_080d47b4(5, 0x100, 20);
  Event_ShowMessage(0x1005, 0);
  Motion_SetModeAndWaitAnimation(7, 3);
  Event_ShowMessage(0x2007, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Actor_SetModeById(26, 3);
  Actor_SetModeById(5, 3);
  Motion_SetModeAndWaitAnimation(6, 3);
  Actor_SetModeById(7, 3);
  Event_ShowMessage(0x2007, 0);
  Actor_ShowEmote(6, 257, 0);
  Event_ShowMessage(0x1006, 0);
  Actor_FaceDirection(7, FACING_SOUTHWEST, 0);
  Event_ShowMessage(0x2007, 0);
  Actor_ShowEmote(26, 257, 0);
  Event_ShowMessage(0x101a, 0);
  Actor_FaceDirection(26, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(6, FACING_SOUTH, 40);
  Event_ShowMessage(0x101a, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 257, 0);
  Actor_ShowEmote(5, 257, 0);
  Actor_ShowEmote(6, 257, 40);
  Actor_FaceDirection(26, FACING_NORTH + FACING_STEP, 0);
  Event_ShowMessage(0x101a, 0);
  Actor_SetModeById(7, 3);
  Event_ShowMessage(0x2007, 0);
  Actor_ShowEmote(5, 257, 0);
  Actor_ArmCallback(5, FACING_EAST, 20);
  Event_ShowMessage(0x1005, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_ArmCallback(6, FACING_EAST, 0);
  Actor_ArmCallback(26, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(7, FACING_WEST);
  Actor_SetModeById(7, 4);
  Event_ShowMessage(0x2007, 0);
  Actor_FaceDirection(26, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 257, 0);
  Actor_ShowEmote(5, 257, 0);
  Actor_ShowEmote(6, 257, 40);
  Func_080d47b4(7, 261, 20);
  Event_ShowMessage(0x2007, 0);
  Actor_SetModeById(6, 3);
  Event_ShowMessageAndWait(0x1006, 0);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHEAST);
  Actor_SetModeById(7, 4);
  Event_ShowMessage(0x2007, 0);
  Event_ShowMessage(0x1005, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Event_ShowMessage(0x2007, 0);
  Actor_SetModeById(7, 4);
  Event_ShowMessage(0x2007, 0);
  Actor_FaceDirectionAndWait(26, FACING_NORTHWEST + FACING_STEP);
  Event_ShowMessage(0x101a, 0);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHWEST);
  Event_ShowMessageAndWait(0x2007, 0);
  Actor_ArmCallbackAndRefresh(26, FACING_EAST);
  Actor_SetModeById(26, 3);
  Event_ShowMessage(0x101a, 0);
  Motion_SetModeAndWaitAnimation(7, 3);
  Func_080d47b4(26, 264, 20);
  Event_ShowMessage(0x101a, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(6, FACING_SOUTH, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 257, 0);
  Actor_ShowEmote(5, 257, 0);
  Func_080d47b4(6, 257, 40);
  Actor_FaceDirection(26, FACING_NORTH + FACING_STEP, 0);
  Event_OpenMessage(0x101a, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 1) {
    Event_ShowMessageAndWait(0x101a, 0);
    Motion_SetModeAndWaitAnimation(5, 4);
    Event_ShowMessageAndWait(0x1005, 0);
    *(u16 *)(EVENT_WORK + 0x1c4) += 1;
  } else {
    *(u16 *)(EVENT_WORK + 0x1c4) += 2;
    Motion_SetModeAndWaitAnimation(26, 3);
    Event_ShowMessageAndWait(0x101a, 0);
  }
  Actor_FaceDirection(26, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_ArmCallback(26, FACING_EAST, 20);
  Motion_SetModeAndWaitAnimation(26, 3);
  Event_ShowMessage(0x101a, 0);
  Event_ShowMessage(0x2007, 0);
  Actor_FaceDirection(26, FACING_NORTHWEST + FACING_STEP, 0);
  Event_ShowMessage(0x101a, 0);
  Actor_ShowEmote(6, 0x100, 20);
  Event_ShowMessage(0x1006, 0);
  Motion_SetModeAndWaitAnimation(26, 3);
  Event_ShowMessage(0x101a, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Actor_ArmCallback(6, FACING_EAST, 0);
  Actor_ArmCallback(26, FACING_EAST, 20);
  Motion_SetModeAndWaitAnimation(7, 3);
  Actor_SetModeById(26, 4);
  Event_ShowMessage(0x101a, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(6, FACING_SOUTH, 0);
  Actor_FaceDirection(7, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(26, FACING_NORTHWEST + FACING_STEP, 0);
  Func_080d47b4(26, 261, 20);
  Event_ShowMessage(0x101a, 0);
  Func_080d47b4(7, 262, 40);
  Event_ShowMessage(0x2007, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Actor_ArmCallback(6, FACING_EAST, 0);
  Actor_ArmCallback(26, FACING_EAST, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x100, 0);
  Actor_ShowEmote(5, 0x100, 0);
  Actor_ShowEmote(6, 0x100, 40);
  Actor_FaceDirection(7, FACING_WEST, 20);
  Actor_SetModeById(7, 4);
  Event_ShowMessage(0x2007, 0);
  Motion_SetModeAndWaitAnimation(26, 3);
  Event_ShowMessage(0x101a, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(6, FACING_SOUTH, 0);
  Actor_FaceDirection(7, FACING_SOUTH + FACING_STEP, 20);
  Motion_SetModeAndWaitAnimation(5, 4);
  Event_ShowMessage(0x1005, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Actor_ShowEmote(7, 257, 40);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Event_ShowMessage(0x1005, 0);
  Actor_FaceDirectionAndWait(26, FACING_NORTHWEST + FACING_STEP);
  Actor_SetModeById(26, 4);
  Event_ShowMessage(0x101a, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_ShowEmote(6, 257, 20);
  Event_ShowMessage(0x1006, 0);
  Actor_FaceDirection(7, FACING_SOUTHWEST, 0);
  Actor_SetModeById(7, 4);
  Event_ShowMessage(0x2007, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_ArmCallback(6, FACING_EAST, 0);
  Actor_ArmCallback(26, FACING_EAST, 0);
  Actor_ShowEmote(5, 0x100, 20);
  Event_ShowMessage(0x1005, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Event_ShowMessage(0x2007, 0);
  Actor_SetModeById(26, 3);
  Event_ShowMessage(0x101a, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(6, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHWEST);
  Motion_SetModeAndWaitAnimation(7, 3);
  Event_ShowMessage(0x2007, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_ArmCallback(6, FACING_EAST, 40);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_ArmCallback(6, FACING_EAST, 20);
  Event_ShowMessage(0x1006, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Actor_SetAttachedEffect(7, 258);
  Event_Wait(20);
  Event_ShowMessage(0x2007, 0);
  Actor_ShowEmote(6, 257, 0);
  Event_ShowMessage(0x1006, 0);
  Actor_FaceDirection(26, FACING_NORTHWEST + FACING_STEP, 10);
  Actor_SetModeById(26, 4);
  Event_ShowMessage(0x101a, 0);
  Motion_SetModeAndWaitAnimation(7, 3);
  Actor_ArmCallback(26, FACING_EAST, 0);
  Event_ShowMessage(0x2007, 0);
  Actor_ShowEmote(7, 0x100, 0);
  Event_ShowMessage(0x2007, 0);
  Actor_FaceDirection(7, FACING_NORTH, 20);
  Event_ShowMessage(0x2007, 0);
  Motion_SetModeAndWaitAnimation(7, 3);
  Event_ShowMessageAndWait(0x2007, 0);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 7);
  Actor_WatchAndSetScript(5, 7);
  Actor_WatchAndSetScript(6, 7);
  Actor_WatchAndSetScript(26, 7);
  Actor_SetSpeed(7, 0x19999, 0xcccc);
  Actor_SetPositionAndReset(7, 72, 368);
  Actor_SetAttachedEffect(26, 258);
  Actor_WalkToAndWait(7, 40, 348);
  Actor_WalkToAndWait(7, 40, 332);
  Actor_FaceDirection(7, FACING_NORTHEAST, 0);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(5);
  Actor_Stop(6);
  Actor_Stop(26);
  Actor_FaceDirection(26, FACING_NORTHWEST + FACING_STEP, 0);
  Event_ShowMessage(0x101a, 0);
  Func_080d47b4(7, 257, 20);
  Actor_FaceDirection(7, FACING_SOUTH, 40);
  Actor_FaceDirectionAndWait(26, FACING_NORTH + FACING_STEP);
  Event_ShowMessage(0x101a, 0);
  Func_080d47b4(7, 0x100, 20);
  Event_ShowMessage(0x1007, 0);
  Actor_FaceDirection(26, FACING_NORTHWEST + FACING_STEP, 0);
  Motion_SetModeAndWaitAnimation(26, 4);
  Event_ShowMessage(0x101a, 0);
  Event_ShowMessage(0x101a, 0);
  Func_080d47b4(7, 265, 40);
  Event_ShowMessage(0x1007, 0);
  Actor_SetAttachedEffect(26, 258);
  Event_Wait(20);
  Event_ShowMessage(0x101a, 0);
  Motion_SetModeAndWaitAnimation(7, 4);
  Event_ShowMessageAndWait(0x1007, 0);
  Actor_SetModeById(26, 4);
  Event_ShowMessage(0x101a, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(6, FACING_SOUTH, 40);
  Event_ShowMessage(0x101a, 0);
  Actor_FaceDirection(26, FACING_NORTH + FACING_STEP, 0);
  Event_OpenMessage(0x101a, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Func_080d47b4(26, 264, 20);
    Event_ShowMessageAndWait(0x101a, 0);
    Actor_FaceDirection(5, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(6, FACING_NORTHEAST, 20);
    Actor_SetModeById(5, 3);
    Motion_SetModeAndWaitAnimation(6, 3);
    *(u16 *)(EVENT_WORK + 0x1c4) += 3;
  } else {
    *(u16 *)(EVENT_WORK + 0x1c4) += 1;
    Func_080d47b4(26, 263, 20);
    Event_ShowMessageAndWait(0x101a, 0);
    Actor_FaceDirectionAndWait(5, FACING_SOUTHEAST);
    Event_ShowMessageAndWait(0x1005, 0);
    Actor_ArmCallbackAndRefresh(6, FACING_EAST);
    Event_ShowMessageAndWait(0x1006, 0);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_WEST, 0);
    FieldEffect_SetAttached(gMainRoutineWork.currentOwner, 258);
    Event_Wait(40);
    Func_02005c7c();
  }
  Func_080d47b4(7, 265, 40);
  Event_ShowMessageAndWait(0x1007, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(26, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(5, FACING_NORTH);
  Actor_SetModeById(5, 3);
  Event_ShowMessageAndWait(0x9005, 0);
  Motion_SetModeAndWaitAnimation(7, 3);
  Actor_SetModeById(6, 3);
  Event_ShowMessageAndWait(0x1006, 0);
  Actor_SetModeById(26, 3);
  Event_ShowMessageAndWait(0x101a, 0);
  Motion_SetModeAndWaitAnimation(7, 3);
  Actor_SetSpeed(7, 0x6666, 0x3333);
  Actor_SetPositionAndReset(7, 40, 350);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Motion_SetModeAndWaitAnimation(26, 3);
  Motion_SetModeAndWaitAnimation(7, 3);
  Actor_SetSpeed(7, 0xcccc, 0x6666);
  Actor_SetMovementTypeById(5, gData_02005fe0 + 0);
  Actor_SetMovementTypeById(6, gData_02005fe0 + 0);
  Actor_SetMovementTypeById(26, gData_02005fe0 + 0);
  Actor_SetMovementTypeAndWaitById(7, gData_02005fe0 + 0);
  GameFlag_SetBit(0x8f4);
  GameFlag_SetBit(0x92e);
  Event_PrepareObjectAndApplyValue(7, 1);
  Func_02005a8c(); // arity?
}
#undef EVENT_WORK

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)

extern const u8 gData_02006014[];
extern const u8 gData_02006058[];
extern const u8 gData_0200609c[];
extern const u8 gData_020060e0[];
extern const u8 gData_02006108[];
void Func_0200384c(void);

void Func_02002050(void) {
  s32 done = Event_GetFlag(0x86a);

  if (done != 0)
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 344, 208);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 308, 200);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 290, 188);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 0);
  Actor_SetSpeed(5, 0xcccc, 0x6666);
  Actor_SetSpeed(6, 0xcccc, 0x6666);
  Actor_SetSpeed(26, 0xcccc, 0x6666);
  Actor_SetSpeed(7, 0xcccc, 0x6666);
  Actor_MoveToObject(5, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(6, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(26, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(7, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_SetMovementTypeById(5, gData_02006014);
  Actor_SetMovementTypeById(6, gData_02006058);
  Actor_SetMovementTypeById(26, gData_0200609c);
  Actor_SetMovementTypeAndWaitById(7, gData_020060e0);
  Event_Wait(20);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Event_SetMessageId(MsgItSADead);
  Event_ShowMessageAndWait(0x8007, 0);
  Camera_SetSpeedParameters(0x26666, 0x4ccc);
  ((struct Actor *)((s32)Func_080d4568__s1318()))->motionFlags = done;
  FieldCamera_MoveTo(PIXELS(200), -1, PIXELS(180), 1);
  Func_02005bb4();
  Event_Wait(10);
  FieldCamera_MoveTo(PIXELS(174), -1, PIXELS(224), 1);
  Func_02005bb4();
  Event_Wait(10);
  FieldCamera_MoveTo(PIXELS(268), -1, PIXELS(120), 1);
  Func_02005bb4();
  Event_Wait(20);
  FieldCamera_MoveTo(PIXELS(272), -1, PIXELS(180), 1);
  Func_02005bb4();
  Actor_SetModeById(6, 4);
  Event_ShowMessageAndWait(6, 0);
  Actor_ShowEmote(5, 257, 0);
  Event_ShowMessageAndWait(5, 0);
  Func_080d47b4(26, 262, 20);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 26);
  Actor_WatchAndSetScript(5, 26);
  Actor_WatchAndSetScript(6, 26);
  Actor_WatchAndSetScript(7, 26);
  FieldCamera_SetSpeed(0x13333, 0x2666);
  FieldCamera_MoveTo(PIXELS(268), -1, PIXELS(146), 1);
  Actor_SetSpeed(26, 0x13333, 0x9999);
  Actor_WalkToAndWait(26, 290, 116);
  Actor_WalkToAndWait(26, 268, 109);
  Actor_FaceDirection(26, FACING_SOUTH + FACING_STEP, 20);
  Actor_FaceDirection(26, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_FaceDirection(26, FACING_SOUTH + FACING_STEP, 40);
  Event_ShowMessageAndWait(0x801a, 0);
  Camera_MoveWithinMapBounds(PIXELS(268), -1, PIXELS(188), 1);
  Actor_SetPositionAndReset(26, 218, 156);
  Actor_ArmCallback(26, FACING_EAST, 40);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(5);
  Actor_Stop(6);
  Actor_Stop(7);
  Event_ShowMessageAndWait(26, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_ArmCallback(7, FACING_EAST, 0);
  Actor_FaceDirection(5, FACING_NORTHEAST, 0);
  Actor_FaceDirection(6, FACING_SOUTHWEST, 20);
  Actor_SetSpeed(26, 0x6666, 0x3333);
  Actor_SetPositionAndReset(26, 248, 156);
  Event_Wait(40);
  Motion_SetModeAndWaitAnimation(26, 3);
  Event_ShowMessageAndWait(26, 0);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 26);
  Actor_WatchAndSetScript(5, 26);
  Actor_WatchAndSetScript(6, 26);
  Actor_WatchAndSetScript(7, 26);
  Event_Wait(20);
  Actor_GetById__s1318(26)->actionFlags &= ~1;
  Actor_SetPositionAndReset(26, 248, 188);
  Event_Wait(1);
  Actor_GetById__s1318(26)->actionFlags |= 1;
  Event_Wait(20);
  Actor_SetModeById(26, 7);
  Event_Wait(20);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 257, 0);
  Actor_ShowEmote(5, 257, 0);
  Actor_ShowEmote(6, 257, 0);
  Actor_ShowEmote(7, 257, 40);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(5);
  Actor_Stop(6);
  Actor_Stop(7);
  Event_ShowMessageAndWait(26, 0);
  Actor_FaceDirection(5, FACING_NORTHEAST, 0);
  Actor_FaceDirection(6, FACING_SOUTHWEST, 0);
  Actor_ShowEmote(5, 257, 0);
  Actor_ShowEmote(6, 257, 20);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x6666, 0x3333);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 280, 188);
  Actor_SetModeById(26, 1);
  Event_Wait(10);
  Actor_GetById__s1318(26)->actionFlags &= ~1;
  Actor_SetPositionAndReset(26, 238, 188);
  Event_Wait(1);
  Actor_GetById__s1318(26)->actionFlags |= 1;
  FieldMessage_ShowAtActor(26, 0);
  Func_080d47b4(gMainRoutineWork.currentOwner, 257, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirection(6, FACING_WEST, 0);
ask:
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    *(u16 *)(EVENT_WORK + 0x1c4) += 1;
  } else {
    Event_ShowMessageAndWait(26, 0);
    *(s16 *)(EVENT_WORK + 0x1c4) -= 2;
    FieldMessage_ShowAtActor(26, 0);
    goto ask;
  }
  Event_SetFlag(545);
  Func_0200384c();
  GameFlag_ClearBit(545);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x100, 0);
  Actor_ShowEmote(5, 0x100, 0);
  Actor_ShowEmote(6, 0x100, 0);
  Actor_ShowEmote(7, 0x100, 40);
  Actor_SetModeById(26, 3);
  Event_ShowMessageAndWait(26, 0);
  Actor_SetAttachedEffect(5, 258);
  Event_ShowMessageAndWait(5, 0);
  Actor_SetModeById(26, 3);
  Event_ShowMessageAndWait(26, 0);
  Actor_FaceDirection(6, FACING_SOUTH, 0);
  Event_ShowMessageAndWait(0x4006, 0);
  Actor_FaceDirection(26, FACING_SOUTHEAST + FACING_STEP, 20);
  Motion_SetModeAndWaitAnimation(26, 3);
  Actor_FaceDirection(6, FACING_WEST, 0);
  Actor_ArmCallbackAndRefresh(26, FACING_EAST);
  Event_ShowMessageAndWait(26, 0);
  Event_ShowMessageAndWait(26, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_WatchAndSetScript(5, gMainRoutineWork.currentOwner);
  Actor_WatchAndSetScript(6, gMainRoutineWork.currentOwner);
  Actor_WatchAndSetScript(26, gMainRoutineWork.currentOwner);
  Actor_WatchAndSetScript(7, gMainRoutineWork.currentOwner);
  FieldCamera_MoveTo(PIXELS(268), -1, PIXELS(164), 1);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 280, 120);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_WEST);
  Event_SetFlag(0x220);
  Func_0200384c();
  GameFlag_ClearBit(0x220);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 232, 142);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTH);
  Event_SetFlag(546);
  Func_0200384c();
  GameFlag_ClearBit(546);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 296, 142);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTH);
  Event_SetFlag(547);
  Func_0200384c();
  GameFlag_ClearBit(547);
  FieldCamera_MoveTo(PIXELS(268), -1, PIXELS(188), 1);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 284, 188);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 10);
  Actor_Stop(5);
  Actor_Stop(6);
  Actor_Stop(26);
  Actor_Stop(7);
  Motion_SetModeAndWaitAnimation(26, 3);
  Event_ShowMessageAndWait(26, 0);
  Actor_ShowEmote(5, 257, 0);
  Event_ShowMessageAndWait(5, 0);
  Actor_FaceDirection(26, FACING_NORTH + FACING_STEP, 0);
  Event_ShowMessageAndWait(26, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 0);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  Actor_FaceDirection(6, FACING_NORTHWEST, 0);
  Actor_FaceDirection(7, FACING_NORTHEAST, 0);
  Actor_SetAttachedEffect(7, 258);
  Event_ShowMessageAndWait(7, 0);
  Func_080d47b4(26, 265, 0);
  Actor_FaceDirectionAndWait(26, FACING_WEST);
  Event_ShowMessage(0x201a, 0);
  Actor_ShowEmote(7, 257, 20);
  Actor_FaceDirectionAndWait(26, FACING_NORTH + FACING_STEP);
  Event_ShowMessageThenWait(26, 0, 20);
  Actor_FaceDirection(5, FACING_NORTHEAST, 0);
  Actor_FaceDirection(6, FACING_SOUTHWEST, 20);
  Event_ShowMessageAndWait(6, 0);
  Actor_ArmCallback(26, FACING_EAST, 10);
  Actor_SetModeById(26, 4);
  Event_ShowMessageAndWait(26, 0);
  Actor_FaceDirection(6, FACING_NORTHWEST, 0);
  Actor_FaceDirection(26, FACING_NORTH + FACING_STEP, 0);
  FieldCamera_MoveTo(PIXELS(268), -1, PIXELS(164), 1);
  Actor_WalkToAndWait(5, 296, 168);
  Actor_WalkToAndWait(5, 280, 154);
  Actor_FaceDirection(5, FACING_WEST, 40);
  Event_ShowMessageAndWait(5, 0);
  Actor_ShowEmote(26, 257, 0);
  Event_ShowMessageAndWait(26, 0);
  Actor_FaceDirection(5, FACING_SOUTHWEST, 0);
  Event_ShowMessageAndWait(5, 0);
  Actor_ShowEmote(26, 0x100, 20);
  Event_ShowMessageAndWait(26, 0);
  Camera_MoveWithinMapBounds(PIXELS(268), -1, PIXELS(188), 1);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_FaceDirection(6, FACING_WEST, 0);
  Actor_ArmCallback(7, FACING_EAST, 0);
  Actor_FaceDirection(26, FACING_SOUTHEAST + FACING_STEP, 40);
  Actor_FaceDirection(26, FACING_SOUTH + FACING_STEP, 20);
  Actor_FaceDirection(26, FACING_NORTH + FACING_STEP, 40);
  Actor_ArmCallbackAndRefresh(26, FACING_EAST);
  Event_ShowMessageAndWait(26, 0);
  Actor_SetAttachedEffect(26, 258);
  Event_ShowMessageAndWait(26, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 20);
  Actor_SetModeById(26, 4);
  Event_ShowMessageAndWait(26, 0);
  Actor_FaceDirection(5, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_FaceDirection(26, FACING_SOUTHEAST + FACING_STEP, 20);
  Event_ShowMessageAndWait(26, 0);
  Actor_ArmCallback(26, FACING_EAST, 10);
  Event_ShowMessageAndWait(26, 0);
  Func_080d47b4(7, 263, 0);
  Event_ShowMessageAndWait(7, 0);
  Actor_FaceDirectionAndWait(26, FACING_WEST);
  Actor_SetModeById(26, 4);
  Event_ShowMessageAndWait(0x201a, 0);
  Func_080d47b4(7, 257, 40);
  Actor_ArmCallbackAndRefresh(26, FACING_EAST);
  Event_ShowMessageAndWait(26, 0);
  Actor_ShowEmote(7, 0x100, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x100, 0);
  Actor_ShowEmote(5, 0x100, 0);
  Func_080d47b4(6, 0x100, 40);
  Func_080d47b4(26, 264, 20);
  Event_ShowMessageAndWait(26, 0);
  Actor_SetModeById(26, 3);
  Event_ShowMessageAndWait(26, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Motion_SetModeAndWaitAnimation(7, 3);
  Actor_SetSpeed(5, 0x13333, 0x9999);
  Actor_SetSpeed(6, 0x13333, 0x9999);
  Actor_SetSpeed(26, 0x13333, 0x9999);
  Actor_SetSpeed(7, 0x13333, 0x9999);
  Actor_SetMovementTypeById(5, gData_02006108);
  Actor_SetMovementTypeById(6, gData_02006108);
  Actor_SetMovementTypeById(26, gData_02006108);
  Actor_SetMovementTypeAndWaitById(7, gData_02006108);
  GameFlag_SetBit(0x8f5);
  Event_End();
}

#undef EVENT_WORK

// ---- games/tla/src/scene/kibombo/s2ad8.c ----
// Scene code of overlay 0x670, in ROM order, from s2ad8's offset up to the next
// object the overlay links.

void Func_02002ad8(void) {
  struct KibomboWork90 *work = Ram_HeapSlots->unk84[3];

  work->leader = Actor_GetById(gMainRoutineWork.currentOwner);
}

void Func_02002af8(void) {
  struct KibomboWork90 *work = Ram_HeapSlots->unk84[3];

  work->leader = 0;
}

s32 Func_02002b08(s32 value) {
  if (value > 31)
    value = 31;
  if (value < 0)
    value = 0;
  return value;
}

#include "io_reg.h"

void Func_02002b18(s32 index, s32 entry, s32 delta) {
  u16 color = gData_02007948[entry];
  s16 r = color & 0x1f;
  s16 g = (color >> 5) & 0x1f;
  s16 b = (color >> 10) & 0x1f;

  r += delta;
  g += delta >> 1;
  b += delta >> 2;

  r = Func_02002b08(r);
  g = Func_02002b08(g);
  b = Func_02002b08(b);
  {
    u16 *dst = &((u16 *)BG_PLTT)[index];

    r |= (b << 10) | (g << 5);
    *dst = r;
  }
}

void Func_02002ba4(s32 index, s32 entry, s32 delta) {
  u16 color = gData_02007948[entry];
  s16 r = color & 0x1f;
  s16 g = (color >> 5) & 0x1f;
  s16 b = (color >> 10) & 0x1f;

  r += delta + 4;
  delta -= 4;
  g += delta >> 1;
  b += delta >> 2;

  r = Func_02002b08(r);
  g = Func_02002b08(g);
  b = Func_02002b08(b);
  {
    u16 *dst = &((u16 *)BG_PLTT)[index];

    r |= (b << 10) | (g << 5);
    *dst = r;
  }
}

/// 2 copies: kibombo, kibombo.
#define DEFINE_KIBOMBO_WOBBLE(name, n1, n2, n3, n4, call1, n5, n6, n7)         \
  void name(void) {                                                            \
    struct KibomboEventWork *work = Ram_HeapSlots->eventWork;                  \
                                                                               \
    if (work->field166 == 0 && work->field15a == 0 && work->field15e == 0 &&   \
        (gFrameCount & 7) == 0) {                                              \
      Func_02002b18(9, 0, ((Random16Far() * 10) >> 16) - 5);                   \
      Func_02002b18(n1, 1, ((Random16Far() * n2) >> 16) - n3);                 \
      Func_02002b18(n4, 2, ((Random16Far() * 4) >> 16) - 2);                   \
      call1(n5, 3, ((Random16Far() * n6) >> 16) - n7);                         \
    }                                                                          \
  }

DEFINE_KIBOMBO_WOBBLE(Func_02002c34, 87, 8, 4, 103, Func_02002b18, 119, 4, 2)

DEFINE_KIBOMBO_WOBBLE(Func_02002ccc, 7, 6, 3, 6, Func_02002ba4, 76, 8, 4)

void Func_02002d64(void) {
  struct KibomboEventWork *work = Ram_HeapSlots->eventWork;

  if (work->field166 == 0 && work->field15a == 0 && work->field15e == 0 &&
      (gFrameCount & 7) == 0) {
    Func_02002b18(169, 0, ((Random16Far() * 4) >> 16) - 2);
    Func_02002b18(167, 1, ((Random16Far() * 2) >> 16) - 1);
    Func_02002b18(166, 2, Random16Far() >> 16);
  }
}

void Func_02002de0(void) {
  struct KibomboObject **slot = &gData_02007950;
  s32 zero = 0;
  struct KibomboObject *object =
      Import_ActorCreate(0x1e9, 0x1090000, 0x240000, 0x1580000);

  *slot = object;
  if (object != 0) {
    struct FieldSprite *sprite = object->sprite;
    u8 *buffer;

    sprite->fullColor = 0;
    sprite->palette = 0;
    sprite->priority = 1;
    object->motionFlags = zero;
    object->field5c = 1;
    sprite->nextFrame = zero;
    sprite->unk1b[0] = zero;
    buffer = HeapSlot_AllocFar(68, 0x608);
    Icon_DrawItem(242);
    VramCache_Load(sprite->vramBlock, 128, buffer + 0x400);
    HeapSlot_Release(68);
  }
}

void Func_02002e60(void) {
  struct KibomboObject *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  if (leader->y > 0x100000) {
    Actor_SetHorizontalPositionWithTerrain(29, 0, 0);
    Actor_SetSpritePriority(30, 2);
  } else {
    Kibombo_ObjSetPos(Actor_GetById(29), PIXELS(120), 0, PIXELS(168));
    Actor_SetSpritePriority(30, 3);
  }
}

void Func_02002eb0(void) {
  if (gData_02007950 != 0) {
    struct KibomboObject *object = Actor_GetById(27);

    Actor_SetPositionAndResetMotion(gData_02007950, object->x,
                                    object->y + PIXELS(12), object->z);
  }
}

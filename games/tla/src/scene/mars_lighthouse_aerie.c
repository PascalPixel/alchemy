/// @file
/// The scene code of overlay 0x6ae, in ROM order.

#include "shared/mars_lighthouse.h"
#include "shared/effects.h"
#include "event_work.h"
#include "module/scene/mars_lighthouse_aerie.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "scene_ids.h"
#include "module/field/event_commands.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "system/io_reg.h"
#include "field_sprite.h"
#include "system/ram_buffer.h"
#include "call.h"
#include "field_event.h"
#include "system/frame.h"
#include "shared/actors.h"
#include "shared/tables.h"
#include "item_icon.h"
#include "version.h"
#include "system/dma.h"
#include "system/task.h"
#include "system/vector3.h"
#include "objcmd.h"
#ifndef TLA_MARS_LIGHTHOUSE_AERIE_H
#define TLA_MARS_LIGHTHOUSE_AERIE_H

#include "types.h"
#include "system/ram_buffer.h"
#include "system/frame.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "scene_tables.h"

struct Trigger;

u32 Random16(void);
void Func_02000038(struct Actor *object);
void Func_02004d24(s32 id);
void Func_02004d50(s32 id);
void Func_02004fac(void);
void Func_02005010(void);
void PaletteCycle_Fade(void);
void Teleport_PlaySparks(void);

extern u8 MsgUhhhhnnnICan[];
extern u8 MsgKoffKoffYou[];
extern u8 MsgUnnnnIAm[];
extern u8 MsgIUnderstandYour[];
extern s16 gMarsLighthouseAerie_Placements[];
extern struct ActorPlacement gData_0200644c[];
extern s16 gData_020066bc[];
extern u32 gData_02006b64;
extern u32 gData_02006b6c;

void Func_02004e80(s32 from, s32 to);

extern u8 MsgDoNotLet[];
extern u16 gData_02006cd0;
extern u16 gData_02006cd8;
extern u16 gData_02006cdc;
extern u16 *gData_02006ce0;
extern u16 gData_02006cec;
extern u16 gData_02006cf0;

struct MarsLighthouseAerieParty {
  u8 unk000[0x26a];
  s16 held;
};

s32 List_CountEntries(u16 *list);

#endif

#include "text/msg_ids.h"

extern u32 Data_0200eb60;
void TileMap_ConvertBuffer(s32 scene, s32 a1);
void FieldCamera_WaitForArrival();
extern u8 gData_02006c20[];
extern u8 gData_02006b70[];
void Party_RestoreHpPp();
void Weather_StartRain(struct Actor *object);

void Func_02000038(struct Actor *object) {
  u8 *timer = &object->unk5c[6];

  if (*timer != 0) {
    (*timer)--;
  } else {
    *timer = (((u32)Random16() * 80) >> 16) + 60;
    object->facing = Random16();
  }
}

void Func_02000068(struct Actor *object) {
  if ((gFrameCount >> 1) & 1)
    FieldActor_SetSpriteColor(object, 15);
  else
    FieldActor_SetSpriteColor(object, 0);
}

/// Shrinks the object toward its normal size, and when its timer runs out
/// restarts it at 40 to 119 frames and swells it again.
void Func_0200008c(struct Actor *object) {
  u8 *timer;
  s32 scale = object->scaleX;

  if (scale > 0x10000) {
    object->scaleX = scale - 0x800;
    object->scaleY -= 0x400;
  }
  timer = &object->unk5c[6];
  if (*timer != 0) {
    (*timer)--;
  } else {
    u32 r = Random16();

    *timer = *timer + ((((r << 2) + r) * 16) >> 16) + 40;
    object->scaleX = 0x12000;
    object->scaleY = 0x11000;
  }
}

/// Circles the object around actor 8: each frame it steps its angle back by
/// 0x800 and places itself at the anchor's x and z plus the cosine and sine
/// times two radii read from its speed limit and its counter byte.
// The twin of ☀️'s SceneEffect_UpdateCounterDrivenOrbit.
void Func_020000e4(struct Actor *object) {
  struct Actor *anchor = Actor_GetById(8);
  u16 *pangle = (u16 *)&object->timer;
  s32 angle = *pangle;
  s32 cosine;
  s32 sine;
  s32 along;
  s32 across;
  u8 *counter;

  cosine = Trig_Cos(angle);
  counter = &object->unk5c[6];
  {
    s32 radius = object->speedLimit + *counter;
    struct Actor *a;

    a = anchor;
    along = a->x + cosine * (radius + 6);
  }
  object->x = along;
  sine = Math_Sin(angle);
  across = anchor->z + sine * (*counter + 4);
  object->z = across;
  object->targetX = object->x;
  object->targetZ = across;
  *pangle -= 0x800;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct Trigger *Scene_GetPlacements(void) {
  if (gMainRoutineWork.entrance == 2)
    return (const struct Trigger *)gData_0200644c;
  if ((u16)(gMainRoutineWork.entrance - 3) <= 2)
    return (const struct Trigger *)gData_020066bc;
  return (const struct Trigger *)gMarsLighthouseAerie_Placements;
}

DEFINE_SCENE_LEAVE_BY_EXIT(Scene_LeaveByExit, Func_02004fac,
                           Teleport_PlaySparks, 1, Func_02005010)

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

extern s16 gData_02005aa4[];
extern s16 gData_02005ab4[];
extern s16 gData_02005ae0[];
extern u32 gData_02005b0c[];

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);
/* VIEW(mars_lighthouse_aerie): routine Palette_SetColorsFade as void
 * Palette_SetColorsFade(const u8 *a, const u8 *b, const u8 *c, const u8 *d);
 * scene/mars_lighthouse_aerie/s3370 has
 * DEFINE_PALETTE_SET_COLORS_FADE(Palette_SetColorsFade, gData_02006ce0,
 * Data_02006ce4, Data_02006ce8, Data_02006cd4, Func_020051e0) #include "task.h"
 * DEFINE_PALETTE_CYCLE_START(Func_02004fac, gData_02006cf0, gData_02006cec,
 * List_CountEntries, gData_02006ce0, gData_02006cdc, 15, gData_02006cd8, 16,
 * gData_02006cd0, PaletteCycle_Fade) void Func_02005010(void) {; tried as the
 * one declaration void Palette_SetColorsFade(const u8 *a, const u8 *b, const u8
 * *c, const u8 *d), Func_020001d8 moves: 3 of 124 instructions in tla-ja. */
void Func_02004ef0__mars_lighthouse_aerie(
    const u8 *a, const u8 *b, const u8 *c,
    const u8 *d) __asm__("Palette_SetColorsFade");
#define Palette_SetColorsFade Func_02004ef0__mars_lighthouse_aerie
void Func_020004a4(void);

void Func_020001d8(void) {
  struct Actor *object;
  s32 i;

  Palette_SetColorsFade(gData_02005aa4, gData_02005ab4, gData_02005ae0,
                        gData_02005b0c);
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 0x40;
  for (i = 10; i <= 16; i++) {
    object = Actor_Get(i);
    object->priorityFlags |= 0x80;
    Actor_SetSpriteColorMode(object, 5);
  }
  if (!Event_GetFlag(0xa3c))
    return;
  Actor_SetPosition(8, 0x2000000, 0x1080000);
  Actor_SetPosition(9, 0x2000000, 0x10f0000);
  System_Wait(1);
  FieldActor_SetSpritePriority(8, 1);
  object = Actor_Get(8);
  object->motionFlags = 4;
  object->y = 0x700000;
  Actor_SetSpriteColorMode(object, 5);
  Actor_SetSpriteFlags(object, 0);
  object->priorityFlags |= 0x20;
  FieldActor_SetSpritePriority(9, 2);
  object = Actor_Get(9);
  object->motionFlags = 4;
  object->y = 0x700000;
  object->scaleX = 0x14ccc;
  object->scaleY = 0x14ccc;
  Actor_SetSpriteColorMode(object, 5);
  Actor_SetSpriteFlags(object, 0);
  object->priorityFlags |= 0x20;
  object->priorityFlags |= 2;
  Actor_SetBlendMode(object, 1);
  Data_0200eb60 = 0;
  FieldTask_AddFrame(Func_020004a4, 0x480);
}

/// Sets the scene up for the entrance the party arrived by (5 to 2).
s32 Scene_Initialize(void) {
  ((struct MarsLighthouseAerieEventWork *)Ram_HeapSlots->eventWork)
      ->screenEffect = 0x204;
  Func_020001d8();
  Event_SetFlag(0x112);
  if (gMainRoutineWork.entrance == 5) {
    /* FAKEMATCH(pin): the blend word pinned to r3 as a halfword loads from
     * the pool before the register address, as native; as a plain store, 2
     * of 121 instructions differ in tla-en. */
    register u16 alpha asm("r3") = 0xc08;

    REG_BLDALPHA = alpha;
    REG_BLDCNT = 0x3f10;
    MarsLighthouseAerie_WatchGoldenSunRise();
  } else if (gMainRoutineWork.entrance == 4) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Actor_SetAnimation(20, 8);
    Actor_SetAnimation(21, 8);
    Actor_SetAnimation(22, 7);
    Actor_SetPosition(19, PIXELS(498), PIXELS(320));
    Actor_SetPosition(1, PIXELS(538), PIXELS(320));
    Actor_SetPosition(gMainRoutineWork.currentOwner, PIXELS(506), PIXELS(280));
    System_Wait(1);
    FieldCamera_MoveTo(-1, -1, -1, 0);
    EventCamera_GetActor()->motionFlags = 0;
    FieldCamera_MoveTo(PIXELS(512), PIXELS(96), PIXELS(312), 1);
    Map_CenterOnFollowedPoint();
    System_Wait(1);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    FieldEvent_Wait(40);
    MarsLighthouseAerie_LightBeacon();
    Event_End();
  } else if (gMainRoutineWork.entrance == 3) {
    Actor_SetAnimation(20, 8);
    Actor_SetAnimation(21, 8);
    Actor_SetAnimation(22, 7);
    if (Event_GetFlag(0xa3d) == 0)
      MarsLighthouseAerie_UrgeOnToBeacon();
  } else if (gMainRoutineWork.entrance == 2) {
    MarsLighthouseAerie_FindWhoTheDragonWas();
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.entrance == 1 && Event_GetFlag(0xa3d) == 0)
    Weather_StartRain(1);
  TileMap_ConvertBuffer(gMainRoutineWork.map + ((s32)&SceneId_Ov6ACRoom3 -
                                                (s32)&SceneId_Ov6a8Room2),
                        0x5000000);
  return 0;
}

extern u32 gData_02006ad0[];

void Actor_StartScript(struct Actor *object, const u8 *script);
void Func_020005a0(struct Actor *object);

void Func_020004a4(void) {
  struct Actor *anchor = Actor_Get(8);
  s32 offset = (Random16() * 48 >> 16) << 16;
  struct Actor *object;
  struct MarsLighthouseAerieSprite *sprite;
  u32 phase;

  if (anchor != NULL && (phase = Data_0200eb60 & 15) == 0) {
    object = FieldActor_Create(0x2a0, anchor->x + 0x80000,
                               anchor->y + offset + 0x80000, anchor->z);
    offset = (offset / 0x60000) << 16;
    if (object != NULL) {
      u8 zero;

      sprite = object->sprite;
      Actor_StartScript(object, gData_02006ad0);
      FieldActor_SetSpriteColor(object, 10);
      object->motionFlags = phase;
      {
        /* FAKEMATCH(temporary): the masked value held in a local; as
         * object->action = Random16() & 0x0ffff000, 45 of 103 instructions
         * differ in tla-en. */
        s32 action = Random16() & 0x0ffff000;

        object->timer = action;
      }
      object->unk66 = phase;
      object->update = Func_020005a0;
      zero = 0;
      object->watchedObject = anchor;
      object->speedLimit = Math_Sin((offset & 0xfffff) >> 4) * 24 >> 16;
      ((u8 *)sprite)[26] = zero;
      sprite->layer =
          ((struct MarsLighthouseAerieSprite *)anchor->sprite)->layer;
    }
  }
  Data_0200eb60++;
}

/// Swings the object along an arc beside its watched object, drawing it behind
/// (priority 2) while its z is short of 0x108 and in front (priority 1) after.
void Func_020005a0(struct Actor *object) {
  u16 *pangle = (u16 *)&object->timer;
  u16 angle = *pangle;
  struct Actor *parent = object->watchedObject;

  object->x = parent->x + Trig_Cos(angle) * (object->speedLimit + 28);
  object->z = (Math_Sin(angle) << 4) + 0x1080000;
  object->targetX = object->x;
  object->targetZ = object->z;
  if (object->z < 0x1080000)
    ((struct FieldSprite *)object->sprite)->priority = 2;
  else
    ((struct FieldSprite *)object->sprite)->priority = 1;
  *pangle -= 0x200;
}

void Func_02000610(s32 id) {
  struct Actor *object = Actor_Get(id);

  FieldActor_Stop(id);
  object->unk5c[6] = (((u32)Random16() * 40) >> 16) + 20;
  object->update = (void (*)(void))Func_02000038;
}

void Func_02000640(s32 id) {
  Actor_Get(id)->update = NULL;
  Actor_FaceDirection(id, FACING_NORTH, 0);
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
struct SparkleWork;
s32 ObjectTable_DestroyById();
s32 Actor_MoveToObject();
extern u32 gData_02005ba8[];
extern u32 gData_02005bd0[];
extern u32 gData_02005bf8[];
extern u32 gData_02005c20[];
extern u32 gData_02005c48[];
extern s32 gData_02005c70[];
extern u32 gData_02005cac[];
extern u32 gData_02005ce8[];
extern u32 gData_02005d2c[];
extern u32 gData_02005d70[];
extern u32 gData_02005db4[];
extern u32 gData_02005df8[];
extern u32 gData_02005e3c[];
extern u32 gData_02005e80[];
extern u32 gData_02005ec4[];

/// The party reaches the beacon in the storm, and the voice calls them
/// betrayers before the Doom Dragon's battle.
/// ★ Named from the game: "Betrayers, you have arrived!" (MsgBetrayersYouHave)
void MarsLighthouseAerie_FaceDoomDragon(void) {
  u8 *unk85;
  u16 *dispcnt;
  struct MainRoutineWork *work;
  s32 t0;
  u16 *palette;
  s32 actor, t4, t16, t18, t21, t23, t25, t27, t48, t50;
  s32 t53, t55, t57, t59, t61, t88, t91, t94, t98, t99;
  s32 t100, t102, t112, t114, t118, t120, t122, t123, t125;
  s32 t127, t129, t132, t134, t136, t137, t141, t144, t166;
  s32 t182, t191, t194, t206, t278, t298, t334, t356, t406;
  s32 t414, t415, t419, t420, t423, t424, t433, t451, t461, t462;
  s32 t463, t465, t466, t469, t471, t473, t482, t484, t490, t492;
  s32 t494, t496, t497, t499, t502, t503, t523, t528;
  s32 t531, t540, t545, t562, t628, t670, t679;

  t0 = gMainRoutineWork.currentOwner;
  actor = Actor_Get(t0);
  palette = (u16 *)0x500021e;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  t4 = Actor_Get(18);
  FieldActor_SetSpriteColor(t4, 15);
  Actor_SetPosition(18, 0x2000000, 0x1140000);
  System_Wait(1);
  FieldActor_SetSpritePriority(6, 1);
  FieldActor_SetSpritePriority(5, 1);
  FieldActor_SetSpritePriority(19, 1);
  FieldActor_SetSpritePriority(2, 1);
  FieldActor_SetSpritePriority(3, 1);
  Event_PlaySound(0);
  FieldEvent_SetMessageId((s32)MsgBetrayersYouHave);
  Event_ShowMessage(18, 0);
  t16 = gMainRoutineWork.currentOwner;
  Actor_ShowEmote(t16, 0x100, 20);
  t18 = EventCamera_GetActor();
  FIELD_AT(t18, u8, 85) = 0;
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(0x2000000, -1, 0x1300000, 1);
  t21 = gMainRoutineWork.currentOwner;
  Actor_SetSpeed(t21, 0x19999, 0xcccc);
  t23 = gMainRoutineWork.currentOwner;
  Actor_WalkToAndWait(t23, 0x1f8, 0x140);
  t25 = gMainRoutineWork.currentOwner;
  Actor_FaceDirection(t25, 0xc000, 0);
  t27 = gMainRoutineWork.currentOwner;
  Actor_MoveToObject(0, t27);
  System_Wait(1);
  Actor_SetSpeed(0, 0x19999, 0xcccc);
  Actor_WalkToAndWait(0, 0x208, 0x140);
  Actor_FaceDirection(0, 0xc000, 0);
  Actor_MoveToObject(1, 0);
  System_Wait(1);
  Actor_SetSpeed(1, 0x19999, 0xcccc);
  Actor_WalkToAndWait(1, 0x218, 0x140);
  Actor_FaceDirection(1, 0xa000, 20);
  Actor_FaceDirection(1, 0xe000, 20);
  Actor_FaceDirection(1, 0xc000, 0);
  Event_ShowMessage(1, 0);
  Actor_MoveToObject(3, 1);
  System_Wait(1);
  Actor_SetSpeed(3, 0x19999, 0xcccc);
  Actor_WalkToAndWait(3, 0x220, 0x148);
  Actor_FaceDirectionAndWait(3, 0xa000);
  Actor_SetAnimation(3, 4);
  Event_ShowMessage(3, 0);
  t48 = gMainRoutineWork.currentOwner;
  FieldActor_Stop(t48);
  t50 = Random16();
  FIELD_AT(actor, u8, 98) = (((u32)(((t50 << 2) + t50) << 2) >> 16) + 20);
  FIELD_AT(actor, s32, 108) = (s32)Func_02000038;
  Func_02000610(1);
  Func_02000610(3);
  t53 = gMainRoutineWork.currentOwner;
  Actor_MoveToObject(5, t53);
  t55 = gMainRoutineWork.currentOwner;
  Actor_MoveToObject(6, t55);
  t57 = gMainRoutineWork.currentOwner;
  Actor_MoveToObject(7, t57);
  t59 = gMainRoutineWork.currentOwner;
  Actor_MoveToObject(2, t59);
  t61 = gMainRoutineWork.currentOwner;
  Actor_MoveToObject(19, t61);
  System_Wait(1);
  Actor_SetSpeed(5, 0x19999, 0xcccc);
  Actor_SetSpeed(6, 0x19999, 0xcccc);
  Actor_SetSpeed(7, 0x19999, 0xcccc);
  Actor_SetSpeed(2, 0x19999, 0xcccc);
  Actor_SetSpeed(19, 0x19999, 0xcccc);
  Actor_SetScript(5, (s32)gData_02005ba8);
  Actor_SetScript(6, (s32)gData_02005bd0);
  Actor_SetScript(7, (s32)gData_02005bf8);
  Actor_SetScript(19, (s32)gData_02005c48);
  Actor_RunScriptAndWait(2, (s32)gData_02005c20);
  FieldEvent_Wait(10);
  ObjectTable_DestroyById(16);
  ObjectTable_DestroyById(15);
  ObjectTable_DestroyById(14);
  ObjectTable_DestroyById(13);
  ObjectTable_DestroyById(12);
  ObjectTable_DestroyById(11);
  ObjectTable_DestroyById(10);
  Func_02000610(5);
  Func_02000610(6);
  Func_02000610(7);
  Func_02000610(2);
  Func_02000610(19);
  FieldEvent_Wait(80);
  t88 = Actor_Get(6);
  FIELD_AT(t88, void *, 108) = 0;
  Actor_FaceDirectionAndWait(6, 0);
  Event_ShowMessage(6, 0);
  t91 = Actor_Get(5);
  FIELD_AT(t91, void *, 108) = 0;
  Actor_FaceDirectionAndWait(5, 0x8000);
  FieldActor_SetAnimationAndWait(5, 3);
  t94 = Actor_Get(7);
  FIELD_AT(t94, void *, 108) = 0;
  Actor_FaceDirectionAndWait(7, 0);
  FieldActor_Jump(7, 4, 10);
  Event_ShowMessage(7, 0);
  t98 = gMainRoutineWork.currentOwner;
  t99 = Actor_Get(t98);
  FIELD_AT(t99, void *, 108) = 0;
  t100 = gMainRoutineWork.currentOwner;
  Actor_FaceDirectionAndWait(t100, 0x8000);
  t102 = gMainRoutineWork.currentOwner;
  FieldActor_SetAnimationAndWait(t102, 3);
  Func_02000640(0);
  Func_02000640(1);
  Func_02000640(19);
  Func_02000640(2);
  Func_02000640(3);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x10003, 1);
  PaletteFade_Interpolate(40);
  t112 = gMainRoutineWork.currentOwner;
  Actor_SetSpeed(t112, 0xcccc, 0x6666);
  t114 = gMainRoutineWork.currentOwner;
  Actor_WalkToAndWait(t114, 0x1f8, 0x124);
  Event_PlaySound(162);
  FieldEvent_Wait(20);
  t118 = gMainRoutineWork.currentOwner;
  Actor_RunRepeatedMotion(t118, 2);
  t120 = gMainRoutineWork.currentOwner;
  Actor_SetAttachedEffect(t120, 0x102);
  t122 = gMainRoutineWork.currentOwner;
  t123 = Actor_Get(t122);
  {
    u8 *f = (u8 *)t123 + 90;
    *f &= 254;
  }
  t125 = gMainRoutineWork.currentOwner;
  Actor_SetSpeed(t125, 0x3333, 0x1999);
  t127 = gMainRoutineWork.currentOwner;
  Actor_MoveToAndWait(t127, 0x1f8, 0x134);
  t129 = gMainRoutineWork.currentOwner;
  Actor_RunRepeatedMotion(t129, 3);
  Event_PlaySound(0x186);
  t132 = gMainRoutineWork.currentOwner;
  Actor_SetAttachedEffect(t132, 0x102);
  t134 = gMainRoutineWork.currentOwner;
  Actor_MoveToAndWait(t134, 0x1f8, 0x140);
  t136 = gMainRoutineWork.currentOwner;
  t137 = Actor_Get(t136);
  FIELD_AT(t137, u8, 90) |= 1;
  *palette = 0x7fff;
  System_Wait(1);
  Event_ShowMessage(18, 0);
  t141 = gMainRoutineWork.currentOwner;
  Actor_FaceDirection(t141, 0, 0);
  Actor_FaceDirection(0, 0x8000, 40);
  t144 = gMainRoutineWork.currentOwner;
  Actor_FaceDirection(t144, 0xc000, 0);
  Actor_FaceDirectionAndWait(0, 0xc000);
  Actor_WalkToAndWait(0, 0x208, 0x134);
  Actor_FaceDirection(0, 0xa000, 20);
  Actor_FaceDirection(0, 0xe000, 20);
  Actor_FaceDirection(0, 0xc000, 0);
  FieldEvent_ShowMessageThenWait(0x8000, 0, 20);
  Event_ShowMessage(18, 0);
  Actor_ShowEmote(0, 0x101, 0);
  Actor_ShowEmote(1, 0x101, 0);
  Actor_FaceDirection(0, 0x2000, 0);
  Actor_FaceDirection(1, 0xa000, 40);
  Event_ShowMessage(2, 0);
  Actor_FaceDirection(0, 0x4000, 0);
  Actor_FaceDirectionAndWait(1, 0x4000);
  Actor_SetAnimation(0, 4);
  FieldActor_SetAnimationAndWait(1, 4);
  Event_PlaySound(36);
  Event_ShowMessage(18, 0);
  Actor_FaceDirection(0, 0xc000, 0);
  Actor_FaceDirectionAndWait(1, 0xc000);
  t166 = gMainRoutineWork.currentOwner;
  Func_02000610(t166);
  Func_02000610(5);
  Func_02000610(6);
  Func_02000610(7);
  Func_02000610(0);
  Func_02000610(1);
  Func_02000610(3);
  Func_02000610(2);
  Func_02000610(19);
  FieldEvent_Wait(40);
  Func_02000640(19);
  Actor_ShowEmote(19, 0x100, 20);
  FieldActor_Jump(19, 4, 10);
  FieldActor_Jump(19, 6, 10);
  Event_ShowMessage(19, 0);
  t182 = gMainRoutineWork.currentOwner;
  Func_02000640(t182);
  Func_02000640(5);
  Func_02000640(6);
  Func_02000640(7);
  Func_02000640(0);
  Func_02000640(1);
  Func_02000640(3);
  Func_02000640(2);
  t191 = Actor_Get(18);
  FieldActor_SetSpriteColor(t191, 0);
  actor = Actor_Get(18);
  FIELD_AT(actor, u8, 85) = 0;
  t194 = Actor_Get(18);
  FieldActor_SetPositionAndResetMotion(t194, 0x2000000, 0x780000, 0xd00000);
  FieldEvent_Wait(20);
  FieldCamera_SetSpeed(0x4cccc, 0x9999);
  FieldCamera_MoveTo(0x2000000, -1, 0xd80000, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(40);
  FieldEvent_ShowMessageThenWait(1, 0, 20);
  FieldCamera_MoveTo(0x2000000, -1, 0x1300000, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_SetAttachedEffect(0, 0x102);
  t206 = gMainRoutineWork.currentOwner;
  Actor_SetAttachedEffect(t206, 0x102);
  Actor_SetAttachedEffect(1, 0x102);
  Actor_SetAttachedEffect(5, 0x102);
  Actor_SetAttachedEffect(19, 0x102);
  FieldEvent_Wait(20);
  Event_ShowMessage(19, 0);
  Actor_SetAnimation(0, 3);
  Actor_SetAnimationAndWait(0, 3);
  Actor_FaceDirectionAndWait(5, 0xe000);
  Event_ShowMessage(5, 0);
  Actor_ShowEmote(0, 0x109, 0);
  Event_ShowMessage(0x8000, 0);
  Actor_ShowEmote(2, 0x101, 20);
  Event_ShowMessage(2, 0);
  Actor_FaceDirectionAndWait(1, 0x4000);
  Event_ShowMessage(0x4001, 0);
  Actor_FaceDirectionAndWait(0, 0x4000);
  Event_ShowMessage(0x5000, 0);
  Actor_ShowEmote(3, 0x105, 0);
  Event_ShowMessage(3, 0);
  Actor_ShowEmote(1, 0x103, 0);
  Actor_FaceDirectionAndWait(1, 0x2000);
  Event_ShowMessage(0x5001, 0);
  Actor_FaceDirectionAndWait(7, 0);
  Event_ShowMessage(0x4007, 0);
  FieldCamera_SetSpeed(0x6666, 0xccc);
  FieldCamera_MoveTo(0x2000000, -1, 0x1280000, 1);
  Actor_SetSpeed(18, 0xcccc, 0x6666);
  FieldActor_SetMoveTarget(actor, 0x2000000, 0x600000, 0x1140000);
  FieldActor_WaitForMoveById(18);
  FieldEvent_Wait(10);
  Event_ShowMessage(18, 0);
  Actor_FaceDirection(7, 0xc000, 0);
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_FaceDirection(1, 0xc000, 0);
  Actor_RunRepeatedMotion(0, 1);
  Actor_WalkToAndWait(0, 0x208, 0x140);
  Actor_FaceDirectionAndWait(0, 0xc000);
  FieldActor_SetAnimation(18, 9);
  FieldEvent_Wait(20);
  Event_ShowMessage(18, 0);
  Event_ShowMessage(5, 0);
  FieldActor_SetAnimation(18, 1);
  FieldEvent_Wait(10);
  Event_ShowMessage(18, 0);
  Actor_SetAttachedEffect(6, 0x102);
  Event_ShowMessage(6, 0);
  FieldActor_SetAnimation(18, 9);
  FieldEvent_Wait(20);
  Event_ShowMessage(18, 0);
  Event_ShowMessage(0, 0);
  Event_ShowMessage(3, 0);
  Event_ShowMessage(7, 0);
  FieldActor_Jump(6, 4, 10);
  Event_ShowMessage(6, 0);
  Actor_SetAnimation(5, 4);
  Event_ShowMessage(5, 0);
  FieldActor_SetAnimation(18, 11);
  FieldEvent_Wait(20);
  Event_ShowMessage(6, 0);
  FieldActor_SetAnimation(18, 12);
  Actor_WaitForAnimationChange(18);
  FieldEvent_Wait(20);
  Event_ShowMessage(18, 0);
  Actor_FaceDirection(0, 0, 0);
  Actor_FaceDirection(1, 0x8000, 0);
  Actor_FaceDirection(2, 0, 0);
  Actor_FaceDirection(3, 0x8000, 0);
  Actor_FaceDirection(5, 0x8000, 0);
  Actor_FaceDirection(6, 0, 0);
  Actor_FaceDirection(7, 0, 0);
  t278 = gMainRoutineWork.currentOwner;
  Actor_FaceDirection(t278, 0x8000, 20);
  Actor_ShowEmote(19, 0x100, 20);
  Event_ShowMessage(19, 0);
  FieldActor_SetAnimation(18, 10);
  FieldEvent_Wait(20);
  FieldActor_SetAnimation(18, 1);
  FieldEvent_Wait(10);
  FieldActor_SetAnimation(18, 10);
  FieldEvent_Wait(5);
  FieldActor_SetAnimation(18, 1);
  FieldEvent_Wait(20);
  Event_ShowMessage(19, 0);
  Actor_FaceDirection(0, 0xc000, 0);
  Actor_FaceDirection(1, 0xc000, 0);
  Actor_FaceDirection(2, 0xc000, 0);
  Actor_FaceDirection(3, 0xc000, 0);
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_FaceDirection(6, 0xc000, 0);
  Actor_FaceDirection(7, 0xc000, 0);
  t298 = gMainRoutineWork.currentOwner;
  Actor_FaceDirection(t298, 0xc000, 0);
  Actor_ShowEmote(18, 0x105, 80);
  Actor_SetAnimation(5, 4);
  Event_ShowMessage(5, 0);
  FieldActor_SetAnimation(18, 9);
  FieldEvent_Wait(20);
  Event_ShowMessage(18, 0);
  Actor_ShowEmote(19, 0x103, 20);
  Event_ShowMessage(19, 0);
  FieldActor_SetAnimation(18, 10);
  FieldEvent_Wait(20);
  Event_ShowMessage(18, 0);
  FieldActor_SetAnimation(18, 9);
  FieldEvent_Wait(20);
  FieldActor_SetAnimation(18, 3);
  FieldEvent_Wait(10);
  FieldActor_SetAnimation(18, 6);
  FieldEvent_Wait(10);
  Func_020053e4(actor, (s32)gData_02006c20);
  FieldEvent_Wait(80);
  FieldActor_SetAnimation(18, 3);
  FieldEvent_Wait(10);
  Event_ShowMessage(18, 0);
  Actor_ShowEmote(19, 0x101, 20);
  Event_ShowMessage(19, 0);
  FieldActor_SetAnimation(18, 3);
  FieldEvent_Wait(20);
  Event_ShowMessage(18, 0);
  Actor_FaceDirection(0, 0, 0);
  Actor_FaceDirection(1, 0x8000, 0);
  Actor_FaceDirection(2, 0, 0);
  Actor_FaceDirection(3, 0x8000, 0);
  Actor_FaceDirection(5, 0x8000, 0);
  Actor_FaceDirection(6, 0, 0);
  Actor_FaceDirection(7, 0, 0);
  t334 = gMainRoutineWork.currentOwner;
  Actor_FaceDirection(t334, 0x8000, 20);
  FieldActor_SetAnimation(18, 6);
  FieldEvent_Wait(20);
  Event_PlaySound(142);
  PaletteFade_ApplyTarget(0x10005, 0);
  PaletteFade_Interpolate(40);
  FieldEvent_Wait(40);
  PaletteFade_ApplyTarget(0x10003, 1);
  PaletteFade_ApplyTarget(0x10000, 2);
  PaletteFade_Interpolate(40);
  FieldEvent_Wait(40);
  FieldActor_SetAnimation(18, 3);
  FieldEvent_Wait(20);
  Event_ShowMessage(18, 0);
  Actor_FaceDirection(0, 0xc000, 0);
  Actor_FaceDirection(1, 0xc000, 0);
  Actor_FaceDirection(2, 0xc000, 0);
  Actor_FaceDirection(3, 0xc000, 0);
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_FaceDirection(6, 0xc000, 0);
  Actor_FaceDirection(7, 0xc000, 0);
  t356 = gMainRoutineWork.currentOwner;
  Actor_FaceDirectionAndWait(t356, 0xc000);
  Event_ShowMessage(5, 0);
  FieldActor_SetAnimation(18, 4);
  FieldEvent_Wait(20);
  Event_ShowMessage(18, 0);
  Actor_ShowEmote(19, 0x101, 20);
  Event_ShowMessage(19, 0);
  FieldEvent_Wait(20);
  FieldActor_SetAnimation(18, 13);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(18, 0, 20);
  Event_ShowMessage(19, 0);
  FieldActor_SetAnimation(18, 8);
  FieldEvent_Wait(180);
  Event_ShowMessage(18, 0);
  Actor_ShowEmote(2, 0x107, 0);
  Event_ShowMessage(2, 0);
  Actor_FaceDirection(19, 0, 0);
  FieldActor_SetAnimation(18, 1);
  FieldEvent_Wait(5);
  Actor_SetAnimation(18, 9);
  Actor_ShowEmote(18, 0x105, 40);
  Actor_ShowEmote(19, 0x100, 20);
  Event_ShowMessage(19, 0);
  Func_020054e4(actor);
  FieldActor_SetAnimation(18, 1);
  FieldEvent_Wait(20);
  Event_ShowMessage(18, 0);
  Actor_ShowEmote(7, 0x103, 0);
  Actor_FaceDirection(7, 0x2000, 0);
  FieldActor_Jump(7, 4, 10);
  Event_ShowMessage(0x4007, 0);
  FieldActor_SetAnimation(18, 10);
  FieldEvent_Wait(20);
  FieldActor_SetAnimation(18, 11);
  FieldEvent_Wait(5);
  FieldActor_SetAnimation(18, 1);
  FieldEvent_Wait(20);
  Actor_ShowEmote(1, 0x107, 0);
  Actor_FaceDirectionAndWait(1, 0x2000);
  Event_ShowMessage(0x5001, 0);
  Actor_ShowEmote(3, 0x109, 0);
  Actor_SetAnimation(3, 4);
  Event_ShowMessage(3, 0);
  Actor_SetAnimation(0, 4);
  Event_ShowMessage(0, 0);
  Actor_FaceDirection(7, 0, 0);
  Actor_FaceDirection(6, 0xe000, 0);
  Actor_FaceDirection(5, 0xe000, 0);
  t406 = gMainRoutineWork.currentOwner;
  Actor_FaceDirection(t406, 0, 0);
  Actor_FaceDirection(19, 0xd000, 0);
  Actor_FaceDirection(3, 0xa000, 0);
  Actor_FaceDirection(1, 0x8000, 20);
  Actor_FaceDirectionAndWait(0, 0x8000);
  Actor_SetAnimation(0, 4);
  Event_OpenMessage(0x4000, 0);
  t414 = gMainRoutineWork.currentOwner;
  t415 = FieldEvent_AskYesNoWithActorAnimation(t414, 0);
  if (t415 != 0)
    goto L_02009464;
  Actor_SetAnimation(6, 3);
  FieldActor_Jump(6, 4, 10);
  Event_ShowMessage(6, 0);
  t419 = (s32)Ram_HeapSlots->eventWork;
  t420 = FIELD_AT(t419, u16, 452);
  FIELD_AT(t419, u16, 452) = (t420 + 1);
  goto L_02009490;
L_02009464:
  Actor_SetAnimation(6, 4);
  FieldActor_Jump(6, 4, 10);
  t423 = (s32)Ram_HeapSlots->eventWork;
  t424 = FIELD_AT(t423, u16, 452);
  FIELD_AT(t423, u16, 452) = (t424 + 1);
  Event_ShowMessage(6, 0);
L_02009490:
  Actor_FaceDirection(0, 0xc000, 0);
  Actor_FaceDirection(1, 0xc000, 0);
  Actor_FaceDirection(2, 0xc000, 0);
  Actor_FaceDirection(3, 0xc000, 0);
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_FaceDirection(6, 0xc000, 0);
  Actor_FaceDirection(7, 0xc000, 0);
  t433 = gMainRoutineWork.currentOwner;
  Actor_FaceDirection(t433, 0xc000, 0);
  FieldActor_SetAnimation(18, 11);
  FieldEvent_Wait(20);
  Event_ShowMessage(18, 0);
  Actor_FaceDirectionAndWait(6, 0);
  Actor_ShowEmote(6, 0x109, 20);
  Actor_FaceDirection(5, 0x8000, 40);
  Actor_ShowEmote(5, 0x100, 20);
  Actor_FaceDirectionAndWait(5, 0xc000);
  Event_ShowMessage(5, 0);
  Actor_SetAnimation(18, 8);
  Actor_FaceDirection(19, 0x8000, 0);
  Actor_FaceDirection(2, 0x8000, 0);
  Actor_FaceDirection(3, 0x8000, 0);
  Actor_FaceDirection(1, 0x6000, 0);
  Actor_FaceDirection(0, 0x6000, 0);
  Actor_FaceDirection(7, 0x2000, 0);
  t451 = gMainRoutineWork.currentOwner;
  Actor_FaceDirection(t451, 0x4000, 0);
  Actor_WaitForAnimationChange(18);
  Actor_SetAnimation(18, 1);
  Actor_ShowEmote(2, 0x108, 20);
  Event_ShowMessage(0x4002, 0);
  Actor_FaceDirectionAndWait(1, 0x8000);
  Event_ShowMessage(0x4001, 0);
  Actor_FaceDirectionAndWait(0, 0x8000);
  Event_OpenMessage(0x4000, 0);
  t461 = gMainRoutineWork.currentOwner;
  t462 = FieldEvent_AskYesNoWithActorAnimation(t461, 0);
  if (t462 != 0)
    goto L_02009624;
  t463 = gMainRoutineWork.currentOwner;
  Actor_WaitForAnimationChange(t463);
  t465 = (s32)Ram_HeapSlots->eventWork;
  t466 = FIELD_AT(t465, u16, 452);
  FIELD_AT(t465, u16, 452) = (t466 + 1);
  goto L_02009638;
L_02009624:
  Actor_SetAnimation(1, 4);
  Event_ShowMessage(0x4001, 0);
L_02009638:
  work = &gMainRoutineWork;
  t469 = work->currentOwner;
  Actor_FaceDirectionAndWait(t469, 0);
  t471 = work->currentOwner;
  FieldActor_SetAnimationAndWait(t471, 3);
  t473 = work->currentOwner;
  Actor_FaceDirectionAndWait(t473, 0xc000);
  Actor_FaceDirection(7, 0xc000, 0);
  Actor_FaceDirection(0, 0xc000, 0);
  Actor_FaceDirection(1, 0xc000, 0);
  Actor_FaceDirection(6, 0xc000, 0);
  Actor_FaceDirection(19, 0xc000, 0);
  Actor_FaceDirection(2, 0xc000, 0);
  Actor_FaceDirection(3, 0xc000, 0);
  t482 = work->currentOwner;
  Actor_SetSpeed(t482, 0x6666, 0x3333);
  t484 = work->currentOwner;
  Actor_WalkToAndWait(t484, 0x1f8, 0x134);
  Event_PlaySound(201);
  FieldActor_SetAnimation(18, 14);
  FieldEvent_Wait(10);
  Actor_SetSpriteColorById(18, 0x100);
  t490 = work->currentOwner;
  Actor_RunRepeatedMotion(t490, 2);
  t492 = work->currentOwner;
  Actor_SetSpeed(t492, 0x19999, 0xcccc);
  t494 = work->currentOwner;
  FieldActor_Jump(t494, 4, 0);
  t496 = work->currentOwner;
  t497 = Actor_Get(t496);
  {
    u8 *f = (u8 *)t497 + 90;
    *f &= 254;
  }
  t499 = work->currentOwner;
  Actor_WalkToAndWait(t499, 0x1f8, 0x140);
  FieldEvent_Wait(1);
  t502 = work->currentOwner;
  t503 = Actor_Get(t502);
  FIELD_AT(t503, u8, 90) |= 1;
  Actor_SetAttachedEffect(7, 0x102);
  FieldEvent_Wait(20);
  Event_ShowMessage(7, 0);
  FieldActor_SetAnimation(18, 14);
  FieldEvent_Wait(10);
  FieldActor_SetAnimation(18, 10);
  FieldEvent_Wait(20);
  Event_ShowMessage(18, 0);
  FieldActor_SetAnimation(18, 9);
  FieldEvent_Wait(20);
  Event_ShowMessage(18, 0);
  Actor_FaceDirection(0, 0, 0);
  Actor_FaceDirection(1, 0x8000, 0);
  Actor_FaceDirection(2, 0, 0);
  Actor_FaceDirection(3, 0x8000, 0);
  Actor_FaceDirection(5, 0x8000, 0);
  Actor_FaceDirection(6, 0, 0);
  Actor_FaceDirection(7, 0, 0);
  t523 = work->currentOwner;
  Actor_FaceDirection(t523, 0x8000, 40);
  Event_ShowMessage(7, 0);
  Event_PlaySound(0x23c);
  Actor_SetSpriteColorById(18, 0);
  t528 = Actor_Get(18);
  FieldActor_SetSpriteColor(t528, 7);
  FieldEvent_Wait(20);
  t531 = Actor_Get(18);
  FieldActor_SetSpriteColor(t531, 0);
  Actor_FaceDirection(0, 0xc000, 0);
  Actor_FaceDirection(1, 0xc000, 0);
  Actor_FaceDirection(2, 0xc000, 0);
  Actor_FaceDirection(3, 0xc000, 0);
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_FaceDirection(6, 0xc000, 0);
  Actor_FaceDirection(7, 0xc000, 0);
  t540 = work->currentOwner;
  Actor_FaceDirection(t540, 0xc000, 20);
  Event_ShowMessage(18, 0);
  Event_ShowMessage(19, 0);
  Actor_SetAnimation(7, 3);
  t545 = work->currentOwner;
  FieldActor_SetAnimation(t545, 3);
  Actor_SetAnimation(0, 3);
  Actor_SetAnimation(1, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(2, 3);
  FieldActor_SetAnimationAndWait(3, 3);
  Event_PlaySound(78);
  FIELD_AT(actor, s32, 108) = (s32)Func_02000068;
  Actor_SetScript(18, (s32)gData_02005c70);
  PaletteFade_ApplyTarget(0x10001, 1);
  PaletteFade_Interpolate(20);
  System_Wait(20);
  PaletteFade_ApplyTarget(0, 0);
  PaletteFade_Interpolate(40);
  System_Wait(40);
  Actor_SetPosition(7, 0, 0);
  t562 = work->currentOwner;
  Actor_SetPosition(t562, 0, 0);
  Actor_SetPosition(0, 0, 0);
  Actor_SetPosition(1, 0, 0);
  Actor_SetPosition(6, 0, 0);
  Actor_SetPosition(5, 0, 0);
  Actor_SetPosition(19, 0, 0);
  Actor_SetPosition(2, 0, 0);
  Actor_SetPosition(3, 0, 0);
  System_Wait(1);
  actor = Actor_Get(17);
  FIELD_AT(actor, s32, 24) = 0x1000;
  FIELD_AT(actor, s32, 28) = 0x1000;
  Actor_SetPosition(17, 0x2000000, 0x1180000);
  System_Wait(1);
  unk85 = (u8 *)actor + 85;
  *unk85 = 0;
  FieldActor_SetPositionAndResetMotion(actor, 0x2000000, 0x300000, 0x1180000);
  System_Wait(1);
  *unk85 = 0;
  PaletteFade_ApplyTarget(0x10000, 2);
  PaletteFade_Interpolate(20);
  dispcnt = (u16 *)0x4000000;
  Event_PlaySound(39);
  Actor_RunScriptAndWait(17, (s32)gData_02005cac);
  *dispcnt = *dispcnt & 0xf9ff;
  PaletteFade_ApplyTarget(0x10003, 1);
  PaletteFade_Interpolate(40);
  Actor_SetSpeed(17, 0x33333, 0x3333);
  Event_PlaySound(138);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(1);
  System_Wait(1);
  FieldActor_SetMoveTarget(actor, 0x2200000, 0x580000, 0x1180000);
  PaletteFade_ApplyTarget(0x10000, 2);
  PaletteFade_ApplyTarget(0x10003, 1);
  PaletteFade_Interpolate(10);
  FieldActor_WaitForMoveById(17);
  Event_PlaySound(138);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(1);
  System_Wait(1);
  FieldActor_SetMoveTarget(actor, 0x1d00000, 0x580000, 0x1180000);
  PaletteFade_ApplyTarget(0x10000, 2);
  PaletteFade_ApplyTarget(0x10003, 1);
  PaletteFade_Interpolate(10);
  FieldActor_WaitForMoveById(17);
  Event_PlaySound(138);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(1);
  System_Wait(1);
  FieldActor_SetMoveTarget(actor, 0x2400000, 0x580000, 0x1180000);
  PaletteFade_ApplyTarget(0x10000, 2);
  PaletteFade_ApplyTarget(0x10003, 1);
  PaletteFade_Interpolate(10);
  FieldActor_WaitForMoveById(17);
  Actor_SetSpeed(17, 0x66666, 0x33333);
  Event_PlaySound(138);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(1);
  System_Wait(1);
  FieldActor_SetMoveTarget(actor, 0x1fc0000, 0xd00000, 0x1200000);
  PaletteFade_ApplyTarget(0x10000, 2);
  PaletteFade_ApplyTarget(0x10003, 1);
  PaletteFade_Interpolate(10);
  FieldActor_WaitForMoveById(17);
  Event_PlaySound(138);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(1);
  System_Wait(1);
  *dispcnt |= 0x600;
  Actor_SetPosition(7, 0x1e80000, 0x1400000);
  t628 = work->currentOwner;
  Actor_SetPosition(t628, 0x1f80000, 0x1400000);
  Actor_SetPosition(0, 0x2080000, 0x1400000);
  Actor_SetPosition(1, 0x2180000, 0x1400000);
  Actor_SetPosition(6, 0x1e00000, 0x1480000);
  Actor_SetPosition(5, 0x1f00000, 0x1480000);
  Actor_SetPosition(19, 0x2000000, 0x1480000);
  Actor_SetPosition(2, 0x2100000, 0x1480000);
  Actor_SetPosition(3, 0x2200000, 0x1480000);
  System_Wait(1);
  PaletteFade_ApplyTarget(0x10003, 1);
  PaletteFade_ApplyTarget(0x10000, 2);
  PaletteFade_Interpolate(10);
  System_Wait(20);
  Event_PlaySound(138);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(1);
  System_Wait(1);
  PaletteFade_ApplyTarget(0x10003, 1);
  PaletteFade_ApplyTarget(0x10000, 2);
  PaletteFade_Interpolate(40);
  System_Wait(40);
  FIELD_AT(actor, s32, 72) = 0x4000;
  *unk85 = 3;
  FieldEvent_Wait(30);
  Event_PlaySound(208);
  MapGrid_SetShake(0x20000, 0x40000, 0x10000);
  FieldEvent_Wait(10);
  MapGrid_SetShake(-1, -1, 0xe666);
  FieldMapGrid_WaitForShakeToSettle();
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(17, 1);
  Event_PlaySound(148);
  FieldEvent_Wait(80);
  Event_ShowMessage(6, 0);
  Actor_ShowEmote(7, 0x103, 0);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(1, 0x103, 0);
  Event_ShowMessage(1, 0);
  Actor_SetAnimation(3, 3);
  Event_ShowMessage(3, 0);
  Actor_FaceDirectionAndWait(2, 0xa000);
  Event_ShowMessage(2, 0);
  Actor_SetSpeed(7, 0x19999, 0xcccc);
  t670 = work->currentOwner;
  Actor_SetSpeed(t670, 0x19999, 0xcccc);
  Actor_SetSpeed(0, 0x19999, 0xcccc);
  Actor_SetSpeed(1, 0x19999, 0xcccc);
  Actor_SetSpeed(6, 0x19999, 0xcccc);
  Actor_SetSpeed(5, 0x19999, 0xcccc);
  Actor_SetSpeed(2, 0x19999, 0xcccc);
  Actor_SetSpeed(3, 0x19999, 0xcccc);
  Actor_SetScript(7, (s32)gData_02005df8);
  t679 = work->currentOwner;
  Actor_SetScript(t679, (s32)gData_02005ce8);
  Actor_SetScript(0, (s32)gData_02005d2c);
  Actor_SetScript(6, (s32)gData_02005db4);
  Actor_SetScript(5, (s32)gData_02005d70);
  Actor_SetScript(2, (s32)gData_02005e3c);
  Actor_SetScript(3, (s32)gData_02005e80);
  Actor_RunScriptAndWait(1, (s32)gData_02005ec4);
  Actor_ShowEmote(19, 0x101, 40);
  Event_ShowMessage(19, 0);
  Actor_SetAttachedEffect(19, 0x102);
  Actor_FaceDirectionAndWait(19, 0xa000);
  Event_ShowMessage(0x2013, 0);
  Actor_FaceDirectionAndWait(1, 0x6000);
  FieldActor_Jump(1, 6, 10);
  Event_ShowMessage(1, 0);
  FieldActor_Jump(19, 2, 10);
  FieldActor_Jump(19, 4, 10);
  Event_ShowMessage(0x2013, 0);
  FIELD_AT(work, u8, 0x24b) = 2;
  FieldEvent_SetAfterBattleDestination((s32)&SceneId_Ov6aeRoom1, 2);
  Call2(Field_RequestBattleFromEncounterTable, 102, 1);
  return;
}

void Func_02001eb0(struct Actor *object) {
  if ((gData_02006b64 >> gData_02006b6c) & 1)
    FieldActor_SetSpriteColor(object, 15);
  else
    FieldActor_SetSpriteColor(object, 0);
  gData_02006b64++;
}

void Func_02001ee4(struct Actor *object) {
  if ((gData_02006b64 >> gData_02006b6c) & 1)
    FieldActor_SetSpriteColor(object, 0);
  else
    FieldActor_SetSpriteColor(object, 15);
}

extern u32 gData_02005f08[];
extern u32 gData_02005f50[];
extern u32 gData_02005f98[];
extern u32 gData_02005fdc[];
extern u32 gData_02006004[];
extern u32 gData_0200602c[];
extern u32 gData_02006070[];

/// ★ Named from the game: "...Who are they?" (MsgWhoAreThey)
void MarsLighthouseAerie_FindWhoTheDragonWas(void) {
  s32 lastArg, spill;
  s32 owner, actor, owner2, t1;
  s32 t2, t16, t34;
  s32 t85, t86, t87;
  s32 t158, t162;
  s32 t166, t170;
  s32 t244, t248;

  actor = Actor_Get(17);
  t1 = Actor_Get(30);
  t2 = Actor_Get(31);
  spill = (Actor_Get(32));
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(17, 2);
  FIELD_AT(actor, u8, 85) = 0;
  FIELD_AT(actor, s32, 8) -= 0x40000;
  FIELD_AT(actor, s32, 16) += 0x100000;
  Actor_SetAnimation(28, 2);
  Actor_SetAnimation(29, 3);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  System_Wait(1);
  Actor_SetAnimation(30, 7);
  Actor_SetAnimation(31, 7);
  Actor_SetAnimation(32, 6);
  owner = (s32)&gMainRoutineWork;
  owner = (owner + 0x214);
  t16 = *(s32 *)owner;
  lastArg = 0xc000;
  Actor_SetPositionAndFacing(t16, 0x1f80000, 0x1400000, lastArg);
  System_Wait(1);
  PaletteFade_ApplySource(0, 0);
  PaletteFade_ApplyTarget(0, 0);
  PaletteFade_Interpolate(1);
  FieldActor_SetSpriteColor((Actor_Get(30)), 15);
  FieldActor_SetSpriteColor((Actor_Get(31)), 15);
  FieldActor_SetSpriteColor((Actor_Get(32)), 15);
  System_Wait(1);
  FieldActor_SetSpritePriority(21, 1);
  FieldActor_SetSpritePriority(20, 1);
  FieldActor_SetSpritePriority(19, 1);
  FieldActor_SetSpritePriority(26, 1);
  FieldActor_SetSpritePriority(25, 1);
  t34 = (s32)Ram_HeapSlots->eventWork;
  FIELD_AT(t34, s32, 428) = 0x204;
  FIELD_AT(t34, s32, 436) = 1;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(0x186);
  PaletteFade_ApplyTarget(0x10003, 1);
  PaletteFade_ApplyTarget(0x10000, 2);
  PaletteFade_Interpolate(16);
  System_Wait(16);
  FieldEvent_Wait(80);
  *(s32 *)&gData_02006b64 = 0;
  *(s32 *)&gData_02006b6c = 4;
  FIELD_AT((Actor_Get(17)), s32, 108) = (s32)Func_02001eb0;
  FIELD_AT((Actor_Get(27)), s32, 108) = (s32)Func_02001eb0;
  FIELD_AT((Actor_Get(28)), s32, 108) = (s32)Func_02001eb0;
  FIELD_AT((Actor_Get(29)), s32, 108) = (s32)Func_02001eb0;
  FIELD_AT(t1, s32, 108) = (s32)Func_02001ee4;
  FIELD_AT(t2, s32, 108) = (s32)Func_02001ee4;
  FIELD_AT((spill), s32, 108) = (s32)Func_02001ee4;
  Event_PlaySound(0x253);
  System_Wait(10);
  Event_PlaySound(0x253);
  System_Wait(10);
  Event_PlaySound(0x253);
  System_Wait(10);
  Event_PlaySound(0x253);
  System_Wait(10);
  Event_PlaySound(0x253);
  System_Wait(40);
  *(s32 *)&gData_02006b6c = 3;
  System_Wait(30);
  *(s32 *)&gData_02006b6c = 2;
  System_Wait(20);
  Actor_SetPosition(17, 0, 0);
  Actor_SetPosition(27, 0, 0);
  Actor_SetPosition(28, 0, 0);
  Actor_SetPosition(29, 0, 0);
  FIELD_AT((Actor_Get(17)), s32, 108) = 0;
  FIELD_AT((Actor_Get(27)), s32, 108) = 0;
  FIELD_AT((Actor_Get(28)), s32, 108) = 0;
  FIELD_AT((Actor_Get(29)), s32, 108) = 0;
  FIELD_AT(t1, s32, 108) = 0;
  FIELD_AT(t2, s32, 108) = 0;
  FIELD_AT((spill), s32, 108) = 0;
  System_Wait(1);
  Event_PlaySound(103);
  FieldActor_SetSpriteColor((Actor_Get(30)), 9);
  FieldActor_SetSpriteColor((Actor_Get(31)), 9);
  FieldActor_SetSpriteColor((Actor_Get(32)), 9);
  FieldEvent_Wait(20);
  FieldActor_SetSpriteColor((Actor_Get(30)), 0);
  FieldActor_SetSpriteColor((Actor_Get(31)), 0);
  FieldActor_SetSpriteColor((Actor_Get(32)), 0);
  FieldEvent_Wait(20);
  t85 = Random16();
  FIELD_AT(t1, u8, 98) = (((u32)(((t85 << 2) + t85) << 4) >> 16) + 40);
  t86 = Random16();
  FIELD_AT(t2, u8, 98) = (((u32)(((t86 << 2) + t86) << 4) >> 16) + 40);
  t87 = Random16();
  FIELD_AT((spill), u8, 98) = (((u32)(((t87 << 2) + t87) << 4) >> 16) + 40);
  FIELD_AT(t1, s32, 108) = (s32)Func_0200008c;
  FIELD_AT(t2, s32, 108) = (s32)Func_0200008c;
  FIELD_AT((spill), s32, 108) = (s32)Func_0200008c;
  FieldEvent_Wait(20);
  PaletteFade_ApplyTarget(0x10000, 1);
  PaletteFade_Interpolate(40);
  System_Wait(80);
  FieldEvent_SetMessageId((s32)MsgWhoAreThey);
  Actor_FaceDirectionAndWait(26, 0x8000);
  Event_ShowMessage(26, 0);
  Event_ShowMessage(19, 0);
  Actor_FaceDirection(22, 0x2000, 0);
  Actor_FaceDirection((*(s32 *)owner), 0x2000, 0);
  Actor_FaceDirection(23, 0x4000, 0);
  Actor_FaceDirection(24, 0x4000, 0);
  Actor_FaceDirection(21, 0x2000, 0);
  Actor_FaceDirection(20, 0x2000, 0);
  Actor_FaceDirection(26, 0x4000, 0);
  Actor_FaceDirection(25, 0x4000, 0);
  FieldCamera_SetSpeed(0x19999, 0x3333);
  FIELD_AT((EventCamera_GetActor()), u8, 85) = 0;
  FieldCamera_MoveTo(0x2000000, 0x500000, 0x1540000, 1);
  FieldCamera_WaitForArrival();
  Actor_ShowEmote(19, 0x109, 20);
  Event_ShowMessage(19, 0);
  Event_PlaySound(8);
  Actor_SetAttachedEffect(24, 0x102);
  FieldEvent_Wait(20);
  Event_ShowMessage(24, 0);
  Event_ShowMessage(25, 0);
  Actor_SetAnimationAndWait(19, 3);
  Event_ShowMessage(19, 0);
  Actor_SetAttachedEffect(26, 0x102);
  Event_ShowMessage(0x101a, 0);
  Actor_SetAnimation(19, 4);
  Event_ShowMessage(19, 0);
  Actor_ShowEmote(23, 0x103, 0);
  Actor_SetSpeed(23, 0x19999, 0xcccc);
  Actor_WalkToAndWait(23, 0x200, 0x148);
  Actor_FaceDirectionAndWait(23, 0x2000);
  Event_ShowMessage(0x1017, 0);
  Actor_FaceDirectionAndWait(20, 0x8000);
  Event_ShowMessage(0xa014, 0);
  Actor_ShowEmote(19, 0x100, 20);
  FieldActor_SetSpritePriority(21, 2);
  FIELD_AT((Actor_Get(20)), u8, 98) = 0;
  FIELD_AT((Actor_Get(21)), u8, 98) = 0;
  Actor_SetSpeed(21, 0xcccc, 0x6666);
  Actor_SetSpeed(20, 0xcccc, 0x6666);
  Actor_SetScript(21, (s32)gData_02005f08);
  Actor_SetScript(20, (s32)gData_02005f50);
  Actor_SetSpeed(19, 0x26666, 0x13333);
  Actor_WalkToAndWait(19, 0x21c, 0x154);
  FieldActor_Jump(19, 4, 10);
  Event_ShowMessage(19, 0);
  actor = Actor_Get(21);
L_0200a47a:
  System_Wait(1);
  if (FIELD_AT(actor, u8, 98) == 0)
    goto L_0200a47a;
  Actor_FaceDirection(20, 0x2000, 0);
  Actor_FaceDirectionAndWait(21, 0x2000);
  Event_ShowMessage(21, 0);
  FieldActor_SetSpritePriority(20, 2);
  FieldActor_SetSpritePriority(22, 2);
  FieldActor_ShowEmote(20, 0x100, 0);
  Actor_SetSpeed(20, 0x19999, 0xcccc);
  Actor_WalkToAndWait(20, 0x1ea, 0x128);
  Actor_FaceDirection(21, 0xe000, 20);
  Actor_FaceDirection(20, 0xa000, 20);
  Actor_FaceDirection(20, 0xe000, 20);
  Actor_SetSpeed(20, 0x3333, 0x1999);
  t158 = Actor_Get(20);
  FIELD_AT(t158, u8, 90) &= 254;
  Actor_WalkToAndWait(20, 0x1e8, 0x12c);
  FieldEvent_Wait(1);
  t162 = Actor_Get(20);
  FIELD_AT(t162, u8, 90) |= 1;
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(20, 2);
  t166 = Actor_Get(20);
  FIELD_AT(t166, u8, 90) &= 254;
  Actor_WalkToAndWait(20, 0x1e6, 0x134);
  FieldEvent_Wait(1);
  t170 = Actor_Get(20);
  FIELD_AT(t170, u8, 90) |= 1;
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(0x2000000, 0x600000, 0x1400000, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(21, 0, 40);
  Actor_FaceDirection(21, 0xe000, 20);
  Actor_FaceDirectionAndWait(21, 0);
  Event_ShowMessage(0x8015, 0);
  Actor_FaceDirection(22, 0xc000, 0);
  owner2 = (s32)&gMainRoutineWork;
  owner2 = (owner2 + 0x214);
  Actor_FaceDirection((*(s32 *)owner2), 0xa000, 0);
  Actor_FaceDirection(23, 0xa000, 0);
  Actor_FaceDirection(24, 0xa000, 0);
  Actor_FaceDirection(26, 0xa000, 0);
  Actor_FaceDirection(25, 0xa000, 0);
  Event_ShowMessage(0x8014, 0);
  actor = Actor_Get(32);
  FIELD_AT(actor, s32, 108) = 0;
  System_Wait(1);
  FIELD_AT(actor, s32, 24) = 0x10000;
  FIELD_AT(actor, s32, 28) = 0x10000;
  Actor_RunRepeatedMotion(32, 2);
  Event_ShowMessage(32, 0);
  Actor_ShowEmote(23, 0x100, 0);
  Actor_FaceDirection(23, 0xe000, 40);
  Actor_FaceDirection(25, 0x8000, 0);
  Actor_ShowEmote(25, 0x101, 20);
  Event_ShowMessage(25, 0);
  Actor_FaceDirection(23, 0, 40);
  Actor_FaceDirectionAndWait(23, 0xc000);
  FieldActor_StartRepeatedMotion(32, 3);
  Event_ShowMessage(32, 0);
  Actor_FaceDirection(24, 0xc000, 40);
  Actor_FaceDirectionAndWait(24, 0x6000);
  Event_ShowMessage(0x4018, 0);
  Actor_FaceDirectionAndWait(21, 0x2000);
  Actor_SetAttachedEffect(21, 0x102);
  Event_ShowMessage(21, 0);
  Actor_FaceDirection((*(s32 *)owner2), 0xc000, 0);
  Actor_FaceDirection(24, 0xc000, 0);
  Actor_FaceDirection(26, 0xc000, 0);
  Actor_FaceDirection(25, 0xc000, 0);
  actor = Actor_Get(30);
  FIELD_AT(actor, s32, 108) = 0;
  System_Wait(1);
  FIELD_AT(actor, s32, 24) = 0x10000;
  FIELD_AT(actor, s32, 28) = 0x10000;
  actor = Actor_Get(31);
  FIELD_AT(actor, s32, 108) = 0;
  System_Wait(1);
  FIELD_AT(actor, s32, 24) = 0x10000;
  FIELD_AT(actor, s32, 28) = 0x10000;
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(31, 1);
  Event_ShowMessage(0x201f, 0);
  Actor_RunRepeatedMotion(30, 2);
  Event_ShowMessage(30, 0);
  FieldActor_Jump(20, 2, 10);
  FieldActor_Jump(20, 4, 10);
  Event_ShowMessage(0x8014, 0);
  Actor_SetSpeed(20, 0x26666, 0x13333);
  Actor_SetSpeed((*(s32 *)owner2), 0x26666, 0x13333);
  Actor_SetSpeed(23, 0x26666, 0x13333);
  Actor_SetSpeed(24, 0x26666, 0x13333);
  Actor_SetScript(20, (s32)gData_02005f98);
  Actor_SetScript((*(s32 *)owner2), (s32)gData_02005fdc);
  Actor_SetScript(23, (s32)gData_02006004);
  Actor_RunScriptAndWait(24, (s32)gData_0200602c);
  FieldEvent_Wait(20);
  Actor_SetSpeed(22, 0x19999, 0xcccc);
  Actor_WalkToAndWait(22, 0x210, 0x140);
  Actor_FaceDirection(22, 0x2000, 0);
  Actor_FaceDirectionAndWait(25, 0xa000);
  Event_ShowMessage(0x5016, 0);
  Actor_SetSpeed(25, 0x19999, 0xcccc);
  Actor_SetScript(25, (s32)gData_02006070);
  Actor_WalkToAndWait(22, 0x1e6, 0x134);
  Actor_FaceDirectionAndWait(22, 0xa000);
  Actor_SetSpeed(21, 0x26666, 0x13333);
  t244 = Actor_Get(21);
  FIELD_AT(t244, u8, 90) &= 254;
  Actor_WalkToAndWait(21, 0x1da, 0x13c);
  FieldEvent_Wait(1);
  t248 = Actor_Get(21);
  FIELD_AT(t248, u8, 90) |= 1;
  Actor_FaceDirection(21, 0xe000, 0);
  Actor_WalkToAndWait(22, 0x1ce, 0x118);
  Actor_FaceDirectionAndWait(22, 0);
  FieldActor_SetSpritePriority(22, 1);
  Func_020053e4((Actor_Get(22)), (s32)gData_02006c20);
  Func_020053e4((Actor_Get(25)), (s32)gData_02006b70);
  FieldEvent_Wait(40);
  Actor_SetAnimationAndWait(19, 4);
  Event_ShowMessage(19, 0);
  Event_PlaySound(78);
  PaletteFade_ApplyTarget(0, 0);
  PaletteFade_Interpolate(120);
  System_Wait(120);
  Func_020054e4(Actor_Get(22));
  Func_020054e4(Actor_Get(25));
  FieldEvent_Wait(80);
  FieldEvent_SetExitRequest(2);
  return;
}

void Func_0200298c(s32 id) {
  struct Actor *object = Actor_Get(id);

  object->collisionFlags = 0;
  object->motionFlags = 0;
  FieldActor_SetSpritePriority(id, 1);
}

/// The scene that opens with MsgWhatSTheMatter; either answer to its prompt
/// steps the event's message number.
void MarsLighthouseAerie_UrgeOnToBeacon(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  System_Wait(1);
  Actor_SetPosition(gMainRoutineWork.currentOwner, PIXELS(506), PIXELS(280));
  System_Wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldActor_SetSpritePriority(3, 1);
  Func_020053e4((s32)Actor_Get(7), (s32)gData_02006c20);
  Func_020053e4((s32)Actor_Get(3), (s32)gData_02006b70);
  FieldEvent_Wait(120);
  Func_020054e4((s32)Actor_Get(7));
  Func_020054e4((s32)Actor_Get(3));
  FieldEvent_Wait(40);
  Actor_SetAnimationAndWait(7, 4);
  Actor_FaceDirectionAndWait(6, FACING_NORTH);
  FieldEvent_SetMessageId(MsgWhatSTheMatter);
  Event_ShowMessage(6, 0);
  Actor_FaceDirectionAndWait(2, FACING_NORTHWEST);
  Event_ShowMessage(0x8002, 0);
  Actor_FaceDirection(3, FACING_SOUTHEAST, 20);
  FieldActor_ShowEmote(3, 0x109, 0x14);
  Event_ShowMessage(0x1003, 0);
  Actor_FaceDirectionAndWait(0, FACING_NORTHEAST);
  Actor_SetAnimationAndWait(7, 4);
  Event_ShowMessage(0x2007, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(5, FACING_NORTHEAST);
  FieldActor_ShowEmote(5, 0x107, 0);
  FieldActor_Jump(5, 2, 0xa);
  FieldActor_Jump(5, 4, 0xa);
  Event_ShowMessage(0x8005, 0);
  Actor_FaceDirection(7, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(6, FACING_NORTHWEST, 0);
  Actor_FaceDirection(0, FACING_WEST, 0);
  Actor_FaceDirection(1, FACING_WEST, 0);
  Actor_FaceDirection(2, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(3, FACING_WEST);
  Event_ShowMessage(0x8005, 0);
  Actor_SetAnimation(7, 4);
  Event_ShowMessage(0x2007, 0);
  FieldActor_ShowEmote(5, 0x109, 0x14);
  FieldEvent_ShowMessageThenWait(0x8005, 0, 0x28);
  Event_ShowMessage(0x2006, 0);
  Actor_FaceDirectionAndWait(5, FACING_EAST);
  Actor_SetAnimationAndWait(5, 4);
  Event_ShowMessage(0x8005, 0);
  Actor_SetSpeed(5, 0x19999, 0xcccc);
  Actor_SetAnimation(5, 5);
  Actor_MoveToAndWait(5, 460, 288);
  Actor_SetAnimation(5, 0x16);
  Event_ShowMessage(5, 0);
  FieldActor_ShowEmote(0x13, 0x109, 0);
  Event_PlaySound(0x4e);
  FieldCamera_SetSpeed(0x3333, 0x666);
  FieldCamera_MoveTo(PIXELS(512), -1, PIXELS(320), 1);
  FieldCamera_WaitForArrival();
  Event_ShowMessage(0x2013, 0);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(6, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(0, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(1, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(2, FACING_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(3, FACING_SOUTHWEST);
  Actor_SetAnimationAndWait(19, 4);
  FieldEvent_ShowMessageThenWait(0x2013, 0, 0x14);
  Actor_SetAnimation(5, 1);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(5, FACING_SOUTHEAST);
  Event_ShowMessage(5, 0);
  FieldEvent_ShowMessageThenWait(0x2013, 0, 0x14);
  Event_ShowMessage(5, 0);
  Actor_SetAnimationAndWait(19, 3);
  Event_ShowMessage(0x2013, 0);
  Actor_FaceDirection(5, FACING_EAST, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(0, FACING_EAST, 40);
  Actor_SetSpeed(1, 0xcccc, 0x6666);
  Actor_Get(1)->actionFlags &= 0xfe;
  Actor_WalkToAndWait(1, 542, 328);
  FieldEvent_Wait(1);
  Actor_Get(1)->actionFlags |= 1;
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  EventCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(540), PIXELS(76), PIXELS(340), 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirectionAndWait(1, FACING_SOUTH);
  Actor_WalkToAndWait(1, 542, 340);
  FieldActor_Jump(1, 2, 0xa);
  FieldActor_Jump(1, 4, 0xa);
  Event_ShowMessage(0x5001, 0);
  Actor_FaceDirection(1, FACING_SOUTHWEST, 40);
  Actor_FaceDirection(1, FACING_SOUTHEAST, 20);
  FieldActor_Jump(1, 4, 0xa);
  Event_ShowMessage(0x5001, 0);
  FieldCamera_SetSpeed(0x4cccc, 0x9999);
  FieldCamera_MoveTo(PIXELS(478), PIXELS(96), PIXELS(312), 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(6, FACING_SOUTH, 40);
  Actor_FaceDirectionAndWait(6, FACING_WEST);
  FieldActor_ShowEmote(6, 0x103, 0x14);
  Actor_FaceDirectionAndWait(6, FACING_SOUTHWEST);
  FieldActor_Jump(6, 4, 0xa);
  Event_ShowMessage(0x6006, 0);
  FieldCamera_MoveTo(PIXELS(480), -1, PIXELS(276), 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(7, FACING_NORTHEAST, 20);
  Actor_FaceDirection(7, FACING_NORTHWEST, 20);
  Actor_FaceDirectionAndWait(7, FACING_NORTH);
  Event_ShowMessage(0x8007, 0);
  Actor_SetAnimationAndWait(7, 4);
  Event_ShowMessage(0x8007, 0);
  Event_ShowMessage(0x5000, 0);
  FieldCamera_MoveTo(PIXELS(512), -1, PIXELS(312), 1);
  Actor_FaceDirection(7, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(3, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(19, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(7, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(1, FACING_NORTH, 40);
  Actor_FaceDirectionAndWait(0, FACING_NORTH);
  FieldEvent_ShowMessageThenWait(0x8000, 0, 0x14);
  {
    s32 third = 0x28;
    s32 second = 0x105;
    s32 actor;

    actor = gMainRoutineWork.currentOwner;
    FieldActor_ShowEmote(actor, second, third);
  }
  Actor_FaceDirectionAndWait(0, FACING_EAST);
  FieldActor_ShowEmote(0, 0x109, 0x14);
  Event_ShowMessage(0x9000, 0);
  Actor_FaceDirectionAndWait(0, FACING_NORTHWEST);
  {
    s32 flags = 0;

    Event_OpenMessage(0x8000, flags);
  }
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
      0) {
    Actor_SetAnimationAndWait(5, 3);
    Event_ShowMessage(0x8005, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    Actor_SetAnimationAndWait(5, 3);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Event_ShowMessage(5, 0);
  }
  Actor_SetSpeed(1, 0xcccc, 0x6666);
  Actor_WalkToAndWait(1, 548, 320);
  Actor_FaceDirectionAndWait(1, FACING_NORTHWEST);
  Actor_SetAnimation(1, 3);
  Event_ShowMessage(1, 0);
  Actor_FaceDirection(6, FACING_NORTHEAST, 0);
  FieldActor_ShowEmote(6, 0x109, 0x28);
  Event_ShowMessage(6, 0);
  Actor_FaceDirection(2, FACING_WEST, 0);
  FieldActor_ShowEmote(2, 0x105, 0x28);
  Event_ShowMessage(0x8002, 0);
  Actor_SetAnimation(7, 3);
  Event_ShowMessage(7, 0);
  Actor_SetAnimation(3, 4);
  Event_ShowMessage(3, 0);
  Actor_SetSpeed(19, 0xcccc, 0x6666);
  Actor_WalkToAndWait(19, 498, 320);
  Actor_SetAnimation(19, 3);
  Event_ShowMessage(0x2013, 0);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTH);
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  Event_SetFlag(0xa3d);
  Event_PlaySound(0x1c);
  Actor_SetSpeed(0, 0xcccc, 0x6666);
  Actor_WalkToAndWait(0, 540, 286);
  Actor_FaceDirectionAndWait(0, FACING_SOUTHEAST);
  Func_0200298c(7);
  Func_0200298c(5);
  Func_0200298c(6);
  Func_0200298c(0x13);
  Func_0200298c(0);
  Func_0200298c(3);
  Func_0200298c(2);
  Func_0200298c(1);
  FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Event_SetFlag(0x202);
  MapGrid_CopyCellAttributes(0x27, 0x28, 0x12, 0x10, 0x17, 0xa);
  Party_RestoreHpPp(1);
  Party_RemoveMemberRestored(7);
  Party_RemoveMemberRestored(5);
  Party_RemoveMemberRestored(6);
  Party_RemoveMemberRestored(0);
  Party_RemoveMemberRestored(3);
  Party_RemoveMemberRestored(2);
  Party_RemoveMemberRestored(1);
  Actor_Get(20)->collisionFlags |= 8;
  Actor_Get(21)->collisionFlags |= 8;
  FieldActor_SetSpriteColor(Actor_Get(28), 0xf);
  Actor_SetPosition(28, PIXELS(568), PIXELS(274));
  Actor_Get(17)->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(Actor_Get(17), PIXELS(492), PIXELS(96),
                                       PIXELS(284));
  Actor_Get(29)->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(Actor_Get(29), PIXELS(510), PIXELS(96),
                                       PIXELS(295));
  Actor_Get(30)->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(Actor_Get(30), PIXELS(540), PIXELS(96),
                                       PIXELS(300));
  System_Wait(1);
  Event_SetFlag(0x17c);
  Event_SetFlag(0x144);
  Event_End();
}

/// Circles the actor around actor 23 by its timer, at its speed plus 3 in x
/// and twice the sine in z, and turns the timer back by 0x800; Func_02000148
/// of the Jupiter Lighthouse's fifth room is the same function.
void Func_02003210(struct Actor *self) {
  struct Actor *centre = Actor_Get(23);
  u16 angle = self->timer;
  s32 sine;
  s32 x;
  s32 z;

  self->x = centre->x + Trig_Cos(angle) * (self->speedLimit + 3);
  sine = Math_Sin(angle);
  z = centre->z;
  x = self->x;
  z += sine * 2;
  self->z = z;
  self->targetX = x;
  self->targetZ = z;
  self->timer += -0x800;
}

static inline struct OrbitEffect *CreateOrbitEffect(s32 type, s32 x, s32 y,
                                                    s32 z) {
  /* FAKEMATCH(wrapper): the effect created through an inline, which
   * evaluates the position before the type; as a direct call of
   * Import_ActorCreate, 4 of 92 instructions differ in tla-en. */
  return (struct OrbitEffect *)Import_ActorCreate(type, x, y, z);
}

/// Every third frame, or every frame once flag 0x200 is set, starts a particle
/// circling actor 23 at a random height above it, as overlay 69e's
/// Func_020001a4 does under flag 0x201.
DEFINE_ORBS_SPAWN_RISING(Orbs_SpawnRising, 0x200, gData_02006afc, Func_02003210,
                         2)

#include "../field/object/park_record.c"

#undef FIELD_AT

#undef Palette_SetColorsFade
// ---- scene/mars_lighthouse_aerie/s3370.c ----

/// Scene code of overlay 0x6ae, in ROM order.

#include "text/msg_ids.h"

void BattleEffect_GetObjectPriority(void);
void Func_02004ec0(void);
void FieldCamera_WaitForArrival();
void Teleport_PlayWhirl(void);

extern s32 gData_02005b54[];

void Actor_StartScript(struct Actor *object, const u8 *script);

void Func_02003370(void) {
  struct Actor *anchor = Actor_GetById(8);
  s32 spawn = 0;
  struct Actor *object;
  struct MarsLighthouseAerieSprite *sprite;

  switch (Data_02006ccc) {
  case 0:
    Event_PlaySound(220);
    MapGrid_SetShake(0x60000, 0x60000, 0x10000);
    PaletteFade_ApplyTarget(0x2063ff, 1);
    PaletteFade_Interpolate(8);
    break;
  case 8:
    PaletteFade_ApplyTarget(0x10000, 1);
    PaletteFade_Interpolate(8);
    break;
  case 16:
    MapGrid_SetShake(0x10000, 0x10000, 0x10000);
    break;
  case 24:
    anchor->x = 0x2000000;
    anchor->y = -0x1680000;
    anchor->z = 0xcc0000;
    anchor->scaleX = 0x10000;
    anchor->scaleY = 0x10000;
    SceneActor_ParkRecord(anchor);
    Actor_SetScript(8, gData_02005b54);
    break;
  case 25:
    Data_02006ccc--;
    if (anchor->y > 0) {
      PaletteFade_ApplyTarget(0x203210, 0);
      PaletteFade_Interpolate(16);
      Data_02006ccc++;
    } else {
      if ((gFrameCount & 7) == 0)
        Event_PlaySound(246);
      spawn = 1;
      anchor->y += 0x24000;
    }
    break;
  case 26:
    Data_02006ccc--;
    if (anchor->y > 0x140000) {
      Data_02006ccc = 999;
      FieldTask_Remove(Func_02003370);
    } else {
      if ((gFrameCount & 7) == 0)
        Event_PlaySound(246);
      anchor->y += 0x24000;
      spawn = 1;
    }
    break;
  }
  if (spawn) {
    object = FieldActor_Create(
        0x2a0, anchor->x, anchor->y - ((Random16() * 80 >> 16) << 16) - 0x80000,
        anchor->z);
    if (object != NULL) {
      u8 zero;
      s32 mask;

      sprite = object->sprite;
      Actor_StartScript(object, gData_02006180);
      FieldActor_SetSpriteColor(object, 1);
      object->motionFlags = 0;
      {
        /* FAKEMATCH(temporary): the masked value held in a local; as
         * object->action = Random16() & 0x0ffff000, 36 of 207 instructions
         * differ in tla-en. */
        s32 action = Random16() & 0x0ffff000;

        object->timer = action;
      }
      zero = 0;
      object->unk66 = 0;
      {
        /* FAKEMATCH(temporary): the byte held in a local; as a direct store, 2
         * of 207 instructions differ in tla-en. */
        u8 phase = Random16() >> 13;

        ((u8 *)object)[98] = phase;
      }
      object->update = Func_020000e4;
      object->speedLimit = Math_Sin(Random16() * 0xffff >> 20) * 24;
      object->speedLimit = anchor->speedLimit >> 16;
      sprite->unk1a = zero;
      mask = -13;
      ((u8 *)sprite)[9] = (((u8 *)sprite)[9] & mask) | 4;
    }
  }
  if (++Data_02006ccc > Data_02006b68)
    FieldTask_Remove(Func_02003370);
}

void Func_020035ec(s32 id) {
  struct Actor *object = Actor_Get(id);

  FieldActor_SetSpritePriority(id, 1);
  FieldActor_SetPositionAndResetMotion(object, object->x - PIXELS(16),
                                       object->y, object->z + PIXELS(32));
}

void Func_0200361c(s32 id) {
  struct Actor *object = Actor_Get(id);

  FieldActor_SetSpritePriority(id, 1);
  FieldActor_SetPositionAndResetMotion(object, object->x + PIXELS(16),
                                       object->y, object->z + PIXELS(32));
}

void Func_02003648(s32 id) { Actor_GetById(id)->motionFlags = 3; }

void Func_02000610(s32 id);
void Func_02000640(s32 id);
extern s32 gData_020060fc[];

#define OWNER gMainRoutineWork.currentOwner

#define A(p) ((struct OverlayActor *)(p))

/// ★ Named from the game: "The beacon is lit!" (MsgTheBeaconIs)
void MarsLighthouseAerie_LightBeacon(void) {
  struct AerieEventWork *work, *later;
  struct OverlaySprite *sprite;
  s32 created, zero, firstBeam, object, tiles, glowTask, message;
  s32 anchor, y, x, z, spawned, block;
  s32 beam, beam2, piece;
  s32 piece2, piece3;
  s32 owner, actor;

  FieldCamera_MoveTo(-1, -1, -1, 0);
  System_Wait(1);
  Actor_SetPosition(28, 0, 0);
  Actor_SetPosition(29, 0, 0);
  Actor_SetPosition(30, 0, 0);
  FieldCamera_SetSpeed(0x19999, 0x3333);
  A(((s32)FieldCamera_GetActor()))->motionFlags = 0;
  FieldCamera_MoveTo(0x2000000, 0x600000, 0x1380000, 1);
  FieldCamera_WaitForArrival();
  System_Wait(1);
  *(u16 *)0x4000052 = 0xc08;
  *(u16 *)0x4000050 = 0x3f10;
  MapGrid_CopyCellAttributes(18, 40, 18, 16, 23, 10);
  Func_02003648(7);
  Func_02003648(5);
  Func_02003648(6);
  firstBeam = 23;
  zero = 0;
  Func_02003648(19);
  Func_02003648(0);
  Func_02003648(3);
  Func_02003648(2);
  Func_02003648(1);
  Event_PrepareActorAndApplyValue(0, 0);
  Actor_SetSpeed((OWNER), 0xcccc, 0x6666);
  Actor_WalkToAndWait((OWNER), 0x200, 0x118);
  Actor_FaceDirectionAndWait((OWNER), 0xc000);
  Actor_RunRepeatedMotion((OWNER), 2);
  FieldEvent_Wait(20);
  PartyInventory_Remove(222);
  anchor = (s32)Actor_Get(4);
  y = A(anchor)->y;
  x = A(anchor)->x;
  y += 0x80000;
  z = A(anchor)->z;
  spawned = FieldActor_Create(0x1e9, x, y, z);
  created = spawned;
  object = spawned;
  if (created == 0)
    goto L_0200b8ac;
  sprite = (struct OverlaySprite *)A(object)->animation;
  sprite->unk5_5 = 0;
  sprite->unk1b = zero;
  sprite->unk9_4 = 0;
  sprite->unk9_2 = 2;
  A(object)->priorityFlags &= 254;
  A(object)->motionFlags = zero;
  A(object)->unk5c = 1;
  A(object)->priorityFlags |= 2;
  Actor_SetSpriteFlags(created, 0);
  A(object)->speedLimit = 0x6666;
  A(object)->acceleration = 0xccc;
  A(object)->gravity = 0x8000;
  block = HeapSlot_Alloc(68, 0x608);
  Item_LoadIcon(247);
  tiles = (block + 0x400);
  Vram_Load(sprite->vramBlock, 128, tiles);
  HeapSlot_Release(68);
  A(object)->motionFlags = zero;
  FieldActor_SetMoveTarget(object, 0x2000000, 0x800000, 0x1180000);
  FieldActor_WaitForMove(object);
  A(object)->motionFlags = 4;
  FieldEvent_Wait(60);
  A(object)->motionFlags = zero;
  FieldActor_SetMoveTarget(object, 0x2000000, 0x800000, 0x1080000);
  FieldActor_WaitForMove(object);
  FieldEvent_Wait(20);
  FieldActor_SetMoveTarget(object, 0x2000000, 0x500000, 0xfc0000);
  FieldActor_WaitForMove(object);
  FieldEvent_Wait(20);
  Actor_Release(object);
L_0200b8ac:
  FieldEvent_Wait(40);
  FieldActor_SetSpritePriority(23, 2);
  FieldActor_SetSpritePriority(24, 2);
  FieldActor_SetSpritePriority(25, 2);
  FieldActor_SetSpritePriority(26, 2);
  FieldActor_SetSpritePriority(27, 2);
  Event_PlaySound(141);
  MapGrid_SetShake(0x10000, 0x10000, 0x10000);
  FieldActor_SetSpriteColor(((s32)Actor_Get(23)), 7);
  beam = (s32)Actor_Get(23);
  A(beam)->scaleX = 0x1999;
  A(beam)->scaleY = 0xffff0000;
  A(beam)->motionFlags = zero;
  FieldActor_SetPositionAndResetMotion(beam, 0x2000000, 0x400000, 0xcc0000);
  FieldActor_SetSpriteColor(((s32)Actor_Get(24)), 7);
  beam2 = (s32)Actor_Get(24);
  A(beam2)->scaleX = 0x1999;
  A(beam2)->scaleY = 0xffff0000;
  A(beam2)->motionFlags = zero;
  FieldActor_SetPositionAndResetMotion(beam2, 0x2000000, 0x600000, 0xcc0000);
  Task_AddFrame(Orbs_SpawnRising, 0x480);
  Actor_FaceDirection(7, 0xe000, 0);
  Actor_FaceDirection(5, 0xe000, 0);
  Actor_FaceDirection(6, 0xe000, 0);
  Actor_FaceDirection(19, 0xd000, 0);
  Actor_FaceDirection(0, 0xa000, 0);
  Actor_FaceDirection(3, 0xa000, 0);
  Actor_FaceDirection(1, 0xa000, 0);
  Actor_FaceDirection(1, 0xa000, 80);
  Actor_SetScript(23, gData_020060b4);
  Actor_SetScript(24, gData_020060b4);
  Event_PlaySound(145);
  MapGrid_SetShake(0x60000, 0x60000, 0x10000);
  PaletteFade_ApplyTarget(0x4063ff, 0);
  PaletteFade_Interpolate(16);
  System_Wait(20);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(24);
  System_Wait(60);
  Event_PlaySound(141);
  Event_SetFlag(0x200);
  A(((s32)Actor_Get(23)))->y = 0xffc80000;
  A(((s32)Actor_Get(24)))->y = 0xffe80000;
  FieldActor_SetSpriteColor(((s32)Actor_Get(25)), 7);
  piece = (s32)Actor_Get(25);
  A(piece)->scaleY = 0xffff0000;
  A(piece)->scaleX = (A(((s32)Actor_Get(23)))->scaleX);
  A(piece)->motionFlags = zero;
  FieldActor_SetPositionAndResetMotion(piece, 0x2000000, 0x80000, 0xcc0000);
  FieldActor_SetSpriteColor(((s32)Actor_Get(26)), 7);
  piece2 = (s32)Actor_Get(26);
  A(piece2)->scaleY = 0xffff0000;
  A(piece2)->scaleX = (A(((s32)Actor_Get(23)))->scaleX);
  A(piece2)->motionFlags = zero;
  FieldActor_SetPositionAndResetMotion(piece2, 0x2000000, 0x280000, 0xcc0000);
  FieldActor_SetSpriteColor(((s32)Actor_Get(27)), 7);
  piece3 = (s32)Actor_Get(27);
  A(piece3)->scaleY = 0xffff0000;
  A(piece3)->scaleX = (A(((s32)Actor_Get(23)))->scaleX);
  A(piece3)->motionFlags = zero;
  FieldActor_SetPositionAndResetMotion(piece3, 0x2000000, 0x480000, 0xcc0000);
  MapGrid_CopyCellsTo(106, 36, 87, 7, 17, firstBeam);
  MapGrid_CopyCellsTo(42, 98, 23, 69, 17, firstBeam);
  MapGrid_CopyCellAttributes(0, 40, 18, 20, firstBeam, 10);
  Func_020035ec(7);
  Func_020035ec(5);
  Func_020035ec(6);
  glowTask = (s32)Orbs_SpawnRising;
  Func_020035ec(20);
  Func_020035ec(21);
  Func_020035ec(19);
  Func_0200361c(2);
  Func_0200361c(1);
  Func_0200361c(22);
  Func_0200361c(0);
  Func_0200361c(3);
  owner = OWNER;
  {
    s32 speed = 0xe000;
    Actor_SetPositionAndFacing(owner, 0x1e40000, 0x1380000, speed);
  }
  FieldActor_SetSpritePriority((OWNER), 1);
  System_Wait(1);
  MapGrid_SetShake(0x40000, 0x40000, 0x10000);
  PaletteFade_ApplyTarget(0x4063ff, 0);
  PaletteFade_Interpolate(120);
  Actor_SetScript(23, gData_020060fc);
  Actor_SetScript(24, gData_020060fc);
  Actor_SetScript(25, gData_020060fc);
  Actor_SetScript(26, gData_020060fc);
  Actor_SetScript(27, gData_020060fc);
  System_Wait(120);
  MapGrid_SetShake(0x30000, 0x30000, 0x10000);
  PaletteFade_ApplyTarget(0x203210, 0);
  PaletteFade_Interpolate(120);
  System_Wait(120);
  MapGrid_SetShake(0x20000, 0x20000, 0x10000);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(120);
  System_Wait(120);
  MapGrid_SetShake(0x10000, 0x10000, 0x10000);
  Event_PlaySound(78);
  A(((s32)Actor_Get(23)))->scaleY = 0x51e;
  Actor_SetScript(24, gData_02006120);
  Actor_SetScript(25, gData_02006120);
  Actor_SetScript(26, gData_02006120);
  Actor_RunScriptAndWait(27, gData_02006120);
  Event_PlaySound(0x186);
  FieldActor_SetSpriteColor(((s32)Actor_Get(23)), 15);
  FieldEvent_Wait(20);
  Actor_RunScriptAndWait(23, gData_02006150);
  FieldTask_Remove(glowTask);
  actor = (s32)Actor_Get(8);
  A(actor)->motionFlags = zero;
  Actor_SetBlendMode(actor, 1);
  FieldActor_SetSpriteColor(((s32)Actor_Get(8)), 7);
  FieldActor_SetSpritePriority(8, 2);
  *(s32 *)(s32)&Data_02006ccc = zero;
  *(s32 *)(s32)&Data_02006b68 = 240;
  Task_AddFrame(Func_02003370, 0x480);
L_0200bcea:
  System_Wait(1);
  if ((u32)(*(s32 *)(s32)&Data_02006ccc) <= (u32)100)
    goto L_0200bcea;
  Event_PlaySound(72);
  FieldEvent_Wait(120);
  Actor_ShowEmote(3, 0x109, 40);
  FieldEvent_SetMessageId((s32)MsgTheBeaconIs);
  Event_ShowMessage(3, 0);
  FieldActor_ShowEmote(2, 0x105, 40);
  Event_ShowMessage(2, 0);
  Event_ShowMessage(6, 0);
  FieldActor_ShowEmote(7, 0x109, 20);
  Event_ShowMessage(7, 0);
  FieldActor_ShowEmote(1, 0x103, 40);
  Event_ShowMessage(1, 0);
  Actor_FaceDirection(7, 0, 0);
  Actor_FaceDirection((OWNER), 0, 0);
  Actor_FaceDirection(5, 0, 0);
  Actor_FaceDirection(6, 0, 0);
  Actor_FaceDirection(19, 0, 0);
  Actor_FaceDirection(3, 0x8000, 0);
  Actor_FaceDirection(0, 0x8000, 0);
  Actor_FaceDirection(1, 0x8000, 0);
  Actor_FaceDirection(2, 0x8000, 20);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation((OWNER), 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(19, 3);
  Actor_SetAnimation(3, 3);
  Actor_SetAnimation(0, 3);
  Actor_SetAnimation(1, 3);
  Actor_SetAnimationAndWait(2, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(7, 0x4000, 0);
  Actor_FaceDirection((OWNER), 0x4000, 0);
  Actor_FaceDirection(5, 0, 0);
  Actor_FaceDirection(6, 0xc000, 0);
  Actor_FaceDirection(19, 0xb000, 0);
  Actor_FaceDirection(3, 0x4000, 0);
  Actor_FaceDirection(1, 0xe000, 0);
  Actor_FaceDirectionAndWait(0, 0);
  FieldEvent_ShowMessageThenWait(0x4005, 0, 10);
  MapGrid_SetShake(-1, -1, 0xe666);
  FieldMapGrid_WaitForShakeToSettle();
  FieldPsynergy_StartMindReadWave(23, 24);
  Message_ShowCentered(((s32)MsgTheBeaconIs + 6), 1);
  Func_02005794();
  FieldEvent_Wait(20);
  FieldPsynergy_StartMindReadWave(23, 24);
  Message_ShowCentered(((s32)MsgTheBeaconIs + 7), 1);
  Func_02005794();
  work = Ram_HeapSlots->eventWork;
  work->step.value = work->stepRead + 2;
  Func_02000610((OWNER));
  Func_02000610(7);
  Func_02000610(5);
  Func_02000610(6);
  Func_02000610(19);
  Func_02000610(1);
  Func_02000610(2);
  Func_02000610(3);
  Actor_FaceDirection(0, 0xc000, 40);
  Actor_FaceDirection(0, 0x4000, 40);
  Actor_FaceDirectionAndWait(0, 0x4000);
  FieldEvent_ShowMessageThenWait(0x6000, 0, 10);
  FieldPsynergy_StartMindReadWave(23, 24);
  message = ((s32)MsgTheBeaconIs + 9);
  Message_ShowCentered(message, 1);
  Func_02005794();
  Func_02000640(3);
  Actor_FaceDirectionAndWait(0x6003, 0x4000);
  A(((s32)Actor_Get(3)))->update = 0;
  // Read as a word, so the store to the actor before it may alias.
  later =
      (struct AerieEventWork *)((struct HeapWords *)Ram_HeapSlots)->eventWord;
  later->stepRead = later->stepRead + 1;
  FieldActor_ShowEmote(3, 0x100, 20);
  FieldEvent_ShowMessageThenWait(3, 0, 20);
  Event_PlaySound(201);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(20);
  System_Wait(20);
  FieldEvent_SetExitRequest(3);
  return;
}

#undef A

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
void Func_020004a4(void);

/// ★ Named from the game: "What could he be doing?" (MsgWhatCouldHe)
void MarsLighthouseAerie_WatchGoldenSunRise(void) {
  s32 owner, i, ownerInLoop, ownerAfterLoop, quarterTurn;
  s32 threeSixteenthsTurn, actor, t2, t3;
  s32 t4, t5, t6, t7, t8, t9, t10, t17, t21, t25;
  s32 t31, t49, t62, t76, t87, t101, t104, t107, t156;
  s32 t158, t160, t168, t184, t186, t189, t195, t205;
  s32 t215, t228, t238, t250, t263, t271, t274, t284, t292;
  s32 t296;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;
    owner = (s32)&party->currentOwner;
  }
  quarterTurn = 0x4000;
  t2 = *(s32 *)owner;
  t3 = Actor_Get(t2);
  ((struct Actor *)t3)->facing = quarterTurn;
  t4 = Actor_Get(7);
  ((struct Actor *)t4)->facing = quarterTurn;
  t5 = Actor_Get(5);
  ((struct Actor *)t5)->facing = quarterTurn;
  t6 = Actor_Get(6);
  ((struct Actor *)t6)->facing = quarterTurn;
  t7 = Actor_Get(19);
  threeSixteenthsTurn = 0x3000;
  ((struct Actor *)t7)->facing = threeSixteenthsTurn;
  t8 = Actor_Get(0);
  ((struct Actor *)t8)->facing = quarterTurn;
  t9 = Actor_Get(1);
  ((struct Actor *)t9)->facing = quarterTurn;
  t10 = Actor_Get(2);
  ((struct Actor *)t10)->facing = quarterTurn;
  System_Wait(1);
  Actor_SetAnimation(20, 8);
  Actor_SetAnimation(21, 8);
  Actor_SetAnimation(22, 7);
  Actor_SetPosition(19, 0x1f20000, 0x1400000);
  Actor_SetPosition(1, 0x21a0000, 0x1400000);
  t17 = *(s32 *)owner;
  Actor_SetPosition(t17, 0x1fa0000, 0x1180000);
  System_Wait(1);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  t21 = FieldCamera_GetActor();
  ((struct Actor *)t21)->motionFlags = 0;
  FieldCamera_MoveTo(0x2000000, 0x600000, 0x1200000, 0);
  Map_CenterOnFollowedPoint();
  System_Wait(1);
  t25 = (s32)Ram_HeapSlots->eventWork;
  FIELD_AT(t25, s32, 428) = 0x201;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(20);
  Actor_FaceDirection(7, 0, 0);
  Actor_FaceDirection(6, 0, 0);
  t31 = *(s32 *)owner;
  Actor_FaceDirection(t31, 0, 0);
  Actor_FaceDirection(5, 0, 0);
  Actor_FaceDirection(19, 0, 0);
  Actor_FaceDirection(0, 0x8000, 0);
  Actor_FaceDirection(3, 0x8000, 0);
  Actor_FaceDirection(1, 0x8000, 0);
  Actor_FaceDirection(2, 0x8000, 40);
  Actor_FaceDirectionAndWait(6, 0xe000);
  FieldEvent_SetMessageId((s32)MsgWhatCouldHe);
  Event_ShowMessage(6, 0);
  Actor_FaceDirection(3, 0x6000, 20);
  Actor_SetAnimationAndWait(3, 4);
  FieldActor_Jump(19, 4, 10);
  Event_ShowMessage(19, 0);
  Actor_FaceDirection(6, 0, 0);
  Actor_FaceDirection(5, 0x2000, 0);
  Actor_FaceDirection(7, quarterTurn, 0);
  t49 = *(s32 *)owner;
  Actor_FaceDirection(t49, quarterTurn, 0);
  Actor_FaceDirection(0, 0x6000, 0);
  Actor_FaceDirection(2, 0x6000, 0);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(0x2000000, -1, 0x1400000, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirectionAndWait(19, 0xd000);
  Actor_SetAnimation(19, 3);
  Event_ShowMessage(19, 0);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  t62 = *(s32 *)owner;
  Actor_SetAnimation(t62, 3);
  Actor_SetAnimation(0, 3);
  Actor_SetAnimation(3, 3);
  Actor_SetAnimation(2, 3);
  Actor_SetAnimationAndWait(1, 3);
  Actor_SetAttachedEffect(19, 0x102);
  FieldEvent_Wait(20);
  Event_ShowMessage(19, 0);
  Event_ShowMessage(7, 0);
  Actor_FaceDirectionAndWait(19, 0xb000);
  Actor_SetAnimationAndWait(19, 3);
  Actor_ShowEmote(1, 0x103, 20);
  Event_ShowMessage(1, 0);
  t76 = Actor_Get(28);
  FieldActor_SetSpriteColor(t76, 15);
  Actor_SetPosition(28, 0x2000000, 0x1680000);
  System_Wait(1);
  PaletteFade_ApplySource(0x10000, 0);
  FieldPsynergy_StartMindReadWave(23, 24);
  Event_ShowMessage(28, 0);
  Func_02005794();
  Actor_FaceDirection(2, quarterTurn, 0);
  FieldActor_Jump(2, 4, 10);
  Event_ShowMessage(2, 0);
  t87 = *(s32 *)owner;
  Actor_FaceDirection(t87, quarterTurn, 0);
  Actor_FaceDirection(7, quarterTurn, 0);
  Actor_FaceDirection(5, quarterTurn, 0);
  Actor_FaceDirection(6, quarterTurn, 0);
  Actor_FaceDirection(19, threeSixteenthsTurn, 0);
  Actor_FaceDirection(1, quarterTurn, 0);
  Actor_FaceDirection(3, quarterTurn, 0);
  Actor_FaceDirectionAndWait(0, quarterTurn);
  FieldActor_ShowEmote(0, 0x109, 20);
  Event_ShowMessage(0, 0);
  FieldPsynergy_StartMindReadWave(23, 24);
  Event_ShowMessage(28, 0);
  Func_02005794();
  t101 = *(s32 *)owner;
  Actor_FaceDirection(t101, 0, 0);
  Actor_FaceDirection(0, 0x8000, 20);
  t104 = *(s32 *)owner;
  Actor_SetAnimation(t104, 3);
  Actor_SetAnimationAndWait(0, 3);
  t107 = *(s32 *)owner;
  Actor_FaceDirection(t107, quarterTurn, 0);
  Actor_FaceDirectionAndWait(0, quarterTurn);
  FieldPsynergy_StartMindReadWave(23, 24);
  Event_ShowMessage(28, 0);
  Func_02005794();
  actor = Actor_Get(5);
  ((struct Actor *)actor)->facing = 0x6000;
  Actor_SetAnimation(5, 22);
  FieldEvent_Wait(10);
  FieldActor_StartRepeatedMotion(5, 2);
  Event_ShowMessage(5, 0);
  FieldPsynergy_StartMindReadWave(23, 24);
  Event_ShowMessage(28, 0);
  Func_02005794();
  ((struct Actor *)actor)->facing = quarterTurn;
  Actor_SetAnimation(5, 1);
  FieldActor_ShowEmote(5, 0x100, 20);
  Event_ShowMessage(5, 0);
  FieldPsynergy_StartMindReadWave(23, 24);
  Event_ShowMessage(28, 0);
  Func_02005794();
  Event_ShowMessage(1, 0);
  FieldPsynergy_StartMindReadWave(23, 24);
  Event_ShowMessage(28, 0);
  Func_02005794();
  FieldActor_ShowEmote(1, 0x100, 20);
  Event_ShowMessage(1, 0);
  FieldPsynergy_StartMindReadWave(23, 24);
  Event_ShowMessage(28, 0);
  Func_02005794();
  Actor_ShowEmote(19, 0x100, 20);
  Event_ShowMessage(19, 0);
  FieldPsynergy_StartMindReadWave(23, 24);
  Event_ShowMessage(28, 0);
  Func_02005794();
  FieldEvent_Wait(40);
  FieldCamera_MoveTo(0x2000000, -1, 0x1280000, 1);
  Actor_FaceDirection(19, 0x5000, 20);
  Actor_FaceDirection(19, threeSixteenthsTurn, 40);
  Actor_FaceDirectionAndWait(19, 0xd000);
  Event_ShowMessage(19, 0);
  Actor_FaceDirectionAndWait(5, 0x2000);
  Event_ShowMessage(0x4005, 0);
  Actor_FaceDirectionAndWait(0, 0x8000);
  Event_ShowMessage(0x8000, 0);
  Actor_FaceDirectionAndWait(7, 0);
  Event_ShowMessage(7, 0);
  Actor_FaceDirectionAndWait(2, 0x8000);
  Actor_ShowEmote(2, 0x109, 40);
  Event_OpenMessage(2, 0);
  t156 = *(s32 *)owner;
  Actor_FaceDirectionAndWait(t156, 0);
  t158 = *(s32 *)owner;
  FieldEvent_AskYesNoWithActorAnimation(t158, 0);
  t160 = *(s32 *)owner;
  Actor_WaitForAnimationChange(t160);
  PaletteFade_ApplyTarget(0x10004, 1);
  PaletteFade_Interpolate(40);
  Event_PlaySound(78);
  Event_PlaySound(141);
  MapGrid_SetShake(0x10000, 0x10000, 0x10000);
  FieldEvent_Wait(20);
  t168 = *(s32 *)owner;
  Func_02000610(t168);
  Func_02000610(5);
  Func_02000610(6);
  Func_02000610(7);
  Func_02000610(19);
  Func_02000610(0);
  Func_02000610(1);
  Func_02000610(2);
  Func_02000610(3);
  FieldEvent_Wait(20);
  FieldCamera_SetSpeed(0x19999, 0x3333);
  FieldCamera_MoveTo(0x2000000, -1, 0x1200000, 1);
  FieldCamera_WaitForArrival();
  FieldTask_Remove((s32)Func_020004a4);
  Event_PlaySound(140);
  t184 = Actor_Get(8);
  FieldActor_SetSpriteColor(t184, 12);
  t186 = Actor_Get(9);
  FieldActor_SetSpriteColor(t186, 12);
  FieldActor_SetSpritePriority(23, 2);
  t189 = Actor_Get(23);
  FieldActor_SetSpriteColor(t189, 7);
  actor = Actor_Get(23);
  ((struct Actor *)actor)->scaleY = 0xffff0000;
  ((struct Actor *)actor)->scaleX = 0x1999;
  ((struct Actor *)actor)->motionFlags = 0;
  ((struct Actor *)actor)->priorityFlags |= 2;
  FieldActor_SetPositionAndResetMotion(actor, 0x2000000, 0x400000, 0xcc0000);
  i = 0;
L_0200c66c:
  if (i != 20)
    goto L_0200c72a;
  MapGrid_SetShake(0x20000, 0x20000, 0x10000);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;
    ownerInLoop = (s32)&party->currentOwner;
  }
  t195 = *(s32 *)ownerInLoop;
  FieldActor_ShowEmote(t195, 0x100, 0);
  FieldActor_ShowEmote(5, 0x100, 0);
  FieldActor_ShowEmote(6, 0x100, 0);
  FieldActor_ShowEmote(7, 0x100, 0);
  FieldActor_ShowEmote(19, 0x100, 0);
  FieldActor_ShowEmote(0, 0x100, 0);
  FieldActor_ShowEmote(1, 0x100, 0);
  FieldActor_ShowEmote(2, 0x100, 0);
  Actor_ShowEmote(3, 0x100, 0);
  t205 = *(s32 *)ownerInLoop;
  Func_02000640(t205);
  Func_02000640(5);
  Func_02000640(6);
  Func_02000640(7);
  Func_02000640(19);
  Func_02000640(0);
  Func_02000640(1);
  Func_02000640(2);
  Func_02000640(3);
L_0200c72a:
  t215 = ((struct Actor *)actor)->scaleX;
  ((struct Actor *)actor)->scaleX = (t215 + 0x400);
  System_Wait(1);
  i = (i + 1);
  if ((u32)i <= (u32)39)
    goto L_0200c66c;
  Event_PlaySound(63);
  Actor_SetScript(23, (s32)gMarsLighthouseAerie_WatchGoldenSunRiseScript);
  FieldEvent_Wait(80);
  FieldActor_Jump(1, 4, 10);
  FieldActor_Jump(1, 6, 10);
  Actor_FaceDirectionAndWait(1, 0x8000);
  Event_ShowMessage(1, 0);
  Event_ShowMessage(7, 0);
  Actor_FaceDirectionAndWait(6, 0xa000);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(19, 0);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;
    ownerAfterLoop = (s32)&party->currentOwner;
  }
  t228 = *(s32 *)ownerAfterLoop;
  Actor_FaceDirection(t228, 0, 0);
  Actor_FaceDirection(5, 0, 0);
  Actor_FaceDirection(6, 0, 0);
  Actor_FaceDirection(7, 0, 0);
  Actor_FaceDirection(19, 0, 0);
  Actor_FaceDirection(0, 0x8000, 0);
  Actor_FaceDirection(1, 0x8000, 0);
  Actor_FaceDirection(2, 0x8000, 0);
  Actor_FaceDirection(3, 0x8000, 40);
  t238 = *(s32 *)ownerAfterLoop;
  Actor_FaceDirection(t238, 0xc000, 0);
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_FaceDirection(6, 0xc000, 0);
  Actor_FaceDirection(7, 0xc000, 0);
  Actor_FaceDirection(19, 0xc000, 0);
  Actor_FaceDirection(0, 0xc000, 0);
  Actor_FaceDirection(1, 0xc000, 0);
  Actor_FaceDirection(2, 0xc000, 0);
  Actor_FaceDirection(3, 0xc000, 20);
  Actor_FaceDirectionAndWait(3, 0x8000);
  Event_ShowMessage(3, 0);
  t250 = *(s32 *)ownerAfterLoop;
  Actor_FaceDirection(t250, 0, 0);
  Actor_FaceDirection(5, 0, 0);
  Actor_FaceDirection(6, 0, 0);
  Actor_FaceDirection(7, 0, 0);
  Actor_FaceDirection(19, 0, 0);
  Actor_FaceDirection(0, 0x8000, 0);
  Actor_FaceDirection(1, 0x8000, 0);
  Actor_FaceDirection(2, 0x8000, 0);
  Actor_FaceDirection(3, 0x8000, 20);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  t263 = *(s32 *)ownerAfterLoop;
  Actor_SetAnimation(t263, 3);
  Actor_SetAnimation(0, 3);
  Actor_SetAnimation(3, 3);
  Actor_SetAnimation(2, 3);
  Actor_SetAnimationAndWait(1, 3);
  FieldCamera_MoveTo(0x2000000, -1, 0x1380000, 1);
  Actor_SetPosition(8, 0, 0);
  t271 = Actor_Get(9);
  FieldActor_SetSpriteColor(t271, 7);
  FieldActor_SetSpritePriority(9, 1);
  t274 = Actor_Get(9);
  ((struct Actor *)t274)->priorityFlags &= 253;
  Event_PlaySound(140);
  MapGrid_SetShake(0x30000, 0x30000, 0x10000);
  PaletteFade_ApplyTarget(0x203210, 0);
  PaletteFade_Interpolate(40);
  Actor_FaceDirection(7, 0x4000, 0);
  Actor_FaceDirection(5, 0x2000, 0);
  Actor_FaceDirection(6, 0xe000, 0);
  Actor_FaceDirection(19, 0xb000, 0);
  t284 = *(s32 *)ownerAfterLoop;
  Actor_FaceDirection(t284, 0x4000, 0);
  Actor_FaceDirection(3, 0x4000, 0);
  Actor_FaceDirection(6, 0x8000, 0);
  Actor_FaceDirection(0, 0, 0);
  Actor_FaceDirection(1, 0xc000, 80);
  FieldActor_Jump(1, 4, 0);
  Actor_SetSpeed(1, 0x33333, 0x19999);
  t292 = Actor_Get(1);
  FIELD_AT(t292, u8, 90) &= 254;
  Actor_WalkToAndWait(1, 0x21e, 0x148);
  FieldEvent_Wait(1);
  t296 = Actor_Get(1);
  FIELD_AT(t296, u8, 90) |= 1;
  Event_ShowMessage(1, 0);
  Actor_FaceDirection(0, 0x4000, 0);
  Actor_FaceDirection(3, 0x2000, 0);
  Actor_FaceDirection(2, 0x4000, 40);
  Actor_FaceDirection(3, 0x6000, 0);
  Actor_FaceDirection(2, 0xa000, 0);
  Actor_FaceDirection(0, 0xe000, 20);
  Actor_ShowEmote(0, 0x103, 0);
  Actor_FaceDirectionAndWait(0, 0x4000);
  FieldActor_Jump(0, 4, 10);
  Event_ShowMessage(0x1000, 0);
  Actor_SetAnimation(1, 4);
  Event_ShowMessage(1, 0);
  Actor_FaceDirection(3, 0x4000, 0);
  Actor_ShowEmote(3, 0x100, 20);
  Event_ShowMessage(3, 0);
  Actor_SetAttachedEffect(7, 0x102);
  FieldEvent_Wait(20);
  Event_ShowMessage(7, 0);
  FieldActor_Jump(5, 4, 10);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(0x8002, 0);
  Actor_FaceDirectionAndWait(19, 0xd000);
  FieldActor_Jump(19, 2, 10);
  FieldActor_Jump(19, 4, 10);
  Event_ShowMessage(19, 0);
  Event_PlaySound(0x12a);
  Event_PlaySound(78);
  System_Wait(1);
  Event_PlaySound(140);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(20);
  System_Wait(240);
  *(u16 *)0x4000000 = 0;
  PaletteFade_ApplyTarget(0, 0);
  PaletteFade_Interpolate(8);
  System_Wait(8);
  FieldEvent_SetExitRequest(4);
  return;
}

/// ★ Named from the game: "Do not let your sadness overwhelm you, ! Go!"
/// (MsgDoNotLet)
void MarsLighthouseAerie_HoldBackFromParents(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceDirectionAndWait(19, FACING_SOUTH + FACING_STEP);
  FieldEvent_SetMessageId(MsgDoNotLet);
  Event_ShowMessage(0x8013, 0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 480, 348);
  Actor_FaceDirectionAndWait(19, FACING_NORTHWEST + FACING_STEP);
  Event_End();
}

/// ★ Named from the game: "Do not let your sadness overwhelm you, ! Go!"
/// (MsgDoNotLet)
void MarsLighthouseAerie_HoldBackFromParentsAgain(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceDirectionAndWait(19, FACING_SOUTHEAST + FACING_STEP);
  FieldEvent_SetMessageId(MsgDoNotLet);
  Event_ShowMessage(0x8013, 0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 544, 348);
  Actor_FaceDirectionAndWait(19, FACING_NORTHWEST + FACING_STEP);
  Event_End();
}

/// ★ Named from the game: "Will you cast in the Mars Star?" (MsgWillYouCast)
void MarsLighthouseAerie_OfferToCastMarsStar(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetPosition(18, PIXELS(512), PIXELS(232));
  System_Wait(1);
  FieldEvent_SetMessageId(MsgWillYouCast);
  Event_OpenMessage(18, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
      0) {
    Actor_SetPosition(18, 0, 0);
    System_Wait(1);
    MarsLighthouseAerie_LightBeacon();
  }
  Actor_SetPosition(18, 0, 0);
  System_Wait(1);
  Event_SetFlag(0x202);
  Event_End();
}

void Func_02004d14(void) { GameFlag_Clear(0x202); }

// From patch 1, as is Func_02004d50.
#if TLA_PATCH_1
void Func_02004d24(s32 id) {
  u8 *work = Ram_HeapSlots->windowTiles;

  if (((struct MarsLighthouseAerieParty *)&gMainRoutineWork)->held == id)
    work[0x138a] = 1;
}
#endif

// From patch 1; the Japanese overlay has no such function.
#if TLA_PATCH_1

/// A halfword held in its own local: its load, the pool's mask and its store
/// keep their halfword forms.
struct HeldHalfword {
  u16 value;
};

void Func_02004d50(s32 id) {
  u8 *work = Ram_HeapSlots->windowTiles;
  struct MarsLighthouseAerieParty *party =
      (struct MarsLighthouseAerieParty *)&gMainRoutineWork;
  s32 held = party->held;
  /* FAKEMATCH(temporary): the word held in a halfword struct; as a plain store
   * of -1, 42 of 32 instructions differ in tla-en. */
  struct HeldHalfword raw = *(struct HeldHalfword *)&party->held;
  struct HeldHalfword ones;

  if (held == id) {
    s32 zero = 0;

    work[0x138a] = zero;
    ones.value = 0xffff;
    party->held = raw.value | ones.value;
    Actor_Get(held)->update = (void *)zero;
    FieldActor_SetSpriteColor(Actor_Get(held), 0);
  }
}

#endif

/// ★ Named from the game: ", I understand your loss..." (MsgIUnderstandYour)
void MarsLighthouseAerie_TalkToConsoler(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
#if TLA_PATCH_1
  Func_02004d24(2);
#endif
  Actor_FaceActor(2, gMainRoutineWork.currentOwner, 0);
  FieldEvent_SetMessageId(MsgIUnderstandYour);
  Event_ShowMessage(2, 0);
#if TLA_PATCH_1
  Actor_FaceDirectionAndWait(2, FACING_WEST);
  Func_02004d50(2);
#else
  Actor_FaceDirectionAndWait(2, FACING_WEST);
#endif
  Event_End();
}

/// ★ Named from the game: "I...can't see you, but I sense that you're
/// nearby..." (MsgUhhhhnnnICan)
void MarsLighthouseAerie_TalkToBlindedParent(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
#if TLA_PATCH_1
  Func_02004d24(20);
#endif
  FieldEvent_SetMessageId(MsgUhhhhnnnICan);
  Event_ShowMessage(20, 0);
#if TLA_PATCH_1
  Func_02004d50(20);
#endif
  Event_End();
}

/// ★ Named from the game: "Tell Dora...to live her life to the fullest...for
/// me..." (MsgKoffKoffYou)
void MarsLighthouseAerie_TalkToCoughingParent(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
#if TLA_PATCH_1
  Func_02004d24(22);
#endif
  FieldEvent_SetMessageId(MsgKoffKoffYou);
  Event_ShowMessage(22, 0);
#if TLA_PATCH_1
  Func_02004d50(22);
#endif
  Event_End();
}

/// ★ Named from the game: "I am so happy...to have one last chance to see you
/// again..." (MsgUnnnnIAm)
void MarsLighthouseAerie_TalkToGladParent(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
#if TLA_PATCH_1
  Func_02004d24(21);
#endif
  FieldEvent_SetMessageId(MsgUnnnnIAm);
  Event_ShowMessage(21, 0);
#if TLA_PATCH_1
  Func_02004d50(21);
#endif
  Event_End();
}

// From patch 1; the Japanese overlay has no such function.
#if TLA_PATCH_1

void Func_02004e80(s32 from, s32 to) {
  struct Actor *found = Actor_Get(from);
  struct Actor *object;
  struct Actor *target;
  struct MainRoutineWork *party = &gMainRoutineWork;

  party->savedByte = 0;
  object = found;
  ((struct MarsLighthouseAerieParty *)party)->held = to;
  target = Actor_Get(to);
  target->update = object->update;
  object->update = NULL;
}
#endif

// These four, their rows, and Func_02004e80 are from patch 1.
#if TLA_PATCH_1

void Func_02004ec0(void) { Func_02004e80(28, 2); }

void Func_02004ecc(void) { Func_02004e80(17, 20); }

void Func_02004ed8(void) { Func_02004e80(30, 22); }

void Func_02004ee4(void) { Func_02004e80(29, 21); }
#endif

/// Records the four palette lists, writes each listed palette entry from its
/// red, green and blue, then copies two palettes from block32 and starts the
/// blend.
DEFINE_PALETTE_SET_COLORS_FADE(Palette_SetColorsFade, gData_02006ce0,
                               Data_02006ce4, Data_02006ce8, Data_02006cd4,
                               Func_020051e0)

DEFINE_PALETTE_CYCLE_START(Func_02004fac, gData_02006cf0, gData_02006cec,
                           List_CountEntries, gData_02006ce0, gData_02006cdc,
                           15, gData_02006cd8, 16, gData_02006cd0,
                           PaletteCycle_Fade)

void Func_02005010(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 256;
  FieldTask_Remove(PaletteCycle_Fade);
}

DEFINE_PALETTE_CYCLE_START(Func_02005030, gData_02006cf0, gData_02006cec,
                           List_CountEntries, gData_02006ce0, gData_02006cdc, 2,
                           gData_02006cd8, 2, gData_02006cd0, PaletteCycle_Fade)

DEFINE_LIST_COUNT_ENTRIES(List_CountEntries)

DEFINE_PALETTE_CYCLE_FADE(PaletteCycle_Fade, gData_02006cd8, gData_02006cdc,
                          Data_02006ce8, gData_02006cec, Data_02006cd4,
                          gData_02006cf0, gData_02006ce0, gData_02006cd0,
                          Data_02006ce4)

void Func_020051e0(void) {
  struct MarsLighthouseAerieEventWork *work = Ram_HeapSlots->eventWork;
  s32 saved;

  saved = work->screenEffect;
  PaletteFade_ApplyTarget(0x202108, 1);
  PaletteFade_ApplyTarget(0x10000, 2);
  ((struct MarsLighthouseAerieEventWork *)Ram_HeapSlots->eventWork)
      ->screenEffect = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02005030();
  Teleport_PlayWhirl();
  System_Wait(40);
  Func_02005010();
  PaletteFade_ApplyTarget(0x10000, 1);
  PaletteFade_Interpolate(16);
  System_Wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->unk2c2 = 0;
  }
  work->screenEffect = saved;
}

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

void Func_02005268(struct Actor *object) {
  struct Vector3 position;
  s32 x, z, sine;
  s32 i, j;
  struct SparkleWork *work = (struct SparkleWork *)object->watchedObject;
  u8 *count = (u8 *)object + 99;
  struct SparkleSprite *sprite;

  if (*count & 1)
    Actor_SetSpriteColorMode(
        object, (u8)((u32)((struct CounterBits *)count)->rest % 6));
  if (((u8 *)object)[98] == 0) {
    if ((*count & 15) == 0) {
      Event_PlaySound(0x256);
      work->unka0[work->unka6] = 0;
      work->unka6 ^= 1;
    }
  } else if (((u8 *)object)[98] == 1) {
    if (work->unka0[0] > 31 && work->unka0[1] > 31) {
      Actor_SetSpriteColorMode(object, 0);
      VramCache_FreeEntry(work->vramEntry);
      object->update = 0;
      return;
    }
  }
  (*count)++;
  for (i = 0; i < 2; i++) {
    sine = Math_Sin(work->unka0[i] << 10);
    if (work->unka0[i] >= 0) {
      if (work->unka0[i] <= 31) {
        struct Vector3 *p = &position;

        p->x = object->x;
        p->y = object->y + (work->unka0[i] << 16);
        p->z = object->z;
        Camera_WorldToScreen(p);
        x = p->x;
        j = 0;
        z = p->z;
        sprite = &work->sprites[i * 2];
        for (; j <= 1; j++) {
          sprite->unk0c = x;
          sprite->unk10 = z;
          sprite->unk14 = sine;
          sprite->unk18 = sine;
          if (j == 0) {
            sprite->unk1e = BattleEffect_GetObjectDepth(object) - 1;
          } else {
            sprite->unk1e = BattleEffect_GetObjectDepth(object) + 1;
            sprite->unk10 -= 0x10000;
            sprite->unk18 = -sprite->unk18;
          }
          EffectSprite_Draw(sprite);
          sprite++;
        }
        work->unka0[i]++;
      }
    }
  }
}
#undef OWNER
#undef A
#undef FIELD_AT

/* VIEW(s53e4): tag struct SparkleSprite as struct SparkleSprite__s53e4; s3370.c
 * has another definition; tried with the definition the rest of the object has,
 * it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_aerie.c:2972: structure has no member
 * named `flag'. */
struct SparkleSprite__s53e4 {
  u8 unk00[5];
  u8 attr5Low : 5;
  u8 flag : 1;
  u8 attr5High : 2;
  u8 unk06[2];
  u8 unk08;
  u8 unk09 : 2;
  u8 priority : 2;
  u8 palette : 4;
  u8 unk0a[0x1e];
};

/* VIEW(s53e4): tag struct SparkleWork as struct SparkleWork__s53e4; s3370.c has
 * another definition; tried with the definition the rest of the object has, it
 * does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_aerie.c:2962:
 * incompatible types in assignment. */
struct SparkleWork__s53e4 {
  struct SparkleSprite__s53e4 sprites[4];
  u16 unka0;
  u16 unka2;
  s16 vramEntry;
  u16 unka6;
  struct Actor *object;
};

/* VIEW(s3370): routine BattleEffect_GetObjectPriority as s32
 * BattleEffect_GetObjectPriority(struct Actor *object); module/scene/scene.h
 * has void BattleEffect_GetObjectPriority(void); not tried:
 * BattleEffect_GetObjectPriority's bound declaration was not found. */
extern s32 Func_020057b4__s3370(struct Actor *object) __asm__(
    "BattleEffect_GetObjectPriority");

void Func_020053e4(struct Actor *object, struct SparkleWork__s53e4 *work) {
  struct SparkleSprite__s53e4 *sprite;
  s32 tile;
  s32 row;
  s32 col;
  s32 offset;

  work->vramEntry = VramCache_FindFreeEntry();
  Vram_Load(work->vramEntry, 256, gData_020057e4);
  work->unka6 = 0;
  work->unka0 = 999;
  work->unka2 = 999;
  work->object = object;
  ((u8 *)object)[0x62] = 0;
  ((u8 *)object)[0x63] = 0;
  object->update = Func_02005268;
  tile = gResourceTableEntries[work->vramEntry].offset >> 5;
  object->watchedObject = (struct Actor *)work;
  for (row = 0; row <= 1; row++) {
    col = 1;
    offset = (row * 2) * sizeof(struct SparkleSprite__s53e4);
    sprite = (struct SparkleSprite__s53e4 *)(offset + (s32)work);
    for (; col >= 0; col--) {
      Func_020057a4(sprite, 16, 16, 0x80004000, tile);
      sprite->flag = 0;
      sprite->palette = 0;
      sprite->priority = Func_020057b4__s3370(object) & 3;
      sprite++;
    }
  }
}

void Func_020054e4(struct Actor *object) { object->unk5c[6] = 1; }

#include "text/msg_ids.h"

void Scene_LeaveByExit(void);
void MarsLighthouseAerie_FaceDoomDragon(void);
void Func_02004ed8(void);

// clang-format off
s32 gData_020057e4[] = {
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

s16 gData_02005aa4[] = {
    137, 138, 139, 140, 141, 142, 143, -1,
};

s16 gData_02005ab4[] = {
    23, 25, 22, 23, 25, 22, 23, 25,
    22, 23, 25, 22, 23, 25, 22, 23,
    25, 22, 23, 25, 22, -1,
};

s16 gData_02005ae0[] = {
    30, 27, 31, 22, 21, 31, 20, 18,
    31, 13, 11, 27, 11, 8, 24, 8,
    5, 21, 31, 31, 31, -1,
};

u32 gData_02005b0c[] = {
    0x89008e, 0x8b008a, 0x8d008c, 0x8e008d, 0x8a0089, 0x8c008b,
    0x8d008c, 0x89008e, 0x8b008a, 0x8c008b, 0x8e008d, 0x8a0089,
    0x8b008a, 0x8d008c, 0x89008e, 0x8a0089, 0x8c008b, 0x8e008d,
};

DEFINE_G_DATA_02005B54(gData_02005b54)

DEFINE_G_DATA_02002F1C(gData_02005ba8, 496, 328)

DEFINE_G_DATA_02002F1C(gData_02005bd0, 480, 328)

DEFINE_G_DATA_02002F1C(gData_02005bf8, 488, 320)

DEFINE_G_DATA_02002F1C(gData_02005c20, 528, 328)

DEFINE_G_DATA_02002F1C(gData_02005c48, 512, 328)

s32 gData_02005c70[] = {
    23, 9, -1024, 23, 10, -1024,
    0, 1, 13, 64, 0, 3,
    0, 1, 17,
};

u32 gData_02005cac[] = {
    SC_ADD(SW_SCALE_X, 512),
    SC_ADD(SW_SCALE_Y, 512),
    SC_ADD(SW_Y, 0x2000),
    SC_WAIT(1),
    SC_LOOP(128, SC_TOP),
    SC_END,
};

DEFINE_G_DATA_02002F44(gData_02005ce8, 500, 306)

DEFINE_G_DATA_02002F88(gData_02005d2c, 524, 306, 10)

DEFINE_G_DATA_02002F44(gData_02005d70, 488, 320)

DEFINE_G_DATA_02002F44(gData_02005db4, 472, 304)

DEFINE_G_DATA_02003534(gData_02005df8, 464, 280, 10)

DEFINE_G_DATA_02002F88(gData_02005e3c, 538, 320, 10)

DEFINE_G_DATA_02002F88(gData_02005e80, 552, 304, 10)

DEFINE_G_DATA_02005EC4(gData_02005ec4)

u32 gData_02005f08[] = {
    39, 2, 4, 0x1d80000, 0, 0x1400000,
    1, 4, 0x1d80000, 0, 0x1300000, 1,
    39, 1, 22, 39, 1, 17,
};

u32 gData_02005f50[] = {
    39, 2, 4, 0x1dc0000, 0, 0x1480000,
    1, 4, 0x1d80000, 0, 0x1400000, 1,
    39, 1, 22, 39, 1, 17,
};

DEFINE_G_DATA_02002F88(gData_02005f98, 486, 286, 10)

DEFINE_G_DATA_02002F1C(gData_02005fdc, 504, 300)

DEFINE_G_DATA_02002F1C(gData_02006004, 528, 292)

DEFINE_G_DATA_02005EC4(gData_0200602c)

DEFINE_G_DATA_02002F88(gData_02006070, 548, 292, 10)

s32 gData_020060b4[] = {
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_WAIT(1),
    SC_LOOP(32, SC_TOP),
    SC_END,
    SC_ADD(SW_SCALE_X, -896),
    SC_WAIT(1),
    SC_LOOP(140, SC_TOP),
    SC_END,
};

DEFINE_G_DATA_02002F94(gData_020060fc, 896, 160)

u32 gData_02006120[] = {
    23, 6, 0x40000, 0, 1, 13,
    30, 0, 3, 0, 1, 17,
};

u32 gData_02006150[] = {
    23, 6, 0x20000, 0, 1, 13,
    160, 0, 3, 0, 1, 17,
};

s32 gData_02006180[] = {
    22, 9, 0xcccc, 22, 10, 0xcccc,
    39, 1, (s32)0x80010000, 23, 6, -0x10000,
    0, 1, 13, 16, (s32)0xc0010000, 3,
    0, 1, 38,
};

s32 gMarsLighthouseAerie_WatchGoldenSunRiseScript[] = {
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_WAIT(1),
    SC_LOOP(8, SC_TOP),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -4096),
    SC_WAIT(1),
    SC_LOOP(8, SC_TO(1)),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

DEFINE_G_SCENE_EXITS_2(297, 287, 1, 297, 3, 309, 2, 272, 20)

s16 gMarsLighthouseAerie_Placements[] = {
    489, -1, 1, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 2, 489, -1, 1, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 2,
    302, -1, 7, 0, 0, 584, 0, 16,
    0, 440, 0x4000, 2, 302, -1, 7, 0,
    0, 616, 0, 16, 0, 440, 0x4000, 258,
    302, -1, 7, 0, 0, 648, 0, 16,
    0, 440, 0x4000, 258, 302, -1, 7, 0,
    0, 648, 0, 16, 0, 408, 0x4000, 258,
    302, -1, 7, 0, 0, 648, 0, 16,
    0, 376, 0x4000, 258, 302, -1, 7, 0,
    0, 648, 0, 16, 0, 344, 0x4000, 258,
    302, -1, 7, 0, 0, 648, 0, 16,
    0, 312, 0x4000, 258, 182, -1, 7, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 2,
    311, -1, 7, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 2, 56, -1, 1, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 2,
    5, -1, 1, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 2, 6, -1, 1, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 2,
    7, -1, 1, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 2, 0, -1, 1, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 2,
    1, -1, 1, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 2, 3, -1, 1, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 2,
    2, -1, 1, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 2, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

struct ActorPlacement gData_0200644c[] = {
    {489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 1},
    {489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 1},
    {489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 1},
    {489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 1},
    {489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 1},
    {489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 1},
    {182, CONDITION_ALWAYS, 7, PIXELS(512), 0, PIXELS(236), FACING_SOUTH, 2, 0},
    {311, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {56, CONDITION_ALWAYS, 1, PIXELS(544), 0, PIXELS(354), FACING_NORTHWEST + FACING_STEP, 2, 0},
    {48, CONDITION_ALWAYS, 1, PIXELS(496), 0, PIXELS(328), FACING_NORTH, 2, 0},
    {49, CONDITION_ALWAYS, 1, PIXELS(480), 0, PIXELS(328), FACING_NORTH, 2, 0},
    {50, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(320), FACING_NORTH, 2, 0},
    {51, CONDITION_ALWAYS, 1, PIXELS(520), 0, PIXELS(320), FACING_NORTH, 2, 0},
    {52, CONDITION_ALWAYS, 1, PIXELS(536), 0, PIXELS(320), FACING_NORTH, 2, 0},
    {54, CONDITION_ALWAYS, 1, PIXELS(544), 0, PIXELS(328), FACING_NORTH, 2, 0},
    {53, CONDITION_ALWAYS, 1, PIXELS(528), 0, PIXELS(328), FACING_NORTH, 2, 0},
    {183, CONDITION_ALWAYS, 7, PIXELS(478), 0, PIXELS(266), FACING_SOUTH, 2, 0},
    {183, CONDITION_ALWAYS, 7, PIXELS(506), 0, PIXELS(280), FACING_SOUTH, 2, 0},
    {183, CONDITION_ALWAYS, 7, PIXELS(536), 0, PIXELS(276), FACING_SOUTH, 2, 0},
    {61, CONDITION_ALWAYS, 7, PIXELS(480), 0, PIXELS(282), FACING_SOUTH, 2, 0},
    {60, CONDITION_ALWAYS, 7, PIXELS(506), 0, PIXELS(292), FACING_SOUTH, 2, 0},
    {62, CONDITION_ALWAYS, 7, PIXELS(540), 0, PIXELS(284), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

s16 gData_020066bc[] = {
    303, -1, 7, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 2, 303, -1, 7, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 258,
    302, -1, 7, 0, 0, 584, 0, 16,
    0, 440, 0x4000, 2, 302, -1, 7, 0,
    0, 616, 0, 16, 0, 440, 0x4000, 258,
    302, -1, 7, 0, 0, 648, 0, 16,
    0, 440, 0x4000, 258, 302, -1, 7, 0,
    0, 648, 0, 16, 0, 408, 0x4000, 258,
    302, -1, 7, 0, 0, 648, 0, 16,
    0, 376, 0x4000, 258, 302, -1, 7, 0,
    0, 648, 0, 16, 0, 344, 0x4000, 258,
    302, -1, 7, 0, 0, 648, 0, 16,
    0, 312, 0x4000, 258, 489, -1, 7, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 2,
    489, -1, 7, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 2, 56, -1, 1, 0,
    0, 504, 0, 0, 0, 328, 0xb000, 2,
    5, -1, 1, 0, 0, 458, 0, 0,
    0, 288, 0, 1, 6, -1, 1, 0,
    0, 476, 0, 0, 0, 312, 0xe000, 1,
    7, -1, 1, 0, 0, 482, 0, 0,
    0, 276, 0x4000, 1, 0, -1, 1, 0,
    0, 529, 0, 0, 0, 296, 0, 1,
    1, -1, 1, 0, 0, 538, 0, 0,
    0, 312, 0xe000, 1, 3, -1, 1, 0,
    0, 552, 0, 0, 0, 274, 0x4000, 1,
    2, -1, 1, 0, 0, 566, 0, 0,
    0, 286, 0x8000, 1, 61, -1, 7, 0,
    0, 480, 0, 0, 0, 304, 0x4000, 2,
    60, -1, 7, 0, 0, 502, 0, 0,
    0, 312, 0x4000, 2, 62, -1, 7, 0,
    0, 548, 0, 0, 0, 304, 0x4000, 2,
    294, -1, 7, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 2, 294, -1, 7, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 258,
    294, -1, 7, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 258, 294, -1, 7, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 258,
    294, -1, 7, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 258, 59, -1, 7, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 2,
    489, -1, 7, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 2, 489, -1, 7, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 2,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

struct Trigger gSceneEvents[] = {
    {0x9c05, 1, CONDITION_ALWAYS, EVENT(Scene_LeaveByExit)},
    {TRIGGER_STEP, 10, 0xa3d, EVENT(MarsLighthouseAerie_FaceDoomDragon)},
    {TRIGGER_TALK, 0, CONDITION_ALWAYS, MSGNO(MsgOurQuestIs)},
    {TRIGGER_TALK, 1, CONDITION_ALWAYS, MSGNO(MsgIWishICould)},
    {TRIGGER_TALK, 3, CONDITION_ALWAYS, MSGNO(MsgIfAlexSet)},
    {TRIGGER_TALK, 5, CONDITION_ALWAYS, MSGNO(MsgIThoughtID)},
    {TRIGGER_TALK, 6, CONDITION_ALWAYS, MSGNO(MsgIDonTKnowMy)},
    {TRIGGER_TALK, 7, CONDITION_ALWAYS, MSGNO(MsgIMSorryThings)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgBothKyleAnd)},
    {0x4400, 28, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_TalkToConsoler)},
    {0x4400, 17, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_TalkToBlindedParent)},
    {0x6400, 17, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_TalkToBlindedParent)},
    {0xe400, 30, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_TalkToCoughingParent)},
    {0x400, 30, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_TalkToCoughingParent)},
    {0x2400, 30, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_TalkToCoughingParent)},
    {TRIGGER_TALK, 29, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_TalkToGladParent)},
#if TLA_PATCH_1 /* From patch 1. */
    {PSYNERGY_ON_ACTOR(ABILITY_HALT), 28, CONDITION_ALWAYS, EVENT(Func_02004ec0)},
    {PSYNERGY_ON_ACTOR(ABILITY_HALT), 17, CONDITION_ALWAYS, EVENT(Func_02004ecc)},
    {PSYNERGY_ON_ACTOR(ABILITY_HALT), 30, CONDITION_ALWAYS, EVENT(Func_02004ed8)},
    {PSYNERGY_ON_ACTOR(ABILITY_HALT), 29, CONDITION_ALWAYS, EVENT(Func_02004ee4)},
#endif
    {TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_HoldBackFromParents)},
    {TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(MarsLighthouseAerie_HoldBackFromParentsAgain)},
    {TRIGGER_STEP, 22, 0x202, EVENT(MarsLighthouseAerie_OfferToCastMarsStar)},
    {TRIGGER_STEP, 23, 0x1202, EVENT(Func_02004d14)},
    {TRIGGER_TABLE_END},
};

u32 gData_02006ad0[] = {
    SC_SET(SW_SCALE_Y, 0x16666),
    SC_SET(SW_SCALE_X, 0x16666),
    SC_ARG(4),
    SC_WAIT(30),
    SC_FREE,
};

DEFINE_G_DATA_02003214(gData_02006afc)
// clang-format on

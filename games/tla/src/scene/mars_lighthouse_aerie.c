#include "mars_lighthouse_aerie/mars_lighthouse_aerie.h"

void Func_02000038(struct Actor *object) {
  u8 *timer = &object->unk5c[6];

  if (*timer != 0) {
    (*timer)--;
  } else {
    *timer = (((u32)Random16Far() * 80) >> 16) + 60;
    object->angle = Random16Far();
  }
}

void Func_02000068(struct Actor *object) {
  if ((gFrameCount >> 1) & 1)
    Actor_SetSpriteColor(object, 15);
  else
    Actor_SetSpriteColor(object, 0);
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
    u32 r = Random16Far();

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
  u16 *pangle = (u16 *)&object->action;
  s32 angle = *pangle;
  s32 cosine;
  s32 sine;
  s32 along;
  s32 across;
  u8 *counter;

  cosine = Math_Cosine(angle);
  counter = &object->unk5c[6];
  {
    s32 radius = object->speedLimit + *counter;
    /* FAKEMATCH(pin): the anchor copied into r1 for its x; as anchor->x read
     * straight, 2 of 48 instructions differ in tla-en. */
    register struct Actor *a asm("r1");

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

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct Trigger *Func_02000164(void) {
  if (gMainRoutineWork.entrance == 2)
    return (const struct Trigger *)gData_0200644c;
  if ((u16)(gMainRoutineWork.entrance - 3) <= 2)
    return (const struct Trigger *)gData_020066bc;
  return (const struct Trigger *)gData_0200626c;
}

void Func_020001a0(void) {
  Func_02004fac();
  Func_020057cc();
  {
    u8 *party = (u8 *)&gMainRoutineWork;

    party[0x2c2] = 2;
  }
  FieldEvent_SetExitRequest(1);
  FieldEvent_Wait(40);
  Func_02005010();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern u32 Data_0200eb60;
extern const u8 gData_02005aa4[];
extern const u8 gData_02005ab4[];
extern const u8 gData_02005ae0[];
extern const u8 gData_02005b0c[];

void Animation_ApplyChildValues(struct Actor *object, s32 value);
struct FieldActor;
void Func_02004ef0(const u8 *a, const u8 *b, const u8 *c, const u8 *d);
void Func_020004a4(void);

static __inline__ void PlaceObject(s32 id, s32 a, s32 b) {
  /* FAKEMATCH(wrapper): forwarded inline; as a direct call, 10 of 124
   * instructions differ in tla-en. */
  Actor_SetHorizontalPositionWithTerrain(id, a, b);
}

void Func_020001d8(void) {
  struct Actor *object;
  s32 i;

  Func_02004ef0(gData_02005aa4, gData_02005ab4, gData_02005ae0, gData_02005b0c);
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 0x40;
  for (i = 10; i <= 16; i++) {
    object = Actor_Get(i);
    object->priorityFlags |= 0x80;
    Animation_ApplyChildValues(object, 5);
  }
  if (!Event_GetFlag(0xa3c))
    return;
  PlaceObject(8, 0x2000000, 0x1080000);
  PlaceObject(9, 0x2000000, 0x10f0000);
  System_Wait(1);
  FieldActor_SetSpritePriority(8, 1);
  object = Actor_Get(8);
  object->motionFlags = 4;
  object->y = 0x700000;
  Animation_ApplyChildValues(object, 5);
  Actor_SetSingleChildFlags(object, 0);
  object->priorityFlags |= 0x20;
  FieldActor_SetSpritePriority(9, 2);
  object = Actor_Get(9);
  object->motionFlags = 4;
  object->y = 0x700000;
  object->scaleX = 0x14ccc;
  object->scaleY = 0x14ccc;
  Animation_ApplyChildValues(object, 5);
  Actor_SetSingleChildFlags(object, 0);
  object->priorityFlags |= 0x20;
  object->priorityFlags |= 2;
  Animation_SetStateFlags(object, 1);
  Data_0200eb60 = 0;
  FieldTask_AddFrame(Func_020004a4, 0x480);
}

#include "io_reg.h"

struct MarsLighthouseAerieEventWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

void Func_02001f10(void);
void Func_020029ac(void);
void Func_02003658(void);
void Func_02003ff4(void);
void Func_0200558c(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

/// Sets the scene up for the entrance the party arrived by (5 to 2).
s32 Func_02000314(void) {
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
    Func_02003ff4();
  } else if (gMainRoutineWork.entrance == 4) {
    Func_0200560c();
    FieldEvent_ResolvePendingActions(0);
    Actor_SetAnimation(20, 8);
    Actor_SetAnimation(21, 8);
    Actor_SetAnimation(22, 7);
    Actor_SetPosition(19, PIXELS(498), PIXELS(320));
    Actor_SetPosition(1, PIXELS(538), PIXELS(320));
    Actor_SetPosition(gMainRoutineWork.currentOwner, PIXELS(506), PIXELS(280));
    System_Wait(1);
    FieldCamera_MoveTo(-1, -1, -1, 0);
    FieldCamera_GetActor()->motionFlags = 0;
    FieldCamera_MoveTo(PIXELS(512), PIXELS(96), PIXELS(312), 1);
    Func_0200558c();
    System_Wait(1);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    FieldEvent_Wait(40);
    Func_02003658();
    Func_02005614();
  } else if (gMainRoutineWork.entrance == 3) {
    Actor_SetAnimation(20, 8);
    Actor_SetAnimation(21, 8);
    Actor_SetAnimation(22, 7);
    if (Event_GetFlag(0xa3d) == 0)
      Func_020029ac();
  } else if (gMainRoutineWork.entrance == 2) {
    Func_02001f10();
  }
  return 0;
}

extern char SceneId_Ov6a8Room2;
extern char SceneId_Ov6ACRoom3;

void Func_020057d4(s32 a);
void Func_020055dc(s32 scene, s32 a1);

s32 Func_02000450(void) {
  if (gMainRoutineWork.entrance == 1 && Event_GetFlag(0xa3d) == 0)
    Func_020057d4(1);
  Func_020055dc(gMainRoutineWork.map +
                    ((s32)&SceneId_Ov6ACRoom3 - (s32)&SceneId_Ov6a8Room2),
                0x5000000);
  return 0;
}

struct MarsLighthouseAerieSprite {
  u8 unk00[9];
  u8 unk09_0 : 2;
  u8 layer : 2;
  u8 unk09_4 : 4;
  u8 unk0a[0x10];
  u8 unk1a;
};

extern const u8 gData_02006ad0[];

void Func_0200556c(struct Actor *object, const u8 *script);
void Func_020005a0(struct Actor *object);

void Func_020004a4(void) {
  struct Actor *anchor = Actor_Get(8);
  s32 offset = (Random16Far() * 48 >> 16) << 16;
  struct Actor *object;
  struct MarsLighthouseAerieSprite *sprite;
  u32 phase;

  if (anchor != NULL && (phase = Data_0200eb60 & 15) == 0) {
    /* FAKEMATCH(wrapper): the spawn forwarded through Value4; as a direct call,
     * 35 of 103 instructions differ in tla-en. */
    object = FieldActor_Create(0x2a0, anchor->x + 0x80000, anchor->y + offset + 0x80000, anchor->z);
    offset = (offset / 0x60000) << 16;
    if (object != NULL) {
      u8 zero;

      sprite = object->animation;
      Func_0200556c(object, gData_02006ad0);
      Actor_SetSpriteColor(object, 10);
      object->motionFlags = phase;
      {
        /* FAKEMATCH(temporary): the masked value held in a local; as
         * object->action = Random16Far() & 0x0ffff000, 45 of 103 instructions
         * differ in tla-en. */
        s32 action = Random16Far() & 0x0ffff000;

        object->action = action;
      }
      object->unk66 = phase;
      object->update = Func_020005a0;
      zero = 0;
      object->watchedObject = anchor;
      object->speedLimit = Math_Sin((offset & 0xfffff) >> 4) * 24 >> 16;
      ((u8 *)sprite)[26] = zero;
      sprite->layer =
          ((struct MarsLighthouseAerieSprite *)anchor->animation)->layer;
    }
  }
  Data_0200eb60++;
}

#include "field_sprite.h"

/// Swings the object along an arc beside its watched object, drawing it behind
/// (priority 2) while its z is short of 0x108 and in front (priority 1) after.
void Func_020005a0(struct Actor *object) {
  u16 *pangle = (u16 *)&object->action;
  u16 angle = *pangle;
  struct Actor *parent = object->watchedObject;

  object->x = parent->x + Math_Cosine(angle) * (object->speedLimit + 28);
  object->z = (Math_Sin(angle) << 4) + 0x1080000;
  object->targetX = object->x;
  object->targetZ = object->z;
  if (object->z < 0x1080000)
    ((struct FieldSprite *)object->animation)->priority = 2;
  else
    ((struct FieldSprite *)object->animation)->priority = 1;
  *pangle -= 0x200;
}

void Func_02000610(s32 id) {
  struct Actor *object = Actor_Get(id);

  FieldActor_Stop(id);
  object->unk5c[6] = (((u32)Random16Far() * 40) >> 16) + 20;
  object->update = (void (*)(void))Func_02000038;
}

void Func_02000640(s32 id) {
  Actor_Get(id)->update = NULL;
  Actor_FaceDirection(id, FACING_NORTH, 0);
}

#include "ram_buffer.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
struct SparkleWork;
void Func_020053e4(struct Actor *object, struct SparkleWork *work);
void Func_020054e4(struct Actor *object);
s32 Func_0200563c();
s32 Func_0200568c();
void Func_02005734();
s32 Func_0200574c();
extern u8 MsgBetrayersYouHave[];
extern u8 gData_02005ba8[];
extern u8 gData_02005bd0[];
extern u8 gData_02005bf8[];
extern u8 gData_02005c20[];
extern u8 gData_02005c48[];
extern u8 gData_02005c70[];
extern u8 gData_02005cac[];
extern u8 gData_02005ce8[];
extern u8 gData_02005d2c[];
extern u8 gData_02005d70[];
extern u8 gData_02005db4[];
extern u8 gData_02005df8[];
extern u8 gData_02005e3c[];
extern u8 gData_02005e80[];
extern u8 gData_02005ec4[];
extern u8 gData_02006c20[];
extern char SceneId_Ov6aeRoom1;

void Func_0200065c(void) {
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
  Func_0200560c();
  FieldEvent_ResolvePendingActions(0);
  t4 = Actor_Get(18);
  Actor_SetSpriteColor(t4, 15);
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
  t18 = FieldCamera_GetActor();
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
  Func_0200568c(0, t27);
  System_Wait(1);
  Actor_SetSpeed(0, 0x19999, 0xcccc);
  Actor_WalkToAndWait(0, 0x208, 0x140);
  Actor_FaceDirection(0, 0xc000, 0);
  Func_0200568c(1, 0);
  System_Wait(1);
  Actor_SetSpeed(1, 0x19999, 0xcccc);
  Actor_WalkToAndWait(1, 0x218, 0x140);
  Actor_FaceDirection(1, 0xa000, 20);
  Actor_FaceDirection(1, 0xe000, 20);
  Actor_FaceDirection(1, 0xc000, 0);
  Event_ShowMessage(1, 0);
  Func_0200568c(3, 1);
  System_Wait(1);
  Actor_SetSpeed(3, 0x19999, 0xcccc);
  Actor_WalkToAndWait(3, 0x220, 0x148);
  Actor_FaceDirectionAndWait(3, 0xa000);
  Actor_SetAnimation(3, 4);
  Event_ShowMessage(3, 0);
  t48 = gMainRoutineWork.currentOwner;
  FieldActor_Stop(t48);
  t50 = Random16Far();
  FIELD_AT(actor, u8, 98) = (((u32)(((t50 << 2) + t50) << 2) >> 16) + 20);
  FIELD_AT(actor, s32, 108) = (s32)Func_02000038;
  Func_02000610(1);
  Func_02000610(3);
  t53 = gMainRoutineWork.currentOwner;
  Func_0200568c(5, t53);
  t55 = gMainRoutineWork.currentOwner;
  Func_0200568c(6, t55);
  t57 = gMainRoutineWork.currentOwner;
  Func_0200568c(7, t57);
  t59 = gMainRoutineWork.currentOwner;
  Func_0200568c(2, t59);
  t61 = gMainRoutineWork.currentOwner;
  Func_0200568c(19, t61);
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
  Func_0200563c(16);
  Func_0200563c(15);
  Func_0200563c(14);
  Func_0200563c(13);
  Func_0200563c(12);
  Func_0200563c(11);
  Func_0200563c(10);
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
  { u8 *f = (u8 *)t123 + 90; *f &= 254; }
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
  Actor_SetSpriteColor(t191, 0);
  actor = Actor_Get(18);
  FIELD_AT(actor, u8, 85) = 0;
  t194 = Actor_Get(18);
  FieldActor_SetPositionAndResetMotion(t194, 0x2000000, 0x780000, 0xd00000);
  FieldEvent_Wait(20);
  FieldCamera_SetSpeed(0x4cccc, 0x9999);
  FieldCamera_MoveTo(0x2000000, -1, 0xd80000, 1);
  Func_02005734();
  FieldEvent_Wait(40);
  FieldEvent_ShowMessageThenWait(1, 0, 20);
  FieldCamera_MoveTo(0x2000000, -1, 0x1300000, 1);
  Func_02005734();
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
  { u8 *f = (u8 *)t497 + 90; *f &= 254; }
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
  Actor_SetSpriteColor(t528, 7);
  FieldEvent_Wait(20);
  t531 = Actor_Get(18);
  Actor_SetSpriteColor(t531, 0);
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
  MapGrid_WaitForShakeToSettle();
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
  Event_SetAfterBattleDestination((s32)&SceneId_Ov6aeRoom1, 2);
  Call2(Func_0200574c, 102, 1);
  return;
}

void Func_02001eb0(struct Actor *object) {
  if ((gData_02006b64 >> gData_02006b6c) & 1)
    Actor_SetSpriteColor(object, 15);
  else
    Actor_SetSpriteColor(object, 0);
  gData_02006b64++;
}

void Func_02001ee4(struct Actor *object) {
  if ((gData_02006b64 >> gData_02006b6c) & 1)
    Actor_SetSpriteColor(object, 0);
  else
    Actor_SetSpriteColor(object, 15);
}

extern u8 MsgWhoAreThey[];
extern u8 gData_02005f08[];
extern u8 gData_02005f50[];
extern u8 gData_02005f98[];
extern u8 gData_02005fdc[];
extern u8 gData_02006004[];
extern u8 gData_0200602c[];
extern u8 gData_02006070[];
extern u8 gData_02006b70[];

static __inline__ void W_Func_02005714(s32 a0, s32 a1, s32 a2) {
  Func_080d47b4(a0, a1, a2);
}

void Func_02001f10(void) {
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
  Func_0200560c();
  FieldEvent_ResolvePendingActions(0);
  /* FAKEMATCH(wrapper): Actor_SetModeById called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetAnimation(17, 2);
  FIELD_AT(actor, u8, 85) = 0;
  FIELD_AT(actor, s32, 8) -= 0x40000;
  FIELD_AT(actor, s32, 16) += 0x100000;
  /* FAKEMATCH(wrapper): Actor_SetModeById called through call.h's Call2; as a
   * direct call, 4 of 1038 instructions differ in tla-en. */
  Actor_SetAnimation(28, 2);
  /* FAKEMATCH(wrapper): Actor_SetModeById called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetAnimation(29, 3);
  /* FAKEMATCH(wrapper): Camera_MoveWithinMapBounds called through call.h's
   * Call4; as a direct call, 34 of 1038 instructions differ in tla-en. */
  FieldCamera_MoveTo(-1, -1, -1, 0);
  System_Wait(1);
  /* FAKEMATCH(wrapper): Actor_SetModeById called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetAnimation(30, 7);
  /* FAKEMATCH(wrapper): Actor_SetModeById called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetAnimation(31, 7);
  /* FAKEMATCH(wrapper): Actor_SetModeById called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetAnimation(32, 6);
  owner = (s32)&gMainRoutineWork;
  owner = (owner + 0x214);
  t16 = *(s32 *)owner;
  lastArg = 0xc000;
  Actor_SetPositionAndFacing(t16, 0x1f80000, 0x1400000, lastArg);
  System_Wait(1);
  /* FAKEMATCH(wrapper): BattleEffect_ApplyColorToSourceBuffer called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  PaletteFade_ApplySource(0, 0);
  PaletteFade_ApplyTarget(0, 0);
  PaletteFade_Interpolate(1);
  Actor_SetSpriteColor((Actor_Get(30)), 15);
  Actor_SetSpriteColor((Actor_Get(31)), 15);
  Actor_SetSpriteColor((Actor_Get(32)), 15);
  System_Wait(1);
  /* FAKEMATCH(wrapper): Actor_SetSpritePriority called through call.h's Call2;
   * as a direct call, 2 of 1038 instructions differ in tla-en. */
  FieldActor_SetSpritePriority(21, 1);
  /* FAKEMATCH(wrapper): Actor_SetSpritePriority called through call.h's Call2;
   * as a direct call, 2 of 1038 instructions differ in tla-en. */
  FieldActor_SetSpritePriority(20, 1);
  /* FAKEMATCH(wrapper): Actor_SetSpritePriority called through call.h's Call2;
   * as a direct call, 2 of 1038 instructions differ in tla-en. */
  FieldActor_SetSpritePriority(19, 1);
  /* FAKEMATCH(wrapper): Actor_SetSpritePriority called through call.h's Call2;
   * as a direct call, 2 of 1038 instructions differ in tla-en. */
  FieldActor_SetSpritePriority(26, 1);
  /* FAKEMATCH(wrapper): Actor_SetSpritePriority called through call.h's Call2;
   * as a direct call, 2 of 1038 instructions differ in tla-en. */
  FieldActor_SetSpritePriority(25, 1);
  t34 = (s32)Ram_HeapSlots->eventWork;
  FIELD_AT(t34, s32, 428) = 0x204;
  FIELD_AT(t34, s32, 436) = 1;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(0x186);
  /* FAKEMATCH(wrapper): BattleEffect_ApplyColorToTargetBuffer called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  PaletteFade_ApplyTarget(0x10003, 1);
  /* FAKEMATCH(wrapper): BattleEffect_ApplyColorToTargetBuffer called through call.h's Call2; as a
   * direct call, 12 of 1038 instructions differ in tla-en. */
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
  /* FAKEMATCH(wrapper): Sound_Play called through call.h's Call1; as a
   * direct call, 31 of 1038 instructions differ in tla-en. */
  Event_PlaySound(0x253);
  System_Wait(10);
  /* FAKEMATCH(wrapper): Sound_Play called through call.h's Call1; as a
   * direct call, 27 of 1038 instructions differ in tla-en. */
  Event_PlaySound(0x253);
  System_Wait(10);
  /* FAKEMATCH(wrapper): Sound_Play called through call.h's Call1; as a
   * direct call, 23 of 1038 instructions differ in tla-en. */
  Event_PlaySound(0x253);
  System_Wait(10);
  /* FAKEMATCH(wrapper): Sound_Play called through call.h's Call1; as a
   * direct call, 15 of 1038 instructions differ in tla-en. */
  Event_PlaySound(0x253);
  System_Wait(10);
  Event_PlaySound(0x253);
  System_Wait(40);
  *(s32 *)&gData_02006b6c = 3;
  System_Wait(30);
  *(s32 *)&gData_02006b6c = 2;
  System_Wait(20);
  /* FAKEMATCH(wrapper): Actor_SetHorizontalPositionWithTerrain called through call.h's Call3; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetPosition(17, 0, 0);
  /* FAKEMATCH(wrapper): Actor_SetHorizontalPositionWithTerrain called through call.h's Call3; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetPosition(27, 0, 0);
  /* FAKEMATCH(wrapper): Actor_SetHorizontalPositionWithTerrain called through call.h's Call3; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
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
  Actor_SetSpriteColor((Actor_Get(30)), 9);
  Actor_SetSpriteColor((Actor_Get(31)), 9);
  Actor_SetSpriteColor((Actor_Get(32)), 9);
  FieldEvent_Wait(20);
  Actor_SetSpriteColor((Actor_Get(30)), 0);
  Actor_SetSpriteColor((Actor_Get(31)), 0);
  Actor_SetSpriteColor((Actor_Get(32)), 0);
  FieldEvent_Wait(20);
  t85 = Random16Far();
  FIELD_AT(t1, u8, 98) = (((u32)(((t85 << 2) + t85) << 4) >> 16) + 40);
  t86 = Random16Far();
  FIELD_AT(t2, u8, 98) = (((u32)(((t86 << 2) + t86) << 4) >> 16) + 40);
  t87 = Random16Far();
  FIELD_AT((spill), u8, 98) = (((u32)(((t87 << 2) + t87) << 4) >> 16) + 40);
  FIELD_AT(t1, s32, 108) = (s32)Func_0200008c;
  FIELD_AT(t2, s32, 108) = (s32)Func_0200008c;
  FIELD_AT((spill), s32, 108) = (s32)Func_0200008c;
  FieldEvent_Wait(20);
  PaletteFade_ApplyTarget(0x10000, 1);
  PaletteFade_Interpolate(40);
  System_Wait(80);
  FieldEvent_SetMessageId((s32)MsgWhoAreThey);
  /* FAKEMATCH(wrapper): Actor_ArmCallbackAndRefresh called through call.h's Call2; as a
   * direct call, 24 of 1038 instructions differ in tla-en. */
  Actor_FaceDirectionAndWait(26, 0x8000);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(26, 0);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(19, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 25 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(22, 0x2000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 26 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection((*(s32 *)owner), 0x2000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 30 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(23, 0x4000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 15 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(24, 0x4000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 23 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(21, 0x2000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 24 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(20, 0x2000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 10 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(26, 0x4000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(25, 0x4000, 0);
  /* FAKEMATCH(wrapper): Camera_SetSpeedParameters called through call.h's Call2; as a
   * direct call, 59 of 1038 instructions differ in tla-en. */
  FieldCamera_SetSpeed(0x19999, 0x3333);
  FIELD_AT((FieldCamera_GetActor()), u8, 85) = 0;
  FieldCamera_MoveTo(0x2000000, 0x500000, 0x1540000, 1);
  Func_02005734();
  Actor_ShowEmote(19, 0x109, 20);
  Event_ShowMessage(19, 0);
  Event_PlaySound(8);
  /* FAKEMATCH(wrapper): Func_080d489c called through call.h's Call2; as a
   * direct call, 8 of 1038 instructions differ in tla-en. */
  Actor_SetAttachedEffect(24, 0x102);
  FieldEvent_Wait(20);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(24, 0);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(25, 0);
  /* FAKEMATCH(wrapper): Motion_SetModeAndWaitAnimation called through call.h's
   * Call2; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetAnimationAndWait(19, 3);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(19, 0);
  /* FAKEMATCH(wrapper): Func_080d489c called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetAttachedEffect(26, 0x102);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(0x101a, 0);
  /* FAKEMATCH(wrapper): Actor_SetModeById called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetAnimation(19, 4);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(19, 0);
  Actor_ShowEmote(23, 0x103, 0);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 51 of 1038 instructions differ in tla-en. */
  Actor_SetSpeed(23, 0x19999, 0xcccc);
  /* FAKEMATCH(wrapper): Actor_SetPositionAndReset called through call.h's
   * Call3; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_WalkToAndWait(23, 0x200, 0x148);
  /* FAKEMATCH(wrapper): Actor_ArmCallbackAndRefresh called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_FaceDirectionAndWait(23, 0x2000);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(0x1017, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallbackAndRefresh called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_FaceDirectionAndWait(20, 0x8000);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(0xa014, 0);
  Actor_ShowEmote(19, 0x100, 20);
  FieldActor_SetSpritePriority(21, 2);
  FIELD_AT((Actor_Get(20)), u8, 98) = 0;
  FIELD_AT((Actor_Get(21)), u8, 98) = 0;
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 38 of 1038 instructions differ in tla-en. */
  Actor_SetSpeed(21, 0xcccc, 0x6666);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetSpeed(20, 0xcccc, 0x6666);
  Actor_SetScript(21, (s32)gData_02005f08);
  Actor_SetScript(20, (s32)gData_02005f50);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 4 of 1038 instructions differ in tla-en. */
  Actor_SetSpeed(19, 0x26666, 0x13333);
  /* FAKEMATCH(wrapper): Actor_SetPositionAndReset called through call.h's
   * Call3; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_WalkToAndWait(19, 0x21c, 0x154);
  /* FAKEMATCH(wrapper): Actor_Jump called through call.h's Call3; as a direct
   * call, 2 of 1038 instructions differ in tla-en. */
  FieldActor_Jump(19, 4, 10);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(19, 0);
  actor = Actor_Get(21);
L_0200a47a:
  System_Wait(1);
  if (FIELD_AT(actor, u8, 98) == 0)
    goto L_0200a47a;
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 55 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(20, 0x2000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallbackAndRefresh called through call.h's Call2; as a
   * direct call, 55 of 1038 instructions differ in tla-en. */
  Actor_FaceDirectionAndWait(21, 0x2000);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(21, 0);
  /* FAKEMATCH(wrapper): Actor_SetSpritePriority called through call.h's Call2;
   * as a direct call, 2 of 1038 instructions differ in tla-en. */
  FieldActor_SetSpritePriority(20, 2);
  /* FAKEMATCH(wrapper): Actor_SetSpritePriority called through call.h's Call2;
   * as a direct call, 2 of 1038 instructions differ in tla-en. */
  FieldActor_SetSpritePriority(22, 2);
  /* FAKEMATCH(wrapper): this call through W_Func_02005714; as Actor_ShowEmote(20,
   * 0x100, 0), 103 of 1038 instructions differ in tla-en (as Call3, 2). */
  W_Func_02005714(20, 0x100, 0);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 55 of 1038 instructions differ in tla-en. */
  Actor_SetSpeed(20, 0x19999, 0xcccc);
  /* FAKEMATCH(wrapper): Actor_SetPositionAndReset called through call.h's
   * Call3; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_WalkToAndWait(20, 0x1ea, 0x128);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 55 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(21, 0xe000, 20);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 139 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(20, 0xa000, 20);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 55 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(20, 0xe000, 20);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 27 of 1038 instructions differ in tla-en. */
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
  /* FAKEMATCH(wrapper): Actor_SetPositionAndReset called through call.h's
   * Call3; as a direct call, 53 of 1038 instructions differ in tla-en. */
  Actor_WalkToAndWait(20, 0x1e6, 0x134);
  FieldEvent_Wait(1);
  t170 = Actor_Get(20);
  FIELD_AT(t170, u8, 90) |= 1;
  /* FAKEMATCH(wrapper): Camera_SetSpeedParameters called through call.h's Call2; as a
   * direct call, 57 of 1038 instructions differ in tla-en. */
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(0x2000000, 0x600000, 0x1400000, 1);
  Func_02005734();
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(21, 0, 40);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 55 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(21, 0xe000, 20);
  /* FAKEMATCH(wrapper): Actor_ArmCallbackAndRefresh called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_FaceDirectionAndWait(21, 0);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(0x8015, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 128 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(22, 0xc000, 0);
  owner2 = (s32)&gMainRoutineWork;
  owner2 = (owner2 + 0x214);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 118 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection((*(s32 *)owner2), 0xa000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 111 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(23, 0xa000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 100 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(24, 0xa000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 107 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(26, 0xa000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 88 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(25, 0xa000, 0);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 100 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(0x8014, 0);
  actor = Actor_Get(32);
  FIELD_AT(actor, s32, 108) = 0;
  System_Wait(1);
  FIELD_AT(actor, s32, 24) = 0x10000;
  FIELD_AT(actor, s32, 28) = 0x10000;
  /* FAKEMATCH(wrapper): Motion_SetVarCbAndRefresh called through call.h's
   * Call2; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_RunRepeatedMotion(32, 2);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(32, 0);
  Actor_ShowEmote(23, 0x100, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 55 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(23, 0xe000, 40);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(25, 0x8000, 0);
  Actor_ShowEmote(25, 0x101, 20);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(25, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(23, 0, 40);
  /* FAKEMATCH(wrapper): Actor_ArmCallbackAndRefresh called through call.h's Call2; as a
   * direct call, 179 of 1038 instructions differ in tla-en. */
  Actor_FaceDirectionAndWait(23, 0xc000);
  /* FAKEMATCH(wrapper): Actor_StartRepeatedMotion called through call.h's
   * Call2; as a direct call, 2 of 1038 instructions differ in tla-en. */
  FieldActor_StartRepeatedMotion(32, 3);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(32, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 162 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(24, 0xc000, 40);
  /* FAKEMATCH(wrapper): Actor_ArmCallbackAndRefresh called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_FaceDirectionAndWait(24, 0x6000);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(0x4018, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallbackAndRefresh called through call.h's Call2; as a
   * direct call, 91 of 1038 instructions differ in tla-en. */
  Actor_FaceDirectionAndWait(21, 0x2000);
  /* FAKEMATCH(wrapper): Func_080d489c called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetAttachedEffect(21, 0x102);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(21, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 159 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection((*(s32 *)owner2), 0xc000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 160 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(24, 0xc000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 155 of 1038 instructions differ in tla-en. */
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
  /* FAKEMATCH(wrapper): Motion_SetVarCbAndRefresh called through call.h's
   * Call2; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_RunRepeatedMotion(31, 1);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(0x201f, 0);
  /* FAKEMATCH(wrapper): Motion_SetVarCbAndRefresh called through call.h's
   * Call2; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_RunRepeatedMotion(30, 2);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(30, 0);
  /* FAKEMATCH(wrapper): Actor_Jump called through call.h's Call3; as a direct
   * call, 2 of 1038 instructions differ in tla-en. */
  FieldActor_Jump(20, 2, 10);
  /* FAKEMATCH(wrapper): Actor_Jump called through call.h's Call3; as a direct
   * call, 2 of 1038 instructions differ in tla-en. */
  FieldActor_Jump(20, 4, 10);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(0x8014, 0);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 85 of 1038 instructions differ in tla-en. */
  Actor_SetSpeed(20, 0x26666, 0x13333);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 85 of 1038 instructions differ in tla-en. */
  Actor_SetSpeed((*(s32 *)owner2), 0x26666, 0x13333);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 83 of 1038 instructions differ in tla-en. */
  Actor_SetSpeed(23, 0x26666, 0x13333);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 31 of 1038 instructions differ in tla-en. */
  Actor_SetSpeed(24, 0x26666, 0x13333);
  Actor_SetScript(20, (s32)gData_02005f98);
  Actor_SetScript((*(s32 *)owner2), (s32)gData_02005fdc);
  Actor_SetScript(23, (s32)gData_02006004);
  Actor_RunScriptAndWait(24, (s32)gData_0200602c);
  FieldEvent_Wait(20);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 18 of 1038 instructions differ in tla-en. */
  Actor_SetSpeed(22, 0x19999, 0xcccc);
  /* FAKEMATCH(wrapper): Actor_SetPositionAndReset called through call.h's
   * Call3; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_WalkToAndWait(22, 0x210, 0x140);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(22, 0x2000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallbackAndRefresh called through call.h's Call2; as a
   * direct call, 10 of 1038 instructions differ in tla-en. */
  Actor_FaceDirectionAndWait(25, 0xa000);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(0x5016, 0);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetSpeed(25, 0x19999, 0xcccc);
  Actor_SetScript(25, (s32)gData_02006070);
  /* FAKEMATCH(wrapper): Actor_SetPositionAndReset called through call.h's
   * Call3; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_WalkToAndWait(22, 0x1e6, 0x134);
  /* FAKEMATCH(wrapper): Actor_ArmCallbackAndRefresh called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_FaceDirectionAndWait(22, 0xa000);
  Actor_SetSpeed(21, 0x26666, 0x13333);
  t244 = Actor_Get(21);
  FIELD_AT(t244, u8, 90) &= 254;
  Actor_WalkToAndWait(21, 0x1da, 0x13c);
  FieldEvent_Wait(1);
  t248 = Actor_Get(21);
  FIELD_AT(t248, u8, 90) |= 1;
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(21, 0xe000, 0);
  /* FAKEMATCH(wrapper): Actor_SetPositionAndReset called through call.h's
   * Call3; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_WalkToAndWait(22, 0x1ce, 0x118);
  /* FAKEMATCH(wrapper): Actor_ArmCallbackAndRefresh called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_FaceDirectionAndWait(22, 0);
  FieldActor_SetSpritePriority(22, 1);
  Func_020053e4((Actor_Get(22)), (s32)gData_02006c20);
  Func_020053e4((Actor_Get(25)), (s32)gData_02006b70);
  FieldEvent_Wait(40);
  /* FAKEMATCH(wrapper): Motion_SetModeAndWaitAnimation called through call.h's
   * Call2; as a direct call, 2 of 1038 instructions differ in tla-en. */
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

#include "event_work.h"

extern const u8 MsgWhatSTheMatter[];
void Func_020055fc();
void Party_RemoveOwnerRestored();

static __inline__ void Func_02005714_(s32 actor, s32 a, s32 b) {
  Func_080d47b4(actor, a, b);
}

void Func_0200298c(s32 id) {
  struct Actor *object = Actor_Get(id);

  object->collisionFlags = 0;
  object->motionFlags = 0;
  FieldActor_SetSpritePriority(id, 1);
}

/// The scene that opens with MsgWhatSTheMatter; either answer to its prompt
/// steps the event's message number.
void Func_020029ac(void) {
  Func_0200560c();
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
  Func_02005714_(3, 0x109, 0x14);
  Event_ShowMessage(0x1003, 0);
  Actor_FaceDirectionAndWait(0, FACING_NORTHEAST);
  Actor_SetAnimationAndWait(7, 4);
  Event_ShowMessage(0x2007, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(5, FACING_NORTHEAST);
  Func_02005714_(5, 0x107, 0);
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
  Func_02005714_(5, 0x109, 0x14);
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
  Func_02005714_(0x13, 0x109, 0);
  Event_PlaySound(0x4e);
  FieldCamera_SetSpeed(0x3333, 0x666);
  FieldCamera_MoveTo(PIXELS(512), -1, PIXELS(320), 1);
  Func_02005734();
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
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(540), PIXELS(76), PIXELS(340), 1);
  Func_02005734();
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
  Func_02005734();
  Actor_FaceDirection(6, FACING_SOUTH, 40);
  Actor_FaceDirectionAndWait(6, FACING_WEST);
  Func_02005714_(6, 0x103, 0x14);
  Actor_FaceDirectionAndWait(6, FACING_SOUTHWEST);
  FieldActor_Jump(6, 4, 0xa);
  Event_ShowMessage(0x6006, 0);
  FieldCamera_MoveTo(PIXELS(480), -1, PIXELS(276), 1);
  Func_02005734();
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
  Func_02005714_(0, 0x109, 0x14);
  Event_ShowMessage(0x9000, 0);
  Actor_FaceDirectionAndWait(0, FACING_NORTHWEST);
  {
    s32 flags = 0;

    Event_OpenMessage(0x8000, flags);
  }
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
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
  Func_02005714_(6, 0x109, 0x28);
  Event_ShowMessage(6, 0);
  Actor_FaceDirection(2, FACING_WEST, 0);
  Func_02005714_(2, 0x105, 0x28);
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
  Func_020055fc(1);
  Party_RemoveOwnerRestored(7);
  Party_RemoveOwnerRestored(5);
  Party_RemoveOwnerRestored(6);
  Party_RemoveOwnerRestored(0);
  Party_RemoveOwnerRestored(3);
  Party_RemoveOwnerRestored(2);
  Party_RemoveOwnerRestored(1);
  Actor_Get(20)->collisionFlags |= 8;
  Actor_Get(21)->collisionFlags |= 8;
  Actor_SetSpriteColor(Actor_Get(28), 0xf);
  Actor_SetPosition(28, PIXELS(568), PIXELS(274));
  Actor_Get(17)->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(Actor_Get(17), PIXELS(492), PIXELS(96), PIXELS(284));
  Actor_Get(29)->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(Actor_Get(29), PIXELS(510), PIXELS(96), PIXELS(295));
  Actor_Get(30)->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(Actor_Get(30), PIXELS(540),
                                  PIXELS(96), PIXELS(300));
  System_Wait(1);
  Event_SetFlag(0x17c);
  Event_SetFlag(0x144);
  Func_02005614();
}

#include "field_event.h"

void Func_02003210(struct FieldActor *self);

/// Circles the actor around actor 23 by its timer, at its speed plus 3 in x
/// and twice the sine in z, and turns the timer back by 0x800; Func_02000148
/// of the Jupiter Lighthouse's fifth room is the same function.
void Func_02003210(struct FieldActor *self) {
  struct FieldActor *centre = (struct FieldActor *)Actor_Get(23);
  u16 angle = self->timer;
  s32 sine;
  s32 x;
  s32 z;

  self->x = centre->x + Math_Cosine(angle) * (self->speed + 3);
  sine = Math_Sin(angle);
  z = centre->z;
  x = self->x;
  z += sine * 2;
  self->z = z;
  self->targetX = x;
  /* FAKEMATCH(dead): the target z store stays ahead of the timer load;
   * without it, 2 of 40 instructions differ in tla-en. */
  do {
  } while (0);
  self->targetZ = z;
  *(s16 *)&self->timer += -0x800;
}

#include "frame.h"

struct OrbitEffect {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x1c];
  s32 radius;
  u8 unk34[0x1c];
  struct FieldSprite *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[0x0e];
  u16 angle;
  u16 unk66;
  u8 unk68[4];
  void (*update)(struct OrbitEffect *effect);
};

extern u32 gData_02006afc[];

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
void Func_0200326c(void) {
  struct FieldActor *center;
  struct OrbitEffect *effect;
  struct FieldSprite *sprite;
  s32 angle;

  if (Event_GetFlag(0x200) == 0 && gFrameCount % 3 != 0)
    return;
  center = (struct FieldActor *)Actor_Get(23);
  if (Event_GetFlag(0x200) != 0) {
    u32 random;

    random = Random16Far();
    effect = CreateOrbitEffect(0x2a0, center->x,
                               (s32)((u32)(random << 8) >> 16 << 16) +
                                   center->y - 0x1c0000,
                               center->z);
  } else {
    u32 random;

    random = Random16Far();
    effect = CreateOrbitEffect(0x2a0, center->x,
                               (s32)((u32)(random << 6) >> 16 << 16) +
                                   center->y - 0x1c0000,
                               center->z);
  }
  if (effect != 0) {
    sprite = effect->sprite;
    Func_0200556c(effect, gData_02006afc);
    Actor_SetSpriteColor(effect, 1);
    effect->motionFlags = 0;
    angle = Random16Far() & 0xffff000;
    effect->angle = angle;
    effect->unk66 = 0;
    effect->update = (void (*)(struct OrbitEffect *))Func_02003210;
    effect->radius = Math_Sin((u32)(Random16Far() * 0xffff) >> 20) * 24 >> 16;
    sprite->nextFrame = 0;
    sprite->priority = 2;
  }
}

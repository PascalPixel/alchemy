/// @file
/// The scene code of overlay 0x69e, in ROM order.

// ---- games/tla/src/scene/jupiter_lighthouse_room_5.c ----

#include "shared/whirlwind.h"
#include "shared/jupiter_lighthouse.h"
#include "shared/actors.h"
#include "shared/effects.h"
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
#include "field_event.h"
#include "field/event_inline.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "macros.h"
#include "system/task.h"
#include "system/frame.h"
#include "event_work.h"
#include "item_icon.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "field/cell_target.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "shared/jupiter_lighthouse_aerie.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field_sprite.h"
#include "call.h"
#include "system/field_effect.h"
#include "field/field_coordinate.h"
#include "gamepak/far_call/system.h"
#include "graphics/resource.h"
#include "map.h"
#include "graphics/vram_tab.h"
#include "shared/cracked_tile.h"
#include "system/heap_release.h"
#include "system/io_write.h"
#include "shared/tables.h"
#include "module/field/field_effect.h"
#include "objcmd.h"
#include "text/msg_ids.h"

/// What The Lost Age's code declares for the files that use it, written once.

struct Actor;
struct CellTarget;
struct CellTargetEntry;
struct SparkSprite;

enum {
  EFFECT_DEFAULT_TYPE = 285,
  EFFECT_SCALE_ONE = 0x10000,
  EFFECT_OPTION_MASK = 0xffff0000,
  EFFECT_SCRIPT_REPEAT_COUNT = 3
};

extern u32 gOrbScript[];

void Func_02001718(const s32 *from, const s32 *to);

void JupiterLighthouse_LoadPalettes(s32 value);

void Func_02001538(void);

void Actor_PlaceBlocks(void *data);

void Func_020029e0(struct SparkSprite *sprite, s32 width, s32 height,
                   s32 attributes, s32 tile);

extern s16 gLightSpriteTile;
extern s16 gLightSpriteCount;
extern struct Actor *gLightSpriteActors[];
extern s16 gLightSpritesLarge;
extern s16 gLightSpritePriority;

void Leader_CenterInCell(void);

extern struct CellTarget gCrackedTiles;

void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);

void Func_020006ac(void);

void Func_02000724(void);

void Func_020008a4(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

struct FieldEffect;
struct SparkSprite__jupiter_lighthouse_room_5;

void Actor_PlaceMovedBlock(void);
extern u16 gPaletteCycleColors[];
extern u16 gPaletteCycleUnlitPalette[];
extern u16 gPaletteCycleUnlitColors[];
void MapGrid_SetAttributeBAt();
void Import_ActorSelectAnimation(struct FieldEffect *effect, s32 mode);
void Func_020029e8(struct SparkSprite__jupiter_lighthouse_room_5 *sprite);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

extern s16 gData_02002fb8[];

void Func_02000054(void) { Actor_LiftTargetBlock(gData_02002fb8); }

void Actor_RaiseFromCell(s32 id);

void Func_02000064(s32 unused, s32 id) {
  struct Actor *object;
  s32 i;

  object = Actor_Get(id);
  if (id == 11) {
    if (!Event_GetFlag(0xa00) || object->x >> 20 != 13 || object->z >> 20 != 34)
      return;
    object->motionFlags = 0;
    for (i = 15; i >= 0; i--) {
      object->y += 0x20000;
      System_Wait(1);
    }
    Actor_RaiseFromCell(id);
    Event_SetFlag(0xa09);
  } else {
    Actor_PlaceMovedBlock();
  }
}

u32 __umodsi3();
void Actor_SetSpriteColorMode();

DEFINE_ACTOR_CYCLE_PICTURES(Actor_CyclePictures)

void Func_02000120(struct Actor *object) {
  u32 frameCount = gFrameCount;

  if (frameCount & 1)
    Actor_SetSpriteColorMode(object, (frameCount >> 1) % 6);
}

/// Circles the object around actor 23, as overlay 6a5's Func_02000090 circles
/// actor 31.
void Orb_Update(struct Actor *self) {
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

void Actor_StartScript(struct OrbitEffect *object, const void *script);

static inline struct OrbitEffect *CreateOrbitEffect(s32 type, s32 x, s32 y,
                                                    s32 z) {
  /* FAKEMATCH(wrapper): the effect created through an inline, which
   * evaluates the position before the type; as a direct call of
   * Import_ActorCreate, 4 of 94 instructions differ in tla-en. */
  return ((struct OrbitEffect *)Import_ActorCreate(type, x, y, z));
}

/// Every third frame, or every frame once flag 0x201 is set, starts a particle
/// circling actor 23 at a random height above it.
DEFINE_ORBS_SPAWN_RISING(Orbs_SpawnRising, 0x201, gOrbScript, Orb_Update, 1)

/* VIEW(jupiter_lighthouse_room_5): routine gData_02003274 as this part declared
 * it; the joined file has another; tried as the definition's extern
 * DEFINE_G_DATA_02003214(gOrbScript) u32 gData_02003274[], it does not compile
 * in tla-ja: games/tla/src/scene/jupiter_lighthouse_room_5.c:207:
 * `gData_02003274' undeclared (first use in this function). */
extern s32 gData_02003274__jupiter_lighthouse_room_5 __asm__("gData_02003274");
#define gData_02003274 gData_02003274__jupiter_lighthouse_room_5
/* VIEW(jupiter_lighthouse_room_5): routine gData_02003278 as this part declared
 * it; the joined file has another; tried as the definition's extern u32
 * gData_02003278[], it does not compile in tla-ja:
 * games/tla/src/scene/jupiter_lighthouse_room_5.c:209: `gData_02003278'
 * undeclared (first use in this function). */
extern s32 gData_02003278__jupiter_lighthouse_room_5 __asm__("gData_02003278");
#define gData_02003278 gData_02003278__jupiter_lighthouse_room_5

void Func_02001650();

void Func_02000294(void) {
  if (gData_02003278 != 0)
    Event_PlaySound(201);
  Func_02001650(gData_02003274);
}

extern u32 gData_02002f70[];
/* VIEW(jupiter_lighthouse_room_5): routine gData_02002f94 as this part declared
 * it; the joined file has another; tried as the one declaration extern const u8
 * gData_02002f94[], it does not compile in tla-ja:
 * games/tla/src/scene/jupiter_lighthouse_room_5.c:317: `gData_02002f94'
 * undeclared (first use in this function). */
extern const u8
    gData_02002f94__jupiter_lighthouse_room_5[] __asm__("gData_02002f94");
#define gData_02002f94 gData_02002f94__jupiter_lighthouse_room_5

void Func_0200185c(void);

static __inline__ void Bring(s32 id) {
  struct Actor *object;

  Actor_SetSpritePriority(id, 2);
  Actor_SetSpriteColor(Actor_GetById(id), 7);
  object = Actor_GetById(id);
  object->motionFlags = 0;
  object->priorityFlags |= 2;
  object->scaleY = 0xffff0000;
  object->scaleX = 0x1999;
  Actor_SetMovementTypeById(id, gData_02002f70);
}

/// The event when the leader stands on cell (42, 41): sets flag 2560, turns the
/// leader (Actor_FaceDirectionById), plays its hooks and sounds 220 and
/// 178, starts the spark ring from above the leader to (680, 0, 574)
/// (Func_02001718) and a pulse callback (Func_02000294) that quickens over 20
/// steps, sounds 390 and 144 with the screen shaking, objects 23 and 24 brought
/// in on their action scripts, flashes, the room's cells copied open
/// (MapGrid_CopyCellPictureCellsRect) and JupiterLighthouse_LoadPalettes(2);
/// then the callbacks stop, objects 13 to 16 rise 2 pixels a frame for 16
/// frames, take their next state (Actor_RaiseFromCell) and sound 80 plays.
void Func_020002b8(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *a = Actor_GetById(13);
  struct Actor *b = Actor_GetById(14);
  struct Actor *c = Actor_GetById(15);
  struct Actor *d = Actor_GetById(16);
  s32 from[3];
  s32 to[3];
  s32 i;

  if (leader->x >> 20 != 42 || leader->z >> 20 != 41)
    return;
  Event_SetFlag(2560);
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_NORTH);
  FieldEvent_Wait(30);
  leader->update = Func_02000120;
  FieldEvent_Wait(30);
  Event_PlaySound(220);
  Actor_SetAnimationSpeed(leader, 64);
  FieldEvent_Wait(50);
  Event_PlaySound(178);
  leader->timer = 0;
  leader->update = Actor_CyclePictures;
  FieldEvent_Wait(30);
  from[0] = leader->x;
  from[1] = leader->y + 0x140000;
  from[2] = leader->z;
  to[0] = 170 << 18;
  to[1] = 0;
  to[2] = 0x23e0000;
  Func_02001718(from, to);
  gData_02003274 = 0;
  gData_02003278 = 1;
  Task_AddFrame(Func_02000294, 0x480);
  for (i = 0; i < 20; i++) {
    gData_02003274 = 0;
    System_Wait((20 - i) / 2);
    gData_02003274 = 1;
    System_Wait(1);
  }
  Event_PlaySound(390);
  MapGrid_SetShake(0x10000, 0x10000, 0x10000);
  gData_02003274 = 1;
  FieldEvent_Wait(90);
  MapGrid_SetShake(0x30000, 0x30000, 0x10000);
  gData_02003274 = 3;
  gData_02003278 = 0;
  FieldEvent_Wait(40);
  Bring(23);
  Bring(24);
  Task_AddFrame(Orbs_SpawnRising, 0x480);
  Event_PlaySound(144);
  MapGrid_SetShake(0x60000, 0x60000, 0x10000);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x4063ff, 0);
  PaletteFade_Interpolate(60);
  System_Wait(60);
  Event_PlaySound(144);
  System_Wait(30);
  Event_PlaySound(144);
  System_Wait(30);
  Event_PlaySound(144);
  System_Wait(30);
  MapGrid_CopyCellsLayered(58, 99, 3, 3, 41, 99);
  MapGrid_CopyCellsLayered(123, 32, 1, 5, 106, 32);
  JupiterLighthouse_LoadPalettes(2);
  Event_PlaySound(144);
  System_Wait(30);
  leader->timer = 3;
  System_Wait(1);
  leader->update = 0;
  Actor_SetAnimationSpeed(leader, 16);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 1);
  Actor_SetScript(23, gData_02002f94);
  Actor_SetScript(24, gData_02002f94);
  gData_02003274 = 2;
  FieldEvent_Wait(30);
  gData_02003274 = 1;
  FieldEvent_Wait(30);
  gData_02003274 = 0;
  FieldActor_RefreshSelectorById(23);
  FieldActor_RefreshSelectorById(24);
  Actor_GetById(23)->scaleX = 0;
  Actor_GetById(24)->scaleX = 0;
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(30);
  MapGrid_SetShake(0x20000, 0x20000, 0x10000);
  FieldEvent_Wait(30);
  MapGrid_SetShake(0, 0, 0x10000);
  FieldTask_Remove(Func_02000294);
  Func_0200185c();
  FieldEvent_Wait(30);
  a->motionFlags = 0;
  b->motionFlags = 0;
  c->motionFlags = 0;
  d->motionFlags = 0;
  for (i = 0; i < 16; i++) {
    a->y += PIXELS(2);
    b->y += PIXELS(2);
    c->y += PIXELS(2);
    d->y += PIXELS(2);
    System_Wait(1);
  }
  Actor_RaiseFromCell(13);
  Actor_RaiseFromCell(14);
  Actor_RaiseFromCell(15);
  Actor_RaiseFromCell(16);
  Event_PlaySound(80);
  Sound_WaitForJingle();
}

struct SparkWork {
  u8 unk0000[0x1d9c];
  s16 resource;
};

/* VIEW(jupiter_lighthouse_room_5): routine gPaletteCyclePausesInEvents as this
 * part declared it; the joined file has another; tried as the definition's
 * extern s32 gPaletteCyclePausesInEvents, it does not compile in tla-ja:
 * games/tla/src/scene/jupiter_lighthouse_room_5.c:1252: conflicting types for
 * `gPaletteCyclePausesInEvents'. */
extern s32 gPaletteCyclePausesInEvents__jupiter_lighthouse_room_5 __asm__(
    "gPaletteCyclePausesInEvents");
#define gPaletteCyclePausesInEvents                                            \
  gPaletteCyclePausesInEvents__jupiter_lighthouse_room_5

/* VIEW(jupiter_lighthouse_room_5): routine FieldEffect_Spawn as this part
 * declared it; the joined file has another; tried as the definition's void
 * FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY, s32
 * velocityZ, u32 flags, const struct EffectOptions *options), which the object
 * already declares, Func_02001230 moves: 1 of 90 instructions in tla-ja. */
void FieldEffect_Spawn__jupiter_lighthouse_room_5(
    s32 x, s32 y, s32 z, s32 a3, s32 a4, s32 a5, s32 a6,
    struct EffectParams *params) __asm__("FieldEffect_Spawn");
#define FieldEffect_Spawn FieldEffect_Spawn__jupiter_lighthouse_room_5

void Func_020006ac(void) {
  if (!Event_GetFlag(0xa0e)) {
    Event_SetFlag(0xa0e);
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    PaletteFade_ApplySource(0x10000, 0);
    PaletteFade_ApplyTarget(0x10003, 1);
    PaletteFade_Interpolate(30);
    Event_WaitTransitionFrames();
    Actor_SetAnimation(0, 1);
    FieldEvent_Wait(30);
    FieldMessage_ShowCentered((s32)MsgFieldShowAnemosPower, 0, -2);
    PaletteFade_ApplyTarget(0x10000, 0);
    PaletteFade_Interpolate(30);
    Event_End();
  }
}

void Func_02000724(void) { GameFlag_Clear(0xa0e); }

/// Actor 22 rises in a shower of sparks: one every other frame for 32 frames.
void Func_02000734(void) {
  struct EffectParams params;
  struct EffectParams *p;
  struct SparkActor *actor;
  u32 i;
  s32 x;
  s32 y;
  u32 random;
  u32 random2;
  u32 scaled;

  actor = ((struct SparkActor *)Actor_Get(22));
  Event_PlaySound(190);
  Actor_SetSpriteFlags(((struct SparkActor *)Actor_Get(22)), 0);
  p = &params;
  p->count = 1;
  p->kind = 5;
  p->tile = 0x2a0;
  p->spread = 0x6666;
  p->rise = 0x30000;
  for (i = 0; i <= 31; i++) {
    FieldEvent_Wait(1);
    if (!(i & 1)) {
      random = Random16();
      // random * 24, written in steps around the two barriers below, so x
      // loads between the multiply's add and its last shift.
      scaled = random << 1;
      scaled += random;
      x = actor->x;
      x += (((scaled << 3) >> 16) << 16);
      x += -0xc0000;
      random2 = Random16();
      y = actor->y;
      FieldEffect_Spawn(x, y + (((random2 << 5) >> 16) << 16) + 0x200000,
                        actor->z, 0, -0x40000, 0, 0x1b0000, p);
    }
    if (i == 20)
      Actor_SetSpriteColorById(22, 0x100);
  }
  Actor_SetSpriteColorById(22, 0);
  Actor_SetSpriteFlags(((struct SparkActor *)Actor_Get(22)), 1);
}

void Func_02000810(s32 actor, s32 target) {
  s32 i;

  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode(Actor_Get(actor), 2);
  Event_PlaySound(201);
  System_Wait(50);
  Event_PlaySound(82);
  for (i = 0; i < 120; i++) {
    {
      struct Actor *object = Actor_Get(target);
      u32 facing = object->facing;

      object->facing = facing + 0x1000;
    }
    FieldActor_SetSpriteColor(Actor_Get(target), (i & 4) ? 7 : 0);
    System_Wait(1);
  }
  Actor_Get(target)->facing = 0xc000;
  FieldActor_SetSpriteColor(Actor_Get(target), 0);
  BattleEffect_SetObjectUpdateMode(Actor_Get(actor), 0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  Actor_SetAnimation(target, 1);
}

#define EVENT_WORK                                                             \
  ((struct JupiterLighthouseRoom5EventWork *)Ram_HeapSlots->eventWork)

/// Once flag 0xa24 is set and neither 0xa23 nor 0xa22 is, actor 4 turns
/// about, actor 22 rises in sparks and talks with it, then joins it; flags
/// 0xa25, 0x161 and 0x144 are set.
void Func_020008a4(void) {
  s32 angle;

  if (!Event_GetFlag(0xa24))
    return;
  if (Event_GetFlag(0xa23))
    return;
  if (Event_GetFlag(0xa22))
    return;
  Event_SetFlag(0xa22);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId(MsgFieldWaitingForCompanion);
  FieldEvent_ShowMessageThenWait(22, 0, 5);
  Actor_WalkToAndWait(4, 68, 280);
  Actor_WalkToAndWait(4, 68, 272);
  Actor_FaceDirectionAndWait(4, FACING_WEST);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(4, FACING_NORTH);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(4, FACING_EAST);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(4, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_SetSpriteFlags(((void *)Actor_Get(22)), 0);
  FieldActor_SetSpriteColor(((void *)Actor_Get(22)), 15);
  angle = 0x4000;
  Actor_SetPositionAndFacing(22, PIXELS(64), PIXELS(225), angle);
  Func_02000734();
  FieldEvent_Wait(40);
  FieldCamera_MoveTo(PIXELS(68), -1, PIXELS(240), 1);
  Event_OpenMessage(22, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
    Actor_SetAnimationAndWait(22, 4);
    FieldEvent_ShowMessageThenWait(22, 0, 5);
    EVENT_WORK->message++;
  } else {
    Actor_SetAnimationAndWait(22, 3);
    FieldEvent_Wait(10);
    EVENT_WORK->message++;
    FieldEvent_ShowMessageThenWait(22, 0, 5);
  }
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(22, 0, 5);
  Actor_ShowEmote(22, 265, 30);
  FieldEvent_ShowMessageThenWait(22, 0, 5);
  Actor_SetAnimationAndWait(22, 4);
  FieldEvent_ShowMessageThenWait(22, 0, 5);
  FieldEvent_ShowMessageThenWait(22, 0, 5);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(30);
  FieldCamera_MoveTo(PIXELS(68), -1, PIXELS(256), 1);
  Actor_WalkByAndWait(22, 0, 16);
  FieldEvent_Wait(30);
  Func_02000810(22, 4);
  Party_RestoreHpPp(1);
  Message_ShowCentered((s16)EVENT_WORK->message++, 5);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(22, 0, 5);
  Event_SetFlag(0xa25);
  Event_SetFlag(0x161);
  Event_SetFlag(0x144);
  Event_End();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

/// Scene preparation: the event value 0x204, actor 22 placed while flag 0xa22
/// is set (cleared once 0xa23 is), actors 23 and 24 placed, and with flag
/// 0xa00 two more and the particle callback.
s32 Scene_Initialize(void) {
  struct Actor *object;
  s32 angle;

  ((struct JupiterLighthouseRoom5EventWork1ac *)Ram_HeapSlots->eventWork)
      ->value = 0x204;
  if (Event_GetFlag(0xa23))
    Event_ClearFlag(0xa22);
  if (Event_GetFlag(0xa22)) {
    angle = 0x4000;
    Actor_SetPositionAndFacing(22, PIXELS(64), PIXELS(225), angle);
  }
  Actor_PlaceBlocks(gData_02002fb8);
  Actor_SetActionCallback(Actor_Get(64), 9);
  object = Actor_Get(23);
  object->scaleX = 0;
  object->motionFlags = 0;
  SET_POSITION(object, 0x2a80000, 0x200000, 0x2440000);
  Actor_SetSpriteFlags(Actor_Get(23), 0);
  object = Actor_Get(24);
  object->scaleX = 0;
  object->motionFlags = 0;
  SET_POSITION(object, 0x2a80000, 0x400000, 0x2440000);
  Actor_SetSpriteFlags(Actor_Get(24), 0);
  if (Event_GetFlag(0xa00)) {
    MapGrid_CopyCellsLayered(58, 99, 3, 3, 41, 99);
    MapGrid_CopyCellsLayered(123, 32, 1, 5, 106, 32);
    Task_AddFrame(Orbs_SpawnRising, TASK_PASS_VBLANK);
  }
  return 0;
}

/// Scene setup: the arrival flags and palette, the leader's priority bit 0x40,
/// and the actors flags 0xa00 and 0xa09 bring in.
s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  JupiterLighthouse_LoadPalettes(0);
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 0x40;
  if (Event_GetFlag(0xa00))
    MapGrid_SetCellBehavior(0xd00000, 0x2a00000, 2, 236);
  if (Event_GetFlag(0xa00)) {
    Actor_RaiseFromCell(10);
    Actor_RaiseFromCell(12);
    Actor_RaiseFromCell(13);
    Actor_RaiseFromCell(14);
    Actor_RaiseFromCell(15);
    Actor_RaiseFromCell(16);
  }
  if (Event_GetFlag(0xa09)) {
    Actor_SetPosition(11, PIXELS(216), PIXELS(552));
    Actor_RaiseFromCell(11);
  }
  return 0;
}

#include "shared/scene_set_arrival_flags.c"
#undef EVENT_WORK

// ---- games/tla/src/scene/jupiter_lighthouse_room_5/s0cb8.c ----
// Scene code of overlay 0x69e, in ROM order, from s0cb8's offset up to the
// next object the overlay links.

void PaletteCycle_SetPausesInEvents(s32 value) {
  gPaletteCyclePausesInEvents = value;
}

/* VIEW(s0cb8): routine gPaletteCycleFrame as this part declared it; the joined
 * file has another; tried as the definition's extern s32 gPaletteCycleFrame, it
 * does not compile in tla-ja:
 * games/tla/src/scene/jupiter_lighthouse_room_5.c:618: `gPaletteCycleFrame'
 * undeclared (first use in this function). */
extern s32 gPaletteCycleFrame__s0cb8 __asm__("gPaletteCycleFrame");
#define gPaletteCycleFrame gPaletteCycleFrame__s0cb8

/// Sets the counter, its value built as a word.
static inline void Counter_Set(s32 value) { gPaletteCycleFrame = value; }

DEFINE_PALETTE_CYCLE_STEP(PaletteCycle_Step, gPaletteCycleFrame,
                          gPaletteCyclePausesInEvents, gPaletteCycleColors)

void MapGrid_SetCellHeight();

DEFINE_JUPITER_LIGHTHOUSE_LOAD_PALETTES(JupiterLighthouse_LoadPalettes,
                                        gPaletteCyclePausesInEvents,
                                        gPaletteCycleColors,
                                        gPaletteCycleUnlitPalette,
                                        PaletteCycle_Step,
                                        gPaletteCycleUnlitColors)

DEFINE_ACTOR_RAISE_FROM_CELL(Actor_RaiseFromCell, MapGrid_SetCellHeight,
                             MapGrid_SetAttributeBAt)

DEFINE_WHIRLWIND_CARRY_LEADER(Whirlwind_CarryLeader)

DEFINE_POSITION_FLAT_DISTANCE(Position_FlatDistance)

extern s32 gData_020033e0[];
extern s32 gData_02003400;

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);

void Func_02001070(struct Actor *self) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 position[3];
  s32 distance;
  s32 near;

  if (*(s16 *)(work + 358) != 0 || *(s16 *)(work + 346) != 0 ||
      *(s16 *)(work + 350) != 0 || *(s16 *)(work + 360) != 0) {
    Actor_SetAnimationSpeed(self, 0);
    return;
  }
  Actor_SetAnimationSpeed(self, 16);
  if ((++*(u16 *)&self->timer & 31) == 31)
    Event_PlaySound(231);
  {
    s8 *to = self->sprite;
    s8 *from = leader->sprite;
    u32 a = from[9] & 12;
    to[9] = (s8)((to[9] & ~12) | a);
  }
  if (MapGrid_GetCellBehavior(self->x, self->z, 2) != 255) {
    position[0] = self->x;
    position[1] = self->y;
    position[2] = self->z;
    Camera_WorldToScreen(position);
    distance = position[0];
    if (distance > 0x1100000)
      goto stop;
    if (distance < -0x200000)
      goto stop;
    {
      if (*((u8 *)self + 0x62) == 0) {
        distance = leader->y - self->y;
        if (distance < 0)
          distance = self->y - leader->y;
        near = 0;
        if (Position_FlatDistance(&leader->x, &self->x) <= 12 &&
            distance < 0xc0000)
          near = 1;
        if (near && !Event_GetFlag(260)) {
          leader->facing = self->facing;
          leader->watchedObject = self;
          leader->priorityFlags &= ~1;
          {
            s32 delay = 200;
            *(u16 *)(work + 362) = delay;
          }
          gMainRoutineWork.renderMode = 0;
          *((u8 *)self + 0x62) = 1;
          *((u8 *)self + 0x63) = 1;
        }
      }
      {
        s32 (*multiply)(s32, s32);
        s32 cosine = Trig_Cos(self->facing);
        s32 speed = self->speedLimit;
        /* FAKEMATCH(dead): the routine address taken inside a once-only
         * block, so the speed loads first; as a plain call, 2 of 197
         * instructions differ in tla-en. */
        do {
          multiply = Iwram_MulQ16;
        } while (0);
        self->x += multiply(cosine, speed);
      }
      self->z += Iwram_MulQ16(Math_Sin(self->facing), self->speedLimit);
      return;
    }
  }
stop:
  {
    s32 zero = 0;
    *((u8 *)self + 0x63) = zero;
    self->update = zero;
    Actor_StartScript(self, gData_020033e0);
    Event_PlaySound(228);
    gData_02003400 = zero;
  }
}

// ---- games/tla/src/scene/jupiter_lighthouse_room_5/s1230.c ----
// Scene code of overlay 0x69e, in ROM order, from s1230's offset up to the next
// object the overlay links.

struct GustObject *Func_02001230(struct GustObject *source) {
  struct Vector3 position;
  struct EffectOptions options;
  struct GustObject *object;

  Event_PlaySound(222);
  position.x = source->x + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                  Trig_Cos(source->angle));
  position.z = source->z + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                  Math_Sin(source->angle));
  object = ((struct GustObject *)FieldActor_Create(280, position.x, source->y,
                                                   position.z));
  Import_ActorSelectAnimation(object, 2);
  Actor_SetSpriteFlags(object, 0);
  object->motionFlags = 0;
  object->priorityFlags &= ~1;
  object->angle = source->angle;
  object->unk64 = 0;
  object->unk62 = 0;
  object->unk63 = 0;
  object->update = Func_02001070;
  object->speedLimit = 0x18000;
  options.priority = 1;
  options.palette = 7;
  FieldEffect_Spawn(position.x, source->y, position.z - 0x60000, 0, 0, 0,
                    0x30000, &options);
  return object;
}

s32 Func_02001308(struct Actor *object) {
  struct EffectOptions options;
  s32 phase, x, y, speed;

  phase = gFrameCount & 7;
  if (phase != 0)
    goto done;
  options.priority = 3;
  options.startScaleX = 0xb333;
  options.startScaleY = 0xb333;
  options.palette = 14;
  x = object->x + (((s32)((u32)(Random16() * 9) >> 16) - 4) << 16);
  y = object->y + ((32 - (s32)((u32)(Random16() * 20) >> 16)) << 16);
  speed = (((s32)((u32)(Random16() * 5) >> 16) << 16) + 0x50000) / 10;
  FieldEffect_Spawn(x, y, object->z, 0, speed, phase,
                    EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                        EFFECT_USE_START_SCALE,
                    &options);
done:
  return 0;
}

DEFINE_JUPITER_LIGHTHOUSE_PLACE_RAISED_ACTORS(
    JupiterLighthouse_PlaceRaisedActors)

struct Spark {
  u8 unk00[0x0c];
  s32 dx;
  s32 dy;
  s32 duration;
  s32 age;
};

/* VIEW(jupiter_lighthouse_room_5): tag struct SparkSprite as struct
 * SparkSprite__jupiter_lighthouse_room_5; module/tla.h has another definition;
 * tried with the definition the rest of the object has, it does not compile in
 * tla-ja: games/tla/src/scene/jupiter_lighthouse_room_5.c:837: structure has no
 * member named `x'. */
struct SparkSprite__jupiter_lighthouse_room_5 {
  u8 unk00[0x0c];
  s32 x;
  s32 z;
  u8 unk14[0x14];
};

/* VIEW(s1230): tag struct SparkWork as struct SparkWork__s1230;
 * jupiter_lighthouse_room_5.c has another definition; tried with the definition
 * the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/jupiter_lighthouse_room_5.c:812: structure has no member
 * named `sparks'. */
struct SparkWork__s1230 {
  u8 unk0000[0x400];
  struct Spark sparks[96];
  struct SparkSprite__jupiter_lighthouse_room_5 sprites[96];
  s32 from[3];
  s32 to[3];
  s16 next;
};

void Func_02001538(void) {
  struct SparkWork__s1230 *work = Ram_HeapSlots->menuRuntime;
  struct Spark *spark = work->sparks;
  struct SparkSprite__jupiter_lighthouse_room_5 *sprite = work->sprites;
  s32 position[3];
  s32 duration;
  s32 i;

  for (i = 95; i >= 0; sprite++, i--, spark++) {
    duration = spark->duration;
    if (spark->age < 0 || spark->age > duration)
      continue;
    {
      s32 sine = Math_Sin((s32)((spark->age << 15) / duration));
      s32 *p = position;
      p[0] = work->from[0] +
             (work->to[0] - work->from[0]) * spark->age / duration +
             sine * spark->dx;
      sine = Math_Sin((s32)((spark->age << 15) / duration));
      p[1] = work->from[1] +
             (work->to[1] - work->from[1]) * spark->age / duration +
             sine * spark->dy;
      p[2] =
          work->from[2] + (work->to[2] - work->from[2]) * spark->age / duration;
      Camera_WorldToScreen(p);
      sprite->x = p[0];
      sprite->z = p[2];
      Func_020029e8(sprite);
      spark->age++;
    }
  }
}

// ---- scene/jupiter_lighthouse_room_5/s1650.c ----

/// Scene code of overlay 0x69e, in ROM order.

/* VIEW(s1650): tag struct SparkWork as struct SparkWork__s1650;
 * games/tla/src/scene/jupiter_lighthouse_room_5 has another definition; tried
 * with the definition the rest of the object has, it does not compile in
 * tla-ja: games/tla/src/scene/jupiter_lighthouse_room_5.c:873: structure has no
 * member named `sparks'. */
struct SparkWork__s1650 {
  u8 tiles[0x400];
  struct Spark sparks[96];
  struct SparkSprite sprites[96];
  s32 from[3];
  s32 to[3];
  s16 next;
  u16 tile;
  s16 vramEntry;
};

/// Adds count sparks to the spark ring's 96 entries in heap slot 220, each
/// with a random angle from 0x6000 across three quarters of a turn, the
/// cosine and negated sine of it at 64 pixels as its direction, a random
/// speed of 16 to 31 and an age of 0.
void Func_02001650(s32 count) {
  struct SparkWork__s1650 *work = Ram_HeapSlots->menuRuntime;
  struct Spark *spark;
  s32 angle;
  s32 i;
  s32 radius;
  u64 cosine;
  s32 speed;

  if (count > 0) {
    i = count;
  next:
    spark = &work->sparks[work->next];
    angle = ((Random16() * 3) >> 2) + 0x6000;
    radius = 0x400000;
    speed = ((Random16() << 4) >> 16) + 16;
    /* FAKEMATCH(temporary): the cosine held in a 64-bit temporary; as
     * Iwram_MulQ16(Trig_Cos(angle), radius), 14 of 92 instructions differ
     * in tla-en. */
    cosine = Trig_Cos(angle);
    spark->dx = Iwram_MulQ16(cosine, radius) >> 16;
    spark->dy = -Iwram_MulQ16(Math_Sin(angle), radius) >> 16;
    spark->duration = speed;
    spark->age = 0;
    work->next++;
    work->next %= 96;
    if (--i != 0)
      goto next;
  }
}

void Func_02001718(const s32 *from, const s32 *to) {
  struct SparkWork__s1650 *work =
      HeapSlot_Alloc(220, sizeof(struct SparkWork__s1650));
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct SparkSprite *sprite;
  struct Spark *spark;
  s32 entry;
  s32 tile;
  s32 i;

  work->from[0] = from[0];
  work->from[1] = from[1];
  work->from[2] = from[2];
  work->to[0] = to[0];
  work->to[1] = to[1];
  work->to[2] = to[2];
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_ColouredOrbs),
                      work);
  entry = VramCache_FindFreeEntry();
  tile = Vram_Load(entry, 0x400, work);
  work->tile = tile;
  work->vramEntry = entry;
  i = 0;
  spark = work->sparks;
  sprite = work->sprites;
  for (; i <= 95; i++, sprite++, spark++) {
    Func_020029e0(sprite, 4, 4, 0, tile + (i & 15) * 2);
    sprite->fullColor = 1;
    sprite->palette = 0;
    sprite->priority = BattleEffect_GetObjectPriority(leader) & 3;
    // Through a union, so this store may alias the age store after it.
    ((union {
       u16 v;
       s32 w;
     } *)&sprite->depth)
        ->v = BattleEffect_GetObjectDepth(leader) - 1;
    spark->age = -1;
  }
  {
    s32 zero = 0;

    work->next = zero;
  }
  Task_AddFrame(Func_02001538, 0x480);
}

void Func_0200185c(void) {
  struct SparkWork__s1650 *work = Ram_HeapSlots->menuRuntime;

  FieldTask_Remove(Func_02001538);
  VramCache_FreeEntry(work->vramEntry);
  HeapSlot_Release(220);
}

extern struct LightOam gLightSpriteOams[];

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one when the big switch is set, at its animation
/// picture and depth.
DEFINE_LIGHT_SPRITES_DRAW(LightSprites_Draw, gLightSpriteTile, gLightSpriteOams,
                          gLightSpriteCount, gLightSpriteActors,
                          gLightSpritesLarge, gLightSpritePriority,
                          BattleEffect_GetObjectPriority,
                          Oam_AddSpriteAtDepthClamped)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_LIGHT_SPRITES_INIT_A(LightSprites_InitA, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_B(LightSprites_InitB, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_C(LightSprites_InitC, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_ADD(LightSprites_Add, gLightSpriteCount,
                         gLightSpriteActors)

void LightSprites_SetPriority(s32 value) { gLightSpritePriority = value; }

#include "../field/object/set_sprite_priority.c"

#include "../field/field_effect/move.c"

#undef EVENT_WORK
#undef gData_02003274
#undef gData_02003278
#undef gData_02002f94
#undef gPaletteCyclePausesInEvents
#undef FieldEffect_Spawn
#undef gPaletteCycleFrame
// @file
// Scene code of overlay 0x69e, in ROM order.

extern const s32 *gFieldEffect_DurationScripts[];

void Actor_SetSpriteFlags(struct Actor *object, u32 value);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct Actor *party = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  s32 target;
  s32 count;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = ((struct FieldEffect *)Import_ActorCreate(options->type, x, y, z));
  else
    effect = ((struct FieldEffect *)Import_ActorCreate(EFFECT_DEFAULT_TYPE, x,
                                                       y, z));
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Import_ActorSelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect,
                    gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect,
                               ((struct FieldSprite *)party->sprite)->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & EFFECT_OPTION_MASK) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    target = options->targetScaleX;
    count = script[EFFECT_SCRIPT_REPEAT_COUNT];
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
      effect->scaleRateX = (target - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) /
                           script[EFFECT_SCRIPT_REPEAT_COUNT];
    } else {
      effect->scaleRateX = (target - EFFECT_SCALE_ONE) / count;
      effect->scaleRateY = (options->targetScaleY - EFFECT_SCALE_ONE) /
                           script[EFFECT_SCRIPT_REPEAT_COUNT];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Import_ActorSelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

// The cells CrackedTiles_Step watches, and the cell it copies over the one the
// leader steps on.
#define CELL_TABLE gCrackedTiles
#define CELL_SPRITE_DRAW CrackedTiles_Update
#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map/cell_sprite.c"

DEFINE_CRACKED_TILES_FIND(CrackedTiles_Find, gCrackedTiles, CrackedTiles_Update)

DEFINE_LEADER_CENTER_IN_CELL(Leader_CenterInCell)

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
DEFINE_CRACKED_TILE_CRACK(CrackedTile_Crack, Leader_CenterInCell)

// ---- scene/jupiter_lighthouse_room_5/s22b8.c ----

// Scene code of overlay 0x69e, in ROM order.

/* VIEW(s22b8): tag struct CellTarget as struct CellTarget__s22b8; cell_target.h
 * has another definition; tried with the definition the rest of the object has,
 * CrackedTile_Fall moves: 2 of 131 instructions in tla-ja. */
struct CellTarget__s22b8 {
  s16 firstFlag;
  s16 flagByte;
  s16 kind;
  s16 x;
  s16 z;
  s16 count;
  s16 current;
  s16 shown;
  s16 tile;
};

DEFINE_CRACKED_TILE_FALL(CrackedTile_Fall, CellTarget__s22b8,
                         Leader_CenterInCell)

#include "../field/field_effect/advance_motion.c"

#undef CELL_TABLE
#undef CELL_SPRITE_DRAW
#undef CELL_PUSH_SLOT_ENTRY
// @file
// Scene code of overlay 0x69e, in ROM order.

/* VIEW(s2450): routine FieldEffect_Spawn as this part declared it; the joined
 * file has another; tried as the definition's void FieldEffect_Spawn(s32 x, s32
 * y, s32 z, s32 velocityX, s32 velocityY, s32 velocityZ, u32 flags, const
 * struct EffectOptions *options), which the object already declares,
 * CrackedTile_Fall (gone) moves: 131 of 131 instructions in tla-ja. */
void FieldEffect_Spawn__s2450(
    s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY, s32 velocityZ, u32 flags,
    struct EffectOptions *options) __asm__("FieldEffect_Spawn");
#define FieldEffect_Spawn FieldEffect_Spawn__s2450

DEFINE_CRACKED_TILE_DROP_IN(CrackedTile_DropIn, FieldEffect_Spawn)

void GameFlag_SetByte(s32 flag, s32 value);

DEFINE_CRACKED_TILES_STEP(CrackedTiles_Step, gCrackedTiles, CrackedTile_Crack,
                          CrackedTile_Fall)

DEFINE_CRACKED_TILE_PLACE_LANDING(CrackedTile_PlaceLanding, gCrackedTiles,
                                  CrackedTile_DropIn)

#include "text/msg_ids.h"

void Func_02000054(void);
void Func_020002b8(void);

// clang-format off
DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_ShortScript, 22)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_MediumScript, 44)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_LongScript, 126)

u32 gData_02002f70[] = {
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_WAIT(1),
    SC_LOOP(32, SC_TOP),
    SC_END,
};

DEFINE_G_DATA_02002F94(gData_02002f94, 960, 140)

s16 gData_02002fb8[] = {
    17, 18, 19, -1,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

DEFINE_G_SCENE_EXITS_6(258, 257, 2, 258, 3, 2, 263, 258, 8, 258, 7, 259)

struct ActorPlacement gActorPlacements[] = {
    {341, CONDITION_ALWAYS, 1, PIXELS(536), 0, PIXELS(608), FACING_SOUTHWEST, 2, 0},
    {341, CONDITION_ALWAYS, 1, PIXELS(824), 0, PIXELS(608), FACING_SOUTHEAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(552), FACING_SOUTHEAST, 2, 0},
    {316, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(600), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(552), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(616), 0, PIXELS(664), FACING_EAST, 2, 0},
    {316, CONDITION_ALWAYS, 1, PIXELS(648), 0, PIXELS(664), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(712), 0, PIXELS(664), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(744), 0, PIXELS(664), FACING_EAST, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(280), FACING_SOUTH, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(296), FACING_SOUTH, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(232), FACING_SOUTH, 2, 0},
    {341, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(328), FACING_SOUTHWEST, 2, 0},
    {341, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(328), FACING_SOUTHEAST, 2, 1},
    {57, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 1, 0},
    {294, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {294, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 1},
    {294, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 1},
    {294, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 1},
    {294, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 1},
    {SCENE_TABLE_END},
};

DEFINE_G_DATA_02003214(gOrbScript)

u32 gData_02003274[] = {
    0,
};

u32 gData_02003278[] = {
    1,
};

struct Trigger gSceneEvents[] = {
    {49, 1, CONDITION_ALWAYS, 1},
    {33, 2, CONDITION_ALWAYS, 2},
    {49, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {33, 9, CONDITION_ALWAYS, 9},
    {33, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_STEP, 21, 0xa00, EVENT(Func_020006ac)},
    {TRIGGER_STEP, 22, 0xa00, EVENT(Func_02000724)},
    {TRIGGER_STEP, 100, CONDITION_ALWAYS, EVENT(Func_020008a4)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgFieldFollowTrueFeelings)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgFieldPartWithAgatio)},
    {8, -1, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {9, -1, CONDITION_ALWAYS, EVENT(Func_02000064)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 17, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 17, CONDITION_ALWAYS, EVENT(Func_02000064)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 18, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 18, CONDITION_ALWAYS, EVENT(Func_02000064)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 19, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 19, CONDITION_ALWAYS, EVENT(Func_02000064)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 11, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, CONDITION_ALWAYS, EVENT(Func_02000064)},
    {PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_ACTS), 0, 0xa00, EVENT(Func_020002b8)},
    {TRIGGER_TABLE_END},
};

s32 gPaletteCycleFrame[] = {
    -1,
};

u32 gPaletteCyclePausesInEvents[] = {
    1,
};

s32 gData_020033e0[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};
#undef FieldEffect_Spawn

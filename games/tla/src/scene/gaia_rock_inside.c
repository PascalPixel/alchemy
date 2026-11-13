/// @file
/// Gaia Rock's inside, the rooms of this overlay near Izumo: the party places
/// the doll on the altar by the entrance and on the altars deeper in
/// (GaiaRockInside_PlaceDollOnFirstAltar to
/// GaiaRockInside_PlaceDollOnRoom5Altar), each one weakening the Serpent, and
/// clears overgrowth and cobwebs with Cyclone. In the Serpent's lair Susa
/// fights to save Kushinada (GaiaRockInside_MeetSusa); examining the Serpent
/// starts the battle with it (GaiaRockInside_ChallengeSerpent). Felix reads
/// the rock's tablet and learns Sand (GaiaRockInside_ReadSandTablet).

#include "shared/actors.h"
#include "types.h"
#include "actor.h"
#include "field/event_inline.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_tables.h"
#include "system/main_routine_work.h"
#include "version.h"
#include "system/ram_buffer.h"
#include "system/field_effect.h"
#include "field_sprite.h"
#include "event_work.h"
#include "module/field/event_commands.h"
#include "module/field/object.h"
#include "module/scene/scene.h"
#include "shared/effects.h"
#include "flags.h"
#include "module/field/field.h"
#include "module/tla.h"
#include "module/field/field_effect.h"
#include "call.h"
#include "system/iwram_call.h"
#include "system/frame.h"
#include "system/io_reg.h"
#include "fixed_math.h"
#include "module/system/system.h"
#include "actor_view.h"
#include "map.h"
#include "shared/tables.h"
#include "system/task.h"
#include "objcmd.h"
#include "../field/object/set_sprite_priority.c"

#include "../field/object/prepare_object.c"

DEFINE_SCENE_CREATE_ACTOR(Scene_CreateActor)

#include "../field/field_effect/move.c"

// @file
// Scene code of overlay 0x681, in ROM order.

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect/spawn.c"

#undef EFFECT_SPAWN

// ---- scene/gaia_rock_inside/gaia_rock_inside.c ----

// Scene code of overlay 0x681, in ROM order.

/// What The Lost Age's code declares for the files that use it, written once.

struct Actor;

// One grid cell: the step that stands on it (0 for none, 99 for the border)
// and the column and row of the next step.
typedef struct {
  s8 unk_0;
  s8 unk_1;
  s8 unk_2;
} Unk_Cell;

// One step of the path: its number, then the step found below, to the left,
// to the right and above it.
typedef struct {
  u8 unk_0;
  u8 unk_1;
  u8 unk_2;
  u8 unk_3;
  u8 unk_4;
  u8 unk_5;
  u8 unk_6;
  u8 unk_7;
} Unk_Step;

// At gInventorySnapshot + 0x288.
typedef struct {
  s32 unk_0;
  Unk_Step steps[15];
} Unk_Path;

struct Counted {
  s32 *script;
  u8 unk04[0x57];
  u8 movementState;
  u8 unk5c[4];
  u8 count;
};

struct EventWork900 {
  u8 unk000[362];
  u16 timer;
};

struct CellPoint {
  s16 position[2];
};

struct BoulderSprite {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 layer : 2;
};

struct GaiaRockInsideOrbit {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x30];
  s32 centreX;
  s32 centreZ;
  s32 radius;
  u8 unk50[0x12];
  u8 phase;
  u8 growth;
  s16 angle;
  s16 speed;
};

struct GaiaRockInsideEventWork {
  u8 unk000[0x15a];
  u16 unk15a;
};

struct SwayState {
  u16 frame;
  u16 pending;
};

struct GaiaRockInsideLayer {
  u8 unk00[8];
  s32 x;
  s32 z;
  u8 unk10[0x28];
};

struct GaiaRockInsideMapWork {
  u8 unk000[0x108];
  struct GaiaRockInsideLayer layers[2];
};

struct GaiaRockInsideNode {
  u8 unk0[2];
  u8 next[4];
  u8 unk6[2];
};

struct GaiaRockInsidePath {
  s32 current;
  struct GaiaRockInsideNode nodes[1];
};

extern u32 gData_020050f4[];
extern s32 gData_020056c4[];
extern s32 gData_02005728[];

extern char SceneId_Ov681Room2;
extern char SceneId_Ov681Room3;
extern char SceneId_Ov681Room4;

extern struct ActorPlacement gGaiaRockInside_Placements[];
extern struct ActorPlacement gGaiaRockInside_LairPlacements[];
extern struct ActorPlacement gGaiaRockInside_Room3Placements[];
extern struct ActorPlacement gGaiaRockInside_Room4Placements[];
extern struct ActorPlacement gGaiaRockInside_Room5Placements[];
extern s32 gData_02005d6c[];

void Cyclone_CopyTouchedCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void FieldItem_JumpToLeader(s32 a, s32 b, s32 c);

extern u32 gData_020051b8[];
extern s32 gData_02005118[];
extern s32 gData_02005168[];

extern s32 gData_020064ec;
extern s32 gData_020064f0;
extern s32 Data_020064f4;

extern s32 gData_020055c0[];
extern s32 gData_02005610[];

void Func_02000f00(void);
void Func_02000fac(void);

void Actor_Release(struct Actor *object);

extern char SceneId_Ov681Room1;

s32 Scene_TalkToActor();

void Func_02002b58(s32 id, s32 x, s32 z);

void Func_02002b04(u32 id, s8 mode);
void Func_02002b8c(void);

void Func_0200495c(s32 layer, s32 keep);

extern u32 gData_020053e4[];

void Func_020044f8(s32 level);
void Func_020048a0(void);
s32 Func_02004904(void);

void Map_CopyRawCellsRect(s32 x, s32 z, s32 width, s32 height, s32 cellX,
                          s32 cellZ);

void Func_0200036c(void);

void Func_02000c38(void);

void Func_02000c74(void);

void Func_02000cf8(void);

void Func_02000d18(void);

void GaiaRockInside_SusaSaysMoveAside(void);

void GaiaRockInside_MeetSusa(void);
void GaiaRockInside_TalkToSusa(void);
void GaiaRockInside_ReadSusasMind(void);

void Func_02001d14(void);
void GaiaRockInside_ChallengeSerpent(void);

void Leader_SetLayerByte(void);

void Func_02001f84(void);
void Func_02001fa0(void);
void GaiaRockInside_CheckAltar(void);
void GaiaRockInside_PlaceDollOnFirstAltar(void);
void GaiaRockInside_PlaceDollOnWestAltar(void);
void GaiaRockInside_PlaceDollOnEastAltar(void);
void GaiaRockInside_PlaceDollOnRoom5Altar(void);

void Func_02002a14(void);

void Func_02002a24(void);
void Func_02002a34(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

extern char SceneId_Ov681Room5;

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern u32 gData_02005458[];

const u8 *Scene_GetRegions(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room5)
    return gData_02005458;
  return 0;
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_02004eec_(s32 id, s32 a, s32 b) {
  Actor_JumpDownLedge(id, a, b);
}

void Func_0200036c(void) {
  FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
  OverlayObject_PrepareObject(0x2c80000, 0, 0x1d40000, 286);
  OverlayObject_PrepareObject(0x2c80000, 0, 0x1ea0000, 286);
  Func_02004eec_(gMainRoutineWork.currentOwner, 6, 0x800);
  Event_PlaySound(310);
  Event_SetFlag(290);
  FieldEvent_SetExitRequest(8);
}

// Includes the shared FieldEffect_AdvanceMotion in this scene.
// Neither it nor its caller Func_0200042c exists in the Japanese scene.
#if TLA_PATCH_1
#include "../field/field_effect/advance_motion.c"
#endif

/* VIEW(gaia_rock_inside): routine Event_End as s32 Event_End(void);
 * module/scene/scene.h has void Event_End(void); two events return its value,
 * which the bytes need. */
extern s32 Event_EndWithResult(void) __asm__("Event_End");

// The Japanese edition has no form of this function here.
#if TLA_PATCH_1
s32 Func_0200042c(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  u32 i;
  struct EffectOptions options;
  s32 spawn = 1;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Map_CenterOnFollowedPoint();
  System_Wait(1);
  actor->y = 0x820000;
  actor->gravity = 0x8000;
  actor->bounce = 0;
  actor->motionFlags = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(204);
  actor->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = 0xa3d7;
  options.startScaleY = 0xa3d7;
  i = 0;
  // A goto loop carries no loop notes: as a do, for or while loop, loop.c
  // moves the velocity array's address out of the loop.
loop:
  {
    s32 angle;
    s32 velocity[3];
    s32 cosine;

    angle = i << 12;
    cosine = Trig_Cos(angle);
    /* FAKEMATCH(dead): spawn is always 1, a constant GCSE only folds after its
     * partial redundancy pass, so that pass leaves the velocity array's
     * address in the loop, built after the cosine call as the native has it;
     * as the plain body, 4 of 177 instructions differ in tla-en. */
    if (spawn) {
      velocity[0] = cosine;
      velocity[1] = 0;
      velocity[2] = Math_Sin(angle);
      velocity[0] += velocity[0] >> 1;
      velocity[0] += (Random16() * 0xc000 >> 16) - 0x6000;
      velocity[2] += (Random16() * 0x6000 >> 16) - 0x3000;
      FieldEffect_Spawn(actor->x, actor->y, actor->z, velocity[0], velocity[1],
                        velocity[2], 0x1090001, &options);
    }
    i++;
  }
  if (i <= 16)
    goto loop;
  Event_PlaySound(188);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 49);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  FieldMapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x100);
  actor->gravity = 0x10000;
  actor->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  return Event_EndWithResult();
}
#endif

const u32 *Scene_GetExits(void) { return gSceneExits; }

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_020005dc(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(9));
  Actor_SelectAnimation(t0, 3);
  Actor_SetAnimationSpeed(t0, 0);
  return 0;
}

s32 Func_020005f8(void) {
  struct Actor *t0;

  t0 = Actor_Get(9);
  FIELD_AT(t0->sprite, u16, 18) = (FIELD_AT(t0->sprite, u16, 18) + 0x300);
  return 0;
}

extern u32 gData_020050d0[];

s32 Func_02000610(void) {
  struct FieldSprite *sprite = Actor_Get(9)->sprite;

  sprite->rotation -= 0x300;
  return 0;
}

// Randomly vary the scale.
void Func_0200062c(struct Actor *object) {
  u32 random = Random16();
  s32 scale = object->scaleX;
  s32 bias;
  s32 limit;

  bias = -0x800;
  scale += (random << 12) >> 16;
  scale += bias;
  object->scaleX = scale;
  object->scaleY = object->scaleY + ((Random16() << 12) >> 16) + bias;
  limit = 240;
  scale = object->scaleX;
  limit <<= 4;
  limit += 255;
  if (scale <= limit)
    object->scaleX = 0x2000;
  if (object->scaleY <= limit)
    object->scaleY = 0x2000;
}

s32 Func_02000678(void) {
  struct Actor *source = Actor_Get(9);
  struct Actor *object;
  struct FieldSprite *sprite;

  object = (FieldActor_Create(14, source->x + 0x80000, source->y - 0x20000,
                              source->z));
  object->motionFlags = 0;
  Actor_SetSpriteFlags(object, 0);
  Actor_StartScript(object, gData_020050d0);
  object->scaleX = 0x6666;
  object->scaleY = 0x6666;
  sprite = object->sprite;
  sprite->priority = 1;
  object->bounce = 0;
  object->unk4c = 0;
  object->update = (void (*)(void))Func_0200062c;
  return 0;
}

extern s32 gData_02005544[];

s32 Func_020006e0(void) {
  Actor_StartScript(((s32)Actor_Get(9)), (s32)gData_02005544);
  return 0;
}

extern u32 gData_0200559c[];

s32 Func_020006f8(void) {
  Actor_StartScript(((s32)Actor_Get(9)), (s32)gData_0200559c);
  Actor_SetAttachedEffect(0xa, 0x102);
  return 0;
}

s32 Func_02000718(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(9));
  Actor_StartScript(t0, (s32)gData_020055c0);
  FIELD_AT(t0, s32, 8) = 0x1d80000;
  Actor_SetAnimationSpeed(t0, 0x10);
  Actor_SetAttachedEffect(0xa, 0x102);
  return 0;
}

s32 Func_02000748(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(9));
  Actor_StartScript(t0, (s32)gData_02005610);
  Actor_SetAnimationSpeed(t0, 0);
  Actor_SetAttachedEffect(0xa, 0x102);
  return 0;
}

#include "../field/field_effect/advance_motion_priority.c"

#undef FIELD_AT

// ---- scene/gaia_rock_inside/s07f4.c ----

// Scene code of overlay 0x681, in ROM order.

// ---- games/tla/src/scene/gaia_rock_inside/s07f4.c ----

#include "text/msg_ids.h"

/* VIEW(s07f4): tag struct DustSprite as struct DustSprite__s07f4;
 * module/scene/scene.h has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/gaia_rock_inside.c:525: structure has no member named
 * `layer'. */
struct DustSprite__s07f4 {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 layer : 2;
};

s32 Func_020007f4(void) {
  struct Actor *source = Actor_Get(9);
  s32 i;

  Event_PlaySound(202);
  i = 3;
  do {
    {
      struct Actor *dust =
          (FieldActor_Create(0x11d, source->x, source->y, source->z));

      if (dust != 0) {
        dust->motionFlags = 0;
        dust->collisionFlags = 0;
        Actor_SetSpriteFlags(dust, 0);
        Actor_SelectAnimation(dust, 1);
        Actor_StartScript(dust, gData_020050f4);
        ((struct DustSprite__s07f4 *)dust->sprite)->layer = 1;
        dust->bounce = Trig_Cos(((Random16() << 3) >> 16) << 14) >> 1;
        dust->unk4c = Math_Sin(((Random16() << 3) >> 16) << 14) >> 1;
        dust->gravity = 0;
        dust->timer = ((Random16() << 9) >> 16) - 0x100;
        dust->speedLimit = 0;
        dust->acceleration = 0;
        dust->update = Effect_AdvanceMotionAtLeaderPriority;
      }
    }
  } while (--i >= 0);
  return 0;
}

s32 Func_020008b4(void) {
  struct Actor *object = Actor_Get(9);
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);

  if (leader->z >> 19 <= 45 && leader->x >> 19 <= 57)
    Actor_StartScript(object, gData_02005728);
  else
    Actor_StartScript(object, gData_020056c4);
  return 0;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02000900(struct Counted *object) {
  struct Counted *other = (struct Counted *)Actor_Get(9);

  if (object->count == 30 || other->count == 30) {
    ((struct EventWork900 *)Ram_HeapSlots->eventWork)->timer = 42;
    if (object->count == 30)
      object->count = 31;
    if (other->count == 30)
      other->count = 31;
  }
  if (object->count > 2) {
    other->movementState = 1;
    Event_SetFlag(0x206);
  }
  {
    u8 *otherCount = &other->count;

    if (*otherCount > 2 && !Event_GetFlag(0x206) &&
        other->script != gData_020056c4 && other->script != gData_02005728) {
      object->movementState = 1;
      Event_SetFlag(0x205);
    }
    if (object->count == 0) {
      other->movementState = 0;
      Event_ClearFlag(0x206);
    }
    if (*otherCount == 0) {
      object->movementState = 0;
      Event_ClearFlag(0x205);
    }
    if (object->count > 35)
      object->count = 35;
    if (*otherCount > 35)
      *otherCount = 35;
  }
}

extern struct ActorPlacement gGaiaRockInside_Room2Placements[];

/* VIEW(s07f4): variable gData_02005ee4 as s32 gData_02005ee4;
 * scene/gaia_rock_inside/s4d00 has u32 gData_02005ee4[] = {; tried as the
 * definition's extern u32 gData_02005ee4[], it does not compile in tla-ja:
 * games/tla/src/scene/gaia_rock_inside.c:622: `gData_02005ee4' undeclared
 * (first use in this function). */
extern s32 gData_02005ee4__s07f4 __asm__("gData_02005ee4");
#define gData_02005ee4 gData_02005ee4__s07f4
extern s32 gData_02005e28[];

// Return the table for the current room.
const void *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room2)
    return gGaiaRockInside_Room2Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room3)
    return gGaiaRockInside_Room3Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room4)
    return gGaiaRockInside_Room4Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room5)
    return gGaiaRockInside_Room5Placements;
  if (gMainRoutineWork.entrance > 8)
    return gGaiaRockInside_LairPlacements;
  return gGaiaRockInside_Placements;
}

void Func_02000a60(struct Actor *object) {
  s32 value = 0;

  if ((gData_02005ee4 & 7) <= 0)
    value = 7;
  Actor_SetSpriteColorMode(object, value & 7);
  if ((gData_02005ee4 & 31) == 0) {
    object->scaleX = 0x10000;
    Actor_StartScript(object, gData_02005e28);
    Event_PlaySound(106);
  }
  gData_02005ee4++;
}

void Func_02000aac(struct Actor *object) {
  s32 value = 0;

  if ((gData_02005ee4 & 7) <= 0)
    value = 7;
  Actor_SetSpriteColorMode(object, value & 7);
  if ((gData_02005ee4 & 31) == 0) {
    object->scaleY = 0x10000;
    Actor_StartScript(object, gData_02005d6c);
    Event_PlaySound(106);
  }
  gData_02005ee4++;
}

void Func_02000af8(s32 unused, u32 id) {
  struct Actor *object = Actor_Get(id);
  u32 direction;
  s32 *state;

  direction = Func_02004904();
  state = (s32 *)(gInventorySnapshot + 0x288);
  object->collisionFlags = 0;
  Actor_SelectAnimation(object, ((direction & 0xc000) >> 14) + 2);
  if (direction & 0x4000)
    object->update = (void (*)(void))Func_02000aac;
  else
    object->update = (void (*)(void))Func_02000a60;
  object->collisionFlags = 1;
  Event_SetFlag(*state + 0xa50);
  Event_SetFlag(0x200);
  System_Wait(20);
}

// Unless FieldMapGrid_GetCellBehaviorAt(2, ...) holds, copies map cell (36,
// 23) to (38, 25), moves a leader standing on that cell aside, and sets flag
// 0x30a, or 0x9bf once FLAG_SERPENT_SLAIN is set.
void Func_02000b6c(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);

  Call7(Cyclone_CopyTouchedCells, 37, 26, 0, 3, 3, 61, 0);
  if (FieldMapGrid_GetCellBehaviorAt(2, 152 << 18, 200 << 17) == 0) {
    MapGrid_CopyCellAttributes(36, 23, 1, 1, 38, 25);
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    if (leader->x >> 20 == 38 && leader->z >> 20 == 25) {
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 258);
      Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 16, 0);
      leader->velocityY = PIXELS(6);
      FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
    }
    if (!Event_GetFlag(FLAG_SERPENT_SLAIN)) {
      Event_SetFlag(0x30a);
      FieldEvent_Wait(30);
      ObjectTable_Snapshot();
      FieldEvent_SetExitRequest(20);
    } else {
      Event_SetFlag(0x9bf);
      Event_End();
    }
  }
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ s32 GameFlag_Test_(s32 flag) { return GameFlag_Get(flag); }

static __inline__ void Func_0200500c_(s32 a, s32 b, s32 c) {
  FieldItem_JumpToLeader(a, b, c);
}

struct MainRoutineWork254 {
  u8 unk000[0x254];
  u16 value;
};

static __inline__ void Func_02004ff4_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  Cyclone_CopyTouchedCells(a, b, c, d, e, f, g);
}

void Func_02000c38(void) {
  Func_02004ff4_(16, 25, 0, 5, 5, 40, 8);
  if (GameFlag_Test_(0xa63) == 0) {
    Event_SetFlag(0xa63);
    Func_02002cb4(2);
  }
}

void Func_02000c74(void) {
  Func_02004ff4_(18, 17, 0, 3, 3, 31, 0);
  if (Event_GetFlag(0x9b7) == 0) {
    Func_0200500c_(249, 328, 344);
    ((struct MainRoutineWork254 *)&gMainRoutineWork)->value = 0x19b7;
    gMainRoutineWork.battleRule24b = 2;
    Field_RequestBattleFromEncounterTable(106, 2);
  }
}

void Func_02000cd8(void) {
  Call7(Cyclone_CopyTouchedCells, 0x13, 0x18, 0, 4, 3, 0x1f, 4);
}

void Func_02000cf8(void) { Func_02004ff4_(18, 17, 0, 3, 3, 31, 0); }

/* VIEW(s07f4): variable gData_020051d8 as const struct CellPoint
 * gData_020051d8[12]; scene/gaia_rock_inside/s4d00 has u16 gData_020051d8[] =
 * {; tried as the definition's extern u16 gData_020051d8[], it does not compile
 * in tla-ja: games/tla/src/scene/gaia_rock_inside.c:755: `gData_020051d8'
 * undeclared (first use in this function). */
extern const struct CellPoint
    gData_020051d8__s07f4[12] __asm__("gData_020051d8");
#define gData_020051d8 gData_020051d8__s07f4

void Func_02000d18(void) {
  struct Actor *guard;
  struct Actor *leader;
  s16 count;
  s16 i;
  s32 height = 8;
  s32 sourceX = 117;
  s32 sourceY = 20;

  Cyclone_CopyTouchedCells(104, sourceY, 1, height, height, sourceX, sourceY);
  if (GameFlag_Get(0x9c4))
    return;
  count = 0;
  for (i = 0; i <= 11; i++) {
    if (MapGrid_GetCellBehaviorAt(2, gData_020051d8[i].position[0] << 20,
                                  gData_020051d8[i].position[1] << 20) == 0)
      count++;
  }
  if (count <= 0)
    return;
  guard = Actor_Get(15);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (leader->z >> 19 > 53 && leader->x >> 19 > 83 && leader->x >> 19 <= 91) {
    leader->speedLimit = 0x20000;
    Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x100, 0);
    Actor_SetScript(gMainRoutineWork.currentOwner, gData_020051b8);
  }
  Actor_SetScript(15, gData_02005118);
  FieldEvent_Wait(60);
  Actor_SetScript(15, gData_02005168);
  FieldEvent_Wait(30);
  MapGrid_CopyCellsLayered(117, 20, 7, 8, 104, 20);
  MapGrid_CopyCellsLayered(111, 12, 1, 6, 111, 20);
  {
    s32 destX = 40;
    s32 destY = 84;
    MapGrid_CopyCellAttributes(49, destY, 8, 8, destX, destY);
  }
  FieldEvent_Wait(30);
  OverlayObject_PrepareObject(175 << 18, 0, 0x1d40000, 286);
  OverlayObject_PrepareObject(175 << 18, 0, 0x1ea0000, 286);
  OverlayObject_PrepareObject(181 << 18, 0, 0x1d40000, 286);
  OverlayObject_PrepareObject(181 << 18, 0, 0x1ea0000, 286);
  Event_PlaySound(204);
  guard->motionFlags = 3;
  ((struct BoulderSprite *)guard->sprite)->layer = 3;
  FieldEvent_Wait(10);
  Actor_SetPosition(15, 0, 0);
  ObjectTable_Snapshot();
  Event_SetFlag(0x9c4);
  FieldEvent_SetExitRequest(9);
}

void GaiaRockInside_SusaSaysMoveAside(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgGaiaRockInside05);
  Event_ShowMessage(0xa, 0);
  Event_End();
}

extern struct Actor gData_0200646c;
extern s32 gData_020064f8;

void Func_02000f00(void) {
  struct Actor *walker = Actor_Get(9);
  struct Actor *object = Actor_Get(10);
  u8 *sprite = object->sprite;
  u8 *animation = FIELD_AT(sprite, u8 *, 40);

  Iwram_CopyWordsEntry(&gData_0200646c, object, 128);
  gData_020064f8 = animation[21];
  Data_020064f4 = sprite[24];
  {
    s32 x = object->x;
    s32 z;

    gData_020064ec = x;
    z = object->z;
    object->update = 0;
    gData_020064f0 = z;
    object->velocityZ = 0;
    object->velocityY = 0;
    object->velocityX = 0;
    object->targetX = x;
    object->targetY = object->y;
    object->targetZ = z;
  }
  Actor_ResetTargetsAndVelocity(10);
  Actor_SetAnimationSpeed(object, 10);
  object->speedLimit = 0x10000;
  if (walker->script == gData_020055c0 || walker->script == gData_0200559c) {
    walker->movementState = 1;
    Actor_SetAnimationSpeed(walker, 0);
  }
}

void Func_02000fac(void) {
  struct Actor *walker = Actor_Get(9);
  struct Actor *object = Actor_Get(10);

  if (gData_020064ec != object->x || gData_020064f0 != object->z) {
    s16 *savedZ = (s16 *)&gData_020064f0;

    Actor_WalkTo(10, gData_020064ec >> 16, savedZ[1]);
    FieldActor_WaitForMoveById(10);
  }
  if (walker->script == gData_020055c0 || walker->script == gData_0200559c) {
    walker->movementState = 0;
    Actor_SetAnimationSpeed(walker, 16);
  }
  Iwram_CopyWordsEntry(object, &gData_0200646c, 128);
  Actor_SetAnimation(10, Data_020064f4);
  Actor_SetAnimationSpeed(object, gData_020064f8);
}
#undef FIELD_AT

#undef FIELD_AT
#undef gData_020051d8
#undef gData_02005ee4
// ---- games/tla/src/scene/gaia_rock_inside/s1048.c ----
// Scene code of overlay 0x681, in ROM order, from s1048's offset up to the next
// object the overlay links.

#include "text/msg_ids.h"

/* VIEW(s1048): variable gData_0200646c as extern s32 gData_0200646c[]; s07f4
 * has extern struct Actor gData_0200646c; tried as the one declaration extern
 * s32 gData_0200646c[], it does not compile in tla-ja:
 * games/tla/src/scene/gaia_rock_inside.c:874: conflicting types for
 * `gData_0200646c'. */
extern s32 gData_0200646c__s1048[] __asm__("gData_0200646c");

// Object 10 turns to the leader, talks and walks to its place while the camera
// follows; flag 0x9de is set, and object 9's script decides whether two
// words of gData_0200646c are copied into place.
void GaiaRockInside_MeetSusa(void) {
  struct Actor *object = Actor_Get(9);

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgGaiaRockInside02);
  EventCamera_FollowActor(10, 1);
  if (object->script == gData_02005610 || object->script == gData_0200559c)
    FieldActor_RefreshSelectorById(9);
  Func_02000f00();
  Actor_SetAnimation(10, 1);
  Actor_FaceActor(10, gMainRoutineWork.currentOwner, 0);
  Actor_ShowEmote(10, 256, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(10, 0);
  Actor_WalkTo(10, 496, 456);
  FieldActor_WaitForMoveById(10);
  Actor_FaceDirection(10, FACING_EAST + FACING_STEP, 0);
  FieldEvent_Wait(10);
  Event_ShowMessage(10, 0);
  Actor_ShowEmote(10, 261, 0);
  FieldEvent_Wait(45);
  Event_ShowMessage(10, 0);
  Actor_FaceDirection(10, FACING_NORTH + FACING_STEP, 0);
  FieldEvent_Wait(30);
  FieldCamera_MoveTo(PIXELS(488), -1, PIXELS(392), 1);
  FieldCamera_WaitForArrival();
  Event_ShowMessage(10, 0);
  EventCamera_FollowActor(10, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(10, FACING_EAST + FACING_STEP, 0);
  FieldEvent_Wait(10);
  Event_ShowMessage(10, 0);
  Actor_SetAnimationAndWait(10, 3);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Event_SetFlag(0x9de);
  if (object->script == gData_020056c4 || object->script == gData_02005728) {
    gData_0200646c__s1048[2] = gData_020064ec = gData_0200646c__s1048[14];
    gData_0200646c__s1048[4] = gData_020064f0 = gData_0200646c__s1048[16];
  }
  Func_02000fac();
  Event_End();
}

// Talking to object 10 while its byte at 0x60 is at most 2: once flag 0x9ac is
// set it says one of two lines, by whether any of flags 0x9bc to 0x9bf is set;
// before that it turns to the leader and speaks.
void GaiaRockInside_TalkToSusa(void) {
  if (*((u8 *)Actor_Get(10) + 96) > 2)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (Event_GetFlag(0x9ac)) {
    if (Event_GetFlag(0x9bc) || Event_GetFlag(0x9bd) || Event_GetFlag(0x9be) ||
        Event_GetFlag(0x9bf))
      FieldEvent_SetMessageId((s32)MsgGaiaRockInside10);
    else
      FieldEvent_SetMessageId((s32)MsgGaiaRockInside06);
    Event_ShowMessage(10, 0);
  } else {
    if (Actor_Get(9)->script == gData_02005610)
      FieldActor_RefreshSelectorById(9);
    Func_02000f00();
    FieldEvent_SetMessageId((s32)MsgGaiaRockInside03);
    Actor_FaceActor(10, gMainRoutineWork.currentOwner, 0);
    FieldEvent_Wait(10);
    Event_ShowMessage(10, 0);
    Actor_SetAnimationAndWait(10, 3);
    FieldEvent_Wait(10);
    Func_02000fac();
  }
  Event_End();
}

// ---- games/tla/src/scene/gaia_rock_inside/s12ac.c ----
// Scene code of overlay 0x681, in ROM order, from s12ac's offset up to the next
// object the overlay links.

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void GaiaRockInside_ReadSusasMind(void) {
  struct Actor *t5;
  s32 t2;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  t2 = FieldPartyInventory_FindMember(0x1c3);
  if (t2 != -1) {
    FieldEvent_SetMessageId((s32)MsgGaiaRockInside07);
    Event_ShowMessage(0xa, 0);
  } else {
    t5 = Actor_Get(9);
    if (t5->script == (s32)gData_02005610) {
      FieldActor_RefreshSelectorById(9);
    }
    Func_02000f00();
    FieldEvent_SetMessageId((s32)MsgGaiaRockInside04);
    Event_ShowMessage(0xa, 0);
    Func_02000fac();
  }
  Event_End();
}

extern u32 gData_02005208[];

void Func_02001314(s32 a0) {
  s32 t0;

  t0 = (s32)FieldActor_Create(0x2a0, FIELD_AT(a0, s32, 8),
                              FIELD_AT(a0, s32, 12), FIELD_AT(a0, s32, 16));
  FIELD_AT(t0, s32, 28) = 0x6666;
  FIELD_AT(t0, s32, 24) = 0x6666;
  FIELD_AT((t0 + 0x55), u8, 0) = 0;
  FieldActor_SetSpriteColor(t0, 2);
  Scene_SetActorSpritePriority(t0, 1);
  Actor_SelectAnimation(t0, 1);
  Actor_SetSpriteFlags(t0, 0);
  Actor_SetBlendMode(t0, 1);
  Actor_StartScript(t0, (s32)gData_02005208);
}
#undef FIELD_AT

// ---- games/tla/src/scene/gaia_rock_inside/s136c.c ----
// Scene code of overlay 0x681, in ROM order, from s136c's offset up to the next
// object the overlay links.

// Steps an orbiting object around its anchor: turns it by half its speed, puts
// it on the anchor (raised 8 pixels) and pushes it out by its radius; when its
// countdown runs out returns 0, else, unless its radius is down to 2 pixels,
// takes a second, shrinking step. Returns 1 while it runs. The same code as
// overlay 67C's Func_020013bc. The first push goes through Call3 (directly, its
// offset pointer is set after the angle's load).
DEFINE_ORBITER_SPIRAL(Orbiter_Spiral, Func_02001314)

// Moves around in a circle.
s32 Orbit_Update(struct GaiaRockInsideOrbit *object) {
  s32 z;

  object->angle += object->speed;
  object->x = IwramMulQ16(Trig_Cos(object->angle), object->radius);
  z = IwramMulQ16(Math_Sin(object->angle), object->radius);
  object->x += object->centreX;
  object->z = (z >> 1) + object->centreZ;
  if (object->growth != 141 && (gFrameCount & 1)) {
    object->radius = object->growth * Math_Sin(object->phase << 10);
    object->growth += 10;
  }
  if (object->speed == 0)
    return 0;
  if (object->growth == 141)
    object->speed -= 128;
  return 1;
}

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

DEFINE_ACTOR_AIM_AT_WATCHED(Actor_AimAtWatched)

DEFINE_ACTOR_STEP_TO_WATCHED(Actor_StepToWatched)

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

DEFINE_ACTOR_EMIT_BUBBLES(Actor_EmitBubbles, FieldEffect_Spawn)
#undef BYTE_AT

// ---- scene/gaia_rock_inside/s15d0.c ----

// Scene code of overlay 0x681, in ROM order.

#include "text/msg_ids.h"

struct Spark {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x0f];
  u8 priorityFlags;
  u8 unk24[0x0c];
  s32 speedLimit;
  s32 acceleration;
  u8 unk38[0x0c];
  s32 bounce;
  s32 gravity;
  s32 unk4c;
  u8 unk50[5];
  u8 motionFlags;
  u8 unk56[0x0c];
  u8 unk62;
  u8 unk63;
  s16 action;
  u16 unk66;
  struct Actor *watchedObject;
};

/* VIEW(s15d0): variable gData_02005264 as const u8 gData_02005264[];
 * scene/gaia_rock_inside/s4d00 has DEFINE_G_DATA_02005264(gData_02005264, 24)
 * DEFINE_G_DATA_02005264(gData_02005288, 16)
 * DEFINE_G_DATA_020052AC(gData_020052ac, 24, 0)
 * DEFINE_G_DATA_020052AC(gData_020052dc, 16, 3) u16 gData_0200530c[] = {; tried
 * as the one declaration extern const u8 gData_02005264[], it does not compile
 * in tla-ja: games/tla/src/scene/gaia_rock_inside.c:1128: `gData_02005264'
 * undeclared (first use in this function). */
extern const u8 gData_02005264__s15d0[] __asm__("gData_02005264");
#define gData_02005264 gData_02005264__s15d0
/* VIEW(s15d0): variable gData_02005218 as const u8 gData_02005218[];
 * scene/gaia_rock_inside/s4d00 has DEFINE_G_DATA_02005218(gData_02005218) s32
 * gData_02005234[] = {; tried as the one declaration extern const u8
 * gData_02005218[], it does not compile in tla-ja:
 * games/tla/src/scene/gaia_rock_inside.c:1149: `gData_02005218' undeclared
 * (first use in this function). */
extern const u8 gData_02005218__s15d0[] __asm__("gData_02005218");
#define gData_02005218 gData_02005218__s15d0
extern s32 gData_02005234[];
/* VIEW(s15d0): variable gData_02005288 as const u8 gData_02005288[];
 * scene/gaia_rock_inside/s4d00 has DEFINE_G_DATA_02005264(gData_02005288, 16)
 * DEFINE_G_DATA_020052AC(gData_020052ac, 24, 0)
 * DEFINE_G_DATA_020052AC(gData_020052dc, 16, 3) u16 gData_0200530c[] = {; tried
 * as the one declaration extern const u8 gData_02005288[], it does not compile
 * in tla-ja: games/tla/src/scene/gaia_rock_inside.c:1192: `gData_02005288'
 * undeclared (first use in this function). */
extern const u8 gData_02005288__s15d0[] __asm__("gData_02005288");
#define gData_02005288 gData_02005288__s15d0
/* VIEW(s15d0): variable gData_02005244 as const u8 gData_02005244[];
 * scene/gaia_rock_inside/s4d00 has DEFINE_G_DATA_02005244(gData_02005244,
 * Orbit_Update, Actor_AimAtWatched, Actor_StepToWatched)
 * DEFINE_G_DATA_02005264(gData_02005264, 24)
 * DEFINE_G_DATA_02005264(gData_02005288, 16)
 * DEFINE_G_DATA_020052AC(gData_020052ac, 24, 0)
 * DEFINE_G_DATA_020052AC(gData_020052dc, 16, 3) u16 gData_0200530c[] = {; tried
 * as the one declaration extern const u8 gData_02005244[], it does not compile
 * in tla-ja: games/tla/src/scene/gaia_rock_inside.c:1220: `gData_02005244'
 * undeclared (first use in this function). */
extern const u8 gData_02005244__s15d0[] __asm__("gData_02005244");
#define gData_02005244 gData_02005244__s15d0
/* VIEW(s15d0): variable gData_020052ac as const u8 gData_020052ac[];
 * scene/gaia_rock_inside/s4d00 has DEFINE_G_DATA_020052AC(gData_020052ac, 24,
 * 0) DEFINE_G_DATA_020052AC(gData_020052dc, 16, 3) u16 gData_0200530c[] = {;
 * tried as the one declaration extern const u8 gData_020052ac[], it does not
 * compile in tla-ja: games/tla/src/scene/gaia_rock_inside.c:1236:
 * `gData_020052ac' undeclared (first use in this function). */
extern const u8 gData_020052ac__s15d0[] __asm__("gData_020052ac");
#define gData_020052ac gData_020052ac__s15d0
/* VIEW(s15d0): variable gData_020052dc as const u8 gData_020052dc[];
 * scene/gaia_rock_inside/s4d00 has DEFINE_G_DATA_020052AC(gData_020052dc, 16,
 * 3) u16 gData_0200530c[] = {; tried as the one declaration extern const u8
 * gData_020052dc[], it does not compile in tla-ja:
 * games/tla/src/scene/gaia_rock_inside.c:1237: `gData_020052dc' undeclared
 * (first use in this function). */
extern const u8 gData_020052dc__s15d0[] __asm__("gData_020052dc");
#define gData_020052dc gData_020052dc__s15d0

void Func_020015d0(s32 idA, s32 idB) {
  struct Actor *a = Actor_GetById(idA);
  struct Actor *b = Actor_GetById(idB);
  s32 i;
  s16 *angle;
  s32 kind = 0x2a0;
  // Unused: gives the stack frame its 12 bytes.
  UNUSED s32 unused[3];

  Sound_Play(78);
  Event_Wait(30);
  FieldCamera_SetSpeed(0x8000, 0x1000);
  FieldCamera_MoveWithinMapBounds(((s16 *)&a->x)[1] << 16,
                                  ((s16 *)&a->y)[1] << 16,
                                  ((s16 *)&a->z)[1] << 16, 1);
  Sound_Play(141);
  MapGrid_SetShake(0x20000, 0x20000, 0x10000);
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 1);
  PaletteFade_StartInterpolation(60);
  a->motionFlags = 0;
  a->acceleration = 0x3333;
  a->speedLimit = 0x3333;
  Actor_SetMovementTypeById(idA, gData_02005264);
  Sound_Play(194);
  Event_Wait(45);
  Actor_ConfigureSpriteColor(idA, 0x100);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;
  i = 0;
first:
  {
    struct Spark *spark = ((void *)Import_ActorCreate(kind, a->x, a->y, a->z));
    Sound_Play(246);
    spark->x += ((Random16() & 31) - 16) << 16;
    spark->y += ((Random16() & 15) - 8) << 16;
    spark->speedLimit = 0x80000;
    spark->acceleration = 0x3333;
    spark->motionFlags = 0;
    Actor_SetSpriteColor(spark, 2);
    Scene_SetActorSpritePriority(spark, 1);
    Actor_SelectAnimation(spark, 7);
    Actor_SetSpriteFlags(spark, 0);
    Actor_SetBlendMode(spark, 1);
    Actor_StartScript(spark, gData_02005218);
    REG_BLDALPHA = 0x1004;
    wait(2);
    REG_BLDALPHA = 0x100a;
    wait(2);
    REG_BLDALPHA = 0x1010;
    wait(2);
    REG_BLDALPHA = 0x100a;
    wait(2);
  }
  i++;
  if (i != 16)
    goto first;
  REG_BLDALPHA = 0x1008;
  wait(30);
  i = 0;
second:
  {
    struct Spark *spark = ((void *)Import_ActorCreate(kind, b->x, b->y, b->z));
    Sound_Play(195);
    angle = &spark->action;
    *angle = (Random16() & 0x7fff) + 0x8000;
    spark->unk66 = ((i & 1) * 2 - 1) << ((Random16() & 3) + 9);
    spark->motionFlags = 0;
    spark->unk4c = 0x7a0000;
    spark->watchedObject = b;
    spark->unk62 = (16 - i) * 8 + 15;
    Actor_SetSpriteColor(spark, 2);
    Scene_SetActorSpritePriority(spark, 1);
    Actor_SelectAnimation(spark, 7);
    Actor_SetSpriteFlags(spark, 0);
    Actor_SetBlendMode(spark, 1);
    FieldVector_AddPolarOffset(spark->unk4c, *angle, &spark->x);
    Actor_StartScript(spark, gData_02005234);
    if (i == 3) {
      Actor_SetSpriteColorById(idB, 0x100);
      b->motionFlags = 0;
      b->acceleration = 0x3333;
      b->speedLimit = 0x3333;
      /* FAKEMATCH(barrier): references to idA rank it over idB and hold idB in
       * r0 before the script load; as no asm, 22 of 402 instructions differ in
       * tla-en. */
      asm("" : : "r"(idA));
      Actor_SetMovementTypeById(idB, gData_02005288);
    }
    wait(8);
  }
  i++;
  if (i != 16)
    goto second;
  Sound_Play(220);
  wait(16);
  i = 2;
third:
  {
    struct Spark *spark = ((void *)Import_ActorCreate(
        kind, b->x, (i << 16) + b->y - 0x80000, b->z));
    spark->action = Random16();
    spark->unk66 = 0x800;
    spark->unk62 = i;
    spark->unk63 = 1;
    spark->bounce = spark->x;
    spark->unk4c = 0;
    spark->gravity = spark->z;
    spark->watchedObject = b;
    spark->motionFlags = 0;
    spark->priorityFlags = 1;
    Actor_SetSpriteColor(spark, 2);
    Actor_SelectAnimation(spark, 7);
    Actor_SetSpriteFlags(spark, 0);
    Actor_SetBlendMode(spark, 1);
    Actor_StartScript(spark, gData_02005244);
  }
  i += 2;
  if (i != 32)
    goto third;
  Sound_Play(220);
  wait(50);
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
  PaletteFade_StartInterpolation(8);
  wait(16);
  MapGrid_SetShake(0, 0, 0);
  Actor_ConfigureSpriteColor(idA, 0);
  Actor_ConfigureSpriteColor(idB, 0);
  Sound_Play(574);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  PaletteFade_StartInterpolation(80);
  Actor_SetMovementTypeById(idA, gData_020052ac);
  Actor_SetMovementTypeById(idB, gData_020052dc);
  b->update = Actor_EmitBubbles;
  wait(120);
  b->update = 0;
  Sound_Play(390);
  Scene_PlayMusic();
  wait(1);
}

void Actor_MoveToObject(s32 id, s32 target);
void BlendScript_Suspend(void);
void BlendScript_Resume(void);

// The party meets person 11 on the ledge: they talk, the ground shakes, the
// screen blends out and back while the ledge changes, the party gains the item
// and person 11 joins the leader (flag 0x9ba).
s32 GaiaRockInside_ReadSandTablet(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  /* FAKEMATCH(volatile): the blend level rereads through its slot; as a plain
   * local, 112 of 250 instructions differ in tla-en. */
  volatile u16 level;
  s32 message;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  message = (s32)MsgGaiaRockInside09;
  ((s32 (*)())FieldMessage_Show)(message, 1);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x100, 0);
  if (leader->z >> 19 <= 39) {
    Actor_WalkTo(gMainRoutineWork.currentOwner, 0x1b8, 0x148);
    FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  }
  Actor_MoveToObject(11, gMainRoutineWork.currentOwner);
  Actor_WalkTo(gMainRoutineWork.currentOwner, 0x1d6, 0x162);
  Actor_WalkTo(11, 0x1be, 0x162);
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  FieldActor_WaitForMoveById(11);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xc000, 0);
  Actor_FaceDirection(11, 0, 0);
  FieldEvent_SetMessageId(message + 1);
  Event_ShowMessage(11, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x8000, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(11, 3);
  FieldEvent_Wait(10);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xc000, 0);
  Actor_FaceDirection(11, 0xc000, 0);
  FieldEvent_Wait(10);
  Actor_WalkTo(gMainRoutineWork.currentOwner, 0x1d2, 0x156);
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Message_ShowCentered(message + 2, 1);
  MapGrid_SetShake(0x50000, 0x70000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  FieldMapGrid_WaitForShakeToSettle();
  BlendScript_Suspend();
  level = REG_BLDALPHA & 0xff;
  while (level != 0) {
    REG_BLDALPHA = level | 0x1000;
    level--;
    System_Wait(1);
  }
  REG_DISPCNT &= ~0x200;
  Func_020015d0(12, 4);
  REG_DISPCNT |= 0xf00;
  REG_BLDALPHA = level | 0x1000;
  REG_BLDCNT = 0x3f42;
  while (level != 8) {
    REG_BLDALPHA = level | 0x1000;
    level++;
    System_Wait(1);
  }
  BlendScript_Resume();
  Actor_SetPosition(12, 0, 0);
  Party_LearnAbility(FELIX, ABILITY_SAND);
  Event_SetFlag(0x9ba);
  Actor_SetSpeed(11, 0x13333, 0x9999);
  Actor_SetAnimation(11, 2);
  {
    struct Actor *owner = Actor_Get(gMainRoutineWork.currentOwner);

    if (owner != 0)
      Actor_SetDestination(11, ((s16 *)&owner->x)[1], ((s16 *)&owner->z)[1]);
  }
  FieldActor_WaitForMoveById(11);
  Actor_SetPosition(11, 0, 0);
  FieldEvent_Wait(20);
  return Event_EndWithResult();
}

void Effect_AdvanceMotionLocal(union FieldObject *object) {
  struct FieldEffect *effect = &object->effect;
  s32 velocityX;
  s32 velocityZ;
  s32 scaleRateX;
  struct FieldSprite *sprite;

  velocityX = effect->velocity.x;
  effect->x += velocityX;
  effect->y += effect->velocity.y;
  velocityZ = effect->velocity.z;
  effect->z += velocityZ;

  effect->velocity.x = velocityX - velocityX / 18;
  effect->velocity.z = velocityZ - velocityZ / 16;

  scaleRateX = effect->scaleRateX;
  effect->scaleX += scaleRateX;
  sprite = effect->sprite;
  effect->scaleY += effect->scaleRateY;
  sprite->rotation += effect->spin;
}

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

void Func_02001c9c(void) {
  s32 velocity[3];
  struct EffectOptions options;
  u32 i;

  options.update = Effect_AdvanceMotionLocal;
  i = 0;
  do {
    {
      s32 angle = i << 12;

      velocity[0] = Trig_Cos(angle);
      velocity[1] = 0;
      velocity[2] = Math_Sin(angle);
      velocity[0] += velocity[0] / 3;
      FieldEffect_Spawn(0x1d00000, -0x200000, 0x1480000, velocity[0],
                        velocity[1], velocity[2], EFFECT_USE_UPDATE | 1,
                        &options);
      i += 2;
    }
  } while (i <= 16);
}

// Unless flag 0x9ee or 0x9ba is set: fades the blend out, lowers person 12 by
// one unit every 2 frames for 20 steps, with Func_02001c9c's ring at the start
// and halfway, places object 323, sets flag 0x9ee, and fades the blend back.
void Func_02001d14(void) {
  struct Actor *person;
  struct Actor *object;
  /* FAKEMATCH(volatile): the form kept; as u16 level, 146 of 173 instructions
   * differ in tla-en. */
  volatile u16 level;
  s32 i;

  if (GameFlag_Get(0x9ee) || GameFlag_Get(0x9ba))
    return;
  person = Actor_Get(12);
  level = REG_BLDALPHA;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  MapGrid_SetShake(Q16_ONE * 10, Q16_ONE * 10, Q16_ONE);
  Event_PlaySound(107);
  BlendScript_Suspend();
  level &= 0xff;
  while (level != 0) {
    REG_BLDALPHA = level | 0x1000;
    level--;
    System_Wait(1);
  }
  REG_DISPCNT &= ~0x200;
  FieldEvent_Wait(30);
  object =
      OverlayObject_PrepareObject(232 << 17, -Q16_ONE * 32, 174 << 17, 323);
  Actor_SetPosition(12, 232 << 17, 174 << 17);
  Func_02001c9c();
  for (i = 0; i <= 19; i++) {
    if (i == 10)
      Func_02001c9c();
    person->z -= Q16_ONE;
    FieldEvent_Wait(2);
  }
  Event_SetFlag(0x9ee);
  MapGrid_SetShake(-1, -1, 0xe666);
  object->x = 0;
  object->z = 0;
  FieldEvent_Wait(1);
  Actor_Release(object);
  Event_PlaySound(390);
  FieldEvent_Wait(30);
  REG_DISPCNT |= 0xf00;
  REG_BLDALPHA = level | 0x1000;
  REG_BLDCNT = 0x3f42;
  while (level != 8) {
    REG_BLDALPHA = level | 0x1000;
    level++;
    System_Wait(1);
  }
  BlendScript_Resume();
  Event_End();
}

// Leaving Gaia Rock: after FLAG_SERPENT_SLAIN a message; otherwise sets the
// next destinations (scene 0xb1, entrance 10, and 11 or 12 by the item 0x1c3
// and the four flags 0x9bc to 0x9bf) and leaves. The destination setters take
// s32: as u16 the scene symbol loads through ldrh and the pools break up.
#include "text/msg_ids.h"

void GaiaRockInside_ChallengeSerpent(void) {
  s32 count;

  if (Event_GetFlag(FLAG_SERPENT_SLAIN)) {
    Message_ShowCentered((s32)MsgGaiaRockInside08, 1);
  } else {
    s32 a = Event_GetFlag(0x9bc);
    s32 b = Event_GetFlag(0x9bd);
    s32 c = Event_GetFlag(0x9be);
    s32 scene;

    count = a + b + c + Event_GetFlag(0x9bf);
    /* FAKEMATCH(dead): the count is summed before the battle rule store;
     * without it, 8 of 68 instructions differ in tla-en. */
    do {
    } while (0);
    gMainRoutineWork.battleRule24b = 2;
    /* FAKEMATCH(dead): the scene address is loaded after the battle rule
     * store; without it, 4 of 68 instructions differ in tla-en. */
    do {
    } while (0);
    scene = (s32)&SceneId_Ov681Room1;
    FieldEvent_SetAfterBattleDestination(scene, 10);
    if (FieldPartyInventory_FindMember(0x1c3) != -1) {
      if (count == 0)
        Event_SetDefeatDestination(scene, 12);
    } else {
      Event_SetDefeatDestination(scene, 11);
    }
    Field_RequestBattleFromEncounterTable(12, count);
  }
}

#undef gData_02005218
#undef gData_02005244
#undef gData_02005264
#undef gData_02005288
#undef gData_020052ac
#undef gData_020052dc
// ---- scene/gaia_rock_inside/s1f68.c ----

// Scene code of overlay 0x681, in ROM order.

// ---- games/tla/src/scene/gaia_rock_inside/s1f68.c ----

#include "text/msg_ids.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

DEFINE_LEADER_SET_LAYER_BYTE(Leader_SetLayerByte, 2)

DEFINE_LEADER_SET_LAYER_BYTE(Func_02001f84, 0)

void Func_02001fa0(void) {
  struct Actor *t0;

  t0 = Actor_Get(66);
  t0->x = 0x2280000;
  t0->z = 0x1280000;
  t0->y = -1966080;
  t0->terrainHeight = -2097152;
  Event_ClearFlag(0x142);
  Scene_TalkToActor(0x1042);
}

// Examines the altar.
void GaiaRockInside_CheckAltar(void) {
  const u8 *message = MsgCheckedTheAltar;
  const u8 *next;
  s32 owner;

  Message_ShowCentered(message, 1);
  owner = FieldPartyInventory_FindMember(451);
  next = message + 2;
  if (owner != -1) {
    struct GaiaRockInsideEventWork *work = Ram_HeapSlots->eventWork;

    Message_ShowCentered(next, 1);
    work->unk15a = 1;
  } else {
    Message_ShowCentered(next, 1);
  }
}

#include "text/msg_ids.h"
void Actor_MoveToObject(s32 id, s32 target);

void GaiaRockInside_PlaceDollOnFirstAltar(void) {
  s32 t1, t2, t26, t27, t28, t34, t35, t37, t38, t43, t44, t50, v_r5, v_r6,
      v_r7, v_r8;
  struct Actor *v_r10;

  t1 = ((s32)Actor_Get(8));
  v_r7 = t1;
  t2 = ((s32)Actor_Get(9));
  v_r8 = t2;
  v_r10 = Actor_Get(13);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered((s32)&MsgGaiaRockInside01, 1);
  Event_PlaySound(193);
  Actor_SetSpritePriorityDirect(v_r10, 1);
  v_r5 = (s32)&gMainRoutineWork;
  v_r5 = (v_r5 + 0x214);
  Actor_MoveToObject(13, FIELD_AT(v_r5, s32, 0));
  Actor_SetDestination(13, 496, 712);
  v_r10->velocityY = 0x60000;
  FieldActor_WaitForMoveById(13);
  FieldEvent_Wait(45);
  Actor_FaceActor(8, FIELD_AT(v_r5, s32, 0), 0);
  Actor_FaceActor(9, FIELD_AT(v_r5, s32, 0), 0);
  Event_PlaySound(0x240);
  Actor_SetAnimation(8, 5);
  Actor_SetAnimation(9, 5);
  System_Wait(30);
  Actor_SetAnimation(8, 2);
  Actor_SetAnimation(9, 2);
  FieldEvent_Wait(30);
  Actor_SetAnimation(8, 3);
  Actor_SetAnimation(9, 3);
  FieldEvent_Wait(40);
  Event_PlaySound(191);
  t26 = Event_GetFlag(0xa62);
  v_r5 = 119;
  if (t26 != 0)
    goto L_0200a1d8;
  v_r5 = 19;
L_0200a10a:
  t27 = Random16();
  FIELD_AT(v_r7, s32, 40) = t27;
  t28 = Random16();
  FIELD_AT(v_r8, s32, 40) = t28;
  v_r5 = (v_r5 + -1);
  System_Wait(1);
  if (v_r5 >= 0)
    goto L_0200a10a;
  Actor_SetSpeed(8, 0x4000, 0xccc);
  Actor_SetSpeed(9, 0x4000, 0xccc);
  Actor_SetDestinationOffset(8, 24, 0);
  Actor_SetDestinationOffset(9, -24, 0);
  v_r6 = v_r7;
  v_r6 = (v_r6 + 86);
  v_r5 = 0;
  for (; v_r5 < 300 && FIELD_AT(v_r6, u8, 0) != 0; v_r5++) {
    t34 = Random16();
    FIELD_AT(v_r7, s32, 40) = t34;
    t35 = Random16();
    FIELD_AT(v_r8, s32, 40) = t35;
    System_Wait(1);
  }
  v_r5 = 9;
L_0200a18c:
  t37 = Random16();
  FIELD_AT(v_r7, s32, 40) = t37;
  t38 = Random16();
  FIELD_AT(v_r8, s32, 40) = t38;
  v_r5 = (v_r5 + -1);
  System_Wait(1);
  if (v_r5 >= 0)
    goto L_0200a18c;
  v_r5 = 40;
  MapGrid_CopyCells(119, 31, 1, 2, 89, v_r5);
  MapGrid_CopyCells(119, 31, 1, 2, 100, v_r5);
  Event_SetFlag(0xa62);
  goto L_0200a1f8;
L_0200a1d8:
  t43 = Random16();
  FIELD_AT(v_r7, s32, 40) = t43;
  t44 = Random16();
  FIELD_AT(v_r8, s32, 40) = t44;
  v_r5 = (v_r5 + -1);
  System_Wait(1);
  if (v_r5 >= 0)
    goto L_0200a1d8;
  FieldEvent_Wait(30);
L_0200a1f8:
  Actor_SetAnimation(8, 4);
  Actor_SetAnimation(9, 4);
  FieldEvent_Wait(30);
  t50 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  if (t50 == 0)
    goto L_0200a22e;
  Actor_SetDestination(13, FIELD_AT(t50, s16, 10), FIELD_AT(t50, s16, 18));
L_0200a22e:
  v_r10->velocityY = 0x60000;
  FieldActor_WaitForMoveById(13);
  Actor_SetPosition(13, 0, 0);
  Event_End();
}

// Three scene events of one shape, for objects 8 and 12, 9 and 12, and 8 and
// 14: the first object shakes with random vertical speeds, and while the flag
// of its event (0x9bd, 0x9be, 0x9bc) is clear it moves, two map copies open
// the way and the partner returns to the leader.
void GaiaRockInside_PlaceDollOnWestAltar(void) {
  struct Actor *object;
  struct Actor *partner;
  struct Actor *leader;
  s32 i;

  object = Actor_GetById(8);
  partner = Actor_GetById(12);
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgGaiaRockInside01, 1);
  Sound_Play(193);
  Actor_SetSpritePriorityDirect(partner, 1);
  Actor_MoveToObject(12, gMainRoutineWork.currentOwner);
  Actor_SetDestination(12, 528, 372);
  partner->velocityY = PIXELS(6);
  Actor_WaitForMoveById(12);
  Event_Wait(45);
  Actor_FaceActorById(8, gMainRoutineWork.currentOwner, 0);
  Sound_Play(576);
  Actor_SetModeById(8, 5);
  wait(30);
  Actor_SetModeById(8, 2);
  Event_Wait(30);
  Actor_SetModeById(8, 3);
  Event_Wait(40);
  Sound_Play(191);
  if (!GameFlag_Get(0x9bd)) {
    for (i = 19; i >= 0; i--) {
      object->velocityY = Random16();
      wait(1);
    }
    Actor_SetSpeed(8, 0x4000, 0xccc);
    Actor_SetDestinationOffsetById(8, 24, 0);
    MapGrid_CopyCellAttributes(27, 23, 1, 1, 28, 22);
    Map_CopyCellAttributeRect(32, 16, 2, 1, 29, 22);
    i = 0;
    if (object->unk56 != 0) {
      do {
        object->velocityY = Random16();
        wait(1);
        if (++i > 299)
          break;
      } while (object->unk56 != 0);
    }
    MapGrid_CopyCells(2, 93, 1, 5, 28, 85);
    for (i = 9; i >= 0; i--) {
      object->velocityY = Random16();
      wait(1);
    }
    Sound_Play(564);
    Actor_SetModeById(8, 4);
    Event_Wait(30);
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    if (leader != NULL)
      Actor_SetDestinationById(12, *(s16 *)((u8 *)leader + 10),
                               *(s16 *)((u8 *)leader + 18));
    partner->velocityY = PIXELS(6);
    Actor_WaitForMoveById(12);
    Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
    if (!GameFlag_Get(FLAG_SERPENT_SLAIN)) {
      GameFlag_Set(0x308);
      Event_Wait(30);
      ObjectTable_Snapshot();
      Import_EventSetExitRequest(20);
    } else {
      GameFlag_Set(0x9bd);
      Event_End();
    }
  } else {
    for (i = 119; i >= 0; i--) {
      object->velocityY = Random16();
      wait(1);
    }
    Event_Wait(30);
    Actor_SetModeById(8, 4);
    Event_Wait(30);
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    if (leader != NULL)
      Actor_SetDestinationById(12, *(s16 *)((u8 *)leader + 10),
                               *(s16 *)((u8 *)leader + 18));
    partner->velocityY = PIXELS(6);
    Actor_WaitForMoveById(12);
    Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
    Event_End();
  }
}

void GaiaRockInside_PlaceDollOnEastAltar(void) {
  struct Actor *object;
  struct Actor *partner;
  struct Actor *leader;
  s32 i;

  object = Actor_GetById(9);
  partner = Actor_GetById(12);
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgGaiaRockInside01, 1);
  Sound_Play(193);
  Actor_SetSpritePriorityDirect(partner, 1);
  Actor_MoveToObject(12, gMainRoutineWork.currentOwner);
  Actor_SetDestination(12, 672, 372);
  partner->velocityY = PIXELS(6);
  Actor_WaitForMoveById(12);
  Event_Wait(45);
  Actor_FaceActorById(9, gMainRoutineWork.currentOwner, 0);
  Sound_Play(576);
  Actor_SetModeById(9, 5);
  wait(30);
  Actor_SetModeById(9, 2);
  Event_Wait(30);
  Actor_SetModeById(9, 3);
  Event_Wait(40);
  Sound_Play(191);
  if (!GameFlag_Get(0x9be)) {
    for (i = 19; i >= 0; i--) {
      object->velocityY = Random16();
      wait(1);
    }
    Actor_SetSpeed(9, 0x4000, 0xccc);
    Actor_SetDestination(9, 608, 352);
    MapGrid_CopyCellAttributes(35, 23, 1, 1, 36, 22);
    Map_CopyCellAttributeRect(32, 16, 2, 1, 37, 22);
    i = 0;
    if (object->unk56 != 0) {
      do {
        object->velocityY = Random16();
        wait(1);
        if (++i > 299)
          break;
      } while (object->unk56 != 0);
    }
    MapGrid_CopyCells(2, 93, 1, 5, 36, 85);
    for (i = 9; i >= 0; i--) {
      object->velocityY = Random16();
      wait(1);
    }
    Sound_Play(564);
    Actor_SetModeById(9, 4);
    Event_Wait(30);
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    if (leader != NULL)
      Actor_SetDestinationById(12, *(s16 *)((u8 *)leader + 10),
                               *(s16 *)((u8 *)leader + 18));
    partner->velocityY = PIXELS(6);
    Actor_WaitForMoveById(12);
    Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
    if (!GameFlag_Get(FLAG_SERPENT_SLAIN)) {
      GameFlag_Set(0x309);
      Event_Wait(30);
      ObjectTable_Snapshot();
      Import_EventSetExitRequest(20);
    } else {
      GameFlag_Set(0x9be);
      Event_End();
    }
  } else {
    for (i = 119; i >= 0; i--) {
      object->velocityY = Random16();
      wait(1);
    }
    Event_Wait(30);
    Actor_SetModeById(9, 4);
    Event_Wait(30);
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    if (leader != NULL)
      Actor_SetDestinationById(12, *(s16 *)((u8 *)leader + 10),
                               *(s16 *)((u8 *)leader + 18));
    partner->velocityY = PIXELS(6);
    Actor_WaitForMoveById(12);
    Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
    Event_End();
  }
}

void GaiaRockInside_PlaceDollOnRoom5Altar(void) {
  struct Actor *object;
  struct Actor *partner;
  struct Actor *leader;
  s32 i;

  object = Actor_GetById(8);
  partner = Actor_GetById(14);
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgGaiaRockInside01, 1);
  Sound_Play(193);
  Actor_SetSpritePriorityDirect(partner, 1);
  Actor_MoveToObject(14, gMainRoutineWork.currentOwner);
  Actor_SetDestination(14, 336, 396);
  partner->velocityY = PIXELS(6);
  Actor_WaitForMoveById(14);
  Event_Wait(45);
  Actor_FaceActorById(8, gMainRoutineWork.currentOwner, 0);
  Sound_Play(576);
  Actor_SetModeById(8, 5);
  wait(30);
  Actor_SetModeById(8, 2);
  Event_Wait(30);
  Actor_SetModeById(8, 3);
  Event_Wait(40);
  Sound_Play(191);
  if (!GameFlag_Get(0x9bc)) {
    for (i = 19; i >= 0; i--) {
      object->velocityY = Random16();
      wait(1);
    }
    Actor_SetSpeed(8, 0x4000, 0xccc);
    Actor_SetDestinationOffset(8, -16, 0);
    Map_CopyCellAttributeRect(27, 15, 2, 1, 23, 23);
    Map_CopyCellAttributeRect(25, 25, 1, 1, 25, 23);
    i = 0;
    if (object->unk56 != 0) {
      do {
        object->velocityY = Random16();
        wait(1);
        if (++i > 299)
          break;
      } while (object->unk56 != 0);
    }
    MapGrid_CopyCells(1, 85, 1, 4, 25, 85);
    for (i = 9; i >= 0; i--) {
      object->velocityY = Random16();
      wait(1);
    }
    Sound_Play(564);
    Actor_SetModeById(8, 4);
    Event_Wait(30);
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    if (leader != NULL)
      Actor_SetDestinationById(14, *(s16 *)((u8 *)leader + 10),
                               *(s16 *)((u8 *)leader + 18));
    partner->velocityY = PIXELS(6);
    Actor_WaitForMoveById(14);
    Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
    if (!GameFlag_Get(FLAG_SERPENT_SLAIN)) {
      GameFlag_Set(0x307);
      Event_Wait(30);
      ObjectTable_Snapshot();
      Import_EventSetExitRequest(20);
    } else {
      GameFlag_Set(0x9bc);
      Event_End();
    }
  } else {
    for (i = 119; i >= 0; i--) {
      object->velocityY = Random16();
      wait(1);
    }
    Event_Wait(30);
    Actor_SetModeById(8, 4);
    Event_Wait(30);
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    if (leader != NULL)
      Actor_SetDestinationById(14, *(s16 *)((u8 *)leader + 10),
                               *(s16 *)((u8 *)leader + 18));
    partner->velocityY = PIXELS(6);
    Actor_WaitForMoveById(14);
    Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
    Event_End();
  }
}

/* VIEW(s1f68): variable gData_0200530c as const s16 gData_0200530c[][2];
 * scene/gaia_rock_inside/s4d00 has u16 gData_0200530c[] = {; tried as the
 * definition's extern DEFINE_G_DATA_02005244(gData_02005244, Orbit_Update,
 * Actor_AimAtWatched, Actor_StepToWatched)
 * DEFINE_G_DATA_02005264(gData_02005264, ..., it does not compile in tla-ja:
 * games/tla/src/scene/gaia_rock_inside.c:1927: `gData_0200530c' undeclared
 * (first use in this function). */
extern const s16 gData_0200530c__s1f68[][2] __asm__("gData_0200530c");
#define gData_0200530c gData_0200530c__s1f68

// Records which of eleven listed cells persons 10 and 11 stand on, as the two
// halves of the byte flag 0x210.
s32 Func_02002974(void) {
  struct Actor *first = Actor_Get(10);
  s8 firstIndex = 0;
  s32 firstX = first->x >> 19;
  s8 secondIndex = 0;
  s32 firstZ = first->z >> 19;
  struct Actor *second = Actor_Get(11);
  s32 secondX = second->x >> 19;
  s32 secondZ = second->z >> 19;
  s8 i;

  for (i = 1; i <= 11; i++) {
    if (firstX == gData_0200530c[i][0] && firstZ == gData_0200530c[i][1])
      firstIndex = i;
    if (secondX == gData_0200530c[i][0] && secondZ == gData_0200530c[i][1])
      secondIndex = i;
  }
  {
    u32 high = secondIndex << 28;
    return GameFlag_SetByte(0x210, (s32)((firstIndex << 24) | high) >> 24);
  }
}

void Func_02002a14(void) { Func_02004af0(0xb, 0x303); }

void Func_02002a24(void) { Func_02004af0(0xc, 0x304); }

void Func_02002a34(void) { Func_02004af0(0xd, 0x305); }

extern char SceneId_Ov681Room5;
extern struct Trigger gGaiaRockInside_Triggers[];
extern struct Trigger gGaiaRockInside_LairTriggers[];
extern struct Trigger gData_02005fcc[];
extern struct Trigger gGaiaRockInside_Room2Triggers[];
extern struct Trigger gGaiaRockInside_Room3Triggers[];
extern struct Trigger gGaiaRockInside_Room4Triggers[];
extern struct Trigger gGaiaRockInside_Room5Triggers[];

// Returns the current room's table.
const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room2)
    return gGaiaRockInside_Room2Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room3)
    return gGaiaRockInside_Room3Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room4)
    return gGaiaRockInside_Room4Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room5)
    return gGaiaRockInside_Room5Triggers;
  if (gMainRoutineWork.entrance == 9)
    return gGaiaRockInside_LairTriggers;
  if (gMainRoutineWork.entrance > 9)
    return gData_02005fcc;
  return gGaiaRockInside_Triggers;
}
#undef FIELD_AT

#undef FIELD_AT
#undef gData_0200530c
// ---- games/tla/src/scene/gaia_rock_inside/s2ac8.c ----
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

/* VIEW(s2ac8): routine Actor_GetById as s32 Actor_GetById(); event.h has struct
 * Actor *Actor_GetById(u32 objectId); tried as the definition's struct Actor
 * *Actor_GetById(u32 objectId), which the object already declares, it does not
 * compile in tla-ja: games/tla/src/scene/gaia_rock_inside.c:1989: too few
 * arguments to function `Actor_GetById'. */
extern s32 Actor_GetById__s2ac8() __asm__("Actor_GetById");

void Func_02002ac8(s32 a0, s32 a1) {
  s32 t2;
  s32 t0;

  t0 = Actor_GetById__s2ac8();
  Actor_FaceActorById(a0, a1, 0);
  if (t0 != 0) {
    t2 = ResourceMetadata_Register(FIELD_AT(t0, s32, 80), 0x185);
    FIELD_AT(t2, u8, 5) = 0;
    FIELD_AT(t2, u8, 6) = 0;
    Actor_SelectAnimation(t0, 0);
    Actor_SelectAnimation(t0, 4);
  }
}
#undef FIELD_AT

// ---- games/tla/src/scene/gaia_rock_inside/s2b04.c ----
void Func_02002b04(u32 id, s8 mode) {
  struct Actor *object = Actor_Get(id);

  object->motionFlags = 0;
  object->priorityFlags |= 0x80;
  object->acceleration = 0x1000;
  object->speedLimit = 0x10000;
  object->y = PIXELS(16);
  object->timer = object->x >> 16;
  object->unk66 = object->z >> 16;
  Actor_SetAnimation(id, mode);
}

// ---- games/tla/src/scene/gaia_rock_inside/s2b58.c ----
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

/* VIEW(s2b58): routine Actor_GetById as s32 Actor_GetById(); event.h has struct
 * Actor *Actor_GetById(u32 objectId); tried as the definition's struct Actor
 * *Actor_GetById(u32 objectId), which the object already declares, it does not
 * compile in tla-ja: games/tla/src/scene/gaia_rock_inside.c:2029: too few
 * arguments to function `Actor_GetById'. */
extern s32 Actor_GetById__s2b58() __asm__("Actor_GetById");

void Func_02002b58(s32 a0, s32 a1, s32 a2) {
  s32 t0;

  t0 = Actor_GetById__s2b58();
  Call4(Actor_SetMoveTarget, t0,
        (((a1 << 4) + FIELD_AT((t0 + 0x64), s16, 0)) << 16),
        FIELD_AT(t0, s32, 12),
        (((a2 << 4) + FIELD_AT((t0 + 0x66), s16, 0)) << 16));
}
#undef FIELD_AT

// ---- games/tla/src/scene/gaia_rock_inside/s2b8c.c ----
// The frame callback of the room where persons 8 to 16 sway: all of them
// stop while flag 0x106 is set or the event work holds them; otherwise, once
// per pending step, at frame 60 of every 240 they lean one way and at frame
// 180 they come back.

extern struct SwayState gSceneState;

void Func_02002b8c(void) {
  // Pointers that stay on one global for their whole life.
  struct SwayState *state = &gSceneState;
  u16 *pending = &state->pending;
  u16 *frame = &state->frame;
  struct EventWork *work = Ram_HeapSlots->eventWork;
  s32 stopped = Event_GetFlag(0x106);
  s32 id;

  if (work->unk166 != 0)
    stopped |= 1;
  id = 8;
  do {
    Actor_Get(id++)->movementState = stopped;
  } while (id <= 16);
  if (stopped != 0)
    return;
  ++*pending;
  if (work->unk156 != 0)
    return;
  if (*pending == 0)
    return;
step:
  switch (*frame) {
  case 60:
    Func_02002b58(8, 0, 3);
    Func_02002b58(9, 3, 0);
    Func_02002b58(10, 0, 3);
    Func_02002b58(11, 0, 3);
    Func_02002b58(12, 0, -3);
    Func_02002b58(13, 0, -3);
    Func_02002b58(14, 0, 3);
    Func_02002b58(15, 0, -3);
    Func_02002b58(16, 3, 0);
    break;
  case 180:
    id = 8;
    do {
      Func_02002b58(id, 0, 0);
      id++;
    } while (id <= 16);
    break;
  }
  if (++*frame == 240)
    *frame = 0;
  if (--*pending != 0)
    goto step;
}

// ---- scene/gaia_rock_inside/s2cb4.c ----

// Scene code of overlay 0x681, in ROM order.

// Starts the room where persons 8 to 16 sway: unless flag 265 is set, clears
// the sway state and places each person, then starts the sway's task.
/* VIEW(s2cb4): variable gSceneState as u16 gSceneState[];
 * games/tla/src/scene/gaia_rock_inside/s2b8c has struct SwayState gSceneState;
 * tried as the 3 declarations' extern struct SceneState gSceneState, it does
 * not compile in tla-ja: games/tla/src/scene/gaia_rock_inside.c:2109:
 * incompatible types in initialization. */
extern u16 gSceneState__s2cb4[] __asm__("gSceneState");
#define gSceneState gSceneState__s2cb4
/* VIEW(s2cb4): variable gData_0200533c as const s16 gData_0200533c[][2];
 * scene/gaia_rock_inside/s4d00 has u32 gData_0200533c[] = {; tried as the
 * definition's extern u32 gData_0200533c[], it does not compile in tla-ja:
 * games/tla/src/scene/gaia_rock_inside.c:2120: `gData_0200533c' undeclared
 * (first use in this function). */
extern const s16 gData_0200533c__s2cb4[][2] __asm__("gData_0200533c");
#define gData_0200533c gData_0200533c__s2cb4

// Starts the room where persons 8 to 16 sway: unless flag 265 is set, clears
// the sway state and places each person, then starts the sway's task.
void Func_02002cb4(u8 mode) {
  u16 *state = gSceneState;
  u16 *pending = state + 1;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    s32 i;
    s32 id;

    *state = 0;
    *pending = 0;
    Actor_Get(FieldParty_GetLeaderActor())->priorityFlags |= 0x40;
    for (i = 0, id = 8; id <= 16; i++, id++) {
      Actor_SetPosition(id, gData_0200533c[i][0] << 19,
                        gData_0200533c[i][1] << 19);
      Func_02002b04(id, mode);
    }
  }
  {
    s32 priority = 0x480;

    Task_AddFrame(Func_02002b8c, priority);
  }
}

// Opens the passages on each side of the hall by how many of its two pairs of
// flags (0x9bc and 0x9bd, 0x9be and 0x9bf) are set: both open the wide way, one
// a side way, and a single side opened alone also opens its connecting cells.
void Func_02002d30(void) {
  u8 a = GameFlag_Get(0x9bc);
  u8 third = GameFlag_Get(0x9be);
  s32 b = GameFlag_Get(0x9bd);
  s8 first = a;
  s8 sideThird = third;
  u8 sumLeft = b + first;
  s32 d = GameFlag_Get(0x9bf);
  u8 sumRight = sideThird + d;
  s8 left = sumLeft;
  s8 right = sumRight;

  if (left != 0) {
    if (left == 2) {
      MapGrid_CopyCellsLayered(74, 115, 7, 12, 24, 76);
      if (right == 0)
        return;
      if (right == 2)
        MapGrid_CopyCellsLayered(71, 110, 1, 3, 30, 85);
      else if (sideThird != 0)
        MapGrid_CopyCellsLayered(67, 110, 1, 3, 30, 85);
      else
        MapGrid_CopyCellsLayered(79, 110, 1, 3, 30, 85);
    } else if (first != 0) {
      MapGrid_CopyCellsLayered(66, 95, 7, 12, 24, 76);
      if (right == 0)
        return;
      if (right == 2)
        MapGrid_CopyCellsLayered(81, 110, 1, 3, 30, 85);
      else if (sideThird != 0)
        MapGrid_CopyCellsLayered(75, 110, 1, 3, 30, 85);
      else
        MapGrid_CopyCellsLayered(65, 110, 1, 3, 30, 85);
    } else {
      MapGrid_CopyCellsLayered(69, 82, 5, 12, 26, 76);
      if (right == 0)
        return;
      if (right == 2)
        MapGrid_CopyCellsLayered(69, 110, 1, 3, 30, 85);
      else if (sideThird != 0)
        MapGrid_CopyCellsLayered(73, 110, 1, 3, 30, 85);
      else
        MapGrid_CopyCellsLayered(77, 110, 1, 3, 30, 85);
    }
  }
  if (right == 0)
    return;
  if (right == 2) {
    MapGrid_CopyCellsLayered(83, 115, 6, 12, 31, 76);
    if (left == 0)
      MapGrid_CopyCellsLayered(82, 124, 1, 3, 30, 85);
  } else if (sideThird != 0) {
    MapGrid_CopyCellsLayered(76, 82, 4, 12, 31, 76);
    if (left == 0)
      MapGrid_CopyCellsLayered(75, 91, 1, 3, 30, 85);
  } else {
    MapGrid_CopyCellsLayered(82, 82, 6, 12, 31, 76);
    if (left == 0)
      MapGrid_CopyCellsLayered(81, 91, 1, 3, 30, 85);
  }
}

#if TLA_PATCH_1

#include "call.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "system/io_reg.h"
#include "field/field.h"

#define Test(flag) Event_GetFlag(flag)
#define C1(f, a) Call1((void (*)())f, a)
#define C2(f, a, b) Call2((void (*)())f, a, b)
#define C3(f, a, b, c) Call3((void (*)())f, a, b, c)
#define C4(f, a, b, c, d) Call4((void (*)())f, a, b, c, d)
#define C6(f, a, b, c, d, e, g) Call6((void (*)())f, a, b, c, d, e, g)

struct GaiaRockAnimation {
  u8 unk00[18];
  u16 angle;
};

struct GaiaRockInsideSceneWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

/* VIEW(s2cb4): variable gData_0200530c as s16 gData_0200530c[][2];
 * games/tla/src/scene/gaia_rock_inside/s1f68 has const s16 gData_0200530c[][2];
 * tried as the definition's extern DEFINE_G_DATA_02005244(gData_02005244,
 * Orbit_Update, Actor_AimAtWatched, Actor_StepToWatched)
 * DEFINE_G_DATA_02005264(gData_02005264, ..., it does not compile in tla-ja:
 * games/tla/src/scene/gaia_rock_inside.c:4291: conflicting types for
 * `gData_0200530c__s2cb4'. */
extern s16 gData_0200530c__s2cb4[][2] __asm__("gData_0200530c");
#define gData_0200530c gData_0200530c__s2cb4
/* VIEW(s2cb4): variable gData_02005360 as u8 gData_02005360[];
 * scene/gaia_rock_inside/s4d00 has u32 gData_02005360[] = {; tried as the
 * definition's extern u32 gData_02005360[], it does not compile in tla-ja:
 * games/tla/src/scene/gaia_rock_inside.c:4302: conflicting types for
 * `gData_02005360__s2cb4'. */
extern u8 gData_02005360__s2cb4[] __asm__("gData_02005360");
#define gData_02005360 gData_02005360__s2cb4
/* VIEW(s2cb4): variable gData_0200539c as u8 gData_0200539c[];
 * scene/gaia_rock_inside/s4d00 has u32 gData_0200539c[] = {; tried as the
 * definition's extern u32 gData_0200539c[], it does not compile in tla-ja:
 * games/tla/src/scene/gaia_rock_inside.c:4306: conflicting types for
 * `gData_0200539c__s2cb4'. */
extern u8 gData_0200539c__s2cb4[] __asm__("gData_0200539c");
#define gData_0200539c gData_0200539c__s2cb4
/* VIEW(s2cb4): variable gData_020053d8 as u8 gData_020053d8[];
 * scene/gaia_rock_inside/s4d00 has u32 gData_020053d8[] = {; tried as the
 * definition's extern u32 gData_020053d8[], it does not compile in tla-ja:
 * games/tla/src/scene/gaia_rock_inside.c:4310: conflicting types for
 * `gData_020053d8__s2cb4'. */
extern u8 gData_020053d8__s2cb4[] __asm__("gData_020053d8");
#define gData_020053d8 gData_020053d8__s2cb4
/* VIEW(s2cb4): variable gData_0200578c as u8 gData_0200578c[];
 * scene/gaia_rock_inside/s4d00 has s32 gData_0200578c[] = {; tried as the
 * definition's extern DEFINE_G_DATA_020056C4(gData_020056c4, 424, 344)
 * DEFINE_G_DATA_020056C4(gData_02005728, 512, 384) s32 gData_0200578c[], it
 * does not compile in tla-ja: games/tla/src/scene/gaia_rock_inside.c:4462:
 * conflicting types for `gData_0200578c__s2cb4'. */
extern u8 gData_0200578c__s2cb4[] __asm__("gData_0200578c");
#define gData_0200578c gData_0200578c__s2cb4

struct Actor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 kind);
s32 __modsi3(s32 numerator, s32 denominator);
void Func_02002ac8();
void Func_02001c9c(void);
void Func_02004a5c(s32 id, s32 x, s32 y, s32 z);
void Func_02004d00(void);
void Actor_SelectAnimation(struct Actor *object, s32 mode);
void BlendScript_Resume(void);
void BlendScript_Suspend(void);
void Actor_SetSpritePriorityDirect();
void Event_End(void);
void FieldEffect_StartBlankOverlay();
void HangingVine_Place();
void WorldMap_SetReturnLeavingScene();
void Actor_SetAnimationSpeed();
void Actor_SetSpriteColorMode();
void Event_FinishScreenTransition(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

// Room entry for the five Gaia Rock rooms, chosen by scene, then by entrance
// in the first room.
s32 Scene_Initialize(void) {
  s32 message;
  struct MainRoutineWork *work;
  /* FAKEMATCH(pin): room in r8, which leaves r7 to the owner pointer; as a
   * plain local, 82 of 1969 instructions differ in tla-en. */
  register struct MainRoutineWork *room asm("r8");
  s16 *entrance;
  s16 *early;

  work = &gMainRoutineWork;
  ((struct GaiaRockInsideSceneWork *)Ram_HeapSlots->eventWork)->screenEffect =
      516;
  if (work->scene == (s32)&SceneId_Ov681Room1) {
    early = &work->entrance;
    if (work->entrance > 8) {
      C2(Actor_SetSpriteFlags, Actor_GetById(8), 0);
      Func_02002d30();
    }
    if (*early == 20) {
      Event_Start();
      C1(Event_ResolvePendingActions, 0);
      C4(FieldCamera_MoveWithinMapBounds, -1, -1, -1, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, work->currentOwner, 0, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 10, 0, 0);
      C1(Actor_Stop, 9);
      C1(Actor_Stop, 10);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      C1(Event_Wait, 60);
      {
        s32 ending = 20;
        if (Test(0x307)) {
          C1(GameFlag_Clear, 0x307);
          C1(GameFlag_Set, 0x9bc);
          ending = 21;
        }
        if (Test(0x308)) {
          C1(GameFlag_Clear, 0x308);
          C1(GameFlag_Set, 0x9bd);
          ending = 22;
        }
        if (Test(0x309)) {
          C1(GameFlag_Clear, 0x309);
          C1(GameFlag_Set, 0x9be);
          ending = 23;
        }
        if (Test(0x30a)) {
          C1(GameFlag_Clear, 0x30a);
          C1(GameFlag_Set, 0x9bf);
          ending = 24;
        }
        C1(Sound_Play, 564);
        Func_02002d30();
        C1(Event_Wait, 15);
        Actor_SetModeById(8, 3);
        C1(Sound_Play, 530);
        C3(MapGrid_SetScreenShake, 0x20000, 0x20000, 0x10000);
        C3(MapGrid_SetScreenShake, -1, -1, 0xe666);
        C1(Event_Wait, 120);
        Actor_SetModeById(8, 1);
        C1(Event_Wait, 60);
        C1(GameFlag_Set, 0x109);
        C1(Import_EventSetExitRequest, ending);
      }
    }
    if ((u16)(gMainRoutineWork.entrance - 9) <= 1) {
      C1(GameFlag_Set, 0x144);
      if (Test(FLAG_SERPENT_SLAIN)) {
        C2(Actor_SetModeById, 8, 4);
        C6(MapGrid_CopyCellPictureCellsRect, 85, 72, 5, 2, 30, 19);
        C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
        C3(Actor_SetHorizontalPositionWithTerrain, 10, 0, 0);
      }
      if (Test(0x9ee) && !Test(0x9ba))
        C3(Actor_SetHorizontalPositionWithTerrain, 12, 0x1d00000, 0x1480000);
    }
    room = &gMainRoutineWork;
    entrance = &room->entrance;
    if (*entrance == 9) {
      s32 none;
      s32 found;

      if (Test(FLAG_SERPENT_SLAIN))
        goto end;
      C1(Sound_Play, 53);
      found = FieldPartyInventory_FindMember(451);
      none = -1;
      if (found != none) {
        s32 flag;
        struct Actor *dragon;
        struct Actor *partner;
        s32 width;
        s32 height;
        s32 mask;
        s32 fast;
        s32 i;
        s32 dx;
        s32 dz;
        s32 movement;
        struct Actor *object;
        struct Actor *shards[4];

        flag = Test(0x9ac);
        if (flag != 0) {
          C2(Actor_SetModeById, 8, 2);
          C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
          C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1b00000, 0x1400000);
          C2(Actor_SetSpritePriority, 10, 3);
          C6(Map_CopyCellAttributeRect, 30, 19, 1, 1, 27, 19);
          C2(Actor_SetModeById, 10, 5);
          goto end;
        }
        dragon = Actor_GetById(10);
        partner = Actor_GetById(8);
        C1(Sound_Play, 390);
        C1(GameFlag_Set, 0x9ac);
        C1(GameFlag_Set, 0x9de);
        Event_Start();
        C1(Event_ResolvePendingActions, 0);
        C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
        C2(Actor_SetMovementTypeById, 9, 1);
        C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1c80000, 0x1680000);
        C2(Actor_SetMovementTypeById, 10, 1);
        C3(Actor_ArmCallback, 10, 0xe000, 0);
        C2(Actor_SetModeById, 8, 2);
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        C4(FieldCamera_MoveWithinMapBounds, 0x1e80000, none, 0x1800000, 1);
        FieldCamera_WaitForArrival();
        C3(Actor_ShowEmoteById, 10, 264, 0);
        C1(Event_Wait, 40);
        C1(Event_SetMessageId, (s32)MsgFieldAtLastTheSerpentHas);
        C2(Event_ShowMessageAndWait, 10, 0);
        C2(Motion_SetModeAndWaitAnimation, 10, 3);
        C2(Event_ShowMessageAndWait, 10, 0);
        C2(Actor_SetModeById, 10, 8);
        C2(Event_ShowMessageAndWait, 0x600a, 0);
        dragon->speedLimit = 0x18000;
        C3(Actor_SetDestinationById, 10, 488, 328);
        C1(Event_Wait, 5);
        Actor_SetModeById(10, 6);
        C1(Event_Wait, 10);
        dragon->velocityY = 0x60000;
        C1(Event_Wait, 10);
        dragon->movementState = 1;
        partner->movementState = 1;
        C2(Actor_SetAnimationSpeed, dragon, 0);
        C2(Actor_SetAnimationSpeed, partner, 0);
        C2(Actor_SetSpriteColorMode, dragon, 6);
        C2(Actor_SetSpriteColorMode, partner, 6);
        width = 30;
        height = 19;
        C6(MapGrid_CopyCellPictureCellsRect, 85, 76, 5, 2, width, height);
        C1(Sound_Play, 572);
        C2(BattleEffect_ApplyColorToTargetBuffer, 0x7fff, 0);
        C1(PaletteFade_StartInterpolation, 10);
        C1(Event_Wait, 10);
        C2(BattleEffect_ApplyColorToTargetBuffer, 0x10000, 0);
        C1(PaletteFade_StartInterpolation, 10);
        C1(Event_Wait, 10);
        C1(Event_Wait, 60);
        C2(Actor_SetSpriteColorMode, dragon, 0);
        C2(Actor_SetSpriteColorMode, partner, 0);
        C6(MapGrid_CopyCellPictureCellsRect, 85, 74, 5, 2, width, height);
        dragon->movementState = 0;
        partner->movementState = 0;
        C2(Actor_SetAnimationSpeed, dragon, 16);
        C2(Actor_SetAnimationSpeed, partner, 16);
        mask = 254;
        Actor_GetById(10)->actionFlags &= mask;
        C3(Actor_WalkToById, 10, 456, 360);
        fast = 0x40000;
        dragon->velocityY = fast;
        C2(FieldEffect_SetAttached, 10, 258);
        C1(Actor_WaitForMoveById, 10);
        C3(Actor_ShowEmoteById, 10, 257, 20);
        C1(Event_Wait, 30);
        Actor_SetModeById(10, 8);
        C1(Event_Wait, 10);
        dragon->speedLimit = 0x18000;
        C3(Actor_SetDestinationById, 10, 488, 328);
        C1(Event_Wait, 14);
        dragon->velocityY = 0x60000;
        Actor_SetModeById(10, 6);
        C1(Event_Wait, 10);
        C1(Sound_Play, 572);
        {
          struct Actor *a = Actor_GetById(10);
          mask &= a->actionFlags;
          a->actionFlags = mask;
        }
        C3(Actor_WalkToById, 10, 456, 360);
        dragon->velocityY = fast;
        C2(FieldEffect_SetAttached, 10, 258);
        C1(Actor_WaitForMoveById, 10);
        C2(Event_ShowMessageAndWait, 10, 0);
        C1(Event_Wait, 15);
        C2(Actor_SetAnimationSpeed, partner, 0);
        C1(Event_Wait, 15);
        C1(Sound_Play, 215);
        object = FieldActor_Create(285, 0x1e80000, -0x100000, 0x1590000);
        C2(Actor_SetSpriteFlags, object, 0);
        C2(Actor_StartScript, object, gData_0200539c);
        C2(Actor_SelectAnimation, object, 1);
        C2(Actor_SetSpriteColorMode, object, 6);
        C3(Actor_Jump, 10, 2, 0);
        C1(Event_Wait, 5);
        C3(Actor_ShowEmoteById, 10, 258, 0);
        C1(Event_Wait, 55);
        C1(Event_Wait, 5);
        C1(Sound_Play, 136);
        object = FieldActor_Create(412, 0x1e80000, -0x100000, 0x1590000);
        C2(Actor_SetSpriteFlags, object, 0);
        C2(Actor_StartScript, object, gData_02005360);
        ((struct GaiaRockAnimation *)object->sprite)->angle = 0x2000;
        object->speedLimit = 0x18000;
        *(u8 *)&object->motionFlags = 0;
        object->targetX = 0x1c80000;
        object->targetY = -0x200000;
        object->targetZ = 0x1660000;
        object->scaleX = 0x6000;
        object->scaleY = 0x6000;
        C1(Event_Wait, 10);
        C2(Actor_SetAnimationSpeed, partner, 16);
        C1(Event_Wait, 10);
        C1(Sound_Play, 133);
        object->x = 0;
        object->z = 0;
        C3(MapGrid_SetScreenShake, fast, fast, 0x10000);
        for (i = 0; i < 4; i++) {
          dx = -8;
          if (i > 1)
            dx = 8;
          dz = -8;
          if ((i & 1) == 0)
            dz = 8;
          shards[i] = (struct Actor *)Import_ActorCreate(
              403, 0x1c80000 + (dx << 16), -0x200000, 0x1660000 + (dz << 16));
          C2(Actor_SelectAnimation, shards[i], 2);
          C2(Actor_SetSpriteFlags, shards[i], 0);
          C2(Actor_StartScript, shards[i], gData_0200539c);
        }
        dragon->velocityY = 0xa0000;
        Actor_GetById(10)->actionFlags &= 254;
        C3(Actor_WalkToById, 10, 432, 320);
        C1(Event_Wait, 10);
        C3(MapGrid_SetScreenShake, -1, -1, 0xe666);
        Actor_SetModeById(10, 5);
        C1(Event_Wait, 30);
        C2(FieldCamera_MoveToActorById, 10, 1);
        FieldCamera_WaitForArrival();
        movement = (s32)gData_02005e28;
        C2(Actor_SetMovementTypeById, 10, movement);
        C1(Actor_RefreshSelectorById, 10);
        C1(Event_Wait, 20);
        C2(Actor_SetMovementTypeById, 10, movement);
        C1(Event_Wait, 20);
        C2(Event_ShowMessageAndWait, 0x600a, 0);
        C2(FieldCamera_FollowActor, gMainRoutineWork.currentOwner, 1);
        FieldCamera_WaitForArrival();
        C2(Actor_SetSpritePriority, 10, 3);
        C6(Map_CopyCellAttributeRect, 30, 19, 1, 1, 27, 19);
        Event_End();
        goto end;
      } else {
        if (Test(0x109))
          goto end;
        C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1d80000, 0x1c80000);
        C2(Actor_SetMovementTypeById, 10, gData_0200578c);
        goto end;
      }
    } else if (*entrance == 10) {
      s32 flag;
      s32 scale;
      struct Actor *dragon;
      s32 slow;
      s32 *owner;
      struct Actor *other;
      struct Actor *object;
      volatile u16 alpha;
      s32 i;

      C1(Sound_Play, 7);
      flag = Test(FLAG_SERPENT_SLAIN);
      if (flag != 0)
        goto end;
      Event_Start();
      C1(Event_ResolvePendingActions, 0);
      C1(GameFlag_Set, FLAG_SERPENT_SLAIN);
      C1(Event_SetMessageId, (s32)MsgFieldItLooksLikeTheDragon);
      C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
      Actor_SetModeById(8, 3);
      Actor_GetById(11)->speedLimit = scale = 0x10000;
      slow = 0x8000;
      dragon = Actor_GetById(10);
      dragon->speedLimit = slow;
      C3(Actor_SetHorizontalPositionWithTerrain, 11, 0x1b80000, 0x1380000);
      owner = &room->currentOwner;
      C3(Actor_SetHorizontalPositionWithTerrain, *owner, 0x1c80000, 0x1480000);
      C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1e80000, 0x1a80000);
      C3(Actor_ArmCallback, *owner, 0, 0);
      C3(Actor_ArmCallback, 11, 0, 0);
      C3(Actor_ArmCallback, 10, 0xc000, 0);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      C3(Actor_ShowEmoteById, 11, 258, 30);
      C1(Sound_Play, 530);
      C2(Event_ShowMessageAndWait, 11, 0);
      C1(Sound_Play, 530);
      C2(Event_ShowMessageAndWait, 10, 0);
      Actor_ShowEmote(*owner, 256, 0);
      C3(Actor_ShowEmoteById, 11, 256, 0);
      C1(Event_Wait, 15);
      C3(Actor_ArmCallback, *owner, 0x4000, 0);
      C3(Actor_ArmCallback, 11, 0x4000, 0);
      C1(Event_Wait, 30);
      C2(FieldCamera_MoveToActorById, 10, 1);
      FieldCamera_WaitForArrival();
      C2(Actor_StartRepeatedMotion, 10, 2);
      C2(FieldCamera_FollowActor, 10, 1);
      C3(Actor_WalkToById, 10, 488, 376);
      C1(Actor_WaitForMoveById, 10);
      C2(Actor_StartRepeatedMotion, 10, 2);
      C1(Event_Wait, 20);
      C3(Actor_WalkToById, 10, 472, 360);
      C1(Actor_WaitForMoveById, 10);
      C3(Actor_FaceActorById, *owner, 10, 0);
      C3(Actor_FaceActorById, 11, 10, 0);
      C3(Actor_FaceActorById, 10, 8, 0);
      C4(FieldCamera_MoveWithinMapBounds, -1, -1, -1, 0);
      C2(Actor_SetModeById, 10, 8);
      C2(Event_ShowMessageAndWait, 10, 0);
      dragon->speedLimit = 0x18000;
      C3(Actor_SetDestinationById, 10, 488, 328);
      C1(Event_Wait, 5);
      Actor_SetModeById(10, 6);
      C1(Event_Wait, 10);
      dragon->velocityY = 0x60000;
      C1(Event_Wait, 10);
      C1(Sound_Play, 572);
      dragon->movementState = 1;
      C2(BattleEffect_ApplyColorToTargetBuffer, 0x7fff, 0);
      C1(PaletteFade_StartInterpolation, 10);
      C1(Event_Wait, 10);
      C1(Sound_Play, 530);
      C2(BattleEffect_ApplyColorToTargetBuffer, scale, 0);
      C1(PaletteFade_StartInterpolation, 10);
      C1(Event_Wait, 10);
      C2(BattleEffect_ApplyColorToTargetBuffer, 0x7fff, 0);
      C1(PaletteFade_StartInterpolation, 10);
      C1(Event_Wait, 10);
      C2(BattleEffect_ApplyColorToTargetBuffer, scale, 0);
      C1(PaletteFade_StartInterpolation, 10);
      C1(Event_Wait, 10);
      C2(BattleEffect_ApplyColorToTargetBuffer, 0x7fff, 0);
      C1(PaletteFade_StartInterpolation, 10);
      C1(Event_Wait, 60);
      C2(Actor_SetModeById, 8, 4);
      C6(MapGrid_CopyCellPictureCellsRect, 85, 72, 5, 2, 30, 19);
      dragon->y = -0x200000;
      dragon->velocityY = 0;
      dragon->speedLimit = slow;
      C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1c80000, 0x1680000);
      C2(Actor_SetModeById, 10, 1);
      C2(BattleEffect_ApplyColorToTargetBuffer, scale, 0);
      C1(PaletteFade_StartInterpolation, 10);
      C1(Event_Wait, 10);
      C3(Actor_FaceActorById, 10, *owner, 0);
      C2(Event_ShowMessageAndWait, 10, 0);
      C2(Motion_SetModeAndWaitAnimation, 10, 3);
      C2(Event_ShowMessageAndWait, 10, 0);
      C3(Actor_ArmCallback, 10, 0x4000, 0);
      C2(Actor_SetModeById, 10, 2);
      C3(Actor_WalkToById, 10, 488, 392);
      C2(Actor_StartRepeatedMotion, 11, 1);
      C1(Event_Wait, 5);
      C2(Event_ShowMessageAndWait, 11, 0);
      C1(Actor_WaitForMoveById, 10);
      C2(Actor_SetModeById, 10, 0);
      C3(Actor_FaceActorById, 10, 11, 0);
      C1(Event_Wait, 15);
      C2(Event_ShowMessageAndWait, 10, 0);
      C2(Motion_SetModeAndWaitAnimation, 10, 3);
      C3(Actor_ArmCallback, 10, 0x4000, 0);
      C2(Actor_SetModeById, 10, 2);
      C3(Actor_WalkToById, 10, 488, 536);
      C2(FieldCamera_FollowActor, *owner, 1);
      C3(Actor_FaceActorById, *owner, 11, 0);
      C3(Actor_FaceActorById, 11, *owner, 0);
      C1(Event_Wait, 30);
      C2(Motion_SetModeAndWaitAnimation, 11, 3);
      C2(Actor_SetModeById, 11, 2);
      {
        struct Actor *ownerActor = Actor_GetById(*owner);

        if (ownerActor != NULL)
          Actor_SetDestinationById(11, ownerActor->x >> 16,
                                   ownerActor->z >> 16);
      }
      C1(Actor_WaitForMoveById, 11);
      C3(Actor_SetHorizontalPositionWithTerrain, 11, 0, 0);
      C1(Actor_WaitForMoveById, 11);
      C3(Actor_SetHorizontalPositionWithTerrain, 10, 0, 0);
      C3(Actor_WalkToById, *owner, 488, 376);
      C1(Actor_WaitForMoveById, *owner);
      C3(MapGrid_SetScreenShake, 0x50000, 0x50000, scale);
      C1(Sound_Play, 107);
      C1(Event_Wait, 30);
      C3(Actor_ShowEmoteById, *owner, 256, 0);
      C1(Event_Wait, 15);
      C3(Actor_ArmCallback, *owner, 0xc000, 0);
      C1(Event_Wait, 15);
      other = Actor_GetById(12);
      alpha = REG_BLDALPHA;
      C3(MapGrid_SetScreenShake, 0xa0000, 0xa0000, scale);
      C1(Sound_Play, 107);
      BlendScript_Suspend();
      alpha = *(u8 *)&alpha;
      while (alpha != 0) {
        REG_BLDALPHA = alpha | 0x1000;
        alpha = alpha + 0xffff;
        C1(wait, 1);
      }
      REG_DISPCNT &= 0xfdff;
      C1(Event_Wait, 30);
      {
        s32 x;
        /* FAKEMATCH(pin): z in r5 and x in r6; as a plain local, 12 of 1969
         * instructions differ in tla-en. */
        register s32 z asm("r5");

        x = 0x1d00000;
        z = 0x15c0000;
        object = OverlayObject_PrepareObject(x, -0x200000, z, 323);
        Actor_SetHorizontalPositionWithTerrain(12, x, z);
      }
      for (i = 0; i < 20; i++) {
        if (i % 5 == 0)
          Func_02001c9c();
        other->z += -0x10000;
        C1(Event_Wait, 2);
      }
      C1(GameFlag_Set, 0x9ee);
      C3(MapGrid_SetScreenShake, -1, -1, 0xe666);
      object->x = 0;
      object->z = 0;
      C1(Event_Wait, 1);
      C1(Actor_Release, object);
      C1(Sound_Play, 390);
      C1(Event_Wait, 30);
      REG_DISPCNT |= 0xf00;
      REG_BLDALPHA = alpha | 0x1000;
      REG_BLDCNT = 0x3f42;
      while (alpha != 8) {
        REG_BLDALPHA = alpha | 0x1000;
        alpha++;
        C1(wait, 1);
      }
      BlendScript_Resume();
      Event_End();
    } else if (*entrance == 11) {
      Event_Start();
      C1(Event_ResolvePendingActions, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1c00000, 0x1580000);
      {
        s32 *lead = &room->currentOwner;

        C3(Actor_FaceActorById, 10, *lead, 0);
        Value2((s32 (*)())Actor_SetModeById, *lead, 19);
      }
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      message = (s32)MsgFieldYouYouCannotFightThe;
      goto leave;
    } else if (*entrance == 12) {
      Event_Start();
      C1(Event_ResolvePendingActions, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1ae0000, 0x1400000);
      C2(Actor_SetModeById, 10, 5);
      C2(Actor_SetModeById, room->currentOwner, 19);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      C1(Event_Wait, 30);
      Value2((s32 (*)())Actor_StartRepeatedMotion, 10, 2);
      C1(Event_Wait, 30);
      message = (s32)MsgFieldLikeISaidTheSerpent;
    leave:
      C1(Event_SetMessageId, message);
      Event_ShowMessageAndWait(10, 0);
      Event_FinishScreenTransition();
      Event_WaitTransitionFrames();
      C2(Event_SetDestination, (s16)room->savedScene, (s16)room->savedEntrance);
      C2(WorldMap_SetReturnLeavingScene, room->scene, *entrance);
      Event_End();
    }
  } else if (work->scene == (s32)&SceneId_Ov681Room2) {
    s32 mask = 32;
    s32 low;
    s32 high;
    struct Actor *object = Actor_Get(8);

    object->priorityFlags |= mask;
    C2(Actor_SetSpritePriorityDirect, object, 3);
    object = Actor_Get(9);
    object->priorityFlags |= mask;
    C2(Actor_SetSpritePriorityDirect, object, 3);
    C2(Func_02002ac8, 8, 12);
    C2(Func_02002ac8, 9, 12);
    if (Test(0xa62)) {
      C3(Actor_SetHorizontalPositionWithTerrain, 8, 0x1b80000, 0x2b00000);
      C3(Actor_SetHorizontalPositionWithTerrain, 9, 0x2280000, 0x2b00000);
    } else {
      s32 height = 40;

      C6(Map_CopyCellsRect, 88, 40, 1, 2, 89, height);
      C6(Map_CopyCellsRect, 99, 40, 1, 2, 100, height);
    }
    low = FieldGameFlag_GetByte(0x210) & 15;
    high = (u8)(FieldGameFlag_GetByte(0x210) >> 4);
    if (low != 0)
      C3(Actor_SetHorizontalPositionWithTerrain, 10,
         gData_0200530c[low][0] << 19, gData_0200530c[low][1] << 19);
    if ((s8)high != 0)
      C3(Actor_SetHorizontalPositionWithTerrain, 11,
         gData_0200530c[(s8)high][0] << 19, gData_0200530c[(s8)high][1] << 19);
  } else if (work->scene == (s32)&SceneId_Ov681Room4) {
    if (Test(0xa63))
      C1(Func_02002cb4, 1);
  } else if (work->scene == (s32)&SceneId_Ov681Room3) {
    struct Actor *object;
    struct Actor *other;

    object = Actor_GetById(10);
    C2(Actor_SetSpritePriorityDirect, object, 3);
    C2(Func_02002ac8, 8, 10);
    C2(Func_02002ac8, 9, 11);
    if (!Test(0x9bc))
      C6(Map_CopyCellsRect, 25, 77, 1, 15, 26, 77);
    if (Test(0x9bd)) {
      s32 width;
      s32 height;

      C3(Actor_SetHorizontalPositionWithTerrain, 8, 0x1e00000, 0x1600000);
      width = 28;
      C6(Map_CopyCellsRect, 2, 93, 1, 5, width, 85);
      height = 22;
      C6(Map_CopyCellAttributeRect, 27, 23, 1, 1, width, height);
      C6(Map_CopyCellAttributeRect, 32, 16, 2, 1, 29, height);
    }
    if (Test(0x9be)) {
      s32 width;
      s32 height;

      C3(Actor_SetHorizontalPositionWithTerrain, 9, 0x2600000, 0x1600000);
      width = 36;
      C6(Map_CopyCellsRect, 2, 93, 1, 5, width, 85);
      height = 22;
      C6(Map_CopyCellAttributeRect, 35, 23, 1, 1, width, height);
      C6(Map_CopyCellAttributeRect, 32, 16, 2, 1, 37, height);
    }
    if (Test(0x9bf)) {
      C6(MapGrid_CopyCellPictureCellsRect, 61, 0, 3, 3, 37, 26);
      C6(Map_CopyCellAttributeRect, 36, 23, 1, 1, 38, 25);
    }
    if (work->entrance == 6) {
      s32 frames = 60;

      Event_Start();
      C1(Event_ResolvePendingActions, 0);
      C4(FieldCamera_MoveWithinMapBounds, -1, -1, -1, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 11, 0, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, work->currentOwner, 0, 0);
      Event_StartScreenTransition();
      C1(Event_Wait, 15);
      C3(Actor_SetHorizontalPositionWithTerrain, 11, 0x2a00000, 0x1800000);
      C2(Actor_SetSpriteFlags, Actor_GetById(11), 0);
      object = Actor_GetById(11);
      other = FieldActor_Create(324, object->x, object->y, object->z);
      if (other != NULL) {
        C2(Actor_SetSpriteFlags, other, 0);
        C2(Actor_StartScript, other, gData_020053d8);
        C2(Actor_SelectAnimation, other, 1);
        C2(Actor_SetSpriteColorMode, other, 14);
        ((u8 *)other->sprite)[9] |= 12;
        other->scaleX = 0x13333;
        other->scaleY = 0x13333;
      }
      object->y += 0x800000;
      for (; frames != 0; frames--) {
        C1(wait, 1);
        if (object->velocityY > -1)
          break;
      }
      C1(Sound_Play, 188);
      C1(Event_Wait, 30);
      C1(GameFlag_Set, 0x109);
      C1(Import_EventSetExitRequest, 3);
    }
    if (!Test(0x9c4))
      C3(Actor_SetHorizontalPositionWithTerrain, 11, 0, 0);
    if (!Test(0x109) && gMainRoutineWork.entrance == 3)
      Func_0200042c();
  } else if (work->scene == (s32)&SceneId_Ov681Room5) {
    C2(HangingVine_Place, 10, 1);
    C2(Func_02002ac8, 8, 9);
    if (work->entrance == 1 || work->entrance == 3 || work->entrance == 5)
      Func_02004d00();
    if (Test(0x303))
      Func_02004a5c(11, 0x1800000, 0x4b0000, 0x2a80000);
    if (Test(0x304))
      Func_02004a5c(12, 0x1cf0000, 0x330000, 0x2a80000);
    if (Test(0x305))
      Func_02004a5c(13, 0x2350000, 0x370000, 0x2a80000);
    if (Test(0x9bc)) {
      s32 width;
      s32 height;

      C3(Actor_SetHorizontalPositionWithTerrain, 8, 0x1800000, 0x1700000);
      width = 25;
      C6(Map_CopyCellsRect, 1, 85, 1, 4, width, 85);
      height = 23;
      C6(Map_CopyCellAttributeRect, 27, 15, 2, 1, height, height);
      C6(Map_CopyCellAttributeRect, 25, 25, 1, 1, width, height);
    }
    C2(Actor_SetSpriteFlags, Actor_GetById(15), 0);
    if (Test(0x9c4)) {
      s32 height = 20;

      C6(MapGrid_CopyCellPictureCellsRect, 117, 20, 7, 8, 104, height);
      C6(MapGrid_CopyCellPictureCellsRect, 111, 12, 1, 6, 111, height);
      C6(Map_CopyCellAttributeRect, 49, 84, 8, 8, 40, 84);
      C3(Actor_SetHorizontalPositionWithTerrain, 15, 0, 0);
    }
  }
end:
  C1(FieldEffect_StartBlankOverlay, 0);
  return 0;
}

#else

// The Japanese release's older form: room 1's entrances 11 and 12 end without
// WorldMap_SetReturnLeavingScene, room 3 has no flag 0x109 check calling
// Func_0200042c, and the room pointer is a plain local.

#include "call.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "system/io_reg.h"
#include "field/field.h"

#define Test(flag) Event_GetFlag(flag)
#define C1(f, a) Call1((void (*)())f, a)
#define C2(f, a, b) Call2((void (*)())f, a, b)
#define C3(f, a, b, c) Call3((void (*)())f, a, b, c)
#define C4(f, a, b, c, d) Call4((void (*)())f, a, b, c, d)
#define C6(f, a, b, c, d, e, g) Call6((void (*)())f, a, b, c, d, e, g)

struct GaiaRockAnimation {
  u8 unk00[18];
  u16 angle;
};

struct GaiaRockInsideSceneWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

extern char SceneId_Ov681Room1;
extern char SceneId_Ov681Room2;
extern char SceneId_Ov681Room3;
extern char SceneId_Ov681Room4;
extern char SceneId_Ov681Room5;
/* VIEW(s2cb4): variable gData_0200530c as s16 gData_0200530c[][2];
 * scene/gaia_rock_inside/s4d00 has u16 gData_0200530c[] = {; tried as the
 * definition's extern DEFINE_G_DATA_02005244(gData_02005244, Orbit_Update,
 * Actor_AimAtWatched, Actor_StepToWatched)
 * DEFINE_G_DATA_02005264(gData_02005264, ..., it does not compile in tla-ja:
 * games/tla/src/scene/gaia_rock_inside.c:3509: `gData_0200530c' undeclared
 * (first use in this function). */
extern s16 gData_0200530c__s2cb4[][2] __asm__("gData_0200530c");
#define gData_0200530c gData_0200530c__s2cb4
/* VIEW(s2cb4): variable gData_02005360 as u8 gData_02005360[];
 * scene/gaia_rock_inside/s4d00 has u32 gData_02005360[] = {; tried as the
 * definition's extern u32 gData_02005360[], it does not compile in tla-ja:
 * games/tla/src/scene/gaia_rock_inside.c:3198: `gData_02005360' undeclared
 * (first use in this function). */
extern u8 gData_02005360__s2cb4[] __asm__("gData_02005360");
#define gData_02005360 gData_02005360__s2cb4
/* VIEW(s2cb4): variable gData_0200539c as u8 gData_0200539c[];
 * scene/gaia_rock_inside/s4d00 has u32 gData_0200539c[] = {; tried as the
 * definition's extern u32 gData_0200539c[], it does not compile in tla-ja:
 * games/tla/src/scene/gaia_rock_inside.c:3187: `gData_0200539c' undeclared
 * (first use in this function). */
extern u8 gData_0200539c__s2cb4[] __asm__("gData_0200539c");
#define gData_0200539c gData_0200539c__s2cb4
/* VIEW(s2cb4): variable gData_020053d8 as u8 gData_020053d8[];
 * scene/gaia_rock_inside/s4d00 has u32 gData_020053d8[] = {; tried as the
 * definition's extern u32 gData_020053d8[], it does not compile in tla-ja:
 * games/tla/src/scene/gaia_rock_inside.c:3568: `gData_020053d8' undeclared
 * (first use in this function). */
extern u8 gData_020053d8__s2cb4[] __asm__("gData_020053d8");
#define gData_020053d8 gData_020053d8__s2cb4
/* VIEW(s2cb4): variable gData_0200578c as u8 gData_0200578c[];
 * scene/gaia_rock_inside/s4d00 has s32 gData_0200578c[] = {; tried as the
 * definition's extern DEFINE_G_DATA_020056C4(gData_020056c4, 424, 344)
 * DEFINE_G_DATA_020056C4(gData_02005728, 512, 384) s32 gData_0200578c[], it
 * does not compile in tla-ja: games/tla/src/scene/gaia_rock_inside.c:3253:
 * `gData_0200578c' undeclared (first use in this function). */
extern u8 gData_0200578c__s2cb4[] __asm__("gData_0200578c");
#define gData_0200578c gData_0200578c__s2cb4

struct Actor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 kind);
s32 __modsi3(s32 numerator, s32 denominator);
void Func_02002ac8();
void Func_02001c9c(void);
void Func_02004a5c(s32 id, s32 x, s32 y, s32 z);
void Func_02004d00(void);
void Actor_SelectAnimation(struct Actor *object, s32 mode);
void Actor_Release();
void BlendScript_Resume(void);
void BlendScript_Suspend(void);
void Actor_SetSpritePriorityDirect();
void Event_Start(void);
void Event_End(void);
void FieldCamera_WaitForArrival(void);
void FieldCamera_MoveToActorById();
void Event_SetDestination();
void FieldEffect_StartBlankOverlay();
void HangingVine_Place();
void WorldMap_SetReturnLeavingScene();
void Actor_SetAnimationSpeed();
void Actor_SetSpriteColorMode();
void Event_FinishScreenTransition(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

// Room entry for the five Gaia Rock rooms, chosen by scene, then by entrance
// in the first room.
s32 Scene_Initialize(void) {
  struct MainRoutineWork *room;
  struct MainRoutineWork *work;
  s16 *entrance;
  s16 *early;

  work = &gMainRoutineWork;
  ((struct GaiaRockInsideSceneWork *)Ram_HeapSlots->eventWork)->screenEffect =
      516;
  if (work->scene == (s32)&SceneId_Ov681Room1) {
    early = &work->entrance;
    if (work->entrance > 8) {
      C2(Actor_SetSpriteFlags, Actor_GetById(8), 0);
      Func_02002d30();
    }
    if (*early == 20) {
      Event_Start();
      C1(Event_ResolvePendingActions, 0);
      C4(FieldCamera_MoveWithinMapBounds, -1, -1, -1, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, work->currentOwner, 0, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 10, 0, 0);
      C1(Actor_Stop, 9);
      C1(Actor_Stop, 10);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      C1(Event_Wait, 60);
      {
        s32 ending = 20;
        if (Test(0x307)) {
          C1(GameFlag_Clear, 0x307);
          C1(GameFlag_Set, 0x9bc);
          ending = 21;
        }
        if (Test(0x308)) {
          C1(GameFlag_Clear, 0x308);
          C1(GameFlag_Set, 0x9bd);
          ending = 22;
        }
        if (Test(0x309)) {
          C1(GameFlag_Clear, 0x309);
          C1(GameFlag_Set, 0x9be);
          ending = 23;
        }
        if (Test(0x30a)) {
          C1(GameFlag_Clear, 0x30a);
          C1(GameFlag_Set, 0x9bf);
          ending = 24;
        }
        C1(Sound_Play, 564);
        Func_02002d30();
        C1(Event_Wait, 15);
        Actor_SetModeById(8, 3);
        C1(Sound_Play, 530);
        C3(MapGrid_SetScreenShake, 0x20000, 0x20000, 0x10000);
        C3(MapGrid_SetScreenShake, -1, -1, 0xe666);
        C1(Event_Wait, 120);
        Actor_SetModeById(8, 1);
        C1(Event_Wait, 60);
        C1(GameFlag_Set, 0x109);
        C1(Import_EventSetExitRequest, ending);
      }
    }
    if ((u16)(gMainRoutineWork.entrance - 9) <= 1) {
      C1(GameFlag_Set, 0x144);
      if (Test(FLAG_SERPENT_SLAIN)) {
        C2(Actor_SetModeById, 8, 4);
        C6(MapGrid_CopyCellPictureCellsRect, 85, 72, 5, 2, 30, 19);
        C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
        C3(Actor_SetHorizontalPositionWithTerrain, 10, 0, 0);
      }
      if (Test(0x9ee) && !Test(0x9ba))
        C3(Actor_SetHorizontalPositionWithTerrain, 12, 0x1d00000, 0x1480000);
    }
    room = &gMainRoutineWork;
    entrance = &room->entrance;
    if (*entrance == 9) {
      s32 none;
      s32 found;

      if (Test(FLAG_SERPENT_SLAIN))
        goto end;
      C1(Sound_Play, 53);
      found = FieldPartyInventory_FindMember(451);
      none = -1;
      if (found != none) {
        s32 flag;
        struct Actor *dragon;
        struct Actor *partner;
        s32 width;
        s32 height;
        s32 mask;
        s32 fast;
        s32 i;
        s32 dx;
        s32 dz;
        s32 movement;
        struct Actor *object;
        struct Actor *shards[4];

        flag = Test(0x9ac);
        if (flag != 0) {
          C2(Actor_SetModeById, 8, 2);
          C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
          C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1b00000, 0x1400000);
          C2(Actor_SetSpritePriority, 10, 3);
          C6(Map_CopyCellAttributeRect, 30, 19, 1, 1, 27, 19);
          C2(Actor_SetModeById, 10, 5);
          goto end;
        }
        dragon = Actor_GetById(10);
        partner = Actor_GetById(8);
        C1(Sound_Play, 390);
        C1(GameFlag_Set, 0x9ac);
        C1(GameFlag_Set, 0x9de);
        Event_Start();
        C1(Event_ResolvePendingActions, 0);
        C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
        C2(Actor_SetMovementTypeById, 9, 1);
        C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1c80000, 0x1680000);
        C2(Actor_SetMovementTypeById, 10, 1);
        C3(Actor_ArmCallback, 10, 0xe000, 0);
        C2(Actor_SetModeById, 8, 2);
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        C4(FieldCamera_MoveWithinMapBounds, 0x1e80000, none, 0x1800000, 1);
        FieldCamera_WaitForArrival();
        C3(Actor_ShowEmoteById, 10, 264, 0);
        C1(Event_Wait, 40);
        C1(Event_SetMessageId, (s32)MsgFieldAtLastTheSerpentHas);
        C2(Event_ShowMessageAndWait, 10, 0);
        C2(Motion_SetModeAndWaitAnimation, 10, 3);
        C2(Event_ShowMessageAndWait, 10, 0);
        C2(Actor_SetModeById, 10, 8);
        C2(Event_ShowMessageAndWait, 0x600a, 0);
        dragon->speedLimit = 0x18000;
        C3(Actor_SetDestinationById, 10, 488, 328);
        C1(Event_Wait, 5);
        Actor_SetModeById(10, 6);
        C1(Event_Wait, 10);
        dragon->velocityY = 0x60000;
        C1(Event_Wait, 10);
        dragon->movementState = 1;
        partner->movementState = 1;
        C2(Actor_SetAnimationSpeed, dragon, 0);
        C2(Actor_SetAnimationSpeed, partner, 0);
        C2(Actor_SetSpriteColorMode, dragon, 6);
        C2(Actor_SetSpriteColorMode, partner, 6);
        width = 30;
        height = 19;
        C6(MapGrid_CopyCellPictureCellsRect, 85, 76, 5, 2, width, height);
        C1(Sound_Play, 572);
        C2(BattleEffect_ApplyColorToTargetBuffer, 0x7fff, 0);
        C1(PaletteFade_StartInterpolation, 10);
        C1(Event_Wait, 10);
        C2(BattleEffect_ApplyColorToTargetBuffer, 0x10000, 0);
        C1(PaletteFade_StartInterpolation, 10);
        C1(Event_Wait, 10);
        C1(Event_Wait, 60);
        C2(Actor_SetSpriteColorMode, dragon, 0);
        C2(Actor_SetSpriteColorMode, partner, 0);
        C6(MapGrid_CopyCellPictureCellsRect, 85, 74, 5, 2, width, height);
        dragon->movementState = 0;
        partner->movementState = 0;
        C2(Actor_SetAnimationSpeed, dragon, 16);
        C2(Actor_SetAnimationSpeed, partner, 16);
        mask = 254;
        Actor_GetById(10)->actionFlags &= mask;
        C3(Actor_WalkToById, 10, 456, 360);
        fast = 0x40000;
        dragon->velocityY = fast;
        C2(FieldEffect_SetAttached, 10, 258);
        C1(Actor_WaitForMoveById, 10);
        C3(Actor_ShowEmoteById, 10, 257, 20);
        C1(Event_Wait, 30);
        Actor_SetModeById(10, 8);
        C1(Event_Wait, 10);
        dragon->speedLimit = 0x18000;
        C3(Actor_SetDestinationById, 10, 488, 328);
        C1(Event_Wait, 14);
        dragon->velocityY = 0x60000;
        Actor_SetModeById(10, 6);
        C1(Event_Wait, 10);
        C1(Sound_Play, 572);
        {
          struct Actor *a = Actor_GetById(10);
          mask &= a->actionFlags;
          a->actionFlags = mask;
        }
        C3(Actor_WalkToById, 10, 456, 360);
        dragon->velocityY = fast;
        C2(FieldEffect_SetAttached, 10, 258);
        C1(Actor_WaitForMoveById, 10);
        C2(Event_ShowMessageAndWait, 10, 0);
        C1(Event_Wait, 15);
        C2(Actor_SetAnimationSpeed, partner, 0);
        C1(Event_Wait, 15);
        C1(Sound_Play, 215);
        object = FieldActor_Create(285, 0x1e80000, -0x100000, 0x1590000);
        C2(Actor_SetSpriteFlags, object, 0);
        C2(Actor_StartScript, object, gData_0200539c);
        C2(Actor_SelectAnimation, object, 1);
        C2(Actor_SetSpriteColorMode, object, 6);
        C3(Actor_Jump, 10, 2, 0);
        C1(Event_Wait, 5);
        C3(Actor_ShowEmoteById, 10, 258, 0);
        C1(Event_Wait, 55);
        C1(Event_Wait, 5);
        C1(Sound_Play, 136);
        object = FieldActor_Create(412, 0x1e80000, -0x100000, 0x1590000);
        C2(Actor_SetSpriteFlags, object, 0);
        C2(Actor_StartScript, object, gData_02005360);
        ((struct GaiaRockAnimation *)object->sprite)->angle = 0x2000;
        object->speedLimit = 0x18000;
        *(u8 *)&object->motionFlags = 0;
        object->targetX = 0x1c80000;
        object->targetY = -0x200000;
        object->targetZ = 0x1660000;
        object->scaleX = 0x6000;
        object->scaleY = 0x6000;
        C1(Event_Wait, 10);
        C2(Actor_SetAnimationSpeed, partner, 16);
        C1(Event_Wait, 10);
        C1(Sound_Play, 133);
        object->x = 0;
        object->z = 0;
        C3(MapGrid_SetScreenShake, fast, fast, 0x10000);
        for (i = 0; i < 4; i++) {
          dx = -8;
          if (i > 1)
            dx = 8;
          dz = -8;
          if ((i & 1) == 0)
            dz = 8;
          shards[i] = (struct Actor *)Import_ActorCreate(
              403, 0x1c80000 + (dx << 16), -0x200000, 0x1660000 + (dz << 16));
          C2(Actor_SelectAnimation, shards[i], 2);
          C2(Actor_SetSpriteFlags, shards[i], 0);
          C2(Actor_StartScript, shards[i], gData_0200539c);
        }
        dragon->velocityY = 0xa0000;
        Actor_GetById(10)->actionFlags &= 254;
        C3(Actor_WalkToById, 10, 432, 320);
        C1(Event_Wait, 10);
        C3(MapGrid_SetScreenShake, -1, -1, 0xe666);
        Actor_SetModeById(10, 5);
        C1(Event_Wait, 30);
        C2(FieldCamera_MoveToActorById, 10, 1);
        FieldCamera_WaitForArrival();
        movement = (s32)gData_02005e28;
        C2(Actor_SetMovementTypeById, 10, movement);
        C1(Actor_RefreshSelectorById, 10);
        C1(Event_Wait, 20);
        C2(Actor_SetMovementTypeById, 10, movement);
        C1(Event_Wait, 20);
        C2(Event_ShowMessageAndWait, 0x600a, 0);
        C2(FieldCamera_FollowActor, gMainRoutineWork.currentOwner, 1);
        FieldCamera_WaitForArrival();
        C2(Actor_SetSpritePriority, 10, 3);
        C6(Map_CopyCellAttributeRect, 30, 19, 1, 1, 27, 19);
        Event_End();
        goto end;
      } else {
        if (Test(0x109))
          goto end;
        C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1d80000, 0x1c80000);
        C2(Actor_SetMovementTypeById, 10, gData_0200578c);
        goto end;
      }
    } else if (*entrance == 10) {
      s32 flag;
      s32 scale;
      struct Actor *dragon;
      s32 slow;
      s32 *owner;
      struct Actor *other;
      struct Actor *object;
      volatile u16 alpha;
      s32 i;

      C1(Sound_Play, 7);
      flag = Test(FLAG_SERPENT_SLAIN);
      if (flag != 0)
        goto end;
      Event_Start();
      C1(Event_ResolvePendingActions, 0);
      C1(GameFlag_Set, FLAG_SERPENT_SLAIN);
      C1(Event_SetMessageId, (s32)MsgFieldItLooksLikeTheDragon);
      C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
      Actor_SetModeById(8, 3);
      Actor_GetById(11)->speedLimit = scale = 0x10000;
      slow = 0x8000;
      dragon = Actor_GetById(10);
      dragon->speedLimit = slow;
      C3(Actor_SetHorizontalPositionWithTerrain, 11, 0x1b80000, 0x1380000);
      owner = &room->currentOwner;
      C3(Actor_SetHorizontalPositionWithTerrain, *owner, 0x1c80000, 0x1480000);
      C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1e80000, 0x1a80000);
      C3(Actor_ArmCallback, *owner, 0, 0);
      C3(Actor_ArmCallback, 11, 0, 0);
      C3(Actor_ArmCallback, 10, 0xc000, 0);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      C3(Actor_ShowEmoteById, 11, 258, 30);
      C1(Sound_Play, 530);
      C2(Event_ShowMessageAndWait, 11, 0);
      C1(Sound_Play, 530);
      C2(Event_ShowMessageAndWait, 10, 0);
      Actor_ShowEmote(*owner, 256, 0);
      C3(Actor_ShowEmoteById, 11, 256, 0);
      C1(Event_Wait, 15);
      C3(Actor_ArmCallback, *owner, 0x4000, 0);
      C3(Actor_ArmCallback, 11, 0x4000, 0);
      C1(Event_Wait, 30);
      C2(FieldCamera_MoveToActorById, 10, 1);
      FieldCamera_WaitForArrival();
      C2(Actor_StartRepeatedMotion, 10, 2);
      C2(FieldCamera_FollowActor, 10, 1);
      C3(Actor_WalkToById, 10, 488, 376);
      C1(Actor_WaitForMoveById, 10);
      C2(Actor_StartRepeatedMotion, 10, 2);
      C1(Event_Wait, 20);
      C3(Actor_WalkToById, 10, 472, 360);
      C1(Actor_WaitForMoveById, 10);
      C3(Actor_FaceActorById, *owner, 10, 0);
      C3(Actor_FaceActorById, 11, 10, 0);
      C3(Actor_FaceActorById, 10, 8, 0);
      C4(FieldCamera_MoveWithinMapBounds, -1, -1, -1, 0);
      C2(Actor_SetModeById, 10, 8);
      C2(Event_ShowMessageAndWait, 10, 0);
      dragon->speedLimit = 0x18000;
      C3(Actor_SetDestinationById, 10, 488, 328);
      C1(Event_Wait, 5);
      Actor_SetModeById(10, 6);
      C1(Event_Wait, 10);
      dragon->velocityY = 0x60000;
      C1(Event_Wait, 10);
      C1(Sound_Play, 572);
      dragon->movementState = 1;
      C2(BattleEffect_ApplyColorToTargetBuffer, 0x7fff, 0);
      C1(PaletteFade_StartInterpolation, 10);
      C1(Event_Wait, 10);
      C1(Sound_Play, 530);
      C2(BattleEffect_ApplyColorToTargetBuffer, scale, 0);
      C1(PaletteFade_StartInterpolation, 10);
      C1(Event_Wait, 10);
      C2(BattleEffect_ApplyColorToTargetBuffer, 0x7fff, 0);
      C1(PaletteFade_StartInterpolation, 10);
      C1(Event_Wait, 10);
      C2(BattleEffect_ApplyColorToTargetBuffer, scale, 0);
      C1(PaletteFade_StartInterpolation, 10);
      C1(Event_Wait, 10);
      C2(BattleEffect_ApplyColorToTargetBuffer, 0x7fff, 0);
      C1(PaletteFade_StartInterpolation, 10);
      C1(Event_Wait, 60);
      C2(Actor_SetModeById, 8, 4);
      C6(MapGrid_CopyCellPictureCellsRect, 85, 72, 5, 2, 30, 19);
      dragon->y = -0x200000;
      dragon->velocityY = 0;
      dragon->speedLimit = slow;
      C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1c80000, 0x1680000);
      C2(Actor_SetModeById, 10, 1);
      C2(BattleEffect_ApplyColorToTargetBuffer, scale, 0);
      C1(PaletteFade_StartInterpolation, 10);
      C1(Event_Wait, 10);
      C3(Actor_FaceActorById, 10, *owner, 0);
      C2(Event_ShowMessageAndWait, 10, 0);
      C2(Motion_SetModeAndWaitAnimation, 10, 3);
      C2(Event_ShowMessageAndWait, 10, 0);
      C3(Actor_ArmCallback, 10, 0x4000, 0);
      C2(Actor_SetModeById, 10, 2);
      C3(Actor_WalkToById, 10, 488, 392);
      C2(Actor_StartRepeatedMotion, 11, 1);
      C1(Event_Wait, 5);
      C2(Event_ShowMessageAndWait, 11, 0);
      C1(Actor_WaitForMoveById, 10);
      C2(Actor_SetModeById, 10, 0);
      C3(Actor_FaceActorById, 10, 11, 0);
      C1(Event_Wait, 15);
      C2(Event_ShowMessageAndWait, 10, 0);
      C2(Motion_SetModeAndWaitAnimation, 10, 3);
      C3(Actor_ArmCallback, 10, 0x4000, 0);
      C2(Actor_SetModeById, 10, 2);
      C3(Actor_WalkToById, 10, 488, 536);
      C2(FieldCamera_FollowActor, *owner, 1);
      C3(Actor_FaceActorById, *owner, 11, 0);
      C3(Actor_FaceActorById, 11, *owner, 0);
      C1(Event_Wait, 30);
      C2(Motion_SetModeAndWaitAnimation, 11, 3);
      C2(Actor_SetModeById, 11, 2);
      {
        struct Actor *ownerActor = Actor_GetById(*owner);

        if (ownerActor != NULL)
          Actor_SetDestinationById(11, ownerActor->x >> 16,
                                   ownerActor->z >> 16);
      }
      C1(Actor_WaitForMoveById, 11);
      C3(Actor_SetHorizontalPositionWithTerrain, 11, 0, 0);
      C1(Actor_WaitForMoveById, 11);
      C3(Actor_SetHorizontalPositionWithTerrain, 10, 0, 0);
      C3(Actor_WalkToById, *owner, 488, 376);
      C1(Actor_WaitForMoveById, *owner);
      C3(MapGrid_SetScreenShake, 0x50000, 0x50000, scale);
      C1(Sound_Play, 107);
      C1(Event_Wait, 30);
      C3(Actor_ShowEmoteById, *owner, 256, 0);
      C1(Event_Wait, 15);
      C3(Actor_ArmCallback, *owner, 0xc000, 0);
      C1(Event_Wait, 15);
      other = Actor_GetById(12);
      alpha = REG_BLDALPHA;
      C3(MapGrid_SetScreenShake, 0xa0000, 0xa0000, scale);
      C1(Sound_Play, 107);
      BlendScript_Suspend();
      alpha = *(u8 *)&alpha;
      while (alpha != 0) {
        REG_BLDALPHA = alpha | 0x1000;
        alpha = alpha + 0xffff;
        C1(wait, 1);
      }
      REG_DISPCNT &= 0xfdff;
      C1(Event_Wait, 30);
      {
        s32 x;
        /* FAKEMATCH(pin): z in r5 and x in r6; as a plain local, 7 of 1961
         * instructions differ in tla-ja. */
        register s32 z asm("r5");

        x = 0x1d00000;
        z = 0x15c0000;
        object = OverlayObject_PrepareObject(x, -0x200000, z, 323);
        Actor_SetHorizontalPositionWithTerrain(12, x, z);
      }
      for (i = 0; i < 20; i++) {
        if (i % 5 == 0)
          Func_02001c9c();
        other->z += -0x10000;
        C1(Event_Wait, 2);
      }
      C1(GameFlag_Set, 0x9ee);
      C3(MapGrid_SetScreenShake, -1, -1, 0xe666);
      object->x = 0;
      object->z = 0;
      C1(Event_Wait, 1);
      C1(Actor_Release, object);
      C1(Sound_Play, 390);
      C1(Event_Wait, 30);
      REG_DISPCNT |= 0xf00;
      REG_BLDALPHA = alpha | 0x1000;
      REG_BLDCNT = 0x3f42;
      while (alpha != 8) {
        REG_BLDALPHA = alpha | 0x1000;
        alpha++;
        C1(wait, 1);
      }
      BlendScript_Resume();
      Event_End();
    } else if (*entrance == 11) {
      Event_Start();
      C1(Event_ResolvePendingActions, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1c00000, 0x1580000);
      {
        s32 *lead = &room->currentOwner;

        C3(Actor_FaceActorById, 10, *lead, 0);
        Value2((s32 (*)())Actor_SetModeById, *lead, 19);
      }
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      C1(Event_SetMessageId, (s32)MsgFieldYouYouCannotFightThe);
      Event_ShowMessageAndWait(10, 0);
      Event_FinishScreenTransition();
      Event_WaitTransitionFrames();
      C2(Event_SetDestination, (s16)room->savedScene, (s16)room->savedEntrance);
      Event_End();
    } else if (*entrance == 12) {
      Event_Start();
      C1(Event_ResolvePendingActions, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 10, 0x1ae0000, 0x1400000);
      C2(Actor_SetModeById, 10, 5);
      C2(Actor_SetModeById, room->currentOwner, 19);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      C1(Event_Wait, 30);
      Value2((s32 (*)())Actor_StartRepeatedMotion, 10, 2);
      C1(Event_Wait, 30);
      C1(Event_SetMessageId, (s32)MsgFieldLikeISaidTheSerpent);
      Event_ShowMessageAndWait(10, 0);
      Event_FinishScreenTransition();
      Event_WaitTransitionFrames();
      C2(Event_SetDestination, (s16)room->savedScene, (s16)room->savedEntrance);
      Event_End();
    }
  } else if (work->scene == (s32)&SceneId_Ov681Room2) {
    s32 mask = 32;
    s32 low;
    s32 high;
    struct Actor *object = Actor_Get(8);

    object->priorityFlags |= mask;
    C2(Actor_SetSpritePriorityDirect, object, 3);
    object = Actor_Get(9);
    object->priorityFlags |= mask;
    C2(Actor_SetSpritePriorityDirect, object, 3);
    C2(Func_02002ac8, 8, 12);
    C2(Func_02002ac8, 9, 12);
    if (Test(0xa62)) {
      C3(Actor_SetHorizontalPositionWithTerrain, 8, 0x1b80000, 0x2b00000);
      C3(Actor_SetHorizontalPositionWithTerrain, 9, 0x2280000, 0x2b00000);
    } else {
      s32 height = 40;

      C6(Map_CopyCellsRect, 88, 40, 1, 2, 89, height);
      C6(Map_CopyCellsRect, 99, 40, 1, 2, 100, height);
    }
    low = FieldGameFlag_GetByte(0x210) & 15;
    high = (u8)(FieldGameFlag_GetByte(0x210) >> 4);
    if (low != 0)
      C3(Actor_SetHorizontalPositionWithTerrain, 10,
         gData_0200530c[low][0] << 19, gData_0200530c[low][1] << 19);
    if ((s8)high != 0)
      C3(Actor_SetHorizontalPositionWithTerrain, 11,
         gData_0200530c[(s8)high][0] << 19, gData_0200530c[(s8)high][1] << 19);
  } else if (work->scene == (s32)&SceneId_Ov681Room4) {
    if (Test(0xa63))
      C1(Func_02002cb4, 1);
  } else if (work->scene == (s32)&SceneId_Ov681Room3) {
    struct Actor *object;
    struct Actor *other;

    object = Actor_GetById(10);
    C2(Actor_SetSpritePriorityDirect, object, 3);
    C2(Func_02002ac8, 8, 10);
    C2(Func_02002ac8, 9, 11);
    if (!Test(0x9bc))
      C6(Map_CopyCellsRect, 25, 77, 1, 15, 26, 77);
    if (Test(0x9bd)) {
      s32 width;
      s32 height;

      C3(Actor_SetHorizontalPositionWithTerrain, 8, 0x1e00000, 0x1600000);
      width = 28;
      C6(Map_CopyCellsRect, 2, 93, 1, 5, width, 85);
      height = 22;
      C6(Map_CopyCellAttributeRect, 27, 23, 1, 1, width, height);
      C6(Map_CopyCellAttributeRect, 32, 16, 2, 1, 29, height);
    }
    if (Test(0x9be)) {
      s32 width;
      s32 height;

      C3(Actor_SetHorizontalPositionWithTerrain, 9, 0x2600000, 0x1600000);
      width = 36;
      C6(Map_CopyCellsRect, 2, 93, 1, 5, width, 85);
      height = 22;
      C6(Map_CopyCellAttributeRect, 35, 23, 1, 1, width, height);
      C6(Map_CopyCellAttributeRect, 32, 16, 2, 1, 37, height);
    }
    if (Test(0x9bf)) {
      C6(MapGrid_CopyCellPictureCellsRect, 61, 0, 3, 3, 37, 26);
      C6(Map_CopyCellAttributeRect, 36, 23, 1, 1, 38, 25);
    }
    if (work->entrance == 6) {
      s32 frames = 60;

      Event_Start();
      C1(Event_ResolvePendingActions, 0);
      C4(FieldCamera_MoveWithinMapBounds, -1, -1, -1, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, 11, 0, 0);
      C3(Actor_SetHorizontalPositionWithTerrain, work->currentOwner, 0, 0);
      Event_StartScreenTransition();
      C1(Event_Wait, 15);
      C3(Actor_SetHorizontalPositionWithTerrain, 11, 0x2a00000, 0x1800000);
      C2(Actor_SetSpriteFlags, Actor_GetById(11), 0);
      object = Actor_GetById(11);
      other = FieldActor_Create(324, object->x, object->y, object->z);
      if (other != NULL) {
        C2(Actor_SetSpriteFlags, other, 0);
        C2(Actor_StartScript, other, gData_020053d8);
        C2(Actor_SelectAnimation, other, 1);
        C2(Actor_SetSpriteColorMode, other, 14);
        ((u8 *)other->sprite)[9] |= 12;
        other->scaleX = 0x13333;
        other->scaleY = 0x13333;
      }
      object->y += 0x800000;
      for (; frames != 0; frames--) {
        C1(wait, 1);
        if (object->velocityY > -1)
          break;
      }
      C1(Sound_Play, 188);
      C1(Event_Wait, 30);
      C1(GameFlag_Set, 0x109);
      C1(Import_EventSetExitRequest, 3);
    }
    if (!Test(0x9c4))
      C3(Actor_SetHorizontalPositionWithTerrain, 11, 0, 0);
  } else if (work->scene == (s32)&SceneId_Ov681Room5) {
    C2(HangingVine_Place, 10, 1);
    C2(Func_02002ac8, 8, 9);
    if (work->entrance == 1 || work->entrance == 3 || work->entrance == 5)
      Func_02004d00();
    if (Test(0x303))
      Func_02004a5c(11, 0x1800000, 0x4b0000, 0x2a80000);
    if (Test(0x304))
      Func_02004a5c(12, 0x1cf0000, 0x330000, 0x2a80000);
    if (Test(0x305))
      Func_02004a5c(13, 0x2350000, 0x370000, 0x2a80000);
    if (Test(0x9bc)) {
      s32 width;
      s32 height;

      C3(Actor_SetHorizontalPositionWithTerrain, 8, 0x1800000, 0x1700000);
      width = 25;
      C6(Map_CopyCellsRect, 1, 85, 1, 4, width, 85);
      height = 23;
      C6(Map_CopyCellAttributeRect, 27, 15, 2, 1, height, height);
      C6(Map_CopyCellAttributeRect, 25, 25, 1, 1, width, height);
    }
    C2(Actor_SetSpriteFlags, Actor_GetById(15), 0);
    if (Test(0x9c4)) {
      s32 height = 20;

      C6(MapGrid_CopyCellPictureCellsRect, 117, 20, 7, 8, 104, height);
      C6(MapGrid_CopyCellPictureCellsRect, 111, 12, 1, 6, 111, height);
      C6(Map_CopyCellAttributeRect, 49, 84, 8, 8, 40, 84);
      C3(Actor_SetHorizontalPositionWithTerrain, 15, 0, 0);
    }
  }
end:
  C1(FieldEffect_StartBlankOverlay, 0);
  return 0;
}

#endif

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02004274(s32 a0) { Func_0200495c(1, a0); }

void *Unit_Get(s32 owner);
void BlendScript_Start(u16 *script);
/* VIEW(s2cb4): routine Func_02000aac as void Func_02000aac(void);
 * games/tla/src/scene/gaia_rock_inside/s07f4 has void Func_02000aac(struct
 * Actor *object) {; tried as the definition's void Func_02000aac(void),
 * Scene_PrepareMap moves: 1 of 226 instructions in tla-ja. */
void Func_02000aac__s2cb4(void) __asm__("Func_02000aac");
#define Func_02000aac Func_02000aac__s2cb4
/* VIEW(s2cb4): routine Func_02000a60 as void Func_02000a60(void);
 * games/tla/src/scene/gaia_rock_inside/s07f4 has void Func_02000a60(struct
 * Actor *object) {; tried as the definition's void Func_02000a60(struct Actor
 * *object), which the object already declares, Scene_PrepareMap moves: 1 of 226
 * instructions in tla-ja. */
void Func_02000a60__s2cb4(void) __asm__("Func_02000a60");
#define Func_02000a60 Func_02000a60__s2cb4

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room1) {
    u8 *map = (u8 *)Ram_HeapSlots->mapWork;

    if (gMainRoutineWork.entrance <= 4) {
      u8 *unit;
      s32 total;
      s32 *stage;
      s32 i;

      total = 0;
      unit = Unit_Get(gMainRoutineWork.currentOwner);

      for (i = 0; i <= 13; i++)
        total += unit[i];
      Func_020044f8(total);
      Actor_GetById(Party_GetLeaderActor())->priorityFlags |= 32;
      map[261] = 1;
      {
        volatile u16 control = *(volatile u16 *)0x0400000c & ~3;
        // Once-only block: keeps the sky cell window's store after its read.
        do {
        } while (0);

        *(volatile u16 *)0x0400000c = control | 1;
      }
      Func_020048a0();
      stage = (s32 *)(gInventorySnapshot + 0x288);
      if (*stage == 3 && !GameFlag_Get(0x3f6))
        Actor_SetPosition(64, 0x680000, 0x580000);
      if (*stage == 9)
        Actor_SetPosition(65, 0x680000, 0x480000);
      if (GameFlag_Get(*stage + 0xa50)) {
        struct Actor *climber = Actor_GetById(8);
        u32 angle = Func_02004904();

        climber->collisionFlags = 0;
        Actor_SelectAnimation(climber, ((angle & 0xc000) >> 14) + 2);
        climber->update = (angle & 0x4000) != 0 ? Func_02000aac : Func_02000a60;
        climber->collisionFlags = 1;
        GameFlag_Set(0x200);
      }
    } else if (gMainRoutineWork.entrance > 8) {
      u8 *layer;

      *(s32 *)(gInventorySnapshot + 0x288) = 1;
      layer = map + 376;
      FIELD_AT(layer, u16, 40) = 127;
      FIELD_AT(layer, s32, 24) = 0;
      FIELD_AT(layer, s32, 28) = 0;
      FIELD_AT(layer, s32, 32) = 0;
      FIELD_AT(layer, s32, 36) = 0;
      FIELD_AT(layer, s32, 16) = 0x10000;
      FIELD_AT(layer, s32, 20) = 0x10000;
      FIELD_AT(layer, u16, 42) = 0x3f80;
      // Once-only block: keeps the scale defaults before the position sums.
      do {
      } while (0);
      ((struct MapLayer *)layer)->x =
          Iwram_MulQ16(((struct MapWork *)map)->viewX,
                       FIELD_AT(layer, s32, 16)) +
          FIELD_AT(layer, s32, 8);
      ((struct MapLayer *)layer)->y =
          Iwram_MulQ16(((struct MapWork *)map)->viewZ,
                       FIELD_AT(layer, s32, 20)) +
          FIELD_AT(layer, s32, 12);
      Map_CenterOnFollowedPoint();
      System_Wait(1);
      Iwram_CopyWords(Ram_MapCollision + 0x1e00, gData_020053e4, 24);
      BlendScript_Start((u16 *)(Ram_MapCollision + 0x1e00));
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room2) {
    switch (gMainRoutineWork.entrance) {
    case 1:
    case 2:
    case 5:
    case 8:
      Func_02004274(100);
      break;
    case 9:
    case 10:
      Func_02004274(102);
      break;
    case 3:
    case 4:
      Func_02004274(103);
      break;
    case 6:
    case 7:
      Func_02004274(101);
      break;
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room5) {
    switch (gMainRoutineWork.entrance) {
    case 1:
    case 3:
    case 5:
      Func_02004274(100);
      break;
    case 2:
    case 4:
    case 7:
      Func_02004274(101);
      break;
    case 6:
      Func_02004274(102);
      break;
    }
  }
  return 0;
}

#define GRID_WIDTH 28

typedef struct {
  Unk_Cell cells[GRID_WIDTH * GRID_WIDTH];
} Unk_Grid;

#define gPath ((Unk_Path *)(gInventorySnapshot + 0x288))

static __inline__ void *Heap_AllocEwram_(s32 size) {
  /* FAKEMATCH(wrapper): forwarded inline, as scene_call.h describes; as a
   * direct call, 102 of 418 instructions differ in tla-en. */
  return Heap_AllocEwram(size);
}

#define AT(x, y) grid->cells[(y) * GRID_WIDTH + (x)]
#define AT_ADDRESS(x, y)                                                       \
  ((Unk_Cell *)(((y) * GRID_WIDTH + (x)) * 4 + (s32)grid))

void Func_020044f8(s32 seed) {
  Unk_Grid *grid;
  Unk_Path *path;
  Unk_Step *steps;
  Unk_Cell *cell;
  s32 x, y, nx, ny, step, tries, dir, v, next;
  u32 saved;

  saved = gRandomState;
  path = gPath;
  steps = path->steps;
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
    return;
  if (GameFlag_Get(0x3f0))
    return;
  gRandomState = seed;
restart:
  grid = Heap_AllocEwram_(sizeof *grid);
  Iwram_ClearWordsEntry(grid, sizeof *grid);
  // BUG: this runs 29 times, so row 28 and column 28 lie past the 28 by 28
  // cells and the last row's marks land after the block.
  {
    Unk_Cell *a, *b, *e;
    s32 o, t;
    x = GRID_WIDTH / 2;
    y = GRID_WIDTH / 2;
    o = 0;
    b = &AT(GRID_WIDTH, 0);
    // step counts the border's rows here, before the walk numbers its steps.
    step = GRID_WIDTH;
    a = grid->cells;
    e = grid->cells;
  border:
    a->unk_0 = 99;
    a += GRID_WIDTH;
    b->unk_0 = 99;
    b += GRID_WIDTH;
    e->unk_0 = 99;
    e++;
    t = o + GRID_WIDTH * GRID_WIDTH * 4;
    ((s8 *)grid)[t] = 99;
    o += sizeof(Unk_Cell);
    step--;
    if (step >= 0)
      goto border;
  }
  for (step = 1; step <= 14;) {
    tries = 0;
  retry:
    if (++tries > 16)
      goto restart;
    nx = x;
    ny = y;
    dir = (Random16() * 4) >> 16;
    if (dir == 0 && (step == 1 || step == 13))
      goto retry;
    switch (dir) {
    case 0:
      if (AT(x, y + 1).unk_0 != 0 || AT(x, y + 2).unk_0 != 0 ||
          AT(x - 1, y + 1).unk_0 != 0 || AT(x + 1, y + 1).unk_0 != 0)
        goto retry;
      ny = y + 1;
      break;
    case 1:
      if (AT(x - 1, y).unk_0 != 0 || AT(x - 2, y).unk_0 != 0 ||
          AT(x - 1, y - 1).unk_0 != 0 || AT(x - 1, y + 1).unk_0 != 0)
        goto retry;
      nx = x - 1;
      break;
    case 2:
      if (AT(x + 1, y).unk_0 != 0 || AT(x + 2, y).unk_0 != 0 ||
          AT(x + 1, y - 1).unk_0 != 0 || AT(x + 1, y + 1).unk_0 != 0)
        goto retry;
      nx = x + 1;
      break;
    case 3:
      if (AT(x, y - 1).unk_0 != 0 || AT(x, y - 2).unk_0 != 0 ||
          AT(x - 1, y - 1).unk_0 != 0 || AT(x + 1, y - 1).unk_0 != 0)
        goto retry;
      ny = y - 1;
      break;
    }
    // The same cell variable the second loop reads its steps from.
    cell = AT_ADDRESS(x, y);
    cell->unk_0 = step;
    step++;
    cell->unk_1 = nx;
    cell->unk_2 = ny;
    x = nx;
    y = ny;
  }

  x = GRID_WIDTH / 2;
  y = GRID_WIDTH / 2;
  for (step = 1; step <= 14; step = next) {
    cell = AT_ADDRESS(x, y);
    next = step + 1;

    {
      s32 c;

      v = step;
      c = AT(x, y + 1).unk_0;

      if (c == next)
        v = next;
      else if (c == step - 1)
        v = c;
      else {
        Unk_Cell *p = cell;
      scanUp:
        p -= GRID_WIDTH;
        c = p->unk_0;
        if (c == v - 1) {
          v = c;
          goto scanUp;
        }
      }
      if (v == 0)
        v = 1;
    }
    steps[step].unk_2 = v;

    {
      s32 c;

      v = step;
      c = AT(x, y - 1).unk_0;

      if (c == next)
        v = next;
      else if (c == step - 1)
        v = c;
      else {
        Unk_Cell *p = AT_ADDRESS(x, y);
      scanDown:
        p += GRID_WIDTH;
        c = p->unk_0;
        if (c == v - 1) {
          v = c;
          goto scanDown;
        }
      }
      if (v == 0)
        v = 1;
    }
    steps[step].unk_5 = v;

    {
      s32 c;

      v = step;
      c = AT(x - 1, y).unk_0;

      if (c == next)
        v = next;
      else if (c == step - 1)
        v = c;
      else {
        nx = x;
      scanRight:
        c = AT(++nx, y).unk_0;
        if (c == v - 1) {
          v = c;
          goto scanRight;
        }
      }
      if (v == 0)
        v = 1;
    }
    steps[step].unk_3 = v;

    {
      s32 c;

      v = step;
      c = AT(x + 1, y).unk_0;

      if (c == next)
        v = next;
      else if (c == step - 1)
        v = c;
      else {
        nx = x;
      scanLeft:
        c = AT(--nx, y).unk_0;
        if (c == v - 1) {
          v = c;
          goto scanLeft;
        }
      }
      if (v == 0)
        v = 1;
    }
    steps[step].unk_4 = v;
    x = cell->unk_1;
    steps[step].unk_0 = step;
    y = cell->unk_2;
  }
  steps[14].unk_5 = 251;
  steps[1].unk_2 = 250;
  path->unk_0 = 1;
  Heap_Release(grid);
  Event_SetFlag(0x3f0);
  gRandomState = saved;
}

void Func_0200485c(s32 direction) {
  s32 *state = (s32 *)(gInventorySnapshot + 0x288);
  u8 *row = (u8 *)state + *state * 8;
  s32 next = row[((direction - 1) & 3) + 6];

  switch (next) {
  case 250:
  case 251:
    FieldEvent_SetExitRequest(next);
    break;
  case 0:
    next = 1;
  default:
    *state = next;
    FieldEvent_SetExitRequest(direction);
    break;
  }
  Event_PlaySound(123);
}

/* VIEW(s2cb4): variable gData_020053fc as s8 gData_020053fc[][2];
 * scene/gaia_rock_inside/s4d00 has u32 gData_020053fc[] = {; tried as the
 * definition's extern u32 gData_020053fc[], it does not compile in tla-ja:
 * games/tla/src/scene/gaia_rock_inside.c:4011: `gData_020053fc' undeclared
 * (first use in this function). */
extern s8 gData_020053fc__s2cb4[][2] __asm__("gData_020053fc");
#define gData_020053fc gData_020053fc__s2cb4

// From the third state on, for each of the two map layers:
// Map_CopyRawCellsRect with the state's origin plus the layer's cell, a 15 by
// 10 size and the layer's cell plus one.
void Func_020048a0(void) {
  s32 state = *(s32 *)(gInventorySnapshot + 0x288);
  s8 *origin = gData_020053fc[state];
  struct GaiaRockInsideMapWork *map =
      (struct GaiaRockInsideMapWork *)Ram_HeapSlots->mapWork;

  if (state > 1) {
    s32 originX = origin[0];
    s32 originZ = origin[1];
    struct GaiaRockInsideLayer *layer = map->layers;
    s32 i;

    for (i = 1; i >= 0; i--, layer++) {
      /* FAKEMATCH(pin): x fixed to r2, so x is held in r2 and z in r3; as a
       * plain local, 18 of 45 instructions differ in tla-en. */
      register s32 x asm("r2") = layer->x >> 20;
      s32 z = layer->z >> 20;

      Map_CopyRawCellsRect(originX + x, originZ + z, 15, 10, x + 1, z + 1);
    }
  }
}

#define GAIA_ROCK_INSIDE_PATH                                                  \
  ((struct GaiaRockInsidePath *)(gInventorySnapshot + 0x288))

// The direction from the current node whose link is 251 or the next node:
// 0x4000, 0x8000, 0 or 0xc000 for the four links in turn, or -1 if none is.
s32 Func_02004904(void) {
  struct GaiaRockInsidePath *path = GAIA_ROCK_INSIDE_PATH;
  s32 current = path->current;
  /* FAKEMATCH(pin): the node fixed to r3 and each link to r2; as a plain local,
   * 34 of 41 instructions differ in tla-en. */
  register struct GaiaRockInsideNode *node asm("r3") = &path->nodes[current];
  s32 next = current + 1;
  u32 link;

  link = node->next[0];
  if (link == 251 || link == next)
    return 0x4000;
  link = node->next[1];
  if (link == 251 || link == next)
    return 0x8000;
  link = node->next[2];
  if (link == 251 || link == next)
    return 0;
  link = node->next[3];
  if (link == 251 || link == next)
    return 0xc000;
  return -1;
}

void Func_0200495c(s32 layer, s32 keep) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapLayer *mapLayer = &map->layers[layer];
  u32 *cells = (u32 *)mapLayer->cells;
  s32 width = (map->maxX - map->minX) >> 20;
  s32 height = (map->maxZ - map->minZ) >> 20;
  u32 fill;
  u16 z;
  u16 x;

  cells += ((map->minZ >> 20) << 7) + (map->minX >> 20);
  fill = *cells;
  for (z = 0; z < height; z++) {
    u32 *cell = &cells[z * 128];

    for (x = 0; x < width; x++) {
      if (((u8 *)cell)[2] != 0 && ((u8 *)cell)[2] != keep)
        *cell = fill;
      cell++;
    }
  }
}

void Func_020049e4(void) {
  if (gMainRoutineWork.renderMode == 2) {
    u8 *event = Ram_HeapSlots->eventWork;
    struct MapWork *map = Ram_HeapSlots->mapWork;
    struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
    s32 x = object->x / 0x100000;
    s32 z = (object->z - object->y + 0x7fff) / 0x100000;
    struct MapCell *cells = map->layers[2].cells;
    struct MapCell *cell = &cells[x + (z << 7)];
    s32 code = cell->collisionCode;

    if (code >= 1 && code <= 230)
      *(u16 *)(event + 340) = code;
  }
}

void Func_02004a5c(s32 id, s32 x, s32 y, s32 z) {
  struct Actor *object = Actor_Get(id);
  struct MapCell *cell = Ram_HeapSlots->mapWork->layers[2].cells;
  s32 column = x / 0x100000;
  s32 row = (z - y + 0x7fff) / 0x100000;

  cell += column + (row << 7);
  cell->collisionCode = 255;
  object->x = (x & 0xfff00000) + 0x80000;
  object->z = (z & 0xfff00000) + 0x80000;
  object->y = object->z - (row << 20);
  object->motionFlags = 0;
  object->priorityFlags |= 2;
  Actor_SetSpriteFlags(object, 0);
}

/* VIEW(s2cb4): variable gData_020062d4 as const u8 gData_020062d4[];
 * scene/gaia_rock_inside/s4d00 has DEFINE_G_DATA_020016CC(gData_020062d4) //
 * clang-format on #undef Func_020049e4 #undef Func_020005dc #undef
 * Func_020005f8 #undef Func_02000610 #undef Func_02000678 #undef Func_020006e0
 * #undef Func_020006f8 #undef Func_02000718 #undef Func_02000748 #undef
 * Func_020007f4 #undef Func_020008b4 #undef Func_02000900 #undef Func_02000af8
 * #undef Orbiter_Spiral #undef Orbit_Update #undef Actor_AimAtWatched #undef
 * Actor_StepToWatched #undef GaiaRockInside_ReadSandTablet #undef Func_02002974
 * #undef Func_0200485c; tried as the one declaration extern const u8
 * gData_020062d4[], it does not compile in tla-ja:
 * games/tla/src/scene/gaia_rock_inside.c:4146: `gData_020062d4' undeclared
 * (first use in this function). */
extern const u8 gData_020062d4__s2cb4[] __asm__("gData_020062d4");
#define gData_020062d4 gData_020062d4__s2cb4
extern u32 gData_02006290[];

void Func_02004af0(s32 id, s32 flag) {
  struct MainRoutineWork *entryWork = &gMainRoutineWork;
  struct Actor *leader;
  s32 gravity;
  s32 value;
  struct Actor *object;
  s32 i;
  struct MainRoutineWork *work;

  /* FAKEMATCH(barrier): the form kept; as plain gMainRoutineWork.currentOwner
   * with no asm, 2 of 218 instructions differ in tla-en. */
  __asm__("" : "=r"(entryWork) : "0"(entryWork));
  /* FAKEMATCH(barrier): the form kept; as plain code with no asm here, 2 of 218
   * instructions differ in tla-en. */
  __asm__("");
  /* FAKEMATCH(temporary): the form kept; as id used directly from entry, 4 of
   * 218 instructions differ in tla-en. */
  value = id;
  leader = Actor_Get(entryWork->currentOwner);
  gravity = leader->gravity;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  object = Actor_Get(value);
  Func_02004a5c(value, leader->x, leader->y, leader->z);
  Actor_StartScript(object, gData_020062d4);
  i = 15;
burst:
  object = (FieldActor_Create(255, leader->x, leader->y + 0x100000, leader->z));
  if (object != NULL) {
    u32 angle;
    u32 size;
    s32 scale;

    Actor_StartScript(object, gData_02006290);
    angle = Random16();
    value = Random16();
    size = Random16();
    scale = (size >> 1) + 0x2000;
    object->velocityY = Iwram_MulQ16(value << 3, Trig_Cos(angle));
    object->velocityX = Iwram_MulQ16(value << 3, Math_Sin(angle));
    object->acceleration = 0;
    object->scaleX = scale;
    object->scaleY = scale;
    object->gravity = 0x547a;
    object->bounce = 0x8000;
  }
  i--;
  if (i >= 0)
    goto burst;
  Event_PlaySound(590);
  Event_PlaySound(217);
  MapGrid_SetShake(0x20000, 0x20000, 0xe666);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 257);
  Actor_SelectAnimation(leader, 40);
  FieldEvent_Wait(40);
  Event_PlaySound(204);
  leader->motionFlags = 3;
  leader->gravity = 0x547a;
  for (value = 0; value < 180; value++) {
    System_Wait(1);
    if (leader->y <= leader->terrainHeight)
      break;
  }
  Event_PlaySound(188);
  work = &gMainRoutineWork;
  leader->gravity = gravity;
  Actor_SetAttachedEffect(work->currentOwner, 0);
  Actor_SelectAnimation(leader, 38);
  MapGrid_SetShake(0x10000, 0x40000, 0x10000);
  System_Wait(10);
  MapGrid_SetShake(-1, -1, 0xe666);
  System_Wait(20);
  FieldCamera_MoveTo(leader->x, leader->y, leader->z, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_SetSpriteFlags(leader, 1);
  Actor_SelectAnimation(leader, 1);
  leader->velocityY = 0x40000;
  System_Wait(20);
  work->renderMode = 0;
  Event_End();
  Event_SetFlag(flag);
}

#undef gSceneState
#undef gData_0200530c
#undef Func_02000aac
#undef Func_02000a60
#undef gData_0200533c
#undef gData_02005360
#undef gData_0200539c
#undef gData_020053d8
#undef gData_020053fc
#undef gData_0200578c
#undef gData_020062d4

// ---- scene/gaia_rock_inside/s4d00.c ----

// Scene code of overlay 0x681, in ROM order.

void Func_02004d00(void) {
  Task_AddFrame((s32)Func_020049e4, TASK_PASS_VBLANK);
}

#include "text/msg_ids.h"

void Func_02000b6c(void);
void Func_02000cd8(void);

extern u32 gData_02005528[];
extern u32 gData_02005a30[];

// clang-format off
DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_ShortScript, 22)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_MediumScript, 44)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_LongScript, 126)

u32 gData_020050d0[] = {
    0, 230, 22, 38, 0, 3,
    0, 1, 38,
};

u32 gData_020050f4[] = {
    0, 25, 22, 38, 0, 3,
    0, 1, 38,
};

DEFINE_G_DATA_02005118(gData_02005118, 2, 4)

DEFINE_G_DATA_02005118(gData_02005168, 4, 8)

u32 gData_020051b8[] = {
    SC_MOVE(PIXELS(744), 0, PIXELS(432)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u16 gData_020051d8[] = {
    42, 30, 44, 20, 46, 21, 47, 23,
    47, 24, 46, 26, 44, 27, 43, 27,
    41, 26, 40, 24, 40, 23, 41, 21,
};

u32 gData_02005208[] = {
    0, 6, 38, 17,
};

DEFINE_G_DATA_02005218(gData_02005218)

s32 gData_02005234[] = {
    SC_CALL(Orbiter_Spiral),
    SC_FREE,
    SC_END,
};

DEFINE_G_DATA_02005244(gData_02005244, Orbit_Update, Actor_AimAtWatched,
    Actor_StepToWatched)

DEFINE_G_DATA_02005264(gData_02005264, 24)

DEFINE_G_DATA_02005264(gData_02005288, 16)

DEFINE_G_DATA_020052AC(gData_020052ac, 24, 0)

DEFINE_G_DATA_020052AC(gData_020052dc, 16, 3)

u16 gData_0200530c[] = {
    0, 0, 113, 47, 115, 47, 115, 45,
    117, 45, 119, 45, 119, 47, 121, 47,
    117, 49, 119, 49, 119, 51, 121, 51,
};

u32 gData_0200533c[] = {
    0x33002d, 0x35001f, 0x39001f, 0x3b0027, 0x3b0029, 0x3f002b,
    0x3f002d, 0x450023, 0x450025,
};

u32 gData_02005360[] = {
    0, 1, 13, 44, 0, 0, 1, 2, 0, 0, 0, 0, 1, 38, 17,
};

u32 gData_0200539c[] = {
    0, 1, 13, 44, 0, 0, 1, 2, 0, 0, 0, 0, 1, 38, 17,
};

u32 gData_020053d8[] = {
    0, 1, 17,
};

u32 gData_020053e4[] = {
    0x10043f42, 0x1006001e, 0x1008001e, 0x1006001e, 0xfe00001e, 0xffff,
};

u32 gData_020053fc[] = {
    0x1010000, 0x1210111, 0xc310131, 0x22311731, 0x2d212d31, 0x2d012d11,
    0x17012201, 0xc01,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 24, 0, 408, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gData_02005458[] = {
    0xffe202bc, 0x2d0014a, 0x15efff6, 0x8ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(177),
    SCENE_EXIT(1, 177, 4),
    SCENE_EXIT(2, 177, 3),
    SCENE_EXIT(3, 177, 2),
    SCENE_EXIT(4, 177, 1),
    SCENE_EXIT(9, 178, 4),
    SCENE_EXIT(21, 181, 4),
    SCENE_EXIT(22, 179, 3),
    SCENE_EXIT(23, 179, 3),
    SCENE_EXIT(24, 179, 3),
    SCENE_EXIT(250, 178, 4),
    SCENE_EXIT(251, 177, 9),
    SCENE_EXITS(178),
    SCENE_EXIT(1, 174, 4),
    SCENE_EXIT(2, 178, 3),
    SCENE_EXIT(3, 178, 2),
    SCENE_EXIT(4, 177, 1),
    SCENE_EXIT(5, 178, 6),
    SCENE_EXIT(6, 178, 5),
    SCENE_EXIT(7, 180, 1),
    SCENE_EXIT(8, 178, 9),
    SCENE_EXIT(9, 178, 8),
    SCENE_EXIT(10, 180, 2),
    SCENE_EXITS(179),
    SCENE_EXIT(1, 180, 3),
    SCENE_EXIT(2, 181, 7),
    SCENE_EXIT(3, 181, 6),
    SCENE_EXIT(20, 177, 20),
    SCENE_EXITS(180),
    SCENE_EXIT(1, 178, 10),
    SCENE_EXIT(2, 178, 7),
    SCENE_EXIT(3, 179, 1),
    SCENE_EXITS(181),
    SCENE_EXIT(1, 181, 2),
    SCENE_EXIT(2, 181, 1),
    SCENE_EXIT(3, 181, 4),
    SCENE_EXIT(4, 181, 3),
    SCENE_EXIT(5, 181, 6),
    SCENE_EXIT(6, 181, 5),
    SCENE_EXIT(7, 179, 2),
    SCENE_EXIT(8, 179, 3),
    SCENE_EXIT(9, 179, 6),
    SCENE_EXIT(20, 177, 20),
    SCENE_EXITS_END,
};

u32 gData_02005528[] = {
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_Y, 0),
    SC_END,
};

s32 gData_02005544[] = {
    22,
    15,
    0x10000,
    22,
    30,
    129,
    22,
    26,
    0,
    22,
    2,
    0x3000,
    2,
    0x1c80000,
    -0x200000,
    0x1c80000,
    22,
    22,
    -0x200000,
    46,
    (s32)Func_020005dc,
    17,
};

u32 gData_0200559c[] = {
    SC_SET(SW_SPEED, 0x4000),
    SC_MOVE_BY(PIXELS(16), 0, 0),
    SC_SYNC,
    SC_END,
};

s32 gData_020055c0[] = {
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_COLLISION, 129),
    SC_SET(SW_MOTION, 3),
    SC_MOVE(PIXELS(472), PIXELS(-32), PIXELS(344)),
    SC_SYNC,
    SC_SET(SW_COLLISION, 0),
    SC_CALL(Func_020005dc),
    SC_END,
};

s32 gData_02005610[] = {
    SC_SET(SW_SPEED, 0x10000),
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_COLLISION, 0),
    SC_MOVE_BY(PIXELS(4), PIXELS(8), 0),
    SC_SYNC,
    SC_LABEL(1),
    SC_CALL(Func_020005f8),
    SC_WAIT(2),
    SC_LOOP(8, SC_TO(1)),
    SC_CMD_2A(197),
    SC_CALL(Func_02000678),
    SC_WAIT(240),
    SC_LABEL(2),
    SC_CALL(Func_02000610),
    SC_WAIT(1),
    SC_LOOP(8, SC_TO(2)),
    SC_MOVE_BY(PIXELS(-4), PIXELS(-8), 0),
    SC_SYNC,
    SC_SET(SW_MOTION, 3),
    SC_END,
};

DEFINE_G_DATA_020056C4(gData_020056c4, 424, 344)

DEFINE_G_DATA_020056C4(gData_02005728, 512, 384)

s32 gData_0200578c[] = {
    SC_SET(SW_SPEED, 0x10000),
    SC_SET(SW_HOOK, (s32)Func_02000900),
    SC_SET(SW_COLLISION, 129),
    SC_LABEL(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_CALL(Func_020006e0),
    SC_WAIT(15),
    SC_SET(SW_SPEED, 0x4000),
    SC_SET(SW_STEERING, 0),
    SC_ARG(9),
    SC_CALL(Func_020006f8),
    SC_MOVE_BY(PIXELS(16), 0, 0),
    SC_SYNC,
    SC_SET(SW_SPEED, 0x10000),
    SC_SET(SW_STEERING, 1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ARG(2),
    SC_MOVE_BY(0, 0, PIXELS(16)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_WEST),
    SC_MOVE_BY(PIXELS(-16), 0, 0),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTH),
    SC_SET(SW_SPEED, 0x8000),
    SC_CALL(Func_02000718),
    SC_ARG(7),
    SC_WAIT(15),
    SC_MOVE(PIXELS(472), 0, PIXELS(360)),
    SC_SYNC,
    SC_WAIT(10),
    SC_ARG(0),
    SC_CALL(Func_02000748),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(260),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_CALL(Func_020008b4),
    SC_WAIT(10),
    SC_SET(SW_SPEED, 0x10000),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ARG(2),
    SC_MOVE(PIXELS(472), 0, PIXELS(456)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_WEST),
    SC_ARG(0),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

struct ActorPlacement gGaiaRockInside_Placements[] = {
    {394, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(104), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gGaiaRockInside_LairPlacements[] = {
    {148, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(328), FACING_SOUTH, 2, 0},
    {398, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {29, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(456), FACING_SOUTH, 2, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {323, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

u32 gData_02005a30[] = {
    SC_SET(SW_SCALE_X, 0xc000),
    SC_SET(SW_SCALE_Y, 0xc000),
    SC_WAIT(1),
    SC_END,
};

struct ActorPlacement gGaiaRockInside_Room2Placements[] = {
    {388, CONDITION_ALWAYS, 1, PIXELS(408), 0, PIXELS(688), FACING_SOUTH, 2, 0},
    {388, CONDITION_ALWAYS, 1, PIXELS(584), 0, PIXELS(688), FACING_SOUTH, 2, 0},
    {273, CONDITION_ALWAYS, (s32)gData_02005a30, PIXELS(920), 0, PIXELS(360), FACING_SOUTH, 2, 0},
    {273, CONDITION_ALWAYS, (s32)gData_02005a30, PIXELS(952), 0, PIXELS(408), FACING_SOUTH, 2, 0},
    {385, CONDITION_ALWAYS, 1, PIXELS(496), 0, PIXELS(728), FACING_SOUTH, 2, 0},
    {397, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gGaiaRockInside_Room3Placements[] = {
    {388, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(352), FACING_SOUTH, 2, 0},
    {388, CONDITION_ALWAYS, 1, PIXELS(584), 0, PIXELS(352), FACING_SOUTH, 2, 0},
    {385, CONDITION_ALWAYS, 1, PIXELS(528), 0, PIXELS(384), FACING_SOUTH, 2, 0},
    {385, CONDITION_ALWAYS, 1, PIXELS(672), 0, PIXELS(384), FACING_SOUTH, 2, 0},
    {397, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

DEFINE_G_DATA_020057C0(gGaiaRockInside_Room4Placements, 320, 0, 0, 0, 0, 320, 0, 320)

struct ActorPlacement gGaiaRockInside_Room5Placements[] = {
    {388, CONDITION_ALWAYS, 1, PIXELS(400), 0, PIXELS(368), FACING_SOUTH, 2, 0},
    {385, CONDITION_ALWAYS, 1, PIXELS(336), 0, PIXELS(408), FACING_SOUTH, 2, 0},
    {372, CONDITION_ALWAYS, 1, PIXELS(680), 0, PIXELS(648), FACING_SOUTH, 2, 0},
    {378, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {378, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {378, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {397, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {385, CONDITION_ALWAYS, (s32)gData_02005528, PIXELS(704), 0, PIXELS(384), FACING_SOUTH, 2, 0},
    {347, CONDITION_ALWAYS, (s32)gData_02005528, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

s32 gData_02005d6c[] = {
    SC_LABEL(1),
    SC_ADD(SW_SCALE_Y, 0x1800),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_Y, -0x1800),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(2)),
    SC_LABEL(3),
    SC_ADD(SW_SCALE_Y, 0x1000),
    SC_WAIT(1),
    SC_LOOP(2, SC_TO(3)),
    SC_LABEL(4),
    SC_ADD(SW_SCALE_Y, -4096),
    SC_WAIT(1),
    SC_LOOP(2, SC_TO(4)),
    SC_ADD(SW_SCALE_Y, 0x1000),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_Y, -4096),
    SC_WAIT(1),
    SC_END,
};

s32 gData_02005e28[] = {
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 0x1800),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_X, -0x1800),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(2)),
    SC_LABEL(3),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_WAIT(1),
    SC_LOOP(2, SC_TO(3)),
    SC_LABEL(4),
    SC_ADD(SW_SCALE_X, -4096),
    SC_WAIT(1),
    SC_LOOP(2, SC_TO(4)),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, -4096),
    SC_WAIT(1),
    SC_END,
};

u32 gData_02005ee4[] = {
    0,
};

struct Trigger gGaiaRockInside_Triggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(Func_0200485c)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(Func_0200485c)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(Func_0200485c)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(Func_0200485c)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 8, 0x200, EVENT(Func_02000af8)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGaiaRockInside_LairTriggers[] = {
    {TRIGGER_RAISED, 42, CONDITION_ALWAYS, EVENT(GaiaRockInside_SusaSaysMoveAside)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_STEP, 40, 0x9de, EVENT(GaiaRockInside_MeetSusa)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(GaiaRockInside_TalkToSusa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, EVENT(GaiaRockInside_ReadSusasMind)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(GaiaRockInside_ReadSandTablet)},
    {0x53, 50, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGaiaRockInside11))},
    {TRIGGER_EXAMINE, 55, CONDITION_ALWAYS, EVENT(GaiaRockInside_ChallengeSerpent)},
    {TRIGGER_EXAMINE, 56, CONDITION_ALWAYS, EVENT(GaiaRockInside_ChallengeSerpent)},
    {0x53, 77, 0xe41, EVENT(Func_02001fa0)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(Leader_SetLayerByte)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_02001f84)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02005fcc[] = {
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_STEP, 42, CONDITION_ALWAYS, EVENT(Func_02001d14)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(GaiaRockInside_ReadSandTablet)},
    {0x53, 50, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGaiaRockInside11))},
    {TRIGGER_EXAMINE, 55, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGaiaRockInside08))},
    {0x53, 77, 0xe41, EVENT(Func_02001fa0)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(Leader_SetLayerByte)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_02001f84)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGaiaRockInside_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {0x21, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {0x21, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(GaiaRockInside_CheckAltar)},
    {0x1c314, 0x40c, CONDITION_ALWAYS, EVENT(GaiaRockInside_PlaceDollOnFirstAltar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS, EVENT(Func_02002974)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, CONDITION_ALWAYS, EVENT(Func_02002974)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_02002974)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGaiaRockInside_Room3Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0x21, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(GaiaRockInside_CheckAltar)},
    {0x1c314, 0x40a, CONDITION_ALWAYS, EVENT(GaiaRockInside_PlaceDollOnWestAltar)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(GaiaRockInside_CheckAltar)},
    {0x1c314, 0x40b, CONDITION_ALWAYS, EVENT(GaiaRockInside_PlaceDollOnEastAltar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 20, CONDITION_ALWAYS, EVENT(Func_02000b6c)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), -1, 0, 0},
};

struct Trigger gGaiaRockInside_Room4Triggers[] = {
    {0x31, 1, CONDITION_ALWAYS, 1},
    {0x31, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 20, CONDITION_ALWAYS, EVENT(Func_02000c38)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGaiaRockInside_Room5Triggers[] = {
    {0xce01, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0xce01, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {0xce01, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {0x31, 7, CONDITION_ALWAYS, 7},
    {0x4602, 43, CONDITION_ALWAYS, EVENT(Func_0200036c)},
    {TRIGGER_STEP, 30, 0x303, EVENT(Func_02002a14)},
    {TRIGGER_STEP, 31, 0x304, EVENT(Func_02002a24)},
    {TRIGGER_STEP, 32, 0x305, EVENT(Func_02002a34)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(GaiaRockInside_CheckAltar)},
    {0x1c314, 0x409, CONDITION_ALWAYS, EVENT(GaiaRockInside_PlaceDollOnRoom5Altar)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 10, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 40, CONDITION_ALWAYS, EVENT(Func_02000cd8)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 41, CONDITION_ALWAYS, EVENT(Func_02000cf8)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 42, CONDITION_ALWAYS, EVENT(Func_02000d18)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 43, CONDITION_ALWAYS, EVENT(Func_02000d18)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 44, CONDITION_ALWAYS, EVENT(Func_02000c74)},
    {TRIGGER_TABLE_END},
};

u32 gData_02006290[] = {
    0, 40, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

DEFINE_G_DATA_020016CC(gData_020062d4)
// clang-format on
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef EFFECT_SPRITE_PRIORITY
#undef FIELD_AT
#undef BYTE_AT
#undef FIELD_AT
#undef FIELD_AT
#undef FIELD_AT
#undef Test
#undef C1
#undef C2
#undef C3
#undef C4
#undef C6
#undef FIELD_AT
#undef GRID_WIDTH
#undef gPath
#undef AT
#undef AT_ADDRESS
#undef GAIA_ROCK_INSIDE_PATH
#undef gData_02005ee4
#undef gData_020051d8
#undef gData_02005264
#undef gData_02005218
#undef gData_02005288
#undef gData_02005244
#undef gData_020052ac
#undef gData_020052dc
#undef gData_0200530c
#undef gSceneState
#undef gData_0200533c
#undef gData_0200530c
#undef gData_02005360
#undef gData_0200539c
#undef gData_020053d8
#undef gData_0200578c
#undef gData_0200530c
#undef gData_02005360
#undef gData_0200539c
#undef gData_020053d8
#undef gData_0200578c
#undef Func_02000aac
#undef Func_02000a60
#undef gData_020053fc
#undef gData_020062d4

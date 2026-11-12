/// @file
/// The scene code of overlay 0x671 that its scene tables belong to, in ROM order; the rest of the overlay is in gabomba_statue/.

#include "field/field.h"

#include "types.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "field_effect.h"
#include "event_inline.h"

extern void Func_02005624(struct FieldEffect *effect, s32 mode);
extern void Func_02005634(struct FieldEffect *effect, const s32 *script);
extern const s32 *gData_02005ae4[];

void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options) {
  struct FieldEffect *party = ((struct FieldEffect *)Actor_GetById(gMainRoutineWork.currentOwner));
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  u32 start;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = ((struct FieldEffect *)Import_ActorCreate(options->type, x, y, z));
  else
    effect = ((struct FieldEffect *)Import_ActorCreate(285, x, y, z));
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Func_02005624(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Func_02005634(effect, gData_02005ae4[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSingleChildFlags(effect, 0);
  effect->update = BattleEffect_UpdateObjectMotionScaleAndLinkedAngle;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect, party->sprite->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    Actor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  start = flags & EFFECT_USE_START_SCALE;
  if (start != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 targetX;
    s32 count;

    script = gData_02005ae4[flags & EFFECT_SCRIPT_MASK];
    targetX = options->targetScaleX;
    count = script[3];
    if (start != 0) {
      effect->scaleRateX = (targetX - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (targetX - 0x10000) / count;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Func_02005624(effect, 1);
    Func_02005634(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

struct Wanderer {
  u8 unk00[6];
  u16 facing;
  u8 unk08[90];
  u8 timer;
  u8 turned;
  u16 facingA;
  u16 facingB;
};

u32 Random16Far(void);

void Func_02000280(void) { Func_080d2b4c(33, 9); }
s32 Func_0200028c(struct Wanderer *actor) {
  u8 *timer = &actor->timer;

  if (*timer != 0) {
    (*timer)--;
  } else {
    u8 *turned;
    u16 *facing;

    *timer = (Random16Far() * 40 >> 16) + 10;
    turned = &actor->turned;
    if (*turned != 0) {
      *turned = 0;
      /* FAKEMATCH(dead): an empty once-only block, so the flag is cleared
       * before the facing is addressed; without it, 2 of 35 instructions differ
       * in tla-en. */
      do {
      } while (0);
      facing = &actor->facingA;
    } else {
      *turned = 1;
      facing = &actor->facingB;
    }
    actor->facing = *facing;
  }
  return 1;
}

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

#include "scene_ids.h"

extern const u8 gData_02005dfc[];
extern const u8 gData_02005e2c[];
extern const u8 gData_02005e4c[];
extern const u8 gData_02005e8c[];
extern const u8 gData_02005ecc[];
extern const u8 gData_02005eec[];

const u8 *Func_020002dc(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov671Room7)
    return gData_02005dfc;
  if (scene == (s32)&SceneId_Ov671Room1)
    return gData_02005e2c;
  if (scene == (s32)&SceneId_Ov671Room2)
    return gData_02005e4c;
  if (scene == (s32)&SceneId_Ov671Room3)
    return gData_02005e8c;
  if (scene == (s32)&SceneId_Ov671Room4)
    return gData_02005ecc;
  if (scene == (s32)&SceneId_Ov671Room5)
    return gData_02005eec;
  return NULL;
}

#include "frame.h"
#include "field_event.h"

const u32 *Scene_GetExits(void) { return gSceneExits; }

struct FieldPartBytes {
  u8 unk00[5];
  u8 frame;
};

struct FieldAnimation {
  u8 unk00[40];
  struct FieldPartBytes *part;
};

s32 Func_02000364(struct FieldActor *actor) {
  u32 frame = gFrameCount;

  if (frame % 10 == 0)
    ((struct FieldAnimation *)actor->sprite)->part->frame = frame % 20 / 10 + 3;
  return 1;
}

#include "macros.h"

struct Orbiter {
  u8 unk00[6];
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[15];
  u8 flags23;
  u8 unk24[62];
  u8 phase;
  u8 unk63;
  s16 centerX;
  s16 centerZ;
  void *link;
};

extern s32 gData_02005af0;
s32 __divsi3(s32 numerator, s32 denominator);

void Func_02000398(struct Orbiter *actor, s32 *x, s32 *z) {
  s32 position[3];
  s32 angle = (actor->phase << 16) / 180;
  s32 turn;

  SET_VECTOR(position, (actor->centerX + 16) << 16, 0x100000,
             actor->centerZ << 16);
  turn = gData_02005af0 * 0x3000 + angle;
  FieldVector_AddPolarOffset(0xd0000, turn + 0x8000, position);
  *x = position[0];
  *z = position[2];
}

s32 Func_02000408(struct Orbiter *actor) {
  u8 *phase = &actor->phase;
  s32 angle = (*phase << 16) / 180;
  s32 next;

  actor->flags23 = 0;
  SET_POSITION(actor, (actor->centerX + 16) << 16, 0x100000,
               actor->centerZ << 16);
  FieldVector_AddPolarOffset(0xd0000, angle + 0x8000, &actor->x);
  next = *phase + gData_02005af0;
  if (next < 0)
    next = 179;
  if (next > 179)
    next = 0;
  *phase = next;
  actor->link = Func_02000398;
  return 1;
}

#include "ram_buffer.h"
#include "actor.h"

struct PuzzleCell {
  u8 kind;
  u8 blocked;
  u8 state;
  u8 occupied;
};

struct SceneState {
  struct PuzzleCell cells[88];
  u8 unk160[4];
  s16 firstId;
  s16 endId;
  u16 tileEntry;
  s8 redrawA;
  s8 redrawB;
  s16 originX;
  s16 originZ;
};

struct EventWork1cc {
  u8 unk000[0x1cc];
  struct Actor *object;
};

struct IoReg16 {
  u16 value;
};

#define REG_BLDALPHA ((struct IoReg16 *)0x04000052)
#define OBJECT_BYTE(object, offset) (((u8 *)(object))[offset])

extern struct SceneState gSceneState;
extern const s8 gData_02005940[];
extern const u8 gData_02005948[];

s32 Resource_FindFreeEntry(void);
struct Actor *Func_020005b4(struct Actor *origin, s32 id);
void Func_02001950(s32 show, s32 first, u32 count);
void Func_02002394(void);
void Func_02004380(s32 x, s32 z, s32 a);
void Func_020055cc(const u8 *data, void *buffer);
void Func_0200561c(void *animation, s32 a);
void Func_020056b4(struct Actor *object);
void Func_020056bc(void);
void Func_02005824(void);
void Func_0200582c(s32 index, s32 a, s32 b);
void Func_0200583c(s32 index, s32 flag, s32 a, s32 b);

s32 Func_0200047c(struct Actor *object) {
  struct Actor *owner = object->watchedObject;

  if (owner != NULL) {
    u8 *phase = &OBJECT_BYTE(object, 98);

    object->x = owner->x;
    object->y = owner->y + 0x100000;
    object->z = owner->z;
    object->angle = *phase << 8;
    FieldVector_AddPolarOffset(0x60000, *phase << 8, &object->x);
    *phase += 16;
  }
  return 1;
}

extern const u8 gData_02006b20[];
extern const u8 gData_02005fd4[];
extern const u8 gData_02006004[];
extern const u8 gData_02006244[];
extern const u8 gData_020064cc[];
extern const u8 gData_020067e4[];
extern const u8 gData_02006898[];
extern const u8 gData_02006a90[];

const u8 *Func_020004b8(void) {
  s16 scene = gMainRoutineWork.scene;
  const u8 *table = gData_02006b20;

  if (scene == (s32)&SceneId_Ov671Room1)
    table = gData_02005fd4;
  else if (scene == (s32)&SceneId_Ov671Room2)
    table = gData_02006004;
  else if (scene == (s32)&SceneId_Ov671Room3)
    table = gData_02006244;
  else if (scene == (s32)&SceneId_Ov671Room4)
    table = gData_020064cc;
  else if (scene == (s32)&SceneId_Ov671Room5)
    table = gData_020067e4;
  else if (scene == (s32)&SceneId_Ov671Room6)
    table = gData_02006898;
  else if (scene == (s32)&SceneId_Ov671Room7)
    table = gData_02006a90;
  return table;
}

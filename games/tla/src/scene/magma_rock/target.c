#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "magma_rock.h"
#include "macros.h"
#include "event_inline.h"

void Func_02000ff4(void);
void Func_020011e4(struct Actor *object);
void Func_02003820(s32 x, s32 y, s32 z, s32 kind);
void Func_020038dc(void);
void Func_02003998(void);
void Func_02003a54(struct Actor *object, s32 a);
void Func_02003aec(void);
void Func_02003af4(void);

void Func_02001444(void) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;
  struct Actor *object;

  Func_02003aec();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_Wait(10);
  object = work->target;
  object->update = NULL;
  Actor_SetSpriteColor(object, 7);
  FieldEvent_Wait(3);
  Actor_SetSpriteColor(work->target, 0);
  FieldEvent_Wait(1);
  Func_020011e4(work->target);
  Func_02003af4();
}

void Func_0200148c(void) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;
  struct Actor *object;

  Func_02003aec();
  FieldEvent_ResolvePendingActions(0);
  object = work->target;
  object->action = 0x400;
  object->update = Func_02000ff4;
  Func_02003af4();
}

void Func_020014bc(s32 mode, s32 cue) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;

  if (cue == 0)
    Func_020038dc();
  if (mode == 2) {
    struct Actor *object = work->target;

    Func_02003820(object->x, object->y + 0xf0000, object->z, 1);
  }
  if (mode == 3) {
    struct Actor *object = work->target;
    struct Actor *target;

    Func_02003820(object->x, object->y + 0xf0000, object->z, 8);
    target = work->target;
    SET_POSITION(target, 0, 0, 0);
  }
  if (cue == 0x3e7)
    Func_02003998();
  if (cue == 178)
    Func_02003a54(work->target, 3);
}

// One of the four parts Func_02001738 builds: rises and falls with its owner's action counter.

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

/// A 16.16 position word, read whole or by halves.
union Position {
  s32 word;
  s16 half[2];
};

#define PART_Y(part) (((union Position *)&(part)->y)->word)

void Func_02003a6c(struct Actor *part);

/// One of the four parts Func_02001738 builds: rises and falls with its owner's
/// action counter.
void Func_02001528(struct Actor *part) {
  struct Actor *owner = part->watchedObject;

  if (owner->action > 35 && owner->action < BYTE_AT(part, 98) * 2 + 36) {
    PART_Y(part) = owner->y + 0x200000;
    PART_Y(part) += (owner->action << 19) + (-35 << 19);
  } else if (BYTE_AT(part, 98) * 2 + 36 < owner->action && owner->action < 332) {
    PART_Y(part) = owner->y + (BYTE_AT(part, 98) << 20) + 0x200000;
  } else if (owner->action > 332 && owner->action < BYTE_AT(part, 98) * 2 + 333) {
    PART_Y(part) = owner->y + (BYTE_AT(part, 98) << 20) + 0x200000;
    PART_Y(part) -= (owner->action << 19) - (332 << 19);
  } else if (BYTE_AT(part, 98) * 2 + 333 < owner->action) {
    Func_02003a6c(part);
  }
}

// The fifth part of Func_02001738's group: follows its owner like the others, throws effect 403 every eighth frame and marks the ground beneath.
#include "system/field_effect.h"
#include "system/main_routine_work.h"
#include "frame.h"

struct MagmaRockEffectConfig {
  s32 kind;
  s32 variant;
  s32 unk08;
  s32 unk0c;
  u8 unk10[8];
  u16 id;
  u8 unk1a[2];
  const void *data;
  u8 unk20[4];
  void (*unk24)(void);
};

extern u32 gData_02003d50[];
void Func_020011b8(void);
u32 Random16Far(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   struct MagmaRockEffectConfig *config);
void Func_020005cc(s32 layer, s32 x, s32 z, s32 height);

#define EFFECT_Y(object) (((union Position *)&(object)->effect.y)->word)

/// The fifth part of Func_02001738's group: follows its owner like the others,
/// throws effect 403 every eighth frame and marks the ground beneath.
void Func_020015e4(union FieldObject *object) {
  struct Actor *watched = (struct Actor *)object->effect.countdown;
  struct MagmaRockEffectConfig config;
  s32 step;
  s32 mask;

  if (watched->action >= 35 && watched->action <= 42) {
    EFFECT_Y(object) = watched->y + 0x180000;
    EFFECT_Y(object) = EFFECT_Y(object) + (watched->action << 19) + (-34 << 19);
  } else {
    if (watched->action >= 9 && watched->action <= 331) {
      EFFECT_Y(object) = watched->y + (object->effect.unk5a[8] << 20) + 0x180000;
    } else if (watched->action > 332 &&
               watched->action < (object->effect.unk5a[8] << 1) + 333) {
      EFFECT_Y(object) = watched->y + (object->effect.unk5a[8] << 20) + 0x180000;
      EFFECT_Y(object) -= (watched->action << 19) - (332 << 19);
    } else if ((object->effect.unk5a[8] << 1) + 333 < watched->action) {
      object->effect.y = object->actor.terrainHeight;
      object->effect.update = 0;
    }
  }

  mask = 7;
  step = gFrameCount & mask;
  if (step == 0) {
    config.id = 403;
    config.variant = 10;
    config.kind = 2;
    config.unk24 = Func_020011b8;
    config.data = gData_02003d50;
    Func_0200015c(object->effect.x + 0x10000, object->effect.y - 0x40000,
                  object->effect.z, ((Random16Far() & mask) - 3) << 14,
                  -0x20000, step, 0x1330000, &config);
  }
  Func_020005cc(watched->terrainId, watched->x >> 20, watched->z >> 20,
                (object->effect.y >> 19) + 1);
}

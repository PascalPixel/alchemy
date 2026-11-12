/// @file
/// Scene code of overlay 0x64d, in ROM order, from s23a4's offset up to the next object the overlay links.

#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "event_inline.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

struct GlowState {
  s32 x; ///< 16.16 pixels
  s32 y; ///< 16.16 pixels
  s32 z; ///< 16.16 pixels
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 frame;
};

struct GlowSprite {
  u8 unk00[4];
  u16 attr0 : 13;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 attr1;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[20];
  u16 unk1e;
  u8 unk20[8];
};

struct GlowWork {
  u8 unk000[0x400];
  struct GlowSprite sprites[16];
  struct GlowState states[16];
  struct Actor *actor;
  struct Actor *effect;
  s32 x;
  s32 y;
  s32 z;
  s16 stage;
  s16 timer;
  u16 tileTop;
  u16 tile;
  s16 height;
  s8 follow;
};

struct GlowAnimation {
  u32 unk00[4];
  u32 unk10 : 16;
  u32 angle : 16;
};

extern u8 gField_Tiles64D[];

void *HeapSlot_AllocFar(s32 slot, s32 size);
void Func_02002824(void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 Resource_ResetEntry(u32 slot);
void Func_02002a0c(struct GlowSprite *sprite, s32 width, s32 height, s32 attr,
                   s32 tile);
s32 Func_02002a24(struct Actor *actor);
s32 Func_02002a2c(struct Actor *actor);
void Func_0200285c(struct Actor *actor, s32 mode);
void Func_02002874(struct Actor *actor);
void Func_02001fd0(void);

void Func_020023a4(s32 id) {
  struct GlowWork *work = HeapSlot_AllocFar(220, 0x860);
  struct Actor *actor = Actor_Get(id);
  struct Actor *effect;
  struct GlowSprite *sprite;
  struct GlowState *state;
  s32 resource, tile, i;
  u8 zero = 0; // Holds 0 for its whole life.

  actor->movementState = zero;
  work->actor = actor;
  work->x = actor->x;
  work->y = actor->y;
  work->z = actor->z + 0x20000;
  Func_02002824(gField_Tiles64D, work);
  resource = Resource_FindFreeEntry();
  tile = Vram_Load(resource, 0x800, work);
  work->tile = tile;
  work->tileTop = tile + 32;
  state = work->states;
  sprite = work->sprites;
  for (i = 15; i >= 0;) {
    Func_02002a0c(sprite, 8, 8, 0x40000000, tile);
    sprite->unk1e = Func_02002a2c(actor) - 2;
    sprite->priority = Func_02002a24(actor);
    sprite->fullColor = 1;
    sprite->palette = 0;
    state->frame = -1;
    i--;
    sprite++;
    state++;
  }
  effect =
      (FieldActor_Create(290, actor->x, actor->y, actor->z));
  work->effect = effect;
  Func_0200285c(effect, 1);
  Actor_SetSingleChildFlags(effect, 0);
  effect->priorityFlags = 2;
  Actor_SetSingleChildFlags(actor, 0);
  ((struct GlowAnimation *)actor->animation)->angle = 0x4000;
  actor->x += -0xc0000;
  actor->motionFlags = 0;
  work->stage = 0;
  work->timer = 0;
  work->height = 0;
  work->follow = 1;
  Event_PlaySound(221);
  Task_AddFrame(Func_02001fd0, 0x480);
  if (work->stage != 999) {
  wait:
    System_Wait(1);
    if (work->stage != 999)
      goto wait;
  }
  FieldTask_Remove(Func_02001fd0);
  Func_02002874(effect);
  Resource_ResetEntry(resource);
  HeapSlot_Release(220);
}

struct Unk_0200258cState {
  u8 unk00[24];
  s32 frame;
};

struct Unk_0200258cSprite {
  u8 unk00[8];
  u16 tile : 10;
  u16 attributes : 6;
  u8 unk0a[30];
};

struct Unk_0200258cWork {
  u8 unk0000[0x800];
  struct Unk_0200258cState states[96];
  struct Unk_0200258cSprite sprites[96];
  u16 unk2180;
  u16 unk2182;
};

void Func_02002a14(struct Unk_0200258cSprite *sprite, struct Unk_0200258cState *state);
void Func_02002a1c(struct Unk_0200258cState *state, s32 a, s32 b);

void Func_0200258c(void) {
  struct Unk_0200258cWork *work = Ram_HeapSlots->menuRuntime;
  struct Unk_0200258cState *state = work->states;
  struct Unk_0200258cSprite *sprite = work->sprites;
  s32 value;
  s32 i;

  for (i = 0; i < 96; i++, sprite++, state++) {
    value = state->frame;
    if ((u32)value < 16) {
      sprite->tile = work->unk2182 + ((value >> 1) & 7) * 8;
      Func_02002a14(sprite, state);
      Func_02002a1c(state, 64, 0);
      state->frame++;
    }
  }
}

struct SparkState {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 frame;
};

struct SparkSprite {
  u8 unk00[8];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[20];
  u16 unk1e;
  u8 unk20[8];
};

struct SparkWork {
  u8 unk0000[0x800];
  struct SparkState states[96];
  struct SparkSprite sprites[96];
  s16 next;
  u16 tileBase;
};

void Func_0200260c(struct Actor *source, s32 x, s32 y, s32 z) {
  struct SparkWork *work = Ram_HeapSlots->menuRuntime;
  s32 angle = Random16Far();
  s16 *next = &work->next;
  struct SparkSprite *sprite = &work->sprites[*next];
  struct SparkState *state = &work->states[*next];

  sprite->priority = Func_02002a24(source);
  sprite->unk1e = Func_02002a2c(source);
  state->frame = 0;
  state->x = x;
  state->y = y;
  state->z = z;
  FieldVector_AddPolarOffset(Random16Far() * 2 + 0x20000, angle, &state->x);
  state->velocityX = 0;
  state->velocityY = (Random16Far() >> 1) + 0xc000;
  state->velocityZ = 0;
  FieldVector_AddPolarOffset(Random16Far() >> 2, angle, &state->velocityX);
  *next = *next + 1;
  *next = *next % 96;
}

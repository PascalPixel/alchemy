// Draft, differ 50 of 215 (was 94): the Venus Lighthouse entrance glow:
// takes heap slot 220, loads gField_Tiles64D's tiles, sets up its sixteen
// sprites and effect 290 over object id, then runs Func_02001fd0 until
// stage 999. The wait for stage 999 is a goto loop (the native rebuilds the
// stage's address and 999 in each pass), and the object's angle is a
// 16-bit bitfield of its animation, so 0x4000 is built with movs rather
// than loaded as a halfword constant. Left: register choice in the sprite
// loop's bitfield stores (r1 and r2 swapped) and the order of three setup
// loads. Tried: statement moves and four permuter minutes (48 by its count,
// no closer form).
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

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
  struct ObjectRuntime *object;
  struct ObjectRuntime *effect;
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
  u8 unk00[0x10];
  u32 unk10 : 16;
  u32 angle : 16;
};

extern u8 gField_Tiles64D[];

void *HeapSlot_AllocFar(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Func_02002824(void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void Resource_ResetEntry(s32 entry);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
void Func_02002a0c(struct GlowSprite *sprite, s32 width, s32 height, s32 attr,
                   s32 tile);
s32 Func_02002a24(struct ObjectRuntime *object);
s32 Func_02002a2c(struct ObjectRuntime *object);
struct ObjectRuntime *Func_0200286c(s32 kind, s32 x, s32 y, s32 z);
void Func_0200285c(struct ObjectRuntime *object, s32 mode);
void Func_02002874(struct ObjectRuntime *object);
void Import_SoundPlay(s32 cue);
void WaitFrames(s32 frames);
s32 Task_Add(void (*callback)(void), s32 order);
void Task_RemoveFar(void (*callback)(void));
void Func_02001fd0(void);

void Func_020023a4(s32 id) {
  struct GlowWork *work = HeapSlot_AllocFar(220, 0x860);
  struct ObjectRuntime *object = Object_GetById(id);
  struct ObjectRuntime *effect;
  struct GlowSprite *sprite;
  struct GlowState *state;
  s32 resource, tile, i;

  object->movementState = 0;
  work->object = object;
  work->x = object->x;
  work->y = object->y;
  work->z = object->z + 0x20000;
  Func_02002824(gField_Tiles64D, work);
  resource = Resource_FindFreeEntry();
  tile = VramCache_Load(resource, 0x800, work);
  work->tile = tile;
  work->tileTop = tile + 32;
  state = work->states;
  sprite = work->sprites;
  for (i = 15; i >= 0; i--) {
    Func_02002a0c(sprite, 8, 8, 0x40000000, tile);
    sprite->unk1e = Func_02002a2c(object) - 2;
    sprite->priority = Func_02002a24(object);
    sprite->fullColor = 1;
    sprite->palette = 0;
    state->frame = -1;
    sprite++;
    state++;
  }
  effect = Func_0200286c(290, object->x, object->y, object->z);
  work->effect = effect;
  Func_0200285c(effect, 1);
  ObjectDispatch_SetSingleChildFlags(effect, 0);
  effect->priorityFlags = 2;
  ObjectDispatch_SetSingleChildFlags(object, 0);
  ((struct GlowAnimation *)object->animation)->angle = 0x4000;
  object->x += -0xc0000;
  object->motionFlags = 0;
  work->stage = 0;
  work->timer = 0;
  work->height = 0;
  work->follow = 1;
  Import_SoundPlay(221);
  Task_Add(Func_02001fd0, 0x480);
  if (work->stage != 999) {
  wait:
    WaitFrames(1);
    if (work->stage != 999)
      goto wait;
  }
  Task_RemoveFar(Func_02001fd0);
  Func_02002874(effect);
  Resource_ResetEntry(resource);
  HeapSlot_Release(220);
}

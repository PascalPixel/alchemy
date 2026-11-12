#include "event_inline.h"
#include "scene.h"

struct ChampaCliffCavesSprite {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 attr1;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[0x14];
  u16 unk1e;
  u8 unk20[8];
};

struct ChampaCliffCavesWork {
  u8 tiles[0x1000];
  s32 originX;
  s32 originY;
  s32 originZ;
  struct ChampaCliffCavesSprite spriteA;
  struct ChampaCliffCavesSprite spriteB;
  u16 entryA;
  u16 vramA;
  u16 entryB;
  u16 vramB;
  s32 field1064;
  s32 field1068;
  // A union: the two stores below keep their place after the sprite's.
  union {
    s32 value;
    u16 half;
  } field106c, field1070;
  u16 field1074;
  u8 unk1076[2];
};

void *Resource_GetTableEntry(s32 id);
void Func_020033dc(void *resource, void *destination);
s32 Resource_FindFreeEntry(void);
void Func_020035fc(struct ChampaCliffCavesSprite *sprite, s32 width, s32 height,
                   u32 attributes, s32 vram);

extern char ResourceId_FireColumn;

static __inline__ void Place(struct ChampaCliffCavesSprite *a, s32 b, s32 c,
                             u32 d, s32 e) {
  /* FAKEMATCH(wrapper): forwarded inline; as a direct call, 4 of 78
   * instructions differ in tla-en. */
  Func_020035fc(a, b, c, d, e);
}

void Func_02002d34(void) {
  struct ChampaCliffCavesWork *work = Ram_HeapSlots->menuRuntime;
  struct ChampaCliffCavesSprite *sprite;
  s32 i;
  u8 *tile;
  s32 entry;
  s32 vram;

  Func_020033dc(Resource_GetTableEntry((s32)&ResourceId_FireColumn), work);
  i = 0;
  tile = work->tiles;
next:
  if ((*tile & 0xf0) == 0xf0)
    *tile = (*tile & 0xf) + 0x60;
  i++;
  tile++;
  if (i < 0x1000)
    goto next;
  entry = Resource_FindFreeEntry();
  vram = VramCache_Load(entry, 0x800, work);
  work->entryB = entry;
  work->vramB = vram;
  sprite = &work->spriteB;
  Place(sprite, 15, 47, 0xc0008000, vram);
  sprite->fullColor = 1;
  sprite->palette = 1;
  sprite->priority = 1;
  sprite->unk1e = 1;
  work->field106c.value = 0;
  work->field1070.value = 0;
}

#include "types.h"
#include "ram_buffer.h"

// The glow's frame callback.
struct GlowSprite {
  u8 unk_0[12];
  s32 x;
  s32 z;
  s32 scale;
  s32 trailingScale;
  u8 unk_1C[12];
};

struct Glow {
  u8 unk_0[4096];
  s32 position[3];
  struct GlowSprite sprites[2];
  u8 unk_105C[8];
  s32 scale;
  s32 trailingScale;
  s32 scale2;
  s32 trailingScale2;
  s16 frame;
};

void Func_020035c4(s32 *position);
void Func_02003604(struct GlowSprite *sprite);

/// Cycles thirty-two colours of the object palettes through a wave of greens
/// and blues, places the glow's two sprites at its spot with their scales, and
/// lets each sprite's second scale trail its first at 31/40.
void Func_02002de4(void) {
  struct Glow *work = (struct Glow *)Ram_HeapSlots->menuRuntime;
  s32 position[3];
  struct GlowSprite *sprite;
  s32 i;

  i = 0;
loop:
  {
    s32 red = 31;
    s16 *frame = &work->frame;
    s32 wave = Math_Sine((*frame + i) << 11) * 6;
    s32 blue = Math_Sine(((*frame / 2 + i) << 11) + 0x4800) * 3;
    s32 colour;
    wave >>= 16;
    blue >>= 16;
    wave += 20;
    blue += 3;
    if (i <= 15) {
      colour = blue << 10 | wave << 5 | red;
      ((u16 *)0x05000200)[111 - i] = colour;
    } else {
      colour = blue << 10 | wave << 5 | red;
      ((u16 *)0x05000200)[255 - i] = colour;
    }
    i++;
  }
  if (i <= 31)
    goto loop;
  position[0] = work->position[0];
  position[1] = work->position[1];
  position[2] = work->position[2];
  sprite = &work->sprites[0];
  Func_020035c4(position);
  sprite->x = position[0];
  sprite->z = position[2];
  sprite->scale = work->scale;
  sprite->trailingScale = work->trailingScale;
  Func_02003604(sprite);
  position[0] = work->position[0];
  position[1] = work->position[1];
  position[2] = work->position[2];
  sprite = &work->sprites[1];
  Func_020035c4(position);
  sprite->x = position[0];
  sprite->z = position[2];
  sprite->scale = work->scale2;
  sprite->trailingScale = work->trailingScale2;
  Func_02003604(sprite);
  work->trailingScale = work->scale * 31 / 40;
  work->trailingScale2 = work->scale2 * 31 / 40;
  work->frame++;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

u8 *HeapSlot_AllocFar(s32 slot, s32 size);
void Func_02002c88(void);

void Func_02002f58(const s32 *position) {
  u8 *work = HeapSlot_AllocFar(220, 0x1078);
  s32 time;
  s32 state;

  FIELD_AT(work, s32, 4096) = position[0];
  FIELD_AT(work, s32, 4100) = position[1];
  FIELD_AT(work, s32, 4104) = position[2];
  Func_02002c88();
  Func_02002d34();
  FIELD_AT(work, u16, 4212) = 0;
  time = 0;
  {
    s32 priority = 0x480;

    Task_Add(Func_02002de4, priority);
  }
  MapGrid_SetShake(0x40000, 0x40000, 0x10000);
  state = 0;
  do {
    switch (state) {
    case 0:
      if ((FIELD_AT(work, s32, 4196) += 0x600) > 0x14ccb) {
        FIELD_AT(work, s32, 4196) = 0x14ccc;
        state = 1;
        time = -1;
      }
      break;
    case 1:
      if (time == 10) {
        state = 2;
        time = -1;
      }
      break;
    case 2:
      if ((FIELD_AT(work, s32, 4204) += 0x400) > 0x6665) {
        FIELD_AT(work, s32, 4204) = 0x6666;
        state = 3;
        time = -1;
      }
      break;
    case 3:
      if ((FIELD_AT(work, s32, 4204) += 0x3000) > 0x14ccb) {
        FIELD_AT(work, s32, 4204) = 0x14ccc;
        state = 999;
      }
      break;
    }
    System_Wait(1);
    time++;
  } while (state != 999);
}

s32 Resource_ResetEntry(u32 slot);
void HeapSlot_Release(s32 slot);

void Func_02003078(void) {
  u8 *work = Ram_HeapSlots->menuRuntime;
  s32 position[3];
  s32 state;
  s32 time;

  position[0] = FIELD_AT(work, s32, 4096);
  position[1] = FIELD_AT(work, s32, 4100) - 0x200000;
  state = 0;
  time = 0;
  position[2] = FIELD_AT(work, s32, 4104);
  do {
    switch (state) {
    case 0:
      if ((FIELD_AT(work, s32, 4204) -= 0x1000) < 0) {
        FIELD_AT(work, s32, 4204) = 0;
        time = -1;
        state = 1;
      }
      break;
    case 1:
      if ((FIELD_AT(work, s32, 4196) -= 0x800) < 0) {
        FIELD_AT(work, s32, 4196) = 0;
        state = 2;
        time = -1;
      }
      break;
    case 2:
      if (time == 1)
        state = 999;
      break;
    }
    wait(1);
    time++;
  } while (state != 999);
  Task_RemoveFar(Func_02002de4);
  Map_SetScreenShake(0, 0, 0);
  Resource_ResetEntry(FIELD_AT(work, s16, 4188));
  Resource_ResetEntry(FIELD_AT(work, s16, 4192));
  HeapSlot_Release(220);
}

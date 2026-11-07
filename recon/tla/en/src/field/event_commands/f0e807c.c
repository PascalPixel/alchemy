#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "effect_sprite.h"
#include "bg_scroll.h"

struct Unk_080e807cEntry {
  s32 x;
  s32 y;
  s32 z;
  s32 unk0c;
  s32 unk10;
  s32 unk14;
  s32 timer;
};

struct Unk_080e807cWork {
  u8 unk0000[0x2100];
  struct EffectSprite sprites[16];
  u8 unk2380[0x280];
  struct Unk_080e807cEntry entries[16];
  u8 unk27c0[0x1c2];
  u16 tile;
  s8 unk2984;
  s8 unk2985;
  s8 unk2986;
  u8 unk2987;
  u16 phase;
};

struct Unk_080e807cEvent {
  u8 unk000[0x166];
  u16 unk166;
  u8 unk168[0xb45];
  s8 unkcad;
  s16 count;
};

struct Actor *Actor_GetById(s32 id);
void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);
void Camera_WorldToScreen(s32 *position);
void Func_080e7fec(s32 x, s32 y);
s32 Trig_Sin(s32 angle);
u32 Random16(void);
void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite,
                                      s32 *position);

void Func_080e807c(void) {
  struct Unk_080e807cWork *work = Ram_HeapSlots->battleFxWork[0];
  struct Unk_080e807cEvent *event = Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 position[3];
  s32 *pos = position;
  struct Unk_080e807cEntry *entry;
  struct EffectSprite *sprite;
  s32 x;
  s32 y;
  s32 i;
  s32 timer;
  u16 *phase;
  u16 *tile;
  s32 red;

  pos[0] = actor->x;
  pos[1] = actor->y + 0x100000;
  pos[2] = actor->z;
  if (work->unk2985 != 0)
    Vector_AddPolarOffset(0x20000, -(event->count << 11), pos);
  Camera_WorldToScreen(pos);
  x = pos[0] >> 16;
  gBgScroll[0].x = 64 - x;
  y = pos[2] >> 16;
  gBgScroll[0].y = 64 - y;
  Func_080e7fec(x - 64, y - 64);

  if (work->unk2986 != 0) {
    i = 0;
    phase = &work->phase;
    red = 28;
  cycle:
    {
      register s32 green asm("r5") = Trig_Sin(((*phase >> 1) + i) << 11) * 8;
      /* FAKEMATCH(pin): green kept in r5; as a plain local, 17 of 272
       * instructions differ in tla-en. */
      s32 blue;
      s32 color;

      blue = Trig_Sin((((*phase >> 2) + i) << 11) + 0x4800) * 10;
      green = (green >> 16) + 23;
      blue = ((blue >> 16) + 12) << 10;
      green <<= 5;
      /* FAKEMATCH(barrier): an empty asm before the colour is assembled;
       * without it, 2 of 272 instructions differ in tla-en. */
      asm("");
      color = blue | green | red;

      ((u16 *)0x050001e0)[i] = color;
      ((u16 *)0x050003e0)[i] = color;
    }
    if (++i < 16)
      goto cycle;
    work->phase++;
  }

  if (work->unk2984 != 0) {
    entry = work->entries;
    sprite = work->sprites;
    tile = &work->tile;
    for (i = 15; i >= 0; i--, sprite++, entry++) {
      timer = entry->timer;
      if (timer == 0) {
        sprite->angle = Random16();
        pos[2] = timer;
        pos[1] = timer;
        pos[0] = timer;
        Vector_AddPolarOffset(0x360000, sprite->angle + 0x2000, pos);
        entry->x = pos[0];
        entry->y = pos[1] + 0x100000;
        entry->z = pos[2];
        timer = entry->timer;
      }
      if (timer >= 0) {
        sprite->oam.index = *tile + ((timer >> 2) & 3) * 4;
        pos[0] = actor->x + entry->x;
        pos[1] = actor->y + entry->y;
        pos[2] = actor->z + entry->z;
        EffectSprite_DrawAtWorldPosition(sprite, pos);
        timer = entry->timer;
      }
      if ((entry->timer = timer + 1) == 16)
        entry->timer = 0;
    }
  }

  if (event->unkcad != 0 && event->count != 0)
    event->count--;
  if (event->count == 0)
    event->unk166 = 0x209b;
}

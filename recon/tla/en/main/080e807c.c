// Near miss, differ 48 of 272 (2026-10-26, statements reordered) (2026-10-18, written from the listing; ☀️ has
// no twin): the frame callback of the effect Func_080e87c4 ends, with the
// ring picture of graphics/field_fx/ring_arcs.s. The position is reached
// through a pointer, as the ROM keeps sp + 4 in r7 (221 -> 167). The colour
// loop is a goto loop over a pointer to the phase: as a for loop agscc
// strength-reduces both palette addresses and hoists their pool loads, which
// the ROM rebuilds from i each pass (164 -> 71 -> 53). What remains: the red
// sine is copied to r5 and shifted after the second call, where the ROM
// shifts it into r5 at once (a volatile asm after it keeps the shift before
// the call but not the copy, 53; an s16 Trig_Sin 57); the sparkle loop's
// tile pointer and count swap r2 and r3 while being built; and reordered
// pairs, the spill of the sprite pointer among them.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "adventure.h"
#include "effect_sprite.h"
#include "bg_scroll.h"

struct RingMote {
  s32 x;
  s32 y;
  s32 z;
  s32 unk0c;
  s32 unk10;
  s32 unk14;
  s32 timer;
};

struct RingWork {
  u8 unk0000[0x2100];
  struct EffectSprite sprites[16];
  u8 unk2380[0x280];
  struct RingMote motes[16];
  u8 unk27c0[0x1c2];
  u16 tile;
  s8 sparkling;
  s8 orbiting;
  s8 cycling;
  u8 unk2987;
  u16 phase;
};

struct RingEvent {
  u8 unk000[0x166];
  u16 cue;
  u8 unk168[0xb45];
  s8 counting;
  s16 count;
};

struct Actor *Actor_GetById(s32 id);
void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);
void Camera_WorldToScreen(s32 *position);
void Func_080e7fec(s32 x, s32 y);
s32 Trig_Sin(s32 angle);
u32 Random16(void);
void Func_080eb298(struct EffectSprite *sprite, s32 *position);

void Func_080e807c(void) {
  struct RingWork *work = Ram_HeapSlots->battleFxWork[0];
  struct RingEvent *event = Ram_HeapSlots->eventWork;
  struct Actor *object = Actor_GetById(gAdventure.currentOwner);
  s32 position[3];
  s32 *pos = position;
  struct RingMote *mote;
  struct EffectSprite *sprite;
  s32 x;
  s32 y;
  s32 i;
  s32 timer;
  u16 *phase;

  pos[0] = object->x;
  pos[1] = object->y + 0x100000;
  pos[2] = object->z;
  if (work->orbiting != 0)
    Vector_AddPolarOffset(0x20000, -(event->count << 11), pos);
  Camera_WorldToScreen(pos);
  x = *((s16 *)pos + 1);
  gBgScroll[0].x = 64 - x;
  y = *((s16 *)pos + 5);
  gBgScroll[0].y = 64 - y;
  Func_080e7fec(x - 64, y - 64);

  if (work->cycling != 0) {
    i = 0;
    phase = &work->phase;
  cycle:
    {
      s32 red = Trig_Sin(((*phase >> 1) + i) << 11) * 8;
      s32 green;
      s32 color;

      green = Trig_Sin((((*phase >> 2) + i) << 11) + 0x4800) * 10;
      color = ((green >> 16) + 12) << 10 | ((red >> 16) + 23) << 5 | 28;

      ((u16 *)0x050001e0)[i] = color;
      ((u16 *)0x050003e0)[i] = color;
    }
    if (++i < 16)
      goto cycle;
    work->phase++;
  }

  if (work->sparkling != 0) {
    mote = work->motes;
    sprite = work->sprites;
    for (i = 15; i >= 0; i--, sprite++, mote++) {
      timer = mote->timer;
      if (timer == 0) {
        sprite->angle = Random16();
        pos[2] = timer;
        pos[1] = timer;
        pos[0] = timer;
        Vector_AddPolarOffset(0x360000, sprite->angle + 0x2000, pos);
        mote->x = pos[0];
        mote->y = pos[1] + 0x100000;
        mote->z = pos[2];
        timer = mote->timer;
      }
      if (timer >= 0) {
        timer = mote->timer;
        sprite->oam.index = work->tile + ((timer >> 2) & 3) * 4;
        pos[0] = object->x + mote->x;
        pos[1] = object->y + mote->y;
        pos[2] = object->z + mote->z;
        Func_080eb298(sprite, pos);
      }
      if ((mote->timer = timer + 1) == 16)
        mote->timer = 0;
    }
  }

  if (event->counting != 0 && event->count != 0)
    event->count--;
  if (event->count == 0)
    event->cue = 0x209b;
}

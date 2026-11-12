/// @file
/// Scene code of overlay 0x687, in ROM order, from s0ef0's offset up to the
/// next object the overlay links.

#include "types.h"
#include "actor.h"
#include "system/field_effect.h"
#include "event_inline.h"
#include "frame.h"

void Func_02001fac(void);
void Func_02001fb4(void);
s32 Math_Cosine(s32 angle);
s32 Func_02001f9c(struct Actor *actor);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

/// Sends actor 8 to a point 8 pixels along z and, until Func_02001f9c says it
/// is done, raises a pair of effects at its two sides every fourth frame.
void Func_02000ef0(void) {
  s32 sides[4];
  struct EffectOptions options;
  struct Actor *actor;
  s32 cosine, sine;
  s32 x, y, z;
  s32 velocity, reverseVelocity, alternate;

  actor = Actor_GetById(8);
  Func_02001fac();
  Event_ResolvePendingActions(0);
  Actor_SetMoveTarget(actor, actor->x, actor->y, actor->z + 0x80000);
  // A goto loop: as while (Func_02001f9c(actor) == 0), loop.c moves 0xb0000
  // out of the loop and 191 of 165 instructions differ.
  while (Func_02001f9c(actor) == 0) {
  if ((gFrameCount & 3) == 0) {
    options.priority = 2;
    options.palette = 7;
    options.startScaleX = 0x16666;
    options.startScaleY = 0x13333;
    cosine = Math_Cosine(actor->facing);
    sine = Math_Sine(actor->facing);
    sides[0] = cosine * 12;
    sides[1] = sine * 12;
    sides[2] = -(cosine * 12);
    sides[3] = -(sine * 12);
    sides[0] += actor->x;
    sides[1] += actor->z;
    sides[2] += actor->x;
    sides[3] += actor->z;
    velocity = 0x8000;
    if (sides[1] != actor->z) {
      sides[1] -= 0xc0000;
      sides[3] -= 0xc0000;
    }
    alternate = gFrameCount & 4;
    reverseVelocity = -velocity;
    x = sides[0];
    y = actor->y;
    z = sides[1];
    if (alternate) {
      Func_0200015c(x + 0x40000, y, z, velocity, 0, 0,
                    EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                        EFFECT_USE_START_SCALE,
                    &options);
      Func_0200015c(sides[2] - 0x40000, actor->y, sides[3], reverseVelocity, 0, 0,
                    EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                        EFFECT_USE_START_SCALE,
                    &options);
    } else {
      Func_0200015c(x - 0x40000, y, z, reverseVelocity, 0, 0,
                    EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                        EFFECT_USE_START_SCALE,
                    &options);
      Func_0200015c(sides[2] + 0x40000, actor->y, sides[3], velocity, 0, 0,
                    EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                        EFFECT_USE_START_SCALE,
                    &options);
    }
  }
  wait(1);
  }
  Func_02001fb4();
}


#include "sea_of_time.h"
#include "main_routine_work.h"
#include "io_reg.h"
#include "scene_ids.h"
#include "scene_tables.h"

void Func_02001068(void) {
  Func_02001fac();
  Event_ResolvePendingActions(0);
  if (!Event_GetFlag(0x8dd)) {
    Import_EventSetExitRequest(4);
    GameFlag_SetBit(0x8dd);
  } else {
    Import_EventSetExitRequest(1);
  }
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001fb4();
}

void Func_020010a8(struct Actor *object) {
  struct EffectOptions spark;
  s32 even = gFrameCount & 1;

  if (even == 0) {
    s32 dx = ((Random16Far() & 31) << 16) + -0xe0000;

    spark.startScaleX = 0xb333;
    spark.startScaleY = 0xb333;
    spark.palette = 7;
    spark.priority = 1;
    Func_0200015c(object->x + dx, 0, object->z + 0x100000, 0, even, even,
                  0xb0000, &spark);
  }
}

/// The 14 colours of BG palette 3 (entries 1 to 14) that the fade returns to,
/// copied from palette RAM by Func_02001cdc and rotated by Func_02001650 in
/// step with BG palette entries 49 to 62.
struct SavedColors {
  u16 colors[14];
};

extern struct SavedColors gData_020026c0;

void Func_02001f4c(struct Actor *object, s32 value);
void Func_0200200c(s32 a, s32 b);

void Func_02001108(void) {
  volatile u16 fill;
  struct EffectOptions burst;
  struct EffectOptions rain;
  struct Actor *object;
  s32 changed;
  volatile u16 *palette;
  struct SavedColors *saved;
  u32 i;
  u32 j;

  Func_02001fac();
  Event_ResolvePendingActions(0);
  GameFlag_SetBit(0x8dd);
  Actor_SetSpeed(8, 0x10000, 0x8000);
  Actor_GetById(8)->facing = FACING_NORTH;
  Actor_WalkTo(8, 352, 264);
  Camera_SetSpeedParameters(0x10000, 0x2000);
  Camera_MoveWithinMapBounds(352 << 16, -1, 196 << 16, 1);
  Func_02001ffc();
  Actor_WaitForMoveById(8);
  Sound_Play(78);
  Sound_Play(556);
  // Fills palette 3's entries 1 to 14 with one colour.
  fill = RGB(11, 6, 23);
  Dma_Set((const void *)&fill, (void *)&BG_PLTT_COLOR(3, 1),
          (DMA_ENABLE | DMA_SRC_FIXED) << 16 | 14, REG_DMA3);
  Map_CopyCellPictureIndicesRect(49, 70, 19, 73, 6, 6);
  // Steps each colour's red, green and blue by one toward the saved palette
  // every 10 frames until none changes.
fade:
  wait(10);
  changed = 0;
  palette = &BG_PLTT_COLOR(3, 0);
  // i counts palette entries from 1 and j saved colours from 0; the saved
  // colours are read through a pointer set after i, each pass.
  for (i = 1, saved = &gData_020026c0, j = 0; i != 15; i++, j++) {
    u16 color = palette[i];
    u16 target = saved->colors[j];
    u16 r = color & 31;
    u16 g = (color >> 5) & 31;
    u16 b = (color >> 10) & 31;
    u16 tr = target & 31;
    u16 tg = (target >> 5) & 31;
    u16 tb = (target >> 10) & 31;

    if (r < tr) {
      r++;
      changed = 1;
    }
    if (r > tr) {
      r--;
      changed = 1;
    }
    if (g < tg) {
      g++;
      changed = 1;
    }
    if (g > tg) {
      g--;
      changed = 1;
    }
    if (b < tb) {
      b++;
      changed = 1;
    }
    if (b > tb) {
      b--;
      changed = 1;
    }
    palette[i] = RGB(r, g, b);
  }
  if (changed)
    goto fade;
  Event_Wait(30);
  // For 360 frames, spawns an effect every 16 frames, then every 8, then
  // every other frame, and plays sound 164 every 64 frames. As a C loop,
  // loop.c moves the mask, the palette and the options address out of it,
  // which the native does not (differ 252).
  i = 0;
sparks:
  if ((i & 63) == 0)
    Sound_Play(164);
  if (i < 120) {
    if (i & 15)
      goto next;
  } else if (i < 240) {
    if (i & 7)
      goto next;
  } else if (i & 1)
    goto next;
  {
    s32 x = Random16Far();
    s32 mask = 31;
    s32 z = Random16Far();

    burst.palette = 7;
    Func_0200015c(((x & mask) << 16) + 0x1500000, 0,
                  ((z & mask) << 16) + 0xb00000, 0, 0, 0, 0x10000, &burst);
    wait(1);
  }
next:
  i++;
  if (i != 360)
    goto sparks;
  // Then 16 larger effects, one a frame. As a C loop, loop.c also moves the
  // mask and the options address out, which the native does not (differ 228).
  // Entered at its test so that PRE leaves the address in the body.
  i = 0;
  while (i != 16) {
    {
      s32 x = Random16Far();
      s32 mask = 31;
      s32 z = Random16Far();
      s32 offsetZ = (z & mask) << 16;
      s32 offsetX = (x & mask) << 16;

      rain.startScaleX = 0x18000;
      rain.startScaleY = 0x18000;
      rain.palette = 7;
      rain.priority = 1;
      Func_0200015c(offsetX + 0x1500000, 0, offsetZ + 0xb00000, 0, 0x10000, 0,
                    0xb0000, &rain);
      wait(1);
    }
    i++;
  }
  object = Import_ActorCreate(159, 0x15c0000, 0, 0xb80000);
  // The object's per-frame hook, struct Actor's update.
  *(void (**)(struct Actor *))((u8 *)object + 0x6c) = Func_020010a8;
  Event_Wait(60);
  Func_02001f4c(object, 1);
  Event_Wait(40);
  gMainRoutineWork.battleRule24b = 2;
  Party_SetFields1eeAnd1f0((s32)&SceneId_Ov687Room2, 4);
  Func_0200200c(12, 6);
  Func_02001fb4();
}

extern struct Trigger gData_020022d0[];
extern struct Trigger gData_0200242c[];
extern struct Trigger gData_0200251c[];

void Func_0200141c(s32 x, s32 z, s32 actor) {
  s32 cx = x >> 20;
  s32 cz = z >> 20;

  Func_02000388(1, cx - 1, cz - 1, 0);
  Func_02000388(1, cx, cz - 1, 0);
  Func_02000388(1, cx - 1, cz, 0);
  Func_02000388(1, cx, cz, 0);
  Actor_SetHorizontalPositionWithTerrain(actor, 0, 0);
}

void Func_02001480(void) {
  struct Actor *object;

  Func_02001fac();
  Event_ResolvePendingActions(0);
  object = Actor_GetById(9);
  Func_0200141c(object->x, Actor_GetById(9)->z, 9);
  GameFlag_SetBit(0xa90);
  Func_02001fb4();
}

void Func_020014b4(void) {
  struct Actor *object;

  Func_02001fac();
  Event_ResolvePendingActions(0);
  object = Actor_GetById(10);
  Func_0200141c(object->x, Actor_GetById(10)->z, 10);
  GameFlag_SetBit(0xa91);
  Func_02001fb4();
}

void Func_020014ec(void) {
  struct Actor *object;

  Func_02001fac();
  Event_ResolvePendingActions(0);
  object = Actor_GetById(11);
  Func_0200141c(object->x, Actor_GetById(11)->z, 11);
  GameFlag_SetBit(0xa92);
  Func_02001fb4();
}

void Func_02001524(void) {
  struct Actor *object;

  Func_02001fac();
  Event_ResolvePendingActions(0);
  object = Actor_GetById(12);
  Func_0200141c(object->x, Actor_GetById(12)->z, 12);
  GameFlag_SetBit(0xa93);
  Func_02001fb4();
}

void Func_0200155c(void) {
  struct Actor *object;

  Func_02001fac();
  Event_ResolvePendingActions(0);
  object = Actor_GetById(13);
  Func_0200141c(object->x, Actor_GetById(13)->z, 13);
  GameFlag_SetBit(0xa94);
  Func_02001fb4();
}

void Func_02001594(void) {
  struct Actor *object;

  Func_02001fac();
  Event_ResolvePendingActions(0);
  object = Actor_GetById(9);
  Func_0200141c(object->x, Actor_GetById(9)->z, 9);
  GameFlag_SetBit(0xa95);
  Func_02001fb4();
}

void Func_020015cc(void) {
  Func_02001fac();
  Event_ResolvePendingActions(0);
  Import_EventSetExitRequest(4);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001fb4();
}

void Func_020015ec(void) {
  Func_02001fac();
  Event_ResolvePendingActions(0);
  Import_EventSetExitRequest(5);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001fb4();
}

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov687Room1)
    return gData_020022d0;
  if (gMainRoutineWork.entrance <= 4)
    return gData_0200242c;
  return gData_0200251c;
}


#include "map.h"

// The Sea of Time's frame callback: rotates two runs of the background palette,
// pulses the red of colour 15 (or steps it with the alarm), copies the palette
// and ripples the layer's horizontal scroll line by line.
/// The event work's fields this function reads that SeaOfTimeEventWork lacks.
struct EventWork_02001650 {
  u8 unk000[0x166];
  s16 unk166;
  u8 unk168[0x4a];
  u16 unk1b2;
};

extern u16 Data_02002580[160];

extern s16 gData_0200257c[1];

/// The Sea of Time's frame callback: rotates two runs of the background
/// palette, pulses the red of colour 15 (or steps it with the alarm), copies
/// the palette and ripples the layer's horizontal scroll line by line.
void Func_02001650(void) {
  u16 *line = Data_02002580;
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[2];
  u8 *block = Ram_HeapSlots->block32;
  struct EventWork_02001650 *event = Ram_HeapSlots->eventWork;
  s32 i;
  s16 color;

  if (event->unk166 != 0)
    return;
  if (*(s8 *)(block + 0x2a01) != 0)
    return;
  if (event->unk1b2 == 0)
    return;

  if (gFrameCount - gData_020022c8 > gData_020022cc) {
    s16 target;

    color = BG_PLTT[62];
    target = gData_020026c0.colors[13];

    for (i = 14; i != 0; i--) {
      BG_PLTT[48 + i] = BG_PLTT[47 + i];
      gData_020026c0.colors[i - 1] = gData_020026c0.colors[i - 2];
    }
    BG_PLTT[49] = color;
    gData_020026c0.colors[0] = target;
    gData_020022c8 = gFrameCount;
  }

  if ((gFrameCount & 1) == 0) {
    color = BG_PLTT[78];

    for (i = 14; i != 0; i--)
      BG_PLTT[64 + i] = BG_PLTT[63 + i];
    BG_PLTT[65] = color;

    if ((gFrameCount & 3) == 0) {
      u16 color = BG_PLTT[15];
      s32 red = color & 31;
      s32 green = (color >> 5) & 31;
      s32 blue = (color >> 10) & 31;
      s32 flag = GameFlag_Test(0x200);
      /* FAKEMATCH(temporary): each channel shifted up after the flag test; as
       * (u16)green << 5 and so on in the store, 167 of 272 instructions differ
       * in tla-en. */
      u32 greenBits = green << 16;
      u32 redBits = red << 16;
      u32 blueBits = blue << 16;

      if (flag == 0) {
        u32 step = (u16)gData_0200257c[0];
        u32 sum = (u16)(redBits >> 16) + step;
        red = (s16)sum;
        if ((u16)sum < 18) {
          red = 18;
          gData_0200257c[0] = 1;
        } else if ((u16)sum > 31) {
          red = 31;
          gData_0200257c[0] = -1;
        }
      } else {
        if (((struct SeaOfTimeEventWork *)Ram_HeapSlots->eventWork)->alarm ==
            200) {
          u16 current = redBits >> 16;
          if (current != 0)
            red = (s16)(u16)(current - 1);
        } else if (((struct SeaOfTimeEventWork *)Ram_HeapSlots->eventWork)
                       ->alarm == 201) {
          u16 current = redBits >> 16;
          if (current < 30)
            red = current + 1;
        }
      }
      {
        u16 last = red;
        BG_PLTT[15] = (blueBits >> 6) | (greenBits >> 11) | (last & 31);
      }
    }
  }

  Dma_Set((const void *)BG_PLTT, Ram_HeapSlots->block32, DMA_COPY_WORDS(112),
          REG_DMA3);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  *(s32 *)((u8 *)layer + 0x1c) = 0;
  for (i = 0; i != 160; i++)
    *line++ =
        (s16)(layer->x >> 16) +
        (Math_Sine((i + *(u8 *)&gFrameCount + (s16)(layer->y >> 16)) << 9) >>
         15);
  line = Data_02002580;
  REG_BG1HOFS = *line;
  Dma_Set(line + 1, (void *)&REG_BG1HOFS, 0xa2600001, (volatile u32 *)REG_DMA0);
}

void Func_020018a8(s32 x, s32 z, s32 actor) {
  Actor_GetById(actor)->collisionFlags |= 16;
  Actor_SetHorizontalPositionWithTerrain(actor, (x << 19) + 0x80000,
                                         (z << 19) + 0x80000);
  x >>= 1;
  z >>= 1;
  Func_02000388(1, x, z, 0xff00);
  Func_02000388(1, x + 1, z, 0xff00);
  Func_02000388(1, x, z + 1, 0xff00);
  Func_02000388(1, x + 1, z + 1, 0xff00);
}

void Func_02001928(s32 x, s32 z, s32 actor) {
  struct EffectOptions spark;
  s32 px;
  s32 pz;
  s32 i;

  Camera_SetSpeedParameters(0x60000, 0xc000);
  px = (x << 19) + 0x80000;
  pz = (z << 19) + 0x80000;
  FieldCamera_MoveTo(px, -1, pz, 1);
  Func_02001ffc();
  Sound_Play(144);
  MapGrid_SetShake(0, 0x100000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  spark.startScaleX = 0x13333;
  spark.startScaleY = 0x18000;
  spark.priority = 1;
  spark.type = 276;
  Func_0200015c(px, 0, pz, 0, 0, 0, 0x1a0000, &spark);
  Actor_GetById(actor)->scaleY = 0x1999;
  Func_020018a8(x, z, actor);
  for (i = 0; i != 10; i++) {
    Actor_GetById(actor)->scaleY += 0x1999;
    Event_Wait(1);
  }
  Actor_GetById(actor)->scaleY = 0x10000;
  Event_Wait(20);
}

#include "iwram_call.h"
#include "system.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

struct EventWork687 {
  u8 unk000[0x16a];
  u16 unk16a;
};

void Func_02001f64();
void Func_0200204c();
/// The Sea of Time's opening: places the camera and the first effects, showers
/// sparks for 360 frames and larger effects for 16, then fades BG palette 3 to
/// RGB(11, 6, 23).
void Func_02001a14(void) {
  struct Actor *actor;
  u32 i;
  s32 changed;
  volatile u16 *palette;
  struct EffectOptions params;
  struct EffectOptions burst;

  Func_02001fac();
  Event_ResolvePendingActions(0);
  Sound_Play(0);
  Map_CopyCellPictureIndicesRect(49, 70, 19, 73, 6, 6);
  /* FAKEMATCH(wrapper): forwarded, so that its constants are built in the
   * native's order; as a direct call, 2 of 307 instructions differ in tla-en.
   */
  actor = FieldActor_Create(159, 0x15c0000, 0, 0xb80000);
  actor->update = (void (*)(void))Func_020010a8;
  /* FAKEMATCH(wrapper): forwarded, so that its constants are not shared with
   * the second move's; as a direct call, 47 of 307 instructions differ in
   * tla-en. */
  FieldCamera_MoveTo(0x1600000, -1, 0xc40000, 0);
  Event_StartScreenTransition();
  wait(1);
  Task_Add(Func_02001650, 0xc80);
  Event_WaitTransitionFrames();
  Event_Wait(60);
  Func_02001928(21, 23, 9);
  Func_02001928(37, 17, 10);
  Func_02001928(47, 27, 12);
  Func_02001928(59, 19, 13);
  Map_WaitForShakeToSettle();
  Camera_MoveWithinMapBounds(0x1600000, -1, 0xc40000, 1);
  Func_02001ffc();
  GameFlag_SetBit(0x200);
  ((struct EventWork687 *)Ram_HeapSlots->eventWork)->unk16a = 200;
  Sound_Play(0x212);
  Event_Wait(40);
  for (i = 0; i != 360; i++) {
    if ((i & 31) == 0)
      Sound_Play(164);
    if (i < 120) {
      if (i & 15)
        continue;
    } else if (i < 240) {
      if (i & 7)
        continue;
    } else if (i & 1)
      continue;
    {
      s32 mask = 31;
      s32 x = Random16Far();
      s32 y = Random16Far();

      params.startScaleX = 0xcccc;
      params.startScaleY = 0xcccc;
      params.priority = 1;
      params.palette = 7;
      x &= mask;
      x <<= 16;
      x += 0x1500000;
      Func_0200015c(x, 0, ((y & mask) << 16) + 0xb00000, 0, 0, 0, 0xb0000,
                    &params);
      wait(1);
    }
  }
  for (i = 0; i != 16; i++) {
    s32 mask = 31;
    s32 x = Random16Far();
    s32 y = Random16Far();

    burst.startScaleX = 0x18000;
    burst.startScaleY = 0x18000;
    burst.palette = 7;
    burst.priority = 1;
    x &= mask;
    x <<= 16;
    x += 0x1500000;
    Func_0200015c(x, 0, ((y & mask) << 16) + 0xb00000, 0, 0x10000, 0, 0xb0000,
                  &burst);
    wait(1);
  }
  Func_02001f64(actor);
  Sound_Play(0x22d);
  do {
    wait(10);
    changed = 0;
    for (i = 1, palette = &BG_PLTT_COLOR(3, 1); i != 14; i++, palette++) {
      u16 color = *palette;
      u16 r = color & 31;
      u16 g = (color >> 5) & 31;
      u16 b = (color >> 10) & 31;

      if (r < 11) {
        r++;
        changed = 1;
      }
      if (r > 11) {
        r--;
        changed = 1;
      }
      if (g < 6) {
        g++;
        changed = 1;
      }
      if (g > 6) {
        g--;
        changed = 1;
      }
      if (b < 23) {
        b++;
        changed = 1;
      }
      if (b > 23) {
        b--;
        changed = 1;
      }
      *palette = RGB(r, g, b);
    }
    gData_020022cc += gFrameCount & 1;
  } while (changed);
  Task_RemoveFar(Func_02001650);
  Func_0200204c();
  GameFlag_SetBit(0x940);
  Func_02001fb4();
}


// The overlay's start: sets the screen effect, restores the clock and palette
// in the first room, draws the unlit cells in the second, and schedules the
// frame callback.

void Func_02000c50(void);

/// The overlay's start: sets the screen effect, restores the clock and palette
/// in the first room, draws the unlit cells in the second, and schedules the
/// frame callback.
s32 Func_02001cdc(void) {
  ((struct SeaOfTimeEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 256;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov687Room1) {
    gData_020022c8 = gFrameCount;
    gData_020022cc = 1;
    Dma_Set((const void *)0x05000062, gData_020026c0.colors, 0x8000000e,
            (volatile u32 *)0x040000d4);
    if (GameFlag_Test(0x200)) {
      u32 *state = gSceneState;
      u32 offset, i;

      gData_020022c8 = *state++;
      gData_020022cc = *state++;
      gFrameCount = *state;
      i = 1;
      offset = 98;
      do {
        *(volatile u16 *)(offset + 0x05000000) = 0x5ccb;
        i++;
        offset += 2;
      } while (i != 14);
      Task_Add(Func_02000c50, 0x480);
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov687Room2) {
    gData_020022c8 = gFrameCount;
    gData_020022cc = 0;
    if (gMainRoutineWork.entrance <= 4) {
      if (!GameFlag_Test(0x940)) {
        Dma_Set((const void *)0x05000062, gData_020026c0.colors, 0x8000000e,
                (volatile u32 *)0x040000d4);
      } else {
        if (!GameFlag_Test(0xa90))
          Func_020018a8(21, 23, 9);
        if (!GameFlag_Test(0xa91))
          Func_020018a8(37, 17, 10);
        if (!GameFlag_Test(0xa93))
          Func_020018a8(47, 27, 12);
        if (!GameFlag_Test(0xa94))
          Func_020018a8(59, 19, 13);
        {
          void *block = Ram_HeapSlots->block32;

          *(volatile u16 *)0x0500001e &= 0xffe0;
          Dma_Set((const void *)0x05000000, block, 0x84000070,
                  (volatile u32 *)0x040000d4);
        }
        BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
      }
      if (gMainRoutineWork.entrance == 4 && !GameFlag_Test(265))
        Func_02001a14();
    } else {
      if (!GameFlag_Test(0xa95))
        Func_020018a8(89, 85, 9);
    }
  }
  {
    s32 priority = 0xc80;

    Task_Add(Func_02001650, priority);
  }
  return 0;
}

s32 Func_02001ec8(void) {
  void *block = Ram_HeapSlots->block32;

  Dma_Fill32(-1, block, 0x1c0);
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  return 0;
}

void Func_0200033c(void);
void Func_0200043c(void);
void Func_0200054c(void);
void Func_02000818(void);
void Func_02000a80(void);
void Func_02000c94(void);
void Func_02000da0(void);

extern u32 gData_0200205c[];
extern u32 gData_02002098[];
extern u32 gData_020020d4[];
extern s32 gData_02002154[];

// clang-format off
u32 gData_0200205c[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02002098[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_020020d4[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02002110[] = {
    0x18101810, 0x1810,
};

u16 gData_02002118[] = {
    0, 672, 0, 704, 0, 896, 0, 320,
    0, 512, 0, 192, 0, 320, 0, 416,
    0, 320, 0, 832, 0, 96, 0, 160,
};
// clang-format on

s32 gData_02002148[] = {
    (s32)gData_0200205c,
    (s32)gData_02002098,
    (s32)gData_020020d4,
};

s32 gData_02002154[] = {
    46,
    (s32)Func_0200033c,
    17,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(197),
    SCENE_EXIT_IF(1, 2, 42, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 198, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 197, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 42, 20, EXIT_FLAG_NONE),
    SCENE_EXITS(198),
    SCENE_EXIT_IF(1, 197, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 198, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 2, 44, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 2, 43, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(5, 198, 2, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_020021e4[] = {
    {8, CONDITION_ALWAYS, (s32)gData_02002154, PIXELS(120), 0, PIXELS(280),
     FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02002214[] = {
    {8, CONDITION_ALWAYS, (s32)gData_02002154, PIXELS(120), 0, PIXELS(280),
     FACING_NORTH, 2, 0},
    {424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_NORTH, 2, 0},
    {424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_NORTH, 2, 0},
    {424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_NORTH, 2, 0},
    {424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_NORTH, 2, 0},
    {424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

u16 gData_020022bc[] = {
    0,
};

u16 gData_020022be[] = {
    0,
};

u32 gData_020022c0[] = {
    0,
};

u32 gData_020022c4[] = {
    0,
};

volatile u32 gData_020022c8 = 0;

u32 gData_020022cc = 0;

struct Trigger gData_020022d0[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {2, 49, CONDITION_ALWAYS, EVENT(Func_02001068)},
    {2, 30, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 31, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 32, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 33, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 34, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 35, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 36, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 37, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 40, CONDITION_ALWAYS, EVENT(Func_0200043c)},
    {2, 41, CONDITION_ALWAYS, EVENT(Func_0200043c)},
    {2, 44, CONDITION_ALWAYS, EVENT(Func_02000ef0)},
    {2, 45, 0x200, EVENT(Func_02000818)},
    {2, 46, 0x200, EVENT(Func_02000818)},
    {2, 47, 0x200, EVENT(Func_02000818)},
    {2, 48, 0x200, EVENT(Func_02000818)},
    {2, 50, 0x200, EVENT(Func_02000a80)},
    {2, 51, 0x200, EVENT(Func_02000a80)},
    {2, 52, 0x200, EVENT(Func_02000a80)},
    {2, 53, 0x200, EVENT(Func_02000a80)},
    {2, 54, 0x200, EVENT(Func_02000a80)},
    {2, 55, 0x200, EVENT(Func_02000a80)},
    {2, 56, 0x200, EVENT(Func_02000a80)},
    {2, 57, 0x200, EVENT(Func_02000a80)},
    {6, 200, CONDITION_ALWAYS, EVENT(Func_02000c94)},
    {6, 201, CONDITION_ALWAYS, EVENT(Func_02000da0)},
    {TRIGGERS_END},
};

struct Trigger gData_0200242c[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {2, 5, 0x940, EVENT(Func_02001108)},
    {2, 30, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 31, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 32, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 33, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 34, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 35, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 36, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 37, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 48, CONDITION_ALWAYS, EVENT(Func_02000818)},
    {2, 41, CONDITION_ALWAYS, EVENT(Func_0200043c)},
    {0x9815, 9, CONDITION_ALWAYS, EVENT(Func_02001480)},
    {0x9815, 10, CONDITION_ALWAYS, EVENT(Func_020014b4)},
    {0x9815, 11, CONDITION_ALWAYS, EVENT(Func_020014ec)},
    {0x9815, 12, CONDITION_ALWAYS, EVENT(Func_02001524)},
    {0x9815, 13, CONDITION_ALWAYS, EVENT(Func_0200155c)},
    {TRIGGERS_END},
};

struct Trigger gData_0200251c[] = {
    {2, 1, CONDITION_ALWAYS, EVENT(Func_020015cc)},
    {2, 2, CONDITION_ALWAYS, EVENT(Func_020015ec)},
    {2, 45, CONDITION_ALWAYS, EVENT(Func_02000818)},
    {2, 46, CONDITION_ALWAYS, EVENT(Func_02000818)},
    {2, 47, CONDITION_ALWAYS, EVENT(Func_02000818)},
    {2, 48, CONDITION_ALWAYS, EVENT(Func_02000818)},
    {0x9815, 9, CONDITION_ALWAYS, EVENT(Func_02001594)},
    {TRIGGERS_END},
};

s16 gData_0200257c[] = {
    1,
};

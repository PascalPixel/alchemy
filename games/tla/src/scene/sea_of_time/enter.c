/// @file
/// Scene code of overlay 0x687, in ROM order, from enter's offset up to the next object the overlay links.

#include "sea_of_time.h"
#include "event_inline.h"
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

void Func_020010a8(struct SeaOfTimeObject *object) {
  struct SeaOfTimeSpark spark;
  s32 even = gFrameCount & 1;

  if (even == 0) {
    s32 dx = ((Random16Far() & 31) << 16) + -0xe0000;

    spark.scaleX = 0xb333;
    spark.scaleY = 0xb333;
    spark.kind = 7;
    spark.mode = 1;
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

void Func_02001f4c(struct SeaOfTimeObject *object, s32 value);
void Func_0200200c(s32 a, s32 b);

void Func_02001108(void) {
  volatile u16 fill;
  struct SeaOfTimeSpark burst;
  struct SeaOfTimeSpark rain;
  struct SeaOfTimeObject *object;
  s32 changed;
  volatile u16 *palette;
  struct SavedColors *saved;
  u32 i;
  u32 j;

  Func_02001fac();
  Event_ResolvePendingActions(0);
  GameFlag_SetBit(0x8dd);
  Actor_SetSpeed(8, 0x10000, 0x8000);
  ((struct SeaOfTimeObject *)Actor_GetById(8))->facing = FACING_NORTH;
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

    burst.kind = 7;
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

      rain.scaleX = 0x18000;
      rain.scaleY = 0x18000;
      rain.kind = 7;
      rain.mode = 1;
      Func_0200015c(offsetX + 0x1500000, 0, offsetZ + 0xb00000, 0, 0x10000, 0,
                    0xb0000, &rain);
      wait(1);
    }
    i++;
  }
  object = ((struct SeaOfTimeObject *)Import_ActorCreate(159, 0x15c0000, 0,
                                                         0xb80000));
  // The object's per-frame hook, struct Actor's update.
  *(void (**)(struct SeaOfTimeObject *))((u8 *)object + 0x6c) = Func_020010a8;
  Event_Wait(60);
  Func_02001f4c(object, 1);
  Event_Wait(40);
  gMainRoutineWork.battleRule24b = 2;
  Party_SetFields1eeAnd1f0((s32)&SceneId_Ov687Room2, 4);
  Func_0200200c(12, 6);
  Func_02001fb4();
}

extern const u8 gData_020022d0[];
extern const u8 gData_0200242c[];
extern const u8 gData_0200251c[];

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
  struct SeaOfTimeObject *object;

  Func_02001fac();
  Event_ResolvePendingActions(0);
  object = ((struct SeaOfTimeObject *)Actor_GetById(9));
  Func_0200141c(object->x, ((struct SeaOfTimeObject *)Actor_GetById(9))->z, 9);
  GameFlag_SetBit(0xa90);
  Func_02001fb4();
}

void Func_020014b4(void) {
  struct SeaOfTimeObject *object;

  Func_02001fac();
  Event_ResolvePendingActions(0);
  object = ((struct SeaOfTimeObject *)Actor_GetById(10));
  Func_0200141c(object->x, ((struct SeaOfTimeObject *)Actor_GetById(10))->z,
                10);
  GameFlag_SetBit(0xa91);
  Func_02001fb4();
}

void Func_020014ec(void) {
  struct SeaOfTimeObject *object;

  Func_02001fac();
  Event_ResolvePendingActions(0);
  object = ((struct SeaOfTimeObject *)Actor_GetById(11));
  Func_0200141c(object->x, ((struct SeaOfTimeObject *)Actor_GetById(11))->z,
                11);
  GameFlag_SetBit(0xa92);
  Func_02001fb4();
}

void Func_02001524(void) {
  struct SeaOfTimeObject *object;

  Func_02001fac();
  Event_ResolvePendingActions(0);
  object = ((struct SeaOfTimeObject *)Actor_GetById(12));
  Func_0200141c(object->x, ((struct SeaOfTimeObject *)Actor_GetById(12))->z,
                12);
  GameFlag_SetBit(0xa93);
  Func_02001fb4();
}

void Func_0200155c(void) {
  struct SeaOfTimeObject *object;

  Func_02001fac();
  Event_ResolvePendingActions(0);
  object = ((struct SeaOfTimeObject *)Actor_GetById(13));
  Func_0200141c(object->x, ((struct SeaOfTimeObject *)Actor_GetById(13))->z,
                13);
  GameFlag_SetBit(0xa94);
  Func_02001fb4();
}

void Func_02001594(void) {
  struct SeaOfTimeObject *object;

  Func_02001fac();
  Event_ResolvePendingActions(0);
  object = ((struct SeaOfTimeObject *)Actor_GetById(9));
  Func_0200141c(object->x, ((struct SeaOfTimeObject *)Actor_GetById(9))->z, 9);
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
    return (const struct Trigger *)gData_020022d0;
  if (gMainRoutineWork.entrance <= 4)
    return (const struct Trigger *)gData_0200242c;
  return (const struct Trigger *)gData_0200251c;
}

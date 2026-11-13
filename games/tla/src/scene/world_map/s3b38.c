/// @file
/// Scene code of overlay 0x64b, in ROM order, from s3b38's offset up to the next object the overlay links, and the overlay's data.

#include "types.h"
#include "system.h"
#include "frame.h"
#include "actor.h"
#include "event_inline.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "field.h"
#include "module/scene/scene.h"
#include "module/scene/staff_credit.h"
#include "module/scene/world_map.h"
#include "module/system/system.h"
#include "resource.h"

void Animation_ApplyChildValues(struct Actor *object, s32 value);

void Func_02003b38(struct Actor *object) {
  if ((gFrameCount & 2) != 0) {
    Animation_ApplyChildValues(object, 7);
  } else {
    Animation_ApplyChildValues(object, 0);
  }
  if ((gFrameCount & 15) == 0) {
    Func_020039f4(object);
  }
}

extern s32 gData_02006be4;

void Func_02003b70(struct Actor *object) {
  if (gFrameCount & 1) {
    Animation_ApplyChildValues(object, gData_02004fc8[gData_02006be4]);
    gData_02006be4 = (gData_02006be4 + 1) & 3;
  }
  if (!(gFrameCount & 3)) {
    Func_02003aac(object);
  }
}

void Func_02003bb8(struct Actor *object, s32 mode) {
  if (mode == 0) {
    object->update = 0;
    Animation_ApplyChildValues(object, 0);
    Event_PlaySound(390);
  } else if (mode == 1) {
    object->update = (void (*)(void))Func_02003b38;
  } else {
    object->update = (void (*)(void))Func_02003b70;
    Event_PlaySound(291);
  }
}

void Func_02003bf0(void) {
  struct Actor *object = Actor_Get(FieldParty_GetLeaderActor());
  s32 i;

  Event_PlaySound(212);
  Func_02003bb8(object, 1);
  for (i = 0; i < 20; i++) {
    s32 hit = Func_02004b90(&object->x);

    if (hit != 0 && SceneEvent_FindActiveRecord(2, hit) != 0)
      break;
    System_Wait(1);
  }
  Func_02003bb8(object, 2);
}

u32 __umodsi3(u32 a, u32 b);

void Func_02003c3c(void) { Func_02003bb8(Actor_GetById(Party_GetLeaderActor()), 0); }

static inline void QueueBlend(u32 address, u32 value) {
  /* FAKEMATCH(wrapper): forwarding through the inline sets the arguments up in
   * their own order: the native finishes the value's orr before the register
   * address's add; as direct calls, 4 of 31 instructions of Func_02003c50
   * differ in tla-en. */
  QueueIoWriteDelay2(address, value);
}

/// Every other two frames the world map's blend alternates between two
/// strengths.
void Func_02003c50(void) {

  QueueIoWriteDelay2((u32)&REG_BLDCNT,
                     BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
  if (gFrameCount & 2)
    QueueBlend((u32)&REG_BLDALPHA, gData_02006e08 | 12);
  else
    QueueBlend((u32)&REG_BLDALPHA, gData_02006e08 | 16);
}

static inline void CameraMove(s32 x, s32 y, s32 z, s32 mode) {
  /* FAKEMATCH(wrapper): as in the scene layer of The Broken Seal, forwarding
   * through the inline sets the arguments up in their own order: the native
   * shifts z before it starts the -1, where called directly the -1 starts
   * first; as direct calls, 6 of 66 instructions of Func_02003ca8 differ in
   * tla-en. */
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

/// Shakes the camera diagonally by one pixel every three frames.
void Func_02003ca8(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = *(s16 *)((u8 *)object + 10);
  s32 z = *(s16 *)((u8 *)object + 18);

  if (gFrameCount % 3 == 0) {
    switch ((u32)(Random16Far() << 2) >> 16) {
    case 0:
      CameraMove((x << 16) - 0x10000, -1, (z << 16) + 0x10000, 1);
      break;
    case 1: {
      Camera_MoveWithinMapBounds((x << 16) + 0x10000, -1, (z << 16) - 0x10000,
                                 1);
      break;
    }
    case 2:
      CameraMove((x << 16) + 0x10000, -1, (z << 16) + 0x10000, 1);
      break;
    case 3:
      CameraMove((x << 16) - 0x10000, -1, (z << 16) - 0x10000, 1);
      break;
    }
  }
}

#include "event_work.h"
#include "bg_scroll.h"
#include "sprite.h"
#include "field_sprite.h"
#include "vram_tab.h"
#include "resource.h"

#define PUSH_PICTURE(offset)                                                   \
  IO_WRITE_QUEUE_PUSH(0x06002000, (u32)(buffer + (offset)), 0x84000140)

void Func_02003d44(s32 mode) {
  u8 *buffer;
  u32 blend;

  System_Wait(1);
  Event_ClearFlag(0x109);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0;
  Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->delay = 16;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  if (mode == 2) {
    MapGrid_SetShake(0x20000, 0x30000, 0x10000);
    Event_PlaySound(141);
  }
  Func_02004da8();
  buffer = Heap_AllocEwram(0x4000);
  Resource_DecodeType01(gWorldMapPackedTiles, buffer);
  Resource_DecodeType01(gWorldMapPackedPictures, buffer + 0x1000);
  IO_WRITE_QUEUE_PUSH(0x050001c0, (u32)(gWorldMapPalettes + mode * 32),
                      0x80000010);
  IO_WRITE_QUEUE_PUSH(0x06001000, (u32)buffer, 0x84000400);
  Task_AddFrame(Func_02003c50, 0x480);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(246);
  gData_02006e08 = 0xe00;
  PUSH_PICTURE(0x3480);
  FieldEvent_Wait(2);
  gData_02006e08 = 0xd00;
  PUSH_PICTURE(0x2e80);
  FieldEvent_Wait(2);
  gData_02006e08 = 0xc00;
  PUSH_PICTURE(0x2880);
  FieldEvent_Wait(2);
  gData_02006e08 = 0xb00;
  PUSH_PICTURE(0x2280);
  FieldEvent_Wait(2);
  gData_02006e08 = 0xa00;
  PUSH_PICTURE(0x1c80);
  FieldEvent_Wait(2);
  gData_02006e08 = 0x900;
  PUSH_PICTURE(0x1680);
  FieldEvent_Wait(2);
  gData_02006e08 = 0x800;
  PUSH_PICTURE(0x1080);
  FieldEvent_Wait(140);
  PUSH_PICTURE(0x1680);
  FieldEvent_Wait(4);
  PUSH_PICTURE(0x1c80);
  FieldEvent_Wait(4);
  PUSH_PICTURE(0x2280);
  FieldEvent_Wait(4);
  gData_02006e08 = 0x900;
  PUSH_PICTURE(0x2880);
  FieldEvent_Wait(4);
  gData_02006e08 = 0xa00;
  PUSH_PICTURE(0x2e80);
  FieldEvent_Wait(4);
  gData_02006e08 = 0xb00;
  PUSH_PICTURE(0x3480);
  FieldEvent_Wait(4);
  gData_02006e08 = 0xc00;
  PUSH_PICTURE(0x3a80);
  if (mode == 1) {
    MapGrid_SetShake(0x20000, 0x20000, 0x10000);
    Event_PlaySound(141);
    gData_02006e08 = 0xd00;
    FieldEvent_Wait(4);
    gData_02006e08 = 0xe00;
    FieldEvent_Wait(4);
    gData_02006e08 = 0xf00;
    FieldEvent_Wait(4);
    gData_02006e08 = 0x1000;
    FieldEvent_Wait(45);
    FieldTask_Remove(Func_02003c50);
    DebugText_Clear();
    /* FAKEMATCH(temporary): the blend read into blend before each of the two
     * QueueIoWriteDelay2 calls; as QueueIoWriteDelay2((u32)&REG_BLDALPHA,
     * gData_02006e08) at both, 2 of 593 instructions differ in tla-en. */
    blend = gData_02006e08;
    QueueIoWriteDelay2((u32)&REG_BLDALPHA, blend);
    QueueIoWriteDelay2((u32)&REG_BLDCNT, 0);
  } else {
    gData_02006e08 = 0xd00;
    FieldEvent_Wait(4);
    gData_02006e08 = 0xe00;
    FieldEvent_Wait(4);
    gData_02006e08 = 0xf00;
    FieldEvent_Wait(4);
    gData_02006e08 = 0x1000;
    FieldEvent_Wait(45);
    FieldTask_Remove(Func_02003c50);
    DebugText_Clear();
    blend = gData_02006e08;
    QueueIoWriteDelay2((u32)&REG_BLDALPHA, blend);
    QueueIoWriteDelay2((u32)&REG_BLDCNT, 0);
  }
  Heap_Release(buffer);
  Event_SetFlag(0x101);
}

void Func_020042e0(void) {
  u8 *buffer = Heap_AllocEwram(0x4000);
  u8 *frames;
  s32 x, y, i, t, scrollX;
  u16 scrollY;
  s32 mode;

  Resource_DecodeType01(gWorldMapPackedTilesB, buffer);
  frames = buffer + 0x2000;
  Resource_DecodeType01(gWorldMapPackedPicturesB, frames);
  IO_WRITE_QUEUE_PUSH(0x050001c0, (u32)gWorldMapPalettesB, 0x80000010);
  IO_WRITE_QUEUE_PUSH(0x06000000, (u32)buffer, 0x84000400);
  IO_WRITE_QUEUE_PUSH(0x06001000, (u32)frames, 0x84000800);
  QueueIoWriteDelay2(0x04000050, 0x3f41);
  QueueIoWriteDelay2(0x04000052, 0x1010);
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 352;
  IO_WRITE_QUEUE_PUSH(0x04000008, 0xc200, 0x20000);
  y = 0x2d60;
  x = 561;
  for (i = 95; i >= 0; i--) {
    /* FAKEMATCH(temporary): x * 16 held in t before the divide; as scrollX =
     * x * 16 / 128, 22 of 227 instructions differ in tla-en. */
    t = x * 16;
    scrollX = t / 128;
    scrollY = -y / 128 + 352;
    gBgScroll[0].x = scrollX;
    gBgScroll[0].y = scrollY;
    System_Wait(1);
    y += 352;
    x += 17;
  }
  Event_PlaySound(164);
  System_Wait(120);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  // mode holds one 0 for the rest of the function; the closing scroll reset
  // stores it again.
  mode = 0;
  PaletteFade_ApplyTarget(0x7fff, mode);
  PaletteFade_Interpolate(1);
  QueueIoWriteDelay2(0x04000052, 0x1000);
  System_Wait(1);
  PaletteFade_ApplyTarget(0, 0);
  PaletteFade_Interpolate(60);
  Event_PlaySound(78);
  System_Wait(60);
  Event_PlaySound(390);
  System_Wait(120);
  Ram_HeapSlots->objectSystem->suspended = 1;
  System_Wait(1);
  Window_LoadGraphics();
  DebugText_Clear();
  WindowColor_Apply(gMainRoutineWork.paletteGlow[0],
                gMainRoutineWork.paletteGlow[1]);
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
}

void Func_02004500(struct Actor *object) {
  if ((gFrameCount & 2) != 0) {
    Animation_ApplyChildValues(object, 7);
  } else {
    Animation_ApplyChildValues(object, 0);
  }
  if ((gFrameCount & 15) == 0) {
    Func_02004634(object);
  }
}

void Func_02004538(struct Actor *object) {
  u32 frame = gFrameCount;

  if (frame & 1)
    Animation_ApplyChildValues(object, (frame >> 1) % 6);
  if ((gFrameCount & 15) == 0)
    Func_02004634(object);
}

void Func_02004570(struct Actor *object) {
  u32 frame = gFrameCount;

  if (frame & 1)
    Animation_ApplyChildValues(object, (frame >> 1) % 6);
}

void Func_02004598(struct Actor *object) {
  struct Actor *parent = object->watchedObject;
  s32 step = ++object->timer;
  s32 scale;
  s32 rise;

  if (step > 31) {
    Func_02004b38(object);
    return;
  }
  scale = Math_Sin(step << 10);
  object->scaleX = scale;
  object->scaleY = scale;

  rise = 0x10000;
  object->x = parent->x;
  object->y += rise;
  object->z = parent->z + (rise - scale) * 5 + 0x80000;
}

void Func_020045e4(struct Actor *object) {
  struct Actor *parent = object->watchedObject;
  s32 step = ++object->timer;
  s32 scale;
  s32 rise;

  if (step > 31) {
    Func_02004b38(object);
    return;
  }
  scale = Math_Sin(step << 10);
  object->scaleX = scale;
  object->scaleY = -scale;

  rise = 0x10000;
  object->x = parent->x;
  object->y += rise;
  object->z = parent->z - (rise - scale) * 5 + 0x100000;
}

s32 Animation_ApplyChildArgument(struct AnimationGroup *state, s32 flags);

void Func_02004634(struct Actor *parent) {
  struct Actor *pair[2];
  struct Actor *child;
  struct FieldSprite *sprite;
  struct LinkedEffectWork *work = Ram_HeapSlots->effectWork;
  s32 i;

  for (i = 0; i < 2; ++i) {
    child = (FieldActor_Create(271, parent->x, parent->y,
                                                parent->z));
    pair[i] = child;
    if (child != NULL) {
      child->terrainHeight = parent->terrainHeight;
      sprite = child->sprite;
      child->motionFlags = 0;
      child->timer = 0;
      child->watchedObject = parent;
      if (sprite != NULL) {
        Animation_ApplyChildArgument(sprite, 0);
        sprite->nextFrame = 0;
        Resource_ResetEntry(sprite->vramBlock);
        sprite->vramBlock = work->vramBlock;
        // A one-bit field: as a byte `|= 1` its zero mask moves the
        // motionFlags address out of r3 (differ 15).
        ((struct SpriteCacheBits *)&sprite->unk11)->retainCacheEntry = 1;
        sprite->tile =
            (gResourceTableEntries[sprite->vramBlock].offset >> 5) & 0x3ff;
        sprite->fullColor = 0;
        sprite->shape = 1;
        ((struct WorldMapOam *)sprite)->size = 2;
        ((struct LinkedDetail *)sprite->record)->field16 = 0;
      }
    }
  }
  {
    struct Actor *p = pair[0];
    struct FieldSprite *sp = p->sprite;

    p->update = (void *)Func_020045e4;
    sp->priority = 1;
  }
  {
    struct Actor *p = pair[1];
    struct FieldSprite *sp = p->sprite;

    sp->priority = 1;
    p->update = (void *)Func_02004598;
    p->priorityFlags = 2;
  }
}

#include "call.h"
#include "task.h"
#include "input.h"

/* VIEW(s3b38): variable gData_02006e0c as s16 gData_02006e0c; module/scene/scene.h has u8 gData_02006e0c[]; tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/scene/world_map/s3b38.c:447: `gData_02006e0c' has an incomplete type. */
extern s16 gData_02006e0c__s3b38 __asm__("gData_02006e0c");
/* VIEW(s3b38): variable gData_02006e0e as s16 gData_02006e0e; module/scene/scene.h has u8 gData_02006e0e[]; tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/scene/world_map/s3b38.c:532: array subscript is not an integer. */
extern s16 gData_02006e0e__s3b38 __asm__("gData_02006e0e");
/* VIEW(s3b38): variable gData_02006f20 as u8 gData_02006f20[][32]; module/scene/scene.h has u8 gData_02006f20[]; tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/scene/world_map/s3b38.c:492: subscripted value is neither array nor pointer. */
extern u8 gData_02006f20__s3b38[][32] __asm__("gData_02006f20");

void Func_02004744(s32 progress) {
  s32 level = ((progress * 30 + 0x8000) >> 16) + 1;
  s32 kind = 0;
  s32 count, base, i;

  if (progress != 0)
    kind = 100;
  if (level <= 15 && (gFrameCount & 15) <= 1)
    kind = 200;
  if (level <= 7 && (gFrameCount & 7) <= 1)
    kind = 200;
  if (kind + level == gData_02006e0c__s3b38)
    return;
  gData_02006e0c__s3b38 = kind + level;
  Call2((void (*)())Iwram_ClearWords, (s32)(Data_02006e20), (s32)(0x100));
  {
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWords;

    if (kind == 0) {
      copy(Data_02006e20, gData_02006f20__s3b38[8], 0x80);
      goto done;
    }
    if (kind == 100)
      copy(Data_02006e20, gData_02006f20__s3b38[4], 0x80);
    else
      copy(Data_02006e20, gData_02006f20__s3b38[8], 0x80);
  }
  base = 0;
  count = level;
  if (count > 7) {
    Iwram_CopyWords(Data_02006e20, gData_02006f20__s3b38, 32);
    base = 8;
    count -= 8;
  }
  if (level > 15) {
    Iwram_CopyWords(Data_02006e20[1], gData_02006f20__s3b38[1], 32);
    count = level;
    base = 16;
    count -= 16;
  }
  if (level > 23) {
    Iwram_CopyWords(Data_02006e20[2], gData_02006f20__s3b38[2], 32);
    count = level;
    base = 24;
    count -= 24;
  }
  if (level > 30) {
    Iwram_CopyWords(Data_02006e20[3], gData_02006f20__s3b38[3], 32);
    base = 0;
    count = 0;
  }
  if (count != 0) {
    i = 0;
    if (i >= count)
      goto done;
  loop:
    {
      s32 j;
      u8 *source = &gData_02006f20__s3b38[base / 8][4] + i / 2;
      u8 *dest = &Data_02006e20[base / 8][4] + i / 2;
      /* FAKEMATCH(barrier): the empty asm on dest changes its allocation rank;
       * as no asm, 22 of 181 instructions differ in tla-en. */
      asm("" : "=r"(dest) : "0"(dest));

      j = 2;
      if ((i & 1) == 0) {
        j = 1;
      even:
        *dest = (*dest & 240) | (*source & 15);
        j++;
        source += 4;
        dest += 4;
        if (j > 3)
          goto next;
        goto even;
      } else {
      oddLoop:
        *dest = (*dest & 15) | (*source & 240);
        j--;
        source += 4;
        dest += 4;
        if (j >= 0)
          goto oddLoop;
      }
    }
  next:
    i++;
    if (i < count)
      goto loop;
  }
done:
  Vram_Load(gData_02006e0e__s3b38, 0x100, Data_02006e20);
}

// The world map cursor: counts down while A or B is held, redraws the gauge and
// queues its sprite over the leader.
void Func_020048f4(void) {
  u32 *oam = Data_02006e14;
  s32 tile = gResourceTableEntries[gData_02006e0e__s3b38].offset >> 5;
  struct Actor *leader = Actor_Get(FieldParty_GetLeaderActor());
  s32 screen[3];
  u32 attributes;

  if ((gInput.held & 3) != 0 && Event_GetFlag(0x106) == 0 && leader->y > 0x40000) {
    if (--gData_02006e10 <= 0) {
      Func_02004c08(1);
      gData_02006e10 = 10;
    }
  }
  Func_02004744(Func_02004c10());
  attributes = 0x80004000;
  Call2((void (*)())Render_ProjectPoint, (s32)(&leader->x), (s32)(screen));
  {
    s32 x = screen[0] - 16;
    s32 y = screen[1] - 32;

    *oam++ = 0;
    *oam++ = (y | (x << 16)) | attributes;
  }
  *oam = tile | 0x400;
  Func_02004ad0(Data_02006e14, 255);
}

void Func_020049bc(void) {
  Resource_DecodeType01(gWorldMapPackedTilesC, gData_02006f20__s3b38);
  gData_02006e0e__s3b38 = Resource_FindFreeEntry();
  Vram_Load(gData_02006e0e__s3b38, 0x100, 0);
  gData_02006e0c__s3b38 = -1;
  gData_02006e10 = 10;
  Task_AddFrame(Func_020048f4, TASK_PASS_VBLANK - 10);
}

void Func_02004a1c(void) {
  Resource_DecodeType01(gWorldMapPackedTilesC, gData_02006f20__s3b38);
  Resource_ResetEntry(gData_02006e0e__s3b38);
  gData_02006e0c__s3b38 = -1;
  FieldTask_Remove(Func_020048f4);
}

#include "scene_tables.h"

void Func_02003418(void);
void Func_020039a8(void);

// clang-format off
u32 gData_02004e38[] = {
    39, 2, 48, 0x23010000, 0x16cd0000, 1,
    3, 17,
};

u32 gData_02004e58[] = {
    39, 2, 48, 0x22f10000, 0x16dd0000, 1,
    48, 0x23010000, 0x16cd0000, 1, 3, 17,
};

u32 gData_02004e88[] = {
    39, 2, 48, 0x22f10000, 0x16dd0000, 1,
    48, 0x23010000, 0x16cd0000, 1, 3, 17,
};

u32 gData_02004eb8[] = {
    5, 0x180000, 0, 0, 1, 13,
    10, 0, 22, 35, 1, 5,
    0x180000, 0, 0, 1, 5, 0x180000,
    0, 0, 1, 17,
};

s32 gData_02004f10[] = {
    23, 5, (s32)0xffffe667, 23, 7, 0x1999,
    23, 9, 0xe00, 23, 10, -384,
    0, 1, 13, 48, 0, 2,
    0, 0, 0, 0, 1, 38,
};

s32 gData_02004f70[] = {
    46,
    (s32)Func_02003418,
    0,
    1,
    13,
    120,
    0,
    38,
};

s32 gData_02004f90[] = {
    46,
    (s32)Func_020039a8,
    38,
};

u32 gData_02004f9c[] = {
    0, 39, 22, 5, 0, 22,
    7, 0, 0, 1, 38,
};

u32 gData_02004fc8[] = {
    0, 9, 0, 6,
};
// clang-format on

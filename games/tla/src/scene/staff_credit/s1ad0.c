/// @file
/// Scene code of overlay 0x64c, in ROM order, from s1ad0's offset up to the next object the overlay links, and the overlay's data.

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "field_sprite.h"
#include "vram_tab.h"
#include "actor_view.h"
#include "graphics/resource.h"
#include "event_inline.h"
#include "call.h"
#include "iwram_call.h"
#include "task.h"
#include "frame.h"

struct LinkedDetail {
  u8 unk00[22];
  u8 unk16;
};

struct LinkedEffectWork {
  u8 unk000[0x7c0];
  u16 vramBlock;
};

struct LinkedSpriteView {
  u8 unk00[6];
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u8 unk08[9];
  u8 retainCacheEntry : 1;
  u8 unk11 : 7;
};

void Func_02001a34(struct Actor *object);
void Func_02001a80(struct Actor *object);

void Func_02001ad0(struct Actor *parent) {
  struct Actor *pair[2];
  struct Actor *child;
  struct FieldSprite *sprite;
  struct LinkedEffectWork *work = Ram_HeapSlots->effectWork;
  s32 i;

  for (i = 0; i < 2; i++) {
    child = (FieldActor_Create(271, parent->x, parent->y,
                                                parent->z));
    pair[i] = child;
    if (child != NULL) {
      child->terrainHeight = parent->terrainHeight;
      sprite = child->animation;
      child->motionFlags = 0;
      child->action = 0;
      child->watchedObject = parent;
      if (sprite != NULL) {
        Animation_ApplyChildArgument((struct ActorSpriteView *)sprite, 0);
        sprite->nextFrame = 0;
        Resource_ResetEntry(sprite->vramBlock);
        sprite->vramBlock = work->vramBlock;
        /* FAKEMATCH(signature): the sprite written through LinkedSpriteView,
         * which splits the byte at 0x11 and attribute 1's bits 14-15 that
         * FieldSprite leaves whole; as sprite->unk11 |= 1, flipX = 0 and
         * flipY = 1, 44 of 120 instructions differ in tla-en. */
        ((struct LinkedSpriteView *)sprite)->retainCacheEntry = 1;
        sprite->tile =
            (gResourceTableEntries[sprite->vramBlock].offset >> 5) & 0x3ff;
        sprite->fullColor = 0;
        sprite->shape = 1;
        ((struct LinkedSpriteView *)sprite)->size = 2;
        ((struct LinkedDetail *)sprite->record)->unk16 = 0;
      }
    }
  }
  {
    struct Actor *p = pair[0];
    struct FieldSprite *sp = p->animation;

    p->update = (void *)Func_02001a80;
    sp->priority = 1;
  }
  {
    struct Actor *p = pair[1];
    struct FieldSprite *sp = p->animation;

    sp->priority = 1;
    p->update = (void *)Func_02001a34;
    p->priorityFlags = 2;
  }
}

extern s16 gData_02003fc4;
extern s16 gData_02003fc6;
extern u8 gData_02003fd8[][32];
extern u8 gData_020040d8[][32];

void Func_02001be0(s32 progress) {
  /* FAKEMATCH(wrapper): calls forwarded through inlines (call.h's Call and
   * Value, this file's own); as direct calls, 2 of 181 instructions differ in
   * tla-en. */
  s32 level = ((progress * 30 + 0x8000) >> 16) + 1;
  s32 kind = 0;
  s32 count, base, i;

  if (progress != 0)
    kind = 100;
  if (level <= 15 && (gFrameCount & 15) <= 1)
    kind = 200;
  if (level <= 7 && (gFrameCount & 7) <= 1)
    kind = 200;
  if (kind + level == gData_02003fc4)
    return;
  gData_02003fc4 = kind + level;
  Call2((void (*)())Iwram_ClearWords, (s32)(gData_02003fd8), (s32)(0x100));
  {
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWords;

    if (kind == 0) {
      copy(gData_02003fd8, gData_020040d8[8], 0x80);
      goto done;
    }
    if (kind == 100)
      copy(gData_02003fd8, gData_020040d8[4], 0x80);
    else
      copy(gData_02003fd8, gData_020040d8[8], 0x80);
  }
  base = 0;
  count = level;
  if (count > 7) {
    Iwram_CopyWords(gData_02003fd8, gData_020040d8, 32);
    base = 8;
    count -= 8;
  }
  if (level > 15) {
    Iwram_CopyWords(gData_02003fd8[1], gData_020040d8[1], 32);
    count = level;
    base = 16;
    count -= 16;
  }
  if (level > 23) {
    Iwram_CopyWords(gData_02003fd8[2], gData_020040d8[2], 32);
    count = level;
    base = 24;
    count -= 24;
  }
  if (level > 30) {
    Iwram_CopyWords(gData_02003fd8[3], gData_020040d8[3], 32);
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
      u8 *source = &gData_020040d8[base / 8][4] + i / 2;
      u8 *dest = &gData_02003fd8[base / 8][4] + i / 2;
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
  Vram_Load(gData_02003fc6, 0x100, gData_02003fd8);
}

extern u32 gInput;
extern s16 gData_02003fc8;
extern u32 Data_02003fcc[3];

void Func_0200200c(s32 value);
s32 Func_02002014(void);
void Render_ProjectPoint(s32 *position, s32 *screen);
void Func_02001f7c(u32 *oam, s32 priority);

// The same cursor as the world map's, in this overlay: counts down while A or B
// is held, redraws the gauge and queues its sprite over the leader.
void Func_02001d90(void) {
  u32 *oam = Data_02003fcc;
  s32 tile = gResourceTableEntries[gData_02003fc6].offset >> 5;
  struct Actor *leader = Actor_Get(FieldParty_GetLeaderActor());
  s32 screen[3];
  u32 attributes;

  if ((gInput & 3) != 0 && Event_GetFlag(0x106) == 0 && leader->y > 0x40000) {
    if (--gData_02003fc8 <= 0) {
      Func_0200200c(1);
      gData_02003fc8 = 10;
    }
  }
  Func_02001be0(Func_02002014());
  attributes = 0x80004000;
  Call2((void (*)())Render_ProjectPoint, (s32)(&leader->x), (s32)(screen));
  {
    s32 x = screen[0] - 16;
    s32 y = screen[1] - 32;

    *oam++ = 0;
    *oam++ = (y | (x << 16)) | attributes;
  }
  *oam = tile | 0x400;
  Func_02001f7c(Data_02003fcc, 255);
}

extern const u8 gWorldMapPackedTilesC[];

void Func_02001f54(const u8 *a, u8 *b);

void Func_02001e58(void) {
  Func_02001f54(gWorldMapPackedTilesC, gData_020040d8);
  gData_02003fc6 = Resource_FindFreeEntry();
  Vram_Load(gData_02003fc6, 0x100, 0);
  gData_02003fc4 = -1;
  gData_02003fc8 = 10;
  Task_AddFrame(Func_02001d90, TASK_PASS_VBLANK - 10);
}

void Func_02001eb8(void) {
  Func_02001f54(gWorldMapPackedTilesC, gData_020040d8);
  Resource_ResetEntry(gData_02003fc6);
  gData_02003fc4 = -1;
  FieldTask_Remove(Func_02001d90);
}

#include "scene_tables.h"

void Func_02001064(void);

s32 gData_02002e74[] = {
    46,
    (s32)Func_02001064,
    38,
};

// clang-format off
u32 gData_02002e80[] = {
    0, 39, 22, 5, 0, 22,
    7, 0, 0, 1, 38,
};

u32 gData_02002eac[] = {
    0, 9, 0, 6,
};
// clang-format on

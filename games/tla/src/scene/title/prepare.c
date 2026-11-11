#include "types.h"
#include "scene_tables.h"
#include "resource.h"
#include "dma.h"
#include "heap_release.h"
#include "io_reg.h"
#include "vram_tab.h"
#include "input.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "event_inline.h"

struct TitleSprite {
  u32 attr[3];
};

extern char SceneId_Title;
extern char SceneId_Ov650;
extern char ResourceId_FarCall00B;
extern char ResourceId_IntroTilesB;
extern s16 gTitleVramBlock;
extern s16 gData_020006dc;
extern struct TitleSprite gData_020006f0[];
extern u32 gFrameCount;

void Fade_ToBlack(s32 vblanks);
void Fade_Wait(void);
void Func_0200031c(s32 unused);
void Func_020005ac(struct TitleSprite *sprite, s32 priority);
void Func_020005bc(s32 resource);
s32 Func_020005d4(void);
void Func_020005e4(void);
void Func_020005ec(void);
void Func_02000604(s32 scene, s32 entrance);
void Func_0200061c(s32 value);
s32 Func_02000624(s32 value);
void Func_0200062c(s32 value);
void Func_02000634(s32 value);
void Func_0200063c(s32 value);

/// Runs the title by entrance: at entrance 10 the title is revealed and waits
/// up to 3600 frames for a key before the title scene starts again at
/// entrance 2; entrances 8, 1, 2 and 3 each lead to their own scene.
// ☀️'s Title_Run with ⚓️'s entrances.
s32 Func_0200005c(void) {
  s32 frames;
  s32 limit;

  if (gMainRoutineWork.entrance == 10) {
    Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
    Sound_Play(75);
    Func_0200031c(0);
    wait(120);
    // Goto loops: the native reloads gInput's address on every pass,
    // which loop.c hoists out of a for loop.
    frames = 0;
    goto test;
  next:
    frames++;
  test:
    limit = 3599;
    /* FAKEMATCH(barrier): the bound goes through an empty asm, so 3599 is
     * compared with bgt; as frames > 3599, which combine canonicalizes to 3600
     * with bge, 4 instructions differ. */
    __asm__("" : "+l"(limit));
    if (frames > limit)
      goto out;
    wait(1);
    if (gInput.pressed == 0)
      goto next;
  out:
    Func_02000604((s32)&SceneId_Title, 2);
    return 0;
  }
  if (gMainRoutineWork.entrance == 8) {
    Sound_Play(68);
    Func_0200061c(1);
    Sound_Play(78);
    Fade_ToBlack(60);
    Fade_Wait();
    Event_Wait(240);
    Sound_Play(0);
    Func_02000604((s32)&SceneId_Title, 2);
    return 0;
  }
  Func_020005bc((s32)&ResourceId_FarCall00B);
  if (gMainRoutineWork.entrance == 1) {
    for (;;) {
      Sound_Play(0);
      Func_0200062c(0);
      Func_02000634(0);
      if (Func_020005d4() > 0) {
        Sound_Play(40);
        Func_0200063c(1);
        if (Func_02000624(1) == 0) {
          Sound_Play(78);
          Fade_ToBlack(30);
          Fade_Wait();
          /* FAKEMATCH(dead): an empty once-only block keeps the counter's zero
           * after the wait call; without it, sched2 fills the call's stall
           * with it and 2 instructions differ. */
          do {
          } while (0);
          frames = 0;
          goto test2;
        next2:
          frames++;
        test2:
          if (frames > 119)
            continue;
          wait(1);
          if (gInput.pressed != 0)
            continue;
          goto next2;
        }
      }
      break;
    }
    Func_02000604((s32)&SceneId_Ov64a, 1);
  } else {
    if (gMainRoutineWork.entrance == 2) {
      Sound_Play(40);
      Func_0200063c(0);
      Func_0803f9c0(gMainRoutineWork.paletteGlow[0], gMainRoutineWork.paletteGlow[1]);
      Func_020005e4();
      Func_02000604((s32)&SceneId_VenusLighthouse, 99);
    }
    if (gMainRoutineWork.entrance == 3) {
      Sound_Play(75);
      Func_02000624(0);
      Func_0803f9c0(gMainRoutineWork.paletteGlow[0], gMainRoutineWork.paletteGlow[1]);
      Func_020005ec();
      Func_02000604((s32)&SceneId_Ov650, 13);
    }
  }
  Sound_Play(78);
  Fade_ToBlack(30);
  Fade_Wait();
  Event_Wait(60);
  Sound_Play(0);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Title_LoadSprites(s32 unused) {
  u8 *buffer;
  volatile s32 *dma;

  buffer = Heap_AllocEwram(0x520);
  if (gTitleVramBlock == -1)
    gTitleVramBlock = Resource_FindFreeEntry();
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_IntroTilesB),
                        buffer);
  Dma_Set(buffer, (void *)&OBJ_PLTT_COLOR(15, 0), DMA_COPY_WORDS(8), REG_DMA3);
  VramCache_Load(gTitleVramBlock, 0x500, buffer + 32);
  dma = (volatile s32 *)REG_DMA3;
  while (dma[2] < 0)
    ;
  Heap_Release(buffer);
}

/// Rebuilds the row of 18 sprites under the logo and pushes those the reveal
/// has reached, hiding the two newest on odd frames, then counts the reveal
/// on.
// ☀️'s Title_RevealSpriteRow.
void Func_02000278(void) {
  u32 *w;
  struct TitleSprite *p;
  s32 tile;
  s32 i;
  s32 n;
  s32 y;
  s32 x;

  p = gData_020006f0;
  w = gData_020006f0[0].attr;
  {
    register u32 offset asm("r3") =
        gResourceTableEntries[gTitleVramBlock].offset;

    tile = offset >> 5;
  }
  i = 0;
  y = 0x88;
  do {
    /* FAKEMATCH(temporary): the row's y copied to n, so 0x88 is rebuilt in n's
     * register each pass; as y used directly, which stays in r8, 20
     * instructions differ. */
    n = y;
    x = 232 - (18 - i) * 8;
    *w++ = 0;
    *w++ = (x << 16) | n | 0x8400;
    *w++ = 0xf000 | tile;
    n = gData_020006dc / 2 - i;
    if (n < 0)
      n = 0;
    if (n <= 2 && (gFrameCount & 1))
      n = 0;
    if (n != 0)
      Func_020005ac(p++, 255);
    tile += 2;
  } while (++i <= 17);
  gData_020006dc++;
}

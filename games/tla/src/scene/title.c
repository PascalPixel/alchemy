/// @file
/// The scene code of overlay 0x649 that its scene tables belong to, in ROM order; the rest of the overlay is in title/.

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
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
#include "task.h"
#include "io_write_queue.h"
#include "ram_buffer.h"
#include "bg_scroll.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

struct TitleSprite {
  u32 attr[3];
};

extern char SceneId_Ov650;
extern char ResourceId_FarCall00B;
extern char ResourceId_IntroTilesB;
extern s16 gTitleVramBlock;
extern s16 gData_020006dc;
extern struct TitleSprite gData_020006f0[];
extern u32 gFrameCount;

void Fade_ToBlack(s32 vblanks);
void Fade_Wait(void);

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
     * with bge, 19 of 155 instructions differ in tla-en. */
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
           * after the wait call, where sched2 would fill the call's stall
           * with it; without it, 2 of 155 instructions differ in tla-en. */
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
     * register each pass; as y used directly, which stays in r8, 20 of
     * 70 instructions differ in tla-en. */
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

void Title_LoadBackground(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

/// The title screen appears: the logo and its sprite row are loaded, the
/// blend fades the logo in over 17 steps and the screen transition opens.
void Func_0200031c(void) {
  s32 i;

  Title_LoadBackground();
  Event_Wait(30);
  gData_020006dc = 0;
  Title_LoadSprites(0);
  Task_Add(Func_02000278, TASK_PASS_VBLANK);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_ON | DISPCNT_BG2_ON | DISPCNT_BG0_ON |
                          DISPCNT_OBJ_1D_MAP,
                      IO_WRITE_16);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT,
                      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
                          BLDCNT_EFFECT_DARKEN | BLDCNT_TGT2_BG0 |
                          BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 |
                          BLDCNT_TGT2_BD,
                      IO_WRITE_16);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDY, 16, IO_WRITE_16);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, BLEND_ALPHA(16, 16), IO_WRITE_16);
  Event_Wait(120);
  // A goto loop: it carries no loop notes, so each pass reloads the queue and
  // interrupt register addresses; as a for or do-while loop, 90 instructions
  // differ in tla-en.
  i = 0;
fade:
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDY, 16 - i, IO_WRITE_16);
  wait(3);
  if (++i <= 16)
    goto fade;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 436) = 1;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 436) = 60;
}

struct TitleWork {
  u8 unk00[20];
  u16 mode;
};

extern char ResourceId_LogoOnBlack;

static __inline__ void Decode_(void *source, void *destination) {
  /* FAKEMATCH(wrapper): the decode forwarded through this inline keeps the
   * buffer address a constant of each call, as The Broken Seal's system call
   * layer does; as a direct call, which keeps the address in r5, 17
   * of 94 instructions differ in tla-en. */
  Resource_DecodeType01(source, destination);
}

/// The title logo: its palette, its tiles decoded and copied to VRAM, a 30 by
/// 20 tilemap of consecutive tiles with two blank cells closing each row, and
/// the four scroll pairs cleared.
void Title_LoadBackground(void) {
  u8 *encodedGraphics;
  s32 resourceId;
  u16 *tilemap;
  u32 screenX;
  u32 screenY;
  u16 tileIndex;
  struct BgScroll *bgScroll;
  s32 blankTile;

  resourceId = (s32)&ResourceId_LogoOnBlack;
  Fade_ToBlack(0);
  REG_BG2CNT = 0x681;
  gBgScroll[2].y = 0;
  blankTile = 0x1ff;
  encodedGraphics = Resource_GetTableEntry(resourceId);
  Dma_Set(encodedGraphics, (void *)BG_PLTT, DMA_COPY_WORDS(112), REG_DMA3);
  encodedGraphics += 0x1c0;
  Decode_(encodedGraphics, Ram_MapCellBuffer);
  Dma_Set((void *)Ram_MapCellBuffer, (void *)0x06006800,
          DMA_COPY_WORDS(0x9600 / 4), REG_DMA3);

  // Fill the visible 30 columns and pad each 32-entry tilemap row.
  tilemap = (u16 *)0x06003000;
  tileIndex = 0x1a0;
  // Written with goto: as for loops, the pinned column counter keeps its exit
  // test ahead of the loop (differ 17).
  screenY = 0;
row:
  screenX = 0;
column:
  *tilemap++ = tileIndex++;
  screenX++;
  if (screenX <= 29)
    goto column;
  *tilemap++ = blankTile;
  *tilemap++ = blankTile;
  screenY++;
  if (screenY <= 19)
    goto row;
  // Reset all four background scroll pairs.
  bgScroll = gBgScroll;
  screenY = 0;
scroll_pair:
  bgScroll->y = 0;
  bgScroll->x = 0;
  bgScroll++;
  screenY++;
  if (screenY <= 3)
    goto scroll_pair;
  Dma_Set(gBgScroll, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
  ((struct TitleWork *)Ram_HeapSlots->mapWork)->mode = 0x1400;
}

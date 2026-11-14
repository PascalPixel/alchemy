/// @file
/// The title: the Nintendo and Camelot logos, the intro, the title screen with
/// its row of sprites revealed under the logo, and the starts it leads to, the
/// prologue with Jenna or the game with Felix; entrance 8 plays the credits.

#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "graphics/resource.h"
#include "system/dma.h"
#include "system/heap_release.h"
#include "system/io_reg.h"
#include "graphics/vram_tab.h"
#include "system/input.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "field/event_inline.h"
#include "system/task.h"
#include "system/io_write_queue.h"
#include "system/ram_buffer.h"
#include "bg_scroll.h"
#include "system/frame.h"
#include "field/field_sequence_event.h"
#include "module/game/game.h"
#include "module/scene/scene.h"
#include "module/system/system.h"
#include "resource_ids.h"

extern s16 gTitleVramBlock;

void Credits_Run(s32 value);

s32 Title_ShowIntro(s32 value);

void TitleIntro_Run(s32 value);

void Title_LoadBackground(void);

struct TitleSprite__title;

extern s16 gTitle_RevealStep;

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

/* VIEW(title): tag struct TitleSprite as struct TitleSprite__title;
 * module/tla.h has another definition; tried with the definition the rest of
 * the object has, it does not compile in tla-ja:
 * games/tla/src/scene/title.c:194: invalid use of undefined type `struct
 * TitleSprite'. */
struct TitleSprite__title {
  u32 attr[3];
};

extern struct TitleSprite__title gTitle_SpriteRow[];

s32 Save_ScanFileSummaries(void);
void Title_ShowNintendoLogo(s32 value);
void Title_ShowCamelotLogo(s32 value);

/// Runs the title by entrance: at entrance 10 the title is revealed and waits
/// up to 3600 frames for a key before the title scene starts again at
/// entrance 2; entrances 8, 1, 2 and 3 each lead to their own scene.
// ☀️'s Title_Run with ⚓️'s entrances.
s32 Title_Run(void) {
  s32 frames;

  if (gMainRoutineWork.entrance == 10) {
    Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
    Event_PlaySound(MUS_075);
    Title_RevealScreen(0);
    wait(120);
    for (frames = 0; frames <= 3599; frames++) {
      wait(1);
      if (gInput.pressed != 0)
        break;
    }
    Event_SetDestination((s32)&SceneId_Title, 2);
    return 0;
  }
  if (gMainRoutineWork.entrance == 8) {
    Event_PlaySound(MUS_068);
    Credits_Run(1);
    Event_PlaySound(SOUND_FADE_OUT_BGM);
    ScreenFade_ToBlack(60);
    ScreenFade_Wait();
    Event_Wait(240);
    Event_PlaySound(MUS_000);
    Event_SetDestination((s32)&SceneId_Title, 2);
    return 0;
  }
  Resource_ReservedNoOp((s32)&ResourceId_FarCall00B);
  if (gMainRoutineWork.entrance == 1) {
    for (;;) {
      Event_PlaySound(MUS_000);
      Title_ShowNintendoLogo(0);
      Title_ShowCamelotLogo(0);
      if (Save_ScanFileSummaries() > 0) {
        Event_PlaySound(MUS_040);
        TitleIntro_Run(1);
        if (Title_ShowIntro(1) == 0) {
          Event_PlaySound(SOUND_FADE_OUT_BGM);
          ScreenFade_ToBlack(30);
          ScreenFade_Wait();
          /* FAKEMATCH(dead): an empty once-only block keeps the counter's zero
           * after the wait call, where sched2 would fill the call's stall with
           * it. */
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
    Event_SetDestination((s32)&SceneId_Clear, 1);
  } else {
    if (gMainRoutineWork.entrance == 2) {
      Event_PlaySound(MUS_040);
      TitleIntro_Run(0);
      FieldWindowColor_Apply(gMainRoutineWork.paletteGlow[0],
                             gMainRoutineWork.paletteGlow[1]);
      Party_SetLeaderJenna();
      Event_SetDestination((s32)&SceneId_VenusLighthouse, 99);
    }
    if (gMainRoutineWork.entrance == 3) {
      Event_PlaySound(MUS_075);
      Title_ShowIntro(0);
      FieldWindowColor_Apply(gMainRoutineWork.paletteGlow[0],
                             gMainRoutineWork.paletteGlow[1]);
      Party_SetLeaderFelix();
      Event_SetDestination((s32)&SceneId_Idejima, 13);
    }
  }
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  ScreenFade_ToBlack(30);
  ScreenFade_Wait();
  Event_Wait(60);
  Event_PlaySound(MUS_000);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Title_LoadSprites(s32 unused) {
  u8 *buffer;
  volatile s32 *dma;

  buffer = Heap_AllocEwram(0x520);
  if (gTitleVramBlock == -1)
    gTitleVramBlock = VramCache_FindFreeEntry();
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_IntroTilesB),
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
void Title_RevealSpriteRow(void) {
  u32 *w;
  struct TitleSprite__title *p;
  s32 tile;
  s32 i;
  s32 n;
  s32 y;
  s32 x;

  p = gTitle_SpriteRow;
  w = gTitle_SpriteRow[0].attr;
  tile = gResourceTableEntries[gTitleVramBlock].offset >> 5;
  i = 0;
  y = 0x88;
  do {
    /* FAKEMATCH(temporary): the row's y copied to n. */
    n = y;
    x = 232 - (18 - i) * 8;
    *w++ = 0;
    *w++ = (x << 16) | n | 0x8400;
    *w++ = 0xf000 | tile;
    n = gTitle_RevealStep / 2 - i;
    if (n < 0)
      n = 0;
    if (n <= 2 && (gFrameCount & 1))
      n = 0;
    if (n != 0)
      Oam_AddSpriteAtDepthClamped(p++, 255);
    tile += 2;
  } while (++i <= 17);
  gTitle_RevealStep++;
}

/// The title screen appears: the logo and its sprite row are loaded, the
/// blend fades the logo in over 17 steps and the screen transition opens.
void Title_RevealScreen(void) {
  s32 i;

  Title_LoadBackground();
  Event_Wait(30);
  gTitle_RevealStep = 0;
  Title_LoadSprites(0);
  Task_Add(Title_RevealSpriteRow, TASK_PASS_VBLANK);
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
  i = 0;
fade:
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDY, 16 - i, IO_WRITE_16);
  wait(3);
  if (++i <= 16)
    goto fade;
  gFieldWork->transition = 0;
  gFieldWork->transitionFrames = 1;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  gFieldWork->transitionFrames = 60;
}

static __inline__ void Decode_(void *source, void *destination) {
  /* FAKEMATCH(wrapper): the decode forwarded through this inline keeps the
   * buffer address a constant of each call. */
  Decompress_TaggedLz(source, destination);
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
  ScreenFade_ToBlack(0);
  REG_BG2CNT = 0x681;
  gBgScroll[2].y = 0;
  blankTile = 0x1ff;
  encodedGraphics = Resource_GetTableEntry(resourceId);
  Dma_Set(encodedGraphics, (void *)BG_PLTT, DMA_COPY_WORDS(112), REG_DMA3);
  encodedGraphics += 0x1c0;
  Decode_(encodedGraphics, Ram_MapCellBuffer);
  Dma_Set((void *)Ram_MapCellBuffer, BG_SCREEN_BLOCK(13),
          DMA_COPY_WORDS(0x9600 / 4), REG_DMA3);

  // Fill the visible 30 columns and pad each 32-entry tilemap row.
  tilemap = (u16 *)0x06003000;
  tileIndex = 0x1a0;
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

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 0, 0, 0, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_TABLE_END},
};

s16 gTitleVramBlock = -1;

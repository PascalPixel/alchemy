/// @file
/// The epilogue: the party's last words, line after line in a message window
/// with the speaker's name on a green panel beside it, over the ending's
/// pictures, from "Finally, we've..." (MsgFinallyWeVe) to The End; then it
/// waits for a button, or three minutes, and returns to the title.

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
#include "system/frame.h"
#include "item_icon.h"
#include "module/scene/scene.h"
#include "module/window/window.h"
#include "graphics/resource.h"
#include "scene_ids.h"
#include "graphics/uiflow.h"
#include "call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field/event_inline.h"
#include "edition.h"
#include "bg_scroll.h"
#include "system/main_routine_work.h"
#include "system/input.h"
#include "system/ram_buffer.h"

struct EpilogueSprite;

void EffectSprite_Draw(struct EpilogueSprite *sprite);

s32 EndingPicture_LowerTopStrip();

extern s32 gEpilogueMessage; ///< the next line to show
/// Nonzero while a speaker's name is shown on the green panel.
extern s8 gEpilogueNameShown;
extern s16 gEpilogueNameX; ///< the name's left edge, in pixels
extern s16 gEpilogueNameY; ///< the name's line, in pixels
extern struct EpilogueSprite gEpilogueNamePanel;
extern s32 gEpilogueNamePanelEntry; ///< the panel's VRAM cache entry
extern s32 gEpilogueNamePanelTile;  ///< the panel's first tile

extern const u16 gEpilogueNamePanelPalette[];
extern char ResourceId_GreenPanel;

s32 EndingPicture_Show();
#include "text/msg_ids.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

/// Each frame while a name is shown, keeps the green panel under it.
void Epilogue_PlaceNamePanel(void) {
  if (gEpilogueNameShown != 0) {
    gEpilogueNamePanel.unk0c = gEpilogueNameX << 16;
    gEpilogueNamePanel.unk10 = gEpilogueNameY << 16;
    EffectSprite_Draw(&gEpilogueNamePanel);
  }
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

void EffectSprite_Init(struct EpilogueSprite *sprite, s32 a, s32 b, u32 c,
                       s32 tile);

static __inline__ void Set(struct EpilogueSprite *sprite, s32 a, s32 b, u32 c,
                           s32 tile) {
  /* FAKEMATCH(wrapper): forwarded inline. */
  EffectSprite_Init(sprite, a, b, c, tile);
}

/// Loads the green panel the speakers' names are written on, and starts
/// Epilogue_PlaceNamePanel.
void Epilogue_LoadNamePanel(void) {
  /* FAKEMATCH(wrapper): the size through Value1. */
  void *buffer = (void *)Value1((s32 (*)())Heap_Alloc, 0x800);
  struct EpilogueSprite *sprite;
  s32 *slot;
  s32 *tileSlot;
  s8 zero;
  s32 priority;
  s32 value;

  Dma_Set(gEpilogueNamePanelPalette, (void *)0x050003c0, 0x80000010, REG_DMA3);
  sprite = &gEpilogueNamePanel;
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_GreenPanel),
                      buffer);
  slot = &gEpilogueNamePanelEntry;
  value = VramCache_FindFreeEntry();
  *slot = value;
  tileSlot = &gEpilogueNamePanelTile;
  value = Vram_Load(value, 0x800, buffer);
  *tileSlot = value;
  Set(sprite, 0, 0, 0xc0000000, value);
  sprite->unk1e = 240;
  sprite->unk09_2 = 0;
  sprite->unk05_5 = 0;
  sprite->unk09_4 = 14;
  Heap_Release(buffer);
  zero = 0;
  gEpilogueNameShown = zero;
  priority = 0x480;
  FieldTask_AddFrame(Epilogue_PlaceNamePanel, priority);
}

struct EpilogueWindow *Message_OpenWindow(s32 id, s32 x, s32 y, s32 resource);

/// Opens the epilogue's message window at x and y and, unless bit 15 of the
/// message is set, a name window at left and line that Epilogue_PlaceNamePanel
/// follows; in mode 2 it restarts the window's text. Then it waits for the
/// text, closes both windows and waits the given frames.
void Epilogue_ShowLineAt(s32 message, s32 x, s32 y, s32 left, s32 line,
                         s32 frames, s32 mode) {
  s32 wide;
  struct EpilogueWindow *window;
  struct EpilogueWindow *name;

  name = 0;
  wide = 0x8000 & message;
  message &= 0x7fff;
  window = Message_OpenWindow(gEpilogueMessage, x, y,
                              (Speaker_GetVoice(message) << 16) | 4);
  if (!wide) {
    /* FAKEMATCH(temporary): the name window held in a temporary until after the
     * stores. */
    struct EpilogueWindow *opened = Portrait_OpenWindow(message, 0, left, line);

    gEpilogueNameShown = 1;
    gEpilogueNameX = left << 3;
    gEpilogueNameY = line << 3;
    name = opened;
  }
  if (mode == 2) {
    window->unk0e++;
    window->unk08 = 0;
    window->unk0a = 0;
  }
  while (!Message_AllTyped())
    System_Wait(1);
  System_Wait(1);
  Window_Close(window, 2);
  if (!wide) {
    Window_Close(name, 2);
    gEpilogueNameShown = wide;
  }
  if (frames > 0) {
    s32 i = frames;

    do {
      System_Wait(1);
    } while (--i != 0);
  }
  gEpilogueMessage++;
}

/// Shows the epilogue's next line, spoken by speaker (its portrait and name),
/// centred on the screen's top (position 0), its middle (1) or its foot (2),
/// then holds it frames more.

void Text_GetMessageSize();

void Epilogue_ShowLine(s32 message, s32 position, s32 frames) {
  s32 x;
  s32 y;
  s32 width;
  s32 height;
  s32 mode = 0;
  s32 left;
  s32 line = 0;
  s32 half;
  s32 wide;

  // Default: nothing to the left yet; x and y start from it.
  left = 0;
  x = left;
  y = left;
  wide = message & 0x8000;
  Text_GetMessageSize(gEpilogueMessage, &x, &y, &width, &height);
  if (width <= 24) {
    if (!wide)
      x = (25 - width) / 2 + 5;
    else
      x = (30 - width) / 2;
    left = x - 5;
  } else {
    x = (30 - width) / 2;
    left = x;
  }
  half = (height - 1) / 2;
  if (position == 0) {
    y = 3;
    line = half + 1;
  } else if (position == 1) {
    y = 14 - half;
    line = y + half - 2;
  } else if (position == 2) {
    if (half == 3) {
      y = 13;
      line = 15;
      mode = 2;
    } else {
      y = 16;
      line = 15;
    }
  }
  Epilogue_ShowLineAt(message, x, y, left, line, frames, mode);
}

s32 SceneState_SetScrollTarget();
s32 EndingPicture_WaitForScroll();
s32 WindowSystem_InitField();
extern char ResourceId_EndingCliffDayPicture,
    ResourceId_EndingCliffSunsetPicture, ResourceId_EndingVillagersPictureA,
    ResourceId_EndingEmbracePicture, ResourceId_EndingReunionPicture,
    ResourceId_EndingSunsetPlainPicture, ResourceId_EndingTheEndPicture;

#if LANGUAGE == LANGUAGE_JAPANESE
#define TILES_FLAG_OFFSET 0x118c
#else
#define TILES_FLAG_OFFSET 0x138c
#endif

s32 Scene_Initialize(void) {
  struct MainRoutineWork *work;
  u8 *eventMode;
  s32 saved;
  s32 zero, one, value;
  s32 count;
  void *p;
  u8 *actor;
  u8 *tiles;
  struct BgScroll *scroll;

  work = &gMainRoutineWork;
  eventMode = &work->eventMode;
  saved = *eventMode;
  actor = (u8 *)Actor_Get(work->currentOwner);
  zero = 0;
  actor[85] = zero;
  one = 1;
  *eventMode = one;
  EndingPicture_Start();
  WindowSystem_InitField();
  tiles = Ram_HeapSlots->windowTiles;
  tiles[6] = one;
  tiles[TILES_FLAG_OFFSET] = one;
  gEpilogueMessage = (s32)MsgFinallyWeVe;
  Epilogue_LoadNamePanel();
  FieldEvent_Wait(30);
  Call3((void (*)())Epilogue_ShowLine, 5, 0, 40);
  Call3((void (*)())Epilogue_ShowLine, 1, 1, 40);
  Call3((void (*)())Epilogue_ShowLine, 6, 0, 40);
  Call3((void (*)())Epilogue_ShowLine, 2, 1, 40);
#if LANGUAGE == LANGUAGE_GERMAN
  Call3((void (*)())Epilogue_ShowLine, 2, 1, 40);
#endif
  Call3((void (*)())Epilogue_ShowLine, 3, 0, 30);
  Call3((void (*)())Epilogue_ShowLine, 7, 1, 30);
  Call3((void (*)())Epilogue_ShowLine, 56, 0, 40);
  Call3((void (*)())Epilogue_ShowLine, 60, 1, 30);
  Call3((void (*)())Epilogue_ShowLine, 61, 1, 40);
  Call3((void (*)())Epilogue_ShowLine, 0, 0, 40);
  Call3((void (*)())Epilogue_ShowLine, 62, 1, 30);
  Call3((void (*)())Epilogue_ShowLine, 0, 0, 30);
  Call3((void (*)())Epilogue_ShowLine, 0, 0, 30);
  Call3((void (*)())Epilogue_ShowLine, 5, 1, 40);
  Call3((void (*)())Epilogue_ShowLine, 4, 0, 30);
  Epilogue_ShowLine(1, 1, 30);
  Event_PlaySound(MUS_067);
  scroll = gBgScroll;
  scroll[3].y = 16;
  scroll[2].y = 16;
  Call3((void (*)())EndingPicture_Show, (s32)&ResourceId_EndingCliffDayPicture,
        (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(100);
  Call3((void (*)())Epilogue_ShowLine, 60, 2, 30);
  Call3((void (*)())Epilogue_ShowLine, 61, 2, 40);
  Call3((void (*)())Epilogue_ShowLine, 0, 2, 30);
  Call3((void (*)())Epilogue_ShowLine, 62, 2, 30);
  Call3((void (*)())Epilogue_ShowLine, 1, 2, 40);
#if LANGUAGE == LANGUAGE_GERMAN
  Call3((void (*)())Epilogue_ShowLine, 5, 2, 40);
#endif
  Epilogue_ShowLine(5, 2, 40);
  SceneState_SetScrollTarget(120);
  System_Wait(1);
  EndingPicture_WaitForScroll();
  FieldEvent_Wait(30);
  Call3((void (*)())EndingPicture_Show,
        (s32)&ResourceId_EndingCliffSunsetPicture, (s32)Ram_MapCellBuffer, 120);
  FieldEvent_Wait(120);
  Call3((void (*)())Epilogue_ShowLine, 5, 2, 30);
  Call3((void (*)())Epilogue_ShowLine, 7, 2, 30);
  Call3((void (*)())Epilogue_ShowLine, 56, 2, 40);
  Call3((void (*)())Epilogue_ShowLine, 4, 2, 40);
  Call3((void (*)())Epilogue_ShowLine, 4, 2, 40);
  Call3((void (*)())Epilogue_ShowLine, 4, 2, 40);
  Call3((void (*)())Epilogue_ShowLine, 4, 2, 40);
  Call3((void (*)())Epilogue_ShowLine, 4, 2, 30);
#if LANGUAGE == LANGUAGE_GERMAN
  Call3((void (*)())Epilogue_ShowLine, 4, 2, 30);
#endif
  Call3((void (*)())Epilogue_ShowLine, 0, 2, 30);
  Call3((void (*)())Epilogue_ShowLine, 62, 2, 30);
  Call3((void (*)())Epilogue_ShowLine, 1, 2, 40);
  Call3((void (*)())Epilogue_ShowLine, 1, 2, 40);
  Call3((void (*)())Epilogue_ShowLine, 6, 2, 30);
  Call3((void (*)())Epilogue_ShowLine, 5, 2, 30);
#if LANGUAGE == LANGUAGE_GERMAN
  Call3((void (*)())Epilogue_ShowLine, 5, 2, 30);
#endif
  Call3((void (*)())Epilogue_ShowLine, 3, 2, 40);
#if LANGUAGE == LANGUAGE_GERMAN
  Call3((void (*)())Epilogue_ShowLine, 1, 2, 30);
#endif
  Epilogue_ShowLine(1, 2, 30);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Call3((void (*)())Epilogue_ShowLine, 0x8040, 2, 30);
  Call3((void (*)())Epilogue_ShowLine, 0, 2, 30);
  Call3((void (*)())Epilogue_ShowLine, 0x8041, 2, 30);
  Call3((void (*)())Epilogue_ShowLine, 0, 2, 30);
  Call3((void (*)())Epilogue_ShowLine, 0x8045, 2, 30);
  Call3((void (*)())Epilogue_ShowLine, 1, 2, 30);
  Call3((void (*)())EndingPicture_Show,
        (s32)&ResourceId_EndingVillagersPictureA, (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(60);
  Event_PlaySound(MUS_073);
  FieldEvent_Wait(30);
  Call3((void (*)())Epilogue_ShowLine, 64, 2, 30);
#if LANGUAGE == LANGUAGE_GERMAN
  Call3((void (*)())Epilogue_ShowLine, 64, 2, 30);
#endif
  Call3((void (*)())Epilogue_ShowLine, 65, 2, 30);
  Call3((void (*)())Epilogue_ShowLine, 67, 2, 30);
  Call3((void (*)())Epilogue_ShowLine, 62, 2, 30);
  Call3((void (*)())Epilogue_ShowLine, 70, 2, 30);
  Call3((void (*)())Epilogue_ShowLine, 68, 2, 30);
  Call3((void (*)())Epilogue_ShowLine, 56, 2, 30);
  Call3((void (*)())Epilogue_ShowLine, 1, 2, 40);
  Epilogue_ShowLine(56, 2, 40);
  SceneState_SetScrollTarget(200);
  System_Wait(1);
  EndingPicture_WaitForScroll();
  FieldEvent_Wait(30);
  Call3((void (*)())Epilogue_ShowLine, 62, 2, 20);
  Call3((void (*)())Epilogue_ShowLine, 0, 2, 40);
  Call3((void (*)())Epilogue_ShowLine, 66, 2, 20);
  Call3((void (*)())Epilogue_ShowLine, 62, 2, 30);
  Call3((void (*)())Epilogue_ShowLine, 66, 2, 40);
  Call3((void (*)())EndingPicture_Show, 0, (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(60);
  scroll[3].y = zero;
  scroll[2].y = zero;
  EndingPicture_LowerTopStrip(0);
  EndingPicture_Show((s32)&ResourceId_EndingEmbracePicture,
                     (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(60);
  goto L_02008692;
L_0200868c:
  System_Wait(1);
L_02008692:
  p = &gInput;
  if (((struct InputState *)p)->pressed == 0)
    goto L_0200868c;
  Call3((void (*)())EndingPicture_Show, (s32)&ResourceId_EndingReunionPicture,
        (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(120);
  SceneState_SetScrollTarget(120);
  System_Wait(1);
  EndingPicture_WaitForScroll();
  FieldEvent_Wait(180);
  Call3((void (*)())EndingPicture_Show,
        (s32)&ResourceId_EndingSunsetPlainPicture, (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(300);
  EndingPicture_Show((s32)&ResourceId_EndingTheEndPicture,
                     (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(180);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  p = &gMainRoutineWork;
  ((struct MainRoutineWork *)p)->eventMode = saved;
  FieldEvent_Wait(300);
  count = 0;
  if (gInput.pressed != 0)
    goto L_0200871e;
L_02008704:
  System_Wait(1);
  count++;
  if (count > 10799)
    goto L_0200871e;
  if (gInput.pressed == 0)
    goto L_02008704;
L_0200871e:
  EndingPicture_Show(0, (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(60);
  value = 0;
  /* FAKEMATCH(dead): the store in a once-only block. */
  do {
    REG_BLDCNT = value;
  } while (0);
  value = 0x1040;
  REG_DISPCNT = value;
  Window_LoadGraphics();
  DebugText_Clear();
  gRenderOamEnabled = 1;
  Event_ClearFlag(0x17c);
  Call2((void (*)())Event_SetDestination, (s32)&SceneId_Clear, 2);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

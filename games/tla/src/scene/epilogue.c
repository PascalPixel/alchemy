/// @file
/// The epilogue: the party's last words, line after line in a message window
/// with the speaker's name on a green panel beside it, over the ending's
/// pictures, from "Finally, we've..." (MsgFinallyWeVe) to The End; then it
/// waits for a button, or three minutes, and returns to the title.

#include "flags.h"
#include "resource_ids.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "macros.h"
#include "field/scene_tables.h"
#include "system/frame.h"
#include "window/item_icon.h"
#include "scene/scene.h"
#include "scene/event_layer.h"
#include "field/effect_sprite.h"
#include "window/window.h"
#include "graphics/resource.h"
#include "scene_ids.h"
#include "graphics/uiflow.h"
#include "field/call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field/event_inline.h"
#include "edition.h"
#include "system/bg_scroll.h"
#include "system/main_routine_work.h"
#include "system/input.h"
#include "system/ram_buffer.h"
#include "system/io_write.h"

s32 EndingPicture_LowerTopStrip();

/// The panel's VRAM cache entry.
s32 gEpilogueNamePanelEntry BSS_DATA ALIGNED(16);
u8 gEpilogue_UnusedAfterNamePanelEntry[268] BSS_DATA;
struct EffectSprite gEpilogueNamePanel BSS_DATA;
s32 gEpilogueMessage BSS_DATA;       ///< the next line to show
s16 gEpilogueNameY BSS_DATA;         ///< the name's line, in pixels
s32 gEpilogueNamePanelTile BSS_DATA; ///< the panel's first tile
/// Nonzero while a speaker's name is shown on the green panel.
s8 gEpilogueNameShown BSS_DATA;
s16 gEpilogueNameX BSS_DATA ALIGNED(4); ///< the name's left edge, in pixels

extern const u16 gEpilogueNamePanelPalette[];

s32 EndingPicture_Show();
#include "text/msg_ids.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Each frame while a name is shown, keeps the green panel under it.
void Epilogue_PlaceNamePanel(void) {
  if (gEpilogueNameShown != 0) {
    gEpilogueNamePanel.x = toFIXED(gEpilogueNameX);
    gEpilogueNamePanel.y = toFIXED(gEpilogueNameY);
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

static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

/// Loads the green panel the speakers' names are written on, and starts
/// Epilogue_PlaceNamePanel.
void Epilogue_LoadNamePanel(void) {
  void *buffer = BumpAlloc(64 * 64 / 2);
  struct EffectSprite *sprite;
  s32 *slot;
  s32 *tileSlot;
  s32 value;

  Dma_Set(gEpilogueNamePanelPalette, (void *)&OBJ_PLTT_COLOR(14, 0),
          DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
  sprite = &gEpilogueNamePanel;
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_GREEN_PANEL), buffer);
  slot = &gEpilogueNamePanelEntry;
  value = VramCache_FindFreeEntry();
  *slot = value;
  tileSlot = &gEpilogueNamePanelTile;
  value = Vram_Load(value, 64 * 64 / 2, buffer);
  *tileSlot = value;
  FieldEffectSprite_Init(sprite, 0, 0, OBJ_SIZE_64X64, value);
  sprite->depth = 240;
  sprite->oam.priority = 0;
  sprite->oam.fullColor = 0;
  sprite->oam.palette = 14;
  Heap_Release(buffer);
  gEpilogueNameShown = 0;
  FieldTask_AddFrame(Epilogue_PlaceNamePanel, TASK_PASS_FRAME);
}

struct Window *Message_OpenWindow(s32 message, s32 x, s32 y, u32 packed);

/// A speaker (a sprite number) or'd with this shows the line without the
/// speaker's name.
#define EPILOGUE_NO_NAME 0x8000

/// Opens the epilogue's message window at x and y and, unless the speaker has
/// EPILOGUE_NO_NAME, a name window at left and line that
/// Epilogue_PlaceNamePanel follows; in mode 2 it restarts the window's text.
/// Then it waits for the text, closes both windows and waits the given frames.
void Epilogue_ShowLineAt(s32 speaker, s32 x, s32 y, s32 left, s32 line,
                         s32 frames, s32 mode) {
  s32 nameless;
  struct Window *window;
  struct Window *name;

  name = 0;
  nameless = EPILOGUE_NO_NAME & speaker;
  speaker &= EPILOGUE_NO_NAME - 1;
  window = Message_OpenWindow(gEpilogueMessage, x, y,
                              (Speaker_GetVoice(speaker) << 16) | 4);
  if (!nameless) {
    name = Portrait_OpenWindow(speaker, 0, left, line);
    gEpilogueNameShown = 1;
    gEpilogueNameX = left << 3;
    gEpilogueNameY = line << 3;
  }
  if (mode == 2) {
    window->y++;
    window->width = 0;
    window->height = 0;
  }
  while (!Message_AllTyped())
    System_Wait(1);
  System_Wait(1);
  Window_Close(window, 2);
  if (!nameless) {
    Window_Close(name, 2);
    gEpilogueNameShown = nameless;
  }
  if (frames > 0) {
    s32 i = frames;

    do {
      System_Wait(1);
    } while (--i != 0);
  }
  gEpilogueMessage++;
}

void Text_GetMessageSize();

/// Shows the epilogue's next line, spoken by speaker (its portrait and name),
/// centred on the screen's top (position 0), its middle (1) or its foot (2),
/// then holds it frames more.
void Epilogue_ShowLine(s32 speaker, s32 position, s32 frames) {
  s32 x;
  s32 y;
  s32 width;
  s32 height;
  s32 mode = 0;
  s32 left;
  s32 line = 0;
  s32 half;
  s32 nameless;

  // Default: nothing to the left yet; x and y start from it.
  left = 0;
  x = left;
  y = left;
  nameless = speaker & EPILOGUE_NO_NAME;
  Text_GetMessageSize(gEpilogueMessage, &x, &y, &width, &height);
  if (width <= 24) {
    if (!nameless)
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
  Epilogue_ShowLineAt(speaker, x, y, left, line, frames, mode);
}

s32 SceneState_SetScrollTarget();
s32 EndingPicture_WaitForScroll();
s32 WindowSystem_InitField();

/// Runs the whole epilogue, picture after picture, then sets out for the
/// clear screen. Returns 0.
s32 Scene_Initialize(void) {
  struct MainRoutineWork *work;
  s32 savedSpeed;
  s32 count;
  void *p;
  struct Actor *leader;
  struct WindowWork *windowWork;
  struct BgScroll *scroll;

  work = &gMainRoutineWork;
  savedSpeed = work->messageSpeed;
  leader = Actor_Get(work->leaderActorId);
  leader->motionFlags = 0;
  work->messageSpeed = 1;
  EndingPicture_Start();
  WindowSystem_InitField();
  windowWork = (struct WindowWork *)Ram_HeapSlots->windowWork;
  windowWork->holdTilemapUpload = 1;
  windowWork->textMode = 1;
  gEpilogueMessage = (s32)MsgFinallyWeVe;
  Epilogue_LoadNamePanel();
  FieldEvent_Wait(30);
  Epilogue_ShowLine(SPRITE_JENNA, 0, 40);
  Epilogue_ShowLine(SPRITE_GARET, 1, 40);
  Epilogue_ShowLine(SPRITE_SHEBA, 0, 40);
  Epilogue_ShowLine(SPRITE_IVAN, 1, 40);
#if LANGUAGE == LANGUAGE_GERMAN
  Epilogue_ShowLine(SPRITE_IVAN, 1, 40);
#endif
  Epilogue_ShowLine(SPRITE_MIA, 0, 30);
  Epilogue_ShowLine(SPRITE_PIERS, 1, 30);
  Epilogue_ShowLine(SPRITE_KRADEN, 0, 40);
  Epilogue_ShowLine(SPRITE_JENNAS_FATHER, 1, 30);
  Epilogue_ShowLine(SPRITE_JENNAS_MOTHER, 1, 40);
  Epilogue_ShowLine(SPRITE_ISAAC, 0, 40);
  Epilogue_ShowLine(SPRITE_KYLE, 1, 30);
  Epilogue_ShowLine(SPRITE_ISAAC, 0, 30);
  Epilogue_ShowLine(SPRITE_ISAAC, 0, 30);
  Epilogue_ShowLine(SPRITE_JENNA, 1, 40);
  Epilogue_ShowLine(SPRITE_FELIX, 0, 30);
  Epilogue_ShowLine(SPRITE_GARET, 1, 30);
  Event_PlaySound(MUS_EPILOGUE);
  scroll = gBgScroll;
  scroll[3].y = 16;
  scroll[2].y = 16;
  Call3((void (*)())EndingPicture_Show, RESOURCE_ENDING_CLIFF_DAY_PICTURE,
        (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(100);
  Epilogue_ShowLine(SPRITE_JENNAS_FATHER, 2, 30);
  Epilogue_ShowLine(SPRITE_JENNAS_MOTHER, 2, 40);
  Epilogue_ShowLine(SPRITE_ISAAC, 2, 30);
  Epilogue_ShowLine(SPRITE_KYLE, 2, 30);
  Epilogue_ShowLine(SPRITE_GARET, 2, 40);
#if LANGUAGE == LANGUAGE_GERMAN
  Epilogue_ShowLine(SPRITE_JENNA, 2, 40);
#endif
  Epilogue_ShowLine(SPRITE_JENNA, 2, 40);
  SceneState_SetScrollTarget(120);
  System_Wait(1);
  EndingPicture_WaitForScroll();
  FieldEvent_Wait(30);
  Call3((void (*)())EndingPicture_Show, RESOURCE_ENDING_CLIFF_SUNSET_PICTURE,
        (s32)Ram_MapCellBuffer, 120);
  FieldEvent_Wait(120);
  Epilogue_ShowLine(SPRITE_JENNA, 2, 30);
  Epilogue_ShowLine(SPRITE_PIERS, 2, 30);
  Epilogue_ShowLine(SPRITE_KRADEN, 2, 40);
  Epilogue_ShowLine(SPRITE_FELIX, 2, 40);
  Epilogue_ShowLine(SPRITE_FELIX, 2, 40);
  Epilogue_ShowLine(SPRITE_FELIX, 2, 40);
  Epilogue_ShowLine(SPRITE_FELIX, 2, 40);
  Epilogue_ShowLine(SPRITE_FELIX, 2, 30);
#if LANGUAGE == LANGUAGE_GERMAN
  Epilogue_ShowLine(SPRITE_FELIX, 2, 30);
#endif
  Epilogue_ShowLine(SPRITE_ISAAC, 2, 30);
  Epilogue_ShowLine(SPRITE_KYLE, 2, 30);
  Epilogue_ShowLine(SPRITE_GARET, 2, 40);
  Epilogue_ShowLine(SPRITE_GARET, 2, 40);
  Epilogue_ShowLine(SPRITE_SHEBA, 2, 30);
  Epilogue_ShowLine(SPRITE_JENNA, 2, 30);
#if LANGUAGE == LANGUAGE_GERMAN
  Epilogue_ShowLine(SPRITE_JENNA, 2, 30);
#endif
  Epilogue_ShowLine(SPRITE_MIA, 2, 40);
#if LANGUAGE == LANGUAGE_GERMAN
  Epilogue_ShowLine(SPRITE_GARET, 2, 30);
#endif
  Epilogue_ShowLine(SPRITE_GARET, 2, 30);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Epilogue_ShowLine(EPILOGUE_NO_NAME | SPRITE_MAN_IN_TEAL_SHIRT, 2, 30);
  Epilogue_ShowLine(SPRITE_ISAAC, 2, 30);
  Epilogue_ShowLine(EPILOGUE_NO_NAME | SPRITE_WOMAN_IN_GREEN_HEADSCARF, 2, 30);
  Epilogue_ShowLine(SPRITE_ISAAC, 2, 30);
  Epilogue_ShowLine(EPILOGUE_NO_NAME | SPRITE_BOY_WITH_ORANGE_HAIR, 2, 30);
  Epilogue_ShowLine(SPRITE_GARET, 2, 30);
  Call3((void (*)())EndingPicture_Show, RESOURCE_ENDING_VILLAGERS_PICTURE_A,
        (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(60);
  Event_PlaySound(MUS_EPILOGUE_VILLAGERS);
  FieldEvent_Wait(30);
  Epilogue_ShowLine(SPRITE_MAN_IN_TEAL_SHIRT, 2, 30);
#if LANGUAGE == LANGUAGE_GERMAN
  Epilogue_ShowLine(SPRITE_MAN_IN_TEAL_SHIRT, 2, 30);
#endif
  Epilogue_ShowLine(SPRITE_WOMAN_IN_GREEN_HEADSCARF, 2, 30);
  Epilogue_ShowLine(SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, 2, 30);
  Epilogue_ShowLine(SPRITE_KYLE, 2, 30);
  Epilogue_ShowLine(SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, 2, 30);
  Epilogue_ShowLine(SPRITE_YOUNG_MAN_WITH_RED_HAIR, 2, 30);
  Epilogue_ShowLine(SPRITE_KRADEN, 2, 30);
  Epilogue_ShowLine(SPRITE_GARET, 2, 40);
  Epilogue_ShowLine(SPRITE_KRADEN, 2, 40);
  SceneState_SetScrollTarget(200);
  System_Wait(1);
  EndingPicture_WaitForScroll();
  FieldEvent_Wait(30);
  Epilogue_ShowLine(SPRITE_KYLE, 2, 20);
  Epilogue_ShowLine(SPRITE_ISAAC, 2, 40);
  Epilogue_ShowLine(SPRITE_DORA, 2, 20);
  Epilogue_ShowLine(SPRITE_KYLE, 2, 30);
  Epilogue_ShowLine(SPRITE_DORA, 2, 40);
  Call3((void (*)())EndingPicture_Show, 0, (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(60);
  scroll[3].y = 0;
  scroll[2].y = 0;
  EndingPicture_LowerTopStrip(0);
  EndingPicture_Show(RESOURCE_ENDING_EMBRACE_PICTURE, (s32)Ram_MapCellBuffer,
                     0);
  FieldEvent_Wait(60);
  // The embrace stays until a button is pressed.
  goto checkForPress;
waitForPress:
  System_Wait(1);
checkForPress:
  p = &gInput;
  if (((struct InputState *)p)->pressed == 0)
    goto waitForPress;
  Call3((void (*)())EndingPicture_Show, RESOURCE_ENDING_REUNION_PICTURE,
        (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(120);
  SceneState_SetScrollTarget(120);
  System_Wait(1);
  EndingPicture_WaitForScroll();
  FieldEvent_Wait(180);
  Call3((void (*)())EndingPicture_Show, RESOURCE_ENDING_SUNSET_PLAIN_PICTURE,
        (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(300);
  EndingPicture_Show(RESOURCE_ENDING_THE_END_PICTURE, (s32)Ram_MapCellBuffer,
                     0);
  FieldEvent_Wait(180);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  p = &gMainRoutineWork;
  ((struct MainRoutineWork *)p)->messageSpeed = savedSpeed;
  FieldEvent_Wait(300);
  // The End stays until a button is pressed, or for three minutes.
  count = 0;
  if (gInput.pressed == 0) {
    do {
      System_Wait(1);
      count++;
      if (count >= 3 * 60 * FRAMES_PER_SECOND)
        break;
    } while (gInput.pressed == 0);
  }
  EndingPicture_Show(0, (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(60);
  Io_Write16(0, &REG_BLDCNT);
  Io_Write16(DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON, &REG_DISPCNT);
  Window_LoadGraphics();
  DebugText_Clear();
  gOamCopyEnabled = 1;
  Event_ClearFlag(FLAG_CANNOT_SAVE_HERE);
  Event_SetDestination(SCENE_CLEAR, 2);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

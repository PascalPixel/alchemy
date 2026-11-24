/// @file
/// The epilogue: the party's last words, line after line, each with the
/// speaker's portrait in a frame beside it, over the ending's images, from
/// "Finally! We've reached Vale." (MsgFinallyWeVe) to The End; then it waits
/// for a button, or three minutes, and goes on to the file screen.

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
#include "graphics/window_close.h"
#include "field/call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field/event_inline.h"
#include "edition.h"
#include "system/bg_scroll.h"
#include "system/main_routine_state.h"
#include "system/input.h"
#include "system/ram_buffer.h"
#include "system/io_write.h"
#include "window/message_speed.h"
#include "scene_exits.h"

void EndingImage_LowerTopStrip(s32 enable);

/// The portrait frame's VRAM cache entry.
s32 gEpiloguePortraitFrameEntry BSS_DATA ALIGNED(16);
/// Unused: 268 bytes after the frame's entry that the epilogue never touches.
u8 gEpilogue_UnusedAfterPortraitFrameEntry[268] BSS_DATA;
/// The frame drawn round each portrait, a sprite under the portrait.
struct EffectSprite gEpiloguePortraitFrame BSS_DATA;
s32 gEpilogueMessage BSS_DATA;           ///< the next line to show
s16 gEpiloguePortraitY BSS_DATA;         ///< the portrait's top, in pixels
s32 gEpiloguePortraitFrameTile BSS_DATA; ///< the frame's first tile
/// Nonzero while a portrait is shown, and the frame drawn round it.
s8 gEpiloguePortraitShown BSS_DATA;
s16 gEpiloguePortraitX BSS_DATA ALIGNED(4); ///< the portrait's left, in pixels

/// The colours of the epilogue's name panel (green_panel.s).
extern const u16 gEpilogueNamePanelPalette[];

void EndingImage_Show(s32 resource, u8 *map, s32 position);
#include "text/msg_ids.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Each frame while a portrait is shown, draws its frame round it.
void Epilogue_PlacePortraitFrame(void) {
  if (gEpiloguePortraitShown != 0) {
    gEpiloguePortraitFrame.x = toFIXED(gEpiloguePortraitX);
    gEpiloguePortraitFrame.y = toFIXED(gEpiloguePortraitY);
    EffectSprite_Draw(&gEpiloguePortraitFrame);
  }
}

#include "shared/scene_entrances_mapless.c"

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

/// Loads the portraits' frame, a window border round a green panel
/// (RESOURCE_GREEN_PANEL) drawn in portrait slot 0's palette, and starts
/// Epilogue_PlacePortraitFrame.
void Epilogue_LoadPortraitFrame(void) {
  void *buffer = BumpAlloc(64 * 64 / 2);
  struct EffectSprite *sprite;
  s32 *slot;
  s32 *tileSlot;
  s32 value;

  Dma_Set(gEpilogueNamePanelPalette,
          (void *)&OBJ_PLTT_COLOR(PORTRAIT_PALETTE, 0),
          DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
  sprite = &gEpiloguePortraitFrame;
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_GREEN_PANEL), buffer);
  slot = &gEpiloguePortraitFrameEntry;
  value = VramCache_FindFreeEntry();
  *slot = value;
  tileSlot = &gEpiloguePortraitFrameTile;
  value = Vram_Load(value, 64 * 64 / 2, buffer);
  *tileSlot = value;
  FieldEffectSprite_Init(sprite, 0, 0, OBJ_SIZE_64X64, value);
  sprite->depth = OAM_DEPTH_WINDOW;
  sprite->oam.priority = 0;
  sprite->oam.fullColor = 0;
  sprite->oam.palette = PORTRAIT_PALETTE;
  Heap_Release(buffer);
  gEpiloguePortraitShown = 0;
  FieldTask_AddFrame(Epilogue_PlacePortraitFrame, TASK_PASS_FRAME);
}

struct Window *Message_OpenWindow(s32 message, s32 x, s32 y, u32 packed);
struct Window *Portrait_OpenWindow(s32 speaker, s32 portraitOption, s32 x,
                                   s32 y);

/// A speaker (a sprite number) or'd with this shows the line with no portrait:
/// a voice heard from off the image.
#define EPILOGUE_NO_PORTRAIT 0x8000

/// The epilogue's message windows appear at once, without MESSAGE_WINDOW_ZOOM,
/// and set an option bit that Message_OpenWindow never reads.
#define EPILOGUE_WINDOW_OPTIONS 4

/// Epilogue_ShowLineAt's mode for a line of seven or eight rows at the foot of
/// the screen: its window is moved a row down and given no size of its own.
#define EPILOGUE_TALL_LINE_AT_FOOT 2

/// Opens the epilogue's message window at tile (x, y) and, unless the speaker
/// has EPILOGUE_NO_PORTRAIT, the speaker's portrait at tile (portraitX,
/// portraitY) with its frame round it, the window lowered in mode
/// EPILOGUE_TALL_LINE_AT_FOOT. Then it waits for the text, closes both windows
/// and waits the given frames.
void Epilogue_ShowLineAt(s32 speaker, s32 x, s32 y, s32 portraitX,
                         s32 portraitY, s32 frames, s32 mode) {
  s32 noPortrait;
  struct Window *window;
  struct Window *portrait;

  portrait = 0;
  noPortrait = EPILOGUE_NO_PORTRAIT & speaker;
  speaker &= EPILOGUE_NO_PORTRAIT - 1;
  window = Message_OpenWindow(gEpilogueMessage, x, y,
                              MESSAGE_WINDOW_VOICE(Speaker_GetVoice(speaker)) |
                                  EPILOGUE_WINDOW_OPTIONS);
  if (!noPortrait) {
    portrait = Portrait_OpenWindow(speaker, 0, portraitX, portraitY);
    gEpiloguePortraitShown = 1;
    gEpiloguePortraitX = portraitX << 3;
    gEpiloguePortraitY = portraitY << 3;
  }
  if (mode == EPILOGUE_TALL_LINE_AT_FOOT) {
    window->y++;
    window->width = 0;
    window->height = 0;
  }
  while (!Message_AllTyped())
    System_Wait(1);
  System_Wait(1);
  Window_Close(window, 2);
  if (!noPortrait) {
    Window_Close(portrait, 2);
    gEpiloguePortraitShown = noPortrait;
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

/// Where on the screen Epilogue_ShowLine puts a line.
enum EpilogueLinePosition {
  LINE_AT_TOP,
  LINE_IN_MIDDLE,
  LINE_AT_FOOT, ///< below the image
};

/// Shows the epilogue's next line, spoken by speaker (a sprite, whose portrait
/// and voice it takes), at the given position, centred across the screen or,
/// beside a portrait, across the columns right of it; then holds it frames
/// more.
void Epilogue_ShowLine(s32 speaker, s32 position, s32 frames) {
  s32 x;
  s32 y;
  s32 width;
  s32 height;
  s32 mode = 0;
  s32 portraitX;
  s32 portraitY = 0;
  s32 half;
  s32 noPortrait;

  // Default: x, y and the portrait's column start at 0.
  portraitX = 0;
  x = portraitX;
  y = portraitX;
  noPortrait = speaker & EPILOGUE_NO_PORTRAIT;
  Text_GetMessageSize(gEpilogueMessage, &x, &y, &width, &height);
  if (width <= DISPLAY_TILE_WIDTH - PORTRAIT_WINDOW_TILES - 1) {
    if (!noPortrait)
      x = (DISPLAY_TILE_WIDTH - PORTRAIT_WINDOW_TILES - width) / 2 +
          PORTRAIT_WINDOW_TILES;
    else
      x = (DISPLAY_TILE_WIDTH - width) / 2;
    portraitX = x - PORTRAIT_WINDOW_TILES;
  } else {
    x = (DISPLAY_TILE_WIDTH - width) / 2;
    portraitX = x;
  }
  half = (height - 1) / 2;
  if (position == LINE_AT_TOP) {
    y = 3;
    portraitY = half + 1;
  } else if (position == LINE_IN_MIDDLE) {
    y = 14 - half;
    portraitY = y + half - 2;
  } else if (position == LINE_AT_FOOT) {
    if (half == 3) {
      y = 13;
      portraitY = 15;
      mode = EPILOGUE_TALL_LINE_AT_FOOT;
    } else {
      y = 16;
      portraitY = 15;
    }
  }
  Epilogue_ShowLineAt(speaker, x, y, portraitX, portraitY, frames, mode);
}

void EndingImage_SetTargetRow(u16 row);
void EndingImage_WaitForScroll(void);
void WindowSystem_InitField(void);

/// Runs the whole epilogue, image after image, then sets out for the file
/// screen, which offers to save the cleared game. Returns 0.
s32 Scene_Initialize(void) {
  struct MainRoutineState *state;
  s32 savedSpeed;
  s32 count;
  struct Actor *leader;
  struct WindowState *windowState;
  struct BgScroll *scroll;

  state = &gMainRoutineState;
  savedSpeed = state->messageSpeed;
  leader = Actor_Get(state->leaderActorId);
  leader->motionFlags = 0;
  state->messageSpeed = MESSAGE_SPEED_NORMAL;
  EndingImage_Start();
  WindowSystem_InitField();
  windowState = (struct WindowState *)Ram_HeapSlots->windowState;
  windowState->holdTilemapUpload = 1;
  windowState->textMode = 1;
  gEpilogueMessage = (s32)MsgFinallyWeVe;
  Epilogue_LoadPortraitFrame();
  FieldEvent_Wait(30);
  Epilogue_ShowLine(SPRITE_JENNA, LINE_AT_TOP, 40);
  Epilogue_ShowLine(SPRITE_GARET, LINE_IN_MIDDLE, 40);
  Epilogue_ShowLine(SPRITE_SHEBA, LINE_AT_TOP, 40);
  Epilogue_ShowLine(SPRITE_IVAN, LINE_IN_MIDDLE, 40);
#if LANGUAGE == LANGUAGE_GERMAN
  Epilogue_ShowLine(SPRITE_IVAN, LINE_IN_MIDDLE, 40);
#endif
  Epilogue_ShowLine(SPRITE_MIA, LINE_AT_TOP, 30);
  Epilogue_ShowLine(SPRITE_PIERS, LINE_IN_MIDDLE, 30);
  Epilogue_ShowLine(SPRITE_KRADEN, LINE_AT_TOP, 40);
  Epilogue_ShowLine(SPRITE_JENNAS_FATHER, LINE_IN_MIDDLE, 30);
  Epilogue_ShowLine(SPRITE_JENNAS_MOTHER, LINE_IN_MIDDLE, 40);
  Epilogue_ShowLine(SPRITE_ISAAC, LINE_AT_TOP, 40);
  Epilogue_ShowLine(SPRITE_KYLE, LINE_IN_MIDDLE, 30);
  Epilogue_ShowLine(SPRITE_ISAAC, LINE_AT_TOP, 30);
  Epilogue_ShowLine(SPRITE_ISAAC, LINE_AT_TOP, 30);
  Epilogue_ShowLine(SPRITE_JENNA, LINE_IN_MIDDLE, 40);
  Epilogue_ShowLine(SPRITE_FELIX, LINE_AT_TOP, 30);
  Epilogue_ShowLine(SPRITE_GARET, LINE_IN_MIDDLE, 30);
  Event_PlaySound(MUS_EPILOGUE);
  scroll = gBgScroll;
  scroll[3].y = 16;
  scroll[2].y = 16;
  Call3((void (*)())EndingImage_Show, RESOURCE_ENDING_CLIFF_DAY_PICTURE,
        (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(100);
  Epilogue_ShowLine(SPRITE_JENNAS_FATHER, LINE_AT_FOOT, 30);
  Epilogue_ShowLine(SPRITE_JENNAS_MOTHER, LINE_AT_FOOT, 40);
  Epilogue_ShowLine(SPRITE_ISAAC, LINE_AT_FOOT, 30);
  Epilogue_ShowLine(SPRITE_KYLE, LINE_AT_FOOT, 30);
  Epilogue_ShowLine(SPRITE_GARET, LINE_AT_FOOT, 40);
#if LANGUAGE == LANGUAGE_GERMAN
  Epilogue_ShowLine(SPRITE_JENNA, LINE_AT_FOOT, 40);
#endif
  Epilogue_ShowLine(SPRITE_JENNA, LINE_AT_FOOT, 40);
  // The image pans 120 pixels right, to the three on the cliff.
  EndingImage_SetTargetRow(120);
  System_Wait(1);
  EndingImage_WaitForScroll();
  FieldEvent_Wait(30);
  // The sunset over the cliff, shown already panned to them.
  Call3((void (*)())EndingImage_Show, RESOURCE_ENDING_CLIFF_SUNSET_PICTURE,
        (s32)Ram_MapCellBuffer, 120);
  FieldEvent_Wait(120);
  Epilogue_ShowLine(SPRITE_JENNA, LINE_AT_FOOT, 30);
  Epilogue_ShowLine(SPRITE_PIERS, LINE_AT_FOOT, 30);
  Epilogue_ShowLine(SPRITE_KRADEN, LINE_AT_FOOT, 40);
  Epilogue_ShowLine(SPRITE_FELIX, LINE_AT_FOOT, 40);
  Epilogue_ShowLine(SPRITE_FELIX, LINE_AT_FOOT, 40);
  Epilogue_ShowLine(SPRITE_FELIX, LINE_AT_FOOT, 40);
  Epilogue_ShowLine(SPRITE_FELIX, LINE_AT_FOOT, 40);
  Epilogue_ShowLine(SPRITE_FELIX, LINE_AT_FOOT, 30);
#if LANGUAGE == LANGUAGE_GERMAN
  Epilogue_ShowLine(SPRITE_FELIX, LINE_AT_FOOT, 30);
#endif
  Epilogue_ShowLine(SPRITE_ISAAC, LINE_AT_FOOT, 30);
  Epilogue_ShowLine(SPRITE_KYLE, LINE_AT_FOOT, 30);
  Epilogue_ShowLine(SPRITE_GARET, LINE_AT_FOOT, 40);
  Epilogue_ShowLine(SPRITE_GARET, LINE_AT_FOOT, 40);
  Epilogue_ShowLine(SPRITE_SHEBA, LINE_AT_FOOT, 30);
  Epilogue_ShowLine(SPRITE_JENNA, LINE_AT_FOOT, 30);
#if LANGUAGE == LANGUAGE_GERMAN
  Epilogue_ShowLine(SPRITE_JENNA, LINE_AT_FOOT, 30);
#endif
  Epilogue_ShowLine(SPRITE_MIA, LINE_AT_FOOT, 40);
#if LANGUAGE == LANGUAGE_GERMAN
  Epilogue_ShowLine(SPRITE_GARET, LINE_AT_FOOT, 30);
#endif
  Epilogue_ShowLine(SPRITE_GARET, LINE_AT_FOOT, 30);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Epilogue_ShowLine(EPILOGUE_NO_PORTRAIT | SPRITE_MAN_IN_TEAL_SHIRT,
                    LINE_AT_FOOT, 30);
  Epilogue_ShowLine(SPRITE_ISAAC, LINE_AT_FOOT, 30);
  Epilogue_ShowLine(EPILOGUE_NO_PORTRAIT | SPRITE_WOMAN_IN_GREEN_HEADSCARF,
                    LINE_AT_FOOT, 30);
  Epilogue_ShowLine(SPRITE_ISAAC, LINE_AT_FOOT, 30);
  Epilogue_ShowLine(EPILOGUE_NO_PORTRAIT | SPRITE_BOY_WITH_ORANGE_HAIR,
                    LINE_AT_FOOT, 30);
  Epilogue_ShowLine(SPRITE_GARET, LINE_AT_FOOT, 30);
  Call3((void (*)())EndingImage_Show, RESOURCE_ENDING_VILLAGERS_PICTURE_A,
        (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(60);
  Event_PlaySound(MUS_EPILOGUE_VILLAGERS);
  FieldEvent_Wait(30);
  Epilogue_ShowLine(SPRITE_MAN_IN_TEAL_SHIRT, LINE_AT_FOOT, 30);
#if LANGUAGE == LANGUAGE_GERMAN
  Epilogue_ShowLine(SPRITE_MAN_IN_TEAL_SHIRT, LINE_AT_FOOT, 30);
#endif
  Epilogue_ShowLine(SPRITE_WOMAN_IN_GREEN_HEADSCARF, LINE_AT_FOOT, 30);
  Epilogue_ShowLine(SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, LINE_AT_FOOT, 30);
  Epilogue_ShowLine(SPRITE_KYLE, LINE_AT_FOOT, 30);
  Epilogue_ShowLine(SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, LINE_AT_FOOT, 30);
  Epilogue_ShowLine(SPRITE_YOUNG_MAN_WITH_RED_HAIR, LINE_AT_FOOT, 30);
  Epilogue_ShowLine(SPRITE_KRADEN, LINE_AT_FOOT, 30);
  Epilogue_ShowLine(SPRITE_GARET, LINE_AT_FOOT, 40);
  Epilogue_ShowLine(SPRITE_KRADEN, LINE_AT_FOOT, 40);
  // The villagers' image pans 200 pixels right, to Dora by the trees.
  EndingImage_SetTargetRow(200);
  System_Wait(1);
  EndingImage_WaitForScroll();
  FieldEvent_Wait(30);
  Epilogue_ShowLine(SPRITE_KYLE, LINE_AT_FOOT, 20);
  Epilogue_ShowLine(SPRITE_ISAAC, LINE_AT_FOOT, 40);
  Epilogue_ShowLine(SPRITE_DORA, LINE_AT_FOOT, 20);
  Epilogue_ShowLine(SPRITE_KYLE, LINE_AT_FOOT, 30);
  Epilogue_ShowLine(SPRITE_DORA, LINE_AT_FOOT, 40);
  // No image: the villagers fade to black before the embrace.
  Call3((void (*)())EndingImage_Show, 0, (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(60);
  scroll[3].y = 0;
  scroll[2].y = 0;
  EndingImage_LowerTopStrip(0);
  EndingImage_Show(RESOURCE_ENDING_EMBRACE_PICTURE, Ram_MapCellBuffer, 0);
  FieldEvent_Wait(60);
  // The embrace stays until a button is pressed.
  while (gInput.pressed == 0)
    System_Wait(1);
  Call3((void (*)())EndingImage_Show, RESOURCE_ENDING_REUNION_PICTURE,
        (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(120);
  // The reunion pans 120 pixels right, along the party.
  EndingImage_SetTargetRow(120);
  System_Wait(1);
  EndingImage_WaitForScroll();
  FieldEvent_Wait(180);
  Call3((void (*)())EndingImage_Show, RESOURCE_ENDING_SUNSET_PLAIN_PICTURE,
        (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(300);
  EndingImage_Show(RESOURCE_ENDING_THE_END_PICTURE, Ram_MapCellBuffer, 0);
  FieldEvent_Wait(180);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  gMainRoutineState.messageSpeed = savedSpeed;
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
  EndingImage_Show(0, Ram_MapCellBuffer, 0);
  FieldEvent_Wait(60);
  Io_Write16(0, &REG_BLDCNT);
  Io_Write16(DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON, &REG_DISPCNT);
  Window_LoadGraphics();
  DebugText_Clear();
  gOamCopyEnabled = 1;
  Event_ClearFlag(FLAG_CANNOT_SAVE_HERE);
  Event_SetDestination(SCENE_CLEAR, CLEAR_FROM_EPILOGUE);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

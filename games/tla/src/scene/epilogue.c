/// @file
/// Overlay 0x6b3's scene: its code and its tables, in ROM order.

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
#include "frame.h"
#include "item_icon.h"
#include "module/scene/epilogue.h"
#include "module/scene/scene.h"
#include "module/window/window.h"
#include "resource.h"
#include "scene_ids.h"
#include "text/msg_ids.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

void Func_0200005c(void) {
  if (gData_02000a34 != 0) {
    gData_02000a00.unk0c = gData_02000a38 << 16;
    gData_02000a00.unk10 = gData_02000a2c << 16;
    Func_0200083c(&gData_02000a00);
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
    {TRIGGERS_END},
};

#include "call.h"
#include "dma.h"
#include "io_reg.h"
#include "event_inline.h"
#include "edition.h"
#include "bg_scroll.h"
#include "system/main_routine_work.h"
#include "system/input.h"
#include "system/ram_buffer.h"

/* VIEW(epilogue): routine Resource_DecodeType01 as void Resource_DecodeType01(void *resource, void *destination); resource.h has s32 Resource_DecodeType01(const void *source, void *destination). */
extern void Resource_DecodeType01__epilogue(void *resource, void *destination) __asm__("Resource_DecodeType01");
void Func_02000834(struct EpilogueSprite *sprite, s32 a, s32 b, u32 c,
                   s32 tile);

static __inline__ void Set(struct EpilogueSprite *sprite, s32 a, s32 b, u32 c,
                           s32 tile) {
  /* FAKEMATCH(wrapper): forwarded inline; as a direct call, 6 of 69
   * instructions differ in tla-en. */
  Func_02000834(sprite, a, b, c, tile);
}

void Func_02000098(void) {
  /* FAKEMATCH(wrapper): the size through Value1; as a direct call, 21 of 69
   * instructions differ in tla-en. */
  void *buffer = (void *)Value1((s32 (*)())Heap_Alloc, 0x800);
  struct EpilogueSprite *sprite;
  s32 *slot;
  s32 *tileSlot;
  s8 zero;
  s32 priority;
  s32 value;

  Dma_Set(Data_02000874, (void *)0x050003c0, 0x80000010, REG_DMA3);
  sprite = &gData_02000a00;
  Resource_DecodeType01__epilogue(Resource_GetTableEntry((s32)&ResourceId_GreenPanel), buffer);
  slot = &Data_020008f0;
  value = Resource_FindFreeEntry();
  *slot = value;
  tileSlot = &Data_02000a30;
  value = Vram_Load(value, 0x800, buffer);
  *tileSlot = value;
  Set(sprite, 0, 0, 0xc0000000, value);
  sprite->unk1e = 240;
  sprite->unk09_2 = 0;
  sprite->unk05_5 = 0;
  sprite->unk09_4 = 14;
  Heap_Release(buffer);
  zero = 0;
  gData_02000a34 = zero;
  priority = 0x480;
  FieldTask_AddFrame(Func_0200005c, priority);
}

struct EpilogueWindow *Message_OpenWindow(s32 id, s32 x, s32 y,
                                                s32 resource);
/* VIEW(epilogue): routine Func_02000804 as struct EpilogueWindow *Func_02000804(s32 message, s32 a, s32 left, s32 line); module/scene/champa_cliff_caves.h has void Func_02000804(s32 item). */
extern struct EpilogueWindow *Func_02000804__epilogue(s32 message, s32 a, s32 left, s32 line) __asm__("Func_02000804");
/* VIEW(epilogue): routine Window_Close as void Window_Close(struct EpilogueWindow *window, s32 mode); uiflow.h has void Window_Close(struct Window *window, s32 release). */
extern void Window_Close__epilogue(struct EpilogueWindow *window, s32 mode) __asm__("Window_Close");

/// Opens the epilogue's message window at x and y and, unless bit 15 of the
/// message is set, a name window at left and line that Func_0200005c follows;
/// in mode 2 it restarts the window's text. Then it waits for the text, closes
/// both windows and waits the given frames.
void Func_02000158(s32 message, s32 x, s32 y, s32 left, s32 line, s32 frames,
                   s32 mode) {
  s32 wide;
  struct EpilogueWindow *window;
  struct EpilogueWindow *name;

  name = 0;
  wide = 0x8000 & message;
  message &= 0x7fff;
  window = Message_OpenWindow(
      gData_02000a28, x, y, (BattleEffect_GetResourceId(message) << 16) | 4);
  if (!wide) {
    /* FAKEMATCH(temporary): the name window held in a temporary until after
     * the stores; as name = Func_02000804(...), 2 of 100 instructions differ
     * in tla-en. */
    struct EpilogueWindow *opened = Func_02000804__epilogue(message, 0, left, line);

    gData_02000a34 = 1;
    gData_02000a38 = left << 3;
    gData_02000a2c = line << 3;
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
  Window_Close__epilogue(window, 2);
  if (!wide) {
    Window_Close__epilogue(name, 2);
    gData_02000a34 = wide;
  }
  if (frames > 0) {
    s32 i = frames;

    do {
      System_Wait(1);
    } while (--i != 0);
  }
  gData_02000a28++;
}

// Draft of overlay 6b3 Func_02000244: centres a message and hands it to
// Func_02000158. The logic is right; the listing keeps x and y in their stack
// slots (it stores y in every branch and loads both once before the call),
// where this draft forwards y as a constant and reloads x in each branch, and
// the listing builds x's and y's zero in r4 apart from the mode's zero in r3.
// Tried: integer-cast addresses, "y = x = 0", and the four values as one
// struct (the stores then stay, but through a base register: 75 differ).

void Text_GetMessageSize();

void Func_02000244(s32 message, s32 position, s32 frames) {
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
  Text_GetMessageSize(gData_02000a28, &x, &y, &width, &height);
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
  Func_02000158(message, x, y, left, line, frames, mode);
}

/* VIEW(epilogue): routine DebugText_Clear as s32 DebugText_Clear(); module/system/system.h has void DebugText_Clear(void). */
extern s32 DebugText_Clear__epilogue() __asm__("DebugText_Clear");
/* VIEW(epilogue): routine Window_LoadGraphics as s32 Window_LoadGraphics(); module/system/system.h has void Window_LoadGraphics(void). */
extern s32 Window_LoadGraphics__epilogue() __asm__("Window_LoadGraphics");
/* VIEW(epilogue): routine Func_02000844 as s32 Func_02000844(); module/scene/aqua_rock.h has void Func_02000844(void). */
extern s32 Func_02000844__epilogue() __asm__("Func_02000844");
s32 Func_02000854();
s32 Func_0200085c();
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
  Func_02000844__epilogue();
  WindowSystem_InitField();
  tiles = Ram_HeapSlots->windowTiles;
  tiles[6] = one;
  tiles[TILES_FLAG_OFFSET] = one;
  gData_02000a28 = (s32)MsgFinallyWeVe;
  Func_02000098();
  FieldEvent_Wait(30);
  Call3((void (*)())Func_02000244, 5, 0, 40);
  Call3((void (*)())Func_02000244, 1, 1, 40);
  Call3((void (*)())Func_02000244, 6, 0, 40);
  Call3((void (*)())Func_02000244, 2, 1, 40);
#if LANGUAGE == LANGUAGE_GERMAN
  Call3((void (*)())Func_02000244, 2, 1, 40);
#endif
  Call3((void (*)())Func_02000244, 3, 0, 30);
  Call3((void (*)())Func_02000244, 7, 1, 30);
  Call3((void (*)())Func_02000244, 56, 0, 40);
  Call3((void (*)())Func_02000244, 60, 1, 30);
  Call3((void (*)())Func_02000244, 61, 1, 40);
  Call3((void (*)())Func_02000244, 0, 0, 40);
  Call3((void (*)())Func_02000244, 62, 1, 30);
  Call3((void (*)())Func_02000244, 0, 0, 30);
  Call3((void (*)())Func_02000244, 0, 0, 30);
  Call3((void (*)())Func_02000244, 5, 1, 40);
  Call3((void (*)())Func_02000244, 4, 0, 30);
  Func_02000244(1, 1, 30);
  Event_PlaySound(67);
  scroll = gBgScroll;
  scroll[3].y = 16;
  scroll[2].y = 16;
  Call3((void (*)())Func_0200084c, (s32)&ResourceId_EndingCliffDayPicture,
        (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(100);
  Call3((void (*)())Func_02000244, 60, 2, 30);
  Call3((void (*)())Func_02000244, 61, 2, 40);
  Call3((void (*)())Func_02000244, 0, 2, 30);
  Call3((void (*)())Func_02000244, 62, 2, 30);
  Call3((void (*)())Func_02000244, 1, 2, 40);
#if LANGUAGE == LANGUAGE_GERMAN
  Call3((void (*)())Func_02000244, 5, 2, 40);
#endif
  Func_02000244(5, 2, 40);
  Func_02000854(120);
  System_Wait(1);
  Func_0200085c();
  FieldEvent_Wait(30);
  Call3((void (*)())Func_0200084c, (s32)&ResourceId_EndingCliffSunsetPicture,
        (s32)Ram_MapCellBuffer, 120);
  FieldEvent_Wait(120);
  Call3((void (*)())Func_02000244, 5, 2, 30);
  Call3((void (*)())Func_02000244, 7, 2, 30);
  Call3((void (*)())Func_02000244, 56, 2, 40);
  Call3((void (*)())Func_02000244, 4, 2, 40);
  Call3((void (*)())Func_02000244, 4, 2, 40);
  Call3((void (*)())Func_02000244, 4, 2, 40);
  Call3((void (*)())Func_02000244, 4, 2, 40);
  Call3((void (*)())Func_02000244, 4, 2, 30);
#if LANGUAGE == LANGUAGE_GERMAN
  Call3((void (*)())Func_02000244, 4, 2, 30);
#endif
  Call3((void (*)())Func_02000244, 0, 2, 30);
  Call3((void (*)())Func_02000244, 62, 2, 30);
  Call3((void (*)())Func_02000244, 1, 2, 40);
  Call3((void (*)())Func_02000244, 1, 2, 40);
  Call3((void (*)())Func_02000244, 6, 2, 30);
  Call3((void (*)())Func_02000244, 5, 2, 30);
#if LANGUAGE == LANGUAGE_GERMAN
  Call3((void (*)())Func_02000244, 5, 2, 30);
#endif
  Call3((void (*)())Func_02000244, 3, 2, 40);
#if LANGUAGE == LANGUAGE_GERMAN
  Call3((void (*)())Func_02000244, 1, 2, 30);
#endif
  Func_02000244(1, 2, 30);
  Event_PlaySound(78);
  Call3((void (*)())Func_02000244, 0x8040, 2, 30);
  Call3((void (*)())Func_02000244, 0, 2, 30);
  Call3((void (*)())Func_02000244, 0x8041, 2, 30);
  Call3((void (*)())Func_02000244, 0, 2, 30);
  Call3((void (*)())Func_02000244, 0x8045, 2, 30);
  Call3((void (*)())Func_02000244, 1, 2, 30);
  Call3((void (*)())Func_0200084c, (s32)&ResourceId_EndingVillagersPictureA,
        (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(60);
  Event_PlaySound(73);
  FieldEvent_Wait(30);
  Call3((void (*)())Func_02000244, 64, 2, 30);
#if LANGUAGE == LANGUAGE_GERMAN
  Call3((void (*)())Func_02000244, 64, 2, 30);
#endif
  Call3((void (*)())Func_02000244, 65, 2, 30);
  Call3((void (*)())Func_02000244, 67, 2, 30);
  Call3((void (*)())Func_02000244, 62, 2, 30);
  Call3((void (*)())Func_02000244, 70, 2, 30);
  Call3((void (*)())Func_02000244, 68, 2, 30);
  Call3((void (*)())Func_02000244, 56, 2, 30);
  Call3((void (*)())Func_02000244, 1, 2, 40);
  Func_02000244(56, 2, 40);
  Func_02000854(200);
  System_Wait(1);
  Func_0200085c();
  FieldEvent_Wait(30);
  Call3((void (*)())Func_02000244, 62, 2, 20);
  Call3((void (*)())Func_02000244, 0, 2, 40);
  Call3((void (*)())Func_02000244, 66, 2, 20);
  Call3((void (*)())Func_02000244, 62, 2, 30);
  Call3((void (*)())Func_02000244, 66, 2, 40);
  Call3((void (*)())Func_0200084c, 0, (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(60);
  scroll[3].y = zero;
  scroll[2].y = zero;
  Func_02000864(0);
  Func_0200084c((s32)&ResourceId_EndingEmbracePicture, (s32)Ram_MapCellBuffer,
                0);
  FieldEvent_Wait(60);
  goto L_02008692;
L_0200868c:
  System_Wait(1);
L_02008692:
  /* FAKEMATCH(temporary): p reused for the input and main work addresses; as
   * gInput and gMainRoutineWork read directly, 8 of 426 instructions differ in
   * tla-en. */
  p = &gInput;
  if (((struct InputState *)p)->pressed == 0)
    goto L_0200868c;
  Call3((void (*)())Func_0200084c, (s32)&ResourceId_EndingReunionPicture,
        (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(120);
  Func_02000854(120);
  System_Wait(1);
  Func_0200085c();
  FieldEvent_Wait(180);
  Call3((void (*)())Func_0200084c, (s32)&ResourceId_EndingSunsetPlainPicture,
        (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(300);
  Func_0200084c((s32)&ResourceId_EndingTheEndPicture, (s32)Ram_MapCellBuffer,
                0);
  FieldEvent_Wait(180);
  Event_PlaySound(78);
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
  Func_0200084c(0, (s32)Ram_MapCellBuffer, 0);
  FieldEvent_Wait(60);
  value = 0;
  /* FAKEMATCH(dead): the store in a once-only block; as a plain store, 2 of 426
   * instructions differ in tla-en. */
  do {
    REG_BLDCNT = value;
  } while (0);
  value = 0x1040;
  REG_DISPCNT = value;
  Window_LoadGraphics__epilogue();
  DebugText_Clear__epilogue();
  gRenderOamEnabled = 1;
  Event_ClearFlag(0x17c);
  Call2((void (*)())Func_02000824, (s32)&SceneId_Ov64a, 2);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

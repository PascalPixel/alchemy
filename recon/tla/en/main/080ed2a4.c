// Draft, differ 645 of 548: runs the world map screen (mode 0 to look, nonzero
// to choose a destination) until A or B. In event mode 3 with mode 0 the
// field stays loaded: the map work's sixteen layer flags are set aside and
// the screen is drawn over it; otherwise the field is snapshotted and
// closed, the task table reset and BG1's map numbered for the picture. Each
// frame runs Func_080ed0a0 and Func_080ec4d4; Start glides the cursor to the
// leader's place until released, R and L step to the next and previous shown
// place (Func_080ec2b8), Select toggles the whole-map view, and the first
// time flag 284 or 286 is set message 0xe2a is shown. Choosing with A stores
// the place in the party state and calls Func_080ca368; B stores -1. The
// field is then restored (mode 0 in event mode 3) or flag 265 set and the
// event work's 0x158 set to 999.
// Same calls, offsets and control flow; left: the native spills mode and the
// zoom flag (sp 28 and 0, read back with ldrh for the halfword stores) and
// keeps the world map work in r11; its frame is 56 bytes against 48.
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "party_state.h"
#include "bg_scroll.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "scene_ids.h"

struct WorldMapWork {
  u16 vramBlock;
  u16 windowSlot;
  s32 x;
  s32 z;
  u8 unk0c[6];
  s16 shown;
  u8 unk14[4];
  s32 speed;
  void *window;
  u8 unk20[0x622];
  u16 mode;
};

struct WorldMapPlace {
  u8 flag;
  u8 value;
  s16 unk02;
  s16 x;
  s16 z;
  u8 unk08[8];
  s16 scene;
  s16 entrance;
};

struct MapLayerFlags {
  u8 unk00[10];
  u16 flag;
};

extern struct WorldMapWork gWorldMapWork;
extern s16 Data_0202a640;
extern s32 Data_0202a644;
extern s32 Data_0202a648;
extern s32 Data_0202a64c;
extern s32 Data_0202a650;
extern s32 Data_0202a630;
extern s32 Data_0202a634;
extern u16 Data_02000500;

void ObjectTable_Snapshot(void);
void EventRuntime_StartScreen(void);
void EventRuntime_FinishScreen(void);
void EventRuntime_WaitDelay(void);
void Scheduler_ResetTaskTable(void);
void UiWork_InitializeWithResourceCountersFar(void);
void WaitFrames(s32 frames);
void Func_080ebf94(void);
void Func_080ece20(void);
void Func_080ed0a0(void);
void Func_080ec4d4(void);
void Func_080ec1d0(s32 *screenX, s32 *screenY);
void Func_080ec2b8(s32 *screenX, s32 *screenY, s32 step);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 mode);
void RenderOutput_PrepareForRedrawFar(void *window);
struct WorldMapPlace *FlagEntries_GetTable(void);
void Func_080ca368(s32 scene, s32 entrance);
void BattleEffect_CleanupResourcesAndWindow(void);
void Func_08020270(void);

s32 Func_080ed2a4(s32 mode) {
  u8 *mapWork = (u8 *)Ram_HeapSlots->mapWork;
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *objects = (u8 *)Ram_HeapSlots->objectSystem;
  struct MapLayerFlags *layer;
  struct WorldMapPlace *place;
  u8 savedFlags[16];
  u32 *map;
  s32 zoomed = 0;
  s32 messageShown = 0;
  s32 gliding = 0;
  s32 savedMode;
  s32 blend;
  s32 screenX;
  s32 screenY;
  u32 tiles;
  u8 *saved;
  s32 row;
  s32 column;

  gWorldMapWork.mode = mode;
  if (mode != 0 || ((s8 *)event)[394] != 3) {
    ObjectTable_Snapshot();
    if (((s8 *)event)[394] != 3)
      goto full;
  }
  savedMode = *(s32 *)(event + 436);
  *(s32 *)(event + 436) = 6;
  EventRuntime_FinishScreen();
  EventRuntime_WaitDelay();
  layer = (struct MapLayerFlags *)(mapWork + 24);
  for (saved = savedFlags; saved <= &savedFlags[15]; saved++) {
    *saved = layer->flag;
    layer->flag = 1;
    layer = (struct MapLayerFlags *)((u8 *)layer + 12);
  }
  *(u16 *)(objects + 4) = 1;
  WaitFrames(1);
  mapWork[0x972] = 1;
  blend = (s16)REG_BLDALPHA;
  goto setup;

full:
  (*(u8 **)(event + 460))[91] = 1;
  savedMode = *(s32 *)(event + 436);
  *(s32 *)(event + 436) = 6;
  *(s32 *)(event + 428) = 0;
  EventRuntime_FinishScreen();
  EventRuntime_WaitDelay();
  Scheduler_ResetTaskTable();
  UiWork_InitializeWithResourceCountersFar();
  *(u16 *)(objects + 4) = 1;
  WaitFrames(1);
  blend = (s16)REG_BLDALPHA;
  map = (u32 *)0x06003000;
  tiles = 0x01810180;
  for (row = 0; row < 20; row++) {
    for (column = 0; column < 16; column++) {
      *map++ = tiles;
      tiles += 0x20002;
    }
  }
  tiles = 0x01010100;
  for (row = 0; row < 24; row++) {
    for (column = 0; column < 16; column++) {
      *map++ = tiles;
      tiles += 0x20002;
    }
  }
  tiles = 0x01010100;
  for (row = 0; row < 4; row++) {
    for (column = 0; column < 16; column++) {
      *map++ = tiles;
      tiles += 0x20002;
    }
  }
  *(s32 *)mapWork = 0;
  gBgScroll[1].x = 0;
  gBgScroll[1].y = 0;
  REG_BG1CNT = 0x682;
  REG_DISPCNT = 0x1340;
setup:
  Func_080ebf94();
redraw:
  Data_0202a640 = zoomed;
  Func_080ece20();
  WaitFrames(1);
  REG_BG1CNT = 0x8682;
  REG_DISPCNT = 0x1340;
  if ((GameFlag_Test(284) || GameFlag_Test(286)) && !messageShown) {
    UiText_ShowPositionedMessageAndWaitFar(0xe2a, 1);
    messageShown = 1;
  }
  for (;;) {
    Func_080ed0a0();
    Func_080ec4d4();
    WaitFrames(1);
    if ((gInput.repeat & 8) && !gliding) {
      Func_080ec1d0(&screenX, &screenY);
      Data_0202a644 = screenX << 16;
      Data_0202a648 = screenY << 16;
      gliding = 1;
    }
    if (!(gInput.held & 8) && gliding == 1) {
      Data_0202a644 = Data_0202a64c;
      gliding = 0;
      Data_0202a648 = Data_0202a650;
    }
    if (gInput.repeat & 0x100) {
      Func_080ec2b8(&screenX, &screenY, 1);
      Data_0202a644 = screenX << 16;
      Data_0202a648 = screenY << 16;
      Data_0202a64c = Data_0202a644;
      Data_0202a650 = Data_0202a648;
    }
    if (gInput.repeat & 0x200) {
      Func_080ec2b8(&screenX, &screenY, -1);
      Data_0202a644 = screenX << 16;
      Data_0202a648 = screenY << 16;
      Data_0202a64c = Data_0202a644;
      Data_0202a650 = Data_0202a648;
    }
    if (gInput.repeat & 4) {
      gWorldMapWork.shown = -1;
      RenderOutput_PrepareForRedrawFar(gWorldMapWork.window);
      REG_DISPCNT = 64;
      zoomed ^= 1;
      if (Data_0202a644 != 0) {
        gWorldMapWork.x = Data_0202a644;
        gWorldMapWork.z = Data_0202a648;
        Data_0202a630 = Data_0202a644 - 0x780000;
        Data_0202a634 = gWorldMapWork.z - 0x500000;
        if (Data_0202a630 < 0)
          Data_0202a630 = 0;
        if (Data_0202a630 > 0x1100000)
          Data_0202a630 = 0x1100000;
        if (Data_0202a634 < 0)
          Data_0202a634 = 0;
        if (Data_0202a634 > 0xe00000)
          Data_0202a634 = 0xe00000;
      }
      goto redraw;
    }
    if (gInput.repeat & 1) {
      if (mode == 0)
        break;
      if (gWorldMapWork.shown == -1)
        continue;
      place = &FlagEntries_GetTable()[gWorldMapWork.shown];
      *(s16 *)gPartyState.unk2be = (s32)&SceneId_Ov64a;
      Data_02000500 = place->flag;
      Func_080ca368(place->scene, place->entrance);
      break;
    }
    if (!(gInput.repeat & 2))
      continue;
    if (mode == 0)
      break;
    *(s16 *)gPartyState.unk2be = -1;
    Data_02000500 = -1;
    break;
  }
  BattleEffect_CleanupResourcesAndWindow();
  REG_DISPCNT = 64;
  if (mode == 0 && ((s8 *)event)[394] == 3) {
    Func_08020270();
    IO_WRITE_QUEUE_PUSH(0x04000052, (u16)blend, 0x20000);
    layer = (struct MapLayerFlags *)(mapWork + 24);
    for (saved = savedFlags; saved <= &savedFlags[15]; saved++) {
      layer->flag = *saved;
      layer = (struct MapLayerFlags *)((u8 *)layer + 12);
    }
    *(u16 *)(objects + 4) = 0;
    gBgScroll[1].x = 0;
    gBgScroll[1].y = 0;
    mapWork[0x972] = 0;
    EventRuntime_StartScreen();
    EventRuntime_WaitDelay();
    *(s32 *)(event + 436) = savedMode;
    (*(u8 **)(event + 460))[91] = 0;
  } else {
    *(s16 *)(event + 344) = 999;
    GameFlag_SetBit(265);
  }
  return 0;
}

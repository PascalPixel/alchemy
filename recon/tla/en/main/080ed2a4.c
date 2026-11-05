// Near miss, differ 47 of 548 (tla-en, 2026-11-05), with untagged empty asms, one-pass
// blocks and locals found by search; each needs its S2 tag on adoption, measured then.
// Draft, differ 127 of 548, skeleton gap 23 (tla-en, 2026-11-01): same calls,
// fields, control flow and block order; what is left is below.
// Runs the world map screen (mode 0 to look, nonzero to choose a destination)
// until A or B. In event mode 3 the field stays loaded: the map work's sixteen
// animation flags are set aside and the screen is drawn over it (a nonzero
// mode still snapshots the objects first); otherwise the objects are
// snapshotted, the field closed, the task table reset and BG1's map numbered
// for the picture. Each frame runs Func_080ed0a0 and Func_080ec4d4; Start
// glides the cursor to the leader's place until released, R and L step to the
// next and previous shown place (Func_080ec2b8), Select toggles the whole-map
// view, and the first time flag 284 or 286 is set MsgNotOnMap is shown.
// Choosing with A stores the place in gMainRoutineWork and calls
// Func_080ca368; B stores -1. The field is then restored (mode 0 in event
// mode 3) or flag 265 set and the event work's countdown set to 999.
// Shape, as ☀️'s WorldPicture_Show: register writes through Io_Write16, the
// flags saved by an index loop, the mode-3 setup before the full one, the
// closing test inverted, the redraw an outer loop.
// What the frame loop's form rests on, measured: the native reloads a value
// straight after storing it (the cursor, the scroll pair, the work's z) because
// CSE cannot tell its pointers apart, and only a pointer set twice has no known
// base; hence the NULL defaults before the outer loop. keys and input are the
// native's r10 and r5: input is set again in the Start and release bodies. The
// clamp keeps zero and the limits in locals set before the scroll stores. The
// entrance is read before Data_02000500 is stored, so it is a local. Three
// sets of input, the two of wayZ and that local are plain C but unusually
// shaped: an adoption should decide whether any of them is a device (S2).
// Left: event->screenEffect = 0 reloads zoomed's slot (CSE knows it holds 0)
// where the native builds the 0; tried the first test as a goto, as !(a && b)
// and as an empty then-arm: unchanged. place->scene is loaded straight into r0
// where the native loads r3 and copies, one instruction short; an s16 or s32
// scene local changes nothing. The native's address of unk2be is a reload pair
// (r6 and r1, then r2 and r4), the draft's routine a pseudo, which shifts the
// reload registers of the closing tests; a local per arm folds the symbol and
// offset into one literal and loses 150 of differ. The blend store schedules
// two instructions late in the full setup. Declaration order is at a local
// optimum (every single move tried).
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "main_routine_work.h"
#include "bg_scroll.h"
#include "io_reg.h"
#include "io_write.h"
#include "io_write_queue.h"
#include "scene_ids.h"
#include "event_work.h"
#include "actor.h"
#include "map.h"
#include "field/sprite.h"
#include "flag_call.h"

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

struct WorldMapChoice {
  u8 unk000[0x2be];
  u16 unk2be;
};

extern struct WorldMapWork gWorldMapWork;
extern s16 Data_0202a640;
extern s32 Data_0202a644;
extern s32 Data_0202a648;
extern s32 Data_0202a64c;
extern s32 Data_0202a650;
extern s32 Data_0202a630;
extern s32 Data_0202a634;
extern s16 Data_02000500;
extern char MsgNotOnMap;

void ObjectTable_Snapshot(void);
void WindowSystem_InitFieldFar(void);
void Task_ResetAll(void);
void wait(s32 frames);
void Func_080ebf94(void);
void Func_080ece20(void);
void Func_080ed0a0(void);
void Func_080ec4d4(void);
void Func_080ec1d0(s32 *screenX, s32 *screenY);
void Func_080ec2b8(s32 *screenX, s32 *screenY, s32 step);
void FieldMessage_ShowFar(s32 message, s32 mode);
void WindowSprite_PrepareForRedrawFar(void *window);
struct WorldMapPlace *FlagEntries_GetTable(void);
void Func_080ca368(s32 scene, s32 entrance);
void BattleEffect_CleanupResourcesAndWindow(void);
void Func_08020270(void);

s32 Func_080ed2a4(s32 mode) {
  s32 savedDelay;
  u8 savedFlags[16];
  s32 screenX;
  s32 screenY;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct ObjectSystemWork *objects = Ram_HeapSlots->objectSystem;
  s16 blend;
  s32 messageShown = 0;
  s32 zoomed = 0;
  s32 gliding = 0;
  struct WorldMapWork *world = &gWorldMapWork;
  struct MapAnimation *layer;
  struct WorldMapPlace *place;
  s32 entrance;
  u32 *tiles;
  u32 tile;
  s32 row;
  s32 column;
  s32 i;
  struct InputState *input;
  struct InputState *keys;
  s32 *wayX;
  s32 *wayZ;
  s32 *cursorX;
  s32 *cursorZ;
  s32 *scrollX;
  s32 *scrollZ;
  s32 zero;
  s32 limitX;
  s32 limitZ;
  s32 value;
  struct WorldMapChoice *routine;

  world->mode = mode;
  if (mode != 0 || event->controlMode != 3)
    ObjectTable_Snapshot();
  if (event->controlMode == 3) {
    savedDelay = event->delay;
    event->delay = 6;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    layer = map->anim;
    for (i = 0; i < 16; i++) {
      savedFlags[i] = layer->paused;
      layer->paused = 1;
      layer++;
    }
    objects->suspended = 1;
    wait(1);
    ((struct MapViewWork *)map)->picturePending = 1;
  do {
    blend = REG_BLDALPHA;
  } while (0);
  } else {
  do {
    event->viewCenter->movementState = 1;
  } while (0);
    savedDelay = event->delay;
    event->delay = 6;
    event->screenEffect = 0;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Task_ResetAll();
    WindowSystem_InitFieldFar();
    objects->suspended = 1;
    wait(1);
    blend = REG_BLDALPHA;
    tiles = (u32 *)0x06003000;
    tile = 0x01810180;
    for (row = 0; row < 20; row++)
      for (column = 0; column < 16; column++) {
        *tiles++ = tile;
        tile += 0x20002;
      }
    tile = 0x01010100;
    for (row = 0; row < 24; row++)
      for (column = 0; column < 16; column++) {
        *tiles++ = tile;
        tile += 0x20002;
      }
    tile = 0x01010100;
    for (row = 0; row < 4; row++)
      for (column = 0; column < 16; column++) {
        *tiles++ = tile;
        tile += 0x20002;
      }
    map->origin = 0;
    gBgScroll[1].x = 0;
    gBgScroll[1].y = 0;
    Io_Write16(0x682, &REG_BG1CNT);
    Io_Write16(0x1340, &REG_DISPCNT);
  }
  // Default: no pointer yet.
  wayX = NULL;
  cursorX = NULL;
  cursorZ = NULL;
  scrollX = NULL;
  scrollZ = NULL;
  Func_080ebf94();
  for (;;) {
    Data_0202a640 = zoomed;
    Func_080ece20();
    wait(1);
    Io_Write16(0x8682, &REG_BG1CNT);
    Io_Write16(0x1340, &REG_DISPCNT);
    if ((GameFlag_Test(284) || GameFlag_Test(286)) && !messageShown) {
      FieldMessage_ShowFar((s32)&MsgNotOnMap, 1);
      messageShown = 1;
    }
    keys = &gInput;
    wayX = &Data_0202a64c;
    cursorX = &Data_0202a644;
    cursorZ = &Data_0202a648;
    for (;;) {
      Func_080ed0a0();
      Func_080ec4d4();
      input = keys;
      wait(1);
      if ((input->repeat & 8) && gliding == 0) {
        Func_080ec1d0(&screenX, &screenY);
        *cursorX = screenX << 16;
        input = &gInput;
        *cursorZ = screenY << 16;
        gliding = 1;
      }
      if (!(keys->held & 8) && gliding == 1) {
        *cursorX = *wayX;
        input = &gInput;
        *cursorZ = Data_0202a650;
        gliding = 0;
      }
      if (keys->repeat & 0x100) {
        Func_080ec2b8(&screenX, &screenY, 1);
        *cursorX = screenX << 16;
        wayZ = &Data_0202a650;
        *cursorZ = screenY << 16;
        *wayX = *cursorX;
        *wayZ = *cursorZ;
      }
      if (input->repeat & 0x200) {
        Func_080ec2b8(&screenX, &screenY, -1);
        *cursorX = screenX << 16;
        wayZ = &Data_0202a650;
        *cursorZ = screenY << 16;
        *wayX = *cursorX;
        *wayZ = *cursorZ;
      }
      if (input->repeat & 4) {
        world->shown = -1;
        WindowSprite_PrepareForRedrawFar(world->window);
        Io_Write16(64, &REG_DISPCNT);
        zoomed ^= 1;
        if (*cursorX != 0) {
          world->x = *cursorX;
          world->z = *cursorZ;
          zero = 0;
          limitX = 0x1100000;
          limitZ = 0xe00000;
          scrollX = &Data_0202a630;
          *scrollX = world->x - 0x780000;
          scrollZ = &Data_0202a634;
          *scrollZ = world->z - 0x500000;
          value = *scrollX;
          if (value < 0) {
            *scrollX = zero;
            value = *scrollX;
          }
          if (value > limitX)
            *scrollX = limitX;
          value = *scrollZ;
          if (value < 0) {
            *scrollZ = zero;
            value = *scrollZ;
          }
          if (value > limitZ)
            *scrollZ = limitZ;
        }
        break;
      }
      if (input->repeat & 1) {
        if (mode == 0)
          goto done;
        if (world->shown == -1)
          continue;
        place = FlagEntries_GetTable();
        routine = (struct WorldMapChoice *)&gMainRoutineWork;
        place += world->shown;
        routine->unk2be = (s32)&SceneId_Ov64a;
        entrance = place->entrance;
        Data_02000500 = place->flag;
  __asm__("" : "=r"(Data_02000500) : "0"(Data_02000500));
        Func_080ca368(place->scene, entrance);
        goto done;
      }
      if (input->repeat & 2) {
        if (mode != 0) {
          routine = (struct WorldMapChoice *)&gMainRoutineWork;
          routine->unk2be = 0xffff;
  {
    __typeof__(-1) hoisted = -1;
    Data_02000500 =  hoisted ;
  }
        }
        goto done;
      }
    }
  }
done:
  BattleEffect_CleanupResourcesAndWindow();
  Io_Write16(64, &REG_DISPCNT);
  if (mode != 0 || event->controlMode != 3) {
    event->countdown = 999;
    GameFlag_SetBit(265);
  } else {
    Func_08020270();
    IO_WRITE_QUEUE_PUSH(0x04000052, (u16)blend, 0x20000);
    layer = map->anim;
    for (i = 0; i < 16; i++) {
      layer->paused = savedFlags[i];
      layer++;
    }
    objects->suspended = 0;
    gBgScroll[1].x = 0;
    gBgScroll[1].y = 0;
    ((struct MapViewWork *)map)->picturePending = 0;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    event->delay = savedDelay;
    event->viewCenter->movementState = 0;
  }
  return 0;
}

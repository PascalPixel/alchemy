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
#include "field/world_map.h"
#include "text/msg_ids.h"

extern s16 Data_0202a640;
extern s32 Data_0202a644;
extern s32 Data_0202a648;
extern s32 Data_0202a64c;
extern s32 Data_0202a650;
extern s32 Data_0202a630;
extern s32 Data_0202a634;

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
  struct FlagEntry *place;
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
  struct WorldMapChoice *choice = &Data_02000500;
  struct MainRoutineWork *routine = &gMainRoutineWork;

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
      // A once-only block: as a plain statement, 16 of 548 instructions differ.
      blend = REG_BLDALPHA;
    } while (0);
  } else {
    do {
      // A once-only block: as a plain statement, 281 of 548 instructions
      // differ.
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
      FieldMessage_ShowFar((s32)MsgNotOnMap, 1);
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
        /* FAKEMATCH(dead): input reset to the &gInput it already holds; as
         * without the reset, 57 of 548 instructions differ in tla-en. */
        input = &gInput;
        *cursorZ = screenY << 16;
        gliding = 1;
      }
      if (!(keys->held & 8) && gliding == 1) {
        *cursorX = *wayX;
        /* FAKEMATCH(dead): input reset to the &gInput it already holds; as
         * without the reset, 61 of 548 instructions differ in tla-en. */
        input = &gInput;
        *cursorZ = Data_0202a650;
        gliding = 0;
      }
      if (keys->repeat & 0x100) {
        Func_080ec2b8(&screenX, &screenY, 1);
        *cursorX = screenX << 16;
        // wayZ only ever holds &Data_0202a650, set where it is used.
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
        place += world->shown;
        routine->unk2be = (s32)&SceneId_WorldMap;
        choice->flag = place->flag;
        Func_080ca368(place->scene, place->entrance);
        goto done;
      }
      if (input->repeat & 2) {
        if (mode != 0) {
          routine->unk2be = 0xffff;
          {
            // The -1 in a local of its own; as choice->flag = -1, 40 of 548
            // instructions differ in tla-en.
            s32 none = -1;

            choice->flag = none;
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

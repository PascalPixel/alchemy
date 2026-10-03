/* Field: show the world map. The map layers are hidden and the map picture
   is decoded over BG1 until A or B is pressed, then the field comes back. */
#include "TYPES.H"
#include "DMA.H"
#include "CALLBACK_SCHEDULER.H"
#include "IO_WRITE_QUEUE.H"
#include "IO_REG.H"
#include "RESOURCE_IDS.H"
#include "OBJECT_LOOKUP.H"
#include "GLOBAL_CELLS.H"
#include "MAP.H"
#include "FIELD_EVENT.H"
#include "EVENT_RUNTIME.H"
#include "OBJECT_RUNTIME.H"

void Map_UpdateWorldMapMarkers(void);
extern u8 gMapCellBuffer[];

struct MenuControl {
    u8 padding00[4];
    u16 suspended;
};

extern u8 gWorkSlot[];
extern u8 MsgNotOnMap[];
extern u32 gKeysRepeat;
void *Runtime_AllocateBlock(s32 slot, s32 size);
void Event_ClearStatus1c6(void);
void Event_SetStatus1c6(void);
void Event_WaitValue1c8Frames(void);
void WaitFrames(s32 frames);
u8 *Resource_GetTableEntry(s32 index);
s32 Resource_DecodeType01(const void *source, void *destination);
void BattleFx_SetupResourcesAndWindow(void);
void BattleFx_CleanupResourcesAndWindow(void);
s32 GameFlag_TestFar(s32 flag);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 mode);
void Map_LoadAreaGraphicsFar(void);

extern u32 gKeysHeld;
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
void Battle_InitializeRenderObject(void);
void BattleFx_ScheduleRatioTransition(s32, s32);

void Map_ShowWorldMap(void)
{
    /* FAKEMATCH: two one-pass loops act as scheduling barriers: one around
     * the display-control value 64 so it is built before the register address,
     * one around reloading the layer table so its spilled pointer loads first.
     * The queued blend restore is QueueIoWriteDelay2 (SYSTEM/IO_WRITE_QUEUE.C)
     * written out inline with its one-pass loop around the IME read and its
     * count stored through an explicit u16 pointer. */
    struct MapState *map = *(struct MapState **)(gWorkSlot + 8 * 4);
    struct EventWork *field = Runtime_AllocateBlock(27, 0xccc);
    struct MenuControl *menu = *(struct MenuControl **)(gWorkSlot + 6 * 4);
    s32 resource = (s32)&ResourceId_WorldMapPicture;
    struct MapAnimation *layer;
    u8 saved_flags[16];
    s32 i;
    u8 *graphics;
    s32 saved_status;
    s16 blend;

    if (((struct EventRuntime *)field)->mode_19e != 3)
        return;
    ((struct ObjectRuntime *)field->view_center)->movement_state = 1;
    saved_status = field->transition_frames;
    field->transition_frames = 6;
    Event_ClearStatus1c6();
    Event_WaitValue1c8Frames();
    layer = map->anim;
    for (i = 0; i < 16; i++) {
        saved_flags[i] = layer->paused;
        layer->paused = 1;
        layer++;
    }
    menu->suspended = 1;
    WaitFrames(1);
    blend = *(volatile u16 *)0x04000052;
    graphics = Resource_GetTableEntry(resource);
    Dma_Set(graphics, (void *)0x05000000, 0x84000070, (volatile u32 *)0x040000d4);
    *(u16 *)0x05000000 = 0;
    Resource_DecodeType01(graphics + 448, (void *)gMapCellBuffer);
    Dma_Set((void *)gMapCellBuffer, (void *)0x06006a00, 0x84002580, (volatile u32 *)0x040000d4);
    {
        s32 v = 0x682;
        *(volatile u16 *)0x0400000a = v;
        v = 0x1340;
        *(volatile u16 *)0x04000000 = v;
    }
    BattleFx_SetupResourcesAndWindow();
    Scheduler_AddOrUpdateCallback((s32)Map_UpdateWorldMapMarkers, 0xc80);
    if (GameFlag_TestFar(284))
        UiText_ShowPositionedMessageAndWaitFar((s32)MsgNotOnMap, 1);
    do {
        WaitFrames(1);
    } while ((gKeysRepeat & 3) == 0);
    Scheduler_RemoveCallback((u32)Map_UpdateWorldMapMarkers);
    BattleFx_CleanupResourcesAndWindow();
    {
        s32 v;
        /* FAKEMATCH: removing this one-pass block changes instruction scheduling. */
        do {
            v = 64;
        } while (0);
        *(volatile u16 *)0x04000000 = v;
    }
    Map_LoadAreaGraphicsFar();
    {
        volatile u16 *ime;
        struct IoWriteQueue *q;
        u32 saved;
        s32 count;

        q = &gIoWriteQueue;
        /* FAKEMATCH: removing this one-pass block changes instruction scheduling. */
        do {
            ime = &REG_IME;
            saved = *ime;
        } while (0);
        *ime = (u16)ime;
        count = q->count;
        if (count <= 31) {
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);
            *(u16 *)&q->count = count + 1;
            *destination++ = (u16)blend;
            *destination++ = 0x04000052;
            *destination = 0x20000;
        }
        *ime = saved;
    }
    /* FAKEMATCH: removing this one-pass block changes instruction scheduling. */
    do {
        layer = map->anim;
    } while (0);
    for (i = 0; i < 16; i++) {
        layer->paused = saved_flags[i];
        layer++;
    }
    menu->suspended = 0;
    Event_SetStatus1c6();
    Event_WaitValue1c8Frames();
    field->transition_frames = saved_status;
    ((struct ObjectRuntime *)field->view_center)->movement_state = 0;
}

void BattleFx_UpdateObjectVisibilityBounds(void)
{
    /* FAKEMATCH: retain the discarded object lookup while its native source
     * dataflow is unresolved; its result is unused before the runtime object
     * address is loaded. */
    struct ObjectRuntime *object;
    s32 x;
    s32 y;
    s32 left;
    s32 right;
    s32 top;
    s32 bottom;
    u32 id;

    Object_GetById(gGameState.selected_actor);
    object = (struct ObjectRuntime *)gEventWork->view_center;
    x = object->x;
    left = x + 0xFEC00000;
    right = x + 0x01400000;
    y = object->z;
    top = y + 0xFDA80000;
    bottom = y + 0x01900000;

    id = 8;
    do {
        object = ObjectTable_Get(id);

        if (object != 0) {
            s32 ox = object->x;
            s32 oy = object->z;

            if (ox < left || ox > right ||
                oy < top || oy > bottom) {
                object->animation_kind = 0;
            } else {
                object->animation_kind = 1;
            }
        }
        id++;
    } while (id <= 65);
}

void BattleFx_RunVisibilityTransition(void)
{
  if (((struct EventRuntime *)Runtime_AllocateBlock(27, 0xccc))->mode_19e == 3)
  {
    Scheduler_EnableUnmaskedOverlayCallbacks();
    BattleFx_UpdateObjectVisibilityBounds();
    Battle_InitializeRenderObject();
    BattleFx_ScheduleRatioTransition(0x9D89, 6);
    if ((*((volatile u32 *) ((u32)&gKeysHeld))) & 0x200)
    {
      do
      {
        WaitFrames(1);
      }
      while ((*((volatile u32 *) ((u32)&gKeysHeld))) & 0x200);
    }
    BattleFx_ScheduleRatioTransition(0x10000, 6);
    Scheduler_DisableOverlayCallbacks();
  }
}

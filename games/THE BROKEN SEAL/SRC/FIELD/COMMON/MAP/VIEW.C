#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "MEM.H"
#include "RESOURCE.H"
#include "BTLSYS.H"
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
#include "ANIMSPR.H"
#include "HEAP_STATE.H"
#include "text/MSG_IDS.H"

extern u8 gMapCellBuffer[];

void Event_FinishScreenTransition(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 mode);

extern u32 gKeysHeld;
void BattleFx_ScheduleRatioTransition(s32, s32);

void Map_ShowWorldMap(s32 picture)
{
    /* FAKEMATCH: needed to match: one-pass loops order display-control setup, the spilled layer load and the IME read; the count uses a halfword store. */
    /* FAKEMATCH: needed to match: the shared slot 8/6 word-cell reads preserve the saved-layer spill and avoid four extra bytes. */
    struct MapState *map = *(struct MapState **)((u8 *)gWorkSlot + 8 * sizeof(void *));
    struct EventWork *field = Runtime_AllocateBlock(27, 0xccc);
    struct ObjectSystemWork *menu = *(struct ObjectSystemWork **)((u8 *)gWorkSlot + 6 * sizeof(void *));
    s32 resource = (s32)&ResourceId_WorldMapPicture;
    struct MapAnimation *layer;
    u8 saved_flags[16];
    s32 i;
    u8 *graphics;
    s32 saved_status;
    s16 blend;

    if (field->mode != 3)
        return;
    field->view_center->paused = 1;
    saved_status = field->transition_frames;
    field->transition_frames = 6;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    layer = map->anim;
    for (i = 0; i < 16; i++) {
        saved_flags[i] = layer->paused;
        layer->paused = 1;
        layer++;
    }
    menu->suspended = 1;
    WaitFrames(1);
    blend = REG_BLDALPHA;
    graphics = Resource_GetTableEntry(resource);
    Dma_Set(graphics, (void *)BG_PLTT, DMA_ENABLE32 | DMA_32BIT | (448 / 4), REG_DMA3);
    BG_PLTT[0] = 0;
    Resource_DecodeType01(graphics + 448, gMapCellBuffer);
    Dma_Set(gMapCellBuffer, BG_CHAR_BLOCK(1) + 0x2a00, DMA_ENABLE32 | DMA_32BIT | (240 * 160 / 4), REG_DMA3);
    {
        /* FAKEMATCH: needed to match: a word temporary keeps the halfword values as word constants; direct stores load them by halfword from the pool. */
        s32 v = BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6) | 2;
        REG_BG1CNT = v;
        v = DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
        REG_DISPCNT = v;
    }
    BattleFx_SetupResourcesAndWindow();
    Scheduler_AddOrUpdateCallback(Map_UpdateWorldMapMarkers, 0xc80);
    if (GameFlag_TestFar(284))
        UiText_ShowPositionedMessageAndWaitFar((s32)MsgNotOnMap, 1);
    do {
        WaitFrames(1);
    } while ((gKeysRepeat & (KEY_A | KEY_B)) == 0);
    Scheduler_RemoveCallback(Map_UpdateWorldMapMarkers);
    BattleFx_CleanupResourcesAndWindow();
    {
        s32 v;
        /* FAKEMATCH: needed to match: removing this one-pass block changes instruction scheduling. */
        do {
            v = DISPCNT_OBJ_1D_MAP;
        } while (0);
        REG_DISPCNT = v;
    }
    Map_LoadAreaGraphicsFar();
    {
        volatile u16 *ime;
        struct IoWriteQueue *q;
        u32 saved;
        s32 count;

        q = &gIoWriteQueue;
        /* FAKEMATCH: needed to match: removing this one-pass block changes instruction scheduling. */
        do {
            ime = &REG_IME;
            saved = *ime;
        } while (0);
        *ime = (u16)ime;
        count = q->count;
        if (count <= 31) {
            u32 *destination = q->entries[count];
            *(u16 *)&q->count = count + 1;
            *destination++ = (u16)blend;
            *destination++ = (u32)&REG_BLDALPHA;
            *destination = 0x20000;
        }
        *ime = saved;
    }
    /* FAKEMATCH: needed to match: removing this one-pass block changes instruction scheduling. */
    do {
        layer = map->anim;
    } while (0);
    for (i = 0; i < 16; i++) {
        layer->paused = saved_flags[i];
        layer++;
    }
    menu->suspended = 0;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    field->transition_frames = saved_status;
    field->view_center->paused = 0;
}

void BattleFx_UpdateObjectVisibilityBounds(void)
{
    /* FAKEMATCH: needed to match: the discarded object lookup remains before the runtime object address load. */
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
    left = x - (320 << 16);
    right = x + (320 << 16);
    y = object->z;
    top = y - (600 << 16);
    bottom = y + (400 << 16);

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
    struct EventWork *field = Runtime_AllocateBlock(27, 0xccc);

    if (field->mode == 3) {
        Scheduler_EnableUnmaskedOverlayCallbacks();
        BattleFx_UpdateObjectVisibilityBounds();
        Battle_InitializeRenderObject();
        BattleFx_ScheduleRatioTransition(0x9d89, 6);
        if (gKeysHeld & KEY_L) {
            do {
                WaitFrames(1);
            } while (gKeysHeld & KEY_L);
        }
        BattleFx_ScheduleRatioTransition(0x10000, 6);
        Scheduler_DisableOverlayCallbacks();
    }
}

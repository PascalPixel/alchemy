#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
#include "EDITION.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "IO_WRITE_QUEUE.H"
#include "PARTY_STATE.H"
#include "RAM_BUFFER.H"

extern u8 gMapCellBuffer[];

extern void UiWork_FinalizeFar(u32 arg0, u32 arg1);

void BattleFx_CleanupResourcesAndWindow(void)
{
    Resource_ResetEntry(*(u16 *)(gMapCellBuffer + 0));
    Resource_ResetEntry(*(u16 *)(gMapCellBuffer + 2));
    UiWork_FinalizeFar(*(u32 *)(gMapCellBuffer + 0x1c), 2);
}

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#else
/* The European editions build the marker window their own way, which stays
   in their scaffolds for now. */

struct MapMarker {
    struct MapMarker *link;
    u16 y : 8;
    u16 affine : 2;
    u16 blend_mode : 2;
    u16 mosaic : 1;
    u16 full_color : 1;
    u16 shape : 2;
    u16 x : 9;
    u16 affine_index : 5;
    u16 size : 2;
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
    u16 padding0a;
};

union MapFixed {
    s32 value;
    struct {
        u16 fraction;
        s16 whole;
    } part;
};

struct WorldMapWork {
    u16 vram_block;
    u16 padding02;
    union MapFixed x;
    union MapFixed z;
    u8 padding0c[6];
    s16 shown;
    u8 padding14[4];
    s32 speed;
    s32 window;
    struct MapMarker markers[67];
};

struct MapObject {
    u8 padding00[8];
    s32 x;
    u8 padding0c[4];
    union MapFixed z;
};

struct WorldMapVramBlock {
    u16 base;
    u16 offset;
};

extern struct WorldMapVramBlock gVramBlockCache[];
extern u32 gFrameCount;
extern u32 gKeysHeld;
extern const u8 WorldMap_MarkerBlendCycle[];
extern const u16 WorldMap_CursorDirectionAngles[];
extern const s32 WorldMap_PlaceMarkers[];
extern u8 MsgDebugEntryName[];
extern u8 MsgPresentLocation[];

s32 GameFlag_TestFar(s32 flag);
struct MapObject *ObjectTable_Get(s32 id);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *position);
s32 BattleFx_FindConditionResource(s32 id, s32 kind);
void UiWindow_Clear(s32 window);
void UiText_DrawMessageAt(s32 message, s32 window, s32 x, s32 y);
void UiText_MeasureResourceEntriesFar(s32 message, s32 *width, s32 *height);

#define QUEUE_IO_WRITE_DELAY2(address, value) do {                          \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        /* FAKEMATCH: a block of its own keeps the saved IME read first. */ \
        do { \
            /* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling. */ \
            saved = *ime;                                                   \
        } while (0);                                                        \
        *ime = (u16)ime;                                                    \
        count = q->count;                                                   \
        if (count <= 31) {                                                  \
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);           \
            *(u16 *)&q->count = count + 1;                                  \
            *destination++ = (value);                                       \
            *destination++ = (address);                                     \
            *destination = 0x20000;                                         \
        }                                                                   \
        *ime = saved;                                                       \
    } while (0)

/* The world map's frame callback: move the cursor with the keys, place
   the markers of the places the flags have opened, name the nearest one
   in the window, and pulse the markers' blend. */
void Map_UpdateWorldMapMarkers(void)
{
    s32 leader;
    const s32 *place;
    s32 tile_base;
    s32 blend;
    s32 best;
    s32 best_distance;
    s32 best_x;
    s32 best_y;
    s32 message;
    s32 best_message;
    s32 height;
    s32 width;
    s32 position[3];
    struct WorldMapWork *work;
    struct MapObject *object;
    s32 cursor_x;
    s32 cursor_y;
    s32 tile;
    s32 mode;
    s32 flag;
    s32 i;
    struct MapMarker *marker;
    s32 x;
    s32 y;
    s32 angle;
    struct IoWriteQueue *q;
    volatile u16 *ime;

    work = (struct WorldMapWork *)Ram_MapCellBuffer;
    leader = gGameState.selected_actor;
    place = WorldMap_PlaceMarkers;
    tile_base = gVramBlockCache[work->vram_block].offset >> 5;
    marker = work->markers;
    best = -1;
    best_distance = 100;
    blend = WorldMap_MarkerBlendCycle[(gFrameCount >> 1) & 31];
    if (!GameFlag_TestFar(0x11c) && (gKeysHeld & KEYS_SHOULDERS)) {
        object = ObjectTable_Get(gGameState.selected_actor);
        if (object == NULL)
            goto markers;
        cursor_x = ((object->x - 0x10000000) >> 16) * 240 / 4096;
        cursor_y = object->z.part.whole * 160 / 4096;
    } else {
        angle = WorldMap_CursorDirectionAngles[(gKeysHeld >> 4) & 15];
        if (angle != 0xffff) {
            position[0] = work->x.value;
            position[1] = 0;
            position[2] = work->z.value;
            Vector_AddPolarOffset(work->speed, angle, position);
            if (position[0] < 0x100000)
                position[0] = 0x100000;
            if (position[0] > 0xef0000)
                position[0] = 0xef0000;
            if (position[2] < 0)
                position[2] = 0;
            if (position[2] > 0x8d0000)
                position[2] = 0x8d0000;
            work->x.value = position[0];
            work->z.value = position[2];
            if (work->speed < 0x60000)
                work->speed += 0x2000;
        } else {
            work->speed = 0x10000;
        }
        cursor_x = work->x.part.whole;
        cursor_y = work->z.part.whole;
    }
markers:
    for (i = 0; i <= 65; i++) {
        if (i != leader && i <= 10)
            continue;
        if (i == 0) {
            if (GameFlag_TestFar(0x11c))
                continue;
            tile = 0;
            flag = leader;
            mode = 0;
        } else {
            flag = *place++;
            message = *place++;
            if (flag == 0)
                continue;
            tile = flag >> 16;
            mode = 1;
            if (flag == -1)
                break;
        }
        if (!GameFlag_TestFar(flag))
            continue;
        object = ObjectTable_Get(i);
        if (object == NULL)
            continue;
        x = ((object->x - 0x10000000) >> 16) * 240 / 4096;
        y = object->z.part.whole * 160 / 4096;
        marker->blend_mode = mode;
        marker->tile = tile_base + tile;
        marker->x = x - 1;
        marker->y = y - 1;
        width = x - cursor_x;
        height = y - cursor_y;
        if (width * width + height * height < best_distance) {
            best_message = message;
            best = i;
            best_distance = width * width + height * height;
            best_x = x;
            best_y = y;
        }
        if (i != 0 || (gFrameCount & 15) <= 7)
            Runtime_PushSlotEntry(marker++, y);
    }
    if (best != -1 && (gFrameCount & 15) <= 7) {
        marker->blend_mode = 0;
        marker->tile = tile_base + 3;
        marker->x = best_x - 2;
        marker->y = best_y - 2;
        Runtime_PushSlotEntry(marker, best_y);
    }
    marker = &work->markers[66];
    marker->affine_index = 0;
    marker->x = cursor_x - 17;
    marker->y = cursor_y + 1;
    Runtime_PushSlotEntry(marker, 246);
    if (work->shown != best) {
        work->shown = best;
        UiWindow_Clear(work->window);
        if (best != -1) {
            if (best == 0)
                best_message = (s32)MsgPresentLocation;
            else
                best_message = BattleFx_FindConditionResource(best_message, 1) + (s32)MsgDebugEntryName;
            UiText_MeasureResourceEntriesFar(best_message, &width, &height);
            cursor_x = best_x;
            cursor_y = best_y;
            cursor_x--;
            cursor_y -= 11;
            if (cursor_x + width > 240) {
                cursor_x = 232 - width;
                cursor_y = best_y - 20;
            }
            if (cursor_y < 0)
                cursor_y = 0;
            UiText_DrawMessageAt(best_message, work->window, cursor_x, cursor_y);
        }
    }
    q = &gIoWriteQueue;
    ime = &REG_IME;
    QUEUE_IO_WRITE_DELAY2(0x04000050, 0x3f00);
    QUEUE_IO_WRITE_DELAY2(0x04000052, ((16 - blend) << 8) | blend);
}
#endif

/* Draft, not exact (2026-09-25): 1088 of 1088 bytes, 36 differing halfwords.
   Map_UpdateWorldMapMarkers, the world-map frame callback Map_ShowWorldMap
   schedules (0x0809bcf9). Written from the listing. What lined up: a work
   pointer to the constant 0x02010000 (spilled, so every use reloads the
   literal), locals declared in reverse stack-slot order, the place table
   walked as a word pointer, and the two blend writes as QueueIoWriteDelay2
   written out inline, the one-pass IME barrier kept only on the second.
   Remaining: scheduling only. The ROM loads the queue literal before the
   IME literal (pool order) and copies the saved IME value right after the
   read in the first write (a barrier there swaps the queue and IME
   registers); the window placement computes x before y and loads the window
   pointer before the x argument; best_message + 0x99b lands in r0; two
   marker byte accesses in the loop sit one slot apart. */
#include "TYPES.H"
#include "IO_WRITE_QUEUE.H"
#include "PARTY_STATE.H"

struct MapMarker {
    u32 link;
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

#define Data_02010000 (*(struct WorldMapWork *)0x02010000)
extern struct WorldMapVramBlock Data_03001b10[];
extern u32 Data_03001e40;
extern u32 Data_03001ae8;
extern const u8 Data_0809f168[];
extern const u16 Data_0809f188[];
extern const s32 Data_080a0138[];
extern volatile u16 Data_04000208;
#define REG_IME Data_04000208

s32 GameFlag_TestFar(s32 flag);
struct MapObject *ObjectTable_Get(s32 id);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *position);
void Runtime_PushSlotEntry(void *entry, s32 slot);
s32 BattleFx_FindConditionResource(s32 id, s32 kind);
void Func_08015060(s32 window);
void Func_08015078(s32 message, s32 window, s32 x, s32 y);
void Func_080153c0(s32 message, s32 *width, s32 *height);

#define QUEUE_IO_WRITE_DELAY2(address, value) {                             \
        volatile u16 *ime;                                                  \
        struct IoWriteQueue *q;                                             \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        q = &gIoWriteQueue;                                                 \
        ime = &REG_IME;                                                     \
        saved = *ime;                                                       \
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
    }

#define QUEUE_IO_WRITE_DELAY2_BARRIER(address, value) {                             \
        volatile u16 *ime;                                                  \
        struct IoWriteQueue *q;                                             \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        q = &gIoWriteQueue;                                                 \
        do {  \
        ime = &REG_IME;   \
        saved = *ime;    \
        } while (0); \
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
    }

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

    work = &Data_02010000;
    leader = PARTY_STATE.current_owner;
    place = Data_080a0138;
    tile_base = Data_03001b10[work->vram_block].offset >> 5;
    marker = work->markers;
    best = -1;
    best_distance = 100;
    blend = Data_0809f168[(Data_03001e40 >> 1) & 31];
    if (!GameFlag_TestFar(0x11c) && (Data_03001ae8 & 0x300)) {
        object = ObjectTable_Get(PARTY_STATE.current_owner);
        if (object == NULL)
            goto markers;
        cursor_x = ((object->x - 0x10000000) >> 16) * 240 / 4096;
        cursor_y = object->z.part.whole * 160 / 4096;
    } else {
        angle = Data_0809f188[(Data_03001ae8 >> 4) & 15];
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
        if (i != 0 || (Data_03001e40 & 15) <= 7)
            Runtime_PushSlotEntry(marker++, y);
    }
    if (best != -1 && (Data_03001e40 & 15) <= 7) {
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
        Func_08015060(work->window);
        if (best != -1) {
            if (best == 0)
                best_message = 0x984;
            else
                best_message = BattleFx_FindConditionResource(best_message, 1) + 0x99b;
            Func_080153c0(best_message, &width, &height);
            cursor_x = best_x - 1;
            cursor_y = best_y - 11;
            if (cursor_x + width > 240) {
                cursor_x = 232 - width;
                cursor_y = best_y - 20;
            }
            if (cursor_y < 0)
                cursor_y = 0;
            Func_08015078(best_message, work->window, cursor_x, cursor_y);
        }
    }
    QUEUE_IO_WRITE_DELAY2(0x04000050, 0x3f00);
    QUEUE_IO_WRITE_DELAY2_BARRIER(0x04000052, ((16 - blend) << 8) | blend);
}

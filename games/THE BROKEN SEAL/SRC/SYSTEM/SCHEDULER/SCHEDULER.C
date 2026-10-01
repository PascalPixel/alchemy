#include "TYPES.H"
#include "FIXED_MATH.H"
#include "IWRAM_CALL.H"
#include "GLOBAL_CELLS.H"
#include "DMA.H"
#include "RUNTIME_MEM.H"
#include "VRAM_BLOCK.H"
#include "SCENE.H"
#include "CALLBACK_SCHEDULER.H"
#include "STRING.H"
#include "IO_REG.H"
#include "LOW_RUNTIME.H"

struct Effect {
    unsigned x : 16;
    unsigned y : 16;
    unsigned angle : 16;
    unsigned unused : 16;
};

union AffineMatrix {
    s16 coefficients[4];
    u32 rows[2];
};

extern u8 gObjAffineCount;
extern union AffineMatrix gObjAffineMatrices[];

extern u8 Data_03001400[];
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))
extern const u8 Render_BuildOamList[];
typedef void (*LoadedRoutine)(void *argument);

/* Linker-resolved absolute size of the routine copied into the heap. */
extern u8 LoadedRuntime_Size[];

extern u8 ResourceBlockOwners[512];
extern u8 ResourceBlockOwners[];

struct ResourceTableEntry {
    u16 value;
    u16 flags;
};

extern struct ResourceTableEntry ResourceTableEntries[];

typedef s32 (*KeyCallbackFn)(void);

extern u8 Data_03001cb4[];
extern const u16 Math_ArcTanTable[];

extern const u8 System_BasicColorPalette[];

extern u8 gNumberTextBuffer[];

/* ui/text/fmt/format_hex_to_work.c */
extern const u8 RomBytes_0800795c[];

/* ui/text/format_signed_decimal_to_work.c */
extern u8 Text_PowersOfTen[];

/* graphics/fill_word_stream_with_f000.c */
extern u16 *gDebugTextCursor;

s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);

s32 AffineMatrix_BuildForEffect(struct Effect *source)
{
    union AffineMatrix *matrix;
    s16 *coefficient;
    s32 x_scale;
    s32 y_scale;
    s32 angle;
    u8 index;

    index = gObjAffineCount;
    x_scale = (s16)source->x;
    y_scale = (s16)source->y;
    angle = source->angle;
    if (index > 31)
        return 0;

    matrix = &gObjAffineMatrices[index];
    coefficient = matrix->coefficients;
    if ((x_scale == y_scale || -x_scale == y_scale) && angle == 0) {
        s32 (*divide)(s32, s32);
        s32 reciprocal;
        s32 x_reciprocal;

        divide = Iwram_SignedDivide;
        reciprocal = divide(0x10000, y_scale);
        x_reciprocal = reciprocal;
        if (-x_scale == y_scale)
            x_reciprocal = -reciprocal;

        matrix->rows[0] = (u16)x_reciprocal;
        matrix->rows[1] = (u32)reciprocal << 16;
    } else {
        s32 sine;
        s32 cosine;

        sine = Trig_Sin(angle);
        cosine = Trig_Cos(angle);
        *coefficient = cosine / x_scale;
        coefficient++;
        *coefficient = sine / x_scale;
        coefficient++;
        *coefficient = (-sine) / y_scale;
        coefficient++;
        *coefficient = cosine / y_scale;
    }

    gObjAffineCount = index + 1;
    return index;
}

void Runtime_PushSlotEntry(s32 *slot_entry, s32 slot)
{
    s32 *previous_head;
    s32 slot_offset;
    s32 clamped_slot;

    clamped_slot = slot;
    if (clamped_slot > 0xFF) {
        clamped_slot = 0xFF;
    }
    if (clamped_slot < 0) {
        clamped_slot = 0;
    }
    slot_offset = clamped_slot * 4;
    previous_head = FIELD_AT_OFFSET(slot_offset, s32 **, ((u32)&Data_03001400));
    FIELD_AT_OFFSET(slot_offset, s32 **, ((u32)&Data_03001400)) = slot_entry;
    *slot_entry = previous_head;
}

void Runtime_CopyAndCallRoutine(void *argument)
{
    u32 size;
    LoadedRoutine routine;

    /*
     * FAKEMATCH: a loop that runs once around the size load. It is a
     * scheduling barrier: without it the size literal is loaded before the
     * argument is copied to r8.
     */
    do {
        size = (u32)LoadedRuntime_Size;
    } while (0);
    routine = (LoadedRoutine)Runtime_BumpAllocate(size);
    Dma_Set((const void *)Render_BuildOamList, (void *)routine,
            (size >> 2) | 0x84000000, (volatile u32 *)0x040000d4);
    routine(argument);
    Sys_Free((void *)routine);
}

/* Finds the first run of SIZE / 64 free VRAM blocks, marks them as owned by
   resource ID and returns the run's byte offset, or -1 when ID is out of range
   or no run is free. Occupied blocks are skipped a whole cached entry at a
   time. */
s32 ResourceTable_AllocateBlocks(u32 id, u32 size)
{
    u32 blocks;
    s32 result;
    s32 pos;
    u32 end;
    u32 i;

    blocks = size >> 6;
    if (id > 95) {
        return -1;
    }
    pos = 0;
    for (;;) {
        result = -1;
        if (pos >= 512) {
            goto done;
        }
        if (ResourceBlockOwners[pos] != 0xff) {
            goto occupied;
        }
        result = pos;
        end = blocks + result;
        while (pos < end) {
            if (ResourceBlockOwners[pos] != 0xff) {
                goto occupied;
            }
            pos++;
        }
        for (i = 0; i < blocks; i++) {
            ResourceBlockOwners[result + i] = id;
        }
        goto found;
occupied:
        pos += gVramBlockCache[ResourceBlockOwners[pos]].size >> 6;
    }
found:
    result <<= 6;
done:
    return result;
}

s32 ResourceTable_GetLongestFreeBlockRun(void)
{
    s32 run = 0;
    u8 *marker = ResourceBlockOwners;
    s32 longest = 0;
    s32 remaining = 0x200;

    do {
        if (*marker++ != 0xff) {
            run = 0;
        } else {
            run++;
            if (longest < run)
                longest = run;
        }
        remaining--;
    } while (remaining != 0);
    return longest;
}

s32 Resource_ClearSlotReferences(s32 resource_id)
{
    s32 cleared_count = 0;
    s32 remaining;
    u8 *marker;
    u8 empty_marker;

    if ((u32)resource_id > 0x5f)
        return -1;
    marker = ResourceBlockOwners;
    empty_marker = 0xff;
    remaining = 0x200;
    do {
        if (*marker == resource_id) {
            *marker = empty_marker;
            cleared_count++;
        }
        remaining--;
        marker++;
    } while (remaining != 0);
    if (cleared_count != 0)
        return -1;
    return 0;
}

s32 Resource_ResetEntry(u32 resource_index)
{
    struct ResourceTableEntry *entry = &ResourceTableEntries[resource_index];

    if (resource_index > 95)
        return -1;
    if (entry->flags != 0xffff) {
        Resource_ClearSlotReferences(resource_index);
        entry->flags |= 0xffff;
        entry->value = 0;
    }
    return 0;
}

s32 Resource_ActivateEntry(u32 resource_index)
{
    u16 *entry = (u16 *)&ResourceTableEntries[resource_index];

    if (resource_index > 95)
        return -1;
    if (*entry > 16) {
        s32 value;

        Resource_ClearSlotReferences(resource_index);
        value = 1;
        *entry = value;
    }
    return 0;
}

s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source)
{
    struct VramBlockCacheEntry *entry;
    s32 offset;
    void *destination;

    entry = &gVramBlockCache[slot];
    if (slot > 95)
        return 0;
    if (size > 0x2000)
        return 0;
    if (entry->size > 16) {
        if (entry->size != size) {
            Resource_ResetEntry(slot);
            offset = ResourceTable_AllocateBlocks(slot, size);
        } else {
            offset = entry->offset;
        }
    } else {
        offset = ResourceTable_AllocateBlocks(slot, size);
    }

    if (offset != -1) {
        destination = (void *)(0x06010000 + offset);
        entry->size = size;
        entry->offset = offset;
        if (source != 0) {
            if (source == (const void *)-1) {
                Iwram_ClearWords(destination, size);
            } else {
                Dma_Set(source, destination, (size >> 2) | 0x84000000, (volatile u32 *)0x040000d4);
            }
        }
        return (u32)offset >> 5;
    }
    return 0;
}

/* resource/table/initialize.c */
/* resource/load_into_free_slot.c */
/* resource/initialize.c */
void Resource_InitializeTable(void)
{
    u32 limit = 0x1ff;
    u8 *occupancy_markers = ResourceBlockOwners;
    u32 count = 0;
    u32 empty_marker = 0xff;

    do {
        *occupancy_markers++ = empty_marker;
        count++;
    } while (count <= limit);

    {
        struct ResourceTableEntry *resource_entry = ResourceTableEntries;

        count = 0;
        do {
            resource_entry->flags |= 0xffff;
            resource_entry->value = 0;
            resource_entry++;
            count++;
        } while (count <= 95);
    }
}

/* resource/table/Resource_FindFreeEntry.c */
s32 Resource_FindFreeEntry(void)
{
  s32 free_slot;
  s32 slot_index;
  void *table_base;
  int first_slot;
  void *entry_cursor;
  entry_cursor = (void *)((u32)&ResourceTableEntries);
  free_slot = 0x60;
  first_slot = 0;
  slot_index = first_slot;
  table_base = (void *)((u32)&ResourceTableEntries);
  if ((*((u16 *)(((u8 *)table_base) + 2))) == 0xFFFF)
  {
    return first_slot;
  }
  loop_2:
  slot_index += 1;

  entry_cursor += 4;
  if (slot_index <= 0x5F)
  {
    if ((*((u16 *)(((u8 *)entry_cursor) - -2))) == 0xFFFF)
    {
      free_slot = slot_index;
    } else
    {
      goto loop_2;
    }
  }
  return free_slot;
}

s32 Resource_LoadIntoFreeSlot(s32 arg0)
{
    s32 slot;

    slot = Resource_FindFreeEntry();
    VramBlock_LoadCached(slot, arg0, 0);
    return slot;
}

s32 Resource_GetBuffer(s32 index, s32 value)
{
    return VramBlock_LoadCached(index, ResourceTableEntries[index].value, value);
}

/*
 * Each table update masks interrupts by writing IME the low half of its own
 * address (0x208, bit 0 clear) and restores the saved value afterwards.
 */
void Scheduler_ResetTaskTable(void)
{
    struct SchedulerTask *task = gSchedulerTaskTable;
    s32 remaining;

    gSchedulerTaskCount = 0;
    gSchedulerStatus = 0;
    remaining = 19;
    do {
        task->callback = 0;
        task->state = 0xffff;
        task->mask = 0;
        task++;
        remaining--;
    } while (remaining >= 0);
    gSchedulerTaskCount = 1;
}

void Scheduler_CopyWords(u32 *destination, u32 *source, u32 byte_count)
{
    u32 index;
    byte_count >>= 2;
    for (index = 0; index < byte_count; index++)
        *destination++ = *source++;
}

void Scheduler_SortTasks(void)
{
    struct SchedulerTask saved;
    struct SchedulerTask *base = gSchedulerTaskTable;
    struct SchedulerTask *task;
    s32 pass = 19;
    s32 remaining;
    goto sort_pass;
next_pass:
    base = gSchedulerTaskTable;
sort_pass:
    task = base;
    if (pass <= 0)
        goto finish_pass;
    remaining = pass;
next_task:
    if ((s16)task[1].state > (s16)task->state) {
        memcpy(&saved, task, sizeof(saved));
        base = task;
        task++;
        memcpy(base, task, sizeof(saved));
        memcpy(task, &saved, sizeof(saved));
    } else {
        task++;
    }
    if (--remaining != 0)
        goto next_task;
finish_pass:
    if (--pass > 1)
        goto next_pass;
}

s32 Scheduler_FindCallback(u32 callback)
{
    s32 result;
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 i;

    result = -1;
    task = gSchedulerTaskTable;
    /* FAKEMATCH: the two blocks that run once are meaningless. Without either, the
     * interrupt-master register's address is loaded before the task table's. */
    do {
        saved_interrupt_master = REG_IME;
        {
            volatile u16 *ime = &REG_IME;

            *ime = (u16)(u32)ime;
        }
        do {
            for (i = 0; i <= 19; i++, task++) {
                if (task->callback == callback) {
                    result = i;
                    break;
                }
            }
        } while (0);
        REG_IME = saved_interrupt_master;
    } while (0);
    return result;
}

s32 Scheduler_AddOrUpdateCallback(s32 callback, s32 order)
{
    u32 saved_interrupt_state;
    s32 index;
    struct SchedulerTask *task;
    s32 i;

    index = -1;
    task = gSchedulerTaskTable;
    (void)gSchedulerStatus; /* read and dropped */
    /* FAKEMATCH: the two blocks that run once, as in Scheduler_FindCallback */
    do {
        saved_interrupt_state = REG_IME;
        {
            volatile u16 *ime = &REG_IME;

            *ime = (u16)(u32)ime;
        }
        do {
            i = 0;
            if (task->callback == callback) {
                task->state = order;
                index = 0;
            } else {
            find_existing:
                i++;
                task++;
                if (i <= 19) {
                    if (task->callback == callback) {
                        task->state = order;
                        index = i;
                    } else {
                        goto find_existing;
                    }
                }
            }
            task = gSchedulerTaskTable;
            if (index == -1) {
                i = 0;
                if (task->callback == 0) {
                    task->callback = callback;
                    task->state = order;
                    task->mask = 0;
                    index = 0;
                } else {
                find_empty:
                    i++;
                    task++;
                    if (i <= 19) {
                        if (task->callback == 0) {
                            task->callback = callback;
                            task->state = order;
                            task->mask = 0;
                            index = i;
                        } else {
                            goto find_empty;
                        }
                    }
                }
            }
        } while (0);
        Scheduler_SortTasks();
        REG_IME = saved_interrupt_state;
    } while (0);
    return index;
}

void Scheduler_Idle(void) {}

void Scheduler_EmptyCallback(void) {}

s32 Scheduler_RemoveCallback(u32 callback)
{
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 result;
    s32 i;

    result = -1;
    task = gSchedulerTaskTable;
    /* FAKEMATCH: the two blocks that run once, as in Scheduler_FindCallback */
    do {
        saved_interrupt_master = REG_IME;
        {
            volatile u16 *ime = &REG_IME;

            *ime = (u16)(u32)ime;
        }
        do {
            for (i = 0; i <= 19; i++, task++) {
                if (task->callback == callback) {
                    task->callback = 0;
                    task->state = 0x7fff;
                    result = i;
                    break;
                }
            }
        } while (0);
        REG_IME = saved_interrupt_master;
    } while (0);
    return result;
}

s32 Scheduler_EnableCallbacks(u32 callback)
{
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 result;
    s32 i;

    result = -1;
    task = gSchedulerTaskTable;
    /* FAKEMATCH: the two blocks that run once, as in Scheduler_FindCallback */
    do {
        saved_interrupt_master = REG_IME;
        {
            volatile u16 *ime = &REG_IME;

            *ime = (u16)(u32)ime;
        }
        do {
            for (i = 0; i <= 19; i++, task++) {
                if (callback != 0) {
                    if (task->callback != callback)
                        continue;
                }
                TASK_STATE_HIGH(task) |= 1;
                result = i;
            }
        } while (0);
        REG_IME = saved_interrupt_master;
    } while (0);
    return result;
}

s32 Scheduler_EnableUnmaskedOverlayCallbacks(void)
{
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 result;
    s32 i;

    result = -1;
    task = gSchedulerTaskTable;
    /* FAKEMATCH: the two blocks that run once, as in Scheduler_FindCallback */
    do {
        saved_interrupt_master = REG_IME;
        {
            volatile u16 *ime = &REG_IME;

            *ime = (u16)(u32)ime;
        }
        do {
            for (i = 0; i <= 19; i++, task++) {
                if ((task->callback >> 24) == 2 && (task->mask & 1) == 0) {
                    TASK_STATE_HIGH(task) |= 1;
                    result = i;
                }
            }
        } while (0);
        REG_IME = saved_interrupt_master;
    } while (0);
    return result;
}

s32 Scheduler_SetCallbackMask(u32 callback, u32 mask)
{
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 result;
    s32 i;

    result = -1;
    task = gSchedulerTaskTable;
    /* FAKEMATCH: the two blocks that run once, as in Scheduler_FindCallback */
    do {
        saved_interrupt_master = REG_IME;
        {
            volatile u16 *ime = &REG_IME;

            *ime = (u16)(u32)ime;
        }
        do {
            for (i = 0; i <= 19; i++, task++) {
                if (task->callback == callback) {
                    task->mask = mask;
                    result = i;
                    break;
                }
            }
        } while (0);
        REG_IME = saved_interrupt_master;
    } while (0);
    return result;
}

s32 Scheduler_DisableCallbacks(u32 callback)
{
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 result;
    s32 i;

    result = -1;
    task = gSchedulerTaskTable;
    /* FAKEMATCH: the two blocks that run once, as in Scheduler_FindCallback */
    do {
        saved_interrupt_master = REG_IME;
        {
            volatile u16 *ime = &REG_IME;

            *ime = (u16)(u32)ime;
        }
        do {
            for (i = 0; i <= 19; i++, task++) {
                if (callback == 0 || task->callback == callback) {
                    TASK_STATE_HIGH(task)&= (u8)~1;
                    result = i;
                }
            }
        } while (0);
        REG_IME = saved_interrupt_master;
    } while (0);
    return result;
}

s32 Scheduler_DisableOverlayCallbacks(void)
{
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 result;
    s32 i;

    result = -1;
    task = gSchedulerTaskTable;
    /* FAKEMATCH: the two blocks that run once, as in Scheduler_FindCallback */
    do {
        saved_interrupt_master = REG_IME;
        {
            volatile u16 *ime = &REG_IME;

            *ime = (u16)(u32)ime;
        }
        do {
            for (i = 0; i <= 19; i++, task++) {
                if ((task->callback >> 24) == 2) {
                    TASK_STATE_HIGH(task)&= (u8)~1;
                    result = i;
                }
            }
        } while (0);
        REG_IME = saved_interrupt_master;
    } while (0);
    return result;
}

/* The scheduler's retained globals (defined in the scheduler unit). */
void Runtime_InvokeCallbacksByKey(s32 arg0)
{
    s32 key = arg0;
    u8 *p = ((u8 *)gSchedulerTaskTable);
    s32 i;
    key = key >> 8;
    if (gSchedulerTaskCount == 1) {
        i = 0x15;
        p -= 8;
loop:
        i -= 1;
        if (i != 0) {
            p += 8;
            if (p[5] == key) (*(KeyCallbackFn *)p)();
            goto loop;
        }
    }
}

u32 Random16(void)
{
    u32 value = *(u32 *)((u32)&Data_03001cb4) * 0x41c64e6d + 0x3039;

    *(u32 *)((u32)&Data_03001cb4) = value;
    return (value << 8) >> 16;
}

/* Moves an (x, y, z) position radius along angle in the x-z plane. */
void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position)
{
    *position++ += Iwram_MulQ16(radius, Trig_Sin(angle + 0x4000));
    position++;
    *position += Iwram_MulQ16(radius, Trig_Sin(angle));
}

u16 ArcTan2(s32 x, s32 y)
{
    const u16 *table;
    s32 value;
    s32 ratio;
    s32 result;

    if (x == 0) {
        result = 0;
    } else if (y == 0) {
        result = 0x4000;
    } else {
        ratio = y;
        if (ratio < 0)
            ratio = -ratio;
        value = x;
        if (value < 0)
            value = -value;

        ratio = (value << 8) / ratio;
        result = 0x4000;
        if (ratio <= 0xFB6A) {
            table = Math_ArcTanTable;
            result = 0;

            value = *table;
            table -= 0x40;
            if (ratio > value) {
                result = 0x2000;
                table += 0x80;
            }
            value = *table;
            table -= 0x20;
            if (ratio > value) {
                result |= 0x1000;
                table += 0x40;
            }
            value = *table;
            table -= 0x10;
            if (ratio > value) {
                result |= 0x800;
                table += 0x20;
            }
            value = *table;
            table -= 8;
            if (ratio > value) {
                result |= 0x400;
                table += 0x10;
            }
            value = *table;
            table -= 4;
            if (ratio > value) {
                result |= 0x200;
                table += 8;
            }
            value = *table;
            table -= 2;
            if (ratio > value) {
                result |= 0x100;
                table += 4;
            }
            value = *table;
            table--;
            if (ratio > value) {
                result |= 0x80;
                table += 2;
            }
            value = *table;
            if (ratio > value)
                result |= 0x40;
        }
    }

    if (y < 0)
        result = 0x8000 - result;
    if (x < 0)
        result = -result;

    return (u16)result;
}

s32 Math_IntegerSqrt(s32 value)
{
    s32 trial;
    s32 remainder;
    s32 bit;
    s32 result;

    remainder = value;
    result = 0;
    bit = 0xf;
    do {
        trial = (result << (bit + 1)) + (1 << (bit * 2));
        if (trial <= remainder) {
            result |= 1 << bit;
            remainder -= trial;
        }
        bit -= 1;
    } while (bit >= 0);
    return result;
}

s32 FixedSqrt(s32 value)
{
    s32 (*root)(s32) = Iwram_Sqrt;

    return (s32)((u32)root(value) << 8);
}

s32 Runtime_GetLowTableAddress(void)
{
    return (s32)System_BasicColorPalette;
}

/* ui/text/fmt/text_format_hex_to_work.c */
void Text_FormatHexToWork(u32 value)
{
    u8 *buffer = gNumberTextBuffer;
    const u8 *digits = RomBytes_0800795c;
    s32 index = 7;

    do {
        buffer[index] = digits[value & 0xF];
        value >>= 4;
        index--;
    } while (index >= 0);

    {
        u8 *terminator = gNumberTextBuffer;
        terminator[8] = 0;
    }
}

void Text_FormatSignedDecimalToWork(s32 arg0) {
    u32 *tbl;
    s8 *out;
    s32 count;
    u32 word;
    s32 result;
    s32 val;
    s8 sign;

    result = arg0;
    val = result;
    tbl = (u32 *)Text_PowersOfTen;
    sign = 0x20;
    out = (s8 *)gNumberTextBuffer;
    if (val < 0) {
        val = -val;
        sign = 0x2D;
    }
    word = *tbl++;
    count = 9;
    if ((u32) val < word) {
        do {
            count -= 1;
            *out++ = 0x20;
            if (count == 0) break;
            word = *tbl++;
        } while ((u32) val < word);
    }
    *out++ = sign;
    tbl -= 1;
    if (count != 0) {
        do {
            word = *tbl++;
            result = (u32) val / word;
            *out++ = result + 0x30;
            val -= result * word;
            count -= 1;
        } while (count != 0);
    }
    out[0] = val + 0x30;
    out[1] = 0;
}

void Graphics_FillWordStreamWithF000(u32 count)
{
    u16 *dst = gDebugTextCursor;
    u32 index;

    for (index = 0; index < count; index++)
        *dst++ = 0xf000;
    gDebugTextCursor = dst;
}

#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "fixed_math.h"

/* resource/initialize.c */
/* resource/table/initialize.c */
struct ResourceTableEntry {
    u16 value;
    u16 flags;
};

extern u8 ResourceBlockOwners[];
extern struct ResourceTableEntry ResourceTableEntries[];

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

/* resource/table/find_free_slot.c */
s32 Resource_FindFreeSlot(void)
{
  s32 free_slot;
  s32 slot_index;
  void *table_base;
  int first_slot;
  void *entry_cursor;
  entry_cursor = (void *)ADDR_03001B10;
  free_slot = 0x60;
  first_slot = 0;
  slot_index = first_slot;
  table_base = (void *)ADDR_03001B10;
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

/* resource/load_into_free_slot.c */
s32 Resource_CopyData(s32, s32, s32);
s32 Resource_FindFreeSlot();

s32 Resource_LoadIntoFreeSlot(s32 arg0)
{
    s32 slot;

    slot = Resource_FindFreeSlot();
    Resource_CopyData(slot, arg0, 0);
    return slot;
}

/* resource/table/get_buffer.c */
extern u16 ResourceTableEntries[];
s32 Resource_CopyData(s32, u16, s32);

s32 Resource_GetBuffer(s32 resource_index, s32 copy_value)
{
    return Resource_CopyData(resource_index, ResourceTableEntries[resource_index * 2], copy_value);
}

/* runtime/scheduler/callback_scheduler.c */
struct SchedulerTask {
    u32 callback;
    u16 state;
    u8 mask;
    u8 reserved;
};

#define TASK_STATE_HIGH(task) (((u8 *)&(task)->state)[1])

extern volatile u8 gIwCallbackScheduler;
extern u8 gIw2CallbackScheduler;
extern struct SchedulerTask gIw3[20];
extern volatile u16 gUnk;

void Scheduler_ResetTaskTable(void)
{
    struct SchedulerTask *task = gIw3;
    s32 remaining = ((u32)task | ~(u32)task) + 1;
    gIw2CallbackScheduler = remaining;
    gIwCallbackScheduler = remaining;
    {
        u32 zero = 0;
        remaining = 19;
        do {
            task->callback = zero;
            task->state = 0xffff;
            task->mask = zero;
            task++;
            remaining--;
        } while (remaining >= 0);
    }
    gIw2CallbackScheduler = 1;
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
    struct SchedulerTask *base = gIw3;
    struct SchedulerTask *task;
    s32 pass = 19;
    s32 remaining;
    goto sort_pass;
next_pass:
    base = gIw3;
sort_pass:
    task = base;
    if (pass <= 0)
        goto finish_pass;
    remaining = pass;
next_task:
    if ((s16)task[1].state > (s16)task->state) {
        __builtin_memcpy(&saved, task, sizeof(saved));
        base = task;
        task++;
        __builtin_memcpy(base, task, sizeof(saved));
        __builtin_memcpy(task, &saved, sizeof(saved));
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
    s32 returned_result;
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 i;

    result = -1;
    task = gIw3;
    do {
        saved_interrupt_master = gUnk;
        gUnk = (u16)&gUnk;
        do {
            for (i = 0; i <= 19; i++, task++) {
                if (task->callback == callback) {
                    result = i;
                    break;
                }
            }
        } while (0);
        gUnk = saved_interrupt_master;
        returned_result = result;
    } while (0);
    return returned_result;
}

s32 Scheduler_AddOrUpdateCallback(s32 callback, s32 order)
{
    u32 saved_interrupt_state;
    s32 index;
    s32 returned_index;
    struct SchedulerTask *task;
    volatile u8 *scheduler_status;
    s32 i;

    scheduler_status = &gIwCallbackScheduler;
    index = -1;
    task = gIw3;
    (void)*scheduler_status;
    do {
        saved_interrupt_state = gUnk;
        gUnk = (u16)&gUnk;
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
            task = gIw3;
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
        gUnk = saved_interrupt_state;
        returned_index = index;
    } while (0);
    return returned_index;
}

void Scheduler_Idle(void) {}

void Scheduler_EmptyCallback(void) {}

s32 Scheduler_RemoveCallback(u32 callback)
{
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 result;
    s32 returned_result;
    s32 i;

    result = -1;
    task = gIw3;
    do {
        saved_interrupt_master = gUnk;
        gUnk = (u16)&gUnk;
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
        gUnk = saved_interrupt_master;
        returned_result = result;
    } while (0);
    return returned_result;
}

s32 Scheduler_EnableCallbacks(u32 callback)
{
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 result;
    s32 returned_result;
    s32 i;

    result = -1;
    task = gIw3;
    do {
        saved_interrupt_master = gUnk;
        gUnk = (u16)&gUnk;
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
        gUnk = saved_interrupt_master;
        returned_result = result;
    } while (0);
    return returned_result;
}

s32 Scheduler_EnableUnmaskedOverlayCallbacks(void)
{
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 result;
    s32 returned_result;
    s32 i;

    result = -1;
    task = gIw3;
    do {
        saved_interrupt_master = gUnk;
        gUnk = (u16)&gUnk;
        do {
            for (i = 0; i <= 19; i++, task++) {
                if ((task->callback >> 24) == 2 && (task->mask & 1) == 0) {
                    TASK_STATE_HIGH(task) |= 1;
                    result = i;
                }
            }
        } while (0);
        gUnk = saved_interrupt_master;
        returned_result = result;
    } while (0);
    return returned_result;
}

s32 Scheduler_SetCallbackMask(u32 callback, u32 mask)
{
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 result;
    s32 returned_result;
    s32 i;

    result = -1;
    task = gIw3;
    do {
        saved_interrupt_master = gUnk;
        gUnk = (u16)&gUnk;
        do {
            for (i = 0; i <= 19; i++, task++) {
                if (task->callback == callback) {
                    task->mask = mask;
                    result = i;
                    break;
                }
            }
        } while (0);
        gUnk = saved_interrupt_master;
        returned_result = result;
    } while (0);
    return returned_result;
}

s32 Scheduler_DisableCallbacks(u32 callback)
{
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 result;
    s32 returned_result;
    s32 i;

    result = -1;
    task = gIw3;
    do {
        saved_interrupt_master = gUnk;
        gUnk = (u16)&gUnk;
        do {
            for (i = 0; i <= 19; i++, task++) {
                if (callback == 0 || task->callback == callback) {
                    TASK_STATE_HIGH(task)&= (u8)~1;
                    result = i;
                }
            }
        } while (0);
        gUnk = saved_interrupt_master;
        returned_result = result;
    } while (0);
    return returned_result;
}

s32 Scheduler_DisableOverlayCallbacks(void)
{
    struct SchedulerTask *task;
    u32 saved_interrupt_master;
    s32 result;
    s32 returned_result;
    s32 i;

    result = -1;
    task = gIw3;
    do {
        saved_interrupt_master = gUnk;
        gUnk = (u16)&gUnk;
        do {
            for (i = 0; i <= 19; i++, task++) {
                if ((task->callback >> 24) == 2) {
                    TASK_STATE_HIGH(task)&= (u8)~1;
                    result = i;
                }
            }
        } while (0);
        gUnk = saved_interrupt_master;
        returned_result = result;
    } while (0);
    return returned_result;
}

/* runtime/callbacks/invoke_by_key.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))
typedef s32 (*KeyCallbackFn)(void);
extern u8 gIwInvokeByKey;
extern u8 gIw2InvokeByKey[];
void Runtime_InvokeCallbacksByKey(s32 arg0)
{
    s32 key = arg0;
    u8 *p = gIw2InvokeByKey;
    s32 i;
    key = key >> 8;
    if (gIwInvokeByKey == 1) {
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

/* math/random.c */
/* math/random_16.c */
u32 Random16(void)
{
    s32 value;
    value = (*(s32 *)ADDR_03001CB4 * 0x41c64e6d) + 0x3039;
    *(s32 *)ADDR_03001CB4 = value;
    return (u32)(value << 8) >> 16;
}

/* math/arctan2.c */
s32 FixedPoint_Ratio(s32 numerator, s32 denominator);
extern const u16 gRomRandom[];

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

        ratio = FixedPoint_Ratio(value << 8, ratio);
        result = 0x4000;
        if (ratio <= 0xFB6A) {
            table = gRomRandom;
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

/* math/integer_sqrt.c */
s32 Math_IntegerSqrt(s32 value)
{
    s32 trial;
    s32 remainder;
    s32 bit;
    s32 result;

    remainder = value;
    result = 0;
    bit = 0xF;
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

/* math/fixed_sqrt.c */
/*
 * Fixed-point square root: hand the value to the relocated routine at
 * 0x030001d8 and shift its result up by eight.
 */

/*
 * Sys_SetMode names a `bx rN` slot, so the call is indirect through the
 * register that slot selects; the trailing argument is the callee address
 * loaded into that register, not a parameter of the callee.
 */

/*
 * The callee takes one argument and returns one. Callers feed it a sum of
 * squares and use the result as a length, which reads as a square root but
 * is not established.
 */
s32 FixedSqrt(s32 value, s32 unused1, s32 unused2)
{
    return (s32)((u32)Sys_SetMode(value, unused1, unused2, 0x030001D8) << 8);
}

/* runtime/get_low_table_address.c */
extern const u8 gRomLowTableAddress[];

s32 Runtime_GetLowTableAddress(void)
{
    return (s32)gRomLowTableAddress;
}

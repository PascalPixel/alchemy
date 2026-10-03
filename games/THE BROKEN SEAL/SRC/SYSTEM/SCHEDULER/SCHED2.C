#include "AFFINE.H"
#include "RESOURCE.H"
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


typedef void (*KeyCallbackFn)(void);

/* The linear-congruential generator updates one unsigned word. */
extern u32 Data_03001cb4;
extern const u16 Math_ArcTanTable[];

extern const u8 System_BasicColorPalette[];

extern u8 gNumberTextBuffer[];

/* ui/text/fmt/format_hex_to_work.c */
extern const u8 RomBytes_0800795c[];

/* ui/text/format_signed_decimal_to_work.c */
extern u8 Text_PowersOfTen[];

/* graphics/fill_word_stream_with_f000.c */
extern u16 *gDebugTextCursor;


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
    remaining = SCHEDULER_TBS_TASK_COUNT - 1;
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
    /* FAKEMATCH: the ordinary nested for loops reduce this
       complete native sort from 84 to 80 bytes. Retain its existing
       leading test and reused swap cursor over the actual task records. */
    struct SchedulerTask saved;
    struct SchedulerTask *base = gSchedulerTaskTable;
    struct SchedulerTask *task;
    s32 pass = SCHEDULER_TBS_TASK_COUNT - 1;
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
            for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
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
            for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
                if (task->callback == (u32)callback) {
                    task->state = order;
                    index = i;
                    break;
                }
            }
            task = gSchedulerTaskTable;
            if (index == -1) {
                for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
                    if (task->callback == 0) {
                        task->callback = callback;
                        task->state = order;
                        task->mask = 0;
                        index = i;
                        break;
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
            for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
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
            for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
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
            for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
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
            for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
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
            for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
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
            for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
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

/* Run tasks whose high state byte selects this callback key. */
void Runtime_InvokeCallbacksByKey(s32 key)
{
    struct SchedulerTask *task = gSchedulerTaskTable;
    s32 remaining;

    key >>= 8;
    if (gSchedulerTaskCount == 1) {
        remaining = SCHEDULER_TBS_TASK_COUNT + 1;
        task--;
next_task:
        remaining--;
        if (remaining != 0) {
            task++;
            if (TASK_STATE_HIGH(task) == key) {
                KeyCallbackFn callback = *(KeyCallbackFn *)&task->callback;

                /* FAKEMATCH: three ordinary void-call shapes select r3
                   and _call_via_r3. The inherited C89 ignored-result cast
                   keeps native r0/_call_via_r0 at the same 56-byte extent.
                   Callback owners remain void; no result is consumed. */
                ((s32 (*)(void))callback)();
            }
            goto next_task;
        }
    }
}

u32 Random16(void)
{
    u32 value = Data_03001cb4 * 0x41c64e6d + 0x3039;

    Data_03001cb4 = value;
    return (value << 8) >> 16;
}

/* Moves an (x, y, z) position radius along angle in the x-z plane. */
void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position)
{
    *position++ += Iwram_MulQ16(radius, Trig_Sin(angle + 0x4000));
    position++;
    *position += Iwram_MulQ16(radius, Trig_Sin(angle));
}

s32 ArcTan2(s32 x, s32 y)
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

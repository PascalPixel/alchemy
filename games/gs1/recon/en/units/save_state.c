#include "types.h"
#include "fixed_math.h"
#include "flash.h"
#include "global_cells.h"

/* Ordered save-state translation unit: 080056cc through 08005c68. */

struct DmaRegisters {
    u32 source;
    u32 destination;
    u32 control;
};

struct SaveSlotHeader {
    u8 signature[7];
    u8 record_id;
    u16 checksum;
    u16 sequence;
    u8 reserved[4];
};

struct SaveWorkspace {
    u8 occupied[16];
    u8 record_id[16];
    u16 sequence[16];
    union {
        struct {
            struct SaveSlotHeader header;
            u8 payload[0xff0];
        } record;
        u8 bytes[0x1000];
    } slot;
    u8 summary[3][64];
};

#define SAVE_WORKSPACE (*(struct SaveWorkspace **)ADDR_03001F1C)
#define DMA3 ((volatile struct DmaRegisters *)0x040000d4)
#define DMA3_BUSY 0x80000000
#define SAVE_HEADER_TEMPLATE ((const struct SaveSlotHeader *)0x080079b8)
#define SAVE_SIGNATURE ((const u8 *)0x080079b0)

#define START_DMA(source_, destination_, control_) do { \
    DMA3->source = (u32)(source_); \
    DMA3->destination = (u32)(destination_); \
    DMA3->control = (control_); \
} while (0)

#define WAIT_DMA() do { \
    while ((DMA3->control & DMA3_BUSY) != 0) {} \
} while (0)

void *Func_080048f4(s32 kind, s32 size);
s32 Func_080030f8(s32 frames);
s32 Func_080072f0(s32, s32, s32, s32);
s32 Func_08006c68(u16 sector, s32 source);
s32 Func_080058ac(s32 index);
u32 Func_08005810(s32 record_id);
u32 Func_08005868();
s32 Func_08005920(s32 record_id, void *source);
u32 Func_08005a78(s32 record_id, void *destination);
u32 Func_08005ac0(s32 record_id);
s32 Func_08005ae0(void);
u32 Func_08005b24(s32 record_id);
s32 Func_08005b64(s32 index);
s32 Func_08005c08(u8 *left, u8 *right, s32 count);
u32 Func_08005c2c(s32 record_id);
s32 Func_08005c68(void);

s32 Func_080056cc(void)
{
    struct SaveWorkspace *work;
    struct SaveSlotHeader header;
    u32 zero;
    s32 attempt;
    s32 index;

    work = Func_080048f4(0x33, sizeof(*work));
    zero = 0;
    START_DMA(&zero, work, 0x85000440);
    SetFlashTimerIntr(2, (void (**)(void))0x030000f4);

    attempt = 0;
    while (attempt <= 7 && IdentifyFlash() != 0) {
        Func_080030f8(1);
        attempt++;
    }
    if (attempt > 7)
        return 1;

    for (index = 0; index <= 15; index++) {
        s32 status;

        work->occupied[index] = 0;
        work->record_id[index] = 0x10;
        work->sequence[index] = 0xffff;
        status = Func_080058ac(index);
        START_DMA(&work->slot, &header, 0x84000004);
        WAIT_DMA();

        if (Func_08005c08(header.signature, (u8 *)SAVE_SIGNATURE, 7) != 0)
            continue;
        work->sequence[index] = header.sequence;
        if (header.record_id > 15 || status != 0)
            continue;

        work->occupied[index] = 1;
        work->record_id[index] = header.record_id;
        if (status < index) {
            do {
                if (work->record_id[status] == header.record_id) {
                    if (work->sequence[status] < header.sequence)
                        work->occupied[status] = 0;
                    else
                        work->occupied[index] = 0;
                }
                status++;
            } while (status < index);
        }
    }
    return 0;
}

u32 Func_08005810(s32 mode)
{
    u32 empty[16];
    s32 count;
    u32 index;
    u8 *slot;
    u8 value;

    slot = *(u8 **)ADDR_03001F1C;
    count = 0;
    index = 0;
    do {
        value = *slot++;
        if (value == 0)
            empty[count++] = index;
        index += 1;
    } while (index <= 0xFU);
    index = 0x10;
    if (count != 0) {
        if (count == 1) {
            index = empty[0];
            if (Func_08005b24(mode) == 0x10)
                index = 0x10;
        } else {
            index = UnsignedModulo(Random16(), count);
            index = empty[index];
        }
    }
    return index;
}

u32 Func_08005868(code)
u16 code;
{
    s32 *param = (s32 *)0x02004c04;
    s32 result;
    struct SaveWorkspace *work;
    s32 value;

    work = SAVE_WORKSPACE;
    value = code & 0xffff;
    if ((Func_080072f0(value, (s32)&work->slot,
                       (s32)param, *param) << 16) != 0) {
        return 1U;
    }
    result = Func_08006c68(value, (s32)&work->slot);
    return (u32)((0 - result) | result) >> 31;
}

s32 Func_080058ac(s32 index)
{
    struct SaveWorkspace *work;
    struct SaveSlotHeader header;
    u32 checksum;

    work = SAVE_WORKSPACE;
    ReadFlash((u16)index, 0, work->slot.bytes, sizeof(work->slot));
    START_DMA(&work->slot, &header, 0x84000004);
    WAIT_DMA();
    checksum = Func_08005ae0();
    return (u16)checksum - header.checksum;
}

typedef u16 (*Callback_08005904)(u16);
extern Callback_08005904 Data_02004c14;

u16 Func_08005904(u16 value)
{
    return Data_02004c14(value);
}

s32 Func_08005920(s32 record_id, void *source)
{
    struct SaveWorkspace *work;
    struct SaveSlotHeader header;
    u32 zero;
    s32 current;
    s32 slot;

    work = SAVE_WORKSPACE;
    zero = 0;
    START_DMA(&zero, &work->slot, 0x85000400);
    WAIT_DMA();
    current = Func_08005b24(record_id);
    slot = Func_08005810(record_id);
    if (slot > 15)
        return 1;

    START_DMA(source, work->slot.record.payload, 0x840003fc);
    WAIT_DMA();
    START_DMA(SAVE_HEADER_TEMPLATE, &header, 0x84000002);
    WAIT_DMA();
    header.record_id = record_id;
    header.checksum = Func_08005ae0();
    header.sequence = Func_08005c2c(record_id) + 1;
    START_DMA(&header, &work->slot.record.header, 0x84000004);
    WAIT_DMA();

    if (Func_08005868(slot) != 0)
        return 1;
    if (current <= 15 && Func_08005b64(current) != 0)
        return 1;

    if (header.sequence > 0xfde8) {
        header.sequence = 1;
        START_DMA(&header, &work->slot.record.header, 0x84000004);
        WAIT_DMA();
        if (Func_08005868(current) != 0)
            return 1;
        if (Func_08005b64(slot) != 0)
            return 1;
        slot = current;
    }

    work->occupied[slot] = 1;
    work->record_id[slot] = record_id;
    work->sequence[slot] = header.sequence;
    return 0;
}

u32 Func_08005a78(s32 record_id, void *destination)
{
    struct SaveWorkspace *work;
    u32 index;

    work = SAVE_WORKSPACE;
    index = Func_08005b24(record_id);
    if (index > 15)
        return 1;
    Func_080058ac(index);
    /* The reference's standalone three-register Thumb STMIA is not emitted
     * by canonical GCC 2.96 from an ordinary-C aggregate store. */
    *DMA3 = (struct DmaRegisters){
        (u32)work->slot.record.payload,
        (u32)destination,
        0x840003fc
    };
    WAIT_DMA();
    return 0;
}

u32 Func_08005ac0(s32 record_id)
{
    u32 index;
    s32 result;

    index = Func_08005b24(record_id);
    if (index > 15)
        return 1U;
    result = Func_08005b64(index);
    return (u32)((0 - result) | result) >> 31;
}

s32 Func_08005ae0(void)
{
    struct SaveWorkspace *work;
    u32 limit;
    u32 offset;
    s32 sum;

    work = SAVE_WORKSPACE;
    limit = 0xfe7;
    sum = 0;
    offset = 0;
    do {
        sum += work->slot.bytes[offset + 0x10];
        sum += work->slot.bytes[offset + 0x11];
        sum += work->slot.bytes[offset + 0x12];
        sum += work->slot.bytes[offset + 0x13];
        sum += work->slot.bytes[offset + 0x14];
        sum += work->slot.bytes[offset + 0x15];
        sum += work->slot.bytes[offset + 0x16];
        sum += work->slot.bytes[offset + 0x17];
        offset += 8;
    } while (offset <= limit);
    return sum;
}

u32 Func_08005b24(s32 record_id)
{
    struct SaveWorkspace *work;
    u32 best_sequence;
    u32 best_slot;
    u32 index;

    work = SAVE_WORKSPACE;
    best_slot = 0x10;
    best_sequence = 0;
    index = 0;
    do {
        if (work->occupied[index] != 0 && record_id == work->record_id[index]) {
            if (best_sequence < work->sequence[index]) {
                best_sequence = work->sequence[index];
                best_slot = index;
            }
        }
        index++;
    } while (index <= 15);
    return best_slot;
}

s32 Func_08005b64(s32 index)
{
    struct SaveWorkspace *work;
    struct SaveSlotHeader header;
    u32 zero;

    work = SAVE_WORKSPACE;
    zero = 0;
    START_DMA(&zero, &header, 0x85000004);
    WAIT_DMA();
    START_DMA(SAVE_HEADER_TEMPLATE, &header, 0x84000002);
    WAIT_DMA();
    header.record_id = 0x10;
    header.sequence = 0;
    START_DMA(&header, &work->slot.record.header, 0x84000004);
    WAIT_DMA();
    if (Func_08005868(index) != 0)
        return 1;
    work->occupied[index] = 0;
    work->record_id[index] = 0x10;
    work->sequence[index] = 0;
    return 0;
}

s32 Func_08005c08(u8 *left, u8 *right, s32 count)
{
    s32 difference = 0;

    while (count != 0) {
        difference = *left - *right;
        if (difference != 0)
            break;
        count--;
        left++;
        right++;
    }
    return difference;
}

u32 Func_08005c2c(s32 record_id)
{
    struct SaveWorkspace *work;
    u32 best_sequence;
    u32 index;

    work = SAVE_WORKSPACE;
    best_sequence = 0;
    index = 0;
    do {
        if (work->occupied[index] != 0 && record_id == work->record_id[index]) {
            if (best_sequence < work->sequence[index])
                best_sequence = work->sequence[index];
        }
        index++;
    } while (index <= 15);
    return best_sequence;
}

s32 Func_08005c68(void)
{
    struct SaveWorkspace *work;
    u8 *summary;
    u32 zero;
    s32 group;
    s32 count;

    work = SAVE_WORKSPACE;
    summary = work->summary[0];
    count = 0;
    group = 0;
    do {
        u32 index;

        zero = 0;
        START_DMA(&zero, summary, 0x85000010);
        index = Func_08005b24(group);
        if (index <= 15) {
            ReadFlash((u16)index, 0, summary, 64);
            count++;
        }
        index = Func_08005b24(group + 3);
        if (index <= 15)
            ReadFlash((u16)index, 0x110, summary + 56, 4);
        else
            *(u32 *)(summary + 56) = zero;
        group++;
        summary += 64;
    } while (group <= 2);
    return count;
}

#ifndef GUARD_SAVE_STATE_FAMILY_H
#define GUARD_SAVE_STATE_FAMILY_H

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
s32 Func_08005920(s32 record_id, void *source);
u32 Func_08005a78(s32 record_id, void *destination);
u32 Func_08005ac0(s32 record_id);
s32 Func_08005ae0(void);
u32 Func_08005b24(s32 record_id);
s32 Func_08005b64(s32 index);
s32 Func_08005c08(u8 *left, u8 *right, s32 count);
u32 Func_08005c2c(s32 record_id);
s32 Func_08005c68(void);

#endif

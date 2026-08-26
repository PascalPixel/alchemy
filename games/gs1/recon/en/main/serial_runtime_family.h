#ifndef GUARD_SERIAL_RUNTIME_FAMILY_H
#define GUARD_SERIAL_RUNTIME_FAMILY_H

#include "types.h"
#include "global_cells.h"

/* Ordered serial/runtime translation unit: 08005d10 through 0800651c. */

#define SERIAL_RUNTIME_TU

typedef void (*InterruptHandler)(void);

struct DmaRegisters {
    u32 source;
    u32 destination;
    u32 control;
};

struct SerialPacket {
    u8 sequence;
    u8 peer_pair;
    u16 checksum;
    u8 payload[24];
    u8 padding[4];
};

union SerialDataRegisters {
    u32 words[2];
    u16 halfwords[4];
};

struct SerialRuntime {
    u8 mode;
    u8 phase;
    u8 received_mask;
    u8 current_mask;
    u8 channel_flags[4];
    u8 transfer_enabled;
    u8 is_parent;
    u8 reserved_0a;
    u8 sequence;
    u8 reserved_0c[8];
    s32 send_index;
    s32 receive_index[2];
    u8 reserved_20[8];
    u16 *send_buffer[2];
    u16 *incoming_buffer[4];
    u16 *ready_buffer[4];
    u16 *pending_buffer[4];
    u8 storage[0x100];
};

struct SerialTransferState {
    u8 active;
    u8 status;
    u8 flags;
    u8 peer_flags;
    u8 reserved[20];
    u16 value;
};

#define SERIAL_RUNTIME ((struct SerialRuntime *)0x02002240)
#define SERIAL_TRANSFER ((struct SerialTransferState *)0x02002220)
#define DMA3 ((volatile struct DmaRegisters *)0x040000d4)
#define REG_SIODATA32 ((volatile u32 *)0x04000120)
#define REG_SIOCNT (*(volatile u32 *)0x04000128)
#define REG_SIOCNT16 (*(volatile u16 *)0x04000128)
#define REG_SIODATA8 (*(volatile u16 *)0x0400012a)
#define REG_RCNT (*(volatile u16 *)0x04000134)
#define REG_TM3CNT (*(volatile u32 *)0x0400010c)
#define REG_TM3CNT_H (*(volatile u16 *)0x0400010e)
#define REG_IE (*(volatile u16 *)0x04000200)
#define REG_IF (*(volatile u16 *)0x04000202)
#define REG_IME (*(volatile u16 *)0x04000208)

#define SERIAL_ACTIVE_A (*(volatile s32 *)0x02002080)
#define SERIAL_VALUE_A (*(volatile u16 *)0x02002008)
#define SERIAL_ACTIVE_B (*(volatile s32 *)0x020023ac)
#define SERIAL_VALUE_B (*(volatile u16 *)0x02002238)
#define SERIAL_RESULT (*(volatile u8 *)0x020023a4)

#define START_DMA(source_, destination_, control_) do { \
    DMA3->source = (u32)(source_); \
    DMA3->destination = (u32)(destination_); \
    DMA3->control = (control_); \
} while (0)

void Func_0800307c(s32 index, s32 priority, InterruptHandler handler);
s32 Func_080030f8(s32 frames);
void Func_080060e8(void *payload);
u8 Func_0800615c(void *payload);
void Func_08006240(void);
void Func_0800651c(void);
extern u8 Data_00000000[];
extern u8 Data_000000c0[];
extern struct SerialTransferState Data_02002220;
extern volatile s32 Data_02002080;
extern volatile u16 Data_02002008;
extern volatile s32 Data_020023ac;
extern volatile u16 Data_02002238;
extern volatile u16 Data_04000208;

#endif

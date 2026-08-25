#include "types.h"

struct TransferSlot {
    s32 source;
    u16 size;
    u8 pending;
    u8 reserved;
};

extern volatile u8 Data_03001a10;
extern struct TransferSlot Data_03001a20[20];
extern volatile u16 Data_04000208;
void Func_08004144(void);

#define Scheduler_AddOrUpdateCallback Func_080041d8

s32 Scheduler_AddOrUpdateCallback(s32 source, s32 size)
{
    u32 saved_interrupt_state;
    s32 index;
    struct TransferSlot *slot;
    s32 i;

    index = -1;
    slot = Data_03001a20;
    (void)Data_03001a10;
    saved_interrupt_state = Data_04000208;
    Data_04000208 = (u16)&Data_04000208;

    i = 0;
    if (slot->source == source) {
        slot->size = size;
        index = 0;
    } else {
find_existing:
        i++;
        slot++;
        if (i <= 19) {
            if (slot->source == source) {
                slot->size = size;
                index = i;
            } else {
                goto find_existing;
            }
        }
    }
    if (index == -1) {
        slot = Data_03001a20;
        i = 0;
        if (slot->source == 0) {
            slot->source = source;
            slot->size = size;
            slot->pending = 0;
            index = 0;
        } else {
find_empty:
            i++;
            slot++;
            if (i <= 19) {
                if (slot->source == 0) {
                    slot->source = source;
                    slot->size = size;
                    slot->pending = 0;
                    index = i;
                } else {
                    goto find_empty;
                }
            }
        }
    }
    Func_08004144();
    Data_04000208 = saved_interrupt_state;
    return index;
}

void Scheduler_Idle(void)
{
}

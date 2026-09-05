#include "types.h"

u8 *Runtime_GetObject(s32);

extern u8 *Data_03001f2c;

void Djinn_CountTurns(u8 *counts)
{
    u8 *work;
    u16 *owner_ids;
    u8 *owner;
    u32 *row;
    s32 owner_index;
    s32 row_index;
    s32 bit;
    s32 active;
    s32 count;
    u32 mask;
    u32 one;

    work = Data_03001f2c;
    owner_index = 0;
    if (owner_index < work[0x219]) {
        one = 1;
        owner_ids = (u16 *)(work + 0x208);
        do {
            owner = Runtime_GetObject(*owner_ids);
            row_index = 0;
            count = 0;
            row = (u32 *)(owner + 0xf8);
            do {
                active = row[4];
                bit = 0;
                do {
                    mask = one << bit;
                    if (active & mask)
                        count++;
                    else if (row[0] & mask)
                        count++;
                    bit++;
                } while (bit <= 19);
                row_index++;
                row++;
            } while (row_index <= 3);
            counts[owner_index] = count;
            owner_index++;
            owner_ids++;
        } while (owner_index < work[0x219]);
    }
}

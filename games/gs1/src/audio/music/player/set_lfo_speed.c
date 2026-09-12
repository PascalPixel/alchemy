#include "types.h"

void MusicTrack_ClearModulation(u8 *entry);

void MusicPlayer_SetLfoSpeed(u8 *object, u32 selected, u32 value)
{
    u16 selected_bits = selected;
    u8 stored_value = value;

    if (*(u32 *)(object + 52) == 0x68736D53) {
        s32 count;
        u8 *entry;
        u32 mask;

        *(u32 *)(object + 52) = *(u32 *)(object + 52) + 1;
        count = object[8];
        entry = *(u8 **)(object + 44);
        mask = 1;

        if (count > 0) {
            u8 test_value = stored_value;

            do {
                if ((selected_bits & mask) != 0 && (entry[0] & 0x80) != 0) {
                    entry[25] = stored_value;
                    if (test_value == 0) {
                        MusicTrack_ClearModulation(entry);
                    }
                }
                count--;
                entry += 80;
                mask <<= 1;
            } while (count > 0);
        }

        *(u32 *)(object + 52) = 0x68736D53;
    }
}

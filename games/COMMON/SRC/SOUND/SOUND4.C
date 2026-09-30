#include "TYPES.H"

void MusicTrack_ClearModulation(void *);

void MusicPlayer_SetModulationDepth(u8 *player, s32 mask_arg, s32 value_arg)
{
    register s32 mask = (u16)mask_arg;
    register s32 value = (u8)value_arg;
    register s32 count;
    register u8 *entry;
    register u32 bit;

    if (*(u32 *)(player + 0x34) != 0x68736d53)
        return;

    *(u32 *)(player + 0x34) += 1;
    count = player[8];
    entry = *(u8 **)(player + 0x2c);
    bit = 1;
    if (count > 0) {
        register s32 check = value;
        do {
            if ((mask & bit) && (entry[0] & 0x80)) {
                entry[0x17] = value;
                if (check == 0)
                    MusicTrack_ClearModulation(entry);
            }
            count--;
            entry += 0x50;
            bit <<= 1;
        } while (count > 0);
    }
    *(u32 *)(player + 0x34) = 0x68736d53;
}

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

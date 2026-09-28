#include "TYPES.H"
#include "DMA.H"

#define SOUND_PRESET_COUNT 5
#define SOUND_PRESET_SIZE 152

extern u8 Data_08000404[]; /* five 152-byte sound presets in ROM */
extern u8 Data_03000bd8[]; /* active sound parameters in IWRAM */

s32 Sound_LoadPresetParameters(s32 preset)
{
    s32 index;

    index = preset;
    if ((u32)preset > SOUND_PRESET_COUNT - 1) {
        index = 0;
    }
    Dma_Set(Data_08000404 + index * SOUND_PRESET_SIZE, Data_03000bd8, 0x84000026,
            (volatile u32 *)0x040000d4);
    return 0;
}

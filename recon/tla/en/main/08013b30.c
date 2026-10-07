/* ported from games/THE BROKEN SEAL/SRC/SYSTEM/VBLANK.C */
#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "DMA.H"
#include "SERIAL_RUNTIME.H"
#include "IO_REG.H"
#include "CALLBACK_SCHEDULER.H"
#include "IO_WRITE_QUEUE.H"
#include "FRAME.H"

extern u16 gLinkStatus;

#define SOUND_PRESET_COUNT 5
#define SOUND_PRESET_SIZE 152



s32 Sound_LoadPresetParameters(s32 preset)
{
    s32 index;

    index = preset;
    if ((u32)preset > SOUND_PRESET_COUNT - 1) {
        index = 0;
    }
    Dma_Set(IwramCodeRecords + index * SOUND_PRESET_SIZE, Data_03000bd8, 0x84000026,
            REG_DMA3);
    return 0;
}

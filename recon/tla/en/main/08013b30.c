/* ported from games/tbs/src/system/vblank.c */
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "dma.h"
#include "serial_runtime.h"
#include "io_reg.h"
#include "callback_scheduler.h"
#include "io_write_queue.h"
#include "frame.h"

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
    Dma_Set(IwramCodeRecords + index * SOUND_PRESET_SIZE, IwramSoundPresetCode, 0x84000026,
            REG_DMA3);
    return 0;
}

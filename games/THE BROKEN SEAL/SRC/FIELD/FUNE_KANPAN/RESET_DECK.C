/* Restart the deck: reseed the two wave angles, stop layer 5's scroll and
   apply the entry state again. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

/* The map work pointer heads the field's IWRAM pointer block; the event
   work pointer is its twentieth word. */
extern u8 *gMapWork;
extern s32 FuneKanpan_WaveAngleX;
extern s32 FuneKanpan_WaveAngleY;
extern s32 FuneKanpan_LayerScroll[];
extern s32 FuneKanpan_LayerSpeed[];

void FuneKanpan_ApplyEntryState(void);

s32 FuneKanpan_ResetDeck(void)
{
    u8 **base = &gMapWork;
    u8 *map = base[0] + 0x104;

    Engine_GameFlagClear(0x11c);
    *(s32 *)(base[19] + 0x1c0) = 0x209;
    *(s32 *)(map + 0x1c) = 0;
    FuneKanpan_WaveAngleX = (u16)Engine_RandomNext();
    FuneKanpan_WaveAngleY = (u16)Engine_RandomNext();
    FuneKanpan_LayerScroll[0] = 0;
    FuneKanpan_LayerScroll[1] = 0;
    FuneKanpan_LayerSpeed[0] = 0;
    Engine_MapRedraw();
    Engine_TaskWait(1);
    FuneKanpan_ApplyEntryState();
    return 0;
}

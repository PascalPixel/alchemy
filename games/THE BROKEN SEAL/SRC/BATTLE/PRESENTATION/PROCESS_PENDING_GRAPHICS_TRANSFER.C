/* Battle presentation: once per frame, flush the effect canvas to VRAM when
   an effect marked it pending, in the transfer mode the effect chose (plain
   copy, copy and clear, one of two blends, or a fade by amount); otherwise
   count the frames since the last flush. */
#include "TYPES.H"
#include "IWRAM_CALL.H"

extern u8 gBattleFxWork[];

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

static __inline__ void CopyWords(void *destination, const void *source, s32 size)
{
    Iwram_CopyWords(destination, source, size);
}

static __inline__ void FillWords(void *destination, s32 size, s32 value)
{
    Iwram_FillWords(destination, size, value);
}

void ColorBuffer_BackupAndHalve(void *source, void *destination, s32 size);
void ColorBuffer_BackupAndScaleThreeQuarters(void *source, void *destination, s32 size);
void ColorBuffer_BackupAndDarken(void *source, s32 amount, void *destination, s32 size);
void ColorBuffer_BackupAndBrighten(void *source, s32 amount, void *destination, s32 size);

void BattlePresentation_ProcessPendingGraphicsTransfer(void)
{
    void **heap_cache = (void **)gBattleFxWork;
    void *work = heap_cache[0];
    void *source;
    s32 *counter;
    s32 next_counter;

    if (FIELD(work, s32, 0x7824) == 1) {
        source = heap_cache[1];
        switch (FIELD(work, u32, 0x7780)) {
        case 0:
            CopyWords((void *)0x06004000, source, 0x4000);
            break;
        case 1:
            CopyWords((void *)0x06004000, source, 0x4000);
            FillWords(source, 0x4000, FIELD(work, s32, 0x7784));
            break;
        case 2:
            if (FIELD(work, s32, 0x7784) == 50) {
                ColorBuffer_BackupAndHalve(source, (void *)0x06004000, 0x4000);
            } else {
                ColorBuffer_BackupAndScaleThreeQuarters(source, (void *)0x06004000, 0x4000);
            }
            break;
        case 3:
            ColorBuffer_BackupAndDarken(source, FIELD(work, s32, 0x7784),
                (void *)0x06004000, 0x4000);
            break;
        case 4:
            ColorBuffer_BackupAndBrighten(source, FIELD(work, s32, 0x7784),
                (void *)0x06004000, 0x4000);
            break;
        }
        FIELD(work, s32, 0x7824) = 0;
        counter = (s32 *)((u8 *)work + 0x7820);
        next_counter = 1;
    } else {
        counter = (s32 *)((u8 *)work + 0x7820);
        next_counter = *counter + 1;
    }
    *counter = next_counter;
}

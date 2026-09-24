#include "TYPES.H"
#include "SCENE.H"
#include "BATTLE_EFFECT_WORK.H"

typedef s32 (*WordCopyFn)(void *destination, const void *source, s32 size);

static __inline__ void CopyWords(
    void *destination, const void *source, s32 size)
{
    ((WordCopyFn)0x03001388)(destination, source, size);
}

static __inline__ void FillWords(
    void *destination, s32 size, s32 value)
{
    ((WordCopyFn)0x03000168)(destination, (void *)size, value);
}

/* Flush the battle compositor's pending display transfer. */
void BattleFx_FlushPendingGraphicsTransfer(void)
{
    void **heap_cache;
    struct BattleEffectWork *work;
    void *source;
    s32 transfer_mode;

    heap_cache = (void **)0x03001eec;
    work = heap_cache[0];
    source = (void *)0x02010000;
    if (work->transfer_pending != 1)
        return;

    transfer_mode = work->transfer_mode;
    switch (transfer_mode) {
    case 0:
        CopyWords((void *)0x06008000, source, 0x7800);
        break;
    case 1:
        CopyWords((void *)0x06008000, source, 0x7800);
        FillWords(source, 0x7800, work->transfer_value);
        break;
    case 2:
        if (work->transfer_value == 50) {
            ColorBuffer_BackupAndHalve(source, (void *)0x06008000, 0x7800);
        } else {
            ColorBuffer_BackupAndScaleThreeQuarters(source, (void *)0x06008000, 0x7800);
        }
        break;
    case 3:
        ColorBuffer_BackupAndDarken(source, work->transfer_value,
            (void *)0x06008000, 0x7800);
        break;
    }

    work->transfer_pending = 0;
}

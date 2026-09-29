#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"
#include "BATTLE_EFFECT_WORK.H"
#include "RAM_BUFFER.H"

extern u8 gBattleFxWork[];
void ColorBuffer_BackupAndHalve(const void *source, void *destination, s32 size);
void ColorBuffer_BackupAndScaleThreeQuarters(const void *source, void *destination, s32 size);
void ColorBuffer_BackupAndDarken(const void *source, s32 mode, void *destination, s32 size);

static __inline__ void CopyWords(
    void *destination, const void *source, s32 size)
{
    Iwram_CopyWords(destination, source, size);
}

static __inline__ void FillWords(
    void *destination, s32 size, s32 value)
{
    Iwram_FillWords(destination, size, value);
}

/* Flush the battle compositor's pending display transfer. */
void BattleFx_FlushPendingGraphicsTransfer(void)
{
    void **heap_cache;
    struct BattleEffectWork *work;
    void *source;
    s32 transfer_mode;

    heap_cache = (void **)gBattleFxWork;
    work = heap_cache[0];
    source = Ram_MapCellBuffer;
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

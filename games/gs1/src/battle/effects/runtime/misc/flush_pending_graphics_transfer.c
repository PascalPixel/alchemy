#include "types.h"

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

void Func_08005534(const void *source, void *destination, s32 size);
void Func_080054e4(const void *source, void *destination, s32 size);
void Func_08005490(const void *source, s32 mode, void *destination, s32 size);

/* Flush the battle compositor's pending display transfer. */
void BattleEffect_FlushPendingGraphicsTransfer(void)
{
    void **heap_cache;
    u8 *work;
    void *source;
    s32 transfer_mode;

    heap_cache = (void **)0x03001eec;
    work = heap_cache[0];
    source = (void *)0x02010000;
    if (*(s32 *)(work + 0x7824) != 1)
        return;

    transfer_mode = *(s32 *)(work + 0x7780);
    switch (transfer_mode) {
    case 0:
        CopyWords((void *)0x06008000, source, 0x7800);
        break;
    case 1:
        CopyWords((void *)0x06008000, source, 0x7800);
        FillWords(source, 0x7800, *(s32 *)(work + 0x7784));
        break;
    case 2:
        if (*(s32 *)(work + 0x7784) == 50) {
            Func_08005534(source, (void *)0x06008000, 0x7800);
        } else {
            Func_080054e4(source, (void *)0x06008000, 0x7800);
        }
        break;
    case 3:
        Func_08005490(source, *(s32 *)(work + 0x7784),
            (void *)0x06008000, 0x7800);
        break;
    }

    *(s32 *)(work + 0x7824) = 0;
}

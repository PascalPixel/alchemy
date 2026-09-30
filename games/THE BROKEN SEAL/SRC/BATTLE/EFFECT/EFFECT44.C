#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"
#include "BATTLE_EFFECT_WORK.H"
#include "RAM_BUFFER.H"

extern u8 gWorkSlot[];
u32 Random16(void);
void WaitFrames(s32 frames);

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

/* Battle effect: wipe the 128 by 128 canvas (16 by 16 tiles of 8 by 8
   bytes) with a ragged front. Each of the 128 lanes waits a random 0..63
   steps; the front then accelerates every frame, across the canvas when
   mode is 1 and down it otherwise, writing 1 - value into every pixel it
   passes. */
void BattleEffect_WipeCanvas(s32 mode, s32 value)
{
    u8 *work;
    u8 *canvas;
    u8 delay[128];
    s32 x;
    s32 y;
    s32 pos;

    canvas = *(u8 **)(gWorkSlot + 40 * 4);
    work = *(u8 **)(gWorkSlot + 39 * 4);
    for (x = 0; x != 128; x++)
        delay[x] = Random16() & 0x3f;

    if (mode == 1) {
        s32 front;
        s32 speed;

        front = 0;
        speed = 1;
        x = 0;
        do {
            front += speed;
            speed++;
            for (; x != front; x++) {
                for (y = 0; y != 128; y++) {
                    pos = x - delay[y];
                    if (pos >= 0) if (pos <= 127)
                        canvas[(((y / 8) * 16 + pos / 8) * 8 + (y & 7)) * 8 + (pos & 7)] = 1 - value;
                }
            }
            *(s32 *)(work + 0x7824) = 1;
            WaitFrames(1);
        } while (front <= 256);
    } else {
        s32 front;
        s32 speed;

        front = 0;
        speed = 1;
        y = 0;
        do {
            front += speed / 2;
            speed += 4;
            for (; y != front; y++) {
                for (x = 0; x != 128; x++) {
                    pos = y - delay[x];
                    if (pos >= 0) if (pos <= 127)
                        canvas[(((pos / 8) * 16 + x / 8) * 8 + (pos & 7)) * 8 + (x & 7)] = 1 - value;
                }
            }
            *(s32 *)(work + 0x7824) = 1;
            WaitFrames(1);
        } while (front <= 191);
    }
}

/* Battle presentation: once per frame, flush the effect canvas to VRAM when
   an effect marked it pending, in the transfer mode the effect chose (plain
   copy, copy and clear, one of two blends, or a fade by amount); otherwise
   count the frames since the last flush. */
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

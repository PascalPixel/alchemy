#include "TYPES.H"

/* Battle effect: wipe the 128 by 128 canvas (16 by 16 tiles of 8 by 8
   bytes) with a ragged front. Each of the 128 lanes waits a random 0..63
   steps; the front then accelerates every frame, across the canvas when
   mode is 1 and down it otherwise, writing 1 - value into every pixel it
   passes. */

extern u8 gWorkSlot[];

u32 Random16(void);
void WaitFrames(s32 frames);

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

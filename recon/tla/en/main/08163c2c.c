/* Draft: BattleEffect_WipeCanvas, The Broken Seal's EFFECT44.C for ⚓️: the
 * canvas pair is heap slots 0x5c/0x60, the dirty flag sits at 0x78e8 and mode 3
 * leaves the canvas alone. Differ 11 (dd): the canvas address is built as
 * movs/add sp instead of add sp, #8 at the start, and the inner loops
 * subtract the delay one instruction earlier. Devsearch (300) reached 9.
 */
#include "TYPES.H"
#include "RAM_BUFFER.H"

u32 Random16(void);
void WaitFrames(s32 frames);

/* Battle effect: wipe the 128 by 128 canvas with a ragged front, as The
   Broken Seal's does; mode 3 leaves the canvas as it is. */
void BattleEffect_WipeCanvas(s32 mode, s32 value)
{
    u8 *work;
    u8 *canvas;
    u8 delay[128];
    s32 x;
    s32 y;
    s32 pos;

    canvas = Ram_HeapSlots->battle_fx_work[1];
    work = Ram_HeapSlots->battle_fx_work[0];
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
            *(s32 *)(work + 0x78e8) = 1;
            WaitFrames(1);
        } while (front <= 256);
    } else if (mode != 3) {
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
            *(s32 *)(work + 0x78e8) = 1;
            WaitFrames(1);
        } while (front <= 191);
    }
}

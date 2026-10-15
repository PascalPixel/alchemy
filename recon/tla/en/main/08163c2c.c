/* Draft, differ 9 of 170: The Broken Seal's BattleEffect_WipeCanvas with
 * The Lost Age's third mode (3 wipes nothing) and its transfer flag at
 * 0x78e8. Left: the native builds the delay array's address in one add
 * (add r2, sp, #8) where this loads 8 and adds sp; it stores the second
 * argument before it starts 0x03000000; and at each frame's end it reloads
 * the work before it starts the flag's offset (twice). Tried: the work as
 * a struct, a volatile work pointer (differ 110), alchemy permute for 5
 * minutes (differ 8 at best, the canvas read in a one-pass block). */
#include "types.h"
#include "ram_buffer.h"
#include "system.h"

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
                    if (pos >= 0)
                        if (pos <= 127)
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
                    if (pos >= 0)
                        if (pos <= 127)
                            canvas[(((pos / 8) * 16 + x / 8) * 8 + (pos & 7)) * 8 + (x & 7)] = 1 - value;
                }
            }
            *(s32 *)(work + 0x78e8) = 1;
            WaitFrames(1);
        } while (front <= 191);
    }
}

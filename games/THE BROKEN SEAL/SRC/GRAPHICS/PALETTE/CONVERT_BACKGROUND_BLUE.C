#include "TYPES.H"

/* Convert the tiled background through a blue palette ramp. Each screen
 * column begins after its random delay; the row budget grows each frame.
 * The second palette bank preserves the converted pixels after the sweep.
 */

typedef void *(*WordCopy)(void *, const void *, s32);
static __inline__ void CopyWords(WordCopy copy, void *destination,
                                 const void *source, s32 size)
{
    copy(destination, source, size);
}
extern u8 Data_03001e50[];
#define PIXEL_BUFFER ((u8 *)0x02010000)
void Func_080030f8(s32);
#define WaitFrames Func_080030f8
u32 Func_08004458(void);
#define Random16 Func_08004458

void Graphics_ConvertBackgroundToBlueRamp(void)
{
    s32 i, x, y, row, end, speed;
    s32 red, green, blue;
    u16 color;
    u8 *delays, *p, *pixel;
    u16 *palette;
    u32 delay_mask;
    palette = (u16 *)0x05000040;
    i = 0;
    do {
        s32 half;
        half = i / 2;
        *palette++ = (i << 10) | (half << 5) | half;
        i++;
    } while (i != 32);
    delays = *(u8 **)(Data_03001e50 + 41 * 4);
    end = 0;
    speed = 16;
    CopyWords((WordCopy)0x03001388, PIXEL_BUFFER, (void *)0x06008000, 0x7800);
    delay_mask = 63;
    p = delays;
    do {
        *p++ = Random16() & delay_mask;
    } while (p != delays + 256);
    row = 0;
    i = 0;
    do {
        end += speed / 4;
        speed++;
        while (row != end) {
            x = 0;
            do {
                y = row - delays[x];
                if (y < 0) goto next_pixel;
                if (y > 119) goto next_pixel;
                {
                    pixel = PIXEL_BUFFER + ((x & 7) + ((x / 8) << 6) + ((y & 7) << 3) + ((y / 8) << 11));
                    color = ((s16 *)0x05000000)[*pixel];
                    red = color & 31;
                    green = (color >> 5) & 31;
                    blue = (color >> 10) & 31;
                    if (red < green) red = green;
                    if (red < blue) red = blue;
                    *pixel = 63 - red;
                }
next_pixel:
                x++;
            } while (x != 256);
            row++;
        }
        ((WordCopy)0x03001388)((void *)0x06008000, PIXEL_BUFFER, 0x7800);
        WaitFrames(1);
        if (end > 248) break;
        i++;
    } while (i != 27);
    palette = (u16 *)0x050000c0;
    i = 0;
    do {
        s32 half;
        half = i / 2;
        *palette++ = (i << 10) | (half << 5) | half;
        i++;
    } while (i != 32);
    i = 0;
    do {
        PIXEL_BUFFER[i] += 64;
        i++;
    } while (i != 0x7800);
    CopyWords((WordCopy)0x03001388, (void *)0x06008000, PIXEL_BUFFER, 0x7800);
    WaitFrames(1);
}

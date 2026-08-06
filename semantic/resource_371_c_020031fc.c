#include "types.h"

extern u8 Data_0200d14c[];
extern u32 Data_03001e40;







/* Emit one randomized particle and periodically nudge the camera quadrant. */
extern u32 Func_0200739c(void);
extern u32 Func_020073a4(void);
extern u8 * Func_02007436(s32, s32, s32, s32);
extern u32 Func_020073d2(void);
extern void Func_02007462(u8 *, s32);
extern void Func_02007472(u8 *, u8 *);
extern s32 Func_020073f4(s32, s32);
extern u32 Func_0200741c(void);
extern void Func_020076a8(s32, s32, s32, s32);
extern void Func_020076b8(s32, s32, s32, s32);
void Func_020031fc(void)
{
    s32 x = 0x17b00000 + (s32)(((Func_0200739c() * 40) & 0xffff) << 16);
    s32 z = 0x0c4c0000 + (s32)(((Func_020073a4() * 30) & 0xffff) << 16);
    u8 *particle = Func_02007436(222, x, 0, z);

    if (particle != 0) {
        u8 *sprite = *(u8 **)(particle + 80);
        s32 scale = 0x13333 + (s32)((Func_020073d2() << 15) >> 16);

        particle[85] = 0;
        *(s32 *)(particle + 24) = scale;
        sprite[38] = 0;
        *(s32 *)(particle + 28) = scale;
        sprite[9] = (u8)((sprite[9] & ~12) | 8);
        Func_02007462(particle, 1);
        Func_02007472(particle, Data_0200d14c);
    }

    if (Func_020073f4(Data_03001e40, 3) == 0) {
        u32 quadrant = (Func_0200741c() << 2) >> 16;

        if (quadrant <= 2) {
            s32 cameraX = quadrant == 0 ? 0x17c70000 : 0x17c90000;
            s32 cameraZ = quadrant == 1 ? 0x0c670000 : 0x0c690000;

            Func_020076a8(cameraX, -1, cameraZ, 1);
        } else if (quadrant == 3) {
        } else if (quadrant == 3) {
            Func_020076b8(0x17c70000, -1, 0x0c670000, 1);
        }
    }
}

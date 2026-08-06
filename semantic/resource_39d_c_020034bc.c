#include "types.h"

extern u8 *Data_03001e70;
extern u32 Data_03001e40;









/* Place actor 8 and periodically attach a randomized orbiting effect. */
extern u8 * Func_02006bba(s32 actorId);
extern u32 Func_02006b14(void);
extern void Func_02006c2c(s32 actorId, s32 x, s32 z);
extern u8 * Func_02006bfa(s32 actorId);
extern void Func_02006c46(s32 actorId, s32 x, s32 z);
extern u8 * Func_02006c14(s32 actorId);
extern u8 * Func_02006c1e(s32 actorId);
extern void Func_02006c74(s32 actorId, s32 x, s32 z);
extern u8 * Func_02006be8(s32 kind, s32 x, s32 y, s32 z);
extern s32 Func_02006bac(s32 value, s32 divisor);
extern void Func_02006c02(u8 *object, u32 script);
extern void Func_02006d02(u8 *object, s32 mode);
extern u32 Func_02006bec(void);
extern s32 Func_02006c1c(s32 angle);
void Func_020034bc(void)
{
    u8 *actor = Func_02006bba(8);
    u8 *placed;
    u8 *object;
    u8 *owner;
    u32 randomOffset = (Func_02006b14() * 48u) & 0xffff0000u;
    u32 phase;
    s32 velocity;

    if (*(s16 *)(Data_03001e70 + 234) <= 129) {
        if ((Data_03001e40 & 1) != 0) {
            Func_02006c2c(8, 152 << 17, 144 << 16);
            placed = Func_02006bfa(8);
            velocity = 0x10000;
        } else {
            Func_02006c46(8, 152 << 17, 151 << 16);
            placed = Func_02006c14(8);
            velocity = 0x14ccc;
        }
        *(s32 *)(placed + 24) = velocity;
        placed = Func_02006c1e(8);
        *(s32 *)(placed + 28) = velocity;
    } else {
        Func_02006c74(8, 128 << 12, 128 << 12);
    }

    if (actor == 0 || (Data_03001e40 & 15) != 0)
        return;

    object = Func_02006be8(284,
                           *(s32 *)(actor + 8) + (128 << 12),
                           *(s32 *)(actor + 12) + (s32)randomOffset + (128 << 12),
                           *(s32 *)(actor + 16));
    phase = (u32)Func_02006bac((s32)randomOffset, 192 << 11) << 16;
    if (object == 0)
        return;

    Func_02006c02(object, 0x0200bc54);
    Func_02006d02(object, 3);
    owner = *(u8 **)(object + 80);
    object[85] = 0;
    *(u16 *)(object + 102) = 0;
    *(u8 **)(object + 104) = actor;
    *(u32 *)(object + 108) = 0x0200b461;
    *(u16 *)(object + 100) = Func_02006bec() & 0x0ffff000u;
    {
        s32 wave = Func_02006c1c((phase & 0x000fffff) >> 4);
        *(s32 *)(object + 48) = ((s32)((u32)wave * 24u)) >> 16;
    }
    owner[38] = 0;
    owner[9] = (owner[9] & (u8)~12) | (*(u8 *)(*(u8 **)(actor + 80) + 9) & 12);
}

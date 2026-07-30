#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void *Func_08092054(void *);
u32 Func_08004458(void);
void Func_0800447c(s32, s32, s32 *);
void Func_080974d8(s32 *);
s8 Func_0809ba34(void *);
void Func_0809bb34(void *);
void Func_080f9010(s32);

/*
 * Advance the rising projectile through launch, one of two travel passes,
 * and teardown. The first pass can reverse to stage zero; the second advances
 * to cleanup after assigning its next randomized arc.
 */
void Func_0809641c(void *effect)
{
    u8 *owner = *(u8 **)(0x02000240 + 500);
    u8 *controller = Func_08092054(owner);
    s8 stage = M2C_FIELD(effect, s8 *, 0x40);
    s32 vector[3];

    if (stage == 0) {
        s32 magnitude;

        vector[0] = M2C_FIELD(controller, s32 *, 8);
        vector[1] = M2C_FIELD(controller, s32 *, 0xC) +
            Func_08004458() * 5 + 0xF0000;
        vector[2] = M2C_FIELD(controller, s32 *, 0x10);
        Func_080974d8(vector);
        magnitude = Func_08004458() * 6 + 0x20000;
        Func_0800447c(magnitude, Func_08004458(), vector);

        M2C_FIELD(effect, s32 *, 0xC) = vector[0];
        M2C_FIELD(effect, s32 *, 0x10) = vector[2];
        M2C_FIELD(effect, s32 *, 4) = vector[0];
        M2C_FIELD(effect, s32 *, 8) = vector[2] - 0x640000;
        M2C_FIELD(effect, s32 *, 0x24) = 0x30000;
        M2C_FIELD(effect, s32 *, 0x20) =
            Func_08004458() * 3 + 0x30000;
        M2C_FIELD(effect, s32 *, 0x28) = 0x10000;
        M2C_FIELD(effect, s32 *, 0x2C) = 0x10000;
        M2C_FIELD(effect, u8 *, 0x42) = 0;
        M2C_FIELD(effect, u8 *, 0x41) = 1;
        M2C_FIELD(effect, u8 *, 0x40)++;
    } else if (stage == 1 || stage == 2) {
        if (Func_0809ba34(effect) == 0) {
            vector[0] = M2C_FIELD(effect, s32 *, 4);
            vector[2] = M2C_FIELD(effect, s32 *, 8);
            Func_0800447c(0xC0000, Func_08004458(), vector);
            M2C_FIELD(effect, s32 *, 0xC) = vector[0];
            M2C_FIELD(effect, s32 *, 0x10) = vector[2];
            M2C_FIELD(effect, u8 *, 0x41) = 0;
            M2C_FIELD(effect, s32 *, 0x1C) = 0x10000;
            M2C_FIELD(effect, s32 *, 0x24) = 0;
            M2C_FIELD(effect, s32 *, 0x20) =
                Func_08004458() + 0x23333;
            M2C_FIELD(effect, s32 *, 0x28) = 0x8000;
            M2C_FIELD(effect, s32 *, 0x2C) = 0x8000;
            Func_080f9010(0x8F);
            if (stage == 1) {
                M2C_FIELD(effect, u8 *, 0x40)--;
            } else {
                M2C_FIELD(effect, u8 *, 0x40)++;
            }
            M2C_FIELD(effect, u16 *, 0x3A) = 6;
        }
    } else if (stage == 3) {
        Func_0809bb34(effect);
    }
}

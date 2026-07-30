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
 * Advance one orbiting-object effect through launch, travel, return setup,
 * return travel, and teardown. Position and velocity vectors are explicit
 * three-word stack objects rather than decompiler frame aliases.
 */
void Func_08095c08(void *effect)
{
    u8 *object = M2C_FIELD(effect, u8 **, 0);
    u8 *owner = *(u8 **)(0x02000240 + 500);
    u8 *controller = Func_08092054(owner);
    s8 stage = M2C_FIELD(effect, s8 *, 0x40);
    s32 vector[3];

    switch (stage) {
    case 0:
    {
        u32 first;
        u32 second;

        vector[0] = M2C_FIELD(effect, s32 *, 0x14);
        vector[2] = M2C_FIELD(effect, s32 *, 0x18);
        M2C_FIELD(effect, s32 *, 4) = vector[0];
        M2C_FIELD(effect, s32 *, 8) = vector[2];
        first = Func_08004458();
        second = Func_08004458();
        Func_0800447c(
            0x780000,
            ((first * 0x1800) >> 16) -
            ((second * 0x1800) >> 16) + 0xC000,
            vector);
        M2C_FIELD(effect, s32 *, 0xC) = vector[0];
        M2C_FIELD(effect, s32 *, 0x10) = vector[2];
        M2C_FIELD(effect, s32 *, 0x24) = 0x50000;
        M2C_FIELD(effect, s32 *, 0x20) = 0x50000;
        M2C_FIELD(effect, u8 *, 0x42) = 0;
        M2C_FIELD(effect, u8 *, 0x40)++;
        object[9] =
            (object[9] & ~0xC) |
            (M2C_FIELD(controller, u8 **, 0x50)[9] & 0xC);
        M2C_FIELD(effect, u8 *, 0x47) = 0;
        M2C_FIELD(effect, s16 *, 0x38) = 0;
        if ((*(u32 *)0x03001800 & 1) != 0) {
            Func_080f9010(0x86);
        }
        break;
    }

    case 1:
        if (M2C_FIELD(effect, s16 *, 0x38) == 3) {
            object[9] &= ~0xC;
            M2C_FIELD(effect, u8 *, 0x47) = 4;
        }
        /* Fall through to the shared backward-travel update. */

    case 4:
        if (Func_0809ba34(effect) == 0) {
            M2C_FIELD(effect, u8 *, 0x40)--;
        }
        break;

    case 2:
        if (Func_0809ba34(effect) == 0) {
            M2C_FIELD(effect, s32 *, 0x14) =
                M2C_FIELD(effect, s32 *, 4);
            M2C_FIELD(effect, s32 *, 0x18) =
                M2C_FIELD(effect, s32 *, 8);
            object[9] &= ~0xC;
            M2C_FIELD(effect, u8 *, 0x47) = 4;
            M2C_FIELD(effect, u8 *, 0x44) = 0;
            M2C_FIELD(effect, u8 *, 0x40)++;
            M2C_FIELD(effect, s16 *, 0x3A) = 0x28;
        }
        break;

    case 3:
        M2C_FIELD(effect, u8 *, 0x44) = 1;
        M2C_FIELD(effect, s32 *, 4) =
            M2C_FIELD(effect, s32 *, 0x14);
        M2C_FIELD(effect, s32 *, 8) =
            M2C_FIELD(effect, s32 *, 0x18);
        vector[0] = M2C_FIELD(controller, s32 *, 8);
        vector[1] = M2C_FIELD(controller, s32 *, 0xC) + 0x140000;
        vector[2] = M2C_FIELD(controller, s32 *, 0x10);
        Func_080974d8(vector);
        Func_0800447c(0x40000, Func_08004458(), vector);
        M2C_FIELD(effect, s32 *, 0xC) = vector[0];
        M2C_FIELD(effect, s32 *, 0x10) = vector[2];
        M2C_FIELD(effect, u8 *, 0x40)++;
        if ((*(u32 *)0x03001800 & 1) != 0) {
            Func_080f9010(0x91);
        }
        break;

    case 5:
        if (Func_0809ba34(effect) == 0) {
            Func_0809bb34(effect);
        }
        break;
    }
}

#include "types.h"

#define OverlayObject_SpawnConfiguredObject Func_020009f4
#define OverlayObject_PrepareObjectWithCommand15 Func_02000a4c
#define OverlayObject_IntegrateVelocities Func_02000ab0
#define OverlayObject_WaitUntilIdle Func_02000cd0
u8 *Func_020037ae(s32 kind, s32 x, s32 y, s32 z);
void Func_0200381c(u8 *object, s32 mode);
void Func_02003904(u8 *object, s32 mode);
void Func_02003834(u8 *object, s32 mode);
u8 *Func_02003806(s32 kind, s32 x, s32 y, s32 z);
void Func_02003878(u8 *object, s32 mode);
void Func_02003960(u8 *object, s32 mode);
void Func_02003a5e();

/*
 * Spawns an overlay object and puts it into its configured state --
 * resource_3c5.
 */

/*
 * The four callees are declared at the in-overlay entry points that are
 * called directly, not at their veneer addresses.
 */

/*
 * Spawn an object, clear the low bits of its owner record, and issue command
 * 15.  The three callees are declared at their in-overlay entry points, not at
 * veneers.  Each read-modify-write takes its mask from a local that is
 * initialised first and ANDed with the loaded byte afterwards, which is what
 * puts the mask in r3 and the byte in r2.  The +0x23 mask stays byte-wide as
 * 0xfe rather than ~1, which would widen it to a negated 2.
 */

u8 *OverlayObject_SpawnConfiguredObject(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *result;
    u8 *object = Func_020037ae(kind, x, y, z);

    if (object != 0) {
        u8 *owner = *(u8 **)(object + 80);
        /*
         * The mask is a local initialised first and ANDed with the loaded
         * byte afterwards; folding the clear into one expression swaps the
         * two registers.
         */
        u32 masked = ~12;
        masked = masked & owner[9];
        owner[9] = (u8)masked;
        object[85] = 0;
        object[89] = 8;
        Func_0200381c(object, 0);
        Func_02003904(object, 14);
        Func_02003834(object, 1);
        result = object;
    } else {
        result = 0;
    }
    return result;
}

u8 *OverlayObject_PrepareObjectWithCommand15(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *result;
    u8 *object = Func_02003806(kind, x, y, z);

    if (object != 0) {
        u8 *owner = *(u8 **)(object + 80);
        u32 mask = ~12;
        u32 low;
        mask = mask & owner[9];
        owner[9] = (u8)(mask | 4);
        object[85] = 0;
        object[89] = 8;
        Func_02003878(object, 0);
        Func_02003960(object, 15);
        low = 0xfe;
        low = low & object[35];
        object[35] = (u8)(low | 2);
        result = object;
    } else {
        result = 0;
    }
    return result;
}

void OverlayObject_IntegrateVelocities(void *arg0)
{
    u8 *a = arg0;

    *(volatile s32 *)(a + 0x08) += *(s32 *)(a + 0x44);
    *(volatile s32 *)(a + 0x0C) += *(s32 *)(a + 0x48);
    *(volatile s32 *)(a + 0x10) += *(s32 *)(a + 0x4C);
    *(volatile s32 *)(a + 0x18) += *(s32 *)(a + 0x30);
    *(volatile s32 *)(a + 0x1C) += *(s32 *)(a + 0x34);
    *(volatile u16 *)(*(u8 **)(a + 0x50) + 0x1E) += *(u16 *)(a + 0x64);
}

void OverlayObject_WaitUntilIdle(s32 *obj)
{
    s32 i = 60;

    while (i != 0) {
        Func_02003a5e(1);
        i--;
        if (obj[10] == 0) {
            break;
        }
    }
}

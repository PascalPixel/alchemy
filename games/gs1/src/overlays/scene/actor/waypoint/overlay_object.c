#include "types.h"

#define OverlayObject_PrepareSpawnedObject Func_020009f4
#define OverlayObject_SpawnConfiguredWithMode15 Func_02000a4c
#define OverlayObject_IntegrateVelocities Func_02000ab0
#define OverlayObject_ApplyField100 Func_02000d6c
#define OverlayObject_ApplyZero Func_02000d80
u8 *Func_0200373a(s32 kind, s32 x, s32 y, s32 z);
void Func_02003788(u8 *obj, s32 mode);
void Func_02003860(u8 *obj, s32 mode);
void Func_020037a0(u8 *obj, s32 mode);
u8 *Func_02003792(s32 kind, s32 x, s32 y, s32 z);
void Func_020037e4(u8 *obj, s32 mode);
void Func_020038bc(u8 *obj, s32 mode);
void Func_02003ba4(s32 a, s32 b);
void Func_02003ae2(s32 a, s32 b);

/*
 * Spawns an overlay object and puts it into its configured state --
 * resource_39f.
 */

/*
 * The four callees are declared at the in-overlay entry points that are
 * called directly, not at their veneer addresses.
 */

/*
 * Spawn an object, clear the low bits of its owner record, and leave it in
 * mode 15.  The three callees are declared at their in-overlay entry points,
 * not at veneers.  Each read-modify-write takes its mask from a local that is
 * initialised first and ANDed with the loaded byte afterwards, which is what
 * puts the mask in r3 and the byte in r2.  The +0x23 mask stays byte-wide as
 * 0xfe rather than ~1, which would widen it to a negated 2.
 */

u8 *OverlayObject_PrepareSpawnedObject(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret;
    u8 *obj = Func_0200373a(kind, x, y, z);

    if (obj != 0) {
        u8 *rec = *(u8 **)(obj + 80);
        /*
         * The mask is a local initialised first and ANDed with the loaded
         * byte afterwards; folding the clear into one expression swaps the
         * two registers.
         */
        u32 mask = ~12;
        mask = mask & rec[9];
        rec[9] = (u8)mask;
        obj[85] = 0;
        obj[89] = 8;
        Func_02003788(obj, 0);
        Func_02003860(obj, 14);
        Func_020037a0(obj, 1);
        ret = obj;
    } else {
        ret = 0;
    }
    return ret;
}

u8 *OverlayObject_SpawnConfiguredWithMode15(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret;
    u8 *obj = Func_02003792(kind, x, y, z);

    if (obj != 0) {
        u8 *rec = *(u8 **)(obj + 80);
        u32 mask = ~12;
        u32 m;
        mask = mask & rec[9];
        rec[9] = (u8)(mask | 4);
        obj[85] = 0;
        obj[89] = 8;
        Func_020037e4(obj, 0);
        Func_020038bc(obj, 15);
        m = 0xfe;
        m = m & obj[35];
        obj[35] = (u8)(m | 2);
        ret = obj;
    } else {
        ret = 0;
    }
    return ret;
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

s32 OverlayObject_ApplyField100(s32 a)
{
    Func_02003ba4(a, *(s16 *)(a + 100));
    return 0;
}

s32 OverlayObject_ApplyZero(s32 a)
{
    Func_02003ae2(a, 0);
    return 0;
}

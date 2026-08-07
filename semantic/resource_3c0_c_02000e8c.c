#include "types.h"

/*
 * resource_3c0 owner at 0x02000e8c, 160 bytes: THE OVERLAY'S ENTRY
 * DRIVER -- the target of the header veneer at image offset 4
 * (bun tools/overlay/overlay_driver.ts resource_3c0), so the loader enters
 * here and no bl inside the image reaches it.
 *
 * Body: take the camera/scene pointer at 0x03001e70 and write 513 to
 * the workspace's +448 s32.  If flag 0x210 is set, mark the
 * 0x02000240 global's +498 byte with 2 and install callback
 * 0x02008401 with a 3200-tick argument.  Then read the scene id at
 * 0x02000240+448: for 164 or 165 it saves palette entry 0x0500019e
 * into the image cell at 0x02009a00 and runs Func_02000d24.  It then
 * RE-READS the same scene id -- Func_02000d24 can move it, so the
 * second read is meaningful and is kept -- and dispatches: 164 to
 * Func_02000f50, 165 to Func_02001094, anything else to
 * Func_080f9010(0x120).  Finally, if the sub-selector at
 * 0x02000240+450 is zero, clear bit 9 of the camera record's +20 u16.
 * Returns 0.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02000e8c through
 * `movs r0, #0 / pop {r5, r6, r7} / pop {r1} / bx r1` at
 * 0x02000f24-0x02000f2a, alignment halfword, then the nine-word
 * literal pool 0x02000f2c-0x02000f4f.  Returns a literal 0.
 *
 * The scene id and sub-selector are the +448 and +450 s16 of the same
 * 0x02000240 global that resource_38a, resource_39e, resource_3a4 and
 * resource_3a6 read, and the workspace +448 write is the recurring
 * idiom (513 here).  The pool word 0x03001e70 is the base that
 * Data_03001ebc itself lives 0x4c into: the reference loads
 * *(u32 *)(0x03001e70 + 0x4c), which IS Data_03001ebc, so that store
 * is written through the named pointer rather than the raw base.
 *
 * Cross-checked against m2c per the 2026-08-01 ruling (m2c is a
 * cross-check, never the source): tools/m2c_guard.ts clears this row
 * of the call-through-register hazard, and m2c agreed on every value
 * -- including the 0x03001e70+0x4c indirection, the 0xfdff mask and
 * 0x02008401.  Its rendering folded the globals into absolute
 * addresses (0x02000400, 0x02000402, 0x02000432); the named-offset
 * form is kept here so the row stays readable against its siblings.
 *
 * Uncertainty: flag id 0x210, scene ids 164/165 and the callback
 * constant are literal; the camera record's +20 bits and the
 * 0x02000240 +498 byte are recorded by offset, roles open.
 */

extern s16 Data_02000240[];
extern u8 *Data_03001e70;
extern u8 *Data_03001ebc;

extern void Func_02000d24(void);
extern void Func_02000f50(void);
extern void Func_02001094(void);

extern void Func_080000d0(s32 callback, s32 arg1);
extern s32 Func_080770e0(s32 flagId);
extern void Func_080f9010(s32 arg0);

s32 Func_02000e8c(void)
{
    u8 *camera;

    camera = Data_03001e70;
    *(s32 *)(Data_03001ebc + 448) = 513;
    if (Func_080770e0(0x210) != 0) {
        ((s8 *)Data_02000240)[498] = 2;
        Func_080000d0(0x02008401, 3200);
    }
    if (Data_02000240[224] == 164
        || Data_02000240[224] == 165) {
        *(u16 *)0x02009a00 = *(u16 *)0x0500019e;
        Func_02000d24();
    }
    if (Data_02000240[224] == 164) {
        Func_02000f50();
    } else if (Data_02000240[224] == 165) {
        Func_02001094();
    } else {
        Func_080f9010(0x120);
    }
    if (Data_02000240[225] == 0) {
        *(u16 *)(camera + 20) &= 0xfdff;
    }
    return 0;
}

#include "types.h"

/*
 * Colosso tournament selection and progression handler.
 *
 * This draft was reconstructed mechanically from the owner's complete
 * reference assembly, including its jump table and literal pools.  Most
 * locals are still m2c's raw sp/temp/reg names; renaming the rest needs the
 * same per-site tracing this file has already had for a growing set:
 *
 *   - ctx: the save/session pointer loaded from 0x03001F2C once at entry
 *     and reused for every M2C_FIELD access below.
 *   - events: ctx's substructure at +0x184. +0x2128 holds the current event
 *     code (0 means none pending, dispatched through the 28-way switch
 *     below via eventCode/switchIndex once it's nonzero); +0x212C is a
 *     counter incremented per handled event; +0xA0 is a per-slot enable
 *     byte array indexed the same way as status[].
 *   - statusPtr: a write cursor over the local status[8] array (one byte
 *     per tournament slot), cleared to 0 in the entry loop.
 *   - slotFlags: an 8-byte per-slot flag buffer filled by Func_080ae714
 *     from ctx's +0x1C count field; checked per slot as a boolean.
 *   - mode: arg0, compared against 0 and 1 to select which per-slot status
 *     pass and layout call run; a third mode is handled by the fallback
 *     path further down.
 *   - windowPtr: ctx's field at +0x10C, always used through one
 *     dereference (*windowPtr) as a window-handle argument to the
 *     Func_08015xxx layout calls; its address is computed lazily at first
 *     use, matching the reference (see 21dc7e844).
 *   - packedPos: a per-mode u16 field at ctx + mode*2 + 0x174, decomposed
 *     via Func_08002304/Func_080022f4(packedPos, 10) into sp38 (remainder)
 *     and sp30-then-temp_r0_47 (quotient); the function's tail recombines
 *     them as sp38 + sp30*0xA and writes the result back to the same
 *     field, confirming the round*10+slot packing.
 *   - result: the function's return value, set to a small status code
 *     (1/2/3/4/7/0xA/or 0-var_r5_1785) on each exit path.
 *   - statusFlags: *(s32*)0x03001B04, read once at entry and tested
 *     bit-by-bit (0x10/0x20/0x40/0x80) later to pick a message ID.
 *
 * differing_halfwords=2364 at 4756/4888 bytes; still a genuine mismatch in
 * structure, not just naming, so most of the remaining locals are still
 * provisional m2c names (sp/temp/reg/var_rN). Two hand attempts this
 * session to nudge register pressure toward the reference's 108-byte frame
 * (hoisting/merging a shared subexpression) both measured worse and were
 * reverted; an automated permuter search did close half that gap by
 * splitting expressions into extra locals, but at the cost of ~150
 * unnamed, ungrounded new_varN locals that can't be humanized honestly, so
 * that state was reverted (39ad6a9a5) in favor of this hand-understood one.
 * Keep every edit tied to local evidence and score it through gs1cc.
 */
s32 Func_080ab5e4(s32 arg0)
{
    s8 status[8];
    s32 *windowPtr;
    int new_var45;
    s32 sp0;
    s32 new_var5;
    void *new_var66;
    s32 *new_var91;
    s32 sp4;
    u32 sp8;
    s8 *new_var22;
    u32 *new_var86;
    s32 sp14;
    s32 sp18;
    s8 *statusPtr;
    s32 sp44;
    int new_var17;
    s32 sp20;
    s32 *new_var12;
    u32 new_var54;
    u32 sp28;
    void *new_var73;
    s32 sp38;
    int new_var36;
    s32 sp3C;
    s32 sp2C;
    s32 sp34;
    u32 new_var81;
    s32 sp30;
    s32 sp40;
    s32 sp48;
    s32 new_var80;
    void *ctx;
    s32 mode;
    u8 slotFlags[8];
    s8 *new_var48;
    s8 *new_var75;
    s8 *new_var6;
    s32 *var_r2_1122;
    s32 temp_r0_119;
    s8 *new_var37;
    int new_var68;
    s32 temp_r0_2083;
    s8 new_var47;
    s32 temp_r0_2233;
    s32 temp_r0_2301;
    s32 temp_r0_2363;
    s32 temp_r1_126;
    s32 temp_r1_1953;
    s32 *new_var32;
    u32 new_var30;
    s32 temp_r1_1966;
    s8 **new_var31;
    s32 temp_r1_2240;
    s32 temp_r1_2308;
    int new_var16;
    s32 sp24;
    int new_var2;
    s32 sp10;
    u32 new_var27;
    s32 eventCode;
    s32 temp_r2_25;
    short new_var52;
    int new_var60;
    s32 *new_var76;
    s32 temp_r3_1572;
    s8 *new_var28;
    int new_var55;
    int new_var69;
    int new_var74;
    s32 temp_r3_1597;
    s32 new_var49;
    s32 *new_var53;
    s32 temp_r3_1908;
    s32 temp_r3_1936;
    s32 temp_r3_2211;
    s32 temp_r3_2279;
    s32 temp_r3_88;
    s32 temp_r5_1001;
    s32 temp_r5_1029;
    u32 new_var21;
    int new_var67;
    int new_var15;
    s32 temp_r5_1083;
    s32 temp_r5_1163;
    s32 new_var3;
    s32 temp_r5_1278;
    s32 temp_r5_1327;
    s8 *new_var56;
    int new_var58;
    s32 temp_r5_1374;
    s8 **new_var24;
    s32 temp_r5_1702;
    s32 temp_r5_1817;
    s32 temp_r5_1872;
    u32 new_var33;
    s32 temp_r5_95;
    int new_var39;
    int new_var25;
    s32 temp_r6_1509;
    s32 temp_r6_216;
    s32 temp_r6_339;
    s32 temp_r6_601;
    s32 temp_r7_312;
    s32 temp_r7_514;
    s32 temp_r7_723;
    s32 statusFlags;
    u32 *new_var8;
    s32 var_r0_255;
    int new_var88;
    s32 var_r3_1124;
    s32 new_var9;
    int new_var82;
    s32 result;
    int new_var77;
    int new_var89;
    s32 var_r5_1785;
    s32 var_r5_237;
    s32 var_r6_240;
    int new_var71;
    s32 var_r8_1869;
    s8 *var_r0_148;
    s8 *var_r1_151;
    int new_var29;
    void *new_var79;
    s8 *new_var19;
    u32 new_var63;
    s8 *new_var42;
    s8 *var_r2_152;
    s8 *var_r3_61;
    s8 var_r1_2077;
    int new_var38;
    s8 var_r1_2127;
    s8 *var_r1_82;
    s8 var_r1_2319;
    u16 *var_sl_1853;
    u16 temp_r0_47;
    u16 temp_r2_218;
    s8 *new_var7;
    u16 temp_r2_2357;
    u16 temp_r2_424;
    int new_var51;
    int new_var34;
    u16 temp_r5_204;
    u32 new_var40;
    s32 *new_var11;
    u16 packedPos;
    s32 new_var;
    u16 temp_r6_199;
    int new_var50;
    u16 temp_r7_1856;
    u32 switchIndex;
    u32 temp_r4_1863;
    u32 temp_r5_1508;
    u32 temp_r5_1573;
    int new_var65;
    u32 temp_r5_1598;
    u32 temp_r5_1909;
    s32 *new_var59;
    int new_var44;
    u32 temp_r5_1937;
    u32 temp_r5_515;
    u32 temp_r5_596;
    u32 temp_r5_724;
    u32 temp_r6_1574;
    s32 *new_var20;
    s8 *new_var10;
    u32 temp_r6_1599;
    u32 temp_r6_1701;
    int new_var62;
    u32 temp_r6_1871;
    u32 temp_r6_1910;
    u32 temp_r6_1938;
    u32 new_var83;
    u32 temp_r6_509;
    u32 temp_r6_719;
    u32 temp_r7_1504;
    u32 temp_r7_1696;
    int new_var64;
    s32 new_var70;
    u32 temp_r7_600;
    u32 var_r3_581;
    u32 var_r4_1290;
    s8 *new_var90;
    u32 var_r4_1337;
    int new_var4;
    int new_var87;
    u32 *new_var61;
    int new_var43;
    u32 var_r4_1384;
    u32 var_r4_1515;
    u32 var_r4_606;
    int new_var46;
    u32 var_r4_907;
    s32 *new_var13;
    s32 *new_var14;
    s8 *new_var57;
    u8 *var_r2_85;
    u8 temp_r2_2035;
    u8 temp_r2_2137;
    int new_var72;
    u32 new_var18;
    u8 temp_r2_2165;
    void *temp_r3_18;
    int new_var23;
    int new_var26;
    int new_var35;
    void *events;
    u32 *new_var41;
    void *temp_r5_343;
    s8 *new_var84;
    s8 *new_var85;
    s32 *new_var78;
    mode = arg0;
    temp_r3_18 = (new_var66 = (*((void **)0x03001F2C)));
    ctx = temp_r3_18;
    events = (*((void **)(((s8 *)temp_r3_18) + 0x184)));
    sp48 = 1;
    temp_r2_25 = 2;
    temp_r2_25 = (mode * temp_r2_25);
    sp34 = temp_r2_25;
    new_var70 = temp_r2_25;
    new_var2 = 0x174;
    packedPos = (*((u16 *)(((s8 *)ctx) + (new_var70 + new_var2))));
    sp38 = ((u16)Func_08002304(packedPos, 0xA));
    temp_r0_47 = Func_080022f4(packedPos, 0xA);
    statusPtr = (new_var85 = (new_var56 = status));
    sp30 = ((s32)temp_r0_47);
    sp2C = 0;
    sp28 = 0;
    sp24 = 0;
    sp20 = new_var89;
    sp20 = (sp20 = 0);
    sp3C = ((-1) & 0xFFFF);
    var_r3_61 = (&status[7]);
    do
    {
        (*var_r3_61) = 0;
        var_r3_61 -= 1;
    }
    while (((s32)var_r3_61) >= ((s32)statusPtr));
    if (mode == 0)
    {
        Func_080aafb8(events);
        sp44 = 0;
        if (mode < ((s32)(*((u8 *)(((s8 *)ctx) + 0x219)))))
        {
            var_r1_82 = statusPtr;
            do
            {
                var_r2_85 = (events + 0xA0);
                do
                {
                    temp_r3_88 = (*var_r2_85);
                    temp_r3_88 <<= 0x18;
                    var_r2_85 += 1;
                    if (temp_r3_88 == 0)
                    {
                        (*var_r1_82) = 4;
                    }
                    temp_r5_95 = (sp44 + 1);
                    sp44 = temp_r5_95;
                    var_r1_82 += 1;
                }
                while (temp_r5_95 < ((s32)(*((u8 *)(((s8 *)ctx) + 0x219)))));
            }
            while (0);
        }
        sp2C = sp30;
        sp44 = 0;
        if (((s32)(*((u8 *)(((s8 *)ctx) + 0x219)))) > 0)
        {
            do
            {
                do
                {
                    if (statusPtr[sp38] == 4)
                    {
                        temp_r0_119 = 1;
                        temp_r0_119 = (sp38 + temp_r0_119);
                        sp38 = temp_r0_119;
                        sp38 = Func_080aa538(temp_r0_119, *((u8 *)(((s8 *)ctx) + 0x219)));
                    }
                    temp_r1_126 = (sp44 + 1);
                    sp44 = temp_r1_126;
                }
                while (temp_r1_126 < ((s32)(*((u8 *)(((s8 *)ctx) + 0x219)))));
            }
            while (0);
        }
    }
    else
    {
        Func_080ae714(slotFlags, new_var47 = (*((s8 *)(((s8 *)ctx) + 0x1C))));
        do
        {
        }
        while (0);
        sp44 = 0;
        if (((s32)(*((u8 *)(((s8 *)ctx) + 0x219)))) > 0)
        {
            var_r0_148 = statusPtr;
            var_r1_151 = (0xA0 + events);
            var_r2_152 = var_r0_148;
            do
            {
                if (sp44 == (*((s8 *)(((s8 *)ctx) + 0x1C))))
                {
                    (*var_r2_152) = 7;
                }
                else
                {
                    if (slotFlags[(short)sp44] != 0)
                    {
                        (*var_r2_152) = 0;
                    }
                    else
                    {
                        (*var_r2_152) = 3;
                        if ((*var_r1_151) == 0)
                        {
                            (*(*(new_var31 = (&var_r0_148)))) = 7;
                        }
                    }
                }
                sp44 = (sp44 + 1);
                var_r0_148 += 1;
                var_r1_151 += 1;
                var_r2_152 += 1;
            }
            while (sp44 < ((s32)(*((u8 *)(((s8 *)ctx) + 0x219)))));
        }
    }
    if (mode == 1)
    {
        temp_r6_199 = (*((u16 *)(((s8 *)ctx) + 0x174)));
        temp_r5_204 = Func_08002304(temp_r6_199, 0xA);
        ;
        temp_r2_218 = Func_080022f4(temp_r6_199, 0xA);
        sp0 = mode;
        sp4 = 0xE;
        Func_080ab1f4(*((s32 *)(((s8 *)ctx) + 0x30)), (temp_r5_204 * 7) + 1, temp_r2_218 + 2, (double)6);
        do
        {
        }
        while (0);
        sp0 = 7;
        sp4 = 6;
        Func_080ab2ec(new_var52 = ((char)((short)(*((s32 *)(new_var6 = (((s8 *)(new_var73 = ctx)) + 0x30)))))), (char)((temp_r5_204 * 7) + 1), 2);
        var_r5_237 = 0;
        if (((s32)(*((u8 *)(((s8 *)ctx) + 0x219)))) > 0)
        {
            var_r6_240 = 8;
            do
            {
                if (var_r5_237 == (*((s8 *)(((s8 *)ctx) + 0x1C))))
                {
                    if ((*((u16 *)(((s8 *)ctx) + 0x178))) & 0x8000)
                    {
                        var_r0_255 = 0xBB0;
                    }
                    else
                    {
                        var_r0_255 = 0xBAF;
                    }
                }
                else
                {
                    if (2 & ((u8)statusPtr[var_r5_237]))
                    {
                        var_r0_255 = 0xBAE;
                    }
                    else
                    {
                        var_r0_255 = 0xBB1;
                    }
                }
                Func_08015080(var_r0_255, *((s32 *)(((s8 *)ctx) + 0x30)), var_r6_240, 8);
                var_r5_237 += 1;
                var_r6_240 += 0x38;
            }
            while (var_r5_237 < ((s32)(*((u8 *)(((s8 *)ctx) + 0x219)))));
        }
    }
    windowPtr = ((s32 *)(((s8 *)temp_r3_18) + 0x10C));
    Func_08015270(*windowPtr);
    (*((s8 *)(((s8 *)(*((void **)(((s8 *)ctx) + 0x14)))) + 5))) = 1;
    sp18 = ((*(new_var53 = (&sp38))) * 8);
    loop_36:
        temp_r7_312 = sp48;
    if (temp_r7_312 == 0)
    {
    }
    else
    {
        sp48 = 0;
        sp3C = (-1);
        if (!(1 & ((u8)statusPtr[sp38])))
        {
            sp3C = sp30;
        }
        temp_r6_339 = (*((s32 *)(((s8 *)ctx) + 0x10)));
        new_var34 = 0x208;
        temp_r5_343 = Func_08077008(*((u16 *)((new_var84 = ((s8 *)ctx)) + ((sp38 * 2) + new_var34))));
        Func_08015270(temp_r6_339);
        do
        {
            Func_08015090(temp_r5_343, temp_r6_339, 0, 0);
            new_var15 = 0x129;
            if (1)
            {
                do
                {
                    do
                    {
                        Func_08015080((*((u8 *)(((s8 *)temp_r5_343) + new_var15))) + 0x741, temp_r6_339, 0, 8);
                    }
                    while (0);
                    Func_08015090((void *)0x080AF28C, temp_r6_339, 0x30, 0);
                    sp0 = sp48;
                }
                while (0);
                Func_080150b0(*((u8 *)(((s8 *)temp_r5_343) + 0xF)), 2, temp_r6_339, 0x48);
            }
            if (mode == 0)
            {
                Func_08015080(0xBA9, temp_r6_339, 0, 0x10);
            }
            if (sp3C != (-1))
            {
                sp28 = ((u32)(*((u16 *)(((s8 *)events) + (new_var43 = (((sp38 * 0xA) + sp3C) * 2))))));
            }
            Func_08015270(*windowPtr);
        }
        while (0);
        if (mode == 1)
        {
            Func_08015120(*((u8 *)(((s8 *)ctx) + 0x21A)), 1);
            new_var67 = 0x5001;
            sp8 = 0xBB2;
            Func_08015080(0xBB2, *windowPtr, 0, 0);
            new_var23 = 0x178;
            temp_r2_424 = (*((u16 *)(((s8 *)(new_var79 = ctx)) + new_var23)));
            Func_08015120((((((u32)(0xE0 & temp_r2_424)) >> 5) * 0x14) + (0x1F & temp_r2_424)) + 0x12C, 4);
            new_var65 = 0xE0;
            sp0 = sp48;
            Func_08015280(*windowPtr, (((u32)(new_var65 & ((unsigned char)(*((u16 *)(((s8 *)ctx) + 0x178)))))) >> 5) + new_var67, 6, 0);
            new_var64 = 0xBB4;
            Func_08015080(0xBB3, *windowPtr, 0x38, (float)0);
            Func_08015080(new_var64, *windowPtr, 0, (long long)8);
        }
        if (sp3C == (-1))
        {
            Func_080ad5b4(mode, 0, 0xC8, 0);
        }
        else
        {
            if (mode == 0)
            {
                if (sp24 != 0)
                {
                    if (sp20 == 0)
                    {
                        Func_08015080(0xB98, *windowPtr, 0, 0);
                        do
                        {
                        }
                        while (0);
                    }
                    else
                    {
                        Func_08015080(0xB99, *windowPtr, 0, (double)0);
                    }
                    temp_r6_509 = (((u32)(0xF00 & sp28)) >> 8);
                    new_var38 = (0xE0 & (*(new_var41 = (&sp28))));
                    temp_r7_514 = (0x1F & sp28);
                    temp_r5_515 = (((u32)new_var38) >> 5);
                    if ((Func_08077210(temp_r6_509, temp_r5_515, temp_r7_514) != 0) || (Func_08077208(temp_r6_509, temp_r5_515, temp_r7_514) != 0))
                    {
                        if (Func_08077210(temp_r6_509, temp_r5_515, temp_r7_514) != 0)
                        {
                            Func_080ad608(mode, temp_r5_515, 1);
                        }
                        else
                        {
                            Func_080ad608(mode, temp_r5_515, 2);
                        }
                        new_var5 = sp38;
                        Func_080ad5b4(mode, ((sp18 - new_var5) * 8) + 0x30, 0x3E, 0);
                    }
                    else
                    {
                        Func_08015080(0xB9E, *windowPtr, 0, 0x10);
                        new_var9 = mode;
                        new_var21 = temp_r5_515;
                        new_var82 = 1;
                        new_var26 = new_var82;
                        Func_080ad608(new_var9, new_var21, new_var26);
                        Func_080ad5b4(new_var9, ((sp18 - new_var5) * 8) + 0x30, 0x3E, new_var26);
                    }
                    new_var83 = (((u32)temp_r7_312) >> 1);
                    var_r3_581 = new_var83;
                }
                else
                {
                    new_var77 = 0;
                    sp8 = 0xB9A;
                    Func_08015080(0xB9A, *windowPtr, new_var77, 0);
                    temp_r5_596 = (((u32)(0xF00 & (*(new_var86 = (&sp28))))) >> 8);
                    do
                    {
                        temp_r7_600 = ((new_var54 = ((u32)(0xE0 & sp28))) >> 5);
                    }
                    while (0);
                    temp_r6_601 = (0x1F & sp28);
                    var_r4_606 = 0xB9A;
                    if ((Func_08077210(temp_r5_596, temp_r7_600, temp_r6_601) != 0) || (var_r4_606 = sp8, Func_08077208(temp_r5_596, temp_r7_600, temp_r6_601) != 0))
                    {
                        sp8 = (*(new_var8 = (&var_r4_606)));
                        if (Func_08077210(temp_r5_596, temp_r7_600, temp_r6_601) != 0)
                        {
                            Func_08015080(var_r4_606 + 3, *windowPtr, 0, 0x10);
                            Func_080ad608((unsigned long long)0, temp_r7_600, 1);
                        }
                        else
                        {
                            Func_08015080(var_r4_606 + 2, *windowPtr, 0, 0x10);
                            Func_080ad608(0, temp_r7_600, 2);
                            do
                            {
                            }
                            while (0);
                        }
                        Func_080ad5b4(mode, ((sp18 - sp38) * 8) + 0x30, 0x3E, 0);
                    }
                    else
                    {
                        Func_08015080(var_r4_606 + 4, *windowPtr, 0, 0x10);
                        Func_080ad608(0, temp_r7_600, 1);
                        do
                        {
                            Func_080ad5b4(0, ((new_var71 = (sp18 - sp38)) * 8) + 0x30, 0x3E, 1);
                        }
                        while (0);
                    }
                    var_r3_581 = (((u32)temp_r7_312) >> 1);
                }
                if (var_r3_581 != 0)
                {
                    Func_080ad5f4(mode, 0);
                }
            }
            else
            {
                temp_r6_719 = (((u32)(0xF00 & sp28)) >> 8);
                new_var30 = ((u32)(0xE0 & sp28));
                temp_r7_723 = (0x1F & sp28);
                temp_r5_724 = (new_var30 >> 5);
                if ((Func_08077210(temp_r6_719, temp_r5_724, temp_r7_723) != 0) || (Func_08077208(temp_r6_719, temp_r5_724, temp_r7_723) != 0))
                {
                    if (0 != Func_08077210(temp_r6_719, temp_r5_724, temp_r7_723))
                    {
                        new_var33 = temp_r5_724;
                        Func_080ad608(mode, new_var33, 1);
                    }
                    else
                    {
                        Func_080ad608(mode, *(new_var61 = (&new_var33)), 2);
                    }
                    Func_080ad5b4(mode, ((sp18 - sp38) * 8) + 0x30, 0x36, 0);
                }
                else
                {
                    Func_080ad608(mode, temp_r5_724, 1);
                    Func_080ad5b4(mode, ((sp18 - sp38) * 8) + 0x30, 0x36, 1);
                }
                if ((((u32)temp_r7_312) >> 1) != 0)
                {
                    Func_080ad5f4(*(new_var12 = (&mode)), 0);
                }
            }
        }
        Func_08015278(*((s32 *)(((s8 *)ctx) + 0x30)));
        if (sp3C != (-1))
        {
            Func_08015080(0xBAD, *((s32 *)(((s8 *)ctx) + ((char)0x30))), 0, 0x50);
            sp0 = 0x68;
            new_var36 = (0x1F & sp28);
            Func_08015068(*((s32 *)(0x30 + ((s8 *)ctx))), 0, 0x60, 0xE0);
            new_var88 = (-0x30);
            Func_08015080((((((u32)(0xE0 & sp28)) >> 5) * 0x14) + new_var36) + 0x666, new_var = (*((s32 *)(((s8 *)ctx) - new_var88))), 0, 0x60);
        }
        if (!(1 & ((u8)statusPtr[sp38])))
        {
            sp4 = 0xE;
            sp0 = 1;
            Func_080ab1f4(*(new_var11 = (&(*((s32 *)(((s8 *)ctx) + 0x30))))), (sp18 - sp38) + 1, sp30 + 2, 6);
        }
        (*((s8 *)(((s8 *)(*((void **)((int)((unsigned short)0x03001E8C))))) + 0xEA3))) = 1;
    }
    if (1 & ((u8)statusPtr[sp38]))
    {
        Func_080a1a40(((sp18 - sp38) * 8) - 8, 0x34);
    }
    else
    {
        new_var72 = 8;
        Func_080a1a40(((sp18 - sp38) * new_var72) - new_var72, (sp30 * new_var72) + 0x3C);
    }
    Func_080030f8(1);
    if ((!((*((s32 *)0x03001AE8)) & 0x100)) || ((*((s32 *)0x03001AF8)) & 0x100))
    {
        if (sp24 != 0)
        {
            sp48 = 1;
        }
        sp24 = 0;
        sp20 = 0;
    }
    statusFlags = (*((s32 *)0x03001B04));
    new_var4 = 0x03001C94;
    var_r4_907 = (new_var81 = (*((u32 *)new_var4)));
    do
    {
        eventCode = (*((s32 *)(((s8 *)events) + 0x212C)));
        if (eventCode == 0)
        {
        }
        else
        {
            var_r4_907 = 0;
            (*((s32 *)(((s8 *)events) + 0x2128))) = ((s32)((*((s32 *)(((s8 *)events) + 0x2128))) + 1));
            switchIndex = (eventCode - 1);
            statusFlags = 0;
            switch (switchIndex)
            {
                case 27:
                    if (!((*((u32 *)0x03001C94)) & 1))
                    {
                        do
                        {
                            Func_080a1a40(0x96, 0x1A);
                            Func_080030f8(1);
                        }
                        while (!((*((u32 *)0x03001C94)) & 1));
                    }
                var_r4_907 = 2;
                statusFlags = 2;
                break;
                case 26:
                    if ((*((s32 *)(((s8 *)events) + 0x2128))) != 0x3C)
                    {
                    }
                    else
                    {
                        temp_r5_1001 = Func_08015038(0xC4C, 9, 9, 1);
                        (*((s8 *)0x0200044C)) = 1;
                        while (Func_08015048() == 0)
                        {
                            Func_080030f8(1);
                        }
                        Func_08015018(temp_r5_1001, 1);
                        Func_080aafb8(events);
                        Func_080030f8(1);
                        temp_r5_1029 = Func_08015038(0xC4D, 9, 9, 1);
                        (*((u8 *)((short)0x0200044C))) = 1;
                        while (Func_08015048() == 0)
                        {
                            Func_080030f8(1);
                        }
                        Func_08015018(temp_r5_1029, 1);
                        new_var87 = 2;
                        Func_080aafb8(events);
                        (*((s32 *)(((s8 *)events) + 0x2128))) = 0;
                        Func_080b50f8();
                        Func_080b50f8();
                        Func_080b50f8();
                        Func_080771b8(0u, 0u, 0);
                        Func_080771c8(0u, 0u, 0);
                        Func_08077010(0u);
                        statusFlags = new_var87;
                        block_167:
                            var_r4_907 = 2;
                    }
                break;
                case 0:
                    if ((*((s32 *)(((s8 *)events) + 0x2128))) != 0x3C)
                    {
                    }
                    else
                    {
                        do
                        {
                            sp8 = 0;
                            temp_r5_1083 = Func_08015038((char)0xC40, 9, 9, 1);
                            (*((u8 *)0x0200044C)) = 1;
                            while (Func_08015048() == 0)
                            {
                                Func_080030f8(1);
                            }
                            Func_08015018(temp_r5_1083, 1);
                        }
                        while (0);
                        Func_080aafb8(events);
                        (*((s32 *)(((s8 *)events) + 0x2128))) = 0;
                        (*((s32 *)((new_var90 = ((s8 *)events)) + 0x212C))) = 2;
                        var_r4_907 = sp8;
                    }
                break;
                case 1:
                    case 3:
                        case 7:
                            case 8:
                                case 21:
                                    if ((*((s32 *)(((s8 *)events) + 0x2128))) != 0x5A)
                                    {
                                    }
                                    else
                                    {
                                        new_var29 = 0x2128;
                                        statusFlags = 1;
                                        (*(new_var76 = ((s32 *)((new_var42 = ((s8 *)events)) + new_var29)))) = 0;
                                        var_r2_1122 = (events + 0x212C);
                                        var_r3_1124 = ((*((s32 *)(new_var28 = (((s8 *)events) + 0x212C)))) + 1);
                                        block_162:
                                            var_r4_907 = 1;
                                        (*var_r2_1122) = var_r3_1124;
                                    }
                break;
                case 2:
                    if ((*((s32 *)(((s8 *)events) + 0x2128))) != 0x5A)
                    {
                    }
                    else
                    {
                        (*((s32 *)(((s8 *)events) + 0x2128))) = 0;
                        statusFlags = 0x10;
                        var_r4_907 = 0x10;
                        (*((s32 *)(((s8 *)events) + 0x212C))) = 4;
                    }
                break;
                case 5:
                    case 6:
                        if ((*((s32 *)(((s8 *)events) + 0x2128))) != 0x3C)
                        {
                        }
                        else
                        {
                            sp8 = 0;
                            temp_r5_1163 = Func_08015038(0xC41, 9, 9, 1);
                            (*((u8 *)0x0200044C)) = 1;
                            do
                            {
                                while (Func_08015048() == 0)
                                {
                                    Func_080030f8(1);
                                }
                                Func_08015018(temp_r5_1163, 1);
                            }
                            while (0);
                            Func_080aafb8(events);
                            (*((s32 *)(((s8 *)events) + 0x2128))) = 0;
                            (*((s32 *)(((s8 *)events) + 0x212C))) = 8;
                            var_r4_907 = sp8;
                        }
                break;
                case 12:
                    if ((*((s32 *)(((s8 *)events) + 0x2128))) != 0x28)
                    {
                    }
                    else
                    {
                        (*((s32 *)(((s8 *)events) + 0x2128))) = 0;
                        statusFlags = 2;
                        var_r4_907 = 2;
                        (*((s32 *)(((s8 *)events) + 0x212C))) = ((s32)((*((s32 *)(((s8 *)events) + 0x212C))) + 1));
                    }
                break;
                case 13:
                    case 15:
                        case 16:
                            if ((*((s32 *)(((s8 *)events) + 0x2128))) != 0x28)
                            {
                            }
                            else
                            {
                                (*((s32 *)(((s8 *)events) + 0x2128))) = 0;
                                new_var45 = 0x212C;
                                (*((s32 *)(((s8 *)events) + 0x212C))) = (new_var3 = ((s32)((*((s32 *)(((s8 *)events) + new_var45))) + 1)));
                            }
                break;
                case 14:
                    if ((*((s32 *)(((s8 *)events) + 0x2128))) != 0x3C)
                    {
                    }
                    else
                    {
                        (*((u8 *)0x0200044C)) = 1;
                        sp8 = 0;
                        temp_r5_1278 = Func_08015038(0xC44, 9, 9, 1);
                        Func_080a1ac0(2, 0x92);
                        var_r4_1290 = sp8;
                        while (Func_08015048() == 0)
                        {
                            Func_080030f8(1);
                        }
                        if (!((*((u32 *)0x03001C94)) & 1))
                        {
                            do
                            {
                                sp8 = var_r4_1290;
                                Func_080a1a40((unsigned long long)2, 0x92);
                                Func_080030f8(1);
                            }
                            while (!((*((u32 *)0x03001C94)) & 1));
                        }
                        sp8 = var_r4_1290;
                        Func_08015018(temp_r5_1278, 1);
                        Func_080aafb8(events);
                        Func_080030f8(1);
                        temp_r5_1327 = Func_08015038(0xC45, 9, 9, 1);
                        do
                        {
                            var_r4_1337 = sp8;
                            while (Func_08015048() == 0)
                            {
                                Func_080030f8(1);
                            }
                            if (!(1 & (*((u32 *)0x03001C94))))
                            {
                                do
                                {
                                    sp8 = var_r4_1337;
                                    Func_080a1a40(2, 0x92);
                                    Func_080030f8(1);
                                }
                                while (!((*((u32 *)0x03001C94)) & 1));
                            }
                            sp8 = var_r4_1337;
                            Func_08015018(temp_r5_1327, 1);
                            Func_080aafb8(events);
                        }
                        while (0);
                        Func_080030f8(1);
                        temp_r5_1374 = Func_08015038(0xC46, 9, 9, 1);
                        var_r4_1384 = sp8;
                        while (Func_08015048() == 0)
                        {
                            Func_080030f8(1);
                        }
                        if (!((*((u32 *)0x03001C94)) & 1))
                        {
                            do
                            {
                                new_var63 = var_r4_1384;
                                sp8 = new_var63;
                                Func_080a1a40(((((2 & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu, 0x92);
                                Func_080030f8(1);
                            }
                            while (!((*((u32 *)0x03001C94)) & 1));
                        }
                        sp8 = var_r4_1384;
                        Func_08015018(temp_r5_1374, 1);
                        Func_080aafb8(events);
                        Func_080030f8(1);
                        (*((s32 *)(((s8 *)events) + 0x2128))) = 0;
                        (*((s32 *)((new_var37 = ((s8 *)events)) + 0x212C))) = 0x10;
                        var_r4_907 = sp8;
                    }
                break;
                case 17:
                    case 19:
                        if ((*((s32 *)(((s8 *)events) + 0x2128))) == 0x5A)
                        {
                            statusFlags = 1;
                            do
                            {
                            }
                            while (0);
                            (*((s32 *)(((s8 *)events) + 0x2128))) = 0;
                            var_r3_1124 = 0x15;
                            var_r2_1122 = events;
                            var_r2_1122 = (var_r2_1122 + 0x212C);
                            goto block_162;
                        }
                break;
                case 20:
                    if ((*((s32 *)(0x2128 + ((s8 *)events)))) == 0x5A)
                    {
                        (*((s32 *)(((s8 *)events) + 0x2128))) = 0;
                        statusFlags = 0x20;
                        var_r4_907 = 0x20;
                        (*((s32 *)(0x212C + (new_var75 = ((s8 *)events))))) = 0x16;
                    }
                break;
                case 23:
                    if ((*((s32 *)(((s8 *)events) + 0x2128))) == 0x3C)
                    {
                        (*((s32 *)(((s8 *)events) + 0x2128))) = 0;
                        (*((s32 *)(((s8 *)events) + 0x212C))) = 0x19;
                        statusFlags = 2;
                        goto block_167;
                    }
                break;
            }
        }
        if (mode != 0)
        {
            goto block_186;
        }
        if (!(0x100 & var_r4_907))
        {
            goto block_183;
        }
        if (sp3C == (-1))
        {
            goto block_193;
        }
        sp44 = 0;
    }
    while (0);
    temp_r7_1504 = (((u32)(0xF00 & sp28)) >> 8);
    temp_r5_1508 = (((u32)(0xE0 & sp28)) >> 5);
    temp_r6_1509 = (0x1F & sp28);
    sp8 = var_r4_907;
    var_r4_1515 = var_r4_907;
    if ((Func_08077210(temp_r7_1504, temp_r5_1508, temp_r6_1509) != 0) || (var_r4_1515 = sp8, Func_08077208(temp_r7_1504, temp_r5_1508, temp_r6_1509) != 0))
    {
        sp44 = 1;
    }
    sp24 = 1;
    (*((u32 *)0x03001AF8)) = 0;
    if (sp44 == 0)
    {
        Func_080f9010(0x72);
        Func_08015278(*((s32 *)(((s8 *)ctx) + 0x30)));
        sp0 = 0x68;
        Func_08015068(*(new_var32 = (&(*((s32 *)(new_var19 = ((new_var10 = ((s8 *)ctx)) + 0x30)))))), 0, 0x50, 0xD8);
        Func_08015078(0xBBE, *((s32 *)(((s8 *)ctx) + 0x30)), 0, 0x60);
        sp48 = 1;
        goto loop_36;
    }
    if ((sp28 >> 0xF) != 0)
    {
        sp8 = var_r4_1515;
        Func_080f9010(0xAF);
        new_var74 = 8;
        new_var58 = 0x1F;
        ;
        new_var18 = (((u32)(0xF00 & sp28)) >> new_var74);
        new_var60 = new_var58;
        temp_r3_1572 = (new_var60 & sp28);
        temp_r5_1573 = new_var18;
        temp_r6_1574 = (((u32)(0xE0 & sp28)) >> 5);
        Func_080771b8(temp_r5_1573, temp_r6_1574, temp_r3_1572);
        Func_080771c8(temp_r5_1573, temp_r6_1574, temp_r3_1572);
    }
    else
    {
        new_var44 = 0xF00;
        sp8 = var_r4_1515;
        new_var62 = 5;
        Func_080f9010(0x8B);
        temp_r3_1597 = (0x1F & sp28);
        temp_r5_1598 = (((u32)(new_var44 & sp28)) >> new_var74);
        temp_r6_1599 = (((u32)(0xE0 & sp28)) >> new_var62);
        Func_080771b0(temp_r5_1598, temp_r6_1599, temp_r3_1597);
        Func_080771c0(temp_r5_1598, temp_r6_1599, temp_r3_1597);
    }
    Func_08077010(((u32)(0xF00 & sp28)) >> 8);
    Func_080aafb8(events);
    sp48 = 1;
    var_r4_907 = sp8;
    block_183:
        if ((mode == 0) && (0x200 & var_r4_907))
        {
            result = 7;
            Func_080f9010(0x70);
        }
        else
        {
            block_186:
                if (!(1 & var_r4_907))
                {
                    if (mode != 1)
                    {
                        goto block_208;
                    }
                    if (!(0x100 & var_r4_907))
                    {
                        block_208:
                            if (8 & var_r4_907)
                            {
                                var_r5_1785 = 2;
                                goto block_212;
                            }
                        if (2 & var_r4_907)
                        {
                            do
                            {
                                var_r5_1785 = 1;
                                block_212:
                                    Func_080f9010(0x71);
                                result = var_r5_1785;
                                result = 0;
                                result = (result - result);
                            }
                            while (0);
                        }
                        else
                        {
                            if (mode != 0)
                            {
                                goto block_241;
                            }
                            if (!(4 & var_r4_907))
                            {
                                goto block_241;
                            }
                            if (sp24 == 0)
                            {
                                sp0 = 1;
                                sp4 = 0xF;
                                Func_080ab1f4(*((s32 *)(((s8 *)ctx) + 0x30)), (new_var46 = (sp18 - sp38)) + 1, sp30 + 2, 6);
                                result = 0xA;
                                Func_080f9010(0x70);
                            }
                            else
                            {
                                temp_r5_1817 = (sp20 ^ 1);
                                sp20 = temp_r5_1817;
                                if (temp_r5_1817 != 0)
                                {
                                    Func_080f9010(0x8B);
                                }
                                else
                                {
                                    Func_080f9010(0xAF);
                                }
                                sp44 = 0;
                                if (((s32)(*((u8 *)(((s8 *)ctx) + 0x219)))) <= 0)
                                {
                                }
                                else
                                {
                                    sp14 = 0xA0;
                                    sp10 = 0;
                                    loop_225:
                                        sp40 = 0;
                                    if (((s32)(*((u8 *)(((s8 *)events) + sp14)))) > 0)
                                    {
                                        var_sl_1853 = ((sp10 * 2) + events);
                                        do
                                        {
                                            temp_r7_1856 = (*var_sl_1853);
                                            var_sl_1853 = (var_sl_1853 + 1);
                                            temp_r4_1863 = (((u32)(temp_r7_1856 & 0xF00)) >> 8);
                                            var_r8_1869 = 0;
                                            temp_r6_1871 = (((u32)(temp_r7_1856 & 0xE0)) >> 5);
                                            temp_r5_1872 = (temp_r7_1856 & 0x1F);
                                            if ((Func_08077210(temp_r4_1863, temp_r6_1871, temp_r5_1872) != 0) || (Func_08077208(temp_r4_1863, temp_r6_1871, temp_r5_1872) != 0))
                                            {
                                                var_r8_1869 = 1;
                                            }
                                            if (var_r8_1869 != 0)
                                            {
                                                if (sp20 != 0)
                                                {
                                                    if ((temp_r7_1856 >> 0xF) == 0)
                                                    {
                                                        ;
                                                        temp_r5_1909 = (((u32)(temp_r7_1856 & 0xF00)) >> 8);
                                                        temp_r6_1910 = 5;
                                                        temp_r6_1910 = (((u32)(temp_r7_1856 & 0xE0)) >> temp_r6_1910);
                                                        Func_080771b0(temp_r5_1909, temp_r6_1910, 0x1F & temp_r7_1856);
                                                        Func_080771c0(temp_r5_1909, temp_r6_1910, 0x1F & temp_r7_1856);
                                                        Func_08077010(temp_r5_1909);
                                                    }
                                                }
                                                else
                                                {
                                                    if ((temp_r7_1856 >> 0xF) != 0)
                                                    {
                                                        new_var27 = (((u32)(temp_r7_1856 & 0xF00)) >> 8);
                                                        temp_r6_1938 = (((u32)(temp_r7_1856 & 0xE0)) >> 5);
                                                        temp_r3_1936 = (0x1F & temp_r7_1856);
                                                        temp_r5_1937 = new_var27;
                                                        Func_080771b8(temp_r5_1937, temp_r6_1938, temp_r3_1936);
                                                        Func_080771c8(temp_r5_1937, temp_r6_1938, temp_r3_1936);
                                                        Func_08077010(temp_r5_1937);
                                                    }
                                                }
                                            }
                                            temp_r1_1953 = (sp40 + 1);
                                            new_var49 = temp_r1_1953;
                                            sp40 = new_var49;
                                        }
                                        while (new_var49 < ((((s32)(*((u8 *)(((s8 *)events) + sp14)))) - 1) + 1));
                                    }
                                    temp_r1_1966 = (sp44 + 1);
                                    sp14 += 1;
                                    sp10 += 0xA;
                                    sp44 = temp_r1_1966;
                                    if (temp_r1_1966 < ((s32)(*((u8 *)(((s8 *)ctx) + 0x219)))))
                                    {
                                        goto loop_225;
                                    }
                                }
                                Func_080aafb8(events);
                                sp48 = 1;
                                block_241:
                                    if (0x40 & statusFlags)
                                    {
                                        Func_080f9010(0x6F);
                                        if (!(4 & ((u8)statusPtr[sp38])))
                                        {
                                            do
                                            {
                                                sp4 = 0xF;
                                                new_var39 = (sp30 + 2);
                                                sp0 = 1;
                                                Func_080ab1f4(*((s32 *)(((s8 *)ctx) + 0x30)), (new_var51 = ((*(new_var14 = (&sp18))) - sp38)) + 1, new_var39, 6);
                                            }
                                            while (0);
                                            temp_r2_2035 = ((u8)(*(new_var24 = (&statusPtr)))[sp38]);
                                            if (!(4 & temp_r2_2035))
                                            {
                                                if (1 & temp_r2_2035)
                                                {
                                                    statusPtr[sp38] = temp_r2_2035;
                                                    statusPtr[sp38] = ((-2) & statusPtr[sp38]);
                                                    sp30 = 0;
                                                    goto block_249;
                                                }
                                                if ((sp30 == 0) && (2 & temp_r2_2035))
                                                {
                                                    statusPtr[sp38] = (temp_r2_2035 | 1);
                                                    sp48 = 2;
                                                }
                                                else
                                                {
                                                    block_249:
                                                        sp30 -= 1;
                                                    var_r1_2077 = (*((s8 *)(((s8 *)events) + (sp38 + (new_var35 = 0xA0)))));
                                                    if (0 == var_r1_2077)
                                                    {
                                                        var_r1_2077 = 1;
                                                    }
                                                    temp_r0_2083 = Func_080aa538(sp30, (u8)var_r1_2077);
                                                    sp30 = temp_r0_2083;
                                                    sp2C = temp_r0_2083;
                                                    sp48 = 2;
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        do
                                        {
                                            if (0x80 & statusFlags)
                                            {
                                                if (1)
                                                {
                                                    Func_080f9010(0x6F);
                                                }
                                                if (!(4 & ((u8)statusPtr[sp38])))
                                                {
                                                    sp4 = 0xF;
                                                    sp0 = 1;
                                                    Func_080ab1f4(*((s32 *)(((s8 *)ctx) + 0x30)), ((new_var80 = sp18) - sp38) + 1, sp30 + 2, 6);
                                                    sp30 += 1;
                                                    do
                                                    {
                                                        var_r1_2127 = (*((s8 *)(((s8 *)events) + (new_var17 = (sp38 + 0xA0)))));
                                                        if (var_r1_2127 == 0)
                                                        {
                                                            var_r1_2127 = 1;
                                                        }
                                                    }
                                                    while (0);
                                                    sp30 = Func_080aa538(sp30, (u8)var_r1_2127);
                                                    temp_r2_2137 = ((u8)statusPtr[sp38]);
                                                    if ((1 & temp_r2_2137) && (!(4 & temp_r2_2137)))
                                                    {
                                                        statusPtr[sp38] = (-2);
                                                        statusPtr[sp38] = (statusPtr[sp38] & temp_r2_2137);
                                                        sp30 = 0;
                                                    }
                                                    else
                                                    {
                                                        if (sp30 == 0)
                                                        {
                                                            temp_r2_2165 = ((u8)statusPtr[sp38]);
                                                            if (2 & temp_r2_2165)
                                                            {
                                                                do
                                                                {
                                                                }
                                                                while (0);
                                                                statusPtr[*(new_var59 = (&(*(new_var13 = (&sp38)))))] = (1 | temp_r2_2165);
                                                            }
                                                        }
                                                    }
                                                    sp2C = sp30;
                                                    sp48 = 2;
                                                }
                                            }
                                            else
                                            {
                                                if (0x20 & statusFlags)
                                                {
                                                    Func_080f9010(0x6F);
                                                    if (!(4 & ((u8)statusPtr[sp38])))
                                                    {
                                                        sp0 = 1;
                                                        sp4 = 0xF;
                                                        Func_080ab1f4(*((s32 *)(((s8 *)ctx) + 0x30)), (sp18 - sp38) + 1, sp30 + 2, 6);
                                                    }
                                                    temp_r3_2211 = (sp38 - 1);
                                                    sp38 = temp_r3_2211;
                                                    sp38 = Func_080aa538(temp_r3_2211, *((u8 *)(((s8 *)ctx) + 0x219)));
                                                    if (mode == 0)
                                                    {
                                                        sp44 = 0;
                                                        if (mode < ((s32)(*((u8 *)(((s8 *)ctx) + 0x219)))))
                                                        {
                                                            do
                                                            {
                                                                if (statusPtr[sp38] == 4)
                                                                {
                                                                    temp_r0_2233 = (sp38 - 1);
                                                                    sp38 = temp_r0_2233;
                                                                    sp38 = Func_080aa538(temp_r0_2233, *((u8 *)(((s8 *)ctx) + 0x219)));
                                                                }
                                                                temp_r1_2240 = (sp44 + 1);
                                                                sp44 = temp_r1_2240;
                                                            }
                                                            while (temp_r1_2240 < ((s32)(*((u8 *)(((s8 *)ctx) + 0x219)))));
                                                        }
                                                    }
                                                    goto block_281;
                                                }
                                                if (0x10 & statusFlags)
                                                {
                                                    Func_080f9010(0x6F);
                                                    if (!(4 & ((u8)statusPtr[sp38])))
                                                    {
                                                        sp0 = 1;
                                                        sp4 = 0xF;
                                                        Func_080ab1f4(*((s32 *)(((s8 *)ctx) + 0x30)), (sp18 - sp38) + 1, sp30 + 2, 6);
                                                    }
                                                    temp_r3_2279 = (sp38 + 1);
                                                    sp38 = temp_r3_2279;
                                                    sp38 = Func_080aa538(temp_r3_2279, *((u8 *)(((s8 *)ctx) + 0x219)));
                                                    if (mode == 0)
                                                    {
                                                        sp44 = 0;
                                                        if (mode < ((s32)(*((u8 *)(((s8 *)ctx) + 0x219)))))
                                                        {
                                                            do
                                                            {
                                                                if (statusPtr[sp38] == 4)
                                                                {
                                                                    temp_r0_2301 = (sp38 + 1);
                                                                    sp38 = temp_r0_2301;
                                                                    sp38 = Func_080aa538(temp_r0_2301, *((u8 *)(((s8 *)ctx) + 0x219)));
                                                                }
                                                                temp_r1_2308 = (sp44 - (-1));
                                                                sp44 = temp_r1_2308;
                                                            }
                                                            while (temp_r1_2308 < ((s32)(*((u8 *)(((s8 *)ctx) + 0x219)))));
                                                        }
                                                    }
                                                    block_281:
                                                        sp30 = sp2C;
                                                    var_r1_2319 = (*((s8 *)(((s8 *)events) + (new_var25 = (sp38 + 0xA0)))));
                                                    if (var_r1_2319 == 0)
                                                    {
                                                        var_r1_2319 = 1;
                                                    }
                                                    sp30 = Func_080aa538(sp30, (u8)var_r1_2319);
                                                    sp48 = 2;
                                                    sp18 = (sp38 * 8);
                                                }
                                            }
                                        }
                                        while (0);
                                    }
                                goto loop_36;
                            }
                        }
                    }
                    else
                    {
                        goto block_191;
                    }
                }
                else
                {
                    block_191:
                        sp44 = 1;
                    if (!(1 & ((u8)statusPtr[sp38])))
                    {
                        if (sp3C == (-1))
                        {
                            block_193:
                                Func_080f9010(0x72);
                            goto loop_36;
                        }
                        temp_r7_1696 = (((u32)(((unsigned char)0xF00) & sp28)) >> 8);
                        temp_r6_1701 = 5;
                        temp_r6_1701 = (((u32)(0xE0 & sp28)) >> temp_r6_1701);
                        temp_r5_1702 = (0x1F & sp28);
                        sp44 = 0;
                        if ((Func_08077210(temp_r7_1696, temp_r6_1701, temp_r5_1702) != 0) || (Func_08077208(temp_r7_1696, temp_r6_1701, temp_r5_1702) != 0))
                        {
                            sp44 = 1;
                        }
                    }
                    if (sp44 == 0)
                    {
                        Func_080f9010(0x72);
                        Func_08015278(*(new_var78 = (new_var20 = ((s32 *)(((s8 *)ctx) + 0x30)))));
                        sp0 = 0x68;
                        new_var69 = 0x30;
                        Func_08015068(*(new_var91 = ((s32 *)(((s8 *)ctx) + new_var69))), 0, 0x50, 0xD8);
                        Func_08015078(0xBBE, *((s32 *)(new_var22 = (((s8 *)ctx) + 0x30))), 0, 0x60);
                        goto loop_36;
                    }
                    if (mode == 1)
                    {
                        result = 4;
                        if (mode & ((u8)statusPtr[sp38]))
                        {
                            if (sp38 == ((char)(*((s8 *)(((s8 *)ctx) + 0x1C)))))
                            {
                                result = 2;
                                if (!(((short)0x8000) & (*((u16 *)(((s8 *)ctx) + 0x178)))))
                                {
                                    goto block_206;
                                }
                            }
                            else
                            {
                                result = 3;
                            }
                        }
                    }
                    else
                    {
                        block_206:
                            result = 1;
                    }
                    Func_080f9010(0x70);
                }
        }
    (*((s8 *)((new_var7 = ((s8 *)ctx)) + (mode + 0x1C)))) = sp38;
    if (sp3C != (-1))
    {
        temp_r2_2357 = (*((u16 *)(new_var57 = ((new_var48 = ((s8 *)events)) + (((sp38 * 0xA) + sp3C) * 2)))));
        (*((u16 *)(((s8 *)ctx) + (sp34 + (new_var68 = 0x178))))) = temp_r2_2357;
        temp_r0_2363 = (mode + 0x254);
        new_var55 = (temp_r0_2363 + 2);
        (*((s8 *)(((s8 *)ctx) + temp_r0_2363))) = (0x1F & temp_r2_2357);
        ;
        (*((s8 *)(((s8 *)ctx) + new_var55))) = ((s8)(((u32)(0xE0 & temp_r2_2357)) >> 5));
        new_var16 = 0x258;
        (*((s8 *)(((s8 *)ctx) + (mode + new_var16)))) = ((s8)(((u32)(0xF00 & temp_r2_2357)) >> 8));
    }
    (*((u16 *)(((s8 *)ctx) + (sp34 + 0x174)))) = (sp38 + (sp30 * 0xA));
    return result;
}


#include "types.h"

/*
 * Colosso tournament selection and progression handler.
 *
 * This draft is a permuter-verified structural rewrite (out/colosso-r7,
 * candidate 97997, semantics-preserving temp-extraction/reorder mutations
 * only) adopted for its measured improvement: wrong_instructions 1972->1786,
 * differing_halfwords 2364->2335, still 4752/4888 bytes. It reads rougher
 * than the hand-named draft it replaced -- the M2C_FIELD macro got
 * expanded to raw pointer arithmetic and several generated new_varN locals
 * appear -- because the priority right now is closing the remaining byte
 * gap; the readability/humanization pass (ctx/statusPtr/mode and friends)
 * needs to be redone once this is byte-exact. Not byte-exact yet.
 */
typedef s32 M2C_UNK;
s32 Func_080ab5e4(s32 arg0)
{
    int new_var47;
    u32 sp28;
    s32 sp48;
    int new_var87;
    s32 *win10C;
    void *new_var20;
    int new_var126;
    s32 new_var113;
    s32 sp4;
    u32 sp8;
    s32 sp10;
    s8 *statusPtr;
    s32 sp2C;
    int new_var151;
    u32 new_var141;
    s32 sp14;
    s32 sp18;
    int new_var42;
    s32 sp20;
    int new_var111;
    void *new_var95;
    s8 **new_var17;
    s32 sp30;
    s32 sp0;
    s32 sp34;
    int new_var72;
    s32 sp38;
    s8 status[8];
    s32 sp3C;
    int new_var146;
    int new_var117;
    int new_var116;
    u16 new_var58;
    s32 sp40;
    s8 *new_var128;
    s32 new_var40;
    int new_var104;
    void *ctx;
    s32 sp44;
    int new_var94;
    s8 **new_var26;
    s32 *new_var6;
    int new_var66;
    s8 *new_var31;
    int new_var106;
    s32 mode;
    u8 buf54[8];
    u32 *new_var57;
    int new_var102;
    s32 *var_r2_1122;
    s32 temp_r0_119;
    u16 new_var100;
    int new_var90;
    int new_var124;
    s32 temp_r0_2083;
    int new_var122;
    s32 temp_r0_2233;
    int new_var45;
    int new_var140;
    void *new_var82;
    u16 new_var130;
    s32 *new_var148;
    int new_var25;
    s32 temp_r0_2301;
    u32 new_var112;
    int new_var97;
    s32 temp_r0_2363;
    int new_var78;
    int new_var75;
    int new_var142;
    s32 temp_r1_1953;
    s32 temp_r1_1966;
    s32 new_var44;
    int new_var105;
    s32 sp24;
    int new_var54;
    int new_var69;
    s32 temp_r1_2240;
    s32 new_var93;
    s32 temp_r1_126;
    u32 new_var109;
    s32 *new_var5;
    s32 temp_r1_2308;
    s8 *new_var3;
    s32 temp_r1_910;
    int new_var85;
    int new_var139;
    s32 temp_r2_25;
    s8 *new_var60;
    s32 *new_var144;
    s32 temp_r3_1572;
    s8 *new_var129;
    s32 temp_r3_1597;
    s32 temp_r3_1908;
    int new_var138;
    s32 temp_r3_1936;
    s32 *new_var76;
    s32 temp_r3_2211;
    s32 temp_r3_2279;
    u32 new_var56;
    s32 temp_r3_88;
    int new_var46;
    int new_var89;
    s32 temp_r5_1001;
    s32 temp_r5_1029;
    int new_var2;
    s32 temp_r5_1083;
    int new_var27;
    int new_var149;
    s32 temp_r5_1163;
    s32 temp_r5_1327;
    void *new_var67;
    s32 temp_r5_1278;
    int new_var32;
    int new_var65;
    s32 new_var14;
    int new_var64;
    int new_var28;
    int new_var33;
    u32 *new_var9;
    s32 temp_r5_1374;
    s32 temp_r5_1702;
    s32 temp_r5_1817;
    int new_var98;
    s8 *new_var83;
    s32 *new_var29;
    s8 *new_var50;
    int new_var96;
    s8 *new_var135;
    int new_var16;
    s8 *new_var51;
    int new_var73;
    u32 new_var52;
    s32 temp_r5_1872;
    s32 temp_r5_95;
    s8 *new_var137;
    int new_var24;
    s32 temp_r6_1509;
    s32 temp_r6_216;
    s32 temp_r6_339;
    int new_var134;
    s32 temp_r6_601;
    void *new_var84;
    int new_var74;
    s32 *new_var132;
    s32 temp_r7_312;
    s32 temp_r7_514;
    int new_var55;
    s32 temp_r7_723;
    s32 var_fp_905;
    s32 var_r0_255;
    s32 var_r3_1124;
    u8 *new_var119;
    int new_var79;
    s8 *new_var30;
    int new_var35;
    int new_var19;
    s32 var_r5_1634;
    s32 var_r5_1785;
    s8 *var_r1_151;
    int new_var118;
    int new_var53;
    s32 new_var63;
    s32 var_r5_237;
    int new_var131;
    s32 var_r6_240;
    s32 var_r8_1869;
    s8 *var_r0_148;
    s8 *var_r1_82;
    int new_var11;
    s8 *var_r2_152;
    u32 new_var38;
    u32 new_var145;
    void *new_var127;
    int new_var80;
    s8 *new_var22;
    int new_var121;
    void *new_var18;
    int new_var115;
    int new_var101;
    s8 *var_r3_61;
    s8 var_r1_2077;
    int new_var70;
    s8 var_r1_2127;
    s8 var_r1_2319;
    u16 *var_sl_1853;
    int new_var61;
    s32 new_var136;
    void *new_var71;
    s8 *new_var92;
    u16 temp_r2_2357;
    u16 temp_r0_47;
    u16 temp_r2_218;
    u16 temp_r2_424;
    u16 temp_r5_204;
    int new_var36;
    u16 temp_r5_34;
    u16 temp_r6_199;
    s32 new_var99;
    u16 temp_r7_1856;
    u32 temp_r3_921;
    int new_var21;
    u32 temp_r4_1863;
    u16 *new_var86;
    u32 temp_r5_1508;
    int new_var110;
    u32 temp_r5_1573;
    u32 temp_r5_1598;
    void *new_var41;
    u32 temp_r5_1909;
    s8 *new_var4;
    u32 temp_r5_1937;
    u32 temp_r5_515;
    u32 temp_r5_596;
    u32 temp_r5_724;
    void *new_var43;
    s32 *new_var147;
    u32 temp_r6_1574;
    int new_var48;
    u32 temp_r6_1599;
    u32 new_var150;
    int new_var49;
    u32 temp_r6_1701;
    u32 temp_r6_1871;
    void *new_var143;
    u32 temp_r6_1910;
    u32 temp_r6_1938;
    s8 *new_var12;
    int new_var103;
    u32 temp_r6_509;
    int new_var120;
    u32 temp_r6_719;
    u32 temp_r7_1504;
    s8 *new_var59;
    u32 temp_r7_1696;
    s32 **new_var10;
    s8 *new_var8;
    u32 var_r3_581;
    u32 temp_r7_600;
    u32 var_r4_1290;
    int new_var133;
    void *new_var;
    u32 var_r4_1337;
    u32 var_r4_1384;
    s32 new_var88;
    u32 var_r4_1515;
    void *new_var15;
    u32 var_r4_606;
    u32 new_var81;
    u32 new_var108;
    u32 var_r4_907;
    u8 *var_r2_85;
    int new_var37;
    s8 *new_var7;
    u8 temp_r2_2035;
    s32 *new_var107;
    u8 temp_r2_2137;
    int new_var34;
    u8 temp_r2_2165;
    int new_var114;
    void *temp_r3_18;
    void *new_var123;
    void *temp_r5_343;
    void *temp_r9_32;
    u32 new_var13;
    s8 *new_var68;
    int new_var23;
    int new_var91;
    int new_var77;
    s32 *new_var125;
    s8 *new_var62;
    int new_var39;
    new_var55 = ((unsigned short)0);
    mode = arg0;
    temp_r3_18 = (*((void **)0x03001F2C));
    new_var27 = 2;
    ctx = temp_r3_18;
    temp_r9_32 = (*((void **)(((s8 *)temp_r3_18) + 0x184)));
    sp48 = 1;
    new_var111 = (mode * new_var27);
    new_var87 = new_var111;
    temp_r2_25 = new_var87;
    new_var65 = 0;
    sp34 = new_var87;
    temp_r5_34 = (new_var130 = (*(new_var86 = ((u16 *)(((s8 *)ctx) + (temp_r2_25 + 0x174))))));
    sp38 = ((u16)Func_08002304(temp_r5_34, 0xA));
    temp_r0_47 = Func_080022f4(temp_r5_34, 0xA);
    statusPtr = (new_var129 = (new_var83 = status));
    sp30 = ((s32)temp_r0_47);
    sp2C = new_var65;
    sp28 = new_var55;
    sp3C = (-1);
    sp24 = new_var65;
    sp20 = new_var65;
    var_r3_61 = (&status[7]);
    do
    {
        (*var_r3_61) = 0;
        var_r3_61 = (var_r3_61 - 1);
    }
    while (((s32)var_r3_61) >= ((s32)statusPtr));
    if (mode == 0)
    {
        Func_080aafb8(temp_r9_32);
        sp44 = 0;
        if (mode < ((s32)(*((u8 *)(((s8 *)ctx) + 0x219)))))
        {
            do
            {
                var_r1_82 = statusPtr;
            }
            while (0);
            var_r2_85 = ((new_var43 = temp_r9_32) + 0xA0);
            do
            {
                temp_r3_88 = (*var_r2_85);
                temp_r3_88 = (temp_r3_88 << 0x18);
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
        sp2C = sp30;
        sp44 = 0;
        if (((s32)(*((u8 *)(((s8 *)ctx) + 0x219)))) > 0)
        {
            do
            {
                if (statusPtr[sp38] == 4)
                {
                    temp_r0_119 = (sp38 + 1);
                    sp38 = temp_r0_119;
                    sp38 = Func_080aa538(temp_r0_119, *((u8 *)(((s8 *)ctx) + 0x219)));
                }
                temp_r1_126 = (sp44 + 1);
                sp44 = temp_r1_126;
            }
            while (temp_r1_126 < ((s32)(*((u8 *)(((s8 *)ctx) + 0x219)))));
        }
    }
    else
    {
        Func_080ae714(buf54, *((s8 *)(((s8 *)ctx) + 0x1C)));
        sp44 = 0;
        if (((s32)(*((u8 *)(((s8 *)ctx) + 0x219)))) > 0)
        {
            var_r0_148 = statusPtr;
            var_r1_151 = (temp_r9_32 + 0xA0);
            var_r2_152 = (*(new_var26 = (&var_r0_148)));
            do
            {
                if (sp44 == (*((s8 *)(((s8 *)ctx) + 0x1C))))
                {
                    (*var_r2_152) = 7;
                }
                else
                {
                    if (buf54[(short)sp44] != 0)
                    {
                        (*var_r2_152) = 0;
                    }
                    else
                    {
                        (*var_r2_152) = 3;
                        if ((*var_r1_151) == 0)
                        {
                            (*var_r0_148) = 7;
                        }
                    }
                }
                sp44 += 1;
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
        do
        {
            temp_r5_204 = Func_08002304(temp_r6_199, 0xA);
            ;
            temp_r2_218 = Func_080022f4(temp_r6_199, 0xA);
            new_var73 = 6;
            sp4 = 0xE;
            sp0 = mode;
            new_var122 = (temp_r2_218 + 2);
            Func_080ab1f4(*((s32 *)(new_var137 = (((s8 *)ctx) + 0x30))), (temp_r5_204 * 7) + 1, new_var122, 6);
            sp0 = 7;
        }
        while (0);
        sp4 = new_var73;
        Func_080ab2ec(*((s32 *)(new_var92 = (((s8 *)ctx) + 0x30))), (temp_r5_204 * 7) + 1, 2);
        var_r5_237 = 0;
        if (((((s32)(*((u8 *)(((s8 *)ctx) + 0x219)))) - 1) + 1) > 0)
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
                Func_08015080(var_r0_255, *((s32 *)((new_var128 = ((s8 *)ctx)) + 0x30)), var_r6_240, 8);
                var_r5_237 += 1;
                var_r6_240 += 0x38;
            }
            while (var_r5_237 < ((s32)(*((u8 *)(((s8 *)ctx) + 0x219)))));
        }
    }
    win10C = ((s32 *)(((s8 *)temp_r3_18) + 0x10C));
    Func_08015270(*win10C);
    (*((s8 *)(((s8 *)(*((void **)(((s8 *)ctx) + 0x14)))) + 5))) = 1;
    sp18 = (sp38 * 8);
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
        temp_r6_339 = ((unsigned short)(*((s32 *)(((s8 *)ctx) + 0x10))));
        new_var36 = 0x208;
        temp_r5_343 = Func_08077008(new_var100 = (*((u16 *)(((s8 *)ctx) + ((sp38 * 2) + new_var36)))));
        Func_08015270(temp_r6_339);
        Func_08015090(temp_r5_343, temp_r6_339, 0, 0);
        new_var18 = ((void *)0x080AF28C);
        do
        {
            new_var126 = 0;
            Func_08015080((*((u8 *)(((s8 *)temp_r5_343) + 0x129))) + 0x741, temp_r6_339, new_var126, 8);
            Func_08015090(new_var18, temp_r6_339, 0x30, 0);
            sp0 = sp48;
            Func_080150b0(*((u8 *)(((s8 *)temp_r5_343) + 0xF)), 2, temp_r6_339, 0x48);
            if (mode == 0)
            {
                Func_08015080(0xBA9, temp_r6_339, 0, 0x10);
            }
        }
        while (0);
        if (sp3C != (-1))
        {
            sp28 = ((u32)(*((u16 *)(((s8 *)temp_r9_32) + (((sp38 * 0xA) + sp3C) * 2)))));
        }
        Func_08015270(*win10C);
        if (mode == 1)
        {
            new_var102 = 0xBB4;
            Func_08015120(*((u8 *)(((s8 *)ctx) + 0x21A)), 1);
            new_var115 = 0xBB2;
            sp8 = new_var115;
            Func_08015080(new_var115, *win10C, 0, 0);
            new_var104 = 0xE0;
            new_var121 = 0x178;
            temp_r2_424 = (*((u16 *)(new_var3 = (((s8 *)ctx) + new_var121))));
            new_var32 = ((((((u32)(new_var104 & temp_r2_424)) >> 5) * 0x14) + (0x1F & temp_r2_424)) + 0x12C);
            Func_08015120(new_var32, 4);
            ;
            sp0 = sp48;
            new_var120 = 0x5001;
            new_var114 = 0;
            Func_08015280(*win10C, (((u32)(0xE0 & (new_var58 = (*((u16 *)(((s8 *)(new_var123 = ctx)) + new_var121)))))) >> 5) + new_var120, 6, new_var114);
            Func_08015080(0xBB3, *win10C, 0x38, new_var114);
            new_var69 = new_var102;
            Func_08015080(new_var69, *(new_var125 = win10C), new_var114, 8);
        }
        if (sp3C == (-1))
        {
            Func_080ad5b4(*(new_var144 = (&mode)), 0, 0xC8, 0);
        }
        else
        {
            if (mode == 0)
            {
                if (sp24 != 0)
                {
                    if (sp20 == 0)
                    {
                        new_var103 = 0xB98;
                        Func_08015080(new_var103, *(*(new_var10 = (&win10C))), 0, 0);
                    }
                    else
                    {
                        Func_08015080(0xB99, *win10C, 0, 0);
                    }
                    new_var150 = (((u32)(0xF00 & sp28)) >> 8);
                    temp_r6_509 = new_var150;
                    new_var52 = ((u32)(0xE0 & sp28));
                    temp_r7_514 = (0x1F & sp28);
                    temp_r5_515 = (new_var52 >> 5);
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
                        Func_080ad5b4(mode, ((sp18 - sp38) * 8) + 0x30, 0x3E, 0);
                    }
                    else
                    {
                        Func_08015080(0xB9E, *win10C, 0, 0x10);
                        Func_080ad608(mode, temp_r5_515, 1);
                        Func_080ad5b4(mode, ((new_var101 = (sp18 - sp38)) * 8) + 0x30, 0x3E, 1);
                    }
                    var_r3_581 = (((u32)temp_r7_312) >> 1);
                }
                else
                {
                    sp8 = 0xB9A;
                    Func_08015080(0xB9A, *win10C, 0, 0);
                    temp_r5_596 = (((u32)(0xF00 & sp28)) >> 8);
                    temp_r7_600 = (((u32)(0xE0 & sp28)) >> 5);
                    new_var13 = sp28;
                    var_r4_606 = ((char)0xB9A);
                    temp_r6_601 = (0x1F & new_var13);
                    if ((Func_08077210(temp_r5_596, (short)temp_r7_600, temp_r6_601) != 0) || (var_r4_606 = sp8, Func_08077208(temp_r5_596, temp_r7_600, temp_r6_601) != 0))
                    {
                        sp8 = var_r4_606;
                        if (Func_08077210(temp_r5_596, temp_r7_600, temp_r6_601) != 0)
                        {
                            Func_08015080(3 + var_r4_606, *win10C, 0, 0x10);
                            Func_080ad608(0, temp_r7_600, 1);
                        }
                        else
                        {
                            Func_08015080((*(new_var57 = (&var_r4_606))) + 2, *win10C, 0, 0x10);
                            Func_080ad608(0, temp_r7_600, 2);
                        }
                        Func_080ad5b4(mode, ((sp18 - sp38) * 8) + 0x30, 0x3E, 0);
                    }
                    else
                    {
                        do
                        {
                            Func_08015080(var_r4_606 + 4, *win10C, 0, 0x10);
                            Func_080ad608(0, temp_r7_600, 1);
                            new_var33 = 0;
                        }
                        while (0);
                        Func_080ad5b4(new_var33, ((sp18 - sp38) * 8) + 0x30, 0x3E, 1);
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
                new_var38 = ((u32)(0xE0 & sp28));
                temp_r7_723 = (0x1F & sp28);
                temp_r5_724 = (new_var38 >> 5);
                if ((Func_08077210(temp_r6_719, temp_r5_724, temp_r7_723) != 0) || (Func_08077208(temp_r6_719, temp_r5_724, temp_r7_723) != 0))
                {
                    if (Func_08077210(temp_r6_719, temp_r5_724, temp_r7_723) != 0)
                    {
                        Func_080ad608(mode, temp_r5_724, 1);
                    }
                    else
                    {
                        Func_080ad608(mode, temp_r5_724, 2);
                    }
                    Func_080ad5b4(mode, ((sp18 - sp38) * 8) + 0x30, 0x36, 0);
                }
                else
                {
                    Func_080ad608(mode, temp_r5_724, 1);
                    new_var53 = 1;
                    Func_080ad5b4(mode, ((sp18 - sp38) * 8) + 0x30, 0x36, new_var53);
                }
                if ((((u32)temp_r7_312) >> 1) != 0)
                {
                    new_var136 = mode;
                    Func_080ad5f4(new_var136, 0);
                }
            }
        }
        Func_08015278(*((s32 *)((new_var60 = ((s8 *)ctx)) + 0x30)));
        if (sp3C != (-1))
        {
            Func_08015080(0xBAD, *((s32 *)(((s8 *)ctx) + 0x30)), 0, 0x50);
            new_var74 = 0x30;
            sp0 = 0x68;
            new_var79 = (0x1F & sp28);
            Func_08015068(*(new_var29 = (&(*((s32 *)((new_var12 = ((s8 *)ctx)) + 0x30))))), 0, 0x60, 0xE0);
            Func_08015080((((((u32)(0xE0 & (new_var141 = sp28))) >> 5) * 0x14) + new_var79) + 0x666, *((s32 *)(((s8 *)(new_var95 = ctx)) + new_var74)), 0, 0x60);
        }
        if (!(1 & ((u8)statusPtr[sp38])))
        {
            sp4 = 0xE;
            sp0 = 1;
            Func_080ab1f4(*((s32 *)(new_var31 = (((s8 *)(new_var71 = ctx)) + 0x30))), (new_var45 = ((*(new_var76 = (&sp18))) - sp38)) + 1, sp30 + 2, 6);
        }
        (*((s8 *)(((s8 *)(*((void **)0x03001E8C))) + 0xEA3))) = 1;
    }
    if (1 & ((u8)statusPtr[sp38]))
    {
        Func_080a1a40(((sp18 - sp38) * 8) - 8, 0x34);
    }
    else
    {
        do
        {
        }
        while (0);
        Func_080a1a40(((sp18 - sp38) * 8) - 8, (sp30 * 8) + 0x3C);
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
    var_fp_905 = (*((s32 *)0x03001B04));
    var_r4_907 = (*((u32 *)((short)0x03001C94)));
    temp_r1_910 = (new_var63 = (*((s32 *)(((s8 *)temp_r9_32) + 0x212C))));
    if (temp_r1_910 == 0)
    {
    }
    else
    {
        var_r4_907 = 0;
        (*((s32 *)(((s8 *)temp_r9_32) + 0x2128))) = ((s32)((new_var113 = (*((s32 *)(((s8 *)temp_r9_32) + 0x2128)))) + 1));
        new_var146 = 1;
        temp_r3_921 = (temp_r1_910 - new_var146);
        var_fp_905 = 0;
        switch (temp_r3_921)
        {
            case 27:
                if (!((*((u32 *)0x03001C94)) & 1))
                {
                    do
                    {
                        Func_080a1a40((unsigned long long)0x96, 0x1A);
                        Func_080030f8(1);
                    }
                    while (!((*((u32 *)0x03001C94)) & 1));
                }
            var_r4_907 = 2;
            var_fp_905 = 2;
            break;
            case 26:
                if ((*((s32 *)(((s8 *)temp_r9_32) + 0x2128))) != 0x3C)
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
                    Func_080aafb8(temp_r9_32);
                    Func_080030f8(1);
                    temp_r5_1029 = Func_08015038(0xC4D, 9, 9, 1);
                    (*((u8 *)0x0200044C)) = 1;
                    while (Func_08015048() == 0)
                    {
                        Func_080030f8(1);
                    }
                    Func_08015018(temp_r5_1029, 1);
                    new_var118 = 0;
                    Func_080aafb8(temp_r9_32);
                    (*((s32 *)(((s8 *)temp_r9_32) + 0x2128))) = 0;
                    Func_080b50f8();
                    Func_080b50f8();
                    Func_080b50f8();
                    Func_080771b8(0u, 0u, 0);
                    new_var28 = 0u;
                    Func_080771c8(new_var28, new_var28, 0);
                    Func_08077010(new_var28);
                    var_fp_905 = 2;
                    block_167:
                        var_r4_907 = 2;
                }
            break;
            case 0:
                if ((*((s32 *)(((s8 *)temp_r9_32) + 0x2128))) != 0x3C)
                {
                }
                else
                {
                    sp8 = 0;
                    temp_r5_1083 = Func_08015038(0xC40, 9, 9, 1);
                    (*((u8 *)0x0200044C)) = 1;
                    while (Func_08015048() == 0)
                    {
                        Func_080030f8(1);
                    }
                    Func_08015018(temp_r5_1083, 1);
                    new_var91 = 2;
                    Func_080aafb8(temp_r9_32);
                    (*((s32 *)(new_var68 = ((new_var4 = (new_var51 = ((s8 *)temp_r9_32))) + 0x2128)))) = 0;
                    (*((s32 *)(((s8 *)(new_var82 = temp_r9_32)) + 0x212C))) = new_var91;
                    var_r4_907 = sp8;
                }
            break;
            case 1:
                case 3:
                    case 7:
                        case 8:
                            case 21:
                                if ((*((s32 *)(((s8 *)temp_r9_32) + 0x2128))) != 0x5A)
                                {
                                }
                                else
                                {
                                    var_fp_905 = 1;
                                    (*((s32 *)(((s8 *)temp_r9_32) + 0x2128))) = 0;
                                    var_r2_1122 = (temp_r9_32 + 0x212C);
                                    var_r3_1124 = (new_var37 = ((*((s32 *)(((s8 *)temp_r9_32) + 0x212C))) + 1));
                                    block_162:
                                        var_r4_907 = 1;
                                    (*var_r2_1122) = var_r3_1124;
                                }
            break;
            case 2:
                if ((*((s32 *)(((s8 *)temp_r9_32) + 0x2128))) != 0x5A)
                {
                }
                else
                {
                    (*((s32 *)(((s8 *)(new_var41 = temp_r9_32)) + 0x2128))) = 0;
                    var_fp_905 = 0x10;
                    var_r4_907 = 0x10;
                    (*((s32 *)(new_var30 = (((s8 *)temp_r9_32) + 0x212C)))) = 4;
                }
            break;
            case 5:
                case 6:
                    if ((*((s32 *)(((s8 *)temp_r9_32) + 0x2128))) != 0x3C)
                    {
                    }
                    else
                    {
                        sp8 = 0;
                        temp_r5_1163 = Func_08015038(0xC41, 9, 9, 1);
                        (*(new_var119 = ((u8 *)0x0200044C))) = 1;
                        while (Func_08015048() == 0)
                        {
                            Func_080030f8(1);
                        }
                        Func_08015018(temp_r5_1163, 1);
                        Func_080aafb8(temp_r9_32);
                        (*((s32 *)(((s8 *)temp_r9_32) + 0x2128))) = 0;
                        new_var90 = 0x212C;
                        (*((s32 *)(((s8 *)temp_r9_32) + new_var90))) = 8;
                        var_r4_907 = (*(new_var9 = (&sp8)));
                    }
            break;
            case 12:
                if ((*((s32 *)(((s8 *)temp_r9_32) + 0x2128))) != 0x28)
                {
                }
                else
                {
                    (*((s32 *)(((s8 *)temp_r9_32) + 0x2128))) = 0;
                    var_fp_905 = 2;
                    new_var124 = 1;
                    var_r4_907 = 2;
                    (*((s32 *)(((s8 *)temp_r9_32) + 0x212C))) = ((s32)((*((s32 *)((new_var62 = ((s8 *)temp_r9_32)) + 0x212C))) + new_var124));
                }
            break;
            case 13:
                case 15:
                    case 16:
                        if ((*((s32 *)(((s8 *)temp_r9_32) + 0x2128))) != 0x28)
                        {
                        }
                        else
                        {
                            (*((s32 *)(((s8 *)temp_r9_32) + 0x2128))) = 0;
                            new_var48 = 1;
                            (*((s32 *)((new_var7 = ((s8 *)temp_r9_32)) + 0x212C))) = ((s32)((new_var40 = (*((s32 *)(((s8 *)temp_r9_32) + 0x212C)))) + new_var48));
                        }
            break;
            case 14:
                if ((*((s32 *)(((s8 *)temp_r9_32) + 0x2128))) != 0x3C)
                {
                }
                else
                {
                    (*((u8 *)((unsigned short)0x0200044C))) = 1;
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
                            Func_080a1a40(2, 0x92);
                            Func_080030f8(1);
                        }
                        while (!((*((u32 *)0x03001C94)) & 1));
                    }
                    sp8 = var_r4_1290;
                    Func_08015018(temp_r5_1278, 1);
                    Func_080aafb8(temp_r9_32);
                    Func_080030f8(1);
                    temp_r5_1327 = Func_08015038(0xC45, 9, 9, 1);
                    var_r4_1337 = sp8;
                    while (Func_08015048() == 0)
                    {
                        Func_080030f8(1);
                    }
                    if (!((*((u32 *)0x03001C94)) & 1))
                    {
                        do
                        {
                            sp8 = var_r4_1337;
                            Func_080a1a40((unsigned long long)2, 0x92);
                            Func_080030f8(1);
                        }
                        while (!((*((u32 *)0x03001C94)) & 1));
                    }
                    sp8 = var_r4_1337;
                    Func_08015018(temp_r5_1327, 1);
                    Func_080aafb8(temp_r9_32);
                    new_var21 = 1;
                    Func_080030f8(new_var21);
                    temp_r5_1374 = Func_08015038(0xC46, 9, 9, new_var21);
                    var_r4_1384 = sp8;
                    while (Func_08015048() == 0)
                    {
                        Func_080030f8(1);
                    }
                    if (!((*((u32 *)0x03001C94)) & 1))
                    {
                        do
                        {
                            sp8 = var_r4_1384;
                            Func_080a1a40(2, 0x92);
                            Func_080030f8(1);
                        }
                        while (!((*((u32 *)0x03001C94)) & 1));
                    }
                    sp8 = var_r4_1384;
                    Func_08015018(temp_r5_1374, 1);
                    Func_080aafb8(temp_r9_32);
                    Func_080030f8(1);
                    (*(new_var132 = ((s32 *)(((s8 *)temp_r9_32) + 0x2128)))) = 0;
                    (*((s32 *)(((s8 *)temp_r9_32) + 0x212C))) = 0x10;
                    var_r4_907 = sp8;
                }
            break;
            case 17:
                case 19:
                    if ((*((s32 *)(((s8 *)temp_r9_32) + 0x2128))) == 0x5A)
                    {
                        var_fp_905 = 1;
                        do
                        {
                            (*((s32 *)(((s8 *)(new_var84 = (new_var20 = (new_var143 = temp_r9_32)))) + 0x2128))) = 0;
                        }
                        while (0);
                        var_r3_1124 = 0x15;
                        var_r2_1122 = temp_r9_32;
                        var_r2_1122 = (var_r2_1122 + 0x212C);
                        goto block_162;
                    }
            break;
            case 20:
                if ((*((s32 *)(((s8 *)temp_r9_32) + 0x2128))) == 0x5A)
                {
                    (*((s32 *)(((s8 *)temp_r9_32) + 0x2128))) = 0;
                    var_fp_905 = 0x20;
                    var_r4_907 = 0x20;
                    (*((s32 *)(new_var59 = (((s8 *)temp_r9_32) + 0x212C)))) = 0x16;
                }
            break;
            case 23:
                if ((*((s32 *)(((s8 *)temp_r9_32) + 0x2128))) == 0x3C)
                {
                    (*((s32 *)(((s8 *)(new_var67 = temp_r9_32)) + 0x2128))) = 0;
                    (*((s32 *)(new_var50 = (((s8 *)temp_r9_32) + 0x212C)))) = 0x19;
                    var_fp_905 = 2;
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
    new_var47 = 0xF00;
    new_var145 = ((u32)(new_var47 & sp28));
    temp_r7_1504 = (new_var145 >> 8);
    new_var105 = 5;
    temp_r5_1508 = (((u32)(0xE0 & sp28)) >> new_var105);
    sp8 = var_r4_907;
    temp_r6_1509 = (0x1F & sp28);
    var_r4_1515 = var_r4_907;
    if ((Func_08077210(temp_r7_1504, temp_r5_1508, temp_r6_1509) != 0) || (var_r4_1515 = sp8, Func_08077208(temp_r7_1504, temp_r5_1508, temp_r6_1509) != 0))
    {
        sp44 = 1;
    }
    sp24 = 1;
    new_var89 = 0;
    (*((u32 *)0x03001AF8)) = new_var89;
    if (sp44 == 0)
    {
        Func_080f9010(0x72);
        do
        {
            Func_08015278(*((s32 *)(((s8 *)ctx) + 0x30)));
            sp0 = 0x68;
        }
        while (0);
        Func_08015068(new_var88 = (*((s32 *)(new_var8 = (((s8 *)ctx) + 0x30)))), 0, 0x50, 0xD8);
        new_var19 = 0x30;
        Func_08015078(0xBBE, *((s32 *)(((s8 *)ctx) + new_var19)), 0, 0x60);
        sp48 = 1;
        goto loop_36;
    }
    if ((sp28 >> 0xF) != 0)
    {
        sp8 = var_r4_1515;
        new_var108 = sp28;
        Func_080f9010(new_var138 = 0xAF);
        ;
        temp_r5_1573 = (((u32)(0xF00 & new_var108)) >> 8);
        ;
        temp_r6_1574 = (((u32)(0xE0 & new_var108)) >> 5);
        Func_080771b8(temp_r5_1573, temp_r6_1574, 0x1F & new_var108);
        Func_080771c8(temp_r5_1573, temp_r6_1574, 0x1F & new_var108);
    }
    else
    {
        sp8 = var_r4_1515;
        Func_080f9010(0x8B);
        temp_r5_1598 = (((u32)(0xF00 & new_var108)) >> 8);
        new_var94 = 0xE0;
        temp_r6_1599 = (((u32)(new_var94 & new_var108)) >> 5);
        temp_r3_1597 = (0x1F & new_var108);
        Func_080771b0(temp_r5_1598, temp_r6_1599, temp_r3_1597);
        new_var112 = temp_r5_1598;
        Func_080771c0(new_var112, temp_r6_1599, temp_r3_1597);
    }
    Func_08077010(((u32)(0xF00 & sp28)) >> 8);
    Func_080aafb8(temp_r9_32);
    sp48 = 1;
    var_r4_907 = sp8;
    block_183:
        if ((mode == 0) && (0x200 & var_r4_907))
        {
            var_r5_1634 = 7;
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
                                var_r5_1634 = (0 - var_r5_1785);
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
                                new_var49 = (sp30 + 2);
                                Func_080ab1f4(*((s32 *)(((s8 *)ctx) + 0x30)), (new_var117 = (sp18 - (new_var44 = sp38))) + 1, new_var49, 6);
                                var_r5_1634 = 0xA;
                                Func_080f9010(new_var35 = 0x70);
                            }
                            else
                            {
                                temp_r5_1817 = 1;
                                temp_r5_1817 = (sp20 ^ temp_r5_1817);
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
                                    if (((s32)(*((u8 *)(((s8 *)temp_r9_32) + sp14)))) > 0)
                                    {
                                        var_sl_1853 = ((sp10 * 2) + temp_r9_32);
                                        do
                                        {
                                            temp_r7_1856 = (*var_sl_1853);
                                            var_sl_1853 += 1;
                                            temp_r4_1863 = (((u32)(temp_r7_1856 & 0xF00)) >> 8);
                                            var_r8_1869 = 0;
                                            temp_r6_1871 = (((u32)(temp_r7_1856 & 0xE0)) >> 5);
                                            temp_r5_1872 = (((short)temp_r7_1856) & ((unsigned int)0x1F));
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
                                                        new_var56 = (((u32)(temp_r7_1856 & 0xF00)) >> 8);
                                                        temp_r5_1909 = new_var56;
                                                        new_var106 = 0xE0;
                                                        new_var142 = 0x1F;
                                                        new_var77 = (temp_r7_1856 & new_var106);
                                                        ;
                                                        Func_080771b0(((u32)(0xF00 & temp_r7_1856)) >> 8, ((u32)new_var77) >> 5, new_var142 & temp_r7_1856);
                                                        new_var131 = (new_var142 & temp_r7_1856);
                                                        Func_080771c0(temp_r5_1909, ((u32)new_var77) >> 5, new_var131);
                                                        Func_08077010(temp_r5_1909);
                                                    }
                                                }
                                                else
                                                {
                                                    if ((temp_r7_1856 >> 0xF) != 0)
                                                    {
                                                        temp_r5_1937 = (((u32)(temp_r7_1856 & 0xF00)) >> 8);
                                                        new_var54 = (temp_r7_1856 & 0xE0);
                                                        temp_r3_1936 = temp_r7_1856;
                                                        temp_r3_1936 = (0x1F & temp_r3_1936);
                                                        new_var109 = ((u32)new_var54);
                                                        new_var109 = (new_var109 >> 5);
                                                        temp_r6_1938 = new_var109;
                                                        Func_080771b8(temp_r5_1937, temp_r6_1938, temp_r3_1936);
                                                        Func_080771c8(temp_r5_1937, temp_r6_1938, temp_r3_1936);
                                                        Func_08077010(temp_r5_1937);
                                                    }
                                                }
                                            }
                                            temp_r1_1953 = (sp40 + 1);
                                            new_var93 = temp_r1_1953;
                                            sp40 = new_var93;
                                        }
                                        while (new_var93 < ((s32)(*((u8 *)(((s8 *)temp_r9_32) + sp14)))));
                                    }
                                    temp_r1_1966 = (sp44 + 1);
                                    sp14 += 1;
                                    sp10 = (sp10 + 0xA);
                                    sp44 = temp_r1_1966;
                                    if (temp_r1_1966 < ((s32)(*((u8 *)(((s8 *)ctx) + 0x219)))))
                                    {
                                        goto loop_225;
                                    }
                                }
                                Func_080aafb8(temp_r9_32);
                                sp48 = 1;
                                block_241:
                                    if (0x40 & var_fp_905)
                                    {
                                        Func_080f9010(0x6F);
                                        if (!(4 & ((u8)statusPtr[sp38])))
                                        {
                                            new_var2 = (sp18 - sp38);
                                            sp4 = 0xF;
                                            new_var75 = (new_var2 + 1);
                                            new_var24 = 1;
                                            sp0 = new_var24;
                                            new_var78 = sp30;
                                            new_var78 = (new_var78 + 2);
                                            Func_080ab1f4(*((s32 *)(((s8 *)ctx) + 0x30)), new_var75, new_var78, 6);
                                            temp_r2_2035 = ((u8)(*(new_var17 = (&statusPtr)))[sp38]);
                                            if (!(4 & temp_r2_2035))
                                            {
                                                if (1 & temp_r2_2035)
                                                {
                                                    new_var66 = (-2);
                                                    statusPtr[sp38] = (new_var139 = (new_var66 & temp_r2_2035));
                                                    new_var116 = 0;
                                                    sp30 = new_var116;
                                                    if (1)
                                                    {
                                                    }
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
                                                    var_r1_2077 = (*((s8 *)(((s8 *)temp_r9_32) + (sp38 + 0xA0))));
                                                    if (var_r1_2077 == 0)
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
                                        if (0x80 & var_fp_905)
                                        {
                                            Func_080f9010(0x6F);
                                            if (!(4 & ((u8)statusPtr[sp38])))
                                            {
                                                new_var80 = (sp30 + 2);
                                                sp4 = 0xF;
                                                do
                                                {
                                                }
                                                while (0);
                                                sp0 = 1;
                                                Func_080ab1f4(*((s32 *)(((s8 *)(new_var15 = ctx)) + 0x30)), (sp18 - sp38) + 1, new_var80, 6);
                                                sp30 += 1;
                                                var_r1_2127 = (*((s8 *)(((s8 *)temp_r9_32) + (new_var11 = ((new_var14 = sp38) + 0xA0)))));
                                                if (var_r1_2127 == 0)
                                                {
                                                    new_var39 = 1;
                                                    var_r1_2127 = new_var39;
                                                }
                                                sp30 = Func_080aa538(sp30, (u8)var_r1_2127);
                                                temp_r2_2137 = ((u8)statusPtr[sp38]);
                                                if ((1 & temp_r2_2137) && (!(4 & temp_r2_2137)))
                                                {
                                                    new_var34 = (-2);
                                                    new_var34 = (new_var34 & temp_r2_2137);
                                                    statusPtr[sp38] = new_var34;
                                                    sp30 = 0;
                                                }
                                                else
                                                {
                                                    if (sp30 == 0)
                                                    {
                                                        temp_r2_2165 = ((u8)statusPtr[*(new_var5 = (new_var107 = (&sp38)))]);
                                                        if (2 & temp_r2_2165)
                                                        {
                                                            statusPtr[sp38] = (1 | temp_r2_2165);
                                                        }
                                                    }
                                                }
                                                sp2C = sp30;
                                                sp48 = 2;
                                            }
                                        }
                                        else
                                        {
                                            if (0x20 & var_fp_905)
                                            {
                                                Func_080f9010(0x6F);
                                                if (!(4 & ((u8)statusPtr[sp38])))
                                                {
                                                    sp0 = 1;
                                                    do
                                                    {
                                                        sp4 = 0xF;
                                                        new_var85 = (sp30 + 2);
                                                    }
                                                    while (0);
                                                    Func_080ab1f4(*((s32 *)(((s8 *)(new_var = ctx)) + 0x30)), (new_var151 = (sp18 - sp38)) + 1, new_var85, 6);
                                                }
                                                temp_r3_2211 = (sp38 - 1);
                                                sp38 = temp_r3_2211;
                                                sp38 = Func_080aa538(temp_r3_2211, *((u8 *)(((s8 *)(new_var127 = ctx)) + 0x219)));
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
                                            if (0x10 & var_fp_905)
                                            {
                                                Func_080f9010(0x6F);
                                                if (!(4 & ((u8)statusPtr[sp38])))
                                                {
                                                    sp0 = 1;
                                                    sp4 = 0xF;
                                                    Func_080ab1f4(*((s32 *)(((s8 *)ctx) + 0x30)), (new_var133 = (sp18 - sp38)) + 1, ((unsigned char)sp30) + 2, 6);
                                                }
                                                temp_r3_2279 = (sp38 + 1);
                                                sp38 = (new_var99 = temp_r3_2279);
                                                sp38 = Func_080aa538(temp_r3_2279, *((u8 *)(((s8 *)ctx) + 0x219)));
                                                if (((unsigned short)mode) == 0)
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
                                                            temp_r1_2308 = (sp44 + 1);
                                                            sp44 = temp_r1_2308;
                                                        }
                                                        while (temp_r1_2308 < ((s32)(*((u8 *)(((s8 *)ctx) + 0x219)))));
                                                    }
                                                }
                                                block_281:
                                                    sp30 = sp2C;
                                                new_var61 = 0xA0;
                                                new_var149 = 0xA0;
                                                var_r1_2319 = (*((s8 *)(((s8 *)temp_r9_32) + (sp38 + new_var149))));
                                                if (var_r1_2319 == 0)
                                                {
                                                    var_r1_2319 = 1;
                                                }
                                                sp30 = Func_080aa538(sp30, (u8)var_r1_2319);
                                                sp48 = 2;
                                                sp18 = (new_var64 = (sp38 * 8));
                                            }
                                        }
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
                        temp_r7_1696 = (((u32)((unsigned char)(0xF00 & sp28))) >> 8);
                        new_var134 = 5;
                        sp44 = 0;
                        temp_r6_1701 = ((u32)(0xE0 & sp28));
                        temp_r6_1701 = new_var134;
                        temp_r6_1701 = (temp_r6_1701 >> temp_r6_1701);
                        temp_r5_1702 = ((double)sp28);
                        temp_r5_1702 = 0x1F;
                        temp_r5_1702 = (temp_r5_1702 & temp_r5_1702);
                        if ((Func_08077210(temp_r7_1696, temp_r6_1701, temp_r5_1702) != 0) || (Func_08077208(temp_r7_1696, temp_r6_1701, temp_r5_1702) != 0))
                        {
                            sp44 = 1;
                        }
                    }
                    if (sp44 == 0)
                    {
                        new_var98 = 0x30;
                        Func_080f9010(0x72);
                        new_var97 = 0x30;
                        Func_08015278(*(new_var6 = ((s32 *)(((s8 *)ctx) + 0x30))));
                        sp0 = 0x68;
                        new_var46 = 0x50;
                        Func_08015068(*(new_var147 = ((s32 *)(((s8 *)ctx) + new_var98))), 0, new_var46, 0xD8);
                        new_var16 = (new_var42 = 0x60);
                        Func_08015078(0xBBE, *(new_var148 = ((s32 *)(((s8 *)ctx) + new_var97))), 0, new_var16);
                        goto loop_36;
                    }
                    if (mode == 1)
                    {
                        var_r5_1634 = 4;
                        if (mode & ((u8)statusPtr[sp38]))
                        {
                            if (sp38 == (*((s8 *)(((s8 *)ctx) + 0x1C))))
                            {
                                var_r5_1634 = 2;
                                do
                                {
                                    if (!(0x8000 & (*((u16 *)(((s8 *)ctx) + 0x178)))))
                                    {
                                        goto block_206;
                                    }
                                }
                                while (0);
                            }
                            else
                            {
                                var_r5_1634 = 3;
                            }
                        }
                    }
                    else
                    {
                        block_206:
                            var_r5_1634 = 1;
                    }
                    Func_080f9010(0x70);
                }
        }
    (*((s8 *)(((s8 *)ctx) + (mode + (new_var70 = 0x1C))))) = sp38;
    if (sp3C != (-1))
    {
        new_var96 = (sp34 + 0x178);
        temp_r2_2357 = (*((u16 *)(((s8 *)temp_r9_32) + (((new_var110 = (sp38 * 0xA)) + sp3C) * 2))));
        new_var25 = new_var96;
        new_var72 = (0x1F & temp_r2_2357);
        new_var140 = 2;
        (*((u16 *)(((s8 *)ctx) + new_var25))) = temp_r2_2357;
        temp_r0_2363 = 0x254;
        temp_r0_2363 = (mode + temp_r0_2363);
        (*((s8 *)((new_var135 = ((s8 *)ctx)) + temp_r0_2363))) = new_var72;
        (*((s8 *)(((s8 *)ctx) + (temp_r0_2363 + new_var140)))) = ((s8)(((u32)(0xE0 & temp_r2_2357)) >> 5));
        (*((s8 *)(new_var22 = (((s8 *)ctx) + (mode + 0x258))))) = ((s8)(((u32)(0xF00 & temp_r2_2357)) >> 8));
    }
    (*((u16 *)(((s8 *)ctx) + (sp34 + 0x174)))) = (sp38 + (sp30 * 0xA));
    return var_r5_1634;
}


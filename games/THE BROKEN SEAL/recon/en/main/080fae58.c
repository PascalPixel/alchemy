#include "types.h"

#define Func_080fae58 Func_080fae58

void Func_080fada0();
void Func_080fadf0();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

void Func_080fae58(s32 a0, s32 a1, s32 a2, s32 a3)
{
    u32 i;
    s32 p10;
    s32 p10b;
    s32 p10c;
    s32 p10d;
    s32 p10e;
    s32 p4b;
    s32 p6;
    s32 p8;
    s32 p8b;
    s32 p8c;
    s32 p8d;
    s32 p8e;
    s32 p9;
    s32 p9b;
    s32 p9c;
    s32 p9d;
    s32 p9e;
    s32 record;
    s32 r3;
    u8 *v4;
    s32 base6_1;
    s32 v2;
    u8 *v7;
    s32 v5;
    s32 v0;
    s32 v8;
    s32 v9;
    s32 v10;
    s32 v1;
    u8 *slot4;
    u8 *slot8;
    u8 *slot12;
    u8 *slot16;
    u8 *slot20;
    s32 slot0;
    s32 slot24;
    u8 *p4;

    slot4 = *(s32 *)0x03007ff0;
    if (slot4[10] != 0) {
        slot4[10] = (slot4[10] - 1);
    } else {
        slot4[10] = 14;
    }
    base6_1 = 1;
    p4 = *(s32 *)((s32)slot4 + 28);
    v4 = (s32)p4;
    L_080fae88:;
    p10 = base6_1 + 1;
    p9 = (s32)v4 + 64;
    if ((199 & v4[0]) == 0) {
        v9 = p9;
        v10 = p10;
    } else {
        switch (base6_1) {
        case 1:
            slot8 = 0x4000060;
            v7 = 0x4000062;
            slot12 = 0x4000063;
            slot16 = ((s32)slot8 + 4);
            v2 = ((s32)slot12 + 2);
            goto L_080faf18;
            /* fall through */
        case 2:
            slot8 = 0x4000061;
            v2 = 0x4000069;
            v7 = 0x4000068;
            goto L_080faf10;
            /* fall through */
        case 3:
            slot8 = 0x4000070;
            v7 = 0x4000072;
            slot12 = 0x4000073;
            slot16 = ((s32)slot8 + 4);
            v2 = ((s32)slot12 + 2);
            goto L_080faf18;
            break;
        }
        slot8 = 0x4000071;
        v2 = 0x4000079;
        v7 = 0x4000078;
        L_080faf10:;
        slot12 = v2;
        slot16 = ((s32)slot8 + 11);
        v2 = ((s32)slot12 + 4);
        L_080faf18:;
        slot20 = v2;
        slot0 = slot4[10];
        p8 = slot12[0];
        if ((128 & v4[0]) != 0) {
            p10b = base6_1 + 1;
            p9b = (s32)v4 + 64;
            if (((u32)((64 & v4[0]) << 24) >> 24) != 0) {
                v9 = p9b;
                v10 = p10b;
                goto L_080fb032;
            }
            v5 = ((u32)((64 & v4[0]) << 24) >> 24);
            v4[0] = 3;
            v4[29] = 3;
            slot24 = 64;
            Func_080fadf0((s32)v4);
            switch (base6_1) {
            case 1:
                slot8[0] = v4[31];
                /* fall through */
            case 2:
                v7[0] = (v4[30] + (*(s32 *)((s32)v4 + 36) << 6));
                goto L_080fafe0;
                /* fall through */
            case 3:
                if (*(s32 *)((s32)v4 + 36) != *(s32 *)((s32)v4 + 40)) {
                    slot8[0] = r3;
                    *(s32 *)0x04000090 = *(s32 *)(*(s32 *)((s32)v4 + 36));
                    *(s32 *)0x04000094 = *(s32 *)(*(s32 *)((s32)v4 + 36) + 4);
                    *(s32 *)0x04000098 = *(s32 *)(*(s32 *)((s32)v4 + 36) + 8);
                    *(s32 *)0x0400009c = *(s32 *)(*(s32 *)((s32)v4 + 36) + 12);
                    *(s32 *)((s32)v4 + 40) = *(s32 *)((s32)v4 + 36);
                }
                slot8[0] = v5;
                v7[0] = v4[30];
                if (v4[30] != 0) {
                    v0 = 192;
                    v8 = p8;
                    goto L_080fafee;
                }
                v4[26] = -128;
                v8 = p8;
                goto L_080faff0;
                break;
            }
            v7[0] = v4[30];
            slot16[0] = (*(s32 *)((s32)v4 + 36) << 3);
            L_080fafe0:;
            p8b = (v4[4] + 8);
            v0 = v4[30];
            v8 = p8b;
            if (v4[30] != 0) {
                v0 = 64;
            }
            L_080fafee:;
            v4[26] = v0;
            L_080faff0:;
            v10 = p10c;
            v4[11] = v4[4];
            v9 = p9c;
            p10c = base6_1 + 1;
            p9c = (s32)v4 + 64;
            if ((255 & v4[4]) == 0) {
                v9 = p9c;
                v10 = p10c;
                goto L_080fb146;
            }
            v4[9] = 0;
        } else {
            if ((4 & v4[0]) != 0) {
                v0 = (v4[13] - 1);
                v4[13] = (v4[13] - 1);
                p10d = base6_1 + 1;
                v10 = p10d;
                p9d = (s32)v4 + 64;
                v9 = p9d;
                if (((v0 & 255) << 24) > 0) {
                    v8 = p8;
                    v9 = p9d;
                    v10 = p10d;
                    goto L_080fb186;
                }
                L_080fb032:;
                Func_080fada0(((u32)(base6_1 << 24) >> 24));
                v4[0] = 0;
                v10 = p10e;
                goto L_080fb284;
            }
            p10e = base6_1 + 1;
            p9e = (s32)v4 + 64;
            if ((64 & v4[0]) != 0) {
                if ((3 & v4[0]) == 0) {
                    goto L_080fb080;
                }
                v4[0] &= 252;
                v4[11] = v4[7];
                if ((255 & v4[7]) == 0) {
                    goto L_080fb0b2;
                }
                {
                    u8 value = *(volatile u8 *)&v4[29];
                
                    v4[29] = (u8)(value | 1);
                }
                v8 = p8c;
                v9 = p9e;
                v10 = p10e;
                if (base6_1 == 3) {
                    v8 = p8;
                    v9 = p9e;
                    v10 = p10e;
                    goto L_080fb174;
                }
                v8 = p8;
                v9 = p9e;
                p8c = v4[7];
            } else {
                L_080fb080:;
                if (v4[11] == 0) {
                    if (base6_1 == 3) {
                        {
                            u8 value = *(volatile u8 *)&v4[29];
                        
                            v4[29] = (u8)(value | 1);
                        }
                    }
                    Func_080fadf0((s32)v4);
                    if ((3 & v4[0]) == 0) {
                        v0 = (v4[9] - 1);
                        v4[9] = (v4[9] - 1);
                        v8 = p8;
                        if (((v0 & 255) << 24) <= 0) {
                            L_080fb0b2:;
                            v4[9] = (((v4[12] * v4[10]) + 255) >> 8);
                            if (((((v4[12] * v4[10]) + 255) >> 8) << 24) == 0) {
                                goto L_080fb032;
                            }
                            {
                                u8 value = *(volatile u8 *)&v4[0];
                            
                                v4[0] = (u8)(value | 4);
                            }
                            {
                                u8 value = *(volatile u8 *)&v4[29];
                            
                                v4[29] = (u8)(value | 1);
                            }
                            if (base6_1 == 3) {
                                v8 = p8;
                                v9 = p9e;
                                v10 = p10e;
                                goto L_080fb186;
                            }
                        v0 = v4[7];
                        v9 = p9e;
                        v10 = p10e;
                            v8 = 8;
                            v8 = 8;
                            v9 = p9e;
                            v10 = p10e;
                            goto L_080fb186;
                        }
                    } else {
                            v0 = 7;
                            v8 = p8;
                            v9 = p9e;
                            v10 = p10e;
                        if ((3 & v4[0]) == 1) {
                            L_080fb0ea:;
                            v4[9] = v4[25];
                        } else {
                            if ((3 & v4[0]) == 2) {
                                v0 = (v4[9] - 1);
                                v4[9] = (v4[9] - 1);
                                v8 = p8;
                                if (((v0 & 255) << 24) <= (v4[25] << 24)) {
                                    L_080fb10a:;
                                    if (v4[6] == 0) {
                                        v4[0] &= 252;
                                        goto L_080fb0b2;
                                    }
                                    v4[0] = (v4[0] - 1);
                                    {
                                        u8 value = *(volatile u8 *)&v4[29];
                                    
                                        v4[29] = (u8)(value | 1);
                                    }
                                    if (base6_1 == 3) {
                                        goto L_080fb0ea;
                                    }
                                v0 = v4[5];
                                v9 = p9e;
                                v10 = p10e;
                                    v8 = 8;
                                    goto L_080fb0ea;
                                }
                                v8 = p8;
                                v9 = p9e;
                                v10 = p10e;
                            } else {
                                v0 = (v4[9] + 1);
                                v4[9] += 1;
                                if ((u32)(v0 & 255) >= v4[10]) {
                                    L_080fb146:;
                                    v8 = p8;
                                    v9 = p9e;
                                    v10 = p10e;
                                    v4[0] = (v4[0] - 1);
                                    v4[11] = v4[5];
                                    if ((255 & v4[5]) == 0) {
                                        goto L_080fb10a;
                                    }
                                    {
                                        u8 value = *(volatile u8 *)&v4[29];
                                    
                                        v4[29] = (u8)(value | 1);
                                    }
                                    v4[9] = v4[10];
                                    if (base6_1 == 3) {
                                        goto L_080fb174;
                                    }
                                    p8d = v4[5];
                                    v8 = p8d;
                                    v8 = p8d;
                                    goto L_080fb174;
                                }
                                v0 = v4[4];
                            }
                        }
                    }
                    v4[11] = v0;
                }
            }
        }
        L_080fb174:;
        v4[11] = (v4[11] - 1);
        if (slot0 == 0) {
            slot0 = (slot0 - 1);
            goto L_080fb080;
        }
        L_080fb186:;
        if ((2 & v4[29]) != 0) {
            if (base6_1 <= 3) {
                if ((8 & v4[1]) != 0) {
                    if (*(u8 *)0x04000089 <= 63) {
                        v0 = (*(s32 *)((s32)v4 + 32) + 2);
                        v1 = 0x7fc;
                    } else {
                        if (*(u8 *)0x04000089 > 127) {
                            goto L_080fb1c6;
                        }
                        v0 = (*(s32 *)((s32)v4 + 32) + 1);
                        v1 = 0x7fe;
                    }
                    *(s32 *)((s32)v4 + 32) = (v0 & v1);
                }
            }
            L_080fb1c6:;
            if (base6_1 != 4) {
                slot16[0] = *(s32 *)((s32)v4 + 32);
            } else {
                slot16[0] = (*(s32 *)((s32)v4 + 32) | (8 & slot16[0]));
            }
            v0 = (v4[33] + (192 & v4[26]));
            v4[26] = (v4[33] + (192 & v4[26]));
            slot20[0] = (v0 & 255);
        }
        if ((1 & v4[29]) != 0) {
            *(u8 *)0x04000081 = ((*(u8 *)0x04000081 & ~v4[28]) | v4[27]);
            if (base6_1 == 3) {
                slot12[0] = *(u8 *)((v4[9] + 0x80fba04));
                if ((128 & v4[26]) == 0) {
                    goto L_080fb284;
                }
                slot8[0] = 128;
                slot20[0] = v4[26];
                v4[26] &= 127;
            } else {
                p8e = (v8 & 15);
                slot12[0] = ((v4[9] << 4) + p8e);
                slot20[0] = (v4[26] | 128);
                if (base6_1 == 1) {
                    if ((8 & slot8[0]) == 0) {
                        slot20[0] = (v4[26] | 128);
                    }
                }
            }
        }
        L_080fb284:;
        v4[29] = 0;
    }
    p6 = v10;
    base6_1 = p6;
    p4b = v9;
    v4 = p4b;
    if (base6_1 <= 4) {
        goto L_080fae88;
    }
    p8 = a3;
    p9 = (s32)v4;
}

/* Save selection, link transfer and password display. */
#include "types.h"

#define SaveMenu_Run Func_020003cc

extern u8 Data_02000000[];
extern u8 Data_02000240[];
extern u8 Data_02001100[];
extern u8 Data_02002080[];
extern u8 Data_02002224[];
extern u8 Data_02002226[];
extern u8 Data_02002228[];
extern u8 Data_0200222a[];
extern u8 Data_03001ebc[];
void Func_02000438();
s32 Func_0200096c();
s32 Func_020009e0();
void Func_02000f0c();
s32 Func_0200171a();
void Func_020017b2();
void Func_020017c0();
void Func_020017ca();
s32 Func_020017d2();
void Func_020017ee();
void Func_02001804();
void Func_02001812();
void Func_02001814();
s32 Func_02001816();
void Func_0200939c();
s32 Func_0200938c(s32, s32);
s32 Func_02001822();
s32 Func_02001830();
void Func_0200184a();
void Func_020094ac(void);
void Func_020093dc();
void Func_020093ac();
s32 Func_020018b6();
void Func_020018c0();
s32 Func_020018c8();
s32 Func_020094c4(s32);
s32 Func_02009494(s32);
void Func_020018fa();
void Func_0200190c();
void Func_02001912();
void Func_0200192a();
void Func_02001938();
s32 Func_02001940();
void Func_02001944();
void Func_02001956();
s32 Func_02001978();
void Func_020019c2();
s32 Func_020019ca();
void Func_020019d2();
void Func_020019dc();
void Func_020019f0();
void Func_020019fc();
void Func_02001a16();
void Func_02001a1c();
s32 Func_02001a2e();
void Func_02001a38();
s32 Func_02001a6e();
void Func_0200945c(s32);
void Func_02009464(s32);
void Func_02001aa2();
void Func_02001ab4();
void Func_02001af0();
void Func_02001af2();
void Func_02001b00();
s32 Func_02001b10();
void Func_02001b58();
s32 Func_02001b84();
s32 Func_02001b8e();
void Func_02001b9a();
void Func_02001b9e();
s32 Func_02001bb0();
s32 Func_02001bb4();
void Func_02009474(s32);
void Func_02001bc2();
void Func_02001bc8();
void Func_02001bca();
void Func_02001bcc();
void Func_0200948c(void);
void Func_02001bd2();
void Func_02001bd6();
void Func_02001bd8();
void Func_02001bde();
void Func_02001bec();
void Func_02001bf0();
void Func_02001bf6();
s32 Func_02001bf8();
void Func_02001c34();
void Func_02001c42();
void Func_02001c4a();
void Func_02001c56();
void Func_02001c72();
s32 Func_02001c80();
s32 Func_02001c94();
void Func_02001ca0();
void Func_02001cae();
void Func_02001cb6();
void Func_02001cb8();
s32 Func_02001cbe();
void Func_02009374();
void Func_02001d1e();
void Func_02001d2e();
void Func_02001d36();
void Func_02001d44();
s32 Func_02009314(s32, s32);
void Func_02001d54();
void Func_02001d6e();
void Func_020094cc(s32);
s32 Func_020093d4(s32);
s32 Func_02001dae();
s32 Func_02001dce();
s32 Func_02001df4();
void Func_02001e06();
s32 Func_02001e12();
void Func_02001e32();
void Func_02001e44();
void Func_02001e50();
s32 Func_02001e6e();
void Func_02001e76();
void Func_02001ece();
void Func_02001ee2();
void Func_02001efe();
void Func_02001f1a();
s32 Func_02001f3c();
s32 Func_02001f4a();
s32 Func_02001f4e();
s32 Func_02001f6e();
s32 Func_02001f72();
void Func_02001f92();
void Func_02001fa2();
void Func_02001faa();
void Func_02001fac();
void Func_02001fb4();
void Func_020093e4(s32, s32, s32, s32, s32);
void Func_02002012();
void Func_02002030();
void Func_0200213c();
void Func_02002150();
void Func_02002166();
void Func_02002172();
void Func_0200217a();

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

s32 SaveMenu_Run(void)
{
    s32 page_count;
    s32 rec;
    s32 encoded_size;
    s32 page_window;
    s32 rec5;
    s32 rec7;
    s32 rec8;
    s32 record;
    s32 value;
    s32 base3_2000240;
    s32 v5;
    s32 v7;
    s32 base2_2000240;
    s32 count;
    s32 base5_2000240;
    s32 v6;
    s32 link_message;
    s32 v1;
    s32 v3;
    s32 v4;
    s32 missing_link_frames;
    s32 base7_3001c94;
    s32 none;
    s32 page_message;
    s32 page;
    s32 x;
    s32 started;
    s32 slot4;
    s32 heading_window;
    s32 help_window;
    u8 save_data[200];
    u8 password[320];
    u8 glyph[4];

    started = 0;
    Func_02000438();
    Value2(Func_0200171a, 0x20081fd, 0xc80);
    *(s32 *)((*(u8 **)Data_03001ebc + 0x1c0)) = started;
    Func_020094ac();
    Func_020018c0();
    if (*(s16 *)(Data_02000240 + 0x1c2) == 2) {
        do {
            Func_020017b2(0x7, 5);
            rec = Value1(Func_020094c4, 1);
            Func_020017ee();
            if (rec != 0) {
                break;
            }
            Func_020017ca((0x7 + 1), 1);
            base3_2000240 = (s32)Data_02000240;
            *(u8 *)((base3_2000240 + 0x20f)) = 1;
            rec = Func_02001830();
            v5 = 0x7;
            v7 = rec;
        } while (v7 == -1);
        Func_02009494(60);
        Func_0200190c();
        Func_0200192a(17);
        Call1(Func_020018fa, 0x12c);
        Func_02001912(0x2, 72);
        goto L_Return;
        L_02000474:;
        Func_02001944(112);
        Func_0200939c(page_window);
        Func_02001812(page_window, 2);
        Value2(Func_0200938c, help_window, 2);
        Value2(Func_02001822, heading_window, 2);
        Func_020017c0(1);
    } else {
        *(u8 *)0x03001ca0 = started;
    }
    L_020004a8:;
    do {
        rec8 = Value0(Func_020018b6);
        if (rec8 < 0) {
            if (*(u8 *)0x03001f54 != 0) {
                base3_2000240 = (s32)Data_02000240;
                *(u8 *)((base3_2000240 + 0x22a)) = 1;
                *(u8 *)0x03001d08 = 1;
                Func_020093dc(0xa, 1, 8);
            }
        }
        if (rec8 == 0) {
            if (started == 0) {
                Func_02001804(30);
                Value2(Func_02001816, 0x2008155, 0xc80);
                Func_02001814(1);
                started = 1;
            }
        }
        if (rec8 > 0) {
            rec8 = Value0(Func_020018c8);
        } else {
            rec8 = 0;
        }
        if (rec8 != 0) {
            goto L_0200057a;
        }
        Func_02001956();
        base3_2000240 = (s32)Data_02000240;
        base2_2000240 = (s32)Data_02000240;
        Func_02001938(*(u8 *)((base3_2000240 + 0x205)), *(u8 *)((base2_2000240 + 0x206)));
        v7 = 0;
        v5 = 1;
        L_02000522:;
        Func_0200184a(6);
        rec8 = Func_02001940(v7);
        if (rec8 != -1) {
            goto L_02000540;
        }
    } while (v7 == 0);
    v7 = (v7 - 1);
    goto L_02000522;
    L_02000540:;
    if (*(s16 *)(0x20096b2) != 0) {
        v5 = 4;
    }
    if (*(s16 *)(0x20096b4) != 0) {
        v5 = 7;
    }
    v7 = (v7 + 1);
    if (v7 < v5) {
        goto L_02000522;
    }
    Func_020019dc();
    *(u16 *)(Data_02000240 + 0x1c0) = 0x8;
    {
        s32 shown = 20;

        *(u16 *)(Data_02000240 + 0x1c2) = shown;
    }
    goto L_02000c96;
    L_0200057a:;
    if (rec8 != 1) {
    } else {
        rec8 = Value1(Func_02001978, 1);
        if (rec8 == -1) {
            goto L_020004a8;
        }
        Call1(Func_020019f0, 0x109);
        base5_2000240 = (s32)Data_02000240;
        v5 = base5_2000240;
        v5 = base5_2000240;
        base3_2000240 = (s32)Data_02000240;
        base3_2000240 = (s32)Data_02000240;
        Func_020019c2(*(u8 *)((base3_2000240 + 0x205)), *(u8 *)((base3_2000240 + 0x206)));
        record = Func_02001a2e();
        if (*(s32 *)base5_2000240 != record) {
            *(u16 *)(Data_02000240 + 0x1c0) = *(u16 *)(Data_02000240 + 0x1c4);
            *(u16 *)(Data_02000240 + 0x1c2) = *(u16 *)(Data_02000240 + 0x1c6);
            Call1(Func_02001a38, 0x109);
        } else {
            if ((*(s32 *)0x03001ae8 & 0x208) == 0x208) {
                if (Value0(Func_0200096c) != 0) {
                    goto L_02000664;
                }
                *(u16 *)(Data_02000240 + 0x1c0) = *(u16 *)(Data_02000240 + 0x1c4);
                *(u16 *)(Data_02000240 + 0x1c2) = *(u16 *)(Data_02000240 + 0x1c6);
                Func_02009464(0x109);
                Func_0200945c(0x13e);
            } else {
                if (*(s32 *)(base5_2000240 + 4) != *(s32 *)Data_02001100) {
                    Value2(Func_020019ca, 0x4, 9);
                    Func_020019d2((0x4 + 1), 13);
                    if (Value4(Func_02001a6e, 1, 0, 0, 0) != 0) {
                        Func_02001a16();
                        goto L_020004a8;
                    }
                    Func_02001a1c();
                    if (Value0(Func_020009e0) != 0) {
                        L_02000664:;
                        Func_020019fc(0x6, 9);
                        goto L_020004a8;
                    }
                    *(u16 *)(Data_02000240 + 0x1c0) = *(u16 *)(Data_02000240 + 0x1c4);
                    *(u16 *)(Data_02000240 + 0x1c2) = *(u16 *)(Data_02000240 + 0x1c6);
                    Call1(Func_02001af2, 0x109);
                    Call1(Func_02001af0, 0x13f);
                }
            }
        }
        Call1(Func_02001b00, 0x106);
        goto L_02000c96;
    }
    if (rec8 == 2) {
        Func_02001aa2();
        goto L_020004a8;
    }
    if (rec8 == 3) {
        Func_02001ab4();
        goto L_020004a8;
    }
    if (rec8 == 4) {
        rec8 = Func_02001b10(4);
        if (rec8 == -1) {
            goto L_020004a8;
        }
        v5 = 0x2000240;
        v5 = 0x2000240;
        v5 = 0x2000240;
        *(s32 *)(Data_02000240 + 0x1f4) = 0;
        if (Value1(Func_02001b8e, 0x952) != 0) {
            Func_0200948c();
            Func_02009474(0);
            Func_02001bc2(1);
            Func_02001bc8(2);
            Func_02009474(3);
            Func_02001bcc(0);
            Func_02001bd2(1);
            Func_02001bd8(2);
            Func_02001bde(3);
        }
        base3_2000240 = (s32)Data_02000240;
        base3_2000240 = (s32)Data_02000240;
        Func_02001b9e(*(u8 *)((base3_2000240 + 0x205)), *(u8 *)((base3_2000240 + 0x206)));
        Call1(Func_02001bec, 0x109);
        Func_02009464(0x106);
        Func_0200945c(0x17e);
        *(u8 *)0x03001ca0 = 1;
        Func_02001c4a(0xbe, 1);
        goto L_02000c96;
        v6 = rec8;
        v7 = 1;
        L_020007aa:;
        v7 = 1;
        goto L_020008e0;
        L_020007ae:;
        v4 = 0;
        goto L_0200095e;
    } else {
        if (rec8 != 5) {
            goto L_020004a8;
        }
        do {
            rec8 = Func_02001bb0(5);
            if (rec8 == -1) {
                goto L_020004a8;
            }
            base3_2000240 = (s32)Data_02000240;
            base2_2000240 = (s32)Data_02000240;
            Func_02001bf6(*(u8 *)((base3_2000240 + 0x205)), *(u8 *)((base2_2000240 + 0x206)));
            L_020007dc:;
            rec8 = Func_02001bb4(0);
            v6 = rec8;
        } while (v6 == -1);
        if (v6 != 1) {
            goto L_020009cc;
        }
        rec8 = Func_02001b84(6, 5, 18, 8, 2);
        link_message = 0xc83;
        v6 = rec8;
        Func_020093ac(link_message, v6, 0, 4);
        Func_02001bca((link_message + 1), v6, 0, 16);
        Func_02001bd6((link_message + 3), v6, 0, 36);
        Func_02001b9a();
        Func_02001b58(10);
        *(u16 *)Data_02002224 = 0x30;
        *(u16 *)Data_02002226 = 0x30;
        *(u16 *)Data_02002228 = 0x30;
        *(u16 *)Data_0200222a = 0x30;
        v5 = 3;
        v7 = 0;
        v1 = 0;
        v3 = 0x2002024;
        do {
            v1 = (v1 + 1);
            *(u16 *)(v3) = 0x30;
            *(u16 *)(v3 + 2) = 0x30;
            *(u16 *)(v3 + 4) = 0x30;
            *(u16 *)(v3 + 6) = 0x30;
            v3 = (v3 + 24);
        } while (v1 != 4);
        while ((*(s32 *)0x03001c94 & 2) == 0) {
            if ((3 & *(u16 *)0x03001f64) == 3) {
                u32 peer = ((*(volatile u32 *)0x04000128 >> 4) & 3) ^ 1;
                u16 *received = (u16 *)(0x02002024 + peer * 24);

                if (received[0] == 85) {
                    if (received[1] == 86) {
                        if (received[2] == 84) {
                            if (received[3] == 83) {
                                goto L_020007aa;
                            }
                        }
                    }
                }
            }
            Func_02001bf0(1);
        }
        Func_020094cc(113);
        L_020008e0:;
        Func_02001c72(v6, 2);
        if (v7 == 0) {
            goto L_020007dc;
        }
        rec8 = Func_02001c80(5, 10, 20, 4, 2);
        v6 = rec8;
        Call4(Func_02001cb6, 0xc85, v6, 0, 4);
        Func_02001c34(10);
        Value2(Func_02001c94, (s32)Data_02000000, 0x1004);
        Func_02001c42(10);
        missing_link_frames = 0;
        count = 0;
        v1 = 3;
        v4 = 1;
        for (;;) {
            if (count > 0x927bf) {
                break;
            }
            missing_link_frames = (missing_link_frames + 1);
            if ((v1 & *(u16 *)0x03001f64) == v1) {
                missing_link_frames = 0;
            }
            if (missing_link_frames == 10) {
                goto L_020007ae;
            }
            if (*(s32 *)Data_02002080 == 0) {
                break;
            }
            Func_02001c56(1);
            count = (count + 1);
        }
    }
    L_0200095e:;
    if (v4 == 0) {
        Func_0200939c(v6, v1);
        Call4(Func_02001d1e, 0xc87, v6, 0, 4);
            base7_3001c94 = 0x3001c94;
        v5 = 1;
        do {
            Func_02001ca0(1);
            v7 = base7_3001c94;
        } while ((*(s32 *)base7_3001c94 & 1) == 0);
    }
    Func_02001cae(10);
        Func_02009374();
    Func_02001cb8(10);
    Func_02001d36(v6);
    Func_02001d2e(v6, 2);
    goto L_020004a8;
    L_020009a6:;
    Func_02001e76(113);
    Func_0200939c(page_window);
    Func_02001d44(page_window, 2);
    Func_0200938c(help_window, 2);
    Func_02001d54(heading_window, 2);
    goto L_020009d2;
    L_020009cc:;
    if (v6 != 0) {
    } else {
        L_020009d2:;
        rec8 = Value1(Func_020093d4, 1);
        v6 = rec8;
        if (v6 == -1) {
            goto L_020007dc;
        }
        encoded_size = Value3(Func_020017d2, 0, v6, save_data);
        value = Func_02001cbe(encoded_size, save_data);
        *(u8 *)(save_data + encoded_size) = ((u32)(value << 16) >> 24);
        *(u8 *)(save_data + (encoded_size + 1)) = ((value << 16) >> 16);
        encoded_size = Value3(Func_02001bf8, save_data, (encoded_size + 2), password);
        v6 = 2;
        Func_02001d6e();
        page_window = Func_02001dae(5, 4, 20, 12, v6);
        none = 0;
        page_count = (Func_02009314(encoded_size, 50) + 1);
        record = Func_02001dce(10, 0, 10, 4, v6);
        page_message = 0xc82;
        heading_window = record;
        Func_02001e06(page_message, heading_window, 6, 4);
        v7 = 1;
        if (page_count == 1) {
            record = Func_02001df4(5, 16, 20, 3, v6);
            help_window = record;
            Call4(Func_02001e32, 0xc80, help_window, 80, 0);
        } else {
            record = Func_02001e12(1, 16, 28, 3, v6);
            help_window = record;
            Call4(Func_02001e50, 0xc81, help_window, 0, 0);
        }
        Call1(Func_02001ece, 0x6006000);
        Func_02001e44(page_window);
        page = none;
        L_02000aaa:;
        Call1(Func_02001ee2, 0x6002500);
        if ((*(s32 *)0x03001c94 & 2) != 0) {
            goto L_020009a6;
        }
        if ((*(s32 *)0x03001c94 & 1) != 0) {
            page = (page + 1);
            v7 = 1;
            if (page == page_count) {
                goto L_02000474;
            }
            Func_02001fa2(111);
        } else {
            if ((*(s32 *)0x03001c94 & 32) != 0) {
                if (page_count <= 1) {
                    goto L_02000b30;
                }
                Func_020094cc(111);
                rec5 = Func_02001e6e(((page + page_count) - 1), page_count);
            } else {
                L_02000b30:;
                if ((*(s32 *)0x03001c94 & 16) == 0) {
                    goto L_02000b58;
                }
                if (page_count <= 1) {
                    goto L_02000b58;
                }
                Func_02002012(111);
                rec5 = Func_02001e6e(((page + page_count) + 1), page_count);
            }
            v7 = 1;
            page = rec5;
        }
        L_02000b58:;
        if (v7 != 1) {
            v5 = page_message;
        } else {
            v7 = 0;
            v5 = 2;
            Func_02001efe(page_window);
            do {
                Func_02001f1a(page_window, 0, v5, 18, v5);
                v5 = (v5 + 2);
                v7 = (v7 + 1);
            } while (v7 != 4);
            if (page_count > 1) {
                    count = 0;
                if (page_count != 0) {
                    v6 = 0;
                    v5 = (-page_count + 18);
                    do {
                        value = count + 0xf301;
                        if (count == page) {
                            value = count + 0xf30b;
                        }
                        Func_02001f92(page_window, value, v5, -1, v6);
                        count = (count + 1);
                        v5 = (v5 + 1);
                    } while (count != page_count);
                }
                v5 = 0;
                Func_02001fac(page_window, 0xf128, (17 - (s32)page_count), -1, 0);
                Func_020093e4(page_window, 0xf129, 18, -1, 0);
                *(u8 *)((*(s32 *)0x03001e8c + 0xea3)) |= (2 << ((u32)*(u16 *)(page_window + 14) >> 2));
            }
            v6 = (page * 50);
            v6 = (v6 + 50);
            if (v6 > encoded_size) {
                v6 = encoded_size;
            }
            if ((page * 50) != v6) {
                v3 = 50;
                v4 = 0;
                v7 = (page * 50);
                do {
                    v3 = password;
                    v3 = *(u8 *)(v3 + v7);
                    slot4 = v4;
                    Func_02000f0c((63 & v3), glyph);
                    record = Func_02001f3c(v7, 10);
                    v4 = slot4;
                    if (record > 4) {
                        rec7 = Func_02001f4a(v4, 10);
                        v4 = slot4;
                        record = Func_02001f4e(v4, 10);
                        x = rec7 * 12 + 18;
                        v3 = record;
                        v3 = (v3 << 4);
                    } else {
                        slot4 = v4;
                        rec7 = Func_02001f6e(v4, 10);
                        v4 = slot4;
                        record = Func_02001f72(v4, 10);
                        x = rec7 * 12 + 8;
                        v3 = record;
                        v3 = (v3 << 4);
                    }
                    v3 = (v3 + 2);
                    Func_02002030(glyph, page_window, x, v3);
                    v4 = slot4;
                    v7 = (v7 + 1);
                    v4 = (v4 + 1);
                } while (v7 != v6);
            }
            v7 = 0;
        }
        Func_02001faa(1);
        goto L_02000aaa;
    }
    Call1(Func_02001fb4, 0x12c);
    goto L_020004a8;
    L_02000c96:;
    *(u16 *)((*(u8 **)Data_03001ebc + 0x170)) = 0x3e7;
    Func_0200213c(30);
    Func_0200217a(17);
    Func_02002166();
    Func_02002172();
    Func_02002150(60);
    L_Return:;
    return 0;
}

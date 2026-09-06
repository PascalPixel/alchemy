#include "types.h"

#define RunSceneStateSequence Func_020001d8

void Func_02000cba();
s32 Func_02001170();
void Func_0200190a();
s32 Func_02001b4e();
u8 *Func_02001b64();
s32 Func_02001b6a();
void Func_02001b6e();
u8 *Func_02001b6e_a();
void Func_02001b76();
u8 *Func_02001b76_a();
u8 *Func_02001b82();
s32 Func_02001bbe();
void Func_02001be0();
void Func_02001be2();
void Func_02001bfe();
void Func_02001c14();
void Func_02001c16();
void Func_02001c1c();
void Func_02001c24();
void Func_02001c24_a();
void Func_02001c36();
void Func_02001c46();
void Func_02001c4e();
void Func_02001c4e_a();
void Func_02001c58();
s32 Func_02001c78();
void Func_02001c8a();
void Func_02001c96();
void Func_02001ca2();
void Func_02001cae();
void Func_02001cbe();
void Func_02001cc4();
void Func_02001ccc();
void Func_02001cce();
void Func_02001cce_a();
void Func_02001cec();
void Func_02001cee();
void Func_02001cfa();
void Func_02001d02();
void Func_02001d1a();
void Func_02001d36();
void Func_02001d3a();
void Func_02001d3a_a();
void Func_02001d46();
void Func_02001d4c();
void Func_02001d4e();
void Func_02001d4e_a();
void Func_02001d54();
void Func_02001d5c();
void Func_02001d84();
void Func_02001d84_a();
void Func_02001dae();
void Func_02001db8();
void Func_02001dbc();
void Func_02001dd0();

s32 RunSceneStateSequence(void)
{
    s16 *state = (s16 *)0x02000240;
    u8 *work;
    u8 *actor;
    s32 delta;
    s32 topic;
    s8 *entry;
    s8 stop;

    if (state[224] == 0xbd) {
        work = *(u8 **)0x03001ebc;
        *(u32 *)(work + 0x1c0) = 0x100;
        *(volatile u16 *)0x04000050 = 0x3f42;
        *(volatile u16 *)0x04000052 = 0x080c;
        Func_02001b6e(24, 2);
        Func_02001b76(25, 2);
        actor = Func_02001b64(24);
        *(s32 *)(actor + 24) = -0x10000;
        actor = Func_02001b6e_a(25);
        *(s32 *)(actor + 24) = -0x10000;
        actor = Func_02001b76_a(24);
        actor[35] = 2;
        actor = Func_02001b82(25);
        actor[35] = 2;
        Func_02001be2();
        if (state[225] != 1)
            return 0;
        Func_0200190a();
        if (Func_02001b4e(0x200) == 0)
            return 0;
        *(volatile u16 *)0x04000050 = 0x3f42;
        *(volatile u16 *)0x04000052 = 0x1000;
        return 0;
    }

    if (Func_02001b6a(0x950) != 0)
        Func_02001be0(17, 0, 0);
    *(u8 *)0x03001d18 = 1;
    work = *(u8 **)0x03001ebc;
    *(u32 *)(work + 0x1c0) = 0x209;

    if (state[225] == 10) {
        Func_02001c1c(8, 1);
        Func_02001c24(9, 2);
    }
    if ((u16)state[225] == 13 && Func_02001bbe(0x109) == 0) {
        Func_02001bfe();
        Func_02001c46(8, 1);
        Func_02001c4e(9, 2);
        Func_02001c8a();
        Func_02001c96();
        Func_02001c14(10);
        Func_02001c4e_a(0, 120, 112);
        Func_02001c24_a(20);
        delta = *(s32 *)0x02000250 - *(s32 *)0x02001000;
        if (delta > 0) {
            if (delta > 20000)
                Func_02001cec(93);
            else if (delta > 5000)
                Func_02001cfa(92);
            else
                Func_02001d02(91);
            Func_02001c58(20);
            Func_02001cae(0xe13);
            Func_02001c16(delta, 5);
            Func_02001cce(9, 0);
            Func_02001d1a();
        } else if (delta < 0) {
            Func_02001cce_a(0xe14);
            Func_02001c36(-delta, 5);
            Func_02001cee(9, 0);
        }
        Func_02001ca2();
    }

    if (state[225] == 12 && Func_02001c78(0x109) == 0) {
        entry = (s8 *)0x0200036c;
        Func_02001cbe();
        Func_02001d3a();
        Func_02001d46();
        Func_02001cc4(10);
        if (*entry == -1) {
            Func_02000cba(1);
        } else if (*entry != -2) {
            Func_02001d36(0xe2e);
            Func_02001d4e(8, 0);
            stop = -1;
            while (*entry != stop) {
                if (entry == (s8 *)0x0200036c)
                    Func_02001d54(0xe2f);
                else
                    Func_02001d5c(0xe30);
                topic = Func_02001170(*entry);
                Func_02001ccc(topic, 2);
                Func_02001d84(8, 0);
                Func_02001dbc(topic, 3);
                Func_02001d4c(topic, 0);
                Func_02001d3a_a(10);
                Func_02001dae(0, 0xc000, 0);
                entry++;
                Func_02001d4e_a(30);
            }
            *(u8 *)0x0200036c = 0xfe;
            Func_02001db8(0xe31);
            Func_02001dd0(8, 0);
        }
        Func_02001d84_a();
    }
    return 0;
}

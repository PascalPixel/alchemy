typedef signed short s16;
typedef signed int s32;

extern s16 Data_02000240[];
extern void Func_02001b5c(s32); extern s32 Func_02001d64(s32,s32); extern void Func_02001e20(s32,s32);
extern void Func_02001e7c(s32,s32,s32); extern void Func_02002d28(s32,s32,s32); extern void Func_02002de8(void);
extern void Func_02002910(s32,s32,s32); extern void Func_02002e50(s32,s32,s32); extern void Func_0200033c(s32,s32,s32);
extern void Func_02002aec(s32); extern void Func_0808a018(void); extern void Func_0808a020(void);
extern void Func_0808a170(s32); extern void Func_0808a208(s32,s32); extern void Func_0808a210(s32,s32,s32,s32);
extern void Func_0808a218(void); extern void Func_0808a180(s32,s32); extern void Func_0808a010(s32);
extern void Func_0808a1b8(s32,s32,s32); extern void Func_0808a1e8(s32,s32,s32); extern void Func_0808a090(s32,s32,s32);
extern void Func_0808a100(s32,s32); extern void Func_0808a0d0(s32,s32,s32); extern void Func_0808a200(s32,s32);
extern void Func_0808a0f0(s32,s32,s32);

void Func_020016ec(s32 actor)
{
    s32 phase;

    if (Data_02000240[225] == 2) {
        Func_02001b5c(actor);
        return;
    }

    Func_0808a018();
    phase = Func_02001d64(actor, 4);
    if (phase == 0) {
        Func_0808a170(0x2099);
        Func_0808a208(0x30000, 0x6000);
        Func_0808a210(0x04400000, -1, 0x00a80000, 1);
        Func_0808a218();
        Func_0808a180(actor, 0);
        Func_02002d28(120, 72, 0);
        Func_0808a180(actor, 0);
        Func_02002de8();
        Func_0808a010(15);
        Func_02002910(0, 984, 200);
        Func_0808a1b8(0, 0, 10);
        Func_0808a180(actor, 0);
        Func_0808a1b8(0, 0x4000, 30);
        Func_0808a1e8(0, 262, 60);
        Func_0808a090(0, 0x18000, 0xc000);
        Func_02002e50(0, 1000, 192);
        Func_02002e50(0, 1000, 176);
        Func_02002e50(0, 1016, 168);
        Func_0808a010(15);
        Func_0200033c(18, 160, 0);
        Func_0808a210(0x04400000, -1, 0x00a80000, 1);
        Func_0808a100(0, 1);
        Func_0808a010(10);
        Func_0808a090(0, 0x10000, 0x8000);
        Func_0808a0d0(0, 1192, 168);
        Func_0808a010(10);
        Func_0808a1b8(0, 0x8000, 30);
        Func_0808a1e8(0, 258, 60);
        Func_0808a180(actor, 0);
        Func_02002aec(0);
        Func_0808a200(0, 0);
        Func_0808a0f0(18, 0x03f80000, 0x00a80000);
        Func_02001e20(actor, 4);
    } else if (phase == 1) {
        Func_0808a170(0x2098);
        Func_0808a180(actor, 0);
    }

    Func_02001e7c(phase, actor, 4);
    Func_0808a020();
}

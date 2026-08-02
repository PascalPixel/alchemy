typedef signed short s16;
typedef signed int s32;

/* resource_3bc state-machine slot 6, complete 248-byte owner and pool. */

extern s16 Data_02000240[];

extern void Func_0200288c(void);
extern s32 Func_02002a94(s32 actor, s32 slot);
extern void Func_02002b50(s32 actor, s32 slot);
extern void Func_02002bac(s32 state, s32 actor, s32 slot);
extern void Func_02003a58(s32 x, s32 z, s32 mode);
extern void Func_02003abc(s32 x, s32 z, s32 mode);
extern void Func_02003b18(void);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a170(s32 message);
extern void Func_0808a180(s32 actor, s32 mode);
extern void Func_0808a200(s32 actor, s32 mode);
extern void Func_0808a208(s32 x, s32 z);
extern void Func_0808a210(s32 x, s32 y, s32 z, s32 mode);
extern void Func_0808a218(void);

void Func_020024d0(s32 actor)
{
    s32 state;

    if (Data_02000240[225] == 2) {
        Func_0200288c();
        return;
    }

    Func_0808a018();
    state = Func_02002a94(actor, 6);

    if (state == 0) {
        Func_0808a170(0x20c7);
        Func_0808a208(0x30000, 0x6000);
        Func_0808a210(0x5080000, -1, 0x980000, 1);
        Func_0808a218();
        Func_0808a010(30);
        Func_0808a180(actor, 0);
        Func_02003a58(0xb4, 0x58, 0);
        Func_0808a010(60);
        Func_0808a180(actor, 0);
        Func_02003abc(0x20, 0x54, 10);
        Func_0808a010(30);
        Func_0808a180(actor, 0);
        Func_02003abc(0x60, 0x54, 30);
        Func_0808a010(60);
        Func_0808a180(actor, 0);
        Func_02003b18();
        Func_0808a010(2);
        Func_0808a200(0, 0);
        Func_02002b50(actor, 6);
    } else if (state == 1) {
        Func_0808a170(0x20c6);
        Func_0808a180(actor, 0);
    }

    Func_02002bac(state, actor, 6);
    Func_0808a020();
}

typedef signed short s16;
typedef signed int s32;

/* resource_3bc state-machine slot 2, complete 408-byte owner and pool. */

extern s16 Data_02000240[];

extern void Func_0200288c(void);
extern s32 Func_02002a94(s32 actor, s32 slot);
extern void Func_02002b50(s32 actor, s32 slot);
extern void Func_02002bac(s32 state, s32 actor, s32 slot);
extern void Func_02003640(s32 actor, s32 x, s32 z);
extern void Func_0200381c(s32 actor);
extern void Func_02003a58(s32 x, s32 z, s32 mode);
extern void Func_02003abc(s32 x, s32 z, s32 mode);
extern void Func_02003b18(void);
extern void Func_02003b80(s32 actor, s32 x, s32 z);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a090(s32 actor, s32 x, s32 z);
extern void Func_0808a170(s32 message);
extern void Func_0808a180(s32 actor, s32 mode);
extern void Func_0808a1b8(s32 actor, s32 speed, s32 frames);
extern void Func_0808a200(s32 actor, s32 mode);
extern void Func_0808a208(s32 x, s32 z);
extern void Func_0808a210(s32 x, s32 y, s32 z, s32 mode);
extern void Func_0808a218(void);
extern void Func_0808a2a0(void);

void Func_02001df8(s32 actor)
{
    s32 state;

    if (Data_02000240[225] == 2) {
        Func_0200288c();
        return;
    }

    Func_0808a018();
    state = Func_02002a94(actor, 2);

    if (state == 0) {
        Func_0808a170(0x20b7);
        Func_0808a208(0x30000, 0x6000);
        Func_0808a210(0x1780000, -1, 0x980000, 1);
        Func_0808a218();
        Func_0808a010(30);
        Func_0808a180(actor, 0);
        Func_02003640(0, 0x118, 0xc8);
        Func_0808a090(0, 0x18000, 0xc000);
        Func_02003b80(0, 0x118, 0x98);
        Func_02003b80(0, 0x128, 0x98);
        Func_0808a010(10);

        Func_0808a2a0();
        Func_0808a210(-1, -1, -1, 0);
        Func_0808a1b8(0, 0xc000, 15);
        Func_0808a2a0();
        Func_0808a210(-1, -1, -1, 0);
        Func_0808a1b8(0, 0, 15);
        Func_0808a2a0();
        Func_0808a210(-1, -1, -1, 0);
        Func_0808a1b8(0, 0x4000, 15);

        Func_0808a180(actor, 0);
        Func_02003a58(0x60, 0x28, 0);
        Func_02003abc(0x80, 0x28, 10);
        Func_0808a010(30);
        Func_02003abc(0xa0, 0x28, 10);
        Func_0808a010(30);
        Func_02003abc(0xa0, 0x48, 10);
        Func_0808a010(30);
        Func_0808a180(actor, 0);
        Func_02003b18();
        Func_0200381c(0);
        Func_0808a200(0, 0);
        Func_02002b50(actor, 2);
    } else if (state == 1) {
        Func_0808a170(0x20b6);
        Func_0808a180(actor, 0);
    }

    Func_02002bac(state, actor, 2);
    Func_0808a020();
}

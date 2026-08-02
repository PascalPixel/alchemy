typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/* resource_3bc state-machine slot 4, complete 468-byte owner and pool. */

extern s16 Data_02000240[];

extern void Func_0200288c(void);
extern s32 Func_02002a94(s32 actor, s32 slot);
extern void Func_02002b50(s32 actor, s32 slot);
extern void Func_02002bac(s32 state, s32 actor, s32 slot);
extern void Func_02003640(s32 actor, s32 x, s32 z);
extern void Func_0200381c(s32 actor);
extern void Func_08009150(void *record, s32 x, s32 z, s32 y);
extern void Func_08009158(void *record);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 actor);
extern void Func_0808a090(s32 actor, s32 x, s32 z);
extern void Func_0808a0d0(s32 actor, s32 x, s32 z);
extern void Func_0808a100(s32 actor, s32 mode);
extern void Func_0808a170(s32 message);
extern void Func_0808a180(s32 actor, s32 mode);
extern void Func_0808a1b8(s32 actor, s32 speed, s32 frames);
extern void Func_0808a200(s32 actor, s32 mode);
extern void Func_0808a208(s32 x, s32 z);
extern void Func_0808a210(s32 x, s32 y, s32 z, s32 mode);
extern void Func_0808a218(void);
extern void Func_0808a2b0(void);

void Func_020020f0(s32 actor)
{
    s32 state;
    s32 x;
    s32 z;
    s32 moved_x;
    s32 moved_z;
    u8 *record;

    if (Data_02000240[225] == 2) {
        Func_0200288c();
        return;
    }

    Func_0808a018();
    state = Func_02002a94(actor, 4);

    if (state == 0) {
        Func_0808a170(0x20bf);
        Func_0808a208(0x30000, 0x6000);
        Func_0808a210(0x3580000, -1, 0xa80000, 1);
        Func_0808a218();
        Func_0808a010(30);
        Func_0808a180(actor, 0);
        Func_0808a180(actor, 0);
        Func_02003640(0, 0x330, 0xc8);
        Func_0808a090(0, 0x10000, 0x8000);
        Func_0808a0d0(0, 0x348, 0xc8);
        Func_0808a1b8(0, 0xc000, 20);
        Func_0808a2b0();
        Func_0808a210(-1, -1, -1, 0);
        Func_0808a090(0, 0x8000, 0x4000);

        record = Func_0808a080(0);
        x = *(s32 *)(record + 8);
        z = *(s32 *)(record + 12);
        Func_0808a090(0, 0x8000, 0x4000);

        Func_0808a100(0, 10);
        moved_z = z + 0x60000;
        Func_08009150(record, x, moved_z, *(s32 *)(record + 0x10));
        Func_08009158(record);

        Func_0808a100(0, 14);
        moved_x = x + 0x400000;
        Func_08009150(record, moved_x, moved_z, *(s32 *)(record + 0x10));
        Func_08009158(record);

        Func_0808a100(0, 10);
        moved_z = z + 0x360000;
        Func_08009150(record, moved_x, moved_z, *(s32 *)(record + 0x10));
        Func_08009158(record);

        Func_0808a100(0, 15);
        Func_08009150(record, x + 0x300000, moved_z,
                      *(s32 *)(record + 0x10));
        Func_08009158(record);

        Func_0808a100(0, 12);
        Func_0808a180(actor, 0);
        Func_0200381c(0);
        Func_0808a200(0, 0);
        Func_02002b50(actor, 4);
    } else if (state == 1) {
        Func_0808a170(0x20be);
        Func_0808a180(actor, 0);
    }

    Func_02002bac(state, actor, 4);
    Func_0808a020();
}

typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_3bc state-machine slot 3, complete 352-byte owner and pool.  The
 * workspace halfword at +0x182 is the cue polled during the bounded wait and
 * reset after the successful sequence.
 */

extern s16 Data_02000240[];

extern void Func_02000188(void);
extern void Func_020001b4(void);
extern void Func_020001c8(void);
extern void Func_02000b30(void);
extern void Func_0200288c(void);
extern s32 Func_02002a94(s32 actor, s32 slot);
extern void Func_02002b50(s32 actor, s32 slot);
extern void Func_02002bac(s32 state, s32 actor, s32 slot);
extern void Func_02003640(s32 actor, s32 x, s32 z);
extern void Func_0200381c(s32 actor);
extern void Func_02003b40(s32 actor, s32 x, s32 z);
extern void Func_080000c0(s32 frames);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a090(s32 actor, s32 x, s32 z);
extern void Func_0808a170(s32 message);
extern void Func_0808a180(s32 actor, s32 mode);
extern void Func_0808a1b8(s32 actor, s32 speed, s32 frames);
extern void Func_0808a1e8(s32 actor, s32 cue, s32 frames);
extern void Func_0808a200(s32 actor, s32 mode);
extern void Func_0808a208(s32 x, s32 z);
extern void Func_0808a210(s32 x, s32 y, s32 z, s32 mode);
extern void Func_0808a218(void);

void Func_02001f90(s32 actor)
{
    s32 state;
    u8 *workspace;

    if (Data_02000240[225] == 2) {
        Func_0200288c();
        return;
    }

    Func_0808a018();
    state = Func_02002a94(actor, 3);

    if (state == 0) {
        workspace = *(u8 **)0x03001ebc;
        Func_0808a170(0x20bb);
        Func_02000188();
        Func_0808a208(0x30000, 0x6000);
        Func_0808a210(0x2680000, -1, 0xb80000, 1);
        Func_0808a218();
        Func_0808a010(30);
        Func_0808a180(actor, 0);
        Func_020001b4();
        Func_0808a010(60);
        Func_0808a180(actor, 0);
        Func_02003640(0, 0x1f8, 0xc8);
        Func_0808a1b8(0, 0, 0);
        Func_020001c8();
        Func_0808a090(0, 0x18000, 0xc000);
        Func_02003b40(0, 0x2a8, 0xc8);

        while (*(s16 *)(workspace + 0x182) != 5) {
            state++;
            Func_080000c0(1);
            if (state > 239)
                break;
        }

        Func_02000b30();
        Func_0808a1b8(0, 0xc000, 20);
        Func_0808a1e8(0, 0x103, 60);
        Func_0808a180(actor, 0);
        Func_0200381c(0);
        Func_0808a200(0, 0);
        Func_02002b50(actor, 3);
        *(s16 *)(workspace + 0x182) = 0;
    } else if (state == 1) {
        Func_0808a170(0x20ba);
        Func_0808a180(actor, 0);
    }

    Func_02002bac(state, actor, 3);
    Func_0808a020();
}

typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_3bc state-machine slot 5, complete 416-byte owner and nine-word
 * pool.  Its case-zero arm stages the central actor, runs two cue phases,
 * performs the bounded leftward glide, and publishes the scene byte at +498.
 */

extern s16 Data_02000240[];

extern void Func_020022c4(s32 actor, s32 x, s32 z);
extern void Func_020022f4(s32 actor, s32 x, s32 z);
extern void Func_0200288c(void);
extern s32 Func_02002a94(s32 actor, s32 slot);
extern void Func_02002b50(s32 actor, s32 slot);
extern void Func_02002bac(s32 state, s32 actor, s32 slot);
extern void Func_02003640(s32 actor, s32 x, s32 z);
extern void Func_0200381c(s32 actor);
extern void Func_020038dc(s32 actor);
extern void Func_080000c0(s32 frames);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 actor);
extern void Func_0808a090(s32 actor, s32 x, s32 z);
extern void Func_0808a100(s32 actor, s32 mode);
extern void Func_0808a170(s32 message);
extern void Func_0808a180(s32 actor, s32 mode);
extern void Func_0808a1e8(s32 actor, s32 cue, s32 frames);
extern void Func_0808a1f0(s32 actor, s32 cue);
extern void Func_0808a200(s32 actor, s32 mode);
extern void Func_0808a208(s32 x, s32 z);
extern void Func_0808a210(s32 x, s32 y, s32 z, s32 mode);
extern void Func_0808a218(void);

void Func_02002330(s32 actor)
{
    s32 state;
    s32 frames;
    u8 *record;

    if (Data_02000240[225] == 2) {
        Func_0200288c();
        return;
    }

    Func_0808a018();
    state = Func_02002a94(actor, 5);

    if (state == 0) {
        Func_0808a170(0x20c3);
        Func_0808a208(0x30000, 0x6000);
        Func_0808a210(0x4380000, -1, 0xa80000, 1);
        Func_0808a218();
        Func_0808a010(30);
        Func_0808a180(actor, 0);
        Func_0808a180(actor, 0);
        Func_02003640(0, 0x3d8, 0xb8);
        Func_0808a090(0, 0x18000, 0xc000);
        Func_020022f4(0, 0x3e0, 0xb8);
        Func_0808a090(0, 0x4ccc, 0x2666);
        Func_020022c4(0, 0x460, 0xb8);
        Func_0808a010(120);
        Func_0808a1f0(0, 0x101);
        Func_0808a010(120);
        Func_020038dc(0);
        Func_0808a100(0, 1);
        Func_0808a1f0(0, 0x100);
        Func_0808a1e8(0, 0x105, 0);

        record = Func_0808a080(0);
        for (frames = 119; frames >= 0; frames--) {
            if (*(s32 *)(record + 8) > 0x3e00000)
                *(s32 *)(record + 8) -= 0x13333;
            Func_080000c0(1);
        }

        Func_0808a1e8(0, 0x103, 60);
        Func_020022c4(0, 0x460, 0xb8);
        Func_0808a180(actor, 0);
        Func_020038dc(0);
        ((u8 *)Data_02000240)[498] = 1;
        Func_0200381c(0);
        Func_0808a200(0, 0);
        Func_02002b50(actor, 5);
    } else if (state == 1) {
        Func_0808a170(0x20c2);
        Func_0808a180(actor, 0);
    }

    Func_02002bac(state, actor, 5);
    Func_0808a020();
}

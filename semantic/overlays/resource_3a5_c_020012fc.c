typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

extern void Func_02001054(void);
extern u8 *Func_0808a080();
extern s32 Func_080091d8();
extern u32 Func_080000f8(void);
extern void Func_0200013c();
extern void Func_0808a1f0();
extern void Func_08009150();
extern void Func_08009080();
extern void Func_08009158();
extern void Func_080000c0();

/* Resolve the active actor against three forward probes and react on impact. */
void Func_020012fc(void)
{
    u8 *workspace = *(u8 **)0x03001ebc;
    s32 slot = *(s32 *)(0x02000240 + 500);
    u8 *actor;
    s32 probe[3];
    s32 collision;

    Func_02001054();
    actor = Func_0808a080(slot);
    probe[0] = *(s32 *)(actor + 8);
    probe[1] = *(s32 *)(actor + 12);
    probe[2] = *(s32 *)(actor + 16) + 0x600000;
    collision = Func_080091d8(actor, probe);

    if ((*(u32 *)0x03001e40 & 4) == 0) {
        s32 descriptor[10] = { 0 };
        s32 x = *(s32 *)(actor + 8) + ((Func_080000f8() * 12) & 0xffff0000) - 0x60000;
        descriptor[8] = (s32)(((Func_080000f8() << 12) >> 16) + 0x3e0000);
        Func_080000f8();
        Func_0200013c(x, *(s32 *)(actor + 12), *(s32 *)(actor + 16),
                      0, 0, 0x800000, descriptor);
    }

    if (collision < 0) {
        Func_0808a1f0(slot, 0x102);
        Func_08009150(actor, *(s32 *)(actor + 8), *(s32 *)(actor + 12),
                      *(s32 *)(actor + 16) + 0x80000);
        Func_08009080(actor, 7);
        Func_08009158(actor);
        do Func_080000c0(1);
        while (*(s32 *)(actor + 12) != *(s32 *)(actor + 20));
        Func_08009080(actor, 6);
        Func_080000c0(3);
        return;
    }

    probe[2] = *(s32 *)(actor + 16) + 0x400000;
    collision = Func_080091d8(actor, probe);
    if (collision <= 0) {
        probe[0] = *(s32 *)(actor + 8) + 0x5b333;
        probe[2] = *(s32 *)(actor + 16) + 0x5b333;
        collision = Func_080091d8(actor, probe);
    }
    if (collision <= 0) {
        probe[0] = *(s32 *)(actor + 8) - 0x5b333;
        probe[2] = *(s32 *)(actor + 16) + 0x5b333;
        collision = Func_080091d8(actor, probe);
    }
    if (collision <= 0) {
        *(s32 *)(workspace + 496) += 0x600000;
        *(s32 *)(actor + 16) += 0x600000;
    }
}

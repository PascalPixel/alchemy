typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

extern s32 Func_03000380(s32 value, s32 divisor);
extern s32 Func_030003f0(s32 value, s32 divisor);
extern void Func_080f9010(s32 cue);
extern u8 *Func_080090c8(s32 kind, s32 x, s32 y, s32 z);
extern s32 Func_0808a498(u8 *owner, s32 previous);
extern void Func_080091e0(u8 *effect, s32 phase);
extern void Func_08009080(u8 *effect, s32 mode);

/* Emit a shrinking ring burst every ten frames of the 121-frame cycle. */
void Func_0200162c(void)
{
    s32 *counter = (s32 *)0x0200adcc;
    s32 *origin = (s32 *)0x0200adc0;
    s32 frame = *counter;
    s32 decade = Func_03000380(frame, 10);
    s32 chained = 0;

    if ((u32)frame <= 44 &&
        (frame == 0 || frame == 10 || frame == 20 ||
         frame == 30 || frame == 40)) {
        s32 count = 6 - decade;
        s32 i;

        Func_080f9010(220);
        for (i = 0; (u32)i < (u32)count; i++) {
            u8 *effect = Func_080090c8(0x11d,
                                       origin[0], origin[1], origin[2]);

            if (effect != 0) {
                u8 *owner = *(u8 **)(effect + 80);
                s32 sector;

                chained = Func_0808a498(owner, chained);
                effect[85] = 0;
                owner[9] &= (u8)~0x0c;
                Func_080091e0(effect, 0);
                Func_08009080(effect, 1);
                *(u16 *)(effect + 100) = 0;

                sector = Func_030003f0(360, count);
                *(u16 *)(effect + 102) =
                    (u16)Func_030003f0((sector * i) << 16, 360);
                *(s32 *)(effect + 56) = origin[0];
                *(s32 *)(effect + 60) = origin[1];
                *(s32 *)(effect + 64) = origin[2];
                *(s32 *)(effect + 48) = 0x19999;
                *(s32 *)(effect + 108) = 0x020095ad;
            }
        }
        Func_080f9010(0x121);
    }

    frame = *counter + 1;
    *counter = frame;
    if (frame > 120)
        *counter = 0;
}

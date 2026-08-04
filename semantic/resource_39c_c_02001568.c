typedef unsigned char u8;
typedef signed int s32;

/* resource_39c owner at 0x02001568, 120 bytes: when story flag 0x256 is set,
 * consume it, raise actor 0, snapshot the raised y field, wait five frames,
 * issue one six-argument presentation, play cue 217, attach descriptor
 * 0x0200e03a, and close the sequence. Prologue/frame through the sole return at
 * 0x020015ce-0x020015d4, then alignment and two pool words through 0x020015df. */
s32 Func_080770c0();
void Func_080770d0();
void Func_0808a018();
u8 *Func_0808a080();
void Func_0808a010();
void Func_08009180();
void Func_080f9010();
void Func_08009178();
void Func_0808a020();

void Func_02001568(void)
{
    u8 *actor;
    u8 *source;

    if (Func_080770c0(0x256) == 0)
        return;

    Func_0808a018();
    Func_080770d0(0x256);

    actor = Func_0808a080(0);
    *(s32 *)(actor + 12) += 0x20000;

    actor = Func_0808a080(0);
    source = Func_0808a080(0);
    *(s32 *)(actor + 60) = *(s32 *)(source + 12);

    Func_0808a010(5);
    Func_08009180(7, 2, 5, 11, 1, 1);
    Func_080f9010(217);
    Func_08009178((void *)0x0200e03a, 9, 7);
    Func_0808a020();
}

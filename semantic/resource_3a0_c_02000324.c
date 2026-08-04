typedef unsigned char u8;
typedef signed short s16;

u8 *Func_0808a080();

/* Complete actor-20 presentation-bit update through interworking return. */
void Func_02000324(void)
{
    u8 *entity = Func_0808a080(0);
    u8 *actor;

    if (*(s16 *)(entity + 14) > 31) {
        actor = Func_0808a080(20);
        actor[35] |= 2;
    } else {
        actor = Func_0808a080(20);
        actor[35] &= 0xfd;
    }
}

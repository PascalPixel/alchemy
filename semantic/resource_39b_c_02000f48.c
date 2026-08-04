typedef signed int s32;

extern void Func_0808a018();
extern void Func_080f9010();
extern void Func_0808a090();
extern void Func_0808a1e0();
extern void Func_0808a0e0();
extern void *Func_0808a080();
extern void Func_080091e0();
extern void Func_0808a010();
extern void Func_0808a0f0();

/* Slide actor 0 to a page-indexed horizontal stop. */
void Func_02000f48(s32 page)
{
    void *actor;

    Func_0808a018();
    Func_080f9010(228);
    Func_0808a090(0, 0x6666, 0x3333);
    Func_0808a1e0(0, 2);
    Func_0808a0e0(0, 0, -8);
    actor = Func_0808a080(0);
    Func_080091e0(actor, 0);
    Func_0808a010(8);
    Func_0808a0f0(0, 0x40000 + (page << 19), 0);
    Func_0808a010(30);
}

typedef unsigned char u8;
typedef signed int s32;

extern u8 *Func_0808a080();
extern void Func_080770c8();
extern void Func_0808a098();

/* Select actor 8's path according to actor 0's wrapped X tile. */
void Func_0200116c(void)
{
    s32 x = *(s32 *)(Func_0808a080(0) + 8);
    s32 tile = ((x < 0 ? x + 0x000fffff : x) >> 20);

    Func_080770c8(0x205);
    if (tile == 7)
        Func_0808a098(8, (void *)0x0200a874);
    else
        Func_0808a098(8, (void *)0x0200a8c8);
}

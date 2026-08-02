/* Story-flag-selected actor-8 scene sequence. */
typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;
u8 *Func_0808a080();
void Func_0808a018(); void Func_0808a020();
s32 Func_080770c0(); void Func_080770c8();
void Func_0808a0a8(); void Func_0808a010(); void Func_0808a138();
void Func_0808a100(); void Func_0808a170(); void Func_0808a180();
void Func_0808a098(); void Func_0808a1b8(); void Func_0808a188();
void Func_0808a128();

void Func_02003e48(void)
{
    u8 *lead = Func_0808a080(0);
    u8 *actor = Func_0808a080(8);

    Func_0808a018();
    if (Func_080770c0(0x305)) {
        Func_0808a0a8(8);
        Func_0808a010(10);
        Func_0808a138(8, 2);
        Func_0808a010(40);
        if (*(s16 *)(lead + 6) >= 0)
            Func_0808a100(8, 7);
        else
            Func_0808a100(8, 8);
        Func_0808a138(8, 2);
        Func_0808a010(20);
        Func_0808a170(0xed2);
        Func_0808a180(8, 0);
        Func_0808a098(8, (void *)0x0200cec8);
        Func_0808a100(8, 6);
    } else {
        Func_0808a0a8(8);
        *(s32 *)(actor + 24) = 0x10000;
        *(s32 *)(actor + 28) = 0x10000;
        Func_0808a1b8(8, 0x1000, 0);
        if (*(s16 *)(lead + 6) >= 0)
            Func_0808a100(8, 7);
        else
            Func_0808a100(8, 8);
        Func_0808a010(20);
        Func_0808a170(0xed1);
        Func_0808a188(8, 0, 20);
        Func_0808a100(8, 1);
        Func_0808a128(8, 4, 0);
        Func_0808a010(80);
        Func_0808a138(8, 2);
        Func_0808a010(40);
        if (*(s16 *)(lead + 6) >= 0)
            Func_0808a100(8, 7);
        else
            Func_0808a100(8, 8);
        Func_0808a010(2);
        Func_0808a128(8, 2, 0);
        Func_0808a010(60);
        Func_0808a138(8, 2);
        Func_0808a010(20);
        Func_0808a180(8, 0);
        Func_0808a098(8, (void *)0x0200cec8);
        Func_0808a100(8, 6);
        Func_080770c8(0x305);
    }
    Func_0808a020();
}

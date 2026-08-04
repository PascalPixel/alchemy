typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/* resource_3bc owner at 0x020029ac, 164 bytes, independently matched to
 * resource_3bb:1f14. Prologue through the sole return at
 * 0x02002a3c-0x02002a44, then alignment and two pool words through 0x02002a4f.
 * The party-count-selected interaction accounts for all seventeen calls. */
u8 *Func_0808a080(); void Func_0808a018(); s32 Func_08077148();
void Func_0808a170(); s32 Func_0808a190(); void Func_0808a090();
void Func_0808a0c8(); void Func_0808a010(); void Func_0808a0d0();
void Func_0808a368(); void Func_0808a370(); void Func_0808a248();
void Func_0808a180(); void Func_0808a020();

void Func_020029ac(s32 selector)
{
    u8 *actor = Func_0808a080(selector);
    s32 x = *(s16 *)(actor + 10);
    s32 z = *(s16 *)(actor + 18);
    Func_0808a018();
    if (Func_08077148() <= 1) {
        Func_0808a170(0x20e5);
        if (Func_0808a190(selector, 0) == 0) {
            Func_0808a090(0, 0x10000, 0x8000);
            Func_0808a090(selector, 0x10000, 0x8000);
            Func_0808a0c8(selector, x, z + 64);
            Func_0808a010(15);
            Func_0808a0d0(0, x, z);
            Func_0808a0d0(0, x, z + 32);
            Func_0808a368(); Func_0808a370(); Func_0808a248(11);
        }
    } else {
        Func_0808a170(0x20e8); Func_0808a180(selector, 0);
    }
    Func_0808a020();
}

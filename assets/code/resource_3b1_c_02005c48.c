typedef unsigned int u32;
typedef signed int s32;

int Func_0200c09c();
void Func_0200c0ee();
void Func_0200c2de();
void Func_0200c116();

/*
 * resource_3b1 owner at 0x02005c48, 92 bytes including alignment and its two
 * pool words. It counts down a shared effect timer, triggers the terminal fade
 * at 70, and seeds a new 80-frame pulse on one out of 120 random samples. The
 * sole return is at 0x02005c98; all four calls appear in machine order.
 * Per-site call veneers (raw asm confirms each callee slot uses its own
 * local stub, distinct from the generic main-image symbol name).
 */
void Func_02005c48(void)
{
    s32 *timer = (s32 *)0x0200ff84;

    if (*timer != 0) {
        *timer -= 1;
        if (*timer == 70)
            Func_0200c0ee(-1, -1, 0xe666);
    } else if (((u32)(Func_0200c09c() * 120) >> 16) == 0) {
        Func_0200c2de(0xb5);
        Func_0200c116(0x20000, 0x20000, 0x10000);
        *timer = 80;
    }
}

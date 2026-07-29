typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * The service interfaces are not reconstructed yet.  The first call observes
 * `adjustment` still in r0, so passing it explicitly preserves that interface
 * without asserting whether the callee consumes it.
 */
void Func_02001d9e();
u16 Func_02001d64(u16 color, s32 adjustment);
void Func_02001e22();
void Func_02001e06();
void Func_02001fc0(s32 size, s32 mode);

/*
 * Adjust the background-palette entries outside the two protected ranges.
 *
 * Owner span: 0x02000e64-0x02000ecc.  The return is at 0x02000ec2 and
 * the two-word literal pool occupies the remainder of the span.
 */
void Func_02000e64(s32 adjustment)
{
    volatile u16 *const backgroundPalette = (volatile u16 *)0x05000000;
    u32 index;

    Func_02001d9e(adjustment);
    for (index = 0; index <= 223; index++) {
        if ((index >= 17 && index <= 23) ||
            (index >= 193 && index <= 200)) {
            continue;
        }

        backgroundPalette[index] =
            Func_02001d64(backgroundPalette[index], adjustment);
    }

    Func_02001e22();
    Func_02001e06();
    Func_02001fc0(0x10000, 0);
}

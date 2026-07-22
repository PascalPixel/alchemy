typedef unsigned char u8;
typedef signed int s32;

s32 Func_080771a0(void);

/*
 * The r9 static-chain setup at the call sites and the callee's matching r9
 * save identify this as a GNU C nested function. The public alias gives the
 * reconstructed entry its address-derived name without changing its code.
 */
extern s32 Func_080bd3e4(u8 *)
    __attribute__((alias("Select_080bd3e4.0")));

static __inline__ s32 Scope_080bd3e4(void)
{
    s32 Select_080bd3e4(u8 *weights)
    {
        s32 value;
        s32 total;
        s32 result;
        s32 index;

        value = Func_080771a0() & 0xFF;
        total = weights[0];
        result = 0;
        index = 0;
        if (value >= total) {
loop:
            index++;
            if (index <= 7) {
                total += weights[index];
                if (value < total)
                    result = index;
                else
                    goto loop;
            }
        }
        return result;
    }

    return 0;
}

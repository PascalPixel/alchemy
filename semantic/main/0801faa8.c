/*
 * Correctness fix, veneer audit (mars, 2026-08-01).
 *
 * `Func_080072f0` is not a function.  0x080072e4 begins the GCC
 * `__call_via_rN` veneer bank -- fifteen four-byte `bx rN; nop` entries,
 * r0..lr, ending at 0x08007320 -- so 0x080072f0 is `__call_via_r3` and
 * `bl 0x80072f0` calls whatever r3 holds.
 *
 * At every site in this file the ROM loads r3 from the literal pool with
 * the constant 0x03001388, so the callee is the relocated IWRAM word copy
 * at that address.  Its signature is not guessed: the EXACT source
 * src/080d40ec.c declares it as
 * `void *(*)(void *destination, const void *source, s32 size)` and
 * src/080e0524.c casts the same address to the same shape.
 *
 * This draft had modelled the callee as a fourth ARGUMENT (and as an
 * `extern` data symbol at 0x03001388).  It was never data and never an
 * argument: it is the call target.
 */
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef void *(*WordCopy)(void *destination, const void *source, s32 size);

void *Func_08004970(s32);
s32 Func_080056cc(void);
void Func_0801776c(s32, s32);
s32 Func_08005a78(s16, void *);
s32 Func_08005920(s16, void *);
void Func_08005cf8(void);
void Func_08002df0(void *);

extern s16 Data_02002004;
extern u8 Data_020004e4;
extern u8 Data_02000000;
extern u8 Value_0000000a;
extern u8 Value_0000000b;

s32 Func_0801faa8(void)
{
    void *buffer;
    s32 return_value;
    s32 result;
    s32 address;
    s16 *data;

    result = 0;
    buffer = Func_08004970(0x1000);
    data = &Data_02002004;
    return_value = *data;
    if (return_value != -1) {
        if (Func_080056cc() != 0) {
            result = 9;
            Func_0801776c((s32)&Value_0000000a, 1);
            goto negate_result;
        }
        if (Func_08005a78(*data, buffer) != 0) {
            Func_0801776c((s32)&Value_0000000b, 1);
            result = -2;
        }
        address = (s32)buffer + (s32)&Data_020004e4;
        ((WordCopy)0x03001388)(
            (void *)(address - (s32)&Data_02000000),
            &Data_020004e4,
            16);
        if (Func_08005920(*data, buffer) != 0) {
            Func_0801776c((s32)&Value_0000000b, 1);
            result = 3;
negate_result:
            result = -result;
        }
        Func_08005cf8();
        Func_08002df0(buffer);
        return_value = result;
    }
    return return_value;
}

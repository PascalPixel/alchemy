typedef unsigned char u8;
typedef signed int s32;

extern void Func_0200263a(void);
extern void Func_02002616(s32, s32, s32, s32, s32, s32);
/* Two sites reach this one symbol with different arities; old-style so both
 * calls are legal. */
extern u8 *Func_0200266c();
extern void Func_02002648(s32);

void Func_02001144(void)
{
    u8 *slot;

    Func_0200263a();
    {
        s32 fifth = 21;
        s32 sixth = 11;

        Func_02002616(14, 6, 1, 2, fifth, sixth);
    }
    slot = Func_0200266c(15) + 89;
    *slot = 254;
    Func_02002648(0x201);
    Func_0200266c();
}

typedef unsigned char u8;
typedef signed int s32;
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_08097384(void);
void *Func_08098070(s32);
void Func_08098184(void *);
void Func_08009080(void *, s32);
void Func_080030f8(s32);
void Func_0809748c(void);
void Func_080981b0(void *);

void Func_0809802c(void)
{
    register s32 value;
    register void *state;
    void *object;
    volatile s32 scratch[3];

    state = *(volatile void **)0x03001F30;
    value = M2C_FIELD(state, volatile s32 *, 0x10);
    Func_08097384();
    object = Func_08098070(value);
    Func_08098184(object);
    if (object != (void *)0) {
        Func_08009080(object, 4);
        Func_080030f8(0x1E);
    }
    Func_0809748c();
    Func_080981b0(object);
}

typedef unsigned char u8;

extern u8 *Func_080090c8();
extern void Func_08009098();
extern void Func_08009020();

/* Spawn and configure the compact companion object at a source position. */
void Func_02001bc8(u8 *source)
{
    u8 *object = Func_080090c8(24, *(int *)(source + 8),
                              *(int *)(source + 12), *(int *)(source + 16));
    if (object != 0) {
        u8 *owner = *(u8 **)(object + 80);
        Func_08009098(object, (void *)0x0200a7b8);
        object[85] = 0;
        object[34] = 1;
        object[35] = 2;
        if (owner != 0) {
            Func_08009020(owner, 2);
            owner[38] = 0;
            owner[9] |= 0x0c;
        }
    }
}

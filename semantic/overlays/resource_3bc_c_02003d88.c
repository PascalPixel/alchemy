typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

u8 *Func_0808a080();
void Func_08009098();
void Func_08009150();
void Func_080f9010();

/*
 * resource_3bc owner at 0x02003d88, 84 bytes: the resource_3bb indexed-record
 * mover with this overlay's independently resolved callback pool word.
 */
s32 Func_02003d88(u8 *object)
{
    s16 *index = (s16 *)(object + 100);
    u8 *record = Func_0808a080(*index);

    Func_08009150(record,
                  *(s32 *)(object + 8),
                  *(s32 *)(object + 12) + 0x02400000,
                  *(s32 *)(object + 16));
    record[0x55] = 0;
    Func_08009098(record, (void *)0x0200ce74);
    Func_080f9010(0x53);
    *index = 0;
    return 0;
}

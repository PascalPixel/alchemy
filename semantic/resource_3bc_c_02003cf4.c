typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

u32 Func_080000f8(void);
void Func_08000128();
u8 *Func_080090c8();
void Func_08009080();
void Func_08009098();
void Func_080091e0();

/*
 * resource_3bc owner at 0x02003cf4, 148 bytes: the resource_3bb clone owner
 * with this overlay's independently resolved callback pool word.
 */
void Func_02003cf4(u8 *object)
{
    s32 position[3];
    u32 random_x;
    u32 random_z;
    u8 *clone;

    if ((u32)(*(s32 *)(object + 40) + 255) <= 510)
        object[0x55] = 0;

    if (((Func_080000f8() * 100) >> 16) > 9)
        return;

    position[0] = *(s32 *)(object + 8);
    position[1] = *(s32 *)(object + 12);
    position[2] = *(s32 *)(object + 16);
    random_x = Func_080000f8();
    random_z = Func_080000f8();
    Func_08000128(random_x << 4, random_z, position);

    clone = Func_080090c8(0x11d, position[0], position[1], position[2]);
    if (clone != 0) {
        clone[0x55] = 0;
        Func_080091e0(clone, 0);
        Func_08009098(clone, (void *)0x0200ce50);
        Func_08009080(clone, 1);
        Func_08009080(clone, 0);
    }
}

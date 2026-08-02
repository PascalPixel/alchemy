/* Copy the low two mode bits into the object's owner record. */
typedef signed int s32;
typedef unsigned char u8;

void Func_020009dc(u8 *object, s32 mode)
{
    u8 *owner = *(u8 **)(object + 80);
    owner[9] = (owner[9] & ~12) | ((mode & 3) << 2);
}

typedef unsigned char u8;
typedef signed int s32;

void Func_080091e0();

/* Complete 24-byte entity reset owner. */
s32 Func_02000324(u8 *entity)
{
    Func_080091e0(entity, 0);
    entity[89] = 0;
    return 0;
}

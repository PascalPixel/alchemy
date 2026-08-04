/* Apply the actor's low four counter bits as its animation mode. */
typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;

void Func_02003fa2(u8 *actor, s32 mode);

s32 Func_02001428(u8 *actor)
{
    Func_02003fa2(actor, *(u16 *)(actor + 100) & 15);
    return 0;
}

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * resource_3bb actor-state latch at 0x02001c78, complete 72-byte owner through
 * its three-word pool before the next prologue at 0x02001cc0.
 */

extern s32 Func_080770c0(s32 flag);

void Func_02001c78(void)
{
    u8 *workspace = *(u8 **)0x03001ebc;
    s32 actor = *(s32 *)0x02000434;

    if (actor != 0 && (*(u16 *)(workspace + 382) >> 10) == actor &&
        Func_080770c0(0x141) != 0)
        *(u16 *)(workspace + 386) = 99;
}

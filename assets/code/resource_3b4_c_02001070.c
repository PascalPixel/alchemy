typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern u8 *Func_020035b6(s32);
extern s32 Func_0200358e(s32);
extern s32 Func_02003588(s32);
extern void Func_020035ae(s32);
extern void Func_02002082(s32);
extern void Func_020035ac(s32);

void Func_02001070(void)
{
    u8 *slot = Func_020035b6(0);

    if (*(u16 *)(slot + 6) != 0xC000) {
        return;
    }
    if (Func_0200358e(0x9C4) != 0) {
        return;
    }
    if (Func_02003588(243) == -1) {
        return;
    }
    Func_020035ae(0x9C4);
    Func_02002082(0x100);
    Func_020035ac(243);
}

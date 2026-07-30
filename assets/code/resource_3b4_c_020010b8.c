typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern u8 *Func_02003600(s32);
extern s32 Func_020035de(s32);
extern s32 Func_020035d8(s32);
extern void Func_020035fe(s32);
extern void Func_02002038(s32);

void Func_020010b8(s32 selector)
{
    u8 *slot = Func_02003600(0);
    s32 flag;

    if (*(u16 *)(slot + 6) != 0xC000) {
        return;
    }
    flag = selector + 2496;
    if (Func_020035de(flag) != 0) {
        return;
    }
    if (Func_020035d8(244) == -1) {
        return;
    }
    Func_020035fe(flag);
    Func_02002038(0x100 | selector);
    Func_020035fe(244);
}

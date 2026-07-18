typedef unsigned short u16;
typedef signed int s32;

s32 Func_08077394(s32);
void Func_08078588(s32, s32);
void Func_08078708(s32, s32);

void Func_0807a628(s32 owner, s32 target)
{
    s32 state = Func_08077394(owner);
    u16 *entry;
    s32 index;

    Func_08078588(owner, target);
    index = 0;
    entry = (u16 *)(state + 0xd8);
    do {
        if (*entry++ == target)
            Func_08078708(owner, index);
        index++;
    } while (index <= 14);
}

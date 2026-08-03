typedef unsigned char u8;
typedef signed int s32;

u8 *Func_02003362();
void Func_020033b8();

void Func_02001004(s32 subject, s32 x, s32 z)
{
    u8 *entry = Func_02003362(subject);
    u8 *flags;

    if (entry == 0) {
        return;
    }

    /* Activate the subject, set its two state bytes, then place it at cell center. */
    Func_020033b8(subject, 3);
    flags = entry;
    flags += 34;
    {
        int value = 2;
        *flags = value;
    }
    flags++;
    {
        int mask = 2;
        *flags = mask | *flags;
    }
    *(s32 *)(entry + 8) = (x << 20) + 0x80000;
    *(s32 *)(entry + 16) = (z << 20) + 0x80000;
}

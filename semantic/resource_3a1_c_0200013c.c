/* Spawn an object and apply the optional fields selected by the flag word. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

u8 *Func_020007d2(s32 slot);
u8 *Func_020007b0(s32 kind, s32 x, s32 y, void *z);
void Func_020007ba(u8 *object, s32 mode);
void Func_020007d4(u8 *object, s32 value);
void Func_020008a6(u8 *object, s32 mode);
s32 Func_02000890(s32 value, s32 divisor);
s32 Func_020008a8(s32 value, s32 divisor);
s32 Func_020008b6(s32 value, s32 divisor);
void Func_020008d4(u8 *object, s32 mode);
void Func_020008e4(u8 *object, s32 value);

void Func_0200013c(
    s32 x,
    s32 y,
    void *z,
    s32 field44,
    s32 field48,
    s32 field4c,
    s32 flags,
    u8 *extra)
{
    u8 **table = (u8 **)0x0200876c;
    register void *spawnZ = z;
    register s32 options = flags;
    s32 indexOffset;
    u8 *object;
    u8 *owner;
    u8 *source;
    u8 *entry;
    s32 copiedPosition;
    s32 deltaY;
    struct {
        u16 *field64;
        s32 field44;
    } local;

    local.field44 = field44;

    source = Func_020007d2(0);

    object = Func_020007b0(
        (options & 0x100000) != 0 && extra != 0 ? *(short *)(extra + 24) : 222,
        x,
        y,
        spawnZ);
    if (object == 0)
        return;

    owner = *(u8 **)(object + 80);
    Func_020007ba(object, (options + 1) & 15);

    indexOffset = (options & 15) * 4;
    Func_020007d4(object, *(s32 *)((u8 *)table + indexOffset));

    object[85] = 0;
    owner[38] = 0;
    *(u32 *)(object + 108) = 0x02008105;
    *(s32 *)(object + 68) = local.field44;
    *(s32 *)(object + 72) = field48;
    *(s32 *)(object + 76) = field4c;

    owner[9] = (owner[9] & ~12) | (*(u8 *)(*(u8 **)(source + 80) + 9) & 12);

    *(s32 *)(object + 48) = 0;
    *(s32 *)(object + 52) = 0;
    local.field64 = (u16 *)(object + 100);
    *local.field64 = 0;

    if ((options & 0xffff0000) != 0 && extra != 0) {

    if ((options & 0x10000) != 0)
        Func_020008a6(object, *(s32 *)(extra + 4));

    if ((options & 0x20000) != 0) {
        object[35] &= ~1;
        owner[9] = (owner[9] & ~12) | ((extra[0] & 3) << 2);
    }

    copiedPosition = options & 0x80000;
    if (copiedPosition != 0) {
        *(s32 *)(object + 24) = *(s32 *)(extra + 8);
        *(s32 *)(object + 28) = *(s32 *)(extra + 12);
    }

    if ((options & 0x40000) != 0) {
        entry = *(u8 **)((u8 *)table + indexOffset);
        if (copiedPosition != 0) {
            *(s32 *)(object + 48) = Func_02000890(
                *(s32 *)(extra + 16) - *(s32 *)(object + 24),
                *(s32 *)(entry + 12));
            deltaY = *(s32 *)(extra + 20) - *(s32 *)(object + 28);
        } else {
            *(s32 *)(object + 48) = Func_020008a8(
                *(s32 *)(extra + 16) - 0x10000,
                *(s32 *)(entry + 12));
            deltaY = *(s32 *)(extra + 20) - 0x10000;
        }
        *(s32 *)(object + 52) = Func_020008b6(deltaY, *(s32 *)(entry + 12));
    }

    if ((options & 0x200000) != 0) {
        Func_020008d4(object, 1);
        Func_020008e4(object, *(s32 *)(extra + 28));
    }

    if ((options & 0x400000) != 0)
        *(u16 *)(owner + 30) = *(u16 *)(extra + 32);

    if ((options & 0x800000) != 0)
        *local.field64 = *(u16 *)(extra + 34);

    if ((options & 0x1000000) != 0)
        *(u32 *)(object + 108) = *(u32 *)(extra + 36);
    }
}

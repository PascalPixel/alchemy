typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern s32 Func_08019d2c();
extern void Func_08016418(u8 *work, s32 release);

void Func_08019e48(void)
{
    u8 *base;
    u8 *work;
    s32 id;
    s32 index;
    s32 i;
    u8 *entity;
    s32 offset;

    base = *(u8 **)0x03001E8C;
    work = base + 0x500;
    id = Func_08019d2c();
    if (id == -1)
        return;

    if (*(u16 *)(base + 0x12EE) == id) {
        index = 1;
    } else if (*(u16 *)(base + 0x12EC) == id) {
        index = 0;
    } else {
        return;
    }

    offset = 0x12F0 + index * 2;
    id = *(u16 *)(base + offset);

    for (i = 0; i != 8; i++, work += 36) {
        entity = *(u8 **)work;
        if (*(u8 *)(entity + 4) == 2 && *(u8 *)(entity + 14) == id) {
            Func_08016418(work, 2);
            return;
        }
    }
}

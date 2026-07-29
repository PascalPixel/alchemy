typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void *Func_0808ba1c(u32);
void Func_08009080(void *, s32);
void Func_080091e0(void *, s32);
void Func_08009140(void *);

void Func_0808bb2c(void)
{
    u8 *config0;
    u8 *config1;
    u8 *config2;
    u8 *ids;
    u8 id;
    s32 index;
    u8 *source;
    void *object;
    void *child;
    s32 bits;
    s32 value;
    void *target;

    source = (u8 *)0x02001124;
    config0 = source + 0xE00;
    config1 = source + 0xE20;
    config2 = source + 0xE40;
    id = source[-32];
    index = 0;
    ids = source - 31;

    if (id != 0xFF) {
loop:
        object = Func_0808ba1c(id);
        if (object != 0) {
            child = FIELD(object, void *, 0x50);
            *(u32 *)0x040000D4 = (u32)source;
            *(void **)0x040000D8 = object;
            *(u32 *)0x040000DC = 0x8400001C;

            if (*config0 != 0)
                Func_08009080(object, *config0);
            Func_080091e0(object, *config1);

            bits = (*config2 & 3) * 4;
            FIELD(child, u8, 9) = (FIELD(child, u8, 9) & -13) | bits;
            FIELD(child, u8, 0x15) = (FIELD(child, u8, 0x15) & -13) | bits;
            FIELD(object, void *, 0x50) = child;

            if (id == *(s32 *)0x02000434) {
                target = FIELD(*(void **)0x03001EBC, void *, 0x1E0);
                value = FIELD(object, s32, 0xC);
                FIELD(target, s32, 0x14) = value;
                FIELD(target, s32, 0xC) = value;
                FIELD(**(void ***)0x03001E70, s32, 4) = value;
                Func_08009140(object);
            }
        }

        source += 0x70;
        index++;
        config0++;
        config1++;
        config2++;
        if (index <= 31) {
            id = *ids++;
            if (id != 0xFF)
                goto loop;
        }
    }
}

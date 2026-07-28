#include "types.h"

struct MenuDefaults {
    u8 padding_000[0x1C8];
    s16 primary;
    s16 secondary;
};

struct TextObject {
    u8 storage[12];
};

struct Work;

extern struct MenuDefaults Data_02000240;
extern volatile u32 Data_03001ae8;

struct Work *Func_080162d4(s32 kind, s32 x, s32 y, s32 width, s32 layer);
void Func_08028ef0(
    struct Work *work, s16 primary, const s16 *secondary);
void Func_0801c0dc(struct TextObject *object, s32 *resource);
void Func_080030f8(s32 frames);
s16 Func_08029094(
    struct Work *work,
    s16 primary,
    s16 *secondary,
    s16 *mode);
void Func_0801c17c(s32 resource);
void Func_08016418(struct Work *work, s32 release);
void Func_0808a238(s16 primary, s16 secondary);
void Func_0801c154(struct TextObject *object, s32 x, s32 y);

s16 Func_08028f98(void)
{
    s32 resource;
    s16 mode;
    s16 secondary;
    struct TextObject object;
    s16 primary;
    struct Work *work;
    s16 result;

    work = 0;
    mode = 0;
    primary = Data_02000240.primary;
    secondary = Data_02000240.secondary;
    work = Func_080162d4(0, 7, 30, 5, 2);
    Func_08028ef0(work, primary, &secondary);
    Func_0801c0dc(&object, &resource);

    while (Data_03001ae8 != 0)
        Func_080030f8(1);

    for (;;) {
        result = Func_08029094(work, primary, &secondary, &mode);
        if (result == -1) {
            Func_0801c17c(resource);
            Func_08016418(work, 2);
            Func_0808a238(primary, secondary);
            return result;
        }
        if (result == -2) {
            Func_0801c17c(resource);
            Func_08016418(work, 2);
            return result;
        }

        Func_0801c154(&object, 74, mode * 14 + 60);
        primary = result;
        Func_080030f8(1);
    }
}

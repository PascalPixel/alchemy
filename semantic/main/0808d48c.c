#include "types.h"

struct Entry_0808d48c {
    s32 descriptor;
    u8 kind;
    u8 pad_05;
    s16 value_06;
    s32 value_08;
};

extern u8 *Data_03001ebc;
extern u8 Value_02000240;

void *Func_0808ba1c(s32);
s32 Func_0808d458(s32, s32);
s32 Func_0808d428(s32);

struct Entry_0808d48c *Func_0808d48c(s32 descriptor_kind, s32 kind)
{
    u8 *runtime;
    struct Entry_0808d48c *entry;
    u32 position;
    s32 descriptor;
    s32 threshold;
    s32 accepted;
    s32 direction;

    runtime = Data_03001ebc;
    entry = *(struct Entry_0808d48c **)(runtime + 0x10);
    position = *(u16 *)((u8 *)Func_0808ba1c(
        *(s32 *)(((u16 *)&Value_02000240) + 250)) + 6);
    descriptor = entry->descriptor;

    while (descriptor != -1) {
        if ((descriptor & 0xf) == descriptor_kind && entry->kind == kind) {
            if (Func_0808d458(descriptor, entry->value_08) != 0)
                goto validate;
            if (Func_0808d428(entry->value_06) != 0) {
                descriptor = entry->descriptor;
validate:
                accepted = 0;
                threshold = 12;
                if ((descriptor & 0x800) != 0)
                    threshold = 2;
                direction = descriptor & 0x600;
                switch (direction) {
                case 0x200:
                    if (*(s16 *)(runtime + 0x19c) > threshold)
                        accepted = 1;
                    break;
                case 0:
                    accepted = 1;
                    break;
                case 0x600:
                    if (*(s16 *)(runtime + 0x19c) <= threshold)
                        break;
                case 0x400:
                    if ((u16)(((descriptor & 0xf000) - position) + 0x1fff)
                        <= 0x3ffe)
                        accepted = 1;
                    break;
                }
                if (accepted != 0)
                    return entry;
            }
        }
        entry++;
        descriptor = entry->descriptor;
    }
    return 0;
}

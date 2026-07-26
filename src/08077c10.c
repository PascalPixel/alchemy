#include "types.h"

struct Owner_08077c10 {
    u8 padding[216];
    u16 entries[15];
};

extern u8 Data_02000240[];

void Func_08079374(s32);
s32 Func_080795fc(void);
struct Owner_08077c10 *Func_08077394(s32);
s32 Func_08078414(s32);
s32 Func_08079358(s32);

void Func_08077c10(void)
{
    s32 count;
    s32 n;

    Func_08079374(0x167);
    count = Func_080795fc();
    for (n = 0; n < count; n++) {
        struct Owner_08077c10 *owner;
        s32 i;

        owner = Func_08077394(Data_02000240[504 + n]);
        for (i = 0; i < 15; i++) {
            if (owner->entries[i] & 0x200) {
                u8 *record;
                s32 j;

                record = (u8 *)Func_08078414(owner->entries[i]) + 24;
                for (j = 0; j < 4; j++) {
                    u8 kind;

                    kind = *record;
                    record += 4;
                    if (kind == 27) {
                        Func_08079358(0x167);
                    }
                }
            }
        }
    }
}

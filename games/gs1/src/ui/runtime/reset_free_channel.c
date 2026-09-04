#include "types.h"
#include "gs1_edition.h"

#define UiWork_ResetFreeChannel Func_08016758

struct EntryState {
    u8 padding00[0x14];
    u16 state;
};

struct EntrySlot {
    struct EntryState *entry;
    s16 field04;
    s16 field06;
    u8 padding08[0x0C];
    s16 field14;
    s16 field16;
    s16 field18;
    s16 field1a;
    u8 padding1c[0x0C];
};

extern u8 *Data_03001e8c;

char Func_0801671c(void);

void UiWork_ResetFreeChannel(void)
{
    struct EntrySlot *slot =
        (struct EntrySlot *)(Data_03001e8c + RENDER_CHANNEL_OFS);
    struct EntrySlot *sel = 0;
    s32 i;

    for (i = 0; i != 3; slot++, i++) {
        if (slot->entry == 0 || slot->entry->state != 0) {
            sel = slot;
            break;
        }
    }
    if (sel != 0) {
        if (sel->entry != 0) {
            Func_0801671c();
            sel->field06 = 0;
        }
        sel->field04 = 0;
        sel->field14 = 0;
        sel->field16 = 0xF;
        sel->field18 = 0;
        sel->field1a = 0xA;
    }
}

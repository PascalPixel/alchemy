#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "TLA_EDITION.H"

struct UiChannelWork {
    u8 padding00[0x14];
    u16 state;
};

struct UiChannelSlot {
    struct UiChannelWork *work;
    u16 field04;
    u16 field06;
    u16 values[4];
    u16 field10;
    u16 field12;
    u16 field14;
    u16 field16;
    u16 field18;
    u16 field1a;
    u16 field1c;
    u16 field1e;
    u16 field20;
    u16 field22;
    u16 field24;
    u16 field26;
};

s32 Ui_ClearVramBlock(void);

void UiWork_ResetFreeChannel(void)
{
    struct UiChannelSlot *slot =
        (struct UiChannelSlot *)(Ram_HeapSlots->window_tiles + RENDER_CHANNEL_OFS);
    struct UiChannelSlot *sel = 0;
    s32 i;

    for (i = 0; i != 3; slot++, i++) {
        if (slot->work == 0 || slot->work->state != 0) {
            sel = slot;
            break;
        }
    }
    if (sel != 0) {
        if (sel->work != 0) {
            Ui_ClearVramBlock();
            sel->field06 = 0;
        }
        sel->field04 = 0;
        sel->field14 = 0;
        sel->field16 = 0xF;
        sel->field18 = 0;
        sel->field1a = 0xA;
    }
}

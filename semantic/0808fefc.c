#include "types.h"

struct FieldState_0808fefc {
    u8 unknown_000[0x14];
    u16 display_bits;
    u8 unknown_016[0xea];
    s16 transition_x;
    s16 transition_z;
};

struct EffectState_0808fefc {
    u8 unknown_000[0x528];
    s16 mode;
    s16 parameter;
    u8 unknown_52c[8];
    s16 limit;
    s16 enabled;
    u8 unknown_538[2];
    s8 x;
    s8 z;
    s8 value;
    s8 reserved;
};

struct DisplayWrite_0808fefc {
    volatile u32 value;
    volatile u32 address;
    volatile u32 control;
};

struct DisplayWriteQueue_0808fefc {
    volatile u16 count;
    u16 padding_02;
    struct DisplayWrite_0808fefc entries[32];
};

extern struct FieldState_0808fefc *Data_03001e70;

void Func_08003b70(s32 value);
void Func_08003bb4(s32 value);
void Func_080030f8(u32 frames);
void Func_08091220(s32 value, s32 mode);
void Func_08091254(s32 value);
void Func_08091240(s32 value);
struct EffectState_0808fefc *Func_0808fecc(void);
void Func_080041d8(u32 callback, u32 workspace_size);
void Func_080907b0(s32 value);
void Func_0800307c(s32 enabled, s32 mode, u32 callback);

static void QueueDisplayWrite_0808fefc(
    struct FieldState_0808fefc *field)
{
    volatile u16 *interrupt_master_enable =
        (volatile u16 *)0x04000208;
    volatile u16 *display_control =
        (volatile u16 *)0x04000000;
    struct DisplayWriteQueue_0808fefc *queue =
        (struct DisplayWriteQueue_0808fefc *)0x02002090;
    u16 saved_interrupt_state = *interrupt_master_enable;
    u16 slot;

    /*
     * The original writes the low half of the IME register's own address.
     * Bit zero is clear, so this disables interrupts while retaining the
     * exact value used by the relocated queue code.
     */
    *interrupt_master_enable = (u16)(u32)interrupt_master_enable;
    slot = queue->count;
    if (slot <= 31) {
        struct DisplayWrite_0808fefc *write =
            &queue->entries[slot];

        queue->count = slot + 1;
        write->value =
            field->display_bits | *display_control;
        write->address = 0x04000000;
        write->control = 0x00020000;
    }
    *interrupt_master_enable = saved_interrupt_state;
}

/*
 * Dispatch one field-transition command.  The high byte selects the effect
 * family and the low byte supplies its mode; every path queues the resulting
 * display-control write before returning.
 */
void Func_0808fefc(s32 command, s32 value)
{
    struct FieldState_0808fefc *field = Data_03001e70;
    u8 selector = (u8)(command >> 8);
    u8 mode = (u8)command;

    switch (selector) {
    case 0:
        Func_08003b70(0);
        Func_08003bb4(value);
        Func_080030f8(1);
        break;

    case 1:
        Func_08091220(
            0x8000,
            *(volatile u16 *)0x05000000);
        Func_08091254(value);
        Func_080030f8(1);
        QueueDisplayWrite_0808fefc(field);
        Func_08091240(0);
        return;

    case 2: {
        struct EffectState_0808fefc *effect =
            Func_0808fecc();

        effect->mode = mode;
        effect->parameter = 0;
        effect->limit = 0x3f;
        effect->enabled = 1;
        Func_080041d8(0x0808f52d, 0xc80);
        Func_080041d8(0x0808f499, 0x480);
        Func_080030f8(1);
        QueueDisplayWrite_0808fefc(field);
        effect->x = 0;
        effect->z = 0x20;
        effect->value = (s8)value;
        effect->reserved = 0;
        return;
    }

    case 3: {
        struct EffectState_0808fefc *effect =
            Func_0808fecc();

        effect->mode = mode;
        effect->parameter = 0x20;
        Func_080907b0(15);
        Func_080030f8(1);
        Func_080041d8(0x08090659, 0xc80);
        QueueDisplayWrite_0808fefc(field);
        effect->x = 0;
        effect->z = 0x20;
        effect->value = (s8)value;
        effect->reserved = 0;
        return;
    }

    case 4: {
        struct EffectState_0808fefc *effect;
        u32 callback;

        field = Data_03001e70;
        effect = Func_0808fecc();
        field->transition_x = 0x50;
        field->transition_z = 0x50;
        Func_080030f8(1);
        if (mode == 0)
            callback = 0x080903bd;
        else
            callback = 0x08090489;
        Func_080041d8(callback, 0xc80);
        Func_0800307c(1, 0, 0x08090585);
        effect->x = 0x50;
        effect->z = 0;
        effect->value = (s8)value;
        effect->reserved = 0;
        break;
    }

    default:
        break;
    }

    QueueDisplayWrite_0808fefc(field);
}

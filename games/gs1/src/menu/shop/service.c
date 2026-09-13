#include "types.h"

struct ShopServiceWork {
    u8 unk_000[0x380];
    void *mode_state;
    u8 unk_384[0x20];
    u16 value;
    u8 unk_3a6[4];
    s8 mode;
};

u8 *Runtime_GetObject(s32);
extern struct ShopServiceWork *Data_03001f2c;
s32 Shop_CanServe(s32 selection, s32 variant);
extern u8 Value_00000d24;
extern u8 Value_00000d2e;
extern u8 Value_00000d38;
extern u8 Value_00000d42;
s32 Func_0808a540(u16);
void UiWork_FinalizePending(void);
s32 Shop_MsgByMode(s32 value);
void UiWork_Create(s32, s32, s32, s32);
s32 UiWork_IsCompleteFar(void);
void WaitFrames(u32);

s32 Shop_ServicePrice(s32 entry_no, s32 kind)
{
    u8 value = Runtime_GetObject(entry_no)[0xF];
    s32 result = 0;

    if (kind == 0) {
        result = value * 20;
    } else if (kind == 1) {
        result = 10;
    } else if (kind == 2) {
        result = 50;
    } else if (kind == 3) {
        result = value * 10;
    }
    return result;
}

s32 Shop_CanServe(s32 entry_no, s32 kind)
{
    u8 *entry = Runtime_GetObject(entry_no);
    s32 result = 0;

    if ((kind == 0 && *(s16 *)(entry + 56) <= 0)
        || (kind == 1 && *(s8 *)(entry + 305) != 0)
        || (kind == 2 && entry[320] != 0)
        || (kind == 3 && *(s8 *)(entry + 304) != 0)) {
        result = 1;
    }
    return result;
}

s32 Shop_CountUnits(void)
{
    u8 *work = (u8 *)Data_03001f2c;
    u8 *base;
    s32 active = 0;
    s32 variant = (s8)work[0x3AA];
    s32 index = 0;
    s32 offset;

    if (active < *(s8 *)(work + 0x3A7)) {
        base = work + 2;
        offset = 0x36C;
        do {
            if (Shop_CanServe(*(s16 *)(base + offset), variant) != 0)
                active++;
            index++;
            offset += 2;
        } while (index < *(s8 *)(work + 0x3A7));
    }

    return active;
}

s32 Shop_MsgByMode(s32 value)
{
    s8 mode = Data_03001f2c->mode;

    if (mode == 1) {
        value += (u32)&Value_00000d2e - (u32)&Value_00000d24;
    }
    if (mode == 2) {
        value += (u32)&Value_00000d38 - (u32)&Value_00000d24;
    }
    if (mode == 3) {
        value += (u32)&Value_00000d42 - (u32)&Value_00000d24;
    }
    return value;
}

void UiMessage_ShowResolvedAndWait(s32 value)
{
    s32 no;

    no = Func_0808a540(Data_03001f2c->value);
    UiWork_FinalizePending();
    value = Shop_MsgByMode(value);
    UiWork_Create(value, 5, 0, (no << 0x10) | 0x22);
    while (UiWork_IsCompleteFar() == 0) {
        WaitFrames(1U);
    }
    WaitFrames(1U);
}

void UiMessage_ShowResolvedAndRestoreState(s32 arg0)
{
    struct ShopServiceWork *state;
    void **slot;
    s32 value;
    u8 saved;

    state = Data_03001f2c;
    slot = &state->mode_state;
    saved = *(u8 *)((u8 *)*slot + 5);
    value = Func_0808a540(state->value);
    arg0 = Shop_MsgByMode(arg0);
    *(u8 *)((u8 *)*slot + 5) = 13;
    UiWork_FinalizePending();
    UiWork_Create(arg0, 5, 0, (value << 16) | 0x22);
    while (UiWork_IsCompleteFar() == 0)
        WaitFrames(1);
    WaitFrames(1);
    *(u8 *)((u8 *)state->mode_state + 5) = saved;
}

typedef unsigned char u8;
typedef signed int s32;

struct State_0808c2dc {
    u8 padding[0x1f8];
    u8 values[1];
};

extern struct State_0808c2dc Data_02000240;

s32 Func_08077148(s32);
void Func_08077120(s32, s32);

void Func_0808c2dc(s32 owner)
{
    s32 count = Func_08077148(owner);

    if (count > 0) {
        u8 *base = (u8 *)&Data_02000240;
        s32 offset = 252 << 1;
        u8 *value = base + offset;
        s32 remaining = count;

        do {
            Func_08077120(*value++, owner);
            remaining--;
        } while (remaining != 0);
    }
}

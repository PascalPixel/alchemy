typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

void Func_0801b9a8(void *state, u16 index);
void Func_0801b9ec(void *state, u16 index);
void Func_0801ba68(void *state, u32 mode);
void Func_0801b010(u16 type, u32 value);
void Func_080030f8(u32 value);

void Func_0801b5c0(u8 *state)
{
    u32 *selection = (u32 *)(state + 0x39c);

    if (*selection != 0) {
        u32 savedIndex;

        Func_0801b9a8(state, *(u16 *)(state + 0x39e));
        {
            u16 *status = (u16 *)(state + 0x3a2);
            u32 value = 33;

            *status = value;
        }
        Func_080030f8(1);
        savedIndex = *(u16 *)(state + 0x39e);
        if (savedIndex == 1 && *(u16 *)selection != 0) {
            {
                u32 value = 8;

                *(u16 *)(state + 8) = value;
            }
            *(u16 *)selection += 0xffff;
            Func_0801ba68(state, 0);
            if (*(u16 *)selection == 0) {
                *(u16 *)(state + 10) = 0;
            }
            *(u16 *)(state + 62) = savedIndex;
        } else {
            *(u16 *)(state + 0x39e) += 0xffff;
        }
        {
            u16 *status = (u16 *)(state + 0x3a2);
            u32 value = 1;

            *status = value;
            Func_0801b9ec(state, *(u16 *)(state + 0x39e));
        }
        Func_080030f8(1);
        Func_0801b010(*(u16 *)(*(u8 **)(state + 0x348) + 10), 0);
        Func_080030f8(1);
    }
}

typedef signed int s32;
typedef void (*Callback)(s32 *);

void Func_080048b0(s32, s32);
void Func_08002dd8(s32);
extern Callback Data_080ee2b4[];

void Func_080d6660(s32 *state)
{
    s32 index;
    s32 **destination;

    Func_080048b0(41, 0x302);
    Func_080048b0(39, 0x782c);
    Func_080048b0(40, 0x4000);

    destination = (s32 **)(*(s32 *)0x03001eec + 0x7828);
    index = state[0];
    *destination = state;
    if (index == 0)
        state[6] = 0;
    else
        Data_080ee2b4[index - 1](state);

    Func_08002dd8(40);
    Func_08002dd8(39);
    Func_08002dd8(41);
}

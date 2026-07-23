typedef unsigned char u8;
typedef signed int s32;

void Func_0801e858(u8 *, s32, s32, s32);

void Func_08020b64(s32 output, u8 *input)
{
    u8 text[20];
    s32 length = 0;

    if (*input != 0) {
        do {
            text[length] = *input;
            input++;
            length++;
        } while (*input != 0);
    }

    text[length++] = 8;
    text[length++] = 2;

    while (length <= 6) {
        text[length++] = 95;
    }

    text[length++] = 8;
    text[length++] = 15;
    text[length] = 0;
    Func_0801e858(text, output, 0, -2);
}

#include "TYPES.H"
#include "SCENE.H"
extern u8 gNumberTextBuffer[];

/* ui/text/fmt/text_format_hex_to_work.c */
/* ui/text/fmt/format_hex_to_work.c */
extern const u8 RomBytes_0800795c[];

void Text_FormatHexToWork(u32 value)
{
    u8 *buffer = gNumberTextBuffer;
    const u8 *digits = RomBytes_0800795c;
    s32 index = 7;

    do {
        buffer[index] = digits[value & 0xF];
        value >>= 4;
        index--;
    } while (index >= 0);

    {
        u8 *terminator = gNumberTextBuffer;
        terminator[8] = 0;
    }
}

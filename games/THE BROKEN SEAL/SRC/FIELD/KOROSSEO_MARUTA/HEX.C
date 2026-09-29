/* Writing a word as hexadecimal text. */
#include "LOG_ROLLING.H"

/* Complete eight-byte state setter plus its sole four-byte pool word. */
void WriteU32AsHex(u8 *hex_text, u32 value)
{
    s32 digit_index;

    hex_text += 8;
    *hex_text = 0;
    hex_text--;
    for (digit_index = 7; digit_index >= 0; digit_index--) {
        *hex_text = gColossoHexChars[value & 15];
        value >>= 4;
        hex_text--;
    }
}

void ColossoLogRollingStage_NoopSceneHook(void)
{
}

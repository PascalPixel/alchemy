typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

u8 *Func_0800a97c(const u8 *source, u8 *destination)
{
    s32 token;
    u32 length;
    u32 sentinel;
    u32 control;
    u8 byte;
    u8 *start;
    const u8 *base;
    const u8 *command;
    const u8 *copy;
    const u8 *literal;

    token = source[0] | (source[1] << 8);
    literal = source + 2;
    if (token == 0) {
        return (u8 *)literal;
    }

    command = source + token;
    control = *command;
    sentinel = 128;
    sentinel <<= 1;
    base = literal;
    start = destination;
    goto apply_control;

back_reference:
    length = token >> 12;
    if (length == 0) {
        length = *command + 16;
        command++;
    }
    length += 2;
    copy = base - (token & 0x0fff);
    while (length != 0) {
        *destination++ = *copy++;
        length--;
    }

shift:
    control >>= 1;
    if (control == 0) {
        control = *command;
        sentinel = 128;
        sentinel <<= 1;
    apply_control:
        command++;
        control |= sentinel;
    }

dispatch:
    if ((control & 1) != 0) {
        if (control != 1) {
            *destination++ = *literal++;
        }
        goto shift;
    }

    byte = *command++;
    token = byte << 8;
    token |= *command++;
    if (token != 0) {
        goto back_reference;
    }
    return start;
}

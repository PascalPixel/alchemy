typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

struct Runtime08005ae0 {
    u8 bytes[0x103F];
};

s32 Func_08005ae0(void)
{
    struct Runtime08005ae0 *runtime;
    u32 limit;
    u32 offset;
    s32 sum;

    runtime = *(struct Runtime08005ae0 **)0x03001F1C;
    limit = 0xFE7;
    sum = 0;
    offset = 0;
    do {
        sum += runtime->bytes[offset + 0x50];
        sum += runtime->bytes[offset + 0x51];
        sum += runtime->bytes[offset + 0x52];
        sum += runtime->bytes[offset + 0x53];
        sum += runtime->bytes[offset + 0x54];
        sum += runtime->bytes[offset + 0x55];
        sum += runtime->bytes[offset + 0x56];
        sum += runtime->bytes[offset + 0x57];
        offset += 8;
    } while (offset <= limit);
    return sum;
}

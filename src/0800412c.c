typedef unsigned int u32;

void Func_0800412c(u32 *destination, u32 *source, u32 byteCount)
{
    u32 index;

    byteCount >>= 2;
    for (index = 0; index < byteCount; index++)
        *destination++ = *source++;
}

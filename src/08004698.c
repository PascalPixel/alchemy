typedef unsigned short u16;
typedef unsigned int u32;

extern u16 *Data_03001cbc;

void Func_08004698(u32 count)
{
    u16 *destination = Data_03001cbc;
    u32 index;

    for (index = 0; index < count; index++)
        *destination++ = 0xf000;
    Data_03001cbc = destination;
}

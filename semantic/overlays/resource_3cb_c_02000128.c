/* Index-remap one word from the source table into the destination table. */
typedef unsigned int u32;
typedef unsigned char u8;

void Func_02000128(u32 index)
{
    u32 destination = ((const u8 *)0x02009940)[index];
    ((u32 *)0x02002224)[destination] = ((const u32 *)0x02009928)[index];
}

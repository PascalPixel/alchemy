typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

extern s32 Func_08006878(void);
extern volatile u16 Data_04000204;
extern void *Data_08007a0c[];
extern volatile u32 Data_02004c04;
extern volatile u32 Data_02004c10;
extern volatile u32 Data_02004c14;
extern volatile u32 Data_02004c00;
extern volatile u32 Data_02004c18;
extern void *volatile Data_02004c08;

s32 Func_08006910(void)
{
    s32 result;
    u32 waitValue;
    u32 id;
    void *volatile *cursor;
    void *entry;
    volatile u32 *destination;

    waitValue = Data_04000204;
    waitValue &= 0xfffc;
    waitValue |= 3;
    Data_04000204 = waitValue;
    id = (u16)Func_08006878();
    cursor = Data_08007a0c;
    result = 1;
    goto check;
next:
    cursor++;
check:
    entry = *cursor;
    if (FIELD(entry, u8, 0x28) == 0)
        goto done;
    if (id != FIELD(entry, u16, 0x28))
        goto next;
    result = 0;
done:
    destination = &Data_02004c04;
    *destination = FIELD(*cursor, u32, 0);
    destination = &Data_02004c10;
    *destination = FIELD(*cursor, u32, 4);
    destination = &Data_02004c14;
    *destination = FIELD(*cursor, u32, 8);
    destination = &Data_02004c00;
    *destination = FIELD(*cursor, u32, 0xc);
    destination = &Data_02004c18;
    *destination = FIELD(*cursor, u32, 0x10);
    Data_02004c08 = (u8 *)*cursor + 0x14;
    return result;
}

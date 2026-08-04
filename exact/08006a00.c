typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern s32 Data_02004c18;
extern volatile u16 Data_04000200;
extern volatile u16 Data_04000208;
extern volatile u8 Data_02004c20;
extern u16 Data_02004c22;
extern u8 Data_02004c24;
extern u16 *volatile Data_02004c28;
extern u16 Data_02004c2c;

void Func_08006a00(u8 index)
{
    s32 *table = &Data_02004c18;
    u16 *entry = (u16 *) (index * 6 + *table);
    u16 *cursor;

    Data_02004c2c = Data_04000208;
    Data_04000208 = 0;
    Data_04000200 |= 8 << Data_02004c20;
    Data_04000208 = 1;
    Data_02004c24 = 0;
    Data_02004c22 = *entry;
    entry++;
    cursor = Data_02004c28;
    *cursor = *entry;
    cursor++;
    Data_02004c28 = cursor;
    *cursor = entry[1];
    cursor--;
    Data_02004c28 = cursor;
}

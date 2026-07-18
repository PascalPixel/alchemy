typedef signed char s8;
typedef unsigned int u32;
typedef signed int s32;

extern s8 Data_0809f160[];

s32 Func_08004458(void);
void Func_08009240(s32, s8);

void Func_0809ad70(s32 arg0) {
    s8 *table = Data_0809f160;
    s32 index = Func_08004458();
    Func_08009240(arg0, table[(u32)(index * 8) >> 16]);
}

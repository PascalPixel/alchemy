typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
u32 Func_080072e8(u32, s32);
void Func_080fa678(u32 arg0)
{
  s32 new_var;
  int new_var2;
  new_var2 = 0x02004088;
  new_var2 = *((s32 *) new_var2);
  new_var = new_var2;
  Func_080072e8(arg0, new_var);
}


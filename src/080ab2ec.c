typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
void Func_080ab21c(s32, s32, s32, s32, s32);
void Func_080ab2ec(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
  int new_var;
  new_var = 0xC;
  Func_080ab21c(((*((u16 *) (((u8 *) arg0) + new_var))) + arg1) + 1, ((*((u16 *) (((u8 *) arg0) + 0xE))) + arg2) + 1, arg3, arg4, arg5);
}


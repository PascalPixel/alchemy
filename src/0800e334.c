typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
void Func_0800e334(void *arg0, s32 arg1, int arg2)
{
  s8 var_r1;
  void *new_var;
  if (arg1 == 0)
  {
    *((u16 *) (((u8 *) arg0) + 0x20)) = arg2;
    return;
  }
  if (arg1 == 1)
  {
    *((u16 *) (((u8 *) arg0) + 0x20)) = (u16) ((*((u16 *) (((u8 *) (new_var = arg0)) + 0x20))) + arg2);
    return;
  }
  var_r1 = 0;
  if ((*((u16 *) (((u8 *) arg0) + 0x20))) == ((s16) arg2))
  {
    var_r1 = 1;
  }
  *((s8 *) (((u8 *) arg0) + 0x57)) = var_r1;
}


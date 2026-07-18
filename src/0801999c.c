typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
s32 Func_080f9048();
s32 Func_0801999c(void *arg0)
{
  int new_var;
  s32 var_r5;
  var_r5 = 0;
  if (((*((u8 *) (((u8 *) (*((void **) 0x03001E8C))) + 0x12F9))) != 0) && (Func_080f9048() == 0))
  {
    var_r5 = 1;
    if (1)
    {
    }
  }
  new_var = 0;
 ;
  if ((*((s32 *) 0x03001AE8)) & 0x303)
  {
    var_r5 = 1;
  }
  if (var_r5 != new_var)
  {
    *((s16 *) (((u8 *) arg0) + 0x14)) = new_var;
    return 1;
  }
  return new_var;
}

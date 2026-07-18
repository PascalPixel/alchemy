typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
volatile unsigned int Func_080030f8(s32);
void Func_08006488(void)
{
  int new_var2;
  unsigned int new_var;
  u32 var_r5;
  int new_var3;
  var_r5 = 0;
  if ((*((s32 *) 0x020023AC)) != 0)
  {
    new_var2 = 0x020023AC;
    loop_2:
    Func_080030f8(1);

    var_r5 += 1;
    new_var = new_var2;
    new_var3 = 0;
    if (var_r5 <= 0x927BFU)
    {
      if ((*((s32 *) new_var)) != new_var3)
      {
        goto loop_2;
      }
    }
  }
}


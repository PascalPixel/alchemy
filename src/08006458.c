typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
s32 Func_080030f8(s32);
void Func_08006458(void)
{
  unsigned int new_var;
  u32 var_r5;
  var_r5 = 0;
  if ((*((s32 *) 0x02002080)) != 0)
  {
    new_var = 0x02002080;
    loop_2:
    Func_080030f8(1);

    var_r5 += 1;
    if (var_r5 <= 0x927BFU)
    {
      if ((*((s32 *) new_var)) != 0)
      {
        goto loop_2;
      }
    }
  }
}


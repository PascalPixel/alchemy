typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
void Func_08098294(int arg0)
{
  s32 var_r0;
  u8 *new_var;
  u8 temp_r4;
  void *temp_r1;
  void *temp_r2;
  void *var_ip;
  var_ip = *((void **) 0x03001E64);
  var_r0 = 0x3F;
  do
  {
    if (1)
    {
      if ((*((s32 *) (((u8 *) var_ip) + 0))) != 0)
      {
        temp_r4 = *(new_var = (u8 *) (((u8 *) var_ip) + 0x54));
        if (temp_r4 == 1)
        {
          temp_r1 = *((void **) (((u8 *) var_ip) + 0x50));
          temp_r2 = *((void **) (((u8 *) temp_r1) + 0x28));
          if ((*((s16 *) (((u8 *) temp_r2) + 0))) == 0xC8)
          {
            *((s8 *) (((u8 *) temp_r2) + 5)) = arg0;
            *((u8 *) (((u8 *) temp_r1) + 0x25)) = temp_r4;
          }
        }
      }
      var_r0 -= 1;
    }
    var_ip += 0x70;
  }
  while (var_r0 >= 0);
}


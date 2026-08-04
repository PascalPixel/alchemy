typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32;
struct S {
  u8 f0[6]; u16 f6; u8 f8[4]; u32 fC; u8 f10[8]; u32 f18; u32 f1C; u8 f20[0x34]; s8 f54;
};
/* +0x18/+0x1cを0x280減らし、+6/+0x0cを加算する。 */
void Func_0809b0dc(struct S *arg0) {
  u32 temp_r2;
  arg0->f1C -= 0x280;
  temp_r2 = arg0->f18 - 0x280;
  arg0->f6 = arg0->f6 + 0x2000;
  arg0->fC = arg0->fC + 0x10000;
  arg0->f18 = temp_r2;
  if ((s32)temp_r2 < 0x3000) {
    arg0->f54 = 0;
  }
}

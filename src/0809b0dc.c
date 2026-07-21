typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32;
struct S {
  u8 f0[6]; u16 f6; u8 f8[4]; s32 fC; u8 f10[8]; s32 f18; s32 f1C; u8 f20[0x34]; s8 f54;
};
void Func_0809b0dc(struct S *arg0) {
  s32 temp_r2;
  arg0->f1C = arg0->f1C + 0xFFFFFD80;
  temp_r2 = arg0->f18 + 0xFFFFFD80;
  arg0->f6 = arg0->f6 + 0x2000;
  arg0->fC = arg0->fC + 0x10000;
  arg0->f18 = temp_r2;
  if (temp_r2 < 0x3000) {
    arg0->f54 = 0;
  }
}

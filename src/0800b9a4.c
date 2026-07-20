typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))
s32 Func_0800bc48();
void Func_0800b9a4(s32 arg0, u32 arg1) {
 s32 *p; s32 t3; s32 off; s32 v; s32 cnt; u32 i;
 if ((arg0 != 0) && (arg1 <= 3U)) {
  off = (arg1 * 4) + 0x28;
  v = M2C_FIELD(arg0, s32, off);
  if (v != 0) {
   Func_0800bc48(v);
   M2C_FIELD(arg0, s32 *, off) = 0;
   i = arg1 + 1;
   cnt = 0;
   if (i <= 3U) { p = (s32 *)((i * 4) + arg0 + 0x28);
    do { t3 = *p++; if (t3 != 0) { cnt += 1; } i += 1; } while (i <= 3U); }
   if (cnt == 0) { M2C_FIELD(arg0, s8, 0x27) = (s8) arg1; }
  }
 }
}

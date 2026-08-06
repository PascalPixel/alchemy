#include "types.h"

  
  
  
  
  
  
  
 

extern s32 Func_02001e32(s32);
extern s32 Func_02001e3c(s32);
extern void Func_02001f68(s32);
extern void Func_02001e86(void);
extern u8 * Func_02001eb4(s32);
extern void Func_02001efa(s32,s32,s32);
extern void Func_02001ed4(s32,s32,s32);
extern void Func_02001f10(s32,s32,s32);
extern void Func_02001f9a(s32,s32);
extern void Func_02001fb4(s32,s32,s32,s32);
extern void Func_02001f36(s32,s32,s32);
extern void Func_02001fca(void);
extern void Func_02001f7c(s32,s32,s32);
extern void Func_02001ef2(s32);
extern void Func_02001f98(s32);
extern void Func_02001fba(s32,s32,s32);
extern void Func_02001f82(s32,s32);
extern s32 Func_020009ae(void);
extern void Func_02001fb8(s32);
extern void Func_02001fd0(s32,s32);
extern void Func_020009d8(void);
extern void Func_02001e8a(s32);
extern void Func_02001f50(s32);
extern void Func_02002030(s32);
extern void Func_02002044(void);
extern void Func_02002050(void);
extern void Func_02001f5c(void);
void Func_020004a0(void)
{
    u8 *workspace = *(u8 **)0x03001ebc;
    s32 state = *(s16 *)(workspace + 364);
    u8 *leader;

    if (Func_02001e32(0x855) != 0 || Func_02001e3c(0x856) == 0) {
        Func_02001f68(state - 19);
        return;
    }

    Func_02001e86();
    leader = Func_02001eb4(0);
    if (leader != 0)
        Func_02001efa(2, *(s32 *)(leader + 8), *(s32 *)(leader + 16));
    Func_02001ed4(2, 0xcccc, 0x6666);

    if (state == 20) {
        Func_02001f10(2, 400, 448);
    } else {
        Func_02001f9a(0xcccc, 0x1999);
        Func_02001fb4(0x00e00000, -1, 0x00a20000, 1);
        Func_02001f36(2, 224, 162);
        Func_02001fca();
    }

    Func_02001f7c(0, 2, 0);
    Func_02001ef2(20);
    Func_02001f98(0x1327);
    Func_02001fba(0x9002, 0, 20);
    Func_02001f82(0, 3);
    if (Func_020009ae() != 0) {
        Func_02001fb8(0x132a);
        Func_02001fd0(2, 0);
        Func_020009d8();
        Func_02001e8a(20);
    }
    Func_02001f50(2);
    Func_02002030(state - 19);
    Func_02002044();
    Func_02002050();
    Func_02001f5c();
}

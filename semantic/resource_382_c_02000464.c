#include "types.h"
 
  

extern void Func_02001f28(void);
extern u8 * Func_02001dde(int);
extern void Func_02001e26(int,int,int,int);
extern void Func_02001f54(int);
extern void Func_02001e0a(int);
extern void Func_02001e10(int);
void Func_02000464(void)
{
    Func_02001f28();
    if ((*(u32 *)(Func_02001dde(2) + 248) & 1) != 0) {
        Func_02001e26(2, 0, 0, 0);
        Func_02001f54(126);
        Func_02001e0a(0);
        Func_02001e10(2);
    }
}

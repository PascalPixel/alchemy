#include "types.h"
extern u8 *Data_03001ebc;








extern void Func_020025a4(void);
extern int Func_0200258c(int);
extern void Func_020025a6(int);
extern void Func_020025ac(int);
extern void Func_020025b2(int);
extern void Func_020025b8(int);
extern void Func_020025b6(int);
extern void Func_020025bc(int);
extern void Func_02002672(int);
extern void Func_02002686(void);
extern void Func_02002692(void);
extern void Func_020025f6(void);
void Func_02001080(void)
{
    Func_020025a4();
    if(Func_0200258c(0x204)){
        Func_020025a6(0x9a3); Func_020025ac(0x9a5);
        Func_020025b2(0x9a4); Func_020025b8(0x9a6);
        Func_020025b6(0x9a5); Func_020025bc(0x9a4);
    }
    Func_02002672(*(short *)(Data_03001ebc+364));
    Func_02002686(); Func_02002692(); Func_020025f6();
}

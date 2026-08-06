#include "types.h"









extern void Func_0200267a(void);
extern u8 * Func_02002698(int);
extern void Func_0200264c(int,int,int,int,int,int);
extern void Func_0200269a(int);
extern void Func_020026f4(int,int,int);
extern void Func_02002702(int,int,int);
extern void Func_02002712(int,int);
extern void Func_02002780(int);
extern void Func_020026b6(int);
extern void Func_020026da(void);
void Func_02001158(void)
{
    u8 *a;
    Func_0200267a();
    a=Func_02002698(9);
    if((*(int *)(a+8)>>20)>42){
        Func_0200264c(108,17,1,1,107,17);
        Func_0200269a(8);
        Func_020026f4(9,0,0);
        Func_02002702(10,0x2b80000,0x1200000);
        Func_02002712(10,3);
        Func_02002780(154);
        Func_020026b6(0x9a5);
    }
    Func_020026da();
}

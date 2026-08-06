#include "types.h"






extern u8 * Func_02002638(int);
extern void Func_02002624(void);
extern u8 * Func_02002642(int);
extern void Func_02002314(int,int,int);
extern void Func_020025fc(int,int,int,int,int,int);
extern void Func_0200263a(int);
extern void Func_0200265e(void);
void Func_020010fc(void)
{
    u8 *a=Func_02002638(8);
    Func_02002624();
    a=Func_02002642(8);
    if((*(int *)(a+8)>>20)<=30){
        Func_02002314(8,0,0);
        Func_020025fc(29,19,1,1,27,19);
        Func_0200263a(0x9a2);
    }
    Func_0200265e();
}

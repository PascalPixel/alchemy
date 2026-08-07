#include "types.h"
extern void Func_02002674(int);
void Func_020011d8(u8 *actor,int target)
{
    int frames=40;
    while(frames-- && *(int *)(actor+12)>target)
        Func_02002674(1);
}

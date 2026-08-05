#include "types.h"
  
  
 
extern void Func_020025a8(void);
extern void Func_02002646(int);
extern void Func_0200261e(int,int);
extern void Func_02002344(int,int,int);
extern void Func_02002670(int,int);
extern void Func_02002638(int,int);
extern int Func_0200259e(int);
extern int Func_020025b4(int);
extern void Func_02002602(void);
void Func_02000bec(void)
{
    Func_020025a8(); Func_02002646(0x1342); Func_0200261e(19, 0); Func_02002344(19, 0, 2);
    Func_02002670(19, 0); Func_02002638(19, 1);
    if (Func_0200259e(231) != -1 && Func_020025b4(0x858) == 0) {
        u8 *workspace = *(u8 **)0x03001ebc; *(s16 *)(workspace + 370) = 1;
    }
    Func_02002602();
}

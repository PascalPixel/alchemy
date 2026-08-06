/* Story-flag-selected actor-8 scene sequence. */
#include "types.h"

 
 
  
  
  


extern u8 * Func_0200857a();
extern u8 * Func_02008582();
extern void Func_02008558();
extern s32 Func_0200853e();
extern void Func_020085c0();
extern void Func_02008566();
extern void Func_0200862e();
extern void Func_02008574();
extern void Func_02008624();
extern void Func_02008656();
extern void Func_0200859c();
extern void Func_02008682();
extern void Func_0200869a();
extern void Func_02008602();
extern void Func_0200865a();
extern void Func_02008622();
extern void Func_020086de();
extern void Func_02008686();
extern void Func_02008690();
extern void Func_020085f6();
extern void Func_020086dc();
extern void Func_020086fe();
extern void Func_020086ae();
extern void Func_020086c8();
extern void Func_0200861e();
extern void Func_020086e6();
extern void Func_0200862c();
extern void Func_0200864c();
extern void Func_02008706();
extern void Func_0200865c();
extern void Func_02008724();
extern void Func_0200866a();
extern void Func_02008762();
extern void Func_020086ca();
extern void Func_02008722();
extern void Func_02008678();
extern void Func_0200869c();
void Func_02003e48(void)
{
    u8 *lead = Func_0200857a(0);
    u8 *actor = Func_02008582(8);

    Func_02008558();
    if (Func_0200853e(0x305)) {
        Func_020085c0(8);
        Func_02008566(10);
        Func_0200862e(8, 2);
        Func_02008574(40);
        if (*(s16 *)(lead + 6) >= 0)
            Func_02008624(8, 7);
        else
            Func_0200862e(8, 8);
        Func_02008656(8, 2);
        Func_0200859c(20);
        Func_02008682(0xed2);
        Func_0200869a(8, 0);
        Func_02008602(8, (void *)0x0200cec8);
        Func_0200865a(8, 6);
    } else {
        Func_02008622(8);
        *(s32 *)(actor + 24) = 0x10000;
        *(s32 *)(actor + 28) = 0x10000;
        Func_020086de(8, 0x1000, 0);
        if (*(s16 *)(lead + 6) >= 0)
            Func_02008686(8, 7);
        else
            Func_02008690(8, 8);
        Func_020085f6(20);
        Func_020086dc(0xed1);
        Func_020086fe(8, 0, 20);
        Func_020086ae(8, 1);
        Func_020086c8(8, 4, 0);
        Func_0200861e(80);
        Func_020086e6(8, 2);
        Func_0200862c(40);
        if (*(s16 *)(lead + 6) >= 0)
            Func_020086dc(8, 7);
        else
            Func_020086e6(8, 8);
        Func_0200864c(2);
        Func_02008706(8, 2, 0);
        Func_0200865c(60);
        Func_02008724(8, 2);
        Func_0200866a(20);
        Func_02008762(8, 0);
        Func_020086ca(8, (void *)0x0200cec8);
        Func_02008722(8, 6);
        Func_02008678(0x305);
    }
    Func_0200869c();
}

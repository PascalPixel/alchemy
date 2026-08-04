#include "types.h"
u8 *Func_0808a080(); void Func_0808a018(); void Func_0808a020();
void Func_0808a010(); void Func_0808a090(); void Func_0808a098();
void Func_0808a0c8(); void Func_0808a0d0(); void Func_0808a0e8();
void Func_0808a130(); void Func_0808a1b8(); void Func_0808a1e8();

/* Complete 224-byte actor-18 setup; repeated fetches retain all 22 calls. */
void Func_02001060(void)
{
    Func_0808a018(); Func_0808a098(18, 1);
    *(s32 *)(Func_0808a080(18) + 108) = 0;
    *(s32 *)(Func_0808a080(18) + 56) = (s32)0x80000000;
    *(s32 *)(Func_0808a080(18) + 64) = (s32)0x80000000;
    *(s32 *)(Func_0808a080(18) + 36) = 0;
    *(s32 *)(Func_0808a080(18) + 44) = 0;
    *(s32 *)(Func_0808a080(18) + 48) = 0;
    *(s32 *)(Func_0808a080(18) + 52) = 0;
    Func_0808a1e8(18, 0x103, 0); Func_0808a130(18, 2); Func_0808a010(60);
    Func_0808a090(18, 0x18000, 0xc000); Func_0808a090(0, 0x18000, 0xc000);
    Func_0808a0c8(18, 280, 232); Func_0808a0d0(0, 296, 232);
    Func_0808a0e8(18); Func_0808a1b8(0, 0x8000, 20);
    Func_0808a1e8(0, 258, 60); Func_0808a098(18, (u8 *)0x020095b0);
    *(s32 *)(Func_0808a080(18) + 108) = 0x02008501;
    Func_0808a020();
}

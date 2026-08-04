#include "types.h"

extern u8 *Data_03001ebc;
extern void Func_02000a84(s32);
extern void Func_080000c0(s32);
extern void Func_080091e0(u8 *, s32);
extern void Func_080770c8(s32);
extern void Func_0808a010(s32); extern void Func_0808a018(void); extern void Func_0808a020(void);
extern void Func_0808a030(void *); extern u8 *Func_0808a080(s32);
extern void Func_0808a090(s32,s32,s32); extern void Func_0808a098(s32,void *);
extern void Func_0808a0a8(s32); extern void Func_0808a0b8(s32,s32,s32);
extern void Func_0808a0d0(s32,s32,s32); extern void Func_0808a0f0(s32,s32,s32);
extern void Func_0808a128(s32,s32,s32); extern void Func_0808a138(s32,s32);
extern void Func_0808a158(s32,s32); extern void Func_0808a1b8(s32,s32,s32);
extern void Func_0808a1e0(s32,s32); extern void Func_0808a1e8(s32,s32,s32);
extern void Func_0808a248(s32); extern void Func_0808a360(void);
extern void Func_0808a368(void); extern void Func_0808a370(void);
extern void Func_080f9010(s32);

/* Complete 896-byte first ensemble scene through its 11-word pool at
 * 0x02000e4c-0x02000e77.  Seven supporting actors are kept explicit so all
 * 91 static scene calls remain visible in machine order. */
void Func_02000af8(void)
{
    u8 *actor;
    s32 frame;
    Func_0808a018();
    Func_0808a158(0,15);
    actor=Func_0808a080(0); Func_080091e0(actor,0);
    Func_080000c0(1); Func_0808a030((void *)0x0200976c);
    Func_080000c0(1); Func_0808a030((void *)0x02009844); Func_080000c0(1);
    Func_02000a84(9); Func_02000a84(10); Func_02000a84(11); Func_02000a84(12);
    Func_02000a84(13); Func_02000a84(14); Func_02000a84(15);
    Func_0808a098(8,(void *)0x0200939c);
    *(s32 *)(Data_03001ebc+448)=515;
    Func_0808a360(); Func_0808a370(); Func_0808a010(300);
    Func_080f9010(147); Func_0808a010(100);
    Func_0808a0a8(9); Func_0808a0a8(10); Func_0808a0a8(11); Func_0808a0a8(12);
    Func_0808a0a8(13); Func_0808a0a8(14); Func_0808a0a8(15);
    Func_0808a090(9,0x30000,0x18000); Func_0808a090(10,0x30000,0x18000);
    Func_0808a090(11,0x30000,0x18000); Func_0808a090(12,0x30000,0x18000);
    Func_0808a090(13,0x30000,0x18000); Func_0808a090(14,0x30000,0x18000);
    Func_0808a090(15,0x30000,0x18000);
    Func_0808a0b8(9,0,100); Func_0808a0b8(10,60,100); Func_0808a0b8(11,120,100);
    Func_0808a0b8(12,180,100); Func_0808a0b8(13,240,100);
    Func_0808a0b8(14,320,100); Func_0808a0b8(15,380,100);
    Func_0808a010(40); Func_0808a1e8(8,0x101,0); Func_0808a010(20);
    Func_0808a0f0(9,0,0); Func_0808a0f0(10,0,0); Func_0808a0f0(11,0,0);
    Func_0808a0f0(12,0,0); Func_0808a0f0(13,0,0); Func_0808a0f0(14,0,0);
    Func_0808a0f0(15,0,0); Func_0808a010(100);
    actor=Func_0808a080(18); *(s32 *)(actor+24)=0x1999; *(s32 *)(actor+28)=0x1999;
    Func_0808a0f0(18,0x00ac0000,0x01540000); Func_0808a0a8(8);
    Func_080000c0(1); Func_0808a138(8,1); Func_0808a1b8(8,0x3000,0);
    Func_080f9010(29); Func_080770c8(0x8f0);
    for(frame=0;frame<32;frame++){ *(s32 *)(actor+24)+=0xccc; *(s32 *)(actor+28)+=0xccc; Func_080000c0(1); }
    Func_0808a1e8(8,0x101,60); Func_0808a138(8,2);
    Func_0808a0d0(8,168,340); Func_0808a0d0(8,200,340);
    Func_0808a1b8(8,0x8000,0);
    actor=Func_0808a080(17); *(s32 *)(actor+8)=0x00ac0000; *(s32 *)(actor+12)=0x00a00000;
    *(s32 *)(actor+16)=0x01540000; *(s32 *)(actor+24)=0x12666; *(s32 *)(actor+28)=0x12666;
    *(s32 *)(actor+68)=0x6666; *(s32 *)(actor+72)=0x30000;
    Func_0808a010(20); Func_0808a128(8,6,20); Func_080f9010(147);
    Func_0808a010(20); Func_0808a098(8,(void *)0x020093ac); Func_0808a010(80);
    Func_0808a1e0(17,1); Func_0808a090(17,0x10000,0x8000); Func_080f9010(153);
    Func_0808a0b8(17,132,360); Func_0808a0b8(18,132,360); Func_0808a010(40);
    Func_0808a0f0(17,0,0); actor=Func_0808a080(8);
    *(s32 *)(actor+24)=0x10000; *(s32 *)(actor+28)=0x10000; *(unsigned short *)(actor+6)=0x5000;
    Func_0808a010(40); *(s32 *)(Data_03001ebc+448)=514;
    Func_0808a368(); Func_0808a370(); Func_0808a248(13); Func_0808a020();
}

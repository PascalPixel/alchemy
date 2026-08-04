#include "types.h"
extern u8 *Func_0808a080(s32); extern void Func_080000c0(s32);
extern s32 Func_08000120(s32); extern s32 Func_08000118(s32);
extern void Func_080091f0(s32,s32,s32); extern void Func_080091f8(void);
extern void Func_0808a010(s32); extern void Func_0808a090(s32,s32,s32);
extern void Func_0808a0b8(s32,s32,s32); extern void Func_0808a0e8(s32);
extern void Func_0808a100(s32,s32); extern void Func_080f9010(s32);
extern void Func_020011d8(u8*,s32); extern void Func_0200013c(s32,s32,s32,s32,s32,s32,s32,s32);
/*
 * Complete 392-byte actor launch/particle controller through its four-word
 * pool.  It winds actor geometry for nine frames, launches and waits for the
 * actor, emits a seventeen-sample curved particle trail, then settles the
 * actor.  All 21 static calls remain in machine order.
 */
void Func_020011f8(s32 actorId,s32 unused1,s32 unused2){
 u8 *a=Func_0808a080(actorId); u8 *part=*(u8**)(a+80); s32 i;
 (void)unused1;(void)unused2; a[0x55]=0;
 for(i=0;i<=8;i++){s32 step; Func_080000c0(1); *(u16*)(part+30)-=0x100; step=Func_08000120(*(u16*)(part+30)); *(s32*)(a+8)-=(step+(step<0))/2; *(s32*)(a+56)=0x80000000;}
 *(s32*)(a+108)=0x020091c9; Func_080f9010(136);
 Func_0808a090(actorId,0x20000,0x10000); Func_0808a0b8(actorId,472,288);
 *(s32*)(a+72)=0xcccc; a[0x55]=3; a[0x22]=0; Func_0808a0e8(actorId);
 Func_020011d8(a,0x400000); Func_080091f0(0x50000,0x50000,0x10000);
 Func_080091f0(-1,-1,0xe666);
 for(i=0;i<=16;i++){s32 x=Func_08000120(i<<12),z=Func_08000118(i<<12); s32 dx=x-(x+(x<0?3:0))/4; s32 dz=z-(z+(z<0))/2; Func_0200013c(*(s32*)(a+8),*(s32*)(a+12),*(s32*)(a+16),dx,0,dz,0,0);}
 Func_0808a0b8(actorId,440,308); Func_0808a0e8(actorId); Func_020011d8(a,0x400000);
 *(s32*)(a+108)=0; *(u16*)(part+30)=0x4000; Func_080f9010(154);
 Func_0808a100(actorId,3); Func_080091f8(); Func_0808a010(10); Func_0808a100(actorId,2);
}

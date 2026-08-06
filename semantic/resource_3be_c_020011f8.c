#include "types.h"
 
 
 
 
 
 
 
/*
 * Complete 392-byte actor launch/particle controller through its four-word
 * pool.  It winds actor geometry for nine frames, launches and waits for the
 * actor, emits a seventeen-sample curved particle trail, then settles the
 * actor.  All 21 static calls remain in machine order.
 */
extern u8 * Func_02002740(s32);
extern void Func_020026aa(s32);
extern s32 Func_020026cc(s32);
extern void Func_0200282a(s32);
extern void Func_020027a0(s32);
extern void Func_020027b6(s32,s32,s32);
extern void Func_020027e4(s32,s32,s32);
extern void Func_0200246a(u8*,s32);
extern void Func_0200278e(s32,s32,s32);
extern void Func_0200279c(s32,s32,s32);
extern s32 Func_02002760(s32);
extern s32 Func_02002766(s32);
extern void Func_02001442(s32,s32,s32,s32,s32,s32,s32,s32);
extern void Func_02002862(s32,s32,s32);
extern void Func_02002880(s32);
extern void Func_02002506(u8*,s32);
extern void Func_02002914(s32);
extern void Func_020028b4(s32,s32);
extern void Func_02002840(void);
extern void Func_02002866(s32);
extern void Func_020028c6(s32,s32);
void Func_020011f8(s32 actorId,s32 unused1,s32 unused2){
 s32 permuted_5;
 s32 permuted_12;
 u8 *a=Func_02002740(actorId); u8 *part=*(u8**)(a+80); u32 i;
 for(i=0;i<=8;i++){s32 step; Func_020026aa(1); *(u16*)(part+30)-=0x100; step=Func_020026cc(*(u16*)(part+30)); *(s32*)(a+8)-=(step+(step<0))/2; *(s32*)(a+56)=0x80000000;}
 *(s32*)(a+108)=0x020091c9; Func_0200282a(136);
 (void)unused1;(void)unused2; a[0x55]=0;
 permuted_5 = 0xcccc; a[0x55]=3; a[0x22]=0; Func_020027a0(actorId);
 Func_020027b6(actorId,0x20000,0x10000); Func_020027e4(actorId,472,288);
 Func_0200246a(a,0x400000); Func_0200278e(0x50000,0x50000,0x10000);
 Func_0200279c(-1,-1,0xe666);
 for(i=0;i<=16;i++){s32 x=Func_02002760(i<<12),z=Func_02002766(i<<12); s32 dx=x-(x+(x<0?3:0))/4; s32 dz=z-(z+(z<0))/2; Func_02001442(*(s32*)(a+8),*(s32*)(a+12),*(s32*)(a+16),dx,0,dz,0,0);}
 *(s32*)(a+72) = permuted_5;
 Func_02002862(actorId,440,308); Func_02002880(actorId); Func_02002506(a,0x400000);
 permuted_12 = 0; *(u16*)(part+30)=0x4000; Func_02002914(154);
 *(s32*)(a+108) = permuted_12;
 Func_020028b4(actorId,3); Func_02002840(); Func_02002866(10); Func_020028c6(actorId,2);
}

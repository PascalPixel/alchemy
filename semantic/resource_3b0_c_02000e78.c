typedef unsigned char u8; typedef signed int s32;
extern u8 *Data_03001ebc;
extern void Func_02000a84(s32);
extern s32 Func_02001130(void);
extern void Func_080000c0(s32); extern void Func_080091e0(u8*,s32);
extern void Func_0808a010(s32); extern void Func_0808a018(void); extern void Func_0808a020(void);
extern void Func_0808a030(void*); extern u8 *Func_0808a080(s32);
extern void Func_0808a090(s32,s32,s32); extern void Func_0808a098(s32,void*);
extern void Func_0808a0a8(s32); extern void Func_0808a0d0(s32,s32,s32);
extern void Func_0808a128(s32,s32,s32); extern void Func_0808a130(s32,s32);
extern void Func_0808a158(s32,s32); extern void Func_0808a170(s32);
extern void Func_0808a188(s32,s32,s32); extern void Func_0808a1e8(s32,s32,s32);
extern void Func_0808a1f0(s32,s32); extern void Func_0808a248(s32);
extern void Func_0808a360(void); extern void Func_0808a368(void); extern void Func_0808a370(void);

/* Complete 552-byte second ensemble scene through its 15-word pool. */
void Func_02000e78(void)
{
    u8 *actor;
    Func_0808a018(); Func_0808a158(0,15); actor=Func_0808a080(0);
    Func_080091e0(actor,0); Func_0808a030((void*)0x0200976c); Func_080000c0(1);
    Func_02000a84(9); Func_02000a84(10); Func_02000a84(11); Func_02000a84(12);
    Func_02000a84(13); Func_02000a84(14); Func_02000a84(15);
    Func_0808a098(8,(void*)0x0200939c); *(s32 *)(Data_03001ebc+448)=0x203;
    Func_0808a360(); Func_0808a370(); Func_0808a010(400);
    Func_0808a0a8(9); Func_0808a0a8(10); Func_0808a0a8(11); Func_0808a0a8(12);
    Func_0808a0a8(13); Func_0808a0a8(14); Func_0808a0a8(15);
    Func_0808a090(9,0x30000,0x18000); Func_0808a090(10,0x30000,0x18000);
    Func_0808a090(11,0x30000,0x18000); Func_0808a090(12,0x30000,0x18000);
    Func_0808a090(13,0x30000,0x18000); Func_0808a090(14,0x30000,0x18000);
    Func_0808a090(15,0x30000,0x18000);
    Func_0808a098(9,(void*)0x02009450); Func_0808a098(10,(void*)0x02009480);
    Func_0808a098(11,(void*)0x020094b0); Func_0808a098(12,(void*)0x020094e0);
    Func_0808a098(13,(void*)0x02009510); Func_0808a098(14,(void*)0x02009540);
    Func_0808a098(15,(void*)0x02009570); Func_0808a010(40);
    Func_0808a130(8,3); Func_0808a1f0(8,0x102); Func_0808a010(120);
    Func_0808a130(8,1); Func_0808a1e8(8,0x100,60);
    Func_0808a090(8,0x10000,0x8000); Func_0808a0d0(8,164,344);
    Func_0808a128(8,4,10); Func_0808a128(8,6,20);
    Func_0808a170(0x1ee4); Func_0808a188(8,0,20);
    *(s32 *)(Data_03001ebc+448)=0x202; Func_0808a368(); Func_0808a370();
    if(Func_02001130()==11) Func_0808a248(15); else Func_0808a248(14);
    Func_0808a020();
}

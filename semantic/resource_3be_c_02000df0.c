typedef unsigned char u8; typedef signed short s16; typedef signed int s32;
extern u8 *Data_03001ebc;
extern s32 Func_080770c0(s32); extern void Func_080770d0(s32);
extern void Func_0808a018(void); extern void Func_0808a020(void); extern void Func_0808a010(s32);
extern u8 *Func_0808a080(s32); extern s32 Func_0808a070(s32,s32);
extern void Func_0808a090(s32,s32,s32); extern void Func_0808a0b8(s32,s32,s32);
extern void Func_0808a0c8(s32,s32,s32); extern void Func_0808a0e0(s32,s32,s32);
extern void Func_0808a0e8(s32); extern void Func_0808a0f0(s32,s32,s32);
extern void Func_0808a100(s32,s32); extern void Func_0808a148(s32,s32,s32);
extern void Func_0808a170(s32); extern void Func_0808a178(s32,s32);
extern void Func_0808a180(s32,s32); extern void Func_0808a1b8(s32,s32,s32);
/* Complete flag-0x98a/0x9a0 actor-11 scene, including its five-word pool. */
void Func_02000df0(void){
 u8 *a;
 if(Func_080770c0(0x98a)||!Func_080770c0(0x9a0))return;
 Func_0808a018(); Func_0808a090(11,0x10000,0x8000);
 a=Func_0808a080(0); if(a)Func_0808a0f0(11,*(s32*)(a+8),*(s32*)(a+16));
 Func_0808a0e0(11,-8,16); Func_0808a0e8(11); Func_0808a1b8(11,0xd000,0);
 Func_0808a010(10); Func_0808a148(0,11,0); Func_0808a170(0x23da); Func_0808a178(11,0);
 if(Func_0808a070(0,0)==0){
  Func_0808a180(11,0); Func_0808a0c8(11,152,232); Func_080770d0(0x9a0);
  Func_0808a0e8(11); Func_0808a100(11,1);
  *(s16*)(0x02000240+452)=0x88; *(s16*)(0x02000240+454)=30;
 }else{
  (*(unsigned short*)(Data_03001ebc+472))++; Func_0808a180(11,0); Func_0808a100(11,2);
  a=Func_0808a080(0); if(a)Func_0808a0b8(11,*(s16*)(a+10),*(s16*)(a+18));
  Func_0808a0e8(11); Func_0808a0f0(11,0,0); Func_0808a010(30);
  Func_0808a100(0,2); Func_0808a0e0(0,0,16); Func_0808a0e8(0); Func_0808a100(0,1);
 }
 Func_0808a020();
}

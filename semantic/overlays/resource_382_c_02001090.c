typedef unsigned char u8;
extern u8 *Func_0808a080(int); extern void Func_080f9010(int); extern void Func_08009180(int,int,int,int,int,int);
extern void Func_0808a010(int); extern void Func_080091c0(int,int,int,int,int,int); extern void Func_02000ef4(int,int,int);
void Func_02001090(void)
{
    u8 *leader = Func_0808a080(0); u8 *sprite = *(u8 **)(leader + 80);
    Func_080f9010(188);
    Func_08009180(42,33,34,16,2,2); Func_08009180(42,35,36,16,2,2); Func_0808a010(4);
    Func_08009180(40,33,34,16,2,2); Func_08009180(40,35,36,16,2,2); Func_0808a010(4);
    Func_080091c0(33,21,2,2,3,16); leader[35] &= (u8)~1; sprite[9] |= 12; Func_02000ef4(64,272,11);
}

typedef unsigned char u8;
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(int);
extern void Func_080091c0(int,int,int,int,int,int);
extern void Func_0808a010(int);
extern void Func_0808a0f0(int,int,int);
extern void Func_0808a100(int,int);
extern void Func_080f9010(int);
extern void Func_080770c8(int);
void Func_02001158(void)
{
    u8 *a;
    Func_0808a018();
    a=Func_0808a080(9);
    if((*(int *)(a+8)>>20)>42){
        Func_080091c0(108,17,1,1,107,17);
        Func_0808a010(8);
        Func_0808a0f0(9,0,0);
        Func_0808a0f0(10,0x2b80000,0x1200000);
        Func_0808a100(10,3);
        Func_080f9010(154);
        Func_080770c8(0x9a5);
    }
    Func_0808a020();
}

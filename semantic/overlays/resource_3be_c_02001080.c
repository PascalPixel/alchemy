typedef unsigned char u8;
extern u8 *Data_03001ebc;
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern int Func_080770c0(int);
extern void Func_080770d0(int);
extern void Func_080770c8(int);
extern void Func_0808a248(int);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
void Func_02001080(void)
{
    Func_0808a018();
    if(Func_080770c0(0x204)){
        Func_080770d0(0x9a3); Func_080770d0(0x9a5);
        Func_080770d0(0x9a4); Func_080770d0(0x9a6);
        Func_080770c8(0x9a5); Func_080770c8(0x9a4);
    }
    Func_0808a248(*(short *)(Data_03001ebc+364));
    Func_0808a368(); Func_0808a370(); Func_0808a020();
}

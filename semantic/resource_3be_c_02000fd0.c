typedef unsigned char u8; extern u8 *Data_03001ebc;
extern void Func_0808a170(int); extern void Func_0808a178(int,int); extern int Func_0808a070(int,int);
extern int Func_080770c0(int); extern void Func_0808a180(int,int);
/* Complete actor-8 state branch through its three-word pool. */
void Func_02000fd0(void)
{
    Func_0808a170(0x23cc);
    Func_0808a178(8,0);
    if(Func_0808a070(0,0)==0){
        if(Func_080770c0(0x950)&&!Func_080770c0(0x96f))
            Func_0808a170(0x23d5);
        Func_0808a180(8,0);
    }else{
        (*(unsigned short*)(Data_03001ebc+472))++;
        Func_0808a180(8,0);
    }
}

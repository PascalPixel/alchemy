typedef unsigned char u8;
extern void Func_080000c0(int);
void Func_020011d8(u8 *actor,int target)
{
    int frames=40;
    while(frames-- && *(int *)(actor+12)>target)
        Func_080000c0(1);
}

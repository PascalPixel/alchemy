typedef unsigned char u8; typedef signed int s32;
void Func_02000104(u8 *o)
{
    *(s32 *)(o+8)+=*(s32 *)(o+68); *(s32 *)(o+12)+=*(s32 *)(o+72);
    *(s32 *)(o+16)+=*(s32 *)(o+76); *(s32 *)(o+24)+=*(s32 *)(o+48);
    *(s32 *)(o+28)+=*(s32 *)(o+52);
    *(unsigned short *)(*(u8 **)(o+80)+30)+=*(unsigned short *)(o+100);
}

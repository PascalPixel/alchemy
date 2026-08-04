typedef unsigned char u8;
typedef signed int s32;

/*
 * Set the two scene-state words at +0x1c0 and +0x1c8, then run the local
 * follow-up service.  The literal word at 0x02000718 belongs to this owner
 * because its opening load references it directly.
 */

extern u8 *Data_03001ebc;
extern void Func_0808a248(void);

void Func_020006f8(void)
{
    *(s32 *)(Data_03001ebc + 0x1c0) = 0x201;
    *(s32 *)(Data_03001ebc + 0x1c8) = 24;
    Func_0808a248();
}

typedef signed short s16;
typedef signed int s32;

/* Remove the marker task and release its selected display slot. */
extern s16 Data_0200cb46;
extern void Func_080000d8(void *callback);
extern void Func_080001b8(s32 slot);

void Func_02003080(void)
{
    Func_080000d8((void *)0x0200ae85);
    Func_080001b8(Data_0200cb46);
    Data_0200cb46 = -1;
}

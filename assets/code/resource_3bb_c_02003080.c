typedef signed short s16;
typedef signed int s32;

/* Remove the marker task and release its selected display slot.
 * Per-site veneers (raw sub_ symbols from the overlay .s). */
extern s16 Data_0200cb46;
extern void Func_02006e36(void *callback);
extern void Func_02006e90(s32 slot);

void Func_02003080(void)
{
    Func_02006e36((void *)0x0200ae85);
    Func_02006e90(Data_0200cb46);
    Data_0200cb46 = -1;
}

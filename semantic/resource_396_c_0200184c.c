extern void Func_080000d8(void (*callback)(void));
extern void Func_02001244(void);

/* Register this overlay's palette-fade callback. */
void Func_0200184c(void)
{
    Func_080000d8(Func_02001244);
}

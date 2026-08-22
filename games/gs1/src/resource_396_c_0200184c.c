/*
 * Register this overlay's palette-fade callback.
 *
 * Both addresses are named at their decoded sites: the reference bl's raw
 * decode is 0x02003282 (the registration import as this stream reaches it,
 * semantically Func_080000d8), and the pool word is 0x02009245 -- the
 * callback lives at 0x02009244 in a later-loaded RAM segment, not at this
 * stream's own same-shaped 0x02001244.
 */
extern void Func_02003282(void (*callback)(void));
extern void Func_02009244(void);

void Func_0200184c(void)
{
    Func_02003282(Func_02009244);
}

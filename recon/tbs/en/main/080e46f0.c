/* Draft, not exact (2026-09-24): 200 of 200 bytes, 32 differing halfwords
   (30 edits; was 37). Step every palette colour one unit towards the colours
   of resource_id. The IWRAM word copy returns a value (ignored) and the green
   and blue masks are a u16 held from the 0x1f link symbol: that halfword pool
   constant has a 60-byte reach, which puts the literal pool before the loop
   as in the reference. Residual: the reference recomputes the buffer address
   from sp after the copy call; here it is kept in r7 across the call, which
   renames the loop registers. */
#include "TYPES.H"

typedef s32 (*CopyWordsFn)(void *destination, const void *source, s32 size);
#define CopyWords(d, s, n) ((CopyWordsFn)0x03001388)((d), (s), (n))

#include "RESOURCE.H"
extern u8 Value_0000001f;
#define MASK mask

void Func_080e46f0(s32 resource_id)
{
    u16 buf[64];
    u16 *pal = (u16 *)0x05000000;
    u16 *dst;
    s32 i;
    u16 mask;
    s32 r;
    s32 g;
    s32 b;
    s32 tr;
    s32 tg;
    s32 tb;

    CopyWords(buf, GetResource(resource_id), 128);
    buf[0] = 0;
    mask = (u16)(s32)&Value_0000001f;
    dst = buf;
    for (i = 0; i != 64; i++) {
        r = *pal & 0x1f;
        g = (*pal >> 5) & MASK;
        b = (*pal >> 10) & MASK;
        tr = dst[i] & 0x1f;
        tg = (dst[i] >> 5) & MASK;
        tb = (dst[i] >> 10) & MASK;
        if (r < tr) {
            r++;
        } else if (r > tr) {
            r--;
        }
        if (g < tg) {
            g++;
        } else if (g > tg) {
            g--;
        }
        if (b < tb) {
            b++;
        } else if (b > tb) {
            b--;
        }
        dst[i] = (b << 10) | (g << 5) | r;
        pal++;
    }
    CopyWords((u16 *)0x05000000, buf, 128);
}

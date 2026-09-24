/* Draft, not exact (2026-09-24): 11 differing halfwords, 200 of 200 bytes.
   The word copies go through an inline wrapper, which brought it from 37.
   Residual: the reference holds the 0x1f mask in sl and the buffer base
   in lr (here the other way round), and builds 128 before 0x05000000 for
   the final copy. The do-while around buf[0] = 0 is a FAKEMATCH. */
#include "TYPES.H"
#include "RESOURCE.H"
typedef void (*CopyWordsFn)(void *destination, const void *source, s32 size);
static __inline__ void CopyWords(void *d, const void *s, s32 n)
{
    ((CopyWordsFn)0x03001388)(d, s, n);
}

extern u8 Data_03001388[];

void Func_080e46f0(s32 resource_id)
{
    u16 buf[64];
    u16 *pal = (u16 *)0x05000000;
    u16 *dst;
    s32 i;
    s32 r;
    s32 g;
    s32 b;
    s32 tr;
    s32 tg;
    s32 tb;
    s32 mask;

    CopyWords(buf, GetResource(resource_id), 128);
    do { buf[0] = 0; } while (0);
    i = 0;
    dst = buf;
    mask = 0x1f;
    do {
        r = *pal & 0x1f;
        g = (*pal >> 5) & 0x1f;
        b = (*pal >> 10) & 0x1f;
        tr = dst[i] & 0x1f;
        tg = (dst[i] >> 5) & 0x1f;
        tb = (dst[i] >> 10) & 0x1f;
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
        i++;
        pal++;
    } while (i != 64);
    CopyWords((void *)0x05000000, buf, 128);
}

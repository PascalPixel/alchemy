/* Draft, not exact (2026-09-24): 37 differing halfwords, equal size (was 16 bytes long). The word copies are ordinary calls through the IWRAM CopyWords pointer, not calls to the call_via_r3 veneer; the residual is register choice in the fade loop.
   FAKEMATCH marks below are empty do-while wraps that only move scheduling
   or register choice; they stay tagged until a real spelling replaces them. */
#include "TYPES.H"
#include "RESOURCE.H"
typedef void (*CopyWordsFn)(void *destination, const void *source, s32 size);
#define CopyWords(d, s, n) ((CopyWordsFn)0x03001388)((d), (s), (n))

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

    CopyWords(buf, GetResource(resource_id), 128);
    do { buf[0] = 0; } while (0); /* FAKEMATCH */
    dst = buf;
    i = 0;
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
        do { dst[i] = (b << 10) | (g << 5) | r; } while (0); /* FAKEMATCH */
        pal++;
        i++;
    } while (i != 64);
    CopyWords((u16 *)0x05000000, buf, 128);
}

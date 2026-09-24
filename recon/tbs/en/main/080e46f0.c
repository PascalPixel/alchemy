/* Draft, not exact (2026-09-24): 200 of 200 bytes, 15 differing halfwords
   (14 edits; was 32). Step every palette colour one unit towards the colours
   of resource_id. The IWRAM word copy is a static inline wrapper, so the
   buffer address is rematerialised from sp for each call, and the target
   colours are read as buf[i] (dst only aliases the buffer), which gives the
   reference its index in ip and buffer base in lr. The green and blue masks
   are a u16 held from the 0x1f link symbol (a halfword pool constant that
   puts the literal pool before the loop). Residual: global allocation
   order; the reference gives the offset induction r6 and the mask r7 (here
   r7 and r6). The mask priority (9 refs over 43 insns) outranks the
   induction (9 refs over 84); the reference must allocate it later. */
#include "TYPES.H"

typedef s32 (*CopyWordsFn)(void *destination, const void *source, s32 size);
static __inline__ void CopyWords(void *destination, const void *source, s32 size)
{
    ((CopyWordsFn)0x03001388)(destination, source, size);
}

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
        tr = buf[i] & 0x1f;
        tg = (buf[i] >> 5) & MASK;
        tb = (buf[i] >> 10) & MASK;
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
        buf[i] = (b << 10) | (g << 5) | r;
        pal++;
    }
    CopyWords((u16 *)0x05000000, buf, 128);
}

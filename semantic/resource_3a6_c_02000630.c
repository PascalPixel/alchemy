#include "types.h"

extern u8 *Data_03001ebc;
extern u8 *Func_0808a080(s32 actorId);

/* Complete 296-byte rectangle search owner.  Resource-local table addresses
 * come directly from the four-word pool at 0748. */
u8 *Func_02000630(s32 *referenceBucketOut, s32 *slotIndexOut,
                  s32 *keyIndexOut)
{
    const s32 *keyCursor = (const s32 *)0x02009e88;
    const s32 *deltaCursor = (const s32 *)0x02009ea0;
    const s32 *referenceDeltas = (const s32 *)0x02009e48;
    u8 *reference = Func_0808a080(0);
    s32 referenceBucket = *(u16 *)(reference + 6) >> 12;
    s32 referenceDelta = referenceDeltas[referenceBucket];
    s32 referenceX = ((*(s32 *)(reference + 8) >> 16) +
                      (referenceDelta >> 16)) >> 4;
    s32 referenceZ = ((*(s32 *)(reference + 16) >> 16) +
                      (s16)referenceDelta) >> 4;
    s32 slotIndex;

    *referenceBucketOut = referenceBucket;
    for (slotIndex = 8; slotIndex <= 65; slotIndex++) {
        u8 *slot = ((u8 **)(Data_03001ebc + 0x14))[slotIndex];
        u8 *record = *(u8 **)(slot + 0x50);
        s32 key = *(s16 *)(record + 0x28);
        u32 keyIndex;

        for (keyIndex = 0; keyIndex <= 5; keyIndex++) {
            const s32 *delta = deltaCursor;
            if (*keyCursor == key) {
                s32 baseX = *(s16 *)(record + 10);
                s32 baseZ = *(s16 *)(record + 18);
                s32 minX = (baseX + delta[0]) >> 4;
                s32 minZ = (baseZ + delta[1]) >> 4;
                s32 maxX = (baseX + delta[2]) >> 4;
                s32 maxZ = (baseZ + delta[3]) >> 4;

                *keyIndexOut = keyIndex;
                if (minX <= referenceX && referenceX < maxX &&
                    minZ <= referenceZ && referenceZ < maxZ) {
                    s32 edge = (keyIndex & 1) != 0
                        ? (*(s32 *)(reference + 8) >> 20)
                        : (*(s32 *)(reference + 16) >> 20);
                    s32 minimum = (keyIndex & 1) != 0 ? minX : minZ;
                    if (minimum != edge) {
                        *slotIndexOut = slotIndex;
                        return slot;
                    }
                }
            }
            keyCursor++;
            deltaCursor += 4;
        }
    }
    return 0;
}

#include "types.h"
#include "scene.h"
#include "abi/resource/metadata/release_slot.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void ResourceMetadata_ReleaseSlot(u8 *rec, u32 no)
{
    void **p;
    void *t;
    s32 off;
    void *v;
    s32 cnt;
    u32 i;

    if (rec != 0 && no <= 3) {
        off = (no * 4) + 0x28;
        v = FIELD_AT_OFFSET(rec, void *, off);
        if (v != 0) {
            Sys_Do(v);
            FIELD_AT_OFFSET(rec, void *, off) = NULL;
            i = no + 1;
            cnt = 0;
            if (i <= 3) {
                p = (void **)((i * 4) + (u32)rec + 0x28);
                do {
                    t = *p++;
                    if (t != 0) {
                        cnt++;
                    }
                    i++;
                } while (i <= 3);
            }
            if (cnt == 0) {
                FIELD_AT_OFFSET(rec, s8, 0x27) = (s8)no;
            }
        }
    }
}

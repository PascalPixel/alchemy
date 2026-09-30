#include "TYPES.H"

struct MetadataSlotState {
    u8 unknown_00[24];
    s32 shifted;
    u8 unknown_1c[4];
    u8 first;
    u8 second;
    u8 third;
    u8 fourth;
    u8 unknown_24[3];
    u8 count;
    s32 slots[4];
};

struct MetadataRecord {
    u8 first;
    u8 second;
    u16 value;
    u8 unknown_04[2];
    u8 third;
    u8 fourth;
};

s32 AnimationObject_Allocate(s32);
struct MetadataRecord *Resource_GetMetadataRecordFar(s32);
void ResourceMetadata_ClearRecord(void *);

void ResourceMetadata_ReleaseSlot(u8 *rec, u32 no)
{
    void **p;
    void *t;
    s32 off;
    void *v;
    s32 cnt;
    u32 i;

    if (rec != NULL && no <= 3) {
        off = no * 4 + 0x28;
        v = *(void **)(rec + off);
        if (v != NULL) {
            ResourceMetadata_ClearRecord(v);
            *(void **)(rec + off) = NULL;
            i = no + 1;
            cnt = 0;
            if (i <= 3) {
                p = (void **)(i * 4 + (u32)rec + 0x28);
                do {
                    t = *p++;
                    if (t != NULL)
                        cnt++;
                    i++;
                } while (i <= 3);
            }
            if (cnt == 0)
                *(s8 *)(rec + 0x27) = (s8)no;
        }
    }
}

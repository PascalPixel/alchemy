#include "EDITION.H"
#include "RESOURCE.H"
#include "DMA.H"

/* Release and the adjacent resource-command metadata bank. The two games
   keep resource/flags at different positions in the same56-byte work extent. */
struct ResourceObjectWork {
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    u8 unknown_00[16];
    u8 resource;
    u8 flags;
    u8 unknown_12[22];
#else
    u8 unknown_00[28];
    u8 resource;
    u8 flags;
    u8 unknown_1e[10];
#endif
    void *children[4];
};

typedef char ResourceObjectWork_size[
    sizeof(struct ResourceObjectWork) == 56 ? 1 : -1
];

s32 Resource_ResetEntry(u32 resource);
void ResourceMetadata_ClearRecord(void *destination);

void ResourceObject_Release(struct ResourceObjectWork *work)
{
    volatile u32 zero;
    s32 count;
    void **child;

    if (work) {
        if (!(work->flags & 1))
            Resource_ResetEntry(work->resource);
        child = work->children;
        count = 3;
        do {
            ResourceMetadata_ClearRecord(*child++);
        } while (--count >= 0);
        zero = 0;
        Dma_Set(&zero, work, 0x85000000 | (sizeof(*work) / 4), (volatile u32 *)0x040000d4);
    }
}

#include "METADATA_LOOKUP.H"
#include "TYPES.H"
#include "SCENE.H"

s32 ResourceMetadata_SumCommandLengths(s32 id, u32 no, s32 cnt)
{
    struct AnimationMetadata *info;
    u8 *p;
    u8 op;
    u8 val;
    s32 sum = 0;

    info = Resource_GetMetadataRecordFar(id);
    if (no >= info->animation_count) {
        return 0;
    }
    p = ((u8 **)info->animation)[no];
    for (;;) {
        op = p[0];
        val = p[1];
        p += 2;
        /* 終端命令は長さへ含めない。 */
        if (op == 254 || op == 241 || op == 253 || op == 239) {
            break;
        }
        if (op == 245 || op == 255 || op <= 238) {
            sum += val;
            cnt--;
            if (cnt == 0) {
                break;
            }
        }
    }
    return sum;
}

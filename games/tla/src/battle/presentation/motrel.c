#include "types.h"
#include "motobj.h"
#include "mem.h"

struct Work0811e7dc {
    u8 unknown_00[0x2c];
    void *buffer;
    u8 object_id;
};

struct Record0811e7dc {
    u8 unknown_00[26];
    u8 flags;
};

/* Func_080219cc の遠隔呼び出し。表の並びが版で違い、英語版と日本語版では 0x08020370 の枠、ほかの版では
   Func_08020340 の名前の枠（どちらも Func_080219cc へ飛ぶ） */
#if defined(TLA_EDITION_EN) || defined(TLA_EDITION_JA)
void Func_08020370(void *record);
#define Func_080219ccFar Func_08020370
#else
void Func_08020340(void *record);
#define Func_080219ccFar Func_08020340
#endif
s32 WaitFrames(s32 frames);
void ResourceMetadata_ReleaseSlotFar(void *record, s32 slot);

// 借りたバッファを返し、モーションの絵の枠を放す
void Func_0811e7dc(struct Work0811e7dc *work)
{
    struct MotionObject *object = BtlObjGet(work->object_id)->object;

    if (work->buffer != NULL) {
        Func_080219ccFar(GetMotionRecord(object, 0));
        WaitFrames(1);
        MemFree(work->buffer);
        work->buffer = NULL;
        {
            struct Record0811e7dc *record = GetMotionRecord(object, 0);

            record->flags = 0x20 | record->flags;
        }
        ResourceMetadata_ReleaseSlotFar(GetMotionRecord(object, 0), 0);
    }
}

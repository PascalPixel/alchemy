#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "gs1_edition.h"

/* ui/text/misc/build_render_entries_mode1.c */
s32 UiText_BuildRenderEntries(s32, s32);

s32 UiText_BuildRenderEntriesMode1(s32 arg0)
{
    return UiText_BuildRenderEntries(arg0, 1);
}

/* ui/runtime/set/set_two_entries_to_999.c */
#if defined(GS1_EDITION_JA)
#define WORK_NO 0x8BE
#else
#define WORK_NO 0x976
#endif

/* 連続する2要素へ0x3e7を設定する。 */
void UiWork_SetTwoEntriesTo999(void)
{
    s16 *work = (s16 *)*(void **)ADDR_03001E8C;
    s32 no = WORK_NO;

    do {
        work[no] = 0x3e7;
        no++;
    } while (no != WORK_NO + 2);
}

/* ui/window/create_with_side_object.c */
#define FIELD_AT_OFFSET(base, type, ofs)     (*(type *)((u8 *)(base) + (ofs)))

extern s32 Localization_LookupEntryId();
extern s32 UiWindow_Create();
extern s32 CreateSideObject();

s32 UiWindow_CreateWithSideObject(s32 arg0, s32 arg1, s32 x, s32 y)
{
    s32 win;
    s32 minus_four;
    s32 ofs;
    void *work;

    work = *(void **)ADDR_03001E8C;
    if (Localization_LookupEntryId(arg0) == -1) {
        return 0;
    }
    minus_four = -4;
    ofs = minus_four;
    if (FIELD_AT_OFFSET(work, u8, RENDER_MODE_OFS) != 0) {
        win = UiWindow_Create(x, y, 6, 5, 2);
        ofs = 0;
    } else {
        win = UiWindow_Create(x, y, 5, 5, 2);
    }
    if (win != 0) {
        CreateSideObject(arg0, arg1, -1, win, ofs, minus_four);
    }
    return win;
}

/* ui/runtime/misc/finalize_entity_matching_localized_id.c */
extern s32 Localization_LookupEntryId();
extern void UiWork_Finalize(struct Work *work, s32 release);

void UiWork_FinalizeEntityMatchingLocalizedId(void)
{
    u8 *base;
    u8 *work;
    s32 id;
    s32 index;
    s32 i;
    u8 *entity;
    s32 offset;

    base = *(u8 **)ADDR_03001E8C;
    work = base + 0x500;
    id = Localization_LookupEntryId();
    if (id == -1)
        return;

    if (*(u16 *)(base + 0x12EE) == id) {
        index = 1;
    } else if (*(u16 *)(base + 0x12EC) == id) {
        index = 0;
    } else {
        return;
    }

    offset = 0x12F0 + index * 2;
    id = *(u16 *)(base + offset);

    for (i = 0; i != 8; i++, work += 36) {
        entity = *(u8 **)work;
        if (*(u8 *)(entity + 4) == 2 && *(u8 *)(entity + 14) == id) {
            UiWork_Finalize(work, 2);
            return;
        }
    }
}

/* ui/icon/count_table_entries.c */
extern u8 gRom[];
extern u8 gRom2[];

/* 2つの境界アドレス間を4バイト単位で数える。 */
s32 Ui_CountIconTableEntries(void)
{
    return (s32)((u32)gRom - (u32)gRom2) >> 2;
}

/* ui/count_second_table_entries.c */
extern u8 gRom[];
extern u8 gRom2[];

s32 Ui_CountSecondTableEntries(void)
{
    return (gRom - gRom2) >> 2;
}

/* ui/render/build_paired_patterns_to_slot.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

typedef struct {
    u8 pad0[0x400];
    u8 f400;
    u8 pad1[0x600 - 0x401];
    s16 f600;
    s16 f602;
    s32 f604;
} T;

extern s32 Runtime_AllocateHeapBlock(s32 no0, s32 no1);

extern s32 Resource_FindFreeSlot(void);
extern s32 Resource_CopyData(s32, s32, u8 *);

extern s32 gRom[];
extern s32 gRom2[];

void Ui_BuildPairedPatternsToSlot(s32 no0, s32 no1, s32 *slot, s32 *ret, s32 flag)
{
    T *work;

    work = Runtime_AllocateHeapBlock(0x11, 0x608);
    work->f604 = gRom[no1];
    work->f600 = 2;
    work->f602 = 2;
    Ui_Apply(work, 0);
    work->f604 = gRom2[no0];
    work->f600 = 2;
    work->f602 = 2;
    Ui_Apply(work, 1);
    if (flag == 0) {
        *slot = Resource_FindFreeSlot();
    }
    *ret = Resource_CopyData(*slot, 0x80, &work->f400);
    Ui_Check(0x11);
}

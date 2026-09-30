#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"

extern u8 Data_03001e8c[];

extern const s16 SideObject_CharacterIdMap[];
extern const s16 SideObject_ActorKindIdMap[];

#define FIELD_AT_OFFSET(base, type, ofs)     (*(type *)((u8 *)(base) + (ofs)))
extern s32 Localization_LookupEntryId();
extern s32 UiWindow_Create();
extern s32 CreateSideObject();

#if defined(TBS_EDITION_JA)
#define WORK_NO 0x8BE
#else
#define WORK_NO 0x976
#endif

/* 連続する2要素へ0x3e7を設定する。 */
void UiWork_SetTwoEntriesTo999(void)
{
    s16 *work = (s16 *)*(void **)((u32)&Data_03001e8c);
    s32 no = WORK_NO;

    do {
        work[no] = 0x3e7;
        no++;
    } while (no != WORK_NO + 2);
}

/* Looks up the entry id paired with value: values below 20 search the
 * character pairs, others the second pair table, whose ids start at 128.
 * Returns -1 when the value is not listed. */
s32 Localization_LookupEntryId(u32 value)
{
    s32 result = -1;
    s32 i = 0;

    if (value < 20) {
        for (;;) {
            s32 key = SideObject_CharacterIdMap[i];

            if (key == -1)
                break;
            if (key == value) {
                i++;
                result = SideObject_CharacterIdMap[i];
                break;
            }
            i += 2;
        }
    } else {
        for (;;) {
            s32 key = SideObject_ActorKindIdMap[i];

            if (key == -1)
                break;
            if (key == value) {
                i++;
                result = SideObject_ActorKindIdMap[i];
                result += 128;
                break;
            }
            i += 2;
        }
    }
    return result;
}

s32 UiWindow_CreateWithSideObject(s32 arg0, s32 arg1, s32 x, s32 y)
{
    s32 win;
    s32 minus_four;
    s32 ofs;
    void *work;

    work = *(void **)((u32)&Data_03001e8c);
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

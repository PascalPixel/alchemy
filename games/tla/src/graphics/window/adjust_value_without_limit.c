//--------------------------------------------------------------------------
// adjust_value_without_limit.c
// ウィンドウ：メッセージを長さの制限なしに文字列にする
//--------------------------------------------------------------------------

#include "types.h"
#include "scene.h"
#include "window.h"
#include "ram_buffer.h"

#define UI_WORK ((struct UiRenderWork *)Ram_HeapSlots->window_tiles)

void UiText_BuildRenderEntries(s32 key, s32 mode);

//--------------------------------------------------------------------------
// メッセージ message を buf に展開する（長さの上限なし）
//--------------------------------------------------------------------------
void Ui_AdjustValueWithoutLimit(s32 message, s32 buf)
{
	UiText_DecodeMessage(message, buf, 0x7fffffff);
}

// Builds message key's text entries and copies them to destination, at most
// capacity - 1 characters and a terminator; returns the characters copied.
s32 UiText_CopyMessageString(s32 key, u16 *destination, u32 capacity)
{
    struct UiRenderWork *work;
    u32 count;

    work = UI_WORK;
    work->count = 0;
    UiText_BuildRenderEntries(key, 1);
    for (count = 0; count < capacity - 1 && (destination[count] = work->entries[count]) != 0;
         count++) {
    }
    destination[count] = 0;
    return count;
}

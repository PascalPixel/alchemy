#include "types.h"
#include "window.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "text_reader.h"
#include "heap_state.h"
#include "dma.h"
#include "ramcode.h"
#include "io_reg.h"
#include "scene.h"
#include "lib.h"
#include "edition.h"
#include "tbs_edition.h"
#include "global_cells.h"
#include "render_input.h"

s32 UiText_CopyMessageString(s32 key, u16 *destination, u32 capacity)
{
    struct UiRenderWork *work;
    u32 count;

    work = (struct UiRenderWork *)gWindowWork[0];
    work->count = 0;
    UiText_BuildRenderEntries(key, 1);
    for (count = 0; count < capacity - 1 &&
        (destination[count] = work->entries[count]) != 0; count++) {
    }
    destination[count] = 0;
    return count;
}

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define DECODE_EUROPEAN
#endif

void UiText_DecodeMessage(s32 message, u16 *text, s32 capacity)
{
    struct TextReader reader;
    void *original;
    s32 (*decode)(struct TextReader *);
    u32 value;
#if !defined(DECODE_EUROPEAN)
    u16 delta;
#endif

    original = ((union HeapState *)&gWorkSlot)->slots[50];
    if (original == NULL)
        RamCode_LoadSlot(50, Text_DecodeSymbol, Text_DecodeSymbolCodeSize);
    decode = (s32 (*)(struct TextReader *))((union HeapState *)&gWorkSlot)->slots[50];
    UiText_LookupMessage(&reader, message);
#if !defined(DECODE_EUROPEAN)
    delta = 0xffff;
#endif
    while ((value = decode(&reader)) != 0) {
        switch (value) {
        case 14:
            capacity -= 3;
            if (capacity <= 0)
                goto done;
            *text++ = value;
#if defined(DECODE_EUROPEAN)
            value = decode(&reader) + 0xffff;
            *text++ = value;
            value = decode(&reader) + 0xffff;
#else
            *text++ = decode(&reader) + delta;
            value = decode(&reader) + delta;
#endif
            break;
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 15:
#if defined(DECODE_EUROPEAN)
        case 29:
#endif
            capacity--;
            if (capacity <= 0)
                goto done;
            *text++ = value;
            value = decode(&reader) + 0xffff;
            break;
        default:
            capacity--;
            if (capacity <= 0)
                goto done;
            break;
        }
        *text++ = value;
    }
done:
    if (original == NULL)
        Runtime_ReleaseHeapBlock(50);
    *text = 0;
}

//--------------------------------------------------------------------------

// clear_word_if_set.c

// メモリ：0 でないワードを 0 にする

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// *p が 0 でなければ 0 にして、p を返す
//--------------------------------------------------------------------------
s32 *Runtime_ClearWordIfSet(s32 *p)
{
	if (*p != 0)
	{
		*p = 0;
	}
	return p;
}

//--------------------------------------------------------------------------

// uiwork.c

// ウィンドウ：メッセージのチャンネルの後始末と、メッセージに埋める値の表

//--------------------------------------------------------------------------

#if !GAME_TLA
#include "io_reg.h"
#include "scene.h"
#include "global_cells.h"
#include "tbs_edition.h"
#include "keystate.h"

void UiWindow_DrawFrame(s32 x, s32 y, s32 width, s32 height);

s32		   AudioCommand_GetStateByteFar();

//--------------------------------------------------------------------------
// ウィンドウをすべて閉じる
// 3 つのチャンネルのウィンドウを閉じ、開閉の動きが終わるまで待ってから、
// 直接開いたウィンドウを閉じる
//--------------------------------------------------------------------------
void UiWork_DrainPending(void)
{
	struct UiRenderWork	 *uw;
	struct UiChannelSlot *ch;
	struct UiWindow		 *win;
	u32					  done;
	struct UiWindow		 *work;
	struct UiWindow		 *poll_work;
	s32					  i;
	u16					  flag;

	uw = (struct UiRenderWork *)gWindowWork[0];
	ch = uw->channels;
	win = uw->windows;
	i = 0;
	do
	{
		work = ch->work;
		if (work != 0 && work->flags != 0)
			UiWork_Finalize(work, 0);
		i++;
		ch++;
	} while (i != 3);

	// 閉じ終えたチャンネルを空ける。動いているものがあれば 1 フレーム待つ
poll:
	done = 1;
	ch = uw->channels;
	i = 0;
	do
	{
		poll_work = ch->work;
		if (poll_work != 0)
		{
			if ((*(s32 *)&poll_work->frame) == 0) // frame と duration がともに 0
			{
				flag = poll_work->flags;
				if (flag == 0)
					ch->work = (struct UiWindow *)(u32)flag;
				else
					done = 0;
			}
			else
			{
				done = 0;
			}
		}
		i++;
		ch++;
	} while (i != 3);
	i = 0;
	if (!done)
	{
		WaitFrames(1);
		goto poll;
	}

	// 直接開いたウィンドウ
	goto win_test;
win_loop:
	if (win->flags != 0)
		UiWork_Finalize(win, 0);
	win++;
	i++;
win_test:
	if (i != WINDOW_COUNT)
		goto win_loop;
}

//--------------------------------------------------------------------------
// チャンネルのウィンドウの効果 4 を 1 フレーム進める
// countdown のあいだ 1 タイル大きい枠を描き、終わったら元の枠に戻す
//--------------------------------------------------------------------------
void UiWork_AdvanceChannelTransition(struct UiChannelSlot *ch)
{
	struct UiWindow *work = ch->work;
	s32				 effect = work->effect;
	s32				 x = work->x;
	s32				 y = work->y;
	s32				 w = work->width;
	s32				 h = work->height;

	if (effect != 4)
		return;

	UiWindow_DrawFrame(x - 1, y - 1, w + 2, h + 2);
	ch->countdown--;
	if (ch->countdown != 0)
		return;

	ch->work->effect = 0;
	UiWindow_EraseBorderRect(x - 1, y - 1, w + 2, h + 2);
	UiWindow_DrawFrame(x, y, w, h);
}

//--------------------------------------------------------------------------
// メッセージに埋める値の表（8 つ）を空にする
//--------------------------------------------------------------------------
void UiWork_ClearValueNameTables(void)
{
	s32					 no;
	struct UiRenderWork *work;

	work = (struct UiRenderWork *)gWindowWork[0];
	no = 0;

	do
	{
		work->values[no] = 0;
		work->names[no] = 0;
		no++;
	} while (no != 8);
}

//--------------------------------------------------------------------------
// 表のあいているところに、種類 kind の値 value を入れる
// （いっぱいなら入れない）
//--------------------------------------------------------------------------
void UiWork_PushValueSlot(u32 value, u32 kind)
{
	struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
	u32 no = 0;
	u32 limit = 8;

	do
	{
		if (work->names[no] == 0)
		{
			work->values[no] = value;
			work->names[no] = kind;
			break;
		}
		no++;
	} while (no != limit);
}

//--------------------------------------------------------------------------
// 表から種類 kind の値をさがす（なければ 0）
// clear : 0 でなければ、見つけたものを表から消す
//--------------------------------------------------------------------------
u32 UiRender_LookupNamedValue(u32 kind, u32 clear)
{
	struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
	u32					 i;
	u32					 value = 0;

	for (i = 0; i < 8; i++)
	{
		if (work->names[i] == kind)
		{
			value = work->values[i];
			if (clear != 0)
			{
				work->values[i] = 0;
				work->names[i] = 0;
			}
			break;
		}
	}
	return value;
}

//--------------------------------------------------------------------------
// メッセージの待ちを打ち切るか
// busy で音の状態が 0 のとき、または A・B・L・R を押しているとき、
// countdown を 0 にして 1 を返す
//--------------------------------------------------------------------------
s32 UiWork_CheckCancelByInput(struct UiChannelSlot *ch)
{
	struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
	s32					 cancel = 0;
	s32					 zero;

	if (work->busy != 0 && AudioCommand_GetStateByteFar() == 0)
		cancel = 1;
	zero = 0;
	if (gKeysHeld & (KEY_A | KEY_B | KEYS_SHOULDERS))
		cancel = 1;
	if (cancel != zero)
	{
		ch->countdown = zero;
		return 1;
	}
	return zero;
}

//--------------------------------------------------------------------------
// メッセージの待ちを打ち切るか（押したキーで見る）
// mode が 0 でなければ、ためておいたキーで見る
//--------------------------------------------------------------------------
s32 UiWork_CheckCancelByModeInput(struct UiChannelSlot *ch)
{
	struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
	s32					 key;
	s32					 cancel = 0;
	u8					 zero;

	if (work->busy != 0 && AudioCommand_GetStateByteFar() == 0)
		cancel = 1;
	key = gKeyState;
	zero = 0;
	if (work->mode != zero)
		key = gKeysPressedLatch;
	if (key & (KEY_A | KEY_B | KEYS_SHOULDERS))
		cancel = 1;
	if (cancel != 0)
	{
		ch->countdown = zero;
		return 1;
	}
	return 0;
}

//--------------------------------------------------------------------------
// 書き終えたチャンネルのウィンドウを閉じ、10 フレーム待つ
//--------------------------------------------------------------------------
void UiWork_FinalizePendingCore(void)
{
	s32					  i;
	struct UiChannelSlot *ch;
	struct UiWindow		 *work;

	ch = ((struct UiRenderWork *)gWindowWork[0])->channels;
	i = 0;
	do
	{
		work = ch->work;
		if (work != NULL && (*(s32 *)&work->frame) == 0 && work->flags != 0 && work->state != 0)
		{
			UiWork_Finalize(work, 2 & work->flags);
		}
		i++;
		ch++;
	} while (i != 3);
	WaitFrames(10);
}
#endif

struct UiChannelSlot *UiText_QueueRenderEntries(struct UiWindow *window, s32 entry, s32 x, s32 y, u16 *colours, s32 flags);
s32 UiWork_IsComplete(void);

void UiText_ShowCenteredMessage(s32 message, s32 mode, s32 y_offset)
{
    struct UiRenderWork *work;
    struct UiWindow *window;
    s32 x;
    s32 y;
    s32 width;
    s32 height;
    s32 entry;

    work = (struct UiRenderWork *)gWindowWork[0];
    x = 8;
    y = 8;
    // the window starts as NULL, though every path that reads it assigns it first; the same zero is the style argument below
    window = NULL;
    entry = UiText_BuildRenderEntries(message, 1);
    if (work->entries[entry] != 0) {
        UiText_GetResourceDimensions(message, &x, &y, (u32 *)&width, (u32 *)&height);
        x = (30 - width) >> 1;
        y = ((15 - height) >> 1) + y_offset;
        if (mode != 0)
            window = UiWindow_Create(x, y, width, height, 0);
        else {
            window = UiWindow_Create(x, y, 0, 0, 2);
            window->width = mode;
            window->height = mode;
        }
        if (UiText_QueueRenderEntries(window, entry, 0, 0, 0, 0) == 0)
            UiWork_Finalize(window, 1);
        else {
            while (UiWork_IsComplete() == 0)
                WaitFrames(1);
            if (mode != 0) {
                UiWork_Finalize(window, 0);
                while (UiWork_IsIdle(window) == 0)
                    WaitFrames(1);
            } else
                UiWork_Finalize(window, 1);
            work->result[0] = 0;
            work->result[1] = 0;
        }
    }
}

s32 UiText_BuildRenderEntriesMode1(s32 arg0)
{
    return UiText_BuildRenderEntries(arg0, 1);
}

void UiText_LookupMessage(struct TextReader *reader, s32 message)
{
    void (*routine)(struct TextReader *, s32);

    RamCode_Load(routine, UiText_LookupMessageArm, UiText_LookupMessageCodeSize);
    routine(reader, message);
    MemFree(routine);
}

extern const void *const Text_MessageContexts[][2];

s32 UiText_DecodeSymbol(struct TextReader *reader)
{
    u32 code;
    u32 hi;
    const u8 *base;
    const u8 *tree;
    const u8 *leaves;
    const u8 *text;
    s32 text_bits;
    s32 tree_bits;
    s32 rank;
    s32 depth;
    s32 bit;
    u32 index;
    u32 half;

    code = reader->previous_code;
    text = (const u8 *)reader->words;
    text_bits = reader->bits;
    tree_bits = 1;
    hi = code >> 8;
    code &= 0xff;
    base = Text_MessageContexts[hi][0];
    base = base + ((const u16 *)Text_MessageContexts[hi][1])[code];
    tree = base;
    leaves = base - 1;
    rank = 0;
    for (;;) {
        bit = tree_bits & 1;
        tree_bits >>= 1;
        if (bit != 0 && tree_bits == 0) {
            tree_bits = *tree++;
            bit = tree_bits & 1;
            tree_bits = (tree_bits >> 1) | 0x80;
        }
        if (bit != 0)
            break;
        bit = text_bits & 1;
        text_bits >>= 1;
        if (bit != 0 && text_bits == 0) {
            text_bits = *text++;
            bit = text_bits & 1;
            text_bits = (text_bits >> 1) | 0x80;
        }
        if (bit == 0)
            continue;
        depth = 0;
        do {
            bit = tree_bits & 1;
            tree_bits >>= 1;
            if (bit != 0 && tree_bits == 0) {
                tree_bits = *tree++;
                bit = tree_bits & 1;
                tree_bits = (tree_bits >> 1) | 0x80;
            }
            if (bit == 0)
                depth++;
            else {
                rank++;
                depth--;
            }
        } while (depth >= 0);
    }
    index = rank * 3;
    if (((index * 4) & 7) == 0) {
        bit = leaves[-(index >> 1)];
        code = bit << 4;
        bit = leaves[-(index >> 1) - 1];
        code |= (u32)bit >> 4;
    } else {
        half = leaves[-(index >> 1)];
        half &= 0xf;
        code = leaves[-(index >> 1) - 1] | (half << 8);
    }
    reader->bits = text_bits;
    reader->previous_code = code;
    reader->words = (const u32 *)text;
    return code;
}

extern s32 CreateSideObject();

void UiWork_SetTwoEntriesTo999(void)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    s32 index = 0;

    do {
        work->resource_ids[index] = 999;
        index++;
    } while (index != 2);
}

s32 Localization_LookupEntryId(u32 value)
{
    s32 result = -1;
    s32 i = 0;

    if (value < 20) {
        for (;;) {
            s32 key = Portrait_CharacterMap[i];

            if (key == -1)
                break;
            if (key == value) {
                i++;
                result = Portrait_CharacterMap[i];
                break;
            }
            i += 2;
        }
    } else {
        for (;;) {
            s32 key = Portrait_SpriteMap[i];

            if (key == -1)
                break;
            if (key == value) {
                i++;
                result = Portrait_SpriteMap[i];
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

    work = gWindowWork[0];
    if (Localization_LookupEntryId(arg0) == -1) {
        return 0;
    }
    minus_four = -4;
    ofs = minus_four;
    if (((struct UiRenderWork *)work)->mode != 0) {
        win = (s32)UiWindow_Create(x, y, 6, 5, 2);
        ofs = 0;
    } else {
        win = (s32)UiWindow_Create(x, y, 5, 5, 2);
    }
    if (win != 0) {
        CreateSideObject(arg0, arg1, -1, win, ofs, minus_four);
    }
    return win;
}

void UiWork_FinalizeEntityMatchingLocalizedId(u32 entry)
{
    struct UiRenderWork *state;
    struct UiWindow *work;
    s32 id;
    s32 index;
    s32 i;
    struct RenderOutput *entity;

    state = (struct UiRenderWork *)gWindowWork[0];
    work = state->windows;
    id = Localization_LookupEntryId(entry);
    if (id == -1)
        return;

    if (state->resource_ids[1] == id) {
        index = 1;
    } else if (state->resource_ids[0] == id) {
        index = 0;
    } else {
        return;
    }

    id = state->output_ids[index];

    for (i = 0; i != WINDOW_COUNT; i++, work++) {
        entity = work->output.head;
        if ((u8)entity->kind == 2 && (u8)entity->index == id) {
            UiWork_Finalize(work, 2);
            return;
        }
    }
}

//--------------------------------------------------------------------------
// glow_update_sine.c
// パレット：BG パレット 14 の色 8〜11 を sin で明滅させる
//--------------------------------------------------------------------------

#include "edition.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "character_menu.h"
#include "animspr.h"
#include "render_input.h"
#include "four_object_motion.h"
#include "field_sprite.h"
#include "types.h"
#include "callback_scheduler.h"
#include "scene.h"
#include "resource.h"
#include "text_render_runtime.h"
#include "lib.h"
#include "text/msg_ids.h"
#include "keystate.h"
#include "save_state.h"
#include "game_state.h"
#include "runtime_interfaces.h"
#include "mem.h"
#include "tbs_edition.h"
#include "window.h"
#include "heap_state.h"
#include "io_reg.h"

#if EDITION_INTERNATIONAL || defined(TLA_EDITION_JA)
#include "types.h"
#include "fixed_math.h"

extern u8 gFrameTick[];

//--------------------------------------------------------------------------
// スケジューラから呼ばれ、4 色を位相をずらした sin で明滅させる
// s = sin / 16384 として RGB555 で 赤 22 + 2s・緑 16 + s・青 20 + s
//--------------------------------------------------------------------------
void PaletteGlow_UpdateSine(void)
{
	s32			 *tick;
	volatile u16 *pal;
	s32			  col;
	s32			  s;
	s32			  i;

	tick = (s32 *)gFrameTick;
	pal = (volatile u16 *)0x050001D0; // BG パレット 14 の色 8
	i = 0;
	do
	{
		s = Trig_Sin((*tick + i * 8) * 0x300); // 1 色ごとに 8 フレームずらす
		col = s / 16384;
		{
			u32 r = col << 1;
			u32 g = col;

			r += 22;
			g += 16;
			col += 20;
			/* FAKEMATCH: the do-while fixes the shift's place in the schedule. */
			do
			{
				col <<= 10;
			} while (0);
			g <<= 5;
			col |= g;
			col |= r;
		}
		/* FAKEMATCH: likewise for the store. */
		do
		{
			*pal++ = col;
		} while (0);
	} while (++i <= 3);
}

#endif

#if EDITION_INTERNATIONAL
#include "types.h"
#include "window.h"
#include "callback_scheduler.h"
#include "tbs_edition.h"
#include "global_cells.h"


void Scheduler_ScheduleCallbackAAfterFrames(void)
{
    Scheduler_AddOrUpdateCallback(PaletteGlow_UpdateSine, 0xC80);
}

void Scheduler_ScheduleCallbackA(void)
{
    Scheduler_RemoveCallback(PaletteGlow_UpdateSine);
}

void UiWindow_FillTilemapRect(struct UiWindow *window, s32 x, s32 y, s32 width, s32 height)
{
    u16 *map = (u16 *)gWindowWork[0];
    s32 sum;

    sum = x + window->x;
    x = sum + 1;
    sum = y + ((struct UiWindow *)window)->y;
    y = sum + 1;

    if (x < 0) {
        width += x;
        x = 0;
    }
    if (x + width > 29)
        width = 30 - x;

    if (y < 0) {
        height += y;
        y = 0;
    }
    if (y + height > 29)
        height = 20 - y;

    if (width > 0 && height > 0) {
        s32 y_ofs = y << 6;
        s32 x_ofs = x << 1;
        s32 offset = y_ofs + x_ofs;
        do {
            s32 remaining = width;
            u16 *cell = (u16 *)(offset + (s32)map);
            if (remaining != 0) {
                do {
                    remaining--;
                    *cell++ = 0xE006;
                } while (remaining != 0);
            }
            height--;
            offset += 64;
        } while (height != 0);
        ((struct UiRenderWork *)map)->dirty = 1;
    }
}

#endif

s32 AnimationObjects_SelectAnimationFar(struct AnimationObject *state, s32 flags);

struct PlacementDescriptor {
    u8 pad_00[0x2c];
    s8 kinds[4];
    u8 pad_30[3];
    s8 variant;
};

struct PlacementState {
    u8 pad_000[0x114];
    struct AnimationObject *objects[4];
    u8 pad_124[0x10];
    s16 x[4];
    u8 pad_13c[8];
    s16 y[4];
    u8 pad_14c[8];
    s32 scale[4];
};

s32 Object_ApplyProjectedPlacementFar(s32, void *, void *, s32);

void ObjectPlacement_CreateGroup(struct RenderInput *origin, s32 x, s32 y,
                                 struct PlacementDescriptor *descriptor)
{
    struct PlacementState *state = (struct PlacementState *)gMenuWork;
    s32 i;
    s32 duration;

    for (i = 0; i < 4 && descriptor->kinds[i] != -1; i++) {
        struct AnimationObject *object =
            ResourceObject_CreateFar(Party_RemapCharacterIdIfEnabledFar(descriptor->kinds[i],
                                        descriptor->variant));

        if (object != 0) {
            AnimationObjects_SelectAnimationFar(object, 1);
            ((struct FieldSprite *)object)->flags = 0;
            ((struct FieldSprite *)object)->priority = 0;
        }

        state->objects[i] = object;
        state->x[i] = (x + origin->x + i * 3) * 8 + 0x10;
        state->y[i] = (y + origin->y) * 8 + 0x10;
        state->scale[i] = 0x10000;
    }

    duration = 200;
    duration <<= 4;
    Scheduler_AddOrUpdateCallback(Menu_UpdateFirstObjectRowPositions, duration);
}

void Menu_ClearFirstObjectRowAndScheduleUpdate(void)
{
    struct PlacementState *state = (struct PlacementState *)gMenuWork;
    s32 i;

    Scheduler_RemoveCallback(Menu_UpdateFirstObjectRowPositions);
    for (i = 0; i < 4; i++) {
        if (state->objects[i] != 0) {
            ResourceObject_ReleaseFar(state->objects[i]);
            state->objects[i] = 0;
        }
    }
}

void Menu_UpdateFirstObjectRowPositions(void)
{
    struct PlacementState *state = (struct PlacementState *)gMenuWork;
    s32 source[2];
    s32 request[4];
    s32 index = 0;
    s32 handle;

    while (1) {
        handle = (s32)state->objects[index];
        if (handle != 0) {
            source[0] = state->scale[index];
            source[1] = state->scale[index];
            request[0] = state->x[index] << 16;
            request[1] = 0x01F40000;
            request[2] = (state->y[index] << 16) + 0x01F40000;
            request[3] = 0;
            Object_ApplyProjectedPlacementFar(handle, request, source, 0x4000);
        }
        index += 1;
        if (index > 3) {
            break;
        }
    }
}

void Menu_SpawnFourObjectsAtOrigin(struct RenderInput *origin, s32 x, s32 y)
{
    struct FourObjectMotionState *state = gMenuWork;
    s32 i;

    if (origin != 0) {
        for (i = 0; i < 4; i++) {
            struct AnimationObject *object = (struct AnimationObject *)ResourceObject_CreateFar(Menu_PartySpriteResourceIds[i]);

            if (object != 0) {
                AnimationObjects_SelectAnimationFar(object, 2);
                ((struct FieldSprite *)object)->flags = 0;
                ((struct FieldSprite *)object)->priority = 0;
            }

            state->objects[i] = object;
            state->positions_x[i] = (origin->x + x + i * 3) * 8 + 0x10;
            state->positions_y[i] = (origin->y + y) * 8 + 0x10;
        }

        Scheduler_AddOrUpdateCallback(Menu_UpdateSecondObjectRowPositions, 200 << 4);
    }
}

void Menu_ClearSecondObjectRowAndScheduleUpdate(void)
{
    struct FourObjectMotionState *state = gMenuWork;
    s32 i;

    Scheduler_RemoveCallback(Menu_UpdateSecondObjectRowPositions);
    for (i = 0; i < 4; i++) {
        if (state->objects[i] != 0) {
            ResourceObject_ReleaseFar(state->objects[i]);
            state->objects[i] = 0;
        }
    }
}

void Menu_UpdateSecondObjectRowPositions(void)
{
    struct FourObjectMotionState *root = gMenuWork;
    s32 scale[2];
    s32 position[4];
    s32 index;

    index = 0;
    do {
        void *object = root->objects[index];

        if (object != 0) {
            s32 unit = 0x10000;
            s32 bias = 0x1f40000;

            scale[0] = unit;
            scale[1] = unit;
            position[0] = root->positions_x[index] << 16;
            position[1] = bias;
            position[2] = (root->positions_y[index] << 16) + bias;
            position[3] = 0;
            Object_ApplyProjectedPlacementFar((s32)object, position, scale, 0x4000);
        }
        index++;
    } while (index <= 3);
}

//--------------------------------------------------------------------------

// draw_four_numbers_in_row.c

// セーブ：記録のジンの数を属性ごとに横に並べて書く

//--------------------------------------------------------------------------

// セーブの記録（SaveSummary）のうち、ここで読むところ
struct NumberRow
{
	u8 unknown_00[0x28];
	s8 values[4]; // 属性ごとのジンの数
};

//--------------------------------------------------------------------------
// 4 つの数を 2 けたで、24 ドットおきに y 16 に書く
// win : 書き込み先（0 なら何もしない）
//--------------------------------------------------------------------------
void UiText_DrawFourNumbersInRow(struct RenderInput *win, struct NumberRow *row)
{
	s32 i;

	if (win != 0)
	{
		for (i = 0; i < 4; i++)
			UiText_DrawNumberAtOffset(row->values[i], 2, win, i * 24, 16);
	}
}

//--------------------------------------------------------------------------
// draw_character_summary.c
// セーブ：記録の名前・レベル・クラス・コイン・プレイ時間を書く
//--------------------------------------------------------------------------

#define UiText_DrawStringAtOffset ((void (*)(void *, s32, s32, s32))UiText_DrawStringAtOffset)
#define UiText_DrawNumberAtOffset ((void (*)(s32, s32, s32, s32, s32))UiText_DrawNumberAtOffset)
#define UiText_DrawCharacterAtOffset ((void (*)(s32, s32, s32, s32))UiText_DrawCharacterAtOffset)

void RenderOutput_PrepareForRedraw(void);
void UiWindow_DrawDividerLine(s32, s32, s32, s32, s32);
void UiText_DrawStringInWindow(s32, s32, s32, s32);
void UiText_DrawNumberInWindow(s32, s32, s32, s32, s32);

// 版ごとの「Lv」と「コイン」の x（ドット）
#if defined(TBS_EDITION_ES) || defined(TLA_EDITION_ES)
#define SUMMARY_LEVEL_X 64
#define SUMMARY_COINS_X 56
#elif defined(TBS_EDITION_FR) || defined(TLA_EDITION_FR)
#define SUMMARY_LEVEL_X 59
#define SUMMARY_COINS_X 56
#elif defined(TLA_EDITION_DE) || defined(TLA_EDITION_IT)
#define SUMMARY_COINS_X 56
#else
#define SUMMARY_COINS_X 48
#endif

//--------------------------------------------------------------------------
// win : 書き込み先（0 なら何もしない）
// rec : セーブの記録（SaveSummary）
//       +16 名前、+28 レベル、+29 クラス、+32 プレイ時間、+36 コイン
//--------------------------------------------------------------------------
void StatusMenu_DrawCharacterSummary(s32 win, u8 *rec)
{
	u32 buf[4]; // プレイ時間の文字列
	s32 y;

	if (win != 0)
	{
		RenderOutput_PrepareForRedraw();
		UiWindow_DrawDividerLine(win, 0, 4, 13, 4);
		UiText_DrawStringAtOffset(rec + 16, win, 0, 0);
		y = 0;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TLA_EDITION_ES) \
	|| defined(TLA_EDITION_FR)
		UiText_DrawStringAtOffset(StatusMenu_LevelLetterString, win, SUMMARY_LEVEL_X, 0);
#else
		UiText_DrawStringInWindow((s32)StatusMenu_LevelLetterString, win, 72, 0);
#endif
		UiText_DrawNumberAtOffset(rec[28], 2, win, 80, y);
		UiText_DrawCharacterAtOffset(rec[29] + (s32)MsgClassName, win, 0, 16);
#if !EDITION_INTERNATIONAL
		y = 32;
		UiText_DrawNumberInWindow(*(s32 *)(rec + 36), 6, win, 24, y);
		UiText_DrawCharacterAtOffset((s32)MsgCoins, win, 72, 32);
		UiText_DrawCharacterAtOffset((s32)MsgStatusLabel, win, 0, 48);
		UiText_DrawStringInWindow(
			(s32)Text_FormatPlayTime(*(s32 *)(rec + 32), (u8 *)buf), win, 48, 48);
#else
		UiText_DrawCharacterAtOffset((s32)MsgStatusLabel, win, 0, 32);
		UiText_DrawStringInWindow(
			(s32)Text_FormatPlayTime(*(s32 *)(rec + 32), (u8 *)buf), win, 48, 40);
		y = 48;
		UiText_DrawNumberInWindow(*(s32 *)(rec + 36), 6, win, 0, y);
#if defined(TBS_EDITION_FR) || defined(TLA_EDITION_FR)
		// フランス語版は 1 枚以下なら単数形
		if (*(u32 *)(rec + 36) > 1)
			UiText_DrawCharacterAtOffset((s32)MsgCoins, win, SUMMARY_COINS_X, 48);
		else
			UiText_DrawCharacterAtOffset((s32)MsgPiece, win, SUMMARY_COINS_X, 48);
#else
		UiText_DrawCharacterAtOffset((s32)MsgCoins, win, SUMMARY_COINS_X, 48);
#endif
#endif
	}
}

#define RenderOutput_RedrawSavedRect ((void (*)(void *))RenderOutput_RedrawSavedRect)

#define UiWindow_FillTilemapRect ((void (*)(void*,s32,s32,s32,s32))UiWindow_FillTilemapRect)
#define UiText_DrawFourNumbersInRow ((void (*)(void*,void*))UiText_DrawFourNumbersInRow)
#define StatusMenu_DrawCharacterSummary ((void (*)(void*,void*))StatusMenu_DrawCharacterSummary)
#define ObjectPlacement_CreateGroup ((void (*)(void*,s32,s32,void*))ObjectPlacement_CreateGroup)
#define Menu_SpawnFourObjectsAtOrigin ((void (*)(void*,s32,s32))Menu_SpawnFourObjectsAtOrigin)
#define RenderResource_CreatePair ((struct RenderOutput * (*)(s32,struct UiWindow*,s32,s32))RenderResource_CreatePair)
#define Ui_ApplyTableOffsetToPair ((void (*)(void*))Ui_ApplyTableOffsetToPair)

void UiText_ShowPositionedMessageAndWait(s32 msg, s32 mode);
s32 UiWork_IsComplete(void);
void UiWork_FinalizePendingCore(void);

extern s32 gLoadedStateWord;
extern u8 gOptionMirror;
extern s16 gPostLoadCounter;

#if EDITION_INTERNATIONAL

void AudioCommand_PlayFar(s32 cue);
void PaletteGlow_Update(s32 icon, s32 icon_sub);
void UiText_DrawResource(s32 message, void *win, s32 x, s32 y);
void UiText_DrawString(void *text, void *win, s32 x, s32 y);

s32 SaveMenu_SelectSlot(s32 slot, s32 mode)
{
    struct UiWindow *list;
    struct SaveWorkspace *save;
    struct UiWindow *status;
    struct UiWindow *djinn;
    struct UiWindow *counts;
    struct SaveSummary *summary;
    struct RenderOutput *cursor;
    s32 redraw;
    struct UiRenderWork *render;
    u32 stamp;
    void *work;
    volatile u32 zero;
    s32 result;
    s32 i;
    s32 y;

    work = Runtime_AllocateHeapBlock(55, 0xa70);
    save = *(gWorkSlot + HEAP_SLOT_SAVE);
    status = NULL;
    djinn = NULL;
    counts = NULL;
    redraw = 1;
    render = *(gWorkSlot + HEAP_SLOT_WINDOW);
    stamp = Runtime_GetBuildStampTimeFar();
    if (slot < 0)
        slot = 0;
    if (mode == 1) {
        for (i = 0; i < 3 && (save->summary[slot].level == 0 || (s8)save->summary[slot].send_flag != 0); i++) {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3) {
            result = -2;
            goto done;
        }
    } else if (mode == 4) {
        for (i = 0; i < 3 && (save->summary[slot].level == 0 || (s8)save->summary[slot].flag_count == 0); i++) {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3) {
            result = -2;
            goto done;
        }
    } else if (mode == 5) {
        for (i = 0; i < 3 && (save->summary[slot].level == 0 || (s8)save->summary[slot].send_flag == 0); i++) {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3) {
            result = -2;
            goto done;
        }
    } else if (mode != 0) {
        for (i = 0; i < 3 && (save->summary[slot].level == 0); i++) {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3) {
            result = -2;
            goto done;
        }
    }

    zero = 0;
    Dma_Set(&zero, work, 0x85000000 | (0xa70 / 4), REG_DMA3);
    Scheduler_ScheduleCallbackAAfterFrames();
    list = UiWindow_Create(1, 2, 28, 7, 2);
    summary = save->summary;
    y = 0;
    for (i = 0; i < 3; i++, y += 16, summary++) {
        if (summary->level == 0) {
            UiText_DrawResource((s32)MsgSaveNoData, list, 10, y);
        } else if (summary->frames < stamp) {
            UiText_DrawResource((s32)MsgSaveContinue, list, 10, y);
        } else if (summary->play_time != *(u32 *)summary->unknown_38) {
            UiText_DrawResource((s32)MsgSaveCorrupted, list, 10, y);
        } else if (mode == 5 && (s8)summary->send_flag == 0) {
            UiText_DrawResource((s32)MsgSaveNotCleared, list, 10, y);
        } else {
            UiText_DrawString(summary->name, list, SAVE_SLOT_NAME_X, y);
            UiText_DrawResource(summary->area + (s32)MsgDebugEntryName, list, 62, y);
            render->dirty = 1;
        }
    }
    UiWindow_DrawDividerLine(list, 0, 2, 27, 2);
    UiWindow_DrawDividerLine(list, 0, 4, 27, 4);
    cursor = RenderResource_CreatePair(mode, list, 72, -24);

    while (1) {
        if (redraw != 0) {
            redraw = 0;
            if (save->summary[slot].level != 0) {
                PaletteGlow_Update(save->summary[slot].palette_glow, save->summary[slot].palette_glow_sub);
                if (status == NULL)
                    status = UiWindow_Create(1, 10, 14, 9, 2);
                StatusMenu_DrawCharacterSummary(status, &save->summary[slot]);
                WaitFrames(1);
                if (djinn == NULL)
                    djinn = UiWindow_Create(16, 10, 13, 3, 2);
                Menu_ClearFirstObjectRowAndScheduleUpdate();
                ObjectPlacement_CreateGroup(djinn, 0, 0, &save->summary[slot]);
                WaitFrames(1);
                if ((s8)save->summary[slot].djinn[0] + (s8)save->summary[slot].djinn[1] + (s8)save->summary[slot].djinn[2]
                        + (s8)save->summary[slot].djinn[3] != 0) {
                    if (counts == NULL)
                        counts = UiWindow_Create(16, 14, 13, 5, 2);
                    UiText_DrawFourNumbersInRow(counts, &save->summary[slot]);
                    Menu_ClearSecondObjectRowAndScheduleUpdate();
                    Menu_SpawnFourObjectsAtOrigin(counts, 0, 0);
                } else {
                    Menu_ClearSecondObjectRowAndScheduleUpdate();
                    UiWork_Finalize(counts, 2);
                    counts = NULL;
                }
            } else {
                PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
                Menu_ClearSecondObjectRowAndScheduleUpdate();
                Menu_ClearFirstObjectRowAndScheduleUpdate();
                UiWork_Finalize(counts, 2);
                UiWork_Finalize(djinn, 2);
                UiWork_Finalize(status, 2);
                counts = NULL;
                djinn = NULL;
                status = NULL;
            }
            RenderOutput_RedrawSavedRect(list);
            UiWindow_DrawDividerLine(list, 0, 2, 27, 2);
            UiWindow_DrawDividerLine(list, 0, 4, 27, 4);
            UiWindow_FillTilemapRect(list, 0, slot * 2, 26, 1);
        }
        Ui_ApplyTableOffsetToPair(cursor);
        WaitFrames(1);
        if (gKeysRepeat & 0x40) {
            AudioCommand_PlayFar(111);
            redraw = 1;
            do {
                slot = (slot + 2) % 3;
                if (mode == 0)
                    break;
            } while ((save->summary[slot].level == 0
                || (mode == 1 && (s8)save->summary[slot].send_flag != 0)
                || (mode == 4 && (s8)save->summary[slot].flag_count == 0)
                || (mode == 5 && (s8)save->summary[slot].send_flag == 0)));
        } else if (gKeysRepeat & 0x80) {
            AudioCommand_PlayFar(111);
            redraw = 1;
            do {
                slot = (slot + 4) % 3;
                if (mode == 0)
                    break;
            } while ((save->summary[slot].level == 0
                || (mode == 1 && (s8)save->summary[slot].send_flag != 0)
                || (mode == 4 && (s8)save->summary[slot].flag_count == 0)
                || (mode == 5 && (s8)save->summary[slot].send_flag == 0)));
        } else if (gKeyState & 2) {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        } else if (gKeyState & 1) {
            AudioCommand_PlayFar(112);
            result = slot;
            break;
        }
    }
    Menu_ClearSecondObjectRowAndScheduleUpdate();
    Menu_ClearFirstObjectRowAndScheduleUpdate();
    UiWork_Finalize(counts, 2);
    UiWork_Finalize(djinn, 2);
    UiWork_Finalize(status, 2);
    UiWork_Finalize(list, 2);
    Scheduler_ScheduleCallbackA();
    Runtime_ReleaseHeapBlock(55);
    PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
    WaitFrames(1);
done:
    return result;
}
#elif defined(TBS_EDITION_JA)
#include "shop.h"
#include "resource.h"

#define Shop_SetCursorFar ((void (*)(void*,s32,s32,s32))Shop_SetCursorFar)

extern u8 Resource_FixedBlockBTiles[];

void AudioCommand_PlayFar(s32 cue);
void PaletteGlow_Update(s32 icon, s32 icon_sub);
void UiText_DrawResource(s32 message, void *win, s32 x, s32 y);
void UiText_DrawString(void *text, void *win, s32 x, s32 y);

s32 SaveMenu_SelectSlot(s32 slot, s32 mode)
{
    struct UiWindow *list;
    struct UiWindow *status;
    struct SaveWorkspace *save;
    struct UiWindow *djinn;
    struct UiWindow *counts;
    struct SaveSummary *summary;
    struct RenderOutput *cursor;
    s32 kind;
    s32 redraw;
    struct UiRenderWork *render;
    u32 stamp;
    void *work;
    struct ShopCursor marker;
    volatile u32 zero;
    s32 result;
    s32 i;
    s32 x;
    s32 y;
    s32 tile;
    struct RenderOutput *anchor;

    work = Runtime_AllocateHeapBlock(55, 0xa70);
    save = *(gWorkSlot + HEAP_SLOT_SAVE);
    status = NULL;
    djinn = NULL;
    counts = NULL;
    redraw = 1;
    render = *(gWorkSlot + HEAP_SLOT_WINDOW);
    stamp = Runtime_GetBuildStampTimeFar();
    if (slot < 0)
        slot = 0;
    if (mode != 0) {
        for (i = 0; i < 3 && (save->summary[slot].level == 0); i++) {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3) {
            result = -2;
            goto done;
        }
    }
    if (mode == 1) {
        for (i = 0; i < 3 && ((s8)save->summary[slot].send_flag != 0); i++) {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3) {
            result = -2;
            goto done;
        }
    }
    if (mode == 4) {
        for (i = 0; i < 3 && ((s8)save->summary[slot].flag_count == 0); i++) {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3) {
            result = -2;
            goto done;
        }
    }
    if (mode == 5) {
        for (i = 0; i < 3 && ((s8)save->summary[slot].send_flag == 0); i++) {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3) {
            result = -2;
            goto done;
        }
    }

    zero = 0;
    Dma_Set(&zero, work, 0x85000000 | (0xa70 / 4), REG_DMA3);
    list = UiWindow_Create(1, 2, 28, 7, 2);

    for (i = 0, summary = save->summary; i < 3; i++, summary++) {
        if (save->summary[i].level == 0) {
            UiText_DrawResource((s32)MsgSaveNoData, list, 10, i * 16);
        } else if (save->summary[i].frames < stamp) {
            UiText_DrawResource((s32)MsgSaveContinue, list, 10, i * 16);
        } else if (save->summary[i].play_time != *(u32 *)save->summary[i].unknown_38) {
            UiText_DrawResource((s32)MsgSaveCorrupted, list, 10, i * 16);
        } else if (mode == 5 && (s8)save->summary[i].send_flag == 0) {
            UiText_DrawResource((s32)MsgSaveNotCleared, list, 10, i * 16);
        } else {
            y = i * 16;
            UiText_DrawString(summary->name, list, SAVE_SLOT_NAME_X, y);
            UiText_DrawResource(save->summary[i].area + (s32)MsgDebugEntryName, list, 72, y);
            render->dirty = 1;
        }
    }
    UiWindow_DrawDividerLine(list, 0, 2, 27, 2);
    UiWindow_DrawDividerLine(list, 0, 4, 27, 4);
    tile = Resource_FindFreeEntry();
    if (tile < 96) {
        VramBlock_LoadCached(tile, 128, Resource_FixedBlockBTiles);
        x = -12;
        y = slot * 16 + 4;
        anchor = RenderOutput_Create(tile, 0x40000000, (struct RenderInput *)list, x, y);
        anchor->active = 1;
        marker.anchor = anchor;
    }
    kind = 1;
    cursor = RenderResource_CreatePair(mode, list, 72, -24);

    while (1) {
        if (redraw != 0) {
            redraw = 0;
            if (save->summary[slot].level != 0) {
                PaletteGlow_Update(save->summary[slot].palette_glow, save->summary[slot].palette_glow_sub);
                if (status == NULL)
                    status = UiWindow_Create(1, 10, 14, 9, 2);
                StatusMenu_DrawCharacterSummary(status, &save->summary[slot]);
                WaitFrames(1);
                if (djinn == NULL)
                    djinn = UiWindow_Create(16, 10, 13, 3, 2);
                Menu_ClearFirstObjectRowAndScheduleUpdate();
                ObjectPlacement_CreateGroup(djinn, 0, 0, &save->summary[slot]);
                WaitFrames(1);
                if ((s8)save->summary[slot].djinn[0] + (s8)save->summary[slot].djinn[1] + (s8)save->summary[slot].djinn[2]
                        + (s8)save->summary[slot].djinn[3] != 0) {
                    if (counts == NULL)
                        counts = UiWindow_Create(16, 14, 13, 5, 2);
                    UiText_DrawFourNumbersInRow(counts, &save->summary[slot]);
                    Menu_ClearSecondObjectRowAndScheduleUpdate();
                    Menu_SpawnFourObjectsAtOrigin(counts, 0, 0);
                } else {
                    Menu_ClearSecondObjectRowAndScheduleUpdate();
                    UiWork_Finalize(counts, 2);
                    counts = NULL;
                }
            } else {
                PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
                Menu_ClearSecondObjectRowAndScheduleUpdate();
                Menu_ClearFirstObjectRowAndScheduleUpdate();
                UiWork_Finalize(counts, 2);
                UiWork_Finalize(djinn, 2);
                UiWork_Finalize(status, 2);
                counts = NULL;
                djinn = NULL;
                status = NULL;
            }
            x = list->x * 8 - 4;
            y = (list->y << 3) + slot * 16 + 12;
            Shop_SetCursorFar(&marker, x, y, kind);
            kind = 3;
        }
        ShopCursor_AdvanceFar(&marker);
        Ui_ApplyTableOffsetToPair(cursor);
        WaitFrames(1);
        if (gKeysRepeat & 0x40) {
            AudioCommand_PlayFar(111);
            redraw = 1;
            do {
                slot = (slot + 2) % 3;
                if (mode == 0)
                    break;
            } while ((save->summary[slot].level == 0
                || (mode == 1 && (s8)save->summary[slot].send_flag != 0)
                || (mode == 4 && (s8)save->summary[slot].flag_count == 0)
                || (mode == 5 && (s8)save->summary[slot].send_flag == 0)));
        } else if (gKeysRepeat & 0x80) {
            AudioCommand_PlayFar(111);
            redraw = 1;
            do {
                slot = (slot + 4) % 3;
                if (mode == 0)
                    break;
            } while ((save->summary[slot].level == 0
                || (mode == 1 && (s8)save->summary[slot].send_flag != 0)
                || (mode == 4 && (s8)save->summary[slot].flag_count == 0)
                || (mode == 5 && (s8)save->summary[slot].send_flag == 0)));
        } else if (gKeyState & 2) {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        } else if (gKeyState & 1) {
            AudioCommand_PlayFar(112);
            result = slot;
            break;
        }
    }
    Menu_ClearSecondObjectRowAndScheduleUpdate();
    Menu_ClearFirstObjectRowAndScheduleUpdate();
    UiWork_Finalize(counts, 2);
    UiWork_Finalize(djinn, 2);
    UiWork_Finalize(status, 2);
    UiWork_Finalize(list, 2);
    Runtime_ReleaseHeapBlock(55);
    PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
    WaitFrames(1);
done:
    return result;
}
#endif

s32 Save_WriteSelectedSlot(void)
{
    s32 result;
    s32 slot;
    s32 flag;
    struct SaveWorkspace *work;

    result = 0;
    flag = SaveState_InitializeWorkspace();
    if (flag != 0) {
        UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
        result = -9;
    } else {
        SaveState_LoadSummaryRecords();
        work = gSaveWorkspace;
        slot = SaveMenu_SelectSlot(gSaveSlot, 0);
        if (slot == -1) {
            result = slot;
        } else {
            if (work->summary[slot].level != 0) {
                UiText_ShowPositionedMessageAndWait((s32)MsgOverwriteConfirm, 13);
                while (UiWork_IsComplete() == 0) {
                    WaitFrames(1);
                }
                if (Menu_RunConfirmSelection(1, 0, 0, 1) != 0) {
                    UiWork_FinalizePendingCore();
                    goto skip;
                }
                UiWork_FinalizePendingCore();
            }
            gSaveSlot = slot;
            AudioCommand_PlayFar(85);
            UiText_ShowPositionedMessageAndWait((s32)MsgSaving, 13);
            while (UiWork_IsComplete() == 0) {
                WaitFrames(1);
            }
            SaveState_BuildSummaryHeader();
            SaveState_CaptureObjectTableFar();
            flag = SaveState_WriteRecord(slot, gSaveBuffer);
            flag |= SaveState_WriteRecord(slot + 3, gSaveBuffer + 0x1000);
            UiWork_FinalizePendingCore();
            if (flag != 0) {
                UiText_ShowPositionedMessageAndWait((s32)MsgSaveFailed, 1);
                result = -3;
            } else {
                UiText_ShowPositionedMessageAndWait((s32)MsgGameSaved, 9);
            }
        }
    }
skip:
    SaveState_ReleaseWorkspace();
    return result;
}

s32 SaveState_LoadRecordIntoWork(s32 arg)
{
    s32 ret = 0;
    s32 err = SaveState_InitializeWorkspace();

    if (err != 0) {
        UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
        ret = -9;
    } else {
        s32 value;

        SaveState_LoadSummaryRecords();
        value = SaveMenu_SelectSlot(gSaveSlot, arg);
        if (value == -1) {
            ret = value;
        } else {
            void *base = &gSaveBuffer;

            err = SaveState_ReadRecordPayload(value, base);
            base = (char *)base + 0x1000;
            err |= SaveState_ReadRecordPayload(value + 3, base);
            if (err != 0) {
                UiText_ShowPositionedMessageAndWait((s32)MsgLoadFailed, 1);
                ret = -2;
            } else {
                gLoadedStateWord = gGameState.play_time;
                gOptionMirror = gGameState.saved_options;
                gPostLoadCounter = 0;
                gSaveSlot = value;
            }
        }
    }
    SaveState_ReleaseWorkspace();
    return ret;
}

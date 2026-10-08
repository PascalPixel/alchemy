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
#include "disptran.h"
#include "types.h"
#include "dma.h"
#include "callback_scheduler.h"
#include "scene.h"
#include "io_reg.h"
#include "global_cells.h"
#include "game_state.h"
#include "frame.h"
#include "iwram_call.h"
#include "map_scroll.h"
#include "object_runtime.h"
#include "object_lookup.h"
#include "system.h"


struct TransitionSpan {
    u16 cnt;
    u16 val;
    u8 x0;
    u8 x1;
    u8 y0;
    u8 y1;
};

struct TransitionShape {
    u16 first;
    u16 second;
    struct TransitionSpan spans[3];
};

extern struct TransitionShape DisplayTransition_Shapes[];


void DisplayTransition_UpdateScanlineTable(void)
{
    struct DisplayTransitionState *state;
    struct TransitionSpan *span;
    struct TransitionShape *shape;
    struct MapScrollWork *map;
    u16 *dst;
    s32 *view7;
    s32 *view8;
    s32 *view9;
    s32 *view77;
    struct ObjectRuntime *obj;
    u32 i;
    s32 k;
    s32 cnt;
    s32 v;
    s32 rr;
    s32 d;
    u32 h;
    s32 lo;
    s32 hi;
    s32 cx;
    s32 cy;

    /* FAKEMATCH: pins ax to r5, as the native allocates it. */
    register s32 ax asm("r5");
    s32 ay;
    s32 bx;
    s32 by;
    u32 w;
    s32 e;

    map = gMapWork[0];
    state = gMapWork[23];

    if (state->duration != 0) {
        if (state->step >= state->duration) {
            state->duration = 0;
            if (state->next_mode == 0) {
                if (state->end == 64) {
                    REG_DISPCNT &= 0x81ff;
                } else {
                    REG_DISPCNT &= 0x9fff;
                }
                Scheduler_RemoveCallback((u32)BattleFx_StartWindowHBlankDma);
                Scheduler_RemoveCallback((u32)DisplayTransition_UpdateScanlineTable);
                {
                    volatile u16 *dma0 = REG_DMA0;

                    dma0[5] &= 0xc5ff;
                    dma0[5] &= 0x7fff;
                    dma0[5];
                }
                return;
            }
            state->mode = state->next_mode;
        } else {
            state->value = state->start
                + Iwram_SignedDivide((state->end - state->start) * ++state->step,
                                     state->duration);
        }
    }

    dst = state->pages[1 ^ state->page];
    UiWindow_FillFromSceneFar(dst + 2);

    switch (state->mode) {
    case 0:
        *dst++ = 0x7f7f;
        *dst++ = 1;
        v = state->value;
        if (v & 32) {
            v = 32 - (v & 31);
        } else {
            v = v & 31;
        }
        v = DisplayTransition_NoiseWidths[v];
        for (i = 0; i < 160; i++) {
            w = (Random16() * (u32)(241 - v)) >> 16;
            dst[0] = (u16)((w << 8) | (w + v));
            dst += 2;
        }
        break;

    case 1:
        v = state->value;
        shape = &DisplayTransition_Shapes[v & 31];
        if (v & 32) {
            *dst++ = shape->first;
            *dst++ = shape->second;
        } else {
            *dst++ = shape->second;
            *dst++ = shape->first;
        }
        for (i = 0; i < 3; i++) {
            span = &shape->spans[i];
            cnt = span->cnt;
            hi = span->val;
            if (cnt != 0) {
                if (hi != 0) {
                    for (k = 0; k < cnt; k++) {
                        dst[0] = (u16)hi;
                        dst += 2;
                    }
                } else {
                    rr = span->x0;
                    lo = span->y0;
                    d = span->x1;
                    hi = span->y1;
                    for (k = 0; k < cnt; k++) {
                        dst[0] = (u16)(((rr + (d - rr) * k / cnt) << 8)
                                       + (lo + (hi - lo) * k / cnt));
                        dst += 2;
                    }
                }
            }
        }
        break;

    case 2:
        v = state->value - 1;
        if (v & 32) {
            *dst++ = 1;
            *dst++ = 0x7f7f;
        } else {
            *dst++ = 0x7f7f;
            *dst++ = 1;
        }
        v &= 31;
        for (i = 0; i < 160; i++) {
            h = (v << 4) + ((16 * Random16()) >> 16);
            if (h > 255) {
                h = 255;
            }
            dst[0] = (u16)h;
            dst += 2;
        }
        break;

    case 3:
        v = state->value;
        *dst++ = 0x7f7f;
        *dst++ = 1;
        if ((u32)v > 32) {
            *dst++ = 1;
            *dst++ = 0x7f7f;
            v = 64 - v;
        }
        v = v * 5;
        rr = (v * v) << 16;
        for (i = 0; i < 160; i++) {
            v = i - 80;
            h = Iwram_Sqrt(rr - ((v * v) << 16)) >> 8;
            lo = 120 - h;
            hi = h + 120;
            if (lo < 0) {
                lo = 0;
            }
            if (hi < 0) {
                hi = 0;
            }
            if (lo > 240) {
                lo = 240;
            }
            if (hi > 240) {
                hi = 240;
            }
            dst[0] = (u16)((lo << 8) + hi);
            dst += 2;
        }
        break;

    case 4:
        v = state->value;
        if (v & 32) {
            *dst++ = 1;
            *dst++ = 0x7f7f;
        } else {
            *dst++ = 0x7f7f;
            *dst++ = 1;
        }
        v = (u32)(240 * (v & 31)) >> 5;
        for (i = 0; i < 160; i += 2) {
            e = ((240 - v) << 8) + 240;
            dst[0] = (u16)v;
            dst[2] = (u16)e;
            dst += 4;
        }
        break;

    case 7:
        view7 = &map->view_x;
        ax = view7[0] & 0xffff0000;
        ay = view7[1] & 0xffff0000;
        obj = ObjectTable_Get(gGameState.selected_actor);
        cx = (obj->x - ax) / 0x10000;
        cy = ((obj->z - obj->y) - ay) / 0x10000 - 16;
        /* FAKEMATCH: needed to match: the native reads the frame counter here, where mode 8 forms its index, and discards it. */
        gFrameCount;
        *dst++ = state->active;
        *dst++ = state->mask;
        v = state->value;
        if ((v & 32) == 0) {
            v = 32 - (v & 31);
        } else {
            v = v & 31;
        }
        if (gFrameCount & 1) {
            v = 0;
        }
        v = v * 5;
        rr = (v * v) << 16;
        for (i = 0; i < 160; i++) {
            v = i - cy;
            h = Iwram_Sqrt(rr - ((v * v * 3) << 15)) >> 8;
            lo = cx - h;
            hi = cx + h;
            if (lo < 0) {
                lo = 0;
            }
            if (hi < 0) {
                hi = 0;
            }
            if (lo > 240) {
                lo = 240;
            }
            if (hi > 240) {
                hi = 240;
            }
            dst[0] = (u16)((lo << 8) + hi);
            dst += 2;
        }
        break;

    case 8:
        view8 = &map->view_x;
        bx = view8[0] & 0xffff0000;
        by = view8[1] & 0xffff0000;
        cx = (state->centre_x - bx) / 0x10000;
        cy = (state->centre_y - by) / 0x10000 - 16;
        k = -cy * 2 + gFrameCount;
        *dst++ = state->mask;
        *dst++ = state->active;
        v = state->value;
        if (v & 32) {
            v = 32 - (v & 31);
        } else {
            v = v & 31;
        }
        v = v * 5;
        rr = (v * v) << 16;
        for (i = 0; i < 160; i++) {
            v = i - cy;
            h = Iwram_Sqrt(rr - ((v * v * 3) << 15)) >> 8;
            lo = cx - h;
            hi = cx + h;
            if (lo < hi) {
                d = DisplayTransition_EdgeOffsets[k & 31];
                lo -= d;
                hi += d;
                if (lo >= hi) {
                    lo = 240;
                    hi = 240;
                }
            }
            if (lo < 0) {
                lo = 0;
            }
            if (hi < 0) {
                hi = 0;
            }
            if (lo > 240) {
                lo = 240;
            }
            if (hi > 240) {
                hi = 240;
            }
            dst[0] = (u16)((lo << 8) + hi);
            dst += 2;
            k += 2;
        }
        break;

    case 9:
        view9 = &map->view_x;
        ax = view9[0] & 0xffff0000;
        ay = view9[1] & 0xffff0000;
        obj = ObjectTable_Get(gGameState.selected_actor);
        cx = (obj->x - ax) / 0x10000;
        cy = ((obj->z - obj->y) - ay) / 0x10000 - 16;
        /* FAKEMATCH: needed to match: the native reads the frame counter here, where mode 8 forms its index, and discards it. */
        gFrameCount;
        *dst++ = state->mask;
        *dst++ = state->active;
        v = state->value;
        if (v & 32) {
            v = 32 - (v & 31);
        } else {
            v = v & 31;
        }
        v = v * 5;
        rr = (v * v) << 16;
        for (i = 0; i < 160; i++) {
            v = i - cy;
            h = Iwram_Sqrt(rr - ((v * v * 3) << 15)) >> 8;
            lo = cx - h;
            hi = cx + h;
            if (lo < 0) {
                lo = 0;
            }
            if (hi < 0) {
                hi = 0;
            }
            if (lo > 240) {
                lo = 240;
            }
            if (hi > 240) {
                hi = 240;
            }
            dst[0] = (u16)((lo << 8) + hi);
            dst += 2;
        }
        break;

    case 77:
        view77 = &map->view_x;
        ax = view77[0] & 0xffff0000;
        ay = view77[1] & 0xffff0000;
        obj = ObjectTable_Get(gGameState.selected_actor);
        cx = (obj->x - ax) / 0x10000;
        cy = ((obj->z - obj->y) - ay) / 0x10000 - 8;
        /* FAKEMATCH: needed to match: the native reads the frame counter here, where mode 8 forms its index, and discards it. */
        gFrameCount;
        *dst++ = state->mask;
        *dst++ = state->active;
        v = state->value;
        if (v & 32) {
            v = 32 - (v & 31);
        } else {
            v = v & 31;
        }
        v = v * 5;
        rr = (v * v) << 16;
        for (i = 0; i < 160; i++) {
            v = i - cy;
            h = Iwram_Sqrt(rr - ((v * v * 3) << 14)) >> 8;
            lo = cx - h;
            hi = cx + h;
            if (lo < 0) {
                lo = 0;
            }
            if (hi < 0) {
                hi = 0;
            }
            if (lo > 240) {
                lo = 240;
            }
            if (hi > 240) {
                hi = 240;
            }
            dst[0] = (u16)((lo << 8) + hi);
            dst += 2;
        }
        break;
    }

    state->page ^= 1;
}

void DisplayTransition_InitializeBattleEffectState(s32 mode)
{
    struct DisplayTransitionState *state = Runtime_AllocateBlock(31, sizeof(*state));
    volatile u32 zero;

    zero = 0;
    Dma_Set((const void *)&zero, state, 0x85000000 | (sizeof(*state) / 4), REG_DMA3);
    state->mode = mode;
    state->value = 0;
    state->mask = 0x3f3f;
    state->active = 1;
    Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateScanlineTable, 0xc80);
    Scheduler_AddOrUpdateCallback(BattleFx_StartWindowHBlankDma, 0x480);
}

void BattleFx_EnableTwoCallbacks(void)
{
    Scheduler_EnableCallbacks(&DisplayTransition_UpdateScanlineTable);
    Scheduler_EnableCallbacks(&BattleFx_StartWindowHBlankDma);
}

struct DisplayTransitionState *DisplayTransition_AllocateAndClearState(void)
{
    volatile u32 clear_value;
    struct DisplayTransitionState *state;

    state = Runtime_AllocateBlock(31, sizeof(*state));
    clear_value = 0;
    Dma_Set(&clear_value, state, 0x85000000 | (sizeof(*state) / 4), REG_DMA3);
    return state;
}

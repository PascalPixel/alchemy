#include "EDITION.H"
#include "TYPES.H"
#include "SCENE.H"
#include "TBS_EDITION.H"
#include "GLOBAL_CELLS.H"
#include "TEXT_RENDER_RUNTIME.H"
#include "RENDER_INPUT.H"
#include "SOUND_IDS.H"
#include "DMA.H"
#include "RESOURCE.H"
#include "RESOURCE_IDS.H"

extern u8 gKeyState[];
extern u8 gKeysRepeat[];
s32 GameFlag_TestFar(s32);
void Shop_SetCursorFar(void *, s32, s32, s32);
void Menu_InitializeSelectedWorkspace(void);
void RenderResource_LoadFrame(s32, s32, s32);
void Ui_ApplyTableOffsetToPair(void *);
void UiWork_Finalize(void *, s32);
void Runtime_ScheduleCallbackAndReleaseBlock20A(void);

/* menu/run_workspace_selection_loop.c */
/* Owner-local field access until this runtime workspace layout is recovered
 * elsewhere; other menu owners reach the same 0x03001EA0 pointer. */

extern void *gSelectionWork;
extern u8 gDebugMode;
extern s8 Menu_WorkspaceIconFrames[];

void *Menu_CreateWorkspaceWindows(void);
void UiIcon_PrepareObjectFar(void *);

void *RenderResource_CreatePair(s32, void *, s32, s32);

s32 Menu_RunWorkspaceSelectionLoop(void)
{
    void *sp10;
    s32 spC;
    s32 sp8;
    s32 sp4;
    s32 i;
    s32 j;
    s32 var_fp_21;
    s32 temp_r0_22;
    s32 var_r2_101;
    s32 var_r4_35;
    s32 var_r5_144;
    s32 var_r8_51;
    s32 temp_e;
    s32 temp_c;
    s32 temp_e2;
    s8 *tbl;
    void *temp_r5_90;
    void *temp_sl_29;

    spC = 1;
    var_fp_21 = 3;
    temp_r0_22 = GameFlag_TestFar(0x17E);
    sp4 = 0;
    Menu_InitializeSelectedWorkspace();
    temp_sl_29 = gSelectionWork;
    sp10 = Menu_CreateWorkspaceWindows();
#if EDITION_INTERNATIONAL
    var_r4_35 = -0x18;
    if (gDebugMode != 0) {
        var_r4_35 = -0x10;
    }
    sp8 = (s32)RenderResource_CreatePair(6, sp10, 0x28, var_r4_35);
#else
    /* The Japanese pair sits at one height in the debug build too. */
    sp8 = (s32)RenderResource_CreatePair(6, sp10, 0x28, -0x18);
#endif
    WaitFrames(1);
    var_r8_51 = FIELD_AT_OFFSET(temp_sl_29, u16 *, 0x574);
    if (temp_r0_22 != 0) {
        var_fp_21 = 1;
        sp4 = 2;
    }
    if (gDebugMode != 0) {
        var_fp_21 += 3;
    }
loop_6:
    if (spC != 0) {
        spC = 0;
        var_r8_51 = (var_r8_51 + var_fp_21) % var_fp_21;
        FIELD_AT_OFFSET(temp_sl_29, u16 *, 0x574) = var_r8_51;
        i = 0;
        if (i < var_fp_21) {
            j = sp4;
            tbl = Menu_WorkspaceIconFrames;
            do {
                temp_r5_90 = ((void **)((u8 *)temp_sl_29 + 0x610))[i];
                FIELD_AT_OFFSET(temp_r5_90, u8 *, 0xF) = 0xFB;
                UiIcon_PrepareObjectFar(temp_r5_90);
                temp_e = FIELD_AT_OFFSET(temp_r5_90, u8 *, 0xE);
                var_r2_101 = 0;
                if (i != FIELD_AT_OFFSET(temp_sl_29, u16 *, 0x574)) {
                    var_r2_101 = 1;
                }
                RenderResource_LoadFrame(*(s8 *)(j + (s32)tbl), temp_e, var_r2_101);
                i++;
                j++;
            } while (i < var_fp_21);
        }
        temp_e2 = FIELD_AT_OFFSET(sp10, u16 *, 0xE);
        temp_c = FIELD_AT_OFFSET(sp10, u16 *, 0xC) * 8;
        var_r4_35 = (((var_r8_51 * 3) + temp_e2) * 8) + 0x10;
        Shop_SetCursorFar(
            (u8 *)temp_sl_29 + 0x5A4,
            temp_c,
            var_r4_35,
            3);
    }
    Ui_ApplyTableOffsetToPair((void *)sp8);
    WaitFrames(1);
    if (*(volatile s32 *)gKeyState & 1) {
        var_r5_144 = var_r8_51;
        Audio_PlayCue(SOUND_MENU_CONFIRM);
    } else if (*(volatile s32 *)gKeyState & 0xA) {
        var_r5_144 = -1;
        Audio_PlayCue(SOUND_MENU_CANCEL);
    } else {
        if (*(volatile s32 *)gKeysRepeat & 0x40) {
            Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
            var_r8_51 -= 1;
            spC = 1;
        } else if (*(volatile s32 *)gKeysRepeat & 0x80) {
            Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
            var_r8_51 += 1;
            spC = 1;
        }
        goto loop_6;
    }
    UiWork_Finalize(sp10, 2);
    Runtime_ScheduleCallbackAndReleaseBlock20A();
    WaitFrames(1);
    if (var_r5_144 >= 0) {
        var_r5_144 += sp4;
    }
    return var_r5_144;
}

/* The tile slots of the window work: one in-use byte per tile and the slot
   the next search starts from. */
struct UiTileSlotWork {
    u8 unknown_000[RENDER_TILE_ATTR_OFS];
    u8 used[0x100];
    u16 next;
};

extern struct UiTileSlotWork *gWindowWork;

/* Draws one pattern of the window tile sheet over the tile a map entry shows.
   The tile is unpacked to one colour per byte, the pattern's pixels replace
   those whose remapped colour is not transparent, and the result is packed
   again. A shared tile (slot below 128) is first given a free slot of its
   own, which both map entries then name. */
void UiWindow_OverlayTilePattern(u16 *entry, u16 *mirror, s32 pattern, u8 *remap)
{
    struct UiTileSlotWork *work = gWindowWork;
    u8 pixels[128];
    u8 *sheet = Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
    u32 slot = *(u8 *)entry;
    u8 *src;
    u8 *dst;
    u32 n;
    u32 i;

    dst = pixels;
    src = (u8 *)(0x06000000 + 32 * slot);
    for (i = 0; i < 32; i++) {
        /* FAKEMATCH: the second byte's address as its own value keeps the
           two stores on separate address registers. */
        u8 *high = dst + 1;

        n = *src++;
        dst[0] = n & 15;
        *high = n >> 4;
        dst += 2;
    }

    dst = pixels;
    src = sheet + pattern * 32;
    for (i = 0; i < 32; i++) {
        u32 value = *src++;
        u32 color = remap[value & 15];

        if (color != 0)
            *dst = color;
        dst++;
        color = remap[value >> 4];
        if (color != 0)
            *dst = color;
        dst++;
    }

    {
        u8 *p;

        dst = pixels;
        /* FAKEMATCH: counter before pointer, in one statement, orders the
           two initial moves as the listing has them. */
        i = 0, p = dst;
        while (i < 32) {
            u32 value = p[0] | p[1] << 4;

            p += 2;
            *dst++ = value;
            i++;
        }
    }

    if ((s8)slot >= 0) {
        for (n = 0; n < 128; n++) {
            u32 value = work->next;
            u32 following = (value + 1) % 128;

            slot = (u8)value;
            work->next = following;
            if (work->used[slot] == 0)
                break;
        }
        work->used[slot] = 1;
        slot |= 0x80;
        *entry = slot | 0xf000;
        *mirror = slot | 0xf000;
    }
    Dma_Set(pixels, (void *)(0x06000000 + slot * 32), 0x84000008, (volatile u32 *)0x040000d4);
}

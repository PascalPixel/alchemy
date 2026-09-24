#include "TYPES.H"
#include "RENDER_INPUT.H"
#include "WORKSPACE_OPTIONS.H"

extern volatile u32 Data_03001c94;
extern volatile u32 Data_03001b04;
extern u8 Data_03001ca0;
extern u8 Data_03001d08;
extern u8 Value_00000c0a;
extern u8 Value_00000c10;
extern u8 Value_00000c13;
extern u8 Value_00000c15;
extern u8 Data_02000240[];
extern s8 Data_080367c9[];
extern s8 Data_080367cc[];
extern s8 Data_080367ce[];
extern u8 Data_080367d0[];
extern u8 Data_080367d6[];

void OptionMenu_InitializeWork(void);
struct RenderInput *UiWindow_Create(s32, s32, s32, s32, s32);
void *RenderOutput_CreatePair(s32, struct RenderInput *, s32, s32);
void WaitFrames(s32);
s32 Math_Mod(s32, s32);
s32 Math_Div(s32, s32);
void UiIcon_PrepareObjectFar(struct RenderOutput *);
void RenderOutput_LoadFrame(s32 frame, s32 index, s32 dim);
void Shop_SetCursorFar(void *cursor, s32 x, s32 y, s32 mode);
void UiWindow_ClearInteriorTiles(struct RenderInput *, s32, s32, s32, s32);
void UiText_DrawCharacterAtOffset(s32 message, struct RenderInput *win, s32 x, s32 y);
void PaletteGlow_Update(s32, s32);
void RenderOutput_ClearList(struct RenderInput *);
void UiText_DrawResource(s32, struct RenderInput *, s32, s32);
void Ui_ApplyTableOffsetToPair(void *);
void Audio_PlayCue(s32);
void UiWork_Finalize(struct RenderInput *, s32);
void Runtime_ScheduleCallbackAndReleaseBlock20B(void);

/* Runs the workspace options page until A/START (0) or B (-1).  L and R
   step through the presets, up and down move between the five settings and
   left and right change the selected one; every change redraws the page.
   On A the settings are copied into the game state. */
s32 Menu_RunWorkspaceOptions(void)
{
    struct WorkspaceWork *work;
    struct RenderInput *icon;
    struct RenderInput *win;
    void *pair;
    s8 *pA;
    s8 *pB;
    s32 redraw;
    s32 page;
    s32 i;
    s32 dim;
    s32 index;
    s32 x;
    s32 y;
    s32 result;
    u16 *preset;
    s32 n;

    redraw = 1;
    page = 0;
    OptionMenu_InitializeWork();
    work = Data_03001ea0;
    win = UiWindow_Create(1, 2, 28, 3, 2);
    icon = Menu_OpenWorkspaceOptions();
    pair = RenderOutput_CreatePair(7, icon, 64, -48);
    WaitFrames(1);
    pA = &work->option[0];
    pB = &work->option[1];

    for (;;) {
        if (redraw != 0) {
            redraw = 0;
            page = Math_Mod(page + 5, 5);
            work->option[page] = Math_Mod(work->option[page] + work->option_count[page], work->option_count[page]);
            work->page = page;
            if (Data_03001ca0 != 0)
                work->option[4] = 0;

            for (i = 0; i <= 2; i++) {
                work->frame[0][i]->sentinel = 0xfb;
                UiIcon_PrepareObjectFar(work->frame[0][i]);
                index = (u8)work->frame[0][i]->index;
                dim = 0;
                if (i != work->option[2])
                    dim = 1;
                RenderOutput_LoadFrame(Data_080367c9[i], index, dim);
            }
            for (i = 0; i <= 1; i++) {
                work->frame[1][i]->sentinel = 0xfb;
                UiIcon_PrepareObjectFar(work->frame[1][i]);
                index = (u8)work->frame[1][i]->index;
                dim = 0;
                if (i != work->option[3])
                    dim = 1;
                RenderOutput_LoadFrame(Data_080367cc[i], index, dim);
            }
            for (i = 0; i <= 1; i++) {
                work->frame[2][i]->sentinel = 0xfb;
                UiIcon_PrepareObjectFar(work->frame[2][i]);
                index = (u8)work->frame[2][i]->index;
                dim = 0;
                if (i != work->option[4])
                    dim = 1;
                RenderOutput_LoadFrame(Data_080367ce[i], index, dim);
            }

            x = icon->x * 8 + 140;
            x += Math_Div(*pA * 60, work->option_count[0]);
            y = icon->y * 8 + 4;
            Shop_SetCursorFar(&work->marker[0], x, y, 1);
            x = icon->x * 8 + 140;
            x += Math_Div(*pB * 60, work->option_count[1]);
            y = icon->y * 8 + 20;
            Shop_SetCursorFar(&work->marker[1], x, y, 1);

            x = work->option[2] + (s32)&Value_00000c0a;
            UiWindow_ClearInteriorTiles(icon, 160, 40, 200, 48);
            UiText_DrawCharacterAtOffset(x, icon, 160, 40);
            x = work->option[3] + (s32)&Value_00000c10;
            UiWindow_ClearInteriorTiles(icon, 160, 64, 184, 72);
            UiText_DrawCharacterAtOffset(x, icon, 160, 64);
            x = work->option[4] + (s32)&Value_00000c13;
            UiWindow_ClearInteriorTiles(icon, 160, 88, 184, 96);
            UiText_DrawCharacterAtOffset(x, icon, 160, 88);
            PaletteGlow_Update(*pA, *pB);

            x = icon->x * 8;
            y = (page * 3 + icon->y) * 8 + 4;
            if (page == 0)
                y += 8;
            Shop_SetCursorFar(&work->cursor, x, y, 3);
            RenderOutput_ClearList(win);
            UiText_DrawResource(page + (s32)&Value_00000c15, win, 0, 0);
        }
        Ui_ApplyTableOffsetToPair(pair);
        WaitFrames(1);

        if (Data_03001c94 & 4) {
            Audio_PlayCue(112);
            preset = &work->preset;
            n = *preset + 1;
            *preset = n;
            /* FAKEMATCH: the preset wraps to the zero held in dim. */
            dim = 0;
            redraw = 1;
            if ((u32)(n << 16) > 0x50000)
                work->preset = dim;
            *pA = Data_080367d0[*preset];
            *pB = Data_080367d6[*preset];
            continue;
        }
        if (Data_03001c94 & 9) {
            result = 0;
            Audio_PlayCue(112);
            break;
        }
        if (Data_03001c94 & 2) {
            result = -1;
            Audio_PlayCue(113);
            break;
        }
        if (Data_03001b04 & 64) {
            Audio_PlayCue(111);
            page--;
            redraw = 1;
        } else if (Data_03001b04 & 128) {
            Audio_PlayCue(111);
            page++;
            redraw = 1;
        } else {
            if (Data_03001b04 & 32) {
                Audio_PlayCue(111);
                work->option[page]--;
                redraw = 1;
            }
            if (Data_03001b04 & 16) {
                Audio_PlayCue(111);
                work->option[page]++;
                redraw = 1;
            }
        }
    }

    UiWork_Finalize(win, 2);
    UiWork_Finalize(icon, 2);
    if (result == 0) {
        Data_02000240[0x205] = work->option[0];
        Data_02000240[0x206] = work->option[1];
        Data_02000240[0x20c] = work->option[2];
        Data_02000240[0x20a] = work->option[3];
        Data_02000240[0x22a] = work->option[4];
        Data_03001d08 = Data_02000240[0x22a];
    } else {
        PaletteGlow_Update(Data_02000240[0x205], Data_02000240[0x206]);
    }
    Runtime_ScheduleCallbackAndReleaseBlock20B();
    WaitFrames(1);
    return result;
}

#include "types.h"

struct AssetSelectionGlobals {
    void *display_state;
    u8 reserved_004[0x20];
    u8 *process_state;
    u8 reserved_028[0x2c];
    u8 *selection_state;
};

struct AssetSelectionScreen {
    u8 reserved_000[0x24];
    s32 resource_handle;
    u8 reserved_028[0xe4];
    s32 window;
    u8 reserved_110[0x64];
    u16 selection_style;
    u8 reserved_176[0x92];
    u8 session[0x11];
    u8 session_mode;
};

#define ASSET_SELECTION_GLOBALS ((struct AssetSelectionGlobals *)0x03001e68)

void *Func_08004970(s32 size);
struct AssetSelectionScreen *Func_080048b0(s32 asset_id, s32 size);
void Func_08015408(s32, s32, s32, s32);
void WaitFrames(s32 frames);
void Func_080a1090(s32);
s32 Func_08077158(void *session);
void Func_080a3354(s32, s32, s32, s32);
void Func_080a5534(void);
void Func_080a2144(s32);
void Func_08015418(void *address);
s32 UiWindow_CreateFar(s32, s32, s32, s32, s32);
void Func_080a1070(void);
typedef void (*CopyFn)(const void *source, void *destination, s32 size);
void Func_080072f0(const void *, s32, u32, void *);
void Func_080153e0(s32);
void Func_080a2474(void);
s32 Func_080a2680(s32 *category, s32 *value, s32 *index);
void Func_080a2490(void);
void Func_08015278(s32);
void Func_080a34c0(void);
void Func_080ae8dc(void);
void Func_08002dd8(s32);
void Func_080152a8(void);
void Func_08002df0(void *);
void Func_080a1050(void);
void Func_08015410(s32, s32, s32, s32);
void Func_0808a548(void);

#define RunAssetSelectionScreen Func_080a24d0

/* Run the modal asset-selection screen and publish an accepted selection. */
s32 RunAssetSelectionScreen(void)
{
    void *display_backup;
    struct AssetSelectionScreen *screen;
    s32 selected_index;
    s32 selected_value;
    s32 selected_category;
    s32 result;
    CopyFn copy_fn = (CopyFn)0x03001388;

    display_backup = Func_08004970(0x2000);
    screen = Func_080048b0(0x37, 0xa70);
    *(s16 *)((u8 *)ASSET_SELECTION_GLOBALS->display_state + 4) = 1;
    Func_08015408(0, 0, 30, 20);
    WaitFrames(1);
    Func_080a1090(0);
    screen->session_mode = Func_08077158(screen->session);
    Func_080a3354(0, 3, 0, 7);
    Func_080a5534();
    Func_080a2144(14);
    Func_08015418((void *)0x06002500);
    screen->window = UiWindow_CreateFar(13, 0, 17, 3, 2);
    Func_080a1070();
    copy_fn(display_backup, (void *)0x06004000, 0x2000);
    Func_080072f0((void *)0x06004000, 0x2000, 0x33333333, (void *)0x03000168);
    Func_080153e0(1);
    Func_080a2474();
    result = Func_080a2680(&selected_category, &selected_value, &selected_index);
    Func_080a2490();

    if (result == 1) {
        u8 *selection = ASSET_SELECTION_GLOBALS->selection_state;

        *(u16 *)(selection + 0x180) =
            (selected_category << 10) | (selected_index & 0x1ff);
        *(u16 *)(selection + 0x19a) = screen->selection_style;
    }

    Func_08015278(screen->resource_handle);
    ASSET_SELECTION_GLOBALS->process_state[0xea6] = 1;
    Func_080a34c0();
    Func_08015408(0, 0, 30, 20);
    Func_080ae8dc();
    Func_08002dd8(0x37);
    *(s16 *)((u8 *)ASSET_SELECTION_GLOBALS->display_state + 4) = 0;
    Func_080152a8();
    Func_080153e0(0);
    copy_fn((void *)0x06004000, display_backup, 0x2000);
    ASSET_SELECTION_GLOBALS->process_state[0xea6] = 0;
    Func_08002df0(display_backup);
    WaitFrames(1);
    Func_080a1050();
    WaitFrames(1);
    Func_08015410(0, 0, 30, 20);
    ASSET_SELECTION_GLOBALS->process_state[0xea6] = 0;
    Func_0808a548();
    return result;
}

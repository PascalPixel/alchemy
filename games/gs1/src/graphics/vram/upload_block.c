#include "types.h"
#include "scene.h"

/* graphics/vram/upload_block.c */
struct State {
    u8 unknown[156];
    u32 context;
    u32 source;
};

extern struct State gIw;

u32 Graphics_UploadVramBlock(void)
{
    void *source = (void *)gIw.source;
    u8 *context = (u8 *)gIw.context;

    if (source != 0) {
        u32 *active = (u32 *)(context + 0x13C0);

        if (*active != 0) {
            *active = 0;
            return Sys_Place(source, (void *)0x06004000, 0x4000);
        }
    }
    return (u32)source;
}

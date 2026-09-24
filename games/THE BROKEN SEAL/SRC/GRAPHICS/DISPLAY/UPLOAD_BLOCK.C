#include "TYPES.H"
#include "SCENE.H"

/* graphics/vram/Display_UploadBlock.c */
struct State {
    u8 unknown[156];
    u32 context;
    u32 source;
};

extern struct State gWorkSlot;

u32 Graphics_UploadVramBlock(void)
{
    void *source = (void *)gWorkSlot.source;
    u8 *context = (u8 *)gWorkSlot.context;

    if (source != 0) {
        u32 *active = (u32 *)(context + 0x13C0);

        if (*active != 0) {
            *active = 0;
            return ColorBuffer_BackupAndScaleThreeQuarters(source, (void *)0x06004000, 0x4000);
        }
    }
    return (u32)source;
}

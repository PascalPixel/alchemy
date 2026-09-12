#include "audio_engine_symbols.h"
#include "types.h"
#include "scene.h"
#include "audio_engine.h"

/* audio/init/initialize_runtime_defaults.c */
void Audio_Initialize(void);
extern u8 RomBytes_02003000;
extern u8 RomBytes_02003004;
extern u16 gOv3;
extern u16 gOv4;
extern u16 gOv5;
extern u8 gOv6;
extern u16 gOv7;
extern u16 gOv8;
extern u16 gOv9;
extern u16 gOv10;
extern u8 gOv11;
extern u8 gOv12;

void Audio_InitializeRuntimeDefaults(void)
{
    s16 *player_volume;
    s32 remaining;

    Audio_Initialize();
    gOv11 = 0xff;
    RomBytes_02003000 = 0;
    gOv9 = 0x100;
    gOv3 = 0x100;
    gOv5 = 4;
    gOv8 = 0x100;
    gOv10 = 0x100;
    gOv4 = 4;
    gOv6 = 0;
    gOv12 = 0;
    player_volume = &gOv7;
    RomBytes_02003004 = 0;
    remaining = 7;
    do {
        remaining--;
        *player_volume = 0;
        player_volume += 1;
    } while (remaining >= 0);
}

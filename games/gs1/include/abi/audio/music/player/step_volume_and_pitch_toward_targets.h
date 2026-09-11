#ifndef ALCHEMY_ABI_AUDIO_MUSIC_PLAYER_STEP_VOLUME_AND_PITCH_TOWARD_TARGETS
#define ALCHEMY_ABI_AUDIO_MUSIC_PLAYER_STEP_VOLUME_AND_PITCH_TOWARD_TARGETS

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080f9c44(void);
void Func_080fb2a4(u8 *, s32);
void Func_080fb2cc(u8 *, s32, s32);
void Func_080fb334(u8 *, s32, s32);

#define Audio_Apply Func_080fb2a4
#define Audio_Place Func_080fb2cc
#define Audio_Place2 Func_080fb334
#define Audio_Run Func_080f91e8
#define Audio_Run2 Func_080f9c44
#define gOv Data_02003000
#define gOv2 Data_02003008
#define gOv3 Data_0200300c
#define gOv4 Data_02003010
#define gOv5 Data_02003030
#define gOv6 Data_02003034
#define gOv7 Data_02003038
#define gOv8 Data_02004210
#define gOv9 Data_02004290

#endif

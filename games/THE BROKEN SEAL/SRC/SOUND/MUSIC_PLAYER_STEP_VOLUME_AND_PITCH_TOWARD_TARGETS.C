#include "TYPES.H"

extern u8 Data_02003000;
extern u16 Data_02003008;
extern u16 Data_0200300c;
extern u16 Data_02003010;
extern u16 Data_02003030;
extern u16 Data_02003034;
extern u16 Data_02003038;
extern u8 Data_02004210[];
extern u8 Data_02004290[];

void Func_080f9c44(void);
void Func_080fb2a4(u8 *, s32);
#define MusicPlayer_SetPitchAndUpdateFrequency Func_080fb2a4
void Func_080fb2cc(u8 *, s32, s32);
#define MusicPlayer_SetVolume Func_080fb2cc
void Func_080fb334(u8 *, s32, s32);
#define MusicPlayer_SetPitch Func_080fb334

void Func_080f91e8(void)
#define MusicPlayer_StepVolumeAndPitchTowardTargets Func_080f91e8
{
    s32 delta;

    if (Data_02003000 != 0) {
        if (Data_02003000 == 1) {
            if (Data_02004210[4] == 0) {
                Data_02003000 = 0;
                Data_02003034 = 0x100;
            }
        } else {
            Data_02003000 -= 1;
        }
    }
    if ((s16)Data_02003034 != (s16)Data_02003008) {
        delta = (s16)Data_02003034 - (s16)Data_02003008;
        if (delta > 0) {
            Data_02003008 = Data_02003008 + Data_02003010;
        } else {
            Data_02003008 = Data_02003008 - Data_02003010;
        }
        if ((((s16)Data_02003034 - (s16)Data_02003008) ^ delta) < 0) {
            Data_02003008 = Data_02003034;
        }
        MusicPlayer_SetVolume(Data_02004290, 255, Data_02003008);
    }
    if ((s16)Data_02003030 != (s16)Data_02003038) {
        delta = (s16)Data_02003030 - (s16)Data_02003038;
        if (delta > 0) {
            Data_02003038 = Data_02003038 + Data_0200300c;
        } else {
            Data_02003038 = Data_02003038 - Data_0200300c;
        }
        if ((((s16)Data_02003030 - (s16)Data_02003038) ^ delta) < 0) {
            Data_02003038 = Data_02003030;
        }
        MusicPlayer_SetPitchAndUpdateFrequency(Data_02004290, Data_02003038);
        MusicPlayer_SetPitch(Data_02004290, 255, (s16)((s16)Data_02003038 * 12 - 3072));
    }
    Func_080f9c44();
}

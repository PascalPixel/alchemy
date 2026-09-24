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

void Sound_TickDmaRestartTimer(void);
void MusicPlayer_SetPitchAndUpdateFrequency(u8 *, s32);
void MusicPlayer_SetVolume(u8 *, s32, s32);
void MusicPlayer_SetPitch(u8 *, s32, s32);

void MusicPlayer_StepVolumeAndPitchTowardTargets(void)
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
    Sound_TickDmaRestartTimer();
}

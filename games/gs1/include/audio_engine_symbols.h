#ifndef ALCHEMY_AUDIO_ENGINE_SYMBOLS_H
#define ALCHEMY_AUDIO_ENGINE_SYMBOLS_H

/*
 * Readable names for the audio engine's exact owner symbols.  The two command
 * slot names are deliberately temporary: their table positions are proven,
 * while their higher-level roles are not yet fully established.
 */
#define MusicTrack_ReadCommandByte         Func_080fa1c8
#define Pcm_KeyToFrequency                 Func_080fa1fc
#define Audio_NoopStub                     Func_080fa260
#define MusicPlayer_BeginFadeOut           Func_080fa280
#define Audio_Initialize                   Func_080fa2a0
#define AudioEngine_RunMixer               Func_080fa318
#define Audio_PlaySound                    Func_080fa324
#define Audio_PlaySoundIfInactive          Func_080fa350
#define Audio_PlayOrResumeSound            Func_080fa39c
#define Audio_StopSound                    Func_080fa3f0
#define Audio_ResumeSound                  Func_080fa424
#define Audio_StopAllPlayers               Func_080fa458
#define MusicPlayer_Resume                 Func_080fa484
#define Audio_ResumeAllPlayers             Func_080fa490
#define MusicPlayer_FadeOut                Func_080fa4bc
#define MusicPlayer_FadeOutPause           Func_080fa4cc
#define MusicPlayer_FadeIn                 Func_080fa4ec
#define MusicPlayer_ResetActiveTracks      Func_080fa514
#define CgbAudio_Initialize                Func_080fa55c

#define AudioCommand_InvokeSlot34          Func_080fa678
#define AudioCommand_InvokeSlot35          Func_080fa68c
#define AudioEngine_Initialize             Func_080fa6a0
#define AudioEngine_SetPcmRate             Func_080fa798
#define AudioEngine_SetMode                Func_080fa83c
#define AudioEngine_StopAllChannels        Func_080fa8d4
#define AudioEngine_DisablePcmDma          Func_080fa928
#define AudioEngine_EnablePcmDma           Func_080fa9a4
#define MusicPlayer_Initialize             Func_080fa9e0
#define MusicPlayer_StartSong              Func_080faa58
#define MusicPlayer_Stop                   Func_080fab3c
#define MusicPlayer_UpdateFade             Func_080fab7c
#define MusicTrack_UpdateVolumePitch       Func_080fac44
#define Cgb_KeyToFrequency                 Func_080facf8
#define Cgb_StopOscillator                 Func_080fada0
#define CgbChannel_UpdatePanEnvelope       Func_080fadf0
#define MusicPlayer_ExecuteMemoryAccessCommand Func_080fb518

#endif

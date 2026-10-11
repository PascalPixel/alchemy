/* FarCall_SoundTable: one entry per 8-byte stub, in table order. */
FAR_CALL(AudioEngine_RunMixerFar, AudioEngine_RunMixer, void, (void))
FAR_CALL(Audio_InitializeRuntimeDefaultsFar, Audio_InitializeRuntimeDefaults, void, (void))
#ifndef OWN_AudioCommand_PlayFar
FAR_CALL(AudioCommand_PlayFar, AudioCommand_Play, void, (s32 cue))
#endif
FAR_CALL(MusicPlayer_StepVolumeAndPitchTowardTargetsFar, MusicPlayer_StepVolumeAndPitchTowardTargets, void, (void))
FAR_CALL(MusicCommand_SetPitchAndUpdateFrequencyFar, MusicCommand_SetPitchAndUpdateFrequency, void, (u16 value))
FAR_SLOT(MusicCommand_SetPitch)
FAR_SLOT(Audio_SetWorkPairB)
FAR_SLOT(MusicCommand_SetVolume)
FAR_SLOT(Audio_SetMusicVolumeTargetAndStep)
FAR_ENTRY(AudioCommand_GetStateByteFar, AudioCommand_GetStateByte)
FAR_SLOT(AudioTest_RunParameterController)
FAR_SLOT(AudioCommand_StopAllPlayers)
FAR_SLOT(AudioCommand_ResumeAllPlayers)
FAR_SLOT(AudioCommand_UpdateToggleMask)
FAR_CALL(AudioCommand_GetSecondaryStateByteFar, AudioCommand_GetSecondaryStateByte, s32, (void))
FAR_CALL(AudioCommand_WaitForCompletionFar, AudioCommand_WaitForCompletion, void, (void))

.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global FarCall_SoundTable
FarCall_SoundTable:
	.global AudioEngine_RunMixerFar
	.thumb_func
AudioEngine_RunMixerFar:
	overlay_veneer AudioEngine_RunMixer
	.global Audio_InitializeRuntimeDefaultsFar
	.thumb_func
Audio_InitializeRuntimeDefaultsFar:
	overlay_veneer Audio_InitializeRuntimeDefaults
	@ O2 pending: shared code calls this stub Audio_PlayCue while overlay veneers of that name target the other label
	.global Audio_PlayCue
	.thumb_func
Audio_PlayCue:
	.global AudioCommand_PlayFar
	.thumb_func
AudioCommand_PlayFar:
	overlay_veneer AudioCommand_Play
	.global MusicPlayer_StepVolumeAndPitchTowardTargetsFar
	.thumb_func
MusicPlayer_StepVolumeAndPitchTowardTargetsFar:
	overlay_veneer MusicPlayer_StepVolumeAndPitchTowardTargets
	.global MusicCommand_SetPitchAndUpdateFrequencyFar
	.thumb_func
MusicCommand_SetPitchAndUpdateFrequencyFar:
	overlay_veneer MusicCommand_SetPitchAndUpdateFrequency
	overlay_veneer MusicCommand_SetPitch
	overlay_veneer Audio_SetWorkPairB
	overlay_veneer MusicCommand_SetVolume
	overlay_veneer Audio_SetMusicVolumeTargetAndStep
	.global AudioCommand_GetStateByteFar
	.thumb_func
AudioCommand_GetStateByteFar:
	overlay_veneer AudioCommand_GetStateByte
	overlay_veneer AudioTest_RunParameterController
	overlay_veneer AudioCommand_StopAllPlayers
	overlay_veneer AudioCommand_ResumeAllPlayers
	overlay_veneer AudioCommand_UpdateToggleMask
	.global AudioCommand_GetSecondaryStateByteFar
	.thumb_func
AudioCommand_GetSecondaryStateByteFar:
	overlay_veneer AudioCommand_GetSecondaryStateByte
	.global AudioCommand_WaitForCompletionFar
	.thumb_func
AudioCommand_WaitForCompletionFar:
	overlay_veneer AudioCommand_WaitForCompletion

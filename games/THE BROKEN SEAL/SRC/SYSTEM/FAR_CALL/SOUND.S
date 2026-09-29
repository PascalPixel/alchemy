@ Far-call stub table heading the sound command code at 0x080f9000: fixed 8-byte
@ veneers, each loading its target into r4 and branching, which the calling
@ convention permits. No compiler emits them; the shared overlay veneer macro
@ builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global FarCall_SoundTable
FarCall_SoundTable:
	.global Func_080f9000
	.thumb_func
Func_080f9000:
	overlay_veneer AudioEngine_RunMixer
	.global Audio_InitializeRuntimeDefaultsFar
	.global Func_080f9008
	.thumb_func
Audio_InitializeRuntimeDefaultsFar:
Func_080f9008:
	overlay_veneer Audio_InitializeRuntimeDefaults
	.global Audio_PlayCue
	.thumb_func
Audio_PlayCue:
	.global Func_080f9010
	.thumb_func
Func_080f9010:
	.global AudioCommand_PlayFar
	.thumb_func
AudioCommand_PlayFar:
	overlay_veneer AudioCommand_Play
	.global Func_080f9018
	.thumb_func
Func_080f9018:
	overlay_veneer MusicPlayer_StepVolumeAndPitchTowardTargets
	.global MusicCommand_SetPitchAndUpdateFrequencyFar
	.thumb_func
MusicCommand_SetPitchAndUpdateFrequencyFar:
	overlay_veneer MusicCommand_SetPitchAndUpdateFrequency
	overlay_veneer MusicCommand_SetPitch
	overlay_veneer Audio_SetWorkPairB
	overlay_veneer MusicCommand_SetVolume
	overlay_veneer Audio_SetWorkPairA
	.global AudioCommand_GetStateByteFar
	.global Func_080f9048
	.thumb_func
AudioCommand_GetStateByteFar:
Func_080f9048:
	.global Audio_Check
	.thumb_func
Audio_Check:
	overlay_veneer AudioCommand_GetStateByte
	overlay_veneer AudioTest_RunParameterController
	overlay_veneer AudioCommand_StopAllPlayers
	overlay_veneer AudioCommand_ResumeAllPlayers
	overlay_veneer AudioCommand_UpdateToggleMask
	.global Func_080f9070
	.thumb_func
Func_080f9070:
	overlay_veneer AudioCommand_GetSecondaryStateByte
	.global AudioCommand_WaitForCompletionFar
	.thumb_func
AudioCommand_WaitForCompletionFar:
	overlay_veneer AudioCommand_WaitForCompletion
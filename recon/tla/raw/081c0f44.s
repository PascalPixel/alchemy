.syntax unified
	.thumb
	.global Func_081c0f44
	.thumb_func
Func_081c0f44:
	push {lr}
	adds r1, r0, #0
	lsls r1, r1, #16
	ldr r0, .L_081c0f54
	lsrs r1, r1, #16
	bl MusicPlayer_SetPitchAndUpdateFrequency
	pop {pc}
.L_081c0f54:
	.4byte gMusicPlayerBgm

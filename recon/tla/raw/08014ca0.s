.syntax unified
	.thumb
	.global Func_08014ca0
	.thumb_func
Func_08014ca0:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #4]
	ldr r0, .L_08014cac
	subs r0, r0, r3
	bx lr
.L_08014cac:
	.4byte IwramSoundMixWorkspace

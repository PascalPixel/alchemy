.syntax unified
	.thumb
	.global Func_081c0c1c
	.thumb_func
Func_081c0c1c:
	push {lr}
	bl Audio_Initialize
	ldr r1, .L_081c0c5c
	ldr r3, .L_081c0c68
	ldr r2, .L_081c0c60
	strh r1, [r3]
	ldr r3, .L_081c0c6c
	ldr r0, .L_081c0c64
	strb r1, [r3]
	ldr r3, .L_081c0c70
	strh r2, [r3]
	ldr r3, .L_081c0c74
	strh r2, [r3]
	ldr r3, .L_081c0c78
	strh r0, [r3]
	ldr r3, .L_081c0c7c
	strh r2, [r3]
	ldr r3, .L_081c0c80
	strh r2, [r3]
	ldr r3, .L_081c0c84
	ldr r2, .L_081c0c88
	strh r0, [r3]
	ldr r3, .L_081c0c8c
	strb r1, [r3]
	ldr r3, .L_081c0c90
	strb r1, [r3]
	ldr r3, .L_081c0c94
	strb r1, [r3]
	movs r3, #7
	b .L_081c0c98
	.2byte 0x0000
.L_081c0c5c:
	.4byte 0x00000000
.L_081c0c60:
	.4byte 0x00000100
.L_081c0c64:
	.4byte 0x00000004
.L_081c0c68:
	.4byte Data_02005830
.L_081c0c6c:
	.4byte gMusicRestoreDelay
.L_081c0c70:
	.4byte gMusicVolumeTarget
.L_081c0c74:
	.4byte gMusicVolume
.L_081c0c78:
	.4byte Data_02005810
.L_081c0c7c:
	.4byte Data_02005834
.L_081c0c80:
	.4byte Data_0200583c
	.global Func_081c0c84
	.thumb_func
Func_081c0c84:
.L_081c0c84:
	.4byte Data_0200580c
.L_081c0c88:
	.4byte Data_02005820
.L_081c0c8c:
	.4byte Data_02005814
.L_081c0c90:
	.4byte Audio_CommandMask
.L_081c0c94:
	.4byte Data_02005804
.L_081c0c98:
	subs r3, #1
	strh r1, [r2]
	adds r2, #2
	cmp r3, #0
	bge .L_081c0c98
	movs r0, #144
	lsls r0, r0, #12
	bl Func_081c11ec
	pop {pc}

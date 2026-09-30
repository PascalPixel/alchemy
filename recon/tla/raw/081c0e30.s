.syntax unified
	.thumb
	.global Func_081c0e30
	.thumb_func
Func_081c0e30:
	push {r5, r6, lr}
	ldr r1, .L_081c0e7c
	ldrb r3, [r1]
	adds r2, r3, #0
	cmp r2, #0
	beq .L_081c0e56
	cmp r2, #1
	bne .L_081c0e52
	ldr r3, .L_081c0e80
	ldrb r3, [r3, #4]
	cmp r3, #0
	bne .L_081c0e56
	strb r3, [r1]
	ldr r2, .L_081c0e84
	ldr r3, .L_081c0e78
	strh r3, [r2]
	b .L_081c0e56
.L_081c0e52:
	adds r3, #255
	strb r3, [r1]
.L_081c0e56:
	ldr r3, .L_081c0e84
	ldr r1, .L_081c0e88
	movs r0, #0
	ldrsh r2, [r3, r0]
	movs r5, #0
	ldrsh r3, [r1, r5]
	ldrh r4, [r1]
	cmp r2, r3
	beq .L_081c0eba
	subs r0, r2, r3
	cmp r0, #0
	ble .L_081c0e90
	ldr r3, .L_081c0e8c
	.global Func_081c0e70
	.thumb_func
Func_081c0e70:
	ldrh r3, [r3]
	adds r3, r4, r3
	b .L_081c0e96
	.2byte 0x0000
.L_081c0e78:
	.4byte 0x00000100
.L_081c0e7c:
	.4byte gMusicRestoreDelay
.L_081c0e80:
	.4byte Data_02006a10
.L_081c0e84:
	.4byte gMusicVolumeTarget
.L_081c0e88:
	.4byte gMusicVolume
.L_081c0e8c:
	.4byte Data_02005810
.L_081c0e90:
	ldr r3, .L_081c0f20
	ldrh r3, [r3]
	subs r3, r4, r3
.L_081c0e96:
	strh r3, [r1]
	ldr r3, .L_081c0f24
	ldr r1, .L_081c0f28
	ldrh r4, [r3]
	movs r2, #0
	ldrsh r3, [r3, r2]
	movs r5, #0
	ldrsh r2, [r1, r5]
	subs r3, r3, r2
	eors r3, r0
	cmp r3, #0
	bge .L_081c0eb0
	strh r4, [r1]
.L_081c0eb0:
	ldrh r2, [r1]
	ldr r0, .L_081c0f2c
	movs r1, #255
	bl MusicPlayer_SetVolume
.L_081c0eba:
	ldr r3, .L_081c0f30
	ldr r1, .L_081c0f34
	movs r0, #0
	ldrsh r2, [r3, r0]
	movs r5, #0
	ldrsh r3, [r1, r5]
	ldrh r4, [r1]
	cmp r2, r3
	beq .L_081c0f1e
	subs r0, r2, r3
	cmp r0, #0
	ble .L_081c0eda
	ldr r3, .L_081c0f38
	ldrh r3, [r3]
	adds r3, r4, r3
	b .L_081c0ee0
.L_081c0eda:
	ldr r3, .L_081c0f38
	ldrh r3, [r3]
	subs r3, r4, r3
.L_081c0ee0:
	strh r3, [r1]
	ldr r3, .L_081c0f30
	ldr r6, .L_081c0f34
	ldrh r1, [r3]
	movs r2, #0
	ldrsh r3, [r3, r2]
	movs r4, #0
	ldrsh r2, [r6, r4]
	subs r3, r3, r2
	eors r3, r0
	cmp r3, #0
	bge .L_081c0efa
	strh r1, [r6]
.L_081c0efa:
	ldr r5, .L_081c0f2c
	ldrh r1, [r6]
	adds r0, r5, #0
	bl MusicPlayer_SetPitchAndUpdateFrequency
	movs r0, #0
	ldrsh r3, [r6, r0]
	movs r1, #255
	lsls r2, r3, #1
	adds r2, r2, r3
	movs r3, #244
	lsls r2, r2, #18
	lsls r3, r3, #24
	adds r2, r2, r3
	asrs r2, r2, #16
	adds r0, r5, #0
	bl MusicPlayer_SetPitch
.L_081c0f1e:
	pop {r5, r6, pc}
.L_081c0f20:
	.4byte Data_02005810
.L_081c0f24:
	.4byte gMusicVolumeTarget
.L_081c0f28:
	.4byte gMusicVolume
.L_081c0f2c:
	.4byte gMusicPlayerBgm
.L_081c0f30:
	.4byte Data_02005834
.L_081c0f34:
	.4byte Data_0200583c
.L_081c0f38:
	.4byte Data_0200580c

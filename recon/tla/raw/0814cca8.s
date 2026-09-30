.syntax unified
	.thumb
	.global Func_0814cca8
	.thumb_func
Func_0814cca8:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #36]
	ldr r6, [r3, #96]
	ldr r3, .L_0814ccf4
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	mov r8, r2
	ldr r2, .L_0814ccf8
	movs r3, #32
	strh r3, [r2, #6]
	movs r3, #206
	lsls r3, r3, #3
	adds r1, r1, r3
	ldrh r1, [r1]
	movs r0, #1
	movs r2, #24
	bl Func_08118040
	ldr r5, .L_0814ccfc
	movs r1, #128
	adds r0, r6, #0
	lsls r1, r1, #7
	movs r2, #0
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_0814cd00
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	mov lr, r5
	.2byte 0xf800
	b .L_0814cd04
	.2byte 0x0000
.L_0814ccf4:
	.4byte 0x00000001
.L_0814ccf8:
	.4byte Data_03001120
.L_0814ccfc:
	.4byte IwramFillWords
.L_0814cd00:
	.4byte 0x06004000
.L_0814cd04:
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_0814cd30
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0814cd34
	subs r2, #2
	strh r3, [r2]
	ldr r3, .L_0814cd38
	mov r2, r8
	strh r3, [r2]
	ldr r2, .L_0814cd3c
	movs r3, #120
	movs r0, #195
	str r3, [r2, #16]
	lsls r0, r0, #1
	bl Audio_PlayCue
	b .L_0814cd40
.L_0814cd30:
	.4byte 0x0000100e
.L_0814cd34:
	.4byte 0x00003f46
.L_0814cd38:
	.4byte 0x00007741
.L_0814cd3c:
	.4byte gCameraSceneParameters
.L_0814cd40:
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000

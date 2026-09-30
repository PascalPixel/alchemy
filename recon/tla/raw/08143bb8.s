.syntax unified
	.thumb
	.global Func_08143bb8
	.thumb_func
Func_08143bb8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	movs r0, #195
	lsls r3, r3, #18
	lsls r0, r0, #1
	ldr r5, [r3, #92]
	ldr r7, [r3, #36]
	bl Audio_PlayCue
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #160
	adds r3, r5, r2
	ldr r6, .L_08143c28
	ldr r3, [r3]
	ldr r2, .L_08143c2c
	strh r3, [r6, #4]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #164
	adds r5, r5, r3
	ldr r3, [r5]
	movs r1, #128
	strh r3, [r6, #6]
	movs r3, #120
	str r3, [r2, #12]
	str r3, [r2, #16]
	ldr r3, .L_08143c24
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	lsls r1, r1, #7
	ldr r3, .L_08143c30
	ldr r0, .L_08143c34
	mov lr, r3
	.2byte 0xf800
	ldr r0, .L_08143c38
	bl Func_08014644
	ldr r1, .L_08143c3c
	movs r3, #32
	strh r3, [r6, #6]
	ldr r0, .L_08143c40
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_08143c66
	b .L_08143c44
	.2byte 0x0000
.L_08143c24:
	.4byte 0x00000787
.L_08143c28:
	.4byte Data_03001120
.L_08143c2c:
	.4byte gCameraSceneParameters
.L_08143c30:
	.4byte IwramClearWords
.L_08143c34:
	.4byte 0x06004000
.L_08143c38:
	.4byte Func_08143488
.L_08143c3c:
	.4byte Data_020038e0
.L_08143c40:
	.4byte 0x04000208
.L_08143c44:
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #230
	adds r3, r3, r1
	lsls r2, r2, #7
	adds r3, #4
	adds r2, #65
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_08143c66:
	strh r4, [r0]
	movs r2, #128
	ldr r3, .L_08143ca0
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r2, #206
	lsls r2, r2, #3
	adds r3, r7, r2
	ldrh r1, [r3]
	movs r2, #7
	movs r0, #2
	bl Func_08118038
	movs r0, #1
	bl WaitFrames
	movs r3, #206
	lsls r3, r3, #3
	movs r2, #21
	movs r6, #0
	adds r7, r7, r3
	movs r5, #0
	mov r8, r2
	b .L_08143ca4
	.2byte 0x0000
.L_08143ca0:
	.4byte 0x00000000
.L_08143ca4:
	mov r3, r8
	subs r1, r3, r5
	ldrh r0, [r7]
	bl Func_08118048
	adds r6, #1
	movs r0, #1
	bl WaitFrames
	adds r5, #3
	cmp r6, #8
	bne .L_08143ca4
	ldr r1, .L_08143cfc
	ldr r0, .L_08143d00
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_08143cee
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #234
	adds r3, r3, r1
	lsls r2, r2, #7
	adds r3, #4
	adds r2, #65
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_08143cee:
	strh r4, [r0]
	movs r0, #1
	bl WaitFrames
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_08143cfc:
	.4byte Data_020038e0
.L_08143d00:
	.4byte 0x04000208

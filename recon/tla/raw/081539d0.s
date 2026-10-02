.syntax unified
	.thumb
	.global Func_081539d0
	.thumb_func
Func_081539d0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #92]
	ldr r3, [r3, #96]
	sub sp, #52
	adds r5, r0, #0
	movs r0, #0
	str r3, [sp, #16]
	mov r11, r2
	bl BattleFx_BeginCanvasLayer
	movs r1, #224
	lsls r1, r1, #3
	ldr r0, .L_08153c20
	add r1, r11
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	ldr r0, [r5, #4]
	movs r3, #1
	eors r0, r3
	add r1, sp, #20
	ldr r5, .L_08153c24
	bl Func_08144aac
	movs r3, #255
	movs r7, #0
	mov r8, r3
	movs r6, #0
.L_08153a1a:
	bl Random16
	mov r2, r8
	ands r0, r2
	subs r0, #127
	lsls r0, r0, #16
	str r0, [r5]
	bl Random16
	mov r3, r8
	ands r0, r3
	subs r0, #127
	lsls r0, r0, #16
	str r0, [r5, #4]
	bl Random16
	mov r2, r8
	ands r0, r2
	subs r0, #127
	movs r3, #128
	lsls r0, r0, #16
	adds r7, #1
	lsls r3, r3, #1
	str r0, [r5, #8]
	str r6, [r5, #12]
	str r6, [r5, #16]
	str r6, [r5, #20]
	str r6, [r5, #24]
	adds r5, #28
	cmp r7, r3
	bne .L_08153a1a
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #1
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	movs r5, #0
	add r3, r11
	movs r1, #200
	str r5, [r3]
	ldr r0, .L_08153c28
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	movs r3, #160
	mov r2, sp
	adds r2, #40
	lsls r3, r3, #15
	str r2, [sp, #12]
	str r5, [r2]
	str r3, [r2, #4]
	str r5, [r2, #8]
	mov r10, r5
.L_08153a8a:
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #48]
	bl Func_08014de4
	adds r1, r5, #0
	adds r1, #12
	adds r0, r5, #0
	bl Graphics_PrepareTransferInIwramWork
	ldr r0, [sp, #12]
	bl SceneTransform_ApplyPosition
	mov r3, r10
	negs r3, r3
	mov r2, r10
	ldr r6, .L_08153c24
	str r3, [sp, #8]
	lsls r2, r2, #8
	lsls r3, r3, #8
	movs r7, #0
	mov r8, r3
	mov r9, r2
.L_08153ab8:
	adds r3, r7, #0
	cmp r7, #0
	bge .L_08153ac0
	adds r3, r7, #3
.L_08153ac0:
	asrs r3, r3, #2
	cmp r10, r3
	bgt .L_08153ac8
	b .L_08153bc8
.L_08153ac8:
	ldr r3, [r6, #24]
	cmp r3, #0
	bne .L_08153bc8
	bl Graphics_SaveTransferWorkOnce
	movs r3, #3
	ands r3, r7
	cmp r3, #1
	beq .L_08153af6
	cmp r3, #1
	bgt .L_08153ae4
	cmp r3, #0
	beq .L_08153aee
	b .L_08153b12
.L_08153ae4:
	cmp r3, #2
	beq .L_08153afe
	cmp r3, #3
	beq .L_08153b06
	b .L_08153b12
.L_08153aee:
	mov r0, r9
	bl Func_08015068
	b .L_08153b12
.L_08153af6:
	mov r0, r8
	bl SceneTransform_ApplyPitch
	b .L_08153b12
.L_08153afe:
	mov r0, r8
	bl Func_080150e4
	b .L_08153b12
.L_08153b06:
	mov r0, r8
	bl SceneTransform_ApplyPitch
	mov r0, r8
	bl Func_080150e4
.L_08153b12:
	add r5, sp, #28
	adds r1, r5, #0
	adds r0, r6, #0
	bl Func_0815e1ec
	ldr r3, [r5]
	asrs r3, r3, #1
	str r3, [r5]
	bl Func_08014ea8
	ldr r2, [r5, #8]
	cmp r2, #249
	bgt .L_08153b32
	movs r3, #250
	str r3, [r5, #8]
	movs r2, #250
.L_08153b32:
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #122
	cmp r2, r3
	ble .L_08153b40
	str r3, [r5, #8]
	adds r2, r3, #0
.L_08153b40:
	adds r3, r2, #0
	subs r3, #250
	cmp r3, #0
	bge .L_08153b4a
	adds r3, #63
.L_08153b4a:
	asrs r3, r3, #6
	movs r0, #9
	subs r0, r0, r3
	movs r3, #3
	ands r3, r7
	lsls r1, r3, #1
	ldr r2, .L_08153c2c
	adds r1, r1, r3
	lsls r4, r0, #1
	lsls r1, r1, #7
	adds r1, r1, r3
	subs r3, r4, #2
	ldrh r3, [r2, r3]
	lsls r1, r1, #1
	adds r1, r1, r3
	movs r3, #224
	lsls r3, r3, #3
	add r1, r11
	ldr r2, [r5]
	adds r1, r1, r3
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, [r5, #4]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #16]
	ldr r4, [sp, #20]
	mov lr, r4
	.2byte 0xf800
	adds r0, r6, #0
	movs r1, #60
	movs r2, #0
	bl BattleFxKernels_IntegrateVector3
	adds r3, r7, #0
	cmp r7, #0
	bge .L_08153b9c
	adds r3, r7, #3
.L_08153b9c:
	asrs r3, r3, #2
	adds r3, #30
	cmp r10, r3
	ble .L_08153bc8
	ldr r2, [r6]
	ldr r3, [r6, #12]
	negs r2, r2
	asrs r2, r2, #8
	ldr r1, [r6, #4]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	negs r1, r1
	asrs r1, r1, #8
	ldr r0, [r6, #8]
	adds r3, r3, r1
	str r3, [r6, #16]
	ldr r3, [r6, #20]
	negs r0, r0
	asrs r0, r0, #8
	adds r3, r3, r0
	str r3, [r6, #20]
.L_08153bc8:
	ldr r2, [sp, #8]
	adds r7, #1
	lsls r3, r2, #3
	mov r2, r10
	add r8, r3
	lsls r3, r2, #3
	add r9, r3
	adds r6, #28
	cmp r7, #64
	beq .L_08153bde
	b .L_08153ab8
.L_08153bde:
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r11
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r3, #1
	add r10, r3
	mov r2, r10
	cmp r2, #160
	beq .L_08153bfc
	b .L_08153a8a
.L_08153bfc:
	ldr r0, .L_08153c28
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #52
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08153c20:
	.4byte 0x0000014f
.L_08153c24:
	.4byte gMapCellBuffer
.L_08153c28:
	.4byte Func_08143000
.L_08153c2c:
	.4byte Data_08197410

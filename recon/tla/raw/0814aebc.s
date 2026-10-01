.syntax unified
	.thumb
	.global Func_0814aebc
	.thumb_func
Func_0814aebc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #56
	str r0, [sp, #28]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #92]
	movs r2, #0
	str r0, [sp, #24]
	movs r0, #1
	ldr r1, [r5, #96]
	str r2, [sp, #8]
	str r1, [sp, #20]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_0814af20
	movs r2, #128
	lsls r2, r2, #19
	add r4, sp, #8
	adds r2, #32
	strh r3, [r2]
	ldrh r4, [r4]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #80
	strh r4, [r3]
	ldr r2, [sp, #24]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0814af24
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r1, #35
	movs r0, #104
	bl Func_081963ec
	ldr r4, [r5, #104]
	movs r0, #188
	movs r1, #47
	str r4, [sp, #12]
	b .L_0814af28
	.2byte 0x0000
.L_0814af20:
	.4byte 0x00000100
.L_0814af24:
	.4byte 0x000000e3
.L_0814af28:
	bl Func_081963ec
	adds r5, #188
	ldr r5, [r5]
	movs r7, #0
	str r5, [sp, #16]
	ldr r5, .L_0814b14c
.L_0814af36:
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #32
	lsls r3, r3, #16
	str r3, [r5]
	ldr r3, .L_0814b150
	movs r6, #0
	str r3, [r5, #4]
	bl Random16
	str r6, [r5, #16]
	bl Random16
	movs r3, #3
	ands r3, r0
	str r3, [r5, #8]
	bl Random16
	movs r3, #255
	ands r3, r0
	adds r7, #1
	str r3, [r5, #24]
	adds r5, #28
	cmp r7, #32
	bne .L_0814af36
	add r5, sp, #44
	adds r0, r5, #0
	bl Func_0815e22c
	ldr r2, [r5]
	adds r3, r2, #0
	subs r3, #64
	str r3, [r5]
	cmp r3, #0
	bge .L_0814af86
	str r3, [sp, #8]
	str r6, [r5]
	b .L_0814af92
.L_0814af86:
	cmp r3, #112
	ble .L_0814af92
	subs r2, #176
	movs r3, #112
	str r2, [sp, #8]
	str r3, [r5]
.L_0814af92:
	ldr r3, [r5]
	movs r2, #128
	lsls r2, r2, #19
	negs r3, r3
	adds r2, #40
	lsls r3, r3, #8
	str r3, [r2]
	ldr r0, [sp, #24]
	movs r1, #239
	lsls r1, r1, #7
	adds r2, r0, r1
	movs r3, #1
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r0, r3
	movs r1, #200
	movs r3, #0
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0814b154
	bl Scheduler_AddOrUpdateCallback
	movs r0, #142
	bl Audio_PlayCue
	movs r4, #0
	mov r11, r4
.L_0814afcc:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #48]
	mov r0, r11
	mov r10, r3
	cmp r0, #80
	bne .L_0814afe0
	movs r0, #0
	bl Func_081180e8
.L_0814afe0:
	ldr r2, [sp, #28]
	movs r1, #0
	ldr r3, [r2, #20]
	mov r8, r1
	cmp r3, #0
	beq .L_0814b04a
	movs r3, #12
	add r3, r10
	mov r9, r3
	add r6, sp, #32
	movs r7, #36
.L_0814aff6:
	ldr r4, [sp, #28]
	ldrsh r0, [r7, r4]
	bl GetBattleObjectSlotFar
	ldr r5, [r0]
	bl Func_08014de4
	mov r0, r10
	mov r1, r9
	bl Graphics_PrepareTransferInIwramWork
	ldr r3, [r5, #8]
	adds r0, r6, #0
	str r3, [r6]
	movs r3, #160
	lsls r3, r3, #14
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	str r3, [r6, #8]
	bl SceneTransform_ApplyPosition
	mov r2, r8
	lsls r3, r2, #4
	adds r3, #64
	cmp r11, r3
	bne .L_0814b03c
	ldr r3, [sp, #28]
	movs r1, #0
	ldrsh r0, [r7, r3]
	movs r3, #0
	str r3, [sp, #0]
	movs r2, #5
	subs r3, #1
	bl Func_0814cd48
.L_0814b03c:
	ldr r1, [sp, #28]
	movs r0, #1
	ldr r3, [r1, #20]
	add r8, r0
	adds r7, #2
	cmp r8, r3
	bne .L_0814aff6
.L_0814b04a:
	ldr r6, .L_0814b14c
	movs r2, #32
	movs r7, #0
	mov r8, r2
.L_0814b052:
	lsls r3, r7, #2
	cmp r11, r3
	ble .L_0814b0fe
	ldr r0, [r6, #4]
	ldr r3, .L_0814b158
	cmp r0, r3
	bgt .L_0814b0fe
	ldr r1, [r6, #24]
	cmp r1, #0
	bge .L_0814b068
	adds r1, #15
.L_0814b068:
	asrs r1, r1, #4
	movs r3, #7
	ands r1, r3
	cmp r1, #3
	bgt .L_0814b09c
	ldr r4, [sp, #24]
	lsls r1, r1, #10
	movs r2, #240
	adds r1, r4, r1
	lsls r2, r2, #4
	ldr r4, [sp, #8]
	adds r1, r1, r2
	movs r3, #2
	ldrsh r2, [r6, r3]
	asrs r3, r0, #16
	adds r2, r4, r2
	mov r0, r8
	str r0, [sp, #0]
	str r0, [sp, #4]
	subs r2, #16
	subs r3, #16
	ldr r0, [sp, #20]
	ldr r4, [sp, #12]
	mov lr, r4
	.2byte 0xf800
	b .L_0814b0c2
.L_0814b09c:
	ldr r2, [sp, #24]
	ldr r3, .L_0814b15c
	lsls r1, r1, #10
	adds r1, r2, r1
	adds r1, r1, r3
	movs r4, #2
	ldrsh r2, [r6, r4]
	ldr r3, [sp, #8]
	mov r4, r8
	adds r2, r3, r2
	asrs r3, r0, #16
	str r4, [sp, #0]
	str r4, [sp, #4]
	subs r2, #16
	subs r3, #16
	ldr r0, [sp, #20]
	ldr r4, [sp, #16]
	mov lr, r4
	.2byte 0xf800
.L_0814b0c2:
	ldr r4, [r6, #16]
	movs r0, #128
	lsls r0, r0, #6
	ldr r2, [r6, #4]
	adds r1, r4, r0
	ldr r3, [r6, #24]
	ldr r0, [r6, #8]
	movs r5, #184
	adds r2, r2, r4
	adds r3, r3, r0
	lsls r5, r5, #15
	str r2, [r6, #4]
	str r1, [r6, #16]
	str r3, [r6, #24]
	cmp r2, r5
	ble .L_0814b0fe
	cmp r1, #0
	bne .L_0814b0fe
	movs r1, #128
	lsls r1, r1, #6
	adds r1, #1
	adds r3, r4, r1
	adds r2, r0, #4
	negs r3, r3
	str r2, [r6, #8]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r5, [r6, #4]
	str r3, [r6, #16]
.L_0814b0fe:
	adds r7, #1
	adds r6, #28
	cmp r7, #12
	bne .L_0814b052
	ldr r3, [sp, #24]
	movs r4, #240
	lsls r4, r4, #7
	adds r4, #232
	adds r2, r3, r4
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	movs r0, #1
	add r11, r0
	mov r1, r11
	cmp r1, #98
	beq .L_0814b126
	b .L_0814afcc
.L_0814b126:
	ldr r0, .L_0814b154
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0814b14c:
	.4byte gMapCellBuffer
.L_0814b150:
	.4byte 0xffe00000
.L_0814b154:
	.4byte Func_08143000
.L_0814b158:
	.4byte 0x007fffff
.L_0814b15c:
	.4byte 0xffffff00

.syntax unified
	.thumb
	.global Func_0816f928
	.thumb_func
Func_0816f928:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r5, #192
	lsls r5, r5, #18
	mov r9, r0
	ldr r0, [r5, #92]
	ldr r2, [r5, #96]
	sub sp, #32
	mov r8, r0
	movs r0, #1
	str r2, [sp, #16]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_0816f98c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	movs r1, #3
	movs r0, #104
	bl Func_081963ec
	ldr r5, [r5, #104]
	ldr r0, .L_0816f990
	str r5, [sp, #8]
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0816f994
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	movs r2, #239
	lsls r2, r2, #7
	add r2, r8
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	movs r3, #50
	b .L_0816f998
	.2byte 0x0000
.L_0816f98c:
	.4byte 0x00001010
.L_0816f990:
	.4byte 0x00000166
.L_0816f994:
	.4byte IwramCopyWords
.L_0816f998:
	add r2, r8
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0816fa00
	bl Scheduler_AddOrUpdateCallback
	mov r4, r9
	add r5, sp, #20
	movs r3, #36
	ldrsh r0, [r4, r3]
	adds r1, r5, #0
	bl Func_0815e21c
	ldr r2, [r5]
	movs r3, #64
	subs r3, r3, r2
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #40
	lsls r3, r3, #8
	movs r5, #224
	str r3, [r2]
	lsls r5, r5, #3
	mov r2, r8
	movs r1, #128
	lsls r1, r1, #1
	adds r0, r2, r5
	ldr r3, .L_0816fa04
	mov lr, r3
	.2byte 0xf800
	movs r0, #224
	lsls r0, r0, #3
	movs r7, #0
	adds r0, #3
	movs r1, #24
.L_0816f9e0:
	mov r4, r9
	ldr r3, [r4, #4]
	cmp r3, #0
	bne .L_0816fa08
	lsls r2, r7, #2
	adds r3, r7, #0
	cmp r7, #0
	bge .L_0816f9f2
	adds r3, #15
.L_0816f9f2:
	asrs r3, r3, #4
	subs r3, r2, r3
	adds r3, r3, r0
	mov r2, r8
	strb r1, [r2, r3]
	b .L_0816fa1c
	.2byte 0x0000
.L_0816fa00:
	.4byte Func_08143000
.L_0816fa04:
	.4byte IwramClearWords
.L_0816fa08:
	lsls r2, r7, #2
	adds r3, r7, #0
	cmp r7, #0
	bge .L_0816fa12
	adds r3, #15
.L_0816fa12:
	asrs r3, r3, #4
	adds r3, r2, r3
	adds r3, r3, r5
	mov r4, r8
	strb r1, [r4, r3]
.L_0816fa1c:
	adds r7, #1
	cmp r7, #64
	bne .L_0816f9e0
	movs r1, #228
	lsls r1, r1, #6
	ldr r0, .L_0816fa70
	movs r2, #1
	movs r3, #0
	add r1, r8
	bl Resource_LoadAndDecompress
	movs r2, #128
	ldr r3, .L_0816fa6c
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	movs r0, #0
	mov r11, r0
.L_0816fa40:
	mov r2, r11
	cmp r2, #0
	bne .L_0816fac2
	movs r0, #170
	bl Audio_PlayCue
	ldr r3, .L_0816fa74
	movs r1, #1
	movs r2, #128
	movs r7, #0
	negs r1, r1
	lsls r2, r2, #1
.L_0816fa58:
	adds r7, #1
	str r1, [r3]
	adds r3, #28
	cmp r7, r2
	bne .L_0816fa58
	movs r7, #0
	movs r6, #63
	mov r5, r8
	b .L_0816fa78
	.2byte 0x0000
.L_0816fa6c:
	.4byte 0x00000100
.L_0816fa70:
	.4byte 0x00000161
.L_0816fa74:
	.4byte Data_02010018
.L_0816fa78:
	bl Random16
	movs r3, #31
	ands r3, r0
	subs r3, #64
	lsls r3, r3, #16
	str r3, [r5, #4]
	mov r4, r9
	ldr r3, [r4, #4]
	cmp r3, #0
	bne .L_0816fa9c
	bl Random16
	ldr r3, .L_0816fc74
	ands r0, r6
	adds r0, #40
	lsls r0, r0, #16
	b .L_0816faaa
.L_0816fa9c:
	bl Random16
	ands r0, r6
	adds r0, #24
	movs r3, #128
	lsls r0, r0, #16
	lsls r3, r3, #9
.L_0816faaa:
	str r0, [r5]
	str r3, [r5, #12]
	movs r3, #128
	lsls r3, r3, #13
	str r3, [r5, #16]
	movs r3, #1
	negs r3, r3
	adds r7, #1
	str r3, [r5, #24]
	adds r5, #28
	cmp r7, #64
	bne .L_0816fa78
.L_0816fac2:
	mov r0, r11
	cmp r0, #32
	bne .L_0816fada
	mov r2, r9
	movs r3, #36
	ldrsh r1, [r2, r3]
	movs r3, #192
	ldr r0, [r2, #8]
	lsls r3, r3, #10
	movs r2, #8
	bl BattleMotion_ApproachTargetFar
.L_0816fada:
	mov r4, r11
	cmp r4, #46
	bne .L_0816faf2
	movs r0, #134
	bl Func_081180e8
	mov r3, r9
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r1, #4
	bl Func_08118088
.L_0816faf2:
	ldr r5, .L_0816fc78
	movs r7, #0
.L_0816faf6:
	ldr r3, [r5, #24]
	cmp r3, #23
	bhi .L_0816fb3e
	cmp r3, #0
	bge .L_0816fb02
	adds r3, #3
.L_0816fb02:
	asrs r3, r3, #2
	lsls r1, r3, #3
	adds r1, r1, r3
	lsls r1, r1, #7
	movs r4, #228
	lsls r4, r4, #6
	add r1, r8
	movs r0, #2
	ldrsh r2, [r5, r0]
	adds r1, r1, r4
	movs r4, #6
	ldrsh r3, [r5, r4]
	movs r0, #24
	str r0, [sp, #0]
	movs r0, #48
	subs r3, #32
	str r0, [sp, #4]
	subs r2, #12
	ldr r0, [sp, #16]
	ldr r4, [sp, #8]
	mov lr, r4
	.2byte 0xf800
	adds r0, r5, #0
	movs r1, #56
	ldr r2, .L_0816fc7c
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
.L_0816fb3e:
	movs r0, #128
	adds r7, #1
	lsls r0, r0, #1
	adds r5, #28
	cmp r7, r0
	bne .L_0816faf6
	movs r7, #0
	movs r5, #6
.L_0816fb4e:
	cmp r11, r5
	bne .L_0816fb6c
	mov r3, r9
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r3, #3
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
	movs r0, #132
	bl Audio_PlayCue
.L_0816fb6c:
	adds r7, #1
	adds r5, #4
	cmp r7, #8
	bne .L_0816fb4e
	movs r4, #0
	movs r7, #0
	mov r6, r8
	mov r10, r4
.L_0816fb7c:
	cmp r11, r7
	blt .L_0816fbfa
	movs r0, #6
	ldrsh r4, [r6, r0]
	cmp r4, #111
	bgt .L_0816fbfa
	movs r1, #64
	cmp r4, #48
	ble .L_0816fb92
	movs r3, #112
	subs r1, r3, r4
.L_0816fb92:
	movs r3, #2
	ldrsh r2, [r6, r3]
	str r1, [sp, #4]
	movs r1, #224
	movs r3, #4
	lsls r1, r1, #3
	str r3, [sp, #0]
	add r1, r8
	adds r3, r4, #0
	ldr r0, [sp, #16]
	ldr r4, [sp, #8]
	mov lr, r4
	.2byte 0xf800
	ldr r2, [r6]
	ldr r3, [r6, #12]
	adds r1, r2, r3
	ldr r3, [r6, #4]
	ldr r2, [r6, #16]
	str r1, [r6]
	adds r3, r3, r2
	str r3, [r6, #4]
	asrs r3, r3, #16
	cmp r3, #48
	ble .L_0816fbfa
	ldr r5, .L_0816fc78
	movs r0, #1
	add r5, r10
	ldr r3, [r5, #24]
	negs r0, r0
	cmp r3, r0
	bne .L_0816fbfa
	movs r3, #224
	lsls r3, r3, #15
	str r3, [r5, #4]
	movs r3, #0
	str r3, [r5, #24]
	str r1, [r5]
	bl Random16
	movs r1, #12
	bl Math_ModU
	subs r0, #6
	lsls r0, r0, #15
	str r0, [r5, #12]
	bl Random16
	movs r3, #7
	negs r0, r0
	ands r0, r3
	lsls r0, r0, #10
	str r0, [r5, #16]
.L_0816fbfa:
	movs r2, #28
	adds r7, #1
	adds r6, #28
	add r10, r2
	cmp r7, #32
	bne .L_0816fb7c
	movs r0, #4
	movs r1, #4
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	movs r3, #1
	add r2, r8
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r3, #1
	add r11, r3
	mov r4, r11
	cmp r4, #68
	beq .L_0816fc30
	b .L_0816fa40
.L_0816fc30:
	ldr r6, .L_0816fc80
	adds r0, r6, #0
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	movs r1, #240
	ldr r5, .L_0816fc84
	lsls r1, r1, #6
	ldr r0, .L_0816fc88
	mov lr, r5
	.2byte 0xf800
	movs r1, #240
	ldr r0, [sp, #16]
	lsls r1, r1, #6
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_0816fc8c
	bl Scheduler_RemoveCallback
	adds r0, r6, #0
	bl Scheduler_RemoveCallback
	mov r0, r9
	bl Func_081504b4
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0816fc74:
	.4byte 0xffff0000
.L_0816fc78:
	.4byte gMapCellBuffer
.L_0816fc7c:
	.4byte 0xffffc000
.L_0816fc80:
	.4byte Func_08143000
.L_0816fc84:
	.4byte IwramClearWords
.L_0816fc88:
	.4byte 0x06004000
.L_0816fc8c:
	.4byte Func_08143488

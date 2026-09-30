.syntax unified
	.thumb
	.global Func_0814b160
	.thumb_func
Func_0814b160:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r5, #192
	lsls r5, r5, #18
	mov r10, r0
	ldr r0, [r5, #96]
	sub sp, #40
	str r0, [sp, #24]
	movs r0, #1
	ldr r1, [r5, #92]
	str r1, [sp, #20]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_0814b1bc
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r3, .L_0814b1c0
	adds r2, #48
	strh r3, [r2]
	ldr r2, [sp, #20]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0814b1c4
	movs r3, #1
	movs r2, #1
	bl Func_08157cf4
	mov r4, r10
	ldr r3, [r4, #4]
	cmp r3, #1
	bne .L_0814b1cc
	movs r2, #128
	ldr r3, .L_0814b1c8
	lsls r2, r2, #19
	adds r2, #40
	str r3, [r2]
	b .L_0814b1cc
	.2byte 0x0000
.L_0814b1bc:
	.4byte 0x00000100
.L_0814b1c0:
	.4byte 0x00000000
.L_0814b1c4:
	.4byte 0x0000013b
.L_0814b1c8:
	.4byte 0xffff9000
.L_0814b1cc:
	movs r1, #35
	movs r0, #104
	bl Func_081963ec
	ldr r0, [sp, #20]
	movs r1, #239
	lsls r1, r1, #7
	adds r2, r0, r1
	movs r3, #1
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	ldr r5, [r5, #104]
	adds r3, #132
	adds r2, r0, r3
	movs r1, #200
	movs r3, #0
	str r3, [r2]
	ldr r0, .L_0814b34c
	lsls r1, r1, #4
	str r5, [sp, #12]
	bl Scheduler_AddOrUpdateCallback
	mov r0, r10
	ldr r2, [r0, #20]
	movs r4, #0
	mov r9, r4
	cmp r2, #0
	beq .L_0814b222
	ldr r5, [sp, #20]
	movs r6, #63
	adds r5, #24
.L_0814b20c:
	bl Random16
	ands r0, r6
	str r0, [r5]
	mov r3, r10
	ldr r2, [r3, #20]
	movs r1, #1
	add r9, r1
	adds r5, #28
	cmp r9, r2
	bne .L_0814b20c
.L_0814b222:
	movs r0, #32
	movs r4, #0
	lsls r3, r2, #5
	negs r0, r0
	mov r8, r4
	cmp r3, r0
	beq .L_0814b32c
.L_0814b230:
	mov r1, r8
	cmp r1, #32
	bne .L_0814b240
	movs r0, #0
	bl Func_081180e8
	mov r3, r10
	ldr r2, [r3, #20]
.L_0814b240:
	movs r4, #0
	mov r9, r4
	cmp r2, #0
	beq .L_0814b306
	movs r0, #36
	ldr r1, [sp, #20]
	str r0, [sp, #8]
	mov r11, r1
.L_0814b250:
	mov r2, r9
	lsls r6, r2, #4
	cmp r8, r6
	bne .L_0814b25e
	movs r0, #143
	bl Audio_PlayCue
.L_0814b25e:
	cmp r8, r6
	blt .L_0814b2f0
	adds r3, r6, #0
	adds r3, #72
	cmp r8, r3
	bge .L_0814b2f0
	ldr r3, [sp, #8]
	mov r1, r10
	add r5, sp, #28
	ldrsh r0, [r3, r1]
	adds r1, r5, #0
	bl Func_0815e20c
	mov r2, r10
	ldr r3, [r2, #4]
	cmp r3, #1
	bne .L_0814b286
	ldr r3, [r5]
	subs r3, #112
	str r3, [r5]
.L_0814b286:
	ldr r3, [r5, #4]
	movs r1, #20
	adds r2, r3, #0
	subs r2, #16
	str r2, [r5, #4]
	subs r3, #20
	ldr r2, [r5]
	mov r12, r3
	ldr r4, [sp, #20]
	movs r3, #16
	str r3, [sp, #0]
	movs r3, #220
	lsls r3, r3, #4
	str r1, [sp, #4]
	subs r2, #8
	adds r1, r4, r3
	ldr r7, [sp, #12]
	ldr r0, [sp, #24]
	mov r3, r12
	mov lr, r7
	.2byte 0xf800
	cmp r8, r6
	blt .L_0814b2f0
	mov r1, r11
	ldr r3, [r1, #24]
	mov r4, r8
	subs r0, r4, r6
	adds r0, r0, r3
	movs r1, #6
	bl Math_Div
	movs r1, #9
	bl __modsi3
	lsls r1, r0, #1
	ldr r2, [sp, #20]
	adds r1, r1, r0
	lsls r1, r1, #6
	movs r3, #224
	adds r1, r2, r1
	lsls r3, r3, #3
	ldr r2, [r5]
	adds r1, r1, r3
	ldr r3, [r5, #4]
	movs r0, #12
	movs r4, #16
	str r0, [sp, #4]
	subs r2, #8
	subs r3, #16
	str r4, [sp, #0]
	ldr r0, [sp, #24]
	mov lr, r7
	.2byte 0xf800
.L_0814b2f0:
	ldr r0, [sp, #8]
	mov r4, r10
	adds r0, #2
	str r0, [sp, #8]
	movs r2, #1
	ldr r3, [r4, #20]
	movs r1, #28
	add r9, r2
	add r11, r1
	cmp r9, r3
	bne .L_0814b250
.L_0814b306:
	ldr r0, [sp, #20]
	movs r1, #240
	lsls r1, r1, #7
	adds r1, #232
	adds r2, r0, r1
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	mov r4, r10
	ldr r3, [r4, #20]
	movs r2, #1
	add r8, r2
	adds r2, r3, #0
	lsls r3, r2, #5
	adds r3, #32
	cmp r8, r3
	bne .L_0814b230
.L_0814b32c:
	ldr r0, .L_0814b34c
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0814b34c:
	.4byte Func_08143000

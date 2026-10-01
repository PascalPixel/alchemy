.syntax unified
	.thumb
	.global Func_080e9090
	.thumb_func
Func_080e9090:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #132
	lsls r1, r1, #6
	adds r1, #84
	movs r0, #92
	sub sp, #32
	bl Runtime_AllocateHeapBlock
	movs r2, #192
	str r0, [sp, #28]
	lsls r2, r2, #18
	adds r3, r2, #0
	adds r3, #224
	ldr r5, [r3]
	ldr r2, [r2, #108]
	str r2, [sp, #24]
	bl BattleEffect_InitializeSharedScene
	movs r1, #0
	ldr r0, [r5, #16]
	bl Func_080e1420
	movs r0, #1
	bl Func_080e89e4
	bl Func_080e8cfc
	movs r0, #10
	bl WaitFrames
	movs r0, #0
	movs r1, #128
	str r0, [sp, #16]
	str r0, [sp, #20]
	str r0, [sp, #4]
	lsls r1, r1, #10
	mov r9, r1
	mov r10, r0
	mov r11, r0
.L_080e90ea:
	ldr r2, [sp, #20]
	movs r3, #15
	ands r3, r2
	cmp r3, #15
	bne .L_080e90fa
	movs r0, #152
	bl Audio_PlayCue
.L_080e90fa:
	ldr r0, [sp, #24]
	movs r1, #197
	lsls r1, r1, #1
	adds r3, r0, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #2
	bne .L_080e9170
	ldr r0, [sp, #28]
	movs r2, #0
	ldr r3, [r0, #80]
	cmp r2, r3
	bge .L_080e9170
	ldr r3, [sp, #28]
	ldr r1, [sp, #4]
	adds r3, #32
	str r1, [sp, #12]
	str r3, [sp, #8]
	adds r0, #64
	mov r8, r0
.L_080e9124:
	ldr r1, [sp, #8]
	ldmia r1!, {r3}
	adds r0, r1, #0
	str r0, [sp, #8]
	mov r1, r9
	ldr r7, [r3, #80]
	movs r0, #2
	mov r3, r8
	ldrh r6, [r3]
	add r8, r0
	cmp r1, #0
	ble .L_080e9164
	ldr r3, [sp, #12]
	str r2, [sp, #0]
	lsls r0, r3, #11
	bl Trig_Sin
	movs r1, #192
	adds r5, r0, #0
	lsls r1, r1, #1
	mov r0, r9
	bl __divsi3
	ldr r3, .L_080e9244
	adds r1, r0, #0
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	adds r0, r6, r0
	strh r0, [r7, #18]
	ldr r2, [sp, #0]
	b .L_080e9166
.L_080e9164:
	strh r6, [r7, #18]
.L_080e9166:
	ldr r0, [sp, #28]
	adds r2, #1
	ldr r3, [r0, #80]
	cmp r2, r3
	blt .L_080e9124
.L_080e9170:
	ldr r1, [sp, #16]
	ldr r2, [sp, #4]
	adds r3, r1, #0
	adds r2, #3
	str r2, [sp, #4]
	adds r3, #1
	movs r0, #1
	mov r2, r9
	str r3, [sp, #16]
	bl Func_080e8b44
	mov r0, r10
	cmp r0, #1
	beq .L_080e91b6
	cmp r0, #1
	bgt .L_080e9196
	cmp r0, #0
	beq .L_080e91a2
	b .L_080e9206
.L_080e9196:
	mov r1, r10
	cmp r1, #2
	beq .L_080e91dc
	cmp r1, #3
	beq .L_080e91f4
	b .L_080e9206
.L_080e91a2:
	movs r2, #152
	lsls r2, r2, #6
	ldr r3, .L_080e9248
	adds r2, #102
	add r9, r2
	cmp r9, r3
	ble .L_080e9206
	movs r1, #1
	movs r0, #1
	b .L_080e91c8
.L_080e91b6:
	ldr r2, [sp, #28]
	ldr r3, [r2, #80]
	cmp r3, #0
	beq .L_080e91d0
	mov r3, r11
	cmp r3, #50
	bne .L_080e9206
	movs r1, #1
	movs r0, #2
.L_080e91c8:
	negs r1, r1
	mov r10, r0
	mov r11, r1
	b .L_080e9206
.L_080e91d0:
	mov r2, r11
	cmp r2, #10
	bne .L_080e9206
	movs r0, #1
	movs r3, #2
	b .L_080e91ec
.L_080e91dc:
	ldr r1, .L_080e924c
	movs r2, #128
	add r9, r1
	lsls r2, r2, #10
	cmp r9, r2
	bgt .L_080e9206
	movs r0, #1
	movs r3, #3
.L_080e91ec:
	negs r0, r0
	mov r10, r3
	mov r11, r0
	b .L_080e9206
.L_080e91f4:
	movs r1, #0
	mov r2, r11
	mov r9, r1
	cmp r2, #0
	bne .L_080e9206
	movs r3, #186
	lsls r3, r3, #2
	adds r3, #255
	mov r10, r3
.L_080e9206:
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #20]
	movs r2, #186
	lsls r2, r2, #2
	movs r0, #1
	adds r1, #1
	adds r2, #255
	add r11, r0
	str r1, [sp, #20]
	cmp r10, r2
	beq .L_080e9222
	b .L_080e90ea
.L_080e9222:
	bl Func_080e8c9c
	bl Func_080e8d80
	bl BattleFx_PrepareBufferInterpolation
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e9244:
	.4byte IwramMulQ16
.L_080e9248:
	.4byte 0x0005ffff
.L_080e924c:
	.4byte 0xffffd99a

.syntax unified
	.thumb
	.global Func_080d92d4
	.thumb_func
Func_080d92d4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #52
	str r0, [sp, #40]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #156
	ldr r3, [r3]
	adds r0, r1, #0
	adds r6, r2, #0
	mov r8, r3
	bl ObjectTable_Get
	adds r7, r0, #0
	adds r0, r6, #0
	bl ObjectTable_Get
	mov r10, r0
	cmp r7, #0
	bne .L_080d9308
	b .L_080d947c
.L_080d9308:
	cmp r0, #0
	bne .L_080d930e
	b .L_080d947c
.L_080d930e:
	ldr r3, [r0, #8]
	mov r2, r10
	str r3, [sp, #48]
	ldr r4, [sp, #40]
	ldr r0, [r0, #12]
	str r0, [sp, #36]
	ldr r3, [r2, #16]
	str r3, [sp, #44]
	lsls r3, r4, #5
	add r3, r8
	adds r3, #12
	str r3, [sp, #32]
	mov r3, r10
	adds r3, #99
	ldrb r3, [r3]
	cmp r3, #99
	bne .L_080d933e
	mov r5, r10
	add r1, sp, #48
	add r2, sp, #44
	ldr r3, [r5, #104]
	mov r0, r10
	mov lr, r3
	.2byte 0xf800
.L_080d933e:
	ldr r3, [sp, #48]
	ldr r2, [r7, #8]
	ldr r0, [sp, #36]
	subs r3, r3, r2
	asrs r3, r3, #8
	mov r8, r3
	ldr r3, [r7, #12]
	ldr r2, [r7, #16]
	subs r3, r0, r3
	asrs r3, r3, #8
	mov r9, r3
	ldr r3, [sp, #44]
	ldr r6, .L_080d948c
	subs r3, r3, r2
	asrs r3, r3, #8
	mov r1, r8
	mov r0, r8
	mov r11, r3
	mov lr, r6
	.2byte 0xf800
	mov r1, r9
	str r0, [sp, #28]
	mov r0, r9
	mov lr, r6
	.2byte 0xf800
	mov r1, r11
	str r0, [sp, #24]
	mov r0, r11
	mov lr, r6
	.2byte 0xf800
	ldr r3, [sp, #24]
	ldr r2, [sp, #28]
	adds r2, r2, r3
	adds r0, r2, r0
	str r2, [sp, #16]
	str r0, [sp, #20]
	ldr r3, .L_080d9490
	mov lr, r3
	.2byte 0xf800
	movs r1, #6
	bl __divsi3
	adds r5, r0, #1
	ldr r0, [sp, #32]
	movs r4, #4
	ldrsh r6, [r0, r4]
	cmp r5, r6
	ble .L_080d942c
	ldr r3, [sp, #48]
	ldr r2, [r7, #8]
	adds r6, #1
	subs r3, r3, r2
	adds r0, r6, #0
	muls r0, r3
	adds r1, r5, #0
	mov r8, r2
	bl __divsi3
	ldr r3, [sp, #36]
	ldr r4, [r7, #16]
	mov r9, r3
	ldr r3, [sp, #44]
	adds r1, r5, #0
	subs r3, r3, r4
	add r8, r0
	adds r0, r6, #0
	muls r0, r3
	mov r10, r4
	bl __divsi3
	ldr r5, .L_080d9494
	add r0, r10
	mov r11, r0
	ldr r0, [r5]
	bl Trig_Sin
	ldr r2, [r7, #12]
	movs r3, #128
	lsls r3, r3, #12
	mov r10, r3
	lsls r0, r0, #2
	mov r6, r9
	ldr r1, [r7, #8]
	ldr r3, [r7, #16]
	mov r4, r8
	str r0, [sp, #12]
	add r2, r10
	ldr r0, [sp, #40]
	add r6, r10
	mov r5, r11
	str r4, [sp, #0]
	str r6, [sp, #4]
	str r5, [sp, #8]
	bl Func_080d9498
	ldr r0, [sp, #32]
	movs r3, #0
	str r3, [r0, #24]
	movs r0, #1
	bl WaitFrames
	ldr r2, .L_080d9494
	ldr r0, [r2]
	bl Trig_Sin
	ldr r2, [r7, #12]
	lsls r0, r0, #2
	ldr r1, [r7, #8]
	ldr r3, [r7, #16]
	mov r4, r8
	str r0, [sp, #12]
	add r2, r10
	ldr r0, [sp, #40]
	str r4, [sp, #0]
	str r6, [sp, #4]
	str r5, [sp, #8]
	bl Func_080d9790
	b .L_080d947c
.L_080d942c:
	ldr r6, .L_080d9494
	movs r5, #128
	ldr r0, [r6]
	bl Trig_Sin
	lsls r5, r5, #12
	mov r12, r5
	mov r5, r10
	ldr r4, [r5, #8]
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	str r4, [sp, #0]
	lsls r0, r0, #2
	ldr r4, [r5, #12]
	add r2, r12
	add r4, r12
	str r4, [sp, #4]
	ldr r4, [r5, #16]
	str r0, [sp, #12]
	ldr r0, [sp, #40]
	str r4, [sp, #8]
	bl Func_080d9498
	ldr r3, [r6]
	movs r0, #128
	lsls r0, r0, #5
	adds r3, r3, r0
	str r3, [r6]
	movs r0, #6
	bl WaitFrames
	adds r0, r7, #0
	movs r1, #2
	bl Object_SetMode
	mov r0, r10
	movs r1, #2
	bl Object_SetMode
.L_080d947c:
	add sp, #52
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d948c:
	.4byte IwramMulQ16
.L_080d9490:
	.4byte IwramFillWords + 0x74
.L_080d9494:
	.4byte Data_080f393c

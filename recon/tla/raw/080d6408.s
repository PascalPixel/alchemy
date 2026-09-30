.syntax unified
	.thumb
	.global Func_080d6408
	.thumb_func
Func_080d6408:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r1, #0
	movs r1, #242
	adds r5, r0, #0
	lsls r1, r1, #4
	movs r0, #136
	sub sp, #4
	mov r8, r2
	adds r7, r3, #0
	bl Runtime_AllocateBlock
	movs r3, #0
	adds r4, r0, #0
	mov r0, sp
	str r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r1, r4, #0
	ldr r2, .L_080d64ac
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #128
	adds r1, r3, #0
	lsls r2, r2, #24
.L_080d643e:
	ldr r3, [r1, #8]
	ands r3, r2
	cmp r3, #0
	bne .L_080d643e
	movs r2, #240
	lsls r2, r2, #4
	adds r2, #1
	adds r3, r4, r2
	adds r2, #7
	strb r5, [r3]
	adds r3, r4, r2
	str r6, [r3]
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #12
	adds r2, r4, r3
	ldr r3, [sp, #24]
	movs r1, #144
	str r3, [r2]
	movs r2, #240
	lsls r2, r2, #4
	adds r2, #24
	adds r3, r4, r2
	str r7, [r3]
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #28
	adds r2, r4, r3
	ldr r3, [sp, #32]
	lsls r1, r1, #3
	str r3, [r2]
	movs r2, #241
	lsls r2, r2, #4
	adds r3, r4, r2
	mov r2, r8
	str r2, [r3]
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #20
	adds r2, r4, r3
	ldr r3, [sp, #28]
	ldr r0, .L_080d64b0
	str r3, [r2]
	bl Scheduler_AddOrUpdateCallback
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_080d64b4
	bl Scheduler_AddOrUpdateCallback
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d64ac:
	.4byte 0x850003c8
.L_080d64b0:
	.4byte Func_080d607c
.L_080d64b4:
	.4byte Func_080d5ff8

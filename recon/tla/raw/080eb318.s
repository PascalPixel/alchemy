.syntax unified
	.thumb
	.global Func_080eb318
	.thumb_func
Func_080eb318:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	adds r5, r3, #0
	ldr r3, [sp, #40]
	subs r4, r2, r0
	mov r11, r3
	movs r3, #0
	cmp r4, #0
	bge .L_080eb340
	movs r2, #1
	negs r2, r2
	mov r9, r2
	negs r4, r4
	b .L_080eb344
.L_080eb340:
	movs r2, #1
	mov r9, r2
.L_080eb344:
	subs r7, r5, r1
	cmp r7, #0
	bge .L_080eb354
	movs r2, #1
	negs r2, r2
	mov r10, r2
	negs r7, r7
	b .L_080eb358
.L_080eb354:
	movs r2, #1
	mov r10, r2
.L_080eb358:
	adds r6, r0, #0
	adds r5, r1, #0
	cmp r4, r7
	ble .L_080eb390
	movs r2, #0
	mov r8, r2
	cmp r8, r4
	bge .L_080eb3c0
.L_080eb368:
	adds r0, r6, #0
	adds r1, r5, #0
	mov r2, r11
	str r3, [sp, #4]
	str r4, [sp, #0]
	bl Func_080eb2d8
	ldr r3, [sp, #4]
	ldr r4, [sp, #0]
	adds r3, r3, r7
	cmp r3, r4
	blt .L_080eb384
	subs r3, r3, r4
	add r5, r10
.L_080eb384:
	movs r2, #1
	add r8, r2
	add r6, r9
	cmp r8, r4
	blt .L_080eb368
	b .L_080eb3c0
.L_080eb390:
	cmp r7, #0
	ble .L_080eb3c0
	mov r8, r7
.L_080eb396:
	adds r0, r6, #0
	adds r1, r5, #0
	mov r2, r11
	str r3, [sp, #4]
	str r4, [sp, #0]
	bl Func_080eb2d8
	ldr r3, [sp, #4]
	ldr r4, [sp, #0]
	adds r3, r3, r4
	cmp r3, r7
	blt .L_080eb3b2
	subs r3, r3, r7
	add r6, r9
.L_080eb3b2:
	movs r2, #1
	negs r2, r2
	add r8, r2
	mov r2, r8
	add r5, r10
	cmp r2, #0
	bne .L_080eb396
.L_080eb3c0:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000

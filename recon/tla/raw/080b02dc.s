.syntax unified
	.thumb
	.global Func_080b02dc
	.thumb_func
Func_080b02dc:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r5, .L_080b0374
	mov r8, r1
	movs r1, #0
	ldrsh r2, [r5, r1]
	adds r6, r0, #0
	movs r4, #16
	ldrsh r3, [r5, r4]
	movs r0, #5
	cmp r6, r2
	ble .L_080b02fa
	adds r6, r2, #0
	b .L_080b0300
.L_080b02fa:
	cmp r6, r3
	bge .L_080b0300
	adds r6, r3, #0
.L_080b0300:
	movs r1, #0
	movs r4, #0
	cmp r1, r0
	bge .L_080b0330
	movs r7, #0
	ldrsh r3, [r5, r7]
	cmp r6, r3
	bgt .L_080b032a
	mov r12, r5
	movs r2, #0
.L_080b0314:
	adds r1, #1
	adds r2, #4
	cmp r1, r0
	bge .L_080b032e
	adds r4, r2, #0
	mov r3, r12
	ldrsh r3, [r4, r3]
	mov lr, r3
	cmp r6, lr
	ble .L_080b0314
	b .L_080b0330
.L_080b032a:
	movs r4, #0
	b .L_080b0330
.L_080b032e:
	lsls r4, r1, #2
.L_080b0330:
	cmp r1, r0
	bne .L_080b033a
	subs r3, r4, #2
	ldrsh r0, [r5, r3]
	b .L_080b0356
.L_080b033a:
	subs r3, r4, #4
	ldrsh r1, [r5, r3]
	ldrsh r0, [r5, r4]
	subs r3, r4, #2
	ldrsh r2, [r5, r3]
	adds r3, r4, #2
	ldrsh r5, [r5, r3]
	subs r1, r1, r0
	subs r2, r2, r5
	subs r0, r6, r0
	muls r0, r2
	bl Math_Div
	adds r0, r0, r5
.L_080b0356:
	mov r7, r8
	cmp r7, #0
	beq .L_080b0366
	cmp r7, #1
	bne .L_080b0366
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r0, r3, #1
.L_080b0366:
	movs r1, #128
	lsls r1, r1, #1
	adds r0, r0, r1
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080b0374:
	.4byte Data_080c6b04

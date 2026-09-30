.syntax unified
	.thumb
	.global Func_0801451c
	.thumb_func
Func_0801451c:
	push {r5, r6, lr}
	sub sp, #8
	ldr r2, .L_08014568
	movs r4, #23
	b .L_08014528
.L_08014526:
	ldr r2, .L_08014568
.L_08014528:
	adds r1, r2, #0
	cmp r4, #0
	ble .L_0801455c
	adds r0, r4, #0
.L_08014530:
	movs r3, #12
	ldrsh r2, [r1, r3]
	movs r5, #4
	ldrsh r3, [r1, r5]
	cmp r2, r3
	ble .L_08014554
	adds r3, r1, #0
	mov r2, sp
	ldmia r3!, {r5, r6}
	stmia r2!, {r5, r6}
	adds r2, r1, #0
	adds r1, r3, #0
	ldmia r3!, {r5, r6}
	stmia r2!, {r5, r6}
	mov r3, sp
	ldmia r3!, {r5, r6}
	stmia r2!, {r5, r6}
	b .L_08014556
.L_08014554:
	adds r1, #8
.L_08014556:
	subs r0, #1
	cmp r0, #0
	bne .L_08014530
.L_0801455c:
	subs r4, #1
	cmp r4, #1
	bgt .L_08014526
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_08014568:
	.4byte Data_02003610

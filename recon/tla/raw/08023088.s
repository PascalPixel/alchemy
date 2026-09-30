.syntax unified
	.thumb
	.global Func_08023088
	.thumb_func
Func_08023088:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	ldr r4, .L_080230d4
	adds r3, #228
	adds r5, r1, #0
	ldr r1, [r3]
	ldr r2, [r3, #4]
	ldr r3, [r0, #8]
	ands r1, r4
	subs r1, r3, r1
	ldr r3, [r0, #16]
	ldr r0, .L_080230d8
	ands r2, r4
	subs r2, r3, r2
	adds r3, r1, r0
	ldr r0, .L_080230dc
	cmp r3, r0
	bhi .L_080230c8
	cmp r2, #0
	ble .L_080230c8
	movs r3, #224
	lsls r3, r3, #16
	cmp r2, r3
	bge .L_080230c8
	asrs r3, r1, #16
	stmia r5!, {r3}
	asrs r3, r2, #16
	str r3, [r5]
	movs r0, #0
	b .L_080230d2
.L_080230c8:
	movs r3, #0
	stmia r5!, {r3}
	movs r0, #1
	str r3, [r5]
	negs r0, r0
.L_080230d2:
	pop {r5, pc}
.L_080230d4:
	.4byte 0xffff0000
.L_080230d8:
	.4byte 0x001fffff
.L_080230dc:
	.4byte 0x012ffffe

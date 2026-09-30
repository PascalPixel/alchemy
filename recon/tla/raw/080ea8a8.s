.syntax unified
	.thumb
	.global Func_080ea8a8
	.thumb_func
Func_080ea8a8:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	adds r7, r2, #0
	bl Func_08020308 + 0x8
	cmp r0, #0
	bne .L_080ea8d0
	adds r1, r6, #0
	adds r2, r7, #0
	adds r0, r5, #0
	bl Func_080dbde8
	movs r3, #128
	orrs r3, r0
	adds r1, r6, #0
	adds r0, r5, #0
	adds r2, r7, #0
	bl Func_080dbdf4
.L_080ea8d0:
	pop {r5, r6, r7, pc}
	.2byte 0x0000

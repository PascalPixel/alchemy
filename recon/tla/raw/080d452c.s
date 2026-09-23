.syntax unified
	.thumb
	.set sub_08014d00, 0x08014d00
	.set sub_08020150, 0x08020150
	.set sub_080cad84, 0x080cad84
	.set sub_080d2240, 0x080d2240
	.set sub_080d440c, 0x080d440c
	.global Func_080d452c
	.thumb_func
Func_080d452c:
	push	{r5, lr}
	adds	r5, r1, #0
	bl	sub_080cad84
	adds	r3, r0, #0
	cmp	r3, #0
	beq.n	.L_080d4546
	ldr	r0, [r3, #8]
	ldr	r1, [r3, #12]
	ldr	r2, [r3, #16]
	adds	r3, r5, #0
	bl	sub_080d440c
.L_080d4546:
	pop	{r5, pc}
	push	{lr}
	movs	r1, #213
	lsls	r1, r1, #4
	movs	r0, #108
	bl	sub_08014d00
	movs	r3, #230
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldr	r0, [r0, #0]
	bl	sub_08020150
	movs	r0, #2
	bl	sub_080d2240
	pop	{pc}
	push	{lr}
	movs	r1, #213
	lsls	r1, r1, #4
	movs	r0, #108
	bl	sub_08014d00
	movs	r3, #230
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldr	r0, [r0, #0]
	pop	{pc}
	.2byte 0x0000

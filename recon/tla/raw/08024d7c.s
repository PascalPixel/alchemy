.syntax unified
	.thumb
	.set sub_08024cdc, 0x08024cdc
	.global Func_08024d7c
	.thumb_func
Func_08024d7c:
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r2, #4
	ldrsh	r3, [r5, r2]
	ldr	r2, [r5, #0]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #4]
	adds	r3, r5, #0
	adds	r3, #87
	ldrb	r3, [r3, #0]
	ldrh	r0, [r5, #4]
	cmp	r3, #0
	beq.n	.L_08024da2
	adds	r0, r5, #0
	bl	sub_08024cdc
	strh	r0, [r5, #4]
	b.n	.L_08024da6
.L_08024da2:
	adds	r3, r0, #2
	strh	r3, [r5, #4]
.L_08024da6:
	movs	r0, #1
	pop	{r5, pc}
	.align 2, 0

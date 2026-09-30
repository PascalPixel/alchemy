.syntax unified
	.thumb
	.global Func_080b0434
	.thumb_func
Func_080b0434:
	push	{lr}
	bl	Owner_GetState
	movs	r2, #42
	adds	r2, #255
	adds	r3, r0, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080b044c
	bl	Func_080b005c
	b.n	.L_080b045c
.L_080b044c:
	movs	r1, #1
	bl	Inventory_GetEquippedDefinition
	cmp	r0, #0
	beq.n	.L_080b045a
	ldr	r0, [r0, #20]
	b.n	.L_080b045c
.L_080b045a:
	movs	r0, #4
.L_080b045c:
	pop	{pc}
	.2byte 0x0000

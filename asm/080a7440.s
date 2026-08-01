@ Byte-exact retained Thumb owner. The canonical behavioral reconstruction is
@ semantic/main/080a7440.c; C/H assembly escape constructs are forbidden.
.syntax unified
	.thumb
	.global Func_080a7440
	.thumb_func
Func_080a7440:
	push	{r5, lr}
	ldr	r3, .Lpool
	ldr	r5, [r3]
	movs	r2, #0
	movs	r1, #186
	lsls	r1, r1, #1
	adds	r3, r5, r1
	strh	r2, [r3]
	movs	r0, #0
	bl	Func_080a77a4
	movs	r3, #1
	negs	r3, r3
	adds	r2, r0, #0
	cmp	r0, r3
	beq	.Lreturn
	ldr	r1, .Lpool+4
	adds	r3, r5, r1
	ldrb	r2, [r3]
.Lreturn:
	adds	r0, r2, #0
	pop	{r5}
	pop	{r1}
	bx	r1
	.align	2, 0
.Lpool:
	.word	0x03001f2c
	.word	0x0000021a

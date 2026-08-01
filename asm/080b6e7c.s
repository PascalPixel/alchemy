@ Byte-exact retained Thumb owner. The canonical behavioral reconstruction is
@ semantic/main/080b6e7c.c; C/H assembly escape constructs are forbidden.
.syntax unified
	.thumb
	.global Func_080b6e7c
	.thumb_func
Func_080b6e7c:
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r4, #0
	ldr	r0, .Lpool
.Lloop:
	lsls	r1, r4, #1
	ldrh	r2, [r0, r1]
	ldr	r3, .Lpool+4
	ands	r3, r2
	cmp	r5, r3
	bne	.Lnext
	ldrh	r3, [r0, r1]
	lsrs	r0, r3, #9
	b	.Lreturn
.Lnext:
	lsls	r3, r2, #16
	movs	r2, #1
	asrs	r3, r3, #16
	negs	r2, r2
	adds	r4, #1
	cmp	r3, r2
	bne	.Lloop
	movs	r0, #6
.Lreturn:
	pop	{r5}
	pop	{r1}
	bx	r1
	.align	2, 0
.Lpool:
	.word	Data_080c593c
	.word	511

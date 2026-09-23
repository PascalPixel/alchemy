.syntax unified
	.thumb
	.set sub_080072f0, 0x080072f0
	.set sub_08097a10, 0x08097a10
	.global Func_080979a4
	.thumb_func
Func_080979a4:
	push	{r5, r6, lr}
	adds	r6, r1, #0
	movs	r1, #180
	lsls	r1, r1, #17
	adds	r5, r2, #0
	bl	sub_08097a10
	ldr	r2, [pc, #72]
	adds	r4, r0, #0
	cmp	r4, r2
	bgt.n	.L_080979c2
	ldr	r3, [pc, #68]
	adds	r0, r5, #0
	adds	r1, r4, #0
	b.n	.L_080979e0
.L_080979c2:
	ldr	r1, [pc, #64]
	adds	r3, r4, r1
	movs	r1, #240
	lsls	r1, r1, #15
	adds	r0, r5, #0
	cmp	r3, r1
	bcc.n	.L_080979f4
	ldr	r1, [pc, #52]
	adds	r3, r4, r1
	cmp	r3, r2
	bhi.n	.L_080979f2
	movs	r1, #240
	lsls	r1, r1, #16
	ldr	r3, [pc, #32]
	subs	r1, r1, r4
.L_080979e0:
	mov	ip, pc
	bx	r3
	adds	r1, r0, #0
	movs	r0, #240
	ldr	r3, [pc, #32]
	lsls	r0, r0, #14
	bl	sub_080072f0
	b.n	.L_080979f4
.L_080979f2:
	adds	r0, r6, #0
.L_080979f4:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x003bffff
	.4byte 0x03000118
	.4byte 0xffc40000
	.4byte 0xff4c0000
	.4byte 0x0300013c

.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.global Func_08127ba0
	.thumb_func
Func_08127ba0:
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #36]
	adds	r3, r6, #0
	adds	r3, #64
	ldrb	r5, [r3, #0]
	bl	sub_08016ca4
	movs	r3, #42
	mov	ip, r0
	adds	r3, #255
	add	r3, ip
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_08127c2a
	movs	r3, #165
	lsls	r3, r3, #1
	add	r3, ip
	movs	r1, #0
	ldrh	r0, [r3, #0]
	cmp	r1, r5
	bge.n	.L_08127be6
	ldrh	r3, [r6, #16]
	cmp	r3, r0
	beq.n	.L_08127be6
	adds	r2, r6, #0
	adds	r2, #16
.L_08127bd8:
	adds	r1, #1
	cmp	r1, r5
	bge.n	.L_08127be6
	adds	r2, #2
	ldrh	r3, [r2, #0]
	cmp	r3, r0
	bne.n	.L_08127bd8
.L_08127be6:
	cmp	r1, r5
	beq.n	.L_08127c2a
	lsls	r1, r1, #2
	adds	r3, r1, #0
	adds	r3, #28
	ldr	r3, [r6, r3]
	cmp	r3, #0
	beq.n	.L_08127c2a
	mov	r2, ip
	ldrb	r3, [r2, #0]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L_08127c0e
.L_08127c00:
	adds	r4, #1
	cmp	r4, #13
	bgt.n	.L_08127c0e
	adds	r2, #1
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_08127c00
.L_08127c0e:
	movs	r0, #32
	cmp	r4, #0
	ble.n	.L_08127c1e
	subs	r3, r4, #1
	mov	r2, ip
	ldrb	r3, [r2, r3]
	adds	r0, r3, #0
	subs	r0, #49
.L_08127c1e:
	adds	r1, #28
	ldr	r3, [r6, r1]
	movs	r2, #1
	lsls	r2, r0
	bics	r3, r2
	str	r3, [r6, r1]
.L_08127c2a:
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	movs	r5, #0
	movs	r7, #49
.L_08127c34:
	adds	r0, r5, #0
	adds	r0, #128
	bl	sub_08016ca4
	adds	r2, r0, #0
	movs	r0, #149
	lsls	r0, r0, #1
	adds	r3, r2, r0
	ldrb	r1, [r3, #0]
	cmp	r1, #1
	bne.n	.L_08127c76
	adds	r0, #32
	adds	r3, r2, r0
	ldrh	r3, [r3, #0]
	cmp	r3, r6
	bne.n	.L_08127c76
	ldrb	r3, [r2, #0]
	movs	r0, #0
	cmp	r3, #0
	bne.n	.L_08127c62
	strb	r7, [r2, #0]
	strb	r0, [r2, r1]
	b.n	.L_08127c7c
.L_08127c62:
	adds	r0, #1
	cmp	r0, #13
	bgt.n	.L_08127c7c
	ldrb	r1, [r2, r0]
	cmp	r1, #0
	bne.n	.L_08127c62
	adds	r3, r0, #1
	strb	r7, [r2, r0]
	strb	r1, [r2, r3]
	b.n	.L_08127c7c
.L_08127c76:
	adds	r5, #1
	cmp	r5, #5
	ble.n	.L_08127c34
.L_08127c7c:
	pop	{r5, r6, r7, pc}
	.align 2, 0

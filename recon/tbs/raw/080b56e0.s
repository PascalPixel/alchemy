.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_0800403c, 0x0800403c
	.set sub_080040e8, 0x080040e8
	.set sub_08004760, 0x08004760
	.set sub_0800479c, 0x0800479c
	.set sub_08004858, 0x08004858
	.set sub_08077010, 0x08077010
	.set sub_08077098, 0x08077098
	.set sub_080770c8, 0x080770c8
	.set sub_080770d0, 0x080770d0
	.set sub_080b5368, 0x080b5368
	.set sub_080b5534, 0x080b5534
	.set sub_080b63c8, 0x080b63c8
	.set sub_080c2a08, 0x080c2a08
	.global Unnamed_080b56e0
	.global Func_080b56e0
	.thumb_func
Unnamed_080b56e0:
Func_080b56e0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r2, #0
	mov	sl, r2
	bl	sub_08077098
.L_080b56f6:
	movs	r5, #181
	lsls	r5, r5, #1
	bl	sub_0800479c
	bl	sub_08004760
	bl	sub_080040e8
	bl	sub_08004858
	bl	sub_0800403c
	adds	r0, r5, #0
	bl	sub_080770c8
	ldr	r3, [pc, #312]
	ldr	r3, [r3, #0]
	movs	r2, #128
	ands	r3, r2
	ldr	r6, [pc, #308]
	cmp	r3, #0
	bne.n	.L_080b5724
	b.n	.L_080b583e
.L_080b5724:
	movs	r3, #1
	negs	r3, r3
	adds	r0, r5, #0
	mov	r8, r3
	bl	sub_080770d0
	ldr	r2, [pc, #292]
	movs	r3, #85
	mov	r9, r2
	negs	r3, r3
	add	r3, r9
	ldr	r5, [pc, #288]
	movs	r7, #0
	mov	fp, r3
.L_080b5740:
	movs	r0, #32
	bl	sub_080770d0
	movs	r0, #1
	bl	sub_080030f8
	b.n	.L_080b579a
.L_080b574e:
	ldr	r3, [r5, #0]
	movs	r2, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b575c
	bl	sub_080b5534
.L_080b575c:
	ldr	r3, [r5, #0]
	movs	r2, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b576a
	bl	sub_080c2a08
.L_080b576a:
	ldr	r3, [r5, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080b577a
	mov	r2, sl
	cmp	r2, #0
	beq.n	.L_080b5784
.L_080b577a:
	movs	r3, #1
	mov	sl, r3
	movs	r3, #5
	mov	r2, r9
	strb	r3, [r2, #0]
.L_080b5784:
	cmp	r7, r8
	beq.n	.L_080b5794
	bl	sub_08077098
	adds	r0, r7, #0
	bl	sub_080b5368
	mov	r8, r7
.L_080b5794:
	movs	r0, #1
	bl	sub_080030f8
.L_080b579a:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b57a6
	adds	r6, #1
.L_080b57a6:
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b57b2
	subs	r6, #1
.L_080b57b2:
	ldr	r3, [r5, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b57be
	subs	r6, #10
.L_080b57be:
	ldr	r3, [r5, #0]
	movs	r1, #128
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080b57ca
	adds	r6, #10
.L_080b57ca:
	ldr	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b57d8
	adds	r7, #1
.L_080b57d8:
	ldr	r3, [r5, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b57e6
	subs	r7, #1
.L_080b57e6:
	ldr	r3, [r5, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b574e
	ldr	r3, [pc, #92]
	ldr	r3, [r3, #0]
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080b5802
	movs	r0, #182
	lsls	r0, r0, #1
	bl	sub_080770c8
.L_080b5802:
	movs	r0, #0
	bl	sub_08077010
	ldr	r3, [pc, #84]
	mov	r2, fp
	strh	r3, [r2, #0]
	cmp	r6, #28
	bne.n	.L_080b581a
	movs	r0, #183
	lsls	r0, r0, #1
	bl	sub_080770c8
.L_080b581a:
	movs	r0, #177
	lsls	r0, r0, #1
	bl	sub_080770c8
	adds	r0, r6, #0
	bl	sub_080b63c8
	bl	sub_0800479c
	bl	sub_08004760
	bl	sub_080040e8
	bl	sub_08004858
	bl	sub_0800403c
	b.n	.L_080b5740
.L_080b583e:
	movs	r0, #177
	lsls	r0, r0, #1
	bl	sub_080770c8
	ldr	r0, [pc, #12]
	bl	sub_080b63c8
	b.n	.L_080b56f6
	movs	r0, r0
	.4byte 0x03001ae8
	.4byte 0x00000101
	.4byte 0x0200046b
	.4byte 0x03001b04
	.4byte 0x0000001d

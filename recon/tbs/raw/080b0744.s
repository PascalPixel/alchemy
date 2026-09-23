.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_08002dd8, 0x08002dd8
	.set sub_08003fa4, 0x08003fa4
	.set sub_08004080, 0x08004080
	.set sub_080048f4, 0x080048f4
	.set sub_080150c8, 0x080150c8
	.set sub_080b06ec, 0x080b06ec
	.global Func_080b0744
	.thumb_func
Func_080b0744:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r8, r1
	movs	r1, #128
	adds	r5, r0, #0
	lsls	r1, r1, #3
	movs	r0, #14
	mov	sl, r2
	adds	r7, r3, #0
	sub	sp, #4
	bl	sub_080048f4
	movs	r3, #0
	adds	r6, r0, #0
	mov	r9, r3
	ldr	r0, [pc, #196]
	ldr	r3, [pc, #200]
	adds	r1, r6, #0
	ldr	r2, [pc, #200]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r5, #0
	movs	r1, #10
	bl	sub_080022fc
	adds	r1, r6, #0
	movs	r2, #0
	bl	sub_080b06ec
	adds	r0, r5, #0
	movs	r1, #10
	bl	sub_080022ec
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080b07f2
	movs	r1, #10
	bl	sub_080022fc
	adds	r1, r6, #0
	movs	r2, #1
	bl	sub_080b06ec
	adds	r0, r5, #0
	movs	r1, #10
	bl	sub_080022ec
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080b07f2
	movs	r1, #10
	bl	sub_080022fc
	adds	r1, r6, #0
	movs	r2, #2
	bl	sub_080b06ec
	adds	r0, r5, #0
	movs	r1, #10
	bl	sub_080022ec
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080b07f2
	movs	r1, #10
	bl	sub_080022fc
	adds	r1, r6, #0
	movs	r2, #3
	bl	sub_080b06ec
	adds	r0, r5, #0
	movs	r1, #10
	bl	sub_080022ec
	cmp	r0, #0
	beq.n	.L_080b07f2
	movs	r1, #10
	bl	sub_080022fc
	adds	r1, r6, #0
	movs	r2, #4
	bl	sub_080b06ec
.L_080b07f2:
	bl	sub_08004080
	adds	r5, r0, #0
	cmp	r5, #96
	beq.n	.L_080b0816
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r2, r6, #0
	bl	sub_08003fa4
	ldr	r1, [pc, #52]
	adds	r0, r5, #0
	mov	r2, r8
	mov	r3, sl
	str	r7, [sp, #0]
	bl	sub_080150c8
	mov	r9, r0
.L_080b0816:
	movs	r0, #14
	bl	sub_08002dd8
	mov	r0, r9
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x080b3e80
	.4byte 0x040000d4
	.4byte 0x84000040
	.4byte 0x80008000

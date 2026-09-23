.syntax unified
	.thumb
	.set sub_08015778, 0x08015778
	.set sub_080eb01c, 0x080eb01c
	.global Func_080db9cc
	.thumb_func
Func_080db9cc:
	push	{r5, lr}
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r3, [r2, #108]
	movs	r1, #197
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	sub	sp, #24
	adds	r4, r0, #0
	cmp	r3, #3
	bne.n	.L_080dba18
	ldr	r3, [r4, #8]
	add	r0, sp, #12
	str	r3, [r0, #0]
	mov	r5, sp
	ldr	r3, [r4, #12]
	adds	r1, r5, #0
	str	r3, [r0, #4]
	ldr	r3, [r4, #16]
	str	r3, [r0, #8]
	bl	sub_08015778
	ldr	r2, [r5, #8]
	movs	r3, #128
	lsls	r3, r3, #2
	subs	r3, r3, r2
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r0, r3, #0
	adds	r0, #128
	cmp	r0, #0
	bgt.n	.L_080dba3a
	movs	r0, #1
	b.n	.L_080dba3a
.L_080dba18:
	ldr	r2, [r2, #32]
	adds	r3, r2, #0
	adds	r3, #228
	ldr	r1, [r3, #4]
	ldr	r3, [pc, #28]
	ands	r1, r3
	ldr	r3, [r2, #0]
	ldr	r2, [r4, #16]
	ldr	r0, [r3, #4]
	ldr	r3, [r4, #12]
	subs	r3, r3, r0
	adds	r3, r3, r2
	subs	r3, r3, r0
	subs	r3, r3, r1
	asrs	r3, r3, #16
	adds	r0, r3, #0
	adds	r0, #62
.L_080dba3a:
	add	sp, #24
	pop	{r5, pc}
	movs	r0, r0
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb500
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r0, [r3, #0]
	movs	r3, #249
	lsls	r3, r3, #3
	adds	r0, r0, r3
	bl	sub_080eb01c
	pop	{pc}
	.align 2, 0

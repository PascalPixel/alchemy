.syntax unified
	.thumb
	.set sub_0811be3c, 0x0811be3c
	.set sub_0811c210, 0x0811c210
	.global Func_0811c214
	.thumb_func
Func_0811c214:
	push	{r5, r6, lr}
	adds	r5, r1, #0
	sub	sp, #12
	bl	sub_0811be3c
	ldr	r6, [r0, #0]
	adds	r0, r5, #0
	bl	sub_0811be3c
	ldr	r2, [r6, #56]
	movs	r4, #128
	lsls	r4, r4, #24
	ldr	r0, [r0, #0]
	cmp	r2, r4
	bne.n	.L_0811c234
	ldr	r2, [r6, #8]
.L_0811c234:
	ldr	r5, [r6, #64]
	cmp	r5, r4
	bne.n	.L_0811c23c
	ldr	r5, [r6, #16]
.L_0811c23c:
	ldr	r3, [r0, #56]
	cmp	r3, r4
	bne.n	.L_0811c244
	ldr	r3, [r0, #8]
.L_0811c244:
	ldr	r1, [r0, #64]
	cmp	r1, r4
	bne.n	.L_0811c24c
	ldr	r1, [r0, #16]
.L_0811c24c:
	adds	r3, r2, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	mov	r0, sp
	asrs	r3, r3, #1
	str	r3, [r0, #0]
	movs	r3, #0
	str	r3, [r0, #4]
	adds	r3, r5, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	movs	r1, #128
	str	r3, [r0, #8]
	lsls	r1, r1, #5
	bl	sub_0811c210
	add	sp, #12
	pop	{r5, r6, pc}
	.2byte 0x0000

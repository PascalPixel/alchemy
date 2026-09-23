.syntax unified
	.thumb
	.set sub_08017054, 0x08017054
	.set sub_0801788c, 0x0801788c
	.global Func_08016e0c
	.thumb_func
Func_08016e0c:
	push	{r4, r5, lr}
	sub	sp, #68
	mov	r0, sp
	bl	sub_08017054
	mov	r5, sp
	adds	r5, #1
	ldr	r2, [pc, #24]
	movs	r0, #170
	strb	r0, [r2, #0]
	ldr	r1, [pc, #20]
	movs	r0, #85
	strb	r0, [r1, #0]
	movs	r0, #144
	strb	r0, [r2, #0]
	add	r1, sp, #64
	ldr	r2, [pc, #12]
	adds	r0, r2, #0
	b.n	.L_08016e44
	movs	r0, r0
	.4byte 0x0e005555
	.4byte 0x0e002aaa
	.2byte 0x4e20
	.2byte 0x0000
.L_08016e40:
	ldrh	r0, [r1, #0]
	subs	r0, #1
.L_08016e44:
	strh	r0, [r1, #0]
	ldrh	r0, [r1, #0]
	cmp	r0, #0
	bne.n	.L_08016e40
	ldr	r0, [pc, #44]
	bl	sub_0801788c
	lsls	r0, r0, #24
	lsrs	r4, r0, #16
	movs	r0, #224
	lsls	r0, r0, #20
	bl	sub_0801788c
	lsls	r0, r0, #24
	lsrs	r0, r0, #24
	orrs	r4, r0
	ldr	r2, [pc, #24]
	movs	r0, #170
	strb	r0, [r2, #0]
	ldr	r1, [pc, #24]
	movs	r0, #85
	strb	r0, [r1, #0]
	movs	r0, #240
	strb	r0, [r2, #0]
	add	r1, sp, #64
	ldr	r2, [pc, #16]
	adds	r0, r2, #0
	b.n	.L_08016e90
	.4byte 0x0e000001
	.4byte 0x0e005555
	.4byte 0x0e002aaa
	.2byte 0x4e20
	.2byte 0x0000
.L_08016e8c:
	ldrh	r0, [r1, #0]
	subs	r0, #1
.L_08016e90:
	strh	r0, [r1, #0]
	ldrh	r0, [r1, #0]
	cmp	r0, #0
	bne.n	.L_08016e8c
	adds	r0, r4, #0
	add	sp, #68
	pop	{r4, r5}
	pop	{r1}
	bx	r1
	.align 2, 0

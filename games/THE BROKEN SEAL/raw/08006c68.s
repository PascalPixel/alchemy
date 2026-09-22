.syntax unified
	.thumb
	.set sub_08006ac0, 0x08006ac0
	.set sub_080072f0, 0x080072f0
	.global Overlay_08006c68
Overlay_08006c68:
	push	{r4, r5, lr}
	sub	sp, #256
	adds	r5, r1, #0
	lsls	r0, r0, #16
	lsrs	r4, r0, #16
	ldr	r2, [pc, #32]
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #32]
	ands	r0, r1
	movs	r1, #3
	orrs	r0, r1
	strh	r0, [r2, #0]
	ldr	r3, [pc, #24]
	movs	r0, #1
	eors	r3, r0
	mov	r2, sp
	ldr	r0, [pc, #20]
	ldr	r1, [pc, #16]
	subs	r0, r0, r1
	lsls	r0, r0, #15
	b.n	.L_08006cb0
	movs	r0, r0
	.4byte 0x04000204
	.4byte 0x0000fffc
	.4byte 0x08006c25
	.2byte 0x6c69
	.2byte 0x0800
.L_08006ca4:
	ldrh	r0, [r3, #0]
	strh	r0, [r2, #0]
	adds	r3, #2
	adds	r2, #2
	subs	r0, r1, #1
	lsls	r0, r0, #16
.L_08006cb0:
	lsrs	r1, r0, #16
	cmp	r1, #0
	bne.n	.L_08006ca4
	mov	r3, sp
	adds	r3, #1
	ldr	r0, [pc, #28]
	ldrb	r1, [r0, #28]
	lsls	r4, r1
	adds	r1, r4, #0
	movs	r2, #224
	lsls	r2, r2, #20
	adds	r1, r1, r2
	ldrh	r2, [r0, #24]
	adds	r0, r5, #0
	bl	sub_080072f0
	add	sp, #256
	pop	{r4, r5}
	pop	{r1}
	bx	r1
	.2byte 0x7abc
	.2byte 0x0800
	push	{r4, r5, r6, lr}
	sub	sp, #64
	ldr	r5, [pc, #84]
	ldrh	r1, [r5, #0]
	ldr	r6, [pc, #84]
	ands	r1, r6
	ldr	r0, [pc, #84]
	ldr	r0, [r0, #0]
	ldrh	r0, [r0, #16]
	orrs	r0, r1
	strh	r0, [r5, #0]
	ldr	r1, [pc, #80]
	movs	r4, #170
	strb	r4, [r1, #0]
	ldr	r3, [pc, #76]
	movs	r2, #85
	strb	r2, [r3, #0]
	movs	r0, #128
	strb	r0, [r1, #0]
	strb	r4, [r1, #0]
	strb	r2, [r3, #0]
	movs	r0, #16
	strb	r0, [r1, #0]
	mov	r0, sp
	bl	sub_08006ac0
	ldr	r0, [pc, #56]
	movs	r1, #224
	lsls	r1, r1, #20
	ldr	r3, [r0, #0]
	movs	r0, #3
	movs	r2, #255
	bl	sub_080072f0
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	ldrh	r1, [r5, #0]
	ands	r1, r6
	movs	r2, #3
	orrs	r1, r2
	strh	r1, [r5, #0]
	add	sp, #64
	pop	{r4, r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x04000204
	.4byte 0x0000fffc
	.4byte 0x02004c08
	.4byte 0x0e005555
	.4byte 0x0e002aaa
	.2byte 0x4c00
	.2byte 0x0200
	push	{r4, r5, r6, lr}
	mov	r6, r8
	push	{r6}
	sub	sp, #64
	lsls	r0, r0, #16
	lsrs	r3, r0, #16
	cmp	r3, #15
	bhi.n	.L_08006dd8
	ldr	r6, [pc, #92]
	ldrh	r1, [r6, #0]
	ldr	r0, [pc, #92]
	mov	r8, r0
	ands	r1, r0
	ldr	r0, [pc, #92]
	ldr	r2, [r0, #0]
	ldrh	r0, [r2, #16]
	orrs	r0, r1
	strh	r0, [r6, #0]
	ldrb	r4, [r2, #8]
	lsls	r3, r4
	adds	r4, r3, #0
	movs	r2, #224
	lsls	r2, r2, #20
	adds	r4, r4, r2
	ldr	r1, [pc, #72]
	movs	r5, #170
	strb	r5, [r1, #0]
	ldr	r3, [pc, #72]
	movs	r2, #85
	strb	r2, [r3, #0]
	movs	r0, #128
	strb	r0, [r1, #0]
	strb	r5, [r1, #0]
	strb	r2, [r3, #0]
	movs	r0, #48
	strb	r0, [r4, #0]
	mov	r0, sp
	bl	sub_08006ac0
	ldr	r0, [pc, #52]
	ldr	r3, [r0, #0]
	movs	r0, #2
	adds	r1, r4, #0
	movs	r2, #255
	bl	sub_080072f0
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	ldrh	r1, [r6, #0]
	mov	r2, r8
	ands	r1, r2
	movs	r2, #3
	orrs	r1, r2
	strh	r1, [r6, #0]
	b.n	.L_08006dda
	movs	r0, r0
	.4byte 0x04000204
	.4byte 0x0000fffc
	.4byte 0x02004c08
	.4byte 0x0e005555
	.4byte 0x0e002aaa
	.2byte 0x4c00
	.2byte 0x0200
.L_08006dd8:
	ldr	r0, [pc, #12]
.L_08006dda:
	add	sp, #64
	pop	{r3}
	mov	r8, r3
	pop	{r4, r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x000080ff

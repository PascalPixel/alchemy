.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_080901c0, 0x080901c0
	.set sub_080f9010, 0x080f9010
	.global Func_080941e0
	.thumb_func
Func_080941e0:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #220]
	movs	r1, #247
	ldr	r6, [r3, #0]
	ldr	r3, [pc, #216]
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	sub	sp, #8
	bl	sub_080f9010
	movs	r0, #144
	lsls	r0, r0, #1
	bl	sub_080f9010
	movs	r0, #147
	bl	sub_080f9010
	movs	r1, #207
	lsls	r1, r1, #1
	adds	r3, r6, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	bne.n	.L_08094266
	ldr	r2, [pc, #176]
	ldr	r3, [pc, #180]
	strh	r2, [r3, #0]
	ldr	r0, [pc, #180]
	movs	r1, #16
	bl	sub_080901c0
	movs	r3, #227
	lsls	r3, r3, #1
	adds	r2, r6, r3
	movs	r3, #0
	strh	r3, [r2, #0]
	movs	r0, #16
	bl	sub_080030f8
	movs	r7, #240
	movs	r1, #240
	movs	r5, #0
	lsls	r1, r1, #2
	lsls	r7, r7, #7
	movs	r6, #30
.L_08094242:
	adds	r3, r7, #0
	orrs	r3, r1
	ldr	r2, [pc, #136]
	orrs	r3, r6
	strh	r3, [r2, #0]
	movs	r0, #1
	str	r1, [sp, #4]
	bl	sub_080030f8
	ldr	r2, [pc, #128]
	ldr	r1, [sp, #4]
	adds	r5, #1
	subs	r1, #64
	adds	r7, r7, r2
	subs	r6, #2
	cmp	r5, #15
	ble.n	.L_08094242
	b.n	.L_080942b6
.L_08094266:
	ldr	r3, [pc, #100]
	movs	r5, #160
	lsls	r5, r5, #19
	strh	r3, [r5, #0]
	ldr	r0, [pc, #108]
	movs	r1, #16
	bl	sub_080901c0
	movs	r3, #227
	lsls	r3, r3, #1
	adds	r2, r6, r3
	movs	r3, #0
	strh	r3, [r2, #0]
	movs	r0, #16
	bl	sub_080030f8
	movs	r7, #240
	movs	r2, #240
	mov	r8, r5
	lsls	r2, r2, #2
	lsls	r7, r7, #7
	movs	r6, #30
	movs	r5, #15
.L_08094294:
	adds	r3, r7, #0
	orrs	r3, r2
	orrs	r3, r6
	mov	r1, r8
	strh	r3, [r1, #0]
	movs	r0, #1
	str	r2, [sp, #0]
	bl	sub_080030f8
	ldr	r3, [pc, #48]
	ldr	r2, [sp, #0]
	subs	r5, #1
	subs	r2, #64
	adds	r7, r7, r3
	subs	r6, #2
	cmp	r5, #0
	bge.n	.L_08094294
.L_080942b6:
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00007fff
	.4byte 0x050001e6
	.4byte 0x00000401
	.4byte 0xfffff800
	.4byte 0x00000207

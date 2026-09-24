.syntax unified
	.thumb
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002df0, 0x08002df0
	.set sub_080030f8, 0x080030f8
	.set sub_080048b0, 0x080048b0
	.set sub_08004970, 0x08004970
	.set sub_080072f0, 0x080072f0
	.set sub_08007310, 0x08007310
	.set sub_08015010, 0x08015010
	.set sub_08015278, 0x08015278
	.set sub_080152a8, 0x080152a8
	.set sub_080153e0, 0x080153e0
	.set sub_08015408, 0x08015408
	.set sub_08015410, 0x08015410
	.set sub_08015418, 0x08015418
	.set sub_08077158, 0x08077158
	.set sub_0808a548, 0x0808a548
	.set sub_080a1050, 0x080a1050
	.set sub_080a1070, 0x080a1070
	.set sub_080a1090, 0x080a1090
	.set sub_080a2144, 0x080a2144
	.set sub_080a2680, 0x080a2680
	.set sub_080a2474, 0x080a2474
	.set sub_080a2490, 0x080a2490
	.set sub_080a3354, 0x080a3354
	.set sub_080a34c0, 0x080a34c0
	.set sub_080a5534, 0x080a5534
	.set sub_080ae8dc, 0x080ae8dc
	.global RunAssetSelectionScreen
	.global Func_080a24d0
	.thumb_func
RunAssetSelectionScreen:
Func_080a24d0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #128
	lsls	r1, r1, #6
	mov	r9, r1
	mov	r0, r9
	sub	sp, #16
	bl	sub_08004970
	movs	r1, #167
	adds	r7, r0, #0
	lsls	r1, r1, #4
	movs	r0, #55
	bl	sub_080048b0
	ldr	r2, [pc, #280]
	mov	r8, r2
	ldr	r2, [r2, #0]
	movs	r3, #1
	movs	r1, #0
	adds	r5, r0, #0
	strh	r3, [r2, #4]
	movs	r0, #0
	movs	r2, #30
	movs	r3, #20
	bl	sub_08015408
	movs	r0, #1
	bl	sub_080030f8
	movs	r0, #0
	bl	sub_080a1090
	movs	r3, #130
	lsls	r3, r3, #2
	adds	r0, r5, r3
	bl	sub_08077158
	ldr	r1, [pc, #240]
	adds	r3, r5, r1
	movs	r2, #0
	movs	r1, #3
	strb	r0, [r3, #0]
	movs	r3, #7
	movs	r0, #0
	bl	sub_080a3354
	bl	sub_080a5534
	movs	r0, #14
	bl	sub_080a2144
	ldr	r0, [pc, #216]
	bl	sub_08015418
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #17
	movs	r3, #3
	movs	r0, #13
	bl	sub_08015010
	movs	r2, #134
	lsls	r2, r2, #1
	adds	r3, r5, r2
	str	r0, [r3, #0]
	bl	sub_080a1070
	ldr	r3, [pc, #184]
	ldr	r1, [pc, #188]
	mov	fp, r3
	mov	r2, r9
	adds	r0, r7, #0
	bl	sub_08007310
	ldr	r3, [pc, #180]
	mov	r1, r9
	ldr	r2, [pc, #180]
	ldr	r0, [pc, #168]
	bl	sub_080072f0
	movs	r0, #1
	bl	sub_080153e0
	bl	sub_080a2474
	add	r1, sp, #8
	add	r0, sp, #12
	add	r2, sp, #4
	bl	sub_080a2680
	mov	sl, r0
	bl	sub_080a2490
	mov	r1, sl
	cmp	r1, #1
	bne.n	.L_080a25c2
	mov	r2, r8
	ldr	r0, [r2, #84]
	ldr	r1, [sp, #12]
	ldr	r3, [sp, #4]
	ldr	r2, [pc, #136]
	lsls	r1, r1, #10
	ands	r3, r2
	subs	r2, #127
	orrs	r1, r3
	adds	r3, r0, r2
	strh	r1, [r3, #0]
	movs	r1, #186
	lsls	r1, r1, #1
	adds	r3, r5, r1
	ldrh	r3, [r3, #0]
	adds	r1, #38
	adds	r2, r0, r1
	strh	r3, [r2, #0]
.L_080a25c2:
	mov	r6, r8
	ldr	r0, [r5, #36]
	adds	r6, #36
	bl	sub_08015278
	ldr	r5, [pc, #100]
	ldr	r2, [r6, #0]
	ldr	r3, [pc, #60]
	strb	r3, [r2, r5]
	bl	sub_080a34c0
	movs	r1, #0
	movs	r2, #30
	movs	r3, #20
	movs	r0, #0
	bl	sub_08015408
	bl	sub_080ae8dc
	movs	r0, #55
	bl	sub_08002dd8
	mov	r3, r8
	ldr	r2, [r3, #0]
	movs	r3, #0
	strh	r3, [r2, #4]
	bl	sub_080152a8
	movs	r0, #0
	bl	sub_080153e0
	mov	r2, r9
	adds	r1, r7, #0
	ldr	r0, [pc, #28]
	bl	sub_08007310
	ldr	r3, [r6, #0]
	b.n	.L_080a2638
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x03001e68
	.4byte 0x00000219
	.4byte 0x06002500
	.4byte 0x03001388
	.4byte 0x06004000
	.4byte 0x03000168
	.4byte 0x33333333
	.4byte 0x000001ff
	.2byte 0x0ea6
	.2byte 0x0000
.L_080a2638:
	movs	r1, #0
	adds	r3, r3, r5
	strb	r1, [r3, #0]
	adds	r0, r7, #0
	bl	sub_08002df0
	movs	r0, #1
	bl	sub_080030f8
	bl	sub_080a1050
	movs	r0, #1
	bl	sub_080030f8
	movs	r0, #0
	movs	r1, #0
	movs	r2, #30
	movs	r3, #20
	bl	sub_08015410
	ldr	r3, [r6, #0]
	movs	r2, #0
	adds	r3, r3, r5
	strb	r2, [r3, #0]
	bl	sub_0808a548
	mov	r0, sl
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1

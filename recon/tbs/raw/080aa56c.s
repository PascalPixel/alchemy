.syntax unified
	.thumb
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002df0, 0x08002df0
	.set sub_080030f8, 0x080030f8
	.set sub_080048b0, 0x080048b0
	.set sub_08004970, 0x08004970
	.set sub_080072f8, 0x080072f8
	.set sub_08015010, 0x08015010
	.set sub_080152a8, 0x080152a8
	.set sub_080153e0, 0x080153e0
	.set sub_08015408, 0x08015408
	.set sub_08015410, 0x08015410
	.set sub_08015418, 0x08015418
	.set sub_080770c0, 0x080770c0
	.set sub_08077158, 0x08077158
	.set sub_0808a548, 0x0808a548
	.set sub_080a1050, 0x080a1050
	.set sub_080a1070, 0x080a1070
	.set sub_080a1090, 0x080a1090
	.set sub_080a2144, 0x080a2144
	.set sub_080a3354, 0x080a3354
	.set sub_080a34c0, 0x080a34c0
	.set sub_080aa544, 0x080aa544
	.set sub_080aa768, 0x080aa768
	.set sub_080ad508, 0x080ad508
	.set sub_080ad658, 0x080ad658
	.set sub_080ae88c, 0x080ae88c
	.set sub_080ae8dc, 0x080ae8dc
	.global Func_080aa56c
	.thumb_func
Func_080aa56c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r1, #167
	lsls	r1, r1, #4
	movs	r0, #55
	sub	sp, #4
	bl	sub_080048b0
	ldr	r3, [pc, #136]
	movs	r2, #131
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldrb	r2, [r3, #0]
	mov	r9, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	ldr	r3, [pc, #124]
	movs	r2, #1
	ldr	r3, [r3, #0]
	mov	sl, r2
	mov	r2, sl
	movs	r1, #0
	strh	r2, [r3, #4]
	adds	r7, r0, #0
	movs	r2, #30
	movs	r3, #20
	movs	r0, #0
	bl	sub_08015408
	movs	r0, #1
	bl	sub_080030f8
	movs	r0, #0
	bl	sub_080a1090
	ldr	r0, [pc, #88]
	bl	sub_08004970
	movs	r2, #194
	lsls	r2, r2, #1
	adds	r3, r7, r2
	str	r0, [r3, #0]
	ldr	r6, [pc, #80]
	ldr	r3, [pc, #80]
	mov	r8, r0
	movs	r5, #0
	add	r3, r8
	add	r6, r8
	movs	r0, #183
	str	r5, [r3, #0]
	str	r5, [r6, #0]
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_080aa62c
	ldr	r0, [pc, #60]
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_080aa5fe
	ldr	r0, [pc, #52]
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_080aa5fa
	mov	r3, sl
	b.n	.L_080aa62a
.L_080aa5fa:
	movs	r3, #14
	b.n	.L_080aa62a
.L_080aa5fe:
	ldr	r0, [pc, #36]
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_080aa628
	movs	r3, #27
	b.n	.L_080aa62a
	.4byte 0x02000240
	.4byte 0x03001e68
	.4byte 0x00002130
	.4byte 0x0000212c
	.4byte 0x00002128
	.4byte 0x0000016f
	.2byte 0x0171
	.2byte 0x0000
.L_080aa628:
	movs	r3, #28
.L_080aa62a:
	str	r3, [r6, #0]
.L_080aa62c:
	bl	sub_080a1070
	movs	r0, #1
	bl	sub_080153e0
	ldr	r0, [pc, #140]
	bl	sub_08015418
	movs	r2, #130
	lsls	r2, r2, #2
	adds	r0, r7, r2
	bl	sub_08077158
	ldr	r2, [pc, #128]
	adds	r3, r7, r2
	strb	r0, [r3, #0]
	bl	sub_080ae88c
	movs	r1, #3
	movs	r2, #0
	movs	r3, #7
	movs	r0, #0
	bl	sub_080a3354
	movs	r0, #0
	bl	sub_080aa544
	movs	r0, #14
	bl	sub_080a2144
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #17
	movs	r3, #5
	movs	r0, #13
	bl	sub_08015010
	movs	r2, #188
	lsls	r2, r2, #1
	movs	r3, #134
	adds	r1, r7, r2
	lsls	r3, r3, #1
	ldr	r2, [pc, #60]
	adds	r4, r7, r3
	movs	r3, #255
	str	r0, [r4, #0]
	strh	r3, [r1, #0]
	strb	r2, [r7, #28]
	strb	r2, [r7, #29]
	movs	r2, #186
	lsls	r2, r2, #1
	movs	r5, #0
	adds	r3, r7, r2
	adds	r2, #2
	strh	r5, [r3, #0]
	adds	r3, r7, r2
	strh	r5, [r3, #0]
	movs	r1, #0
	ldr	r0, [r4, #0]
	bl	sub_080ad508
	bl	sub_080aa768
	bl	sub_080ad658
	bl	sub_080ae8dc
	movs	r0, #1
	bl	sub_080030f8
	bl	sub_080a34c0
	b.n	.L_080aa6cc
	.4byte 0x00000000
	.4byte 0x06002500
	.2byte 0x0219
	.2byte 0x0000
.L_080aa6cc:
	movs	r1, #0
	movs	r2, #30
	movs	r0, #0
	movs	r3, #20
	bl	sub_08015408
	ldr	r3, [pc, #116]
	ldr	r3, [r3, #0]
	strh	r5, [r3, #4]
	bl	sub_080152a8
	movs	r0, #0
	bl	sub_080153e0
	mov	r1, r8
	movs	r2, #128
	ldr	r5, [pc, #100]
	adds	r1, #168
	lsls	r2, r2, #6
	ldr	r0, [pc, #100]
	bl	sub_080072f8
	ldr	r1, [pc, #96]
	movs	r2, #128
	add	r1, r8
	ldr	r0, [pc, #96]
	bl	sub_080072f8
	movs	r0, #1
	bl	sub_080030f8
	bl	sub_080a1050
	movs	r1, #0
	movs	r0, #0
	movs	r2, #30
	movs	r3, #20
	bl	sub_08015410
	movs	r2, #194
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	bl	sub_08002df0
	movs	r0, #55
	bl	sub_08002dd8
	bl	sub_0808a548
	ldr	r3, [pc, #48]
	movs	r2, #131
	lsls	r2, r2, #2
	adds	r3, r3, r2
	mov	r2, r9
	movs	r0, #1
	strb	r2, [r3, #0]
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e68
	.4byte 0x03001388
	.4byte 0x06004000
	.4byte 0x000020a8
	.4byte 0x05000080
	.4byte 0x02000240

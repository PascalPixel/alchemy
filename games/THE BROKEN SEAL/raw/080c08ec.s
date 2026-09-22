.syntax unified
	.thumb
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080041d8, 0x080041d8
	.set sub_080048b0, 0x080048b0
	.set sub_080072ec, 0x080072ec
	.set sub_080072f0, 0x080072f0
	.set sub_080770c0, 0x080770c0
	.set sub_080c0098, 0x080c0098
	.set sub_080c00d8, 0x080c00d8
	.set sub_080c1724, 0x080c1724
	.global Overlay_080c08ec
Overlay_080c08ec:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r7, r2, #0
	ldr	r2, [pc, #212]
	ldr	r2, [r2, #0]
	mov	r9, r0
	adds	r0, r1, #0
	mov	sl, r2
	bl	sub_08002f40
	ldr	r3, [pc, #200]
	subs	r3, #140
	ldr	r6, [r3, #0]
	mov	r8, r0
	ldr	r5, [pc, #196]
	movs	r0, #49
	adds	r1, r5, #0
	bl	sub_080048b0
	movs	r2, #132
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	adds	r1, r0, #0
	ldr	r3, [pc, #180]
	ldr	r0, [pc, #184]
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #128
	ldr	r2, [pc, #160]
	lsls	r0, r0, #1
	ldr	r3, [r2, #20]
	ldr	r1, [pc, #172]
	add	r0, r8
	bl	sub_080072f0
	movs	r0, #49
	bl	sub_08002dd8
	ldr	r3, [pc, #160]
	adds	r4, r6, r3
	mov	r0, r8
	ldr	r3, [pc, #144]
	adds	r1, r4, #0
	ldr	r2, [pc, #156]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	cmp	r7, #0
	blt.n	.L_080c0974
	lsls	r3, r7, #4
	ldr	r2, [pc, #148]
	adds	r3, r3, r7
	lsls	r3, r3, #4
	adds	r0, r6, r2
	adds	r3, r3, r7
	movs	r2, #128
	lsls	r3, r3, #2
	lsls	r2, r2, #9
	subs	r2, r2, r3
	str	r2, [r0, #0]
	ldr	r1, [pc, #132]
	adds	r0, r4, #0
	movs	r3, #128
	bl	sub_080c1724
.L_080c0974:
	ldr	r3, [pc, #96]
	ldr	r0, [pc, #124]
	ldr	r1, [pc, #124]
	ldr	r2, [pc, #128]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #124]
	ldr	r2, [pc, #128]
	ldrh	r3, [r3, #0]
	ldr	r0, [pc, #128]
	strh	r3, [r2, #0]
	bl	sub_080c0098
	ldr	r0, [pc, #124]
	bl	sub_080c00d8
	ldr	r3, [pc, #120]
	ldr	r0, [pc, #124]
	movs	r1, #64
	bl	sub_080072f0
	mov	r2, sl
	ldr	r3, [r2, #8]
	cmp	r3, #0
	bne.n	.L_080c09ae
	ldr	r0, [pc, #112]
	ldr	r1, [pc, #112]
	bl	sub_080041d8
.L_080c09ae:
	mov	r3, r9
	mov	r2, sl
	str	r3, [r2, #8]
	cmp	r3, #1
	bne.n	.L_080c09be
	ldr	r2, [pc, #100]
	ldr	r3, [pc, #16]
	strh	r3, [r2, #0]
.L_080c09be:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00001f83
	.4byte 0x03001f00
	.4byte 0x00000230
	.4byte 0x040000d4
	.4byte 0x080b5138
	.4byte 0x06008000
	.4byte 0x00000544
	.4byte 0x84000040
	.4byte 0x00000644
	.4byte 0x050000c0
	.4byte 0x05000200
	.4byte 0x050000a0
	.4byte 0x80000010
	.4byte 0x050001e8
	.4byte 0x050000bc
	.4byte 0x06003800
	.4byte 0x0600f800
	.4byte 0x03000164
	.4byte 0x0600ffc0
	.4byte 0x080c0131
	.4byte 0x000004ff
	.2byte 0x000a
	.2byte 0x0400
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	str	r3, [sp, #4]
	ldr	r3, [pc, #316]
	str	r2, [sp, #8]
	str	r1, [sp, #12]
	adds	r2, r3, #0
	subs	r2, #136
	ldr	r2, [r2, #0]
	mov	sl, r0
	movs	r0, #128
	ldr	r1, [r3, #0]
	lsls	r0, r0, #4
	mov	r9, r2
	subs	r3, #128
	movs	r2, #0
	ldr	r3, [r3, #0]
	mov	fp, r0
	str	r2, [sp, #0]
	ldr	r0, [sp, #48]
	movs	r2, #128
	lsls	r2, r2, #9
	cmp	r0, r2
	blt.n	.L_080c0a78
	movs	r0, #128
	lsls	r0, r0, #6
	str	r0, [sp, #0]
	movs	r0, #54
	ldrsh	r2, [r3, r0]
	negs	r2, r2
	lsls	r3, r2, #1
	adds	r3, r3, r2
	movs	r2, #208
	lsls	r2, r2, #7
	adds	r2, r2, r3
	mov	fp, r2
.L_080c0a78:
	mov	r3, r9
	cmp	r3, #0
	bne.n	.L_080c0a80
	b.n	.L_080c0bd2
.L_080c0a80:
	ldr	r0, [r1, #8]
	cmp	r0, #1
	beq.n	.L_080c0a8c
	ldr	r3, [r1, #12]
	cmp	r3, #1
	bne.n	.L_080c0a9a
.L_080c0a8c:
	ldr	r3, [r1, #16]
	cmp	r3, #0
	bne.n	.L_080c0a9a
	ldr	r2, [pc, #228]
	mov	r1, fp
	asrs	r3, r1, #8
	strh	r3, [r2, #4]
.L_080c0a9a:
	cmp	r0, #2
	beq.n	.L_080c0aa0
	b.n	.L_080c0bd2
.L_080c0aa0:
	mov	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #1
	eors	r2, r3
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #6
	movs	r1, #128
	add	r3, r9
	ldr	r0, [sp, #48]
	lsls	r1, r1, #9
	ldr	r2, [pc, #196]
	adds	r7, r3, #0
	bl	sub_080072ec
	mov	r6, r9
	adds	r6, #16
	asrs	r5, r0, #8
	movs	r3, #0
	strh	r5, [r6, #0]
	strh	r3, [r6, #2]
	strh	r3, [r6, #4]
	strh	r5, [r6, #6]
	mov	lr, r0
	ldr	r3, [sp, #48]
	ldr	r0, [pc, #172]
	adds	r3, r3, r0
	mov	r8, r3
	adds	r7, #32
	ldr	r4, [pc, #168]
	mov	r0, sl
	mov	r1, r8
	mov	ip, pc
	bx	r4
	adds	r1, r0, #0
	mov	r0, lr
	mov	ip, pc
	bx	r4
	adds	r3, r0, #0
	mov	r1, r8
	ldr	r0, [sp, #12]
	movs	r0, r0
	mov	ip, pc
	bx	r4
	adds	r1, r0, #0
	mov	r0, lr
	mov	ip, pc
	bx	r4
	ldr	r2, [pc, #132]
	ldr	r1, [sp, #8]
	adds	r3, r3, r2
	asrs	r3, r3, #8
	adds	r3, r3, r1
	add	r3, fp
	str	r3, [r6, #8]
	adds	r0, r0, r2
	ldr	r2, [sp, #4]
	ldr	r3, [pc, #120]
	asrs	r0, r0, #8
	adds	r0, r0, r2
	lsls	r5, r5, #16
	movs	r1, #128
	adds	r0, r0, r3
	asrs	r5, r5, #16
	lsls	r1, r1, #7
	str	r0, [r6, #12]
	subs	r1, r1, r0
	ldr	r2, [pc, #84]
	adds	r0, r5, #0
	bl	sub_080072ec
	asrs	r0, r0, #16
	adds	r6, r0, #1
	ldr	r0, [pc, #92]
	movs	r5, #0
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_080c0b4a
	ldr	r3, [pc, #44]
.L_080c0b40:
	adds	r5, #1
	strh	r3, [r7, #0]
	adds	r7, #2
	cmp	r5, #15
	bls.n	.L_080c0b40
.L_080c0b4a:
	cmp	r6, #136
	bls.n	.L_080c0b50
	movs	r6, #136
.L_080c0b50:
	cmp	r5, r6
	bcs.n	.L_080c0b94
	ldr	r3, [sp, #0]
	lsls	r2, r3, #16
	ldr	r3, [pc, #20]
	lsrs	r2, r2, #16
	orrs	r2, r3
.L_080c0b5e:
	adds	r5, #1
	strh	r2, [r7, #0]
	adds	r7, #2
	cmp	r5, r6
	bcc.n	.L_080c0b5e
	b.n	.L_080c0b94
	movs	r0, r0
	.4byte 0x00003f8e
	.4byte 0x0000478a
	.4byte 0x03001f00
	.4byte 0x03001ad0
	.4byte 0x0300013c
	.4byte 0xffff0000
	.4byte 0x03000118
	.4byte 0x00007fff
	.4byte 0xfffff000
	.2byte 0x016b
	.2byte 0x0000
.L_080c0b94:
	cmp	r5, #135
	bhi.n	.L_080c0bac
	ldr	r0, [sp, #0]
	ldr	r3, [pc, #36]
	lsls	r2, r0, #16
	lsrs	r2, r2, #16
	orrs	r2, r3
.L_080c0ba2:
	adds	r5, #1
	strh	r2, [r7, #0]
	adds	r7, #2
	cmp	r5, #135
	bls.n	.L_080c0ba2
.L_080c0bac:
	cmp	r5, #159
	bhi.n	.L_080c0bc8
	ldr	r3, [pc, #16]
.L_080c0bb2:
	adds	r5, #1
	strh	r3, [r7, #0]
	adds	r7, #2
	cmp	r5, #159
	bls.n	.L_080c0bb2
	b.n	.L_080c0bc8
	movs	r0, r0
	.4byte 0x0000478e
	.2byte 0x3f8e
	.2byte 0x0000
.L_080c0bc8:
	mov	r1, r9
	ldr	r3, [r1, #0]
	movs	r2, #1
	eors	r3, r2
	str	r3, [r1, #0]
.L_080c0bd2:
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0

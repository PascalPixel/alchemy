.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_08002df0, 0x08002df0
	.set sub_08004938, 0x08004938
	.set sub_08004970, 0x08004970
	.set sub_080072f0, 0x080072f0
	.set sub_080162d4, 0x080162d4
	.set sub_08016418, 0x08016418
	.set sub_08017c8c, 0x08017c8c
	.set sub_08019000, 0x08019000
	.set sub_0801e41c, 0x0801e41c
	.set sub_0801e71c, 0x0801e71c
	.set sub_0801e7c0, 0x0801e7c0
	.set sub_0801ea08, 0x0801ea08
	.set sub_080218dc, 0x080218dc
	.set sub_080228bc, 0x080228bc
	.set sub_080228e4, 0x080228e4
	.set sub_08022a38, 0x08022a38
	.set sub_08022a7c, 0x08022a7c
	.set sub_08077008, 0x08077008
	.set sub_08077010, 0x08077010
	.set sub_08077080, 0x08077080
	.set sub_080771b0, 0x080771b0
	.set sub_080771b8, 0x080771b8
	.set sub_08077208, 0x08077208
	.global DjinnMenu_ShowChangePreview
	.global Func_08022b44
	.thumb_func
DjinnMenu_ShowChangePreview:
Func_08022b44:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #76
	add	r4, sp, #72
	adds	r7, r1, #0
	str	r0, [r4, #0]
	adds	r0, r7, #0
	mov	sl, r4
	mov	r8, r2
	str	r3, [sp, #60]
	bl	sub_08077008
	str	r0, [sp, #56]
	mov	r1, sl
	ldr	r0, [r1, #0]
	cmp	r0, #0
	beq.n	.L_08022b76
	movs	r1, #1
	bl	sub_08016418
.L_08022b76:
	ldr	r2, [sp, #60]
	cmp	r2, #0
	bne.n	.L_08022b92
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r0, #0
	movs	r3, #11
	movs	r1, #8
	movs	r2, #21
	bl	sub_080162d4
	mov	r3, sl
	str	r0, [r3, #0]
	b.n	.L_08022ba6
.L_08022b92:
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r0, #0
	movs	r1, #5
	movs	r2, #21
	movs	r3, #14
	bl	sub_080162d4
	mov	r4, sl
	str	r0, [r4, #0]
.L_08022ba6:
	mov	r1, sl
	ldr	r3, [r1, #0]
	movs	r0, #0
	cmp	r3, #0
	bne.n	.L_08022bb2
	b.n	.L_08023134
.L_08022bb2:
	movs	r0, #128
	bl	sub_08004938
	movs	r5, #166
	lsls	r5, r5, #1
	str	r0, [sp, #48]
	adds	r0, r5, #0
	bl	sub_08004938
	str	r0, [sp, #52]
	movs	r0, #96
	bl	sub_08004970
	adds	r2, r5, #0
	str	r0, [sp, #24]
	ldr	r3, [pc, #640]
	ldr	r1, [sp, #56]
	ldr	r0, [sp, #52]
	bl	sub_080072f0
	mov	r2, r8
	asrs	r5, r2, #8
	movs	r3, #15
	movs	r6, #255
	ands	r5, r3
	ands	r6, r2
	adds	r0, r7, #0
	adds	r1, r5, #0
	adds	r2, r6, #0
	bl	sub_08077208
	cmp	r0, #0
	beq.n	.L_08022c00
	adds	r0, r7, #0
	adds	r1, r5, #0
	adds	r2, r6, #0
	bl	sub_080771b8
	b.n	.L_08022c0a
.L_08022c00:
	adds	r0, r7, #0
	adds	r1, r5, #0
	adds	r2, r6, #0
	bl	sub_080771b0
.L_08022c0a:
	adds	r0, r7, #0
	bl	sub_08077010
	ldr	r0, [sp, #52]
	ldr	r1, [sp, #56]
	add	r2, sp, #64
	add	r3, sp, #68
	str	r2, [sp, #0]
	adds	r1, #88
	ldr	r2, [sp, #24]
	adds	r0, #88
	bl	sub_080228e4
	movs	r1, #5
	str	r0, [sp, #20]
	subs	r0, #1
	bl	sub_080022ec
	ldr	r3, [sp, #108]
	adds	r0, #1
	str	r0, [r3, #0]
	ldr	r4, [sp, #60]
	lsls	r3, r4, #2
	adds	r3, r3, r4
	ldr	r1, [sp, #20]
	subs	r3, #5
	cmp	r3, r1
	blt.n	.L_08022c44
	str	r0, [sp, #60]
.L_08022c44:
	ldr	r2, [sp, #60]
	cmp	r2, #0
	beq.n	.L_08022c4c
	b.n	.L_08022d8c
.L_08022c4c:
	ldr	r5, [pc, #520]
	mov	r3, sl
	ldr	r1, [r3, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #8
	bl	sub_0801e7c0
	mov	r4, sl
	ldr	r1, [r4, #0]
	adds	r0, r5, #1
	movs	r2, #0
	movs	r3, #16
	bl	sub_0801e7c0
	mov	r2, sl
	adds	r0, r5, #2
	ldr	r1, [r2, #0]
	movs	r3, #24
	movs	r2, #0
	bl	sub_0801e7c0
	mov	r3, sl
	adds	r0, r5, #3
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r3, #32
	bl	sub_0801e7c0
	mov	r4, sl
	ldr	r1, [r4, #0]
	adds	r0, r5, #4
	movs	r2, #0
	movs	r3, #40
	bl	sub_0801e7c0
	adds	r5, #5
	mov	r2, sl
	ldr	r1, [r2, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #48
	bl	sub_0801e7c0
	ldr	r3, [sp, #52]
	movs	r4, #56
	ldrsh	r3, [r3, r4]
	ldr	r0, [sp, #48]
	adds	r1, r3, #0
	str	r3, [sp, #44]
	bl	sub_080228bc
	ldr	r5, [sp, #48]
	mov	r4, sl
	adds	r5, #14
	ldr	r1, [r4, #0]
	movs	r2, #5
	movs	r3, #1
	adds	r0, r5, #0
	bl	sub_08017c8c
	ldr	r1, [sp, #52]
	movs	r2, #58
	ldrsh	r1, [r1, r2]
	ldr	r0, [sp, #48]
	str	r1, [sp, #40]
	bl	sub_080228bc
	mov	r2, sl
	ldr	r1, [r2, #0]
	adds	r0, r5, #0
	movs	r2, #5
	movs	r3, #2
	bl	sub_08017c8c
	ldr	r3, [sp, #52]
	ldrh	r3, [r3, #60]
	ldr	r0, [sp, #48]
	adds	r1, r3, #0
	str	r3, [sp, #36]
	bl	sub_080228bc
	ldr	r5, [sp, #48]
	mov	r4, sl
	adds	r5, #16
	ldr	r1, [r4, #0]
	movs	r2, #6
	movs	r3, #3
	adds	r0, r5, #0
	bl	sub_08017c8c
	ldr	r1, [sp, #52]
	ldrh	r1, [r1, #62]
	ldr	r0, [sp, #48]
	str	r1, [sp, #32]
	bl	sub_080228bc
	mov	r2, sl
	ldr	r1, [r2, #0]
	movs	r3, #4
	movs	r2, #6
	adds	r0, r5, #0
	bl	sub_08017c8c
	ldr	r3, [sp, #52]
	adds	r3, #64
	ldrh	r3, [r3, #0]
	ldr	r0, [sp, #48]
	adds	r1, r3, #0
	str	r3, [sp, #28]
	bl	sub_080228bc
	mov	r3, sl
	ldr	r1, [r3, #0]
	movs	r2, #6
	movs	r3, #5
	adds	r0, r5, #0
	bl	sub_08017c8c
	ldr	r3, [sp, #52]
	adds	r3, #66
	ldrb	r1, [r3, #0]
	ldr	r0, [sp, #48]
	bl	sub_080228bc
	mov	r4, sl
	ldr	r1, [r4, #0]
	movs	r2, #5
	movs	r3, #6
	adds	r0, r5, #0
	bl	sub_08017c8c
	mov	r1, sl
	movs	r3, #8
	ldr	r0, [r1, #0]
	movs	r2, #8
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r3, #19
	bl	sub_0801e41c
	ldr	r3, [sp, #68]
	cmp	r3, #0
	bne.n	.L_08022d72
	ldr	r3, [sp, #64]
	cmp	r3, #0
	beq.n	.L_08022d78
.L_08022d72:
	movs	r0, #2
	bl	sub_0801e71c
.L_08022d78:
	mov	r2, sl
	ldr	r0, [pc, #224]
	ldr	r1, [r2, #0]
	movs	r3, #64
	movs	r2, #24
	bl	sub_0801e7c0
	movs	r0, #15
	bl	sub_0801e71c
.L_08022d8c:
	ldr	r3, [sp, #60]
	cmp	r3, #0
	bgt.n	.L_08022d94
	b.n	.L_08022f54
.L_08022d94:
	ldr	r1, [sp, #20]
	movs	r4, #0
	str	r4, [sp, #12]
	str	r1, [sp, #8]
	cmp	r1, #4
	ble.n	.L_08022da4
	movs	r2, #5
	str	r2, [sp, #8]
.L_08022da4:
	ldr	r4, [sp, #60]
	lsls	r3, r4, #2
	adds	r3, r3, r4
	subs	r3, #5
	mov	r8, r3
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #8]
	movs	r1, #0
	str	r1, [sp, #16]
	cmp	r2, r3
	blt.n	.L_08022dbc
	b.n	.L_08022ee0
.L_08022dbc:
	ldr	r4, [sp, #20]
	cmp	r8, r4
	blt.n	.L_08022dc4
	b.n	.L_08022ee0
.L_08022dc4:
	mov	r1, r8
	ldr	r2, [sp, #24]
	movs	r4, #4
	lsls	r3, r1, #1
	negs	r4, r4
	adds	r6, r3, r2
	str	r4, [sp, #4]
	movs	r3, #0
	mov	r7, sl
	mov	r9, r3
	mov	fp, r3
.L_08022dda:
	ldrh	r0, [r6, #0]
	bl	sub_08077080
	adds	r5, r0, #0
	ldrb	r3, [r5, #2]
	cmp	r3, #4
	beq.n	.L_08022dfe
	mov	r1, sl
	ldr	r2, [pc, #116]
	ldr	r0, [r1, #0]
	adds	r1, r3, #0
	movs	r3, #0
	adds	r1, r1, r2
	str	r3, [sp, #0]
	movs	r2, #15
	mov	r3, r9
	bl	sub_08019000
.L_08022dfe:
	ldrb	r3, [r5, #8]
	cmp	r3, #255
	bne.n	.L_08022e08
	movs	r3, #11
	b.n	.L_08022e0a
.L_08022e08:
	subs	r3, #1
.L_08022e0a:
	movs	r4, #0
	ldr	r0, [r7, #0]
	movs	r1, #16
	mov	r2, r9
	str	r4, [sp, #0]
	bl	sub_080218dc
	ldrh	r3, [r6, #0]
	ldr	r1, [pc, #72]
	ldr	r2, [sp, #4]
	ands	r3, r1
	ldr	r0, [r7, #0]
	movs	r1, #0
	bl	sub_08022a38
	ldrh	r2, [r6, #0]
	ldr	r3, [pc, #32]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08022e3a
	movs	r0, #4
	bl	sub_0801e71c
	b.n	.L_08022e6e
.L_08022e3a:
	ldr	r3, [pc, #20]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08022e68
	movs	r0, #2
	bl	sub_0801e71c
	b.n	.L_08022e6e
	movs	r0, r0
	.4byte 0x00008000
	.4byte 0x00004000
	.4byte 0x03001388
	.4byte 0x000008ae
	.4byte 0x000008ad
	.4byte 0x00005001
	.2byte 0x3fff
	.2byte 0x0000
.L_08022e68:
	movs	r0, #15
	bl	sub_0801e71c
.L_08022e6e:
	ldrh	r3, [r6, #0]
	ldr	r0, [pc, #724]
	ands	r0, r3
	ldr	r3, [pc, #724]
	ldr	r1, [r7, #0]
	adds	r0, r0, r3
	movs	r2, #16
	mov	r3, fp
	bl	sub_0801e7c0
	movs	r2, #0
	ldr	r0, [r7, #0]
	mov	r3, r9
	str	r2, [sp, #0]
	ldr	r1, [pc, #708]
	movs	r2, #11
	bl	sub_08019000
	movs	r3, #0
	ldr	r0, [r7, #0]
	ldr	r1, [pc, #700]
	str	r3, [sp, #0]
	movs	r2, #12
	mov	r3, r9
	bl	sub_08019000
	ldrh	r0, [r6, #0]
	bl	sub_08077080
	mov	r4, fp
	ldr	r2, [r7, #0]
	ldrb	r0, [r0, #9]
	movs	r1, #2
	movs	r3, #104
	str	r4, [sp, #0]
	bl	sub_0801ea08
	movs	r2, #16
	ldr	r3, [sp, #4]
	ldr	r4, [sp, #16]
	movs	r1, #2
	add	fp, r2
	ldr	r2, [sp, #8]
	add	r9, r1
	adds	r3, #16
	adds	r4, #1
	movs	r1, #1
	str	r3, [sp, #4]
	str	r4, [sp, #16]
	adds	r6, #2
	add	r8, r1
	cmp	r4, r2
	bge.n	.L_08022ee0
	ldr	r3, [sp, #20]
	cmp	r8, r3
	bge.n	.L_08022ee0
	b.n	.L_08022dda
.L_08022ee0:
	ldr	r3, [sp, #68]
	cmp	r3, #0
	beq.n	.L_08022efe
	movs	r0, #4
	bl	sub_0801e71c
	mov	r4, sl
	ldr	r1, [r4, #0]
	ldr	r0, [pc, #612]
	movs	r2, #32
	movs	r3, #80
	bl	sub_0801e7c0
	movs	r1, #1
	str	r1, [sp, #12]
.L_08022efe:
	ldr	r3, [sp, #64]
	cmp	r3, #0
	beq.n	.L_08022f22
	movs	r0, #2
	bl	sub_0801e71c
	ldr	r4, [sp, #12]
	mov	r2, sl
	lsls	r3, r4, #3
	ldr	r1, [r2, #0]
	ldr	r0, [pc, #584]
	adds	r3, #80
	movs	r2, #32
	bl	sub_0801e7c0
	ldr	r1, [sp, #12]
	adds	r1, #1
	str	r1, [sp, #12]
.L_08022f22:
	ldr	r2, [sp, #12]
	cmp	r2, #0
	bne.n	.L_08022f36
	mov	r3, sl
	ldr	r1, [r3, #0]
	ldr	r0, [pc, #560]
	movs	r2, #32
	movs	r3, #80
	bl	sub_0801e7c0
.L_08022f36:
	movs	r0, #15
	bl	sub_0801e71c
	movs	r0, #15
	bl	sub_0801e71c
	movs	r3, #10
	mov	r4, sl
	ldr	r0, [r4, #0]
	movs	r1, #0
	str	r3, [sp, #0]
	movs	r2, #10
	movs	r3, #19
	bl	sub_0801e41c
.L_08022f54:
	ldr	r1, [sp, #60]
	cmp	r1, #0
	beq.n	.L_08022f5c
	b.n	.L_08023110
.L_08022f5c:
	ldr	r2, [sp, #52]
	ldr	r5, [pc, #516]
	adds	r2, r2, r5
	ldrb	r0, [r2, #0]
	ldr	r6, [pc, #512]
	mov	r3, sl
	ldr	r1, [r3, #0]
	mov	r8, r2
	adds	r0, r0, r6
	movs	r2, #0
	movs	r3, #0
	bl	sub_0801e7c0
	ldr	r4, [sp, #56]
	adds	r5, r4, r5
	ldrb	r0, [r5, #0]
	mov	r2, sl
	ldr	r1, [r2, #0]
	movs	r3, #0
	movs	r2, #80
	adds	r0, r0, r6
	bl	sub_0801e7c0
	mov	r3, r8
	ldrb	r2, [r3, #0]
	ldrb	r3, [r5, #0]
	cmp	r2, r3
	beq.n	.L_08022fa8
	ldr	r2, [sp, #60]
	mov	r4, sl
	ldr	r0, [r4, #0]
	ldr	r1, [pc, #464]
	str	r2, [sp, #0]
	movs	r3, #0
	movs	r2, #9
	bl	sub_08019000
	b.n	.L_08022fba
.L_08022fa8:
	mov	r3, sl
	ldr	r4, [sp, #60]
	ldr	r0, [r3, #0]
	ldr	r1, [pc, #448]
	movs	r2, #9
	movs	r3, #0
	str	r4, [sp, #0]
	bl	sub_08019000
.L_08022fba:
	ldr	r3, [sp, #56]
	ldr	r0, [sp, #48]
	movs	r2, #56
	ldrsh	r1, [r3, r2]
	bl	sub_080228bc
	ldr	r6, [sp, #48]
	mov	r4, sl
	adds	r6, #14
	ldr	r1, [r4, #0]
	movs	r2, #11
	movs	r3, #1
	adds	r0, r6, #0
	bl	sub_08017c8c
	ldr	r2, [sp, #56]
	ldr	r4, [sp, #44]
	movs	r1, #56
	ldrsh	r3, [r2, r1]
	cmp	r3, r4
	beq.n	.L_08022ff8
	movs	r2, #0
	cmp	r3, r4
	ble.n	.L_08022fec
	movs	r2, #1
.L_08022fec:
	add	r1, sp, #76
	mov	r9, r1
	movs	r0, #80
	movs	r1, #14
	bl	sub_08022a7c
.L_08022ff8:
	ldr	r3, [sp, #56]
	ldr	r0, [sp, #48]
	movs	r2, #58
	ldrsh	r1, [r3, r2]
	bl	sub_080228bc
	mov	r4, sl
	ldr	r1, [r4, #0]
	movs	r2, #11
	movs	r3, #2
	adds	r0, r6, #0
	bl	sub_08017c8c
	ldr	r2, [sp, #56]
	ldr	r4, [sp, #40]
	movs	r1, #58
	ldrsh	r3, [r2, r1]
	cmp	r3, r4
	beq.n	.L_08023032
	movs	r2, #0
	cmp	r3, r4
	ble.n	.L_08023026
	movs	r2, #1
.L_08023026:
	add	r1, sp, #76
	mov	r9, r1
	movs	r0, #80
	movs	r1, #22
	bl	sub_08022a7c
.L_08023032:
	ldr	r2, [sp, #56]
	ldr	r0, [sp, #48]
	ldrh	r1, [r2, #60]
	bl	sub_080228bc
	mov	r3, sl
	ldr	r1, [r3, #0]
	adds	r0, r6, #0
	movs	r3, #3
	movs	r2, #11
	bl	sub_08017c8c
	ldr	r4, [sp, #56]
	ldr	r1, [sp, #36]
	ldrh	r3, [r4, #60]
	cmp	r3, r1
	beq.n	.L_08023068
	movs	r2, #0
	cmp	r3, r1
	ble.n	.L_0802305c
	movs	r2, #1
.L_0802305c:
	add	r3, sp, #76
	mov	r9, r3
	movs	r0, #80
	movs	r1, #30
	bl	sub_08022a7c
.L_08023068:
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #48]
	ldrh	r1, [r4, #62]
	bl	sub_080228bc
	mov	r2, sl
	ldr	r1, [r2, #0]
	movs	r3, #4
	adds	r0, r6, #0
	movs	r2, #11
	bl	sub_08017c8c
	ldr	r4, [sp, #56]
	ldr	r1, [sp, #32]
	ldrh	r3, [r4, #62]
	cmp	r3, r1
	beq.n	.L_0802309e
	movs	r2, #0
	cmp	r3, r1
	ble.n	.L_08023092
	movs	r2, #1
.L_08023092:
	add	r3, sp, #76
	mov	r9, r3
	movs	r0, #80
	movs	r1, #38
	bl	sub_08022a7c
.L_0802309e:
	ldr	r5, [sp, #56]
	adds	r5, #64
	ldrh	r1, [r5, #0]
	ldr	r0, [sp, #48]
	bl	sub_080228bc
	mov	r4, sl
	ldr	r1, [r4, #0]
	movs	r3, #5
	adds	r0, r6, #0
	movs	r2, #11
	bl	sub_08017c8c
	ldrh	r3, [r5, #0]
	ldr	r1, [sp, #28]
	cmp	r3, r1
	beq.n	.L_080230d4
	movs	r2, #0
	cmp	r3, r1
	ble.n	.L_080230c8
	movs	r2, #1
.L_080230c8:
	add	r3, sp, #76
	mov	r9, r3
	movs	r0, #80
	movs	r1, #46
	bl	sub_08022a7c
.L_080230d4:
	ldr	r5, [sp, #56]
	adds	r5, #66
	ldrb	r1, [r5, #0]
	ldr	r0, [sp, #48]
	bl	sub_080228bc
	ldr	r0, [sp, #48]
	mov	r4, sl
	ldr	r1, [r4, #0]
	movs	r3, #6
	adds	r0, #16
	movs	r2, #12
	bl	sub_08017c8c
	ldr	r3, [sp, #52]
	adds	r3, #66
	ldrb	r1, [r5, #0]
	ldrb	r3, [r3, #0]
	cmp	r1, r3
	beq.n	.L_08023110
	movs	r2, #0
	cmp	r1, r3
	bls.n	.L_08023104
	movs	r2, #1
.L_08023104:
	add	r1, sp, #76
	mov	r9, r1
	movs	r0, #80
	movs	r1, #54
	bl	sub_08022a7c
.L_08023110:
	movs	r2, #166
	lsls	r2, r2, #1
	ldr	r3, [pc, #92]
	ldr	r0, [sp, #56]
	ldr	r1, [sp, #52]
	bl	sub_080072f0
	ldr	r0, [sp, #24]
	bl	sub_08002df0
	ldr	r0, [sp, #52]
	bl	sub_08002df0
	ldr	r0, [sp, #48]
	bl	sub_08002df0
	mov	r2, sl
	ldr	r0, [r2, #0]
.L_08023134:
	add	sp, #76
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x00003fff
	.4byte 0x00000333
	.4byte 0x0000f01f
	.4byte 0x0000f01e
	.4byte 0x00000ba2
	.4byte 0x00000ba3
	.4byte 0x00000ba8
	.4byte 0x00000129
	.4byte 0x00000741
	.4byte 0x0000f728
	.4byte 0x0000f729
	.4byte 0x03001388

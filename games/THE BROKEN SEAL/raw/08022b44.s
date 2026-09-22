.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_08002df0, 0x08002df0
	.set sub_080030f8, 0x080030f8
	.set sub_0800352c, 0x0800352c
	.set sub_08003dec, 0x08003dec
	.set sub_08003f3c, 0x08003f3c
	.set sub_080040b4, 0x080040b4
	.set sub_080040d0, 0x080040d0
	.set sub_08004938, 0x08004938
	.set sub_08004970, 0x08004970
	.set sub_080072f0, 0x080072f0
	.set sub_080072fc, 0x080072fc
	.set sub_080162d4, 0x080162d4
	.set sub_08016418, 0x08016418
	.set sub_08016498, 0x08016498
	.set sub_080164ac, 0x080164ac
	.set sub_08016738, 0x08016738
	.set sub_08017248, 0x08017248
	.set sub_08017aa4, 0x08017aa4
	.set sub_08017c8c, 0x08017c8c
	.set sub_08018efc, 0x08018efc
	.set sub_08019000, 0x08019000
	.set sub_0801965c, 0x0801965c
	.set sub_080198dc, 0x080198dc
	.set sub_08019908, 0x08019908
	.set sub_0801a3d0, 0x0801a3d0
	.set sub_0801e318, 0x0801e318
	.set sub_0801e3c8, 0x0801e3c8
	.set sub_0801e41c, 0x0801e41c
	.set sub_0801e71c, 0x0801e71c
	.set sub_0801e7c0, 0x0801e7c0
	.set sub_0801e8b0, 0x0801e8b0
	.set sub_0801e9d4, 0x0801e9d4
	.set sub_0801ea08, 0x0801ea08
	.set sub_0801f200, 0x0801f200
	.set sub_080218dc, 0x080218dc
	.set sub_08021ab0, 0x08021ab0
	.set sub_08021b80, 0x08021b80
	.set sub_08022768, 0x08022768
	.set sub_080228bc, 0x080228bc
	.set sub_080228e4, 0x080228e4
	.set sub_08022a38, 0x08022a38
	.set sub_08022a7c, 0x08022a7c
	.set sub_08077000, 0x08077000
	.set sub_08077008, 0x08077008
	.set sub_08077010, 0x08077010
	.set sub_08077080, 0x08077080
	.set sub_080771b0, 0x080771b0
	.set sub_080771b8, 0x080771b8
	.set sub_080771d8, 0x080771d8
	.set sub_080771e0, 0x080771e0
	.set sub_080771f8, 0x080771f8
	.set sub_08077208, 0x08077208
	.set sub_08077258, 0x08077258
	.set sub_08077290, 0x08077290
	.set sub_080b5098, 0x080b5098
	.set sub_080f9010, 0x080f9010
	.global Overlay_08022b44
Overlay_08022b44:
.L_08022b44:
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
	.2byte 0x1388
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #352
	str	r0, [sp, #92]
	str	r2, [sp, #84]
	str	r1, [sp, #88]
	ldr	r5, [pc, #360]
	movs	r3, #1
	ldr	r1, [r5, #0]
	movs	r0, #128
	movs	r2, #1
	negs	r3, r3
	lsls	r0, r0, #2
	str	r1, [sp, #80]
	mov	sl, r2
	str	r3, [sp, #68]
	bl	sub_080040b4
	movs	r4, #0
	str	r0, [sp, #52]
	ldr	r0, [sp, #68]
	str	r4, [sp, #48]
	bl	sub_08077290
	str	r0, [sp, #44]
	adds	r5, #168
	ldr	r5, [r5, #0]
	ldr	r0, [r5, #68]
	mov	r1, sl
	str	r1, [r5, #72]
	cmp	r0, #0
	beq.n	.L_080231ca
	bl	sub_08016418
	ldr	r2, [sp, #48]
	str	r2, [r5, #68]
.L_080231ca:
	movs	r0, #112
	bl	sub_080f9010
	movs	r6, #4
	movs	r2, #0
	add	r3, sp, #100
.L_080231d6:
	subs	r6, #1
	strb	r2, [r3, #0]
	subs	r3, #1
	cmp	r6, #0
	bge.n	.L_080231d6
	movs	r3, #0
	add	r2, sp, #96
	str	r3, [r2, #8]
	str	r3, [r2, #12]
	str	r3, [r2, #16]
	movs	r0, #128
	bl	sub_080040b4
	movs	r3, #142
	movs	r4, #136
	lsls	r3, r3, #1
	lsls	r4, r4, #1
	add	r3, sp
	add	r4, sp
	movs	r1, #1
	negs	r1, r1
	str	r0, [sp, #64]
	str	r3, [sp, #16]
	str	r4, [sp, #20]
	mov	r8, r1
	adds	r5, r4, #0
	adds	r7, r3, #0
	movs	r6, #10
.L_0802320e:
	movs	r0, #128
	bl	sub_080040b4
	subs	r6, #1
	mov	r2, r8
	strb	r2, [r5, #0]
	stmia	r7!, {r0}
	adds	r5, #1
	cmp	r6, #0
	bge.n	.L_0802320e
	ldr	r3, [sp, #88]
	cmp	r3, #0
	beq.n	.L_08023258
	ldr	r4, [sp, #92]
	ldrh	r3, [r4, #0]
	movs	r6, #0
	cmp	r3, #255
	beq.n	.L_08023258
	cmp	r3, #254
	beq.n	.L_0802323c
	ldr	r1, [sp, #84]
	cmp	r3, r1
	beq.n	.L_08023256
.L_0802323c:
	adds	r6, #1
	cmp	r6, #5
	bgt.n	.L_08023258
	ldr	r2, [sp, #92]
	lsls	r3, r6, #1
	ldrh	r3, [r3, r2]
	cmp	r3, #255
	beq.n	.L_08023258
	cmp	r3, #254
	beq.n	.L_0802323c
	ldr	r4, [sp, #84]
	cmp	r3, r4
	bne.n	.L_0802323c
.L_08023256:
	str	r6, [sp, #68]
.L_08023258:
	movs	r3, #6
	movs	r1, #0
	str	r3, [sp, #0]
	movs	r2, #30
	movs	r3, #20
	movs	r0, #0
	str	r1, [sp, #56]
	str	r1, [sp, #60]
	bl	sub_080162d4
	movs	r3, #10
	str	r0, [sp, #76]
	str	r3, [sp, #0]
	movs	r1, #14
	movs	r2, #30
	movs	r3, #6
	movs	r0, #0
	bl	sub_080162d4
	str	r0, [sp, #72]
	bl	sub_0801e318
	movs	r3, #170
	movs	r4, #164
	mov	r2, sp
	lsls	r3, r3, #1
	lsls	r4, r4, #1
	mov	r1, sp
	adds	r2, #128
	add	r3, sp
	add	r4, sp
	adds	r1, #140
	str	r2, [sp, #28]
	str	r3, [sp, #8]
	str	r4, [sp, #12]
	str	r1, [sp, #24]
.L_080232a0:
	ldr	r3, [pc, #88]
	ldr	r3, [r3, #0]
	ldr	r2, [sp, #28]
	str	r3, [sp, #40]
	ldr	r3, [pc, #84]
	str	r3, [r2, #4]
	movs	r3, #0
	str	r3, [r2, #8]
	ldr	r1, [sp, #52]
	ldr	r0, [sp, #84]
	bl	sub_08021b80
	ldr	r3, [pc, #52]
	ands	r0, r3
	ldr	r3, [sp, #28]
	ldrh	r2, [r3, #8]
	ldr	r3, [pc, #64]
	ldr	r4, [sp, #28]
	ands	r3, r2
	orrs	r3, r0
	strh	r3, [r4, #8]
	ldrh	r2, [r4, #6]
	ldr	r3, [pc, #56]
	ands	r3, r2
	ldr	r2, [pc, #32]
	ldr	r1, [sp, #28]
	orrs	r3, r2
	strh	r3, [r1, #6]
	ldrb	r2, [r1, #9]
	movs	r3, #24
	strb	r3, [r1, #4]
	movs	r3, #15
	ands	r3, r2
	movs	r2, #224
	orrs	r3, r2
	strb	r3, [r1, #9]
	ldr	r0, [sp, #28]
	movs	r1, #240
	b.n	.L_0802330c
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x00000008
	.4byte 0x03001e8c
	.4byte 0x03001b04
	.4byte 0x80000400
	.4byte 0xfffffc00
	.2byte 0xfe00
	.2byte 0xffff
.L_0802330c:
	bl	sub_08003dec
	ldr	r3, [sp, #104]
	cmp	r3, #21
	bhi.n	.L_08023378
	ldr	r2, [pc, #516]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x08023378
	.4byte 0x08023378
	.4byte 0x08023378
	.4byte 0x08023378
	.4byte 0x08023378
	.4byte 0x08023378
	.4byte 0x08023378
	.4byte 0x08023378
	.4byte 0x08023378
	.4byte 0x08023378
	.4byte 0x08023390
	.4byte 0x08023390
	.4byte 0x08023390
	.4byte 0x08023390
	.4byte 0x0802339c
	.4byte 0x0802339c
	.4byte 0x0802339c
	.4byte 0x0802339c
	.4byte 0x0802339c
	.4byte 0x0802339c
	.4byte 0x0802339c
	.2byte 0x339c
	.2byte 0x0802
.L_08023378:
	movs	r3, #0
	add	r2, sp, #96
	str	r3, [r2, #16]
	ldr	r3, [sp, #44]
	cmp	r3, #0
	beq.n	.L_0802338a
	movs	r3, #9
	str	r3, [r2, #20]
	b.n	.L_080233a6
.L_0802338a:
	movs	r3, #7
	str	r3, [r2, #20]
	b.n	.L_080233a6
	add	r2, sp, #96
	movs	r3, #1
	str	r3, [r2, #16]
	movs	r3, #4
	str	r3, [r2, #20]
	b.n	.L_080233a6
	add	r2, sp, #96
	movs	r3, #2
	str	r3, [r2, #16]
	ldr	r4, [sp, #48]
	str	r4, [r2, #20]
.L_080233a6:
	ldr	r1, [sp, #76]
	ldr	r3, [sp, #76]
	ldrh	r0, [r1, #12]
	ldrh	r2, [r3, #8]
	movs	r4, #15
	ldrh	r1, [r1, #14]
	ldrh	r3, [r3, #10]
	str	r4, [sp, #0]
	bl	sub_08022768
	ldr	r4, [sp, #48]
	cmp	r4, #0
	bne.n	.L_080233c2
	b.n	.L_080234ce
.L_080233c2:
	add	r6, sp, #96
	ldr	r3, [r6, #16]
	cmp	r3, #1
	bhi.n	.L_08023458
	ldr	r1, [sp, #40]
	ldrsb	r5, [r6, r3]
	movs	r3, #128
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080233ee
	movs	r2, #0
	movs	r0, #111
	str	r2, [sp, #40]
	bl	sub_080f9010
	ldr	r3, [r6, #20]
	adds	r5, #1
	cmp	r5, r3
	blt.n	.L_080234c8
	ldr	r3, [r6, #16]
	movs	r5, #0
	b.n	.L_0802340e
.L_080233ee:
	ldr	r4, [sp, #40]
	movs	r3, #64
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_0802341a
	movs	r1, #0
	movs	r0, #111
	subs	r5, #1
	str	r1, [sp, #40]
	bl	sub_080f9010
	cmp	r5, #0
	bge.n	.L_080234c8
	ldr	r3, [r6, #20]
	subs	r5, r3, #1
	ldr	r3, [r6, #16]
.L_0802340e:
	cmp	r3, #1
	bne.n	.L_080234c8
	movs	r3, #2
	str	r3, [r6, #16]
	ldrsb	r5, [r6, r3]
	b.n	.L_080234c8
.L_0802341a:
	ldr	r2, [sp, #40]
	movs	r3, #49
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080234c8
	movs	r3, #0
	movs	r0, #111
	str	r3, [sp, #40]
	bl	sub_080f9010
	ldr	r1, [r6, #16]
	movs	r3, #2
	eors	r1, r3
	str	r1, [r6, #16]
	ldr	r0, [pc, #232]
	ldr	r3, [r0, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08023446
	ldrsb	r5, [r6, r1]
	b.n	.L_080234c8
.L_08023446:
	ldr	r2, [r0, #0]
	movs	r3, #32
	ldr	r5, [sp, #48]
	ands	r2, r3
	subs	r5, #1
	cmp	r2, #0
	bne.n	.L_080234c8
	movs	r5, #0
	b.n	.L_080234c8
.L_08023458:
	cmp	r3, #2
	bne.n	.L_080234ce
	ldrsb	r5, [r6, r3]
	ldr	r2, [r6, #20]
	cmp	r5, r2
	blt.n	.L_08023466
	subs	r5, r2, #1
.L_08023466:
	cmp	r5, #0
	bge.n	.L_08023474
	movs	r3, #0
	str	r3, [r6, #16]
	movs	r5, #0
	ldrsb	r5, [r6, r5]
	b.n	.L_080234c8
.L_08023474:
	ldr	r4, [sp, #40]
	movs	r3, #16
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_0802348c
	movs	r1, #0
	adds	r5, #1
	str	r1, [sp, #40]
	cmp	r5, r2
	blt.n	.L_080234a6
	str	r1, [r6, #16]
	b.n	.L_080234a2
.L_0802348c:
	ldr	r2, [sp, #40]
	movs	r3, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080234ae
	movs	r3, #0
	subs	r5, #1
	str	r3, [sp, #40]
	cmp	r5, #0
	bge.n	.L_080234a6
	str	r3, [r6, #16]
.L_080234a2:
	movs	r5, #0
	ldrsb	r5, [r6, r5]
.L_080234a6:
	movs	r0, #111
	bl	sub_080f9010
	b.n	.L_080234c8
.L_080234ae:
	ldr	r4, [sp, #40]
	movs	r3, #193
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_080234c8
	movs	r1, #0
	str	r1, [sp, #40]
	movs	r0, #111
	str	r1, [r6, #16]
	movs	r5, #0
	ldrsb	r5, [r6, r5]
	bl	sub_080f9010
.L_080234c8:
	add	r2, sp, #96
	ldr	r3, [r2, #16]
	strb	r5, [r2, r3]
.L_080234ce:
	add	r0, sp, #96
	ldr	r2, [r0, #16]
	cmp	r2, #0
	bne.n	.L_08023512
	ldr	r3, [sp, #44]
	ldrsb	r2, [r0, r2]
	cmp	r3, #0
	bne.n	.L_080234e0
	adds	r2, #9
.L_080234e0:
	ldr	r3, [pc, #64]
	lsls	r2, r2, #3
	adds	r2, r2, r3
	ldrb	r3, [r2, #0]
	str	r3, [r0, #8]
	ldrb	r3, [r2, #1]
	str	r3, [r0, #24]
	ldrb	r3, [r2, #2]
	str	r3, [r0, #28]
	ldr	r4, [sp, #76]
	ldrb	r3, [r2, #3]
	ldrh	r0, [r4, #12]
	ldrh	r1, [r4, #14]
	adds	r0, r0, r3
	ldrb	r3, [r2, #4]
	adds	r1, r1, r3
	movs	r3, #14
	ldrb	r2, [r2, #5]
	adds	r0, #1
	str	r3, [sp, #0]
	adds	r1, #1
	movs	r3, #1
	bl	sub_08022768
	b.n	.L_08023544
.L_08023512:
	cmp	r2, #1
	bne.n	.L_0802352c
	ldrsb	r3, [r0, r2]
	ldr	r1, [pc, #12]
	b.n	.L_08023534
	.4byte 0x08023320
	.4byte 0x03001b04
	.4byte 0x08037328
	.2byte 0x73a8
	.2byte 0x0803
.L_0802352c:
	cmp	r2, #2
	bne.n	.L_08023544
	ldrsb	r3, [r0, r2]
	ldr	r1, [pc, #156]
.L_08023534:
	lsls	r3, r3, #2
	ldrb	r2, [r1, r3]
	adds	r3, r3, r1
	str	r2, [r0, #8]
	ldrb	r2, [r3, #1]
	ldrb	r3, [r3, #2]
	str	r2, [r0, #24]
	str	r3, [r0, #28]
.L_08023544:
	add	r2, sp, #96
	ldr	r3, [r2, #12]
	ldr	r1, [r2, #8]
	cmp	r3, r1
	beq.n	.L_08023554
	str	r1, [r2, #12]
	movs	r1, #2
	mov	sl, r1
.L_08023554:
	ldr	r3, [r2, #24]
	lsls	r3, r3, #3
	str	r3, [sp, #56]
	ldr	r3, [r2, #28]
	ldr	r0, [sp, #84]
	lsls	r3, r3, #3
	str	r3, [sp, #60]
	bl	sub_080b5098
	ldr	r3, [r0, #0]
	ldr	r4, [sp, #8]
	ldr	r2, [r3, #80]
	ldr	r3, [pc, #100]
	movs	r7, #0
	str	r3, [r4, #4]
	str	r7, [r4, #8]
	ldr	r1, [pc, #96]
	ldrh	r2, [r2, #8]
	ldr	r5, [pc, #96]
	mov	r8, r1
	ldrh	r1, [r4, #8]
	adds	r3, r5, #0
	lsls	r2, r2, #22
	lsrs	r2, r2, #22
	ands	r3, r1
	orrs	r3, r2
	ldr	r2, [sp, #8]
	strh	r3, [r2, #8]
	ldr	r3, [sp, #8]
	ldr	r6, [pc, #80]
	ldrh	r2, [r3, #6]
	adds	r3, r6, #0
	ands	r3, r2
	ldr	r2, [pc, #52]
	ldr	r4, [sp, #8]
	orrs	r3, r2
	strh	r3, [r4, #6]
	movs	r3, #56
	strb	r3, [r4, #4]
	ldr	r0, [sp, #8]
	movs	r1, #240
	bl	sub_08003dec
	ldr	r3, [pc, #56]
	ldr	r1, [sp, #12]
	str	r3, [r1, #4]
	str	r7, [r1, #8]
	ldr	r0, [sp, #64]
	ldr	r1, [pc, #48]
	bl	sub_080040d0
	ldr	r4, [sp, #12]
	ldrh	r3, [r4, #8]
	mov	r2, r8
	ands	r2, r0
	ands	r5, r3
	adds	r1, r4, #0
	orrs	r5, r2
	strh	r5, [r1, #8]
	b.n	.L_080235ec
	.4byte 0x000000ac
	.4byte 0x080373b8
	.4byte 0xc0002400
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0xfffffe00
	.4byte 0x40000400
	.2byte 0x10a4
	.2byte 0x0803
.L_080235ec:
	ldr	r3, [sp, #76]
	ldr	r0, [pc, #84]
	ldrh	r2, [r3, #12]
	ldr	r3, [r0, #0]
	ldr	r4, [sp, #56]
	movs	r1, #4
	ands	r3, r1
	lsls	r2, r2, #3
	adds	r2, r4, r2
	lsrs	r3, r3, #2
	ldr	r4, [sp, #12]
	subs	r2, r2, r3
	ldr	r3, [pc, #56]
	adds	r2, #16
	ands	r2, r3
	ldrh	r3, [r4, #6]
	ands	r6, r3
	orrs	r6, r2
	adds	r2, r4, #0
	strh	r6, [r2, #6]
	ldr	r3, [sp, #76]
	ldrh	r2, [r3, #14]
	ldr	r3, [r0, #0]
	ldr	r4, [sp, #60]
	ands	r3, r1
	lsls	r2, r2, #3
	adds	r2, r4, r2
	lsrs	r3, r3, #2
	ldr	r1, [sp, #12]
	subs	r2, r2, r3
	adds	r2, #16
	strb	r2, [r1, #4]
	movs	r3, #63
	ldrb	r2, [r1, #7]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #16
	orrs	r3, r2
	strb	r3, [r1, #7]
	ldr	r0, [sp, #12]
	b.n	.L_08023648
	movs	r0, r0
	.4byte 0x000001ff
	.2byte 0x1e40
	.2byte 0x0300
.L_08023648:
	movs	r1, #241
	bl	sub_08003dec
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L_08023656
	b.n	.L_08023cfa
.L_08023656:
	ldr	r0, [sp, #84]
	bl	sub_08077008
	mov	r9, r0
	bl	sub_0801e318
	bl	sub_08016738
	movs	r3, #1
	mov	r4, sl
	ands	r3, r4
	cmp	r3, #0
	bne.n	.L_08023672
	b.n	.L_08023b42
.L_08023672:
	ldr	r0, [sp, #76]
	bl	sub_08016498
	ldr	r1, [sp, #72]
	ldr	r3, [sp, #72]
	ldrh	r0, [r1, #12]
	ldrh	r2, [r3, #8]
	ldrh	r1, [r1, #14]
	ldrh	r3, [r3, #10]
	str	r7, [sp, #0]
	bl	sub_08017248
	movs	r3, #14
	str	r3, [sp, #0]
	ldr	r0, [sp, #76]
	movs	r1, #0
	movs	r2, #14
	movs	r3, #29
	bl	sub_0801e41c
	mov	r0, r9
	ldr	r1, [sp, #76]
	movs	r2, #0
	movs	r3, #0
	bl	sub_0801e8b0
	ldr	r0, [pc, #616]
	ldr	r1, [sp, #76]
	movs	r2, #56
	movs	r3, #0
	bl	sub_0801e8b0
	mov	r4, r9
	ldr	r2, [sp, #76]
	ldrb	r0, [r4, #15]
	movs	r1, #2
	movs	r3, #72
	str	r7, [sp, #0]
	bl	sub_0801ea08
	ldr	r7, [pc, #592]
	ldr	r1, [sp, #76]
	adds	r0, r7, #0
	movs	r2, #0
	movs	r3, #8
	bl	sub_0801e7c0
	movs	r3, #146
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r0, [r3, #0]
	movs	r3, #8
	str	r3, [sp, #0]
	ldr	r2, [sp, #76]
	movs	r1, #8
	movs	r3, #40
	bl	sub_0801ea08
	ldr	r0, [pc, #560]
	ldr	r1, [sp, #76]
	movs	r2, #40
	movs	r3, #24
	bl	sub_0801e8b0
	movs	r3, #24
	mov	r2, r9
	movs	r1, #56
	ldrsh	r0, [r2, r1]
	mov	r8, r3
	str	r3, [sp, #0]
	ldr	r2, [sp, #76]
	movs	r1, #4
	movs	r3, #56
	bl	sub_0801ea08
	ldr	r5, [pc, #528]
	ldr	r1, [sp, #76]
	adds	r0, r5, #0
	movs	r2, #88
	movs	r3, #24
	bl	sub_0801e8b0
	mov	r1, r9
	mov	r2, r8
	movs	r4, #52
	ldrsh	r0, [r1, r4]
	movs	r3, #96
	str	r2, [sp, #0]
	movs	r1, #4
	ldr	r2, [sp, #76]
	bl	sub_0801ea08
	movs	r3, #32
	ldr	r1, [sp, #76]
	ldr	r0, [pc, #496]
	movs	r2, #40
	bl	sub_0801e8b0
	mov	r4, r9
	ldr	r2, [sp, #76]
	movs	r3, #58
	ldrsh	r0, [r4, r3]
	movs	r6, #32
	movs	r1, #4
	movs	r3, #56
	str	r6, [sp, #0]
	bl	sub_0801ea08
	adds	r0, r5, #0
	ldr	r1, [sp, #76]
	movs	r2, #88
	movs	r3, #32
	bl	sub_0801e8b0
	mov	r2, r9
	movs	r1, #54
	ldrsh	r0, [r2, r1]
	movs	r3, #96
	ldr	r2, [sp, #76]
	movs	r1, #4
	str	r6, [sp, #0]
	bl	sub_0801ea08
	adds	r0, r7, #0
	ldr	r1, [sp, #76]
	subs	r0, #10
	movs	r2, #136
	movs	r3, #16
	bl	sub_0801e7c0
	mov	r3, r9
	ldrh	r0, [r3, #60]
	movs	r3, #16
	str	r3, [sp, #0]
	ldr	r2, [sp, #76]
	movs	r1, #3
	movs	r3, #184
	bl	sub_0801ea08
	adds	r0, r7, #0
	ldr	r1, [sp, #76]
	subs	r0, #9
	movs	r2, #136
	movs	r3, #24
	bl	sub_0801e7c0
	mov	r4, r9
	mov	r1, r8
	ldrh	r0, [r4, #62]
	ldr	r2, [sp, #76]
	str	r1, [sp, #0]
	movs	r3, #184
	movs	r1, #3
	bl	sub_0801ea08
	adds	r0, r7, #0
	ldr	r1, [sp, #76]
	subs	r0, #8
	movs	r2, #136
	movs	r3, #32
	bl	sub_0801e7c0
	mov	r3, r9
	adds	r3, #64
	ldr	r2, [sp, #76]
	ldrh	r0, [r3, #0]
	movs	r1, #3
	movs	r3, #184
	str	r6, [sp, #0]
	bl	sub_0801ea08
	subs	r0, r7, #7
	ldr	r1, [sp, #76]
	movs	r2, #136
	movs	r3, #40
	bl	sub_0801e7c0
	mov	r3, r9
	adds	r3, #66
	ldrb	r0, [r3, #0]
	movs	r3, #40
	str	r3, [sp, #0]
	ldr	r2, [sp, #76]
	movs	r1, #3
	movs	r3, #184
	bl	sub_0801ea08
	ldr	r3, [pc, #312]
	add	r3, r9
	ldrb	r0, [r3, #0]
	ldr	r3, [pc, #312]
	movs	r2, #0
	adds	r0, r0, r3
	ldr	r1, [sp, #76]
	movs	r3, #48
	bl	sub_0801e7c0
	ldr	r2, [sp, #44]
	cmp	r2, #0
	beq.n	.L_0802380e
	subs	r0, r7, #1
	ldr	r1, [sp, #76]
	movs	r2, #0
	movs	r3, #72
	bl	sub_0801e7c0
.L_0802380e:
	ldr	r1, [sp, #76]
	subs	r0, r7, #5
	movs	r2, #0
	movs	r3, #80
	bl	sub_0801e7c0
	subs	r0, r7, #4
	ldr	r1, [sp, #76]
	movs	r2, #0
	movs	r3, #88
	bl	sub_0801e7c0
	movs	r6, #140
	subs	r0, r7, #3
	ldr	r1, [sp, #76]
	movs	r2, #0
	movs	r3, #96
	bl	sub_0801e7c0
	lsls	r6, r6, #1
	movs	r3, #72
	movs	r1, #40
	movs	r2, #7
	movs	r4, #0
	mov	fp, r3
	mov	r8, r1
	add	r6, r9
	movs	r7, #48
	mov	sl, r2
.L_08023848:
	ldr	r1, [sp, #44]
	movs	r3, #1
	cmp	r1, #0
	beq.n	.L_08023852
	movs	r3, #0
.L_08023852:
	ldr	r2, [pc, #216]
	adds	r1, r4, r2
	movs	r2, #0
	adds	r3, #8
	str	r2, [sp, #0]
	ldr	r0, [sp, #76]
	mov	r2, sl
	lsls	r5, r4, #2
	str	r4, [sp, #4]
	bl	sub_08019000
	ldr	r3, [sp, #44]
	ldr	r4, [sp, #4]
	cmp	r3, #0
	beq.n	.L_080238a0
	mov	r1, fp
	ldr	r2, [sp, #76]
	ldrb	r0, [r6, #4]
	mov	r3, r8
	str	r1, [sp, #0]
	movs	r1, #1
	bl	sub_0801ea08
	ldr	r0, [pc, #152]
	ldr	r1, [sp, #76]
	adds	r2, r7, #0
	movs	r3, #72
	bl	sub_0801e8b0
	mov	r2, fp
	adds	r3, r7, #0
	ldrb	r0, [r6, #0]
	adds	r3, #8
	str	r2, [sp, #0]
	movs	r1, #1
	ldr	r2, [sp, #76]
	bl	sub_0801ea08
	ldr	r4, [sp, #4]
.L_080238a0:
	adds	r1, r4, #0
	ldr	r0, [sp, #84]
	str	r4, [sp, #4]
	bl	sub_080771f8
	movs	r3, #80
	str	r3, [sp, #0]
	ldr	r2, [sp, #76]
	movs	r1, #2
	adds	r3, r7, #0
	bl	sub_0801ea08
	adds	r5, #72
	mov	r3, r9
	ldrsh	r0, [r3, r5]
	movs	r3, #88
	ldr	r2, [sp, #76]
	str	r3, [sp, #0]
	movs	r1, #3
	mov	r3, r8
	bl	sub_0801ea08
	add	r5, r9
	movs	r3, #96
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	movs	r1, #3
	str	r3, [sp, #0]
	ldr	r2, [sp, #76]
	mov	r3, r8
	bl	sub_0801ea08
	ldr	r4, [sp, #4]
	movs	r3, #32
	movs	r1, #4
	adds	r4, #1
	add	r8, r3
	adds	r6, #1
	adds	r7, #32
	add	sl, r1
	cmp	r4, #3
	ble.n	.L_08023848
	mov	r4, r9
	movs	r2, #56
	ldrsh	r3, [r4, r2]
	movs	r6, #0
	cmp	r3, #0
	bne.n	.L_08023908
	ldr	r1, [sp, #20]
	movs	r3, #16
	strb	r3, [r1, #0]
	movs	r6, #1
.L_08023908:
	ldr	r3, [sp, #20]
	adds	r2, r6, r3
	b.n	.L_08023aa0
	movs	r0, r0
	.4byte 0x080373d8
	.4byte 0x000008ba
	.4byte 0x080373dc
	.4byte 0x080373e0
	.4byte 0x080373e4
	.4byte 0x00000129
	.4byte 0x00000741
	.2byte 0x5001
	.2byte 0x0000
.L_08023930:
	ldr	r1, [pc, #812]
	add	r1, r9
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	cmp	r3, #1
	bne.n	.L_08023942
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08023942:
	cmp	r6, #7
	ble.n	.L_08023948
	b.n	.L_08023ad4
.L_08023948:
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	cmp	r3, #2
	bne.n	.L_08023956
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08023956:
	cmp	r6, #7
	ble.n	.L_0802395c
	b.n	.L_08023ad4
.L_0802395c:
	ldr	r3, [pc, #772]
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0802396e
	movs	r3, #4
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_0802396e:
	cmp	r6, #7
	ble.n	.L_08023974
	b.n	.L_08023ad4
.L_08023974:
	ldr	r3, [pc, #752]
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08023986
	movs	r3, #3
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08023986:
	cmp	r6, #7
	ble.n	.L_0802398c
	b.n	.L_08023ad4
.L_0802398c:
	movs	r3, #158
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080239a0
	movs	r3, #5
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_080239a0:
	cmp	r6, #7
	ble.n	.L_080239a6
	b.n	.L_08023ad4
.L_080239a6:
	movs	r3, #160
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080239ba
	movs	r3, #7
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_080239ba:
	cmp	r6, #7
	ble.n	.L_080239c0
	b.n	.L_08023ad4
.L_080239c0:
	movs	r3, #156
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080239d4
	movs	r3, #6
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_080239d4:
	cmp	r6, #7
	bgt.n	.L_08023ad4
	movs	r3, #153
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08023a0a
	ldr	r3, [pc, #644]
	add	r3, r9
	ldrb	r1, [r3, #0]
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_080239fc
	movs	r3, #9
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_080239fc:
	lsls	r3, r1, #24
	cmp	r3, #0
	bge.n	.L_08023a0a
	movs	r3, #10
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08023a0a:
	cmp	r6, #7
	bgt.n	.L_08023ad4
	movs	r3, #154
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08023a40
	ldr	r3, [pc, #596]
	add	r3, r9
	ldrb	r1, [r3, #0]
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_08023a32
	movs	r3, #11
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08023a32:
	lsls	r3, r1, #24
	cmp	r3, #0
	bge.n	.L_08023a40
	movs	r3, #12
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08023a40:
	cmp	r6, #7
	bgt.n	.L_08023ad4
	movs	r3, #155
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08023a76
	ldr	r3, [pc, #544]
	add	r3, r9
	ldrb	r1, [r3, #0]
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_08023a68
	movs	r3, #13
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08023a68:
	lsls	r3, r1, #24
	cmp	r3, #0
	bge.n	.L_08023a76
	movs	r3, #14
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08023a76:
	cmp	r6, #7
	bgt.n	.L_08023ad4
	ldr	r3, [pc, #508]
	add	r3, r9
	ldrb	r1, [r3, #0]
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_08023a92
	movs	r3, #17
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08023a92:
	lsls	r3, r1, #24
	cmp	r3, #0
	bge.n	.L_08023ad4
	movs	r3, #18
	strb	r3, [r2, #0]
	adds	r6, #1
	b.n	.L_08023ad4
.L_08023aa0:
	movs	r3, #152
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_08023ab8
	movs	r3, #15
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08023ab8:
	cmp	r6, #7
	bgt.n	.L_08023ad4
	ldr	r3, [pc, #444]
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08023ace
	movs	r3, #8
	strb	r3, [r2, #0]
	adds	r6, #1
	adds	r2, #1
.L_08023ace:
	cmp	r6, #7
	bgt.n	.L_08023ad4
	b.n	.L_08023930
.L_08023ad4:
	cmp	r6, #0
	ble.n	.L_08023af6
	ldr	r7, [sp, #16]
	ldr	r5, [sp, #20]
	adds	r4, r6, #0
.L_08023ade:
	ldrb	r0, [r5, #0]
	lsls	r0, r0, #24
	asrs	r0, r0, #24
	ldmia	r7!, {r1}
	str	r4, [sp, #4]
	bl	sub_08021ab0
	ldr	r4, [sp, #4]
	subs	r4, #1
	adds	r5, #1
	cmp	r4, #0
	bne.n	.L_08023ade
.L_08023af6:
	cmp	r6, #0
	bne.n	.L_08023b00
	ldr	r4, [sp, #20]
	strb	r6, [r4, #0]
	movs	r6, #1
.L_08023b00:
	cmp	r6, #10
	bgt.n	.L_08023b1c
	ldr	r3, [sp, #20]
	movs	r2, #1
	negs	r2, r2
	adds	r1, r2, #0
	adds	r2, r6, r3
	movs	r3, #11
	subs	r4, r3, r6
.L_08023b12:
	subs	r4, #1
	strb	r1, [r2, #0]
	adds	r2, #1
	cmp	r4, #0
	bne.n	.L_08023b12
.L_08023b1c:
	str	r6, [sp, #48]
	ldr	r4, [sp, #20]
	movs	r3, #0
	ldrsb	r3, [r4, r3]
	cmp	r3, #0
	bne.n	.L_08023b42
	mov	r2, r9
	movs	r1, #56
	ldrsh	r3, [r2, r1]
	cmp	r3, #0
	beq.n	.L_08023b36
	ldr	r0, [pc, #332]
	b.n	.L_08023b38
.L_08023b36:
	ldr	r0, [pc, #332]
.L_08023b38:
	ldr	r1, [sp, #76]
	movs	r2, #112
	movs	r3, #0
	bl	sub_0801e7c0
.L_08023b42:
	movs	r0, #128
	lsls	r0, r0, #1
	bl	sub_08004938
	ldr	r3, [sp, #104]
	str	r0, [sp, #36]
	cmp	r3, #13
	bhi.n	.L_08023b54
	b.n	.L_08023c94
.L_08023b54:
	ldr	r4, [sp, #20]
	subs	r3, #14
	ldrsb	r3, [r4, r3]
	str	r3, [sp, #32]
	cmp	r3, #0
	bne.n	.L_08023b6e
	mov	r2, r9
	movs	r1, #56
	ldrsh	r3, [r2, r1]
	cmp	r3, #0
	bne.n	.L_08023b6e
	movs	r3, #16
	str	r3, [sp, #32]
.L_08023b6e:
	movs	r4, #166
	lsls	r4, r4, #1
	mov	r8, r4
	mov	r0, r8
	bl	sub_08004938
	movs	r4, #0
	str	r4, [sp, #4]
	mov	r1, r9
	mov	r2, r8
	ldr	r3, [pc, #260]
	adds	r6, r0, #0
	bl	sub_080072f0
	ldr	r3, [pc, #224]
	ldr	r4, [sp, #4]
	mov	r5, r9
	adds	r5, #64
	add	r3, r9
	mov	r1, r9
	mov	r2, r9
	ldrh	r1, [r1, #60]
	ldrh	r2, [r2, #62]
	ldrh	r7, [r5, #0]
	strb	r4, [r3, #0]
	ldr	r3, [pc, #204]
	add	r3, r9
	strb	r4, [r3, #0]
	ldr	r3, [pc, #208]
	add	r3, r9
	strb	r4, [r3, #0]
	ldr	r0, [sp, #84]
	mov	sl, r1
	mov	fp, r2
	bl	sub_08077010
	mov	r1, r9
	ldrh	r3, [r1, #60]
	mov	r2, sl
	subs	r2, r2, r3
	ldrh	r3, [r1, #62]
	mov	r1, fp
	subs	r1, r1, r3
	ldrh	r3, [r5, #0]
	mov	sl, r2
	mov	fp, r1
	mov	r2, r8
	adds	r1, r6, #0
	subs	r7, r7, r3
	mov	r0, r9
	ldr	r3, [pc, #180]
	bl	sub_080072f0
	adds	r0, r6, #0
	bl	sub_08002df0
	ldr	r3, [sp, #32]
	subs	r3, #8
	ldr	r4, [sp, #4]
	cmp	r3, #10
	bhi.n	.L_08023c4e
	ldr	r2, [pc, #160]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x08023c1c
	.4byte 0x08023c24
	.4byte 0x08023c28
	.4byte 0x08023c2e
	.4byte 0x08023c32
	.4byte 0x08023c40
	.4byte 0x08023c40
	.4byte 0x08023c4e
	.4byte 0x08023c4e
	.4byte 0x08023c38
	.2byte 0x3c3c
	.2byte 0x0802
	ldr	r3, [pc, #92]
	add	r3, r9
	ldrb	r4, [r3, #0]
	b.n	.L_08023c4e
	mov	r4, sl
	b.n	.L_08023c4e
	mov	r1, sl
	negs	r4, r1
	b.n	.L_08023c4e
	mov	r4, fp
	b.n	.L_08023c4e
	mov	r2, fp
	negs	r4, r2
	b.n	.L_08023c4e
	adds	r4, r7, #0
	b.n	.L_08023c4e
	negs	r4, r7
	b.n	.L_08023c4e
	ldr	r3, [pc, #48]
	add	r3, r9
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r4, r3, #2
.L_08023c4e:
	adds	r0, r4, #0
	movs	r1, #5
	bl	sub_08019908
	ldr	r0, [pc, #56]
	ldr	r3, [sp, #32]
	ldr	r1, [sp, #36]
	adds	r0, r3, r0
	b.n	.L_08023cbe
	.4byte 0x00000131
	.4byte 0x0000013d
	.4byte 0x0000013b
	.4byte 0x00000133
	.4byte 0x00000135
	.4byte 0x00000137
	.4byte 0x00000147
	.4byte 0x00000141
	.4byte 0x000008bd
	.4byte 0x000008be
	.4byte 0x03001388
	.4byte 0x08023bf0
	.2byte 0x08d2
	.2byte 0x0000
.L_08023c94:
	cmp	r3, #2
	bne.n	.L_08023ccc
	mov	r4, r9
	ldrb	r3, [r4, #15]
	cmp	r3, #98
	bhi.n	.L_08023ccc
	adds	r1, r3, #0
	adds	r1, #1
	ldr	r0, [sp, #84]
	bl	sub_08077258
	movs	r3, #146
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r3, [r3, #0]
	movs	r1, #5
	subs	r0, r0, r3
	bl	sub_08019908
	ldr	r0, [pc, #12]
	ldr	r1, [sp, #36]
.L_08023cbe:
	movs	r2, #128
	bl	sub_0801965c
	b.n	.L_08023cda
	movs	r0, r0
	.2byte 0x08bf
	.2byte 0x0000
.L_08023ccc:
	ldr	r0, [sp, #104]
	ldr	r3, [pc, #128]
	ldr	r1, [sp, #36]
	adds	r0, r0, r3
	movs	r2, #128
	bl	sub_0801965c
.L_08023cda:
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #72]
	movs	r2, #0
	movs	r3, #4
	bl	sub_08017aa4
	ldr	r0, [sp, #36]
	bl	sub_08002df0
	ldr	r4, [pc, #100]
	ldr	r3, [sp, #80]
	adds	r2, r3, r4
	movs	r3, #1
	movs	r1, #0
	strb	r3, [r2, #0]
	mov	sl, r1
.L_08023cfa:
	ldr	r1, [pc, #92]
	ldr	r5, [sp, #24]
	ldr	r7, [sp, #16]
	movs	r6, #0
	mov	r8, r1
	movs	r4, #112
.L_08023d06:
	ldr	r3, [pc, #84]
	str	r3, [r5, #4]
	movs	r3, #0
	str	r3, [r5, #8]
	ldmia	r7!, {r3}
	lsls	r3, r3, #2
	add	r3, r8
	ldrh	r2, [r3, #2]
	ldr	r1, [pc, #44]
	ldrh	r3, [r5, #8]
	lsls	r2, r2, #17
	lsrs	r2, r2, #22
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #8]
	ldr	r3, [pc, #32]
	adds	r1, r4, #0
	ands	r1, r3
	ldr	r2, [pc, #32]
	ldrh	r3, [r5, #6]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r5, #6]
	ldr	r2, [sp, #76]
	ldrh	r3, [r2, #14]
	lsls	r3, r3, #3
	adds	r3, #8
	strb	r3, [r5, #4]
	ldr	r1, [sp, #20]
	ldrsb	r3, [r1, r6]
	b.n	.L_08023d60
	.4byte 0xfffffc00
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x000008c0
	.4byte 0x00000ea3
	.4byte 0x03001b10
	.2byte 0x0400
	.2byte 0x4000
.L_08023d60:
	cmp	r3, #0
	ble.n	.L_08023d70
	adds	r0, r5, #0
	movs	r1, #240
	str	r4, [sp, #4]
	bl	sub_08003dec
	ldr	r4, [sp, #4]
.L_08023d70:
	adds	r6, #1
	adds	r4, #15
	adds	r5, #12
	cmp	r6, #10
	ble.n	.L_08023d06
	ldr	r3, [pc, #232]
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #76]
	cmp	r3, #0
	beq.n	.L_08023e00
	ldr	r3, [pc, #224]
	ldr	r3, [r3, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08023e00
	ldr	r2, [sp, #88]
	cmp	r2, #0
	beq.n	.L_08023df6
	movs	r3, #128
	ldr	r4, [sp, #40]
	lsls	r3, r3, #1
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_08023dc6
	ldr	r1, [sp, #68]
	adds	r1, #1
	str	r1, [sp, #68]
	cmp	r1, r2
	blt.n	.L_08023db0
	movs	r2, #0
	str	r2, [sp, #68]
.L_08023db0:
	ldr	r4, [sp, #68]
	ldr	r1, [sp, #92]
	lsls	r3, r4, #1
	ldrh	r3, [r3, r1]
	movs	r2, #1
	movs	r0, #111
	str	r3, [sp, #84]
	mov	sl, r2
	bl	sub_080f9010
	b.n	.L_08023df6
.L_08023dc6:
	movs	r3, #128
	ldr	r4, [sp, #40]
	lsls	r3, r3, #2
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_08023df6
	ldr	r1, [sp, #68]
	subs	r1, #1
	str	r1, [sp, #68]
	cmp	r1, #0
	bge.n	.L_08023de2
	ldr	r2, [sp, #88]
	subs	r2, #1
	str	r2, [sp, #68]
.L_08023de2:
	ldr	r4, [sp, #68]
	ldr	r1, [sp, #92]
	lsls	r3, r4, #1
	ldrh	r3, [r3, r1]
	movs	r2, #1
	movs	r0, #111
	str	r3, [sp, #84]
	mov	sl, r2
	bl	sub_080f9010
.L_08023df6:
	movs	r0, #1
	bl	sub_080030f8
	bl	.L_080232a0
.L_08023e00:
	ldr	r5, [sp, #16]
	movs	r6, #10
.L_08023e04:
	ldmia	r5!, {r0}
	subs	r6, #1
	bl	sub_08003f3c
	cmp	r6, #0
	bge.n	.L_08023e04
	ldr	r0, [sp, #52]
	bl	sub_08003f3c
	ldr	r0, [sp, #64]
	bl	sub_08003f3c
	movs	r0, #1
	bl	sub_080030f8
	bl	sub_0801e318
	movs	r1, #1
	ldr	r0, [sp, #76]
	bl	sub_08016418
	movs	r1, #1
	ldr	r0, [sp, #72]
	bl	sub_08016418
	ldr	r5, [pc, #52]
	ldr	r3, [r5, #0]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	adds	r5, #192
	bl	sub_0801f200
	ldr	r2, [r5, #0]
	movs	r3, #0
	str	r3, [r2, #72]
	movs	r0, #1
	bl	sub_080030f8
	movs	r0, #0
	add	sp, #352
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f34
	.4byte 0x03001c94
	.2byte 0x1e74
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #224
	str	r0, [sp, #76]
	ldr	r5, [pc, #768]
	movs	r3, #1
	ldr	r1, [r5, #0]
	movs	r2, #0
	negs	r3, r3
	movs	r0, #128
	str	r1, [sp, #72]
	str	r2, [sp, #68]
	str	r3, [sp, #64]
	bl	sub_080040b4
	str	r0, [sp, #60]
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_08004970
	ldr	r2, [sp, #64]
	movs	r1, #0
	movs	r3, #42
	movs	r6, #0
	str	r0, [sp, #56]
	str	r1, [sp, #52]
	str	r2, [sp, #48]
	str	r3, [sp, #0]
	movs	r1, #4
	movs	r2, #30
	movs	r3, #4
	movs	r0, #0
	str	r6, [sp, #40]
	str	r6, [sp, #32]
	str	r6, [sp, #80]
	str	r6, [sp, #28]
	str	r6, [sp, #24]
	bl	sub_080162d4
	str	r0, [sp, #44]
	movs	r0, #1
	bl	sub_0801e3c8
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r1, #9
	movs	r2, #9
	movs	r3, #11
	movs	r0, #21
	bl	sub_080162d4
	mov	r9, r0
	adds	r5, #168
	ldr	r3, [r5, #0]
	ldr	r1, [r3, #52]
	ldr	r2, [r3, #48]
	ldr	r3, [r3, #56]
	mov	fp, r1
	mov	sl, r2
	str	r3, [sp, #36]
	ldr	r0, [sp, #76]
	bl	sub_08077008
	adds	r0, #248
	movs	r7, #0
	mov	r8, r0
.L_08023efe:
	ldr	r1, [sp, #52]
	ldr	r2, [sp, #56]
	lsls	r3, r1, #2
	movs	r6, #0
	adds	r5, r3, r2
.L_08023f08:
	mov	r1, r8
	movs	r2, #1
	ldr	r3, [r1, #16]
	lsls	r2, r6
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08023f24
	lsls	r3, r7, #8
	orrs	r3, r6
	stmia	r5!, {r3}
	ldr	r2, [sp, #52]
	adds	r2, #1
	str	r2, [sp, #52]
	b.n	.L_08023fba
.L_08023f24:
	mov	r1, r8
	ldr	r3, [r1, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08023fba
	ldr	r2, [sp, #76]
	movs	r0, #0
	cmp	r2, #7
	bls.n	.L_08023f38
	movs	r0, #1
.L_08023f38:
	bl	sub_08077000
	movs	r2, #132
	adds	r3, r0, #0
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	movs	r1, #0
	adds	r0, #8
	movs	r4, #0
	cmp	r1, r3
	bge.n	.L_08023f94
	ldrb	r3, [r0, #2]
	ldr	r2, [sp, #76]
	cmp	r3, r2
	bne.n	.L_08023f64
	ldrb	r3, [r0, #0]
	cmp	r3, r7
	bne.n	.L_08023f64
	ldrb	r3, [r0, #1]
	cmp	r3, r6
	beq.n	.L_08023f8e
.L_08023f64:
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r3, r0, r2
	ldr	r3, [r3, #0]
	adds	r1, #1
	cmp	r1, r3
	bge.n	.L_08023f92
	lsls	r4, r1, #2
	adds	r2, r0, r4
	ldrb	r3, [r2, #2]
	mov	ip, r3
	ldr	r3, [sp, #76]
	cmp	ip, r3
	bne.n	.L_08023f64
	ldrb	r3, [r2, #0]
	cmp	r3, r7
	bne.n	.L_08023f64
	ldrb	r3, [r2, #1]
	cmp	r3, r6
	bne.n	.L_08023f64
	b.n	.L_08023f94
.L_08023f8e:
	movs	r4, #0
	b.n	.L_08023f94
.L_08023f92:
	lsls	r4, r1, #2
.L_08023f94:
	lsls	r2, r7, #8
	movs	r3, #128
	lsls	r3, r3, #9
	orrs	r2, r6
	orrs	r2, r3
	str	r2, [r5, #0]
	adds	r3, r0, r4
	ldrb	r3, [r3, #3]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L_08023fb2
	lsls	r3, r3, #17
	orrs	r2, r3
	str	r2, [r5, #0]
.L_08023fb2:
	ldr	r1, [sp, #52]
	adds	r1, #1
	str	r1, [sp, #52]
	adds	r5, #4
.L_08023fba:
	adds	r6, #1
	cmp	r6, #19
	ble.n	.L_08023f08
	movs	r2, #4
	adds	r7, #1
	add	r8, r2
	cmp	r7, #3
	ble.n	.L_08023efe
	ldr	r3, [sp, #52]
	ldr	r1, [sp, #56]
	lsls	r2, r3, #2
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r2, r1]
	ldr	r3, [sp, #72]
	ldr	r1, [pc, #428]
	adds	r2, r3, r1
	movs	r3, #1
	strb	r3, [r2, #0]
	mov	r2, sp
	adds	r2, #212
	str	r2, [sp, #4]
	ldr	r1, [sp, #72]
	ldr	r2, [pc, #412]
	mov	r3, sp
	adds	r3, #84
	adds	r2, r1, r2
	str	r3, [sp, #12]
	str	r2, [sp, #8]
.L_08023ff4:
	ldr	r3, [sp, #48]
	cmp	fp, r3
	bne.n	.L_08024008
	ldr	r1, [sp, #64]
	cmp	sl, r1
	bne.n	.L_08024008
	ldr	r2, [sp, #28]
	cmp	r2, #0
	bne.n	.L_08024008
	b.n	.L_08024328
.L_08024008:
	mov	r3, fp
	ldr	r1, [sp, #56]
	add	r3, sl
	lsls	r3, r3, #2
	ldr	r5, [r3, r1]
	ldr	r3, [sp, #72]
	ldr	r1, [pc, #372]
	movs	r2, #0
	str	r2, [sp, #40]
	adds	r2, r3, r1
	movs	r3, #1
	strb	r3, [r2, #0]
	mov	r2, r9
	ldrh	r0, [r2, #12]
	ldrh	r1, [r2, #14]
	ldr	r2, [sp, #64]
	lsls	r3, r2, #1
	adds	r1, r1, r3
	mov	r3, r9
	ldrh	r2, [r3, #8]
	movs	r3, #15
	adds	r1, #1
	str	r3, [sp, #0]
	adds	r0, #1
	subs	r2, #2
	movs	r3, #1
	bl	sub_08022768
	ldr	r1, [sp, #32]
	cmp	r1, #0
	beq.n	.L_08024064
	ldr	r0, [sp, #44]
	movs	r1, #1
	bl	sub_08016418
	movs	r3, #42
	str	r3, [sp, #0]
	movs	r0, #0
	movs	r1, #4
	movs	r2, #30
	movs	r3, #4
	bl	sub_080162d4
	str	r0, [sp, #44]
	bl	sub_08016738
.L_08024064:
	ldr	r3, [sp, #52]
	movs	r2, #0
	str	r2, [sp, #28]
	cmp	r3, #0
	bne.n	.L_08024070
	b.n	.L_080241a0
.L_08024070:
	bl	sub_080198dc
	movs	r1, #0
	str	r1, [sp, #24]
	ldr	r0, [pc, #276]
	ldr	r3, [r0, #0]
	movs	r2, #228
	ldr	r3, [r3, r2]
	ldr	r1, [sp, #28]
	cmp	r3, r5
	bne.n	.L_0802408c
	movs	r2, #1
	str	r2, [sp, #24]
	b.n	.L_080240a2
.L_0802408c:
	adds	r1, #1
	cmp	r1, #7
	bgt.n	.L_080240a2
	ldr	r3, [r0, #0]
	lsls	r2, r1, #2
	adds	r2, #228
	ldr	r3, [r3, r2]
	cmp	r3, r5
	bne.n	.L_0802408c
	movs	r3, #1
	str	r3, [sp, #24]
.L_080240a2:
	ldr	r1, [sp, #24]
	cmp	r1, #0
	beq.n	.L_080240ca
	ldr	r6, [sp, #12]
	movs	r2, #52
	ldr	r0, [pc, #228]
	adds	r1, r6, #0
	bl	sub_0801965c
	ldr	r2, [sp, #68]
	cmp	r2, #0
	beq.n	.L_080241ac
	adds	r0, r2, #0
	movs	r1, #1
	bl	sub_08016418
	movs	r3, #0
	str	r3, [sp, #68]
	str	r3, [sp, #32]
	b.n	.L_080241ac
.L_080240ca:
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_0802414c
	movs	r0, #248
	lsls	r0, r0, #14
	ands	r0, r5
	cmp	r0, #0
	beq.n	.L_0802411e
	lsrs	r0, r0, #17
	movs	r1, #5
	bl	sub_08019908
	movs	r3, #240
	lsls	r3, r3, #4
	ands	r3, r5
	lsrs	r3, r3, #8
	lsls	r0, r3, #2
	adds	r0, r0, r3
	movs	r3, #255
	ands	r5, r3
	ldr	r6, [sp, #12]
	ldr	r3, [pc, #156]
	lsls	r0, r0, #2
	adds	r0, r0, r5
	adds	r1, r6, #0
	adds	r0, r0, r3
	movs	r2, #52
	bl	sub_0801965c
	ldr	r1, [sp, #68]
	cmp	r1, #0
	beq.n	.L_080241ac
	adds	r0, r1, #0
	movs	r1, #1
	bl	sub_08016418
	movs	r2, #0
	str	r2, [sp, #68]
	str	r2, [sp, #32]
	b.n	.L_080241ac
.L_0802411e:
	add	r3, sp, #80
	str	r3, [sp, #0]
	ldr	r1, [sp, #76]
	adds	r2, r5, #0
	ldr	r3, [sp, #32]
	ldr	r0, [sp, #68]
	bl	.L_08022b44
	ldr	r6, [sp, #12]
	str	r0, [sp, #68]
	adds	r1, r6, #0
	ldr	r0, [pc, #100]
	movs	r2, #52
	bl	sub_0801965c
	movs	r3, #240
	lsls	r3, r3, #4
	ands	r5, r3
	lsrs	r3, r5, #8
	movs	r1, #1
	lsls	r1, r3
	str	r1, [sp, #40]
	b.n	.L_080241ac
.L_0802414c:
	add	r3, sp, #80
	adds	r2, r5, #0
	str	r3, [sp, #0]
	ldr	r1, [sp, #76]
	ldr	r3, [sp, #32]
	ldr	r0, [sp, #68]
	bl	.L_08022b44
	movs	r3, #240
	lsls	r3, r3, #4
	ands	r3, r5
	lsrs	r3, r3, #8
	str	r0, [sp, #68]
	lsls	r0, r3, #2
	adds	r0, r0, r3
	movs	r3, #255
	ands	r5, r3
	lsls	r0, r0, #2
	ldr	r3, [pc, #36]
	ldr	r6, [sp, #12]
	adds	r0, r0, r5
	adds	r0, r0, r3
	adds	r1, r6, #0
	movs	r2, #52
	bl	sub_0801965c
	b.n	.L_080241ac
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x00000ea3
	.4byte 0x00000ea6
	.4byte 0x03001f34
	.4byte 0x000008ef
	.4byte 0x00000666
	.2byte 0x0899
	.2byte 0x0000
.L_080241a0:
	ldr	r6, [sp, #12]
	ldr	r0, [pc, #824]
	adds	r1, r6, #0
	movs	r2, #52
	bl	sub_0801965c
.L_080241ac:
	ldr	r2, [sp, #72]
	ldr	r3, [pc, #816]
	movs	r1, #0
	adds	r5, r2, r3
	strb	r1, [r5, #0]
	ldr	r2, [sp, #32]
	cmp	r2, #0
	bne.n	.L_080241e0
	movs	r3, #1
	strb	r3, [r5, #0]
	ldr	r0, [sp, #44]
	movs	r1, #1
	bl	sub_08016418
	movs	r3, #42
	str	r3, [sp, #0]
	movs	r0, #0
	movs	r3, #4
	movs	r1, #4
	movs	r2, #30
	bl	sub_080162d4
	str	r0, [sp, #44]
	add	r3, sp, #32
	ldrb	r3, [r3, #0]
	strb	r3, [r5, #0]
.L_080241e0:
	ldr	r1, [sp, #44]
	movs	r2, #0
	adds	r0, r6, #0
	movs	r3, #4
	bl	sub_08017aa4
	ldr	r2, [sp, #48]
	mov	r1, sl
	str	r1, [sp, #64]
	cmp	fp, r2
	beq.n	.L_080242ae
	mov	r0, r9
	bl	sub_08016498
	mov	r1, fp
	ldr	r2, [sp, #56]
	lsls	r3, r1, #2
	adds	r3, r3, r2
	movs	r1, #128
	ldr	r6, [r3, #0]
	lsls	r1, r1, #24
	movs	r7, #0
	cmp	r6, r1
	beq.n	.L_080242a8
	mov	r8, r3
.L_08024212:
	movs	r2, #240
	lsls	r2, r2, #4
	adds	r1, r6, #0
	ands	r1, r2
	ldr	r3, [pc, #712]
	lsrs	r1, r1, #8
	adds	r1, r1, r3
	movs	r2, #0
	lsls	r3, r7, #1
	mov	r0, r9
	str	r2, [sp, #0]
	bl	sub_08019000
	movs	r3, #248
	lsls	r3, r3, #14
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_0802423e
	movs	r0, #4
	bl	sub_0801e71c
	b.n	.L_0802424e
.L_0802423e:
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_0802424e
	movs	r0, #2
	bl	sub_0801e71c
.L_0802424e:
	movs	r1, #240
	lsls	r1, r1, #4
	adds	r3, r6, #0
	ands	r3, r1
	lsrs	r3, r3, #8
	lsls	r0, r3, #2
	adds	r0, r0, r3
	movs	r3, #255
	ands	r3, r6
	lsls	r0, r0, #2
	adds	r0, r0, r3
	ldr	r3, [pc, #640]
	lsls	r5, r7, #4
	adds	r0, r0, r3
	mov	r1, r9
	movs	r2, #8
	adds	r3, r5, #0
	bl	sub_0801e7c0
	movs	r0, #248
	lsls	r0, r0, #14
	ands	r0, r6
	cmp	r0, #0
	beq.n	.L_0802428c
	lsrs	r0, r0, #17
	movs	r1, #1
	mov	r2, r9
	movs	r3, #48
	str	r5, [sp, #0]
	bl	sub_0801e9d4
.L_0802428c:
	movs	r0, #15
	adds	r7, #1
	bl	sub_0801e71c
	cmp	r7, #4
	bgt.n	.L_080242a8
	movs	r2, #4
	add	r8, r2
	mov	r3, r8
	movs	r1, #128
	ldr	r6, [r3, #0]
	lsls	r1, r1, #24
	cmp	r6, r1
	bne.n	.L_08024212
.L_080242a8:
	mov	ip, fp
	mov	r2, ip
	str	r2, [sp, #48]
.L_080242ae:
	ldr	r1, [sp, #52]
	cmp	r1, #5
	ble.n	.L_080242f8
	movs	r7, #0
	adds	r1, #4
	mov	r8, r1
	b.n	.L_080242ea
.L_080242bc:
	ldr	r2, [pc, #556]
	mov	r0, fp
	movs	r1, #5
	adds	r6, r7, r2
	bl	sub_080022ec
	cmp	r7, r0
	bne.n	.L_080242d0
	ldr	r3, [pc, #544]
	adds	r6, r7, r3
.L_080242d0:
	mov	r1, r9
	ldrh	r2, [r1, #8]
	subs	r2, r2, r5
	adds	r2, r2, r7
	movs	r3, #0
	str	r3, [sp, #0]
	subs	r2, #2
	mov	r0, r9
	adds	r1, r6, #0
	subs	r3, #1
	bl	sub_08019000
	adds	r7, #1
.L_080242ea:
	mov	r0, r8
	movs	r1, #5
	bl	sub_080022ec
	adds	r5, r0, #0
	cmp	r7, r5
	blt.n	.L_080242bc
.L_080242f8:
	mov	r1, r9
	ldrh	r0, [r1, #12]
	mov	r2, sl
	ldrh	r1, [r1, #14]
	lsls	r3, r2, #1
	adds	r1, r1, r3
	mov	r3, r9
	ldrh	r2, [r3, #8]
	movs	r3, #14
	adds	r1, #1
	subs	r2, #2
	str	r3, [sp, #0]
	adds	r0, #1
	movs	r3, #1
	bl	sub_08022768
	ldr	r1, [sp, #8]
	movs	r3, #1
	strb	r3, [r1, #0]
	ldr	r2, [sp, #72]
	ldr	r1, [pc, #444]
	adds	r3, r2, r1
	movs	r2, #0
	strb	r2, [r3, #0]
.L_08024328:
	ldr	r3, [sp, #52]
	cmp	r3, #5
	ble.n	.L_08024412
	movs	r7, #0
	adds	r3, #4
	mov	r8, r3
	b.n	.L_0802438a
.L_08024336:
	ldr	r3, [pc, #444]
	movs	r2, #128
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #428]
	lsls	r2, r2, #1
	ands	r3, r2
	adds	r6, r7, r1
	cmp	r3, #0
	bne.n	.L_08024354
	ldr	r3, [pc, #428]
	ldr	r3, [r3, #0]
	movs	r2, #15
	ands	r3, r2
	cmp	r3, #11
	bhi.n	.L_08024364
.L_08024354:
	mov	r0, fp
	movs	r1, #5
	bl	sub_080022ec
	cmp	r7, r0
	bne.n	.L_08024364
	ldr	r2, [pc, #396]
	adds	r6, r7, r2
.L_08024364:
	mov	r3, r9
	movs	r1, #5
	mov	r0, r8
	ldrh	r5, [r3, #8]
	bl	sub_080022ec
	subs	r5, r5, r0
	adds	r5, r5, r7
	movs	r1, #0
	subs	r5, #2
	movs	r3, #1
	str	r1, [sp, #0]
	mov	r0, r9
	adds	r1, r6, #0
	adds	r2, r5, #0
	negs	r3, r3
	bl	sub_08019000
	adds	r7, #1
.L_0802438a:
	mov	r0, r8
	movs	r1, #5
	bl	sub_080022ec
	cmp	r7, r0
	blt.n	.L_08024336
	ldr	r3, [pc, #348]
	ldr	r5, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r5, r3
	cmp	r5, #0
	bne.n	.L_080243d0
	mov	r3, r9
	ldrh	r2, [r3, #8]
	movs	r3, #1
	subs	r2, r2, r0
	subs	r2, #3
	mov	r0, r9
	ldr	r1, [pc, #328]
	negs	r3, r3
	str	r5, [sp, #0]
	bl	sub_08019000
	mov	r1, r9
	ldrh	r2, [r1, #8]
	movs	r3, #1
	subs	r2, #2
	mov	r0, r9
	ldr	r1, [pc, #312]
	negs	r3, r3
	str	r5, [sp, #0]
	bl	sub_08019000
	b.n	.L_080243fe
.L_080243d0:
	mov	r3, r9
	ldrh	r2, [r3, #8]
	movs	r1, #0
	subs	r2, r2, r0
	movs	r3, #1
	subs	r2, #3
	str	r1, [sp, #0]
	mov	r0, r9
	ldr	r1, [pc, #288]
	negs	r3, r3
	bl	sub_08019000
	mov	r3, r9
	ldrh	r2, [r3, #8]
	movs	r1, #0
	movs	r3, #1
	str	r1, [sp, #0]
	subs	r2, #2
	mov	r0, r9
	ldr	r1, [pc, #272]
	negs	r3, r3
	bl	sub_08019000
.L_080243fe:
	mov	r2, r9
	ldrh	r3, [r2, #14]
	ldr	r1, [sp, #8]
	subs	r3, #1
	lsrs	r3, r3, #2
	movs	r2, #2
	lsls	r2, r3
	ldrb	r3, [r1, #0]
	orrs	r2, r3
	strb	r2, [r1, #0]
.L_08024412:
	ldr	r3, [pc, #248]
	ldr	r1, [r3, #0]
	ldr	r3, [pc, #248]
	ldr	r0, [pc, #248]
	ldr	r7, [r3, #0]
	ldr	r3, [pc, #212]
	ldr	r2, [r0, #0]
	ldr	r3, [r3, #0]
	mov	r8, r3
	adds	r3, r2, #0
	adds	r3, #216
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0802444a
	adds	r2, #220
	ldr	r3, [r2, #0]
	movs	r1, #0
	movs	r7, #0
	mov	r8, r1
	cmp	r3, #0
	bne.n	.L_08024446
	movs	r3, #60
	str	r3, [r2, #0]
	movs	r7, #1
	movs	r1, #1
	b.n	.L_0802444a
.L_08024446:
	subs	r3, #1
	str	r3, [r2, #0]
.L_0802444a:
	ldr	r2, [r0, #0]
	ldr	r3, [r2, #76]
	cmp	r3, #0
	beq.n	.L_0802445a
	movs	r3, #2
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_08024466
.L_0802445a:
	movs	r0, #113
	movs	r6, #1
	bl	sub_080f9010
	negs	r6, r6
	b.n	.L_080248a0
.L_08024466:
	movs	r3, #1
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0802451c
	ldr	r3, [sp, #52]
	cmp	r3, #0
	beq.n	.L_080244d6
	mov	r3, fp
	add	r3, sl
	ldr	r1, [sp, #56]
	lsls	r3, r3, #2
	ldr	r0, [r3, r1]
	movs	r6, #248
	lsls	r6, r6, #14
	adds	r5, r0, #0
	ands	r5, r6
	cmp	r5, #0
	bne.n	.L_080244a0
	ldr	r3, [sp, #24]
	cmp	r3, #0
	bne.n	.L_080244ce
	mov	r1, fp
	mov	r3, sl
	str	r1, [r2, #52]
	str	r3, [r2, #48]
	ldr	r1, [sp, #36]
	adds	r6, r0, #0
	str	r1, [r2, #56]
	b.n	.L_080248a0
.L_080244a0:
	ldr	r2, [sp, #24]
	cmp	r2, #0
	bne.n	.L_080244ce
	ands	r5, r6
	bl	sub_08016738
	bl	sub_080198dc
	lsrs	r0, r5, #17
	movs	r1, #5
	bl	sub_08019908
	movs	r2, #52
	ldr	r1, [sp, #12]
	ldr	r0, [pc, #88]
	bl	sub_0801965c
	movs	r2, #0
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #44]
	movs	r3, #4
	bl	sub_08017aa4
.L_080244ce:
	movs	r0, #114
	bl	sub_080f9010
	b.n	.L_0802451c
.L_080244d6:
	movs	r6, #1
	negs	r6, r6
	b.n	.L_080248a0
	.4byte 0x000008ed
	.4byte 0x00000ea6
	.4byte 0x00005001
	.4byte 0x0000045f
	.4byte 0x0000f301
	.4byte 0x0000f30b
	.4byte 0x03001ae8
	.4byte 0x03001e40
	.4byte 0x0000f334
	.4byte 0x0000f335
	.4byte 0x0000f011
	.4byte 0x0000f012
	.4byte 0x03001c94
	.4byte 0x03001b04
	.4byte 0x03001f34
	.2byte 0x0898
	.2byte 0x0000
.L_0802451c:
	ldr	r3, [sp, #52]
	cmp	r3, #0
	bne.n	.L_08024524
	b.n	.L_08024766
.L_08024524:
	movs	r3, #128
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08024550
	movs	r0, #111
	bl	sub_080f9010
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	cmp	r2, #5
	beq.n	.L_08024546
	mov	r3, fp
	ldr	r1, [sp, #52]
	add	r3, sl
	cmp	r3, r1
	bne.n	.L_0802454a
.L_08024546:
	movs	r2, #0
	mov	sl, r2
.L_0802454a:
	mov	r3, sl
	str	r3, [sp, #36]
	b.n	.L_08024766
.L_08024550:
	movs	r3, #64
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08024590
	movs	r0, #111
	bl	sub_080f9010
	movs	r1, #1
	negs	r1, r1
	add	sl, r1
	mov	r2, sl
	cmp	r2, #0
	bge.n	.L_0802458a
	ldr	r0, [sp, #52]
	movs	r1, #5
	subs	r0, #1
	bl	sub_080022ec
	lsls	r3, r0, #2
	adds	r3, r3, r0
	cmp	fp, r3
	bne.n	.L_08024586
	ldr	r1, [sp, #52]
	mov	r2, fp
	subs	r3, r1, r2
	subs	r3, #1
	b.n	.L_08024588
.L_08024586:
	movs	r3, #4
.L_08024588:
	mov	sl, r3
.L_0802458a:
	mov	r1, sl
	str	r1, [sp, #36]
	b.n	.L_08024766
.L_08024590:
	movs	r3, #128
	lsls	r3, r3, #1
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08024694
	ldr	r3, [sp, #68]
	cmp	r3, #0
	beq.n	.L_08024622
	ldr	r0, [sp, #80]
	movs	r5, #0
	cmp	r5, r0
	bge.n	.L_080245e6
.L_080245aa:
	ldr	r3, [pc, #384]
	ldr	r2, [pc, #384]
	ldr	r3, [r3, #0]
	adds	r1, r5, r2
	movs	r2, #15
	ands	r3, r2
	cmp	r3, #11
	bhi.n	.L_080245c8
	ldr	r3, [sp, #32]
	subs	r3, #1
	cmp	r5, r3
	bne.n	.L_080245c8
	ldr	r3, [sp, #32]
	ldr	r2, [pc, #364]
	adds	r1, r3, r2
.L_080245c8:
	ldr	r3, [sp, #68]
	ldrh	r2, [r3, #8]
	subs	r2, r2, r0
	adds	r2, r2, r5
	movs	r3, #0
	str	r3, [sp, #0]
	ldr	r0, [sp, #68]
	subs	r2, #2
	subs	r3, #1
	bl	sub_08019000
	ldr	r0, [sp, #80]
	adds	r5, #1
	cmp	r5, r0
	blt.n	.L_080245aa
.L_080245e6:
	ldr	r1, [sp, #68]
	ldrh	r2, [r1, #8]
	movs	r3, #0
	subs	r2, r2, r0
	str	r3, [sp, #0]
	adds	r0, r1, #0
	subs	r2, #3
	ldr	r1, [pc, #320]
	subs	r3, #1
	bl	sub_08019000
	ldr	r1, [sp, #68]
	ldrh	r2, [r1, #8]
	movs	r3, #0
	str	r3, [sp, #0]
	adds	r0, r1, #0
	subs	r2, #2
	ldr	r1, [pc, #304]
	subs	r3, #1
	bl	sub_08019000
	ldr	r1, [sp, #68]
	ldrh	r2, [r1, #14]
	ldr	r1, [sp, #8]
	lsrs	r2, r2, #2
	movs	r3, #2
	lsls	r3, r2
	ldrb	r2, [r1, #0]
	orrs	r3, r2
	strb	r3, [r1, #0]
.L_08024622:
	ldr	r2, [sp, #32]
	cmp	r2, #0
	bne.n	.L_08024642
	ldr	r0, [sp, #80]
	cmp	r0, #0
	beq.n	.L_08024644
	ldr	r3, [sp, #68]
	cmp	r3, #0
	beq.n	.L_0802463a
	adds	r0, r3, #0
	bl	sub_080164ac
.L_0802463a:
	movs	r1, #1
	str	r1, [sp, #32]
	str	r1, [sp, #28]
	b.n	.L_08024766
.L_08024642:
	ldr	r0, [sp, #80]
.L_08024644:
	ldr	r2, [sp, #32]
	cmp	r2, r0
	ble.n	.L_0802464c
	str	r0, [sp, #32]
.L_0802464c:
	ldr	r3, [sp, #32]
	cmp	r3, #0
	bne.n	.L_08024654
	b.n	.L_08024766
.L_08024654:
	movs	r3, #16
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08024672
	movs	r0, #111
	bl	sub_080f9010
	ldr	r1, [sp, #32]
	ldr	r3, [sp, #80]
	adds	r1, #1
	str	r1, [sp, #32]
	cmp	r1, r3
	ble.n	.L_0802468e
	movs	r2, #1
	b.n	.L_0802468c
.L_08024672:
	movs	r3, #32
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08024766
	movs	r0, #111
	bl	sub_080f9010
	ldr	r1, [sp, #32]
	subs	r1, #1
	str	r1, [sp, #32]
	cmp	r1, #0
	bgt.n	.L_0802468e
	ldr	r2, [sp, #80]
.L_0802468c:
	str	r2, [sp, #32]
.L_0802468e:
	movs	r3, #1
	str	r3, [sp, #28]
	b.n	.L_08024766
.L_08024694:
	ldr	r1, [sp, #32]
	cmp	r1, #0
	beq.n	.L_080246b0
	ldr	r2, [sp, #68]
	cmp	r2, #0
	beq.n	.L_080246a6
	adds	r0, r2, #0
	bl	sub_080164ac
.L_080246a6:
	movs	r3, #0
	movs	r1, #1
	str	r3, [sp, #32]
	str	r1, [sp, #28]
	b.n	.L_08024766
.L_080246b0:
	movs	r3, #16
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08024708
	movs	r0, #111
	bl	sub_080f9010
	bl	sub_0800352c
	mov	r3, fp
	ldr	r2, [sp, #52]
	adds	r3, #5
	cmp	r3, r2
	blt.n	.L_080246dc
	mov	r3, fp
	cmp	r3, #0
	beq.n	.L_08024766
	ldr	r2, [sp, #36]
	movs	r1, #0
	mov	fp, r1
	mov	sl, r2
	b.n	.L_08024766
.L_080246dc:
	ldr	r0, [sp, #52]
	mov	fp, r3
	ldr	r3, [sp, #36]
	subs	r0, #1
	movs	r1, #5
	mov	sl, r3
	bl	sub_080022ec
	lsls	r3, r0, #2
	adds	r3, r3, r0
	cmp	fp, r3
	bne.n	.L_08024766
	ldr	r1, [sp, #52]
	mov	r2, fp
	subs	r3, r1, r2
	subs	r3, #1
	mov	sl, r3
	ldr	r3, [sp, #36]
	cmp	sl, r3
	ble.n	.L_08024766
	mov	sl, r3
	b.n	.L_08024766
.L_08024708:
	movs	r3, #32
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_08024766
	movs	r0, #111
	bl	sub_080f9010
	bl	sub_0800352c
	mov	r1, fp
	cmp	r1, #0
	beq.n	.L_08024740
	movs	r2, #5
	ldr	r3, [sp, #36]
	negs	r2, r2
	add	fp, r2
	mov	sl, r3
	b.n	.L_08024766
	.4byte 0x03001e40
	.4byte 0x0000f301
	.4byte 0x0000f30a
	.4byte 0x0000f334
	.2byte 0xf335
	.2byte 0x0000
.L_08024740:
	ldr	r0, [sp, #52]
	movs	r1, #5
	subs	r0, #1
	bl	sub_080022ec
	lsls	r3, r0, #2
	ldr	r1, [sp, #36]
	adds	r3, r3, r0
	mov	fp, r3
	mov	sl, r1
	cmp	r3, #0
	beq.n	.L_08024766
	ldr	r2, [sp, #52]
	subs	r3, r2, r3
	subs	r3, #1
	mov	sl, r3
	cmp	sl, r1
	ble.n	.L_08024766
	mov	sl, r1
.L_08024766:
	mov	r1, r9
	ldrh	r3, [r1, #12]
	lsls	r3, r3, #3
	mov	r2, sl
	subs	r3, #2
	str	r3, [sp, #16]
	lsls	r3, r2, #1
	ldrh	r2, [r1, #14]
	adds	r3, r3, r2
	lsls	r3, r3, #3
	adds	r3, #20
	ldr	r1, [sp, #4]
	str	r3, [sp, #20]
	movs	r3, #128
	lsls	r3, r3, #23
	movs	r2, #0
	str	r3, [r1, #4]
	str	r2, [r1, #8]
	ldr	r0, [sp, #60]
	ldr	r1, [pc, #72]
	bl	sub_080040d0
	ldr	r3, [pc, #52]
	ldr	r1, [sp, #4]
	ands	r0, r3
	ldr	r2, [pc, #48]
	ldrh	r3, [r1, #8]
	ldr	r6, [pc, #60]
	ands	r3, r2
	orrs	r3, r0
	adds	r2, r1, #0
	strh	r3, [r2, #8]
	ldr	r2, [r6, #0]
	movs	r5, #4
	ldr	r3, [sp, #16]
	ands	r2, r5
	ldr	r1, [pc, #48]
	lsrs	r2, r2, #1
	adds	r2, r3, r2
	adds	r2, r2, r1
	ldr	r3, [pc, #24]
	ldr	r1, [sp, #4]
	ands	r2, r3
	ldrh	r3, [r1, #6]
	ldr	r1, [pc, #20]
	ands	r3, r1
	orrs	r3, r2
	b.n	.L_080247e4
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x080310a4
	.4byte 0x03001e40
	.2byte 0xfffa
	.2byte 0x0000
.L_080247e4:
	ldr	r2, [sp, #4]
	strh	r3, [r2, #6]
	ldr	r3, [r6, #0]
	ldr	r1, [sp, #20]
	ands	r3, r5
	lsrs	r3, r3, #2
	subs	r3, r1, r3
	adds	r3, #248
	strb	r3, [r2, #4]
	ldr	r2, [sp, #52]
	cmp	r2, #0
	beq.n	.L_08024804
	ldr	r0, [sp, #4]
	movs	r1, #242
	bl	sub_08003dec
.L_08024804:
	ldr	r3, [pc, #260]
	ldr	r3, [r3, #0]
	ldrh	r2, [r3, #12]
	ldr	r6, [r6, #0]
	ldr	r7, [r3, #0]
	movs	r3, #2
	ands	r3, r2
	ands	r6, r5
	cmp	r3, #0
	beq.n	.L_08024858
	movs	r5, #0
.L_0802481a:
	negs	r3, r6
	orrs	r3, r6
	lsrs	r3, r3, #31
	adds	r2, r3, #0
	movs	r3, #15
	subs	r2, r3, r2
	ldr	r1, [sp, #40]
	movs	r3, #1
	lsls	r3, r5
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_08024834
	movs	r2, #15
.L_08024834:
	ldr	r3, [pc, #216]
	ldrh	r0, [r7, #12]
	ldrb	r3, [r3, r5]
	adds	r0, r0, r3
	ldr	r3, [pc, #212]
	ldrh	r1, [r7, #14]
	ldrb	r3, [r3, r5]
	adds	r1, r1, r3
	str	r2, [sp, #0]
	adds	r0, #1
	adds	r1, #1
	movs	r2, #2
	movs	r3, #2
	adds	r5, #1
	bl	sub_08022768
	cmp	r5, #3
	ble.n	.L_0802481a
.L_08024858:
	ldr	r3, [pc, #188]
	ldr	r3, [r3, #0]
	movs	r2, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0802487e
	ldr	r5, [pc, #180]
	movs	r2, #32
	adds	r1, r5, #0
	ldr	r6, [pc, #180]
	ldr	r0, [pc, #180]
	bl	sub_080072fc
	ldr	r0, [pc, #180]
	adds	r1, r5, #0
	movs	r2, #32
	bl	sub_080072fc
	b.n	.L_08024896
.L_0802487e:
	ldr	r3, [pc, #172]
	movs	r1, #32
	ldr	r2, [pc, #172]
	ldr	r0, [pc, #156]
	bl	sub_080072f0
	ldr	r3, [pc, #148]
	ldr	r0, [pc, #152]
	ldr	r1, [pc, #140]
	movs	r2, #32
	bl	sub_080072f0
.L_08024896:
	movs	r0, #1
	bl	sub_080030f8
	bl	.L_08023ff4
.L_080248a0:
	ldr	r3, [pc, #104]
	ldr	r1, [r3, #0]
	ldrh	r2, [r1, #12]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080248c4
	ldr	r3, [r1, #0]
	ldrh	r0, [r3, #12]
	ldrh	r1, [r3, #14]
	movs	r3, #15
	str	r3, [sp, #0]
	adds	r0, #1
	adds	r1, #1
	movs	r2, #4
	movs	r3, #4
	bl	sub_08022768
.L_080248c4:
	ldr	r0, [sp, #60]
	bl	sub_08003f3c
	movs	r1, #1
	ldr	r0, [sp, #44]
	bl	sub_08016418
	movs	r1, #1
	ldr	r0, [sp, #68]
	bl	sub_08016418
	movs	r1, #1
	mov	r0, r9
	bl	sub_08016418
	bl	sub_0801e318
	movs	r0, #0
	bl	sub_0801e3c8
	ldr	r0, [sp, #56]
	bl	sub_08002df0
	movs	r0, #1
	bl	sub_080030f8
	adds	r0, r6, #0
	add	sp, #224
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e90
	.4byte 0x080373e7
	.4byte 0x080373eb
	.4byte 0x03001e40
	.4byte 0x08037308
	.4byte 0x03001388
	.4byte 0x06006500
	.4byte 0x06006520
	.4byte 0x03000168
	.2byte 0x4444
	.2byte 0x4444
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #372
	str	r2, [sp, #84]
	ldr	r5, [pc, #184]
	ldr	r0, [r5, #0]
	movs	r1, #1
	str	r0, [sp, #72]
	negs	r1, r1
	movs	r0, #128
	str	r1, [sp, #68]
	mov	r9, r1
	bl	sub_080040b4
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	movs	r3, #42
	str	r0, [sp, #64]
	str	r3, [sp, #0]
	movs	r1, #4
	movs	r2, #30
	movs	r3, #4
	movs	r0, #0
	bl	sub_080162d4
	movs	r6, #6
	str	r0, [sp, #60]
	movs	r1, #8
	movs	r2, #10
	movs	r3, #3
	movs	r0, #20
	str	r6, [sp, #0]
	bl	sub_080162d4
	movs	r2, #0
	str	r0, [sp, #56]
	str	r2, [sp, #52]
	adds	r5, #168
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #52]
	ldr	r1, [r3, #48]
	ldr	r3, [r3, #56]
	mov	fp, r0
	mov	sl, r1
	str	r3, [sp, #48]
	str	r6, [sp, #0]
	movs	r2, #17
	movs	r3, #9
	movs	r0, #13
	movs	r1, #11
	bl	sub_080162d4
	movs	r2, #156
	lsls	r2, r2, #1
	add	r2, sp
	ldr	r3, [pc, #84]
	movs	r7, #128
	str	r0, [sp, #76]
	str	r2, [sp, #28]
	movs	r4, #0
	mov	ip, r3
	adds	r5, r2, #0
	lsls	r7, r7, #23
	movs	r6, #0
.L_080249be:
	lsls	r0, r4, #1
	str	r7, [r5, #4]
	str	r6, [r5, #8]
	ldr	r1, [sp, #76]
	ldrh	r2, [r1, #12]
	ldr	r3, [pc, #48]
	lsls	r2, r2, #3
	ldrh	r1, [r5, #6]
	adds	r2, #8
	ands	r2, r3
	mov	r3, ip
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #6]
	ldr	r2, [sp, #76]
	ldrh	r3, [r2, #14]
	adds	r0, r0, r3
	lsls	r0, r0, #3
	adds	r0, #4
	adds	r4, #1
	strb	r0, [r5, #4]
	adds	r5, #12
	cmp	r4, #3
	ble.n	.L_080249be
	ldr	r3, [pc, #24]
	ldr	r7, [sp, #28]
	movs	r5, #8
	add	r6, sp, #96
	mov	r8, r3
	movs	r4, #3
	b.n	.L_08024a0c
	.4byte 0x000001ff
	.4byte 0x03001e8c
	.4byte 0xfffffe00
	.2byte 0xfc00
	.2byte 0xffff
.L_08024a0c:
	movs	r0, #128
	str	r4, [sp, #4]
	bl	sub_080040b4
	movs	r1, #1
	negs	r1, r1
	stmia	r6!, {r0}
	bl	sub_080040d0
	ldr	r3, [pc, #24]
	ands	r0, r3
	ldrh	r3, [r5, r7]
	mov	r1, r8
	ldr	r4, [sp, #4]
	ands	r3, r1
	orrs	r3, r0
	subs	r4, #1
	strh	r3, [r5, r7]
	adds	r5, #12
	cmp	r4, #0
	bge.n	.L_08024a0c
	b.n	.L_08024a3c
	.2byte 0x03ff
	.2byte 0x0000
.L_08024a3c:
	movs	r2, #138
	lsls	r2, r2, #1
	add	r2, sp
	mov	r8, r2
	mov	r0, r8
	bl	sub_080771d8
	str	r0, [sp, #80]
	movs	r7, #0
	adds	r3, r0, #0
	subs	r3, #1
	str	r3, [sp, #20]
	cmp	r3, #0
	blt.n	.L_08024aa0
	mov	r0, sp
	adds	r0, #240
	adds	r5, r3, #0
	str	r0, [sp, #32]
	add	r5, r8
.L_08024a62:
	ldrb	r6, [r5, #0]
	adds	r0, r6, #0
	bl	sub_080771e0
	ldr	r1, [sp, #84]
	adds	r0, #4
	ldrb	r2, [r0, #0]
	ldrb	r3, [r1, #0]
	movs	r4, #0
	cmp	r2, r3
	bhi.n	.L_08024a8a
.L_08024a78:
	adds	r4, #1
	cmp	r4, #3
	bgt.n	.L_08024a8a
	adds	r0, #1
	adds	r1, #1
	ldrb	r2, [r0, #0]
	ldrb	r3, [r1, #0]
	cmp	r2, r3
	bls.n	.L_08024a78
.L_08024a8a:
	cmp	r4, #4
	bne.n	.L_08024a98
	ldr	r2, [sp, #32]
	movs	r3, #32
	strb	r6, [r2, r7]
	strb	r3, [r5, #0]
	adds	r7, #1
.L_08024a98:
	subs	r5, #1
	cmp	r5, r8
	bge.n	.L_08024a62
	b.n	.L_08024aa6
.L_08024aa0:
	mov	r3, sp
	adds	r3, #240
	str	r3, [sp, #32]
.L_08024aa6:
	ldr	r0, [sp, #80]
	cmp	r0, #0
	ble.n	.L_08024ac8
	ldr	r2, [sp, #32]
	adds	r1, r7, r2
	ldr	r2, [sp, #80]
	mov	r0, r8
.L_08024ab4:
	ldrb	r3, [r0, #0]
	adds	r0, #1
	cmp	r3, #32
	beq.n	.L_08024ac2
	strb	r3, [r1, #0]
	adds	r7, #1
	adds	r1, #1
.L_08024ac2:
	subs	r2, #1
	cmp	r2, #0
	bne.n	.L_08024ab4
.L_08024ac8:
	ldr	r0, [sp, #32]
	movs	r3, #32
	strb	r3, [r0, r7]
	movs	r1, #180
	ldr	r3, [sp, #64]
	lsls	r1, r1, #1
	mov	r2, sl
	add	r1, sp
	lsls	r2, r2, #1
	lsls	r3, r3, #16
	str	r1, [sp, #24]
	str	r2, [sp, #16]
	str	r3, [sp, #12]
.L_08024ae2:
	cmp	fp, r9
	bne.n	.L_08024aee
	ldr	r0, [sp, #68]
	cmp	sl, r0
	bne.n	.L_08024aee
	b.n	.L_08024d90
.L_08024aee:
	ldr	r1, [sp, #72]
	ldr	r2, [pc, #316]
	movs	r0, #1
	adds	r3, r1, r2
	strb	r0, [r3, #0]
	ldr	r1, [sp, #76]
	ldr	r2, [sp, #68]
	ldrh	r0, [r1, #12]
	ldrh	r1, [r1, #14]
	lsls	r3, r2, #1
	adds	r1, r1, r3
	ldr	r3, [sp, #76]
	ldrh	r2, [r3, #8]
	movs	r3, #15
	str	r3, [sp, #0]
	subs	r2, #2
	adds	r1, #1
	movs	r3, #1
	adds	r0, #1
	bl	sub_08022768
	bl	sub_08016738
	mov	r3, fp
	ldr	r1, [sp, #32]
	add	r3, sl
	ldrb	r0, [r1, r3]
	bl	sub_080771e0
	adds	r6, r0, #0
	ldrh	r0, [r6, #0]
	ldr	r3, [pc, #260]
	add	r5, sp, #112
	adds	r0, r0, r3
	adds	r1, r5, #0
	movs	r2, #52
	bl	sub_0801965c
	movs	r2, #0
	ldr	r1, [sp, #60]
	movs	r3, #4
	adds	r0, r5, #0
	bl	sub_08017aa4
	movs	r3, #0
	mov	r2, sl
	str	r3, [sp, #52]
	str	r2, [sp, #68]
	movs	r1, #1
	movs	r2, #0
	adds	r6, #4
.L_08024b54:
	ldrb	r3, [r6, #0]
	adds	r6, #1
	cmp	r3, #0
	beq.n	.L_08024b66
	ldr	r0, [sp, #52]
	adds	r3, r1, #0
	lsls	r3, r2
	orrs	r0, r3
	str	r0, [sp, #52]
.L_08024b66:
	adds	r2, #1
	cmp	r2, #3
	ble.n	.L_08024b54
	cmp	fp, r9
	bne.n	.L_08024b72
	b.n	.L_08024d06
.L_08024b72:
	ldr	r0, [sp, #76]
	bl	sub_08016498
	movs	r5, #0
	movs	r7, #0
	movs	r6, #1
.L_08024b7e:
	ldr	r2, [pc, #184]
	ldr	r0, [sp, #56]
	adds	r1, r5, r2
	movs	r3, #0
	lsls	r2, r5, #1
	str	r7, [sp, #0]
	bl	sub_08019000
	ldr	r3, [sp, #84]
	ldrb	r1, [r3, r5]
	adds	r2, r6, #0
	adds	r1, #48
	ldr	r0, [sp, #56]
	movs	r3, #0
	adds	r5, #1
	str	r7, [sp, #0]
	adds	r6, #2
	bl	sub_08018efc
	cmp	r5, #3
	ble.n	.L_08024b7e
	ldr	r0, [sp, #32]
	mov	r1, fp
	ldrb	r6, [r0, r1]
	movs	r4, #0
	cmp	r6, #32
	bne.n	.L_08024bb6
	b.n	.L_08024ce8
.L_08024bb6:
	mov	r2, sp
	adds	r2, #88
	str	r2, [sp, #8]
.L_08024bbc:
	adds	r0, r6, #0
	str	r4, [sp, #4]
	bl	sub_080771e0
	str	r0, [sp, #36]
	adds	r1, r0, #0
	ldr	r0, [sp, #84]
	adds	r1, #4
	ldrb	r2, [r1, #0]
	ldrb	r3, [r0, #0]
	movs	r7, #0
	ldr	r4, [sp, #4]
	cmp	r2, r3
	bhi.n	.L_08024bea
.L_08024bd8:
	adds	r7, #1
	cmp	r7, #3
	bgt.n	.L_08024bea
	adds	r1, #1
	adds	r0, #1
	ldrb	r2, [r1, #0]
	ldrb	r3, [r0, #0]
	cmp	r2, r3
	bls.n	.L_08024bd8
.L_08024bea:
	movs	r3, #4
	eors	r3, r7
	ldr	r2, [sp, #36]
	negs	r5, r3
	orrs	r5, r3
	ldr	r0, [pc, #48]
	ldrh	r3, [r2, #0]
	movs	r1, #1
	ands	r0, r3
	add	r2, sp, #96
	lsls	r3, r4, #2
	lsrs	r5, r5, #31
	adds	r2, r2, r3
	str	r1, [sp, #0]
	ldr	r3, [sp, #8]
	subs	r5, r1, r5
	movs	r1, #0
	str	r4, [sp, #4]
	bl	sub_0801a3d0
	ldr	r4, [sp, #4]
	lsls	r3, r4, #1
	adds	r1, r3, r4
	ldr	r2, [sp, #28]
	mov	r8, r3
	lsls	r1, r1, #2
	ldr	r3, [pc, #12]
	ldr	r0, [sp, #88]
	adds	r1, #8
	ands	r0, r3
	b.n	.L_08024c3c
	.4byte 0x00003fff
	.4byte 0x000003ff
	.4byte 0x00000ea6
	.4byte 0x0000053a
	.2byte 0x5001
	.2byte 0x0000
.L_08024c3c:
	ldrh	r3, [r2, r1]
	ldr	r2, [pc, #56]
	ands	r3, r2
	orrs	r3, r0
	ldr	r0, [sp, #28]
	strh	r3, [r0, r1]
	cmp	r5, #0
	bne.n	.L_08024c54
	movs	r0, #2
	bl	sub_0801e71c
	ldr	r4, [sp, #4]
.L_08024c54:
	adds	r0, r6, #0
	str	r4, [sp, #4]
	bl	sub_080771e0
	ldr	r3, [pc, #28]
	ldr	r4, [sp, #4]
	ldrh	r0, [r0, #0]
	ldr	r1, [sp, #76]
	adds	r0, r0, r3
	movs	r2, #16
	lsls	r3, r4, #4
	bl	sub_0801e7c0
	movs	r1, #0
	ldr	r6, [sp, #36]
	lsls	r3, r1, #1
	b.n	.L_08024c80
	movs	r0, r0
	.4byte 0xfffffc00
	.2byte 0x0333
	.2byte 0x0000
.L_08024c80:
	adds	r5, r3, #0
	ldr	r4, [sp, #4]
	movs	r7, #0
	mov	r9, r1
	adds	r6, #4
	adds	r5, #13
.L_08024c8c:
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_08024cbc
	ldr	r2, [pc, #128]
	mov	r3, r9
	adds	r1, r7, r2
	str	r3, [sp, #0]
	ldr	r0, [sp, #76]
	adds	r2, r5, #0
	mov	r3, r8
	str	r4, [sp, #4]
	bl	sub_08019000
	ldrb	r1, [r6, #0]
	mov	r0, r9
	adds	r2, r5, #1
	str	r0, [sp, #0]
	adds	r1, #48
	ldr	r0, [sp, #76]
	mov	r3, r8
	bl	sub_08018efc
	ldr	r4, [sp, #4]
	adds	r5, #2
.L_08024cbc:
	adds	r7, #1
	adds	r6, #1
	cmp	r7, #3
	ble.n	.L_08024c8c
	movs	r0, #15
	str	r4, [sp, #4]
	bl	sub_0801e71c
	ldr	r4, [sp, #4]
	add	r3, sp, #92
	movs	r1, #1
	strb	r1, [r3, r4]
	adds	r4, #1
	cmp	r4, #3
	bgt.n	.L_08024d04
	mov	r2, fp
	ldr	r0, [sp, #32]
	adds	r3, r2, r4
	ldrb	r6, [r0, r3]
	cmp	r6, #32
	beq.n	.L_08024ce8
	b.n	.L_08024bbc
.L_08024ce8:
	cmp	r4, #3
	bgt.n	.L_08024d04
	add	r2, sp, #372
	ldr	r0, [pc, #40]
	adds	r3, r4, r2
	adds	r2, r3, r0
	movs	r3, #4
	movs	r1, #0
	subs	r4, r3, r4
.L_08024cfa:
	subs	r4, #1
	strb	r1, [r2, #0]
	adds	r2, #1
	cmp	r4, #0
	bne.n	.L_08024cfa
.L_08024d04:
	mov	r9, fp
.L_08024d06:
	ldr	r1, [sp, #80]
	cmp	r1, #4
	ble.n	.L_08024d60
	movs	r4, #0
	adds	r5, r1, #0
	adds	r5, #3
	b.n	.L_08024d50
	.4byte 0x00005001
	.2byte 0xfee8
	.2byte 0xffff
.L_08024d1c:
	ldr	r2, [pc, #256]
	mov	r3, fp
	adds	r1, r4, r2
	cmp	r3, #0
	bge.n	.L_08024d28
	adds	r3, #3
.L_08024d28:
	asrs	r3, r3, #2
	cmp	r4, r3
	bne.n	.L_08024d32
	ldr	r3, [pc, #244]
	adds	r1, r4, r3
.L_08024d32:
	ldr	r3, [sp, #76]
	ldrh	r2, [r3, #8]
	subs	r2, r2, r0
	movs	r0, #0
	adds	r2, r2, r4
	str	r0, [sp, #0]
	adds	r0, r3, #0
	movs	r3, #1
	subs	r2, #2
	negs	r3, r3
	str	r4, [sp, #4]
	bl	sub_08019000
	ldr	r4, [sp, #4]
	adds	r4, #1
.L_08024d50:
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_08024d5a
	ldr	r3, [sp, #80]
	adds	r3, #6
.L_08024d5a:
	asrs	r0, r3, #2
	cmp	r4, r0
	blt.n	.L_08024d1c
.L_08024d60:
	ldr	r1, [sp, #76]
	ldr	r2, [sp, #16]
	ldrh	r0, [r1, #12]
	ldr	r3, [sp, #76]
	ldrh	r1, [r1, #14]
	adds	r1, r1, r2
	ldrh	r2, [r3, #8]
	movs	r3, #14
	adds	r0, #1
	adds	r1, #1
	subs	r2, #2
	str	r3, [sp, #0]
	movs	r3, #1
	bl	sub_08022768
	ldr	r1, [pc, #168]
	ldr	r0, [sp, #72]
	movs	r2, #1
	adds	r3, r0, r1
	adds	r1, #3
	strb	r2, [r3, #0]
	adds	r3, r0, r1
	movs	r2, #0
	strb	r2, [r3, #0]
.L_08024d90:
	ldr	r6, [sp, #28]
	movs	r4, #0
	add	r5, sp, #92
.L_08024d96:
	ldrb	r3, [r5, #0]
	adds	r5, #1
	cmp	r3, #0
	beq.n	.L_08024daa
	adds	r0, r6, #0
	movs	r1, #240
	str	r4, [sp, #4]
	bl	sub_08003dec
	ldr	r4, [sp, #4]
.L_08024daa:
	adds	r4, #1
	adds	r6, #12
	cmp	r4, #3
	ble.n	.L_08024d96
	ldr	r0, [sp, #76]
	ldrh	r3, [r0, #12]
	lsls	r3, r3, #3
	subs	r3, #2
	ldr	r1, [sp, #16]
	str	r3, [sp, #40]
	ldrh	r3, [r0, #14]
	adds	r3, r1, r3
	lsls	r3, r3, #3
	adds	r3, #20
	ldr	r2, [sp, #24]
	str	r3, [sp, #44]
	movs	r3, #128
	lsls	r3, r3, #23
	str	r3, [r2, #4]
	movs	r3, #0
	str	r3, [r2, #8]
	ldr	r1, [sp, #12]
	lsrs	r0, r1, #16
	ldr	r1, [pc, #80]
	bl	sub_080040d0
	ldr	r3, [pc, #48]
	ldr	r2, [sp, #24]
	ands	r0, r3
	ldrh	r3, [r2, #8]
	ldr	r2, [pc, #44]
	ldr	r1, [pc, #68]
	ands	r3, r2
	orrs	r3, r0
	ldr	r0, [sp, #24]
	ldr	r2, [r1, #0]
	strh	r3, [r0, #8]
	movs	r0, #4
	ldr	r3, [sp, #40]
	ands	r2, r0
	ldr	r1, [pc, #56]
	lsrs	r2, r2, #1
	adds	r2, r3, r2
	adds	r2, r2, r1
	ldr	r3, [pc, #20]
	ldr	r1, [sp, #24]
	ands	r2, r3
	ldrh	r3, [r1, #6]
	ldr	r1, [pc, #16]
	ands	r3, r1
	b.n	.L_08024e38
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x0000f301
	.4byte 0x0000f30b
	.4byte 0x00000ea3
	.4byte 0x080310a4
	.4byte 0x03001e40
	.2byte 0xfffc
	.2byte 0x0000
.L_08024e38:
	orrs	r3, r2
	ldr	r1, [pc, #800]
	ldr	r2, [sp, #24]
	strh	r3, [r2, #6]
	ldr	r3, [r1, #0]
	ldr	r2, [sp, #44]
	ands	r3, r0
	lsrs	r3, r3, #2
	ldr	r0, [sp, #24]
	subs	r3, r2, r3
	adds	r3, #248
	strb	r3, [r0, #4]
	movs	r1, #242
	ldr	r0, [sp, #24]
	bl	sub_08003dec
	ldr	r1, [pc, #768]
	ldr	r6, [r1, #0]
	movs	r3, #8
	ands	r6, r3
	movs	r5, #0
.L_08024e62:
	negs	r3, r6
	orrs	r3, r6
	lsrs	r3, r3, #31
	adds	r2, r3, #0
	movs	r3, #15
	subs	r2, r3, r2
	ldr	r0, [sp, #52]
	movs	r3, #1
	lsls	r3, r5
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_08024e7c
	movs	r2, #15
.L_08024e7c:
	ldr	r1, [sp, #56]
	ldrh	r0, [r1, #12]
	lsls	r3, r5, #1
	ldrh	r1, [r1, #14]
	adds	r0, r0, r3
	str	r2, [sp, #0]
	adds	r0, #1
	adds	r1, #1
	movs	r2, #2
	movs	r3, #1
	adds	r5, #1
	bl	sub_08022768
	cmp	r5, #3
	ble.n	.L_08024e62
	ldr	r2, [sp, #80]
	cmp	r2, #4
	ble.n	.L_08024f48
	movs	r4, #0
	adds	r5, r2, #0
	adds	r5, #3
	b.n	.L_08024ef2
.L_08024ea8:
	ldr	r3, [pc, #692]
	ldr	r0, [pc, #688]
	adds	r1, r4, r3
	ldr	r3, [r0, #0]
	movs	r2, #15
	ands	r3, r2
	cmp	r3, #11
	bhi.n	.L_08024eca
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L_08024ec0
	adds	r3, #3
.L_08024ec0:
	asrs	r3, r3, #2
	cmp	r4, r3
	bne.n	.L_08024eca
	ldr	r2, [pc, #668]
	adds	r1, r4, r2
.L_08024eca:
	ldr	r0, [sp, #76]
	adds	r2, r5, #0
	ldrh	r3, [r0, #8]
	cmp	r5, #0
	bge.n	.L_08024ed8
	ldr	r2, [sp, #80]
	adds	r2, #6
.L_08024ed8:
	asrs	r2, r2, #2
	subs	r2, r3, r2
	adds	r2, r2, r4
	movs	r3, #0
	str	r3, [sp, #0]
	subs	r2, #2
	ldr	r0, [sp, #76]
	subs	r3, #1
	str	r4, [sp, #4]
	bl	sub_08019000
	ldr	r4, [sp, #4]
	adds	r4, #1
.L_08024ef2:
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_08024efc
	ldr	r3, [sp, #80]
	adds	r3, #6
.L_08024efc:
	asrs	r2, r3, #2
	cmp	r4, r2
	blt.n	.L_08024ea8
	ldr	r0, [sp, #76]
	ldrh	r3, [r0, #8]
	movs	r5, #1
	negs	r5, r5
	subs	r2, r3, r2
	movs	r1, #0
	str	r1, [sp, #0]
	ldr	r0, [sp, #76]
	adds	r3, r5, #0
	subs	r2, #3
	ldr	r1, [pc, #592]
	bl	sub_08019000
	ldr	r3, [sp, #76]
	ldrh	r2, [r3, #8]
	movs	r0, #0
	str	r0, [sp, #0]
	subs	r2, #2
	adds	r0, r3, #0
	ldr	r1, [pc, #576]
	adds	r3, r5, #0
	bl	sub_08019000
	ldr	r2, [sp, #72]
	ldr	r3, [pc, #572]
	ldr	r0, [sp, #76]
	adds	r1, r2, r3
	ldrh	r3, [r0, #14]
	subs	r3, #1
	lsrs	r3, r3, #2
	movs	r2, #2
	lsls	r2, r3
	ldrb	r3, [r1, #0]
	orrs	r2, r3
	strb	r2, [r1, #0]
.L_08024f48:
	ldr	r3, [pc, #552]
	ldr	r2, [r3, #0]
	mov	r1, fp
	mov	r3, sl
	str	r1, [r2, #52]
	str	r3, [r2, #48]
	ldr	r0, [sp, #48]
	str	r0, [r2, #56]
	ldr	r3, [pc, #540]
	ldr	r1, [r3, #0]
	ldr	r3, [pc, #540]
	ldr	r0, [r3, #0]
	adds	r3, r2, #0
	adds	r3, #216
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08024f84
	adds	r2, #220
	ldr	r3, [r2, #0]
	movs	r0, #0
	movs	r1, #0
	cmp	r3, #0
	bne.n	.L_08024f80
	movs	r3, #120
	str	r3, [r2, #0]
	movs	r0, #1
	movs	r1, #1
	b.n	.L_08024f84
.L_08024f80:
	subs	r3, #1
	str	r3, [r2, #0]
.L_08024f84:
	adds	r3, r1, #0
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08024f98
	mov	r3, fp
	ldr	r0, [sp, #32]
	add	r3, sl
	ldrb	r6, [r0, r3]
	b.n	.L_08025106
.L_08024f98:
	ldr	r3, [pc, #472]
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #76]
	cmp	r3, #0
	beq.n	.L_08024faa
	movs	r3, #2
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_08024fb6
.L_08024faa:
	movs	r0, #113
	movs	r6, #1
	bl	sub_080f9010
	negs	r6, r6
	b.n	.L_08025106
.L_08024fb6:
	movs	r3, #128
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_08024fe6
	movs	r0, #111
	bl	sub_080f9010
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	cmp	r2, #4
	beq.n	.L_08024fd8
	mov	r3, fp
	ldr	r0, [sp, #80]
	add	r3, sl
	cmp	r3, r0
	bne.n	.L_08024fdc
.L_08024fd8:
	movs	r1, #0
	mov	sl, r1
.L_08024fdc:
	mov	r3, sl
	mov	r2, sl
	lsls	r3, r3, #1
	str	r2, [sp, #48]
	b.n	.L_080250fc
.L_08024fe6:
	movs	r3, #64
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_0802502c
	movs	r0, #111
	bl	sub_080f9010
	movs	r0, #1
	negs	r0, r0
	add	sl, r0
	mov	r1, sl
	cmp	r1, #0
	bge.n	.L_08025022
	ldr	r3, [sp, #20]
	cmp	r3, #0
	bge.n	.L_0802500a
	ldr	r3, [sp, #80]
	adds	r3, #2
.L_0802500a:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	cmp	fp, r3
	bne.n	.L_0802501e
	ldr	r2, [sp, #80]
	mov	r0, fp
	subs	r3, r2, r0
	subs	r3, #1
	mov	sl, r3
	b.n	.L_08025022
.L_0802501e:
	movs	r1, #3
	mov	sl, r1
.L_08025022:
	mov	r3, sl
	mov	r2, sl
	lsls	r3, r3, #1
	str	r2, [sp, #48]
	b.n	.L_080250fc
.L_0802502c:
	movs	r3, #16
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_0802508a
	movs	r0, #111
	bl	sub_080f9010
	bl	sub_0800352c
	mov	r3, fp
	ldr	r0, [sp, #80]
	adds	r3, #4
	cmp	r3, r0
	blt.n	.L_0802505e
	mov	r1, fp
	cmp	r1, #0
	beq.n	.L_080250fe
	ldr	r3, [sp, #48]
	mov	sl, r3
	mov	r0, sl
	movs	r2, #0
	lsls	r0, r0, #1
	mov	fp, r2
	str	r0, [sp, #16]
	b.n	.L_080250fe
.L_0802505e:
	mov	fp, r3
	ldr	r1, [sp, #48]
	ldr	r3, [sp, #20]
	mov	sl, r1
	cmp	r3, #0
	bge.n	.L_0802506e
	ldr	r3, [sp, #80]
	adds	r3, #2
.L_0802506e:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	cmp	fp, r3
	bne.n	.L_080250e0
	ldr	r2, [sp, #80]
	mov	r0, fp
	subs	r3, r2, r0
	subs	r3, #1
	ldr	r1, [sp, #48]
	mov	sl, r3
	cmp	sl, r1
	ble.n	.L_080250e8
	mov	sl, r1
	b.n	.L_080250f0
.L_0802508a:
	movs	r3, #32
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080250fe
	movs	r0, #111
	bl	sub_080f9010
	bl	sub_0800352c
	mov	r3, fp
	cmp	r3, #0
	beq.n	.L_080250b4
	ldr	r1, [sp, #48]
	mov	sl, r1
	movs	r0, #4
	mov	r2, sl
	negs	r0, r0
	lsls	r2, r2, #1
	add	fp, r0
	str	r2, [sp, #16]
	b.n	.L_080250fe
.L_080250b4:
	ldr	r3, [sp, #20]
	cmp	r3, #0
	bge.n	.L_080250be
	ldr	r3, [sp, #80]
	adds	r3, #2
.L_080250be:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	mov	fp, r3
	ldr	r3, [sp, #48]
	mov	r0, fp
	mov	sl, r3
	cmp	r0, #0
	beq.n	.L_080250f0
	ldr	r1, [sp, #80]
	subs	r3, r1, r0
	subs	r3, #1
	ldr	r2, [sp, #48]
	mov	sl, r3
	cmp	sl, r2
	ble.n	.L_080250f8
	mov	sl, r2
	b.n	.L_080250f8
.L_080250e0:
	mov	r0, sl
	lsls	r0, r0, #1
	str	r0, [sp, #16]
	b.n	.L_080250fe
.L_080250e8:
	mov	r1, sl
	lsls	r1, r1, #1
	str	r1, [sp, #16]
	b.n	.L_080250fe
.L_080250f0:
	mov	r2, sl
	lsls	r2, r2, #1
	str	r2, [sp, #16]
	b.n	.L_080250fe
.L_080250f8:
	mov	r3, sl
	lsls	r3, r3, #1
.L_080250fc:
	str	r3, [sp, #16]
.L_080250fe:
	movs	r0, #1
	bl	sub_080030f8
	b.n	.L_08024ae2
.L_08025106:
	movs	r0, #1
	bl	sub_080030f8
	movs	r4, #3
	add	r5, sp, #96
.L_08025110:
	ldmia	r5!, {r0}
	str	r4, [sp, #4]
	bl	sub_08003f3c
	ldr	r4, [sp, #4]
	subs	r4, #1
	cmp	r4, #0
	bge.n	.L_08025110
	ldr	r1, [sp, #12]
	lsrs	r0, r1, #16
	bl	sub_08003f3c
	movs	r1, #1
	ldr	r0, [sp, #56]
	bl	sub_08016418
	movs	r1, #1
	ldr	r0, [sp, #60]
	bl	sub_08016418
	movs	r1, #1
	ldr	r0, [sp, #76]
	bl	sub_08016418
	movs	r0, #1
	bl	sub_080030f8
	adds	r0, r6, #0
	add	sp, #372
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e40
	.4byte 0x0000f301
	.4byte 0x0000f30b
	.4byte 0x0000f334
	.4byte 0x0000f335
	.4byte 0x00000ea3
	.4byte 0x03001f34
	.4byte 0x03001c94
	.4byte 0x03001b04

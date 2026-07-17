@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08022b44
	.thumb_func
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
	bl	Func_08077008
	str	r0, [sp, #56]
	mov	r1, sl
	ldr	r0, [r1, #0]
	cmp	r0, #0
	beq.n	.L0
	movs	r1, #1
	bl	Func_08016418
.L0:
	ldr	r2, [sp, #60]
	cmp	r2, #0
	bne.n	.L1
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r0, #0
	movs	r3, #11
	movs	r1, #8
	movs	r2, #21
	bl	Func_080162d4
	mov	r3, sl
	str	r0, [r3, #0]
	b.n	.L2
.L1:
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r0, #0
	movs	r1, #5
	movs	r2, #21
	movs	r3, #14
	bl	Func_080162d4
	mov	r4, sl
	str	r0, [r4, #0]
.L2:
	mov	r1, sl
	ldr	r3, [r1, #0]
	movs	r0, #0
	cmp	r3, #0
	bne.n	.L3
	b.n	.L4
.L3:
	movs	r0, #128
	bl	Func_08004938
	movs	r5, #166
	lsls	r5, r5, #1
	str	r0, [sp, #48]
	adds	r0, r5, #0
	bl	Func_08004938
	str	r0, [sp, #52]
	movs	r0, #96
	bl	Func_08004970
	adds	r2, r5, #0
	str	r0, [sp, #24]
	ldr	r3, [pc, #640]
	ldr	r1, [sp, #56]
	ldr	r0, [sp, #52]
	bl	Func_080072f0
	mov	r2, r8
	asrs	r5, r2, #8
	movs	r3, #15
	movs	r6, #255
	ands	r5, r3
	ands	r6, r2
	adds	r0, r7, #0
	adds	r1, r5, #0
	adds	r2, r6, #0
	bl	Func_08077208
	cmp	r0, #0
	beq.n	.L5
	adds	r0, r7, #0
	adds	r1, r5, #0
	adds	r2, r6, #0
	bl	Func_080771b8
	b.n	.L6
.L5:
	adds	r0, r7, #0
	adds	r1, r5, #0
	adds	r2, r6, #0
	bl	Func_080771b0
.L6:
	adds	r0, r7, #0
	bl	Func_08077010
	ldr	r0, [sp, #52]
	ldr	r1, [sp, #56]
	add	r2, sp, #64
	add	r3, sp, #68
	str	r2, [sp, #0]
	adds	r1, #88
	ldr	r2, [sp, #24]
	adds	r0, #88
	bl	Func_080228e4
	movs	r1, #5
	str	r0, [sp, #20]
	subs	r0, #1
	bl	Func_080022ec
	ldr	r3, [sp, #108]
	adds	r0, #1
	str	r0, [r3, #0]
	ldr	r4, [sp, #60]
	lsls	r3, r4, #2
	adds	r3, r3, r4
	ldr	r1, [sp, #20]
	subs	r3, #5
	cmp	r3, r1
	blt.n	.L7
	str	r0, [sp, #60]
.L7:
	ldr	r2, [sp, #60]
	cmp	r2, #0
	beq.n	.L8
	b.n	.L9
.L8:
	ldr	r5, [pc, #520]
	mov	r3, sl
	ldr	r1, [r3, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #8
	bl	Func_0801e7c0
	mov	r4, sl
	ldr	r1, [r4, #0]
	adds	r0, r5, #1
	movs	r2, #0
	movs	r3, #16
	bl	Func_0801e7c0
	mov	r2, sl
	adds	r0, r5, #2
	ldr	r1, [r2, #0]
	movs	r3, #24
	movs	r2, #0
	bl	Func_0801e7c0
	mov	r3, sl
	adds	r0, r5, #3
	ldr	r1, [r3, #0]
	movs	r2, #0
	movs	r3, #32
	bl	Func_0801e7c0
	mov	r4, sl
	ldr	r1, [r4, #0]
	adds	r0, r5, #4
	movs	r2, #0
	movs	r3, #40
	bl	Func_0801e7c0
	adds	r5, #5
	mov	r2, sl
	ldr	r1, [r2, #0]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #48
	bl	Func_0801e7c0
	ldr	r3, [sp, #52]
	movs	r4, #56
	ldrsh	r3, [r3, r4]
	ldr	r0, [sp, #48]
	adds	r1, r3, #0
	str	r3, [sp, #44]
	bl	Func_080228bc
	ldr	r5, [sp, #48]
	mov	r4, sl
	adds	r5, #14
	ldr	r1, [r4, #0]
	movs	r2, #5
	movs	r3, #1
	adds	r0, r5, #0
	bl	Func_08017c8c
	ldr	r1, [sp, #52]
	movs	r2, #58
	ldrsh	r1, [r1, r2]
	ldr	r0, [sp, #48]
	str	r1, [sp, #40]
	bl	Func_080228bc
	mov	r2, sl
	ldr	r1, [r2, #0]
	adds	r0, r5, #0
	movs	r2, #5
	movs	r3, #2
	bl	Func_08017c8c
	ldr	r3, [sp, #52]
	ldrh	r3, [r3, #60]
	ldr	r0, [sp, #48]
	adds	r1, r3, #0
	str	r3, [sp, #36]
	bl	Func_080228bc
	ldr	r5, [sp, #48]
	mov	r4, sl
	adds	r5, #16
	ldr	r1, [r4, #0]
	movs	r2, #6
	movs	r3, #3
	adds	r0, r5, #0
	bl	Func_08017c8c
	ldr	r1, [sp, #52]
	ldrh	r1, [r1, #62]
	ldr	r0, [sp, #48]
	str	r1, [sp, #32]
	bl	Func_080228bc
	mov	r2, sl
	ldr	r1, [r2, #0]
	movs	r3, #4
	movs	r2, #6
	adds	r0, r5, #0
	bl	Func_08017c8c
	ldr	r3, [sp, #52]
	adds	r3, #64
	ldrh	r3, [r3, #0]
	ldr	r0, [sp, #48]
	adds	r1, r3, #0
	str	r3, [sp, #28]
	bl	Func_080228bc
	mov	r3, sl
	ldr	r1, [r3, #0]
	movs	r2, #6
	movs	r3, #5
	adds	r0, r5, #0
	bl	Func_08017c8c
	ldr	r3, [sp, #52]
	adds	r3, #66
	ldrb	r1, [r3, #0]
	ldr	r0, [sp, #48]
	bl	Func_080228bc
	mov	r4, sl
	ldr	r1, [r4, #0]
	movs	r2, #5
	movs	r3, #6
	adds	r0, r5, #0
	bl	Func_08017c8c
	mov	r1, sl
	movs	r3, #8
	ldr	r0, [r1, #0]
	movs	r2, #8
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r3, #19
	bl	Func_0801e41c
	ldr	r3, [sp, #68]
	cmp	r3, #0
	bne.n	.L10
	ldr	r3, [sp, #64]
	cmp	r3, #0
	beq.n	.L11
.L10:
	movs	r0, #2
	bl	Func_0801e71c
.L11:
	mov	r2, sl
	ldr	r0, [pc, #224]
	ldr	r1, [r2, #0]
	movs	r3, #64
	movs	r2, #24
	bl	Func_0801e7c0
	movs	r0, #15
	bl	Func_0801e71c
.L9:
	ldr	r3, [sp, #60]
	cmp	r3, #0
	bgt.n	.L12
	b.n	.L13
.L12:
	ldr	r1, [sp, #20]
	movs	r4, #0
	str	r4, [sp, #12]
	str	r1, [sp, #8]
	cmp	r1, #4
	ble.n	.L14
	movs	r2, #5
	str	r2, [sp, #8]
.L14:
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
	blt.n	.L15
	b.n	.L16
.L15:
	ldr	r4, [sp, #20]
	cmp	r8, r4
	blt.n	.L17
	b.n	.L16
.L17:
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
.L24:
	ldrh	r0, [r6, #0]
	bl	Func_08077080
	adds	r5, r0, #0
	ldrb	r3, [r5, #2]
	cmp	r3, #4
	beq.n	.L18
	mov	r1, sl
	ldr	r2, [pc, #116]
	ldr	r0, [r1, #0]
	adds	r1, r3, #0
	movs	r3, #0
	adds	r1, r1, r2
	str	r3, [sp, #0]
	movs	r2, #15
	mov	r3, r9
	bl	Func_08019000
.L18:
	ldrb	r3, [r5, #8]
	cmp	r3, #255
	bne.n	.L19
	movs	r3, #11
	b.n	.L20
.L19:
	subs	r3, #1
.L20:
	movs	r4, #0
	ldr	r0, [r7, #0]
	movs	r1, #16
	mov	r2, r9
	str	r4, [sp, #0]
	bl	Func_080218dc
	ldrh	r3, [r6, #0]
	ldr	r1, [pc, #72]
	ldr	r2, [sp, #4]
	ands	r3, r1
	ldr	r0, [r7, #0]
	movs	r1, #0
	bl	Func_08022a38
	ldrh	r2, [r6, #0]
	ldr	r3, [pc, #32]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L21
	movs	r0, #4
	bl	Func_0801e71c
	b.n	.L22
.L21:
	ldr	r3, [pc, #20]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L23
	movs	r0, #2
	bl	Func_0801e71c
	b.n	.L22
	movs	r0, r0
	.4byte 0x00008000
	.4byte 0x00004000
	.4byte 0x03001388
	.4byte 0x000008ae
	.4byte 0x000008ad
	.4byte 0x00005001
	.4byte 0x00003fff
.L23:
	movs	r0, #15
	bl	Func_0801e71c
.L22:
	ldrh	r3, [r6, #0]
	ldr	r0, [pc, #724]
	ands	r0, r3
	ldr	r3, [pc, #724]
	ldr	r1, [r7, #0]
	adds	r0, r0, r3
	movs	r2, #16
	mov	r3, fp
	bl	Func_0801e7c0
	movs	r2, #0
	ldr	r0, [r7, #0]
	mov	r3, r9
	str	r2, [sp, #0]
	ldr	r1, [pc, #708]
	movs	r2, #11
	bl	Func_08019000
	movs	r3, #0
	ldr	r0, [r7, #0]
	ldr	r1, [pc, #700]
	str	r3, [sp, #0]
	movs	r2, #12
	mov	r3, r9
	bl	Func_08019000
	ldrh	r0, [r6, #0]
	bl	Func_08077080
	mov	r4, fp
	ldr	r2, [r7, #0]
	ldrb	r0, [r0, #9]
	movs	r1, #2
	movs	r3, #104
	str	r4, [sp, #0]
	bl	Func_0801ea08
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
	bge.n	.L16
	ldr	r3, [sp, #20]
	cmp	r8, r3
	bge.n	.L16
	b.n	.L24
.L16:
	ldr	r3, [sp, #68]
	cmp	r3, #0
	beq.n	.L25
	movs	r0, #4
	bl	Func_0801e71c
	mov	r4, sl
	ldr	r1, [r4, #0]
	ldr	r0, [pc, #612]
	movs	r2, #32
	movs	r3, #80
	bl	Func_0801e7c0
	movs	r1, #1
	str	r1, [sp, #12]
.L25:
	ldr	r3, [sp, #64]
	cmp	r3, #0
	beq.n	.L26
	movs	r0, #2
	bl	Func_0801e71c
	ldr	r4, [sp, #12]
	mov	r2, sl
	lsls	r3, r4, #3
	ldr	r1, [r2, #0]
	ldr	r0, [pc, #584]
	adds	r3, #80
	movs	r2, #32
	bl	Func_0801e7c0
	ldr	r1, [sp, #12]
	adds	r1, #1
	str	r1, [sp, #12]
.L26:
	ldr	r2, [sp, #12]
	cmp	r2, #0
	bne.n	.L27
	mov	r3, sl
	ldr	r1, [r3, #0]
	ldr	r0, [pc, #560]
	movs	r2, #32
	movs	r3, #80
	bl	Func_0801e7c0
.L27:
	movs	r0, #15
	bl	Func_0801e71c
	movs	r0, #15
	bl	Func_0801e71c
	movs	r3, #10
	mov	r4, sl
	ldr	r0, [r4, #0]
	movs	r1, #0
	str	r3, [sp, #0]
	movs	r2, #10
	movs	r3, #19
	bl	Func_0801e41c
.L13:
	ldr	r1, [sp, #60]
	cmp	r1, #0
	beq.n	.L28
	b.n	.L29
.L28:
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
	bl	Func_0801e7c0
	ldr	r4, [sp, #56]
	adds	r5, r4, r5
	ldrb	r0, [r5, #0]
	mov	r2, sl
	ldr	r1, [r2, #0]
	movs	r3, #0
	movs	r2, #80
	adds	r0, r0, r6
	bl	Func_0801e7c0
	mov	r3, r8
	ldrb	r2, [r3, #0]
	ldrb	r3, [r5, #0]
	cmp	r2, r3
	beq.n	.L30
	ldr	r2, [sp, #60]
	mov	r4, sl
	ldr	r0, [r4, #0]
	ldr	r1, [pc, #464]
	str	r2, [sp, #0]
	movs	r3, #0
	movs	r2, #9
	bl	Func_08019000
	b.n	.L31
.L30:
	mov	r3, sl
	ldr	r4, [sp, #60]
	ldr	r0, [r3, #0]
	ldr	r1, [pc, #448]
	movs	r2, #9
	movs	r3, #0
	str	r4, [sp, #0]
	bl	Func_08019000
.L31:
	ldr	r3, [sp, #56]
	ldr	r0, [sp, #48]
	movs	r2, #56
	ldrsh	r1, [r3, r2]
	bl	Func_080228bc
	ldr	r6, [sp, #48]
	mov	r4, sl
	adds	r6, #14
	ldr	r1, [r4, #0]
	movs	r2, #11
	movs	r3, #1
	adds	r0, r6, #0
	bl	Func_08017c8c
	ldr	r2, [sp, #56]
	ldr	r4, [sp, #44]
	movs	r1, #56
	ldrsh	r3, [r2, r1]
	cmp	r3, r4
	beq.n	.L32
	movs	r2, #0
	cmp	r3, r4
	ble.n	.L33
	movs	r2, #1
.L33:
	add	r1, sp, #76
	mov	r9, r1
	movs	r0, #80
	movs	r1, #14
	bl	Func_08022a7c
.L32:
	ldr	r3, [sp, #56]
	ldr	r0, [sp, #48]
	movs	r2, #58
	ldrsh	r1, [r3, r2]
	bl	Func_080228bc
	mov	r4, sl
	ldr	r1, [r4, #0]
	movs	r2, #11
	movs	r3, #2
	adds	r0, r6, #0
	bl	Func_08017c8c
	ldr	r2, [sp, #56]
	ldr	r4, [sp, #40]
	movs	r1, #58
	ldrsh	r3, [r2, r1]
	cmp	r3, r4
	beq.n	.L34
	movs	r2, #0
	cmp	r3, r4
	ble.n	.L35
	movs	r2, #1
.L35:
	add	r1, sp, #76
	mov	r9, r1
	movs	r0, #80
	movs	r1, #22
	bl	Func_08022a7c
.L34:
	ldr	r2, [sp, #56]
	ldr	r0, [sp, #48]
	ldrh	r1, [r2, #60]
	bl	Func_080228bc
	mov	r3, sl
	ldr	r1, [r3, #0]
	adds	r0, r6, #0
	movs	r3, #3
	movs	r2, #11
	bl	Func_08017c8c
	ldr	r4, [sp, #56]
	ldr	r1, [sp, #36]
	ldrh	r3, [r4, #60]
	cmp	r3, r1
	beq.n	.L36
	movs	r2, #0
	cmp	r3, r1
	ble.n	.L37
	movs	r2, #1
.L37:
	add	r3, sp, #76
	mov	r9, r3
	movs	r0, #80
	movs	r1, #30
	bl	Func_08022a7c
.L36:
	ldr	r4, [sp, #56]
	ldr	r0, [sp, #48]
	ldrh	r1, [r4, #62]
	bl	Func_080228bc
	mov	r2, sl
	ldr	r1, [r2, #0]
	movs	r3, #4
	adds	r0, r6, #0
	movs	r2, #11
	bl	Func_08017c8c
	ldr	r4, [sp, #56]
	ldr	r1, [sp, #32]
	ldrh	r3, [r4, #62]
	cmp	r3, r1
	beq.n	.L38
	movs	r2, #0
	cmp	r3, r1
	ble.n	.L39
	movs	r2, #1
.L39:
	add	r3, sp, #76
	mov	r9, r3
	movs	r0, #80
	movs	r1, #38
	bl	Func_08022a7c
.L38:
	ldr	r5, [sp, #56]
	adds	r5, #64
	ldrh	r1, [r5, #0]
	ldr	r0, [sp, #48]
	bl	Func_080228bc
	mov	r4, sl
	ldr	r1, [r4, #0]
	movs	r3, #5
	adds	r0, r6, #0
	movs	r2, #11
	bl	Func_08017c8c
	ldrh	r3, [r5, #0]
	ldr	r1, [sp, #28]
	cmp	r3, r1
	beq.n	.L40
	movs	r2, #0
	cmp	r3, r1
	ble.n	.L41
	movs	r2, #1
.L41:
	add	r3, sp, #76
	mov	r9, r3
	movs	r0, #80
	movs	r1, #46
	bl	Func_08022a7c
.L40:
	ldr	r5, [sp, #56]
	adds	r5, #66
	ldrb	r1, [r5, #0]
	ldr	r0, [sp, #48]
	bl	Func_080228bc
	ldr	r0, [sp, #48]
	mov	r4, sl
	ldr	r1, [r4, #0]
	movs	r3, #6
	adds	r0, #16
	movs	r2, #12
	bl	Func_08017c8c
	ldr	r3, [sp, #52]
	adds	r3, #66
	ldrb	r1, [r5, #0]
	ldrb	r3, [r3, #0]
	cmp	r1, r3
	beq.n	.L29
	movs	r2, #0
	cmp	r1, r3
	bls.n	.L42
	movs	r2, #1
.L42:
	add	r1, sp, #76
	mov	r9, r1
	movs	r0, #80
	movs	r1, #54
	bl	Func_08022a7c
.L29:
	movs	r2, #166
	lsls	r2, r2, #1
	ldr	r3, [pc, #92]
	ldr	r0, [sp, #56]
	ldr	r1, [sp, #52]
	bl	Func_080072f0
	ldr	r0, [sp, #24]
	bl	Func_08002df0
	ldr	r0, [sp, #52]
	bl	Func_08002df0
	ldr	r0, [sp, #48]
	bl	Func_08002df0
	mov	r2, sl
	ldr	r0, [r2, #0]
.L4:
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

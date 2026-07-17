@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800aa0c
	.thumb_func
Func_0800aa0c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	lsls	r1, r1, #16
	sub	sp, #56
	asrs	r7, r1, #16
	movs	r1, #0
	str	r0, [sp, #44]
	str	r1, [sp, #40]
	ldr	r3, [pc, #772]
	ldr	r2, [r3, #0]
	movs	r4, #1
	str	r2, [sp, #32]
	str	r4, [sp, #24]
	adds	r5, r3, #0
	adds	r5, #184
	ldr	r1, [r5, #0]
	str	r1, [sp, #28]
	cmp	r1, #0
	bne.n	.L0
	ldr	r1, [pc, #752]
	movs	r0, #52
	bl	Func_080048b0
	ldr	r2, [pc, #748]
	adds	r1, r0, #0
	ldr	r0, [pc, #748]
	movs	r4, #132
	subs	r2, r2, r0
	lsls	r4, r4, #24
	lsrs	r2, r2, #2
	ldr	r3, [pc, #744]
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [r5, #0]
	movs	r2, #0
	str	r5, [sp, #28]
	str	r2, [sp, #24]
.L0:
	ldr	r4, [sp, #44]
	movs	r3, #0
	adds	r4, #39
	ldr	r1, [sp, #44]
	mov	r8, r3
	ldr	r2, [sp, #40]
	ldrb	r3, [r4, #0]
	adds	r1, #37
	mov	fp, r4
	str	r1, [sp, #0]
	cmp	r2, r3
	blt.n	.L1
	b.n	.L2
.L1:
	mov	r4, r8
	lsls	r3, r4, #2
	ldr	r1, [sp, #44]
	adds	r3, #40
	ldr	r6, [r1, r3]
	cmp	r6, #0
	bne.n	.L3
	b.n	.L4
.L3:
	ldr	r3, [r6, #16]
	cmp	r3, #0
	bne.n	.L5
	b.n	.L4
.L5:
	movs	r4, #2
	ldrsh	r3, [r6, r4]
	ldrh	r2, [r6, #2]
	cmp	r3, #0
	bgt.n	.L6
	ldrb	r3, [r6, #20]
	adds	r2, r3, #1
	strb	r2, [r6, #20]
	ldr	r1, [r6, #16]
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	ldrb	r0, [r1, r3]
	adds	r3, r2, #1
	strb	r3, [r6, #20]
	lsls	r2, r2, #24
	adds	r3, r0, #0
	lsrs	r2, r2, #24
	subs	r3, #239
	ldrb	r5, [r1, r2]
	cmp	r3, #16
	bhi.n	.L7
	ldr	r2, [pc, #640]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x0800ab46
	.4byte 0x0800ab32
	.4byte 0x0800ab2a
	.4byte 0x0800aa94
	.4byte 0x0800aa94
	.4byte 0x0800aa94
	.4byte 0x0800ab20
	.4byte 0x0800aa94
	.4byte 0x0800aa94
	.4byte 0x0800aa94
	.4byte 0x0800aa94
	.4byte 0x0800aa94
	.4byte 0x0800ab5a
	.4byte 0x0800aa94
	.4byte 0x0800ab1c
	.4byte 0x0800ab0c
	.4byte 0x0800ab36
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	Func_0800b9f4
	ldr	r3, [sp, #44]
	adds	r3, #36
	strb	r5, [r3, #0]
	b.n	.L5
	strb	r5, [r6, #20]
	b.n	.L5
	ldrh	r3, [r6, #2]
	lsls	r2, r5, #4
	adds	r3, r3, r2
	strh	r3, [r6, #2]
	b.n	.L5
	ldrb	r3, [r6, #20]
	adds	r3, #254
	strb	r3, [r6, #20]
	b.n	.L8
	strb	r5, [r6, #4]
	b.n	.L5
	movs	r3, #255
	strb	r3, [r6, #23]
	ldrh	r3, [r6, #2]
	lsls	r2, r5, #4
	adds	r3, r3, r2
	movs	r0, #255
	strh	r3, [r6, #2]
	b.n	.L9
	movs	r3, #255
	mov	r1, fp
	strb	r3, [r6, #23]
	movs	r3, #0
	str	r3, [r6, #16]
	ldrb	r3, [r1, #0]
	adds	r3, #255
	movs	r0, #255
	strb	r3, [r1, #0]
	b.n	.L9
.L7:
	ldrh	r3, [r6, #2]
	lsls	r2, r5, #4
	adds	r3, r3, r2
	strb	r0, [r6, #23]
	strh	r3, [r6, #2]
	b.n	.L9
.L6:
	ldrb	r3, [r6, #21]
	subs	r3, r2, r3
	strh	r3, [r6, #2]
.L8:
	ldrb	r0, [r6, #23]
.L9:
	ldrb	r3, [r6, #4]
	subs	r3, #1
	cmp	r3, #87
	bls.n	.L10
	b.n	.L11
.L10:
	ldr	r2, [pc, #456]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0800ace0
	.4byte 0x0800ace6
	.4byte 0x0800acf2
	.4byte 0x0800acf6
	.4byte 0x0800acfa
	.4byte 0x0800ad04
	.4byte 0x0800ad6c
	.4byte 0x0800ad0e
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ace6
	.4byte 0x0800ad6c
	.4byte 0x0800acec
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad6c
	.4byte 0x0800ad1a
	ldr	r2, [pc, #100]
	lsls	r3, r7, #16
	b.n	.L12
	ldr	r2, [pc, #100]
	lsls	r3, r7, #16
	b.n	.L12
	ldr	r2, [pc, #96]
	lsls	r3, r7, #16
	b.n	.L12
	ldr	r2, [pc, #96]
	b.n	.L13
	ldr	r2, [pc, #96]
	b.n	.L14
	ldr	r2, [pc, #96]
.L13:
	lsls	r3, r7, #16
	lsrs	r3, r3, #28
	ldrb	r2, [r2, r3]
	b.n	.L15
	ldr	r2, [pc, #88]
.L14:
	lsls	r3, r7, #16
	lsrs	r3, r3, #26
	ldrb	r2, [r2, r3]
	b.n	.L15
	movs	r4, #128
	lsls	r3, r7, #16
	lsls	r4, r4, #21
	ldr	r2, [pc, #76]
	adds	r3, r3, r4
	b.n	.L12
	movs	r1, #128
	lsls	r3, r7, #16
	lsls	r1, r1, #21
	ldr	r2, [pc, #68]
	adds	r3, r3, r1
.L12:
	lsrs	r3, r3, #29
	ldrb	r2, [r2, r3]
	b.n	.L15
	movs	r0, r0
	.4byte 0x03001e68
	.4byte 0x000002c4
	.4byte 0x08009d9c
	.4byte 0x08009bb8
	.4byte 0x040000d4
	.4byte 0x0800aac8
	.4byte 0x0800ab80
	.4byte 0x0801307c
	.4byte 0x08013094
	.4byte 0x0801308c
	.4byte 0x0801309c
	.4byte 0x080130cc
	.4byte 0x080130ac
	.4byte 0x0801310c
	.4byte 0x080130bc
	.4byte 0x080130c4
.L11:
	movs	r2, #0
.L15:
	movs	r3, #7
	ands	r3, r2
	adds	r0, r0, r3
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L16
	lsrs	r3, r2, #7
	cmp	r3, #0
	beq.n	.L16
	movs	r4, #1
	str	r4, [sp, #40]
.L16:
	ldrb	r3, [r6, #22]
	cmp	r3, r0
	beq.n	.L4
	strb	r0, [r6, #22]
	ldr	r1, [sp, #0]
	movs	r3, #1
	strb	r3, [r1, #0]
.L4:
	mov	r4, fp
	movs	r2, #1
	ldrb	r3, [r4, #0]
	add	r8, r2
	cmp	r8, r3
	bge.n	.L2
	b.n	.L1
.L2:
	ldr	r1, [sp, #0]
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	bne.n	.L17
	b.n	.L18
.L17:
	ldr	r2, [sp, #44]
	ldr	r3, [sp, #44]
	adds	r2, #32
	str	r2, [sp, #20]
	adds	r3, #33
	ldrb	r2, [r2, #0]
	str	r3, [sp, #16]
	ldrb	r3, [r3, #0]
	adds	r4, r3, #0
	muls	r4, r2
	adds	r0, r4, #0
	str	r4, [sp, #36]
	bl	Func_08004938
	ldr	r3, [pc, #512]
	ldr	r1, [sp, #36]
	mov	sl, r0
	bl	Func_080072f0
	mov	r2, fp
	ldrb	r3, [r2, #0]
	movs	r1, #1
	negs	r1, r1
	subs	r3, #1
	mov	r9, r1
	mov	r8, r3
	cmp	r3, #0
	blt.n	.L19
	ldr	r4, [sp, #44]
	lsls	r3, r3, #2
	adds	r3, r3, r4
	adds	r3, #40
	movs	r1, #0
	mov	ip, r3
	mov	lr, r1
.L26:
	mov	r2, ip
	movs	r3, #4
	ldr	r6, [r2, #0]
	negs	r3, r3
	add	ip, r3
	cmp	r6, #0
	beq.n	.L20
	ldr	r3, [r6, #8]
	cmp	r3, #0
	beq.n	.L20
	ldrb	r3, [r6, #22]
	cmp	r3, #255
	beq.n	.L20
	ldrb	r0, [r6, #6]
	cmp	r0, #3
	bhi.n	.L20
	lsls	r0, r0, #8
	mov	r4, r8
	mov	r6, r9
	orrs	r0, r4
	cmp	r6, #0
	blt.n	.L21
	add	r5, sp, #48
	lsls	r2, r6, #1
	ldrh	r3, [r5, r2]
	cmp	r3, r0
	bls.n	.L22
	mov	r1, lr
	strh	r3, [r5, r1]
	adds	r3, r2, r5
	adds	r7, r5, #0
	adds	r4, r3, #2
	adds	r1, r2, #0
.L25:
	subs	r6, #1
	subs	r1, #2
	cmp	r6, #0
	blt.n	.L23
	adds	r3, r1, #0
	ldrh	r2, [r3, r7]
	cmp	r2, r0
	bls.n	.L24
	subs	r4, #2
	strh	r2, [r4, #0]
	b.n	.L25
.L21:
	mov	r2, r9
	add	r5, sp, #48
	lsls	r3, r2, #1
	b.n	.L24
.L22:
	adds	r3, r2, #0
	b.n	.L24
.L23:
	lsls	r3, r6, #1
.L24:
	adds	r3, #2
	strh	r0, [r5, r3]
	movs	r4, #1
	movs	r3, #2
	add	lr, r3
	add	r9, r4
.L20:
	movs	r1, #1
	negs	r1, r1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #0
	bge.n	.L26
.L19:
	movs	r3, #1
	movs	r4, #0
	add	r9, r3
	mov	r8, r4
	cmp	r8, r9
	bge.n	.L27
.L32:
	mov	r1, r8
	lsls	r3, r1, #1
	add	r2, sp, #56
	adds	r3, r3, r2
	subs	r3, #8
	ldrb	r3, [r3, #0]
	ldr	r4, [sp, #44]
	lsls	r3, r3, #2
	adds	r3, #40
	ldr	r6, [r4, r3]
	ldrb	r3, [r6, #7]
	cmp	r3, #1
	bne.n	.L28
	ldrb	r3, [r6, #22]
	ldr	r2, [r6, #8]
	lsls	r3, r3, #2
	ldr	r0, [r3, r2]
	mov	r1, sl
	bl	Func_08005340
	b.n	.L29
.L28:
	cmp	r3, #3
	bne.n	.L30
	ldrb	r3, [r6, #5]
	cmp	r3, #0
	beq.n	.L31
	movs	r0, #128
	lsls	r0, r0, #3
	bl	Func_08004938
	ldrb	r3, [r6, #22]
	ldr	r2, [r6, #8]
	adds	r5, r0, #0
	lsls	r3, r3, #2
	ldr	r0, [r3, r2]
	adds	r1, r5, #0
	bl	Func_0800a97c
	ldrb	r2, [r6, #5]
	mov	r1, sl
	ldr	r3, [sp, #28]
	bl	Func_080072f0
	adds	r0, r5, #0
	bl	Func_08002df0
	b.n	.L29
.L31:
	ldrb	r3, [r6, #22]
	ldr	r2, [r6, #8]
	lsls	r3, r3, #2
	ldr	r0, [r3, r2]
	mov	r1, sl
	ldr	r3, [pc, #232]
	bl	Func_080072f0
	cmp	r0, #0
	beq.n	.L29
	mov	r1, sl
	movs	r2, #0
	ldr	r4, [sp, #28]
	bl	Func_080072f4
	b.n	.L29
.L30:
	ldrb	r3, [r6, #22]
	ldr	r2, [r6, #8]
	lsls	r3, r3, #2
	ldr	r0, [r3, r2]
	mov	r1, sl
	ldrb	r2, [r6, #5]
	ldr	r3, [sp, #28]
	bl	Func_080072f0
.L29:
	movs	r4, #1
	add	r8, r4
	cmp	r8, r9
	blt.n	.L32
.L27:
	ldr	r3, [sp, #44]
	adds	r3, #38
	ldrb	r2, [r3, #0]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L33
	ldr	r0, [sp, #36]
	bl	Func_08004938
	str	r0, [sp, #12]
	ldr	r3, [sp, #32]
	ldr	r1, [sp, #20]
	ldrb	r3, [r3, #6]
	ldr	r2, [sp, #16]
	ldrb	r7, [r1, #0]
	ldrb	r5, [r2, #0]
	ldr	r4, [sp, #32]
	str	r3, [sp, #8]
	ldrb	r4, [r4, #7]
	ldr	r3, [pc, #140]
	str	r4, [sp, #4]
	ldr	r1, [sp, #36]
	bl	Func_080072f0
	mov	r1, sl
	ldr	r2, [sp, #12]
	adds	r3, r1, r7
	adds	r1, r3, #1
	adds	r3, r2, r7
	adds	r4, r3, #1
	subs	r5, #1
	movs	r3, #1
	mov	r8, r3
	mov	r9, r5
	cmp	r8, r9
	bcs.n	.L34
	subs	r2, r7, #1
	mov	lr, r2
.L38:
	movs	r6, #1
	cmp	r6, lr
	bcs.n	.L35
	movs	r3, #1
	mov	fp, r3
	mov	ip, lr
	adds	r5, r1, r7
	subs	r0, r1, r7
	subs	r2, r1, #1
.L37:
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L36
	ldrb	r3, [r2, #2]
	cmp	r3, #0
	beq.n	.L36
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L36
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L36
	mov	r3, fp
	strb	r3, [r4, #0]
.L36:
	adds	r6, #1
	adds	r4, #1
	adds	r5, #1
	adds	r0, #1
	adds	r2, #1
	adds	r1, #1
	cmp	r6, ip
	bcc.n	.L37
.L35:
	movs	r2, #1
	add	r8, r2
	adds	r4, #2
	adds	r1, #2
	cmp	r8, r9
	bcc.n	.L38
.L34:
	movs	r3, #0
	ldr	r2, [sp, #36]
	mov	r8, r3
	mov	r1, sl
	ldr	r4, [sp, #12]
	cmp	r8, r2
	bcs.n	.L39
.L42:
	ldrb	r3, [r4, #0]
	cmp	r3, #0
	beq.n	.L40
	add	r3, sp, #4
	ldrb	r3, [r3, #0]
	strb	r3, [r1, #0]
	b.n	.L41
	movs	r0, r0
	.4byte 0x03000164
	.4byte 0x030005c0
.L40:
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L41
	add	r2, sp, #8
	ldrb	r2, [r2, #0]
	strb	r2, [r1, #0]
.L41:
	movs	r3, #1
	ldr	r2, [sp, #36]
	add	r8, r3
	adds	r4, #1
	adds	r1, #1
	cmp	r8, r2
	bcc.n	.L42
.L39:
	ldr	r0, [sp, #12]
	bl	Func_08002df0
.L33:
	ldr	r3, [sp, #44]
	ldr	r1, [sp, #36]
	ldrb	r0, [r3, #28]
	movs	r2, #0
	bl	Func_08003fa4
	ldr	r4, [pc, #72]
	adds	r5, r0, #0
	lsls	r3, r5, #5
	ldr	r0, [pc, #72]
	adds	r3, r3, r4
	ldr	r2, [sp, #20]
	ldr	r4, [sp, #16]
	adds	r0, #212
	ldrb	r1, [r2, #0]
	ldrb	r2, [r4, #0]
	ldr	r4, [r0, #0]
	mov	r0, sl
	bl	Func_080072f4
	ldr	r3, [pc, #40]
	ldr	r1, [sp, #44]
	ands	r5, r3
	ldrh	r2, [r1, #8]
	ldr	r3, [pc, #44]
	ands	r3, r2
	orrs	r3, r5
	adds	r2, r1, #0
	strh	r3, [r2, #8]
	ldr	r4, [sp, #0]
	movs	r3, #0
	strb	r3, [r4, #0]
	ldr	r1, [sp, #32]
	ldr	r2, [sp, #36]
	ldrh	r3, [r1, #0]
	adds	r4, r1, #0
	adds	r3, r3, r2
	strh	r3, [r4, #0]
	mov	r0, sl
	bl	Func_08002df0
	b.n	.L18
	.4byte 0x000003ff
	.4byte 0x06010000
	.4byte 0x03001e50
	.4byte 0xfffffc00
.L18:
	ldr	r1, [sp, #24]
	cmp	r1, #0
	bne.n	.L43
	movs	r0, #52
	bl	Func_08002dd8
.L43:
	ldr	r0, [sp, #40]
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1

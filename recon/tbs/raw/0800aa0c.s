.syntax unified
	.thumb
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002df0, 0x08002df0
	.set sub_08003fa4, 0x08003fa4
	.set sub_080048b0, 0x080048b0
	.set sub_08004938, 0x08004938
	.set sub_08005340, 0x08005340
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_0800a97c, 0x0800a97c
	.set sub_0800b9f4, 0x0800b9f4
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
	bne.n	.L_0800aa62
	ldr	r1, [pc, #752]
	movs	r0, #52
	bl	sub_080048b0
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
.L_0800aa62:
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
	blt.n	.L_0800aa7c
	b.n	.L_0800ada0
.L_0800aa7c:
	mov	r4, r8
	lsls	r3, r4, #2
	ldr	r1, [sp, #44]
	adds	r3, #40
	ldr	r6, [r1, r3]
	cmp	r6, #0
	bne.n	.L_0800aa8c
	b.n	.L_0800ad92
.L_0800aa8c:
	ldr	r3, [r6, #16]
	cmp	r3, #0
	bne.n	.L_0800aa94
	b.n	.L_0800ad92
.L_0800aa94:
	movs	r4, #2
	ldrsh	r3, [r6, r4]
	ldrh	r2, [r6, #2]
	cmp	r3, #0
	bgt.n	.L_0800ab66
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
	bhi.n	.L_0800ab5a
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
	.2byte 0xab36
	.2byte 0x0800
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_0800b9f4
	ldr	r3, [sp, #44]
	adds	r3, #36
	strb	r5, [r3, #0]
	b.n	.L_0800aa94
	strb	r5, [r6, #20]
	b.n	.L_0800aa94
	ldrh	r3, [r6, #2]
	lsls	r2, r5, #4
	adds	r3, r3, r2
	strh	r3, [r6, #2]
	b.n	.L_0800aa94
	ldrb	r3, [r6, #20]
	adds	r3, #254
	strb	r3, [r6, #20]
	b.n	.L_0800ab6c
	strb	r5, [r6, #4]
	b.n	.L_0800aa94
	movs	r3, #255
	strb	r3, [r6, #23]
	ldrh	r3, [r6, #2]
	lsls	r2, r5, #4
	adds	r3, r3, r2
	movs	r0, #255
	strh	r3, [r6, #2]
	b.n	.L_0800ab6e
	movs	r3, #255
	mov	r1, fp
	strb	r3, [r6, #23]
	movs	r3, #0
	str	r3, [r6, #16]
	ldrb	r3, [r1, #0]
	adds	r3, #255
	movs	r0, #255
	strb	r3, [r1, #0]
	b.n	.L_0800ab6e
.L_0800ab5a:
	ldrh	r3, [r6, #2]
	lsls	r2, r5, #4
	adds	r3, r3, r2
	strb	r0, [r6, #23]
	strh	r3, [r6, #2]
	b.n	.L_0800ab6e
.L_0800ab66:
	ldrb	r3, [r6, #21]
	subs	r3, r2, r3
	strh	r3, [r6, #2]
.L_0800ab6c:
	ldrb	r0, [r6, #23]
.L_0800ab6e:
	ldrb	r3, [r6, #4]
	subs	r3, #1
	cmp	r3, #87
	bls.n	.L_0800ab78
	b.n	.L_0800ad6c
.L_0800ab78:
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
	.2byte 0xad1a
	.2byte 0x0800
	ldr	r2, [pc, #100]
	lsls	r3, r7, #16
	b.n	.L_0800ad24
	ldr	r2, [pc, #100]
	lsls	r3, r7, #16
	b.n	.L_0800ad24
	ldr	r2, [pc, #96]
	lsls	r3, r7, #16
	b.n	.L_0800ad24
	ldr	r2, [pc, #96]
	b.n	.L_0800acfc
	ldr	r2, [pc, #96]
	b.n	.L_0800ad06
	ldr	r2, [pc, #96]
.L_0800acfc:
	lsls	r3, r7, #16
	lsrs	r3, r3, #28
	ldrb	r2, [r2, r3]
	b.n	.L_0800ad6e
	ldr	r2, [pc, #88]
.L_0800ad06:
	lsls	r3, r7, #16
	lsrs	r3, r3, #26
	ldrb	r2, [r2, r3]
	b.n	.L_0800ad6e
	movs	r4, #128
	lsls	r3, r7, #16
	lsls	r4, r4, #21
	ldr	r2, [pc, #76]
	adds	r3, r3, r4
	b.n	.L_0800ad24
	movs	r1, #128
	lsls	r3, r7, #16
	lsls	r1, r1, #21
	ldr	r2, [pc, #68]
	adds	r3, r3, r1
.L_0800ad24:
	lsrs	r3, r3, #29
	ldrb	r2, [r2, r3]
	b.n	.L_0800ad6e
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
	.2byte 0x30c4
	.2byte 0x0801
.L_0800ad6c:
	movs	r2, #0
.L_0800ad6e:
	movs	r3, #7
	ands	r3, r2
	adds	r0, r0, r3
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L_0800ad84
	lsrs	r3, r2, #7
	cmp	r3, #0
	beq.n	.L_0800ad84
	movs	r4, #1
	str	r4, [sp, #40]
.L_0800ad84:
	ldrb	r3, [r6, #22]
	cmp	r3, r0
	beq.n	.L_0800ad92
	strb	r0, [r6, #22]
	ldr	r1, [sp, #0]
	movs	r3, #1
	strb	r3, [r1, #0]
.L_0800ad92:
	mov	r4, fp
	movs	r2, #1
	ldrb	r3, [r4, #0]
	add	r8, r2
	cmp	r8, r3
	bge.n	.L_0800ada0
	b.n	.L_0800aa7c
.L_0800ada0:
	ldr	r1, [sp, #0]
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	bne.n	.L_0800adaa
	b.n	.L_0800b054
.L_0800adaa:
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
	bl	sub_08004938
	ldr	r3, [pc, #512]
	ldr	r1, [sp, #36]
	mov	sl, r0
	bl	sub_080072f0
	mov	r2, fp
	ldrb	r3, [r2, #0]
	movs	r1, #1
	negs	r1, r1
	subs	r3, #1
	mov	r9, r1
	mov	r8, r3
	cmp	r3, #0
	blt.n	.L_0800ae6e
	ldr	r4, [sp, #44]
	lsls	r3, r3, #2
	adds	r3, r3, r4
	adds	r3, #40
	movs	r1, #0
	mov	ip, r3
	mov	lr, r1
.L_0800adf0:
	mov	r2, ip
	movs	r3, #4
	ldr	r6, [r2, #0]
	negs	r3, r3
	add	ip, r3
	cmp	r6, #0
	beq.n	.L_0800ae62
	ldr	r3, [r6, #8]
	cmp	r3, #0
	beq.n	.L_0800ae62
	ldrb	r3, [r6, #22]
	cmp	r3, #255
	beq.n	.L_0800ae62
	ldrb	r0, [r6, #6]
	cmp	r0, #3
	bhi.n	.L_0800ae62
	lsls	r0, r0, #8
	mov	r4, r8
	mov	r6, r9
	orrs	r0, r4
	cmp	r6, #0
	blt.n	.L_0800ae48
	add	r5, sp, #48
	lsls	r2, r6, #1
	ldrh	r3, [r5, r2]
	cmp	r3, r0
	bls.n	.L_0800ae50
	mov	r1, lr
	strh	r3, [r5, r1]
	adds	r3, r2, r5
	adds	r7, r5, #0
	adds	r4, r3, #2
	adds	r1, r2, #0
.L_0800ae32:
	subs	r6, #1
	subs	r1, #2
	cmp	r6, #0
	blt.n	.L_0800ae54
	adds	r3, r1, #0
	ldrh	r2, [r3, r7]
	cmp	r2, r0
	bls.n	.L_0800ae56
	subs	r4, #2
	strh	r2, [r4, #0]
	b.n	.L_0800ae32
.L_0800ae48:
	mov	r2, r9
	add	r5, sp, #48
	lsls	r3, r2, #1
	b.n	.L_0800ae56
.L_0800ae50:
	adds	r3, r2, #0
	b.n	.L_0800ae56
.L_0800ae54:
	lsls	r3, r6, #1
.L_0800ae56:
	adds	r3, #2
	strh	r0, [r5, r3]
	movs	r4, #1
	movs	r3, #2
	add	lr, r3
	add	r9, r4
.L_0800ae62:
	movs	r1, #1
	negs	r1, r1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #0
	bge.n	.L_0800adf0
.L_0800ae6e:
	movs	r3, #1
	movs	r4, #0
	add	r9, r3
	mov	r8, r4
	cmp	r8, r9
	bge.n	.L_0800af12
.L_0800ae7a:
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
	bne.n	.L_0800aea4
	ldrb	r3, [r6, #22]
	ldr	r2, [r6, #8]
	lsls	r3, r3, #2
	ldr	r0, [r3, r2]
	mov	r1, sl
	bl	sub_08005340
	b.n	.L_0800af0a
.L_0800aea4:
	cmp	r3, #3
	bne.n	.L_0800aef8
	ldrb	r3, [r6, #5]
	cmp	r3, #0
	beq.n	.L_0800aed8
	movs	r0, #128
	lsls	r0, r0, #3
	bl	sub_08004938
	ldrb	r3, [r6, #22]
	ldr	r2, [r6, #8]
	adds	r5, r0, #0
	lsls	r3, r3, #2
	ldr	r0, [r3, r2]
	adds	r1, r5, #0
	bl	sub_0800a97c
	ldrb	r2, [r6, #5]
	mov	r1, sl
	ldr	r3, [sp, #28]
	bl	sub_080072f0
	adds	r0, r5, #0
	bl	sub_08002df0
	b.n	.L_0800af0a
.L_0800aed8:
	ldrb	r3, [r6, #22]
	ldr	r2, [r6, #8]
	lsls	r3, r3, #2
	ldr	r0, [r3, r2]
	mov	r1, sl
	ldr	r3, [pc, #232]
	bl	sub_080072f0
	cmp	r0, #0
	beq.n	.L_0800af0a
	mov	r1, sl
	movs	r2, #0
	ldr	r4, [sp, #28]
	bl	sub_080072f4
	b.n	.L_0800af0a
.L_0800aef8:
	ldrb	r3, [r6, #22]
	ldr	r2, [r6, #8]
	lsls	r3, r3, #2
	ldr	r0, [r3, r2]
	mov	r1, sl
	ldrb	r2, [r6, #5]
	ldr	r3, [sp, #28]
	bl	sub_080072f0
.L_0800af0a:
	movs	r4, #1
	add	r8, r4
	cmp	r8, r9
	blt.n	.L_0800ae7a
.L_0800af12:
	ldr	r3, [sp, #44]
	adds	r3, #38
	ldrb	r2, [r3, #0]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0800aff0
	ldr	r0, [sp, #36]
	bl	sub_08004938
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
	bl	sub_080072f0
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
	bcs.n	.L_0800afaa
	subs	r2, r7, #1
	mov	lr, r2
.L_0800af60:
	movs	r6, #1
	cmp	r6, lr
	bcs.n	.L_0800af9e
	movs	r3, #1
	mov	fp, r3
	mov	ip, lr
	adds	r5, r1, r7
	subs	r0, r1, r7
	subs	r2, r1, #1
.L_0800af72:
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_0800af8e
	ldrb	r3, [r2, #2]
	cmp	r3, #0
	beq.n	.L_0800af8e
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L_0800af8e
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_0800af8e
	mov	r3, fp
	strb	r3, [r4, #0]
.L_0800af8e:
	adds	r6, #1
	adds	r4, #1
	adds	r5, #1
	adds	r0, #1
	adds	r2, #1
	adds	r1, #1
	cmp	r6, ip
	bcc.n	.L_0800af72
.L_0800af9e:
	movs	r2, #1
	add	r8, r2
	adds	r4, #2
	adds	r1, #2
	cmp	r8, r9
	bcc.n	.L_0800af60
.L_0800afaa:
	movs	r3, #0
	ldr	r2, [sp, #36]
	mov	r8, r3
	mov	r1, sl
	ldr	r4, [sp, #12]
	cmp	r8, r2
	bcs.n	.L_0800afea
.L_0800afb8:
	ldrb	r3, [r4, #0]
	cmp	r3, #0
	beq.n	.L_0800afd0
	add	r3, sp, #4
	ldrb	r3, [r3, #0]
	strb	r3, [r1, #0]
	b.n	.L_0800afdc
	movs	r0, r0
	.4byte 0x03000164
	.2byte 0x05c0
	.2byte 0x0300
.L_0800afd0:
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_0800afdc
	add	r2, sp, #8
	ldrb	r2, [r2, #0]
	strb	r2, [r1, #0]
.L_0800afdc:
	movs	r3, #1
	ldr	r2, [sp, #36]
	add	r8, r3
	adds	r4, #1
	adds	r1, #1
	cmp	r8, r2
	bcc.n	.L_0800afb8
.L_0800afea:
	ldr	r0, [sp, #12]
	bl	sub_08002df0
.L_0800aff0:
	ldr	r3, [sp, #44]
	ldr	r1, [sp, #36]
	ldrb	r0, [r3, #28]
	movs	r2, #0
	bl	sub_08003fa4
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
	bl	sub_080072f4
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
	bl	sub_08002df0
	b.n	.L_0800b054
	.4byte 0x000003ff
	.4byte 0x06010000
	.4byte 0x03001e50
	.2byte 0xfc00
	.2byte 0xffff
.L_0800b054:
	ldr	r1, [sp, #24]
	cmp	r1, #0
	bne.n	.L_0800b060
	movs	r0, #52
	bl	sub_08002dd8
.L_0800b060:
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
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	ip, r0
	mov	fp, r3
	mov	r3, ip
	adds	r3, #32
	ldrb	r3, [r3, #0]
	lsrs	r0, r3, #1
	movs	r3, #33
	add	r3, ip
	mov	lr, r3
	ldrb	r3, [r3, #0]
	sub	sp, #8
	lsrs	r4, r3, #1
	movs	r3, #8
	str	r3, [sp, #4]
	movs	r3, #4
	str	r3, [sp, #0]
	mov	sl, r2
	ldr	r2, [sp, #44]
	movs	r3, #1
	mov	r9, r3
	ldmia	r2!, {r3}
	ldr	r5, [r2, #0]
	movs	r2, #128
	lsls	r2, r2, #9
	cmp	r3, r2
	bgt.n	.L_0800b0ba
	cmp	r5, r2
	ble.n	.L_0800b0ca
.L_0800b0ba:
	movs	r2, #3
	mov	r9, r2
	movs	r3, #16
	movs	r2, #8
	str	r3, [sp, #4]
	str	r2, [sp, #0]
	lsls	r0, r0, #1
	lsls	r4, r4, #1
.L_0800b0ca:
	asrs	r1, r1, #16
	subs	r7, r1, r0
	mov	r3, fp
	mov	r0, sl
	mov	r2, lr
	mov	r8, r1
	subs	r1, r3, r0
	ldrb	r3, [r2, #0]
	mov	r2, ip
	adds	r2, #35
	ldrb	r2, [r2, #0]
	lsls	r2, r2, #24
	asrs	r2, r2, #24
	lsrs	r3, r3, #1
	subs	r3, r3, r2
	muls	r3, r5
	asrs	r1, r1, #16
	subs	r1, r1, r4
	ldr	r4, [pc, #88]
	adds	r3, r3, r4
	asrs	r3, r3, #16
	subs	r6, r1, r3
	mov	r0, ip
	movs	r1, #4
	ldrb	r2, [r0, #5]
	negs	r1, r1
	adds	r3, r1, #0
	mov	r5, r9
	ands	r3, r2
	orrs	r3, r5
	ldr	r2, [pc, #68]
	strb	r3, [r0, #5]
	ldr	r4, [pc, #56]
	ldrh	r0, [r0, #6]
	adds	r3, r2, #0
	ands	r7, r4
	ands	r3, r0
	orrs	r3, r7
	mov	r0, ip
	strh	r3, [r0, #6]
	strb	r6, [r0, #4]
	ldr	r0, [sp, #4]
	mov	r3, r8
	subs	r7, r3, r0
	ldr	r3, [sp, #40]
	mov	r0, fp
	subs	r3, r0, r3
	ldr	r0, [sp, #0]
	asrs	r3, r3, #16
	subs	r6, r3, r0
	mov	r3, ip
	adds	r3, #12
	ldrb	r0, [r3, #5]
	ands	r1, r0
	orrs	r1, r5
	strb	r1, [r3, #5]
	ldrh	r1, [r3, #6]
	ands	r7, r4
	ands	r2, r1
	orrs	r2, r7
	b.n	.L_0800b150
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.2byte 0xfe00
	.2byte 0xffff
.L_0800b150:
	strh	r2, [r3, #6]
	strb	r6, [r3, #4]
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000

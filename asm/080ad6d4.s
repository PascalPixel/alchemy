@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080ad6d4
	.thumb_func
Func_080ad6d4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #112]
	ldr	r3, [r3, #0]
	sub	sp, #136
	mov	r9, r3
	movs	r1, #1
	movs	r2, #0
	movs	r3, #2
	mov	fp, r0
	movs	r0, #0
	str	r0, [sp, #100]
	str	r1, [sp, #96]
	str	r3, [sp, #88]
	str	r2, [sp, #92]
	mov	r4, r9
	ldr	r2, [r4, #20]
	movs	r3, #13
	mov	r0, sp
	strb	r3, [r2, #5]
	movs	r7, #0
	adds	r0, #128
	movs	r3, #165
	str	r7, [sp, #128]
	str	r0, [sp, #24]
	lsls	r3, r3, #1
	str	r7, [r0, #4]
	ldr	r1, [pc, #48]
	movs	r2, #3
	add	r3, r9
.L0:
	subs	r2, #1
	strh	r1, [r3, #0]
	subs	r3, #2
	cmp	r2, #0
	bge.n	.L0
	mov	r1, r9
	movs	r3, #29
	ldrsb	r3, [r1, r3]
	movs	r4, #154
	lsls	r4, r4, #1
	ldr	r2, [pc, #28]
	lsls	r3, r3, #1
	adds	r3, r3, r4
	mov	r0, r9
	strh	r2, [r0, r3]
	movs	r3, #29
	ldrsb	r3, [r0, r3]
	movs	r0, #162
	ldr	r1, [pc, #16]
	lsls	r0, r0, #1
	lsls	r3, r3, #1
	b.n	.L1
	movs	r0, r0
	.4byte 0x000000c8
	.4byte 0x00000078
	.4byte 0x00000020
	.4byte 0x03001f2c
.L1:
	adds	r3, r3, r0
	mov	r2, r9
	strh	r1, [r2, r3]
	movs	r3, #28
	ldrsb	r3, [r2, r3]
	ldr	r2, [pc, #60]
	lsls	r3, r3, #1
	adds	r3, r3, r4
	mov	r4, r9
	strh	r2, [r4, r3]
	movs	r3, #28
	ldrsb	r3, [r4, r3]
	lsls	r3, r3, #1
	adds	r3, r3, r0
	mov	r0, r9
	strh	r1, [r0, r3]
	movs	r5, #134
	ldr	r0, [r0, #48]
	lsls	r5, r5, #1
	bl	Func_08015278
	add	r5, r9
	movs	r0, #1
	bl	Func_080030f8
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_080a1114
	movs	r3, #5
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r1, #0
	b.n	.L2
	movs	r0, r0
	.4byte 0x00000010
.L2:
	movs	r2, #0
	adds	r0, r5, #0
	movs	r3, #30
	bl	Func_080a10d0
	mov	r1, sp
	movs	r7, #1
	adds	r1, #120
	mov	r2, fp
	str	r7, [sp, #120]
	str	r1, [sp, #28]
	str	r7, [r1, #4]
	cmp	r2, #1
	bls.n	.L3
	b.n	.L4
.L3:
	movs	r0, #96
	bl	Func_08004970
	movs	r5, #166
	lsls	r5, r5, #1
	str	r0, [sp, #84]
	adds	r0, r5, #0
	bl	Func_08004970
	movs	r3, #0
	str	r3, [sp, #76]
	str	r3, [sp, #72]
	ldr	r3, [pc, #932]
	add	r3, r9
	ldrb	r6, [r3, #0]
	ldr	r3, [pc, #928]
	add	r3, r9
	ldrb	r3, [r3, #0]
	str	r3, [sp, #80]
	movs	r3, #149
	lsls	r3, r3, #2
	movs	r4, #31
	add	r3, r9
	mov	r8, r4
	ldrb	r3, [r3, #0]
	mov	sl, r0
	mov	r0, r8
	ands	r0, r3
	movs	r3, #188
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #8
	ands	r3, r2
	lsls	r3, r3, #16
	lsrs	r3, r3, #16
	mov	r8, r0
	adds	r0, r6, #0
	str	r3, [sp, #68]
	bl	Func_08077008
	adds	r7, r0, #0
	ldr	r3, [pc, #876]
	adds	r1, r7, #0
	adds	r2, r5, #0
	mov	r0, sl
	bl	Func_080072f0
	adds	r0, r6, #0
	ldr	r1, [sp, #80]
	mov	r2, r8
	bl	Func_080771b8
	mov	r1, fp
	cmp	r1, #0
	bne.n	.L5
	ldr	r3, [pc, #852]
	add	r3, r9
	ldrb	r3, [r3, #0]
	str	r3, [sp, #76]
	ldr	r3, [pc, #848]
	add	r3, r9
	ldrb	r3, [r3, #0]
	movs	r2, #31
	ands	r2, r3
	movs	r3, #189
	str	r2, [sp, #72]
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	movs	r5, #128
	lsls	r5, r5, #8
	ands	r5, r3
	lsls	r5, r5, #16
	lsrs	r5, r5, #16
	adds	r0, r6, #0
	ldr	r1, [sp, #76]
	bl	Func_080771a8
	cmp	r5, #0
	beq.n	.L5
	adds	r0, r6, #0
	ldr	r1, [sp, #76]
	ldr	r2, [sp, #72]
	bl	Func_080771b0
.L5:
	adds	r0, r6, #0
	bl	Func_08077010
	mov	r3, sl
	mov	r4, sp
	mov	r0, sp
	adds	r4, #116
	adds	r3, #88
	adds	r0, #112
	adds	r1, r7, #0
	str	r3, [sp, #64]
	str	r0, [sp, #56]
	str	r0, [sp, #0]
	ldr	r2, [sp, #84]
	adds	r1, #88
	adds	r0, r3, #0
	adds	r3, r4, #0
	str	r4, [sp, #60]
	bl	Func_080aae14
	movs	r2, #166
	str	r0, [sp, #120]
	mov	r1, sl
	lsls	r2, r2, #1
	ldr	r5, [pc, #740]
	adds	r0, r7, #0
	bl	Func_080072f8
	ldr	r3, [pc, #744]
	add	r3, r9
	ldrb	r6, [r3, #0]
	adds	r0, r6, #0
	bl	Func_08077008
	movs	r2, #166
	adds	r7, r0, #0
	adds	r1, r7, #0
	mov	r0, sl
	lsls	r2, r2, #1
	bl	Func_080072f8
	mov	r1, fp
	cmp	r1, #0
	bne.n	.L6
	adds	r0, r6, #0
	ldr	r1, [sp, #76]
	ldr	r2, [sp, #72]
	bl	Func_080771b8
.L6:
	mov	r2, r8
	adds	r0, r6, #0
	ldr	r1, [sp, #80]
	bl	Func_080771a8
	ldr	r2, [sp, #68]
	cmp	r2, #0
	beq.n	.L7
	adds	r0, r6, #0
	ldr	r1, [sp, #80]
	mov	r2, r8
	bl	Func_080771b0
.L7:
	adds	r0, r6, #0
	bl	Func_08077010
	ldr	r3, [sp, #56]
	adds	r1, r7, #0
	str	r3, [sp, #0]
	ldr	r2, [sp, #84]
	ldr	r0, [sp, #64]
	ldr	r3, [sp, #60]
	adds	r1, #88
	bl	Func_080aae14
	ldr	r4, [sp, #28]
	movs	r2, #166
	str	r0, [r4, #4]
	mov	r1, sl
	lsls	r2, r2, #1
	adds	r0, r7, #0
	bl	Func_080072f8
	mov	r0, sl
	bl	Func_08002df0
	ldr	r0, [sp, #84]
	bl	Func_08002df0
	b.n	.L8
.L4:
	mov	r3, fp
	subs	r3, #2
	cmp	r3, #1
	bhi.n	.L8
	movs	r0, #96
	bl	Func_08004970
	str	r0, [sp, #52]
	movs	r0, #166
	lsls	r0, r0, #1
	bl	Func_08004970
	ldr	r3, [pc, #580]
	add	r3, r9
	ldrb	r6, [r3, #0]
	ldr	r3, [pc, #576]
	add	r3, r9
	ldrb	r3, [r3, #0]
	str	r3, [sp, #48]
	movs	r3, #149
	lsls	r3, r3, #2
	mov	sl, r0
	add	r3, r9
	movs	r0, #31
	ldrb	r3, [r3, #0]
	mov	r8, r0
	mov	r1, r8
	ands	r1, r3
	mov	r2, fp
	movs	r3, #3
	eors	r3, r2
	negs	r5, r3
	adds	r0, r6, #0
	mov	r8, r1
	orrs	r5, r3
	bl	Func_08077008
	lsrs	r5, r5, #31
	subs	r5, r7, r5
	movs	r2, #166
	adds	r7, r0, #0
	adds	r1, r7, #0
	lsls	r2, r2, #1
	mov	r0, sl
	ldr	r3, [pc, #524]
	bl	Func_080072f0
	adds	r0, r6, #0
	ldr	r1, [sp, #48]
	mov	r2, r8
	bl	Func_080771b8
	cmp	r5, #0
	beq.n	.L9
	adds	r0, r6, #0
	ldr	r1, [sp, #48]
	mov	r2, r8
	bl	Func_080771b0
.L9:
	adds	r0, r6, #0
	bl	Func_08077010
	mov	r0, sl
	add	r2, sp, #104
	adds	r1, r7, #0
	add	r3, sp, #108
	str	r2, [sp, #0]
	adds	r1, #88
	ldr	r2, [sp, #52]
	adds	r0, #88
	bl	Func_080aae14
	movs	r2, #166
	str	r0, [sp, #120]
	mov	r1, sl
	lsls	r2, r2, #1
	ldr	r4, [pc, #460]
	adds	r0, r7, #0
	bl	Func_080072f4
	ldr	r0, [sp, #28]
	ldr	r3, [sp, #120]
	movs	r1, #0
	str	r3, [r0, #4]
	mov	r0, sl
	str	r1, [sp, #88]
	bl	Func_08002df0
	ldr	r0, [sp, #52]
	bl	Func_08002df0
.L8:
	ldr	r0, [sp, #120]
	movs	r1, #5
	subs	r0, #1
	bl	Func_080022ec
	adds	r0, #1
	str	r0, [sp, #120]
	cmp	r0, #0
	bne.n	.L10
	movs	r3, #1
	str	r3, [sp, #120]
.L10:
	ldr	r2, [sp, #28]
	ldr	r0, [r2, #4]
	movs	r1, #5
	subs	r0, #1
	bl	Func_080022ec
	ldr	r3, [sp, #28]
	adds	r0, #1
	str	r0, [r3, #4]
	cmp	r0, #0
	bne.n	.L11
	ldr	r4, [sp, #28]
	movs	r3, #1
	str	r3, [r4, #4]
.L11:
	mov	r0, r9
	movs	r5, #2
	adds	r0, #36
	movs	r6, #15
	movs	r1, #0
	movs	r2, #5
	movs	r3, #15
	str	r5, [sp, #4]
	str	r0, [sp, #44]
	str	r6, [sp, #0]
	bl	Func_080a10d0
	mov	r1, r9
	str	r5, [sp, #4]
	movs	r5, #134
	adds	r1, #52
	lsls	r5, r5, #1
	str	r1, [sp, #40]
	movs	r2, #5
	adds	r0, r1, #0
	movs	r3, #15
	movs	r1, #15
	add	r5, r9
	str	r6, [sp, #0]
	bl	Func_080a10d0
	ldr	r0, [r5, #0]
	bl	Func_08015270
	mov	r2, fp
	cmp	r2, #2
	bne.n	.L12
	ldr	r0, [pc, #332]
	b.n	.L13
.L12:
	mov	r3, fp
	cmp	r3, #3
	bne.n	.L14
	ldr	r0, [pc, #324]
.L13:
	ldr	r1, [r5, #0]
	movs	r2, #96
	movs	r3, #0
	bl	Func_08015080
	ldr	r0, [pc, #316]
	ldr	r1, [r5, #0]
	movs	r2, #96
	movs	r3, #16
	bl	Func_08015080
	b.n	.L15
.L14:
	mov	r4, fp
	cmp	r4, #0
	bne.n	.L16
	ldr	r0, [pc, #300]
	ldr	r1, [r5, #0]
	movs	r2, #128
	movs	r3, #0
	bl	Func_08015080
	ldr	r0, [pc, #284]
	ldr	r1, [r5, #0]
	movs	r2, #128
	movs	r3, #8
	bl	Func_08015080
	b.n	.L15
.L16:
	ldr	r0, [pc, #280]
	ldr	r1, [r5, #0]
	movs	r2, #128
	movs	r3, #0
	bl	Func_08015080
	ldr	r0, [pc, #260]
	ldr	r1, [r5, #0]
	movs	r2, #128
	movs	r3, #8
	bl	Func_08015080
.L15:
	movs	r1, #1
	mov	r2, fp
	movs	r7, #188
	eors	r2, r1
	lsls	r7, r7, #1
	movs	r0, #128
	negs	r3, r2
	add	r7, r9
	lsls	r0, r0, #8
	orrs	r3, r2
	mov	r8, r0
	ldrh	r2, [r7, #0]
	lsrs	r5, r3, #31
	mov	r3, r8
	ands	r3, r2
	subs	r5, r1, r5
	cmp	r3, #0
	bne.n	.L17
	movs	r0, #2
	bl	Func_080150b8
	ldrh	r2, [r7, #0]
.L17:
	movs	r3, #224
	ands	r3, r2
	lsrs	r3, r3, #5
	lsls	r0, r3, #2
	adds	r0, r0, r3
	movs	r6, #134
	movs	r3, #31
	ands	r3, r2
	ldr	r1, [pc, #200]
	lsls	r0, r0, #2
	lsls	r6, r6, #1
	adds	r0, r0, r3
	add	r6, r9
	adds	r0, r0, r1
	lsls	r3, r5, #3
	ldr	r1, [r6, #0]
	movs	r2, #48
	bl	Func_08015080
	ldrh	r3, [r7, #0]
	movs	r1, #224
	ands	r1, r3
	ldr	r2, [pc, #176]
	movs	r3, #0
	lsrs	r1, r1, #5
	ldr	r0, [r6, #0]
	adds	r1, r1, r2
	str	r3, [sp, #0]
	movs	r2, #5
	mov	sl, r3
	adds	r3, r5, #0
	bl	Func_08015280
	movs	r0, #15
	bl	Func_080150b8
	mov	r4, fp
	cmp	r4, #0
	bne.n	.L18
	movs	r5, #189
	lsls	r5, r5, #1
	add	r5, r9
	ldrh	r2, [r5, #0]
	mov	r3, r8
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L19
	movs	r0, #2
	bl	Func_080150b8
	ldrh	r2, [r5, #0]
.L19:
	movs	r3, #224
	ands	r3, r2
	lsrs	r3, r3, #5
	lsls	r0, r3, #2
	adds	r0, r0, r3
	movs	r3, #31
	ands	r3, r2
	ldr	r1, [pc, #100]
	lsls	r0, r0, #2
	adds	r0, r0, r3
	adds	r0, r0, r1
	movs	r2, #48
	ldr	r1, [r6, #0]
	movs	r3, #16
	bl	Func_08015080
	ldrh	r3, [r5, #0]
	movs	r1, #224
	ands	r1, r3
	ldr	r2, [pc, #80]
	mov	r3, fp
	lsrs	r1, r1, #5
	ldr	r0, [r6, #0]
	adds	r1, r1, r2
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #2
	bl	Func_08015280
	movs	r0, #15
	bl	Func_080150b8
	b.n	.L20
.L64:
	movs	r7, #1
	b.n	.L21
	movs	r0, r0
	.4byte 0x0000021a
	.4byte 0x00000256
	.4byte 0x03001388
	.4byte 0x00000257
	.4byte 0x00000255
	.4byte 0x0000021b
	.4byte 0x00000ba5
	.4byte 0x00000ba4
	.4byte 0x00000bc1
	.4byte 0x00000ba7
	.4byte 0x00000ba6
	.4byte 0x0000045f
	.4byte 0x00005001
.L66:
	movs	r0, #113
	movs	r7, #2
	b.n	.L22
.L18:
	mov	r3, fp
	subs	r3, #2
	cmp	r3, #1
	bhi.n	.L20
	ldrh	r2, [r7, #0]
	mov	r3, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L23
	movs	r0, #2
	bl	Func_080150b8
	ldrh	r2, [r7, #0]
.L23:
	movs	r3, #224
	ands	r3, r2
	lsrs	r3, r3, #5
	lsls	r0, r3, #2
	adds	r0, r0, r3
	movs	r3, #31
	ldr	r4, [pc, #824]
	ands	r3, r2
	lsls	r0, r0, #2
	adds	r0, r0, r3
	adds	r0, r0, r4
	ldr	r1, [r6, #0]
	movs	r2, #48
	movs	r3, #16
	bl	Func_08015080
	ldrh	r3, [r7, #0]
	movs	r1, #224
	ands	r1, r3
	ldr	r2, [pc, #804]
	mov	r3, sl
	lsrs	r1, r1, #5
	adds	r1, r1, r2
	ldr	r0, [r6, #0]
	movs	r2, #5
	str	r3, [sp, #0]
	movs	r3, #2
	bl	Func_08015280
	movs	r0, #15
	bl	Func_080150b8
	ldr	r0, [r6, #0]
	mov	r4, sl
	ldr	r1, [pc, #776]
	movs	r2, #7
	movs	r3, #1
	str	r4, [sp, #0]
	bl	Func_08015280
.L20:
	ldr	r3, [pc, #768]
	ldr	r3, [r3, #0]
	mov	sl, r3
	movs	r3, #194
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r6, [r3, #0]
	ldr	r3, [pc, #760]
	ldr	r3, [r3, #0]
	str	r3, [sp, #36]
	ldr	r3, [pc, #756]
	ldr	r3, [r3, #0]
	ldr	r0, [pc, #756]
	str	r3, [sp, #32]
	adds	r5, r6, r0
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L24
	b.n	.L25
.L24:
	movs	r0, #128
	lsls	r0, r0, #2
	bl	Func_08004970
	movs	r1, #0
	ldr	r3, [pc, #736]
	str	r1, [sp, #36]
	str	r1, [sp, #32]
	adds	r2, r6, r3
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	ldr	r3, [r5, #0]
	subs	r3, #5
	mov	r8, r0
	cmp	r3, #18
	bls.n	.L26
	b.n	.L27
.L26:
	ldr	r2, [pc, #716]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080adcc4
	.4byte 0x080ade06
	.4byte 0x080ade06
	.4byte 0x080ade06
	.4byte 0x080ade06
	.4byte 0x080adce6
	.4byte 0x080add72
	.4byte 0x080adcc4
	.4byte 0x080ade06
	.4byte 0x080ade06
	.4byte 0x080ade06
	.4byte 0x080ade06
	.4byte 0x080ade06
	.4byte 0x080ade06
	.4byte 0x080ade06
	.4byte 0x080ade06
	.4byte 0x080ade06
	.4byte 0x080ade06
	.4byte 0x080adcc4
	ldr	r4, [pc, #624]
	adds	r2, r6, r4
	ldr	r3, [r2, #0]
	cmp	r3, #100
	beq.n	.L28
	b.n	.L27
.L28:
	movs	r0, #1
	ldr	r1, [pc, #608]
	str	r0, [sp, #32]
	str	r0, [sp, #36]
	movs	r3, #0
	str	r3, [r2, #0]
	adds	r2, r6, r1
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	b.n	.L27
	ldr	r2, [pc, #592]
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	cmp	r3, #60
	beq.n	.L29
	b.n	.L27
.L29:
	movs	r2, #128
	mov	r1, sl
	ldr	r3, [pc, #584]
	mov	r0, r8
	lsls	r2, r2, #2
	bl	Func_080072f0
	movs	r1, #8
	movs	r2, #0
	movs	r3, #1
	ldr	r0, [pc, #572]
	bl	Func_08015038
	adds	r7, r0, #0
	mov	r4, r9
	ldr	r3, [r4, #20]
	movs	r5, #1
	strb	r5, [r3, #5]
	movs	r0, #2
	movs	r1, #96
	bl	Func_080a1ac0
	ldr	r3, [pc, #552]
	movs	r0, #131
	lsls	r0, r0, #2
	adds	r3, r3, r0
	strb	r5, [r3, #0]
	b.n	.L30
.L31:
	movs	r0, #1
	bl	Func_080030f8
.L30:
	bl	Func_08015048
	cmp	r0, #0
	beq.n	.L31
	adds	r0, r7, #0
	movs	r1, #1
	bl	Func_08015018
	movs	r2, #128
	mov	r1, r8
	ldr	r3, [pc, #504]
	lsls	r2, r2, #2
	mov	r0, sl
	bl	Func_080072f0
	bl	Func_080152a8
	ldr	r2, [pc, #504]
	movs	r3, #1
	add	r2, sl
	strb	r3, [r2, #0]
	ldr	r2, [pc, #476]
	str	r3, [sp, #96]
	movs	r1, #0
	adds	r3, r6, r2
	str	r1, [r3, #0]
	ldr	r3, [pc, #460]
	adds	r2, r6, r3
	movs	r3, #11
	str	r3, [r2, #0]
	mov	r4, r9
	ldr	r2, [r4, #20]
	b.n	.L32
	ldr	r0, [pc, #452]
	adds	r3, r6, r0
	ldr	r3, [r3, #0]
	cmp	r3, #60
	bne.n	.L27
	movs	r2, #128
	mov	r1, sl
	ldr	r3, [pc, #444]
	mov	r0, r8
	lsls	r2, r2, #2
	bl	Func_080072f0
	movs	r1, #8
	movs	r2, #0
	movs	r3, #1
	ldr	r0, [pc, #444]
	bl	Func_08015038
	adds	r7, r0, #0
	mov	r1, r9
	ldr	r3, [r1, #20]
	movs	r5, #1
	strb	r5, [r3, #5]
	movs	r0, #106
	movs	r1, #56
	bl	Func_080a1ac0
	ldr	r3, [pc, #412]
	movs	r2, #131
	lsls	r2, r2, #2
	adds	r3, r3, r2
	strb	r5, [r3, #0]
	b.n	.L33
.L34:
	movs	r0, #1
	bl	Func_080030f8
.L33:
	bl	Func_08015048
	cmp	r0, #0
	beq.n	.L34
	adds	r0, r7, #0
	movs	r1, #1
	bl	Func_08015018
	movs	r2, #128
	mov	r1, r8
	ldr	r3, [pc, #368]
	lsls	r2, r2, #2
	mov	r0, sl
	bl	Func_080072f0
	bl	Func_080152a8
	ldr	r2, [pc, #364]
	movs	r3, #1
	add	r2, sl
	strb	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r4, [pc, #332]
	movs	r3, #1
	ldr	r0, [pc, #324]
	str	r3, [sp, #96]
	movs	r5, #0
	adds	r3, r6, r4
	str	r5, [r3, #0]
	adds	r2, r6, r0
	movs	r3, #12
	str	r3, [r2, #0]
	mov	r1, r9
	ldr	r2, [r1, #20]
.L32:
	movs	r3, #13
	strb	r3, [r2, #5]
.L27:
	mov	r0, r8
	bl	Func_08002df0
.L25:
	ldr	r2, [sp, #96]
	cmp	r2, #0
	bne.n	.L35
	b.n	.L36
.L35:
	movs	r3, #1
	mov	r4, fp
	mov	r8, r3
	cmp	r4, #1
	bhi.n	.L37
	movs	r5, #134
	lsls	r5, r5, #1
	movs	r3, #24
	add	r5, r9
	ldr	r0, [r5, #0]
	movs	r1, #128
	str	r3, [sp, #0]
	movs	r2, #16
	movs	r3, #224
	bl	Func_08015068
	ldr	r0, [sp, #88]
	cmp	r0, #1
	bne.n	.L38
	ldr	r0, [pc, #280]
	ldr	r1, [r5, #0]
	movs	r2, #128
	movs	r3, #16
	bl	Func_08015080
	b.n	.L37
.L38:
	ldr	r1, [sp, #88]
	movs	r2, #2
	eors	r2, r1
	negs	r3, r2
	orrs	r3, r2
	movs	r2, #134
	lsls	r2, r2, #2
	lsrs	r3, r3, #31
	adds	r3, r3, r2
	mov	r2, r9
	adds	r2, #2
	ldrb	r0, [r2, r3]
	movs	r1, #1
	bl	Func_08015120
	movs	r3, #134
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r1, [r3, #0]
	ldr	r0, [pc, #232]
	movs	r2, #128
	movs	r3, #16
	bl	Func_08015080
.L37:
	ldr	r2, [pc, #224]
	movs	r3, #1
	add	r2, sl
	strb	r3, [r2, #0]
	mov	r2, r9
	ldr	r0, [r2, #36]
	bl	Func_08015060
	mov	r3, r9
	ldr	r0, [r3, #52]
	bl	Func_08015060
	ldr	r4, [sp, #96]
	lsrs	r3, r4, #1
	cmp	r3, #0
	beq.n	.L39
	movs	r0, #1
	bl	Func_080030f8
.L39:
	mov	r0, fp
	cmp	r0, #3
	bne.n	.L40
	movs	r1, #0
	movs	r7, #0
	mov	r8, r1
	b.n	.L41
.L40:
	mov	r2, fp
	cmp	r2, #2
	bne.n	.L42
	movs	r3, #0
	movs	r7, #1
	mov	r8, r3
	b.n	.L41
.L42:
	movs	r7, #2
.L41:
	ldr	r4, [sp, #88]
	cmp	r4, #0
	beq.n	.L43
	cmp	r4, #1
	beq.n	.L44
	mov	r0, fp
	movs	r1, #0
	bl	Func_080aca04
	b.n	.L45
.L44:
	mov	r0, fp
	cmp	r0, #1
	bne.n	.L46
	movs	r7, #4
.L46:
	ldr	r6, [pc, #132]
	ldr	r2, [sp, #88]
	mov	r1, r9
	add	r6, r9
	ldr	r0, [r1, #36]
	ldrb	r3, [r6, #0]
	movs	r5, #0
	str	r2, [sp, #0]
	movs	r1, #0
	movs	r2, #0
	str	r5, [sp, #4]
	str	r7, [sp, #8]
	str	r5, [sp, #12]
	str	r5, [sp, #16]
	bl	Func_080acab8
	ldr	r4, [sp, #88]
	mov	r3, r9
	ldr	r0, [r3, #52]
	ldr	r1, [sp, #24]
	ldrb	r3, [r6, #0]
	str	r4, [sp, #0]
	str	r5, [sp, #4]
	str	r7, [sp, #8]
	ldr	r2, [r1, #4]
	adds	r2, #1
	str	r2, [sp, #12]
	movs	r1, #0
	movs	r2, #0
	str	r5, [sp, #16]
	bl	Func_080acab8
	b.n	.L45
	movs	r0, r0
	.4byte 0x0000045f
	.4byte 0x00005001
	.4byte 0x0000f296
	.4byte 0x03001e8c
	.4byte 0x03001c94
	.4byte 0x03001b04
	.4byte 0x0000212c
	.4byte 0x00002128
	.4byte 0x080adc78
	.4byte 0x03001388
	.4byte 0x00000c43
	.4byte 0x02000240
	.4byte 0x00000ea3
	.4byte 0x00000c42
	.4byte 0x00000ba1
	.4byte 0x00000ba0
	.4byte 0x00000ea6
	.4byte 0x0000021b
.L43:
	mov	r2, fp
	cmp	r2, #1
	bne.n	.L47
	movs	r3, #0
	movs	r7, #1
	mov	r8, r3
.L47:
	movs	r5, #150
	ldr	r1, [sp, #88]
	lsls	r5, r5, #2
	mov	r4, r9
	add	r5, r9
	mov	r2, r8
	ldr	r0, [r4, #36]
	ldrb	r3, [r5, #0]
	movs	r6, #1
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	str	r1, [sp, #12]
	movs	r2, #0
	movs	r1, #0
	str	r7, [sp, #8]
	str	r6, [sp, #16]
	bl	Func_080acab8
	ldr	r2, [sp, #128]
	mov	r3, r9
	mov	r1, r8
	adds	r2, #1
	ldr	r4, [sp, #88]
	ldr	r0, [r3, #52]
	ldrb	r3, [r5, #0]
	str	r1, [sp, #4]
	str	r2, [sp, #12]
	movs	r1, #0
	movs	r2, #0
	str	r4, [sp, #0]
	str	r7, [sp, #8]
	str	r6, [sp, #16]
	bl	Func_080acab8
.L45:
	ldr	r2, [pc, #764]
	movs	r3, #0
	add	r2, sl
	strb	r3, [r2, #0]
.L36:
	ldr	r2, [sp, #88]
	cmp	r2, #1
	bgt.n	.L48
	ldr	r0, [sp, #28]
	mov	r3, r9
	lsls	r4, r2, #2
	ldr	r7, [r3, #52]
	adds	r3, r4, r0
	ldr	r2, [r3, #0]
	cmp	r2, #1
	ble.n	.L48
	movs	r5, #0
	cmp	r5, r2
	bge.n	.L49
	adds	r6, r3, #0
.L52:
	ldr	r2, [pc, #732]
	adds	r1, r5, r2
	cmp	r5, #9
	ble.n	.L50
	ldr	r1, [pc, #728]
.L50:
	ldr	r0, [sp, #24]
	ldr	r3, [r4, r0]
	cmp	r5, r3
	bne.n	.L51
	ldr	r2, [pc, #720]
	adds	r1, r1, r2
.L51:
	ldr	r3, [r6, #0]
	ldrh	r2, [r7, #8]
	subs	r2, r2, r3
	adds	r2, r2, r5
	movs	r3, #0
	str	r3, [sp, #0]
	subs	r2, #2
	subs	r3, #1
	adds	r0, r7, #0
	str	r4, [sp, #20]
	bl	Func_08015280
	ldr	r3, [r6, #0]
	adds	r5, #1
	ldr	r4, [sp, #20]
	cmp	r5, r3
	blt.n	.L52
.L49:
	ldr	r0, [sp, #28]
	ldrh	r2, [r7, #8]
	ldr	r3, [r4, r0]
	movs	r6, #1
	negs	r6, r6
	subs	r2, r2, r3
	movs	r5, #0
	adds	r0, r7, #0
	adds	r3, r6, #0
	ldr	r1, [pc, #668]
	subs	r2, #3
	str	r5, [sp, #0]
	bl	Func_08015280
	ldrh	r2, [r7, #8]
	ldr	r1, [pc, #660]
	subs	r2, #2
	adds	r0, r7, #0
	adds	r3, r6, #0
	str	r5, [sp, #0]
	bl	Func_08015280
	ldr	r1, [pc, #652]
	ldrh	r2, [r7, #14]
	add	r1, sl
	lsrs	r2, r2, #2
	movs	r3, #2
	lsls	r3, r2
	ldrb	r2, [r1, #0]
	orrs	r3, r2
	strb	r3, [r1, #0]
.L48:
	ldr	r1, [sp, #92]
	adds	r1, #1
	str	r1, [sp, #92]
	adds	r0, r1, #0
	movs	r1, #60
	bl	Func_080022fc
	subs	r6, r0, #5
	cmp	r6, #0
	bge.n	.L53
	movs	r6, #0
.L53:
	cmp	r6, #29
	ble.n	.L54
	movs	r6, #29
.L54:
	ldr	r5, [pc, #608]
	movs	r0, #0
	adds	r1, r5, #0
	bl	Func_080ad5f4
	movs	r0, #1
	adds	r1, r5, #0
	bl	Func_080ad5f4
	mov	r2, fp
	cmp	r2, #1
	bhi.n	.L55
	movs	r1, #30
	adds	r0, r6, #0
	bl	Func_080022fc
	adds	r3, r0, #0
	lsls	r0, r3, #4
	adds	r0, r0, r3
	lsls	r0, r0, #4
	adds	r0, r0, r3
	lsls	r0, r0, #2
	bl	Func_08002322
	ldr	r3, [pc, #564]
	adds	r1, r0, #0
	movs	r0, #16
	movs	r0, r0
	mov	ip, pc
	bx	r3
	movs	r3, #6
	negs	r5, r0
	negs	r3, r3
	cmp	r5, r3
	bge.n	.L56
	adds	r5, r3, #0
.L56:
	cmp	r5, #12
	ble.n	.L57
	movs	r5, #12
.L57:
	adds	r0, r6, #0
	movs	r1, #35
	bl	Func_080022fc
	lsls	r6, r0, #1
	adds	r1, r6, #0
	adds	r2, r5, #0
	movs	r3, #0
	adds	r1, #34
	adds	r2, #20
	movs	r0, #0
	bl	Func_080ad5b4
	mov	r3, fp
	cmp	r3, #0
	bne.n	.L58
	movs	r1, #99
	movs	r2, #36
	subs	r1, r1, r6
	subs	r2, r2, r5
	movs	r0, #1
	movs	r3, #0
	bl	Func_080ad5b4
	b.n	.L58
.L55:
	movs	r0, #0
	movs	r1, #32
	movs	r2, #30
	movs	r3, #0
	bl	Func_080ad5b4
.L58:
	ldr	r4, [sp, #96]
	cmp	r4, #0
	beq.n	.L59
	movs	r0, #0
	str	r0, [sp, #96]
	movs	r1, #2
	ldr	r0, [sp, #100]
	bl	Func_080aa538
	str	r0, [sp, #100]
.L59:
	ldr	r1, [sp, #100]
	lsls	r0, r1, #3
	subs	r0, r0, r1
	lsls	r0, r0, #3
	adds	r0, #80
	movs	r1, #16
	bl	Func_080a1a40
	ldr	r2, [sp, #92]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L60
	movs	r3, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L61
	ldr	r1, [pc, #424]
	ldr	r3, [pc, #424]
	ldr	r0, [pc, #428]
	movs	r2, #32
	bl	Func_080072f0
	b.n	.L60
.L61:
	ldr	r3, [pc, #420]
	ldr	r0, [pc, #416]
	movs	r1, #32
	ldr	r2, [pc, #420]
	bl	Func_080072f0
.L60:
	ldr	r4, [sp, #36]
	movs	r3, #1
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L62
	ldr	r0, [sp, #100]
	cmp	r0, #0
	bne.n	.L63
	b.n	.L64
.L63:
	movs	r0, #113
	movs	r7, #1
.L22:
	bl	Func_080f9010
	negs	r7, r7
	b.n	.L21
.L62:
	ldr	r1, [sp, #36]
	movs	r3, #8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L65
	b.n	.L66
.L65:
	movs	r3, #2
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L63
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L67
	mov	r2, fp
	cmp	r2, #1
	bhi.n	.L68
	ldr	r0, [sp, #88]
	movs	r1, #3
	adds	r0, #1
	bl	Func_080022fc
	movs	r3, #2
	str	r0, [sp, #88]
	movs	r0, #111
	str	r3, [sp, #96]
	bl	Func_080f9010
	b.n	.L68
.L67:
	ldr	r4, [sp, #32]
	movs	r3, #32
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L69
	ldr	r0, [sp, #88]
	cmp	r0, #1
	bgt.n	.L68
	ldr	r1, [sp, #24]
	lsls	r3, r0, #2
	adds	r5, r3, r1
	ldr	r0, [r5, #0]
	subs	r0, #1
	str	r0, [r5, #0]
	ldr	r2, [sp, #28]
	ldr	r1, [r3, r2]
	bl	Func_080aa538
	str	r0, [r5, #0]
	movs	r0, #111
	bl	Func_080f9010
	bl	Func_0800352c
	movs	r3, #1
	str	r3, [sp, #96]
	b.n	.L68
.L69:
	ldr	r4, [sp, #32]
	movs	r3, #16
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L68
	ldr	r0, [sp, #88]
	cmp	r0, #1
	bgt.n	.L68
	movs	r0, #111
	bl	Func_080f9010
	bl	Func_0800352c
	ldr	r1, [sp, #88]
	ldr	r2, [sp, #24]
	lsls	r3, r1, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	adds	r0, #1
	str	r0, [r5, #0]
	movs	r4, #1
	str	r4, [sp, #96]
	ldr	r2, [sp, #28]
	ldr	r1, [r3, r2]
	bl	Func_080aa538
	str	r0, [r5, #0]
.L68:
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L20
.L21:
	movs	r1, #0
	movs	r0, #0
	bl	Func_080ad5f4
	movs	r1, #0
	movs	r0, #1
	bl	Func_080ad5f4
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #192]
	bl	Func_080041d8
	ldr	r3, [pc, #188]
	movs	r5, #134
	ldr	r2, [r3, #0]
	ldr	r6, [pc, #124]
	lsls	r5, r5, #1
	mov	r8, r3
	add	r5, r9
	movs	r3, #1
	movs	r4, #0
	strb	r3, [r2, r6]
	adds	r0, r5, #0
	movs	r1, #1
	mov	sl, r4
	bl	Func_080a1114
	movs	r0, #1
	bl	Func_080030f8
	movs	r3, #5
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	movs	r2, #0
	movs	r3, #17
	adds	r0, r5, #0
	movs	r1, #13
	bl	Func_080a10d0
	movs	r1, #1
	ldr	r0, [sp, #44]
	bl	Func_080a1114
	movs	r1, #1
	ldr	r0, [sp, #40]
	bl	Func_080a1114
	mov	r1, r9
	ldr	r0, [r1, #48]
	bl	Func_08015270
	mov	r2, r9
	ldr	r0, [r2, #40]
	bl	Func_08015270
	mov	r3, r9
	ldr	r0, [r3, #16]
	bl	Func_08015270
	mov	r4, r8
	ldr	r3, [r4, #0]
	mov	r0, sl
	adds	r3, r3, r6
	strb	r0, [r3, #0]
	movs	r0, #1
	bl	Func_080030f8
	adds	r0, r7, #0
	add	sp, #136
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x00000ea6
	.4byte 0x0000f031
	.4byte 0x0000f030
	.4byte 0xfffff000
	.4byte 0x0000f128
	.4byte 0x0000f129
	.4byte 0x00000ea3
	.4byte 0xffff4000
	.4byte 0x03000118
	.4byte 0x080af26c
	.4byte 0x03001388
	.4byte 0x060052c0
	.4byte 0x03000168
	.4byte 0x44444444
	.4byte 0x080a19a1
	.4byte 0x03001e8c

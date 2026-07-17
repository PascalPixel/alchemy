@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08012518
	.thumb_func
Func_08012518:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #64
	movs	r0, #144
	movs	r1, #96
	movs	r2, #1
	str	r0, [sp, #24]
	str	r1, [sp, #20]
	movs	r3, #0
	movs	r1, #160
	movs	r0, #9
	mov	sl, r3
	str	r2, [sp, #16]
	str	r2, [sp, #12]
	bl	Func_080048f4
	ldr	r2, [pc, #116]
	str	r0, [sp, #8]
	add	r4, sp, #28
	mov	r0, sl
	movs	r3, #3
	strb	r3, [r2, #0]
	str	r0, [r4, #0]
	ldr	r3, [pc, #104]
	adds	r0, r4, #0
	ldr	r1, [sp, #8]
	ldr	r2, [pc, #104]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r1, sl
	str	r1, [r4, #0]
	adds	r0, r4, #0
	add	r1, sp, #32
	ldr	r2, [pc, #92]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #1
	movs	r1, #1
	negs	r0, r0
	bl	Func_08012af8
	add	r2, sp, #32
	mov	r8, r2
	ldr	r1, [pc, #48]
	movs	r7, #0
	movs	r2, #1
	mov	r3, r8
.L0:
	adds	r7, #1
	strh	r2, [r3, #2]
	strb	r1, [r3, #5]
	strh	r0, [r3, #0]
	adds	r3, #8
	cmp	r7, #3
	bls.n	.L0
	movs	r3, #1
	mov	r0, r8
	ldr	r2, [pc, #52]
	strb	r3, [r0, #4]
	movs	r3, #2
	strb	r3, [r2, #0]
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #16]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #16]
	movs	r2, #160
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	b.n	.L1
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x00003f42
	.4byte 0x000001e0
	.4byte 0x03001c90
	.4byte 0x040000d4
	.4byte 0x85000001
	.4byte 0x85000008
	.4byte 0x03001f54
	.4byte 0x04000050
.L1:
	movs	r2, #128
	ldr	r3, [pc, #60]
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	Func_08003bb4
.L44:
	bl	Func_08004858
	bl	Func_080040e8
	movs	r1, #160
	movs	r0, #9
	bl	Func_080048f4
	str	r0, [sp, #8]
	bl	Func_0800403c
	movs	r0, #2
	bl	Func_0800c004
	mov	r0, r8
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	ldr	r1, [pc, #16]
	movs	r3, #0
	movs	r0, #0
	bl	Func_0800b6b8
	mov	r2, r8
	b.n	.L2
	movs	r0, r0
	.4byte 0x00001140
	.4byte 0x02010000
.L2:
	movs	r1, #0
	ldrsh	r0, [r2, r1]
	bl	Func_08185000
	ldrb	r3, [r0, #4]
	cmp	r3, #20
	bne.n	.L3
	mov	r0, r8
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	ldr	r1, [pc, #780]
	adds	r2, #1
	movs	r0, #1
	movs	r3, #0
	bl	Func_0800b6b8
.L3:
	movs	r7, #0
	mov	r6, r8
.L5:
	movs	r1, #0
	ldrsh	r0, [r6, r1]
	bl	Func_08185000
	ldrb	r3, [r0, #4]
	movs	r5, #0
	cmp	r3, #20
	bne.n	.L4
	movs	r3, #1
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L4
	movs	r5, #1
.L4:
	movs	r2, #0
	ldrsh	r0, [r6, r2]
	lsls	r3, r5, #12
	adds	r0, r0, r5
	adds	r0, r0, r3
	bl	Func_0800bc70
	movs	r3, #8
	ldrsh	r1, [r6, r3]
	adds	r5, r0, #0
	bl	Func_0800b8ac
	movs	r0, #16
	ldrsh	r1, [r6, r0]
	adds	r0, r5, #0
	bl	Func_0800b8ac
	adds	r0, r5, #0
	movs	r2, #24
	ldrsh	r1, [r6, r2]
	bl	Func_0800b8ac
	add	r3, sp, #12
	ldrb	r3, [r3, #0]
	adds	r5, #38
	adds	r7, #1
	strb	r3, [r5, #0]
	cmp	r7, #9
	bls.n	.L5
	mov	r6, r8
	movs	r7, #0
	mov	r5, r8
	adds	r6, #4
	movs	r2, #4
.L8:
	mov	r0, r8
	ldrb	r3, [r2, r0]
	cmp	r3, #0
	beq.n	.L6
	movs	r1, #1
	ldrsb	r1, [r6, r1]
	adds	r0, r7, #0
	str	r2, [sp, #4]
	b.n	.L7
.L6:
	adds	r0, r7, #0
	movs	r1, #8
	str	r2, [sp, #4]
.L7:
	bl	Func_08012d48
	ldr	r2, [sp, #4]
	movs	r1, #6
	ldrsb	r1, [r5, r1]
	adds	r0, r7, #0
	str	r2, [sp, #4]
	bl	Func_08012d20
	movs	r3, #2
	ldrsh	r1, [r5, r3]
	adds	r0, r7, #0
	bl	Func_08012d70
	ldr	r2, [sp, #4]
	adds	r7, #1
	adds	r5, #8
	adds	r6, #8
	adds	r2, #8
	cmp	r7, #3
	bls.n	.L8
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #20]
	ldr	r2, [sp, #8]
	bl	Func_08012b2c
	movs	r1, #200
	ldr	r0, [pc, #596]
	lsls	r1, r1, #4
	bl	Func_080041d8
.L31:
	movs	r0, #1
	bl	Func_080030f8
	ldr	r0, [pc, #588]
	ldr	r1, [pc, #588]
	mov	fp, r0
	mov	r9, r1
.L65:
	mov	r3, fp
	ldr	r2, [r3, #0]
	movs	r3, #8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L9
	mov	r0, fp
	ldr	r2, [r0, #0]
	movs	r3, #32
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L10
	ldr	r1, [sp, #24]
	subs	r1, #1
	str	r1, [sp, #24]
.L10:
	mov	r3, fp
	ldr	r2, [r3, #0]
	movs	r3, #16
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L11
	ldr	r0, [sp, #24]
	adds	r0, #1
	str	r0, [sp, #24]
.L11:
	mov	r1, fp
	ldr	r2, [r1, #0]
	movs	r3, #64
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L12
	ldr	r2, [sp, #20]
	subs	r2, #1
	str	r2, [sp, #20]
.L12:
	mov	r3, fp
	ldr	r2, [r3, #0]
	movs	r3, #128
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L13
	ldr	r0, [sp, #20]
	adds	r0, #1
	str	r0, [sp, #20]
.L13:
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #20]
	ldr	r2, [sp, #8]
	bl	Func_08012b2c
	b.n	.L14
.L9:
	ldr	r1, [pc, #488]
	ldr	r3, [r1, #0]
	mov	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #64
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L15
	movs	r0, #1
	negs	r0, r0
	add	sl, r0
	mov	r2, sl
	movs	r3, #3
	ands	r2, r3
	mov	sl, r2
.L15:
	ldr	r2, [r1, #0]
	movs	r3, #128
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L16
	movs	r3, #1
	add	sl, r3
	mov	r0, sl
	movs	r3, #3
	ands	r0, r3
	mov	sl, r0
.L16:
	ldr	r2, [r1, #0]
	movs	r3, #32
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L17
	ldr	r2, [sp, #16]
	movs	r3, #3
	subs	r2, #1
	ands	r2, r3
	str	r2, [sp, #16]
.L17:
	ldr	r2, [r1, #0]
	movs	r3, #16
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L14
	ldr	r3, [sp, #16]
	adds	r3, #1
	str	r3, [sp, #16]
	ldr	r0, [sp, #16]
	movs	r3, #3
	ands	r0, r3
	str	r0, [sp, #16]
.L14:
	ldr	r3, [pc, #396]
	ldr	r2, [r3, #0]
	movs	r3, #8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L18
	ldr	r3, [pc, #388]
	ldr	r1, [sp, #12]
	ldr	r2, [r3, #0]
	movs	r3, #1
	eors	r1, r3
	str	r1, [sp, #12]
	movs	r7, #0
	adds	r2, #38
.L19:
	add	r3, sp, #12
	ldrb	r3, [r3, #0]
	adds	r7, #1
	strb	r3, [r2, #0]
	adds	r2, #56
	cmp	r7, #9
	bls.n	.L19
.L18:
	ldr	r0, [sp, #16]
	cmp	r0, #1
	beq.n	.L20
	cmp	r0, #1
	bcc.n	.L21
	cmp	r0, #2
	bne.n	.L22
	b.n	.L23
.L22:
	cmp	r0, #3
	bne.n	.L24
	b.n	.L25
.L24:
	b.n	.L26
.L21:
	mov	r1, sl
	cmp	r1, #1
	bne.n	.L27
	b.n	.L26
.L27:
	ldr	r1, [pc, #316]
	movs	r3, #128
	ldr	r2, [r1, #0]
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L28
	mov	r2, sl
	mov	r3, r8
	lsls	r6, r2, #3
	adds	r2, r3, r6
	ldrh	r3, [r2, #2]
	subs	r3, #1
	strh	r3, [r2, #2]
	lsls	r3, r3, #16
	movs	r1, #0
	cmp	r3, #0
	bge.n	.L29
	strh	r1, [r2, #2]
.L29:
	mov	r0, sl
	cmp	r0, #0
	beq.n	.L30
	b.n	.L31
.L30:
	mov	r1, r8
	ldrh	r3, [r1, #2]
	mov	r2, r8
	strh	r3, [r2, #10]
	b.n	.L31
.L28:
	ldr	r2, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L32
	b.n	.L26
.L32:
	mov	r3, sl
	lsls	r6, r3, #3
	mov	r0, r8
	adds	r2, r0, r6
	ldrh	r3, [r2, #2]
	movs	r1, #198
	adds	r3, #1
	strh	r3, [r2, #2]
	lsls	r1, r1, #15
	lsls	r3, r3, #16
	cmp	r3, r1
	ble.n	.L33
	movs	r3, #99
	strh	r3, [r2, #2]
.L33:
	mov	r2, sl
	cmp	r2, #0
	beq.n	.L34
	b.n	.L31
.L34:
	mov	r0, r8
	ldrh	r3, [r0, #2]
	mov	r1, r8
	strh	r3, [r1, #10]
	b.n	.L31
.L20:
	mov	r3, fp
	ldr	r2, [r3, #0]
	movs	r3, #8
	ands	r2, r3
	movs	r1, #0
	movs	r4, #1
	cmp	r2, #0
	beq.n	.L35
	movs	r4, #10
.L35:
	mov	r0, r9
	ldr	r2, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L36
	movs	r7, #0
	cmp	r1, r4
	bcs.n	.L37
	mov	r1, sl
	mov	r6, r8
	lsls	r5, r1, #3
.L38:
	movs	r1, #1
	ldrsh	r0, [r6, r5]
	negs	r1, r1
	str	r4, [sp, #0]
	bl	Func_08012af8
	ldr	r4, [sp, #0]
	adds	r7, #1
	strh	r0, [r6, r5]
	cmp	r7, r4
	bcc.n	.L38
.L37:
	movs	r1, #1
.L36:
	mov	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L39
	movs	r7, #0
	cmp	r7, r4
	bcs.n	.L40
	mov	r0, sl
	mov	r6, r8
	lsls	r5, r0, #3
.L41:
	ldrsh	r0, [r6, r5]
	movs	r1, #1
	str	r4, [sp, #0]
	bl	Func_08012af8
	ldr	r4, [sp, #0]
	adds	r7, #1
	strh	r0, [r6, r5]
	cmp	r7, r4
	bcc.n	.L41
.L40:
	movs	r1, #1
.L39:
	cmp	r1, #0
	bne.n	.L42
	b.n	.L26
.L42:
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L43
	b.n	.L44
.L43:
	lsls	r6, r2, #3
	adds	r5, r6, #4
	mov	r0, r8
	ldrb	r3, [r0, r5]
	cmp	r3, #0
	bne.n	.L45
	b.n	.L31
.L45:
	ldrsh	r1, [r0, r6]
	add	r5, r8
	mov	r0, sl
	bl	Func_08012de8
	movs	r1, #1
	ldrsb	r1, [r5, r1]
	mov	r0, sl
	bl	Func_08012d48
	movs	r1, #2
	ldrsb	r1, [r5, r1]
	mov	r0, sl
	bl	Func_08012d20
	mov	r0, r8
	adds	r3, r0, r6
	movs	r2, #2
	ldrsh	r1, [r3, r2]
	mov	r0, sl
	bl	Func_08012d70
	b.n	.L31
	movs	r0, r0
	.4byte 0x02018000
	.4byte 0x08012e29
	.4byte 0x03001ae8
	.4byte 0x03001b04
	.4byte 0x03001c94
	.4byte 0x03001e60
.L23:
	mov	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	movs	r1, #0
	cmp	r2, #0
	beq.n	.L46
	mov	r0, sl
	lsls	r6, r0, #3
	adds	r3, r6, #4
	mov	r1, r8
	adds	r2, r1, r3
	ldrb	r3, [r2, #1]
	subs	r3, #1
	strb	r3, [r2, #1]
	lsls	r3, r3, #24
	cmp	r3, #0
	bge.n	.L47
	movs	r3, #3
	strb	r3, [r2, #1]
.L47:
	movs	r1, #1
.L46:
	mov	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L48
	mov	r0, sl
	lsls	r6, r0, #3
	adds	r3, r6, #4
	mov	r1, r8
	adds	r2, r1, r3
	ldrb	r3, [r2, #1]
	movs	r0, #192
	adds	r3, #1
	strb	r3, [r2, #1]
	lsls	r0, r0, #18
	lsls	r3, r3, #24
	movs	r1, #0
	cmp	r3, r0
	ble.n	.L49
	strb	r1, [r2, #1]
.L49:
	movs	r1, #1
.L48:
	cmp	r1, #0
	beq.n	.L26
	mov	r1, sl
	lsls	r6, r1, #3
	adds	r2, r6, #4
	mov	r0, r8
	ldrb	r3, [r0, r2]
	cmp	r3, #0
	bne.n	.L50
	b.n	.L31
.L50:
	adds	r3, r0, r2
	movs	r1, #1
	ldrsb	r1, [r3, r1]
	b.n	.L51
.L25:
	mov	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	movs	r1, #0
	cmp	r2, #0
	beq.n	.L52
	mov	r0, sl
	lsls	r6, r0, #3
	adds	r3, r6, #4
	mov	r1, r8
	adds	r2, r1, r3
	ldrb	r3, [r2, #2]
	subs	r3, #1
	strb	r3, [r2, #2]
	lsls	r3, r3, #24
	cmp	r3, #0
	bge.n	.L53
	movs	r3, #15
	strb	r3, [r2, #2]
.L53:
	movs	r1, #1
.L52:
	mov	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L54
	mov	r0, sl
	lsls	r6, r0, #3
	adds	r3, r6, #4
	mov	r1, r8
	adds	r2, r1, r3
	ldrb	r3, [r2, #2]
	movs	r0, #240
	adds	r3, #1
	strb	r3, [r2, #2]
	lsls	r0, r0, #20
	lsls	r3, r3, #24
	movs	r1, #0
	cmp	r3, r0
	ble.n	.L55
	strb	r1, [r2, #2]
.L55:
	movs	r1, #1
.L54:
	cmp	r1, #0
	beq.n	.L26
	mov	r1, sl
	lsls	r6, r1, #3
	adds	r2, r6, #4
	mov	r0, r8
	ldrb	r3, [r0, r2]
	cmp	r3, #0
	bne.n	.L56
	b.n	.L31
.L56:
	adds	r3, r0, r2
	movs	r1, #2
	ldrsb	r1, [r3, r1]
	mov	r0, sl
	bl	Func_08012d20
	b.n	.L31
.L26:
	mov	r1, r9
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L57
	ldr	r2, [sp, #16]
	cmp	r2, #0
	bne.n	.L58
	mov	r3, sl
	cmp	r3, #1
	beq.n	.L57
	lsls	r6, r3, #3
	adds	r3, r6, #4
	mov	r0, r8
	ldrb	r3, [r0, r3]
	cmp	r3, #0
	bne.n	.L59
	b.n	.L31
.L59:
	adds	r3, r0, r6
	movs	r2, #2
	ldrsh	r1, [r3, r2]
	mov	r0, sl
	bl	Func_08012d70
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L60
	b.n	.L31
.L60:
	mov	r2, r8
	movs	r0, #10
	ldrsh	r1, [r2, r0]
	movs	r0, #1
	bl	Func_08012d70
	b.n	.L31
.L58:
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L57
	lsls	r3, r3, #3
	adds	r1, r3, #4
	mov	r0, r8
	ldrb	r2, [r0, r1]
	movs	r3, #1
	eors	r2, r3
	strb	r2, [r0, r1]
	cmp	r2, #0
	beq.n	.L61
	adds	r3, r0, r1
	movs	r1, #1
	ldrsb	r1, [r3, r1]
	b.n	.L51
.L61:
	movs	r1, #8
.L51:
	mov	r0, sl
	bl	Func_08012d48
	b.n	.L31
.L57:
	mov	r1, r9
	ldr	r2, [r1, #0]
	movs	r3, #4
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L62
	bl	Func_080040e8
	mov	r2, fp
	ldr	r3, [r2, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L63
	ldr	r0, [pc, #20]
	b.n	.L64
.L63:
	ldr	r0, [pc, #20]
.L64:
	ldr	r1, [pc, #24]
	bl	Func_08002f0c
	b.n	.L44
.L62:
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L65
	movs	r0, r0
	.4byte 0x00000011
	.4byte 0x00000012
	.4byte 0x08185000

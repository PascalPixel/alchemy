@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080bae40
	.thumb_func
Func_080bae40:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #92
	str	r0, [sp, #28]
	ldr	r3, [pc, #44]
	movs	r0, #0
	adds	r7, r1, #0
	ldr	r1, [r3, #0]
	str	r0, [sp, #20]
	ldrb	r3, [r7, #0]
	mov	r8, r0
	cmp	r3, #0
	bne.n	.L0
	b.n	.L1
.L0:
	movs	r2, #0
	cmp	r3, #2
	beq.n	.L2
	cmp	r3, #4
	bne.n	.L3
.L2:
	movs	r2, #1
.L3:
	ldr	r3, [sp, #28]
	cmp	r3, #7
	bls.n	.L4
	cmp	r2, #0
	beq.n	.L5
	b.n	.L6
	movs	r0, r0
	.4byte 0x03001e74
.L4:
	cmp	r2, #0
	beq.n	.L6
.L5:
	movs	r4, #0
	str	r4, [sp, #24]
	movs	r3, #88
	ldrsh	r3, [r1, r3]
	cmp	r3, #255
	beq.n	.L1
	mov	r0, r8
	lsls	r3, r0, #1
	add	r2, sp, #92
	adds	r3, r3, r2
	mov	r4, r8
	adds	r0, r3, #0
	lsls	r3, r4, #2
	adds	r3, r3, r2
	adds	r4, r3, #0
	ldr	r5, [pc, #28]
	adds	r1, #88
	subs	r0, #12
	subs	r4, #36
.L9:
	movs	r6, #0
	ldrsh	r2, [r1, r6]
	cmp	r2, #254
	beq.n	.L7
	ldrb	r3, [r7, #0]
	cmp	r3, #4
	bne.n	.L8
	ldr	r3, [sp, #28]
	cmp	r2, r3
	bne.n	.L7
	b.n	.L8
	.4byte 0x00000100
.L8:
	stmia	r4!, {r2}
	ldr	r3, [sp, #24]
	movs	r6, #1
	orrs	r3, r5
	strh	r3, [r0, #0]
	add	r8, r6
	adds	r0, #2
.L7:
	ldr	r2, [sp, #24]
	adds	r2, #1
	str	r2, [sp, #24]
	adds	r1, #2
	movs	r6, #0
	ldrsh	r3, [r1, r6]
	cmp	r3, #255
	bne.n	.L9
	b.n	.L1
.L6:
	movs	r0, #0
	str	r0, [sp, #24]
	adds	r2, r1, #2
	movs	r3, #100
	ldrsh	r3, [r2, r3]
	mov	ip, r2
	cmp	r3, #255
	beq.n	.L1
	mov	r4, r8
	lsls	r3, r4, #1
	add	r6, sp, #92
	adds	r3, r3, r6
	adds	r1, r3, #0
	lsls	r3, r4, #2
	adds	r3, r3, r6
	adds	r4, r3, #0
	ldr	r5, [pc, #24]
	movs	r0, #100
	subs	r1, #12
	subs	r4, #36
.L12:
	ldrsh	r2, [r2, r0]
	cmp	r2, #254
	beq.n	.L10
	ldrb	r3, [r7, #0]
	cmp	r3, #4
	bne.n	.L11
	ldr	r6, [sp, #28]
	cmp	r2, r6
	bne.n	.L10
	b.n	.L11
	.4byte 0x00000180
.L11:
	stmia	r4!, {r2}
	ldr	r3, [sp, #24]
	movs	r2, #1
	orrs	r3, r5
	strh	r3, [r1, #0]
	add	r8, r2
	adds	r1, #2
.L10:
	ldr	r3, [sp, #24]
	adds	r3, #1
	str	r3, [sp, #24]
	adds	r0, #2
	mov	r2, ip
	ldrsh	r3, [r2, r0]
	cmp	r3, #255
	bne.n	.L12
.L1:
	mov	r0, r8
	cmp	r0, #0
	bne.n	.L13
	movs	r0, #2
	negs	r0, r0
	b.n	.L14
.L13:
	movs	r1, #0
	str	r1, [sp, #24]
	cmp	r1, r8
	blt.n	.L15
	b.n	.L16
.L15:
	add	r2, sp, #56
	mov	sl, r2
.L58:
	ldr	r3, [sp, #24]
	mov	r4, sl
	lsls	r3, r3, #2
	ldr	r0, [r4, r3]
	mov	r9, r3
	bl	Func_08077008
	adds	r5, r0, #0
	ldrb	r0, [r7, #3]
	movs	r6, #0
	cmp	r0, #64
	bls.n	.L17
	b.n	.L18
.L17:
	ldr	r2, [pc, #860]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080bb304
	.4byte 0x080bb304
	.4byte 0x080bb304
	.4byte 0x080bb132
	.4byte 0x080bb146
	.4byte 0x080bb2c8
	.4byte 0x080bb088
	.4byte 0x080bb088
	.4byte 0x080bb0ac
	.4byte 0x080bb0ac
	.4byte 0x080bb0c2
	.4byte 0x080bb0c2
	.4byte 0x080bb0dc
	.4byte 0x080bb0dc
	.4byte 0x080bb0fa
	.4byte 0x080bb0fa
	.4byte 0x080bb114
	.4byte 0x080bb114
	.4byte 0x080bb290
	.4byte 0x080bb29c
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb2ac
	.4byte 0x080bb2b2
	.4byte 0x080bb142
	.4byte 0x080bb2ba
	.4byte 0x080bb142
	.4byte 0x080bb28a
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb1ac
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb2c8
	.4byte 0x080bb2c8
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb142
	.4byte 0x080bb198
	.4byte 0x080bb198
	.4byte 0x080bb142
	.4byte 0x080bb222
	ldr	r1, [pc, #592]
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r3, #1
	cmp	r3, #4
	bgt.n	.L19
.L22:
	movs	r6, #1
.L19:
	movs	r2, #153
	lsls	r2, r2, #1
	adds	r3, r5, r2
.L24:
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	beq.n	.L20
	b.n	.L21
.L20:
	adds	r6, #1
	b.n	.L21
	ldr	r4, [pc, #556]
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r1, #4
	subs	r3, #1
	negs	r1, r1
	cmp	r3, r1
	blt.n	.L19
	b.n	.L22
	ldr	r4, [pc, #540]
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r3, #1
	cmp	r3, #4
	bgt.n	.L23
	movs	r6, #1
.L23:
	movs	r1, #154
	lsls	r1, r1, #1
	adds	r3, r5, r1
	b.n	.L24
	ldr	r2, [pc, #512]
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r4, #4
	subs	r3, #1
	negs	r4, r4
	cmp	r3, r4
	blt.n	.L25
	movs	r6, #1
.L25:
	movs	r1, #154
	lsls	r1, r1, #1
	adds	r3, r5, r1
	b.n	.L24
	ldr	r2, [pc, #488]
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r3, #1
	cmp	r3, #4
	bgt.n	.L26
	movs	r6, #1
.L26:
	movs	r4, #155
	lsls	r4, r4, #1
	adds	r3, r5, r4
	b.n	.L24
	ldr	r1, [pc, #460]
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r2, #4
	subs	r3, #1
	negs	r2, r2
	cmp	r3, r2
	blt.n	.L27
	movs	r6, #1
.L27:
	movs	r4, #155
	lsls	r4, r4, #1
	adds	r3, r5, r4
	b.n	.L24
	ldr	r1, [pc, #436]
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L18
	b.n	.L21
.L18:
	movs	r6, #1
	b.n	.L21
	movs	r2, #156
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L28
	movs	r6, #1
.L28:
	ldr	r4, [pc, #404]
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L29
	adds	r6, #1
.L29:
	movs	r1, #157
	lsls	r1, r1, #1
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L30
	adds	r6, #1
.L30:
	movs	r2, #158
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L31
	adds	r6, #1
.L31:
	ldr	r4, [pc, #368]
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L32
	adds	r6, #1
.L32:
	ldr	r1, [pc, #360]
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
.L49:
	cmp	r3, #0
	bne.n	.L33
	b.n	.L21
.L33:
	adds	r6, #1
	b.n	.L21
	movs	r3, #56
	ldrsh	r2, [r5, r3]
	movs	r4, #52
	ldrsh	r3, [r5, r4]
	ldrh	r1, [r5, #56]
	cmp	r2, r3
	blt.n	.L34
	b.n	.L35
.L34:
	movs	r6, #1
	b.n	.L35
	ldr	r1, [pc, #300]
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L36
	movs	r6, #1
.L36:
	ldr	r2, [pc, #288]
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L37
	adds	r6, #1
.L37:
	ldr	r4, [pc, #276]
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L38
	adds	r6, #1
.L38:
	movs	r1, #150
	lsls	r1, r1, #1
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L39
	adds	r6, #1
.L39:
	ldr	r2, [pc, #264]
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L40
	adds	r6, #1
.L40:
	movs	r4, #151
	lsls	r4, r4, #1
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L41
	adds	r6, #1
.L41:
	ldr	r1, [pc, #232]
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	ble.n	.L21
	adds	r6, #1
	b.n	.L21
	movs	r2, #156
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L42
	movs	r6, #1
.L42:
	ldr	r4, [pc, #184]
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L43
	adds	r6, #1
.L43:
	movs	r1, #157
	lsls	r1, r1, #1
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L44
	adds	r6, #1
.L44:
	movs	r2, #158
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L45
	adds	r6, #1
.L45:
	ldr	r4, [pc, #148]
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L46
	adds	r6, #1
.L46:
	ldr	r1, [pc, #140]
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L47
	adds	r6, #1
.L47:
	movs	r2, #160
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L48
	adds	r6, #1
.L48:
	ldr	r4, [pc, #104]
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	b.n	.L49
	ldr	r1, [pc, #104]
	adds	r3, r5, r1
	b.n	.L50
	ldr	r2, [pc, #84]
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	b.n	.L51
	ldr	r4, [pc, #72]
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #1
	bgt.n	.L21
	b.n	.L18
	ldr	r1, [pc, #80]
	adds	r3, r5, r1
	b.n	.L50
	movs	r2, #158
	lsls	r2, r2, #1
	adds	r3, r5, r2
	b.n	.L50
	movs	r4, #160
	lsls	r4, r4, #1
	adds	r3, r5, r4
.L50:
	ldrb	r3, [r3, #0]
.L51:
	cmp	r3, #0
	bne.n	.L21
	b.n	.L18
	movs	r2, #56
	ldrsh	r3, [r5, r2]
	ldrh	r1, [r5, #56]
	cmp	r3, #0
	bne.n	.L52
	movs	r6, #100
	b.n	.L35
	movs	r0, r0
	.4byte 0x080baf84
	.4byte 0x00000133
	.4byte 0x00000135
	.4byte 0x00000137
	.4byte 0x00000131
	.4byte 0x00000139
	.4byte 0x0000013d
	.4byte 0x00000141
	.4byte 0x0000012d
	.4byte 0x0000012f
	.4byte 0x0000013b
.L21:
	ldrh	r1, [r5, #56]
.L35:
	lsls	r3, r1, #16
	cmp	r3, #0
	bne.n	.L52
	bl	Func_080772b8
	cmp	r0, #0
	bne.n	.L52
	movs	r6, #0
.L52:
	cmp	r6, #0
	bne.n	.L53
	ldrb	r2, [r7, #1]
	movs	r3, #15
	ands	r3, r2
	subs	r3, #1
	cmp	r3, #9
	bhi.n	.L54
	ldr	r2, [pc, #604]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080bb364
	.4byte 0x080bb376
	.4byte 0x080bb358
	.4byte 0x080bb358
	.4byte 0x080bb376
	.4byte 0x080bb376
	.4byte 0x080bb386
	.4byte 0x080bb376
	.4byte 0x080bb376
	.4byte 0x080bb35e
	movs	r4, #56
	ldrsh	r3, [r5, r4]
	b.n	.L55
	movs	r1, #58
	ldrsh	r3, [r5, r1]
	b.n	.L55
	movs	r3, #56
	ldrsh	r2, [r5, r3]
	cmp	r2, #0
	beq.n	.L54
	movs	r4, #52
	ldrsh	r3, [r5, r4]
	cmp	r2, r3
	bge.n	.L54
	b.n	.L56
	ldrh	r3, [r7, #10]
	cmp	r3, #0
	beq.n	.L54
	movs	r0, #56
	ldrsh	r3, [r5, r0]
.L55:
	cmp	r3, #0
	beq.n	.L54
.L56:
	adds	r6, #1
.L54:
	cmp	r6, #0
	beq.n	.L57
.L53:
	ldr	r2, [sp, #20]
	mov	r4, sl
	mov	r6, r9
	ldr	r3, [r4, r6]
	lsls	r1, r2, #2
	str	r3, [r4, r1]
	ldr	r4, [sp, #24]
	add	r2, sp, #80
	lsls	r3, r4, #1
	ldrh	r3, [r2, r3]
	mov	r0, sp
	adds	r0, #32
	str	r3, [r0, r1]
	ldr	r6, [sp, #20]
	adds	r6, #1
	str	r6, [sp, #20]
.L57:
	ldr	r0, [sp, #24]
	adds	r0, #1
	str	r0, [sp, #24]
	cmp	r0, r8
	bge.n	.L16
	b.n	.L58
.L16:
	ldr	r1, [sp, #20]
	cmp	r1, #0
	bne.n	.L59
	movs	r0, #1
	negs	r0, r0
	b.n	.L14
.L59:
	ldrb	r3, [r7, #0]
	mov	r2, sp
	adds	r2, #32
	str	r2, [sp, #12]
	cmp	r3, #1
	beq.n	.L60
	b.n	.L61
.L60:
	ldrb	r3, [r7, #8]
	cmp	r3, #1
	beq.n	.L62
	b.n	.L63
.L62:
	ldr	r0, [sp, #28]
	bl	Func_08077008
	movs	r3, #148
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldrb	r0, [r0, #0]
	bl	Func_08077198
	adds	r0, #53
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	cmp	r3, #2
	bne.n	.L64
	b.n	.L65
.L64:
	ldrb	r2, [r7, #1]
	movs	r3, #15
	ands	r3, r2
	subs	r3, #3
	cmp	r3, #2
	bls.n	.L66
	b.n	.L67
.L66:
	ldr	r6, [sp, #20]
	movs	r1, #1
	movs	r4, #0
	negs	r1, r1
	str	r4, [sp, #24]
	cmp	r4, r6
	bge.n	.L68
	mov	r0, sp
	adds	r0, #32
	subs	r6, #1
	str	r0, [sp, #12]
	str	r6, [sp, #8]
.L74:
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #8]
	cmp	r2, r3
	bge.n	.L69
	mov	r4, sp
	lsls	r3, r2, #2
	adds	r4, #56
	adds	r6, r3, r4
	str	r4, [sp, #16]
	adds	r0, r3, #4
	adds	r5, r3, #0
	ldr	r4, [sp, #24]
	ldr	r3, [sp, #8]
	ldr	r2, [sp, #12]
	ldr	r7, [sp, #16]
	mov	sl, r0
	subs	r3, r3, r4
	str	r6, [sp, #4]
	mov	r9, r2
	mov	fp, r3
	add	r7, sl
.L73:
	ldr	r6, [sp, #16]
	ldr	r0, [r5, r6]
	str	r1, [sp, #0]
	bl	Func_08077008
	mov	r8, r0
	ldr	r0, [r7, #0]
	bl	Func_08077008
	adds	r6, r0, #0
	ldr	r0, [sp, #28]
	bl	Func_08077008
	movs	r2, #148
	lsls	r2, r2, #1
	adds	r0, r0, r2
	ldrb	r0, [r0, #0]
	bl	Func_08077198
	adds	r0, #53
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	ldr	r1, [sp, #0]
	cmp	r3, #0
	bne.n	.L70
	mov	r0, r8
	movs	r4, #56
	ldrsh	r3, [r0, r4]
	movs	r2, #56
	ldrsh	r0, [r6, r2]
	b.n	.L71
.L70:
	mov	r0, r8
	movs	r4, #52
	ldrsh	r3, [r0, r4]
	movs	r2, #52
	ldrsh	r0, [r6, r2]
.L71:
	cmp	r3, r0
	bge.n	.L72
	ldr	r3, [sp, #16]
	ldr	r4, [sp, #4]
	ldr	r2, [r5, r3]
	ldr	r3, [r7, #0]
	str	r3, [r4, #0]
	str	r2, [r7, #0]
	mov	r6, r9
	mov	r0, sl
	ldr	r2, [r6, r5]
	ldr	r3, [r6, r0]
	str	r3, [r6, r5]
	str	r2, [r6, r0]
.L72:
	movs	r4, #1
	ldr	r2, [sp, #4]
	negs	r4, r4
	add	fp, r4
	adds	r2, #4
	movs	r3, #4
	mov	r6, fp
	str	r2, [sp, #4]
	adds	r7, #4
	add	sl, r3
	adds	r5, #4
	cmp	r6, #0
	bne.n	.L73
.L69:
	ldr	r0, [sp, #24]
	ldr	r2, [sp, #20]
	adds	r0, #1
	str	r0, [sp, #24]
	cmp	r0, r2
	blt.n	.L74
	b.n	.L75
.L68:
	mov	r3, sp
	adds	r3, #32
	str	r3, [sp, #12]
.L75:
	ldr	r4, [sp, #20]
	cmp	r4, #2
	beq.n	.L76
	cmp	r4, #2
	bgt.n	.L77
	cmp	r4, #1
	beq.n	.L78
	b.n	.L79
.L77:
	ldr	r6, [sp, #20]
	cmp	r6, #3
	beq.n	.L80
	cmp	r6, #4
	beq.n	.L81
	b.n	.L79
.L76:
	bl	Func_08004458
	movs	r3, #11
	muls	r3, r0
	lsrs	r3, r3, #16
	movs	r1, #0
	cmp	r3, #5
	bls.n	.L79
.L83:
	movs	r1, #1
	b.n	.L79
.L80:
	bl	Func_08004458
	lsls	r3, r0, #4
	subs	r3, r3, r0
	lsrs	r1, r3, #16
	cmp	r1, #5
	ble.n	.L78
	cmp	r1, #10
	bgt.n	.L82
	b.n	.L83
.L81:
	bl	Func_08004458
	lsls	r3, r0, #3
	adds	r3, r3, r0
	lsls	r3, r3, #1
	lsrs	r1, r3, #16
	cmp	r1, #5
	bgt.n	.L84
.L78:
	movs	r1, #0
	b.n	.L79
.L84:
	cmp	r1, #10
	ble.n	.L83
	cmp	r1, #14
	bgt.n	.L85
.L82:
	movs	r1, #2
	b.n	.L79
.L85:
	movs	r1, #3
.L79:
	cmp	r1, #0
	blt.n	.L61
	lsls	r3, r1, #2
	ldr	r1, [sp, #12]
	ldr	r0, [r1, r3]
	b.n	.L14
.L63:
	mov	r2, sp
	adds	r2, #32
	str	r2, [sp, #12]
	b.n	.L61
.L65:
	mov	r3, sp
	adds	r3, #32
	str	r3, [sp, #12]
	b.n	.L61
.L67:
	mov	r4, sp
	adds	r4, #32
	str	r4, [sp, #12]
.L61:
	bl	Func_08004458
	ldr	r6, [sp, #20]
	adds	r3, r6, #0
	muls	r3, r0
	ldr	r1, [sp, #12]
	lsrs	r3, r3, #16
	lsls	r3, r3, #2
	ldr	r0, [r1, r3]
.L14:
	add	sp, #92
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x080bb330

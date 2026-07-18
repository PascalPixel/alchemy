@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08018038
	.thumb_func
Func_08018038:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #132
	str	r0, [sp, #48]
	ldr	r0, [pc, #292]
	mov	fp, r1
	ldr	r1, [r0, #0]
	movs	r2, #1
	movs	r3, #0
	ldr	r5, [pc, #284]
	str	r1, [sp, #44]
	str	r2, [sp, #40]
	str	r3, [sp, #36]
	adds	r3, r1, r5
	ldrh	r3, [r3, #0]
	str	r2, [sp, #20]
	movs	r2, #235
	str	r3, [sp, #32]
	adds	r6, r3, #0
	lsls	r2, r2, #4
	movs	r5, #1
	ldr	r3, [sp, #48]
	mov	sl, r0
	movs	r7, #0
	movs	r0, #0
	adds	r1, r1, r2
	negs	r5, r5
	str	r0, [sp, #28]
	str	r0, [sp, #24]
	str	r7, [sp, #52]
	mov	r8, r1
	str	r0, [sp, #16]
	cmp	r3, r5
	bne.n	.L0
	ldr	r0, [sp, #44]
	ldr	r1, [pc, #236]
	adds	r3, r0, r1
	ldrh	r3, [r3, #0]
	str	r3, [sp, #32]
	b.n	.L1
.L0:
	ldr	r5, [pc, #232]
	movs	r0, #50
	adds	r1, r5, #0
	bl	Func_080048b0
	movs	r2, #132
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	adds	r1, r0, #0
	ldr	r3, [pc, #216]
	ldr	r0, [pc, #220]
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r3, sl
	add	r2, sp, #56
	adds	r3, #140
	ldr	r3, [r3, #0]
	mov	sl, r2
	mov	r0, sl
	ldr	r1, [sp, #48]
	mov	r9, r3
	bl	Func_08019bac
	mov	r3, sp
	adds	r3, #84
	str	r3, [sp, #12]
.L80:
	mov	r0, sl
	bl	Func_08007308
	adds	r5, r7, #0
	adds	r7, r0, #0
	cmp	r7, #255
	bls.n	.L2
	movs	r7, #64
.L2:
	ldr	r0, [sp, #16]
	cmp	r0, #0
	beq.n	.L3
	cmp	r7, #31
	bls.n	.L4
	b.n	.L5
.L4:
	cmp	r7, #18
	beq.n	.L6
	cmp	r7, #18
	bhi.n	.L7
	cmp	r7, #9
	bhi.n	.L8
	cmp	r7, #8
	bcs.n	.L6
	cmp	r7, #1
	beq.n	.L9
	cmp	r7, #1
	bcc.n	.L10
	cmp	r7, #2
	beq.n	.L10
	b.n	.L5
.L8:
	cmp	r7, #16
	bne.n	.L11
	b.n	.L5
.L11:
	cmp	r7, #17
	beq.n	.L6
	b.n	.L5
.L7:
	cmp	r7, #22
	beq.n	.L12
	cmp	r7, #22
	bhi.n	.L13
	cmp	r7, #20
	beq.n	.L14
	cmp	r7, #20
	bhi.n	.L15
	mov	r0, sl
	bl	Func_08007308
	movs	r0, #3
	b.n	.L16
.L13:
	cmp	r7, #29
	beq.n	.L6
	cmp	r7, #29
	bhi.n	.L17
	cmp	r7, #23
	beq.n	.L18
	b.n	.L5
.L17:
	cmp	r7, #30
	beq.n	.L10
	b.n	.L5
.L10:
	movs	r1, #0
	str	r1, [sp, #20]
	b.n	.L5
.L12:
	movs	r0, #5
	b.n	.L16
.L14:
	mov	r0, sl
	bl	Func_08007308
	movs	r0, #2
	b.n	.L16
.L15:
	movs	r0, #4
	b.n	.L16
.L18:
	movs	r0, #6
.L16:
	mov	r1, fp
	bl	Func_08019944
	b.n	.L5
.L6:
	mov	r0, sl
	bl	Func_08007308
	b.n	.L5
.L9:
	movs	r2, #0
	str	r2, [sp, #20]
	movs	r7, #2
	b.n	.L5
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x000012b2
	.4byte 0x000012b4
	.4byte 0x00000140
	.4byte 0x040000d4
	.4byte 0x08015430
.L3:
	ldr	r0, [sp, #44]
	ldr	r1, [pc, #68]
	adds	r3, r0, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L19
	ldr	r2, [sp, #40]
	cmp	r2, #0
	bne.n	.L19
	cmp	r7, #222
	beq.n	.L19
	cmp	r7, #223
	beq.n	.L19
	ldr	r3, [pc, #40]
	lsls	r2, r6, #1
	mov	r0, r8
	ldr	r1, [pc, #40]
	adds	r6, #1
	strh	r3, [r2, r0]
	ands	r6, r1
.L19:
	ldr	r2, [sp, #44]
	ldr	r0, [pc, #36]
	adds	r3, r2, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L20
	ldr	r1, [sp, #40]
	cmp	r1, #0
	bne.n	.L20
	cmp	r7, #222
	beq.n	.L20
	cmp	r7, #223
	beq.n	.L20
	b.n	.L21
	.4byte 0x00000005
	.4byte 0x000012fa
	.4byte 0x000001ff
	.4byte 0x000012fb
.L21:
	movs	r2, #128
	lsls	r2, r2, #1
	cmp	r5, r2
	bhi.n	.L20
	cmp	r5, #127
	bls.n	.L20
	cmp	r5, #222
	beq.n	.L20
	cmp	r5, #223
	beq.n	.L20
	cmp	r5, #32
	beq.n	.L20
	cmp	r5, #165
	beq.n	.L20
	cmp	r5, #161
	beq.n	.L20
	cmp	r5, #164
	beq.n	.L20
	ldr	r3, [pc, #40]
	lsls	r2, r6, #1
	mov	r5, r8
	ldr	r0, [pc, #40]
	adds	r6, #1
	strh	r3, [r2, r5]
	ands	r6, r0
.L20:
	cmp	r7, #31
	bls.n	.L22
	ldr	r1, [sp, #44]
	ldr	r2, [pc, #28]
	adds	r3, r1, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L23
	cmp	r7, #32
	beq.n	.L24
	ldr	r3, [sp, #28]
	cmp	r3, #10
	bls.n	.L23
.L24:
	ldr	r0, [pc, #4]
	b.n	.L25
	.4byte 0x000000de
	.4byte 0x000001ff
	.4byte 0x000012fa
.L25:
	lsls	r3, r6, #1
	ldr	r2, [pc, #40]
	adds	r6, #1
	ands	r6, r0
	mov	r5, r8
	strh	r2, [r3, r5]
	lsls	r3, r6, #1
	adds	r6, #1
	ands	r6, r0
	mov	r1, r8
	strh	r2, [r3, r1]
	lsls	r3, r6, #1
	strh	r2, [r3, r5]
	adds	r6, #1
	ldr	r1, [sp, #28]
	ands	r6, r0
	movs	r0, #1
	str	r0, [sp, #16]
	cmp	r1, #10
	bls.n	.L23
	movs	r7, #32
	b.n	.L23
	.4byte 0x0000002e
.L23:
	cmp	r7, #34
	bne.n	.L26
	ldr	r2, [sp, #36]
	movs	r3, #1
	eors	r2, r3
	str	r2, [sp, #36]
	cmp	r2, #0
	beq.n	.L26
	movs	r7, #142
.L26:
	ldr	r0, [pc, #752]
	lsls	r3, r6, #1
	mov	r5, r8
	adds	r6, #1
	movs	r1, #0
	strh	r7, [r3, r5]
	ands	r6, r0
	str	r1, [sp, #40]
	b.n	.L5
.L22:
	cmp	r7, #20
	bne.n	.L27
	b.n	.L28
.L27:
	cmp	r7, #20
	bhi.n	.L29
	cmp	r7, #9
	bhi.n	.L30
	cmp	r7, #8
	bcs.n	.L31
	cmp	r7, #1
	bne.n	.L32
	b.n	.L33
.L32:
	cmp	r7, #1
	bcc.n	.L34
	cmp	r7, #2
	beq.n	.L34
	cmp	r7, #3
	bne.n	.L35
	b.n	.L33
.L35:
	b.n	.L36
.L30:
	cmp	r7, #17
	bne.n	.L37
	b.n	.L38
.L37:
	cmp	r7, #17
	bhi.n	.L39
	cmp	r7, #16
	bne.n	.L40
	b.n	.L41
.L40:
	b.n	.L36
.L39:
	cmp	r7, #18
	bne.n	.L42
	b.n	.L43
.L42:
	cmp	r7, #19
	beq.n	.L44
	b.n	.L36
.L29:
	cmp	r7, #25
	bne.n	.L45
	b.n	.L46
.L45:
	cmp	r7, #25
	bhi.n	.L47
	cmp	r7, #22
	beq.n	.L48
	cmp	r7, #22
	bcs.n	.L49
	b.n	.L50
.L49:
	cmp	r7, #23
	bne.n	.L51
	b.n	.L52
.L51:
	cmp	r7, #24
	bne.n	.L53
	b.n	.L54
.L53:
	b.n	.L36
.L47:
	cmp	r7, #29
	beq.n	.L31
	cmp	r7, #29
	bhi.n	.L55
	cmp	r7, #26
	bne.n	.L56
	b.n	.L57
.L56:
	cmp	r7, #27
	bne.n	.L58
	b.n	.L59
.L58:
	b.n	.L36
.L55:
	cmp	r7, #30
	beq.n	.L34
	movs	r2, #1
	negs	r2, r2
	cmp	r7, r2
	bne.n	.L60
	b.n	.L5
.L60:
	b.n	.L36
.L34:
	movs	r3, #0
	str	r3, [sp, #20]
	b.n	.L5
.L31:
	ldr	r0, [pc, #584]
	lsls	r3, r6, #1
	mov	r5, r8
	adds	r6, #1
	ands	r6, r0
	strh	r7, [r3, r5]
	mov	r0, sl
	bl	Func_08007308
	ldr	r1, [pc, #572]
	lsls	r3, r6, #1
	adds	r0, r0, r1
	mov	r2, r8
	strh	r0, [r3, r2]
	ldr	r3, [pc, #556]
	adds	r6, #1
	ands	r6, r3
	b.n	.L5
.L48:
	mov	r1, fp
	movs	r0, #5
	bl	Func_08019944
	adds	r1, r0, #0
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L61
	negs	r3, r1
.L61:
	movs	r5, #1
	str	r5, [sp, #24]
	cmp	r3, #1
	bgt.n	.L62
	movs	r0, #0
	str	r0, [sp, #24]
.L62:
	add	r5, sp, #68
	adds	r0, r5, #0
	movs	r2, #0
	bl	Func_08017dd4
	subs	r4, r0, r5
	cmp	r4, #16
	bne.n	.L63
	b.n	.L5
.L63:
	ldrb	r3, [r5, r4]
	cmp	r3, #0
	bne.n	.L64
	b.n	.L5
.L64:
	ldr	r1, [pc, #492]
	adds	r0, r4, r5
	mov	ip, r1
	adds	r1, r0, #0
.L66:
	ldrb	r3, [r1, #0]
	lsls	r2, r6, #1
	mov	r5, r8
	strh	r3, [r2, r5]
	adds	r6, #1
	mov	r2, ip
	adds	r4, #1
	adds	r1, #1
	ands	r6, r2
	cmp	r4, #16
	bne.n	.L65
	b.n	.L5
.L65:
	adds	r0, #1
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	bne.n	.L66
	b.n	.L5
.L44:
	mov	r0, sl
	bl	Func_08007308
	mov	r1, fp
	subs	r5, r0, #1
	movs	r0, #3
	bl	Func_08019944
	adds	r2, r0, #0
	ldr	r0, [pc, #440]
	ldr	r1, [sp, #12]
	adds	r0, r2, r0
	movs	r2, #24
	bl	Func_080196c4
	ldr	r3, [sp, #24]
	str	r3, [sp, #4]
	add	r3, sp, #52
	str	r3, [sp, #8]
	adds	r2, r6, #0
	movs	r0, #0
	ldr	r1, [sp, #12]
	mov	r3, r8
	str	r5, [sp, #0]
	b.n	.L67
.L28:
	mov	r0, sl
	bl	Func_08007308
	mov	r1, fp
	subs	r5, r0, #1
	movs	r0, #2
	bl	Func_08019944
	adds	r2, r0, #0
	ldr	r0, [pc, #384]
	ands	r2, r0
	ldr	r0, [pc, #392]
	ldr	r1, [sp, #12]
	adds	r0, r2, r0
	movs	r2, #24
	bl	Func_080196c4
	ldr	r1, [sp, #24]
	add	r3, sp, #52
	str	r1, [sp, #4]
	str	r3, [sp, #8]
	adds	r2, r6, #0
	movs	r0, #0
	ldr	r1, [sp, #12]
	mov	r3, r8
	str	r5, [sp, #0]
	b.n	.L67
.L50:
	mov	r1, fp
	movs	r0, #4
	bl	Func_08019944
	adds	r2, r0, #0
	ldr	r0, [pc, #352]
	ldr	r1, [sp, #12]
	adds	r0, r2, r0
	movs	r2, #24
	bl	Func_080196c4
	ldr	r1, [sp, #12]
	ldrh	r2, [r1, #0]
	adds	r3, r2, #0
	adds	r0, r6, #0
	cmp	r3, #0
	beq.n	.L68
	ldr	r4, [pc, #312]
.L69:
	lsls	r3, r0, #1
	mov	r5, r8
	strh	r2, [r3, r5]
	adds	r1, #2
	ldrh	r2, [r1, #0]
	adds	r0, #1
	adds	r3, r2, #0
	ands	r0, r4
	cmp	r3, #0
	bne.n	.L69
	b.n	.L68
.L52:
	mov	r1, fp
	movs	r0, #6
	bl	Func_08019944
	movs	r1, #1
	bl	Func_0808a5d0
	ldr	r3, [pc, #296]
	ldr	r1, [sp, #12]
	adds	r0, r0, r3
	movs	r2, #24
	bl	Func_080196c4
	ldr	r1, [sp, #12]
	ldrh	r2, [r1, #0]
	adds	r3, r2, #0
	adds	r0, r6, #0
	cmp	r3, #0
	beq.n	.L68
	ldr	r4, [pc, #252]
.L70:
	lsls	r3, r0, #1
	mov	r5, r8
	strh	r2, [r3, r5]
	adds	r1, #2
	ldrh	r2, [r1, #0]
	adds	r0, #1
	adds	r3, r2, #0
	ands	r0, r4
	cmp	r3, #0
	bne.n	.L70
	b.n	.L68
.L41:
	ldr	r3, [pc, #252]
	movs	r0, #250
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl	Func_08077008
	add	r1, sp, #84
	adds	r2, r1, #0
	movs	r4, #0
.L71:
	ldrb	r3, [r0, #0]
	adds	r4, #1
	strh	r3, [r2, #0]
	adds	r0, #1
	adds	r2, #2
	cmp	r4, #14
	bls.n	.L71
	b.n	.L72
.L43:
	mov	r0, sl
	bl	Func_08007308
	mov	r1, fp
	subs	r5, r0, #1
	movs	r0, #1
	bl	Func_08019944
	bl	Func_08077008
	add	r1, sp, #84
	adds	r2, r1, #0
	movs	r4, #0
.L73:
	ldrb	r3, [r0, #0]
	adds	r4, #1
	strh	r3, [r2, #0]
	adds	r0, #1
	adds	r2, #2
	cmp	r4, #14
	bls.n	.L73
	ldr	r2, [sp, #24]
	add	r3, sp, #52
	str	r2, [sp, #4]
	str	r3, [sp, #8]
	adds	r2, r6, #0
	movs	r0, #0
	mov	r3, r8
	str	r5, [sp, #0]
	b.n	.L67
.L38:
	mov	r0, sl
	bl	Func_08007308
	subs	r2, r0, #1
	adds	r0, r2, #0
	bl	Func_08077008
	add	r1, sp, #84
	adds	r2, r1, #0
	movs	r4, #0
.L74:
	ldrb	r3, [r0, #0]
	adds	r4, #1
	strh	r3, [r2, #0]
	adds	r0, #1
	adds	r2, #2
	cmp	r4, #14
	bls.n	.L74
.L72:
	movs	r3, #0
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	add	r3, sp, #52
	str	r3, [sp, #8]
	adds	r2, r6, #0
	movs	r0, #0
	mov	r3, r8
.L67:
	bl	Func_08017e88
.L68:
	adds	r6, r0, #0
	b.n	.L5
.L57:
	mov	r0, sl
	bl	Func_08007308
	subs	r0, #1
	lsls	r0, r0, #1
	ldr	r1, [pc, #64]
	lsls	r3, r6, #1
	adds	r2, r0, #0
	adds	r6, #1
	adds	r2, #128
	ands	r6, r1
	mov	r5, r8
	strh	r2, [r3, r5]
	adds	r0, #129
	lsls	r3, r6, #1
	mov	r2, r8
	adds	r6, #1
	strh	r0, [r3, r2]
	ands	r6, r1
	b.n	.L5
.L54:
	ldr	r3, [pc, #28]
	ldr	r0, [pc, #32]
	lsls	r2, r6, #1
	mov	r5, r8
	adds	r6, #1
	strh	r3, [r2, r5]
	ands	r6, r0
	ldr	r3, [pc, #16]
	lsls	r2, r6, #1
	mov	r1, r8
	adds	r6, #1
	strh	r3, [r2, r1]
	ands	r6, r0
	b.n	.L5
	movs	r0, r0
	.4byte 0x0000008f
	.4byte 0x0000002d
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000741
	.4byte 0x00000182
	.4byte 0x00000333
	.4byte 0x0000099b
	.4byte 0x02000240
.L46:
	ldr	r2, [sp, #24]
	cmp	r2, #0
	beq.n	.L5
	ldr	r3, [sp, #52]
	cmp	r3, #0
	beq.n	.L75
	ldr	r3, [pc, #28]
	lsls	r2, r6, #1
	mov	r5, r8
	ldr	r0, [pc, #32]
	adds	r6, #1
	strh	r3, [r2, r5]
	ands	r6, r0
.L75:
	ldr	r3, [pc, #20]
	lsls	r2, r6, #1
	mov	r1, r8
	strh	r3, [r2, r1]
	ldr	r2, [pc, #16]
	adds	r6, #1
	ands	r6, r2
	b.n	.L5
	movs	r0, r0
	.4byte 0x00000065
	.4byte 0x00000073
	.4byte 0x000001ff
.L59:
	ldr	r2, [pc, #32]
	lsls	r3, r6, #1
	mov	r5, r8
	strh	r2, [r3, r5]
	ldr	r1, [pc, #32]
	ldr	r3, [sp, #52]
	adds	r6, #1
	ands	r6, r1
	cmp	r3, #0
	bne.n	.L5
	ldr	r3, [pc, #16]
	lsls	r2, r6, #1
	mov	r0, r8
	adds	r6, #1
	strh	r3, [r2, r0]
	ands	r6, r1
	b.n	.L5
	movs	r0, r0
	.4byte 0x00000027
	.4byte 0x00000073
	.4byte 0x000001ff
.L33:
	movs	r1, #1
	str	r1, [sp, #40]
.L36:
	lsls	r3, r6, #1
	mov	r2, r8
	strh	r7, [r3, r2]
	ldr	r3, [pc, #16]
	adds	r6, #1
	ands	r6, r3
	cmp	r7, #115
	beq.n	.L76
	cmp	r7, #83
	bne.n	.L77
.L76:
	movs	r3, #1
	b.n	.L78
	.4byte 0x000001ff
.L77:
	movs	r3, #0
.L78:
	str	r3, [sp, #52]
.L5:
	ldr	r5, [sp, #28]
	ldr	r0, [sp, #20]
	adds	r5, #1
	str	r5, [sp, #28]
	cmp	r0, #0
	beq.n	.L79
	ldr	r1, [pc, #72]
	cmp	r5, r1
	bhi.n	.L79
	b.n	.L80
.L79:
	ldr	r1, [pc, #64]
	lsls	r3, r6, #1
	mov	r2, r8
	adds	r6, #1
	strh	r7, [r3, r2]
	ands	r6, r1
	ldr	r3, [pc, #48]
	lsls	r2, r6, #1
	mov	r5, r8
	strh	r3, [r2, r5]
	adds	r3, r6, #1
	ands	r3, r1
	ldr	r0, [sp, #44]
	ldr	r1, [pc, #44]
	adds	r2, r0, r1
	strh	r3, [r2, #0]
	movs	r0, #50
	bl	Func_08002dd8
	ldr	r5, [pc, #36]
	ldr	r2, [sp, #44]
	add	r0, sp, #32
	ldrh	r0, [r0, #0]
	adds	r3, r2, r5
	strh	r0, [r3, #0]
.L1:
	mov	r1, fp
	cmp	r1, #0
	beq.n	.L81
	bl	Func_080198dc
	b.n	.L81
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x000001ff
	.4byte 0x000012b2
	.4byte 0x000012b4
.L81:
	ldr	r0, [sp, #32]
	add	sp, #132
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1

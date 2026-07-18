@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080bbb0c
	.thumb_func
Func_080bbb0c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #100
	str	r0, [sp, #80]
	movs	r0, #0
	str	r0, [sp, #60]
	ldr	r3, [pc, #796]
	movs	r5, #166
	ldr	r3, [r3, #0]
	lsls	r5, r5, #1
	str	r0, [sp, #52]
	str	r0, [sp, #44]
	str	r0, [sp, #40]
	str	r0, [sp, #28]
	str	r0, [sp, #20]
	adds	r0, r5, #0
	str	r3, [sp, #56]
	adds	r6, r1, #0
	bl	Func_08004938
	str	r0, [sp, #16]
	ldr	r1, [sp, #80]
	ldrb	r1, [r1, #0]
	ldr	r2, [sp, #80]
	str	r1, [sp, #68]
	ldr	r4, [sp, #80]
	adds	r2, #2
	ldrb	r3, [r2, r6]
	ldr	r4, [r4, #76]
	mov	sl, r3
	str	r4, [sp, #64]
	adds	r3, r6, #0
	adds	r3, #28
	ldrsb	r3, [r2, r3]
	ldr	r0, [sp, #80]
	ldr	r1, [sp, #80]
	ldr	r0, [r0, #80]
	str	r3, [sp, #48]
	adds	r3, r6, #0
	adds	r3, #44
	ldrsb	r3, [r1, r3]
	mov	r9, r0
	adds	r0, r4, #0
	str	r3, [sp, #32]
	mov	r8, r2
	bl	Func_08077080
	str	r0, [sp, #76]
	ldr	r0, [sp, #68]
	bl	Func_08077008
	str	r0, [sp, #72]
	mov	r0, sl
	bl	Func_08077008
	adds	r7, r0, #0
	ldr	r3, [pc, #700]
	adds	r2, r5, #0
	ldr	r0, [sp, #16]
	adds	r1, r7, #0
	bl	Func_080072f0
	ldr	r2, [sp, #76]
	ldrb	r3, [r2, #8]
	cmp	r3, #255
	beq.n	.L0
	adds	r3, r6, #0
	ldr	r4, [sp, #80]
	adds	r3, #16
	ldrsb	r3, [r4, r3]
	mov	fp, r3
	cmp	r3, #0
	bge.n	.L1
	mov	r5, fp
	negs	r5, r5
	mov	fp, r5
	b.n	.L1
.L0:
	movs	r0, #0
	mov	fp, r0
.L1:
	mov	r1, r9
	cmp	r1, #4
	beq.n	.L2
	lsls	r3, r1, #2
	adds	r4, r7, #0
	adds	r3, r3, r7
	adds	r4, #36
	movs	r2, #38
	ldrsh	r0, [r3, r2]
	movs	r5, #2
	ldrsh	r3, [r4, r5]
	movs	r1, #0
	cmp	r0, r3
	blt.n	.L3
	adds	r2, r4, #0
.L4:
	adds	r1, #1
	adds	r2, #4
	cmp	r1, #3
	bgt.n	.L3
	movs	r5, #2
	ldrsh	r3, [r2, r5]
	cmp	r0, r3
	bge.n	.L4
.L3:
	cmp	r1, #4
	bne.n	.L5
	movs	r1, #1
	negs	r1, r1
	str	r1, [sp, #20]
.L5:
	movs	r2, #2
	ldrsh	r3, [r4, r2]
	movs	r1, #0
	cmp	r0, r3
	bgt.n	.L6
	adds	r2, r7, #0
	adds	r2, #36
.L7:
	adds	r1, #1
	adds	r2, #4
	cmp	r1, #3
	bgt.n	.L6
	movs	r4, #2
	ldrsh	r3, [r2, r4]
	cmp	r0, r3
	ble.n	.L7
.L6:
	cmp	r1, #4
	bne.n	.L2
	movs	r5, #1
	str	r5, [sp, #20]
.L2:
	ldr	r0, [sp, #80]
	ldr	r2, [r0, #80]
	cmp	r2, #3
	bhi.n	.L8
	adds	r0, #72
	str	r0, [sp, #4]
	movs	r1, #0
	ldrsh	r3, [r0, r1]
	cmp	r3, #2
	beq.n	.L9
	lsls	r3, r2, #2
	ldr	r4, [sp, #72]
	adds	r3, #72
	ldrsh	r4, [r4, r3]
	str	r4, [sp, #12]
	b.n	.L10
.L8:
	ldr	r0, [sp, #80]
	adds	r0, #72
	str	r0, [sp, #4]
.L9:
	movs	r1, #100
	str	r1, [sp, #12]
.L10:
	ldr	r5, [sp, #4]
	movs	r4, #0
	ldrsh	r3, [r5, r4]
	cmp	r3, #5
	bne.n	.L11
	cmp	r2, #3
	bhi.n	.L11
	ldr	r0, [sp, #20]
	cmp	r0, #0
	ble.n	.L11
	lsls	r3, r2, #2
	adds	r3, #72
	adds	r3, r7, r3
	movs	r1, #2
	ldrsh	r5, [r3, r1]
	ldr	r2, [sp, #12]
	ldr	r3, [pc, #488]
	subs	r5, r2, r5
	adds	r5, #30
	muls	r5, r3
	bl	Func_080771a0
	ldr	r3, [pc, #480]
	ands	r0, r3
	cmp	r5, r0
	ble.n	.L11
	movs	r0, #13
	movs	r1, #5
	bl	Func_080bbabc
.L11:
	ldr	r4, [sp, #76]
	ldrb	r3, [r4, #1]
	movs	r5, #15
	ands	r5, r3
	str	r5, [sp, #24]
	adds	r3, r6, #0
	adds	r3, #56
	mov	r6, r8
	movs	r1, #1
	ldrsb	r0, [r6, r3]
	negs	r1, r1
	cmp	r0, r1
	bne.n	.L12
	ldr	r2, [pc, #444]
	ldrb	r3, [r4, #3]
	mov	r4, fp
	ldrb	r2, [r2, r4]
	ldr	r0, [sp, #68]
	str	r2, [sp, #0]
	mov	r1, sl
	mov	r2, r9
	bl	Func_08077178
.L12:
	str	r0, [sp, #36]
	ldr	r5, [sp, #76]
	ldrb	r3, [r5, #3]
	movs	r6, #128
	adds	r3, #206
	lsls	r3, r3, #24
	lsls	r6, r6, #17
	cmp	r3, r6
	bls.n	.L13
	b.n	.L14
.L13:
	ldr	r0, [sp, #72]
	movs	r1, #148
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldrb	r5, [r3, #0]
	bl	Func_080b7514
	ldr	r2, [sp, #76]
	ldrb	r3, [r2, #3]
	mov	r8, r0
	cmp	r3, #51
	bne.n	.L15
	ldr	r3, [sp, #56]
	ldr	r0, [r3, #0]
	bl	Func_080c1fa8
	adds	r5, r0, #0
.L15:
	ldr	r4, [sp, #36]
	cmp	r4, #0
	beq.n	.L16
	adds	r0, r5, #0
	bl	Func_080b6cdc
	cmp	r0, #0
	beq.n	.L16
	mov	r6, r8
	cmp	r6, #0
	blt.n	.L16
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_080c1df4
	movs	r3, #128
	adds	r6, r0, #0
	lsls	r3, r3, #8
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L17
	adds	r0, r5, #0
	bl	Func_080c1f50
.L17:
	ldr	r2, [pc, #324]
	adds	r1, r5, #0
	ands	r2, r6
	mov	r0, r8
	bl	Func_08077140
	ldr	r1, [sp, #56]
	movs	r2, #100
	adds	r1, #2
	ldrsh	r3, [r1, r2]
	movs	r5, #0
	mov	lr, r5
	mov	ip, r1
	movs	r0, #100
	movs	r4, #0
	cmp	r3, #254
	bne.n	.L18
	mov	r3, r8
	strh	r3, [r1, r2]
	b.n	.L19
.L20:
	mov	r3, lr
	mov	r4, r8
	adds	r3, #102
	strh	r4, [r1, r0]
	strh	r2, [r1, r3]
	b.n	.L19
.L18:
	mov	r6, ip
	ldrsh	r2, [r0, r6]
	cmp	r2, #255
	beq.n	.L20
	adds	r5, #1
	adds	r0, #2
	adds	r4, #2
	cmp	r5, #5
	bgt.n	.L19
	ldrsh	r3, [r0, r1]
	mov	lr, r4
	cmp	r3, #254
	bne.n	.L18
	mov	r2, r8
	strh	r2, [r0, r1]
.L19:
	bl	Func_080b7548
	mov	r0, r8
	bl	Func_080b7dd0
	ldr	r2, [r0, #12]
	cmp	r2, #0
	bge.n	.L21
	ldr	r3, [pc, #220]
	adds	r2, r2, r3
.L21:
	ldr	r3, [r0, #16]
	asrs	r2, r2, #16
	cmp	r3, #0
	bge.n	.L22
	ldr	r4, [pc, #208]
	adds	r3, r3, r4
.L22:
	asrs	r3, r3, #16
	mov	r1, r8
	bl	Func_080b6f44
	bl	Func_080b6c90
	add	r5, sp, #84
	adds	r0, r5, #0
	bl	Func_080b6ae0
	cmp	r0, #0
	ble.n	.L23
	adds	r6, r5, #0
	adds	r5, r0, #0
.L24:
	ldrh	r0, [r6, #0]
	subs	r5, #1
	adds	r6, #2
	bl	Func_080b8000
	cmp	r5, #0
	bne.n	.L24
.L23:
	movs	r0, #0
	mov	r1, r8
	bl	Func_080bbabc
	ldr	r6, [pc, #164]
	ldr	r5, [sp, #64]
	cmp	r5, r6
	beq.n	.L25
	ldr	r1, [pc, #160]
	b.n	.L26
.L25:
	ldr	r1, [pc, #160]
	b.n	.L26
.L16:
	ldr	r0, [sp, #64]
	ldr	r1, [pc, #148]
	cmp	r0, r1
	bne.n	.L27
	ldr	r1, [pc, #152]
.L26:
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L14
.L27:
	ldr	r1, [pc, #148]
	movs	r0, #4
	bl	Func_080bbabc
.L14:
	ldr	r2, [sp, #36]
	cmp	r2, #0
	beq.n	.L28
	ldr	r3, [sp, #76]
	ldrb	r2, [r3, #3]
	adds	r3, r2, #0
	cmp	r3, #53
	bne.n	.L29
	movs	r4, #0
	str	r4, [sp, #36]
	movs	r3, #187
	ldr	r5, [sp, #56]
	lsls	r3, r3, #2
	ldrsh	r3, [r5, r3]
	movs	r2, #0
	cmp	r3, sl
	bne.n	.L30
	movs	r0, #1
	str	r0, [sp, #36]
	b.n	.L28
.L30:
	adds	r2, #1
	cmp	r2, #19
	bhi.n	.L28
	movs	r1, #187
	lsls	r3, r2, #4
	lsls	r1, r1, #2
	ldr	r4, [sp, #56]
	adds	r3, r3, r1
	ldrsh	r3, [r4, r3]
	cmp	r3, sl
	bne.n	.L30
	movs	r6, #1
	str	r6, [sp, #36]
	b.n	.L28
.L29:
	adds	r3, r2, #0
	cmp	r3, #35
	bne.n	.L31
	movs	r0, #1
	str	r0, [sp, #52]
	b.n	.L28
.L31:
	cmp	r3, #34
	bne.n	.L32
	movs	r1, #1
	str	r1, [sp, #40]
	b.n	.L28
.L32:
	cmp	r3, #27
	bne.n	.L33
	movs	r2, #1
	str	r2, [sp, #28]
	b.n	.L28
	movs	r0, r0
	.4byte 0x03001e74
	.4byte 0x03001388
	.4byte 0x0000028f
	.4byte 0x0000ffff
	.4byte 0x080c2ab8
	.4byte 0x00007fff
	.4byte 0x000001f7
	.4byte 0x000008f5
	.4byte 0x000008f3
	.4byte 0x000008f4
	.4byte 0x000008f6
.L33:
	cmp	r3, #55
	bne.n	.L34
	ldr	r5, [sp, #72]
	movs	r4, #56
	ldrsh	r3, [r5, r4]
	cmp	r3, #0
	beq.n	.L28
	movs	r0, #12
	ldr	r1, [sp, #68]
	bl	Func_080bbabc
	b.n	.L28
.L34:
	cmp	r3, #32
	bne.n	.L28
	movs	r6, #58
	ldrsh	r3, [r7, r6]
	cmp	r3, #0
	beq.n	.L35
	movs	r0, #10
	str	r0, [sp, #24]
	b.n	.L28
.L35:
	movs	r1, #0
	str	r1, [sp, #36]
.L28:
	ldr	r2, [sp, #28]
	cmp	r2, #0
	beq.n	.L36
	b.n	.L37
.L36:
	movs	r4, #56
	ldrsh	r3, [r7, r4]
	cmp	r3, #0
	bne.n	.L38
	ldr	r5, [sp, #76]
	ldrb	r0, [r5, #3]
	bl	Func_080772b8
	cmp	r0, #0
	bne.n	.L38
	b.n	.L37
.L38:
	ldr	r3, [sp, #24]
	adds	r3, #1
	cmp	r3, #12
	bls.n	.L39
	b.n	.L37
.L39:
	ldr	r2, [pc, #792]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080bc20c
	.4byte 0x080bc666
	.4byte 0x080bc150
	.4byte 0x080bc51e
	.4byte 0x080bbf00
	.4byte 0x080bbf00
	.4byte 0x080bc2ba
	.4byte 0x080bc2ba
	.4byte 0x080bc666
	.4byte 0x080bc2ba
	.4byte 0x080bc666
	.4byte 0x080bc084
	.4byte 0x080bc49a
	ldrh	r1, [r7, #62]
	ldr	r2, [sp, #52]
	movs	r0, #56
	ldrsh	r6, [r7, r0]
	mov	fp, r1
	cmp	r2, #0
	beq.n	.L40
	lsrs	r1, r1, #1
	mov	fp, r1
.L40:
	movs	r3, #1
	mov	r8, r3
.L56:
	mov	r4, r9
	cmp	r4, #4
	beq.n	.L41
	lsls	r3, r4, #2
	adds	r3, #72
	adds	r3, r7, r3
	movs	r5, #2
	ldrsh	r3, [r3, r5]
	ldr	r0, [sp, #12]
	subs	r3, r0, r3
	str	r3, [sp, #60]
.L41:
	mov	r1, r8
	cmp	r1, #0
	bne.n	.L42
	movs	r2, #0
	str	r2, [sp, #60]
.L42:
	ldr	r3, [sp, #76]
	ldr	r4, [sp, #24]
	ldrh	r5, [r3, #10]
	cmp	r4, #4
	bne.n	.L43
	ldr	r1, [sp, #72]
	movs	r2, #0
	ldrh	r0, [r1, #60]
	ldr	r3, [sp, #60]
	mov	r1, fp
	bl	Func_08077180
	movs	r1, #10
	muls	r0, r5
	bl	Func_080022ec
	b.n	.L44
.L43:
	ldr	r2, [sp, #72]
	mov	r1, fp
	ldrh	r0, [r2, #60]
	ldr	r3, [sp, #60]
	adds	r2, r5, #0
	bl	Func_08077180
.L44:
	adds	r5, r0, #0
	ldr	r3, [sp, #48]
	ldr	r4, [sp, #32]
	muls	r5, r3
	cmp	r4, #0
	beq.n	.L45
	cmp	r4, #1
	bne.n	.L46
	lsls	r3, r5, #2
	adds	r0, r3, r5
	cmp	r0, #0
	bge.n	.L47
	adds	r0, #3
.L47:
	asrs	r5, r0, #2
	b.n	.L48
.L46:
	lsls	r3, r5, #1
	adds	r3, r3, r5
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r5, r3, #1
.L48:
	ldrb	r0, [r7, #15]
	movs	r1, #5
	bl	Func_080022f4
	lsls	r0, r0, #24
	lsrs	r0, r0, #24
	adds	r0, r5, r0
	adds	r5, r0, #6
	mov	r0, r8
	cmp	r0, #0
	bne.n	.L45
	movs	r1, #0
	movs	r0, #6
	bl	Func_080bbabc
	mov	r2, sl
	ldr	r1, [pc, #560]
	cmp	r2, #7
	bhi.n	.L49
	adds	r1, #1
.L49:
	movs	r0, #5
	bl	Func_080bbabc
.L45:
	bl	Func_080771a0
	movs	r3, #3
	ldr	r4, [pc, #544]
	ands	r3, r0
	adds	r5, r5, r3
	adds	r3, r7, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L50
	cmp	r3, #1
	bne.n	.L51
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r5, r3, #1
	b.n	.L50
.L51:
	adds	r0, r5, #0
	movs	r1, #10
	bl	Func_080022ec
	adds	r5, r0, #0
.L50:
	cmp	r5, #0
	bgt.n	.L52
	movs	r5, #1
.L52:
	ldr	r0, [sp, #40]
	cmp	r0, #0
	beq.n	.L53
	subs	r3, r6, #1
	cmp	r5, r3
	bge.n	.L53
	adds	r5, r3, #0
	cmp	r5, #0
	bgt.n	.L53
	movs	r5, #1
.L53:
	movs	r0, #183
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L54
	ldr	r2, [sp, #4]
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	cmp	r3, #5
	bne.n	.L54
	cmp	r6, r5
	bgt.n	.L54
	subs	r5, r6, #1
.L54:
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	cmp	r4, #1
	bgt.n	.L55
	b.n	.L56
.L55:
	mov	r1, sl
	movs	r0, #8
	bl	Func_080bbabc
	mov	r1, sl
	movs	r0, #0
	bl	Func_080bbabc
	subs	r6, r6, r5
	adds	r1, r5, #0
	movs	r0, #1
	mov	r5, sl
	bl	Func_080bbabc
	cmp	r5, #7
	bhi.n	.L57
	ldr	r3, [pc, #408]
	ldr	r0, [sp, #20]
	adds	r1, r0, r3
	b.n	.L58
.L57:
	ldr	r3, [pc, #404]
	ldr	r2, [sp, #20]
	adds	r1, r2, r3
.L58:
	movs	r0, #4
	bl	Func_080bbabc
	cmp	r6, #0
	ble.n	.L59
	b.n	.L60
.L59:
	mov	r1, sl
	movs	r0, #9
	bl	Func_080bbabc
	movs	r0, #0
	mov	r1, sl
	bl	Func_080bbabc
	mov	r3, sl
	movs	r6, #0
	cmp	r3, #7
	bls.n	.L61
	b.n	.L62
.L61:
	ldr	r1, [pc, #364]
	b.n	.L63
	ldr	r5, [sp, #76]
	ldrh	r3, [r5, #10]
	cmp	r3, #0
	bne.n	.L64
	b.n	.L37
.L64:
	mov	r1, r9
	movs	r0, #58
	ldrsh	r6, [r7, r0]
	cmp	r1, #4
	beq.n	.L65
	lsls	r3, r1, #2
	adds	r3, #72
	adds	r3, r7, r3
	movs	r2, #2
	ldrsh	r3, [r3, r2]
	ldr	r4, [sp, #12]
	subs	r3, r4, r3
	str	r3, [sp, #60]
.L65:
	ldr	r0, [sp, #76]
	ldrh	r5, [r0, #10]
	movs	r2, #128
	ldr	r1, [sp, #60]
	lsls	r2, r2, #1
	adds	r0, r5, #0
	bl	Func_08077188
	ldr	r2, [pc, #312]
	mov	r1, fp
	lsls	r3, r1, #2
	ldr	r3, [r2, r3]
	adds	r5, r0, #0
	adds	r0, r3, #0
	muls	r0, r5
	movs	r1, #100
	bl	Func_080022ec
	ldr	r4, [pc, #276]
	adds	r3, r7, r4
	ldr	r2, [sp, #48]
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r5, r0, #0
	muls	r5, r2
	cmp	r3, #0
	beq.n	.L66
	cmp	r3, #1
	bne.n	.L67
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r5, r3, #1
	b.n	.L66
.L67:
	adds	r0, r5, #0
	movs	r1, #10
	bl	Func_080022ec
	adds	r5, r0, #0
.L66:
	ldr	r0, [sp, #76]
	ldrb	r3, [r0, #3]
	cmp	r3, #32
	bne.n	.L68
	cmp	r5, r6
	ble.n	.L68
	adds	r5, r6, #0
.L68:
	mov	r1, sl
	movs	r0, #8
	bl	Func_080bbabc
	adds	r1, r5, #0
	movs	r0, #1
	bl	Func_080bbabc
	mov	r1, sl
	movs	r0, #0
	bl	Func_080bbabc
	mov	r1, sl
	cmp	r1, #7
	bhi.n	.L69
	ldr	r1, [pc, #212]
	b.n	.L70
.L69:
	ldr	r1, [pc, #212]
.L70:
	movs	r0, #4
	subs	r6, r6, r5
	bl	Func_080bbabc
	cmp	r6, #0
	bgt.n	.L71
	movs	r6, #0
.L71:
	movs	r0, #11
	mov	r1, sl
	bl	Func_080bbabc
	movs	r2, #58
	ldrsh	r3, [r7, r2]
	subs	r3, r3, r6
	str	r3, [sp, #44]
	mov	r0, sl
	strh	r6, [r7, #58]
	bl	Func_08077128
	b.n	.L37
	ldr	r4, [sp, #76]
	ldrh	r3, [r4, #10]
	cmp	r3, #0
	bne.n	.L72
	b.n	.L37
.L72:
	mov	r0, r9
	movs	r5, #56
	ldrsh	r6, [r7, r5]
	ldr	r1, [sp, #12]
	adds	r5, r3, #0
	cmp	r0, #4
	bne.n	.L73
	movs	r1, #100
.L73:
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r0, r5, #0
	bl	Func_08077190
	ldr	r2, [pc, #136]
	mov	r1, fp
	lsls	r3, r1, #2
	ldr	r3, [r2, r3]
	adds	r5, r0, #0
	adds	r0, r3, #0
	muls	r0, r5
	movs	r1, #100
	bl	Func_080022ec
	ldr	r2, [sp, #48]
	adds	r5, r0, #0
	muls	r5, r2
	bl	Func_080771a0
	movs	r3, #3
	ands	r3, r0
	adds	r5, r5, r3
	movs	r4, #52
	ldrsh	r3, [r7, r4]
	adds	r6, r6, r5
	cmp	r6, r3
	ble.n	.L74
	adds	r6, r3, #0
	movs	r5, #56
	ldrsh	r3, [r7, r5]
	subs	r5, r6, r3
.L74:
	movs	r0, #0
	mov	r1, sl
	bl	Func_080bbabc
	movs	r0, #52
	ldrsh	r3, [r7, r0]
	cmp	r6, r3
	bne.n	.L75
	ldr	r1, [pc, #72]
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L76
.L75:
	adds	r1, r5, #0
	movs	r0, #1
	bl	Func_080bbabc
	ldr	r1, [pc, #56]
	movs	r0, #4
	bl	Func_080bbabc
.L76:
	movs	r1, #56
	ldrsh	r3, [r7, r1]
	b.n	.L77
	movs	r0, r0
	.4byte 0x080bbecc
	.4byte 0x00000822
	.4byte 0x0000012b
	.4byte 0x00000834
	.4byte 0x00000831
	.4byte 0x00000825
	.4byte 0x080c2ac0
	.4byte 0x0000082a
	.4byte 0x00000829
	.4byte 0x080c2ad8
	.4byte 0x00000820
	.4byte 0x0000081d
	ldr	r2, [sp, #76]
	ldrh	r3, [r2, #10]
	cmp	r3, #0
	bne.n	.L78
	b.n	.L37
.L78:
	mov	r4, r9
	movs	r3, #58
	ldrsh	r6, [r7, r3]
	cmp	r4, #4
	beq.n	.L79
	lsls	r3, r4, #2
	adds	r3, #72
	adds	r3, r7, r3
	movs	r5, #2
	ldrsh	r3, [r3, r5]
	ldr	r0, [sp, #12]
	subs	r3, r0, r3
	str	r3, [sp, #60]
.L79:
	ldr	r1, [sp, #76]
	ldrh	r5, [r1, #10]
	movs	r2, #128
	ldr	r1, [sp, #60]
	lsls	r2, r2, #1
	adds	r0, r5, #0
	bl	Func_08077188
	ldr	r2, [pc, #844]
	mov	r4, fp
	lsls	r3, r4, #2
	ldr	r3, [r2, r3]
	adds	r5, r0, #0
	adds	r0, r3, #0
	muls	r0, r5
	movs	r1, #100
	bl	Func_080022ec
	ldr	r1, [pc, #828]
	adds	r3, r7, r1
	adds	r5, r0, #0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	ldr	r0, [sp, #48]
	muls	r5, r0
	cmp	r3, #0
	beq.n	.L80
	cmp	r3, #1
	bne.n	.L81
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r5, r3, #1
	b.n	.L80
.L81:
	adds	r0, r5, #0
	movs	r1, #10
	bl	Func_080022ec
	adds	r5, r0, #0
.L80:
	mov	r1, sl
	movs	r0, #8
	bl	Func_080bbabc
	adds	r1, r5, #0
	movs	r0, #1
	bl	Func_080bbabc
	movs	r0, #0
	mov	r1, sl
	bl	Func_080bbabc
	mov	r2, sl
	cmp	r2, #7
	bhi.n	.L82
	ldr	r1, [pc, #760]
	b.n	.L83
.L82:
	ldr	r1, [pc, #760]
.L83:
	movs	r0, #4
	subs	r6, r6, r5
	bl	Func_080bbabc
	cmp	r6, #0
	bgt.n	.L84
	movs	r6, #0
.L84:
	movs	r0, #11
	mov	r1, sl
	bl	Func_080bbabc
	b.n	.L85
	ldr	r4, [sp, #76]
	ldrh	r3, [r4, #10]
	cmp	r3, #0
	bne.n	.L86
	b.n	.L37
.L86:
	movs	r0, #1
	movs	r5, #56
	ldrsh	r6, [r7, r5]
	mov	r8, r0
.L101:
	mov	r1, r9
	cmp	r1, #4
	beq.n	.L87
	lsls	r3, r1, #2
	adds	r3, #72
	adds	r3, r7, r3
	movs	r2, #2
	ldrsh	r3, [r3, r2]
	ldr	r4, [sp, #12]
	subs	r3, r4, r3
	str	r3, [sp, #60]
.L87:
	mov	r5, r8
	cmp	r5, #0
	bne.n	.L88
	movs	r0, #0
	str	r0, [sp, #60]
.L88:
	ldr	r4, [sp, #4]
	ldr	r1, [sp, #76]
	movs	r2, #0
	ldrsh	r3, [r4, r2]
	ldrh	r5, [r1, #10]
	cmp	r3, #6
	bne.n	.L89
	ldr	r0, [sp, #64]
	ldr	r1, [pc, #672]
	adds	r3, r0, r1
	cmp	r3, #21
	bhi.n	.L90
	ldr	r2, [pc, #668]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080bc364
	.4byte 0x080bc36a
	.4byte 0x080bc370
	.4byte 0x080bc376
	.4byte 0x080bc37a
	.4byte 0x080bc37a
	.4byte 0x080bc364
	.4byte 0x080bc36a
	.4byte 0x080bc370
	.4byte 0x080bc376
	.4byte 0x080bc37a
	.4byte 0x080bc37a
	.4byte 0x080bc364
	.4byte 0x080bc36a
	.4byte 0x080bc370
	.4byte 0x080bc376
	.4byte 0x080bc37a
	.4byte 0x080bc37a
	.4byte 0x080bc364
	.4byte 0x080bc36a
	.4byte 0x080bc370
	.4byte 0x080bc376
	movs	r2, #3
	str	r2, [sp, #8]
	b.n	.L90
	movs	r3, #6
	str	r3, [sp, #8]
	b.n	.L90
	movs	r4, #9
	str	r4, [sp, #8]
	b.n	.L90
	movs	r0, #12
	str	r0, [sp, #8]
.L90:
	movs	r1, #52
	ldrsh	r3, [r7, r1]
	ldr	r2, [sp, #8]
	movs	r1, #100
	adds	r0, r2, #0
	muls	r0, r3
	bl	Func_080022ec
	adds	r5, r5, r0
.L89:
	movs	r2, #128
	adds	r0, r5, #0
	lsls	r2, r2, #1
	ldr	r1, [sp, #60]
	bl	Func_08077188
	ldr	r3, [sp, #48]
	adds	r5, r0, #0
	muls	r5, r3
	ldr	r3, [sp, #24]
	cmp	r3, #6
	beq.n	.L91
	cmp	r3, #6
	bgt.n	.L92
	cmp	r3, #5
	beq.n	.L93
	b.n	.L94
.L92:
	cmp	r3, #8
	beq.n	.L95
	b.n	.L94
.L93:
	ldr	r2, [pc, #496]
	mov	r4, fp
	lsls	r3, r4, #2
	b.n	.L96
.L95:
	ldr	r2, [pc, #492]
	mov	r0, fp
	lsls	r3, r0, #2
	b.n	.L96
.L91:
	ldr	r2, [pc, #488]
	mov	r1, fp
	lsls	r3, r1, #2
.L96:
	ldr	r3, [r2, r3]
	movs	r1, #100
	adds	r0, r3, #0
	muls	r0, r5
	bl	Func_080022ec
	adds	r5, r0, #0
.L94:
	bl	Func_080771a0
	movs	r3, #3
	ldr	r2, [pc, #436]
	ands	r3, r0
	adds	r5, r5, r3
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L97
	cmp	r3, #1
	bne.n	.L98
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r5, r3, #1
	b.n	.L97
.L98:
	adds	r0, r5, #0
	movs	r1, #10
	bl	Func_080022ec
	adds	r5, r0, #0
.L97:
	movs	r0, #183
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L99
	ldr	r0, [sp, #4]
	movs	r4, #0
	ldrsh	r3, [r0, r4]
	cmp	r3, #6
	bne.n	.L99
	cmp	r6, r5
	ble.n	.L99
	adds	r5, r6, #0
.L99:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #1
	bgt.n	.L100
	b.n	.L101
.L100:
	mov	r1, sl
	movs	r0, #8
	bl	Func_080bbabc
	adds	r1, r5, #0
	movs	r0, #1
	bl	Func_080bbabc
	movs	r0, #0
	mov	r1, sl
	bl	Func_080bbabc
	mov	r3, sl
	cmp	r3, #7
	bhi.n	.L102
	ldr	r3, [pc, #356]
	ldr	r4, [sp, #20]
	adds	r1, r4, r3
	b.n	.L103
.L102:
	ldr	r3, [pc, #352]
	ldr	r0, [sp, #20]
	adds	r1, r0, r3
.L103:
	movs	r0, #4
	subs	r6, r6, r5
	bl	Func_080bbabc
	cmp	r6, #0
	bgt.n	.L104
	mov	r1, sl
	movs	r0, #9
	bl	Func_080bbabc
	mov	r1, sl
	movs	r0, #0
	bl	Func_080bbabc
	mov	r1, sl
	movs	r6, #0
	cmp	r1, #7
	bhi.n	.L105
	ldr	r1, [pc, #316]
	b.n	.L106
.L105:
	ldr	r1, [pc, #316]
.L106:
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L107
.L104:
	movs	r0, #11
	mov	r1, sl
	bl	Func_080bbabc
.L107:
	movs	r2, #56
	ldrsh	r3, [r7, r2]
	b.n	.L77
	ldr	r4, [sp, #76]
	ldrh	r3, [r4, #10]
	cmp	r3, #0
	bne.n	.L108
	b.n	.L37
.L108:
	mov	r0, r9
	movs	r5, #58
	ldrsh	r6, [r7, r5]
	ldr	r1, [sp, #12]
	adds	r5, r3, #0
	cmp	r0, #4
	bne.n	.L109
	movs	r1, #100
.L109:
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r0, r5, #0
	bl	Func_08077190
	ldr	r2, [pc, #260]
	mov	r1, fp
	lsls	r3, r1, #2
	ldr	r3, [r2, r3]
	adds	r5, r0, #0
	adds	r0, r3, #0
	muls	r0, r5
	movs	r1, #100
	bl	Func_080022ec
	ldr	r2, [sp, #48]
	adds	r5, r0, #0
	muls	r5, r2
	movs	r4, #54
	ldrsh	r3, [r7, r4]
	adds	r6, r6, r5
	cmp	r6, r3
	ble.n	.L110
	adds	r6, r3, #0
	movs	r5, #58
	ldrsh	r3, [r7, r5]
	subs	r5, r6, r3
.L110:
	movs	r0, #0
	mov	r1, sl
	bl	Func_080bbabc
	movs	r0, #54
	ldrsh	r3, [r7, r0]
	cmp	r6, r3
	bne.n	.L111
	ldr	r1, [pc, #204]
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L85
.L111:
	adds	r1, r5, #0
	movs	r0, #1
	bl	Func_080bbabc
	ldr	r1, [pc, #188]
	movs	r0, #4
	bl	Func_080bbabc
.L85:
	strh	r6, [r7, #58]
	mov	r0, sl
	bl	Func_08077128
	b.n	.L37
	ldr	r1, [sp, #36]
	cmp	r1, #0
	bne.n	.L112
	b.n	.L113
.L112:
	ldr	r2, [sp, #76]
	ldrh	r3, [r2, #10]
	cmp	r3, #0
	bne.n	.L114
	b.n	.L37
.L114:
	mov	r4, r9
	movs	r3, #56
	ldrsh	r6, [r7, r3]
	cmp	r4, #4
	beq.n	.L115
	lsls	r3, r4, #2
	adds	r3, #72
	adds	r3, r7, r3
	movs	r5, #2
	ldrsh	r3, [r3, r5]
	ldr	r0, [sp, #12]
	subs	r3, r0, r3
	str	r3, [sp, #60]
.L115:
	ldr	r1, [sp, #76]
	ldrh	r5, [r1, #10]
	movs	r2, #128
	ldr	r1, [sp, #60]
	lsls	r2, r2, #1
	adds	r0, r5, #0
	bl	Func_08077188
	ldr	r2, [sp, #48]
	adds	r5, r0, #0
	muls	r5, r2
	mov	r4, fp
	ldr	r2, [pc, #108]
	lsls	r3, r4, #2
	ldr	r3, [r2, r3]
	movs	r1, #100
	adds	r0, r3, #0
	muls	r0, r5
	bl	Func_080022ec
	adds	r5, r0, #0
	ldr	r0, [pc, #28]
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L116
	cmp	r3, #1
	bne.n	.L117
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r5, r3, #1
	b.n	.L116
	movs	r0, r0
	.4byte 0x080c2af0
	.4byte 0x0000012b
	.4byte 0x00000827
	.4byte 0x00000826
	.4byte 0xfffffe84
	.4byte 0x080bc30c
	.4byte 0x080c2b08
	.4byte 0x080c2b20
	.4byte 0x080c2b38
	.4byte 0x00000834
	.4byte 0x00000831
	.4byte 0x00000825
	.4byte 0x00000824
	.4byte 0x080c2b50
	.4byte 0x00000821
	.4byte 0x0000081e
	.4byte 0x080c2b68
.L117:
	adds	r0, r5, #0
	movs	r1, #10
	bl	Func_080022ec
	adds	r5, r0, #0
.L116:
	mov	r1, sl
	movs	r0, #8
	bl	Func_080bbabc
	adds	r1, r5, #0
	movs	r0, #1
	bl	Func_080bbabc
	mov	r1, sl
	movs	r0, #0
	bl	Func_080bbabc
	mov	r1, sl
	cmp	r1, #7
	bhi.n	.L118
	ldr	r1, [pc, #916]
	b.n	.L119
.L118:
	ldr	r1, [pc, #916]
.L119:
	movs	r0, #4
	subs	r6, r6, r5
	bl	Func_080bbabc
	cmp	r6, #0
	bgt.n	.L60
	mov	r1, sl
	movs	r0, #9
	bl	Func_080bbabc
	movs	r0, #0
	mov	r1, sl
	bl	Func_080bbabc
	mov	r2, sl
	movs	r6, #0
	cmp	r2, #7
	bhi.n	.L62
	ldr	r1, [pc, #884]
	b.n	.L63
.L62:
	ldr	r1, [pc, #884]
.L63:
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L120
.L60:
	movs	r0, #11
	mov	r1, sl
	bl	Func_080bbabc
.L120:
	movs	r4, #56
	ldrsh	r3, [r7, r4]
.L77:
	subs	r3, r3, r6
	str	r3, [sp, #44]
	mov	r0, sl
	strh	r6, [r7, #56]
	bl	Func_08077128
	b.n	.L37
.L113:
	mov	r1, sl
	movs	r0, #11
	bl	Func_080bbabc
	mov	r1, sl
	movs	r0, #0
	bl	Func_080bbabc
	ldr	r1, [pc, #836]
	movs	r0, #4
	bl	Func_080bbabc
.L37:
	movs	r0, #0
	mov	r1, sl
	bl	Func_080bbabc
	ldr	r5, [sp, #76]
	ldrb	r0, [r5, #3]
	bl	Func_080772b8
	cmp	r0, #0
	bne.n	.L121
	movs	r6, #56
	ldrsh	r3, [r7, r6]
	cmp	r3, #0
	bne.n	.L121
	ldrb	r0, [r5, #3]
	bl	Func_080bbae8
	cmp	r0, #0
	bne.n	.L121
	bl	.L122
.L121:
	ldr	r0, [sp, #36]
	cmp	r0, #0
	bne.n	.L123
	bl	.L122
.L123:
	ldr	r1, [sp, #76]
	ldrb	r3, [r1, #3]
	subs	r3, #3
	cmp	r3, #66
	bls.n	.L124
	bl	.L122
.L124:
	ldr	r2, [pc, #764]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080bcd38
	.4byte 0x080bc884
	.4byte 0x080bccd6
	.4byte 0x080bcb68
	.4byte 0x080bcb22
	.4byte 0x080bcad8
	.4byte 0x080bca8e
	.4byte 0x080bcc8c
	.4byte 0x080bcc42
	.4byte 0x080bcbfc
	.4byte 0x080bcbb2
	.4byte 0x080bce84
	.4byte 0x080bce30
	.4byte 0x080bcdde
	.4byte 0x080bcd8c
	.4byte 0x080bced8
	.4byte 0x080bcef4
	.4byte 0x080bcf10
	.4byte 0x080bcf20
	.4byte 0x080bcf32
	.4byte 0x080bcf42
	.4byte 0x080bcf50
	.4byte 0x080bcf5c
	.4byte 0x080bd1fa
	.4byte 0x080bcfa4
	.4byte 0x080bd20a
	.4byte 0x080bcfe0
	.4byte 0x080bcff4
	.4byte 0x080bd006
	.4byte 0x080bd068
	.4byte 0x080bd180
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd2b6
	.4byte 0x080bd282
	.4byte 0x080bd29c
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd274
	.4byte 0x080bd24e
	.4byte 0x080bd2c0
	.4byte 0x080bccf4
	.4byte 0x080bcd16
	.4byte 0x080bca5c
	.4byte 0x080bca2a
	.4byte 0x080bd006
	.4byte 0x080bc926
	.4byte 0x080bc926
	.4byte 0x080bc9dc
	.4byte 0x080bc7bc
	.4byte 0x080bd2c0
	.4byte 0x080bd23e
	.4byte 0x080bcf7e
	.4byte 0x080bd2c0
	.4byte 0x080bd128
	movs	r3, #156
	lsls	r3, r3, #1
	adds	r2, r7, r3
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L125
	movs	r3, #0
	ldr	r1, [pc, #480]
	strb	r3, [r2, #0]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r0, #0
	mov	r1, sl
	bl	Func_080bbabc
.L125:
	ldr	r4, [pc, #464]
	adds	r2, r7, r4
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L126
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r1, #0
	movs	r0, #7
	bl	Func_080bbabc
	mov	r1, sl
	movs	r0, #0
	bl	Func_080bbabc
	ldr	r1, [pc, #440]
	movs	r0, #4
	bl	Func_080bbabc
.L126:
	movs	r5, #158
	lsls	r5, r5, #1
	adds	r3, r7, r5
	movs	r6, #0
	ldr	r0, [pc, #428]
	strb	r6, [r3, #0]
	adds	r2, r7, r0
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L127
	ldr	r1, [pc, #420]
	strb	r6, [r2, #0]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r0, #0
	mov	r1, sl
	bl	Func_080bbabc
.L127:
	ldr	r1, [pc, #404]
	adds	r2, r7, r1
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L128
	ldr	r1, [pc, #400]
	strb	r6, [r2, #0]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r0, #0
	mov	r1, sl
	bl	Func_080bbabc
.L128:
	movs	r3, #160
	lsls	r3, r3, #1
	adds	r2, r7, r3
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L129
	ldr	r1, [pc, #372]
	strb	r6, [r2, #0]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r0, #0
	mov	r1, sl
	bl	Func_080bbabc
.L129:
	ldr	r4, [pc, #360]
	adds	r5, r7, r4
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	beq.n	.L130
	ldr	r1, [pc, #352]
	movs	r0, #4
	bl	Func_080bbabc
	strb	r6, [r5, #0]
.L130:
	movs	r0, #7
	movs	r1, #0
	bl	Func_080bbabc
	bl	.L122
	movs	r5, #156
	lsls	r5, r5, #1
	adds	r2, r7, r5
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L131
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r1, #0
	movs	r0, #7
	bl	Func_080bbabc
	mov	r1, sl
	movs	r0, #0
	bl	Func_080bbabc
	ldr	r1, [pc, #260]
	movs	r0, #4
	bl	Func_080bbabc
.L131:
	ldr	r6, [pc, #256]
	adds	r2, r7, r6
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L132
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r1, #0
	movs	r0, #7
	bl	Func_080bbabc
	mov	r1, sl
	movs	r0, #0
	bl	Func_080bbabc
	ldr	r1, [pc, #232]
	movs	r0, #4
	bl	Func_080bbabc
.L132:
	movs	r0, #158
	lsls	r0, r0, #1
	adds	r3, r7, r0
	movs	r5, #0
	ldr	r1, [pc, #220]
	strb	r5, [r3, #0]
	adds	r2, r7, r1
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L133
	strb	r5, [r2, #0]
	movs	r1, #0
	movs	r0, #7
	bl	Func_080bbabc
	mov	r1, sl
	movs	r0, #0
	bl	Func_080bbabc
	ldr	r1, [pc, #192]
	movs	r0, #4
	bl	Func_080bbabc
.L133:
	ldr	r3, [pc, #188]
	adds	r2, r7, r3
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L134
	bl	.L122
.L134:
	strb	r5, [r2, #0]
	movs	r1, #0
	movs	r0, #7
	bl	Func_080bbabc
	mov	r1, sl
	movs	r0, #0
	bl	Func_080bbabc
	ldr	r1, [pc, #160]
	bl	.L135
	ldr	r1, [sp, #76]
	ldrh	r4, [r7, #56]
	ldrb	r3, [r1, #3]
	movs	r6, #56
	ldrsh	r5, [r7, r6]
	mov	r8, r4
	ldrh	r6, [r7, #52]
	movs	r0, #52
	ldrsh	r2, [r7, r0]
	cmp	r3, #61
	bne.n	.L136
	lsls	r0, r2, #4
	subs	r0, r0, r2
	lsls	r0, r0, #2
	b.n	.L137
.L136:
	lsls	r0, r2, #4
	subs	r0, r0, r2
	lsls	r0, r0, #1
.L137:
	movs	r1, #100
	bl	Func_080022ec
	adds	r5, r5, r0
	lsls	r3, r6, #16
	asrs	r2, r3, #16
	cmp	r5, r2
	ble.n	.L138
	adds	r5, r2, #0
.L138:
	mov	r4, r8
	lsls	r3, r4, #16
	asrs	r3, r3, #16
	subs	r1, r5, r3
	cmp	r1, #0
	bne.n	.L139
	ldr	r6, [sp, #24]
	cmp	r6, #1
	beq.n	.L139
	bl	.L122
.L139:
	cmp	r5, r2
	bne.n	.L140
	ldr	r1, [pc, #92]
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L141
.L140:
	movs	r0, #1
	bl	Func_080bbabc
	ldr	r1, [pc, #80]
	movs	r0, #4
	bl	Func_080bbabc
.L141:
	strh	r5, [r7, #56]
	b.n	.L142
	movs	r0, r0
	.4byte 0x00000827
	.4byte 0x00000826
	.4byte 0x00000825
	.4byte 0x00000824
	.4byte 0x00000854
	.4byte 0x080bc6b0
	.4byte 0x0000088b
	.4byte 0x0000013b
	.4byte 0x0000088d
	.4byte 0x0000013d
	.4byte 0x0000088c
	.4byte 0x00000141
	.4byte 0x00000894
	.4byte 0x0000088f
	.4byte 0x00000131
	.4byte 0x00000884
	.4byte 0x00000820
	.4byte 0x0000081d
	movs	r1, #54
	ldrsh	r6, [r7, r1]
	movs	r0, #58
	ldrsh	r5, [r7, r0]
	lsls	r0, r6, #3
	subs	r0, r0, r6
	movs	r1, #100
	bl	Func_080022ec
	mov	r8, r5
	adds	r5, r5, r0
	cmp	r5, r6
	ble.n	.L143
	adds	r5, r6, #0
.L143:
	mov	r2, r8
	subs	r1, r5, r2
	cmp	r1, #0
	bne.n	.L144
	ldr	r3, [sp, #24]
	cmp	r3, #11
	beq.n	.L144
	bl	.L122
.L144:
	cmp	r5, r6
	bne.n	.L145
	ldr	r1, [pc, #836]
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L146
.L145:
	movs	r0, #1
	bl	Func_080bbabc
	ldr	r1, [pc, #824]
	movs	r0, #4
	bl	Func_080bbabc
.L146:
	strh	r5, [r7, #58]
	b.n	.L142
	ldr	r4, [pc, #816]
	movs	r5, #163
	adds	r2, r7, r4
	movs	r3, #8
	lsls	r5, r5, #1
	strb	r3, [r2, #0]
	adds	r2, r7, r5
	movs	r3, #5
	strb	r3, [r2, #0]
	mov	r0, sl
	bl	Func_08077010
	adds	r3, r7, #0
	adds	r3, #64
	ldrh	r1, [r3, #0]
	ldr	r3, [sp, #16]
	adds	r3, #64
	ldrh	r3, [r3, #0]
	movs	r0, #1
	subs	r1, r1, r3
	bl	Func_080bbabc
	ldr	r1, [pc, #776]
	bl	.L135
	ldr	r6, [pc, #764]
	movs	r0, #163
	adds	r2, r7, r6
	movs	r3, #252
	lsls	r0, r0, #1
	strb	r3, [r2, #0]
	adds	r2, r7, r0
	movs	r3, #5
	strb	r3, [r2, #0]
	mov	r0, sl
	bl	Func_08077010
	ldr	r3, [sp, #16]
	adds	r3, #64
	ldrh	r1, [r3, #0]
	adds	r3, r7, #0
	adds	r3, #64
	ldrh	r3, [r3, #0]
	movs	r0, #1
	subs	r1, r1, r3
	bl	Func_080bbabc
	ldr	r1, [pc, #728]
	bl	.L135
	ldr	r1, [pc, #728]
	adds	r2, r7, r1
	ldrb	r3, [r2, #0]
	subs	r3, #1
	strb	r3, [r2, #0]
	movs	r4, #4
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r4, r4
	cmp	r3, r4
	bge.n	.L147
	movs	r3, #252
	strb	r3, [r2, #0]
.L147:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #4
	ble.n	.L148
	movs	r3, #4
	strb	r3, [r2, #0]
.L148:
	mov	r0, sl
	bl	Func_08077010
	ldr	r5, [sp, #16]
	ldrh	r3, [r7, #60]
	ldrh	r1, [r5, #60]
	movs	r0, #1
	subs	r1, r1, r3
	movs	r6, #153
	bl	Func_080bbabc
	lsls	r6, r6, #1
	ldr	r1, [pc, #668]
	movs	r0, #4
	bl	Func_080bbabc
	adds	r2, r7, r6
	b.n	.L149
	ldr	r0, [pc, #652]
	adds	r2, r7, r0
	ldrb	r3, [r2, #0]
	subs	r3, #2
	strb	r3, [r2, #0]
	movs	r1, #4
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r1, r1
	cmp	r3, r1
	bge.n	.L150
	movs	r3, #252
	strb	r3, [r2, #0]
.L150:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #4
	ble.n	.L151
	movs	r3, #4
	strb	r3, [r2, #0]
.L151:
	mov	r0, sl
	bl	Func_08077010
	ldr	r2, [sp, #16]
	ldrh	r3, [r7, #60]
	ldrh	r1, [r2, #60]
	movs	r0, #1
	subs	r1, r1, r3
	bl	Func_080bbabc
	ldr	r1, [pc, #600]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r3, #153
	lsls	r3, r3, #1
	adds	r2, r7, r3
	b.n	.L149
	ldr	r4, [pc, #580]
	adds	r2, r7, r4
	ldrb	r3, [r2, #0]
	adds	r3, #1
	strb	r3, [r2, #0]
	movs	r5, #4
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r5, r5
	cmp	r3, r5
	bge.n	.L152
	movs	r3, #252
	strb	r3, [r2, #0]
.L152:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #4
	ble.n	.L153
	movs	r3, #4
	strb	r3, [r2, #0]
.L153:
	mov	r0, sl
	bl	Func_08077010
	ldr	r6, [sp, #16]
	ldrh	r1, [r7, #60]
	ldrh	r3, [r6, #60]
	movs	r0, #1
	subs	r1, r1, r3
	bl	Func_080bbabc
	ldr	r1, [pc, #528]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r0, #153
	b.n	.L154
	ldr	r1, [pc, #508]
	adds	r2, r7, r1
	ldrb	r3, [r2, #0]
	adds	r3, #2
	strb	r3, [r2, #0]
	movs	r4, #4
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r4, r4
	cmp	r3, r4
	bge.n	.L155
	movs	r3, #252
	strb	r3, [r2, #0]
.L155:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #4
	ble.n	.L156
	movs	r3, #4
	strb	r3, [r2, #0]
.L156:
	mov	r0, sl
	bl	Func_08077010
	ldr	r5, [sp, #16]
	ldrh	r1, [r7, #60]
	ldrh	r3, [r5, #60]
	movs	r0, #1
	subs	r1, r1, r3
	movs	r6, #153
	bl	Func_080bbabc
	lsls	r6, r6, #1
	ldr	r1, [pc, #456]
	movs	r0, #4
	bl	Func_080bbabc
	adds	r2, r7, r6
	b.n	.L149
	ldr	r0, [pc, #448]
	adds	r2, r7, r0
	ldrb	r3, [r2, #0]
	subs	r3, #1
	strb	r3, [r2, #0]
	movs	r1, #4
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r1, r1
	cmp	r3, r1
	bge.n	.L157
	movs	r3, #252
	strb	r3, [r2, #0]
.L157:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #4
	ble.n	.L158
	movs	r3, #4
	strb	r3, [r2, #0]
.L158:
	mov	r0, sl
	bl	Func_08077010
	ldr	r2, [sp, #16]
	ldrh	r3, [r7, #62]
	ldrh	r1, [r2, #62]
	movs	r0, #1
	subs	r1, r1, r3
	bl	Func_080bbabc
	ldr	r1, [pc, #392]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r3, #154
	lsls	r3, r3, #1
	adds	r2, r7, r3
	b.n	.L149
	ldr	r4, [pc, #372]
	adds	r2, r7, r4
	ldrb	r3, [r2, #0]
	subs	r3, #2
	strb	r3, [r2, #0]
	movs	r5, #4
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r5, r5
	cmp	r3, r5
	bge.n	.L159
	movs	r3, #252
	strb	r3, [r2, #0]
.L159:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #4
	ble.n	.L160
	movs	r3, #4
	strb	r3, [r2, #0]
.L160:
	mov	r0, sl
	bl	Func_08077010
	ldr	r6, [sp, #16]
	ldrh	r3, [r7, #62]
	ldrh	r1, [r6, #62]
	movs	r0, #1
	subs	r1, r1, r3
	bl	Func_080bbabc
	ldr	r1, [pc, #320]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r0, #154
	b.n	.L154
	ldr	r1, [pc, #304]
	adds	r2, r7, r1
	ldrb	r3, [r2, #0]
	adds	r3, #1
	strb	r3, [r2, #0]
	movs	r4, #4
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r4, r4
	cmp	r3, r4
	bge.n	.L161
	movs	r3, #252
	strb	r3, [r2, #0]
.L161:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #4
	ble.n	.L162
	movs	r3, #4
	strb	r3, [r2, #0]
.L162:
	mov	r0, sl
	bl	Func_08077010
	ldr	r5, [sp, #16]
	ldrh	r1, [r7, #62]
	ldrh	r3, [r5, #62]
	movs	r0, #1
	subs	r1, r1, r3
	movs	r6, #154
	bl	Func_080bbabc
	lsls	r6, r6, #1
	ldr	r1, [pc, #248]
	movs	r0, #4
	bl	Func_080bbabc
	adds	r2, r7, r6
	b.n	.L149
	ldr	r0, [pc, #228]
	adds	r2, r7, r0
	ldrb	r3, [r2, #0]
	adds	r3, #2
	strb	r3, [r2, #0]
	movs	r1, #4
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r1, r1
	cmp	r3, r1
	bge.n	.L163
	movs	r3, #252
	strb	r3, [r2, #0]
.L163:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #4
	ble.n	.L164
	movs	r3, #4
	strb	r3, [r2, #0]
.L164:
	mov	r0, sl
	bl	Func_08077010
	ldr	r2, [sp, #16]
	ldrh	r1, [r7, #62]
	ldrh	r3, [r2, #62]
	movs	r0, #1
	subs	r1, r1, r3
	bl	Func_080bbabc
	ldr	r1, [pc, #180]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r3, #154
	lsls	r3, r3, #1
	adds	r2, r7, r3
	b.n	.L149
	movs	r4, #56
	ldrsh	r3, [r7, r4]
	cmp	r3, #0
	beq.n	.L165
	b.n	.L122
.L165:
	ldr	r1, [pc, #156]
	movs	r0, #4
	bl	Func_080bbabc
	ldrh	r3, [r7, #52]
	mov	r0, sl
	strh	r3, [r7, #56]
	bl	Func_08077128
	b.n	.L122
	movs	r5, #56
	ldrsh	r3, [r7, r5]
	cmp	r3, #0
	beq.n	.L166
	b.n	.L122
.L166:
	ldr	r1, [pc, #128]
	movs	r0, #4
	bl	Func_080bbabc
	ldrh	r3, [r7, #52]
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	lsrs	r3, r3, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	strh	r2, [r7, #56]
	b.n	.L142
	movs	r6, #56
	ldrsh	r3, [r7, r6]
	cmp	r3, #0
	beq.n	.L167
	b.n	.L122
.L167:
	ldr	r1, [pc, #92]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r1, #52
	ldrsh	r0, [r7, r1]
	movs	r1, #10
	lsls	r0, r0, #3
	bl	Func_080022ec
	strh	r0, [r7, #56]
	b.n	.L142
	ldr	r2, [pc, #72]
	adds	r5, r7, r2
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	beq.n	.L168
	ldr	r1, [pc, #64]
	movs	r0, #4
	bl	Func_080bbabc
.L168:
	movs	r3, #0
	strb	r3, [r5, #0]
	b.n	.L122
	movs	r0, r0
	.4byte 0x00000821
	.4byte 0x0000081e
	.4byte 0x00000147
	.4byte 0x00000877
	.4byte 0x00000878
	.4byte 0x00000133
	.4byte 0x00000860
	.4byte 0x00000861
	.4byte 0x00000135
	.4byte 0x00000862
	.4byte 0x00000863
	.4byte 0x00000864
	.4byte 0x00000131
	.4byte 0x00000884
	ldr	r3, [pc, #816]
	adds	r2, r7, r3
	ldrb	r3, [r2, #0]
	subs	r3, #1
	strb	r3, [r2, #0]
	movs	r4, #4
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r4, r4
	cmp	r3, r4
	bge.n	.L169
	movs	r3, #252
	strb	r3, [r2, #0]
.L169:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	ldrb	r1, [r2, #0]
	cmp	r3, #4
	ble.n	.L170
	movs	r3, #4
	strb	r3, [r2, #0]
	movs	r1, #4
.L170:
	ldr	r5, [sp, #16]
	ldr	r6, [pc, #772]
	adds	r3, r5, r6
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	lsls	r3, r1, #24
	asrs	r3, r3, #24
	subs	r2, r2, r3
	lsls	r1, r2, #2
	adds	r1, r1, r2
	lsls	r1, r1, #2
	movs	r0, #1
	bl	Func_080bbabc
	ldr	r1, [pc, #752]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r0, #155
	b.n	.L154
	ldr	r1, [pc, #736]
	adds	r2, r7, r1
	ldrb	r3, [r2, #0]
	subs	r3, #2
	strb	r3, [r2, #0]
	movs	r4, #4
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r4, r4
	cmp	r3, r4
	bge.n	.L171
	movs	r3, #252
	strb	r3, [r2, #0]
.L171:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	ldrb	r1, [r2, #0]
	cmp	r3, #4
	ble.n	.L172
	movs	r3, #4
	strb	r3, [r2, #0]
	movs	r1, #4
.L172:
	ldr	r5, [sp, #16]
	ldr	r6, [pc, #692]
	adds	r3, r5, r6
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	lsls	r3, r1, #24
	asrs	r3, r3, #24
	subs	r2, r2, r3
	lsls	r1, r2, #2
	adds	r1, r1, r2
	lsls	r1, r1, #2
	movs	r0, #1
	bl	Func_080bbabc
	ldr	r1, [pc, #668]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r0, #155
	b.n	.L154
	ldr	r1, [pc, #652]
	adds	r2, r7, r1
	ldrb	r3, [r2, #0]
	adds	r3, #1
	strb	r3, [r2, #0]
	movs	r4, #4
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r4, r4
	cmp	r3, r4
	bge.n	.L173
	movs	r3, #252
	strb	r3, [r2, #0]
.L173:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	ldrb	r1, [r2, #0]
	cmp	r3, #4
	ble.n	.L174
	movs	r3, #4
	strb	r3, [r2, #0]
	movs	r1, #4
.L174:
	ldr	r5, [sp, #16]
	ldr	r6, [pc, #608]
	adds	r2, r5, r6
	ldrb	r2, [r2, #0]
	lsls	r2, r2, #24
	asrs	r2, r2, #24
	lsls	r3, r1, #24
	asrs	r3, r3, #24
	subs	r3, r3, r2
	lsls	r1, r3, #2
	adds	r1, r1, r3
	lsls	r1, r1, #2
	movs	r0, #1
	bl	Func_080bbabc
	ldr	r1, [pc, #588]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r0, #155
	b.n	.L154
	ldr	r1, [pc, #568]
	adds	r2, r7, r1
	ldrb	r3, [r2, #0]
	adds	r3, #2
	strb	r3, [r2, #0]
	movs	r4, #4
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r4, r4
	cmp	r3, r4
	bge.n	.L175
	movs	r3, #252
	strb	r3, [r2, #0]
.L175:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	ldrb	r1, [r2, #0]
	cmp	r3, #4
	ble.n	.L176
	movs	r3, #4
	strb	r3, [r2, #0]
	movs	r1, #4
.L176:
	ldr	r5, [sp, #16]
	ldr	r6, [pc, #524]
	adds	r2, r5, r6
	ldrb	r2, [r2, #0]
	lsls	r2, r2, #24
	asrs	r2, r2, #24
	lsls	r3, r1, #24
	asrs	r3, r3, #24
	subs	r3, r3, r2
	lsls	r1, r3, #2
	adds	r1, r1, r3
	lsls	r1, r1, #2
	movs	r0, #1
	bl	Func_080bbabc
	ldr	r1, [pc, #504]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r0, #155
	b.n	.L154
	ldr	r1, [pc, #496]
	adds	r5, r7, r1
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	beq.n	.L177
	b.n	.L122
.L177:
	ldr	r1, [pc, #488]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r3, #1
	strb	r3, [r5, #0]
	b.n	.L122
	ldr	r2, [pc, #468]
	adds	r5, r7, r2
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	cmp	r3, #1
	ble.n	.L178
	b.n	.L122
.L178:
	ldr	r1, [pc, #464]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r3, #2
	strb	r3, [r5, #0]
	b.n	.L122
	ldr	r1, [pc, #452]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r3, #156
	lsls	r3, r3, #1
	adds	r2, r7, r3
	b.n	.L149
	ldr	r1, [pc, #440]
	movs	r0, #4
	bl	Func_080bbabc
	ldr	r4, [pc, #436]
	movs	r3, #7
	adds	r2, r7, r4
	strb	r3, [r2, #0]
	b.n	.L122
	movs	r5, #157
	ldr	r1, [pc, #428]
	movs	r0, #4
	lsls	r5, r5, #1
	bl	Func_080bbabc
	adds	r2, r7, r5
	b.n	.L149
	ldr	r6, [pc, #420]
	ldr	r1, [pc, #420]
	movs	r0, #4
	bl	Func_080bbabc
	adds	r2, r7, r6
	b.n	.L149
	ldr	r1, [pc, #412]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r0, #158
	b.n	.L154
	mov	r1, sl
	cmp	r1, #7
	bhi.n	.L179
	ldr	r1, [pc, #400]
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L180
.L179:
	ldr	r1, [pc, #392]
	movs	r0, #4
	bl	Func_080bbabc
.L180:
	ldr	r2, [pc, #388]
	adds	r1, r7, r2
	ldrb	r2, [r1, #0]
	movs	r3, #7
	b.n	.L181
	mov	r3, sl
	cmp	r3, #7
	bhi.n	.L182
	ldr	r1, [pc, #364]
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L183
.L182:
	ldr	r1, [pc, #360]
	movs	r0, #4
	bl	Func_080bbabc
.L183:
	ldr	r4, [pc, #356]
	adds	r1, r7, r4
	ldrb	r2, [r1, #0]
	movs	r3, #16
.L181:
	orrs	r3, r2
	strb	r3, [r1, #0]
	b.n	.L122
	movs	r5, #149
	movs	r0, #9
	mov	r1, sl
	lsls	r5, r5, #1
	bl	Func_080bbabc
	adds	r3, r7, r5
	ldrb	r3, [r3, #0]
	cmp	r3, #2
	bne.n	.L184
	ldr	r1, [pc, #324]
	b.n	.L185
.L184:
	ldr	r6, [sp, #64]
	cmp	r6, #219
	bne.n	.L186
	ldr	r1, [pc, #320]
.L185:
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L187
.L186:
	ldr	r1, [pc, #312]
	movs	r0, #4
	bl	Func_080bbabc
.L187:
	movs	r3, #0
	strh	r3, [r7, #56]
.L142:
	mov	r0, sl
	bl	Func_08077128
	b.n	.L122
	ldr	r1, [pc, #296]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r0, #159
.L154:
	lsls	r0, r0, #1
	adds	r2, r7, r0
.L149:
	movs	r3, #7
	strb	r3, [r2, #0]
	b.n	.L122
	ldr	r1, [pc, #280]
	movs	r0, #4
	bl	Func_080bbabc
	ldr	r1, [pc, #276]
	movs	r3, #7
	adds	r2, r7, r1
	strb	r3, [r2, #0]
	b.n	.L122
	ldr	r3, [sp, #72]
	ldr	r4, [sp, #76]
	movs	r2, #56
	ldrsh	r6, [r3, r2]
	ldrb	r3, [r4, #3]
	adds	r2, r6, #0
	ldr	r5, [sp, #44]
	cmp	r3, #60
	bne.n	.L188
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r5, r3, #1
.L188:
	ldr	r1, [sp, #72]
	movs	r0, #52
	ldrsh	r3, [r1, r0]
	adds	r6, r6, r5
	cmp	r6, r3
	ble.n	.L189
	adds	r6, r3, #0
	subs	r5, r6, r2
.L189:
	movs	r1, #0
	movs	r0, #7
	bl	Func_080bbabc
	movs	r0, #0
	ldr	r1, [sp, #68]
	bl	Func_080bbabc
	ldr	r4, [sp, #72]
	movs	r2, #52
	ldrsh	r3, [r4, r2]
	cmp	r6, r3
	bne.n	.L190
	ldr	r1, [pc, #204]
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L191
.L190:
	adds	r1, r5, #0
	movs	r0, #1
	bl	Func_080bbabc
	ldr	r1, [pc, #192]
	movs	r0, #4
	bl	Func_080bbabc
.L191:
	ldr	r5, [sp, #72]
	strh	r6, [r5, #56]
	b.n	.L192
	ldr	r0, [sp, #72]
	movs	r6, #58
	ldrsh	r5, [r0, r6]
	ldr	r6, [sp, #44]
	movs	r1, #54
	ldrsh	r3, [r0, r1]
	adds	r2, r5, #0
	adds	r5, r5, r6
	cmp	r5, r3
	ble.n	.L193
	adds	r5, r3, #0
	subs	r6, r5, r2
.L193:
	movs	r1, #0
	movs	r0, #7
	bl	Func_080bbabc
	movs	r0, #0
	ldr	r1, [sp, #68]
	bl	Func_080bbabc
	ldr	r4, [sp, #72]
	movs	r2, #54
	ldrsh	r3, [r4, r2]
	cmp	r5, r3
	bne.n	.L194
	ldr	r1, [pc, #132]
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L195
.L194:
	adds	r1, r6, #0
	movs	r0, #1
	bl	Func_080bbabc
	ldr	r1, [pc, #116]
	movs	r0, #4
	bl	Func_080bbabc
.L195:
	ldr	r6, [sp, #72]
	strh	r5, [r6, #58]
.L192:
	ldr	r0, [sp, #68]
	bl	Func_08077128
	b.n	.L122
	.4byte 0x00000137
	.4byte 0x00000865
	.4byte 0x00000866
	.4byte 0x00000131
	.4byte 0x00000867
	.4byte 0x00000874
	.4byte 0x00000868
	.4byte 0x00000869
	.4byte 0x00000139
	.4byte 0x0000086a
	.4byte 0x0000013b
	.4byte 0x0000086b
	.4byte 0x0000086c
	.4byte 0x0000086d
	.4byte 0x00000876
	.4byte 0x0000013d
	.4byte 0x0000084f
	.4byte 0x00000850
	.4byte 0x0000084c
	.4byte 0x0000086f
	.4byte 0x00000870
	.4byte 0x0000013f
	.4byte 0x00000820
	.4byte 0x0000081d
	.4byte 0x00000821
	.4byte 0x0000081e
	ldr	r0, [sp, #44]
	movs	r1, #10
	bl	Func_080022ec
	adds	r5, r0, #0
	movs	r0, #58
	ldrsh	r3, [r7, r0]
	cmp	r3, r5
	bge.n	.L196
	adds	r5, r3, #0
.L196:
	ldr	r3, [sp, #72]
	ldr	r6, [sp, #72]
	movs	r2, #58
	ldrsh	r1, [r3, r2]
	movs	r4, #54
	ldrsh	r2, [r6, r4]
	adds	r3, r1, r5
	cmp	r3, r2
	ble.n	.L197
	subs	r5, r2, r1
.L197:
	cmp	r5, #0
	bne.n	.L198
	b.n	.L122
.L198:
	movs	r0, #1
	adds	r1, r5, #0
	bl	Func_080bbabc
	mov	r0, sl
	cmp	r0, #7
	bhi.n	.L199
	ldr	r1, [pc, #520]
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L200
.L199:
	ldr	r1, [pc, #516]
	movs	r0, #4
	bl	Func_080bbabc
.L200:
	ldr	r0, [sp, #68]
	adds	r1, r5, #0
	bl	Func_08077120
	b.n	.L122
	ldr	r1, [pc, #500]
	adds	r2, r7, r1
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	ble.n	.L201
	movs	r4, #153
	movs	r3, #0
	lsls	r4, r4, #1
	strb	r3, [r2, #0]
	adds	r2, r7, r4
	strb	r3, [r2, #0]
.L201:
	ldr	r5, [pc, #480]
	adds	r1, r7, r5
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	ble.n	.L202
	movs	r6, #154
	lsls	r6, r6, #1
	movs	r2, #0
	adds	r3, r7, r6
	strb	r2, [r1, #0]
	strb	r2, [r3, #0]
.L202:
	ldr	r0, [pc, #460]
	adds	r2, r7, r0
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	ble.n	.L203
	movs	r1, #155
	movs	r3, #0
	lsls	r1, r1, #1
	strb	r3, [r2, #0]
	adds	r2, r7, r1
	strb	r3, [r2, #0]
.L203:
	ldr	r3, [pc, #440]
	adds	r2, r7, r3
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	ble.n	.L204
	movs	r3, #0
	strb	r3, [r2, #0]
.L204:
	movs	r4, #150
	lsls	r4, r4, #1
	ldr	r5, [pc, #424]
	movs	r2, #0
	adds	r3, r7, r4
	movs	r6, #151
	strb	r2, [r3, #0]
	ldr	r0, [pc, #420]
	adds	r3, r7, r5
	lsls	r6, r6, #1
	strb	r2, [r3, #0]
	adds	r3, r7, r6
	strb	r2, [r3, #0]
	adds	r3, r7, r0
	strb	r2, [r3, #0]
	ldr	r1, [pc, #408]
	b.n	.L135
	ldr	r1, [pc, #408]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r1, #160
	lsls	r1, r1, #1
	adds	r2, r7, r1
	b.n	.L205
	ldr	r2, [pc, #396]
	adds	r5, r7, r2
	ldrb	r3, [r5, #0]
	adds	r2, r3, #0
	cmp	r2, #0
	bne.n	.L206
	ldr	r1, [pc, #388]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r3, #7
	strb	r3, [r5, #0]
	b.n	.L122
.L206:
	cmp	r2, #1
	bls.n	.L122
	adds	r3, #255
	strb	r3, [r5, #0]
	movs	r0, #1
	ldrb	r1, [r5, #0]
	bl	Func_080bbabc
	ldr	r1, [pc, #360]
.L135:
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L122
	ldr	r1, [pc, #356]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r3, #162
	lsls	r3, r3, #1
	adds	r2, r7, r3
	b.n	.L207
	ldr	r1, [pc, #344]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r4, #164
	lsls	r4, r4, #1
	adds	r2, r7, r4
	movs	r3, #1
	mov	r5, sl
	strb	r3, [r2, #0]
	cmp	r5, #7
	bhi.n	.L122
	ldr	r3, [sp, #56]
	adds	r3, #67
	ldrb	r2, [r3, #0]
	movs	r1, #2
	orrs	r2, r1
	strb	r2, [r3, #0]
	b.n	.L122
	ldr	r6, [pc, #308]
	ldr	r1, [pc, #312]
	movs	r0, #4
	bl	Func_080bbabc
	adds	r2, r7, r6
	b.n	.L205
	ldr	r1, [pc, #304]
	movs	r0, #4
	bl	Func_080bbabc
	ldr	r0, [pc, #300]
	adds	r2, r7, r0
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	bgt.n	.L122
.L205:
	movs	r3, #1
	strb	r3, [r2, #0]
	b.n	.L122
	ldr	r1, [pc, #284]
	movs	r0, #4
	bl	Func_080bbabc
	ldr	r1, [pc, #272]
	adds	r2, r7, r1
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #1
	bgt.n	.L122
.L207:
	movs	r3, #2
	strb	r3, [r2, #0]
	b.n	.L122
	movs	r1, #1
	negs	r1, r1
	movs	r0, #4
	bl	Func_080bbabc
.L122:
	movs	r0, #7
	movs	r1, #0
	bl	Func_080bbabc
	movs	r2, #56
	ldrsh	r3, [r7, r2]
	cmp	r3, #0
	beq.n	.L208
	movs	r3, #158
	lsls	r3, r3, #1
	adds	r5, r7, r3
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L208
	cmp	r3, #6
	bhi.n	.L208
	ldr	r4, [sp, #44]
	cmp	r4, #0
	ble.n	.L208
	bl	Func_080771a0
	movs	r3, #3
	ands	r0, r3
	cmp	r0, #0
	bne.n	.L208
	strb	r0, [r5, #0]
	mov	r1, sl
	movs	r0, #0
	bl	Func_080bbabc
	ldr	r1, [pc, #192]
	movs	r0, #4
	bl	Func_080bbabc
.L208:
	ldr	r0, [sp, #16]
	bl	Func_08002df0
	mov	r0, sl
	bl	Func_08077010
	ldr	r3, [pc, #176]
	ldr	r3, [r3, #0]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	bl	Func_08015130
	movs	r5, #56
	ldrsh	r3, [r7, r5]
	cmp	r3, #0
	beq.n	.L209
	movs	r0, #11
	mov	r1, sl
	bl	Func_080bbabc
.L209:
	ldr	r6, [sp, #72]
	movs	r0, #160
	lsls	r0, r0, #1
	adds	r3, r6, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L210
	bl	Func_080771a0
	movs	r3, #3
	ands	r0, r3
	cmp	r0, #0
	bne.n	.L210
	ldr	r1, [sp, #44]
	cmp	r1, #0
	ble.n	.L210
	asrs	r0, r1, #2
	cmp	r0, #0
	bne.n	.L211
	movs	r0, #1
.L211:
	ldr	r2, [sp, #80]
	ldr	r3, [r2, #96]
	adds	r3, r3, r0
	str	r3, [r2, #96]
.L210:
	add	sp, #100
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x0000085f
	.4byte 0x0000085e
	.4byte 0x00000133
	.4byte 0x00000135
	.4byte 0x00000137
	.4byte 0x00000147
	.4byte 0x0000012d
	.4byte 0x0000012f
	.4byte 0x00000896
	.4byte 0x00000872
	.4byte 0x00000141
	.4byte 0x00000873
	.4byte 0x00000875
	.4byte 0x0000087d
	.4byte 0x0000087e
	.4byte 0x00000145
	.4byte 0x0000087f
	.4byte 0x00000881
	.4byte 0x0000012b
	.4byte 0x00000882
	.4byte 0x00000883
	.4byte 0x03001e74

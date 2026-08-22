@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080ab5e4
	.thumb_func
Func_080ab5e4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #108
	str	r0, [sp, #80]
	ldr	r3, [pc, #496]
	ldr	r3, [r3, #0]
	movs	r0, #194
	str	r3, [sp, #76]
	ldr	r2, [sp, #80]
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r3, [r3, #0]
	lsls	r2, r2, #1
	movs	r1, #1
	movs	r5, #186
	str	r1, [sp, #72]
	str	r2, [sp, #52]
	ldr	r7, [sp, #76]
	lsls	r5, r5, #1
	mov	r9, r3
	adds	r3, r2, r5
	ldrh	r5, [r7, r3]
	movs	r1, #10
	adds	r0, r5, #0
	bl	Func_08002304
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	str	r0, [sp, #56]
	movs	r1, #10
	adds	r0, r5, #0
	bl	Func_080022f4
	mov	r2, sp
	adds	r2, #100
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	str	r2, [sp, #28]
	movs	r1, #1
	str	r0, [sp, #48]
	negs	r1, r1
	movs	r0, #0
	ldr	r5, [sp, #28]
	mov	r3, sp
	str	r0, [sp, #44]
	str	r0, [sp, #40]
	str	r0, [sp, #36]
	str	r0, [sp, #32]
	str	r1, [sp, #60]
	movs	r2, #0
	adds	r3, #107
	mov	ip, r5
.L0:
	strb	r2, [r3, #0]
	subs	r3, #1
	cmp	r3, ip
	bge.n	.L0
	ldr	r7, [sp, #80]
	cmp	r7, #0
	bne.n	.L1
	mov	r0, r9
	bl	Func_080aafb8
	ldr	r3, [pc, #384]
	movs	r0, #0
	ldr	r1, [sp, #76]
	str	r0, [sp, #68]
	adds	r2, r1, r3
	ldrb	r3, [r2, #0]
	cmp	r7, r3
	bge.n	.L2
	adds	r0, r2, #0
	ldr	r1, [sp, #28]
	mov	r2, r9
	movs	r4, #4
	adds	r2, #160
.L4:
	ldrb	r3, [r2, #0]
	lsls	r3, r3, #24
	adds	r2, #1
	cmp	r3, #0
	bne.n	.L3
	strb	r4, [r1, #0]
.L3:
	ldr	r5, [sp, #68]
	adds	r5, #1
	str	r5, [sp, #68]
	ldrb	r3, [r0, #0]
	adds	r1, #1
	cmp	r5, r3
	blt.n	.L4
.L2:
	ldr	r7, [sp, #48]
	movs	r0, #0
	ldr	r1, [sp, #76]
	ldr	r2, [pc, #324]
	str	r7, [sp, #44]
	str	r0, [sp, #68]
	adds	r3, r1, r2
	ldrb	r3, [r3, #0]
	cmp	r0, r3
	bge.n	.L5
	adds	r5, r1, r2
.L7:
	ldr	r7, [sp, #28]
	ldr	r0, [sp, #56]
	ldrsb	r3, [r7, r0]
	cmp	r3, #4
	bne.n	.L6
	adds	r0, #1
	str	r0, [sp, #56]
	ldrb	r1, [r5, #0]
	bl	Func_080aa538
	str	r0, [sp, #56]
.L6:
	ldr	r1, [sp, #68]
	adds	r1, #1
	str	r1, [sp, #68]
	ldrb	r3, [r5, #0]
	cmp	r1, r3
	blt.n	.L7
	b.n	.L5
.L1:
	ldr	r2, [sp, #76]
	add	r5, sp, #84
	movs	r1, #28
	ldrsb	r1, [r2, r1]
	adds	r0, r5, #0
	bl	Func_080ae714
	movs	r3, #0
	ldr	r7, [sp, #76]
	ldr	r0, [pc, #256]
	str	r3, [sp, #68]
	adds	r2, r7, r0
	ldrb	r3, [r2, #0]
	movs	r1, #0
	cmp	r1, r3
	bge.n	.L5
	ldr	r0, [sp, #28]
	mov	r1, r9
	adds	r4, r2, #0
	adds	r1, #160
	adds	r2, r0, #0
	movs	r6, #7
.L11:
	ldr	r7, [sp, #76]
	movs	r3, #28
	ldrsb	r3, [r7, r3]
	ldr	r7, [sp, #68]
	cmp	r7, r3
	bne.n	.L8
	strb	r6, [r2, #0]
	b.n	.L9
.L8:
	ldr	r7, [sp, #68]
	ldrb	r3, [r5, r7]
	cmp	r3, #0
	beq.n	.L10
	movs	r3, #0
	strb	r3, [r2, #0]
	b.n	.L9
.L10:
	movs	r3, #3
	strb	r3, [r2, #0]
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	bne.n	.L9
	movs	r3, #7
	strb	r3, [r0, #0]
.L9:
	ldr	r3, [sp, #68]
	adds	r3, #1
	str	r3, [sp, #68]
	ldr	r7, [sp, #68]
	ldrb	r3, [r4, #0]
	adds	r0, #1
	adds	r1, #1
	adds	r2, #1
	cmp	r7, r3
	blt.n	.L11
.L5:
	ldr	r0, [sp, #80]
	cmp	r0, #1
	bne.n	.L12
	ldr	r1, [sp, #76]
	movs	r2, #186
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldrh	r6, [r3, #0]
	movs	r1, #10
	adds	r0, r6, #0
	bl	Func_08002304
	movs	r1, #10
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	Func_080022f4
	lsls	r5, r5, #16
	lsrs	r5, r5, #16
	ldr	r3, [sp, #76]
	adds	r2, r0, #0
	lsls	r6, r5, #3
	subs	r6, r6, r5
	lsls	r2, r2, #16
	ldr	r0, [r3, #48]
	ldr	r5, [sp, #80]
	adds	r6, #1
	movs	r3, #14
	lsrs	r2, r2, #16
	str	r3, [sp, #4]
	adds	r2, #2
	adds	r1, r6, #0
	movs	r3, #6
	str	r5, [sp, #0]
	bl	Func_080ab1f4
	ldr	r7, [sp, #76]
	movs	r3, #7
	ldr	r0, [r7, #48]
	adds	r1, r6, #0
	str	r3, [sp, #0]
	movs	r2, #2
	movs	r3, #6
	str	r3, [sp, #4]
	bl	Func_080ab2ec
	ldr	r0, [pc, #72]
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	movs	r5, #0
	cmp	r5, r3
	bge.n	.L12
	movs	r6, #8
.L17:
	ldr	r1, [sp, #76]
	movs	r3, #28
	ldrsb	r3, [r1, r3]
	cmp	r5, r3
	bne.n	.L13
	movs	r2, #188
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldrh	r3, [r3, #0]
	ldr	r2, [pc, #32]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L14
	ldr	r0, [pc, #36]
	b.n	.L15
.L14:
	ldr	r0, [pc, #36]
	b.n	.L15
.L13:
	ldr	r3, [sp, #28]
	ldrb	r2, [r3, r5]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L16
	ldr	r0, [pc, #24]
	b.n	.L15
	movs	r0, r0
	.4byte 0x00008000
	.4byte 0x03001f2c
	.4byte 0x00000219
	.4byte 0x00000bb0
	.4byte 0x00000baf
	.4byte 0x00000bae
.L16:
	ldr	r0, [pc, #840]
.L15:
	ldr	r7, [sp, #76]
	adds	r2, r6, #0
	movs	r3, #8
	ldr	r1, [r7, #48]
	bl	Func_08015080
	ldr	r0, [pc, #832]
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	adds	r5, #1
	adds	r6, #56
	cmp	r5, r3
	blt.n	.L17
.L12:
	ldr	r1, [sp, #76]
	movs	r2, #134
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldr	r0, [r3, #0]
	bl	Func_08015270
	ldr	r3, [sp, #76]
	ldr	r2, [r3, #20]
	movs	r3, #1
	strb	r3, [r2, #5]
	ldr	r5, [sp, #56]
	lsls	r5, r5, #3
	str	r5, [sp, #24]
.L99:
	ldr	r7, [sp, #72]
	cmp	r7, #0
	bne.n	.L18
	b.n	.L19
.L18:
	movs	r1, #1
	movs	r0, #0
	negs	r1, r1
	ldr	r3, [sp, #28]
	str	r0, [sp, #72]
	str	r1, [sp, #60]
	ldr	r5, [sp, #56]
	ldrb	r2, [r3, r5]
	movs	r3, #1
	ands	r3, r2
	mov	sl, r7
	cmp	r3, #0
	bne.n	.L20
	ldr	r7, [sp, #48]
	str	r7, [sp, #60]
.L20:
	ldr	r1, [sp, #56]
	movs	r2, #130
	ldr	r0, [sp, #76]
	lsls	r3, r1, #1
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r6, [r0, #16]
	ldrh	r0, [r0, r3]
	bl	Func_08077008
	ldr	r7, [pc, #736]
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	Func_08015270
	adds	r0, r5, #0
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #0
	bl	Func_08015090
	adds	r3, r5, r7
	ldrb	r0, [r3, #0]
	ldr	r3, [pc, #712]
	adds	r1, r6, #0
	adds	r0, r0, r3
	movs	r2, #0
	movs	r3, #8
	bl	Func_08015080
	ldr	r0, [pc, #704]
	adds	r1, r6, #0
	movs	r2, #48
	movs	r3, #0
	bl	Func_08015090
	ldr	r1, [sp, #72]
	ldrb	r0, [r5, #15]
	adds	r2, r6, #0
	str	r1, [sp, #0]
	movs	r3, #72
	movs	r1, #2
	bl	Func_080150b0
	ldr	r2, [sp, #80]
	cmp	r2, #0
	bne.n	.L21
	ldr	r0, [pc, #672]
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #16
	bl	Func_08015080
.L21:
	movs	r7, #1
	ldr	r3, [sp, #60]
	negs	r7, r7
	cmp	r3, r7
	beq.n	.L22
	ldr	r5, [sp, #56]
	lsls	r3, r5, #2
	adds	r3, r3, r5
	ldr	r0, [sp, #60]
	lsls	r3, r3, #1
	adds	r3, r3, r0
	lsls	r3, r3, #1
	mov	r1, r9
	ldrh	r3, [r1, r3]
	str	r3, [sp, #40]
.L22:
	ldr	r2, [sp, #76]
	movs	r3, #134
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldr	r0, [r2, #0]
	mov	r8, r2
	bl	Func_08015270
	ldr	r5, [sp, #80]
	cmp	r5, #1
	bne.n	.L23
	ldr	r0, [sp, #76]
	ldr	r1, [pc, #612]
	adds	r3, r0, r1
	ldrb	r0, [r3, #0]
	movs	r1, #1
	bl	Func_08015120
	ldr	r4, [pc, #604]
	mov	r2, r8
	adds	r0, r4, #0
	ldr	r1, [r2, #0]
	movs	r3, #0
	movs	r2, #0
	str	r4, [sp, #8]
	bl	Func_08015080
	movs	r5, #188
	ldr	r3, [sp, #76]
	lsls	r5, r5, #1
	adds	r6, r3, r5
	ldrh	r2, [r6, #0]
	movs	r5, #224
	adds	r3, r5, #0
	ands	r3, r2
	lsrs	r3, r3, #5
	lsls	r0, r3, #2
	adds	r0, r0, r3
	movs	r3, #31
	ands	r3, r2
	lsls	r0, r0, #2
	movs	r1, #150
	adds	r0, r0, r3
	lsls	r1, r1, #1
	adds	r0, r0, r1
	movs	r1, #4
	bl	Func_08015120
	ldrh	r3, [r6, #0]
	ands	r5, r3
	ldr	r3, [pc, #544]
	ldr	r1, [sp, #72]
	lsrs	r5, r5, #5
	mov	r2, r8
	adds	r5, r5, r3
	ldr	r0, [r2, #0]
	movs	r3, #0
	str	r1, [sp, #0]
	movs	r2, #6
	adds	r1, r5, #0
	bl	Func_08015280
	ldr	r4, [sp, #8]
	mov	r2, r8
	adds	r0, r4, #1
	ldr	r1, [r2, #0]
	movs	r3, #0
	movs	r2, #56
	bl	Func_08015080
	ldr	r4, [sp, #8]
	mov	r3, r8
	adds	r4, #2
	ldr	r1, [r3, #0]
	adds	r0, r4, #0
	movs	r2, #0
	movs	r3, #8
	bl	Func_08015080
.L23:
	ldr	r5, [sp, #60]
	cmp	r5, r7
	bne.n	.L24
	ldr	r0, [sp, #80]
	movs	r1, #0
	movs	r2, #200
	movs	r3, #0
	bl	Func_080ad5b4
	b.n	.L25
.L24:
	ldr	r7, [sp, #80]
	cmp	r7, #0
	beq.n	.L26
	b.n	.L27
.L26:
	ldr	r0, [sp, #36]
	cmp	r0, #0
	beq.n	.L28
	ldr	r1, [sp, #32]
	cmp	r1, #0
	bne.n	.L29
	mov	r2, r8
	ldr	r1, [r2, #0]
	ldr	r0, [pc, #444]
	movs	r2, #0
	movs	r3, #0
	bl	Func_08015080
	b.n	.L30
.L29:
	mov	r3, r8
	ldr	r1, [r3, #0]
	ldr	r0, [pc, #432]
	movs	r2, #0
	movs	r3, #0
	bl	Func_08015080
.L30:
	ldr	r5, [sp, #40]
	movs	r3, #240
	lsls	r3, r3, #4
	ands	r3, r5
	lsrs	r6, r3, #8
	ldr	r0, [sp, #40]
	movs	r3, #224
	ands	r3, r5
	movs	r7, #31
	ands	r7, r0
	lsrs	r5, r3, #5
	adds	r0, r6, #0
	adds	r1, r5, #0
	adds	r2, r7, #0
	bl	Func_08077210
	cmp	r0, #0
	bne.n	.L31
	adds	r0, r6, #0
	adds	r1, r5, #0
	adds	r2, r7, #0
	bl	Func_08077208
	cmp	r0, #0
	beq.n	.L32
.L31:
	adds	r0, r6, #0
	adds	r1, r5, #0
	adds	r2, r7, #0
	bl	Func_08077210
	cmp	r0, #0
	beq.n	.L33
	ldr	r0, [sp, #80]
	adds	r1, r5, #0
	movs	r2, #1
	bl	Func_080ad608
	b.n	.L34
.L33:
	ldr	r0, [sp, #80]
	adds	r1, r5, #0
	movs	r2, #2
	bl	Func_080ad608
.L34:
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #56]
	subs	r1, r2, r3
	lsls	r1, r1, #3
	adds	r1, #48
	ldr	r0, [sp, #80]
	movs	r2, #62
	movs	r3, #0
	bl	Func_080ad5b4
	b.n	.L35
.L32:
	ldr	r7, [sp, #76]
	movs	r1, #134
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r1, [r3, #0]
	ldr	r0, [pc, #308]
	movs	r3, #16
	movs	r2, #0
	bl	Func_08015080
	adds	r1, r5, #0
	movs	r2, #1
	ldr	r0, [sp, #80]
	bl	Func_080ad608
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #56]
	subs	r1, r2, r3
	lsls	r1, r1, #3
	adds	r1, #48
	ldr	r0, [sp, #80]
	movs	r2, #62
	movs	r3, #1
	bl	Func_080ad5b4
.L35:
	mov	r5, sl
	lsrs	r3, r5, #1
	b.n	.L36
.L28:
	ldr	r4, [pc, #268]
	mov	r7, r8
	adds	r0, r4, #0
	ldr	r1, [r7, #0]
	movs	r2, #0
	movs	r3, #0
	str	r4, [sp, #8]
	bl	Func_08015080
	movs	r3, #240
	ldr	r0, [sp, #40]
	lsls	r3, r3, #4
	ands	r3, r0
	lsrs	r5, r3, #8
	movs	r3, #224
	ands	r3, r0
	movs	r6, #31
	lsrs	r7, r3, #5
	ands	r6, r0
	adds	r1, r7, #0
	adds	r0, r5, #0
	adds	r2, r6, #0
	bl	Func_08077210
	ldr	r4, [sp, #8]
	cmp	r0, #0
	bne.n	.L37
	adds	r0, r5, #0
	adds	r1, r7, #0
	adds	r2, r6, #0
	bl	Func_08077208
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L38
.L37:
	adds	r0, r5, #0
	adds	r1, r7, #0
	adds	r2, r6, #0
	str	r4, [sp, #8]
	bl	Func_08077210
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L39
	mov	r2, r8
	ldr	r1, [r2, #0]
	adds	r0, r4, #3
	movs	r2, #0
	movs	r3, #16
	bl	Func_08015080
	movs	r0, #0
	adds	r1, r7, #0
	movs	r2, #1
	bl	Func_080ad608
	b.n	.L40
.L39:
	mov	r3, r8
	ldr	r1, [r3, #0]
	adds	r0, r4, #2
	movs	r2, #0
	movs	r3, #16
	bl	Func_08015080
	movs	r0, #0
	adds	r1, r7, #0
	movs	r2, #2
	bl	Func_080ad608
.L40:
	ldr	r5, [sp, #24]
	ldr	r7, [sp, #56]
	subs	r1, r5, r7
	lsls	r1, r1, #3
	adds	r1, #48
	ldr	r0, [sp, #80]
	movs	r2, #62
	movs	r3, #0
	bl	Func_080ad5b4
	b.n	.L41
.L38:
	mov	r2, r8
	adds	r0, r4, #4
	ldr	r1, [r2, #0]
	movs	r3, #16
	movs	r2, #0
	bl	Func_08015080
	adds	r1, r7, #0
	movs	r2, #1
	movs	r0, #0
	bl	Func_080ad608
	ldr	r3, [sp, #24]
	ldr	r5, [sp, #56]
	subs	r1, r3, r5
	lsls	r1, r1, #3
	adds	r1, #48
	movs	r0, #0
	movs	r2, #62
	movs	r3, #1
	bl	Func_080ad5b4
.L41:
	mov	r7, sl
	lsrs	r3, r7, #1
.L36:
	cmp	r3, #0
	beq.n	.L25
	ldr	r0, [sp, #80]
	movs	r1, #0
	bl	Func_080ad5f4
	b.n	.L25
	.4byte 0x00000bb1
	.4byte 0x00000219
	.4byte 0x00000129
	.4byte 0x00000741
	.4byte 0x080af28c
	.4byte 0x00000ba9
	.4byte 0x0000021a
	.4byte 0x00000bb2
	.4byte 0x00005001
	.4byte 0x00000b98
	.4byte 0x00000b99
	.4byte 0x00000b9e
	.4byte 0x00000b9a
.L27:
	ldr	r0, [sp, #40]
	movs	r3, #240
	lsls	r3, r3, #4
	ands	r3, r0
	lsrs	r6, r3, #8
	movs	r3, #224
	ands	r3, r0
	movs	r7, #31
	ands	r7, r0
	lsrs	r5, r3, #5
	adds	r0, r6, #0
	adds	r1, r5, #0
	adds	r2, r7, #0
	bl	Func_08077210
	cmp	r0, #0
	bne.n	.L42
	adds	r0, r6, #0
	adds	r1, r5, #0
	adds	r2, r7, #0
	bl	Func_08077208
	cmp	r0, #0
	beq.n	.L43
.L42:
	adds	r0, r6, #0
	adds	r1, r5, #0
	adds	r2, r7, #0
	bl	Func_08077210
	cmp	r0, #0
	beq.n	.L44
	ldr	r0, [sp, #80]
	adds	r1, r5, #0
	movs	r2, #1
	bl	Func_080ad608
	b.n	.L45
.L44:
	ldr	r0, [sp, #80]
	adds	r1, r5, #0
	movs	r2, #2
	bl	Func_080ad608
.L45:
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #56]
	subs	r1, r2, r3
	lsls	r1, r1, #3
	adds	r1, #48
	ldr	r0, [sp, #80]
	movs	r2, #54
	movs	r3, #0
	bl	Func_080ad5b4
	b.n	.L46
.L43:
	adds	r1, r5, #0
	movs	r2, #1
	ldr	r0, [sp, #80]
	bl	Func_080ad608
	ldr	r5, [sp, #24]
	ldr	r7, [sp, #56]
	subs	r1, r5, r7
	lsls	r1, r1, #3
	adds	r1, #48
	ldr	r0, [sp, #80]
	movs	r2, #54
	movs	r3, #1
	bl	Func_080ad5b4
.L46:
	mov	r0, sl
	lsrs	r3, r0, #1
	cmp	r3, #0
	beq.n	.L25
	ldr	r0, [sp, #80]
	movs	r1, #0
	bl	Func_080ad5f4
.L25:
	ldr	r1, [sp, #76]
	ldr	r0, [r1, #48]
	bl	Func_08015278
	movs	r3, #1
	ldr	r2, [sp, #60]
	negs	r3, r3
	cmp	r2, r3
	beq.n	.L47
	ldr	r5, [sp, #76]
	ldr	r0, [pc, #908]
	ldr	r1, [r5, #48]
	movs	r2, #0
	movs	r3, #80
	bl	Func_08015080
	movs	r3, #104
	ldr	r0, [r5, #48]
	movs	r1, #0
	str	r3, [sp, #0]
	movs	r2, #96
	movs	r3, #224
	bl	Func_08015068
	ldr	r7, [sp, #40]
	movs	r3, #224
	ands	r3, r7
	lsrs	r3, r3, #5
	lsls	r0, r3, #2
	adds	r0, r0, r3
	movs	r3, #31
	ands	r3, r7
	lsls	r0, r0, #2
	adds	r0, r0, r3
	ldr	r3, [pc, #864]
	ldr	r1, [r5, #48]
	adds	r0, r0, r3
	movs	r2, #0
	movs	r3, #96
	bl	Func_08015080
.L47:
	ldr	r0, [sp, #28]
	ldr	r1, [sp, #56]
	movs	r5, #1
	ldrb	r2, [r0, r1]
	adds	r3, r5, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L48
	ldr	r2, [sp, #76]
	ldr	r3, [sp, #24]
	ldr	r0, [r2, #48]
	ldr	r2, [sp, #48]
	subs	r1, r3, r1
	movs	r3, #14
	str	r3, [sp, #4]
	adds	r1, #1
	adds	r2, #2
	movs	r3, #6
	str	r5, [sp, #0]
	bl	Func_080ab1f4
.L48:
	ldr	r3, [pc, #812]
	ldr	r7, [pc, #816]
	ldr	r3, [r3, #0]
	adds	r3, r3, r7
	strb	r5, [r3, #0]
.L19:
	ldr	r0, [sp, #28]
	ldr	r1, [sp, #56]
	ldrb	r2, [r0, r1]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L49
	ldr	r2, [sp, #24]
	subs	r0, r2, r1
	lsls	r0, r0, #3
	subs	r0, #8
	movs	r1, #52
	bl	Func_080a1a40
	b.n	.L50
.L49:
	ldr	r3, [sp, #24]
	ldr	r5, [sp, #56]
	ldr	r7, [sp, #48]
	subs	r0, r3, r5
	lsls	r0, r0, #3
	lsls	r1, r7, #3
	subs	r0, #8
	adds	r1, #60
	bl	Func_080a1a40
.L50:
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [pc, #756]
	movs	r2, #128
	ldr	r3, [r3, #0]
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L51
	ldr	r3, [pc, #744]
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L52
.L51:
	ldr	r0, [sp, #36]
	cmp	r0, #0
	beq.n	.L53
	movs	r1, #1
	str	r1, [sp, #72]
.L53:
	movs	r2, #0
	str	r2, [sp, #36]
	str	r2, [sp, #32]
.L52:
	ldr	r3, [pc, #724]
	ldr	r3, [r3, #0]
	mov	fp, r3
	ldr	r3, [pc, #720]
	ldr	r4, [r3, #0]
	ldr	r3, [pc, #720]
	add	r3, r9
	ldr	r1, [r3, #0]
	cmp	r1, #0
	bne.n	.L54
	b.n	.L55
.L54:
	ldr	r2, [pc, #712]
	add	r2, r9
	ldr	r3, [r2, #0]
	adds	r3, #1
	movs	r4, #0
	str	r3, [r2, #0]
	subs	r3, r1, #1
	mov	fp, r4
	cmp	r3, #27
	bls.n	.L56
	b.n	.L55
.L56:
	ldr	r2, [pc, #696]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080abe82
	.4byte 0x080abede
	.4byte 0x080abefc
	.4byte 0x080abede
	.4byte 0x080ac1b8
	.4byte 0x080abf1c
	.4byte 0x080abf1c
	.4byte 0x080abede
	.4byte 0x080abede
	.4byte 0x080ac1b8
	.4byte 0x080ac1b8
	.4byte 0x080ac1b8
	.4byte 0x080abf78
	.4byte 0x080abf9a
	.4byte 0x080abff8
	.4byte 0x080abf9a
	.4byte 0x080abf9a
	.4byte 0x080ac160
	.4byte 0x080ac1b8
	.4byte 0x080ac160
	.4byte 0x080ac17e
	.4byte 0x080abede
	.4byte 0x080ac1b8
	.4byte 0x080ac19c
	.4byte 0x080ac1b8
	.4byte 0x080ac1b8
	.4byte 0x080abdcc
	.4byte 0x080abda0
	ldr	r1, [pc, #560]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L57
	adds	r6, r1, #0
	movs	r5, #1
.L58:
	movs	r0, #150
	movs	r1, #26
	bl	Func_080a1a40
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [r6, #0]
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L58
.L57:
	movs	r4, #2
	mov	fp, r4
	b.n	.L55
	ldr	r3, [pc, #524]
	add	r3, r9
	ldr	r3, [r3, #0]
	cmp	r3, #60
	beq.n	.L59
	b.n	.L55
.L59:
	movs	r2, #9
	movs	r3, #1
	ldr	r0, [pc, #516]
	movs	r1, #9
	bl	Func_08015038
	ldr	r2, [pc, #512]
	movs	r3, #131
	lsls	r3, r3, #2
	adds	r2, r2, r3
	adds	r5, r0, #0
	movs	r3, #1
	strb	r3, [r2, #0]
	b.n	.L60
.L61:
	movs	r0, #1
	bl	Func_080030f8
.L60:
	bl	Func_08015048
	cmp	r0, #0
	beq.n	.L61
	movs	r1, #1
	adds	r0, r5, #0
	bl	Func_08015018
	mov	r0, r9
	bl	Func_080aafb8
	movs	r0, #1
	bl	Func_080030f8
	movs	r2, #9
	movs	r3, #1
	ldr	r0, [pc, #464]
	movs	r1, #9
	bl	Func_08015038
	movs	r7, #131
	ldr	r2, [pc, #448]
	lsls	r7, r7, #2
	adds	r2, r2, r7
	movs	r3, #1
	adds	r5, r0, #0
	strb	r3, [r2, #0]
	b.n	.L62
.L63:
	movs	r0, #1
	bl	Func_080030f8
.L62:
	bl	Func_08015048
	cmp	r0, #0
	beq.n	.L63
	movs	r1, #1
	adds	r0, r5, #0
	bl	Func_08015018
	mov	r0, r9
	bl	Func_080aafb8
	ldr	r2, [pc, #396]
	movs	r3, #0
	add	r2, r9
	str	r3, [r2, #0]
	bl	Func_080b50f8
	bl	Func_080b50f8
	bl	Func_080b50f8
	movs	r1, #0
	movs	r2, #0
	movs	r0, #0
	bl	Func_080771b8
	movs	r1, #0
	movs	r2, #0
	movs	r0, #0
	bl	Func_080771c8
	movs	r0, #0
	bl	Func_08077010
	movs	r0, #2
	mov	fp, r0
	b.n	.L64
	ldr	r3, [pc, #344]
	add	r3, r9
	ldr	r3, [r3, #0]
	cmp	r3, #60
	beq.n	.L65
	b.n	.L55
.L65:
	movs	r1, #9
	movs	r2, #9
	movs	r3, #1
	ldr	r0, [pc, #344]
	str	r4, [sp, #8]
	bl	Func_08015038
	ldr	r2, [pc, #328]
	movs	r1, #131
	lsls	r1, r1, #2
	adds	r2, r2, r1
	movs	r3, #1
	adds	r5, r0, #0
	strb	r3, [r2, #0]
	b.n	.L66
.L67:
	movs	r0, #1
	str	r4, [sp, #8]
	bl	Func_080030f8
.L66:
	ldr	r4, [sp, #8]
	str	r4, [sp, #8]
	bl	Func_08015048
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L67
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08015018
	mov	r0, r9
	bl	Func_080aafb8
	ldr	r2, [pc, #264]
	movs	r3, #0
	add	r2, r9
	str	r3, [r2, #0]
	ldr	r2, [pc, #252]
	movs	r3, #2
	b.n	.L68
	ldr	r2, [pc, #252]
	add	r2, r9
	ldr	r3, [r2, #0]
	cmp	r3, #90
	beq.n	.L69
	b.n	.L55
.L69:
	movs	r3, #1
	mov	fp, r3
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r2, [pc, #228]
	add	r2, r9
	ldr	r3, [r2, #0]
	adds	r3, #1
	b.n	.L70
	ldr	r2, [pc, #220]
	add	r2, r9
	ldr	r3, [r2, #0]
	cmp	r3, #90
	beq.n	.L71
	b.n	.L55
.L71:
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r2, [pc, #200]
	movs	r5, #16
	add	r2, r9
	movs	r3, #4
	mov	fp, r5
	movs	r4, #16
	str	r3, [r2, #0]
	b.n	.L55
	ldr	r3, [pc, #188]
	add	r3, r9
	ldr	r3, [r3, #0]
	cmp	r3, #60
	beq.n	.L72
	b.n	.L55
.L72:
	movs	r1, #9
	movs	r2, #9
	movs	r3, #1
	ldr	r0, [pc, #196]
	str	r4, [sp, #8]
	bl	Func_08015038
	movs	r7, #131
	ldr	r2, [pc, #172]
	lsls	r7, r7, #2
	adds	r2, r2, r7
	movs	r3, #1
	adds	r5, r0, #0
	strb	r3, [r2, #0]
	b.n	.L73
.L74:
	movs	r0, #1
	str	r4, [sp, #8]
	bl	Func_080030f8
.L73:
	ldr	r4, [sp, #8]
	str	r4, [sp, #8]
	bl	Func_08015048
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L74
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08015018
	mov	r0, r9
	bl	Func_080aafb8
	ldr	r2, [pc, #112]
	movs	r3, #0
	add	r2, r9
	str	r3, [r2, #0]
	ldr	r2, [pc, #100]
	movs	r3, #8
	b.n	.L68
	ldr	r2, [pc, #96]
	add	r2, r9
	ldr	r3, [r2, #0]
	cmp	r3, #40
	beq.n	.L75
	b.n	.L55
.L75:
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r2, [pc, #76]
	add	r2, r9
	ldr	r3, [r2, #0]
	movs	r0, #2
	adds	r3, #1
	mov	fp, r0
	movs	r4, #2
	str	r3, [r2, #0]
	b.n	.L55
	ldr	r2, [pc, #64]
	add	r2, r9
	ldr	r3, [r2, #0]
	cmp	r3, #40
	beq.n	.L76
	b.n	.L55
.L76:
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r2, [pc, #44]
	add	r2, r9
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	b.n	.L55
	movs	r0, r0
	.4byte 0x00000bad
	.4byte 0x00000666
	.4byte 0x03001e8c
	.4byte 0x00000ea3
	.4byte 0x03001ae8
	.4byte 0x03001af8
	.4byte 0x03001b04
	.4byte 0x03001c94
	.4byte 0x0000212c
	.4byte 0x00002128
	.4byte 0x080abd30
	.4byte 0x00000c4c
	.4byte 0x02000240
	.4byte 0x00000c4d
	.4byte 0x00000c40
	.4byte 0x00000c41
	ldr	r3, [pc, #844]
	add	r3, r9
	ldr	r3, [r3, #0]
	cmp	r3, #60
	beq.n	.L77
	b.n	.L55
.L77:
	ldr	r3, [pc, #836]
	movs	r1, #131
	lsls	r1, r1, #2
	adds	r3, r3, r1
	movs	r2, #1
	strb	r2, [r3, #0]
	movs	r1, #9
	movs	r2, #9
	movs	r3, #1
	ldr	r0, [pc, #824]
	str	r4, [sp, #8]
	bl	Func_08015038
	movs	r1, #146
	adds	r5, r0, #0
	movs	r0, #2
	bl	Func_080a1ac0
	b.n	.L78
.L79:
	movs	r0, #1
	str	r4, [sp, #8]
	bl	Func_080030f8
.L78:
	ldr	r4, [sp, #8]
	str	r4, [sp, #8]
	bl	Func_08015048
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L79
	ldr	r1, [pc, #784]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L80
	adds	r7, r1, #0
	movs	r6, #1
.L81:
	movs	r0, #2
	movs	r1, #146
	str	r4, [sp, #8]
	bl	Func_080a1a40
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [r7, #0]
	ands	r3, r6
	ldr	r4, [sp, #8]
	cmp	r3, #0
	beq.n	.L81
.L80:
	movs	r1, #1
	adds	r0, r5, #0
	str	r4, [sp, #8]
	bl	Func_08015018
	mov	r0, r9
	bl	Func_080aafb8
	movs	r0, #1
	bl	Func_080030f8
	movs	r1, #9
	movs	r2, #9
	movs	r3, #1
	ldr	r0, [pc, #720]
	bl	Func_08015038
	adds	r5, r0, #0
	b.n	.L82
.L83:
	movs	r0, #1
	str	r4, [sp, #8]
	bl	Func_080030f8
.L82:
	ldr	r4, [sp, #8]
	str	r4, [sp, #8]
	bl	Func_08015048
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L83
	ldr	r1, [pc, #684]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L84
	adds	r7, r1, #0
	movs	r6, #1
.L85:
	movs	r0, #2
	movs	r1, #146
	str	r4, [sp, #8]
	bl	Func_080a1a40
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [r7, #0]
	ands	r3, r6
	ldr	r4, [sp, #8]
	cmp	r3, #0
	beq.n	.L85
.L84:
	movs	r1, #1
	adds	r0, r5, #0
	str	r4, [sp, #8]
	bl	Func_08015018
	mov	r0, r9
	bl	Func_080aafb8
	movs	r0, #1
	bl	Func_080030f8
	movs	r1, #9
	movs	r2, #9
	movs	r3, #1
	ldr	r0, [pc, #620]
	bl	Func_08015038
	adds	r5, r0, #0
	b.n	.L86
.L87:
	movs	r0, #1
	str	r4, [sp, #8]
	bl	Func_080030f8
.L86:
	ldr	r4, [sp, #8]
	str	r4, [sp, #8]
	bl	Func_08015048
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L87
	ldr	r1, [pc, #580]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L88
	adds	r7, r1, #0
	movs	r6, #1
.L89:
	movs	r0, #2
	movs	r1, #146
	str	r4, [sp, #8]
	bl	Func_080a1a40
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [r7, #0]
	ands	r3, r6
	ldr	r4, [sp, #8]
	cmp	r3, #0
	beq.n	.L89
.L88:
	movs	r1, #1
	adds	r0, r5, #0
	str	r4, [sp, #8]
	bl	Func_08015018
	mov	r0, r9
	bl	Func_080aafb8
	movs	r0, #1
	bl	Func_080030f8
	ldr	r2, [pc, #504]
	movs	r3, #0
	add	r2, r9
	str	r3, [r2, #0]
	ldr	r2, [pc, #520]
	movs	r3, #16
.L68:
	add	r2, r9
	str	r3, [r2, #0]
	ldr	r4, [sp, #8]
	b.n	.L55
	ldr	r2, [pc, #484]
	add	r2, r9
	ldr	r3, [r2, #0]
	cmp	r3, #90
	bne.n	.L55
	movs	r3, #1
	mov	fp, r3
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r2, [pc, #492]
	movs	r3, #21
	add	r2, r9
.L70:
	movs	r4, #1
	str	r3, [r2, #0]
	b.n	.L55
	ldr	r2, [pc, #456]
	add	r2, r9
	ldr	r3, [r2, #0]
	cmp	r3, #90
	bne.n	.L55
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r2, [pc, #464]
	movs	r5, #32
	add	r2, r9
	movs	r3, #22
	mov	fp, r5
	movs	r4, #32
	str	r3, [r2, #0]
	b.n	.L55
	ldr	r2, [pc, #424]
	add	r2, r9
	ldr	r3, [r2, #0]
	cmp	r3, #60
	bne.n	.L55
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r2, [pc, #436]
	movs	r3, #25
	add	r2, r9
	movs	r7, #2
	str	r3, [r2, #0]
	mov	fp, r7
.L64:
	movs	r4, #2
.L55:
	ldr	r0, [sp, #80]
	cmp	r0, #0
	beq.n	.L90
	b.n	.L91
.L90:
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r3, r4
	cmp	r3, #0
	bne.n	.L92
	b.n	.L93
.L92:
	movs	r2, #1
	ldr	r1, [sp, #60]
	negs	r2, r2
	cmp	r1, r2
	bne.n	.L94
	b.n	.L95
.L94:
	movs	r3, #0
	ldr	r5, [sp, #40]
	str	r3, [sp, #68]
	movs	r3, #240
	lsls	r3, r3, #4
	ands	r3, r5
	ldr	r0, [sp, #40]
	lsrs	r7, r3, #8
	movs	r3, #224
	ands	r3, r5
	movs	r6, #31
	lsrs	r5, r3, #5
	ands	r6, r0
	adds	r1, r5, #0
	adds	r0, r7, #0
	adds	r2, r6, #0
	str	r4, [sp, #8]
	bl	Func_08077210
	ldr	r4, [sp, #8]
	cmp	r0, #0
	bne.n	.L96
	adds	r0, r7, #0
	adds	r1, r5, #0
	adds	r2, r6, #0
	bl	Func_08077208
	ldr	r4, [sp, #8]
	cmp	r0, #0
	beq.n	.L97
.L96:
	movs	r1, #1
	str	r1, [sp, #68]
.L97:
	movs	r2, #1
	str	r2, [sp, #36]
	ldr	r2, [pc, #324]
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r3, [sp, #68]
	cmp	r3, #0
	bne.n	.L98
	movs	r0, #114
	bl	Func_080f9010
	ldr	r5, [sp, #76]
	ldr	r0, [r5, #48]
	bl	Func_08015278
	movs	r3, #104
	ldr	r0, [r5, #48]
	movs	r1, #0
	str	r3, [sp, #0]
	movs	r2, #80
	movs	r3, #216
	bl	Func_08015068
	ldr	r0, [pc, #288]
	ldr	r1, [r5, #48]
	movs	r2, #0
	movs	r3, #96
	bl	Func_08015078
	movs	r7, #1
	str	r7, [sp, #72]
	bl	.L99
.L98:
	ldr	r0, [sp, #40]
	lsrs	r3, r0, #15
	cmp	r3, #0
	beq.n	.L100
	movs	r0, #175
	str	r4, [sp, #8]
	bl	Func_080f9010
	movs	r5, #240
	ldr	r1, [sp, #40]
	lsls	r5, r5, #4
	movs	r6, #224
	ands	r5, r1
	ands	r6, r1
	movs	r3, #31
	ands	r3, r1
	lsrs	r5, r5, #8
	lsrs	r6, r6, #5
	adds	r2, r3, #0
	adds	r1, r6, #0
	adds	r0, r5, #0
	str	r3, [sp, #12]
	bl	Func_080771b8
	ldr	r3, [sp, #12]
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r3, #0
	bl	Func_080771c8
	b.n	.L101
.L100:
	movs	r0, #139
	str	r4, [sp, #8]
	bl	Func_080f9010
	movs	r5, #240
	ldr	r2, [sp, #40]
	lsls	r5, r5, #4
	movs	r6, #224
	ands	r5, r2
	ands	r6, r2
	movs	r3, #31
	ands	r3, r2
	lsrs	r5, r5, #8
	lsrs	r6, r6, #5
	adds	r2, r3, #0
	adds	r1, r6, #0
	adds	r0, r5, #0
	str	r3, [sp, #12]
	bl	Func_080771b0
	ldr	r3, [sp, #12]
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r3, #0
	bl	Func_080771c0
.L101:
	ldr	r4, [sp, #8]
	ldr	r3, [sp, #40]
	movs	r0, #240
	lsls	r0, r0, #4
	ands	r0, r3
	lsrs	r0, r0, #8
	str	r4, [sp, #8]
	bl	Func_08077010
	mov	r0, r9
	bl	Func_080aafb8
	movs	r5, #1
	str	r5, [sp, #72]
	ldr	r4, [sp, #8]
.L93:
	ldr	r7, [sp, #80]
	cmp	r7, #0
	bne.n	.L91
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L91
	movs	r0, #112
	movs	r5, #7
	bl	Func_080f9010
	b.n	.L102
.L91:
	movs	r3, #1
	ands	r3, r4
	cmp	r3, #0
	bne.n	.L103
	ldr	r0, [sp, #80]
	cmp	r0, #1
	beq.n	.L104
	b.n	.L105
.L104:
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r3, r4
	cmp	r3, #0
	bne.n	.L103
	b.n	.L105
.L103:
	movs	r1, #1
	ldr	r3, [sp, #28]
	str	r1, [sp, #68]
	ldr	r5, [sp, #56]
	ldrb	r2, [r3, r5]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L106
	movs	r0, #1
	ldr	r7, [sp, #60]
	negs	r0, r0
	cmp	r7, r0
	bne.n	.L107
.L95:
	movs	r0, #114
	bl	Func_080f9010
	bl	.L99
	movs	r0, r0
	.4byte 0x00002128
	.4byte 0x02000240
	.4byte 0x00000c44
	.4byte 0x03001c94
	.4byte 0x00000c45
	.4byte 0x00000c46
	.4byte 0x0000212c
	.4byte 0x03001af8
	.4byte 0x00000bbe
.L107:
	ldr	r2, [sp, #40]
	movs	r3, #240
	lsls	r3, r3, #4
	ands	r3, r2
	lsrs	r7, r3, #8
	movs	r3, #224
	ands	r3, r2
	movs	r5, #31
	movs	r1, #0
	lsrs	r6, r3, #5
	ands	r5, r2
	str	r1, [sp, #68]
	adds	r0, r7, #0
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl	Func_08077210
	cmp	r0, #0
	bne.n	.L108
	adds	r0, r7, #0
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl	Func_08077208
	cmp	r0, #0
	beq.n	.L106
.L108:
	movs	r3, #1
	str	r3, [sp, #68]
.L106:
	ldr	r5, [sp, #68]
	cmp	r5, #0
	bne.n	.L109
	movs	r0, #114
	bl	Func_080f9010
	ldr	r7, [sp, #76]
	ldr	r0, [r7, #48]
	bl	Func_08015278
	movs	r3, #104
	ldr	r0, [r7, #48]
	movs	r1, #0
	str	r3, [sp, #0]
	movs	r2, #80
	movs	r3, #216
	bl	Func_08015068
	ldr	r0, [pc, #828]
	ldr	r1, [r7, #48]
	movs	r2, #0
	movs	r3, #96
	bl	Func_08015078
	bl	.L99
.L109:
	ldr	r0, [sp, #80]
	cmp	r0, #1
	bne.n	.L110
	ldr	r3, [sp, #56]
	ldr	r1, [sp, #28]
	ldrb	r2, [r1, r3]
	adds	r3, r0, #0
	ands	r3, r2
	movs	r5, #4
	cmp	r3, #0
	beq.n	.L111
	ldr	r5, [sp, #76]
	ldr	r7, [sp, #56]
	movs	r3, #28
	ldrsb	r3, [r5, r3]
	cmp	r7, r3
	bne.n	.L112
	movs	r0, #188
	lsls	r0, r0, #1
	adds	r3, r5, r0
	ldrh	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #8
	ands	r3, r2
	movs	r5, #2
	cmp	r3, #0
	bne.n	.L111
	b.n	.L110
.L112:
	movs	r5, #3
	b.n	.L111
.L110:
	movs	r5, #1
.L111:
	movs	r0, #112
	bl	Func_080f9010
	b.n	.L102
.L105:
	movs	r3, #8
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L113
	movs	r0, #113
	movs	r5, #2
	b.n	.L114
.L113:
	movs	r3, #2
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L115
	movs	r0, #113
	movs	r5, #1
.L114:
	bl	Func_080f9010
	negs	r5, r5
	b.n	.L102
.L115:
	ldr	r1, [sp, #80]
	cmp	r1, #0
	beq.n	.L116
	b.n	.L117
.L116:
	movs	r3, #4
	ands	r3, r4
	cmp	r3, #0
	bne.n	.L118
	b.n	.L117
.L118:
	ldr	r2, [sp, #36]
	cmp	r2, #0
	bne.n	.L119
	b.n	.L120
.L119:
	ldr	r5, [sp, #32]
	movs	r3, #1
	eors	r5, r3
	str	r5, [sp, #32]
	cmp	r5, #0
	beq.n	.L121
	movs	r0, #139
	bl	Func_080f9010
	b.n	.L122
.L121:
	movs	r0, #175
	bl	Func_080f9010
.L122:
	movs	r7, #0
	ldr	r0, [sp, #76]
	ldr	r1, [pc, #656]
	str	r7, [sp, #68]
	adds	r3, r0, r1
	ldrb	r3, [r3, #0]
	cmp	r7, r3
	blt.n	.L123
	b.n	.L124
.L123:
	movs	r2, #160
	movs	r3, #0
	str	r2, [sp, #20]
	str	r3, [sp, #16]
.L131:
	movs	r5, #0
	str	r5, [sp, #64]
	ldr	r7, [sp, #20]
	mov	r0, r9
	ldrsb	r3, [r7, r0]
	cmp	r5, r3
	bge.n	.L125
	ldr	r1, [sp, #16]
	lsls	r3, r1, #1
	add	r3, r9
	mov	sl, r3
.L130:
	mov	r2, sl
	ldrh	r7, [r2, #0]
	movs	r3, #2
	movs	r0, #240
	lsls	r0, r0, #4
	add	sl, r3
	adds	r3, r7, #0
	ands	r3, r0
	lsrs	r4, r3, #8
	movs	r5, #0
	movs	r1, #224
	adds	r3, r7, #0
	ands	r3, r1
	movs	r2, #31
	mov	r8, r5
	adds	r5, r7, #0
	lsrs	r6, r3, #5
	ands	r5, r2
	adds	r0, r4, #0
	adds	r1, r6, #0
	adds	r2, r5, #0
	str	r4, [sp, #8]
	bl	Func_08077210
	ldr	r4, [sp, #8]
	cmp	r0, #0
	bne.n	.L126
	adds	r0, r4, #0
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl	Func_08077208
	cmp	r0, #0
	beq.n	.L127
.L126:
	movs	r3, #1
	mov	r8, r3
.L127:
	mov	r5, r8
	cmp	r5, #0
	beq.n	.L128
	ldr	r0, [sp, #32]
	cmp	r0, #0
	beq.n	.L129
	lsrs	r3, r7, #15
	cmp	r3, #0
	bne.n	.L128
	movs	r1, #240
	lsls	r1, r1, #4
	movs	r2, #224
	adds	r5, r7, #0
	adds	r6, r7, #0
	ands	r5, r1
	ands	r6, r2
	movs	r3, #31
	ands	r3, r7
	lsrs	r5, r5, #8
	lsrs	r6, r6, #5
	adds	r2, r3, #0
	adds	r1, r6, #0
	adds	r0, r5, #0
	str	r3, [sp, #12]
	bl	Func_080771b0
	ldr	r3, [sp, #12]
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r3, #0
	bl	Func_080771c0
	adds	r0, r5, #0
	bl	Func_08077010
	b.n	.L128
.L129:
	lsrs	r3, r7, #15
	cmp	r3, #0
	beq.n	.L128
	movs	r3, #240
	lsls	r3, r3, #4
	movs	r0, #224
	adds	r5, r7, #0
	adds	r6, r7, #0
	ands	r5, r3
	ands	r6, r0
	movs	r3, #31
	ands	r3, r7
	lsrs	r5, r5, #8
	lsrs	r6, r6, #5
	adds	r2, r3, #0
	adds	r1, r6, #0
	adds	r0, r5, #0
	str	r3, [sp, #12]
	bl	Func_080771b8
	ldr	r3, [sp, #12]
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, r3, #0
	bl	Func_080771c8
	adds	r0, r5, #0
	bl	Func_08077010
.L128:
	ldr	r1, [sp, #64]
	adds	r1, #1
	str	r1, [sp, #64]
	ldr	r2, [sp, #20]
	mov	r5, r9
	ldrsb	r3, [r2, r5]
	cmp	r1, r3
	blt.n	.L130
.L125:
	ldr	r7, [sp, #20]
	ldr	r0, [sp, #16]
	ldr	r1, [sp, #68]
	adds	r7, #1
	adds	r0, #10
	adds	r1, #1
	ldr	r2, [sp, #76]
	ldr	r5, [pc, #380]
	str	r7, [sp, #20]
	str	r0, [sp, #16]
	str	r1, [sp, #68]
	adds	r3, r2, r5
	ldrb	r3, [r3, #0]
	cmp	r1, r3
	bge.n	.L124
	b.n	.L131
.L124:
	mov	r0, r9
	bl	Func_080aafb8
	movs	r7, #1
	str	r7, [sp, #72]
	b.n	.L117
.L120:
	ldr	r1, [sp, #76]
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #56]
	ldr	r0, [r1, #48]
	subs	r1, r2, r3
	ldr	r2, [sp, #48]
	movs	r3, #1
	str	r3, [sp, #0]
	movs	r3, #15
	str	r3, [sp, #4]
	adds	r1, #1
	adds	r2, #2
	movs	r3, #6
	bl	Func_080ab1f4
	movs	r0, #112
	movs	r5, #10
	bl	Func_080f9010
	b.n	.L102
.L117:
	movs	r3, #64
	mov	r5, fp
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L132
	movs	r0, #111
	bl	Func_080f9010
	ldr	r7, [sp, #28]
	ldr	r0, [sp, #56]
	movs	r5, #4
	ldrb	r2, [r7, r0]
	adds	r3, r5, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L133
	bl	.L99
.L133:
	ldr	r1, [sp, #76]
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #56]
	ldr	r0, [r1, #48]
	subs	r1, r2, r3
	ldr	r2, [sp, #48]
	movs	r3, #15
	adds	r2, #2
	str	r3, [sp, #4]
	adds	r1, #1
	movs	r3, #6
	movs	r6, #1
	str	r6, [sp, #0]
	bl	Func_080ab1f4
	ldr	r0, [sp, #56]
	ldrb	r2, [r7, r0]
	adds	r3, r5, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L134
	bl	.L99
.L134:
	adds	r3, r6, #0
	ands	r3, r2
	movs	r1, #1
	cmp	r3, #0
	beq.n	.L135
	movs	r3, #2
	negs	r3, r3
	ands	r3, r2
	movs	r1, #0
	strb	r3, [r7, r0]
	str	r1, [sp, #48]
	b.n	.L136
.L135:
	ldr	r3, [sp, #48]
	cmp	r3, #0
	bne.n	.L136
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L136
	adds	r3, r2, #0
	ldr	r5, [sp, #28]
	ldr	r7, [sp, #56]
	orrs	r3, r1
	movs	r0, #2
	strb	r3, [r5, r7]
	str	r0, [sp, #72]
	bl	.L99
.L136:
	ldr	r1, [sp, #48]
	ldr	r3, [sp, #56]
	subs	r1, #1
	str	r1, [sp, #48]
	adds	r3, #160
	mov	r2, r9
	ldrsb	r1, [r2, r3]
	cmp	r1, #0
	bne.n	.L137
	movs	r1, #1
.L137:
	ldr	r0, [sp, #48]
	bl	Func_080aa538
	movs	r3, #2
	str	r0, [sp, #48]
	str	r0, [sp, #44]
	str	r3, [sp, #72]
	bl	.L99
.L132:
	movs	r3, #128
	mov	r5, fp
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L138
	movs	r0, #111
	bl	Func_080f9010
	ldr	r7, [sp, #28]
	ldr	r0, [sp, #56]
	movs	r5, #4
	ldrb	r2, [r7, r0]
	adds	r3, r5, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L139
	bl	.L99
.L139:
	ldr	r1, [sp, #76]
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #56]
	ldr	r0, [r1, #48]
	subs	r1, r2, r3
	ldr	r2, [sp, #48]
	movs	r3, #15
	adds	r1, #1
	str	r3, [sp, #4]
	adds	r2, #2
	movs	r3, #6
	movs	r6, #1
	str	r6, [sp, #0]
	bl	Func_080ab1f4
	ldr	r7, [sp, #48]
	ldr	r3, [sp, #56]
	adds	r7, #1
	str	r7, [sp, #48]
	adds	r3, #160
	mov	r0, r9
	ldrsb	r1, [r0, r3]
	cmp	r1, #0
	bne.n	.L140
	movs	r1, #1
.L140:
	ldr	r0, [sp, #48]
	bl	Func_080aa538
	ldr	r3, [sp, #56]
	str	r0, [sp, #48]
	ldr	r1, [sp, #28]
	ldrb	r2, [r1, r3]
	adds	r3, r6, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L141
	adds	r3, r5, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L141
	movs	r3, #2
	negs	r3, r3
	ldr	r5, [sp, #56]
	ands	r3, r2
	movs	r7, #0
	strb	r3, [r1, r5]
	str	r7, [sp, #48]
	b.n	.L142
	movs	r0, r0
	.4byte 0x00000bbe
	.4byte 0x00000219
.L141:
	ldr	r0, [sp, #48]
	cmp	r0, #0
	bne.n	.L142
	ldr	r3, [sp, #56]
	ldr	r1, [sp, #28]
	ldrb	r2, [r1, r3]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L142
	movs	r3, #1
	ldr	r5, [sp, #56]
	orrs	r3, r2
	strb	r3, [r1, r5]
.L142:
	ldr	r7, [sp, #48]
	movs	r0, #2
	str	r7, [sp, #44]
	str	r0, [sp, #72]
	bl	.L99
.L138:
	movs	r3, #32
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L143
	movs	r0, #111
	bl	Func_080f9010
	ldr	r3, [sp, #28]
	ldr	r5, [sp, #56]
	ldrb	r2, [r3, r5]
	movs	r3, #4
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L144
	ldr	r2, [sp, #24]
	ldr	r7, [sp, #76]
	subs	r1, r2, r5
	movs	r3, #1
	ldr	r2, [sp, #48]
	ldr	r0, [r7, #48]
	str	r3, [sp, #0]
	movs	r3, #15
	str	r3, [sp, #4]
	adds	r1, #1
	adds	r2, #2
	movs	r3, #6
	bl	Func_080ab1f4
.L144:
	ldr	r3, [sp, #56]
	ldr	r7, [sp, #76]
	subs	r3, #1
	ldr	r0, [pc, #380]
	str	r3, [sp, #56]
	adds	r5, r7, r0
	ldrb	r1, [r5, #0]
	adds	r0, r3, #0
	bl	Func_080aa538
	ldr	r1, [sp, #80]
	str	r0, [sp, #56]
	cmp	r1, #0
	bne.n	.L145
	movs	r2, #0
	str	r2, [sp, #68]
	ldrb	r3, [r5, #0]
	cmp	r1, r3
	bge.n	.L145
.L147:
	ldr	r7, [sp, #28]
	ldr	r0, [sp, #56]
	ldrsb	r3, [r7, r0]
	cmp	r3, #4
	bne.n	.L146
	subs	r0, #1
	str	r0, [sp, #56]
	ldrb	r1, [r5, #0]
	bl	Func_080aa538
	str	r0, [sp, #56]
.L146:
	ldr	r1, [sp, #68]
	adds	r1, #1
	str	r1, [sp, #68]
	ldrb	r3, [r5, #0]
	cmp	r1, r3
	blt.n	.L147
	b.n	.L145
.L143:
	movs	r3, #16
	mov	r1, fp
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L148
	bl	.L99
.L148:
	movs	r0, #111
	bl	Func_080f9010
	ldr	r3, [sp, #28]
	ldr	r5, [sp, #56]
	ldrb	r2, [r3, r5]
	movs	r3, #4
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L149
	ldr	r2, [sp, #24]
	ldr	r7, [sp, #76]
	subs	r1, r2, r5
	movs	r3, #1
	ldr	r2, [sp, #48]
	ldr	r0, [r7, #48]
	str	r3, [sp, #0]
	movs	r3, #15
	str	r3, [sp, #4]
	adds	r1, #1
	adds	r2, #2
	movs	r3, #6
	bl	Func_080ab1f4
.L149:
	ldr	r3, [sp, #56]
	ldr	r7, [sp, #76]
	adds	r3, #1
	ldr	r0, [pc, #244]
	str	r3, [sp, #56]
	adds	r5, r7, r0
	ldrb	r1, [r5, #0]
	adds	r0, r3, #0
	bl	Func_080aa538
	ldr	r1, [sp, #80]
	str	r0, [sp, #56]
	cmp	r1, #0
	bne.n	.L145
	movs	r2, #0
	str	r2, [sp, #68]
	ldrb	r3, [r5, #0]
	cmp	r1, r3
	bge.n	.L145
.L151:
	ldr	r7, [sp, #28]
	ldr	r0, [sp, #56]
	ldrsb	r3, [r7, r0]
	cmp	r3, #4
	bne.n	.L150
	adds	r0, #1
	str	r0, [sp, #56]
	ldrb	r1, [r5, #0]
	bl	Func_080aa538
	str	r0, [sp, #56]
.L150:
	ldr	r1, [sp, #68]
	adds	r1, #1
	str	r1, [sp, #68]
	ldrb	r3, [r5, #0]
	cmp	r1, r3
	blt.n	.L151
.L145:
	ldr	r2, [sp, #44]
	ldr	r3, [sp, #56]
	str	r2, [sp, #48]
	adds	r3, #160
	mov	r5, r9
	ldrsb	r1, [r5, r3]
	cmp	r1, #0
	bne.n	.L152
	movs	r1, #1
.L152:
	ldr	r0, [sp, #48]
	bl	Func_080aa538
	str	r0, [sp, #48]
	ldr	r0, [sp, #56]
	movs	r7, #2
	lsls	r0, r0, #3
	str	r7, [sp, #72]
	str	r0, [sp, #24]
	bl	.L99
.L102:
	ldr	r3, [sp, #80]
	add	r1, sp, #56
	ldrb	r2, [r1, #0]
	ldr	r1, [sp, #76]
	adds	r3, #28
	strb	r2, [r1, r3]
	ldr	r2, [sp, #60]
	movs	r3, #1
	negs	r3, r3
	cmp	r2, r3
	beq.n	.L153
	ldr	r2, [sp, #56]
	ldr	r7, [sp, #52]
	movs	r0, #188
	lsls	r0, r0, #1
	lsls	r3, r2, #2
	adds	r1, r7, r0
	adds	r3, r3, r2
	ldr	r7, [sp, #60]
	lsls	r3, r3, #1
	adds	r3, r3, r7
	lsls	r3, r3, #1
	mov	r0, r9
	ldrh	r2, [r0, r3]
	ldr	r3, [sp, #76]
	strh	r2, [r3, r1]
	ldr	r7, [sp, #80]
	movs	r1, #149
	lsls	r1, r1, #2
	adds	r0, r7, r1
	movs	r3, #31
	ldr	r7, [sp, #76]
	ands	r3, r2
	strb	r3, [r7, r0]
	movs	r3, #224
	adds	r1, r7, #0
	ands	r3, r2
	adds	r1, #2
	lsrs	r3, r3, #5
	strb	r3, [r1, r0]
	ldr	r0, [sp, #80]
	movs	r3, #150
	lsls	r3, r3, #2
	adds	r1, r0, r3
	movs	r3, #240
	lsls	r3, r3, #4
	ands	r3, r2
	lsrs	r3, r3, #8
	strb	r3, [r7, r1]
.L153:
	ldr	r1, [sp, #48]
	ldr	r7, [sp, #52]
	movs	r0, #186
	lsls	r0, r0, #1
	lsls	r3, r1, #2
	adds	r2, r7, r0
	adds	r3, r3, r1
	ldr	r7, [sp, #56]
	ldr	r0, [sp, #76]
	lsls	r3, r3, #1
	adds	r3, r7, r3
	strh	r3, [r0, r2]
	add	sp, #108
	adds	r0, r5, #0
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x00000219

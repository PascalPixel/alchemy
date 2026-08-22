@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800cacc
	.thumb_func
Func_0800cacc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #676]
	ldr	r6, [r3, #0]
	adds	r1, r6, #0
	adds	r2, r6, #0
	sub	sp, #48
	movs	r0, #63
	adds	r1, #85
	adds	r2, #86
	str	r0, [sp, #32]
	str	r1, [sp, #4]
	str	r2, [sp, #0]
.L63:
	movs	r3, #0
	str	r3, [sp, #12]
	ldr	r2, [r6, #0]
	cmp	r2, #0
	bne.n	.L0
	b.n	.L1
.L0:
	ldr	r3, [r6, #108]
	cmp	r3, #0
	beq.n	.L2
	adds	r0, r6, #0
	bl	Func_080072f0
	ldr	r2, [r6, #0]
.L2:
	cmp	r2, #0
	bne.n	.L3
	b.n	.L1
.L3:
	ldr	r4, [sp, #4]
	ldrb	r3, [r4, #6]
	cmp	r3, #0
	beq.n	.L4
	b.n	.L1
.L4:
	ldr	r0, [sp, #0]
	movs	r4, #8
	ldrsh	r3, [r0, r4]
	ldrh	r1, [r0, #8]
	cmp	r3, #0
	beq.n	.L5
	subs	r3, r1, #1
	strh	r3, [r0, #8]
	b.n	.L6
.L5:
	ldr	r5, [pc, #596]
	b.n	.L7
.L8:
	adds	r3, r1, #1
	strh	r3, [r6, #4]
.L9:
	ldr	r2, [r6, #0]
.L7:
	movs	r4, #4
	ldrsh	r3, [r6, r4]
	lsls	r3, r3, #2
	ldr	r3, [r2, r3]
	ldrh	r1, [r6, #4]
	cmp	r3, #63
	bhi.n	.L8
	lsls	r3, r3, #2
	ldr	r3, [r5, r3]
	adds	r0, r6, #0
	bl	Func_080072f0
	cmp	r0, #0
	bne.n	.L9
	ldr	r3, [r6, #0]
	cmp	r3, #0
	bne.n	.L6
	b.n	.L1
.L6:
	ldr	r0, [r6, #8]
	str	r0, [sp, #28]
	ldr	r1, [r6, #12]
	str	r1, [sp, #24]
	ldr	r2, [r6, #16]
	ldr	r4, [sp, #4]
	str	r2, [sp, #20]
	ldrb	r3, [r4, #12]
	cmp	r3, #0
	beq.n	.L10
	b.n	.L11
.L10:
	str	r4, [sp, #8]
	ldrb	r3, [r4, #0]
	cmp	r3, #0
	beq.n	.L12
	b.n	.L13
.L12:
	movs	r0, #128
	ldr	r3, [r6, #56]
	lsls	r0, r0, #24
	cmp	r3, r0
	bne.n	.L14
	b.n	.L15
.L14:
	ldr	r1, [sp, #28]
	subs	r0, r3, r1
	cmp	r0, #0
	bge.n	.L16
	ldr	r2, [pc, #504]
	adds	r0, r0, r2
.L16:
	ldr	r3, [r6, #60]
	ldr	r4, [sp, #24]
	asrs	r0, r0, #16
	mov	r8, r0
	subs	r0, r3, r4
	cmp	r0, #0
	bge.n	.L17
	ldr	r1, [pc, #484]
	adds	r0, r0, r1
.L17:
	ldr	r3, [r6, #64]
	ldr	r2, [sp, #20]
	asrs	r0, r0, #16
	mov	fp, r0
	subs	r0, r3, r2
	cmp	r0, #0
	bge.n	.L18
	ldr	r3, [pc, #468]
	adds	r0, r0, r3
.L18:
	asrs	r0, r0, #16
	mov	r9, r0
	mov	r4, r8
	mov	r1, fp
	mov	r0, r8
	muls	r0, r4
	mov	r3, fp
	muls	r3, r1
	mov	r4, r9
	mov	r2, r9
	muls	r2, r4
	adds	r0, r0, r3
	adds	r0, r0, r2
	ldr	r3, [pc, #440]
	bl	Func_080072f0
	cmp	r0, #0
	bne.n	.L19
	ldr	r0, [r6, #56]
	str	r0, [sp, #28]
	ldr	r1, [r6, #60]
	str	r1, [sp, #24]
	ldr	r2, [r6, #64]
	str	r2, [sp, #20]
	b.n	.L11
.L19:
	ldr	r1, [r6, #52]
	ldr	r3, [pc, #420]
	lsls	r0, r0, #16
	bl	Func_080072f0
	adds	r5, r0, #0
	mov	r3, r8
	muls	r3, r5
	ldr	r0, [r6, #36]
	adds	r0, r0, r3
	mov	r3, fp
	muls	r3, r5
	mov	r2, r9
	muls	r2, r5
	ldr	r1, [r6, #40]
	adds	r1, r1, r3
	ldr	r3, [r6, #44]
	ldr	r4, [pc, #392]
	adds	r3, r3, r2
	mov	sl, r0
	str	r1, [r6, #40]
	adds	r7, r1, #0
	str	r0, [r6, #36]
	str	r3, [r6, #44]
	mov	r9, r3
	mov	r8, r4
	mov	r1, sl
	movs	r0, r0
	mov	ip, pc
	bx	r8
	adds	r3, r0, #0
	adds	r1, r7, #0
	adds	r0, r7, #0
	movs	r0, r0
	mov	ip, pc
	bx	r8
	adds	r4, r0, #0
	mov	r1, r9
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r8
	adds	r3, r3, r4
	adds	r3, r3, r0
	adds	r0, r3, #0
	bl	Func_080045d4
	ldr	r1, [r6, #48]
	cmp	r0, r1
	bgt.n	.L20
	b.n	.L11
.L20:
	ldr	r2, [pc, #320]
	bl	Func_080072ec
	adds	r5, r0, #0
	adds	r1, r5, #0
	mov	r0, sl
	movs	r0, r0
	mov	ip, pc
	bx	r8
	str	r0, [r6, #36]
	adds	r1, r5, #0
	adds	r0, r7, #0
	movs	r0, r0
	mov	ip, pc
	bx	r8
	str	r0, [r6, #40]
	adds	r1, r5, #0
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r8
	str	r0, [r6, #44]
	b.n	.L11
.L15:
	ldr	r3, [r6, #36]
	ldr	r0, [r6, #44]
	ldr	r4, [r6, #40]
	mov	r8, r3
	mov	r9, r0
	mov	fp, r4
	ldr	r7, [pc, #264]
	mov	r0, r8
	mov	r1, r8
	movs	r0, r0
	mov	ip, pc
	bx	r7
	adds	r3, r0, #0
	mov	r1, fp
	mov	r0, fp
	movs	r0, r0
	mov	ip, pc
	bx	r7
	adds	r4, r0, #0
	mov	r1, r9
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r7
	adds	r3, r3, r4
	adds	r3, r3, r0
	adds	r0, r3, #0
	bl	Func_080045d4
	cmp	r0, #0
	beq.n	.L21
	ldr	r3, [r6, #52]
	subs	r1, r0, r3
	cmp	r1, #0
	bge.n	.L22
	movs	r1, #0
.L22:
	ldr	r3, [pc, #200]
	bl	Func_080072f0
	adds	r5, r0, #0
	adds	r1, r5, #0
	mov	r0, r8
	mov	ip, pc
	bx	r7
	str	r0, [r6, #36]
	adds	r1, r5, #0
	mov	r0, fp
	movs	r0, r0
	mov	ip, pc
	bx	r7
	str	r0, [r6, #40]
	adds	r1, r5, #0
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r7
	str	r0, [r6, #44]
	b.n	.L11
.L21:
	str	r0, [r6, #36]
	str	r0, [r6, #40]
	str	r0, [r6, #44]
	b.n	.L11
.L13:
	movs	r1, #128
	ldr	r3, [r6, #56]
	lsls	r1, r1, #24
	cmp	r3, r1
	bne.n	.L23
	b.n	.L24
.L23:
	ldr	r2, [sp, #28]
	subs	r0, r3, r2
	cmp	r0, #0
	bge.n	.L25
	ldr	r3, [pc, #120]
	adds	r0, r0, r3
.L25:
	ldr	r3, [r6, #64]
	ldr	r4, [sp, #20]
	asrs	r0, r0, #16
	mov	r8, r0
	subs	r0, r3, r4
	cmp	r0, #0
	bge.n	.L26
	ldr	r1, [pc, #104]
	adds	r0, r0, r1
.L26:
	asrs	r0, r0, #16
	mov	r9, r0
	mov	r2, r8
	mov	r4, r9
	mov	r3, r9
	muls	r3, r4
	mov	r0, r8
	muls	r0, r2
	adds	r0, r0, r3
	ldr	r3, [pc, #84]
	bl	Func_080072f0
	ldr	r1, [pc, #92]
	lsls	r0, r0, #16
	cmp	r0, r1
	bgt.n	.L27
	ldr	r3, [r6, #56]
	ldr	r2, [sp, #28]
	ldr	r4, [sp, #20]
	subs	r2, r3, r2
	ldr	r3, [r6, #64]
	mov	r8, r2
	subs	r4, r3, r4
	mov	r9, r4
	mov	r0, r8
	ldr	r4, [pc, #60]
	mov	r1, r8
	mov	ip, pc
	bx	r4
	adds	r3, r0, #0
	mov	r1, r9
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r4
	adds	r3, r3, r0
	adds	r0, r3, #0
	bl	Func_080045d4
.L27:
	cmp	r0, #0
	bne.n	.L28
	ldr	r0, [r6, #56]
	str	r0, [sp, #28]
	ldr	r1, [r6, #64]
	str	r1, [sp, #20]
	b.n	.L29
	movs	r0, r0
	.4byte 0x03001e64
	.4byte 0x08013624
	.4byte 0x0000ffff
	.4byte 0x030001d8
	.4byte 0x0300013c
	.4byte 0x03000118
	.4byte 0x00ffffff
.L28:
	ldr	r2, [pc, #688]
	ldr	r1, [r6, #52]
	mov	fp, r2
	bl	Func_08007310
	adds	r5, r0, #0
	ldr	r7, [pc, #680]
	mov	r0, r8
	adds	r1, r5, #0
	movs	r0, r0
	mov	ip, pc
	bx	r7
	ldr	r4, [r6, #36]
	adds	r4, r4, r0
	str	r4, [r6, #36]
	mov	r0, r9
	adds	r1, r5, #0
	movs	r0, r0
	mov	ip, pc
	bx	r7
	ldr	r3, [r6, #44]
	mov	sl, r4
	adds	r3, r3, r0
	str	r3, [r6, #44]
	mov	r9, r3
	mov	r0, sl
	mov	r1, sl
	movs	r0, r0
	mov	ip, pc
	bx	r7
	adds	r3, r0, #0
	mov	r1, r9
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r7
	adds	r3, r3, r0
	adds	r0, r3, #0
	bl	Func_080045d4
	ldr	r1, [r6, #48]
	cmp	r0, r1
	ble.n	.L29
	bl	Func_08007310
	adds	r5, r0, #0
	adds	r1, r5, #0
	mov	r0, sl
	mov	ip, pc
	bx	r7
	str	r0, [r6, #36]
	adds	r1, r5, #0
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r7
	b.n	.L30
.L24:
	ldr	r3, [r6, #36]
	ldr	r4, [r6, #44]
	mov	r8, r3
	mov	r9, r4
	ldr	r7, [pc, #572]
	mov	r0, r8
	mov	r1, r8
	mov	ip, pc
	bx	r7
	adds	r3, r0, #0
	mov	r1, r9
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r7
	adds	r3, r3, r0
	adds	r0, r3, #0
	bl	Func_080045d4
	cmp	r0, #0
	beq.n	.L31
	ldr	r3, [r6, #52]
	subs	r1, r0, r3
	cmp	r1, #0
	bge.n	.L32
	movs	r1, #0
.L32:
	ldr	r3, [pc, #524]
	bl	Func_080072f0
	adds	r5, r0, #0
	adds	r1, r5, #0
	mov	r0, r8
	movs	r0, r0
	mov	ip, pc
	bx	r7
	str	r0, [r6, #36]
	adds	r1, r5, #0
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r7
	b.n	.L30
.L31:
	str	r0, [r6, #36]
.L30:
	str	r0, [r6, #44]
.L29:
	ldr	r0, [sp, #8]
	ldrb	r2, [r0, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L33
	b.n	.L34
.L33:
	ldr	r3, [r6, #36]
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #20]
	adds	r1, r1, r3
	ldr	r3, [r6, #44]
	adds	r2, r2, r3
	adds	r3, r6, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	mov	sl, r1
	mov	r9, r2
	bl	Func_08011f54
	str	r0, [sp, #16]
	ldr	r4, [sp, #24]
	ldr	r3, [r6, #20]
	subs	r7, r0, r3
	subs	r3, r0, r4
	ldr	r0, [pc, #444]
	cmp	r3, r0
	ble.n	.L35
	adds	r4, r4, r7
	str	r4, [sp, #24]
.L35:
	cmp	r7, #0
	bge.n	.L36
	negs	r7, r7
.L36:
	ldr	r3, [r6, #52]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	cmp	r7, r3
	ble.n	.L37
	adds	r7, r3, #0
.L37:
	lsls	r3, r7, #1
	adds	r7, r3, r7
	cmp	r7, #0
	beq.n	.L38
	ldr	r1, [sp, #8]
	ldrb	r2, [r1, #0]
	movs	r3, #16
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L38
	ldr	r2, [r6, #36]
	ldr	r0, [pc, #388]
	ldr	r3, [r6, #40]
	ldr	r4, [r6, #44]
	mov	r8, r2
	mov	sl, r0
	mov	fp, r3
	mov	r9, r4
	mov	r0, r8
	mov	r1, r8
	movs	r0, r0
	mov	ip, pc
	bx	sl
	adds	r3, r0, #0
	mov	r1, fp
	mov	r0, fp
	movs	r0, r0
	mov	ip, pc
	bx	sl
	adds	r4, r0, #0
	mov	r1, r9
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	sl
	adds	r3, r3, r4
	adds	r3, r3, r0
	adds	r0, r3, #0
	bl	Func_080045d4
	cmp	r0, #0
	beq.n	.L38
	subs	r1, r0, r7
	cmp	r1, #0
	bge.n	.L39
	movs	r1, #0
.L39:
	ldr	r3, [pc, #316]
	bl	Func_080072f0
	adds	r5, r0, #0
	adds	r1, r5, #0
	mov	r0, r8
	movs	r0, r0
	mov	ip, pc
	bx	sl
	str	r0, [r6, #36]
	adds	r1, r5, #0
	mov	r0, fp
	movs	r0, r0
	mov	ip, pc
	bx	sl
	str	r0, [r6, #40]
	adds	r1, r5, #0
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	sl
	str	r0, [r6, #44]
.L38:
	ldr	r1, [sp, #16]
	str	r1, [r6, #20]
.L34:
	ldr	r3, [sp, #8]
	ldrb	r2, [r3, #0]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L40
	ldr	r4, [r6, #20]
	ldr	r0, [sp, #24]
	str	r4, [sp, #16]
	cmp	r0, r4
	ble.n	.L41
	ldr	r3, [r6, #40]
	ldr	r2, [r6, #72]
	subs	r3, r3, r2
	b.n	.L42
.L41:
	ldr	r0, [r6, #40]
	cmp	r0, #0
	bge.n	.L40
	ldr	r1, [sp, #16]
	str	r1, [sp, #24]
	ldr	r3, [pc, #232]
	ldr	r1, [r6, #68]
	movs	r0, r0
	mov	ip, pc
	bx	r3
	negs	r3, r0
	adds	r2, r3, #0
	str	r3, [r6, #40]
	cmp	r2, #0
	bge.n	.L43
	adds	r2, r0, #0
.L43:
	ldr	r3, [r6, #72]
	cmp	r2, r3
	bgt.n	.L40
	movs	r3, #0
.L42:
	str	r3, [r6, #40]
.L40:
	ldr	r3, [sp, #8]
	ldrb	r2, [r3, #0]
	movs	r3, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L11
	ldr	r1, [r6, #68]
	movs	r3, #63
	ands	r1, r3
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L44
	ldr	r2, [pc, #180]
	lsrs	r3, r1, #1
	lsls	r3, r3, #2
	ldr	r2, [r2, r3]
	ldr	r3, [r6, #72]
	muls	r3, r2
	cmp	r3, #0
	bge.n	.L45
	adds	r3, #15
.L45:
	asrs	r3, r3, #4
	b.n	.L46
.L44:
	ldr	r2, [pc, #160]
	lsrs	r3, r1, #1
	lsls	r3, r3, #2
	ldr	r2, [r2, r3]
	ldr	r3, [r6, #72]
	muls	r3, r2
	cmp	r3, #0
	bge.n	.L47
	adds	r3, #63
.L47:
	asrs	r3, r3, #6
.L46:
	str	r3, [r6, #40]
	ldr	r3, [r6, #68]
	adds	r3, #1
	str	r3, [r6, #68]
.L11:
	ldr	r3, [r6, #36]
	ldr	r4, [sp, #28]
	adds	r4, r4, r3
	str	r4, [sp, #28]
	ldr	r0, [sp, #24]
	ldr	r3, [r6, #40]
	adds	r0, r0, r3
	str	r0, [sp, #24]
	ldr	r1, [sp, #20]
	ldr	r3, [r6, #44]
	adds	r1, r1, r3
	str	r1, [sp, #20]
	ldr	r3, [sp, #4]
	ldrb	r2, [r3, #4]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L48
	add	r1, sp, #36
	str	r0, [r1, #4]
	str	r4, [r1, #0]
	ldr	r4, [sp, #20]
	adds	r0, r6, #0
	str	r4, [r1, #8]
	bl	Func_0800d924
	cmp	r0, #0
	beq.n	.L49
	ldr	r0, [sp, #4]
	ldrb	r3, [r0, #11]
	adds	r3, #1
	strb	r3, [r0, #11]
	b.n	.L1
.L49:
	ldr	r1, [sp, #4]
	strb	r0, [r1, #11]
.L48:
	ldr	r1, [sp, #0]
	ldrb	r3, [r1, #0]
	cmp	r3, #17
	beq.n	.L50
	cmp	r3, #17
	bgt.n	.L51
	cmp	r3, #16
	beq.n	.L52
	b.n	.L53
.L51:
	cmp	r3, #18
	beq.n	.L54
	b.n	.L53
.L52:
	ldr	r2, [r6, #56]
	ldr	r3, [sp, #28]
	cmp	r3, r2
	beq.n	.L55
	ldr	r3, [r6, #8]
	ldr	r4, [sp, #28]
	b.n	.L56
.L50:
	ldr	r2, [r6, #60]
	ldr	r3, [sp, #24]
	cmp	r3, r2
	beq.n	.L55
	ldr	r3, [r6, #12]
	ldr	r4, [sp, #24]
	b.n	.L56
	.4byte 0x0300013c
	.4byte 0x03000118
	.4byte 0xfffc0000
	.4byte 0x080131c0
.L54:
	ldr	r2, [r6, #64]
	ldr	r3, [sp, #20]
	cmp	r3, r2
	beq.n	.L55
	ldr	r3, [r6, #16]
	ldr	r4, [sp, #20]
.L56:
	subs	r3, r3, r2
	subs	r2, r4, r2
	eors	r3, r2
	cmp	r3, #0
	bge.n	.L53
.L55:
	movs	r0, #1
	str	r0, [sp, #12]
.L53:
	ldr	r2, [sp, #12]
	cmp	r2, #0
	beq.n	.L57
	ldr	r4, [sp, #4]
	ldrb	r3, [r4, #3]
	cmp	r3, #0
	beq.n	.L58
	ldr	r0, [r6, #56]
	str	r0, [sp, #28]
	ldr	r2, [r6, #64]
	movs	r3, #0
	str	r3, [r6, #36]
	str	r2, [sp, #20]
	str	r3, [r6, #44]
	ldrb	r3, [r4, #0]
	cmp	r3, #0
	bne.n	.L58
	ldr	r4, [r6, #60]
	str	r4, [sp, #24]
	str	r3, [r6, #40]
.L58:
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r6, #56]
	str	r3, [r6, #60]
	str	r3, [r6, #64]
	movs	r3, #0
	strb	r3, [r1, #0]
.L57:
	ldr	r0, [sp, #28]
	str	r0, [r6, #8]
	ldr	r1, [sp, #24]
	str	r1, [r6, #12]
	ldr	r2, [sp, #20]
	str	r2, [r6, #16]
	ldr	r3, [sp, #4]
	ldrb	r2, [r3, #5]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	ldr	r4, [r6, #36]
	str	r4, [sp, #28]
	ldr	r0, [r6, #44]
	str	r0, [sp, #20]
	cmp	r4, #0
	bne.n	.L59
	cmp	r0, #0
	beq.n	.L1
.L59:
	ldr	r0, [sp, #20]
	ldr	r1, [sp, #28]
	bl	Func_080044d0
	ldrh	r3, [r6, #6]
	subs	r0, r0, r3
	lsls	r0, r0, #16
	movs	r2, #128
	asrs	r0, r0, #16
	lsls	r2, r2, #5
	cmp	r0, r2
	ble.n	.L60
	adds	r0, r2, #0
.L60:
	ldr	r2, [pc, #56]
	cmp	r0, r2
	bge.n	.L61
	adds	r0, r2, #0
.L61:
	adds	r3, r3, r0
	strh	r3, [r6, #6]
.L1:
	ldr	r1, [sp, #32]
	ldr	r2, [sp, #4]
	ldr	r3, [sp, #0]
	subs	r1, #1
	adds	r2, #112
	adds	r3, #112
	str	r1, [sp, #32]
	str	r2, [sp, #4]
	str	r3, [sp, #0]
	adds	r6, #112
	cmp	r1, #0
	blt.n	.L62
	b.n	.L63
.L62:
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0xfffff000

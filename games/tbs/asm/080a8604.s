@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a8604
	.thumb_func
Func_080a8604:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #40
	str	r2, [sp, #24]
	str	r1, [sp, #28]
	ldr	r3, [pc, #700]
	adds	r7, r0, #0
	ldr	r3, [r3, #0]
	movs	r0, #1
	negs	r0, r0
	mov	r8, r3
	bl	Func_08077290
	negs	r3, r0
	orrs	r3, r0
	lsrs	r3, r3, #31
	ldr	r0, [sp, #28]
	str	r3, [sp, #12]
	bl	Func_08077008
	ldr	r2, [sp, #24]
	movs	r3, #255
	ands	r3, r2
	movs	r2, #7
	str	r0, [sp, #20]
	str	r2, [sp, #16]
	cmp	r3, #1
	beq.n	.L0
	movs	r3, #10
	str	r3, [sp, #16]
.L0:
	movs	r3, #190
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r2, [r3, #0]
	movs	r3, #1
	strb	r3, [r2, #5]
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #24]
	adds	r0, r7, #0
	bl	Func_080a8914
	add	r5, sp, #32
	ldr	r2, [sp, #28]
	movs	r1, #1
	adds	r0, r5, #0
	bl	Func_080a8b10
	adds	r0, r5, #0
	bl	Func_080a9dc4
	movs	r6, #128
	ldr	r2, [sp, #24]
	lsls	r6, r6, #1
	ands	r6, r2
	cmp	r6, #0
	bne.n	.L1
	movs	r3, #96
	adds	r0, r7, #0
	movs	r1, #0
	movs	r2, #40
	str	r3, [sp, #0]
	bl	Func_08015068
.L1:
	movs	r3, #0
	mov	sl, r3
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L2
	movs	r2, #16
	ldr	r0, [pc, #576]
	adds	r1, r7, #0
	movs	r3, #40
	bl	Func_08015080
	movs	r2, #1
	mov	sl, r2
.L2:
	ldrb	r3, [r5, #1]
	cmp	r3, #0
	beq.n	.L3
	mov	r2, sl
	lsls	r3, r2, #4
	adds	r3, #40
	ldr	r0, [pc, #556]
	adds	r1, r7, #0
	movs	r2, #16
	bl	Func_08015080
	movs	r3, #1
	add	sl, r3
.L3:
	ldrb	r3, [r5, #2]
	cmp	r3, #0
	beq.n	.L4
	mov	r2, sl
	lsls	r3, r2, #4
	adds	r3, #40
	ldr	r0, [pc, #532]
	adds	r1, r7, #0
	movs	r2, #16
	bl	Func_08015080
	movs	r3, #1
	add	sl, r3
.L4:
	ldrb	r3, [r5, #3]
	cmp	r3, #0
	beq.n	.L5
	mov	r2, sl
	lsls	r3, r2, #4
	adds	r3, #40
	ldr	r0, [pc, #512]
	adds	r1, r7, #0
	movs	r2, #16
	bl	Func_08015080
	movs	r3, #1
	add	sl, r3
.L5:
	ldrb	r3, [r5, #4]
	cmp	r3, #0
	beq.n	.L6
	mov	r2, sl
	lsls	r3, r2, #4
	adds	r3, #40
	ldr	r0, [pc, #488]
	adds	r1, r7, #0
	movs	r2, #16
	bl	Func_08015080
	movs	r3, #1
	add	sl, r3
.L6:
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L7
	ldr	r0, [pc, #472]
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #40
	bl	Func_08015080
.L7:
	adds	r0, r5, #0
	bl	Func_080a9dc4
	adds	r0, r5, #0
	bl	Func_080a9d3c
	movs	r3, #136
	lsls	r3, r3, #2
	add	r3, r8
	ldrh	r3, [r3, #0]
	cmp	r3, #3
	bne.n	.L8
	b.n	.L9
.L8:
	cmp	r6, #0
	bne.n	.L10
	movs	r0, #1
	bl	Func_080030f8
	movs	r3, #96
	str	r3, [sp, #0]
	adds	r0, r7, #0
	movs	r1, #64
	movs	r2, #56
	movs	r3, #224
	bl	Func_08015068
.L10:
	movs	r0, #15
	bl	Func_080150b8
	ldr	r3, [sp, #24]
	cmp	r3, #1
	beq.n	.L11
	ldr	r2, [sp, #12]
	cmp	r2, #1
	bne.n	.L12
.L11:
	movs	r5, #4
	ldr	r3, [sp, #16]
	adds	r0, r7, #0
	movs	r1, #1
	movs	r2, #15
	str	r5, [sp, #0]
	bl	Func_08015280
	adds	r0, r7, #0
	ldr	r3, [sp, #16]
	movs	r1, #2
	movs	r2, #19
	str	r5, [sp, #0]
	bl	Func_08015280
	adds	r0, r7, #0
	movs	r1, #3
	movs	r2, #23
	ldr	r3, [sp, #16]
	str	r5, [sp, #0]
	bl	Func_08015280
	adds	r0, r7, #0
	movs	r1, #4
	movs	r2, #27
	ldr	r3, [sp, #16]
	str	r5, [sp, #0]
	bl	Func_08015280
.L12:
	ldr	r3, [sp, #12]
	cmp	r3, #0
	beq.n	.L13
	ldr	r2, [sp, #16]
	lsls	r6, r2, #3
	adds	r3, r6, #0
	ldr	r0, [pc, #324]
	adds	r3, #8
	adds	r1, r7, #0
	movs	r2, #64
	bl	Func_08015080
.L13:
	ldr	r3, [sp, #24]
	cmp	r3, #1
	bne.n	.L14
	ldr	r2, [sp, #12]
	cmp	r2, #0
	bne.n	.L15
	ldr	r3, [sp, #16]
	subs	r3, #1
	str	r3, [sp, #16]
.L15:
	ldr	r2, [sp, #16]
	lsls	r6, r2, #3
	adds	r3, r6, #0
	ldr	r0, [pc, #292]
	adds	r3, #16
	adds	r1, r7, #0
	movs	r2, #64
	bl	Func_08015090
	ldr	r5, [pc, #284]
	adds	r3, r6, #0
	adds	r0, r5, #0
	adds	r3, #24
	adds	r1, r7, #0
	movs	r2, #64
	bl	Func_08015080
	adds	r5, #1
	adds	r3, r6, #0
	adds	r3, #32
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #64
	bl	Func_08015080
.L14:
	ldr	r2, [sp, #16]
	movs	r3, #0
	lsls	r2, r2, #3
	mov	sl, r3
	ldr	r3, [sp, #20]
	str	r2, [sp, #8]
	adds	r2, #8
	adds	r3, #72
	str	r2, [sp, #4]
	movs	r2, #104
	mov	r8, r3
	mov	fp, r2
	movs	r3, #120
	ldr	r2, [sp, #20]
	mov	r9, r3
	adds	r3, #160
	adds	r5, r2, r3
.L20:
	ldr	r2, [sp, #12]
	cmp	r2, #0
	beq.n	.L16
	ldr	r3, [sp, #4]
	ldrb	r0, [r5, #0]
	movs	r1, #1
	str	r3, [sp, #0]
	adds	r2, r7, #0
	mov	r3, r9
	bl	Func_080150b0
.L16:
	ldr	r2, [sp, #24]
	movs	r3, #255
	ands	r3, r2
	cmp	r3, #1
	bne.n	.L17
	ldr	r3, [sp, #12]
	cmp	r3, #0
	beq.n	.L18
	ldr	r2, [sp, #4]
	ldrb	r0, [r5, #4]
	movs	r1, #1
	str	r2, [sp, #0]
	mov	r3, fp
	adds	r2, r7, #0
	ldr	r6, [sp, #8]
	bl	Func_080150b0
	mov	r2, r9
	subs	r2, #8
	ldr	r0, [pc, #164]
	adds	r1, r7, #0
	ldr	r3, [sp, #4]
	bl	Func_08015098
	b.n	.L19
.L18:
	ldr	r3, [sp, #16]
	lsls	r6, r3, #3
.L19:
	ldr	r0, [sp, #28]
	mov	r1, sl
	bl	Func_080771f8
	adds	r2, r6, #0
	adds	r2, #16
	mov	r3, r9
	str	r2, [sp, #0]
	subs	r3, #8
	adds	r2, r7, #0
	movs	r1, #2
	bl	Func_080150b0
	mov	r3, r8
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	adds	r3, r6, #0
	adds	r3, #24
	adds	r2, r7, #0
	str	r3, [sp, #0]
	movs	r1, #3
	mov	r3, fp
	bl	Func_080150b0
	mov	r3, r8
	movs	r2, #2
	ldrsh	r0, [r3, r2]
	adds	r3, r6, #0
	adds	r3, #32
	str	r3, [sp, #0]
	movs	r1, #3
	adds	r2, r7, #0
	mov	r3, fp
	bl	Func_080150b0
.L17:
	movs	r2, #4
	add	r8, r2
	movs	r2, #1
	movs	r3, #32
	add	sl, r2
	add	fp, r3
	add	r9, r3
	mov	r3, sl
	adds	r5, #1
	cmp	r3, #3
	ble.n	.L20
.L9:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
	.4byte 0x00000bd5
	.4byte 0x00000bd6
	.4byte 0x00000bd7
	.4byte 0x00000bd8
	.4byte 0x00000bd9
	.4byte 0x00000bd4
	.4byte 0x00000afd
	.4byte 0x080af22c
	.4byte 0x00000afe
	.4byte 0x080af230

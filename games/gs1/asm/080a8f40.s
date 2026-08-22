@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a8f40
	.thumb_func
Func_080a8f40:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #328]
	ldr	r3, [r3, #0]
	mov	fp, r3
	ldr	r3, [pc, #324]
	add	r3, fp
	mov	r8, r0
	ldrb	r0, [r3, #0]
	adds	r5, r2, #0
	sub	sp, #8
	bl	Func_08077008
	str	r0, [sp, #4]
	mov	r0, r8
	bl	Func_08015270
	ldr	r2, [r5, #8]
	lsls	r3, r2, #2
	adds	r6, r3, r2
	ldr	r3, [r5, #20]
	subs	r3, r3, r6
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	mov	r9, r3
	cmp	r3, #5
	bls.n	.L0
	movs	r1, #5
	mov	r9, r1
.L0:
	movs	r3, #58
	str	r3, [sp, #0]
	movs	r0, #5
	adds	r1, r6, #0
	mov	r2, r8
	movs	r3, #80
	bl	Func_080a2324
	movs	r2, #28
	ldr	r1, [r5, #20]
	ldr	r3, [r5, #8]
	mov	r0, r8
	str	r2, [sp, #0]
	movs	r2, #5
	bl	Func_080a21b0
	movs	r2, #176
	movs	r3, #0
	ldr	r0, [pc, #244]
	mov	r1, r8
	bl	Func_08015080
	movs	r2, #0
	mov	r3, r9
	mov	sl, r2
	cmp	r3, #0
	bls.n	.L1
	movs	r1, #228
	lsls	r3, r6, #1
	lsls	r1, r1, #1
	adds	r7, r3, r1
.L4:
	mov	r2, fp
	ldrh	r3, [r7, r2]
	ldr	r0, [pc, #220]
	ands	r0, r3
	bl	Func_08077080
	mov	r1, fp
	ldrh	r3, [r7, r1]
	adds	r6, r0, #0
	ldr	r0, [pc, #204]
	mov	r2, sl
	ands	r0, r3
	lsls	r5, r2, #4
	ldr	r3, [pc, #200]
	adds	r5, #16
	adds	r0, r0, r3
	mov	r1, r8
	movs	r2, #88
	adds	r3, r5, #0
	bl	Func_08015080
	ldrb	r0, [r6, #9]
	movs	r1, #2
	mov	r2, r8
	movs	r3, #176
	str	r5, [sp, #0]
	bl	Func_080150a8
	ldrb	r4, [r6, #8]
	cmp	r4, #255
	bne.n	.L2
	movs	r4, #11
	b.n	.L3
.L2:
	subs	r4, #1
.L3:
	mov	r3, sl
	lsls	r2, r3, #1
	movs	r3, #0
	str	r3, [sp, #0]
	adds	r2, #2
	adds	r3, r4, #0
	mov	r0, r8
	movs	r1, #25
	bl	Func_080a8cc0
	mov	r3, sl
	adds	r3, #1
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	mov	sl, r3
	adds	r7, #2
	cmp	r9, sl
	bhi.n	.L4
.L1:
	movs	r3, #134
	lsls	r3, r3, #2
	add	r3, fp
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L5
	ldr	r0, [pc, #116]
	mov	r1, r8
	movs	r2, #96
	movs	r3, #17
	bl	Func_08015080
.L5:
	ldr	r0, [sp, #4]
	mov	r1, r8
	movs	r2, #40
	movs	r3, #0
	bl	Func_08015090
	ldr	r2, [pc, #96]
	ldr	r1, [sp, #4]
	adds	r3, r1, r2
	ldrb	r0, [r3, #0]
	ldr	r3, [pc, #92]
	mov	r1, r8
	adds	r0, r0, r3
	movs	r2, #0
	movs	r3, #32
	bl	Func_08015080
	mov	r1, r8
	ldr	r0, [pc, #80]
	movs	r2, #0
	movs	r3, #48
	bl	Func_08015098
	ldr	r3, [sp, #4]
	ldrb	r0, [r3, #15]
	movs	r3, #48
	str	r3, [sp, #0]
	movs	r1, #2
	mov	r2, r8
	movs	r3, #24
	bl	Func_080150b0
	movs	r0, #1
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x0000021a
	.4byte 0x00000aed
	.4byte 0x00003fff
	.4byte 0x00000333
	.4byte 0x00000aef
	.4byte 0x00000129
	.4byte 0x00000741
	.4byte 0x080af22c

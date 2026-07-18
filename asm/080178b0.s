@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080178b0
	.thumb_func
Func_080178b0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #388
	str	r1, [sp, #0]
	ldr	r3, [pc, #384]
	adds	r5, r0, #0
	ldr	r6, [r3, #0]
	lsls	r3, r5, #8
	asrs	r3, r3, #16
	mov	sl, r3
	movs	r1, #192
	movs	r3, #255
	ands	r5, r3
	add	r0, sp, #4
	ldr	r3, [pc, #368]
	lsls	r1, r1, #1
	bl	Func_080072f0
	ldr	r0, [pc, #364]
	adds	r3, r6, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L0
	movs	r2, #0
	movs	r3, #8
	mov	r9, r2
	b.n	.L1
.L0:
	ldr	r2, [pc, #348]
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	movs	r0, #1
	mov	r9, r0
.L1:
	mov	fp, r3
	adds	r3, r5, #0
	subs	r3, #32
	ldr	r2, [pc, #336]
	lsls	r3, r3, #5
	adds	r7, r3, r2
	ldrh	r3, [r7, #0]
	ldr	r0, [pc, #332]
	mov	r8, r3
	adds	r3, r6, r0
	ldrh	r3, [r3, #0]
	adds	r7, #2
	cmp	r3, #1
	bne.n	.L2
	mov	r1, sp
	ldr	r5, [pc, #320]
	adds	r1, #53
	mov	r2, r9
	adds	r0, r7, #0
	bl	Func_080072f8
	mov	r1, sp
	adds	r1, #54
	mov	r2, r9
	adds	r0, r7, #0
	bl	Func_080072f8
	mov	r2, fp
	add	r1, sp, #36
	adds	r0, r7, #0
	bl	Func_080072f8
	mov	r1, sp
	mov	r2, fp
	adds	r1, #37
	adds	r0, r7, #0
	bl	Func_080072f8
	movs	r2, #1
	add	r8, r2
	b.n	.L3
.L2:
	mov	r1, sp
	adds	r1, #53
	mov	r2, r9
	ldr	r5, [pc, #264]
	adds	r0, r7, #0
	bl	Func_080072f8
	adds	r0, r7, #0
	add	r1, sp, #36
	mov	r2, fp
	bl	Func_080072f8
.L3:
	mov	r0, sl
	lsls	r3, r0, #16
	lsrs	r2, r3, #16
	cmp	r2, #0
	beq.n	.L4
	ldr	r3, [pc, #240]
	lsls	r2, r2, #5
	ldr	r0, [pc, #228]
	adds	r7, r2, r3
	movs	r3, #0
	ldrsh	r2, [r7, r3]
	adds	r3, r6, r0
	ldrh	r3, [r3, #0]
	mov	sl, r2
	adds	r7, #2
	cmp	r3, #1
	bne.n	.L5
	add	r3, sp, #4
	mov	r2, r8
	adds	r6, r3, r2
	adds	r1, r6, #0
	ldr	r5, [pc, #204]
	adds	r1, #49
	mov	r2, r9
	adds	r0, r7, #0
	bl	Func_080072f8
	adds	r1, r6, #0
	adds	r1, #50
	mov	r2, r9
	adds	r0, r7, #0
	bl	Func_080072f8
	adds	r1, r6, #0
	adds	r1, #32
	mov	r2, fp
	adds	r0, r7, #0
	bl	Func_080072f8
	adds	r1, r6, #0
	mov	r2, fp
	adds	r1, #33
	adds	r0, r7, #0
	bl	Func_080072f8
	mov	r0, sl
	movs	r2, #128
	lsls	r3, r0, #16
	lsls	r2, r2, #9
	adds	r3, r3, r2
	asrs	r3, r3, #16
	mov	sl, r3
	b.n	.L6
.L5:
	add	r5, sp, #4
	add	r5, r8
	adds	r1, r5, #0
	adds	r1, #49
	mov	r2, r9
	ldr	r6, [pc, #128]
	adds	r0, r7, #0
	bl	Func_080072fc
	adds	r1, r5, #0
	adds	r1, #32
	adds	r0, r7, #0
	mov	r2, fp
	bl	Func_080072fc
.L6:
	mov	r0, sl
	lsls	r3, r0, #16
	lsrs	r3, r3, #16
	add	r8, r3
.L4:
	mov	r1, sp
	adds	r1, #11
	movs	r6, #0
.L10:
	movs	r5, #0
.L9:
	movs	r4, #0
.L8:
	movs	r2, #0
	movs	r0, #7
.L7:
	ldrb	r3, [r1, #0]
	lsls	r2, r2, #4
	subs	r0, #1
	adds	r2, r2, r3
	subs	r1, #1
	cmp	r0, #0
	bge.n	.L7
	ldr	r0, [sp, #0]
	stmia	r0!, {r2}
	adds	r4, #1
	adds	r3, r0, #0
	str	r3, [sp, #0]
	adds	r1, #24
	cmp	r4, #7
	ble.n	.L8
	adds	r5, #1
	subs	r1, #120
	cmp	r5, #1
	ble.n	.L9
	adds	r6, #1
	adds	r1, #112
	cmp	r6, #1
	ble.n	.L10
	mov	r0, r8
	add	sp, #388
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e8c
	.4byte 0x03000164
	.4byte 0x00000ea4
	.4byte 0x00000eae
	.4byte 0x08032224
	.4byte 0x00000eac
	.4byte 0x03000214
	.4byte 0x08031e24

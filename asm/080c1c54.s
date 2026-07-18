@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080c1c54
	.thumb_func
Func_080c1c54:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	str	r0, [sp, #8]
	movs	r0, #36
	mov	sl, r1
	bl	Func_08004970
	mov	r9, r0
	ldr	r0, [sp, #8]
	bl	Func_08077008
	adds	r7, r0, #0
	adds	r6, r7, #0
	adds	r6, #16
	movs	r2, #36
	ldr	r3, [pc, #360]
	adds	r1, r6, #0
	mov	r0, r9
	bl	Func_080072f0
	mov	r1, sl
	lsls	r3, r1, #1
	add	r3, sl
	movs	r0, #0
	ldrsh	r5, [r6, r0]
	lsls	r0, r3, #5
	movs	r1, #10
	add	r0, sl
	mov	fp, r3
	bl	Func_080022ec
	adds	r5, r5, r0
	mov	r0, r9
	movs	r2, #0
	ldrsh	r3, [r0, r2]
	lsls	r0, r3, #3
	subs	r0, r0, r3
	movs	r1, #10
	bl	Func_080022ec
	cmp	r5, r0
	bge.n	.L0
	adds	r5, r0, #0
.L0:
	ldr	r1, [pc, #308]
	mov	r8, r1
	cmp	r5, r8
	ble.n	.L1
	mov	r5, r8
.L1:
	mov	r3, sl
	lsls	r3, r3, #4
	strh	r5, [r6, #0]
	mov	r1, sl
	subs	r0, r3, r1
	movs	r1, #10
	movs	r2, #18
	ldrsh	r5, [r7, r2]
	str	r3, [sp, #4]
	bl	Func_080022ec
	adds	r5, r5, r0
	mov	r0, r9
	movs	r2, #2
	ldrsh	r3, [r0, r2]
	lsls	r0, r3, #3
	subs	r0, r0, r3
	movs	r1, #10
	bl	Func_080022ec
	cmp	r5, r0
	bge.n	.L2
	adds	r5, r0, #0
.L2:
	cmp	r5, r8
	ble.n	.L3
	mov	r5, r8
.L3:
	mov	r1, sl
	lsls	r1, r1, #5
	mov	r2, sl
	subs	r0, r1, r2
	lsls	r0, r0, #2
	strh	r5, [r7, #18]
	mov	r8, r1
	subs	r0, r0, r2
	movs	r1, #10
	bl	Func_080022ec
	ldrh	r5, [r7, #24]
	adds	r5, r5, r0
	mov	r0, r9
	ldrh	r3, [r0, #8]
	lsls	r0, r3, #3
	subs	r0, r0, r3
	movs	r1, #10
	bl	Func_080022ec
	cmp	r5, r0
	bge.n	.L4
	adds	r5, r0, #0
.L4:
	ldr	r6, [pc, #204]
	cmp	r5, r6
	ble.n	.L5
	adds	r5, r6, #0
.L5:
	mov	r0, r8
	strh	r5, [r7, #24]
	movs	r1, #10
	add	r0, sl
	bl	Func_080022ec
	mov	r1, r9
	ldrh	r3, [r1, #10]
	ldrh	r5, [r7, #26]
	adds	r5, r5, r0
	lsls	r0, r3, #3
	subs	r0, r0, r3
	movs	r1, #10
	bl	Func_080022ec
	cmp	r5, r0
	bge.n	.L6
	adds	r5, r0, #0
.L6:
	cmp	r5, r6
	ble.n	.L7
	adds	r5, r6, #0
.L7:
	mov	r2, fp
	lsls	r0, r2, #4
	strh	r5, [r7, #26]
	movs	r1, #10
	add	r0, fp
	bl	Func_080022ec
	ldrh	r5, [r7, #28]
	adds	r5, r5, r0
	mov	r0, r9
	ldrh	r3, [r0, #12]
	lsls	r0, r3, #3
	subs	r0, r0, r3
	movs	r1, #10
	bl	Func_080022ec
	cmp	r5, r0
	bge.n	.L8
	adds	r5, r0, #0
.L8:
	cmp	r5, r6
	ble.n	.L9
	adds	r5, r6, #0
.L9:
	movs	r1, #20
	strh	r5, [r7, #28]
	mov	r8, r1
	movs	r6, #36
	movs	r4, #3
.L12:
	ldr	r0, [sp, #4]
	ldrsh	r2, [r6, r7]
	mov	r1, sl
	subs	r3, r0, r1
	adds	r5, r2, r3
	mov	r1, r9
	mov	r2, r8
	ldrsh	r3, [r2, r1]
	lsls	r0, r3, #3
	subs	r0, r0, r3
	movs	r1, #10
	str	r4, [sp, #0]
	bl	Func_080022ec
	ldr	r4, [sp, #0]
	cmp	r5, r0
	bge.n	.L10
	adds	r5, r0, #0
.L10:
	cmp	r5, #200
	ble.n	.L11
	movs	r5, #200
.L11:
	movs	r2, #4
	subs	r4, #1
	strh	r5, [r6, r7]
	add	r8, r2
	adds	r6, #4
	cmp	r4, #0
	bge.n	.L12
	ldrb	r3, [r7, #15]
	add	r3, sl
	strb	r3, [r7, #15]
	ldr	r0, [sp, #8]
	bl	Func_08077010
	mov	r0, r9
	bl	Func_08002df0
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001388
	.4byte 0x0000270f
	.4byte 0x000003e7

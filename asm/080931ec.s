@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080931ec
	.thumb_func
Func_080931ec:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r9, r3
	ldr	r3, [pc, #248]
	ldr	r6, [sp, #36]
	mov	sl, r2
	mov	r8, r1
	ldr	r5, [r3, #0]
	bl	Func_08092ba8
	mov	fp, r0
	adds	r0, r6, #0
	bl	Func_08092ba8
	adds	r7, r0, #0
	mov	r0, fp
	bl	Func_080915ac
	movs	r2, #236
	lsls	r2, r2, #1
	adds	r5, r5, r2
	adds	r3, r0, #0
	ldrh	r0, [r5, #0]
	adds	r2, r0, #1
	lsls	r0, r0, #16
	strh	r2, [r5, #0]
	mov	r1, r8
	mov	r2, sl
	lsls	r3, r3, #16
	asrs	r0, r0, #16
	bl	Func_08015038
	mov	r2, r9
	ldr	r3, [sp, #32]
	movs	r1, #0
	mov	sl, r0
	mov	r0, fp
	bl	Func_080150f8
	adds	r0, r7, #0
	bl	Func_080915ac
	adds	r3, r0, #0
	ldrh	r0, [r5, #0]
	adds	r2, r0, #1
	strh	r2, [r5, #0]
	lsls	r0, r0, #16
	ldr	r1, [sp, #40]
	ldr	r2, [sp, #44]
	lsls	r3, r3, #16
	asrs	r0, r0, #16
	bl	Func_08015038
	movs	r1, #0
	mov	r8, r0
	ldr	r2, [sp, #48]
	adds	r0, r7, #0
	ldr	r3, [sp, #52]
	bl	Func_080150f8
	b.n	.L0
.L1:
	movs	r0, #1
	bl	Func_080030f8
.L0:
	bl	Func_08015048
	cmp	r0, #0
	beq.n	.L1
	movs	r0, #1
	bl	Func_080030f8
	ldr	r1, [pc, #116]
	ldr	r2, [pc, #120]
	ldr	r3, [r1, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L2
	adds	r6, r1, #0
	adds	r5, r2, #0
.L3:
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [r6, #0]
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L3
.L2:
	movs	r0, #1
	bl	Func_080030f8
	mov	r0, fp
	bl	Func_08015100
	adds	r0, r7, #0
	bl	Func_08015100
	bl	Func_08015140
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L4
.L5:
	movs	r0, #1
	bl	Func_080030f8
.L4:
	mov	r0, sl
	bl	Func_08015050
	cmp	r0, #0
	beq.n	.L5
	b.n	.L6
.L7:
	movs	r0, #1
	bl	Func_080030f8
.L6:
	mov	r0, r8
	bl	Func_08015050
	cmp	r0, #0
	beq.n	.L7
	movs	r0, #1
	bl	Func_080030f8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001ebc
	.4byte 0x03001c94
	.4byte 0x00000303

@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08091c7c
	.thumb_func
Func_08091c7c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #236]
	mov	r8, r0
	ldr	r6, [r3, #0]
	movs	r0, #250
	lsls	r0, r0, #1
	adds	r3, r6, r0
	ldr	r0, [r3, #0]
	sub	sp, #4
	mov	r9, r1
	bl	Func_0808d394
	movs	r2, #0
	ldrsh	r1, [r0, r2]
	movs	r0, #252
	lsls	r0, r0, #1
	adds	r3, r6, r0
	ldr	r3, [r3, #0]
	mov	fp, r1
	movs	r1, #254
	lsls	r1, r1, #1
	movs	r2, #1
	mov	sl, r3
	adds	r3, r6, r1
	ldr	r5, [r3, #0]
	str	r2, [sp, #0]
	ldr	r2, [pc, #188]
	ldr	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L0
	adds	r7, r2, #0
.L1:
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [r7, #0]
	cmp	r3, #0
	bne.n	.L1
	b.n	.L0
.L2:
	movs	r0, #1
	bl	Func_080030f8
.L0:
	bl	Func_08015048
	cmp	r0, #0
	beq.n	.L2
	movs	r0, #3
	bl	Func_080030f8
	mov	r3, r9
	cmp	r3, #0
	bne.n	.L3
	mov	r0, sl
	ldrh	r2, [r0, #14]
	ldrh	r3, [r0, #10]
	adds	r1, r2, r3
	cmp	r5, #0
	beq.n	.L4
	ldrh	r2, [r5, #14]
	ldrh	r3, [r5, #10]
	adds	r2, r2, r3
	cmp	r1, r2
	bge.n	.L4
	adds	r1, r2, #0
.L4:
	cmp	r1, #15
	ble.n	.L3
	movs	r1, #0
	str	r1, [sp, #0]
.L3:
	ldr	r2, [pc, #112]
	adds	r3, r6, r2
	adds	r2, #2
	movs	r0, #0
	ldrsh	r1, [r3, r0]
	adds	r3, r6, r2
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r0, [sp, #0]
	movs	r3, #0
	bl	Func_08015390
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L5
	movs	r1, #4
	mov	r0, r8
	bl	Func_080924d4
	mov	r0, fp
	bl	Func_08015100
	bl	Func_08015140
	mov	r0, r8
	movs	r1, #4
	bl	Func_08091c44
	b.n	.L6
.L5:
	movs	r1, #3
	mov	r0, r8
	bl	Func_080924d4
	mov	r0, fp
	bl	Func_08015100
	bl	Func_08015140
	mov	r0, r8
	movs	r1, #3
	bl	Func_08091c44
.L6:
	adds	r0, r5, #0
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001ebc
	.4byte 0x03001c94
	.4byte 0x00000cc2

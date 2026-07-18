@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a153c
	.thumb_func
Func_080a153c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	movs	r2, #0
	ldr	r0, [pc, #156]
	movs	r3, #40
	adds	r6, r1, #0
	bl	Func_08015090
	ldr	r3, [pc, #148]
	mov	r8, r3
	mov	r0, r8
	movs	r3, #40
	adds	r1, r6, #0
	movs	r2, #48
	bl	Func_08015098
	movs	r3, #52
	ldrsh	r5, [r7, r3]
	adds	r1, r6, #0
	movs	r3, #40
	adds	r0, r5, #0
	movs	r2, #88
	bl	Func_080a14f0
	movs	r3, #56
	ldrsh	r5, [r7, r3]
	ldrh	r3, [r7, #52]
	lsls	r3, r3, #16
	asrs	r3, r3, #18
	cmp	r5, r3
	bge.n	.L0
	movs	r0, #4
	bl	Func_080150b8
.L0:
	cmp	r5, #0
	bne.n	.L1
	movs	r0, #2
	bl	Func_080150b8
.L1:
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r2, #48
	movs	r3, #40
	bl	Func_080a14f0
	movs	r0, #15
	bl	Func_080150b8
	adds	r1, r6, #0
	ldr	r0, [pc, #72]
	movs	r2, #0
	movs	r3, #48
	bl	Func_08015090
	mov	r0, r8
	adds	r1, r6, #0
	movs	r3, #48
	movs	r2, #48
	bl	Func_08015098
	movs	r3, #58
	ldrsh	r5, [r7, r3]
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r3, #48
	movs	r2, #48
	bl	Func_080a14f0
	movs	r3, #54
	ldrsh	r5, [r7, r3]
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r2, #88
	movs	r3, #48
	bl	Func_080a14f0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x080af210
	.4byte 0x080af214
	.4byte 0x080af218

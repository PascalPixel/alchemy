@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801cc50
	.thumb_func
Func_0801cc50:
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r6, r2, #0
	movs	r2, #0
	ldrsh	r0, [r5, r2]
	ldr	r4, [pc, #96]
	lsls	r0, r0, #16
	movs	r0, r0
	mov	ip, pc
	bx	r4
	asrs	r7, r0, #16
	movs	r2, #2
	ldrsh	r0, [r5, r2]
	adds	r1, r6, #0
	lsls	r0, r0, #16
	movs	r0, r0
	mov	ip, pc
	bx	r4
	asrs	r6, r0, #16
	movs	r2, #4
	ldrsh	r0, [r5, r2]
	adds	r1, r3, #0
	lsls	r0, r0, #16
	movs	r0, r0
	mov	ip, pc
	bx	r4
	asrs	r0, r0, #16
	cmp	r7, #0
	bge.n	.L0
	movs	r7, #0
.L0:
	cmp	r7, #31
	ble.n	.L1
	movs	r7, #31
.L1:
	cmp	r6, #0
	bge.n	.L2
	movs	r6, #0
.L2:
	cmp	r6, #31
	ble.n	.L3
	movs	r6, #31
.L3:
	cmp	r0, #0
	bge.n	.L4
	movs	r0, #0
.L4:
	cmp	r0, #31
	ble.n	.L5
	movs	r0, #31
.L5:
	lsls	r3, r6, #5
	lsls	r0, r0, #10
	adds	r0, r0, r3
	adds	r0, r7, r0
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03000118

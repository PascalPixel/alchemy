@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801b148
	.thumb_func
Func_0801b148:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #156]
	ldr	r6, [r3, #0]
	bl	Func_0801a97c
	movs	r2, #212
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r0, [r3, #0]
	movs	r1, #2
	bl	Func_08016418
	movs	r0, #1
	bl	Func_080030f8
	movs	r2, #210
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r5, [r3, #0]
	cmp	r5, #0
	beq.n	.L0
	movs	r7, #0
.L2:
	ldrh	r3, [r5, #10]
	cmp	r3, #0
	beq.n	.L1
	ldrh	r0, [r5, #12]
	bl	Func_08003f3c
	strh	r7, [r5, #10]
.L1:
	ldr	r5, [r5, #4]
	cmp	r5, #0
	bne.n	.L2
.L0:
	movs	r2, #211
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r5, [r3, #0]
	cmp	r5, #0
	beq.n	.L3
	movs	r7, #0
.L5:
	ldrh	r3, [r5, #10]
	cmp	r3, #0
	beq.n	.L4
	ldrh	r0, [r5, #12]
	bl	Func_08003f3c
	strh	r7, [r5, #10]
.L4:
	ldr	r5, [r5, #4]
	cmp	r5, #0
	bne.n	.L5
.L3:
	bl	Func_0801c21c
	movs	r2, #18
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	beq.n	.L6
	ldrh	r0, [r6, #12]
	bl	Func_08003f3c
	movs	r2, #18
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	beq.n	.L6
	adds	r3, r6, #0
	adds	r3, #64
	ldrh	r0, [r3, #0]
	bl	Func_08003f3c
.L6:
	movs	r2, #185
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldrh	r0, [r3, #0]
	bl	Func_08003f3c
	movs	r0, #18
	bl	Func_08002dd8
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e98

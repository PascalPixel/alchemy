@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0807a7a0
	.thumb_func
Func_0807a7a0:
	push	{r5, r6, lr}
	ldr	r5, [pc, #116]
	ldrh	r3, [r5, #0]
	ldr	r1, [pc, #116]
	mov	ip, r3
	adds	r5, #2
	cmp	ip, r1
	bne.n	.L0
	movs	r6, #0
.L2:
	adds	r0, r6, #0
	bl	Func_08077394
	movs	r2, #14
	adds	r0, #216
.L1:
	ldrh	r3, [r5, #0]
	subs	r2, #1
	strh	r3, [r0, #0]
	adds	r5, #2
	adds	r0, #2
	cmp	r2, #0
	bge.n	.L1
	adds	r0, r6, #0
	bl	Func_08079ae8
	adds	r0, r6, #0
	adds	r6, #1
	bl	Func_08077428
	cmp	r6, #3
	ble.n	.L2
	ldr	r0, [pc, #64]
	movs	r4, #136
	ldrh	r2, [r5, #0]
	lsls	r4, r4, #2
	adds	r3, r0, r4
	strh	r2, [r3, #0]
	adds	r5, #2
	ldrh	r2, [r5, #0]
	adds	r4, #2
	adds	r3, r0, r4
	strh	r2, [r3, #0]
	movs	r3, #252
	adds	r5, #2
	lsls	r3, r3, #1
	adds	r2, r0, r3
	ldrh	r3, [r5, #0]
	strh	r3, [r2, #0]
	subs	r4, #40
	ldrh	r3, [r5, #2]
	ldr	r5, [pc, #20]
	adds	r2, r0, r4
	movs	r1, #0
	strh	r3, [r2, #0]
	ldr	r0, [pc, #24]
	strh	r1, [r5, #0]
	bl	Func_08079374
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x02001078
	.4byte 0x00006774
	.4byte 0x02000240
	.4byte 0x00000952

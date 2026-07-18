@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080f7df0
	.thumb_func
Func_080f7df0:
	push	{r5, lr}
	ldr	r3, [pc, #56]
	lsls	r1, r0, #1
	ldr	r4, [r3, #0]
	ldr	r3, [pc, #52]
	adds	r1, r1, r0
	lsls	r0, r0, #2
	adds	r0, r0, r3
	ldr	r2, [r4, r0]
	movs	r0, #192
	lsls	r2, r2, #2
	lsls	r1, r1, #2
	adds	r3, r4, r2
	lsls	r0, r0, #6
	adds	r3, r3, r0
	adds	r5, r1, #4
	str	r3, [r4, r5]
	adds	r2, r2, r0
	ldr	r3, [r4, r2]
	str	r3, [r4, r1]
	adds	r3, r4, r1
	str	r3, [r4, r2]
	ldr	r2, [r3, #0]
	cmp	r2, #0
	beq.n	.L0
	str	r3, [r2, #4]
.L0:
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02004c00
	.4byte 0x00003404

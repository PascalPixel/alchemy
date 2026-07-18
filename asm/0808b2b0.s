@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808b2b0
	.thumb_func
Func_0808b2b0:
	push	{lr}
	subs	r0, #1
	cmp	r0, #6
	bhi.n	.L0
	ldr	r2, [pc, #68]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0808b2dc
	.4byte 0x0808b2e0
	.4byte 0x0808b2e4
	.4byte 0x0808b2e8
	.4byte 0x0808b2ec
	.4byte 0x0808b2ec
	.4byte 0x0808b2e8
	ldr	r2, [pc, #36]
	b.n	.L1
	ldr	r2, [pc, #36]
	b.n	.L1
	ldr	r2, [pc, #36]
	b.n	.L1
	ldr	r2, [pc, #36]
	b.n	.L1
	ldr	r2, [pc, #36]
	b.n	.L1
.L0:
	ldr	r2, [pc, #36]
.L1:
	ldr	r3, [pc, #40]
	movs	r1, #235
	lsls	r1, r1, #1
	adds	r3, r3, r1
	strh	r2, [r3, #0]
	pop	{r0}
	bx	r0
	.4byte 0x0808b2c0
	.4byte 0x00000038
	.4byte 0x0000003a
	.4byte 0x0000003c
	.4byte 0x00000036
	.4byte 0x00000037
	.4byte 0x00000039
	.4byte 0x02000240

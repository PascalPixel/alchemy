@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08091f14
	.thumb_func
Func_08091f14:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #108]
	movs	r5, #128
	ldr	r3, [r3, #0]
	lsls	r5, r5, #4
	mov	r8, r3
	ands	r5, r0
	movs	r3, #255
	adds	r7, r1, #0
	ands	r0, r3
	cmp	r5, #0
	bne.n	.L0
	bl	Func_0809537c
.L0:
	movs	r1, #150
	lsls	r1, r1, #1
	ldr	r6, [pc, #80]
	adds	r3, r7, r1
	movs	r1, #141
	lsls	r1, r1, #2
	orrs	r3, r5
	adds	r2, r6, r1
	strh	r3, [r2, #0]
	adds	r0, r7, #0
	bl	Func_0808b074
	movs	r3, #190
	lsls	r3, r3, #1
	add	r3, r8
	strh	r0, [r3, #0]
	movs	r3, #207
	lsls	r3, r3, #1
	add	r3, r8
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	bne.n	.L1
	movs	r1, #250
	lsls	r1, r1, #1
	adds	r3, r6, r1
	ldr	r0, [r3, #0]
	bl	Func_0808ba1c
	adds	r0, #8
	bl	Func_0808adf0
.L1:
	movs	r1, #0
	movs	r0, #0
	bl	Func_0808b320
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ebc
	.4byte 0x02000240
	ldr	r3, [pc, #16]
	movs	r4, #231
	lsls	r4, r4, #1
	adds	r2, r3, r4
	strh	r0, [r2, #0]
	movs	r2, #232
	lsls	r2, r2, #1
	adds	r3, r3, r2
	strh	r1, [r3, #0]
	bx	lr
	.4byte 0x02000240

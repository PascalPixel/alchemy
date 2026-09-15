@ 通信状態を交換して結果ビットを返す処理。単独のＣ翻訳では一致するが、
@ 元の翻訳単位を標準設定で一括コンパイルすると一致しないため保持する。
.syntax unified
	.thumb
	.global SerialRuntime_ExchangePayloads
	.global Func_08006088
	.thumb_func
SerialRuntime_ExchangePayloads:
Func_08006088:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #84]
	ldr	r5, [pc, #84]
	ldr	r7, [r3, #0]
	ldrb	r3, [r5, #1]
	adds	r6, r0, #0
	adds	r0, r1, #0
	cmp	r3, #1
	bne.n	.L_status
	bl	Func_0800615c
	adds	r0, r6, #0
	bl	Func_080060e8
	ldrb	r3, [r5, #11]
	adds	r3, #1
	strb	r3, [r5, #11]
.L_status:
	ldrb	r3, [r5, #2]
	ldrb	r2, [r5, #3]
	lsls	r3, r3, #8
	orrs	r2, r3
	ldrb	r3, [r5, #0]
	cmp	r3, #8
	bne.n	.L_parent
	movs	r3, #128
	orrs	r2, r3
.L_parent:
	ldrb	r3, [r5, #9]
	adds	r0, r2, #0
	cmp	r3, #0
	beq.n	.L_control
	movs	r3, #128
	lsls	r3, r3, #5
	orrs	r0, r3
.L_control:
	lsls	r3, r7, #26
	lsrs	r3, r3, #30
	cmp	r3, #1
	bls.n	.L_return
	movs	r3, #128
	lsls	r3, r3, #6
	orrs	r0, r3
.L_return:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte	0x04000128
	.4byte	0x02002240

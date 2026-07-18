@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08007028
	.thumb_func
Func_08007028:
	push	{r4, r5, r6, lr}
	sub	sp, #64
	mov	r0, sp
	bl	Func_08006ac0
	ldr	r5, [pc, #76]
	ldrh	r0, [r5, #0]
	ldr	r6, [pc, #76]
	ands	r0, r6
	ldr	r1, [pc, #76]
	ldrh	r1, [r1, #36]
	orrs	r0, r1
	strh	r0, [r5, #0]
	ldr	r1, [pc, #72]
	movs	r4, #170
	strb	r4, [r1, #0]
	ldr	r3, [pc, #68]
	movs	r2, #85
	strb	r2, [r3, #0]
	movs	r0, #128
	strb	r0, [r1, #0]
	strb	r4, [r1, #0]
	strb	r2, [r3, #0]
	movs	r0, #16
	strb	r0, [r1, #0]
	ldr	r0, [pc, #56]
	movs	r1, #224
	lsls	r1, r1, #20
	ldr	r3, [r0, #0]
	movs	r0, #3
	movs	r2, #255
	bl	Func_080072f0
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	ldrh	r1, [r5, #0]
	ands	r1, r6
	movs	r2, #3
	orrs	r1, r2
	strh	r1, [r5, #0]
	add	sp, #64
	pop	{r4, r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x04000204
	.4byte 0x0000fffc
	.4byte 0x08007c10
	.4byte 0x0e005555
	.4byte 0x0e002aaa
	.4byte 0x02004c00

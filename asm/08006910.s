@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08006910
	.thumb_func
Func_08006910:
	push	{r4, lr}
	ldr	r2, [pc, #28]
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #28]
	ands	r0, r1
	movs	r1, #3
	orrs	r0, r1
	strh	r0, [r2, #0]
	bl	Func_08006878
	lsls	r0, r0, #16
	lsrs	r3, r0, #16
	ldr	r2, [pc, #12]
	movs	r4, #1
	b.n	.L0
	movs	r0, r0
	.4byte 0x04000204
	.4byte 0x0000fffc
	.4byte 0x08007a0c
.L2:
	adds	r2, #4
.L0:
	ldr	r1, [r2, #0]
	adds	r0, r1, #0
	adds	r0, #40
	ldrb	r0, [r0, #0]
	cmp	r0, #0
	beq.n	.L1
	ldrh	r0, [r1, #40]
	cmp	r3, r0
	bne.n	.L2
	movs	r4, #0
.L1:
	ldr	r1, [pc, #56]
	ldr	r0, [r2, #0]
	ldr	r0, [r0, #0]
	str	r0, [r1, #0]
	ldr	r1, [pc, #52]
	ldr	r0, [r2, #0]
	ldr	r0, [r0, #4]
	str	r0, [r1, #0]
	ldr	r1, [pc, #48]
	ldr	r0, [r2, #0]
	ldr	r0, [r0, #8]
	str	r0, [r1, #0]
	ldr	r1, [pc, #44]
	ldr	r0, [r2, #0]
	ldr	r0, [r0, #12]
	str	r0, [r1, #0]
	ldr	r1, [pc, #40]
	ldr	r0, [r2, #0]
	ldr	r0, [r0, #16]
	str	r0, [r1, #0]
	ldr	r1, [pc, #36]
	ldr	r0, [r2, #0]
	adds	r0, #20
	str	r0, [r1, #0]
	adds	r0, r4, #0
	pop	{r4}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x02004c04
	.4byte 0x02004c10
	.4byte 0x02004c14
	.4byte 0x02004c00
	.4byte 0x02004c18
	.4byte 0x02004c08

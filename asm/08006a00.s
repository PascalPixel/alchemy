@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08006a00
	.thumb_func
Func_08006a00:
	push	{r4, r5, lr}
	lsls	r0, r0, #24
	lsrs	r0, r0, #24
	ldr	r1, [pc, #80]
	lsls	r2, r0, #1
	adds	r2, r2, r0
	lsls	r2, r2, #1
	ldr	r0, [r1, #0]
	adds	r2, r2, r0
	ldr	r1, [pc, #72]
	ldr	r3, [pc, #72]
	ldrh	r0, [r3, #0]
	strh	r0, [r1, #0]
	movs	r5, #0
	strh	r5, [r3, #0]
	ldr	r4, [pc, #68]
	ldr	r0, [pc, #68]
	ldrb	r0, [r0, #0]
	movs	r1, #8
	lsls	r1, r0
	ldrh	r0, [r4, #0]
	orrs	r0, r1
	strh	r0, [r4, #0]
	movs	r0, #1
	strh	r0, [r3, #0]
	ldr	r0, [pc, #56]
	strb	r5, [r0, #0]
	ldr	r1, [pc, #56]
	ldrh	r0, [r2, #0]
	strh	r0, [r1, #0]
	adds	r2, #2
	ldr	r3, [pc, #52]
	ldr	r0, [r3, #0]
	ldrh	r1, [r2, #0]
	strh	r1, [r0, #0]
	adds	r0, #2
	str	r0, [r3, #0]
	ldrh	r1, [r2, #2]
	strh	r1, [r0, #0]
	subs	r0, #2
	str	r0, [r3, #0]
	pop	{r4, r5}
	pop	{r0}
	bx	r0
	.4byte 0x02004c18
	.4byte 0x02004c2c
	.4byte 0x04000208
	.4byte 0x04000200
	.4byte 0x02004c20
	.4byte 0x02004c24
	.4byte 0x02004c22
	.4byte 0x02004c28

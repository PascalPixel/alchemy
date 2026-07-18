@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08003538
	.thumb_func
Func_08003538:
	push	{r5, lr}
	ldr	r4, [pc, #240]
	ldr	r0, [r4, #0]
	movs	r5, #0
	cmp	r0, #0
	bgt.n	.L0
	ldr	r2, [pc, #232]
	ldr	r3, [pc, #236]
	ldr	r2, [r2, #0]
	str	r2, [r3, #0]
	ldr	r1, [r3, #0]
	cmp	r0, #0
	bne.n	.L1
	movs	r3, #6
	str	r3, [r4, #0]
	b.n	.L2
.L1:
	movs	r3, #19
	str	r3, [r4, #0]
	b.n	.L2
.L0:
	ldr	r3, [pc, #212]
	str	r5, [r3, #0]
	ldr	r1, [r3, #0]
.L2:
	cmp	r1, #0
	beq.n	.L3
	movs	r3, #64
	ands	r3, r1
	movs	r2, #0
	cmp	r3, #0
	beq.n	.L4
	movs	r2, #1
.L4:
	movs	r3, #128
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L5
	adds	r2, #1
.L5:
	movs	r3, #32
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L6
	adds	r2, #1
.L6:
	movs	r3, #16
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L7
	adds	r2, #1
.L7:
	ldr	r0, [pc, #164]
	str	r1, [r0, #0]
	cmp	r2, #1
	beq.n	.L8
	cmp	r2, #1
	bcc.n	.L9
	cmp	r2, #2
	beq.n	.L10
	cmp	r2, #3
	beq.n	.L11
	ldr	r2, [pc, #148]
	movs	r3, #48
	str	r3, [r2, #0]
	ldr	r2, [pc, #144]
	b.n	.L12
.L9:
	ldr	r2, [pc, #136]
	movs	r3, #48
	str	r3, [r2, #0]
	b.n	.L13
.L8:
	ldr	r2, [pc, #128]
	movs	r3, #240
	ands	r1, r3
	str	r1, [r2, #0]
	b.n	.L13
.L10:
	ldr	r1, [pc, #120]
	ldr	r3, [r1, #0]
	ldr	r2, [r0, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L14
	movs	r3, #48
	str	r3, [r1, #0]
.L14:
	ldr	r3, [r1, #0]
	ldr	r2, [pc, #108]
	eors	r3, r2
	ldr	r2, [r0, #0]
	ands	r2, r3
	str	r2, [r0, #0]
	b.n	.L13
.L11:
	ldr	r4, [pc, #88]
	ldr	r3, [r4, #0]
	movs	r2, #48
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L15
	movs	r5, #48
.L15:
	ldr	r3, [r4, #0]
	movs	r2, #192
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L16
	movs	r5, #192
.L16:
	ldr	r2, [pc, #72]
	eors	r2, r5
	ands	r1, r2
	str	r1, [r4, #0]
.L12:
	ldr	r3, [r0, #0]
	ands	r3, r2
	str	r3, [r0, #0]
	b.n	.L13
.L3:
	ldr	r3, [pc, #44]
	str	r1, [r3, #0]
.L13:
	ldr	r1, [pc, #32]
	ldr	r0, [pc, #52]
	ldr	r3, [r1, #0]
	ldr	r2, [r0, #0]
	eors	r3, r2
	ldr	r2, [r1, #0]
	ldr	r4, [pc, #48]
	ands	r3, r2
	str	r3, [r4, #0]
	ldr	r3, [r1, #0]
	str	r3, [r0, #0]
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001b00
	.4byte 0x03001ae8
	.4byte 0x03001b04
	.4byte 0x03001afc
	.4byte 0x03001d04
	.4byte 0x0000ff0f
	.4byte 0x0000ffff
	.4byte 0x03001cf4
	.4byte 0x03001c94

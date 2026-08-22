@ 転送状態を退避するサム補助処理。後続処理と状態領域の番地を共有する。
.syntax unified
	.thumb
	.align	2
	.global Func_08002dd8
	.thumb_func
Func_08002dd8:
	ldr	r4, .L_transfer_state
	lsls	r0, r0, #2
	ldr	r1, [r0, r4]
	lsrs	r3, r1, #22
	beq.n	.L_return
	movs	r2, #0
	str	r2, [r0, r4]
	movs	r0, #4
	ands	r3, r0
	str	r1, [r3, r4]
.L_return:
	bx	lr
	movs	r0, r0

@ 転送状態を記録するサム補助処理。
	.global Func_08002df0
	.thumb_func
Func_08002df0:
	ldr	r4, .L_transfer_state
	movs	r1, #4
	lsrs	r2, r0, #22
	ands	r2, r1
	str	r0, [r2, r4]
	bx	lr
.L_transfer_state:
	.4byte 0x03001e50

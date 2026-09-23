.syntax unified
	.thumb
	.set sub_080e0524, 0x080e0524
	.set sub_080e4b4c, 0x080e4b4c
	.set sub_080e4b54, 0x080e4b54
	.global Fragment_080e4ab8
	.global Func_080e4ab8
	.thumb_func
Fragment_080e4ab8:
Func_080e4ab8:
	ldr	r0, [pc, #120]
	b.n	.L_080e4ac6
	ldr	r0, [pc, #120]
	b.n	.L_080e4ac6
	ldr	r0, [pc, #120]
	b.n	.L_080e4ac6
	ldr	r0, [pc, #120]
.L_080e4ac6:
	ldr	r1, [pc, #124]
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	b.n	.L_080e4ade
	ldr	r0, [pc, #100]
	ldr	r1, [pc, #108]
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
.L_080e4ade:
	ldr	r3, [sp, #92]
	movs	r5, #239
	lsls	r5, r5, #7
	adds	r2, r3, r5
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r0, [sp, #96]
	cmp	r0, #12
	bne.n	sub_080e4b4c
	ldr	r1, [sp, #92]
	ldr	r3, [pc, #84]
	adds	r2, r1, r3
	movs	r3, #75
	b.n	sub_080e4b54
	movs	r0, r0
	lsls	r5, r7, #1
	movs	r0, r0
	lsls	r1, r5, #2
	movs	r0, r0
	lsls	r6, r1, #3
	movs	r0, r0
	movs	r0, r0
	lsls	r1, r0, #8
	lsls	r4, r0, #3
	movs	r0, r0
	lsrs	r6, r2, #17
	lsls	r1, r0, #8
	lsls	r1, r7, #1
	movs	r0, r0
	lsls	r3, r0, #3
	movs	r0, r0
	lsls	r7, r5, #1
	movs	r0, r0
	lsls	r0, r7, #2
	movs	r0, r0
	lsls	r4, r6, #2
	movs	r0, r0
	lsls	r3, r2, #1
	movs	r0, r0
	lsls	r6, r3, #2
	movs	r0, r0
	ldr	r1, [pc, #144]
	lsrs	r6, r1, #32
	.4byte 0x00000094
	.4byte 0x00000092
	.4byte 0x0000008e
	.4byte 0x00000090
	.4byte 0x02013c56
	.4byte 0x00007784

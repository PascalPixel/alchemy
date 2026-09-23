.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.set sub_0200058e, 0x0200058e
	.set sub_020005e6, 0x020005e6
	.set sub_02000606, 0x02000606
	.set sub_02000640, 0x02000640
	.set sub_0200065c, 0x0200065c
	.set sub_0200066e, 0x0200066e
	.set sub_02000672, 0x02000672
	.set sub_02000682, 0x02000682
	.set sub_0200068e, 0x0200068e
	.set sub_020006a0, 0x020006a0
	.set sub_020006a8, 0x020006a8
	.set sub_020006b0, 0x020006b0
	.set sub_020006b8, 0x020006b8
	.set sub_020006c4, 0x020006c4
	.set sub_020006c8, 0x020006c8
	.set sub_020006d6, 0x020006d6
	.set sub_020006da, 0x020006da
	.set sub_020006ea, 0x020006ea
	.set sub_020006f6, 0x020006f6
	.set sub_02000708, 0x02000708
	.set sub_02000710, 0x02000710
	.set sub_02000718, 0x02000718
	.set sub_0200071e, 0x0200071e
	.set sub_02000720, 0x02000720
	.set sub_02000730, 0x02000730
	.set sub_02000734, 0x02000734
	.set sub_0200073e, 0x0200073e
	.set sub_02000744, 0x02000744
	.set sub_02000756, 0x02000756
	.set sub_02000764, 0x02000764
	.set sub_0200077a, 0x0200077a
	.set sub_0200077c, 0x0200077c
	.set sub_02000798, 0x02000798
	.set sub_0200079a, 0x0200079a
	.set sub_020007be, 0x020007be
	.set sub_020007c4, 0x020007c4
	.set sub_020007d6, 0x020007d6
	.set sub_020007da, 0x020007da
	.set sub_020007f8, 0x020007f8
	.set sub_020007fa, 0x020007fa
	.set sub_02000802, 0x02000802
	.set sub_0200081e, 0x0200081e
	.set sub_02000824, 0x02000824
	.set sub_02000836, 0x02000836
	.set sub_02000856, 0x02000856
	.set sub_02000858, 0x02000858
	.set sub_02000862, 0x02000862
	.set sub_0200087a, 0x0200087a
	.set sub_02000880, 0x02000880
	.set sub_02000892, 0x02000892
	.set sub_020008b2, 0x020008b2
	.set sub_020008b4, 0x020008b4
	.set sub_020008be, 0x020008be
	.set sub_020008c8, 0x020008c8
	.set sub_020008d6, 0x020008d6
	.set sub_020008e4, 0x020008e4
	.set sub_020008ea, 0x020008ea
	.set sub_020008ee, 0x020008ee
	.set sub_02000910, 0x02000910
	.set sub_02000924, 0x02000924
	.set sub_02000936, 0x02000936
	.set sub_02000938, 0x02000938
	.set sub_0200093a, 0x0200093a
	.set sub_0200093c, 0x0200093c
	.set sub_02000940, 0x02000940
	.set sub_02000948, 0x02000948
	.set sub_02000950, 0x02000950
	.set sub_0200095c, 0x0200095c
	.set sub_0200096c, 0x0200096c
	.set sub_0200098c, 0x0200098c
	.set sub_02000996, 0x02000996
	.set sub_02000998, 0x02000998
	.set sub_020009a4, 0x020009a4
	.set sub_020009c6, 0x020009c6
	.set sub_020009ce, 0x020009ce
	.set sub_020009d8, 0x020009d8
	.set sub_020009e4, 0x020009e4
	.set sub_020009f0, 0x020009f0
	.set sub_020009fa, 0x020009fa
	.set sub_02000a04, 0x02000a04
	.set sub_02000a06, 0x02000a06
	.set sub_02000a12, 0x02000a12
	.set sub_02000a1c, 0x02000a1c
	.set sub_02000a1e, 0x02000a1e
	.set sub_02000a2a, 0x02000a2a
	.set sub_02000a34, 0x02000a34
	.set sub_02000a4a, 0x02000a4a
	.set sub_02000a62, 0x02000a62
	.set sub_02000a6c, 0x02000a6c
	.set sub_02000ab0, 0x02000ab0
	.set sub_02000abe, 0x02000abe
	.set sub_02000ad6, 0x02000ad6
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008481, 0x02008039, 0x02008045, 0x0200804d, 0x020080b9, 0x02008041, 0x02008521
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x865c
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x868c
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #40]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #10
	bne.n	.L_02000062
	ldr	r0, [pc, #28]
	b.n	.L_02000076
.L_02000062:
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #255
	bl	sub_0200058e
	cmp	r0, #0
	beq.n	.L_02000074
	ldr	r0, [pc, #12]
	b.n	.L_02000076
.L_02000074:
	ldr	r0, [pc, #12]
.L_02000076:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x020088c4
	.4byte 0x02008954
	.2byte 0x86cc
	.2byte 0x0200
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #208
	lsls	r1, r1, #4
	adds	r1, #56
	adds	r2, r3, r1
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r3, [pc, #20]
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #10
	beq.n	.L_020000b0
	movs	r3, #0
	strb	r3, [r2, #0]
.L_020000b0:
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #255
	bl	sub_020005e6
	cmp	r0, #0
	beq.n	.L_020000cc
	ldr	r0, [pc, #4]
	b.n	.L_020000ce
.L_020000cc:
	ldr	r0, [pc, #4]
.L_020000ce:
	pop	{pc}
	.4byte 0x02008cfc
	.2byte 0x8b4c
	.2byte 0x0200
	push	{r5, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #93
	bl	sub_02000606
	cmp	r0, #0
	bne.n	.L_02000128
	ldr	r5, [pc, #76]
	adds	r0, r5, #0
	bl	sub_02000672
	movs	r1, #0
	movs	r0, #14
	bl	sub_02000682
	bl	sub_020006d6
	movs	r1, #0
	bl	sub_0200065c
	cmp	r0, #0
	bne.n	.L_02000118
	adds	r0, r5, #1
	bl	sub_0200068e
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #93
	bl	sub_02000640
	b.n	.L_0200011e
.L_02000118:
	adds	r0, r5, #2
	bl	sub_020006a0
.L_0200011e:
	movs	r0, #14
	movs	r1, #0
	bl	sub_020006b8
	b.n	.L_02000136
.L_02000128:
	ldr	r0, [pc, #16]
	bl	sub_020006b0
	movs	r0, #14
	movs	r1, #0
	bl	sub_020006c8
.L_02000136:
	pop	{r5, pc}
	.4byte 0x00001d8b
	.2byte 0x1d8e
	.2byte 0x0000
	push	{r5, lr}
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #94
	bl	sub_0200066e
	cmp	r0, #0
	bne.n	.L_02000190
	ldr	r5, [pc, #76]
	adds	r0, r5, #0
	bl	sub_020006da
	movs	r1, #0
	movs	r0, #18
	bl	sub_020006ea
	bl	sub_0200073e
	movs	r1, #0
	bl	sub_020006c4
	cmp	r0, #0
	bne.n	.L_02000180
	adds	r0, r5, #1
	bl	sub_020006f6
	movs	r0, #128
	lsls	r0, r0, #4
	adds	r0, #93
	bl	sub_020006a8
	b.n	.L_02000186
.L_02000180:
	adds	r0, r5, #2
	bl	sub_02000708
.L_02000186:
	movs	r0, #18
	movs	r1, #0
	bl	sub_02000720
	b.n	.L_0200019e
.L_02000190:
	ldr	r0, [pc, #16]
	bl	sub_02000718
	movs	r0, #14
	movs	r1, #0
	bl	sub_02000730
.L_0200019e:
	pop	{r5, pc}
	.4byte 0x00001d92
	.2byte 0x1d95
	.2byte 0x0000
	push	{r5, lr}
	ldr	r5, [pc, #64]
	adds	r0, r5, #0
	bl	sub_02000734
	movs	r1, #0
	movs	r0, #19
	bl	sub_02000744
	bl	sub_02000798
	movs	r1, #0
	bl	sub_0200071e
	cmp	r0, #0
	bne.n	.L_020001d6
	movs	r0, #10
	bl	sub_02000710
	adds	r0, r5, #1
	bl	sub_02000756
	b.n	.L_020001e2
.L_020001d6:
	movs	r0, #20
	bl	sub_0200071e
	adds	r0, r5, #2
	bl	sub_02000764
.L_020001e2:
	movs	r0, #19
	movs	r1, #0
	bl	sub_0200077c
	pop	{r5, pc}
	.2byte 0x1d96
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl	sub_02000764
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #24]
	ands	r3, r2
	movs	r2, #128
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_0200022c
	movs	r0, #16
	adds	r1, r5, #0
	bl	sub_02000802
	b.n	.L_02000248
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_0200022c:
	bl	sub_0200077a
	movs	r0, #0
	bl	sub_020007f8
	ldr	r0, [pc, #20]
	bl	sub_020007be
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_020007d6
	bl	sub_0200079a
.L_02000248:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1e04
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl	sub_020007c4
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #24]
	ands	r3, r2
	movs	r2, #128
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_0200028c
	movs	r0, #16
	adds	r1, r5, #0
	bl	sub_02000862
	b.n	.L_020002a8
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_0200028c:
	bl	sub_020007da
	movs	r0, #0
	bl	sub_02000858
	ldr	r0, [pc, #20]
	bl	sub_0200081e
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_02000836
	bl	sub_020007fa
.L_020002a8:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1e2c
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl	sub_02000824
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #20]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_020002e8
	adds	r0, r5, #0
	bl	sub_020008c8
	b.n	.L_02000304
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_020002e8:
	bl	sub_02000836
	movs	r0, #0
	bl	sub_020008b4
	ldr	r0, [pc, #20]
	bl	sub_0200087a
	movs	r0, #26
	movs	r1, #0
	bl	sub_02000892
	bl	sub_02000856
.L_02000304:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1e0b
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl	sub_02000880
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #20]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_02000344
	adds	r0, r5, #0
	bl	sub_02000924
	b.n	.L_02000360
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_02000344:
	bl	sub_02000892
	movs	r0, #0
	bl	sub_02000910
	ldr	r0, [pc, #20]
	bl	sub_020008d6
	movs	r0, #26
	movs	r1, #0
	bl	sub_020008ee
	bl	sub_020008b2
.L_02000360:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x1e33
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #108]
	bl	sub_020008be
	movs	r0, #0
	bl	sub_0200093c
	movs	r5, #8
.L_0200037c:
	adds	r0, r5, #0
	bl	sub_020008e4
	cmp	r0, #0
	beq.n	.L_0200038e
	adds	r2, r0, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
.L_0200038e:
	adds	r5, #1
	cmp	r5, #63
	bls.n	.L_0200037c
	movs	r3, #170
	lsls	r3, r3, #1
	adds	r6, r6, r3
	movs	r3, #0
	ldrsh	r5, [r6, r3]
	movs	r0, #158
	bl	sub_02000996
	subs	r5, #4
	ldr	r0, [pc, #80]
	lsls	r5, r5, #3
	adds	r3, r5, #4
	ldrh	r1, [r0, r3]
	adds	r3, r3, r0
	ldrh	r2, [r3, #2]
	ldr	r0, [r0, r5]
	bl	sub_020008ea
	ldr	r5, [pc, #64]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #7
	ldr	r0, [r5, #0]
	lsls	r1, r1, #8
	bl	sub_02000938
	ldr	r0, [r5, #0]
	bl	sub_02000936
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #2
	ldr	r0, [r5, #0]
	bl	sub_0200095c
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	bl	sub_0200098c
	bl	sub_02000998
	bl	sub_020009a4
	bl	sub_02000948
	pop	{r5, r6, pc}
	.4byte 0x0200864c
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	bl	sub_02000950
	movs	r0, #0
	bl	sub_020009ce
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl	sub_02000940
	bl	sub_0200096c
	pop	{pc}
	.4byte 0x049b23c0
	.4byte 0x23d06eda
	.4byte 0x3331011b
	.4byte 0x230018d1
	.4byte 0x23d0700b
	.4byte 0x3332011b
	.4byte 0x231818d1
	.4byte 0x23d0700b
	.4byte 0x3333011b
	.4byte 0x231918d2
	.2byte 0x7013
	.2byte 0x4770
	push	{lr}
	movs	r0, #27
	movs	r1, #0
	movs	r2, #0
	bl	sub_020009c6
	movs	r1, #208
	movs	r2, #208
	movs	r0, #65
	lsls	r1, r1, #15
	lsls	r2, r2, #15
	bl	sub_02000a1c
	pop	{pc}
	push	{lr}
	movs	r0, #65
	movs	r1, #0
	movs	r2, #0
	bl	sub_02000a2a
	movs	r1, #208
	movs	r2, #208
	movs	r0, #27
	lsls	r1, r1, #15
	lsls	r2, r2, #15
	bl	sub_020009f0
	pop	{pc}
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	subs	r2, #172
	str	r2, [r3, #0]
	movs	r0, #9
	bl	sub_020009fa
	movs	r1, #0
	bl	sub_020009d8
	movs	r0, #10
	bl	sub_02000a06
	movs	r1, #0
	bl	sub_020009e4
	movs	r0, #23
	bl	sub_02000a12
	movs	r1, #0
	bl	sub_020009f0
	movs	r0, #23
	bl	sub_02000a1e
	ldr	r3, [pc, #92]
	movs	r5, #128
	str	r3, [r0, #28]
	movs	r0, #27
	bl	sub_02000a2a
	lsls	r5, r5, #8
	str	r5, [r0, #24]
	movs	r0, #27
	bl	sub_02000a34
	str	r5, [r0, #28]
	bl	sub_0200093a
	movs	r0, #129
	lsls	r0, r0, #1
	adds	r0, #255
	bl	sub_02000a04
	cmp	r0, #0
	beq.n	.L_020004f0
	movs	r0, #23
	movs	r1, #0
	movs	r2, #0
	bl	sub_02000a62
.L_020004f0:
	bl	sub_02000abe
	movs	r1, #128
	lsls	r1, r1, #2
	movs	r2, #24
	movs	r3, #25
	adds	r1, #2
	movs	r0, #0
	bl	sub_02000ad6
	movs	r0, #24
	bl	sub_02000a6c
	movs	r1, #0
	bl	sub_02000a4a
	movs	r0, #25
	bl	sub_02000ab0
	movs	r0, #0
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x9999
	.2byte 0x0001
	movs	r0, #0
	bx	lr
	.irp EntryTarget, 0x080003c9, 0x080003d1, 0x08020171, 0x08020219, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80f9, 0x080c8119, 0x080c8181, 0x080c8189, 0x080c81a1, 0x080c8209, 0x080c8279, 0x080c83b1, 0x080c83b9, 0x080c8409, 0x080c84e1, 0x080c86a9, 0x080c86e9, 0x080c8779, 0x08108009, 0x08108011, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x0200001a
	.4byte 0x0000ffff
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00180000
	.4byte 0x00000011
	.4byte 0x00000027
	.4byte 0x00000003
	.4byte 0x00000011
	.4byte 0x002a0008
	.4byte 0x00020001
	.4byte 0xffff0006
	.4byte 0x02008640
	.4byte 0x00330012
	.4byte 0x02008640
	.4byte 0x00220015
	.4byte 0xffff0000
	.4byte 0x000000e0
	.4byte 0x400000be
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000069
	.4byte 0x1010106a
	.4byte 0xffffffff
	.4byte 0x1020206a
	.4byte 0xffffffff
	.4byte 0x1030306a
	.4byte 0xffffffff
	.4byte 0x1040406a
	.4byte 0xffffffff
	.4byte 0x1050506a
	.4byte 0xffffffff
	.4byte 0x10617002
	.4byte 0xffffffff
	.4byte 0x10718002
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00f60000
	.4byte 0x00004000
	.4byte 0xffff0130
	.4byte 0x02008604
	.4byte 0x013c0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff0130
	.4byte 0x02008604
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff019a
	.4byte 0x02008610
	.4byte 0x013c0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff019a
	.4byte 0x02008610
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00014000
	.4byte 0xffff0074
	.4byte 0x00000003
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00014000
	.4byte 0xffff0080
	.4byte 0x00000003
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0000e000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00012000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00012000
	.4byte 0xffff0088
	.4byte 0x00000002
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0000e000
	.4byte 0xffff0093
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00016000
	.4byte 0xffff0070
	.4byte 0x00000002
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00010000
	.4byte 0xffff0133
	.4byte 0x02008634
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00014000
	.4byte 0xffff0148
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff016c
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00f60000
	.4byte 0x00004000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x013c0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff019a
	.4byte 0x00000001
	.4byte 0x013c0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff019a
	.4byte 0x00000001
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00f60000
	.4byte 0x00004000
	.4byte 0xffff0130
	.4byte 0x02008604
	.4byte 0x013c0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff0130
	.4byte 0x02008604
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff019a
	.4byte 0x02008610
	.4byte 0x013c0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff019a
	.4byte 0x02008610
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00014000
	.4byte 0xffff0074
	.4byte 0x00000003
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00014000
	.4byte 0xffff0080
	.4byte 0x00000003
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0000e000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00012000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00012000
	.4byte 0xffff0088
	.4byte 0x00000002
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0000e000
	.4byte 0xffff0093
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00016000
	.4byte 0xffff0070
	.4byte 0x00000002
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00010000
	.4byte 0xffff0133
	.4byte 0x02008634
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00014000
	.4byte 0xffff0148
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008369
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008369
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001d8a
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x020080d9
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001d8f
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001d90
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001d91
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02008141
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x020081a9
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001d99
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00001da2
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x020081f1
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x020082b1
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001d9a
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001d9b
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001d9c
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001d9d
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001d9e
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001d9f
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001da0
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001da1
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001da3
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001e05
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x00001e0c
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.4byte 0x00403041
	.4byte 0x10009a15
	.4byte 0xffff0008
	.4byte 0x02008089
	.4byte 0x00004e15
	.4byte 0x02010017
	.4byte 0x02008401
	.4byte 0x00008515
	.4byte 0x02020018
	.4byte 0x00000000
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008449
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02008465
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008369
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008369
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001e0d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001e0e
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001e0f
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001e10
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001e11
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001e12
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001e13
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001e14
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00001e1d
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x02008251
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x0200830d
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001e15
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001e16
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001e17
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001e18
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001e19
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001e1a
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001e1b
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001e1c
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001e1e
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001e2d
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x00001e34
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.4byte 0x00403041
	.4byte 0x10009a15
	.4byte 0xffff0008
	.4byte 0x02008089
	.4byte 0x00004e15
	.4byte 0x02010017
	.4byte 0x02008401
	.4byte 0x00008515
	.4byte 0x02020018
	.4byte 0x00000000
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008449
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02008465
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000

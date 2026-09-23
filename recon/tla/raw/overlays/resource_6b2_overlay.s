.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.set sub_020005a8, 0x020005a8
	.set sub_020005dc, 0x020005dc
	.set sub_020005e4, 0x020005e4
	.set sub_02000670, 0x02000670
	.set sub_02000682, 0x02000682
	.set sub_02000684, 0x02000684
	.set sub_020006b4, 0x020006b4
	.set sub_020006bc, 0x020006bc
	.set sub_020006c0, 0x020006c0
	.set sub_020006d2, 0x020006d2
	.set sub_020006fa, 0x020006fa
	.set sub_02000712, 0x02000712
	.set sub_02000722, 0x02000722
	.set sub_0200073e, 0x0200073e
	.set sub_02000764, 0x02000764
	.set sub_0200077c, 0x0200077c
	.set sub_020007aa, 0x020007aa
	.set sub_020007b2, 0x020007b2
	.set sub_020007b8, 0x020007b8
	.set sub_020007c2, 0x020007c2
	.set sub_020007d2, 0x020007d2
	.set sub_020007ec, 0x020007ec
	.set sub_020007f2, 0x020007f2
	.set sub_02000802, 0x02000802
	.set sub_02000806, 0x02000806
	.set sub_0200080e, 0x0200080e
	.set sub_02000810, 0x02000810
	.set sub_02000824, 0x02000824
	.set sub_02000834, 0x02000834
	.set sub_02000836, 0x02000836
	.set sub_0200083c, 0x0200083c
	.set sub_0200083e, 0x0200083e
	.set sub_02000848, 0x02000848
	.set sub_02000850, 0x02000850
	.set sub_0200085c, 0x0200085c
	.set sub_02000866, 0x02000866
	.set sub_02000868, 0x02000868
	.set sub_02000870, 0x02000870
	.set sub_0200087c, 0x0200087c
	.set sub_02000880, 0x02000880
	.set sub_02000896, 0x02000896
	.set sub_0200089a, 0x0200089a
	.set sub_020008a8, 0x020008a8
	.set sub_020008bc, 0x020008bc
	.set sub_020008d8, 0x020008d8
	.set sub_020008f4, 0x020008f4
	.set sub_0200090c, 0x0200090c
	.set sub_0200091c, 0x0200091c
	.set sub_0200092c, 0x0200092c
	.set sub_0200093c, 0x0200093c
	.set sub_0200094c, 0x0200094c
	.set sub_0200095c, 0x0200095c
	.set sub_02000960, 0x02000960
	.set sub_02000968, 0x02000968
	.set sub_0200096c, 0x0200096c
	.set sub_02000974, 0x02000974
	.set sub_0200097c, 0x0200097c
	.set sub_02000988, 0x02000988
	.set sub_02000994, 0x02000994
	.set sub_02000998, 0x02000998
	.set sub_0200099a, 0x0200099a
	.set sub_020009a4, 0x020009a4
	.set sub_020009a8, 0x020009a8
	.set sub_020009b4, 0x020009b4
	.set sub_020009c6, 0x020009c6
	.set sub_020009c8, 0x020009c8
	.set sub_020009ca, 0x020009ca
	.set sub_020009cc, 0x020009cc
	.set sub_020009d4, 0x020009d4
	.set sub_020009d6, 0x020009d6
	.set sub_020009da, 0x020009da
	.set sub_020009de, 0x020009de
	.set sub_020009e8, 0x020009e8
	.set sub_020009f2, 0x020009f2
	.set sub_020009fc, 0x020009fc
	.set sub_02000a02, 0x02000a02
	.set sub_02000a10, 0x02000a10
	.set sub_02000a24, 0x02000a24
	.set sub_02000a32, 0x02000a32
	.set sub_02000a40, 0x02000a40
	.set sub_02000a46, 0x02000a46
	.set sub_02000a5a, 0x02000a5a
	.set sub_02000a6c, 0x02000a6c
	.set sub_02000a6e, 0x02000a6e
	.set sub_02000a78, 0x02000a78
	.set sub_02000a84, 0x02000a84
	.set sub_02000ac2, 0x02000ac2
	.set sub_02000ac8, 0x02000ac8
	.set sub_02000ad2, 0x02000ad2
	.set sub_02000ad6, 0x02000ad6
	.set sub_02000afc, 0x02000afc
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x020084c5, 0x020080d1, 0x020080dd, 0x020080e5, 0x020080ed, 0x020080d9, 0x0200851d
	overlay_veneer \EntryTarget
	.endr
	push	{lr}
	ldr	r3, [pc, #32]
	movs	r2, #1
	ldr	r3, [r3, #0]
	lsrs	r3, r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000050
	movs	r1, #15
	bl	sub_020005dc
	b.n	.L_02000056
.L_02000050:
	movs	r1, #7
	bl	sub_020005e4
.L_02000056:
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x87e4
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	adds	r5, r6, #0
	adds	r5, #98
	ldrb	r3, [r5, #0]
	adds	r7, r3, #0
	cmp	r7, #0
	beq.n	.L_02000076
	adds	r3, #255
	strb	r3, [r5, #0]
	b.n	.L_020000ce
.L_02000076:
	bl	sub_020005a8
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #4
	lsrs	r3, r3, #16
	adds	r3, #40
	strb	r3, [r5, #0]
	adds	r1, r6, #0
	adds	r1, #99
	ldrb	r3, [r1, #0]
	cmp	r3, #1
	beq.n	.L_020000ae
	cmp	r3, #1
	bgt.n	.L_0200009a
	cmp	r3, #0
	beq.n	.L_020000a4
	b.n	.L_020000ce
.L_0200009a:
	cmp	r3, #2
	beq.n	.L_020000ba
	cmp	r3, #3
	beq.n	.L_020000c4
	b.n	.L_020000ce
.L_020000a4:
	movs	r3, #128
	lsls	r3, r3, #8
	strh	r3, [r6, #6]
	movs	r3, #1
	b.n	.L_020000cc
.L_020000ae:
	ldr	r2, [pc, #16]
	movs	r3, #176
	lsls	r3, r3, #8
	strh	r3, [r6, #6]
	strb	r2, [r1, #0]
	b.n	.L_020000ce
.L_020000ba:
	movs	r3, #3
	strh	r7, [r6, #6]
	b.n	.L_020000cc
	.2byte 0x0000
	.2byte 0x0000
.L_020000c4:
	movs	r3, #208
	lsls	r3, r3, #8
	strh	r3, [r6, #6]
	movs	r3, #2
.L_020000cc:
	strb	r3, [r1, #0]
.L_020000ce:
	pop	{r5, r6, r7, pc}
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x868c
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x86bc
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x86d0
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x87d8
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	movs	r0, #8
	movs	r1, #3
	bl	sub_020006b4
	movs	r1, #3
	movs	r0, #9
	bl	sub_020006bc
	ldr	r3, [pc, #152]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	sub_02000682
	movs	r1, #0
	bl	sub_02000670
	ldr	r3, [pc, #136]
	ldr	r7, [r3, #0]
	movs	r3, #15
	ands	r7, r3
	cmp	r7, #0
	bne.n	.L_02000194
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #162
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_02000684
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02000194
	ldr	r5, [r6, #80]
	bl	sub_02000670
	lsls	r3, r0, #2
	adds	r3, r3, r0
	movs	r2, #203
	lsls	r2, r2, #18
	lsls	r3, r3, #3
	adds	r1, r6, #0
	adds	r3, r3, r2
	adds	r1, #85
	strb	r7, [r1, #0]
	movs	r2, #174
	str	r3, [r6, #8]
	movs	r3, #160
	lsls	r2, r2, #18
	lsls	r3, r3, #16
	str	r3, [r6, #12]
	str	r2, [r6, #16]
	adds	r0, r6, #0
	movs	r1, #0
	bl	sub_020006c0
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	ldrb	r2, [r5, #9]
	strb	r3, [r1, #0]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	ldr	r1, [pc, #36]
	adds	r0, r6, #0
	strb	r3, [r5, #9]
	bl	sub_020006d2
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_020006d2
.L_02000194:
	ldr	r2, [pc, #20]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0300122c
	.4byte 0x02008618
	.2byte 0x87e4
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	bl	sub_02000722
	movs	r0, #0
	bl	sub_020007b8
	ldr	r3, [pc, #748]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl	sub_0200073e
	movs	r1, #15
	bl	sub_02000764
	movs	r0, #1
	bl	sub_020006fa
	ldr	r3, [pc, #724]
	movs	r6, #0
	movs	r1, #144
	str	r6, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #720]
	bl	sub_02000712
	movs	r0, #8
	movs	r1, #2
	bl	sub_020007aa
	movs	r0, #9
	movs	r1, #2
	bl	sub_020007b2
	movs	r2, #192
	lsls	r2, r2, #18
	movs	r3, #218
	mov	r8, r2
	lsls	r3, r3, #1
	ldr	r2, [r2, #108]
	mov	sl, r3
	mov	r1, sl
	movs	r3, #40
	str	r3, [r2, r1]
	bl	sub_020007f2
	bl	sub_02000806
	movs	r0, #40
	bl	sub_0200077c
	movs	r1, #14
	movs	r0, #13
	bl	sub_02000824
	ldr	r0, [pc, #660]
	bl	sub_020007c2
	movs	r0, #3
	movs	r1, #0
	bl	sub_020007d2
	bl	sub_0200083e
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #8
	bl	sub_02000802
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #20
	movs	r0, #8
	bl	sub_0200080e
	movs	r1, #192
	movs	r0, #8
	lsls	r1, r1, #6
	movs	r2, #0
	bl	sub_02000802
	movs	r2, #10
	movs	r0, #8
	movs	r1, #4
	bl	sub_020007ec
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #8
	movs	r1, #0
	bl	sub_02000810
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #11
	bl	sub_0200083c
	movs	r1, #2
	adds	r1, #255
	movs	r2, #40
	movs	r0, #12
	bl	sub_02000848
	movs	r1, #208
	movs	r0, #11
	lsls	r1, r1, #8
	movs	r2, #0
	bl	sub_0200083c
	movs	r1, #176
	movs	r2, #0
	movs	r0, #12
	lsls	r1, r1, #8
	bl	sub_02000848
	movs	r0, #13
	movs	r1, #14
	bl	sub_020008a8
	movs	r0, #3
	movs	r1, #0
	bl	sub_02000850
	bl	sub_020008bc
	movs	r0, #9
	movs	r1, #3
	bl	sub_02000834
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #9
	movs	r1, #0
	bl	sub_02000868
	movs	r0, #12
	movs	r1, #0
	bl	sub_02000880
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #12
	movs	r1, #0
	bl	sub_0200087c
	movs	r1, #128
	movs	r0, #11
	lsls	r1, r1, #8
	bl	sub_02000896
	movs	r0, #11
	movs	r1, #4
	bl	sub_02000866
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #11
	bl	sub_0200089a
	movs	r0, #11
	bl	sub_02000870
	adds	r7, r0, #0
	bl	sub_02000836
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #4
	adds	r2, r7, #0
	lsrs	r3, r3, #16
	adds	r2, #98
	ldr	r5, [pc, #424]
	adds	r3, #40
	strb	r3, [r2, #0]
	adds	r3, r7, #0
	adds	r3, #99
	strb	r6, [r3, #0]
	movs	r0, #12
	str	r5, [r7, #108]
	bl	sub_02000896
	adds	r7, r0, #0
	bl	sub_0200085c
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #4
	adds	r2, r7, #0
	lsrs	r3, r3, #16
	adds	r3, #40
	adds	r2, #98
	strb	r3, [r2, #0]
	movs	r3, #2
	adds	r2, #1
	strb	r3, [r2, #0]
	movs	r0, #13
	str	r5, [r7, #108]
	movs	r1, #14
	bl	sub_0200094c
	movs	r0, #3
	movs	r1, #0
	bl	sub_020008f4
	bl	sub_02000960
	movs	r0, #8
	movs	r1, #4
	bl	sub_020008d8
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #8
	movs	r1, #0
	bl	sub_0200090c
	movs	r0, #13
	movs	r1, #14
	bl	sub_02000974
	movs	r0, #3
	movs	r1, #0
	bl	sub_0200091c
	bl	sub_02000988
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #9
	movs	r1, #0
	bl	sub_0200092c
	movs	r0, #13
	movs	r1, #14
	bl	sub_02000994
	movs	r0, #3
	movs	r1, #0
	bl	sub_0200093c
	bl	sub_020009a8
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #8
	movs	r1, #0
	bl	sub_0200094c
	movs	r0, #13
	movs	r1, #14
	bl	sub_020009b4
	movs	r0, #3
	movs	r1, #0
	bl	sub_0200095c
	bl	sub_020009c8
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #9
	movs	r1, #0
	bl	sub_0200096c
	movs	r0, #13
	movs	r1, #14
	bl	sub_020009d4
	movs	r0, #3
	movs	r1, #0
	bl	sub_0200097c
	bl	sub_020009e8
	movs	r0, #8
	movs	r1, #3
	bl	sub_02000968
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #8
	movs	r1, #0
	bl	sub_02000994
	movs	r0, #13
	movs	r1, #14
	bl	sub_020009fc
	movs	r0, #1
	movs	r1, #0
	bl	sub_020009a4
	bl	sub_02000a10
	movs	r0, #9
	movs	r1, #2
	movs	r2, #0
	bl	sub_0200099a
	movs	r1, #8
	movs	r2, #20
	adds	r1, #255
	movs	r0, #9
	bl	sub_020009de
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #9
	movs	r1, #0
	bl	sub_020009ca
	movs	r0, #13
	movs	r1, #14
	bl	sub_02000a32
	movs	r0, #3
	movs	r1, #0
	bl	sub_020009da
	bl	sub_02000a46
	movs	r0, #8
	movs	r1, #3
	bl	sub_020009c6
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #8
	movs	r1, #0
	bl	sub_020009f2
	movs	r0, #13
	movs	r1, #14
	bl	sub_02000a5a
	movs	r1, #0
	movs	r0, #6
	bl	sub_02000a02
	bl	sub_02000a6e
	movs	r0, #20
	bl	sub_020009cc
	movs	r3, #160
	lsls	r3, r3, #19
	movs	r0, #1
	strh	r6, [r3, #0]
	bl	sub_02000998
	mov	r2, r8
	ldr	r3, [r2, #108]
	mov	r1, sl
	movs	r2, #16
	str	r2, [r3, r1]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #85
	str	r2, [r3, #0]
	bl	sub_02000a78
	bl	sub_02000a84
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #60
	bl	sub_020009d6
	movs	r0, #3
	bl	sub_02000a6c
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x020087e4
	.4byte 0x020080f5
	.4byte 0x00002e2f
	.2byte 0x8061
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #80]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #12
	bne.n	.L_0200050e
	bl	sub_02000a40
	movs	r0, #0
	bl	sub_02000ad6
	bl	sub_02000ac2
	bl	sub_02000ad6
	movs	r0, #201
	bl	sub_02000afc
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl	sub_02000ac8
	movs	r0, #20
	bl	sub_02000ad6
	movs	r0, #20
	bl	sub_02000a24
	movs	r0, #9
	bl	sub_02000ad2
	b.n	.L_02000512
.L_0200050e:
	bl	sub_020006c0
.L_02000512:
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	.irp EntryTarget, 0x080000c1, 0x080000d1, 0x080000f9, 0x080003d1, 0x08020091, 0x080200a9, 0x080200c1, 0x08020219, 0x080c8011, 0x080c8019, 0x080c8089, 0x080c8119, 0x080c8129, 0x080c8139, 0x080c8171, 0x080c8181, 0x080c81a1, 0x080c81d1, 0x080c81d9, 0x080c8201, 0x080c8211, 0x080c8279, 0x080c8379, 0x080c8391, 0x080c83a9, 0x080c83b1, 0x080c83b9, 0x080c84e1, 0x080c84e9, 0x080c84f1, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xfffe0000
	.4byte 0x0000002e
	.4byte 0x02008039
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xfffe0000
	.4byte 0x0000002e
	.4byte 0x02008039
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xffff0000
	.4byte 0x0000002e
	.4byte 0x02008039
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000028
	.4byte 0xc0010000
	.4byte 0x00000026
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000135
	.4byte 0x00202135
	.4byte 0x00305129
	.4byte 0x00902135
	.4byte 0x000001ff
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00025000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00023000
	.4byte 0xffff00be
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002b000
	.4byte 0xffff00bc
	.4byte 0x00000001
	.4byte 0x038a0000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00028000
	.4byte 0xffff00c2
	.4byte 0x00000001
	.4byte 0x030e0000
	.4byte 0x00000000
	.4byte 0x02fa0000
	.4byte 0x00020000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x03b20000
	.4byte 0x00024000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x03b20000
	.4byte 0x00024000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x030c0000
	.4byte 0x00000000
	.4byte 0x03b20000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x000a0000
	.4byte 0x00000000
	.4byte 0x000a0000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x000a0000
	.4byte 0x00000000
	.4byte 0x000a0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000

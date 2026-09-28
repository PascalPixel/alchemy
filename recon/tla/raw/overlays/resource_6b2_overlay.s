.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
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
	bl 0x02008590
	b.n	.L_02000056
.L_02000050:
	movs	r1, #7
	bl 0x02008590
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
	bl 0x02008530
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
	bl 0x020085b8
	movs	r1, #3
	movs	r0, #9
	bl 0x020085b8
	ldr	r3, [pc, #152]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl 0x02008570
	movs	r1, #0
	bl 0x02008558
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
	bl 0x02008550
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02000194
	ldr	r5, [r6, #80]
	bl 0x02008530
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
	bl 0x02008558
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
	bl 0x02008548
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x02008540
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
	bl 0x02008568
	movs	r0, #0
	bl 0x020085f8
	ldr	r3, [pc, #748]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl 0x02008570
	movs	r1, #15
	bl 0x02008590
	movs	r0, #1
	bl 0x02008520
	ldr	r3, [pc, #724]
	movs	r6, #0
	movs	r1, #144
	str	r6, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #720]
	bl 0x02008528
	movs	r0, #8
	movs	r1, #2
	bl 0x020085b8
	movs	r0, #9
	movs	r1, #2
	bl 0x020085b8
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
	bl 0x020085e0
	bl 0x020085f0
	movs	r0, #40
	bl 0x02008560
	movs	r1, #14
	movs	r0, #13
	bl 0x02008600
	ldr	r0, [pc, #660]
	bl 0x02008598
	movs	r0, #3
	movs	r1, #0
	bl 0x020085a0
	bl 0x02008608
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #0
	movs	r0, #8
	bl 0x020085c0
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #20
	movs	r0, #8
	bl 0x020085c0
	movs	r1, #192
	movs	r0, #8
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x020085a8
	movs	r2, #10
	movs	r0, #8
	movs	r1, #4
	bl 0x02008588
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #8
	movs	r1, #0
	bl 0x020085a0
	movs	r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r0, #11
	bl 0x020085c0
	movs	r1, #2
	adds	r1, #255
	movs	r2, #40
	movs	r0, #12
	bl 0x020085c0
	movs	r1, #208
	movs	r0, #11
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020085a8
	movs	r1, #176
	movs	r2, #0
	movs	r0, #12
	lsls	r1, r1, #8
	bl 0x020085a8
	movs	r0, #13
	movs	r1, #14
	bl 0x02008600
	movs	r0, #3
	movs	r1, #0
	bl 0x020085a0
	bl 0x02008608
	movs	r0, #9
	movs	r1, #3
	bl 0x02008578
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #9
	movs	r1, #0
	bl 0x020085a0
	movs	r0, #12
	movs	r1, #0
	bl 0x020085b0
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #12
	movs	r1, #0
	bl 0x020085a0
	movs	r1, #128
	movs	r0, #11
	lsls	r1, r1, #8
	bl 0x020085b0
	movs	r0, #11
	movs	r1, #4
	bl 0x02008578
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #11
	bl 0x020085a0
	movs	r0, #11
	bl 0x02008570
	adds	r7, r0, #0
	bl 0x02008530
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
	bl 0x02008570
	adds	r7, r0, #0
	bl 0x02008530
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
	bl 0x02008600
	movs	r0, #3
	movs	r1, #0
	bl 0x020085a0
	bl 0x02008608
	movs	r0, #8
	movs	r1, #4
	bl 0x02008578
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #8
	movs	r1, #0
	bl 0x020085a0
	movs	r0, #13
	movs	r1, #14
	bl 0x02008600
	movs	r0, #3
	movs	r1, #0
	bl 0x020085a0
	bl 0x02008608
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #9
	movs	r1, #0
	bl 0x020085a0
	movs	r0, #13
	movs	r1, #14
	bl 0x02008600
	movs	r0, #3
	movs	r1, #0
	bl 0x020085a0
	bl 0x02008608
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #8
	movs	r1, #0
	bl 0x020085a0
	movs	r0, #13
	movs	r1, #14
	bl 0x02008600
	movs	r0, #3
	movs	r1, #0
	bl 0x020085a0
	bl 0x02008608
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #9
	movs	r1, #0
	bl 0x020085a0
	movs	r0, #13
	movs	r1, #14
	bl 0x02008600
	movs	r0, #3
	movs	r1, #0
	bl 0x020085a0
	bl 0x02008608
	movs	r0, #8
	movs	r1, #3
	bl 0x02008580
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #8
	movs	r1, #0
	bl 0x020085a0
	movs	r0, #13
	movs	r1, #14
	bl 0x02008600
	movs	r0, #1
	movs	r1, #0
	bl 0x020085a0
	bl 0x02008608
	movs	r0, #9
	movs	r1, #2
	movs	r2, #0
	bl 0x02008588
	movs	r1, #8
	movs	r2, #20
	adds	r1, #255
	movs	r0, #9
	bl 0x020085c0
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #9
	movs	r1, #0
	bl 0x020085a0
	movs	r0, #13
	movs	r1, #14
	bl 0x02008600
	movs	r0, #3
	movs	r1, #0
	bl 0x020085a0
	bl 0x02008608
	movs	r0, #8
	movs	r1, #3
	bl 0x02008580
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #8
	movs	r1, #0
	bl 0x020085a0
	movs	r0, #13
	movs	r1, #14
	bl 0x02008600
	movs	r1, #0
	movs	r0, #6
	bl 0x020085a0
	bl 0x02008608
	movs	r0, #20
	bl 0x02008560
	movs	r3, #160
	lsls	r3, r3, #19
	movs	r0, #1
	strh	r6, [r3, #0]
	bl 0x02008520
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
	bl 0x020085e8
	bl 0x020085f0
	movs	r0, #160
	lsls	r0, r0, #4
	adds	r0, #60
	bl 0x02008538
	movs	r0, #3
	bl 0x020085c8
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
	bl 0x02008568
	movs	r0, #0
	bl 0x020085f8
	bl 0x020085e0
	bl 0x020085f0
	movs	r0, #201
	bl 0x02008610
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x020085d0
	movs	r0, #20
	bl 0x020085d8
	movs	r0, #20
	bl 0x02008520
	movs	r0, #9
	bl 0x020085c8
	b.n	.L_02000512
.L_0200050e:
	bl 0x020081b0
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

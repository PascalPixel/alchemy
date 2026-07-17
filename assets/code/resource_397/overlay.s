.syntax unified
	.thumb
	.set sub_0200018e, 0x0200018e
	.set sub_0200037c, 0x0200037c
	.set sub_020003ac, 0x020003ac
	.set sub_020003b4, 0x020003b4
	.set sub_020003da, 0x020003da
	.set sub_0200048c, 0x0200048c
	.set sub_02000496, 0x02000496
	.set sub_020004ae, 0x020004ae
	.set sub_020004e4, 0x020004e4
	.set sub_020004f0, 0x020004f0
	.set sub_020004fc, 0x020004fc
	.set sub_0200051e, 0x0200051e
	.set sub_02000560, 0x02000560
	.set sub_020005f6, 0x020005f6
	.set sub_020005f8, 0x020005f8
	.global Overlay_02000000
Overlay_02000000:
	.4byte 0x47204c00
	.4byte 0x02008201
	.4byte 0x47204c00
	.4byte 0x02008041
	.4byte 0x47204c00
	.4byte 0x0200804d
	.4byte 0x47204c00
	.4byte 0x02008055
	.4byte 0x47204c00
	.4byte 0x020081b5
	.4byte 0x47204c00
	.2byte 0x8049
	.2byte 0x0200
	push	{lr}
	movs	r0, #8
	movs	r1, #61
	bl	sub_0200037c
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x47704800
	.4byte 0x0200835c
	.4byte 0x47702000
	.4byte 0x47704800
	.4byte 0x0200844c
	.4byte 0x47704800
	.2byte 0x8474
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #108]
	ldr	r3, [r3, #0]
	movs	r2, #250
	mov	r8, r3
	ldr	r3, [pc, #104]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r6, r0, #0
	ldr	r0, [r3, #0]
	adds	r7, r1, #0
	bl	sub_020003ac
	adds	r5, r0, #0
	lsls	r6, r6, #20
	lsls	r7, r7, #20
	cmp	r5, #0
	beq.n	.L_0200009e
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	adds	r1, r1, r6
	adds	r2, r2, r7
	str	r1, [r5, #8]
	str	r2, [r5, #16]
	adds	r3, r5, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	bl	sub_020003b4
	str	r0, [r5, #12]
	str	r0, [r5, #20]
.L_0200009e:
	movs	r3, #240
	lsls	r3, r3, #1
	add	r3, r8
	ldr	r5, [r3, #0]
	cmp	r5, #0
	beq.n	.L_020000c4
	ldr	r1, [r5, #8]
	ldr	r2, [r5, #16]
	adds	r1, r1, r6
	adds	r2, r2, r7
	str	r1, [r5, #8]
	str	r2, [r5, #16]
	adds	r3, r5, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	bl	sub_020003da
	str	r0, [r5, #12]
	str	r0, [r5, #20]
.L_020000c4:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	movs	r0, #0
	movs	r1, #5
	bl	.L_0200013c
	pop	{r0}
	bx	r0
	.2byte 0x0000
	push	{lr}
	movs	r1, #5
	negs	r1, r1
	movs	r0, #0
	bl	.L_0200014e
	pop	{r0}
	bx	r0
	push	{lr}
	movs	r0, #0
	movs	r1, #5
	bl	.L_0200015c
	pop	{r0}
	bx	r0
	.2byte 0x0000
	push	{lr}
	movs	r1, #5
	negs	r1, r1
	movs	r0, #0
	bl	.L_0200016e
	pop	{r0}
	bx	r0
	push	{lr}
	movs	r0, #0
	movs	r1, #6
	bl	.L_0200017c
	pop	{r0}
	bx	r0
	.2byte 0x0000
	push	{lr}
	movs	r1, #6
	negs	r1, r1
	movs	r0, #0
	bl	sub_0200018e
	pop	{r0}
	bx	r0
	push	{r5, lr}
	ldr	r3, [pc, #28]
.L_0200013c:
	movs	r0, #123
	ldr	r5, [r3, #0]
	bl	sub_02000496
	movs	r3, #182
	lsls	r3, r3, #1
	adds	r5, r5, r3
	movs	r3, #0
	ldrsh	r0, [r5, r3]
.L_0200014e:
	bl	sub_0200048c
	pop	{r5}
	pop	{r0}
	bx	r0
	.2byte 0x1ebc
	.2byte 0x0300
.L_0200015c:
	push	{lr}
	movs	r0, #9
	bl	sub_020004ae
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #40]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	adds	r2, #2
.L_0200016e:
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	ldr	r2, [r3, #0]
	ldr	r3, [pc, #40]
	adds	r1, r2, r3
	ldr	r3, [pc, #40]
	strh	r3, [r1, #0]
.L_0200017c:
	ldr	r3, [pc, #40]
	adds	r1, r2, r3
	movs	r3, #31
	strh	r3, [r1, #0]
	ldr	r3, [pc, #36]
	adds	r2, r2, r3
	movs	r3, #10
	strh	r3, [r2, #0]
	b.n	.L_020001b0
	.2byte 0x0000
	.4byte 0x00003f42
	.4byte 0x00000c04
	.4byte 0x04000050
	.4byte 0x03001ecc
	.4byte 0x00000534
	.4byte 0x00003f3f
	.4byte 0x00000536
	.2byte 0x052a
	.2byte 0x0000
.L_020001b0:
	pop	{r0}
	bx	r0
	.4byte 0x47704800
	.2byte 0x84a4
	.2byte 0x0200
	push	{lr}
	bl	sub_020004e4
	bl	sub_020004f0
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x00004770
	.2byte 0x4770
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #24]
	ldr	r0, [pc, #24]
	ldr	r3, [r3, #0]
	strh	r3, [r0, #0]
	ldr	r3, [pc, #24]
	ldr	r3, [r3, #0]
	ldr	r1, [pc, #24]
	strh	r3, [r0, #2]
	bl	sub_020004fc
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x03001ae8
	.4byte 0x02008590
	.4byte 0x03001b04
	.2byte 0x85b0
	.2byte 0x0200
	push	{r5, lr}
	ldr	r5, [pc, #72]
	movs	r2, #224
	ldr	r3, [r5, #0]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	subs	r2, #192
	str	r2, [r3, #0]
	movs	r0, #9
	bl	sub_02000560
	ldr	r2, [pc, #56]
	ldr	r3, [pc, #40]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [pc, #44]
	adds	r1, r2, r3
	ldr	r3, [pc, #44]
	strh	r3, [r1, #0]
	ldr	r3, [pc, #44]
	adds	r1, r2, r3
	movs	r3, #31
	strh	r3, [r1, #0]
	ldr	r3, [pc, #40]
	adds	r2, r2, r3
	movs	r3, #10
	strh	r3, [r2, #0]
	bl	sub_0200051e
	movs	r0, #0
	b.n	.L_02000264
	.4byte 0x00003f42
	.4byte 0x00000c04
	.4byte 0x03001ebc
	.4byte 0x04000050
	.4byte 0x00000534
	.4byte 0x00003f3f
	.4byte 0x00000536
	.2byte 0x052a
	.2byte 0x0000
.L_02000264:
	pop	{r5}
	pop	{r1}
	bx	r1
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #28]
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #28]
	ldr	r3, [r3, #0]
	cmp	r2, r3
	blt.n	.L_0200027e
	ldr	r3, [pc, #24]
	b.n	.L_02000280
.L_0200027e:
	ldr	r3, [pc, #24]
.L_02000280:
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #24]
	strh	r2, [r3, #0]
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x04000006
	.4byte 0x02008610
	.4byte 0x02008614
	.4byte 0x02008616
	.4byte 0x0400001c
	.4byte 0x21824b0a
	.4byte 0x0049681a
	.4byte 0x23061852
	.4byte 0x48085ed1
	.4byte 0x1a5b23c0
	.4byte 0x4b076003
	.4byte 0x5e522102
	.4byte 0x4b06801a
	.4byte 0x4906681b
	.4byte 0x1ad2089b
	.4byte 0x4770800a
	.4byte 0x03001e70
	.4byte 0x02008610
	.4byte 0x02008614
	.4byte 0x03001e40
	.2byte 0x8616
	.2byte 0x0200
	push	{lr}
	ldr	r2, [pc, #24]
	movs	r0, #1
	movs	r1, #0
	bl	sub_020005f6
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #12]
	bl	sub_020005f8
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x0200826d
	.4byte 0x020082a1
	.4byte 0x47204c00
	.4byte 0x080000d1
	.4byte 0x47204c00
	.4byte 0x08000131
	.4byte 0x47204c00
	.4byte 0x08000309
	.4byte 0x47204c00
	.4byte 0x080091a9
	.4byte 0x47204c00
	.4byte 0x0808a019
	.4byte 0x47204c00
	.4byte 0x0808a021
	.4byte 0x47204c00
	.4byte 0x0808a081
	.4byte 0x47204c00
	.4byte 0x0808a249
	.4byte 0x47204c00
	.4byte 0x0808a259
	.4byte 0x47204c00
	.4byte 0x0808a301
	.4byte 0x47204c00
	.4byte 0x080f9011
	.4byte 0xffff0000
	.4byte 0x00000064
	.4byte 0x40000064
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000148
	.4byte 0x800001a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000001a8
	.4byte 0x000001b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000178
	.4byte 0x40000168
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x000001a8
	.4byte 0x00000148
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x00000178
	.4byte 0x40000108
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x00000148
	.4byte 0x800000f8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x00000148
	.4byte 0x80000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x000001a8
	.4byte 0x00000088
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000002e
	.4byte 0x0010202d
	.4byte 0x0020302d
	.4byte 0x0030402d
	.4byte 0x0040502d
	.4byte 0x0050702d
	.4byte 0x0060602d
	.4byte 0x0070802d
	.4byte 0x0080902d
	.4byte 0x000001ff
	.4byte 0x006d005d
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00008000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000a02
	.4byte 0xffff0001
	.4byte 0x02008139
	.4byte 0x00000a02
	.4byte 0xffff0002
	.4byte 0x02008139
	.4byte 0x00000a02
	.4byte 0xffff0003
	.4byte 0x02008139
	.4byte 0x00000a02
	.4byte 0xffff0004
	.4byte 0x02008139
	.4byte 0x00000a02
	.4byte 0xffff0005
	.4byte 0x02008139
	.4byte 0x00000a02
	.4byte 0xffff0006
	.4byte 0x02008139
	.4byte 0x00000a02
	.4byte 0xffff0007
	.4byte 0x02008139
	.4byte 0x00000a02
	.4byte 0xffff0008
	.4byte 0x02008139
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x020080d9
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x020080e9
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte 0x020080f9
	.4byte 0x00000002
	.4byte 0xffff000d
	.4byte 0x02008109
	.4byte 0x00000002
	.4byte 0xffff000e
	.4byte 0x02008119
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte 0x02008129
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008031
	.4byte 0x20009085
	.4byte 0xffff0000
	.4byte 0x0200815d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00020002
	.4byte 0x00020002
	.4byte 0x00020060
	.4byte 0x00020002
	.2byte 0xffff

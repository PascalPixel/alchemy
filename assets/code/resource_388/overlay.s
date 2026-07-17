.syntax unified
	.thumb
	.set sub_0200015c, 0x0200015c
	.set sub_0200017a, 0x0200017a
	.set sub_0200017e, 0x0200017e
	.set sub_02000184, 0x02000184
	.set sub_0200018e, 0x0200018e
	.set sub_02000190, 0x02000190
	.set sub_0200019c, 0x0200019c
	.set sub_0200019e, 0x0200019e
	.global Overlay_02000000
Overlay_02000000:
	.4byte 0x47204c00
	.4byte 0x02008055
	.4byte 0x47204c00
	.4byte 0x02008031
	.4byte 0x47204c00
	.4byte 0x0200803d
	.4byte 0x47204c00
	.4byte 0x02008045
	.4byte 0x47204c00
	.4byte 0x0200804d
	.4byte 0x47204c00
	.4byte 0x02008039
	.4byte 0x47704800
	.4byte 0x02008108
	.4byte 0x47702000
	.4byte 0x47704800
	.4byte 0x02008180
	.4byte 0x47704800
	.4byte 0x02008194
	.4byte 0x47704800
	.2byte 0x81c4
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #124]
	ldr	r1, [r3, #0]
	movs	r3, #224
	lsls	r3, r3, #1
	adds	r2, r1, r3
	adds	r3, #68
	str	r3, [r2, #0]
	subs	r3, #60
	adds	r2, r1, r3
	movs	r0, #192
	movs	r3, #24
	str	r3, [r2, #0]
	lsls	r0, r0, #2
	sub	sp, #8
	bl	sub_0200015c
	cmp	r0, #0
	beq.n	.L_020000c8
	movs	r1, #216
	movs	r2, #136
	lsls	r2, r2, #16
	movs	r0, #8
	lsls	r1, r1, #16
	bl	sub_0200017e
	movs	r1, #2
	movs	r0, #8
	bl	sub_0200018e
	movs	r0, #8
	bl	sub_02000184
	movs	r1, #0
	bl	sub_0200017a
	movs	r0, #8
	bl	sub_02000190
	movs	r3, #2
	adds	r0, #35
	strb	r3, [r0, #0]
	movs	r0, #8
	bl	sub_0200019c
	movs	r5, #0
	adds	r0, #89
	movs	r3, #11
	movs	r2, #6
	strb	r5, [r0, #0]
	movs	r1, #36
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #11
	movs	r2, #5
	movs	r3, #5
	bl	sub_0200019e
.L_020000c8:
	movs	r0, #0
	add	sp, #8
	pop	{r5}
	pop	{r1}
	bx	r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x47204c00
	.4byte 0x080091c1
	.4byte 0x47204c00
	.4byte 0x080091e1
	.4byte 0x47204c00
	.4byte 0x080770c1
	.4byte 0x47204c00
	.4byte 0x0808a081
	.4byte 0x47204c00
	.4byte 0x0808a0f1
	.4byte 0x47204c00
	.4byte 0x0808a101
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000078
	.4byte 0x40000058
	.4byte 0x00180000
	.4byte 0x01f80008
	.4byte 0x000001a8
	.4byte 0xffff0002
	.4byte 0x000001a8
	.4byte 0x40000158
	.4byte 0x00180000
	.4byte 0x01f80008
	.4byte 0x000001a8
	.4byte 0xffff0003
	.4byte 0x00000198
	.4byte 0x40000088
	.4byte 0x00180000
	.4byte 0x01f80008
	.4byte 0x000001a8
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001a
	.4byte 0x0010401b
	.4byte 0x0020501b
	.4byte 0x0030301b
	.4byte 0x000001ff
	.4byte 0xffff00d4
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0006
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0007
	.4byte 0x00000003
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000

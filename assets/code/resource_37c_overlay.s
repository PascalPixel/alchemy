.syntax unified
	.thumb
	.set sub_02000118, 0x02000118
	.set sub_0200011a, 0x0200011a
	.set sub_0200012a, 0x0200012a
	.set sub_0200013a, 0x0200013a
	.set sub_02000146, 0x02000146
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
	.4byte 0x020080c8
	.4byte 0x47702000
	.4byte 0x47704800
	.4byte 0x02008110
	.4byte 0x47704800
	.4byte 0x0200811c
	.4byte 0x47704800
	.2byte 0x8134
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #64]
	movs	r2, #224
	ldr	r3, [r3, #0]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r0, #162
	adds	r2, #68
	lsls	r0, r0, #1
	str	r2, [r3, #0]
	bl	sub_0200011a
	ldr	r0, [pc, #44]
	bl	sub_02000118
	cmp	r0, #0
	beq.n	.L_02000090
	movs	r0, #141
	bl	sub_0200013a
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	bl	sub_0200012a
	bl	sub_02000146
.L_02000090:
	movs	r0, #0
	pop	{r1}
	bx	r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000814
	.4byte 0x47204c00
	.4byte 0x080091f1
	.4byte 0x47204c00
	.4byte 0x080770c1
	.4byte 0x47204c00
	.4byte 0x080770c9
	.4byte 0x47204c00
	.4byte 0x0808a2f9
	.4byte 0x47204c00
	.4byte 0x0808a5e1
	.4byte 0xffff0000
	.4byte 0x00000057
	.4byte 0xc000008e
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000057
	.4byte 0xc000008e
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000d
	.4byte 0x0010200c
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03500064
	.4byte 0x00300000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000

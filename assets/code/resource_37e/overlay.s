.syntax unified
	.thumb
	.set sub_02000118, 0x02000118
	.set sub_0200012a, 0x0200012a
	.set sub_02000132, 0x02000132
	.set sub_0200013e, 0x0200013e
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
	.4byte 0x020080c0
	.4byte 0x47702000
	.4byte 0x47704800
	.4byte 0x02008120
	.4byte 0x47704800
	.4byte 0x02008130
	.4byte 0x47704800
	.2byte 0x8148
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #64]
	ldr	r1, [r3, #0]
	movs	r3, #224
	lsls	r3, r3, #1
	adds	r2, r1, r3
	adds	r3, #68
	str	r3, [r2, #0]
	subs	r3, #60
	adds	r2, r1, r3
	movs	r3, #16
	str	r3, [r2, #0]
	ldr	r0, [pc, #44]
	bl	sub_02000118
	cmp	r0, #0
	beq.n	.L_02000090
	movs	r0, #141
	bl	sub_02000132
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	bl	sub_0200012a
	bl	sub_0200013e
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
	.4byte 0x0808a2f9
	.4byte 0x47204c00
	.4byte 0x0808a5e1
	.4byte 0xffff0000
	.4byte 0x00000078
	.4byte 0x40000064
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000002d7
	.4byte 0xc000005c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000038
	.4byte 0x40000268
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000000f
	.4byte 0x00105010
	.4byte 0x00201010
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
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000

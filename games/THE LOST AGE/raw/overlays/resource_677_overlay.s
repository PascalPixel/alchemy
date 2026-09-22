.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.set sub_02000a00, 0x02000a00
	.set sub_02000a08, 0x02000a08
	.set sub_02000a40, 0x02000a40
	.set sub_02000a76, 0x02000a76
	.set sub_02000a80, 0x02000a80
	.set sub_02000ab6, 0x02000ab6
	.set sub_02000ac0, 0x02000ac0
	.set sub_02000ac2, 0x02000ac2
	.set sub_02000aca, 0x02000aca
	.set sub_02000ae2, 0x02000ae2
	.set sub_02000ae6, 0x02000ae6
	.set sub_02000af6, 0x02000af6
	.set sub_02000b00, 0x02000b00
	.set sub_02000b02, 0x02000b02
	.set sub_02000b0a, 0x02000b0a
	.set sub_02000b22, 0x02000b22
	.set sub_02000b26, 0x02000b26
	.set sub_02000b36, 0x02000b36
	.set sub_02000b40, 0x02000b40
	.set sub_02000b42, 0x02000b42
	.set sub_02000b4a, 0x02000b4a
	.set sub_02000b60, 0x02000b60
	.set sub_02000b62, 0x02000b62
	.set sub_02000b66, 0x02000b66
	.set sub_02000b68, 0x02000b68
	.set sub_02000b82, 0x02000b82
	.set sub_02000b88, 0x02000b88
	.set sub_02000b8a, 0x02000b8a
	.set sub_02000b90, 0x02000b90
	.set sub_02000ba2, 0x02000ba2
	.set sub_02000ba6, 0x02000ba6
	.set sub_02000bb0, 0x02000bb0
	.set sub_02000bb4, 0x02000bb4
	.set sub_02000bcc, 0x02000bcc
	.set sub_02000bdc, 0x02000bdc
	.set sub_02000be6, 0x02000be6
	.set sub_02000bf4, 0x02000bf4
	.set sub_02000bf8, 0x02000bf8
	.set sub_02000c04, 0x02000c04
	.set sub_02000c0e, 0x02000c0e
	.set sub_02000c1a, 0x02000c1a
	.set sub_02000c1c, 0x02000c1c
	.set sub_02000c28, 0x02000c28
	.set sub_02000c30, 0x02000c30
	.set sub_02000c36, 0x02000c36
	.set sub_02000c4a, 0x02000c4a
	.set sub_02000c6e, 0x02000c6e
	.set sub_02000c7c, 0x02000c7c
	.set sub_02000c7e, 0x02000c7e
	.set sub_02000c8a, 0x02000c8a
	.set sub_02000c90, 0x02000c90
	.set sub_02000c9e, 0x02000c9e
	.set sub_02000caa, 0x02000caa
	.set sub_02000cb6, 0x02000cb6
	.set sub_02000cba, 0x02000cba
	.set sub_02000cdc, 0x02000cdc
	.set sub_02000cea, 0x02000cea
	.set sub_02000cfe, 0x02000cfe
	.set sub_02000d0a, 0x02000d0a
	.set sub_02000d16, 0x02000d16
	.set sub_02000d30, 0x02000d30
	.set sub_02000d32, 0x02000d32
	.set sub_02000d3c, 0x02000d3c
	.set sub_02000d4a, 0x02000d4a
	.set sub_02000d5e, 0x02000d5e
	.set sub_02000d64, 0x02000d64
	.set sub_02000d6a, 0x02000d6a
	.set sub_02000d76, 0x02000d76
	.set sub_02000d90, 0x02000d90
	.set sub_02000d92, 0x02000d92
	.set sub_02000d9c, 0x02000d9c
	.set sub_02000db2, 0x02000db2
	.set sub_02000dbe, 0x02000dbe
	.set sub_02000dd2, 0x02000dd2
	.set sub_02000dd6, 0x02000dd6
	.set sub_02000df0, 0x02000df0
	.set sub_02000df2, 0x02000df2
	.set sub_02000e04, 0x02000e04
	.set sub_02000e0e, 0x02000e0e
	.set sub_02000e26, 0x02000e26
	.set sub_02000e2e, 0x02000e2e
	.set sub_02000e34, 0x02000e34
	.set sub_02000e3e, 0x02000e3e
	.set sub_02000e58, 0x02000e58
	.set sub_02000e5e, 0x02000e5e
	.set sub_02000e62, 0x02000e62
	.set sub_02000e6c, 0x02000e6c
	.set sub_02000e74, 0x02000e74
	.set sub_02000e7a, 0x02000e7a
	.set sub_02000e82, 0x02000e82
	.set sub_02000e8e, 0x02000e8e
	.set sub_02000e96, 0x02000e96
	.set sub_02000e9a, 0x02000e9a
	.set sub_02000e9c, 0x02000e9c
	.set sub_02000ea0, 0x02000ea0
	.set sub_02000eb2, 0x02000eb2
	.set sub_02000eb4, 0x02000eb4
	.set sub_02000ebc, 0x02000ebc
	.set sub_02000ec0, 0x02000ec0
	.set sub_02000ec2, 0x02000ec2
	.set sub_02000ec4, 0x02000ec4
	.set sub_02000ed4, 0x02000ed4
	.set sub_02000eda, 0x02000eda
	.set sub_02000ee2, 0x02000ee2
	.set sub_02000efa, 0x02000efa
	.set sub_02000efe, 0x02000efe
	.set sub_02000f00, 0x02000f00
	.set sub_02000f2c, 0x02000f2c
	.set sub_02000f36, 0x02000f36
	.set sub_02000f42, 0x02000f42
	.set sub_02000f7c, 0x02000f7c
	.set sub_02000f80, 0x02000f80
	.set sub_02000f90, 0x02000f90
	.set sub_02000fa2, 0x02000fa2
	.set sub_02000fac, 0x02000fac
	.set sub_02000fcc, 0x02000fcc
	.set sub_02000fce, 0x02000fce
	.set sub_02000fd8, 0x02000fd8
	.set sub_02000fe0, 0x02000fe0
	.set sub_02000fe6, 0x02000fe6
	.set sub_02000ff0, 0x02000ff0
	.set sub_02000ffa, 0x02000ffa
	.set sub_02000ffe, 0x02000ffe
	.set sub_02001010, 0x02001010
	.set sub_02001016, 0x02001016
	.set sub_02001018, 0x02001018
	.set sub_0200102c, 0x0200102c
	.set sub_0200102e, 0x0200102e
	.set sub_0200103c, 0x0200103c
	.set sub_02001040, 0x02001040
	.set sub_02001058, 0x02001058
	.set sub_0200105a, 0x0200105a
	.set sub_0200105e, 0x0200105e
	.set sub_0200106c, 0x0200106c
	.set sub_02001072, 0x02001072
	.set sub_02001080, 0x02001080
	.set sub_02001086, 0x02001086
	.set sub_020010a8, 0x020010a8
	.set sub_020010b0, 0x020010b0
	.set sub_020010b2, 0x020010b2
	.set sub_020010b6, 0x020010b6
	.set sub_020010bc, 0x020010bc
	.set sub_020010c4, 0x020010c4
	.set sub_020010ce, 0x020010ce
	.set sub_020010d0, 0x020010d0
	.set sub_020010da, 0x020010da
	.set sub_020010e2, 0x020010e2
	.set sub_020010ea, 0x020010ea
	.set sub_020010fc, 0x020010fc
	.set sub_0200110a, 0x0200110a
	.set sub_0200110c, 0x0200110c
	.set sub_0200110e, 0x0200110e
	.set sub_02001116, 0x02001116
	.set sub_02001124, 0x02001124
	.set sub_02001132, 0x02001132
	.set sub_0200113e, 0x0200113e
	.set sub_02001142, 0x02001142
	.set sub_0200114a, 0x0200114a
	.set sub_02001170, 0x02001170
	.set sub_02001174, 0x02001174
	.set sub_0200118c, 0x0200118c
	.set sub_02001192, 0x02001192
	.set sub_020011ac, 0x020011ac
	.set sub_020011b4, 0x020011b4
	.set sub_020011d6, 0x020011d6
	.set sub_020011ea, 0x020011ea
	.set sub_020011f0, 0x020011f0
	.set sub_020011fa, 0x020011fa
	.set sub_02001220, 0x02001220
	.set sub_02001230, 0x02001230
	.set sub_02001236, 0x02001236
	.set sub_02001238, 0x02001238
	.set sub_02001240, 0x02001240
	.set sub_02001248, 0x02001248
	.set sub_0200125e, 0x0200125e
	.set sub_02001268, 0x02001268
	.set sub_02001276, 0x02001276
	.set sub_020012a4, 0x020012a4
	.set sub_020012b0, 0x020012b0
	.set sub_020012b2, 0x020012b2
	.set sub_020012be, 0x020012be
	.set sub_020012c8, 0x020012c8
	.set sub_020012e4, 0x020012e4
	.set sub_020012f6, 0x020012f6
	.set sub_020012fc, 0x020012fc
	.set sub_02001312, 0x02001312
	.set sub_02001326, 0x02001326
	.set sub_0200133a, 0x0200133a
	.set sub_02001346, 0x02001346
	.set sub_0200134e, 0x0200134e
	.set sub_0200135e, 0x0200135e
	.set sub_02001374, 0x02001374
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x0200886d, 0x02008039, 0x02008045, 0x0200804d, 0x02008205, 0x02008041, 0x0200895d
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8c18
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8c48
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #28]
	cmp	r2, r3
	bne.n	.L_02000064
	ldr	r0, [pc, #24]
	b.n	.L_02000070
.L_02000064:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_0200006e
	ldr	r0, [pc, #24]
	b.n	.L_02000070
.L_0200006e:
	ldr	r0, [pc, #24]
.L_02000070:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000008d
	.4byte 0x02008ca4
	.4byte 0x0000008e
	.4byte 0x02008dac
	.2byte 0x8c8c
	.2byte 0x0200
	push	{lr}
	bl	sub_02000a40
	movs	r0, #0
	bl	sub_02000ae6
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl	sub_02000a00
	cmp	r0, #0
	beq.n	.L_020000ae
	ldr	r0, [pc, #28]
	bl	sub_02000ac2
	b.n	.L_020000b4
.L_020000ae:
	ldr	r0, [pc, #24]
	bl	sub_02000aca
.L_020000b4:
	movs	r0, #14
	movs	r1, #0
	bl	sub_02000ae2
	bl	sub_02000a76
	movs	r0, #0
	pop	{pc}
	.4byte 0x00002233
	.2byte 0x2228
	.2byte 0x0000
	push	{lr}
	bl	sub_02000a80
	movs	r0, #0
	bl	sub_02000b26
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl	sub_02000a40
	cmp	r0, #0
	beq.n	.L_020000ee
	ldr	r0, [pc, #28]
	bl	sub_02000b02
	b.n	.L_020000f4
.L_020000ee:
	ldr	r0, [pc, #24]
	bl	sub_02000b0a
.L_020000f4:
	movs	r0, #15
	movs	r1, #0
	bl	sub_02000b22
	bl	sub_02000ab6
	movs	r0, #0
	pop	{pc}
	.4byte 0x00002234
	.2byte 0x2229
	.2byte 0x0000
	push	{lr}
	bl	sub_02000ac0
	movs	r0, #0
	bl	sub_02000b66
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl	sub_02000a80
	cmp	r0, #0
	beq.n	.L_0200012e
	ldr	r0, [pc, #28]
	bl	sub_02000b42
	b.n	.L_02000134
.L_0200012e:
	ldr	r0, [pc, #24]
	bl	sub_02000b4a
.L_02000134:
	movs	r0, #16
	movs	r1, #0
	bl	sub_02000b62
	bl	sub_02000af6
	movs	r0, #0
	pop	{pc}
	.4byte 0x00002235
	.2byte 0x222a
	.2byte 0x0000
	push	{lr}
	bl	sub_02000b00
	movs	r0, #0
	bl	sub_02000ba6
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl	sub_02000ac0
	cmp	r0, #0
	beq.n	.L_0200016e
	ldr	r0, [pc, #28]
	bl	sub_02000b82
	b.n	.L_02000174
.L_0200016e:
	ldr	r0, [pc, #24]
	bl	sub_02000b8a
.L_02000174:
	movs	r0, #16
	movs	r1, #0
	bl	sub_02000ba2
	bl	sub_02000b36
	movs	r0, #0
	pop	{pc}
	.4byte 0x00002232
	.2byte 0x2227
	.2byte 0x0000
	push	{lr}
	bl	sub_02000b40
	movs	r0, #0
	bl	sub_02000be6
	ldr	r0, [pc, #20]
	bl	sub_02000bb4
	movs	r0, #10
	movs	r1, #0
	bl	sub_02000bcc
	bl	sub_02000b60
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x2218
	.2byte 0x0000
	push	{lr}
	bl	sub_02000b68
	movs	r0, #0
	bl	sub_02000c0e
	ldr	r0, [pc, #20]
	bl	sub_02000bdc
	movs	r0, #11
	movs	r1, #0
	bl	sub_02000bf4
	bl	sub_02000b88
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x221a
	.2byte 0x0000
	push	{lr}
	bl	sub_02000b90
	movs	r0, #0
	bl	sub_02000c36
	ldr	r0, [pc, #20]
	bl	sub_02000c04
	movs	r0, #12
	movs	r1, #0
	bl	sub_02000c1c
	bl	sub_02000bb0
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.2byte 0x221c
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #28]
	cmp	r2, r3
	bne.n	.L_0200021c
	ldr	r0, [pc, #24]
	b.n	.L_02000228
.L_0200021c:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_02000226
	ldr	r0, [pc, #24]
	b.n	.L_02000228
.L_02000226:
	ldr	r0, [pc, #24]
.L_02000228:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000008d
	.4byte 0x02008ea8
	.4byte 0x0000008e
	.4byte 0x02008fe0
	.2byte 0x8e84
	.2byte 0x0200
	push	{r5, lr}
	bl	sub_02000bf8
	movs	r0, #0
	bl	sub_02000c9e
	ldr	r5, [pc, #68]
	adds	r0, r5, #0
	bl	sub_02000c6e
	movs	r1, #0
	movs	r0, #9
	bl	sub_02000c7e
	bl	sub_02000cba
	movs	r1, #0
	bl	sub_02000c30
	cmp	r0, #0
	bne.n	.L_0200027c
	movs	r0, #10
	bl	sub_02000c1a
	adds	r0, r5, #1
	bl	sub_02000c90
	b.n	.L_02000288
.L_0200027c:
	movs	r0, #20
	bl	sub_02000c28
	adds	r0, r5, #2
	bl	sub_02000c9e
.L_02000288:
	movs	r0, #9
	movs	r1, #0
	bl	sub_02000cb6
	bl	sub_02000c4a
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x220a
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl	sub_02000c7c
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #24]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_020002d8
	movs	r0, #20
	adds	r1, r5, #0
	bl	sub_02000d32
	b.n	.L_020002f4
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_020002d8:
	bl	sub_02000c8a
	movs	r0, #0
	bl	sub_02000d30
	ldr	r0, [pc, #20]
	bl	sub_02000cfe
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_02000d16
	bl	sub_02000caa
.L_020002f4:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x2217
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl	sub_02000cdc
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #24]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_02000338
	movs	r0, #21
	adds	r1, r5, #0
	bl	sub_02000d92
	b.n	.L_02000354
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_02000338:
	bl	sub_02000cea
	movs	r0, #0
	bl	sub_02000d90
	ldr	r0, [pc, #20]
	bl	sub_02000d5e
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_02000d76
	bl	sub_02000d0a
.L_02000354:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x2219
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #52]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl	sub_02000d3c
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #24]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_02000398
	movs	r0, #22
	adds	r1, r5, #0
	bl	sub_02000df2
	b.n	.L_020003b4
	.2byte 0x0000
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_02000398:
	bl	sub_02000d4a
	movs	r0, #0
	bl	sub_02000df0
	ldr	r0, [pc, #20]
	bl	sub_02000dbe
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_02000dd6
	bl	sub_02000d6a
.L_020003b4:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x221b
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #60]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl	sub_02000d9c
	ldrh	r3, [r0, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	ldr	r2, [pc, #32]
	ands	r3, r2
	movs	r2, #192
	lsls	r3, r3, #16
	lsls	r2, r2, #24
	cmp	r3, r2
	bne.n	.L_02000400
	movs	r0, #7
	adds	r1, r5, #0
	bl	sub_02000e62
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl	sub_02000d64
	b.n	.L_0200041c
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_02000400:
	bl	sub_02000db2
	movs	r0, #0
	bl	sub_02000e58
	ldr	r0, [pc, #20]
	bl	sub_02000e26
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_02000e3e
	bl	sub_02000dd2
.L_0200041c:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x221d
	.2byte 0x0000
	push	{r5, lr}
	ldr	r3, [pc, #48]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl	sub_02000e04
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
	bne.n	.L_0200045c
	adds	r0, r5, #0
	bl	sub_02000ec0
	b.n	.L_02000478
	.4byte 0xffffc000
	.2byte 0x0240
	.2byte 0x0200
.L_0200045c:
	bl	sub_02000e0e
	movs	r0, #0
	bl	sub_02000eb4
	ldr	r0, [pc, #20]
	bl	sub_02000e82
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_02000e9a
	bl	sub_02000e2e
.L_02000478:
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x2221
	.2byte 0x0000
	push	{r5, lr}
	bl	sub_02000e34
	movs	r0, #0
	bl	sub_02000eda
	movs	r1, #2
	movs	r0, #14
	bl	sub_02000e9a
	ldr	r5, [pc, #68]
	adds	r0, r5, #0
	bl	sub_02000eb2
	movs	r1, #0
	movs	r0, #14
	bl	sub_02000ec2
	bl	sub_02000efe
	movs	r1, #0
	bl	sub_02000e74
	cmp	r0, #0
	bne.n	.L_020004c0
	movs	r0, #10
	bl	sub_02000e5e
	adds	r0, r5, #1
	bl	sub_02000ed4
	b.n	.L_020004cc
.L_020004c0:
	movs	r0, #20
	bl	sub_02000e6c
	adds	r0, r5, #2
	bl	sub_02000ee2
.L_020004cc:
	movs	r0, #14
	movs	r1, #0
	bl	sub_02000efa
	bl	sub_02000e8e
	pop	{r5, pc}
	.2byte 0x0000
	.2byte 0x2223
	.2byte 0x0000
	push	{r5, lr}
	movs	r5, #174
	adds	r5, #255
.L_020004e6:
	adds	r0, r5, #0
	bl	sub_02000e7a
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L_02000510
	movs	r3, #182
	adds	r5, #1
	adds	r3, #255
	cmp	r5, r3
	ble.n	.L_020004e6
	movs	r5, #162
	adds	r5, #255
.L_02000502:
	adds	r0, r5, #0
	bl	sub_02000e96
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_02000514
.L_02000510:
	movs	r0, #1
	b.n	.L_02000520
.L_02000514:
	movs	r3, #172
	adds	r5, #1
	adds	r3, #255
	cmp	r5, r3
	ble.n	.L_02000502
	movs	r0, #0
.L_02000520:
	pop	{r5, pc}
	.2byte 0x0000
	push	{r5, lr}
	bl	sub_02000a08
	cmp	r0, #0
	beq.n	.L_02000538
	movs	r0, #16
	adds	r0, #255
	bl	sub_02000e9c
	b.n	.L_02000556
.L_02000538:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl	sub_02000ea0
	cmp	r0, #0
	bne.n	.L_02000556
	bl	sub_02000fd8
	cmp	r0, #0
	bne.n	.L_02000556
	movs	r0, #16
	adds	r0, #255
	bl	sub_02000ec4
.L_02000556:
	movs	r0, #16
	adds	r0, #255
	bl	sub_02000ebc
	cmp	r0, #0
	bne.n	.L_02000598
	ldr	r5, [pc, #516]
	adds	r0, r5, #0
	bl	sub_02000f80
	movs	r1, #0
	movs	r0, #14
	bl	sub_02000f90
	bl	sub_02000fcc
	movs	r1, #0
	bl	sub_02000f42
	cmp	r0, #0
	bne.n	.L_0200058a
	movs	r0, #10
	bl	sub_02000f2c
	adds	r0, r5, #1
	b.n	.L_020005b4
.L_0200058a:
	movs	r0, #20
	bl	sub_02000f36
	adds	r0, r5, #2
	bl	sub_02000fac
	b.n	.L_020005b8
.L_02000598:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl	sub_02000f00
	cmp	r0, #0
	beq.n	.L_020005aa
	ldr	r0, [pc, #452]
	b.n	.L_020005b4
.L_020005aa:
	bl	sub_0200103c
	cmp	r0, #0
	beq.n	.L_020005c2
	ldr	r0, [pc, #444]
.L_020005b4:
	bl	sub_02000fce
.L_020005b8:
	movs	r0, #14
	movs	r1, #0
	bl	sub_02000fe6
	b.n	.L_02000766
.L_020005c2:
	ldr	r5, [pc, #432]
	adds	r0, r5, #0
	bl	sub_02000fe0
	movs	r1, #0
	movs	r0, #14
	bl	sub_02000ff0
	bl	sub_0200102c
	movs	r1, #0
	bl	sub_02000fa2
	cmp	r0, #0
	beq.n	.L_020005e2
	b.n	.L_02000758
.L_020005e2:
	adds	r0, r5, #1
	bl	sub_02000ffe
	movs	r0, #14
	movs	r1, #0
	bl	sub_02001016
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r0, #14
	bl	sub_02001058
	movs	r0, #14
	bl	sub_02000fce
	movs	r1, #1
	bl	sub_02000f7c
	ldr	r5, [pc, #368]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
	movs	r1, #244
	movs	r2, #204
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	ldr	r0, [r5, #0]
	bl	sub_02000ffa
	ldr	r0, [r5, #0]
	bl	sub_02001010
	ldr	r0, [r5, #0]
	movs	r1, #0
	movs	r2, #0
	bl	sub_0200105a
	movs	r1, #252
	movs	r2, #204
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	movs	r0, #14
	bl	sub_02001018
	movs	r0, #14
	bl	sub_0200102e
	ldr	r1, [r5, #0]
	movs	r0, #14
	movs	r2, #0
	bl	sub_02001058
	movs	r0, #14
	bl	sub_020010ce
	bl	sub_020010e2
	cmp	r0, #0
	beq.n	.L_02000722
	movs	r1, #204
	movs	r2, #204
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #14
	adds	r1, #102
	adds	r2, #51
	bl	sub_02001040
	movs	r1, #142
	movs	r2, #204
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	movs	r0, #14
	bl	sub_0200105e
	movs	r0, #14
	bl	sub_0200106c
	movs	r1, #142
	movs	r2, #180
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	movs	r0, #14
	bl	sub_02001072
	movs	r0, #14
	bl	sub_02001080
	movs	r1, #158
	movs	r2, #173
	movs	r0, #16
	lsls	r1, r1, #2
	lsls	r2, r2, #1
	bl	sub_02001086
	movs	r1, #128
	movs	r2, #0
	movs	r0, #16
	lsls	r1, r1, #7
	bl	sub_020010da
	movs	r0, #16
	movs	r1, #0
	bl	sub_020010b2
	movs	r1, #150
	movs	r2, #180
	lsls	r2, r2, #1
	lsls	r1, r1, #2
	movs	r0, #14
	bl	sub_020010a8
	movs	r0, #14
	bl	sub_020010b6
	movs	r0, #14
	movs	r1, #0
	bl	sub_020010ce
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #14
	bl	sub_0200110a
	movs	r0, #14
	bl	sub_020010b0
	movs	r3, #129
	adds	r0, #89
	strb	r3, [r0, #0]
	movs	r0, #14
	bl	sub_020010bc
	movs	r3, #1
	adds	r0, #35
	strb	r3, [r0, #0]
	movs	r1, #1
	movs	r0, #14
	bl	sub_02001132
	movs	r0, #14
	bl	sub_020010d0
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r5, #1
	orrs	r5, r3
	strb	r5, [r0, #0]
	movs	r1, #0
	movs	r0, #17
	movs	r2, #0
	bl	sub_0200110c
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl	sub_02001086
	b.n	.L_02000766
.L_02000722:
	movs	r1, #236
	movs	r2, #199
	adds	r1, #255
	lsls	r2, r2, #1
	movs	r0, #14
	bl	sub_0200110e
	movs	r0, #14
	bl	sub_02001124
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #14
	bl	sub_02001170
	movs	r0, #14
	bl	sub_02001116
	movs	r1, #0
	bl	sub_020010c4
	movs	r0, #14
	movs	r1, #0
	bl	sub_020011b4
	b.n	.L_02000766
.L_02000758:
	adds	r0, r5, #2
	bl	sub_02001174
	movs	r0, #14
	movs	r1, #0
	bl	sub_0200118c
.L_02000766:
	pop	{r5, pc}
	.4byte 0x00001307
	.4byte 0x0000130d
	.4byte 0x0000130e
	.4byte 0x0000130a
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	movs	r0, #16
	adds	r0, #255
	bl	sub_020010ea
	cmp	r0, #0
	bne.n	.L_02000794
	ldr	r0, [pc, #196]
	b.n	.L_02000806
.L_02000794:
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl	sub_020010fc
	cmp	r0, #0
	beq.n	.L_020007a6
	ldr	r0, [pc, #184]
	b.n	.L_02000806
.L_020007a6:
	bl	sub_02001238
	cmp	r0, #0
	beq.n	.L_02000842
	bl	sub_02001240
	adds	r6, r0, #0
	bl	sub_0200113e
	ldrh	r0, [r0, #0]
	movs	r1, #2
	mov	r8, r0
	adds	r0, r6, #0
	bl	sub_02001142
	movs	r1, #5
	mov	r0, r8
	bl	sub_0200114a
	ldr	r7, [pc, #144]
	adds	r0, r7, #0
	bl	sub_020011ea
	movs	r1, #0
	adds	r0, r5, #0
	bl	sub_020011fa
	bl	sub_02001236
	movs	r1, #0
	bl	sub_020011ac
	cmp	r0, #0
	beq.n	.L_020007ee
	adds	r0, r7, #1
	b.n	.L_02000806
.L_020007ee:
	adds	r0, r6, #0
	bl	sub_02001192
	cmp	r0, #0
	bge.n	.L_020007fc
	adds	r0, r7, #2
	b.n	.L_02000806
.L_020007fc:
	ldr	r3, [pc, #100]
	ldr	r3, [r3, #16]
	cmp	r3, r8
	bcs.n	.L_02000814
	adds	r0, r7, #3
.L_02000806:
	bl	sub_02001220
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_02001238
	b.n	.L_02000850
.L_02000814:
	adds	r0, r7, #4
	bl	sub_02001230
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_02001248
	adds	r0, r6, #0
	movs	r1, #3
	bl	sub_02001268
	movs	r1, #0
	adds	r0, r6, #0
	bl	sub_020011f0
	movs	r0, #0
	bl	sub_020012be
	mov	r3, r8
	negs	r0, r3
	bl	sub_020011d6
	b.n	.L_02000850
.L_02000842:
	ldr	r0, [pc, #36]
	bl	sub_0200125e
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_02001276
.L_02000850:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x0000130f
	.4byte 0x00001310
	.4byte 0x00001311
	.4byte 0x02000240
	.2byte 0x1316
	.2byte 0x0000
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	movs	r2, #133
	lsls	r1, r1, #1
	lsls	r2, r2, #1
	adds	r3, r3, r1
	adds	r2, #255
	str	r2, [r3, #0]
	ldr	r3, [pc, #204]
	subs	r2, #41
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #196]
	cmp	r2, r3
	bne.n	.L_020008f0
	movs	r0, #192
	lsls	r0, r0, #2
	adds	r0, #1
	bl	sub_020011fa
	cmp	r0, #0
	beq.n	.L_020008d4
	movs	r1, #150
	movs	r2, #180
	movs	r0, #14
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl	sub_020012a4
	movs	r1, #158
	movs	r2, #173
	movs	r0, #16
	lsls	r1, r1, #18
	lsls	r2, r2, #17
	bl	sub_020012b2
	movs	r1, #128
	movs	r0, #16
	lsls	r1, r1, #7
	movs	r2, #0
	bl	sub_020012f6
	movs	r0, #17
	movs	r1, #0
	movs	r2, #0
	bl	sub_020012c8
	b.n	.L_020008dc
.L_020008d4:
	movs	r0, #14
	movs	r1, #0
	bl	sub_0200133a
.L_020008dc:
	movs	r0, #17
	bl	sub_020012b0
	movs	r1, #0
	bl	sub_0200125e
	movs	r0, #11
	movs	r1, #2
	bl	sub_02001326
.L_020008f0:
	ldr	r3, [pc, #92]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #88]
	cmp	r2, r3
	bne.n	.L_0200094c
	movs	r0, #0
	bl	sub_0200134e
	movs	r1, #2
	movs	r0, #12
	bl	sub_02001346
	movs	r0, #12
	bl	sub_020012e4
	adds	r0, #89
	ldrb	r3, [r0, #0]
	movs	r5, #4
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r1, #2
	movs	r0, #10
	bl	sub_0200135e
	movs	r0, #10
	bl	sub_020012fc
	adds	r0, #89
	ldrb	r3, [r0, #0]
	movs	r1, #2
	orrs	r3, r5
	strb	r3, [r0, #0]
	movs	r0, #11
	bl	sub_02001374
	movs	r0, #11
	bl	sub_02001312
	adds	r0, #89
	ldrb	r3, [r0, #0]
	orrs	r5, r3
	strb	r5, [r0, #0]
.L_0200094c:
	movs	r0, #0
	pop	{r5, pc}
	.4byte 0x02000240
	.4byte 0x0000008d
	.2byte 0x008e
	.2byte 0x0000
	movs	r0, #0
	bx	lr
	.irp EntryTarget, 0x080003c9, 0x080003d1, 0x080003d9, 0x08020219, 0x08038121, 0x080ad011, 0x080ad039, 0x080ad1d9, 0x080ad2f1, 0x080c8011, 0x080c8019, 0x080c8021, 0x080c8061, 0x080c8071, 0x080c8089, 0x080c8099, 0x080c80c1, 0x080c80d1, 0x080c80f1, 0x080c80f9, 0x080c8119, 0x080c8141, 0x080c8159, 0x080c8181, 0x080c8189, 0x080c81a1, 0x080c81d1, 0x080c8201, 0x080c83e1, 0x080c8481, 0x080c84e1, 0x080c8779, 0x080c87b1, 0x08108009, 0x08108011, 0x08108019, 0x08108021, 0x08108099, 0x081080a1
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000083
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00004ccc
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x005c0000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00004ccc
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01b50000
	.4byte 0x00000000
	.4byte 0x007f0000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01c50000
	.4byte 0x00000000
	.4byte 0x00720000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
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
	.4byte 0x0000008d
	.4byte 0x1010108b
	.4byte 0xffffffff
	.4byte 0x1020208b
	.4byte 0xffffffff
	.4byte 0x1030708b
	.4byte 0xffffffff
	.4byte 0x1040408b
	.4byte 0xffffffff
	.4byte 0x0000008e
	.4byte 0x1010308b
	.4byte 0xffffffff
	.4byte 0x1020608b
	.4byte 0xffffffff
	.4byte 0x1030508b
	.4byte 0xffffffff
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00016000
	.4byte 0xffff0074
	.4byte 0x02008a98
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00012000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x01c60000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00012000
	.4byte 0xffff0086
	.4byte 0x00000001
	.4byte 0x01260000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00014000
	.4byte 0xffff0087
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00004000
	.4byte 0xffff0068
	.4byte 0x00000001
	.4byte 0x01eb0000
	.4byte 0x00000000
	.4byte 0x018e0000
	.4byte 0x00024000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x02300000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00018000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x015a0000
	.4byte 0x0001c000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01fa0000
	.4byte 0x00000000
	.4byte 0x018e0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x00bc0000
	.4byte 0x00010000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00014000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01950000
	.4byte 0x00014000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x01950000
	.4byte 0x00014000
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x015c0000
	.4byte 0x00000000
	.4byte 0x01950000
	.4byte 0x00014000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00014000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00014000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x015c0000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
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
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002209
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008245
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0000220f
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002210
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020083bd
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000221e
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008525
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x0200877d
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x0200814d
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x02008525
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000220d
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000220e
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002211
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002212
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000221f
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002220
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0200808d
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x020080cd
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x0200810d
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x0200808d
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x00403056
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
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002213
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002214
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0200829d
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x020082fd
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0200835d
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0200829d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x020082fd
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x0200835d
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002215
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002216
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002218
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000221a
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000221c
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0200818d
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x020081b5
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x020081dd
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000

.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.set sub_0200020c, 0x0200020c
	.set sub_02000220, 0x02000220
	.set sub_02000240, 0x02000240
	.set sub_02000284, 0x02000284
	.set sub_020002d4, 0x020002d4
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x020080fd, 0x02008039, 0x02008045, 0x0200804d, 0x020080a5, 0x02008041, 0x02008149
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8188
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x81e8
	.2byte 0x0200
	push	{lr}
	ldr	r1, [pc, #44]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #36]
	cmp	r2, r3
	bne.n	.L_02000078
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldrh	r3, [r3, #0]
	movs	r0, #128
	subs	r3, #8
	lsls	r3, r3, #16
	lsls	r0, r0, #9
	cmp	r3, r0
	bhi.n	.L_02000078
	ldr	r0, [pc, #12]
	b.n	.L_0200007a
.L_02000078:
	ldr	r0, [pc, #12]
.L_0200007a:
	pop	{pc}
	.4byte 0x02000240
	.4byte 0x000000b7
	.4byte 0x020083a8
	.2byte 0x8228
	.2byte 0x0200
	push	{lr}
	adds	r1, r0, #0
	movs	r0, #25
	bl	sub_0200020c
	pop	{pc}
	push	{lr}
	adds	r1, r0, #0
	movs	r0, #10
	bl	sub_02000220
	pop	{pc}
	push	{lr}
	ldr	r1, [pc, #64]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_020000d0
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldrh	r3, [r3, #0]
	movs	r0, #128
	subs	r3, #8
	lsls	r3, r3, #16
	lsls	r0, r0, #9
	cmp	r3, r0
	bhi.n	.L_020000d0
	ldr	r0, [pc, #32]
	b.n	.L_020000e4
.L_020000d0:
	movs	r0, #136
	lsls	r0, r0, #4
	adds	r0, #255
	bl	sub_02000240
	cmp	r0, #0
	beq.n	.L_020000e2
	ldr	r0, [pc, #20]
	b.n	.L_020000e4
.L_020000e2:
	ldr	r0, [pc, #20]
.L_020000e4:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000b7
	.4byte 0x02008780
	.4byte 0x020085ac
	.2byte 0x83d8
	.2byte 0x0200
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r0, #214
	movs	r2, #133
	lsls	r0, r0, #1
	lsls	r2, r2, #1
	adds	r3, r3, r0
	adds	r2, #255
	ldr	r1, [pc, #44]
	str	r2, [r3, #0]
	subs	r2, #41
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #36]
	cmp	r2, r3
	bne.n	.L_0200013a
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldrh	r3, [r3, #0]
	movs	r0, #128
	subs	r3, #8
	lsls	r3, r3, #16
	lsls	r0, r0, #9
	cmp	r3, r0
	bhi.n	.L_0200013a
	bl	sub_02000284
.L_0200013a:
	movs	r0, #0
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x00b7
	.2byte 0x0000
	movs	r0, #0
	bx	lr
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r0, #48
	adds	r2, #93
	str	r2, [r3, #0]
	adds	r0, #255
	bl	sub_020002d4
	pop	{pc}
	.irp EntryTarget, 0x080003c9, 0x080003d9, 0x08108009, 0x08108019
	overlay_veneer \EntryTarget
	.endr
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x000001a0
	.4byte 0xc0000298
	.4byte 0x01200000
	.4byte 0x02100218
	.4byte 0x000002b8
	.4byte 0xffff0009
	.4byte 0x00000260
	.4byte 0xc0000298
	.4byte 0x01e80000
	.4byte 0x02d80210
	.4byte 0x000002c0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000b7
	.4byte 0x101010b6
	.4byte 0xffffffff
	.4byte 0x102020b6
	.4byte 0xffffffff
	.4byte 0x103030b6
	.4byte 0xffffffff
	.4byte 0x104040b6
	.4byte 0xffffffff
	.4byte 0x105050b6
	.4byte 0xffffffff
	.4byte 0x106060b6
	.4byte 0xffffffff
	.4byte 0x0080b086
	.4byte 0x00902083
	.4byte 0x000001ff
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0xffff0088
	.4byte 0x00000002
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00015000
	.4byte 0xffff008a
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0xffff00c3
	.4byte 0x00000001
	.4byte 0x01300000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00010000
	.4byte 0xffff00c3
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x0001d000
	.4byte 0xffff00c3
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00008000
	.4byte 0xffff0070
	.4byte 0x00000002
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00018000
	.4byte 0xffff0072
	.4byte 0x00000002
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x0000c000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00010000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x02500000
	.4byte 0x00015000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00018000
	.4byte 0xffff00c7
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00015000
	.4byte 0xffff00d1
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x005b0000
	.4byte 0x00018000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff004a
	.4byte 0x00000003
	.4byte 0x02600000
	.4byte 0x00000000
	.4byte 0x02640000
	.4byte 0x00004000
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
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002406
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002407
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002408
	.4byte 0x00008400
	.4byte 0xffff000b
	.4byte 0x00002409
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000240a
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000240b
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000240c
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000240d
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0000240e
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000240f
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002410
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002411
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002412
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002413
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00002414
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002415
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002416
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002417
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00002418
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00002419
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x0000241a
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x0000241b
	.4byte 0x0000c400
	.4byte 0xffff0013
	.4byte 0x0200808d
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00002431
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00002432
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00002433
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00002434
	.4byte 0x0000c400
	.4byte 0xffff0015
	.4byte 0x02008099
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00002435
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00002436
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00002437
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00002438
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
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000025dd
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000025de
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000025df
	.4byte 0x00008400
	.4byte 0xffff000b
	.4byte 0x000025e0
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000025e1
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000025e2
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000025e3
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000025e4
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000025e5
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000025e6
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000025e7
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000025e8
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000025e9
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000025ea
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000025eb
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x000025ec
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000025ed
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000025ee
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x000025ef
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x000025f0
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x000025f1
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000025f2
	.4byte 0x0000c400
	.4byte 0xffff0013
	.4byte 0x0200808d
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x000026a8
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x000026a9
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x000026aa
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x000026ab
	.4byte 0x0000c400
	.4byte 0xffff0015
	.4byte 0x02008099
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x000026ac
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x000026ad
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x000026ae
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x000026af
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000218d
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002191
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000

.syntax unified
	.thumb
	.set sub_02000678, 0x02000678
	.set sub_02000686, 0x02000686
	.set sub_020006b4, 0x020006b4
	.set sub_020006cc, 0x020006cc
	.set sub_020006de, 0x020006de
	.set sub_02000710, 0x02000710
	.set sub_02000714, 0x02000714
	.set sub_02000770, 0x02000770
	.set sub_020007b0, 0x020007b0
	.set sub_020007ba, 0x020007ba
	.set sub_020007d2, 0x020007d2
	.set sub_020007d4, 0x020007d4
	.set sub_02000890, 0x02000890
	.set sub_020008a6, 0x020008a6
	.set sub_020008a8, 0x020008a8
	.set sub_020008b6, 0x020008b6
	.set sub_020008d4, 0x020008d4
	.set sub_020008e4, 0x020008e4
	.set sub_020009b0, 0x020009b0
	.set sub_020009c4, 0x020009c4
	.set sub_020009c6, 0x020009c6
	.set sub_020009d6, 0x020009d6
	.set sub_02000a04, 0x02000a04
	.set sub_02000a20, 0x02000a20
	.set sub_02000a24, 0x02000a24
	.set sub_02000a26, 0x02000a26
	.set sub_02000a28, 0x02000a28
	.set sub_02000a2e, 0x02000a2e
	.set sub_02000a36, 0x02000a36
	.set sub_02000a38, 0x02000a38
	.set sub_02000a64, 0x02000a64
	.set sub_02000a80, 0x02000a80
	.set sub_02000a84, 0x02000a84
	.set sub_02000a86, 0x02000a86
	.set sub_02000a88, 0x02000a88
	.set sub_02000a8e, 0x02000a8e
	.set sub_02000a96, 0x02000a96
	.set sub_02000a98, 0x02000a98
	.set sub_02000acc, 0x02000acc
	.set sub_02000ae0, 0x02000ae0
	.set sub_02000aec, 0x02000aec
	.set sub_02000aee, 0x02000aee
	.set sub_02000af0, 0x02000af0
	.set sub_02000afe, 0x02000afe
	.set sub_02000b10, 0x02000b10
	.set sub_02000b2e, 0x02000b2e
	.set sub_02000b44, 0x02000b44
	.set sub_02000b4a, 0x02000b4a
	.set sub_02000b52, 0x02000b52
	.set sub_02000b5e, 0x02000b5e
	.set sub_02000b60, 0x02000b60
	.set sub_02000b62, 0x02000b62
	.set sub_02000b64, 0x02000b64
	.set sub_02000b7e, 0x02000b7e
	.set sub_02000b82, 0x02000b82
	.set sub_02000b84, 0x02000b84
	.set sub_02000b9c, 0x02000b9c
	.set sub_02000ba2, 0x02000ba2
	.set sub_02000bac, 0x02000bac
	.set sub_02000bc2, 0x02000bc2
	.set sub_02000bc6, 0x02000bc6
	.set sub_02000bc8, 0x02000bc8
	.set sub_02000bd6, 0x02000bd6
	.set sub_02000bdc, 0x02000bdc
	.set sub_02000bfc, 0x02000bfc
	.set sub_02000c3e, 0x02000c3e
	.set sub_02000c76, 0x02000c76
	.global Overlay_02000000
Overlay_02000000:
	.4byte 0x47204c00
	.4byte 0x02008599
	.4byte 0x47204c00
	.4byte 0x02008315
	.4byte 0x47204c00
	.4byte 0x02008321
	.4byte 0x47204c00
	.4byte 0x02008329
	.4byte 0x47204c00
	.4byte 0x02008571
	.4byte 0x47204c00
	.4byte 0x0200831d
AlchemyC_02000030:
	.space 0x16
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r4, r0, #0
	adds	r5, r1, #0
	adds	r6, r2, #0
	adds	r0, r3, #0
	adds	r2, r5, #0
	adds	r1, r4, #0
	adds	r3, r6, #0
	bl	sub_02000686
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02000096
	ldr	r1, [r5, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	adds	r2, r5, #0
	strb	r3, [r1, #9]
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r2, #4
	movs	r3, #8
	strb	r3, [r2, #0]
	movs	r1, #0
	bl	sub_020006b4
	adds	r0, r5, #0
	movs	r1, #14
	bl	sub_02000714
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_020006cc
	adds	r0, r5, #0
	b.n	.L_02000098
.L_02000096:
	movs	r0, #0
.L_02000098:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r4, r0, #0
	adds	r5, r1, #0
	adds	r6, r2, #0
	adds	r0, r3, #0
	adds	r2, r5, #0
	adds	r1, r4, #0
	adds	r3, r6, #0
	bl	sub_020006de
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020000fa
	ldr	r1, [r5, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	adds	r2, r5, #0
	strb	r3, [r1, #9]
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r2, #4
	movs	r3, #8
	strb	r3, [r2, #0]
	movs	r1, #0
	bl	sub_02000710
	adds	r0, r5, #0
	movs	r1, #15
	bl	sub_02000770
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r2, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r0, r5, #0
	b.n	.L_020000fc
.L_020000fa:
	movs	r0, #0
.L_020000fc:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.2byte 0x0000
AlchemyC_02000104:
	.space 0x36
	.2byte 0x0000
AlchemyC_0200013c:
	.space 0x1d8
AlchemyC_02000314:
	.space 0x8
AlchemyC_0200031c:
	.space 0x4
AlchemyC_02000320:
	.space 0x8
AlchemyC_02000328:
	.space 0x34
AlchemyC_0200035c:
	.space 0x60
AlchemyC_020003bc:
	.space 0x60
AlchemyC_0200041c:
	.space 0x68
AlchemyC_02000484:
	.space 0x60
AlchemyC_020004e4:
	.space 0x20
AlchemyC_02000504:
	.space 0x20
AlchemyC_02000524:
	.space 0x4c
AlchemyC_02000570:
	.space 0x28
AlchemyC_02000598:
	.space 0x7c
	.4byte 0x47204c00
	.4byte 0x03000380
	.4byte 0x47204c00
	.4byte 0x08009081
	.4byte 0x47204c00
	.4byte 0x08009099
	.4byte 0x47204c00
	.4byte 0x080090c9
	.4byte 0x47204c00
	.4byte 0x080091e1
	.4byte 0x47204c00
	.4byte 0x080091e9
	.4byte 0x47204c00
	.4byte 0x080770c1
	.4byte 0x47204c00
	.4byte 0x080770d1
	.4byte 0x47204c00
	.4byte 0x0808a011
	.4byte 0x47204c00
	.4byte 0x0808a019
	.4byte 0x47204c00
	.4byte 0x0808a021
	.4byte 0x47204c00
	.4byte 0x0808a039
	.4byte 0x47204c00
	.4byte 0x0808a081
	.4byte 0x47204c00
	.4byte 0x0808a101
	.4byte 0x47204c00
	.4byte 0x0808a149
	.4byte 0x47204c00
	.4byte 0x0808a161
	.4byte 0x47204c00
	.4byte 0x0808a171
	.4byte 0x47204c00
	.4byte 0x0808a181
	.4byte 0x47204c00
	.4byte 0x0808a189
	.4byte 0x47204c00
	.4byte 0x0808a191
	.4byte 0x47204c00
	.4byte 0x080b0009
	.4byte 0x47204c00
	.4byte 0x080b0019
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x020086c4
	.4byte 0x020086fc
	.4byte 0x02008734
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000080
	.4byte 0xc00000b8
	.4byte 0x00180000
	.4byte 0x00f80008
	.4byte 0x000000d0
	.4byte 0xffff0002
	.4byte 0x000001c0
	.4byte 0xc00000c8
	.4byte 0x01200000
	.4byte 0x02500008
	.4byte 0x000000e0
	.4byte 0xffff0003
	.4byte 0x000002e0
	.4byte 0xc00000c8
	.4byte 0x02600000
	.4byte 0x03500008
	.4byte 0x000000e0
	.4byte 0xffff0004
	.4byte 0x000000a0
	.4byte 0xc00001d8
	.4byte 0x00300000
	.4byte 0x01500120
	.4byte 0x000001f0
	.4byte 0xffff0005
	.4byte 0x000001d8
	.4byte 0x00000168
	.4byte 0x01800000
	.4byte 0x02a00120
	.4byte 0x000001f0
	.4byte 0xffff0006
	.4byte 0x00000200
	.4byte 0xc00002b8
	.4byte 0x01600000
	.4byte 0x029001f0
	.4byte 0x000002d0
	.4byte 0xffff0007
	.4byte 0x00000078
	.4byte 0x00000168
	.4byte 0x00300000
	.4byte 0x01500120
	.4byte 0x000001f0
	.4byte 0xffff0008
	.4byte 0x00000078
	.4byte 0xc00002b8
	.4byte 0x00400000
	.4byte 0x01300220
	.4byte 0x000002e8
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000049
	.4byte 0x00105048
	.4byte 0x00206048
	.4byte 0x00307048
	.4byte 0x00408048
	.4byte 0x00507049
	.4byte 0x00609048
	.4byte 0x00705049
	.4byte 0x0080403d
	.4byte 0x000001ff
	.4byte 0x000000a0
	.4byte 0x00000001
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00004000
	.4byte 0x000000a2
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x0001c000
	.4byte 0x0000009c
	.4byte 0x00000001
	.4byte 0x01f00000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00004000
	.4byte 0x0000009f
	.4byte 0x00000002
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x00004000
	.4byte 0x000000a4
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x000000a6
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x0001c000
	.4byte 0x0000007c
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00014000
	.4byte 0x0000007d
	.4byte 0x00000001
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00014000
	.4byte 0x00000076
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00014000
	.4byte 0x00000077
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01c00000
	.4byte 0x00018000
	.4byte 0x00000080
	.4byte 0x00000002
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01a00000
	.4byte 0x00004000
	.4byte 0x0000007b
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00010000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x18950028
	.4byte 0x00000001
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x02900000
	.4byte 0x00014000
	.4byte 0x18950027
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02c00000
	.4byte 0x00010000
	.4byte 0x18950029
	.4byte 0x00000001
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x02c00000
	.4byte 0x00024000
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
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x08950008
	.4byte 0x0000180b
	.4byte 0x00000000
	.4byte 0x08950009
	.4byte 0x0000180c
	.4byte 0x00000000
	.4byte 0x0895000a
	.4byte 0x0000180f
	.4byte 0x00000000
	.4byte 0x0895000b
	.4byte 0x00001810
	.4byte 0x00000000
	.4byte 0x0895000c
	.4byte 0x00001813
	.4byte 0x00000000
	.4byte 0x0895000d
	.4byte 0x00001814
	.4byte 0x00000000
	.4byte 0x0895000e
	.4byte 0x0200835d
	.4byte 0x00000000
	.4byte 0x0895000f
	.4byte 0x020083bd
	.4byte 0x00000000
	.4byte 0x08950010
	.4byte 0x0200841d
	.4byte 0x00000000
	.4byte 0x08950011
	.4byte 0x02008485
	.4byte 0x00000000
	.4byte 0x08950012
	.4byte 0x0000181e
	.4byte 0x00000000
	.4byte 0x08950013
	.4byte 0x0000181f
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001a36
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001a37
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020084e5
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001a3d
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x02008505
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001a43
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0200835d
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x020083bd
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x0200841d
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x02008485
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001a4f
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001a50
	.4byte 0x00008d15
	.4byte 0x08950008
	.4byte 0x0000180d
	.4byte 0x00008d15
	.4byte 0x08950009
	.4byte 0x0000180e
	.4byte 0x00008d15
	.4byte 0x0895000a
	.4byte 0x00001811
	.4byte 0x00008d15
	.4byte 0x0895000b
	.4byte 0x00001812
	.4byte 0x00008d15
	.4byte 0x0895000c
	.4byte 0x00001815
	.4byte 0x00008d15
	.4byte 0x0895000d
	.4byte 0x00001816
	.4byte 0x00008d15
	.4byte 0x0895000e
	.4byte 0x00001818
	.4byte 0x00008d15
	.4byte 0x0895000f
	.4byte 0x0000181a
	.4byte 0x00008d15
	.4byte 0x08950010
	.4byte 0x0000181c
	.4byte 0x00008d15
	.4byte 0x08950011
	.4byte 0x00001820
	.4byte 0x00008d15
	.4byte 0x08950012
	.4byte 0x00001821
	.4byte 0x00008d15
	.4byte 0x08950013
	.4byte 0x00001822
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001a38
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001a39
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001a3e
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001a3f
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001a44
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001a45
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001a47
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001a49
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001a4d
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001a51
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001a52
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001a53
	.4byte 0x00000033
	.4byte 0x0f6f0064
	.4byte 0x001000bc
	.4byte 0x00000023
	.4byte 0x0f700065
	.4byte 0x001000e3
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x0040299b
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x0040299c
	.4byte 0x00000173
	.4byte 0xffff00ca
	.4byte 0x0040299d
	.4byte 0x00000173
	.4byte 0xffff00cb
	.4byte 0x0040299e
	.4byte 0x00000173
	.4byte 0xffff00cc
	.4byte 0x0040299f
	.4byte 0x0000c5b3
	.4byte 0xffff00cd
	.4byte 0x004029a0
	.4byte 0x000001b3
	.4byte 0xffff00ce
	.4byte 0x004029a1
	.4byte 0x000001b3
	.4byte 0xffff00cf
	.4byte 0x004029a2
	.4byte 0x000001b3
	.4byte 0xffff00d0
	.4byte 0x004029a3
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x08b30008
	.4byte 0x00001a67
	.4byte 0x00000000
	.4byte 0x08b30009
	.4byte 0x02008525
	.4byte 0x00000000
	.4byte 0x08b3000a
	.4byte 0x00001a66
	.4byte 0x00008d15
	.4byte 0x08b30008
	.4byte 0x00001a6a
	.4byte 0x00008d15
	.4byte 0x08b30009
	.4byte 0x00001a68
	.4byte 0x00008d15
	.4byte 0x08b3000a
	.4byte 0x00001a69
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001a6d
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001a6b
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001a6c
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001a70
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001a6e
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001a6f
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000

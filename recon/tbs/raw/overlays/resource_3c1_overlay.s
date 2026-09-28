.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/SUHARA_MURA/ENTRY.INC"
	.section .text.x02008068,"ax",%progbits
	.global Func_02000068
	.thumb_func
Func_02000068:
	push {r5, r6, lr}
	ldr r5, [pc, #64]
	adds r6, r0, #0
	adds r0, r5, #0
	bl 0x020082c0
	movs r1, #0
	adds r0, r6, #0
	bl 0x020082c8
	movs r0, #0
	movs r1, #0
	bl 0x020082b0
	cmp r0, #0
	bne .L_02000068_0
	movs r0, #10
	bl 0x020082a8
	adds r0, r5, #1
	bl 0x020082c0
	b .L_02000068_1
.L_02000068_0:
	adds r0, r5, #2
	bl 0x020082c0
.L_02000068_1:
	adds r0, r6, #0
	movs r1, #0
	bl 0x020082d0
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000025b8
	.global Func_020000b0
	.thumb_func
Func_020000b0:
	push {r5, r6, lr}
	ldr r5, [pc, #64]
	adds r6, r0, #0
	adds r0, r5, #0
	bl 0x020082c0
	movs r1, #0
	adds r0, r6, #0
	bl 0x020082c8
	movs r0, #0
	movs r1, #0
	bl 0x020082b0
	cmp r0, #0
	bne .L_020000b0_0
	movs r0, #10
	bl 0x020082a8
	adds r0, r5, #1
	bl 0x020082c0
	b .L_020000b0_1
.L_020000b0_0:
	adds r0, r5, #2
	bl 0x020082c0
.L_020000b0_1:
	adds r0, r6, #0
	movs r1, #0
	bl 0x020082d0
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000025dc
	.section .text.x02008288,"ax",%progbits
	.include "games/THE BROKEN SEAL/SRC/FIELD/SUHARA_MURA/IMPORT.INC"
	.section .rodata,"a",%progbits
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000088
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000100
	.4byte 0x40000078
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000058
	.4byte 0x40000128
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x000000d8
	.4byte 0x40000118
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x00000128
	.4byte 0x40000128
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x00000078
	.4byte 0x00000110
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000028
	.4byte 0x000000b0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000a7
	.4byte 0x001010a8
	.4byte 0x002020a8
	.4byte 0x003030a8
	.4byte 0x004040a8
	.4byte 0x005050a8
	.4byte 0x00a1b002
	.4byte 0x00b38002
	.4byte 0x00c39002
	.4byte 0x000001ff
	.4byte 0xffff006c
	.4byte 0x00000002
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00008000
	.4byte 0xffff006a
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00008000
	.4byte 0xffff008f
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00014000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00004000
	.4byte 0xffff0067
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x0000c000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00002000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x0001c000
	.4byte 0xffff0066
	.4byte 0x00000002
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00004000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff006c
	.4byte 0x00000002
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00008000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00014000
	.4byte 0xffff008f
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0001c000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00002000
	.4byte 0xffff0067
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x0000c000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00002000
	.4byte 0xffff006b
	.4byte 0x00000002
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff0066
	.4byte 0x00000002
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00004000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global SuharaMura_CellAnimationOrigins
SuharaMura_CellAnimationOrigins:
	.4byte 0x00000000
	.4byte 0x00070008
	.4byte 0x0005000f
	.4byte 0x00100005
	.4byte 0x000f000d
	.4byte 0x00100012
	.4byte 0x00000019
	.4byte 0x00020001
	.4byte 0x001a0008
	.4byte 0x00010000
	.4byte 0x00010002
	.4byte 0x001affff
	.4byte 0x00020002
	.4byte 0x00080002
	.4byte 0x0002001c
	.4byte 0x00020002
	.4byte 0xffff0001
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008121
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008195
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008121
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008121
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008121
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000025b7
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008069
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000025bb
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000025bc
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000025bd
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000025be
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000025bf
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000025c0
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000025c1
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000025c2
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000025c3
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000025c4
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000025c5
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000025c6
	.4byte 0x00000013
	.4byte 0x0fad0064
	.4byte 0x001000c2
	.4byte 0x00000023
	.4byte 0x0fae0065
	.4byte 0x001000e5
	.4byte 0x00000023
	.4byte 0x0faf0066
	.4byte 0x00200005
	.4byte 0x00000c15
	.4byte 0x02010010
	.4byte 0x020080f9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008121
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008195
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008121
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008121
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008121
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000025d7
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000025d8
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000025d9
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000025da
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000025db
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x020080b1
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000025df
	.4byte 0x00000000
	.4byte 0xffff000f
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
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000025e5
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000025e6
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000025e7
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000025e8
	.4byte 0x00000013
	.4byte 0x0fad0064
	.4byte 0x001000c2
	.4byte 0x00000023
	.4byte 0x0fae0065
	.4byte 0x001000e5
	.4byte 0x00000023
	.4byte 0x0faf0066
	.4byte 0x00200005
	.4byte 0x00000c15
	.4byte 0x02010010
	.4byte 0x020080f9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000

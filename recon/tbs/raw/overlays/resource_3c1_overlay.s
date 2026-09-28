.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/SUHARA_MURA/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020082f0
	.global Func_02000038
	.thumb_func
Func_02000038:
	movs r0, #0
	bx lr
	.global Func_0200003c
	.thumb_func
Func_0200003c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020083c8
	.global Func_02000044
	.thumb_func
Func_02000044:
	push {lr}
	ldr r0, [pc, #20]
	bl 0x02008298
	cmp r0, #0
	beq .L_02000044_0
	ldr r0, [pc, #12]
	b .L_02000044_1
.L_02000044_0:
	ldr r0, [pc, #12]
.L_02000044_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x0000096f
	.4byte 0x020084e0
	.4byte 0x020083f0
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
	.global Func_020000f8
	.thumb_func
Func_020000f8:
	push {lr}
	sub sp, #8
	movs r3, #4
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #9
	movs r2, #1
	movs r3, #1
	movs r0, #25
	bl 0x02008290
	ldr r0, [pc, #8]
	bl 0x020082a0
	sub sp, #-8
	pop {r0}
	bx r0
	.4byte 0x00000201
	.global Func_02000120
	.thumb_func
Func_02000120:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r1, [pc, #92]
	movs r2, #182
	ldr r3, [r1]
	lsls r2, r2, #1
	adds r3, r3, r2
	mov r8, r1
	movs r2, #0
	ldrsh r1, [r3, r2]
	ldr r2, [pc, #80]
	lsls r3, r1, #2
	mov r10, r1
	ldrsh r6, [r2, r3]
	adds r3, #2
	ldrsh r5, [r2, r3]
	lsls r6, r6, #16
	lsls r5, r5, #16
	movs r0, #158
	lsrs r6, r6, #16
	lsrs r5, r5, #16
	bl 0x020082e8
	adds r1, r6, #0
	adds r2, r5, #0
	ldr r0, [pc, #56]
	bl 0x02008288
	movs r2, #16
	movs r0, #0
	movs r1, #0
	negs r2, r2
	bl 0x020082e0
	mov r2, r8
	ldr r3, [r2]
	movs r1, #228
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #16
	str r2, [r3]
	mov r0, r10
	bl 0x020082d8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x020085d0
	.4byte 0x020085e8
	.global Func_02000194
	.thumb_func
Func_02000194:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r1, [pc, #92]
	movs r2, #182
	ldr r3, [r1]
	lsls r2, r2, #1
	adds r3, r3, r2
	mov r8, r1
	movs r2, #0
	ldrsh r1, [r3, r2]
	ldr r2, [pc, #80]
	lsls r3, r1, #2
	mov r10, r1
	ldrsh r6, [r2, r3]
	adds r3, #2
	ldrsh r5, [r2, r3]
	lsls r6, r6, #16
	lsls r5, r5, #16
	movs r0, #158
	lsrs r6, r6, #16
	lsrs r5, r5, #16
	bl 0x020082e8
	adds r1, r6, #0
	adds r2, r5, #0
	ldr r0, [pc, #56]
	bl 0x02008288
	movs r2, #16
	movs r0, #0
	movs r1, #0
	negs r2, r2
	bl 0x020082e0
	mov r2, r8
	ldr r3, [r2]
	movs r1, #228
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #16
	str r2, [r3]
	mov r0, r10
	bl 0x020082d8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x020085d0
	.4byte 0x020085fe
	.global Func_02000208
	.thumb_func
Func_02000208:
	push {lr}
	ldr r0, [pc, #20]
	bl 0x02008298
	cmp r0, #0
	beq .L_02000208_0
	ldr r0, [pc, #12]
	b .L_02000208_1
.L_02000208_0:
	ldr r0, [pc, #12]
.L_02000208_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x0000096f
	.4byte 0x02008758
	.4byte 0x02008614
	.global Func_0200022c
	.thumb_func
Func_0200022c:
	push {lr}
	ldr r3, [pc, #72]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #90
	bne .L_0200022c_0
	ldr r0, [pc, #60]
	bl 0x020082a0
.L_0200022c_0:
	ldr r3, [pc, #56]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	subs r3, #192
	str r3, [r2]
	adds r3, #200
	adds r2, r1, r3
	movs r3, #24
	str r3, [r2]
	ldr r0, [pc, #40]
	bl 0x02008298
	cmp r0, #0
	beq .L_0200022c_1
	bl 0x020080f8
	movs r0, #16
	movs r1, #4
	bl 0x020082b8
.L_0200022c_1:
	movs r0, #0
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000096f
	.4byte 0x03001ebc
	.4byte 0x00000201
	.include "games/THE BROKEN SEAL/SRC/FIELD/SUHARA_MURA/IMPORT.INC"
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

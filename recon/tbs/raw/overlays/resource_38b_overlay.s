.syntax unified
	.thumb
	.section .text.x02008088,"ax",%progbits
	.balign 4
	.global Func_02000088
	.thumb_func
Func_02000088:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02000088_0
	ldr r0, [pc, #36]
	b .L_02000088_1
.L_02000088_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000088_2
	ldr r0, [pc, #36]
	b .L_02000088_1
.L_02000088_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000088_3
	ldr r0, [pc, #32]
	b .L_02000088_1
.L_02000088_3:
	ldr r0, [pc, #32]
.L_02000088_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000001e
	.4byte 0x020091ec
	.4byte 0x00000023
	.4byte 0x0200930c
	.4byte 0x00000020
	.4byte 0x0200936c
	.4byte 0x020091d4
	.global Func_020000dc
	.thumb_func
Func_020000dc:
	push {lr}
	ldr r3, [pc, #24]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #16]
	movs r0, #0
	cmp r2, r3
	bne .L_020000dc_0
	ldr r0, [pc, #12]
.L_020000dc_0:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000020
	.4byte 0x020093fc
	.section .text.x0200811c,"ax",%progbits
	.balign 4
	.global Func_0200011c
	.thumb_func
Func_0200011c:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_0200011c_0
	ldr r0, [pc, #36]
	b .L_0200011c_1
.L_0200011c_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_0200011c_2
	ldr r0, [pc, #36]
	b .L_0200011c_1
.L_0200011c_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_0200011c_3
	ldr r0, [pc, #32]
	b .L_0200011c_1
.L_0200011c_3:
	ldr r0, [pc, #32]
.L_0200011c_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000001e
	.4byte 0x02009498
	.4byte 0x00000023
	.4byte 0x02009600
	.4byte 0x00000020
	.4byte 0x020096f0
	.4byte 0x02009480
	.section .text.x02008198,"ax",%progbits
	.balign 4
	.global Func_02000198
	.thumb_func
Func_02000198:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02000198_0
	ldr r0, [pc, #36]
	b .L_02000198_1
.L_02000198_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000198_2
	ldr r0, [pc, #36]
	b .L_02000198_1
.L_02000198_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000198_3
	ldr r0, [pc, #32]
	b .L_02000198_1
.L_02000198_3:
	ldr r0, [pc, #32]
.L_02000198_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000001e
	.4byte 0x02009744
	.4byte 0x00000023
	.4byte 0x02009a2c
	.4byte 0x00000020
	.4byte 0x02009bc4
	.4byte 0x02009738
	.section .text.x02008890,"ax",%progbits
	.balign 4
	.global Func_02000890
	.thumb_func
Func_02000890:
	push {lr}
	ldr r3, [pc, #68]
	movs r2, #224
	ldr r1, [r3]
	movs r3, #128
	lsls r2, r2, #1
	lsls r3, r3, #1
	str r3, [r1, r2]
	ldr r3, [pc, #56]
	ldrsh r2, [r3, r2]
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_02000890_0
	bl 0x020088f0
	b .L_02000890_1
.L_02000890_0:
	ldr r3, [pc, #48]
	cmp r2, r3
	bne .L_02000890_2
	bl 0x02008ae0
	movs r1, #200
	ldr r0, [pc, #40]
	lsls r1, r1, #4
	bl 0x02009020
	b .L_02000890_1
.L_02000890_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000890_1
	bl 0x02008d10
.L_02000890_1:
	movs r0, #0
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0000001e
	.4byte 0x00000023
	.4byte 0x02008ed9
	.4byte 0x00000020
	.section .text.x02008db4,"ax",%progbits
	.balign 4
	.global Func_02000db4
	.thumb_func
Func_02000db4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #0
	sub sp, #12
	bl 0x020090d0
	ldrh r3, [r0, #6]
	ldr r2, [pc, #240]
	lsrs r3, r3, #12
	lsls r5, r3, #2
	ldr r3, [r2, r5]
	mov r8, r0
	movs r1, #10
	ldrsh r0, [r0, r1]
	mov r10, r2
	asrs r2, r3, #16
	adds r0, r0, r2
	mov r2, r8
	movs r4, #18
	ldrsh r1, [r2, r4]
	lsls r3, r3, #16
	asrs r3, r3, #16
	adds r1, r1, r3
	asrs r0, r0, #4
	asrs r1, r1, #4
	bl 0x02008d80
	adds r7, r0, #0
	cmp r7, #0
	beq .L_02000db4_0
	movs r3, #0
	adds r2, r7, #0
	adds r2, #34
	mov r9, r3
	movs r3, #2
	strb r3, [r2]
	mov r4, r10
	ldr r1, [r4, r5]
	ldr r2, [pc, #184]
	ldr r3, [r7, #8]
	ands r2, r1
	mov r6, sp
	adds r3, r3, r2
	str r3, [r6]
	ldr r3, [r7, #12]
	str r3, [r6, #4]
	ldr r3, [r7, #16]
	lsls r1, r1, #16
	adds r3, r3, r1
	adds r1, r6, #0
	str r3, [r6, #8]
	bl 0x02009080
	cmp r0, #0
	bgt .L_02000db4_0
	movs r1, #8
	mov r0, r8
	bl 0x02009038
	ldr r5, [pc, #144]
	movs r0, #15
	bl 0x02009018
	movs r0, #185
	bl 0x020091b8
	str r5, [r7, #48]
	str r5, [r7, #52]
	adds r0, r7, #0
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	bl 0x02009058
	mov r1, r8
	str r5, [r1, #48]
	str r5, [r1, #52]
	mov r0, r8
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	bl 0x02009058
	adds r0, r7, #0
	bl 0x02009060
	ldr r3, [r6]
	str r3, [r7, #8]
	ldr r3, [r6, #8]
	mov r2, r9
	str r3, [r7, #16]
	str r2, [r7, #36]
	str r2, [r7, #44]
	movs r1, #1
	mov r0, r8
	bl 0x02009038
	ldr r3, [pc, #72]
	movs r4, #224
	lsls r4, r4, #1
	adds r3, r3, r4
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #64]
	cmp r2, r3
	bne .L_02000db4_1
	bl 0x02008cb4
	b .L_02000db4_0
.L_02000db4_1:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_02000db4_2
	bl 0x020089cc
	b .L_02000db4_0
.L_02000db4_2:
	ldr r3, [pc, #48]
	cmp r2, r3
	bne .L_02000db4_0
	bl 0x02008fa0
.L_02000db4_0:
	sub sp, #-12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02009d3c
	.4byte 0xffff0000
	.4byte 0x00003333
	.4byte 0x02000240
	.4byte 0x00000023
	.4byte 0x0000001e
	.4byte 0x00000020
	.section .rodata,"a",%progbits
	.global Mura_VillagerActions
Mura_VillagerActions:
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a8
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000f0
	.4byte 0xc00001d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000000e0
	.4byte 0x40000058
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
	.4byte 0x000000c8
	.4byte 0x40000148
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x00000068
	.4byte 0x40000158
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x000000a8
	.4byte 0x400000d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x00000048
	.4byte 0x400000d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x00000128
	.4byte 0x400000b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0009
	.4byte 0x00000190
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000028
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x00000108
	.4byte 0xc00000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a8
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000038
	.4byte 0x00000178
	.4byte 0x00100000
	.4byte 0x01900010
	.4byte 0x00000190
	.4byte 0xffff0002
	.4byte 0x00000178
	.4byte 0x800000b8
	.4byte 0x00100000
	.4byte 0x01900010
	.4byte 0x00000190
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000e8
	.4byte 0x40000090
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000078
	.4byte 0xc0000198
	.4byte 0x00000000
	.4byte 0x00f00120
	.4byte 0x000001c0
	.4byte 0xffff0002
	.4byte 0x00000078
	.4byte 0x40000180
	.4byte 0x00000000
	.4byte 0x00f00120
	.4byte 0x000001c0
	.4byte 0xffff0003
	.4byte 0x00040048
	.4byte 0xc0000080
	.4byte 0x00200000
	.4byte 0x01800020
	.4byte 0x00000130
	.4byte 0xffff0004
	.4byte 0x00040128
	.4byte 0xc00000e0
	.4byte 0x00200000
	.4byte 0x01800020
	.4byte 0x00000130
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x001a0042
	.4byte 0x004e0080
	.4byte 0x008c0026
	.4byte 0x0003ffff
	.4byte 0x001a0122
	.4byte 0x012e00e0
	.4byte 0x00ec0026
	.4byte 0x0004ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Placement_Messages
Placement_Messages:
	.4byte 0x0000001e
	.4byte 0x00105002
	.4byte 0x00201022
	.4byte 0x0030601f
	.4byte 0x0040101f
	.4byte 0x0050201f
	.4byte 0x0060301f
	.4byte 0x0070501f
	.4byte 0x0080401f
	.4byte 0x00923009
	.4byte 0x00a01020
	.4byte 0x00b04020
	.4byte 0x00000020
	.4byte 0x0010a01e
	.4byte 0x00203020
	.4byte 0x00302020
	.4byte 0x0040b01e
	.4byte 0x00000023
	.4byte 0x00106002
	.4byte 0x0022f002
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x008c0000
	.4byte 0x00024000
	.4byte 0xffff0069
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00024000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00006000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x0001e000
	.4byte 0xffff0065
	.4byte 0x00000002
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00010000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00018000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00004000
	.4byte 0xffff0066
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00006000
	.4byte 0xffff0087
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00018000
	.4byte 0xffff00f3
	.4byte 0x00000007
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01b00000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00f1
	.4byte 0x00000007
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x012c0000
	.4byte 0x00024000
	.4byte 0xffff0069
	.4byte 0x00000001
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x009a0000
	.4byte 0x00023000
	.4byte 0xffff0069
	.4byte 0x00000001
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x009a0000
	.4byte 0x00023000
	.4byte 0xffff0069
	.4byte 0x00000001
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x009a0000
	.4byte 0x00023000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x011a0000
	.4byte 0x00000000
	.4byte 0x00aa0000
	.4byte 0x00009000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00f60000
	.4byte 0x00000000
	.4byte 0x00aa0000
	.4byte 0x00007000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x01060000
	.4byte 0x00000000
	.4byte 0x009c0000
	.4byte 0x00009000
	.4byte 0xffff0017
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00f3
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00640000
	.4byte 0x00024000
	.4byte 0x006c005d
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
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
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008329
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008329
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008329
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008329
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008329
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x02008329
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte 0x02008329
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008241
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x020082c9
	.4byte 0x00000000
	.4byte 0x0845000a
	.4byte 0x000013b1
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000016c2
	.4byte 0x00000000
	.4byte 0x0845000b
	.4byte 0x000013b2
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000016c3
	.4byte 0x00000000
	.4byte 0x0845000c
	.4byte 0x02008289
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000016c4
	.4byte 0x00000000
	.4byte 0x0845000d
	.4byte 0x000013b6
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000016c5
	.4byte 0x00000000
	.4byte 0x0845000e
	.4byte 0x020082a9
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000016c6
	.4byte 0x00000000
	.4byte 0x0845000f
	.4byte 0x000013ba
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000016c7
	.4byte 0x00000000
	.4byte 0x08450010
	.4byte 0x000013bb
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x020082e9
	.4byte 0x00000000
	.4byte 0x08450011
	.4byte 0x000013bc
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x000016cb
	.4byte 0x00000000
	.4byte 0x08450012
	.4byte 0x000013bd
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02008309
	.4byte 0x00000000
	.4byte 0x08450013
	.4byte 0x000013be
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x000016cf
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000013b0
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x000016d3
	.4byte 0x00008d15
	.4byte 0x0845000a
	.4byte 0x000013c6
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000016d4
	.4byte 0x00008d15
	.4byte 0x0845000b
	.4byte 0x000013c7
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000016d5
	.4byte 0x00008d15
	.4byte 0x0845000c
	.4byte 0x000013c8
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000016d6
	.4byte 0x00008d15
	.4byte 0x0845000d
	.4byte 0x000013c9
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000016d7
	.4byte 0x00008d15
	.4byte 0x0845000e
	.4byte 0x000013ca
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000016d8
	.4byte 0x00008d15
	.4byte 0x0845000f
	.4byte 0x000013cb
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000016d9
	.4byte 0x00008d15
	.4byte 0x08450010
	.4byte 0x000013cc
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000016da
	.4byte 0x00008d15
	.4byte 0x08450011
	.4byte 0x000013cd
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x000016db
	.4byte 0x00008d15
	.4byte 0x08450012
	.4byte 0x000013ce
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000016dc
	.4byte 0x00008d15
	.4byte 0x08450013
	.4byte 0x000013cf
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x000016dd
	.4byte 0x00008c15
	.4byte 0xffff0014
	.4byte 0x020089cd
	.4byte 0x00000202
	.4byte 0xffff0014
	.4byte 0x02008db5
	.4byte 0x00004e15
	.4byte 0xffff0008
	.4byte 0x00000000
	.4byte 0x00000023
	.4byte 0x0f500064
	.4byte 0x001000b4
	.4byte 0x00000023
	.4byte 0x0f510065
	.4byte 0x00200005
	.4byte 0x00000003
	.4byte 0xffff005a
	.4byte 0x02008171
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008225
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008225
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x02008225
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x02008225
	.4byte 0x00008d15
	.4byte 0x18810009
	.4byte 0x00001771
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001471
	.4byte 0x00008d15
	.4byte 0x08480010
	.4byte 0x00001772
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001770
	.4byte 0x00008d15
	.4byte 0x0848000a
	.4byte 0x00001772
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001770
	.4byte 0x00008d15
	.4byte 0x0848000b
	.4byte 0x00001773
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001774
	.4byte 0x00000000
	.4byte 0x0848000c
	.4byte 0x0000177f
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0000177c
	.4byte 0x00000000
	.4byte 0x0848000d
	.4byte 0x00001780
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000177d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0000177e
	.4byte 0x00008d15
	.4byte 0x0848000c
	.4byte 0x00001784
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001781
	.4byte 0x00008d15
	.4byte 0x0848000d
	.4byte 0x00001785
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001782
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001783
	.4byte 0x00000003
	.4byte 0xffff000b
	.4byte 0x020081ed
	.4byte 0x00000003
	.4byte 0xffff001e
	.4byte 0x00400953
	.4byte 0x00000002
	.4byte 0x0849000d
	.4byte 0x02008585
	.4byte 0x00008c15
	.4byte 0x0848000b
	.4byte 0x02008405
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte 0x02008579
	.4byte 0x00008602
	.4byte 0xffff000a
	.4byte 0x02008579
	.4byte 0x00004602
	.4byte 0xffff000a
	.4byte 0x02008db5
	.4byte 0x00000202
	.4byte 0xffff000c
	.4byte 0x02008db5
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte 0x02008cb5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0xffff000a
	.4byte 0x02008209
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0200810d
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte 0x02008fa1
	.4byte 0x00004602
	.4byte 0xffff000b
	.4byte 0x02008db5
	.4byte 0x0000c602
	.4byte 0xffff000c
	.4byte 0x02008db5
	.4byte 0x00000202
	.4byte 0xffff000d
	.4byte 0x02008db5
	.4byte 0x00000602
	.4byte 0xffff000b
	.4byte 0x02008579
	.4byte 0x00008602
	.4byte 0xffff000b
	.4byte 0x02008579
	.4byte 0x00004602
	.4byte 0xffff000c
	.4byte 0x02008579
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00210025
	.4byte 0x00020004
	.4byte 0x00210005
	.4byte 0x00040021
	.4byte 0x00050002
	.4byte 0x0023ffff
	.4byte 0x0002002b
	.4byte 0x00050002
	.4byte 0x002b0021
	.4byte 0x00020002
	.4byte 0xffff0005
	.4byte 0x00340023
	.4byte 0x00020002
	.4byte 0x00210005
	.4byte 0x00020034
	.4byte 0x00050002
	.4byte 0x0023ffff
	.4byte 0x00020036
	.4byte 0x00050002
	.4byte 0x00360021
	.4byte 0x00020002
	.4byte 0xffff0005
	.4byte 0x00300023
	.4byte 0x00020002
	.4byte 0x00210005
	.4byte 0x00020030
	.4byte 0x00050002
	.4byte 0x0023ffff
	.4byte 0x0002002e
	.4byte 0x00050002
	.4byte 0x002e0021
	.4byte 0x00020002
	.4byte 0xffff0005
	.4byte 0x00320023
	.4byte 0x00020002
	.4byte 0x00210005
	.4byte 0x00020032
	.4byte 0x00050002
	.4byte 0x0000ffff
	.global Mura_DoorCellSteps
Mura_DoorCellSteps:
	.4byte 0x02009cac
	.4byte 0x02009c96
	.4byte 0x02009cc2
	.4byte 0x02009cd8
	.4byte 0x02009c6a
	.4byte 0x02009c80
	.4byte 0x02009c54
	.global Mura_DoorCellOrigins
Mura_DoorCellOrigins:
	.4byte 0x00140036
	.4byte 0x0012002b
	.4byte 0x00130025
	.4byte 0x000b0029
	.4byte 0x000b0023
	.4byte 0x00090031
	.4byte 0x00060037
	.global Mura_RepaintCells
Mura_RepaintCells:
	.4byte 0x12091208
	.4byte 0x1308120a
	.4byte 0x130a1309
	.4byte 0x1409130b
	.4byte 0x140b140a
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x00100000
	.global Mura_SpawnScript
Mura_SpawnScript:
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000001b

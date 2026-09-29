.syntax unified
	.thumb
	.section .text.x0200809c,"ax",%progbits
	.balign 4
	.global Func_0200009c
	.thumb_func
Func_0200009c:
	push {lr}
	ldr r3, [pc, #64]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_0200009c_0
	ldr r0, [pc, #52]
	bl 0x02008ab4
	cmp r0, #0
	beq .L_0200009c_1
	ldr r3, [pc, #48]
	movs r2, #1
	adds r3, #118
	strb r2, [r3]
.L_0200009c_1:
	ldr r0, [pc, #44]
	bl 0x02008ab4
	cmp r0, #0
	beq .L_0200009c_2
	ldr r3, [pc, #28]
	movs r2, #0
	adds r3, #70
	strb r2, [r3]
.L_0200009c_2:
	ldr r0, [pc, #20]
	b .L_0200009c_3
.L_0200009c_0:
	ldr r0, [pc, #24]
.L_0200009c_3:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000022
	.4byte 0x0000084f
	.4byte 0x02008c7c
	.4byte 0x00000845
	.4byte 0x02008c64
	.section .text.x02008154,"ax",%progbits
	.balign 4
	.global Func_02000154
	.thumb_func
Func_02000154:
	push {lr}
	ldr r3, [pc, #28]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_02000154_0
	ldr r0, [pc, #16]
	b .L_02000154_1
.L_02000154_0:
	ldr r0, [pc, #16]
.L_02000154_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000022
	.4byte 0x02008d30
	.4byte 0x02008d24
	.section .text.x020084bc,"ax",%progbits
	.balign 4
	.global Func_020004bc
	.thumb_func
Func_020004bc:
	push {r5, lr}
	ldr r3, [pc, #64]
	movs r5, #224
	ldr r2, [r3]
	movs r3, #128
	lsls r3, r3, #1
	lsls r5, r5, #1
	str r3, [r2, r5]
	movs r0, #8
	bl 0x02008af4
	adds r2, r0, #0
	adds r2, #35
	movs r3, #0
	strb r3, [r2]
	ldr r1, [r0, #80]
	ldrb r2, [r1, #9]
	subs r3, #13
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	ldr r3, [pc, #24]
	ldrsh r2, [r3, r5]
	ldr r3, [pc, #24]
	cmp r2, r3
	bne .L_020004bc_0
	bl 0x0200850c
.L_020004bc_0:
	movs r0, #0
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00000022
	.section .rodata,"a",%progbits
	.global Niwa_GateCells
Niwa_GateCells:
	.4byte 0x00190022
	.4byte 0x00030001
	.4byte 0x00230005
	.4byte 0x00010019
	.4byte 0x00050003
	.4byte 0x00190024
	.4byte 0x00030001
	.4byte 0xffff0005
	.global BiribinoNiwa_GuardScript
BiribinoNiwa_GuardScript:
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Placement_Scripts
Placement_Scripts:
	.4byte 0xffff0001
	.4byte 0x00000140
	.4byte 0xc0000148
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000148
	.4byte 0x40000110
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Placement_Messages
Placement_Messages:
	.4byte 0x00000022
	.4byte 0x0010201e
	.4byte 0x00201021
	.4byte 0x00a1d021
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0087
	.4byte 0x00000001
	.4byte 0x00670000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00018000
	.4byte 0xffff0087
	.4byte 0x00000002
	.4byte 0x00870000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00004000
	.4byte 0xffff0045
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x01470000
	.4byte 0x0001c000
	.4byte 0xffff0096
	.4byte 0x00000001
	.4byte 0x01400000
	.4byte 0x00000000
	.4byte 0x01070000
	.4byte 0x00015000
	.4byte 0xffff0096
	.4byte 0x00000001
	.4byte 0x014e0000
	.4byte 0x00000000
	.4byte 0x01070000
	.4byte 0x00003000
	.4byte 0x0fd20016
	.4byte 0x00000007
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00024000
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
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x0200845d
	.4byte 0x00000000
	.4byte 0x08450008
	.4byte 0x000013bf
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000016d0
	.4byte 0x00000000
	.4byte 0x08450009
	.4byte 0x02008185
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000016d1
	.4byte 0x00000000
	.4byte 0x0845000a
	.4byte 0x020081a5
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000016d2
	.4byte 0x00000000
	.4byte 0x084f000b
	.4byte 0x0000140c
	.4byte 0x00000000
	.4byte 0x084e000b
	.4byte 0x00001468
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x020081c5
	.4byte 0x00000000
	.4byte 0x084f000c
	.4byte 0x020081e5
	.4byte 0x00000000
	.4byte 0x084e000c
	.4byte 0x00001469
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001754
	.4byte 0x00008d15
	.4byte 0x08450008
	.4byte 0x000013d0
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000016de
	.4byte 0x00008d15
	.4byte 0x08450009
	.4byte 0x000013d1
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000016df
	.4byte 0x00008d15
	.4byte 0x0845000a
	.4byte 0x000013d2
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000016e0
	.4byte 0x00008d15
	.4byte 0x084a000b
	.4byte 0x00001417
	.4byte 0x00008d15
	.4byte 0x084e000b
	.4byte 0x0000146c
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001757
	.4byte 0x00008d15
	.4byte 0x084a000c
	.4byte 0x00001418
	.4byte 0x00008d15
	.4byte 0x084f000c
	.4byte 0x00001419
	.4byte 0x00008d15
	.4byte 0x084e000c
	.4byte 0x0000146d
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001758
	.4byte 0x00000003
	.4byte 0xffff005a
	.4byte 0x0200812d
	.4byte 0x00009415
	.4byte 0x0fd2000d
	.4byte 0x020080f9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000

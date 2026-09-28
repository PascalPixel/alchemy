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
	.global Func_02000184
	.thumb_func
Func_02000184:
	push {lr}
	bl 0x02008ad4
	ldr r0, [pc, #20]
	bl 0x02008b54
	movs r1, #0
	movs r0, #9
	bl 0x02008b74
	bl 0x02008adc
	pop {r0}
	bx r0
	.4byte 0x000013c0
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
	.section .text.x020085dc,"ax",%progbits
	.balign 4
	.global BiribinoNiwa_RunGardenScene
	.thumb_func
BiribinoNiwa_RunGardenScene:
	push {lr}
	bl 0x02008ad4
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r0, r0
	negs r1, r1
	negs r2, r2
	movs r3, #0
	bl 0x02008b9c
	movs r0, #160
	movs r1, #1
	movs r2, #160
	movs r3, #0
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl 0x02008b9c
	bl 0x02008a8c
	movs r0, #1
	bl 0x02008a4c
	movs r1, #160
	movs r2, #186
	lsls r2, r2, #17
	movs r0, #0
	lsls r1, r1, #17
	bl 0x02008b24
	bl 0x02008bac
	ldr r0, [pc, #660]
	ldr r1, [pc, #660]
	bl 0x02008b94
	movs r0, #160
	movs r1, #1
	movs r2, #145
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	bl 0x02008b9c
	movs r0, #0
	ldr r1, [pc, #640]
	ldr r2, [pc, #640]
	bl 0x02008afc
	movs r1, #160
	movs r2, #155
	movs r0, #0
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x02008b0c
	movs r1, #192
	movs r2, #10
	movs r0, #11
	lsls r1, r1, #6
	bl 0x02008b7c
	movs r0, #11
	movs r1, #2
	bl 0x02008b3c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #60
	movs r0, #11
	bl 0x02008b84
	ldr r0, [pc, #592]
	bl 0x02008b54
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl 0x02008b6c
	movs r1, #160
	movs r2, #10
	movs r0, #12
	lsls r1, r1, #7
	bl 0x02008b7c
	movs r0, #12
	movs r1, #2
	bl 0x02008b3c
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #60
	bl 0x02008b84
	movs r0, #12
	movs r1, #0
	movs r2, #20
	bl 0x02008b6c
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #5
	movs r2, #0
	bl 0x02008b7c
	movs r1, #224
	movs r0, #12
	lsls r1, r1, #7
	movs r2, #40
	bl 0x02008b7c
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #6
	movs r2, #0
	bl 0x02008b7c
	movs r1, #160
	movs r2, #10
	movs r0, #12
	lsls r1, r1, #7
	bl 0x02008b7c
	movs r0, #11
	movs r1, #1
	bl 0x02008b44
	movs r2, #10
	movs r0, #11
	movs r1, #0
	bl 0x02008b6c
	movs r0, #12
	movs r1, #1
	bl 0x02008b44
	movs r1, #0
	movs r0, #12
	bl 0x02008b5c
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02008b7c
	b .L_020005dc_0
.L_020005dc_1:
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #60
	movs r0, #12
	bl 0x02008b84
	ldr r0, [pc, #432]
	bl 0x02008b54
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl 0x02008b6c
	movs r0, #12
	movs r1, #2
	bl 0x02008b3c
	movs r0, #12
	movs r1, #0
	bl 0x02008b5c
.L_020005dc_0:
	movs r0, #0
	movs r1, #0
	bl 0x02008aec
	cmp r0, #0
	bne .L_020005dc_1
	movs r0, #10
	bl 0x02008acc
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #6
	movs r2, #0
	bl 0x02008b7c
	movs r1, #160
	movs r2, #20
	movs r0, #12
	lsls r1, r1, #7
	bl 0x02008b7c
	movs r0, #11
	movs r1, #3
	bl 0x02008b2c
	movs r1, #3
	movs r0, #12
	bl 0x02008b34
	movs r0, #20
	bl 0x02008acc
	movs r1, #1
	movs r0, #11
	bl 0x02008b44
	ldr r0, [pc, #332]
	bl 0x02008b54
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl 0x02008b6c
	movs r1, #128
	movs r2, #128
	movs r0, #11
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x02008afc
	movs r1, #157
	movs r2, #140
	movs r0, #11
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x02008b0c
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #20
	bl 0x02008b7c
	movs r2, #40
	movs r0, #11
	movs r1, #0
	bl 0x02008b6c
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x02008b8c
	movs r0, #60
	bl 0x02008acc
	ldr r0, [pc, #252]
	bl 0x02008ab4
	cmp r0, #0
	bne .L_020005dc_2
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #12
	bl 0x02008afc
	movs r0, #12
	bl 0x02008af4
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #173
	strb r3, [r0]
	ldr r2, [pc, #216]
	lsls r1, r1, #1
	movs r0, #12
	bl 0x02008b0c
	movs r0, #1
	bl 0x02008acc
	movs r0, #12
	bl 0x02008af4
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
.L_020005dc_2:
	movs r0, #11
	ldr r1, [pc, #184]
	ldr r2, [pc, #188]
	bl 0x02008afc
	movs r0, #0
	ldr r1, [pc, #176]
	ldr r2, [pc, #176]
	bl 0x02008afc
	movs r1, #164
	movs r2, #131
	movs r0, #11
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x02008b04
	movs r1, #164
	movs r2, #139
	lsls r2, r2, #1
	movs r0, #0
	lsls r1, r1, #1
	bl 0x02008b0c
	movs r1, #1
	movs r0, #11
	bl 0x02008b2c
	bl 0x020088e8
	movs r0, #40
	bl 0x02008acc
	movs r1, #164
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #242
	bl 0x02008b04
	movs r1, #164
	movs r0, #11
	lsls r1, r1, #1
	movs r2, #242
	bl 0x02008b0c
	movs r1, #0
	movs r2, #0
	movs r0, #11
	bl 0x02008b24
	movs r0, #0
	bl 0x02008b1c
	movs r1, #0
	movs r0, #0
	movs r2, #0
	bl 0x02008b24
	ldr r3, [pc, #80]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #65
	str r2, [r3]
	bl 0x02008bb4
	bl 0x02008bbc
	movs r0, #10
	bl 0x02008ba4
	bl 0x02008adc
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00003333
	.4byte 0x00000666
	.4byte 0x00009999
	.4byte 0x00004ccc
	.4byte 0x00001720
	.4byte 0x00001724
	.4byte 0x00001726
	.4byte 0x0000084a
	.4byte 0x00000107
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x03001ebc
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

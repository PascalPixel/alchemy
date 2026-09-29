.syntax unified
	.thumb
	.section .text.x020082f0,"ax",%progbits
	.global Func_020002f0
	.thumb_func
Func_020002f0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #68]
	movs r0, #16
	ldr r5, [r3]
	bl 0x02008d7c
	adds r7, r0, #0
	movs r3, #6
	ldrsh r2, [r7, r3]
	adds r6, r7, #0
	adds r6, #100
	mov r8, r2
	bl 0x02008d5c
	ldrh r2, [r6]
	ldr r3, [pc, #36]
	orrs r3, r2
	movs r2, #191
	lsls r2, r2, #1
	strh r3, [r6]
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020002f0_0
	movs r0, #149
	lsls r0, r0, #4
	bl 0x02008d44
	cmp r0, #0
	beq .L_020002f0_1
	ldr r0, [pc, #12]
	b .L_020002f0_2
	.2byte 0x0000
	.4byte 0x00000002
	.4byte 0x03001ebc
	.4byte 0x00002365
.L_020002f0_1:
	ldr r0, [pc, #112]
	bl 0x02008d44
	cmp r0, #0
	beq .L_020002f0_3
	ldr r0, [pc, #108]
	b .L_020002f0_2
.L_020002f0_3:
	ldr r0, [pc, #108]
	b .L_020002f0_2
.L_020002f0_0:
	movs r0, #149
	lsls r0, r0, #4
	bl 0x02008d44
	cmp r0, #0
	beq .L_020002f0_4
	ldr r0, [pc, #96]
	b .L_020002f0_2
.L_020002f0_4:
	ldr r0, [pc, #80]
	bl 0x02008d44
	cmp r0, #0
	beq .L_020002f0_5
	ldr r0, [pc, #84]
	b .L_020002f0_2
.L_020002f0_5:
	ldr r0, [pc, #84]
.L_020002f0_2:
	bl 0x02008df4
	movs r0, #16
	movs r1, #0
	bl 0x02008dbc
	movs r0, #16
	movs r1, #0
	movs r2, #2
	bl 0x02008de4
	movs r1, #0
	movs r2, #10
	movs r0, #16
	bl 0x02008e0c
	mov r3, r8
	strh r3, [r7, #6]
	movs r0, #1
	bl 0x02008cfc
	ldrh r2, [r6]
	movs r3, #1
	ands r3, r2
	strh r3, [r6]
	bl 0x02008d64
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000962
	.4byte 0x000021e2
	.4byte 0x00001f95
	.4byte 0x00002371
	.4byte 0x000021f5
	.4byte 0x00001faa
	.global Func_020003d0
	.thumb_func
Func_020003d0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #68]
	movs r0, #17
	ldr r5, [r3]
	bl 0x02008d7c
	adds r7, r0, #0
	movs r3, #6
	ldrsh r2, [r7, r3]
	adds r6, r7, #0
	adds r6, #100
	mov r8, r2
	bl 0x02008d5c
	ldrh r2, [r6]
	ldr r3, [pc, #36]
	orrs r3, r2
	movs r2, #191
	lsls r2, r2, #1
	strh r3, [r6]
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020003d0_0
	movs r0, #149
	lsls r0, r0, #4
	bl 0x02008d44
	cmp r0, #0
	beq .L_020003d0_1
	ldr r0, [pc, #12]
	b .L_020003d0_2
	.2byte 0x0000
	.4byte 0x00000002
	.4byte 0x03001ebc
	.4byte 0x00002366
.L_020003d0_1:
	ldr r0, [pc, #112]
	bl 0x02008d44
	cmp r0, #0
	beq .L_020003d0_3
	ldr r0, [pc, #108]
	b .L_020003d0_2
.L_020003d0_3:
	ldr r0, [pc, #108]
	b .L_020003d0_2
.L_020003d0_0:
	movs r0, #149
	lsls r0, r0, #4
	bl 0x02008d44
	cmp r0, #0
	beq .L_020003d0_4
	ldr r0, [pc, #96]
	b .L_020003d0_2
.L_020003d0_4:
	ldr r0, [pc, #80]
	bl 0x02008d44
	cmp r0, #0
	beq .L_020003d0_5
	ldr r0, [pc, #84]
	b .L_020003d0_2
.L_020003d0_5:
	ldr r0, [pc, #84]
.L_020003d0_2:
	bl 0x02008df4
	movs r0, #17
	movs r1, #0
	bl 0x02008dbc
	movs r0, #17
	movs r1, #0
	movs r2, #2
	bl 0x02008de4
	movs r1, #0
	movs r2, #10
	movs r0, #17
	bl 0x02008e0c
	mov r3, r8
	strh r3, [r7, #6]
	movs r0, #1
	bl 0x02008cfc
	ldrh r2, [r6]
	movs r3, #1
	ands r3, r2
	strh r3, [r6]
	bl 0x02008d64
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000962
	.4byte 0x000021e3
	.4byte 0x00001f96
	.4byte 0x00002372
	.4byte 0x000021f6
	.4byte 0x00001fab
	.section .text.x02008528,"ax",%progbits
	.global Func_02000528
	.thumb_func
Func_02000528:
	push {lr}
	bl 0x02008d5c
	ldr r0, [pc, #52]
	bl 0x02008df4
	movs r1, #192
	movs r2, #0
	movs r0, #25
	lsls r1, r1, #8
	bl 0x02008e1c
	movs r0, #25
	movs r1, #0
	bl 0x02008e04
	movs r1, #128
	movs r2, #0
	movs r0, #25
	lsls r1, r1, #8
	bl 0x02008e1c
	movs r0, #25
	movs r1, #0
	bl 0x02008e04
	bl 0x02008d64
	pop {r0}
	bx r0
	.4byte 0x00001fa0
	.section .text.x02008980,"ax",%progbits
	.global Func_02000980
	.thumb_func
Func_02000980:
	push {r5, lr}
	bl 0x02008d5c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	bl 0x02008e2c
	movs r1, #128
	movs r2, #128
	movs r0, #29
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x02008d84
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #30
	bl 0x02008d84
	ldr r5, [pc, #728]
	adds r0, r5, #0
	bl 0x02008df4
	movs r1, #144
	movs r2, #208
	movs r0, #29
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl 0x02008db4
	movs r1, #224
	movs r2, #208
	lsls r2, r2, #16
	movs r0, #30
	lsls r1, r1, #14
	bl 0x02008db4
	movs r1, #15
	movs r0, #32
	bl 0x02008dec
	movs r0, #32
	bl 0x02008d7c
	movs r1, #0
.L_020009e8:
	bl 0x02008d24
	movs r1, #190
	movs r2, #160
	movs r0, #32
	lsls r1, r1, #15
	lsls r2, r2, #14
	bl 0x02008db4
	movs r0, #29
	movs r1, #72
	movs r2, #248
	bl 0x02008d94
	movs r0, #30
	movs r1, #56
	movs r2, #248
	bl 0x02008d94
	movs r2, #132
	movs r0, #0
	movs r1, #64
	lsls r2, r2, #1
	bl 0x02008d9c
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #0
	bl 0x02008e1c
	movs r0, #29
	bl 0x02008dac
	movs r0, #29
	movs r1, #1
	bl 0x02008dbc
	movs r0, #30
	movs r1, #1
	bl 0x02008dbc
	movs r0, #0
	movs r1, #1
	bl 0x02008dbc
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl 0x02008ddc
	movs r2, #0
	movs r1, #0
	movs r0, #30
	bl 0x02008ddc
	movs r0, #20
	bl 0x02008d54
	movs r1, #129
	movs r0, #29
	lsls r1, r1, #1
	bl 0x02008e24
	movs r1, #129
	movs r0, #30
	lsls r1, r1, #1
	bl 0x02008e24
	movs r0, #29
	movs r1, #2
	bl 0x02008dcc
	movs r1, #2
	movs r0, #30
	bl 0x02008dd4
	movs r0, #20
	bl 0x02008d54
	movs r1, #0
	movs r0, #29
	bl 0x02008dfc
	movs r0, #25
	bl 0x02008d54
	adds r5, #3
	movs r1, #0
	movs r2, #12
	movs r3, #7
	movs r0, #52
	bl 0x02008d3c
	adds r0, r5, #0
	movs r1, #11
	movs r2, #12
	movs r3, #2
	bl 0x02008d34
	ldr r5, [pc, #480]
	movs r2, #250
	ldr r3, [r5]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #32
	str r2, [r3]
	movs r0, #0
	movs r1, #0
	bl 0x02008d74
	cmp r0, #0
	bne .L_020009e8_0
	movs r0, #20
	bl 0x02008d54
	movs r1, #2
	movs r0, #30
	bl 0x02008dd4
	movs r0, #30
	bl 0x02008d54
	movs r2, #0
	movs r1, #0
	movs r0, #30
	bl 0x02008e1c
	movs r0, #30
	bl 0x02008d54
	movs r0, #10
	bl 0x02008d54
	movs r1, #3
	movs r0, #29
	bl 0x02008dc4
	movs r0, #20
	bl 0x02008d54
	movs r2, #0
	movs r1, #0
	movs r0, #29
	bl 0x02008e1c
	movs r0, #30
	bl 0x02008d54
	movs r1, #0
	movs r0, #29
	bl 0x02008e04
	movs r0, #20
	bl 0x02008d54
	movs r1, #128
	movs r0, #29
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02008e1c
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #30
	bl 0x02008e1c
	movs r0, #30
	bl 0x02008d54
	movs r0, #29
	movs r1, #3
	bl 0x02008dbc
	movs r1, #3
	movs r0, #30
	bl 0x02008dc4
	movs r0, #20
	bl 0x02008d54
	movs r0, #29
	ldr r1, [pc, #320]
	ldr r2, [pc, #320]
	bl 0x02008d84
	movs r0, #30
	ldr r1, [pc, #308]
	ldr r2, [pc, #312]
	bl 0x02008d84
	movs r1, #232
	movs r2, #248
	movs r0, #29
	bl 0x02008d94
	movs r0, #2
	bl 0x02008d54
	movs r1, #232
	movs r2, #248
	movs r0, #30
	bl 0x02008d94
	movs r0, #29
	bl 0x02008dac
	movs r0, #29
	movs r1, #248
	movs r2, #248
	bl 0x02008d94
	movs r0, #30
	movs r1, #248
	movs r2, #248
	bl 0x02008d9c
	b .L_020009e8_1
.L_020009e8_0:
	movs r0, #20
	bl 0x02008d54
	movs r1, #2
	movs r0, #30
	bl 0x02008dd4
	movs r0, #30
	bl 0x02008d54
	movs r2, #0
	movs r1, #0
	movs r0, #30
	bl 0x02008e1c
	movs r0, #30
	bl 0x02008d54
	movs r0, #10
	bl 0x02008d54
	movs r1, #4
	movs r0, #29
	bl 0x02008dc4
	movs r0, #20
	bl 0x02008d54
	movs r1, #0
	movs r2, #0
	movs r0, #29
	bl 0x02008e1c
	movs r0, #30
	bl 0x02008d54
	ldr r2, [r5]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r0, #29
	bl 0x02008e04
	movs r0, #20
	bl 0x02008d54
	movs r1, #128
	movs r0, #29
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02008e1c
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #30
	bl 0x02008e1c
	movs r0, #30
	bl 0x02008d54
	movs r0, #29
	movs r1, #3
	bl 0x02008dbc
	movs r1, #3
	movs r0, #30
	bl 0x02008dc4
	movs r0, #20
	bl 0x02008d54
	movs r0, #29
	ldr r1, [pc, #100]
	ldr r2, [pc, #104]
	bl 0x02008d84
	movs r0, #30
	ldr r1, [pc, #92]
	ldr r2, [pc, #92]
	bl 0x02008d84
	movs r0, #29
	movs r1, #72
	movs r2, #184
	bl 0x02008d94
	movs r0, #30
	movs r1, #56
	movs r2, #184
	bl 0x02008d9c
.L_020009e8_1:
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl 0x02008db4
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl 0x02008db4
	movs r1, #0
	movs r2, #0
	movs r0, #32
	bl 0x02008db4
	movs r0, #140
	lsls r0, r0, #4
	bl 0x02008d4c
	bl 0x02008d64
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x1fb6
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x0001cccc
	.4byte 0x0000e666
	.4byte 0x00019999
	.4byte 0x0000cccc
	.section .rodata.part1,"a",%progbits
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x0000001c
	.4byte 0x00000007
	.4byte 0x00000010
	.global gTorebiMachiActor16Action
gTorebiMachiActor16Action:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x80010000
	.4byte 0x00000003
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.global gTorebiMachiActor17Action
gTorebiMachiActor17Action:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x80010000
	.4byte 0x00000003
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.global gTorebiMachiEntrances
gTorebiMachiEntrances:
	.4byte 0xffff0000
	.4byte 0x00000128
	.4byte 0x400000e8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000130
	.4byte 0xc00001c0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000000a8
	.4byte 0x400000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000198
	.4byte 0x400000a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x00000218
	.4byte 0x400000e8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x00000218
	.4byte 0x400000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x000001c8
	.4byte 0x40000178
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x00000170
	.4byte 0x400001a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x000000c8
	.4byte 0x400001c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0009
	.4byte 0x000000c8
	.4byte 0x400001a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000058
	.4byte 0x40000188
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x00000040
	.4byte 0x40000070
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000c
	.4byte 0x00000130
	.4byte 0x40000040
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000d
	.4byte 0x000001a8
	.4byte 0x400000e0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0014
	.4byte 0x00000130
	.4byte 0x40000130
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff001e
	.4byte 0x00000238
	.4byte 0xc0000128
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTorebiMachiExits
gTorebiMachiExits:
	.4byte 0x00000087
	.4byte 0x00104088
	.4byte 0x00202088
	.4byte 0x00309088
	.4byte 0x0040b089
	.4byte 0x00501088
	.4byte 0x00603088
	.4byte 0x00706088
	.4byte 0x00805088
	.4byte 0x00908088
	.4byte 0x00a0108b
	.4byte 0x00b0108e
	.4byte 0x00c17002
	.4byte 0x00d0a089
	.4byte 0x01e010bd
	.4byte 0x000001ff
	.global gTorebiMachiPlacements
gTorebiMachiPlacements:
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00015000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00015000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x0001d000
	.4byte 0xffff006a
	.4byte 0x00000002
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00003000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00015000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00015000
	.4byte 0xffff0067
	.4byte 0x00000002
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00003000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x0001d000
	.4byte 0xffff006b
	.4byte 0x02008ec0
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00020000
	.4byte 0xffff0066
	.4byte 0x02008f90
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00028000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00013000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00015000
	.4byte 0xffff0082
	.4byte 0x00000002
	.4byte 0x01900000
	.4byte 0x00000000
	.4byte 0x01b00000
	.4byte 0x0000b000
	.4byte 0xffff0082
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00015000
	.4byte 0xffff009d
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x0000d000
	.4byte 0xffff009d
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x0001b000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00015000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x0002d000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00025000
	.4byte 0xffff0074
	.4byte 0x02008ea8
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0002b000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x012e0000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x0001b000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0xfff70000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00015000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0xfff70000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00015000
	.4byte 0x005c005c
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00004000
	.4byte 0xffff0034
	.4byte 0x00000001
	.4byte 0xfff70000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00015000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTorebiMachiEvents
gTorebiMachiEvents:
	.4byte 0x00000c15
	.4byte 0x02000008
	.4byte 0x02008225
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008645
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008645
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008645
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008645
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008645
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008645
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008645
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x02008645
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte 0x02008645
	.4byte 0x0000c402
	.4byte 0xffff000a
	.4byte 0x02008965
	.4byte 0x0000c402
	.4byte 0xffff000b
	.4byte 0x02008965
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00004602
	.4byte 0xffff000c
	.4byte 0x02008965
	.4byte 0x0000c402
	.4byte 0xffff000d
	.4byte 0x02008965
	.4byte 0x00000002
	.4byte 0x08c00014
	.4byte 0x02008981
	.4byte 0x0000c402
	.4byte 0x18c100ef
	.4byte 0x02008955
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001f8d
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001f8e
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001f8f
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001f90
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001f91
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x020084e9
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x020082f1
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x020083d1
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001f97
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001f98
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001f99
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00001f9a
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00001f9b
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x02008509
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x02008529
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x02008569
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x0200859d
	.4byte 0x00000000
	.4byte 0x08c1001c
	.4byte 0x02008895
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x00000e40
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001fa4
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001fa5
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001fa6
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001fa7
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001fa8
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001fa9
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x020082f1
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x020083d1
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001fac
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001fad
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001fae
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001faf
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001fb0
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00001fb2
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x00001fb3
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x00001fb4
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x00001fb5
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x00000e41
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte 0x02008031
	.4byte 0x00000003
	.4byte 0xffff001e
	.4byte 0x020082b9
	.4byte 0x00000003
	.4byte 0xffff001f
	.4byte 0x020082d5
	.4byte 0x00000033
	.4byte 0x0f970064
	.4byte 0x00200009
	.4byte 0x0000c423
	.4byte 0x0f980065
	.4byte 0x001000e5
	.4byte 0x00000013
	.4byte 0x0f990066
	.4byte 0x001000bf
	.4byte 0x00001815
	.4byte 0x02010009
	.4byte 0x02008ca9
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008261
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x0200828d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTorebiMachiEvents2
gTorebiMachiEvents2:
	.4byte 0x00000c15
	.4byte 0x02000008
	.4byte 0x02008225
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008645
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008645
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008645
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008645
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008645
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008645
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008645
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x02008645
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte 0x02008645
	.4byte 0x0000c402
	.4byte 0xffff000a
	.4byte 0x02008965
	.4byte 0x0000c402
	.4byte 0xffff000b
	.4byte 0x02008965
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00004602
	.4byte 0xffff000c
	.4byte 0x02008965
	.4byte 0x0000c402
	.4byte 0xffff000d
	.4byte 0x02008965
	.4byte 0x0000c402
	.4byte 0x18c100ef
	.4byte 0x02008955
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000021dc
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000021dd
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000021de
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000021df
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000021e0
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000021e1
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x020082f1
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x020083d1
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x000021e4
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x000021e5
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x000021e6
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x000021e7
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x000021e8
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x02008509
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x02008529
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x02008569
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x000021ee
	.4byte 0x00000000
	.4byte 0x08c1001c
	.4byte 0x02008895
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x00000e40
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000021ef
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000021f0
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000021f1
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000021f2
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000021f3
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000021f4
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x020082f1
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x020083d1
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000021f7
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x000021f8
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x000021f9
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x000021fa
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x000021fb
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x000021fd
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x000021fe
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x000021ff
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x00002200
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x00000e41
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte 0x02008031
	.4byte 0x00000003
	.4byte 0xffff001e
	.4byte 0x020082b9
	.4byte 0x00000003
	.4byte 0xffff001f
	.4byte 0x020082d5
	.4byte 0x00000033
	.4byte 0x0f970064
	.4byte 0x00200009
	.4byte 0x00000023
	.4byte 0x0f980065
	.4byte 0x001000e5
	.4byte 0x00000013
	.4byte 0x0f990066
	.4byte 0x001000bf
	.4byte 0x00001815
	.4byte 0x02010009
	.4byte 0x02008ca9
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008261
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x0200828d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTorebiMachiEvents3
gTorebiMachiEvents3:
	.4byte 0x00000c15
	.4byte 0x02000008
	.4byte 0x02008225
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008645
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008645
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008645
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008645
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008645
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008645
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008645
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x02008645
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte 0x02008645
	.4byte 0x0000c402
	.4byte 0xffff000a
	.4byte 0x02008965
	.4byte 0x0000c402
	.4byte 0xffff000b
	.4byte 0x02008965
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00004602
	.4byte 0xffff000c
	.4byte 0x02008965
	.4byte 0x0000c402
	.4byte 0xffff000d
	.4byte 0x02008965
	.4byte 0x0000c402
	.4byte 0x18c100ef
	.4byte 0x02008955
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0000235d
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0000235e
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020085bd
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002362
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002363
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00002364
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x020082f1
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x020083d1
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00002367
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x020085dd
	.4byte 0x00000000
	.4byte 0x08c1001c
	.4byte 0x02008895
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x00000e40
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000236b
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000236c
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000236d
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000236e
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000236f
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002370
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x020082f1
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x020083d1
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00002373
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00002374
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x00000e41
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte 0x02008031
	.4byte 0x00000003
	.4byte 0xffff001e
	.4byte 0x020082b9
	.4byte 0x00000003
	.4byte 0xffff001f
	.4byte 0x020082d5
	.4byte 0x00000033
	.4byte 0x0f970064
	.4byte 0x00200009
	.4byte 0x00000023
	.4byte 0x0f980065
	.4byte 0x001000e5
	.4byte 0x00000013
	.4byte 0x0f990066
	.4byte 0x001000bf
	.4byte 0x00001815
	.4byte 0x02010009
	.4byte 0x02008ca9
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008261
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x0200828d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00200027
	.4byte 0x00020001
	.4byte 0x00280006
	.4byte 0x00010020
	.4byte 0x00060002
	.4byte 0x0000ffff
	.global gTorebiMachiCellSteps
gTorebiMachiCellSteps:
	.4byte 0x02009ce8
	.4byte 0x00060030
	.4byte 0x02009ce8
	.4byte 0x0004003f
	.4byte 0x02009ce8
	.4byte 0x00080047
	.4byte 0x02009ce8
	.4byte 0x00040047
	.4byte 0x02009ce8
	.4byte 0x00150042
	.4byte 0x02009ce8
	.4byte 0x0018003d
	.4byte 0x02009ce8
	.4byte 0x001a0032
	.4byte 0x02009ce8
	.4byte 0x00160032
	.4byte 0x02009ce8
	.4byte 0x0014002b

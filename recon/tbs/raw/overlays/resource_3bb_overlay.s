.syntax unified
	.thumb
	.section .text.x02008a1c,"ax",%progbits
	.balign 4
	.global Func_02000a1c
	.thumb_func
Func_02000a1c:
	push {r5, r6, lr}
	bl 0x0200b228
	bl 0x0200bf38
	movs r1, #89
	movs r0, #77
	bl 0x0200b344
	adds r6, r0, #0
	bl 0x0200b238
	movs r5, #9
.L_02000a1c_0:
	movs r0, #8
	subs r5, #1
	bl 0x0200bf68
	cmp r5, #0
	bge .L_02000a1c_0
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bf60
	movs r2, #128
	movs r0, #8
	movs r1, #88
	lsls r2, r2, #1
	bl 0x0200bf78
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bf60
	movs r2, #128
	lsls r2, r2, #1
	movs r0, #0
	movs r1, #120
	bl 0x0200bf80
	movs r0, #8
	movs r1, #1
	bl 0x0200bf98
	movs r2, #0
	movs r1, #8
	movs r0, #0
	bl 0x0200bfc0
	movs r0, #10
	bl 0x0200bf30
	movs r0, #8
	movs r1, #3
	bl 0x0200bf98
	movs r1, #3
	movs r0, #0
	bl 0x0200bfa0
	movs r0, #20
	bl 0x0200bf30
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200bf60
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200bf60
	movs r2, #128
	movs r0, #0
	movs r1, #112
	lsls r2, r2, #1
	bl 0x0200bf78
	movs r2, #128
	lsls r2, r2, #1
	movs r0, #8
	movs r1, #96
	bl 0x0200bf80
	movs r0, #0
	movs r1, #16
	bl 0x0200bf98
	movs r1, #9
	movs r0, #8
	bl 0x0200bf98
	movs r0, #10
	bl 0x0200bf30
	movs r1, #2
	subs r1, r1, r6
	adds r1, #1
	movs r0, #72
	bl 0x0200c038
	ldr r3, [pc, #40]
	ldr r2, [pc, #40]
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	ldr r5, [pc, #36]
	movs r1, #4
	adds r0, r5, #0
	bl 0x0200c040
	adds r0, r5, #0
	movs r1, #5
	bl 0x0200c048
	movs r0, #141
	lsls r0, r0, #1
	bl 0x0200bef0
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x0000022b
	.4byte 0x00000090
	.section .text.x02008bd4,"ax",%progbits
	.balign 4
	.global Func_02000bd4
	.thumb_func
Func_02000bd4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, [pc, #956]
	movs r1, #224
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	movs r0, #162
	str r2, [r3]
	lsls r0, r0, #1
	sub sp, #12
	bl 0x0200bef0
	movs r3, #100
	movs r2, #11
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #14
	movs r1, #11
	movs r2, #12
	movs r3, #4
	bl 0x0200be88
	movs r3, #120
	movs r2, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #48
	movs r2, #5
	movs r1, #10
	movs r3, #6
	bl 0x0200be88
	movs r2, #2
	movs r6, #26
	movs r7, #0
	mov r8, r2
.L_02000bd4_0:
	adds r0, r6, #0
	bl 0x0200bf50
	movs r1, #4
	adds r5, r0, #0
	bl 0x0200be48
	adds r3, r5, #0
	adds r3, #85
	strb r7, [r3]
	mov r1, r8
	subs r3, #50
	adds r6, #1
	str r7, [r5, #12]
	strb r1, [r3]
	cmp r6, #30
	ble .L_02000bd4_0
	movs r0, #18
	bl 0x0200bf50
	movs r3, #2
	adds r0, #35
	strb r3, [r0]
	movs r0, #204
	lsls r0, r0, #2
	bl 0x0200bee8
	cmp r0, #0
	beq .L_02000bd4_1
	movs r0, #30
	bl 0x0200bf50
	movs r3, #168
	adds r5, r0, #0
	lsls r3, r3, #17
	str r3, [r5, #8]
	ldr r3, [pc, #816]
	str r3, [r5, #12]
	movs r3, #132
	lsls r3, r3, #17
	str r3, [r5, #16]
	movs r2, #16
	movs r3, #20
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #19
	movs r1, #16
	movs r2, #1
	movs r3, #1
	bl 0x0200be88
	movs r3, #21
	movs r2, #80
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #20
	movs r1, #80
	movs r2, #1
	movs r3, #1
	bl 0x0200be88
	b .L_02000bd4_2
.L_02000bd4_1:
	movs r0, #30
	bl 0x0200bf50
	movs r1, #3
	adds r5, r0, #0
	bl 0x0200be48
	movs r3, #128
	lsls r3, r3, #13
	str r3, [r5, #12]
.L_02000bd4_2:
	movs r0, #11
	bl 0x0200bf50
	movs r3, #2
	mov r8, r3
	adds r0, #35
	mov r1, r8
	movs r2, #0
	strb r1, [r0]
	ldr r0, [pc, #732]
	mov r10, r2
	bl 0x0200bee8
	cmp r0, #0
	beq .L_02000bd4_3
	movs r3, #35
	movs r2, #77
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #35
	movs r1, #78
	movs r2, #1
	movs r3, #1
	bl 0x0200be88
.L_02000bd4_3:
	ldr r0, [pc, #704]
	bl 0x0200bee8
	cmp r0, #0
	beq .L_02000bd4_4
	movs r0, #19
	movs r1, #4
	bl 0x0200bf98
	movs r3, #32
	movs r2, #77
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #32
	movs r1, #37
	movs r2, #1
	movs r3, #4
	bl 0x0200be88
.L_02000bd4_4:
	ldr r0, [pc, #668]
	bl 0x0200bee8
	cmp r0, #0
	beq .L_02000bd4_5
	movs r0, #20
	bl 0x0200bf50
	mov r2, r10
	adds r0, #85
	strb r2, [r0]
	movs r0, #20
	bl 0x0200bf50
	mov r3, r8
	adds r0, #35
	strb r3, [r0]
	movs r1, #5
	movs r0, #20
	bl 0x0200bf98
	movs r3, #44
	movs r2, #17
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #46
	movs r1, #17
	movs r2, #1
	movs r3, #1
	bl 0x0200be88
.L_02000bd4_5:
	ldr r0, [pc, #612]
	bl 0x0200bee8
	cmp r0, #0
	beq .L_02000bd4_6
	movs r0, #21
	bl 0x0200bf50
	mov r1, r10
	adds r0, #85
	strb r1, [r0]
	movs r0, #21
	bl 0x0200bf50
	mov r2, r8
	adds r0, #35
	strb r2, [r0]
	movs r1, #5
	movs r0, #21
	bl 0x0200bf98
	movs r3, #50
	movs r2, #17
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #46
	movs r1, #17
	movs r2, #1
	movs r3, #1
	bl 0x0200be88
.L_02000bd4_6:
	movs r0, #32
	bl 0x0200bf50
	adds r5, r0, #0
	ldr r3, [r5, #8]
	asrs r2, r3, #20
	adds r3, r5, #0
	adds r3, #85
	mov r1, r10
	strb r1, [r3]
	subs r3, #50
	mov r1, r8
	strb r1, [r3]
	movs r3, #10
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r1, #28
	movs r2, #1
	movs r3, #3
	movs r0, #52
	bl 0x0200be88
	movs r0, #33
	bl 0x0200bf50
	adds r5, r0, #0
	ldr r3, [r5, #8]
	asrs r2, r3, #20
	adds r3, r5, #0
	adds r3, #85
	mov r1, r10
	strb r1, [r3]
	subs r3, #50
	mov r1, r8
	strb r1, [r3]
	movs r3, #13
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #52
	movs r1, #28
	movs r2, #1
	movs r3, #3
	bl 0x0200be88
	movs r0, #208
	lsls r0, r0, #2
	bl 0x0200bf00
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02000bd4_7
	movs r6, #73
.L_02000bd4_7:
	movs r0, #12
	bl 0x0200bf50
	movs r2, #128
	lsls r3, r6, #20
	lsls r2, r2, #12
	adds r5, r0, #0
	adds r3, r3, r2
	str r3, [r5, #8]
	adds r3, r5, #0
	adds r3, #85
	mov r1, r10
	strb r1, [r3]
	mov r2, r8
	subs r3, #50
	strb r2, [r3]
	movs r0, #71
	movs r1, #16
	movs r2, #1
	movs r3, #1
	movs r7, #16
	str r6, [sp, #0]
	str r7, [sp, #4]
	bl 0x0200be88
	movs r0, #210
	lsls r0, r0, #2
	bl 0x0200bf00
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02000bd4_8
	movs r6, #76
.L_02000bd4_8:
	movs r0, #13
	bl 0x0200bf50
	movs r1, #128
	lsls r3, r6, #20
	lsls r1, r1, #12
	adds r5, r0, #0
	adds r3, r3, r1
	str r3, [r5, #8]
	adds r3, r5, #0
	adds r3, #85
	mov r2, r10
	strb r2, [r3]
	mov r1, r8
	subs r3, #50
	strb r1, [r3]
	movs r0, #71
	movs r1, #16
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r7, [sp, #4]
	bl 0x0200be88
	movs r0, #212
	lsls r0, r0, #2
	bl 0x0200bf00
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02000bd4_9
	movs r6, #79
.L_02000bd4_9:
	movs r0, #14
	bl 0x0200bf50
	movs r2, #128
	lsls r2, r2, #12
	lsls r3, r6, #20
	adds r5, r0, #0
	adds r3, r3, r2
	str r3, [r5, #8]
	adds r3, r5, #0
	adds r3, #85
	mov r1, r10
	strb r1, [r3]
	mov r2, r8
	subs r3, #50
	strb r2, [r3]
	movs r0, #71
	movs r1, #16
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r7, [sp, #4]
	bl 0x0200be88
	bl 0x0200862c
	movs r0, #31
	movs r1, #10
	bl 0x0200bf98
	movs r0, #205
	lsls r0, r0, #2
	bl 0x0200bee8
	cmp r0, #0
	beq .L_02000bd4_10
	movs r3, #13
	movs r6, #22
	mov r8, r3
	movs r7, #58
.L_02000bd4_11:
	adds r0, r6, #0
	bl 0x0200bf50
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	movs r1, #4
	bl 0x0200be48
	mov r1, r8
	str r1, [sp, #4]
	movs r0, #56
	movs r1, #13
	movs r2, #1
	movs r3, #1
	adds r6, #1
	str r7, [sp, #0]
	bl 0x0200be88
	adds r7, #2
	cmp r6, #25
	ble .L_02000bd4_11
	movs r0, #31
	movs r1, #10
	bl 0x0200bf98
	movs r0, #31
	bl 0x0200c0a0
	b .L_02000bd4_12
.L_02000bd4_10:
	movs r2, #2
	movs r7, #128
	movs r6, #22
	mov r8, r2
	lsls r7, r7, #8
.L_02000bd4_13:
	adds r0, r6, #0
	bl 0x0200bf50
	adds r5, r0, #0
	adds r3, r5, #0
	adds r3, #35
	mov r1, r8
	strb r1, [r3]
	movs r1, #4
	bl 0x0200be48
	ldr r3, [pc, #144]
	adds r6, #1
	str r7, [r5, #48]
	str r3, [r5, #52]
	cmp r6, #25
	ble .L_02000bd4_13
	ldr r5, [pc, #136]
	ldr r1, [pc, #136]
	adds r0, r5, #0
	bl 0x0200bda8
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200be38
.L_02000bd4_12:
	movs r0, #8
	movs r1, #9
	bl 0x0200bf98
	ldr r5, [pc, #116]
	movs r3, #249
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #0
	strb r3, [r2]
	movs r1, #89
	movs r0, #41
	bl 0x0200b138
	movs r1, #77
	movs r0, #40
	bl 0x0200b138
	movs r1, #1
	movs r0, #8
	bl 0x0200bfc8
	movs r1, #225
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #1
	cmp r3, #4
	bls .L_02000bd4_14
	b .L_02000bd4_15
.L_02000bd4_14:
	ldr r2, [pc, #68]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	movs r0, r0
	ldrh r0, [r1, #62]
	lsls r0, r0, #8
	str r0, [sp, #368]
	lsls r0, r0, #8
	str r0, [sp, #568]
	lsls r0, r0, #8
	str r0, [sp, #656]
	lsls r0, r0, #8
	str r0, [sp, #712]
	lsls r0, r0, #8
	.4byte 0x03001ebc
	.4byte 0xfff80000
	.4byte 0x00000335
	.4byte 0x00000333
	.4byte 0x00000331
	.4byte 0x00000332
	.4byte 0x00003333
	.4byte 0x02008715
	.4byte 0x00000c85
	.4byte 0x02000240
	.4byte 0x02008f88
	.2byte 0x2280
	.2byte 0x0452
	.2byte 0x9200
	.2byte 0x2228
	.2byte 0x9201
	.2byte 0x23d0
	.2byte 0x2229
	.2byte 0x9202
	.2byte 0x03db
	.2byte 0x2108
	.2byte 0x2205
	.2byte 0x2000
	.2byte 0xf002
	.2byte 0xfd0c
	.2byte 0x234f
	.2byte 0x2206
	.2byte 0x9300
	.2byte 0x9201
	.2byte 0x2100
	.2byte 0x2201
	.2byte 0x2302
	.2byte 0x207f
	.2byte 0xf002
	.2byte 0xff4c
	.2byte 0x2022
	.2byte 0xf002
	.2byte 0xffad
	.2byte 0x2023
	.2byte 0xf002
	.2byte 0xffaa
	.2byte 0x2024
	.2byte 0xf002
	.2byte 0xffa7
	.2byte 0x2025
	.2byte 0xf002
	.2byte 0xffa4
	.2byte 0x2026
	.2byte 0xf002
	.2byte 0xffa1
	.2byte 0x2027
	.2byte 0xf002
	.2byte 0xff9e
	.2byte 0x482c
	.2byte 0xf002
	.2byte 0xff63
	.2byte 0x2800
	.2byte 0xd10a
	.2byte 0x2011
	.2byte 0xf003
	.2byte 0xf842
	.2byte 0x2000
	.2byte 0xf000
	.2byte 0xfdbb
	.2byte 0xf000
	.2byte 0xfd7d
	.2byte 0x2002
	.2byte 0xf001
	.2byte 0xfcca
	.2byte 0x2001
	.2byte 0x2100
	.2byte 0xf003
	.2byte 0xf82a
	.2byte 0x2002
	.2byte 0x2100
	.2byte 0xf003
	.2byte 0xf826
	.2byte 0x2003
	.2byte 0x2100
	.2byte 0xf003
	.2byte 0xf822
	.2byte 0x481f
	.2byte 0xf002
	.2byte 0xfd45
	.2byte 0xe031
	.2byte 0x21c8
	.2byte 0x0109
	.2byte 0x481d
	.2byte 0xf002
	.2byte 0xfea1
	.2byte 0x2028
	.2byte 0xf002
	.2byte 0xff76
	.2byte 0x2029
	.2byte 0xf002
	.2byte 0xff73
	.2byte 0x4817
	.2byte 0xf002
	.2byte 0xff38
	.2byte 0x2800
	.2byte 0xd121
	.2byte 0xf000
	.2byte 0xfd58
	.2byte 0x2001
	.2byte 0xf000
	.2byte 0xfd91
	.2byte 0x2000
	.2byte 0xf001
	.2byte 0xfca2
	.2byte 0xe018
	.2byte 0x4810
	.2byte 0xf002
	.2byte 0xff2a
	.2byte 0x2800
	.2byte 0xd113
	.2byte 0x2022
	.2byte 0xf000
	.2byte 0xf81f
	.2byte 0xf000
	.2byte 0xff8b
	.2byte 0xe00d
	.2byte 0x2002
	.2byte 0xf7ff
	.2byte 0xfd47
	.2byte 0x2004
	.2byte 0xf002
	.2byte 0xffc0
	.2byte 0xe006
	.2byte 0x2002
	.2byte 0x4240
	.2byte 0xf7ff
	.2byte 0xfd3f
	.2byte 0x2005
	.2byte 0xf002
	.2byte 0xffb8
.L_02000bd4_15:
	movs r0, #0
	sub sp, #-12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0109
	.2byte 0x0000
	.2byte 0x00e5
	.2byte 0x0000
	.2byte 0x9c79
	.2byte 0x0200
	.global Func_020010dc
	.thumb_func
Func_020010dc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r7, r0, #0
	bl 0x0200bf50
	movs r3, #10
	ldrsh r2, [r0, r3]
	mov r9, r2
	movs r3, #18
	ldrsh r2, [r0, r3]
	mov r10, r2
	bl 0x0200bf38
	movs r1, #128
	movs r2, #128
	adds r0, r7, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bf60
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bf60
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bf60
	movs r1, #128
	movs r2, #128
	movs r0, #2
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bf60
	movs r1, #128
	movs r2, #128
	movs r0, #3
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bf60
	mov r3, r10
	lsls r5, r3, #16
	mov r2, r9
	ldr r3, [pc, #396]
	lsls r6, r2, #16
	adds r1, r6, #0
	adds r2, r5, r3
	movs r0, #0
	bl 0x0200bf90
	ldr r3, [pc, #388]
	ldr r2, [pc, #388]
	adds r3, r3, r5
	mov r8, r3
	adds r1, r6, r2
	movs r0, #1
	mov r2, r8
	bl 0x0200bf90
	movs r2, #128
	lsls r2, r2, #13
	adds r1, r6, r2
	movs r0, #2
	mov r2, r8
	bl 0x0200bf90
	ldr r3, [pc, #364]
	adds r1, r6, #0
	adds r2, r5, r3
	movs r0, #3
	bl 0x0200bf90
	ldr r2, [pc, #356]
	adds r5, r5, r2
	adds r2, r5, #0
	adds r1, r6, #0
	adds r0, r7, #0
	bl 0x0200bf90
	movs r0, #0
	bl 0x0200bf50
	movs r6, #192
	lsls r6, r6, #8
	movs r1, #0
	strh r6, [r0, #6]
	movs r0, #0
	bl 0x0200c010
	bl 0x0200c060
	bl 0x0200c070
	ldr r0, [pc, #316]
	bl 0x0200bfd0
	adds r0, r7, #0
	movs r1, #3
	bl 0x0200bfa0
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200bfe0
	adds r0, r7, #0
	movs r1, #2
	bl 0x0200bfa8
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200bfe0
	adds r0, r7, #0
	movs r1, #2
	bl 0x0200bfa8
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200bfe0
	adds r0, r7, #0
	movs r1, #2
	bl 0x0200bfa8
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200bfe0
	movs r0, #3
	movs r1, #3
	bl 0x0200bf98
	movs r0, #1
	movs r1, #3
	bl 0x0200bf98
	movs r0, #2
	movs r1, #3
	bl 0x0200bf98
	movs r1, #3
	movs r0, #0
	bl 0x0200bfa0
	movs r0, #6
	bl 0x0200bf30
	movs r0, #1
	movs r1, #2
	bl 0x0200bf98
	movs r0, #0
	bl 0x0200bf50
	cmp r0, #0
	beq .L_020010dc_0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200bf70
.L_020010dc_0:
	movs r0, #2
	movs r1, #2
	bl 0x0200bf98
	movs r0, #0
	bl 0x0200bf50
	cmp r0, #0
	beq .L_020010dc_1
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #2
	bl 0x0200bf70
.L_020010dc_1:
	movs r0, #3
	movs r1, #2
	bl 0x0200bf98
	movs r0, #0
	bl 0x0200bf50
	cmp r0, #0
	beq .L_020010dc_2
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #3
	bl 0x0200bf70
.L_020010dc_2:
	mov r5, r9
	subs r5, #16
	mov r2, r10
	adds r0, r7, #0
	adds r1, r5, #0
	subs r2, #64
	bl 0x0200bf80
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200bf90
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x0200bf90
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x0200bf90
	mov r2, r10
	adds r0, r7, #0
	adds r1, r5, #0
	subs r2, #16
	bl 0x0200bf80
	adds r0, r7, #0
	mov r1, r9
	mov r2, r10
	bl 0x0200bf80
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #10
	bl 0x0200bff0
	bl 0x0200bf40
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0xffd00000
	.4byte 0xffd80000
	.4byte 0xfff00000
	.4byte 0xffe00000
	.4byte 0xffb00000
	.4byte 0x000020e9
	.section .text.x02009ba8,"ax",%progbits
	.balign 4
	.global Korosseo_SelectSoloCompetitor
	.thumb_func
Korosseo_SelectSoloCompetitor:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #0
	bl 0x0200bf20
	movs r0, #1
	bl 0x0200bf20
	movs r0, #2
	bl 0x0200bf20
	movs r0, #3
	bl 0x0200bf20
	movs r0, #5
	bl 0x0200bf20
	adds r0, r5, #0
	bl 0x0200bf18
	ldr r3, [pc, #76]
	movs r1, #250
	lsls r1, r1, #1
	adds r3, r3, r1
	str r5, [r3]
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200c010
	adds r0, r5, #0
	bl 0x0200bed0
	adds r5, r0, #0
	ldrh r3, [r5, #52]
	ldr r1, [pc, #52]
	strh r3, [r5, #56]
	ldrh r3, [r5, #54]
	ldr r2, [pc, #40]
	strh r3, [r5, #58]
	adds r3, r5, r1
	strb r2, [r3]
	movs r2, #56
	ldrsh r0, [r5, r2]
	movs r3, #52
	ldrsh r1, [r5, r3]
	lsls r0, r0, #14
	bl 0x0200bd98
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bgt .L_02001ba8_0
	movs r3, #0
	cmp r0, #0
	blt .L_02001ba8_0
	adds r3, r0, #0
	b .L_02001ba8_0
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x02000240
	.4byte 0x00000131
.L_02001ba8_0:
	strh r3, [r5, #20]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_02001ba8_1
	movs r1, #56
	ldrsh r3, [r5, r1]
	cmp r3, #0
	beq .L_02001ba8_1
	movs r3, #1
	strh r3, [r5, #20]
.L_02001ba8_1:
	movs r2, #58
	ldrsh r0, [r5, r2]
	movs r3, #54
	ldrsh r1, [r5, r3]
	lsls r0, r0, #14
	bl 0x0200bd98
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bgt .L_02001ba8_2
	movs r3, #0
	cmp r0, #0
	blt .L_02001ba8_2
	adds r3, r0, #0
.L_02001ba8_2:
	strh r3, [r5, #22]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_02001ba8_3
	movs r1, #58
	ldrsh r3, [r5, r1]
	cmp r3, #0
	beq .L_02001ba8_3
	movs r3, #1
	strh r3, [r5, #22]
.L_02001ba8_3:
	bl 0x0200c090
	pop {r5}
	pop {r0}
	bx r0
	.section .text.x02009df4,"ax",%progbits
	.balign 4
	.global Korosseo_FinishSoloRound
	.thumb_func
Korosseo_FinishSoloRound:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, [pc, #248]
	adds r7, r0, #0
	ldr r0, [r5]
	mov r9, r0
	adds r0, r7, #0
	bl 0x0200bf50
	adds r0, r7, #0
	bl 0x0200bf50
	ldr r3, [pc, #232]
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r6, [r3]
	adds r0, r6, #0
	bl 0x0200bf50
	mov r11, r0
	bl 0x0200bf38
	ldr r3, [pc, #212]
	mov r8, r3
	mov r0, r8
	bl 0x0200bfd0
	movs r1, #0
	adds r0, r7, #0
	bl 0x0200bfd8
	ldr r2, [r5]
	ldr r0, [pc, #196]
	ldr r1, [pc, #200]
	adds r3, r2, r0
	strh r1, [r3]
	ldr r3, [pc, #196]
	adds r2, r2, r3
	movs r3, #4
	strh r3, [r2]
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200bf48
	mov r10, r0
	cmp r0, #0
	bne .L_02001df4_0
	mov r0, r8
	adds r0, #1
	bl 0x0200bfd0
	adds r0, r7, #0
	movs r1, #0
	movs r7, #224
	bl 0x0200bfe0
	lsls r7, r7, #1
	movs r3, #128
	movs r2, #228
	lsls r3, r3, #2
	add r7, r9
	lsls r2, r2, #1
	add r2, r9
	str r3, [r7]
	movs r3, #15
	str r3, [r2]
	bl 0x0200c068
	bl 0x0200c070
	mov r0, r11
	ldr r1, [r0, #8]
	movs r2, #220
	lsls r5, r6, #4
	lsls r2, r2, #2
	adds r0, r5, r2
	asrs r1, r1, #20
	bl 0x0200bf08
	mov r3, r11
	ldr r1, [r3, #16]
	movs r2, #222
	lsls r2, r2, #2
	asrs r1, r1, #20
	adds r0, r5, r2
	adds r6, #1
	bl 0x0200bf08
	cmp r6, #3
	ble .L_02001df4_1
	movs r0, #10
	bl 0x0200c030
	movs r0, #141
	lsls r0, r0, #1
	bl 0x0200bef0
	b .L_02001df4_2
.L_02001df4_1:
	adds r0, r6, #0
	bl 0x02009ba8
	bl 0x0200c060
	bl 0x0200c070
	mov r3, r10
	str r3, [r7]
	b .L_02001df4_2
.L_02001df4_0:
	mov r0, r8
	adds r0, #2
	bl 0x0200bfd0
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200bfe0
.L_02001df4_2:
	bl 0x0200bf40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00002086
	.4byte 0x00000cc2
	.4byte 0x00002089
	.4byte 0x00000cc4
	.section .text.x02009ffc,"ax",%progbits
	.balign 4
	.global KorosseoKabe_RunStateInteraction
	.thumb_func
KorosseoKabe_RunStateInteraction:
	push {r5, r6, lr}
	adds r5, r1, #0
	adds r6, r0, #0
	bl 0x0200c080
	movs r1, #5
	adds r0, r5, #0
	bl 0x0200beb0
	ldr r3, [pc, #140]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #132]
	cmp r2, r3
	bne .L_02001ffc_0
	ldr r0, [pc, #128]
	b .L_02001ffc_1
.L_02001ffc_0:
	ldr r3, [pc, #128]
	cmp r2, r3
	bne .L_02001ffc_2
	ldr r0, [pc, #128]
	b .L_02001ffc_1
.L_02001ffc_2:
	ldr r0, [pc, #128]
.L_02001ffc_1:
	bl 0x0200bfd0
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200bfe0
	movs r2, #128
	lsls r2, r2, #2
	adds r0, r5, r2
	bl 0x0200bee8
	cmp r0, #0
	bne .L_02001ffc_3
	movs r3, #130
	lsls r3, r3, #2
	adds r5, r5, r3
	adds r0, r5, #0
	bl 0x0200bee8
	cmp r0, #0
	beq .L_02001ffc_4
	movs r0, #0
	bl 0x0200bec0
	cmp r0, #1
	bne .L_02001ffc_5
.L_02001ffc_3:
	movs r0, #2
	b .L_02001ffc_6
.L_02001ffc_5:
	cmp r0, #2
	beq .L_02001ffc_7
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	bne .L_02001ffc_6
.L_02001ffc_7:
	movs r0, #3
	b .L_02001ffc_6
.L_02001ffc_4:
	adds r0, r5, #0
	bl 0x0200bef0
	ldr r0, [pc, #52]
	bl 0x0200bfd0
	movs r1, #0
	adds r0, r6, #0
	bl 0x0200bfd8
	movs r0, #0
	movs r1, #0
	bl 0x0200bf48
.L_02001ffc_6:
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000008f
	.4byte 0x00002076
	.4byte 0x00000090
	.4byte 0x00002078
	.4byte 0x0000207a
	.4byte 0x0000207c
	.global KorosseoKabe_ShowFollowUpPrompt
	.thumb_func
KorosseoKabe_ShowFollowUpPrompt:
	push {r5, lr}
	adds r5, r0, #0
	adds r0, r1, #0
	movs r1, #5
	bl 0x0200beb0
	ldr r3, [pc, #52]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_020020b8_0
	ldr r0, [pc, #44]
	b .L_020020b8_1
.L_020020b8_0:
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_020020b8_2
	ldr r0, [pc, #40]
	b .L_020020b8_1
.L_020020b8_2:
	ldr r0, [pc, #40]
.L_020020b8_1:
	adds r0, #1
	bl 0x0200bfd0
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200bfe0
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000008f
	.4byte 0x00002076
	.4byte 0x00000090
	.4byte 0x00002078
	.4byte 0x0000207a
	.section .text.x0200a3bc,"ax",%progbits
	.balign 4
	.global Korosseo_LoadPortrait
	.thumb_func
Korosseo_LoadPortrait:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #229
	lsls r0, r0, #5
	bl 0x0200bde8
	ldr r7, [pc, #104]
	movs r2, #0
	ldrsh r3, [r7, r2]
	movs r2, #1
	negs r2, r2
	adds r6, r0, #0
	cmp r3, r2
	bne .L_020023bc_0
	bl 0x0200be18
	strh r0, [r7]
.L_020023bc_0:
	ldr r3, [pc, #88]
	ldrb r3, [r3, r5]
	mov r8, r3
	cmp r5, #8
	bne .L_020023bc_1
	movs r5, #4
.L_020023bc_1:
	ldr r0, [pc, #80]
	bl 0x0200be30
	adds r1, r6, #0
	bl 0x0200bdf8
	mov r2, r8
	adds r0, r6, r2
	ldr r3, [pc, #68]
	ldr r1, [pc, #68]
	ldr r2, [pc, #72]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	lsls r2, r5, #10
	adds r2, r2, r6
	movs r1, #128
	adds r2, #160
	lsls r1, r1, #3
	movs r3, #0
	ldrsh r0, [r7, r3]
	bl 0x0200be10
	movs r2, #128
	ldr r1, [pc, #36]
	lsls r2, r2, #24
.L_020023bc_2:
	ldr r3, [r1, #8]
	ands r3, r2
	cmp r3, #0
	bne .L_020023bc_2
	adds r0, r6, #0
	bl 0x0200bdf0
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200ca1c
	.4byte 0x0200c0c4
	.4byte 0x000000e7
	.4byte 0x040000d4
	.4byte 0x050003e0
	.4byte 0x84000008
	.global CommandInterpolationRenderer_Update
	.thumb_func
CommandInterpolationRenderer_Update:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r0, [pc, #340]
	sub	sp, #20
	str	r0, [sp, #8]
	ldr	r3, [pc, #336]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	ldr	r2, [pc, #336]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	lsrs	r3, r3, #5
	mov	sl, r0
	mov	r8, r3
.L_02002478:
	ldr	r1, [pc, #324]
	movs	r2, #0
	ldrsh	r4, [r1, r2]
	ldrh	r3, [r1, #0]
	cmp	r4, #0
	bne.n	.L_0200254e
	ldr	r5, [pc, #316]
	ldr	r0, [r5, #0]
	ldrh	r3, [r0, #0]
	movs	r2, #128
	lsls	r3, r3, #16
	adds	r0, #2
	asrs	r3, r3, #16
	lsls	r2, r2, #6
	str	r0, [r5, #0]
	cmp	r3, r2
	beq.n	.L_02002514
	cmp	r3, r2
	bgt.n	.L_020024b0
	movs	r1, #1
	negs	r1, r1
	cmp	r3, r1
	beq.n	.L_0200253c
	movs	r2, #128
	lsls	r2, r2, #5
	cmp	r3, r2
	beq.n	.L_020024fc
	b.n	.L_02002478
.L_020024b0:
	movs	r2, #128
	lsls	r2, r2, #7
	cmp	r3, r2
	beq.n	.L_020024ce
	cmp	r3, r2
	bgt.n	.L_020024c6
	movs	r1, #192
	lsls	r1, r1, #6
	cmp	r3, r1
	beq.n	.L_020024e4
	b.n	.L_02002478
.L_020024c6:
	ldr	r2, [pc, #256]
	cmp	r3, r2
	beq.n	.L_02002532
	b.n	.L_02002478
.L_020024ce:
	movs	r1, #0
	ldrsh	r3, [r0, r1]
	ldr	r2, [pc, #248]
	lsls	r3, r3, #8
	str	r3, [r2, #0]
	adds	r2, r0, #2
	ldrh	r3, [r2, #0]
	ldr	r1, [pc, #240]
	strh	r3, [r1, #0]
	ldr	r3, [pc, #240]
	b.n	.L_0200252a
.L_020024e4:
	ldr	r2, [pc, #232]
	ldr	r1, [pc, #240]
	ldrh	r3, [r2, #0]
	strh	r3, [r1, #0]
	ldrh	r3, [r0, #0]
	strh	r3, [r2, #0]
	adds	r2, r0, #2
	ldrh	r3, [r2, #0]
	ldr	r1, [pc, #220]
	strh	r3, [r1, #0]
	ldr	r3, [pc, #224]
	b.n	.L_0200252a
.L_020024fc:
	ldr	r2, [pc, #224]
	ldr	r1, [pc, #228]
	ldrh	r3, [r2, #0]
	strh	r3, [r1, #0]
	ldrh	r3, [r0, #0]
	strh	r3, [r2, #0]
	adds	r2, r0, #2
	ldrh	r3, [r2, #0]
	ldr	r1, [pc, #216]
	strh	r3, [r1, #0]
	ldr	r3, [pc, #216]
	b.n	.L_0200252a
.L_02002514:
	ldr	r2, [pc, #216]
	ldr	r1, [pc, #220]
	ldrh	r3, [r2, #0]
	strh	r3, [r1, #0]
	ldrh	r3, [r0, #0]
	strh	r3, [r2, #0]
	adds	r2, r0, #2
	ldrh	r3, [r2, #0]
	ldr	r1, [pc, #208]
	strh	r3, [r1, #0]
	ldr	r3, [pc, #208]
.L_0200252a:
	adds	r2, #2
	str	r2, [r5, #0]
	strh	r4, [r3, #0]
	b.n	.L_02002478
.L_02002532:
	ldrh	r3, [r0, #0]
	strh	r3, [r1, #0]
	adds	r3, r0, #2
	str	r3, [r5, #0]
	b.n	.L_02002478
.L_0200253c:
	ldr	r0, [pc, #192]
	bl 0x0200bdb0
	ldr	r3, [pc, #116]
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200be00
	b.n	.L_02002904
.L_0200254e:
	subs	r3, #1
	strh	r3, [r1, #0]
	ldr	r3, [pc, #148]
	movs	r5, #0
	ldrsh	r7, [r3, r5]
	mov	r9, r3
	cmp	r7, #0
	bne.n	.L_02002568
	ldr	r3, [pc, #128]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	mov	fp, r0
	b.n	.L_0200259a
.L_02002568:
	ldr	r3, [pc, #120]
	movs	r2, #0
	ldrsh	r6, [r3, r2]
	ldr	r2, [pc, #124]
	ldr	r3, [pc, #108]
	movs	r5, #0
	ldrsh	r3, [r3, r5]
	ldrh	r5, [r2, #0]
	adds	r5, #1
	strh	r5, [r2, #0]
	lsls	r5, r5, #16
	subs	r3, r3, r6
	asrs	r5, r5, #16
	adds	r0, r5, #0
	muls	r0, r3
	adds	r1, r7, #0
	bl 0x0200bd98
	adds	r6, r6, r0
	mov	fp, r6
	cmp	r5, r7
	blt.n	.L_0200259a
	ldr	r3, [pc, #24]
	mov	r0, r9
	strh	r3, [r0, #0]
.L_0200259a:
	ldr	r1, [pc, #92]
	movs	r2, #0
	ldrsh	r7, [r1, r2]
	mov	r9, r1
	cmp	r7, #0
	bne.n	.L_02002604
	ldr	r3, [pc, #72]
	movs	r0, #0
	ldrsh	r5, [r3, r0]
	str	r5, [sp, #4]
	b.n	.L_02002636
	.4byte 0x00000000
	.4byte 0x0200cc60
	.4byte 0x0200ca1c
	.4byte 0x03001b10
	.4byte 0x0200cc3c
	.4byte 0x0200cc40
	.4byte 0x00007fff
	.4byte 0x0200cc10
	.4byte 0x0200cc98
	.4byte 0x0200cc0c
	.4byte 0x0200cc90
	.4byte 0x0200cc1c
	.4byte 0x0200cc18
	.4byte 0x0200cc08
	.4byte 0x0200cbf4
	.4byte 0x0200cc9c
	.4byte 0x0200cc34
	.4byte 0x0200cc38
	.4byte 0x0200cc48
	.4byte 0x0200cc24
	.2byte 0xa451
	.2byte 0x0200
.L_02002604:
	ldr	r3, [pc, #72]
	movs	r1, #0
	ldrsh	r6, [r3, r1]
	ldr	r3, [pc, #72]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	ldr	r2, [pc, #68]
	ldrh	r5, [r2, #0]
	adds	r5, #1
	strh	r5, [r2, #0]
	lsls	r5, r5, #16
	subs	r3, r3, r6
	asrs	r5, r5, #16
	adds	r0, r5, #0
	muls	r0, r3
	adds	r1, r7, #0
	bl 0x0200bd98
	adds	r6, r6, r0
	str	r6, [sp, #4]
	cmp	r5, r7
	blt.n	.L_02002636
	ldr	r3, [pc, #24]
	mov	r5, r9
	strh	r3, [r5, #0]
.L_02002636:
	ldr	r0, [pc, #36]
	movs	r1, #0
	ldrsh	r7, [r0, r1]
	mov	r9, r0
	cmp	r7, #0
	bne.n	.L_02002664
	ldr	r3, [pc, #28]
	movs	r2, #0
	ldrsh	r6, [r3, r2]
	b.n	.L_02002694
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0200cc38
	.4byte 0x0200cc34
	.4byte 0x0200cc24
	.4byte 0x0200cc0c
	.2byte 0xcc98
	.2byte 0x0200
.L_02002664:
	ldr	r2, [pc, #100]
	ldr	r3, [pc, #104]
	movs	r5, #0
	ldrsh	r6, [r3, r5]
	ldrh	r5, [r2, #0]
	ldr	r3, [pc, #100]
	adds	r5, #1
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	strh	r5, [r2, #0]
	lsls	r5, r5, #16
	subs	r3, r3, r6
	asrs	r5, r5, #16
	adds	r0, r5, #0
	muls	r0, r3
	adds	r1, r7, #0
	bl 0x0200bd98
	adds	r6, r6, r0
	cmp	r5, r7
	blt.n	.L_02002694
	ldr	r3, [pc, #56]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_02002694:
	add	r0, sp, #12
	ldr	r3, [r0, #4]
	ldr	r2, [pc, #60]
	ands	r3, r2
	str	r3, [r0, #4]
	mov	r3, fp
	lsls	r1, r3, #16
	ldr	r3, [sp, #12]
	lsrs	r1, r1, #16
	ands	r3, r2
	ldr	r2, [pc, #48]
	orrs	r3, r1
	ands	r3, r2
	lsls	r1, r1, #16
	orrs	r3, r1
	str	r3, [sp, #12]
	bl 0x0200be20
	ldr	r2, [pc, #36]
	ldr	r3, [r2, #0]
	lsls	r0, r0, #16
	adds	r3, r3, r6
	asrs	r0, r0, #16
	str	r3, [r2, #0]
	b.n	.L_020026e4
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0200cc1c
	.4byte 0x0200cc90
	.4byte 0x0200cc98
	.4byte 0xffff0000
	.4byte 0x0000ffff
	.2byte 0xcc10
	.2byte 0x0200
.L_020026e4:
	cmp	r3, #0
	bge.n	.L_020026ea
	adds	r3, #255
.L_020026ea:
	asrs	r6, r3, #8
	ldr	r3, [pc, #552]
	movs	r5, #0
	ldrsh	r3, [r3, r5]
	cmp	r3, #2
	bne.n	.L_020026f8
	b.n	.L_02002850
.L_020026f8:
	cmp	r3, #2
	bgt.n	.L_02002702
	cmp	r3, #1
	beq.n	.L_0200270c
	b.n	.L_020028a2
.L_02002702:
	cmp	r3, #3
	beq.n	.L_02002782
	cmp	r3, #4
	beq.n	.L_020027fe
	b.n	.L_020028a2
.L_0200270c:
	lsls	r0, r0, #25
	ldr	r4, [pc, #524]
	movs	r5, #0
	movs	r7, #56
	mov	r9, r0
.L_02002716:
	lsls	r3, r5, #5
	subs	r3, #48
	mov	r0, fp
	muls	r0, r3
	adds	r3, r0, #0
	cmp	r3, #0
	bge.n	.L_02002726
	adds	r3, #255
.L_02002726:
	asrs	r3, r3, #8
	adds	r3, r6, r3
	ldr	r1, [pc, #500]
	adds	r2, r3, #0
	adds	r3, #152
	adds	r2, #88
	cmp	r3, r1
	bhi.n	.L_02002776
	ldr	r3, [pc, #492]
	ldr	r1, [sp, #8]
	ands	r2, r3
	movs	r3, #0
	stmia	r1!, {r3}
	lsls	r3, r2, #16
	orrs	r3, r7
	orrs	r3, r4
	mov	r2, r9
	orrs	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r0, r1, #0
	orrs	r3, r2
	str	r0, [sp, #8]
	stmia	r1!, {r3}
	movs	r3, #244
	adds	r0, r1, #0
	lsls	r3, r3, #8
	mov	r1, r8
	orrs	r3, r1
	stmia	r0!, {r3}
	movs	r1, #12
	adds	r2, r0, #0
	mov	r0, sl
	add	sl, r1
	movs	r1, #236
	str	r4, [sp, #0]
	str	r2, [sp, #8]
	bl 0x0200be28
	ldr	r4, [sp, #0]
.L_02002776:
	movs	r2, #8
	adds	r5, #1
	add	r8, r2
	cmp	r5, #3
	ble.n	.L_02002716
	b.n	.L_020028a2
.L_02002782:
	lsls	r0, r0, #25
	ldr	r4, [pc, #404]
	movs	r5, #0
	movs	r7, #48
	mov	r9, r0
.L_0200278c:
	lsls	r3, r5, #5
	subs	r3, #16
	mov	r0, fp
	muls	r0, r3
	adds	r3, r0, #0
	cmp	r3, #0
	bge.n	.L_0200279c
	adds	r3, #255
.L_0200279c:
	asrs	r3, r3, #8
	adds	r3, r6, r3
	ldr	r1, [pc, #380]
	adds	r2, r3, #0
	adds	r3, #152
	adds	r2, #88
	cmp	r3, r1
	bhi.n	.L_020027f2
	ldr	r3, [pc, #372]
	ldr	r1, [sp, #8]
	ands	r2, r3
	movs	r3, #0
	stmia	r1!, {r3}
	lsls	r3, r2, #16
	orrs	r3, r7
	orrs	r3, r4
	mov	r2, r9
	orrs	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r0, r1, #0
	orrs	r3, r2
	str	r0, [sp, #8]
	stmia	r1!, {r3}
	ldr	r3, [pc, #344]
	adds	r0, r1, #0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	movs	r2, #244
	lsls	r2, r2, #8
	add	r3, r8
	orrs	r3, r2
	stmia	r0!, {r3}
	movs	r1, #12
	adds	r2, r0, #0
	mov	r0, sl
	add	sl, r1
	movs	r1, #236
	str	r4, [sp, #0]
	str	r2, [sp, #8]
	bl 0x0200be28
	ldr	r4, [sp, #0]
.L_020027f2:
	movs	r2, #8
	adds	r5, #1
	add	r8, r2
	cmp	r5, #1
	ble.n	.L_0200278c
	b.n	.L_020028a2
.L_020027fe:
	adds	r3, r6, #0
	movs	r5, #152
	adds	r2, r6, #0
	adds	r3, #120
	lsls	r5, r5, #1
	movs	r7, #48
	ldr	r4, [pc, #288]
	adds	r2, #56
	cmp	r3, r5
	bcs.n	.L_020028a2
	ldr	r3, [pc, #272]
	mov	r1, sl
	ands	r2, r3
	movs	r3, #0
	stmia	r1!, {r3}
	lsls	r3, r2, #16
	orrs	r3, r7
	lsls	r2, r0, #25
	orrs	r3, r4
	orrs	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r5, r1, #0
	orrs	r3, r2
	str	r5, [sp, #8]
	stmia	r1!, {r3}
	adds	r2, r1, #0
	str	r2, [sp, #8]
	ldr	r3, [pc, #240]
	movs	r5, #0
	ldrsh	r3, [r3, r5]
	movs	r2, #244
	add	r3, r8
	lsls	r2, r2, #8
	orrs	r3, r2
	str	r3, [r1, #0]
	mov	r0, sl
	movs	r1, #236
	bl 0x0200be28
	b.n	.L_020028a2
.L_02002850:
	adds	r3, r6, #0
	movs	r1, #152
	movs	r4, #128
	adds	r2, r6, #0
	adds	r3, #152
	lsls	r1, r1, #1
	movs	r7, #48
	lsls	r4, r4, #24
	adds	r2, #88
	cmp	r3, r1
	bcs.n	.L_020028a2
	ldr	r3, [pc, #188]
	mov	r5, sl
	ands	r2, r3
	movs	r3, #0
	stmia	r5!, {r3}
	lsls	r3, r2, #16
	orrs	r3, r7
	lsls	r2, r0, #25
	orrs	r3, r4
	orrs	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r1, r5, #0
	orrs	r3, r2
	str	r1, [sp, #8]
	stmia	r5!, {r3}
	adds	r0, r5, #0
	str	r0, [sp, #8]
	ldr	r3, [pc, #156]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	movs	r2, #244
	add	r3, r8
	lsls	r2, r2, #8
	orrs	r3, r2
	str	r3, [r5, #0]
	mov	r0, sl
	movs	r1, #236
	bl 0x0200be28
.L_020028a2:
	ldr	r0, [pc, #140]
	ldr	r1, [pc, #140]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r0, #0]
	cmp	r2, #31
	bgt.n	.L_020028d0
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r0
	strh	r2, [r0, #0]
	movs	r2, #252
	adds	r3, #4
	lsls	r2, r2, #6
	stmia	r3!, {r2}
	ldr	r2, [pc, #112]
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_020028d0:
	strh	r4, [r1, #0]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r3, [r0, #0]
	cmp	r3, #31
	bgt.n	.L_02002902
	lsls	r2, r3, #1
	adds	r2, r2, r3
	adds	r3, #1
	strh	r3, [r0, #0]
	ldr	r5, [sp, #4]
	movs	r3, #16
	lsls	r2, r2, #2
	subs	r3, r3, r5
	adds	r2, r2, r0
	lsls	r3, r3, #8
	adds	r2, #4
	orrs	r3, r5
	stmia	r2!, {r3}
	ldr	r3, [pc, #64]
	stmia	r2!, {r3}
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r2, #0]
.L_02002902:
	strh	r4, [r1, #0]
.L_02002904:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x0200cc30
	.4byte 0x80004000
	.4byte 0x0000012f
	.4byte 0x000001ff
	.4byte 0x0200cc04
	.4byte 0xc0004000
	.4byte 0x02002090
	.4byte 0x04000208
	.4byte 0x04000050
	.2byte 0x0052
	.2byte 0x0400
	.section .text.x0200ae84,"ax",%progbits
	.balign 4
	.global Korosseo_UpdateMarker
	.thumb_func
Korosseo_UpdateMarker:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #180]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	ldr	r2, [pc, #176]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	ldr	r2, [pc, #172]
	lsrs	r3, r3, #5
	ldr	r0, [pc, #172]
	mov	sl, r3
	movs	r3, #0
	ldrsh	r7, [r2, r3]
	mov	fp, r0
	mov	r9, r2
	cmp	r7, #0
	beq.n	.L_02002f18
	ldr	r3, [pc, #160]
	ldrh	r5, [r3, #0]
	adds	r5, #1
	strh	r5, [r3, #0]
	ldr	r0, [pc, #156]
	ldr	r1, [pc, #160]
	ldr	r3, [pc, #160]
	mov	r8, r0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	lsls	r5, r5, #16
	subs	r2, r2, r3
	asrs	r5, r5, #16
	ldrh	r6, [r1, #0]
	adds	r0, r5, #0
	muls	r0, r2
	adds	r1, r7, #0
	bl 0x0200bd98
	ldr	r2, [pc, #136]
	adds	r6, r6, r0
	mov	r1, r8
	strh	r6, [r1, #0]
	mov	r8, r2
	ldr	r3, [pc, #128]
	ldr	r2, [pc, #132]
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	ldrh	r6, [r2, #0]
	movs	r1, #0
	ldrsh	r2, [r2, r1]
	subs	r3, r3, r2
	adds	r0, r5, #0
	muls	r0, r3
	adds	r1, r7, #0
	bl 0x0200bd98
	mov	r2, r8
	adds	r6, r6, r0
	strh	r6, [r2, #0]
	cmp	r5, r7
	blt.n	.L_02002f12
	ldr	r3, [pc, #52]
	mov	r0, r9
	strh	r3, [r0, #0]
.L_02002f12:
	ldr	r2, [pc, #96]
	ldr	r3, [pc, #44]
	strh	r3, [r2, #0]
.L_02002f18:
	ldr	r2, [pc, #88]
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r3, #13
	bgt.n	.L_02002fa4
	ldr	r3, [pc, #48]
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	ldr	r3, [pc, #56]
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	mov	r0, fp
	movs	r3, #0
	stmia	r0!, {r3}
	subs	r1, #8
	ldr	r3, [pc, #56]
	lsls	r1, r1, #16
	b.n	.L_02002f7c
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0200cb46
	.4byte 0x03001b10
	.4byte 0x0200cc2c
	.4byte 0x0200cc50
	.4byte 0x0200cbf0
	.4byte 0x0200cc94
	.4byte 0x0200cc44
	.4byte 0x0200cc00
	.4byte 0x0200cc20
	.4byte 0x0200cca0
	.4byte 0x0200cc5c
	.4byte 0x0200cc14
	.2byte 0xcbf8
	.2byte 0x0200
.L_02002f7c:
	subs	r2, #8
	orrs	r2, r1
	movs	r4, #128
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	lsls	r4, r4, #23
	lsls	r3, r3, #28
	orrs	r2, r4
	orrs	r2, r3
	movs	r3, #128
	stmia	r0!, {r2}
	lsls	r3, r3, #3
	mov	r2, sl
	orrs	r2, r3
	str	r2, [r0, #0]
	movs	r1, #255
	mov	r0, fp
	bl 0x0200be28
	b.n	.L_02002fac
.L_02002fa4:
	cmp	r3, #19
	ble.n	.L_02002fac
	ldr	r3, [pc, #16]
	strh	r3, [r2, #0]
.L_02002fac:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.2byte 0x0000
	.section .text.x0200b22c,"ax",%progbits
	.balign 4
	.global Func_0200322c
	.thumb_func
Func_0200322c:
	ldr r2, [pc, #4]
	movs r3, #9
	strh r3, [r2]
	bx lr
	.4byte 0x02001000
	.global Func_02003238
	.thumb_func
Func_02003238:
	push {r5, lr}
	ldr r5, [pc, #28]
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #9
	beq .L_02003238_0
.L_02003238_1:
	movs r0, #1
	bl 0x0200bda0
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #9
	bne .L_02003238_1
.L_02003238_0:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x02001000
	.section .text.x0200b638,"ax",%progbits
	.global FieldScene_RunExtendedActorSequence
	.thumb_func
FieldScene_RunExtendedActorSequence:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #916]
	ldr	r3, [r3, #0]
	adds	r1, r3, #0
	sub	sp, #20
	mov	r8, r1
	str	r3, [sp, #12]
	str	r3, [sp, #16]
	mov	r7, r8
	adds	r7, #216
	movs	r1, #0
	ldrsh	r3, [r7, r1]
	ldr	r2, [pc, #896]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	lsrs	r3, r3, #5
	str	r3, [sp, #8]
	mov	r3, r8
	adds	r3, #230
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	subs	r3, #10
	mov	fp, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_02003686
	mov	r6, r8
	adds	r6, #218
	movs	r3, #2
	strh	r3, [r6, #0]
	b.n	.L_020036f4
.L_02003686:
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200bee8
	cmp	r0, #0
	beq.n	.L_020036a6
	mov	r6, r8
	adds	r6, #218
	movs	r1, #0
	ldrsh	r3, [r6, r1]
	ldrh	r2, [r6, #0]
	cmp	r3, #0
	ble.n	.L_020036f4
	subs	r3, r2, #1
.L_020036a2:
	strh	r3, [r6, #0]
	b.n	.L_020036f4
.L_020036a6:
	mov	r6, r8
	adds	r6, #218
	movs	r1, #0
	ldrsh	r3, [r6, r1]
	ldrh	r2, [r6, #0]
	cmp	r3, #1
	bgt.n	.L_020036f4
	adds	r3, r2, #1
	movs	r2, #128
	strh	r3, [r6, #0]
	lsls	r2, r2, #9
	lsls	r3, r3, #16
	cmp	r3, r2
	bne.n	.L_020036f4
	ldr	r3, [pc, #800]
	ldr	r0, [pc, #800]
	ldr	r1, [pc, #804]
	ldr	r2, [pc, #804]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x0200bde8
	adds	r5, r0, #0
	adds	r1, r5, #0
	ldr	r0, [pc, #792]
	bl 0x0200bdf8
	movs	r1, #128
	movs	r3, #0
	ldrsh	r0, [r7, r3]
	lsls	r1, r1, #2
	adds	r2, r5, #0
	bl 0x0200be10
	adds	r0, r5, #0
	bl 0x0200bdf0
.L_020036f4:
	movs	r1, #0
	ldrsh	r2, [r6, r1]
	cmp	r2, #0
	bne.n	.L_0200370a
	ldr	r3, [sp, #12]
	adds	r3, #216
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200be08
	b.n	.L_020039ca
.L_0200370a:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r7, r3, #0
	subs	r7, #8
	movs	r3, #255
	ands	r7, r3
	mov	r3, fp
	lsls	r3, r3, #4
	str	r3, [sp, #4]
	movs	r2, #128
	ldr	r1, [sp, #4]
	lsls	r2, r2, #8
	movs	r3, #104
	mov	r9, r2
	ldr	r2, [sp, #12]
	subs	r4, r3, r1
	movs	r3, #0
	stmia	r2!, {r3}
	lsls	r3, r4, #16
	adds	r1, r2, #0
	str	r1, [sp, #12]
	orrs	r3, r7
	mov	r1, r9
	orrs	r3, r1
	stmia	r2!, {r3}
	ldr	r3, [sp, #8]
	movs	r5, #228
	lsls	r5, r5, #8
	adds	r1, r2, #0
	orrs	r3, r5
	str	r1, [sp, #12]
	stmia	r2!, {r3}
	adds	r1, r2, #0
	str	r1, [sp, #12]
.L_02003750:
	mov	r0, r8
	movs	r2, #12
	movs	r1, #255
	movs	r6, #0
	add	r8, r2
	bl 0x0200be28
	cmp	r6, fp
	bcs.n	.L_020037a4
	ldr	r3, [sp, #8]
	movs	r1, #128
	adds	r3, #2
	orrs	r3, r5
	lsls	r1, r1, #23
	ldr	r5, [sp, #12]
	mov	r9, r1
	mov	sl, r3
.L_02003772:
	lsls	r2, r6, #4
	movs	r3, #96
	subs	r4, r3, r2
	movs	r3, #0
	str	r3, [r5, #0]
	lsls	r3, r4, #16
	mov	r2, r9
	orrs	r3, r7
	orrs	r3, r2
	str	r3, [r5, #4]
	mov	r3, sl
	str	r3, [r5, #8]
	ldr	r1, [sp, #12]
	adds	r1, #12
	str	r1, [sp, #12]
	mov	r0, r8
	movs	r2, #12
	movs	r1, #255
	adds	r6, #1
	add	r8, r2
	adds	r5, #12
	bl 0x0200be28
	cmp	r6, fp
	bcc.n	.L_02003772
.L_020037a4:
	ldr	r2, [sp, #12]
	movs	r6, #0
	movs	r3, #128
	stmia	r2!, {r6}
	lsls	r3, r3, #8
	mov	r9, r3
	movs	r3, #224
	adds	r1, r2, #0
	lsls	r3, r3, #15
	str	r1, [sp, #12]
	orrs	r3, r7
	mov	r1, r9
	orrs	r3, r1
	stmia	r2!, {r3}
	ldr	r5, [sp, #8]
	adds	r1, r2, #0
	movs	r2, #228
	lsls	r2, r2, #8
	adds	r5, #6
.L_020037ca:
	orrs	r5, r2
.L_020037cc:
	stmia	r1!, {r5}
	mov	r0, r8
	adds	r3, r1, #0
	mov	sl, r2
	movs	r1, #255
	movs	r2, #12
	add	r8, r2
	str	r3, [sp, #12]
	bl 0x0200be28
	ldr	r1, [sp, #12]
	stmia	r1!, {r6}
	adds	r3, r1, #0
	str	r3, [sp, #12]
	movs	r3, #240
	lsls	r3, r3, #15
	mov	r2, r9
	orrs	r3, r7
	orrs	r3, r2
	movs	r2, #128
	lsls	r2, r2, #21
	orrs	r3, r2
	stmia	r1!, {r3}
	adds	r2, r1, #0
	str	r2, [sp, #12]
	stmia	r1!, {r5}
	adds	r3, r1, #0
	movs	r1, #12
	mov	r0, r8
	add	r8, r1
	movs	r1, #255
	str	r3, [sp, #12]
	bl 0x0200be28
	cmp	r6, fp
	bcs.n	.L_02003866
	ldr	r4, [sp, #8]
	movs	r2, #128
	movs	r1, #128
	mov	r3, sl
	adds	r4, #2
	lsls	r2, r2, #23
	lsls	r1, r1, #16
	ldr	r5, [sp, #12]
	mov	r9, r2
	orrs	r4, r3
	mov	sl, r1
.L_0200382a:
	movs	r3, #0
	str	r3, [r5, #0]
	mov	r2, sl
	adds	r3, r7, #0
	orrs	r3, r2
	mov	r1, r9
	movs	r2, #128
	orrs	r3, r1
	lsls	r2, r2, #21
	orrs	r3, r2
	str	r3, [r5, #4]
	str	r4, [r5, #8]
	ldr	r2, [sp, #12]
	mov	r0, r8
	adds	r2, #12
	movs	r3, #12
	movs	r1, #255
	str	r4, [sp, #0]
	str	r2, [sp, #12]
	add	r8, r3
	bl 0x0200be28
	movs	r1, #128
	lsls	r1, r1, #13
	adds	r6, #1
	adds	r5, #12
	add	sl, r1
	ldr	r4, [sp, #0]
	cmp	r6, fp
	bcc.n	.L_0200382a
.L_02003866:
	movs	r2, #128
	ldr	r4, [sp, #4]
	lsls	r2, r2, #8
	mov	r9, r2
	ldr	r2, [sp, #12]
	movs	r3, #0
	adds	r4, #128
	stmia	r2!, {r3}
	mov	fp, r3
	lsls	r3, r4, #16
	orrs	r7, r3
	mov	r3, r9
	orrs	r7, r3
	movs	r3, #128
	lsls	r3, r3, #21
	adds	r1, r2, #0
	orrs	r7, r3
	str	r1, [sp, #12]
	stmia	r2!, {r7}
	ldr	r3, [sp, #8]
	adds	r1, r2, #0
	movs	r2, #228
	lsls	r2, r2, #8
	orrs	r3, r2
	mov	sl, r2
	adds	r2, r1, #0
	stmia	r2!, {r3}
	adds	r1, r2, #0
	movs	r3, #12
	str	r1, [sp, #12]
	mov	r0, r8
	movs	r1, #255
	add	r8, r3
	bl 0x0200be28
	ldr	r3, [pc, #328]
	ldr	r3, [r3, #0]
	movs	r2, #15
	ands	r3, r2
	cmp	r3, #4
	bhi.n	.L_020038ba
	b.n	.L_020039ca
.L_020038ba:
	ldr	r3, [sp, #16]
	movs	r1, #128
	adds	r3, #224
	lsls	r1, r1, #23
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	mov	r9, r1
	bl 0x0200c078
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02003948
	ldr	r3, [sp, #16]
	adds	r3, #232
	ldr	r3, [r3, #0]
	ldr	r0, [r6, #8]
	movs	r5, #224
	lsls	r5, r5, #12
	subs	r0, r0, r3
	adds	r1, r5, #0
	bl 0x0200bd98
	ldr	r3, [sp, #16]
	adds	r3, #236
	ldr	r3, [r3, #0]
	adds	r4, r0, #0
	ldr	r0, [r6, #16]
	adds	r4, #112
	adds	r1, r5, #0
	subs	r0, r0, r3
	str	r4, [sp, #0]
	bl 0x0200bd98
	ldr	r3, [sp, #16]
	adds	r3, #218
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldr	r1, [sp, #12]
	subs	r7, r0, #4
	movs	r3, #255
	ands	r7, r3
	mov	r3, fp
	stmia	r1!, {r3}
.L_02003918:
	ldr	r4, [sp, #0]
	adds	r2, r1, #0
	lsls	r3, r4, #16
	str	r2, [sp, #12]
	orrs	r7, r3
	mov	r2, r9
	orrs	r7, r2
	stmia	r1!, {r7}
	adds	r3, r1, #0
	str	r3, [sp, #12]
	ldr	r3, [sp, #8]
	mov	r1, sl
	adds	r3, #12
	orrs	r3, r1
	ldr	r1, [sp, #12]
	stmia	r1!, {r3}
	adds	r2, r1, #0
	str	r2, [sp, #12]
	mov	r0, r8
	movs	r2, #12
	movs	r1, #255
	add	r8, r2
	bl 0x0200be28
.L_02003948:
	ldr	r3, [sp, #16]
	adds	r3, #222
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl 0x0200c078
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_020039ca
	ldr	r3, [sp, #16]
	adds	r3, #232
	ldr	r3, [r3, #0]
	ldr	r0, [r6, #8]
	movs	r5, #224
	lsls	r5, r5, #12
	subs	r0, r0, r3
	adds	r1, r5, #0
.L_0200396a:
	bl 0x0200bd98
	ldr	r3, [sp, #16]
	adds	r3, #236
	ldr	r3, [r3, #0]
	adds	r4, r0, #0
	ldr	r0, [r6, #16]
	adds	r4, #112
	adds	r1, r5, #0
	subs	r0, r0, r3
	str	r4, [sp, #0]
	bl 0x0200bd98
	ldr	r3, [sp, #16]
	adds	r3, #218
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #1
.L_02003992:
	adds	r0, r0, r3
	ldr	r1, [sp, #12]
	subs	r7, r0, #4
	movs	r3, #255
	ands	r7, r3
	mov	r3, fp
	stmia	r1!, {r3}
	ldr	r4, [sp, #0]
	adds	r2, r1, #0
	lsls	r3, r4, #16
	str	r2, [sp, #12]
	orrs	r7, r3
	mov	r2, r9
	orrs	r7, r2
	stmia	r1!, {r7}
	adds	r3, r1, #0
	str	r3, [sp, #12]
	ldr	r3, [sp, #8]
	mov	r1, sl
	adds	r3, #8
	ldr	r2, [sp, #12]
	orrs	r3, r1
	str	r3, [r2, #0]
	mov	r3, r8
	adds	r0, r3, #0
	movs	r1, #255
	bl 0x0200be28
.L_020039ca:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f3c
	.4byte 0x03001b10
	.4byte 0x040000d4
	.4byte 0x0200c174
	.4byte 0x050003c0
	.4byte 0x80000010
	.4byte 0x0200c194
	.2byte 0x1e40
	.2byte 0x0300
	.section .text.x0200bae4,"ax",%progbits
	.balign 4
	.global Func_02003ae4
	.thumb_func
Func_02003ae4:
	push {r5, r6, lr}
	ldr r3, [pc, #60]
	ldr r6, [r3]
	ldr r5, [pc, #60]
	bl 0x0200be30
	adds r1, r6, #0
	adds r1, #240
	bl 0x0200bdf8
	ldr r0, [pc, #48]
	bl 0x0200bee8
	cmp r0, #0
	bne .L_02003ae4_0
	movs r3, #1
	strh r3, [r5]
	strh r3, [r5, #2]
	adds r3, r6, #0
	adds r3, #224
	ldrh r3, [r3]
	strh r0, [r5, #8]
	strh r3, [r5, #4]
	strh r0, [r5, #6]
.L_02003ae4_0:
	ldr r1, [pc, #24]
	ldr r0, [pc, #28]
	bl 0x0200bda8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001f3c
	.4byte 0x02001000
	.4byte 0x00000109
	.4byte 0x00000c85
	.4byte 0x0200b459
	.section .rodata,"a",%progbits
	.global KorosseoKabe_RollLogScript
KorosseoKabe_RollLogScript:
	.4byte 0x03030202
	.4byte 0x20202000
	.4byte 0x40404060
	.2byte 0x0080
	.global KorosseoKabe_ModeRecordTwo
KorosseoKabe_ModeRecordTwo:
	.2byte 0x1000
	.4byte 0x00010200
	.4byte 0x00002000
	.4byte 0x10000001
	.4byte 0x00060100
	.4byte 0x00102000
	.4byte 0x7fff0006
	.4byte 0x2000001e
	.4byte 0x001e0000
	.4byte 0x001e7fff
	.2byte 0xffff
	.global KorosseoKabe_ModeRecordThreeAlt
KorosseoKabe_ModeRecordThreeAlt:
	.2byte 0x1000
	.4byte 0x00010080
	.4byte 0x00002000
	.4byte 0x10000001
	.4byte 0x00060100
	.4byte 0x00102000
	.4byte 0x7fff0006
	.4byte 0x1000003c
	.4byte 0x00060180
	.4byte 0x00067fff
	.4byte 0x00f01000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060170
	.4byte 0x00067fff
	.4byte 0x00e01000
	.4byte 0x7fff0006
	.4byte 0x20000006
	.4byte 0x001e0000
	.4byte 0x01601000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000600d0
	.4byte 0x00067fff
	.4byte 0x01501000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000600c0
	.4byte 0x00067fff
	.4byte 0x01401000
	.4byte 0x7fff0006
	.4byte 0xffff0006
	.4byte 0x377f10a0
	.4byte 0x121722bb
	.4byte 0x0ccc1172
	.4byte 0x7df07ef7
	.4byte 0x610b7ce7
	.4byte 0x50007c00
	.4byte 0x00147fff
	.4byte 0x2f1f0000
	.global KorosseoKabe_MarkerGraphics
KorosseoKabe_MarkerGraphics:
	.4byte 0x82fc0100
	.4byte 0x70462310
	.4byte 0x201abddc
	.4byte 0x648ad0cc
	.4byte 0xa8a89c76
	.4byte 0x81984754
	.4byte 0x6138edda
	.4byte 0x5f28474a
	.4byte 0xa01027d3
	.4byte 0xa0abb823
	.4byte 0xf0214faa
	.4byte 0x5557fc29
	.4byte 0x1c29f456
	.4byte 0xc0a8882e
	.4byte 0xf029560c
	.4byte 0x39f9b811
	.4byte 0xa3e78fa8
	.4byte 0xd88e8df8
	.4byte 0x60101ddd
	.4byte 0x2101d56c
	.4byte 0xa68033e0
	.4byte 0x1ca188ea
	.4byte 0xb037924e
	.4byte 0x8e40d46f
	.4byte 0x56a22701
	.4byte 0x9957005d
	.4byte 0x6e51113b
	.4byte 0x9f8022e5
	.4byte 0x3a3ae095
	.4byte 0x39edc9ab
	.4byte 0x9ddeeaec
	.4byte 0x378733aa
	.4byte 0xbb23a0e8
	.4byte 0x7b4bb0f7
	.4byte 0x0596cc80
	.4byte 0xc075eccf
	.4byte 0x3fd9b0ef
	.4byte 0xcc032ec8
	.4byte 0x8d5aec81
	.4byte 0xee419f03
	.4byte 0x7c20f81f
	.4byte 0x38be6e09
	.4byte 0xbe2a7c41
	.4byte 0x07316774
	.4byte 0xf7104b82
	.4byte 0xf17c90f8
	.4byte 0x00000001
	.global HexDigits
HexDigits:
	.4byte 0x33323130
	.4byte 0x37363534
	.4byte 0x42413938
	.4byte 0x46454443
	.4byte 0x00000000
	.global KorosseoKabe_SparkScript
KorosseoKabe_SparkScript:
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xffffc000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x0000001b
	.global KorosseoKabe_RaiseScript
KorosseoKabe_RaiseScript:
	.4byte 0x00000015
	.4byte 0x00000026
	.4byte 0x0200b25d
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00016000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00016000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00012000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00012000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00016000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00016000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00012000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00012000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0xc0010000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000010
	.global KorosseoKabe_DirectionSteps
KorosseoKabe_DirectionSteps:
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
	.global gKorosseoKabeEntrances
gKorosseoKabeEntrances:
	.4byte 0xffff0001
	.4byte 0x000004f8
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000004c8
	.4byte 0xc00001e8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000180
	.4byte 0x40000108
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKorosseoKabeExits
gKorosseoKabeExits:
	.4byte 0x00000090
	.4byte 0x00a01090
	.4byte 0x00b4408c
	.4byte 0x0041f08c
	.4byte 0x0056208a
	.4byte 0x000001ff
	.global gKorosseoKabePlacements
gKorosseoKabePlacements:
	.4byte 0xffff0000
	.4byte 0x00000001
	.4byte 0x04f80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00004000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff003e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x04f80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x04c80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x04980000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00024000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0103
	.4byte 0x00000007
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x01020000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000007
	.4byte 0x03a80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000007
	.4byte 0x03c80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01024000
	.4byte 0xffff0122
	.4byte 0x00000007
	.4byte 0x03e80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01024000
	.4byte 0xffff0122
	.4byte 0x00000007
	.4byte 0x04080000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01024000
	.4byte 0xffff0122
	.4byte 0x00000007
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff0122
	.4byte 0x00000007
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff0122
	.4byte 0x00000007
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff0122
	.4byte 0x00000007
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff0103
	.4byte 0x00000007
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x04300000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00028000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x04c80000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00014000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x04b80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00014000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x03d80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00014000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00014000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00014000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00014000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global KorosseoKabe_SpectatorPhase
KorosseoKabe_SpectatorPhase:
	.4byte 0x00000000
	.global KorosseoKabe_SpectatorTimer
KorosseoKabe_SpectatorTimer:
	.4byte 0x00000000
	.global gKorosseoKabeEvents
gKorosseoKabeEvents:
	.4byte 0x00004602
	.4byte 0xffff001e
	.4byte 0x020082e9
	.4byte 0x00008602
	.4byte 0xffff001e
	.4byte 0x02008305
	.4byte 0x00008602
	.4byte 0xffff001c
	.4byte 0x020086c5
	.4byte 0x00000602
	.4byte 0xffff001b
	.4byte 0x020086d9
	.4byte 0x00008602
	.4byte 0xffff0018
	.4byte 0x0200869d
	.4byte 0x00000602
	.4byte 0xffff0019
	.4byte 0x020086b1
	.4byte 0x00000202
	.4byte 0xffff0016
	.4byte 0x02008401
	.4byte 0x00000202
	.4byte 0xffff0015
	.4byte 0x02008051
	.4byte 0x00000202
	.4byte 0xffff0014
	.4byte 0x020080b1
	.4byte 0x00000202
	.4byte 0xffff001a
	.4byte 0x020080b1
	.4byte 0x00000002
	.4byte 0x02100032
	.4byte 0x02008a1d
	.4byte 0x00000013
	.4byte 0x03380065
	.4byte 0x001000b5
	.4byte 0x00008413
	.4byte 0x03390066
	.4byte 0x001000e2
	.4byte 0x00000013
	.4byte 0x033b0068
	.4byte 0x001000b5
	.4byte 0x50001815
	.4byte 0x03310014
	.4byte 0x02008259
	.4byte 0x50001815
	.4byte 0x03320015
	.4byte 0x0200828d
	.4byte 0x10008e15
	.4byte 0x03300012
	.4byte 0x02008151
	.4byte 0x00008e15
	.4byte 0x03300012
	.4byte 0x02008161
	.4byte 0x00000c15
	.4byte 0x03330013
	.4byte 0x020082c1
	.4byte 0x00008c15
	.4byte 0xffff0009
	.4byte 0x0200804d
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte 0x0200804d
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte 0x0200805d
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte 0x020080c1
	.4byte 0x00008c15
	.4byte 0xffff000d
	.4byte 0x020080c1
	.4byte 0x00008c15
	.4byte 0xffff000e
	.4byte 0x020080c1
	.4byte 0x00009115
	.4byte 0x0334001f
	.4byte 0x020089b1
	.4byte 0x00000000
	.4byte 0xffff0022
	.4byte 0x02009f15
	.4byte 0x00000000
	.4byte 0xffff0023
	.4byte 0x020092f1
	.4byte 0x00000000
	.4byte 0xffff0024
	.4byte 0x02009425
	.4byte 0x00000000
	.4byte 0xffff0025
	.4byte 0x02009539
	.4byte 0x00000000
	.4byte 0xffff0026
	.4byte 0x020096d5
	.4byte 0x00000000
	.4byte 0xffff0027
	.4byte 0x020099bd
	.4byte 0x00008d15
	.4byte 0xffff0022
	.4byte 0x0000213c
	.4byte 0x00008d15
	.4byte 0xffff0023
	.4byte 0x00002141
	.4byte 0x00008d15
	.4byte 0xffff0024
	.4byte 0x00002142
	.4byte 0x00008d15
	.4byte 0xffff0025
	.4byte 0x00002143
	.4byte 0x00008d15
	.4byte 0xffff0026
	.4byte 0x00002144
	.4byte 0x00008d15
	.4byte 0xffff0027
	.4byte 0x00002145
	.4byte 0x00000006
	.4byte 0xffff0063
	.4byte 0x02009cc1
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.2byte 0xffff
	.global KorosseoKabe_ModeRecordDefault
KorosseoKabe_ModeRecordDefault:
	.2byte 0x4000
	.4byte 0x0800ff44
	.4byte 0x01801000
	.4byte 0x20000001
	.4byte 0x00010010
	.4byte 0x000e7fff
	.4byte 0x00003000
	.4byte 0x7fff0014
	.4byte 0x3000003c
	.4byte 0x00140800
	.4byte 0x003c7fff
	.2byte 0xffff
	.global KorosseoKabe_ModeRecordFour
KorosseoKabe_ModeRecordFour:
	.2byte 0x1000
	.4byte 0x00010200
	.4byte 0x00002000
	.4byte 0x10000001
	.4byte 0x00060100
	.4byte 0x00102000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060180
	.4byte 0x00067fff
	.4byte 0x01001000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060180
	.4byte 0x00067fff
	.4byte 0x01001000
	.4byte 0x7fff0006
	.4byte 0x20000006
	.4byte 0x001e0000
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060100
	.4byte 0x00067fff
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060100
	.4byte 0x00067fff
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0xffff0006
	.global KorosseoKabe_ModeRecordThree
KorosseoKabe_ModeRecordThree:
	.4byte 0x00801000
	.4byte 0x20000001
	.4byte 0x00010000
	.4byte 0x01001000
	.4byte 0x20000006
	.4byte 0x00060010
	.4byte 0x003c7fff
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060110
	.4byte 0x00067fff
	.4byte 0x01901000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060120
	.4byte 0x00067fff
	.4byte 0x00002000
	.4byte 0x1000001e
	.4byte 0x000601a0
	.4byte 0x00067fff
	.4byte 0x01301000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000601b0
	.4byte 0x00067fff
	.4byte 0x01401000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000601c0
	.4byte 0x00067fff
	.2byte 0xffff
	.global KorosseoKabe_CursorSlot
KorosseoKabe_CursorSlot:
	.2byte 0xffff
	.global Korosseo_RivalFinishScript
Korosseo_RivalFinishScript:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000022
	.4byte 0x0200b2f1
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x0000001c
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000096
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KorosseoKabe_ApproachScript
KorosseoKabe_ApproachScript:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000022
	.4byte 0x0200b2f1
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x0000001c
	.4byte 0x0000001c
	.4byte 0x00000000
	.4byte 0x00000096
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.section .bss,"aw",%nobits
	.global KorosseoKabe_MarkerElapsed
KorosseoKabe_MarkerElapsed:
	.space 8
	.global KorosseoKabe_MarkerStyle
KorosseoKabe_MarkerStyle:
	.space 4
	.global Korosseo_CompetitorStartZ
Korosseo_CompetitorStartZ:
	.space 4
	.global KorosseoKabe_MarkerTargetX
KorosseoKabe_MarkerTargetX:
	.space 4
	.global KorosseoKabe_ModeTaskParam
KorosseoKabe_ModeTaskParam:
	.space 8
	.global KorosseoKabe_ModeTaskCount
KorosseoKabe_ModeTaskCount:
	.space 4
	.global KorosseoKabe_ModeTaskWord
KorosseoKabe_ModeTaskWord:
	.space 4
	.global KorosseoKabe_MarkerFrame
KorosseoKabe_MarkerFrame:
	.space 12
	.global KorosseoKabe_MarkerY
KorosseoKabe_MarkerY:
	.space 8
	.global Korosseo_CompetitorStartAngle
Korosseo_CompetitorStartAngle:
	.space 4
	.global KorosseoKabe_MarkerDuration
KorosseoKabe_MarkerDuration:
	.space 4
	.global KorosseoKabe_ModeTaskMode
KorosseoKabe_ModeTaskMode:
	.space 12
	.global KorosseoKabe_ModeTaskTimer
KorosseoKabe_ModeTaskTimer:
	.space 4
	.global KorosseoKabe_ModeTaskHandler
KorosseoKabe_ModeTaskHandler:
	.space 4
	.global KorosseoKabe_MarkerStartX
KorosseoKabe_MarkerStartX:
	.space 24
	.global KorosseoKabe_MarkerStartY
KorosseoKabe_MarkerStartY:
	.space 56
	.global KorosseoKabe_MarkerX
KorosseoKabe_MarkerX:
	.space 4
	.global KorosseoKabe_ModeTaskStep
KorosseoKabe_ModeTaskStep:
	.space 8
	.global KorosseoKabe_MarkerTargetY
KorosseoKabe_MarkerTargetY:
	.space 4
	.global Korosseo_CompetitorStartX
Korosseo_CompetitorStartX:
	.space 4

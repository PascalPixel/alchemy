.syntax unified
	.thumb
	.global Func_0803bb58
	.thumb_func
Func_0803bb58:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #108
	str r2, [sp, #20]
	str r1, [sp, #24]
	mov r8, r3
	movs r3, #192
	lsls r3, r3, #18
	add r6, sp, #44
	ldr r1, [r3, #60]
	movs r3, #15
	str r3, [r6]
	str r3, [r6, #4]
	str r3, [r6, #8]
	str r3, [r6, #12]
	str r3, [r6, #16]
	str r3, [r6, #20]
	str r3, [r6, #24]
	str r3, [r6, #28]
	str r3, [r6, #32]
	str r3, [r6, #36]
	str r3, [r6, #40]
	str r3, [r6, #44]
	str r3, [r6, #48]
	str r3, [r6, #52]
	str r3, [r6, #56]
	str r3, [r6, #60]
	ldr r3, .L_0803bd9c
	movs r2, #0
	mov r12, r3
	mov r3, sp
	adds r3, #28
	mov r9, r2
	str r2, [sp, #16]
	str r3, [sp, #12]
	movs r2, #36
	movs r4, #0
	add r2, sp
	adds r5, r0, #0
	movs r7, #0
	movs r0, #0
	mov r11, r2
	mov r10, r4
.L_0803bbb8:
	movs r2, #244
	lsls r3, r5, #1
	lsls r2, r2, #4
	adds r3, r3, r2
	ldrh r2, [r1, r3]
	movs r3, #128
	lsls r3, r3, #1
	adds r5, #1
	adds r3, #255
	ands r5, r3
	cmp r2, #31
	bls .L_0803bbfc
	cmp r2, #176
	beq .L_0803bbfc
	cmp r2, #32
	bne .L_0803bbde
	adds r7, #5
	adds r0, #1
	b .L_0803bbb8
.L_0803bbde:
	ldr r3, .L_0803bda0
	subs r2, #32
	lsls r2, r2, #5
	ldrh r2, [r3, r2]
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #60
	ldrh r3, [r3, r1]
	cmp r3, #1
	beq .L_0803bbf6
	cmp r3, #5
	bne .L_0803bbf8
.L_0803bbf6:
	adds r2, #1
.L_0803bbf8:
	adds r7, r7, r2
	b .L_0803bbb8
.L_0803bbfc:
	cmp r2, #28
	bhi .L_0803bbb8
	lsls r3, r2, #2
	mov r2, r12
	ldr r3, [r3, r2]
	mov pc, r3
.L_0803bc08:
	.2byte 0xbcb2
	.2byte 0x0803
	.2byte 0xbccc
	.2byte 0x0803
	.2byte 0xbbb8
	.2byte 0x0803
	.2byte 0xbc7c
	.2byte 0x0803
	.2byte 0xbbb8
	.2byte 0x0803
	.2byte 0xbbb8
	.2byte 0x0803
	.2byte 0xbbb8
	.2byte 0x0803
	.2byte 0xbbb8
	.2byte 0x0803
	.2byte 0xbd04
	.2byte 0x0803
	.2byte 0xbd10
	.2byte 0x0803
	.2byte 0xbd04
	.2byte 0x0803
	.2byte 0xbbb8
	.2byte 0x0803
	.2byte 0xbbb8
	.2byte 0x0803
	.2byte 0xbbb8
	.2byte 0x0803
	.2byte 0xbcfa
	.2byte 0x0803
	.2byte 0xbd04
	.2byte 0x0803
	.2byte 0xbbb8
	.2byte 0x0803
	.2byte 0xbd04
	.2byte 0x0803
	.2byte 0xbbb8
	.2byte 0x0803
	.2byte 0xbbb8
	.2byte 0x0803
	.2byte 0xbbb8
	.2byte 0x0803
	.2byte 0xbbb8
	.2byte 0x0803
	.2byte 0xbbb8
	.2byte 0x0803
	.2byte 0xbbb8
	.2byte 0x0803
	.2byte 0xbbb8
	.2byte 0x0803
	.2byte 0xbbb8
	.2byte 0x0803
	.2byte 0xbbb8
	.2byte 0x0803
	.2byte 0xbbb8
	.2byte 0x0803
	.2byte 0xbcfa
	.2byte 0x0803
	.2byte 0x465b
	.2byte 0x4652
	.2byte 0x3001
	.2byte 0x5298
	.2byte 0x9b03
	.2byte 0x529f
	.2byte 0x2c00
	.2byte 0xd102
	.2byte 0x45b9
	.2byte 0xd200
	.2byte 0x46b9
	.2byte 0x9b04
	.2byte 0x2b02
	.2byte 0xd803
	.2byte 0x3301
	.2byte 0x9304
	.2byte 0x005b
	.2byte 0x469a
	.2byte 0x00a2
	.2byte 0x58b3
	.2byte 0x2000
	.2byte 0x330f
	.2byte 0x50b3
	.2byte 0x4a3c
	.2byte 0x2700
	.2byte 0x4694
	.2byte 0xe782
	.2byte 0x465b
	.2byte 0x4652
	.2byte 0x3001
	.2byte 0x5298
	.2byte 0x9b03
	.2byte 0x529f
	.2byte 0x2c00
	.2byte 0xd102
	.2byte 0x45b9
	.2byte 0xd200
	.2byte 0x46b9
	.2byte 0x3401
	.2byte 0xe033
	.2byte 0x465b
	.2byte 0x4652
	.2byte 0x3001
	.2byte 0x5298
	.2byte 0x9b03
	.2byte 0x529f
	.2byte 0x2c00
	.2byte 0xd102
	.2byte 0x45b9
	.2byte 0xd200
	.2byte 0x46b9
	.2byte 0x3401
	.2byte 0x9002
	.2byte 0x9101
	.2byte 0x9400
	.2byte 0xf7fe
	.2byte 0xfb8b
	.2byte 0x4b2b
	.2byte 0x9802
	.2byte 0x469c
	.2byte 0x9901
	.2byte 0x9c00
	.2byte 0xe75e
	.2byte 0x2380
	.2byte 0x005b
	.2byte 0x3501
	.2byte 0x33ff
	.2byte 0x401d
	.2byte 0x2380
	.2byte 0x005b
	.2byte 0x3501
	.2byte 0x33ff
	.2byte 0x401d
	.2byte 0xe753
	.2byte 0x22f4
	.2byte 0x006b
	.2byte 0x0112
	.2byte 0x189b
	.2byte 0x5aca
	.2byte 0x23f0
	.2byte 0x011b
	.2byte 0x333c
	.2byte 0x185b
	.2byte 0x801a
	.2byte 0x2380
	.2byte 0x4a1d
	.2byte 0x005b
	.2byte 0x3501
	.2byte 0x33ff
	.2byte 0x401d
	.2byte 0x4694
	.2byte 0xe741
	.2byte 0x790b
	.2byte 0x2b00
	.2byte 0xd001
	.2byte 0x2302
	.2byte 0x4499
	.2byte 0x2000
	.2byte 0x42a0
	.2byte 0xd212
	.2byte 0x1c31
	.2byte 0x1c0d
	.2byte 0x2800
	.2byte 0xd103
	.2byte 0x682b
	.2byte 0x9a05
	.2byte 0x6013
	.2byte 0xe006
	.2byte 0x9a05
	.2byte 0x6813
	.2byte 0x680a
	.2byte 0x4293
	.2byte 0xd201
	.2byte 0x9b05
	.2byte 0x601a
	.2byte 0x3001
	.2byte 0x3104
	.2byte 0x42a0
	.2byte 0xd3ee
	.2byte 0x9b06
	.2byte 0x464a
	.2byte 0x601a
	.2byte 0x464b
	.2byte 0x3313
	.2byte 0x08db
	.2byte 0x00db
	.2byte 0x3b10
	.2byte 0x4642
	.2byte 0x4699
	.2byte 0x2a00
	.2byte 0xd027
	.2byte 0x2600
	.2byte 0x2500
	.global Func_0803bd86
	.thumb_func
Func_0803bd86:
	.2byte 0x465a
	.2byte 0x5aab
	.2byte 0x2b01
	.2byte 0xd80a
	.2byte 0x4b02
	.2byte 0x4642
	.2byte 0x8013
	.2byte 0xe016
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_0803bd9c:
	.4byte .L_0803bc08
.L_0803bda0:
	.4byte UiText_Glyphs

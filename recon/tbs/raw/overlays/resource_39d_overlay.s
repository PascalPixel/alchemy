.syntax unified
	.thumb
	.section .text.x0200813c,"ax",%progbits
	.balign 4
	.global Effect_Spawn
	.thumb_func
Effect_Spawn:
	.global Func_0200013c
	.thumb_func
Func_0200013c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	adds r6, r1, #0
	ldr r1, [sp, #48]
	adds r5, r0, #0
	movs r0, #0
	mov r8, r2
	str r3, [sp, #4]
	mov r10, r1
	ldr r7, [sp, #52]
	bl 0x0200b6f0
	movs r3, #128
	lsls r3, r3, #13
	mov r2, r10
	ands r3, r2
	mov r9, r0
	cmp r3, #0
	beq .L_0200013c_0
	cmp r7, #0
	beq .L_0200013c_0
	movs r3, #24
	ldrsh r0, [r7, r3]
	adds r2, r6, #0
	b .L_0200013c_1
.L_0200013c_0:
	adds r2, r6, #0
	movs r0, #222
.L_0200013c_1:
	adds r1, r5, #0
	mov r3, r8
	bl 0x0200b670
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200013c_2
	b .L_0200013c_3
.L_0200013c_2:
	ldr r1, [r6, #80]
	mov r8, r1
	mov r1, r10
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	bl 0x0200b660
	mov r3, r10
	ldr r2, [pc, #356]
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl 0x0200b668
	adds r3, r6, #0
	movs r0, #0
	adds r3, #85
	strb r0, [r3]
	mov r3, r8
	adds r3, #38
	strb r0, [r3]
	ldr r3, [pc, #328]
	str r3, [r6, #108]
	ldr r3, [sp, #4]
	str r3, [r6, #68]
	ldr r3, [sp, #40]
	str r3, [r6, #72]
	ldr r3, [sp, #44]
	mov r1, r9
	str r3, [r6, #76]
	ldr r3, [r1, #80]
	ldrb r3, [r3, #9]
	movs r2, #12
	ands r2, r3
	mov r3, r8
	ldrb r1, [r3, #9]
	movs r3, #13
	negs r3, r3
	mov r9, r3
	ands r3, r1
	orrs r3, r2
	adds r2, r6, #0
	mov r1, r8
	adds r2, #100
	strb r3, [r1, #9]
	adds r3, r2, #0
	str r0, [r6, #48]
	str r0, [r6, #52]
	str r2, [sp, #0]
	strh r0, [r3]
	ldr r3, [pc, #276]
	mov r1, r10
	ands r3, r1
	movs r5, #3
	cmp r3, #0
	beq .L_0200013c_3
	cmp r7, #0
	beq .L_0200013c_3
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_0200013c_4
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl 0x0200b760
.L_0200013c_4:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200013c_5
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	mov r3, r8
	ldrb r2, [r7]
	ldrb r1, [r3, #9]
	ands r2, r5
	mov r3, r9
	ands r3, r1
	lsls r2, r2, #2
	orrs r3, r2
	mov r1, r8
	strb r3, [r1, #9]
.L_0200013c_5:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r10
	ands r2, r3
	cmp r2, #0
	beq .L_0200013c_6
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_0200013c_6:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_0200013c_7
	ldr r3, [pc, #156]
	mov r1, r11
	ldr r5, [r3, r1]
	cmp r2, #0
	beq .L_0200013c_8
	ldr r0, [r7, #16]
	ldr r3, [r6, #24]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	bl 0x0200b628
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_0200013c_9
.L_0200013c_8:
	ldr r0, [r7, #16]
	ldr r2, [pc, #128]
	ldr r1, [r5, #12]
	adds r0, r0, r2
	bl 0x0200b628
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [pc, #116]
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200013c_9:
	bl 0x0200b628
	str r0, [r6, #52]
.L_0200013c_7:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_0200013c_10
	adds r0, r6, #0
	movs r1, #1
	bl 0x0200b660
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl 0x0200b668
.L_0200013c_10:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200013c_11
	ldrh r3, [r7, #32]
	mov r1, r8
	strh r3, [r1, #30]
.L_0200013c_11:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200013c_12
	ldrh r3, [r7, #34]
	ldr r1, [sp, #0]
	strh r3, [r1]
.L_0200013c_12:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200013c_3
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_0200013c_3:
	sub sp, #-8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200b92c
	.4byte 0x02008105
	.4byte 0xffff0000
	.section .text.x0200b4bc,"ax",%progbits
	.balign 4
	.global Func_020034bc
	.thumb_func
Func_020034bc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #8
	bl 0x0200b6f0
	ldr r3, [pc, #100]
	mov r10, r0
	ldr r5, [r3]
	bl 0x0200b640
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #4
	lsrs r3, r3, #16
	lsls r3, r3, #16
	adds r5, #232
	mov r8, r3
	movs r0, #2
	ldrsh r3, [r5, r0]
	cmp r3, #129
	bgt .L_020034bc_0
	ldr r3, [pc, #72]
	ldr r3, [r3]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_020034bc_1
	movs r1, #152
	movs r2, #144
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #8
	bl 0x0200b728
	movs r0, #8
	bl 0x0200b6f0
	movs r5, #128
	lsls r5, r5, #9
	b .L_020034bc_2
.L_020034bc_1:
	movs r1, #152
	movs r2, #151
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #8
	bl 0x0200b728
	movs r0, #8
	bl 0x0200b6f0
	ldr r5, [pc, #20]
.L_020034bc_2:
	str r5, [r0, #24]
	movs r0, #8
	bl 0x0200b6f0
	str r5, [r0, #28]
	b .L_020034bc_3
	.4byte 0x03001e70
	.4byte 0x03001e40
	.4byte 0x00014ccc
.L_020034bc_0:
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #12
	lsls r2, r2, #12
	bl 0x0200b728
.L_020034bc_3:
	mov r1, r10
	cmp r1, #0
	beq .L_020034bc_4
	ldr r3, [pc, #160]
	ldr r6, [r3]
	movs r3, #15
	ands r6, r3
	cmp r6, #0
	bne .L_020034bc_4
	mov r0, r10
	ldr r2, [r0, #12]
	ldr r1, [r1, #8]
	movs r3, #128
	lsls r3, r3, #12
	add r2, r8
	adds r1, r1, r3
	adds r2, r2, r3
	ldr r3, [r0, #16]
	movs r0, #142
	lsls r0, r0, #1
	bl 0x0200b670
	movs r1, #192
	lsls r1, r1, #11
	adds r7, r0, #0
	mov r0, r8
	bl 0x0200b628
	mov r8, r0
	mov r1, r8
	lsls r1, r1, #16
	mov r8, r1
	cmp r7, #0
	beq .L_020034bc_4
	ldr r1, [pc, #104]
	adds r0, r7, #0
	ldr r5, [r7, #80]
	bl 0x0200b668
	movs r1, #3
	adds r0, r7, #0
	bl 0x0200b760
	adds r3, r7, #0
	adds r3, #85
	strb r6, [r3]
	bl 0x0200b640
	ldr r3, [pc, #80]
	adds r2, r7, #0
	ands r3, r0
	adds r2, #100
	ldr r0, [pc, #60]
	strh r3, [r2]
	adds r3, r7, #0
	mov r9, r0
	adds r3, #102
	ldr r0, [pc, #64]
	strh r6, [r3]
	mov r2, r8
	ldr r3, [pc, #64]
	mov r1, r10
	ands r0, r2
	str r1, [r7, #104]
	str r3, [r7, #108]
	asrs r0, r0, #4
	bl 0x0200b648
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #3
	asrs r3, r3, #16
	str r3, [r7, #48]
	adds r3, r5, #0
	adds r3, #38
	mov r0, r9
	strb r0, [r3]
	mov r1, r10
	ldr r3, [r1, #80]
	ldrb r3, [r3, #9]
	movs r2, #12
	ands r2, r3
	b .L_020034bc_5
	.4byte 0x00000000
	.4byte 0x03001e40
	.4byte 0x0200bc54
	.4byte 0x0ffff000
	.4byte 0x000fffff
	.4byte 0x0200b461
.L_020034bc_5:
	ldrb r1, [r5, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r5, #9]
.L_020034bc_4:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
@ The compiler library links here from its licensed container.
	.section .text.x02008890,"ax",%progbits
	.balign 4
	.global MeasureFixedPointPositionDistance
	.thumb_func
MeasureFixedPointPositionDistance:
	push {r5, lr}
	ldmia r0!, {r5}
	ldmia r1!, {r3}
	ldmia r0!, {r4}
	subs r5, r5, r3
	ldmia r1!, {r3}
	ldr r2, [r1]
	subs r4, r4, r3
	ldr r3, [r0]
	subs r3, r3, r2
	asrs r5, r5, #16
	asrs r4, r4, #16
	asrs r3, r3, #16
	adds r0, r5, #0
	muls r0, r5
	adds r2, r4, #0
	muls r2, r4
	adds r1, r3, #0
	muls r1, r3
	adds r0, r0, r2
	adds r3, r1, #0
	adds r0, r0, r3
	ldr r3, [pc, #8]
	bl 0x0200b854
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x030001d8
	.section .rodata.part1,"a",%progbits
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
	.4byte 0x0200b884
	.4byte 0x0200b8bc
	.4byte 0x0200b8f4
	.global MakyuriChojo_ScriptTable
MakyuriChojo_ScriptTable:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000008
	.4byte 0x40000008
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x000001a8
	.4byte 0xffff0002
	.4byte 0x00000008
	.4byte 0x40000008
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x000001a8
	.4byte 0xffff0005
	.4byte 0x000001f8
	.4byte 0x40000098
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x000001a8
	.4byte 0xffff0033
	.4byte 0x000001f8
	.4byte 0x400000a8
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x000001a8
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global MakyuriChojo_MessageTable
MakyuriChojo_MessageTable:
	.4byte 0x0000003a
	.4byte 0x0010f039
	.4byte 0x000001ff
	.global MakyuriChojo_ActorTable
MakyuriChojo_ActorTable:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00026000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00022000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x02530005
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00020000
	.4byte 0xffff00f4
	.4byte 0x00000007
	.4byte 0x01300000
	.4byte 0x00280000
	.4byte 0x00900000
	.4byte 0x00024000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00024000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x01024000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01024000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01024000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0x0253001e
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0x02530023
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00020000
	.4byte 0xffff0020
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x02530021
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00020000
	.4byte 0xffff002b
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global MakyuriChojo_EventTable
MakyuriChojo_EventTable:
	.4byte 0x00000002
	.4byte 0x08800005
	.4byte 0x02008b25
	.4byte 0x00000002
	.4byte 0x02510006
	.4byte 0x0200addd
	.4byte 0x00000202
	.4byte 0xffff000b
	.4byte 0x02008929
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x02008ad1
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte 0x02008929
	.4byte 0x00000002
	.4byte 0x0250000a
	.4byte 0x020089fd
	.4byte 0x00000003
	.4byte 0x03500064
	.4byte 0x00300000
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte 0x0200aeb9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global MakyuriChojo_NearestActor
MakyuriChojo_NearestActor:
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00016666
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00016666
	.4byte 0x0000001c
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000001b

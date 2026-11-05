.syntax unified
	.thumb
	.section .text.x0200815a,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008588,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.balign 4
	.global Func_02000588
	.thumb_func
Func_02000588:
	.ifdef TLA_EDITION_JA
	push {r5, r6, r7, lr}
	ldr r3, .L_ja_020086e0
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6, #0]
	bl Actor_GetById
	adds r5, r0, #0
	ldr r3, [r5, #16]
	movs r7, #152
	asrs r3, r3, #20
	lsls r7, r7, #17
	cmp r3, #9
	bgt .L_ja_020085a8
	negs r7, r7
.L_ja_020085a8:
	movs r0, #0
	bl Func_02000d44
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_020085bc
	b .L_ja_020086de
.L_ja_020085bc:
	adds r3, r5, #0
	adds r3, #34
	ldrb r0, [r3, #0]
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	bl Func_02000c8c
	cmp r0, #77
	beq .L_ja_020085d0
	b .L_ja_020086de
.L_ja_020085d0:
	ldr r0, [r6, #0]
	movs r1, #1
	bl Actor_SetModeById
	ldr r0, [r6, #0]
	cmp r7, #0
	ble .L_ja_020085ea
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	b .L_ja_020085f4
.L_ja_020085ea:
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
.L_ja_020085f4:
	movs r0, #10
	bl wait
	movs r6, #0
.L_ja_020085fc:
	ldr r3, .L_ja_020086e4
	ldrsb r0, [r3, r6]
	bl Actor_GetById
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2, #0]
	movs r2, #128
	ldr r3, [r5, #12]
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r5, #12]
	movs r3, #128
	lsls r3, r3, #5
	str r3, [r5, #52]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r1, #1
	bl Actor_SetSingleChildFlags
	ldr r1, [r5, #8]
	ldr r2, .L_ja_020086e8
	ldr r3, [r5, #16]
	adds r0, r5, #0
	bl Func_02000c74
	ldr r3, .L_ja_020086ec
	movs r0, #152
	str r3, [r5, #108]
	bl Func_02000d64
	bl Random16Far
	lsls r0, r0, #2
	lsrs r0, r0, #16
	adds r0, #3
	adds r6, #1
	bl wait
	cmp r6, #6
	ble .L_ja_020085fc
	movs r0, #40
	bl wait
	movs r0, #198
	bl Func_02000d64
	movs r6, #9
.L_ja_02008662:
	adds r0, r6, #0
	bl Actor_GetById
	adds r5, r0, #0
	ldr r3, [r5, #16]
	ldr r1, [r5, #8]
	subs r3, r3, r7
	ldr r2, .L_ja_020086e8
	adds r6, #1
	bl Func_02000c74
	cmp r6, #14
	ble .L_ja_02008662
	movs r0, #9
	bl Actor_GetById
	bl Func_02000c7c
	movs r0, #40
	bl wait
	movs r6, #9
.L_ja_0200868e:
	adds r0, r6, #0
	bl Actor_GetById
	adds r5, r0, #0
	ldr r3, [r5, #16]
	ldr r2, .L_ja_020086f0
	adds r1, r5, #0
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r5, #16]
	adds r1, #85
	movs r2, #0
	movs r3, #3
	adds r6, #1
	strb r3, [r1, #0]
	str r2, [r5, #108]
	cmp r6, #14
	ble .L_ja_0200868e
	movs r0, #3
	bl wait
	movs r0, #188
	bl Func_02000d64
	movs r0, #10
	bl wait
	movs r0, #154
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_020086de
	cmp r7, #0
	ble .L_ja_020086de
	bl Func_02000388
.L_ja_020086de:
	pop {r5, r6, r7, pc}
.L_ja_020086e0:
	.4byte gMainRoutineWork
.L_ja_020086e4:
	.4byte gData_02000eec
.L_ja_020086e8:
	.4byte 0xfff80000
.L_ja_020086ec:
	.4byte Func_020004b4
.L_ja_020086f0:
	.4byte 0xfff00000
	bx lr
	movs r0, r0
	.else
	push {r5, r6, r7, lr}
	ldr r3, .L_020086fc
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	bl Actor_GetById
	adds r5, r0, #0
	ldr r3, [r5, #16]
	movs r7, #152
	asrs r3, r3, #20
	lsls r7, r7, #17
	cmp r3, #9
	bgt .L_020085a8
	negs r7, r7
.L_020085a8:
	movs r0, #0
	bl Func_02000d44
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020085bc
	b .L_020086f8
.L_020085bc:
	adds r3, r5, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	bl Func_02000c8c
	cmp r0, #77
	beq .L_020085d0
	b .L_020086f8
.L_020085d0:
	ldr r0, [r6]
	movs r1, #1
	bl Actor_SetModeById
	ldr r2, .L_02008700
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	movs r0, #128
	lsls r0, r0, #12
	ands r1, r2
	ands r3, r2
	adds r1, r1, r0
	adds r3, r3, r0
	ldr r2, [r5, #12]
	adds r0, r5, #0
	bl Func_02000c74
	ldr r0, [r6]
	cmp r7, #0
	ble .L_02008604
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	b .L_0200860e
.L_02008604:
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
.L_0200860e:
	movs r0, #10
	bl wait
	movs r6, #0
.L_02008616:
	ldr r3, .L_02008704
	ldrsb r0, [r3, r6]
	bl Actor_GetById
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r2, #128
	ldr r3, [r5, #12]
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r5, #12]
	movs r3, #128
	lsls r3, r3, #5
	str r3, [r5, #52]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r1, #1
	bl Actor_SetSingleChildFlags
	ldr r1, [r5, #8]
	ldr r2, .L_02008708
	ldr r3, [r5, #16]
	adds r0, r5, #0
	bl Func_02000c74
	ldr r3, .L_0200870c
	movs r0, #152
	str r3, [r5, #108]
	bl Func_02000d64
	bl Random16Far
	lsls r0, r0, #2
	lsrs r0, r0, #16
	adds r0, #3
	adds r6, #1
	bl wait
	cmp r6, #6
	ble .L_02008616
	movs r0, #40
	bl wait
	movs r0, #198
	bl Func_02000d64
	movs r6, #9
.L_0200867c:
	adds r0, r6, #0
	bl Actor_GetById
	adds r5, r0, #0
	ldr r3, [r5, #16]
	ldr r1, [r5, #8]
	subs r3, r3, r7
	ldr r2, .L_02008708
	adds r6, #1
	bl Func_02000c74
	cmp r6, #14
	ble .L_0200867c
	movs r0, #9
	bl Actor_GetById
	bl Func_02000c7c
	movs r0, #40
	bl wait
	movs r6, #9
.L_020086a8:
	adds r0, r6, #0
	bl Actor_GetById
	adds r5, r0, #0
	ldr r3, [r5, #16]
	ldr r2, .L_02008700
	adds r1, r5, #0
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r5, #16]
	adds r1, #85
	movs r2, #0
	movs r3, #3
	adds r6, #1
	strb r3, [r1]
	str r2, [r5, #108]
	cmp r6, #14
	ble .L_020086a8
	movs r0, #3
	bl wait
	movs r0, #188
	bl Func_02000d64
	movs r0, #10
	bl wait
	movs r0, #154
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020086f8
	cmp r7, #0
	ble .L_020086f8
	bl Func_02000388
.L_020086f8:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020086fc:
	.4byte gMainRoutineWork
.L_02008700:
	.4byte 0xfff00000
.L_02008704:
	.4byte gData_02000eec
.L_02008708:
	.4byte 0xfff80000
.L_0200870c:
	.4byte Func_020004b4
	.4byte 0x00004770
	.endif
	.endif
	.section .bss,"aw",%nobits
	.global gData_02001cdc
gData_02001cdc:
	.space 0x00000002
	.global gData_02001cde
gData_02001cde:
	.space 0x00000002
	.global gData_02001ce0
gData_02001ce0:
	.space 0x00000040
	.global gData_02001d20
gData_02001d20:
	.space 0x000000c0
	.global gData_02001de0
gData_02001de0:
	.space 0x00000002
	.global gData_02001de2
gData_02001de2:
	.space 0x00000002
	.global gData_02001de4
gData_02001de4:

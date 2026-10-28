.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200815a,"ax",%progbits
	.2byte 0x0000
	.section .text.x020083e8,"ax",%progbits
	.global Func_020003e8
	.thumb_func
Func_020003e8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r5, #0
	ldrsh r3, [r3, r5]
	cmp r3, #0
	beq .L_02008404
	b .L_0200851a
.L_02008404:
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #164
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	mov r8, r3
	cmp r3, #0
	beq .L_0200841a
	b .L_0200851a
.L_0200841a:
	ldr r7, .L_02008520
	movs r2, #1
	ldr r3, [r7]
	negs r2, r2
	cmp r3, r2
	bne .L_02008460
	ldr r6, .L_02008524
	movs r1, #160
	lsls r1, r1, #19
	ldr r5, .L_02008528
	adds r0, r6, #0
	adds r1, #96
	movs r2, #32
	mov lr, r5
	.2byte 0xf800
	adds r1, r6, #0
	movs r2, #32
	ldr r0, .L_0200852c
	mov lr, r5
	.2byte 0xf800
	ldr r6, .L_02008530
	movs r1, #160
	lsls r1, r1, #19
	adds r1, #128
	movs r2, #32
	adds r0, r6, #0
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_02008534
	adds r1, r6, #0
	movs r2, #32
	mov lr, r5
	.2byte 0xf800
	mov r3, r8
	str r3, [r7]
.L_02008460:
	ldr r3, [r7]
	cmp r3, #0
	bge .L_02008468
	adds r3, #7
.L_02008468:
	asrs r2, r3, #3
	adds r3, r2, #0
	cmp r2, #0
	bge .L_02008472
	adds r3, r2, #3
.L_02008472:
	asrs r1, r3, #2
	lsls r3, r1, #2
	subs r1, r2, r3
	ldr r5, .L_02008534
	ldr r2, .L_02008530
	ldr r7, .L_0200852c
	ldr r6, .L_02008524
	movs r4, #0
	mov r12, r5
	mov r8, r2
	movs r0, #24
.L_02008488:
	adds r2, r1, r4
	adds r3, r2, #0
	cmp r2, #0
	bge .L_02008492
	adds r3, r2, #3
.L_02008492:
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r3, r2, r3
	ldrh r2, [r6, r0]
	lsls r3, r3, #1
	adds r3, #24
	strh r2, [r7, r3]
	mov r5, r8
	ldrh r5, [r5, r0]
	mov r2, r12
	adds r4, #1
	strh r5, [r2, r3]
	adds r0, #2
	cmp r4, #3
	ble .L_02008488
	ldr r0, .L_02008538
	ldr r1, .L_0200853c
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_020084e0
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r0
	adds r3, #4
	adds r2, #1
	stmia r3!, {r7}
	strh r2, [r0]
	movs r2, #160
	lsls r2, r2, #19
	adds r2, #96
	stmia r3!, {r2}
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #8
	str r2, [r3]
.L_020084e0:
	strh r4, [r1]
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_02008510
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r0
	adds r3, #4
	strh r2, [r0]
	mov r2, r12
	stmia r3!, {r2}
	movs r2, #160
	lsls r2, r2, #19
	adds r2, #128
	stmia r3!, {r2}
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #8
	str r2, [r3]
.L_02008510:
	strh r4, [r1]
	ldr r2, .L_02008520
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_0200851a:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02008520:
	.4byte gData_020024c4
.L_02008524:
	.4byte Data_02002958
.L_02008528:
	.4byte IwramCopyWords
.L_0200852c:
	.4byte Data_02002978
.L_02008530:
	.4byte gData_02002998
.L_02008534:
	.4byte gData_020029b8
.L_02008538:
	.4byte gIoWriteQueue
.L_0200853c:
	.4byte 0x04000208
	.section .text.x02008540,"ax",%progbits
	.global Func_02000540
	.thumb_func
Func_02000540:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r5, #0
	ldrsh r3, [r3, r5]
	cmp r3, #0
	beq .L_0200855c
	b .L_0200866a
.L_0200855c:
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #164
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	mov r8, r3
	cmp r3, #0
	beq .L_02008572
	b .L_0200866a
.L_02008572:
	ldr r7, .L_02008670
	movs r2, #1
	ldr r3, [r7]
	negs r2, r2
	cmp r3, r2
	bne .L_020085b4
	ldr r6, .L_02008674
	movs r1, #160
	lsls r1, r1, #19
	ldr r5, .L_02008678
	adds r0, r6, #0
	adds r1, #96
	movs r2, #32
	mov lr, r5
	.2byte 0xf800
	adds r1, r6, #0
	movs r2, #32
	ldr r0, .L_0200867c
	mov lr, r5
	.2byte 0xf800
	ldr r6, .L_02008680
	ldr r1, .L_02008684
	movs r2, #32
	adds r0, r6, #0
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_02008688
	adds r1, r6, #0
	movs r2, #32
	mov lr, r5
	.2byte 0xf800
	mov r3, r8
	str r3, [r7]
.L_020085b4:
	ldr r3, [r7]
	cmp r3, #0
	bge .L_020085bc
	adds r3, #7
.L_020085bc:
	asrs r2, r3, #3
	adds r3, r2, #0
	cmp r2, #0
	bge .L_020085c6
	adds r3, r2, #3
.L_020085c6:
	asrs r1, r3, #2
	lsls r3, r1, #2
	subs r1, r2, r3
	ldr r5, .L_02008688
	ldr r2, .L_02008680
	ldr r7, .L_0200867c
	ldr r6, .L_02008674
	movs r4, #0
	mov r12, r5
	mov r8, r2
	movs r0, #24
.L_020085dc:
	adds r2, r1, r4
	adds r3, r2, #0
	cmp r2, #0
	bge .L_020085e6
	adds r3, r2, #3
.L_020085e6:
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r3, r2, r3
	ldrh r2, [r6, r0]
	lsls r3, r3, #1
	adds r3, #24
	strh r2, [r7, r3]
	mov r5, r8
	ldrh r5, [r5, r0]
	mov r2, r12
	adds r4, #1
	strh r5, [r2, r3]
	adds r0, #2
	cmp r4, #3
	ble .L_020085dc
	ldr r0, .L_0200868c
	ldr r1, .L_02008690
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_02008634
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r0
	adds r3, #4
	adds r2, #1
	stmia r3!, {r7}
	strh r2, [r0]
	movs r2, #160
	lsls r2, r2, #19
	adds r2, #96
	stmia r3!, {r2}
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #8
	str r2, [r3]
.L_02008634:
	strh r4, [r1]
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_02008660
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r0
	adds r3, #4
	strh r2, [r0]
	mov r2, r12
	stmia r3!, {r2}
	ldr r2, .L_02008684
	stmia r3!, {r2}
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #8
	str r2, [r3]
.L_02008660:
	strh r4, [r1]
	ldr r2, .L_02008670
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_0200866a:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02008670:
	.4byte gData_020024c8
.L_02008674:
	.4byte Data_020029d8
.L_02008678:
	.4byte IwramCopyWords
.L_0200867c:
	.4byte Data_020029f8
.L_02008680:
	.4byte Data_02002a18
.L_02008684:
	.4byte 0x050001a0
.L_02008688:
	.4byte Data_02002a38
.L_0200868c:
	.4byte gIoWriteQueue
.L_02008690:
	.4byte 0x04000208
	.section .text.x020086bc,"ax",%progbits
	.global Func_020006bc
	.thumb_func
Func_020006bc:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r5, .L_02008754
	movs r2, #1
	ldr r3, [r5]
	negs r2, r2
	cmp r3, r2
	bne .L_020086e2
	movs r1, #160
	lsls r1, r1, #19
	ldr r3, .L_02008758
	ldr r0, .L_0200875c
	adds r1, #128
	movs r2, #32
	mov lr, r3
	.2byte 0xf800
	movs r3, #0
	str r3, [r5]
.L_020086e2:
	ldr r0, [r5]
	movs r1, #15
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	bl __modsi3
	movs r5, #0
	adds r7, r0, #0
	movs r6, #2
.L_020086f6:
	ldr r3, .L_02008760
	adds r0, r7, r5
	movs r1, #15
	mov r8, r3
	bl __modsi3
	ldr r3, .L_0200875c
	lsls r0, r0, #1
	ldrh r3, [r3, r6]
	adds r0, #2
	mov r2, r8
	adds r5, #1
	strh r3, [r2, r0]
	adds r6, #2
	cmp r5, #14
	ble .L_020086f6
	ldr r1, .L_02008764
	ldr r0, .L_02008768
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_02008744
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r1
	adds r3, #4
	strh r2, [r1]
	mov r2, r8
	stmia r3!, {r2}
	ldr r2, .L_0200876c
	stmia r3!, {r2}
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #8
	str r2, [r3]
.L_02008744:
	strh r4, [r0]
	ldr r2, .L_02008754
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02008754:
	.4byte gData_020024cc
.L_02008758:
	.4byte IwramCopyWords
.L_0200875c:
	.4byte gData_02002a58
.L_02008760:
	.4byte gData_02002a78
.L_02008764:
	.4byte gIoWriteQueue
.L_02008768:
	.4byte 0x04000208
.L_0200876c:
	.4byte 0x050003c0
	.section .text.x02008ef8,"ax",%progbits
	.global Func_02000ef8
	.thumb_func
Func_02000ef8:
	push {lr}
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #6
	ldrh r2, [r3]
	ldr r3, .L_02008f24
	ldr r3, [r3]
	cmp r2, r3
	bge .L_02008f2c
	ldr r3, .L_02008f28
	ldrh r2, [r3]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #20
	strh r2, [r3]
	movs r2, #128
	ldr r3, .L_02008f20
	lsls r2, r2, #19
	adds r2, #80
	b .L_02008f46
.L_02008f20:
	.4byte 0x00000000
.L_02008f24:
	.4byte gData_02002a98
.L_02008f28:
	.4byte gData_02002a9c
.L_02008f2c:
	ldr r3, .L_02008f54
	ldrh r2, [r3]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #20
	strh r2, [r3]
	movs r2, #128
	ldr r3, .L_02008f4c
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_02008f50
	subs r2, #2
.L_02008f46:
	strh r3, [r2]
	pop {pc}
	.2byte 0x0000
.L_02008f4c:
	.4byte 0x0000100c
.L_02008f50:
	.4byte 0x00003f42
.L_02008f54:
	.4byte gData_02002a9e
	.section .text.x020091a8,"ax",%progbits
	.global Func_020011a8
	.thumb_func
Func_020011a8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r6, .L_020092f4
	movs r3, #192
	lsls r3, r3, #18
	movs r0, #133
	ldr r2, [r3, #108]
	lsls r0, r0, #2
	adds r3, r6, r0
	movs r1, #230
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r2, r2, r1
	ldr r2, [r2]
	mov r8, r3
	mov r0, r8
	mov r10, r2
	sub sp, #12
	bl Actor_GetById
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r6, r2
	adds r5, r0, #0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_020092f8
	cmp r2, r3
	bne .L_020091ec
	ldr r3, [r5, #20]
	cmp r3, #0
	beq .L_020092e8
.L_020091ec:
	ldr r3, [r5, #8]
	mov r7, sp
	str r3, [r7]
	movs r1, #128
	ldr r3, [r5, #12]
	lsls r1, r1, #10
	str r3, [r7, #4]
	adds r0, r5, #0
	ldr r3, [r5, #16]
	adds r3, r3, r1
	str r3, [r7, #8]
	adds r1, r7, #0
	bl Func_02001c9c
	ldr r3, .L_020092fc
	movs r2, #4
	ldr r3, [r3]
	adds r6, r0, #0
	ands r3, r2
	cmp r3, #0
	bne .L_0200921c
	adds r0, r5, #0
	bl Func_020010d8
.L_0200921c:
	cmp r6, #0
	bge .L_0200926e
	movs r1, #129
	mov r0, r8
	lsls r1, r1, #1
	bl Func_02001d74
	ldr r3, [r5, #16]
	movs r0, #128
	lsls r0, r0, #12
	adds r3, r3, r0
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	adds r0, r5, #0
	bl Func_02001c84
	adds r0, r5, #0
	movs r1, #49
	bl Func_02001c6c
	adds r0, r5, #0
	bl Func_02001c8c
.L_0200924a:
	movs r0, #1
	bl wait
	ldr r2, [r5, #12]
	ldr r3, [r5, #20]
	cmp r2, r3
	bne .L_0200924a
	adds r0, r5, #0
	bl Func_020010d8
	adds r0, r5, #0
	movs r1, #49
	bl Func_02001c6c
	movs r0, #3
	bl wait
	b .L_020092e8
.L_0200926e:
	ldr r3, [r5, #8]
	movs r1, #128
	str r3, [r7]
	lsls r1, r1, #12
	ldr r3, [r5, #12]
	adds r0, r5, #0
	str r3, [r7, #4]
	ldr r3, [r5, #16]
	adds r3, r3, r1
	str r3, [r7, #8]
	adds r1, r7, #0
	bl Func_02001c9c
	adds r6, r0, #0
	cmp r6, #0
	bgt .L_020092e8
	ldr r3, [r5, #8]
	ldr r2, .L_02009300
	adds r0, r5, #0
	adds r3, r3, r2
	str r3, [r7]
	adds r1, r7, #0
	ldr r3, [r5, #12]
	str r3, [r7, #4]
	ldr r3, [r5, #16]
	adds r3, r3, r2
	str r3, [r7, #8]
	bl Func_02001c9c
	adds r6, r0, #0
	cmp r6, #0
	bgt .L_020092e8
	ldr r3, [r5, #8]
	ldr r2, .L_02009304
	ldr r0, .L_02009300
	adds r3, r3, r2
	str r3, [r7]
	adds r1, r7, #0
	ldr r3, [r5, #12]
	str r3, [r7, #4]
	ldr r3, [r5, #16]
	adds r3, r3, r0
	str r3, [r7, #8]
	adds r0, r5, #0
	bl Func_02001c9c
	adds r6, r0, #0
	cmp r6, #0
	bgt .L_020092e8
	mov r1, r10
	ldr r3, [r1, #16]
	movs r2, #128
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r1, #16]
	ldr r3, [r5, #16]
	adds r3, r3, r2
	str r3, [r5, #16]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
.L_020092e8:
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020092f4:
	.4byte gAdventure
.L_020092f8:
	.4byte 0x0000009e
.L_020092fc:
	.4byte gFrameCount
.L_02009300:
	.4byte 0x0005b333
.L_02009304:
	.4byte 0xfffa4ccd
	.section .text.x020095f8,"ax",%progbits
	.global Func_020015f8
	.thumb_func
Func_020015f8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	ldr r3, .L_02009774
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, [r1]
	sub sp, #4
	ldr r3, [r3, #4]
	ldr r5, .L_02009778
	str r3, [sp, #0]
	mov r8, r2
	ldrh r3, [r5]
	ldr r2, .L_0200977c
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r2, [r3, #2]
	ldrh r4, [r3, #2]
	lsrs r2, r2, #5
	mov r11, r2
	ldr r2, .L_02009780
	movs r3, #128
	adds r1, r4, r2
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	mov r9, r0
	adds r3, #212
	ldr r0, .L_02009784
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_02009788
	ldr r0, .L_0200978c
	adds r1, r4, r2
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_02009790
	ldr r0, .L_02009794
	adds r1, r4, r2
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_02009798
	ldr r0, .L_0200979c
	adds r1, r4, r2
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_020097a0
	ldr r0, .L_020097a4
	adds r1, r4, r2
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_020097a8
	ldr r0, .L_020097ac
	adds r1, r4, r2
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_020097b0
	ldr r0, .L_020097a4
	adds r1, r4, r2
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_020097b4
	ldr r0, .L_020097ac
	adds r1, r4, r2
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #127
	adds r5, #4
	mov r10, r3
.L_020096c4:
	ldrh r0, [r5, #18]
	cmp r0, #0
	beq .L_02009758
	ldr r3, [r5]
	mov r1, r9
	subs r4, r3, r1
	ldr r3, [r5, #8]
	ldr r2, [r5, #4]
	mov r1, r8
	subs r3, r3, r1
	subs r1, r3, r2
	ldr r3, [r5, #12]
	cmp r2, r3
	bne .L_020096ea
	movs r2, #236
	lsls r2, r2, #8
	mov r12, r2
	cmp r0, #2
	bne .L_020096f0
.L_020096ea:
	movs r3, #232
	lsls r3, r3, #8
	mov r12, r3
.L_020096f0:
	movs r0, #0
	asrs r3, r4, #16
	asrs r2, r1, #16
	mov lr, r0
	movs r0, #167
	adds r4, r3, #0
	adds r1, r2, #0
	adds r3, #7
	lsls r0, r0, #1
	subs r4, #8
	subs r1, #8
	cmp r3, r0
	bhi .L_02009758
	adds r3, r2, #0
	movs r2, #143
	adds r3, #39
	lsls r2, r2, #1
	cmp r3, r2
	bhi .L_02009758
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r4, r3
	movs r3, #255
	ands r1, r3
	ldrh r3, [r5, #16]
	adds r7, r5, #0
	adds r7, #20
	adds r2, r7, #0
	lsls r6, r3, #2
	movs r0, #0
	cmp r3, #2
	bne .L_02009736
	movs r0, #128
	lsls r0, r0, #21
.L_02009736:
	mov r3, lr
	str r3, [r2]
	lsls r3, r4, #16
	orrs r1, r3
	ldr r3, .L_020097b8
	orrs r1, r0
	orrs r1, r3
	mov r0, r11
	adds r3, r0, r6
	str r1, [r5, #24]
	mov r1, r12
	orrs r1, r3
	str r1, [r5, #28]
	adds r0, r7, #0
	movs r1, #0
	bl Func_02001c3c
.L_02009758:
	movs r2, #1
	negs r2, r2
	add r10, r2
	mov r3, r10
	adds r5, #32
	cmp r3, #0
	bge .L_020096c4
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009774:
	.4byte 0xffff0000
.L_02009778:
	.4byte gData_02002aa0
.L_0200977c:
	.4byte gResourceTableEntries
.L_02009780:
	.4byte 0x06010000
.L_02009784:
	.4byte 0x0600f000
.L_02009788:
	.4byte 0x06010040
.L_0200978c:
	.4byte 0x0600f400
.L_02009790:
	.4byte 0x06010080
.L_02009794:
	.4byte 0x0600f040
.L_02009798:
	.4byte 0x060100c0
.L_0200979c:
	.4byte 0x0600f440
.L_020097a0:
	.4byte 0x06010100
.L_020097a4:
	.4byte 0x0600f080
.L_020097a8:
	.4byte 0x06010140
.L_020097ac:
	.4byte 0x0600f480
.L_020097b0:
	.4byte 0x06010180
.L_020097b4:
	.4byte 0x060101c0
.L_020097b8:
	.4byte 0x40000400
	.section .text.x020097f8,"ax",%progbits
	.global Func_020017f8
	.thumb_func
Func_020017f8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #28
	str r1, [sp, #24]
	str r2, [sp, #20]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r1, #132
	mov r12, r3
	movs r3, #160
	lsls r3, r3, #1
	add r3, r12
	ldr r3, [r3, #48]
	lsls r1, r1, #1
	mov r8, r3
	mov r3, r12
	add r1, r12
	adds r3, #236
	ldr r2, [r1, #8]
	mov r10, r0
	ldr r0, [r3]
	adds r3, #4
	adds r2, r2, r0
	str r2, [sp, #16]
	ldr r2, [r3]
	ldr r1, [r1, #12]
	adds r3, #4
	adds r1, r1, r2
	str r1, [sp, #12]
	ldr r1, .L_02009938
	ldr r3, [r3]
	mov r9, r1
	subs r3, r3, r0
	asrs r3, r3, #20
	str r3, [sp, #8]
	mov r3, r12
	adds r3, #248
	ldr r3, [r3]
	asrs r0, r0, #20
	subs r3, r3, r2
	asrs r3, r3, #20
	str r3, [sp, #4]
	asrs r2, r2, #20
	ldrh r3, [r1, #2]
	lsls r2, r2, #7
	adds r2, r2, r0
	str r2, [sp, #0]
	ldr r2, [sp, #24]
	ldr r1, .L_0200993c
	lsls r3, r3, #5
	add r3, r9
	adds r5, r3, #4
	lsls r3, r2, #8
	str r3, [r1]
	ldr r1, [sp, #0]
	movs r2, #0
	lsls r3, r1, #2
	add r8, r3
	ldr r3, [sp, #4]
	mov lr, r2
	cmp lr, r3
	bge .L_02009928
.L_02009880:
	mov r1, lr
	lsls r1, r1, #16
	lsrs r3, r1, #7
	mov r2, r8
	adds r6, r2, r3
	ldr r3, [sp, #8]
	movs r7, #0
	mov r11, r1
	cmp r7, r3
	bge .L_02009912
.L_02009894:
	ldrb r4, [r6, #2]
	cmp r4, #0
	beq .L_020098fe
	cmp r4, r10
	bcc .L_020098fe
	mov r3, r10
	adds r3, #4
	cmp r4, r3
	bcs .L_020098fe
	ldr r1, [sp, #16]
	lsls r0, r7, #16
	lsrs r0, r0, #16
	lsls r3, r0, #20
	movs r2, #128
	adds r3, r3, r1
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [sp, #12]
	mov r3, r11
	lsrs r1, r3, #16
	lsls r3, r1, #20
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r5, #8]
	ldr r2, [sp, #20]
	lsls r1, r1, #7
	lsls r3, r2, #19
	str r3, [r5, #12]
	ldr r2, [sp, #24]
	adds r0, r0, r1
	lsls r3, r2, #19
	mov r2, r10
	str r3, [r5, #4]
	subs r3, r4, r2
	strh r3, [r5, #16]
	movs r3, #1
	strh r3, [r5, #18]
	mov r2, r9
	ldrh r3, [r2, #2]
	adds r5, #32
	adds r3, #1
	strh r3, [r2, #2]
	movs r3, #158
	lsls r3, r3, #1
	add r3, r12
	ldr r2, [r3]
	ldr r3, [sp, #0]
	adds r2, r2, r3
	movs r3, #120
	strb r3, [r2, r0]
.L_020098fe:
	movs r1, #128
	lsls r3, r7, #16
	lsls r1, r1, #9
	ldr r2, [sp, #8]
	adds r3, r3, r1
	asrs r7, r3, #16
	lsrs r3, r3, #16
	adds r6, #4
	cmp r3, r2
	blt .L_02009894
.L_02009912:
	mov r1, lr
	movs r2, #128
	lsls r3, r1, #16
	lsls r2, r2, #9
	adds r3, r3, r2
	ldr r2, [sp, #4]
	asrs r1, r3, #16
	lsrs r3, r3, #16
	mov lr, r1
	cmp r3, r2
	blt .L_02009880
.L_02009928:
	add sp, #28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009938:
	.4byte gData_02002aa0
.L_0200993c:
	.4byte Data_0202c001 + 0x1df
	.section .text.x02009940,"ax",%progbits
	.global Func_02001940
	.thumb_func
Func_02001940:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #32]
	ldr r2, [r2, #116]
	adds r3, #228
	ldr r1, [r3]
	ldr r3, [r3, #4]
	mov r8, r2
	mov r5, r8
	movs r2, #0
	adds r5, #8
	mov r11, r1
	mov r9, r3
	mov r10, r2
.L_0200996a:
	ldrh r3, [r5, #28]
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r3, r1
	adds r2, r1, #0
	ands r2, r3
	strh r3, [r5, #28]
	cmp r2, r1
	bne .L_02009980
	b .L_02009ab6
.L_02009980:
	movs r0, #179
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009992
	ldrh r3, [r5, #28]
	adds r3, #1
	strh r3, [r5, #28]
.L_02009992:
	ldrh r2, [r5, #28]
	mov r1, r11
	lsls r3, r2, #2
	adds r3, r3, r2
	ldr r2, .L_02009a40
	lsls r3, r3, #1
	adds r4, r3, r2
	ldr r3, [r5, #12]
	subs r2, r3, r1
	cmp r2, #0
	bge .L_020099b0
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r2, r2, r3
.L_020099b0:
	movs r1, #0
	ldrsh r3, [r4, r1]
	asrs r2, r2, #16
	adds r7, r2, r3
	ldr r2, [r5, #16]
	ldr r3, [r5, #20]
	adds r4, #2
	subs r3, r3, r2
	mov r2, r9
	subs r3, r3, r2
	cmp r3, #0
	bge .L_020099d0
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r3, r1
.L_020099d0:
	movs r1, #0
	ldrsh r2, [r4, r1]
	asrs r3, r3, #16
	adds r6, r3, r2
	adds r3, r7, #0
	adds r3, #16
	adds r4, #2
	cmp r3, #255
	bhi .L_02009a60
	movs r2, #32
	negs r2, r2
	cmp r6, r2
	blt .L_02009a60
	cmp r6, #159
	bgt .L_02009a60
	ldrb r3, [r5, #9]
	movs r1, #13
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r5, #9]
	ldr r3, .L_02009a30
	ldr r2, .L_02009a34
	ands r7, r3
	ldrh r3, [r5, #6]
	strb r6, [r5, #4]
	ands r3, r2
	orrs r3, r7
	strh r3, [r5, #6]
	mov r2, r8
	ldrh r3, [r4]
	ldr r1, [r2, #4]
	ldr r2, .L_02009a38
	adds r1, r1, r3
	ldr r3, .L_02009a3c
	adds r4, #2
	ands r1, r3
	ldrh r3, [r5, #8]
	ldrb r0, [r5, #5]
	ands r3, r2
	orrs r3, r1
	strh r3, [r5, #8]
	movs r2, #63
	ldrb r1, [r4]
	adds r3, r2, #0
	b .L_02009a44
.L_02009a30:
	.4byte 0x000001ff
.L_02009a34:
	.4byte 0xfffffe00
.L_02009a38:
	.4byte 0xfffffc00
.L_02009a3c:
	.4byte 0x000003ff
.L_02009a40:
	.4byte gData_02001f18
.L_02009a44:
	lsls r1, r1, #6
	ands r3, r0
	orrs r3, r1
	strb r3, [r5, #5]
	ldrb r1, [r5, #7]
	ldrb r3, [r4, #2]
	ands r2, r1
	lsls r3, r3, #6
	orrs r2, r3
	strb r2, [r5, #7]
	adds r0, r5, #0
	movs r1, #240
	bl Func_02001c3c
.L_02009a60:
	ldrh r3, [r5, #28]
	cmp r3, #0
	bne .L_02009ab6
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #8
	add r3, r8
	ldr r6, [r3]
	cmp r6, #0
	beq .L_02009aac
	bl Random16Far
	ldr r3, [r6]
	lsls r2, r0, #4
	ldr r1, .L_02009ad0
	subs r2, r2, r0
	lsls r2, r2, #4
	adds r3, r3, r2
	adds r7, r3, r1
	bl Random16Far
	ldr r3, [r6, #8]
	lsls r2, r0, #2
	adds r2, r2, r0
	lsls r2, r2, #5
	adds r3, r3, r2
	ldr r2, .L_02009ad4
	str r7, [r5, #12]
	adds r6, r3, r2
	str r6, [r5, #20]
	movs r0, #0
	adds r1, r7, #0
	adds r2, r6, #0
	bl Map_GetTerrainHeight
	movs r3, #16
	str r0, [r5, #16]
	b .L_02009ab4
.L_02009aac:
	movs r3, #16
	str r6, [r5, #12]
	str r6, [r5, #20]
	str r6, [r5, #16]
.L_02009ab4:
	strh r3, [r5, #28]
.L_02009ab6:
	movs r3, #1
	add r10, r3
	mov r1, r10
	adds r5, #32
	cmp r1, #63
	bhi .L_02009ac4
	b .L_0200996a
.L_02009ac4:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009ad0:
	.4byte 0xff880000
.L_02009ad4:
	.4byte 0xffb00000
	.section .bss,"aw",%nobits
	.space 0x00000002
	.global Data_02002958
Data_02002958:
	.space 0x00000020
	.global Data_02002978
Data_02002978:
	.space 0x00000020
	.global gData_02002998
gData_02002998:
	.space 0x00000020
	.global gData_020029b8
gData_020029b8:
	.space 0x00000020
	.global Data_020029d8
Data_020029d8:
	.space 0x00000020
	.global Data_020029f8
Data_020029f8:
	.space 0x00000020
	.global Data_02002a18
Data_02002a18:
	.space 0x00000020
	.global Data_02002a38
Data_02002a38:
	.space 0x00000020
	.global gData_02002a58
gData_02002a58:
	.space 0x00000020
	.global gData_02002a78
gData_02002a78:
	.space 0x00000020
	.global gData_02002a98
gData_02002a98:
	.space 0x00000004
	.global gData_02002a9c
gData_02002a9c:
	.space 0x00000002
	.global gData_02002a9e
gData_02002a9e:
	.space 0x00000002
	.global gData_02002aa0
gData_02002aa0:

.syntax unified
	.thumb
	.section .text.x020084c8,"ax",%progbits
	.balign 4
	.global Func_020004c8
	.thumb_func
Func_020004c8:
	push {r5, r6, r7, lr}
	ldr r3, .L_02008634
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008638
	sub sp, #12
	adds r6, r0, #0
	cmp r2, r3
	bne .L_02008516
	ldr r3, .L_0200863c
	adds r2, r6, #0
	subs r2, #10
	ldrb r0, [r3, r2]
	bl Func_02002708
	adds r7, r0, #0
	cmp r6, #12
	bne .L_02008502
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #110
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008502
	b .L_0200862e
.L_02008502:
	cmp r6, #18
	bne .L_02008516
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #109
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008516
	b .L_0200862e
.L_02008516:
	ldr r3, .L_02008634
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008640
	cmp r2, r3
	bne .L_0200855a
	ldr r3, .L_02008644
	adds r2, r6, #0
	subs r2, #10
	ldrb r0, [r3, r2]
	bl Func_02002708
	adds r7, r0, #0
	cmp r6, #10
	bne .L_02008548
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200862e
.L_02008548:
	cmp r6, #12
	bne .L_0200855a
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200862e
.L_0200855a:
	ldr r3, .L_02008634
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008648
	cmp r2, r3
	bne .L_02008588
	adds r0, r6, #0
	subs r0, #10
	bl Func_02002708
	adds r7, r0, #0
	cmp r6, #10
	bne .L_02008588
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200862e
.L_02008588:
	ldr r5, .L_02008634
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200864c
	cmp r2, r3
	bne .L_020085a4
	adds r0, r6, #0
	subs r0, #10
	bl Func_02002708
	adds r7, r0, #0
.L_020085a4:
	cmp r7, #0
	beq .L_0200862e
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Actor_GetById
	adds r5, r0, #0
	bl Func_02002898
	movs r0, #0
	bl Func_02002968
	b .L_02008610
.L_020085c2:
	cmp r0, #0
	bge .L_020085d4
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	adds r0, r6, #0
	bl Func_02000428
	b .L_0200862e
.L_020085d4:
	ldr r3, [r5, #16]
	movs r1, #128
	lsls r1, r1, #10
	adds r3, r3, r1
	str r3, [r5, #16]
	ldr r2, [r5, #8]
	ldr r3, .L_02008650
	ldr r1, .L_02008654
	ands r3, r2
	adds r3, r3, r1
	movs r1, #128
	lsls r1, r1, #9
	cmp r3, r1
	ble .L_020085f4
	movs r3, #128
	lsls r3, r3, #9
.L_020085f4:
	ldr r1, .L_02008658
	cmp r3, r1
	bge .L_020085fc
	ldr r3, .L_02008658
.L_020085fc:
	subs r3, r2, r3
	str r3, [r5, #8]
	ldrh r3, [r5, #6]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	strh r3, [r5, #6]
	movs r0, #1
	bl wait
.L_02008610:
	ldr r3, [r5, #8]
	mov r1, sp
	str r3, [r1]
	movs r2, #128
	ldr r3, [r5, #12]
	lsls r2, r2, #12
	str r3, [r1, #4]
	adds r0, r5, #0
	ldr r3, [r5, #16]
	adds r3, r3, r2
	str r3, [r1, #8]
	bl Func_02002850
	cmp r0, #0
	ble .L_020085c2
.L_0200862e:
	add sp, #12
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008634:
	.4byte gMainRoutineWork
.L_02008638:
	.4byte 0x00000093
.L_0200863c:
	.4byte gData_020029b8
.L_02008640:
	.4byte 0x00000095
.L_02008644:
	.4byte gData_020029c0 + 0x1
.L_02008648:
	.4byte 0x00000096
.L_0200864c:
	.4byte 0x00000097
.L_02008650:
	.4byte 0x000fffff
.L_02008654:
	.4byte 0xfff80000
.L_02008658:
	.4byte 0xffff0000


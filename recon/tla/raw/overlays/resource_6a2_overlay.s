.syntax unified
	.thumb
	.section .text.x02008fde,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009016,"ax",%progbits
	.2byte 0x0000
	.section .text.x020097de,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009978,"ax",%progbits
	.global Func_02001978
	.thumb_func
Func_02001978:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02009a20
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Actor_GetById
	ldr r3, [r0, #8]
	ldr r6, .L_02009a24
	asrs r3, r3, #20
	mov r8, r3
	ldr r3, [r0, #16]
	adds r5, r6, #0
	asrs r3, r3, #20
	mov r10, r3
	movs r1, #10
	ldrsh r3, [r6, r1]
	movs r7, #0
	adds r5, #32
	ldrh r2, [r6, #10]
	cmp r7, r3
	bge .L_02009a14
.L_020099ac:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, r8
	bne .L_02009a08
	movs r1, #2
	ldrsh r3, [r5, r1]
	cmp r3, r10
	bne .L_02009a08
	movs r2, #4
	ldrsh r0, [r5, r2]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020099dc
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_020015d8
	movs r3, #4
	ldrsh r0, [r5, r3]
	bl GameFlag_SetBit
	strh r7, [r6, #12]
	b .L_02009a14
.L_020099dc:
	movs r1, #12
	ldrsh r3, [r6, r1]
	cmp r7, r3
	beq .L_02009a14
	adds r0, r6, #0
	adds r1, r5, #0
	strh r7, [r6, #12]
	bl Func_02001648
	movs r2, #2
	ldrsh r0, [r6, r2]
	mov r1, r8
	bl GameFlag_SetByte
	movs r3, #2
	ldrsh r0, [r6, r3]
	mov r1, r10
	adds r0, #8
	bl GameFlag_SetByte
	movs r0, #1
	b .L_02009a16
.L_02009a08:
	lsls r3, r2, #16
	adds r7, #1
	asrs r3, r3, #16
	adds r5, #8
	cmp r7, r3
	blt .L_020099ac
.L_02009a14:
	movs r0, #0
.L_02009a16:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009a20:
	.4byte gMainRoutineWork
.L_02009a24:
	.4byte Data_02002764
	.section .bss,"aw",%nobits
	.global gData_02002618
gData_02002618:
	.space 0x00000004
	.global gData_0200261c
gData_0200261c:
	.space 0x00000020
	.global gData_0200263c
gData_0200263c:
	.space 0x00000004
	.global gData_02002640
gData_02002640:
	.space 0x0000001c
	.global gData_0200265c
gData_0200265c:
	.space 0x00000002
	.global gData_0200265e
gData_0200265e:
	.space 0x00000002
	.global gData_02002660
gData_02002660:
	.space 0x00000040
	.global gData_020026a0
gData_020026a0:
	.space 0x000000c0
	.global gData_02002760
gData_02002760:
	.space 0x00000002
	.global gData_02002762
gData_02002762:
	.space 0x00000002
	.global Data_02002764
Data_02002764:

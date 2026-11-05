.syntax unified
	.thumb
	.section .text.x020085a0,"ax",%progbits
	.global Func_020005a0
	.thumb_func
Func_020005a0:
	push {r5, lr}
	ldr r3, .L_020085e4
	adds r5, r0, #0
	adds r4, r3, #0
	movs r2, #0
	ldrsh r3, [r4, r2]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	beq .L_020085de
	movs r0, #2
	movs r1, #0
.L_020085b8:
	ldrsh r2, [r4, r1]
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_020085d0
	ldrsh r2, [r0, r4]
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_020085d0
	movs r0, #1
	b .L_020085e0
.L_020085d0:
	adds r1, #4
	ldrsh r3, [r4, r1]
	movs r2, #1
	negs r2, r2
	adds r0, #4
	cmp r3, r2
	bne .L_020085b8
.L_020085de:
	movs r0, #0
.L_020085e0:
	pop {r5, pc}
	.2byte 0x0000
.L_020085e4:
	.4byte gData_02002c64
	.section .text.x0200915a,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009f3a,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009f72,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200a73a,"ax",%progbits
	.2byte 0x0000
	.section .bss,"aw",%nobits
	.global Data_020035b8
Data_020035b8:
	.space 0x00000004
	.global gData_020035bc
gData_020035bc:
	.space 0x00000020
	.global gData_020035dc
gData_020035dc:
	.space 0x00000004
	.global gData_020035e0
gData_020035e0:
	.space 0x0000001c
	.global gData_020035fc
gData_020035fc:
	.space 0x00000002
	.global gData_020035fe
gData_020035fe:
	.space 0x00000002
	.global gData_02003600
gData_02003600:
	.space 0x00000040
	.global Data_02003640
Data_02003640:
	.space 0x000000c0
	.global gData_02003700
gData_02003700:
	.space 0x00000002
	.global gData_02003702
gData_02003702:
	.space 0x00000002
	.global gData_02003704
gData_02003704:

.syntax unified
	.thumb
	.section .text.x02008c78,"ax",%progbits
	.global Func_02000c78
	.thumb_func
Func_02000c78:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	sub sp, #8
	mov r8, r3
	bl Actor_GetById
	ldr r3, [r0, #80]
	ldr r5, .L_02008d7c
	ldr r3, [r3, #40]
	movs r1, #0
	movs r2, #0
	ldrsh r3, [r3, r2]
	lsls r4, r3, #16
	ldrh r3, [r5, r1]
	lsrs r2, r4, #16
	cmp r2, r3
	beq .L_02008cbe
.L_02008ca4:
	movs r2, #128
	lsls r3, r1, #16
	lsls r2, r2, #9
	adds r3, r3, r2
	lsrs r2, r3, #16
	asrs r1, r3, #16
	cmp r2, #5
	bhi .L_02008cbe
	lsls r3, r2, #1
	ldrh r3, [r5, r3]
	lsrs r2, r4, #16
	cmp r2, r3
	bne .L_02008ca4
.L_02008cbe:
	lsls r3, r1, #16
	lsrs r2, r3, #16
	cmp r2, #6
	bne .L_02008cca
	movs r0, #0
	b .L_02008d72
.L_02008cca:
	ldr r6, .L_02008d80
	lsls r2, r2, #2
	ldrsb r4, [r6, r2]
	adds r1, r4, #0
	cmp r4, #0
	bge .L_02008cd8
	negs r1, r4
.L_02008cd8:
	adds r3, r2, #2
	ldrsb r3, [r6, r3]
	cmp r3, #0
	bge .L_02008ce2
	negs r3, r3
.L_02008ce2:
	adds r3, r1, r3
	asrs r7, r3, #4
	adds r3, r2, #1
	ldrsb r1, [r6, r3]
	adds r5, r1, #0
	cmp r1, #0
	bge .L_02008cf2
	negs r5, r1
.L_02008cf2:
	adds r3, r2, #3
	ldrsb r2, [r6, r3]
	cmp r2, #0
	bge .L_02008cfc
	negs r2, r2
.L_02008cfc:
	adds r5, r5, r2
	mov r10, r5
	ldr r6, [r0, #8]
	mov r3, r10
	ldr r5, [r0, #16]
	asrs r3, r3, #4
	mov r10, r3
	lsls r3, r4, #16
	adds r6, r6, r3
	lsls r3, r1, #16
	adds r5, r5, r3
	movs r3, #164
	lsls r3, r3, #1
	add r3, r8
	ldr r3, [r3]
	asrs r6, r6, #20
	asrs r1, r3, #20
	movs r3, #166
	lsls r3, r3, #1
	add r3, r8
	ldr r3, [r3]
	lsls r2, r1, #16
	asrs r3, r3, #20
	lsls r3, r3, #16
	asrs r5, r5, #20
	lsrs r2, r2, #16
	lsrs r3, r3, #16
	adds r2, r6, r2
	adds r3, r5, r3
	str r2, [sp, #0]
	str r3, [sp, #4]
	adds r0, r6, #0
	adds r1, r5, #0
	adds r2, r7, #0
	mov r3, r10
	bl Func_02002330
	movs r3, #255
	mov r2, r10
	str r2, [sp, #0]
	str r3, [sp, #4]
	mov r8, r3
	movs r0, #0
	adds r1, r6, #0
	adds r2, r5, #0
	adds r3, r7, #0
	bl Map_FillCellRectByte2
	mov r2, r10
	mov r3, r8
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #2
	adds r1, r6, #0
	adds r2, r5, #0
	adds r3, r7, #0
	bl Map_FillCellRectByte2
	movs r0, #1
.L_02008d72:
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02008d7c:
	.4byte gData_02002478
.L_02008d80:
	.4byte gData_02002484
	.section .bss,"aw",%nobits
	.global gData_02002ce8
gData_02002ce8:
	.space 0x00000004
	.global gData_02002cec
gData_02002cec:
	.space 0x00000020
	.global gData_02002d0c
gData_02002d0c:
	.space 0x00000004
	.global gData_02002d10
gData_02002d10:
	.space 0x0000001c
	.global gData_02002d2c
gData_02002d2c:
	.space 0x00000002
	.global gData_02002d2e
gData_02002d2e:
	.space 0x00000002
	.global gData_02002d30
gData_02002d30:
	.space 0x00000040
	.global Data_02002d70
Data_02002d70:
	.space 0x000000c0
	.global gData_02002e30
gData_02002e30:
	.space 0x00000002
	.global gData_02002e32
gData_02002e32:
	.space 0x00000002
	.global gData_02002e34
gData_02002e34:

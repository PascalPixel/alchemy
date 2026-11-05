.syntax unified
	.thumb
	.section .text.x02008c4c,"ax",%progbits
	.global Func_02000c4c
	.thumb_func
Func_02000c4c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	adds r7, r1, #0
	sub sp, #4
	bl Resource_FindFreeEntry
	movs r1, #164
	adds r1, r1, r7
	mov r8, r1
	ldr r2, .L_02008ca8
	mov r3, r8
	strh r0, [r3]
	movs r1, #128
	lsls r0, r0, #16
	mov r10, r2
	lsls r1, r1, #1
	ldr r2, .L_02008cac
	asrs r0, r0, #16
	bl VramCache_Load
	adds r3, r7, #0
	movs r2, #186
	movs r5, #0
	adds r3, #166
	lsls r2, r2, #2
	strh r5, [r3]
	adds r2, #255
	subs r3, #6
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	adds r3, #6
	str r6, [r3]
	adds r3, r6, #0
	mov r0, r10
	adds r3, #98
	strb r0, [r3]
	adds r3, #1
	b .L_02008cb0
	.2byte 0x0000
.L_02008ca8:
	.4byte 0x00000000
.L_02008cac:
	.4byte gData_02000f44
.L_02008cb0:
	strb r0, [r3]
	ldr r3, .L_02008d40
	mov r0, r8
	str r3, [r6, #108]
	movs r1, #0
	ldrsh r3, [r0, r1]
	ldr r2, .L_02008d44
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	str r7, [r6, #104]
	lsrs r3, r3, #5
	mov r11, r3
	mov r9, r5
	mov r10, r5
.L_02008cce:
	movs r1, #1
	mov r2, r10
	mov r8, r1
	adds r5, r2, r7
.L_02008cd6:
	mov r3, r11
	str r3, [sp, #0]
	adds r0, r5, #0
	movs r1, #16
	movs r2, #16
	ldr r3, .L_02008d48
	bl Func_02000ee4
	ldrb r3, [r5, #5]
	movs r0, #33
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	ldrb r2, [r5, #9]
	strb r3, [r5, #5]
	movs r3, #15
	ands r3, r2
	strb r3, [r5, #9]
	adds r0, r6, #0
	bl Func_02000ef4
	movs r3, #3
	ands r0, r3
	movs r1, #13
	ldrb r3, [r5, #9]
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	movs r2, #1
	lsls r0, r0, #2
	negs r2, r2
	orrs r3, r0
	add r8, r2
	strb r3, [r5, #9]
	mov r3, r8
	adds r5, #40
	cmp r3, #0
	bge .L_02008cd6
	movs r1, #1
	add r9, r1
	movs r0, #80
	mov r2, r9
	add r10, r0
	cmp r2, #1
	ble .L_02008cce
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008d40:
	.4byte Func_02000ad0
.L_02008d44:
	.4byte gResourceTableEntries
.L_02008d48:
	.4byte 0x80004000
	.section .bss,"aw",%nobits
	.balign 16
	.global gData_020017b0
gData_020017b0:
	.space 0x000000b0
	.global gData_02001860
gData_02001860:

.syntax unified
	.thumb
	.section .text.x02008ba0,"ax",%progbits
	.balign 4
	.global Func_02000ba0
	.thumb_func
Func_02000ba0:
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
	beq .L_02008bbc
	b .L_02008cca
.L_02008bbc:
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #164
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	mov r8, r3
	cmp r3, #0
	beq .L_02008bd2
	b .L_02008cca
.L_02008bd2:
	ldr r7, .L_02008cd0
	movs r2, #1
	ldr r3, [r7]
	negs r2, r2
	cmp r3, r2
	bne .L_02008c14
	ldr r6, .L_02008cd4
	movs r1, #160
	lsls r1, r1, #19
	ldr r5, .L_02008cd8
	adds r0, r6, #0
	adds r1, #96
	movs r2, #32
	mov lr, r5
	.2byte 0xf800
	adds r1, r6, #0
	movs r2, #32
	ldr r0, .L_02008cdc
	mov lr, r5
	.2byte 0xf800
	ldr r6, .L_02008ce0
	ldr r1, .L_02008ce4
	movs r2, #32
	adds r0, r6, #0
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_02008ce8
	adds r1, r6, #0
	movs r2, #32
	mov lr, r5
	.2byte 0xf800
	mov r3, r8
	str r3, [r7]
.L_02008c14:
	ldr r3, [r7]
	cmp r3, #0
	bge .L_02008c1c
	adds r3, #7
.L_02008c1c:
	asrs r2, r3, #3
	adds r3, r2, #0
	cmp r2, #0
	bge .L_02008c26
	adds r3, r2, #3
.L_02008c26:
	asrs r1, r3, #2
	lsls r3, r1, #2
	subs r1, r2, r3
	ldr r5, .L_02008ce8
	ldr r2, .L_02008ce0
	ldr r7, .L_02008cdc
	ldr r6, .L_02008cd4
	movs r4, #0
	mov r12, r5
	mov r8, r2
	movs r0, #4
.L_02008c3c:
	adds r2, r1, r4
	adds r3, r2, #0
	cmp r2, #0
	bge .L_02008c46
	adds r3, r2, #3
.L_02008c46:
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r3, r2, r3
	ldrh r2, [r6, r0]
	lsls r3, r3, #1
	adds r3, #4
	strh r2, [r7, r3]
	mov r5, r8
	ldrh r5, [r5, r0]
	mov r2, r12
	adds r4, #1
	strh r5, [r2, r3]
	adds r0, #2
	cmp r4, #3
	ble .L_02008c3c
	ldr r0, .L_02008cec
	ldr r1, .L_02008cf0
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_02008c94
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
.L_02008c94:
	strh r4, [r1]
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_02008cc0
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r0
	adds r3, #4
	strh r2, [r0]
	mov r2, r12
	stmia r3!, {r2}
	ldr r2, .L_02008ce4
	stmia r3!, {r2}
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #8
	str r2, [r3]
.L_02008cc0:
	strh r4, [r1]
	ldr r2, .L_02008cd0
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_02008cca:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02008cd0:
	.4byte gData_02001aa4
.L_02008cd4:
	.4byte Data_02001aa8
.L_02008cd8:
	.4byte IwramCopyWords
.L_02008cdc:
	.4byte gData_02001ac8
.L_02008ce0:
	.4byte Data_02001ae8
.L_02008ce4:
	.4byte 0x05000140
.L_02008ce8:
	.4byte Data_02001b08
.L_02008cec:
	.4byte gIoWriteQueue
.L_02008cf0:
	.4byte 0x04000208
	.section .bss,"aw",%nobits
	.global Data_02001aa8
Data_02001aa8:
	.space 0x00000020
	.global gData_02001ac8
gData_02001ac8:
	.space 0x00000020
	.global Data_02001ae8
Data_02001ae8:
	.space 0x00000020
	.global Data_02001b08
Data_02001b08:

.syntax unified
	.thumb
	.global Func_0803aed8
	.thumb_func
Func_0803aed8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #32
	mov r10, r0
	adds r5, r1, #0
	adds r0, r2, #0
	ldr r1, [sp, #60]
	ldr r2, [sp, #68]
	adds r7, r3, #0
	mov r3, r10
	mov r8, r1
	mov lr, r2
	cmp r3, #0
	beq .L_0803af28
	ldr r3, .L_0803af20
	lsls r2, r0, #1
	strh r3, [r2, r7]
	movs r2, #128
	lsls r2, r2, #1
	adds r2, #255
	adds r0, #1
	ands r0, r2
	ldr r1, .L_0803af24
	lsls r3, r0, #1
	adds r0, #1
	ands r0, r2
	strh r1, [r3, r7]
	lsls r3, r0, #1
	strh r1, [r3, r7]
	adds r0, #1
	ands r0, r2
	b .L_0803af28
	.2byte 0x0000
.L_0803af20:
	.4byte 0x00000020
.L_0803af24:
	.4byte 0x0000000a
.L_0803af28:
	mov r4, r8
	cmp r4, #1
	beq .L_0803af38
	cmp r4, #3
	bne .L_0803afbe
	ldr r6, [sp, #64]
	cmp r6, #0
	bne .L_0803afbe
.L_0803af38:
	ldr r3, .L_0803affc
	mov r9, sp
	movs r1, #0
	mov r2, r9
	mov r12, r1
	ldmia r3!, {r1, r4, r6}
	stmia r2!, {r1, r4, r6}
	ldmia r3!, {r1, r4, r6}
	stmia r2!, {r1, r4, r6}
	ldmia r3!, {r4, r6}
	stmia r2!, {r4, r6}
	ldrh r2, [r5]
	cmp r2, #29
	bne .L_0803af5c
	ldrh r3, [r5, #2]
	adds r5, #4
	subs r3, #1
	mov r12, r3
.L_0803af5c:
	mov r1, r12
	cmp r1, #0
	bne .L_0803af7e
	cmp r2, #65
	beq .L_0803af7a
	cmp r2, #73
	beq .L_0803af7a
	cmp r2, #85
	beq .L_0803af7a
	cmp r2, #69
	beq .L_0803af7a
	movs r3, #1
	mov r12, r3
	cmp r2, #79
	bne .L_0803af7e
.L_0803af7a:
	movs r4, #2
	mov r12, r4
.L_0803af7e:
	movs r3, #7
	mov r6, r12
	ands r6, r3
	lsls r3, r6, #2
	mov r1, r9
	ldr r4, [r1, r3]
	movs r6, #0
	ldrb r3, [r4]
	adds r4, #1
	lsls r1, r3, #24
	cmp r1, #0
	beq .L_0803afc8
	movs r2, #128
	lsls r2, r2, #1
	adds r2, #255
	mov r12, r2
.L_0803af9e:
	lsls r2, r0, #1
	asrs r3, r1, #24
	strh r3, [r2, r7]
	adds r0, #1
	mov r3, r12
	adds r6, #1
	ands r0, r3
	cmp r6, #7
	bgt .L_0803afc8
	ldrb r3, [r4]
	adds r4, #1
	lsls r3, r3, #24
	adds r1, r3, #0
	cmp r3, #0
	bne .L_0803af9e
	b .L_0803afc8
.L_0803afbe:
	ldrh r3, [r5]
	ldrh r2, [r5]
	cmp r3, #29
	bne .L_0803afca
	adds r5, #4
.L_0803afc8:
	ldrh r2, [r5]
.L_0803afca:
	adds r3, r2, #0
	cmp r3, #0
	beq .L_0803b00c
	movs r6, #128
	lsls r6, r6, #1
	adds r6, #255
	movs r4, #1
	movs r1, #0
.L_0803afda:
	lsls r2, r2, #16
	asrs r2, r2, #16
	lsls r3, r0, #1
	strh r2, [r3, r7]
	lsls r2, r2, #16
	adds r0, #1
	lsrs r2, r2, #16
	adds r5, #2
	ands r0, r6
	cmp r2, #83
	beq .L_0803aff4
	cmp r2, #115
	bne .L_0803b000
.L_0803aff4:
	mov r2, lr
	str r4, [r2]
	b .L_0803b004
	.2byte 0x0000
.L_0803affc:
	.4byte Data_0805c10c
.L_0803b000:
	mov r3, lr
	str r1, [r3]
.L_0803b004:
	ldrh r2, [r5]
	adds r3, r2, #0
	cmp r3, #0
	bne .L_0803afda
.L_0803b00c:
	mov r4, r8
	cmp r4, #2
	beq .L_0803b01c
	cmp r4, #3
	bne .L_0803b044
	ldr r6, [sp, #64]
	cmp r6, #0
	beq .L_0803b044
.L_0803b01c:
	mov r1, lr
	ldr r3, [r1]
	cmp r3, #0
	beq .L_0803b034
	ldr r2, .L_0803b058
	lsls r3, r0, #1
	strh r2, [r3, r7]
	movs r3, #128
	lsls r3, r3, #1
	adds r0, #1
	adds r3, #255
	ands r0, r3
.L_0803b034:
	ldr r2, .L_0803b05c
	lsls r3, r0, #1
	strh r2, [r3, r7]
	movs r3, #128
	lsls r3, r3, #1
	adds r0, #1
	adds r3, #255
	ands r0, r3
.L_0803b044:
	mov r2, r10
	cmp r2, #0
	beq .L_0803b088
	ldr r3, .L_0803b060
	movs r1, #128
	lsls r2, r0, #1
	lsls r1, r1, #1
	strh r3, [r2, r7]
	b .L_0803b064
	.2byte 0x0000
.L_0803b058:
	.4byte 0x00000065
.L_0803b05c:
	.4byte 0x00000073
.L_0803b060:
	.4byte 0x0000000a
.L_0803b064:
	adds r1, #255
	ldr r3, .L_0803b080
	adds r0, #1
	ands r0, r1
	lsls r2, r0, #1
	strh r3, [r2, r7]
	adds r0, #1
	ldr r3, .L_0803b084
	ands r0, r1
	lsls r2, r0, #1
	strh r3, [r2, r7]
	adds r0, #1
	ands r0, r1
	b .L_0803b088
.L_0803b080:
	.4byte 0x00000008
.L_0803b084:
	.4byte 0x00000020
.L_0803b088:
	add sp, #32
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}

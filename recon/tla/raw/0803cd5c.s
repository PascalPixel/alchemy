.syntax unified
	.thumb
	.global Func_0803cd5c
	.thumb_func
Func_0803cd5c:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	movs r2, #152
	lsls r2, r2, #5
	adds r2, #137
	adds r3, r3, r2
	ldrb r3, [r3]
	adds r6, r0, #0
	movs r5, #0
	cmp r3, #0
	beq .L_0803cd80
	bl Audio_Check
	cmp r0, #0
	bne .L_0803cd80
	movs r5, #1
.L_0803cd80:
	ldr r1, .L_0803cdb0
	movs r2, #129
	ldr r3, [r1]
	lsls r2, r2, #2
	adds r2, #255
	ands r3, r2
	cmp r3, #0
	beq .L_0803cd92
	movs r5, #1
.L_0803cd92:
	ldr r3, [r1]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0803cd9e
	movs r5, #1
.L_0803cd9e:
	cmp r5, #0
	beq .L_0803cdaa
	movs r3, #0
	strh r3, [r6, #20]
	movs r0, #1
	b .L_0803cdac
.L_0803cdaa:
	movs r0, #0
.L_0803cdac:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0803cdb0:
	.4byte gInput

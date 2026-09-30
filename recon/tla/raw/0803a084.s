.syntax unified
	.thumb
	.global Func_0803a084
	.thumb_func
Func_0803a084:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r7, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	mov r8, r2
	mov r11, r3
	lsls r3, r1, #6
	lsls r2, r0, #1
	add r3, r11
	adds r6, r3, r2
	adds r5, r6, #0
	mov r2, r8
	adds r5, #8
	cmp r2, #1
	bhi .L_0803a0b2
	b .L_0803a1ae
.L_0803a0b2:
	cmp r7, #1
	bhi .L_0803a0b8
	b .L_0803a1ae
.L_0803a0b8:
	cmp r2, #30
	bls .L_0803a0be
	b .L_0803a1ae
.L_0803a0be:
	cmp r7, #30
	bls .L_0803a0c4
	b .L_0803a1ae
.L_0803a0c4:
	adds r3, r7, #0
	bl Func_08041abc
	mov r2, r11
	ldrb r3, [r2, #4]
	cmp r3, #0
	beq .L_0803a0da
	ldr r3, .L_0803a104
	strh r3, [r5]
	adds r5, #2
	b .L_0803a0e2
.L_0803a0da:
	ldr r3, .L_0803a108
	strh r3, [r5]
	adds r5, r6, #0
	adds r5, #10
.L_0803a0e2:
	movs r3, #2
	negs r3, r3
	add r3, r8
	mov r10, r3
	adds r0, r5, #0
	mov r2, r10
	ldr r1, .L_0803a110
	bl Func_0803a054
	mov r2, r11
	ldrb r3, [r2, #4]
	adds r5, r0, #0
	cmp r3, #0
	beq .L_0803a114
	ldr r3, .L_0803a10c
	b .L_0803a116
	.2byte 0x0000
.L_0803a104:
	.4byte 0x0000f01c
.L_0803a108:
	.4byte 0x0000f010
.L_0803a10c:
	.4byte 0x0000f41c
.L_0803a110:
	.4byte 0xf011f011
.L_0803a114:
	ldr r3, .L_0803a14c
.L_0803a116:
	strh r3, [r5]
	adds r5, #2
	movs r3, #32
	mov r2, r8
	subs r3, r3, r2
	lsls r3, r3, #1
	movs r6, #1
	subs r7, #1
	adds r5, r5, r3
	cmp r6, r7
	bcs .L_0803a164
	mov r9, r3
.L_0803a12e:
	movs r3, #240
	lsls r3, r3, #8
	adds r3, #22
	mov r2, r8
	strh r3, [r5]
	adds r5, #2
	cmp r2, #2
	beq .L_0803a156
	adds r0, r5, #0
	ldr r1, .L_0803a150
	mov r2, r10
	bl Func_0803a054
	b .L_0803a154
	.2byte 0x0000
.L_0803a14c:
	.4byte 0x0000f012
.L_0803a150:
	.4byte 0xf020f020
.L_0803a154:
	adds r5, r0, #0
.L_0803a156:
	ldr r3, .L_0803a170
	adds r6, #1
	strh r3, [r5]
	adds r5, #2
	add r5, r9
	cmp r6, r7
	bcc .L_0803a12e
.L_0803a164:
	mov r2, r11
	ldrb r3, [r2, #4]
	cmp r3, #0
	beq .L_0803a178
	ldr r3, .L_0803a174
	b .L_0803a17a
.L_0803a170:
	.4byte 0x0000f017
.L_0803a174:
	.4byte 0x0000f81c
.L_0803a178:
	ldr r3, .L_0803a198
.L_0803a17a:
	strh r3, [r5]
	adds r5, #2
	adds r0, r5, #0
	mov r2, r10
	ldr r1, .L_0803a1a0
	bl Func_0803a054
	mov r2, r11
	ldrb r3, [r2, #4]
	adds r5, r0, #0
	cmp r3, #0
	beq .L_0803a1a4
	ldr r3, .L_0803a19c
	b .L_0803a1a6
	.2byte 0x0000
.L_0803a198:
	.4byte 0x0000f013
.L_0803a19c:
	.4byte 0x0000fc1c
.L_0803a1a0:
	.4byte 0xf014f014
.L_0803a1a4:
	ldr r3, .L_0803a1bc
.L_0803a1a6:
	strh r3, [r5]
	movs r3, #1
	mov r2, r11
	strb r3, [r2, #3]
.L_0803a1ae:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0803a1bc:
	.4byte 0x0000f015

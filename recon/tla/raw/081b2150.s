.syntax unified
	.thumb
	.global Func_081b2150
	.thumb_func
Func_081b2150:
	push {r5, r6, r7, lr}
	ldr r5, .L_081b2188
	ldr r7, .L_081b2184
	movs r6, #0
.L_081b2158:
	ldrh r2, [r5]
	movs r4, #31
	lsls r3, r2, #16
	lsrs r0, r3, #26
	ands r0, r7
	lsrs r1, r3, #21
	ands r1, r7
	ands r4, r2
	subs r0, #1
	subs r1, #1
	subs r4, #1
	cmp r0, #0
	bge .L_081b2174
	movs r0, #0
.L_081b2174:
	cmp r1, #0
	bge .L_081b217a
	movs r1, #0
.L_081b217a:
	cmp r4, #0
	bge .L_081b218c
	movs r4, #0
	b .L_081b218c
	.2byte 0x0000
.L_081b2184:
	.4byte 0x0000001f
.L_081b2188:
	.4byte 0x05000140
.L_081b218c:
	lsls r3, r0, #10
	lsls r2, r1, #5
	orrs r3, r2
	orrs r3, r4
	adds r6, #1
	strh r3, [r5]
	adds r5, #2
	cmp r6, #16
	bne .L_081b2158
	ldr r5, .L_081b21d4
	ldr r7, .L_081b21d0
	movs r6, #0
.L_081b21a4:
	ldrh r2, [r5]
	movs r4, #31
	lsls r3, r2, #16
	lsrs r0, r3, #26
	ands r0, r7
	lsrs r1, r3, #21
	ands r1, r7
	ands r4, r2
	subs r0, #1
	subs r1, #1
	subs r4, #1
	cmp r0, #0
	bge .L_081b21c0
	movs r0, #0
.L_081b21c0:
	cmp r1, #0
	bge .L_081b21c6
	movs r1, #0
.L_081b21c6:
	cmp r4, #0
	bge .L_081b21d8
	movs r4, #0
	b .L_081b21d8
	.2byte 0x0000
.L_081b21d0:
	.4byte 0x0000001f
.L_081b21d4:
	.4byte 0x05000202
.L_081b21d8:
	lsls r3, r0, #10
	lsls r2, r1, #5
	orrs r3, r2
	orrs r3, r4
	adds r6, #1
	strh r3, [r5]
	adds r5, #2
	cmp r6, #239
	bne .L_081b21a4
	pop {r5, r6, r7, pc}

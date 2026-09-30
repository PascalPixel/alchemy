.syntax unified
	.thumb
	.global Func_081c057c
	.thumb_func
Func_081c057c:
	push {lr}
	lsls r0, r0, #16
	ldr r2, .L_081c05a8
	ldr r1, .L_081c05ac
	lsrs r0, r0, #13
	adds r0, r0, r1
	ldrh r3, [r0, #4]
	lsls r1, r3, #1
	adds r1, r1, r3
	lsls r1, r1, #2
	adds r1, r1, r2
	ldr r1, [r1]
	ldr r3, [r1]
	ldr r2, [r0]
	cmp r3, r2
	beq .L_081c05b0
	adds r0, r1, #0
	adds r1, r2, #0
	bl Func_081c0c84
	b .L_081c05c4
	.2byte 0x0000
.L_081c05a8:
	.4byte 0x00000000
.L_081c05ac:
	.4byte 0x00000000
.L_081c05b0:
	ldr r2, [r1, #4]
	ldrh r0, [r1, #4]
	cmp r0, #0
	beq .L_081c05bc
	cmp r2, #0
	bge .L_081c05c4
.L_081c05bc:
	adds r0, r1, #0
	adds r1, r3, #0
	bl Func_081c0c84
.L_081c05c4:
	pop {r0}
	bx r0

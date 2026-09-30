.syntax unified
	.thumb
	.global Func_081a60c0
	.thumb_func
Func_081a60c0:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	ldr r3, [r5]
	movs r1, #0
	sub sp, #4
	cmp r1, r3
	bge .L_081a6144
	ldr r7, .L_081a614c
	ldr r6, .L_081a6150
.L_081a60d2:
	ldr r2, [r5, #20]
	cmp r2, #0
	beq .L_081a60e0
	ldr r3, [r7]
	ands r3, r2
	cmp r3, #0
	bne .L_081a60e8
.L_081a60e0:
	ldrh r3, [r6, #6]
	ldr r2, [r5, #4]
	adds r3, r3, r2
	strh r3, [r6, #6]
.L_081a60e8:
	ldr r2, [r5, #24]
	cmp r2, #0
	beq .L_081a60f6
	ldr r3, [r7]
	ands r3, r2
	cmp r3, #0
	bne .L_081a60fe
.L_081a60f6:
	ldrh r3, [r6, #4]
	ldr r2, [r5, #8]
	adds r3, r3, r2
	strh r3, [r6, #4]
.L_081a60fe:
	ldr r2, [r5, #28]
	cmp r2, #0
	beq .L_081a610c
	ldr r3, [r7]
	ands r3, r2
	cmp r3, #0
	bne .L_081a6114
.L_081a610c:
	ldrh r3, [r6, #10]
	ldr r2, [r5, #12]
	adds r3, r3, r2
	strh r3, [r6, #10]
.L_081a6114:
	ldr r2, [r5, #32]
	cmp r2, #0
	beq .L_081a6122
	ldr r3, [r7]
	ands r3, r2
	cmp r3, #0
	bne .L_081a612a
.L_081a6122:
	ldrh r3, [r6, #8]
	ldr r2, [r5, #16]
	adds r3, r3, r2
	strh r3, [r6, #8]
.L_081a612a:
	movs r0, #1
	str r1, [sp, #0]
	bl Func_081a6094
	ldr r1, [sp, #0]
	cmp r0, #0
	beq .L_081a613c
	movs r0, #0
	b .L_081a6146
.L_081a613c:
	ldr r3, [r5]
	adds r1, #1
	cmp r1, r3
	blt .L_081a60d2
.L_081a6144:
	movs r0, #1
.L_081a6146:
	add sp, #4
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081a614c:
	.4byte Data_0300122c
.L_081a6150:
	.4byte Data_03001120

.syntax unified
	.thumb
	.global Func_081a0a00
	.thumb_func
Func_081a0a00:
	push {r5, r6, r7, lr}
	ldr r5, .L_081a0a60
	ldr r6, .L_081a0a64
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #0
	bne .L_081a0a5e
	ldr r3, .L_081a0a68
	movs r7, #0
	ldrsh r0, [r3, r7]
	ldrh r4, [r3]
	adds r3, r0, #0
	cmp r0, #0
	bge .L_081a0a1e
	adds r3, #15
.L_081a0a1e:
	ldr r1, .L_081a0a6c
	asrs r2, r3, #4
	movs r7, #0
	ldrsh r3, [r1, r7]
	cmp r3, #0
	bge .L_081a0a2c
	adds r3, #15
.L_081a0a2c:
	asrs r3, r3, #4
	cmp r2, r3
	beq .L_081a0a5e
	strh r4, [r1]
	cmp r0, #0
	bge .L_081a0a3a
	adds r0, #7
.L_081a0a3a:
	asrs r0, r0, #3
	lsls r0, r0, #16
	asrs r3, r0, #16
	lsrs r0, r0, #31
	adds r0, r3, r0
	movs r2, #31
	adds r3, #16
	ands r3, r2
	lsls r1, r3, #1
	asrs r0, r0, #1
	adds r1, r1, r3
	adds r0, r0, r6
	subs r0, #1
	lsls r1, r1, #3
	movs r2, #1
	bl Func_081a0be8
	strh r0, [r5]
.L_081a0a5e:
	pop {r5, r6, r7, pc}
.L_081a0a60:
	.4byte Data_02007508
.L_081a0a64:
	.4byte 0x00001189
.L_081a0a68:
	.4byte Data_02007504
.L_081a0a6c:
	.4byte Data_0200750c

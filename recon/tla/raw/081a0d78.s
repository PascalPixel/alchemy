.syntax unified
	.thumb
	.global Func_081a0d78
	.thumb_func
Func_081a0d78:
	push {r5, r6, lr}
	ldr r3, .L_081a0de4
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldrh r0, [r3]
	adds r3, r2, #0
	cmp r2, #0
	bge .L_081a0d8a
	adds r3, r2, #7
.L_081a0d8a:
	ldr r5, .L_081a0de8
	asrs r4, r3, #3
	movs r6, #0
	ldrsh r3, [r5, r6]
	ldrh r1, [r5]
	cmp r3, #0
	beq .L_081a0da6
	adds r3, r1, #1
	strh r3, [r5]
	ldr r3, .L_081a0dec
	movs r2, #31
	ldr r0, [r3]
	adds r3, r4, #0
	b .L_081a0dce
.L_081a0da6:
	ldr r2, .L_081a0df0
	movs r1, #0
	ldrsh r3, [r2, r1]
	cmp r3, #0
	bge .L_081a0db2
	adds r3, #7
.L_081a0db2:
	asrs r3, r3, #3
	cmp r4, r3
	beq .L_081a0e0a
	strh r0, [r2]
	ldr r2, .L_081a0dec
	lsls r3, r4, #2
	ldr r0, [r2, r3]
	cmp r0, #255
	bne .L_081a0df4
	ldr r3, .L_081a0de0
	ldr r0, [r2]
	strh r3, [r5]
	adds r3, r4, #0
	movs r2, #31
.L_081a0dce:
	adds r3, #16
	ands r3, r2
	lsls r1, r3, #1
	adds r1, r1, r3
	lsls r1, r1, #3
	movs r2, #1
	bl Func_081a101c
	b .L_081a0e0a
.L_081a0de0:
	.4byte 0x00000001
.L_081a0de4:
	.4byte Data_02007504
.L_081a0de8:
	.4byte Data_02007508
.L_081a0dec:
	.4byte Data_081a20bc
.L_081a0df0:
	.4byte Data_0200750c
.L_081a0df4:
	adds r3, r4, #0
	movs r2, #31
	adds r3, #16
	ands r3, r2
	lsls r1, r3, #1
	adds r1, r1, r3
	lsls r1, r1, #3
	movs r2, #1
	bl Func_081a101c
	strh r0, [r5]
.L_081a0e0a:
	pop {r5, r6, pc}

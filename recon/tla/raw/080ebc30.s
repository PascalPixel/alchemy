.syntax unified
	.thumb
	.global Func_080ebc30
	.thumb_func
Func_080ebc30:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #69
	movs r3, #0
	ldrsb r3, [r6, r3]
	cmp r3, #0
	beq .L_080ebc92
	ldrh r3, [r5, #56]
	ldrh r2, [r5, #58]
	adds r3, #1
	strh r3, [r5, #56]
	movs r1, #58
	ldrsh r3, [r5, r1]
	cmp r3, #0
	beq .L_080ebc56
	subs r3, r2, #1
	strh r3, [r5, #58]
	b .L_080ebc62
.L_080ebc56:
	ldr r3, [r5, #52]
	cmp r3, #0
	beq .L_080ebc62
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
.L_080ebc62:
	movs r3, #0
	ldrsb r3, [r6, r3]
	cmp r3, #0
	beq .L_080ebc92
	adds r3, r5, #0
	adds r3, #67
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080ebc7e
	adds r0, r5, #0
	bl Func_080ebd24
.L_080ebc7e:
	adds r3, r5, #0
	adds r3, #68
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080ebc92
	adds r0, r5, #0
	bl Func_080ebc94
.L_080ebc92:
	pop {r5, r6, pc}

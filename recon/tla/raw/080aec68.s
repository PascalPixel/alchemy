.syntax unified
	.thumb
	.global Item_GetEquipmentGroup
	.thumb_func
Item_GetEquipmentGroup:
	push {lr}
	bl Item_GetDirect
	ldrb r1, [r0, #2]
	movs r0, #1
	cmp r1, #1
	beq .L_080aeca0
	movs r0, #2
	cmp r1, #2
	beq .L_080aeca0
	cmp r1, #3
	beq .L_080aeca0
	cmp r1, #4
	beq .L_080aeca0
	cmp r1, #5
	beq .L_080aeca0
	cmp r1, #9
	beq .L_080aeca0
	movs r0, #1
	cmp r1, #7
	beq .L_080aeca0
	movs r3, #10
	eors r3, r1
	negs r2, r3
	orrs r2, r3
	lsrs r0, r2, #31
	movs r3, #1
	subs r0, r3, r0
.L_080aeca0:
	pop {pc}
	.2byte 0x0000

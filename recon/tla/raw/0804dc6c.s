.syntax unified
	.thumb
	.global Menu_HandleSelectionRowInput
	.thumb_func
Menu_HandleSelectionRowInput:
	push {r5, r6, r7, lr}
	ldr r6, .L_0804dcd8
	adds r7, r0, #0
	adds r0, r3, #0
	ldr r3, [r6, #12]
	adds r4, r2, #0
	movs r2, #1
	lsls r1, r1, #16
	ands r3, r2
	asrs r5, r1, #16
	cmp r3, #0
	beq .L_0804dc8a
	movs r0, #1
	negs r0, r0
	b .L_0804ddc6
.L_0804dc8a:
	ldr r3, [r6, #12]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0804dc9a
	movs r0, #2
	negs r0, r0
	b .L_0804ddc6
.L_0804dc9a:
	ldr r3, [r6, #12]
	movs r2, #128
	ands r3, r2
	cmp r3, #0
	bne .L_0804dcae
	ldr r1, [r6, #12]
	movs r3, #64
	ands r1, r3
	cmp r1, #0
	beq .L_0804dcb8
.L_0804dcae:
	ldrh r3, [r0]
	ldr r2, .L_0804dcd4
	eors r3, r2
	strh r3, [r0]
	b .L_0804ddc4
.L_0804dcb8:
	ldr r3, [r6, #12]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_0804dcf8
	movs r2, #0
	ldrsh r3, [r0, r2]
	cmp r3, #0
	bne .L_0804dcdc
	adds r3, r5, #1
	lsls r3, r3, #16
	asrs r5, r3, #16
	b .L_0804dcee
	.2byte 0x0000
.L_0804dcd4:
	.4byte 0x00000001
.L_0804dcd8:
	.4byte gInput
.L_0804dcdc:
	ldrh r3, [r4]
	movs r2, #198
	adds r3, #1
	strh r3, [r4]
	lsls r2, r2, #15
	lsls r3, r3, #16
	cmp r3, r2
	ble .L_0804dcee
	strh r1, [r4]
.L_0804dcee:
	movs r3, #162
	lsls r3, r3, #1
	cmp r5, r3
	ble .L_0804dd72
	b .L_0804dd70
.L_0804dcf8:
	ldr r3, [r6, #12]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_0804dd30
	movs r1, #0
	ldrsh r3, [r0, r1]
	cmp r3, #0
	bne .L_0804dd12
	subs r3, r5, #1
	lsls r3, r3, #16
	asrs r5, r3, #16
	b .L_0804dd22
.L_0804dd12:
	ldrh r3, [r4]
	subs r3, #1
	strh r3, [r4]
	lsls r3, r3, #16
	cmp r3, #0
	bge .L_0804dd22
	ldr r3, .L_0804dd2c
	strh r3, [r4]
.L_0804dd22:
	cmp r5, #0
	bge .L_0804dd72
	movs r5, #162
	lsls r5, r5, #1
	b .L_0804dd72
.L_0804dd2c:
	.4byte 0x00000063
.L_0804dd30:
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0804dd7e
	movs r2, #0
	ldrsh r3, [r0, r2]
	cmp r3, #0
	bne .L_0804dd50
	strh r3, [r4]
	adds r3, r5, #0
	adds r3, #10
	lsls r3, r3, #16
	asrs r5, r3, #16
	b .L_0804dd68
.L_0804dd50:
	ldrh r2, [r4]
	movs r1, #198
	adds r3, r2, #0
	adds r3, #10
	strh r3, [r4]
	lsls r1, r1, #15
	lsls r3, r3, #16
	cmp r3, r1
	ble .L_0804dd68
	adds r3, r2, #0
	subs r3, #89
	strh r3, [r4]
.L_0804dd68:
	movs r2, #162
	lsls r2, r2, #1
	cmp r5, r2
	ble .L_0804dd72
.L_0804dd70:
	movs r5, #0
.L_0804dd72:
	adds r0, r7, #0
	adds r1, r5, #0
	adds r2, r4, #0
	bl Menu_DrawSelectionRow
	b .L_0804ddc4
.L_0804dd7e:
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0804ddc4
	movs r1, #0
	ldrsh r3, [r0, r1]
	cmp r3, #0
	bne .L_0804dd9e
	strh r3, [r4]
	adds r3, r5, #0
	subs r3, #10
	lsls r3, r3, #16
	asrs r5, r3, #16
	b .L_0804ddb2
.L_0804dd9e:
	ldrh r2, [r4]
	adds r3, r2, #0
	subs r3, #10
	strh r3, [r4]
	lsls r3, r3, #16
	cmp r3, #0
	bge .L_0804ddb2
	adds r3, r2, #0
	adds r3, #89
	strh r3, [r4]
.L_0804ddb2:
	cmp r5, #0
	bge .L_0804ddba
	movs r5, #162
	lsls r5, r5, #1
.L_0804ddba:
	adds r0, r7, #0
	adds r1, r5, #0
	adds r2, r4, #0
	bl Menu_DrawSelectionRow
.L_0804ddc4:
	adds r0, r5, #0
.L_0804ddc6:
	pop {r5, r6, r7, pc}

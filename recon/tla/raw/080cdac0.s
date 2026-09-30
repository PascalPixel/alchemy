.syntax unified
	.thumb
	.global Func_080cdac0
	.thumb_func
Func_080cdac0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r10, r0
	adds r0, r1, #0
	adds r5, r2, #0
	bl ObjectTable_Get
	adds r7, r0, #0
	adds r0, r5, #0
	bl ObjectTable_Get
	adds r6, r0, #0
	cmp r7, #0
	beq .L_080cdbe0
	cmp r6, #0
	beq .L_080cdbe0
	movs r1, #89
	adds r1, r1, r6
	ldrb r2, [r1]
	movs r3, #8
	ands r3, r2
	mov r9, r1
	cmp r3, #0
	bne .L_080cdbe0
	mov r0, r10
	bl Func_080cda58
	movs r2, #4
	ldrsh r3, [r0, r2]
	mov r8, r0
	lsls r1, r3, #16
	mov r3, r10
	cmp r3, #18
	bne .L_080cdb22
	ldr r0, [r6, #20]
	ldr r3, [r7, #20]
	subs r2, r0, r3
	cmp r2, #0
	blt .L_080cdb1a
.L_080cdb14:
	cmp r2, r1
	bgt .L_080cdbe0
	b .L_080cdb32
.L_080cdb1a:
	subs r3, r3, r0
	cmp r3, r1
	ble .L_080cdb32
	b .L_080cdbe0
.L_080cdb22:
	ldr r0, [r6, #12]
	ldr r3, [r7, #12]
	subs r2, r0, r3
	cmp r2, #0
	bge .L_080cdb14
	subs r3, r3, r0
	cmp r3, r1
	bgt .L_080cdbe0
.L_080cdb32:
	mov r2, r8
	movs r1, #2
	ldrsh r3, [r2, r1]
	adds r5, r3, #1
	mov r3, r9
	ldrb r2, [r3]
	movs r3, #16
	ands r3, r2
	cmp r3, #0
	beq .L_080cdb50
	lsls r3, r5, #1
	adds r3, r3, r5
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r5, r3, #1
.L_080cdb50:
	ldr r2, [r6, #8]
	ldr r3, [r7, #8]
	subs r0, r2, r3
	cmp r0, #0
	bge .L_080cdb62
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r0, r0, r1
.L_080cdb62:
	ldr r2, [r6, #16]
	ldr r3, [r7, #16]
	asrs r0, r0, #16
	subs r2, r2, r3
	cmp r2, #0
	bge .L_080cdb76
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r2, r2, r3
.L_080cdb76:
	asrs r3, r2, #16
	adds r1, r0, #0
	muls r1, r0
	adds r2, r3, #0
	muls r2, r3
	adds r0, r1, #0
	adds r3, r2, #0
	adds r0, r0, r3
	ldr r3, .L_080cdbf4
	mov lr, r3
	.2byte 0xf800
	mov r10, r0
	cmp r10, r5
	bgt .L_080cdbe0
	mov r3, r10
	cmp r3, #11
	ble .L_080cdbe6
	mov r1, r8
	movs r2, #6
	ldrsh r1, [r1, r2]
	mov r8, r1
	cmp r3, #19
	ble .L_080cdbae
	lsls r0, r1, #1
	movs r1, #3
	bl __divsi3
	mov r8, r0
.L_080cdbae:
	ldrh r5, [r7, #6]
	movs r2, #128
	lsls r2, r2, #6
	movs r3, #192
	adds r5, r5, r2
	lsls r3, r3, #8
	ands r5, r3
	ldr r0, [r6, #16]
	ldr r3, [r7, #16]
	ldr r1, [r6, #8]
	subs r0, r0, r3
	ldr r3, [r7, #8]
	subs r1, r1, r3
	bl ArcTan2
	lsls r0, r0, #16
	lsrs r0, r0, #16
	subs r0, r0, r5
	lsls r0, r0, #16
	asrs r0, r0, #16
	cmp r0, #0
	bge .L_080cdbdc
	negs r0, r0
.L_080cdbdc:
	cmp r0, r8
	blt .L_080cdbe6
.L_080cdbe0:
	movs r0, #1
	negs r0, r0
	b .L_080cdbe8
.L_080cdbe6:
	mov r0, r10
.L_080cdbe8:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080cdbf4:
	.4byte IwramFillWords + 0x74

.syntax unified
	.thumb
	.global Func_0803f974
	.thumb_func
Func_0803f974:
	push {r5, lr}
	movs r5, #0
	ldrsh r4, [r0, r5]
	muls r1, r4
	asrs r4, r1, #16
	movs r5, #2
	ldrsh r1, [r0, r5]
	muls r2, r1
	asrs r1, r2, #16
	movs r5, #4
	ldrsh r2, [r0, r5]
	muls r3, r2
	asrs r3, r3, #16
	cmp r4, #0
	bge .L_0803f994
	movs r4, #0
.L_0803f994:
	cmp r4, #31
	ble .L_0803f99a
	movs r4, #31
.L_0803f99a:
	cmp r1, #0
	bge .L_0803f9a0
	movs r1, #0
.L_0803f9a0:
	cmp r1, #31
	ble .L_0803f9a6
	movs r1, #31
.L_0803f9a6:
	cmp r3, #0
	bge .L_0803f9ac
	movs r3, #0
.L_0803f9ac:
	cmp r3, #31
	ble .L_0803f9b2
	movs r3, #31
.L_0803f9b2:
	lsls r2, r1, #5
	lsls r3, r3, #10
	orrs r3, r2
	orrs r4, r3
	lsls r0, r4, #16
	lsrs r0, r0, #16
	pop {r5, pc}

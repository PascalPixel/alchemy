.syntax unified
	.thumb
	.global Func_08128228
	.thumb_func
Func_08128228:
	push {lr}
	adds r2, r0, #0
	adds r2, #13
.L_0812822e:
	ldrb r3, [r0]
	movs r1, #128
	adds r3, #207
	lsls r3, r3, #24
	lsls r1, r1, #20
	cmp r3, r1
	bhi .L_08128242
	ldrb r0, [r0]
	subs r0, #49
	b .L_0812824a
.L_08128242:
	adds r0, #1
	cmp r0, r2
	ble .L_0812822e
	movs r0, #9
.L_0812824a:
	pop {pc}

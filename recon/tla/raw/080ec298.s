.syntax unified
	.thumb
	.global Func_080ec298
	.thumb_func
Func_080ec298:
	push {lr}
	ldr r3, .L_080ec2b4
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_080ec2aa
	ldrb r0, [r0, #1]
	lsrs r0, r0, #4
	b .L_080ec2b0
.L_080ec2aa:
	ldrb r3, [r0, #1]
	movs r0, #15
	ands r0, r3
.L_080ec2b0:
	pop {pc}
	.2byte 0x0000
.L_080ec2b4:
	.4byte Data_0202a642

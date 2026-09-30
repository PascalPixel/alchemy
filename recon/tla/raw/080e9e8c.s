.syntax unified
	.thumb
	.global Func_080e9e8c
	.thumb_func
Func_080e9e8c:
	push {lr}
	ldr r1, [r0, #80]
	adds r0, #100
	ldrh r3, [r0]
	movs r2, #3
	ands r2, r3
	ldr r4, [r1, #40]
	cmp r2, #1
	beq .L_080e9ec0
	cmp r2, #1
	bgt .L_080e9ea8
	cmp r2, #0
	beq .L_080e9eb2
	b .L_080e9eda
.L_080e9ea8:
	cmp r2, #2
	beq .L_080e9ec4
	cmp r2, #3
	beq .L_080e9ed2
	b .L_080e9eda
.L_080e9eb2:
	movs r3, #7
	strb r3, [r4, #5]
	movs r3, #1
	strb r3, [r1, #25]
	movs r3, #2
	strb r3, [r1, #26]
	b .L_080e9eda
.L_080e9ec0:
	movs r3, #0
	b .L_080e9ecc
.L_080e9ec4:
	movs r2, #7
	movs r3, #0
	strb r2, [r4, #5]
	movs r2, #1
.L_080e9ecc:
	strb r2, [r1, #25]
	strb r3, [r1, #26]
	b .L_080e9eda
.L_080e9ed2:
	movs r3, #0
	strb r3, [r4, #5]
	movs r3, #1
	strb r3, [r1, #25]
.L_080e9eda:
	ldrh r3, [r0]
	adds r3, #1
	strh r3, [r0]
	pop {pc}
	.2byte 0x0000

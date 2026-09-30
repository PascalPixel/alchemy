.syntax unified
	.thumb
	.global Func_080d37d4
	.thumb_func
Func_080d37d4:
	push {lr}
	adds r2, r0, #0
	movs r0, #0
	cmp r2, #0
	beq .L_080d3808
	adds r3, r2, #0
	adds r3, #100
	ldrh r3, [r3]
	ldrh r1, [r2, #6]
	subs r3, r3, r1
	lsls r3, r3, #16
	asrs r0, r3, #16
	cmp r0, #0
	beq .L_080d3808
	movs r3, #128
	lsls r3, r3, #5
	cmp r0, r3
	ble .L_080d37fc
	movs r0, #128
	lsls r0, r0, #4
.L_080d37fc:
	ldr r3, .L_080d380c
	cmp r0, r3
	bge .L_080d3804
	ldr r0, .L_080d3810
.L_080d3804:
	adds r3, r1, r0
	strh r3, [r2, #6]
.L_080d3808:
	pop {pc}
	.2byte 0x0000
.L_080d380c:
	.4byte 0xfffff000
.L_080d3810:
	.4byte 0xfffff800

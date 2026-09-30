.syntax unified
	.thumb
	.global Func_0802e7a8
	.thumb_func
Func_0802e7a8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r2, #192
	lsls r2, r2, #18
	ldr r1, [r2, #16]
	sub sp, #12
	ldr r3, [r1, #40]
	mov r8, r1
	ldrb r0, [r3, #4]
	ldr r3, .L_0802e894
	movs r6, #0
	ldr r4, [r3, #4]
	ldr r3, [r3]
	ldr r1, [r2, #36]
	str r3, [sp, #4]
	str r4, [sp, #8]
	cmp r0, #6
	beq .L_0802e812
	cmp r0, #6
	bhi .L_0802e7f2
	cmp r0, #4
	beq .L_0802e812
	cmp r0, #4
	bhi .L_0802e808
	movs r3, #168
	lsls r3, r3, #6
	movs r5, #0
	adds r3, #170
	movs r4, #6
	cmp r0, #3
	beq .L_0802e830
	b .L_0802e826
.L_0802e7f2:
	cmp r0, #20
	beq .L_0802e81e
	cmp r0, #20
	bhi .L_0802e800
	cmp r0, #8
	beq .L_0802e808
	b .L_0802e826
.L_0802e800:
	cmp r0, #44
	beq .L_0802e808
	cmp r0, #88
	bne .L_0802e826
.L_0802e808:
	movs r3, #128
	movs r5, #0
	lsls r3, r3, #6
	movs r4, #8
	b .L_0802e830
.L_0802e812:
	movs r3, #200
	lsls r3, r3, #5
	movs r5, #0
	adds r3, #153
	movs r4, #10
	b .L_0802e830
.L_0802e81e:
	movs r5, #0
	movs r3, #0
	ldr r6, .L_0802e898
	b .L_0802e82e
.L_0802e826:
	movs r5, #128
	movs r3, #128
	lsls r5, r5, #6
	lsls r3, r3, #7
.L_0802e82e:
	movs r4, #4
.L_0802e830:
	movs r7, #0
	cmp r7, r4
	bcs .L_0802e884
	lsls r3, r3, #16
	movs r2, #4
	mov r9, r3
	lsls r3, r6, #16
	add r2, sp
	lsrs r3, r3, #16
	mov r11, r2
	mov r10, r3
	adds r6, r1, #0
.L_0802e848:
	lsls r5, r5, #16
	lsrs r5, r5, #16
	adds r3, r5, #0
	mov r0, r8
	adds r1, r6, #0
	mov r2, r11
	str r4, [sp, #0]
	bl Render_ApplyProjectedPlacement
	movs r3, #56
	mov r1, r9
	add r8, r3
	lsrs r3, r1, #16
	adds r5, r5, r3
	movs r3, #1
	lsls r5, r5, #16
	ands r3, r7
	asrs r5, r5, #16
	ldr r4, [sp, #0]
	cmp r3, #0
	beq .L_0802e87c
	lsls r3, r5, #16
	lsrs r3, r3, #16
	add r3, r10
	lsls r3, r3, #16
	asrs r5, r3, #16
.L_0802e87c:
	adds r7, #1
	adds r6, #16
	cmp r7, r4
	bcc .L_0802e848
.L_0802e884:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0802e894:
	.4byte Battle_FormationPlacementScale
.L_0802e898:
	.4byte 0xffff8000

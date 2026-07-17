@ Reconstructed thumb disassembly of a call-graph-reached code region
@ (code interleaved with its data), verified byte-identical by build_asm.
.syntax unified
	.thumb
	.set sub_080cd594, 0x080cd594
	.set sub_080cdb24, 0x080cdb24
	.set sub_080df9d0, 0x080df9d0
	.set sub_080e0524, 0x080e0524
	.set sub_080e4ade, 0x080e4ade
	.global Region_080e47b8
Region_080e47b8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #184
	ldr r3, [pc, #56]
	str r1, [sp, #96]
	str r0, [sp, #100]
	adds r2, r3, #0
	ldmia r2!, {r0}
	str r0, [sp, #92]
	ldr r2, [r2, #0]
	str r2, [sp, #88]
	adds r2, r3, #0
	subs r2, #108
	ldr r2, [r2, #0]
	str r2, [sp, #80]
	ldr r1, [pc, #36]
	ldr r3, [r3, #8]
	ldr r2, [sp, #100]
	str r3, [sp, #76]
	adds r3, r0, r1
	str r2, [r3, #0]
	ldr r3, [sp, #96]
	cmp r3, #11
	beq .L_080e47fa
	cmp r3, #8
	beq .L_080e47fa
	cmp r3, #32
	bne .L_080e480c
.L_080e47fa:
	movs r0, #0
	bl sub_080cdb24
	b .L_080e4812
	.2byte 0x0000
	.4byte 0x03001eec
	.4byte 0x00007828
.L_080e480c:
	movs r0, #0
	bl sub_080cd594
.L_080e4812:
	ldr r2, [pc, #68]
	ldr r3, [pc, #60]
	strh r3, [r2, #0]
	ldr r1, [sp, #76]
	ldr r0, [pc, #64]
	movs r2, #0
	movs r3, #0
	bl sub_080e0524
	ldr r0, [pc, #56]
	ldr r1, [sp, #92]
	movs r2, #1
	movs r3, #0
	bl sub_080e0524
	ldr r0, [pc, #48]
	ldr r1, [pc, #52]
	movs r2, #1
	movs r3, #0
	bl sub_080e0524
	movs r0, #162
	ldr r5, [sp, #92]
	lsls r0, r0, #7
	movs r3, #144
	adds r1, r5, r0
	lsls r3, r3, #1
	ldr r0, [pc, #28]
	movs r2, #40
	bl sub_080df9d0
	b .L_080e486c
	.2byte 0x0000
	.4byte 0x00001010
	.4byte 0x04000052
	.4byte 0x00000073
	.4byte 0x00000096
	.4byte 0x00000099
	.4byte 0x02010000
.L_080e486c:
	ldr r1, [sp, #96]
	cmp r1, #5
	beq .L_080e4876
	cmp r1, #23
	bne .L_080e487a
.L_080e4876:
	ldr r0, [pc, #644]
	b .L_080e48f0
.L_080e487a:
	ldr r2, [sp, #96]
	cmp r2, #12
	bne .L_080e4884
	ldr r0, [pc, #636]
	b .L_080e48f0
.L_080e4884:
	ldr r3, [sp, #96]
	cmp r3, #6
	beq .L_080e488e
	cmp r3, #27
	bne .L_080e48a0
.L_080e488e:
	ldr r0, [pc, #628]
	ldr r1, [pc, #628]
	movs r2, #1
	movs r3, #0
	bl sub_080e0524
	ldr r0, [pc, #624]
	ldr r1, [pc, #624]
	b .L_080e48f2
.L_080e48a0:
	ldr r5, [sp, #96]
	cmp r5, #31
	bne .L_080e48aa
	ldr r0, [pc, #620]
	b .L_080e48b2
.L_080e48aa:
	ldr r0, [sp, #96]
	cmp r0, #8
	bne .L_080e48be
	ldr r0, [pc, #612]
.L_080e48b2:
	ldr r1, [pc, #596]
	movs r2, #1
	movs r3, #1
	bl sub_080e0524
	b .L_080e4912
.L_080e48be:
	ldr r1, [sp, #96]
	cmp r1, #14
	bne .L_080e48c8
	ldr r0, [pc, #596]
	b .L_080e48f0
.L_080e48c8:
	ldr r2, [sp, #96]
	cmp r2, #30
	bne .L_080e48d2
	ldr r0, [pc, #564]
	b .L_080e48f0
.L_080e48d2:
	ldr r3, [sp, #96]
	cmp r3, #16
	bne .L_080e48dc
	ldr r0, [pc, #580]
	b .L_080e48f0
.L_080e48dc:
	ldr r5, [sp, #96]
	cmp r5, #20
	bne .L_080e48e6
	ldr r0, [pc, #576]
	b .L_080e48f0
.L_080e48e6:
	ldr r3, [sp, #96]
	subs r3, #33
	cmp r3, #1
	bhi .L_080e48fc
	ldr r0, [pc, #568]
.L_080e48f0:
	ldr r1, [pc, #532]
.L_080e48f2:
	movs r2, #1
	movs r3, #0
	bl sub_080e0524
	b .L_080e4912
.L_080e48fc:
	ldr r0, [sp, #96]
	cmp r0, #11
	beq .L_080e4912
	cmp r0, #32
	beq .L_080e4912
	ldr r0, [pc, #548]
	ldr r1, [pc, #508]
	movs r2, #1
	movs r3, #0
	bl sub_080e0524
.L_080e4912:
	ldr r1, [sp, #96]
	cmp r1, #100
	bls .L_080e491a
	b sub_080e4ade
.L_080e491a:
	ldr r2, [pc, #532]
	lsls r3, r1, #2
	ldr r3, [r3, r2]
	mov pc, r3

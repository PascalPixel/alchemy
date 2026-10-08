@ C2: unpaired multiply halves. Seven of its thirteen smull keep both
@ halves outside a register pair: smull r1, r3 (three times), smull
@ r2, r0 and smull r1, r0 (twice), reversed or apart, and smull r2, r3,
@ r3, r2 over both its inputs. agbcc_arm and agscc keep both halves only
@ through mulsidi3 (smull %Q0, %R0: RdHi = RdLo + 1, "=&r" apart from the
@ inputs); smulsi3_highpart frees RdLo but discards it. Through the game
@ ARM route (K1) the s64 form pairs every smull (r5, r6; r7, r8; r1, r2;
@ r0, r1): differ 448 of the listing's 307 instructions with UPDFAR.S's
@ veneers (6 with the inline asm), 456 with every device removed.
	.syntax unified
	.arm

	.set	OBJ_SCRIPT, 0x00
	.set	OBJ_CURSOR, 0x04
	.set	OBJ_FACING, 0x06
	.set	OBJ_X, 0x08
	.set	OBJ_Y, 0x0c
	.set	OBJ_Z, 0x10
	.set	OBJ_GROUND, 0x14
	.set	OBJ_VX, 0x24
	.set	OBJ_VY, 0x28
	.set	OBJ_VZ, 0x2c
	.set	OBJ_SPEED_LIMIT, 0x30
	.set	OBJ_ACCEL, 0x34
	.set	OBJ_TARGET_X, 0x38
	.set	OBJ_TARGET_Y, 0x3c
	.set	OBJ_TARGET_Z, 0x40
	.set	OBJ_BOUNCE, 0x44
	.set	OBJ_GRAVITY, 0x48
	.set	OBJ_MOTION_FLAGS, 0x55
	.set	OBJ_ARRIVAL_AXIS, 0x56
	.set	OBJ_SNAP, 0x58
	.set	OBJ_STEERING, 0x5a
	.set	OBJ_PAUSED, 0x5b
	.set	OBJ_WAIT, 0x5e
	.set	OBJ_FROZEN, 0x61
	.set	OBJ_HOOK, 0x6c
	.set	OBJ_SIZE, 0x70

	.set	OBJ_SLOTS, 8
	.set	TARGET_UNSET, 1 << 31
	.set	TURN_MAX, 0x1000

@ sp+0 次の番号, sp+4 次の物体, sp+8 到着, sp+12 y
	.global	Object_UpdateAll
	.type	Object_UpdateAll, %function
Object_UpdateAll:
	push	{r5, r6, r7, r8, r9, sl, lr}
	mov	r2, #0
	ldr	r3, .L_gObjectSlots
	sub	sp, sp, #16
	ldr	r5, [r3]
.L_slot:
	ldr	r3, [r5, #OBJ_SCRIPT]
	add	r2, r2, #1
	str	r2, [sp]
	add	r1, r5, #OBJ_SIZE
	str	r1, [sp, #4]
	cmp	r3, #0
	beq	.L_next
	ldr	r3, [r5, #OBJ_HOOK]
	cmp	r3, #0
	beq	.L_hooked
	mov	r0, r5
	mov	lr, pc
	bx	r3
.L_hooked:
	ldr	r3, [r5, #OBJ_SCRIPT]
	cmp	r3, #0
	beq	.L_next
	ldrb	r3, [r5, #OBJ_PAUSED]
	cmp	r3, #0
	bne	.L_next
	ldrsh	r3, [r5, #OBJ_WAIT]			@ 待ち
	cmp	r3, #0
	ldrh	r3, [r5, #OBJ_WAIT]
	subne	r3, r3, #1
	strhne	r3, [r5, #OBJ_WAIT]
	bne	.L_move
	ldr	r6, .L_ScriptObject_CommandTable
	b	.L_command
.L_skip:
	ldrh	r3, [r5, #OBJ_CURSOR]
	add	r3, r3, #1
	strh	r3, [r5, #OBJ_CURSOR]
.L_command:
	ldrsh	r2, [r5, #OBJ_CURSOR]
	ldr	r3, [r5, #OBJ_SCRIPT]
	ldr	r3, [r3, r2, lsl #2]
	cmp	r3, #63
	bhi	.L_skip
	mov	r0, r5
	ldr	ip, [r6, r3, lsl #2]
	mov	lr, pc
	bx	ip
	cmp	r0, #0
	bne	.L_command
	ldr	r3, [r5, #OBJ_SCRIPT]
	cmp	r3, #0
	beq	.L_next
.L_move:
	ldr	r3, [r5, #OBJ_Y]
	ldr	r9, [r5, #OBJ_X]
	str	r3, [sp, #12]
	ldrb	r3, [r5, #OBJ_FROZEN]
	cmp	r3, #0
	ldr	sl, [r5, #OBJ_Z]
	bne	.L_step
	ldr	r2, [r5, #OBJ_TARGET_X]
	str	r3, [sp, #8]
	cmp	r2, #TARGET_UNSET
	beq	.L_coast

@ 目標へ
	rsb	r2, r9, r2
	add	r3, r2, #0xff00
	add	r3, r3, #0xff
	cmp	r2, #0
	ldr	r1, [r5, #OBJ_TARGET_Z]
	asrge	r6, r2, #16
	asrlt	r6, r3, #16
	rsb	r1, sl, r1
	add	r3, r1, #0xff00
	add	r3, r3, #0xff
	cmp	r1, #0
	asrge	r7, r1, #16
	asrlt	r7, r3, #16
	mul	r2, r7, r7
	mla	r0, r6, r6, r2
	ldr	r1, .L_IwramSqrt
	mov	lr, pc
	bx	r1
	lsl	r0, r0, #16
	cmp	r0, #0x1000000
	bge	.L_length
	ldr	r3, [r5, #OBJ_TARGET_X]
	ldr	r2, [r5, #OBJ_TARGET_Z]
	rsb	r6, r9, r3
	rsb	r7, sl, r2
	smull	r1, r3, r6, r6
	lsl	r3, r3, #16
	orr	r3, r3, r1, lsr #16
	smull	r2, r0, r7, r7
	lsl	r0, r0, #16
	orr	r0, r0, r2, lsr #16
	add	r0, r3, r0
	ldr	r3, .L_IwramSqrt
	mov	lr, pc
	bx	r3
	lsl	r0, r0, #8
.L_length:
	cmp	r0, #0
	ldreq	r9, [r5, #OBJ_TARGET_X]
	ldreq	sl, [r5, #OBJ_TARGET_Z]
	beq	.L_fall
	ldr	r1, [r5, #OBJ_ACCEL]			@ 加速
	ldr	r3, .L_IwramRatioMulQ14
	mov	lr, pc
	bx	r3
	smull	r1, r3, r0, r6
	lsl	r3, r3, #16
	orr	r3, r3, r1, lsr #16
	ldr	r2, [r5, #OBJ_VX]
	add	r8, r2, r3
	str	r8, [r5, #OBJ_VX]
	smull	r1, r3, r0, r7
	lsl	r3, r3, #16
	orr	r3, r3, r1, lsr #16
	ldr	r2, [r5, #OBJ_VZ]
	add	r6, r2, r3
	str	r6, [r5, #OBJ_VZ]
	smull	r2, r3, r8, r8
	lsl	r3, r3, #16
	orr	r3, r3, r2, lsr #16
	smull	r1, r0, r6, r6
	lsl	r0, r0, #16
	orr	r0, r0, r1, lsr #16
	ldr	r1, .L_IwramSqrt
	add	r0, r3, r0
	mov	lr, pc
	bx	r1
	ldr	r1, [r5, #OBJ_SPEED_LIMIT]		@ 最高速度
	lsl	r0, r0, #8
	cmp	r0, r1
	ble	.L_fall
	ldr	r3, .L_IwramRatioMulQ14
	mov	lr, pc
	bx	r3
	smull	r2, r3, r0, r8
	lsl	r3, r3, #16
	orr	r3, r3, r2, lsr #16
	str	r3, [r5, #OBJ_VX]
	smull	r1, r2, r0, r6
	b	.L_store_vz

@ 減速
.L_coast:
	ldr	r6, [r5, #OBJ_VX]
	ldr	r7, [r5, #OBJ_VZ]
	smull	r2, r3, r6, r6
	lsl	r3, r3, #16
	orr	r3, r3, r2, lsr #16
	smull	r1, r0, r7, r7
	lsl	r0, r0, #16
	orr	r0, r0, r1, lsr #16
	add	r0, r3, r0
	bl	IwramSqrtFarArm
	lsls	r0, r0, #8
	beq	.L_halt
	ldr	r3, [r5, #OBJ_ACCEL]
	subs	r1, r0, r3
	ldrmi	r1, [sp, #8]
	strmi	r1, [r5, #OBJ_VX]
	strmi	r1, [r5, #OBJ_VZ]
	bmi	.L_fall
	bl	IwramRatioMulQ14FarArm
	smull	r2, r3, r0, r6
	lsl	r3, r3, #16
	orr	r3, r3, r2, lsr #16
	str	r3, [r5, #OBJ_VX]
	smull	r1, r2, r0, r7
.L_store_vz:
	lsl	r2, r2, #16
	orr	r2, r2, r1, lsr #16
	str	r2, [r5, #OBJ_VZ]
	b	.L_fall
.L_halt:
	ldr	r3, [sp, #8]
	str	r3, [r5, #OBJ_VX]
	str	r3, [r5, #OBJ_VZ]

@ 重力
.L_fall:
	ldrb	r3, [r5, #OBJ_MOTION_FLAGS]
	tst	r3, #2
	beq	.L_step
	ldr	r1, [r5, #OBJ_GROUND]			@ 地面
	ldr	r3, [sp, #12]
	cmp	r3, r1
	ble	.L_ground
	ldr	r3, [r5, #OBJ_VY]
	ldr	r2, [r5, #OBJ_GRAVITY]
	rsb	r3, r2, r3
	str	r3, [r5, #OBJ_VY]
	bgt	.L_step
.L_ground:
	ldr	r2, [r5, #OBJ_VY]
	cmp	r2, #0
	bge	.L_step
	str	r1, [sp, #12]
	ldr	r3, [r5, #OBJ_BOUNCE]			@ 跳ね返り
	smull	r2, r3, r3, r2
	lsl	r3, r3, #16
	orr	r3, r3, r2, lsr #16
	rsb	r3, r3, #0
	str	r3, [r5, #OBJ_VY]
	ldr	r2, [r5, #OBJ_GRAVITY]
	cmp	r3, #0
	rsblt	r3, r3, #0
	cmp	r3, r2
	movle	r3, #0
	strle	r3, [r5, #OBJ_VY]

@ 移動
.L_step:
	ldr	r3, [r5, #OBJ_VX]
	ldr	r2, [r5, #OBJ_VY]
	add	r9, r9, r3
	ldr	r3, [sp, #12]
	ldr	r1, [r5, #OBJ_VZ]
	add	r3, r3, r2
	str	r3, [sp, #12]
	ldrb	r3, [r5, #OBJ_ARRIVAL_AXIS]
	add	sl, sl, r1
	cmp	r3, #17
	beq	.L_axis_y
	bgt	.L_axis_high
	cmp	r3, #16
	beq	.L_axis_x
	b	.L_arrived
.L_axis_high:
	cmp	r3, #18
	beq	.L_axis_z
	b	.L_arrived
.L_axis_x:
	ldr	r2, [r5, #OBJ_TARGET_X]
	cmp	r9, r2
	beq	.L_reach
	ldr	r3, [r5, #OBJ_X]
	rsb	r3, r2, r3
	rsb	r2, r2, r9
	b	.L_crossed
.L_axis_y:
	ldr	r2, [r5, #OBJ_TARGET_Y]
	ldr	r3, [sp, #12]
	cmp	r3, r2
	beq	.L_reach_y
	ldr	r3, [r5, #OBJ_Y]
	ldr	r1, [sp, #12]
	rsb	r3, r2, r3
	rsb	r2, r2, r1
	teq	r3, r2
	bpl	.L_arrived
.L_reach_y:
	mov	r3, #1
	str	r3, [sp, #8]
	b	.L_arrived

	.align	2
.L_gObjectSlots:
	.4byte	gObjectSlots
.L_ScriptObject_CommandTable:
	.4byte	ScriptObject_CommandTable
.L_IwramSqrt:
	.4byte	IwramSqrt
.L_IwramRatioMulQ14:
	.4byte	IwramRatioMulQ14

.L_axis_z:
	ldr	r2, [r5, #OBJ_TARGET_Z]
	cmp	sl, r2
	beq	.L_reach
	ldr	r3, [r5, #OBJ_Z]
	rsb	r3, r2, r3
	rsb	r2, r2, sl
.L_crossed:
	teq	r3, r2
	bpl	.L_arrived
.L_reach:
	mov	r1, #1
	str	r1, [sp, #8]

@ 到着
.L_arrived:
	ldr	r3, [sp, #8]
	cmp	r3, #0
	beq	.L_place
	ldrb	r3, [r5, #OBJ_SNAP]
	cmp	r3, #0
	beq	.L_unset
	ldr	r9, [r5, #OBJ_TARGET_X]
	mov	r3, #0
	str	r3, [r5, #OBJ_VX]
	ldr	sl, [r5, #OBJ_TARGET_Z]
	ldrb	r2, [r5, #OBJ_MOTION_FLAGS]
	str	r3, [r5, #OBJ_VZ]
	cmp	r2, r3
	ldreq	r1, [r5, #OBJ_TARGET_Y]
	streq	r1, [sp, #12]
	streq	r2, [r5, #OBJ_VY]
.L_unset:
	mov	r3, #TARGET_UNSET
	str	r3, [r5, #OBJ_TARGET_X]
	str	r3, [r5, #OBJ_TARGET_Y]
	str	r3, [r5, #OBJ_TARGET_Z]
	mov	r2, #0
	strb	r2, [r5, #OBJ_ARRIVAL_AXIS]
.L_place:
	str	r9, [r5, #OBJ_X]
	ldr	r3, [sp, #12]
	str	r3, [r5, #OBJ_Y]
	ldrb	r3, [r5, #OBJ_STEERING]
	str	sl, [r5, #OBJ_Z]
	tst	r3, #1
	beq	.L_next

@ 向き
	ldr	r9, [r5, #OBJ_VX]
	ldr	sl, [r5, #OBJ_VZ]
	cmp	sl, #0
	cmpeq	r9, #0
	beq	.L_next
	mov	r0, sl
	mov	r1, r9
	bl	ArcTan2FarArm
	lsl	r0, r0, #16
	ldrh	r3, [r5, #OBJ_FACING]
	mov	r2, #1 << 31
	rsb	r0, r3, r0, lsr #16
	lsl	r0, r0, #16
	asr	r0, r0, #16
	cmp	r0, #TURN_MAX
	movge	r0, #TURN_MAX
	cmn	r0, #TURN_MAX
	asrlt	r0, r2, #19
	add	r3, r3, r0
	strh	r3, [r5, #OBJ_FACING]
.L_next:
	ldm	sp, {r2, r5}
	cmp	r2, #OBJ_SLOTS - 1
	ble	.L_slot
	add	sp, sp, #16
	pop	{r5, r6, r7, r8, r9, sl, lr}
	bx	lr
	.size	Object_UpdateAll, . - Object_UpdateAll

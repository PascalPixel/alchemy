# Thumb store-multiple: derived, measured, and not admitted

Stock gcc 2.96 merges N consecutive SImode stores from ascending registers into
one STM. Three `define_peephole`s in `arm.md` call `store_multiple_sequence`,
and all three are gated `TARGET_ARM`. In Thumb the only code that reaches
`STMIA` is `thumb_output_move_mem_multiple`, the block move, which **always**
pairs it with an `LDMIA`.

The reference build emits a bare `stmia r3!, {r0, r1, r2}` followed by
`subs r3, #12`, with no `LDMIA` near it (checked on `08002fb0`, `08011568`,
`080b010c`). So the instruction is **structurally unreachable from any C** under
stock gcc 2.96. That is settled: it is not a matter of trying harder in C.

## The implementation

The patch below is the Thumb form of the same transformation. It reuses stock's
`store_multiple_sequence` unchanged and adds only what Thumb-1 requires: lowest
offset 0 (STMIA is the only Thumb store-multiple), low registers only, a base
that is not itself in the transfer list, and mandatory writeback -- which is why
the base is put back with a `SUB`.

It works. On a three-field struct store it emits, byte for byte, the form the
reference uses:

    c00e  stmia r0!, {r1, r2, r3}
    380c  subs  r0, #12

## Why it is not in the compiler

Measured across all 1,379 main-image owners and the 104 parked ones:

| | |
|---|---|
| owners gained | **0** |
| owners lost | **1** |

Zero gained because all 37 parked owners that want this merge assign their
values to **descending** registers -- `08011568` stores `r1` to `+0` and `r0` to
`+4` -- and the sequence checker correctly declines. Register assignment follows
evaluation order, which is a C shape problem this change does not touch. Opening
the gate is necessary and nowhere near sufficient.

The loss matters more. `08005258` has the canonical mergeable sequence --
ascending `r0`/`r1`/`r2` into `+0`/`+4`/`+8` -- and **the reference does not
merge it**, emitting three separate `STR`s. So "Camelot enabled this peephole
for Thumb" is refuted as stated; something else conditions the merge.
`store_multiple_sequence` already refuses volatile MEMs, and `08005258` writes
three words to a fixed IWRAM address, which makes `volatile` the obvious
candidate. Adding `volatile` to that C to rescue a compiler theory is exactly
the corpus-tuned reasoning this repository spent 15,097 deleted lines learning
not to do.

37 owners want the merge and 1 refuses it, so this is most likely right and
incompletely derived rather than wrong.

## What would admit it

1. The `08005258` counterexample **explained**, not accommodated.
2. At least one owner whose C already produces ascending registers, so the
   change gains something the day it lands.

## The patch

```diff
diff --git a/gcc-2.96/gcc/config/arm/arm-protos.h b/gcc-2.96/gcc/config/arm/arm-protos.h
index 7bdb65b..61f1806 100644
--- a/gcc-2.96/gcc/config/arm/arm-protos.h
+++ b/gcc-2.96/gcc/config/arm/arm-protos.h
@@ -96,6 +96,8 @@ extern int    load_multiple_sequence	PARAMS ((rtx *, int, int *, int *,
 extern const char * emit_ldm_seq	PARAMS ((rtx *, int));
 extern int    store_multiple_sequence	PARAMS ((rtx *, int, int *, int *,
 						HOST_WIDE_INT *));
+extern int    thumb_store_multiple_sequence PARAMS ((rtx *, int, int *, int *));
+extern const char * thumb_output_store_multiple PARAMS ((rtx *, int));
 extern const char * emit_stm_seq	PARAMS ((rtx *, int));
 extern rtx    arm_gen_load_multiple	PARAMS ((int, int, rtx, int, int, int,
 						int, int));
diff --git a/gcc-2.96/gcc/config/arm/arm.c b/gcc-2.96/gcc/config/arm/arm.c
index 5d83e4b..6b250cd 100644
--- a/gcc-2.96/gcc/config/arm/arm.c
+++ b/gcc-2.96/gcc/config/arm/arm.c
@@ -3670,6 +3670,115 @@ store_multiple_sequence (operands, nops, regs, base, load_offset)
   return 0;
 }
 
+/* Thumb store-multiple.
+
+   Stock gcc merges N consecutive SImode stores from ascending registers to
+   ascending adjacent addresses into a single STM -- see the define_peephole
+   group in arm.md that calls store_multiple_sequence.  Those peepholes are
+   gated TARGET_ARM, so in Thumb the same three stores stay three STRs, and
+   the only Thumb code that ever reaches STMIA is the block move in
+   thumb_output_move_mem_multiple, which always pairs it with an LDMIA.
+
+   The reference build emits a BARE `stmia rB!, {r0, r1, r2}' followed by a
+   `sub rB, rB, #12' for the DMA descriptor stores -- no LDMIA anywhere near
+   it.  Nothing stock gcc 2.96 can be asked to do produces that, from any C,
+   so this is the reference compiler's behaviour and belongs here
+   unconditionally, with no option to select it.
+
+   This is deliberately the SAME transformation stock already implements,
+   reusing its sequence checker unchanged, plus the four things Thumb-1
+   requires and ARM does not:
+
+     * only STMIA exists, so the lowest offset must be 0 (return value 1);
+     * every stored register must be a low register;
+     * the base register must be a low register;
+     * writeback is mandatory, and the base must not itself be in the stored
+       list, which would make the instruction UNPREDICTABLE.
+
+   Because writeback is mandatory, the base advances by 4 * nops and is put
+   back with a SUB, which is exactly the pair the reference emits.  */
+int
+thumb_store_multiple_sequence (operands, nops, regs, base)
+     rtx * operands;
+     int nops;
+     int * regs;
+     int * base;
+{
+  HOST_WIDE_INT offset;
+  int local_regs[4];
+  int base_reg;
+  int i;
+
+  if (! TARGET_THUMB)
+    return 0;
+
+  /* The peephole CONDITION calls this with NULL outputs, exactly as the ARM
+     peepholes call store_multiple_sequence; the output emitter then calls it
+     again to collect them. So always sort into a local array and copy out
+     only when asked.  */
+  if (store_multiple_sequence (operands, nops, local_regs, &base_reg, &offset)
+      != 1)
+    return 0;
+
+  if (base_reg > LAST_LO_REGNUM)
+    return 0;
+
+  for (i = 0; i < nops; i++)
+    {
+      if (local_regs[i] > LAST_LO_REGNUM)
+	return 0;
+
+      /* STMIA always writes back in Thumb; a base inside the transfer list
+	 is UNPREDICTABLE.  */
+      if (local_regs[i] == base_reg)
+	return 0;
+    }
+
+  /* The base is restored with SUB Rd, #imm8.  */
+  if (4 * nops > 255)
+    return 0;
+
+  if (base)
+    *base = base_reg;
+  if (regs)
+    for (i = 0; i < nops; i++)
+      regs[i] = local_regs[i];
+
+  return 1;
+}
+
+/* Print the two instructions thumb_store_multiple_sequence approved.  */
+const char *
+thumb_output_store_multiple (operands, nops)
+     rtx * operands;
+     int nops;
+{
+  char pattern[100];
+  int regs[4];
+  int base;
+  int i;
+
+  if (! thumb_store_multiple_sequence (operands, nops, regs, &base))
+    abort ();
+
+  sprintf (pattern, "stmia\t%s%s!, {", REGISTER_PREFIX, reg_names[base]);
+  for (i = 0; i < nops; i++)
+    {
+      if (i != 0)
+	strcat (pattern, ", ");
+      strcat (pattern, REGISTER_PREFIX);
+      strcat (pattern, reg_names[regs[i]]);
+    }
+  strcat (pattern, "}");
+  output_asm_insn (pattern, operands);
+
+  sprintf (pattern, "sub\t%s%s, %s%s, #%d", REGISTER_PREFIX, reg_names[base],
+	   REGISTER_PREFIX, reg_names[base], 4 * nops);
+  output_asm_insn (pattern, operands);
+
+  return "";
+}
+
 const char *
 emit_stm_seq (operands, nops)
      rtx * operands;
diff --git a/gcc-2.96/gcc/config/arm/arm.md b/gcc-2.96/gcc/config/arm/arm.md
index 5d39953..351b32d 100644
--- a/gcc-2.96/gcc/config/arm/arm.md
+++ b/gcc-2.96/gcc/config/arm/arm.md
@@ -7868,6 +7868,27 @@
   return emit_stm_seq (operands, 2);
 ")
 
+;; The Thumb form of the three store-multiple peepholes above. Same
+;; transformation, same sequence checker; thumb_store_multiple_sequence adds
+;; only what Thumb-1 requires -- low registers, offset 0, and a base that is
+;; not itself in the transfer list. Writeback is mandatory in Thumb, so the
+;; base is put back with a SUB, which is the pair the reference build emits.
+;;
+;; Length is 4: two 2-byte Thumb instructions replacing three 2-byte stores.
+
+(define_peephole
+  [(set (match_operand:SI 3 "memory_operand" "=m")
+        (match_operand:SI 0 "s_register_operand" "l"))
+   (set (match_operand:SI 4 "memory_operand" "=m")
+        (match_operand:SI 1 "s_register_operand" "l"))
+   (set (match_operand:SI 5 "memory_operand" "=m")
+        (match_operand:SI 2 "s_register_operand" "l"))]
+  "TARGET_THUMB && thumb_store_multiple_sequence (operands, 3, NULL, NULL)"
+  "* return thumb_output_store_multiple (operands, 3);"
+  [(set_attr "length" "4")
+   (set_attr "type" "store3")]
+)
+
 (define_split
   [(set (match_operand:SI 0 "s_register_operand" "")
 	(and:SI (ge:SI (match_operand:SI 1 "s_register_operand" "")
```

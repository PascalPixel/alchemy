//! `selfTest()` from `tools/lib/alchemy_gcc.ts`, ported.
//!
//! WHAT IT IS. Not a unit test: an executable statement of the compiler routing
//! evidence. Every block below pins one hard-won finding -- that this source
//! needs `old_agbcc`, that this one needs `-mgrouped-dma-store` and its
//! four-byte neighbour must not get it, that a forced family rejects gs2. The
//! routing tables are thousands of entries wide and every entry was paid for by
//! a byte-diff hunt, so the failure this guards against is a well-meaning
//! "cleanup" silently widening or narrowing a key.
//!
//! HOW THIS PORT DIFFERS IN SHAPE ONLY. The TypeScript throws on the first
//! failure and prints one summary line. This returns `Err(message)` with the
//! identical message text, and additionally records every primitive call it
//! makes into a [`Trace`] so the differential harness has a corpus derived from
//! the body rather than hand-listed beside it. The recording is a side channel;
//! remove it and the control flow is the original's, statement for statement.

use alchemy_plan::plan::{
    direct_compiler_command, source_to_assembly_plan, CompilerFamily, CompilerFlagMutations,
    ResolvedFamily, SourceToAssemblyPlan, SourceToAssemblyPlanOptions, StepKind,
};
use alchemy_routing::routing::{agbcc_cflags, cflags_for_target_source, uses_agbcc_compiler, root, CompilerTarget};
use alchemy_routing::routing_data::{
    AGBCC_SOURCES, CALL_ARG0_MOVE_FIRST_OVERLAY_SOURCES, GROUPED_DMA_STORE_SOURCES,
    HIGH_MOVE_BEFORE_STACK_STORE_SOURCES, THUMB_IMMEDIATE_LATENCY_SOURCES,
};

use crate::jsstring::{neighbour_stem, sorted_set};
use crate::lint::callback_arity_lint;
use crate::trace::{
    cflags_key, direct_key, join_unit, outcome, plan_key, plan_payload, uses_agbcc_key, Trace,
};

const GS1: CompilerTarget = CompilerTarget::Gs1;
const GS2: CompilerTarget = CompilerTarget::Gs2;

/// The counters the TypeScript prints on success.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Summary {
    pub agbcc_sources: usize,
    pub grouped_dma_sources: usize,
    pub overlay_call_arg_sources: usize,
}

impl Summary {
    /// The exact success line, byte for byte.
    pub fn line(&self) -> String {
        format!(
            "self-test=ok agbcc_sources={} grouped_dma_sources={} overlay_call_arg_sources={}",
            self.agbcc_sources, self.grouped_dma_sources, self.overlay_call_arg_sources
        )
    }
}

/// The recording wrapper around the four primitives `selfTest` calls.
struct Probe {
    trace: Trace,
}

impl Probe {
    fn cflags(&mut self, target: CompilerTarget, source: &str) -> Vec<String> {
        let flags = cflags_for_target_source(target, source);
        self.trace
            .push(cflags_key(target, source), join_unit(&flags));
        flags
    }

    fn uses_agbcc(&mut self, target: CompilerTarget, source: &str) -> bool {
        let value = uses_agbcc_compiler(target, source);
        self.trace
            .push(uses_agbcc_key(target, source), value.to_string());
        value
    }

    fn direct(
        &mut self,
        input: &str,
        output: &str,
        dumpbase: &str,
        source: Option<&str>,
    ) -> Result<Vec<String>, String> {
        let result = direct_compiler_command(input, output, dumpbase, source);
        self.trace
            .push(direct_key(input, output, dumpbase, source), outcome(&result));
        result
    }

    fn plan(
        &mut self,
        options: &SourceToAssemblyPlanOptions,
    ) -> Result<SourceToAssemblyPlan, String> {
        let result = source_to_assembly_plan(options);
        let value = match &result {
            Ok(plan) => format!("ok\t{}", plan_payload(plan)),
            Err(message) => format!("err\t{message}"),
        };
        self.trace.push(plan_key(options), value);
        result
    }
}

/// `arr.includes(x)`.
fn has(list: &[String], flag: &str) -> bool {
    list.iter().any(|item| item == flag)
}

/// `join(ROOT, relative)`.
fn from_root(relative: &str) -> String {
    root().join(relative).to_string_lossy().into_owned()
}

/// `step.command.at(-n)`.
///
/// PORT NOTE: `at(-n)` returns `undefined` past the start of the array, where
/// Rust indexing panics. Every call here is on an argv that is long enough, but
/// the `Option` keeps the failure shape identical if that ever stops being true.
fn at_back(command: &[String], back: usize) -> Option<&str> {
    command
        .len()
        .checked_sub(back)
        .and_then(|index| command.get(index))
        .map(String::as_str)
}

/// The `throw new Error(...)` message text, verbatim.
fn fail<T>(message: impl Into<String>) -> Result<T, String> {
    Err(message.into())
}

/// `selfTest()`.
pub fn self_test() -> Result<(Summary, Trace), String> {
    let mut probe = Probe { trace: Trace::new() };
    callback_arity_lint()?;

    let expected: [&str; 47] = [
        "080069a4",
        "08006a00", "08006ba8", "08006c24", "08006c68", "08006cdc", "08006d50", "08006dec",
        "08006e24", "08006f48", "08006f84", "08007028", "08007098", "0800711c", "080071a8", "08007220",
        "080f9a50",
        "080fa1fc", "080fa2a0", "080fa324", "080fa350", "080fa39c", "080fa3f0",
        "080fa424", "080fa458", "080fa490", "080fa514", "080fa55c", "080fa6a0", "080fa83c", "080fa8d4", "080fa928", "080fa9a4",
        "080fa9e0", "080fab3c", "080fab7c", "080fac44", "080facf8", "080fada0", "080fadf0", "080fb2cc", "080fb334", "080fb3a8", "080fb430", "080fb4a4",
        "080fb670",
        "080fb6a4",
    ];
    if sorted_set(AGBCC_SOURCES) != expected {
        return fail("old_agbcc source allowlist self-test failed");
    }
    // PORT NOTE: the TypeScript compares `JSON.stringify` of two arrays of
    // strings. That is an exact elementwise comparison for this data (no
    // numbers, no `undefined`, no key order), so `==` on `Vec<String>` is the
    // same predicate without dragging a JSON writer -- and its float formatting
    // trap -- into a test that has no numbers in it.
    let agbcc_o1 = [
        "08006a00", "08006ba8", "08006c68", "08006cdc", "08006d50", "08006e24",
        "08006f84", "08007028", "08007098", "0800711c", "080071a8",
        "08007220",
    ];
    let agbcc_prologue = ["080fb2cc", "080fb334", "080fb3a8"];
    for stem in expected {
        let source = format!("/tmp/{stem}.c");
        if !probe.uses_agbcc(GS1, &source) || probe.uses_agbcc(GS2, &source) {
            return fail(format!("old_agbcc target routing self-test failed for {stem}"));
        }
        let mut expected_flags = agbcc_cflags();
        if agbcc_o1.contains(&stem) {
            expected_flags.push("-O1".to_string());
        }
        if stem == "080fa514" {
            expected_flags.push("-O1".to_string());
            expected_flags.push("-mcommutative-copy-constant".to_string());
        }
        if stem == "080fb670" {
            expected_flags.push("-mliteral-before-shift".to_string());
        }
        if agbcc_prologue.contains(&stem) {
            expected_flags.push("-mprologue-next-high-reg".to_string());
        }
        if stem == "080f9a50" {
            expected_flags.push("-mcompare-only-and-tst".to_string());
        }
        if probe.cflags(GS1, &source) != expected_flags {
            return fail(format!("old_agbcc flags self-test failed for {stem}"));
        }
    }
    if probe.uses_agbcc(GS1, "/tmp/080000c0.c")
        || probe.cflags(GS1, "/tmp/080000c0.c") == agbcc_cflags()
    {
        return fail("old_agbcc unrelated-source routing self-test failed");
    }

    for stem in ["02005a40", "02005a78"] {
        let source = from_root(&format!("exact/resource_3bf_c_{stem}.c"));
        let flags = probe.cflags(GS1, &source);
        if has(&flags, "-fcall-used-r4") || has(&flags, "-mthumb-interwork") {
            return fail(format!(
                "{stem} stock non-interworking ABI routing self-test failed"
            ));
        }
        let unrelated = probe.cflags(GS1, &from_root(&format!("exact/resource_3aa_c_{stem}.c")));
        if !has(&unrelated, "-fcall-used-r4") || !has(&unrelated, "-mthumb-interwork") {
            return fail(format!("{stem} overlay-path isolation self-test failed"));
        }
    }

    let grouped_dma = sorted_set(GROUPED_DMA_STORE_SOURCES);
    let grouped_dma_expected = [
        "08002f10", "08002fb0", "0800300c", "080037d4", "08003e10", "08004760",
        "08004838", "08004858", "080049e8", "08004a28", "08004a44", "08004a5c",
        "08004a94", "08005340", "08005394", "080053e8", "08005a78", "08005c68", "080060e8", "0800bc48", "0800bdd4", "0800c0f4", "0800d304",
        "08011590", "080170c4", "08019bac", "0801a4fc", "0801d014", "0801d980", "080251d4", "080284dc", "0808fe38", "0808fecc", "080907b0", "08090824", "08091174", "08094730", "08095160", "08095290", "080958a8", "08097540",
        "0809bb34", "080a1090", "080b010c", "080b0744", "080b5ad4", "080c0184", "080c08a8", "080f377c",
    ];
    if grouped_dma != grouped_dma_expected {
        return fail("grouped DMA source allowlist self-test failed");
    }
    for stem in &grouped_dma {
        if !has(&probe.cflags(GS1, &format!("/tmp/{stem}.c")), "-mgrouped-dma-store") {
            return fail(format!("grouped DMA flags self-test failed for {stem}"));
        }
    }
    if has(&probe.cflags(GS1, "/tmp/080000c0.c"), "-mgrouped-dma-store")
        || has(&probe.cflags(GS2, "/tmp/080958a8.c"), "-mgrouped-dma-store")
    {
        return fail("grouped DMA unrelated-source routing self-test failed");
    }

    let overlay_dma_source = from_root("exact/resource_3bd_c_02000c98.c");
    let overlay_dma_flags = probe.cflags(GS1, &overlay_dma_source);
    let overlay_dma_neighbor = probe.cflags(GS1, &from_root("exact/resource_3bc_c_02000c98.c"));
    for flag in ["-mgrouped-dma-store", "-fthumb-group-control-last"] {
        if !has(&overlay_dma_flags, flag)
            || has(&overlay_dma_neighbor, flag)
            || has(&probe.cflags(GS2, &overlay_dma_source), flag)
        {
            return fail(format!(
                "resource_3bd:0c98 grouped-DMA routing self-test failed for {flag}"
            ));
        }
    }

    for stem in ["08095160", "08095290"] {
        let flags = probe.cflags(GS1, &format!("/tmp/{stem}.c"));
        let direct = probe.direct(
            &format!("/tmp/{stem}.i"),
            &format!("/tmp/{stem}.s"),
            &format!("{stem}.c"),
            Some(&format!("/tmp/{stem}.c")),
        )?;
        let neighbor = neighbour_stem(stem, 4);
        let neighbor_direct = probe.direct(
            &format!("/tmp/{neighbor}.i"),
            &format!("/tmp/{neighbor}.s"),
            &format!("{neighbor}.c"),
            Some(&format!("/tmp/{neighbor}.c")),
        )?;
        let gs2 = probe.cflags(GS2, &format!("/tmp/{stem}.c"));
        if !has(&flags, "-mgrouped-dma-store")
            || !has(&direct, "-mgrouped-dma-store")
            || has(&neighbor_direct, "-mgrouped-dma-store")
            || has(&gs2, "-mgrouped-dma-store")
        {
            return fail(format!(
                "{stem} effect-initializer DMA routing self-test failed"
            ));
        }
    }

    let polled_dma_flags = probe.cflags(GS1, "/tmp/08094730.c");
    let polled_dma_direct = probe.direct(
        "/tmp/08094730.i",
        "/tmp/08094730.s",
        "08094730.c",
        Some("/tmp/08094730.c"),
    )?;
    let polled_dma_neighbor_direct = probe.direct(
        "/tmp/08094734.i",
        "/tmp/08094734.s",
        "08094734.c",
        Some("/tmp/08094734.c"),
    )?;
    for flag in ["-mgrouped-dma-store", "-fno-sched-depend-count"] {
        if !has(&polled_dma_flags, flag)
            || !has(&polled_dma_direct, flag)
            || has(&probe.cflags(GS1, "/tmp/08094734.c"), flag)
            || has(&polled_dma_neighbor_direct, flag)
            || has(&probe.cflags(GS2, "/tmp/08094730.c"), flag)
        {
            return fail(format!("08094730 polled-DMA routing self-test failed for {flag}"));
        }
    }

    let value1_before_base_flag = "-fthumb-group-value1-before-base";
    let value1_before_base_flags = probe.cflags(GS1, "/tmp/080907b0.c");
    let value1_before_base_direct = probe.direct(
        "/tmp/080907b0.i",
        "/tmp/080907b0.s",
        "080907b0.c",
        Some("/tmp/080907b0.c"),
    )?;
    if !has(&value1_before_base_flags, value1_before_base_flag)
        || !has(&value1_before_base_direct, value1_before_base_flag)
        || has(&probe.cflags(GS1, "/tmp/080907b4.c"), value1_before_base_flag)
        || has(&probe.cflags(GS2, "/tmp/080907b0.c"), value1_before_base_flag)
    {
        return fail("080907b0 grouped-DMA value1/base routing self-test failed");
    }

    let stack_zero_flag = "-fthumb-high-move-before-stack-store";
    let stack_zero_flags = probe.cflags(GS1, "/tmp/08095290.c");
    let stack_zero_direct = probe.direct(
        "/tmp/08095290.i",
        "/tmp/08095290.s",
        "08095290.c",
        Some("/tmp/08095290.c"),
    )?;
    let stack_zero_sibling_direct = probe.direct(
        "/tmp/08095160.i",
        "/tmp/08095160.s",
        "08095160.c",
        Some("/tmp/08095160.c"),
    )?;
    let stack_zero_neighbor_direct = probe.direct(
        "/tmp/08095294.i",
        "/tmp/08095294.s",
        "08095294.c",
        Some("/tmp/08095294.c"),
    )?;
    if sorted_set(HIGH_MOVE_BEFORE_STACK_STORE_SOURCES) != ["08095290"]
        || !has(&stack_zero_flags, stack_zero_flag)
        || !has(&stack_zero_direct, stack_zero_flag)
        || has(&probe.cflags(GS1, "/tmp/08095160.c"), stack_zero_flag)
        || has(&stack_zero_sibling_direct, stack_zero_flag)
        || has(&probe.cflags(GS1, "/tmp/08095294.c"), stack_zero_flag)
        || has(&stack_zero_neighbor_direct, stack_zero_flag)
        || has(&probe.cflags(GS2, "/tmp/08095290.c"), stack_zero_flag)
    {
        return fail("08095290 stack-zero ordering routing self-test failed");
    }

    let copied_decompressor_flags = [
        "-mgrouped-dma-store",
        "-fthumb-move-before-alu",
        "-fno-sched-depend-count",
        "-fthumb-hoist-parameter-save",
    ];
    for stem in ["08005340", "08005394", "080053e8"] {
        let copied_decompressor = probe.cflags(GS1, &format!("/tmp/{stem}.c"));
        let copied_decompressor_direct = probe.direct(
            &format!("/tmp/{stem}.i"),
            &format!("/tmp/{stem}.s"),
            &format!("{stem}.c"),
            Some(&format!("/tmp/{stem}.c")),
        )?;
        let copied_decompressor_gs2 = probe.cflags(GS2, &format!("/tmp/{stem}.c"));
        for flag in copied_decompressor_flags {
            if !has(&copied_decompressor, flag)
                || !has(&copied_decompressor_direct, flag)
                || has(&copied_decompressor_gs2, flag)
            {
                return fail(format!(
                    "{stem} copied-decompressor routing self-test failed for {flag}"
                ));
            }
        }
    }
    for source in ["/tmp/08005344.c", "/tmp/08005398.c", "/tmp/080053ec.c"] {
        for flag in copied_decompressor_flags {
            if has(&probe.cflags(GS1, source), flag) {
                return fail(format!(
                    "{source} copied-decompressor neighbor routing self-test failed for {flag}"
                ));
            }
        }
    }

    let minipool_tail_first = "-fthumb-minipool-tail-first";
    let minipool_source = "/tmp/080a9aec.c";
    let minipool_direct = probe.direct(
        "/tmp/080a9aec.i",
        "/tmp/080a9aec.s",
        "080a9aec.c",
        Some(minipool_source),
    )?;
    if !has(&probe.cflags(GS1, minipool_source), minipool_tail_first)
        || !has(&minipool_direct, minipool_tail_first)
        || has(&probe.cflags(GS1, "/tmp/080a9af0.c"), minipool_tail_first)
        || has(
            &probe.direct(
                "/tmp/080a9af0.i",
                "/tmp/080a9af0.s",
                "080a9af0.c",
                Some("/tmp/080a9af0.c"),
            )?,
            minipool_tail_first,
        )
        || has(&probe.cflags(GS2, minipool_source), minipool_tail_first)
    {
        return fail("080a9aec minipool routing self-test failed");
    }

    let thumb_immediate_latency = "-mthumb-immediate-latency";
    let thumb_immediate_source = "/tmp/080babdc.c";
    let thumb_immediate_direct = probe.direct(
        "/tmp/080babdc.i",
        "/tmp/080babdc.s",
        "080babdc.c",
        Some(thumb_immediate_source),
    )?;
    if sorted_set(THUMB_IMMEDIATE_LATENCY_SOURCES) != ["080babdc"]
        || !has(
            &probe.cflags(GS1, thumb_immediate_source),
            thumb_immediate_latency,
        )
        || !has(&thumb_immediate_direct, thumb_immediate_latency)
        || has(&probe.cflags(GS1, "/tmp/080babe0.c"), thumb_immediate_latency)
        || has(
            &probe.direct(
                "/tmp/080babe0.i",
                "/tmp/080babe0.s",
                "080babe0.c",
                Some("/tmp/080babe0.c"),
            )?,
            thumb_immediate_latency,
        )
        || has(&probe.cflags(GS2, thumb_immediate_source), thumb_immediate_latency)
    {
        return fail("080babdc immediate-latency routing self-test failed");
    }

    let copy_lifetime_flags = probe.cflags(GS1, "/tmp/08006088.c");
    let unrelated_flags = probe.cflags(GS1, "/tmp/0800608c.c");
    if !has(&copy_lifetime_flags, "-fno-rerun-cse-after-loop")
        || !has(&copy_lifetime_flags, "-fno-regmove")
        || has(&unrelated_flags, "-fno-rerun-cse-after-loop")
        || has(&unrelated_flags, "-fno-regmove")
        || has(&probe.cflags(GS2, "/tmp/08006088.c"), "-fno-rerun-cse-after-loop")
    {
        return fail("08006088 copy-lifetime routing self-test failed");
    }
    if !has(&probe.cflags(GS1, "/tmp/0808c30c.c"), "-fno-rerun-cse-after-loop")
        || has(&probe.cflags(GS1, "/tmp/0808c310.c"), "-fno-rerun-cse-after-loop")
        || has(&probe.cflags(GS2, "/tmp/0808c30c.c"), "-fno-rerun-cse-after-loop")
    {
        return fail("0808c30c CSE-rerun routing self-test failed");
    }

    if !has(&probe.cflags(GS1, "/tmp/0801ed40.c"), "-fno-gcse")
        || !has(&probe.cflags(GS1, "/tmp/080981b0.c"), "-fno-gcse")
        || !has(&probe.cflags(GS1, "/tmp/08098c08.c"), "-fno-gcse")
        || !has(&probe.cflags(GS1, "/tmp/080b3284.c"), "-fno-gcse")
        || !has(&probe.cflags(GS1, "/tmp/080d40ec.c"), "-fno-gcse")
        || !has(
            &probe.direct(
                "/tmp/080d40ec.i",
                "/tmp/080d40ec.s",
                "080d40ec.c",
                Some("/tmp/080d40ec.c"),
            )?,
            "-fno-gcse",
        )
        || has(
            &probe.direct(
                "/tmp/080d40f0.i",
                "/tmp/080d40f0.s",
                "080d40f0.c",
                Some("/tmp/080d40f0.c"),
            )?,
            "-fno-gcse",
        )
        || has(&probe.cflags(GS1, "/tmp/0801ed44.c"), "-fno-gcse")
        || has(&probe.cflags(GS1, "/tmp/080981b4.c"), "-fno-gcse")
        || has(&probe.cflags(GS1, "/tmp/08098c0c.c"), "-fno-gcse")
        || has(&probe.cflags(GS1, "/tmp/080b3288.c"), "-fno-gcse")
        || has(&probe.cflags(GS1, "/tmp/080d40f0.c"), "-fno-gcse")
        || has(&probe.cflags(GS2, "/tmp/0801ed40.c"), "-fno-gcse")
        || has(&probe.cflags(GS2, "/tmp/080981b0.c"), "-fno-gcse")
        || has(&probe.cflags(GS2, "/tmp/08098c08.c"), "-fno-gcse")
        || has(&probe.cflags(GS2, "/tmp/080b3284.c"), "-fno-gcse")
        || has(&probe.cflags(GS2, "/tmp/080d40ec.c"), "-fno-gcse")
    {
        return fail("no-GCSE routing self-test failed");
    }

    if !has(&probe.cflags(GS1, "/tmp/08021d88.c"), "-fno-sched-depend-count")
        || has(&probe.cflags(GS1, "/tmp/08021d8c.c"), "-fno-sched-depend-count")
        || has(&probe.cflags(GS2, "/tmp/08021d88.c"), "-fno-sched-depend-count")
    {
        return fail("08021d88 scheduler routing self-test failed");
    }

    let low_constant_before_high_move = "-fthumb-low-constant-before-high-move";
    if !has(&probe.cflags(GS1, "/tmp/080a5614.c"), low_constant_before_high_move)
        || !has(&probe.cflags(GS1, "/tmp/080ba918.c"), low_constant_before_high_move)
        || !has(&probe.cflags(GS1, "/tmp/080ba918.c"), "-fno-rerun-cse-after-loop")
        || has(&probe.cflags(GS1, "/tmp/080ba918.c"), "-fno-expensive-optimizations")
        || !has(&probe.cflags(GS1, "/tmp/080ba918.c"), "-fno-regmove")
        || has(&probe.cflags(GS1, "/tmp/080a6a98.c"), low_constant_before_high_move)
        || has(&probe.cflags(GS2, "/tmp/080a5614.c"), low_constant_before_high_move)
        || has(&probe.cflags(GS2, "/tmp/080ba918.c"), low_constant_before_high_move)
    {
        return fail("low-constant-before-high-move routing self-test failed");
    }

    let orr_dead_input_reuse = "-fthumb-orr-dead-input-reuse";
    if !has(&probe.cflags(GS1, "/tmp/08003adc.c"), orr_dead_input_reuse)
        || has(&probe.cflags(GS1, "/tmp/08003ae0.c"), orr_dead_input_reuse)
        || has(&probe.cflags(GS2, "/tmp/08003adc.c"), orr_dead_input_reuse)
    {
        return fail("ORR dead-input reuse routing self-test failed");
    }

    let entry_frame_cluster = "-fthumb-entry-frame-cluster";
    if !has(&probe.cflags(GS1, "/tmp/0801c34c.c"), entry_frame_cluster)
        || has(&probe.cflags(GS1, "/tmp/0801c350.c"), entry_frame_cluster)
        || has(&probe.cflags(GS2, "/tmp/0801c34c.c"), entry_frame_cluster)
    {
        return fail("entry-frame-cluster routing self-test failed");
    }

    let literal_before_index_shift = "-fthumb-literal-before-index-shift";
    let interrupt_mask_flags = probe.cflags(GS1, "/tmp/0800307c.c");
    if !has(&interrupt_mask_flags, "-fno-schedule-insns2")
        || !has(&interrupt_mask_flags, literal_before_index_shift)
        || has(&probe.cflags(GS1, "/tmp/08003080.c"), literal_before_index_shift)
        || has(&probe.cflags(GS2, "/tmp/0800307c.c"), literal_before_index_shift)
    {
        return fail("literal-before-index-shift routing self-test failed");
    }

    if !has(&probe.cflags(GS1, "/tmp/080049e8.c"), "-O1")
        || has(&probe.cflags(GS1, "/tmp/08004a28.c"), "-O1")
        || has(&probe.cflags(GS1, "/tmp/08004a5c.c"), "-O1")
        || has(&probe.cflags(GS2, "/tmp/080049e8.c"), "-O1")
    {
        return fail("grouped DMA O1 routing self-test failed");
    }

    let call_arg0_move_first_overlays = sorted_set(CALL_ARG0_MOVE_FIRST_OVERLAY_SOURCES);
    if call_arg0_move_first_overlays.len() != 29 {
        return fail("overlay call-argument source allowlist self-test failed");
    }
    for path in &call_arg0_move_first_overlays {
        let absolute = from_root(path);
        if !has(&probe.cflags(GS1, &absolute), "-mcall-arg0-move-first")
            || has(&probe.cflags(GS2, &absolute), "-mcall-arg0-move-first")
        {
            return fail(format!(
                "overlay call-argument flags self-test failed for {path}"
            ));
        }
    }
    if has(&probe.cflags(GS1, "/tmp/020000a0.c"), "-mcall-arg0-move-first")
        || has(
            &probe.cflags(GS1, &from_root("exact/resource_381/c/020000a0.c")),
            "-mcall-arg0-move-first",
        )
    {
        return fail("overlay call-argument unrelated-source routing self-test failed");
    }

    let call_arg0_before_store = "-fthumb-call-arg0-before-store";
    let call_arg0_before_store_flags = probe.cflags(GS1, "/tmp/08077f70.c");
    if !has(&call_arg0_before_store_flags, call_arg0_before_store)
        || !has(&call_arg0_before_store_flags, "-fno-sched-alias")
        || !has(&call_arg0_before_store_flags, "-fsched-store-first")
        || has(&probe.cflags(GS1, "/tmp/08077f74.c"), call_arg0_before_store)
        || has(&probe.cflags(GS2, "/tmp/08077f70.c"), call_arg0_before_store)
    {
        return fail("08077f70 call-argument-before-store routing self-test failed");
    }

    let postcall_byte_increment_r2 = "-fthumb-postcall-byte-increment-r2";
    if !has(&probe.cflags(GS1, "/tmp/08098b10.c"), postcall_byte_increment_r2)
        || has(&probe.cflags(GS1, "/tmp/08098b11.c"), postcall_byte_increment_r2)
        || has(&probe.cflags(GS2, "/tmp/08098b10.c"), postcall_byte_increment_r2)
    {
        return fail("08098b10 post-call byte-increment routing self-test failed");
    }

    // -----------------------------------------------------------------------
    // sourceToAssemblyPlan
    // -----------------------------------------------------------------------

    let mut gcc_options =
        SourceToAssemblyPlanOptions::new(GS1, "/installed/08006088.c", "/work/candidate.c", "/work/candidate.s");
    gcc_options.flags = Some(CompilerFlagMutations {
        add_flags: vec!["-ffixed-r5".to_string()],
        remove_flags: vec!["-fcall-used-r4".to_string()],
    });
    let planned_gcc = probe.plan(&gcc_options)?;
    if planned_gcc.requested_family != CompilerFamily::Routed
        || planned_gcc.family != ResolvedFamily::Gcc296
        || planned_gcc.steps.len() != 1
        || planned_gcc.steps[0].kind != StepKind::Compile
        || planned_gcc.compiler_input != "/work/candidate.c"
        || at_back(&planned_gcc.steps[0].command, 1) != Some("/work/candidate.c")
        || !has(&planned_gcc.flags, "-fno-rerun-cse-after-loop")
        || !has(&planned_gcc.flags, "-fno-regmove")
        || !has(&planned_gcc.flags, "-ffixed-r5")
        || has(&planned_gcc.flags, "-fcall-used-r4")
    {
        return fail("source-to-assembly GCC routing/mutation self-test failed");
    }

    let mut agbcc_options =
        SourceToAssemblyPlanOptions::new(GS1, "/installed/080fa514.c", "/work/candidate.c", "/work/candidate.s");
    agbcc_options.preprocessed_output = Some("/work/candidate.preprocessed.i".to_string());
    let planned_agbcc = probe.plan(&agbcc_options)?;
    if planned_agbcc.requested_family != CompilerFamily::Routed
        || planned_agbcc.family != ResolvedFamily::OldAgbcc
        || planned_agbcc.compiler_input != "/work/candidate.preprocessed.i"
        || planned_agbcc.steps.len() != 2
        || planned_agbcc.steps[0].kind != StepKind::Preprocess
        || planned_agbcc.steps[1].kind != StepKind::Compile
        || at_back(&planned_agbcc.steps[0].command, 2) != Some("/work/candidate.c")
        || at_back(&planned_agbcc.steps[0].command, 1) != Some("/work/candidate.preprocessed.i")
        || planned_agbcc.steps[1].command.get(1).map(String::as_str)
            != Some("/work/candidate.preprocessed.i")
        || at_back(&planned_agbcc.steps[1].command, 1) != Some("/work/candidate.s")
        || !has(&planned_agbcc.flags, "-O1")
        || !has(&planned_agbcc.flags, "-mcommutative-copy-constant")
    {
        return fail("source-to-assembly old_agbcc routing/preprocessing self-test failed");
    }

    let mut forced_options =
        SourceToAssemblyPlanOptions::new(GS1, "/installed/not-routed.c", "/work/forced.c", "/work/forced.s");
    forced_options.family = Some(CompilerFamily::OldAgbcc);
    let forced_agbcc = probe.plan(&forced_options)?;
    if forced_agbcc.family != ResolvedFamily::OldAgbcc
        || has(&forced_agbcc.flags, "-mcommutative-copy-constant")
        || forced_agbcc.compiler_input != "/work/forced.i"
    {
        return fail("source-to-assembly forced old_agbcc self-test failed");
    }

    for (family, driver, minor) in [
        (
            CompilerFamily::PretEarlyThumb,
            alchemy_bundle_pret_early_thumb_driver(),
            "9",
        ),
        (CompilerFamily::Gcc2951, alchemy_bundle_gcc2951_driver(), "95"),
    ] {
        let name = family.as_str();
        let mut options = SourceToAssemblyPlanOptions::new(
            GS1,
            "/installed/not-routed.c",
            format!("/work/{name}.c"),
            format!("/work/{name}.s"),
        );
        options.family = Some(family);
        let planned = probe.plan(&options)?;
        // PORT NOTE: `planned.steps[0]?.command ?? []` in the TypeScript, which
        // is why these are read through `get` rather than indexed -- the very
        // next condition tests `steps.length !== 2`, so a one-step plan must
        // reach that test instead of panicking on the way to it.
        let preprocess: &[String] = planned
            .steps
            .first()
            .map(|step| step.command.as_slice())
            .unwrap_or(&[]);
        let compile: &[String] = planned
            .steps
            .get(1)
            .map(|step| step.command.as_slice())
            .unwrap_or(&[]);
        if CompilerFamily::from(planned.family) != family
            || planned.requested_family != family
            || planned.steps.len() != 2
            || planned.steps[0].kind != StepKind::Preprocess
            || planned.steps[1].kind != StepKind::Compile
            || planned.compiler_input != format!("/work/{name}.i")
            || !has(preprocess, &format!("-D__GNUC_MINOR__={minor}"))
            || compile.first().map(String::as_str) != Some(driver.as_str())
            || compile.get(1).map(String::as_str) != Some(format!("/work/{name}.i").as_str())
            || at_back(compile, 1) != Some(format!("/work/{name}.s").as_str())
        {
            return fail(format!(
                "source-to-assembly {name} family self-test failed"
            ));
        }
    }

    // PORT NOTE: the TypeScript pattern is `let rejected = false; try { ... }
    // catch (error) { rejected = String(error).includes(...) }`. A call that
    // does NOT throw leaves the flag false, and so does a throw carrying the
    // wrong message. `is_err_containing` reproduces both arms.
    let mut conflict_options = SourceToAssemblyPlanOptions::new(
        GS1,
        "/installed/080000c0.c",
        "/work/candidate.c",
        "/work/candidate.s",
    );
    conflict_options.flags = Some(CompilerFlagMutations {
        add_flags: vec!["-O1".to_string()],
        remove_flags: vec!["-O1".to_string()],
    });
    if !is_err_containing(&probe.plan(&conflict_options), "both added and removed") {
        return fail("source-to-assembly conflicting mutation self-test failed");
    }

    let mut gs2_agbcc_options = SourceToAssemblyPlanOptions::new(
        GS2,
        "/installed/080000c0.c",
        "/work/candidate.c",
        "/work/candidate.s",
    );
    gs2_agbcc_options.family = Some(CompilerFamily::OldAgbcc);
    if !is_err_containing(&probe.plan(&gs2_agbcc_options), "only approved for gs1") {
        return fail("source-to-assembly GS2 old_agbcc rejection self-test failed");
    }

    for family in [CompilerFamily::PretEarlyThumb, CompilerFamily::Gcc2951] {
        let mut options = SourceToAssemblyPlanOptions::new(
            GS2,
            "/installed/080000c0.c",
            "/work/candidate.c",
            "/work/candidate.s",
        );
        options.family = Some(family);
        if !is_err_containing(&probe.plan(&options), "only approved for gs1") {
            return fail(format!(
                "source-to-assembly GS2 {} rejection self-test failed",
                family.as_str()
            ));
        }
    }

    Ok((
        Summary {
            agbcc_sources: expected.len(),
            grouped_dma_sources: grouped_dma.len(),
            overlay_call_arg_sources: call_arg0_move_first_overlays.len(),
        },
        probe.trace,
    ))
}

/// `catch (error) { flag = String(error).includes(needle) }`.
fn is_err_containing<T>(result: &Result<T, String>, needle: &str) -> bool {
    match result {
        Ok(_) => false,
        Err(message) => message.contains(needle),
    }
}

// The two experimental drivers `selfTest` names. Read from the bundle crate
// that owns the paths rather than retyped as literals here, which is the whole
// point of depending on it.
fn alchemy_bundle_pret_early_thumb_driver() -> String {
    alchemy_bundle::bundle::pret_early_thumb_driver()
        .to_string_lossy()
        .into_owned()
}

fn alchemy_bundle_gcc2951_driver() -> String {
    alchemy_bundle::bundle::gcc2951_driver()
        .to_string_lossy()
        .into_owned()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn the_body_passes_and_prints_the_line_the_typescript_prints() {
        let (summary, _trace) = self_test().expect("the routing tables are self-consistent");
        assert_eq!(
            summary.line(),
            "self-test=ok agbcc_sources=47 grouped_dma_sources=49 overlay_call_arg_sources=29"
        );
    }

    #[test]
    fn the_body_records_a_corpus_worth_comparing() {
        let (_summary, trace) = self_test().expect("self-test passes");
        assert!(
            trace.distinct_keys().len() > 300,
            "the differential harness has a floor at 300 and this is where it comes from"
        );
    }

    #[test]
    fn flag_order_is_machine_code_not_a_set() {
        // gcc is later-flag-wins, so argv ORDER decides what the compiler does.
        // The agbcc comparison is `==` on a `Vec`, not a subset check, and this
        // pins that: the same flags in a different order must NOT compare equal.
        let mut expected = agbcc_cflags();
        expected.push("-O1".to_string());
        let mut reordered = expected.clone();
        reordered.swap(0, expected.len() - 1);
        assert_ne!(expected, reordered);
        // And the real thing matches in the original order only.
        let actual = cflags_for_target_source(GS1, "/tmp/08006a00.c");
        assert_eq!(actual, expected);
        assert_ne!(actual, reordered);
    }

    #[test]
    fn a_widened_routing_entry_is_caught() {
        // The failure this suite exists to catch: a neighbour four bytes along
        // must NOT inherit the flag. If routing ever widened, this flips.
        let flagged = cflags_for_target_source(GS1, "/tmp/08095290.c");
        let neighbour = cflags_for_target_source(GS1, "/tmp/08095294.c");
        assert!(has(&flagged, "-fthumb-high-move-before-stack-store"));
        assert!(!has(&neighbour, "-fthumb-high-move-before-stack-store"));
    }

    #[test]
    fn rejections_are_matched_by_substring_not_by_prose() {
        // PORT NOTE: `String(error).includes(...)`, so only the substring is
        // load-bearing. An `Ok` must NOT satisfy the check, which is the arm a
        // naive `matches!(result, Err(_))` would get wrong.
        let ok: Result<(), String> = Ok(());
        assert!(!is_err_containing(&ok, "only approved for gs1"));
        let err: Result<(), String> = Err("family x is only approved for gs1 builds".to_string());
        assert!(is_err_containing(&err, "only approved for gs1"));
        assert!(!is_err_containing(&err, "both added and removed"));
    }

    #[test]
    fn the_two_rejection_messages_are_really_produced() {
        let mut conflicting = SourceToAssemblyPlanOptions::new(
            GS1, "/installed/080000c0.c", "/work/a.c", "/work/a.s",
        );
        conflicting.flags = Some(CompilerFlagMutations {
            add_flags: vec!["-O1".to_string()],
            remove_flags: vec!["-O1".to_string()],
        });
        let message = source_to_assembly_plan(&conflicting).unwrap_err();
        assert!(message.contains("both added and removed"), "{message}");

        let mut gs2_agbcc = SourceToAssemblyPlanOptions::new(
            GS2, "/installed/080000c0.c", "/work/a.c", "/work/a.s",
        );
        gs2_agbcc.family = Some(CompilerFamily::OldAgbcc);
        let message = source_to_assembly_plan(&gs2_agbcc).unwrap_err();
        assert!(message.contains("only approved for gs1"), "{message}");
    }

    #[test]
    fn at_back_does_not_panic_past_the_start() {
        let command = vec!["cc".to_string()];
        assert_eq!(at_back(&command, 1), Some("cc"));
        assert_eq!(at_back(&command, 2), None);
        assert_eq!(at_back(&[], 1), None);
    }
}

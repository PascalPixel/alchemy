use alchemy_plan::plan::{
    direct_compiler_command, source_to_assembly_plan, CompilerFamily, CompilerFlagMutations,
    ResolvedFamily, SourceToAssemblyPlan, SourceToAssemblyPlanOptions, StepKind,
};
use alchemy_routing::routing::{agbcc_cflags, cflags_for_target_source, uses_agbcc_compiler, root, CompilerTarget};
use alchemy_routing::routing_data::{
    AGBCC_SOURCES,
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
}

impl Summary {
    /// The exact success line, byte for byte.
    pub fn line(&self) -> String {
        format!(
            "self-test=ok agbcc_sources={}",
            self.agbcc_sources
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

    for stem in ["02005a40", "02005a78"] {
        let source = from_root(&format!("exact/resource_3bf_c_{stem}.c"));
        let flags = probe.cflags(GS1, &source);
        if has(&flags, "-mthumb-interwork") {
            return fail(format!(
                "{stem} stock non-interworking ABI routing self-test failed"
            ));
        }
        let unrelated = probe.cflags(GS1, &from_root(&format!("exact/resource_3aa_c_{stem}.c")));
        if !has(&unrelated, "-mthumb-interwork") {
            return fail(format!("{stem} overlay-path isolation self-test failed"));
        }
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

    for (family, driver, minor) in [(CompilerFamily::Gcc3, alchemy_bundle_gcc3_driver(), "0")] {
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

    for family in [CompilerFamily::Gcc3] {
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

    // 080fb714, 080fb728, 080fb73c, 080fb750 and 080fb75c joined on 2026-08-16.
    // They are m4a engine owners that were routed to gcc296 with -ffixed-r3 and
    // the schedule-insns pair, emulating what old_agbcc does natively; under
    // old_agbcc they reproduce with no flags at all, like the neighbours they
    // sit between. This list is compared against a sorted set, so it must stay
    // in sorted order.
    let expected: [&str; 64] = [
        "08006878", "08006910", "080069a4",
        "08006a00", "08006a78", "08006af8", "08006ba8", "08006c24", "08006c68", "08006cdc", "08006d50", "08006dec",
        "08006e24", "08006f48", "08006f84", "08007028", "08007098", "0800711c", "080071a8", "08007220",
        "080f9a50",
        "080fa1fc", "080fa280", "080fa2a0", "080fa324", "080fa350", "080fa39c", "080fa3f0",
        "080fa424", "080fa458", "080fa490", "080fa4cc", "080fa514", "080fa55c", "080fa6a0", "080fa798", "080fa83c", "080fa8d4", "080fa928", "080fa9a4",
        "080fa9e0", "080faa58", "080fab3c", "080fab7c", "080fac44", "080facf8", "080fada0", "080fadf0", "080fae58",
        "080fb2a4", "080fb2cc", "080fb334", "080fb3a8", "080fb410", "080fb430", "080fb4a4", "080fb518", "080fb670", "080fb6a4",
        "080fb714", "080fb728", "080fb73c", "080fb750", "080fb75c",
    ];
    if sorted_set(AGBCC_SOURCES) != expected {
        return fail("old_agbcc source allowlist self-test failed");
    }

    Ok((
        Summary {
            agbcc_sources: expected.len(),
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

// The experimental driver `selfTest` names. Read from the bundle crate that
// owns the path rather than retyped as a literal here, which is the whole
// point of depending on it.
fn alchemy_bundle_gcc3_driver() -> String {
    alchemy_bundle::bundle::gcc3_driver()
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
            "self-test=ok agbcc_sources=47"
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

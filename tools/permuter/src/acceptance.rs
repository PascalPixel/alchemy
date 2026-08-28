use candidate_show::{
    allocator::{Repair, RepairPlan},
    cli::{options_of, ParseOutcome},
    render::{render, RenderOutput},
};
use compiler_core::routing::root;
use std::path::{Path, PathBuf};
struct Case {
    owner: u32,
    extent: usize,
    source: &'static str,
    perturb: fn(&str) -> Result<String, String>,
    expected: fn(&RepairPlan) -> bool,
}
fn replace_once(source: &str, before: &str, after: &str) -> Result<String, String> {
    if source.matches(before).count() != 1 {
        return Err(format!(
            "acceptance fixture expected one source shape, found {}",
            source.matches(before).count()
        ));
    }
    Ok(source.replacen(before, after, 1))
}
fn perturb_declarations(source: &str) -> Result<String, String> {
    replace_once(
        source,
        "    s32 win2;\n    s32 win1;\n    u8 *object;\n",
        "    u8 *object;\n    s32 win1;\n    s32 win2;\n",
    )
}
fn perturb_lifetime(source: &str) -> Result<String, String> {
    let source = replace_once(
        source,
        "    u8 *entry = base + 8;\n    s32 index;\n",
        "    u8 *entry = base + 8;\n    u32 *limit;\n    s32 index;\n",
    )?;
    let source = replace_once(
        &source,
        "    index = 0;\n    if (*((u32 *)(base + 264)) != 0) {\n",
        "    index = 0;\n    limit = (u32 *)(base + 264);\n    if (*limit != 0) {\n",
    )?;
    replace_once(
        &source,
        "        } while (index != (s32)*((u32 *)(base + 264)));\n",
        "        } while (index != (s32)*limit);\n",
    )
}
fn perturb_zero_carrier(source: &str) -> Result<String, String> {
    replace_once(
        source,
        "        s32 zero = 0;\n        visual->value_26 = zero;\n        visual->value_27 = zero;\n\n        visual->flags_a &= zero - 33;\n",
        "        visual->value_26 = 0;\n        visual->value_27 = 0;\n\n        visual->flags_a &= ~0x20;\n",
    )
}
fn perturb_paired_carriers(source: &str) -> Result<String, String> {
    let source = replace_once(
        source,
        "    /* GCC shape: work holds the state base, then the pool count. */\n    s32 work = (s32)EfxWork;\n    u32 off = (obj_id * 4) + 0x14;\n    /* GCC shape: src_z holds the source pointer, then the spawn Z. */\n    s32 src_z = (s32)*(struct EfxSrc **)(work + off);\n    struct EfxPos pos;\n    s32 x;\n",
        "    s32 state = (s32)EfxWork;\n    u32 off = (obj_id * 4) + 0x14;\n    s32 resource = (s32)*(struct EfxSrc **)(state + off);\n    struct EfxPos pos;\n    s32 x;\n    s32 spawn_z;\n",
    )?;
    let source = replace_once(&source, "    if (src_z == 0)\n", "    if (resource == 0)\n")?;
    let source = source.replace("((struct EfxSrc *)src_z)", "((struct EfxSrc *)resource)");
    let source = replace_once(
        &source,
        "    src_z = (pos.z & 0xfff00000) + 0x80000;\n",
        "    spawn_z = (pos.z & 0xfff00000) + 0x80000;\n",
    )?;
    let source = replace_once(
        &source,
        "    if (*(s16 *)(work + BATTLE_ACTIVE_OFS) != 0) {\n        struct EfxObj *ent = EfxPool;\n\n        work = 63;\n",
        "    if (*(s16 *)(state + BATTLE_ACTIVE_OFS) != 0) {\n        struct EfxObj *ent = EfxPool;\n        s32 count = 63;\n",
    )?;
    let source = replace_once(&source, "            work--;\n", "            count--;\n")?;
    let source = replace_once(
        &source,
        "        } while (work >= 0);\n",
        "        } while (count >= 0);\n",
    )?;
    replace_once(
        &source,
        "    obj = Object_CreateFar(22, x, 0x100000, src_z);\n",
        "    obj = Object_CreateFar(22, x, 0x100000, spawn_z);\n",
    )
}
fn declaration_plan(plan: &RepairPlan) -> bool {
    matches!(
        plan.repairs(),
        [Repair::SwapDeclarations { left, right }]
            if (left == "win2" && right == "object")
                || (left == "object" && right == "win2")
    )
}
fn lifetime_plan(plan: &RepairPlan) -> bool {
    matches!(
        plan.repairs(),
        [Repair::SplitLifetime { name }] if name == "limit"
    )
}
fn zero_plan(plan: &RepairPlan) -> bool {
    matches!(plan.repairs(), [Repair::MergeZeroCarrier])
}
fn paired_plan(plan: &RepairPlan) -> bool {
    matches!(
        plan.repairs(),
        [
            Repair::MergeCarrierPhases { earlier: first, later: second },
            Repair::MergeCarrierPhases { earlier: third, later: fourth }
        ] if first == "state" && second == "count" && third == "resource" && fourth == "spawn_z"
    )
}
fn score(
    source: &Path,
    owner: u32,
    extent: usize,
    work: &Path,
    allocator_order: bool,
) -> Result<RenderOutput, String> {
    let mut arguments = vec![
        source.to_string_lossy().into_owned(),
        "--owner".into(),
        format!("{owner:08x}"),
        "--size".into(),
        extent.to_string(),
        "--work".into(),
        work.to_string_lossy().into_owned(),
    ];
    if allocator_order {
        arguments.push("--allocator-order".into());
    }
    let ParseOutcome::Options(options) = options_of(root(), &arguments)? else {
        return Err("acceptance fixture unexpectedly requested candidate-show help".into());
    };
    render(root(), &options)
}
fn run_case(case: &Case, directory: &Path) -> Result<(), String> {
    let production = root().join(case.source);
    let exact = std::fs::read_to_string(&production)
        .map_err(|error| format!("{}: {error}", production.display()))?;
    let perturbed = (case.perturb)(&exact)?;
    let source = directory.join(format!("{:08x}.c", case.owner));
    std::fs::write(&source, &perturbed)
        .map_err(|error| format!("{}: {error}", source.display()))?;
    let decoded = score(
        &source,
        case.owner,
        case.extent,
        &directory.join("decode"),
        true,
    )?;
    if decoded.differing_halfwords == 0 && decoded.candidate_length == decoded.reference_length {
        return Err(format!(
            "acceptance perturbation {:08x} remained byte exact",
            case.owner
        ));
    }
    let report = decoded
        .allocator
        .ok_or_else(|| format!("decoder emitted no report for {:08x}", case.owner))?;
    let plan = report
        .repair
        .ok_or_else(|| format!("decoder emitted no repair for {:08x}", case.owner))?;
    if !(case.expected)(&plan) {
        return Err(format!(
            "decoder named {} for {:08x}, expected fixture repair\n{}",
            plan.label(),
            case.owner,
            report.text
        ));
    }
    let permutation = crate::perm::parse(&perturbed, &plan)?;
    let mut recovered = None;
    for choice in 1..permutation.count() {
        let candidate = directory.join(format!("recovery-{choice}.c"));
        std::fs::write(&candidate, permutation.evaluate(choice)?)
            .map_err(|error| format!("{}: {error}", candidate.display()))?;
        let result = score(
            &candidate,
            case.owner,
            case.extent,
            &directory.join(format!("recovery-{choice}")),
            false,
        )?;
        if result.differing_halfwords == 0 && result.candidate_length == result.reference_length {
            recovered = Some(choice);
            break;
        }
    }
    let choice = recovered.ok_or_else(|| {
        format!(
            "named repair {} did not recover byte exact owner {:08x}",
            plan.label(),
            case.owner
        )
    })?;
    println!(
        "acceptance=pass owner={:08x} perturbed_halfwords={} repair={} recovery_choice={choice}",
        case.owner,
        decoded.differing_halfwords,
        plan.label()
    );
    Ok(())
}
pub fn run() -> Result<(), String> {
    let cases = [
        Case {
            owner: 0x080b362c,
            extent: 576,
            source: "games/gs1/src/shop/select_use_item.c",
            perturb: perturb_declarations,
            expected: declaration_plan,
        },
        Case {
            owner: 0x0807a550,
            extent: 94,
            source: "games/gs1/src/trade/count_pending_offers.c",
            perturb: perturb_lifetime,
            expected: lifetime_plan,
        },
        Case {
            owner: 0x0808f1c0,
            extent: 204,
            source: "games/gs1/src/battle/effects/objects/start_effect_22.c",
            perturb: perturb_zero_carrier,
            expected: zero_plan,
        },
        Case {
            owner: 0x0808ef70,
            extent: 344,
            source: "games/gs1/src/battle/effects/random_particle/start_emitter.c",
            perturb: perturb_paired_carriers,
            expected: paired_plan,
        },
    ];
    let temporary = tempfile::Builder::new()
        .prefix("alchemy-decoder-acceptance-")
        .tempdir()
        .map_err(|error| error.to_string())?;
    for case in &cases {
        let directory: PathBuf = temporary.path().join(format!("{:08x}", case.owner));
        std::fs::create_dir_all(&directory)
            .map_err(|error| format!("{}: {error}", directory.display()))?;
        run_case(case, &directory)?;
    }
    println!("acceptance=ok tool=permuter owners={}", cases.len());
    Ok(())
}
#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn perturbations_are_bounded_and_named() {
        let declaration =
            std::fs::read_to_string(root().join("games/gs1/src/shop/select_use_item.c")).unwrap();
        assert_ne!(perturb_declarations(&declaration).unwrap(), declaration);
        let lifetime =
            std::fs::read_to_string(root().join("games/gs1/src/trade/count_pending_offers.c"))
                .unwrap();
        assert!(perturb_lifetime(&lifetime).unwrap().contains("u32 *limit;"));
        let zero = std::fs::read_to_string(
            root().join("games/gs1/src/battle/effects/objects/start_effect_22.c"),
        )
        .unwrap();
        assert!(perturb_zero_carrier(&zero)
            .unwrap()
            .contains("flags_a &= ~0x20"));
        let paired = std::fs::read_to_string(
            root().join("games/gs1/src/battle/effects/random_particle/start_emitter.c"),
        )
        .unwrap();
        let paired = perturb_paired_carriers(&paired).unwrap();
        assert!(paired.contains("s32 count = 63"));
        assert!(paired.contains("s32 spawn_z;"));
    }
}

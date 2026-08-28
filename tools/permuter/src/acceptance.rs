use compiler_core::routing::root;
use std::path::Path;

use crate::options::Options;

type Edit = (&'static str, &'static str, usize);

struct Case<'a> {
    owner: u32,
    source: &'static str,
    edits: &'a [Edit],
    expected: &'static str,
}

fn perturb(source: &str, edits: &[Edit]) -> Result<String, String> {
    let mut result = source.to_string();
    for &(before, after, expected) in edits {
        let found = result.matches(before).count();
        if found != expected {
            return Err(format!(
                "acceptance fixture expected {expected} source shapes, found {found}"
            ));
        }
        result = result.replace(before, after);
    }
    Ok(result)
}

fn run_case(case: &Case, directory: &Path) -> Result<(), String> {
    let exact = std::fs::read_to_string(root().join(case.source))
        .map_err(|error| format!("{}: {error}", case.source))?;
    let source = directory.join(format!("{:08x}.c", case.owner));
    std::fs::write(&source, perturb(&exact, case.edits)?)
        .map_err(|error| format!("{}: {error}", source.display()))?;
    let output = directory.join("search");
    crate::runner::run(Options {
        candidate: source,
        iterations: 16,
        jobs: 4,
        seed: 1,
        output: Some(output.clone()),
    })?;
    let report: serde_json::Value = serde_json::from_str(
        &std::fs::read_to_string(output.join("report.json")).map_err(|error| error.to_string())?,
    )
    .map_err(|error| error.to_string())?;
    let repair = report["decoder"]["repair"].as_str().unwrap_or("");
    let recovered = report["results"]
        .as_array()
        .into_iter()
        .flatten()
        .any(|result| result["outcome"]["exact"] == true);
    if report["baseline_differing_halfwords"].as_u64() == Some(0)
        || repair != case.expected
        || !recovered
    {
        return Err(format!(
            "acceptance failed for {:08x}: expected {}, report={report}",
            case.owner, case.expected
        ));
    }
    println!(
        "acceptance=pass owner={:08x} perturbed_halfwords={} repair={repair}",
        case.owner, report["baseline_differing_halfwords"]
    );
    Ok(())
}

pub fn run() -> Result<(), String> {
    let cases = [
        Case {
            owner: 0x080b362c,
            source: "games/gs1/src/shop/select_use_item.c",
            edits: &[(
                "    s32 win2;\n    s32 win1;\n    u8 *object;\n",
                "    u8 *object;\n    s32 win1;\n    s32 win2;\n",
                1,
            )],
            expected: "swap_declarations(win2,object)",
        },
        Case {
            owner: 0x0807a550,
            source: "games/gs1/src/trade/count_pending_offers.c",
            edits: &[
                (
                    "    u8 *entry = base + 8;\n    s32 index;\n",
                    "    u8 *entry = base + 8;\n    u32 *limit;\n    s32 index;\n",
                    1,
                ),
                (
                    "    index = 0;\n    if (*((u32 *)(base + 264)) != 0) {\n",
                    "    index = 0;\n    limit = (u32 *)(base + 264);\n    if (*limit != 0) {\n",
                    1,
                ),
                (
                    "        } while (index != (s32)*((u32 *)(base + 264)));\n",
                    "        } while (index != (s32)*limit);\n",
                    1,
                ),
            ],
            expected: "split_lifetime(limit)",
        },
        Case {
            owner: 0x0808f1c0,
            source: "games/gs1/src/battle/effects/objects/start_effect_22.c",
            edits: &[(
                "        s32 zero = 0;\n        visual->value_26 = zero;\n        visual->value_27 = zero;\n\n        visual->flags_a &= zero - 33;\n",
                "        visual->value_26 = 0;\n        visual->value_27 = 0;\n\n        visual->flags_a &= ~0x20;\n",
                1,
            )],
            expected: "merge_lifetime(zero_carrier)",
        },
        Case {
            owner: 0x0808ef70,
            source: "games/gs1/src/battle/effects/random_particle/start_emitter.c",
            edits: &[
                (
                    "    /* GCC shape: work holds the state base, then the pool count. */\n    s32 work = (s32)EfxWork;\n    u32 off = (obj_id * 4) + 0x14;\n    /* GCC shape: src_z holds the source pointer, then the spawn Z. */\n    s32 src_z = (s32)*(struct EfxSrc **)(work + off);\n    struct EfxPos pos;\n    s32 x;\n",
                    "    s32 state = (s32)EfxWork;\n    u32 off = (obj_id * 4) + 0x14;\n    s32 resource = (s32)*(struct EfxSrc **)(state + off);\n    struct EfxPos pos;\n    s32 x;\n    s32 spawn_z;\n",
                    1,
                ),
                ("src_z == 0", "resource == 0", 1),
                ("((struct EfxSrc *)src_z)", "((struct EfxSrc *)resource)", 4),
                ("    src_z = (pos.z & 0xfff00000) + 0x80000;\n", "    spawn_z = (pos.z & 0xfff00000) + 0x80000;\n", 1),
                (
                    "    if (*(s16 *)(work + BATTLE_ACTIVE_OFS) != 0) {\n        struct EfxObj *ent = EfxPool;\n\n        work = 63;\n",
                    "    if (*(s16 *)(state + BATTLE_ACTIVE_OFS) != 0) {\n        struct EfxObj *ent = EfxPool;\n        s32 count = 63;\n",
                    1,
                ),
                ("            work--;\n", "            count--;\n", 1),
                ("        } while (work >= 0);\n", "        } while (count >= 0);\n", 1),
                ("Object_CreateFar(22, x, 0x100000, src_z)", "Object_CreateFar(22, x, 0x100000, spawn_z)", 1),
            ],
            expected: "compose(merge_carrier_phases(state,count),merge_carrier_phases(resource,spawn_z))",
        },
    ];
    let temporary = tempfile::Builder::new()
        .prefix("alchemy-decoder-acceptance-")
        .tempdir()
        .map_err(|error| error.to_string())?;
    for case in &cases {
        let directory = temporary.path().join(format!("{:08x}", case.owner));
        std::fs::create_dir_all(&directory)
            .map_err(|error| format!("{}: {error}", directory.display()))?;
        run_case(case, &directory)?;
    }
    println!("acceptance=ok tool=permuter owners={}", cases.len());
    Ok(())
}

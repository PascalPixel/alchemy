// Approved bundle digests: the native compiler admission ledger.
//
// NOT generated: no tool writes this file, and the "@generated -- do not edit by
// hand" banner it used to carry was a false claim.
//
// ONE digest per executable per host. There used to be up to 121 for `cc1'
// alone -- every compiler this fork had ever built, all still approved. That is
// not an admission ledger, it is an amnesty: a stale binary left in dist/ by any
// past experiment validated silently, and so did anything cached under it.
// Worse, nearly every one of those digests named a compiler carrying invented
// options that no longer exist in the source, so the ledger went on approving
// compilers this repo can no longer produce and must never accept again.
//
// The rule now: a digest is admissible only from a green `make verify' on a
// bundle built from the committed source, and admitting a NEW one REPLACES the
// old rather than joining it. A list here with more than one entry is a bug.
//
// The values are NOT path-reproducible -- the same gcc sources built at a
// different prefix give a byte-different `cc1' with identical codegen -- so a
// digest that does not match a locally rebuilt binary is not evidence of drift.
// It means this host has not been admitted. Hosts other than darwin-arm64 are
// deliberately EMPTY: build, verify, and pin your own, exactly as
// `hostAdmissionMessage' instructs. Inheriting an unverifiable digest from
// someone else's machine is how the amnesty started.

/// Approved digests for one executable in a bundle.
pub type ExecutableDigests = (&'static str, &'static [&'static str]);
/// One compiler target's executables.
pub type TargetExecutables = (&'static str, &'static [ExecutableDigests]);
/// One host's targets.
pub type HostTargets = (&'static str, &'static [TargetExecutables]);

pub static EXPECTED: &[HostTargets] = &[
    (
        "darwin-arm64",
        &[
            (
                "gs1",
                &[
                    ("xgcc", &["1c0b96c0193a3f547bf10c70078d87df10a62d34207da545653165b65c80d4c2"]),
                    ("cpp", &["c69d8c0423517be6a4962bd88e343c193a0842a164dd600550ceb36f8155d736"]),
                    ("tradcpp", &["f1e4340c3550029675b49e22976dd7d10588e365816ae0f2decd19729371ee00"]),
                    ("cc1", &["f7d1aeb098815f9b98c5df25bb47bab89aa14b1ab184b63c1472986f515fa3dd"]),
                ],
            ),
            (
                "gs2",
                &[
                    ("xgcc", &["128520f13ff01aee64a984b1279a6e3a682a3679de44c99296064f46fb1e8ec2"]),
                    ("cpp0", &["b4ac7f5ff7fd74f4eca40385832fd0360d13cb5d4f0b6c8b3ead4a67d2f3d5b0"]),
                    ("tradcpp0", &["7698319dfea3647dace68ffb5c3dbc0fd459f3a859699acb47c669d3eb8956a3"]),
                    ("cc1", &["91b2a67275a100e8b6695d85ef2d82d1fd144853cbcb361ddf1d8be31858230f"]),
                ],
            ),
        ],
    ),
    (
        "darwin-x64",
        &[
            (
                "gs1",
                &[
                    ("xgcc", &[]),
                    ("cpp", &[]),
                    ("tradcpp", &[]),
                    ("cc1", &[]),
                ],
            ),
            (
                "gs2",
                &[
                    ("xgcc", &[]),
                    ("cpp0", &[]),
                    ("tradcpp0", &[]),
                    ("cc1", &[]),
                ],
            ),
        ],
    ),
    (
        "linux-x64",
        &[
            (
                "gs1",
                &[
                    ("xgcc", &[]),
                    ("cpp", &[]),
                    ("tradcpp", &[]),
                    ("cc1", &[]),
                ],
            ),
            (
                "gs2",
                &[
                    ("xgcc", &[]),
                    ("cpp0", &[]),
                    ("tradcpp0", &[]),
                    ("cc1", &[]),
                ],
            ),
        ],
    ),
    (
        "linux-arm64",
        &[
            (
                "gs1",
                &[
                    ("xgcc", &[]),
                    ("cpp", &[]),
                    ("tradcpp", &[]),
                    ("cc1", &[]),
                ],
            ),
            (
                "gs2",
                &[
                    ("xgcc", &[]),
                    ("cpp0", &[]),
                    ("tradcpp0", &[]),
                    ("cc1", &[]),
                ],
            ),
        ],
    ),
];

pub static AGBCC_EXPECTED: &[(&str, &[&str])] = &[
    (
        "darwin-arm64",
        &["dfbbb5a8a0a9e14e7ed84a0a723eb6e68974836159a2c3e925b904058382e15a"],
    ),
    ("darwin-x64", &[]),
    ("linux-x64", &[]),
    ("linux-arm64", &[]),
];

/// One host's approved digests for a single-executable bundle.
pub type HostDigests = (&'static str, &'static [&'static str]);

// Stock gcc-3.0 comparison probe, used only to test whether Golden Sun sources
// match unmodified GCC 3.0 codegen instead of the gcc-2.96/Camelot ABI. Not
// staged on darwin-arm64; admit your own after a green verify.
pub static GCC3_EXPECTED: &[HostDigests] = &[
    ("darwin-arm64", &[]),
    ("darwin-x64", &[]),
    ("linux-x64", &[]),
    ("linux-arm64", &[]),
];

//! Original search and declaration scenarios through both game toolchains.

use super::tests::test_problem;
use super::*;

#[test]
fn forced_randomizer_seed_search_reaches_candidate() {
    crate::compiler::routing::prefer_installed_binutils();
    let work = tempfile::tempdir().unwrap();
    let draft = work.path().join("draft/08000000.c");
    std::fs::create_dir_all(draft.parent().unwrap()).unwrap();
    let source = include_str!("c/fixtures/mutations-flow.c");
    std::fs::write(&draft, source).unwrap();
    let first: Vec<_> = include_str!("c/fixtures/mutations-integer-seeds.tsv")
        .lines()
        .next()
        .unwrap()
        .split('\t')
        .collect();
    let mut problem = test_problem(&draft, source, vec![], Some("target"));
    problem.symbol = Some("target".into());
    let reference = Job::new(&problem, &work.path().join("reference")).unwrap();
    reference.compile(&first[5].replace("\\n", "\n")).unwrap();
    problem.listing = crate::diff::disassemble(&reference.object, Some("target")).unwrap();
    problem.listing_rows = Some(score::disassemble(&reference.object, Some("target")).unwrap());
    let arguments = [
        draft.to_str().unwrap(),
        "--function",
        "target",
        "--jobs",
        "1",
        "--iterations",
        "1",
        "--search-seed",
        "987",
        "--seed",
        first[1],
    ]
    .map(str::to_string);
    let options = parse_options(&arguments).unwrap();
    let mut state = perm::expand(&problem.nodes, None).unwrap();
    prepare(&reference, &mut state, "target").unwrap();
    let state = problem.state(state);
    let text = problem.source(&state);
    let base = Found {
        score: problem.evaluate(&reference, &text).unwrap(),
        unnatural: 0,
        text,
        steps: vec![],
        expanded: state.expanded.clone(),
        job: 0,
        iteration: 0,
    };
    assert!(base.score.weighted > 0);
    let found = search(
        &problem,
        &state,
        &base,
        true,
        &options,
        None,
        &work.path().join("run"),
        Instant::now(),
        None,
    )
    .unwrap();
    assert_eq!((found[0].score.weighted, found[0].score.differ), (0, 0));
    assert_eq!(found[0].text, first[5].replace("\\n", "\n"));
}

#[test]
fn independent_shared_declarations_match_printing() {
    for row in include_str!("c/fixtures/shared-declarations.tsv").lines() {
        let c: Vec<_> = row.split('\t').collect();
        let source = c[1].replace("\\n", "\n");
        let expected = c[2].replace("\\n", "\n");
        let tree = c::parse(&source).unwrap_or_else(|e| panic!("{}: {e}", c[0]));
        assert_eq!(tree.print(), expected, "{}", c[0]);
        assert_eq!(
            c::parse(&tree.print()).unwrap(),
            tree,
            "{}: round trip",
            c[0]
        );
    }
}

#[test]
fn independent_search_scenarios_reach_targets_and_isolate_workers() {
    crate::compiler::routing::prefer_installed_binutils();
    // Each game/scorer pair owns private compiler jobs and output directories.
    // Run the four pairs together instead of serializing their subprocesses.
    std::thread::scope(|scope| {
        for algorithm in [score::Algorithm::Difflib, score::Algorithm::Levenshtein] {
            for (game, edition) in [("tbs", "tbs-en"), ("tla", "tla-en")] {
                scope.spawn(move || check_search_scenarios(game, edition, algorithm));
            }
        }
    });
}

fn check_search_scenarios(game: &'static str, edition: &str, algorithm: score::Algorithm) {
    for row in include_str!("c/fixtures/search-scenarios.tsv").lines() {
        let c: Vec<_> = row.split('\t').collect();
        let source = c[4].replace("\\n", "\n");
        let target = c[5].replace("\\n", "\n");
        let work = tempfile::tempdir().unwrap();
        let draft = work.path().join("draft/08000000.c");
        std::fs::create_dir_all(draft.parent().unwrap()).unwrap();
        std::fs::write(&draft, &source).unwrap();
        let mut problem = test_problem(&draft, &source, vec![], Some(c[1]));
        problem.target = decomp_target(Some(edition)).unwrap();
        // ign_branch_targets defaults false. Its CLI defaults true instead.
        problem.score_options.ignore_branch_targets = false;
        problem.score_options.algorithm = algorithm;
        problem.symbol = Some(c[1].into());
        let reference = Job::new(&problem, &work.path().join("reference")).unwrap();
        reference
            .compile(&target)
            .unwrap_or_else(|e| panic!("{}: reference: {e}", c[0]));
        problem.listing = crate::diff::disassemble(&reference.object, Some(c[1])).unwrap();
        problem.listing_rows = Some(score::disassemble(&reference.object, Some(c[1])).unwrap());
        let options = Options {
            draft,
            addresses: vec![],
            game,
            target: problem.target,
            minutes: 0.0,
            jobs: c[2].parse().unwrap(),
            seed: 1,
            rng_seed: None,
            perm_seed: None,
            function: Some(c[1].into()),
            overlay: None,
            arm: false,
            iterations: Some(1000),
            heat: false,
            prefix: false,
            score_options: problem.score_options,
            keep_prob: 0.6,
            stop_on_zero: true,
            output: output::Options::default(),
        };
        let output = work.path().join("output");
        let mut checker =
            test_problem(&options.draft, &source, problem.listing.clone(), Some(c[1]));
        checker.target = problem.target;
        checker.score_options = problem.score_options;
        checker.symbol = problem.symbol.clone();
        checker.listing_rows = problem.listing_rows.clone();
        execute(problem, &options, None, &output)
            .unwrap_or_else(|e| panic!("{}: search: {e}", c[0]));
        let best = match std::fs::read_to_string(output.join("best-1.c")) {
            Ok(best) => best,
            Err(e) if e.kind() == std::io::ErrorKind::NotFound => {
                checker.source(&checker.state(perm::expand(&checker.nodes, None).unwrap()))
            }
            Err(e) => panic!("{}: result: {e}", c[0]),
        };
        let checked = checker
            .evaluate(&reference, &best)
            .unwrap_or_else(|e| panic!("{}: saved result: {e}", c[0]));
        assert_eq!(
            checked.weighted == 0,
            c[3] == "1",
            "{}: {}\n{best}",
            c[0],
            checked.summary_with(false)
        );
        if c[3] == "1" {
            assert_eq!(
                checked.differ, 0,
                "{}: saved result must match the compiled target",
                c[0]
            );
        }
        if output.exists() {
            assert!(
                std::fs::read_dir(&output).unwrap().all(|entry| {
                    let name = entry.unwrap().file_name();
                    let name = name.to_string_lossy();
                    name.starts_with("best-") || name.starts_with("output-")
                }),
                "{}: scratch survived",
                c[0]
            );
        }
        println!(
            "independent case {} ({edition}, {algorithm:?}) passed",
            c[0]
        );
    }
}

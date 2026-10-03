import { mkdirSync, statSync } from "node:fs";
import { dirname, join } from "node:path";
import { execFileSync } from "node:child_process";

const main = process.argv[2];
if (!main) throw new Error("usage: bun tools/alchemy/worktree.ts MAIN_CHECKOUT");

mkdirSync("tools/out", { recursive: true });
mkdirSync("out", { recursive: true });
execFileSync("ln", ["-sfn", join(main, "roms"), "roms"], { stdio: "inherit" });
for (const tool of ["binutils", "compiler-runtime", "compilers"]) {
    execFileSync("ln", ["-sfn", join(main, "tools/out", tool), join("tools/out", tool)], {
        stdio: "inherit",
    });
}

const editions = [
    "tbs-ja", "tbs-en", "tbs-de", "tbs-es", "tbs-fr", "tbs-it",
    "tla-ja", "tla-en", "tla-de", "tla-es", "tla-fr", "tla-it",
];
for (const cache of ["tools/out/cargo-target", ...editions.map(id => "out/" + id)]) {
    const source = join(main, cache);
    if (statSync(source, { throwIfNoEntry: false })?.isDirectory()) {
        execFileSync("cp", ["-c", "-R", source, dirname(cache) + "/"], { stdio: "inherit" });
    }
}

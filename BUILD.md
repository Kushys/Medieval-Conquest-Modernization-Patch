# Building from source

The patch is written in C and x86 assembly. The build targets 32-bit Windows PE using LLVM; Python 3, Clang, llvm-ar and lld-link must be on PATH. The tested build environment is Linux; WSL is a suitable starting point on Windows.

From this directory:

```sh
python3 build.py
```

The build creates `MC_Modernization.asi` in the repository root and intermediate files in `build/`. The main implementation is `modernization.c`, with feature modules in `.inc` files and native hooks in `hooks.S`.

To optionally check hook signatures against your own game executable:

```sh
python3 tools/check_hook_signatures.py /path/to/your/MC.exe
```

This check is not required to compile. No game executable or logs are distributed. A signature check does not substitute for running the patch on Windows.

The development baseline executable SHA-256 is `10ea865dd237496107298a475aca0b072ec46a2e7b7247126c148fec9df7965c` (widescreen patched at 2560×1440). The runtime checks hook signatures rather than requiring an identical full-file hash.

For verbose troubleshooting, add a `[Debug]` section to the INI with `VerboseLog=1` and optionally `ResearchDiagnostics=1` or `CameraDiagnostics=1`. Remove these settings when finished. Normal operation writes a reduced diagnostic log beside the executable; the directory must be writable.

The bundled AlphaYellow FOV ASI is not compiled by build.py. See THIRD-PARTY.md for upstream source and provenance.

# Nexum Gate

Windows x64 VST3 and standalone. Original implementation: AGPL-3.0-only.

Three independent Shaper/Gate/Pan cards, 16 curves, eight patterns of up to 64 steps, host/free/MIDI/audio timing, macros and preset management.

## Clone with the shared core
```sh
git clone --recurse-submodules https://github.com/nexum-audio/nexum-gate.git
```
The pinned submodule supplies JUCE and the shared UI. Source ZIP downloads from
GitHub do not include submodules: initialise them, or use the complete suite source
archive linked by the website. Do not copy builds or user audio into this repository.

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release --target NexumGate_VST3 NexumGate_Standalone --parallel 1
```

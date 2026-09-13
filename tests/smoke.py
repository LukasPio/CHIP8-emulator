"""Exercise the released executable, including ROM paths containing spaces.

Run with Python 3 on either Linux or Windows; no display/audio device needed.
"""
import os
from pathlib import Path
import subprocess
import sys
import tempfile


def main():
    executable = str(Path(sys.argv[1]).resolve())
    env = dict(os.environ, SDL_VIDEODRIVER="dummy", SDL_AUDIODRIVER="dummy",
               SDL_RENDER_DRIVER="software")

    def check(args, expected_code, expected_text):
        result = subprocess.run([executable, *args], env=env, capture_output=True,
                                text=True, timeout=15)
        assert result.returncode == expected_code, (args, result.returncode, result.stderr)
        assert expected_text in result.stdout + result.stderr, result

    check(["--help"], 0, "Usage: chip8 <rom_path>")
    check([], 1, "Usage: chip8 <rom_path>")
    with tempfile.TemporaryDirectory(prefix="chip8 release ") as directory:
        directory = Path(directory)
        check([str(directory / "missing rom.ch8")], 1, "ROM was not found")
        oversized = directory / "oversized rom.ch8"
        oversized.write_bytes(bytes(3585))
        check([str(oversized)], 1, "ROM is too large")
        # Set the sound timer, then loop forever: exercise CPU, timers and audio.
        rom = directory / "test game.ch8"
        rom.write_bytes(bytes.fromhex("603c f018 1204"))
        process = subprocess.Popen([executable, str(rom)], cwd=directory, env=env,
                                   stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        try:
            try:
                stdout, stderr = process.communicate(timeout=2)
            except subprocess.TimeoutExpired:
                pass  # An interactive emulator must remain running with this ROM.
            else:
                raise AssertionError((process.returncode, stdout, stderr))
        finally:
            process.terminate()
            process.communicate(timeout=10)
    print("PASS: help, argument errors, missing/oversized ROM, ROM path with spaces, emulation loop")


if __name__ == "__main__":
    main()

"""Linux window/input smoke test: xvfb-run -a python3 tests/window_smoke.py EXE ROM."""
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import time


executable, rom = (str(Path(arg).resolve()) for arg in sys.argv[1:3])
with tempfile.TemporaryDirectory(prefix="chip8 window ") as directory:
    process = subprocess.Popen([executable, rom], cwd=directory,
                               env=dict(os.environ, SDL_AUDIODRIVER="dummy", SDL_VIDEODRIVER="x11"),
                               stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    try:
        window = subprocess.check_output(
            ["xdotool", "search", "--sync", "--onlyvisible", "--pid", str(process.pid),
             "--name", "^CHIP8$"], text=True, timeout=15).strip().splitlines()[0]
        time.sleep(1)
        assert process.poll() is None, "Emulator exited before keyboard input"
        subprocess.run(["xdotool", "keydown", "--window", window, "minus"], check=True, timeout=5)
        stdout, stderr = process.communicate(timeout=5)
        assert process.returncode == 0, (process.returncode, stdout, stderr)
        assert "Program was successfully closed" in stdout, (stdout, stderr)
        print("PASS: visible ROM window and clean exit using the - key")
    except Exception:
        if process.poll() is None:
            process.kill()
        stdout, stderr = process.communicate(timeout=5)
        print(f"Emulator exit={process.returncode}\n{stdout}\n{stderr}", file=sys.stderr)
        raise
    finally:
        if process.poll() is None:
            process.kill()
            process.communicate(timeout=5)

from subprocess import run


def test_main_ok():
    result = run(["acr"], capture_output=True)
    assert result.returncode == 0
    assert result.stdout == b"Hello world!\n"

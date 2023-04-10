from subprocess import run


def test_main_list_readers_ok():
    result = run(["acr", "l"], capture_output=True)
    assert result.returncode == 0
    assert result.stdout == b"0: ACS ACR122U 00 00\n"

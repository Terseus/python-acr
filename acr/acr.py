import _libacr


def main():
    conn = _libacr.PcscConnection()
    conn.show_hello_world()

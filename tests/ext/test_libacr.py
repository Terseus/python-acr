from _libacr import PcscConnection


def test_conn_ok():
    conn = PcscConnection()
    assert conn.is_valid() is True

import sys

from _libacr import PcscConnection


def test_conn_ok():
    conn = PcscConnection()
    assert conn.is_valid() is True


def test_list_readers_refcount():
    conn = PcscConnection()
    assert sys.getrefcount(conn.list_readers()) == 3
    readers = conn.list_readers()
    assert sys.getrefcount(conn.list_readers()) == 4
    del readers
    assert sys.getrefcount(conn.list_readers()) == 3


def test_list_readers_ok():
    conn = PcscConnection()
    result = conn.list_readers()
    expected = ("ACS ACR122U 00 00",)
    assert result == expected

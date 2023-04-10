from argparse import ArgumentParser, Namespace

from _libacr import PcscConnection


def cmd_list_readers(conn: PcscConnection, args: Namespace) -> None:
    for (index, reader) in enumerate(conn.list_readers()):
        print(f"{index}: {reader}")


def build_argument_parser() -> ArgumentParser:
    parser = ArgumentParser(
        description="Command line utility to interact with the ACR122U card reader."
    )
    subparsers = parser.add_subparsers()
    parser_list_readers = subparsers.add_parser("list_readers", aliases=["list", "l"])
    parser_list_readers.set_defaults(cmd=cmd_list_readers)
    return parser


def main():
    args = build_argument_parser().parse_args()
    conn = PcscConnection()
    args.cmd(conn, args)

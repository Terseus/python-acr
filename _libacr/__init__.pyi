class PcscConnection:
    def __init__(self) -> None: ...
    def is_valid(self) -> bool: ...
    def list_readers(self) -> tuple[str]: ...

class PcscError(Exception): ...
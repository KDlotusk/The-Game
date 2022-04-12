__all__ = []  # type: list[str]

from enum import Enum, auto
from socket import socket
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from typing import Any

_HOST = '127.0.0.1'
_PORT = 3490
_SOCKET_BUFFER_SIZE = 1024

_is_connected = False


class _BytesEnum(bytes, Enum):
    def __repr__(self) -> 'str':
        return f'{type(self).__name__}.{self.name}'

    def _generate_next_value_(name, start, count, last_values):
        return name.encode()


class _ProtocolKeyword(_BytesEnum):
    CONEC = auto()
    PLAY_ = auto()
    ACKIT = auto()


class _BadResponse(Exception):
    __slots__ = ('keyword',)

    def __init__(self, keyword, *args, **kwargs) -> None:
        self.keyword = keyword
        super().__init__(*args, **kwargs)

    def __str__(self) -> str:
        return f'The'


_socket: 'socket | None' = None


KEYWORD_TO_FUNCTION = {_ProtocolKeyword.CONEC: print}


def _log_in(__username: 'bytes' = b'me') -> None:
    global _socket, _is_connected

    assert _socket is not None

    _socket.sendall(b''.join((_ProtocolKeyword.CONEC, b' ', __username)))
    data_received = _socket.recv(_SOCKET_BUFFER_SIZE).split()

    # keyword = data_received.pop(0)
    # if keyword != ProtocolKeyword.ACKIT:
    #     raise BadResponse(keyword)

    print(data_received)


def _connect_to_server() -> None:
    # ConnectionRefusedError
    global _socket, _is_connected

    if _socket is not None:
        return

    _socket = socket()
    _socket.settimeout(5)

    try:
        _socket.connect((_HOST, _PORT))
    except Exception as err:
        _socket.close()
        raise err

    _is_connected = True


def _send_to_server(keyword: '_ProtocolKeyword', data_send: 'Any'):
    # BrokenPipeError
    global _socket, _is_connected

    assert _socket is not None

    KEYWORD_TO_FUNCTION[keyword]()

    _socket.sendall(data_send)
    data_received = _socket.recv(_SOCKET_BUFFER_SIZE).split()
    print(data_received)

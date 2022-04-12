__all__ = []  # type: list[str]

from enum import Enum, auto
from socket import socket
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from typing import Any

_HOST = '127.0.0.1'
_PORT = 3490
_SOCKET_BUFFER_SIZE = 1024


class _BytesEnum(bytes, Enum):
    def __repr__(self) -> 'str':
        return f'{type(self).__name__}.{self.name}'

    def _generate_next_value_(name, start, count, last_values):
        return name.encode()


class _ProtocolKeyword(_BytesEnum):
    CONEC = auto()
    VALID = auto()

    SEEGM = auto()
    SNDGM = auto()

    PLAY_ = auto()
    ACKIT = auto()


class _BadResponse(Exception):
    __slots__ = ('data_received',)

    def __init__(
        self,
        data_received: 'list[bytes]',
        *args: 'Any',
        **kwargs: 'Any',
    ) -> None:
        self.data_received = list(data_received)
        super().__init__(*args, **kwargs)

    def __str__(self) -> str:
        return (
            'The server answered with an unexpected response : '
            f'''{b''.join(self.data_received)!r}'''
        )


_is_connected_ = False
_socket: 'socket | None' = None
_id_ = b'-1'


def _is_connected() -> 'bool':
    return _is_connected_


def _id() -> 'bytes':
    return _id_


def _see_games() -> 'list[str]':
    global _id_, _socket

    assert _socket is not None

    _socket.sendall(b''.join((_ProtocolKeyword.SEEGM, b' ', _id_)))
    data_received = _socket.recv(_SOCKET_BUFFER_SIZE).split()
    print(data_received)

    keyword = data_received.pop(0)
    if keyword != _ProtocolKeyword.SNDGM:
        raise _BadResponse(data_received)

    _id_ = data_received.pop(0)

    res = []
    for i in range(int(data_received.pop(0).decode())):
        if i % 2 == 0:
            continue
        res.append(data_received[i].decode())

    return res


def _log_in() -> None:
    global _id_, _socket

    assert _socket is not None

    _socket.sendall(b''.join((_ProtocolKeyword.CONEC, b' ')))
    data_received = _socket.recv(_SOCKET_BUFFER_SIZE).split()
    print(data_received)

    keyword = data_received.pop(0)
    if keyword != _ProtocolKeyword.VALID or len(data_received) != 1:
        raise _BadResponse(data_received)

    _id_ = data_received[0]


def _connect_to_server() -> None:
    # ConnectionRefusedError
    global _is_connected_, _socket

    if _is_connected_ is True:
        return

    _socket = socket()
    _socket.settimeout(5)

    try:
        _socket.connect((_HOST, _PORT))
    except Exception as err:
        _socket.close()
        raise err

    _is_connected_ = True

"""This script generates a graphical interface to play The Game."""

from __future__ import annotations

__all__ = []  # type: list[str]

from asyncio import gather as _gather_coroutines
from asyncio import run as _async_run
from sys import exit as _exit
from typing import TYPE_CHECKING

import click

from ._gui import _show_gui, _try_to_connect_to_server

if TYPE_CHECKING:
    from typing import NoReturn


async def _try_to_connect_to_server_async() -> None:
    _try_to_connect_to_server()


async def _main(__debug: 'bool') -> 'int':
    await _gather_coroutines(_try_to_connect_to_server_async(), _show_gui(__debug))

    print('The windows is closed !')
    return 0


@click.command
@click.option('--debug', is_flag=True)
def main(debug: 'bool') -> 'NoReturn':
    _exit(_async_run(_main(debug)))


if __name__ == '__main__':
    main()

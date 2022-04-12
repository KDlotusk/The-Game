"""This script generates a graphical interface to play The Game."""

from __future__ import annotations

__all__ = []  # type: list[str]

from asyncio import run
from sys import exit as _exit
from typing import TYPE_CHECKING

import click
from dearpygui import dearpygui as dpg
from dearpygui.demo import show_demo

from ._gui import _show_gui

if TYPE_CHECKING:
    from typing import NoReturn


async def _main_(__debug: 'bool') -> 'int':
    dpg.create_context()
    dpg.create_viewport(title='Custom Title')

    show_demo()

    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()
    return 0


async def _main(__debug: 'bool') -> 'int':
    await _show_gui(__debug)
    return 0


@click.command
@click.option('--debug', is_flag=True)
def main(debug: 'bool') -> 'NoReturn':
    """Launch a client to play TheGame."""
    _exit(run(_main(debug)))


if __name__ == '__main__':
    main()

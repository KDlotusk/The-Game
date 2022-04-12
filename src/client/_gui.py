__all__ = []  # type: list[str]

from typing import TYPE_CHECKING

from dearpygui import dearpygui as dpg
from dearpygui.demo import show_demo

from ._connection import _connect_to_server, _is_connected, _log_in

if TYPE_CHECKING:
    from socket import socket
    from typing import Any, Union

    _Tag = Union['int', 'str']

_APP_NAME = 'The Game'

_main_window: '_Tag' = -1

_ignore_connection_error = False
_last_err: 'Exception | None' = None
_err_msg = 'An error occurred while trying to request the server: %s'


def _on_play_clicked(sender: '_Tag', app_data: 'Any', socket: 'socket') -> None:
    if _is_connected:
        pass

    _log_in()


def _create_debug_gui() -> None:
    dpg.show_documentation()
    dpg.show_style_editor()
    dpg.show_debug()
    dpg.show_about()
    dpg.show_metrics()
    dpg.show_font_manager()
    dpg.show_item_registry()


def _close_popup(sender: '_Tag', app_data: 'Any', popup: '_Tag') -> None:
    global _ignore_connection_error

    _ignore_connection_error = True
    dpg.hide_item(popup)


def _try_to_connect_to_server() -> None:
    global _last_err

    try:
        _connect_to_server()
    except Exception as err:
        _last_err = err


async def _show_gui(__debug: 'bool') -> None:
    global _err_msg, _ignore_connection_error, _last_err, _main_window

    dpg.create_context()
    dpg.create_viewport(title=_APP_NAME)
    dpg.setup_dearpygui()

    with dpg.window(label=_APP_NAME) as _main_window:
        dpg.add_button(
            label='Play',
            callback=_on_play_clicked,
        )
        dpg.add_input_text(label='request')
        reconnect_btn = dpg.add_button(
            label='Try to reconnect to server',
            callback=_try_to_connect_to_server,
            show=False,
        )

    with dpg.window(label='Error', show=False, modal=True, no_close=True) as popup:
        err_text = dpg.add_text(_err_msg % None)
        dpg.add_separator()
        with dpg.group(horizontal=True):
            dpg.add_button(label='Retry', callback=_try_to_connect_to_server)
            dpg.add_button(label='Close', callback=_close_popup, user_data=popup)

    if __debug:
        _create_debug_gui()

    dpg.set_primary_window(_main_window, True)
    dpg.show_viewport(maximized=True)

    dpg.value_registry()

    while dpg.is_dearpygui_running():
        # insert here any code you would like to run in the render loop
        # you can manually stop by using stop_dearpygui()
        if _is_connected is False:
            if _ignore_connection_error is False:
                dpg.set_value(err_text, _err_msg % str(_last_err))
                dpg.show_item(popup)
            else:
                dpg.show_item(reconnect_btn)

        dpg.render_dearpygui_frame()

    dpg.destroy_context()

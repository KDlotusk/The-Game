__all__ = []  # type: list[str]

from typing import TYPE_CHECKING

from dearpygui import dearpygui as dpg
from dearpygui.demo import show_demo

from ._connection import _connect_to_server, _id, _is_connected, _log_in, _see_games

if TYPE_CHECKING:
    from typing import Any, Union

    _Tag = Union['int', 'str']

_APP_NAME = 'The Game'

_err_msg = 'An error occurred while trying to request the server: %s'
_ignore_connection_error = False
_last_err: 'Exception | None' = None

_home_window: '_Tag' = -1
_lobbies_window: '_Tag' = -1
_lobbies_list: '_Tag' = -1
_err_text: '_Tag' = -1
_popup: '_Tag' = -1


def _create_debug_gui() -> None:
    dpg.show_documentation()
    dpg.show_style_editor()
    dpg.show_debug()
    dpg.show_about()
    dpg.show_metrics()
    dpg.show_font_manager()
    dpg.show_item_registry()


def _close_popup(sender: '_Tag', app_data: None, popup: '_Tag') -> None:
    global _ignore_connection_error

    _ignore_connection_error = True
    dpg.hide_item(popup)


def _retry_to_connect() -> None:
    global _last_err
    global _popup

    try:
        _connect_to_server()
        print('Connected to server')
        dpg.hide_item(_popup)
    except Exception as err:
        _last_err = err


def _create_error_popup() -> None:
    global _err_text
    global _popup

    with dpg.window(
        label='Error', show=False, modal=True, no_close=True, no_move=True
    ) as _popup:
        _err_text = dpg.add_text(_err_msg % None)
        dpg.add_separator()
        with dpg.group(horizontal=True):
            dpg.add_button(label='Retry', callback=_retry_to_connect)
            dpg.add_button(label='Close', callback=_close_popup, user_data=_popup)

        dpg.set_item_pos(
            _popup,
            (
                dpg.get_viewport_width() / 2 - dpg.get_item_width(_popup) / 2,
                dpg.get_viewport_height() / 2 - dpg.get_item_height(_popup) / 2,
            ),
        )
        print(dpg.get_item_width(_popup))
        dpg.hide_item(_popup)


def _on_back_clicked() -> None:
    global _home_window, _lobbies_window

    dpg.show_item(_home_window)
    dpg.set_primary_window(_home_window, True)
    dpg.hide_item(_lobbies_window)


def _create_lobbies_window() -> None:
    global _lobbies_list, _lobbies_window

    with dpg.window(label=_APP_NAME, show=False) as _lobbies_window:
        _lobbies_list = dpg.add_listbox([], label="Games available")
        dpg.add_button(
            label='Back',
            callback=_on_back_clicked,
        )


def _on_play_clicked(sender: '_Tag', app_data: 'Any') -> None:
    global _last_err, _ignore_connection_error
    global _home_window, _lobbies_window, _popup

    if _is_connected() is False:
        try:
            _connect_to_server()
            print('Connected to server')
        except Exception as err:
            _last_err = err
            _ignore_connection_error = False
            return

    if _id() == b'-1':
        _log_in()

    dpg.show_item(_lobbies_window)
    dpg.set_primary_window(_lobbies_window, True)
    dpg.hide_item(_home_window)

    dpg.set_value(_lobbies_list, _see_games())


def _create_home_window() -> None:
    global _home_window

    with dpg.window(label=_APP_NAME) as _home_window:
        dpg.add_button(
            label='Play',
            callback=_on_play_clicked,
        )
    dpg.set_primary_window(_home_window, True)


def _change_size_popup(sender: '_Tag', app_data: '_Tag') -> None:
    global _popup

    dpg.set_item_pos(
        _popup,
        (
            dpg.get_viewport_width() / 2 - dpg.get_item_width(_popup) / 2,
            dpg.get_viewport_height() / 2 - dpg.get_item_height(_popup) / 2,
        ),
    )
    print(dpg.get_item_width(_popup))


async def _show_gui(__debug: 'bool') -> None:
    global _err_msg, _err_text, _ignore_connection_error, _last_err
    global _popup

    dpg.create_context()
    dpg.create_viewport(title=_APP_NAME)
    dpg.setup_dearpygui()

    dpg.set_viewport_resize_callback(_change_size_popup)
    dpg.show_viewport()

    _create_home_window()
    _create_error_popup()
    _create_lobbies_window()

    if __debug:
        _create_debug_gui()

    while dpg.is_dearpygui_running():
        if _is_connected() is False and _last_err is not None:
            if _ignore_connection_error is False:
                dpg.set_value(_err_text, _err_msg % str(_last_err))
                dpg.show_item(_popup)

        dpg.render_dearpygui_frame()

    dpg.destroy_context()

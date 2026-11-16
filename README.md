# zmk-corne-display

Небольшой ZMK-модуль для Corne, который заменяет стандартный виджет
батареи на экране OLED на «иконка батареи + процент заряда».

Каждая половина клавиатуры показывает заряд **своего** аккумулятора:
левый экран — левой половины, правый — правой.

## Как это работает

- Включается `CONFIG_ZMK_DISPLAY_STATUS_SCREEN_CUSTOM` — стандартный
  `status_screen.c` из ZMK не компилируется.
- Модуль реализует `zmk_display_status_screen()` и переиспользует
  штатные виджеты ZMK: output status (BT/USB), layer status,
  peripheral status и, опционально, WPM.
- Собственный виджет `battery_percent_status` подписан на
  `zmk_battery_state_changed` и `zmk_usb_conn_state_changed`.
- Процент берётся из `zmk_battery_state_of_charge()` — то есть
  локальный заряд текущей половины.

## Подключение

Добавьте модуль в `config/west.yml` вашего config-репозитория:

```yaml
manifest:
  remotes:
    - name: filolio
      url-base: https://github.com/Filolio
  projects:
    - name: zmk-corne-display
      remote: filolio
      revision: main
```

И включите в `config/corne.conf`:

```conf
CONFIG_ZMK_DISPLAY_STATUS_SCREEN_CUSTOM=y
CONFIG_ZMK_CORNE_DISPLAY=y
```

## Настройки

| Опция | По умолчанию | Описание |
| --- | --- | --- |
| `CONFIG_ZMK_CORNE_DISPLAY` | `y` | Включить кастомный экран |
| `CONFIG_ZMK_CORNE_DISPLAY_BATTERY_PERCENT` | `y` | Иконка батареи + процент |

Стандартный виджет `CONFIG_ZMK_WIDGET_BATTERY_STATUS` при этом
выключен — его заменяет виджет модуля.

## Лицензия

MIT

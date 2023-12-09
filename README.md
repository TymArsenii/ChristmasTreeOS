# ChristmasTreeOS
Adressable LED Christmas tree os

### Updates
  v1.0
    -Basic effects 
    -Stable
  v1.1 
    -XY leds access
    -Stable
<a id="versions"></a>

## Версии
<details>
<summary>Старые</summary>

- v1.1 - пуллап отдельныи методом
- v1.2 - можно передать конструктору параметр INPUT_PULLUP / INPUT(умолч)
- v1.3 - виртуальное зажатие кнопки энкодера вынесено в отдельную функцию + мелкие улучшения
- v1.4 - обработка нажатия и отпускания кнопки
- v1.5 - добавлен виртуальный режим
- v1.6 - оптимизация работы в прерывании
- v1.6.1 - подтяжка по умолчанию INPUT_PULLUP
- v1.7 - большая оптимизация памяти, переделан FastIO
- v1.8 - индивидуальная настройка таймаута удержания кнопки (была общая на всех)
- v1.8.1 - убран FastIO
- v1.9 - добавлена отдельная отработка нажатого поворота и запрос направления
- v1.10 - улучшил обработку released, облегчил вес в режиме callback и исправил баги
- v1.11 - ещё больше всякой оптимизации + настройка уровня кнопки
- v1.11.1 - совместимость Digispark
- v1.12 - добавил более точный алгоритм энкодера EB_BETTER_ENC
- v1.13 - добавлен экспериментальный EncButton2
- v1.14 - добавлена releaseStep(). Отпускание кнопки внесено в дебаунс
- v1.15 - добавлен setPins() для EncButton2
- v1.16 - добавлен режим EB_HALFSTEP_ENC для полушаговых энкодеров
- v1.17 - добавлен step с предварительными кликами
- v1.18 - не считаем клики после активации step. hold() и held() тоже могут принимать предварительные клики. Переделан и улучшен дебаунс
- v1.18.1 - исправлена ошибка в releaseStep() (не возвращала результат)
- v1.18.2 - fix compiler warnings
- v1.19 - оптимизация скорости, уменьшен вес в sram
- v1.19.1 - ещё чутка увеличена производительность
- v1.19.2 - ещё немного увеличена производительность, спасибо XRay3D
- v1.19.3 - сделал высокий уровень кнопки по умолчанию в виртуальном режиме
- v1.19.4 - фикс EncButton2
- v1.20 - исправлена критическая ошибка в EncButton2
- v1.21 - EB_HALFSTEP_ENC теперь работает для обычного режима
- v1.22 - улучшен EB_HALFSTEP_ENC для обычного режима
- v1.23 - getDir() заменил на dir()
- v2.0 
    - Алгоритм EB_BETTER_ENC оптимизирован и установлен по умолчанию, дефайн EB_BETTER_ENC упразднён
    - Добавлен setEncType() для настройки типа энкодера из программы, дефайн EB_HALFSTEP_ENC упразднён
    - Добавлен setEncReverse() для смены направления энкодера из программы
    - Добавлен setStepTimeout() для установки периода импульсного удержания, дефайн EB_STEP упразднён
    - Мелкие улучшения и оптимизация
</details>

- v3.0
  - Библиотека переписана с нуля, с предыдущими версиями несовместима!
    - Полностью другая инициализация объекта
    - Переименованы: hold()->holding(), held()->hold()
  - Оптимизация Flash памяти: библиотека весит меньше, в некоторых сценариях - на несколько килобайт
  - Оптимизация скорости выполнения кода, в том числе в прерывании
  - На несколько байт меньше оперативной памяти, несколько уровней оптимизации на выбор
  - Более простое, понятное и удобное использование
  - Более читаемый исходный код
  - Разбитие на классы для использования в разных сценариях
  - Новые функции, возможности и обработчики для кнопки и энкодера
  - Буферизация энкодера в прерывании
  - Нативная обработка двух одновременно нажимаемых кнопок как третьей кнопки
  - Поддержка 4-х типов энкодеров
  - Переписана документация
  - EncButton теперь заменяет GyverLibs/VirtualButton (архивирована)
- v3.1
  - Расширена инициализация кнопки
  - Убраны holdEncButton() и toggleEncButton()
  - Добавлен turnH()
  - Оптимизированы прерывания энкодера, добавлена setEncISR()
  - Буферизация направления и быстрого поворота
  - Сильно оптимизирована скорость работы action() (общий обработчик)
  - Добавлено подключение внешней функции-обработчика событий
  - Добавлена обработка кнопки в прерывании - pressISR()
- v3.2
  - Добавлены функции tickRaw() и clear() для всех классов. Позволяет проводить раздельную обработку (см. доку)
  - Улучшена обработка кнопки с использованием прерываний
- v3.3
  - Добавлены функции получения времени удержания pressFor(), holdFor(), stepFor() (отключаемые)
  - Добавлен счётчик степов getSteps() (отключаемый)
- v3.4
  - Доступ к счётчику кликов во время нажатого поворота
  - Добавлена функция detach()

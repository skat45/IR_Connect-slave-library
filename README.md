## Описание
Библиотека предназначена для подключения устройств к IR_Connect, исходный код которого лежит в [этом репозитории](https://github.com/skat45/IR_Connect_hub).

Работа библиотеки проверена на модуле ESP-12F и отладочной плате WeMos Mini. Для прототипирования или штучного/мелкосерийного производства рекомендуется использовать последнюю.

## Начало работы
Прорядок действий:
- Установка Arduino IDE c [их официального сайта](www.arduino.cc/en/software/) или c [MS Store](https://apps.microsoft.com/detail/9nblggh4rsd8?hl=en-US&gl=US)
- Импортирование данной библиотеки: Скетч (Sketch) -> Подключить библиотеку (Include Library) -> Добавить библиотеку из .ZIP... (Add .ZIP library...)

## Подключение к IR_Connect
Сначала необходимо подключить библиотеку: `#include <IR_Connect.h>`

Далее необходимо задать константы `IR_CONNECT_NUMBER` и `DEVICE_NAME`. Обе имеют строковый тип `String`. Первая указывает номер устройства IR_Connect (задаётся при прошивке и пишется на этикетке на лицевой стороне устройства), вторая - имя программируемого устройства, которое необходимо указать также в WEB-интерфейсе IR_Connect. Система чувствительна к регистру.

Строка `IR_Connect hub(IR_CONNECT_NUMBER, DEVICE_NAME)` инициализуирует объкет класса `IR_Connect`. С его помощью упростится подключение к коммутатору IR_Connect и обработка входящих команд.

В теле функции `setup` стоит инициализировать подключение с помщью функции `connect`: `hub.connect()`. Данная функция возвращает истину (совершено подключние) или ложь (что-то пошло не так).

Также в теле `setup` можно создать ассоциации принятых команд выполняемым функциям: `hub.on("example", onExample)`. При получении команды example выполнится функция `onExample`. Указываемая функция должна быть типа `void` и не принимать аргументов на вход.

Сдесь же определяетя и запасной вариант - действие при получении неизвестной (не указанной с помощью функции `on`) команды с помощью функции `onNotFoundCommand`, единственным аргументом которой является функция типа `void`.

В теле `loop` стоит время от времени проверять состояние подключения к коммутатору IR_Connect: `hub.isConnected()`. Данная функция возвращает истину (всё ок) или ложь (подключение разорвано).

Зацикленная функция также должна содержать проверку поступающих команд: `hub.getCommand()`.

Больше ничего не нужно. При получении команды сразу же будет выполнента соответствующая функция.

## Пример кода

```
#include <IR_Connect.h>

#define IR_CONNECT_NUMBER "0000"
#define DEVICE_NAME "LED STRIPE"

IR_Connect hub(IR_CONNECT_NUMBER, DEVICE_NAME);

void setup() {
  Serial.begin(115200);
  if (!hub.connect())
    ESP.restart();

    hub.on("on", onEnable);
    hub.on("off", onDisable);
    hub.onNotFoundCommand(onNotFound);
}

void loop() {
  if (!hub.isConnected())
    ESP.restart();

  hub.getCommand();
}

void onEnable() {
  Serial.println("ON");
}

void onDisable() {
  Serial.println("OFF");
}

void onNotFound() {
  Serial.println("Recived command isn't known");
}
```

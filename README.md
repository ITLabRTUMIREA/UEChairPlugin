# VRChairPlugin
Данный плагин написан для движка Unreal Engine 4.

Плагин позволяет достучаться до VR кресла и указывать ему направления наклона.

## Установка плагина:
### 1. Создаем папку Plugins в корне проекта UE4

![Create folder Plugins](https://c.radikal.ru/c14/1804/a3/e832badd495a.png)

### 2. [Cкачиваем](https://gitlab.com/RealityShift/VRChairPlugin/tags) и распаковывываем плагин в папку Plugins.

**ОБЯЗАТЕЛЬНО** ***оставляем название папки таким же, как названиется архив!!!***

![Clone rep](https://c.radikal.ru/c41/1804/9b/07e2dae7d3a0.png)

### 3. Запускаем проект

Если все пошло "по плану", то проект должен запуститься и плагин должен работать =)

*************************************************************************

#### Если проект запустился, но плагин не появился, проверьте включен ли плагин:

Нужно зайти в `Setting->Plugins->Project->Virtual Reality`и поставить галку напротив `Enabled`и перезапустить проект.

![On Plugin](https://c.radikal.ru/c16/1804/12/25b2f9743e1c.png)


## Установка плагина, используя Source Code:
### 1. Создаем папку Plugins в корне проекта UE4

![Create folder Plugins](https://c.radikal.ru/c14/1804/a3/e832badd495a.png)

### 2. Клонируем репозиторий в папку Plugins.

**ОБЯЗАТЕЛЬНО** ***оставляем название папки таким же, как название репозитория!!!***

![Clone rep](https://c.radikal.ru/c41/1804/9b/07e2dae7d3a0.png)

### 3. Запускаем проект

При первом запуске появится окно с просьбо скомпилить плагин. Нажимаем "Да".

**ОБЯЗАТЕЛЬНО** ***проект должен иметь хоть один С++ класс, чтобы плагин скомпилился!!!***

![Compil plug](https://d.radikal.ru/d37/1804/75/5f52d46cb5b7.png)

Если все пошло "по плану", то проект должен запуститься и плагин должен работать =)
*******************************************************************************
#### Если проект скомпилился, но плагин не появился, проверьте включен ли плагин:

Нужно зайти в `Setting->Plugins->Project->Virtual Reality`и поставить галку напротив `Enabled`и перезапустить проект.

![On Plugin](https://c.radikal.ru/c16/1804/12/25b2f9743e1c.png)


## Blueprint_API

![On Plugin](https://a.radikal.ru/a11/1804/c6/634dfe185f40.png)

### 1. Serial Port

На вход подается номер COMPort, к которому подключено кресло.
***P.S Если вы будете тестировать приложение на компе, который прилагался к креслу, то ставьте `6`***

На выход подается `bool` переменная с проверкой на подключение к порту и переменная объекта, с помощью которой производятся остальные манипуляции.

### 2. Is Connected

На вход подается переменная объекта, а на выход `bool` с проверкой на подключение к порту.

### 3. Set Rotation Chair

Функция, которая принимает в себя параметры `X (Roll - вправо\влево)`, `Y (Pitch - вперед\назад)` и переменную объекта.

В данной функции указываются параметры для указанию креслу, куда производить наклон.

### 4. Float Pitch\Roll

По сути тоже самое, что и функция `Set Rotation Chair`, но можно по отдельности менять параметры `X` и `Y`.

### 5. Start\Timer\Stop Sending

Функция `Start Sending` и `Timer Sending`  переобразует заданные `float` параметры из `Set Rotation Chair (или Float Pitch\Roll)` в форму параметров, которая отправляется в кресло.

`Start Sending` работает бесконечно, поэтому, чтобы остановить передачу данных в кресло и остановить его, нужно воспользоваться функцией `Stop Sending`.

На вход `Timer Sending` подается `float` параметром со временем отработки данной функции.

Так же у `Start Sending` и `Timer Sending` есть два дополнительных параметра: `Frequency (частота)` и `Log`.

`Frequency` отвечает за частоту отправки "сообщения" креслу. Чем меньше значение, чем чаще будут отправляться байты, чем плавнее будет работать кресло. По-умолчанию установлено `0.05`.

`Log` - выводит в консоль массив байтов, которые отправляются на кресло.


### 6. Close Port

Функция, которую надо, указывать в `Event End Play`, чтобы при завершении работы `Actor`, порт закрылся и при следующем запуске можно было до него достучаться.

***P.S по умолчанию, при завершении работы `Actor` или удалении объекта из сцены, срабатывает закрытие порта, так что эта функция оставлена на всякий случай.***

### 7. Port Write & Float to Bytes

Данные функции были написаны для Debug кресла, чтобы вручную отправлять ему байты.

***В скором времени их не будет, так как они не нужны***

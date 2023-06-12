# Операционные системы ИДЗ 4

0. [**Задание**](#задание)
0. [**Отчёт**](#отчёт)
    + [*4-5 баллов*](#4-5-баллов)
    + [*6-7 баллов*](#6-7-баллов)
    + [*8 баллов*](#8-баллов)
0. [**Как запустить**](#как-запустить)

## Задание

*Сетевые взаимодействия с применением транспортного протокола UDP.*

*Вариант 20. Задача о магазине – 2 (забывчивые покупатели). В магазине работают два отдела, каждый отдел обладает уникальным ассортиментом. В каждом отделе работает один продавец. В магазин ходят исключительно забывчивые покупатели, поэтому каждый покупатель носит с собой список товаров, которые желает купить. Покупатель приобретает товары точно в том порядке, в каком они записаны в его списке. При этом товары в списке расположены в случайном порядке, что заставляет покупателя переходить от отдела к отделу, если это требуется для совершения покупок. Продавец может обслужить только одного покупателя за раз. Покупатель, вставший в очередь, засыпает пока не дойдет до продавца. Продавец засыпает, если в его отделе нет покупателей, и просыпается, если появится хотя бы один.*

*Создать приложение, моделирующее работу магазина в течение рабочего дня*

*Каждый продавец — клиент. Покупатели изначально порождаются отдельным клиентом. Сервер обеспечивает взаимодействие покупателей и клиентов.*

## Отчёт

**Исполнитель: Васильева Алёна Дмитриевна БПИ218**

**Делала на оценку 8 баллов**

### 4-5 баллов

*Разработать клиент–серверное приложение, в котором сервер (или серверы) и клиенты независимо друг от друга отображают только ту информацию, которая поступает им во время обмена. То есть, отсутствует какой-либо общий вывод интегрированной информации, отображающий поведение системы в целом.*

1. *В отчете необходимо привести фамилию, имя, отчество исполнителя, группу.*

    Продублирую: Васильева Алёна Дмитриевна БПИ218

2. *Привести номер варианта и условие задачи.*

    [Условие](#задание)

3. *Представить сценарий решаемой задачи поясняющий, каким образом исходные сущности и их поведение отображаются в серверы, клиенты, процессы и как осуществляется их взаимодействие.*

    Задача описана достаточно подробно. У нас есть 5 сущностей -- продавец, покупатель, товар, ассортимент, список.
    
    Продавец и покупатель будут представлены процессами (два процесса для двух продавцов и `n` процессов для покупателей в зависимости от теста). Товар будет представлен числовым идентификатором типа int. Ассортимент поделен так, что в первом отделе товары с нечётными идентификаторами, во втором -- с чётными. Списки покупателей будут представлены массивами из идентификаторов (товаров). 

    Процессы продавцов и покупателей будут взаимодействовать между собой через TCP протокол. Каждый продавец будет иметь ассортимент и каждый покупатель будет иметь список.

    Покупатели (`client-buyers.c`) работают исключительно как клиенты, сервер (`server.c`) фактически проксирует запросы на продавцов (`client-seller.c`), которые, в свою очередь, тоже умеют принимать и обрабатывать запросы

    Формат входных данных в файле, передаваемом первым аргументом командной строки: в первой строке записано число `n` -- количество покупателей. Далее описаны `n` покупателей в формате: в первой строке число `k` -- количество товаров в списке этого покупателя, в следующей строке `k` чисел -- идентификаторы товаров.


4. *При запуске программ требуемые для их работы IP адреса и порты необходимо задавать в командной строке, чтобы обеспечить гибкую подстройку к любой сети.*

    Формат входных данных для запуска:

    *Сервер:*
    ```
    ./server {server_port} {sellers_ip} {seller_2_port} {seller_1_port}
    ```

    *Покупатели:*
    ```
	./client-buyers {server_ip} {server_port} {test}
    ```

    *Продавцы:*
    ```
	./client-seller {seller_port}
    ```

5. *Для обеспечения корректного взаимодействия сетевых приложений и существующих в них процессов допускается использовать любые ранее изученные программные объекты.*

6. *Разработанное приложение должно работать как на одном компьютере так и в распределенном (сетевом) режиме на нескольких компьютерах, по которым можно будет разнести серверы и клиентов.*

    Это обеспечено конфигурацией IP-адресов сервера и продавцов посредством задания аргументов командной строки и наборов команд в Makefile для отдельного запуска клиента-покупателей, сервера и клиентов-продавцов

7. *Результаты работы программы должны быть отражены в отчете.*

    Для демонстрации работы взяла тест
    ```
    2
    3
    2 3 5
    2
    4 1
    ```

    В нём у нас есть 2 клиента, первый клиент должен купить товары 3 и 5 в первом отделе и товар 2 во втором, второй клиент купит товар 1 в первом отделе и товар 4 во втором

    Логи сервера:
    ```bash
    $ make server
    ./4-5-points/server 50124 127.0.0.1 50225 50226
    [SERVER 40776] Handling client 127.0.0.1
    [SERVER 40776] Got product with id=4
    [SERVER 40776] Got product with id=2
    [SERVER 40776] Got product with id=1
    [SERVER 40776] Got product with id=3
    [SERVER 40776] Got product with id=5
    ```

    Логи клиентов-покупателей:
    ```bash
    $ make buyers
    ./4-5-points/client-buyers 127.0.0.1 50124 tests/test2.in
    Init buyers client connected to server
    [BUYER 40811] Buying stock 4 from 2
    [BUYER 40812] Buying stock 2 from 2
    [BUYER 40811] Buying stock 1 from 1
    [BUYER 40812] Buying stock 3 from 1
    [BUYER 40812] Buying stock 5 from 1
    ```

    Логи первого (нечётного) продавца:
    ```bash
    $ make seller-1
    ./4-5-points/client-seller 50225
    [SELLER 40830] Selling stock with id=1
    [SELLER 40830] Selling stock with id=3
    [SELLER 40830] Selling stock with id=5
    ```

    Логи второго (чётного) продавца:
    ```bash
    $ make seller-2
    ./4-5-points/client-seller 50226
    [SELLER 40894] Selling stock with id=4
    [SELLER 40894] Selling stock with id=2
    ```

    Видим, что вывод программы соответствует логике работы

8. *Завершение работы клиентов и серверов на данном этапе не оговаривается. Но оно должно быть представлено в сценарии.*

    Покупатели завершают свою работу после совершения покупок, сервер и продавцы продолжают работу (надо прерывать вручную)

### 6-7 баллов

*В дополнение к программе на предыдущую оценку необходимо разработать клиентскую программу, подключаемую к серверу, которая предназначена для отображение комплексной информации о выполнении приложения в целом. То есть, данный программный модуль должен адекватно отображать поведение моделируемой системы, позволяя не пользоваться отдельными видами, предоставляемыми клиентами и серверами по отдельности.*

1. *Отчет расширить информацией о добавленном клиенте, модификациях других частей программы. Привести соответствующие результаты работы данной программы.*

    Добавлен `monitoring`, который получает логи от других "сервисов" по push-схеме, в других программах потребовалось добавить соответствующие "пушащие" функции. В качестве альтернативного варианта можно было бы хранить логи на клиентах и по шедулеру спулливать их агентом мониторинга

    Пример работы (фактически это агрегация логов):
    ```bash
    [MONITORING 41232] Started
    [MONITORING 41232] [SERVER 41230] Handling client 127.0.0.1
    [MONITORING 41232] [BUYER 41245] Buying stock 4 from 2
    [MONITORING 41232] [BUYER 41246] Buying stock 2 from 2
    [MONITORING 41232] [SERVER 41231] Got product with id=4
    [MONITORING 41232] [SERVER 41231] Got product with id=2
    [MONITORING 41232] [SELLER 41238] Selling stock with id=4
    [MONITORING 41232] [SELLER 41238] Selling stock with id=2
    [MONITORING 41232] [BUYER 41245] Buying stock 1 from 1
    [MONITORING 41232] [BUYER 41246] Buying stock 3 from 1
    [MONITORING 41232] [SERVER 41231] Got product with id=1
    [MONITORING 41232] [SELLER 41239] Selling stock with id=1
    [MONITORING 41232] [SERVER 41231] Got product with id=3
    [MONITORING 41232] [SELLER 41239] Selling stock with id=3
    [MONITORING 41232] [BUYER 41246] Buying stock 5 from 1
    [MONITORING 41232] [SERVER 41231] Got product with id=5
    [MONITORING 41232] [SELLER 41239] Selling stock with id=5
    ```

### 8 баллов

*В дополнение к предыдущей программе реализовать возможность, подключения множества клиентов, обеспечивающих отображение информации о работе приложения. Это должно позволить осуществлять наблюдение за поведением программы с многих независимых компьютеров.*

В программу на 6-7 баллов добавлен `monitoring-client.c`, который представляет из себя независимого клиента, которому прокси-мониторинг клиент `monitoring.c` передаёт информацию о системе. Его вывод аналогичен предыдущему пункту с обычным мониторингом. Таким образом можно подключить несколько клиентов-мониторингом, указав их IP адреса и порты в аргументах командной строки. При этом, для изменения списка клиентов не требуется рестарт всей системы, а только прокси-мониторинга


## Как запустить

На тестовом (в разных терминалах):

```bash
make server
make seller-1
make seller-2
make buyers
```

Перекомпилировать:

```bash
make compile
```

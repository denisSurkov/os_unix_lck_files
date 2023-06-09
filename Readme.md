# Simple C lock files

Задача 2 из [задачника](https://eykuklin.github.io/OS-UNIX/).

Программа для блокировки файлов без использования `fcntl`. 
Для работы с `filename` создается вспомогательный файл `filename.lck`.
Его наличие обозначает, что данный файл заблокирован каким-то процессом.
PID процесса, который взял блокировку, находится в файле.

```txt
usage: ./os_unix_lck_files filename [-s sleep_time]
```

## Как запустить:
0. `cmake .` - создает `Makefile`. В репозитории уже лежит необходимый `Makefile`, так требует формат выполнения задач.
1. `chmod +x runme.sh`
2. `./runme.sh`

**Важно**: выключение всех задач занимает некоторое время. Конец вывода программы может быть похож на:
```txt
[+]     Sending sigint to all tasks
[+]     Waiting for tasks to end (saving statistics)
[job-1] After run job-1
[job-3] After run job-3
[job-6] After run job-6
[job-9] After run job-9
[job-8] After run job-8
[job-4] After run job-4
[job-5] After run job-5
[job-2] After run job-2
[job-7] After run job-7
[job-10]        After run job-10
[+]     Tasks statistics
```

## Результаты запуска локально:
```txt
[job-1]	total: 29	success: 29
[job-3]	total: 23	success: 23
[job-6]	total: 36	success: 36
[job-9]	total: 27	success: 27
[job-8]	total: 36	success: 36
[job-4]	total: 34	success: 34
[job-5]	total: 27	success: 27
[job-2]	total: 27	success: 27
[job-7]	total: 34	success: 34
[job-10]	total: 26	success: 26
```


## Особенности:
- Добавил таймер `[-s sleep_time]`, чтобы задавать время для сна;
- Программа может читать stdin и записывать в открываемый файл. Используется для записи статистики взятия lock.

Сурков Денис Дмитриевич, МЕН-300801 (ФТ-301), 2023.

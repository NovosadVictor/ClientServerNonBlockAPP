# ClientServerAPP

Клиент-серверное приложение
Работа с базой данных
Запросы:

SELECT (
        [service, period],
        [period,service],
        [service],
        [period],
        [phone, period],
        [phone],
        [ALL],
)

INSERT (
        [phone, service, date, sum],
        [phone,service,sum](тут автоматически текущая дата)
)

DELETE (
        [phone, service, period],
        [phone, period],
        [phone],
        [service, period],
        [service],
        [period],
        [ALL]
)

UPDATE (
        [service WHERE service], 
        [service WHERE phone, service],
        [service WHERE phone],
        [phone WHERE service],
        [phone WHERE phone],
        [phone],
        [service]
)

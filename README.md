# OMP-Task
## com.system.permissions
Cервис com.system.permissions запускается на сессионной шине и имеет 2 метода:
```cpp
void RequestPermission(permissionEnumCode: Int)
```
Пример использования
```bash
gdbus call -e -d com.system.permissions -o /com/system/permissions -m com.system.permissions.RequestPermission 0
```
Метод позволяет запросить разрешение у сервиса. Сервис перед выдачей разрешение получает путь до исполняемого файла, вызвавшего метод и сохраняет в базу данных SQLite.
```cpp
bool CheckApplicationHasPermission(applicationPath: String, permissionEnumCode: Int)
```
Пример использования
```bash
gdbus call -e -d com.system.permissions -o /com/system/permissions -m com.system.permissions.CheckApplicationHasPermission /usr/bin/gdbus 0
```
Метод позволяет проверить имеется ли у приложения с заданнымм путем разрешение.

## com.system.time
Cервис com.system.time запускается на сессионной шине и имеет 1 метод:
```cpp
uint64_t GetSystemTime()
```
Пример использования
```bash
gdbus call -e -d com.system.time -o /com/system/time -m com.system.time.GetSystemTime
```
Метод возвращает timestamp теккущего сисетмного времени в секугдах (Начиная с 1 января 1970). Для получения timestamp требуется разрешение от сервиса com.system.permissions. В противном случае возвращает ошибку Unauthorized access.

## Установка и сборка

### Необходимые зависимости:
   - [sdbus-c++](https://github.com/Kistler-Group/sdbus-cpp)
   - sqlite3
  
### Сборка

1. Сборка сервиса com.system.permission:
```bash
cd Task-1/
mkdir build
cd build
cmake ..
make
```

3. Сборка сервиса com.system.time:
```bash
cd .Task-2/
mkdir build
cd build
cmake ..
make
```

4. Сборка клиентского приложения:
```bash
cd Client/
mkdir build
cd build
cmake ..
make
```

#### Запуск сервисов

1. com.system.permissions
```bash
cd Task-1/build
./permissions-server
```

2. com.system.time

```bash
cd Task-2/build
./time-server
```

3. Клиентское приложение
```bash
cd Client/build
./client-app
```

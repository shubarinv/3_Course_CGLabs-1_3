### Как выбать какая лаба будет запущена?

- Открой файл CmakeLists.txt
- там есть строка

```cmake
add_executable(${PROJECT_NAME} ${EXEC_LIBS} ${EXEC_BASE} labs/lab[номер_лабы].cpp)
```


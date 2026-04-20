# macOS динамическая загрузка: dyld и @rpath

- На macOS динамической загрузкой занимается `dyld` (dynamic linker/loader).
- `dyld` запускается, когда запускаешь бинарник (или библиотеку через `dlopen`).
- Он читает зависимости (`LC_LOAD_DYLIB`) из исполняемого файла и каждой подключаемой `.dylib`.
- При этом подставляет `@rpath`, `@loader_path`, `@executable_path` в реальные пути.

## Примеры команд для проверки
- `otool -L ./your_binary`
- `otool -L ./your_library.dylib`
- `otool -D ./your_library.dylib`
- `otool -l ./your_binary | grep -A2 LC_RPATH`

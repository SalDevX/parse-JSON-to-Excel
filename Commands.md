
# Excel Generator

## Compile

```bash
gcc -o excel_generator main.c cJSON.c -lxlsxwriter
```

## Execute

```bash
./excel_generator        
```



# Recipe Manager

## Compile

```bash
g++ recipe_manager.cpp -o recipe_manager `pkg-config --cflags --libs gtkmm-3.0 jsoncpp`
```

## Execute

```bash
./recipe_manager    
```
====================================
====================================

# My personal site https://saldevx.github.io

====================================
====================================



## Export Json datas to excel 

## excle dinamyc C Template 

## Food costing table 

## recipe_manager compile 

```bash
make
```


## Or manually compile 

```bash 

g++ -g -I. -o recipe_manager recipe_manager.cpp main.c cJSON.c \
    `pkg-config --cflags --libs gtkmm-3.0` \
    -lxlsxwriter -lcjson -lsigc-3.0 -ljsoncpp

```

# main.c

## Key Sections and Explanations

### Constants
- Defined using `#define` for file paths and the starting row.

### Function Declarations
- Declares the `process_recipes()` function.

### Main Function
- Calls `process_recipes()` to start the program.

### Process Recipes Function
- Opens and reads the JSON file.
- Parses the JSON data using the `cJSON` library.
- Creates an Excel workbook and worksheet.
- Defines styles for formatting cells.
- Writes the header row and processes each recipe.
- Cleans up resources after processing.

### C Language Concepts
- **Memory Management**: Uses `malloc()` and `free()` for dynamic memory allocation.
- **File Handling**: Opens, reads, and closes files using `fopen()`, `fread()`, and `fclose()`.
- **Loops and Conditionals**: Uses `for` loops and `if` statements to process data.
- **Functions**: Demonstrates how to define and call functions in C.# parse-JSON-to-Excel



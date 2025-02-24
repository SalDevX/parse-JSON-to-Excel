====================================
====================================

# My personal site https://saldevx.github.io

====================================
====================================



# Export Json to excel 

## Excle dinamyc C Template 

## Food costing table 

## Recipe_manager compile and run the app!
### /assets folder containing relative json files need to be in the root folder of the exec file!
### Excel preadsheet output will be generated in the relative folder you have the application running.
### Will keep on add features.. maybe not! who knows ;-) or you maight..

```bash
make
```
### Compile 

## Or manually 

```bash 

g++ -g -I. -o recipe_manager recipe_manager.cpp main.c cJSON.c \
    `pkg-config --cflags --libs gtkmm-3.0` \
    -lxlsxwriter -lcjson -lsigc-3.0 -ljsoncpp

```
## Run the app
```bash
./recipe_manager 
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



====================================
====================================

# My Personal Site  
# [SalDevX.github.io](https://saldevx.github.io)

====================================
====================================

## Export JSON to Excel  

### Excel Dynamic C Template  

### Food Costing Table  

### Recipe Manager - Compile and Run the App!  
- The `/assets` folder, containing relative JSON files, needs to be in the root folder of the executable file.  
- The Excel spreadsheet output will be generated in the same folder where the application is running.  
- I will keep adding features... maybe not! Who knows? ;-) Or maybe you might...  

### Compile Using Make  
```bash
make
```

### Or Compile Manually  
```bash
g++ -g -I. -o recipe_manager recipe_manager.cpp main.c cJSON.c \
    `pkg-config --cflags --libs gtkmm-3.0` \
    -lxlsxwriter -lcjson -lsigc-3.0 -ljsoncpp
```

### Run the App  
```bash
./recipe_manager
```

---

## `main.c` - Key Sections and Explanations  

### 📌 Constants  
- Defined using `#define` for file paths and the starting row.  

### 📌 Function Declarations  
- Declares the `process_recipes()` function.  

### 📌 Main Function  
- Calls `process_recipes()` to start the program.  

### 📌 Process Recipes Function  
- Opens and reads the JSON file.  
- Parses the JSON data using the `cJSON` library.  
- Creates an Excel workbook and worksheet.  
- Defines styles for formatting cells.  
- Writes the header row and processes each recipe.  
- Cleans up resources after processing.  

### 📌 C Language Concepts  
- **Memory Management**: Uses `malloc()` and `free()` for dynamic memory allocation.  
- **File Handling**: Opens, reads, and closes files using `fopen()`, `fread()`, and `fclose()`.  
- **Loops and Conditionals**: Uses `for` loops and `if` statements to process data.  
- **Functions**: Demonstrates how to define and call functions in C.  


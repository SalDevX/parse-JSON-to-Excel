#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "xlsxwriter.h"



//===========================================================
//==================== CONSTANTS ============================
//===========================================================
#define JSON_FILE "recipes.json"       // Path to the JSON input file
#define OUTPUT_FILE "food_costing_output.xlsx"  // Path to the Excel output file
#define START_ROW 1                    // Starting row for data in the Excel sheet

//===========================================================
//==================== FUNCTION DECLARATIONS ================
//===========================================================
void process_recipes();  // Function to process recipes and generate Excel file

//===========================================================
//==================== MAIN FUNCTION ========================
//===========================================================
int main() {
    process_recipes();  // Call the function to process recipes and generate Excel file
    return 0;           // Return 0 to indicate successful execution
}


//===========================================================
//==================== PROCESS RECIPES FUNCTION =============
//===========================================================
void process_recipes() {
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //==================== OPEN AND READ JSON FILE ==============
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    FILE *file = fopen(JSON_FILE, "r");  // Open the JSON file for reading
    if (!file) {
        perror("Error opening JSON file");  // Print error if file cannot be opened
        return;
    }

    // Determine the size of the JSON file
    fseek(file, 0, SEEK_END);  // Move file pointer to the end
    long file_size = ftell(file);  // Get the size of the file
    fseek(file, 0, SEEK_SET);  // Move file pointer back to the beginning

    // Allocate memory to store the JSON data
    char *json_data = (char *)malloc(file_size + 1);  // +1 for null terminator
    fread(json_data, 1, file_size, file);  // Read the file content into json_data
    fclose(file);  // Close the file
    json_data[file_size] = '\0';  // Null-terminate the string

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //==================== PARSE JSON DATA =====================
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    cJSON *root = cJSON_Parse(json_data);  // Parse the JSON data into a cJSON object
    if (!root) {
        printf("Error parsing JSON: %s\n", cJSON_GetErrorPtr());  // Print parsing error
        free(json_data);  // Free allocated memory
        return;
    }

    // Get the "recipes" array from the JSON data
    cJSON *recipes = cJSON_GetObjectItem(root, "recipes");
    if (!cJSON_IsArray(recipes)) {
        printf("Invalid JSON format: 'recipes' is not an array\n");  // Check if "recipes" is an array
        cJSON_Delete(root);  // Free the cJSON object
        free(json_data);  // Free allocated memory
        return;
    }

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //==================== CREATE EXCEL WORKBOOK ===============
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    lxw_workbook *workbook = workbook_new(OUTPUT_FILE);  // Create a new Excel workbook
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);  // Add a worksheet

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //==================== DEFINE STYLES =======================
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Bold format for recipe names
    lxw_format *bold_format = workbook_add_format(workbook);
    format_set_bold(bold_format);  // Set text to bold
    format_set_align(bold_format, LXW_ALIGN_CENTER);  // Center-align text
    format_set_border(bold_format, LXW_BORDER_THIN);  // Add a thin border

    // Header format for column headers
    lxw_format *header_format = workbook_add_format(workbook);
    format_set_bold(header_format);  // Set text to bold
    format_set_align(header_format, LXW_ALIGN_CENTER);  // Center-align text
    format_set_bg_color(header_format, LXW_COLOR_GRAY);  // Set background color to gray
    format_set_border(header_format, LXW_BORDER_THIN);  // Add a thin border

    // Total cost format (yellow background, red text)
    lxw_format *total_cost_format = workbook_add_format(workbook);
    format_set_bg_color(total_cost_format, LXW_COLOR_YELLOW);  // Set background color to yellow
    format_set_font_color(total_cost_format, LXW_COLOR_RED);   // Set font color to red
    format_set_bold(total_cost_format);  // Set text to bold
    format_set_border(total_cost_format, LXW_BORDER_THIN);  // Add a thin border

    // Border format for cells
    lxw_format *border_format = workbook_add_format(workbook);
    format_set_border(border_format, LXW_BORDER_THIN);  // Add a thin border

    // Define a format with a custom blue background (RGB: 0, 119, 255)
    lxw_format *deep_sky_blue_bg_format = workbook_add_format(workbook);
    lxw_color_t deep_sky_blue = 0x0077FF;  // RGB(0, 119, 255) 
    format_set_bg_color(deep_sky_blue_bg_format, deep_sky_blue);  // Set background color
    format_set_font_color(deep_sky_blue_bg_format, LXW_COLOR_WHITE);  // Set font color to white
    format_set_bold(deep_sky_blue_bg_format);  // Optional: Make the text bold
    format_set_align(deep_sky_blue_bg_format, LXW_ALIGN_CENTER);  // Optional: Center-align the text
    format_set_border(deep_sky_blue_bg_format, LXW_BORDER_THIN);  // Optional: Add a thin border

    // IDR currency format for columns C and E
    lxw_format *idr_format = workbook_add_format(workbook);
    format_set_num_format(idr_format, "\"IDR\" #,##0");  // Format as IDR currency
    format_set_border(idr_format, LXW_BORDER_THIN);  // Add a thin border

    // Generic number format (no decimals) for columns B and D
    lxw_format *number_format = workbook_add_format(workbook);
    format_set_num_format(number_format, "0");  // Format as a number with no decimals
    format_set_border(number_format, LXW_BORDER_THIN);  // Add a thin border

    // Decimal number format for column D (with thousands separator)
    lxw_format *decimal_format = workbook_add_format(workbook);
    format_set_num_format(decimal_format, "#,##0");  // Format as a number with thousands separator
    format_set_border(decimal_format, LXW_BORDER_THIN);  // Add a thin border

    // Combined format for Total Recipe Cost (yellow background, red text, thousands separator)
    lxw_format *total_cost_decimal_format = workbook_add_format(workbook);
    format_set_bg_color(total_cost_decimal_format, LXW_COLOR_YELLOW);  // Set background color to yellow
    format_set_font_color(total_cost_decimal_format, LXW_COLOR_RED);   // Set font color to red
    format_set_bold(total_cost_decimal_format);  // Set text to bold
    format_set_border(total_cost_decimal_format, LXW_BORDER_THIN);  // Add a thin border
    format_set_num_format(total_cost_decimal_format, "#,##0.0");  // Format as a number with thousands separator and 1 decimal place

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //==================== SET COLUMN WIDTHS ===================
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //  worksheet_set_column(worksheet, 0, 0, strlen(recipe_name->valuestring) + 5, NULL); dynamic
    worksheet_set_column(worksheet, 0, 0, 28, NULL);  // Column A: Item Name
    worksheet_set_column(worksheet, 1, 1, 18, NULL);  // Column B: Packaging Quantity
    worksheet_set_column(worksheet, 2, 2, 15, NULL);  // Column C: Price Item
    worksheet_set_column(worksheet, 3, 3, 22, NULL);  // Column D: Grams Recipe
    worksheet_set_column(worksheet, 4, 4, 15, NULL);  // Column E: Total Cost

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //==================== WRITE HEADER ROW ====================
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    worksheet_write_string(worksheet, START_ROW - 1, 0, "Item Name", header_format);
    worksheet_write_string(worksheet, START_ROW - 1, 1, "Packaging Quantity", header_format);
    worksheet_write_string(worksheet, START_ROW - 1, 2, "Price Item", header_format);
    worksheet_write_string(worksheet, START_ROW - 1, 3, "Grams Recipe", header_format);
    worksheet_write_string(worksheet, START_ROW - 1, 4, "Total Cost", header_format);
    printf("Header written successfully.\n");

    // Freeze the header row
    worksheet_freeze_panes(worksheet, START_ROW, 0);

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //==================== PROCESS RECIPES =====================
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    int row = START_ROW;  // Start writing data from this row
    cJSON *recipe;
    cJSON_ArrayForEach(recipe, recipes) {
        cJSON *recipe_name = cJSON_GetObjectItem(recipe, "recipe_name");
        cJSON *ingredients = cJSON_GetObjectItem(recipe, "ingredients");

        if (!cJSON_IsString(recipe_name) || !cJSON_IsArray(ingredients)) {
            continue;  // Skip invalid recipes
        }

        // Write the recipe name with the blue background format
        worksheet_write_string(worksheet, row, 0, recipe_name->valuestring,deep_sky_blue_bg_format);
        row++;

        int recipe_start_row = row;  // Track the starting row for this recipe's ingredients
        cJSON *ingredient;
        cJSON_ArrayForEach(ingredient, ingredients) {
            cJSON *item_name = cJSON_GetObjectItem(ingredient, "item_name");
            cJSON *packaging_quantity = cJSON_GetObjectItem(ingredient, "packaging_quantity");
            cJSON *price_item = cJSON_GetObjectItem(ingredient, "price_item");
            cJSON *grams_recipe = cJSON_GetObjectItem(ingredient, "grams_recipe");

            if (!cJSON_IsString(item_name) || !cJSON_IsNumber(packaging_quantity) ||
                !cJSON_IsNumber(price_item) || !cJSON_IsNumber(grams_recipe)) {
                continue;  // Skip invalid ingredients
            }

            // Write data with appropriate formatting
            worksheet_write_string(worksheet, row, 0, item_name->valuestring, border_format);
            worksheet_write_number(worksheet, row, 1, packaging_quantity->valuedouble, decimal_format);  // Column B: Packaging Quantity
            worksheet_write_number(worksheet, row, 2, price_item->valuedouble, idr_format);            // Column C: Price Item
            worksheet_write_number(worksheet, row, 3, grams_recipe->valuedouble, decimal_format);  // Column D: Grams Recipe

            // Total Cost formula (Column E)
            char formula[50];
            snprintf(formula, sizeof(formula), "=C%d*D%d/B%d", row + 1, row + 1, row + 1);
            worksheet_write_formula(worksheet, row, 4, formula, idr_format);  // Column E: Total Cost

            row++;
        }

        // Total Recipe Cost formula (Column E)
        char total_formula[50];
        snprintf(total_formula, sizeof(total_formula), "=SUM(E%d:E%d)", recipe_start_row, row - 1);
        worksheet_write_string(worksheet, row, 3, "Total Recipe Cost:", bold_format);

        // Write the formula with the combined format
        worksheet_write_formula(worksheet, row, 4, total_formula, total_cost_decimal_format);

        row += 2;  // Add space between recipes
    }

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //==================== CLEAN UP ============================
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    workbook_close(workbook);  // Save and close the Excel workbook
    cJSON_Delete(root);  // Free the cJSON object
    free(json_data);  // Free allocated memory
    printf("Excel file generated: %s\n", OUTPUT_FILE);
}






// Key Sections and Explanations
// Constants: Defined using #define for file paths and starting row.

// Function Declarations: Declares the process_recipes() function.

// Main Function: Calls process_recipes() to start the program.

// Process Recipes Function:

// Opens and reads the JSON file.

// Parses the JSON data using the cJSON library.

// Creates an Excel workbook and worksheet.

// Defines styles for formatting cells.

// Writes the header row and processes each recipe.

// Cleans up resources after processing.

// C Language Concepts:

// Memory Management: Uses malloc() and free() for dynamic memory allocation.

// File Handling: Opens, reads, and closes files using fopen(), fread(), and fclose().

// Loops and Conditionals: Uses for loops and if statements to process data.

// Functions: Demonstrates how to define and call functions in C.
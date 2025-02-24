import pandas as pd
from openpyxl import load_workbook
from openpyxl.styles import Font, Border, Side, Alignment

# Load your recipe data from JSON
data = pd.read_json('recipes.json')

# Load the Excel template
template_path = 'food_costing_template.xlsx'  # Path to your template
workbook = load_workbook(template_path)
sheet = workbook.active  # Get the active sheet

# Define the starting row for the first recipe
start_row = 2  # Assuming the first recipe starts at row 2

# Define the font style for the recipe name
recipe_name_font = Font(
    bold=True,  # Make the text bold
    size=14,    # Set the font size
    color="000000",  # Set the font color (black)
    italic=False,  # Optional: Add italic style
    underline="single"  # Optional: Add underline
)

# Define a border style (optional, if your template uses borders)
thin_border = Border(
    left=Side(style='thin'),
    right=Side(style='thin'),
    top=Side(style='thin'),
    bottom=Side(style='thin')
)

# Iterate through each recipe
for recipe in data['recipes']:
    # Insert the recipe name in the header
    sheet[f'A{start_row}'] = f"Recipe: {recipe['recipe_name']}"
    
    # Apply the font style to the recipe name cell
    sheet[f'A{start_row}'].font = recipe_name_font

    # Insert the ingredient data
    for i, ingredient in enumerate(recipe['ingredients']):
        row = start_row + 2 + i
        sheet[f'A{row}'] = ingredient['item_name']
        sheet[f'B{row}'] = ingredient['packaging_quantity']
        sheet[f'C{row}'] = ingredient['price_item']
        sheet[f'D{row}'] = ingredient['grams_recipe']
        sheet[f'E{row}'] = f"=C{row}*D{row}/B{row}"
        # Do not overwrite column E (it contains the formula)
        
        # Copy formatting from the template row (e.g., row 2) to the new row
        for col in ['A', 'B', 'C', 'D', 'E']:
            source_cell = sheet[f'{col}{start_row + 1}']  # Template cell with formatting
            target_cell = sheet[f'{col}{row}']  # New cell to apply formatting
            
            # Copy font
            target_cell.font = Font(
                name=source_cell.font.name,
                size=source_cell.font.size,
                bold=source_cell.font.bold,
                italic=source_cell.font.italic,
                color=source_cell.font.color
            )
            
            # Copy border
            target_cell.border = Border(
                left=source_cell.border.left,
                right=source_cell.border.right,
                top=source_cell.border.top,
                bottom=source_cell.border.bottom
            )
            
            # Copy alignment
            target_cell.alignment = Alignment(
                horizontal=source_cell.alignment.horizontal,
                vertical=source_cell.alignment.vertical,
                wrap_text=source_cell.alignment.wrap_text
            )
            
            # Copy number format
            target_cell.number_format = source_cell.number_format
    
    # Add a row for the total recipe cost
    total_cost_row = start_row + 2 + len(recipe['ingredients'])  # Row after the last ingredient
    sheet[f'D{total_cost_row}'] = "Total Recipe Cost:"  # Add a label for clarity
    sheet[f'E{total_cost_row}'] = f"=SUM(E{start_row + 2}:E{total_cost_row - 1})"  # Formula to sum total costs

    # Copy formatting for the total cost row
    for col in ['D', 'E']:
        source_cell = sheet[f'{col}{start_row + 1}']  # Template cell with formatting
        target_cell = sheet[f'{col}{total_cost_row}']  # New cell to apply formatting
        
        # Copy font
        target_cell.font = Font(
            name=source_cell.font.name,
            size=source_cell.font.size,
            bold=source_cell.font.bold,
            italic=source_cell.font.italic,
            color=source_cell.font.color
        )
        
        # Copy border
        target_cell.border = Border(
            left=source_cell.border.left,
            right=source_cell.border.right,
            top=source_cell.border.top,
            bottom=source_cell.border.bottom
        )
        
        # Copy alignment
        target_cell.alignment = Alignment(
            horizontal=source_cell.alignment.horizontal,
            vertical=source_cell.alignment.vertical,
            wrap_text=source_cell.alignment.wrap_text
        )
        
        # Copy number format
        target_cell.number_format = source_cell.number_format

    # Move to the next recipe box (leave a gap of 2 rows between recipes)
    start_row = total_cost_row + 3

# Save the updated workbook
output_path = 'food_costing_output.xlsx'  # Save as a new file
workbook.save(output_path)

print(f"Data successfully added to the template and saved as {output_path}!")
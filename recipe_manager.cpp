#include "excel_generator.h"
#include <fstream>
#include <gtkmm.h>
#include <iomanip>
#include <iostream>
#include <json/json.h>
#include <locale>
#include <sstream>

class RecipeManager : public Gtk::Window {
  public:
    RecipeManager();

  private:
    // Widget declarations
    Gtk::Box main_box;
    Gtk::Grid grid;
    Gtk::ScrolledWindow scrolled_window;
    Gtk::Box button_box{Gtk::ORIENTATION_HORIZONTAL};
    Gtk::Label name_label, ingredient_label;
    Gtk::Entry name_entry;
    Gtk::SpinButton ingredient_spin;
    std::vector<std::vector<Gtk::Entry *>> ingredient_entries;
    std::string selected_file = "recipes.json";
    Gtk::Button save_button{"Save Recipe"};
    Gtk::Button select_file_button{"Select File"};
    Gtk::Button clear_table_button{"Clear Table"};
    Gtk::Button generate_excel_button{"Generate Excel"};

    // Method declarations
    void setupGrid();
    void setupButtons();
    void clearTable();
    void generateExcel();
    void addIngredientFields();
    void removeLastRow();
    void selectFile();
    std::string customWriteJson(const Json::Value &root);
    void saveRecipe();
};

// ========================================================================
// Constructor
// ========================================================================
RecipeManager::RecipeManager()
    : name_label("Recipe Name:"), ingredient_label("Number of Ingredients:"),
      main_box(Gtk::ORIENTATION_VERTICAL), selected_file("recipes.json") {
    set_title("Recipe Manager");
    set_default_size(500, 400);

    // Setup main container
    main_box.set_spacing(20);
    main_box.set_margin_top(20);
    main_box.set_margin_bottom(20);
    main_box.set_margin_start(20);
    main_box.set_margin_end(20);
    add(main_box);

    // Setup scrolled window
    scrolled_window.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
    scrolled_window.set_min_content_height(400);
    scrolled_window.set_hexpand(true);

    // Setup grid
    grid.set_column_spacing(10);
    grid.set_row_spacing(10);
    scrolled_window.add(grid);
    main_box.pack_start(scrolled_window, Gtk::PACK_EXPAND_WIDGET);

    // Initial setup
    setupGrid();
    setupButtons();
    main_box.pack_end(button_box, Gtk::PACK_SHRINK);

    show_all_children();
}

// ========================================================================
// Member Function Definitions
// ========================================================================
void RecipeManager::setupGrid() {
    grid.attach(name_label, 0, 0, 1, 1);
    grid.attach(name_entry, 1, 0, 2, 1);

    ingredient_spin.set_range(1, 30);
    ingredient_spin.set_increments(1, 1);
    grid.attach(ingredient_label, 0, 1, 1, 1);
    grid.attach(ingredient_spin, 1, 1, 1, 1);

    grid.attach(*Gtk::make_managed<Gtk::Label>("Item Name"), 0, 3, 1, 1);
    grid.attach(*Gtk::make_managed<Gtk::Label>("Packaging Quantity"), 1, 3, 1,
                1);
    grid.attach(*Gtk::make_managed<Gtk::Label>("Price Item"), 2, 3, 1, 1);
    grid.attach(*Gtk::make_managed<Gtk::Label>("Grams Recipe"), 3, 3, 1, 1);

    ingredient_spin.signal_value_changed().connect(
        sigc::mem_fun(*this, &RecipeManager::addIngredientFields));

    addIngredientFields();
}

void RecipeManager::addIngredientFields() {
    int count = ingredient_spin.get_value_as_int();
    int current_rows = ingredient_entries.size();

    if (count > current_rows) {
        for (int i = current_rows; i < count; i++) {
            std::vector<Gtk::Entry *> row;
            for (int j = 0; j < 4; j++) {
                Gtk::Entry *entry = new Gtk::Entry();
                grid.attach(*entry, j, i + 4, 1, 1);
                row.push_back(entry);
            }
            ingredient_entries.push_back(row);
        }
    } else if (count < current_rows) {
        for (int i = current_rows; i > count; i--) {
            removeLastRow();
        }
    }
    show_all_children();
}

void RecipeManager::setupButtons() {
    button_box.set_halign(Gtk::ALIGN_CENTER);
    button_box.set_spacing(10);

    select_file_button.signal_clicked().connect(
        sigc::mem_fun(*this, &RecipeManager::selectFile));
    save_button.signal_clicked().connect(
        sigc::mem_fun(*this, &RecipeManager::saveRecipe));
    clear_table_button.signal_clicked().connect(
        sigc::mem_fun(*this, &RecipeManager::clearTable));
    generate_excel_button.signal_clicked().connect(
        sigc::mem_fun(*this, &RecipeManager::generateExcel));

    button_box.pack_start(select_file_button, Gtk::PACK_SHRINK);
    button_box.pack_start(save_button, Gtk::PACK_SHRINK);
    button_box.pack_start(clear_table_button, Gtk::PACK_SHRINK);
    button_box.pack_start(generate_excel_button, Gtk::PACK_SHRINK);
}

// Other member functions (removeLastRow, selectFile, etc.) follow the same
// pattern...

void RecipeManager::removeLastRow() {
    if (ingredient_entries.empty())
        return;

    auto last_row = ingredient_entries.back();
    for (auto *entry : last_row) {
        grid.remove(*entry);
        delete entry;
    }
    ingredient_entries.pop_back();
    show_all_children();
}

void RecipeManager::selectFile() {
    Gtk::FileChooserDialog dialog("Select JSON File",
                                  Gtk::FILE_CHOOSER_ACTION_SAVE);
    dialog.set_transient_for(*this);
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Select", Gtk::RESPONSE_OK);

    if (dialog.run() == Gtk::RESPONSE_OK) {
        selected_file = dialog.get_filename();
    }
}

void RecipeManager::clearTable() {
    for (auto &row : ingredient_entries) {
        for (auto &entry : row) {
            entry->set_text("");
        }
    }
}

void RecipeManager::generateExcel() {
    try {
        generate_excel(); // Ensure this C function is defined in main.c
        Gtk::MessageDialog dialog(*this, "Excel file generated!", false,
                                  Gtk::MESSAGE_INFO);
        dialog.run();
    } catch (...) {
        Gtk::MessageDialog dialog(*this, "Error generating Excel file!", false,
                                  Gtk::MESSAGE_ERROR);
        dialog.run();
    }
}

std::string RecipeManager::customWriteJson(const Json::Value &root) {
    std::ostringstream oss;
    oss << "{\n  \"recipes\": [\n";
    for (unsigned int i = 0; i < root["recipes"].size(); ++i) {
        const Json::Value &recipe = root["recipes"][i];
        oss << "    {\n"
            << "      \"recipe_name\": \"" << recipe["recipe_name"].asString()
            << "\",\n"
            << "      \"ingredients\": [\n";
        for (unsigned int j = 0; j < recipe["ingredients"].size(); ++j) {
            const Json::Value &ingredient = recipe["ingredients"][j];
            oss << "        {\n"
                << "          \"item_name\": \""
                << ingredient["item_name"].asString() << "\",\n"
                << "          \"packaging_quantity\": "
                << ingredient["packaging_quantity"].asDouble() << ",\n"
                << "          \"price_item\": "
                << ingredient["price_item"].asDouble() << ",\n"
                << "          \"grams_recipe\": "
                << ingredient["grams_recipe"].asDouble() << "\n"
                << "        }";
            if (j < recipe["ingredients"].size() - 1)
                oss << ",";
            oss << "\n";
        }
        oss << "      ]\n    }";
        if (i < root["recipes"].size() - 1)
            oss << ",";
        oss << "\n";
    }
    oss << "  ]\n}";
    return oss.str();
}

void RecipeManager::saveRecipe() {
    std::string recipe_name = name_entry.get_text().raw();
    if (recipe_name.empty()) {
        Gtk::MessageDialog dialog(*this, "Recipe name cannot be empty", false,
                                  Gtk::MESSAGE_ERROR);
        dialog.run();
        return;
    }

    // JSON Handling Logic
    Json::Value new_recipe;
    new_recipe["recipe_name"] = recipe_name;
    Json::Value ingredients(Json::arrayValue);

    for (const auto &row : ingredient_entries) {
        if (row[0]->get_text().empty())
            continue;

        Json::Value ingredient;
        ingredient["item_name"] = row[0]->get_text().raw();
        try {
            ingredient["packaging_quantity"] =
                std::stod(row[1]->get_text().raw());
            ingredient["price_item"] = std::stod(row[2]->get_text().raw());
            ingredient["grams_recipe"] = std::stod(row[3]->get_text().raw());
        } catch (...) {
            Gtk::MessageDialog dialog(*this, "Invalid numeric input", false,
                                      Gtk::MESSAGE_ERROR);
            dialog.run();
            return;
        }
        ingredients.append(ingredient);
    }

    new_recipe["ingredients"] = ingredients;

    if (new_recipe["ingredients"].empty())
        return;

    // Read existing JSON data
    Json::Value root;
    std::ifstream file(selected_file);
    if (file) {
        file >> root;
        file.close();
    }

    // Append new recipe
    root["recipes"].append(new_recipe);

    // Write updated JSON to file
    std::ofstream outfile(selected_file);
    if (outfile) {
        outfile << customWriteJson(root);
        outfile.close();
        Gtk::MessageDialog dialog(*this, "Recipe saved successfully!", false,
                                  Gtk::MESSAGE_INFO);
        dialog.run();
    }
}

int main(int argc, char *argv[]) {
    auto app =
        Gtk::Application::create(argc, argv, "org.saldev.recipe_manager");
    RecipeManager window;
    return app->run(window);
}
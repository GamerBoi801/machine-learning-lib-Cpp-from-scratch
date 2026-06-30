#ifndef CSV_LOADER_H
#define CSV_LOADER_H

#include "dataset.h"
#include "matrix.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <stdexcept>
using namespace std;

class CSVLoader {
public:

    // ─────────────────────────────────────────────
    // MEMBER FIELDS  (shared state across all methods)
    // ─────────────────────────────────────────────

    vector<string>          headers;      // column names, in order, from row 0 of the CSV
    vector<vector<string>>  raw_data;     // every data row as raw strings — nothing converted yet
    map<string, string>     col_types;    // maps column name → "numeric" or "categorical"
    string                  target_col;   // name of the Y column (set during load)
    int                     target_col_index; // position of target in headers (set during load)

    // ─────────────────────────────────────────────
    // CONSTRUCTOR
    // ─────────────────────────────────────────────

    CSVLoader() : target_col_index(-1) {}

    // ─────────────────────────────────────────────
    // PRIVATE HELPERS
    // ─────────────────────────────────────────────
private:

    // Splits a single CSV line by commas.
    // Does NOT handle quoted fields containing commas — keep your CSV clean.
    vector<string> split_line(const string& line) {
        vector<string> tokens;
        stringstream ss(line);
        string token;
        while (getline(ss, token, ',')) {
            // trim leading/trailing whitespace and carriage returns (\r on Windows files)
            size_t start = token.find_first_not_of(" \t\r\n");
            size_t end   = token.find_last_not_of(" \t\r\n");
            if (start == string::npos) {
                tokens.push_back("");          // cell was entirely whitespace → treat as empty
            } else {
                tokens.push_back(token.substr(start, end - start + 1));
            }
        }
        return tokens;
    }

    // Returns true if a raw string cell counts as missing.
    bool is_missing(const string& cell) {
        static const set<string> missing_markers = {
            "", "NA", "na", "N/A", "n/a", "NULL", "null", "?", "NaN", "nan"
        };
        return missing_markers.count(cell) > 0;
    }

    // Tries to parse a string as double. Returns true on success.
    bool is_numeric_string(const string& s) {
        if (s.empty()) return false;
        try {
            size_t pos;
            stod(s, &pos);
            return pos == s.size();   // entire string was consumed → it's a number
        } catch (...) {
            return false;
        }
    }

// ─────────────────────────────────────────────
// PUBLIC METHODS
// ─────────────────────────────────────────────
public:

    // ─────────────────────────────────────────────
    // 1.  load()
    //
    // Opens the CSV file at `filepath`.
    // Reads the first line as column headers.
    // Stores every subsequent line as a row of raw strings in raw_data.
    // Finds and records the index of `target_column_name` in headers.
    // ─────────────────────────────────────────────
    void load(const string& filepath, const string& target_column_name) {
        ifstream file(filepath);
        if (!file.is_open()) {
            cerr << "[CSVLoader] ERROR: Could not open file: " << filepath << endl;
            return;
        }

        target_col = target_column_name;    
        string line;

        // ── Row 0: headers ──
        if (!getline(file, line)) {
            cerr << "[CSVLoader] ERROR: File is empty." << endl;
            return;
        }
        headers = split_line(line);

        // Find the target column index
        target_col_index = -1;
        for (int i = 0; i < (int)headers.size(); i++) {
            if (headers[i] == target_col) {
                target_col_index = i;
                break;
            }
        }
        if (target_col_index == -1) {
            cerr << "[CSVLoader] ERROR: Target column '" << target_col
                 << "' not found in headers." << endl;
            return;
        }

        // ── Remaining rows: data ──
        while (getline(file, line)) {
            if (line.empty()) continue;           // skip blank lines
            vector<string> row = split_line(line);

            // Pad or trim so every row has the same number of columns as the header
            row.resize(headers.size(), "");
            raw_data.push_back(row);
        }

        cout << "[CSVLoader] Loaded " << raw_data.size()
             << " rows, " << headers.size() << " columns." << endl;
        cout << "[CSVLoader] Target column: '" << target_col
             << "' at index " << target_col_index << endl;
    }

    // ─────────────────────────────────────────────
    // 2.  infer_types()
    //
    // For each column, tries to parse every cell as a double.
    // If all cells parse → "numeric".
    // If any cell fails  → "categorical".
    // Missing values are skipped during type inference (they don't decide the type).
    // Results stored in col_types map.
    // ─────────────────────────────────────────────
    void infer_types() {
        for (int j = 0; j < (int)headers.size(); j++) {
            bool all_numeric = true;

            for (int i = 0; i < (int)raw_data.size(); i++) {
                const string& cell = raw_data[i][j];
                if (is_missing(cell)) continue;           // skip missing — doesn't decide type
                if (!is_numeric_string(cell)) {
                    all_numeric = false;
                    break;                                 // one non-numeric cell → whole column is categorical
                }
            }

            col_types[headers[j]] = all_numeric ? "numeric" : "categorical";
        }

        // Print summary
        cout << "[CSVLoader] Type inference complete:" << endl;
        for (auto& kv : col_types) {
            cout << "  " << kv.first << " → " << kv.second << endl;
        }
    }

    // ─────────────────────────────────────────────
    // 3.  handle_missing()
    //
    // Pass 1: Drop any row where the TARGET column is missing.
    //         You cannot impute what you're trying to predict.
    //
    // Pass 2: For each non-target column:
    //   • numeric    → replace missing cells with the column mean
    //   • categorical → replace missing cells with the column mode (most frequent value)
    // ─────────────────────────────────────────────
    void handle_missing() {

        // ── Pass 1: drop rows with missing target ──
        int before = raw_data.size();
        raw_data.erase(
            remove_if(raw_data.begin(), raw_data.end(),
                [&](const vector<string>& row) {
                    return is_missing(row[target_col_index]);
                }),
            raw_data.end()
        );
        int dropped = before - (int)raw_data.size();
        if (dropped > 0) {
            cout << "[CSVLoader] Dropped " << dropped
                 << " rows with missing target value." << endl;
        }

        // ── Pass 2: impute feature columns ──
        for (int j = 0; j < (int)headers.size(); j++) {
            if (j == target_col_index) continue;   // never impute the target

            string col_name = headers[j];
            string col_type = col_types[col_name];

            if (col_type == "numeric") {
                // compute mean of all non-missing, non-target cells in column j
                double total = 0.0;
                int    count = 0;
                for (auto& row : raw_data) {
                    if (!is_missing(row[j])) {
                        total += stod(row[j]);
                        count++;
                    }
                }
                double mean_val = (count > 0) ? (total / count) : 0.0;
                string mean_str = to_string(mean_val);

                int imputed = 0;
                for (auto& row : raw_data) {
                    if (is_missing(row[j])) {
                        row[j] = mean_str;
                        imputed++;
                    }
                }
                if (imputed > 0) {
                    cout << "[CSVLoader] Imputed " << imputed
                         << " missing value(s) in numeric column '" << col_name
                         << "' with mean " << mean_val << endl;
                }

            } else {
                // categorical: find the mode (most frequent non-missing value)
                map<string, int> freq;
                for (auto& row : raw_data) {
                    if (!is_missing(row[j])) {
                        freq[row[j]]++;
                    }
                }

                string mode_val = "";
                int    mode_count = 0;
                for (auto& kv : freq) {
                    if (kv.second > mode_count) {
                        mode_count = kv.second;
                        mode_val   = kv.first;
                    }
                }

                int imputed = 0;
                for (auto& row : raw_data) {
                    if (is_missing(row[j])) {
                        row[j] = mode_val;
                        imputed++;
                    }
                }
                if (imputed > 0) {
                    cout << "[CSVLoader] Imputed " << imputed
                         << " missing value(s) in categorical column '" << col_name
                         << "' with mode '" << mode_val << "'" << endl;
                }
            }
        }

        cout << "[CSVLoader] Missing value handling complete. "
             << raw_data.size() << " rows remain." << endl;
    }

    // ─────────────────────────────────────────────
    // 4.  encode_categoricals()
    //
    // For every categorical column (except the target):
    //   1. Collect all unique values, sort them (so encoding is deterministic).
    //   2. For each unique value, append a new binary column to headers
    //      named  "originalCol_value"  (e.g. "color_red").
    //   3. For each row, append "1" if that row's value matches, "0" otherwise.
    //   4. Remove the original categorical column from headers and all rows.
    //
    // The target column is NEVER one-hot encoded here.
    // ─────────────────────────────────────────────
    void encode_categoricals() {
        // We iterate over a snapshot of the current headers so that
        // modifications during the loop don't cause index confusion.
        vector<string> original_headers = headers;

        // Track which original column indices to remove at the end
        vector<int> cols_to_remove;

        for (int j = 0; j < (int)original_headers.size(); j++) {
            string col_name = original_headers[j];

            // Only process categorical non-target columns
            if (col_types[col_name] != "categorical") continue;
            if (col_name == target_col)              continue;

            // ── Step 1: collect unique values ──
            set<string> unique_set;
            for (auto& row : raw_data) {
                unique_set.insert(row[j]);
            }
            vector<string> unique_vals(unique_set.begin(), unique_set.end());
            sort(unique_vals.begin(), unique_vals.end());  // deterministic order

            cout << "[CSVLoader] One-hot encoding '" << col_name
                 << "' (" << unique_vals.size() << " categories)" << endl;

            // ── Step 2 & 3: add new binary columns to headers and rows ──
            for (const string& val : unique_vals) {
                string new_col = col_name + "_" + val;   // e.g. "color_red"
                headers.push_back(new_col);
                col_types[new_col] = "numeric";           // binary column is numeric

                for (auto& row : raw_data) {
                    row.push_back((row[j] == val) ? "1" : "0");
                }
            }

            // ── Step 4: mark original column for removal ──
            cols_to_remove.push_back(j);
        }

        // ── Remove original categorical columns (back to front to preserve indices) ──
        sort(cols_to_remove.rbegin(), cols_to_remove.rend());
        for (int j : cols_to_remove) {
            headers.erase(headers.begin() + j);
            for (auto& row : raw_data) {
                row.erase(row.begin() + j);
            }
            // Adjust target_col_index if a column before it was removed
            if (j < target_col_index) {
                target_col_index--;
            }
        }

        cout << "[CSVLoader] Encoding complete. "
             << headers.size() << " columns total." << endl;
    }

    // ─────────────────────────────────────────────
    // 5.  to_dataset()
    //
    // Converts the fully-processed raw_data (all numeric strings) into
    // a Dataset object that the model already knows how to use.
    //
    // • Finds the (possibly updated) target column index.
    // • Parses every cell from string → double.
    // • Separates into X (features) and Y (target).
    // • Sets m (rows) and n (feature columns).
    // • Returns the Dataset — ready for add_bias_column() and normalize().
    // ─────────────────────────────────────────────
    Dataset to_dataset() {
        // Re-find target index in case it shifted during encode_categoricals
        target_col_index = -1;
        for (int j = 0; j < (int)headers.size(); j++) {
            if (headers[j] == target_col) {
                target_col_index = j;
                break;
            }
        }
        if (target_col_index == -1) {
            cerr << "[CSVLoader] ERROR: Target column lost after encoding. "
                 << "Cannot build Dataset." << endl;
            return Dataset();
        }

        int m = raw_data.size();
        int n = (int)headers.size() - 1;   // all columns except the target

        // Build X and Y as 2D double vectors
        vector<vector<double>> X_data(m, vector<double>(n, 0.0));
        vector<vector<double>> Y_data(m, vector<double>(1, 0.0));

        for (int i = 0; i < m; i++) {
            int x_col = 0;
            for (int j = 0; j < (int)headers.size(); j++) {
                if (j == target_col_index) {
                    Y_data[i][0] = stod(raw_data[i][j]);
                } else {
                    X_data[i][x_col++] = stod(raw_data[i][j]);
                }
            }
        }

        // Pack into Dataset
        Dataset ds;
        ds.m = m;
        ds.n = n;
        ds.X = Matrix(m, n);
        ds.Y = Matrix(m, 1);
        ds.X.matrix = X_data;
        ds.Y.matrix = Y_data;

        cout << "[CSVLoader] Dataset built: " << m << " samples, "
             << n << " features." << endl;

        return ds;
    }

    // ─────────────────────────────────────────────
    // UTILITY: print the first `num_rows` of raw_data with headers
    // Useful for debugging at any pipeline stage.
    // ─────────────────────────────────────────────
    void preview(int num_rows = 5) {
        cout << "\n[CSVLoader Preview — first " << num_rows << " rows]\n";
        for (const string& h : headers) cout << h << "\t";
        cout << endl;
        for (int i = 0; i < min(num_rows, (int)raw_data.size()); i++) {
            for (const string& cell : raw_data[i]) cout << cell << "\t";
            cout << endl;
        }
        cout << endl;
    }
};

#endif
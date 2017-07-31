#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <climits>

using namespace std;

class Table {
 public:
  Table(const string& name, const vector<string>& column_names, const vector<vector<string> > data) : name_(name), column_names_(column_names), data_(data) {}

  const string& name() const { return name_; }
  const vector<string>& column_names() const { return column_names_; }
  const vector<vector<string> >& data() const { return data_; }

  // Callee will own the returned pointer
  Table* Select(const vector<string>& column_names) const {
    // IMPLEMENT ME
	vector<size_t> projectedIndex;
	for (const auto &projectedColumnName : column_names) {
		size_t i = 0;
		while (i < column_names_.size() and column_names_.at(i) != projectedColumnName) {
			i++;
		}
		projectedIndex.push_back(i);
	}
	vector<vector<string>> projectedData;
	for (size_t i = 0; i < data_.size(); i++) {
		vector<string> row;
		for (const auto &j : projectedIndex) {
			if (j < column_names_.size()) {
				row.push_back(data_.at(i).at(j));
			}
			else {
				row.push_back("");
			}
		}
		projectedData.push_back(row);
	}
    return new Table("Select", column_names, projectedData);
  }

  // Callee will own the returned pointer
  Table* Where(const string& column_name, const string& value) const {
    // IMPLEMENT ME
	size_t i = 0;
	while (i < column_names_.size() and column_names_.at(i) != column_name) {
		i++;
	}
	if (i == column_names_.size()) {
		return new Table("", {}, {});
	}
	vector<vector<string>> projectedData;
	for (size_t j = 0; j < data_.size(); j++) {
		if (data_.at(j).at(i) == value) {
			projectedData.push_back(data_.at(j));
		}
	}
    return new Table("WhereTable", column_names_, projectedData);
  }

  void Print() {
    string output = JoinStringVector(column_names_);
    for (size_t row_index = 0; row_index < data_.size(); ++row_index) {
      output += JoinStringVector(data_[row_index]);
      output += "\n";
    }
    cout << output << endl;
  }

 private:
  string JoinStringVector(const vector<string>& input) {
    string output = "";
    bool has_data = false;
    for (size_t i = 0; i < input.size(); ++i) {
      has_data = true;
      output += input[i];
      output += ", ";
    }
    if (has_data) {
      output = output.substr(0, output.length() - 2);
    }
    return output;
  }
  const string name_;
  const vector<string> column_names_;
  const vector<vector<string> > data_; // only support string type for simplicity
};


class Database {
 public:
  Database(map<string, const Table*> table_map) : table_map_(table_map) {}
  ~Database() {
    for(map<string, const Table*>::iterator iterator = table_map_.begin(); iterator != table_map_.end(); ++iterator) {
      delete iterator->second;
    }
  }

  const Table* GetTable(const string& table_name) { return table_map_[table_name]; }

  // Callee will own the returned pointer
  Table* InnerJoin(
      const Table* left_table, const string& left_table_key_name,
      const Table* right_table, const string& right_table_key_name) {
    // IMPLEMENT ME
	size_t i = 0;
	while (i < left_table->column_names().size() and left_table->column_names().at(i) != left_table_key_name) {
		i++;
	}
	if (i == left_table->column_names().size()) {
		return new Table("InnerJoin", {}, {});
	}
	size_t j = 0;
	while (j < right_table->column_names().size() and right_table->column_names().at(j) != right_table_key_name) {
		j++;
	}
	if (j == right_table->column_names().size()) {
		return new Table("InnerJoin", {}, {});
	}
	vector<string> projectedColumnNames;
	for (const auto &s : left_table->column_names()) {
		projectedColumnNames.push_back(left_table->name() + '.' + s);
	}
	for (const auto &s : right_table->column_names()) {
		projectedColumnNames.push_back(right_table->name() + '.' + s);
	}
	vector<vector<string>> projectedData;
	for (size_t x = 0; x < left_table->data().size(); x++) {
		for (size_t y = 0; y < right_table->data().size(); y++) {
			vector<string> a(left_table->data().at(x)), b(right_table->data().at(y));
			if (a.at(i) == b.at(j)) {
				vector<string> row;
				row.insert(end(row), begin(a), end(a));
				row.insert(end(row), begin(b), end(b));
				projectedData.push_back(row);
			}
		}
	}
	return new Table("InnerJoin", projectedColumnNames, projectedData);
  }

  // Callee will own the returned pointer
  Table* LeftJoin(
      const Table* left_table, const string& left_table_key_name,
      const Table* right_table, const string& right_table_key_name) {
    // IMPLEMENT ME
	size_t i = 0;
	while (i < left_table->column_names().size() and left_table->column_names().at(i) != left_table_key_name) {
		i++;
	}
	if (i == left_table->column_names().size()) {
		return new Table("LeftJoin", {}, {});
	}
	size_t j = 0;
	while (j < right_table->column_names().size() and right_table->column_names().at(j) != right_table_key_name) {
		j++;
	}
	if (j == right_table->column_names().size()) {
		return new Table("LeftJoin", {}, {});
	}
	vector<string> projectedColumnNames;
	for (const auto &s : left_table->column_names()) {
		projectedColumnNames.push_back(left_table->name() + '.' + s);
	}
	for (const auto &s : right_table->column_names()) {
		projectedColumnNames.push_back(right_table->name() + '.' + s);
	}
	vector<vector<string>> projectedData;
	for (size_t x = 0; x < left_table->data().size(); x++) {
		bool found = false;
		for (size_t y = 0; y < right_table->data().size(); y++) {
			vector<string> a(left_table->data().at(x)), b(right_table->data().at(y));
			if (a.at(i) == b.at(j)) {
				found = true;
				vector<string> row;
				row.insert(end(row), begin(a), end(a));
				row.insert(end(row), begin(b), end(b));
				projectedData.push_back(row);
			}
		}
		if (!found) {
			vector<string> row;
			vector<string> c(left_table->data().at(x));
			row.insert(end(row), begin(c), end(c));
			for (size_t z = 0; z < right_table->data().front().size(); z++) {
				row.push_back("");
			}
			projectedData.push_back(row);
		}
	}
	return new Table("LeftJoin", projectedColumnNames, projectedData);
  }

  Table* RightJoin(
      const Table* left_table, const string& left_table_key_name,
      const Table* right_table, const string& right_table_key_name) {
    // IMPLEMENT ME
    return LeftJoin(right_table, right_table_key_name, left_table, left_table_key_name);
  }

  // Callee will own the returned pointer
  Table* OuterJoin(
      const Table* left_table, const string& left_table_key_name,
      const Table* right_table, const string& right_table_key_name) {
    // IMPLEMENT ME
	size_t i = 0;
	while (i < left_table->column_names().size() and left_table->column_names().at(i) != left_table_key_name) {
		i++;
	}
	if (i == left_table->column_names().size()) {
		return new Table("OuterJoin", {}, {});
	}
	size_t j = 0;
	while (j < right_table->column_names().size() and right_table->column_names().at(j) != right_table_key_name) {
		j++;
	}
	if (j == right_table->column_names().size()) {
		return new Table("OuterJoin", {}, {});
	}
	vector<string> projectedColumnNames;
	for (const auto &s : left_table->column_names()) {
		projectedColumnNames.push_back(left_table->name() + '.' + s);
	}
	for (const auto &s : right_table->column_names()) {
		projectedColumnNames.push_back(right_table->name() + '.' + s);
	}
	vector<vector<string>> projectedData;
	for (size_t x = 0; x < left_table->data().size(); x++) {
		bool found = false;
		for (size_t y = 0; y < right_table->data().size(); y++) {
			vector<string> a(left_table->data().at(x)), b(right_table->data().at(y));
			if (a.at(i) == b.at(j)) {
				found = true;
				vector<string> row;
				row.insert(end(row), begin(a), end(a));
				row.insert(end(row), begin(b), end(b));
				projectedData.push_back(row);
			}
		}
		if (!found) {
			vector<string> row;
			vector<string> c(left_table->data().at(x));
			row.insert(end(row), begin(c), end(c));
			for (size_t z = 0; z < right_table->data().front().size(); z++) {
				row.push_back("");
			}
			projectedData.push_back(row);
		}
	}
	for (size_t y = 0; y < right_table->data().size(); y++) {
		bool found = false;
		for (size_t x = 0; x < left_table->data().size(); x++) {
			if (left_table->data().at(x).at(i) == right_table->data().at(y).at(j)) {
				found = true;
			}
		}
		if (!found) {
			vector<string> row;
			for (size_t z = 0; z < left_table->data().front().size(); z++) {
				row.push_back("");
			}
			vector<string> d(right_table->data().at(y));
			row.insert(end(row), begin(d), end(d));
			projectedData.push_back(row);
		}
	}
	return new Table("OuterJoin", projectedColumnNames, projectedData);
  }

 private:
  map<string, const Table*> table_map_;
};

#ifndef __main__
#define __main__

int main(int argc, char* argv[]) {
  const Table* department_table = new Table(
      "departments",
      { "id", "name" },
      {
        { "0", "engineering" },
        { "1", "finance" }
      });

  const Table* user_table = new Table(
      "users",
      { "id", "department_id", "name" },
      {
        { "0", "0", "Ian" },
        { "1", "0", "John" },
        { "2", "1", "Eddie" },
        { "3", "1", "Mark" },
      });

  const Table* salary_table = new Table(
      "salaries",
      { "id", "user_id", "amount" },
      {
         { "0", "0", "100" },
         { "1", "1", "150" },
         { "2", "1", "200" },
         { "3", "3", "200" },
         { "4", "3", "300" },
         { "5", "4", "400" },
      });

  map<string, const Table*> table_map = map<string, const Table*>();
  table_map["departments"] = department_table;
  table_map["users"] = user_table;
  table_map["salaries"] = salary_table;
  Database* db = new Database(table_map);

  // should print
  // id, department_id, name
  // 1, 0, John
  {
    Table* filtered_table = db->GetTable("users")->Where("id", "1");
    Table* projected_table = filtered_table->Select({ "id", "department_id", "name" });
    projected_table->Print();
    delete projected_table;
    delete filtered_table;
  }

  // should print
  // users.name, departments.name
  // Ian, engineering
  // John, engineering
  {
    Table* table = db->InnerJoin(
        db->GetTable("users"),
        "department_id",
        db->GetTable("departments"),
        "id");
    Table* filtered_table = table->Where("departments.name", "engineering");
    Table* projected_table = filtered_table->Select({ "users.name", "departments.name" });
    projected_table->Print();
    delete projected_table;
    delete filtered_table;
    delete table;
  }

  // should print
  // users.name, salaries.amount
  // Ian, 100
  // John, 150
  // John, 200
  // Mark, 200
  // Mark, 300
  // Eddie,
  {
    Table* table = db->LeftJoin(
        db->GetTable("users"),
        "id",
        db->GetTable("salaries"),
        "user_id");
    Table* projected_table = table->Select({ "users.name", "salaries.amount" });
    projected_table->Print();
    delete projected_table;
    delete table;
  }

  // should print
  // users.name, salaries.amount
  // Ian, 100
  // John, 150
  // John, 200
  // Mark, 200
  // Mark, 300
  // , 400
  {
    Table* table = db->RightJoin(
        db->GetTable("users"),
        "id",
        db->GetTable("salaries"),
        "user_id");
    Table* projected_table = table->Select({ "users.name", "salaries.amount" });
    projected_table->Print();
    delete projected_table;
    delete table;
  }

  // should print
  // users.name, salaries.amount
  // Ian, 100
  // John, 150
  // John, 200
  // Mark, 200
  // Mark, 300
  // Eddie,
  // , 400
  {
    Table* table = db->OuterJoin(
        db->GetTable("users"),
        "id",
        db->GetTable("salaries"),
        "user_id");
    Table* projected_table = table->Select({ "users.name", "salaries.amount" });
    projected_table->Print();
    delete projected_table;
    delete table;
  }

  delete db;
}

#endif

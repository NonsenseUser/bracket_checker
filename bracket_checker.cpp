#include <chrono>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <vector>
using namespace std;
using namespace std::chrono;

struct Bracket {
  string type;
  int line;
  int pos;
};

// Function to check if two characters are a matching pair
bool isMatchingPair(const string &opening, const string &closing) {
  return (opening == "(" && closing == ")") ||
         (opening == "{" && closing == "}") ||
         (opening == "[" && closing == "]") ||
         (opening == "(*" && closing == "*)");
}

bool processLine(const string &expr, int lineNumber, stack<Bracket> &s) {
  for (size_t i = 0; i < expr.length(); ++i) {
    if (i > 0 && expr[i - 1] == '*' && expr[i] == ')')
      continue;

    string c(1, expr[i]);
    string combined = "";
    if (i + 1 < expr.length())
      combined = c + expr[i + 1];

    // Check for opening brackets
    if (combined == "(*") {
      s.push({combined, lineNumber, static_cast<int>(i)});
      i++;
      continue;
    } else if (c == "(" || c == "{" || c == "[") {
      s.push({c, lineNumber, static_cast<int>(i)});
    }
    // Check for closing brackets
    else if (combined == "*)") {
      if (s.empty()) {
        cout << "Line " << lineNumber << ": Error: Closing '*)' at position "
             << i << " without matching opening\n";
        cout << expr << "\n" << string(i, ' ') << "^\n";
        return false;
      }

      Bracket top = s.top();
      s.pop();
      if (!isMatchingPair(top.type, combined)) {
        cout << "Line " << lineNumber << ": Mismatch: '" << top.type
             << "' (line " << top.line << ":" << top.pos
             << ") vs '*)' at position " << i << "\n";
        cout << expr << "\n" << string(i, ' ') << "^\n";
        return false;
      }
      i++;
    } else if (c == ")" || c == "}" || c == "]") {
      if (s.empty()) {
        cout << "Line " << lineNumber << ": Error: Closing '" << c
             << "' at position " << i << " without opening\n";
        cout << expr << "\n" << string(i, ' ') << "^\n";
        return false;
      }

      Bracket top = s.top();
      s.pop();
      if (!isMatchingPair(top.type, c)) {
        cout << "Line " << lineNumber << ": Mismatch: '" << top.type
             << "' (line " << top.line << ":" << top.pos << ") vs '" << c
             << "' at position " << i << "\n";
        cout << expr << "\n" << string(i, ' ') << "^\n";
        return false;
      }
    }
  }
  return true;
}

int main() {
  vector<string> testFiles = {"random.js",
                              "palindrom.py",
                              "machine_learning.py",
                              "calculator.cpp",
                              "ceasar_cipher.py",
                              "database.py",
                              "guess_game.cpp",
                              "server.py",
                              "calculator.js",
                              "random.js",
                              "chat.js",
                              "data_vizualization.py",
                              "email.py",
                              "file_integrity.py",
                              "image.py",
                              "passwords.py",
                              "speech.py",
                              "weather.py",
                              "backup.sh",
                              "convert.cs",
                              "emoji.js",
                              "formatter.rs",
                              "game.cpp",
                              "log_analyzer.go",
                              "machine_learning_2.py",
                              "notifications.kt",
                              "post.py",
                              "scraper.py",
                              "stock.js",
                              "task.sh",
                              "video.py"};

  for (const auto &filePath : testFiles) {

    ifstream inputFile("samples/" + filePath);
    if (!inputFile) {
      cerr << "Error opening: " << filePath << "\n";
      continue;
    }

    vector<string> lines;
    string line;
    while (getline(inputFile, line))
      lines.push_back(line);
    inputFile.close();

    stack<Bracket> bracketStack;
    bool hasErrors = false;
    auto start = high_resolution_clock::now();

    for (size_t i = 0; i < lines.size() && !hasErrors; ++i) {
      if (!processLine(lines[i], i + 1, bracketStack)) {
        hasErrors = true;
      }
    }

    // Check remaining unclosed brackets
    if (!hasErrors && !bracketStack.empty()) {
      hasErrors = true;
      while (!bracketStack.empty()) {
        Bracket b = bracketStack.top();
        cout << "Line " << b.line << ": Unclosed '" << b.type
             << "' at position " << b.pos << "\n";
        cout << lines[b.line - 1] << "\n" << string(b.pos, ' ') << "^\n";
        bracketStack.pop();
      }
    }

    auto duration =
        duration_cast<microseconds>(high_resolution_clock::now() - start);

    cout << "File: " << filePath << " - " << lines.size() << " lines - "
         << (hasErrors ? "ERRORS FOUND" : "ALL BRACKETS BALANCED")
         << " (Processed in " << duration.count() << "ms)\n\n";
  }

  return 0;
}


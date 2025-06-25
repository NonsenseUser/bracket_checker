#include <fstream>
#include <iostream>
#include <stack>
#include <string>

using namespace std;

// Function to check if a character is an opening bracket
bool isOpening(string c) {
  return c == "(" || c == "{" || c == "[" || c == "(*";
}

// Function to check if two characters are a matching pair
bool isMatchingPair(string opening, string closing) {
  return (opening == "(" && closing == ")") ||
         (opening == "{" && closing == "}") ||
         (opening == "[" && closing == "]") ||
         (opening == "(*" && closing == "*)");
}

// Function to check if parentheses are balanced in a string
bool areParenthesesBalanced(const string &expr, ofstream &errorFile,
                            int lineNumber) {
  stack<string> s;
  bool hasError = false;

  for (size_t i = 0; i < expr.length(); ++i) {
    if (i > 0 && expr[i - 1] == '*' && expr[i] == ')') {
      continue;
    }
    string c = string(1, expr[i]);
    string combined = "";
    if ((i + 1 < expr.length())) {
      combined = c + expr[i + 1];
    }

    if (combined == "(*") {
      s.push(combined);
      i++; // Advance past the second character of "(*"
    } else if (c == "(" || c == "{" || c == "[") {
      s.push(c);
    } else if (combined == "*)") {
      if (s.empty()) {
        errorFile << "Line " << lineNumber << ": Error: Closing bracket '"
                  << combined << "' at position " << i
                  << " without a matching opening bracket." << endl;
        errorFile << expr << endl;
        for (size_t j = 0; j < i; ++j)
          errorFile << " ";
        errorFile << "^-- Error here" << endl << endl;
        hasError = true;
        return false;
      }
      if (!isMatchingPair(s.top(), combined)) {
        errorFile << "Line " << lineNumber << ": Error: Mismatched brackets: '"
                  << s.top() << "' and '" << combined << "' at position " << i
                  << "." << endl;
        errorFile << expr << endl;
        for (size_t j = 0; j < i; ++j)
          errorFile << " ";
        errorFile << "^-- Error here" << endl << endl;
        hasError = true;
        return false;
      }
      s.pop();
      i++; // Advance past the second character of "*)"
    } else if (c == ")" || c == "}" || c == "]") {
      if (s.empty()) {
        errorFile << "Line " << lineNumber << ": Error: Closing bracket '" << c
                  << "' at position " << i
                  << " without a matching opening bracket." << endl;
        errorFile << expr << endl;
        for (size_t j = 0; j < i; ++j)
          errorFile << " ";
        errorFile << "^-- Error here" << endl << endl;
        hasError = true;
        return false;
      }

      if (!isMatchingPair(s.top(), c)) {
        errorFile << "Line " << lineNumber << ": Error: Mismatched brackets: '"
                  << s.top() << "' and '" << c << "' at position " << i << "."
                  << endl;
        errorFile << expr << endl;
        for (size_t j = 0; j < i; ++j)
          errorFile << " ";
        errorFile << "^-- Error here" << endl << endl;
        hasError = true;
        return false;
      }
      s.pop();
    }
  }

  while (!s.empty()) {
    errorFile << "Line " << lineNumber << ": Error: Unclosed opening bracket '"
              << s.top() << "'." << endl;
    errorFile << expr << endl;
    errorFile << "^-- Unclosed bracket" << endl << endl;
    s.pop();
    hasError = true;
    return false;
  }
  return !hasError;
}

int main() {
  string filePath;
  cout << "Enter the path to the file: ";
  cin >> filePath;

  ifstream inputFile(filePath);
  if (!inputFile.is_open()) {
    cerr << "Error: Unable to open file at path: " << filePath << endl;
    return 1;
  }

  ofstream errorFile("bracket_errors.txt");
  if (!errorFile.is_open()) {
    cerr << "Error: Unable to open error file." << endl;
    return 1;
  }

  string line;
  int lineNumber = 1;
  bool isBalanced = true;

  while (getline(inputFile, line)) {
    if (!areParenthesesBalanced(line, errorFile, lineNumber)) {
      isBalanced = false;
    }
    lineNumber++;
  }

  inputFile.close();
  errorFile.close();

  if (isBalanced) {
    cout << "The brackets in the file are balanced." << endl;
  } else {
    cout << "The brackets in the file are not balanced. See bracket_errors.txt "
            "for details."
         << endl;
  }

  return 0;
}


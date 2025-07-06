# Mini Arithmetic Expression Evaluator using C++

A comprehensive arithmetic expression evaluator with a beautiful web interface and robust C++ backend.

## Features

- **Lexical Analysis**: Tokenizes C/C++ code into meaningful tokens
- **Syntax Analysis**: Converts expressions to postfix notation
- **Expression Evaluation**: Computes the final result
- **Beautiful Web UI**: Modern, responsive interface
- **Real-time Processing**: Instant feedback and results

## Project Structure

```
├── backend/
│   ├── src/
│   │   ├── lexer.cpp
│   │   ├── parser.cpp
│   │   ├── evaluator.cpp
│   │   └── main.cpp
│   ├── include/
│   │   ├── lexer.h
│   │   ├── parser.h
│   │   └── evaluator.h
│   └── Makefile
├── frontend/
│   ├── index.html
│   ├── styles.css
│   └── script.js
└── README.md
```

## How to Use

1. **Compile the Backend**:
   ```bash
   cd backend
   make
   ```

2. **Run the Application**:
   ```bash
   ./arithmetic_evaluator
   ```

3. **Open the Web Interface**:
   - Open `frontend/index.html` in your browser
   - Or serve it using a local server

## Input Format

The system accepts C/C++ style expressions:
```cpp
int a = 5;
int b = 10;
int sum = a + b * 2;
cout << sum << endl;
```

## Output Format

The system provides detailed analysis:
- **Input Expression**: The original code
- **Token Name**: Type of token (Keyword, Identifier, Operator, etc.)
- **Postfix Conversion**: Expression in postfix notation
- **Output**: Final computed result

## Technologies Used

- **Backend**: C++ (Lexical Analysis, Parsing, Evaluation)
- **Frontend**: HTML5, CSS3, JavaScript
- **Build System**: Make

## License

MIT License 
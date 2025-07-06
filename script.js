// DOM Elements
const codeInput = document.getElementById('codeInput');
const clearBtn = document.getElementById('clearBtn');
const exampleBtn = document.getElementById('exampleBtn');
const evaluateBtn = document.getElementById('evaluateBtn');
const resultsSection = document.getElementById('resultsSection');
const loadingSection = document.getElementById('loadingSection');

// Tab elements
const tabBtns = document.querySelectorAll('.tab-btn');
const resultTabs = document.querySelectorAll('.result-tab');
const progressSteps = document.querySelectorAll('.progress-step');

// Table and display elements
const lexicalTableBody = document.getElementById('lexicalTableBody');
const infixExpression = document.getElementById('infixExpression');
const postfixExpression = document.getElementById('postfixExpression');
const variablesTable = document.getElementById('variablesTable');
const finalResult = document.getElementById('finalResult');
const evaluationSteps = document.getElementById('evaluationSteps');

// Example code
const exampleCode = `int a = 5;
int b = 10;
float f = 2.5;
double d = 35.735;
string s = "Hello";
char c = 'A';
int sum = a + b;
float product = f * a;
double total = d + f + b;
string greet = s + " World";
cout << sum << endl;
cout << product << endl;
cout << total << endl;
cout << greet << endl;`;

// Example with errors for testing
const errorExampleCode = `int a = 5
int 123b = 10;
int sum = a + b * 2;
cout << sum << endl;`;

// Global variables for lexical analysis
const keywords = ['int', 'float', 'double', 'char', 'string', 'bool', 'if', 'else', 'for', 'while', 'return', 'cout', 'cin', 'endl'];
const operators = ['+', '-', '*', '/', '%', '^', '=', '<<', '>>', '==', '!=', '<', '>', '<=', '>='];

// Note: Mock data has been removed as we now use dynamic processing based on user input

// Event Listeners
clearBtn.addEventListener('click', clearCode);
exampleBtn.addEventListener('click', loadExample);
document.getElementById('errorExampleBtn').addEventListener('click', loadErrorExample);
evaluateBtn.addEventListener('click', evaluateCode);

// Tab navigation
tabBtns.forEach(btn => {
    btn.addEventListener('click', () => {
        const targetTab = btn.getAttribute('data-tab');
        switchTab(targetTab);
    });
});

// Functions
function clearCode() {
    codeInput.value = '';
    hideResults();
}

function loadExample() {
    codeInput.value = exampleCode;
}

function loadErrorExample() {
    codeInput.value = errorExampleCode;
}

function evaluateCode() {
    const code = codeInput.value.trim();
    if (!code) {
        showError('Please enter some code to evaluate.');
        return;
    }

    // Basic validation
    if (!validateCode(code)) {
        return;
    }

    showLoading();
    
    // Simulate backend processing delay
    setTimeout(() => {
        hideLoading();
        showResults();
        populateResults(code);
    }, 1000);
}

function validateCode(code) {
    const lines = code.split('\n');
    
    // Check for basic syntax errors
    for (let i = 0; i < lines.length; i++) {
        const line = lines[i].trim();
        if (line === '') continue;
        
        // Check for missing semicolons in variable declarations
        if ((line.includes('int ') || line.includes('float ') || line.includes('double ') || line.includes('char ') || line.includes('string ')) && line.includes('=') && !line.endsWith(';')) {
            showError(`❌ Error on line ${i + 1}: Missing semicolon at the end of the line.\n\nLine: ${line}\n\nPlease add a semicolon (;) at the end.`);
            return false;
        }
        
        // Check for invalid variable names
        if ((line.includes('int ') || line.includes('float ') || line.includes('double ') || line.includes('char ') || line.includes('string ')) && line.includes('=')) {
            const match = line.match(/(int|float|double|char|string)\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*=/);
            if (!match) {
                showError(`❌ Error on line ${i + 1}: Invalid variable declaration.\n\nLine: ${line}\n\nPlease use a valid variable name (starts with letter or underscore).`);
                return false;
            }
        }
        
        // Check for invalid characters (but allow quotes for strings and chars)
        // First, temporarily replace string and char literals to avoid false positives
        let tempLine = line;
        
        // Replace string literals with placeholders
        tempLine = tempLine.replace(/"[^"]*"/g, 'STRING_LITERAL');
        
        // Replace char literals with placeholders
        tempLine = tempLine.replace(/'[^']*'/g, 'CHAR_LITERAL');
        
        // Now check for invalid characters in the modified line (allow decimal points for float/double)
        const invalidChars = tempLine.match(/[^a-zA-Z0-9_+\-*/=<>!&|^; \t.]/g);
        if (invalidChars) {
            const uniqueChars = [...new Set(invalidChars)];
            showError(`❌ Error on line ${i + 1}: Invalid character(s) found: ${uniqueChars.join(', ')}\n\nLine: ${line}\n\nPlease use only valid C++ characters.`);
            return false;
        }
        
        // Check for empty variable names
        if ((line.includes('int ') || line.includes('float ') || line.includes('double ') || line.includes('char ') || line.includes('string ')) && line.includes('=')) {
            const parts = line.split('=');
            const varPart = parts[0].replace(/(int|float|double|char|string)/, '').trim();
            if (varPart === '') {
                showError(`❌ Error on line ${i + 1}: Empty variable name.\n\nLine: ${line}\n\nPlease provide a valid variable name.`);
                return false;
            }
        }
    }
    
    return true;
}

function showError(message) {
    // Hide any existing results
    hideResults();
    
    // Create error display
    const errorDiv = document.createElement('div');
    errorDiv.className = 'error-section';
    errorDiv.innerHTML = `
        <div class="error-content">
            <h3><i class="fas fa-exclamation-triangle"></i> Error</h3>
            <div class="error-message">${message.replace(/\n/g, '<br>')}</div>
            <button class="btn btn-primary" onclick="this.parentElement.parentElement.remove()">
                <i class="fas fa-times"></i> Close
            </button>
        </div>
    `;
    
    // Insert error after input section
    const inputSection = document.querySelector('.input-section');
    inputSection.parentNode.insertBefore(errorDiv, inputSection.nextSibling);
    
    // Scroll to error
    errorDiv.scrollIntoView({ behavior: 'smooth' });
}

function showLoading() {
    loadingSection.style.display = 'block';
    resultsSection.style.display = 'none';
}

function hideLoading() {
    loadingSection.style.display = 'none';
}

function showResults() {
    resultsSection.style.display = 'block';
    resultsSection.scrollIntoView({ behavior: 'smooth' });
}

function hideResults() {
    resultsSection.style.display = 'none';
}

function populateResults(code) {
    // Process the user's input code
    const processedData = processUserInput(code);
    
    // Populate lexical analysis
    populateLexicalTable(processedData.lexical);
    
    // Populate parsing results
    populateParsingResults(processedData.parsing);
    
    // Populate evaluation results
    populateEvaluationResults(processedData.evaluation);
    
    // Update progress steps
    updateProgressSteps();
}

function populateLexicalTable(lexicalData) {
    lexicalTableBody.innerHTML = '';
    
    lexicalData.forEach(token => {
        const row = document.createElement('tr');
        row.innerHTML = `
            <td>${token.expression}</td>
            <td>${token.tokenName}</td>
            <td>${token.line}</td>
        `;
        lexicalTableBody.appendChild(row);
    });
}

function populateParsingResults(parsingData) {
    // Show all expressions
    infixExpression.innerHTML = '';
    postfixExpression.innerHTML = '';
    variablesTable.innerHTML = '';
    
    parsingData.forEach((exprData, idx) => {
        const infixDiv = document.createElement('div');
        infixDiv.textContent = `${exprData.expr} (Infix: ${exprData.infix})`;
        infixExpression.appendChild(infixDiv);
        
        const postfixDiv = document.createElement('div');
        postfixDiv.textContent = `Postfix: ${exprData.postfix}`;
        postfixExpression.appendChild(postfixDiv);
        
        const varsDiv = document.createElement('div');
        varsDiv.className = 'variable-item';
        varsDiv.innerHTML = exprData.variables.map(v => `<span class="variable-name">${v.name}</span>: <span class="variable-value">${v.value}</span>`).join(', ');
        variablesTable.appendChild(varsDiv);
    });
}

function populateEvaluationResults(evaluationData) {
    finalResult.innerHTML = '';
    evaluationSteps.innerHTML = '';
    
    evaluationData.results.forEach(res => {
        const resDiv = document.createElement('div');
        resDiv.className = 'step-item';
        resDiv.textContent = `${res.expr} => ${res.result}`;
        finalResult.appendChild(resDiv);
    });
    
    evaluationData.steps.forEach(step => {
        const stepItem = document.createElement('div');
        stepItem.className = 'step-item';
        stepItem.textContent = step;
        evaluationSteps.appendChild(stepItem);
    });
}

function updateProgressSteps() {
    progressSteps.forEach((step, index) => {
        setTimeout(() => {
            step.classList.add('active');
        }, index * 500);
    });
}

function switchTab(targetTabId) {
    // Remove active class from all tabs and buttons
    resultTabs.forEach(tab => tab.classList.remove('active'));
    tabBtns.forEach(btn => btn.classList.remove('active'));
    
    // Add active class to target tab and button
    document.getElementById(targetTabId).classList.add('active');
    document.querySelector(`[data-tab="${targetTabId}"]`).classList.add('active');
}

// Main function to process user input and generate dynamic results
function processUserInput(code) {
    const lines = code.split('\n').filter(line => line.trim() !== '');
    const lexicalData = [];
    const parsingData = [];
    const evaluationData = {
        results: [],
        steps: []
    };
    
    let lineNumber = 1;
    let variables = {};
    let expressions = [];
    
    // Process each line
    lines.forEach((line, index) => {
        const trimmedLine = line.trim();
        if (trimmedLine === '') return;
        
        // Lexical Analysis
        const tokens = tokenizeLine(trimmedLine, lineNumber);
        lexicalData.push(...tokens);
        
        // Extract variables and expressions
        const lineData = processLine(trimmedLine, lineNumber, variables);
        if (lineData.variables) {
            Object.assign(variables, lineData.variables);
        }
        if (lineData.expression) {
            expressions.push(lineData.expression);
        }
        
        lineNumber++;
    });
    
    // Generate parsing data from expressions
    expressions.forEach((expr, index) => {
        const infix = extractInfixExpression(expr.expression);
        const postfix = infixToPostfix(infix);
        const exprVariables = getExpressionVariables(infix, variables);
        
        parsingData.push({
            infix: infix.join(' '),
            postfix: postfix.join(' '),
            variables: exprVariables,
            result: evaluateExpression(postfix, variables),
            type: expr.type,
            expr: expr.fullLine
        });
    });
    
    // Generate evaluation data
    parsingData.forEach((exprData, index) => {
        evaluationData.results.push({
            expr: exprData.expr,
            result: exprData.result
        });
        evaluationData.steps.push(`Step ${index + 1}: Evaluate ${exprData.infix} = ${exprData.result}`);
    });
    
    return {
        lexical: lexicalData,
        parsing: parsingData,
        evaluation: evaluationData
    };
}

// Tokenize a single line for lexical analysis
function tokenizeLine(line, lineNumber) {
    const tokens = [];
    
    // Handle string literals first
    let tempLine = line;
    const stringMatches = tempLine.match(/"[^"]*"/g) || [];
    stringMatches.forEach((str, index) => {
        tempLine = tempLine.replace(str, `STRING_LITERAL_${index}`);
        tokens.push({
            expression: str,
            tokenName: 'String',
            line: lineNumber
        });
    });
    
    // Handle char literals
    const charMatches = tempLine.match(/'[^']*'/g) || [];
    charMatches.forEach((chr, index) => {
        tempLine = tempLine.replace(chr, `CHAR_LITERAL_${index}`);
        tokens.push({
            expression: chr,
            tokenName: 'String',
            line: lineNumber
        });
    });
    
    // Split remaining line into words
    const words = tempLine.split(/\s+/);
    
    words.forEach(word => {
        if (word === '') return;
        
        // Remove semicolons and other delimiters for token analysis
        const cleanWord = word.replace(/[;()]/g, '');
        if (cleanWord === '') return;
        
        // Skip our placeholders
        if (cleanWord.startsWith('STRING_LITERAL_') || cleanWord.startsWith('CHAR_LITERAL_')) {
            return;
        }
        
        let tokenType = 'Identifier';
        
        // Check if it's a keyword
        if (keywords.includes(cleanWord)) {
            tokenType = 'Keyword';
        }
        // Check if it's a number
        else if (/^\d+(\.\d+)?$/.test(cleanWord)) {
            tokenType = 'Number';
        }
        // Check if it's an operator
        else if (operators.includes(cleanWord)) {
            tokenType = 'Operator';
        }
        // Check if it's a delimiter
        else if (/^[;(){}[\]]$/.test(cleanWord)) {
            tokenType = 'Delimiter';
        }
        
        tokens.push({
            expression: cleanWord,
            tokenName: tokenType,
            line: lineNumber
        });
    });
    
    // Add operators and delimiters that might be part of the word
    const operatorsInLine = line.match(/[+\-*/%=<>!&|^]+/g) || [];
    operatorsInLine.forEach(op => {
        if (!tokens.some(t => t.expression === op)) {
            tokens.push({
                expression: op,
                tokenName: 'Operator',
                line: lineNumber
            });
        }
    });
    
    return tokens;
}

// Process a single line to extract variables and expressions
function processLine(line, lineNumber, variables) {
    const result = { variables: {}, expression: null };
    
    // Check for variable declaration and assignment
    const varMatch = line.match(/(int|float|double|char|string)\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*(.+);/);
    if (varMatch) {
        const type = varMatch[1];
        const varName = varMatch[2];
        const value = varMatch[3].trim();
        
        // Store variable with proper type handling
        if (type === 'float' || type === 'double') {
            // For float and double, ensure decimal values are properly handled
            const numValue = parseFloat(value);
            if (!isNaN(numValue)) {
                result.variables[varName] = numValue.toString();
            } else {
                result.variables[varName] = value;
            }
        } else {
            result.variables[varName] = value;
        }
        
        // Check if the value is an expression
        if (value.match(/[+\-*/^]/)) {
            result.expression = {
                type: type,
                expression: value,
                fullLine: line
            };
        }
    }
    
    // Check for expression assignment (without type declaration)
    const exprMatch = line.match(/([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*(.+);/);
    if (exprMatch && !result.expression) {
        const varName = exprMatch[1];
        const expression = exprMatch[2].trim();
        
        if (expression.match(/[+\-*/^]/)) {
            result.expression = {
                type: 'auto',
                expression: expression,
                fullLine: line
            };
        }
    }
    
    return result;
}

// Extract infix expression from a string
function extractInfixExpression(expression) {
    return expression.split(/\s+/).filter(token => token !== '');
}

// Convert infix to postfix using Shunting Yard algorithm
function infixToPostfix(infix) {
    const output = [];
    const operators = [];
    const precedence = {
        '^': 3,
        '*': 2, '/': 2, '%': 2,
        '+': 1, '-': 1
    };
    
    infix.forEach(token => {
        if (/^\d+(\.\d+)?$/.test(token) || /^[a-zA-Z_][a-zA-Z0-9_]*$/.test(token)) {
            // Operand
            output.push(token);
        } else if (token in precedence) {
            // Operator
            while (operators.length > 0 && 
                   operators[operators.length - 1] in precedence &&
                   precedence[operators[operators.length - 1]] >= precedence[token]) {
                output.push(operators.pop());
            }
            operators.push(token);
        }
    });
    
    while (operators.length > 0) {
        output.push(operators.pop());
    }
    
    return output;
}

// Get variables used in an expression
function getExpressionVariables(infix, variables) {
    const exprVars = [];
    infix.forEach(token => {
        if (/^[a-zA-Z_][a-zA-Z0-9_]*$/.test(token) && variables[token]) {
            exprVars.push({
                name: token,
                value: variables[token]
            });
        }
    });
    return exprVars;
}

// Evaluate postfix expression
function evaluateExpression(postfix, variables) {
    const stack = [];
    
    postfix.forEach(token => {
        if (/^\d+(\.\d+)?$/.test(token)) {
            // Number - handle both integers and decimals
            stack.push(parseFloat(token));
        } else if (/^[a-zA-Z_][a-zA-Z0-9_]*$/.test(token)) {
            // Variable
            const value = variables[token];
            if (value !== undefined && value !== null) {
                // Try to parse as number first
                const numValue = parseFloat(value);
                if (!isNaN(numValue)) {
                    stack.push(numValue);
                } else {
                    // If not a number, check if it's a string or char literal
                    if (typeof value === 'string' && (value.startsWith('"') || value.startsWith("'"))) {
                        stack.push(value); // Keep as string for display
                    } else {
                        stack.push(0); // Default value
                    }
                }
            } else {
                stack.push(0); // Default value
            }
        } else {
            // Operator
            const b = stack.pop();
            const a = stack.pop();
            
            // Check if operands are numbers
            if (typeof a === 'number' && typeof b === 'number') {
                switch (token) {
                    case '+': stack.push(a + b); break;
                    case '-': stack.push(a - b); break;
                    case '*': stack.push(a * b); break;
                    case '/': stack.push(a / b); break;
                    case '%': stack.push(a % b); break;
                    case '^': stack.push(Math.pow(a, b)); break;
                    default: stack.push(0);
                }
            } else {
                // Handle string concatenation or other operations
                if (token === '+' && (typeof a === 'string' || typeof b === 'string')) {
                    stack.push(String(a) + String(b));
                } else {
                    stack.push(0);
                }
            }
        }
    });
    
    return stack.length > 0 ? stack[0].toString() : '0';
}

// Real backend communication (for future implementation)
async function sendToBackend(code) {
    try {
        // In a real implementation, this would send the code to the C++ backend
        // For now, we'll simulate the response
        const response = await fetch('/api/evaluate', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ code: code })
        });
        
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        
        return await response.json();
    } catch (error) {
        console.error('Error communicating with backend:', error);
        // Fall back to dynamic processing for demonstration
        return processUserInput(code);
    }
}

// Keyboard shortcuts
document.addEventListener('keydown', (e) => {
    // Ctrl+Enter to evaluate
    if (e.ctrlKey && e.key === 'Enter') {
        evaluateCode();
    }
    
    // Ctrl+L to load example
    if (e.ctrlKey && e.key === 'l') {
        e.preventDefault();
        loadExample();
    }
    
    // Ctrl+K to clear
    if (e.ctrlKey && e.key === 'k') {
        e.preventDefault();
        clearCode();
    }
});

// Auto-resize textarea
codeInput.addEventListener('input', function() {
    this.style.height = 'auto';
    this.style.height = Math.max(200, this.scrollHeight) + 'px';
});

// Initialize
document.addEventListener('DOMContentLoaded', () => {
    // Set initial textarea height
    codeInput.style.height = '200px';
    
    // Add some visual feedback for the interface
    console.log('Mini Arithmetic Expression Evaluator loaded successfully!');
    console.log('Keyboard shortcuts:');
    console.log('- Ctrl+Enter: Evaluate code');
    console.log('- Ctrl+L: Load example');
    console.log('- Ctrl+K: Clear code');
}); 
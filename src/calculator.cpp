#include "calculator.h"

QVector<Token> tokenize(const QString& expression) {
    QVector<Token> tokens;
    int i = 0;

    while(i < expression.length()) {
        if(expression[i].isSpace()) {
            i++;
            continue;
        }

        else if(expression[i].isDigit() || expression[i] == '.') {
            QString numberText;

            while(i < expression.length() && (expression[i].isDigit() || expression[i] == '.')) {
                numberText += expression[i];
                i++;
            }

            double number = numberText.toDouble();
            
            Token oneToken;
            oneToken.type = TokenType::Number;
            oneToken.numberValue = number;

            tokens.push_back(oneToken);
        }

        else if(expressionSign.contains(expression[i])) {
            Token oneToken;
            oneToken.type = expressionSign[expression[i]];

            tokens.push_back(oneToken);
            i++;
        }

        else if(expression[i].isLetter()) {
            QString identifier;
            Token oneToken;

            while(i < expression.length() && expression[i].isLetterOrNumber()) {
                identifier += expression[i];

                i++;
            }
            
            oneToken.type = TokenType::Identifier;
            oneToken.textValue = identifier;

            tokens.push_back(oneToken);
        }
    }

    return tokens;
}

Parser::Parser(const QVector<Token>& tokens, const QVector<UserFunction>& functions, const QMap<QString, double>& variables) :
    tokens(tokens),
    pos(0),
    userFunctions(functions),
    variables(variables) {

}

Token Parser::currentToken() {
    if(pos >= tokens.size()) {
        Token endToken;
        endToken.type = TokenType::End;

        return endToken;
    }

    return tokens[pos];
}

void Parser::advance() {
    pos++;
}

double Parser::parseFactor() {
    auto thisToken = currentToken();

    if(thisToken.type == TokenType::Number) {
        advance();
        return thisToken.numberValue;
    }

    else if(thisToken.type == TokenType::Minus) {
        advance();
        return -parseFactor();
    }

    else if(thisToken.type == TokenType::LeftParen) {
        advance();
        double parseResult = parseExpression();
        advance();

        return parseResult;
    }

    else if(thisToken.type == TokenType::Identifier) {
        QString name = thisToken.textValue;
        advance();

        auto nextToken = currentToken();

        if(nextToken.type == TokenType::LeftParen) {
            advance();
            QVector<double> args;

            if(currentToken().type != TokenType::RightParen) {
                while(true) {
                    double arg = parseExpression();
                    args.push_back(arg);

                    if(currentToken().type == TokenType::Comma) {
                        advance();
                        continue;
                    }
                    break;
                }
        }

            advance();

            if(builtinBinaryFunctions.contains(name) && args.size() == 2) {
                    return builtinBinaryFunctions[name](args[0], args[1]);
                }

            if(builtinFunctions.contains(name) && args.size() == 1) {
                return builtinFunctions[name](args[0]);
            }

            for(const auto& func : userFunctions) {
                if(func.name == name) {
                    UserFunction oneFunction = func;

                if(oneFunction.parameters.size() != args.size()) {
                    continue;
                }
                QMap<QString, double> newVariables;

                for(auto i = 0; i < oneFunction.parameters.size(); i++) {
                    newVariables[oneFunction.parameters[i]] = args[i];
                }

                auto funcTokens = tokenize(func.expressionText);
                Parser localParser(funcTokens, userFunctions, newVariables);

                return localParser.parseExpression();
                }
            }
            return 0.0;
        }
        else {
            if(variables.contains(name)) {
                return variables[name];
            }

            else {
                return 0.0;
            }
        }
    }

    return 0.0;
}

void CalculatorGui::resetUserFunctions() {
    QSettings settings("NothingWellOrg", "WeelTool");
    settings.clear();
    userFunctions.clear();

    functionsComboBox->clear();
    functionsComboBox->addItem("Select a function...");
}

double Parser::parseTerm() {
    double value = parseFactor();
    auto thisToken = currentToken();

    while(thisToken.type == TokenType::Star || thisToken.type == TokenType::Slash) {
        if(thisToken.type == TokenType::Star) {
            advance();
            double nextValue = parseFactor();

            value = value * nextValue;
        }

        else if(thisToken.type == TokenType::Slash) {
            advance();
            double nextValue = parseFactor();

            value = value / nextValue;
        }

        thisToken = currentToken();
    }

    return value;
}

double Parser::parseExpression() {
    double value = parseTerm();
    auto thisToken = currentToken();

    while(thisToken.type == TokenType::Plus || thisToken.type == TokenType::Minus) {
        if(thisToken.type == TokenType::Plus) {
            advance();
            double nextValue = parseTerm();

            value = value + nextValue;
        }

        else if(thisToken.type == TokenType::Minus) {
            advance();
            double nextValue = parseTerm();

            value = value - nextValue;
        }

        thisToken = currentToken();
    }

    return value;
}

void CalculatorGui::calculate() {
    QString text = inputField->text();
    
    QVector<Token> tokens = tokenize(text);
    Parser parser(tokens, userFunctions);
    
    double parseExpressionResult = parser.parseExpression();

    resultLabel->setText(QString::number(parseExpressionResult));
}

void CalculatorGui::saveUserFunctions() const {
    QSettings settings("NothingWellOrg", "WeelTool");
    settings.beginWriteArray("userFunctions", userFunctions.size());

    for(auto i = 0; i < userFunctions.size(); i++) {
        settings.setArrayIndex(i);

        settings.setValue("name", userFunctions[i].name);
        settings.setValue("parameters", userFunctions[i].parameters);
        settings.setValue("expression", userFunctions[i].expressionText);
    }
    settings.endArray();
}

void CalculatorGui::loadUserFunctions() {
    QSettings settings("NothingWellOrg", "WeelTool");

    userFunctions.clear();
    int size = settings.beginReadArray("userFunctions");

    for(auto i = 0; i < size; i++) {
        settings.setArrayIndex(i);
        UserFunction uFunction;

        uFunction.name = settings.value("name").toString();
        uFunction.parameters = settings.value("parameters").toStringList();
        uFunction.expressionText = settings.value("expression").toString();

        userFunctions.push_back(uFunction);
    }
    settings.endArray();
}

void CalculatorGui::openNewFunctionDialog() {
    QDialog dialog(this);
    dialog.setWindowTitle("New Function");

    QLineEdit nameOfFunction(&dialog);
    QLineEdit parametersOfFunction(&dialog);
    QLineEdit function(&dialog);

    QPushButton saveFunction("Save", &dialog);

    nameOfFunction.setPlaceholderText("Function name");
    parametersOfFunction.setPlaceholderText("Parameters");
    function.setPlaceholderText("Expression");

    QHBoxLayout* fieldsLayout = new QHBoxLayout();

    fieldsLayout->addWidget(&nameOfFunction);
    fieldsLayout->addWidget(&parametersOfFunction);
    fieldsLayout->addWidget(&function);

    QVBoxLayout dialogLayout(&dialog);
    dialogLayout.addWidget(&saveFunction);
    dialogLayout.addLayout(fieldsLayout);

    connect(&saveFunction, &QPushButton::clicked,
        this, [this, &dialog, &nameOfFunction,
               &parametersOfFunction, &function]() {
                const QString name = nameOfFunction.text().trimmed();
                QStringList parameters = parametersOfFunction.text().split(',', Qt::SkipEmptyParts);
                for (QString& parameter : parameters) {
                    parameter = parameter.trimmed();
                }

                const QString expression = function.text().trimmed();

                if(name.isEmpty() || parameters.isEmpty() || expression.isEmpty()) {
                    return;
                }

                else {

                    UserFunction uFunction;

                    uFunction.name = name;
                    uFunction.parameters = parameters;
                    uFunction.expressionText = expression;

                    userFunctions.push_back(uFunction);
                    saveUserFunctions();

                    int newIndex = userFunctions.size() - 1;
                    functionsComboBox->addItem(uFunction.name, newIndex);

                    dialog.accept();
                }
            });

    dialog.exec();
}

void CalculatorGui::onFunctionsComboBoxActivated(int index) {
    if (index == 0) {
        return;
    }

    int arrayIndex = functionsComboBox->itemData(index).toInt();

    if (arrayIndex >= 0 && arrayIndex < userFunctions.size()) {
        UserFunction selectedFunc = userFunctions[arrayIndex];

        QString templateText = selectedFunc.name + "(";
        templateText += selectedFunc.parameters.join(", ");
        templateText += ")";

        inputField->setText(templateText);
    }

    functionsComboBox->setCurrentIndex(0); 
}

CalculatorGui::CalculatorGui(QWidget* parent) : QWidget(parent) {
    inputField = new QLineEdit();
    resultLabel = new QLabel();
    
    QPushButton* result = new QPushButton("=");
    QPushButton* newFunctionButton = new QPushButton("New Function");
    QPushButton* resetUserFunctions = new QPushButton("Reset Functions");

    functionsComboBox = new QComboBox();
    functionsComboBox->addItem("Select a function...");
    
    QVBoxLayout* layout = new QVBoxLayout();
    QHBoxLayout* calculatorLayout = new QHBoxLayout();

    calculatorLayout->addWidget(inputField);
    calculatorLayout->addWidget(result);
    calculatorLayout->addWidget(resultLabel);
    layout->addWidget(newFunctionButton);
    
    layout->addWidget(functionsComboBox);
    layout->addWidget(resetUserFunctions);
    layout->addLayout(calculatorLayout);

    loadUserFunctions();

    for (int i = 0; i < userFunctions.size(); ++i) {
        functionsComboBox->addItem(userFunctions[i].name, i);
    }

    this->setLayout(layout);

    connect(functionsComboBox, QOverload<int>::of(&QComboBox::activated), 
            this, &CalculatorGui::onFunctionsComboBoxActivated);

    connect(result, &QPushButton::clicked,
        this, &CalculatorGui::calculate);

    connect(inputField, &QLineEdit::returnPressed,
        this, &CalculatorGui::calculate);

    connect(newFunctionButton, &QPushButton::clicked,
        this, &CalculatorGui::openNewFunctionDialog);

    connect(resetUserFunctions, &QPushButton::clicked,
        this, &CalculatorGui::resetUserFunctions);
}
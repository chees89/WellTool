#pragma once
#include <QString>
#include <QVector>
#include <QMap>
#include <QChar>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <cmath>
#include <QStringList>
#include <QDialog>
#include <QSettings>
#include <QComboBox>

using MathFunc = double(*)(double);
inline const QMap<QString, MathFunc> builtinFunctions = {
    {"log",   static_cast<MathFunc>(std::log)},
    {"log10", static_cast<MathFunc>(std::log10)},
    {"sqrt",  static_cast<MathFunc>(std::sqrt)},
    {"sin",   static_cast<MathFunc>(std::sin)},
    {"cos",   static_cast<MathFunc>(std::cos)},
    {"tan",   static_cast<MathFunc>(std::tan)},
};

using BinaryMathFunc = double(*)(double, double);
inline const QMap<QString, BinaryMathFunc> builtinBinaryFunctions = {
    {"pow", static_cast<BinaryMathFunc>(std::pow)},
};

enum class TokenType {
    Number, Plus, Minus, Star, Slash,
    LeftParen, RightParen, Comma, Identifier, End
};

struct Token {
    TokenType type;
    double numberValue;
    QString textValue;
};

struct UserFunction {
    QString name;
    QStringList parameters;
    QString expressionText;
};

inline const QMap<QChar, TokenType> expressionSign = {
    {'+', TokenType::Plus},
    {'-', TokenType::Minus},
    {'*', TokenType::Star},
    {'/', TokenType::Slash},
    {'(', TokenType::LeftParen},
    {')', TokenType::RightParen},
    {',', TokenType::Comma}
};

QVector<Token> tokenize(const QString& expression);

class Parser {
public:
    Parser(const QVector<Token>& tokens, const QVector<UserFunction>& functions, const QMap<QString, double>& variables = {});
    double parseExpression();

private:
    double parseTerm();
    double parseFactor();

    void advance();
    QVector<UserFunction> userFunctions;
    QMap<QString, double> variables;

    QVector<Token> tokens;
    Token currentToken();
    int pos;
};

class CalculatorGui : public QWidget {
Q_OBJECT

public:
    CalculatorGui(QWidget* parent = nullptr);

private slots:
    void onFunctionsComboBoxActivated(int index);

private:
    void calculate();
    void openNewFunctionDialog();
    void loadUserFunctions();
    void saveUserFunctions() const;
    void resetUserFunctions();

    QVector<UserFunction> userFunctions;
    QLineEdit* inputField;
    QLabel* resultLabel;

    QComboBox* functionsComboBox;
};
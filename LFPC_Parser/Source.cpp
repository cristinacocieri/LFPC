#pragma warning(disable:4996)
#include <iostream>
#include <math.h>
#include <string>

using namespace std;

// Math constants
#define M_PI             3.1415926535897932384626433832795
#define F_G              9.81
#define M_E              2.71828182845904523536


// Defines
#define MAX_EXPR_LEN   255
#define MAX_TOKEN_LEN  80

#define CALC_END -1
#define CALC_L_BRACKET -2
#define CALC_R_BRACKET -3
#define CALC_NUMBER -4


// Operations
#define OP_PLUS          0
#define OP_MINUS         1
#define OP_MULTIPLY      2
#define OP_DIVIDE        3
#define OP_PERCENT       4
#define OP_POWER         5
#define OP_UMINUS        6

#define OP_SIN           7
#define OP_COS           8
#define OP_TG            9
#define OP_CTG           10
#define OP_ARCSIN        11
#define OP_ARCCOS        12
#define OP_ARCTG         13
#define OP_ARCCTG        14
#define OP_SH            15
#define OP_CH            16
#define OP_TH            17
#define OP_CTH           18
#define OP_EXP           19
#define OP_LG            20
#define OP_LN            21
#define OP_SQRT          22
#define OP_IN            23
#define CALC_PI          24
#define CALC_G           25


/*****************************************************************************/

struct CALCULATOR_Node {
	double value;
	CALCULATOR_Node* left;
	CALCULATOR_Node* right;
	CALCULATOR_Node(
		double _value = 0.0,
		CALCULATOR_Node * _left = nullptr,
		CALCULATOR_Node * _right = nullptr
	) {
		value = _value;
		left = _left;
		right = _right;
	}
};


class CALCULATOR {
private:
	CALCULATOR_Node* root;
	char* expr;
	char curToken[MAX_TOKEN_LEN];
	int typToken;
	int pos;
	double result;
private:
	CALCULATOR_Node* CreateNode(
		double _value = 0.0,
		CALCULATOR_Node * _left = nullptr,
		CALCULATOR_Node * _right = nullptr
	);
	CALCULATOR_Node* Expr(void);
	CALCULATOR_Node* Expr1(void);
	CALCULATOR_Node* Expr2(void);
	CALCULATOR_Node* Expr3(void);
	CALCULATOR_Node* Expr4(void);
	bool GetToken(void);
	bool IsDelim(void);
	bool IsLetter(void);
	bool IsDigit(void);
	bool IsPoint(void);
	double CalcTree(CALCULATOR_Node* tree);
	void  DelTree(CALCULATOR_Node* tree);
	void SendError(int errNum);
public:

	CALCULATOR();
	~CALCULATOR();
	bool Compile(char* expr);
	double Evaluate();
	double GetResult(void);
};



bool CALCULATOR::IsDelim(void) {
	return (strchr("+-*/%^()[]", expr[pos]) != nullptr);
}


bool CALCULATOR::IsLetter(void) {
	return ((expr[pos] >= 'a' && expr[pos] <= 'z') ||
		(expr[pos] >= 'A' && expr[pos] <= 'Z'));
}



bool CALCULATOR::IsDigit() {
	return (expr[pos] >= '0' && expr[pos] <= '9');
}



bool CALCULATOR::IsPoint() {
	return (expr[pos] == '.');
}



CALCULATOR::CALCULATOR() {
	result = 0.0;
	root = nullptr;
}


CALCULATOR::~CALCULATOR() {
	DelTree(root);
	root = nullptr;
}


CALCULATOR_Node* CALCULATOR::CreateNode(
	double _value,
	CALCULATOR_Node* _left,
	CALCULATOR_Node* _right
) {
	CALCULATOR_Node* pNode = new CALCULATOR_Node(_value, _left, _right);
	return pNode;
}



void CALCULATOR::SendError(int errNum) {
	if (*curToken == '\0') {
		strcpy(curToken, "Empty expression!");
	}
	else if (errNum == 2) {
		throw "Expresion is not finished!";
	}
	else if (errNum == 3) {
		throw "Waiting for finishing the expression!";
	}
	else if (errNum == 4) {
		throw "Missing (";
	}
	else if (errNum == 5) {
		throw "Missing )";
	}
	else {
		return;
	}
}



bool CALCULATOR::GetToken(void) {
	*curToken = '\0';
	while (expr[pos] == ' ') {
		pos++;
	}
	if (expr[pos] == '\0') {
		curToken[0] = '\0';
		typToken = CALC_END;
		return true;
	}
	else if (IsDelim()) {
		curToken[0] = expr[pos++];
		curToken[1] = '\0';
		switch (*curToken) {
		case '+':
			typToken = OP_PLUS;
			return true;
		case '-':
			typToken = OP_MINUS;
			return true;
		case '*':
			typToken = OP_MULTIPLY;
			return true;
		case '/':
			typToken = OP_DIVIDE;
			return true;
		case '%':
			typToken = OP_PERCENT;
			return true;
		case '[':
		case '(':
			typToken = CALC_L_BRACKET;
			return true;
		case ']':
		case ')':
			typToken = CALC_R_BRACKET;
			return true;
		}
	}
	else if (IsLetter()) {
		int i = 0;
		while (IsLetter()) {
			curToken[i++] = expr[pos++];
		}
		curToken[i] = '\0';
		int len = static_cast<int>(strlen(curToken));
		for (i = 0; i < len; i++) {
			if (curToken[i] >= 'A' && curToken[i] <= 'Z') {
				curToken[i] += 'a' - 'A';
			}
		}

		if (!strcmp(curToken, "g")) {
			typToken = CALC_G;
			return true;
		}
		else if (!strcmp(curToken, "pi")) {
			typToken = CALC_PI;
			return true;
		}
		else if (!strcmp(curToken, "sin")) {
			typToken = OP_SIN;
			return true;
		}
		else if (!strcmp(curToken, "cos")) {
			typToken = OP_COS;
			return true;
		}
		else if (!strcmp(curToken, "tg")) {
			typToken = OP_TG;
			return true;
		}
		else if (!strcmp(curToken, "ctg")) {
			typToken = OP_CTG;
			return true;
		}
		else if (!strcmp(curToken, "arcsin")) {
			typToken = OP_ARCSIN;
			return true;
		}
		else if (!strcmp(curToken, "arccos")) {
			typToken = OP_ARCCOS;
			return true;
		}
		else if (!strcmp(curToken, "arcctg")) {
			typToken = OP_ARCCTG;
			return true;
		}
		else if (!strcmp(curToken, "sh")) {
			typToken = OP_SH;
			return true;
		}
		else if (!strcmp(curToken, "ch")) {
			typToken = OP_CH;
			return true;
		}
		else if (!strcmp(curToken, "th")) {
			typToken = OP_TH;
			return true;
		}
		else if (!strcmp(curToken, "cth")) {
			typToken = OP_CTH;
			return true;
		}
		else if (!strcmp(curToken, "exp")) {
			typToken = OP_EXP;
			return true;
		}
		else if (!strcmp(curToken, "lg")) {
			typToken = OP_LG;
			return true;
		}
		else if (!strcmp(curToken, "ln")) {
			typToken = OP_LN;
			return true;
		}
		else if (!strcmp(curToken, "sqrt")) {
			typToken = OP_SQRT;
			return true;
		}
		else {
			SendError(0);
		}
	}
	else if (IsDigit() || IsPoint()) {
		int i = 0;
		while (IsDigit()) {
			curToken[i++] = expr[pos++];
		}
		if (IsPoint()) {
			curToken[i++] = expr[pos++];
			while (IsDigit()) {
				curToken[i++] = expr[pos++];
			}
		}
		curToken[i] = '\0';
		typToken = CALC_NUMBER;
		return true;
	}
	else {
		curToken[0] = expr[pos++];
		curToken[1] = '\0';
		SendError(1);
	}
	return false;
}

bool CALCULATOR::Compile(char* _expr) {
	pos = 0;
	expr = _expr;
	*curToken = '\0';
	if (root != nullptr) {
		DelTree(root);
		root = nullptr;
	}
	GetToken();
	if (typToken == CALC_END) {
		SendError(2);
	}
	root = Expr();
	if (typToken != CALC_END) {
		SendError(3);
	}
	return true;
}



CALCULATOR_Node* CALCULATOR::Expr(void) {
	CALCULATOR_Node* temp = Expr1();
	while (true) {
		if (typToken == OP_PLUS) {
			GetToken();
			temp = CreateNode(OP_PLUS, temp, Expr1());
		}
		else if (typToken == OP_MINUS) {
			GetToken();
			temp = CreateNode(OP_MINUS, temp, Expr1());
		}
		else break;
	}
	return temp;
}


CALCULATOR_Node* CALCULATOR::Expr1(void)
{
	CALCULATOR_Node* temp = Expr2();
	while (true) {
		if (typToken == OP_MULTIPLY) {
			GetToken();
			temp = CreateNode(OP_MULTIPLY, temp, Expr2());
		}
		else if (typToken == OP_DIVIDE) {
			GetToken();
			temp = CreateNode(OP_DIVIDE, temp, Expr2());
		}
		else if (typToken == OP_PERCENT) {
			GetToken();
			temp = CreateNode(OP_PERCENT, temp, Expr2());
		}
		else {
			break;
		}
	}
	return temp;
}



CALCULATOR_Node* CALCULATOR::Expr2(void) {
	CALCULATOR_Node* temp;
	if (typToken == OP_PLUS) {
		GetToken();
		temp = Expr3();
	}
	else if (typToken == OP_MINUS) {
		GetToken();
		temp = CreateNode(OP_UMINUS, Expr3());
	}
	else {
		temp = Expr3();
	}
	return temp;
}



CALCULATOR_Node* CALCULATOR::Expr3(void) {
	CALCULATOR_Node* temp;
	if (typToken >= OP_SIN && typToken <= OP_SQRT + 1) {
		temp = CreateNode(OP_SIN - OP_SIN + typToken);
		GetToken();
		if (typToken != CALC_L_BRACKET) {
			SendError(4);
		}
		GetToken();
		temp->left = Expr();
		if (typToken != CALC_R_BRACKET) {
			SendError(5);
		}
		GetToken();
	}
	else {
		temp = Expr4();
	}
	return temp;
}


CALCULATOR_Node* CALCULATOR::Expr4(void) {
	CALCULATOR_Node* temp = nullptr;
	if (typToken == CALC_NUMBER) {
		temp = CreateNode(static_cast<double>(atof(curToken)));
		GetToken();
	}
	else if (typToken == CALC_PI) {
		temp = CreateNode(static_cast<double>(M_PI));
		GetToken();
	}
	else if (typToken == CALC_G) {
		temp = CreateNode(static_cast<double>(F_G));
		GetToken();
	}
	else if (typToken == CALC_L_BRACKET) {
		GetToken();
		temp = Expr();
		if (typToken != CALC_R_BRACKET) {
			SendError(5);
		}
		GetToken();
	}
	else {
		SendError(6);
	}
	return temp;
}


double CALCULATOR::Evaluate(void) {
	result = CalcTree(root);
	return result;
}



double CALCULATOR::GetResult() {
	return result;
}



double CALCULATOR::CalcTree(CALCULATOR_Node* tree) {
	static double temp;
	if (tree->left == nullptr && tree->right == nullptr) {
		return tree->value;
	}
	else {
		switch (static_cast<int>(tree->value)) {
		case OP_PLUS:
			return CalcTree(tree->left) + CalcTree(tree->right);
		case OP_MINUS:
			return CalcTree(tree->left) - CalcTree(tree->right);
		case OP_MULTIPLY:
			return CalcTree(tree->left) * CalcTree(tree->right);
		case OP_DIVIDE:
			return CalcTree(tree->left) / CalcTree(tree->right);
		case OP_PERCENT:
			return static_cast<int>(CalcTree(tree->left))
				% static_cast<int>(CalcTree(tree->right));
		case OP_POWER:
			return static_cast<double>(
				pow(CalcTree(tree->left), CalcTree(tree->right))
				);
		case OP_UMINUS:
			return -CalcTree(tree->left);
		case OP_SIN:
			return sin(CalcTree(tree->left));
		case OP_COS:
			return cos(CalcTree(tree->left));
		case OP_TG:
			return tan(CalcTree(tree->left));
		case OP_CTG:
			return 1.0 / tan(CalcTree(tree->left));
		case OP_ARCSIN:
			return asin(CalcTree(tree->left));
		case OP_ARCCOS:
			return acos(CalcTree(tree->left));
		case OP_ARCTG:
			return atan(CalcTree(tree->left));
		case OP_ARCCTG:
			return M_PI / 2.0 - atan(CalcTree(tree->left));
		case OP_SH:
			temp = CalcTree(tree->left);
			return (exp(temp) - exp(-temp)) / 2.0;
		case OP_CH:
			temp = CalcTree(tree->left);
			return (exp(temp) + exp(-temp)) / 2.0;
		case OP_TH:
			temp = CalcTree(tree->left);
			return (exp(temp) - exp(-temp)) / (exp(temp) + exp(-temp));
		case OP_CTH:
			temp = CalcTree(tree->left);
			return (exp(temp) + exp(-temp)) / (exp(temp) - exp(-temp));
		case OP_EXP:
			return exp(CalcTree(tree->left));
		case OP_LG:
			return log10(CalcTree(tree->left));
		case OP_LN:
			return log(CalcTree(tree->left));
		case OP_SQRT:
			return sqrt(CalcTree(tree->left));
		case OP_IN:
			return 1;
		}
	}
	return 0;
}


void CALCULATOR::DelTree(CALCULATOR_Node* tree) {
	if (tree == nullptr) {
		return;
	}
	DelTree(tree->left);
	DelTree(tree->right);
	delete tree;
	return;
}

int main() {
	CALCULATOR CALC;
	char expr[255];
	cout << "Enter the expression:" << endl;
	while (1) {
		cout << ">>  ";
		cin.getline(expr, 255);
		try {
			CALC.Compile(expr);
			CALC.Evaluate();
			cout << "Answer:    " << CALC.GetResult() << endl << endl;
			cout << "" << endl;
		}
		catch (const char* msg) {
			cerr << msg << endl;
			continue;
		}
	}
}
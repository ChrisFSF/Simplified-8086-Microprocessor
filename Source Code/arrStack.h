//============================================================================
// Name        : 126.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <iostream>
#include <stack>
#include <cstring>
#include <string>
#include <cmath>

using namespace std;

string convert_to_postfix(string a);

bool is_digit(char ch);//input is digit
bool is_digit(string ch);// input is string 
bool is_operator(char ch);
bool is_operator(string ch);//input is string

bool is_bracket(char ch);
//int getWeight(char op);
bool HasHigherPre(char op1, char op2);
void print_s(string s);

template <class T>
void printStack(stack<T> P);
double evaluatePostfix(string a);
double re_val(string ch);
double solve(double a, double b,char op);
string getstr(char x);
// int main(){

// 	string a="2+10*5";
// 	string b="(1+3)*4";
// 	string c="(4+5)*(2-3)";
// //	string d="A+((B+C)*(E-F)-G)/(H-I)";
// //	string e="A+B*(C+D)-E/F*G+H";

// 	cout<<"The result of "<<a<<" = "<<evaluatePostfix(a)<<endl;
// 	cout<<"The result of "<<b<<" = "<<evaluatePostfix(b)<<endl;
// 	cout<<"The result of "<<c<<" = "<<evaluatePostfix(c)<<endl;
// 	cout<<"you finish the exerise 1"<<endl;
// }

double evaluatePostfix(string a){

	int size=a.length();
//	cout<<"converting '";
//		print_s(a);
//			cout<<"' to postfix:"<<endl;
//			//cout<<"Size: "<<size<<endl;
	stack<char> sta;
	stack<string> pfx;
	string ifx="";
	for (int i=0; i<size;i++){
		ifx=a[i];
		if(is_digit(a[i])){//check a[i] is A-Z
			string acc=getstr(a[i]);
			int num=1;
			while(is_digit(a[i+num])){
				acc = acc+getstr(a[i+num]);
				num++;
			}
			pfx.push(acc);
			//cout<<"top: "<<pfx.top()<<endl;
			i=i+num;
		}//add the alpha to the pfx
		if(is_bracket(a[i])){//is bracket
			if(a[i]=='('){
				sta.push(a[i]);//push the ( to the stack
			}
			else if(a[i]==')'){//a[i] is )
				bool done=false;
				while (done==false){
					if(sta.top()=='('){
						sta.pop();
						done =true;
					}
					else{
						pfx.push(getstr(sta.top()));
						sta.pop();
						}
					}
				}
			}

		if(is_operator(a[i])){// symbol is operator
			//cout<<"ope "<<a[i]<<endl;
			while(!sta.empty()){
				if(HasHigherPre(sta.top(), a[i])){
					pfx.push(getstr(sta.top()));
					sta.pop();
				}
				else
					break;
			}
			sta.push(a[i]);//when stack is empty, push a[i]
			//cout<<"sta.top : "<<sta.top()<<endl;
		}
//		if(a[i]==' '){
//			break;
//		}

	}

	while(!sta.empty()){
		pfx.push(getstr(sta.top()));
		sta.pop();
		//cout<<"pfx top: "<<pfx.top()<<endl;
	}

	stack<string> copyStack;
	//cout<<" pfx.size(): "<<pfx.size()<<endl;
		int pfx_s=pfx.size();
	for (int i=0; i<pfx_s;i++){
		//cout<<pfx.top()<<endl;
		copyStack.push(pfx.top());
		pfx.pop();

	}

	stack<double> result;
	string cha;

	while(!copyStack.empty()){
		cha=copyStack.top();
		if (is_digit(cha)){
			result.push(re_val(cha));
		}
		if(is_operator(cha)){
			double op2=result.top();
			result.pop();
			double op1=result.top();
			result.pop();
			result.push(solve(op1, op2,cha[0]));
		}
		copyStack.pop();//pop the top
	}
	double res=0;
	res=result.top();
	result.pop();//clean the copystack
	return res;

}


double solve(double a, double b,char op){
	double sol;
	switch(op){
	case'+':
		sol=a+b;
		break;
	case'-':
			sol=a-b;
			break;
	case'*':
			sol=a*b;
			break;
	case'/':
			sol=a/b;
			break;

	default:
		cout<<"Error from operator input!"<<endl;
		break;
	}
return sol;
}

double re_val(string ch){
	double sum=0;
	int size=ch.length();
	int i=0;
	while(ch[i]!='\0'){
		double val=0;
		switch(ch[i]){
			case '0':
				val= 0;
				break;
			case '1':
				val= 1;
				break;
			case '2':
				val= 2;
				break;
			case '3':
				val= 3;
				break;
			case '4':
				val= 4;
					break;
			case '5':
				val= 5;
						break;
			case '6':
				val= 6;
						break;
			case '7':
				val= 7;
						break;
			case '8':
				val= 8;
						break;
			case '9':
				val= 9;
						break;
		default:
			cout<<"Wrong input"<<endl;
			break;
			}
		sum+=val*pow(10,size-1);
		size--;
		i++;
	}
return sum;
}


string getstr(char x){
	string s(1,x);
	return s;
}


bool is_digit(char ch){
	return (ch=='0'||ch=='1'||ch=='2'||ch=='3'||ch=='4'||ch=='5'||ch=='6'||ch=='7'||ch=='8'|| ch=='9');
}

bool is_digit(string ch){
	return (ch[0]=='0' ||ch[0]=='1' ||ch[0]=='2' ||ch[0]=='3'||ch[0]=='4'||ch[0]=='5'||ch[0]=='6'||ch[0]=='7'||ch[0]=='8'||ch[0]=='9');
}

//string type
bool is_operator(string ch){
	return (ch[0]=='+' ||ch[0]=='-' ||ch[0]=='*' ||ch[0]=='/');
}

//char input
bool is_operator(char ch){
	return (ch=='+'||ch=='-'||ch=='*'||ch=='/');
}

bool is_bracket(char ch){
	return (ch=='('||ch==')');
}

bool HasHigherPre(char op1, char op2){
	int prec1, prec2;
		if (op1 == '*' || op1 == '/')
		{
			prec1 = 2;
		}
		else if (op1 == '+' || op1 == '-')
		{
			prec1 = 1;
		}
		if (op1 == '(')
		{
			prec1 = 0;
		}
		if (op2 == '*' || op2 == '/')
		{
			prec2 = 2;
		}
		if (op2 == '+' || op2 == '-')
		{
			prec2 = 1;
		}
		//cout<<"compare: "<<op1<<" and "<<op2<<endl;
		return (prec1 >= prec2);

}

void print_s(string s){

	for (int i=0; i<s.length();i++){
		cout<<s[i];
	}

}




//PI interpreter

/*
	Author 		:	Ashwani Kumar
	Interpreter :	Brainf**k
	Modified accordingly
*/
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <unistd.h>

/*	
	0       put space
	1	>	increment data pointer
	2	<	decrement data pointer
	3	+	increment byte at data pointer
	4	-	decrement byte at data pointer
	5	.	output the byte at the data pointer
	6	,	accept one byte of input at data pointer
	7	[	if the byte at data pointer is 0, then move instruction pointer to the next pointer forward to the command after matching ]
	8	]	if the byte at data pointer is !=0, then move it back to command after matching [
	9		Take INT or ASCII value (default is ASCII)
	n	push	push n-10 into Queue
*/


class PI_interpret {
public:
	char data[3000] = { 0 };
	char *pointer;
	int *prog;
	const int *p;
	int len;
	std::queue<int> vm;
	bool print_ascii;

	PI_interpret(std::queue<int> input) {
		vm = input;
		len = input.size();
		prog = new int[len];
		int i;
		for (i = 0; i < len; i++) {
			prog[i] = input.front();
			input.pop();
		}
		p = prog;
		pointer = data;
		print_ascii = true;
	}

	void increment_p() {
		pointer++;
	}

	void decrement_p() {
		pointer--;
	}

	void add() {
		(*pointer)++;
	}

	void subtract() {
		(*pointer)--;
	}

	void output() {
		//std::cout << *pointer;
		if(print_ascii){
			std::cout << *pointer;
		}
		else{
			std::cout<<(int)*pointer;
		}
	}

	void input() {
		//std::cin >> *pointer;
		if(print_ascii){
			std::cin >> *pointer;
		}else{
			char tmp;
			std::cin >> tmp; 
			*pointer = (tmp - 48);
		}
	}

	void lopen() {
		int bal = 1;
		if (*pointer == '\0') {
			do {
				p++;
				if (*p == 7) bal++;
				else if (*p == 8) bal--;
			} while (bal != 0);
		}
	}

	void lclose() {
		int bal = 0;
		do {
			if (*p == 7) bal++;
			else if (*p == 8) bal--;
			p--;
		} while (bal != 0);
	}

	void push_n(int k) {
		(*pointer) += k;
	}
	
	void flip(){
		print_ascii = !print_ascii;
	}

	void evaluate() {
		int k;
		while (*p != -1) {
			if (!vm.empty()) {
				k = vm.front();
				vm.pop();
			}
			switch (*p) {
			case 0: std::cout << std::endl;
				break;
			case 1: increment_p();
				break;
			case 2:	decrement_p();
				break;
			case 3:	add();
				break;
			case 4:	subtract();
				break;
			case 5:	output();
				break;
			case 6:	input();
				break;
			case 7:	lopen();
				break;
			case 8:	lclose();
				break;
			case 9: flip();
				break;
			default: push_n(k-10);
				break;
			}
			p++;
		}
	}
	
	void debug(){
		std::cout<<"Debug: "<<std::endl;
		for(int i=0;i<len;i++){
			int tmp = prog[i];
			switch(tmp){
				case 1: std::cout<<'>';
					break;
				case 2: std::cout<<'<';
					break;
				case 3: std::cout<<'+';
					break;
				case 4: std::cout<<'-';
					break;
				case 5: std::cout<<'.';
					break;
				case 6: std::cout<<',';
					break;
				case 7: std::cout<<'[';
					break;
				case 8: std::cout<<']';
					break;
				case 9: std::cout<<'/';
					break;
			}
		}
		std::cout<<"\n";
		for(int i=0;data[i];i++){
			std::cout<<data[i]<<" ";
		}
	}
	

};

void init_queue(std::ifstream &file, std::queue<int> &vm) {
	std::string word = "pi", s;
	std::getline(file, s);
	std::stringstream ss(s);

	int count = 0;
	char ch;

	while (ss >> s) {
		if (s == word) {
			count++;
		}
	}
	vm.push(count);
}

void usage(){
	std::cout<<"Run a file contating PI code"<<std::endl<<std::endl;
	std::cout<<"Usage: PI -h | filename"<<std::endl <<std::endl;
	std::cout<<"     <filename>    Interpret the source file"<<std::endl<<std::endl;
}

int main(int argc, char **argv){
	std::queue<int> vm;

	std::ifstream file;

	if(argc < 2 || argc >= 3){
		usage();
		exit(0);
	}

	char* filename;
	filename = (char*) argv[optind];
	
	file.open(filename);

	while (file.eof() == 0) {
		init_queue(file, vm);
	}
	
	vm.push(-1);
	
	file.close();

	PI_interpret intepreter = PI_interpret(vm);
	//intepreter.debug();
	intepreter.evaluate();

    return 0;
}
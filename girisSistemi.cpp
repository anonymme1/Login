#include <iostream>
#include <fstream>
#include <termios.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

int yildiz() {
	struct termios eski, yeni;
	int ch;
	tcgetattr(STDIN_FILENO, &eski);
	yeni=eski;
	yeni.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &yeni);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &eski);
	return ch;
}

std::string enter() {
	char sifre[30];
	std::cout<<"\n\e[92mEnter password: ";
	int i=0;
	while(1) {
		sifre[i] = yildiz();
		if(sifre[i] == '\b') {
			fprintf(stdout, "\r%d", (int)strlen(sifre)-1);
			continue;
		}
		if(sifre[i] == '\n') {
			sifre[i] = '\0';
			break;
		}

		fprintf(stdout, "*");
		++i;
	}
	std::string pass_(sifre);
	return pass_;
}

std::string encode(std::string a) {
	std::string b;
	b=a;
	for(int i=0;i<a.size();i++) {
		b[i] = b[i] + 2;
	}

	return b;
}

bool access() {
	std::ifstream reads("password.txt");
	if(!reads.is_open()) {
		std::ofstream writes;
		writes.open("password.txt");
		std::string passw;
		system("clear");
		std::cout<<"\n\e[92mEnter new password: ";
		passw = enter();
		writes<<encode(passw)<<std::endl;
		std::cout<<"\nSaved new password. Please restart program\n\n";
		writes.close();
		exit(0);
	} else {
		int kez=3;
		while(kez--) {
			std::string passwd;
			passwd = enter();
			while(!reads.eof()) {
				std::ifstream reads("password.txt");
				std::string line;
				reads>>line;
				if(line == encode(passwd)) {
					std::cout<<"\n\e[93mAccess granted.\n";
					return true;
				}
				else {
					std::cout<<"\n\e[91mAccess denied.\n";
					break;
				}
			}
		}
	}
	return false;
}

void banner() {
	std::cout<<"\n\n\t\e[94m====================================\n";
	std::cout<<"\t\e[94m==   \e[91mauthor: ziyaArslan1            \e[94m==\n";
	std::cout<<"\t\e[94m==   \e[91mgithub: github.com/ziyaArslan1 \e[94m==\n";
	std::cout<<"\t\e[94m====================================\n\n";
}

int main() {
	if(access()) {
		banner();
		std::cout<<"\n\e[92mLogin is success.\n";
	}
	else {
		std::cout<<"\n\e[91mLogin incorret.\n";
	}
}

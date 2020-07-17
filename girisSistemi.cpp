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
	std::cout<<"\nEnter password: ";
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
	std::string encrypt;

	std::ifstream oku("password.txt");
	if(!oku.is_open()) {
		std::ofstream yaz;
		yaz.open("password.txt");
		std::string passw;
		system("clear");
		std::cout<<"\nEnter new password: ";
		passw = enter();
		encrypt = encode(passw);
		yaz<<encrypt<<"\n";
		std::cout<<"\nSaved new password. Please restart program\n\n";
		yaz.close();
		exit(1);
	}

	if(oku.is_open()) {
		int kez=3;
		while(kez--) {
			std::string passwd;
			passwd = enter();
			while(!oku.eof()) {
				std::ifstream oku("password.txt");
				std::string satir;
				oku>>satir;
				if(satir == encode(passwd)) {
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
	std::cout<<"\t\e[94m==   \e[91mauthor: anonymme1            \e[94m==\n";
	std::cout<<"\t\e[94m==   \e[91mgithub: github.com/anonymme1 \e[94m==\n";
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

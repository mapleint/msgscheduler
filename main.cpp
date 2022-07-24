#include <Windows.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <chrono>

bool discordbooted = false;

struct msg{
	std::string txt;
	std::string rec;
	unsigned int date;
	msg(std::string name, std::string body, std::string t) {
		txt = body;
		rec = name;
		date = atoi(t.c_str());

	}

	void enter() {
		INPUT inputs[2] = {};
		ZeroMemory(inputs, sizeof(inputs));

		inputs[0].type = INPUT_KEYBOARD;
		inputs[0].ki.wVk = VK_RETURN;

		inputs[1].type = INPUT_KEYBOARD;
		inputs[1].ki.wVk = VK_RETURN;
		inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

		SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
	}

	void type(const char* str) {
		for (int i = 0; str[i] != 0; i++) {


			INPUT inputs[4] = {};
			ZeroMemory(inputs, sizeof(inputs));
			inputs[1].type = INPUT_KEYBOARD;
			inputs[1].ki.wVk = toupper(str[i]);

			inputs[2].type = INPUT_KEYBOARD;
			inputs[2].ki.wVk = toupper(str[i]);
			inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;
			bool bshift = false;
			if(!isalnum(str[i]) && str[i] != ' ') {
				WORD key = str[i];
				switch (str[i]) {
				case '<':
					bshift = true;
				case ',':
					key = VK_OEM_COMMA;
					break;
				case '>':
					bshift = true;
				case '.':
					key = VK_OEM_PERIOD;
					break;
				case '?':
					bshift = true;
				case '/':
					key = VK_OEM_2;
					break;
				case ':':
					bshift = true;
				case ';':
					key = VK_OEM_1;
					break;
				case '\"':
					bshift = true;
				case '\'':
					key = VK_OEM_7;
					break;
				case '{':
					bshift = true;
				case '[':
					key = VK_OEM_4;
					break;
				case '}':
					bshift = true;
				case ']':
					key = VK_OEM_6;
					break;
				case '|':
					bshift = true;
				case '\\':
					key = VK_OEM_5;
					break;
				case '~':
					bshift = true;
				case '`':
					key = VK_OEM_3;
					break;
				case '_':
					bshift = true;
				case '-':
					key = VK_OEM_MINUS;
					break;
				case '+':
					bshift = true;
				case '=':
					key = VK_OEM_PLUS;
					break;
				case '!':
					bshift = true;
					key = '1';
					break;
				case '@':
					bshift = true;
					key = '2';
					break;
				case '#':
					bshift = true;
					key = '3';
					break;
				case '$':
					bshift = true;
					key = '4';
					break;
				case '%':
					bshift = true;
					key = '5';
					break;
				case '^':
					bshift = true;
					key = '6';
					break;
				case '&':
					bshift = true;
					key = '7';
					break;
				case '*':
					bshift = true;
					key = '8';
					break;
				case '(':
					bshift = true;
					key = '9';
					break;
				case ')':
					bshift = true;
					key = '0';
					break;
				}
				inputs[1].ki.wVk = key;
				inputs[2].ki.wVk = key;
			}

			if (isupper(str[i]) || bshift) {
				inputs[0].type = INPUT_KEYBOARD;
				inputs[0].ki.wVk = VK_LSHIFT;
				inputs[3].type = INPUT_KEYBOARD;
				inputs[3].ki.wVk = VK_LSHIFT;
				inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;
			}

			SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
		}
	}

	void dispatch() {
		if (!discordbooted) {
			char username[256];
			DWORD len = 256;
			GetUserNameA(username, &len);
			std::string discordpath = "C:\\Users\\";
			discordpath += username;
			discordpath += "\\AppData\\Local\\Discord\\app-1.0.9005\\Discord.exe";
			system(discordpath.c_str());
			Sleep(2000);
		}
		discordbooted = true;

		INPUT inputs[4] = {};
		ZeroMemory(inputs, sizeof(inputs));

		inputs[0].type = INPUT_KEYBOARD;
		inputs[0].ki.wVk = VK_LCONTROL;

		inputs[1].type = INPUT_KEYBOARD;
		inputs[1].ki.wVk = 'K';

		inputs[2].type = INPUT_KEYBOARD;
		inputs[2].ki.wVk = 'K';
		inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

		inputs[3].type = INPUT_KEYBOARD;
		inputs[3].ki.wVk = VK_LCONTROL;
		inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

		SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));

		Sleep(500);

		type(rec.c_str());
		Sleep(500);
		enter();
		Sleep(500);
		type(txt.c_str());
		Sleep(500);
		enter();
		return;
	}

};

/*planned format: 
user - no new lines 
text - no new lines 
date - Epoch time is acceptable
blank line is new message
ALL NULL POINTERS ARE TO BE TRASHED
*/

std::vector<msg*> messages;


int main(int argc, char* argv[]) 
{
	
	if (!argc)
		return 1;
	
	/*get path to messages.txt*/
	std::string path = argv[0];
	path = path.substr(0, path.find_last_of('\\') + 1) + "messages.txt";
	
	/*load messages to vector*/
	printf("loading messages from %s\n", path.c_str());
	std::ifstream file(path, std::ifstream::in);
	if (!file.is_open())
		return 404;
	std::string line[3];
	while (std::getline(file, line[0]) && std::getline(file, line[1]) && std::getline(file, line[2])) {
		messages.push_back(new msg(line[0], line[1], line[2]));
	}
	/*sort vector*/
	std::sort(messages.begin(), messages.end(), [](msg* a, msg* b) -> bool
		{
			return a->date < b->date;
		});

	printf("sorted %i messages\n", messages.size());

	/*dispatch messages*/
	for (msg* message : messages) {
		long dt = message->date - time(0);
		/*can be fixed by me being smarter
		but 1.6 months is plenty time*/
		printf("sending message \"%s\" to \"%s\" %i seconds from now\n",
			message->txt.c_str(), message->rec.c_str(), dt);
		if(dt > 0)
			Sleep(dt * 1000);
		message->dispatch();
	}
}
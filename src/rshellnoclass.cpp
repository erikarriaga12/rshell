#include <cstdio>
#include <cstdlib>
#include <memory>
#include <stdio.h>
#include <string.h>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <errno.h>
#include <typeinfo>
#include <sys/types.h>
#include <sys/wait.h>
#include <cctype>
#include <algorithm>
#include <fstream>

using namespace std;

int main()
{

while(1)
{

	vector<char *> cmdTokens;
	vector<char *> separatedCmdTokens;
	vector<char *> andCmdTokens;
	vector<char *> andSeparatedCmdTokens;

	string userInput;
	std::cout << getlogin() << "$ ";
	getline(cin, userInput);

	if (userInput == "exit") 
	{
		return 0;
	}

	size_t commentSymPos = userInput.find("#");
	string userInputNoComments = userInput.substr(0,commentSymPos);

	char * userInputChar = new char[userInputNoComments.length() + 1];
	strcpy(userInputChar, userInputNoComments.c_str());

	char * cmdTok = strtok(userInputChar, ";");

	while (cmdTok != 0)
	{
		cmdTokens.push_back(cmdTok);
		cmdTok = strtok(NULL,";");
	}

	for (unsigned j = 0; j < cmdTokens.size(); j++)
	{	
		if (string(cmdTokens[j]).find("&&") == std::string::npos)
		{
			separatedCmdTokens.clear();
			char* cmdTok4 = strtok(cmdTokens[j], " ");
			while (cmdTok4 != 0)
			{
				separatedCmdTokens.push_back(cmdTok4);
				cmdTok4 = strtok(NULL, " ");
			}

			char *cmdToExec[20];
			for (unsigned i = 0; i < separatedCmdTokens.size(); i++)
			{
				cmdToExec[i] = separatedCmdTokens[i];
			}
			cmdToExec[separatedCmdTokens.size()] = NULL;
			char *cmd = separatedCmdTokens[0];

			int pid = fork();
			if(pid == -1) 
			{
    			perror("fork");
    			exit(1);
			}
			else if (pid == 0)
			{	
				execvp(cmd, cmdToExec);
			}		
			else
			{
				wait(0);
			}
		}

		else if (string(cmdTokens[j]).find("&&") != std::string::npos
			|| string(cmdTokens[j]).find("||") != std::string::npos)
		{
			andCmdTokens.clear();
			char* cmdTok4 = strtok(cmdTokens[j], "&&");
			while (cmdTok4!= 0)
			{
				andCmdTokens.push_back(cmdTok4);
				cmdTok4 = strtok(NULL, "&&");
			}

			for (unsigned k = 0; k < andCmdTokens.size(); k++)
			{
				andSeparatedCmdTokens.clear();
				char* cmdTok3 = strtok(andCmdTokens[k], " ");
				while (cmdTok3 != 0)
				{
					andSeparatedCmdTokens.push_back(cmdTok3);
					cmdTok3 = strtok(NULL, " ");
				}
				
				char *cmdToExec[20];
				for (unsigned i = 0; i < andSeparatedCmdTokens.size(); i++)
				{
					cmdToExec[i] = andSeparatedCmdTokens[i];
				}
				cmdToExec[andSeparatedCmdTokens.size()] = NULL;
				char *cmd = andSeparatedCmdTokens[0];

				int status;
				int pid = fork();
				if(pid == -1) 
				{
					perror("fork");
					exit(1);
				}
				if (pid == 0)
				{	
					if ((status = execvp(cmd, cmdToExec)) == -1)
					{
						goto quitthis;
					} 
					
				}		
				else
				{
					wait(0);
				}
			}
			
		}
	}		
	quitthis: if (errno != 0)
			{
				cout << "There was an error.\n";
			}
	delete[] userInputChar;
}
	return 0;
}

/*	main.cpp

*******************************************************************/
#include <fstream>
#include <iostream>

#include "bank.h"
void* atm_routine(void* arguments);
void* check(void *arg);



using namespace std;

Bank bank;

int main(int argc, char *argv[]){	
	/*
	ofstream clearLogFile("log.txt", std::ios::out);
	clearLogFile << "" << std::endl;
	clearLogFile.close(); // Close the file after truncating it
	
	int thread_id = 0;
	pthread_t thread_atm;
	pair<string,int> thread_args = make_pair(argv[1], thread_id);
	pthread_create(&thread_atm, NULL, atm_routine, (void*) &thread_args);
	pthread_join(thread_atm, NULL);

	int thread_id = 0;
	pthread_t thread_atm;
	pthread_create(&thread_atm, NULL, check,(void*) &thread_id);
	pthread_join(thread_atm,NULL);
	logFile.close();
	return 0;
	*/

	pthread_t thread_id;

    // Create a thread to run the check function
    int result = pthread_create(&thread_id, nullptr, check, nullptr);
    /*if (result != 0) {
        std::cerr << "Error creating thread: " << result << std::endl;
        return 1;
    }

    // Wait for the thread to finish
    result = pthread_join(thread_id, nullptr);
    if (result != 0) {
        std::cerr << "Error joining thread: " << result << std::endl;
        return 1;
    }
	*/

    return 0;
}



void* atm_routine(void* arguments){
	// First - cast the input to a pair of string - int
	pair<string,int>* path_and_id = reinterpret_cast<pair<string,int>*>(arguments);
	string file_path = path_and_id->first;
	int atm_id = path_and_id->second;
	// create a vector for commands in specific file
	vector<string> commands = convert_file_to_vec(file_path);
	auto it = commands.begin();
	// iterate through the lines and execute according to switch-case
	while(it!=commands.end()){
		vector<int> cmd_args;
		char cmd = parse_line(*it, cmd_args);
		exe_command(cmd, cmd_args, atm_id); 
		it++;
	}
	pthread_exit(NULL);
}

void* check(void *arg){
	cout << "check" << endl;
	pthread_exit(NULL);
}





	/*
	vector<int> args;
	string input = "O 12345 1234 300";
	char cmd = parse_line(input, args);
	cout << cmd << endl;
	for(int i=0; i<args.size();i++)
		cout << args[i] << endl;
	
	*/








/*
	vector<string> file_lines = convert_file_to_vec(argv[1]);
	auto it = file_lines.begin();
	while (it!=file_lines.end()){
		vector<int> args;
		char cmd = parse_line(*it, args);
		cout << cmd << endl;
		for(int i=0; i<args.size();i++)
			cout << args[i] << endl;
		it++;
	}
	*/













































/*
	bank.open_account(10, 1234, 1000, 5);
	bank.open_account(10, 4567, 400, 17);
	bank.open_account(20, 3333, 5000, 5);
	bank.close_account(11, 1234, 11);//not exist
	bank.close_account(10, 2222, 11);//wrong pass
	map<int, Account>::iterator it = bank.accounts.begin();
	while (it != bank.accounts.end()) {
		cout << it->first << "   "<< it->second.password <<"   " << it->second.balance << endl;
		it++;
	}
	cout << endl;
	bank.close_account(10, 1234, 5);
	bank.deposit_account(20,3333,23,7);
	bank.open_account(21,1133,5757,12);
	bank.withdraw_account(21,1133,9000,10);
	bank.withdraw_account(21,1133,2000,19);
	bank.check_balance_account(21,1000,2);
	bank.check_balance_account(23,1000,2);
	bank.check_balance_account(20,3333,2);
	bank.transfer_funds_account(21,1133,20,6000,2);
	it = bank.accounts.begin();
	while (it != bank.accounts.end()) {
		cout << it->first << "   "<< it->second.password <<"   " << it->second.balance << endl;
		it++;
	}

	cout << endl;
	bank.take_fees_account();
	bank.print_accounts();
	*/
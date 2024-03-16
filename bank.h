#ifndef _BANK_H
#define _BANK_H
#include <unistd.h> 
#include <stdio.h>
#include <cstdio>
#include <cerrno>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <cstring>
#include <map>
#include <algorithm>    
#include <iostream>
#include <pthread.h>
#include <fstream>

using namespace std;

extern fstream logFile;
extern pthread_mutex_t logFile_mutex;

class Account {
public:
	int password;
	int balance;
	int acc_num_readers;
	pthread_mutex_t acc_read_mutex;
	pthread_mutex_t acc_write_mutex;
	
	//constructors and destructor
	Account();
	Account(int password, int balance, int num_readers = 0);
	~Account();

	//methods

};

class Bank {
public:
	Account manager_acc;
	map<int, Account> accounts;
	int bank_num_readers;
	pthread_mutex_t bank_read_mutex;	
	pthread_mutex_t bank_write_mutex;

	//constructors and destructor
	Bank();
	~Bank();

	// writers readers methods for Bank
	void bank_read_lock();
	void bank_read_unlock();
	void bank_write_lock();
	void bank_write_unlock();
	
	//methods
	void open_account(int acc_num, int password, int initial_balance, int atm_id);
	void close_account(int acc_num, int password, int atm_id);
	void deposit_account(int acc_num, int password, int amount, int atm_id);
	void withdraw_account(int acc_num, int password, int amount, int atm_id);
	void check_balance_account(int acc_num, int password, int atm_id);
	void transfer_funds_account(int src_acc_num, int src_acc_password, int trg_acc_num, int amount, int atm_id);

	

};


//help functions


#endif


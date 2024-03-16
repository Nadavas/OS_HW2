// bank.cpp

/*******************************************/
#include <fstream>
#include "bank.h"

using namespace std;

//creating the log file with read and append permissions
fstream logFile("log.txt", ios::in | ios::app);
//creating the mutex for the log file
pthread_mutex_t logFile_mutex;

/************************************************/
/*******************Bank*************************/
/************************************************/

Bank::Bank() {
    Account manager_acc;
    if (pthread_mutex_init(&this->bank_read_mutex, NULL) != 0) {
        perror("Bank error : pthread_mutex_init failed");
        exit(0);
    }
    if (pthread_mutex_init(&this->bank_write_mutex, NULL) != 0) {
        perror("Bank error : pthread_mutex_init failed");
        exit(0);
    }
}

Bank::~Bank() {
    if (pthread_mutex_destroy(&this->bank_read_mutex) != 0) {
        perror("Bank error : pthread_mutex_destroy failed");
        exit(0);
    }
    if (pthread_mutex_destroy(&this->bank_write_mutex) != 0) {
        perror("Bank error : pthread_mutex_destroy failed");
        exit(0);
    }
}

//// ALL WRITERS READERS BANK'S METHODS ////
void Bank::bank_read_lock(){
    if(pthread_mutex_lock(&bank_read_mutex)){
        perror("Bank error : pthread_mutex_lock failed");
        logFile.close();
        exit(0);
    }
    bank_num_readers++;
    if(bank_num_readers == 1)
        bank_write_lock();
    if(pthread_mutex_unlock(&bank_read_mutex)){
        perror("Bank error : pthread_mutex_unlock failed");
        logFile.close();
        exit(0);
    }
}

void Bank::bank_read_unlock(){
    if(pthread_mutex_lock(&bank_read_mutex)){
        perror("Bank error : pthread_mutex_lock failed");
        logFile.close();
        exit(0);
    }
    bank_num_readers--;
    if(bank_num_readers == 0)
        bank_write_unlock();
    if(pthread_mutex_unlock(&bank_read_mutex)){
        perror("Bank error : pthread_mutex_unlock failed");
        logFile.close();
        exit(0);
    }
}

void Bank::open_account(int acc_num, int password, int initial_balance, int atm_id) {
    //lock bank - WRITER
    //sleep(1)
    if (accounts.find(acc_num) != accounts.end()) {
        //lock log
        logFile << "Error " << atm_id << ": Your transaction failed - account with the same id exists" << endl;
        //unlock log
        return;
    }
    Account new_acc = Account(password, initial_balance);
    accounts[acc_num] = new_acc;

    //lock log
    logFile << atm_id << ": New account id is " << acc_num << " with password "
        << password <<" and initial balance "<< initial_balance << endl;
    //unlock log
    
    //unlock bank - WRITER
}


void Bank::close_account(int acc_num, int password, int atm_id) {
    //lock bank - WRITER
    //sleep(1)
    map<int, Account>::iterator it = accounts.find(acc_num);
    if (it == accounts.end()) {
        //lock log
        logFile << "Error " << atm_id << ": Your transaction failed - account id " << acc_num << " does not exist" << endl;
        //unlock log
        return;
    }
    if (it->second.password != password) {
        //lock log
        logFile << "Error " << atm_id << ": Your transaction failed - password for account id " << acc_num << " is incorrect" << endl;
        //unlock log
        return;
    }
    int balance = accounts[acc_num].balance;
    accounts.erase(acc_num);

    //lock log
    logFile << atm_id << ": Account " << acc_num << " is now closed. Balance was " << balance << endl;
    // unlock log

    //unlock bank - WRITER
}

void Bank::deposit_account(int acc_num, int password, int amount, int atm_id){
    // sleep(1)
    // lock bank - READER
    map<int, Account>::iterator it = accounts.find(acc_num);
    if (it == accounts.end()) {
        //lock log
        logFile << "Error " << atm_id << ": Your transaction failed - account id " << acc_num << " does not exist" << endl;
        //unlock log
        return;
    }
    if (it->second.password != password) {
        //lock log
        logFile << "Error " << atm_id << ": Your transaction failed - password for account id " << acc_num << " is incorrect" << endl;
        //unlock log
        return;
    }
    //lock specific account balance mutex - WRITER
    it->second.balance += amount;
    //lock log
    logFile << atm_id << ": Account " << acc_num << " new balance is " << it->second.balance 
    << " after " << amount << " $ was deposited" <<endl;
    // unlock log
    //unlock specific account balance mutex - WRITER
    // unlock bank - READER
}

void Bank::withdraw_account(int acc_num, int password, int amount, int atm_id){
    // lock bank - READER
    // sleep(1)
    map<int, Account>::iterator it = accounts.find(acc_num);
    if (it == accounts.end()) {
        //lock log
        logFile << "Error " << atm_id << ": Your transaction failed - account id " << acc_num << " does not exist" << endl;
        //unlock log
        return;
    }
    if (it->second.password != password) {
        //lock log
        logFile << "Error " << atm_id << ": Your transaction failed - password for account id " << acc_num << " is incorrect" << endl;
        //unlock log
        return;
    }
    
    //lock specific account balance mutex - WRITER
    if(amount > it->second.balance){
        // lock log
        logFile << "Error " << atm_id << ": Your transaction failed - account id " << acc_num 
        << " balance is lower than " << amount << endl;
        // unlock log
    //unlock specific account balance mutex - WRITER
        return;
    }
    it->second.balance -= amount;
    //lock log
    logFile << atm_id << ": Account " << acc_num << " new balance is " << it->second.balance 
    << " after " << amount << " $ was withdrew" <<endl;
    // unlock log
    //unlock specific account balance mutex - WRITER
    // unlock bank - READER
}

void Bank::check_balance_account(int acc_num, int password, int atm_id){
    // lock bank - READER
    // sleep(1)
    map<int, Account>::iterator it = accounts.find(acc_num);
    if (it == accounts.end()) {
        //lock log
        logFile << "Error " << atm_id << ": Your transaction failed - account id " << acc_num << " does not exist" << endl;
        //unlock log
        return;
    }
    if (it->second.password != password) {
        //lock log
        logFile << "Error " << atm_id << ": Your transaction failed - password for account id " << acc_num << " is incorrect" << endl;
        //unlock log
        return;
    }
    //lock specific account balance mutex - READER
    // lock log
    logFile << atm_id << ": Account " << acc_num << " balance is " << it->second.balance << endl; 
    // unlock log
    //lock specific account balance mutex - READER
    // unlock bank - READER
}

void Bank::transfer_funds_account(int src_acc_num, int src_acc_password, int trg_acc_num, int amount, int atm_id){
    // unlock bank - READER
    // sleep(1)
    map<int, Account>::iterator it_src = accounts.find(src_acc_num);
    if (it_src == accounts.end()) {
        //lock log
        logFile << "Error " << atm_id << ": Your transaction failed - account id " << src_acc_num << " does not exist" << endl;
        //unlock log
        return;
    }
    map<int, Account>::iterator it_trg = accounts.find(trg_acc_num);
    if (it_trg == accounts.end()) {
        //lock log
        logFile << "Error " << atm_id << ": Your transaction failed - account id " << trg_acc_num << " does not exist" << endl;
        //unlock log
        return;
    }
    if (it_src->second.password != src_acc_password) {
        //lock log
        logFile << "Error " << atm_id << ": Your transaction failed - password for account id " << src_acc_num << " is incorrect" << endl;
        //unlock log
        return;
    }
    
    ///// Now pseudo code for locking sequence
    // if (src_acc_num < trg_acc_num)
        //  LOCK SRC THAN TRG - WRITER
    // else
        // LOCK TRG THAN SRC  -WRITER

    if(amount > it_src->second.balance){
        // lock log
        logFile << "Error " << atm_id << ": Your transaction failed - account id " << src_acc_num 
        << " balance is lower than " << amount << endl;
        // unlock log
        // AND RELEASE THE LOCKS IN THE CORRECT ORDER!!
        return;
    }
    it_src->second.balance -= amount;
    it_trg->second.balance += amount;
    // lock log
    logFile << atm_id << ": Transfer "<< amount << " from account " << src_acc_num 
    << " to account " << trg_acc_num << " new account balance is " << it_src->second.balance 
    << " new target account balance is " << it_trg->second.balance << endl; 
    // unlock log
    // if (src_acc_num < trg_acc_num)
        //  UNLOCK SRC THAN TRG - WRITER
    // else
        // UNLOCK TRG THAN SRC  - WRITER
    // unlock bank - READER
}












/************************************************/
/*******************Account**********************/
/************************************************/


Account::Account() {
    if (pthread_mutex_init(&this->acc_read_mutex, NULL) != 0) {
        perror("Bank error : pthread_mutex_init failed");
        exit(0);
    }
    if (pthread_mutex_init(&this->acc_write_mutex, NULL) != 0) {
        perror("Bank error : pthread_mutex_init failed");
        exit(0);
    }
};	//need to fix

Account::Account(int password, int balance, int num_readers)
    : password(password), balance(balance), num_readers(num_readers) {
    if (pthread_mutex_init(&this->acc_read_mutex, NULL) != 0) {
        perror("Bank error : pthread_mutex_init failed");
        exit(0);
    }
    if (pthread_mutex_init(&this->acc_write_mutex, NULL) != 0) {
        perror("Bank error : pthread_mutex_init failed");
        exit(0);
    }
}

Account::~Account() {
    if (pthread_mutex_destroy(&this->acc_read_mutex) != 0) {
        perror("Bank error : pthread_mutex_destroy failed");
        exit(0);
    }
    if (pthread_mutex_destroy(&this->acc_write_mutex) != 0) {
        perror("Bank error : pthread_mutex_destroy failed");
        exit(0);
    }
};

/************************************************/
/****************help functions******************/
/************************************************/

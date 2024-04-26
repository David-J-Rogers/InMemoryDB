#include <unordered_map>
#include <stdexcept>
#include <iostream>

using namespace std;

class transactionDatabase {
private:
    unordered_map<string, int> finalStore;
    unordered_map<string, int> tempStore;
    bool inTransaction;

public:
    transactionDatabase() : inTransaction(false){}

    int* get(const string& key){
        auto it = finalStore.find(key);
        if(it != finalStore.end()){
            return &(it->second);
        }
        else{
            return nullptr;
        }
    }

    void put(const string& key, int val){
        if(inTransaction){
            tempStore[key] = val;
        }
        else{
            throw logic_error("No transaction in progress to add value");
        }
    }

    void begin_transaction(){
        if(!inTransaction){
            inTransaction = true;
            tempStore.clear();
        }
        else{
            throw logic_error("Transaction already in progress");
        }
    }

    void commit(){
        if(inTransaction){
            finalStore.insert(tempStore.begin(), tempStore.end());
            tempStore.clear();
            inTransaction = false;
        }
        else{
            throw logic_error("No transaction in progress to commit");
        }
    }

    void rollback(){
        if(inTransaction){
            tempStore.clear();
            inTransaction = false;
        }
        else{
            throw logic_error("No transaction in progress to rollback");
        }
    }
};

int main(){
    transactionDatabase transDB;

    int* valueA = transDB.get("A");
    if(valueA != nullptr){
        cout << *valueA << endl;
    }
    else{
        cout << "NULL" << endl;
    }

    try{
        transDB.put("A", 5);
    }
    catch(const logic_error& e){
        cout << e.what() << endl;
    }

    transDB.begin_transaction();

    transDB.put("A", 5);

    int* valueAAfterPut = transDB.get("A");
    if(valueAAfterPut != nullptr){
        cout << *valueAAfterPut << endl;
    }
    else{
        cout << "NULL" << endl;
    }

    transDB.put("A", 6);

    transDB.commit();

    int* valueAAfterCommit = transDB.get("A");
    if(valueAAfterCommit != nullptr){
        cout << *valueAAfterCommit << endl;
    }
    else{
        cout << "NULL" << endl;
    }

    try{
        transDB.commit();
    }
    catch(const logic_error& e){
        cout << e.what() << endl;
    }

    try{
        transDB.rollback();
    }
    catch(const logic_error& e){
        cout << e.what() << endl;
    }

    int* valueB = transDB.get("B");
    if(valueB != nullptr){
        cout << *valueB << endl;
    }
    else{
        cout << "NULL" << endl;
    }

    transDB.begin_transaction();

    transDB.put("B", 10);

    transDB.rollback();

    int* valueBAfterRollback = transDB.get("B");
    if(valueBAfterRollback != nullptr){
        cout << *valueBAfterRollback << endl;
    }
    else{
        cout << "NULL" << endl;
    }

    return 0;
}

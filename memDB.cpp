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

    int get(const string& key){
        if(finalStore.find(key) != finalStore.end()){
            return finalStore[key];
        }
        else{
            return -1;
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

    cout << transDB.get("A") << endl;

    try{
        transDB.put("A", 5);
    }
    catch(const logic_error& e){
        cout << e.what() << endl;
    }

    transDB.begin_transaction();

    transDB.put("A", 5);

    cout << transDB.get("A") << endl;

    transDB.put("A", 6);

    transDB.commit();

    cout << transDB.get("A") << endl;

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


    cout << transDB.get("B") << endl;

    transDB.begin_transaction();

    transDB.put("B", 10);

    transDB.rollback();

    cout << transDB.get("B") << endl;


    return 0;
}
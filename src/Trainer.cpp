//
// Created by eshm on 09/11/2021.
//

#include <iostream>
#include "../include/Trainer.h"


Trainer::Trainer(int t_capacity): capacity(t_capacity), open(false), trainer_ID(), customersList(), orderList(), salary(0) {}


int Trainer::getCapacity() const {
    return capacity;
};

void Trainer::addCustomer(Customer* customer){ //adds given customer to the customerList only if the trainer's capacity if not full
    if (static_cast<int>(customersList.size()) < capacity)
        customersList.push_back(customer);
    else
        std::cout << "Trainer's capacity is full" << std::endl;
}


void Trainer::removeCustomer(int id) {
    bool removed = false;
    for (int i = 0; (i < static_cast<int>(customersList.size())) & !removed; ++i) { //search for the customer to remove
        if (customersList[i]->getId() == id) {
            removed  = true;
            customersList.erase(customersList.begin()+i); // remove said customer
            break;
        }
    }
    if(!removed) {
        std::cout <<"no Customer with the given ID" << std::endl; //if the Customer doesn't exist
    }

}

Customer* Trainer::getCustomer(int id){ //returns a pointer to the given customer's id.
    for (int i = 0; i < static_cast<int>(customersList.size()); ++i) {
        if(customersList[i]->getId() == id)
            return customersList[i];
    }
    std::cout << "customer doesn't exist" << std::endl; //If customer doesn't exist returns null.
    return nullptr;

}

std::vector<Customer*> &Trainer::getCustomers(){return customersList;};


std::vector<OrderPair>& Trainer::getOrders() {
    return orderList;
}


void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options){
    for (int i = 0; i < static_cast<int>(workout_ids.size()); ++i) { //Goes through the list of the given workout ids
        OrderPair addPair(customer_id, workout_options[workout_ids[i]]); //Adds each given workout from the workout_options list to the orderList as a pair with the appropriate customer_od
        orderList.push_back(addPair);
    }
}


void Trainer::openTrainer() {
    open = true;
}

void Trainer::closeTrainer() {
    open = false;
    for (int i = 0; i < static_cast<int>(getOrders().size()); ++i) { //suming up the prices of all the trainer's listed workouts
        salary = salary + getOrders()[i].second.getPrice();
    }
    //prints the trainer's salary"
    std::cout << "Trainer " + std::to_string(this->trainer_ID) +  " closed. Salary " + std::to_string(this->salary) + "NIS" <<std::endl;

    resetOrders(); //clears the trainers order vector
    resetCustomers(); // clears the trainers customers vector
}

int Trainer::getSalary() {
    return salary;
}


bool Trainer::isOpen() {
    return open;
}

void Trainer::setTrainerID(int id){
    trainer_ID = id;
}

void Trainer::addOrder(OrderPair pair){
    orderList.push_back(pair);
}

void Trainer::clearOrders(){
    orderList.clear();
}


void Trainer::resetOrders() {
    orderList.clear();
}
void Trainer::resetCustomers() {
    for(Customer *c : customersList){
        if(c){
            delete(c);
            c = nullptr;
        }
    }
    customersList.clear();
}



///Rule of 5:///////////////////////
Trainer::~Trainer() { //destructor
    for (int i = 0; i < static_cast<int>(getCustomers().size()); i++) {
        if (customersList[i]){
            delete (customersList[i]);
            customersList[i] = nullptr;
        }
    }
    customersList.clear();
    orderList.clear();
}

Trainer::Trainer(const Trainer &other) {//copy constructor
    this->capacity = other.capacity;
    this->open = other.open;
    trainer_ID = other.trainer_ID;
    salary = other.salary;
    for (int i = 0; i < static_cast<int>(other.customersList.size()); ++i)
        customersList.push_back(other.customersList[i]->clone());
    for (int i = 0; i < static_cast<int>(other.orderList.size()); ++i)
        orderList.push_back(other.orderList[i]);

}

Trainer &Trainer::operator=(const Trainer &other) {//copy assignment operator
    if(this != &other) {
        for(Customer* customer : customersList){
            if(customer)
                delete customer;
        }

        orderList.clear();
        customersList.clear();

        this->capacity = other.capacity;
        this->open = other.open;
        trainer_ID = other.trainer_ID;
        salary = other.salary;
        for (int i = 0; i < static_cast<int>(other.customersList.size()); ++i)
            customersList.push_back(customersList[i]->clone());
        for (int i = 0; i < static_cast<int>(other.orderList.size()); ++i)
            orderList.push_back(other.orderList[i]);
    }
    return *this;
}

Trainer::Trainer(Trainer && other) {//move constructor
    capacity = other.capacity;
    open = other.open;
    trainer_ID = other.trainer_ID;
    salary = other.salary;
    for (Customer *c : other.customersList){
        customersList.push_back(c);
        c = nullptr;
    }
    for(OrderPair pair : other.orderList) {
        orderList.push_back(pair);
    }
}

Trainer &Trainer::operator=(Trainer &&other) {//move assignment operator
    if(this != &other) {
        for(Customer* customer : customersList){
            if(customer)
                delete customer;
        }
        orderList.clear();
        customersList.clear();

        this->capacity = other.capacity;
        this->open = other.open;
        trainer_ID = other.trainer_ID;
        salary = other.salary;
        for (Customer *c : other.customersList){
            customersList.push_back(c);
            c = nullptr;
        }
        for(OrderPair pair : other.orderList) {
            orderList.push_back(pair);
        }
    }
    return *this;
}
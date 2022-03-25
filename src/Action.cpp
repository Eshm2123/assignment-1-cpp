//
// Created by alon on 07/11/2021.
//

#include "../include/Action.h"
#include "../include/Studio.h"

extern Studio *backup;

BaseAction::BaseAction(): status() {}

ActionStatus BaseAction::getStatus() const{
    return status;
}
BaseAction::~BaseAction() = default;


void BaseAction::complete() {
    status = COMPLETED;
};

void BaseAction::error(std::string errorMsg_) {
    BaseAction::status = ERROR;
    BaseAction::errorMsg = errorMsg_;
    std::cout << "Error: " + errorMsg << std::endl;
}

std::string BaseAction::getErrorMsg() const{
    return errorMsg;
}







//////////////////////////////////OpenTrainer///////////////////////////////////////////

OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList): trainerId(id), customers(customersList) {};


void OpenTrainer::act(Studio &studio) {
    Trainer *t = studio.getTrainer(this->trainerId); //getting a pointer to the trainer with that ID
    if(t == nullptr || t->isOpen()) { //checking for errors that could occur
        error("Trainer does not exist or is not open");
    }else {
        int capacity = t->getCapacity();
        for (int i = 0; i < static_cast<int>(customers.size()) && capacity > static_cast<int>(t->getCustomers().size()); ++i) { //adding the customers to the trainer's list
            t->addCustomer(customers[i]->clone());
        }
        t->openTrainer(); //changing the flag status of the trainer
        complete();
    }
}
std::string OpenTrainer::toString() const {
    return getErrorMsg();
}
OpenTrainer* OpenTrainer::clone(){ //this clone uses the copy constructor
    std::vector<Customer*> copyCustomer;
    for(Customer* c : this->customers){
        copyCustomer.push_back(c->clone());
    }
    OpenTrainer* copy = new OpenTrainer(this->trainerId, copyCustomer); //creates a new customer so that the clone will be a new pointer on the heap
    return copy;
}
OpenTrainer::~OpenTrainer() {
    for(Customer* c : customers){
        if(c) {
            delete (c);
            c = nullptr;
        }
    }
    customers.clear();
}







////////////////////////////////// Order //////////////////////////////////////////////


Order::Order(int id) : trainerId(id){};

void Order::act(Studio &studio){
    //if the trainer does not exist or the session is not open: error
    Trainer *t = studio.getTrainer(trainerId);
    if (t == nullptr || !t->isOpen())
        error("Trainer does not exist or is not open");
    else{
        std::vector<Customer*> customers = t->getCustomers();
        std::vector<Workout> workout_options = studio.getWorkoutOptions();
        //Starts the workout session  for the customers of the given trainer.
        for (int i = 0; i < static_cast<int>(t->getCustomers().size()); ++i) {
            t->order(customers[i]->getId(), customers[i]->order(workout_options), workout_options);
        }
        //Goes through each customer and prints the name of the customer  and the type of workout that he ordered
        std::vector<OrderPair> orders = t->getOrders();
        for (int i = 0; i < static_cast<int>(customers.size()); ++i) {
            for (int j = 0; j < static_cast<int>(orders.size()); ++j) {
                if(customers[i]->getId() == orders[j].first)
                    std::cout << customers[i]->getName() + " Is Doing " + t->getOrders()[j].second.getName() << std::endl;
            }
        }

        complete();
    }
}
std::string Order::toString() const {
    return getErrorMsg();
}
Order* Order::clone(){
    return new Order(this->trainerId);
}
Order::~Order() =default;


//////////////////////////////////MoveCustomer///////////////////////////////////////////////


MoveCustomer::MoveCustomer(int src, int dst, int customerId): srcTrainer(src), dstTrainer(dst), id(customerId){};


void MoveCustomer::act(Studio &studio) {
    Trainer *st = studio.getTrainer(this->srcTrainer); //getting the trainers and customer
    Trainer *dt = studio.getTrainer(this->dstTrainer);
    if((st == nullptr) | (dt == nullptr)) return error("Cannot move customer");
    Customer *c = st->getCustomer(id);
    if(c == nullptr || !st->isOpen() || !dt->isOpen() //checking for potential errors that may occur
       || dt->getCapacity() <= static_cast<int>(dt->getCustomers().size())){
        error("Cannot move customer");
    }
    else {
        st->removeCustomer(id); //removing customer from source trainer
        dt->addCustomer(c); // adding customer to destination trainer
        std::vector<OrderPair> sto = st->getOrders();
        std::vector<OrderPair> dto = dt->getOrders();
        std::vector<OrderPair> Nsto; //this vector will hold all the orders of the source trainer except for the one being moved
        for (int i = 0; i < static_cast<int>(sto.size()); ++i) { //going through the orders list and moving from source to destination
            if(sto[i].first == id) {
                dt->addOrder(sto[i]); //adding the customer workouts to the destination trainer
            }
            else {
                Nsto.push_back(sto[i]); //making a copy in order to create a new order vector without the moved customer orders
            }
        }


        st->clearOrders(); //clearing the order pair vector to rebuild it without the moved customer
        for (int i = 0; i < static_cast<int>(Nsto.size()); ++i) {
            st->addOrder(Nsto[i]); //updating the source trainer order pair vector without the moved customer's orders
        }
        if(st->getCustomers().empty()) { //closing the trainer if there's no customers left
            st->closeTrainer();
        }
        complete();
    }
}

std::string MoveCustomer::toString() const {
    return getErrorMsg();
}
MoveCustomer* MoveCustomer::clone(){

    return new MoveCustomer(this->srcTrainer, this->dstTrainer, this->id);
}
MoveCustomer::~MoveCustomer() = default;






//////////////////////////////////Close///////////////////////////////////////////////


Close::Close(int id) : trainerId(id){}


void Close::act(Studio& studio){
    //if the trainer does not exist or the session is not open: error
    Trainer *t = studio.getTrainer(trainerId);
    if (t == nullptr || !t->isOpen())
        error("Trainer does not exist or is not open");
    else{
        t->closeTrainer(); //closes the trainer's session and updates the trainer's salary by including the now closed workout sessions
        complete();
    }
}

std::string Close::toString() const {
    return getErrorMsg();
}
Close* Close::clone(){
    return new Close(this->trainerId);
}
Close::~Close() =default;







//////////////////////////////////CloseAll/////////////////////////////////////



CloseAll::CloseAll() {}

void CloseAll::act(Studio &studio) {
    for (int i = 0; i < studio.getNumOfTrainers(); ++i) { //goes through each trainer and closes his session
        Trainer *t = studio.getTrainer(i);
        if (!(t == nullptr || !t->isOpen())) { //does this only if the trainer exists or the session is not open
            t->closeTrainer(); //closes the trainer's session and updates the trainer's salary by including the now closed workout sessions
        }
    }
    complete();
}

std::string CloseAll::toString() const {
    return getErrorMsg();
}
CloseAll* CloseAll::clone(){
    return new CloseAll();
}
CloseAll::~CloseAll() =default;






//////////////////////////////////PrintWorkoutOptions////////////////////////////////


PrintWorkoutOptions::PrintWorkoutOptions() {}

void PrintWorkoutOptions::act(Studio &studio) {
    std::vector<Workout> wo = studio.getWorkoutOptions(); //getting the available workout options vector
    for (int i = 0; i < static_cast<int>(wo.size()); ++i) { //printing the available workouts
        WorkoutType work = wo[i].getType();
        std::string s;
        if(work == 0){
            s = "Anaerobic";
        } else  if (work == 1){
            s = "Mixed";
        } else
            s = "Cardio";

        std::cout << wo[i].getName() +", " + s + ", " + std::to_string(wo[i].getPrice()) <<std::endl;
    }
    complete();
}

std::string PrintWorkoutOptions::toString() const {
    return getErrorMsg();
}
PrintWorkoutOptions* PrintWorkoutOptions::clone(){
    return new PrintWorkoutOptions();
}
PrintWorkoutOptions::~PrintWorkoutOptions() =default;





//////////////////////////////////PrintTrainerStatus////////////////////////////////


PrintTrainerStatus::PrintTrainerStatus(int id): trainerId(id) {}


void PrintTrainerStatus::act(Studio &studio) {
    Trainer *t = studio.getTrainer(trainerId); //getting the trainer that the function called for
    if(!t->isOpen()) { //checking for the trainer status
        std::cout << "Trainer " + std::to_string(trainerId) + " status: closed" << std::endl; //if closed print accordingly
    } else {
        std::cout << "Trainer " + std::to_string(trainerId) + " status: open" << std::endl; //else is open and print
        std::cout << "Customers:" <<std::endl;
        std::vector<Customer*> cus = t->getCustomers(); //going throu the trainer's client list and printing them by ID + Name
        for (int i = 0; i < static_cast<int>(cus.size()); ++i) {
            std::cout << std::to_string(cus[i]->getId()) + " " + cus[i]->getName() <<std::endl;
        }
        std::vector<OrderPair> to = t->getOrders(); //getting the trainer's order pairs
        std::cout << "Orders:" <<std::endl;
        for (int i = 0; i < static_cast<int>(to.size()) ; ++i) { // printing the exercises the trainer preform with the customer with the correct ID
            std::cout << to[i].second.getName() + " "
                         + std::to_string(to[i].second.getPrice()) + "NIS " + //printing as required: workout name, price in NIS and customer ID
                         std::to_string(to[i].first) <<std::endl;
        }
        int sum = 0;
        for (int i = 0; i < static_cast<int>(to.size()); ++i) {
            sum = sum + to[i].second.getPrice();
        }
        sum = sum + t->getSalary();
        std::cout << "Current Trainer's Salary: " + std::to_string(sum) + "NIS" <<std::endl; //printing the total salary the trainer acquired
    }
    complete();

}
std::string PrintTrainerStatus::toString() const {
    return getErrorMsg();
}
PrintTrainerStatus* PrintTrainerStatus::clone(){
    return new PrintTrainerStatus(this->trainerId);
}
PrintTrainerStatus::~PrintTrainerStatus() =default;



//////////////////////////////////PrintActionsLog////////////////////////////////


PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Studio &studio){
    for (int i = 0; i < static_cast<int>(studio.getActions_String().size()); ++i) { //goes through each one of the given input lines
        if(studio.getActionsLog()[i]->getStatus() == COMPLETED) //prints the appropriate message depending on if the action was completed or not.
            std::cout << studio.getActions_String()[i] + " Completed" << std::endl;
        else
            std::cout << studio.getActions_String()[i]  + " Error: " + studio.getActionsLog()[i]->toString() << std::endl;
    }
    complete();
}
std::string PrintActionsLog::toString() const {
    return getErrorMsg();
}
PrintActionsLog* PrintActionsLog::clone(){
    return new PrintActionsLog();
}
PrintActionsLog::~PrintActionsLog() =default;



//////////////////////////////////BackupStudio////////////////////////////////


BackupStudio::BackupStudio() {}

void BackupStudio::act(Studio &studio){
    if (backup != nullptr){ //checks if we had already backed up the studio.
        delete backup;       // if we did then we would want to delete the last one to avoid memory leaks and then create a new backup
    }
    backup = new Studio(studio); //uses the copy constructor

    complete();
}
std::string BackupStudio::toString() const {
    return getErrorMsg();
}

BackupStudio* BackupStudio::clone(){
    return new BackupStudio();
}
BackupStudio::~BackupStudio() =default;


//////////////////////////////////RestoreStudio////////////////////////////////


RestoreStudio::RestoreStudio(){};

void RestoreStudio::act(Studio &studio) {
    if(backup) {
        studio = *backup; //uses the copy assignment operator
    } else {
        error("No backup available");
    }

}


std::string RestoreStudio::toString() const {
    return getErrorMsg();
}
RestoreStudio* RestoreStudio::clone(){
    return new RestoreStudio();
}
RestoreStudio::~RestoreStudio() =default;

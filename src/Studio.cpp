//
// Created by alon on 06/11/2021.
//
#include "../include/Studio.h"
#include <iostream>



Studio::Studio() : open(), customerID_cnt(0){};


Studio::Studio(const std::string &configFilePath) : open(), customerID_cnt() {
    //next few lines are to open the input file, and insert it into a string variable called lines:
    std::ifstream input;
    input.open(configFilePath);
    if(!input.is_open())  {
        std::cout<<"file not open"<<std::endl; }
    std::vector<std::string> lines;
    while(!input.eof()){
        std::string line;
        getline(input, line);
        lines.push_back(line);

    }
    input.close();

    std::string trainerCapacity;
    int i = 0;
    int trainerID = 0;
    //This loop creates the trainers:
    while (i < static_cast<int>(lines[1].length())){ //going through the vector line that indicates the capacity of the trainers
        if(lines[1][i] == ','){ //Making sure that we get the whole number of the trainer's capacity and not only the first digit.
            Trainer *t = new Trainer (std::stoi(trainerCapacity)); //creating and adding each trainer to the trainers vector
            t->setTrainerID(trainerID);
            trainers.push_back(t); //vector location indicates trainers ID

            i = i + 1;
            trainerID++;
            trainerCapacity = ""; //If we entered this block it means that the trainer's capacity has been fully copied,
                                    // and we can move on to the next trainer
        } else { //Trainer's capacity has more than 1 digit.
            trainerCapacity = trainerCapacity + lines[1][i];//appending the next number in the line order to find
            i = i + 1;                                      //the capacity of trainer
        }
    }
    //Adding the last trainer in the list
    Trainer *t = new Trainer (std::stoi(trainerCapacity)); //creating and adding each trainer to the trainers vector
    t->setTrainerID(trainerID);
    trainers.push_back(t); //vector location indicates trainers ID

    i = 2;
    int WorkoutIDc = 0; //Workout ID counter
    while(i < static_cast<int>(lines.size())){
        std::string name;
        int location = 0; //keeps track of what i've passed through
        for (int j = 0; (j < (static_cast<int>(lines[i].length()))) & (lines[i][j] != ','); ++j) { //passing on the string
            name = name + lines[i][j]; //appending the first section of the string until the "," to create the name
            location++;
        }
        location = location + 1; // adds one to location in order to jump over the space
        std::string type;
        for (int j = location + 1; (j < static_cast<int>(lines[i].length())) & (lines[i][j] != ','); ++j) {
            type = type + lines[i][j]; //appending to create a string for the workout type
            location++;
        }
        location = location + 1; // adds one to location in order to jump over the space
        std::string price;
        for (int j = location + 1; (j < static_cast<int>(lines[i].length())) & (lines[i][j] != ','); ++j) {
            price = price + lines[i][j]; //creating a string for the price
        }
        WorkoutType wt; //checking for the correct workout type to replace to enum
        if(type == "Anaerobic")
            wt = ANAEROBIC;
        else if(type == "Mixed")
            wt = MIXED;
        else
            wt = CARDIO;
        Workout w (WorkoutIDc,name,std::stoi(price),wt); //creating a workout object
        workout_options.push_back(w); //inserting the workout to the workout_options vector
        WorkoutIDc = WorkoutIDc + 1; //increasing the counts
        i = i + 1;
    }


}




void Studio::start() {
    open = true;

    std::cout <<"Studio is now open!!!"<< std::endl;
    std::cout <<"Please enter a command"<< std::endl;

    std::string input;
    getline(std::cin, input);

    while (input != "closeall") { // loop through all of the user's commands until he inserts the closeall call
        //next variable will hold a string of the first word of the users input
        std::string first;
        int i=0;
        while((i < static_cast<int>(input.length())) & (input[i] != ' ')){
            first = first + input[i];
            i++;
        }

        std::string copyInput = input;

        if(first == "open"){ //This means the user had inserted a "OpenTrainer" command

            input = input.substr(first.length() + 1); //Cuts the first word of the input so we can focus on the rest of the input
            //The next few lines create a variable that holds the trainer's id
            std::string string_id;
            i = 0;
            while((i < static_cast<int>(input.length())) & (input[i] != ' ')){
                string_id = string_id + input[i];
                i++;
            }
            int trainer_id = std::stoi(string_id); //Converts the number of the trainer's id from string to int for later use.


            input = input.substr(string_id.length() + 1);//Cuts the next word of the input so we can focus on the rest of the input
            i = 0;
            std::vector<Customer*> customerList;
            //This loop goes over each of the given new customer and assigns him to the appropriate list.
            while(i < static_cast<int>(input.length())){
                //Next few lines create a variable with the name of the next customer
                std::string name;
                int j = 0;
                while(input[j] != ','){
                    name = name + input[j];
                    j++;
                    i = i + 1;
                }
                //Next few lines create a variable with the ordering strategy of this customer.
                std::string type;
                j = j + 1;
                i = i + 1;
                while((i < static_cast<int>(input.length())) & (input[j] != ' ') ){
                    type = type + input[j];
                    j++;
                    i = i + 1;
                }
                //Create a new customer of the correct type.
                Customer* c;

                if(type == "swt")
                    c = new SweatyCustomer(name, customerID_cnt);
                else if(type =="chp")
                    c = new CheapCustomer(name, customerID_cnt);

                else if(type =="mcl")
                    c = new HeavyMuscleCustomer(name, customerID_cnt);

                else if(type =="fbd")
                    c = new FullBodyCustomer(name, customerID_cnt);
                else {
                    std::cout <<"Illegal input" <<std::endl;

                }
                customerList.push_back(c);// Adds the customer to our customerList
                customerID_cnt = customerID_cnt + 1; //Updates the customer count

                //After using the information for this customer, we cut the given string input to continue to the next customer.
                if(static_cast<int>(input.size()) != j){
                    input = input.substr(j + 1);
                    i = 0;
                }
            }
            //create a new base action and acts on it.
            OpenTrainer *newSession = new OpenTrainer(trainer_id, customerList);

            newSession->act(*this);

            actionsLog.push_back(newSession); //push the new base action into actionsLog for later use
        }
        else if(first == "order") {
            input = input.substr(first.length()+1,input.length());
            int t_id = std::stoi(input);

            Order *newOrder = new Order(t_id);
            newOrder->act(*this);
            actionsLog.push_back(newOrder);
        }
        else if (first == "move") {
            input = input.substr(first.length()+1,input.length()); //cuts the first word "move"
            int OT = stoi(input.substr(0,input.find(' '))); // cuts the origin trainer id and stores it in a variable
            input = input.substr(input.find(' ') + 1); //moves the input to the next number(skips the space)
            int DT =  stoi(input.substr(0,input.find(' ')));// cuts the destination trainer id and stores it in a variable
            input = input.substr(input.find(' ') + 1);//moves the input to the next number(skips the space)
            int CID = stoi(input.substr(0,input.length()));// cuts the customer's id and stores it in a variable

            MoveCustomer *mc = new MoveCustomer(OT,DT,CID);
            mc->act(*this);
            actionsLog.push_back(mc);
        }
        else if (first == "close") {
            input = input.substr(first.length()+1,input.length());

            Close *c = new Close(std::stoi(input));
            c->act(*this);
            actionsLog.push_back(c);
        }
        else if (first == "workout_options") {
            PrintWorkoutOptions *pwo = new PrintWorkoutOptions();
            pwo->act(*this);
            actionsLog.push_back(pwo);
        }
        else if (first == "status") {
            input = input.substr(first.length()+1,input.length());

            PrintTrainerStatus *pts = new PrintTrainerStatus(std::stoi(input)) ;
            pts->act(*this);
            actionsLog.push_back(pts);
        }
        else if (first == "log") {
            PrintActionsLog *pal = new PrintActionsLog();
            pal->act(*this);
            actionsLog.push_back(pal);
        }
        else if (first == "backup") {
            BackupStudio *bs = new BackupStudio();
            actionsLog.push_back(bs);
            bs->act(*this);
        }
        else if (first == "restore") {
            RestoreStudio *rs = new RestoreStudio();
            actionsLog.push_back(rs);
            rs->act(*this);
        }
        actions_String.push_back(copyInput); //inserts the input to a field calles actionsString for later use (printActionsLog)
        getline(std::cin, input);

    }

    if(input == "closeall"){
        CloseAll closeStudio;
        closeStudio.act(*this);
        open = false;
    }



}

int Studio::getNumOfTrainers() const{
    return trainers.size();
}

Trainer* Studio::getTrainer(int tid){
    if(tid > static_cast<int>(trainers.size())-1)
        return nullptr;

    else
        return trainers[tid];
}

const std::vector<BaseAction*>& Studio::getActionsLog() const{
    return actionsLog;
}

std::vector<std::string> Studio::getActions_String(){
    return actions_String;
}

std::vector<Workout>& Studio::getWorkoutOptions(){
    return workout_options;
}


///------------Rule of 5-------------///
//--------Destructor--------//
Studio::~Studio() {
    for(Trainer* trainer:trainers){
        delete (trainer);
        trainer = nullptr;
    }
    trainers.clear();
    workout_options.clear();
    for (BaseAction* baseAction:actionsLog) {
        delete(baseAction);
        baseAction = nullptr;
    }
    actionsLog.clear();
    actions_String.clear();
}
//---------------- copy constructor-------------//
Studio::Studio(const Studio &other) {
        for (BaseAction *baseAction: other.actionsLog) {
            actionsLog.push_back(baseAction->clone()); //creates a new baseaction by using the clone function.
        }
        for (Trainer *trainer: other.trainers) {//creates a new trainer by using the trainer class copy constructor.
            trainers.push_back(new Trainer(*trainer));
        }

        for (Workout workout: other.workout_options) {
            workout_options.push_back(workout);
        }
        for (std::string action: other.actions_String) {
            actions_String.push_back(action);
        }
        customerID_cnt = other.customerID_cnt;
        open = other.open;

}

//-------------copy assignment operator------------//
Studio& Studio::operator=(const Studio& other) {
    if(this == &other){
        return *this;
    }
    for(Trainer* trainer:trainers){
        if(trainer)
            delete (trainer);
    }
    for (BaseAction* baseAction:actionsLog) {
        if(baseAction)
            delete(baseAction);
    }
    trainers.clear();
    actionsLog.clear();
    workout_options.clear();
    actions_String.clear();
    for (BaseAction* baseAction:other.actionsLog){
        actionsLog.push_back(baseAction->clone());
    }
    for (Workout workout:other.workout_options){
        workout_options.push_back(workout);
    }
    for (Trainer* trainer:other.trainers){
        trainers.push_back(new Trainer(*trainer));
    }
    for(std::string action:other.actions_String){
        actions_String.push_back(action);
    }
    open = other.open;
    customerID_cnt = other.customerID_cnt;
    return *this;
}


//-----------------move constructor-----------//
Studio::Studio(Studio &&other) {
    for (BaseAction* baseAction:other.actionsLog){
        actionsLog.push_back(baseAction);
        baseAction = nullptr;
    }
    for (Workout workout:other.workout_options){
        workout_options.push_back(workout);
    }
    for (Trainer* trainer:other.trainers){
        trainers.push_back(trainer);
        trainer = nullptr;
    }
    for(std::string action:other.actions_String){
        actions_String.push_back(action);
    }
    open = other.open;
    customerID_cnt = other.customerID_cnt;
}


//------------- move assignment operator ---------//
Studio& Studio::operator=(Studio&& other){
    if(this == &other){
        return *this;
    }
    for(Trainer* trainer:trainers){
        if(trainer)
            delete (trainer);
    }
    for (BaseAction* baseAction:actionsLog) {
        if(baseAction)
            delete(baseAction);
    }
    workout_options.clear();
    actionsLog.clear();
    for (BaseAction* baseAction:other.actionsLog){
        actionsLog.push_back(baseAction);
        baseAction = nullptr;
    }
    for (Workout workout:other.workout_options){
        workout_options.push_back(workout);
    }
    for (Trainer* trainer:other.trainers){
        trainers.push_back(trainer);
        trainer = nullptr;
    }
    for(std::string action:other.actions_String){
        actions_String.push_back(action);
    }
    open = other.open;
    customerID_cnt = other.customerID_cnt;
    return *this;
}
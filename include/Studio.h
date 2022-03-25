#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"
#include "Customer.h"
#include "fstream"



class Studio{
public:
    Studio();
    Studio(const std::string &configFilePath);

    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<std::string> getActions_String(); //return a string of the actions history
    std::vector<Workout>& getWorkoutOptions();


    virtual ~ Studio(); //destructor
    Studio(const Studio &other); //copy constructor
    Studio &operator=(const Studio &other); //copy assignment operator
    Studio(Studio &&other); //move constructor
    Studio &operator=(Studio && other); //move assignment operator

private:
    bool open;
    int customerID_cnt;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    std::vector<std::string> actions_String; //a vector of strings which hold the actions names

};

#endif
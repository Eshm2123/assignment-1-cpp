//
// Created by eshm on 06/11/2021.
//

#include "../include/Customer.h"
#include <bits/stdc++.h>
#include <algorithm>
#include <array>
#include <vector>


Customer::Customer(std::string c_name, int c_id) : name(c_name), id(c_id) {};

std::string Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}
Customer::~ Customer() = default;







SweatyCustomer::SweatyCustomer(std::string name, int id): Customer(name,id){};


std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> ans;
    for (int i = 0; i < static_cast<int>(workout_options.size()); ++i) { //parsing the vector to find all the cardio workouts
        if(workout_options[i].getType() == CARDIO)
            ans.push_back(workout_options[i].getId()); //add all the cardio workouts ID's found to the answer vector
    }
    return ans;
}


std::string SweatyCustomer::toString() const {
    return nullptr;
}
SweatyCustomer* SweatyCustomer::clone(){
    return new SweatyCustomer(this->getName(), this->getId());;
}

SweatyCustomer::~ SweatyCustomer() = default;

////////////////////////////////////////////////CheapCustomer/////////////////////////////////////

CheapCustomer::CheapCustomer(std::string name, int id): Customer(name,id){};


std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> output;
    if(!workout_options.empty()){ //adds a variable to our output vector only if the workout_options vector isn't empty
        //now we will parse throughout given input vector and find the cheapest element of them all,
        // then stores the id of the appropriate element and insert it into our ouput vector
        int cheapest = (workout_options[0]).getPrice();
        int cheapest_id = workout_options[0].getId();
        for(int i = 1; i<static_cast<int>(workout_options.size()); i++) {
            int nextPrice = (workout_options[i]).getPrice();
            int next_id = (workout_options[i]).getId();
            if (cheapest > nextPrice) {
                cheapest = nextPrice;
                cheapest_id = next_id;
            }
        }
        output.push_back(cheapest_id);
    }
    return  output;
}


std::string CheapCustomer::toString() const {
    return nullptr;
}
CheapCustomer* CheapCustomer::clone(){
    return new CheapCustomer(this->getName(), this->getId());;
}
CheapCustomer::~ CheapCustomer() = default;


////////////////////////////////////////////////HeavyMuscleCustomer/////////////////////////////////////

HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id): Customer(name,id){};



std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> ans;
    std::vector<Workout> work;
    for (int i = 0; i < static_cast<int>(workout_options.size()); ++i) { //parsing the vector to find all the Anaerobic workouts
        if (workout_options[i].getType() == ANAEROBIC)
            work.push_back(workout_options[i]); //add all the Anaerobic workouts prices found to the answer vector
    }
    std::vector<std::pair<int,int>> prices_IDs;
    for (int i = 0; i < static_cast<int>(work.size()); ++i) {//making a new pair vector to sort by prices and accessing later on by ID's
        prices_IDs.push_back(std::make_pair(work[i].getPrice(),work[i].getId()));
    }
    std::sort(prices_IDs.begin(),prices_IDs.end()); //sorting by descending order
    for (int i = prices_IDs.size()-1; i > -1 ; --i) {
        ans.push_back(prices_IDs[i].second);
    }
    return ans;
}



std::string HeavyMuscleCustomer::toString() const {
    return nullptr;
}
HeavyMuscleCustomer* HeavyMuscleCustomer::clone(){
    return new HeavyMuscleCustomer(this->getName(), this->getId());;
}
HeavyMuscleCustomer::~ HeavyMuscleCustomer() = default;

////////////////////////////////////////////////FullBodyCustomer/////////////////////////////////////

FullBodyCustomer::FullBodyCustomer(std::string name, int id): Customer(name,id){};


std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> output;
    //next three "if" blocks are similar:
    //each one of them inserts into our output vector the cheapest/ most expensive type of workout type according to the instrucions.
    //they all parse through the given input's elements, search for the next appropriate workout type,
    //and change the appropriate variable accordingly (if it's cheaper or more expensive)
    //note: if there a specific wotkout type doesn't exist, no new element will be inserted into the output vector.

    //This one searches for the cheapest Cardio workout
    if(workout_options.size() > 0) { //checks if the workout_options vector isn't empty
        int cheapest_cardio = -1;
        int cheapest_cardioID = -1;
        for(int i = 0; i<static_cast<int>(workout_options.size()); i++) {
            if(workout_options[i].getType() == CARDIO) {
                int next_Cardio = workout_options[i].getPrice();
                if ((cheapest_cardio > next_Cardio) | (cheapest_cardio == -1)) {
                    cheapest_cardio = next_Cardio;
                    cheapest_cardioID = (workout_options[i]).getId();
                }
            }
        }
        if (cheapest_cardioID > 0) output.push_back(cheapest_cardioID);

        //This one searches for the most expensive Mixed workout
        int expensive_mixType = -1;
        int expensive_mixTypeID = -1;
        for(int i = 0; (i<static_cast<int>(workout_options.size())); i++) {
            if(workout_options[i].getType() == MIXED){
                int next_Mixed = workout_options[i].getPrice();
                if((expensive_mixType < next_Mixed) | (expensive_mixType == -1)){
                    expensive_mixType = next_Mixed;
                    expensive_mixTypeID = (workout_options[i]).getId();
                }
            }
        }
        if(expensive_mixTypeID > 0) output.push_back(expensive_mixTypeID);

        //This one searches for the cheapest ANAEROBIC workout
        int cheapest_anaerobic = -1;
        int cheapest_anaerobicID = -1;
        for(int i = 0; (i<static_cast<int>(workout_options.size())); i++) {
            if(workout_options[i].getType() == ANAEROBIC) {
                int next_anaerobic = workout_options[i].getPrice();
                if ((cheapest_anaerobic > next_anaerobic) | (cheapest_anaerobic == -1)) {
                    cheapest_anaerobic = next_anaerobic;
                    cheapest_anaerobicID = (workout_options[i]).getId();
                }
            }
        }
        if (cheapest_anaerobicID >= 0)
            output.push_back(cheapest_anaerobicID);


    }

    return output;

}



std::string FullBodyCustomer::toString() const {
    return nullptr;
}

FullBodyCustomer* FullBodyCustomer::clone(){
    return new FullBodyCustomer(this->getName(), this->getId());;
}

FullBodyCustomer::~ FullBodyCustomer() = default;




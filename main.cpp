#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

map<string, int> validCommands;

void setCommands(){
  validCommands.insert(pair<string, int>("Create_parking_lot", 1));
  validCommands.insert(pair<string, int>("Park", 2));
  validCommands.insert(pair<string, int>("Leave", 3));
  validCommands.insert(pair<string, int>("Slot_numbers_for_driver_of_age", 4));
  validCommands.insert(pair<string, int>("Slot_number_for_car_with_number", 5));
  validCommands.insert(pair<string, int>("Vehicle_registration_number_for_driver_of_age", 6));
}

int getCommandId(string &command){
    auto itr = validCommands.find(command);
    if(itr != validCommands.end()){
       return itr->second;
    }
    return -1;
}

class parkingLot {
   int slots;
   set<int> availableSlots;
   map<int, set<int> > slotNumberByDriverAge;
   map<string, int> slotNumberByRegistration;
   map<int, set<string> > registrationNumberByAge;

   public:
     parkingLot(){}
	 parkingLot(int numberOfSlots) {
	 // initialized parkingLot
             slots = numberOfSlots;

             for(int slot = 1; slot <= slots; slot++) {
                 availableSlots.insert(slot);
             } 
	 }
	
	 int parkVehicle(string registrationNumber, int driversAge) {
             // Park if possible and return status
	     if(availableSlots.empty()){
	 	// parkingLot is full, return -1 as stauts code;
	 	return -1;
	     } else{
		//alot nearest slot to entry point
		int slotId = *availableSlots.begin();
		availableSlots.erase(slotId);
		slotNumberByDriverAge[driversAge].insert(slotId);
		slotNumberByRegistration[registrationNumber] = slotId;
		registrationNumberByAge[driversAge].insert(registrationNumber);
		return slotId; // successfully alotted parking slot
	     }
	 }
	
	 vector<int> getSlotsByAge(int driversAge){
             vector<int> slotsByAge;
		 
             for(int slotId : slotNumberByDriverAge[driversAge]) {
                slotsByAge.push_back(slotId);
             }
             return slotsByAge;
	 }

	 int getSlotByRegistration(string registrationNumber){
             if(slotNumberByRegistration.find(registrationNumber) == slotNumberByRegistration.end()) {
         	return -1; //no vehicle has parked with given registration number;
	     }
             else {
         	 return slotNumberByRegistration[registrationNumber]; // return slot nubmer
	     }
	 }
	
	 pair<string, int> clearSlotByid(int slotId) {
	     string regNum = "NULL";
	     int driversAge;

             if(availableSlots.find(slotId) != availableSlots.end()) {
         	 return make_pair(regNum, driversAge); // slot is already cleared
	     }
             availableSlots.insert(slotId);
             for(auto itr = slotNumberByDriverAge.begin(); itr != slotNumberByDriverAge.end(); itr++) {
         	 if(itr -> second.count(slotId)) {
		     driversAge = itr->first;
		     itr->second.erase(slotId);
		     break;
         	 }
             }
         
             for(auto itr = slotNumberByRegistration.begin(); itr != slotNumberByRegistration.end(); itr++) {
         	 if(itr->second == slotId) {
         	     regNum = itr->first;
         	     slotNumberByRegistration.erase(itr);
         	     break;
         	 }
             }

             auto it = registrationNumberByAge[driversAge].find(regNum);
             registrationNumberByAge[driversAge].erase(it);

             return make_pair(regNum, driversAge);
	 }

	 vector<string> getRegistraionNumbersByAge(int driversAge) {
             vector<string> regtistrationNumbers;
             for(auto &itr : registrationNumberByAge[driversAge]) {
         	  regtistrationNumbers.push_back(itr);
             }

             return regtistrationNumbers;
	 }
	~parkingLot(){};
	
};

parkingLot createParkingLot(int n) {
   parkingLot p(n);
   return p;
}

bool isNumber(string &num) {
    for(char c: num) {
        if(!(c >= '0' && c <= '9'))
	    return false;
    }
    return true;
}

bool isValid(vector<string> command) {
    int commandType = getCommandId(command[0]);
    
    if(commandType == -1)
        return false;
    
    if(commandType == 1 || commandType == 3 || commandType == 4 || commandType == 6) {
        int commandLength = command.size();
    	return commandLength == 2 && isNumber(command[1]);
    }
	
    if(commandType == 5){
    	return (int)command.size() == 2;
    }

    // for Park command
    return (int)command.size() == 4 && command[2] == "driver_age" 
            && isNumber(command[3]);
}

int main() { 
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

  
    setCommands(); // mapping valids commands to intergers for sake of simplicity

    string command;
    bool isCreated = false; // true if a parking lot is created, else false
    int status; // status of commands
    parkingLot PL; // parking lot object
    
    while (getline(cin, command)) {
        // below logic handles string commands
        vector<string> commandWithParams;
        string eachWord;
        stringstream ss;
        ss << command;
        while(ss >> eachWord) {
            commandWithParams.push_back(eachWord);
        }
        command.clear();
         
	// invalid command. please note commands are case sensitive
        if(!isValid(commandWithParams)) { 
    	    cout << "INVALID COMMAND! PLEASE TRY AGAIN.";
    	    exit(0);
        }
    
        int commandId = getCommandId(commandWithParams.front());
        string regNum;
        int driversAge, slotId;
        pair<string,int> vehicle;
        vector<int> slotsByAge;
        vector<string> vehiclesByAge;
    
	switch(commandId) {
    	    case 1:
    	    // Create parking lot
    	    if(isCreated) {
    	        cout << "Parking lot has already been created" << endl;
    	    } else {
            	isCreated = true;
                PL = createParkingLot(stoi(commandWithParams[1]));
                cout << "Created parking of " << commandWithParams[1] << " slots." << endl;
            }
    	    break;
    	    case 2:
    	    // Park vehicle
    	        regNum = commandWithParams[1];
    	        driversAge = stoi(commandWithParams[3]);
                status = PL.parkVehicle(regNum, driversAge);
                if(status == -1) {
            	    cout << "Sorry! Parking Lot is full." << endl;
                } else {
            	// succesfully parked, status has slot id
            	    cout << "Car with vehicle registration number " << regNum << " has been parked at slot number " << status << endl;
                }
    		break;
    	    case 3:
    	    // Leave from parking
    	    
                vehicle = PL.clearSlotByid(stoi(commandWithParams[1]));
                if(vehicle.first == "NULL") {
            	    cout << "Slot is already available." << endl;
                } else {
                    cout << "Slot number " << commandWithParams[1] << " vacated, the car with vehicle registration number " << vehicle.first <<" left the space, the driver of the car was of age " << vehicle.second << endl;
                }
    		break;
    	    case 4:
    	    // Get slot numbers for give age drivers
    	        driversAge = stoi(commandWithParams[1]);
                slotsByAge = PL.getSlotsByAge(driversAge);
                if(slotsByAge.empty()) {
            	    cout << "No driver with age " << driversAge << endl;
                } else {
            	    for(int i = 0; i < slotsByAge.size(); i++) {
            	        cout << slotsByAge[i];
            		if(i + 1 < slotsByAge.size())
            		    cout << ",";
            	    }
                    cout << endl; 
                }
    		break;
    	    case 5:
    	    // Get slot number for given registration nubmer
                slotId = PL.getSlotByRegistration(commandWithParams[1]);
                if(slotId == -1) {
            	    cout << "No car with given registration number" << endl;
                } else {
            	    cout << slotId << endl;
                }
    		break;
    	    case 6:
    	    // Get all vehicles reg. number with give age of drivers
                driversAge = stoi(commandWithParams[1]);
                vehiclesByAge = PL.getRegistraionNumbersByAge(driversAge);
                if(vehiclesByAge.empty()) {
                    cout << "null" << endl;
                }
                for(int i = 0; i < vehiclesByAge.size(); i++){
            	    cout << vehiclesByAge[i];
            	    if(i + 1 < vehiclesByAge.size())
            	        cout << ", ";
                }
    		break;
    	    default:
    	    // we won't reach here
    	        break;
        }
   }

  return 0;
}

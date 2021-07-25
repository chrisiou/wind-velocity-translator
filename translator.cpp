#include "include/velocities_to_beaufort.h"

int main(int argc, char* argv[]) {
    std::string filename;
    if (argc == 1) {
        std::cout << "Give input file: ";
        std::cin >> filename;
    } else if (argc > 1) { filename = argv[1];} 
    else { filename = "";}

    std::fstream file_handler;
    file_handler.open(filename, std::ios::in);
    if (not file_handler.is_open()) {
        std::cout << "Failing to open the file!\n" << std::endl;
        return 1;
    }

    std::ofstream file("beaufort.csv");
    file << "TIME,BEAUFORT,VELOCITY,MAX WAVE HEIGHT,SEA STATE\n";

    // initialize
    MinuteVelocity minute;
    std::string status = "";
    std::string prev_row, row; // e.g. format:2021-05-23 19:00:20,43428
    
    getline(file_handler, prev_row);
    minute.add_to_seconds_velocities_set(std::stod(prev_row.substr(20)));
    HourBeaufort hour(prev_row.substr(0,19)); // initialize timestamp

    while(getline(file_handler, row)) {
        if(less_than_an_hour(hour.get_time(), row.substr(0,19))) {
            if (prev_row.substr(14,2) == row.substr(14,2)) { // check if they are in the same minute
                minute.add_to_seconds_velocities_set(std::stod(row.substr(20)));

            } else {
                hour.add_velocity(minute.average_velocity(), prev_row.substr(0,19));
                minute.clear_seconds_velocities_set();
                minute.add_to_seconds_velocities_set(std::stod(row.substr(20)));
            }
        } else {
            hour.add_velocity(minute.average_velocity(), prev_row.substr(0,19));
            minute.clear_seconds_velocities_set();
            minute.add_to_seconds_velocities_set(std::stod(row.substr(20)));

            if (hour.get_code() != -1) {file << hour;}
            hour.set_time(row.substr(0,19));
        }
        prev_row = row;
    }

    file_handler.close();
    file.close();
    
    return 0;
}
